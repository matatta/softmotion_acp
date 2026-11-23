#pragma once
#include "AcpSymbolApi/AcpSymbolApi-c.h"
#include "AcpSymbolApi/AcpSymbolDef.h"

#include "acp_result.h"
#include <list>

#define name2str(name) (#name)

typedef struct _plc_tag {
	const char* name;
	int   size;
	void* content;
} PLC_TAG;

bool acp_open(const char* ip_addr, int mode = 0);
void acp_close();
AcpConnectionStatus acp_conn_status();

ET_RESULT make_result(ET_RESULT_OFFSET offset, int32_t err_code);
ET_RESULT acp_read_tag(PLC_TAG var, int nTimeOutMs = 50);
ET_RESULT acp_write_tag(PLC_TAG var, int nTimeOutMs = 50);
ET_RESULT acp_read_taglist(std::list<PLC_TAG> varlist, int nTimeOutMs = 50);
ET_RESULT acp_write_taglist(std::list<PLC_TAG> varlist, int nTimeOutMs = 50);