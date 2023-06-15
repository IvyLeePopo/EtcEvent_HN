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
    //����Ʊ���ݴ���
    ExecuteOneInvoiceTransmit();
	//����Ʊ����ɾ��
	ExecuteOneInvoiceCancel();

    ClearAbandonInvoice();
    Sleep(10);
}

int CInvoiceUnit::StartNewInvoiceTransmit(InvoiceParameter&param)
{
    {
        trawe::CScopeLock invoice_lock(&m_InvoiceLock);

		//�����ά������
		std::stringstream ss;
		ss << param.ElectronicInvoiceNo << "&";
		ss << "h=" << param.ExitTime;	
		memset((void *)param.QRcode, 0, sizeof(param.QRcode));
		strncpy(param.QRcode, UrlPrefix.c_str(), sizeof(param.QRcode));
		strcat(param.QRcode, "?parameter=");
		strcat(param.QRcode, ss.str().c_str());
		TW_TRACE_DIARY_TYPE("�����豸��ʾ��Ʊ��ά��", TW_TRACE_TPPE_INVOICEUNIT);
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
	int InvoiceExecuteState = 0; //��ˮ����ִ��״̬, 70����ˮ�ϴ��ɹ���71����ˮ�����ɹ���72����ˮƽ̨���粻ͨ��73����ˮƽ̨��������ʧ�ܣ�74����ˮƽ̨��ʱ��Ӧ��

    InvoiceParameter InvoicePara;
    {
        trawe::CScopeLock pay_lock(&m_InvoiceLock);
        if (m_InvoiceGroup.empty())
        {
			if(!GetOneInvoiceFromDB())
				return; //��ǰ�ڴ�����ݿ��н��޷�Ʊ������Ҫ����
        }
        InvoicePara = m_InvoiceGroup.front();
		m_InvoiceGroup.pop_front(); //ɾ��list�еĵ�һ��λ�õ�Ԫ��
    }

/*
	//�����ά������
	std::stringstream ss;
	ss << InvoicePara.ElectronicInvoiceNo << "&";
	ss << "h=" << InvoicePara.ExitTime;
	
	memset((void *)InvoicePara.QRcode, 0, sizeof(InvoicePara.QRcode));
	strncpy(InvoicePara.QRcode, UrlPrefix.c_str(), sizeof(InvoicePara.QRcode));
	strcat(InvoicePara.QRcode, "?parameter=");
	strcat(InvoicePara.QRcode, ss.str().c_str());

	//���ն��豸����ʾ��ά��
	if(InvoicePara.IsDataBaseRecord != 1)
	{
		TW_TRACE_DIARY_TYPE("�����豸��ʾ��Ʊ��ά��", TW_TRACE_TPPE_INVOICEUNIT);
		SetQrcodeDisplay(InvoicePara);
	}
*/

	//��Ӵ��ʹ������
	{
        TW_TRACE_DIARY_TYPE("���ͷ�Ʊ���ݵ�������", TW_TRACE_TPPE_INVOICEUNIT);
        //������������
        InvoiceHttpSync *ihs = new InvoiceHttpSync();
        if (ihs == 0)
        {
            TW_TRACE_DIARY_TYPE("������Ʊ���ʹ�������ʧ��", TW_TRACE_TPPE_INVOICEUNIT);
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
                TW_TRACE_DIARY_TYPE("������Ʊ�������������ʧ��", TW_TRACE_TPPE_INVOICEUNIT);
				InvoiceExecuteState = 73;
				if(InvoicePara.IsDataBaseRecord != 1)
					AddInvoice2DB(InvoicePara);
				break;
            }
            else
            {
                if (RetCode == 0)
                {   //���ͳɹ�
                    TW_TRACE_DIARY_TYPE("�յ���Ʊ�������������ϴ���Ʊ��ˮ�ɹ�", TW_TRACE_TPPE_INVOICEUNIT);
					InvoiceExecuteState = 70;
					if(InvoicePara.IsDataBaseRecord == 1)
						DeleteInvoiceFromDB(InvoicePara);
                }
                else
                {   //����ʧ��
                    char err_log[100] = {0};
                    sprintf(err_log, "�յ���Ʊ�������������ϴ���Ʊ��ˮʧ��(������:%d)", ResultParam.ExecuteCode);
                    TW_TRACE_DIARY_TYPE(err_log, TW_TRACE_TPPE_INVOICEUNIT);
					InvoiceExecuteState = 73;
					if(InvoicePara.IsDataBaseRecord != 1)
						AddInvoice2DB(InvoicePara);
                }
				break;
            }
        }
        if (invoice_cd.Over())//��ʱû�еõ�����Ĵ���
        {
            TW_TRACE_DIARY_TYPE("��Ʊ���ݴ��ͳ�ʱ", TW_TRACE_TPPE_INVOICEUNIT);
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
				return; //��ǰ�ڴ�����ݿ��н��޳�����Ʊ������Ҫ����
        }
        CancelParam = m_InvoiceCancelGroup.front();
        m_InvoiceCancelGroup.pop_front();
    }

    // ��ʼ����ɾ����Ʊ����
    std::string CancleJson;
    PackInvoiceCancelHttpJson(CancelParam, CancleJson);
    std::string utf8_json_str = StringConvertByCodePage(CancleJson, CP_ACP, CP_UTF8);
	TW_TRACE_DIARY_TYPE("��������ˮ����: " + utf8_json_str, TW_TRACE_TPPE_INVOICEUNIT);
    std::string recv_str;

	static unsigned short changeIP = 0;
	//Ĭ��ʹ�õ�һ��IP������Ҫ�ı�ʱ���л���һ��IP����֧��
	m_HttpPipe.ChangeRemotePeer((changeIP % 2) ? GetInvoicePlateformSecondIP() : GetInvoicePlateformIP(), GetInvoicePlateformPort());
	char sLog[128] = {0};
	sprintf_s(sLog, "���ͳ�����ˮ�����ַ:  http://%s:%d/ElectronicInvoice/delete", m_HttpPipe.m_IpStr.c_str(), m_HttpPipe.m_Port);
	TW_TRACE_DIARY_TYPE(std::string(sLog), TW_TRACE_TPPE_INVOICEHTTP);

    if (!m_HttpPipe.Post("/ElectronicInvoice/delete", utf8_json_str, recv_str))
    {
		//��ǰip�˿ڷ�������ʧ�ܣ��ڴ˴������л�ip
		changeIP++;

        TW_TRACE_DIARY_TYPE("���ͳ�����ˮ����ʧ��:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TPPE_INVOICEHTTP);
		if(CancelParam.IsDataBaseRecord != 1)
			AddInvoiceCancel2DB(CancelParam);

        return;
    }
    std::string gbk_recv_str = StringConvertByCodePage(recv_str, CP_UTF8, CP_ACP);
    InvoiceCancelResultParameter cancel_result;
    if (!ParseInvoiceCancleResultFromServer(gbk_recv_str, cancel_result))
    {
		//��ǰip�˿ڷ�������ʧ�ܣ��ڴ˴������л�ip
		changeIP++;

        TW_TRACE_DIARY_TYPE("�����������ĳ�����ˮ����ʧ��:" + gbk_recv_str, TW_TRACE_TPPE_INVOICEHTTP);
		if(CancelParam.IsDataBaseRecord != 1)
			AddInvoiceCancel2DB(CancelParam);
    }
   
    if (cancel_result.ExecuteCode != 0)
    {
        TW_TRACE_DIARY_TYPE("������ˮ����ʧ��:" + gbk_recv_str , TW_TRACE_TPPE_INVOICEHTTP);
		if(CancelParam.IsDataBaseRecord != 1)
			AddInvoiceCancel2DB(CancelParam);
		return;
    }
	TW_TRACE_DIARY_TYPE("������ˮ���ݳɹ�,���ӷ�Ʊ���к�:" + std::string(cancel_result.ElectronicInvoiceNo) , TW_TRACE_TPPE_INVOICEHTTP);
	if(CancelParam.IsDataBaseRecord == 1)
		DeleteInvoiceCancelFromDB(CancelParam);
	//֪ͨ�����ķ�Ʊ��������ǳɹ���
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
        sprintf(info_log, "����%d�����ڷ�Ʊ����", abandon_size);
        TW_TRACE_DIARY_TYPE(info_log, TW_TRACE_TPPE_INVOICEUNIT);
    }
    return 0;
}

