#ifndef ETC_PAY_MANAGE_H
#define ETC_PAY_MANAGE_H
#include "ETCPayUnit.h"
#include "ETCAuthenticateUnit.h"

namespace trawe
{
class ETCPayManage
{
public:
    ETCPayManage();
    ~ETCPayManage();
    /*初始化，一定要在初始化接口中调用此接口*/
    int Init();
	int Destroy();
    /*提供发起白名单鉴权功能*/
    int StartWhiteListQuery(const ObuWhiteListQueryParameter& query_info);
    /*提供白名单鉴权结果查询结果功能*/
    int IsInWhiteList(const ObuWhiteListQueryParameter& query_info );
    /*提供发起交易状态查询功能*/
    int StartDebitStateQuery(const DebitStateQueryParameter& query_info);
    /*提供交易状态查询结果功能*/
    int GetDebitState(const DebitStateQueryParameter& query_info, ETCDebitRecord& state);
    /*提供扣费功能*/
    int DebitMoney(const ETCDebitRecord& debit_para,std::string& result_str,const int& result_type);
    /*查询是否发生系统故障*/
    bool HasUnrecoverableError();
private:
    /*ETC支付处理任务*/
    ETCPayUnit * m_ETCPayTask;
    /*ETC鉴权处理任务*/
    ETCAuthenticateUnit * m_ETCAuthTask;
    /*核心资源锁*/
    CRITICAL_SECTION m_AccessMutex;
private:
    ETCPayManage(const ETCPayManage&);
    ETCPayManage& operator = (const ETCPayManage&);
};



}



#endif // ETC_PAY_MANAGE_H
/**
 * 完成甘肃ETC的所有功能接口
 * 1、白名单功能
 * 2、交易发送
 * 3、交易归档(定期删除)、交易状态查询
 *
 * 方法说明：
 * StartWhiteListQuery:
 *     发起一个白名单查询,0：发起成功，-1:发起失败，其他值（目前只有1）:网络错误
 * IsInWhiteList:
 *     进行白名单查询，0:是白名单用户，-1:未知，其他值:非白名单用户，对应具体错误值
 * StartDebitStateQuery:
 *         发起一个交易查询，0:发起成功，-1:发起失败，其他值(目前只有1):错误
 *         问题：此交易查询，目前是查询本地记录，以后可能需要查询云端。
 * GetDebitState:
 *       0:查询状态成功，-1:状态未知,1:查询状态失败
 *      获取某个交易的交易状态，通过交易标识查询
 * DebitMoney:
 *      0:发起扣款成功，其他值：失败
 *      进行扣费，此处需要立即返回，并更新结果状态。
 *      需要先确保扣款报文保存到硬盘，以免出现问题。如果保存到磁盘失败，则返回失败
 */
