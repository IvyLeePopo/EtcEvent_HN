#include "stdafx.h"
#include "UniQrProtocol.h"

#include "ScanMachine.h"

#define TWPD_STARTFLAG  0xDF    //֡��ʼȡֵ
#define TWPD_ENDFLAG    0xFD    //֡����ȡֵ
#define TWPD_STARTFLAG_ESCAPE_FIRST 0xDE
#define TWPD_STARTFLAG_ESCAPE_SECOND 0x01
#define TWPD_ENDFLAG_ESCAPE_FIRST 0xFC
#define TWPD_ENDFLAG_ESCAPE_SECOND 0x01

#define TWPD_DATA_OFFSET 7

#include "json/json.h"
#include <algorithm>

extern CScanMachineApp theApp;

std::string MakeupPackage(unsigned short id, const std::vector<unsigned char>& data, unsigned char top, unsigned char sub);
std::string NextGuid();
void GenerateC0(const std::vector<unsigned char>& real_data, std::vector<std::string>& c0_queue);

bool CheckPackage(const std::vector<unsigned char>& data);

void str_replace(std::string & str, const std::string & strsrc, const std::string &strdst);


std::string PackDataInitTerminal(unsigned short id)
{	
    std::vector<unsigned char> data;

	//���豸�����ϵ��Լ�ʱ����ʱ���͵ĳ�ʼ����Ϣ��ʱ��Ҫ��ȡ��ǰ��ʱ����Ϊ��׼----2021-11-17
	theApp.m_pA5InitJsonData = modifyA5CurrentTime(theApp.m_pA5InitJsonData);

//	LogCallBack("��ʼ��A5��Ϣ��"+ theApp.m_pA5InitJsonData);


    std::copy(theApp.m_pA5InitJsonData.begin(), theApp.m_pA5InitJsonData.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0xA5, 0x00);
}


std::string PackDataEventDealStart(const char* ParamContext, int iParamSize, unsigned short id)
{
    std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x20);
}

std::string PackDataEventCheckVehQueue(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x21);
}


std::string PackDataEventCheckVehInfo(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x22);
}


std::string PackDataEventCheckEntryInfo(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x23);
}

std::string PackDataEventShowFeeInfo(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x24);
}


std::string PackDataEventPayResultShow(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x25);
}

std::string PackDataEventStartScan(int iFeeValue, unsigned short id)
{
    Json::Value root_node;
	Json::Value ScanData;
	ScanData["FeeValue"] = Json::Value(iFeeValue);
	root_node["ScanData"] = ScanData;

    Json::FastWriter json_writer;
    std::string json_data = json_writer.write(root_node);

    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x26);
}

std::string PackDataEventStopScan(unsigned short id)
{
	std::vector<unsigned char> data;
    return MakeupPackage(id, data, 0x50, 0x27);
}

std::string PackDataEventDealStop(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x28);
}


std::string PackDataEventCardOperationNotify(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x41);
}

std::string PackDataEventModifyVehQueue(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x42);
}

std::string PackDataEventFeeAuthorize(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x50, 0x43);
}

std::string PackDataEventAuthorize(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
	std::vector<unsigned char> data;
	std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

	return MakeupPackage(id, data, 0x50, 0x60);
}

std::string PackDataEventDelVehQueueResult(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
	std::vector<unsigned char> data;
	std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

	return MakeupPackage(id, data, 0x50, 0x61);
}

std::string PackDataDeviceNo(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
	std::vector<unsigned char> data;
	std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

	return MakeupPackage(id, data, 0xA6, 0x05);
}

std::string PackDataQRcodeDisplay(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x70, 0x10);
}

std::string PackDataFeeDisplay(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x70, 0x11);
}

std::string PackDataPayResultDisplay(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x70, 0x12);
}

std::string PackDataClearScreen(const char* ParamContext, int iParamSize, unsigned short id)
{
	std::string json_data = ParamContext;
    std::vector<unsigned char> data;
    std::copy(json_data.begin(), json_data.end(), std::back_inserter(data));

    return MakeupPackage(id, data, 0x70, 0x13);
}


