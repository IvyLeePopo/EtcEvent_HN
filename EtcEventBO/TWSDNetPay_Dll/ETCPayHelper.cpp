#include "stdafx.h"
#include "ETCPayHelper.h"
#include "TWSDNetPay_Dll.h"
#include "XMLHelper.h"
#include "JsonHelper.h"
#include "pystring.h"
#include "RunParameter.h"
#include <sstream>

//////////////////////////////////////////////////////////////
// 函数名称:ParseWhiteListQueryParaFromXML
//
// 功能描述:解析白名单鉴权参数
//
// 输入参数:const std::string& xml_para, ObuWhiteListQueryParameter& query_info,
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ParseWhiteListQueryParaFromXML(const std::string& xml_para, ObuWhiteListQueryParameter& query_info)
{
    CMarkup xml;
    xml.SetDoc(xml_para.c_str());
    xml.ResetMainPos();
    bool read_success = true;
    read_success = read_success ? IntoXMLSubNode(xml, "ETCDebitListQuery") : false;
    read_success = read_success ? ReadXMLCStr(xml, "Version", query_info.Version, sizeof(query_info.Version)) : false;
    read_success = read_success ? IntoXMLSubNode(xml, "Data") : false;
    read_success = read_success ? IntoXMLSubNode(xml, "TradeKeyItem") : false;
    read_success = read_success ? ReadXMLCStr(xml, "PayIdentifier", query_info.PayIdentifier, sizeof(query_info.PayIdentifier)) : false;
    read_success = read_success ? ReadXMLInt(xml, "Money", query_info.Money) : false;
    read_success = read_success ? ReadXMLCStr(xml, "OBUMAC", query_info.OBUMAC, sizeof(query_info.OBUMAC)) : false;
    read_success = read_success ? ReadXMLCStr(xml, "OBUID", query_info.OBUID, sizeof(query_info.OBUID)) : false;
    read_success = read_success ? ReadXMLCStr(xml, "CardID", query_info.CardID, sizeof(query_info.CardID)) : false;
    {
        read_success = read_success ? IntoXMLSubNode(xml, "Vehicle") : false;
        read_success = read_success ? ReadXMLInt(xml, "Type", query_info.VehicleType) : false;
        read_success = read_success ? ReadXMLInt(xml, "Class", query_info.VehicleClass) : false;
        read_success = read_success ? ReadXMLInt(xml, "VLColor", query_info.VehicleLicenseColor) : false;
        read_success = read_success ? ReadXMLCStr(xml, "License", query_info.VehicleLicense, sizeof(query_info.VehicleLicense)) : false;
        {
            read_success = read_success ? IntoXMLSubNode(xml, "AutoPlate") : false;
            read_success = read_success ? ReadXMLCStr(xml, "AutoLicense", query_info.AutoLicense, sizeof(query_info.AutoLicense)) : false;
            read_success = read_success ? ReadXMLInt(xml, "AutoColor", query_info.AutoColor) : false;
            read_success = read_success ? OutXML2Parent(xml) : false;
        }
        read_success = read_success ? OutXML2Parent(xml) : false;
    }
    {
        read_success = read_success ? IntoXMLSubNode(xml, "Entry") : false;
        read_success = read_success ? ReadXMLCStr(xml, "RoadID", query_info.EntryRoadID, sizeof(query_info.EntryRoadID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "StationID", query_info.EntryStationID, sizeof(query_info.EntryStationID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "LaneID", query_info.EntryLaneID, sizeof(query_info.EntryLaneID)) : false;
        read_success = read_success ? ReadXMLTimet(xml, "Time", query_info.EntryTime) : false;
        read_success = read_success ? OutXML2Parent(xml) : false;
    }

    if (!read_success)
    {
        TW_TRACE_DIARY("XML参数错误:" + GetXMLParseError() );
        return false;
    }
    return true;



}
//////////////////////////////////////////////////////////////
// 函数名称:ParseWhiteListQueryParaFromJson
//
// 功能描述:解析白名单鉴权参数
//
// 输入参数:const std::string& json_para, ObuWhiteListQueryParameter&query_info,
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ParseWhiteListQueryParaFromJson(const std::string& json_para, ObuWhiteListQueryParameter&query_info)
{
    Json::Reader json_reader;
    Json::Value root_node, query_node, data_node, trade_key_node;
    Json::Value vehicle_node, entry_node, auto_plate_node;
    if (!json_reader.parse(json_para, root_node))
    {
        return false;
    }
    bool read_success = true;
    read_success = read_success ? ReadJsonSubNode(root_node, "ETCDebitListQuery", query_node) : false;
    read_success = read_success ? ReadJsonSubNode(query_node, "Data", data_node) : false;
    read_success = read_success ? ReadJsonSubNode(data_node, "TradeKeyItem", trade_key_node) : false;
    read_success = read_success ? ReadJsonSubNode(trade_key_node, "Vehicle", vehicle_node) : false;
    read_success = read_success ? ReadJsonSubNode(trade_key_node, "Entry", entry_node) : false;
    read_success = read_success ? ReadJsonSubNode(vehicle_node, "AutoPlate", auto_plate_node) : false;

    read_success = read_success ? ReadJsonChars(query_node, "Version", query_info.Version, sizeof(query_info.Version)) : false;
    read_success = read_success ? ReadJsonChars(trade_key_node, "PayIdentifier", query_info.PayIdentifier, sizeof(query_info.PayIdentifier)) : false;
    read_success = read_success ? ReadJsonInt(trade_key_node, "Money", query_info.Money) : false;
    read_success = read_success ? ReadJsonChars(trade_key_node, "OBUMAC", query_info.OBUMAC, sizeof(query_info.OBUMAC)) : false;
    read_success = read_success ? ReadJsonChars(trade_key_node, "OBUID", query_info.OBUID, sizeof(query_info.OBUID)) : false;
    read_success = read_success ? ReadJsonChars(trade_key_node, "CardID", query_info.CardID, sizeof(query_info.CardID)) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "Type", query_info.VehicleType) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "Class", query_info.VehicleClass) : false;
    read_success = read_success ? ReadJsonChars(vehicle_node, "License", query_info.VehicleLicense, sizeof(query_info.VehicleLicense)) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "VLColor", query_info.VehicleLicenseColor) : false;
    read_success = read_success ? ReadJsonInt(auto_plate_node, "AutoColor", query_info.AutoColor) : false;
    read_success = read_success ? ReadJsonChars(auto_plate_node, "AutoLicense", query_info.AutoLicense, sizeof(query_info.AutoLicense)) : false;
    read_success = read_success ? ReadJsonChars(entry_node, "RoadID", query_info.EntryRoadID, sizeof(query_info.EntryRoadID)) : false;
    read_success = read_success ? ReadJsonChars(entry_node, "StationID", query_info.EntryStationID, sizeof(query_info.EntryStationID)) : false;
    read_success = read_success ? ReadJsonChars(entry_node, "LaneID", query_info.EntryLaneID, sizeof(query_info.EntryLaneID)) : false;
    read_success = read_success ? ReadJsonTimet(entry_node, "Time", query_info.EntryTime) : false;


    if (!read_success)
    {
        TW_TRACE_DIARY("Json解析错误:" + GetJsonParseError());
        return false;
    }
    return true;


}
//////////////////////////////////////////////////////////////
// 函数名称:ParseDebitStateQueryParaFromXML
//
// 功能描述:解析交易结果查询参数
//
// 输入参数:const std::string& xml_para, DebitStateQueryParameter& query_info,
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ParseDebitStateQueryParaFromXML(const std::string& xml_para, DebitStateQueryParameter& query_info)
{
    CMarkup xml;
    xml.SetDoc(xml_para.c_str());
    xml.ResetMainPos();

    bool read_success = true;
    read_success = read_success ? IntoXMLSubNode(xml, "ETCDebitResultQuery") : false;
    read_success = read_success ? ReadXMLCStr(xml, "Version", query_info.Version, sizeof(query_info.Version)) : false;
    read_success = read_success ? IntoXMLSubNode(xml, "Data") : false;
    read_success = read_success ? IntoXMLSubNode(xml, "TradeKeyItem") : false;
    read_success = read_success ? ReadXMLCStr(xml, "PayIdentifier", query_info.PayIdentifier, sizeof(query_info.PayIdentifier)) : false;

    {
        read_success = read_success ? IntoXMLSubNode(xml, "Vehicle") : false;
        read_success = read_success ? ReadXMLInt(xml, "Type", query_info.VehicleType) : false;
        read_success = read_success ? ReadXMLInt(xml, "Class", query_info.VehicleClass) : false;
        read_success = read_success ? ReadXMLInt(xml, "VLColor", query_info.VehicleLicenseColor) : false;
        read_success = read_success ? ReadXMLCStr(xml, "License", query_info.VehicleLicense, sizeof(query_info.VehicleLicense)) : false;
        {
            read_success = read_success ? IntoXMLSubNode(xml, "AutoPlate") : false;
            read_success = read_success ? ReadXMLCStr(xml, "AutoLicense", query_info.AutoLicense, sizeof(query_info.AutoLicense)) : false;
            read_success = read_success ? ReadXMLInt(xml, "AutoColor", query_info.AutoColor) : false;
            read_success = read_success ? OutXML2Parent(xml) : false;
        }
        read_success = read_success ? OutXML2Parent(xml) : false;
    }

    if (!read_success)
    {
        TW_TRACE_DIARY("XML参数错误:" + GetXMLParseError() );
        return false;
    }
    return true;


}
//////////////////////////////////////////////////////////////
// 函数名称:ParseDebitStateQueryParaFromJson
//
// 功能描述:解析交易结果查询参数
//
// 输入参数:const std::string& json_para, DebitStateQueryParameter& query_info,
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ParseDebitStateQueryParaFromJson(const std::string& json_para, DebitStateQueryParameter& query_info)
{

    Json::Reader json_reader;
    Json::Value root_node, query_node, data_node, trade_key_node;
    Json::Value vehicle_node, auto_plate_node;
    if (!json_reader.parse(json_para, root_node))
    {
        return false;
    }
    bool read_success = true;
    read_success = read_success ? ReadJsonSubNode(root_node, "ETCDebitResultQuery", query_node) : false;
    read_success = read_success ? ReadJsonSubNode(query_node, "Data", data_node) : false;
    read_success = read_success ? ReadJsonSubNode(data_node, "TradeKeyItem", trade_key_node) : false;
    read_success = read_success ? ReadJsonSubNode(trade_key_node, "Vehicle", vehicle_node) : false;
    read_success = read_success ? ReadJsonSubNode(vehicle_node, "AutoPlate", auto_plate_node) : false;

    read_success = read_success ? ReadJsonChars(query_node, "Version", query_info.Version, sizeof(query_info.Version)) : false;
    read_success = read_success ? ReadJsonChars(trade_key_node, "PayIdentifier", query_info.PayIdentifier, sizeof(query_info.PayIdentifier)) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "Type", query_info.VehicleType) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "Class", query_info.VehicleClass) : false;
    read_success = read_success ? ReadJsonChars(vehicle_node, "License", query_info.VehicleLicense, sizeof(query_info.VehicleLicense)) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "VLColor", query_info.VehicleLicenseColor) : false;
    read_success = read_success ? ReadJsonInt(auto_plate_node, "AutoColor", query_info.AutoColor) : false;
    read_success = read_success ? ReadJsonChars(auto_plate_node, "AutoLicense", query_info.AutoLicense, sizeof(query_info.AutoLicense)) : false;

    if (!read_success)
    {
        TW_TRACE_DIARY("Json解析错误:" + GetJsonParseError());
        return false;
    }
    return true;


}
//////////////////////////////////////////////////////////////
// 函数名称:PackETCDebitResultXML
//
// 功能描述:生成ETC扣费结果
//
// 输入参数:const ETCDebitRecord & debit_record, std::string& result,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int PackETCDebitResultXML(const ETCDebitRecord & debit_record, std::string& result)
{
    CString strXMLString;
    CMarkup xml;
    xml.SetDoc( _T("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n") );

    xml.AddElem("DebitResultContext");
    xml.IntoElem();

    xml.AddElem(_T("Version"), "1.0");
    xml.AddElem(_T("Data"));
    xml.IntoElem();

    xml.AddElem(_T("ResultType"),   debit_record.LocalResponse.ResultType);
    xml.AddElem(_T("ExecuteCode"),  debit_record.LocalResponse.ExecuteCode);
    xml.AddElem(_T("ExecuteDesc"),  debit_record.LocalResponse.ExecuteDesc);

    xml.AddElem(_T("TradeKeyItem"));
    xml.IntoElem();
    xml.AddElem(_T("TradeSecurityCode"),    debit_record.LocalResponse.TradeSecurityCode);
    xml.AddElem(_T("PayPlatformType"),      debit_record.LocalResponse.PayPlatformType);
    xml.AddElem(_T("PayIdentifier"),        debit_record.LocalResponse.PayIdentifier);
    xml.AddElem(_T("DebitOrder"),           debit_record.LocalResponse.PaySerialNo);
    xml.AddElem(_T("DebitTime"),            Datetime2Str(debit_record.LocalResponse.DebitTime).c_str());
    xml.AddElem(_T("ConsumeTime"),          debit_record.LocalResponse.ConsumeTime);

    xml.OutOfElem();
    xml.OutOfElem();

    result = (LPCSTR)(xml.GetDoc());
    result = pystring::replace(result,"\r\n","");
    return 0;
}
//////////////////////////////////////////////////////////////
// 函数名称:PackETCDebitResultJson
//
// 功能描述:生成ETC扣费结果
//
// 输入参数:const ETCDebitRecord & debit_record, std::string& result,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int PackETCDebitResultJson(const ETCDebitRecord & debit_record, std::string& result)
{

    Json::Value root;
    Json::Value ResultContext;
    Json::Value Data;
    Json::Value KeyItem;
    Json::Value ETCTradeInfo;
    /***********************************************************构造JSAO数据结构***********************************************************/
    //标识版本
    ResultContext["Version"]        = Json::Value(debit_record.LocalResponse.Version);
    //标识扣款类型
    Data["ResultType"]              = Json::Value(debit_record.LocalResponse.ResultType);
    //标识执行状态
    Data["ExecuteCode"]             = Json::Value(debit_record.LocalResponse.ExecuteCode);
    //标识执行结果描述
    Data["ExecuteDesc"]             = Json::Value(debit_record.LocalResponse.ExecuteDesc);

    //关键数据项 -- 交易安全码
    KeyItem["TradeSecurityCode"]    = Json::Value(debit_record.LocalResponse.TradeSecurityCode);
    //关键数据项 -- 第三方支付平台类型
    KeyItem["PayPlatformType"]      = Json::Value(debit_record.LocalResponse.PayPlatformType);
    //关键数据项 -- 交易识别码
    KeyItem["PayIdentifier"]        = Json::Value(debit_record.LocalResponse.PayIdentifier);
    //关键数据项 -- 订单号
    KeyItem["DebitOrder"]           = Json::Value(debit_record.LocalResponse.PaySerialNo);
    //关键数据项 -- 服务端扣款时间
    KeyItem["DebitTime"]            = Json::Value(Datetime2Str(debit_record.LocalResponse.DebitTime));
    //关键数据项 -- 消耗时间
    KeyItem["ConsumeTime"]          = Json::Value(debit_record.LocalResponse.ConsumeTime);

    Data["TradeKeyItem"]        = KeyItem;
    ResultContext["Data"]       = Data;
    root["DebitResultContext"]  = ResultContext;
    //转化为字符串
    Json::FastWriter fast_writer;
    result = fast_writer.write(root);


    return 0;

}

