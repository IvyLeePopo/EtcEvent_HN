#ifndef STATE_DEFINE_H
#define STATE_DEFINE_H

#define  MTC_NO_ERROR                       0      //正常 
#define  MTC_SCAN_PROCESSING            1       //扫码正在处理     
#define  MTC_PORTOPEN_ERR               -1      //打开串口失败 
#define  MTC_PORTCLOSE_ERR              -2      //关闭串口失败 
#define  MTC_PORT_TRANS_ERR             -3      //串口数据通信异常 
#define  MTC_NETWORK_ERR                -4      //网络异常 
#define  MTC_BOX_ERR                    -5      //支付终端异常 
#define  MTC_OTHER_ERR                  -6      //其他异常 
#define  MTC_SCANTIMEOUT_ERR            -7      //扫码查询超时     
#define  MTC_SENDDATALEN_ERR            -8      //发送数据长度错误
#define  MTC_RECEDATALEN_ERR            -9      //支付终端收到数据平台返回的数据,长度超过上位机设定的缓冲长度
#define  MTC_BSREQUESTTIMEOUT_ERR       -10     //等待平台响应超时



typedef struct _DevTimeout
{   
    int ScanQrTimeout;          //二维码扫码超时时间，单位 ms
    int othertimeout;           //其他种类超时时间，单位 ms
}DevTimeout;

typedef struct _Devinfo
{
    char deviceType[5];         //终端类型。0001 : 手持终端,0002:自助终端
    char qrdllver[5];           //QR动态库版本号，如“0100”表示1.00版本
    char deviceSerial[33];      //终端编码，
    char deviceSoftVer[5];      //设备的软件版本
    char deviceHardwareVer[5];  //设备的硬件版本
    _Devinfo()
    {
        memset(this,0,sizeof(_Devinfo));
    }
}Devinfo;


enum DevStateEnum
{
    QR_STATE_NETERROR_MASK      = 0,      //:网络异常；
    QR_STATE_COMERROR_MASK      = 1,      //:串口通信异常；
    QR_STATE_TERMINALERROR_MASK = 2,      //终端异常；
    QR_STATE_OTHERERROR_MASK    = 3       //其他异常；
};                       




#endif // STATE_DEFINE_H
