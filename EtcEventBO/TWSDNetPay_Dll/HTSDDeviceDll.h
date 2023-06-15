/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�HTSDDeviceDll.h
 * ժ    Ҫ������Ӳ���ӿڶ�̬������࣬�����ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��7��24��
 */
#pragma once

class CHTSDDeviceDll
{
//��������
public:
	//��ʼ��Ӳ��
	//typedef BOOL	(WINAPI  *defIF_InitDev)(IN const UINT& nThreadID,IN const HWND& hWnd,IN const UINT& nNotifyMsgID,IN const int& iProvinceID,IN const int& iOverTime);
	//������Դ
	typedef BOOL	(WINAPI  *defIF_Destroy)(void);
	//��ȡ��һ�δ�����Ϣ
	//typedef CString (WINAPI  *defIF_GetLastErrorDesc)(void);
	//ִ��������ȡ���ݲ���
	typedef BOOL	(WINAPI  *defIF_StartGetData)(IN const int& iOvertime);
	//ִ��ֹͣ��ȡ���ݲ���
	typedef BOOL	(WINAPI  *defIF_StopGetData)(IN const int& iChangeMMI);
	//ִ�м��Ӳ��״̬����
	typedef BOOL	(WINAPI  *defIF_CheckDeviceStatus)(IN OUT DWORD& dwState);
	//��ȡ���ݽ��
	//typedef BOOL	(WINAPI  *defIF_GetData)(IN OUT CString& strData,IN OUT int& iDataType,IN OUT int& iSrcType);
	//Ȩ����֤����
	typedef	BOOL	(WINAPI  *defIF_Authority)(IN const CString& strData);
	//��ȡ�̼��汾��Ϣ
	typedef	BOOL	(WINAPI  *defIF_GetDeviceFirewareVer)(IN OUT CString& strXML);
	//��ȡ��ʾ������Ϣ
	typedef	BOOL	(WINAPI  *defIF_GetDisplayContext)(IN OUT int&, CString&);
	//������ʾ������Ϣ
	typedef	BOOL	(WINAPI  *defIF_SetDisplayContext)(IN const int&,IN const CString& strXML);
	//ִ�������·�����
	typedef BOOL	(WINAPI  *defIF_IssuedData)(IN const CString& strXML);
	//ִ�п���IO����
	typedef BOOL	(WINAPI  *defIF_SetIOState)(IN const int& iIOState);


	//add 2017-12
	typedef BOOL	(WINAPI  *defIF_UpgradeFirware)(IN const CString& strJson);
	typedef BOOL	(WINAPI  *defIF_IsUpgrading)();
	typedef	BOOL	(WINAPI  *defIF_RealFirmwareVer)(IN OUT CString& strXML);
	typedef	BOOL	(WINAPI  *defIF_RealSlaveFirmwareVer)(IN OUT CString& strXML);

	//add 2018-07
	typedef BOOL (WINAPI* defIF_QueryLEDVersion)(OUT CString& strJson);

	typedef BOOL (WINAPI* defIF_UserDefinedData)(const  CString& strJson);