int CInvoiceUnit::NotifyResult(int option, int result)
{
    char info_log[100] = {0};
    sprintf(info_log, "֪ͨ�����߷�Ʊ���ݴ�����,���ܴ���:%d,ִ�н��:%d", option, result);
    TW_TRACE_DIARY_TYPE(info_log, TW_TRACE_TPPE_INVOICEUNIT);
    GetGlobalApp()->NotifyCaller(option, result);
    return 0;
}

int CInvoiceUnit::SetQrcodeDisplay(InvoiceParameter&param)
{
	if (GetGlobalApp()->m_pThreadManage == 0 || GetGlobalApp()->m_pThreadManage->m_pThreadDevice == 0)
    {
        TW_TRACE_DIARY("��������û�������豸���޷����з�Ʊ��ά����ʾ");
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

	//ת��Ϊ�ַ���
	CString strReturn;
	Json::FastWriter fast_writer;
	strReturn.Format(_T("%s"),fast_writer.write(root).c_str());

    CString* p_str = new CString();
    if (p_str == 0)
    {
        TW_TRACE_DIARY_TYPE("��Ʊ��ά����ʾ���ڴ�����ʧ��", TW_TRACE_TPPE_INVOICEUNIT);
        return -1;
    }
    p_str->Format(_T("%s"), strReturn);
    GetGlobalApp()->m_pThreadManage->m_pThreadDevice->PostThreadMessage(WM_THREAD_MANAGE_INVOICE_QRCODE_DISPLAY, (WPARAM)p_str, (LPARAM)0);

	return 0;
}

void CInvoiceUnit::AddInvoiceCancel2DB(InvoiceCancelParameter&param)
{   
	//�����ݱ��浽����
    std::string cancle_cache_path = GetInvoiceCancleCachePath();
    sqlite3* sql_conn = DataBaseOpen(cancle_cache_path);
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("��Ʊ�������ݿ��ʧ��:" + cancle_cache_path, TW_TRACE_TPPE_INVOICEUNIT);
        return;
    }

    if ((DataBaseCreateTable(sql_conn, DataBaseGetInvoiceCancleTableName(), DataBaseGetInvoiceCancleTableDefine()) != 0 )||(DataBaseAdd(sql_conn, param) != 0))
    {
        TW_TRACE_DIARY_TYPE("��Ʊ�������ݲ������ݿ�ʧ�ܣ���Ʊ���к�:" + std::string(param.ElectronicInvoiceNo), TW_TRACE_TPPE_INVOICEUNIT);
    }
    else
    {
        TW_TRACE_DIARY_TYPE("��Ʊ�������ݲ������ݿ�ɹ�����Ʊ���кţ�" + std::string(param.ElectronicInvoiceNo), TW_TRACE_TPPE_INVOICEUNIT);
    }
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("��Ʊ�������ݿ�ر�ʧ��:" + cancle_cache_path, TW_TRACE_TPPE_INVOICEUNIT);
    }

	return;
}

