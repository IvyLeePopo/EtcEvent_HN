#ifndef CANCELPAY_UNIT_H
#define CANCELPAY_UNIT_H
#include "StandTask.h"
#include "PayHelper.h"
#include <list>
#include "PayHttpSync.h"
#include "AgencyPay.h"

class CCancelPayUnit : public trawe::CStandTask
{
public:
    CCancelPayUnit();
    virtual ~CCancelPayUnit();
public:
    virtual void RunBody();

private:
	int ExecuteOneCancle();
    int GetDebitCancleFromDataBase();
    int DeleteDebitCancleFromDataBase(const DebitCancleParameter& dp);
private:
    int NotifyResult(int option,int result);

private:
    std::list<DebitCancleParameter> m_DebitCancleGroup;
    /*Ö§¸¶£¬³·µ¥Ëø*/
    CRITICAL_SECTION m_PayLock;
	HttpConnection m_HttpPipe;
private:
    CCancelPayUnit(const CCancelPayUnit&);
    CCancelPayUnit& operator = (const CCancelPayUnit&);

};


#endif // CANCELPAY_UNIT_H
