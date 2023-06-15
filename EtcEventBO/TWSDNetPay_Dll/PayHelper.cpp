#include "stdafx.h"
#include "PayHelper.h"
#include "JsonHelper.h"
#include "XMLHelper.h"
#include "TypeHelper.h"
#include "SystemDiary.h"
#include "BaseDefine.h"
#include "RunParameter.h"
#include "TWSDNetPay_Dll.h"
#include "pystring.h"

bool ParseDebitParaFromJson(const std::string& json_para, DebitParameter& dp )
{
    Json::Reader json_reader;
    Json::Value root_node, context_node, data_node, debit_node;
    Json::Value vehicle_node, operation_node, entry_node, obu_node;
    Json::Value auto_plate_node;
    if (!json_reader.parse(json_para, root_node))
    {
        return false;
    }
    bool read_success = true;
    read_success = read_success ? ReadJsonSubNode(root_node, "DebitParamContext", context_node) : false;
    read_success = read_success ? ReadJsonSubNode(context_node, "Data", data_node) : false;
    read_success = read_success ? ReadJsonSubNode(data_node, "Debit", debit_node) : false;
    read_success = read_success ? ReadJsonSubNode(data_node, "Vehicle", vehicle_node) : false;
    read_success = read_success ? ReadJsonSubNode(data_node, "Operation", operation_node) : false;
    read_success ? ReadJsonSubNode(data_node, "Entry", entry_node) : true;
    read_success ? ReadJsonSubNode(data_node, "AutoPlate", auto_plate_node) : true;

    read_success = read_success ? ReadJsonInt(data_node, "OverTime", dp.OverTime) : false;
    read_success = read_success ? ReadJsonInt(data_node, "OperationMode", dp.OperationMode) : false;
    read_success = read_success ? ReadJsonInt(debit_node, "Money", dp.Money) : false;
    read_success = read_success ? ReadJsonChars(debit_node, "PayIdentifier", dp.PayIdentifier, sizeof(dp.PayIdentifier)) : false;
    if (read_success && dp.OperationMode == 2)
    {
        read_success = read_success ? ReadJsonInt(debit_node, "PayCertificateType", dp.PayCertificateType) : false;
        read_success = read_success ? ReadJsonChars(debit_node, "PayCertificateCode", dp.PayCertificateCode, sizeof(dp.PayCertificateCode)) : false;
    }
    read_success = read_success ? ReadJsonInt(debit_node, "Type", dp.DebitMainType) : false;
    read_success = read_success ? ReadJsonTimet(debit_node, "SubTime", dp.SubTime) : false;

    read_success = read_success ? ReadJsonInt(vehicle_node, "Type", dp.VehicleType) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "Class", dp.VehicleClass) : false;
    read_success = read_success ? ReadJsonChars(vehicle_node, "License", dp.VehicleLicense, sizeof(dp.VehicleLicense)) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "VLColor", dp.VLColor) : false;
    if (read_success && dp.DebitMainType != 4)
    {
        read_success = read_success ? ReadJsonInt(vehicle_node, "AxisCount", dp.AxisCount) : false;
        read_success = read_success ? ReadJsonInt(vehicle_node, "Weight", dp.Weight) : false;
    }
    if (read_success && dp.DebitMainType != 4)
    {
        read_success = read_success ? ReadJsonInt(operation_node, "PassCertificateType", dp.PassCertificateType) : false;
        read_success = read_success ? ReadJsonChars(operation_node, "CardID", dp.CardID, sizeof(dp.CardID)) : false;
    }
    if (read_success && !ReadJsonInt(operation_node, "TollDate", dp.TollDate))
    {
        std::string date_str;
        read_success = ReadJsonStr(operation_node, "TollDate", date_str);
        dp.TollDate = Str2Int(date_str);

    }
    read_success = read_success ? ReadJsonInt(operation_node, "ShiftID", dp.ShiftID) : false;

    if (read_success && dp.DebitMainType != 4)
    {
        read_success = read_success ? ReadJsonStrOrInt(entry_node, "AreaID", dp.EntryAreaID, sizeof(dp.EntryAreaID)) : false;
        read_success = read_success ? ReadJsonStrOrInt(entry_node, "RoadID", dp.EntryRoadID, sizeof(dp.EntryRoadID)) : false;
        read_success = read_success ? ReadJsonStrOrInt(entry_node, "StationID", dp.EntryStationID, sizeof(dp.EntryStationID)) : false;
        read_success = read_success ? ReadJsonStrOrInt(entry_node, "LaneID", dp.EntryLaneID, sizeof(dp.EntryLaneID)) : false;
        read_success = read_success ? ReadJsonChars(entry_node, "StationName", dp.EntryStationName, sizeof(dp.EntryStationName)) : false;
        read_success = read_success ? ReadJsonInt(entry_node, "VType", dp.EntryVehicleType) : false;
        read_success = read_success ? ReadJsonInt(entry_node, "VClass", dp.EntryVehicleClass) : false;
        read_success = read_success ? ReadJsonChars(entry_node, "License", dp.EntryVehicleLicense, sizeof(dp.EntryVehicleLicense)) : false;
        read_success = read_success ? ReadJsonInt(entry_node, "VColor", dp.EntryVLColor) : false;
        read_success = read_success ? ReadJsonTimet(entry_node, "Time", dp.EntryTime) : false;
    }
    if (!read_success)
    {
        TW_TRACE_DIARY("Json解析错误:" + GetJsonParseError());
        return false;
    }
    ReadJsonChars(auto_plate_node, "AutoLicense", dp.AutoLicense, sizeof(dp.AutoLicense));
    ReadJsonInt(auto_plate_node, "AutoColor", dp.AutoColor);
    ReadJsonInt(data_node, "PayModel", dp.PayModel);
    ReadJsonInt(vehicle_node, "LimitWeight", dp.LimitWeight);
    ReadJsonChars(operation_node, "TicketNo", dp.TicketNo, sizeof(dp.TicketNo));
    ReadJsonChars(operation_node, "OperatorID", dp.OperatorID, sizeof(dp.OperatorID));
    ReadJsonChars(operation_node, "OperatorName", dp.OperatorName, sizeof(dp.OperatorName));
    ReadJsonInt(operation_node, "Distance", dp.Distance);

    return true;
}
bool ParseDebitParaFromXML(const std::string& xml_para, DebitParameter& dp )
{
    CMarkup xml;
    xml.SetDoc(xml_para.c_str());
    xml.ResetMainPos();

    bool read_success = true;
    read_success = read_success ? IntoXMLSubNode(xml, "DebitParamContext") : false;
    read_success = read_success ? ReadXMLCStr(xml, "Version", dp.Version, sizeof(dp.Version)) : false;
    read_success = read_success ? IntoXMLSubNode(xml, "Data") : false;
    read_success = read_success ? ReadXMLInt(xml, "OverTime", dp.OverTime) : false;
    read_success = read_success ? ReadXMLInt(xml, "OperationMode", dp.OperationMode) : false;
    read_success ? ReadXMLInt(xml, "PayModel", dp.PayModel) : true;

    {
        read_success = read_success ? IntoXMLSubNode(xml, "Debit") : false;
        read_success = read_success ? ReadXMLCStr(xml, "PayIdentifier", dp.PayIdentifier, sizeof(dp.PayIdentifier)) : false;
        read_success = read_success ? ReadXMLInt(xml, "Money", dp.Money) : false;
        read_success = read_success ? ReadXMLTimet(xml, "SubTime", dp.SubTime) : false;
        read_success = read_success ? ReadXMLInt(xml, "Type", dp.DebitMainType) : false;
        if (read_success && dp.OperationMode == 2)
        {
            read_success = read_success ? ReadXMLInt(xml, "PayCertificateType", dp.PayCertificateType) : false;
            read_success = read_success ? ReadXMLCStr(xml, "PayCertificateCode", dp.PayCertificateCode, sizeof(dp.PayCertificateCode)) : false;
        }
        read_success = read_success ? OutXML2Parent(xml) : false;
    }
    {
        read_success = read_success ? IntoXMLSubNode(xml, "Vehicle") : false;
        read_success = read_success ? ReadXMLInt(xml, "Type", dp.VehicleType) : false;
        read_success = read_success ? ReadXMLInt(xml, "Class", dp.VehicleClass) : false;
        read_success = read_success ? ReadXMLInt(xml, "VLColor", dp.VLColor) : false;
        read_success = read_success ? ReadXMLCStr(xml, "License", dp.VehicleLicense, sizeof(dp.VehicleLicense)) : false;
        if (read_success && dp.DebitMainType != 4)
        {
            read_success = read_success ? ReadXMLInt(xml, "AxisCount", dp.AxisCount) : false;
            read_success = read_success ? ReadXMLInt(xml, "Weight", dp.Weight) : false;
            read_success ? ReadXMLInt(xml, "LimitWeight", dp.LimitWeight) : true;
        }
        if (read_success && IntoXMLSubNode(xml, "AutoPlate"))
        {
            read_success ? ReadXMLInt(xml, "AutoColor", dp.AutoColor) : true;
            read_success ? ReadXMLCStr(xml, "AutoLicense", dp.AutoLicense, sizeof(dp.AutoLicense)) : true;
            OutXML2Parent(xml);
        }
        read_success = read_success ? OutXML2Parent(xml) : false;
    }

    {
        read_success = read_success ? IntoXMLSubNode(xml, "Operation") : false;
        if (read_success && dp.DebitMainType != 4)
        {
            read_success = read_success ? ReadXMLInt(xml, "PassCertificateType", dp.PassCertificateType) : false;
            read_success = read_success ? ReadXMLCStr(xml, "CardID", dp.CardID, sizeof(dp.CardID)) : false;
        }
        read_success = read_success ? ReadXMLInt(xml, "TollDate", dp.TollDate) : false;
        read_success = read_success ? ReadXMLInt(xml, "ShiftID", dp.ShiftID) : false;
        read_success ? ReadXMLCStr(xml, "TicketNo", dp.TicketNo, sizeof(dp.TicketNo)) : true;
        read_success ? ReadXMLCStr(xml, "OperatorID", dp.OperatorID, sizeof(dp.OperatorID)) : true;
        read_success ? ReadXMLCStr(xml, "OperatorName", dp.OperatorName, sizeof(dp.OperatorName)) : true;
        read_success ? ReadXMLInt(xml, "Distance", dp.Distance) : true;
        read_success = read_success ? OutXML2Parent(xml) : false;
    }
    if (read_success && dp.DebitMainType != 4)
    {
        read_success = read_success ? IntoXMLSubNode(xml, "Entry") : false;

        read_success = read_success ? ReadXMLCStr(xml, "AreaID", dp.EntryAreaID, sizeof(dp.EntryAreaID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "RoadID", dp.EntryRoadID, sizeof(dp.EntryRoadID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "StationID", dp.EntryStationID, sizeof(dp.EntryStationID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "StationName", dp.EntryStationName, sizeof(dp.EntryStationName)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "LaneID", dp.EntryLaneID, sizeof(dp.EntryLaneID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "License", dp.EntryVehicleLicense, sizeof(dp.EntryVehicleLicense)) : false;
        read_success = read_success ? ReadXMLTimet(xml, "Time", dp.EntryTime) : false;
        read_success = read_success ? ReadXMLInt(xml, "VColor", dp.EntryVLColor) : false;
        read_success = read_success ? ReadXMLInt(xml, "VClass", dp.EntryVehicleClass) : false;
        read_success = read_success ? ReadXMLInt(xml, "VType", dp.EntryVehicleType) : false;
        read_success = read_success ? OutXML2Parent(xml) : false;
    }
    if (!read_success)
    {
        TW_TRACE_DIARY("XML参数错误:" + GetXMLParseError() );
        return false;
    }
    return true;

}

bool ParseDebitCancleParaFromJson(const std::string& json_para, DebitCancleParameter& dp )
{
    Json::Reader json_reader;
    Json::Value root_node, context_node, data_node, trade_node;
    if (!json_reader.parse(json_para, root_node))
    {
        return false;
    }
    bool read_success = true;
    read_success = read_success ? ReadJsonSubNode(root_node, "DebitCancelParamContext", context_node) : false;
    read_success = read_success ? ReadJsonSubNode(context_node, "Data", data_node) : false;
    read_success = read_success ? ReadJsonSubNode(data_node, "TradeKeyItem", trade_node) : false;
    read_success = read_success ? ReadJsonChars(trade_node, "PayIdentifier", dp.PayIdentifier, sizeof(dp.PayIdentifier)) : false;
    read_success = read_success ? ReadJsonChars(context_node, "Version", dp.Version, sizeof(dp.Version)) : false;
    if (!read_success)
    {
        TW_TRACE_DIARY("Json解析错误:" + GetJsonParseError());
        return false;
    }
    return true;
}
bool ParseDebitCancleParaFromXML(const std::string& xml_para, DebitCancleParameter& dp )
{
    CMarkup xml;
    xml.SetDoc(xml_para.c_str());
    xml.ResetMainPos();

    bool read_success = true;
    read_success = read_success ? IntoXMLSubNode(xml, "DebitCancelParamContext") : false;
    read_success = read_success ? ReadXMLCStr(xml, "Version", dp.Version, sizeof(dp.Version)) : false;
    read_success = read_success ? IntoXMLSubNode(xml, "Data") : false;
    read_success = read_success ? IntoXMLSubNode(xml, "TradeKeyItem") : false;
    read_success = read_success ? ReadXMLCStr(xml, "PayIdentifier", dp.PayIdentifier, sizeof(dp.PayIdentifier)) : false;
    if (!read_success)
    {
        TW_TRACE_DIARY("XML参数错误:" + GetXMLParseError() );
        return false;
    }
    return true;
}

int PackDebitResult2XML(const DebitResultParameter& dp, std::string& xml_str)
{
    CMarkup xml;
    xml.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
    xml.AddElem( _T("DebitResultContext") );//root
    xml.IntoElem();

    xml.AddElem(_T("Version"), _T("1.0"));
    xml.AddElem(_T("Data"));
    xml.IntoElem();

    xml.AddElem(_T("ResultType"),   dp.ResultType);
    xml.AddElem(_T("ExecuteCode"),  dp.ExecuteCode);
    xml.AddElem(_T("ExecuteDesc"),  dp.ExecuteDesc);

    xml.AddElem(_T("TradeKeyItem"));
    xml.IntoElem();
    xml.AddElem(_T("TradeSecurityCode"),    dp.TradeSecurityCode);
    xml.AddElem(_T("PayPlatformType"),      dp.PayPlatformType);
    xml.AddElem(_T("PayIdentifier"),        dp.PayIdentifier);
    xml.AddElem(_T("DebitOrder"),           dp.DebitOrderNumber);
    xml.AddElem(_T("DebitTime"),            Datetime2Str(dp.ServerDebitTime).c_str());
    xml.AddElem(_T("ConsumeTime"),          dp.ConsumeTimeMsec);

    xml.AddElem(_T("ETCTradeInfo"));
    xml.IntoElem();

    xml.AddElem(_T("ETCExecuteCode"), dp.ExpandETCData.ETCExcuteCode);
    xml.AddElem(_T("IsUseVirtualETCCard"), dp.ExpandETCData.IsUseVirtualETCCard);
    xml.AddElem(_T("VirtualETCCardID"), dp.ExpandETCData.VirtualETCCardID);
    xml.AddElem(_T("IssueFlag"), dp.ExpandETCData.IssueFlag);
    xml.AddElem(_T("IssueIndex"), dp.ExpandETCData.IssueIndex);
    xml.AddElem(_T("ETCCardType"), dp.ExpandETCData.ETCCardType);
    xml.AddElem(_T("ETCVehicleLicense"), dp.ExpandETCData.ETCVehicleLicense);
    xml.AddElem(_T("ETCBeforeBalance"), dp.ExpandETCData.ETCBeforeBalance);
    xml.AddElem(_T("ETCAfterBalance"), dp.ExpandETCData.ETCAfterBalance);
    xml.AddElem(_T("ETCProTradeNo"), dp.ExpandETCData.ETCProTradeNo);
    xml.AddElem(_T("ETCTerminalTradeNo"), dp.ExpandETCData.ETCTerminalTradeNo);
    xml.AddElem(_T("ETCTradeDate"), dp.ExpandETCData.ETCTradeDate);
    xml.AddElem(_T("ETCTradeTime"), dp.ExpandETCData.ETCTradeTime);
    xml.AddElem(_T("ETCTac"), dp.ExpandETCData.ETCTac);

    xml.OutOfElem();

    xml.OutOfElem();
    xml.OutOfElem();

    xml_str = (LPCSTR)xml.GetDoc();

    xml_str = pystring::replace(xml_str, "\r\n", "");

    return 0;
}
int PackDebitResult2Json(const DebitResultParameter& dp, std::string& json_str)
{
    Json::Value root;

    Json::Value ResultContext;

    Json::Value Data;
    Json::Value KeyItem;
    Json::Value ETCTradeInfo;

    ResultContext["Version"]        = Json::Value("1.0");

    Data["ResultType"]              = Json::Value(dp.ResultType);
    Data["ExecuteCode"]             = Json::Value(dp.ExecuteCode);
    Data["ExecuteDesc"]             = Json::Value(dp.ExecuteDesc);

    KeyItem["TradeSecurityCode"]    = Json::Value(dp.TradeSecurityCode);
    KeyItem["PayPlatformType"]      = Json::Value(dp.PayPlatformType);
    KeyItem["PayIdentifier"]        = Json::Value(dp.PayIdentifier);
    KeyItem["DebitOrder"]           = Json::Value(dp.DebitOrderNumber);
    KeyItem["DebitTime"]            = Json::Value(Datetime2Str( dp.ServerDebitTime ) );
    KeyItem["ConsumeTime"]          = Json::Value(dp.ConsumeTimeMsec);


    ETCTradeInfo["IsUseVirtualETCCard"] = Json::Value(dp.ExpandETCData.IsUseVirtualETCCard);
    ETCTradeInfo["ETCExecuteCode"] = Json::Value(dp.ExpandETCData.ETCExcuteCode);
    ETCTradeInfo["VirtualETCCardID"] = Json::Value(dp.ExpandETCData.VirtualETCCardID);
    ETCTradeInfo["IssueFlag"] = Json::Value(dp.ExpandETCData.IssueFlag);
    ETCTradeInfo["IssueIndex"] = Json::Value(dp.ExpandETCData.IssueIndex);
    ETCTradeInfo["ETCCardType"] = Json::Value(dp.ExpandETCData.ETCCardType);
    ETCTradeInfo["ETCVehicleLicense"] = Json::Value(dp.ExpandETCData.ETCVehicleLicense);
    ETCTradeInfo["ETCBeforeBalance"] = Json::Value(dp.ExpandETCData.ETCBeforeBalance);
    ETCTradeInfo["ETCAfterBalance"] = Json::Value(dp.ExpandETCData.ETCAfterBalance);
    ETCTradeInfo["ETCProTradeNo"] = Json::Value(dp.ExpandETCData.ETCProTradeNo);
    ETCTradeInfo["ETCTerminalTradeNo"] = Json::Value(dp.ExpandETCData.ETCTerminalTradeNo);
    ETCTradeInfo["ETCTradeDate"] = Json::Value(dp.ExpandETCData.ETCTradeDate);
    ETCTradeInfo["ETCTradeTime"] = Json::Value(dp.ExpandETCData.ETCTradeTime);
    ETCTradeInfo["ETCTac"] = Json::Value(dp.ExpandETCData.ETCTac);

    KeyItem["ETCTradeInfo"] = ETCTradeInfo;

    Data["TradeKeyItem"]        = KeyItem;
    ResultContext["Data"]       = Data;
    root["DebitResultContext"]  = ResultContext;
    //转化为字符串
    Json::FastWriter fast_writer;
    json_str = fast_writer.write(root);
    return 0;
}

int PackDebitAgencyXML(const DebitParameter& dp,  std::string& result)
{
    CMarkup xmlAgencyDebit;
    xmlAgencyDebit.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
    xmlAgencyDebit.AddElem( _T("DebitParamContext") );
    xmlAgencyDebit.IntoElem();

    xmlAgencyDebit.AddElem( _T("Version"),      _T("1.0") );
    xmlAgencyDebit.AddElem( _T("TimeOut"),      GetAgencyPayTimeoutMsec() );

    xmlAgencyDebit.AddElem( _T("Data") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("OverTime"),     dp.OverTime );

    xmlAgencyDebit.AddElem( _T("Debit") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("PayIdentifier"),    dp.PayIdentifier );
    xmlAgencyDebit.AddElem( _T("imageId"),    dp.ImageId );


    xmlAgencyDebit.AddElem( _T("Money"),            dp.Money );
    xmlAgencyDebit.AddElem( _T("SubTime"),          Datetime2Str( dp.SubTime).c_str() );
    xmlAgencyDebit.AddElem( _T("Type"),             1 );

    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.AddElem( _T("Vehicle") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("Type"),         dp.VehicleType );
    xmlAgencyDebit.AddElem( _T("Class"),        dp.VehicleClass );
    xmlAgencyDebit.AddElem( _T("License"),      dp.VehicleLicense );
    xmlAgencyDebit.AddElem( _T("VLColor"),      dp.VLColor );
    xmlAgencyDebit.AddElem( _T("AxisCount"),    dp.AxisCount );
    xmlAgencyDebit.AddElem( _T("Weight"),       dp.Weight );
    xmlAgencyDebit.AddElem( _T("LimitWeight"),  dp.LimitWeight );

    xmlAgencyDebit.AddElem( _T("AutoPlate"));
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("AutoLicense"),  dp.AutoLicense );
    xmlAgencyDebit.AddElem( _T("AutoColor"),  dp.AutoColor );
    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.AddElem( _T("Operation") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("PassCertificateType"),  dp.PassCertificateType );
    xmlAgencyDebit.AddElem( _T("CardID"),       dp.CardID );
    xmlAgencyDebit.AddElem( _T("TollDate"),     dp.TollDate );
    xmlAgencyDebit.AddElem( _T("ShiftID"),      dp.ShiftID );
    xmlAgencyDebit.AddElem( _T("TicketNo"),     dp.TicketNo );
    xmlAgencyDebit.AddElem( _T("Distance"),    dp.Distance );
    xmlAgencyDebit.AddElem( _T("PayOperatorId"),    dp.OperatorID );
    xmlAgencyDebit.AddElem( _T("PayOperatorName"),  dp.OperatorName );
    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.AddElem( _T("Entry") );
    xmlAgencyDebit.IntoElem();
    xmlAgencyDebit.AddElem( _T("ProvinceId"),   dp.LaneConfig.ProvinceID );
    xmlAgencyDebit.AddElem( _T("AreaID"),       dp.EntryAreaID );
    xmlAgencyDebit.AddElem( _T("RoadID"),       dp.EntryRoadID );
    xmlAgencyDebit.AddElem( _T("StationID"),    dp.EntryStationID );
    xmlAgencyDebit.AddElem( _T("StationName"),  dp.EntryStationName );
    xmlAgencyDebit.AddElem( _T("Time"),         Datetime2Str(dp.EntryTime).c_str() );

    xmlAgencyDebit.AddElem( _T("LaneID"),       dp.EntryLaneID );
    xmlAgencyDebit.AddElem( _T("License"),      dp.EntryVehicleLicense );
    xmlAgencyDebit.AddElem( _T("VColor"),       dp.EntryVLColor );
    xmlAgencyDebit.AddElem( _T("VClass"),       dp.EntryVehicleClass );
    xmlAgencyDebit.AddElem( _T("VType"),        dp.EntryVehicleType );
    xmlAgencyDebit.OutOfElem();

    xmlAgencyDebit.OutOfElem();
    result = (LPCSTR)xmlAgencyDebit.GetDoc();
    return 0;
}

bool ParseDebitAgencyETCDataFromXML(const std::string& xml_para, DebitResultParameter& dp, const std::string& curr_id )
{
    if (xml_para.empty())
    {
        return false;
    }

    if (!IsTianJinArea())
    {
        return false;
    }
    CMarkup xml;
    xml.SetDoc(xml_para.c_str());

    bool read_success = true;
    read_success = read_success ? IntoXMLSubNode(xml, "DebitResultContext") : false;
    read_success = read_success ? IntoXMLSubNode(xml, "Data") : false;

    int etc_excute_code = 0;
    read_success = read_success ? ReadXMLInt(xml, "ExecuteCode", etc_excute_code) : false;

    read_success = read_success ? IntoXMLSubNode(xml, "TradeKeyItem") : false;
    read_success = read_success ? IntoXMLSubNode(xml, "ETCTradeInfo") : false;

    read_success = read_success ? ReadXMLInt(xml, "IsUseVirtualETCCard", dp.ExpandETCData.IsUseVirtualETCCard) : false;
    read_success = read_success ? ReadXMLCStr(xml, "VirtualETCCardID", dp.ExpandETCData.VirtualETCCardID, 20) : false;
    read_success = read_success ? ReadXMLCStr(xml, "IssueFlag", dp.ExpandETCData.IssueFlag, 10) : false;
    read_success = read_success ? ReadXMLCStr(xml, "IssueIndex", dp.ExpandETCData.IssueIndex, 10) : false;
    read_success = read_success ? ReadXMLCStr(xml, "ETCVehicleLicense", dp.ExpandETCData.ETCVehicleLicense, 16) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCCardType", dp.ExpandETCData.ETCCardType) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCBeforeBalance", dp.ExpandETCData.ETCBeforeBalance) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCAfterBalance", dp.ExpandETCData.ETCAfterBalance) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCProTradeNo", dp.ExpandETCData.ETCProTradeNo) : false;
    read_success = read_success ? ReadXMLCStr(xml, "ETCTerminalTradeNo", dp.ExpandETCData.ETCTerminalTradeNo, 8) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCTradeDate", dp.ExpandETCData.ETCTradeDate) : false;
    read_success = read_success ? ReadXMLInt(xml, "ETCTradeTime", dp.ExpandETCData.ETCTradeTime) : false;
    read_success = read_success ? ReadXMLCStr(xml, "ETCTac", dp.ExpandETCData.ETCTac, 8) : false;


    strncpy(dp.PayIdentifier, curr_id.c_str(), 50);
    dp.ExpandETCData.ETCExcuteCode = 0;
    switch (etc_excute_code)
    {
    case 0:
        dp.ExpandETCData.ETCExcuteCode = 0;
        break;
    case 22:
        dp.ExpandETCData.ETCExcuteCode = 1;
        break;
    case 25:
        dp.ExpandETCData.ETCExcuteCode = 3;
        break;
    case 26:
        dp.ExpandETCData.ETCExcuteCode = 5;
        break;
    case 27:
        dp.ExpandETCData.ETCExcuteCode = 4;
        break;
    default:
        dp.ExpandETCData.IsUseVirtualETCCard = 0;
        char err_log[100] = {0};
        sprintf(err_log, "虚拟卡执行码解析结果：%d", etc_excute_code);
        TW_TRACE_DIARY(err_log);
        break;
    }

    return true;
}


bool ParseDebitAgencyResultFromXML(const std::string& xml_para, DebitResultParameter& dp)
{
    CMarkup xml;
    if (!xml.SetDoc(xml_para.c_str()))
    {
        TW_TRACE_DIARY("代扣结果,XML格式错误:" + xml_para);
        return false;
    }
    bool read_xml_success = true;
    read_xml_success = read_xml_success ? IntoXMLSubNode(xml, "DebitResultContext") : false;
    read_xml_success = read_xml_success ? IntoXMLSubNode(xml, "Data") : false;
    read_xml_success = read_xml_success ? ReadXMLInt(xml, "ResultType", dp.ResultType) : false;
    read_xml_success = read_xml_success ? ReadXMLInt(xml, "ExecuteCode", dp.ExecuteCode) : false;
    read_xml_success = read_xml_success ? ReadXMLCStr(xml, "ExecuteDesc", dp.ExecuteDesc, sizeof(dp.ExecuteDesc)) : false;
    read_xml_success = read_xml_success ? IntoXMLSubNode(xml, "TradeKeyItem") : false;
    read_xml_success = read_xml_success ? ReadXMLCStr(xml, "TradeSecurityCode", dp.TradeSecurityCode, sizeof(dp.TradeSecurityCode)) : false;
    read_xml_success = read_xml_success ? ReadXMLInt(xml, "PayPlatformType", dp.ServerPayPlatformType) : false;
    read_xml_success = read_xml_success ? ReadXMLCStr(xml, "PayIdentifier", dp.PayIdentifier, sizeof(dp.PayIdentifier)) : false;
    read_xml_success = read_xml_success ? ReadXMLCStr(xml, "DebitOrder", dp.DebitOrderNumber, sizeof(dp.DebitOrderNumber)) : false;
    read_xml_success = read_xml_success ? ReadXMLTimet(xml, "DebitTime", dp.ServerDebitTime) : false;

    if (!read_xml_success)
    {
        TW_TRACE_DIARY_TYPE("解析代扣结果失败:" + GetXMLParseError(), TW_TRACE_TPPE_PAYUNIT);
        return false;
    }
    dp.ResultType = 0;
    dp.ExecuteCode = 0;
    dp.PayPlatformType = dp.ServerPayPlatformType;
    return true;
}


int PackDebitHttpJson(const DebitParameter& dp, std::string& json_str)
{
    Json::Value root;
    Json::Value DebitCondition;
    Json::Value DebitConditionBase;
    Json::Value DebitConditionAddtional;
    Json::Value AddtionalEntryStation;
    Json::Value AddtionalPayStation;
    Json::Value AddtionalVehicleInfo;
    Json::Value DebitConditionNFCPayInfo;
    /***********************************************************构造JSAO你数据结构***********************************************************/

//标识版本
    root["Version"]             = Json::Value(dp.Version);
//标识数据模式(正式数据或测试数据)
    root["IsTestData"]          = Json::Value(0);
//标识数据类型
    root["PackageType"]         = Json::Value(3);
//安全认证
    root["Token"]               = Json::Value("");
//新增序列号
    root["SN"]                  = Json::Value(dp.DealSN );
//标识省份
    root["ProvinceID"]          = Json::Value(dp.LaneConfig.ProvinceID);
//递交次数
    root["SubmitCount"]         = Json::Value(1);
//递交时间
    root["SubmitTime"]          = Json::Value(ChinaTimeStrWithMS());
//交易标记
    root["RecordMatchCode"]     = Json::Value(dp.NetPayRecordMatchCode);
//扣款凭证
    root["PayIdentifier"]       = Json::Value(dp.PayIdentifier);
//交易安全认证码
    root["TradeSecurityCode"]   = Json::Value("");

//是否测试数据
    root["TestingData"]         = Json::Value(0);
//业务动态库版本号
    root["NetPayDllVersion"]    = Json::Value(dp.NetPayDllVersion);
//设备动态库版本号
    root["DeviceDllVersion"]    = Json::Value(dp.DeviceDllVersion);
//用户数据数据来源
    root["AccountInfoSrcType"]  = Json::Value(dp.AccountInfoSrcType);
//用户数据具体设备号
    root["AccountInfoDeviceID"] = Json::Value(dp.AccountInfoDeviceID);
//用户数据具体类型
    root["AccountInfoDataType"] = Json::Value(dp.AccountInfoDataType);
//设备唯一编码
    root["DeviceSerialNo"]      = Json::Value(dp.TWProductSerialNo);
//全字段加密结果
    root["EnDataString"]        = Json::Value("");

//填充基本信息
    DebitConditionBase["ChannelUserId"]         = Json::Value("");
    DebitConditionBase["ChannelType"]           = Json::Value(0);
    DebitConditionBase["PayPlatformQRCode"]     = Json::Value(dp.QRCode);
    DebitConditionBase["VirtualCardID"]         = Json::Value("");
    DebitConditionBase["EntryInfoUniqueFlag"]   = Json::Value("");
    DebitConditionBase["DebitMoney"]            = Json::Value(dp.Money);
    DebitConditionBase["DebitType"]             = Json::Value(dp.DebitType);
    DebitConditionBase["DebitTime"]             = Json::Value(Datetime2Str(dp.SubTime));
    DebitConditionBase["TollDate"]              = Json::Value(dp.TollDate);
    DebitConditionBase["TollShift"]             = Json::Value(dp.ShiftID);

//add 2018-03
    DebitConditionBase["PassCertificateType"]   = Json::Value(dp.PassCertificateType);
    DebitConditionBase["TicketNo"]  = Json::Value(dp.TicketNo);
    DebitConditionBase["Distance"]  = Json::Value(dp.Distance);

//填充附加信息 -- 入站信息
    AddtionalEntryStation["EntryProvinceId"]    = Json::Value(dp.LaneConfig.ProvinceID);
    AddtionalEntryStation["EntryStationId"]     = Json::Value(dp.EntryStationID);
    AddtionalEntryStation["EntryStationName"]   = Json::Value(dp.EntryStationName);
    AddtionalEntryStation["EntryLane"]          = Json::Value(dp.EntryLaneID);
    AddtionalEntryStation["EntryTime"]          = Json::Value(ChinaTime2Str(dp.EntryTime));
    AddtionalEntryStation["EntryRoadNo"]        = Json::Value(dp.EntryRoadID);
    AddtionalEntryStation["EntryAreaNo"]        = Json::Value(dp.EntryAreaID);

//add 2018-03
    AddtionalEntryStation["EntryLicense"]       = Json::Value(dp.EntryVehicleLicense);
    AddtionalEntryStation["EntryVColor"]        = Json::Value(dp.EntryVLColor);
    AddtionalEntryStation["EntryVClass"]        = Json::Value(dp.EntryVehicleClass);
    AddtionalEntryStation["EntryVType"]     = Json::Value(dp.EntryVehicleType);


//填充附加信息 -- 扣费站信息
    AddtionalPayStation["PayProvinceId"]        = Json::Value(dp.LaneConfig.ProvinceID);
    AddtionalPayStation["PayStationId"]         = Json::Value(dp.LaneConfig.StationID);
    AddtionalPayStation["PayStationName"]       = Json::Value(dp.LaneConfig.StationName);
    AddtionalPayStation["PayLane"]              = Json::Value(dp.LaneConfig.LaneID);
    AddtionalPayStation["PayRoadNo"]            = Json::Value(dp.LaneConfig.RoadID);
    AddtionalPayStation["PayOperatorId"]        = Json::Value(dp.OperatorID);
    AddtionalPayStation["PayOperatorName"]      = Json::Value(dp.OperatorName);

//填充附加信息 -- 扣费车辆信息
    AddtionalVehicleInfo["VehicleClass"]        = Json::Value(dp.VehicleClass);
    AddtionalVehicleInfo["VehicleType"]         = Json::Value(dp.VehicleType);
    AddtionalVehicleInfo["VehicleLicense"]      = Json::Value(dp.VehicleLicense);
    AddtionalVehicleInfo["VehicleColor"]        = Json::Value(dp.VLColor);
    AddtionalVehicleInfo["AxisCount"]           = Json::Value(dp.AxisCount);
    AddtionalVehicleInfo["TotalWeight"]         = Json::Value(dp.Weight);
    AddtionalVehicleInfo["LimitWeight"]         = Json::Value(dp.LimitWeight);

//add 2018-04
    AddtionalVehicleInfo["ImageID"] = Json::Value(dp.ImageId);
//add 2018-03
    AddtionalVehicleInfo["AutoColor"]           = Json::Value(dp.AutoColor);
    AddtionalVehicleInfo["AutoLicense"]         = Json::Value(dp.AutoLicense);
//填充银联NFC支付信息

    DebitConditionNFCPayInfo["Terminal"]    = Json::Value(GetNFCTerminal());
    DebitConditionNFCPayInfo["Sn"]          = Json::Value(GetNFCSerialNo());


    DebitConditionNFCPayInfo["PayData"]     = Json::Value(HexToBase64(dp.NFCPayData));

    DebitConditionNFCPayInfo["RevData"]     = Json::Value(HexToBase64(dp.NFCRevData));

//格式化
    DebitConditionAddtional["EntryStation"] = AddtionalEntryStation;
    DebitConditionAddtional["PayStation"]   = AddtionalPayStation;
    DebitConditionAddtional["VehicleInfo"]  = AddtionalVehicleInfo;

    DebitCondition["NFCPayInfo"]    = DebitConditionNFCPayInfo;
    DebitCondition["Base"]          = DebitConditionBase;
    DebitCondition["Addtional"]     = DebitConditionAddtional;

    root["Data"] = DebitCondition;

//转化为字符串
    Json::FastWriter fast_writer;
    json_str = fast_writer.write(root);

    return 0;
}


bool ParseDebitResultFromJson(const std::string& json_para, DebitResultParameter& dp)
{
    bool read_json_success = true;
    Json::Reader json_reader;
    Json::Value  root_node;
    if (!json_reader.parse(json_para, root_node))
    {
        TW_TRACE_DIARY_TYPE("支付回馈Json格式存在问题", TW_TRACE_TPPE_PAYUNIT);
        return false;
    }
    ReadJsonChars(root_node, "Version", dp.Version, sizeof(dp.Version));
    ReadJsonInt(root_node, "PackageType", dp.PackageType);
    ReadJsonInt(root_node, "Status", dp.ServerResponseCode);
    ReadJsonChars(root_node, "Message", dp.ServerResponseResultDesc, sizeof(dp.ServerResponseResultDesc));
    ReadJsonTimet(root_node, "ReceveTime", dp.ServerReceiveDataTime);
    ReadJsonTimet(root_node, "ResponseTime", dp.ServerResponseDataTime);
    ReadJsonTimet(root_node, "ServerDebitTime", dp.ServerDebitTime);

    ReadJsonChars(root_node, "ErrorDescByPayPlatform", dp.DebitStatusDesc, sizeof(dp.DebitStatusDesc));
    ReadJsonChars(root_node, "DebitOrderNumber", dp.DebitOrderNumber, sizeof(dp.DebitOrderNumber));
    ReadJsonChars(root_node, "TradeSecurityCode", dp.TradeSecurityCode, sizeof(dp.TradeSecurityCode));
    ReadJsonChars(root_node, "PayIdentifier", dp.PayIdentifier, sizeof(dp.PayIdentifier));
    ReadJsonChars(root_node, "RecordMatchCode", dp.ServerRecordMatchCode, sizeof(dp.ServerRecordMatchCode));
    std::string virtual_id ;
    ReadJsonStr(root_node, "PayVirtualCardID", virtual_id);
    if (!virtual_id.empty())
    {
        _snprintf(dp.PayVirtualCardID, sizeof(dp.PayVirtualCardID) - 1, "%s%s", GetGlobalApp()->m_strVirtualCardPrefix, virtual_id.c_str());
    }
    ReadJsonInt(root_node, "PayChannelType", dp.ServerPayPlatformType);

    strncpy(dp.TradeSecurityCode, dp.ServerRecordMatchCode, sizeof(dp.TradeSecurityCode) - 1);
    return true;
}


std::map<int, int>  ServerPayCodeCheckList()
{
    std::map<int, int > code_map;
    code_map[HTTP_RC_DEBIT_NEED_PASSWORD] = RCC_INPUT_PASSWORD;
    code_map[HTTP_RC_DEBIT_INSUFFICIENT_BALANCE] = RCC_NO_MONEY;
    code_map[HTTP_RC_DEBIT_SRC_ILLGAL] = RCC_QRCODE_ILLEGAL;
    code_map[HTTP_RC_DEBIT_CODE_TIMEOUT] = RCC_QRCODE_OT;
    code_map[HTTP_RC_INPUT_PASSWORD_ERROR] = RCC_PSW_ERROR;
    code_map[HTTP_RC_NO_SERVICES] = RCC_INNER_ERROR;
    code_map[HTTP_RC_NFC_NO_SUPPORT] = RCC_INNER_ERROR;
    code_map[HTTP_RC_NFC_NO_INIT] = RCC_INNER_ERROR;
    code_map[HTTP_RC_NFC_NO_SERVICES] = RCC_INNER_ERROR;
    code_map[HTTP_RC_DEBIT_QUERY_ERROR] = RCC_DEBIT_QUERY_ERROR;
    code_map[HTTP_RC_DEBIT_FAIL] = RCC_INNER_ERROR;
    code_map[HTTP_RC_DEBIT_SUCCESS] = RCC_OK;
    return code_map;
}


int ServerPayCode2LocalCode(int server_code)
{
    static std::map<int , int> pay_code_dict =  ServerPayCodeCheckList();
    std::map<int, int>::const_iterator it = pay_code_dict.find(server_code);
    if (it != pay_code_dict.end())
    {
        return it->second;
    }
    return RCC_INNER_ERROR;
}

std::map<int, int> ServerPayCodeToFailHintCode()
{
    std::map<int, int> fail_code_map;
    fail_code_map[HTTP_RC_DEBIT_NEED_PASSWORD]        = SM_PRPTOCOL_LIGHT_FALI_PSW_INPUT;
    fail_code_map[HTTP_RC_DEBIT_INSUFFICIENT_BALANCE] = SM_PRPTOCOL_LIGHT_FALI_BL_ERR;
    fail_code_map[HTTP_RC_DEBIT_SRC_ILLGAL]           = SM_PRPTOCOL_LIGHT_FAIL_QR_ERR1;
    fail_code_map[HTTP_RC_DEBIT_CODE_TIMEOUT]         = SM_PRPTOCOL_LIGHT_FAIL_QR_ERR2;
    fail_code_map[HTTP_RC_INPUT_PASSWORD_ERROR]       = SM_PRPTOCOL_LIGHT_FAIL_PSW_ERR;
    fail_code_map[HTTP_RC_NO_SERVICES]                = SM_PRPTOCOL_LIGHT_FALI_PL_STOP;
    fail_code_map[HTTP_RC_NFC_NO_SUPPORT]             = SM_PRPTOCOL_LIGHT_FALI_NFC_01;
    fail_code_map[HTTP_RC_NFC_NO_INIT]                = SM_PRPTOCOL_LIGHT_FALI_NFC_02;
    fail_code_map[HTTP_RC_NFC_NO_SERVICES]            = SM_PRPTOCOL_LIGHT_FALI_NFC_03;
    return fail_code_map;
}

int GetPayFailHintCodeAccordingToServerPayCode(int server_code)
{
    static std::map<int, int> code_map = ServerPayCodeToFailHintCode();
    int     iMMIDetailType = SM_PRPTOCOL_LIGHT_FAIL;
    if (!IsUseExternMMI())
    {
        return iMMIDetailType;
    }
    std::map<int, int>::const_iterator it = code_map.find(server_code);
    if (it == code_map.end())
    {
        return iMMIDetailType;
    }
    return it->second;
}


const std::string GetNetPayRecordMatchCode(const DebitParameter& dp )
{
    CString strMatchCode;
    //校验密钥
    CString strRealKey;
    if (strRealKey.IsEmpty())
    {
        switch (dp.LaneConfig.ProvinceID)
        {
        case 11:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_11);
            break;
        case 12:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_12);
            break;
        case 13:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_13);
            break;
        case 14:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_14);
            break;
        case 15:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_15);
            break;

        case 31:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_31);
            break;
        case 32:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_32);
            break;
        case 33:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_33);
            break;
        case 34:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_34);
            break;
        case 35:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_35);
            break;
        case 36:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_36);
            break;
        case 37:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_37);
            break;

        case 41:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_41);
            break;
        case 42:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_42);
            break;
        case 43:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_43);
            break;
        case 44:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_44);
            break;
        case 45:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_45);
            break;

        case 50:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_50);
            break;
        case 51:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_51);
            break;
        case 52:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_52);
            break;
        case 53:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_53);
            break;
        case 54:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_54);
            break;

        case 61:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_61);
            break;
        case 62:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_62);
            break;
        case 63:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_63);
            break;
        case 64:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_64);
            break;
        case 65:
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA_65);
            break;
        default:
            //默认密钥
            strRealKey.Format(_T("%s"), AES_ENC_KEY_DATA);
            break;
        }
    }
    /**************************构造加密明文**************************/
    CString strData;
    //入口站点
    CString strEntryST;
    CString strTempEnST(_T("00000000000000000000"));
    int iSTLen = static_cast<int>(strlen(dp.EntryStationID));
    if (63 == dp.LaneConfig.ProvinceID)
    {
        if (iSTLen < 7)
        {
            strEntryST.Format(_T("%s%s"), strTempEnST.Left(7 - iSTLen), dp.EntryStationID);
        }
        else
        {
            strEntryST.Format(_T("%s"), dp.EntryStationID);
        }
    }
    else
    {
        if (iSTLen < 10)
        {
            strEntryST.Format(_T("%s%s"), strTempEnST.Left(10 - iSTLen), dp.EntryStationID);
        }
        else
        {
            strEntryST.Format(_T("%s%s"), strTempEnST.Left(20 - iSTLen), dp.EntryStationID);
        }
    }

    //入口时间
    CString strEntryTime;
    CTime cEntryTime(dp.EntryTime);
    strEntryTime.Format(_T("%04d%02d%02d%02d%02d%02d"), cEntryTime.GetYear(), cEntryTime.GetMonth(), cEntryTime.GetDay(), cEntryTime.GetHour(), cEntryTime.GetMinute(), cEntryTime.GetSecond());

    //出口站点
    CString strExitST;
    iSTLen = _tcslen(dp.LaneConfig.StationID);
    if (63 == dp.LaneConfig.ProvinceID)
    {
        if (iSTLen < 7)
        {
            strExitST.Format(_T("%s%s"), strTempEnST.Left(7 - iSTLen), dp.LaneConfig.StationID);
        }
        else
        {
            strExitST.Format(_T("%s"), dp.LaneConfig.StationID);
        }
    }
    else
    {
        if (iSTLen < 10)
        {
            strExitST.Format(_T("%s%s"), strTempEnST.Left(10 - iSTLen), dp.LaneConfig.StationID);
        }
        else
        {
            strExitST.Format(_T("%s%s"), strTempEnST.Left(20 - iSTLen), dp.LaneConfig.StationID);
        }
    }

    //出口时间
    CString strExitTime;
    CTime cExitTime(dp.SubTime);
    strExitTime.Format(_T("%04d%02d%02d%02d%02d%02d"), cExitTime.GetYear(), cExitTime.GetMonth(), cExitTime.GetDay(), cExitTime.GetHour(), cExitTime.GetMinute(), cExitTime.GetSecond());

    //出口车道
    CString strExitLane;
    int iLaneLen = strlen(dp.LaneConfig.LaneID);//_tcslen(dp.LaneConfig.LaneID);
    if (   (44 == dp.LaneConfig.ProvinceID)
            || (61 == dp.LaneConfig.ProvinceID)
            || (62 == dp.LaneConfig.ProvinceID)
            || (63 == dp.LaneConfig.ProvinceID)
       )
    {
        if (iLaneLen < 3)
        {
            strExitLane.Format(_T("%s%s"), strTempEnST.Left(3 - iLaneLen), dp.LaneConfig.LaneID);
        }
        else
        {
            strExitLane.Format(_T("%s"), dp.LaneConfig.LaneID);
        }
    }
    else
    {
        if (iLaneLen < 8)
        {
            strExitLane.Format(_T("%s%s"), strTempEnST.Left(8 - iLaneLen), dp.LaneConfig.LaneID);
        }
        else
        {
            strExitLane.Format(_T("%s"), dp.LaneConfig.LaneID);
        }
    }

    //虚拟卡号
    CString strVirtualCardID;

    if (   (44 == dp.LaneConfig.ProvinceID)
            || (62 == dp.LaneConfig.ProvinceID)
       )
    {
        strVirtualCardID.Format(_T("%s"), _T("00000000000000000000"));
    }
    else
    {
        if (strlen(dp.CardID) == 0) //GetGlobalApp()->m_pCurrentDebitInfo.CardID.IsEmpty())
        {
            strVirtualCardID.Format(_T("%s"), _T("00000000000000000000"));
        }
        else
        {
            CString strCardTemp;
            strCardTemp.Format(_T("%s"), _T("00000000000000000000"));
            if ( strlen(dp.CardID) < 20)
            {
                int iRemainCount = 20 - strlen(dp.CardID);
                strVirtualCardID.Format(_T("%s%s"), strCardTemp.Left(iRemainCount), dp.CardID);
            }
            else
            {
                CString strVT(dp.CardID);
                strVT.TrimLeft();
                strVT.TrimRight();
                strVirtualCardID = strVT;
            }
        }
    }

    //交易金额
    CString strTradeMoney;
    strTradeMoney.Format(_T("%010d"), dp.Money);

    strData.Format(_T("%s%s%s%s%s%s%s"), strEntryST, strEntryTime, strExitST, strExitTime, strExitLane, strVirtualCardID, strTradeMoney);


    /**************************加密处理**************************/

    //AES加密
    CAesECB cAesECB(strRealKey);
    CString strEncData = cAesECB.EncryptData(strData);

    //MD5校验
    CMD5 md5((LPCSTR)strEncData);
    strMatchCode = md5.GetMD5();
    //theApp.RecordLog("Enc:"+strEncData);
    //theApp.RecordLog("md5:"+strMatchCode);
    //返回匹配码
    std::string match_str = (LPCSTR)strMatchCode;
    return match_str;
}

