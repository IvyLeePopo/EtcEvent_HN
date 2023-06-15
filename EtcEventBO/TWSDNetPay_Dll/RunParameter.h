#ifndef RUN_PARAMETER_H
#define RUN_PARAMETER_H

#include <string>

/*��ȡ��ģ�������·��*/
std::string GetSelfModulePath();

/*��ȡdll���ص������ļ�·��*/
std::string GetDllConfigPath();

/*��ȡ�˶�̬�������Ŀ¼*/
std::string GetDllDir();

/*��ȡ�����ļ��Ĵ��Ŀ¼*/
std::string GetUpdateDir();
/*��ȡ���¹����е��м������Ŀ¼*/
std::string GetBackupDir();
/*Ini���ö�ȡ����*/
std::string ReadIniStr(const std::string &file_path, const std::string &section, const std::string &key, const std::string &def);
int ReadIniInt(const std::string &file_path, const std::string &section, const std::string &key, int def);

/*�ۿ������ʱ����λ��*/
std::string GetDebitParaTMPCachePath();

/*��������¼����,��Чʱ��*/
int GetObuWhiteListRecordLifeSpanSeconds();
/*��¼�鵵����ʱ��*/
int GetDebitRecordLifeSpanDays(); 
/*���׻����ļ����·��*/
std::string GetDebitRequestCachePath();
/*���׹鵵�ļ�����Ŀ¼*/
std::string GetDebitRecordArchiveDir();
/*����ETC���׽ӿ�URL*/
std::string GetETCPayURL();
/*����ETC���׽ӿڶ˿�*/
int GetETCPayPort();
/*����ETC�����IP*/
std::string GetETCIP();
/*����ETC��������ȨURL*/
std::string GetETCAuthURL();
/*����ETC��������Ȩ�˿�*/
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
int GetDuplicateDebitShortestMinutes();  //�ظ��������ʱ������

bool IsUseExternMMI();//����ʧ�ܽ������ʾ����ԭ���Ĵ����кͲ����У����ִ����ò�������
bool IsAllowUseNewMMIMode();  //����ʧ�ܽ������ʾ��
bool IsUseAgencyDll();
int GetAgencyPayTimeoutMsec();
int GetDebitCancleCacheEffectiveMinutes();
bool IsTianJinArea();
bool IsHeNanArea();
bool IsJiLinArea();
bool IsUseUnifyPayChannelCode();


#endif // RUN_PARAMETER_H


/**
 * �˴����壬ϵͳ�����й�������Ҫʹ�õĲ�����
 */
