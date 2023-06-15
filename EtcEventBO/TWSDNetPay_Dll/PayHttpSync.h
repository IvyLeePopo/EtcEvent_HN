#ifndef PAY_HTTP_SYNC_H
#define PAY_HTTP_SYNC_H
#include "StandTask.h"
#include "PayHelper.h"
#include "HttpConnection.h"
#include <list>

class PayHttpSync : public trawe::CStandTask
{
public:
    PayHttpSync();
    virtual ~PayHttpSync();
public:
    virtual void RunBody();
    int StartPay(DebitParameter& pay_para);
    int PayState(std::string& recv_str);
    bool IsIdle();
    int Cancle();
private:
    int SetPayState(int state);
	int ExecuteOnePay(DebitParameter& pay_para);
    bool IsCancled();
private:
    std::list<DebitParameter> m_DebitGroup;
    CRITICAL_SECTION m_AccessLock;
    int m_PayState; //����Ͷ���ӿ��ĵ�һ�£�������ֵ(-1),���ڱ�ʾ���ڴ�����
    std::string m_ServerResponse;
    bool m_IsCancled;
    HttpConnection m_HttpPipe;
private:
    PayHttpSync(const PayHttpSync&);
    PayHttpSync& operator = (const PayHttpSync&);

};

#endif // PAY_HTTP_SYNC_H