bool ParseImageTransferParaFromXML(const std::string xml_para, std::string& xml_result)
{
    CMarkup xml;
    if (!xml.SetDoc(xml_para.c_str()))
    {
        TW_TRACE_DIARY("图像传输,XML格式错误:" + xml_para);
        return false;
    }
    int image_type;
    int data_format;
    std::string pay_id;
    std::string image_data;

    bool read_xml_success = true;
    read_xml_success = read_xml_success ? IntoXMLSubNode(xml, "DebitImage") : false;
    read_xml_success = read_xml_success ? IntoXMLSubNode(xml, "Data") : false;
    read_xml_success = read_xml_success ? ReadXMLInt(xml, "Type", image_type) : false;
    read_xml_success = read_xml_success ? ReadXMLInt(xml, "DataFormat", data_format) : false;
    read_xml_success = read_xml_success ? ReadXMLStr(xml, "PayIdentifier", pay_id) : false;
    read_xml_success = read_xml_success ? ReadXMLStr(xml, "Image", image_data) : false;
    if (!read_xml_success)
    {
        TW_TRACE_DIARY("图像传输,XML解析错误:" + GetXMLParseError());
        return false;
    }
    CMarkup xmlImage;
    xmlImage.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
    xmlImage.AddElem( _T("AgencyImageContext") );//root
    xmlImage.IntoElem();

    xmlImage.AddElem( _T("Version"),        _T("1.0") );
    xmlImage.AddElem( _T("TimeOut"),        5000 );

    xmlImage.AddElem( _T("Data") );
    xmlImage.IntoElem();
    xmlImage.AddElem( _T("PayIdentifier"),  pay_id.c_str() );
    xmlImage.AddElem( _T("ImageType"),      image_type );

    xmlImage.AddElem( _T("ItemData") );
    xmlImage.IntoElem();
    xmlImage.AddElem( _T("Item") );
    xmlImage.IntoElem();

    xmlImage.AddElem( _T("Ordinal"),        1 );

    CString strImgType(_T("Hex"));
    switch (data_format)
    {
    case 1:
        strImgType.Format(_T("Hex"));
        break;
    case 2:
        strImgType.Format(_T("Base64"));
        break;
    }
    xmlImage.AddElem( _T("Type"),           strImgType );
    xmlImage.AddElem( _T("Image"),          image_data.c_str() );
    xmlImage.AddElem( _T("MD5"),            _T("No MD5") );

    xmlImage.OutOfElem();
    xmlImage.OutOfElem();
    xmlImage.OutOfElem();

    xml_result = (LPCSTR)xmlImage.GetDoc();
    return true;

}
bool ParseImageTransferParaFromJson(const std::string json_para, std::string& xml_result)
{
    Json::Reader reader;
    Json::Value  root_node;
    if ( !reader.parse( json_para, root_node) )
    {
        TW_TRACE_DIARY("图像传输,JSON解析错误" + json_para);
        return false;
    }
    Json::Value debit_image_node, data_node;
    int image_type, data_format;
    std::string pay_id, image_data;
    bool read_success = true;
    read_success = read_success ? ReadJsonSubNode(root_node, "DebitImage", debit_image_node) : false;
    read_success = read_success ? ReadJsonSubNode(debit_image_node, "Data", data_node) : false;
    read_success = read_success ? ReadJsonInt(data_node, "Type", image_type) : false;
    read_success = read_success ? ReadJsonInt(data_node, "DataFormat", data_format) : false;
    read_success = read_success ? ReadJsonStr(data_node, "PayIdentifier", pay_id) : false;
    read_success = read_success ? ReadJsonStr(data_node, "Image", image_data) : false;

    if (!read_success)
    {
        TW_TRACE_DIARY("图像传输,JSON解析错误:" + GetJsonParseError());
        return false;
    }
    CMarkup xmlImage;
    xmlImage.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );
    xmlImage.AddElem( _T("AgencyImageContext") );//root
    xmlImage.IntoElem();

    xmlImage.AddElem( _T("Version"),        _T("1.0") );
    xmlImage.AddElem( _T("TimeOut"),        5000 );

    xmlImage.AddElem( _T("Data") );
    xmlImage.IntoElem();
    xmlImage.AddElem( _T("PayIdentifier"),  pay_id.c_str() );
    xmlImage.AddElem( _T("ImageType"),      image_type );

    xmlImage.AddElem( _T("ItemData") );
    xmlImage.IntoElem();
    xmlImage.AddElem( _T("Item") );
    xmlImage.IntoElem();

    xmlImage.AddElem( _T("Ordinal"),        1 );

    CString strImgType(_T("Hex"));
    switch (data_format)
    {
    case 1:
        strImgType.Format(_T("Hex"));
        break;
    case 2:
        strImgType.Format(_T("Base64"));
        break;
    }
    xmlImage.AddElem( _T("Type"),           strImgType );
    xmlImage.AddElem( _T("Image"),          image_data.c_str() );
    xmlImage.AddElem( _T("MD5"),            _T("No MD5") );

    xmlImage.OutOfElem();
    xmlImage.OutOfElem();
    xmlImage.OutOfElem();

    xml_result = (LPCSTR)xmlImage.GetDoc();
    return true;
}

