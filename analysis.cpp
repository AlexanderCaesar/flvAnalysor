//作者  蔡砚刚  20160601  xiaoc@pku.edu.cn
#include <stdlib.h>
#include <string.h>
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

/* 分析flv文件头 */
void Anlysis::anlysisFlvHeader()
{
	fread(m_flvheader->data,1,9,g_flv_file); //读取头信息

	if(m_param->b_tag)
	{
		fprintf(g_flv_tag,"==================FLV 头信息=========================\n");
		fprintf(g_flv_tag,"FLV数据签名:    %c%c%c\n",m_flvheader->signature[0],m_flvheader->signature[1],m_flvheader->signature[2]);
		fprintf(g_flv_tag,"FLV版本信息:    %d\n",m_flvheader->version);
		switch(m_flvheader->present)
		{
		case 1:fprintf(g_flv_tag,"FLV数据信息:    只拥有视频流\n");break;
		case 4:fprintf(g_flv_tag,"FLV数据信息:    只拥有音频流\n");break;
		case 5:fprintf(g_flv_tag,"FLV数据信息:    拥有视频和音频流\n");break;
		default:fprintf(g_flv_tag,"FLV数据信息:   flv头信息错误\n");
		}
	}
		
	if(m_param->b_nflv)
	{
		memcpy(m_mend_flvheader->data,m_flvheader->data,9);//拷贝元数据头信息
		{
			//flv 头信息 修改 待添加
		}
		fwrite(m_mend_flvheader->data,1,9,g_flv_out); //写新的头信息
	}
}

/* 分析flv文件 */
void Anlysis::anlysis()
{
	anlysisFlvHeader();//分析flv头
}