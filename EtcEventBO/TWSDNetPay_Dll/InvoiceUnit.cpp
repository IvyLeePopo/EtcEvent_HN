#include "stdafx.h"
#include "InvoiceUnit.h"
#include "RunParameter.h"
#include "SystemDiary.h"
#include "BaseDefine.h"
#include "TWSDNetPay_Dll.h"
#include "JsonHelper.h"
#include "TypeHelper.h"
#include "DataBaseHelper.h"
#include "SimpleUtil.h"
#include <sstream>
using namespace trawe;

CInvoiceUnit::CInvoiceUnit()
{
    ::InitializeCriticalSection(&m_InvoiceLock);
    m_HttpPipe.ChangeRemotePeer(GetInvoicePlateformIP(), GetInvoicePlateformPort());
	UrlPrefix = GetInvoiceUrlPrefix();
}
CInvoiceUnit::~CInvoiceUnit()
{
    Stop();

    for (std::list<InvoiceHttpSync*>::iterator it = m_AbandonInvoiceGroup.begin(); it != m_AbandonInvoiceGroup.end(); ++it)
    {
        if ( (*it)->IsIdle() )
        {
            delete (*it);
        }
    }
    m_AbandonInvoiceGroup.clear();

    ::DeleteCriticalSection(&m_InvoiceLock);
}
void CInvoiceUnit::RunBody()
{	
    //处理发票数据传送
    ExecuteOneInvoiceTransmit();
	//处理发票数据删除
	ExecuteOneInvoiceCancel();

    ClearAbandonInvoice();
    Sleep(10);
}

int CInvoiceUnit::StartNewInvoiceTransmit(InvoiceParameter&param)
{
    {
        trawe::CScopeLock invoice_lock(&m_InvoiceLock);

		//构造二维码数据
		std::stringstream ss;
		ss << param.ElectronicInvoiceNo << "&";
		ss << "h=" << param.ExitTime;	
		memset((void *)param.QRcode, 0, sizeof(param.QRcode));
		strncpy(param.QRcode, UrlPrefix.c_str(), sizeof(param.QRcode));
		strcat(param.QRcode, "?parameter=");
		strcat(param.QRcode, ss.str().c_str());
		TW_TRACE_DIARY_TYPE("控制设备显示发票二维码", TW_TRACE_TPPE_INVOICEUNIT);
		SetQrcodeDisplay(param);

        m_InvoiceGroup.push_back(param);
    }
    return 0;
}

int CInvoiceUnit::StartInvoiceCancel(InvoiceCancelParameter&param)
{
    {
        trawe::CScopeLock invoice_lock(&m_InvoiceLock);
        m_InvoiceCancelGroup.push_back(param);
    }
    return 0;
}

