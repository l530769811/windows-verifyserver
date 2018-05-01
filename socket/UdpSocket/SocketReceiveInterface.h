#ifndef __SOCKET_RECEIVE_INTERFACE_H__
#define __SOCKET_RECEIVE_INTERFACE_H__
#include "UdpSocket_define.h"

class _UDPSOCKET_IMPORT_EXPORT_DLL_ CSocketReceiveInterface
{
public:
	CSocketReceiveInterface(void);
	virtual ~CSocketReceiveInterface(void);

	virtual int rev_data(const unsigned char * data, long len) = 0;
};

#endif //__SOCKET_RECEIVE_INTERFACE_H__