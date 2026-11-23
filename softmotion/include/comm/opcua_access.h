#pragma once
#include <QObject>
#include <Qtimer>
#include <QOpcUaClient>
#include <QOPCUaProvider>
#include <QOpcUaEndpointDescription>

class OpcUaAccess : public QObject
{
	Q_OBJECT

private:
	OpcUaAccess();

public:
	OpcUaAccess(const OpcUaAccess& clone) = delete;
	OpcUaAccess& operator=(const OpcUaAccess&) = delete;
	OpcUaAccess(OpcUaAccess&&) = delete;
	OpcUaAccess& operator = (OpcUaAccess&&) = delete;
	~OpcUaAccess();

	static OpcUaAccess& GetInst() {
		static OpcUaAccess instance;
		return instance;
	}

private:
	bool m_auto_reconnect = false;
	bool m_init_done = false;	
	bool m_last_state = false;

	QString m_ip;
	int m_port;
	QTimer m_timer;
	
	QOpcUaProvider m_provider;
	QOpcUaClient* m_client = nullptr;	
	QOpcUaClient::ClientState m_clientState = QOpcUaClient::Disconnected;

	QVector<QOpcUaEndpointDescription> m_endpoints;
	QOpcUa::UaStatusCode m_statusCode;
	QUrl m_requestUrl;
	QString m_prefix = "ns=2;s=Sinsegye-x86_64-Linux-SM-CNC.Resources.Application.GVL_Axis";

public:
	QString opcua_get_prefix();
	void opcua_set_prefix(const QString& prefix);

	bool opcua_connect(QString ip, int port = 4840, bool auto_reconnect = false);
	void opcua_disconnect();
	bool opcua_connected();

	bool opcua_read_tag(QString name, void* value, int nTimeOutMs);
	bool opcua_write_tag(QString name, void* value, int nTimeOutMs);
};