#ifndef ETC_AUTHENTICATE_UNIT_H
#define ETC_AUTHENTICATE_UNIT_H
#include "StandTask.h"
#include "ETCPayHelper.h"
#include "HttpConnection.h"
#include <list>

namespace trawe
{


class ETCAuthenticateUnit : public CStandTask
{
public:
    ETCAuthenticateUnit();
    virtual ~ETCAuthenticateUnit();
public:
    /*��������*/
    virtual void RunBody();
public:
    /*�����������Ȩ*/
    int StartWhiteListQuery(const ObuWhiteListQueryParameter& query_info);
    /*�Ƿ��ǰ�����*/
    int IsAgencyWhiteList(const ObuWhiteListQueryParameter& query_info );
    /*�����µİ�������¼*/
    int NewObuWhiteListRecord(const ObuWhiteListRecord& obu_record);
    /*�Ƿ���æ*/
    bool IsHTTPIdle();
private:
    /*ʵ��ִ�а�������Ȩ��ѯ*/
    void ExecuteQuery();
private:
    /*�ٽ���Դ��*/
    CRITICAL_SECTION m_AccessLock;
    /*����ͨ��*/
    HttpConnection m_HttpPipe;
    /*��������Ȩ�������*/
    std::list<ObuWhiteListQueryParameter> m_ObuQueryGroup;
    /*��������Ȩ���*/
    std::list<ObuWhiteListRecord> m_ObuWhiteList;
private:
    ETCAuthenticateUnit(const ETCAuthenticateUnit&);
    ETCAuthenticateUnit& operator = (const ETCAuthenticateUnit&);
};

}

#endif // ETC_AUTHENTICATE_UNIT_H

/**
 * ETC��������Ȩ�߳�
 * StartWhiteListQuery:
 *     ����һ����������ѯ,0������ɹ���-1:����ʧ�ܣ�����ֵ:��������
 *     �����������̰߳�ȫ
 * IsAgencyWhiteList��
 *     �Ƿ��ǰ��������û���0:�ǰ������û���-1:δ֪��1:�ǰ������û�
 *     �����������̰߳�ȫ
 * NewObuWhiteListRecord:
 *     �����µ�OBU��������¼
 *     ���̰߳�ȫ��
 *     �˴���������ڵ����ݣ�Ĭ����5s����������
 *  IsHTTPIdle��
 *      �жϲ�ѯ�߳��Ƿ���У�������������ԭ���ϴβ�ѯ��Ȼû�н�����򱾴β�ѯֱ����Ϊ����ʧ�ܡ�
 */