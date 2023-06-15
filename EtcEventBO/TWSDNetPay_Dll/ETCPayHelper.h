#ifndef ETC_PAY_HELPER_H
#define ETC_PAY_HELPER_H

#include <string>
#include "TypeHelper.h"
#include <map>
#include <list>
#include "SystemDiary.h"





// OBU白名单查询参数
struct ObuWhiteListQueryParameter
{
    char Version[20];
    char PayIdentifier[50];
    int Money;
    char OBUMAC[50];
    char OBUID[50];
    char CardID[50];
    int VehicleType;
    int VehicleClass;
    int VehicleLicenseColor;
    char VehicleLicense[30];
    char AutoLicense[50];
    int AutoColor;
    char EntryRoadID[200];
    char EntryStationID[200];
    char EntryLaneID[200];
    time_t EntryTime;

    ObuWhiteListQueryParameter()
    {
        memset(this, 0, sizeof(ObuWhiteListQueryParameter));
    }
};

// 交易状态查询参数
struct DebitStateQueryParameter
{
    char Version[20];
    char PayIdentifier[50];
    int VehicleType;
    int VehicleClass;
    int VehicleLicenseColor;
    char VehicleLicense[50];
    char AutoLicense[50];
    int AutoColor;
    DebitStateQueryParameter()
    {
        memset(this, 0, sizeof(DebitStateQueryParameter));
    }
};

// 交易状态数据
struct DebitStateData
{
    int ResultType;
    int ExecuteCode;
    char ExecuteDesc[50];
    char TradeSecurityCode[50];
    int PayPlatformType;
    char PayIdentifier[50];
    char DebitOrder[50];
    time_t DebitTime;
    time_t ConsumeTime;
    DebitStateData()
    {
        memset(this, 0, sizeof(DebitStateData));
    }
};

// 一条完整的白名单记录
struct ObuWhiteListRecord
{
    time_t GenerateTime;//生成时间
    char VehicleLicense[50];
    int VehicleLicenseColor;
    int VehicleType;
    int VehicleClass;
    int Money;
    char QueryIdentifier[50];
    int AgentStatus;
    ObuWhiteListRecord()
    {
        memset(this, 0, sizeof(ObuWhiteListRecord));
    }
};


// 一条完整的ETC交易信息。
struct ETCDebitRecord
{
    char Version[10];
    int OverTimeMs;
    char PayIdentifier[50];
    int Money;
    time_t SubTime;
    int DebitType;
    int VehicleType;
    int VehicleClass;
    int VehicleLicenseColor;
    char VehicleLicense[20];
    char AutoLicense[20];
    int AutoColor;
    int AxisCount;
    int Weight;
    int LimitWeight;
    int PassCertificateType;
    int TollDate;
    int ShiftID;
    int Distance;
    char CardID[20];
    char TicketNo[20];
    char OperatorID[20];
    char OperatorName[20];
    char EntryAreaID[50];
    char EntryRoadID[50];
    char EntryStationID[50];
    char EntryStationName[50];
    char EntryLaneID[50];
    char EntryLicense[50];
    int EntryLicenseColor;
    int EntryVehicleClass;
    int EntryVehicleType;
    time_t EntryTime;
    char OBULicense[20];
    char OBUID[20];
    char OBUMAC[20];
    int OBULicenseColor;
    int OBUVehicleClass;
    int OBUVehicleType;

    struct
    {
        char Version[10];
        int ResultType;
        int ExecuteCode;
        char ExecuteDesc[20];
        char TradeSecurityCode[50];
        int PayPlatformType;
        char PayIdentifier[50];
        time_t DebitTime;
        int ConsumeTime;
		char PaySerialNo[50];
    }LocalResponse;
    struct
    {
        char DebitOrder[50];
        int ServerStatus;
    } ServerResponse;
    ETCDebitRecord()
    {
        memset(this, 0, sizeof(ETCDebitRecord));
    }
};
//ETC 扣费回应。
struct ETCDebitResponse
{
    ETCDebitResponse()
    {
        memset(this, 0, sizeof(ETCDebitResponse));
    }
};

// ETC交易摘要
struct ETCDebitAbstract
{
    time_t DebitTime;
    char PlateNo[50];
    bool IsSuccess;
    ETCDebitAbstract()
    {
        memset(this,0,sizeof(ETCDebitAbstract));
    }
};

/*解析白名单查询参数，JSON格式*/
bool ParseWhiteListQueryParaFromJson(const std::string& json_para, ObuWhiteListQueryParameter&query_info);
/*解析白名单查询参数，XML格式*/
bool ParseWhiteListQueryParaFromXML(const std::string& xml_para, ObuWhiteListQueryParameter& query_info);
/*代扣查询状态值，转换为对外接口值*/
int AgencyQueryStatusServer2Local(int server_code);//云端返回码，转换成本地状态码

/*解析交易结果查询参数，JSON格式*/
bool ParseDebitStateQueryParaFromJson(const std::string& json_para, DebitStateQueryParameter& query_info);
/*解析交易结果查询参数，XML格式*/
bool ParseDebitStateQueryParaFromXML(const std::string& json_para, DebitStateQueryParameter& query_info);
/*解析服务端回馈的交易状态*/
bool ParseDebitStateFromServer(const std::string& json_response, DebitStateData& state);
/*解析交易参数，JSON格式*/
bool ParseETCDebitParaFromJson(const std::string& json_para, ETCDebitRecord& debit_para);
/*解析交易参数，XML格式*/
bool ParseETCDebitParaFromXML(const std::string& json_para, ETCDebitRecord& debit_para);
/*解析服务端的扣费回馈*/
bool ParseETCDebitResponseFromServer(const std::string& json_response, ETCDebitRecord& state);

/*生成ETC扣费结果，XML格式*/
int PackETCDebitResultXML(const ETCDebitRecord& debit_record, std::string& result);
/*生成ETC扣费结果，JSON格式*/
int PackETCDebitResultJson(const ETCDebitRecord& debit_record, std::string& result);
/*生成发往服务端的ETC扣费参数*/
int PackETCDebitRequestJson(const ETCDebitRecord& debit_record, std::string& result);
/*生成发往服务端的ETC白名单鉴权参数*/
int PackOBUWhiteListQueryJson(const ObuWhiteListQueryParameter& query_para,std::string&result);
/*解析服务端回馈的白名单鉴权*/
bool ParseOBUWhiteListRecordFromServer(const std::string& json_response,ObuWhiteListRecord& obu_white_record);

/*获取交易匹配码*/
std::string GetNetPayRecordMatchCode(const ETCDebitRecord& dp );

#endif // ETC_PAY_HELPER_H

/**
 * ETC支付
 */