void CInvoiceUnit::AddInvoice2DB(InvoiceParameter&param)
{   
	//�����ݱ��浽����
    std::string cache_path = GetInvoiceCachePath();
    sqlite3* sql_conn = DataBaseOpen(cache_path);
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("��Ʊ�ϴ����ݿ��ʧ��:" + cache_path, TW_TRACE_TPPE_INVOICEUNIT);
        return;
    }

    if ((DataBaseCreateTable(sql_conn, DataBaseGetInvoiceTableName(), DataBaseGetInvoiceTableDefine()) != 0 )||(DataBaseAdd(sql_conn, param) != 0))
    {
        TW_TRACE_DIARY_TYPE("��Ʊ�ϴ����ݲ������ݿ�ʧ�ܣ���Ʊ���к�:" + std::string(param.ElectronicInvoiceNo), TW_TRACE_TPPE_INVOICEUNIT);
    }
    else
    {
        TW_TRACE_DIARY_TYPE("��Ʊ�ϴ����ݲ������ݿ�ɹ�����Ʊ���кţ�" + std::string(param.ElectronicInvoiceNo), TW_TRACE_TPPE_INVOICEUNIT);
    }
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("��Ʊ�ϴ����ݿ�ر�ʧ��:" + cache_path, TW_TRACE_TPPE_INVOICEUNIT);
    }

	return;
}

