#ifndef COMMONFRAMEMAIN_H
#define COMMONFRAMEMAIN_H


#define COMMONFRAMEMAIN_LIBRARY//add for exports

#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
   #include <wtypes.h>
    //#include <winbase.h>
    #include <iostream>
    #if defined(COMMONFRAMEMAIN_LIBRARY)
    #  define COMMONFRAMEMAINSHARED_EXPORT __declspec(dllexport)
    #else
    #  define COMMONFRAMEMAINSHARED_EXPORT __declspec(dllimport)
    #endif

#else
#  define COMMONFRAMEMAINSHARED_EXPORT
#endif
//////////////bad bad bad


#include "CommonBus.h"
#include <string>
#include <map>
#include<list>
#include <set>
#include "FrameConfigFileOperate.h"



//#pragma  comment(lib, "CommonBus.lib")

using namespace std;


#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
  typedef map<string,HINSTANCE> LibMap;
  typedef pair<string,HINSTANCE> PairLibMap;
  #define LOAD_LIB_PATH  "..\\loadLib\\"
  #define DOWNLOAD_FILE_PATH  "..\\download\\"
  #define COMMON_LIB_PATH     "..\\lib\\"
	#define dlsym GetProcAddress  //add 2017-11 zhuyabing for windows
#else
  typedef map<string,void* > LibMap;
  typedef pair<string,void* > PairLibMap;

#endif

typedef map<string,int> NameMap;
typedef pair<string,int > PairNameMap;

typedef int (*recvFromBus_X_def)(int destKey, char *msg, int len, int srcKey);

typedef int (*GenerateMainBus)(recvFromBus_X_def fun);
typedef int (*send2Bus_Target)(int main_handle,int destKey, char *msg, int len,int srcKey);
typedef void (*register2Bus_Target)(int main_handle,int key);
typedef void (*unRegister2Bus_Target)(int main_handle,int key);



class /*COMMONFRAMEMAINSHARED_EXPORT*/ CommonFrameMain/*:public CommonBus 2018-01�����ƽ̨�����پ�̬���ض�̬�⣬������ֹ���·������*/
{

public:
	int send2Bus(int destKey,char *msg,int len,int srcKey=0);
private:
	void loadCommonBus();
	void register2Bus(int key);
    void unRegister2Bus(int key);
    
	send2Bus_Target m_send_core;
	register2Bus_Target m_register_core;
	unRegister2Bus_Target m_unRegister_core;
	int m_bushandle_core;
public:
	CRITICAL_SECTION dllMutex;
	struct NetPayInit1
	{
		UINT nThreadID;
		HWND hWnd;
		unsigned int nNotifyMsgID;
		std::string szAreaInfo;
		std::string szLoaclStationID;
		std::string szLoaclLaneID;
		std::string szServerInfo;
		int iProvinceID;
	};
	struct NetPayInit2
	{
		UINT nThreadID;
		HWND hWnd;
		std::string szAreaInfo;
		std::string szLoaclStation;
		std::string szLoaclLaneID;
		std::string szServerInfo;
		int iProvinceID;
		void (WINAPI*fun)(int option, int result);
	};

	struct NetPayInit3
	{
		UINT nThreadID;
		HWND hWnd;
		std::string szAreaInfo;
		std::string szLoaclStation;
		std::string szLoaclLaneID;
		std::string szServerInfo;
		int iProvinceID;
		void* pIndPtr;
		void (WINAPI*fun)(int option, int result, void* pIndPtr);
	};

	//add by pangtw 2018-09-19
	//��������۷ѵȴ���ʱʱ�䡢���۲�ѯ��ʱʱ��		�㶫��Ͷ��Ŀ����
	struct NetPayInit4
	{
		UINT nThreadID;
		HWND hWnd;
		unsigned int nNotifyMsgID;
		std::string szAreaInfo;
		std::string szLoaclStationID;
		std::string szLoaclLaneID;
		std::string szServerInfo;
		int iProvinceID;
		int iOverTime;
		int iTimeOut;
	};


