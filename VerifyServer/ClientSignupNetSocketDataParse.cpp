#include "ClientSignupNetSocketDataParse.h"
#include "DBSqlExecOperate.h"
#include "cJSON_Unicode.h"
#include "SocketProtocol.h"
#include "GlobalFunc.h"
#include "sqlite_sql.h"
#include "ClientManager.h"
#include "Operater.h"
#include "IdentifyCodeSignupMethods.h"



const MyString CClientSignupNetSocketDataParse::m_strRequest = _T("user_regist_request");

CClientSignupNetSocketDataParse::CClientSignupNetSocketDataParse(CClientManager *mgr)
	: m_clientMgr(mgr)
{
}


CClientSignupNetSocketDataParse::~CClientSignupNetSocketDataParse(void)
{
}

bool CClientSignupNetSocketDataParse::_isType(const unsigned char* data, long len){
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

bool CClientSignupNetSocketDataParse::_parseData(const unsigned char* data, long len){
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
				MyString strUserName = _T("");
				MyString strUserPassword = _T("");
				MyString strUserPhone = _T("");
				MyString strUserIdentifyCode = _T("");

				cJSON *p = cJSON_GetObjectItem(pvalue, JSON_USER_NAME_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					strUserName = p->valuestring;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_USER_PASSWORD_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					strUserPassword = p->valuestring;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_PHONE_NUMBER_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					strUserPhone = p->valuestring;
				}

				p = cJSON_GetObjectItem(pvalue, JSON_IDENTIFY_CODE_KEY);
				if (p!=0 && p->type==cJSON_String)
				{
					strUserIdentifyCode = p->valuestring;
				}

				TCHAR sql[1024] = {0};
				MyString str_empty;
				_stprintf(sql, 1024-1, insert_clientuser_data, strUserName.c_str(), strUserPassword.c_str(),strUserPhone.c_str(), str_empty.c_str(), str_empty.c_str());
				this->m_strSignupSql = sql;
				
				if (m_clientMgr!=0)
				{
					CUseCount<CClientSignupData> data(new CClientSignupData(strUserName, strUserPassword, strUserPhone));
					CUseCount<CSignupMethods> methods(new CIdentifyCodeSignupMethods());
					m_clientMgr->ClientSignup(data, methods);
				}
			}
		}
	}

	return bret;
}

void CClientSignupNetSocketDataParse::RefreshOperator(COperater *operate){
	
}