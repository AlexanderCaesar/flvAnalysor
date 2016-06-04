#ifndef XIAOC_FLV_H
#define XIAOC_FLV_H
//作者  蔡砚刚  20160601  xiaoc@pku.edu.cn

typedef struct flv_param
{
	/* 配置信息
	-i 配置 如：-i brazil-bq.flv
	-t 配置 如：-t brazil-bq.log
	-v 配置 如：-v brazil-bq.264
	-a 配置 如：-a brazil-bq.aac
	**/
	int  b_tag;   //是否打印每个tag的详细信息  -t 配置 如：-t brazil-bq.log
	int  b_video; //是否输出视频流信息         -v 配置 如：-v brazil-bq.264 
	int  b_audio; //是否输出音频流信息         -a 配置 如：-a brazil-bq.aac 
	int  b_nflv;  //是否输出新的flv文件        -o 配置 如：-o brazil-new.flv

}flv_param;

/* 设置flv的默认参数 */
void flv_param_default(flv_param *param);

/* 解析配置参数 */
bool parse(flv_param *param,int argc, char **argv);

#endif