//////////////////////////////////////////////////////////////
// 函数名称:PackDebitCancleHttpJson
//
// 功能描述:撤单请求包
//
// 输入参数:const DebitCancleParameter& dp, std::string json_str,
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2019-01-17
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:改自原1.5相关代码
//
//////////////////////////////////////////////////////////////
int PackDebitCancleHttpJson(const DebitCancleParameter& dp, std::string& json_str)
{
    // 表示整个 json 对象
    Json::Value root;
    // 临时JSON对象
    Json::Value DebitCancelCondition;

    Json::Value DebitCancelBase;
    /***********************************************************构造JSAO数据结构***********************************************************/

    //标识版本
    root["Version"]             = Json::Value("1.0");
    //标识数据类型
    root["PackageType"]         = Json::Value(JSON_DT_SEND_DIBIT_CANCEL);
    //安全认证
    root["Token"]               = Json::Value("");
    //标识省份
    root["ProvinceID"]          = Json::Value(dp.ProvinceID);
    //递交次数
    root["SubmitCount"]         = Json::Value(0);
    //递交时间
    root["SubmitTime"]          = Json::Value(ChinaTimeStrWithMS());
    //扣款凭证
    root["PayIdentifier"]       = Json::Value(dp.PayIdentifier);
	//当前车道信息
	char LaneInfo[100] = {0};
	if((strlen(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_RoadID) 
	   + strlen(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_StationID) 
	   + strlen(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationTab) 
	   + strlen(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_LaneID)) < 96)
	{
		_snprintf(LaneInfo, 100, "%s-%s-%s-%s", GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_RoadID, 
												GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_StationID, 
												GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationTab, 
												GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_LaneID);
	}
	root["LaneInfo"]            = Json::Value(LaneInfo);

    //转化为字符串
    Json::FastWriter fast_writer;
    json_str = fast_writer.write(root).c_str();
    return 0;
}