void CInvoiceUnit::ExecuteOneInvoiceTransmit()
{
	bool bInvoiceTransimtSuccess = false;
	int RetCode = 255;
	int InvoiceExecuteState = 0; //流水操作执行状态, 70：流水上传成功，71：流水撤销成功，72：流水平台网络不通，73：流水平台反馈处理失败，74：流水平台超时无应答

    InvoiceParameter InvoicePara;
    {
        trawe::CScopeLock pay_lock(&m_InvoiceLock);
        if (m_InvoiceGroup.empty())
        {
			if(!GetOneInvoiceFromDB())
				return; //当前内存和数据库中皆无发票数据需要发送
        }
        InvoicePara = m_InvoiceGroup.front();
		m_InvoiceGroup.pop_front(); //删除list中的第一个位置的元素
    }

/*
	//构造二维码数据
	std::stringstream ss;
	ss << InvoicePara.ElectronicInvoiceNo << "&";
	ss << "h=" << InvoicePara.ExitTime;
	
	memset((void *)InvoicePara.QRcode, 0, sizeof(InvoicePara.QRcode));
	strncpy(InvoicePara.QRcode, UrlPrefix.c_str(), sizeof(InvoicePara.QRcode));
	strcat(InvoicePara.QRcode, "?parameter=");
	strcat(InvoicePara.QRcode, ss.str().c_str());

	//在终端设备上显示二维码
	if(InvoicePara.IsDataBaseRecord != 1)
	{
		TW_TRACE_DIARY_TYPE("控制设备显示发票二维码", TW_TRACE_TPPE_INVOICEUNIT);
		SetQrcodeDisplay(InvoicePara);
	}
*/

	//添加传送处理代码
	{
        TW_TRACE_DIARY_TYPE("传送发票数据到服务器", TW_TRACE_TPPE_INVOICEUNIT);
        //发起网络请求
        InvoiceHttpSync *ihs = new InvoiceHttpSync();
        if (ihs == 0)
        {
            TW_TRACE_DIARY_TYPE("创建发票传送处理流程失败", TW_TRACE_TPPE_INVOICEUNIT);
			return;
        }
        ihs->StartTransmit(InvoicePara);
        ihs->Resume();
        {
            trawe::CScopeLock invoice_lock(&m_InvoiceLock);
            m_AbandonInvoiceGroup.push_back(ihs);
        }
        Sleep(50);
        std::string Response;
		InvoiceResultParameter ResultParam;
		const int countdown_msecs = 20000;
        const int check_duration_msecs = 1000;
		Util::CountDown invoice_cd(countdown_msecs);
		while (!invoice_cd.Over())
        {
            Sleep(check_duration_msecs);
            RetCode = ihs->TransmitState(Response);
            if (RetCode == -1)
            {   
                continue;
            }
			
            if (!ParseInvoiceResultFromJson(Response, ResultParam))
            {
                TW_TRACE_DIARY_TYPE("解析发票服务器回馈结果失败", TW_TRACE_TPPE_INVOICEUNIT);
				InvoiceExecuteState = 73;
				if(InvoicePara.IsDataBaseRecord != 1)
					AddInvoice2DB(InvoicePara);
				break;
            }
            else
            {
                if (RetCode == 0)
                {   //传送成功
                    TW_TRACE_DIARY_TYPE("收到发票服务器回馈，上传发票流水成功", TW_TRACE_TPPE_INVOICEUNIT);
					InvoiceExecuteState = 70;
					if(InvoicePara.IsDataBaseRecord == 1)
						DeleteInvoiceFromDB(InvoicePara);
                }
                else
                {   //传送失败
                    char err_log[100] = {0};
                    sprintf(err_log, "收到发票服务器回馈，上传发票流水失败(回馈码:%d)", ResultParam.ExecuteCode);
                    TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TPPE_INVOICEUNIT);
					InvoiceExecuteState = 73;
					if(InvoicePara.IsDataBaseRecord != 1)
						AddInvoice2DB(InvoicePara);
                }
				break;
            }
        }
        if (invoice_cd.Over())//超时没有得到结果的处理
        {
            TW_TRACE_DIARY_TYPE("发票数据传送超时", TW_TRACE_TPPE_INVOICEUNIT);
			InvoiceExecuteState = 74;
			if(InvoicePara.IsDataBaseRecord != 1)
				AddInvoice2DB(InvoicePara);
        }
    }
	if(InvoicePara.NeedNotifyResult)
	{
		NotifyResult(7, InvoiceExecuteState);
	}
    trawe::CScopeLock invoice_lock(&m_InvoiceLock);
    //m_InvoiceGroup.clear();
}


