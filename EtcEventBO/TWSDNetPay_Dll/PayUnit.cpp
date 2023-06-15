#include "stdafx.h"
#include "PayUnit.h"
#include "RunParameter.h"
#include "SystemDiary.h"
#include "BaseDefine.h"
#include "TWSDNetPay_Dll.h"
#include "JsonHelper.h"
#include "TypeHelper.h"
#include "DataBaseHelper.h"
#include "SimpleUtil.h"
using namespace trawe;

CPayUnit::CPayUnit()
{
    ::InitializeCriticalSection(&m_PayLock);
    ::InitializeCriticalSection(&m_PayResultLock);
    ::InitializeCriticalSection(&m_CertificateLock);

    m_HttpPipe.ChangeRemotePeer(GetPayIP(), GetPayPort());
}
CPayUnit::~CPayUnit()
{
    Stop();
    m_AgencyPay.Destroy();

    for (std::list<PayHttpSync*>::iterator it = m_AbandonPayGroup.begin(); it != m_AbandonPayGroup.end(); ++it)
    {
        if ( (*it)->IsIdle() )
        {
            delete (*it);
        }
    }
    m_AbandonPayGroup.clear();


    ::DeleteCriticalSection(&m_PayLock);
    ::DeleteCriticalSection(&m_PayResultLock);
    ::DeleteCriticalSection(&m_CertificateLock);
}
void CPayUnit::RunBody()
{	
    //����֧��
    ExecuteOnePay();
    //������ ,Ǩ�Ƶ�һ�������Ĵ���Ԫ CancelPayUnit   ---- 2021-11-19
   // ExecuteOneCancle();
    ClearAbandonPay();
    {
        trawe::CScopeLock pay_lock(&m_PayLock);
        if (m_DebitGroup.empty())
        {
            GetGlobalApp()->m_bIsTrading = FALSE;
        }
    }
    Sleep(10);
}

int CPayUnit::InitAgencyPay(const char* szAreaInfo, const char* szLoaclStation,  const char* szLoaclLaneID,  const char* szServerInfo,  const int& iProvinceID)
{
    trawe::CScopeLock pay_lock(&m_PayLock);
    if (!IsUseAgencyDll())
    {
        TW_TRACE_DIARY("δ���ô��ۿ�" + GetAgencyDllPath());
        return -1;
    }

    if (!m_AgencyPay.Init( GetAgencyDllPath().c_str() ))
    {
        TW_TRACE_DIARY("��ʼ�����ۿ�ʧ�ܣ���ַ:" + GetAgencyDllPath());
        return -1;
    }
    TW_TRACE_DIARY("���ش��ۿ�ɹ�");
    m_AgencyPay.InitEnvironment(szAreaInfo, szLoaclStation, szLoaclLaneID, szServerInfo, iProvinceID);
    return 0;
}
int CPayUnit::StartNewPay(const DebitParameter&dp)
{
    {
        trawe::CScopeLock pay_lock(&m_PayLock);
        bool is_idle = (m_DebitGroup.size() == 0);
        if (!is_idle)
        {
            TW_TRACE_DIARY("֧�����ڽ�����,�����ظ�֧��");
            return -1;
        }
    }
    {
        trawe::CScopeLock pay_lock(&m_PayLock);
        m_DebitGroup.push_back(dp);
    }
    return 0;
}
int CPayUnit::QueryCurrentPayState(std::string& pay_id, int& state)
{
    return -1; // �ݲ�֧��
}

