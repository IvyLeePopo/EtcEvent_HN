/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVXMLDebitResult.cpp
 * ժ    Ҫ���ۿ�������XML���ݽṹ��ʵ���ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��2��
 */
#include "StdAfx.h"
#include "DVXMLDebitResult.h"

CDVXMLDebitResult::CDVXMLDebitResult(void)
	:ResultType(0)
	,ExecuteCode(-1)
	,ExecuteDesc(_T(""))

	,PayIdentifier(_T(""))
	,PayPlatformType(0)
	,TradeSecurityCode(_T(""))
	,DebitOrder(_T(""))
{
	XMLFunctionDesc.Format(_T("DebitResultContext"));
}

CDVXMLDebitResult::~CDVXMLDebitResult(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
// �������ƣ�CDVXMLDebitResult::GetXMLString
//     
// ��������������XML�ַ���
//     
// ���������void
// ���������const
// ��д��Ա��ROCY
// ��дʱ�䣺2016��8��2��
// �޸���Ա��
// �޸�ʱ�䣺
// �����汾��1.0.0.1
// ��ע˵����
//     
////////////////////////////////////////////////////////////////////////////////////////////////
const CString CDVXMLDebitResult::GetXMLString(void)
{
	CString strXMLString;
	CMarkup xml; 
	xml.SetDoc( strXMLString );

	xml.AddElem(XMLFunctionDesc);
	xml.IntoElem();

	xml.AddElem(_T("Version"),XMLVersion); 
	xml.AddElem(_T("Data")); 
	xml.IntoElem();

	xml.AddElem(_T("ResultType"),	ResultType); 
	xml.AddElem(_T("ExecuteCode"),	ExecuteCode); 
	xml.AddElem(_T("ExecuteDesc"),	ExecuteDesc); 

	xml.AddElem(_T("TradeKeyItem")); 
	xml.IntoElem();
	xml.AddElem(_T("TradeSecurityCode"),	TradeSecurityCode); 
	xml.AddElem(_T("PayPlatformType"),		PayPlatformType); 
	xml.AddElem(_T("PayIdentifier"),		PayIdentifier); 
	xml.AddElem(_T("DebitOrder"),			DebitOrder); 
	xml.AddElem(_T("DebitTime"),			DebitTime.Format(_T("%Y-%m-%d %H:%M:%S"))); 
	xml.AddElem(_T("ConsumeTime"),			ConsumeTime);

	//add 2018-09
	xml.AddElem(_T("ETCTradeInfo"));
	xml.IntoElem();

	xml.AddElem(_T("ETCExecuteCode"),ExpandETCInfo.ETCExcuteCode);
	xml.AddElem(_T("IsUseVirtualETCCard"),ExpandETCInfo.IsUseVirtualETCCard);
	xml.AddElem(_T("VirtualETCCardID"),ExpandETCInfo.VirtualETCCardID);
    xml.AddElem(_T("IssueFlag"),ExpandETCInfo.IssueFlag);
    xml.AddElem(_T("IssueIndex"),ExpandETCInfo.IssueIndex);
    xml.AddElem(_T("ETCCardType"),ExpandETCInfo.ETCCardType);
    xml.AddElem(_T("ETCVehicleLicense"),ExpandETCInfo.ETCVehicleLicense);
    xml.AddElem(_T("ETCBeforeBalance"),ExpandETCInfo.ETCBeforeBalance);
    xml.AddElem(_T("ETCAfterBalance"),ExpandETCInfo.ETCAfterBalance);
    xml.AddElem(_T("ETCProTradeNo"),ExpandETCInfo.ETCProTradeNo);
    xml.AddElem(_T("ETCTerminalTradeNo"),ExpandETCInfo.ETCTerminalTradeNo);
    xml.AddElem(_T("ETCTradeDate"),ExpandETCInfo.ETCTradeDate);
    xml.AddElem(_T("ETCTradeTime"),ExpandETCInfo.ETCTradeTime);
    xml.AddElem(_T("ETCTac"),ExpandETCInfo.ETCTac);

	xml.OutOfElem();

	xml.OutOfElem();
	xml.OutOfElem();

	strXMLString = xml.GetDoc();

	return strXMLString;
}