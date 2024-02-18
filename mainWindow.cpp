#include <thread>
#include <qtimer.h>
#include <qevent.h>
#include <qmetaobject.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qjsondocument.h>
#include <qdebug.h>

#include "mainWindow.h"

#include "GameThread.h"
#include "Common/GameOpt.h"
#include "Common/BallPossession.h"

mainWindow::mainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::mainWindowClass())
{
	ui->setupUi(this);
	this->setWindowIcon(QIcon(QString::fromUtf8(":/resource/resource/icons/乒乓球.png")));

	// 加载配置文件
	GameOpt opt;
	QFile opt_file(this->opt_file_name_);
	if (!opt_file.exists()) {
		qDebug() << "未发现配置文件，使用默认配置";
		GameOpt opt;//默认
		QJsonObject j_opt = save_opt_to_json(opt);
		QJsonDocument doc(j_opt);

		opt_file.open(QIODevice::ReadWrite);
		opt_file.write(doc.toJson());
		opt_file.close();
	}
	else {
		// 存在文件
		qDebug() << "发现配置文件，正在导入配置...";
		opt_file.open(QIODevice::ReadWrite);
		QByteArray data = opt_file.readAll();
		QJsonDocument doc = QJsonDocument::fromJson(data);
		QJsonObject j_opt;
		if (doc.isObject()) {
			j_opt = doc.object();
		}
		opt = read_opt_from_json(j_opt);
		opt_file.close();
		qDebug() << "配置加载完成";
	}

	game_win_ = new GameWindow(opt);
	ui->stackedWidget->addWidget(game_win_);

	help_win_ = new HelpWindow();
	ui->stackedWidget->addWidget(help_win_);

	setting_win_ = new SettingWindow(opt);
	ui->stackedWidget->addWidget(setting_win_);

	/* 标题 */
	// 新的一局，根据发球权修改窗口标题
	connect(game_win_->playground_, &Playground::a_new_round, this, [this]() {
		if (game_win_->playground_->possession() == BallPossession::LEFT) {
			this->setWindowTitle(QString("玩家1发球"));
		}
		else {
			this->setWindowTitle(QString("玩家2发球"));
		}
		}
	);
	// 开始游戏后，标题改变
	connect(game_win_, &GameWindow::gameStart, this, [this]() {
		this->setWindowTitle(QString("GO!"));
		}
	);

	/* 点击开始游戏按钮 */
	connect(ui->startBtn, &QPushButton::clicked, this, [this]() {
		// 初始化游戏
		game_win_->init_game();
		emit game_win_->playground_->a_new_round();
		ui->stackedWidget->setCurrentIndex(1);

		// 新开一个线程 不阻塞Qt事件循环
		//std::thread{ std::bind(&gameWindow::game_loop, game_win_) }.detach();
		createGameThread(game_win_);
		GameOpt opt = game_win_->opt();

		this->resize(QSize(opt.width, opt.height + ui->menuBar->height()));
		}
	);
	/* 点击设置按钮 */
	connect(ui->settingBtn, &QPushButton::clicked, this, [this]() {
		ui->stackedWidget->setCurrentIndex(3);
		}
	);
	/* 点击帮助按钮 */
	connect(ui->helpBtn, &QPushButton::clicked, this, [this]() {
		ui->stackedWidget->setCurrentIndex(2);
		}
	);

	/* 菜单栏 */
	// 新游戏
	connect(ui->new_game_action, &QAction::triggered, this, [this]() {
		game_win_->init_game();
		if (ui->stackedWidget->currentIndex() != 1) {
			emit ui->startBtn->clicked();
		}
		}
	);

	// 回到标题
	connect(ui->titleAction, &QAction::triggered, this, [this]() {
		// 摧毁子线程
		destroyGameThread();
		// 切换页面
		ui->stackedWidget->setCurrentIndex(0);
		this->setWindowTitle(QString("PinBall"));
		}
	);
	connect(ui->setting_action, &QAction::triggered, this, [this]() {
		ui->stackedWidget->setCurrentIndex(3);
		}
	);

	// 游戏选项的更新
	connect(setting_win_, &SettingWindow::updateOpt, game_win_, &GameWindow::updateOpt);
	connect(setting_win_, &SettingWindow::updateOpt, this, [this](GameOpt new_opt) {
		bool done = destroyGameThread();
		game_win_->updateOpt(new_opt);
		// 游戏窗口类更新完毕, 重新开启游戏线程
		if (done) {
			createGameThread(game_win_);
		}
		}
	);
}

mainWindow::~mainWindow()
{
	delete game_win_;
	delete help_win_;
	delete setting_win_;
	delete ui;
	if (game_thread_) {
		delete game_thread_;
	}
}

