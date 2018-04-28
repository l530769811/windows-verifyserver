#include <DBSqlManager.h>
#include "DBSqlExecOperate.h"


CDBSqlExecOperate::CDBSqlExecOperate(const CDBSqlManager *pdbmgr,
	const TCHAR *sql,
	int (*callback)(void *data, int argc, char **argv, char **azColName), 
	void *data)
	: m_pdbmgr(pdbmgr)
{
}

CDBSqlExecOperate::CDBSqlExecOperate(void)
	: m_pdbmgr(0)
{
	
}


CDBSqlExecOperate::~CDBSqlExecOperate(void)
{
}


bool CDBSqlExecOperate::Copy(const COperater &p){
	bool bret = false;
	
	return bret;
}

bool CDBSqlExecOperate::_DoOperate(){
	bool bret = false;


	return bret;
}