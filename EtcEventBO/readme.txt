����:2019-03-04
-----------------------------------------
��������:
1���޸�PayUnit.cpp��197�������Ҫ������ʹ�ÿۿ��������֧����ʶ

����:2019-02-22
-----------------------------------------
�������ݣ�
1��RunParameter������GetDuplicateDebitShortestMinutes���������ڻ�ȡ�ظ�������Ч�����á�
2��ETCPayUnit������NewDebitAbstract��QueryDebitAbstract����������֧���ظ�������Ч���жϡ�
3��ETCPayUnit.cpp����299-306�У����յ���������󣬻��жϸó�������Ƿ��׹���
4��ETCPayUnit.cpp����500-507�У����յ����������׻�������������ǳɹ����ף��򻺴����Ϣ��


����:2019-01-29
-----------------------------------------
�������ݣ�
1������PayHelper/PayUnit/PayManage/PayHttpSync������1.5֧���߼�����
2��ThreadManageģ������StartNewPay ,QueryCurrentPayState ,CancelCurrentPay ,CancelTargetPay ,GetCurrentDebitResult ,InitAgencyPay ,QueryVirtualCardDebit ,AgencyDebitQuery ,TransferImage ,SetAgencyPara����������֧��1.5֧���߼�����
3��TWSDNetPay_Dllģ������IF_DebitMoney_Future,IF_DebitCancel_Future,IF_GetDebitResult_Future֧��1.5֧���߼�������
4��DataBaseHelper.h/DataBaseHelper.cpp������DataBaseAdd ,DataBaseQuery ,DataBaseDelete֧��1.5�µĳ������������
5���޸�ThreadDevice.cpp��764-772�У������NFC����չϸ��չʾ��
6���޸�ETCPayUnit�е�HasUnrecoverableError,OccurSystemFatalError��ȥ���������ƣ��Դ�����̷߳��ʳ�ʱ��ȴ���Ҳ����ͨ��������ϵͳ���������޸����˴����ǲ���Ҫ��
7���޸�ETCPayUnit.h������m_DataBaseLock����ʾ���ݿ��������
8���޸�ETCPayUnit.cpp����SaveDebitRecord��MoveDebitRequestFromIOToDataBase�Լ�309�С�366�е������ƣ�ʹ�õ��������ݿ����滻ԭ�е�֧������
9���޸�SystemDiaryģ�飬����CFunctionCallTrace�����ڸ��ݹ��̵��á�
10��ɾ��ETCPayHelper��GetTraceMsgHeader��CInterfaceTraceInfo�Լ���־������غ궨�塣ͳһ��SystemDiaryģ��ʵ�ִ˹��ܡ�
11���޸�ETCPayUnit.cpp�е�SendOneDebit���������ϵͳ�������ٷ������á�������������������ƶ˽���ʧ�ܣ��������5���ط��������������ط�
12���޸�HttpConnection���紦��ģ�飬����SetHttpOption�������������÷��Ͳ�����
13��DataBaseHelper.cpp�У����ݿ�������������Ӵ����������ݿ������ʱ��

-----------------------------------------

����:2019-01-22
�������ݣ�
1���޸�ETCPayUnit.cpp�е�286-298�У��������ϵͳ���ϣ����ٴ����κ�����
2���޸�ETCPayUnit.cpp�еĵ�332��339�У��������������󣬽����´�����ǰ����5��
-----------------------------------------

����:2019-01-22
�������ݣ�
1���޸��ļ�TWSDNetPay_Dll.cpp��4917-4920�У�����������⿨�����е�֧����ʶ�Ͷ����Ž�����
2���޸�RunParameter.cpp�е�274�С�294�У�����ETC����IP�Ͷ˿����á�
-----------------------------------------

����:2019-01-17
�������ݣ�
1���޸��ļ�ETCPayUnit.h/ETCPayUnit.cpp�����ӷ���MoveDebitRequestFromIOToDataBase�����ڽ�IO�еĿۿ�����ת�Ƶ����ݿ⡣
2���޸��ļ�ETCPayUnit.h/ETCPayUnit.cpp�����ӷ���SaveDebitRequestToIO�����ڽ��ۿ���������ȱ��浽IO�С�
3���޸��ļ�TWSDNetPay_Dll.cpp�е�5539��5549-5551�У�������־�����������Ա�鿴�����
-----------------------------------------

����:2019-01-15
��������:
1���޸��ļ�ETCPayHelper.h/ETCPayHelper.cpp ����TW_DEBUG_VARIABLE������֧�ֵ��Ա���
2���޸��ļ�ETCPayHelper.cpp��ParseOBUWhiteListRecordFromServer��ʵ�֣�����QueryIdentifier
�Ľ����жϡ�
3���޸��ļ�ETCPayHelper.cpp��587-596��
4���޸��ļ�ETCPayUnit.cpp�е�491-496�У��޸����ʱ���ж����⡣
5���޸��ļ�TWSDNetPay_Dll.cpp,����QueryAgentDebitEtcUp_Stake ,QueryAgentDebitResultEtcUp_Stake, DebitMoneyEtcUp_Stake,�ṩ������ģ����ԡ�
6���޸��ļ�TypeHelper.cpp�е�GetFileModifyTime���޸���CreatFile�ĵ��ò�����ʹ��ֻ�򿪴��ڵ��ļ���


-----------------------------------------
����:2019-01-11
�������ݣ�
1�������ļ�ETCAuthenticateUnit.cpp\ETCAuthenticateUnit.h�����ڴ������ETC��������Ȩ
2�������ļ�ETCPayHelper.cpp\ETCPayHelper.h�����ڸ���ס��ETC����ת��
3�������ļ�ETCPayManage.cpp\ETCPayManage.h���ṩ����ETC������ҵ����
4�������ļ�ETCPayUnit.cpp\ETCPayUnit.h������ETC֧�����ܺ�֧�������ѯ����
5�������ļ�RunParameter.cpp\RunParameter.h��ϵͳ���в�������
6�������ļ�DataBaseHelper.cpp\DataBaseHelper.h�����ݿ⸨�������ܣ���ɾ�Ĳ顣
-----------------------------------------
����:2018-10-26
�������ݣ�
1����������⿨��������������֧��
2�������˶�֧��ģʽ��֧�֡�
-----------------------------------------
����:2018-09-30
�������ݣ�
1�������˶�������⿨��֧�֣��ۿ���������ETCִ�����ֶΡ