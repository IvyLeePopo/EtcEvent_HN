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
// 函数名称:ETCPayManage::Init
//
// 功能描述:ETC功能的初始化，创建核心线程。
//
// 输入参数:
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
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
            TW_TRACE_DIARY_TYPE("ETC支付处理单元创建失败", TW_TRACE_TYPE_ETCMANAGE);
            return -1;
        }
        m_ETCPayTask->Resume();
        TW_TRACE_DIARY_TYPE("ETC支付处理单元创建成功", TW_TRACE_TYPE_ETCMANAGE);
    }
    if (m_ETCAuthTask == 0)
    {
        m_ETCAuthTask = new ETCAuthenticateUnit();
        if (m_ETCAuthTask == 0)
        {
            TW_TRACE_DIARY_TYPE("ETC支付处理单元创建失败", TW_TRACE_TYPE_ETCMANAGE);
            return -1;
        }
        m_ETCAuthTask->Resume();
        TW_TRACE_DIARY_TYPE("ETC授权处理单元创建成功", TW_TRACE_TYPE_ETCMANAGE);
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
	//TW_TRACE_DIARY_TYPE("ETC授权处理单元删除成功", TW_TRACE_TYPE_ETCMANAGE);
	return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayManage::StartWhiteListQuery
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
int ETCPayManage::StartWhiteListQuery(const ObuWhiteListQueryParameter& query_info)
{
    CScopeLock tmp_lock(&m_AccessMutex);
    TW_TRACE_DIARY_TYPE("开始白名单查询", TW_TRACE_TYPE_ETCMANAGE);
    if (m_ETCAuthTask == 0)
    {
        return 255;
    }
    return m_ETCAuthTask->StartWhiteListQuery(query_info);
}
//////////////////////////////////////////////////////////////
// 函数名称:ETCPayManage::IsInWhiteList
//
// 功能描述:查询白名单鉴权结果
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
// 函数名称:ETCPayManage::StartDebitStateQuery
//
// 功能描述:发起交易状态查询
//
// 输入参数:const DebitStateQueryParameter& query_info,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
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
// 函数名称:ETCPayManage::GetDebitState
//
// 功能描述:获取交易状态
//
// 输入参数:const DebitStateQueryParameter& query_info, ETCDebitRecord& state,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
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
// 函数名称:ETCPayManage::DebitMoney
//
// 功能描述:发起扣款
//
// 输入参数:const ETCDebitRecord& debit_para,std::string& result_str,const int& result_type,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
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
        TW_TRACE_DIARY_TYPE("查询不到该车辆的白名单信息，不处理该交易",TW_TRACE_TYPE_ETCPAYUNIT);
        return 255;
    }
    return m_ETCPayTask->DebitMoney(debit_para,result_str,result_type);
}
//////////////////////////////////////////////////////////////
// 函数名称:ETCPayManage::HasUnrecoverableError
//
// 功能描述:判断是否有系统故障
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



