#pragma once

#include <QWidget>
#include "ui_HelpWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HelpWindowClass; };
QT_END_NAMESPACE

class HelpWindow : public QWidget
{
	Q_OBJECT

public:
	HelpWindow(QWidget *parent = nullptr);
	~HelpWindow();

private:
	Ui::HelpWindowClass *ui;
};
