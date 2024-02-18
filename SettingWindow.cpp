#include "SettingWindow.h"
#include <qshortcut.h>
#include <qdebug.h>
#include <qdialog.h>
#include <qmessagebox.h>

SettingWindow::SettingWindow(GameOpt opt, QWidget* parent)
	: QWidget(parent),
	opt_(opt),
	ui(new Ui::SettingWindowClass())
{
	ui->setupUi(this);
	ui->ballRadiusEdit->setText(QString::number(opt_.ball_radius));
	ui->ballSpeedEdit->setText(QString::number(opt_.ball_speed));
	ui->boardSpeedEdit->setText(QString::number(opt_.board_speed));
	ui->widthEdit->setText(QString::number(opt_.width));
	ui->heightEdit->setText(QString::number(opt_.height));

	QShortcut* save_shortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
	connect(save_shortcut, &QShortcut::activated, this, [this]() { ui->saveBtn->clicked(); });
	connect(ui->saveBtn, &QPushButton::clicked, this, [this]() {
		opt_.ball_radius = ui->ballRadiusEdit->text().toFloat();
		opt_.ball_speed = ui->ballSpeedEdit->text().toFloat();
		opt_.board_speed = ui->boardSpeedEdit->text().toFloat();
		opt_.width = ui->widthEdit->text().toFloat();
		opt_.height = ui->heightEdit->text().toFloat();

		emit updateOpt(opt_);
		QMessageBox::information(this, "Infomation", QString::fromUtf8("±£´æ³É¹¦"), "OK");
		}
	);
}

SettingWindow::~SettingWindow()
{
	delete ui;
}