bool ParseDebitCancleResultFromServer(const std::string json_para, DebitCancleResultParameter& cancle_result)
{
    bool read_json_success = true;
    Json::Reader json_reader;
    Json::Value  root_node;
    if (!json_reader.parse(json_para, root_node))
    {
        TW_TRACE_DIARY_TYPE("撤单回馈Json格式存在问题" , TW_TRACE_TPPE_PAYUNIT);
        return false;
    }
    read_json_success = read_json_success ? ReadJsonInt(root_node, "Status", cancle_result.Status) : false;
    read_json_success = read_json_success ? ReadJsonChars(root_node, "PayIdentifier", cancle_result.PayIdentifier, sizeof(cancle_result.PayIdentifier)) : false;
    if (!read_json_success)
    {
        TW_TRACE_DIARY_TYPE("撤单回馈Json解析错误:" + GetJsonParseError(), TW_TRACE_TPPE_PAYUNIT);
        return false;
    }
    ReadJsonChars(root_node, "Version", cancle_result.Version, sizeof(cancle_result.Version));
    ReadJsonChars(root_node, "DebitOrderNumber", cancle_result.DebitOrderNumber, sizeof(cancle_result.DebitOrderNumber));
    ReadJsonChars(root_node, "RecordMatchCode", cancle_result.RecordMatchCode, sizeof(cancle_result.RecordMatchCode));
    ReadJsonInt(root_node, "PackageType", cancle_result.PackageType);

    return true;

}


