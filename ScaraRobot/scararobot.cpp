#include "stdafx.h"
#include "scararobot.h"
#include "softmotion.h"
#include "acp_access.h"
#include <QByteArray>

#include "handler.h"

ScaraRobot::ScaraRobot(QWidget* parent) : QWidget(parent), ui(new Ui::ScaraRobotClass()) {
	// UI
	ui->setupUi(this);
	m_connected = false;

	// 后台通讯线程
	worker = new Worker;
	worker->moveToThread(&workerThread);
	connect(this, &ScaraRobot::comm_start, worker, &Worker::doWork);
	connect(worker, &Worker::acp_connected, this, &ScaraRobot::connect_state_changed);
	connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
	workerThread.start();

	for (int i = 0; i < 8; i++) {
		job[i] = new Worker;
		job[i]->moveToThread(&jobThread[i]);
		connect(this, &ScaraRobot::comm_start, job[i], &Worker::doWork);
		connect(&jobThread[i], &QThread::finished, job[i], &QObject::deleteLater);
		jobThread[i].start();
	}

	// 按钮动作
	ui_init_axis_btn_actions();
	ui_init_robot_ui();
	ui_init_robot_btn_actions();
	//ui_init_output_action();
	m_timer_id = this->startTimer(100);
		
	emit comm_start();
}

ScaraRobot::~ScaraRobot() {
	killTimer(m_timer_id);
	worker->stopWork();
	workerThread.quit();
	workerThread.wait();
	for (int i = 0; i < 8; i++) {
		job[i]->stopWork();
		jobThread[i].quit();
		jobThread[i].wait();
	}
	delete ui;
}

void ScaraRobot::connect_state_changed(bool state) {	
	ui->btAcpConnect->setEnabled(!state);
	ui->btAcpDisconnect->setEnabled(state);
	ui->fmState->setStyleSheet(state ? "QFrame { background-color: green; }" : "QFrame { background-color: red; }");

	m_connected = state;
	if (m_connected) {
		ui->cbAxisSel->setCurrentIndex(-1);
		ui->cbAxisSel->setCurrentIndex(0);
		ui->cbRobotSel->setCurrentIndex(-1);
		ui->cbRobotSel->setCurrentIndex(0);		

		// 更新XYZC参数
		ui_update_intp_param(0);
	}
}

void ScaraRobot::ui_init_robot_ui() {
	ui->rbTargetPos->setChecked(true);

	ui->lbCircDir->hide();
	ui->cbCircDir->hide();
	ui->lbCircPath->hide();
	ui->cbCircPath->hide();
}

