//����  �����  20160601  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "analysis.h"
Anlysis::Anlysis()
{
	m_param          = NULL;
	m_flvheader      = NULL;
	m_mend_flvheader = NULL;
	
};
/* �����ڴ� */
void Anlysis::create(flv_param *param)
{
	bool ok = true;
	m_param = param;
	m_flvheader = (FlvHeader*)malloc(sizeof(FlvHeader)); //����flvͷ�ڴ�
	ok = ok&&m_flvheader;
	if(m_param->b_nflv)//�����Ҫ����µ�flv�ļ�
	{
		m_mend_flvheader = (FlvHeader*)malloc(sizeof(FlvHeader)); //������flvͷ�ڴ�
		ok = ok&&m_mend_flvheader;
	}

	if(!ok)
	{
		printf("Anlysis�������ڴ�ʧ��\n");
		system("pause");
		exit(0);
	}
}
/* �ͷ��ڴ� */
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

/* ����flv�ļ� */
void Anlysis::anlysis()
{
}