void CInvoiceUnit::ExecuteOneInvoiceCancel()
{
	InvoiceCancelParameter CancelParam;
    {
        trawe::CScopeLock invoice_lock(&m_InvoiceLock);
        if (m_InvoiceCancelGroup.empty())
        {   			
			if(!GetOneInvoiceCancelFromDB())
				return; //当前内存和数据库中皆无撤单发票数据需要发送
        }
        CancelParam = m_InvoiceCancelGroup.front();
        m_InvoiceCancelGroup.pop_front();
    }

    // 开始进行删除发票操作
    std::string CancleJson;
    PackInvoiceCancelHttpJson(CancelParam, CancleJson);
    std::string utf8_json_str = StringConvertByCodePage(CancleJson, CP_ACP, CP_UTF8);
	TW_TRACE_DIARY_TYPE("发起撤销流水请求: " + utf8_json_str, TW_TRACE_TPPE_INVOICEUNIT);
    std::string recv_str;

	static unsigned short changeIP = 0;
	//默认使用第一个IP，当需要改变时，切换另一个IP进行支付
	m_HttpPipe.ChangeRemotePeer((changeIP % 2) ? GetInvoicePlateformSecondIP() : GetInvoicePlateformIP(), GetInvoicePlateformPort());
	char sLog[128] = {0};
	sprintf_s(sLog, "发送撤销流水请求地址:  http://%s:%d/ElectronicInvoice/delete", m_HttpPipe.m_IpStr.c_str(), m_HttpPipe.m_Port);
	TW_TRACE_DIARY_TYPE(std::string(sLog), TW_TRACE_TPPE_INVOICEHTTP);

    if (!m_HttpPipe.Post("/ElectronicInvoice/delete", utf8_json_str, recv_str))
    {
		//当前ip端口发送请求失败，在此处决定切换ip
		changeIP++;

        TW_TRACE_DIARY_TYPE("发送撤销流水请求失败:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TPPE_INVOICEHTTP);
		if(CancelParam.IsDataBaseRecord != 1)
			AddInvoiceCancel2DB(CancelParam);

        return;
    }
    std::string gbk_recv_str = StringConvertByCodePage(recv_str, CP_UTF8, CP_ACP);
    InvoiceCancelResultParameter cancel_result;
    if (!ParseInvoiceCancleResultFromServer(gbk_recv_str, cancel_result))
    {
		//当前ip端口发送请求失败，在此处决定切换ip
		changeIP++;

        TW_TRACE_DIARY_TYPE("解析服务器的撤销流水回馈失败:" + gbk_recv_str, TW_TRACE_TPPE_INVOICEHTTP);
		if(CancelParam.IsDataBaseRecord != 1)
			AddInvoiceCancel2DB(CancelParam);
    }
   
    if (cancel_result.ExecuteCode != 0)
    {
        TW_TRACE_DIARY_TYPE("撤销流水数据失败:" + gbk_recv_str , TW_TRACE_TPPE_INVOICEHTTP);
		if(CancelParam.IsDataBaseRecord != 1)
			AddInvoiceCancel2DB(CancelParam);
		return;
    }
	TW_TRACE_DIARY_TYPE("撤销流水数据成功,电子发票序列号:" + std::string(cancel_result.ElectronicInvoiceNo) , TW_TRACE_TPPE_INVOICEHTTP);
	if(CancelParam.IsDataBaseRecord == 1)
		DeleteInvoiceCancelFromDB(CancelParam);
	//通知车道的发票撤销结果是成功的
	if(CancelParam.NeedNotifyResult)
	{
		NotifyResult(7, 71);
	}

    return;
}

int CInvoiceUnit::ClearAbandonInvoice()
{
    static time_t last_check_time = time(NULL);
    time_t curr_time = time(NULL);
    bool enable_check = (curr_time - last_check_time) > 90;
    if ( !enable_check)
    {
        return -1;
    }
    last_check_time = curr_time;
    trawe::CScopeLock Invoice_lock(&m_InvoiceLock);
    std::list<InvoiceHttpSync*> under_way_invoice;
    for (std::list<InvoiceHttpSync*>::iterator it = m_AbandonInvoiceGroup.begin(); it != m_AbandonInvoiceGroup.end(); ++it)
    {
        if ((*it)->IsIdle())
        {
            delete (*it);
        }
        else
        {
            under_way_invoice.push_back(*it);
        }
    }
    m_AbandonInvoiceGroup = under_way_invoice;
    int abandon_size = static_cast<int>(m_AbandonInvoiceGroup.size());
    if (abandon_size > 0)
    {
        char info_log[100] = {0};
        sprintf(info_log, "存在%d个过期发票请求", abandon_size);
        TW_TRACE_DIARY_TYPE(info_log, TW_TRACE_TPPE_INVOICEUNIT);
    }
    return 0;
}

int CInvoiceUnit::NotifyResult(int option, int result)
{
    char info_log[100] = {0};
    sprintf(info_log, "通知调用者发票数据处理结果,功能代码:%d,执行结果:%d", option, result);
    TW_TRACE_DIARY_TYPE(info_log, TW_TRACE_TPPE_INVOICEUNIT);
    GetGlobalApp()->NotifyCaller(option, result);
    return 0;
}

int CInvoiceUnit::SetQrcodeDisplay(InvoiceParameter&param)
{
	if (GetGlobalApp()->m_pThreadManage == 0 || GetGlobalApp()->m_pThreadManage->m_pThreadDevice == 0)
    {
        TW_TRACE_DIARY("发生错误，没有连接设备，无法进行发票二维码显示");
        return -1;
    }

	Json::Value root;
	Json::Value ElectronicInvoiceStart;
	Json::Value Data;

	std::string str = param.ElectronicInvoiceNo;
	std::string sn  = str.substr(str.length() - 8);
	ElectronicInvoiceStart["ElectronicInvoiceNo"] = Json::Value(sn);

	Data["QRcode"] = Json::Value(param.QRcode);
	Data["VehicleLicense"] = Json::Value(param.VehicleLicense);
	Data["Money"] = Json::Value(param.Money);
	Data["PaymentMethod"] = Json::Value(param.PayMode);
	Data["ExitStation"] = Json::Value(param.PayStationName);
	Data["ExitStationTime"] = Json::Value(param.ExitTime);
	ElectronicInvoiceStart["Data"] = Data;

	root["ElectronicInvoiceStart"] = ElectronicInvoiceStart;

	//转化为字符串
	CString strReturn;
	Json::FastWriter fast_writer;
	strReturn.Format(_T("%s"),fast_writer.write(root).c_str());

    CString* p_str = new CString();
    if (p_str == 0)
    {
        TW_TRACE_DIARY_TYPE("发票二维码显示，内存申请失败", TW_TRACE_TPPE_INVOICEUNIT);
        return -1;
    }
    p_str->Format(_T("%s"), strReturn);
    GetGlobalApp()->m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_MANAGE_INVOICE_QRCODE_DISPLAY, (WPARAM)p_str, (LPARAM)0);

	return 0;
}