void ScaraRobot::ui_init_axis_btn_actions() {
	// Acp连接
	connect(ui->btAcpConnect, &QPushButton::pressed, this, [=]() {
		QByteArray byArr = ui->txAcpIp->text().toLatin1();
		int mode = ui->cbAcpMode->currentIndex();
		smc_open(byArr.data(), mode);
	});

	// Acp断开
	connect(ui->btAcpDisconnect, &QPushButton::pressed, this, [=]() {
		smc_close();
	});
	
	// 测试按钮
	connect(ui->btTest, &QPushButton::pressed, this, [=]() {
		double value = 10;
		ET_RESULT res = smc_var_set_value("Application.GVL.G_SiloMaxDistance", 8, &value);
	});
	
	// Acp读数据
	static bool test = false;
	connect(ui->btAcpRead, &QPushButton::pressed, this, [=]() {
		ET_SILO_STATE state;
		ET_SILO_RESULT res;
		ET_SILO_CMD cmd = EM_SILO_RESET;	

		for (int i = 0; i < 3; i++) {
			hdl_set_silo_command(5, EM_SILO_HOME);
			Sleep(10);
			do {
				hdl_get_silo_result(5, res);
			} while (res != EM_SILO_RESULT_SUCCEED);

			hdl_set_silo_command(5, EM_SILO_GOTO_STANDBY);
			Sleep(10);
			do {
				hdl_get_silo_result(5, res);
			} while (res != EM_SILO_RESULT_SUCCEED);

			hdl_set_silo_command(5, EM_SILO_LOAD);
			Sleep(10);
			do {
				hdl_get_silo_result(5, res);
			} while (res != EM_SILO_RESULT_SUCCEED);

			hdl_set_silo_command(5, EM_SILO_UNLOAD);
			Sleep(10);
			do {
				hdl_get_silo_result(5, res);
			} while (res != EM_SILO_RESULT_SUCCEED);
		}


		/*
		ET_SILO_CMD silo_cmd;
		ST_SILO_INOUT silo_state;
		ST_SILO_PARAM silo_param_rd, silo_param_wr;

		silo_cmd = EM_SILO_LOAD;
		silo_param_wr.M_rSearchHighSpeed = 123;
		silo_param_wr.M_rSearchSlowSpeed = 22;

		hdl_set_silo_command(0, silo_cmd);
		hdl_set_silo_param(0, silo_param_wr);
		hdl_get_silo_inout(0, silo_state, silo_param_rd);
		
		char name[256];
		static int cnt = 0;
		cnt = cnt % 5;
		snprintf(name, sizeof(name), "Application.SR_PCMotion.aiDataSize[%d]", cnt++);

		int32_t val = 0;
		smc_var_get_value(name, 4, &val);
		ui->txAcpTagValue->setValue(val);*/
	});

	// 切换轴
	connect(ui->cbAxisSel, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int axis_no) {
		ui_update_axis_param(axis_no);
	});

	// 开使能
	connect(ui->btAxisPowerOn, &QPushButton::clicked, this, [=](bool checked) {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			if (checked) {
				smc_axis_power_on(num);
				ui->btAxisPowerOn->setText(QStringLiteral("关使能"));
			} else {
				smc_axis_power_off(num);
				ui->btAxisPowerOn->setText(QStringLiteral("开使能"));
			}
		}
	});

	// 绝对移动
	connect(ui->btAxisMoveAbs, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			double ref_pos, ref_vel, ref_acc, ref_dec, ref_jerk;
			ref_pos = ui->txAxisRefPos->value();
			ref_vel = ui->txAxisRefVel->value();
			ref_acc = ui->txAxisRefAcc->value();
			ref_dec = ui->txAxisRefDec->value();
			ref_jerk = ui->txAxisRefJerk->value();
			MC_BUFFER_MODE index = (MC_BUFFER_MODE)ui->cbAxisBufferMode->currentIndex();

			smc_axis_set_profile_velocity(num, ref_vel);
			smc_axis_set_profile_ramp(num, ref_acc, ref_dec, ref_jerk);
			smc_axis_set_profile_buffermode(num, index);
			smc_axis_move_absolute(num, ref_pos);
		}
	});

	// 相对移动
	connect(ui->btAxisMoveRel, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			double ref_pos, ref_vel, ref_acc, ref_dec, ref_jerk;
			ref_pos = ui->txAxisRefPos->value();
			ref_vel = ui->txAxisRefVel->value();
			ref_acc = ui->txAxisRefAcc->value();
			ref_dec = ui->txAxisRefDec->value();
			ref_jerk = ui->txAxisRefJerk->value();
			MC_BUFFER_MODE index = (MC_BUFFER_MODE)ui->cbAxisBufferMode->currentIndex();

			smc_axis_set_profile_velocity(num, ref_vel);
			smc_axis_set_profile_ramp(num, ref_acc, ref_dec, ref_jerk);
			smc_axis_set_profile_buffermode(num, index);
			smc_axis_move_relative(num, ref_pos);
		}
	});

	// 按速度运行
	connect(ui->btAxisMoveVel, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			double ref_pos, ref_vel, ref_acc, ref_dec, ref_jerk;
			ref_pos = ui->txAxisRefPos->value();
			ref_vel = ui->txAxisRefVel->value();
			ref_acc = ui->txAxisRefAcc->value();
			ref_dec = ui->txAxisRefDec->value();
			ref_jerk = ui->txAxisRefJerk->value();
			MC_BUFFER_MODE index = (MC_BUFFER_MODE)ui->cbAxisBufferMode->currentIndex();

			smc_axis_set_profile_velocity(num, ref_vel);
			smc_axis_set_profile_ramp(num, ref_acc, ref_dec, ref_jerk);
			smc_axis_set_profile_buffermode(num, index);
			smc_axis_move_velocity(num, ref_vel);
		}
	});

	// 停止(暂停)
	connect(ui->btAxisMoveHalt, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			smc_axis_halt(num);
		}
	});

	// 位置设定
	connect(ui->btAxisSetPos, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			float pos = ui->txAxisRefPos->value();
			smc_axis_setposition(num, pos);
		}
	});

	// 轴故障复位
	connect(ui->btAxisReset, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			smc_axis_reset(num);
		}
	});

	// 急停
	connect(ui->btAxisEStop, &QPushButton::clicked, this, [=](bool checked) {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			float stop_dec, stop_jerk;
			stop_dec = ui->txAxisRefEStopDec->value();
			stop_jerk = ui->txAxisRefEStopJerk->value();
			smc_axis_set_estop_ramp(num, stop_dec, stop_jerk);			
			smc_axis_estop(num, checked);

			//smc_axis_set_estop_io(num, value);
		}
	});
	
	// 点动前进
	connect(ui->btAxisJogFwd, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			double jog_vel, jog_acc, jog_dec, jog_jerk;			
			jog_vel = ui->txAxisJogVel->value();
			jog_acc = ui->txAxisJogAcc->value();
			jog_dec = ui->txAxisJogDec->value();
			jog_jerk = ui->txAxisJogJerk->value();
			smc_axis_set_jog_velocity(num, jog_vel);
			smc_axis_set_jog_ramp(num, jog_acc, jog_dec, jog_jerk);

			bool inch = ui->cbAxisInch->isChecked();
			double inch_dist = ui->txAxisInchDist->value();
			if (inch) {
				smc_axis_inch_forward(num, true, inch_dist);
			} else {
				smc_axis_jog_forward(num, true);
			}			
		}
	});

	connect(ui->btAxisJogFwd, &QPushButton::released, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {			
			bool inch = ui->cbAxisInch->isChecked();
			double inch_dist = ui->txAxisInchDist->value();
			if (inch) {
				smc_axis_inch_forward(num, false, inch_dist);
			} else {
				smc_axis_jog_forward(num, false);
			}
		}
	});

	// 点动后退
	connect(ui->btAxisJogBwd, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			double jog_vel, jog_acc, jog_dec, jog_jerk;
			jog_vel = ui->txAxisJogVel->value();
			jog_acc = ui->txAxisJogAcc->value();
			jog_dec = ui->txAxisJogDec->value();
			jog_jerk = ui->txAxisJogJerk->value();
			smc_axis_set_jog_velocity(num, jog_vel);
			smc_axis_set_jog_ramp(num, jog_acc, jog_dec, jog_jerk);

			bool inch = ui->cbAxisInch->isChecked();
			double inch_dist = ui->txAxisInchDist->value();
			if (inch) {
				smc_axis_inch_backward(num, true, inch_dist);
			} else {
				smc_axis_jog_backward(num, true);
			}
		}
	});

	connect(ui->btAxisJogBwd, &QPushButton::released, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			bool inch = ui->cbAxisInch->isChecked();
			double inch_dist = ui->txAxisInchDist->value();
			if (inch) {
				smc_axis_inch_backward(num, false, inch_dist);
			} else {
				smc_axis_jog_backward(num, false);
			}
		}
	});

	// 回零参数设置
	connect(ui->btAxisHomeParam, &QPushButton::released, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			double pos = ui->txAxisHomePos->value();
			float vel_h = ui->txAxisHomeHVel->value();
			float vel_l = ui->txAxisHomeLVel->value();
			float acc = ui->txAxisHomeAcc->value();
			float dec = ui->txAxisHomeDec->value();
			float jerk = ui->txAxisHomeJerk->value();

			smc_axis_set_home_position(num, pos);
			smc_axis_set_home_velocity(num, vel_l, vel_h);
			smc_axis_set_home_ramp(num, acc, dec, jerk);
			
			if (true) {
				uint16_t mode = ui->txAxisDrvHomeMode->value();
				bool en_sdo = ui->ckAxisHomeEnSdo->isChecked();
				float offset = ui->txAxisDrvHomeOffset->value();
				smc_axis_set_drv_home_param(num, offset, en_sdo);
			}

			if (true) {
				SMC_HOME_MODE mode;
				int index = ui->cbAxisSmcHomeMode->currentIndex();
				switch (index) {
				case 0:mode = SMC_HOME_FAST_NEG_BSLOW_S_STOP; break;
				case 1:mode = SMC_HOME_FAST_NEG_BSLOW_STOP_S; break;
				case 2:mode = SMC_HOME_FAST_NEG_BSLOW_I_S_STOP; break;
				case 3:mode = SMC_HOME_FAST_NEG_SLOW_S_STOP; break;
				case 4:mode = SMC_HOME_FAST_NEG_SLOW_STOP_S; break;
				case 5:mode = SMC_HOME_FAST_NEG_SLOW_I_S_STOP; break;
				case 6:mode = SMC_HOME_FAST_POS_BSLOW_S_STOP; break;
				case 7:mode = SMC_HOME_FAST_POS_BSLOW_STOP_S; break;
				case 8:mode = SMC_HOME_FAST_POS_BSLOW_I_S_STOP; break;
				case 9:mode = SMC_HOME_FAST_POS_SLOW_S_STOP; break;
				case 10:mode = SMC_HOME_FAST_POS_SLOW_STOP_S; break;
				case 11:mode = SMC_HOME_FAST_POS_SLOW_I_S_STOP; break;
				default:break;
				}

				int switch_module = ui->txAxisHomeSwitchModule->value();
				int switch_io = ui->txAxisHomeSwitchIO->value();
				int index_module = ui->txAxisHomeIndexModule->value();
				int index_io = ui->txAxisHomeIndexIO->value();
				smc_axis_set_plc_home_io(num, switch_module, switch_io, index_module, index_io);

				double signal_delay = ui->txAxisHomeSwitchDelay->value();
				double index_position = ui->txAxisHomePls->value();
				bool return_to_zero = ui->ckHomeRetrunZero->isChecked();
				bool ignore_hw_limits = ui->ckIgnoreHw->isChecked();
				smc_axis_set_plc_home_mode(num, mode);
				smc_axis_set_plc_home_param(num, signal_delay, index_position, return_to_zero, ignore_hw_limits);
			}
		}
	});
	

	// 驱动器回零
	connect(ui->btDrvHome, &QPushButton::released, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			double pos = ui->txAxisHomePos->value();
			float vel_h = ui->txAxisHomeHVel->value();
			float vel_l = ui->txAxisHomeLVel->value();
			float acc = ui->txAxisHomeAcc->value();
			float dec = ui->txAxisHomeDec->value();
			float jerk = ui->txAxisHomeJerk->value();

			smc_axis_set_home_position(num, pos);
			smc_axis_set_home_velocity(num, vel_l, vel_h);
			smc_axis_set_home_ramp(num, acc, dec, jerk);

			uint16_t mode = ui->txAxisDrvHomeMode->value();
			bool en_sdo = ui->ckAxisHomeEnSdo->isChecked();
			bool en_conv = ui->ckAxisHomeEnConv->isChecked();
			float offset = ui->txAxisDrvHomeOffset->value();
			smc_axis_set_drv_home_param(num, en_sdo, en_conv);
			smc_axis_drv_home(num, mode);
		}
	});

	// plc回零
	connect(ui->btPlcHome, &QPushButton::released, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			double pos = ui->txAxisHomePos->value();
			float vel_h = ui->txAxisHomeHVel->value();
			float vel_l = ui->txAxisHomeLVel->value();
			float acc = ui->txAxisHomeAcc->value();
			float dec = ui->txAxisHomeDec->value();
			float jerk = ui->txAxisHomeJerk->value();

			smc_axis_set_home_position(num, pos);
			smc_axis_set_home_velocity(num, vel_l, vel_h);
			smc_axis_set_home_ramp(num, acc, dec, jerk);

			SMC_HOME_MODE mode;
			int index = ui->cbAxisSmcHomeMode->currentIndex();
			switch (index) {
			case 0:mode = SMC_HOME_FAST_NEG_BSLOW_S_STOP; break;
			case 1:mode = SMC_HOME_FAST_NEG_BSLOW_STOP_S; break;
			case 2:mode = SMC_HOME_FAST_NEG_BSLOW_I_S_STOP; break;
			case 3:mode = SMC_HOME_FAST_NEG_SLOW_S_STOP; break;
			case 4:mode = SMC_HOME_FAST_NEG_SLOW_STOP_S; break;
			case 5:mode = SMC_HOME_FAST_NEG_SLOW_I_S_STOP; break;
			case 6:mode = SMC_HOME_FAST_POS_BSLOW_S_STOP; break;
			case 7:mode = SMC_HOME_FAST_POS_BSLOW_STOP_S; break;
			case 8:mode = SMC_HOME_FAST_POS_BSLOW_I_S_STOP; break;
			case 9:mode = SMC_HOME_FAST_POS_SLOW_S_STOP; break;
			case 10:mode = SMC_HOME_FAST_POS_SLOW_STOP_S; break;
			case 11:mode = SMC_HOME_FAST_POS_SLOW_I_S_STOP; break;
			default:break;
			}

			int switch_module = ui->txAxisHomeSwitchModule->value();
			int switch_io = ui->txAxisHomeSwitchIO->value();
			int index_module = ui->txAxisHomeIndexModule->value();
			int index_io = ui->txAxisHomeIndexIO->value();
			smc_axis_set_plc_home_io(num, switch_module, switch_io, index_module, index_io);

			double signal_delay = ui->txAxisHomeSwitchDelay->value();
			double index_position = ui->txAxisHomePls->value();
			bool return_to_zero = ui->ckHomeRetrunZero->isChecked();
			bool ignore_hw_limits = ui->ckIgnoreHw->isChecked();
			smc_axis_set_plc_home_mode(num, mode);
			smc_axis_set_plc_home_param(num, signal_delay, index_position, return_to_zero, ignore_hw_limits);
			smc_axis_plc_home(num, mode);
		}
	});
	
	// 读SDO
	connect(ui->btSdoRead, &QPushButton::released, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			int sdo_index = ui->txSdoIndex->value();
			int sdo_subIndex = ui->txSdoSubIndex->value();
			int sdo_len = ui->txSdoDataLen->value();
			double sdo_val;
			ET_RESULT result = smc_axis_get_sdo_param(num, sdo_index, sdo_subIndex, sdo_len, sdo_val);
			ui->txSdoReadValue->setValue(sdo_val);
		}
	});

	// 写SDO
	connect(ui->btSdoWrite, &QPushButton::released, this, [=]() {
		bool isOk = false;
		int num = ui->cbAxisSel->currentText().toInt(&isOk);
		if (isOk) {
			int sdo_index = ui->txSdoIndex->value();
			int sdo_subIndex = ui->txSdoSubIndex->value();
			int sdo_len = ui->txSdoDataLen->value();
			double sdo_val = ui->txSdoWriteValue->value();
			smc_axis_set_sdo_param(num, sdo_index, sdo_subIndex, sdo_len, sdo_val);
		}
	});

	// Intp开使能
	//connect(ui->btIntpPowerOn, &QPushButton::clicked, this, [=](bool checked) {
	//	if (checked) {
	//		smc_intp_xyz_power_on(0);
	//		ui->btIntpPowerOn->setText(QStringLiteral("关使能"));
	//	} else {
	//		smc_intp_xyz_power_off(0);
	//		ui->btIntpPowerOn->setText(QStringLiteral("开使能"));
	//	}
	//});

	// Intp绝对定位
	//connect(ui->btIntpMvAbs, &QPushButton::released, this, [=]() {
	//	double vel = ui->txIntpRefVel->value();
	//	double acc = ui->txIntpRefAcc->value();
	//	double dec = ui->txIntpRefDec->value();
	//	double jerk = ui->txIntpRefJerk->value();

	//	MC_COORD_REF pos;
	//	SMC_VECTOR3D offset;
	//	pos.X = ui->txIntpRefPosX->value();
	//	pos.Y = ui->txIntpRefPosY->value();
	//	pos.Z = ui->txIntpRefPosZ->value();
	//	pos.C = ui->txIntpRefPosC->value();

	//	offset.dX = ui->txIntpRefOffsetX->value();
	//	offset.dY = ui->txIntpRefOffsetY->value();

	//	smc_intp_xyz_set_profile_param(0, vel, acc, dec, jerk);
	//	smc_intp_xyz_move_absolute(0, pos.X, pos.Y, pos.Z, pos.C, offset.dX, offset.dY);
	//});

	// Intp相对定位
	//connect(ui->btIntpMvRel, &QPushButton::released, this, [=]() {
	//	double vel = ui->txIntpRefVel->value();
	//	double acc = ui->txIntpRefAcc->value();
	//	double dec = ui->txIntpRefDec->value();
	//	double jerk = ui->txIntpRefJerk->value();

	//	MC_COORD_REF pos;
	//	SMC_VECTOR3D offset;
	//	pos.X = ui->txIntpRefPosX->value();
	//	pos.Y = ui->txIntpRefPosY->value();
	//	pos.Z = ui->txIntpRefPosZ->value();
	//	pos.C = ui->txIntpRefPosC->value();

	//	offset.dX = ui->txIntpRefOffsetX->value();
	//	offset.dY = ui->txIntpRefOffsetY->value();

	//	smc_intp_xyz_set_profile_param(0, vel, acc, dec, jerk);
	//	smc_intp_xyz_move_relative(0, pos.X, pos.Y, pos.Z, pos.C, offset.dX, offset.dY);
	//});

	// Intp中止
	//connect(ui->btIntpMvHalt, &QPushButton::released, this, [=]() {
	//	smc_intp_xyz_halt(0);
	//});

	// Intp复位
	//connect(ui->btIntpMvReset, &QPushButton::released, this, [=]() {
	//	smc_intp_xyz_reset(0);
	//});	
}

