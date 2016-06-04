#ifndef XIAOC_FLV_H
#define XIAOC_FLV_H
//����  �����  20160601  xiaoc@pku.edu.cn

typedef struct flv_param
{
	/* ������Ϣ
	-i ���� �磺-i brazil-bq.flv
	-t ���� �磺-t brazil-bq.log
	-v ���� �磺-v brazil-bq.264
	-a ���� �磺-a brazil-bq.aac
	**/
	int  b_tag;   //�Ƿ��ӡÿ��tag����ϸ��Ϣ  -t ���� �磺-t brazil-bq.log
	int  b_video; //�Ƿ������Ƶ����Ϣ         -v ���� �磺-v brazil-bq.264 
	int  b_audio; //�Ƿ������Ƶ����Ϣ         -a ���� �磺-a brazil-bq.aac 
	int  b_nflv;  //�Ƿ�����µ�flv�ļ�        -o ���� �磺-o brazil-new.flv

}flv_param;

/* ����flv��Ĭ�ϲ��� */
void flv_param_default(flv_param *param);

/* �������ò��� */
bool parse(flv_param *param,int argc, char **argv);

#endif
