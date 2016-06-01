#ifndef XIAOC_FLV_FILE_H
#define  XIAOC_FLV_FILE_H
//作者  蔡砚刚  20160601  xiaoc@pku.edu.cn

#include <stdio.h>
#include "flv.h"


extern FILE *g_flv_file;  //FLV 原始文件
extern FILE *g_flv_tag;   //打印每个tag的详细信息
extern FILE *g_flv_video; //输出视频流信息
extern FILE *g_flv_audio; //输出音频流信息
extern FILE *g_flv_out;   //输出修改后的flv流

extern char *flv_file_name;  //FLV 原始文件           -i 配置 如：-i brazil-bq.flv
extern char *flv_tag_name;   //打印每个tag的详细信息  -t 配置 如：-t brazil-bq.log
extern char *flv_video_name; //输出视频流信息         -v 配置 如：-v brazil-bq.264 
extern char *flv_audio_name; //输出音频流信息         -a 配置 如：-a brazil-bq.aac 
extern char *flv_out_name;   //输出修改后的flv流      -o 配置 如：-o brazil-new.flv 

void openFlvFiles();    //打开flv相关文件
void closeFlvFiles();   //关闭flv相关文件
#endif