//////////////////////////////////////////////////////////////
// 函数名称:PackDebitCheckHttpJson
//
// 功能描述:生成扣费查询字符串
//
// 输入参数:const DebitParameter& dp,std::string& json_str,
// 输出参数:int
// 编写人员:朱亚冰
// 编写时间:2019-01-18
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
// 参数构造，使用了原有代码内容。
//
//////////////////////////////////////////////////////////////
int PackDebitCheckHttpJson(const DebitParameter& dp, std::string& json_str)
{
    // 表示整个 json 对象
    Json::Value root;
    // 临时JSON对象
    Json::Value DebitCheckCondition;

    Json::Value DebitCheckBase;
    /***********************************************************构造JSAO你数据结构***********************************************************/

    //标识版本
    root["Version"]             = Json::Value("1.5");
    //标识数据类型
    root["PackageType"]         = Json::Value(JSON_DT_SEND_DIBIT_CHECK);
    //安全认证
    root["Token"]               = Json::Value("");

    //新增序列号
	root["SN"]                  = Json::Value( dp.DealSN );

    //标识省份
    root["ProvinceID"]          = Json::Value(dp.LaneConfig.ProvinceID);
    //递交次数
    root["SubmitCount"]         = Json::Value(0);
    //递交时间
    root["SubmitTime"]          = Json::Value(ChinaTimeStrWithMS());
    //扣款凭证
    root["PayIdentifier"]       = Json::Value(dp.PayIdentifier);

    ////填充请求信息
    //DebitCheckBase["ChannelUserId"]       = Json::Value(pDVJsonDebitCheck->IDBindInPayPlatform);
    //DebitCheckBase["ChannelType"]     = Json::Value(pDVJsonDebitCheck->IDBindInPayPlatformType);
    //DebitCheckBase["PayPlatformQRCode"]   = Json::Value(pDVJsonDebitCheck->QRCodeData);
    //DebitCheckBase["PayIdentifier"]   = Json::Value(pDVJsonDebitCheck->PayIdentifier);
    //DebitCheckBase["DebitMoney"]      = Json::Value(pDVJsonDebitCheck->DebitMoney);
    //DebitCheckBase["DebitType"]           = Json::Value(pDVJsonDebitCheck->DebitType);
    //DebitCheckBase["DebitTime"]           = Json::Value(pDVJsonDebitCheck->DebitTime.Format(_T("%Y-%m-%d %H:%M:%S")));

    ////格式化
    //DebitCheckCondition["Base"]   = DebitCheckBase;
    //root["Data"] = DebitCheckCondition;

    //转化为字符串
    Json::FastWriter fast_writer;
    json_str = fast_writer.write(root);
    return 0;
}