	//�����ʼ������
	struct EventInit
	{
		UINT nThreadID;
		HWND hWnd;
		unsigned int nNotifyMsgID;
		std::string szAreaInfo;
		std::string szLoaclStation;
		std::string szLoaclLaneID;
		std::string szServerInfo;
		int iProvinceID;
		void* pIndPtr;
		void (*fun)(int option, int result, void* pIndPtr);
	};


	std::string moduleDir;
	int  initNumber;
	int  initEventNumber;
	NetPayInit1 mInitInfo1;
	NetPayInit2 mInitInfo2;
	NetPayInit3 mInitInfo3;
	NetPayInit4 mInitInfo4;
	EventInit	m_InitEventInfo;
	std::string updCode;
	void InitNetPay();
	std::set<std::string> ParseConfigDll(const std::string& configPath);
public:
    static CommonFrameMain* instance();
    static CommonFrameMain* self;
    virtual ~CommonFrameMain();
    int recvFromBus(int destKey,char *msg,int len, int srcKey=0); 
    /*���ض�̬��
     * filePath:Ϊ��̬���ļ���·������������̬�Ȿ������
     */
    void loadLib(string filePath,bool& load_flag);
   /**
    ж�ض�̬�⡣
    fileName��ֻ�Ƕ�̬�Ȿ��������
    */
    void unLoadLib(string fileName);

    void createThreadEx();

	static unsigned int  LoadDllSelf(void* arg);
	void LoadDllThread();
#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
    static int threadFun(void *);
#else
    static void *threadFun(void *);
#endif
    void updateDll(string fileDllName);
    void moveFile2Lib(string fileName);
    void moveFile2loadLib(string fileName);


    void tryUpdateDLL();

     LibMap libMap;

     FrameConfigFileOperate myFrameConfigFileOperate;
     ST_FRAME_CONFIG_INFO myST_FRAME_CONFIG_INFO;

private:
	
private:
    CommonFrameMain(); 
//    LibMap libMap;
    NameMap nameMap;
    string filePath;

    list<string> allDllList;
    list<string> updateDllList;
	std::string m_thisModulePath;

    bool isStateFlag;

    void readFileName();
    void* linuxLib;

	//��ʶ���°����Ƿ���ڿ�ܿ�
	bool mbHaveFramDll;
	void deleteOldDllBakFile();

	
#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
    HINSTANCE hDLL;
#endif
};




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

//extern "C" COMMONFRAMEMAINSHARED_EXPORT CommonBus* getInstance(); //��ȡ��TestDll�Ķ���
//extern "C" COMMONFRAMEMAINSHARED_EXPORT CommonBus* deleteInstance(); //��ȡ��TestDll�Ķ���




///////////////////////////API    Interface////////////////////////////////////////////////////////

