#include "ClientSignupNetSocketDataParse.h"
#include "DBSqlExecOperate.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "GlobalFunc.h"
#include "sqlite_sql.h"


const MyString ClientSignupNetSocketDataParse::m_strRequest = _T("user_regist_request");

ClientSignupNetSocketDataParse::ClientSignupNetSocketDataParse(void)
{
}


ClientSignupNetSocketDataParse::~ClientSignupNetSocketDataParse(void)
{
}

bool ClientSignupNetSocketDataParse::_isType(const unsigned char* data, long len){
	bool bret = false;
	TCHAR unicode[1024] = {0};
	GlobalUtf8ToUnicode((const char*)data, unicode, 1023);
	cJSON *proot = cJSON_Parse(unicode);
	if(proot!=0){
		cJSON *pcontent = cJSON_GetObjectItem(proot, JSON_COMMON_CONTENT_KEY);
		if (pcontent!=0)
		{
			cJSON *ptype = cJSON_GetObjectItem(pcontent, JSON_COMMON_CTYPE_KEY);
			if(ptype!=0 && ptype->type == cJSON_String){
				m_strType = ptype->valuestring;
				bret = m_strType.compare(m_strRequest)==0;
			}
		}
	}
	return bret;
}

bool ClientSignupNetSocketDataParse::_parseData(const unsigned char* data, long len){
	bool bret = false;

	TCHAR unicode[1024] = {0};
	GlobalUtf8ToUnicode((const char*)data, unicode, 1023);
	cJSON *proot = cJSON_Parse(unicode);
	if(proot!=0){
		cJSON *pcontent = cJSON_GetObjectItem(proot, JSON_COMMON_CONTENT_KEY);
		if (pcontent!=0)
		{
			cJSON *pvalue = cJSON_GetObjectItem(pcontent, JSON_COMMON_CVALUE_KEY);
			if(pvalue!=0 && pvalue->type == cJSON_Object){
				
				TCHAR sql[MAX_PATH] = {0};
				_stprintf(sql, MAX_PATH-1, insert_clientuser_data, _T(""), _T(""), _T(""), _T(""));
				this->m_strSignupSql = sql;
			}
		}
	}

	return bret;
}

void ClientSignupNetSocketDataParse::RefreshOperator(COperater *operate){
	if(operate!=0){
		//CDBSqlExecOperate o;
		//operate->Copy(o);
	}
}

//
//COperater ClientSignupNetSocketDataParse::CreateOperater()
//{
//	CClientSignupOperate operate;
//
//	return operate;
//	//return CClientSignupOperate();
//}