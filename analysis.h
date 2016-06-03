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
    TagHeader* m_tagHeader;     //当前的Tag头信息
    TagData    m_tagData;       //TagData信息


	FlvHeader* m_mend_flvheader;//修改后的flv头信息
    TagHeader* m_mend_tagHeader;//修改后的Tag头信息
    TagData    m_mend_tagData;  //修改后的TagData信息

    int        m_tagConter;     //统计tag的个数
    int        m_errors;        //统计错误个数

public:
	Anlysis();
	void create(flv_param *param);/* 申请内存 */
	void destory();/* 释放内存 */

	/* 分析flv文件头 */
	void anlysisFlvHeader();

    /* 分析第一个Tag Size */
    void readFirstTagSize();

    /* 分析tag Tag头 */
    bool anlysisFlvTagHeader();

    /* 分析前一个Tag Size */
    void readPreTagSize();

	/* 分析flv文件 */
	void anlysis();
};
#endif