void CInvoiceUnit::AddInvoiceCancel2DB(InvoiceCancelParameter&param)
{   
	//将数据保存到磁盘
    std::string cancle_cache_path = GetInvoiceCancleCachePath();
    sqlite3* sql_conn = DataBaseOpen(cancle_cache_path);
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("发票撤销数据库打开失败:" + cancle_cache_path, TW_TRACE_TPPE_INVOICEUNIT);
        return;
    }

    if ((DataBaseCreateTable(sql_conn, DataBaseGetInvoiceCancleTableName(), DataBaseGetInvoiceCancleTableDefine()) != 0 )||(DataBaseAdd(sql_conn, param) != 0))
    {
        TW_TRACE_DIARY_TYPE("发票撤销数据插入数据库失败，发票序列号:" + std::string(param.ElectronicInvoiceNo), TW_TRACE_TPPE_INVOICEUNIT);
    }
    else
    {
        TW_TRACE_DIARY_TYPE("发票撤销数据插入数据库成功，发票序列号：" + std::string(param.ElectronicInvoiceNo), TW_TRACE_TPPE_INVOICEUNIT);
    }
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("发票撤销数据库关闭失败:" + cancle_cache_path, TW_TRACE_TPPE_INVOICEUNIT);
    }

	return;
}

void CInvoiceUnit::AddInvoice2DB(InvoiceParameter&param)
{   
	//将数据保存到磁盘
    std::string cache_path = GetInvoiceCachePath();
    sqlite3* sql_conn = DataBaseOpen(cache_path);
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("发票上传数据库打开失败:" + cache_path, TW_TRACE_TPPE_INVOICEUNIT);
        return;
    }

    if ((DataBaseCreateTable(sql_conn, DataBaseGetInvoiceTableName(), DataBaseGetInvoiceTableDefine()) != 0 )||(DataBaseAdd(sql_conn, param) != 0))
    {
        TW_TRACE_DIARY_TYPE("发票上传数据插入数据库失败，发票序列号:" + std::string(param.ElectronicInvoiceNo), TW_TRACE_TPPE_INVOICEUNIT);
    }
    else
    {
        TW_TRACE_DIARY_TYPE("发票上传数据插入数据库成功，发票序列号：" + std::string(param.ElectronicInvoiceNo), TW_TRACE_TPPE_INVOICEUNIT);
    }
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("发票上传数据库关闭失败:" + cache_path, TW_TRACE_TPPE_INVOICEUNIT);
    }

	return;
}

