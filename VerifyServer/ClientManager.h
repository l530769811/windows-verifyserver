#ifndef __CLIENT_MANAGER_H__
#define __CLIENT_MANAGER_H__
#include <SocketReceiveInterface.h>
#include <SocketRecevier.h>

class CUDPSocket;
class CSignupMethods;
class COperater;
class CClientManager : 
	public CSocketReceiveInterface,
	public CSocketRecevier
{
public:
	CClientManager(void);
	virtual ~CClientManager(void);
	virtual int rev_data(const unsigned char * data, long len);
	virtual void Recevie(DWORD uSokcetID, BYTE *rev_buf, UINT rev_len);
	virtual void connect_coming(DWORD socketid, unsigned int nport);
	virtual void unconnect_coming(DWORD socketid, unsigned int nport);

	public bool ClientSignup(COperater &operate, CSignupMethods &signupMethods);

private:
	CUDPSocket * m_pUdpVerify;

};

#endif //__CLIENT_MANAGER_H__