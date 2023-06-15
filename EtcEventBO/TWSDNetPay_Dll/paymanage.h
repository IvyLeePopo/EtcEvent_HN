#ifndef PAY_MANAGE_H
#define PAY_MANAGE_H
#include "PayUnit.h"
#include <map>
#include <string>
//����֧����ԪPayUnit.h,����������Ԫ���ڶ����߳���ִ��
#include "CancelPayUnit.h"
#include "InvoiceUnit.h"





class CPayManage
{
public:
    CPayManage();
    ~CPayManage();
    int Init();
	int Destroy();
    bool IsEanble();
public:
    int SetCertificate(const std::string& qr_code,const std::string& nfc_pay,const std::string& nfc_rev);
	int SetCertificateError(int err=0);
    int StartNewPay(const DebitParameter&dp);
    int QueryCurrentPayState(std::string& pay_id,int& state);
    int CancelCurrentPay();
    int CancelTargetPay(const DebitCancleParameter& dp);
	int GetCurrentDebitResult(DebitResultParameter& dp);
    int InitAgencyPay(const char* szAreaInfo, const char* szLoaclStation,  const char* szLoaclLaneID,  const char* szServerInfo,  const int& iProvinceID);
    bool QueryVirtualCardDebit(const char* szParamContext, const int& iParamSize, const int& iFormatType, int& iSupportAgent, OUT char* szResult);
    bool AgencyDebitQuery(const char* szParamContext, const int& iParamSize, const int& iFormatType, OUT int& iResult);
    bool TransferImage(const char* szImageInfo, const int& iSize, const int& iFormatType);
	int StartNewCashInvoiceTransmit(InvoiceParameter& param);
	int StartCashInvoiceCancel(InvoiceCancelParameter& param);
private:
    CPayUnit* m_PayTask;
	CCancelPayUnit* m_CancelPayTask;
	CInvoiceUnit *m_InvoiceTask;
    CRITICAL_SECTION m_AccessMutex;
    bool m_IsEnable;
	InvoiceParameter m_InvoicePara;
	InvoiceCancelParameter m_InvoiceCancelPara;
private:
    CPayManage(const CPayManage&);
    CPayManage& operator = (const CPayManage&);

};


#endif // PAY_MANAGE_H

/**
 * ���ܣ�
 * 1���������֧�����
 * 2����ѯ��ǰ֧��״̬
 * 3���������������߼�
 * 4���ܵ�֧����ʱ�����ⲿ����
 */