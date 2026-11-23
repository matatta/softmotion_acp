/********************************************************************************
** Form generated from reading UI file 'scararobot.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCARAROBOT_H
#define UI_SCARAROBOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScaraRobotClass
{
public:
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_6;
    QLabel *label_7;
    QPushButton *btAcpConnect;
    QLabel *label_6;
    QLineEdit *txAcpIp;
    QPushButton *btAcpDisconnect;
    QSpacerItem *horizontalSpacer;
    QFrame *fmState;
    QLabel *label_14;
    QComboBox *cbAcpMode;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QGridLayout *gridLayout_13;
    QGroupBox *groupBox_22;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_25;
    QFormLayout *formLayout_2;
    QLabel *lbHomeStatus_109;
    QDoubleSpinBox *txIntpRefVel;
    QLabel *lbHomeStatus_100;
    QDoubleSpinBox *txIntpRefAcc;
    QLabel *lbHomeStatus_104;
    QDoubleSpinBox *txIntpRefDec;
    QLabel *lbHomeStatus_108;
    QDoubleSpinBox *txIntpRefJerk;
    QGroupBox *groupBox_23;
    QFormLayout *formLayout;
    QLabel *lbHomeStatus_81;
    QDoubleSpinBox *txIntpRefPosX;
    QLabel *lbHomeStatus_106;
    QDoubleSpinBox *txIntpRefPosY;
    QLabel *lbHomeStatus_101;
    QDoubleSpinBox *txIntpRefPosZ;
    QLabel *lbHomeStatus_99;
    QDoubleSpinBox *txIntpRefPosC;
    QGroupBox *groupBox_26;
    QGridLayout *gridLayout_11;
    QLabel *lbHomeStatus_110;
    QPushButton *btIntpPowerOn;
    QDoubleSpinBox *txIntpRefOffsetX;
    QLabel *lbHomeStatus_111;
    QDoubleSpinBox *txIntpRefOffsetY;
    QSpacerItem *verticalSpacer_6;
    QGroupBox *groupBox_30;
    QVBoxLayout *verticalLayout_6;
    QPushButton *btIntpMvAbs;
    QPushButton *btIntpMvRel;
    QPushButton *btIntpMvHalt;
    QPushButton *btIntpMvReset;
    QGroupBox *groupBox_29;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *ckIntpPowerOn;
    QCheckBox *ckIntpReady;
    QCheckBox *ckIntpDone;
    QCheckBox *ckIntpBusy;
    QCheckBox *ckIntpError;
    QGroupBox *groupBox_20;
    QGridLayout *gridLayout_8;
    QCheckBox *ckHomeRetrunZero;
    QLabel *lbHomeStatus_60;
    QCheckBox *ckSmcHomeIO;
    QLabel *lbHomeStatus_58;
    QLabel *lbHomeStatus_56;
    QLabel *lbHomeStatus_53;
    QSpinBox *txAxisHomeIndexModule;
    QLabel *lbHomeStatus_57;
    QSpinBox *txAxisHomeSwitchModule;
    QCheckBox *ckSmcHomeIndex;
    QCheckBox *ckIgnoreHw;
    QPushButton *btPlcHome;
    QSpinBox *txAxisHomeIndexIO;
    QSpinBox *txAxisHomeSwitchIO;
    QDoubleSpinBox *txAxisHomePls;
    QDoubleSpinBox *txAxisHomeSwitchDelay;
    QComboBox *cbAxisSmcHomeMode;
    QGroupBox *groupBox_15;
    QGridLayout *gridLayout_10;
    QLabel *lbHomeStatus_41;
    QDoubleSpinBox *txAxisRefEStopDec;
    QLabel *lbHomeStatus_42;
    QDoubleSpinBox *txAxisRefEStopJerk;
    QLabel *lbHomeStatus_43;
    QSpinBox *txAxisRefEStopModule;
    QSpinBox *txAxisRefEStopIO;
    QPushButton *btAxisEStop;
    QGroupBox *groupBox_14;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbHomeStatus_24;
    QComboBox *cbAxisSel;
    QLabel *lbHomeStatus_35;
    QDoubleSpinBox *spAxisActPos;
    QLabel *lbHomeStatus_34;
    QDoubleSpinBox *spAxisActVel;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btAxisPowerOn;
    QGroupBox *groupBox_18;
    QGridLayout *gridLayout_17;
    QLabel *lbHomeStatus_59;
    QLabel *label_12;
    QPushButton *btSdoWrite;
    QLabel *label_11;
    QSpinBox *txSdoSubIndex;
    QSpinBox *txSdoIndex;
    QLabel *lbHomeStatus_55;
    QPushButton *btSdoRead;
    QLabel *label_15;
    QSpinBox *txSdoDataLen;
    QDoubleSpinBox *txSdoReadValue;
    QDoubleSpinBox *txSdoWriteValue;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_14;
    QLabel *lbHomeStatus_45;
    QDoubleSpinBox *txAxisHomePos;
    QLabel *lbHomeStatus_47;
    QDoubleSpinBox *txAxisHomeHVel;
    QLabel *lbHomeStatus_51;
    QDoubleSpinBox *txAxisHomeLVel;
    QLabel *lbHomeStatus_48;
    QDoubleSpinBox *txAxisHomeAcc;
    QLabel *lbHomeStatus_46;
    QDoubleSpinBox *txAxisHomeDec;
    QLabel *lbHomeStatus_49;
    QDoubleSpinBox *txAxisHomeJerk;
    QPushButton *btAxisHomeParam;
    QCheckBox *ckAxisHomeEnConv;
    QGroupBox *groupBox_16;
    QGridLayout *gridLayout_9;
    QLabel *lbHomeStatus_29;
    QDoubleSpinBox *txAxisJogVel;
    QDoubleSpinBox *txAxisJogDec;
    QLabel *lbHomeStatus_38;
    QLabel *lbHomeStatus_36;
    QDoubleSpinBox *txAxisJogAcc;
    QLabel *lbHomeStatus_37;
    QDoubleSpinBox *txAxisJogJerk;
    QLabel *lbHomeStatus_61;
    QDoubleSpinBox *txAxisInchDist;
    QPushButton *btAxisJogFwd;
    QPushButton *btAxisJogBwd;
    QCheckBox *cbAxisInch;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_7;
    QLabel *lbHomeStatus_33;
    QDoubleSpinBox *txAxisRefPos;
    QPushButton *btAxisMoveAbs;
    QLabel *lbHomeStatus_31;
    QDoubleSpinBox *txAxisRefVel;
    QPushButton *btAxisMoveRel;
    QLabel *lbHomeStatus_32;
    QDoubleSpinBox *txAxisRefAcc;
    QPushButton *btAxisMoveVel;
    QLabel *lbHomeStatus_30;
    QDoubleSpinBox *txAxisRefDec;
    QPushButton *btAxisMoveHalt;
    QLabel *lbHomeStatus_26;
    QDoubleSpinBox *txAxisRefJerk;
    QLabel *lbHomeStatus_27;
    QComboBox *cbAxisBufferMode;
    QPushButton *btAxisSetPos;
    QPushButton *btAxisReset;
    QGroupBox *groupBox_13;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *ckVirtual;
    QCheckBox *ckCommStatus;
    QCheckBox *ckRefStatus;
    QCheckBox *ckPowerStatus;
    QCheckBox *ckStoppedStatus;
    QCheckBox *ckHomingStatus;
    QCheckBox *ckSyncStatus;
    QCheckBox *ckDone;
    QCheckBox *ckBusy;
    QCheckBox *ckError;
    QCheckBox *ckAlmStatus;
    QFrame *line_4;
    QCheckBox *ckDINegativeLimit;
    QCheckBox *ckDIPositiveLimit;
    QCheckBox *ckDIHomeSwitch;
    QCheckBox *ckDIQuickStop;
    QCheckBox *ckDIVoltageOk;
    QCheckBox *ckDIBrakeReleased;
    QCheckBox *ckDIWarning;
    QCheckBox *ckDIReady;
    QCheckBox *ckDIFault;
    QFrame *line_2;
    QFormLayout *formLayout_3;
    QCheckBox *ckModulo;
    QCheckBox *ckRotary;
    QLabel *lbHomeStatus_80;
    QDoubleSpinBox *spAxisModulo;
    QLabel *lbHomeStatus_76;
    QDoubleSpinBox *spAxisMaxVel;
    QLabel *lbHomeStatus_77;
    QDoubleSpinBox *spAxisMaxAcc;
    QLabel *lbHomeStatus_78;
    QDoubleSpinBox *spAxisMaxDec;
    QLabel *lbHomeStatus_79;
    QDoubleSpinBox *spAxisMaxJerk;
    QFrame *line_3;
    QFormLayout *formLayout_4;
    QLabel *lbHomeStatus_44;
    QSpinBox *spCtrlStatus;
    QLabel *lbHomeStatus_39;
    QSpinBox *spErrorId;
    QLabel *lbHomeStatus_40;
    QSpinBox *spAxisErrorId;
    QLabel *lbHomeStatus_25;
    QSpinBox *spAxisStatus;
    QLabel *lbHomeStatus_74;
    QSpinBox *spCtrlId;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_19;
    QGridLayout *gridLayout_15;
    QLabel *lbHomeStatus_52;
    QDoubleSpinBox *txAxisDrvHomeOffset;
    QLabel *lbHomeStatus_54;
    QSpinBox *txAxisDrvHomeMode;
    QCheckBox *ckAxisHomeEnSdo;
    QPushButton *btDrvHome;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QGridLayout *gridLayout_12;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *rbScaraLeft;
    QRadioButton *rbScaraRight;
    QPushButton *btScaraHandSet;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_5;
    QComboBox *cbRobotSel;
    QLabel *lbHomeStatus_6;
    QLabel *lbHomeStatus_7;
    QLabel *lbHomeStatus_18;
    QLabel *lbHomeStatus_20;
    QLabel *lbHomeStatus_50;
    QLabel *label_19;
    QLabel *label_24;
    QDoubleSpinBox *txRobotAcsPos_A1;
    QDoubleSpinBox *txRobotAcsPos_A2;
    QDoubleSpinBox *txRobotAcsPos_A3;
    QDoubleSpinBox *txRobotAcsPos_A4;
    QDoubleSpinBox *txRobotAcsPos_A5;
    QDoubleSpinBox *txRobotAcsPos_A6;
    QLabel *label_23;
    QDoubleSpinBox *txRobotMcsPos_X;
    QDoubleSpinBox *txRobotMcsPos_Y;
    QDoubleSpinBox *txRobotMcsPos_Z;
    QDoubleSpinBox *txRobotMcsPos_A;
    QDoubleSpinBox *txRobotMcsPos_B;
    QDoubleSpinBox *txRobotMcsPos_C;
    QLabel *label_21;
    QDoubleSpinBox *txRobotWcsPos_X;
    QDoubleSpinBox *txRobotWcsPos_Y;
    QDoubleSpinBox *txRobotWcsPos_Z;
    QDoubleSpinBox *txRobotWcsPos_A;
    QDoubleSpinBox *txRobotWcsPos_B;
    QDoubleSpinBox *txRobotWcsPos_C;
    QLabel *label_20;
    QDoubleSpinBox *txRobotPcs1Pos_X;
    QDoubleSpinBox *txRobotPcs1Pos_Y;
    QDoubleSpinBox *txRobotPcs1Pos_Z;
    QDoubleSpinBox *txRobotPcs1Pos_A;
    QDoubleSpinBox *txRobotPcs1Pos_B;
    QDoubleSpinBox *txRobotPcs1Pos_C;
    QLabel *label_22;
    QDoubleSpinBox *txRobotPcs2Pos_X;
    QDoubleSpinBox *txRobotPcs2Pos_Y;
    QDoubleSpinBox *txRobotPcs2Pos_Z;
    QDoubleSpinBox *txRobotPcs2Pos_A;
    QDoubleSpinBox *txRobotPcs2Pos_B;
    QDoubleSpinBox *txRobotPcs2Pos_C;
    QLabel *label_25;
    QDoubleSpinBox *txRobotTcsPos_X;
    QDoubleSpinBox *txRobotTcsPos_Y;
    QDoubleSpinBox *txRobotTcsPos_Z;
    QDoubleSpinBox *txRobotTcsPos_A;
    QDoubleSpinBox *txRobotTcsPos_B;
    QDoubleSpinBox *txRobotTcsPos_C;
    QGroupBox *groupBox_17;
    QGridLayout *gridLayout_23;
    QPushButton *btCAxisToolPeriod;
    QSpinBox *txRobotToolPeriod;
    QLabel *label;
    QGroupBox *groupBox_21;
    QGridLayout *gridLayout_32;
    QCheckBox *ckRobotEnabled;
    QCheckBox *ckRobotCommStatus;
    QCheckBox *ckRobotPowerStatus;
    QCheckBox *ckRobotRefStatus;
    QCheckBox *ckRobotStoppedStatus;
    QCheckBox *ckRobotHomingStatus;
    QCheckBox *ckRobotTrackingStatus;
    QCheckBox *ckRobotDone;
    QCheckBox *ckRobotBusy;
    QCheckBox *ckRobotActive;
    QCheckBox *ckRobotAborted;
    QCheckBox *ckRobotAccepted;
    QCheckBox *ckRobotError;
    QCheckBox *ckRobotAlarm;
    QFrame *line;
    QLabel *lbHomeStatus_62;
    QSpinBox *txRobotState;
    QLabel *lbHomeStatus_63;
    QSpinBox *txRobotErrId;
    QLabel *lbHomeStatus_64;
    QSpinBox *txRobotAlarmId;
    QLabel *lbHomeStatus_65;
    QSpinBox *txRobotMotionState;
    QLabel *lbHomeStatus_66;
    QSpinBox *txRobotMvId;
    QLabel *lbHomeStatus_67;
    QSpinBox *txRobotActiveId;
    QLabel *lbHomeStatus_68;
    QSpinBox *txRobotFirstRefId;
    QLabel *lbHomeStatus_69;
    QSpinBox *txRobotLastAcceptedId;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_26;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_8;
    QCheckBox *ckRobotEnable;
    QCheckBox *ckRobotPowerOn;
    QCheckBox *ckRobotRel;
    QPushButton *btRobotPTP;
    QPushButton *btRobotLinear;
    QPushButton *btRobotCirc;
    QPushButton *btRobotRdTool;
    QPushButton *btRobotWrTool;
    QPushButton *btRobotSetPos;
    QPushButton *btRobotHome;
    QPushButton *btRobotHalt;
    QPushButton *btRobotReset;
    QPushButton *btRobotJump;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout;
    QRadioButton *rbTargetPos;
    QComboBox *cbCircDir;
    QLabel *label_2;
    QComboBox *cbCircPath;
    QComboBox *cbRobotCoord;
    QLabel *lbCircDir;
    QLabel *lbCircPath;
    QRadioButton *rbMidPos;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_24;
    QStackedWidget *stkpos;
    QWidget *page_3;
    QGridLayout *gridLayout_20;
    QLabel *lbHomeStatus_10;
    QDoubleSpinBox *txRobotRefPosA1;
    QLabel *lbHomeStatus_21;
    QDoubleSpinBox *txRobotRefPosA2;
    QLabel *lbHomeStatus_22;
    QDoubleSpinBox *txRobotRefPosA3;
    QLabel *lbHomeStatus_23;
    QDoubleSpinBox *txRobotRefPosA4;
    QLabel *label_3;
    QDoubleSpinBox *txRobotRefPosA5;
    QLabel *label_4;
    QDoubleSpinBox *txRobotRefPosA6;
    QWidget *page_4;
    QGridLayout *gridLayout_18;
    QLabel *lbHomeStatus_72;
    QLabel *lbHomeStatus_98;
    QDoubleSpinBox *txRobotRefPosA;
    QDoubleSpinBox *txRobotRefPosX;
    QDoubleSpinBox *txRobotRefPosB;
    QLabel *lbHomeStatus_73;
    QDoubleSpinBox *txRobotRefPosC;
    QLabel *lbHomeStatus_71;
    QLabel *lbHomeStatus_70;
    QLabel *lbHomeStatus_96;
    QDoubleSpinBox *txRobotRefPosY;
    QDoubleSpinBox *txRobotRefPosZ;
    QGroupBox *groupBox_31;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lbHomeStatus_15;
    QDoubleSpinBox *txRobotSafeHeight;
    QPushButton *btRobotSetSafeHeight;
    QGroupBox *groupBox_27;
    QGridLayout *gridLayout_2;
    QPushButton *btRobotJogFwd;
    QLabel *lbHomeStatus_86;
    QComboBox *cbRobotJogCoord;
    QPushButton *btRobotJogBwd;
    QDoubleSpinBox *txRobotJogDec;
    QDoubleSpinBox *txRobotJogVel;
    QComboBox *cbRobotJogAxisSel;
    QCheckBox *ckRobotJogAbcAsAcs;
    QLabel *lbHomeStatus_90;
    QLabel *lbHomeStatus_91;
    QLabel *lbHomeStatus_97;
    QDoubleSpinBox *txRobotJogJerk;
    QLabel *lbHomeStatus_103;
    QDoubleSpinBox *txRobotJogKVel;
    QLabel *lbHomeStatus_102;
    QDoubleSpinBox *txRobotJogAcc;
    QDoubleSpinBox *txRobotJogDist;
    QLabel *lbHomeStatus_92;
    QDoubleSpinBox *txRobotJogKJerk;
    QLabel *lbHomeStatus_105;
    QDoubleSpinBox *txRobotJogKTor;
    QDoubleSpinBox *txRobotJogKAcc;
    QDoubleSpinBox *txRobotJogAng;
    QLabel *label_5;
    QGroupBox *groupBox_24;
    QGridLayout *gridLayout_21;
    QDoubleSpinBox *txRobotPathJerk;
    QLabel *lbHomeStatus_87;
    QComboBox *cbRobotOriMode;
    QLabel *lbHomeStatus_13;
    QDoubleSpinBox *txRobotFBKAcc;
    QDoubleSpinBox *txRobotFBKVel;
    QDoubleSpinBox *txRobotPathAcc;
    QStackedWidget *stktrans;
    QWidget *page_7;
    QGridLayout *gridLayout_27;
    QDoubleSpinBox *txRobotTransVelFactor;
    QLabel *lbHomeStatus_82;
    QSpacerItem *verticalSpacer_9;
    QWidget *page_8;
    QGridLayout *gridLayout_31;
    QDoubleSpinBox *txRobotTransCornerDist;
    QLabel *lbHomeStatus_83;
    QSpacerItem *verticalSpacer_3;
    QWidget *page_9;
    QGridLayout *gridLayout_25;
    QLabel *lbHomeStatus_85;
    QLabel *lbHomeStatus_84;
    QDoubleSpinBox *txRobotTransLineDist;
    QDoubleSpinBox *txRobotTransAngDist;
    QCheckBox *ckRobotTransEucDone;
    QDoubleSpinBox *txRobotPathVel;
    QLabel *lbHomeStatus_88;
    QComboBox *cbRobotTransMode;
    QLabel *lbHomeStatus_14;
    QComboBox *cbRobotBufferMode;
    QDoubleSpinBox *txRobotFBKTor;
    QLabel *lbHomeStatus_12;
    QDoubleSpinBox *txRobotPathDec;
    QCheckBox *ckRobotTransBlendAux;
    QLabel *lbHomeStatus_89;
    QDoubleSpinBox *txRobotFBKJerk;
    QLabel *lbHomeStatus_17;
    QLabel *lbHomeStatus_11;
    QGroupBox *groupBox_28;
    QGridLayout *gridLayout_3;
    QLabel *lbHomeStatus_93;
    QDoubleSpinBox *btRobotEStopDec;
    QDoubleSpinBox *btRobotEStopKAcc;
    QLabel *lbHomeStatus_94;
    QDoubleSpinBox *btRobotEStopJerk;
    QDoubleSpinBox *btRobotEStopKJerk;
    QLabel *lbHomeStatus_107;
    QDoubleSpinBox *btRobotEStopKTor;
    QLabel *lbHomeStatus_95;
    QSpinBox *btRobotEStopModule;
    QSpinBox *btRobotEStopIO;
    QSpacerItem *verticalSpacer_7;
    QCheckBox *cbRobotHaltClear;
    QCheckBox *cbRobotEStopClear;
    QPushButton *btRobotEStop;
    QSpacerItem *verticalSpacer_4;
    QWidget *tab;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label_9;
    QLineEdit *txAcpTagName;
    QLabel *label_13;
    QSpinBox *txAcpTagSize;
    QLabel *label_10;
    QDoubleSpinBox *txAcpTagValue;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btAcpRead;
    QPushButton *btAcpWrite;
    QPushButton *btTest;

    void setupUi(QWidget *ScaraRobotClass)
    {
        if (ScaraRobotClass->objectName().isEmpty())
            ScaraRobotClass->setObjectName(QString::fromUtf8("ScaraRobotClass"));
        ScaraRobotClass->resize(916, 1009);
        gridLayout_4 = new QGridLayout(ScaraRobotClass);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        groupBox_11 = new QGroupBox(ScaraRobotClass);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        gridLayout_6 = new QGridLayout(groupBox_11);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(6, 6, 6, 6);
        label_7 = new QLabel(groupBox_11);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);

        gridLayout_6->addWidget(label_7, 0, 6, 1, 1);

        btAcpConnect = new QPushButton(groupBox_11);
        btAcpConnect->setObjectName(QString::fromUtf8("btAcpConnect"));
        sizePolicy.setHeightForWidth(btAcpConnect->sizePolicy().hasHeightForWidth());
        btAcpConnect->setSizePolicy(sizePolicy);

        gridLayout_6->addWidget(btAcpConnect, 0, 12, 1, 1);

        label_6 = new QLabel(groupBox_11);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);

        gridLayout_6->addWidget(label_6, 0, 0, 1, 1);

        txAcpIp = new QLineEdit(groupBox_11);
        txAcpIp->setObjectName(QString::fromUtf8("txAcpIp"));
        sizePolicy.setHeightForWidth(txAcpIp->sizePolicy().hasHeightForWidth());
        txAcpIp->setSizePolicy(sizePolicy);

        gridLayout_6->addWidget(txAcpIp, 0, 1, 1, 1);

        btAcpDisconnect = new QPushButton(groupBox_11);
        btAcpDisconnect->setObjectName(QString::fromUtf8("btAcpDisconnect"));
        sizePolicy.setHeightForWidth(btAcpDisconnect->sizePolicy().hasHeightForWidth());
        btAcpDisconnect->setSizePolicy(sizePolicy);

        gridLayout_6->addWidget(btAcpDisconnect, 0, 13, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer, 0, 8, 1, 1);

        fmState = new QFrame(groupBox_11);
        fmState->setObjectName(QString::fromUtf8("fmState"));
        sizePolicy.setHeightForWidth(fmState->sizePolicy().hasHeightForWidth());
        fmState->setSizePolicy(sizePolicy);
        fmState->setMinimumSize(QSize(40, 0));
        fmState->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        fmState->setFrameShape(QFrame::StyledPanel);
        fmState->setFrameShadow(QFrame::Raised);

        gridLayout_6->addWidget(fmState, 0, 7, 1, 1);

        label_14 = new QLabel(groupBox_11);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        sizePolicy.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy);

        gridLayout_6->addWidget(label_14, 0, 2, 1, 1);

        cbAcpMode = new QComboBox(groupBox_11);
        cbAcpMode->addItem(QString());
        cbAcpMode->addItem(QString());
        cbAcpMode->setObjectName(QString::fromUtf8("cbAcpMode"));
        cbAcpMode->setEnabled(false);

        gridLayout_6->addWidget(cbAcpMode, 0, 3, 1, 1);


        gridLayout_4->addWidget(groupBox_11, 0, 0, 1, 1);

        tabWidget = new QTabWidget(ScaraRobotClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::West);
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        gridLayout_13 = new QGridLayout(tab_1);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        groupBox_22 = new QGroupBox(tab_1);
        groupBox_22->setObjectName(QString::fromUtf8("groupBox_22"));
        groupBox_22->setEnabled(false);
        horizontalLayout_3 = new QHBoxLayout(groupBox_22);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        groupBox_25 = new QGroupBox(groupBox_22);
        groupBox_25->setObjectName(QString::fromUtf8("groupBox_25"));
        formLayout_2 = new QFormLayout(groupBox_25);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        lbHomeStatus_109 = new QLabel(groupBox_25);
        lbHomeStatus_109->setObjectName(QString::fromUtf8("lbHomeStatus_109"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, lbHomeStatus_109);

        txIntpRefVel = new QDoubleSpinBox(groupBox_25);
        txIntpRefVel->setObjectName(QString::fromUtf8("txIntpRefVel"));
        txIntpRefVel->setReadOnly(false);
        txIntpRefVel->setDecimals(2);
        txIntpRefVel->setMinimum(-999999.000000000000000);
        txIntpRefVel->setMaximum(999999.989999999990687);
        txIntpRefVel->setValue(0.000000000000000);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, txIntpRefVel);

        lbHomeStatus_100 = new QLabel(groupBox_25);
        lbHomeStatus_100->setObjectName(QString::fromUtf8("lbHomeStatus_100"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, lbHomeStatus_100);

        txIntpRefAcc = new QDoubleSpinBox(groupBox_25);
        txIntpRefAcc->setObjectName(QString::fromUtf8("txIntpRefAcc"));
        txIntpRefAcc->setReadOnly(false);
        txIntpRefAcc->setDecimals(2);
        txIntpRefAcc->setMinimum(-999999.000000000000000);
        txIntpRefAcc->setMaximum(999999.989999999990687);
        txIntpRefAcc->setValue(0.000000000000000);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, txIntpRefAcc);

        lbHomeStatus_104 = new QLabel(groupBox_25);
        lbHomeStatus_104->setObjectName(QString::fromUtf8("lbHomeStatus_104"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, lbHomeStatus_104);

        txIntpRefDec = new QDoubleSpinBox(groupBox_25);
        txIntpRefDec->setObjectName(QString::fromUtf8("txIntpRefDec"));
        txIntpRefDec->setReadOnly(false);
        txIntpRefDec->setDecimals(2);
        txIntpRefDec->setMinimum(-999999.000000000000000);
        txIntpRefDec->setMaximum(999999.989999999990687);
        txIntpRefDec->setValue(0.000000000000000);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, txIntpRefDec);

        lbHomeStatus_108 = new QLabel(groupBox_25);
        lbHomeStatus_108->setObjectName(QString::fromUtf8("lbHomeStatus_108"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, lbHomeStatus_108);

        txIntpRefJerk = new QDoubleSpinBox(groupBox_25);
        txIntpRefJerk->setObjectName(QString::fromUtf8("txIntpRefJerk"));
        txIntpRefJerk->setReadOnly(false);
        txIntpRefJerk->setDecimals(2);
        txIntpRefJerk->setMinimum(-999999.000000000000000);
        txIntpRefJerk->setMaximum(999999.989999999990687);
        txIntpRefJerk->setValue(0.000000000000000);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, txIntpRefJerk);


        horizontalLayout_3->addWidget(groupBox_25);

        groupBox_23 = new QGroupBox(groupBox_22);
        groupBox_23->setObjectName(QString::fromUtf8("groupBox_23"));
        formLayout = new QFormLayout(groupBox_23);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        lbHomeStatus_81 = new QLabel(groupBox_23);
        lbHomeStatus_81->setObjectName(QString::fromUtf8("lbHomeStatus_81"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lbHomeStatus_81);

        txIntpRefPosX = new QDoubleSpinBox(groupBox_23);
        txIntpRefPosX->setObjectName(QString::fromUtf8("txIntpRefPosX"));
        txIntpRefPosX->setReadOnly(false);
        txIntpRefPosX->setDecimals(2);
        txIntpRefPosX->setMinimum(-999999.000000000000000);
        txIntpRefPosX->setMaximum(999999.989999999990687);
        txIntpRefPosX->setValue(0.000000000000000);

        formLayout->setWidget(0, QFormLayout::FieldRole, txIntpRefPosX);

        lbHomeStatus_106 = new QLabel(groupBox_23);
        lbHomeStatus_106->setObjectName(QString::fromUtf8("lbHomeStatus_106"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lbHomeStatus_106);

        txIntpRefPosY = new QDoubleSpinBox(groupBox_23);
        txIntpRefPosY->setObjectName(QString::fromUtf8("txIntpRefPosY"));
        txIntpRefPosY->setReadOnly(false);
        txIntpRefPosY->setDecimals(2);
        txIntpRefPosY->setMinimum(-999999.000000000000000);
        txIntpRefPosY->setMaximum(999999.989999999990687);
        txIntpRefPosY->setValue(0.000000000000000);

        formLayout->setWidget(1, QFormLayout::FieldRole, txIntpRefPosY);

        lbHomeStatus_101 = new QLabel(groupBox_23);
        lbHomeStatus_101->setObjectName(QString::fromUtf8("lbHomeStatus_101"));

        formLayout->setWidget(2, QFormLayout::LabelRole, lbHomeStatus_101);

        txIntpRefPosZ = new QDoubleSpinBox(groupBox_23);
        txIntpRefPosZ->setObjectName(QString::fromUtf8("txIntpRefPosZ"));
        txIntpRefPosZ->setReadOnly(false);
        txIntpRefPosZ->setDecimals(2);
        txIntpRefPosZ->setMinimum(-999999.000000000000000);
        txIntpRefPosZ->setMaximum(999999.989999999990687);
        txIntpRefPosZ->setValue(0.000000000000000);

        formLayout->setWidget(2, QFormLayout::FieldRole, txIntpRefPosZ);

        lbHomeStatus_99 = new QLabel(groupBox_23);
        lbHomeStatus_99->setObjectName(QString::fromUtf8("lbHomeStatus_99"));

        formLayout->setWidget(3, QFormLayout::LabelRole, lbHomeStatus_99);

        txIntpRefPosC = new QDoubleSpinBox(groupBox_23);
        txIntpRefPosC->setObjectName(QString::fromUtf8("txIntpRefPosC"));
        txIntpRefPosC->setReadOnly(false);
        txIntpRefPosC->setDecimals(2);
        txIntpRefPosC->setMinimum(-999999.000000000000000);
        txIntpRefPosC->setMaximum(999999.989999999990687);
        txIntpRefPosC->setValue(0.000000000000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, txIntpRefPosC);


        horizontalLayout_3->addWidget(groupBox_23);

        groupBox_26 = new QGroupBox(groupBox_22);
        groupBox_26->setObjectName(QString::fromUtf8("groupBox_26"));
        gridLayout_11 = new QGridLayout(groupBox_26);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        lbHomeStatus_110 = new QLabel(groupBox_26);
        lbHomeStatus_110->setObjectName(QString::fromUtf8("lbHomeStatus_110"));

        gridLayout_11->addWidget(lbHomeStatus_110, 1, 0, 1, 1);

        btIntpPowerOn = new QPushButton(groupBox_26);
        btIntpPowerOn->setObjectName(QString::fromUtf8("btIntpPowerOn"));
        btIntpPowerOn->setCheckable(true);

        gridLayout_11->addWidget(btIntpPowerOn, 3, 0, 1, 2);

        txIntpRefOffsetX = new QDoubleSpinBox(groupBox_26);
        txIntpRefOffsetX->setObjectName(QString::fromUtf8("txIntpRefOffsetX"));
        txIntpRefOffsetX->setReadOnly(false);
        txIntpRefOffsetX->setDecimals(2);
        txIntpRefOffsetX->setMinimum(-999999.000000000000000);
        txIntpRefOffsetX->setMaximum(999999.989999999990687);
        txIntpRefOffsetX->setValue(0.000000000000000);

        gridLayout_11->addWidget(txIntpRefOffsetX, 0, 1, 1, 1);

        lbHomeStatus_111 = new QLabel(groupBox_26);
        lbHomeStatus_111->setObjectName(QString::fromUtf8("lbHomeStatus_111"));

        gridLayout_11->addWidget(lbHomeStatus_111, 0, 0, 1, 1);

        txIntpRefOffsetY = new QDoubleSpinBox(groupBox_26);
        txIntpRefOffsetY->setObjectName(QString::fromUtf8("txIntpRefOffsetY"));
        txIntpRefOffsetY->setReadOnly(false);
        txIntpRefOffsetY->setDecimals(2);
        txIntpRefOffsetY->setMinimum(-999999.000000000000000);
        txIntpRefOffsetY->setMaximum(999999.989999999990687);
        txIntpRefOffsetY->setValue(0.000000000000000);

        gridLayout_11->addWidget(txIntpRefOffsetY, 1, 1, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer_6, 2, 1, 1, 1);


        horizontalLayout_3->addWidget(groupBox_26);

        groupBox_30 = new QGroupBox(groupBox_22);
        groupBox_30->setObjectName(QString::fromUtf8("groupBox_30"));
        verticalLayout_6 = new QVBoxLayout(groupBox_30);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        btIntpMvAbs = new QPushButton(groupBox_30);
        btIntpMvAbs->setObjectName(QString::fromUtf8("btIntpMvAbs"));

        verticalLayout_6->addWidget(btIntpMvAbs);

        btIntpMvRel = new QPushButton(groupBox_30);
        btIntpMvRel->setObjectName(QString::fromUtf8("btIntpMvRel"));

        verticalLayout_6->addWidget(btIntpMvRel);

        btIntpMvHalt = new QPushButton(groupBox_30);
        btIntpMvHalt->setObjectName(QString::fromUtf8("btIntpMvHalt"));

        verticalLayout_6->addWidget(btIntpMvHalt);

        btIntpMvReset = new QPushButton(groupBox_30);
        btIntpMvReset->setObjectName(QString::fromUtf8("btIntpMvReset"));

        verticalLayout_6->addWidget(btIntpMvReset);


        horizontalLayout_3->addWidget(groupBox_30);

        groupBox_29 = new QGroupBox(groupBox_22);
        groupBox_29->setObjectName(QString::fromUtf8("groupBox_29"));
        verticalLayout_7 = new QVBoxLayout(groupBox_29);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        ckIntpPowerOn = new QCheckBox(groupBox_29);
        ckIntpPowerOn->setObjectName(QString::fromUtf8("ckIntpPowerOn"));

        verticalLayout_7->addWidget(ckIntpPowerOn);

        ckIntpReady = new QCheckBox(groupBox_29);
        ckIntpReady->setObjectName(QString::fromUtf8("ckIntpReady"));
        ckIntpReady->setEnabled(false);

        verticalLayout_7->addWidget(ckIntpReady);

        ckIntpDone = new QCheckBox(groupBox_29);
        ckIntpDone->setObjectName(QString::fromUtf8("ckIntpDone"));
        ckIntpDone->setEnabled(false);

        verticalLayout_7->addWidget(ckIntpDone);

        ckIntpBusy = new QCheckBox(groupBox_29);
        ckIntpBusy->setObjectName(QString::fromUtf8("ckIntpBusy"));
        ckIntpBusy->setEnabled(false);

        verticalLayout_7->addWidget(ckIntpBusy);

        ckIntpError = new QCheckBox(groupBox_29);
        ckIntpError->setObjectName(QString::fromUtf8("ckIntpError"));
        ckIntpError->setEnabled(false);

        verticalLayout_7->addWidget(ckIntpError);


        horizontalLayout_3->addWidget(groupBox_29);


        gridLayout_13->addWidget(groupBox_22, 6, 0, 2, 2);

        groupBox_20 = new QGroupBox(tab_1);
        groupBox_20->setObjectName(QString::fromUtf8("groupBox_20"));
        gridLayout_8 = new QGridLayout(groupBox_20);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        ckHomeRetrunZero = new QCheckBox(groupBox_20);
        ckHomeRetrunZero->setObjectName(QString::fromUtf8("ckHomeRetrunZero"));

        gridLayout_8->addWidget(ckHomeRetrunZero, 6, 2, 1, 1);

        lbHomeStatus_60 = new QLabel(groupBox_20);
        lbHomeStatus_60->setObjectName(QString::fromUtf8("lbHomeStatus_60"));

        gridLayout_8->addWidget(lbHomeStatus_60, 0, 0, 1, 1);

        ckSmcHomeIO = new QCheckBox(groupBox_20);
        ckSmcHomeIO->setObjectName(QString::fromUtf8("ckSmcHomeIO"));

        gridLayout_8->addWidget(ckSmcHomeIO, 7, 0, 1, 1);

        lbHomeStatus_58 = new QLabel(groupBox_20);
        lbHomeStatus_58->setObjectName(QString::fromUtf8("lbHomeStatus_58"));

        gridLayout_8->addWidget(lbHomeStatus_58, 4, 0, 1, 1);

        lbHomeStatus_56 = new QLabel(groupBox_20);
        lbHomeStatus_56->setObjectName(QString::fromUtf8("lbHomeStatus_56"));

        gridLayout_8->addWidget(lbHomeStatus_56, 2, 0, 1, 1);

        lbHomeStatus_53 = new QLabel(groupBox_20);
        lbHomeStatus_53->setObjectName(QString::fromUtf8("lbHomeStatus_53"));

        gridLayout_8->addWidget(lbHomeStatus_53, 1, 0, 1, 1);

        txAxisHomeIndexModule = new QSpinBox(groupBox_20);
        txAxisHomeIndexModule->setObjectName(QString::fromUtf8("txAxisHomeIndexModule"));
        txAxisHomeIndexModule->setMinimum(-99);

        gridLayout_8->addWidget(txAxisHomeIndexModule, 4, 1, 1, 1);

        lbHomeStatus_57 = new QLabel(groupBox_20);
        lbHomeStatus_57->setObjectName(QString::fromUtf8("lbHomeStatus_57"));

        gridLayout_8->addWidget(lbHomeStatus_57, 3, 0, 1, 1);

        txAxisHomeSwitchModule = new QSpinBox(groupBox_20);
        txAxisHomeSwitchModule->setObjectName(QString::fromUtf8("txAxisHomeSwitchModule"));
        txAxisHomeSwitchModule->setMinimum(-99);

        gridLayout_8->addWidget(txAxisHomeSwitchModule, 3, 1, 1, 1);

        ckSmcHomeIndex = new QCheckBox(groupBox_20);
        ckSmcHomeIndex->setObjectName(QString::fromUtf8("ckSmcHomeIndex"));

        gridLayout_8->addWidget(ckSmcHomeIndex, 7, 2, 1, 1);

        ckIgnoreHw = new QCheckBox(groupBox_20);
        ckIgnoreHw->setObjectName(QString::fromUtf8("ckIgnoreHw"));

        gridLayout_8->addWidget(ckIgnoreHw, 6, 0, 1, 1);

        btPlcHome = new QPushButton(groupBox_20);
        btPlcHome->setObjectName(QString::fromUtf8("btPlcHome"));
        btPlcHome->setCheckable(false);

        gridLayout_8->addWidget(btPlcHome, 7, 3, 1, 1);

        txAxisHomeIndexIO = new QSpinBox(groupBox_20);
        txAxisHomeIndexIO->setObjectName(QString::fromUtf8("txAxisHomeIndexIO"));
        txAxisHomeIndexIO->setMinimum(-99);

        gridLayout_8->addWidget(txAxisHomeIndexIO, 4, 2, 1, 2);

        txAxisHomeSwitchIO = new QSpinBox(groupBox_20);
        txAxisHomeSwitchIO->setObjectName(QString::fromUtf8("txAxisHomeSwitchIO"));
        txAxisHomeSwitchIO->setMinimum(-99);

        gridLayout_8->addWidget(txAxisHomeSwitchIO, 3, 2, 1, 2);

        txAxisHomePls = new QDoubleSpinBox(groupBox_20);
        txAxisHomePls->setObjectName(QString::fromUtf8("txAxisHomePls"));
        txAxisHomePls->setReadOnly(false);
        txAxisHomePls->setDecimals(2);
        txAxisHomePls->setMinimum(-999999.000000000000000);
        txAxisHomePls->setMaximum(999999.989999999990687);
        txAxisHomePls->setValue(0.000000000000000);

        gridLayout_8->addWidget(txAxisHomePls, 2, 1, 1, 3);

        txAxisHomeSwitchDelay = new QDoubleSpinBox(groupBox_20);
        txAxisHomeSwitchDelay->setObjectName(QString::fromUtf8("txAxisHomeSwitchDelay"));
        txAxisHomeSwitchDelay->setReadOnly(false);
        txAxisHomeSwitchDelay->setDecimals(2);
        txAxisHomeSwitchDelay->setMinimum(-999999.000000000000000);
        txAxisHomeSwitchDelay->setMaximum(999999.989999999990687);
        txAxisHomeSwitchDelay->setValue(0.000000000000000);

        gridLayout_8->addWidget(txAxisHomeSwitchDelay, 1, 1, 1, 3);

        cbAxisSmcHomeMode = new QComboBox(groupBox_20);
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->addItem(QString());
        cbAxisSmcHomeMode->setObjectName(QString::fromUtf8("cbAxisSmcHomeMode"));

        gridLayout_8->addWidget(cbAxisSmcHomeMode, 0, 1, 1, 3);


        gridLayout_13->addWidget(groupBox_20, 5, 0, 1, 1);

        groupBox_15 = new QGroupBox(tab_1);
        groupBox_15->setObjectName(QString::fromUtf8("groupBox_15"));
        gridLayout_10 = new QGridLayout(groupBox_15);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        lbHomeStatus_41 = new QLabel(groupBox_15);
        lbHomeStatus_41->setObjectName(QString::fromUtf8("lbHomeStatus_41"));

        gridLayout_10->addWidget(lbHomeStatus_41, 0, 0, 1, 1);

        txAxisRefEStopDec = new QDoubleSpinBox(groupBox_15);
        txAxisRefEStopDec->setObjectName(QString::fromUtf8("txAxisRefEStopDec"));
        txAxisRefEStopDec->setReadOnly(false);
        txAxisRefEStopDec->setDecimals(2);
        txAxisRefEStopDec->setMinimum(-999999.000000000000000);
        txAxisRefEStopDec->setMaximum(999999.989999999990687);
        txAxisRefEStopDec->setValue(0.000000000000000);

        gridLayout_10->addWidget(txAxisRefEStopDec, 0, 1, 1, 2);

        lbHomeStatus_42 = new QLabel(groupBox_15);
        lbHomeStatus_42->setObjectName(QString::fromUtf8("lbHomeStatus_42"));

        gridLayout_10->addWidget(lbHomeStatus_42, 1, 0, 1, 1);

        txAxisRefEStopJerk = new QDoubleSpinBox(groupBox_15);
        txAxisRefEStopJerk->setObjectName(QString::fromUtf8("txAxisRefEStopJerk"));
        txAxisRefEStopJerk->setReadOnly(false);
        txAxisRefEStopJerk->setDecimals(2);
        txAxisRefEStopJerk->setMinimum(-999999.000000000000000);
        txAxisRefEStopJerk->setMaximum(999999.989999999990687);
        txAxisRefEStopJerk->setValue(0.000000000000000);

        gridLayout_10->addWidget(txAxisRefEStopJerk, 1, 1, 1, 2);

        lbHomeStatus_43 = new QLabel(groupBox_15);
        lbHomeStatus_43->setObjectName(QString::fromUtf8("lbHomeStatus_43"));

        gridLayout_10->addWidget(lbHomeStatus_43, 2, 0, 1, 1);

        txAxisRefEStopModule = new QSpinBox(groupBox_15);
        txAxisRefEStopModule->setObjectName(QString::fromUtf8("txAxisRefEStopModule"));
        txAxisRefEStopModule->setMinimum(-99);

        gridLayout_10->addWidget(txAxisRefEStopModule, 2, 1, 1, 1);

        txAxisRefEStopIO = new QSpinBox(groupBox_15);
        txAxisRefEStopIO->setObjectName(QString::fromUtf8("txAxisRefEStopIO"));
        txAxisRefEStopIO->setMinimum(-99);

        gridLayout_10->addWidget(txAxisRefEStopIO, 2, 2, 1, 1);

        btAxisEStop = new QPushButton(groupBox_15);
        btAxisEStop->setObjectName(QString::fromUtf8("btAxisEStop"));
        btAxisEStop->setCheckable(true);

        gridLayout_10->addWidget(btAxisEStop, 2, 3, 1, 1);


        gridLayout_13->addWidget(groupBox_15, 1, 0, 1, 1);

        groupBox_14 = new QGroupBox(tab_1);
        groupBox_14->setObjectName(QString::fromUtf8("groupBox_14"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_14);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lbHomeStatus_24 = new QLabel(groupBox_14);
        lbHomeStatus_24->setObjectName(QString::fromUtf8("lbHomeStatus_24"));

        horizontalLayout_2->addWidget(lbHomeStatus_24);

        cbAxisSel = new QComboBox(groupBox_14);
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->addItem(QString());
        cbAxisSel->setObjectName(QString::fromUtf8("cbAxisSel"));

        horizontalLayout_2->addWidget(cbAxisSel);

        lbHomeStatus_35 = new QLabel(groupBox_14);
        lbHomeStatus_35->setObjectName(QString::fromUtf8("lbHomeStatus_35"));

        horizontalLayout_2->addWidget(lbHomeStatus_35);

        spAxisActPos = new QDoubleSpinBox(groupBox_14);
        spAxisActPos->setObjectName(QString::fromUtf8("spAxisActPos"));
        spAxisActPos->setReadOnly(false);
        spAxisActPos->setDecimals(3);
        spAxisActPos->setMinimum(-999999.000000000000000);
        spAxisActPos->setMaximum(999999.989999999990687);
        spAxisActPos->setValue(0.000000000000000);

        horizontalLayout_2->addWidget(spAxisActPos);

        lbHomeStatus_34 = new QLabel(groupBox_14);
        lbHomeStatus_34->setObjectName(QString::fromUtf8("lbHomeStatus_34"));

        horizontalLayout_2->addWidget(lbHomeStatus_34);

        spAxisActVel = new QDoubleSpinBox(groupBox_14);
        spAxisActVel->setObjectName(QString::fromUtf8("spAxisActVel"));
        spAxisActVel->setReadOnly(false);
        spAxisActVel->setDecimals(3);
        spAxisActVel->setMinimum(-999999.000000000000000);
        spAxisActVel->setMaximum(999999.989999999990687);
        spAxisActVel->setValue(0.000000000000000);

        horizontalLayout_2->addWidget(spAxisActVel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        btAxisPowerOn = new QPushButton(groupBox_14);
        btAxisPowerOn->setObjectName(QString::fromUtf8("btAxisPowerOn"));
        btAxisPowerOn->setCheckable(true);

        horizontalLayout_2->addWidget(btAxisPowerOn);


        gridLayout_13->addWidget(groupBox_14, 0, 0, 1, 3);

        groupBox_18 = new QGroupBox(tab_1);
        groupBox_18->setObjectName(QString::fromUtf8("groupBox_18"));
        gridLayout_17 = new QGridLayout(groupBox_18);
        gridLayout_17->setSpacing(6);
        gridLayout_17->setContentsMargins(11, 11, 11, 11);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        lbHomeStatus_59 = new QLabel(groupBox_18);
        lbHomeStatus_59->setObjectName(QString::fromUtf8("lbHomeStatus_59"));

        gridLayout_17->addWidget(lbHomeStatus_59, 1, 0, 1, 1);

        label_12 = new QLabel(groupBox_18);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy);

        gridLayout_17->addWidget(label_12, 4, 0, 1, 1);

        btSdoWrite = new QPushButton(groupBox_18);
        btSdoWrite->setObjectName(QString::fromUtf8("btSdoWrite"));

        gridLayout_17->addWidget(btSdoWrite, 4, 2, 1, 1);

        label_11 = new QLabel(groupBox_18);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy);

        gridLayout_17->addWidget(label_11, 3, 0, 1, 1);

        txSdoSubIndex = new QSpinBox(groupBox_18);
        txSdoSubIndex->setObjectName(QString::fromUtf8("txSdoSubIndex"));
        txSdoSubIndex->setMinimum(0);
        txSdoSubIndex->setMaximum(32767);
        txSdoSubIndex->setDisplayIntegerBase(16);

        gridLayout_17->addWidget(txSdoSubIndex, 1, 1, 1, 1);

        txSdoIndex = new QSpinBox(groupBox_18);
        txSdoIndex->setObjectName(QString::fromUtf8("txSdoIndex"));
        txSdoIndex->setMinimum(0);
        txSdoIndex->setMaximum(32767);
        txSdoIndex->setDisplayIntegerBase(16);

        gridLayout_17->addWidget(txSdoIndex, 0, 1, 1, 1);

        lbHomeStatus_55 = new QLabel(groupBox_18);
        lbHomeStatus_55->setObjectName(QString::fromUtf8("lbHomeStatus_55"));

        gridLayout_17->addWidget(lbHomeStatus_55, 0, 0, 1, 1);

        btSdoRead = new QPushButton(groupBox_18);
        btSdoRead->setObjectName(QString::fromUtf8("btSdoRead"));

        gridLayout_17->addWidget(btSdoRead, 3, 2, 1, 1);

        label_15 = new QLabel(groupBox_18);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_17->addWidget(label_15, 2, 0, 1, 1);

        txSdoDataLen = new QSpinBox(groupBox_18);
        txSdoDataLen->setObjectName(QString::fromUtf8("txSdoDataLen"));
        txSdoDataLen->setMinimum(0);
        txSdoDataLen->setMaximum(32767);
        txSdoDataLen->setValue(1);
        txSdoDataLen->setDisplayIntegerBase(16);

        gridLayout_17->addWidget(txSdoDataLen, 2, 1, 1, 1);

        txSdoReadValue = new QDoubleSpinBox(groupBox_18);
        txSdoReadValue->setObjectName(QString::fromUtf8("txSdoReadValue"));
        txSdoReadValue->setReadOnly(false);
        txSdoReadValue->setDecimals(2);
        txSdoReadValue->setMinimum(-999999.000000000000000);
        txSdoReadValue->setMaximum(999999.989999999990687);
        txSdoReadValue->setValue(0.000000000000000);

        gridLayout_17->addWidget(txSdoReadValue, 3, 1, 1, 1);

        txSdoWriteValue = new QDoubleSpinBox(groupBox_18);
        txSdoWriteValue->setObjectName(QString::fromUtf8("txSdoWriteValue"));
        txSdoWriteValue->setReadOnly(false);
        txSdoWriteValue->setDecimals(2);
        txSdoWriteValue->setMinimum(-999999.000000000000000);
        txSdoWriteValue->setMaximum(999999.989999999990687);
        txSdoWriteValue->setValue(0.000000000000000);

        gridLayout_17->addWidget(txSdoWriteValue, 4, 1, 1, 1);


        gridLayout_13->addWidget(groupBox_18, 5, 1, 1, 1);

        groupBox_12 = new QGroupBox(tab_1);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        gridLayout_14 = new QGridLayout(groupBox_12);
        gridLayout_14->setSpacing(6);
        gridLayout_14->setContentsMargins(11, 11, 11, 11);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        lbHomeStatus_45 = new QLabel(groupBox_12);
        lbHomeStatus_45->setObjectName(QString::fromUtf8("lbHomeStatus_45"));

        gridLayout_14->addWidget(lbHomeStatus_45, 0, 0, 1, 1);

        txAxisHomePos = new QDoubleSpinBox(groupBox_12);
        txAxisHomePos->setObjectName(QString::fromUtf8("txAxisHomePos"));
        txAxisHomePos->setReadOnly(false);
        txAxisHomePos->setDecimals(2);
        txAxisHomePos->setMinimum(-999999.000000000000000);
        txAxisHomePos->setMaximum(999999.989999999990687);
        txAxisHomePos->setValue(0.000000000000000);

        gridLayout_14->addWidget(txAxisHomePos, 0, 1, 1, 1);

        lbHomeStatus_47 = new QLabel(groupBox_12);
        lbHomeStatus_47->setObjectName(QString::fromUtf8("lbHomeStatus_47"));

        gridLayout_14->addWidget(lbHomeStatus_47, 1, 0, 1, 1);

        txAxisHomeHVel = new QDoubleSpinBox(groupBox_12);
        txAxisHomeHVel->setObjectName(QString::fromUtf8("txAxisHomeHVel"));
        txAxisHomeHVel->setReadOnly(false);
        txAxisHomeHVel->setDecimals(2);
        txAxisHomeHVel->setMinimum(-999999.000000000000000);
        txAxisHomeHVel->setMaximum(999999.989999999990687);
        txAxisHomeHVel->setValue(0.000000000000000);

        gridLayout_14->addWidget(txAxisHomeHVel, 1, 1, 1, 1);

        lbHomeStatus_51 = new QLabel(groupBox_12);
        lbHomeStatus_51->setObjectName(QString::fromUtf8("lbHomeStatus_51"));

        gridLayout_14->addWidget(lbHomeStatus_51, 2, 0, 1, 1);

        txAxisHomeLVel = new QDoubleSpinBox(groupBox_12);
        txAxisHomeLVel->setObjectName(QString::fromUtf8("txAxisHomeLVel"));
        txAxisHomeLVel->setReadOnly(false);
        txAxisHomeLVel->setDecimals(2);
        txAxisHomeLVel->setMinimum(-999999.000000000000000);
        txAxisHomeLVel->setMaximum(999999.989999999990687);
        txAxisHomeLVel->setValue(0.000000000000000);

        gridLayout_14->addWidget(txAxisHomeLVel, 2, 1, 1, 1);

        lbHomeStatus_48 = new QLabel(groupBox_12);
        lbHomeStatus_48->setObjectName(QString::fromUtf8("lbHomeStatus_48"));

        gridLayout_14->addWidget(lbHomeStatus_48, 3, 0, 1, 1);

        txAxisHomeAcc = new QDoubleSpinBox(groupBox_12);
        txAxisHomeAcc->setObjectName(QString::fromUtf8("txAxisHomeAcc"));
        txAxisHomeAcc->setReadOnly(false);
        txAxisHomeAcc->setDecimals(2);
        txAxisHomeAcc->setMinimum(-999999.000000000000000);
        txAxisHomeAcc->setMaximum(999999.989999999990687);
        txAxisHomeAcc->setValue(0.000000000000000);

        gridLayout_14->addWidget(txAxisHomeAcc, 3, 1, 1, 1);

        lbHomeStatus_46 = new QLabel(groupBox_12);
        lbHomeStatus_46->setObjectName(QString::fromUtf8("lbHomeStatus_46"));

        gridLayout_14->addWidget(lbHomeStatus_46, 4, 0, 1, 1);

        txAxisHomeDec = new QDoubleSpinBox(groupBox_12);
        txAxisHomeDec->setObjectName(QString::fromUtf8("txAxisHomeDec"));
        txAxisHomeDec->setReadOnly(false);
        txAxisHomeDec->setDecimals(2);
        txAxisHomeDec->setMinimum(-999999.000000000000000);
        txAxisHomeDec->setMaximum(999999.989999999990687);
        txAxisHomeDec->setValue(0.000000000000000);

        gridLayout_14->addWidget(txAxisHomeDec, 4, 1, 1, 1);

        lbHomeStatus_49 = new QLabel(groupBox_12);
        lbHomeStatus_49->setObjectName(QString::fromUtf8("lbHomeStatus_49"));

        gridLayout_14->addWidget(lbHomeStatus_49, 5, 0, 1, 1);

        txAxisHomeJerk = new QDoubleSpinBox(groupBox_12);
        txAxisHomeJerk->setObjectName(QString::fromUtf8("txAxisHomeJerk"));
        txAxisHomeJerk->setReadOnly(false);
        txAxisHomeJerk->setDecimals(2);
        txAxisHomeJerk->setMinimum(-999999.000000000000000);
        txAxisHomeJerk->setMaximum(999999.989999999990687);
        txAxisHomeJerk->setValue(0.000000000000000);

        gridLayout_14->addWidget(txAxisHomeJerk, 5, 1, 1, 1);

        btAxisHomeParam = new QPushButton(groupBox_12);
        btAxisHomeParam->setObjectName(QString::fromUtf8("btAxisHomeParam"));

        gridLayout_14->addWidget(btAxisHomeParam, 5, 2, 1, 1);

        ckAxisHomeEnConv = new QCheckBox(groupBox_12);
        ckAxisHomeEnConv->setObjectName(QString::fromUtf8("ckAxisHomeEnConv"));

        gridLayout_14->addWidget(ckAxisHomeEnConv, 4, 2, 1, 1);


        gridLayout_13->addWidget(groupBox_12, 2, 0, 2, 1);

        groupBox_16 = new QGroupBox(tab_1);
        groupBox_16->setObjectName(QString::fromUtf8("groupBox_16"));
        gridLayout_9 = new QGridLayout(groupBox_16);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        lbHomeStatus_29 = new QLabel(groupBox_16);
        lbHomeStatus_29->setObjectName(QString::fromUtf8("lbHomeStatus_29"));

        gridLayout_9->addWidget(lbHomeStatus_29, 4, 0, 1, 1);

        txAxisJogVel = new QDoubleSpinBox(groupBox_16);
        txAxisJogVel->setObjectName(QString::fromUtf8("txAxisJogVel"));
        txAxisJogVel->setReadOnly(false);
        txAxisJogVel->setDecimals(2);
        txAxisJogVel->setMinimum(-999999.000000000000000);
        txAxisJogVel->setMaximum(999999.989999999990687);
        txAxisJogVel->setValue(0.000000000000000);

        gridLayout_9->addWidget(txAxisJogVel, 1, 1, 1, 1);

        txAxisJogDec = new QDoubleSpinBox(groupBox_16);
        txAxisJogDec->setObjectName(QString::fromUtf8("txAxisJogDec"));
        txAxisJogDec->setReadOnly(false);
        txAxisJogDec->setDecimals(2);
        txAxisJogDec->setMinimum(-999999.000000000000000);
        txAxisJogDec->setMaximum(999999.989999999990687);
        txAxisJogDec->setValue(0.000000000000000);

        gridLayout_9->addWidget(txAxisJogDec, 3, 1, 1, 1);

        lbHomeStatus_38 = new QLabel(groupBox_16);
        lbHomeStatus_38->setObjectName(QString::fromUtf8("lbHomeStatus_38"));

        gridLayout_9->addWidget(lbHomeStatus_38, 2, 0, 1, 1);

        lbHomeStatus_36 = new QLabel(groupBox_16);
        lbHomeStatus_36->setObjectName(QString::fromUtf8("lbHomeStatus_36"));

        gridLayout_9->addWidget(lbHomeStatus_36, 1, 0, 1, 1);

        txAxisJogAcc = new QDoubleSpinBox(groupBox_16);
        txAxisJogAcc->setObjectName(QString::fromUtf8("txAxisJogAcc"));
        txAxisJogAcc->setReadOnly(false);
        txAxisJogAcc->setDecimals(2);
        txAxisJogAcc->setMinimum(-999999.000000000000000);
        txAxisJogAcc->setMaximum(999999.989999999990687);
        txAxisJogAcc->setValue(0.000000000000000);

        gridLayout_9->addWidget(txAxisJogAcc, 2, 1, 1, 1);

        lbHomeStatus_37 = new QLabel(groupBox_16);
        lbHomeStatus_37->setObjectName(QString::fromUtf8("lbHomeStatus_37"));

        gridLayout_9->addWidget(lbHomeStatus_37, 3, 0, 1, 1);

        txAxisJogJerk = new QDoubleSpinBox(groupBox_16);
        txAxisJogJerk->setObjectName(QString::fromUtf8("txAxisJogJerk"));
        txAxisJogJerk->setReadOnly(false);
        txAxisJogJerk->setDecimals(2);
        txAxisJogJerk->setMinimum(-999999.000000000000000);
        txAxisJogJerk->setMaximum(999999.989999999990687);
        txAxisJogJerk->setValue(0.000000000000000);

        gridLayout_9->addWidget(txAxisJogJerk, 4, 1, 1, 1);

        lbHomeStatus_61 = new QLabel(groupBox_16);
        lbHomeStatus_61->setObjectName(QString::fromUtf8("lbHomeStatus_61"));

        gridLayout_9->addWidget(lbHomeStatus_61, 0, 0, 1, 1);

        txAxisInchDist = new QDoubleSpinBox(groupBox_16);
        txAxisInchDist->setObjectName(QString::fromUtf8("txAxisInchDist"));
        txAxisInchDist->setReadOnly(false);
        txAxisInchDist->setDecimals(2);
        txAxisInchDist->setMinimum(-999999.000000000000000);
        txAxisInchDist->setMaximum(999999.989999999990687);
        txAxisInchDist->setValue(0.000000000000000);

        gridLayout_9->addWidget(txAxisInchDist, 0, 1, 1, 1);

        btAxisJogFwd = new QPushButton(groupBox_16);
        btAxisJogFwd->setObjectName(QString::fromUtf8("btAxisJogFwd"));
        btAxisJogFwd->setCheckable(false);

        gridLayout_9->addWidget(btAxisJogFwd, 0, 2, 1, 1);

        btAxisJogBwd = new QPushButton(groupBox_16);
        btAxisJogBwd->setObjectName(QString::fromUtf8("btAxisJogBwd"));
        btAxisJogBwd->setCheckable(false);

        gridLayout_9->addWidget(btAxisJogBwd, 1, 2, 1, 1);

        cbAxisInch = new QCheckBox(groupBox_16);
        cbAxisInch->setObjectName(QString::fromUtf8("cbAxisInch"));

        gridLayout_9->addWidget(cbAxisInch, 4, 2, 1, 1);


        gridLayout_13->addWidget(groupBox_16, 3, 1, 2, 1);

        groupBox_9 = new QGroupBox(tab_1);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        gridLayout_7 = new QGridLayout(groupBox_9);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        lbHomeStatus_33 = new QLabel(groupBox_9);
        lbHomeStatus_33->setObjectName(QString::fromUtf8("lbHomeStatus_33"));

        gridLayout_7->addWidget(lbHomeStatus_33, 0, 0, 1, 1);

        txAxisRefPos = new QDoubleSpinBox(groupBox_9);
        txAxisRefPos->setObjectName(QString::fromUtf8("txAxisRefPos"));
        txAxisRefPos->setReadOnly(false);
        txAxisRefPos->setDecimals(2);
        txAxisRefPos->setMinimum(-999999.000000000000000);
        txAxisRefPos->setMaximum(999999.989999999990687);
        txAxisRefPos->setValue(0.000000000000000);

        gridLayout_7->addWidget(txAxisRefPos, 0, 1, 1, 1);

        btAxisMoveAbs = new QPushButton(groupBox_9);
        btAxisMoveAbs->setObjectName(QString::fromUtf8("btAxisMoveAbs"));

        gridLayout_7->addWidget(btAxisMoveAbs, 0, 2, 1, 1);

        lbHomeStatus_31 = new QLabel(groupBox_9);
        lbHomeStatus_31->setObjectName(QString::fromUtf8("lbHomeStatus_31"));

        gridLayout_7->addWidget(lbHomeStatus_31, 1, 0, 1, 1);

        txAxisRefVel = new QDoubleSpinBox(groupBox_9);
        txAxisRefVel->setObjectName(QString::fromUtf8("txAxisRefVel"));
        txAxisRefVel->setReadOnly(false);
        txAxisRefVel->setDecimals(2);
        txAxisRefVel->setMinimum(-999999.000000000000000);
        txAxisRefVel->setMaximum(999999.989999999990687);
        txAxisRefVel->setValue(0.000000000000000);

        gridLayout_7->addWidget(txAxisRefVel, 1, 1, 1, 1);

        btAxisMoveRel = new QPushButton(groupBox_9);
        btAxisMoveRel->setObjectName(QString::fromUtf8("btAxisMoveRel"));

        gridLayout_7->addWidget(btAxisMoveRel, 1, 2, 1, 1);

        lbHomeStatus_32 = new QLabel(groupBox_9);
        lbHomeStatus_32->setObjectName(QString::fromUtf8("lbHomeStatus_32"));

        gridLayout_7->addWidget(lbHomeStatus_32, 2, 0, 1, 1);

        txAxisRefAcc = new QDoubleSpinBox(groupBox_9);
        txAxisRefAcc->setObjectName(QString::fromUtf8("txAxisRefAcc"));
        txAxisRefAcc->setReadOnly(false);
        txAxisRefAcc->setDecimals(2);
        txAxisRefAcc->setMinimum(-999999.000000000000000);
        txAxisRefAcc->setMaximum(999999.989999999990687);
        txAxisRefAcc->setValue(0.000000000000000);

        gridLayout_7->addWidget(txAxisRefAcc, 2, 1, 1, 1);

        btAxisMoveVel = new QPushButton(groupBox_9);
        btAxisMoveVel->setObjectName(QString::fromUtf8("btAxisMoveVel"));

        gridLayout_7->addWidget(btAxisMoveVel, 2, 2, 1, 1);

        lbHomeStatus_30 = new QLabel(groupBox_9);
        lbHomeStatus_30->setObjectName(QString::fromUtf8("lbHomeStatus_30"));

        gridLayout_7->addWidget(lbHomeStatus_30, 3, 0, 1, 1);

        txAxisRefDec = new QDoubleSpinBox(groupBox_9);
        txAxisRefDec->setObjectName(QString::fromUtf8("txAxisRefDec"));
        txAxisRefDec->setReadOnly(false);
        txAxisRefDec->setDecimals(2);
        txAxisRefDec->setMinimum(-999999.000000000000000);
        txAxisRefDec->setMaximum(999999.989999999990687);
        txAxisRefDec->setValue(0.000000000000000);

        gridLayout_7->addWidget(txAxisRefDec, 3, 1, 1, 1);

        btAxisMoveHalt = new QPushButton(groupBox_9);
        btAxisMoveHalt->setObjectName(QString::fromUtf8("btAxisMoveHalt"));

        gridLayout_7->addWidget(btAxisMoveHalt, 3, 2, 1, 1);

        lbHomeStatus_26 = new QLabel(groupBox_9);
        lbHomeStatus_26->setObjectName(QString::fromUtf8("lbHomeStatus_26"));

        gridLayout_7->addWidget(lbHomeStatus_26, 4, 0, 1, 1);

        txAxisRefJerk = new QDoubleSpinBox(groupBox_9);
        txAxisRefJerk->setObjectName(QString::fromUtf8("txAxisRefJerk"));
        txAxisRefJerk->setReadOnly(false);
        txAxisRefJerk->setDecimals(2);
        txAxisRefJerk->setMinimum(-999999.000000000000000);
        txAxisRefJerk->setMaximum(999999.989999999990687);
        txAxisRefJerk->setValue(0.000000000000000);

        gridLayout_7->addWidget(txAxisRefJerk, 4, 1, 1, 1);

        lbHomeStatus_27 = new QLabel(groupBox_9);
        lbHomeStatus_27->setObjectName(QString::fromUtf8("lbHomeStatus_27"));

        gridLayout_7->addWidget(lbHomeStatus_27, 5, 0, 1, 1);

        cbAxisBufferMode = new QComboBox(groupBox_9);
        cbAxisBufferMode->addItem(QString());
        cbAxisBufferMode->addItem(QString());
        cbAxisBufferMode->addItem(QString());
        cbAxisBufferMode->addItem(QString());
        cbAxisBufferMode->addItem(QString());
        cbAxisBufferMode->addItem(QString());
        cbAxisBufferMode->setObjectName(QString::fromUtf8("cbAxisBufferMode"));

        gridLayout_7->addWidget(cbAxisBufferMode, 5, 1, 1, 1);

        btAxisSetPos = new QPushButton(groupBox_9);
        btAxisSetPos->setObjectName(QString::fromUtf8("btAxisSetPos"));

        gridLayout_7->addWidget(btAxisSetPos, 5, 2, 1, 1);

        btAxisReset = new QPushButton(groupBox_9);
        btAxisReset->setObjectName(QString::fromUtf8("btAxisReset"));

        gridLayout_7->addWidget(btAxisReset, 4, 2, 1, 1);


        gridLayout_13->addWidget(groupBox_9, 1, 1, 2, 1);

        groupBox_13 = new QGroupBox(tab_1);
        groupBox_13->setObjectName(QString::fromUtf8("groupBox_13"));
        verticalLayout_5 = new QVBoxLayout(groupBox_13);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        ckVirtual = new QCheckBox(groupBox_13);
        ckVirtual->setObjectName(QString::fromUtf8("ckVirtual"));

        verticalLayout_4->addWidget(ckVirtual);

        ckCommStatus = new QCheckBox(groupBox_13);
        ckCommStatus->setObjectName(QString::fromUtf8("ckCommStatus"));
        ckCommStatus->setEnabled(true);

        verticalLayout_4->addWidget(ckCommStatus);

        ckRefStatus = new QCheckBox(groupBox_13);
        ckRefStatus->setObjectName(QString::fromUtf8("ckRefStatus"));
        ckRefStatus->setEnabled(true);

        verticalLayout_4->addWidget(ckRefStatus);

        ckPowerStatus = new QCheckBox(groupBox_13);
        ckPowerStatus->setObjectName(QString::fromUtf8("ckPowerStatus"));
        ckPowerStatus->setEnabled(true);

        verticalLayout_4->addWidget(ckPowerStatus);

        ckStoppedStatus = new QCheckBox(groupBox_13);
        ckStoppedStatus->setObjectName(QString::fromUtf8("ckStoppedStatus"));
        ckStoppedStatus->setEnabled(true);

        verticalLayout_4->addWidget(ckStoppedStatus);

        ckHomingStatus = new QCheckBox(groupBox_13);
        ckHomingStatus->setObjectName(QString::fromUtf8("ckHomingStatus"));
        ckHomingStatus->setEnabled(true);

        verticalLayout_4->addWidget(ckHomingStatus);

        ckSyncStatus = new QCheckBox(groupBox_13);
        ckSyncStatus->setObjectName(QString::fromUtf8("ckSyncStatus"));
        ckSyncStatus->setEnabled(true);

        verticalLayout_4->addWidget(ckSyncStatus);

        ckDone = new QCheckBox(groupBox_13);
        ckDone->setObjectName(QString::fromUtf8("ckDone"));
        ckDone->setEnabled(true);

        verticalLayout_4->addWidget(ckDone);

        ckBusy = new QCheckBox(groupBox_13);
        ckBusy->setObjectName(QString::fromUtf8("ckBusy"));
        ckBusy->setEnabled(true);

        verticalLayout_4->addWidget(ckBusy);

        ckError = new QCheckBox(groupBox_13);
        ckError->setObjectName(QString::fromUtf8("ckError"));
        ckError->setEnabled(true);

        verticalLayout_4->addWidget(ckError);

        ckAlmStatus = new QCheckBox(groupBox_13);
        ckAlmStatus->setObjectName(QString::fromUtf8("ckAlmStatus"));
        ckAlmStatus->setEnabled(true);

        verticalLayout_4->addWidget(ckAlmStatus);

        line_4 = new QFrame(groupBox_13);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_4);

        ckDINegativeLimit = new QCheckBox(groupBox_13);
        ckDINegativeLimit->setObjectName(QString::fromUtf8("ckDINegativeLimit"));

        verticalLayout_4->addWidget(ckDINegativeLimit);

        ckDIPositiveLimit = new QCheckBox(groupBox_13);
        ckDIPositiveLimit->setObjectName(QString::fromUtf8("ckDIPositiveLimit"));

        verticalLayout_4->addWidget(ckDIPositiveLimit);

        ckDIHomeSwitch = new QCheckBox(groupBox_13);
        ckDIHomeSwitch->setObjectName(QString::fromUtf8("ckDIHomeSwitch"));

        verticalLayout_4->addWidget(ckDIHomeSwitch);

        ckDIQuickStop = new QCheckBox(groupBox_13);
        ckDIQuickStop->setObjectName(QString::fromUtf8("ckDIQuickStop"));

        verticalLayout_4->addWidget(ckDIQuickStop);

        ckDIVoltageOk = new QCheckBox(groupBox_13);
        ckDIVoltageOk->setObjectName(QString::fromUtf8("ckDIVoltageOk"));

        verticalLayout_4->addWidget(ckDIVoltageOk);

        ckDIBrakeReleased = new QCheckBox(groupBox_13);
        ckDIBrakeReleased->setObjectName(QString::fromUtf8("ckDIBrakeReleased"));

        verticalLayout_4->addWidget(ckDIBrakeReleased);

        ckDIWarning = new QCheckBox(groupBox_13);
        ckDIWarning->setObjectName(QString::fromUtf8("ckDIWarning"));

        verticalLayout_4->addWidget(ckDIWarning);

        ckDIReady = new QCheckBox(groupBox_13);
        ckDIReady->setObjectName(QString::fromUtf8("ckDIReady"));

        verticalLayout_4->addWidget(ckDIReady);

        ckDIFault = new QCheckBox(groupBox_13);
        ckDIFault->setObjectName(QString::fromUtf8("ckDIFault"));

        verticalLayout_4->addWidget(ckDIFault);

        line_2 = new QFrame(groupBox_13);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_2);


        verticalLayout_5->addLayout(verticalLayout_4);

        formLayout_3 = new QFormLayout();
        formLayout_3->setSpacing(6);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        ckModulo = new QCheckBox(groupBox_13);
        ckModulo->setObjectName(QString::fromUtf8("ckModulo"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, ckModulo);

        ckRotary = new QCheckBox(groupBox_13);
        ckRotary->setObjectName(QString::fromUtf8("ckRotary"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, ckRotary);

        lbHomeStatus_80 = new QLabel(groupBox_13);
        lbHomeStatus_80->setObjectName(QString::fromUtf8("lbHomeStatus_80"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, lbHomeStatus_80);

        spAxisModulo = new QDoubleSpinBox(groupBox_13);
        spAxisModulo->setObjectName(QString::fromUtf8("spAxisModulo"));
        spAxisModulo->setReadOnly(false);
        spAxisModulo->setDecimals(2);
        spAxisModulo->setMinimum(-999999.000000000000000);
        spAxisModulo->setMaximum(999999.989999999990687);
        spAxisModulo->setValue(0.000000000000000);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, spAxisModulo);

        lbHomeStatus_76 = new QLabel(groupBox_13);
        lbHomeStatus_76->setObjectName(QString::fromUtf8("lbHomeStatus_76"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, lbHomeStatus_76);

        spAxisMaxVel = new QDoubleSpinBox(groupBox_13);
        spAxisMaxVel->setObjectName(QString::fromUtf8("spAxisMaxVel"));
        spAxisMaxVel->setReadOnly(false);
        spAxisMaxVel->setDecimals(2);
        spAxisMaxVel->setMinimum(-999999.000000000000000);
        spAxisMaxVel->setMaximum(999999.989999999990687);
        spAxisMaxVel->setValue(0.000000000000000);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, spAxisMaxVel);

        lbHomeStatus_77 = new QLabel(groupBox_13);
        lbHomeStatus_77->setObjectName(QString::fromUtf8("lbHomeStatus_77"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, lbHomeStatus_77);

        spAxisMaxAcc = new QDoubleSpinBox(groupBox_13);
        spAxisMaxAcc->setObjectName(QString::fromUtf8("spAxisMaxAcc"));
        spAxisMaxAcc->setReadOnly(false);
        spAxisMaxAcc->setDecimals(2);
        spAxisMaxAcc->setMinimum(-999999.000000000000000);
        spAxisMaxAcc->setMaximum(999999.989999999990687);
        spAxisMaxAcc->setValue(0.000000000000000);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, spAxisMaxAcc);

        lbHomeStatus_78 = new QLabel(groupBox_13);
        lbHomeStatus_78->setObjectName(QString::fromUtf8("lbHomeStatus_78"));

        formLayout_3->setWidget(4, QFormLayout::LabelRole, lbHomeStatus_78);

        spAxisMaxDec = new QDoubleSpinBox(groupBox_13);
        spAxisMaxDec->setObjectName(QString::fromUtf8("spAxisMaxDec"));
        spAxisMaxDec->setReadOnly(false);
        spAxisMaxDec->setDecimals(2);
        spAxisMaxDec->setMinimum(-999999.000000000000000);
        spAxisMaxDec->setMaximum(999999.989999999990687);
        spAxisMaxDec->setValue(0.000000000000000);

        formLayout_3->setWidget(4, QFormLayout::FieldRole, spAxisMaxDec);

        lbHomeStatus_79 = new QLabel(groupBox_13);
        lbHomeStatus_79->setObjectName(QString::fromUtf8("lbHomeStatus_79"));

        formLayout_3->setWidget(5, QFormLayout::LabelRole, lbHomeStatus_79);

        spAxisMaxJerk = new QDoubleSpinBox(groupBox_13);
        spAxisMaxJerk->setObjectName(QString::fromUtf8("spAxisMaxJerk"));
        spAxisMaxJerk->setReadOnly(false);
        spAxisMaxJerk->setDecimals(2);
        spAxisMaxJerk->setMinimum(-999999.000000000000000);
        spAxisMaxJerk->setMaximum(999999.989999999990687);
        spAxisMaxJerk->setValue(0.000000000000000);

        formLayout_3->setWidget(5, QFormLayout::FieldRole, spAxisMaxJerk);


        verticalLayout_5->addLayout(formLayout_3);

        line_3 = new QFrame(groupBox_13);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(line_3);

        formLayout_4 = new QFormLayout();
        formLayout_4->setSpacing(6);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        lbHomeStatus_44 = new QLabel(groupBox_13);
        lbHomeStatus_44->setObjectName(QString::fromUtf8("lbHomeStatus_44"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, lbHomeStatus_44);

        spCtrlStatus = new QSpinBox(groupBox_13);
        spCtrlStatus->setObjectName(QString::fromUtf8("spCtrlStatus"));
        spCtrlStatus->setMaximum(99999999);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, spCtrlStatus);

        lbHomeStatus_39 = new QLabel(groupBox_13);
        lbHomeStatus_39->setObjectName(QString::fromUtf8("lbHomeStatus_39"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, lbHomeStatus_39);

        spErrorId = new QSpinBox(groupBox_13);
        spErrorId->setObjectName(QString::fromUtf8("spErrorId"));
        spErrorId->setMaximum(99999999);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, spErrorId);

        lbHomeStatus_40 = new QLabel(groupBox_13);
        lbHomeStatus_40->setObjectName(QString::fromUtf8("lbHomeStatus_40"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, lbHomeStatus_40);

        spAxisErrorId = new QSpinBox(groupBox_13);
        spAxisErrorId->setObjectName(QString::fromUtf8("spAxisErrorId"));
        spAxisErrorId->setMaximum(99999999);

        formLayout_4->setWidget(3, QFormLayout::FieldRole, spAxisErrorId);

        lbHomeStatus_25 = new QLabel(groupBox_13);
        lbHomeStatus_25->setObjectName(QString::fromUtf8("lbHomeStatus_25"));

        formLayout_4->setWidget(4, QFormLayout::LabelRole, lbHomeStatus_25);

        spAxisStatus = new QSpinBox(groupBox_13);
        spAxisStatus->setObjectName(QString::fromUtf8("spAxisStatus"));
        spAxisStatus->setMaximum(99999999);

        formLayout_4->setWidget(4, QFormLayout::FieldRole, spAxisStatus);

        lbHomeStatus_74 = new QLabel(groupBox_13);
        lbHomeStatus_74->setObjectName(QString::fromUtf8("lbHomeStatus_74"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, lbHomeStatus_74);

        spCtrlId = new QSpinBox(groupBox_13);
        spCtrlId->setObjectName(QString::fromUtf8("spCtrlId"));
        spCtrlId->setMaximum(99999999);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, spCtrlId);


        verticalLayout_5->addLayout(formLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);


        gridLayout_13->addWidget(groupBox_13, 1, 2, 7, 1);

        groupBox_19 = new QGroupBox(tab_1);
        groupBox_19->setObjectName(QString::fromUtf8("groupBox_19"));
        gridLayout_15 = new QGridLayout(groupBox_19);
        gridLayout_15->setSpacing(6);
        gridLayout_15->setContentsMargins(11, 11, 11, 11);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        lbHomeStatus_52 = new QLabel(groupBox_19);
        lbHomeStatus_52->setObjectName(QString::fromUtf8("lbHomeStatus_52"));

        gridLayout_15->addWidget(lbHomeStatus_52, 0, 0, 1, 1);

        txAxisDrvHomeOffset = new QDoubleSpinBox(groupBox_19);
        txAxisDrvHomeOffset->setObjectName(QString::fromUtf8("txAxisDrvHomeOffset"));
        txAxisDrvHomeOffset->setReadOnly(false);
        txAxisDrvHomeOffset->setDecimals(2);
        txAxisDrvHomeOffset->setMinimum(-999999.000000000000000);
        txAxisDrvHomeOffset->setMaximum(999999.989999999990687);
        txAxisDrvHomeOffset->setValue(0.000000000000000);

        gridLayout_15->addWidget(txAxisDrvHomeOffset, 0, 1, 1, 1);

        lbHomeStatus_54 = new QLabel(groupBox_19);
        lbHomeStatus_54->setObjectName(QString::fromUtf8("lbHomeStatus_54"));

        gridLayout_15->addWidget(lbHomeStatus_54, 1, 0, 1, 1);

        txAxisDrvHomeMode = new QSpinBox(groupBox_19);
        txAxisDrvHomeMode->setObjectName(QString::fromUtf8("txAxisDrvHomeMode"));

        gridLayout_15->addWidget(txAxisDrvHomeMode, 1, 1, 1, 1);

        ckAxisHomeEnSdo = new QCheckBox(groupBox_19);
        ckAxisHomeEnSdo->setObjectName(QString::fromUtf8("ckAxisHomeEnSdo"));

        gridLayout_15->addWidget(ckAxisHomeEnSdo, 0, 2, 1, 1);

        btDrvHome = new QPushButton(groupBox_19);
        btDrvHome->setObjectName(QString::fromUtf8("btDrvHome"));
        btDrvHome->setCheckable(false);

        gridLayout_15->addWidget(btDrvHome, 1, 2, 1, 1);


        gridLayout_13->addWidget(groupBox_19, 4, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_13->addItem(verticalSpacer, 8, 0, 1, 1);

        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_12 = new QGridLayout(tab_2);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_5 = new QHBoxLayout(groupBox_2);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        rbScaraLeft = new QRadioButton(groupBox_2);
        rbScaraLeft->setObjectName(QString::fromUtf8("rbScaraLeft"));

        horizontalLayout_5->addWidget(rbScaraLeft);

        rbScaraRight = new QRadioButton(groupBox_2);
        rbScaraRight->setObjectName(QString::fromUtf8("rbScaraRight"));

        horizontalLayout_5->addWidget(rbScaraRight);

        btScaraHandSet = new QPushButton(groupBox_2);
        btScaraHandSet->setObjectName(QString::fromUtf8("btScaraHandSet"));

        horizontalLayout_5->addWidget(btScaraHandSet);


        gridLayout_12->addWidget(groupBox_2, 2, 0, 1, 1);

        groupBox_5 = new QGroupBox(tab_2);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_5 = new QGridLayout(groupBox_5);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        cbRobotSel = new QComboBox(groupBox_5);
        cbRobotSel->addItem(QString());
        cbRobotSel->addItem(QString());
        cbRobotSel->addItem(QString());
        cbRobotSel->addItem(QString());
        cbRobotSel->addItem(QString());
        cbRobotSel->addItem(QString());
        cbRobotSel->addItem(QString());
        cbRobotSel->addItem(QString());
        cbRobotSel->addItem(QString());
        cbRobotSel->addItem(QString());
        cbRobotSel->setObjectName(QString::fromUtf8("cbRobotSel"));

        gridLayout_5->addWidget(cbRobotSel, 0, 0, 1, 1);

        lbHomeStatus_6 = new QLabel(groupBox_5);
        lbHomeStatus_6->setObjectName(QString::fromUtf8("lbHomeStatus_6"));

        gridLayout_5->addWidget(lbHomeStatus_6, 0, 1, 1, 1);

        lbHomeStatus_7 = new QLabel(groupBox_5);
        lbHomeStatus_7->setObjectName(QString::fromUtf8("lbHomeStatus_7"));

        gridLayout_5->addWidget(lbHomeStatus_7, 0, 2, 1, 1);

        lbHomeStatus_18 = new QLabel(groupBox_5);
        lbHomeStatus_18->setObjectName(QString::fromUtf8("lbHomeStatus_18"));

        gridLayout_5->addWidget(lbHomeStatus_18, 0, 3, 1, 1);

        lbHomeStatus_20 = new QLabel(groupBox_5);
        lbHomeStatus_20->setObjectName(QString::fromUtf8("lbHomeStatus_20"));

        gridLayout_5->addWidget(lbHomeStatus_20, 0, 4, 1, 1);

        lbHomeStatus_50 = new QLabel(groupBox_5);
        lbHomeStatus_50->setObjectName(QString::fromUtf8("lbHomeStatus_50"));

        gridLayout_5->addWidget(lbHomeStatus_50, 0, 5, 1, 1);

        label_19 = new QLabel(groupBox_5);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_5->addWidget(label_19, 0, 6, 1, 1);

        label_24 = new QLabel(groupBox_5);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_5->addWidget(label_24, 1, 0, 1, 1);

        txRobotAcsPos_A1 = new QDoubleSpinBox(groupBox_5);
        txRobotAcsPos_A1->setObjectName(QString::fromUtf8("txRobotAcsPos_A1"));
        txRobotAcsPos_A1->setEnabled(true);
        txRobotAcsPos_A1->setReadOnly(true);
        txRobotAcsPos_A1->setDecimals(3);
        txRobotAcsPos_A1->setMinimum(-999999.000000000000000);
        txRobotAcsPos_A1->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotAcsPos_A1, 1, 1, 1, 1);

        txRobotAcsPos_A2 = new QDoubleSpinBox(groupBox_5);
        txRobotAcsPos_A2->setObjectName(QString::fromUtf8("txRobotAcsPos_A2"));
        txRobotAcsPos_A2->setEnabled(true);
        txRobotAcsPos_A2->setReadOnly(true);
        txRobotAcsPos_A2->setDecimals(3);
        txRobotAcsPos_A2->setMinimum(-999999.000000000000000);
        txRobotAcsPos_A2->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotAcsPos_A2, 1, 2, 1, 1);

        txRobotAcsPos_A3 = new QDoubleSpinBox(groupBox_5);
        txRobotAcsPos_A3->setObjectName(QString::fromUtf8("txRobotAcsPos_A3"));
        txRobotAcsPos_A3->setEnabled(true);
        txRobotAcsPos_A3->setReadOnly(true);
        txRobotAcsPos_A3->setDecimals(3);
        txRobotAcsPos_A3->setMinimum(-999999.000000000000000);
        txRobotAcsPos_A3->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotAcsPos_A3, 1, 3, 1, 1);

        txRobotAcsPos_A4 = new QDoubleSpinBox(groupBox_5);
        txRobotAcsPos_A4->setObjectName(QString::fromUtf8("txRobotAcsPos_A4"));
        txRobotAcsPos_A4->setEnabled(true);
        txRobotAcsPos_A4->setReadOnly(true);
        txRobotAcsPos_A4->setDecimals(3);
        txRobotAcsPos_A4->setMinimum(-999999.000000000000000);
        txRobotAcsPos_A4->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotAcsPos_A4, 1, 4, 1, 1);

        txRobotAcsPos_A5 = new QDoubleSpinBox(groupBox_5);
        txRobotAcsPos_A5->setObjectName(QString::fromUtf8("txRobotAcsPos_A5"));
        txRobotAcsPos_A5->setEnabled(true);
        txRobotAcsPos_A5->setReadOnly(true);
        txRobotAcsPos_A5->setDecimals(3);
        txRobotAcsPos_A5->setMinimum(-999999.000000000000000);
        txRobotAcsPos_A5->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotAcsPos_A5, 1, 5, 1, 1);

        txRobotAcsPos_A6 = new QDoubleSpinBox(groupBox_5);
        txRobotAcsPos_A6->setObjectName(QString::fromUtf8("txRobotAcsPos_A6"));
        txRobotAcsPos_A6->setEnabled(true);
        txRobotAcsPos_A6->setReadOnly(true);
        txRobotAcsPos_A6->setDecimals(3);
        txRobotAcsPos_A6->setMinimum(-999999.000000000000000);
        txRobotAcsPos_A6->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotAcsPos_A6, 1, 6, 1, 1);

        label_23 = new QLabel(groupBox_5);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_5->addWidget(label_23, 2, 0, 1, 1);

        txRobotMcsPos_X = new QDoubleSpinBox(groupBox_5);
        txRobotMcsPos_X->setObjectName(QString::fromUtf8("txRobotMcsPos_X"));
        txRobotMcsPos_X->setEnabled(true);
        txRobotMcsPos_X->setReadOnly(true);
        txRobotMcsPos_X->setDecimals(3);
        txRobotMcsPos_X->setMinimum(-999999.000000000000000);
        txRobotMcsPos_X->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotMcsPos_X, 2, 1, 1, 1);

        txRobotMcsPos_Y = new QDoubleSpinBox(groupBox_5);
        txRobotMcsPos_Y->setObjectName(QString::fromUtf8("txRobotMcsPos_Y"));
        txRobotMcsPos_Y->setEnabled(true);
        txRobotMcsPos_Y->setReadOnly(true);
        txRobotMcsPos_Y->setDecimals(3);
        txRobotMcsPos_Y->setMinimum(-999999.000000000000000);
        txRobotMcsPos_Y->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotMcsPos_Y, 2, 2, 1, 1);

        txRobotMcsPos_Z = new QDoubleSpinBox(groupBox_5);
        txRobotMcsPos_Z->setObjectName(QString::fromUtf8("txRobotMcsPos_Z"));
        txRobotMcsPos_Z->setEnabled(true);
        txRobotMcsPos_Z->setReadOnly(true);
        txRobotMcsPos_Z->setDecimals(3);
        txRobotMcsPos_Z->setMinimum(-999999.000000000000000);
        txRobotMcsPos_Z->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotMcsPos_Z, 2, 3, 1, 1);

        txRobotMcsPos_A = new QDoubleSpinBox(groupBox_5);
        txRobotMcsPos_A->setObjectName(QString::fromUtf8("txRobotMcsPos_A"));
        txRobotMcsPos_A->setEnabled(true);
        txRobotMcsPos_A->setReadOnly(true);
        txRobotMcsPos_A->setDecimals(3);
        txRobotMcsPos_A->setMinimum(-999999.000000000000000);
        txRobotMcsPos_A->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotMcsPos_A, 2, 4, 1, 1);

        txRobotMcsPos_B = new QDoubleSpinBox(groupBox_5);
        txRobotMcsPos_B->setObjectName(QString::fromUtf8("txRobotMcsPos_B"));
        txRobotMcsPos_B->setEnabled(true);
        txRobotMcsPos_B->setReadOnly(true);
        txRobotMcsPos_B->setDecimals(3);
        txRobotMcsPos_B->setMinimum(-999999.000000000000000);
        txRobotMcsPos_B->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotMcsPos_B, 2, 5, 1, 1);

        txRobotMcsPos_C = new QDoubleSpinBox(groupBox_5);
        txRobotMcsPos_C->setObjectName(QString::fromUtf8("txRobotMcsPos_C"));
        txRobotMcsPos_C->setEnabled(true);
        txRobotMcsPos_C->setReadOnly(true);
        txRobotMcsPos_C->setDecimals(3);
        txRobotMcsPos_C->setMinimum(-999999.000000000000000);
        txRobotMcsPos_C->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotMcsPos_C, 2, 6, 1, 1);

        label_21 = new QLabel(groupBox_5);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_5->addWidget(label_21, 3, 0, 1, 1);

        txRobotWcsPos_X = new QDoubleSpinBox(groupBox_5);
        txRobotWcsPos_X->setObjectName(QString::fromUtf8("txRobotWcsPos_X"));
        txRobotWcsPos_X->setEnabled(true);
        txRobotWcsPos_X->setReadOnly(true);
        txRobotWcsPos_X->setDecimals(3);
        txRobotWcsPos_X->setMinimum(-999999.000000000000000);
        txRobotWcsPos_X->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotWcsPos_X, 3, 1, 1, 1);

        txRobotWcsPos_Y = new QDoubleSpinBox(groupBox_5);
        txRobotWcsPos_Y->setObjectName(QString::fromUtf8("txRobotWcsPos_Y"));
        txRobotWcsPos_Y->setEnabled(true);
        txRobotWcsPos_Y->setReadOnly(true);
        txRobotWcsPos_Y->setDecimals(3);
        txRobotWcsPos_Y->setMinimum(-999999.000000000000000);
        txRobotWcsPos_Y->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotWcsPos_Y, 3, 2, 1, 1);

        txRobotWcsPos_Z = new QDoubleSpinBox(groupBox_5);
        txRobotWcsPos_Z->setObjectName(QString::fromUtf8("txRobotWcsPos_Z"));
        txRobotWcsPos_Z->setEnabled(true);
        txRobotWcsPos_Z->setReadOnly(true);
        txRobotWcsPos_Z->setDecimals(3);
        txRobotWcsPos_Z->setMinimum(-999999.000000000000000);
        txRobotWcsPos_Z->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotWcsPos_Z, 3, 3, 1, 1);

        txRobotWcsPos_A = new QDoubleSpinBox(groupBox_5);
        txRobotWcsPos_A->setObjectName(QString::fromUtf8("txRobotWcsPos_A"));
        txRobotWcsPos_A->setEnabled(true);
        txRobotWcsPos_A->setReadOnly(true);
        txRobotWcsPos_A->setDecimals(3);
        txRobotWcsPos_A->setMinimum(-999999.000000000000000);
        txRobotWcsPos_A->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotWcsPos_A, 3, 4, 1, 1);

        txRobotWcsPos_B = new QDoubleSpinBox(groupBox_5);
        txRobotWcsPos_B->setObjectName(QString::fromUtf8("txRobotWcsPos_B"));
        txRobotWcsPos_B->setEnabled(true);
        txRobotWcsPos_B->setReadOnly(true);
        txRobotWcsPos_B->setDecimals(3);
        txRobotWcsPos_B->setMinimum(-999999.000000000000000);
        txRobotWcsPos_B->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotWcsPos_B, 3, 5, 1, 1);

        txRobotWcsPos_C = new QDoubleSpinBox(groupBox_5);
        txRobotWcsPos_C->setObjectName(QString::fromUtf8("txRobotWcsPos_C"));
        txRobotWcsPos_C->setEnabled(true);
        txRobotWcsPos_C->setReadOnly(true);
        txRobotWcsPos_C->setDecimals(3);
        txRobotWcsPos_C->setMinimum(-999999.000000000000000);
        txRobotWcsPos_C->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotWcsPos_C, 3, 6, 1, 1);

        label_20 = new QLabel(groupBox_5);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_5->addWidget(label_20, 4, 0, 1, 1);

        txRobotPcs1Pos_X = new QDoubleSpinBox(groupBox_5);
        txRobotPcs1Pos_X->setObjectName(QString::fromUtf8("txRobotPcs1Pos_X"));
        txRobotPcs1Pos_X->setEnabled(true);
        txRobotPcs1Pos_X->setReadOnly(true);
        txRobotPcs1Pos_X->setDecimals(3);
        txRobotPcs1Pos_X->setMinimum(-999999.000000000000000);
        txRobotPcs1Pos_X->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs1Pos_X, 4, 1, 1, 1);

        txRobotPcs1Pos_Y = new QDoubleSpinBox(groupBox_5);
        txRobotPcs1Pos_Y->setObjectName(QString::fromUtf8("txRobotPcs1Pos_Y"));
        txRobotPcs1Pos_Y->setEnabled(true);
        txRobotPcs1Pos_Y->setReadOnly(true);
        txRobotPcs1Pos_Y->setDecimals(3);
        txRobotPcs1Pos_Y->setMinimum(-999999.000000000000000);
        txRobotPcs1Pos_Y->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs1Pos_Y, 4, 2, 1, 1);

        txRobotPcs1Pos_Z = new QDoubleSpinBox(groupBox_5);
        txRobotPcs1Pos_Z->setObjectName(QString::fromUtf8("txRobotPcs1Pos_Z"));
        txRobotPcs1Pos_Z->setEnabled(true);
        txRobotPcs1Pos_Z->setReadOnly(true);
        txRobotPcs1Pos_Z->setDecimals(3);
        txRobotPcs1Pos_Z->setMinimum(-999999.000000000000000);
        txRobotPcs1Pos_Z->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs1Pos_Z, 4, 3, 1, 1);

        txRobotPcs1Pos_A = new QDoubleSpinBox(groupBox_5);
        txRobotPcs1Pos_A->setObjectName(QString::fromUtf8("txRobotPcs1Pos_A"));
        txRobotPcs1Pos_A->setEnabled(true);
        txRobotPcs1Pos_A->setReadOnly(true);
        txRobotPcs1Pos_A->setDecimals(3);
        txRobotPcs1Pos_A->setMinimum(-999999.000000000000000);
        txRobotPcs1Pos_A->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs1Pos_A, 4, 4, 1, 1);

        txRobotPcs1Pos_B = new QDoubleSpinBox(groupBox_5);
        txRobotPcs1Pos_B->setObjectName(QString::fromUtf8("txRobotPcs1Pos_B"));
        txRobotPcs1Pos_B->setEnabled(true);
        txRobotPcs1Pos_B->setReadOnly(true);
        txRobotPcs1Pos_B->setDecimals(3);
        txRobotPcs1Pos_B->setMinimum(-999999.000000000000000);
        txRobotPcs1Pos_B->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs1Pos_B, 4, 5, 1, 1);

        txRobotPcs1Pos_C = new QDoubleSpinBox(groupBox_5);
        txRobotPcs1Pos_C->setObjectName(QString::fromUtf8("txRobotPcs1Pos_C"));
        txRobotPcs1Pos_C->setEnabled(true);
        txRobotPcs1Pos_C->setReadOnly(true);
        txRobotPcs1Pos_C->setDecimals(3);
        txRobotPcs1Pos_C->setMinimum(-999999.000000000000000);
        txRobotPcs1Pos_C->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs1Pos_C, 4, 6, 1, 1);

        label_22 = new QLabel(groupBox_5);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_5->addWidget(label_22, 5, 0, 1, 1);

        txRobotPcs2Pos_X = new QDoubleSpinBox(groupBox_5);
        txRobotPcs2Pos_X->setObjectName(QString::fromUtf8("txRobotPcs2Pos_X"));
        txRobotPcs2Pos_X->setEnabled(true);
        txRobotPcs2Pos_X->setReadOnly(true);
        txRobotPcs2Pos_X->setDecimals(3);
        txRobotPcs2Pos_X->setMinimum(-999999.000000000000000);
        txRobotPcs2Pos_X->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs2Pos_X, 5, 1, 1, 1);

        txRobotPcs2Pos_Y = new QDoubleSpinBox(groupBox_5);
        txRobotPcs2Pos_Y->setObjectName(QString::fromUtf8("txRobotPcs2Pos_Y"));
        txRobotPcs2Pos_Y->setEnabled(true);
        txRobotPcs2Pos_Y->setReadOnly(true);
        txRobotPcs2Pos_Y->setDecimals(3);
        txRobotPcs2Pos_Y->setMinimum(-999999.000000000000000);
        txRobotPcs2Pos_Y->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs2Pos_Y, 5, 2, 1, 1);

        txRobotPcs2Pos_Z = new QDoubleSpinBox(groupBox_5);
        txRobotPcs2Pos_Z->setObjectName(QString::fromUtf8("txRobotPcs2Pos_Z"));
        txRobotPcs2Pos_Z->setEnabled(true);
        txRobotPcs2Pos_Z->setReadOnly(true);
        txRobotPcs2Pos_Z->setDecimals(3);
        txRobotPcs2Pos_Z->setMinimum(-999999.000000000000000);
        txRobotPcs2Pos_Z->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs2Pos_Z, 5, 3, 1, 1);

        txRobotPcs2Pos_A = new QDoubleSpinBox(groupBox_5);
        txRobotPcs2Pos_A->setObjectName(QString::fromUtf8("txRobotPcs2Pos_A"));
        txRobotPcs2Pos_A->setEnabled(true);
        txRobotPcs2Pos_A->setReadOnly(true);
        txRobotPcs2Pos_A->setDecimals(3);
        txRobotPcs2Pos_A->setMinimum(-999999.000000000000000);
        txRobotPcs2Pos_A->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs2Pos_A, 5, 4, 1, 1);

        txRobotPcs2Pos_B = new QDoubleSpinBox(groupBox_5);
        txRobotPcs2Pos_B->setObjectName(QString::fromUtf8("txRobotPcs2Pos_B"));
        txRobotPcs2Pos_B->setEnabled(true);
        txRobotPcs2Pos_B->setReadOnly(true);
        txRobotPcs2Pos_B->setDecimals(3);
        txRobotPcs2Pos_B->setMinimum(-999999.000000000000000);
        txRobotPcs2Pos_B->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs2Pos_B, 5, 5, 1, 1);

        txRobotPcs2Pos_C = new QDoubleSpinBox(groupBox_5);
        txRobotPcs2Pos_C->setObjectName(QString::fromUtf8("txRobotPcs2Pos_C"));
        txRobotPcs2Pos_C->setEnabled(true);
        txRobotPcs2Pos_C->setReadOnly(true);
        txRobotPcs2Pos_C->setDecimals(3);
        txRobotPcs2Pos_C->setMinimum(-999999.000000000000000);
        txRobotPcs2Pos_C->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotPcs2Pos_C, 5, 6, 1, 1);

        label_25 = new QLabel(groupBox_5);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_5->addWidget(label_25, 6, 0, 1, 1);

        txRobotTcsPos_X = new QDoubleSpinBox(groupBox_5);
        txRobotTcsPos_X->setObjectName(QString::fromUtf8("txRobotTcsPos_X"));
        txRobotTcsPos_X->setEnabled(true);
        txRobotTcsPos_X->setReadOnly(true);
        txRobotTcsPos_X->setDecimals(3);
        txRobotTcsPos_X->setMinimum(-999999.000000000000000);
        txRobotTcsPos_X->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotTcsPos_X, 6, 1, 1, 1);

        txRobotTcsPos_Y = new QDoubleSpinBox(groupBox_5);
        txRobotTcsPos_Y->setObjectName(QString::fromUtf8("txRobotTcsPos_Y"));
        txRobotTcsPos_Y->setEnabled(true);
        txRobotTcsPos_Y->setReadOnly(true);
        txRobotTcsPos_Y->setDecimals(3);
        txRobotTcsPos_Y->setMinimum(-999999.000000000000000);
        txRobotTcsPos_Y->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotTcsPos_Y, 6, 2, 1, 1);

        txRobotTcsPos_Z = new QDoubleSpinBox(groupBox_5);
        txRobotTcsPos_Z->setObjectName(QString::fromUtf8("txRobotTcsPos_Z"));
        txRobotTcsPos_Z->setEnabled(true);
        txRobotTcsPos_Z->setReadOnly(true);
        txRobotTcsPos_Z->setDecimals(3);
        txRobotTcsPos_Z->setMinimum(-999999.000000000000000);
        txRobotTcsPos_Z->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotTcsPos_Z, 6, 3, 1, 1);

        txRobotTcsPos_A = new QDoubleSpinBox(groupBox_5);
        txRobotTcsPos_A->setObjectName(QString::fromUtf8("txRobotTcsPos_A"));
        txRobotTcsPos_A->setEnabled(true);
        txRobotTcsPos_A->setReadOnly(true);
        txRobotTcsPos_A->setDecimals(3);
        txRobotTcsPos_A->setMinimum(-999999.000000000000000);
        txRobotTcsPos_A->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotTcsPos_A, 6, 4, 1, 1);

        txRobotTcsPos_B = new QDoubleSpinBox(groupBox_5);
        txRobotTcsPos_B->setObjectName(QString::fromUtf8("txRobotTcsPos_B"));
        txRobotTcsPos_B->setEnabled(true);
        txRobotTcsPos_B->setReadOnly(true);
        txRobotTcsPos_B->setDecimals(3);
        txRobotTcsPos_B->setMinimum(-999999.000000000000000);
        txRobotTcsPos_B->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotTcsPos_B, 6, 5, 1, 1);

        txRobotTcsPos_C = new QDoubleSpinBox(groupBox_5);
        txRobotTcsPos_C->setObjectName(QString::fromUtf8("txRobotTcsPos_C"));
        txRobotTcsPos_C->setEnabled(true);
        txRobotTcsPos_C->setReadOnly(true);
        txRobotTcsPos_C->setDecimals(3);
        txRobotTcsPos_C->setMinimum(-999999.000000000000000);
        txRobotTcsPos_C->setMaximum(999999.989999999990687);

        gridLayout_5->addWidget(txRobotTcsPos_C, 6, 6, 1, 1);


        gridLayout_12->addWidget(groupBox_5, 0, 0, 1, 3);

        groupBox_17 = new QGroupBox(tab_2);
        groupBox_17->setObjectName(QString::fromUtf8("groupBox_17"));
        gridLayout_23 = new QGridLayout(groupBox_17);
        gridLayout_23->setSpacing(6);
        gridLayout_23->setContentsMargins(11, 11, 11, 11);
        gridLayout_23->setObjectName(QString::fromUtf8("gridLayout_23"));
        btCAxisToolPeriod = new QPushButton(groupBox_17);
        btCAxisToolPeriod->setObjectName(QString::fromUtf8("btCAxisToolPeriod"));

        gridLayout_23->addWidget(btCAxisToolPeriod, 0, 2, 1, 1);

        txRobotToolPeriod = new QSpinBox(groupBox_17);
        txRobotToolPeriod->setObjectName(QString::fromUtf8("txRobotToolPeriod"));
        txRobotToolPeriod->setMinimum(-99);

        gridLayout_23->addWidget(txRobotToolPeriod, 0, 1, 1, 1);

        label = new QLabel(groupBox_17);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_23->addWidget(label, 0, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_17, 3, 0, 2, 1);

        groupBox_21 = new QGroupBox(tab_2);
        groupBox_21->setObjectName(QString::fromUtf8("groupBox_21"));
        gridLayout_32 = new QGridLayout(groupBox_21);
        gridLayout_32->setSpacing(6);
        gridLayout_32->setContentsMargins(11, 11, 11, 11);
        gridLayout_32->setObjectName(QString::fromUtf8("gridLayout_32"));
        ckRobotEnabled = new QCheckBox(groupBox_21);
        ckRobotEnabled->setObjectName(QString::fromUtf8("ckRobotEnabled"));
        ckRobotEnabled->setEnabled(true);

        gridLayout_32->addWidget(ckRobotEnabled, 0, 0, 1, 1);

        ckRobotCommStatus = new QCheckBox(groupBox_21);
        ckRobotCommStatus->setObjectName(QString::fromUtf8("ckRobotCommStatus"));
        ckRobotCommStatus->setEnabled(true);

        gridLayout_32->addWidget(ckRobotCommStatus, 1, 0, 1, 1);

        ckRobotPowerStatus = new QCheckBox(groupBox_21);
        ckRobotPowerStatus->setObjectName(QString::fromUtf8("ckRobotPowerStatus"));
        ckRobotPowerStatus->setEnabled(true);

        gridLayout_32->addWidget(ckRobotPowerStatus, 2, 0, 1, 1);

        ckRobotRefStatus = new QCheckBox(groupBox_21);
        ckRobotRefStatus->setObjectName(QString::fromUtf8("ckRobotRefStatus"));
        ckRobotRefStatus->setEnabled(true);

        gridLayout_32->addWidget(ckRobotRefStatus, 3, 0, 1, 2);

        ckRobotStoppedStatus = new QCheckBox(groupBox_21);
        ckRobotStoppedStatus->setObjectName(QString::fromUtf8("ckRobotStoppedStatus"));
        ckRobotStoppedStatus->setEnabled(true);

        gridLayout_32->addWidget(ckRobotStoppedStatus, 4, 0, 1, 2);

        ckRobotHomingStatus = new QCheckBox(groupBox_21);
        ckRobotHomingStatus->setObjectName(QString::fromUtf8("ckRobotHomingStatus"));
        ckRobotHomingStatus->setEnabled(true);

        gridLayout_32->addWidget(ckRobotHomingStatus, 5, 0, 1, 1);

        ckRobotTrackingStatus = new QCheckBox(groupBox_21);
        ckRobotTrackingStatus->setObjectName(QString::fromUtf8("ckRobotTrackingStatus"));
        ckRobotTrackingStatus->setEnabled(true);

        gridLayout_32->addWidget(ckRobotTrackingStatus, 6, 0, 1, 2);

        ckRobotDone = new QCheckBox(groupBox_21);
        ckRobotDone->setObjectName(QString::fromUtf8("ckRobotDone"));
        ckRobotDone->setEnabled(true);

        gridLayout_32->addWidget(ckRobotDone, 7, 0, 1, 2);

        ckRobotBusy = new QCheckBox(groupBox_21);
        ckRobotBusy->setObjectName(QString::fromUtf8("ckRobotBusy"));
        ckRobotBusy->setEnabled(true);

        gridLayout_32->addWidget(ckRobotBusy, 8, 0, 1, 2);

        ckRobotActive = new QCheckBox(groupBox_21);
        ckRobotActive->setObjectName(QString::fromUtf8("ckRobotActive"));
        ckRobotActive->setEnabled(true);

        gridLayout_32->addWidget(ckRobotActive, 9, 0, 1, 2);

        ckRobotAborted = new QCheckBox(groupBox_21);
        ckRobotAborted->setObjectName(QString::fromUtf8("ckRobotAborted"));
        ckRobotAborted->setEnabled(true);

        gridLayout_32->addWidget(ckRobotAborted, 10, 0, 1, 2);

        ckRobotAccepted = new QCheckBox(groupBox_21);
        ckRobotAccepted->setObjectName(QString::fromUtf8("ckRobotAccepted"));
        ckRobotAccepted->setEnabled(true);

        gridLayout_32->addWidget(ckRobotAccepted, 11, 0, 1, 2);

        ckRobotError = new QCheckBox(groupBox_21);
        ckRobotError->setObjectName(QString::fromUtf8("ckRobotError"));
        ckRobotError->setEnabled(true);

        gridLayout_32->addWidget(ckRobotError, 12, 0, 1, 2);

        ckRobotAlarm = new QCheckBox(groupBox_21);
        ckRobotAlarm->setObjectName(QString::fromUtf8("ckRobotAlarm"));
        ckRobotAlarm->setEnabled(true);

        gridLayout_32->addWidget(ckRobotAlarm, 13, 0, 1, 2);

        line = new QFrame(groupBox_21);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_32->addWidget(line, 14, 0, 1, 2);

        lbHomeStatus_62 = new QLabel(groupBox_21);
        lbHomeStatus_62->setObjectName(QString::fromUtf8("lbHomeStatus_62"));

        gridLayout_32->addWidget(lbHomeStatus_62, 16, 0, 1, 1);

        txRobotState = new QSpinBox(groupBox_21);
        txRobotState->setObjectName(QString::fromUtf8("txRobotState"));

        gridLayout_32->addWidget(txRobotState, 16, 1, 1, 1);

        lbHomeStatus_63 = new QLabel(groupBox_21);
        lbHomeStatus_63->setObjectName(QString::fromUtf8("lbHomeStatus_63"));

        gridLayout_32->addWidget(lbHomeStatus_63, 17, 0, 1, 1);

        txRobotErrId = new QSpinBox(groupBox_21);
        txRobotErrId->setObjectName(QString::fromUtf8("txRobotErrId"));

        gridLayout_32->addWidget(txRobotErrId, 17, 1, 1, 1);

        lbHomeStatus_64 = new QLabel(groupBox_21);
        lbHomeStatus_64->setObjectName(QString::fromUtf8("lbHomeStatus_64"));

        gridLayout_32->addWidget(lbHomeStatus_64, 18, 0, 1, 1);

        txRobotAlarmId = new QSpinBox(groupBox_21);
        txRobotAlarmId->setObjectName(QString::fromUtf8("txRobotAlarmId"));
        txRobotAlarmId->setMaximum(999999);

        gridLayout_32->addWidget(txRobotAlarmId, 18, 1, 1, 1);

        lbHomeStatus_65 = new QLabel(groupBox_21);
        lbHomeStatus_65->setObjectName(QString::fromUtf8("lbHomeStatus_65"));

        gridLayout_32->addWidget(lbHomeStatus_65, 19, 0, 1, 1);

        txRobotMotionState = new QSpinBox(groupBox_21);
        txRobotMotionState->setObjectName(QString::fromUtf8("txRobotMotionState"));

        gridLayout_32->addWidget(txRobotMotionState, 19, 1, 1, 1);

        lbHomeStatus_66 = new QLabel(groupBox_21);
        lbHomeStatus_66->setObjectName(QString::fromUtf8("lbHomeStatus_66"));

        gridLayout_32->addWidget(lbHomeStatus_66, 20, 0, 1, 1);

        txRobotMvId = new QSpinBox(groupBox_21);
        txRobotMvId->setObjectName(QString::fromUtf8("txRobotMvId"));

        gridLayout_32->addWidget(txRobotMvId, 20, 1, 1, 1);

        lbHomeStatus_67 = new QLabel(groupBox_21);
        lbHomeStatus_67->setObjectName(QString::fromUtf8("lbHomeStatus_67"));

        gridLayout_32->addWidget(lbHomeStatus_67, 21, 0, 1, 1);

        txRobotActiveId = new QSpinBox(groupBox_21);
        txRobotActiveId->setObjectName(QString::fromUtf8("txRobotActiveId"));

        gridLayout_32->addWidget(txRobotActiveId, 21, 1, 1, 1);

        lbHomeStatus_68 = new QLabel(groupBox_21);
        lbHomeStatus_68->setObjectName(QString::fromUtf8("lbHomeStatus_68"));

        gridLayout_32->addWidget(lbHomeStatus_68, 22, 0, 1, 1);

        txRobotFirstRefId = new QSpinBox(groupBox_21);
        txRobotFirstRefId->setObjectName(QString::fromUtf8("txRobotFirstRefId"));

        gridLayout_32->addWidget(txRobotFirstRefId, 22, 1, 1, 1);

        lbHomeStatus_69 = new QLabel(groupBox_21);
        lbHomeStatus_69->setObjectName(QString::fromUtf8("lbHomeStatus_69"));

        gridLayout_32->addWidget(lbHomeStatus_69, 23, 0, 1, 1);

        txRobotLastAcceptedId = new QSpinBox(groupBox_21);
        txRobotLastAcceptedId->setObjectName(QString::fromUtf8("txRobotLastAcceptedId"));

        gridLayout_32->addWidget(txRobotLastAcceptedId, 23, 1, 1, 1);


        gridLayout_12->addWidget(groupBox_21, 1, 2, 5, 1);

        groupBox_8 = new QGroupBox(tab_2);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        gridLayout_26 = new QGridLayout(groupBox_8);
        gridLayout_26->setSpacing(6);
        gridLayout_26->setContentsMargins(11, 11, 11, 11);
        gridLayout_26->setObjectName(QString::fromUtf8("gridLayout_26"));
        groupBox_6 = new QGroupBox(groupBox_8);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        verticalLayout_8 = new QVBoxLayout(groupBox_6);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        ckRobotEnable = new QCheckBox(groupBox_6);
        ckRobotEnable->setObjectName(QString::fromUtf8("ckRobotEnable"));

        verticalLayout_8->addWidget(ckRobotEnable);

        ckRobotPowerOn = new QCheckBox(groupBox_6);
        ckRobotPowerOn->setObjectName(QString::fromUtf8("ckRobotPowerOn"));

        verticalLayout_8->addWidget(ckRobotPowerOn);

        ckRobotRel = new QCheckBox(groupBox_6);
        ckRobotRel->setObjectName(QString::fromUtf8("ckRobotRel"));

        verticalLayout_8->addWidget(ckRobotRel);

        btRobotPTP = new QPushButton(groupBox_6);
        btRobotPTP->setObjectName(QString::fromUtf8("btRobotPTP"));

        verticalLayout_8->addWidget(btRobotPTP);

        btRobotLinear = new QPushButton(groupBox_6);
        btRobotLinear->setObjectName(QString::fromUtf8("btRobotLinear"));

        verticalLayout_8->addWidget(btRobotLinear);

        btRobotCirc = new QPushButton(groupBox_6);
        btRobotCirc->setObjectName(QString::fromUtf8("btRobotCirc"));
        btRobotCirc->setEnabled(false);

        verticalLayout_8->addWidget(btRobotCirc);

        btRobotRdTool = new QPushButton(groupBox_6);
        btRobotRdTool->setObjectName(QString::fromUtf8("btRobotRdTool"));

        verticalLayout_8->addWidget(btRobotRdTool);

        btRobotWrTool = new QPushButton(groupBox_6);
        btRobotWrTool->setObjectName(QString::fromUtf8("btRobotWrTool"));

        verticalLayout_8->addWidget(btRobotWrTool);

        btRobotSetPos = new QPushButton(groupBox_6);
        btRobotSetPos->setObjectName(QString::fromUtf8("btRobotSetPos"));

        verticalLayout_8->addWidget(btRobotSetPos);

        btRobotHome = new QPushButton(groupBox_6);
        btRobotHome->setObjectName(QString::fromUtf8("btRobotHome"));

        verticalLayout_8->addWidget(btRobotHome);

        btRobotHalt = new QPushButton(groupBox_6);
        btRobotHalt->setObjectName(QString::fromUtf8("btRobotHalt"));

        verticalLayout_8->addWidget(btRobotHalt);

        btRobotReset = new QPushButton(groupBox_6);
        btRobotReset->setObjectName(QString::fromUtf8("btRobotReset"));

        verticalLayout_8->addWidget(btRobotReset);

        btRobotJump = new QPushButton(groupBox_6);
        btRobotJump->setObjectName(QString::fromUtf8("btRobotJump"));

        verticalLayout_8->addWidget(btRobotJump);


        gridLayout_26->addWidget(groupBox_6, 0, 3, 4, 1);

        groupBox_4 = new QGroupBox(groupBox_8);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout = new QGridLayout(groupBox_4);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        rbTargetPos = new QRadioButton(groupBox_4);
        rbTargetPos->setObjectName(QString::fromUtf8("rbTargetPos"));
        rbTargetPos->setChecked(true);

        gridLayout->addWidget(rbTargetPos, 0, 0, 1, 1);

        cbCircDir = new QComboBox(groupBox_4);
        cbCircDir->addItem(QString());
        cbCircDir->addItem(QString());
        cbCircDir->setObjectName(QString::fromUtf8("cbCircDir"));

        gridLayout->addWidget(cbCircDir, 3, 1, 1, 1);

        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        cbCircPath = new QComboBox(groupBox_4);
        cbCircPath->addItem(QString());
        cbCircPath->addItem(QString());
        cbCircPath->addItem(QString());
        cbCircPath->setObjectName(QString::fromUtf8("cbCircPath"));

        gridLayout->addWidget(cbCircPath, 4, 1, 1, 1);

        cbRobotCoord = new QComboBox(groupBox_4);
        cbRobotCoord->addItem(QString());
        cbRobotCoord->addItem(QString());
        cbRobotCoord->addItem(QString());
        cbRobotCoord->addItem(QString());
        cbRobotCoord->addItem(QString());
        cbRobotCoord->addItem(QString());
        cbRobotCoord->setObjectName(QString::fromUtf8("cbRobotCoord"));

        gridLayout->addWidget(cbRobotCoord, 2, 1, 1, 1);

        lbCircDir = new QLabel(groupBox_4);
        lbCircDir->setObjectName(QString::fromUtf8("lbCircDir"));

        gridLayout->addWidget(lbCircDir, 3, 0, 1, 1);

        lbCircPath = new QLabel(groupBox_4);
        lbCircPath->setObjectName(QString::fromUtf8("lbCircPath"));

        gridLayout->addWidget(lbCircPath, 4, 0, 1, 1);

        rbMidPos = new QRadioButton(groupBox_4);
        rbMidPos->setObjectName(QString::fromUtf8("rbMidPos"));

        gridLayout->addWidget(rbMidPos, 0, 1, 1, 1);


        gridLayout_26->addWidget(groupBox_4, 0, 0, 1, 1);

        groupBox_7 = new QGroupBox(groupBox_8);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        gridLayout_24 = new QGridLayout(groupBox_7);
        gridLayout_24->setSpacing(3);
        gridLayout_24->setContentsMargins(11, 11, 11, 11);
        gridLayout_24->setObjectName(QString::fromUtf8("gridLayout_24"));
        gridLayout_24->setContentsMargins(3, 3, -1, 3);
        stkpos = new QStackedWidget(groupBox_7);
        stkpos->setObjectName(QString::fromUtf8("stkpos"));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        gridLayout_20 = new QGridLayout(page_3);
        gridLayout_20->setSpacing(6);
        gridLayout_20->setContentsMargins(11, 11, 11, 11);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        lbHomeStatus_10 = new QLabel(page_3);
        lbHomeStatus_10->setObjectName(QString::fromUtf8("lbHomeStatus_10"));

        gridLayout_20->addWidget(lbHomeStatus_10, 0, 0, 1, 1);

        txRobotRefPosA1 = new QDoubleSpinBox(page_3);
        txRobotRefPosA1->setObjectName(QString::fromUtf8("txRobotRefPosA1"));
        txRobotRefPosA1->setReadOnly(false);
        txRobotRefPosA1->setDecimals(2);
        txRobotRefPosA1->setMinimum(-999999.000000000000000);
        txRobotRefPosA1->setMaximum(999999.989999999990687);
        txRobotRefPosA1->setValue(0.000000000000000);

        gridLayout_20->addWidget(txRobotRefPosA1, 0, 1, 1, 1);

        lbHomeStatus_21 = new QLabel(page_3);
        lbHomeStatus_21->setObjectName(QString::fromUtf8("lbHomeStatus_21"));

        gridLayout_20->addWidget(lbHomeStatus_21, 1, 0, 1, 1);

        txRobotRefPosA2 = new QDoubleSpinBox(page_3);
        txRobotRefPosA2->setObjectName(QString::fromUtf8("txRobotRefPosA2"));
        txRobotRefPosA2->setReadOnly(false);
        txRobotRefPosA2->setDecimals(2);
        txRobotRefPosA2->setMinimum(-999999.000000000000000);
        txRobotRefPosA2->setMaximum(999999.989999999990687);
        txRobotRefPosA2->setValue(0.000000000000000);

        gridLayout_20->addWidget(txRobotRefPosA2, 1, 1, 1, 1);

        lbHomeStatus_22 = new QLabel(page_3);
        lbHomeStatus_22->setObjectName(QString::fromUtf8("lbHomeStatus_22"));

        gridLayout_20->addWidget(lbHomeStatus_22, 2, 0, 1, 1);

        txRobotRefPosA3 = new QDoubleSpinBox(page_3);
        txRobotRefPosA3->setObjectName(QString::fromUtf8("txRobotRefPosA3"));
        txRobotRefPosA3->setReadOnly(false);
        txRobotRefPosA3->setDecimals(2);
        txRobotRefPosA3->setMinimum(-999999.000000000000000);
        txRobotRefPosA3->setMaximum(999999.989999999990687);
        txRobotRefPosA3->setValue(0.000000000000000);

        gridLayout_20->addWidget(txRobotRefPosA3, 2, 1, 1, 1);

        lbHomeStatus_23 = new QLabel(page_3);
        lbHomeStatus_23->setObjectName(QString::fromUtf8("lbHomeStatus_23"));

        gridLayout_20->addWidget(lbHomeStatus_23, 3, 0, 1, 1);

        txRobotRefPosA4 = new QDoubleSpinBox(page_3);
        txRobotRefPosA4->setObjectName(QString::fromUtf8("txRobotRefPosA4"));
        txRobotRefPosA4->setReadOnly(false);
        txRobotRefPosA4->setDecimals(2);
        txRobotRefPosA4->setMinimum(-999999.000000000000000);
        txRobotRefPosA4->setMaximum(999999.989999999990687);
        txRobotRefPosA4->setValue(0.000000000000000);

        gridLayout_20->addWidget(txRobotRefPosA4, 3, 1, 1, 1);

        label_3 = new QLabel(page_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_20->addWidget(label_3, 4, 0, 1, 1);

        txRobotRefPosA5 = new QDoubleSpinBox(page_3);
        txRobotRefPosA5->setObjectName(QString::fromUtf8("txRobotRefPosA5"));
        txRobotRefPosA5->setReadOnly(false);
        txRobotRefPosA5->setDecimals(2);
        txRobotRefPosA5->setMinimum(-999999.000000000000000);
        txRobotRefPosA5->setMaximum(999999.989999999990687);
        txRobotRefPosA5->setValue(0.000000000000000);

        gridLayout_20->addWidget(txRobotRefPosA5, 4, 1, 1, 1);

        label_4 = new QLabel(page_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_20->addWidget(label_4, 5, 0, 1, 1);

        txRobotRefPosA6 = new QDoubleSpinBox(page_3);
        txRobotRefPosA6->setObjectName(QString::fromUtf8("txRobotRefPosA6"));
        txRobotRefPosA6->setReadOnly(false);
        txRobotRefPosA6->setDecimals(2);
        txRobotRefPosA6->setMinimum(-999999.000000000000000);
        txRobotRefPosA6->setMaximum(999999.989999999990687);
        txRobotRefPosA6->setValue(0.000000000000000);

        gridLayout_20->addWidget(txRobotRefPosA6, 5, 1, 1, 1);

        stkpos->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        gridLayout_18 = new QGridLayout(page_4);
        gridLayout_18->setSpacing(6);
        gridLayout_18->setContentsMargins(11, 11, 11, 11);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        lbHomeStatus_72 = new QLabel(page_4);
        lbHomeStatus_72->setObjectName(QString::fromUtf8("lbHomeStatus_72"));

        gridLayout_18->addWidget(lbHomeStatus_72, 0, 0, 1, 1);

        lbHomeStatus_98 = new QLabel(page_4);
        lbHomeStatus_98->setObjectName(QString::fromUtf8("lbHomeStatus_98"));

        gridLayout_18->addWidget(lbHomeStatus_98, 5, 0, 1, 1);

        txRobotRefPosA = new QDoubleSpinBox(page_4);
        txRobotRefPosA->setObjectName(QString::fromUtf8("txRobotRefPosA"));
        txRobotRefPosA->setReadOnly(false);
        txRobotRefPosA->setDecimals(2);
        txRobotRefPosA->setMinimum(-999999.000000000000000);
        txRobotRefPosA->setMaximum(999999.989999999990687);

        gridLayout_18->addWidget(txRobotRefPosA, 3, 1, 1, 1);

        txRobotRefPosX = new QDoubleSpinBox(page_4);
        txRobotRefPosX->setObjectName(QString::fromUtf8("txRobotRefPosX"));
        txRobotRefPosX->setReadOnly(false);
        txRobotRefPosX->setDecimals(2);
        txRobotRefPosX->setMinimum(-999999.000000000000000);
        txRobotRefPosX->setMaximum(999999.989999999990687);

        gridLayout_18->addWidget(txRobotRefPosX, 0, 1, 1, 1);

        txRobotRefPosB = new QDoubleSpinBox(page_4);
        txRobotRefPosB->setObjectName(QString::fromUtf8("txRobotRefPosB"));
        txRobotRefPosB->setReadOnly(false);
        txRobotRefPosB->setDecimals(2);
        txRobotRefPosB->setMinimum(-999999.000000000000000);
        txRobotRefPosB->setMaximum(999999.989999999990687);

        gridLayout_18->addWidget(txRobotRefPosB, 4, 1, 1, 1);

        lbHomeStatus_73 = new QLabel(page_4);
        lbHomeStatus_73->setObjectName(QString::fromUtf8("lbHomeStatus_73"));

        gridLayout_18->addWidget(lbHomeStatus_73, 3, 0, 1, 1);

        txRobotRefPosC = new QDoubleSpinBox(page_4);
        txRobotRefPosC->setObjectName(QString::fromUtf8("txRobotRefPosC"));
        txRobotRefPosC->setReadOnly(false);
        txRobotRefPosC->setDecimals(2);
        txRobotRefPosC->setMinimum(-999999.000000000000000);
        txRobotRefPosC->setMaximum(999999.989999999990687);

        gridLayout_18->addWidget(txRobotRefPosC, 5, 1, 1, 1);

        lbHomeStatus_71 = new QLabel(page_4);
        lbHomeStatus_71->setObjectName(QString::fromUtf8("lbHomeStatus_71"));

        gridLayout_18->addWidget(lbHomeStatus_71, 1, 0, 1, 1);

        lbHomeStatus_70 = new QLabel(page_4);
        lbHomeStatus_70->setObjectName(QString::fromUtf8("lbHomeStatus_70"));

        gridLayout_18->addWidget(lbHomeStatus_70, 2, 0, 1, 1);

        lbHomeStatus_96 = new QLabel(page_4);
        lbHomeStatus_96->setObjectName(QString::fromUtf8("lbHomeStatus_96"));

        gridLayout_18->addWidget(lbHomeStatus_96, 4, 0, 1, 1);

        txRobotRefPosY = new QDoubleSpinBox(page_4);
        txRobotRefPosY->setObjectName(QString::fromUtf8("txRobotRefPosY"));
        txRobotRefPosY->setReadOnly(false);
        txRobotRefPosY->setDecimals(2);
        txRobotRefPosY->setMinimum(-999999.000000000000000);
        txRobotRefPosY->setMaximum(999999.989999999990687);

        gridLayout_18->addWidget(txRobotRefPosY, 1, 1, 1, 1);

        txRobotRefPosZ = new QDoubleSpinBox(page_4);
        txRobotRefPosZ->setObjectName(QString::fromUtf8("txRobotRefPosZ"));
        txRobotRefPosZ->setReadOnly(false);
        txRobotRefPosZ->setDecimals(2);
        txRobotRefPosZ->setMinimum(-999999.000000000000000);
        txRobotRefPosZ->setMaximum(999999.989999999990687);

        gridLayout_18->addWidget(txRobotRefPosZ, 2, 1, 1, 1);

        stkpos->addWidget(page_4);

        gridLayout_24->addWidget(stkpos, 0, 0, 1, 1);


        gridLayout_26->addWidget(groupBox_7, 1, 0, 1, 1);

        groupBox_31 = new QGroupBox(groupBox_8);
        groupBox_31->setObjectName(QString::fromUtf8("groupBox_31"));
        horizontalLayout_4 = new QHBoxLayout(groupBox_31);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lbHomeStatus_15 = new QLabel(groupBox_31);
        lbHomeStatus_15->setObjectName(QString::fromUtf8("lbHomeStatus_15"));

        horizontalLayout_4->addWidget(lbHomeStatus_15);

        txRobotSafeHeight = new QDoubleSpinBox(groupBox_31);
        txRobotSafeHeight->setObjectName(QString::fromUtf8("txRobotSafeHeight"));
        txRobotSafeHeight->setReadOnly(false);
        txRobotSafeHeight->setDecimals(2);
        txRobotSafeHeight->setMinimum(-999999.000000000000000);
        txRobotSafeHeight->setMaximum(999999.989999999990687);
        txRobotSafeHeight->setValue(0.000000000000000);

        horizontalLayout_4->addWidget(txRobotSafeHeight);

        btRobotSetSafeHeight = new QPushButton(groupBox_31);
        btRobotSetSafeHeight->setObjectName(QString::fromUtf8("btRobotSetSafeHeight"));

        horizontalLayout_4->addWidget(btRobotSetSafeHeight);


        gridLayout_26->addWidget(groupBox_31, 2, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_8, 1, 1, 3, 1);

        groupBox_27 = new QGroupBox(tab_2);
        groupBox_27->setObjectName(QString::fromUtf8("groupBox_27"));
        gridLayout_2 = new QGridLayout(groupBox_27);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        btRobotJogFwd = new QPushButton(groupBox_27);
        btRobotJogFwd->setObjectName(QString::fromUtf8("btRobotJogFwd"));
        btRobotJogFwd->setCheckable(false);

        gridLayout_2->addWidget(btRobotJogFwd, 0, 2, 1, 1);

        lbHomeStatus_86 = new QLabel(groupBox_27);
        lbHomeStatus_86->setObjectName(QString::fromUtf8("lbHomeStatus_86"));

        gridLayout_2->addWidget(lbHomeStatus_86, 4, 0, 1, 1);

        cbRobotJogCoord = new QComboBox(groupBox_27);
        cbRobotJogCoord->addItem(QString());
        cbRobotJogCoord->addItem(QString());
        cbRobotJogCoord->addItem(QString());
        cbRobotJogCoord->addItem(QString());
        cbRobotJogCoord->addItem(QString());
        cbRobotJogCoord->addItem(QString());
        cbRobotJogCoord->setObjectName(QString::fromUtf8("cbRobotJogCoord"));

        gridLayout_2->addWidget(cbRobotJogCoord, 0, 1, 1, 1);

        btRobotJogBwd = new QPushButton(groupBox_27);
        btRobotJogBwd->setObjectName(QString::fromUtf8("btRobotJogBwd"));
        btRobotJogBwd->setCheckable(false);

        gridLayout_2->addWidget(btRobotJogBwd, 1, 2, 1, 1);

        txRobotJogDec = new QDoubleSpinBox(groupBox_27);
        txRobotJogDec->setObjectName(QString::fromUtf8("txRobotJogDec"));
        txRobotJogDec->setReadOnly(false);
        txRobotJogDec->setDecimals(2);
        txRobotJogDec->setMinimum(-999999.000000000000000);
        txRobotJogDec->setMaximum(999999.989999999990687);
        txRobotJogDec->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogDec, 6, 1, 1, 1);

        txRobotJogVel = new QDoubleSpinBox(groupBox_27);
        txRobotJogVel->setObjectName(QString::fromUtf8("txRobotJogVel"));
        txRobotJogVel->setReadOnly(false);
        txRobotJogVel->setDecimals(2);
        txRobotJogVel->setMinimum(-999999.000000000000000);
        txRobotJogVel->setMaximum(999999.989999999990687);
        txRobotJogVel->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogVel, 4, 1, 1, 1);

        cbRobotJogAxisSel = new QComboBox(groupBox_27);
        cbRobotJogAxisSel->addItem(QString());
        cbRobotJogAxisSel->addItem(QString());
        cbRobotJogAxisSel->addItem(QString());
        cbRobotJogAxisSel->addItem(QString());
        cbRobotJogAxisSel->addItem(QString());
        cbRobotJogAxisSel->addItem(QString());
        cbRobotJogAxisSel->setObjectName(QString::fromUtf8("cbRobotJogAxisSel"));

        gridLayout_2->addWidget(cbRobotJogAxisSel, 1, 1, 1, 1);

        ckRobotJogAbcAsAcs = new QCheckBox(groupBox_27);
        ckRobotJogAbcAsAcs->setObjectName(QString::fromUtf8("ckRobotJogAbcAsAcs"));

        gridLayout_2->addWidget(ckRobotJogAbcAsAcs, 8, 2, 1, 1);

        lbHomeStatus_90 = new QLabel(groupBox_27);
        lbHomeStatus_90->setObjectName(QString::fromUtf8("lbHomeStatus_90"));

        gridLayout_2->addWidget(lbHomeStatus_90, 5, 0, 1, 1);

        lbHomeStatus_91 = new QLabel(groupBox_27);
        lbHomeStatus_91->setObjectName(QString::fromUtf8("lbHomeStatus_91"));

        gridLayout_2->addWidget(lbHomeStatus_91, 6, 0, 1, 1);

        lbHomeStatus_97 = new QLabel(groupBox_27);
        lbHomeStatus_97->setObjectName(QString::fromUtf8("lbHomeStatus_97"));

        gridLayout_2->addWidget(lbHomeStatus_97, 8, 0, 1, 1);

        txRobotJogJerk = new QDoubleSpinBox(groupBox_27);
        txRobotJogJerk->setObjectName(QString::fromUtf8("txRobotJogJerk"));
        txRobotJogJerk->setReadOnly(false);
        txRobotJogJerk->setDecimals(2);
        txRobotJogJerk->setMinimum(-999999.000000000000000);
        txRobotJogJerk->setMaximum(999999.989999999990687);
        txRobotJogJerk->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogJerk, 7, 1, 1, 1);

        lbHomeStatus_103 = new QLabel(groupBox_27);
        lbHomeStatus_103->setObjectName(QString::fromUtf8("lbHomeStatus_103"));

        gridLayout_2->addWidget(lbHomeStatus_103, 2, 0, 1, 1);

        txRobotJogKVel = new QDoubleSpinBox(groupBox_27);
        txRobotJogKVel->setObjectName(QString::fromUtf8("txRobotJogKVel"));
        txRobotJogKVel->setWrapping(false);
        txRobotJogKVel->setReadOnly(false);
        txRobotJogKVel->setDecimals(2);
        txRobotJogKVel->setMinimum(-999999.000000000000000);
        txRobotJogKVel->setMaximum(999999.989999999990687);
        txRobotJogKVel->setStepType(QAbstractSpinBox::DefaultStepType);
        txRobotJogKVel->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogKVel, 4, 2, 1, 1);

        lbHomeStatus_102 = new QLabel(groupBox_27);
        lbHomeStatus_102->setObjectName(QString::fromUtf8("lbHomeStatus_102"));

        gridLayout_2->addWidget(lbHomeStatus_102, 0, 0, 1, 1);

        txRobotJogAcc = new QDoubleSpinBox(groupBox_27);
        txRobotJogAcc->setObjectName(QString::fromUtf8("txRobotJogAcc"));
        txRobotJogAcc->setReadOnly(false);
        txRobotJogAcc->setDecimals(2);
        txRobotJogAcc->setMinimum(-999999.000000000000000);
        txRobotJogAcc->setMaximum(999999.989999999990687);
        txRobotJogAcc->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogAcc, 5, 1, 1, 1);

        txRobotJogDist = new QDoubleSpinBox(groupBox_27);
        txRobotJogDist->setObjectName(QString::fromUtf8("txRobotJogDist"));
        txRobotJogDist->setReadOnly(false);
        txRobotJogDist->setDecimals(2);
        txRobotJogDist->setMinimum(-999999.000000000000000);
        txRobotJogDist->setMaximum(999999.989999999990687);
        txRobotJogDist->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogDist, 2, 1, 1, 1);

        lbHomeStatus_92 = new QLabel(groupBox_27);
        lbHomeStatus_92->setObjectName(QString::fromUtf8("lbHomeStatus_92"));

        gridLayout_2->addWidget(lbHomeStatus_92, 7, 0, 1, 1);

        txRobotJogKJerk = new QDoubleSpinBox(groupBox_27);
        txRobotJogKJerk->setObjectName(QString::fromUtf8("txRobotJogKJerk"));
        txRobotJogKJerk->setReadOnly(false);
        txRobotJogKJerk->setDecimals(2);
        txRobotJogKJerk->setMinimum(-999999.000000000000000);
        txRobotJogKJerk->setMaximum(999999.989999999990687);
        txRobotJogKJerk->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogKJerk, 7, 2, 1, 1);

        lbHomeStatus_105 = new QLabel(groupBox_27);
        lbHomeStatus_105->setObjectName(QString::fromUtf8("lbHomeStatus_105"));

        gridLayout_2->addWidget(lbHomeStatus_105, 1, 0, 1, 1);

        txRobotJogKTor = new QDoubleSpinBox(groupBox_27);
        txRobotJogKTor->setObjectName(QString::fromUtf8("txRobotJogKTor"));
        txRobotJogKTor->setReadOnly(false);
        txRobotJogKTor->setDecimals(2);
        txRobotJogKTor->setMinimum(-999999.000000000000000);
        txRobotJogKTor->setMaximum(999999.989999999990687);
        txRobotJogKTor->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogKTor, 8, 1, 1, 1);

        txRobotJogKAcc = new QDoubleSpinBox(groupBox_27);
        txRobotJogKAcc->setObjectName(QString::fromUtf8("txRobotJogKAcc"));
        txRobotJogKAcc->setReadOnly(false);
        txRobotJogKAcc->setDecimals(2);
        txRobotJogKAcc->setMinimum(-999999.000000000000000);
        txRobotJogKAcc->setMaximum(999999.989999999990687);
        txRobotJogKAcc->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogKAcc, 5, 2, 1, 1);

        txRobotJogAng = new QDoubleSpinBox(groupBox_27);
        txRobotJogAng->setObjectName(QString::fromUtf8("txRobotJogAng"));
        txRobotJogAng->setReadOnly(false);
        txRobotJogAng->setDecimals(2);
        txRobotJogAng->setMinimum(-999999.000000000000000);
        txRobotJogAng->setMaximum(999999.989999999990687);
        txRobotJogAng->setValue(0.000000000000000);

        gridLayout_2->addWidget(txRobotJogAng, 3, 1, 1, 1);

        label_5 = new QLabel(groupBox_27);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 3, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_27, 4, 1, 2, 1);

        groupBox_24 = new QGroupBox(tab_2);
        groupBox_24->setObjectName(QString::fromUtf8("groupBox_24"));
        gridLayout_21 = new QGridLayout(groupBox_24);
        gridLayout_21->setSpacing(6);
        gridLayout_21->setContentsMargins(11, 11, 11, 11);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        txRobotPathJerk = new QDoubleSpinBox(groupBox_24);
        txRobotPathJerk->setObjectName(QString::fromUtf8("txRobotPathJerk"));
        txRobotPathJerk->setReadOnly(false);
        txRobotPathJerk->setDecimals(2);
        txRobotPathJerk->setMinimum(-999999.000000000000000);
        txRobotPathJerk->setMaximum(999999.989999999990687);
        txRobotPathJerk->setValue(0.000000000000000);

        gridLayout_21->addWidget(txRobotPathJerk, 3, 1, 1, 1);

        lbHomeStatus_87 = new QLabel(groupBox_24);
        lbHomeStatus_87->setObjectName(QString::fromUtf8("lbHomeStatus_87"));

        gridLayout_21->addWidget(lbHomeStatus_87, 5, 0, 1, 1);

        cbRobotOriMode = new QComboBox(groupBox_24);
        cbRobotOriMode->addItem(QString());
        cbRobotOriMode->addItem(QString());
        cbRobotOriMode->setObjectName(QString::fromUtf8("cbRobotOriMode"));

        gridLayout_21->addWidget(cbRobotOriMode, 6, 1, 1, 2);

        lbHomeStatus_13 = new QLabel(groupBox_24);
        lbHomeStatus_13->setObjectName(QString::fromUtf8("lbHomeStatus_13"));

        gridLayout_21->addWidget(lbHomeStatus_13, 2, 0, 1, 1);

        txRobotFBKAcc = new QDoubleSpinBox(groupBox_24);
        txRobotFBKAcc->setObjectName(QString::fromUtf8("txRobotFBKAcc"));
        txRobotFBKAcc->setReadOnly(false);
        txRobotFBKAcc->setDecimals(2);
        txRobotFBKAcc->setMinimum(-999999.000000000000000);
        txRobotFBKAcc->setMaximum(999999.989999999990687);
        txRobotFBKAcc->setValue(0.000000000000000);

        gridLayout_21->addWidget(txRobotFBKAcc, 1, 2, 1, 1);

        txRobotFBKVel = new QDoubleSpinBox(groupBox_24);
        txRobotFBKVel->setObjectName(QString::fromUtf8("txRobotFBKVel"));
        txRobotFBKVel->setReadOnly(false);
        txRobotFBKVel->setDecimals(2);
        txRobotFBKVel->setMinimum(-999999.000000000000000);
        txRobotFBKVel->setMaximum(999999.989999999990687);
        txRobotFBKVel->setValue(0.000000000000000);

        gridLayout_21->addWidget(txRobotFBKVel, 0, 2, 1, 1);

        txRobotPathAcc = new QDoubleSpinBox(groupBox_24);
        txRobotPathAcc->setObjectName(QString::fromUtf8("txRobotPathAcc"));
        txRobotPathAcc->setReadOnly(false);
        txRobotPathAcc->setDecimals(2);
        txRobotPathAcc->setMinimum(-999999.000000000000000);
        txRobotPathAcc->setMaximum(999999.989999999990687);
        txRobotPathAcc->setValue(0.000000000000000);

        gridLayout_21->addWidget(txRobotPathAcc, 1, 1, 1, 1);

        stktrans = new QStackedWidget(groupBox_24);
        stktrans->setObjectName(QString::fromUtf8("stktrans"));
        stktrans->setFrameShape(QFrame::StyledPanel);
        page_7 = new QWidget();
        page_7->setObjectName(QString::fromUtf8("page_7"));
        gridLayout_27 = new QGridLayout(page_7);
        gridLayout_27->setSpacing(6);
        gridLayout_27->setContentsMargins(11, 11, 11, 11);
        gridLayout_27->setObjectName(QString::fromUtf8("gridLayout_27"));
        txRobotTransVelFactor = new QDoubleSpinBox(page_7);
        txRobotTransVelFactor->setObjectName(QString::fromUtf8("txRobotTransVelFactor"));
        txRobotTransVelFactor->setReadOnly(false);
        txRobotTransVelFactor->setDecimals(2);
        txRobotTransVelFactor->setMinimum(-999999.000000000000000);
        txRobotTransVelFactor->setMaximum(999999.989999999990687);

        gridLayout_27->addWidget(txRobotTransVelFactor, 0, 1, 1, 1);

        lbHomeStatus_82 = new QLabel(page_7);
        lbHomeStatus_82->setObjectName(QString::fromUtf8("lbHomeStatus_82"));

        gridLayout_27->addWidget(lbHomeStatus_82, 0, 0, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_27->addItem(verticalSpacer_9, 2, 0, 1, 1);

        stktrans->addWidget(page_7);
        page_8 = new QWidget();
        page_8->setObjectName(QString::fromUtf8("page_8"));
        gridLayout_31 = new QGridLayout(page_8);
        gridLayout_31->setSpacing(6);
        gridLayout_31->setContentsMargins(11, 11, 11, 11);
        gridLayout_31->setObjectName(QString::fromUtf8("gridLayout_31"));
        txRobotTransCornerDist = new QDoubleSpinBox(page_8);
        txRobotTransCornerDist->setObjectName(QString::fromUtf8("txRobotTransCornerDist"));
        txRobotTransCornerDist->setReadOnly(false);
        txRobotTransCornerDist->setDecimals(2);
        txRobotTransCornerDist->setMinimum(-999999.000000000000000);
        txRobotTransCornerDist->setMaximum(999999.989999999990687);

        gridLayout_31->addWidget(txRobotTransCornerDist, 0, 1, 1, 1);

        lbHomeStatus_83 = new QLabel(page_8);
        lbHomeStatus_83->setObjectName(QString::fromUtf8("lbHomeStatus_83"));

        gridLayout_31->addWidget(lbHomeStatus_83, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_31->addItem(verticalSpacer_3, 1, 0, 1, 1);

        stktrans->addWidget(page_8);
        page_9 = new QWidget();
        page_9->setObjectName(QString::fromUtf8("page_9"));
        gridLayout_25 = new QGridLayout(page_9);
        gridLayout_25->setSpacing(6);
        gridLayout_25->setContentsMargins(11, 11, 11, 11);
        gridLayout_25->setObjectName(QString::fromUtf8("gridLayout_25"));
        lbHomeStatus_85 = new QLabel(page_9);
        lbHomeStatus_85->setObjectName(QString::fromUtf8("lbHomeStatus_85"));

        gridLayout_25->addWidget(lbHomeStatus_85, 1, 0, 1, 1);

        lbHomeStatus_84 = new QLabel(page_9);
        lbHomeStatus_84->setObjectName(QString::fromUtf8("lbHomeStatus_84"));

        gridLayout_25->addWidget(lbHomeStatus_84, 0, 0, 1, 1);

        txRobotTransLineDist = new QDoubleSpinBox(page_9);
        txRobotTransLineDist->setObjectName(QString::fromUtf8("txRobotTransLineDist"));
        txRobotTransLineDist->setReadOnly(false);
        txRobotTransLineDist->setDecimals(2);
        txRobotTransLineDist->setMinimum(-999999.000000000000000);
        txRobotTransLineDist->setMaximum(999999.989999999990687);

        gridLayout_25->addWidget(txRobotTransLineDist, 0, 1, 1, 1);

        txRobotTransAngDist = new QDoubleSpinBox(page_9);
        txRobotTransAngDist->setObjectName(QString::fromUtf8("txRobotTransAngDist"));
        txRobotTransAngDist->setReadOnly(false);
        txRobotTransAngDist->setDecimals(2);
        txRobotTransAngDist->setMinimum(-999999.000000000000000);
        txRobotTransAngDist->setMaximum(999999.989999999990687);

        gridLayout_25->addWidget(txRobotTransAngDist, 1, 1, 1, 1);

        ckRobotTransEucDone = new QCheckBox(page_9);
        ckRobotTransEucDone->setObjectName(QString::fromUtf8("ckRobotTransEucDone"));

        gridLayout_25->addWidget(ckRobotTransEucDone, 2, 0, 1, 2);

        stktrans->addWidget(page_9);

        gridLayout_21->addWidget(stktrans, 9, 0, 1, 3);

        txRobotPathVel = new QDoubleSpinBox(groupBox_24);
        txRobotPathVel->setObjectName(QString::fromUtf8("txRobotPathVel"));
        txRobotPathVel->setReadOnly(false);
        txRobotPathVel->setDecimals(2);
        txRobotPathVel->setMinimum(-999999.000000000000000);
        txRobotPathVel->setMaximum(999999.989999999990687);
        txRobotPathVel->setValue(0.000000000000000);

        gridLayout_21->addWidget(txRobotPathVel, 0, 1, 1, 1);

        lbHomeStatus_88 = new QLabel(groupBox_24);
        lbHomeStatus_88->setObjectName(QString::fromUtf8("lbHomeStatus_88"));

        gridLayout_21->addWidget(lbHomeStatus_88, 7, 0, 1, 1);

        cbRobotTransMode = new QComboBox(groupBox_24);
        cbRobotTransMode->addItem(QString());
        cbRobotTransMode->addItem(QString());
        cbRobotTransMode->addItem(QString());
        cbRobotTransMode->addItem(QString());
        cbRobotTransMode->setObjectName(QString::fromUtf8("cbRobotTransMode"));

        gridLayout_21->addWidget(cbRobotTransMode, 7, 1, 1, 1);

        lbHomeStatus_14 = new QLabel(groupBox_24);
        lbHomeStatus_14->setObjectName(QString::fromUtf8("lbHomeStatus_14"));

        gridLayout_21->addWidget(lbHomeStatus_14, 3, 0, 1, 1);

        cbRobotBufferMode = new QComboBox(groupBox_24);
        cbRobotBufferMode->addItem(QString());
        cbRobotBufferMode->addItem(QString());
        cbRobotBufferMode->addItem(QString());
        cbRobotBufferMode->addItem(QString());
        cbRobotBufferMode->addItem(QString());
        cbRobotBufferMode->addItem(QString());
        cbRobotBufferMode->setObjectName(QString::fromUtf8("cbRobotBufferMode"));

        gridLayout_21->addWidget(cbRobotBufferMode, 5, 1, 1, 2);

        txRobotFBKTor = new QDoubleSpinBox(groupBox_24);
        txRobotFBKTor->setObjectName(QString::fromUtf8("txRobotFBKTor"));
        txRobotFBKTor->setReadOnly(false);
        txRobotFBKTor->setDecimals(2);
        txRobotFBKTor->setMinimum(-999999.000000000000000);
        txRobotFBKTor->setMaximum(999999.989999999990687);
        txRobotFBKTor->setValue(0.000000000000000);

        gridLayout_21->addWidget(txRobotFBKTor, 4, 2, 1, 1);

        lbHomeStatus_12 = new QLabel(groupBox_24);
        lbHomeStatus_12->setObjectName(QString::fromUtf8("lbHomeStatus_12"));

        gridLayout_21->addWidget(lbHomeStatus_12, 1, 0, 1, 1);

        txRobotPathDec = new QDoubleSpinBox(groupBox_24);
        txRobotPathDec->setObjectName(QString::fromUtf8("txRobotPathDec"));
        txRobotPathDec->setReadOnly(false);
        txRobotPathDec->setDecimals(2);
        txRobotPathDec->setMinimum(-999999.000000000000000);
        txRobotPathDec->setMaximum(999999.989999999990687);
        txRobotPathDec->setValue(0.000000000000000);

        gridLayout_21->addWidget(txRobotPathDec, 2, 1, 1, 1);

        ckRobotTransBlendAux = new QCheckBox(groupBox_24);
        ckRobotTransBlendAux->setObjectName(QString::fromUtf8("ckRobotTransBlendAux"));
        ckRobotTransBlendAux->setEnabled(true);

        gridLayout_21->addWidget(ckRobotTransBlendAux, 7, 2, 1, 1);

        lbHomeStatus_89 = new QLabel(groupBox_24);
        lbHomeStatus_89->setObjectName(QString::fromUtf8("lbHomeStatus_89"));

        gridLayout_21->addWidget(lbHomeStatus_89, 6, 0, 1, 1);

        txRobotFBKJerk = new QDoubleSpinBox(groupBox_24);
        txRobotFBKJerk->setObjectName(QString::fromUtf8("txRobotFBKJerk"));
        txRobotFBKJerk->setReadOnly(false);
        txRobotFBKJerk->setDecimals(2);
        txRobotFBKJerk->setMinimum(-999999.000000000000000);
        txRobotFBKJerk->setMaximum(999999.989999999990687);
        txRobotFBKJerk->setValue(0.000000000000000);

        gridLayout_21->addWidget(txRobotFBKJerk, 3, 2, 1, 1);

        lbHomeStatus_17 = new QLabel(groupBox_24);
        lbHomeStatus_17->setObjectName(QString::fromUtf8("lbHomeStatus_17"));

        gridLayout_21->addWidget(lbHomeStatus_17, 4, 0, 1, 1);

        lbHomeStatus_11 = new QLabel(groupBox_24);
        lbHomeStatus_11->setObjectName(QString::fromUtf8("lbHomeStatus_11"));

        gridLayout_21->addWidget(lbHomeStatus_11, 0, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_24, 1, 0, 1, 1);

        groupBox_28 = new QGroupBox(tab_2);
        groupBox_28->setObjectName(QString::fromUtf8("groupBox_28"));
        gridLayout_3 = new QGridLayout(groupBox_28);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        lbHomeStatus_93 = new QLabel(groupBox_28);
        lbHomeStatus_93->setObjectName(QString::fromUtf8("lbHomeStatus_93"));

        gridLayout_3->addWidget(lbHomeStatus_93, 0, 0, 1, 1);

        btRobotEStopDec = new QDoubleSpinBox(groupBox_28);
        btRobotEStopDec->setObjectName(QString::fromUtf8("btRobotEStopDec"));
        btRobotEStopDec->setReadOnly(false);
        btRobotEStopDec->setDecimals(2);
        btRobotEStopDec->setMinimum(-999999.000000000000000);
        btRobotEStopDec->setMaximum(999999.989999999990687);
        btRobotEStopDec->setValue(0.000000000000000);

        gridLayout_3->addWidget(btRobotEStopDec, 0, 1, 1, 1);

        btRobotEStopKAcc = new QDoubleSpinBox(groupBox_28);
        btRobotEStopKAcc->setObjectName(QString::fromUtf8("btRobotEStopKAcc"));
        btRobotEStopKAcc->setReadOnly(false);
        btRobotEStopKAcc->setDecimals(2);
        btRobotEStopKAcc->setMinimum(-999999.000000000000000);
        btRobotEStopKAcc->setMaximum(999999.989999999990687);
        btRobotEStopKAcc->setValue(0.000000000000000);

        gridLayout_3->addWidget(btRobotEStopKAcc, 0, 2, 1, 1);

        lbHomeStatus_94 = new QLabel(groupBox_28);
        lbHomeStatus_94->setObjectName(QString::fromUtf8("lbHomeStatus_94"));

        gridLayout_3->addWidget(lbHomeStatus_94, 1, 0, 1, 1);

        btRobotEStopJerk = new QDoubleSpinBox(groupBox_28);
        btRobotEStopJerk->setObjectName(QString::fromUtf8("btRobotEStopJerk"));
        btRobotEStopJerk->setReadOnly(false);
        btRobotEStopJerk->setDecimals(2);
        btRobotEStopJerk->setMinimum(-999999.000000000000000);
        btRobotEStopJerk->setMaximum(999999.989999999990687);
        btRobotEStopJerk->setValue(0.000000000000000);

        gridLayout_3->addWidget(btRobotEStopJerk, 1, 1, 1, 1);

        btRobotEStopKJerk = new QDoubleSpinBox(groupBox_28);
        btRobotEStopKJerk->setObjectName(QString::fromUtf8("btRobotEStopKJerk"));
        btRobotEStopKJerk->setReadOnly(false);
        btRobotEStopKJerk->setDecimals(2);
        btRobotEStopKJerk->setMinimum(-999999.000000000000000);
        btRobotEStopKJerk->setMaximum(999999.989999999990687);
        btRobotEStopKJerk->setValue(0.000000000000000);

        gridLayout_3->addWidget(btRobotEStopKJerk, 1, 2, 1, 1);

        lbHomeStatus_107 = new QLabel(groupBox_28);
        lbHomeStatus_107->setObjectName(QString::fromUtf8("lbHomeStatus_107"));

        gridLayout_3->addWidget(lbHomeStatus_107, 2, 0, 1, 1);

        btRobotEStopKTor = new QDoubleSpinBox(groupBox_28);
        btRobotEStopKTor->setObjectName(QString::fromUtf8("btRobotEStopKTor"));
        btRobotEStopKTor->setReadOnly(false);
        btRobotEStopKTor->setDecimals(2);
        btRobotEStopKTor->setMinimum(-999999.000000000000000);
        btRobotEStopKTor->setMaximum(999999.989999999990687);
        btRobotEStopKTor->setValue(0.000000000000000);

        gridLayout_3->addWidget(btRobotEStopKTor, 2, 2, 1, 1);

        lbHomeStatus_95 = new QLabel(groupBox_28);
        lbHomeStatus_95->setObjectName(QString::fromUtf8("lbHomeStatus_95"));

        gridLayout_3->addWidget(lbHomeStatus_95, 3, 0, 1, 1);

        btRobotEStopModule = new QSpinBox(groupBox_28);
        btRobotEStopModule->setObjectName(QString::fromUtf8("btRobotEStopModule"));
        btRobotEStopModule->setMinimum(-99);

        gridLayout_3->addWidget(btRobotEStopModule, 3, 1, 1, 1);

        btRobotEStopIO = new QSpinBox(groupBox_28);
        btRobotEStopIO->setObjectName(QString::fromUtf8("btRobotEStopIO"));
        btRobotEStopIO->setMinimum(-99);

        gridLayout_3->addWidget(btRobotEStopIO, 3, 2, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_7, 4, 0, 1, 1);

        cbRobotHaltClear = new QCheckBox(groupBox_28);
        cbRobotHaltClear->setObjectName(QString::fromUtf8("cbRobotHaltClear"));

        gridLayout_3->addWidget(cbRobotHaltClear, 5, 0, 1, 2);

        cbRobotEStopClear = new QCheckBox(groupBox_28);
        cbRobotEStopClear->setObjectName(QString::fromUtf8("cbRobotEStopClear"));

        gridLayout_3->addWidget(cbRobotEStopClear, 6, 0, 1, 2);

        btRobotEStop = new QPushButton(groupBox_28);
        btRobotEStop->setObjectName(QString::fromUtf8("btRobotEStop"));
        btRobotEStop->setCheckable(true);

        gridLayout_3->addWidget(btRobotEStop, 6, 2, 1, 1);


        gridLayout_12->addWidget(groupBox_28, 5, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer_4, 6, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 633, 55));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_9);

        txAcpTagName = new QLineEdit(groupBox);
        txAcpTagName->setObjectName(QString::fromUtf8("txAcpTagName"));
        sizePolicy.setHeightForWidth(txAcpTagName->sizePolicy().hasHeightForWidth());
        txAcpTagName->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(txAcpTagName);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        sizePolicy.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_13);

        txAcpTagSize = new QSpinBox(groupBox);
        txAcpTagSize->setObjectName(QString::fromUtf8("txAcpTagSize"));
        txAcpTagSize->setMinimum(-99);

        horizontalLayout->addWidget(txAcpTagSize);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_10);

        txAcpTagValue = new QDoubleSpinBox(groupBox);
        txAcpTagValue->setObjectName(QString::fromUtf8("txAcpTagValue"));
        txAcpTagValue->setReadOnly(false);
        txAcpTagValue->setDecimals(2);
        txAcpTagValue->setMinimum(-999999.000000000000000);
        txAcpTagValue->setMaximum(999999.989999999990687);
        txAcpTagValue->setValue(0.000000000000000);

        horizontalLayout->addWidget(txAcpTagValue);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btAcpRead = new QPushButton(groupBox);
        btAcpRead->setObjectName(QString::fromUtf8("btAcpRead"));

        horizontalLayout->addWidget(btAcpRead);

        btAcpWrite = new QPushButton(groupBox);
        btAcpWrite->setObjectName(QString::fromUtf8("btAcpWrite"));

        horizontalLayout->addWidget(btAcpWrite);

        btTest = new QPushButton(tab);
        btTest->setObjectName(QString::fromUtf8("btTest"));
        btTest->setGeometry(QRect(80, 140, 75, 23));
        tabWidget->addTab(tab, QString());

        gridLayout_4->addWidget(tabWidget, 1, 0, 1, 1);


        retranslateUi(ScaraRobotClass);
        QObject::connect(rbMidPos, SIGNAL(toggled(bool)), lbCircDir, SLOT(setVisible(bool)));
        QObject::connect(rbMidPos, SIGNAL(toggled(bool)), cbCircDir, SLOT(setVisible(bool)));
        QObject::connect(rbMidPos, SIGNAL(toggled(bool)), lbCircPath, SLOT(setVisible(bool)));
        QObject::connect(rbMidPos, SIGNAL(toggled(bool)), cbCircPath, SLOT(setVisible(bool)));

        tabWidget->setCurrentIndex(0);
        stkpos->setCurrentIndex(0);
        stktrans->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ScaraRobotClass);
    } // setupUi

    void retranslateUi(QWidget *ScaraRobotClass)
    {
        ScaraRobotClass->setWindowTitle(QApplication::translate("ScaraRobotClass", "ScaraRobot", nullptr));
        groupBox_11->setTitle(QApplication::translate("ScaraRobotClass", "ACP \351\200\232\350\256\257", nullptr));
        label_7->setText(QApplication::translate("ScaraRobotClass", "\347\212\266\346\200\201:", nullptr));
        btAcpConnect->setText(QApplication::translate("ScaraRobotClass", "\350\277\236\346\216\245", nullptr));
        label_6->setText(QApplication::translate("ScaraRobotClass", "IP:", nullptr));
        txAcpIp->setText(QApplication::translate("ScaraRobotClass", "192.168.1.200", nullptr));
        btAcpDisconnect->setText(QApplication::translate("ScaraRobotClass", "\346\226\255\345\274\200", nullptr));
        label_14->setText(QApplication::translate("ScaraRobotClass", "\351\200\232\350\256\257", nullptr));
        cbAcpMode->setItemText(0, QApplication::translate("ScaraRobotClass", "TCP", nullptr));
        cbAcpMode->setItemText(1, QApplication::translate("ScaraRobotClass", "SHM", nullptr));

        groupBox_22->setTitle(QApplication::translate("ScaraRobotClass", "XYZ\345\271\263\345\217\260", nullptr));
        groupBox_25->setTitle(QApplication::translate("ScaraRobotClass", "\350\277\220\345\212\250\345\217\202\346\225\260", nullptr));
        lbHomeStatus_109->setText(QApplication::translate("ScaraRobotClass", "\351\200\237\345\272\246", nullptr));
        lbHomeStatus_100->setText(QApplication::translate("ScaraRobotClass", "\345\212\240\351\200\237\345\272\246", nullptr));
        lbHomeStatus_104->setText(QApplication::translate("ScaraRobotClass", "\345\207\217\351\200\237\345\272\246", nullptr));
        lbHomeStatus_108->setText(QApplication::translate("ScaraRobotClass", "\350\267\203\345\272\246", nullptr));
        groupBox_23->setTitle(QApplication::translate("ScaraRobotClass", "\347\233\256\346\240\207\344\275\215\347\275\256", nullptr));
        lbHomeStatus_81->setText(QApplication::translate("ScaraRobotClass", "X", nullptr));
        lbHomeStatus_106->setText(QApplication::translate("ScaraRobotClass", "Y", nullptr));
        lbHomeStatus_101->setText(QApplication::translate("ScaraRobotClass", "Z", nullptr));
        lbHomeStatus_99->setText(QApplication::translate("ScaraRobotClass", "C", nullptr));
        groupBox_26->setTitle(QApplication::translate("ScaraRobotClass", "\345\267\245\345\205\267\345\201\217\347\247\273", nullptr));
        lbHomeStatus_110->setText(QApplication::translate("ScaraRobotClass", "Y", nullptr));
        btIntpPowerOn->setText(QApplication::translate("ScaraRobotClass", "\345\274\200\344\275\277\350\203\275", nullptr));
        lbHomeStatus_111->setText(QApplication::translate("ScaraRobotClass", "X", nullptr));
        groupBox_30->setTitle(QApplication::translate("ScaraRobotClass", "\346\223\215\344\275\234", nullptr));
        btIntpMvAbs->setText(QApplication::translate("ScaraRobotClass", "\347\273\235\345\257\271\345\256\232\344\275\215", nullptr));
        btIntpMvRel->setText(QApplication::translate("ScaraRobotClass", "\347\233\270\345\257\271\345\256\232\344\275\215", nullptr));
        btIntpMvHalt->setText(QApplication::translate("ScaraRobotClass", "\344\270\255\346\255\242", nullptr));
        btIntpMvReset->setText(QApplication::translate("ScaraRobotClass", "\345\244\215\344\275\215", nullptr));
        groupBox_29->setTitle(QApplication::translate("ScaraRobotClass", "\346\250\241\345\235\227\347\212\266\346\200\201", nullptr));
        ckIntpPowerOn->setText(QApplication::translate("ScaraRobotClass", "\345\267\262\344\275\277\350\203\275", nullptr));
        ckIntpReady->setText(QApplication::translate("ScaraRobotClass", "\345\207\206\345\244\207\345\245\275", nullptr));
        ckIntpDone->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\346\211\247\350\241\214\345\256\214\346\210\220", nullptr));
        ckIntpBusy->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\346\211\247\350\241\214\344\270\255", nullptr));
        ckIntpError->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\346\211\247\350\241\214\345\244\261\350\264\245", nullptr));
        groupBox_20->setTitle(QApplication::translate("ScaraRobotClass", "\346\216\247\345\210\266\345\231\250\345\233\236\351\233\266", nullptr));
        ckHomeRetrunZero->setText(QApplication::translate("ScaraRobotClass", "\345\256\214\346\210\220\345\220\216\347\247\273\345\212\250\345\210\260\351\233\266\344\275\215", nullptr));
        lbHomeStatus_60->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266\346\250\241\345\274\217", nullptr));
        ckSmcHomeIO->setText(QApplication::translate("ScaraRobotClass", "\345\216\237\347\202\271\345\274\200\345\205\263", nullptr));
        lbHomeStatus_58->setText(QApplication::translate("ScaraRobotClass", "\345\216\237\347\202\271\347\264\242\345\274\225\350\276\223\345\205\245\347\202\271", nullptr));
        lbHomeStatus_56->setText(QApplication::translate("ScaraRobotClass", "\351\233\266\344\275\215\350\204\211\345\206\262\344\275\215\347\275\256", nullptr));
        lbHomeStatus_53->setText(QApplication::translate("ScaraRobotClass", "\345\216\237\347\202\271\345\274\200\345\205\263\345\273\266\346\227\266(s)", nullptr));
        lbHomeStatus_57->setText(QApplication::translate("ScaraRobotClass", "\345\216\237\347\202\271\345\274\200\345\205\263\350\276\223\345\205\245\347\202\271", nullptr));
        ckSmcHomeIndex->setText(QApplication::translate("ScaraRobotClass", "\345\216\237\347\202\271\347\264\242\345\274\225", nullptr));
        ckIgnoreHw->setText(QApplication::translate("ScaraRobotClass", "\345\277\275\347\225\245\350\275\257\351\231\220\344\275\215", nullptr));
        btPlcHome->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266", nullptr));
        cbAxisSmcHomeMode->setItemText(0, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_NEG_BSLOW_S_STOP", nullptr));
        cbAxisSmcHomeMode->setItemText(1, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_NEG_BSLOW_STOP_S", nullptr));
        cbAxisSmcHomeMode->setItemText(2, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_NEG_BSLOW_I_S_STOP", nullptr));
        cbAxisSmcHomeMode->setItemText(3, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_NEG_SLOW_S_STOP", nullptr));
        cbAxisSmcHomeMode->setItemText(4, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_NEG_SLOW_STOP_S", nullptr));
        cbAxisSmcHomeMode->setItemText(5, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_NEG_SLOW_I_S_STOP", nullptr));
        cbAxisSmcHomeMode->setItemText(6, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_POS_BSLOW_S_STOP", nullptr));
        cbAxisSmcHomeMode->setItemText(7, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_POS_BSLOW_STOP_S", nullptr));
        cbAxisSmcHomeMode->setItemText(8, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_POS_BSLOW_I_S_STOP", nullptr));
        cbAxisSmcHomeMode->setItemText(9, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_POS_SLOW_S_STOP", nullptr));
        cbAxisSmcHomeMode->setItemText(10, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_POS_SLOW_STOP_S", nullptr));
        cbAxisSmcHomeMode->setItemText(11, QApplication::translate("ScaraRobotClass", "SMC_HOME_FAST_POS_SLOW_I_S_STOP", nullptr));

        groupBox_15->setTitle(QApplication::translate("ScaraRobotClass", "\346\200\245\345\201\234\345\217\202\346\225\260", nullptr));
        lbHomeStatus_41->setText(QApplication::translate("ScaraRobotClass", "\345\207\217\351\200\237\345\272\246", nullptr));
        lbHomeStatus_42->setText(QApplication::translate("ScaraRobotClass", "\350\267\203\345\272\246", nullptr));
        lbHomeStatus_43->setText(QApplication::translate("ScaraRobotClass", "\346\200\245\345\201\234\350\276\223\345\205\245\347\202\271", nullptr));
        btAxisEStop->setText(QApplication::translate("ScaraRobotClass", "\346\200\245\345\201\234", nullptr));
        groupBox_14->setTitle(QApplication::translate("ScaraRobotClass", "\350\275\264\351\200\211\346\213\251", nullptr));
        lbHomeStatus_24->setText(QApplication::translate("ScaraRobotClass", "\350\275\264\345\217\267", nullptr));
        cbAxisSel->setItemText(0, QApplication::translate("ScaraRobotClass", "0", nullptr));
        cbAxisSel->setItemText(1, QApplication::translate("ScaraRobotClass", "1", nullptr));
        cbAxisSel->setItemText(2, QApplication::translate("ScaraRobotClass", "2", nullptr));
        cbAxisSel->setItemText(3, QApplication::translate("ScaraRobotClass", "3", nullptr));
        cbAxisSel->setItemText(4, QApplication::translate("ScaraRobotClass", "4", nullptr));
        cbAxisSel->setItemText(5, QApplication::translate("ScaraRobotClass", "5", nullptr));
        cbAxisSel->setItemText(6, QApplication::translate("ScaraRobotClass", "6", nullptr));
        cbAxisSel->setItemText(7, QApplication::translate("ScaraRobotClass", "7", nullptr));
        cbAxisSel->setItemText(8, QApplication::translate("ScaraRobotClass", "8", nullptr));
        cbAxisSel->setItemText(9, QApplication::translate("ScaraRobotClass", "9", nullptr));
        cbAxisSel->setItemText(10, QApplication::translate("ScaraRobotClass", "10", nullptr));
        cbAxisSel->setItemText(11, QApplication::translate("ScaraRobotClass", "11", nullptr));
        cbAxisSel->setItemText(12, QApplication::translate("ScaraRobotClass", "12", nullptr));
        cbAxisSel->setItemText(13, QApplication::translate("ScaraRobotClass", "13", nullptr));
        cbAxisSel->setItemText(14, QApplication::translate("ScaraRobotClass", "14", nullptr));
        cbAxisSel->setItemText(15, QApplication::translate("ScaraRobotClass", "15", nullptr));
        cbAxisSel->setItemText(16, QApplication::translate("ScaraRobotClass", "16", nullptr));
        cbAxisSel->setItemText(17, QApplication::translate("ScaraRobotClass", "17", nullptr));
        cbAxisSel->setItemText(18, QApplication::translate("ScaraRobotClass", "18", nullptr));
        cbAxisSel->setItemText(19, QApplication::translate("ScaraRobotClass", "19", nullptr));
        cbAxisSel->setItemText(20, QApplication::translate("ScaraRobotClass", "20", nullptr));
        cbAxisSel->setItemText(21, QApplication::translate("ScaraRobotClass", "21", nullptr));
        cbAxisSel->setItemText(22, QApplication::translate("ScaraRobotClass", "22", nullptr));
        cbAxisSel->setItemText(23, QApplication::translate("ScaraRobotClass", "23", nullptr));
        cbAxisSel->setItemText(24, QApplication::translate("ScaraRobotClass", "24", nullptr));
        cbAxisSel->setItemText(25, QApplication::translate("ScaraRobotClass", "25", nullptr));
        cbAxisSel->setItemText(26, QApplication::translate("ScaraRobotClass", "26", nullptr));
        cbAxisSel->setItemText(27, QApplication::translate("ScaraRobotClass", "27", nullptr));
        cbAxisSel->setItemText(28, QApplication::translate("ScaraRobotClass", "28", nullptr));
        cbAxisSel->setItemText(29, QApplication::translate("ScaraRobotClass", "29", nullptr));
        cbAxisSel->setItemText(30, QApplication::translate("ScaraRobotClass", "30", nullptr));
        cbAxisSel->setItemText(31, QApplication::translate("ScaraRobotClass", "31", nullptr));
        cbAxisSel->setItemText(32, QApplication::translate("ScaraRobotClass", "32", nullptr));

        lbHomeStatus_35->setText(QApplication::translate("ScaraRobotClass", "\345\275\223\345\211\215\344\275\215\347\275\256", nullptr));
        lbHomeStatus_34->setText(QApplication::translate("ScaraRobotClass", "\345\275\223\345\211\215\351\200\237\345\272\246", nullptr));
        btAxisPowerOn->setText(QApplication::translate("ScaraRobotClass", "\345\274\200\344\275\277\350\203\275", nullptr));
        groupBox_18->setTitle(QApplication::translate("ScaraRobotClass", "SDO\350\257\273\345\206\231", nullptr));
        lbHomeStatus_59->setText(QApplication::translate("ScaraRobotClass", "SDO \345\255\220\347\264\242\345\274\225", nullptr));
        label_12->setText(QApplication::translate("ScaraRobotClass", "\345\217\230\351\207\217\345\200\274(\345\206\231)", nullptr));
        btSdoWrite->setText(QApplication::translate("ScaraRobotClass", "\345\206\231\345\205\245", nullptr));
        label_11->setText(QApplication::translate("ScaraRobotClass", "\345\217\230\351\207\217\345\200\274(\350\257\273)", nullptr));
        txSdoSubIndex->setPrefix(QApplication::translate("ScaraRobotClass", "16#", nullptr));
        txSdoIndex->setSuffix(QString());
        txSdoIndex->setPrefix(QApplication::translate("ScaraRobotClass", "16#", nullptr));
        lbHomeStatus_55->setText(QApplication::translate("ScaraRobotClass", "SDO \344\270\273\347\264\242\345\274\225", nullptr));
        btSdoRead->setText(QApplication::translate("ScaraRobotClass", "\350\257\273\345\217\226", nullptr));
        label_15->setText(QApplication::translate("ScaraRobotClass", "\346\225\260\346\215\256\351\225\277\345\272\246(\345\255\227\350\212\202)", nullptr));
        txSdoDataLen->setPrefix(QApplication::translate("ScaraRobotClass", "16#", nullptr));
        groupBox_12->setTitle(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266\345\217\202\346\225\260", nullptr));
        lbHomeStatus_45->setText(QApplication::translate("ScaraRobotClass", "\345\216\237\347\202\271\344\275\215\347\275\256", nullptr));
        lbHomeStatus_47->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266\351\253\230\351\200\237", nullptr));
        lbHomeStatus_51->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266\344\275\216\351\200\237", nullptr));
        lbHomeStatus_48->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266\345\212\240\351\200\237\345\272\246", nullptr));
        lbHomeStatus_46->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266\345\207\217\351\200\237\345\272\246", nullptr));
        lbHomeStatus_49->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266\350\267\203\345\272\246", nullptr));
        btAxisHomeParam->setText(QApplication::translate("ScaraRobotClass", "\350\256\276\345\256\232", nullptr));
        ckAxisHomeEnConv->setText(QApplication::translate("ScaraRobotClass", "\345\220\257\347\224\250\347\224\250\346\210\267\345\215\225\344\275\215\350\275\254\346\215\242", nullptr));
        groupBox_16->setTitle(QApplication::translate("ScaraRobotClass", "\347\202\271\345\212\250\345\217\202\346\225\260", nullptr));
        lbHomeStatus_29->setText(QApplication::translate("ScaraRobotClass", "\350\267\203\345\272\246", nullptr));
        lbHomeStatus_38->setText(QApplication::translate("ScaraRobotClass", "\345\212\240\351\200\237\345\272\246", nullptr));
        lbHomeStatus_36->setText(QApplication::translate("ScaraRobotClass", "\351\200\237\345\272\246", nullptr));
        lbHomeStatus_37->setText(QApplication::translate("ScaraRobotClass", "\345\207\217\351\200\237\345\272\246", nullptr));
        lbHomeStatus_61->setText(QApplication::translate("ScaraRobotClass", "\345\257\270\345\212\250\350\267\235\347\246\273", nullptr));
        btAxisJogFwd->setText(QApplication::translate("ScaraRobotClass", "\347\202\271\345\212\250\345\211\215\350\277\233", nullptr));
        btAxisJogBwd->setText(QApplication::translate("ScaraRobotClass", "\347\202\271\345\212\250\345\220\216\351\200\200", nullptr));
        cbAxisInch->setText(QApplication::translate("ScaraRobotClass", "\345\257\270\345\212\250", nullptr));
        groupBox_9->setTitle(QApplication::translate("ScaraRobotClass", "\350\277\220\345\212\250\345\217\202\346\225\260", nullptr));
        lbHomeStatus_33->setText(QApplication::translate("ScaraRobotClass", "\347\233\256\346\240\207\344\275\215\347\275\256", nullptr));
        btAxisMoveAbs->setText(QApplication::translate("ScaraRobotClass", "\347\273\235\345\257\271\345\256\232\344\275\215", nullptr));
        lbHomeStatus_31->setText(QApplication::translate("ScaraRobotClass", "\351\200\237\345\272\246", nullptr));
        btAxisMoveRel->setText(QApplication::translate("ScaraRobotClass", "\347\233\270\345\257\271\345\256\232\344\275\215", nullptr));
        lbHomeStatus_32->setText(QApplication::translate("ScaraRobotClass", "\345\212\240\351\200\237\345\272\246", nullptr));
        btAxisMoveVel->setText(QApplication::translate("ScaraRobotClass", "\351\200\237\345\272\246\350\277\220\350\241\214", nullptr));
        lbHomeStatus_30->setText(QApplication::translate("ScaraRobotClass", "\345\207\217\351\200\237\345\272\246", nullptr));
        btAxisMoveHalt->setText(QApplication::translate("ScaraRobotClass", "\344\270\255\346\255\242", nullptr));
        lbHomeStatus_26->setText(QApplication::translate("ScaraRobotClass", "\350\267\203\345\272\246", nullptr));
        lbHomeStatus_27->setText(QApplication::translate("ScaraRobotClass", "\347\274\223\345\206\262\346\250\241\345\274\217", nullptr));
        cbAxisBufferMode->setItemText(0, QApplication::translate("ScaraRobotClass", "Aborting", nullptr));
        cbAxisBufferMode->setItemText(1, QApplication::translate("ScaraRobotClass", "Buffered", nullptr));
        cbAxisBufferMode->setItemText(2, QApplication::translate("ScaraRobotClass", "BlendingLow", nullptr));
        cbAxisBufferMode->setItemText(3, QApplication::translate("ScaraRobotClass", "BlendingPrevious", nullptr));
        cbAxisBufferMode->setItemText(4, QApplication::translate("ScaraRobotClass", "BlendingNext", nullptr));
        cbAxisBufferMode->setItemText(5, QApplication::translate("ScaraRobotClass", "BlendingHigh", nullptr));

        btAxisSetPos->setText(QApplication::translate("ScaraRobotClass", "\344\275\215\347\275\256\350\256\276\345\256\232", nullptr));
        btAxisReset->setText(QApplication::translate("ScaraRobotClass", "\345\244\215\344\275\215", nullptr));
        groupBox_13->setTitle(QApplication::translate("ScaraRobotClass", "\350\275\264\347\212\266\346\200\201", nullptr));
        ckVirtual->setText(QApplication::translate("ScaraRobotClass", "\350\231\232\350\275\264", nullptr));
        ckCommStatus->setText(QApplication::translate("ScaraRobotClass", "\351\200\232\350\256\257\346\214\207\347\244\272", nullptr));
        ckRefStatus->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\345\216\237\347\202\271\345\256\214\346\210\220", nullptr));
        ckPowerStatus->setText(QApplication::translate("ScaraRobotClass", "\344\275\277\350\203\275\347\212\266\346\200\201", nullptr));
        ckStoppedStatus->setText(QApplication::translate("ScaraRobotClass", "\350\275\264\345\201\234\346\255\242\347\212\266\346\200\201", nullptr));
        ckHomingStatus->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\345\216\237\347\202\271\344\270\255", nullptr));
        ckSyncStatus->setText(QApplication::translate("ScaraRobotClass", "\350\275\264\345\220\214\346\255\245\350\277\220\350\241\214\347\212\266\346\200\201", nullptr));
        ckDone->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\346\211\247\350\241\214\345\256\214\346\210\220", nullptr));
        ckBusy->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\346\211\247\350\241\214\344\270\255", nullptr));
        ckError->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\346\211\247\350\241\214\345\244\261\350\264\245", nullptr));
        ckAlmStatus->setText(QApplication::translate("ScaraRobotClass", "\350\275\264\346\212\245\350\255\246", nullptr));
        ckDINegativeLimit->setText(QApplication::translate("ScaraRobotClass", "DI - \350\264\237\351\231\220\344\275\215", nullptr));
        ckDIPositiveLimit->setText(QApplication::translate("ScaraRobotClass", "DI - \346\255\243\351\231\220\344\275\215", nullptr));
        ckDIHomeSwitch->setText(QApplication::translate("ScaraRobotClass", "DI - \345\216\237\347\202\271", nullptr));
        ckDIQuickStop->setText(QApplication::translate("ScaraRobotClass", "DI - \346\200\245\345\201\234", nullptr));
        ckDIVoltageOk->setText(QApplication::translate("ScaraRobotClass", "DI - \344\275\277\350\203\275\347\224\265\345\216\213", nullptr));
        ckDIBrakeReleased->setText(QApplication::translate("ScaraRobotClass", "DI - \346\212\261\351\227\270\345\217\215\351\246\210", nullptr));
        ckDIWarning->setText(QApplication::translate("ScaraRobotClass", "DI - \350\255\246\345\221\212", nullptr));
        ckDIReady->setText(QApplication::translate("ScaraRobotClass", "DI - \351\251\261\345\212\250\345\231\250\345\260\261\347\273\252", nullptr));
        ckDIFault->setText(QApplication::translate("ScaraRobotClass", "DI - \351\251\261\345\212\250\345\231\250\346\225\205\351\232\234", nullptr));
        ckModulo->setText(QApplication::translate("ScaraRobotClass", "\346\250\241\346\225\260\350\275\264", nullptr));
        ckRotary->setText(QApplication::translate("ScaraRobotClass", "\346\227\213\350\275\254\350\275\264", nullptr));
        lbHomeStatus_80->setText(QApplication::translate("ScaraRobotClass", "\346\250\241\346\225\260", nullptr));
        lbHomeStatus_76->setText(QApplication::translate("ScaraRobotClass", "\346\234\200\345\244\247\351\200\237\345\272\246", nullptr));
        lbHomeStatus_77->setText(QApplication::translate("ScaraRobotClass", "\346\234\200\345\244\247\345\212\240\351\200\237\345\272\246", nullptr));
        lbHomeStatus_78->setText(QApplication::translate("ScaraRobotClass", "\346\234\200\345\244\247\345\207\217\351\200\237\345\272\246", nullptr));
        lbHomeStatus_79->setText(QApplication::translate("ScaraRobotClass", "\346\234\200\345\244\247\350\267\203\345\272\246", nullptr));
        lbHomeStatus_44->setText(QApplication::translate("ScaraRobotClass", "\345\275\223\345\211\215\346\250\241\345\274\217", nullptr));
        lbHomeStatus_39->setText(QApplication::translate("ScaraRobotClass", "\351\224\231\350\257\257\344\273\243\347\240\201", nullptr));
        lbHomeStatus_40->setText(QApplication::translate("ScaraRobotClass", "\350\275\264\346\212\245\350\255\246ID", nullptr));
        lbHomeStatus_25->setText(QApplication::translate("ScaraRobotClass", "\350\275\264\350\277\220\345\212\250\347\212\266\346\200\201", nullptr));
        lbHomeStatus_74->setText(QApplication::translate("ScaraRobotClass", "ID", nullptr));
        groupBox_19->setTitle(QApplication::translate("ScaraRobotClass", "\351\251\261\345\212\250\345\231\250\345\233\236\351\233\266", nullptr));
        lbHomeStatus_52->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266\345\201\217\347\247\273(SDO)", nullptr));
        lbHomeStatus_54->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266\346\250\241\345\274\217", nullptr));
        ckAxisHomeEnSdo->setText(QApplication::translate("ScaraRobotClass", "SDO\345\206\231\345\212\240\345\207\217\351\200\237/\345\201\217\347\247\273", nullptr));
        btDrvHome->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("ScaraRobotClass", "\345\215\225\350\275\264\350\260\203\350\257\225", nullptr));
        groupBox_2->setTitle(QApplication::translate("ScaraRobotClass", "SCARA\346\211\213\347\263\273", nullptr));
        rbScaraLeft->setText(QApplication::translate("ScaraRobotClass", "\345\267\246\346\211\213\347\263\273", nullptr));
        rbScaraRight->setText(QApplication::translate("ScaraRobotClass", "\345\217\263\346\211\213\347\263\273", nullptr));
        btScaraHandSet->setText(QApplication::translate("ScaraRobotClass", "\350\256\276\347\275\256\346\211\213\347\263\273", nullptr));
        groupBox_5->setTitle(QApplication::translate("ScaraRobotClass", "\346\234\272\345\231\250\344\272\272", nullptr));
        cbRobotSel->setItemText(0, QApplication::translate("ScaraRobotClass", "0", nullptr));
        cbRobotSel->setItemText(1, QApplication::translate("ScaraRobotClass", "1", nullptr));
        cbRobotSel->setItemText(2, QApplication::translate("ScaraRobotClass", "2", nullptr));
        cbRobotSel->setItemText(3, QApplication::translate("ScaraRobotClass", "3", nullptr));
        cbRobotSel->setItemText(4, QApplication::translate("ScaraRobotClass", "4", nullptr));
        cbRobotSel->setItemText(5, QApplication::translate("ScaraRobotClass", "5", nullptr));
        cbRobotSel->setItemText(6, QApplication::translate("ScaraRobotClass", "6", nullptr));
        cbRobotSel->setItemText(7, QApplication::translate("ScaraRobotClass", "7", nullptr));
        cbRobotSel->setItemText(8, QApplication::translate("ScaraRobotClass", "8", nullptr));
        cbRobotSel->setItemText(9, QApplication::translate("ScaraRobotClass", "9", nullptr));

        lbHomeStatus_6->setText(QApplication::translate("ScaraRobotClass", "A1(X)", nullptr));
        lbHomeStatus_7->setText(QApplication::translate("ScaraRobotClass", "A2(Y)", nullptr));
        lbHomeStatus_18->setText(QApplication::translate("ScaraRobotClass", "A3(Z)", nullptr));
        lbHomeStatus_20->setText(QApplication::translate("ScaraRobotClass", "A4(A)", nullptr));
        lbHomeStatus_50->setText(QApplication::translate("ScaraRobotClass", "A5(B)", nullptr));
        label_19->setText(QApplication::translate("ScaraRobotClass", "A6(C)", nullptr));
        label_24->setText(QApplication::translate("ScaraRobotClass", "\345\235\220\346\240\207(ACS)", nullptr));
        label_23->setText(QApplication::translate("ScaraRobotClass", "\345\235\220\346\240\207(MCS)", nullptr));
        label_21->setText(QApplication::translate("ScaraRobotClass", "\345\235\220\346\240\207(WCS)", nullptr));
        label_20->setText(QApplication::translate("ScaraRobotClass", "\345\235\220\346\240\207\345\235\220\346\240\207(PCS_1)", nullptr));
        label_22->setText(QApplication::translate("ScaraRobotClass", "\345\235\220\346\240\207\345\235\220\346\240\207(PCS_2)", nullptr));
        label_25->setText(QApplication::translate("ScaraRobotClass", "\345\235\220\346\240\207(TCS)", nullptr));
        groupBox_17->setTitle(QApplication::translate("ScaraRobotClass", "\345\267\245\345\205\267\350\275\264\345\221\250\346\234\237(CAXIS)", nullptr));
        btCAxisToolPeriod->setText(QApplication::translate("ScaraRobotClass", "\345\221\250\346\234\237\350\256\276\345\256\232", nullptr));
        label->setText(QApplication::translate("ScaraRobotClass", "\345\221\250\346\234\237", nullptr));
        groupBox_21->setTitle(QApplication::translate("ScaraRobotClass", "\346\234\272\345\231\250\344\272\272\347\212\266\346\200\201", nullptr));
        ckRobotEnabled->setText(QApplication::translate("ScaraRobotClass", "\345\267\262\345\220\257\347\224\250", nullptr));
        ckRobotCommStatus->setText(QApplication::translate("ScaraRobotClass", "\351\200\232\350\256\257\346\214\207\347\244\272", nullptr));
        ckRobotPowerStatus->setText(QApplication::translate("ScaraRobotClass", "\344\275\277\350\203\275\347\212\266\346\200\201", nullptr));
        ckRobotRefStatus->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\345\216\237\347\202\271\345\256\214\346\210\220", nullptr));
        ckRobotStoppedStatus->setText(QApplication::translate("ScaraRobotClass", "\350\275\264\345\201\234\346\255\242\347\212\266\346\200\201", nullptr));
        ckRobotHomingStatus->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\345\216\237\347\202\271\344\270\255", nullptr));
        ckRobotTrackingStatus->setText(QApplication::translate("ScaraRobotClass", "\346\234\272\345\231\250\344\272\272\350\267\237\350\270\252\347\212\266\346\200\201", nullptr));
        ckRobotDone->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\346\211\247\350\241\214\345\256\214\346\210\220", nullptr));
        ckRobotBusy->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\346\211\247\350\241\214\344\270\255", nullptr));
        ckRobotActive->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\345\267\262\346\277\200\346\264\273", nullptr));
        ckRobotAborted->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\350\242\253\346\211\223\346\226\255", nullptr));
        ckRobotAccepted->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\345\267\262\346\216\245\346\224\266", nullptr));
        ckRobotError->setText(QApplication::translate("ScaraRobotClass", "\345\221\275\344\273\244\346\211\247\350\241\214\345\244\261\350\264\245", nullptr));
        ckRobotAlarm->setText(QApplication::translate("ScaraRobotClass", "\346\234\272\345\231\250\344\272\272\346\212\245\350\255\246", nullptr));
        lbHomeStatus_62->setText(QApplication::translate("ScaraRobotClass", "\345\275\223\345\211\215\346\250\241\345\274\217", nullptr));
        lbHomeStatus_63->setText(QApplication::translate("ScaraRobotClass", "\351\224\231\350\257\257\344\273\243\347\240\201", nullptr));
        lbHomeStatus_64->setText(QApplication::translate("ScaraRobotClass", "\346\212\245\350\255\246ID", nullptr));
        lbHomeStatus_65->setText(QApplication::translate("ScaraRobotClass", "\350\277\220\345\212\250\347\212\266\346\200\201", nullptr));
        lbHomeStatus_66->setText(QApplication::translate("ScaraRobotClass", "\350\277\220\345\212\250ID", nullptr));
        lbHomeStatus_67->setText(QApplication::translate("ScaraRobotClass", "\346\277\200\346\264\273\347\232\204ID", nullptr));
        lbHomeStatus_68->setText(QApplication::translate("ScaraRobotClass", "\351\246\226\346\254\241\345\274\225\347\224\250ID", nullptr));
        lbHomeStatus_69->setText(QApplication::translate("ScaraRobotClass", "\346\234\200\345\220\216\346\216\245\346\224\266ID", nullptr));
        groupBox_8->setTitle(QApplication::translate("ScaraRobotClass", "\350\277\220\345\212\250\346\216\247\345\210\266", nullptr));
        groupBox_6->setTitle(QString());
        ckRobotEnable->setText(QApplication::translate("ScaraRobotClass", "\346\234\272\345\231\250\344\272\272\345\220\257\347\224\250", nullptr));
        ckRobotPowerOn->setText(QApplication::translate("ScaraRobotClass", "\346\234\272\345\231\250\344\272\272\344\275\277\350\203\275", nullptr));
        ckRobotRel->setText(QApplication::translate("ScaraRobotClass", "\347\233\270\345\257\271\350\277\220\345\212\250", nullptr));
        btRobotPTP->setText(QApplication::translate("ScaraRobotClass", "PTP\350\277\220\345\212\250", nullptr));
        btRobotLinear->setText(QApplication::translate("ScaraRobotClass", "\347\233\264\347\272\277\346\217\222\350\241\245", nullptr));
        btRobotCirc->setText(QApplication::translate("ScaraRobotClass", "\345\234\206\345\274\247\346\217\222\350\241\245", nullptr));
        btRobotRdTool->setText(QApplication::translate("ScaraRobotClass", "\350\257\273\345\267\245\345\205\267\345\201\217\347\247\273", nullptr));
        btRobotWrTool->setText(QApplication::translate("ScaraRobotClass", "\345\206\231\345\267\245\345\205\267\345\201\217\347\247\273", nullptr));
        btRobotSetPos->setText(QApplication::translate("ScaraRobotClass", "\344\275\215\347\275\256\350\256\276\345\256\232", nullptr));
        btRobotHome->setText(QApplication::translate("ScaraRobotClass", "\345\233\236\351\233\266", nullptr));
        btRobotHalt->setText(QApplication::translate("ScaraRobotClass", "\344\270\255\346\255\242", nullptr));
        btRobotReset->setText(QApplication::translate("ScaraRobotClass", "\345\244\215\344\275\215", nullptr));
        btRobotJump->setText(QApplication::translate("ScaraRobotClass", "\350\267\263\350\267\203", nullptr));
        groupBox_4->setTitle(QString());
        rbTargetPos->setText(QApplication::translate("ScaraRobotClass", "\347\233\256\346\240\207\344\275\215\347\275\256", nullptr));
        cbCircDir->setItemText(0, QApplication::translate("ScaraRobotClass", "\351\241\272\346\227\266\351\222\210", nullptr));
        cbCircDir->setItemText(1, QApplication::translate("ScaraRobotClass", "\351\200\206\346\227\266\351\222\210", nullptr));

        label_2->setText(QApplication::translate("ScaraRobotClass", "\345\235\220\346\240\207\347\263\273", nullptr));
        cbCircPath->setItemText(0, QApplication::translate("ScaraRobotClass", "\344\270\211\347\202\271\345\234\206\345\274\247", nullptr));
        cbCircPath->setItemText(1, QApplication::translate("ScaraRobotClass", "\344\270\244\347\202\271+\345\234\206\345\277\203", nullptr));
        cbCircPath->setItemText(2, QApplication::translate("ScaraRobotClass", "\344\270\244\347\202\271+\347\237\242\351\207\217", nullptr));

        cbRobotCoord->setItemText(0, QApplication::translate("ScaraRobotClass", "ACS", nullptr));
        cbRobotCoord->setItemText(1, QApplication::translate("ScaraRobotClass", "MCS", nullptr));
        cbRobotCoord->setItemText(2, QApplication::translate("ScaraRobotClass", "WCS", nullptr));
        cbRobotCoord->setItemText(3, QApplication::translate("ScaraRobotClass", "PCS_1", nullptr));
        cbRobotCoord->setItemText(4, QApplication::translate("ScaraRobotClass", "PCS_2", nullptr));
        cbRobotCoord->setItemText(5, QApplication::translate("ScaraRobotClass", "TCS", nullptr));

        lbCircDir->setText(QApplication::translate("ScaraRobotClass", "\346\217\222\350\241\245\346\226\271\345\220\221", nullptr));
        lbCircPath->setText(QApplication::translate("ScaraRobotClass", "\346\217\222\350\241\245\346\250\241\345\274\217", nullptr));
        rbMidPos->setText(QApplication::translate("ScaraRobotClass", "\344\270\255\351\227\264\347\202\271(\345\234\206\345\274\247)", nullptr));
        groupBox_7->setTitle(QString());
        lbHomeStatus_10->setText(QApplication::translate("ScaraRobotClass", "A1", nullptr));
        lbHomeStatus_21->setText(QApplication::translate("ScaraRobotClass", "A2", nullptr));
        lbHomeStatus_22->setText(QApplication::translate("ScaraRobotClass", "A3", nullptr));
        lbHomeStatus_23->setText(QApplication::translate("ScaraRobotClass", "A4", nullptr));
        label_3->setText(QApplication::translate("ScaraRobotClass", "A5", nullptr));
        label_4->setText(QApplication::translate("ScaraRobotClass", "A6", nullptr));
        lbHomeStatus_72->setText(QApplication::translate("ScaraRobotClass", "X", nullptr));
        lbHomeStatus_98->setText(QApplication::translate("ScaraRobotClass", "C", nullptr));
        lbHomeStatus_73->setText(QApplication::translate("ScaraRobotClass", "A", nullptr));
        lbHomeStatus_71->setText(QApplication::translate("ScaraRobotClass", "Y", nullptr));
        lbHomeStatus_70->setText(QApplication::translate("ScaraRobotClass", "Z", nullptr));
        lbHomeStatus_96->setText(QApplication::translate("ScaraRobotClass", "B", nullptr));
        groupBox_31->setTitle(QApplication::translate("ScaraRobotClass", "\350\267\263\350\267\203", nullptr));
        lbHomeStatus_15->setText(QApplication::translate("ScaraRobotClass", "\345\256\211\345\205\250\351\253\230\345\272\246", nullptr));
        btRobotSetSafeHeight->setText(QApplication::translate("ScaraRobotClass", "\350\256\276\347\275\256", nullptr));
        groupBox_27->setTitle(QApplication::translate("ScaraRobotClass", "\347\202\271\345\212\250\345\217\202\346\225\260", nullptr));
        btRobotJogFwd->setText(QApplication::translate("ScaraRobotClass", "\347\202\271\345\212\250\345\211\215\350\277\233", nullptr));
        lbHomeStatus_86->setText(QApplication::translate("ScaraRobotClass", "\351\200\237\345\272\246", nullptr));
        cbRobotJogCoord->setItemText(0, QApplication::translate("ScaraRobotClass", "ACS", nullptr));
        cbRobotJogCoord->setItemText(1, QApplication::translate("ScaraRobotClass", "MCS", nullptr));
        cbRobotJogCoord->setItemText(2, QApplication::translate("ScaraRobotClass", "WCS", nullptr));
        cbRobotJogCoord->setItemText(3, QApplication::translate("ScaraRobotClass", "PCS_1", nullptr));
        cbRobotJogCoord->setItemText(4, QApplication::translate("ScaraRobotClass", "PCS_2", nullptr));
        cbRobotJogCoord->setItemText(5, QApplication::translate("ScaraRobotClass", "TCS", nullptr));

        btRobotJogBwd->setText(QApplication::translate("ScaraRobotClass", "\347\202\271\345\212\250\345\220\216\351\200\200", nullptr));
        cbRobotJogAxisSel->setItemText(0, QApplication::translate("ScaraRobotClass", "0", nullptr));
        cbRobotJogAxisSel->setItemText(1, QApplication::translate("ScaraRobotClass", "1", nullptr));
        cbRobotJogAxisSel->setItemText(2, QApplication::translate("ScaraRobotClass", "2", nullptr));
        cbRobotJogAxisSel->setItemText(3, QApplication::translate("ScaraRobotClass", "3", nullptr));
        cbRobotJogAxisSel->setItemText(4, QApplication::translate("ScaraRobotClass", "4", nullptr));
        cbRobotJogAxisSel->setItemText(5, QApplication::translate("ScaraRobotClass", "5", nullptr));

        ckRobotJogAbcAsAcs->setText(QApplication::translate("ScaraRobotClass", "ABC_AS_ACS", nullptr));
        lbHomeStatus_90->setText(QApplication::translate("ScaraRobotClass", "\345\212\240\351\200\237\345\272\246", nullptr));
        lbHomeStatus_91->setText(QApplication::translate("ScaraRobotClass", "\345\207\217\351\200\237\345\272\246", nullptr));
        lbHomeStatus_97->setText(QApplication::translate("ScaraRobotClass", "\346\257\224\344\276\213 - \345\212\233\347\237\251", nullptr));
        lbHomeStatus_103->setText(QApplication::translate("ScaraRobotClass", "\346\234\200\345\244\247\350\277\220\345\212\250\350\267\235\347\246\273", nullptr));
        txRobotJogKVel->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        lbHomeStatus_102->setText(QApplication::translate("ScaraRobotClass", "\345\235\220\346\240\207\347\263\273", nullptr));
        lbHomeStatus_92->setText(QApplication::translate("ScaraRobotClass", "\350\267\203\345\272\246", nullptr));
        txRobotJogKJerk->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        lbHomeStatus_105->setText(QApplication::translate("ScaraRobotClass", "\350\275\264\351\200\211\346\213\251", nullptr));
        txRobotJogKTor->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        txRobotJogKAcc->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        label_5->setText(QApplication::translate("ScaraRobotClass", "\346\234\200\345\244\247\350\277\220\345\212\250\350\247\222\345\272\246", nullptr));
        groupBox_24->setTitle(QApplication::translate("ScaraRobotClass", "\350\267\257\345\276\204\345\217\202\346\225\260", nullptr));
        lbHomeStatus_87->setText(QApplication::translate("ScaraRobotClass", "\347\274\223\345\206\262\346\250\241\345\274\217", nullptr));
        cbRobotOriMode->setItemText(0, QApplication::translate("ScaraRobotClass", "GREATCIRCLE", nullptr));
        cbRobotOriMode->setItemText(1, QApplication::translate("ScaraRobotClass", "AXIS", nullptr));

        lbHomeStatus_13->setText(QApplication::translate("ScaraRobotClass", "\350\267\257\345\276\204\345\207\217\351\200\237\345\272\246", nullptr));
        txRobotFBKAcc->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        txRobotFBKVel->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        lbHomeStatus_82->setText(QApplication::translate("ScaraRobotClass", "\351\200\237\346\257\224", nullptr));
        lbHomeStatus_83->setText(QApplication::translate("ScaraRobotClass", "\350\267\235\347\246\273", nullptr));
        lbHomeStatus_85->setText(QApplication::translate("ScaraRobotClass", "\350\247\222\345\272\246\350\267\235\347\246\273", nullptr));
        lbHomeStatus_84->setText(QApplication::translate("ScaraRobotClass", "\347\233\264\347\272\277\350\267\235\347\246\273", nullptr));
        ckRobotTransEucDone->setText(QApplication::translate("ScaraRobotClass", "\350\267\235\347\246\273\345\210\260\350\276\276\345\220\216\350\276\223\345\207\272done", nullptr));
        lbHomeStatus_88->setText(QApplication::translate("ScaraRobotClass", "\345\271\263\347\247\273\350\277\207\346\270\241\346\250\241\345\274\217", nullptr));
        cbRobotTransMode->setItemText(0, QApplication::translate("ScaraRobotClass", "\346\227\240", nullptr));
        cbRobotTransMode->setItemText(1, QApplication::translate("ScaraRobotClass", "\345\237\272\344\272\216\351\200\237\345\272\246", nullptr));
        cbRobotTransMode->setItemText(2, QApplication::translate("ScaraRobotClass", "\345\237\272\344\272\216\350\275\254\350\247\222\350\267\235\347\246\273", nullptr));
        cbRobotTransMode->setItemText(3, QApplication::translate("ScaraRobotClass", "\345\237\272\344\272\216\350\275\264\350\267\235\347\246\273", nullptr));

        lbHomeStatus_14->setText(QApplication::translate("ScaraRobotClass", "\350\267\257\345\276\204\350\267\203\345\272\246", nullptr));
        cbRobotBufferMode->setItemText(0, QApplication::translate("ScaraRobotClass", "Aborting", nullptr));
        cbRobotBufferMode->setItemText(1, QApplication::translate("ScaraRobotClass", "Buffered", nullptr));
        cbRobotBufferMode->setItemText(2, QApplication::translate("ScaraRobotClass", "BlendingLow", nullptr));
        cbRobotBufferMode->setItemText(3, QApplication::translate("ScaraRobotClass", "BlendingPrevious", nullptr));
        cbRobotBufferMode->setItemText(4, QApplication::translate("ScaraRobotClass", "BlendingNext", nullptr));
        cbRobotBufferMode->setItemText(5, QApplication::translate("ScaraRobotClass", "BlendingHigh", nullptr));

        txRobotFBKTor->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        lbHomeStatus_12->setText(QApplication::translate("ScaraRobotClass", "\350\267\257\345\276\204\345\212\240\351\200\237\345\272\246", nullptr));
        ckRobotTransBlendAux->setText(QApplication::translate("ScaraRobotClass", "\350\276\205\345\212\251\350\275\264\345\217\202\344\270\216\346\267\267\345\220\210", nullptr));
        lbHomeStatus_89->setText(QApplication::translate("ScaraRobotClass", "\346\227\213\350\275\254\350\277\207\346\270\241\346\250\241\345\274\217", nullptr));
        txRobotFBKJerk->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        lbHomeStatus_17->setText(QApplication::translate("ScaraRobotClass", "\345\212\233\347\237\251", nullptr));
        lbHomeStatus_11->setText(QApplication::translate("ScaraRobotClass", "\350\267\257\345\276\204\351\200\237\345\272\246", nullptr));
        groupBox_28->setTitle(QApplication::translate("ScaraRobotClass", "\346\200\245\345\201\234\345\217\202\346\225\260", nullptr));
        lbHomeStatus_93->setText(QApplication::translate("ScaraRobotClass", "\345\207\217\351\200\237\345\272\246", nullptr));
        btRobotEStopKAcc->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        lbHomeStatus_94->setText(QApplication::translate("ScaraRobotClass", "\350\267\203\345\272\246", nullptr));
        btRobotEStopKJerk->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        lbHomeStatus_107->setText(QApplication::translate("ScaraRobotClass", "\345\212\233\347\237\251", nullptr));
        btRobotEStopKTor->setPrefix(QApplication::translate("ScaraRobotClass", "x ", nullptr));
        lbHomeStatus_95->setText(QApplication::translate("ScaraRobotClass", "\346\200\245\345\201\234\350\276\223\345\205\245\347\202\271", nullptr));
        cbRobotHaltClear->setText(QApplication::translate("ScaraRobotClass", "\344\270\255\346\255\242\346\227\266\346\270\205\351\231\244\347\274\223\345\255\230", nullptr));
        cbRobotEStopClear->setText(QApplication::translate("ScaraRobotClass", "\346\200\245\345\201\234\346\227\266\346\270\205\351\231\244\347\274\223\345\255\230", nullptr));
        btRobotEStop->setText(QApplication::translate("ScaraRobotClass", "\346\200\245\345\201\234", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ScaraRobotClass", "\346\234\272\346\242\260\346\211\213\350\260\203\350\257\225", nullptr));
        groupBox->setTitle(QApplication::translate("ScaraRobotClass", "ACP", nullptr));
        label_9->setText(QApplication::translate("ScaraRobotClass", "\345\217\230\351\207\217\345\220\215", nullptr));
        txAcpTagName->setText(QString());
        label_13->setText(QApplication::translate("ScaraRobotClass", "\345\255\227\350\212\202\346\225\260", nullptr));
        label_10->setText(QApplication::translate("ScaraRobotClass", "\345\217\230\351\207\217\345\200\274", nullptr));
        btAcpRead->setText(QApplication::translate("ScaraRobotClass", "\350\257\273\345\217\226", nullptr));
        btAcpWrite->setText(QApplication::translate("ScaraRobotClass", "\345\206\231\345\205\245", nullptr));
        btTest->setText(QApplication::translate("ScaraRobotClass", "PushButton", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ScaraRobotClass", "IO", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScaraRobotClass: public Ui_ScaraRobotClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCARAROBOT_H