void ScaraRobot::ui_init_robot_btn_actions() {
	// 切换机器人
	connect(ui->cbRobotSel, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int robot_no) {
		ui_update_robot_param(robot_no);
	});

	// 切换坐标显示
	connect(ui->cbRobotCoord, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int coord) {
		ui->stkpos->setCurrentIndex(coord == 0 ? 0 : 1);
	});

	// 切换混合模式
	connect(ui->cbRobotTransMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int mode) {
		ui->stktrans->setCurrentIndex(mode > 0 ? mode - 1 : 0);
	});
	

	// 机器人启用
	connect(ui->ckRobotEnable, &QCheckBox::clicked, this, [=](bool checked) {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			if (checked) {
				smc_robot_enable(num);
			} else {
				smc_robot_disable(num);
			}
		}
	});

	// 机器人使能
	connect(ui->ckRobotPowerOn, &QPushButton::clicked, this, [=](bool checked) {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			if (checked) {
				smc_robot_power_on(num);
			} else {
				smc_robot_power_off(num);
			}			
		}
	});

	// 机器人工具周期
	connect(ui->btCAxisToolPeriod, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			int period = ui->txRobotToolPeriod->value();
			smc_robot_set_caxis_period(num, period);
		}
	});

	// 机器人手系设置
	connect(ui->btScaraHandSet, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			if (ui->rbScaraLeft->isChecked()) {
				smc_robot_set_zcscara_elbow_left(num);
			} else if (ui->rbScaraRight->isChecked()) {
				smc_robot_set_zcscara_elbow_right(num);
			}
		}
	});

	// 选择目标位置
	connect(ui->rbMidPos, &QPushButton::toggled, this, [=](bool checked) {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			SMC_COORD_SYSTEM coord = (SMC_COORD_SYSTEM)ui->cbRobotCoord->currentIndex();
			SMC_POS_REF position;
			smc_robot_get_profile_position(num, position);
			if (coord == SMC_COORD_ACS) {
				ui->txRobotRefPosA1->setValue(position.a.a0);
				ui->txRobotRefPosA2->setValue(position.a.a1);
				ui->txRobotRefPosA3->setValue(position.a.a2);
				ui->txRobotRefPosA4->setValue(position.a.a3);
				ui->txRobotRefPosA5->setValue(position.a.a4);
				ui->txRobotRefPosA6->setValue(position.a.a5);
			} else {
				ui->txRobotRefPosX->setValue(position.c.X);
				ui->txRobotRefPosY->setValue(position.c.Y);
				ui->txRobotRefPosZ->setValue(position.c.Z);
				ui->txRobotRefPosA->setValue(position.c.A);
				ui->txRobotRefPosB->setValue(position.c.B);
				ui->txRobotRefPosC->setValue(position.c.C);
			}
		}
	});

	// 选择中间位置
	connect(ui->rbMidPos, &QPushButton::toggled, this, [=](bool checked) {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			SMC_COORD_SYSTEM coord = (SMC_COORD_SYSTEM)ui->cbRobotCoord->currentIndex();
			SMC_POS_REF position;
			smc_robot_get_profile_aux_position(num, position);
			if (coord == SMC_COORD_ACS) {
				ui->txRobotRefPosA1->setValue(position.a.a0);
				ui->txRobotRefPosA2->setValue(position.a.a1);
				ui->txRobotRefPosA3->setValue(position.a.a2);
				ui->txRobotRefPosA4->setValue(position.a.a3);
				ui->txRobotRefPosA5->setValue(position.a.a4);
				ui->txRobotRefPosA6->setValue(position.a.a5);
			} else {
				ui->txRobotRefPosX->setValue(position.c.X);
				ui->txRobotRefPosY->setValue(position.c.Y);
				ui->txRobotRefPosZ->setValue(position.c.Z);
				ui->txRobotRefPosA->setValue(position.c.A);
				ui->txRobotRefPosB->setValue(position.c.B);
				ui->txRobotRefPosC->setValue(position.c.C);
			}
		}
	});

	// 机器人PTP绝对定位
	connect(ui->btRobotPTP, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		bool isRel = ui->ckRobotRel->isChecked();
		if (isOk) {
			SMC_COORD_SYSTEM coord = (SMC_COORD_SYSTEM)ui->cbRobotCoord->currentIndex();
			SMC_POS_REF position;
			if (coord == SMC_COORD_ACS) {
				position.a.a0 = ui->txRobotRefPosA1->value();
				position.a.a1 = ui->txRobotRefPosA2->value();
				position.a.a2 = ui->txRobotRefPosA3->value();
				position.a.a3 = ui->txRobotRefPosA4->value();
				position.a.a4 = ui->txRobotRefPosA5->value();
				position.a.a5 = ui->txRobotRefPosA6->value();
			} else {
				position.c.X = ui->txRobotRefPosX->value();
				position.c.Y = ui->txRobotRefPosY->value();
				position.c.Z = ui->txRobotRefPosZ->value();
				position.c.A = ui->txRobotRefPosA->value();
				position.c.B = ui->txRobotRefPosB->value();
				position.c.C = ui->txRobotRefPosC->value();
			}

			double acc_factor = ui->txRobotFBKVel->value();
			double dec_factor = ui->txRobotFBKAcc->value();
			double jerk_factor = ui->txRobotFBKJerk->value();
			double torque_factor = ui->txRobotFBKTor->value();
			MC_BUFFER_MODE buffermode = (MC_BUFFER_MODE)ui->cbRobotBufferMode->currentIndex();
			MC_TRANSITION_MODE transmode = (MC_TRANSITION_MODE)ui->cbRobotTransMode->currentIndex();

			double trans_vel_factor = ui->txRobotTransVelFactor->value();
			double corner_dist = ui->txRobotTransCornerDist->value();
			double line_dist = ui->txRobotTransLineDist->value();
			double ang_dist = ui->txRobotTransAngDist->value();
			bool euc_done = ui->ckRobotTransEucDone->isChecked();
			bool blend_with_aux = ui->ckRobotTransBlendAux->isChecked();
			smc_robot_set_profile_factor(num, acc_factor, dec_factor, jerk_factor, torque_factor);
			smc_robot_set_profile_buffermode(num, buffermode);

			int trans_mode = ui->cbRobotTransMode->currentIndex();
			switch (trans_mode) {
			case 0:
				smc_robot_set_profile_transmode_none(num);
				break;
			case 1:
				smc_robot_set_profile_transmode_start_velocity(num, trans_vel_factor, blend_with_aux);
				break;
			case 2:
				smc_robot_set_profile_transmode_corner_distance(num, corner_dist);
				break;
			case 3:
				smc_robot_set_profile_transmode_axis_distance(num, line_dist, ang_dist, euc_done, blend_with_aux);
			}			

			SMC_ORIENTATION_MODE orienmode = (SMC_ORIENTATION_MODE)ui->cbRobotOriMode->currentIndex();
			smc_robot_set_profile_orienmode(num, orienmode);
			if (!isRel) {
				smc_robot_ptp_absolute(num, coord, position);
			} else {
				smc_robot_ptp_relative(num, coord, position);
			}
		}
	});


	// 机器人直线绝对定位
	connect(ui->btRobotLinear, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		bool isRel = ui->ckRobotRel->isChecked();
		if (isOk) {
			SMC_COORD_SYSTEM coord = (SMC_COORD_SYSTEM)ui->cbRobotCoord->currentIndex();
			SMC_POS_REF position;
			if (coord == SMC_COORD_ACS) {
				position.a.a0 = ui->txRobotRefPosA1->value();
				position.a.a1 = ui->txRobotRefPosA2->value();
				position.a.a2 = ui->txRobotRefPosA3->value();
				position.a.a3 = ui->txRobotRefPosA4->value();
				position.a.a4 = ui->txRobotRefPosA5->value();
				position.a.a5 = ui->txRobotRefPosA6->value();
			} else {
				position.c.X = ui->txRobotRefPosX->value();
				position.c.Y = ui->txRobotRefPosY->value();
				position.c.Z = ui->txRobotRefPosZ->value();
				position.c.A = ui->txRobotRefPosA->value();
				position.c.B = ui->txRobotRefPosB->value();
				position.c.C = ui->txRobotRefPosC->value();
			}

			double vel = ui->txRobotPathVel->value();
			double acc = ui->txRobotPathAcc->value();
			double dec = ui->txRobotPathDec->value();
			double jerk = ui->txRobotPathJerk->value();
			smc_robot_set_profile_velocity(num, vel);
			smc_robot_set_profile_ramp(num, acc, dec, jerk);

			double acc_factor = ui->txRobotFBKVel->value();
			double dec_factor = ui->txRobotFBKAcc->value();
			double jerk_factor = ui->txRobotFBKJerk->value();
			double torque_factor = ui->txRobotFBKTor->value();
			MC_BUFFER_MODE buffermode = (MC_BUFFER_MODE)ui->cbRobotBufferMode->currentIndex();
			MC_TRANSITION_MODE transmode = (MC_TRANSITION_MODE)ui->cbRobotTransMode->currentIndex();

			double trans_vel_factor = ui->txRobotTransVelFactor->value();
			double corner_dist = ui->txRobotTransCornerDist->value();
			double line_dist = ui->txRobotTransLineDist->value();
			double ang_dist = ui->txRobotTransAngDist->value();
			bool euc_done = ui->ckRobotTransEucDone->isChecked();
			bool blend_with_aux = ui->ckRobotTransBlendAux->isChecked();
			smc_robot_set_profile_factor(num, acc_factor, dec_factor, jerk_factor, torque_factor);
			smc_robot_set_profile_buffermode(num, buffermode);
			
			int trans_mode = ui->cbRobotTransMode->currentIndex();
			switch (trans_mode) {
			case 0:
				smc_robot_set_profile_transmode_none(num);
				break;
			case 1:
				smc_robot_set_profile_transmode_start_velocity(num, trans_vel_factor, blend_with_aux);
				break;
			case 2:
				smc_robot_set_profile_transmode_corner_distance(num, corner_dist);
				break;
			case 3:
				smc_robot_set_profile_transmode_axis_distance(num, line_dist, ang_dist, euc_done, blend_with_aux);
			}

			SMC_ORIENTATION_MODE orienmode = (SMC_ORIENTATION_MODE)ui->cbRobotOriMode->currentIndex();
			smc_robot_set_profile_orienmode(num, orienmode);		

			if (!isRel) {
				smc_robot_linear_absolute(num, coord, position);
			} else {
				smc_robot_linear_relative(num, coord, position);
			}
		}
	});

	// 机器人圆弧定位
	connect(ui->btRobotCirc, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			SMC_COORD_SYSTEM coord = (SMC_COORD_SYSTEM)ui->cbRobotCoord->currentIndex();
			SMC_POS_REF position;
			if (coord == SMC_COORD_ACS) {
				position.a.a0 = ui->txRobotRefPosA1->value();
				position.a.a1 = ui->txRobotRefPosA2->value();
				position.a.a2 = ui->txRobotRefPosA3->value();
				position.a.a3 = ui->txRobotRefPosA4->value();
				position.a.a4 = ui->txRobotRefPosA5->value();
				position.a.a5 = ui->txRobotRefPosA6->value();
			} else {
				position.c.X = ui->txRobotRefPosX->value();
				position.c.Y = ui->txRobotRefPosY->value();
				position.c.Z = ui->txRobotRefPosZ->value();
				position.c.A = ui->txRobotRefPosA->value();
				position.c.B = ui->txRobotRefPosB->value();
				position.c.C = ui->txRobotRefPosC->value();
			}

			double vel = ui->txRobotPathVel->value();
			double acc = ui->txRobotPathAcc->value();
			double dec = ui->txRobotPathDec->value();
			double jerk = ui->txRobotPathJerk->value();
			smc_robot_set_profile_velocity(num, vel);
			smc_robot_set_profile_ramp(num, acc, dec, jerk);

			double acc_factor = ui->txRobotFBKVel->value();
			double dec_factor = ui->txRobotFBKAcc->value();
			double jerk_factor = ui->txRobotFBKJerk->value();
			double torque_factor = ui->txRobotFBKTor->value();
			MC_BUFFER_MODE buffermode = (MC_BUFFER_MODE)ui->cbRobotBufferMode->currentIndex();
			MC_TRANSITION_MODE transmode = (MC_TRANSITION_MODE)ui->cbRobotTransMode->currentIndex();

			double trans_vel_factor = ui->txRobotTransVelFactor->value();
			double corner_dist = ui->txRobotTransCornerDist->value();
			double line_dist = ui->txRobotTransLineDist->value();
			double ang_dist = ui->txRobotTransAngDist->value();
			bool euc_done = ui->ckRobotTransEucDone->isChecked();
			bool blend_with_aux = ui->ckRobotTransBlendAux->isChecked();
			smc_robot_set_profile_factor(num, acc_factor, dec_factor, jerk_factor, torque_factor);
			smc_robot_set_profile_buffermode(num, buffermode);
			
			int trans_mode = ui->cbRobotTransMode->currentIndex();
			switch (trans_mode) {
			case 0:
				smc_robot_set_profile_transmode_none(num);
				break;
			case 1:
				smc_robot_set_profile_transmode_start_velocity(num, trans_vel_factor, blend_with_aux);
				break;
			case 2:
				smc_robot_set_profile_transmode_corner_distance(num, corner_dist);
				break;
			case 3:
				smc_robot_set_profile_transmode_axis_distance(num, line_dist, ang_dist, euc_done, blend_with_aux);
			}

			SMC_ORIENTATION_MODE orienmode = (SMC_ORIENTATION_MODE)ui->cbRobotOriMode->currentIndex();
			smc_robot_set_profile_orienmode(num, orienmode);

			SMC_CIRC_MODE circ_mode = (SMC_CIRC_MODE)ui->cbCircDir->currentIndex();
			MC_CIRC_PATHCHOICE circ_path = (MC_CIRC_PATHCHOICE)ui->cbCircPath->currentIndex();
			smc_robot_set_profile_circular(num, circ_mode, circ_path);

			SMC_POS_REF aux_pos;
			/*if (coord == SMC_COORD_ACS) {
				aux_pos.a.a0 = ui->txRobotRefMidPosA1->value();
				aux_pos.a.a1 = ui->txRobotRefMidPosA2->value();
				aux_pos.a.a2 = ui->txRobotRefMidPosA3->value();
				aux_pos.a.a3 = ui->txRobotRefMidPosA4->value();
				aux_pos.a.a4 = ui->txRobotRefMidPosA5->value();
				aux_pos.a.a5 = ui->txRobotRefMidPosA6->value();
			} else {
				aux_pos.c.X = ui->txRobotRefMidPosX->value();
				aux_pos.c.Y = ui->txRobotRefMidPosY->value();
				aux_pos.c.Z = ui->txRobotRefMidPosZ->value();
				aux_pos.c.A = ui->txRobotRefMidPosA->value();
				aux_pos.c.B = ui->txRobotRefMidPosB->value();
				aux_pos.c.C = ui->txRobotRefMidPosC->value();
			}*/
			smc_robot_circ_absolute(num, coord, position, aux_pos);
			smc_robot_circ_relative(num, coord, position, aux_pos);
		}
	});

	// 读工具偏移
	connect(ui->btRobotRdTool, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			MC_COORD_REF offset;	
			smc_robot_get_tool_offset(num, offset, SMC_READ_TOOL_ACTIVE_MOVEMENT);

			ui->txRobotRefPosX->setValue(offset.X);
			ui->txRobotRefPosY->setValue(offset.Y);
			ui->txRobotRefPosZ->setValue(offset.Z);
			ui->txRobotRefPosA->setValue(offset.A);
			ui->txRobotRefPosB->setValue(offset.B);
			ui->txRobotRefPosC->setValue(offset.C);		
		}
	});

	// 写工具偏移
	connect(ui->btRobotWrTool, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			MC_COORD_REF offset;
			offset.X = ui->txRobotRefPosX->value();
			offset.Y = ui->txRobotRefPosY->value();
			offset.Z = ui->txRobotRefPosZ->value();
			offset.A = ui->txRobotRefPosA->value();
			offset.B = ui->txRobotRefPosB->value();
			offset.C = ui->txRobotRefPosC->value();
			smc_robot_set_tool_offset(num, offset);
		}
	});
	
	// 机器人中止
	connect(ui->btRobotHalt, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			smc_robot_halt(num, true);
		}
	});

	// 机器人位置设定
	connect(ui->btRobotSetPos, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			SMC_COORD_SYSTEM coord = (SMC_COORD_SYSTEM)ui->cbRobotCoord->currentIndex();
			SMC_POS_REF position;
			if (coord == SMC_COORD_ACS) {
				position.a.a0 = ui->txRobotRefPosA1->value();
				position.a.a1 = ui->txRobotRefPosA2->value();
				position.a.a2 = ui->txRobotRefPosA3->value();
				position.a.a3 = ui->txRobotRefPosA4->value();
				position.a.a4 = ui->txRobotRefPosA5->value();
				position.a.a5 = ui->txRobotRefPosA6->value();
			} else {
				position.c.X = ui->txRobotRefPosX->value();
				position.c.Y = ui->txRobotRefPosY->value();
				position.c.Z = ui->txRobotRefPosZ->value();
				position.c.A = ui->txRobotRefPosA->value();
				position.c.B = ui->txRobotRefPosB->value();
				position.c.C = ui->txRobotRefPosC->value();
			}			
			smc_robot_setposition(num, coord, position);
		}
	});

	// 机器人回零
	connect(ui->btRobotHome, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			int16_t axis_num;
			int16_t axis_no_list[10] = { 0 };
			smc_robot_get_axis_list(num, axis_no_list, axis_num);

			// XXY平台测试程序
			if (num == 1) {
				for (int i = 0; i < axis_num; i++) {
					smc_axis_set_home_position(axis_no_list[i], 0);
					smc_axis_set_home_velocity(axis_no_list[i], 2, 10);
					smc_axis_set_home_ramp(axis_no_list[i], 100, 100, 2000);
					smc_axis_set_drv_home_mode(axis_no_list[i], 27);
					smc_axis_set_drv_home_param(axis_no_list[i], 0, false);
				}
				smc_robot_set_home_sequence(num, 0, 0x03);
				smc_robot_set_home_sequence(num, 1, 0x04);				
				smc_robot_drv_home(num);
			}
		}
	});

	// 机器人复位
	connect(ui->btRobotReset, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			smc_robot_reset(num);
		}
	});

	// 机器人急停
	connect(ui->btRobotEStop, &QPushButton::clicked, this, [=](bool checked) {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			double stop_dec = ui->btRobotEStopDec->value(); 
			double stop_jerk = ui->btRobotEStopJerk->value();
			double stop_dec_factor = ui->btRobotEStopKAcc->value();
			double stop_jerk_factor = ui->btRobotEStopKJerk->value();
			double stop_tor_factor = ui->btRobotEStopKTor->value();
			int16_t stop_module_no = ui->btRobotEStopModule->value();
			int16_t stop_io_no = ui->btRobotEStopIO->value();
			bool halt_clr_mv = ui->cbRobotHaltClear->isChecked();
			bool stop_clr_mv= ui->cbRobotEStopClear->isChecked();
			
			smc_robot_set_estop_ramp(num, stop_dec, stop_jerk);
			smc_robot_set_estop_factor(num, stop_dec_factor, stop_jerk_factor, stop_tor_factor);
			smc_robot_set_estop_clear_movement(num, stop_clr_mv);
			smc_robot_set_estop_io(num, stop_module_no, stop_io_no);			
			smc_robot_estop(num, checked, stop_clr_mv);
		}
	});

	// 机器人点动前进
	connect(ui->btRobotJogFwd, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {			
			double jog_line_dist = ui->txRobotJogDist->value();
			double jog_ang_dist = ui->txRobotJogAng->value();
			double jog_vel = ui->txRobotJogVel->value();
			double jog_acc = ui->txRobotJogAcc->value();
			double jog_dec = ui->txRobotJogDec->value();
			double jog_jerk = ui->txRobotJogJerk->value();
			double jog_fac_vel = ui->txRobotJogKVel->value();
			double jog_fac_acc = ui->txRobotJogKAcc->value();
			double jog_fac_jerk = ui->txRobotJogKJerk->value();
			double jog_fac_tor = ui->txRobotJogKTor->value();

			smc_robot_set_jog_limit(num, jog_line_dist, jog_ang_dist);
			smc_robot_set_jog_velocity(num, jog_vel);
			smc_robot_set_jog_ramp(num, jog_acc, jog_dec, jog_jerk);
			smc_robot_set_jog_factor(num, jog_fac_vel, jog_fac_acc, jog_fac_jerk, jog_fac_tor);

			int coord = ui->cbRobotJogCoord->currentIndex();
			int axis_num = ui->cbRobotJogAxisSel->currentIndex();
			bool jog_abs_as_acs = ui->ckRobotJogAbcAsAcs->isChecked();
			smc_robot_jog_forward(num, (SMC_COORD_SYSTEM)coord, axis_num, true, jog_abs_as_acs);
		}
	});

	connect(ui->btRobotJogFwd, &QPushButton::released, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			int coord = ui->cbRobotJogCoord->currentIndex();
			int axis_num = ui->cbRobotJogAxisSel->currentIndex();
			bool jog_abs_as_acs = ui->ckRobotJogAbcAsAcs->isChecked();
			smc_robot_jog_forward(num, (SMC_COORD_SYSTEM)coord, axis_num, false, jog_abs_as_acs);
		}
	});

	// 机器人点动后退
	connect(ui->btRobotJogBwd, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			double jog_line_dist = ui->txRobotJogDist->value();
			double jog_ang_dist = ui->txRobotJogAng->value();
			double jog_vel = ui->txRobotJogVel->value();
			double jog_acc = ui->txRobotJogAcc->value();
			double jog_dec = ui->txRobotJogDec->value();
			double jog_jerk = ui->txRobotJogJerk->value();
			double jog_fac_vel = ui->txRobotJogKVel->value();
			double jog_fac_acc = ui->txRobotJogKAcc->value();
			double jog_fac_jerk = ui->txRobotJogKJerk->value();
			double jog_fac_tor = ui->txRobotJogKTor->value();

			smc_robot_set_jog_limit(num, jog_line_dist, jog_ang_dist);
			smc_robot_set_jog_velocity(num, jog_vel);
			smc_robot_set_jog_ramp(num, jog_acc, jog_dec, jog_jerk);
			smc_robot_set_jog_factor(num, jog_fac_vel, jog_fac_acc, jog_fac_jerk, jog_fac_tor);

			int coord = ui->cbRobotJogCoord->currentIndex();
			int axis_num = ui->cbRobotJogAxisSel->currentIndex();
			bool jog_abs_as_acs = ui->ckRobotJogAbcAsAcs->isChecked();
			smc_robot_jog_backward(num, (SMC_COORD_SYSTEM)coord, axis_num, true, jog_abs_as_acs);
		}
	});

	connect(ui->btRobotJogBwd, &QPushButton::released, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			int coord = ui->cbRobotJogCoord->currentIndex();
			int axis_num = ui->cbRobotJogAxisSel->currentIndex();
			bool jog_abs_as_acs = ui->ckRobotJogAbcAsAcs->isChecked();
			smc_robot_jog_backward(num, (SMC_COORD_SYSTEM)coord, axis_num, false, jog_abs_as_acs);
		}
	});

	// 设置安全高度
	connect(ui->btRobotSetSafeHeight, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			double safe_height = ui->txRobotSafeHeight->value();
			smc_robot_set_safe_height(num, safe_height);
		}
	});

	// 跳跃
	connect(ui->btRobotJump, &QPushButton::pressed, this, [=]() {
		bool isOk = false;
		int num = ui->cbRobotSel->currentText().toInt(&isOk);
		if (isOk) {
			SMC_COORD_SYSTEM coord = (SMC_COORD_SYSTEM)ui->cbRobotCoord->currentIndex();
			SMC_POS_REF position;
			if (coord == SMC_COORD_ACS) {
				position.a.a0 = ui->txRobotRefPosA1->value();
				position.a.a1 = ui->txRobotRefPosA2->value();
				position.a.a2 = ui->txRobotRefPosA3->value();
				position.a.a3 = ui->txRobotRefPosA4->value();
				position.a.a4 = ui->txRobotRefPosA5->value();
				position.a.a5 = ui->txRobotRefPosA6->value();
			} else {
				position.c.X = ui->txRobotRefPosX->value();
				position.c.Y = ui->txRobotRefPosY->value();
				position.c.Z = ui->txRobotRefPosZ->value();
				position.c.A = ui->txRobotRefPosA->value();
				position.c.B = ui->txRobotRefPosB->value();
				position.c.C = ui->txRobotRefPosC->value();
			}

			double acc_factor = ui->txRobotFBKVel->value();
			double dec_factor = ui->txRobotFBKAcc->value();
			double jerk_factor = ui->txRobotFBKJerk->value();
			double torque_factor = ui->txRobotFBKTor->value();
			MC_BUFFER_MODE buffermode = (MC_BUFFER_MODE)ui->cbRobotBufferMode->currentIndex();
			MC_TRANSITION_MODE transmode = (MC_TRANSITION_MODE)ui->cbRobotTransMode->currentIndex();

			double trans_vel_factor = ui->txRobotTransVelFactor->value();
			smc_robot_set_profile_factor(num, acc_factor, dec_factor, jerk_factor, torque_factor);
			smc_robot_set_profile_transmode_start_velocity(num, trans_vel_factor, false);
		
			smc_robot_ptp_jump(num, coord, position.c);
		}
	});
}