//��ʼ���������		    -- �ӿ�1
/******************************************
 *�����б�
        1.���ں�
        2.���ڲ�����
        3.������������ַ��������
        4.���������Ӷ˿ڣ�Ĭ��80����������Ϊ8081��
        5.�������Ƿ񿪷Ŷ˿ڣ�Ĭ��0����������Ϊ1��
        6.�Ƿ������־��ǣ�0��1����Ĭ��Ϊ1
        7.��־�������������ͣ�Ĭ��20
        8.������Ϣ
        9.����վ����Ϣ
        10.���س�������Ϣ
        11.���ط�������Ϣ
        12.ʡ�ݱ���
        13.�ص���������
* ***************************************/
/*
extern "C" COMMONFRAMEMAINSHARED_EXPORT   bool WINAPI IF_InitEnvironment (IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID);
extern "C" COMMONFRAMEMAINSHARED_EXPORT   bool WINAPI IF_InitEnvironment2 (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, void ( *fun)(int option, int result));
extern "C" COMMONFRAMEMAINSHARED_EXPORT   bool WINAPI IF_InitEnvironment3 (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void ( *fun)(int option, int result, void* pIndPtr));


//������Դ			-- �ӿ�2
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_Destroy(void);
//��ȡ��һ�δ�����Ϣ		-- �ӿ�3
extern "C" COMMONFRAMEMAINSHARED_EXPORT  const char*  WINAPI IF_GetLastErrorDesc(void);
//������ȡ�û���Ϣ����		-- �ӿ�4
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_StartGetAccountInfo( const int& iOvertime);
//ֹͣ��ȡ�û���Ϣ����		-- �ӿ�5
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_StopGetAccountInfo(void);
//��ȡ�û���Ϣ				-- �ӿ�6
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_GetAccountInfo(char* szParamContext, int& iParamSize,const int& iFormatType);
//ִ�пۿ����				-- �ӿ�7
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_DebitMoney( const char* szParamContext, const int& iParamSize, const int& iFormatType);
//ִ�г�������				-- �ӿ�8
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_DebitCancel( const char* szParamContext, const int& iParamSize, const int& iFormatType);
//��ȡ�ۿ���				-- �ӿ�9
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_GetDebitResult(char* szParamContext, int& iParamSize, const int& iFormatType);
//���ý�������				-- �ӿ�10
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_SetMMI(const char* szParamContext, const int& iParamSize, const int& iFormatType);
//��ȡӲ������				-- �ӿ�11
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_GetParam( const char* szParamContext, const int& iParamSize, const int& iFormatType, char* szResultContext, int& iResultSize);
//������״̬				-- �ӿ�12
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_GetComponentStatus( unsigned int& uStatus);
//ִ�д���ؼ���Ϣ���ݲ���                  -- �ӿ�13
extern "C" COMMONFRAMEMAINSHARED_EXPORT  bool  WINAPI IF_TranslateData( const char* szParamContext, const int& iParamSize, const int& iFormatType);
*/

//////////////////////////////////////////////////////////////////////////////////////////////////







//Ϊ����ҵ��̬�� �����Ľӿ�ָ��

//��������
//��ʼ��Ӳ��
//
typedef bool  (WINAPI *CommonFrame_InitEnvironment) (IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID);
typedef bool  (WINAPI *CommonFrame_InitEnvironment2) (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, void (WINAPI *fun)(int option, int result));
typedef bool  (WINAPI *CommonFrame_InitEnvironment3) (IN const UINT& nThreadID, IN const HWND& hWnd, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr, void (WINAPI *fun)(int option, int result, void* pIndPtr));

//add by pangtw 2018-09-19
//��������۷ѵȴ���ʱʱ��		�㶫��Ͷ��Ŀ����
typedef bool  (WINAPI *CommonFrame_InitEnvironment4) (IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, OUT int& iOverTime, OUT int& iTimeOut);


