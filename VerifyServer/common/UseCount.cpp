#include "UseCount.h"

CUseCount::CUseCount(void)
	: m_pCount(new atomic_t(1))
{
	ATOMIC_INIT(1);
}
CUseCount::CUseCount(const CUseCount &u)
	:m_pCount(u.m_pCount)
{
	//CLock lock(&m_lock);
	atomic_inc(m_pCount);//(*m_pCount)++;//(*u.m_pCount) also ++;
	
}
CUseCount& CUseCount::operator=(const CUseCount &u)
{
	//CLock lock(&m_lock);
	//为什么不先判断是否复制同一个CUseCount?;
	
	atomic_inc((long*)(u.m_pCount));//(*u.m_pCount)++;	
	atomic_dec((long*)(u.m_pCount));//--(*m_pCount);

	if ((*m_pCount) <= 0)
		delete m_pCount;
	
	atomic_set((unsigned  long*)(&m_pCount), (long)u.m_pCount);//m_pCount = u.m_pCount;
	return *this;
}

CUseCount::~CUseCount(void)
{
	//CLock lock(&m_lock);
	atomic_dec((long*)m_pCount);//--(*m_pCount);
	if ((*m_pCount) <= 0)
	{
		delete m_pCount;
	}
}

bool CUseCount::Only()
{
	//CLock lock(&m_lock);
	return (*m_pCount) == 1;
}

bool CUseCount::Reattach(const CUseCount& u)
{
	//CLock lock(&m_lock);
	atomic_inc((long*)u.m_pCount);  //++(*u.m_pCount);
	atomic_dec((long*)m_pCount);//--(*m_pCount);

	if ((*m_pCount) == 0)
	{
		delete m_pCount;
		atomic_set((unsigned  long*)(&m_pCount), (long)u.m_pCount);//m_pCount = u.m_pCount;
		return true;
	}

	atomic_set((unsigned  long*)(&m_pCount), (long)u.m_pCount);//m_pCount = u.m_pCount;
	return false;
}

bool CUseCount::Makeonly()
{
	//CLock lock(&m_lock);
	if (*m_pCount == 1)
		return false;
	
	atomic_dec((long*)(m_pCount));//--(*m_pCount);
	m_pCount = new atomic_t(1);

	return true;
}

