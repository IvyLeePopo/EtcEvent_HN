/*
 * Copyright(C) 2016,SQUIRREL 个人工作室 保留所有权利。( All rights reserved. )
 * 
 * 文件名称：JsonParamOperate.cpp
 * 摘    要：接口参数JSON格式操作类，实现文件
 * 当前版本：1.0.0.1
 * 作    者：ROCY
 * 创建日期：2016年8月15日
 */
#include "StdAfx.h"
#include "JsonParamOperate.h"

CJsonParamOperate::CJsonParamOperate(void)
{
}

CJsonParamOperate::~CJsonParamOperate(void)
{
}

time_t OleTimeToTime(DATE dTime)
{
	COleDateTimeSpan tSpan;
	COleDateTime t1 = (time_t)0;
	COleDateTime t2 = dTime;
	tSpan = t2 - t1;
	return time_t(tSpan.GetTotalSeconds());
}

time_t CString2time_t(LPCTSTR lpstime)
{
	CString strTemp(lpstime);
	COleDateTime oleGetTime;
	oleGetTime.ParseDateTime(strTemp);
	return OleTimeToTime(oleGetTime.m_dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CJsonParamOperate::FormatParamData2Json
//     
// 功能描述：将接收到的数据结构转化为标准的JSON格式 -- 支付请求参数数据变换(扣费)
//     
// 输入参数：IN const CDVJsonParamDebitMoney* pDVJsonParamDebitMoney
// 输出参数：CString
// 编写人员：ROCY
// 编写时间：2016年8月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamDebitMoney* pDVJsonParamDebitMoney)
{
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamDebitMoney)
			throw -1;
		// 表示整个 json 对象
		Json::Value root;
		// 标识用户参数
		Json::Value DebitParamContext;
		// 标识详细数据项
		Json::Value Data;
		// 标识扣费数据项
		Json::Value Debit;
		// 标识车辆数据项
		Json::Value Vehicle;
		// 标识业务数据项
		Json::Value Operation;
		// 标识入口数据项
		Json::Value Entry;

		//标识版本
		DebitParamContext["Version"]	= Json::Value(pDVJsonParamDebitMoney->Version);
		
		Data["OverTime"]				= Json::Value(pDVJsonParamDebitMoney->OverTime);
		Data["OperationMode"]			= Json::Value(pDVJsonParamDebitMoney->OperationMode);
	
		Debit["PayIdentifier"]			= Json::Value(pDVJsonParamDebitMoney->PayIdentifier);
		Debit["Money"]					= Json::Value(pDVJsonParamDebitMoney->Money);
		Debit["SubTime"]				= Json::Value(pDVJsonParamDebitMoney->SubTime.Format(_T("%Y-%m-%d %H:%M:%S")));
		Debit["Type"]					= Json::Value(pDVJsonParamDebitMoney->DebitMainType);
		Debit["PayCertificateCode"]		= Json::Value(pDVJsonParamDebitMoney->PayCertificateCode);
		Debit["PayCertificateType"]		= Json::Value(pDVJsonParamDebitMoney->PayCertificateType);
		
		Vehicle["Type"]					= Json::Value(pDVJsonParamDebitMoney->VehicleType);
		Vehicle["Class"]				= Json::Value(pDVJsonParamDebitMoney->VehicleClass);
		Vehicle["License"]				= Json::Value(pDVJsonParamDebitMoney->VehicleLicense);
		Vehicle["VLColor"]				= Json::Value(pDVJsonParamDebitMoney->VLColor);
		Vehicle["AxisCount"]			= Json::Value(pDVJsonParamDebitMoney->AxisCount);
		Vehicle["Weight"]				= Json::Value(pDVJsonParamDebitMoney->Weight);
		//add 2018-03
		Json::Value AutoPlate;
		AutoPlate["AutoLicense"]=Json::Value(pDVJsonParamDebitMoney->AutoLicense);
		AutoPlate["AutoColor"]=Json::Value(pDVJsonParamDebitMoney->AutoColor);
		Vehicle["AutoPlate"] = AutoPlate;

		Operation["PassCertificateType"]= Json::Value(pDVJsonParamDebitMoney->PassCertificateType);
		//Operation["PassCertificateType"]= 3;

		Operation["CardID"]				= Json::Value(pDVJsonParamDebitMoney->CardID);
		Operation["TollDate"]			= Json::Value(pDVJsonParamDebitMoney->TollDate);
		Operation["ShiftID"]			= Json::Value(pDVJsonParamDebitMoney->ShiftID);
		Operation["OperatorName"]			= Json::Value(pDVJsonParamDebitMoney->OperatorName);
		Operation["OperatorID"]			= Json::Value(pDVJsonParamDebitMoney->OperatorID);
		Operation["Distance"]			= Json::Value(pDVJsonParamDebitMoney->Distance);
		Operation["TicketNo"]			= Json::Value(pDVJsonParamDebitMoney->TicketNo);
		Operation["AutoPrint"]			= Json::Value(pDVJsonParamDebitMoney->AutoPrint);
		
		
	
		Entry["AreaID"]					= Json::Value(pDVJsonParamDebitMoney->EntryAreaID);
		Entry["RoadID"]					= Json::Value(pDVJsonParamDebitMoney->EntryRoadID);
		Entry["StationID"]				= Json::Value(pDVJsonParamDebitMoney->EntryStationID);
		Entry["StationName"]			= Json::Value(pDVJsonParamDebitMoney->EntryStationName);
		Entry["Time"]					= Json::Value(pDVJsonParamDebitMoney->EntryTime.Format(_T("%Y-%m-%d %H:%M:%S")));
		Entry["LaneID"]					= Json::Value(pDVJsonParamDebitMoney->EntryLane);
		Entry["License"]				= Json::Value(pDVJsonParamDebitMoney->EntryVehicleLicense);
		Entry["VColor"]					= Json::Value(pDVJsonParamDebitMoney->EntryVLColor);
		Entry["VType"]					= Json::Value(pDVJsonParamDebitMoney->EntryVehicleType);
		Entry["VClass"]					= Json::Value(pDVJsonParamDebitMoney->EntryVehicleClass);
		
		Data["Debit"]		= Debit;
		Data["Vehicle"]		= Vehicle;
		Data["Operation"]	= Operation;
		Data["Entry"]		= Entry;
		
		DebitParamContext["Data"]	= Data;
		root["DebitParamContext"]	= DebitParamContext;

		//转化为字符串
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());

	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CJsonParamOperate::FormatParamData2Json
//     
// 功能描述：将接收到的数据格式转化为标准的JSON格式 -- 支付请求参数数据变换(撤单)
//     
// 输入参数：IN const CDVJsonParamDebitCancel* pDVJsonParamDebitCancel
// 输出参数：CString
// 编写人员：ROCY
// 编写时间：2016年8月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamDebitCancel* pDVJsonParamDebitCancel)
{
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamDebitCancel)
			throw -1;
		// 表示整个 json 对象
		Json::Value root;
		// 标识用户参数
		Json::Value DebitCancel;
		// 标识详细数据项
		Json::Value Data;
		// 标识关键数据项
		Json::Value KeyItem;

		//标识版本
		DebitCancel["Version"]			= Json::Value(pDVJsonParamDebitCancel->Version);
		
		KeyItem["PayIdentifier"]		= Json::Value(pDVJsonParamDebitCancel->PayIdentifier);

		Data["TradeKeyItem"]			= KeyItem;
		DebitCancel["Data"]				= Data;
		root["DebitCancelParamContext"]	= DebitCancel;

		//转化为字符串
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CJsonParamOperate::FormatParamData2Json
//     
// 功能描述：将接收到的数据结构转化为标准的JSON格式 -- 界面交互参数数据变换
//     
// 输入参数：IN const CDVJsonParamMMI* pDVJsonParamMMI
// 输出参数：CString
// 编写人员：ROCY
// 编写时间：2016年8月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamMMI* pDVJsonParamMMI)
{
	
		/***
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamMMI)
			throw -1;
		Json::Value root;
		Json::Value MMIParamContext;
		Json::Value Data;


		MMIParamContext["Version"]  = Json::Value(pDVJsonParamMMI->Version);
		if(!(pDVJsonParamMMI->ClearMMI)) //非清屏则有数据
		{
			Data["VType"]       = Json::Value(pDVJsonParamMMI->EntryVehicleType);
			Data["VClass"]      = Json::Value(pDVJsonParamMMI->EntryVehicleClass);
			Data["Money"]       = Json::Value(pDVJsonParamMMI->Money);
			Data["Weight"]      = Json::Value(pDVJsonParamMMI->Weight);
			Data["AxisCount"]   = Json::Value(pDVJsonParamMMI->AxisCount);
			Data["LimitWeight"] = Json::Value(pDVJsonParamMMI->LimitWeight);
			Data["Balance"]     = Json::Value(pDVJsonParamMMI->nBalance);
			Data["StationName"] = Json::Value(pDVJsonParamMMI->EntryStationName);

			Data["VColor"]      = Json::Value(pDVJsonParamMMI->EntryVLColor);
			Data["License"]     = Json::Value(pDVJsonParamMMI->EntryVehicleLicense);
			Data["CroseNum"]    = Json::Value(pDVJsonParamMMI->nCroseNum);
			Data["Program"]     = Json::Value(pDVJsonParamMMI->nProgram);
			Data["Remark1"]     = Json::Value(pDVJsonParamMMI->csRemark1);
			Data["Remark2"]     = Json::Value(pDVJsonParamMMI->csRemark2);
		}

		Data["ClearMMI"]		= Json::Value(pDVJsonParamMMI->ClearMMI);
		MMIParamContext["Data"] = Data;
		root["MMIParamContext"] = MMIParamContext;
		//转化为字符串
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& errCode)
	{
		switch(errCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;
    **/

	
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamMMI)
			throw -1;
		// 表示整个 json 对象
		Json::Value root;
		// 标识交互参数
		Json::Value MMIParamContext;
		// 标识详细数据项
		Json::Value Data;
		// 标识设置数据项区域
		Json::Value ItemData;
		// 标识显示文字区域
		Json::Value ViewText;
		// 标识显示声音区域
		Json::Value ViewVoice;
		// 标识显示图形区域
		Json::Value ViewGraph;

		Json::Value item;

		//标识版本
		MMIParamContext["Version"]		= Json::Value(pDVJsonParamMMI->Version);

		Data["ClearMMI"]				= Json::Value(pDVJsonParamMMI->ClearMMI);

		Data["ItemCount"]				= Json::Value(int(pDVJsonParamMMI->Text.GetSize() + pDVJsonParamMMI->Voice.GetSize() + pDVJsonParamMMI->Graph.GetSize()));

		for(int i = 0;i<pDVJsonParamMMI->Text.GetSize();++i)
		{
			item["Type"]		= Json::Value(pDVJsonParamMMI->Text.GetAt(i).Type);
			item["Sub"]			= Json::Value(pDVJsonParamMMI->Text.GetAt(i).Sub);
			item["Row"]			= Json::Value(pDVJsonParamMMI->Text.GetAt(i).Row);
			item["IsScroll"]	= Json::Value(pDVJsonParamMMI->Text.GetAt(i).IsScroll);
			item["Context"]		= Json::Value(pDVJsonParamMMI->Text.GetAt(i).Context);

			ViewText.append(item);
		}

		for(int i = 0;i<pDVJsonParamMMI->Voice.GetSize();++i)
		{
			item["Type"]		= Json::Value(pDVJsonParamMMI->Voice.GetAt(i).Type);
			item["VoiceNo"]		= Json::Value(pDVJsonParamMMI->Voice.GetAt(i).No);

			ViewVoice.append(item);
		}

		for(int i = 0;i<pDVJsonParamMMI->Graph.GetSize();++i)
		{
			item["Type"]		= Json::Value(pDVJsonParamMMI->Graph.GetAt(i).Type);
			item["VoiceNo"]		= Json::Value(pDVJsonParamMMI->Graph.GetAt(i).No);

			ViewGraph.append(item);
		}

		ItemData["Text"]  = ViewText;
		ItemData["Voice"] = ViewVoice;
		ItemData["Graph"] = ViewGraph;

		Data["ItemData"]  = ItemData;

		MMIParamContext["Data"]		= Data;

		root["MMIParamContext"]		= MMIParamContext;

		//转化为字符串
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;

	/****************************************************************/
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CJsonParamOperate::FormatParamData2Json
//     
// 功能描述：将接收到的数据结构转化为标准的JSON格式 -- 支付请求参数数据变换(扣费结果)
//     
// 输入参数：IN const CDVJsonParamDebitResult* pDVJsonParamDebitResult
// 输出参数：CString
// 编写人员：ROCY
// 编写时间：2016年8月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamDebitResult* pDVJsonParamDebitResult)
{
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamDebitResult)
			throw -1;
		// 表示整个 json 对象
		Json::Value root;
		// 标识用户参数
		Json::Value ResultContext;
		// 标识详细数据项
		Json::Value Data;
		// 标识关键项
		Json::Value KeyItem;
		/***********************************************************构造JSAO数据结构***********************************************************/
		
		//标识版本
		ResultContext["Version"]		= Json::Value(pDVJsonParamDebitResult->Version);
		//标识扣款类型
		Data["ResultType"]				= Json::Value(pDVJsonParamDebitResult->ResultType);
		//标识执行状态
		Data["ExecuteCode"]				= Json::Value(pDVJsonParamDebitResult->ExecuteCode);
		//标识执行结果描述
		Data["ExecuteDesc"]				= Json::Value(pDVJsonParamDebitResult->ExecuteDesc);

		//关键数据项 -- 交易安全码
		KeyItem["TradeSecurityCode"]	= Json::Value(pDVJsonParamDebitResult->TradeSecurityCode);
		//关键数据项 -- 第三方支付平台类型
		KeyItem["PayPlatformType"]		= Json::Value(pDVJsonParamDebitResult->PayPlatformType);
		//关键数据项 -- 交易识别码
		KeyItem["PayIdentifier"]		= Json::Value(pDVJsonParamDebitResult->PayIdentifier);
		//关键数据项 -- 订单号
		KeyItem["DebitOrder"]			= Json::Value(pDVJsonParamDebitResult->DebitOrder);
		//关键数据项 -- 服务端扣款时间
		KeyItem["DebitTime"]			= Json::Value(pDVJsonParamDebitResult->DebitTime.Format(_T("%Y-%m-%d %H:%M:%S")));
		//关键数据项 -- 消耗时间
		KeyItem["ConsumeTime"]			= Json::Value(pDVJsonParamDebitResult->ConsumeTime);

		Data["TradeKeyItem"]		= KeyItem;
		ResultContext["Data"]		= Data;
		root["DebitResultContext"]	= ResultContext;
		//转化为字符串
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}
	return strReturn;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CJsonParamOperate::FormatParamData2Json
