#ifndef MEDIA_MANAGE_H
#define MEDIA_MANAGE_H
#include "MediaUnit.h"
#include <map>
#include <string>


class CMediaManage
{
public:
    CMediaManage();
    ~CMediaManage();
    int Init();
	int Destroy();
    bool IsEanble();
public:
    int EventDealStart();
    int EventDealStop();
	
private:
    CMediaUnit* m_MediaTask;
    CRITICAL_SECTION m_AccessMutex;
    bool m_IsEnable;
private:
    CMediaManage(const CMediaManage&);
    CMediaManage& operator = (const CMediaManage&);
};

#endif // MEDIA_MANAGE_H