//////////////////////////////////////////////////////////////
// ��������:CPayUnit::CancelTargetPay
//
// ��������:����ָ���Ľ���
//
// �������:const DebitCancleParameter& dp,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-17
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:�˴��Ὣ���ݱ��浽���ݿ�
//
//////////////////////////////////////////////////////////////
int CPayUnit::CancelTargetPay(const DebitCancleParameter& dp)
{
    {   //�ж�ȡ�����Ƿ��ǵ�ǰ���ס�
        trawe::CScopeLock pay_lock(&m_PayLock);
        bool is_idle = (m_DebitGroup.size() == 0);
        if (!is_idle && dp.EnableNotification) //�������ⲿ�������õĳ���
        {
            DebitParameter& debit_para = m_DebitGroup.front();
            bool is_same_id = (stricmp(dp.PayIdentifier, debit_para.PayIdentifier) == 0);
            if (is_same_id)
            {
                TW_TRACE_DIARY_TYPE("�жϴ˽��ף�����ʶ����:" + std::string(dp.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
                debit_para.IsCancled = true;
            }
            else
            {
                //TW_DEBUG_VARIABLE(dp.PayIdentifier);
                //TW_DEBUG_VARIABLE(debit_para.PayIdentifier);
            }
        }
    }
    {
        trawe::CScopeLock pay_lock(&m_PayLock);
        m_DebitCancleGroup.push_back(dp);
        {   //�����ݱ��浽����
            std::string cancle_cache_path = GetDebitCancleCachePath();
            sqlite3* sql_conn = DataBaseOpen(cancle_cache_path);
            if (sql_conn == 0)
            {
                TW_TRACE_DIARY_TYPE("�����������ݿ��ʧ��:" + cancle_cache_path, TW_TRACE_TPPE_PAYUNIT);
                return -1;
            }
            if (
                (DataBaseCreateTable(sql_conn, DataBaseGetDebitCancleTableName(), DataBaseGetDebitCancleTableDefine()) != 0 )
                || (DataBaseAdd(sql_conn, dp) != 0)
            )
            {
                TW_TRACE_DIARY_TYPE("�������ݲ������ݿ�ʧ�ܣ�ʶ����:" + std::string(dp.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
            }
            else
            {
                TW_TRACE_DIARY_TYPE("�������ݲ������ݿ�ɹ���ʶ���룺" + std::string(dp.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
            }
            if (DataBaseClose(sql_conn) != 0)
            {
                TW_TRACE_DIARY_TYPE("�������ݿ�ر�ʧ��:" + cancle_cache_path, TW_TRACE_TPPE_PAYUNIT);
            }
        }
    }
    return 0;

}
int CPayUnit::GetCurrentDebitResult(DebitResultParameter& dp)
{
    {
        trawe::CScopeLock result_lock(&m_PayResultLock);
        if (m_DebitResultGroup.empty())
        {
            TW_TRACE_DIARY_TYPE("��֧��������Ի�ȡ", TW_TRACE_TPPE_PAYUNIT);
            return -1;
        }
        dp = m_DebitResultGroup.front();
        m_DebitResultGroup.pop_front();
        return 0;
    }
}
void CPayUnit::ExecuteOnePay()
{

    DebitParameter dp;
    DebitResultParameter debit_result;
    {
        trawe::CScopeLock pay_lock(&m_PayLock);
        if (m_DebitGroup.empty())
        {
            return ;
        }
        dp = m_DebitGroup.front();
    }
    char hint_log[1000] = {0};
    _snprintf(hint_log, 800, "��ʼ����,ʶ����:%s", dp.PayIdentifier);
    TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_PAYUNIT);
    bool debit_success = false;
    int plate_pay_result = ExecutePlatePay(dp, debit_result);
    _snprintf(hint_log, 800, "����ִ����:%d", plate_pay_result);
    TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_PAYUNIT);



    if (plate_pay_result == 0)
    {
        debit_success = true;
    }
    else if (plate_pay_result == 1)//��Ҫ���г���
    {
        DebitCancleParameter dcp ;
        dcp.ProvinceID = dp.LaneConfig.ProvinceID;
        strcpy(dcp.Version, "1.0");
        strncpy(dcp.PayIdentifier, dp.PayIdentifier, sizeof(dcp.PayIdentifier) - 1);
        CancelTargetPay(dcp);
        NotifyResult(1, RCC_PAY_PLATFORM_ERROR);
        HintPayResult(-1, 0);
        trawe::CScopeLock pay_lock(&m_PayLock);
        m_DebitGroup.clear();
        return ;
    }
    if (!debit_success)
    {   //ʹ�ö�ά�룬����֧��
        int code_pay_result = ExecuteCodePay(dp, debit_result);
        if (code_pay_result == 0)
        {
            debit_success = true;
        }
        else if (code_pay_result == 1)//��Ҫ���г���
        {
            DebitCancleParameter dcp ;
            dcp.ProvinceID = dp.LaneConfig.ProvinceID;
            strcpy(dcp.Version, "1.0");
            strncpy(dcp.PayIdentifier, dp.PayIdentifier, sizeof(dcp.PayIdentifier) - 1);
            CancelTargetPay(dcp);
        }

    }
    bool cancle_this_debit = false;
    bool is_result_valid = true;
    if (debit_success)
    {
        char err_log[1000] = {0};
        trawe::CScopeLock pay_lock(&m_PayLock);
        if (m_DebitGroup.empty())
        {
            is_result_valid = false;
            cancle_this_debit = true;
            _snprintf(err_log, 800, "�ۿ�ɹ�,ʶ����:%s,���޷��ҵ�����Դ,�������ν���", debit_result.PayIdentifier);
            TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TPPE_PAYUNIT);
        }
        else
        {
            const DebitParameter& src_para = m_DebitGroup.front();
            bool is_same_id = (stricmp(src_para.PayIdentifier, debit_result.PayIdentifier) == 0);
            if (!is_same_id)
            {
                is_result_valid = false;
                cancle_this_debit = true;
                _snprintf(err_log, 800, "�ۿ�ɹ�,Դʶ����:%s,����ʶ���룺%s,Դ����������������벻һ��,�������ν���", debit_result.PayIdentifier, debit_result.PayIdentifier);
                TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TPPE_PAYUNIT);
            }
            else if (src_para.IsCancled)
            {
                is_result_valid = false;
                _snprintf(err_log, 800, "�ۿ�ɹ�,ʶ����:%s���ý����Ѿ�����������Ϊ����ʧ��", debit_result.PayIdentifier);
                TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TPPE_PAYUNIT);
            }
        }
    }
    if (debit_success && is_result_valid)
    {
        AdjustPayPlatformType(debit_result);
        GetGlobalApp()->LPR_PackPayResult(debit_result.ServerPayPlatformType);
        NewPayResult(debit_result);
        //HintPayResult(0, debit_result.ServerPayPlatformType);
        NotifyResult(1, 0);
    }
    else
    {   //֧��ʧ��
        if (IsTianJinArea())
        {
            NewPayResult(debit_result);
        }
        if (IsTianJinArea() &&
                (dp.PayModel == 2))//�����������Ľ����Ƹ����⴦��
        {
            HintPayResult(-1, 0);
            int err_code = RCC_AGENT_DEBIT_ERROR;
            if (debit_result.AgencyExcuteCode == 26)
            {
                err_code = RCC_AGENT_DEBIT_EXCESS_ERROR;
            }
            else if (debit_result.AgencyExcuteCode == 27)
            {
                err_code = RCC_AGENT_DEBIT_RISK_ERROR;
            }
            NotifyResult(1, err_code);
        }
        else
        {
            NotifyResult(1, ServerPayCode2LocalCode(debit_result.ServerResponseCode));
            //HintPayResult(-1, debit_result.ServerResponseCode);
        }
        if (dp.IsCancled) //������ױ��ⲿ��������������Ҫ����
        {
            ClearScreen();
        }
    }

    {
        trawe::CScopeLock pay_lock(&m_PayLock);
        m_DebitGroup.clear();
    }

    if (cancle_this_debit)//�˱ʽ�����Ҫ����
    {
        DebitCancleParameter dcp ;
        dcp.ProvinceID = dp.LaneConfig.ProvinceID;
        strcpy(dcp.Version, "1.0");
        strncpy(dcp.PayIdentifier, dp.PayIdentifier, sizeof(dcp.PayIdentifier) - 1);
        CancelTargetPay(dcp);
    }
}

//////////////////////////////////////////////////////////////
// ��������:CPayUnit::ExecutePlatePay
//
// ��������:���ó��Ƹ�
//
// �������:const DebitParameter& debit_para, DebitResultParameter& debit_result,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-17
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
// ����˵��:
// 0:�ۿ�ɹ�
// ����ֵ���ۿ�ʧ�ܣ�����1��ʾ��Ҫ����
//
//////////////////////////////////////////////////////////////
int CPayUnit::ExecutePlatePay(const DebitParameter& debit_para, DebitResultParameter& debit_result)
{

    {   // check
        //�ж��Ƿ����ô��۶�̬��
        if (!IsUseAgencyDll())
        {
            TW_TRACE_DIARY_TYPE("δ���ô���,�����д��۴���", TW_TRACE_TPPE_PAYUNIT);
            return -1;
        }
        bool is_disable_agency = (debit_para.PayModel == 1);
        if (is_disable_agency)
        {
            TW_TRACE_DIARY_TYPE("֧��ģʽ��ʹ�ô��ۣ������������۴���", TW_TRACE_TPPE_PAYUNIT);
            return -1;
        }
        bool is_valid_plate = strlen(debit_para.VehicleLicense) >= 7 ;
        if (!is_valid_plate )
        {
            return -1;
        }
    }
    BYTE byFunCode      = 0x01;
    BYTE byExecuteState = RCC_INNER_ERROR;

    std::string agency_xml_str;
    PackDebitAgencyXML(debit_para, agency_xml_str);
    int     nAgencyDebitLength = static_cast<int>(agency_xml_str.size());
    char    szAgencyResult[2048] = {0};

    int iIsCancelAnency = 0x00;
    int iAnencyResult = 0x00;
    DWORD dwStartDebitTime = ::GetTickCount();
    iAnencyResult = m_AgencyPay.AgencyDebitMoney( agency_xml_str.c_str(), szAgencyResult, nAgencyDebitLength, 1);
    debit_result.ConsumeTimeMsec = ::GetTickCount() - dwStartDebitTime;
    ParseDebitAgencyETCDataFromXML(szAgencyResult, debit_result, debit_para.PayIdentifier);

    debit_result.AgencyExcuteCode = iAnencyResult;
    NotifyPayStatus(1, iAnencyResult);
    if ( 0 == iAnencyResult )//֧���ɹ�
    {
        if (!ParseDebitAgencyResultFromXML(szAgencyResult, debit_result))
        {
            return 1;
        }
        return 0;
    }
    else if (iAnencyResult == 25) //����ǳ��Ͳ���
    {
        debit_result.ExecuteCode = 10;
    }
    else
    {
        //�жϷ��ش���ȷ���Ƿ���Ҫ������۳���
        switch (iAnencyResult)
        {
        case 6:
        case 12:
            return 1;
            break;
        }
    }
    return -1;

}

/**
 * ����ɨ�롢����֧��
 * @param  debit_para
 * @param  debit_result
 * @return
 * 0:��ʾ�ɹ�
 * ����ֵ����ʾʧ�ܣ�����1��ʾ��Ҫ����
 */
int CPayUnit::ExecuteCodePay(DebitParameter& debit_para, DebitResultParameter& debit_result)
{
    bool is_disable_code_pay = (debit_para.PayModel == 2);
    if (is_disable_code_pay)
    {
        TW_TRACE_DIARY_TYPE("֧��ģʽ��ʹ��ɨ�룬����ɨ�봦��", TW_TRACE_TPPE_PAYUNIT);
        return -1;
    }

	if(GetGlobalApp()->m_iCurrentWorkMode == 1)
	{
		//���÷���
		SetFeeDisplay(debit_para);
	}

    SetCertificate("");//����ƾ֤
    TW_TRACE_DIARY_TYPE("��ʼ��ȡ֧��ƾ֤", TW_TRACE_TPPE_PAYUNIT);
    int countdown_msecs = 0 ;
    std::string pay_json;
    {   //��ȡ֧��ƾ֤
        if (OpenCertificateScanner(debit_para) != 0)
        {
            TW_TRACE_DIARY_TYPE("��ɨ����ʧ�ܣ�����ʧ��", TW_TRACE_TPPE_PAYUNIT);
            return -1;
        }
        int ret_code = GetCertificate(debit_para);
        if (ret_code != 0)
        {
            debit_result.TraceInfo.ErrorCode = ret_code;
			if(GetGlobalApp()->m_iCurrentWorkMode == 1)
			{
				SetPayResultDisplay(debit_para.PayIdentifier, 1, "��ȡ֧��ƾ֤ʧ�ܣ�");
			}
            return -1;
        }
        TW_TRACE_DIARY_TYPE("��ȡ��֧��ƾ֤�ɹ�", TW_TRACE_TPPE_PAYUNIT);
    }
    {
        TW_TRACE_DIARY_TYPE("����֧�����󵽷�����", TW_TRACE_TPPE_PAYUNIT);
        //������������
        PayHttpSync * phs = new PayHttpSync();
        if (phs == 0)
        {
            TW_TRACE_DIARY_TYPE("����֧����������ʧ��", TW_TRACE_TPPE_PAYUNIT);
            return -1;
        }
        phs->StartPay(debit_para);
        phs->Resume();
        {
            trawe::CScopeLock pay_lock(&m_PayLock);
            m_AbandonPayGroup.push_back(phs);
        }
        Sleep(50);
        std::string result_str;
        //const int countdown_msecs = debit_para.OverTime - static_cast<int>(ChinaTime() - debit_para.SubTime) * 1000;
		const int countdown_msecs = 30000;
        const int check_duration_msecs = 100;
		Util::CountDown debit_cd(countdown_msecs);
		while (!debit_cd.Over())
        {
            Sleep(check_duration_msecs);
            if (IsTargetPayCancled(debit_para.PayIdentifier))
            {   //�ý����Ѿ����ж�
                debit_para.IsCancled = true;
                TW_TRACE_DIARY_TYPE("���ױ�ȡ��������ʶ����:" + std::string(debit_para.PayIdentifier), TW_TRACE_TPPE_PAYUNIT);
                return 255;
            }
            int pay_state = phs->PayState(result_str);
            if (pay_state == 1)
            {   //������������
                //HintPayResult(1, 0);
                continue;
            }
            else if (pay_state == -1)
            {   //״̬δ֪
                continue;
            }

			TW_TRACE_DIARY("֧��ƽ̨��������:" + result_str);
            if (!ParseDebitResultFromJson(result_str, debit_result))
            {
                TW_TRACE_DIARY_TYPE("�����������������ʧ��", TW_TRACE_TPPE_PAYUNIT);
				if(GetGlobalApp()->m_iCurrentWorkMode == 1)
				{
					SetPayResultDisplay(debit_para.PayIdentifier, 1, "֧��ƽ̨�����������");
				}
                return 1;
            }
            else
            {
                if (pay_state == 0)
                {   //֧���ɹ�
                    TW_TRACE_DIARY_TYPE("�յ�������������֧���ɹ�", TW_TRACE_TPPE_PAYUNIT);
					if(GetGlobalApp()->m_iCurrentWorkMode == 1)
					{
						SetPayResultDisplay(debit_para.PayIdentifier, 0, "֧���ɹ�");
					}
                    return 0;
                }
                else
                {   //֧��ʧ��
                    char err_log[100] = {0};
                    sprintf(err_log, "�յ�������������֧��ʧ��(������:%d)", debit_result.ServerResponseCode);
                    TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TPPE_PAYUNIT);
					if(GetGlobalApp()->m_iCurrentWorkMode == 1)
					{
						SetPayResultDisplay(debit_para.PayIdentifier, 1, "֧��ƽ̨�����۷�ʧ��");
					}
                    return 255;
                }
            }

        }

        if (debit_cd.Over())//��ʱû�еõ�����Ĵ���
        {
            phs->Cancle();
            TW_TRACE_DIARY_TYPE("�ۿʱ", TW_TRACE_TPPE_PAYUNIT);
			if(GetGlobalApp()->m_iCurrentWorkMode == 1)
			{
				SetPayResultDisplay(debit_para.PayIdentifier, 1, "֧��ƽ̨Ӧ��ʱ");
			}
            return 1;
        }

    }
    return -1;
}



