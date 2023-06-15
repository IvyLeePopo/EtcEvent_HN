#include "stdafx.h"
#include "RunParameter.h"
#include "pystring.h"
//#include <Windows.h>
#include "TWSDNetPay_Dll.h"
//////////////////////////////////////////////////////////////
// 函数名称:GetDllConfigPath
//
// 功能描述:获取dll加载的配置文件路径
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetDllConfigPath()
{
    static std::string module_path = GetSelfModulePath();
    std::string module_dir = pystring::os::path::dirname(module_path);
    return pystring::os::path::join(module_dir,"filePathConfig.txt");
}

//////////////////////////////////////////////////////////////
// 函数名称:GetNetPayConfigPath
//
// 功能描述:获取业务库的配置文件路径
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetNetPayConfigPath()
{
    std::string module_path = GetSelfModulePath();
    std::string module_dir = pystring::os::path::dirname(module_path);
    return pystring::os::path::join(module_dir,"TWSDNetPayConfig.ini");
}




//////////////////////////////////////////////////////////////
// 函数名称:GetSelfModulePath
//
// 功能描述:获取模块被加载的实际路径
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:官网描述，VirtualQuery支持的最低版本是xp系统，但是实际测试中，2000 系统依然可以支持
//
//////////////////////////////////////////////////////////////
std::string GetSelfModulePath()
{
    MEMORY_BASIC_INFORMATION mbi;
    HMODULE dll_handle = ((::VirtualQuery(GetSelfModulePath, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
    TCHAR t_path[MAX_PATH] = { 0 };
    if(GetModuleFileName(dll_handle, t_path, MAX_PATH)==0)
	{
		return std::string();
	}
	return std::string(t_path);
}

//////////////////////////////////////////////////////////////
// 函数名称:GetDllDir
//
// 功能描述:获取本模块的实际所在目录
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetDllDir()
{
    static std::string dll_path = GetSelfModulePath();
    return pystring::os::path::dirname(dll_path);
}

//////////////////////////////////////////////////////////////
// 函数名称:GetUpdateDir
//
// 功能描述:获取更新文件的存放路径
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetUpdateDir()
{
	return pystring::os::path::join(GetDllDir(),"TWUpdate");
}

//////////////////////////////////////////////////////////////
// 函数名称:GetBackupDir
//
// 功能描述:获取更新过程中，动态库的备份存放目录
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetBackupDir()
{
	return pystring::os::path::join(GetDllDir(),"TWBackup");   
}

//////////////////////////////////////////////////////////////
// 函数名称:GetObuWhiteListRecordLifeSpanSeconds
//
// 功能描述:白名单记录的存放时间。单位：秒
//
// 输入参数:
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int GetObuWhiteListRecordLifeSpanSeconds()
{
    return 5;
}
//////////////////////////////////////////////////////////////
// 函数名称:GetDebitRecordLifeSpanDays
//
// 功能描述:扣费归档记录的保存时间。单位：天
//
// 输入参数:
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int GetDebitRecordLifeSpanDays()
{
    return 365;
}
//////////////////////////////////////////////////////////////
// 函数名称:GetDebitRequestCachePath
//
// 功能描述:缓存文件的存放路径
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetDebitRequestCachePath()
{
    return pystring::os::path::join(GetDllDir(),"TWSDNetPayLog\\TWDebitCache.db");
}

//////////////////////////////////////////////////////////////
// 函数名称:GetDebitParaCachePath
//
// 功能描述:扣款参数保留位置，此处是为了
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-17
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
// 1、表示扣款结构体的保存位置。
//
//////////////////////////////////////////////////////////////
std::string GetDebitParaTMPCachePath()
{
    return pystring::os::path::join(GetDllDir(),"TWSDNetPayLog\\TWDebitParaTmp.db");
}
//////////////////////////////////////////////////////////////
// 函数名称:GetDebitRecordArchiveDir
//
// 功能描述:记录交易归档保存目录。
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2018-12-29
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetDebitRecordArchiveDir()
{
    return pystring::os::path::join(GetDllDir(),"TWRecordArchive");
}  

//////////////////////////////////////////////////////////////
// 函数名称:GetETCPayURL
//
// 功能描述:甘肃ETC支付URL
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetETCPayURL()
{
    return "/withhold";
}
//////////////////////////////////////////////////////////////
// 函数名称:GetETCPayPort
//
// 功能描述:甘肃ETC支付端口
//
// 输入参数:
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int GetETCPayPort()
{
    static int port = ReadIniInt(GetNetPayConfigPath(),"ReflectWeb","ETCPort",80);
	return port;
}
//////////////////////////////////////////////////////////////
// 函数名称:GetETCIP
//
// 功能描述:甘肃 ETC服务IP
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetETCIP()
{
    static std::string ip = ReadIniStr(GetNetPayConfigPath(),"ReflectWeb","ETCIP","101.200.214.203");
    return ip;
}
//////////////////////////////////////////////////////////////
// 函数名称:GetETCAuthURL
//
// 功能描述:甘肃ETC白名单鉴权URL
//
// 输入参数:
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetETCAuthURL()
{
    return "/withhold/vehicleValidation";
}

//////////////////////////////////////////////////////////////
// 函数名称:GetETCAuthPort
//
// 功能描述:甘肃ETC鉴权端口
//
// 输入参数:
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int GetETCAuthPort()
{
    return GetETCPayPort();
}

//////////////////////////////////////////////////////////////
// 函数名称:ReadIniStr
//
// 功能描述:INI配置文件辅助，读取字符串
//
// 输入参数:const std::string &file_path, const std::string &section, const std::string &key, const std::string &def,
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string ReadIniStr(const std::string &file_path, const std::string &section, const std::string &key, const std::string &def)
{
    char buf[1000] = {0};
    ::GetPrivateProfileString(section.c_str(), key.c_str(), def.c_str(), buf, sizeof(buf), file_path.c_str());
    return std::string(buf);
}

//////////////////////////////////////////////////////////////
// 函数名称:ReadIniInt
//
// 功能描述:INI配置文件辅助读取，读取整数。
//
// 输入参数:const std::string &file_path, const std::string &section, const std::string &key, int def,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int ReadIniInt(const std::string &file_path, const std::string &section, const std::string &key, int def)
{
    return ::GetPrivateProfileInt(section.c_str(), key.c_str(), def, file_path.c_str());
}




std::string GetAgencyDllPath()
{
    static std::string module_path = GetSelfModulePath();
    std::string module_dir = pystring::os::path::dirname(module_path);
    return pystring::os::path::join(module_dir,"TWITC_AGENCY.dll");
}

std::string GetDebitCancleCachePath()
{
    return pystring::os::path::join(GetDllDir(),"TWSDNetPayLog\\TWDebitCancleCache.db");
}

std::string GetInvoiceCancleCachePath()
{
    return pystring::os::path::join(GetDllDir(),"TWSDNetPayLog\\TWInvoiceCancelCache.db");
}

std::string GetInvoiceCachePath()
{
    return pystring::os::path::join(GetDllDir(),"TWSDNetPayLog\\TWInvoiceCache.db");
}

int GetAgencyPayTimeoutMsec()
{
    return 2000;
}

//add 2021-11-22 lhq
std::string GetSecondPayIP()
{
	static std::string ip = ReadIniStr(GetNetPayConfigPath(),"ReflectWeb","IPSECOND","101.200.214.203");
	return ip;
}

std::string GetPayIP()
{
    static std::string ip = ReadIniStr(GetNetPayConfigPath(),"ReflectWeb","IP","101.200.214.203");
    return ip;
}

int  GetUpdatePort()
{
	static int port = ReadIniInt(GetNetPayConfigPath(),"ReflectWeb","UpdatePort",8090);
	return port;
}


int  GetPayPort()
{
    static int port = ReadIniInt(GetNetPayConfigPath(),"ReflectWeb","Port",80);
    return port;
}

std::string GetMediaIP()
{
	static std::string ip = ReadIniStr(GetNetPayConfigPath(),"MediaGateWay","IP","101.200.214.203");
    return ip;
}

int GetMediaPort()
{
	static int port = ReadIniInt(GetNetPayConfigPath(),"MediaGateWay","Port",80);
    return port;
}

std::string GetInvoicePlateformIP()
{
    static std::string ip = ReadIniStr(GetNetPayConfigPath(),"InvoiceReflectWeb","IP","101.200.214.203");
    return ip;
}

std::string GetInvoicePlateformSecondIP()
{
	const std::string FirstIP = GetInvoicePlateformIP();
    static std::string ip = ReadIniStr(GetNetPayConfigPath(),"InvoiceReflectWeb","IPSECOND",FirstIP);
    return ip;
}

int GetInvoicePlateformPort()
{
    static int port = ReadIniInt(GetNetPayConfigPath(),"InvoiceReflectWeb","Port",80);
    return port;
}

std::string GetInvoiceUrlPrefix()
{
    static std::string UrlPrefix = ReadIniStr(GetNetPayConfigPath(),"InvoiceReflectWeb","UrlPrefix","https://ytcard.trawe.cn/invoice/index.html");
    return UrlPrefix;
}

std::string GetNetPayDllVersion()
{
    std::string pay_version = (LPCSTR)(GetGlobalApp()->m_strNetPayDllVersion);
    return pay_version;
}
std::string GetDeviceDllVersion()
{
    std::string device_version = (LPCSTR)(GetGlobalApp()->m_strDeviceDllVersion);
    return device_version;
}

std::string GetTWProductSerialNo()
{
    std::string serial_no = (LPCSTR)(GetGlobalApp()->m_strTWProductSerialNo);
    return serial_no;
}

std::string GetNFCTerminal()
{
    CString tmp_str;
    GetGlobalApp()->HexToBase64(GetGlobalApp()->m_strNFCTerminal, tmp_str);
    std::string nfc_terminal_base64_str = (LPCSTR)tmp_str;
    return nfc_terminal_base64_str;
}
std::string GetNFCSerialNo()
{
    CString tmp_str;
    GetGlobalApp()->HexToBase64(GetGlobalApp()->m_strNFCSerialNo, tmp_str);
    std::string nfc_serial_base64_str = (LPCSTR)tmp_str;
    return nfc_serial_base64_str;
}

bool IsUseAgencyDll()
{
    static int use_dll =  ReadIniInt(GetNetPayConfigPath(),"Setup","UseAgencyPayDll",0);
    return (use_dll ==1);
}
bool IsTianJinArea()
{
    return (GetGlobalApp()->m_sTranstParam.ProvinceID ==12);
}
bool IsHeNanArea()
{
    return (GetGlobalApp()->m_sTranstParam.ProvinceID == 41);
}
bool IsJiLinArea()
{
    return (GetGlobalApp()->m_sTranstParam.ProvinceID == 22);   
}
bool IsUseUnifyPayChannelCode()
{
    static int use_unify_code =  ReadIniInt(GetNetPayConfigPath(),"Setup","ReturnPayChannelCodeUnify",0);
    return (use_unify_code ==1);
}

bool IsUseExternMMI()
{
    static int use_extern_mmi = ReadIniInt(GetNetPayConfigPath(),"Setup","UseExternMMI",0);
    return (use_extern_mmi==1);
}

bool IsAllowUseNewMMIMode()
{
	static int use_new_mmi = ReadIniInt(GetNetPayConfigPath(),"Setup","AllowUseNewMMIMode",0);
    return (use_new_mmi==1);
}
int GetDuplicateDebitShortestMinutes()
{
    static int shortest_minutes = ReadIniInt(GetNetPayConfigPath(),"ETC","DuplicateDebitCheckTime",5);
    return shortest_minutes;
}

int GetDebitCancleCacheEffectiveMinutes()
{
    return 20;
}

