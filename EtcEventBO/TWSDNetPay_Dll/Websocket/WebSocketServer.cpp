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
			// �����������ص����ݣ�ԭ�ⲻ�����͸��ͻ��ˣ���������̬�⣩
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
		// δ�ܿ���������
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
	//sprintf_s(szInfo, "��������(%s):%s", bRet ? "�ɹ�" : "ʧ��", strData.c_str());
	//WRITELOG(bRet ? ENUM_LOG_LEVEL_INFO : ENUM_LOG_LEVEL_ERROR, szInfo);

	return bRet;
}

bool WebSocketServer::receiveData(const char* pszData, int nSize)
{
	/*
	* 1�����յ��ͻ��˵����ݣ�ͨ������ֱ�ӷ���������
	* 2���������յ�ָ���ִ��ָ���Ľ������
	* 3�������صĽ����ͨ��sendData�����ͻ���
	*/
	 return true;
}

// ���ն������Ӵ��ڴ��͹���������
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