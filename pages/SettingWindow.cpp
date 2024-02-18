#include "SettingWindow.h"
#include <qshortcut.h>
#include <qdebug.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qregularexpression.h>

SettingWindow::SettingWindow(GameOpt opt, QWidget* parent)
	: QWidget(parent),
	opt_(opt),
	ui(new Ui::SettingWindowClass())
{
	ui->setupUi(this);
	update_opt_to_ui();

	QShortcut* save_shortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
	connect(save_shortcut, &QShortcut::activated, this, [this]() { ui->saveBtn->clicked(); });
	connect(ui->saveBtn, &QPushButton::clicked, this, [this]() {
		bool success = get_opt_from_ui();

		emit updateOpt(opt_);
		if (success) {
			QMessageBox::information(nullptr, "", QString::fromUtf8("保存成功"), "OK");
		}
		else {
			QMessageBox::information(nullptr, "", QString::fromUtf8("保存失败"), "OK");
		}
		update_opt_to_ui();
		}
	);
	// 恢复默认设置
	connect(ui->resetOptBtn, &QPushButton::clicked, this, [this]() {
		opt_ = GameOpt();
		update_opt_to_ui();
		emit updateOpt(opt_);
		QMessageBox::information(this, "", QString::fromUtf8("已恢复默认设置"), "OK");
		}
	);
}

SettingWindow::~SettingWindow()
{
	delete ui;
}

QColor SettingWindow::stringToColor(const QString& str, bool& success)
{
	QRegularExpression reg(QString::fromUtf8("[,，]"));
	QStringList arr = str.split(reg);
	if (arr.size() != 3) {
		success = false;
		return { 0,0,0 };
	}
	int r = arr[0].toInt();
	int g = arr[1].toInt();
	int b = arr[2].toInt();

	if (r < 0) r = 0;
	if (r > 255) r = 255;
	if (g < 0) g = 0;
	if (g > 255) g = 255;
	if (b < 0) b = 0;
	if (b > 255) b = 255;


	success = true;
	return { r,g,b };
}

QString SettingWindow::colorToString(const QColor& color)
{
	return QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue());
}

void SettingWindow::update_opt_to_ui()
{
	ui->ballRadiusEdit->setText(QString::number(opt_.ball_radius));
	ui->ballSpeedEdit->setText(QString::number(opt_.ball_speed));
	ui->ballColorEdit->setText(colorToString(opt_.ball_color));

	ui->boardSpeedEdit->setText(QString::number(opt_.board_speed));
	ui->boardLengthEdit->setText(QString::number(opt_.board_length));
	ui->boardWidthEdit->setText(QString::number(opt_.board_width));
	ui->boardMarginEdit->setText(QString::number(opt_.board_margin));
	ui->boardColorEdit->setText(colorToString(opt_.board_color));

	ui->widthEdit->setText(QString::number(opt_.width));
	ui->heightEdit->setText(QString::number(opt_.height));

	ui->aimlineColorEdit->setText(colorToString(opt_.aim_line_color));
	ui->aimlineLengthEdit->setText(QString::number(opt_.aim_line_length));
	ui->aimlineWidthEdit->setText(QString::number(opt_.aim_line_width));
}

// 成功返回true
// 失败返回false并回滚
bool SettingWindow::get_opt_from_ui()
{
	GameOpt opt_backup = opt_;

	bool valid_color_str = true;
	opt_.ball_radius = ui->ballRadiusEdit->text().toFloat();
	opt_.ball_speed = ui->ballSpeedEdit->text().toFloat();
	QColor color = stringToColor(ui->ballColorEdit->text(), valid_color_str);
	if (valid_color_str) {
		opt_.ball_color = color;
	}
	else {
		opt_ = opt_backup;
		invalidColorStr();
		return false;
	}

	opt_.board_speed = ui->boardSpeedEdit->text().toFloat();
	opt_.board_length = ui->boardLengthEdit->text().toFloat();
	opt_.board_width = ui->boardWidthEdit->text().toFloat();
	opt_.board_margin = ui->boardMarginEdit->text().toFloat();
	color = stringToColor(ui->boardColorEdit->text(), valid_color_str);
	if (valid_color_str) {
		opt_.board_color = color;
	}
	else {
		opt_ = opt_backup;
		invalidColorStr();
		return false;
	}

	opt_.width = ui->widthEdit->text().toFloat();
	opt_.height = ui->heightEdit->text().toFloat();

	color = stringToColor(ui->aimlineColorEdit->text(), valid_color_str);
	if (valid_color_str) {
		opt_.aim_line_color;
	}
	else {
		opt_ = opt_backup;
		invalidColorStr();
		return false;
	}
	opt_.aim_line_length = ui->aimlineLengthEdit->text().toFloat();
	opt_.aim_line_width = ui->aimlineWidthEdit->text().toFloat();

	return true;
}

void SettingWindow::set_opt(GameOpt opt)
{
	opt_ = opt;
}

void SettingWindow::invalidColorStr()
{
	QMessageBox::warning(this, "错误", "非法的颜色值", QMessageBox::Ok);
}
