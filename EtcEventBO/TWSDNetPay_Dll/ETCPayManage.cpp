#include "ETCPayManage.h"


namespace trawe
{

ETCPayManage::ETCPayManage()
{
    m_ETCPayTask = 0;
    m_ETCAuthTask = 0;
    InitializeCriticalSection(&m_AccessMutex);
}
ETCPayManage::~ETCPayManage()
{
	Destroy();
    DeleteCriticalSection(&m_AccessMutex);
}
//////////////////////////////////////////////////////////////
// ��������:ETCPayManage::Init
//
// ��������:ETC���ܵĳ�ʼ�������������̡߳�
//
// �������:
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayManage::Init()
{
    CScopeLock tmp_lock(&m_AccessMutex);
    if (m_ETCPayTask == 0)
    {
        m_ETCPayTask = new ETCPayUnit();
        if (m_ETCPayTask == 0)
        {
            TW_TRACE_DIARY_TYPE("ETC֧������Ԫ����ʧ��", TW_TRACE_TYPE_ETCMANAGE);
            return -1;
        }
        m_ETCPayTask->Resume();
        TW_TRACE_DIARY_TYPE("ETC֧������Ԫ�����ɹ�", TW_TRACE_TYPE_ETCMANAGE);
    }
    if (m_ETCAuthTask == 0)
    {
        m_ETCAuthTask = new ETCAuthenticateUnit();
        if (m_ETCAuthTask == 0)
        {
            TW_TRACE_DIARY_TYPE("ETC֧������Ԫ����ʧ��", TW_TRACE_TYPE_ETCMANAGE);
            return -1;
        }
        m_ETCAuthTask->Resume();
        TW_TRACE_DIARY_TYPE("ETC��Ȩ����Ԫ�����ɹ�", TW_TRACE_TYPE_ETCMANAGE);
    }
    return 0;
}

int ETCPayManage::Destroy()
{
	CScopeLock tmp_lock(&m_AccessMutex);

	delete m_ETCPayTask;
	delete m_ETCAuthTask;
	m_ETCPayTask= 0;
	m_ETCAuthTask =0;
	//TW_TRACE_DIARY_TYPE("ETC��Ȩ����Ԫɾ���ɹ�", TW_TRACE_TYPE_ETCMANAGE);
	return 0;
}

//////////////////////////////////////////////////////////////
// ��������:ETCPayManage::StartWhiteListQuery
//
// ��������:�����������ѯ
//
// �������:const ObuWhiteListQueryParameter& query_info,
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayManage::StartWhiteListQuery(const ObuWhiteListQueryParameter& query_info)
{
    CScopeLock tmp_lock(&m_AccessMutex);
    TW_TRACE_DIARY_TYPE("��ʼ��������ѯ", TW_TRACE_TYPE_ETCMANAGE);
    if (m_ETCAuthTask == 0)
    {
        return 255;
    }
    return m_ETCAuthTask->StartWhiteListQuery(query_info);
}
//////////////////////////////////////////////////////////////
// ��������:ETCPayManage::IsInWhiteList
//
// ��������:��ѯ��������Ȩ���
//
// �������:const ObuWhiteListQueryParameter& query_info,
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayManage::IsInWhiteList(const ObuWhiteListQueryParameter& query_info )
{
    CScopeLock tmp_lock(&m_AccessMutex);
    if (m_ETCAuthTask == 0)
    {
        return 255;
    }
    return m_ETCAuthTask->IsAgencyWhiteList(query_info);
}
//////////////////////////////////////////////////////////////
// ��������:ETCPayManage::StartDebitStateQuery
//
// ��������:������״̬��ѯ
//
// �������:const DebitStateQueryParameter& query_info,
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayManage::StartDebitStateQuery(const DebitStateQueryParameter& query_info)
{
    CScopeLock tmp_lock(&m_AccessMutex);
    if (m_ETCPayTask == 0)
    {
        return 255;
    }
    return m_ETCPayTask->StartDebitStateQuery(query_info);
}
//////////////////////////////////////////////////////////////
// ��������:ETCPayManage::GetDebitState
//
// ��������:��ȡ����״̬
//
// �������:const DebitStateQueryParameter& query_info, ETCDebitRecord& state,
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayManage::GetDebitState(const DebitStateQueryParameter& query_info, ETCDebitRecord& state)
{
    CScopeLock tmp_lock(&m_AccessMutex);
    if (m_ETCPayTask == 0)
    {
        return 255;
    }

    return m_ETCPayTask->GetDebitState(query_info, state);
}
//////////////////////////////////////////////////////////////
// ��������:ETCPayManage::DebitMoney
//
// ��������:����ۿ�
//
// �������:const ETCDebitRecord& debit_para,std::string& result_str,const int& result_type,
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayManage::DebitMoney(const ETCDebitRecord& debit_para,std::string& result_str,const int& result_type)
{
    CScopeLock tmp_lock(&m_AccessMutex);
    if (m_ETCPayTask == 0)
    {
        return 255;
    }
    ObuWhiteListQueryParameter query_info;

    strcpy(query_info.OBUMAC,debit_para.OBUMAC);
    strcpy(query_info.OBUID,debit_para.OBUID);
    strcpy(query_info.CardID,debit_para.CardID);
    strcpy(query_info.VehicleLicense,debit_para.VehicleLicense);

    query_info.VehicleType = debit_para.VehicleType;
    query_info.VehicleClass = debit_para.VehicleClass;
    query_info.VehicleLicenseColor = debit_para.VehicleLicenseColor;
    if(m_ETCAuthTask->IsAgencyWhiteList(query_info)!=0)
    {
        TW_TRACE_DIARY_TYPE("��ѯ�����ó����İ�������Ϣ��������ý���",TW_TRACE_TYPE_ETCPAYUNIT);
        return 255;
    }
    return m_ETCPayTask->DebitMoney(debit_para,result_str,result_type);
}
//////////////////////////////////////////////////////////////
// ��������:ETCPayManage::HasUnrecoverableError
//
// ��������:�ж��Ƿ���ϵͳ����
//
// �������:
// �������:bool 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
bool ETCPayManage::HasUnrecoverableError()
{
    CScopeLock tmp_lock(&m_AccessMutex);
    if (m_ETCPayTask == 0)
    {
        return true;
    }
    if(m_ETCAuthTask==0)
    {
        return true;
    }
    return m_ETCPayTask->HasUnrecoverableError();
}

}