#pragma region IO控制

// IO动作关联
void ScaraRobot::ui_init_output_action() {
	//connect(ui->ckOut00, &QCheckBox::stateChanged, this, [=](bool checked) {
	//	smc_io_set_dq(0, 0, checked);
	//});

	//connect(ui->ckOut01, &QCheckBox::stateChanged, this, [=](bool checked) {
	//	smc_io_set_dq(0, 1, checked);
	//});

	//connect(ui->ckOut02, &QCheckBox::stateChanged, this, [=](bool checked) {
	//	smc_io_set_dq(0, 2, checked);
	//});

	//connect(ui->ckOut03, &QCheckBox::stateChanged, this, [=](bool checked) {
	//	smc_io_set_dq(0, 3, checked);
	//});

	//connect(ui->ckOut04, &QCheckBox::stateChanged, this, [=](bool checked) {
	//	smc_io_set_dq(0, 4, checked);
	//});

	//connect(ui->ckOut05, &QCheckBox::stateChanged, this, [=](bool checked) {
	//	smc_io_set_dq(0, 5, checked);
	//});

	//connect(ui->ckOut06, &QCheckBox::stateChanged, this, [=](bool checked) {
	//	smc_io_set_dq(0, 6, checked);
	//});

	//connect(ui->ckOut07, &QCheckBox::stateChanged, this, [=](bool checked) {
	//	smc_io_set_dq(0, 7, checked);
	//});
}

