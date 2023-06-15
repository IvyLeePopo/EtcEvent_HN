#ifndef INVOICE_UNIT_H
#define INVOICE_UNIT_H
#include "StandTask.h"
#include "PayHelper.h"
#include <list>
#include "InvoiceHttpSync.h"


class CInvoiceUnit : public trawe::CStandTask
{
public:
    CInvoiceUnit();
    virtual ~CInvoiceUnit();
public:
    virtual void RunBody();
public:
    int StartNewInvoiceTransmit(InvoiceParameter&param);
	int StartInvoiceCancel(InvoiceCancelParameter&param);
  
private:
	void ExecuteOneInvoiceTransmit();
	void ExecuteOneInvoiceCancel();
    int ClearAbandonInvoice();
	int SetQrcodeDisplay(InvoiceParameter&param);

private:
    int NotifyResult(int option,int result);
	void AddInvoiceCancel2DB(InvoiceCancelParameter&param);
	void AddInvoice2DB(InvoiceParameter&param);
	void DeleteInvoiceCancelFromDB(InvoiceCancelParameter&param);
	void DeleteInvoiceFromDB(InvoiceParameter&param);
	bool GetOneInvoiceCancelFromDB();
	bool GetOneInvoiceFromDB();

private:
    std::list<InvoiceParameter> m_InvoiceGroup;
	std::list<InvoiceCancelParameter> m_InvoiceCancelGroup;

    std::list<InvoiceHttpSync*> m_AbandonInvoiceGroup;
    /*֧����Ʊ��*/
    CRITICAL_SECTION m_InvoiceLock;

	HttpConnection m_HttpPipe;
	std::string UrlPrefix;
	int PictureNo;
	std::string Hints;
private:
    CInvoiceUnit(const CInvoiceUnit&);
    CInvoiceUnit& operator = (const CInvoiceUnit&);

};


#endif // INVOCE_UNIT_H
/**
 * �˴�����֧���ĺ��Ĺ��ܣ�
 * 1��ɨ��֧����Ʊ���ݷ�����̨
 * 2���ֽ�֧����Ʊ���ݷ�����̨
 * 3������������������ʾ��
 * 4��֪ͨ��Ʊ���ݴ�������
 */