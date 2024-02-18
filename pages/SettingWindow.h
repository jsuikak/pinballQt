#pragma once

#include <QWidget>
#include "ui_SettingWindow.h"
#include "GameCore/Common/GameOpt.h"

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
	bool usePersistence() { return ui->usePersistenceCheckBox->checkState() == Qt::Checked; }
	QColor stringToColor(const QString& str, bool &success);
	QString colorToString(const QColor& color);
private:
	Ui::SettingWindowClass* ui;
	GameOpt opt_;
	void update_opt_to_ui();
	bool get_opt_from_ui();
	void set_opt(GameOpt opt);

	void invalidColorStr();
signals:
	// 设置保存后发出这个信号
	void updateOpt(GameOpt opt);
};
