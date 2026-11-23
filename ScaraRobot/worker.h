#pragma once

#include "softmotion.h"

#include <QObject>
#include <QThread>

extern SOFTMOTION_API ET_RESULT smc_sync_ioctrl_inout();
extern SOFTMOTION_API ET_RESULT smc_sync_axisctrl_inout(int axis_no = -1);
extern SOFTMOTION_API ET_RESULT smc_sync_robotctrl_inout(int robot_no = -1);

class Worker  : public QObject
{
	Q_OBJECT

public:
	Worker(QObject* parent = nullptr) { stopped = false; }
	~Worker() {	}

public slots:
	void doWork() {		
		bool conn = false, last_conn = false;
		while (!stopped) {
			conn = smc_connected_status();
			if (conn) {
				uint32_t input, output;
				int result = smc_io_get_di_32bit(0, input);					
				result = smc_io_get_dq_32bit(0, output);

				AXIS_MODULE_OUTPUTS value;
				smc_axis_total_output(0, value);
			/*	smc_robot_set_profile_factor(0);
				result = smc_sync_ioctrl_inout();
				result = smc_sync_axisctrl_inout(-1);
				result = smc_sync_robotctrl_inout(-1);*/
			}
			if (conn != last_conn) {
				emit acp_connected(conn);
				last_conn = conn;
			}
			
			Sleep(2);
		}
	}

	void stopWork() {
		stopped = true;
	}

signals:
	void acp_connected(bool state);

private:
	volatile bool stopped;
};
