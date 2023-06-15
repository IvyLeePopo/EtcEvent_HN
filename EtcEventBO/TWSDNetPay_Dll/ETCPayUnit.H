#ifndef ETC_PAY_UNIT_H
#define ETC_PAY_UNIT_H
#include "StandTask.h"
#include "ETCPayHelper.h"
#include "HttpConnection.h"

namespace trawe
{
class ETCPayUnit : public CStandTask
{
public:
    ETCPayUnit();
    virtual ~ETCPayUnit();
public:
    /*线程运行主体*/
    virtual void RunBody();
public:
    /*是否处理空闲*/
    bool IsHTTPIdle();
    /*发起一个交易状态查询*/
    int StartDebitStateQuery(const DebitStateQueryParameter& query_info);
    /*获取交易状态*/
    int GetDebitState(const DebitStateQueryParameter& query_info, ETCDebitRecord& state);
    /*进行扣款*/
    int DebitMoney(const ETCDebitRecord& debit_para, std::string& result_str, const int& result_type);
    /*查询是否有系统故障*/
	bool HasUnrecoverableError();
private:
    /*进行实际的扣款发送*/
    int SendOneDebit();
    /*进行实际的交易状态查询*/
    int SendDebitStateQuery();
    /*保存交易记录到磁盘*/
    int SaveDebitRecord(const ETCDebitRecord& record);

    /*将IO中的交易请求，放到数据库*/
    int MoveDebitRequestFromIOToDataBase();

    /*通过IO保存交易请求缓存*/
    int SaveDebitRequestToIO(const ETCDebitRecord& record);

    /*获取交易记录保存的文件名*/
    std::string GetRecordFileName(time_t t);
    /*清楚过期的交易记录*/
    int ClearAbateRecord();
    /*在指定的交易数据库中，查询指定交易信息*/
    int QueryRecordInTargetDataBase(const std::string& database_file,ETCDebitRecord&record);

    int NewDebitAbstract(const ETCDebitAbstract& debit_abstract);
    int QueryDebitAbstract(const std::string& plate,ETCDebitAbstract& debit_abstract);
private:
    /*设置系统故障*/
    int OccurSystemFatalError();
    /*获取下一个交易序号*/
    std::string GetNextPaySerialNo();
private:
    /*对支付数据相关的操作进行锁定*/
    CRITICAL_SECTION m_PayInfoLock;
    /*与数据库相关的操作进行锁定*/
    CRITICAL_SECTION m_DataBaseLock;
    /*交易发送队列*/
    std::list<ETCDebitRecord> m_DebitQueue;
    /*HTTP处理单元*/
    HttpConnection m_HttpPipe;
    /*是否发生不可恢复错误*/
    bool m_IsSystemFatalError;
    /*磁盘中，是否有待发送的交易*/
	bool m_HasDebitCache;
    /*最近交易摘要*/
    std::list<ETCDebitAbstract> m_RecentDebitAbstract;
private:
    ETCPayUnit(const ETCPayUnit&);
    ETCPayUnit& operator = (const ETCPayUnit&);
};

}

#endif // ETC_PAY_UNIT_H

/**
 * ETC支付单元，提供交易请求发送功能、交易状态查询功能
 * 1、发送交易请求到省中心
 * 2、将成功的交易记录进行归档，需要测试归档的性能问题（在归档数据集过大的情况下，插入是否过慢）
 * 3、查询某个交易是否成功
 * 4、按月归档交易，定时删除
 *
 * 方法介绍:
 * IsHTTPIdle:
 *     1、请求发送线程是否空闲，比如，上次请求没有发送完毕，
 *     2、如果不空闲，则认为网络处于阻塞，不接收其他请求
 * StartDebitStateQuery :
 *     
 *     发送交易请求，非阻塞、多线程安全
 * GetDebitState:
 *     查询交易状态，根据支付标识.非阻塞，多线程安全
 * DebitMoney:
 *     发起交易，非阻塞，多线程安全
 * SendOneDebit：
 *     实际发送一笔交易。
 * SendDebitStateQuery：
 *     发送交易状态查询数据
 * SaveDebitRecord：
 *     归档交易记录
 * ClearAbateRecord:
 *     清理过期的交易归档
 */