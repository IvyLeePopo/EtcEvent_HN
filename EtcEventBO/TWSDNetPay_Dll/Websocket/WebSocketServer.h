#pragma once

#include "WebsocketThreadBase.h"

class WebSocketBase;
class WebSocketServer: public WebSocketThreadBase
{
public:
	WebSocketServer(void);
	virtual ~WebSocketServer(void);

	virtual int circle();

	// ���������� 
	bool startServer(const char* pszIp, const int nPort);

	// �رշ����� 
	void stopServer();

	// �������ݵ��ͻ���
	bool sendData(const char* pszJson, int nSize);

	// ���տͻ�������
	bool receiveData(const char* pszData, int nSize);

	// ���ն������Ӵ��ڴ��͹���������
	void readFromReader();

private:
	// ��ʼ��websocket
	void init();

	// ����ʼ��
	void unInit();

private:
	WebSocketBase* m_pWebSocketBase;
	char*  m_pszRecvDataReader; // �������Ӵ��ڴ��͹���������
};
