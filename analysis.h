#ifndef XIAOC_ANALYSIS_H
#define XIAOC_ANALYSIS_H
//作者  蔡砚刚  20160601  xiaoc@pku.edu.cn
#include "flv.h"
#include "flvfile.h"
#include "tag.h"

class Anlysis
{
	flv_param* m_param;         //配置参数信息
	FlvHeader* m_flvheader;     //flv头信息
	FlvHeader* m_mend_flvheader;//修改后的flv头信息
public:
	Anlysis();
	void create(flv_param *param);/* 申请内存 */
	void destory();/* 释放内存 */

	/* 分析flv文件头 */
	void anlysisFlvHeader();

	/* 分析flv文件 */
	void anlysis();
};
#endif