#ifndef XIAOC_FLV_FILE_H
#define  XIAOC_FLV_FILE_H
//����  �����  20160601  xiaoc@pku.edu.cn

#include <stdio.h>
#include "flv.h"


extern FILE *g_flv_file;  //FLV ԭʼ�ļ�
extern FILE *g_flv_tag;   //��ӡÿ��tag����ϸ��Ϣ
extern FILE *g_flv_video; //�����Ƶ����Ϣ
extern FILE *g_flv_audio; //�����Ƶ����Ϣ
extern FILE *g_flv_out;   //����޸ĺ��flv��

extern char *flv_file_name;  //FLV ԭʼ�ļ�           -i ���� �磺-i brazil-bq.flv
extern char *flv_tag_name;   //��ӡÿ��tag����ϸ��Ϣ  -t ���� �磺-t brazil-bq.log
extern char *flv_video_name; //�����Ƶ����Ϣ         -v ���� �磺-v brazil-bq.264 
extern char *flv_audio_name; //�����Ƶ����Ϣ         -a ���� �磺-a brazil-bq.aac 
extern char *flv_out_name;   //����޸ĺ��flv��      -o ���� �磺-o brazil-new.flv 

void openFlvFiles();    //��flv����ļ�
void closeFlvFiles();   //�ر�flv����ļ�
#endif