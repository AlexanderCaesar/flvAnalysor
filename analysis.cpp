//����  �����  20160601  xiaoc@pku.edu.cn
#include <stdlib.h>
#include <string.h>
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

/* ����flv�ļ�ͷ */
void Anlysis::anlysisFlvHeader()
{
	fread(m_flvheader->data,1,9,g_flv_file); //��ȡͷ��Ϣ

	if(m_param->b_tag)
	{
		fprintf(g_flv_tag,"==================FLV ͷ��Ϣ=========================\n");
		fprintf(g_flv_tag,"FLV����ǩ��:    %c%c%c\n",m_flvheader->signature[0],m_flvheader->signature[1],m_flvheader->signature[2]);
		fprintf(g_flv_tag,"FLV�汾��Ϣ:    %d\n",m_flvheader->version);
		switch(m_flvheader->present)
		{
		case 1:fprintf(g_flv_tag,"FLV������Ϣ:    ֻӵ����Ƶ��\n");break;
		case 4:fprintf(g_flv_tag,"FLV������Ϣ:    ֻӵ����Ƶ��\n");break;
		case 5:fprintf(g_flv_tag,"FLV������Ϣ:    ӵ����Ƶ����Ƶ��\n");break;
		default:fprintf(g_flv_tag,"FLV������Ϣ:   flvͷ��Ϣ����\n");
		}
	}
		
	if(m_param->b_nflv)
	{
		memcpy(m_mend_flvheader->data,m_flvheader->data,9);//����Ԫ����ͷ��Ϣ
		{
			//flv ͷ��Ϣ �޸� �����
		}
		fwrite(m_mend_flvheader->data,1,9,g_flv_out); //д�µ�ͷ��Ϣ
	}
}

/* ����flv�ļ� */
void Anlysis::anlysis()
{
	anlysisFlvHeader();//����flvͷ
}