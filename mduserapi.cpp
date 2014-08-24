#include "mduserapi.h"

#include <string.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <sys/stat.h>

using namespace std;

CMdUserApi::CMdUserApi(void)
{
	m_pApi = NULL;
	m_nRequestID = 0;
}

CMdUserApi::~CMdUserApi(void)
{
	Disconnect();
}

bool CMdUserApi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if(bRet)
	{
		//if(m_msgQueue)
		//	m_msgQueue->Input_OnRspError(this,pRspInfo,nRequestID,bIsLast);
	}
	return bRet;
}

bool CMdUserApi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));

	return bRet;
}

void CMdUserApi::Connect(const string& szPath,
		const string& szAddresses,
		const string& szBrokerId,
		const string& szInvestorId,
		const string& szPassword)
{
	m_szBrokerId = szBrokerId;
	m_szInvestorId = szInvestorId;
	m_szPassword = szPassword;
	
	char *pszPath = new char[szPath.length()+20];
	sprintf(pszPath,"%sMd",szPath.c_str());
	mkdir(pszPath, 0777);
	//printf("%s\n", pszPath);
	
	m_pApi = CThostFtdcMdApi::CreateFtdcMdApi(pszPath,(szAddresses.find("udp://") != szAddresses.npos));
	delete[] pszPath;

	if (m_pApi)
	{
		m_pApi->RegisterSpi(this);
		
		//��ӵ�ַ
		size_t len = szAddresses.length()+1;
		char* buf = new char[len];
		strncpy(buf,szAddresses.c_str(),len);

		char* token = strtok(buf, _QUANTBOXC2CTP_SEPS_);
		while(token)
		{
			if (strlen(token)>0)
			{
				char * pch = strstr(token,"udp://");
				if(pch)
				{
					strncpy (pch,"tcp://",6);
				}
				m_pApi->RegisterFront(token);
			}
			token = strtok( NULL, _QUANTBOXC2CTP_SEPS_);
		}
		delete[] buf;
		printf("before init\n");
		//��ʼ������
		m_pApi->Init();
	}
	/*
	*/
}

void CMdUserApi::ReqUserLogin()
{
	if (NULL == m_pApi)
		return;

	CThostFtdcReqUserLoginField request = {0};
	
	strncpy(request.BrokerID, m_szBrokerId.c_str(),sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_szInvestorId.c_str(),sizeof(TThostFtdcInvestorIDType));
	strncpy(request.Password, m_szPassword.c_str(),sizeof(TThostFtdcPasswordType));
	
	//ֻ����һ���õ���m_nRequestID��û�б�Ҫÿ������m_nRequestID����0��ʼ
	m_pApi->ReqUserLogin(&request,++m_nRequestID);
}

void CMdUserApi::Disconnect()
{
	if(m_pApi)
	{
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;
	}
}

void CMdUserApi::Subscribe(const string& szInstrumentIDs)
{
	if(NULL == m_pApi)
		return;

	vector<char*> vct;

	size_t len = szInstrumentIDs.length()+1;
	char* buf = new char[len];
	strncpy(buf,szInstrumentIDs.c_str(),len);

	lock_guard<mutex> cl(m_csMapInstrumentIDs);

	char* token = strtok(buf, _QUANTBOXC2CTP_SEPS_);
	while(token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//��Լ�Ѿ����ڣ������ٶ��ģ�����ζ���Ҳû��ϵ
			m_setInstrumentIDs.insert(token);//��¼�˺�Լ���ж���
			vct.push_back(token);
		}
		token = strtok( NULL, _QUANTBOXC2CTP_SEPS_);
	}
	
	if(vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->SubscribeMarketData(pArray,(int)vct.size());

		delete[] pArray;
	}

	//�ͷ��ڴ�
	delete[] buf;
}

