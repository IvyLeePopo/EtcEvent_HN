/*
 * Copyright(C) 2016,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�DVJsonParamDebitResult.h
 * ժ    Ҫ����ȡ�ۿ�ӿڲ���JSON���ݸ�ʽ�����������ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2016��8��15��
 */
#pragma once
#include "dvjsonbase.h"

class CDVJsonParamDebitResult :
	public CDVJsonBase
{
public:
	CDVJsonParamDebitResult(void);
	~CDVJsonParamDebitResult(void);
//���������
	CDVJsonParamDebitResult& operator=(IN const CDVJsonParamDebitResult& cSrc);
//Interface
public:
	//��������
	void  Reset(IN const int& iType=0);
	//��������У����
	DWORD CreateInnerVerifyCode(void){return 0;}
	//�ڲ�������У��
	BOOL  InnerVerify(void){return TRUE;}
//����
public:
	//�������
	int		ResultType;
	//ִ��״̬
	int		ExecuteCode;
	//ִ�н������
	CString ExecuteDesc;


	//����ʶ����
	CString PayIdentifier;
	//������֧��ƽ̨����
	int		PayPlatformType;
	//���װ�ȫ��
	CString TradeSecurityCode;
	//������
	CString DebitOrder;
	//�������ۿ�ʱ��
	CTime	DebitTime;

	//����ʱ�䣬��λ������
	int		ConsumeTime;
	

	//add 2018-09
	ETCCardData ExpandETCInfo;
};