#pragma once
#include "SingleTon.h"

#ifndef nullptr
#define nullptr NULL
#endif

class CHelper : public SingleTon<CHelper>
{
	friend class SingleTon<CHelper>;

public:
	CHelper(void);
	virtual ~CHelper(void);
	
	// ��ȡʱ���,��ȷ���룬int����
	long getCurrentTimeStamp(bool bMs = false); 


public:
};

#define CheckResult(a)  if(!(a)) \
  return;
#define CheckResultWithRet(a, b)  if(!(a)) \
  return (b);

#define Helper CHelper::getInstance()
