#ifndef STATE_DEFINE_H
#define STATE_DEFINE_H

#define  MTC_NO_ERROR                       0      //���� 
#define  MTC_SCAN_PROCESSING            1       //ɨ�����ڴ���     
#define  MTC_PORTOPEN_ERR               -1      //�򿪴���ʧ�� 
#define  MTC_PORTCLOSE_ERR              -2      //�رմ���ʧ�� 
#define  MTC_PORT_TRANS_ERR             -3      //��������ͨ���쳣 
#define  MTC_NETWORK_ERR                -4      //�����쳣 
#define  MTC_BOX_ERR                    -5      //֧���ն��쳣 
#define  MTC_OTHER_ERR                  -6      //�����쳣 
#define  MTC_SCANTIMEOUT_ERR            -7      //ɨ���ѯ��ʱ     
#define  MTC_SENDDATALEN_ERR            -8      //�������ݳ��ȴ���
#define  MTC_RECEDATALEN_ERR            -9      //֧���ն��յ�����ƽ̨���ص�����,���ȳ�����λ���趨�Ļ��峤��
#define  MTC_BSREQUESTTIMEOUT_ERR       -10     //�ȴ�ƽ̨��Ӧ��ʱ



typedef struct _DevTimeout
{   
    int ScanQrTimeout;          //��ά��ɨ�볬ʱʱ�䣬��λ ms
    int othertimeout;           //�������೬ʱʱ�䣬��λ ms
}DevTimeout;

typedef struct _Devinfo
{
    char deviceType[5];         //�ն����͡�0001 : �ֳ��ն�,0002:�����ն�
    char qrdllver[5];           //QR��̬��汾�ţ��硰0100����ʾ1.00�汾
    char deviceSerial[33];      //�ն˱��룬
    char deviceSoftVer[5];      //�豸������汾
    char deviceHardwareVer[5];  //�豸��Ӳ���汾
    _Devinfo()
    {
        memset(this,0,sizeof(_Devinfo));
    }
}Devinfo;


enum DevStateEnum
{
    QR_STATE_NETERROR_MASK      = 0,      //:�����쳣��
    QR_STATE_COMERROR_MASK      = 1,      //:����ͨ���쳣��
    QR_STATE_TERMINALERROR_MASK = 2,      //�ն��쳣��
    QR_STATE_OTHERERROR_MASK    = 3       //�����쳣��
};                       




#endif // STATE_DEFINE_H
