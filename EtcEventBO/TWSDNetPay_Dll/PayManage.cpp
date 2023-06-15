#include "stdafx.h"
#include "PayManage.h"
#include "SystemDiary.h"
#include "JsonHelper.h"

CPayManage::CPayManage()
{
    InitializeCriticalSection(&m_AccessMutex);
    m_PayTask = 0;
	m_CancelPayTask = 0;
	m_InvoiceTask = 0;

}
CPayManage::~CPayManage()
{
    DeleteCriticalSection(&m_AccessMutex);
    
}

int CPayManage::Destroy()
{
	trawe::CScopeLock tmp_lock(&m_AccessMutex);
	delete m_PayTask;
	m_PayTask=0;
	TW_TRACE_DIARY("支付处理单元，删除成功");

	delete m_CancelPayTask;
	m_CancelPayTask =0;
	TW_TRACE_DIARY("撤单----处理单元，删除成功");

	delete m_InvoiceTask;
	m_InvoiceTask = 0;
	TW_TRACE_DIARY("现金发票流水----处理单元，删除成功");

	return 0;
}


int CPayManage::Init()
{

    m_IsEnable = true;
    if(!m_IsEnable)
    {
        return -1;
    }
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask!=0)
    {
        TW_TRACE_DIARY("支付处理单元已经存在");
        return -1;
    }
	m_PayTask = new CPayUnit();
    if(m_PayTask==0)
    {
        TW_TRACE_DIARY("支付处理单元创建失败");
        return -1;
    }
    TW_TRACE_DIARY("支付处理单元创建成功");
    m_PayTask->Resume();
	//-----------------------
	if(m_CancelPayTask!=0)
	{
		TW_TRACE_DIARY("撤单----处理单元已经存在");
		return -1;
	}
	m_CancelPayTask = new CCancelPayUnit();
	if(m_CancelPayTask==0)
	{
		TW_TRACE_DIARY("撤单----处理单元创建失败");
		return -1;
	}
	TW_TRACE_DIARY("撤单----处理单元创建成功");
	m_CancelPayTask->Resume();

	//-----------------------
	if(m_InvoiceTask != 0)
    {
        TW_TRACE_DIARY("发票处理单元已经存在");
        return -1;
    }
	m_InvoiceTask = new CInvoiceUnit();
    if(m_InvoiceTask==0)
    {
        TW_TRACE_DIARY("发票处理单元创建失败");
        return -1;
    }
    TW_TRACE_DIARY("发票处理单元创建成功");
    m_InvoiceTask->Resume();



    return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:CPayManage::IsEanble
//
// 功能描述:是否启用新支付逻辑，用于过渡使用，在新支付逻辑无问题后，可以去掉该内容
//
// 输入参数:
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-29
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool CPayManage::IsEanble()
{
    return m_IsEnable;
}
int CPayManage::StartNewPay(const DebitParameter&dp)
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return -1;
    }
    return m_PayTask->StartNewPay(dp);
}
int CPayManage::QueryCurrentPayState(std::string& pay_id,int& state)
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return -1;
    }
    return m_PayTask->QueryCurrentPayState(pay_id,state);
}
int CPayManage::CancelCurrentPay()
{
    return -1;
}
int CPayManage::CancelTargetPay(const DebitCancleParameter& dp)
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return -1;
    }
    return m_PayTask->CancelTargetPay(dp);
}
int CPayManage::GetCurrentDebitResult(DebitResultParameter& dp)
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return -1;
    }
    return m_PayTask->GetCurrentDebitResult(dp);
}

int CPayManage::InitAgencyPay(const char* szAreaInfo, const char* szLoaclStation,  const char* szLoaclLaneID,  const char* szServerInfo,  const int& iProvinceID)
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return -1;
    }
    return m_PayTask->InitAgencyPay(szAreaInfo,szLoaclStation,szLoaclLaneID,szServerInfo,iProvinceID);
}

bool CPayManage::QueryVirtualCardDebit(const char* szParamContext, const int& iParamSize, const int& iFormatType, int& iSupportAgent, OUT char* szResult)
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return false;
    }
    return m_PayTask->QueryVirtualCardDebit(szParamContext,iParamSize,iFormatType,iSupportAgent,szResult);
}

bool CPayManage::AgencyDebitQuery(const char* szParamContext, const int& iParamSize, const int& iFormatType, OUT int& iResult)
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return false;
    }
    return m_PayTask->AgencyDebitQuery(szParamContext,iParamSize,iFormatType,iResult);
}
bool CPayManage::TransferImage(const char* szImageInfo, const int& iSize, const int& iFormatType)
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return false;
    }
    return m_PayTask->TransferImage(szImageInfo,iSize,iFormatType);
}

int CPayManage::SetCertificate(const std::string& qr_code,const std::string& nfc_pay,const std::string& nfc_rev)
{
    Json::Value root_node;
    root_node["QRCode"] = Json::Value(qr_code);
    root_node["NFCPayData"] = Json::Value(nfc_pay);
    root_node["NFCRevData"] = Json::Value(nfc_rev);

    Json::FastWriter json_writer;
    std::string json_str = json_writer.write(root_node);
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return -1;
    }
    return m_PayTask->SetCertificate(json_str);
}

int CPayManage::SetCertificateError(int err)
{
	std::string err_str="error_cert";
	trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_PayTask == 0)
    {
        return -1;
    }
    return m_PayTask->SetCertificate(err_str);
}

int CPayManage::StartNewCashInvoiceTransmit(InvoiceParameter& param)
{
	trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_InvoiceTask == 0)
    {
        return -1;
    }
    return m_InvoiceTask->StartNewInvoiceTransmit(param);
}

int CPayManage::StartCashInvoiceCancel(InvoiceCancelParameter& param)
{
	trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_InvoiceTask == 0)
    {
        return -1;
    }

    return m_InvoiceTask->StartInvoiceCancel(param);
}


