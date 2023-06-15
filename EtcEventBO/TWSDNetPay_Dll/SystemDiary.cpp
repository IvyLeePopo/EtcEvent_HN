
#include "stdafx.h"
#include "SystemDiary.h"
#include <fstream>
#include <sstream>
#include "TWSDNetPay_Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



std::string GetTraceMsgHeader(int msgType)
{
    std::string result;
    if (msgType == TW_TRACE_TPPE_INTERFACE)
    {
        result = "[��������Ӧ]";
    }
    else if (msgType == TW_TRACE_TPPE_PAYUNIT)
    {
        result = "[֧������Ԫ]";
    }
    else if(msgType == TW_TRACE_TPPE_PAYHTTP)
    {
        result = "[֧�������߳�]";
    }
	else if (msgType == TW_TRACE_TPPE_INVOICEUNIT)
    {
        result = "[��Ʊ����Ԫ]";
    }
    else if(msgType == TW_TRACE_TPPE_PAYHTTP)
    {
        result = "[֧�������߳�]";
    }
    else if (msgType == TW_TRACE_TYPE_INTERFACE)
    {
        result = "[��������Ӧ]";
    }
	else if (msgType == TW_TRACE_TPPE_ETCEVENTUNIT)
    {
        result = "[ETC���鴦��Ԫ]";
    }
    else if (msgType == TW_TRACE_TYPE_ETCMANAGE)
    {
        result = "[ETC������Ԫ]";
    }
    else if (msgType == TW_TRACE_TYPE_ETCPAYUNIT)
    {
        result = "[ETC֧����Ԫ]";
    }
    else if (msgType == TW_TRACE_TYPE_ETCAUTH)
    {
        result = "[ETC����������]";
    }
    else if(msgType==TW_TRACE_TYPE_DEBUGVALUE)
    {
        result = "[��������]";
    }
    else if(msgType == TW_TRACE_TYPE_INVOKING)
    {
        result = "[���̵���]";
    }
    return result;
}


const std::string GetCurTimeStr(void)
{
    SYSTEMTIME sysTime={0};
    ::GetLocalTime(&sysTime);
    char strTime[100]={0};
    sprintf(strTime,"%04d-%02d-%02d %02d:%02d:%02d.%03d",sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMilliseconds);
    return strTime;
}
const std::string GetLogPrefix()
{
	char strTime[100]={0};
	sprintf(strTime,"[%s#%08X]",GetCurTimeStr().c_str(),GetCurrentThreadId());
    return strTime;
}
void sys_trace_info(const std::string& some_info, int msgType)
{
	// printf("[%s#%08X]%s\n",GetCurTimeStr().c_str(),GetCurrentThreadId(),some_info.c_str());
	// return ;
	char chs[4000]={0};
	//_snprintf(chs,3000,"[%s#%08X]%s%s\n",GetCurTimeStr().c_str(),GetCurrentThreadId(),GetTraceMsgHeader(msgType).c_str(),some_info.c_str());
	_snprintf(chs,3000,"%08X#%s%s",GetCurrentThreadId(),GetTraceMsgHeader(msgType).c_str(),some_info.c_str());
	GetGlobalApp()->RecordLog(chs);
	return ;

	std::ofstream out_file("debug_file.txt",std::ios::app);
	if(!out_file.is_open())
	{
		return ;
	}
	out_file<<chs<<std::endl;
	out_file.close();

}

void sys_debug_var(const std::string& var_name,int var)
{
	char chs[100]={0};
	sprintf(chs,"name:%s,value:%d",var_name.c_str(),var);
	sys_trace_info(chs);
}
void sys_debug_var(const std::string& var_name,std::string var)
{
	char chs[1000]={0};
	_snprintf(chs,800,"name:%s,value:%s",var_name.c_str(),var.c_str());
	sys_trace_info(chs);
}

void sys_debug_var(const std::string& var_name,std::list<std::string> str_list)
{
	std::stringstream ss;
	ss<<"name:"<<var_name<<"->[";
	for(std::list<std::string>::iterator it = str_list.begin();it!=str_list.end();++it)
	{
		ss<<*it<<",";
	}
	ss<<"]";
	sys_trace_info(ss.str());
	
}


void sys_debug_var(const std::string& var_name,SYSTEMTIME var)
{
    char strTime[100]={0};
    sprintf(strTime,"%04d-%02d-%02d %02d:%02d:%02d.%03d",var.wYear,var.wMonth,var.wDay,var.wHour,var.wMinute,var.wSecond,var.wMilliseconds);
	char chs[100]={0};
	sprintf(chs,"name:%s,value:%s",var_name.c_str(),strTime);
	sys_trace_info(chs,TW_TRACE_TYPE_DEBUGVALUE);
}

void sys_debug_var(const std::string& var_name,std::map<std::string,void*> dict)
{
    char chs[100]={0};
    sprintf(chs,"name:%s,value:\n",var_name.c_str());
    sys_trace_info(chs,TW_TRACE_TYPE_DEBUGVALUE);
    for(std::map<std::string, void*>::iterator it = dict.begin();it!=dict.end();++it)
    {
        char mem_chs[100]={0};
        _snprintf(mem_chs,80,"key:%s,value:%p\n",it->first.c_str(),it->second);
        sys_trace_info(mem_chs,TW_TRACE_TYPE_DEBUGVALUE);
    }
}