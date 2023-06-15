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
// 函数名称:ETCAuthenticateUnit::ExecuteQuery
//
// 功能描述:执行授权查询
//
// 输入参数:
// 输出参数:void 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
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
        TW_TRACE_DIARY_TYPE("OBU白名单鉴权参数请求转换失败" , TW_TRACE_TYPE_ETCAUTH);
        return ;
    }
    std::string utf8_json_str = StringConvertByCodePage(query_json, CP_ACP, CP_UTF8);

    std::string server_response;
	TW_TRACE_DIARY_TYPE("发送白名单网络查询", TW_TRACE_TYPE_ETCAUTH);
    if (!m_HttpPipe.Post(GetETCAuthURL(), utf8_json_str, server_response))
    {
        TW_TRACE_DIARY_TYPE("网络处理失败,错误信息:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TYPE_ETCAUTH);
        return ;
    }
	TW_TRACE_DIARY_TYPE("收到服务器白名单查询回馈", TW_TRACE_TYPE_ETCAUTH);
    std::string gbk_recv_str = StringConvertByCodePage(server_response, CP_UTF8, CP_ACP);
    ObuWhiteListRecord obu_record;
    if (!ParseOBUWhiteListRecordFromServer(gbk_recv_str, obu_record))
    {
        TW_TRACE_DIARY_TYPE("解析服务器处理结果失败:" + gbk_recv_str , TW_TRACE_TYPE_ETCAUTH);
        return ;
    }
    obu_record.GenerateTime = ChinaTime();

    NewObuWhiteListRecord(obu_record);

}
//////////////////////////////////////////////////////////////
// 函数名称:ETCAuthenticateUnit::StartWhiteListQuery
//
// 功能描述:发起白名单查询
//
// 输入参数:const ObuWhiteListQueryParameter& query_info,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int ETCAuthenticateUnit::StartWhiteListQuery(const ObuWhiteListQueryParameter& query_info)
{
    if (!IsHTTPIdle())
    {
        TW_TRACE_DIARY_TYPE("处理繁忙", TW_TRACE_TYPE_ETCAUTH);
        return -1;
    }
    TW_TRACE_DIARY_TYPE("将白名单查询调用，放入队列", TW_TRACE_TYPE_ETCAUTH);
    CScopeLock access_lock(&m_AccessLock);
    TW_TRACE_DIARY_TYPE("将白名单查询调用，放入队列成功", TW_TRACE_TYPE_ETCAUTH);
    m_ObuQueryGroup.push_back(query_info);
    return 0;
}
//////////////////////////////////////////////////////////////
// 函数名称:ETCAuthenticateUnit::IsAgencyWhiteList
//
// 功能描述:判断是否在白名单中
//
// 输入参数:const ObuWhiteListQueryParameter& query_info,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
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
            TW_TRACE_DIARY_TYPE("过期白名单记录，移除：" + std::string(m_ObuWhiteList.front().VehicleLicense), TW_TRACE_TYPE_ETCAUTH);
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
// 函数名称:ETCAuthenticateUnit::NewObuWhiteListRecord
//
// 功能描述:查询新的白名单鉴权结果
//
// 输入参数:const ObuWhiteListRecord& obu_record,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int ETCAuthenticateUnit::NewObuWhiteListRecord(const ObuWhiteListRecord& obu_record)
{
    CScopeLock access_lock(&m_AccessLock);
	m_ObuWhiteList.clear();
    m_ObuWhiteList.push_back(obu_record);
    char hint_info[100] ={0};
    _snprintf(hint_info,80,"获取白名单记录，车牌:%s,状态:%d",obu_record.VehicleLicense,obu_record.AgentStatus);
    TW_TRACE_DIARY_TYPE(hint_info,TW_TRACE_TYPE_ETCAUTH);
    return 0;
}
//////////////////////////////////////////////////////////////
// 函数名称:ETCAuthenticateUnit::IsHTTPIdle
//
// 功能描述:是否处理繁忙
//
// 输入参数:
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ETCAuthenticateUnit::IsHTTPIdle()
{
    CScopeLock access_lock(&m_AccessLock);
    return m_ObuQueryGroup.empty();
}

}