//////////////////////////////////////////////////////////////
// ��������:CPayUnit::ExecuteOneCancle
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
int CPayUnit::ExecuteOneCancle()
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
    if (cancle_result.Status != 1)
    {
        TW_TRACE_DIARY_TYPE("����ʧ��:" + gbk_recv_str , TW_TRACE_TPPE_PAYHTTP);
        if (debit_cancle.EnableNotification)
        {
            NotifyResult(2, 255);
        }

		if (cancle_result.Status == 13) //û�ҵ�����ƾ֤ �����Ѿ�����������Ч�� ---- 2021-11-15
		{
			//ɾ�����ݿ��е����ݡ�
			if (DeleteDebitCancleFromDataBase(debit_cancle) == 0)
			{
				TW_TRACE_DIARY_TYPE("�ӳ���������ɾ����¼�ɹ���ʶ����:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
			}
			else
			{
				TW_TRACE_DIARY_TYPE("�ӳ���������ɾ����¼ʧ�ܣ�ʶ����:" + std::string(debit_cancle.PayIdentifier) , TW_TRACE_TPPE_PAYHTTP);
			}
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
int CPayUnit::ClearAbandonPay()
{
    static time_t last_check_time = time(NULL);
    time_t curr_time = time(NULL);
    bool enable_check = (curr_time - last_check_time) > 30;
    if ( !enable_check)
    {
        return -1;
    }
    last_check_time = curr_time;
    trawe::CScopeLock pay_lock(&m_PayLock);
    std::list<PayHttpSync*> under_way_pay;
    for (std::list<PayHttpSync*>::iterator it = m_AbandonPayGroup.begin(); it != m_AbandonPayGroup.end(); ++it)
    {
        if ( (*it)->IsIdle() )
        {
            delete (*it);
        }
        else
        {
            under_way_pay.push_back(*it);
        }
    }
    m_AbandonPayGroup = under_way_pay;
    int abandon_size = static_cast<int>(m_AbandonPayGroup.size());
    if (abandon_size > 0)
    {
        char info_log[100] = {0};
        sprintf(info_log, "����%d������֧������", abandon_size);
        TW_TRACE_DIARY_TYPE(info_log, TW_TRACE_TPPE_PAYUNIT);
    }
    return 0;
}
int CPayUnit::SetCertificate(const std::string & cer)
{
    trawe::CScopeLock cer_lock(&m_CertificateLock);
    m_CertificateJson = cer;
    return 0;
}

int CPayUnit::QueryCertificate(std::string & cer)
{
    trawe::CScopeLock cer_lock(&m_CertificateLock);
    if (m_CertificateJson.empty())
    {
        return 1;
    }
    cer = m_CertificateJson;
    return 0;
}

int CPayUnit::GetCertificate(DebitParameter & debit_para)
{
    //const int countdown_msecs = debit_para.OverTime - static_cast<int>(ChinaTime() - debit_para.SubTime) * 1000;
	const int countdown_msecs = debit_para.OverTime;
	TW_TRACE_DIARY_TYPE("��ȡ֧��ƾ֤�ȴ�ʱ��(ms):"+ Int2Str(countdown_msecs), TW_TRACE_TPPE_PAYUNIT);

	Util::CountDown debit_cd(countdown_msecs);
    const int  check_duration_msecs = 50;
    std::string pay_certificate;
	while (!debit_cd.Over())
    {
        if (QueryCertificate(pay_certificate) == 0)
        {
            break;
        }
        {
            trawe::CScopeLock pay_lock(&m_PayLock);
            if (m_DebitGroup.empty())
            {
                TW_TRACE_DIARY_TYPE("�޷��ҵ�����Դ�����ٻ�ȡ֧��ƾ֤", TW_TRACE_TPPE_PAYUNIT);
                return -1;
            }
            if (m_DebitGroup.front().IsCancled)
            {
                debit_para.IsCancled = true;
                TW_TRACE_DIARY_TYPE("ȡ�����ף����ٻ�ȡ֧��ƾ֤", TW_TRACE_TPPE_PAYUNIT);
                return -1;
            }
        }
        Sleep(check_duration_msecs);
    }
	TW_TRACE_DIARY_TYPE("�ȴ���ȡ֧��ƾ֤����", TW_TRACE_TPPE_PAYUNIT);
    if (pay_certificate.empty())
    {
        TW_TRACE_DIARY_TYPE("��ȡ��ʱ�����ٻ�ȡ֧��ƾ֤", TW_TRACE_TPPE_PAYUNIT);
        return 6;
    }
    Json::Value root_node;
    Json::Reader json_reader;
    if (!json_reader.parse(pay_certificate, root_node))
    {
        TW_TRACE_DIARY_TYPE("����֧��ƾ֤ʧ��:" + pay_certificate, TW_TRACE_TPPE_PAYUNIT);
        return -1;
    }
    bool read_json_success = true;
    read_json_success = read_json_success ? ReadJsonChars(root_node, "QRCode", debit_para.QRCode, sizeof(debit_para.QRCode)) : false;
    read_json_success = read_json_success ? ReadJsonChars(root_node, "NFCPayData", debit_para.NFCPayData, sizeof(debit_para.NFCPayData)) : false;
    read_json_success = read_json_success ? ReadJsonChars(root_node, "NFCRevData", debit_para.NFCRevData, sizeof(debit_para.NFCRevData)) : false;
    if (!read_json_success)
    {
        TW_TRACE_DIARY_TYPE("����֧��ƾ֤ʧ��:" + GetJsonParseError(), TW_TRACE_TPPE_PAYUNIT);
        return -1;
    }
    if (strlen(debit_para.QRCode) <= 5)
    {
        strcpy(debit_para.QRCode, "111111");

    }
    bool is_nfc_signing = (::GetGlobalApp()->GetUnipayWorkFlowState() == UMS_SIGNING);
    if (::GetGlobalApp()->m_bNFCIsUse &&  !is_nfc_signing)
    {   //��������״̬
        GetGlobalApp()->SetUnipayWorkFlowState(UMS_UNKNOW);
    }
    {   //����֧���ж�
        bool is_quickpass = (strlen(debit_para.NFCPayData) != 0);
        bool is_unipaycode = (strlen(debit_para.QRCode) == 19) && (stricmp(debit_para.QRCode, "62") == 0);
        if ( is_quickpass || is_unipaycode)
        {
            debit_para.IsUnipay = true;
        }
    }
    {   //��ԯ֧ͨ�֣�ͨ���볤�ȣ��ж��Ƿ�����ԯͨ
        bool is_xuanyuantong_code = ( strlen(debit_para.QRCode) == 731);
        if (is_xuanyuantong_code)
        {
            debit_para.AccountInfoSrcType        = 24;
            debit_para.AccountInfoDataType       = 24;
        }
    }
    return 0;
}
int CPayUnit::NotifyResult(int option, int result)
{
    char info_log[100] = {0};
    sprintf(info_log, "֪ͨ�����ߴ�����,���ܴ���:%d,ִ�н��:%d", option, result);
    TW_TRACE_DIARY_TYPE(info_log, TW_TRACE_TPPE_PAYUNIT);
    GetGlobalApp()->NotifyCaller(option, result);
    return 0;
}

//////////////////////////////////////////////////////////////
// ��������:CPayUnit::HintPayResult
//
// ��������:��ʾ�û�֧��״̬
//
// �������:bool is_success,int detail_type,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2018-12-13
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
// -1��֧��ʧ��
// 0��֧���ɹ�
// 1��������������
//
//////////////////////////////////////////////////////////////
int CPayUnit::HintPayResult(int pay_state, int detail_type)
{
    if (GetGlobalApp()->m_pThreadManage == 0
            || GetGlobalApp()->m_pThreadManage->m_pThreadDevice == 0)
    {
        TW_TRACE_DIARY("��������û�������豸���޷����н�����ʾ");
        return -1;
    }
    if (pay_state == 0)
    {
        char hint_log[100] = {0};
        sprintf(hint_log, "����֧���ɹ�����:%d", detail_type);
        TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_PAYUNIT);
        GetGlobalApp()->m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x01, (LPARAM)detail_type);
        return 0;
    }
    if (pay_state == -1)
    {
        char hint_log[100] = {0};
        int local_hint_code = GetPayFailHintCodeAccordingToServerPayCode(detail_type);
        sprintf(hint_log, "����֧��ʧ����ʾ�������״̬:%d,����״̬:%d", detail_type, local_hint_code);
        TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_PAYUNIT);
        GetGlobalApp()->m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)local_hint_code, (LPARAM)0);
        // GetGlobalApp()->m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)0x02, (LPARAM)0);
        return 0;
    }
    if (pay_state == 1)
    {   //������������
        static time_t password_hint_time = 0;
        time_t curr_time = time(NULL);
        if ( (curr_time - password_hint_time) > 10)
        {
            TW_TRACE_DIARY_TYPE("�û�������������..", TW_TRACE_TPPE_PAYUNIT);
            password_hint_time = curr_time;
            GetGlobalApp()->m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_CONTROL_DEAL_RESULT_MMI, (WPARAM)SM_PRPTOCOL_LIGHT_FALI_PSW_INPUT, (LPARAM)0x00);
        }
        return 0;
    }
    return -1;
}


