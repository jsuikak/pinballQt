#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"
#include "pages/gameWindow.h"
#include "pages/HelpWindow.h"
#include "pages/SettingWindow.h"
#include "GameThread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainWindowClass; };
QT_END_NAMESPACE

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(QWidget* parent = nullptr);
	~mainWindow();
protected:
	void closeEvent(QCloseEvent* event) override;
private:
	// 返回值：是否摧毁了一个线程
	bool destroyGameThread();
	void createGameThread(GameWindow* game_win);

	Ui::mainWindowClass* ui;

	GameWindow* game_win_{ nullptr };
	GameThread* game_thread_{ nullptr };
	HelpWindow* help_win_{ nullptr };
	SettingWindow* setting_win_{ nullptr };

	const QString opt_file_name_{ "option.json" };
	QJsonObject save_opt_to_json(GameOpt opt);
	GameOpt read_opt_from_json(QJsonObject j_opt);
};
