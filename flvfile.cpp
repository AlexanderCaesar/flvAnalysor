//����  �����  20160601  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "flvfile.h"

#pragma warning(disable: 4996) // POSIX setmode and fileno deprecated

FILE *g_flv_file  = NULL;    //FLV ԭʼ�ļ�
FILE *g_flv_tag   = NULL;    //��ӡÿ��tag����ϸ��Ϣ
FILE *g_flv_video = NULL;    //�����Ƶ����Ϣ
FILE *g_flv_audio = NULL;    //�����Ƶ����Ϣ
FILE *g_flv_out   = NULL;    //����޸ĺ��flv��

char *flv_file_name  = NULL; //FLV ԭʼ�ļ�           -i ���� �磺-i brazil-bq.flv
char *flv_tag_name   = NULL; //��ӡÿ��tag����ϸ��Ϣ  -t ���� �磺-t brazil-bq.log
char *flv_video_name = NULL; //�����Ƶ����Ϣ         -v ���� �磺-v brazil-bq.264 
char *flv_audio_name = NULL; //�����Ƶ����Ϣ         -a ���� �磺-a brazil-bq.aac 
char *flv_out_name   = NULL; //����޸ĺ��flv��      -o ���� �磺-o brazil-new.flv 

//��flv����ļ�
void openFlvFiles()    
{
	if(flv_file_name)
	{
		g_flv_file = fopen(flv_file_name,"rb");
		if(!g_flv_file)
		{
			printf("���ļ�ʧ��: %s\n",flv_file_name);
			system("pause");
			exit(0);
		}
	}
	if(flv_tag_name)
	{
		g_flv_tag = fopen(flv_tag_name,"w");
		if(!g_flv_tag)
		{
			printf("���ļ�ʧ��: %s\n",flv_tag_name);
			system("pause");
			exit(0);
		}
	}

	if(flv_video_name)
	{
		g_flv_video = fopen(flv_video_name,"wb");
		if(!g_flv_video)
		{
			printf("���ļ�ʧ��: %s\n",flv_video_name);
			system("pause");
			exit(0);
		}
	}

	if(flv_audio_name)
	{
		g_flv_audio = fopen(flv_audio_name,"wb");
		if(!g_flv_audio)
		{
			printf("���ļ�ʧ��: %s\n",flv_audio_name);
			system("pause");
			exit(0);
		}
	}

	if(flv_out_name)
	{
		g_flv_out = fopen(flv_out_name,"wb");
		if(!g_flv_out)
		{
			printf("���ļ�ʧ��: %s\n",flv_out_name);
			system("pause");
			exit(0);
		}
	}
}

//�ر�flv����ļ�
void closeFlvFiles()   
{
	if(g_flv_file)
		fclose(g_flv_file);
	if(g_flv_tag)
		fclose(g_flv_tag);
	if(g_flv_video)
		fclose(g_flv_video);
	if(g_flv_audio)
		fclose(g_flv_audio);
	if(g_flv_out)
		fclose(g_flv_out);
}