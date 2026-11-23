#ifndef PLC_ACCESS_H
#define PLC_ACCESS_H

#include <list>

typedef struct _plc_tag {
    const char* name;
    int   size;
    void* content;
} PLC_TAG;

class PlcAccess
{
protected:
    PlcAccess() {}

public:
    PlcAccess(const PlcAccess& clone) = delete;
    PlcAccess& operator=(const PlcAccess&) = delete;
    PlcAccess(PlcAccess &&) = delete;
    PlcAccess & operator=(PlcAccess &&) = delete;
    ~PlcAccess() {}

    static PlcAccess& GetInstance() {
        static PlcAccess instance;
        return instance;
    }

    virtual int open(const char* ip_addr);
    virtual void close();
    virtual int read_tag(PLC_TAG var, int nTimeOutMs = 10);
    virtual int write_tag(PLC_TAG var, int nTimeOutMs = 10);
    virtual int read_taglist(std::list<PLC_TAG> varlist, int nTimeOutMs = 10);
    virtual int write_taglist(std::list<PLC_TAG> varlist, int nTimeOutMs = 10);
};

#endif // PLC_ACCESS_H
