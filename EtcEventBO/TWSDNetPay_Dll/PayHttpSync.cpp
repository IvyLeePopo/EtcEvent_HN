#include "stdafx.h"
#include "PayHttpSync.h"
#include "RunParameter.h"
#include "SystemDiary.h"
#include "BaseDefine.h"
#include "TypeHelper.h"

PayHttpSync::PayHttpSync()
{
    ::InitializeCriticalSection(&m_AccessLock);
    m_PayState = 255;
    m_IsCancled = false;
    m_HttpPipe.ChangeRemotePeer(GetPayIP(), GetPayPort());
}
PayHttpSync::~PayHttpSync()
{
	Stop();
    ::DeleteCriticalSection(&m_AccessLock);
}
int PayHttpSync::SetPayState(int state)
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    m_PayState = state;
	return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:PayHttpSync::RunBody
//
// 功能描述:发送支付和撤单请求
//
// 输入参数:
// 输出参数:void
// 编写人员:朱亚冰
// 编写时间:2018-12-14
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
// ##对于支付请求
// 以下情况，会重新发送请求,其他情况，直接返回失败
// 1、云端返回码，查询失败（HTTP_RC_DEBIT_QUERY_ERROR）
// 2、云端返回码，正在输入密码（HTTP_RC_DEBIT_NEED_PASSWORD）
// 3、云端返回码，状态未明(HTTP_RC_PAY_STATUS_UNKNOW)
// 4、通讯超时
//
//////////////////////////////////////////////////////////////
void PayHttpSync::RunBody()
{
    DebitParameter debit_para;
    {
        trawe::CScopeLock access_lock(&m_AccessLock);
        if (m_DebitGroup.empty())
        {
            Sleep(10);
            return ;
        }
        debit_para = m_DebitGroup.front();
        
    }
    ExecuteOnePay(debit_para);
	trawe::CScopeLock access_lock(&m_AccessLock);
	if(m_DebitGroup.empty())
	{
		TW_TRACE_DIARY_TYPE("支付列表前后状态不一致", TW_TRACE_TPPE_PAYHTTP);
	}
	else
	{
		m_DebitGroup.pop_front();
	}
    Sleep(10);
}

