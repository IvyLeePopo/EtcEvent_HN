#include "StdAfx.h"
#include "WebSocketServer.h"

#include "Helper.h"
#include "WebSocketBase.h"

void recvMsgFunc(void* pArgs, const char* pszData, int nSize)
{
	if (nullptr == pArgs || nullptr == pszData)
		return;

	WebSocketServer* pWebSocket = (WebSocketServer*)pArgs;
	pWebSocket->receiveData(pszData, nSize);
}

WebSocketServer::WebSocketServer(void)
{
}

WebSocketServer::~WebSocketServer(void)
{
}

int WebSocketServer::circle()
{
	while(true)
	{		
		if(m_pWebSocketBase->isStarted())
		{
			break;
		}
		Sleep(1000);
	}

	int nTime = Helper->getCurrentTimeStamp();
	while(isCanRun())
	{
		int nCurTime = Helper->getCurrentTimeStamp();
		if (nCurTime - nTime > 10)
		{
			// 将读卡器返回的数据，原封不动发送给客户端（读卡器动态库）
			m_pWebSocketBase->send(m_pszRecvDataReader,(int)strlen(m_pszRecvDataReader));
			nTime = nCurTime;
		}

		readFromReader();
		Sleep(20);
	}

	return TRUE;
}

bool WebSocketServer::startServer(const char* pszIp, const int nPort)
{
	if(m_pWebSocketBase->isStarted())
		return true;

	if(!m_pWebSocketBase->startSever(pszIp, nPort))
	{
		// 未能开启服务器
		return false;
	}
	
	vStart();
	return true;
}

void WebSocketServer::stopServer()
{
	CheckResult(m_pWebSocketBase);
	m_pWebSocketBase->stopServer();
	vStop();
}

bool WebSocketServer::sendData(const char* pszJson, int nSize)
{
	bool bRet = m_pWebSocketBase->send(pszJson, nSize);

	//char szInfo[1024] = {0};
	//sprintf_s(szInfo, "发送数据(%s):%s", bRet ? "成功" : "失败", strData.c_str());
	//WRITELOG(bRet ? ENUM_LOG_LEVEL_INFO : ENUM_LOG_LEVEL_ERROR, szInfo);

	return bRet;
}

bool WebSocketServer::receiveData(const char* pszData, int nSize)
{
	/*
	* 1、接收到客户端的数据，通过串口直接发给读卡器
	* 2、读卡器收到指令，将执行指令后的结果返回
	* 3、将返回的结果，通过sendData发给客户端
	*/
	 return true;
}

// 接收读卡器从串口传送过来的数据
void WebSocketServer::readFromReader()
{
	// todo
	m_pszRecvDataReader = "";
}

void WebSocketServer::init()
{
	m_pWebSocketBase = new WebSocketBase();
	m_pWebSocketBase->setCallBack(recvMsgFunc, (void*)this);
}

void WebSocketServer::unInit()
{	
	if (nullptr != m_pWebSocketBase)
	{
		delete m_pWebSocketBase;
		m_pWebSocketBase = nullptr;
	}
}