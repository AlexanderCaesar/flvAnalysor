#ifndef XIAOC_ANALYSIS_H
#define XIAOC_ANALYSIS_H
//����  �����  20160601  xiaoc@pku.edu.cn
#include "flv.h"
#include "flvfile.h"
#include "tag.h"

class Anlysis
{
	flv_param* m_param;         //���ò�����Ϣ
	FlvHeader* m_flvheader;     //flvͷ��Ϣ
	FlvHeader* m_mend_flvheader;//�޸ĺ��flvͷ��Ϣ
public:
	Anlysis();
	void create(flv_param *param);/* �����ڴ� */
	void destory();/* �ͷ��ڴ� */

	/* ����flv�ļ�ͷ */
	void anlysisFlvHeader();

	/* ����flv�ļ� */
	void anlysis();
};
#endif