void CPayUnit::AdjustPayPlatformType(DebitResultParameter & debit_result)
{
    debit_result.PayPlatformType = debit_result.ServerPayPlatformType;
    switch (debit_result.ServerPayPlatformType)
    {
    case 0x07:
        debit_result.PayPlatformType = PCTC_ALIIPAY_AGENCY;
        break;
    case 0x08:
        debit_result.PayPlatformType = PCTC_WEIXIN_AGENCY;
        break;
    case 0x09:
        debit_result.PayPlatformType = PCTC_BAIDU_AGENCY;
        break;
    case 0x0A:
        debit_result.PayPlatformType = PCTC_JD_AGENCY;
        break;
    case 0x0B:
        debit_result.PayPlatformType = PCTC_UNIPAY_AGENCY;
        break;
    case 0x11:
        debit_result.PayPlatformType = PCTC_ICBC_AGENCY;
        break;
    case 0x13:
        debit_result.PayPlatformType = PCTC_VIRTUALCARD;
        break;
    case 24:
        debit_result.PayPlatformType = PCTC_XUANYUANTONG;
        break;
    }

    if (IsUseUnifyPayChannelCode())
    {
        switch (debit_result.PayPlatformType)
        {
        case PCTC_WEIXIN_AGENCY:
            debit_result.PayPlatformType = PCTC_WENXIN;
            break;
        case PCTC_ALIIPAY_AGENCY:
            debit_result.PayPlatformType = PCTC_ALIIPAY;
            break;
        case PCTC_BAIDU_AGENCY:
            debit_result.PayPlatformType = PCTC_BAIDU;
            break;
        case PCTC_JD_AGENCY:
            debit_result.PayPlatformType = PCTC_JD;
            break;
        case PCTC_UNIPAY_AGENCY:
            debit_result.PayPlatformType = PCTC_UNIPAY_QR;
            break;
        }
    }
    else
    {
        if (IsHeNanArea())
        {
            if (debit_result.PayPlatformType == PCTC_ALIIPAY_AGENCY)
            {
                debit_result.PayPlatformType = PCTC_WEIXIN_AGENCY;
            }
            else if (debit_result.PayPlatformType == PCTC_WEIXIN_AGENCY)
            {
                debit_result.PayPlatformType = PCTC_ALIIPAY_AGENCY;
            }
        }
    }
    if ( IsJiLinArea() &&
            (debit_result.PayPlatformType == PCTC_ICBC_AGENCY)
       )
    {
        debit_result.PayPlatformType = PCTC_ALIIPAY;
    }
}
int CPayUnit::OpenCertificateScanner(const DebitParameter & debit_para)
{
    if (GetGlobalApp()->m_pThreadManage == 0
            || GetGlobalApp()->m_pThreadManage->m_pThreadDevice == 0)
    {
        TW_TRACE_DIARY("��������û�������豸���޷����н�����ʾ");
        return -1;
    }
    CMarkup xmlDebit;
    xmlDebit.SetDoc( "<?xml version=\"1.0\" encoding=\"GB2312\"?>" );
    xmlDebit.AddElem( "DisplayContext" );//root
    xmlDebit.IntoElem();

    xmlDebit.AddElem( _T("Version"),    _T("1.0") );
    xmlDebit.AddElem( _T("ProvinceID"), debit_para.LaneConfig.ProvinceID );
    xmlDebit.AddElem( _T("VerifyCode"), 0 );
    xmlDebit.AddElem( _T("Data") );

    xmlDebit.IntoElem();

    xmlDebit.AddElem( _T("Type"),       0x03);

    xmlDebit.AddElem( _T("Money"),       debit_para.Money );
    xmlDebit.AddElem( _T("Weight"),      debit_para.Weight );
    xmlDebit.AddElem( _T("VehicleType"), debit_para.VehicleType );
    xmlDebit.AddElem( _T("VehicleClass"), debit_para.VehicleClass );
    xmlDebit.AddElem( _T("CurrentTime"), CTime(ChinaTime()).Format("%Y-%m-%d %H:%M:%S") );

    xmlDebit.OutOfElem();
    xmlDebit.OutOfElem();

    CString strXMLString = xmlDebit.GetDoc();
    CString* p_str = new CString();
    if (p_str == 0)
    {
        TW_TRACE_DIARY_TYPE("��ɨ��ͷʧ�ܣ��ڴ�����ʧ��", TW_TRACE_TPPE_PAYUNIT);
        return -1;
    }
    p_str->Format(_T("%s"), strXMLString);
    GetGlobalApp()->m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_START_DEBIT_FLOW, (WPARAM)p_str, (LPARAM)0);
    return 0;
}

