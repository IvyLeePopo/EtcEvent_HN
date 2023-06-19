#include "StdAfx.h"
#include <WinSock2.h>
#include "WebSocketBase.h"

#pragma comment(lib, "websockets.lib")
#define PROTOCOL_NAME "ws"
#define MAX_PAYLOAD_SIZE 10 * 1024
#define nullptr NULL

static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void* pRecvData, size_t nLen)
{
	return 0;
}

static int callback_websocket(struct lws *pWsi, enum lws_callback_reasons reason, void *pUser, void* pRecvData, size_t nLen)
{
	//if (nullptr == pUser)
	//	return 0;
	
	WebSocketBase* pWebSocket = (WebSocketBase*)WebSocketBase::getLwsToWs(pWsi);
	if (nullptr == pWebSocket)
		return 0;

	switch (reason)
	{
	case LWS_CALLBACK_ESTABLISHED:					// ���ӵ��ͻ��ˣ�����ֻ��һ���ͻ��ˣ�
		pWebSocket->connectResult(true);
		break;
	case LWS_CALLBACK_RECEIVE:						// ���տͻ�������
		pWebSocket->recvMsgByCallBack((const char*)pRecvData, (int)nLen);
		break;
	case LWS_CALLBACK_SERVER_WRITEABLE:				// ����˿��Է���������
		pWebSocket->consumeSendMsg();
		break;
	case LWS_CALLBACK_CLOSED:						// �Ͽ�����
		if(pWebSocket->isCanRun())
			pWebSocket->connectResult(false);
		break;
	default:
		break;
	}
	return 0;
}

static struct lws_protocols protocols[] = {
	//{ "http", callback_http, 0, 0, },
	{ PROTOCOL_NAME, callback_websocket, 0, 0,},
	{ NULL, NULL, 0, 0 } /* end of list */
};

//struct lws_protocols protocols[] = 
//{
//	{ PROTOCOL_NAME, wsCallBack, sizeof(WebSocketBase), MAX_PAYLOAD_SIZE},
//	{NULL, NULL, 0}	
//};

map<lws*, void*> WebSocketBase::m_mapLwsToWs;

WebSocketBase::WebSocketBase(void)
: m_pLwsCtxInfo(nullptr)
, m_pContext(nullptr)
, m_pWsi(nullptr)
, m_bIsConnected(false)
, m_pUser(nullptr)
, m_pRecvWebSocketFunc(nullptr)
, m_bErrorDisconnect(false)
, m_nReconnectTimes(0)
{
	init();
}

WebSocketBase::~WebSocketBase(void)
{
	unInit();
}

int WebSocketBase::circle()
{
	while(isCanRun())
	{
		lws_callback_on_writable_all_protocol(m_pContext, &protocols[0]); 
		lws_service(m_pContext, 1000);//����������

		Sleep(50);

		if (isErrorDisonnect())
		{
			if(isConnected())
			{
				reseErrortDisconnect(false);
				continue;
			}
			//autoConnect();
			lws_service(m_pContext, 1000);
			Sleep(5000);
		}
		if (!m_strLog.empty())
		{
			// �����﷢�ͣ���Ҫ�Ǵ�websocket recv�������̣߳����ֱ�ӵ���д��־�ӿڣ����������
			writeLog(m_strLog.c_str());
			m_strLog = "";
		}
	}

	lws_context_destroy(m_pContext);
	m_pContext = nullptr;
	resetConnected(false);

	writeLog("websocket�����߳�");
	return TRUE;
}

void WebSocketBase::init()
{
	if (nullptr == m_pLwsCtxInfo)
	{
		m_pLwsCtxInfo = new struct lws_context_creation_info();
		memset(m_pLwsCtxInfo, 0, sizeof(struct lws_context_creation_info));
	}

	m_pLwsCtxInfo->port = CONTEXT_PORT_NO_LISTEN;
	m_pLwsCtxInfo->iface = NULL;
	m_pLwsCtxInfo->protocols = protocols;
	m_pLwsCtxInfo->gid = -1;
	m_pLwsCtxInfo->uid = -1;
}

void WebSocketBase::unInit()
{
	if (isCanRun())
	{
		vStop();
		Sleep(20);
	}

	if (nullptr != m_pLwsCtxInfo)
	{
		delete m_pLwsCtxInfo;
		m_pLwsCtxInfo = nullptr;
	}
}

