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
	//������
	ExecuteOneCancle();

	Sleep(10);
}

//////////////////////////////////////////////////////////////
// ��������:CCancelPayUnit::ExecuteOneCancle
//
// ��������:ִ��һ�ʳ���
//
// �������:
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-17
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
// ���������֪ͨ�����������
// 1���ɳ�����������ĳ����������յ����ƶ���ȷ�Ļ������ɹ���ʧ�ܣ�
// �����������֪ͨ�����������
// 1���м�������߼����Զ����õĳ�������
// 2���ɳ�����������ĳ������������������������⣬û���յ��ƶ˵Ļ������˳����ᱣ���������Ժ���.
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
        {   //�޳������ݣ���Ҫ����
            return 0;
        }
        debit_cancle = m_DebitCancleGroup.front();
        m_DebitCancleGroup.pop_front();
    }

	static unsigned short changeIP = 0;
	//Ĭ��ʹ�õ�һ��IP������Ҫ�ı�ʱ���л���һ��IP����֧�� ---- 2021-11-23
	m_HttpPipe.ChangeRemotePeer((changeIP%2)?GetSecondPayIP():GetPayIP(), GetPayPort());
	char sLog[1024] = {0};
	sprintf_s(sLog,"���������ַ:  http://%s:%d/pay/reverse",m_HttpPipe.m_IpStr.c_str(),m_HttpPipe.m_Port);
	TW_TRACE_DIARY_TYPE( std::string(sLog), TW_TRACE_TPPE_PAYHTTP);



    // ��ʼ���г���
    TW_TRACE_DIARY_TYPE("���𳷵�����ʶ����:" + std::string(debit_cancle.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
    std::string cancle_json;
    PackDebitCancleHttpJson(debit_cancle, cancle_json);
    std::string utf8_json_str = StringConvertByCodePage(cancle_json, CP_ACP, CP_UTF8);
    std::string recv_str;
    if (!m_HttpPipe.Post("/pay/reverse", utf8_json_str, recv_str))
    {
		changeIP++;
        TW_TRACE_DIARY_TYPE("���ͳ�������ʧ��:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TPPE_PAYHTTP);
        return -1;
    }
    std::string gbk_recv_str = StringConvertByCodePage(recv_str, CP_UTF8, CP_ACP);
    DebitCancleResultParameter cancle_result;
    if (!ParseDebitCancleResultFromServer(gbk_recv_str, cancle_result ))
    {
		changeIP++;
        TW_TRACE_DIARY_TYPE("������������������ʧ��:" + gbk_recv_str, TW_TRACE_TPPE_PAYHTTP);
        return -1;
    }
    bool is_same_pay = (stricmp(debit_cancle.PayIdentifier, cancle_result.PayIdentifier) == 0);
    if (!is_same_pay)
    {
        char err_log[200];
        _snprintf(err_log, 120, "�յ�������������������ʶ���벻һ�£�����:%s,����:%s", debit_cancle.PayIdentifier, cancle_result.PayIdentifier);
        TW_TRACE_DIARY_TYPE(err_log , TW_TRACE_TPPE_PAYHTTP);
        TW_TRACE_DIARY_TYPE("�������Ϊ:" + gbk_recv_str , TW_TRACE_TPPE_PAYHTTP);
        return -1;
    }

	//������ص�״̬������ö�����û��ʵ��֧���������ɾ�������ݣ����صȴ���������ʧȥʱЧ��ɾ����---2021-11-17 lhq
	if (cancle_result.Status == 13)
	{
		TW_TRACE_DIARY_TYPE("����ʧ��:" + gbk_recv_str , TW_TRACE_TPPE_PAYHTTP);
		//ɾ�����ݿ��е����ݡ�
		if (DeleteDebitCancleFromDataBase(debit_cancle) == 0)
		{
			TW_TRACE_DIARY_TYPE("�ӳ���������ɾ����¼�ɹ���ʶ����:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
		}
		else
		{
			TW_TRACE_DIARY_TYPE("�ӳ���������ɾ����¼ʧ�ܣ�ʶ����:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
		}

		return -1;
	}

	else if (cancle_result.Status != 1)
    {
        TW_TRACE_DIARY_TYPE("����ʧ��:" + gbk_recv_str , TW_TRACE_TPPE_PAYHTTP);
        if (debit_cancle.EnableNotification)
        {
            NotifyResult(2, 255);
        }
        return -1;
    }
    TW_TRACE_DIARY_TYPE("�����ɹ�,ʶ����:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
    if (debit_cancle.EnableNotification)
    {
        NotifyResult(2, 0);
    }
    {   //ɾ�����ݿ��е����ݡ�
        if (DeleteDebitCancleFromDataBase(debit_cancle) == 0)
        {
            TW_TRACE_DIARY_TYPE("�ӳ���������ɾ����¼�ɹ���ʶ����:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
        }
        else
        {
            TW_TRACE_DIARY_TYPE("�ӳ���������ɾ����¼ʧ�ܣ�ʶ����:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
        }

    }
    return 0;

}


int CCancelPayUnit::NotifyResult(int option, int result)
{
	char info_log[100] = {0};
	sprintf(info_log, "֪ͨ�����ߴ�����,���ܴ���:%d,ִ�н��:%d", option, result);
	TW_TRACE_DIARY_TYPE(info_log, TW_TRACE_TPPE_PAYUNIT);
	GetGlobalApp()->NotifyCaller(option, result);
	return 0;
}

//////////////////////////////////////////////////////////////
// ��������:CCancelPayUnit::GetDebitCancleFromDataBase
//
// ��������:�ӻ������ݿ⣬��ȡһ��������¼
//
// �������:
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-17
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
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
			TW_TRACE_DIARY_TYPE("�򿪳����������ݿ�ʧ��", TW_TRACE_TPPE_PAYUNIT);
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
		{   //ɾ����������
			char hint_log[200] = {0};
			_snprintf(hint_log, 150, "ɾ�����ڳ������棬ʶ����:%s,����ʱ��:%s", debit_cancle.PayIdentifier, Datetime2Str(debit_cancle.GenerateTime).c_str());
			TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_PAYUNIT);
			if (DeleteDebitCancleFromDataBase(debit_cancle) == 0)
			{
				TW_TRACE_DIARY_TYPE("ɾ����������ɹ���ʶ���룺" + std::string(debit_cancle.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
			}
			else
			{
				TW_TRACE_DIARY_TYPE("ɾ����������ʧ�ܣ�ʶ���룺" + std::string(debit_cancle.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
			}
			continue;
		}
		else
		{
			break;
		}
	}
	char hint_log[200] = {0};
	_snprintf(hint_log, 150, "�ӳ��������ȡһ�����ݣ�ʶ����:%s,����ʱ��:%s", debit_cancle.PayIdentifier, Datetime2Str(debit_cancle.GenerateTime).c_str());
	TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_PAYUNIT);
	m_DebitCancleGroup.push_back(debit_cancle);
	return 0;
}

//////////////////////////////////////////////////////////////
// ��������:CCancelPayUnit::DeleteDebitCancleFromDataBase
//
// ��������:ɾ��ָ���ĳ�����������
//
// �������:const DebitCancleParameter& dp,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-17
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int CCancelPayUnit::DeleteDebitCancleFromDataBase(const DebitCancleParameter & debit_cancle)
{
	trawe::CScopeLock pay_lock(&m_PayLock);
	sqlite3* sql_conn = DataBaseOpen(GetDebitCancleCachePath());
	if (sql_conn == 0)
	{
		TW_TRACE_DIARY_TYPE("�򿪳����������ݿ�ʧ��" + GetDebitCancleCachePath(), TW_TRACE_TPPE_PAYUNIT);
		return -1;
	}
	int db_ret = DataBaseDelete(sql_conn,  debit_cancle);
	if (DataBaseClose(sql_conn) != 0)
	{
		TW_TRACE_DIARY_TYPE("�رճ����������ݿ�ʧ��" + GetDebitCancleCachePath(), TW_TRACE_TPPE_PAYUNIT);
	}
	return db_ret;
}
