#include "ETCPayUnit.h"
#include "RunParameter.h"
#include "DataBaseHelper.h"
#include "fs_funcs.h"
#include "pystring.h"
#include <sstream>

namespace trawe
{

BOOL MakeDirectory_second(CString strCreateDir)
{
    HANDLE  fFile;                      // File Handle
    WIN32_FIND_DATA fileinfo = {0};     // File Information Structure
    CStringArray arr;               // CString Array to hold Directory Structures
    BOOL bSuccess = false;              // BOOL used to test if Create Directory was successful
    int nCount = 0;                     // Counter
    CString strTemp;                    // Temporary CString Object

    fFile = FindFirstFile(strCreateDir, &fileinfo);

    // if the file exists and it is a directory
    if (fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    {
        //  Directory Exists close file and return
        FindClose(fFile);
        return TRUE;
    }
    arr.RemoveAll();

    for (nCount = 0; nCount < strCreateDir.GetLength(); nCount++ ) // Parse the supplied CString Directory String
    {
        if (strCreateDir.GetAt(nCount) != '\\') // if the Charachter is not a \ 
            strTemp += strCreateDir.GetAt(nCount);  // add the character to the Temp String
        else
        {
            arr.Add(strTemp);   // if the Character is a \ 
            strTemp += _T("\\");   // Now add the \ to the temp string
        }
        if (nCount == strCreateDir.GetLength() - 1) // If we reached the end of the String
            arr.Add(strTemp);
    }

    // Close the file
    FindClose(fFile);

    // Now lets cycle through the String Array and create each directory in turn
    for (nCount = 1; nCount < arr.GetSize(); nCount++)
    {
        strTemp = arr.GetAt(nCount);
        bSuccess = CreateDirectory(strTemp, NULL);

        // If the Directory exists it will return a false
        if (bSuccess)
            SetFileAttributes(strTemp, FILE_ATTRIBUTE_NORMAL);
        // If we were successful we set the attributes to normal
    }
    //  Now lets see if the directory was successfully created
    fFile = FindFirstFile(strCreateDir, &fileinfo);

    arr.RemoveAll();
    if (fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    {
        //  Directory Exists close file and return
        FindClose(fFile);
        return TRUE;
    }
    else
    {
        // For Some reason the Function Failed  Return FALSE
        FindClose(fFile);
        return FALSE;
    }
}



ETCPayUnit::ETCPayUnit()
{
    InitializeCriticalSection(&m_PayInfoLock);
    InitializeCriticalSection(&m_DataBaseLock);
    m_HttpPipe.ChangeRemotePeer(GetETCIP(), GetETCPayPort());
    m_IsSystemFatalError = false;
    m_HasDebitCache = true;

}
ETCPayUnit::~ETCPayUnit()
{
	Stop();
    DeleteCriticalSection(&m_PayInfoLock);
    DeleteCriticalSection(&m_DataBaseLock);
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::RunBody
//
// 功能描述:线程运行主体，包含：
// a、发送交易数据
// b、发送交易状态查询
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
void ETCPayUnit::RunBody()
{
    SendOneDebit();
    SendDebitStateQuery();
    Sleep(5);
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::IsHTTPIdle
//
// 功能描述:判断当前,如果使用HTTP是否需要排队
//
// 输入参数:
// 输出参数:bool
// 编写人员:朱亚冰
// 编写时间:2018-12-24
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ETCPayUnit::IsHTTPIdle()
{
    CScopeLock pay_lock(&m_PayInfoLock);
    

    //交易队列是否为空
    if (!m_DebitQueue.empty())
    {
        TW_TRACE_DIARY_TYPE("HTTP发送队列忙碌:交易数据正在发送", TW_TRACE_TYPE_ETCPAYUNIT);
        return false;
    }
    if (m_HasDebitCache)
    {
        return false;
    }
    return true;

    //交易队列缓存是否为空，需要查询本地数据库
    sqlite3* sql_conn = DataBaseOpen(GetDebitRequestCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("交易缓存数据库存在问题，无法打开:" + GetDebitRequestCachePath() , TW_TRACE_TYPE_ETCPAYUNIT);
        return false;
    }

    ETCDebitRecord no_use_para ;
    bool has_record = (DataBaseQueryRecord(sql_conn, no_use_para ) == 0);
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("数据库关闭失败", TW_TRACE_TYPE_ETCPAYUNIT);
    }
    if (has_record)
    {
        TW_TRACE_DIARY_TYPE("HTTP发送队列忙碌:本地交易缓存需要发送", TW_TRACE_TYPE_ETCPAYUNIT);
        return false;
    }
    return true;

}
//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::StartDebitStateQuery
//
// 功能描述:开启一个交易状态查询。
//
//
// 输入参数:const DebitStateQueryParameter& query_info,
// 输出参数:如果当前处理繁忙，则返回失败
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::StartDebitStateQuery(const DebitStateQueryParameter& query_info)
{
    if (!IsHTTPIdle())
    {
        TW_TRACE_DIARY_TYPE("处理繁忙", TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    //CScopeLock pay_lock(&m_PayInfoLock);
    // m_StateQueryQueue.push_back(query_info);
    return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::GetRecordFileName
//
// 功能描述:根据时间参数，生成文件名
//
// 输入参数:time_t t,
// 输出参数:std::string
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string ETCPayUnit::GetRecordFileName(time_t t)
{   //切割时间字符串
    std::string time_str = Datetime2Str(t);
    std::vector<std::string> time_strs;
    pystring::split(time_str, time_strs, "-");
    if (time_strs.size() < 3)
    {
        return "不支持的文件名";
    }
    std::string record_name = "PayRecord" + time_strs[0] + time_strs[1] + ".db";
    return record_name;
}
//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::GetDebitState
//
// 功能描述:查询指定交易的交易结果。
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
int ETCPayUnit::GetDebitState(const DebitStateQueryParameter& query_info, ETCDebitRecord& state)
{
    std::string today_record_path = pystring::os::path::join(GetDebitRecordArchiveDir(), GetRecordFileName(ChinaTime()));
    ETCDebitRecord debit_state ;
    {   //复制信息
        strncpy(debit_state.PayIdentifier, query_info.PayIdentifier, sizeof(debit_state.PayIdentifier) - 1);
    }
    if (QueryRecordInTargetDataBase(today_record_path, debit_state) == 0)
    {
        state = debit_state;
        return 0;
    }
    std::string yestoday_record_path = pystring::os::path::join(GetDebitRecordArchiveDir(), GetRecordFileName(ChinaTime() - 24 * 60 * 60));
    if (yestoday_record_path == today_record_path)
    {
        TW_TRACE_DIARY_TYPE("查找不到该交易记录，识别码:" + std::string(query_info.PayIdentifier), TW_TRACE_TYPE_ETCPAYUNIT);
        return 255;
    }
    if (QueryRecordInTargetDataBase(yestoday_record_path, debit_state) == 0)
    {
        state = debit_state;
        return 0;
    }
    TW_TRACE_DIARY_TYPE("查找不到该交易记录，识别码:" + std::string(query_info.PayIdentifier), TW_TRACE_TYPE_ETCPAYUNIT);
    return 255;
}
//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::DebitMoney
//
// 功能描述:发起扣费
//
// 输入参数:const ETCDebitRecord& debit_para, std::string& result_str, const int& result_type,
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::DebitMoney(const ETCDebitRecord& debit_para, std::string& result_str, const int& result_type)
{
	
    if (!IsHTTPIdle())
    {
        TW_TRACE_DIARY_TYPE("处理繁忙", TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
	
    ETCDebitAbstract tmp_abstract;
    if( QueryDebitAbstract(debit_para.VehicleLicense,tmp_abstract) ==0)
    {
        char err_log[200]={0};
        sprintf(err_log,"该车牌最近交易过，不允许重复交易，上次交易时间:%s",Datetime2Str(tmp_abstract.DebitTime).c_str());
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_ETCPAYUNIT);
        return 101;  //错误信息
    }
    CScopeLock pay_lock(&m_PayInfoLock);
    ETCDebitRecord one_debit = debit_para;
    std::string pay_serial = GetNextPaySerialNo();
    if (pay_serial.size() < 10)
    {
        TW_TRACE_DIARY_TYPE("交易序号获取失败", TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    strcpy(one_debit.LocalResponse.PaySerialNo, pay_serial.c_str());
    std::string sec_code = GetNetPayRecordMatchCode(one_debit);
    strncpy(one_debit.LocalResponse.TradeSecurityCode, sec_code.c_str(), sizeof(one_debit.LocalResponse.TradeSecurityCode) - 1);
    strcpy(one_debit.LocalResponse.PayIdentifier, one_debit.PayIdentifier);
    one_debit.LocalResponse.DebitTime = one_debit.SubTime;
    one_debit.LocalResponse.PayPlatformType = 2;//目前都是支付宝支付

    {   //插入到数据库中,此处首先使用IO保留，
        if(SaveDebitRequestToIO(one_debit)!=0)
        {
            return -1;
        }
    }
    m_DebitQueue.push_back(one_debit);

    m_HasDebitCache = true;
    std::stringstream ss;
    ss << "交易数据插入队列成功";
    ss << ",识别码:" << one_debit.PayIdentifier;
    ss << ",绑定交易号:" << one_debit.LocalResponse.PaySerialNo;
    TW_TRACE_DIARY_TYPE(ss.str(), TW_TRACE_TYPE_ETCPAYUNIT);

    if (result_type == 1)
    {
        PackETCDebitResultXML(one_debit, result_str);
    }
    else if (result_type == 2)
    {
		PackETCDebitResultJson(one_debit,result_str);
    }
    else
    {
        char err_log[100];
        sprintf(err_log, "不支持结果类型:%d", result_type);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
	
    return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::SendOneDebit
//
// 功能描述:进行实际的交易数据发送。
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
int ETCPayUnit::SendOneDebit()
{
	static time_t last_hint_error_time=time(NULL);
	if(HasUnrecoverableError())
	{
		time_t curr_time = time(NULL);
		if( (curr_time-last_hint_error_time)<20)
		{
			return -1;
		}
		last_hint_error_time = curr_time;
		TW_TRACE_DIARY_TYPE("存在无法恢复的错误，停止ETC支付" , TW_TRACE_TYPE_ETCPAYUNIT);
		return -1;
	}
    ETCDebitRecord debit_record;
    {
        CScopeLock pay_lock(&m_PayInfoLock);
		MoveDebitRequestFromIOToDataBase();
        if (m_DebitQueue.empty() )
        {
            if (!m_HasDebitCache)
            {
                return 0;
            }
            CScopeLock database_lock(&m_DataBaseLock);
            ETCDebitRecord etc_para;
            sqlite3* sql_conn = DataBaseOpen(GetDebitRequestCachePath());
            int db_ret = DataBaseQueryDebitQuest(sql_conn,  etc_para);
            DataBaseClose(sql_conn);
            if (db_ret != 0)
            {   //没有交易记录需要可处理
                m_HasDebitCache = false;
                return 0;
            }
            m_HasDebitCache = true;
            m_DebitQueue.push_back(etc_para);
        }
        debit_record = m_DebitQueue.front();
    }
    //如果发送成功，则尝试删除队列前半段的内容
    TW_TRACE_DIARY_TYPE("发送交易请求到服务器,识别码:" + std::string(debit_record.PayIdentifier) , TW_TRACE_TYPE_ETCPAYUNIT);
    std::string http_json_para;
    std::string http_response_json;
    PackETCDebitRequestJson(debit_record, http_json_para);

    std::string utf8_json_str = StringConvertByCodePage(http_json_para, CP_ACP, CP_UTF8);
    if (!m_HttpPipe.Post(GetETCPayURL(), utf8_json_str, http_response_json))
    {
        TW_TRACE_DIARY_TYPE("网络处理失败,错误信息:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TYPE_ETCPAYUNIT);
		Sleep(5*1000);
        return -1;
    }
    std::string gbk_recv_str = StringConvertByCodePage(http_response_json, CP_UTF8, CP_ACP);
    if (!ParseETCDebitResponseFromServer(gbk_recv_str, debit_record))
    {
        TW_TRACE_DIARY_TYPE("解析服务器处理结果失败:" + gbk_recv_str , TW_TRACE_TYPE_ETCPAYUNIT);
		Sleep(5*1000);
        return -1;
    }
    // TW_TRACE_DIARY_TYPE("######交易回馈处理逻辑尚未实现:" , TW_TRACE_TYPE_ETCPAYUNIT);

    /**
     * 根据回馈的结果处理以下内容
     * 1、生成交易记录，并归档
     * 2、如果队列头部是对应交易，则删除
     * 3、删除缓存中的对应交易
     */
    {   //将该交易记录归档
        if (SaveDebitRecord(debit_record) == 0)
        {
            TW_TRACE_DIARY_TYPE("交易归档成功,识别码:" + std::string(debit_record.PayIdentifier) , TW_TRACE_TYPE_ETCPAYUNIT);
        }
        else
        {
            OccurSystemFatalError();
            TW_TRACE_DIARY_TYPE("交易归档失败,识别码:" + std::string(debit_record.PayIdentifier) , TW_TRACE_TYPE_ETCPAYUNIT);
            return 0;
        }

    }
    {
        CScopeLock database_lock(&m_DataBaseLock);
        //删除磁盘的内容
        sqlite3* sql_conn = DataBaseOpen(GetDebitRequestCachePath());
        if (sql_conn == 0)
        {
            TW_TRACE_DIARY_TYPE("交易请求数据库存在问题，无法打开:" + GetDebitRequestCachePath(), TW_TRACE_TYPE_ETCPAYUNIT);
            OccurSystemFatalError();
            return -1;
        }

        if (DataBaseDeleteDebitQuest(sql_conn, debit_record) != 0)
        {
            std::stringstream ss;
            ss << "识别码:" << debit_record.PayIdentifier << ",删除失败." << DataBaseGetLastError(sql_conn);
            TW_TRACE_DIARY_TYPE(ss.str(), TW_TRACE_TYPE_ETCPAYUNIT);
            DataBaseClose(sql_conn);
            OccurSystemFatalError();
            return -1;
        }
        if (DataBaseClose(sql_conn) != 0)
        {
            TW_TRACE_DIARY_TYPE("数据库关闭失败:" + GetDebitRequestCachePath(), TW_TRACE_TYPE_ETCPAYUNIT);
            OccurSystemFatalError();
            return -1;
        }
        TW_TRACE_DIARY_TYPE("交易请求回馈成功，删除该请求缓存,识别码:" + std::string(debit_record.PayIdentifier), TW_TRACE_TYPE_ETCPAYUNIT);
    }

    {   //删除队列中的内容
        CScopeLock pay_lock(&m_PayInfoLock);
        
        if (!m_DebitQueue.empty())
        {
            bool is_same_payid = (stricmp (debit_record.PayIdentifier, m_DebitQueue.front().PayIdentifier) == 0);
            if (  is_same_payid )
            {
                m_DebitQueue.pop_front();
                TW_TRACE_DIARY_TYPE("交易请求回馈成功，从队列中移除该请求,识别码:" + std::string(debit_record.PayIdentifier), TW_TRACE_TYPE_ETCPAYUNIT);
            }
            else
            {
                TW_TRACE_DIARY_TYPE("交易队列出现异常，正在交易的数据与队头数据不一致" , TW_TRACE_TYPE_ETCPAYUNIT);
            }
        }

    }

   
	if(debit_record.ServerResponse.ServerStatus==1)
	{
		ETCDebitAbstract debit_abstract;
		debit_abstract.IsSuccess = true;
		strncpy(debit_abstract.PlateNo,debit_record.VehicleLicense,sizeof(debit_abstract.PlateNo)-1);
		debit_abstract.DebitTime = time(NULL);
		NewDebitAbstract(debit_abstract);
	}
	

    return 0;
}
//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::SendDebitStateQuery
//
// 功能描述:目前不需要此功能
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
int ETCPayUnit::SendDebitStateQuery()
{
    return 0;//目前不需要，从磁盘直接返回
}
//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::SaveDebitRecord
//
// 功能描述:保存交易记录到磁盘。
//
// 输入参数:const ETCDebitRecord& record,
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::SaveDebitRecord(const ETCDebitRecord& record)
{
    CScopeLock database_lock(&m_DataBaseLock);
    //MakeDirectory(GetDebitRecordArchiveDir());
	MakeDirectory_WindowsFun(GetDebitRecordArchiveDir());
    std::string record_path = pystring::os::path::join(GetDebitRecordArchiveDir(), GetRecordFileName(ChinaTime()));
    sqlite3* sql_conn = DataBaseOpen(record_path);
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("交易归档数据库存在问题，无法打开:" + record_path, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }

    if (
        (DataBaseCreateTable(sql_conn, DataBaseGetDebitRecordTableName(), DataBaseGetDebitRecordTableDefine()) != 0 )
        || (DataBaseAddRecord(sql_conn, record) != 0)
    )
    {
        std::stringstream ss;
        ss << "识别码:" << record.PayIdentifier << ",保存失败." << DataBaseGetLastError(sql_conn);
        TW_TRACE_DIARY_TYPE(ss.str(), TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
		if (DataBaseClose(sql_conn) != 0)
		{
			TW_TRACE_DIARY_TYPE("数据库关闭失败:" + record_path, TW_TRACE_TYPE_ETCPAYUNIT);
		}
		return -1;
    }
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("数据库关闭失败:" + record_path, TW_TRACE_TYPE_ETCPAYUNIT);
    }
    ClearAbateRecord();
    return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::ClearAbateRecord
//
// 功能描述:清除过期的交易记录文件
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
int ETCPayUnit::ClearAbateRecord()
{
    static time_t last_clear_time = 0;
    time_t curr_time = ChinaTime();
    if ( (curr_time - last_clear_time ) < (24 * 60 * 60))
    {
        return -1;
    }
    TW_TRACE_DIARY_TYPE("检测过期归档文件", TW_TRACE_TYPE_ETCPAYUNIT);
    last_clear_time = curr_time;
    Search_file_info sfi;
    strcpy(sfi.root_catalog, GetDebitRecordArchiveDir().c_str());
    sfi.enable_wildcard = true;
    strcpy(sfi.pattern, "*.db");
    std::list<std::string> record_file_group = Search_files(sfi);
    SYSTEMTIME system_time;

    GetSystemTime(&system_time);
    for (std::list<std::string>::iterator it = record_file_group.begin(); it != record_file_group.end(); ++it)
    {
        SYSTEMTIME file_modify_time;
        if (GetFileModifyTime(*it, file_modify_time) != 0)
        {
            TW_TRACE_DIARY_TYPE("获取文件属性失败:" + (*it), TW_TRACE_TYPE_ETCPAYUNIT);
            continue;
        }
        __int64 time_diff_ms = GetDiffMilliSeconds(file_modify_time, system_time);
        //char debug_chs[100]={0};
        //sprintf(debug_chs,"time_diff:%lld",time_diff_ms);
        //TW_DEBUG_VARIABLE(file_modify_time);
        //TW_DEBUG_VARIABLE(system_time);
        //TW_TRACE_DIARY_TYPE(debug_chs, TW_TRACE_TYPE_ETCPAYUNIT);
        bool is_overdue = ( (time_diff_ms / (24 * 60 * 60 * 1000)) > GetDebitRecordLifeSpanDays()) ;
        if (is_overdue)
        {
            remove(it->c_str());
            TW_TRACE_DIARY_TYPE("删除归档文件:" + (*it), TW_TRACE_TYPE_ETCPAYUNIT);
        }
    }
    return 0;


}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::HasUnrecoverableError
//
// 功能描述:查询是否有系统故障
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
bool ETCPayUnit::HasUnrecoverableError()
{
    // CScopeLock pay_lock(&m_PayInfoLock);
    return m_IsSystemFatalError;
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::OccurSystemFatalError
//
// 功能描述:设置系统故障
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
int ETCPayUnit::OccurSystemFatalError()
{
    //CScopeLock pay_lock(&m_PayInfoLock);
    TW_TRACE_DIARY_TYPE("程序无法从错误中恢复", TW_TRACE_TYPE_ETCPAYUNIT);
    m_IsSystemFatalError = true;
    return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::GetNextPaySerialNo
//
// 功能描述:获取交易序号，只需要保持唯一即可，使用guid
//
// 输入参数:
// 输出参数:std::string
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string ETCPayUnit::GetNextPaySerialNo()
{
    return GetGuid();
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::QueryRecordInTargetDataBase
//
// 功能描述:在指定数据库中查询交易结果。
//
// 输入参数:const std::string& database_file,ETCDebitRecord&debit_state,
// 输出参数:0，查询成功
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::QueryRecordInTargetDataBase(const std::string& database_file, ETCDebitRecord&debit_state)
{
    CScopeLock database_lock(&m_DataBaseLock);
    sqlite3* sql_conn = DataBaseOpen(database_file);
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("交易结果数据库存在问题，无法打开:" + database_file, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    bool has_record = (DataBaseQueryRecord(sql_conn, debit_state ) == 0);
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("数据库关闭失败:" + database_file, TW_TRACE_TYPE_ETCPAYUNIT);
    }
    if (has_record)
    {
        return 0;
    }
    return -1;
}


//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::MoveDebitRequestFromIOToDataBase
//
// 功能描述:将IO中的交易请求缓存，保存到数据库
//
// 输入参数:const ETCDebitRecord& record,
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2019-01-17
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::MoveDebitRequestFromIOToDataBase()
{
    CScopeLock database_lock(&m_DataBaseLock);
    std::string tmp_para_path = GetDebitParaTMPCachePath();
    ETCDebitRecord one_debit;
    if (access(tmp_para_path.c_str(), 0) != 0)
    {
        return 0;
    }
    FILE * fid = fopen(tmp_para_path.c_str(), "rb");
    if (fid == 0)
    {
        TW_TRACE_DIARY_TYPE("读交易数据失败，文件打开失败:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
        return -1;
    }
    if (fread(&one_debit, sizeof(one_debit), 1, fid) != 1)
    {
        TW_TRACE_DIARY_TYPE("读交易数据失败，读数据失败:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        fclose(fid);
        OccurSystemFatalError();
        return -1;
    }
    fclose(fid);
    if(remove(tmp_para_path.c_str())!=0)
    {
        TW_TRACE_DIARY_TYPE("清理文件失败:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
        return -1;
    }

    sqlite3* sql_conn = DataBaseOpen(GetDebitRequestCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("交易请求数据库存在问题，无法打开:" + GetDebitRequestCachePath(), TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
        return -1;
    }

    if ((DataBaseCreateTable(sql_conn, DataBaseGetDebitRequestTableName(), DataBaseGetDebitQuestTableDefine()) != 0)
            || (DataBaseAddDebitQuest(sql_conn, one_debit) != 0)
       )
    {
        std::stringstream ss;
        ss << "识别码:" << one_debit.PayIdentifier << ",保存失败." << DataBaseGetLastError(sql_conn);
        TW_TRACE_DIARY_TYPE(ss.str(), TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
        return -1;
    }
    else
    {
        std::stringstream ss;
        ss << "将数据从IO转移到数据库成功:" << one_debit.PayIdentifier ;
        TW_TRACE_DIARY_TYPE(ss.str(), TW_TRACE_TYPE_ETCPAYUNIT);
    }
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("数据库关闭失败:" + GetDebitRequestCachePath(), TW_TRACE_TYPE_ETCPAYUNIT);
    }

    return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:ETCPayUnit::SaveDebitRequestToIO
//
// 功能描述:将交易请求保存到IO
//
// 输入参数:const ETCDebitRecord& record,
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2019-01-17
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::SaveDebitRequestToIO(const ETCDebitRecord& one_debit)
{
    std::string tmp_para_path = GetDebitParaTMPCachePath();
    if (access(tmp_para_path.c_str(), 0) == 0)
    {
        TW_TRACE_DIARY_TYPE("存在未处理的交易数据:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    FILE * fid = fopen(tmp_para_path.c_str(), "wb");
    if (fid == 0)
    {
        TW_TRACE_DIARY_TYPE("写交易数据失败，文件打开失败:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    if (fwrite(&one_debit, sizeof(one_debit), 1, fid) != 1)
    {
        TW_TRACE_DIARY_TYPE("写交易数据失败，写失败:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        fclose(fid);
        remove(tmp_para_path.c_str());
        return -1;
    }
    fclose(fid);
    return 0;
}

int ETCPayUnit::NewDebitAbstract(const ETCDebitAbstract& debit_abstract)
{
    static int dup_check_secs = GetDuplicateDebitShortestMinutes() * 60;
    CScopeLock pay_lock(&m_PayInfoLock);
    
    m_RecentDebitAbstract.push_back(debit_abstract);
    while(true)
    {
        if(m_RecentDebitAbstract.empty())
        {
            break;
        }
		int secs_diff = time(0) - m_RecentDebitAbstract.front().DebitTime;
		if( secs_diff > dup_check_secs)
		{
			m_RecentDebitAbstract.pop_front();
		}
		else
		{
			break;
		}
    }
	while(m_RecentDebitAbstract.size()>1000)
	{
		m_RecentDebitAbstract.pop_front();
	}
    return 0;
}
int ETCPayUnit::QueryDebitAbstract(const std::string& plate,ETCDebitAbstract& debit_abstract)
{
	static int dup_check_secs = GetDuplicateDebitShortestMinutes() * 60;
	CScopeLock pay_lock(&m_PayInfoLock);
    
	for(std::list<ETCDebitAbstract>::const_iterator it = m_RecentDebitAbstract.begin(); it!= m_RecentDebitAbstract.end();++it)
	{
		if(!it->IsSuccess)
		{
			continue;
		}
		if(it->PlateNo != plate)
		{
			continue;
		}
		bool is_overdue = (time(NULL) - it->DebitTime) > dup_check_secs;
		if(is_overdue)
		{
			return -1;
		}
		debit_abstract = (*it);
		return 0;
	}
    return -1;
}


}

