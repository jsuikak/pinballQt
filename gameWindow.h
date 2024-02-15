#pragma once

#include <QWidget>
#include <qtimer.h>

#include "ui_gameWindow.h"
#include "GameCore/Playground.h"

QT_BEGIN_NAMESPACE
namespace Ui { class gameWindowClass; };
QT_END_NAMESPACE

class GameWindow : public QWidget
{
	friend class GameThread;
	Q_OBJECT
public:
	GameWindow(QWidget* parent = nullptr);
	~GameWindow();

	void init_game();
protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void closeEvent(QCloseEvent* event) override;
private:
	Ui::gameWindowClass* ui;
	Playground* playground_;

	// ���ڻ��ƻ���
	//std::vector<std::shared_ptr<GameObj>> objs_;
	//QPainter *painter{ nullptr };

	// ��Ϸ���ڽ���
	bool running_ = false;

	// ��׼��
	bool aiming_ = false;
	// ��׼�Ƕ�
	float angle_ = 45.0f;

	GameOpt opt_;
	//QTimer timer{ this };

signals:
	void windowClosed();
};
