#include <my_assist_define.h>

#include <DBSqlManager.h>
#include "GlobalFunc.h"
#include "UdpNetSocketProxy.h"
#include "UDPSocket.h"
#include "ClientManager.h"
#include "veriry_common_define.h"
#include "SignupMethods.h"
#include "DBSqlExecOperate.h"
#include "NetSocketData.h"
#include "NetSocketProxy.h"
#include "LoginVerifyReponseNetSocketData.h"
#include "UdpNetSocketProxy.h"
#include "ClientVerifyOperator.h"
#include "ClientVerifyNetSocketDataParse.h"
#include "SearchServerNetSocketDataParse.h"
#include "SearchServerNetSocketData.h"
#include "error_id_define.h"
#include "ClientSignupNetSocketDataParse.h"

CClientManager::CClientManager(CDBSqlManager *pdb)
	: m_pUdpVerify(NULL)
	, m_db(pdb)
{
	m_pUdpVerify = new CUDPSocket(this);
	m_pUdpVerify->Start(UDP_REV_PORT);
	
}

bool CClientManager::Open()
{
	bool bret = false;

	return bret;
}

bool CClientManager::Close()
{
	bool bret = false;

	return bret;
}


CClientManager::~CClientManager(void)
{
	SAFE_DELETE(m_pUdpVerify);
}

int CClientManager::rev_data(const unsigned char * data, long len, char *ip_from, unsigned short port_from){
	int nret = 0;
	
	TCHAR str_ip[20] = {0};
	GlobalUtf8ToUnicode(ip_from, str_ip, 19);

	CClientVerifyNetSocketDataParse verify_parse(this);
	if (verify_parse.ParseData(data, len)==true)
	{
		CClientVerifyData verify_data = verify_parse.GetVerifyData();
		int nresult = 0;
		nresult = this->ClientVerify(verify_data) ? 0 : LOGIN_VERIFY_ERROR;
		CLoginVerifyReponseNetSocketData socket_data(nresult);
		CUdpNetSocketProxy proxy(this->m_pUdpVerify, str_ip, UDP_SEND_PORT);
		this->ClientReponse(socket_data, proxy);
	}

	CSearchServerNetSocketDataParse search_server_parse;
	if (search_server_parse.ParseData(data, len) == true)
	{
		CSearchServerNetSocketData socket_data(UDP_REV_PORT, -1);
		
		CUdpNetSocketProxy proxy(this->m_pUdpVerify, str_ip, UDP_SEND_PORT);
		this->ClientReponse(socket_data, proxy);
	}
	
	CClientSignupNetSocketDataParse signup_parse(this);
	if (signup_parse.ParseData(data, len) == true)
	{

	}

	return nret;
}

void CClientManager::Recevie(DWORD uSokcetID, BYTE *rev_buf, UINT rev_len){

}

void CClientManager::connect_coming(DWORD socketid, unsigned int nport){

}

void CClientManager::unconnect_coming(DWORD socketid, unsigned int nport){

}

bool CClientManager::ClientSignup(CUseCount<CClientSignupData> data, CUseCount< CSignupMethods> signupMethods)
{
	bool bret = false;
	if(signupMethods->signupMethodVerify())
	{
		MyString sql = data->ToSql();
		if(m_db!=0)
		{
			m_db->ExecSqlInStack(sql.c_str());
		}
	}
	
	return bret;
}

//int CClientManager::ClientVerifyCallback(void *data, int argc, char **argv, char **azColName)
//{
//	int ret = 0;
//
//	if(data!=0 && typeid(data)==typeid(CClientManager))
//	{
//		CClientManager *pclient = reinterpret_cast <CClientManager*>(data);
//		CLoginVerifyReponseNetSocketData data(0);
//		CUdpNetSocketProxy proxy(pclient->m_pUdpVerify, _T(""), 000);
//		pclient->ClientReponse(data, proxy);
//
//	}
//
//	return ret;
//}

bool CClientManager::ClientVerify(CClientVerifyData &data)
{
	bool ret = false;

	MyString sql = data.ToSql();
	if(m_db!=0)
	{		
		CClientVerifyOperator veryfyReponse(sql);
		veryfyReponse.Exec(m_db);
		ret=veryfyReponse.GetVerifyResult();
	}

	return ret;
}

bool CClientManager::ClientReponse(CNetSocketData &data, CNetSocketProxy &proxy)
{
	bool ret = false;
	ret = data.BeSend(&proxy) > 0;
	return ret;
}