bool CheckTargetACK(const UniQrDriver& urd, unsigned short id)
{
    for (std::list<ACKItem>::const_iterator it = urd.ack_group.begin(); it != urd.ack_group.end(); it++)
    {
        if (it->id == id)
        {
            return (it->state == 0);
        }
    }

    return false;
}

void ParseProtocolData(UniQrDriver& uqd, std::vector<std::string>& c0_queue)
{
    std::list<unsigned char>& ref_datas = uqd.uncomplete_datas;
	uqd.isEtcEventResp = false;
	uqd.EtcEventId = 0;
    while (!ref_datas.empty())
    {
        std::list<unsigned char>::iterator beg_it = std::find(ref_datas.begin(), ref_datas.end(), TWPD_STARTFLAG);
        if (beg_it != ref_datas.begin())
        {
            std::vector<unsigned char> tmp;
            tmp.insert(tmp.end(), ref_datas.begin(), beg_it);
            LogCallBack("����������������:" + Bin2Hex(tmp));
            ref_datas.erase(ref_datas.begin(), beg_it);
            continue;
        }
        std::list<unsigned char>::iterator end_it = std::find(beg_it, ref_datas.end(), TWPD_ENDFLAG);
        if (end_it == ref_datas.end())
        {
            break;
        }

        std::vector<unsigned char> one_data;
        std::advance(end_it, 1);
        one_data.insert(one_data.end(), ref_datas.begin(), end_it);
        ref_datas.erase(ref_datas.begin(), end_it);

        std::vector<unsigned char> real_data;
        UnEscapeComProtocol(one_data, real_data);
        LogCallBack("�յ��豸����:" + Bin2Hex(real_data));
        //У����ȷ��
        if (!CheckPackage(real_data))
        {
            continue;
        }


        if (real_data[5] == 0xC0)
        {
            ParseProtocolACK(real_data, uqd);
            continue;
        }

        GenerateC0(real_data, c0_queue);
        if (real_data[5] == 0xB1)
        {
            ParseProtocolHeart(real_data, uqd);
            continue;
        }
        if (real_data[5] == 0xD2)
        {
            uqd.initdev_flag = false;
            continue;
        }
		if (real_data[5] = 0xB6 && real_data[6] == 0x05)
		{
			//�豸���к�
			ParseProtocolDeviceNo(real_data,uqd);
			break;
		}

        switch (real_data[6]) //�˴����Ըĳɱ�����
        {
		case 0x30:
			//���鴦��ʼӦ��
			ParseProtocolEventDealStartResp(real_data, uqd);
            break;
		case 0x31:
			//��������ȷ��Ӧ��
			ParseProtocolEventCheckVehQueueResp(real_data, uqd);
            break;
		case 0x32:
			//������Ϣȷ��Ӧ��
			ParseProtocolEventCheckVehInfoResp(real_data, uqd);
            break;
		case 0x33:
			//�������ȷ��Ӧ��
			ParseProtocolEventCheckEntryInfoResp(real_data, uqd);
            break;
		case 0x34:
			//�Ѷ���ʾ����Ӧ��
			ParseProtocolEventShowFeeInfoResp(real_data, uqd);
            break;
		case 0x35:
			//չʾ֧��״̬��ϢӦ��
			ParseProtocolEventPayResultShowResp(real_data, uqd);
            break;
		case 0x36:
			//ɨ�赽��ά������Ӧ��
			ParseProtocolStartScanResp(real_data, uqd);			
            break;
		case 0x37:
			//ֹͣɨ��Ӧ��
			ParseProtocolStopScanResp(real_data, uqd);
            break;
		case 0x38:
			//���鴦�����Ӧ��
			ParseProtocolEventDealStopResp(real_data, uqd);
            break;
		case 0x39:
			//�����˵�Ӧ��
			ParseProtocolAuditResp(real_data, uqd);
            break;
		case 0x51:
			//ETC��������ʾӦ��
			ParseProtocolEventCardOperationNotifyResp(real_data, uqd);
			break;
		case 0x52:
			//�޸ĳ�������Ӧ��
			ParseProtocolEventModifyVehQueueResp(real_data, uqd);
			break;
		case 0x53:
			//��С�Ѷ���ȨӦ��
			ParseProtocolEventFeeAuthorize(real_data, uqd);
			break;
		case 0x54:
			//�豸����֪ͨ
			ParseProtocolEventButtonClicked(real_data, uqd);
			break;
		case 0x70:
			//������Ȩ
			ParseProtocolEventAuthorize(real_data, uqd);
			break;
		case 0x80:
			//ɾ����������֪ͨ
			ParseProtocolEventDelVehQueueCmd(real_data, uqd);
			break;
		case 0x71:
			//ɾ�����н��Ӧ��
			ParseProtocolEventDelVehQueueResultResp(real_data, uqd);
			break;
        }
    }

    // ����������
    while(uqd.ack_group.size() > COMITEM_MAXSIZE)
    {
        uqd.ack_group.pop_back();
    }
}


