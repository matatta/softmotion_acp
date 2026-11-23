#include "acp_access.h"
#include <mutex>
#include <chrono>
#include <atomic>
#include <Windows.h>

#define	_DEBUG_ON	0
#pragma comment(lib, "acpsymbolapi-c.lib")

static std::mutex m_mutex;
static AcpConnectionStatus m_status = ACP_RW_ERR_NO_CONN;
static TAG_HANDER hAcp = nullptr;

static void CbAcpConnect(AcpConnectionStatus status, void* lpContext) {
	if (lpContext != nullptr) {
		*(AcpConnectionStatus*)lpContext = status;
	}
}

/**
 * 故障代码统一处理.
 *
 * \param offset 偏移量
 * \param err_code 故障代码
 * \return 故障代码枚举
 */
ET_RESULT make_result(ET_RESULT_OFFSET offset, int32_t err_code) {
	ET_RESULT result = ET_RESULT_NO_ERROR;
	switch (err_code) {
	case -1: result = ET_RESULT_ERR_UNKOWN; break;
	case 0: result = ET_RESULT_NO_ERROR; break;
	default:
		result = static_cast<ET_RESULT>(offset + err_code); break;
	}

	return result;
}

/**
 * 检查连接是否有效.
 *
 * \return 成功返回0,失败返回-1或者错误码
 */
ET_RESULT acp_check_valid() {
	if (hAcp == nullptr)
		return make_result(ET_RESULT_OFFSET_ACP_RW, ACP_RW_ERR_INVAILD_HANDLE);

	AcpConnectionStatus status = acp_conn_status();
	if (status != ACP_RW_ERR_CONN_SUCCESS)
		return make_result(ET_RESULT_OFFSET_ACP_RW_CONN, status);

	return ET_RESULT_NO_ERROR;
}

/**
 * 打开ACP连接.
 * 
 * \param ip_addr ip地址
 * \param mode 通讯模式: 0-socket通讯 1-共享内存
 * \return 成功返回true,失败返回false
 */
bool acp_open(const char* ip_addr, int mode) {
	const std::lock_guard<std::mutex> locker(m_mutex);		
	if (hAcp != nullptr) {
		AcpDestoryConnHandler(hAcp);
		hAcp = nullptr;
	}
/*
	ConnParam connParam;
	connParam.connType = ACP_CONN_TYPE_TCP;
	strcpy_s(connParam.szConnParam, 255, ip_addr);
	hAcp = AcpCreatConnHandler(&connParam, CbAcpConnect, &m_status);
*/
	switch (mode) {
	case 0: 
		hAcp = AcpCreateSocketHandler(AcpConnType::ACP_RW_CONN_TYPE_TCP, ip_addr, CbAcpConnect, &m_status); 
		break;
	case 1: 
		hAcp = AcpCreateIvshmConnHandler(0, 128 * 1024, CbAcpConnect, &m_status);
		break;
	default:break;
	}

	return hAcp != nullptr ? true : false;
}

/**
 * 关闭ACP连接.
 *
 */
void acp_close() {
	const std::lock_guard<std::mutex> locker(m_mutex);	
	if (hAcp != nullptr) {
		AcpDestoryConnHandler(hAcp);
		hAcp = nullptr;
	}
	m_status = AcpConnectionStatus::ACP_RW_ERR_NO_CONN;
}

/**
 * 获取ACP连接状态.
 *
 * \return ACP连接状态
 */
AcpConnectionStatus acp_conn_status() {
	return m_status;
}

/**
 * 读取单一变量.
 * 
 * \param var 变量结构体
 * \param nTimeOutMs 超时时间
 * \return 成功返回0,失败返回-1或者错误码
 */
ET_RESULT acp_read_tag(PLC_TAG var, int nTimeOutMs) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	const std::lock_guard<std::mutex> locker(m_mutex);
	ET_RESULT result = acp_check_valid();
	if (result == ET_RESULT_NO_ERROR) {
		TagInfo info;
		info.tagDesc.szSymbolFullName = var.name;
		info.tagValue.pValueData = var.content;
		info.tagValue.nValueSize = var.size;
		result = make_result(ET_RESULT_OFFSET_ACP_RW, AcpReadTagValue(hAcp, &info, nTimeOutMs));
	}

#ifdef _DEBUG
#if _DEBUG_ON == 1
		auto end = std::chrono::steady_clock::now();
		double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();
		char log[256] = "";
		snprintf(log, sizeof(log), "acp_read(%s): time = %fms, result = %d\n", var.name, milliseconds, result);
		OutputDebugStringA(log);
