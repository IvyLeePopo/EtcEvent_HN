#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#include <string> 
#include <vector>

namespace Util
{
    
class CountDown
{
public:
    CountDown(int ms);
    ~CountDown();
    bool Over();
    static LARGE_INTEGER perfermance_freq;
private:
    LARGE_INTEGER begin_time;
    int duration_ms;
    
};


std::string GetVersionOfFile(const std::string& lpsPath);
std::string StringConvertByCodePage( const std::string& strConvertData_org, IN const int& iSourceCodepage, IN const int& iTargetCodepage);
std::string ConvertLocalTime2Str(time_t t);
std::string GetLocalTimeStr();

std::string Int2Str(int info);
int Str2Int(const std::string& info);

__int64 GetDiffMilliSeconds(const SYSTEMTIME &t1, const SYSTEMTIME &t2);
time_t Str2LocalTime(const std::string& time_str);

std::string GetGuid();

bool MakeDirectory(std::string raw_path);

std::string CalcMD5(const std::string& info);

struct FindFileInfo
{
    char file_pattern[300];
    time_t create_before;
    FindFileInfo()
    {
        memset(this,0,sizeof(FindFileInfo));
    }
};
std::vector<std::string> FindTargetFile(const FindFileInfo& find_info);

}



#endif // SIMPLE_UTIL_H


/**
 * 杂项处理过程
 */