void mainWindow::closeEvent(QCloseEvent* event)
{
	destroyGameThread();
	// 保存数据
	if (setting_win_->usePersistence()) {
		QJsonObject json_obj = save_opt_to_json(setting_win_->opt());
		QByteArray json = QJsonDocument(json_obj).toJson();
		QFile file(opt_file_name_);
		// 只写、阶段 
		file.open(QIODevice::WriteOnly | QIODevice::Truncate);
		file.write(json);
		file.close();
	}

	event->accept();
}

bool mainWindow::destroyGameThread()
{
	// 摧毁子线程
	if (game_thread_ != nullptr) {
		game_thread_->stopThread();
		//game_thread_->exit(0);
		game_thread_->wait();
		//game_win_->init_game();
		delete game_thread_;
		game_thread_ = nullptr;
		return true;
	}
	return false;
}

void mainWindow::createGameThread(GameWindow* game_win)
{
	if (game_thread_ == nullptr) {
		game_thread_ = new GameThread(game_win_);
		game_thread_->start();
	}
}

QJsonObject mainWindow::save_opt_to_json(GameOpt opt)
{
	QJsonObject j_opt;
	j_opt.insert("width", opt.width);
	j_opt.insert("height", opt.height);
	j_opt.insert("background_color", QJsonObject{
		{"red", opt.background_color.red()},
		{"green", opt.background_color.green()},
		{"blue", opt.background_color.blue()}
		});
	j_opt.insert("board_speed", opt.board_speed);
	j_opt.insert("board_margin", opt.board_margin);
	j_opt.insert("board_length", opt.board_length);
	j_opt.insert("board_width", opt.board_width);
	j_opt.insert("board_color", QJsonObject{
		{"red", opt.board_color.red()},
		{"green", opt.board_color.green()},
		{"blue", opt.board_color.blue()}
		});
	j_opt.insert("ball_speed", opt.ball_speed);
	j_opt.insert("ball_init_visible", opt.ball_init_visible);
	j_opt.insert("ball_radius", opt.ball_radius);
	j_opt.insert("ball_color", QJsonObject{
		{"red", opt.ball_color.red()},
		{"green", opt.ball_color.green()},
		{"blue", opt.ball_color.blue()}
		});
	j_opt.insert("delta_t", opt.delta_t);
	j_opt.insert("aim_line_margin", opt.aim_line_margin);
	j_opt.insert("aim_line_length", opt.aim_line_length);
	j_opt.insert("aim_line_color", QJsonObject{
		{"red", opt.aim_line_color.red()},
		{"green", opt.aim_line_color.green()},
		{"blue", opt.aim_line_color.blue()}
		});
	j_opt.insert("aim_line_width", opt.aim_line_width);
	return j_opt;
}

GameOpt mainWindow::read_opt_from_json(QJsonObject j_opt)
{
	GameOpt opt;
	opt.width = j_opt["width"].toInt();
	opt.height = j_opt["height"].toInt();

	QJsonObject bg_color_obj = j_opt["background_color"].toObject();
	opt.background_color.setRed(bg_color_obj["red"].toInt());
	opt.background_color.setGreen(bg_color_obj["green"].toInt());
	opt.background_color.setBlue(bg_color_obj["blue"].toInt());

	opt.board_speed = j_opt["board_speed"].toDouble();
	opt.board_margin = j_opt["board_margin"].toDouble();
	opt.board_length = j_opt["board_length"].toDouble();
	opt.board_width = j_opt["board_width"].toDouble();

	QJsonObject board_color_obj = j_opt["board_color"].toObject();
	opt.board_color.setRed(board_color_obj["red"].toInt());
	opt.board_color.setGreen(board_color_obj["green"].toInt());
	opt.board_color.setBlue(board_color_obj["blue"].toInt());

	opt.ball_speed = j_opt["ball_speed"].toDouble();
	opt.ball_init_visible = j_opt["ball_init_visible"].toBool();
	opt.ball_radius = j_opt["ball_radius"].toDouble();

	QJsonObject ball_color_obj = j_opt["ball_color"].toObject();
	opt.ball_color.setRed(ball_color_obj["red"].toInt());
	opt.ball_color.setGreen(ball_color_obj["green"].toInt());
	opt.ball_color.setBlue(ball_color_obj["blue"].toInt());

	opt.delta_t = j_opt["delta_t"].toDouble();
	opt.aim_line_margin = j_opt["aim_line_margin"].toDouble();
	opt.aim_line_length = j_opt["aim_line_length"].toDouble();

	QJsonObject aim_line_color_obj = j_opt["aim_line_color"].toObject();
	opt.aim_line_color.setRed(aim_line_color_obj["red"].toInt());
	opt.aim_line_color.setGreen(aim_line_color_obj["green"].toInt());
	opt.aim_line_color.setBlue(aim_line_color_obj["blue"].toInt());

	opt.aim_line_width = j_opt["aim_line_width"].toDouble();

	return opt;
}


