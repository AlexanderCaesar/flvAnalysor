//作者  蔡砚刚  20160608  xiaoc@pku.edu.cn
#include "audio.h"
#include "tag.h"
#include "flvfile.h"

void Audio::anlysisAudio(TagData*  tagData)
{
    m_tagData = tagData;
    m_count   = 0;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"++++++++++++++++++音频数据++++++++++++++++++\n");

        unsigned char type = m_tagData->data[m_count++]; //存放音频头信息

        m_soundFormat = (type & 0xF0)>>4; //音频数据格式
        m_soundRate   = (type & 0x0C)>>2; //音频采样率
        m_soundSize   = (type & 0x02)>>1; //音频采样精度
        m_SoundType   = (type & 0x01);    //音频类型
        switch(m_soundFormat)
        {
        case  0:fprintf(g_flv_tag,"FLV 音频数据类型:   Linear PCM, platform endian %d\n",m_soundFormat);break;
        case  1:fprintf(g_flv_tag,"FLV 音频数据类型:   ADPCM %d\n",m_soundFormat);break;
        case  2:fprintf(g_flv_tag,"FLV 音频数据类型:   MP3 %d\n",m_soundFormat);break;
        case  3:fprintf(g_flv_tag,"FLV 音频数据类型:   Linear PCM, little endian %d\n",m_soundFormat);break;
        case  4:fprintf(g_flv_tag,"FLV 音频数据类型:   Nellymoser 16 kHz mono %d\n",m_soundFormat);break;
        case  5:fprintf(g_flv_tag,"FLV 音频数据类型:   Nellymoser 8 kHz mono %d\n",m_soundFormat);break;
        case  6:fprintf(g_flv_tag,"FLV 音频数据类型:   Nellymoser %d\n",m_soundFormat);break;
        case  7:fprintf(g_flv_tag,"FLV 音频数据类型:   G.711 A-law logarithmic PCM %d\n",m_soundFormat);break;
        case  8:fprintf(g_flv_tag,"FLV 音频数据类型:   G.711 mu-law logarithmic PCMn %d\n",m_soundFormat);break;
        case  9:fprintf(g_flv_tag,"FLV 音频数据类型:   保留字段 未来使用 %d\n",m_soundFormat);break;
        case 10:fprintf(g_flv_tag,"FLV 音频数据类型:   AAC %d\n",m_soundFormat);break;
        case 11:fprintf(g_flv_tag,"FLV 音频数据类型:   Speex %d\n",m_soundFormat);break;
        case 12:fprintf(g_flv_tag,"FLV 音频数据类型:   未定义 %d\n",m_soundFormat);break;
        case 13:fprintf(g_flv_tag,"FLV 音频数据类型:   未定义 %d\n",m_soundFormat);break;
        case 14:fprintf(g_flv_tag,"FLV 音频数据类型:   MP3 8 kHz %d\n",m_soundFormat);break;
        case 15:fprintf(g_flv_tag,"FLV 音频数据类型:   Device-specific sound %d\n",m_soundFormat);break;
        default:fprintf(g_flv_tag,"FLV 音频数据类型错误:   无此数据类型\n");g_errors++;
        }
        switch(m_soundRate)
        {
        case  0:fprintf(g_flv_tag,"FLV   音频采样率:   5.5 kHz %d\n",m_soundRate);break;
        case  1:fprintf(g_flv_tag,"FLV   音频采样率:   11  kHz %d\n",m_soundRate);break;
        case  2:fprintf(g_flv_tag,"FLV   音频采样率:   22  kHz %d\n",m_soundRate);break;
        case  3:fprintf(g_flv_tag,"FLV   音频采样率:   44  kHz %d\n",m_soundRate);break;
        }
        switch(m_soundSize)
        {
        case  0:fprintf(g_flv_tag,"FLV 音频采样精度:   8位  %d\n",m_soundSize);break;
        case  1:fprintf(g_flv_tag,"FLV 音频采样精度:   16位 %d\n",m_soundSize);break;
        }
        switch(m_SoundType)
        {
        case  0:fprintf(g_flv_tag,"FLV 音频声道格式:   mono单声道  %d\n",m_SoundType);break;
        case  1:fprintf(g_flv_tag,"FLV 音频声道格式:   stereo立体声道 %d\n",m_SoundType);break;
        }

        if(m_soundFormat == 10) //AAC
        {
            m_AACPacketType = m_tagData->data[m_count++]; //AVC包类型
            switch(m_AACPacketType)
            {
            case  0:fprintf(g_flv_tag,"AAC   音频包类型:   AAC头信息\n");break;
            case  1:fprintf(g_flv_tag,"AAC   音频包类型:   AAC原始数据\n");break;
            default:fprintf(g_flv_tag,"AAC 音频包类型错误:   AAC包错误，只能为0,1\n");g_errors++;
            }

             //待添加 AAC 解析

        }

    }    
}