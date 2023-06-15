#ifndef ETC_PAY_MANAGE_H
#define ETC_PAY_MANAGE_H
#include "ETCPayUnit.h"
#include "ETCAuthenticateUnit.h"

namespace trawe
{
class ETCPayManage
{
public:
    ETCPayManage();
    ~ETCPayManage();
    /*��ʼ����һ��Ҫ�ڳ�ʼ���ӿ��е��ô˽ӿ�*/
    int Init();
	int Destroy();
    /*�ṩ�����������Ȩ����*/
    int StartWhiteListQuery(const ObuWhiteListQueryParameter& query_info);
    /*�ṩ��������Ȩ�����ѯ�������*/
    int IsInWhiteList(const ObuWhiteListQueryParameter& query_info );
    /*�ṩ������״̬��ѯ����*/
    int StartDebitStateQuery(const DebitStateQueryParameter& query_info);
    /*�ṩ����״̬��ѯ�������*/
    int GetDebitState(const DebitStateQueryParameter& query_info, ETCDebitRecord& state);
    /*�ṩ�۷ѹ���*/
    int DebitMoney(const ETCDebitRecord& debit_para,std::string& result_str,const int& result_type);
    /*��ѯ�Ƿ���ϵͳ����*/
    bool HasUnrecoverableError();
private:
    /*ETC֧����������*/
    ETCPayUnit * m_ETCPayTask;
    /*ETC��Ȩ��������*/
    ETCAuthenticateUnit * m_ETCAuthTask;
    /*������Դ��*/
    CRITICAL_SECTION m_AccessMutex;
private:
    ETCPayManage(const ETCPayManage&);
    ETCPayManage& operator = (const ETCPayManage&);
};



}



#endif // ETC_PAY_MANAGE_H
/**
 * ��ɸ���ETC�����й��ܽӿ�
 * 1������������
 * 2�����׷���
 * 3�����׹鵵(����ɾ��)������״̬��ѯ
 *
 * ����˵����
 * StartWhiteListQuery:
 *     ����һ����������ѯ,0������ɹ���-1:����ʧ�ܣ�����ֵ��Ŀǰֻ��1��:�������
 * IsInWhiteList:
 *     ���а�������ѯ��0:�ǰ������û���-1:δ֪������ֵ:�ǰ������û�����Ӧ�������ֵ
 * StartDebitStateQuery:
 *         ����һ�����ײ�ѯ��0:����ɹ���-1:����ʧ�ܣ�����ֵ(Ŀǰֻ��1):����
 *         ���⣺�˽��ײ�ѯ��Ŀǰ�ǲ�ѯ���ؼ�¼���Ժ������Ҫ��ѯ�ƶˡ�
 * GetDebitState:
 *       0:��ѯ״̬�ɹ���-1:״̬δ֪,1:��ѯ״̬ʧ��
 *      ��ȡĳ�����׵Ľ���״̬��ͨ�����ױ�ʶ��ѯ
 * DebitMoney:
 *      0:����ۿ�ɹ�������ֵ��ʧ��
 *      ���п۷ѣ��˴���Ҫ�������أ������½��״̬��
 *      ��Ҫ��ȷ���ۿ�ı��浽Ӳ�̣�����������⡣������浽����ʧ�ܣ��򷵻�ʧ��
 */
