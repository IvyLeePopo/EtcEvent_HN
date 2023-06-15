#ifndef ETC_PAY_HELPER_H
#define ETC_PAY_HELPER_H

#include <string>
#include "TypeHelper.h"
#include <map>
#include <list>
#include "SystemDiary.h"





// OBU��������ѯ����
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

// ����״̬��ѯ����
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

// ����״̬����
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

// һ�������İ�������¼
struct ObuWhiteListRecord
{
    time_t GenerateTime;//����ʱ��
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


// һ��������ETC������Ϣ��
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
//ETC �۷ѻ�Ӧ��
struct ETCDebitResponse
{
    ETCDebitResponse()
    {
        memset(this, 0, sizeof(ETCDebitResponse));
    }
};

// ETC����ժҪ
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

/*������������ѯ������JSON��ʽ*/
bool ParseWhiteListQueryParaFromJson(const std::string& json_para, ObuWhiteListQueryParameter&query_info);
/*������������ѯ������XML��ʽ*/
bool ParseWhiteListQueryParaFromXML(const std::string& xml_para, ObuWhiteListQueryParameter& query_info);
/*���۲�ѯ״ֵ̬��ת��Ϊ����ӿ�ֵ*/
int AgencyQueryStatusServer2Local(int server_code);//�ƶ˷����룬ת���ɱ���״̬��

/*�������׽����ѯ������JSON��ʽ*/
bool ParseDebitStateQueryParaFromJson(const std::string& json_para, DebitStateQueryParameter& query_info);
/*�������׽����ѯ������XML��ʽ*/
bool ParseDebitStateQueryParaFromXML(const std::string& json_para, DebitStateQueryParameter& query_info);
/*��������˻����Ľ���״̬*/
bool ParseDebitStateFromServer(const std::string& json_response, DebitStateData& state);
/*�������ײ�����JSON��ʽ*/
bool ParseETCDebitParaFromJson(const std::string& json_para, ETCDebitRecord& debit_para);
/*�������ײ�����XML��ʽ*/
bool ParseETCDebitParaFromXML(const std::string& json_para, ETCDebitRecord& debit_para);
/*��������˵Ŀ۷ѻ���*/
bool ParseETCDebitResponseFromServer(const std::string& json_response, ETCDebitRecord& state);

/*����ETC�۷ѽ����XML��ʽ*/
int PackETCDebitResultXML(const ETCDebitRecord& debit_record, std::string& result);
/*����ETC�۷ѽ����JSON��ʽ*/
int PackETCDebitResultJson(const ETCDebitRecord& debit_record, std::string& result);
/*���ɷ�������˵�ETC�۷Ѳ���*/
int PackETCDebitRequestJson(const ETCDebitRecord& debit_record, std::string& result);
/*���ɷ�������˵�ETC��������Ȩ����*/
int PackOBUWhiteListQueryJson(const ObuWhiteListQueryParameter& query_para,std::string&result);
/*��������˻����İ�������Ȩ*/
bool ParseOBUWhiteListRecordFromServer(const std::string& json_response,ObuWhiteListRecord& obu_white_record);

/*��ȡ����ƥ����*/
std::string GetNetPayRecordMatchCode(const ETCDebitRecord& dp );

#endif // ETC_PAY_HELPER_H

/**
 * ETC֧��
 */