//////////////////////////////////////////////////////////////
// 函数名称:ParseETCDebitParaFromJson
//
// 功能描述:解析ETC扣费参数
//
// 输入参数:const std::string& json_para, ETCDebitRecord& debit_record,
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ParseETCDebitParaFromJson(const std::string& json_para, ETCDebitRecord& debit_record)
{

    Json::Reader json_reader;
    Json::Value root_node, context_node, data_node;
    Json::Value debit_node, vehicle_node, auto_plate_node;
    Json::Value operation_node, entry_node, obu_node;
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
    read_success = read_success ? ReadJsonSubNode(data_node, "Entry", entry_node) : false;
    read_success = read_success ? ReadJsonSubNode(data_node, "OBU", obu_node) : false;
    read_success = read_success ? ReadJsonSubNode(vehicle_node, "AutoPlate", auto_plate_node) : false;


    read_success = read_success ? ReadJsonChars(context_node, "Version", debit_record.Version, sizeof(debit_record.Version)) : false;

    read_success = read_success ? ReadJsonInt(data_node, "OverTime", debit_record.OverTimeMs) : false;

    read_success = read_success ? ReadJsonChars(debit_node, "PayIdentifier", debit_record.PayIdentifier, sizeof(debit_record.PayIdentifier)) : false;
    read_success = read_success ? ReadJsonInt(debit_node, "Money", debit_record.Money) : false;
    read_success = read_success ? ReadJsonTimet(debit_node, "SubTime", debit_record.SubTime) : false;
    read_success = read_success ? ReadJsonInt(debit_node, "Type", debit_record.DebitType) : false;

    read_success = read_success ? ReadJsonInt(vehicle_node, "Type", debit_record.VehicleType) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "Class", debit_record.VehicleClass) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "VLColor", debit_record.VehicleLicenseColor) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "AxisCount", debit_record.AxisCount) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "Weight", debit_record.Weight) : false;
    read_success = read_success ? ReadJsonInt(vehicle_node, "LimitWeight", debit_record.LimitWeight) : false;
    read_success = read_success ? ReadJsonChars(vehicle_node, "License", debit_record.VehicleLicense, sizeof(debit_record.VehicleLicense)) : false;
    read_success = read_success ? ReadJsonChars(auto_plate_node, "AutoLicense", debit_record.AutoLicense, sizeof(debit_record.AutoLicense)) : false;
    read_success = read_success ? ReadJsonInt(auto_plate_node, "AutoColor", debit_record.AutoColor) : false;

    read_success = read_success ? ReadJsonInt(operation_node, "PassCertificateType", debit_record.PassCertificateType) : false;
    read_success = read_success ? ReadJsonInt(operation_node, "TollDate", debit_record.TollDate) : false;
    read_success = read_success ? ReadJsonInt(operation_node, "ShiftID", debit_record.ShiftID) : false;
    read_success = read_success ? ReadJsonInt(operation_node, "Distance", debit_record.Distance) : false;
    read_success = read_success ? ReadJsonChars(operation_node, "CardID", debit_record.CardID, sizeof(debit_record.CardID)) : false;
    read_success = read_success ? ReadJsonChars(operation_node, "OperatorName", debit_record.OperatorName, sizeof(debit_record.OperatorName)) : false;
    read_success = read_success ? ReadJsonChars(operation_node, "OperatorID", debit_record.OperatorID, sizeof(debit_record.OperatorID)) : false;
    read_success = read_success ? ReadJsonChars(operation_node, "TicketNo", debit_record.TicketNo, sizeof(debit_record.TicketNo)) : false;

    read_success = read_success ? ReadJsonChars(entry_node, "AreaID", debit_record.EntryAreaID, sizeof(debit_record.EntryAreaID)) : false;
    read_success = read_success ? ReadJsonChars(entry_node, "RoadID", debit_record.EntryRoadID, sizeof(debit_record.EntryRoadID)) : false;
    read_success = read_success ? ReadJsonChars(entry_node, "StationID", debit_record.EntryStationID, sizeof(debit_record.EntryStationID)) : false;
    read_success = read_success ? ReadJsonChars(entry_node, "StationName", debit_record.EntryStationName, sizeof(debit_record.EntryStationName)) : false;
    read_success = read_success ? ReadJsonChars(entry_node, "LaneID", debit_record.EntryLaneID, sizeof(debit_record.EntryLaneID)) : false;
    read_success = read_success ? ReadJsonChars(entry_node, "License", debit_record.EntryLicense, sizeof(debit_record.EntryLicense)) : false;
    read_success = read_success ? ReadJsonInt(entry_node, "VColor", debit_record.EntryLicenseColor) : false;
    read_success = read_success ? ReadJsonInt(entry_node, "VClass", debit_record.EntryVehicleClass) : false;
    read_success = read_success ? ReadJsonInt(entry_node, "VType", debit_record.EntryVehicleType) : false;
    read_success = read_success ? ReadJsonTimet(entry_node, "Time", debit_record.EntryTime) : false;

    read_success = read_success ? ReadJsonChars(obu_node, "License", debit_record.OBULicense, sizeof(debit_record.OBULicense)) : false;
    read_success = read_success ? ReadJsonChars(obu_node, "ID", debit_record.OBUID, sizeof(debit_record.OBUID)) : false;
    read_success = read_success ? ReadJsonChars(obu_node, "MAC", debit_record.OBUMAC, sizeof(debit_record.OBUMAC)) : false;

    read_success = read_success ? ReadJsonInt(obu_node, "VColor", debit_record.OBULicenseColor) : false;
    read_success = read_success ? ReadJsonInt(obu_node, "Class", debit_record.OBUVehicleClass) : false;
    read_success = read_success ? ReadJsonInt(obu_node, "Type", debit_record.OBUVehicleType) : false;
    if (!read_success)
    {
        TW_TRACE_DIARY("Json解析错误:" + GetJsonParseError());
        return false;
    }
    return true;

}
//////////////////////////////////////////////////////////////
// 函数名称:ParseETCDebitParaFromXML
//
// 功能描述:解析ETC扣费参数
//
// 输入参数:const std::string& xml_para, ETCDebitRecord& debit_record,
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ParseETCDebitParaFromXML(const std::string& xml_para, ETCDebitRecord& debit_record)
{
    CMarkup xml;
    xml.SetDoc(xml_para.c_str());
    xml.ResetMainPos();

    bool read_success = true;
    read_success = read_success ? IntoXMLSubNode(xml, "DebitParamContext") : false;
    read_success = read_success ? ReadXMLCStr(xml, "Version", debit_record.Version, sizeof(debit_record.Version)) : false;
    read_success = read_success ? IntoXMLSubNode(xml, "Data") : false;
    read_success = read_success ? ReadXMLInt(xml, "OverTime", debit_record.OverTimeMs) : false;

    {
        read_success = read_success ? IntoXMLSubNode(xml, "Debit") : false;
        read_success = read_success ? ReadXMLCStr(xml, "PayIdentifier", debit_record.PayIdentifier, sizeof(debit_record.PayIdentifier)) : false;
        read_success = read_success ? ReadXMLInt(xml, "Money", debit_record.Money) : false;
        read_success = read_success ? ReadXMLTimet(xml, "SubTime", debit_record.SubTime) : false;
        read_success = read_success ? ReadXMLInt(xml, "Type", debit_record.DebitType) : false;
        read_success = read_success ? OutXML2Parent(xml) : false;
    }

    {
        read_success = read_success ? IntoXMLSubNode(xml, "Vehicle") : false;
        read_success = read_success ? ReadXMLInt(xml, "Type", debit_record.VehicleType) : false;
        read_success = read_success ? ReadXMLInt(xml, "Class", debit_record.VehicleClass) : false;
        read_success = read_success ? ReadXMLInt(xml, "VLColor", debit_record.VehicleLicenseColor) : false;
        read_success = read_success ? ReadXMLCStr(xml, "License", debit_record.VehicleLicense, sizeof(debit_record.VehicleLicense)) : false;
        {
            read_success = read_success ? IntoXMLSubNode(xml, "AutoPlate") : false;
            read_success = read_success ? ReadXMLCStr(xml, "AutoLicense", debit_record.AutoLicense, sizeof(debit_record.AutoLicense)) : false;
            read_success = read_success ? ReadXMLInt(xml, "AutoColor", debit_record.AutoColor) : false;
            read_success = read_success ? OutXML2Parent(xml) : false;
        }
        read_success = read_success ? ReadXMLInt(xml, "AxisCount", debit_record.AxisCount) : false;
        read_success = read_success ? ReadXMLInt(xml, "Weight", debit_record.Weight) : false;
        read_success = read_success ? ReadXMLInt(xml, "LimitWeight", debit_record.LimitWeight) : false;
        read_success = read_success ? OutXML2Parent(xml) : false;
    }
    {
        read_success = read_success ? IntoXMLSubNode(xml, "Operation") : false;
        read_success = read_success ? ReadXMLInt(xml, "PassCertificateType", debit_record.PassCertificateType) : false;
        read_success = read_success ? ReadXMLInt(xml, "TollDate", debit_record.TollDate) : false;
        read_success = read_success ? ReadXMLInt(xml, "ShiftID", debit_record.ShiftID) : false;
        read_success = read_success ? ReadXMLInt(xml, "Distance", debit_record.Distance) : false;

        read_success = read_success ? ReadXMLCStr(xml, "CardID", debit_record.CardID, sizeof(debit_record.CardID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "TicketNo", debit_record.TicketNo, sizeof(debit_record.TicketNo)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "OperatorID", debit_record.OperatorID, sizeof(debit_record.OperatorID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "OperatorName", debit_record.OperatorName, sizeof(debit_record.OperatorName)) : false;
        read_success = read_success ? OutXML2Parent(xml) : false;
    }
    {
        read_success = read_success ? IntoXMLSubNode(xml, "Entry") : false;
        read_success = read_success ? ReadXMLCStr(xml, "AreaID", debit_record.EntryAreaID, sizeof(debit_record.EntryAreaID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "RoadID", debit_record.EntryRoadID, sizeof(debit_record.EntryRoadID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "StationID", debit_record.EntryStationID, sizeof(debit_record.EntryStationID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "StationName", debit_record.EntryStationName, sizeof(debit_record.EntryStationName)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "LaneID", debit_record.EntryLaneID, sizeof(debit_record.EntryLaneID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "License", debit_record.EntryLicense, sizeof(debit_record.EntryLicense)) : false;

        read_success = read_success ? ReadXMLInt(xml, "VColor", debit_record.EntryLicenseColor) : false;
        read_success = read_success ? ReadXMLInt(xml, "VClass", debit_record.EntryVehicleClass) : false;
        read_success = read_success ? ReadXMLInt(xml, "VType", debit_record.EntryVehicleType) : false;
        read_success = read_success ? ReadXMLTimet(xml, "Time", debit_record.EntryTime) : false;
        read_success = read_success ? OutXML2Parent(xml) : false;
    }
    {
        read_success = read_success ? IntoXMLSubNode(xml, "OBU") : false;
        read_success = read_success ? ReadXMLCStr(xml, "License", debit_record.OBULicense, sizeof(debit_record.OBULicense)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "ID", debit_record.OBUID, sizeof(debit_record.OBUID)) : false;
        read_success = read_success ? ReadXMLCStr(xml, "MAC", debit_record.OBUMAC, sizeof(debit_record.OBUMAC)) : false;

        read_success = read_success ? ReadXMLInt(xml, "VColor", debit_record.OBULicenseColor) : false;
        read_success = read_success ? ReadXMLInt(xml, "Class", debit_record.OBUVehicleClass) : false;
        read_success = read_success ? ReadXMLInt(xml, "Type", debit_record.OBUVehicleType) : false;
        read_success = read_success ? OutXML2Parent(xml) : false;
    }

    if (!read_success)
    {
        TW_TRACE_DIARY("XML参数错误:" + GetXMLParseError() );
        return false;
    }
    return true;

}

//////////////////////////////////////////////////////////////
// 函数名称:ParseETCDebitResponseFromServer
//
// 功能描述:解析服务端的ETC扣费回馈
//
// 输入参数:const std::string& json_response, ETCDebitRecord& state,
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ParseETCDebitResponseFromServer(const std::string& json_response, ETCDebitRecord& state)
{
    Json::Value root_node;
    Json::Reader json_reader;
    if(!json_reader.parse(json_response,root_node))
    {
        return false;
    }
    bool read_ok = true;
	read_ok = read_ok? ReadJsonInt(root_node,"Status",state.ServerResponse.ServerStatus):false;
    if(!read_ok)
    {
        return false;
    }
    if(state.ServerResponse.ServerStatus!=1)
    {
        return true;
    }
    read_ok = read_ok? ReadJsonChars(root_node,"OrderNo",state.ServerResponse.DebitOrder,sizeof(state.ServerResponse.DebitOrder)):false;

    return true;
}
//////////////////////////////////////////////////////////////
// 函数名称:PackETCDebitRequestJson
//
// 功能描述:生成ETC扣费的请求参数
//
// 输入参数:const ETCDebitRecord& debit_record, std::string& result,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int PackETCDebitRequestJson(const ETCDebitRecord& debit_record, std::string& result)
{
    Json::Value root_node;
    root_node["Amount"] = Json::Value(debit_record.Money);
    root_node["AutoPlateColor"] = Json::Value(debit_record.AutoColor);
    root_node["AutoPlateNo"] = Json::Value(debit_record.AutoLicense);
    root_node["AxisCount"] = Json::Value(debit_record.AxisCount);
    root_node["CardNo"] = Json::Value(debit_record.CardID);
    root_node["DeviceDllVersion"] = Json::Value(GetDeviceDllVersion());
    root_node["Distance"] = Json::Value(debit_record.Distance);
    root_node["EntryAreaID"] = Json::Value(debit_record.EntryAreaID);
    root_node["EntryLaneID"] = Json::Value(debit_record.EntryLaneID);
    root_node["EntryPlateColor"] = Json::Value(debit_record.EntryLicenseColor);
    root_node["EntryPlateNo"] = Json::Value(debit_record.EntryLicense);
    root_node["EntryProvinceId"] = Json::Value(GetGlobalApp()->m_sTranstParam.ProvinceID);
    root_node["EntryRoadID"] = Json::Value(debit_record.EntryRoadID);
    root_node["EntryStationID"] = Json::Value(debit_record.EntryStationID);
    root_node["EntryStationName"] = Json::Value(debit_record.EntryStationName);
    root_node["EntryTime"] = Json::Value(Datetime2Str(debit_record.EntryTime));
    root_node["EntryVehicleClass"] = Json::Value(debit_record.EntryVehicleClass);
    root_node["EntryVehicleType"] = Json::Value(debit_record.EntryVehicleType);
    root_node["LimitWeight"] = Json::Value(debit_record.LimitWeight);
    root_node["NetPayDllVersion"] = Json::Value(GetNetPayDllVersion());
    root_node["Ordinal"] = Json::Value(0);
    root_node["PackageType"] = Json::Value(12);
    root_node["PassCertificateType"] = Json::Value(1);
    root_node["PayIdentifier"] = Json::Value(debit_record.PayIdentifier);
    root_node["PayLane"] = Json::Value(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_LaneID);
    root_node["PayOperatorId"] = Json::Value(debit_record.OperatorID);
    root_node["PayOperatorName"] = Json::Value(debit_record.OperatorName);
    root_node["PayProvinceId"] = Json::Value(GetGlobalApp()->m_sTranstParam.ProvinceID);
    root_node["PayRoadId"] = Json::Value(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_RoadID);
    root_node["PayStationId"] = Json::Value(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_StationID);
    root_node["PayStationName"] = Json::Value(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationTab);
    root_node["PlateColor"] = Json::Value(debit_record.OBULicenseColor);
    root_node["PlateNo"] = Json::Value(debit_record.OBULicense);
    root_node["RecordMatchCode"] = Json::Value(debit_record.LocalResponse.TradeSecurityCode);
    root_node["ShiftId"] = Json::Value(debit_record.ShiftID);
    root_node["Sign"] = Json::Value("");
    root_node["SubTime"] = Json::Value(ChinaTimeStr());
    root_node["TicketNo"] = Json::Value("");
    root_node["Token"] = Json::Value("");
    root_node["TollDate"] = Json::Value(debit_record.TollDate);
    root_node["TollType"] = Json::Value(1);
    root_node["TradeSecurityCode"] = Json::Value(debit_record.LocalResponse.TradeSecurityCode);
    root_node["VehicleClass"] = Json::Value(debit_record.OBUVehicleClass);
    root_node["VehicleType"] = Json::Value(debit_record.OBUVehicleType);
    root_node["Version"] = Json::Value("1.5");
    root_node["Weight"] = Json::Value(debit_record.Weight);
    root_node["ObuMacNo"] = Json::Value(debit_record.OBUMAC);
    root_node["TradeNo"] = Json::Value(debit_record.LocalResponse.PaySerialNo);
    root_node["ETCCardNo"] = Json::Value(debit_record.CardID);

    Json::FastWriter json_writer;
    result = json_writer.write(root_node);
    return 0;
}

//////////////////////////////////////////////////////////////
// 函数名称:PackOBUWhiteListQueryJson
//
// 功能描述:生成白名单鉴权的请求参数
//
// 输入参数:const ObuWhiteListQueryParameter& query_para, std::string&result,
// 输出参数:int 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
int PackOBUWhiteListQueryJson(const ObuWhiteListQueryParameter& query_para, std::string&result)
{
    Json::Value root_node;

    root_node["QueryTime"] = Json::Value(ChinaTimeStrWithMS());
    root_node["PackageType"] = Json::Value(11);
    root_node["PlateColor"] = Json::Value(query_para.VehicleLicenseColor);
    root_node["PlateNo"] = Json::Value(query_para.VehicleLicense);
    root_node["ProvinceId"] = Json::Value(GetGlobalApp()->m_sTranstParam.ProvinceID);
    root_node["RoadId"] = Json::Value(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_RoadID);
    root_node["StationId"] = Json::Value(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.str_StationID);
    root_node["VehicleClass"] = Json::Value(query_para.VehicleClass);
    root_node["VehicleType"] = Json::Value(query_para.VehicleType);
    root_node["Version"] = Json::Value("1.5");
    root_node["AxisCount"] = Json::Value(0);
    root_node["LimitWeight"] = Json::Value(0);
    root_node["Weight"] = Json::Value(0);
    root_node["ObuMacNo"] = Json::Value(query_para.OBUMAC);
    root_node["TradeSecurityCode"] = Json::Value("12");
    root_node["ETCCardNo"] = Json::Value(query_para.CardID);

    root_node["QueryIdentifier"] = Json::Value(query_para.PayIdentifier);
    root_node["Amount"] = Json::Value(query_para.Money);
    root_node["EntryRoadId"] = Json::Value(Str2Int(query_para.EntryRoadID));
    root_node["EntryStationId"] = Json::Value(Str2Int(query_para.EntryStationID));
    root_node["EntryLaneId"] = Json::Value(Str2Int(query_para.EntryLaneID));
    root_node["EntryTime"] = Json::Value(Datetime2Str(query_para.EntryTime));
    root_node["OutTime"] = Json::Value(ChinaTimeStr());
    root_node["OutLaneId"] = Json::Value(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);

    Json::FastWriter json_writer;
    result = json_writer.write(root_node);
    return 0;
}
//////////////////////////////////////////////////////////////
// 函数名称:ParseOBUWhiteListRecordFromServer
//
// 功能描述:解析服务端回馈的白名单鉴权结果
//
// 输入参数:const std::string& json_response, ObuWhiteListRecord& obu_white_record,
// 输出参数:bool 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
bool ParseOBUWhiteListRecordFromServer(const std::string& json_response, ObuWhiteListRecord& obu_white_record)
{
    Json::Value root_node;
    Json::Reader json_reader;
    if (!json_reader.parse(json_response, root_node))
    {
        return false;
    }
    bool read_ok = true;
    int server_status;
    read_ok = read_ok ? ReadJsonInt(root_node, "PlateColor", obu_white_record.VehicleLicenseColor) : false;
    read_ok = read_ok ? ReadJsonInt(root_node, "VehicleClass", obu_white_record.VehicleClass) : false;
    read_ok = read_ok ? ReadJsonInt(root_node, "VehicleType", obu_white_record.VehicleType) : false;
    read_ok = read_ok ? ReadJsonChars(root_node, "PlateNo", obu_white_record.VehicleLicense, sizeof(obu_white_record.VehicleLicense)) : false;
    read_ok = read_ok ? ReadJsonChars(root_node, "QueryIdentifier", obu_white_record.QueryIdentifier, sizeof(obu_white_record.QueryIdentifier)) : false;
    read_ok = read_ok ? ReadJsonInt(root_node, "Status", server_status) : false;
    // read_ok = read_ok ? ReadJsonInt(root_node, "Amount", obu_white_record.Money) : false;
    if (!read_ok)
    {
        TW_TRACE_DIARY("JSON参数错误:" + GetJsonParseError() );
        return false;
    }
    obu_white_record.AgentStatus = 255;
    switch (server_status)
    {
    case 1:
        obu_white_record.AgentStatus = 0;
        break;
    case 30:
    case 31:
    case 32:
        obu_white_record.AgentStatus = 1;
        break;
    case 33:
        obu_white_record.AgentStatus = 2;
        break;
    case 102:
        obu_white_record.AgentStatus = 3;
        break;
    }

    return true;
}

//////////////////////////////////////////////////////////////
// 函数名称:GetNetPayRecordMatchCode
//
// 功能描述:生成交易匹配码
//
// 输入参数:const ETCDebitRecord& dp,
// 输出参数:std::string 
// 编写人员:朱亚冰
// 编写时间:2019-01-11
// 修改人员:
// 修改时间:
// 函数版本:1.0.0.0
// 备注说明:
//
//////////////////////////////////////////////////////////////
std::string GetNetPayRecordMatchCode(const ETCDebitRecord& dp )
{
    CString strMatchCode;
    //校验密钥
    CString strRealKey;
    if (strRealKey.IsEmpty())
    {

        switch (GetGlobalApp()->m_sTranstParam.ProvinceID)
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
    int iSTLen = strlen(dp.EntryStationID);
    if (63 == GetGlobalApp()->m_sTranstParam.ProvinceID)
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
    iSTLen = _tcslen(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
    if (63 == GetGlobalApp()->m_sTranstParam.ProvinceID)
    {
        if (iSTLen < 7)
        {
            strExitST.Format(_T("%s%s"), strTempEnST.Left(7 - iSTLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
        }
        else
        {
            strExitST.Format(_T("%s"), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
        }
    }
    else
    {
        if (iSTLen < 10)
        {
            strExitST.Format(_T("%s%s"), strTempEnST.Left(10 - iSTLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
        }
        else
        {
            strExitST.Format(_T("%s%s"), strTempEnST.Left(20 - iSTLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.StationID);
        }
    }

    //出口时间
    CString strExitTime;
    CTime cExitTime(dp.SubTime);
    strExitTime.Format(_T("%04d%02d%02d%02d%02d%02d"), cExitTime.GetYear(), cExitTime.GetMonth(), cExitTime.GetDay(), cExitTime.GetHour(), cExitTime.GetMinute(), cExitTime.GetSecond());

    //出口车道
    CString strExitLane;
    int iLaneLen = strlen(GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);//_tcslen(dp.LaneConfig.LaneID);
    if (   (44 == GetGlobalApp()->m_sTranstParam.ProvinceID)
            || (61 == GetGlobalApp()->m_sTranstParam.ProvinceID)
            || (62 == GetGlobalApp()->m_sTranstParam.ProvinceID)
            || (63 == GetGlobalApp()->m_sTranstParam.ProvinceID)
       )
    {
        if (iLaneLen < 3)
        {
            strExitLane.Format(_T("%s%s"), strTempEnST.Left(3 - iLaneLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);
        }
        else
        {
            strExitLane.Format(_T("%s"), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);
        }
    }
    else
    {
        if (iLaneLen < 8)
        {
            strExitLane.Format(_T("%s%s"), strTempEnST.Left(8 - iLaneLen), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);
        }
        else
        {
            strExitLane.Format(_T("%s"), GetGlobalApp()->m_sTranstParam.LaneConfigInfo.LaneID);
        }
    }

    //虚拟卡号
    CString strVirtualCardID;

    if (   (44 == GetGlobalApp()->m_sTranstParam.ProvinceID)
            || (62 == GetGlobalApp()->m_sTranstParam.ProvinceID)
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

	/*
    CString debug_data;
    debug_data.Format(_T("%s@%s@%s@%s@%s@%s@%s"), strEntryST, strEntryTime, strExitST, strExitTime, strExitLane, strVirtualCardID, strTradeMoney);
    GetGlobalApp()->RecordLog(debug_data);
	GetGlobalApp()->RecordLog(strRealKey);
	
	CString tmp_data = "000000026220190307151532000000051420190307152411010000000000000000000000000000300";
	CAesECB tmp_aes(strRealKey);
	CString tmp_strEncData = tmp_aes.EncryptData(tmp_data);
	CMD5 tmp_md5((LPCSTR)tmp_strEncData);
	GetGlobalApp()->RecordLog(tmp_md5.GetMD5());
	*/
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

