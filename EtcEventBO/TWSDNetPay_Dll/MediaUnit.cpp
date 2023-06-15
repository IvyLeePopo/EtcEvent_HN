#include "stdafx.h"
#include "MediaUnit.h"
#include "RunParameter.h"
#include "SystemDiary.h"
#include "BaseDefine.h"
#include "TWSDNetPay_Dll.h"
#include "JsonHelper.h"
#include "TypeHelper.h"
#include "DataBaseHelper.h"
#include "SimpleUtil.h"
using namespace trawe;

CMediaUnit::CMediaUnit()
{
    ::InitializeCriticalSection(&m_MediaLock);
    m_HttpPipe.ChangeRemotePeer(GetMediaIP(), GetMediaPort());
}

CMediaUnit::~CMediaUnit()
{
    Stop();
    ::DeleteCriticalSection(&m_MediaLock);
}


/*
	std::list<std::string> m_MediaStartGroup;
    std::list<std::string> m_MediaStopGroup;
*/
void CMediaUnit::RunBody()
{	
	std::string StartData;
    {
        trawe::CScopeLock media_lock(&m_MediaLock);
        if (m_MediaStartGroup.empty() == false)
        {
			StartData = m_MediaStartGroup.front();
			m_MediaStartGroup.pop_front();
			ExecuteMediaStart(StartData);			
        }        
    }
	std::string StopData;
    {
        trawe::CScopeLock media_lock(&m_MediaLock);
        if (m_MediaStopGroup.empty() == false)
        {
			StopData = m_MediaStopGroup.front();
			m_MediaStopGroup.pop_front();
			ExecuteMediaStop(StopData);			
        }        
    }
   
    Sleep(100);
}


int CMediaUnit::EventDealStart()
{
	TW_TRACE_DIARY_TYPE("ý���������鿪ʼ", TW_TRACE_TPPE_ETCEVENTUNIT);
	Json::Value root;

    root["SpecialEventNo"]  = Json::Value(theApp.m_EtcEventNo);

	m_CallID = GetGuid();
    root["CallId"]     = Json::Value(m_CallID);

	//ʡ�ݱ���-·�α���-�շ�վ����-������
	std::string ProvinceNo = Int2Str(theApp.m_EtcProvinceId);
	std::string LaneInfo = ProvinceNo + "-" + theApp.m_EtcRoadId + "-" + theApp.m_EtcStationId + "-" + theApp.m_EtcLaneId;

	root["LaneInfo"] = Json::Value(LaneInfo);
	time_t t = time(0);
	std::string DealTime = ChinaTime2Str(t);
	root["DealTime"] = Json::Value(DealTime);
	//ת��Ϊ�ַ���
    Json::FastWriter fast_writer;
	std::string json_str = fast_writer.write(root);

	{
        trawe::CScopeLock media_lock(&m_MediaLock);
        m_MediaStartGroup.push_back(json_str);
    }

	return 0;
}

int CMediaUnit::EventDealStop()
{
	TW_TRACE_DIARY_TYPE("ý�������������", TW_TRACE_TPPE_ETCEVENTUNIT);
	Json::Value root;

    root["SpecialEventNo"]  = Json::Value(theApp.m_EtcEventNo);
    root["CallId"]     = Json::Value(m_CallID);

	//ʡ�ݱ���-·�α���-�շ�վ����-������
	std::string ProvinceNo = Int2Str(theApp.m_EtcProvinceId);
	std::string LaneInfo = ProvinceNo + "-" + theApp.m_EtcRoadId + "-" + theApp.m_EtcStationId + "-" + theApp.m_EtcLaneId;
	root["LaneInfo"] = Json::Value(LaneInfo);

	time_t t = time(0);
	std::string DealTime = ChinaTime2Str(t);
	root["DealTime"] = Json::Value(DealTime);

	//ת��Ϊ�ַ���
    Json::FastWriter fast_writer;
	std::string json_str = fast_writer.write(root);

	{
        trawe::CScopeLock media_lock(&m_MediaLock);
        m_MediaStopGroup.push_back(json_str);
    }
	return 0;
}

int CMediaUnit::ExecuteMediaStart(std::string& data)
{
	std::string json_str = data;
	TW_TRACE_DIARY_TYPE("ý���������鿪ʼJson:" + json_str, TW_TRACE_TPPE_ETCEVENTUNIT);

	std::string utf8_json_str = StringConvertByCodePage(json_str, CP_ACP, CP_UTF8);
	std::string recv_str;
	int SendTimes = 3;
	bool bRecvResp = false;
	while(SendTimes)
	{
		SendTimes--;
		if (!m_HttpPipe.Post("/ETCEventMediaControl/start", utf8_json_str, recv_str))
		{
			TW_TRACE_DIARY_TYPE("����ý���������鿪ʼJsonʧ��:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TPPE_ETCEVENTUNIT);
			Sleep(20);
			continue;
		}
		bRecvResp = true;
		break;
	}
	
	if(bRecvResp == true)
	{
		std::string gbk_recv_str = StringConvertByCodePage(recv_str, CP_UTF8, CP_ACP);
		TW_TRACE_DIARY_TYPE("�յ�ý���������鿪ʼ����:" + gbk_recv_str , TW_TRACE_TPPE_ETCEVENTUNIT);
	}
	return 0;
}

int CMediaUnit::ExecuteMediaStop(std::string& data)
{
	std::string json_str = data;
	TW_TRACE_DIARY_TYPE("ý�������������Json:" + json_str, TW_TRACE_TPPE_ETCEVENTUNIT);

	std::string utf8_json_str = StringConvertByCodePage(json_str, CP_ACP, CP_UTF8);
	std::string recv_str;
	int SendTimes = 3;
	bool bRecvResp = false;
	while(SendTimes)
	{
		SendTimes--;
		if (!m_HttpPipe.Post("/ETCEventMediaControl/stop", utf8_json_str, recv_str))
		{
			TW_TRACE_DIARY_TYPE("����ý�������������Jsonʧ��:" + m_HttpPipe.LastHttpErrorDesc(), TW_TRACE_TPPE_ETCEVENTUNIT);
			Sleep(20);
			continue;
		}
		bRecvResp = true;
		break;
	}
	
	if(bRecvResp == true)
	{
		std::string gbk_recv_str = StringConvertByCodePage(recv_str, CP_UTF8, CP_ACP);
		TW_TRACE_DIARY_TYPE("�յ�ý�����������������:" + gbk_recv_str , TW_TRACE_TPPE_ETCEVENTUNIT);
	}

	return 0;
}
