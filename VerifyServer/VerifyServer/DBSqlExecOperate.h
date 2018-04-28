#ifndef __CLIENTSIFNUPOPERATE_H__
#define __CLIENTSIFNUPOPERATE_H__

#include "Operater.h"
#include <tchar.h>

class COperater;
class CDBSqlManager;
class CDBSqlExecOperate : public COperater
{
public:
	CDBSqlExecOperate(const CDBSqlManager *pdbmgr,
		const TCHAR *sql,
		int (*callback)(void *data, int argc, char **argv, char **azColName), 
		void *data);
	CDBSqlExecOperate(void);
	~CDBSqlExecOperate(void);
	virtual bool Copy(const COperater &p);

private:
	//virtual bool _OperatePrepare(void);
	virtual bool _DoOperate(void);
	//virtual bool _OperateDone(void);

private:
	const CDBSqlManager *m_pdbmgr;
};

#endif