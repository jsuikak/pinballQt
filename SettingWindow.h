#pragma once

#include <QWidget>
#include "ui_SettingWindow.h"
#include "Common/GameOpt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SettingWindowClass; };
QT_END_NAMESPACE

class SettingWindow : public QWidget
{
	Q_OBJECT

public:
	SettingWindow(GameOpt opt, QWidget* parent = nullptr);
	~SettingWindow();
	GameOpt opt() const { return opt_; }
signals:
	// 设置保存后发出这个信号
	void updateOpt(GameOpt opt);

private:
	Ui::SettingWindowClass* ui;
	GameOpt opt_;
};