#endif
#endif	
	return result;
}

/**
 * 写单一变量.
 * 
 * \param var 变量结构体
 * \param nTimeOutMs 超时时间
 * \return 成功返回0,失败返回-1或者错误码
 */
ET_RESULT acp_write_tag(PLC_TAG var, int nTimeOutMs) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	const std::lock_guard<std::mutex> locker(m_mutex);
	ET_RESULT result = acp_check_valid();
	if (result == ET_RESULT_NO_ERROR) {
		TagInfo info;
		info.tagDesc.szSymbolFullName = var.name;
		info.tagValue.pValueData = var.content;
		info.tagValue.nValueSize = var.size;			
		result = make_result(ET_RESULT_OFFSET_ACP_RW, AcpWriteTagValue(hAcp, &info, nTimeOutMs));
	}

#ifdef _DEBUG
#if _DEBUG_ON == 1
		auto end = std::chrono::steady_clock::now();
		double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();
		char log[256] = "";
		snprintf(log, sizeof(log), "acp_write(%s): time = %fms, result = %d\n", var.name, milliseconds, result);
		OutputDebugStringA(log);
#endif
#endif	
	return result;
}

/**
 * 读多个变量.
 * 
 * \param varlist 变量结构体列表
 * \param nTimeOutMs 超时时间
 * \return 成功返回0,失败返回-1或者错误码
 */
ET_RESULT acp_read_taglist(std::list<PLC_TAG> varlist, int nTimeOutMs) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	const std::lock_guard<std::mutex> locker(m_mutex);
	ET_RESULT result = acp_check_valid();
	if (result == ET_RESULT_NO_ERROR && !varlist.empty()) {
		int cnt = 0;
		TagInfo* pInfo = new TagInfo[varlist.size()];
		for (auto tag = varlist.begin(); tag != varlist.end(); ++tag) {
			pInfo[cnt].tagDesc.szSymbolFullName = tag->name;
			pInfo[cnt].tagValue.pValueData = tag->content;
			pInfo[cnt].tagValue.nValueSize = tag->size;
			cnt++;
		}		
		result = make_result(ET_RESULT_OFFSET_ACP_RW, AcpReadTagList(hAcp, pInfo, (int)varlist.size(), nTimeOutMs));
		delete[] pInfo;
	}

#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();
	char log[256] = "";
	snprintf(log, sizeof(log), "acp_read_taglist: time = %fms, result = %d\n", milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 写多个变量.
 * 
 * \param varlist 变量结构体列表
 * \param nTimeOutMs 超时时间
 * \return 成功返回0,失败返回-1或者错误码
 */
ET_RESULT acp_write_taglist(std::list<PLC_TAG> varlist, int nTimeOutMs) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	const std::lock_guard<std::mutex> locker(m_mutex);
	ET_RESULT result = acp_check_valid();
	if (result == ET_RESULT_NO_ERROR && !varlist.empty()) {
		int cnt = 0;
		TagInfo* pInfo = new TagInfo[varlist.size()];
		for (auto tag = varlist.begin(); tag != varlist.end(); ++tag) {
			pInfo[cnt].tagDesc.szSymbolFullName = tag->name;
			pInfo[cnt].tagValue.pValueData = tag->content;
			pInfo[cnt].tagValue.nValueSize = tag->size;
			cnt++;
		}		
		result = make_result(ET_RESULT_OFFSET_ACP_RW, AcpWriteTagList(hAcp, pInfo, (int)varlist.size(), nTimeOutMs));
		delete[] pInfo;
	}

#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();
	char log[256] = "";
	snprintf(log, sizeof(log), "acp_write_taglist: time = %fms, result = %d\n", milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

//
//ET_RESULT acp_var_get_value(void* var, int size, int timeout) {
//	const std::lock_guard<std::mutex> locker(m_mutex);
//	ET_RESULT result = acp_check_valid();
//	if (result == ET_RESULT_NO_ERROR) {
//		TagInfo info;
//		info.tagDesc.szSymbolFullName = name2str(var);
//		info.tagValue.pValueData = var;
//		info.tagValue.nValueSize = size;
//		result = make_result(ET_RESULT_OFFSET_ACP_RW, AcpReadTagValue(hAcp, &info, timeout));
//	}
//	return result;
//}