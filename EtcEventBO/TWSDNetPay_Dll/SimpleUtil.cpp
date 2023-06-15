#include "stdafx.h"
#include <io.h>
#include "SimpleUtil.h"
#include "md5.h"
#include "pystring.h"

#pragma comment(lib, "Version.lib")

namespace Util {

LARGE_INTEGER query_machine_freq()
{
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    return freq_;
}

LARGE_INTEGER CountDown::perfermance_freq = query_machine_freq();

CountDown::CountDown(int ms)
{
    duration_ms = ms;
    begin_time.QuadPart = 0;
    QueryPerformanceCounter(&begin_time);

}
CountDown::~CountDown() {}

bool CountDown::Over()
{
    if (perfermance_freq.QuadPart == 0)
    {
        return true;
    }
    LARGE_INTEGER curr_time;
    QueryPerformanceCounter(&curr_time);

    double diff_s = (curr_time.QuadPart - begin_time.QuadPart) / (double)perfermance_freq.QuadPart;
    int diff_ms = static_cast<int>(diff_s * 1000);
    if (diff_ms > duration_ms)
    {
        return true;
    }
    return false;

}


std::string GetVersionOfFile(const std::string& lpsPath)
{
    DWORD   dwLength = 0;
    DWORD   dwNullHandle = 0;
    UINT    uiVerLength;
    LPVOID  lpVersionPtr;

    char fileVersion[200] = {0};
    strcpy(fileVersion, "0.0.0.0");

    dwLength = ::GetFileVersionInfoSize(lpsPath.c_str(), &dwNullHandle);
    if (dwLength <= 0)
    {
        return fileVersion;
    }

    BYTE* pVersionInfo = new BYTE [dwLength];
    if (pVersionInfo == 0)
    {
        return fileVersion;
    }
    if (::GetFileVersionInfo (lpsPath.c_str(), NULL, dwLength, pVersionInfo))
    {
        if (::VerQueryValue (pVersionInfo, "\\", &lpVersionPtr, &uiVerLength))
        {
            VS_FIXEDFILEINFO * p = (VS_FIXEDFILEINFO *)lpVersionPtr;
            _snprintf(fileVersion, 150, "%d.%d.%d.%d", HIWORD(p->dwFileVersionMS), LOWORD(p->dwFileVersionMS), HIWORD(p->dwFileVersionLS), LOWORD(p->dwFileVersionLS));
        }
    }
    delete []pVersionInfo;


    return fileVersion;
}




std::string StringConvertByCodePage( const std::string& strConvertData_org, IN const int& iSourceCodepage, IN const int& iTargetCodepage)
{
    CString strConvertData;
    strConvertData.Format(_T("%s"), strConvertData_org.c_str());
    CString strReturn;
    CString strRealConvertData(strConvertData);
    int len = strRealConvertData.GetLength();
    int unicodeLen = MultiByteToWideChar(iSourceCodepage, 0, strRealConvertData, -1, NULL, 0);

    wchar_t *pUnicode;
    pUnicode = new wchar_t[unicodeLen + 1];
    if (NULL != pUnicode)
    {
        memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
        MultiByteToWideChar(iSourceCodepage, 0, strConvertData, -1, (LPWSTR)pUnicode, unicodeLen);

        BYTE *pTargetData = NULL ;
        int targetLen = WideCharToMultiByte(iTargetCodepage, 0, (LPWSTR)pUnicode, -1, (char*)pTargetData, 0, NULL, NULL);

        pTargetData = new BYTE[targetLen + 1];
        if (NULL != pTargetData)
        {
            memset(pTargetData, 0, targetLen + 1);

            WideCharToMultiByte(iTargetCodepage, 0, (LPWSTR)pUnicode, -1, (char*)pTargetData, targetLen, NULL, NULL);

            strReturn.Format(_T("%s"), pTargetData);

            delete []pTargetData;
        }
        delete []pUnicode;
    }
    std::string code_str = (LPCSTR)strReturn;
    return code_str ;
}



std::string ConvertLocalTime2Str(time_t t)
{
    if (t < 1000)
    {
        return "1970-01-01 00:00:00";
    }
    struct tm test_tm =  *localtime(&t);
    char time_chs[100] = {0};
    sprintf(time_chs, "%04d-%02d-%02d %02d:%02d:%02d", test_tm.tm_year + 1900, test_tm.tm_mon + 1, test_tm.tm_mday, test_tm.tm_hour, test_tm.tm_min, test_tm.tm_sec);
    return std::string(time_chs);
}



std::string GetLocalTimeStr()
{
    return ConvertLocalTime2Str(time(0));
}



std::string Int2Str(int info)
{
    char numStr[30] = {0};
    itoa(info, numStr, 10);
    return numStr;
}

int Str2Int(const std::string& info)
{
    return atoi(info.c_str());
}




time_t FileTimeToTime_t(const FILETIME &ft)
{
    ULARGE_INTEGER ui;
    ui.LowPart = ft.dwLowDateTime;
    ui.HighPart = ft.dwHighDateTime;
    return ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
}

__int64 GetDiffMilliSeconds(const SYSTEMTIME &t1, const SYSTEMTIME &t2)
{
    FILETIME fTime1 = { 0, 0 };
    FILETIME fTime2 = { 0, 0 };
    SystemTimeToFileTime(&t1, &fTime1);
    SystemTimeToFileTime(&t2, &fTime2);

    time_t tt1 = FileTimeToTime_t(fTime1);
    time_t tt2 = FileTimeToTime_t(fTime2);

    __int64 diff_milli_secs =  static_cast<__int64>((tt2 - tt1) * 1000 + (t2.wMilliseconds - t1.wMilliseconds));

    return diff_milli_secs;
}


time_t Str2LocalTime(const std::string& time_str)
{
    SYSTEMTIME china_t;
    memset(&china_t, 0, sizeof(SYSTEMTIME));
    sscanf(time_str.c_str(), "%d-%d-%d %d:%d:%d", &china_t.wYear, &china_t.wMonth, &china_t.wDay, &china_t.wHour, &china_t.wMinute, &china_t.wSecond);

    struct tm china_tm = { china_t.wSecond, china_t.wMinute, china_t.wHour, china_t.wDay, china_t.wMonth - 1, china_t.wYear - 1900, china_t.wDayOfWeek, 0, 0 };
    return mktime(&china_tm);
}


std::string GetGuid()
{
    char buffer[100] = { 0 };
    GUID guid;

    if (S_OK != CoCreateGuid(&guid))
    {
        return "";
    }
    _snprintf(buffer, sizeof(buffer),
              "%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X",
              guid.Data1, guid.Data2, guid.Data3,
              guid.Data4[0], guid.Data4[1], guid.Data4[2],
              guid.Data4[3], guid.Data4[4], guid.Data4[5],
              guid.Data4[6], guid.Data4[7]);
    return std::string(buffer);
}



bool MakeDirectory(std::string raw_path)
{
    CString strCreateDir = raw_path.c_str();
    HANDLE  fFile;                      // File Handle
    WIN32_FIND_DATA fileinfo = {0};     // File Information Structure
    CStringArray arr;               // CString Array to hold Directory Structures
    BOOL bSuccess = false;              // BOOL used to test if Create Directory was successful
    int nCount = 0;                     // Counter
    CString strTemp;                    // Temporary CString Object

    fFile = FindFirstFile(strCreateDir, &fileinfo);

    // if the file exists and it is a directory
    if (fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    {
        //  Directory Exists close file and return
        FindClose(fFile);
        return true;
    }
    arr.RemoveAll();

    for (nCount = 0; nCount < strCreateDir.GetLength(); nCount++ ) // Parse the supplied CString Directory String
    {
        if (strCreateDir.GetAt(nCount) != '\\') // if the Charachter is not a \ 
            strTemp += strCreateDir.GetAt(nCount);  // add the character to the Temp String
        else
        {
            arr.Add(strTemp);   // if the Character is a \ 
            strTemp += _T("\\");   // Now add the \ to the temp string
        }
        if (nCount == strCreateDir.GetLength() - 1) // If we reached the end of the String
            arr.Add(strTemp);
    }

    // Close the file
    FindClose(fFile);

    // Now lets cycle through the String Array and create each directory in turn
    for (nCount = 1; nCount < arr.GetSize(); nCount++)
    {
        strTemp = arr.GetAt(nCount);
        bSuccess = CreateDirectory(strTemp, NULL);

        // If the Directory exists it will return a false
        if (bSuccess)
            SetFileAttributes(strTemp, FILE_ATTRIBUTE_NORMAL);
        // If we were successful we set the attributes to normal
    }
    //  Now lets see if the directory was successfully created
    fFile = FindFirstFile(strCreateDir, &fileinfo);

    arr.RemoveAll();
    if (fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    {
        //  Directory Exists close file and return
        FindClose(fFile);
        return true;
    }
    else
    {
        // For Some reason the Function Failed  Return FALSE
        FindClose(fFile);
        return false;
    }
    return false;
}


std::string CalcMD5(const std::string& info)
{
    return md5(info);
}

std::vector<std::string> FindTargetFile(const FindFileInfo& find_para)
{
    long find_handle = 0;
    struct _finddata_t find_data;
    std::vector<std::string> target_files;
    find_handle = _findfirst(find_para.file_pattern, &find_data);

    if (find_handle == -1L)
    {
        return std::vector<std::string>();
    }

    do {
        if (find_data.attrib & _A_SUBDIR)
        {
            continue;
        }
        if ( find_para.create_before > 0 &&
                (find_data.time_create > find_para.create_before ))
        {//time check
            continue;
        }

        std::string abs_path = pystring::os::path::join(pystring::os::path::dirname(find_para.file_pattern), find_data.name);
        target_files.push_back(abs_path);
    } while (_findnext(find_handle, &find_data) == 0);
    _findclose(find_handle);

    return target_files;
}


} // Util
