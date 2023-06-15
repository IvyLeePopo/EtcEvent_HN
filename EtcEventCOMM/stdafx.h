// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果必须将位于下面指定平台之前的平台作为目标，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT


/*******************************************************消息定义********************************************************************/

//定义各个线程消息的起始索引
#define	MSG_START_ID_SQL		0		//SQL线程处理消息起始索引号
#define	MSG_START_ID_SM			100		//扫码线程处理消息起始索引号
#define	MSG_START_ID_IC			200		//读写器线程处理消息起始索引号
#define	MSG_START_ID_EQUIPEMENT	300		//IO外设线程处理消息起始索引号
#define MSG_START_ID_HTTP		400		//Http通讯线程处理消息起始索引号
#define MSG_START_ID_PLATE		500		//车牌识别仪线程消息起始索引号
#define MSG_START_ID_VDM		600		//VDM处理线程消息起始索引号
#define MSG_START_ID_TFI		700		//费显处理线程消息起始索引号
#define MSG_START_ID_REMOTE		800		//远程调图类消息起始索引号
#define MSG_START_ID_AXIS		900		//计重线程消息起始索引号
#define MSG_START_ID_SOCKET		1000	//Socket通讯相关消息起始索引
#define MSG_START_ID_DM			1100	//核心数据管理线程相关消息起始索引
#define MSG_START_ID_MAIN_UI	1200	//主界面相关消息起始索引号


//Http线程类处理的消息
#define WM_THREAD_HTTP_SEND_DATA_REQUEST			WM_APP + MSG_START_ID_HTTP + 1	//发送查询请求消息定义
#define WM_WND_DEAL_PLATFORM_RESPONSE_RESULT		WM_APP + MSG_START_ID_HTTP + 2	//回馈窗体Http平台服务器处理结果
#define WM_THREAD_HTTP_RESEND_DATA_REQUEST			WM_APP + MSG_START_ID_HTTP + 3	//重发数据


//运营平台通讯类处理的消息
#define	WM_WND_DEAL_RESPONSE_FOR_ENTRY_QUERY		WM_APP + MSG_START_ID_HTTP + 3	//从HTTP线程接收到【入口查询】的回馈结果
#define	WM_WND_DEAL_RESPONSE_FOR_EXIT_QUERY			WM_APP + MSG_START_ID_HTTP + 4	//从HTTP线程接收到【出口查询】的回馈结果
#define WM_WND_DEAL_RESPONSE_FOR_OPEN_LOOP			WM_APP + MSG_START_ID_HTTP + 5	//从HTTP线程接收到【入口开环操作】的回馈结果
#define WM_WND_DEAL_RESPONSE_FOR_CLOSE_LOOP			WM_APP + MSG_START_ID_HTTP + 6	//从HTTP线程接收到【出口闭环操作】的回馈结果
#define WM_WND_DEAL_RESPONSE_FOR_DEBIT				WM_APP + MSG_START_ID_HTTP + 7	//从HTTP线程接收到【扣费操作】的回馈结果
#define WM_WND_DEAL_RESPONSE_FOR_DEBIT_CHECK		WM_APP + MSG_START_ID_HTTP + 8	//从HTTP线程接收到【扣费状态查询】的回馈结果
#define WM_WND_DEAL_RESPONSE_FOR_PASS_OPEN_STATION	WM_APP + MSG_START_ID_HTTP + 9	//从HTTP线程接收到【过开放式收费站】的回馈结果


//扫码器控制相关消息
#define WM_THREAD_SM_INIT_WORK							WM_APP + MSG_START_ID_SM + 25	//初始化所有的工作线程

#define WM_THREAD_SM_GET_SCAN_CODE_DATA					WM_APP + MSG_START_ID_SM + 1	//获取到扫码结果
#define WM_THREAD_SM_GET_SYSTEM_INFO					WM_APP + MSG_START_ID_SM + 2	//获取扫码器系统信息
#define WM_THREAD_SM_START_SCAN							WM_APP + MSG_START_ID_SM + 3	//强制开始扫码
#define WM_THREAD_SM_STOP_SCAN							WM_APP + MSG_START_ID_SM + 4	//初始化所有的工作线程
#define WM_THREAD_SM_RESET_MACHINE						WM_APP + MSG_START_ID_SM + 5	//初始化指定扫码器
#define WM_THREAD_SM_DEAL_INNER_DEVICE_RESPONSE_INFO	WM_APP + MSG_START_ID_SM + 6	//协议控制类发送给管理线程关于扫码状态提示信息
#define WM_THREAD_SM_CONTROL_BOARD						WM_APP + MSG_START_ID_SM + 7	//控制串口板
#define WM_THREAD_SM_STATE								WM_APP + MSG_START_ID_SM + 8	//管理线程发送给UI关于扫码器的状态提示信息
#define WM_THREAD_SM_SET_LIGHT_MODE						WM_APP + MSG_START_ID_SM + 9	//控制灯光情况