int CPayUnit::SetFeeDisplay(const DebitParameter & debit_para)
{
    if (GetGlobalApp()->m_pThreadManage == 0
            || GetGlobalApp()->m_pThreadManage->m_pThreadDevice == 0)
    {
        TW_TRACE_DIARY("��������û�������豸���޷����зѶ���ʾ");
        return -1;
    }

	Json::Value root;
	Json::Value ElectronicInvoiceShowFee;
	Json::Value Data;

	ElectronicInvoiceShowFee["ElectronicInvoiceNo"] = Json::Value(debit_para.PayIdentifier);
	Data["Entry"] = Json::Value(debit_para.EntryStationName);
	Data["FeeInfo"] = Json::Value(debit_para.Money);
	ElectronicInvoiceShowFee["Data"] = Data;
	root["ElectronicInvoiceShowFee"] = ElectronicInvoiceShowFee;

	//ת��Ϊ�ַ���
	CString strReturn;
	Json::FastWriter fast_writer;
	strReturn.Format(_T("%s"),fast_writer.write(root).c_str());

    CString* p_str = new CString();
    if (p_str == 0)
    {
        TW_TRACE_DIARY_TYPE("�Ѷ���ʾ���ڴ�����ʧ��", TW_TRACE_TPPE_PAYUNIT);
        return -1;
    }
    p_str->Format(_T("%s"), strReturn);
    GetGlobalApp()->m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_MANAGE_INVOICE_FEE_DISPLAY, (WPARAM)p_str, (LPARAM)0);

    return 0;
}


