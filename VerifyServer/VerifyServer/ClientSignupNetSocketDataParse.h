#ifndef _CLIENTSIGNUPNETSOCKETDATAPAESE_H__
#define _CLIENTSIGNUPNETSOCKETDATAPAESE_H__

#include "netsocketdataparse.h"
#include "my_assist_define.h"
#include <tchar.h>

class ClientSignupNetSocketDataParse :
	public CNetSocketDataParse
{
public:
	ClientSignupNetSocketDataParse(void);
	virtual ~ClientSignupNetSocketDataParse(void);

	virtual void RefreshOperator(COperater *operate);

protected:
	virtual bool _isType(const unsigned char* data, long len);
	virtual bool _parseData(const unsigned char* data, long len);
	//virtual COperater CreateOperater();

private:
	MyString m_strType;
	MyString m_strSignupSql;
	static const MyString m_strRequest ;

};

#endif //_CLIENTSIGNUPNETSOCKETDATAPAESE_H__