//������Դ			-- �ӿ�2
typedef bool  (WINAPI *CommonFrame_Destroy)(void);
//��ȡ��һ�δ�����Ϣ
typedef char*       ( WINAPI *CommonFrame_GetLastErrorDesc) (void);
//������ȡ�û���Ϣ����		-- �ӿ�4
typedef bool  (WINAPI *CommonFrame_StartGetAccountInfo) ( const int& iOvertime);
//ֹͣ��ȡ�û���Ϣ����		-- �ӿ�5
typedef bool  (WINAPI *CommonFrame_StopGetAccountInfo) (void);
//��ȡ�û���Ϣ				-- �ӿ�6
typedef bool  (WINAPI *CommonFrame_GetAccountInfo) (char* szParamContext, int& iParamSize,const int& iFormatType);
//ִ�пۿ����				-- �ӿ�7
typedef bool	(  WINAPI *CommonFrame_DebitMoney) ( const char* szParamContext, const int& iParamSize, const int& iFormatType);
//ִ�г�������				-- �ӿ�8
typedef bool	( WINAPI *CommonFrame_DebitCancel) ( const char* szParamContext, const int& iParamSize, const int& iFormatType);
//ִ�г�������				-- �ӿ�8-2		--��Ͷ��Ŀ��������
typedef bool	( WINAPI *CommonFrame_DebitCancel2) (IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//��ȡ�ۿ���				-- �ӿ�9
typedef bool	( WINAPI *CommonFrame_GetDebitResult) (char* szParamContext, int& iParamSize, const int& iFormatType);
//���ý�������				-- �ӿ�10
typedef bool	( WINAPI *CommonFrame_SetMMI) (const char* szParamContext, const int& iParamSize, const int& iFormatType);
//��ȡӲ������				-- �ӿ�11
typedef bool	( WINAPI *CommonFrame_GetParam) ( const char* szParamContext, const int& iParamSize, const int& iFormatType, char* szResultContext, int& iResultSize);
//������״̬				-- �ӿ�12
typedef bool	( WINAPI *CommonFrame_GetComponentStatus) ( unsigned int& uStatus);
//ִ�д���ؼ���Ϣ���ݲ���                  -- �ӿ�13
typedef bool (WINAPI *CommonFrame_TranslateData) ( const char* szParamContext, const int& iParamSize, const int& iFormatType);
//��ȡ�������				-- �ӿ�14
typedef bool	( WINAPI *CommonFrame_GetCancelResult) (char* szParamContext, int& iParamSize, const int& iFormatType);

typedef bool (WINAPI *CommonFrame_TransferImage)(IN const char* szImageInfo, IN const int& iSize, IN const int& iFormatType);

typedef bool (WINAPI *CommonFrame_AgencyDebitQuery)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT int& iResult);


//modify 2018-07
typedef int  (WINAPI* CommonFrame_ITCEntryTrade)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout);

typedef int  (WINAPI* CommonFrame_ITCEntryQuery)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout,IN bool bSync);

typedef int  (WINAPI* CommonFrame_ITCExitQuery) (IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szEntryInfo, IN OUT int& iEntryInfoSize, IN int iTimeout,IN bool bSync);


typedef int  (WINAPI* CommonFrame_ITCExitTrade)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout);
typedef int  (WINAPI* CommonFrame_ITCQueryPath) (IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout);
typedef bool (WINAPI* CommonFrame_GetServerTime)(IN OUT unsigned int& nServerTime, IN const int& iLimitTime);

// typedef int (WINAPI* CommonFrame_ITCEntryTrade)(IN OUT char* szParamContext, IN OUT int& iParamSize, IN const int& iFormatType, OUT char szResponse[2048], IN int iTimeout);
// typedef int (WINAPI* CommonFrame_ITCExitPreTrade) (IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, OUT char szEntryInfo[2048], IN int iTimeout);
// typedef int (WINAPI* CommonFrame_ITCExitTrade) (IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, OUT char szResponse[2048], IN int iTimeout);

typedef bool(WINAPI* CommonFrame_CompletionTicketNo)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);

bool WINAPI IF_Destroy(void);

//add 2018-05
typedef bool (WINAPI* CommonFrame_SetVehicleStatus)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
typedef bool (WINAPI* CommonFrame_PrinterTicket)(IN  const  char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
typedef bool (WINAPI* CommonFrame_ReadCardInfo)(IN OUT char* szParamContext,IN OUT int& iParamSize,IN const int& iFormatType);
typedef bool (WINAPI* CommonFrame_WriteCardInfo)(IN OUT char* szParamContext,IN OUT int& iParamSize,IN const int& iFormatType);



typedef bool (WINAPI* CommonFrame_QueryVirtualCardDebit)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN int& iSupportAgent, OUT char* szResult);

typedef bool (WINAPI* CommonFrame_TMP_SetUnipayEncrypt)(int setType,const char *setData,char* retData);