void ParseProtocolACK(const std::vector<unsigned char>&ack_data, UniQrDriver& uqd)
{
    ACKItem ack;
    ack.state = ack_data[6];
    ack.id = MAKEWORDBIG(ack_data[9], ack_data[10]);
    uqd.ack_group.push_front(ack);
}


void ParseProtocolHeart(const std::vector<unsigned char>&heart_data, UniQrDriver& uqd)
{
    uqd.device_state = MAKEINTBIG(heart_data[11], heart_data[12], heart_data[13], heart_data[14]);
}


/*************************************��������Ӧ��****************************************************************/
void ParseProtocolEventDealStartResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("���鴦��ʼӦ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 601;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ����鴦��ʼӦ������:" + Resp);
	return;
}

void ParseProtocolEventCheckVehQueueResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("ȷ�ϳ�����������Ӧ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 602;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ�ȷ�ϳ�����������Ӧ������:" + Resp);
	return;
}

void ParseProtocolEventCheckVehInfoResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("ȷ�ϳ�����ϢӦ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 603;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ�ȷ�ϳ�����ϢӦ������:" + Resp);
	return;
}

void ParseProtocolEventCheckEntryInfoResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("ȷ���������Ӧ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 604;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ�ȷ���������Ӧ������:" + Resp);
	return;
}

void ParseProtocolEventShowFeeInfoResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("��ʾ�Ѷ�����Ӧ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 605;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ���ʾ�Ѷ�����Ӧ������:" + Resp);
	return;
}

void ParseProtocolEventPayResultShowResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("չʾ֧��״̬����Ӧ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 606;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ�չʾ֧��״̬����Ӧ������:" + Resp);
	return;
}

void ParseProtocolEventDealStopResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("���鴦�����Ӧ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 607;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ����鴦�����Ӧ������:" + Resp);
	return;
}

void ParseProtocolEventCardOperationNotifyResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("ETC����������Ӧ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 611;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ�ETC��������ʾ����Ӧ������:" + Resp);
	return;
}

void ParseProtocolEventModifyVehQueueResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("���������޸�����Ӧ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 612;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ����������޸�����Ӧ������:" + Resp);
	return;
}


void ParseProtocolStartScanResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 1024)
    {
        LogCallBack("���ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 608;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ�ɨ�赽��ά��Ӧ������:" + Resp);
	return;
}

void ParseProtocolAuditResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 1024)
    {
        LogCallBack("���ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 610;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ������˵�Ӧ������:" + Resp);
	return;
}


void ParseProtocolStopScanResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
    unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 1024)
    {
        LogCallBack("���ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 699;  //ֹͣɨ����Ӧ��
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ�ֹͣɨ��Ӧ������:" + Resp);
	return;
}


void ParseProtocolEventFeeAuthorize(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
	unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 2*1024)
    {
        LogCallBack("��С�Ѷ���ȨӦ�����ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 613;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ���С�Ѷ���ȨӦ������:" + Resp);
	return;
}

void ParseProtocolEventAuthorize(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
	unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
	if (len > 2*1024)
	{
		LogCallBack("������ȨӦ�����ݹ�����������");
		return ;
	}
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
	std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 616;

	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ�������ȨӦ������:" + Resp);
	return;
}

void ParseProtocolDeviceNo(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
	unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
	if (len > 2*1024)
	{
		LogCallBack("�豸���к�Ӧ�����ݹ�����������");
		return ;
	}
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 2*1024);
	std::copy(qr_data.begin() + TWPD_DATA_OFFSET +4, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 630;

	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�豸���к�Ӧ������:" + Resp);
	return;
}

void ParseProtocolEventButtonClicked(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
	unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 1024)
    {
        LogCallBack("���ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 614;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ��豸��������Ӧ������:" + Resp);
	return;
}

void ParseProtocolEventDelVehQueueCmd(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
	unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 1024)
    {
        LogCallBack("���ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 617;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ��豸ɾ��������������Ӧ������:" + Resp);
	return;
}

void ParseProtocolEventDelVehQueueResultResp(const std::vector<unsigned char>&qr_data, UniQrDriver& uqd)
{
	unsigned short len = MAKEWORDBIG(qr_data[1], qr_data[2]);
    if (len > 1024)
    {
        LogCallBack("���ݹ�����������");
        return ;
    }
	uqd.isEtcEventResp = true;
	memset(uqd.EtcEventRespContent, 0, 1024);
    std::copy(qr_data.begin() + TWPD_DATA_OFFSET, qr_data.begin() + TWPD_DATA_OFFSET + len, uqd.EtcEventRespContent);
	uqd.EtcEventId = 618;
	
	std::string Resp = uqd.EtcEventRespContent;
	LogCallBack("�յ��豸ɾ���������н��Ӧ������:" + Resp);
	return;
}


/*************************************************************************************/

std::string Bin2Hex(const std::vector<unsigned char>& src, bool with_space)
{
    CString strHex;
    CString strTemp;
    if (!src.empty())
    {
        for ( DWORD i = 0; i < src.size(); ++i )
        {
            if (with_space)
            {strTemp.Format(_T("%02X "), src[i] );}
            else
            {strTemp.Format(_T("%02X"), src[i] );}



            strHex += strTemp;
        }
    }
    return strHex.GetBuffer(0);
}

unsigned short crc16(const std::vector<unsigned char>::const_iterator& it_first, const std::vector<unsigned char>::const_iterator& it_second)
{
    if (it_first == it_second)
    {
        return 0;
    }
    BYTE    CRC16Hi = 0xFF;
    BYTE    CRC16Lo = 0xFF;
    BYTE    CH = 0xA0;
    BYTE    CL = 0x01;
    BYTE    SaveHi = 0;
    BYTE    SaveLo = 0;

    for (std::vector<unsigned char>::const_iterator it = it_first; it != it_second; ++it)
    {
        CRC16Lo = CRC16Lo ^ (*it);

        for (int Flag = 0; Flag < 8; Flag++)
        {
            SaveHi = CRC16Hi;
            SaveLo = CRC16Lo;
            CRC16Hi = CRC16Hi / 2; //��λ����һλ
            CRC16Lo = CRC16Lo / 2; //��λ����һλ
            if ((SaveHi & 1) == 1)
                CRC16Lo |= 0x80;
            if ((SaveLo & 1) == 1)
            {
                CRC16Hi = CRC16Hi ^ CH;
                CRC16Lo = CRC16Lo ^ CL;
            }
        }//for(int Flag = 0;Flag < 8;Flag++)
    }

    return (WORD)(CRC16Hi * 0x100 + CRC16Lo);
}


bool TestBit(int n, int pos)
{
    if (pos < 0 || pos > (sizeof(n) * 8 - 1) )
    {
        return false;
    }

    return (n & (1 << pos )) > 0;
}


bool EscapeComProtocol(const std::vector<unsigned char>& src, std::vector<unsigned char>&dst )
{
    dst.clear();
    dst.push_back(0xDF);

    for (size_t i = 0; i < src.size(); ++i)
    {
        if (src[i] == TWPD_STARTFLAG)
        {
            dst.push_back(TWPD_STARTFLAG_ESCAPE_FIRST);
            dst.push_back(TWPD_STARTFLAG_ESCAPE_SECOND);
        }
        else if (src[i] == TWPD_STARTFLAG_ESCAPE_FIRST)
        {
            dst.push_back(TWPD_STARTFLAG_ESCAPE_FIRST);
            dst.push_back(0x00);
        }
        else if (src[i] == TWPD_ENDFLAG)
        {
            dst.push_back(TWPD_ENDFLAG_ESCAPE_FIRST);
            dst.push_back(TWPD_ENDFLAG_ESCAPE_SECOND);
        }
        else if (src[i] == TWPD_ENDFLAG_ESCAPE_FIRST)
        {

            dst.push_back(TWPD_ENDFLAG_ESCAPE_FIRST);
            dst.push_back(0x00);
        }
        else
        {
            dst.push_back(src[i]);
        }
    }

    dst.push_back(0xFD);

    return true;
}
bool UnEscapeComProtocol(const std::vector<unsigned char>& src, std::vector<unsigned char>&dst )
{
    dst.clear();

    if (src.empty())
    {
        return true;
    }
    size_t i = 0;
    while (i < (src.size() - 1) ) /**/
    {
        if (src[i] == TWPD_STARTFLAG_ESCAPE_FIRST )
        {
            if (src[i + 1] == TWPD_STARTFLAG_ESCAPE_SECOND)
            {
                dst.push_back(TWPD_STARTFLAG);
            }
            else if (src[i + 1] == 0x00)
            {
                dst.push_back(TWPD_STARTFLAG_ESCAPE_FIRST);
            }
            else
            {
                return false;
            }
            i += 2;
        }
        else if (src[i] == TWPD_ENDFLAG_ESCAPE_FIRST)
        {
            if (src[i + 1] == TWPD_ENDFLAG_ESCAPE_SECOND)
            {
                dst.push_back(TWPD_ENDFLAG);
            }
            else if (src[i + 1] == 0x00)
            {
                dst.push_back(TWPD_ENDFLAG_ESCAPE_FIRST);
            }
            else
            {
                return false;
            }
            i += 2;
        }
        else
        {
            dst.push_back(src[i]);
            i++;
        }
    }
    /*last char unescape*/
    if (src.back() == TWPD_STARTFLAG_ESCAPE_FIRST
            || src.back() == TWPD_ENDFLAG_ESCAPE_FIRST)
    {
        return false;
    }
    else
    {
        dst.push_back(src.back());
    }


    return true;
}


std::string NextGuid()
{
    std::string result;
    char buf[100] = { 0 };
    GUID guid;

    if (S_OK == ::CoCreateGuid(&guid))
    {
        sprintf(buf, "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
                guid.Data1, guid.Data2, guid.Data3,
                guid.Data4[0], guid.Data4[1],
                guid.Data4[2], guid.Data4[3],
                guid.Data4[4], guid.Data4[5],
                guid.Data4[6], guid.Data4[7]);
    }

    return std::string(buf);
}


std::string MakeupPackage(unsigned short id, const std::vector<unsigned char>& data, unsigned char top, unsigned char sub)
{
    std::vector<unsigned char> package;
    unsigned short len = static_cast<unsigned short>(data.size());

    package.push_back(HIBYTE(len));
    package.push_back(LOBYTE(len));
    package.push_back(HIBYTE(id));
    package.push_back(LOBYTE(id));
    package.push_back(top);
    package.push_back(sub);

    std::copy(data.begin(), data.end(), std::back_inserter(package));

    unsigned short crc_code = crc16(package.begin(), package.end());
    package.push_back(HIBYTE(crc_code));
    package.push_back(LOBYTE(crc_code));

    std::vector<unsigned char> escapePackage;
    EscapeComProtocol(package, escapePackage);

    return Bin2Hex(escapePackage, false);
}

bool CheckPackage(const std::vector<unsigned char>& data)
{
    size_t package_len = data.size();
    if (package_len < 10)
    {
        LogCallBack("Э��У��ʧ��:���Ȳ��Ϸ�");
        return false;
    }
    if (data[0] != 0xDF || data[package_len - 1] != 0xFD)
    {
        LogCallBack("Э��У��ʧ��:Э��ͷβ����");
        return false;
    }
    size_t data_len = MAKEWORDBIG(data[1], data[2]);
    if (data_len != (package_len - 10) )
    {
        LogCallBack("Э��У��ʧ��:������ȱʧ");
        return false;
    }

    unsigned short package_crc = MAKEWORD(data[package_len - 2], data[package_len - 3]);
    unsigned short calc_crc = crc16(data.begin() + 1, data.end() - 3);
    std::vector<unsigned char> tmp;
    tmp.insert(tmp.begin(), data.begin() + 1, data.end() - 2);

    if (package_crc != calc_crc)
    {
        LogCallBack("Э��У��ʧ��:CRC����");
        return false;
    }

    return true;

}


void GenerateC0(const std::vector<unsigned char>& real_data, std::vector<std::string>& c0_queue)
{
    if (real_data.size() < 9)
    {
        return ;
    }
    if (real_data[5] == 0xC0)
    {
        return ;
    }
    unsigned short len = 4;
    std::vector<unsigned char> c0_data;
    c0_data.push_back(real_data[5]);
    c0_data.push_back(real_data[6]);
    c0_data.push_back(real_data[3]);
    c0_data.push_back(real_data[4]);

    c0_queue.push_back(MakeupPackage(0, c0_data, 0xC0, 0x00));
}

void str_replace(std::string & str, const std::string & strsrc, const std::string &strdst)
{
    std::string::size_type pos = 0;//λ��
    std::string::size_type srclen = strsrc.size();//Ҫ�滻���ַ�����С
    std::string::size_type dstlen = strdst.size();//Ŀ���ַ�����С
    while ((pos = str.find(strsrc, pos)) != std::string::npos)
    {
        str.replace(pos, srclen, strdst);
        pos += dstlen;
    }
}


/*
"ScanData":
{
"QRcode":"123412348903568"
}
*/
CString ParseQRcode(char *szParamContext)
{
	Json::Reader reader;
	Json::Value  root;
	Json::Value  ScanData;
	
	reader.parse(szParamContext, root);
	ScanData = root["ScanData"];
	
	CString QRcode = ScanData["QRcode"].asCString();

	return QRcode;
}



/*
{
    "AreaID": "4112",
    "CurrentTime": "2021-10-09 10:50:48",
    "DevicePayMode": 3,
    "LaneID": "1",
    "LaneNo": "1",
    "ProvinceID": 41,
    "ProvinceName": "����",
    "ProvinceNo": "41",
    "ReturnPayChannelCodeUnify": 1,
    "RoadID": "52",
    "RoadName": "nameRoad",
    "RoadNo": "52",
    "ServerIP": "127.0.0.1",
    "ServerPort": 8080,
    "ServerType": 0,
    "StationID": "6001",
    "StationName": "��կվ",
    "StationNo": "6001",
    "UpdatePort": 3457
}
*/
std::string modifyA5CurrentTime(std::string A5Json)
{
    Json::Reader reader;
    Json::Value  root;

	std::string curTime =  (LPCTSTR)CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
    reader.parse(A5Json, root);
    root["CurrentTime"] = curTime;

    Json::FastWriter fast_writer;
    std::string str = fast_writer.write(root).c_str();

    return str;
}
bool Hex2Bin(const char* hex_buf,char* bin_buf,size_t hex_len)
{
	if(hex_len%2)
		return false;

	CString strTemp(hex_buf); 
	strTemp.MakeUpper();

	//USES_CONVERSION;
	for(int i=0;i<strTemp.GetLength()/2;i++)
	{
		BYTE a = (BYTE)(strTemp.GetAt(i*2)>='A')?(BYTE)(strTemp.GetAt(i*2)-'A'+10):(BYTE)(strTemp.GetAt(i*2)-'0');
		BYTE b = (BYTE)(strTemp.GetAt(i*2+1)>='A')?(BYTE)(strTemp.GetAt(i*2+1)-'A'+10):(BYTE)(strTemp.GetAt(i*2+1)-'0');
		bin_buf[i] = (BYTE)(a * 0x10 + b);
	}
	return true;
}