int PackInvoiceHttpJson(const InvoiceParameter& param, std::string& json_str)
{
	Json::Value root;
    Json::Value ElectronicInvoiceStart;
    Json::Value Data;

	Data["ElectronicInvoiceNo"] = Json::Value(param.ElectronicInvoiceNo);
	Data["DebitOrderNumber"] = Json::Value(param.DebitOrderNumber);
	Data["PayMode"] = Json::Value(param.PayMode);
	Data["Money"] = Json::Value(param.Money);
	Data["VehicleLicense"] = Json::Value(param.VehicleLicense);
	Data["VLColor"] = Json::Value(param.VehicleColor);
	Data["VehicleType"] = Json::Value(param.VehicleType);
	Data["VehicleClass"] = Json::Value(param.VehicleClass);
	Data["AxisCount"] = Json::Value(param.AxisCount);
	Data["Weight"] = Json::Value(param.TotalWeight);
	Data["LimitWeight"] = Json::Value(param.LimitWeight);
	Data["PayAreaID"] = Json::Value(param.PayAreaID);
	Data["PayProvinceId"] = Json::Value(param.PayProvinceId);
	Data["PayRoadID"] = Json::Value(param.PayRoadID);
	Data["PayStationID"] = Json::Value(param.PayStationID);
	Data["PayStationName"] = Json::Value(param.PayStationName);
	Data["PayLaneID"] = Json::Value(param.PayLane);
	Data["ExitTime"] = Json::Value(param.ExitTime);
	Data["PassCertificateType"] = Json::Value(param.PassCertificateType);
	Data["PassCertificateCode"] = Json::Value(param.PassCertificateCode);
	Data["TollDate"] = Json::Value(param.TollDate);
	Data["ShiftID"] = Json::Value(param.ShiftID);
	Data["OperatorID"] = Json::Value(param.OperatorID);
	Data["OperatorName"] = Json::Value(param.OperatorName);
	Data["Distance"] = Json::Value(param.Distance);
	Data["EntryAreaID"] = Json::Value(param.EntryAreaID);
	Data["EntryRoadID"] = Json::Value(param.EntryRoadID);
	Data["EntryStationID"] = Json::Value(param.EntryStationID);
	Data["EntryStationName"] = Json::Value(param.EntryStationName);
	Data["EntryLaneID"] = Json::Value(param.EntryLaneID);
	Data["EntryTime"] = Json::Value(param.EntryTime);
	Data["EntryVehicleLicense"] = Json::Value(param.EntryVehicleLicense);
	Data["EntryVLColor"] = Json::Value(param.EntryVLColor);
	Data["EntryVehicleClass"] = Json::Value(param.EntryVehicleClass);
	Data["EntryVehicleType"] = Json::Value(param.EntryVehicleType);
	Data["QRcode"] = Json::Value(param.QRcode);
	ElectronicInvoiceStart["Data"] = Data;

	root["ElectronicInvoiceStart"] = ElectronicInvoiceStart;

	//转化为字符串
    Json::FastWriter fast_writer;
    json_str = fast_writer.write(root);
    return 0;
}

