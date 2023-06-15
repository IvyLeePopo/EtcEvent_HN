#ifndef ETC_AUTHENTICATE_UNIT_H
#define ETC_AUTHENTICATE_UNIT_H
#include "StandTask.h"
#include "ETCPayHelper.h"
#include "HttpConnection.h"
#include <list>

namespace trawe
{


class ETCAuthenticateUnit : public CStandTask
{
public:
    ETCAuthenticateUnit();
    virtual ~ETCAuthenticateUnit();
public:
    /*任务主体*/
    virtual void RunBody();
public:
    /*发起白名单鉴权*/
    int StartWhiteListQuery(const ObuWhiteListQueryParameter& query_info);
    /*是否是白名单*/
    int IsAgencyWhiteList(const ObuWhiteListQueryParameter& query_info );
    /*缓存新的白名单记录*/
    int NewObuWhiteListRecord(const ObuWhiteListRecord& obu_record);
    /*是否处理繁忙*/
    bool IsHTTPIdle();
private:
    /*实际执行白名单鉴权查询*/
    void ExecuteQuery();
private:
    /*临界资源锁*/
    CRITICAL_SECTION m_AccessLock;
    /*网络通道*/
    HttpConnection m_HttpPipe;
    /*白名单鉴权请求队列*/
    std::list<ObuWhiteListQueryParameter> m_ObuQueryGroup;
    /*白名单鉴权结果*/
    std::list<ObuWhiteListRecord> m_ObuWhiteList;
private:
    ETCAuthenticateUnit(const ETCAuthenticateUnit&);
    ETCAuthenticateUnit& operator = (const ETCAuthenticateUnit&);
};

}

#endif // ETC_AUTHENTICATE_UNIT_H

/**
 * ETC白名单鉴权线程
 * StartWhiteListQuery:
 *     发起一个白名单查询,0：发起成功，-1:发起失败，其他值:其他错误
 *     非阻塞、多线程安全
 * IsAgencyWhiteList：
 *     是否是白名单经用户，0:是白名单用户，-1:未知，1:非白名单用户
 *     非阻塞、多线程安全
 * NewObuWhiteListRecord:
 *     插入新的OBU白名单记录
 *     多线程安全、
 *     此处会清理过期的数据，默认是5s，可以配置
 *  IsHTTPIdle：
 *      判断查询线程是否空闲，可能由于网络原因，上次查询仍然没有结果，则本次查询直接认为网络失败。
 */