/*
* Copyright(C) 2017,特微智能 保留所有权利。( All rights reserved. )
* 文件名称: ThreadHelper.h
* 摘   要:
* 当前版本:1.0.0.0
* 作   者: 朱亚冰
* 创建时间: 2017-04-08
* 修改记录:
*/

#ifndef THREAD_HELPER_H
#define THREAD_HELPER_H

#include <Windows.h>   //import CRITICAL_SECTION windows7 or early
#ifndef _WIN32_WINNT
#error 需要定义操作系统版本变量，以便确定API调用
#elif (_WIN32_WINNT >= 0x06020000)
#include <Synchapi.h>  //import CRITICAL_SECTION Windows8
#endif 

#include <string>


class CScopeLock
{
public:
    CScopeLock(CRITICAL_SECTION* lck)
        : mMutex(lck)
    {
		if(mMutex!=NULL)
		{
			::EnterCriticalSection(mMutex);
		}
        
    }
    ~CScopeLock()
    {
		if(mMutex!=NULL)
		{
			 ::LeaveCriticalSection(mMutex);
		}
       
    }
private:
    CRITICAL_SECTION* mMutex;
private:
    CScopeLock(const CScopeLock&);
    CScopeLock& operator = ( const CScopeLock&);

};


class CInterfaceTraceInfo
{
public:
    typedef void (*LogWriter)(const std::string& content);
    static void SetWriter(LogWriter lw){global_writer = lw;}
    CInterfaceTraceInfo(const std::string& name,const std::string & para="")
    {
        loc_name = name;
        std::string loginfo = "Invoke:["+loc_name+"]"+para;
        if(global_writer)
            global_writer(loginfo);
    }
    ~CInterfaceTraceInfo()
    {
		std::string loginfo = "Invoke over:["+loc_name+"]";
        if(global_writer)
            global_writer(loginfo);
    }
public:
    std::string loc_name;
 
    static LogWriter global_writer;
private:
    CInterfaceTraceInfo(const CInterfaceTraceInfo&);
    CInterfaceTraceInfo& operator = (const CInterfaceTraceInfo&);
};



#endif // THREAD_HELPER_H
