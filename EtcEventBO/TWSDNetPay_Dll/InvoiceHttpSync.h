#ifndef INVOICE_HTTP_SYNC_H
#define INVOICE_HTTP_SYNC_H
#include "StandTask.h"
#include "PayHelper.h"
#include "HttpConnection.h"
#include <list>

class InvoiceHttpSync : public trawe::CStandTask
{
public:
    InvoiceHttpSync();
    virtual ~InvoiceHttpSync();
public:
    virtual void RunBody();
    int StartTransmit(InvoiceParameter& para);

    bool IsIdle();
	int TransmitState(std::string& recv_str);
private:
    int SetState(int state);
	int ExecuteOneInvoice(InvoiceParameter& para);
private:
    std::list<InvoiceParameter> m_InvoiceGroup;
    CRITICAL_SECTION m_AccessLock;
    int m_State; //����Ͷ���ӿ��ĵ�һ�£�������ֵ(-1),���ڱ�ʾ���ڴ�����
    std::string m_ServerResponse;
    HttpConnection m_HttpPipe;
private:
    InvoiceHttpSync(const InvoiceHttpSync&);
    InvoiceHttpSync& operator = (const InvoiceHttpSync&);

};

#endif // INVOICE_HTTP_SYNC_H