// 更新IO显示
void ScaraRobot::ui_update_inout() {
	//ET_RESULT result = ET_RESULT_NO_ERROR;

	//uint32_t input;
	//result = smc_io_get_di_32bit(0, input);
	//for (int i = 0; i < 8; i++) {		
	//	bool bit = ((input >> i) & 0x01) == 0x01;
	//	switch (i) {
	//	case 0:ui->ckIn00->setChecked(bit); break;
	//	case 1:ui->ckIn01->setChecked(bit); break;
	//	case 2:ui->ckIn02->setChecked(bit); break;
	//	case 3:ui->ckIn03->setChecked(bit); break;
	//	case 4:ui->ckIn04->setChecked(bit); break;
	//	case 5:ui->ckIn05->setChecked(bit); break;
	//	case 6:ui->ckIn06->setChecked(bit); break;
	//	case 7:ui->ckIn07->setChecked(bit); break;
	//	default:
	//		break;
	//	}
	//}

	//uint32_t output;
	//static uint32_t last_output;
	//result = smc_io_get_dq_32bit(0, output);
	//if (last_output != output && result == ET_RESULT_NO_ERROR) {
	//	for (int i = 0; i < 8; i++) {
	//		bool bit = ((output >> i) & 0x01) == 0x01;
	//		switch (i) {
	//		case 0:ui->ckOut00->setChecked(bit); break;
	//		case 1:ui->ckOut01->setChecked(bit); break;
	//		case 2:ui->ckOut02->setChecked(bit); break;
	//		case 3:ui->ckOut03->setChecked(bit); break;
	//		case 4:ui->ckOut04->setChecked(bit); break;
	//		case 5:ui->ckOut05->setChecked(bit); break;
	//		case 6:ui->ckOut06->setChecked(bit); break;
	//		case 7:ui->ckOut07->setChecked(bit); break;
	//		default:
	//			break;
	//		}			
	//	}	
	//	last_output = output;
	//}	
}

