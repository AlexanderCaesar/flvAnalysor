//����  �����  20160603  xiaoc@pku.edu.cn
#include "flvfile.h"
#include "tag.h"

/* ����tag�ļ� */
void TagData::anlysisTag()
{
    switch(m_tagHeader->getTagType())
    {
    case 8: break;
    case 9:break;
    case 18:m_metaData.anlysisMetaData(this);break;
    case 40:fprintf(g_flv_tag,"TAG������Ϣ:    �˲���֧��\n");g_errors++;break;
    case 41:fprintf(g_flv_tag,"TAG������Ϣ:    �˲���֧��\n");g_errors++;break;
    case 50:fprintf(g_flv_tag,"TAG������Ϣ:    �˲���֧��\n");g_errors++;break;
    default:fprintf(g_flv_tag,"TAG������Ϣ:    TAG���ʹ������鵱ǰTAG��ͷ��Ϣ\n");g_errors++;
    }
}