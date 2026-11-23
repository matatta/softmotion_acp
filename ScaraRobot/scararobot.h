#pragma once

#include "ui_scararobot.h"
#include "worker.h"

#include <QtWidgets/QWidget>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class ScaraRobotClass; };
QT_END_NAMESPACE

class ScaraRobot : public QWidget
{
    Q_OBJECT
	
public:
    ScaraRobot(QWidget *parent = nullptr);
    ~ScaraRobot();
	
	//void ui_init_axis_ui();
	void ui_init_axis_btn_actions();
	void ui_init_robot_ui();
	void ui_init_robot_btn_actions();
	void ui_init_output_action();	

	void ui_update_inout();
	void ui_update_axis_param(int axis_no);	
	void ui_update_axis_output(int axis_no);	
	void ui_update_robot_param(int robot_no);
	void ui_update_robot_output(int robot_no);
	void ui_update_intp_param(int intp_no);
	void ui_update_intp_output(int intp_no);

	void timerEvent(QTimerEvent *event);

public slots:
	void connect_state_changed(bool state);

signals:
	void comm_start();

private:
	QThread workerThread, jobThread[50];
	Worker *worker, *job[50];
    Ui::ScaraRobotClass *ui;
	bool m_connected;
	int m_timer_id;
};

