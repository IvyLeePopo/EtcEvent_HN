#ifndef MEDIA_UNIT_H
#define MEDIA_UNIT_H
#include "StandTask.h"
#include "HttpConnection.h"
#include <list>


class CMediaUnit : public trawe::CStandTask
{
public:
    CMediaUnit();
    virtual ~CMediaUnit();

public:
    virtual void RunBody();
   
private:
    int ExecuteMediaStart(std::string& data);
    int ExecuteMediaStop(std::string& data);
private:
    CRITICAL_SECTION m_MediaLock;
	HttpConnection m_HttpPipe;
	std::list<std::string> m_MediaStartGroup;
    std::list<std::string> m_MediaStopGroup;
	std::string m_CallID;

public:
    int EventDealStart();
    int EventDealStop();

private:
    CMediaUnit(const CMediaUnit&);
    CMediaUnit& operator = (const CMediaUnit&);
};

#endif // MEDIA_UNIT_H
