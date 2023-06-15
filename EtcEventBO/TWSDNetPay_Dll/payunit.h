#ifndef PAY_UNIT_H
#define PAY_UNIT_H
#include "StandTask.h"
#include "PayHelper.h"
#include <list>
#include "PayHttpSync.h"
#include "AgencyPay.h"

class CPayUnit : public trawe::CStandTask
{
public:
    CPayUnit();
    virtual ~CPayUnit();
public:
    virtual void RunBody();
public:
    int StartNewPay(const DebitParameter&dp);
    int QueryCurrentPayState(std::string& pay_id,int& state);
    int CancelTargetPay(const DebitCancleParameter& dp);
    int GetCurrentDebitResult(DebitResultParameter& dp);
    int SetCertificate(const std::string& cer);
    int InitAgencyPay(const char* szAreaInfo, const char* szLoaclStation,  const char* szLoaclLaneID,  const char* szServerInfo,  const int& iProvinceID);
    bool QueryVirtualCardDebit(const char* szParamContext, const int& iParamSize, const int& iFormatType, int& iSupportAgent, OUT char* szResult);
    bool AgencyDebitQuery(const char* szParamContext, const int& iParamSize, const int& iFormatType, OUT int& iResult);
    bool TransferImage(const char* szImageInfo, const int& iSize, const int& iFormatType);
private:
	void ExecuteOnePay();//1����ƾ֤֧��2����ȡ֧��ƾ֤
    int ExecutePlatePay(const DebitParameter& debit_para,DebitResultParameter& debit_result );
    int ExecuteCodePay(DebitParameter& debit_para,DebitResultParameter& debit_result );
    int ExecuteOneCancle();
    int GetDebitCancleFromDataBase();
    int DeleteDebitCancleFromDataBase(const DebitCancleParameter& dp);
    int ClearAbandonPay();
private:
    int OpenCertificateScanner(const DebitParameter& debit_para);
    int QueryCertificate(std::string& cer);
    int GetCertificate(DebitParameter& debit_para);
    int NotifyPayStatus(int type,int result );//֧���м�״̬���������
    int NotifyResult(int option,int result);
    int HintPayResult(int pay_state,int detail_type);//�˴�ʹ�ö���ӿ��ĵ��е�״̬����
    void AdjustPayPlatformType(DebitResultParameter& debit_result);
    bool IsTargetPayCancled(const std::string& pay_id);
    int NewPayResult(const DebitResultParameter& debit_result);
    int ClearScreen();
	int SetFeeDisplay(const DebitParameter & debit_para);
	int SetPayResultDisplay(const std::string& pay_id, int Result, std::string FailReason);
private:
    std::list<DebitParameter> m_DebitGroup;
    std::list<DebitCancleParameter> m_DebitCancleGroup;
    std::list<DebitResultParameter> m_DebitResultGroup;
    std::list<PayHttpSync*> m_AbandonPayGroup;
    /*֧����������*/
    CRITICAL_SECTION m_PayLock;
    /*֧�������*/
    CRITICAL_SECTION m_PayResultLock;  
    /*֧��ƾ֤������ά�룬������*/
    CRITICAL_SECTION m_CertificateLock;
    std::string m_CertificateJson;
    CAgencyPay   m_AgencyPay;
	HttpConnection m_HttpPipe;
private:
    CPayUnit(const CPayUnit&);
    CPayUnit& operator = (const CPayUnit&);

};


#endif // PAY_UNIT_H
/**
 * �˴�����֧���ĺ��Ĺ��ܣ�
 * 1������֧��
 * 2������ɨ����ʾ
 * 3����ȡ��ά��֮�󣬷���֧������
 * 4������������������ʾ��
 * 5��֪֧ͨ�������
 */