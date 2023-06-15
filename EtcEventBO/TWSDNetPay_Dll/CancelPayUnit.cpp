#include "stdafx.h"
#include "CancelPayUnit.h"
#include "RunParameter.h"
#include "SystemDiary.h"
#include "BaseDefine.h"
#include "TWSDNetPay_Dll.h"
#include "JsonHelper.h"
#include "TypeHelper.h"
#include "DataBaseHelper.h"
#include "SimpleUtil.h"
using namespace trawe;

CCancelPayUnit::CCancelPayUnit()
{
	::InitializeCriticalSection(&m_PayLock);

	m_HttpPipe.ChangeRemotePeer(GetPayIP(), GetPayPort());
}
CCancelPayUnit::~CCancelPayUnit()
{
	Stop();
	::DeleteCriticalSection(&m_PayLock);
}
void CCancelPayUnit::RunBody()
{	
	//处理撤单
	ExecuteOneCancle();

	Sleep(10);
}

//////////////////////////////////////////////////////////////
// 函数名称:CCancelPayUnit::ExecuteOneCancle
//
// 功能描述:执行一笔撤单
//
// 输入参数:
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2019-01-17
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
// 以下情况会通知车道撤单结果
// 1、由车道主动发起的撤单，并且收到了云端明确的回馈（成功或失败）
// 以下情况不会通知车道撤单结果
// 1、中间件由于逻辑，自动调用的撤单操作
// 2、由车道主动发起的撤单操作，但是由于网络问题，没有收到云端的回馈。此撤单会保存起来，以后发送.
//
//////////////////////////////////////////////////////////////
int CCancelPayUnit::ExecuteOneCancle()
{
	  DebitCancleParameter debit_cancle;
    {
        trawe::CScopeLock pay_lock(&m_PayLock);
        if (m_DebitCancleGroup.empty())
        {
            GetDebitCancleFromDataBase();
        }
        if (m_DebitCancleGroup.empty())
        {   //无撤单数据，需要发送
            return 0;
        }
        debit_cancle = m_DebitCancleGroup.front();
        m_DebitCancleGroup.pop_front();
    }

	static unsigned short changeIP = 0;
	//默认使用第一个IP，当需要改变时，切换另一个IP进行支付 ---- 2021-11-23
	m_HttpPipe.ChangeRemotePeer((changeIP%2)?GetSecondPayIP():GetPayIP(), GetPayPort());
	char sLog[1024] = {0};
	sprintf_s(sLog,"发送请求地址:  http://%s:%d/pay/reverse",m_HttpPipe.m_IpStr.c_str(),m_HttpPipe.m_Port);
	TW_TRACE_DIARY_TYPE( std::string(sLog), TW_TRACE_TPPE_PAYHTTP);



    // 开始进行撤单
    TW_TRACE_DIARY_TYPE("发起撤单请求，识别码:" + std::string(debit_cancle.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
    std::string cancle_json;
    PackDebitCancleHttpJson(debit_cancle, cancle_json);
    std::string utf8_json_str = StringConvertByCodePage(cancle_json, CP_ACP, CP_UTF8);
    std::string recv_str;
    if (!m_HttpPipe.Post("/pay/reverse", utf8_json_str, recv_str))
    {
		changeIP++;
        TW_TRACE_DIARY_TYPE("发送撤单请求失败:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TPPE_PAYHTTP);
        return -1;
    }
    std::string gbk_recv_str = StringConvertByCodePage(recv_str, CP_UTF8, CP_ACP);
    DebitCancleResultParameter cancle_result;
    if (!ParseDebitCancleResultFromServer(gbk_recv_str, cancle_result ))
    {
		changeIP++;
        TW_TRACE_DIARY_TYPE("解析服务器撤单回馈失败:" + gbk_recv_str, TW_TRACE_TPPE_PAYHTTP);
        return -1;
    }
    bool is_same_pay = (stricmp(debit_cancle.PayIdentifier, cancle_result.PayIdentifier) == 0);
    if (!is_same_pay)
    {
        char err_log[200];
        _snprintf(err_log, 120, "收到服务器撤单回馈，但识别码不一致，请求:%s,回馈:%s", debit_cancle.PayIdentifier, cancle_result.PayIdentifier);
        TW_TRACE_DIARY_TYPE(err_log , TW_TRACE_TPPE_PAYHTTP);
        TW_TRACE_DIARY_TYPE("错误回馈为:" + gbk_recv_str , TW_TRACE_TPPE_PAYHTTP);
        return -1;
    }

	//如果返回的状态码表明该订单并没有实际支付，则可以删除该数据，不必等待撤单数据失去时效再删除。---2021-11-17 lhq
	if (cancle_result.Status == 13)
	{
		TW_TRACE_DIARY_TYPE("撤单失败:" + gbk_recv_str , TW_TRACE_TPPE_PAYHTTP);
		//删除数据库中的数据。
		if (DeleteDebitCancleFromDataBase(debit_cancle) == 0)
		{
			TW_TRACE_DIARY_TYPE("从撤单缓存中删除记录成功，识别码:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
		}
		else
		{
			TW_TRACE_DIARY_TYPE("从撤单缓存中删除记录失败，识别码:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
		}

		return -1;
	}

	else if (cancle_result.Status != 1)
    {
        TW_TRACE_DIARY_TYPE("撤单失败:" + gbk_recv_str , TW_TRACE_TPPE_PAYHTTP);
        if (debit_cancle.EnableNotification)
        {
            NotifyResult(2, 255);
        }
        return -1;
    }
    TW_TRACE_DIARY_TYPE("撤单成功,识别码:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
    if (debit_cancle.EnableNotification)
    {
        NotifyResult(2, 0);
    }
    {   //删除数据库中的数据。
        if (DeleteDebitCancleFromDataBase(debit_cancle) == 0)
        {
            TW_TRACE_DIARY_TYPE("从撤单缓存中删除记录成功，识别码:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
        }
        else
        {
            TW_TRACE_DIARY_TYPE("从撤单缓存中删除记录失败，识别码:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
        }

    }
    return 0;

}


int CCancelPayUnit::NotifyResult(int option, int result)
{
	char info_log[100] = {0};
	sprintf(info_log, "通知调用者处理结果,功能代码:%d,执行结果:%d", option, result);
	TW_TRACE_DIARY_TYPE(info_log, TW_TRACE_TPPE_PAYUNIT);
	GetGlobalApp()->NotifyCaller(option, result);
	return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:CCancelPayUnit::GetDebitCancleFromDataBase
//
// 功能描述:从缓存数据库，获取一个撤单记录
//
// 输入参数:
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2019-01-17
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int CCancelPayUnit::GetDebitCancleFromDataBase()
{
	static time_t last_check_time = 0;
	time_t curr_time = time(NULL);
	if ( (curr_time - last_check_time) < 60 )
	{
		return 0;
	}
	last_check_time = curr_time;

	trawe::CScopeLock pay_lock(&m_PayLock);
	DebitCancleParameter debit_cancle;
	for (int i = 0; i < 5; ++i)
	{
		sqlite3* sql_conn = DataBaseOpen(GetDebitCancleCachePath());
		if (sql_conn == 0)
		{
			TW_TRACE_DIARY_TYPE("打开撤单缓存数据库失败", TW_TRACE_TPPE_PAYUNIT);
			return -1;
		}
		int db_ret = DataBaseQuery(sql_conn,  debit_cancle);
		DataBaseClose(sql_conn);
		if (db_ret != 0)
		{
			return 0;
		}
		int diff_minutes = static_cast<int>(curr_time - debit_cancle.GenerateTime) / 60;
		bool is_overdue = (diff_minutes > GetDebitCancleCacheEffectiveMinutes());
		if ( is_overdue )
		{   //删除过期数据
			char hint_log[200] = {0};
			_snprintf(hint_log, 150, "删除过期撤单缓存，识别码:%s,创建时间:%s", debit_cancle.PayIdentifier, Datetime2Str(debit_cancle.GenerateTime).c_str());
			TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_PAYUNIT);
			if (DeleteDebitCancleFromDataBase(debit_cancle) == 0)
			{
				TW_TRACE_DIARY_TYPE("删除撤单缓存成功，识别码：" + std::string(debit_cancle.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
			}
			else
			{
				TW_TRACE_DIARY_TYPE("删除撤单缓存失败，识别码：" + std::string(debit_cancle.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
			}
			continue;
		}
		else
		{
			break;
		}
	}
	char hint_log[200] = {0};
	_snprintf(hint_log, 150, "从撤单缓存获取一条数据，识别码:%s,创建时间:%s", debit_cancle.PayIdentifier, Datetime2Str(debit_cancle.GenerateTime).c_str());
	TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_PAYUNIT);
	m_DebitCancleGroup.push_back(debit_cancle);
	return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:CCancelPayUnit::DeleteDebitCancleFromDataBase
//
// 功能描述:删除指定的撤单缓存数据
//
// 输入参数:const DebitCancleParameter& dp,
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2019-01-17
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int CCancelPayUnit::DeleteDebitCancleFromDataBase(const DebitCancleParameter & debit_cancle)
{
	trawe::CScopeLock pay_lock(&m_PayLock);
	sqlite3* sql_conn = DataBaseOpen(GetDebitCancleCachePath());
	if (sql_conn == 0)
	{
		TW_TRACE_DIARY_TYPE("打开撤单缓存数据库失败" + GetDebitCancleCachePath(), TW_TRACE_TPPE_PAYUNIT);
		return -1;
	}
	int db_ret = DataBaseDelete(sql_conn,  debit_cancle);
	if (DataBaseClose(sql_conn) != 0)
	{
		TW_TRACE_DIARY_TYPE("关闭撤单缓存数据库失败" + GetDebitCancleCachePath(), TW_TRACE_TPPE_PAYUNIT);
	}
	return db_ret;
}
