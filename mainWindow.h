#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainWindow.h"
#include "gameWindow.h"
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
private:
	Ui::mainWindowClass* ui;
	GameWindow* game_win_{};
	GameThread* game_thread_{};
};
