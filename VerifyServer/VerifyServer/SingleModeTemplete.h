#ifndef _SINGOLEMODETEMPLETE_H__
#define _SINGOLEMODETEMPLETE_H__


///////////////////////////////
//����ժ¼�ĵ���ģʽ-ģ����;
////////////////////////////////////////

template <typename T> 
class CSingleModeTemplete
{
public:
	static T * GetInstance()
	{
		static T m_single;
		return &m_single;
	}
};
#endif //_SINGOLEMODETEMPLETE_H__