#define WM_WND_NET_SM_DEBIT_TOLL						WM_APP + MSG_START_ID_SM + 10	//仅仅扣款
#define WM_WND_CHECK_DEBIT_PASSWORD_STATE				WM_APP + MSG_START_ID_SM + 11	//扣款结果查询

//新增加设置显示消息
#define WM_THREAD_SM_SET_DISPLAY						WM_APP + MSG_START_ID_SM + 12	//设置显示内容
#define WM_THREAD_SM_SET_TRANSMISSION					WM_APP + MSG_START_ID_SM + 13	//透传
#define WM_THREAD_SM_TRANSMISSION_RESPONSE				WM_APP + MSG_START_ID_SM + 14	//透传回馈

#define WM_THREAD_SM_SET_ZHEJIANG_TEXT					WM_APP + MSG_START_ID_SM + 20	//设置浙江文本
#define WM_THREAD_SM_SET_ZHEJIANG_SOUND					WM_APP + MSG_START_ID_SM + 21	//设置浙江声音

#define WM_THREAD_SM_NOTIFY_CALLER						WM_APP + MSG_START_ID_SM + 22	//回调函数通知组件调用者



#define FILE_SIZE_LOG_MAX				50*1024*1024			//日志文件最大值
#define DIR_NAME_DEVICE_SCAN			_T("ScanMachine")		//扫码器

#define FILE_NAME_DLL_SCAN				_T("HTSM.dll")			//扫码器控制动态库
#define FILE_NAME_HTTP_CONFIG			_T("ReflectWeb.ini")	//反射服务器配置信息文件名

#define FILE_NAME_DEVICE_SM_CONFIG		_T("HTSMConfig.ini")		//硬件配置信息文件名--扫码器
#define FILE_NAME_DEVICE_SM_CONFIG1		_T("HTSMConfig1.ini")		//硬件配置信息文件名--扫码器


#pragma pack(1)



//车道配置信息结构体
typedef struct _tagLaneConfigInfo
{
	int		MainType;		//封闭式入出口、开放式
	int		SubType;		//ETC,MTC,自动发卡,便携收费机
	int		StationID;		//站点编码
	int		LaneID;			//车道编码
	int		LaneType;		//车道类型(1,入口;2,出口)
	TCHAR	LaneTab[16];	//车道唯一索引
	TCHAR	StationTab[32]; //站点唯一索引
	int		STServerTpye;	//站级服务器类型
	TCHAR	STServerIP[24];	//站级服务器IP地址
	TCHAR	STServerID[24];	//站级服务器ID编码
	TCHAR	WebServerIP[24];//内网反射服务器IP地址
}tagLaneConfigInfo;

//车道APP类向各个线程类传递基本配置信息的结构体
typedef struct _tagTranstParam
{
	//省份编码
	int					ProvinceID;
	//车道配置基本信息
	tagLaneConfigInfo	LaneConfigInfo;
	//特殊主动触发消息ID
	DWORD				ActiveNotifyMsgID;
	/******************各种路径集合******************/

	//车道初始目录绝对路径,默认为运行程序的当前目录
	TCHAR	PathCommDir[MAX_PATH];
	//生产过程产生的临时关键文件存储目录绝对路径
	TCHAR	PathDataDir[MAX_PATH];
	//车道硬件驱动目录绝对路径
	TCHAR	PathDeviceDir[MAX_PATH];
	//车道日志目录绝对路径
	TCHAR	PathLogDir[MAX_PATH];
	//车道数据备份目录绝对路径
	TCHAR	PathDataBackupDir[MAX_PATH];
	//临时文件存储目录绝对路径
	TCHAR	PathTempDir[MAX_PATH];
	//车道下载参数目录绝对路径
	TCHAR	PathDownloadDir[MAX_PATH];

	//核心数据二级目录（车道号）
	TCHAR	PathDataSecondDirForLaneID[MAX_PATH];
	//核心数据二级目录（数据）
	TCHAR	PathDataSecondDirForCore[MAX_PATH];
	//核心数据二级目录（参数）
	TCHAR	PathDataSecondDirForParam[MAX_PATH];
	//核心数据二级目录（图像）
	TCHAR   PathDataSecondDirForImage[MAX_PATH];
}tagTranstParam;


//串口参数信息结构体
typedef struct _tagCOMParam
{
	int		Port;					//串口
	int		BaudRate;				//波特率
	int		ParityBit;				//校验位
	int		StopBit;				//停止位
	int		DataBit;				//数据位

	DWORD	InBufferSize;			//输入缓冲区大小
	DWORD	OutBufferSize;			//输出缓冲区大小
}tagCOMParam;


#ifdef _DEBUG
#pragma comment(lib,".\\lib\\json_vc71_libmtd.lib")
#else
#pragma comment(lib,".\\lib\\json_vc71_libmt.lib")
#endif

#pragma pack()