//     
// 功能描述：将接收到的数据格式转化为标准的JSON格式 -- 用户数据参数数据变换
//     
// 输入参数：IN const CDVJsonParamAccount* pDVJsonParamAccount
// 输出参数：CString
// 编写人员：ROCY
// 编写时间：2016年8月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CString CJsonParamOperate::FormatParamData2Json(IN const CDVJsonParamAccount* pDVJsonParamAccount)
{
	CString strReturn;
	try
	{
		if(NULL == pDVJsonParamAccount)
			throw -1;
		// 表示整个 json 对象
		Json::Value root;
		// 标识用户参数
		Json::Value ParamAccount;
		// 标识详细数据项
		Json::Value Data;
		/***********************************************************构造JSAO数据结构***********************************************************/
		
		//标识版本
		ParamAccount["Version"]	= Json::Value(pDVJsonParamAccount->Version);
		//标识数据类型
		Data["Type"]			= Json::Value(pDVJsonParamAccount->DataType);
		//标识数据来源
		Data["Source"]			= Json::Value(pDVJsonParamAccount->DataSrc);
		//标识数据内容
		Data["Context"]			= Json::Value(pDVJsonParamAccount->DataContext);

		ParamAccount["Data"]	= Data;
		root["AccountContext"]	= ParamAccount;
		//转化为字符串
		Json::FastWriter fast_writer;
		strReturn.Format(_T("%s"),fast_writer.write(root).c_str());
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:
			break;
		}
	}

	return strReturn;
}