void CInvoiceUnit::DeleteInvoiceCancelFromDB(InvoiceCancelParameter&param)
{
	trawe::CScopeLock invoice_lock(&m_InvoiceLock);
    sqlite3* sql_conn = DataBaseOpen(GetInvoiceCancleCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("打开发票撤销数据库失败" + GetInvoiceCancleCachePath(), TW_TRACE_TPPE_INVOICEUNIT);
        return;
    }
    int db_ret = DataBaseDelete(sql_conn, param);
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("关闭发票撤销数据库失败" + GetInvoiceCancleCachePath(), TW_TRACE_TPPE_INVOICEUNIT);
    }
    return;
}

void CInvoiceUnit::DeleteInvoiceFromDB(InvoiceParameter&param)
{
	trawe::CScopeLock invoice_lock(&m_InvoiceLock);
    sqlite3* sql_conn = DataBaseOpen(GetInvoiceCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("打开发票上传数据库失败" + GetInvoiceCachePath(), TW_TRACE_TPPE_INVOICEUNIT);
        return;
    }
    int db_ret = DataBaseDelete(sql_conn, param);
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("关闭发票上传数据库失败" + GetInvoiceCachePath(), TW_TRACE_TPPE_INVOICEUNIT);
    }
    return;
}

bool CInvoiceUnit::GetOneInvoiceCancelFromDB()
{
    static time_t last_check_time = 0;
    time_t curr_time = time(NULL);
    if ( (curr_time - last_check_time) < 60*5 )
    {
        return false;
    }
    last_check_time = curr_time;

    trawe::CScopeLock invoice_lock(&m_InvoiceLock);
	InvoiceCancelParameter invoice_cancle;
    sqlite3* sql_conn = DataBaseOpen(GetInvoiceCancleCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("打开发票撤销数据库失败", TW_TRACE_TPPE_INVOICEUNIT);
        return false;
    }
    int db_ret = DataBaseQuery(sql_conn, invoice_cancle);
    DataBaseClose(sql_conn);
    if (db_ret != 0)
    {
        return false;
    }

    char hint_log[200] = {0};
    _snprintf(hint_log, 150, "从发票撤销数据库中获取一条数据，发票序列号:%s", invoice_cancle.ElectronicInvoiceNo);
    TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_INVOICEUNIT);
    m_InvoiceCancelGroup.push_back(invoice_cancle);
    return true;
}


bool CInvoiceUnit::GetOneInvoiceFromDB()
{
	static time_t last_check_time = 0;
    time_t curr_time = time(NULL);
    if ( (curr_time - last_check_time) < 60 )
    {
        return false;
    }
    last_check_time = curr_time;

    trawe::CScopeLock invoice_lock(&m_InvoiceLock);
	InvoiceParameter invoice;
    sqlite3* sql_conn = DataBaseOpen(GetInvoiceCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("打开发票上传数据库失败", TW_TRACE_TPPE_INVOICEUNIT);
        return false;
    }
    int db_ret = DataBaseQuery(sql_conn, invoice);
    DataBaseClose(sql_conn);
    if (db_ret != 0)
    {
        return false;
    }

    char hint_log[156] = {0};
    _snprintf(hint_log, 150, "从发票上传数据库中获取一条数据，发票序列号:%s", invoice.ElectronicInvoiceNo);
    TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_INVOICEUNIT);
    m_InvoiceGroup.push_back(invoice);
    return true;
}