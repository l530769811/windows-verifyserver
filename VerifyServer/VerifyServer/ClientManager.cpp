#include <my_assist_define.h>
#include "UDPSocket.h"
#include "ClientManager.h"
#include "veriry_common_define.h"


CClientManager::CClientManager(void)
	: m_pUdpVerify(NULL)
{
	m_pUdpVerify = new CUDPSocket(this);
	m_pUdpVerify->Start(UDP_PORT);
	
}


CClientManager::~CClientManager(void)
{
	SAFE_DELETE(m_pUdpVerify);
}

int CClientManager::rev_data(const unsigned char * data, long len){
	int nret = 0;
	
	return nret;
}

void CClientManager::Recevie(DWORD uSokcetID, BYTE *rev_buf, UINT rev_len){

}

void CClientManager::connect_coming(DWORD socketid, unsigned int nport){

}

void CClientManager::unconnect_coming(DWORD socketid, unsigned int nport){

}
