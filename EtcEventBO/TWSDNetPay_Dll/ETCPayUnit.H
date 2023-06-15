#ifndef ETC_PAY_UNIT_H
#define ETC_PAY_UNIT_H
#include "StandTask.h"
#include "ETCPayHelper.h"
#include "HttpConnection.h"

namespace trawe
{
class ETCPayUnit : public CStandTask
{
public:
    ETCPayUnit();
    virtual ~ETCPayUnit();
public:
    /*�߳���������*/
    virtual void RunBody();
public:
    /*�Ƿ�������*/
    bool IsHTTPIdle();
    /*����һ������״̬��ѯ*/
    int StartDebitStateQuery(const DebitStateQueryParameter& query_info);
    /*��ȡ����״̬*/
    int GetDebitState(const DebitStateQueryParameter& query_info, ETCDebitRecord& state);
    /*���пۿ�*/
    int DebitMoney(const ETCDebitRecord& debit_para, std::string& result_str, const int& result_type);
    /*��ѯ�Ƿ���ϵͳ����*/
	bool HasUnrecoverableError();
private:
    /*����ʵ�ʵĿۿ��*/
    int SendOneDebit();
    /*����ʵ�ʵĽ���״̬��ѯ*/
    int SendDebitStateQuery();
    /*���潻�׼�¼������*/
    int SaveDebitRecord(const ETCDebitRecord& record);

    /*��IO�еĽ������󣬷ŵ����ݿ�*/
    int MoveDebitRequestFromIOToDataBase();

    /*ͨ��IO���潻�����󻺴�*/
    int SaveDebitRequestToIO(const ETCDebitRecord& record);

    /*��ȡ���׼�¼������ļ���*/
    std::string GetRecordFileName(time_t t);
    /*������ڵĽ��׼�¼*/
    int ClearAbateRecord();
    /*��ָ���Ľ������ݿ��У���ѯָ��������Ϣ*/
    int QueryRecordInTargetDataBase(const std::string& database_file,ETCDebitRecord&record);

    int NewDebitAbstract(const ETCDebitAbstract& debit_abstract);
    int QueryDebitAbstract(const std::string& plate,ETCDebitAbstract& debit_abstract);
private:
    /*����ϵͳ����*/
    int OccurSystemFatalError();
    /*��ȡ��һ���������*/
    std::string GetNextPaySerialNo();
private:
    /*��֧��������صĲ�����������*/
    CRITICAL_SECTION m_PayInfoLock;
    /*�����ݿ���صĲ�����������*/
    CRITICAL_SECTION m_DataBaseLock;
    /*���׷��Ͷ���*/
    std::list<ETCDebitRecord> m_DebitQueue;
    /*HTTP������Ԫ*/
    HttpConnection m_HttpPipe;
    /*�Ƿ������ɻָ�����*/
    bool m_IsSystemFatalError;
    /*�����У��Ƿ��д����͵Ľ���*/
	bool m_HasDebitCache;
    /*�������ժҪ*/
    std::list<ETCDebitAbstract> m_RecentDebitAbstract;
private:
    ETCPayUnit(const ETCPayUnit&);
    ETCPayUnit& operator = (const ETCPayUnit&);
};

}

#endif // ETC_PAY_UNIT_H

/**
 * ETC֧����Ԫ���ṩ���������͹��ܡ�����״̬��ѯ����
 * 1�����ͽ�������ʡ����
 * 2�����ɹ��Ľ��׼�¼���й鵵����Ҫ���Թ鵵���������⣨�ڹ鵵���ݼ����������£������Ƿ������
 * 3����ѯĳ�������Ƿ�ɹ�
 * 4�����¹鵵���ף���ʱɾ��
 *
 * ��������:
 * IsHTTPIdle:
 *     1���������߳��Ƿ���У����磬�ϴ�����û�з�����ϣ�
 *     2����������У�����Ϊ���紦����������������������
 * StartDebitStateQuery :
 *     
 *     ���ͽ������󣬷����������̰߳�ȫ
 * GetDebitState:
 *     ��ѯ����״̬������֧����ʶ.�����������̰߳�ȫ
 * DebitMoney:
 *     �����ף������������̰߳�ȫ
 * SendOneDebit��
 *     ʵ�ʷ���һ�ʽ��ס�
 * SendDebitStateQuery��
 *     ���ͽ���״̬��ѯ����
 * SaveDebitRecord��
 *     �鵵���׼�¼
 * ClearAbateRecord:
 *     �������ڵĽ��׹鵵
 */