#pragma endregion

#pragma region 单轴参数

void ScaraRobot::ui_update_axis_param(int axis_no) {
	double ref_pos, ref_vel, ref_acc, ref_dec, ref_jerk;
	MC_BUFFER_MODE ref_mode;
	smc_axis_get_profile_setting(axis_no, ref_pos, ref_vel, ref_acc, ref_dec, ref_jerk, ref_mode);

	ui->txAxisRefPos->setValue(ref_pos);
	ui->txAxisRefVel->setValue(ref_vel);
	ui->txAxisRefAcc->setValue(ref_acc);
	ui->txAxisRefDec->setValue(ref_dec);
	ui->txAxisRefJerk->setValue(ref_jerk);
	ui->cbAxisBufferMode->setCurrentIndex(ref_mode);

	double ref_estop_dec, ref_estop_jerk;
	int16_t ref_estop_module, ref_estop_io;
	smc_axis_get_estop_ramp(axis_no, ref_estop_dec, ref_estop_jerk);
	smc_axis_get_estop_io(axis_no, ref_estop_module, ref_estop_io);	

	ui->txAxisRefEStopDec->setValue(ref_estop_dec);
	ui->txAxisRefEStopJerk->setValue(ref_estop_jerk);
	ui->txAxisRefEStopModule->setValue(ref_estop_module);
	ui->txAxisRefEStopIO->setValue(ref_estop_io);
	
	double inch_dist, jog_vel, jog_acc, jog_dec, jog_jerk;
	smc_axis_get_jog_param(axis_no, inch_dist, jog_vel, jog_acc, jog_dec, jog_jerk);

	ui->txAxisInchDist->setValue(inch_dist);
	ui->txAxisJogVel->setValue(jog_vel);
	ui->txAxisJogAcc->setValue(jog_acc);
	ui->txAxisJogDec->setValue(jog_dec);
	ui->txAxisJogJerk->setValue(jog_jerk);
	
	// 回零参数	
	double home_pos, home_vel_low, home_vel_high, home_acc, home_dec, home_jerk;	
	smc_axis_get_home_param(axis_no, home_pos, home_vel_low, home_vel_high, home_acc, home_dec, home_jerk);
		
	ui->txAxisHomePos->setValue(home_pos);
	ui->txAxisHomeHVel->setValue(home_vel_high);
	ui->txAxisHomeLVel->setValue(home_vel_low);
	ui->txAxisHomeAcc->setValue(home_acc);
	ui->txAxisHomeDec->setValue(home_dec);
	ui->txAxisHomeJerk->setValue(home_jerk);
	
	// 驱动器回零
	int16_t axis_index;
	uint16_t drv_home_mode;
	double drv_home_offset;
	bool en_sdo,en_conv;
	smc_axis_get_drv_home_param(axis_no, drv_home_mode, en_sdo, en_conv);

	ui->txAxisDrvHomeMode->setValue(drv_home_mode);
	ui->txAxisDrvHomeOffset->setValue(drv_home_offset);
	ui->ckAxisHomeEnSdo->setChecked(en_sdo);
	ui->ckAxisHomeEnConv->setChecked(en_conv);
	
	// 控制器回零
	SMC_HOME_MODE plc_home_mode;
	double signal_delay, home_index;
	bool return_to_zero, home_ignore_hw;
	int16_t switch_module, switch_no, index_module, index_no;
	smc_axis_get_plc_home_param(axis_no, plc_home_mode, signal_delay, home_index, return_to_zero, home_ignore_hw);
	smc_axis_get_plc_home_io(axis_no, switch_module, switch_no, index_module, index_no);

	//ui->cbAxisSmcHomeMode->setCurrentIndex(plc_home_mode);
	ui->txAxisHomeSwitchDelay->setValue(signal_delay);
	ui->txAxisHomePls->setValue(home_index);
	ui->ckHomeRetrunZero->setChecked(return_to_zero);
	ui->ckIgnoreHw->setChecked(home_ignore_hw);
	ui->txAxisHomeSwitchModule->setValue(switch_module);
	ui->txAxisHomeSwitchIO->setValue(switch_no);
	ui->txAxisHomeIndexModule->setValue(index_module);
	ui->txAxisHomeIndexIO->setValue(index_no);
}

#pragma endregion


