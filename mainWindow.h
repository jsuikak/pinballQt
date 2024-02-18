#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"
#include "gameWindow.h"
#include "HelpWindow.h"
#include "SettingWindow.h"
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
    void closeEvent(QCloseEvent *event) override;
private:
	// 返回值：是否摧毁了一个线程
	bool destroyGameThread();
	void createGameThread(GameWindow* game_win);
	Ui::mainWindowClass* ui;
	GameWindow* game_win_{};
	GameThread* game_thread_{};
	HelpWindow* help_win_{};
	SettingWindow* setting_win_{};
};