int PayHttpSync::ExecuteOnePay(DebitParameter& debit_para)
{
	SetPayState(-1);
	{   //进行实际的支付
        strncpy(debit_para.NetPayDllVersion, GetNetPayDllVersion().c_str(), sizeof(debit_para.NetPayDllVersion) - 1);
        strncpy(debit_para.DeviceDllVersion, GetDeviceDllVersion().c_str(), sizeof(debit_para.DeviceDllVersion) - 1);
        strncpy(debit_para.TWProductSerialNo, GetTWProductSerialNo().c_str(), sizeof(debit_para.TWProductSerialNo) - 1);
        strcpy(debit_para.Version, "1.5");
        debit_para.PackageType = 3;
        debit_para.DebitType = 3;
        strncpy(debit_para.NetPayRecordMatchCode, GetNetPayRecordMatchCode(debit_para).c_str(), sizeof(debit_para.NetPayRecordMatchCode) - 1);

        bool use_debit_check = false; //开始使用查询模式，而非完整包请求模式
        for (int i = 0; i < 99; ++i)
        {
            if(IsCancled())
            {
                TW_TRACE_DIARY_TYPE("支付已经取消:" +std::string(debit_para.PayIdentifier), TW_TRACE_TPPE_PAYHTTP);    
                return -1;
            }
			std::string pay_json;
            if(use_debit_check)
            {
                TW_TRACE_DIARY_TYPE("查询支付请求,支付码:" + std::string(debit_para.PayIdentifier), TW_TRACE_TPPE_PAYHTTP);
                PackDebitCheckHttpJson(debit_para, pay_json);    
            }
            else
            {
                TW_TRACE_DIARY_TYPE("开始发送支付请求,支付码:" + std::string(debit_para.PayIdentifier), TW_TRACE_TPPE_PAYHTTP);
                PackDebitHttpJson(debit_para, pay_json);    
            }
            std::string utf8_json_str = StringConvertByCodePage(pay_json, CP_ACP, CP_UTF8);
            ++debit_para.DealSN;
            std::string recv_str;



			static unsigned short changeIP = 0;
			//默认使用第一个IP，当需要改变时，切换另一个IP进行支付 ---- 2021-11-23
			m_HttpPipe.ChangeRemotePeer((changeIP%2)?GetSecondPayIP():GetPayIP(), GetPayPort());
			char sLog[1024] = {0};
			sprintf_s(sLog, "发送请求地址:  http://%s:%d/pay",m_HttpPipe.m_IpStr.c_str(),m_HttpPipe.m_Port);
			TW_TRACE_DIARY_TYPE( std::string(sLog), TW_TRACE_TPPE_PAYHTTP);



            if(debit_para.IsUnipay)
            {//对于银联支付，单独设置超时时间
                HttpOption http_option;
                http_option.ReceveTimeoutMsecs = 15*1000;
                http_option.ConnectTimeoutMsecs = 16*1000;
                http_option.SendTimeoutMsecs   = 30*1000;
                m_HttpPipe.SetHttpOption(http_option);
            }
            else
            {
                HttpOption http_option;
                http_option.ConnectTimeoutMsecs = 2*1000;
                http_option.SendTimeoutMsecs = 6*1000;
                http_option.ReceveTimeoutMsecs = 30*1000;
                m_HttpPipe.SetHttpOption(http_option);
            }
            if (!m_HttpPipe.Post("/pay", utf8_json_str, recv_str))
            {
				//当前ip端口发送请求失败，在此处决定切换ip
				changeIP++;
                TW_TRACE_DIARY_TYPE("发送支付请求失败:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TPPE_PAYHTTP);
            }
            else
            {
                std::string gbk_recv_str = StringConvertByCodePage(recv_str, CP_UTF8, CP_ACP);
                DebitResultParameter tmp_debit_result;
                if (!ParseDebitResultFromJson(gbk_recv_str, tmp_debit_result))
                {
                    SetPayState(255);
                    TW_TRACE_DIARY_TYPE("解析服务器回馈失败:" + gbk_recv_str, TW_TRACE_TPPE_PAYHTTP);

					//当前ip端口发送请求失败，在此处决定切换ip
					changeIP++;

                    break;
                }
                m_ServerResponse = gbk_recv_str;
                if(tmp_debit_result.ServerResponseCode == HTTP_RC_DEBIT_SUCCESS)
                {
                    TW_TRACE_DIARY_TYPE("服务器返回扣费成功" , TW_TRACE_TPPE_PAYHTTP);
                    SetPayState(0);
                    break;
                }
                else if (tmp_debit_result.ServerResponseCode == HTTP_RC_DEBIT_QUERY_ERROR
                    || tmp_debit_result.ServerResponseCode == HTTP_RC_PAY_STATUS_UNKNOW)
                {
                }
                else if (tmp_debit_result.ServerResponseCode == HTTP_RC_DEBIT_NEED_PASSWORD)
                {
                    use_debit_check = true;
                    SetPayState(1);
                }
                else
                {
                    TW_TRACE_DIARY_TYPE("服务器返回失败:"+ Int2Str(tmp_debit_result.ServerResponseCode), TW_TRACE_TPPE_PAYHTTP);
                    TW_TRACE_DIARY_TYPE("服务器返回失败内容:"+ gbk_recv_str, TW_TRACE_TPPE_PAYHTTP);
                    SetPayState(ServerPayCode2LocalCode(tmp_debit_result.ServerResponseCode));
                    break;
                }

            }
            if (i >= 3)
            {
                Sleep(3 * 1000);
            }
            else
            {
                Sleep(50);
            }
        }
    }
	return 0;
}
int PayHttpSync::StartPay(DebitParameter& pay_para)
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    m_DebitGroup.push_back(pay_para);
    return 0;
}
int PayHttpSync::PayState(std::string& recv_str)
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    recv_str = m_ServerResponse;
    return m_PayState;
}
bool PayHttpSync::IsIdle()
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    return m_DebitGroup.empty();
}

int PayHttpSync::Cancle()
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    m_IsCancled = true;
    return 0;
}

bool PayHttpSync::IsCancled()
{
    trawe::CScopeLock access_lock(&m_AccessLock);
    return m_IsCancled ;
}