int PackInvoiceCancelHttpJson(const InvoiceCancelParameter& param, std::string& json_str)
{
	Json::Value root;
    Json::Value ElectronicInvoiceDelete;

	ElectronicInvoiceDelete["ElectronicInvoiceNo"] = Json::Value(param.ElectronicInvoiceNo);
	ElectronicInvoiceDelete["PayProvinceId"] = Json::Value(param.PayProvinceId);
	ElectronicInvoiceDelete["ExitTime"] = Json::Value(param.ExitTime);
	root["ElectronicInvoiceDelete"] = ElectronicInvoiceDelete;

	//转化为字符串
    Json::FastWriter fast_writer;
    json_str = fast_writer.write(root);
    return 0;
}


bool ParseInvoiceResultFromJson(const std::string& json_para, InvoiceResultParameter& param)
{
    Json::Reader json_reader;
    Json::Value root_node, ElectronicInvoiceResp_node;
    if(!json_reader.parse(json_para, root_node))
    {
        return false;
    }
    bool read_success = true;
    read_success = read_success ? ReadJsonSubNode(root_node, "ElectronicInvoiceResp", ElectronicInvoiceResp_node) : false;
    read_success = read_success ? ReadJsonChars(ElectronicInvoiceResp_node, "ElectronicInvoiceNo", param.ElectronicInvoiceNo, sizeof(param.ElectronicInvoiceNo)) : false;
    read_success = read_success ? ReadJsonInt(ElectronicInvoiceResp_node, "ExecuteCode", param.ExecuteCode) : false;
    if (!read_success)
    {
        TW_TRACE_DIARY("传送发票应答Json解析错误:" + GetJsonParseError());
        return false;
    }
    return true;
}

bool ParseInvoiceCancleResultFromServer(const std::string& json_para, InvoiceCancelResultParameter& param)
{
    Json::Reader json_reader;
    Json::Value root_node, ElectronicInvoiceDeleteResp_node;
    if(!json_reader.parse(json_para, root_node))
    {
        return false;
    }
    bool read_success = true;
    read_success = read_success ? ReadJsonSubNode(root_node, "ElectronicInvoiceDeleteResp", ElectronicInvoiceDeleteResp_node) : false;
    read_success = read_success ? ReadJsonChars(ElectronicInvoiceDeleteResp_node, "ElectronicInvoiceNo", param.ElectronicInvoiceNo, sizeof(param.ElectronicInvoiceNo)) : false;
    read_success = read_success ? ReadJsonInt(ElectronicInvoiceDeleteResp_node, "ExecuteCode", param.ExecuteCode) : false;
	read_success = read_success ? ReadJsonChars(ElectronicInvoiceDeleteResp_node, "DealTime", param.DealTime, sizeof(param.DealTime)) : false;
    if (!read_success)
    {
        TW_TRACE_DIARY("撤销流水数据应答Json解析错误:" + GetJsonParseError());
        return false;
    }
    return true;
}



