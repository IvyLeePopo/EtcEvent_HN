#ifndef FUNC_PROTO_TYPE_H
#define FUNC_PROTO_TYPE_H
#include <map>
#include <string>

struct DllProtoTypeInfo
{
    HMODULE dll_handle;
    std::string dll_path;
    std::map<std::string,void*> func_address_group;
    DllProtoTypeInfo()
    {
        dll_handle = 0;
    }
};


// ����ת������
typedef void (__cdecl* DataForward_Register)(int key, void* pClass, char *topic);
typedef void (__cdecl* DataForward_Unregister)(int key);
typedef void (__cdecl* DataForward_Write)(int destKey, char *message, int len, int srcKey, bool pointerFlag);
typedef void (__cdecl* DataForward_Read)(int destKey, char *message, int len, int srcKey);

// ת�����ĺ�������
typedef bool (WINAPI *CoreFun_InitEnvironment) (IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID);
typedef bool (WINAPI *CoreFun_InitEnvironment2) (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, void (WINAPI *fun)(int option, int result));
typedef bool (WINAPI *CoreFun_InitEnvironment3) (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void (WINAPI *fun)(int option, int result, void* pIndPtr));
typedef bool (WINAPI *CoreFun_InitEnvironment4) (IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, OUT int& iOverTime, OUT int& iTimeOut);
//������Դ          -- �ӿ�2
typedef bool (WINAPI *CoreFun_Destroy)(void);
//��ȡ��һ�δ�����Ϣ
typedef char* (WINAPI *CoreFun_GetLastErrorDesc) (void);
//������ȡ�û���Ϣ����        -- �ӿ�4
typedef bool (WINAPI *CoreFun_StartGetAccountInfo) ( const int& iOvertime);
//ֹͣ��ȡ�û���Ϣ����        -- �ӿ�5
typedef bool (WINAPI *CoreFun_StopGetAccountInfo) (void);
//��ȡ�û���Ϣ                -- �ӿ�6
typedef bool (WINAPI *CoreFun_GetAccountInfo) (char* szParamContext, int& iParamSize, const int& iFormatType);
//ִ�пۿ����                -- �ӿ�7
typedef bool (WINAPI *CoreFun_DebitMoney) ( const char* szParamContext, const int& iParamSize, const int& iFormatType);
//ִ�г�������                -- �ӿ�8
typedef bool (WINAPI *CoreFun_DebitCancel) ( const char* szParamContext, const int& iParamSize, const int& iFormatType);
//ִ�г�������                -- �ӿ�8-2        --��Ͷ��Ŀ��������
typedef bool (WINAPI *CoreFun_DebitCancel2) (IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//��ȡ�ۿ���                -- �ӿ�9
typedef bool (WINAPI *CoreFun_GetDebitResult) (char* szParamContext, int& iParamSize, const int& iFormatType);
//���ý�������                -- �ӿ�10
typedef bool (WINAPI *CoreFun_SetMMI) (const char* szParamContext, const int& iParamSize, const int& iFormatType);
//��ȡӲ������                -- �ӿ�11
typedef bool (WINAPI *CoreFun_GetParam) ( const char* szParamContext, const int& iParamSize, const int& iFormatType, char* szResultContext, int& iResultSize);
//������״̬                -- �ӿ�12
typedef bool (WINAPI *CoreFun_GetComponentStatus) ( unsigned int& uStatus);
//ִ�д���ؼ���Ϣ���ݲ���                  -- �ӿ�13
typedef bool (WINAPI *CoreFun_TranslateData) ( const char* szParamContext, const int& iParamSize, const int& iFormatType);
//��ȡ�������                -- �ӿ�14
typedef bool (WINAPI *CoreFun_GetCancelResult) (char* szParamContext, int& iParamSize, const int& iFormatType);

typedef bool (WINAPI *CoreFun_TransferImage)(IN const char* szImageInfo, IN const int& iSize, IN const int& iFormatType);

typedef bool (WINAPI *CoreFun_AgencyDebitQuery)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT int& iResult);


typedef bool (WINAPI *CoreFun_Authority)(IN const char* szAuthKey, IN const int& iAuthKeySize);

/*ITC�ӿ�*/
typedef int  (WINAPI* CoreFun_ITCEntryTrade)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout);
typedef int  (WINAPI* CoreFun_ITCEntryQuery)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout, IN bool bSync);
typedef int  (WINAPI* CoreFun_ITCExitQuery) (IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szEntryInfo, IN OUT int& iEntryInfoSize, IN int iTimeout, IN bool bSync);
typedef int  (WINAPI* CoreFun_ITCExitTrade)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout);
typedef int  (WINAPI* CoreFun_ITCQueryPath) (IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout);
typedef bool (WINAPI* CoreFun_GetServerTime)(IN OUT unsigned int& nServerTime, IN const int& iLimitTime);
typedef bool (WINAPI* CoreFun_CompletionTicketNo)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);

/*����ֵ�ؽӿ�*/
typedef bool (WINAPI* CoreFun_SetVehicleStatus)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
typedef bool (WINAPI* CoreFun_PrinterTicket)(IN  const  char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
typedef bool (WINAPI* CoreFun_ReadCardInfo)(IN OUT char* szParamContext, IN OUT int& iParamSize, IN const int& iFormatType);
typedef bool (WINAPI* CoreFun_WriteCardInfo)(IN OUT char* szParamContext, IN OUT int& iParamSize, IN const int& iFormatType);

/*������⿨��ѯ�ӿ�*/
typedef bool (WINAPI* CoreFun_QueryVirtualCardDebit)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN int& iSupportAgent, OUT char* szResult);
/*��ʱ�������ܽӿڶ���*/
typedef bool (WINAPI* CoreFun_TMP_SetUnipayEncrypt)(int setType, const char *setData, char* retData);


#endif // FUNC_PROTO_TYPE_H

