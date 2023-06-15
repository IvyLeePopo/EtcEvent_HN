#ifndef PAY_HELPER_H
#define PAY_HELPER_H

#include <string>
struct DebitParameter
{
    struct
    {
        int ProvinceID;
        char   AreaID[200];
        char   RoadID[200];
        char   StationID[200];
        char   LaneID[200];
		char   StationName[200];
    } LaneConfig;
    int DealSN;
    bool    Valid;
    int     OverTime;
    int     OperationMode;
    int     PayModel;
    char    Version[10];

    /*****************Debit详细项************************/
    char PayIdentifier[50];
    int     Money;
    time_t  SubTime;
    int     DebitMainType;
    char PayCertificateCode[50];
    int     PayCertificateType;

    /*****************Vehicle详细项************************/
    int     VehicleType;//车种
    int     VehicleClass;//车型
    int     VLColor;
    char VehicleLicense[40];
    int     AxisCount;
    int     Weight;
    int     LimitWeight;
    char AutoLicense[50];
    int AutoColor;

    /*****************Operation详细项************************/
    int     PassCertificateType;
    char CardID[50];
    int     TollDate;
    int     ShiftID;
    char TicketNo[50];
    char OperatorID[50];
    char OperatorName[50];
    int Distance;
    /*****************Entry详细项************************/
    char EntryStationID[150];
    char EntryStationName[150];
    time_t   EntryTime;
    char EntryLaneID[150];
    char EntryVehicleLicense[50];
    int     EntryVLColor;
    int     EntryVehicleType;//入口车种
    int     EntryVehicleClass;//入口车型
    char     EntryAreaID[150];
    char     EntryRoadID[150];


    char QRCode[1000];
    char TWProductSerialNo[20];
    char NetPayDllVersion[20];
    char DeviceDllVersion[20];
    char NetPayRecordMatchCode[100];
    int DebitType;
	int AccountInfoSrcType;
	int AccountInfoDataType;
	int AccountInfoDeviceID;
	int PackageType;
    /*****************NFC详细项************************/
    char NFCPayData[1000];//NFC消费报文
    char NFCRevData[1000];//NFC冲正报文

    char ImageId[50];//车牌识别系统生成的图像ID
    char TaskId[50];

    bool IsCancled;
    bool IsUnipay;//是否属于银联支付(闪付和二维码)
    DebitParameter()
    {
        memset(this, 0, sizeof(DebitParameter));
    }
};

struct DebitCancleParameter
{
    int ProvinceID;
    char Version[10];
    char PayIdentifier[100];
	char LaneInfo[100];
    bool EnableNotification; //是否通知调用者
    time_t GenerateTime;
    DebitCancleParameter()
    {
        memset(this,0,sizeof(DebitCancleParameter));
    }
};

struct DebitCancleResultParameter
{
    char DebitOrderNumber[50];
    char Message[50];
    int PackageType;
    char PayIdentifier[50];
    char ReData[50];
    time_t ReceveTime;
    char RecordMatchCode[50];
    time_t ResponseTime;
    int Status;
    char Version[10];
    DebitCancleResultParameter()
    {
        memset(this,0,sizeof(DebitCancleResultParameter));
    }
};


struct ETCCardDataResult
{
    int IsUseVirtualETCCard;
    int ETCExcuteCode;
    char VirtualETCCardID[30];
    char IssueFlag[20];
    char IssueIndex[20];
    int ETCCardType;
    char ETCVehicleLicense[20];
    int ETCBeforeBalance;
    int ETCAfterBalance;
    int ETCProTradeNo;
    char ETCTerminalTradeNo[20];
    int ETCTradeDate;
    int ETCTradeTime;
    char ETCTac[10];
    ETCCardDataResult()
    {
        memset(this,0,sizeof(ETCCardDataResult));
    }
};

struct DebitResultParameter
{
    char  Version[10];
    int PackageType;
    int ServerResponseCode;
    char ServerResponseResultDesc[200];
    time_t ServerReceiveDataTime;
    time_t ServerResponseDataTime;
    int DebitStatusCode;
    char DebitStatusDesc[100];
    char DebitOrderNumber[100];
    char ServerRecordMatchCode[50];
    char PayVirtualCardID[100];
    int AgencyExcuteCode;//代扣返回码


    int     ResultType;
    int     ExecuteCode;
    int     PayPlatformType;
    int     ServerPayPlatformType;//云端返回的原始支付类型。
    time_t  ServerDebitTime;
    int     ConsumeTimeMsec;
    char   PayIdentifier[128];
    char   ExecuteDesc[256];
    char   TradeSecurityCode[64];

    ETCCardDataResult ExpandETCData;
    struct 
    {
        int ErrorCode;   
    }TraceInfo;
    DebitResultParameter()
    {
        memset(this,0,sizeof(DebitResultParameter));
        TraceInfo.ErrorCode=255;
    }
  
    
};


