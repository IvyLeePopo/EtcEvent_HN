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
// ��������:ETCPayUnit::RunBody
//
// ��������:�߳��������壬������
// a�����ͽ�������
// b�����ͽ���״̬��ѯ
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
void ETCPayUnit::RunBody()
{
    SendOneDebit();
    SendDebitStateQuery();
    Sleep(5);
}

//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::IsHTTPIdle
//
// ��������:�жϵ�ǰ,���ʹ��HTTP�Ƿ���Ҫ�Ŷ�
//
// �������:
// �������:bool
// ��д��Ա:���Ǳ�
// ��дʱ��:2018-12-24
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
bool ETCPayUnit::IsHTTPIdle()
{
    CScopeLock pay_lock(&m_PayInfoLock);
    

    //���׶����Ƿ�Ϊ��
    if (!m_DebitQueue.empty())
    {
        TW_TRACE_DIARY_TYPE("HTTP���Ͷ���æµ:�����������ڷ���", TW_TRACE_TYPE_ETCPAYUNIT);
        return false;
    }
    if (m_HasDebitCache)
    {
        return false;
    }
    return true;

    //���׶��л����Ƿ�Ϊ�գ���Ҫ��ѯ�������ݿ�
    sqlite3* sql_conn = DataBaseOpen(GetDebitRequestCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("���׻������ݿ�������⣬�޷���:" + GetDebitRequestCachePath() , TW_TRACE_TYPE_ETCPAYUNIT);
        return false;
    }

    ETCDebitRecord no_use_para ;
    bool has_record = (DataBaseQueryRecord(sql_conn, no_use_para ) == 0);
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("���ݿ�ر�ʧ��", TW_TRACE_TYPE_ETCPAYUNIT);
    }
    if (has_record)
    {
        TW_TRACE_DIARY_TYPE("HTTP���Ͷ���æµ:���ؽ��׻�����Ҫ����", TW_TRACE_TYPE_ETCPAYUNIT);
        return false;
    }
    return true;

}
//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::StartDebitStateQuery
//
// ��������:����һ������״̬��ѯ��
//
//
// �������:const DebitStateQueryParameter& query_info,
// �������:�����ǰ����æ���򷵻�ʧ��
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::StartDebitStateQuery(const DebitStateQueryParameter& query_info)
{
    if (!IsHTTPIdle())
    {
        TW_TRACE_DIARY_TYPE("����æ", TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    //CScopeLock pay_lock(&m_PayInfoLock);
    // m_StateQueryQueue.push_back(query_info);
    return 0;
}

//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::GetRecordFileName
//
// ��������:����ʱ������������ļ���
//
// �������:time_t t,
// �������:std::string
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string ETCPayUnit::GetRecordFileName(time_t t)
{   //�и�ʱ���ַ���
    std::string time_str = Datetime2Str(t);
    std::vector<std::string> time_strs;
    pystring::split(time_str, time_strs, "-");
    if (time_strs.size() < 3)
    {
        return "��֧�ֵ��ļ���";
    }
    std::string record_name = "PayRecord" + time_strs[0] + time_strs[1] + ".db";
    return record_name;
}
//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::GetDebitState
//
// ��������:��ѯָ�����׵Ľ��׽����
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
int ETCPayUnit::GetDebitState(const DebitStateQueryParameter& query_info, ETCDebitRecord& state)
{
    std::string today_record_path = pystring::os::path::join(GetDebitRecordArchiveDir(), GetRecordFileName(ChinaTime()));
    ETCDebitRecord debit_state ;
    {   //������Ϣ
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
        TW_TRACE_DIARY_TYPE("���Ҳ����ý��׼�¼��ʶ����:" + std::string(query_info.PayIdentifier), TW_TRACE_TYPE_ETCPAYUNIT);
        return 255;
    }
    if (QueryRecordInTargetDataBase(yestoday_record_path, debit_state) == 0)
    {
        state = debit_state;
        return 0;
    }
    TW_TRACE_DIARY_TYPE("���Ҳ����ý��׼�¼��ʶ����:" + std::string(query_info.PayIdentifier), TW_TRACE_TYPE_ETCPAYUNIT);
    return 255;
}
//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::DebitMoney
//
// ��������:����۷�
//
// �������:const ETCDebitRecord& debit_para, std::string& result_str, const int& result_type,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::DebitMoney(const ETCDebitRecord& debit_para, std::string& result_str, const int& result_type)
{
	
    if (!IsHTTPIdle())
    {
        TW_TRACE_DIARY_TYPE("����æ", TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
	
    ETCDebitAbstract tmp_abstract;
    if( QueryDebitAbstract(debit_para.VehicleLicense,tmp_abstract) ==0)
    {
        char err_log[200]={0};
        sprintf(err_log,"�ó���������׹����������ظ����ף��ϴν���ʱ��:%s",Datetime2Str(tmp_abstract.DebitTime).c_str());
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_ETCPAYUNIT);
        return 101;  //������Ϣ
    }
    CScopeLock pay_lock(&m_PayInfoLock);
    ETCDebitRecord one_debit = debit_para;
    std::string pay_serial = GetNextPaySerialNo();
    if (pay_serial.size() < 10)
    {
        TW_TRACE_DIARY_TYPE("������Ż�ȡʧ��", TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    strcpy(one_debit.LocalResponse.PaySerialNo, pay_serial.c_str());
    std::string sec_code = GetNetPayRecordMatchCode(one_debit);
    strncpy(one_debit.LocalResponse.TradeSecurityCode, sec_code.c_str(), sizeof(one_debit.LocalResponse.TradeSecurityCode) - 1);
    strcpy(one_debit.LocalResponse.PayIdentifier, one_debit.PayIdentifier);
    one_debit.LocalResponse.DebitTime = one_debit.SubTime;
    one_debit.LocalResponse.PayPlatformType = 2;//Ŀǰ����֧����֧��

    {   //���뵽���ݿ���,�˴�����ʹ��IO������
        if(SaveDebitRequestToIO(one_debit)!=0)
        {
            return -1;
        }
    }
    m_DebitQueue.push_back(one_debit);

    m_HasDebitCache = true;
    std::stringstream ss;
    ss << "�������ݲ�����гɹ�";
    ss << ",ʶ����:" << one_debit.PayIdentifier;
    ss << ",�󶨽��׺�:" << one_debit.LocalResponse.PaySerialNo;
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
        sprintf(err_log, "��֧�ֽ������:%d", result_type);
        TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
	
    return 0;
}

//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::SendOneDebit
//
// ��������:����ʵ�ʵĽ������ݷ��͡�
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
		TW_TRACE_DIARY_TYPE("�����޷��ָ��Ĵ���ֹͣETC֧��" , TW_TRACE_TYPE_ETCPAYUNIT);
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
            {   //û�н��׼�¼��Ҫ�ɴ���
                m_HasDebitCache = false;
                return 0;
            }
            m_HasDebitCache = true;
            m_DebitQueue.push_back(etc_para);
        }
        debit_record = m_DebitQueue.front();
    }
    //������ͳɹ�������ɾ������ǰ��ε�����
    TW_TRACE_DIARY_TYPE("���ͽ������󵽷�����,ʶ����:" + std::string(debit_record.PayIdentifier) , TW_TRACE_TYPE_ETCPAYUNIT);
    std::string http_json_para;
    std::string http_response_json;
    PackETCDebitRequestJson(debit_record, http_json_para);

    std::string utf8_json_str = StringConvertByCodePage(http_json_para, CP_ACP, CP_UTF8);
    if (!m_HttpPipe.Post(GetETCPayURL(), utf8_json_str, http_response_json))
    {
        TW_TRACE_DIARY_TYPE("���紦��ʧ��,������Ϣ:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TYPE_ETCPAYUNIT);
		Sleep(5*1000);
        return -1;
    }
    std::string gbk_recv_str = StringConvertByCodePage(http_response_json, CP_UTF8, CP_ACP);
    if (!ParseETCDebitResponseFromServer(gbk_recv_str, debit_record))
    {
        TW_TRACE_DIARY_TYPE("����������������ʧ��:" + gbk_recv_str , TW_TRACE_TYPE_ETCPAYUNIT);
		Sleep(5*1000);
        return -1;
    }
    // TW_TRACE_DIARY_TYPE("######���׻��������߼���δʵ��:" , TW_TRACE_TYPE_ETCPAYUNIT);

    /**
     * ���ݻ����Ľ��������������
     * 1�����ɽ��׼�¼�����鵵
     * 2���������ͷ���Ƕ�Ӧ���ף���ɾ��
     * 3��ɾ�������еĶ�Ӧ����
     */
    {   //���ý��׼�¼�鵵
        if (SaveDebitRecord(debit_record) == 0)
        {
            TW_TRACE_DIARY_TYPE("���׹鵵�ɹ�,ʶ����:" + std::string(debit_record.PayIdentifier) , TW_TRACE_TYPE_ETCPAYUNIT);
        }
        else
        {
            OccurSystemFatalError();
            TW_TRACE_DIARY_TYPE("���׹鵵ʧ��,ʶ����:" + std::string(debit_record.PayIdentifier) , TW_TRACE_TYPE_ETCPAYUNIT);
            return 0;
        }

    }
    {
        CScopeLock database_lock(&m_DataBaseLock);
        //ɾ�����̵�����
        sqlite3* sql_conn = DataBaseOpen(GetDebitRequestCachePath());
        if (sql_conn == 0)
        {
            TW_TRACE_DIARY_TYPE("�����������ݿ�������⣬�޷���:" + GetDebitRequestCachePath(), TW_TRACE_TYPE_ETCPAYUNIT);
            OccurSystemFatalError();
            return -1;
        }

        if (DataBaseDeleteDebitQuest(sql_conn, debit_record) != 0)
        {
            std::stringstream ss;
            ss << "ʶ����:" << debit_record.PayIdentifier << ",ɾ��ʧ��." << DataBaseGetLastError(sql_conn);
            TW_TRACE_DIARY_TYPE(ss.str(), TW_TRACE_TYPE_ETCPAYUNIT);
            DataBaseClose(sql_conn);
            OccurSystemFatalError();
            return -1;
        }
        if (DataBaseClose(sql_conn) != 0)
        {
            TW_TRACE_DIARY_TYPE("���ݿ�ر�ʧ��:" + GetDebitRequestCachePath(), TW_TRACE_TYPE_ETCPAYUNIT);
            OccurSystemFatalError();
            return -1;
        }
        TW_TRACE_DIARY_TYPE("������������ɹ���ɾ�������󻺴�,ʶ����:" + std::string(debit_record.PayIdentifier), TW_TRACE_TYPE_ETCPAYUNIT);
    }

    {   //ɾ�������е�����
        CScopeLock pay_lock(&m_PayInfoLock);
        
        if (!m_DebitQueue.empty())
        {
            bool is_same_payid = (stricmp (debit_record.PayIdentifier, m_DebitQueue.front().PayIdentifier) == 0);
            if (  is_same_payid )
            {
                m_DebitQueue.pop_front();
                TW_TRACE_DIARY_TYPE("������������ɹ����Ӷ������Ƴ�������,ʶ����:" + std::string(debit_record.PayIdentifier), TW_TRACE_TYPE_ETCPAYUNIT);
            }
            else
            {
                TW_TRACE_DIARY_TYPE("���׶��г����쳣�����ڽ��׵��������ͷ���ݲ�һ��" , TW_TRACE_TYPE_ETCPAYUNIT);
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
// ��������:ETCPayUnit::SendDebitStateQuery
//
// ��������:Ŀǰ����Ҫ�˹���
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
int ETCPayUnit::SendDebitStateQuery()
{
    return 0;//Ŀǰ����Ҫ���Ӵ���ֱ�ӷ���
}
//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::SaveDebitRecord
//
// ��������:���潻�׼�¼�����̡�
//
// �������:const ETCDebitRecord& record,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
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
        TW_TRACE_DIARY_TYPE("���׹鵵���ݿ�������⣬�޷���:" + record_path, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }

    if (
        (DataBaseCreateTable(sql_conn, DataBaseGetDebitRecordTableName(), DataBaseGetDebitRecordTableDefine()) != 0 )
        || (DataBaseAddRecord(sql_conn, record) != 0)
    )
    {
        std::stringstream ss;
        ss << "ʶ����:" << record.PayIdentifier << ",����ʧ��." << DataBaseGetLastError(sql_conn);
        TW_TRACE_DIARY_TYPE(ss.str(), TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
		if (DataBaseClose(sql_conn) != 0)
		{
			TW_TRACE_DIARY_TYPE("���ݿ�ر�ʧ��:" + record_path, TW_TRACE_TYPE_ETCPAYUNIT);
		}
		return -1;
    }
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("���ݿ�ر�ʧ��:" + record_path, TW_TRACE_TYPE_ETCPAYUNIT);
    }
    ClearAbateRecord();
    return 0;
}

//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::ClearAbateRecord
//
// ��������:������ڵĽ��׼�¼�ļ�
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
int ETCPayUnit::ClearAbateRecord()
{
    static time_t last_clear_time = 0;
    time_t curr_time = ChinaTime();
    if ( (curr_time - last_clear_time ) < (24 * 60 * 60))
    {
        return -1;
    }
    TW_TRACE_DIARY_TYPE("�����ڹ鵵�ļ�", TW_TRACE_TYPE_ETCPAYUNIT);
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
            TW_TRACE_DIARY_TYPE("��ȡ�ļ�����ʧ��:" + (*it), TW_TRACE_TYPE_ETCPAYUNIT);
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
            TW_TRACE_DIARY_TYPE("ɾ���鵵�ļ�:" + (*it), TW_TRACE_TYPE_ETCPAYUNIT);
        }
    }
    return 0;


}

//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::HasUnrecoverableError
//
// ��������:��ѯ�Ƿ���ϵͳ����
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
bool ETCPayUnit::HasUnrecoverableError()
{
    // CScopeLock pay_lock(&m_PayInfoLock);
    return m_IsSystemFatalError;
}

//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::OccurSystemFatalError
//
// ��������:����ϵͳ����
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
int ETCPayUnit::OccurSystemFatalError()
{
    //CScopeLock pay_lock(&m_PayInfoLock);
    TW_TRACE_DIARY_TYPE("�����޷��Ӵ����лָ�", TW_TRACE_TYPE_ETCPAYUNIT);
    m_IsSystemFatalError = true;
    return 0;
}

//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::GetNextPaySerialNo
//
// ��������:��ȡ������ţ�ֻ��Ҫ����Ψһ���ɣ�ʹ��guid
//
// �������:
// �������:std::string
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string ETCPayUnit::GetNextPaySerialNo()
{
    return GetGuid();
}

//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::QueryRecordInTargetDataBase
//
// ��������:��ָ�����ݿ��в�ѯ���׽����
//
// �������:const std::string& database_file,ETCDebitRecord&debit_state,
// �������:0����ѯ�ɹ�
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::QueryRecordInTargetDataBase(const std::string& database_file, ETCDebitRecord&debit_state)
{
    CScopeLock database_lock(&m_DataBaseLock);
    sqlite3* sql_conn = DataBaseOpen(database_file);
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("���׽�����ݿ�������⣬�޷���:" + database_file, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    bool has_record = (DataBaseQueryRecord(sql_conn, debit_state ) == 0);
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("���ݿ�ر�ʧ��:" + database_file, TW_TRACE_TYPE_ETCPAYUNIT);
    }
    if (has_record)
    {
        return 0;
    }
    return -1;
}


//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::MoveDebitRequestFromIOToDataBase
//
// ��������:��IO�еĽ������󻺴棬���浽���ݿ�
//
// �������:const ETCDebitRecord& record,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-17
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
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
        TW_TRACE_DIARY_TYPE("����������ʧ�ܣ��ļ���ʧ��:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
        return -1;
    }
    if (fread(&one_debit, sizeof(one_debit), 1, fid) != 1)
    {
        TW_TRACE_DIARY_TYPE("����������ʧ�ܣ�������ʧ��:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        fclose(fid);
        OccurSystemFatalError();
        return -1;
    }
    fclose(fid);
    if(remove(tmp_para_path.c_str())!=0)
    {
        TW_TRACE_DIARY_TYPE("�����ļ�ʧ��:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
        return -1;
    }

    sqlite3* sql_conn = DataBaseOpen(GetDebitRequestCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("�����������ݿ�������⣬�޷���:" + GetDebitRequestCachePath(), TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
        return -1;
    }

    if ((DataBaseCreateTable(sql_conn, DataBaseGetDebitRequestTableName(), DataBaseGetDebitQuestTableDefine()) != 0)
            || (DataBaseAddDebitQuest(sql_conn, one_debit) != 0)
       )
    {
        std::stringstream ss;
        ss << "ʶ����:" << one_debit.PayIdentifier << ",����ʧ��." << DataBaseGetLastError(sql_conn);
        TW_TRACE_DIARY_TYPE(ss.str(), TW_TRACE_TYPE_ETCPAYUNIT);
        OccurSystemFatalError();
        return -1;
    }
    else
    {
        std::stringstream ss;
        ss << "�����ݴ�IOת�Ƶ����ݿ�ɹ�:" << one_debit.PayIdentifier ;
        TW_TRACE_DIARY_TYPE(ss.str(), TW_TRACE_TYPE_ETCPAYUNIT);
    }
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("���ݿ�ر�ʧ��:" + GetDebitRequestCachePath(), TW_TRACE_TYPE_ETCPAYUNIT);
    }

    return 0;
}

//////////////////////////////////////////////////////////////
// ��������:ETCPayUnit::SaveDebitRequestToIO
//
// ��������:���������󱣴浽IO
//
// �������:const ETCDebitRecord& record,
// �������:int
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-17
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int ETCPayUnit::SaveDebitRequestToIO(const ETCDebitRecord& one_debit)
{
    std::string tmp_para_path = GetDebitParaTMPCachePath();
    if (access(tmp_para_path.c_str(), 0) == 0)
    {
        TW_TRACE_DIARY_TYPE("����δ����Ľ�������:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    FILE * fid = fopen(tmp_para_path.c_str(), "wb");
    if (fid == 0)
    {
        TW_TRACE_DIARY_TYPE("д��������ʧ�ܣ��ļ���ʧ��:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
        return -1;
    }
    if (fwrite(&one_debit, sizeof(one_debit), 1, fid) != 1)
    {
        TW_TRACE_DIARY_TYPE("д��������ʧ�ܣ�дʧ��:" + tmp_para_path, TW_TRACE_TYPE_ETCPAYUNIT);
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

