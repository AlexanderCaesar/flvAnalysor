//作者  蔡砚刚  20160601  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "analysis.h"
Anlysis::Anlysis()
{
	m_param          = NULL;
	m_flvheader      = NULL;
	m_mend_flvheader = NULL;
	
};
/* 申请内存 */
void Anlysis::create(flv_param *param)
{
	bool ok = true;
	m_param = param;
	m_flvheader = (FlvHeader*)malloc(sizeof(FlvHeader)); //申请flv头内存
	ok = ok&&m_flvheader;
	if(m_param->b_nflv)//如果需要输出新的flv文件
	{
		m_mend_flvheader = (FlvHeader*)malloc(sizeof(FlvHeader)); //申请新flv头内存
		ok = ok&&m_mend_flvheader;
	}

	if(!ok)
	{
		printf("Anlysis类申请内存失败\n");
		system("pause");
		exit(0);
	}
}
/* 释放内存 */
void Anlysis::destory()
{
	if(m_flvheader)
	{
		free(m_flvheader);
	}
	if(m_mend_flvheader)
	{
		free(m_mend_flvheader);
	}
}

/* 分析flv文件 */
void Anlysis::anlysis()
{
}