//作者  蔡砚刚  20160601  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "flvfile.h"

#pragma warning(disable: 4996) // POSIX setmode and fileno deprecated

FILE *g_flv_file  = NULL;    //FLV 原始文件
FILE *g_flv_tag   = NULL;    //打印每个tag的详细信息
FILE *g_flv_video = NULL;    //输出视频流信息
FILE *g_flv_audio = NULL;    //输出音频流信息
FILE *g_flv_out   = NULL;    //输出修改后的flv流

char *flv_file_name  = NULL; //FLV 原始文件           -i 配置 如：-i brazil-bq.flv
char *flv_tag_name   = NULL; //打印每个tag的详细信息  -t 配置 如：-t brazil-bq.log
char *flv_video_name = NULL; //输出视频流信息         -v 配置 如：-v brazil-bq.264 
char *flv_audio_name = NULL; //输出音频流信息         -a 配置 如：-a brazil-bq.aac 
char *flv_out_name   = NULL; //输出修改后的flv流      -o 配置 如：-o brazil-new.flv 

//打开flv相关文件
void openFlvFiles()    
{
	if(flv_file_name)
	{
		g_flv_file = fopen(flv_file_name,"rb");
		if(!g_flv_file)
		{
			printf("打开文件失败: %s\n",flv_file_name);
			system("pause");
			exit(0);
		}
	}
	if(flv_tag_name)
	{
		g_flv_tag = fopen(flv_tag_name,"w");
		if(!g_flv_tag)
		{
			printf("打开文件失败: %s\n",flv_tag_name);
			system("pause");
			exit(0);
		}
	}

	if(flv_video_name)
	{
		g_flv_video = fopen(flv_video_name,"wb");
		if(!g_flv_video)
		{
			printf("打开文件失败: %s\n",flv_video_name);
			system("pause");
			exit(0);
		}
	}

	if(flv_audio_name)
	{
		g_flv_audio = fopen(flv_audio_name,"wb");
		if(!g_flv_audio)
		{
			printf("打开文件失败: %s\n",flv_audio_name);
			system("pause");
			exit(0);
		}
	}

	if(flv_out_name)
	{
		g_flv_out = fopen(flv_out_name,"wb");
		if(!g_flv_out)
		{
			printf("打开文件失败: %s\n",flv_out_name);
			system("pause");
			exit(0);
		}
	}
}

//关闭flv相关文件
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