void CMdUserApi::Subscribe(const set<string>& instrumentIDs)
{
	if(NULL == m_pApi)
		return;

	string szInstrumentIDs;
	for(set<string>::iterator i=instrumentIDs.begin();i!=instrumentIDs.end();++i)
	{
		szInstrumentIDs.append(*i);
		szInstrumentIDs.append(";");
	}

	if (szInstrumentIDs.length()>1)
	{
		Subscribe(szInstrumentIDs);
	}
}

void CMdUserApi::Unsubscribe(const string& szInstrumentIDs)
{
	if(NULL == m_pApi)
		return;

	vector<char*> vct;
	size_t len = szInstrumentIDs.length()+1;
	char* buf = new char[len];
	strncpy(buf,szInstrumentIDs.c_str(),len);

	lock_guard<mutex> cl(m_csMapInstrumentIDs);

	char* token = strtok(buf, _QUANTBOXC2CTP_SEPS_);
	while(token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//��Լ�Ѿ������ڣ�������ȡ�����ģ������ȡ��Ҳûʲô��ϵ
			m_setInstrumentIDs.erase(token);
			vct.push_back(token);
		}
		token = strtok( NULL, _QUANTBOXC2CTP_SEPS_);
	}
	
	if(vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->UnSubscribeMarketData(pArray,(int)vct.size());

		delete[] pArray;
	}

	//�ͷ��ڴ�
	delete[] buf;
}

void CMdUserApi::SubscribeQuote(const string& szInstrumentIDs)
{
	if (NULL == m_pApi)
		return;

	vector<char*> vct;

	size_t len = szInstrumentIDs.length() + 1;
	char* buf = new char[len];
	strncpy(buf, szInstrumentIDs.c_str(), len);

	lock_guard<mutex> cl(m_csMapQuoteInstrumentIDs);

	char* token = strtok(buf, _QUANTBOXC2CTP_SEPS_);
	while (token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//��Լ�Ѿ����ڣ������ٶ��ģ�����ζ���Ҳû��ϵ
			m_setQuoteInstrumentIDs.insert(token);//��¼�˺�Լ���ж���
			vct.push_back(token);
		}
		token = strtok(NULL, _QUANTBOXC2CTP_SEPS_);
	}

	if (vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->SubscribeForQuoteRsp(pArray, (int)vct.size());

		delete[] pArray;
	}

	//�ͷ��ڴ�
	delete[] buf;
}

void CMdUserApi::SubscribeQuote(const set<string>& instrumentIDs)
{
	if (NULL == m_pApi)
		return;

	string szInstrumentIDs;
	for (set<string>::iterator i = instrumentIDs.begin(); i != instrumentIDs.end(); ++i)
	{
		szInstrumentIDs.append(*i);
		szInstrumentIDs.append(";");
	}

	if (szInstrumentIDs.length()>1)
	{
		SubscribeQuote(szInstrumentIDs);
	}
}

void CMdUserApi::UnsubscribeQuote(const string& szInstrumentIDs)
{
	if (NULL == m_pApi)
		return;

	vector<char*> vct;
	size_t len = szInstrumentIDs.length() + 1;
	char* buf = new char[len];
	strncpy(buf, szInstrumentIDs.c_str(), len);

	lock_guard<mutex> cl(m_csMapQuoteInstrumentIDs);

	char* token = strtok(buf, _QUANTBOXC2CTP_SEPS_);
	while (token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//��Լ�Ѿ������ڣ�������ȡ�����ģ������ȡ��Ҳûʲô��ϵ
			m_setQuoteInstrumentIDs.erase(token);
			vct.push_back(token);
		}
		token = strtok(NULL, _QUANTBOXC2CTP_SEPS_);
	}

	if (vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->UnSubscribeForQuoteRsp(pArray, (int)vct.size());

		delete[] pArray;
	}

	//�ͷ��ڴ�
	delete[] buf;
}

void CMdUserApi::OnFrontConnected()
{
	//���ӳɹ����Զ������¼
	(*m_fnOnFrontConnected)(this);

	ReqUserLogin();
}