int CPayUnit::SetPayResultDisplay(const std::string& pay_id, int Result, std::string FailReason)
{
	if (GetGlobalApp()->m_pThreadManage == 0 || GetGlobalApp()->m_pThreadManage->m_pThreadDevice == 0)
    {
        TW_TRACE_DIARY("��������û�������豸���޷�����֧�������ʾ");
        return -1;
    }

	Json::Value root;
	Json::Value ElectronicInvoicePayResult;
	Json::Value Data;

	ElectronicInvoicePayResult["ElectronicInvoiceNo"] = Json::Value(pay_id);
	Data["PayResult"] = Json::Value(Result);
	Data["ReasonFailure"] = Json::Value(FailReason);
	ElectronicInvoicePayResult["Data"] = Data;
	root["ElectronicInvoicePayResult"] = ElectronicInvoicePayResult;

	//ת��Ϊ�ַ���
	CString strReturn;
	Json::FastWriter fast_writer;
	strReturn.Format(_T("%s"), fast_writer.write(root).c_str());

    CString* p_str = new CString();
    if (p_str == 0)
    {
        TW_TRACE_DIARY_TYPE("֧�������ʾ���ڴ�����ʧ��", TW_TRACE_TPPE_PAYUNIT);
        return -1;
    }
    p_str->Format(_T("%s"), strReturn);
    GetGlobalApp()->m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_MANAGE_INVOICE_PAY_RESULT_DISPLAY, (WPARAM)p_str, (LPARAM)0);

	return 0;
}