void CInvoiceUnit::DeleteInvoiceCancelFromDB(InvoiceCancelParameter&param)
{
	trawe::CScopeLock invoice_lock(&m_InvoiceLock);
    sqlite3* sql_conn = DataBaseOpen(GetInvoiceCancleCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("�򿪷�Ʊ�������ݿ�ʧ��" + GetInvoiceCancleCachePath(), TW_TRACE_TPPE_INVOICEUNIT);
        return;
    }
    int db_ret = DataBaseDelete(sql_conn, param);
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("�رշ�Ʊ�������ݿ�ʧ��" + GetInvoiceCancleCachePath(), TW_TRACE_TPPE_INVOICEUNIT);
    }
    return;
}

void CInvoiceUnit::DeleteInvoiceFromDB(InvoiceParameter&param)
{
	trawe::CScopeLock invoice_lock(&m_InvoiceLock);
    sqlite3* sql_conn = DataBaseOpen(GetInvoiceCachePath());
    if (sql_conn == 0)
    {
        TW_TRACE_DIARY_TYPE("�򿪷�Ʊ�ϴ����ݿ�ʧ��" + GetInvoiceCachePath(), TW_TRACE_TPPE_INVOICEUNIT);
        return;
    }
    int db_ret = DataBaseDelete(sql_conn, param);
    if (DataBaseClose(sql_conn) != 0)
    {
        TW_TRACE_DIARY_TYPE("�رշ�Ʊ�ϴ����ݿ�ʧ��" + GetInvoiceCachePath(), TW_TRACE_TPPE_INVOICEUNIT);
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
        TW_TRACE_DIARY_TYPE("�򿪷�Ʊ�������ݿ�ʧ��", TW_TRACE_TPPE_INVOICEUNIT);
        return false;
    }
    int db_ret = DataBaseQuery(sql_conn, invoice_cancle);
    DataBaseClose(sql_conn);
    if (db_ret != 0)
    {
        return false;
    }

    char hint_log[200] = {0};
    _snprintf(hint_log, 150, "�ӷ�Ʊ�������ݿ��л�ȡһ�����ݣ���Ʊ���к�:%s", invoice_cancle.ElectronicInvoiceNo);
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
        TW_TRACE_DIARY_TYPE("�򿪷�Ʊ�ϴ����ݿ�ʧ��", TW_TRACE_TPPE_INVOICEUNIT);
        return false;
    }
    int db_ret = DataBaseQuery(sql_conn, invoice);
    DataBaseClose(sql_conn);
    if (db_ret != 0)
    {
        return false;
    }

    char hint_log[156] = {0};
    _snprintf(hint_log, 150, "�ӷ�Ʊ�ϴ����ݿ��л�ȡһ�����ݣ���Ʊ���к�:%s", invoice.ElectronicInvoiceNo);
    TW_TRACE_DIARY_TYPE(hint_log, TW_TRACE_TPPE_INVOICEUNIT);
    m_InvoiceGroup.push_back(invoice);
    return true;
}