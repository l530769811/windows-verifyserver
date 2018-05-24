#ifndef _CLIENTSIGNUPNETSOCKETDATAPAESE_H__
#define _CLIENTSIGNUPNETSOCKETDATAPAESE_H__

#include "netsocketdataparse.h"
#include "my_assist_define.h"
#include <tchar.h>

class CClientManager;
class CClientSignupData;
class CClientSignupNetSocketDataParse :
	public CNetSocketDataParse
{
public:
	CClientSignupNetSocketDataParse(CClientManager *mgr);
	virtual ~CClientSignupNetSocketDataParse(void);

	virtual void RefreshOperator(COperater *operate);
	CClientSignupData GetSignupData();
	

protected:
	virtual bool _isType(const unsigned char* data, long len);
	virtual bool _parseData(const unsigned char* data, long len);
	
private:
	MyString m_strType;
	MyString m_strSignupSql;
	MyString m_strUserName;
	MyString m_strUserPassword;
	MyString m_strUserPhone;
	MyString m_strIdentifyCode;

	CClientManager *m_clientMgr;

	static const MyString m_strRequest ;

};

#endif //_CLIENTSIGNUPNETSOCKETDATAPAESE_H__