//add by pangtw 2018-12-06
//ִ�к˵�����
typedef bool  (WINAPI  *CommonFrame_BeginAudit)(IN const char* szPayIdentifier,IN OUT int& iOverTime);
//��ȡ�˵��������
typedef bool  (WINAPI  *CommonFrame_GetAuditResult)(IN OUT char* szPayIdentifier,IN OUT int& iResult);


typedef int (WINAPI *CommonFrame_QueryAgentDebitEtcUp)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN int& iSupportAgent, IN const int& iTimeout);
typedef int (WINAPI *CommonFrame_QueryAgentDebitResultEtcUp)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout);
typedef int (WINAPI *CommonFrame_DebitMoneyEtcUp)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType, IN OUT char* szResultContext, IN OUT int& iResultSize, IN int iTimeout);


/****************************************************EtcEvent�ӿ�*******************************************/
//��ʼ���������
typedef bool (WINAPI *CommonFrame_IF_EventInitEnvironment3)(IN const UINT& nThreadID, IN const HWND& hWnd, IN const unsigned int& nNotifyMsgID, IN const char* szAreaInfo, IN const char* szLoaclStation, IN const char* szLoaclLaneID, IN const char* szServerInfo, IN const int& iProvinceID, IN OUT void* pIndPtr,void (*fun)(int option, int result,void* pIndPtr));
//������Դ
typedef bool (WINAPI *CommonFrame_IF_EventDestroy)(void);
//��ȡ��һ�δ�����Ϣ
typedef const char* (WINAPI *CommonFrame_EventGetLastErrorDesc)(void);
//��ȡ���״̬
typedef bool (WINAPI *CommonFrame_EventGetComponentStatus)(IN OUT unsigned int& uStatus);
//���鴦����ʼ
typedef bool (WINAPI *CommonFrame_EventDealStart)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//��������ȷ��
typedef bool (WINAPI *CommonFrame_EventCheckVehQueue)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//������Ϣȷ��
typedef bool (WINAPI *CommonFrame_EventCheckVehInfo)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//�����Ϣȷ��
typedef bool (WINAPI *CommonFrame_EventCheckEntryInfo)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//�Ѷ���ʾ
typedef bool (WINAPI *CommonFrame_EventShowFeeInfo)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//֧��״̬��ʾ
typedef bool (WINAPI *CommonFrame_EventPayResultDisplay)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//���鴦������
typedef bool (WINAPI *CommonFrame_EventDealStop)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//����ɨ��
typedef bool (WINAPI *CommonFrame_EventStartScan)(IN const int& iFeeValue);
//ֹͣɨ��
typedef bool (WINAPI *CommonFrame_EventStopScan)();
//ETC��������ʾ
typedef bool (WINAPI *CommonFrame_EventCardOperationNotify)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//���������޸�
typedef bool (WINAPI *CommonFrame_EventModifyVehQueue)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//��С�Ѷ���Ȩ
typedef bool (WINAPI *CommonFrame_EventFeeAuthorize)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//������������
typedef bool (WINAPI *CommonFrame_EventFreeVoicePlay)(IN const char* szParamContext, IN const int& iParamSize, IN const int& iFormatType);
//����ҵ����Ȩ
typedef bool (WINAPI *CommonFrame_EventAuthorize)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);
//ɾ���������н��
typedef bool (WINAPI *CommonFrame_EventDelVehQueueResult)(IN const char* szParamContext,IN const int& iParamSize,IN const int& iFormatType);

typedef bool (WINAPI *CommonFrame_TransferCashInvoiceData)(const char* szParamContext, const int& iParamSize, const int& iFormatType);
typedef bool (WINAPI *CommonFrame_CancelCashInvoiceData)(const char* szParamContext, const int& iParamSize, const int& iFormatType);

#endif // COMMONFRAMEMAIN_H