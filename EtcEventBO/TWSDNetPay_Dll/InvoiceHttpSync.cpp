#include "stdafx.h"
#include "InvoiceHttpSync.h"
#include "RunParameter.h"
#include "SystemDiary.h"
#include "BaseDefine.h"
#include "TypeHelper.h"

InvoiceHttpSync::InvoiceHttpSync()
{
    ::InitializeCriticalSection(&m_AccessLock);
    m_State = 255;
    m_HttpPipe.ChangeRemotePeer(GetInvoicePlateformIP(), GetInvoicePlateformPort());
}
InvoiceHttpSync::~InvoiceHttpSync()
{
	Stop();
    ::DeleteCriticalSection(&m_AccessLock);
}
int InvoiceHttpSync::SetState(int state)
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    m_State = state;
	return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:InvoiceHttpSync::RunBody
//
// 功能描述:发送发票数据
//
// 输入参数:
// 输出参数:void
// 编写人员:朱道明
// 编写时间:2021-4-16
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//////////////////////////////////////////////////////////////
void InvoiceHttpSync::RunBody()
{
    InvoiceParameter para;
    {
        trawe::CScopeLock access_lock(&m_AccessLock);
        if (m_InvoiceGroup.empty())
        {
            Sleep(10);
            return ;
        }
        para = m_InvoiceGroup.front();
        
    }
    ExecuteOneInvoice(para);
	trawe::CScopeLock access_lock(&m_AccessLock);
	if(m_InvoiceGroup.empty())
	{
		TW_TRACE_DIARY_TYPE("传送列表前后状态不一致", TW_TRACE_TPPE_INVOICEHTTP);
	}
	else
	{
		m_InvoiceGroup.pop_front();
	}
    Sleep(10);
}

int InvoiceHttpSync::ExecuteOneInvoice(InvoiceParameter& para)
{
	SetState(-1);
	{  
        for (int i = 0; i < 3; ++i)
        {
			std::string invoice_json;
            {
                PackInvoiceHttpJson(para, invoice_json);    
            }

            TW_TRACE_DIARY_TYPE("开始上传发票流水: " + invoice_json, TW_TRACE_TPPE_INVOICEHTTP);

            std::string utf8_json_str = StringConvertByCodePage(invoice_json, CP_ACP, CP_UTF8);
            std::string recv_str;

			static unsigned short changeIP = 0;
			//默认使用第一个IP，当需要改变时，切换另一个IP进行支付
			m_HttpPipe.ChangeRemotePeer((changeIP % 2) ? GetInvoicePlateformSecondIP() : GetInvoicePlateformIP(), GetInvoicePlateformPort());
			char sLog[128] = {0};
			sprintf_s(sLog, "上传发票流水地址: http://%s:%d/ElectronicInvoice/start", m_HttpPipe.m_IpStr.c_str(), m_HttpPipe.m_Port);
			TW_TRACE_DIARY_TYPE(std::string(sLog), TW_TRACE_TPPE_INVOICEHTTP);

            HttpOption http_option;
            http_option.ConnectTimeoutMsecs = 2*1000;
            http_option.SendTimeoutMsecs = 6*1000;
            http_option.ReceveTimeoutMsecs = 6*1000;
            m_HttpPipe.SetHttpOption(http_option);

            if (!m_HttpPipe.Post("/ElectronicInvoice/start", utf8_json_str, recv_str))
            {
				//当前ip端口发送请求失败，在此处决定切换ip
				changeIP++;

                TW_TRACE_DIARY_TYPE("传送发票数据失败:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TPPE_INVOICEHTTP);
            }
            else
            {
                std::string gbk_recv_str = StringConvertByCodePage(recv_str, CP_UTF8, CP_ACP);
                InvoiceResultParameter tmp_invoice_result;
                if (!ParseInvoiceResultFromJson(gbk_recv_str, tmp_invoice_result))
                {
					//当前ip端口发送请求失败，在此处决定切换ip
					changeIP++;

                    SetState(255);
                    TW_TRACE_DIARY_TYPE("解析服务器回馈失败:" + gbk_recv_str, TW_TRACE_TPPE_INVOICEHTTP);
                    
					break;
                }
                m_ServerResponse = gbk_recv_str;
				TW_TRACE_DIARY_TYPE("服务器返回应答码:"+ Int2Str(tmp_invoice_result.ExecuteCode), TW_TRACE_TPPE_INVOICEHTTP);
                SetState(tmp_invoice_result.ExecuteCode);
				break;
            }

            Sleep(1000);
        }
    }
	return 0;
}

int InvoiceHttpSync::StartTransmit(InvoiceParameter& para)
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    m_InvoiceGroup.push_back(para);
    return 0;
}

int InvoiceHttpSync::TransmitState(std::string& recv_str)
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    recv_str = m_ServerResponse;
    return m_State;
}

bool InvoiceHttpSync::IsIdle()
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    return m_InvoiceGroup.empty();
}
