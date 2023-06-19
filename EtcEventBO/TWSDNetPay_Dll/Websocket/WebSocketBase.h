/*
	������Ҫ�Ƿ�װ��websocket�Ļ��࣬����libwebsockets��Դ�� 
*/
#pragma once

#include <list>
#include <map>
#include <string.h>
#include "WebsocketThreadBase.h"
#include "libwebsockets/libwebsockets.h"

using namespace std;

#define LOGTYPE "_log_type:"

// �ص���������websocket�Ľ���׸��ϲ�
typedef void(*recvWebSocketMsg)(void* pUser, const char* pszData, int nSize);

// ÿһ��websocket�������̻߳��࣬��Ҫ���ڹ����Լ�����Ϣ
class WebSocketBase : public WebSocketThreadBase
{
public:
	WebSocketBase(void);
	virtual ~WebSocketBase(void);
	virtual int circle();

public:
	// ��ʼ��
	void init();

	// ����ʼ��
	void unInit();

	// �Ƿ��Ѿ�����
	bool isConnected() { return m_bIsConnected; }
	bool isErrorDisonnect() { return m_bErrorDisconnect; }

	// �Ƿ������ݳɹ�
	int isSended() { return m_nSendResult; }

	// �������ӱ�־
	void resetConnected(bool bConnected) { m_bIsConnected = bConnected; }
	void reseErrortDisconnect(bool bDisconnect) { m_bErrorDisconnect = bDisconnect; }

	// ����������
	bool startSever(const char* pszIp, const int nPort);
	void resetStarted(bool bStartServer) { m_bStartServer = bStartServer; }
	bool isStarted();

	// �رշ�����
	bool stopServer();

	// ������Ϣ
	bool send(const char* pszJson, int nSize);

	// ��ȡ���lws��websocketbase
	static void* getLwsToWs(lws* pLws);


public:
	// ���ûص�����
	void setCallBack(recvWebSocketMsg pFunc, void* pUser);

	// �������˷�����
	void connectResult(bool bConnect);
	void getConnectStatus(int nConncet);		//-1-δ���ӣ�0-�ͻ��˶Ͽ���1-����˶Ͽ�

	// ���ѷ�����Ϣ����
	void consumeSendMsg();

	// ��־���ͣ�����recvWebSocketMsg�ص�
	void writeLog(const char* pszData);
	
	// ���յ��ͻ��˵���Ϣ
	bool recvMsgByCallBack(const char* pszData, int nSize);

private:
	int m_nPort;												// websocket �˿�
	string m_strIp;												// websocket ip
	char m_szHost[256];											// ���������
	int m_nSendResult;											// �Ƿ������ݳɹ�
	bool m_bIsConnected;										// �Ƿ��Ѿ��������˿ͻ���
	bool m_bStartServer;										// �Ƿ��Ѿ�����������
	bool m_bErrorDisconnect;									// �Ƿ��쳣�Ͽ�����
	list<string> m_listMsg;										// �ȴ����͵���Ϣ����
	int m_nReconnectTimes;										// �����Ĵ���
	string m_strLog;											// ˽�����͵���־

	struct lws* m_pWsi;											// websocket���Ӿ��
	struct lws_context* m_pContext;								// websocket������
	struct lws_context_creation_info* m_pLwsCtxInfo;			// context����
	struct lws_protocols m_pLwsProtocols[2];					// Э��㴫����
	
	void* m_pUser;												// ���ػص��Ķ���
	struct lws_client_connect_info m_tagConnInfo;				// ���Ӳ���
	recvWebSocketMsg m_pRecvWebSocketFunc;						// �ص�

	static map<lws*, void*> m_mapLwsToWs;						// ���lws��websocketbase
};
