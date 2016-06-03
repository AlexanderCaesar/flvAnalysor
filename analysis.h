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
    TagHeader* m_tagHeader;     //��ǰ��Tagͷ��Ϣ
    TagData    m_tagData;       //TagData��Ϣ


	FlvHeader* m_mend_flvheader;//�޸ĺ��flvͷ��Ϣ
    TagHeader* m_mend_tagHeader;//�޸ĺ��Tagͷ��Ϣ
    TagData    m_mend_tagData;  //�޸ĺ��TagData��Ϣ

    int        m_tagConter;     //ͳ��tag�ĸ���
    int        m_errors;        //ͳ�ƴ������

public:
	Anlysis();
	void create(flv_param *param);/* �����ڴ� */
	void destory();/* �ͷ��ڴ� */

	/* ����flv�ļ�ͷ */
	void anlysisFlvHeader();

    /* ������һ��Tag Size */
    void readFirstTagSize();

    /* ����tag Tagͷ */
    bool anlysisFlvTagHeader();

    /* ����ǰһ��Tag Size */
    void readPreTagSize();

	/* ����flv�ļ� */
	void anlysis();
};
#endif