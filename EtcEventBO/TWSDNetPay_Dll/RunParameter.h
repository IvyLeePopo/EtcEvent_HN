#ifndef RUN_PARAMETER_H
#define RUN_PARAMETER_H

#include <string>

/*获取此模块的完整路径*/
std::string GetSelfModulePath();

/*获取dll加载的配置文件路径*/
std::string GetDllConfigPath();

/*获取此动态库的所在目录*/
std::string GetDllDir();

/*获取更新文件的存放目录*/
std::string GetUpdateDir();
/*获取更新过程中的中间件备份目录*/
std::string GetBackupDir();
/*Ini配置读取辅助*/
std::string ReadIniStr(const std::string &file_path, const std::string &section, const std::string &key, const std::string &def);
int ReadIniInt(const std::string &file_path, const std::string &section, const std::string &key, int def);

/*扣款参数临时保留位置*/
std::string GetDebitParaTMPCachePath();

/*白名单记录寿命,有效时间*/
int GetObuWhiteListRecordLifeSpanSeconds();
/*记录归档保存时间*/
int GetDebitRecordLifeSpanDays(); 
/*交易缓存文件存放路径*/
std::string GetDebitRequestCachePath();
/*交易归档文件保存目录*/
std::string GetDebitRecordArchiveDir();
/*甘肃ETC交易接口URL*/
std::string GetETCPayURL();
/*甘肃ETC交易接口端口*/
int GetETCPayPort();
/*甘肃ETC服务端IP*/
std::string GetETCIP();
/*甘肃ETC白名单鉴权URL*/
std::string GetETCAuthURL();
/*甘肃ETC白名单鉴权端口*/
int GetETCAuthPort();

std::string GetAgencyDllPath();

int  GetUpdatePort(); //add 2021-12-03 lhq
std::string GetSecondPayIP();//add 2021-11-22 lhq
std::string GetPayIP();
int  GetPayPort();
std::string GetMediaIP();
int GetMediaPort();

std::string GetInvoicePlateformIP();
std::string GetInvoicePlateformSecondIP();
int  GetInvoicePlateformPort();
std::string GetInvoiceUrlPrefix();

std::string GetNetPayDllVersion();
std::string GetDeviceDllVersion();
std::string GetTWProductSerialNo();
std::string GetNFCTerminal();
std::string GetNFCSerialNo();
std::string GetDebitCancleCachePath();
std::string GetInvoiceCancleCachePath();
std::string GetInvoiceCachePath();
int GetDuplicateDebitShortestMinutes();  //重复交易最短时间间隔。

bool IsUseExternMMI();//用于失败界面的提示，从原来的代码中和测试中，发现此配置并无作用
bool IsAllowUseNewMMIMode();  //用于失败界面的提示。
bool IsUseAgencyDll();
int GetAgencyPayTimeoutMsec();
int GetDebitCancleCacheEffectiveMinutes();
bool IsTianJinArea();
bool IsHeNanArea();
bool IsJiLinArea();
bool IsUseUnifyPayChannelCode();


#endif // RUN_PARAMETER_H


/**
 * 此处定义，系统在运行过程中需要使用的参数。
 */