void CMdUserApi::OnFrontDisconnected(int nReason)
{
	(*m_fnOnFrontDisconnected)(this, nReason);
	CThostFtdcRspInfoField RspInfo;
	//����ʧ�ܷ��ص���Ϣ��ƴ�Ӷ��ɣ���Ҫ��Ϊ��ͳһ���
	RspInfo.ErrorID = nReason;
	GetOnFrontDisconnectedMsg(&RspInfo);
}

void CMdUserApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo)
		&&pRspUserLogin)
	{
		//�п��ܶ����ˣ������Ƕ������������¶���
		set<string> mapOld = m_setInstrumentIDs;//�����ϴζ��ĵĺ�Լ
		//Unsubscribe(mapOld);//�����Ѿ������ˣ�û�б�Ҫ��ȡ������
		Subscribe(mapOld);//����

		//�п��ܶ����ˣ������Ƕ������������¶���
		mapOld = m_setQuoteInstrumentIDs;//�����ϴζ��ĵĺ�Լ
		SubscribeQuote(mapOld);//����
	}
	else
	{
		//m_status = E_authed;
		//if(m_msgQueue)
		//	m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_logining);
	}
}

void CMdUserApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
//	if(m_msgQueue)
//		m_msgQueue->Input_OnRspError(this,pRspInfo,nRequestID,bIsLast);
}

void CMdUserApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//��ģ��ƽ̨��������������ᴥ��������Ҫ�Լ�ά��һ���Ѿ����ĵĺ�Լ�б�
	if(!IsErrorRspInfo(pRspInfo,nRequestID,bIsLast)
		&&pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapInstrumentIDs);

		m_setInstrumentIDs.insert(pSpecificInstrument->InstrumentID);
	}
}

void CMdUserApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//ģ��ƽ̨��������������ᴥ��
	if(!IsErrorRspInfo(pRspInfo,nRequestID,bIsLast)
		&&pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapInstrumentIDs);

		m_setInstrumentIDs.erase(pSpecificInstrument->InstrumentID);
	}
}

//����ص����ñ�֤�˺������췵��
void CMdUserApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//if(m_msgQueue)
	//	m_msgQueue->Input_OnRtnDepthMarketData(this,pDepthMarketData);
}

void CMdUserApi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (!IsErrorRspInfo(pRspInfo, nRequestID, bIsLast)
		&& pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapQuoteInstrumentIDs);

		m_setQuoteInstrumentIDs.insert(pSpecificInstrument->InstrumentID);
	}
}

void CMdUserApi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo, nRequestID, bIsLast)
		&& pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapQuoteInstrumentIDs);

		m_setQuoteInstrumentIDs.erase(pSpecificInstrument->InstrumentID);
	}
}

void CMdUserApi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	//if (m_msgQueue)
	//	m_msgQueue->Input_OnRtnForQuoteRsp(this, pForQuoteRsp);
}

void CMdUserApi::GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo)
{
	if(NULL == pRspInfo)
		return;

	switch(pRspInfo->ErrorID)
	{
	case 0x1001:
		strcpy(pRspInfo->ErrorMsg,"0x1001 �����ʧ��");
		break;
	case 0x1002:
		strcpy(pRspInfo->ErrorMsg,"0x1002 ����дʧ��");
		break;
	case 0x2001:
		strcpy(pRspInfo->ErrorMsg,"0x2001 ����������ʱ");
		break;
	case 0x2002:
		strcpy(pRspInfo->ErrorMsg,"0x2002 ��������ʧ��");
		break;
	case 0x2003:
		strcpy(pRspInfo->ErrorMsg,"0x2003 �յ�������");
		break;
	default:
		sprintf(pRspInfo->ErrorMsg,"%x δ֪����", pRspInfo->ErrorID);
		break;
	}
}