bool WebSocketBase::startSever(/*const char* pszIp, const int nPort*/)
{
	char szBuf[1028] = {0};
	//if (nullptr == pszIp || 0 == nPort)
	//{
	//	//strErr = "����websocket��ip�Ͷ˿�����";
	//	return false;
	//}

	if (isStarted())
	{
		//sprintf_s(szBuf, "�������Ѿ������������ٴο���!", pszIp, nPort);
		//strErr = szBuf;
		return true;
	}

	if (nullptr == m_pLwsCtxInfo)
	{
		//strErr = "m_pLwsCtxInfo is null,û�г�ʼ������ע��";
		return false;
	}

	if(nullptr != m_pContext)
	{
		lws_context_destroy(m_pContext);
		m_pContext = nullptr;
	}

	int port = 8081;
	int opts = 0;//������Ķ��⹦��ѡ��

	//����info�����info��Ϣ��
	m_pLwsCtxInfo->port = port;
	m_pLwsCtxInfo->protocols = protocols;
	m_pLwsCtxInfo->gid = -1;
	m_pLwsCtxInfo->uid = -1;
	m_pLwsCtxInfo->ssl_private_key_filepath = NULL;
	m_pLwsCtxInfo->ssl_ca_filepath = NULL;
	m_pLwsCtxInfo->options = opts;
	m_pLwsCtxInfo->ka_time = 0;
	m_pLwsCtxInfo->ka_probes = 0;
	m_pLwsCtxInfo->ka_interval = 0;

	m_pContext = lws_create_context(m_pLwsCtxInfo);//���������Ķ��󣬹���ws
	if (!m_pContext) 
	{
		printf("Error creating WebSocket context\n");
		return 1;
	}

	printf("WebSocket server started on port %d\n", port);

	int nTimes = 0;
	while (true) //�������ǿ��Կ����������ģ������󣬽���ѭ��
	{
		lws_callback_on_writable_all_protocol(m_pContext, &protocols[0]); 
		lws_service(m_pContext, 100);//����������

		if (nTimes++ > 10 || isStarted())
			break;

		Sleep(20);
	}

	//lws_context_destroy(context);//���������Ķ���

	vStart();
	return true;
}

bool WebSocketBase::isStarted()
{
	return m_bStartServer;
}

bool WebSocketBase::stopServer()
{
	vStop();
	return true;
}

bool WebSocketBase::send(const char* pszJson, int nSize)
{
	if(isCanRun() && !isConnected())
		return false;

	if (nullptr == pszJson)
		return false;

	_lock();
	// �˴��ȼ��һ�»��棬�����쳣����£����ݷ��Ͳ���ȥ���µĻ���ѻ���
	if (m_listMsg.size() > 30)
		m_listMsg.clear();

	m_listMsg.push_back(pszJson);
	_unLock();
	return true;
}

void* WebSocketBase::getLwsToWs(lws* pLws)
{
	if (nullptr == pLws)
		return nullptr;

	map<lws*, void*>::iterator iter = m_mapLwsToWs.find(pLws);
	if (iter == m_mapLwsToWs.end())
		return nullptr;

	return (void*)iter->second;
}

void WebSocketBase::setCallBack(recvWebSocketMsg pFunc, void* pUser)
{
	if (nullptr != pFunc)
		m_pRecvWebSocketFunc = pFunc;

	if (nullptr != pUser)
		m_pUser = pUser;
}

void WebSocketBase::connectResult(bool bConnect)
{
	if (0 != m_nReconnectTimes)
	{
		char szBuf[1024] = {0};
		sprintf_s(szBuf, "websocket����-%s:%d", m_strIp.c_str(), m_nPort);
		m_strLog = szBuf;
		m_nReconnectTimes = 0;
	}
	
	resetConnected(bConnect);
	reseErrortDisconnect(!bConnect);
}

void WebSocketBase::consumeSendMsg()
{
	if (m_listMsg.empty())
		return;

	string strData;
	_lock();
	list<string>::reverse_iterator rIter = m_listMsg.rbegin();
	strData = *rIter;
	m_listMsg.pop_back();		// �Ӷ���ɾ��
	_unLock();

	if (strData.empty())
		return;

	int nSize = (int)strData.size();
	char *pszBuf = new char[nSize + LWS_PRE];
	memset(&pszBuf[LWS_PRE], 0, nSize);
	memcpy(&pszBuf[LWS_PRE], strData.c_str(), nSize);
	m_nSendResult = lws_write(m_pWsi, (unsigned char*)&pszBuf[LWS_PRE], nSize, LWS_WRITE_TEXT);

	delete [] pszBuf;
	pszBuf = nullptr;
}

void WebSocketBase::writeLog(const char* pszData)
{
	if (nullptr == pszData || nullptr == m_pRecvWebSocketFunc)
		return;

	string strErr = LOGTYPE + string(pszData);
	m_pRecvWebSocketFunc(m_pUser, strErr.c_str(), (int)strErr.size());
}

bool WebSocketBase::recvMsgByCallBack(const char* pszData, int nSize)
{
	if (nullptr == pszData || nullptr == m_pRecvWebSocketFunc)
		return false;

	m_pRecvWebSocketFunc(m_pUser, pszData, nSize);
	return true;
}
