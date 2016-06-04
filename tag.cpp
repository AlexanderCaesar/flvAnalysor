//作者  蔡砚刚  20160603  xiaoc@pku.edu.cn
#include "flvfile.h"
#include "tag.h"

/* 分析tag文件 */
void TagData::anlysisTag()
{
    switch(m_tagHeader->getTagType())
    {
    case 8: break;
    case 9:break;
    case 18:m_metaData.anlysisMetaData(this);break;
    case 40:fprintf(g_flv_tag,"TAG错误信息:    滤波不支持\n");g_errors++;break;
    case 41:fprintf(g_flv_tag,"TAG错误信息:    滤波不支持\n");g_errors++;break;
    case 50:fprintf(g_flv_tag,"TAG错误信息:    滤波不支持\n");g_errors++;break;
    default:fprintf(g_flv_tag,"TAG类型信息:    TAG类型错误，请检查当前TAG的头信息\n");g_errors++;
    }
}