// 更新轴输出信息
void ScaraRobot::ui_update_axis_output(int axis_no) {
	bool virt, comm_status, power_status, ref_status, stopped_status;
	bool home_status, sync_status, alm_status;
	int16_t axis_id;
	uint16_t error_id;
	uint32_t alarm_id;
	bool done, busy, error;
	double pos, vel;

	SMC_AXIS_STATE axis_status;
	smc_axis_common_status(axis_no, axis_id, virt, comm_status, power_status, ref_status);
	smc_axis_motion_status(axis_no, axis_status, stopped_status, home_status, sync_status);
	smc_axis_alarm_status(axis_no, alm_status, alarm_id);
	smc_axis_command_result(axis_no, &done, &busy, &error, &error_id);

	AXIS_DIGITAL_INPUTS di_inputs;
	smc_axis_get_drv_inputs(axis_no, di_inputs);

	static bool last_power;
	if (last_power != power_status) {
		ui->btAxisPowerOn->setChecked(power_status);
		ui->btAxisPowerOn->setText(power_status ? QStringLiteral("关使能") : QStringLiteral("开使能"));
		last_power = power_status;
	}
	
	ET_AXISMODULE_CMD ctrl_status;
	smc_axis_control_status(axis_no, ctrl_status);	
	smc_axis_actual_position(axis_no, pos);
	smc_axis_actual_velocity(axis_no, vel);

	int16_t switch_module, switch_no, index_module, index_no;
	smc_axis_get_plc_home_io(axis_no, switch_module, switch_no, index_module, index_no);
	if (switch_module > 0 && switch_no > 0) {
		bool value;
		smc_io_get_di(switch_module, switch_no, value);
		ui->ckSmcHomeIO->setChecked(value);
	} else {
		ui->ckSmcHomeIO->setChecked(false);
	}

	if (index_module > 0 && index_no > 0) {
		bool value;
		smc_io_get_di(index_module, index_no, value);
		ui->ckSmcHomeIndex->setChecked(value);
	} else {
		ui->ckSmcHomeIndex->setChecked(false);
	}

	AXIS_INFORMATION info;
	smc_axis_get_information(axis_no, info);

	ui->spCtrlId->setValue(axis_id);
	ui->ckVirtual->setChecked(virt);
	ui->ckRotary->setChecked(info.xRotary);
	ui->ckModulo->setChecked(info.xModulo);
	ui->ckCommStatus->setChecked(comm_status);
	ui->ckPowerStatus->setChecked(power_status);
	ui->ckRefStatus->setChecked(ref_status);
	ui->ckStoppedStatus->setChecked(stopped_status);
	ui->ckHomingStatus->setChecked(home_status);
	ui->ckSyncStatus->setChecked(sync_status);
	ui->ckAlmStatus->setChecked(alm_status);
	ui->ckDone->setChecked(done);
	ui->ckBusy->setChecked(busy);
	ui->ckError->setChecked(error);
	ui->spCtrlStatus->setValue(ctrl_status);
	ui->spAxisStatus->setValue(axis_status);
	ui->spErrorId->setValue(error_id);
	ui->spAxisErrorId->setValue(alarm_id);
	ui->spAxisActPos->setValue(pos);
	ui->spAxisActVel->setValue(vel);
	ui->ckDINegativeLimit->setChecked(di_inputs.xDINegativeLimit);
	ui->ckDIPositiveLimit->setChecked(di_inputs.xDIPositiveLimit);
	ui->ckDIHomeSwitch->setChecked(di_inputs.xDIHomeSwitch);
	ui->ckDIQuickStop->setChecked(di_inputs.xDIQuickStop);
	ui->ckDIVoltageOk->setChecked(di_inputs.xDIVoltageOk);
	ui->ckDIBrakeReleased->setChecked(di_inputs.xDIBrakeReleased);
	ui->ckDIWarning->setChecked(di_inputs.xDIWarning);
	ui->ckDIReady->setChecked(di_inputs.xDIReady);
	ui->ckDIFault->setChecked(di_inputs.xDIFault);
	ui->spAxisModulo->setValue(info.rPositionPeriod);
	ui->spAxisMaxVel->setValue(info.rSWMaxVelocity);
	ui->spAxisMaxAcc->setValue(info.rSWMaxAcceleration);
	ui->spAxisMaxDec->setValue(info.rSWMaxDeceleration);
	ui->spAxisMaxJerk->setValue(info.rSWMaxJerk);
}

// 更新机器人参数
void ScaraRobot::ui_update_robot_param(int robot_no) {	
	SMC_COORD_SYSTEM coord;
	SMC_POS_REF ref_pos;
	MC_BUFFER_MODE ref_mode;
	MC_TRANSITION_MODE trans_mode;
	SMC_ORIENTATION_MODE orien_mode;
	double ref_vel, ref_acc, ref_dec, ref_jerk;
	double fac_vel, fac_acc, fac_jerk, fac_tor;
	smc_robot_get_profile_coordinate(robot_no, coord);
	smc_robot_get_profile_factor(robot_no, fac_vel, fac_acc, fac_jerk, fac_tor);
	smc_robot_get_profile_position(robot_no, ref_pos);
	smc_robot_get_profile_velocity(robot_no, ref_vel);
	smc_robot_get_profile_ramp(robot_no, ref_acc, ref_dec, ref_jerk);
	smc_robot_get_profile_buffermode(robot_no, ref_mode);
	smc_robot_get_profile_transmode(robot_no, trans_mode);
	smc_robot_get_profile_orienmode(robot_no, orien_mode);

	ui->cbRobotCoord->setCurrentIndex(coord);
	if (ui->cbRobotCoord->currentIndex() == 0) {
		ui->txRobotRefPosA1->setValue(ref_pos.a.a0);
		ui->txRobotRefPosA2->setValue(ref_pos.a.a1);
		ui->txRobotRefPosA3->setValue(ref_pos.a.a2);
		ui->txRobotRefPosA4->setValue(ref_pos.a.a3);
		ui->txRobotRefPosA5->setValue(ref_pos.a.a4);
		ui->txRobotRefPosA6->setValue(ref_pos.a.a5);
	} else {
		ui->txRobotRefPosX->setValue(ref_pos.c.X);
		ui->txRobotRefPosY->setValue(ref_pos.c.Y);
		ui->txRobotRefPosZ->setValue(ref_pos.c.Z);
		ui->txRobotRefPosA->setValue(ref_pos.c.A);
		ui->txRobotRefPosB->setValue(ref_pos.c.B);
		ui->txRobotRefPosC->setValue(ref_pos.c.C);
	}
		
	ui->txRobotPathVel->setValue(ref_vel);
	ui->txRobotPathAcc->setValue(ref_acc);
	ui->txRobotPathDec->setValue(ref_dec);
	ui->txRobotPathJerk->setValue(ref_jerk);

	ui->txRobotFBKVel->setValue(fac_vel);
	ui->txRobotFBKAcc->setValue(fac_acc);
	ui->txRobotFBKJerk->setValue(fac_jerk);
	ui->txRobotFBKTor->setValue(fac_tor);	

	ui->cbRobotBufferMode->setCurrentIndex(ref_mode);
	ui->cbRobotTransMode->setCurrentIndex(trans_mode);
	ui->cbRobotOriMode->setCurrentIndex(orien_mode);

	double vel_factor, corner_dist, line_dist, ang_dist;
	bool euc_done, blend_with_aux;
	smc_robot_get_profile_transmode_param(robot_no, vel_factor, corner_dist, line_dist, ang_dist, euc_done, blend_with_aux);

	ui->txRobotTransVelFactor->setValue(vel_factor);	
	ui->txRobotTransCornerDist->setValue(corner_dist);
	ui->txRobotTransLineDist->setValue(line_dist);
	ui->txRobotTransAngDist->setValue(ang_dist);
	ui->ckRobotTransEucDone->setChecked(euc_done);
	ui->ckRobotTransBlendAux->setChecked(blend_with_aux);

	double stop_dec, stop_jerk, stop_dec_factor, stop_jerk_factor, stop_tor_factor;
	bool stop_clr_mv;
	int16_t stop_module_no, stop_io_no;
	smc_robot_get_estop_ramp(robot_no, stop_dec, stop_jerk);
	smc_robot_get_estop_factor(robot_no, stop_dec_factor, stop_jerk_factor, stop_tor_factor);
	smc_robot_get_estop_clear_movement(robot_no, stop_clr_mv);
	smc_robot_get_estop_io(robot_no, stop_module_no, stop_io_no);

	ui->btRobotEStopDec->setValue(stop_dec);
	ui->btRobotEStopJerk->setValue(stop_jerk);
	ui->btRobotEStopKAcc->setValue(stop_dec_factor);
	ui->btRobotEStopKJerk->setValue(stop_jerk_factor);
	ui->btRobotEStopKTor->setValue(stop_tor_factor);
	ui->btRobotEStopModule->setValue(stop_module_no);
	ui->btRobotEStopIO->setValue(stop_io_no);
	ui->cbRobotEStopClear->setChecked(stop_clr_mv);

	double jog_line_dist, jog_ang_dist, jog_vel, jog_acc, jog_dec, jog_jerk;
	double jog_fac_vel, jog_fac_acc, jog_fac_jerk, jog_fac_tor;
	smc_robot_get_jog_param(robot_no, jog_line_dist, jog_ang_dist, jog_vel, jog_acc, jog_dec, jog_jerk);
	smc_robot_get_jog_factor(robot_no, jog_fac_vel, jog_fac_acc, jog_fac_jerk, jog_fac_tor);
	
	ui->txRobotJogDist->setValue(jog_line_dist);
	ui->txRobotJogAng->setValue(jog_ang_dist);
	ui->txRobotJogVel->setValue(jog_vel);
	ui->txRobotJogAcc->setValue(jog_acc);
	ui->txRobotJogDec->setValue(jog_dec);
	ui->txRobotJogJerk->setValue(jog_jerk);

	ui->txRobotJogKVel->setValue(jog_fac_vel);
	ui->txRobotJogKAcc->setValue(jog_fac_acc);
	ui->txRobotJogKJerk->setValue(jog_fac_jerk);
	ui->txRobotJogKTor->setValue(jog_fac_tor);

	ui->rbScaraLeft->setChecked(true);
	ui->rbScaraRight->setChecked(false);

	double safe_height;
	smc_robot_get_safe_height(robot_no, safe_height);
	ui->txRobotSafeHeight->setValue(safe_height);
}

