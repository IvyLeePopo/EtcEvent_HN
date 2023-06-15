#include "stdafx.h"
#include "RunParameter.h"
#include "pystring.h"
//#include <Windows.h>
#include "TWSDNetPay_Dll.h"
//////////////////////////////////////////////////////////////
// ��������:GetDllConfigPath
//
// ��������:��ȡdll���ص������ļ�·��
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetDllConfigPath()
{
    static std::string module_path = GetSelfModulePath();
    std::string module_dir = pystring::os::path::dirname(module_path);
    return pystring::os::path::join(module_dir,"filePathConfig.txt");
}

//////////////////////////////////////////////////////////////
// ��������:GetNetPayConfigPath
//
// ��������:��ȡҵ���������ļ�·��
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetNetPayConfigPath()
{
    std::string module_path = GetSelfModulePath();
    std::string module_dir = pystring::os::path::dirname(module_path);
    return pystring::os::path::join(module_dir,"TWSDNetPayConfig.ini");
}




//////////////////////////////////////////////////////////////
// ��������:GetSelfModulePath
//
// ��������:��ȡģ�鱻���ص�ʵ��·��
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:����������VirtualQuery֧�ֵ���Ͱ汾��xpϵͳ������ʵ�ʲ����У�2000 ϵͳ��Ȼ����֧��
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
// ��������:GetDllDir
//
// ��������:��ȡ��ģ���ʵ������Ŀ¼
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetDllDir()
{
    static std::string dll_path = GetSelfModulePath();
    return pystring::os::path::dirname(dll_path);
}

//////////////////////////////////////////////////////////////
// ��������:GetUpdateDir
//
// ��������:��ȡ�����ļ��Ĵ��·��
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetUpdateDir()
{
	return pystring::os::path::join(GetDllDir(),"TWUpdate");
}

//////////////////////////////////////////////////////////////
// ��������:GetBackupDir
//
// ��������:��ȡ���¹����У���̬��ı��ݴ��Ŀ¼
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetBackupDir()
{
	return pystring::os::path::join(GetDllDir(),"TWBackup");   
}

//////////////////////////////////////////////////////////////
// ��������:GetObuWhiteListRecordLifeSpanSeconds
//
// ��������:��������¼�Ĵ��ʱ�䡣��λ����
//
// �������:
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int GetObuWhiteListRecordLifeSpanSeconds()
{
    return 5;
}
//////////////////////////////////////////////////////////////
// ��������:GetDebitRecordLifeSpanDays
//
// ��������:�۷ѹ鵵��¼�ı���ʱ�䡣��λ����
//
// �������:
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int GetDebitRecordLifeSpanDays()
{
    return 365;
}
//////////////////////////////////////////////////////////////
// ��������:GetDebitRequestCachePath
//
// ��������:�����ļ��Ĵ��·��
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetDebitRequestCachePath()
{
    return pystring::os::path::join(GetDllDir(),"TWSDNetPayLog\\TWDebitCache.db");
}

//////////////////////////////////////////////////////////////
// ��������:GetDebitParaCachePath
//
// ��������:�ۿ��������λ�ã��˴���Ϊ��
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-17
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
// 1����ʾ�ۿ�ṹ��ı���λ�á�
//
//////////////////////////////////////////////////////////////
std::string GetDebitParaTMPCachePath()
{
    return pystring::os::path::join(GetDllDir(),"TWSDNetPayLog\\TWDebitParaTmp.db");
}
//////////////////////////////////////////////////////////////
// ��������:GetDebitRecordArchiveDir
//
// ��������:��¼���׹鵵����Ŀ¼��
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2018-12-29
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetDebitRecordArchiveDir()
{
    return pystring::os::path::join(GetDllDir(),"TWRecordArchive");
}  

//////////////////////////////////////////////////////////////
// ��������:GetETCPayURL
//
// ��������:����ETC֧��URL
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetETCPayURL()
{
    return "/withhold";
}
//////////////////////////////////////////////////////////////
// ��������:GetETCPayPort
//
// ��������:����ETC֧���˿�
//
// �������:
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int GetETCPayPort()
{
    static int port = ReadIniInt(GetNetPayConfigPath(),"ReflectWeb","ETCPort",80);
	return port;
}
//////////////////////////////////////////////////////////////
// ��������:GetETCIP
//
// ��������:���� ETC����IP
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetETCIP()
{
    static std::string ip = ReadIniStr(GetNetPayConfigPath(),"ReflectWeb","ETCIP","101.200.214.203");
    return ip;
}
//////////////////////////////////////////////////////////////
// ��������:GetETCAuthURL
//
// ��������:����ETC��������ȨURL
//
// �������:
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string GetETCAuthURL()
{
    return "/withhold/vehicleValidation";
}

//////////////////////////////////////////////////////////////
// ��������:GetETCAuthPort
//
// ��������:����ETC��Ȩ�˿�
//
// �������:
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
int GetETCAuthPort()
{
    return GetETCPayPort();
}

//////////////////////////////////////////////////////////////
// ��������:ReadIniStr
//
// ��������:INI�����ļ���������ȡ�ַ���
//
// �������:const std::string &file_path, const std::string &section, const std::string &key, const std::string &def,
// �������:std::string 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
//
//////////////////////////////////////////////////////////////
std::string ReadIniStr(const std::string &file_path, const std::string &section, const std::string &key, const std::string &def)
{
    char buf[1000] = {0};
    ::GetPrivateProfileString(section.c_str(), key.c_str(), def.c_str(), buf, sizeof(buf), file_path.c_str());
    return std::string(buf);
}

//////////////////////////////////////////////////////////////
// ��������:ReadIniInt
//
// ��������:INI�����ļ�������ȡ����ȡ������
//
// �������:const std::string &file_path, const std::string &section, const std::string &key, int def,
// �������:int 
// ��д��Ա:���Ǳ�
// ��дʱ��:2019-01-11
// �޸���Ա:
// �޸�ʱ��:
// �����汾:1.0.0.0
// ��ע˵��:
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

