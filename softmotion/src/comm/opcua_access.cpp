#include "stdafx.h"
#include "opcua_access.h"

OpcUaAccess::OpcUaAccess()
{
	// 检查 open62541 插件
	if (m_provider.availableBackends().isEmpty()) {
		qDebug() << "no available backends";
		return;
	}

	// 创建客户端
	m_client = m_provider.createClient(m_provider.availableBackends().last());
	if (!m_client) {
		qDebug() << "create opcua client failed";
		return;
	}

	// 查找 endpoint
	connect(m_client, &QOpcUaClient::endpointsRequestFinished,
		[&](const QVector<QOpcUaEndpointDescription> endpoints, QOpcUa::UaStatusCode statusCode, QUrl requestUrl) {
			m_init_done = !endpoints.isEmpty();
			if (m_init_done) {
				m_endpoints = endpoints;
				m_statusCode = statusCode;
				m_requestUrl = requestUrl;
				m_client->connectToEndpoint(m_endpoints.last());
			}
		});

	// 检查连接状态
	connect(m_client, &QOpcUaClient::stateChanged, this, [=](QOpcUaClient::ClientState state) {
		m_clientState = state;
		qDebug() << state;

		// 自动重连
		bool new_state = (state == QOpcUaClient::Connected);
		if (m_auto_reconnect && !new_state && m_last_state) {
			m_timer.setInterval(1000);
			connect(&m_timer, &QTimer::timeout, [=]() {
				opcua_connect(m_ip, m_port, m_auto_reconnect);
				qDebug() << "start auto connect";
				});
			m_timer.start();
		}
		m_last_state = new_state;

		if (state == QOpcUaClient::Connecting || state == QOpcUaClient::Connected) {
			m_timer.stop();
		}
		});

	// 故障处理
	connect(m_client, &QOpcUaClient::errorChanged, this, [=](QOpcUaClient::ClientError err) {
		qDebug() << "client error: " << err;
		});

	// 读取事件
	connect(m_client, &QOpcUaClient::readNodeAttributesFinished, this, [=](QVector<QOpcUaReadResult> results, QOpcUa::UaStatusCode serviceResult) {
		bool var = results[0].value().toBool();
		});

}

OpcUaAccess::~OpcUaAccess()
{
	opcua_disconnect();
	m_client->deleteLater();
}

QString OpcUaAccess::opcua_get_prefix() {
	return m_prefix;
}

void OpcUaAccess::opcua_set_prefix(const QString& prefix) {
	m_prefix = prefix;
}

bool OpcUaAccess::opcua_connect(QString ip, int port, bool auto_reconnect)
{
	if (ip.isEmpty() || port == 0) {
		return false;
	}

	m_ip = ip;
	m_port = port;
	m_auto_reconnect = auto_reconnect;

	QString url = QString("opc.tcp://%1:%2").arg(m_ip).arg(m_port);
	return m_clientState != QOpcUaClient::Connected ? m_client->requestEndpoints(QUrl(url)) : true;
}

void OpcUaAccess::opcua_disconnect()
{
	if (m_init_done) {
		m_init_done = false;
		m_auto_reconnect = false;
		m_timer.stop();
		m_client->disconnectFromEndpoint();
	}
}

bool OpcUaAccess::opcua_connected()
{
	return m_clientState == QOpcUaClient::Connected;
}


bool OpcUaAccess::opcua_read_tag(QString name, void* value, int nTimeOutMs) {
	QOpcUaReadItem item;
	item.setNodeId(m_prefix.isEmpty() ? name : m_prefix + "." + name);
	item.setAttribute(QOpcUa::NodeAttribute::Value);

	QVector<QOpcUaReadItem> request;
	request.push_back(QOpcUaReadItem(item));
	return m_client->readNodeAttributes(request);
}

bool OpcUaAccess::opcua_write_tag(QString name, void* value, int nTimeOutMs) {
	return false;
}