	/***********************************�Խ�ETC���鴦��Ӳ����***********************************************************************************/
	//��ʼ���豸
	typedef bool (WINAPI  *defIF_InitDev)(IN const UINT& dwThreadNotifyCallerID, IN const UINT& dwThreadManagerID, IN const HWND& hWnd, IN const UINT& dwEtcRespMsgID, IN const UINT& dwDevNotifyMsgID, IN OUT void* pIndPtr);
	//�����豸��ʼ������
	typedef bool (WINAPI  *defIF_SetDevInitData)(IN const char* szParamContext, IN const int& iParamSize);	
	//������Դ
	typedef bool (WINAPI  *defIF_DeInitDev)(void);
	//��ȡ��һ�δ�����Ϣ
	typedef const char* (WINAPI  *defIF_GetLastErrorDesc)(void);
	//��ȡ���״̬
	typedef bool (WINAPI  *defIF_GetDevStatus)(IN OUT unsigned int& uStatus);
	//���鴦��ʼ
	typedef bool (WINAPI  *defIF_EtcEventDealStart)(IN const char* szParamContext, IN const int& iParamSize);
	//��������ȷ��
	typedef bool (WINAPI  *defIF_CheckVehQueue)(IN const char* szParamContext, IN const int& iParamSize);
	//������Ϣȷ��
	typedef bool (WINAPI  *defIF_CheckVehInfo)(IN const char* szParamContext,IN const int& iParamSize);
	//�����Ϣȷ��
	typedef bool (WINAPI  *defIF_CheckEntryInfo)(IN const char* szParamContext,IN const int& iParamSize);
	//�Ѷ���ʾ
	typedef bool (WINAPI  *defIF_ShowFeeInfo)(IN const char* szParamContext,IN const int& iParamSize);
	//֧��״̬��ʾ
	typedef bool (WINAPI  *defIF_PayResultDisplay)(IN const char* szParamContext,IN const int& iParamSize);
	//���鴦�����
	typedef bool (WINAPI  *defIF_EtcEventDealStop)(IN const char* szParamContext,IN const int& iParamSize);
	//����ɨ��
	typedef bool (WINAPI  *defIF_StartScan)(IN const int& iFeeValue);
	//ֹͣɨ��
	typedef bool (WINAPI  *defIF_StopScan)();
	//ETC��������ʾ
	typedef bool (WINAPI  *defIF_CardOperationNotify)(IN const char* szParamContext, IN const int& iParamSize);
	//���������޸�
	typedef bool (WINAPI  *defIF_ModifyVehQueue)(IN const char* szParamContext, IN const int& iParamSize);
	//��ȡ��ά������
	typedef bool (WINAPI *defIF_GetData)(IN OUT CString& strData, IN OUT int& iDataType, IN OUT int& iSrcType);
	//�Ѷ���Ȩ
	typedef bool (WINAPI *defIF_FeeAuthorize)(IN const char* szParamContext, IN const int& iParamSize);
	//������������
	typedef bool (WINAPI *defIF_FreeVoicePlay)(IN const char* szParamContext, IN const int& iParamSize);
	//����ҵ����Ȩ
	typedef bool (WINAPI * defIF_EventAuthorize)(IN const char* szParamContext, IN const int& iParamSize);
	//ɾ���������н��
	typedef bool (WINAPI * defIF_DelVehQueueResult)(IN const char* szParamContext, IN const int& iParamSize);
	//��ά����ʾ�ӿڶ���
	typedef bool (WINAPI *defIF_Tr500QRcodeDisplay)(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//�Ѷ���ʾ�ӿڶ���
	typedef bool (WINAPI *defIF_Tr500FeeDisplay)(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//֧�������ʾ�ӿڶ���
	typedef bool (WINAPI *defIF_Tr500PayResultDisplay)(IN const char* strJsonParam, IN const int& iJsonParamSize);
	//�����ӿڶ���
	typedef bool (WINAPI *defIF_Tr500ClearScreen)(IN const char* strJsonParam, IN const int& iJsonParamSize);

public:
	CHTSDDeviceDll(void);
	~CHTSDDeviceDll(void);
public:
	//��ʼ����̬��
	const BOOL Init(IN const CString& strDllFilePath);
	//��ȡ��̬�����״̬
	inline const BOOL GetDllInitState(void){return m_bInitDll;}
	//��ȡ�����ʼ��״̬
	inline const BOOL GetDevInitState(void){return m_bInitDev;}
//Operations
public:
	//�����ʼ��
	BOOL InitDev(IN const UINT& nThreadID,IN const HWND& hWnd,IN const unsigned int& nNotifyMsgID,IN const int& iProvinceID,IN const int& iOverTime);
	//������Դ
	BOOL Destroy(void);
	//��ȡ��һ�δ�����Ϣ
	CString GetLastErrorDesc(void);
	//ִ��������ȡ���ݲ���
	BOOL StartGetData(IN const int& iOvertime);
	//ִ��ֹͣ��ȡ���ݲ���
	BOOL StopGetData(IN const int& iChangeMMI = 0);
	//ִ�м��Ӳ��״̬����
	BOOL CheckDeviceStatus(IN OUT DWORD& dwState);
	//��ȡ���ݽ��
	BOOL GetData(IN OUT CString& strData,IN OUT int& iDataType,IN OUT int& iSrcType);
	//Ȩ����֤����
	BOOL Authority(IN const CString& strData);
	//��ȡ�̼��汾��Ϣ
	BOOL GetDeviceFirewareVer(IN OUT CString& strXML);
	//��ȡ��ʾ������Ϣ
	BOOL GetDisplayContext(IN OUT int& iID, CString& strXML);
	//������ʾ������Ϣ
	BOOL SetDisplayContext(IN const int& iID,IN const CString& strXML);
	//ִ�������·�����
	BOOL IssuedData(IN const CString& strXML);
	//ִ�п���IO����
	BOOL SetIOState(IN const int& iIOState);


	BOOL UpgradeFirware(const CString& strJson);
	BOOL IsUpgrading();

	BOOL RealFirmwareVer(IN OUT CString& strXML);

	BOOL RealSlaveFirmwareVer(IN OUT CString& strXML);
	BOOL QueryLEDVersion(OUT CString& strJson);
	BOOL TransferUserDefinedData(const CString& strJson);

	BOOL QRcodeDisplay(const CString& strJson);
	BOOL FeeDisplay(const CString& strJson);
	BOOL PayResultDisplay(const CString& strJson);
	BOOL ClearScreen(const CString& strJson);

//Attributes
private:
	//��̬����ؾ��
	HMODULE	m_hModule;
	//��̬���ʼ�����
	BOOL	m_bInitDll;

	//����˵��
	CString m_strLastError;
	//�����������
	/*
	defIF_InitDev				fnIF_InitDev;
	defIF_Destroy				fnIF_Destroy;
	defIF_GetLastErrorDesc		fnIF_GetLastErrorDesc;
	defIF_StartGetData			fnIF_StartGetData;
	defIF_StopGetData			fnIF_StopGetData;
	defIF_CheckDeviceStatus		fnIF_CheckDeviceStatus;
	defIF_GetData				fnIF_GetData;
	defIF_Authority				fnIF_Authority;
	defIF_GetDeviceFirewareVer	fnIF_GetDeviceFirewareVer;
	defIF_GetDisplayContext		fnIF_GetDisplayContext;
	defIF_SetDisplayContext		fnIF_SetDisplayContext;
	defIF_IssuedData			fnIF_IssuedData;
	defIF_SetIOState			fnIF_SetIOState;

	defIF_UpgradeFirware        fnIF_UpgradeFirware;
	defIF_IsUpgrading           fnIF_IsUpgrading;

	defIF_RealFirmwareVer       fnIF_RealFirmwareVer;
	defIF_RealSlaveFirmwareVer  fnIF_RealSlaveFirmwareVer;
	defIF_QueryLEDVersion       fnIF_QueryLEDVersion;
	defIF_UserDefinedData       fnIF_UserDefinedData;
*/
public:
	defIF_InitDev				fnIF_InitDev;
	defIF_SetDevInitData		fnIF_SetDevInitData;
	defIF_DeInitDev				fnIF_DeInitDev;
	defIF_GetLastErrorDesc      fnIF_GetLastErrorDesc;
	defIF_GetDevStatus			fnIF_GetDevStatus;
	defIF_EtcEventDealStart		fnIF_EtcEventDealStart;
	defIF_CheckVehQueue			fnIF_CheckVehQueue;
	defIF_CheckVehInfo			fnIF_CheckVehInfo;
	defIF_CheckEntryInfo		fnIF_CheckEntryInfo;
	defIF_ShowFeeInfo			fnIF_ShowFeeInfo;
	defIF_PayResultDisplay		fnIF_PayResultDisplay;
	defIF_EtcEventDealStop		fnIF_EtcEventDealStop;
	defIF_StartScan				fnIF_StartScan;
	defIF_StopScan				fnIF_StopScan;
	defIF_CardOperationNotify   fnIF_CardOperationNotify;
	defIF_ModifyVehQueue		fnIF_ModifyVehQueue;
	defIF_GetData				fnIF_GetData;
	defIF_FeeAuthorize			fnIF_FeeAuthorize;
	defIF_FreeVoicePlay			fnIF_FreeVoicePlay;
	defIF_EventAuthorize		fnIF_EventAuthorize;
	defIF_DelVehQueueResult		fnIF_DelVehQueueResult;

	defIF_Tr500QRcodeDisplay	fnIF_Tr500QRcodeDisplay;
	defIF_Tr500FeeDisplay		fnIF_Tr500FeeDisplay;
	defIF_Tr500PayResultDisplay	fnIF_Tr500PayResultDisplay;
	defIF_Tr500ClearScreen		fnIF_Tr500ClearScreen;

	//�����ʼ�����
	BOOL	m_bInitDev;

};
