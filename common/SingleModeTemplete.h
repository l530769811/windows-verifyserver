#ifndef _SINGOLEMODETEMPLETE_H__
#define _SINGOLEMODETEMPLETE_H__
#include "common_define.h"

///////////////////////////////
//����ժ¼�ĵ���ģʽ-ģ����;
////////////////////////////////////////

template <typename T> 
class _COMMON_IMPORT_EXPORT_DLL_ CSingleModeTemplete
{
public:
	static T * GetInstance()
	{
		static T m_single;
		return &m_single;
	}
};
#endif //_SINGOLEMODETEMPLETE_H__