struct InvoiceParameter
{
	char	ElectronicInvoiceNo[48];
	char    DebitOrderNumber[48];
	int		IsDataBaseRecord;
	int		NeedNotifyResult;
	int     Money;
	int     PayMode;
	char	VehicleLicense[16];
	int     VehicleColor;
	int     VehicleType;
	int		VehicleClass;
	int		AxisCount;
	int		TotalWeight;
	int		LimitWeight;
	char	PayAreaID[64];
	char	PayProvinceId[16];
	char	PayRoadID[32];
	char	PayStationID[32];
	char	PayStationName[64];
	char	PayLane[32];
	char	ExitTime[64];
	time_t  nExitTime;
	int		PassCertificateType;
	char	PassCertificateCode[64];
	char	TollDate[64];
	int		ShiftID;
	char	OperatorID[32];
	char	OperatorName[32];
	int		Distance;
	char	EntryAreaID[64];
	char	EntryRoadID[32];
	char	EntryStationID[32];
	char	EntryStationName[32];
	char	EntryLaneID[16];
	char	EntryTime[64];
	time_t  nEntryTime;
	char	EntryVehicleLicense[16];
	int		EntryVLColor;
	int		EntryVehicleClass;
	int		EntryVehicleType;
	char	QRcode[512];

    InvoiceParameter()
    {
        memset(this, 0, sizeof(InvoiceParameter));
    }
};

struct InvoiceResultParameter
{
	char	ElectronicInvoiceNo[48];
	int		ExecuteCode;
	char	DealTime[64];

    InvoiceResultParameter()
    {
        memset(this, 0, sizeof(InvoiceResultParameter));
    }
};

struct InvoiceCancelResultParameter
{
	char	ElectronicInvoiceNo[48];
	int		ExecuteCode;
	char	DealTime[64];

    InvoiceCancelResultParameter()
    {
        memset(this, 0, sizeof(InvoiceCancelResultParameter));
    }
};

struct InvoiceCancelParameter
{
	int		NeedNotifyResult;
	int		IsDataBaseRecord;
	char	ElectronicInvoiceNo[48];
	char	PayProvinceId[16];
	char	ExitTime[48];

    InvoiceCancelParameter()
    {
        memset(this, 0, sizeof(InvoiceCancelParameter));
    }
};


bool ParseDebitParaFromJson(const std::string& json_para, DebitParameter& dp );
bool ParseDebitParaFromXML(const std::string& xml_para, DebitParameter& dp );

bool ParseDebitCancleParaFromJson(const std::string& json_para, DebitCancleParameter& dp );
bool ParseDebitCancleParaFromXML(const std::string& xml_para, DebitCancleParameter& dp );

bool ParseDebitAgencyETCDataFromXML(const std::string& xml_para,DebitResultParameter& dp,const std::string& curr_id);
bool ParseDebitAgencyResultFromXML(const std::string& xml_para,DebitResultParameter& dp);

bool ParseDebitResultFromJson(const std::string& json_para,DebitResultParameter& dp);
bool ParseDebitCancleResultFromServer(const std::string json_para,DebitCancleResultParameter& cancle_result);
bool ParseImageTransferParaFromXML(const std::string xml_para,std::string& xml_result);
bool ParseImageTransferParaFromJson(const std::string json_para,std::string& xml_result);

int PackDebitHttpJson(const DebitParameter& dp,std::string& json_str);
int PackDebitCheckHttpJson(const DebitParameter& dp,std::string& json_str);
int PackDebitCancleHttpJson(const DebitCancleParameter& dp,std::string& json_str);

int PackDebitResult2XML(const DebitResultParameter& dp,std::string& xml_str);
int PackDebitResult2Json(const DebitResultParameter& dp,std::string& json_str);

int PackDebitAgencyXML(const DebitParameter& dp,std::string& result);

int ServerPayCode2LocalCode(int server_code);

// 将云端返回的错误状态值，转换为相应的界面提示类型，用于更详细的错误提示
int GetPayFailHintCodeAccordingToServerPayCode(int server_code);
const std::string GetNetPayRecordMatchCode(const DebitParameter& dp );

//电子发票
int PackInvoiceHttpJson(const InvoiceParameter& param, std::string& json_str);
bool ParseInvoiceResultFromJson(const std::string& json_para, InvoiceResultParameter& param);
int PackInvoiceCancelHttpJson(const InvoiceCancelParameter& param, std::string& json_str);
bool ParseInvoiceCancleResultFromServer(const std::string& json_para, InvoiceCancelResultParameter& param);


#endif // PAY_HELPER_H


/**
 * 支付数据辅助处理。
 */