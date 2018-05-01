#include "stdafx.h"
#include "DBSqlManager.h"
#include "DBProxy.h"
#include <process.h>


CDBSqlManager::CDBSqlManager(void)
	: m_pDB(NULL)
	, m_stackThread(INVALID_HANDLE_VALUE)
	, m_bthreadRunning(false)
	, m_hEvent(INVALID_HANDLE_VALUE)
{
	::InitializeCriticalSection(&m_criQueueLock);
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

CDBSqlManager::~CDBSqlManager(void)
{
	::DeleteCriticalSection(&m_criQueueLock);
	if (m_hEvent!=NULL && m_hEvent!=INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hEvent);
	}
}

void CDBSqlManager::AttachDB(CDBProxy *pDB)
{
	m_pDB = pDB;
}

void CDBSqlManager::DetachDB()
{
	m_pDB = NULL;
}

unsigned int __stdcall CDBSqlManager::_SqlexecThreadProc(void * pParam)
{
	int ret = -1;
	CDBSqlManager *pManager = reinterpret_cast<CDBSqlManager*>(pParam);
	while (pManager->m_bthreadRunning == true)
	{
		if (pManager->m_sql_exec_queue.empty()==true)
		{
			::WaitForSingleObject(pManager->m_hEvent, INFINITE);
		}

		CSqlExecNode *p = NULL;
		::EnterCriticalSection(&pManager->m_criQueueLock);
		p = pManager->m_sql_exec_queue.back();
		pManager->m_sql_exec_queue.pop();
		::LeaveCriticalSection(&pManager->m_criQueueLock);

		pManager->ExecSql(p->m_sql.c_str(), p->callback, p->m_data);
		::Sleep(100);
		ret = 1;
	}

	return ret;
}

int CDBSqlManager::OpenStack()
{
	int ret = -1;

	if (m_stackThread==NULL || m_stackThread==INVALID_HANDLE_VALUE)
	{
		m_bthreadRunning = true;
		m_stackThread = (HANDLE)_beginthreadex(NULL, 0, _SqlexecThreadProc, reinterpret_cast<void *>(this), 0, 0);
		if (m_stackThread!=NULL && m_stackThread!=INVALID_HANDLE_VALUE)
		{
			::SetThreadPriority(m_stackThread, THREAD_PRIORITY_LOWEST);
			m_bthreadRunning = true;
		}
		else
		{
			m_bthreadRunning = false;
			::CloseHandle(m_stackThread);
		}
	}

	return ret;
}

int CDBSqlManager::CloseStack()
{
	int ret = -1;

	if (m_stackThread!=NULL && m_stackThread!=INVALID_HANDLE_VALUE && m_bthreadRunning==true)
	{
		::SetEvent(m_hEvent);
		m_bthreadRunning = false;
		::WaitForSingleObject(m_stackThread, 5000);		
	}


	::CloseHandle(m_stackThread);
	m_stackThread = INVALID_HANDLE_VALUE;
	while(m_sql_exec_queue.empty()==false)
	{
		CSqlExecNode*p = m_sql_exec_queue.back();
		m_sql_exec_queue.pop();
		if (p != NULL)
		{
			delete p;
		}
	}

	return ret;
}

int CDBSqlManager::Lock()
{
	int ret = -1;

	return ret;
}

int CDBSqlManager::Unlock()
{
	int ret = -1;

	return ret;
}

int CDBSqlManager::ExecSqlInStack(const TCHAR *sql, 
	int (*callback)(void *data, int argc, char **argv, char **azColName), 
	void *data)
{
	int ret = -1;

	if(m_bthreadRunning == true){
		::EnterCriticalSection(&m_criQueueLock);
		CSqlExecNode *pnode = new CSqlExecNode(sql, callback, data);
		m_sql_exec_queue.push(pnode);
		::LeaveCriticalSection(&m_criQueueLock);
		::SetEvent(m_hEvent);
	}
	
	return ret;
}

int CDBSqlManager::ExecSqlInStack(const TCHAR *sql)
{
	int ret = -1;
	if(m_bthreadRunning == true){

		::EnterCriticalSection(&m_criQueueLock);
		CSqlExecNode *pnode = new CSqlExecNode(sql);
		m_sql_exec_queue.push(pnode);
		::LeaveCriticalSection(&m_criQueueLock);
		::SetEvent(m_hEvent);
	}
	return ret;
}

int CDBSqlManager::ExecSql(const TCHAR *sql,
	int (*callback)(void *data, int argc, char **argv, char **azColName), 
	void *data)
{
	int bRet = -1;
	if (m_pDB!=NULL)
	{
		
			bRet = m_pDB->ExecSql(sql, callback, data);
	}

	return bRet;
}

int CDBSqlManager::ExecSql(const TCHAR *sql)
{
	int ret = -1;

	if (m_pDB!=NULL)
	{

		ret = m_pDB->ExecSql(sql, NULL, NULL);
	}

	return ret;
}