bool CPayUnit::QueryVirtualCardDebit(const char* szParamContext, const int& iParamSize, const int& iFormatType, int& iSupportAgent, OUT char* szResult)
{
    if (!IsUseAgencyDll())
    {
        return false;
    }
    iSupportAgent = m_AgencyPay.AgencyQueryVirtualCardDebit(szParamContext, iParamSize, iFormatType, szResult);
    return true;
}

bool CPayUnit::AgencyDebitQuery(const char* szParamContext, const int& iParamSize, const int& iFormatType, OUT int& iResult)
{
    if (!IsUseAgencyDll())
    {
        return false;
    }
    iResult = m_AgencyPay.AgencyDebitQuery(szParamContext, iParamSize, iFormatType);
    return true;
}
bool CPayUnit::TransferImage(const char* szImageInfo, const int& iSize, const int& iFormatType)
{
    if (!IsUseAgencyDll())
    {
        return false;
    }
    std::string image_xml_para;
    if (iFormatType == 1)
    {
        if (!ParseImageTransferParaFromXML(szImageInfo, image_xml_para))
        {
            return false;
        }
    }
    else if (iFormatType == 2)
    {
        if (!ParseImageTransferParaFromJson(szImageInfo, image_xml_para))
        {
            return false;
        }
    }
    else
    {
        TW_TRACE_DIARY("ͼ���䣬��֧�ֵĲ�������");
        return false;
    }
    int para_size = static_cast<int>(image_xml_para.size());
    int trans_result = m_AgencyPay.AgencyDebitImage(image_xml_para.c_str(), para_size, 1);
    if (trans_result != 0)
    {
        char err_log[100] = {0};
        sprintf(err_log, "ͼ����ʧ�ܣ�������:%d", trans_result);
        TW_TRACE_DIARY(err_log);
        return false;
    }
    else
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////
// ��������:CPayUnit::IsTargetPayCancled
//
// ��������:�ж�ָ�����ף��Ƿ��Ѿ���ȡ��
//
// �������:const std::string& pay_id,
// �������:bool
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-17
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
bool CPayUnit::IsTargetPayCancled(const std::string & pay_id)
{
    trawe::CScopeLock pay_lock(&m_PayLock);
    for (std::list<DebitParameter>::const_iterator it = m_DebitGroup.begin(); it != m_DebitGroup.end(); ++it)
    {
        bool is_same_pay = (stricmp(pay_id.c_str(), it->PayIdentifier) == 0);
        if (!is_same_pay)
        {
            continue;
        }
        return it->IsCancled;

    }
    return false;
}
//////////////////////////////////////////////////////////////
// ��������:CPayUnit::GetDebitCancleFromDataBase
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
int CPayUnit::GetDebitCancleFromDataBase()
{
    static time_t last_check_time = 0;
    time_t curr_time = time(NULL);
    if ( (curr_time - last_check_time) < 30 )
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
// ��������:CPayUnit::DeleteDebitCancleFromDataBase
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
int CPayUnit::DeleteDebitCancleFromDataBase(const DebitCancleParameter & debit_cancle)
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

//////////////////////////////////////////////////////////////
// ��������:CPayUnit::NotifyPayStatus
//
// ��������:����֪ͨ�м�֧��״̬��Ŀǰ��Ҫ�������
//
// �������:int type,int result,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-20
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
// typeȡֵ˵����
// 1:��ʾ���۽������Ӧ��result������ʾ���ô��۽ӿڵõ��Ľ����
//
//////////////////////////////////////////////////////////////
int CPayUnit::NotifyPayStatus(int type, int result )
{
    if (!IsTianJinArea())
    {
        return -1;
    }
    if (type == 1 && result == 0)
    {   //�ɹ�ʱ�򣬲��ٸ����м�״̬
        return 0;
    }
    int mid_status = 255;
    if (type == 1)
    {
        switch (result)
        {
        case 12:
            mid_status = 1;
            break;
        case 21:
            mid_status = 2;
            break;
        case 22:
            mid_status = 3;
            break;
        case 25:
            mid_status = 4;
            break;
        case 26:
            mid_status = 5;
            break;
        case 27:
            mid_status = 6;
            break;
        }
    }
    NotifyResult(5, mid_status);
    return 0;
}

//////////////////////////////////////////////////////////////
// ��������:CPayUnit::NewPayResult
//
// ��������:���֧�����
//
// �������:const DebitResultParameter& debit_result,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-20
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
// 1����ౣ��һ�����
//
//////////////////////////////////////////////////////////////
int CPayUnit::NewPayResult(const DebitResultParameter& debit_result)
{
    trawe::CScopeLock result_lock(&m_PayResultLock);
    m_DebitResultGroup.clear();
    m_DebitResultGroup.push_back(debit_result);
    return 0;
}


int CPayUnit::ClearScreen()
{
    if (theApp.m_pThreadManage == 0
            || theApp.m_pThreadManage->m_pThreadDevice == 0)
    {
        return -1;
    }
    theApp.m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_DEVICE_STOP_GET_DATA, (WPARAM)0, (LPARAM)0);
    return  0;
}