// 更新机器人输出信息
void ScaraRobot::ui_update_robot_output(int robot_no) {
	bool en_status, comm_status, power_status, ref_status, stopped_status;
	bool home_status, sync_status, track_status, alm_status;
	uint32_t alarm_id;
	uint16_t error_id;
	bool done, busy, error, active, cmd_aborted, cmd_accepted;

	smc_robot_common_status(robot_no, en_status, comm_status, power_status, ref_status);

	static bool last_en, last_power;
	if (last_en != en_status) {
		ui->ckRobotEnable->setChecked(en_status);
		last_en = en_status;
	}
	if (last_power != power_status) {
		ui->ckRobotPowerOn->setChecked(power_status);
		last_power = power_status;
	}

	SMC_ROBOT_STATE state;
	smc_robot_motion_status(robot_no, state, stopped_status, home_status, sync_status, track_status);
	smc_robot_alarm_status(robot_no, alm_status, alarm_id);
	smc_robot_command_result(robot_no, &done, &busy, &error,&error_id,&active,&cmd_aborted,&cmd_accepted);

	ui->ckRobotEnabled->setChecked(en_status);
	ui->ckRobotCommStatus->setChecked(comm_status);
	ui->ckRobotPowerStatus->setChecked(power_status);
	ui->ckRobotRefStatus->setChecked(ref_status);
	ui->ckRobotStoppedStatus->setChecked(stopped_status);
	ui->ckRobotTrackingStatus->setChecked(track_status);	
	ui->ckRobotDone->setChecked(done);
	ui->ckRobotBusy->setChecked(busy);
	ui->ckRobotActive->setChecked(active);
	ui->ckRobotAborted->setChecked(cmd_aborted);
	ui->ckRobotAccepted->setChecked(cmd_accepted);
	ui->ckRobotError->setChecked(error);
	ui->ckRobotAlarm->setChecked(alm_status);

	ET_ROBOTMODULE_CMD ctrl_status;
	smc_robot_control_status(robot_no, ctrl_status);

	ui->txRobotState->setValue(ctrl_status);
	ui->txRobotMotionState->setValue(state);
	ui->txRobotErrId->setValue(error_id);
	ui->txRobotAlarmId->setValue(alarm_id);
	
	ROBOT_MODULE_OUTPUTS robot_out;
	smc_robot_total_output(robot_no, robot_out);
	ui->txRobotMvId->setValue(robot_out.ulMovementId);
	ui->txRobotActiveId->setValue(robot_out.ulActiveMovementId);
	ui->txRobotFirstRefId->setValue(robot_out.ulFirstRefMovementId);
	ui->txRobotLastAcceptedId->setValue(robot_out.ulLastAcceptedMovementId);

	ROBOT_MODULE_COORD pos, vel;
	smc_robot_actual_position(robot_no, pos);
	smc_robot_actual_velocity(robot_no, vel);

	ui->txRobotAcsPos_A1->setValue(pos.stValInAcs.a0);
	ui->txRobotAcsPos_A2->setValue(pos.stValInAcs.a1);
	ui->txRobotAcsPos_A3->setValue(pos.stValInAcs.a2);
	ui->txRobotAcsPos_A4->setValue(pos.stValInAcs.a3);
	ui->txRobotAcsPos_A5->setValue(pos.stValInAcs.a4);
	ui->txRobotAcsPos_A6->setValue(pos.stValInAcs.a5);

	ui->txRobotMcsPos_X->setValue(pos.stValInMcs.X);
	ui->txRobotMcsPos_Y->setValue(pos.stValInMcs.Y);
	ui->txRobotMcsPos_Z->setValue(pos.stValInMcs.Z);
	ui->txRobotMcsPos_A->setValue(pos.stValInMcs.A);
	ui->txRobotMcsPos_B->setValue(pos.stValInMcs.B);
	ui->txRobotMcsPos_C->setValue(pos.stValInMcs.C);

	ui->txRobotWcsPos_X->setValue(pos.stValInWcs.X);
	ui->txRobotWcsPos_Y->setValue(pos.stValInWcs.Y);
	ui->txRobotWcsPos_Z->setValue(pos.stValInWcs.Z);
	ui->txRobotWcsPos_A->setValue(pos.stValInWcs.A);
	ui->txRobotWcsPos_B->setValue(pos.stValInWcs.B);
	ui->txRobotWcsPos_C->setValue(pos.stValInWcs.C);

	ui->txRobotPcs1Pos_X->setValue(pos.stValInPcsPick.X);
	ui->txRobotPcs1Pos_Y->setValue(pos.stValInPcsPick.Y);
	ui->txRobotPcs1Pos_Z->setValue(pos.stValInPcsPick.Z);
	ui->txRobotPcs1Pos_A->setValue(pos.stValInPcsPick.A);
	ui->txRobotPcs1Pos_B->setValue(pos.stValInPcsPick.B);
	ui->txRobotPcs1Pos_C->setValue(pos.stValInPcsPick.C);

	ui->txRobotPcs2Pos_X->setValue(pos.stValInPcsPlace.X);
	ui->txRobotPcs2Pos_Y->setValue(pos.stValInPcsPlace.Y);
	ui->txRobotPcs2Pos_Z->setValue(pos.stValInPcsPlace.Z);
	ui->txRobotPcs2Pos_A->setValue(pos.stValInPcsPlace.A);
	ui->txRobotPcs2Pos_B->setValue(pos.stValInPcsPlace.B);
	ui->txRobotPcs2Pos_C->setValue(pos.stValInPcsPlace.C);

	ui->txRobotTcsPos_X->setValue(pos.stValInTcs.X);
	ui->txRobotTcsPos_Y->setValue(pos.stValInTcs.Y);
	ui->txRobotTcsPos_Z->setValue(pos.stValInTcs.Z);
	ui->txRobotTcsPos_A->setValue(pos.stValInTcs.A);
	ui->txRobotTcsPos_B->setValue(pos.stValInTcs.B);
	ui->txRobotTcsPos_C->setValue(pos.stValInTcs.C);
}

// 更新Intp参数
void ScaraRobot::ui_update_intp_param(int intp_no) {
	//MC_COORD_REF pos;
	//SMC_VECTOR3D offset;
	//double vel, acc, dec, jerk;
	//ET_RESULT result = smc_intp_xyz_get_profile_param(intp_no, pos, offset, vel, acc, dec, jerk);

	//if (result == ET_RESULT_NO_ERROR) {
	//	ui->txIntpRefVel->setValue(vel);
	//	ui->txIntpRefAcc->setValue(acc);
	//	ui->txIntpRefDec->setValue(dec);
	//	ui->txIntpRefJerk->setValue(jerk);

	//	ui->txIntpRefPosX->setValue(pos.X);
	//	ui->txIntpRefPosY->setValue(pos.Y);
	//	ui->txIntpRefPosZ->setValue(pos.Z);
	//	ui->txIntpRefPosC->setValue(pos.C);

	//	ui->txIntpRefOffsetX->setValue(offset.dX);
	//	ui->txIntpRefOffsetY->setValue(offset.dY);
	//}	
}

// 更新Intp输出信息
void ScaraRobot::ui_update_intp_output(int intp_no) {
	//bool power, ready, done, busy, error;	
	//smc_intp_xyz_command_result(intp_no, &power, &ready, &done, &busy, &error);

	//ui->ckIntpPowerOn->setChecked(power);
	//ui->ckIntpReady->setChecked(ready);
	//ui->ckIntpDone->setChecked(done);
	//ui->ckIntpBusy->setChecked(busy);
	//ui->ckIntpError->setChecked(error);
}

// 定时刷新界面显示
void ScaraRobot::timerEvent(QTimerEvent* event) {
	// 未连接则退出
	if (!m_connected)
		return;	

	// 更新IO输入输出
	ui_update_inout();

	// 更新轴输出
	bool isOk = false;
	int num = ui->cbAxisSel->currentText().toInt(&isOk);
	if (isOk) {
		ui_update_axis_output(num);
	}

	// 更新机器人输出
	num = ui->cbRobotSel->currentText().toInt(&isOk);
	if (isOk) {
		ui_update_robot_output(num);
	}

	ui_update_intp_output(0);
}