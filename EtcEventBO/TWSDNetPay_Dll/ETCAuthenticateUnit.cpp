#include "ETCAuthenticateUnit.h"
#include "RunParameter.h"

namespace trawe
{


ETCAuthenticateUnit::ETCAuthenticateUnit()
{
    InitializeCriticalSection(&m_AccessLock);
    m_HttpPipe.ChangeRemotePeer(GetETCIP(), GetETCAuthPort());
}

ETCAuthenticateUnit::~ETCAuthenticateUnit()
{
	Stop();
    DeleteCriticalSection(&m_AccessLock);
}

void ETCAuthenticateUnit::RunBody()
{
    ExecuteQuery();
    CScopeLock access_lock(&m_AccessLock);
    if (!m_ObuQueryGroup.empty())
    {
        m_ObuQueryGroup.pop_front();
    }
    Sleep(5);
}

//////////////////////////////////////////////////////////////
// ��������:ETCAuthenticateUnit::ExecuteQuery
//
// ��������:ִ����Ȩ��ѯ
//
// �������:
// �������:void 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
void ETCAuthenticateUnit::ExecuteQuery()
{
    ObuWhiteListQueryParameter query_para;
    {
        CScopeLock access_lock(&m_AccessLock);
        if (m_ObuQueryGroup.empty())
        {
            return ;
        }
        query_para = m_ObuQueryGroup.front();
    }
    std::string query_json ;
    if (PackOBUWhiteListQueryJson(query_para, query_json) != 0)
    {
        TW_TRACE_DIARY_TYPE("OBU��������Ȩ��������ת��ʧ��" , TW_TRACE_TYPE_ETCAUTH);
        return ;
    }
    std::string utf8_json_str = StringConvertByCodePage(query_json, CP_ACP, CP_UTF8);

    std::string server_response;
	TW_TRACE_DIARY_TYPE("���Ͱ����������ѯ", TW_TRACE_TYPE_ETCAUTH);
    if (!m_HttpPipe.Post(GetETCAuthURL(), utf8_json_str, server_response))
    {
        TW_TRACE_DIARY_TYPE("���紦��ʧ��,������Ϣ:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TYPE_ETCAUTH);
        return ;
    }
	TW_TRACE_DIARY_TYPE("�յ���������������ѯ����", TW_TRACE_TYPE_ETCAUTH);
    std::string gbk_recv_str = StringConvertByCodePage(server_response, CP_UTF8, CP_ACP);
    ObuWhiteListRecord obu_record;
    if (!ParseOBUWhiteListRecordFromServer(gbk_recv_str, obu_record))
    {
        TW_TRACE_DIARY_TYPE("����������������ʧ��:" + gbk_recv_str , TW_TRACE_TYPE_ETCAUTH);
        return ;
    }
    obu_record.GenerateTime = ChinaTime();

    NewObuWhiteListRecord(obu_record);

}
//////////////////////////////////////////////////////////////
// ��������:ETCAuthenticateUnit::StartWhiteListQuery
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
int ETCAuthenticateUnit::StartWhiteListQuery(const ObuWhiteListQueryParameter& query_info)
{
    if (!IsHTTPIdle())
    {
        TW_TRACE_DIARY_TYPE("����æ", TW_TRACE_TYPE_ETCAUTH);
        return -1;
    }
    TW_TRACE_DIARY_TYPE("����������ѯ���ã��������", TW_TRACE_TYPE_ETCAUTH);
    CScopeLock access_lock(&m_AccessLock);
    TW_TRACE_DIARY_TYPE("����������ѯ���ã�������гɹ�", TW_TRACE_TYPE_ETCAUTH);
    m_ObuQueryGroup.push_back(query_info);
    return 0;
}
//////////////////////////////////////////////////////////////
// ��������:ETCAuthenticateUnit::IsAgencyWhiteList
//
// ��������:�ж��Ƿ��ڰ�������
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
int ETCAuthenticateUnit::IsAgencyWhiteList(const ObuWhiteListQueryParameter& query_info )
{
    CScopeLock access_lock(&m_AccessLock);
    time_t curr_time = ChinaTime();
    while (true)
    {
        if (m_ObuWhiteList.empty())
        {
            return -1;
        }
        if ( ( curr_time - m_ObuWhiteList.front().GenerateTime) > GetObuWhiteListRecordLifeSpanSeconds())
        {
            TW_TRACE_DIARY_TYPE("���ڰ�������¼���Ƴ���" + std::string(m_ObuWhiteList.front().VehicleLicense), TW_TRACE_TYPE_ETCAUTH);
            m_ObuWhiteList.pop_front();
        }
        else
        {
            break;
        }
    }
    if (m_ObuWhiteList.empty())
    {
        return -1;
    }
    ObuWhiteListRecord obu_record = m_ObuWhiteList.front();
    char VehicleLicense[50];
    bool same_obu = (query_info.VehicleLicenseColor == obu_record.VehicleLicenseColor)
                    && (query_info.VehicleType == obu_record.VehicleType)
                    && (query_info.VehicleClass == obu_record.VehicleClass)
                    && (strcmp(query_info.VehicleLicense, obu_record.VehicleLicense) == 0 );

    if (same_obu)
    {
        return obu_record.AgentStatus;
    }
    return -1;
}

//////////////////////////////////////////////////////////////
// ��������:ETCAuthenticateUnit::NewObuWhiteListRecord
//
// ��������:��ѯ�µİ�������Ȩ���
//
// �������:const ObuWhiteListRecord& obu_record,
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCAuthenticateUnit::NewObuWhiteListRecord(const ObuWhiteListRecord& obu_record)
{
    CScopeLock access_lock(&m_AccessLock);
	m_ObuWhiteList.clear();
    m_ObuWhiteList.push_back(obu_record);
    char hint_info[100] ={0};
    _snprintf(hint_info,80,"��ȡ��������¼������:%s,״̬:%d",obu_record.VehicleLicense,obu_record.AgentStatus);
    TW_TRACE_DIARY_TYPE(hint_info,TW_TRACE_TYPE_ETCAUTH);
    return 0;
}
//////////////////////////////////////////////////////////////
// ��������:ETCAuthenticateUnit::IsHTTPIdle
//
// ��������:�Ƿ���æ
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
bool ETCAuthenticateUnit::IsHTTPIdle()
{
    CScopeLock access_lock(&m_AccessLock);
    return m_ObuQueryGroup.empty();
}

}