////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CJsonParamOperate::FormatResponseResult2Class
//     
// 功能描述：将JSON格式字符串转化为标准数据结构类
//     
// 输入参数：IN const CString& strResponseResult
//         ：IN const int& iDataType
// 输出参数：CDVJsonBase*
// 编写人员：ROCY
// 编写时间：2016年8月15日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.1
// 备注说明：
//     
////////////////////////////////////////////////////////////////////////////////////////////////
CDVJsonBase* CJsonParamOperate::FormatResponseResult2Class(IN const CString& strResponseResult,IN const int& iDataType)
{
	CDVJsonBase* pDVJsonBase = NULL;
	try
	{
		switch(iDataType)
		{
		case JSON_DT_PARAM_ACCOUNT:
			{
				pDVJsonBase = new CDVJsonParamAccount();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//进行数据格式转换
				Json::Reader reader;
				Json::Value  root;
				Json::Value  AccountContext;
				Json::Value  Data;

				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//获取用户信息区
				if(Json::nullValue == root["AccountContext"].type())
					throw -4;
				AccountContext = root["AccountContext"];

				//获取版本
				if(Json::stringValue != AccountContext["Version"].type())
					throw -4;
				pDVJsonBase->Version = AccountContext["Version"].asCString();
				
				//获取数据信息区
				if(Json::nullValue == AccountContext["Data"].type())
					throw -4;
				Data = AccountContext["Data"];

				//获取用户数据类型
				if(Json::intValue != Data["Type"].type())
					throw -4;
				((CDVJsonParamAccount*)pDVJsonBase)->DataType = Data["Type"].asInt();

				//获取用户数据来源
				if(Json::intValue != Data["Source"].type())
					throw -4;
				((CDVJsonParamAccount*)pDVJsonBase)->DataSrc = Data["Source"].asInt();

				//获取用户数据内容
				if(Json::stringValue != Data["Context"].type())
					throw -4;
				((CDVJsonParamAccount*)pDVJsonBase)->DataContext = Data["Context"].asCString();

			}
			break;
		case JSON_DT_PARAM_DEBIT_MONEY:
			{
				pDVJsonBase = new CDVJsonParamDebitMoney();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//进行数据格式转换
				Json::Reader reader;
				Json::Value  root;
				Json::Value  DebitParamContext;
				Json::Value  Data;
				Json::Value  Debit;
				Json::Value  Vehicle;
				Json::Value  Operation;
				Json::Value	 Entry;
				
				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//获取交易数据区
				if(Json::nullValue == root["DebitParamContext"].type())
					throw -4;
				DebitParamContext = root["DebitParamContext"];

				//获取数据明细区
				if(Json::nullValue == DebitParamContext["Data"].type())
					throw -4;
				Data = DebitParamContext["Data"];

				//获取超时时间
				if(Json::intValue != Data["OverTime"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->OverTime = Data["OverTime"].asInt();

				//获取扣款操作模式
				if(Json::intValue != Data["OperationMode"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->OperationMode = Data["OperationMode"].asInt();
				
				//获取扣款信息区
				if(Json::nullValue == Data["Debit"].type())
					throw -4;
				Debit = Data["Debit"];

				//获取扣款金额
				if(Json::intValue != Debit["Money"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->Money = Debit["Money"].asInt();

				//获取交易识别码
				if(Json::stringValue != Debit["PayIdentifier"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->PayIdentifier = Debit["PayIdentifier"].asCString();

				//获取支付凭证类型
				if(Json::intValue != Debit["PayCertificateType"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->PayCertificateType = Debit["PayCertificateType"].asInt();

				//获取支付凭证内容
				if(Json::stringValue != Debit["PayCertificateCode"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->PayCertificateCode = Debit["PayCertificateCode"].asCString();

				//获取扣款主分类
				if(Json::intValue != Debit["Type"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->DebitMainType = Debit["Type"].asInt();

				//获取过车时间
				if(Json::stringValue != Debit["SubTime"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->SubTime = CTime(::CString2time_t(Debit["SubTime"].asCString()));

				//获取车辆信息区
				if(Json::nullValue == Data["Vehicle"].type())
					throw -4;
				Vehicle = Data["Vehicle"];
				
				//获取车种
				if(Json::intValue != Vehicle["Type"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->VehicleType = Vehicle["Type"].asInt();

				//获取车型
				if(Json::intValue != Vehicle["Class"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->VehicleClass = Vehicle["Class"].asInt();

				//获取车牌
				if(Json::stringValue != Vehicle["License"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->VehicleLicense = Vehicle["License"].asCString();

				//获取车牌颜色
				if(Json::intValue != Vehicle["VLColor"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->VLColor = Vehicle["VLColor"].asInt();

				//获取轴数
				if(Json::intValue != Vehicle["AxisCount"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->AxisCount = Vehicle["AxisCount"].asInt();
			
				//获取装载重量
				if(Json::intValue != Vehicle["Weight"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->Weight = Vehicle["Weight"].asInt();

				//获取限重
				if(Json::intValue == Vehicle["LimitWeight"].type())
				{
					((CDVJsonParamDebitMoney*)pDVJsonBase)->LimitWeight = Vehicle["LimitWeight"].asInt();
				}

				//获取交易信息区
				if(Json::nullValue == Data["Operation"].type())
					throw -4;
				Operation = Data["Operation"];

				//获取通行凭证类型
				if(Json::intValue != Operation["PassCertificateType"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->PassCertificateType = Operation["PassCertificateType"].asInt();

				//获取通行凭证卡号
				if(Json::stringValue != Operation["CardID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->CardID = Operation["CardID"].asCString();

				//获取统计日
				if(Json::intValue == Operation["TollDate"].type())
				{
					((CDVJsonParamDebitMoney*)pDVJsonBase)->TollDate = Operation["TollDate"].asInt();
				}
				if(0 == ((CDVJsonParamDebitMoney*)pDVJsonBase)->TollDate)
				{
					if(Json::stringValue != Operation["TollDate"].type())
						throw -4;
					((CDVJsonParamDebitMoney*)pDVJsonBase)->TollDate = _ttoi(Operation["TollDate"].asCString());
				}

				//获取统计班次
				if(Json::intValue != Operation["ShiftID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->ShiftID = Operation["ShiftID"].asInt();

				//获取打印票据号
				if(Json::stringValue == Vehicle["TicketNo"].type())
				{
					((CDVJsonParamDebitMoney*)pDVJsonBase)->TicketNo = Vehicle["TicketNo"].asCString();
				}

				//获取入口信息区
				if(Json::nullValue == Data["Entry"].type())
					throw -4;
				Entry = Data["Entry"];

				//获取入口区域编码
				if(Json::intValue != Entry["AreaID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryAreaID = Entry["AreaID"].asInt();

				//获取入口路段编码
				if(Json::intValue != Entry["RoadID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryRoadID = Entry["RoadID"].asInt();

				//获取入口站点编码
				if(Json::intValue != Entry["StationID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryStationID = Entry["StationID"].asInt();

				//获取入口站点名称
				if(Json::stringValue != Entry["StationName"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryStationName = Entry["StationName"].asCString();
				
				//获取入口车道编码
				if(Json::intValue != Entry["LaneID"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryLane = Entry["LaneID"].asInt();

				//获取入口车种
				if(Json::intValue != Entry["VType"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryVehicleType = Entry["VType"].asInt();

				//获取入口车型
				if(Json::intValue != Entry["VClass"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryVehicleClass = Entry["VClass"].asInt();

				//获取入口车牌
				if(Json::stringValue != Entry["License"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryVehicleLicense = Entry["License"].asCString();

				//获取入口车牌颜色
				if(Json::intValue != Entry["VColor"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryVLColor = Entry["VColor"].asInt();

				//获取入口时间
				if(Json::stringValue != Entry["Time"].type())
					throw -4;
				((CDVJsonParamDebitMoney*)pDVJsonBase)->EntryTime = CTime(::CString2time_t(Entry["Time"].asCString()));
			}
			break;
		case JSON_DT_PARAM_DEBIT_CANCLE:
			{
				pDVJsonBase = new CDVJsonParamDebitCancel();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//进行数据格式转换
				Json::Reader reader;
				Json::Value  root;
				Json::Value  DebitCancelParamContext;
				Json::Value  Data;
				Json::Value  TradeKeyItem;

				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//获取用户信息区
				if(Json::nullValue == root["DebitCancelParamContext"].type())
					throw -4;
				DebitCancelParamContext = root["DebitCancelParamContext"];

				//获取版本
				if(Json::stringValue != DebitCancelParamContext["Version"].type())
					throw -4;
				((CDVJsonParamDebitCancel*)pDVJsonBase)->Version = DebitCancelParamContext["Version"].asCString();
				
				//获取数据信息区
				if(Json::nullValue == DebitCancelParamContext["Data"].type())
					throw -4;
				Data = DebitCancelParamContext["Data"];

				//获取交易数据信息区
		//		if(Json::nullValue == Data["TradeKeyItem"].type())
		//			throw -4;
		//		TradeKeyItem = Data["TradeKeyItem"];

				//获取交易识别码
				if(Json::stringValue != Data["PayIdentifier"].type())
					throw -4;
				((CDVJsonParamDebitCancel*)pDVJsonBase)->PayIdentifier = TradeKeyItem["PayIdentifier"].asCString();
			}
			break;
		case JSON_DT_PARAM_DEBIT_RESULT:
			{
				pDVJsonBase = new CDVJsonParamDebitResult();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//进行数据格式转换
				Json::Reader reader;
				Json::Value  root;
				Json::Value  DebitResultContext;
				Json::Value  Data;
				Json::Value  TradeKeyItem;
				Json::Value  ETCTradeInfo;

				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//获取扣款结果信息区
				if(Json::nullValue == root["DebitResultContext"].type())
					throw -4;
				DebitResultContext = root["DebitResultContext"];

				//获取版本
				if(Json::stringValue != DebitResultContext["Version"].type())
					throw -4;
				pDVJsonBase->Version = DebitResultContext["Version"].asCString();
				
				//获取数据信息区
				if(Json::nullValue == DebitResultContext["Data"].type())
					throw -4;
				Data = DebitResultContext["Data"];

				//获取结果类型
				if(Json::intValue != Data["ResultType"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->ResultType = Data["ResultType"].asInt();

				//获取执行状态
				if(Json::intValue != Data["ExecuteCode"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->ExecuteCode = Data["ExecuteCode"].asInt();
				
				//获取执行结果描述
				if(Json::stringValue != Data["ExecuteDesc"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->ExecuteDesc = Data["ExecuteDesc"].asCString();

				//获取数据信息区
				if(Json::nullValue == Data["TradeKeyItem"].type())
					throw -4;
				TradeKeyItem = Data["TradeKeyItem"];


				//获取交易识别码
				if(Json::stringValue != TradeKeyItem["PayIdentifier"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->PayIdentifier = TradeKeyItem["PayIdentifier"].asCString();
				
				//获取第三方支付平台类型
				if(Json::intValue != TradeKeyItem["PayPlatformType"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->PayPlatformType = TradeKeyItem["PayPlatformType"].asInt();
				
				//获取交易安全码
				if(Json::stringValue != TradeKeyItem["TradeSecurityCode"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->TradeSecurityCode = TradeKeyItem["TradeSecurityCode"].asCString();
				
				//获取交订单号
				if(Json::stringValue != TradeKeyItem["DebitOrder"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->DebitOrder = TradeKeyItem["DebitOrder"].asCString();
				
				//获取服务器扣款时间
				if(Json::stringValue != TradeKeyItem["DebitTime"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->DebitTime = CString2time_t(TradeKeyItem["DebitTime"].asCString());//CTime(0);//AccountContext["Context"];
				
				//获取消耗时间
				if(Json::intValue != TradeKeyItem["ConsumeTime"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->ConsumeTime = TradeKeyItem["ConsumeTime"].asInt();

				//获取打票结果
				if(Json::intValue != TradeKeyItem["PrintResultType"].type())
					throw -4;
				((CDVJsonParamDebitResult*)pDVJsonBase)->PrintTicketResult = TradeKeyItem["PrintResultType"].asInt();

				//ETC
				if( Json::nullValue == TradeKeyItem[ "ETCTradeInfo" ].type( ) )
					throw -4;
				ETCTradeInfo = TradeKeyItem[ "ETCTradeInfo" ];

				if( Json::stringValue != ETCTradeInfo[ "ETCProFile0015" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCProFile0015 = ETCTradeInfo[ "ETCProFile0015" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTerminalNo" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTerminalNo = ETCTradeInfo[ "ETCTerminalNo" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCProTradeNo" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCProTradeNo = ETCTradeInfo[ "ETCProTradeNo" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTerminalTradeNo" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTerminalTradeNo = ETCTradeInfo[ "ETCTerminalTradeNo" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTradeDate" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTradeDate = ETCTradeInfo[ "ETCTradeDate" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTradeTime" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTradeTime = ETCTradeInfo[ "ETCTradeTime" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTac" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTac = ETCTradeInfo[ "ETCTac" ].asCString();

				if( Json::stringValue != ETCTradeInfo[ "ETCTradeType" ].type( ) ) 
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_strETCTac = ETCTradeInfo[ "ETCTradeType" ].asCString();

				if( Json::intValue != ETCTradeInfo[" ETCBeforeBalance" ].type( ) )
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_iETCBeforeBalance = ETCTradeInfo[ "ETCBeforeBalance" ].asInt();

				if( Json::intValue != ETCTradeInfo[" ETCAfterBalance" ].type( ) )
					throw -4;
				( ( CDVJsonParamDebitResult* )pDVJsonBase )->m_iETCAfterBalance = ETCTradeInfo[ "ETCAfterBalance" ].asInt();


			}
			break;
		case JSON_DT_PARAM_MMI:
			{
				pDVJsonBase = new CDVJsonParamMMI();
				if(NULL == pDVJsonBase)
				{
					throw -1;
				}
				//进行数据格式转换
				Json::Reader reader;
				Json::Value  root;
				Json::Value  MMIParamContext;
				Json::Value  Data;
				Json::Value  ItemData;
				Json::Value  aryTextView;
				Json::Value  aryVoiceView;
				Json::Value  aryGraphView;

				CString strResult(strResponseResult);
				if(!reader.parse(strResult.GetBuffer(0),root))
				{
					strResult.ReleaseBuffer();
					throw -2;
				}
				
				if(Json::nullValue == root.type())
					throw -3;

				//获取交互界面设置信息区
				if(Json::nullValue == root["MMIParamContext"].type())
					throw -4;
				MMIParamContext = root["MMIParamContext"];

				//获取版本号
				if(Json::stringValue != MMIParamContext["Version"].type())
					throw -4;
				((CDVJsonParamMMI*)pDVJsonBase)->Version = MMIParamContext["Version"].asCString();

				//获取清屏标识
				if( (Json::booleanValue != Data["ClearMMI"].type()) && (Json::intValue != Data["ClearMMI"].type()))
					throw -4;
				if(Json::booleanValue == Data["ClearMMI"].type())
				{
					((CDVJsonParamMMI*)pDVJsonBase)->ClearMMI = (int)Data["ClearMMI"].asBool();
				}
				else if(Json::intValue == Data["ClearMMI"].type())
				{
					((CDVJsonParamMMI*)pDVJsonBase)->ClearMMI = Data["ClearMMI"].asInt();
				}

				//获取数据区域
				if(Json::nullValue == MMIParamContext["Data"].type())
					throw -4;
				Data = MMIParamContext["Data"];


				//非清屏指令
				if(0 == ((CDVJsonParamMMI*)pDVJsonBase)->ClearMMI)
				{
					//获取设置项内容
					if(Json::intValue != Data["VType"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryVehicleType = Data["VType"].asInt();

					if(Json::intValue != Data["VClass"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryVehicleClass = Data["VClass"].asInt();

					if(Json::intValue != Data["Money"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->Money = Data["Money"].asInt();

					if(Json::intValue != Data["AixsCount"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->AxisCount = Data["AxisCount"].asInt();

					if(Json::intValue != Data["LimitWeight"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->LimitWeight = Data["LimitWeight"].asInt();
	
					if(Json::intValue != Data["Balance"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->nBalance = Data["Balance"].asInt();

					if(Json::stringValue != Data["StationName"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryStationName = Data["StationName"].asCString();

					if(Json::intValue != Data["VColor"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryVLColor = Data["VColor"].asInt();

					if(Json::stringValue != Data["License"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->EntryVehicleLicense = Data["License"].asCString();

					if(Json::intValue != Data["CroseNum"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->nCroseNum = Data["CroseNum"].asInt();
	
					if(Json::intValue != Data["Program"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->nProgram = Data["Program"].asInt();
	
					if(Json::stringValue != Data["Remark1"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->csRemark1 = Data["Remark1"].asCString();

					if(Json::stringValue != Data["Remark2"].type())
						throw -4;
					((CDVJsonParamMMI*)pDVJsonBase)->csRemark2 = Data["Remark2"].asCString();

					}

					//获取声音显示项
					if(Json::nullValue == ItemData["Voice"].type())
						throw -4;
					aryVoiceView = root["Voice"];
					for (int i=0; i < aryVoiceView.size();++i)
					{
						tagMMIItemVoice sVoice = {0};
						if(Json::intValue == aryVoiceView[i]["Type"].type())
						{
							sVoice.Type = aryVoiceView[i]["Type"].asInt();
						}
						if(Json::intValue == aryVoiceView[i]["No"].type())
						{
							sVoice.No = aryVoiceView[i]["No"].asInt();
						}
						
						((CDVJsonParamMMI*)pDVJsonBase)->Voice.Add(sVoice);
					}

					//获取图形显示项
					if(Json::nullValue == ItemData["Voice"].type())
						throw -4;
					aryGraphView = root["Voice"];
					for (int i=0; i < aryGraphView.size();++i)
					{
						tagMMIItemGraph sGraph = {0};
						if(Json::intValue == aryGraphView[i]["Type"].type())
						{
							sGraph.Type = aryGraphView[i]["Type"].asInt();
						}
						if(Json::intValue == aryGraphView[i]["No"].type())
						{
							sGraph.No = aryGraphView[i]["No"].asInt();
						}
						
						((CDVJsonParamMMI*)pDVJsonBase)->Graph.Add(sGraph);
					}
				
			}
			break;
		}
	}
	catch(int& iErrorCode)
	{
		switch(iErrorCode)
		{
		case -1:

			break;
		case -2:
			break;
		case -3:
      		break;
		case -4:
			break;
		default:
			break;
		}
	}

	return pDVJsonBase;
}
