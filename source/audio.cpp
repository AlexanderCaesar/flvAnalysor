//����  �����  20160608  xiaoc@pku.edu.cn
#include "audio.h"
#include "tag.h"
#include "flvfile.h"

void Audio::anlysisAudio(TagData*  tagData)
{
    m_tagData = tagData;
    m_count   = 0;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"++++++++++++++++++��Ƶ����++++++++++++++++++\n");

        unsigned char type = m_tagData->data[m_count++]; //�����Ƶͷ��Ϣ

        m_soundFormat = (type & 0xF0)>>4; //��Ƶ���ݸ�ʽ
        m_soundRate   = (type & 0x0C)>>2; //��Ƶ������
        m_soundSize   = (type & 0x02)>>1; //��Ƶ��������
        m_SoundType   = (type & 0x01);    //��Ƶ����
        switch(m_soundFormat)
        {
        case  0:fprintf(g_flv_tag,"FLV ��Ƶ��������:   Linear PCM, platform endian %d\n",m_soundFormat);break;
        case  1:fprintf(g_flv_tag,"FLV ��Ƶ��������:   ADPCM %d\n",m_soundFormat);break;
        case  2:fprintf(g_flv_tag,"FLV ��Ƶ��������:   MP3 %d\n",m_soundFormat);break;
        case  3:fprintf(g_flv_tag,"FLV ��Ƶ��������:   Linear PCM, little endian %d\n",m_soundFormat);break;
        case  4:fprintf(g_flv_tag,"FLV ��Ƶ��������:   Nellymoser 16 kHz mono %d\n",m_soundFormat);break;
        case  5:fprintf(g_flv_tag,"FLV ��Ƶ��������:   Nellymoser 8 kHz mono %d\n",m_soundFormat);break;
        case  6:fprintf(g_flv_tag,"FLV ��Ƶ��������:   Nellymoser %d\n",m_soundFormat);break;
        case  7:fprintf(g_flv_tag,"FLV ��Ƶ��������:   G.711 A-law logarithmic PCM %d\n",m_soundFormat);break;
        case  8:fprintf(g_flv_tag,"FLV ��Ƶ��������:   G.711 mu-law logarithmic PCMn %d\n",m_soundFormat);break;
        case  9:fprintf(g_flv_tag,"FLV ��Ƶ��������:   �����ֶ� δ��ʹ�� %d\n",m_soundFormat);break;
        case 10:fprintf(g_flv_tag,"FLV ��Ƶ��������:   AAC %d\n",m_soundFormat);break;
        case 11:fprintf(g_flv_tag,"FLV ��Ƶ��������:   Speex %d\n",m_soundFormat);break;
        case 12:fprintf(g_flv_tag,"FLV ��Ƶ��������:   δ���� %d\n",m_soundFormat);break;
        case 13:fprintf(g_flv_tag,"FLV ��Ƶ��������:   δ���� %d\n",m_soundFormat);break;
        case 14:fprintf(g_flv_tag,"FLV ��Ƶ��������:   MP3 8 kHz %d\n",m_soundFormat);break;
        case 15:fprintf(g_flv_tag,"FLV ��Ƶ��������:   Device-specific sound %d\n",m_soundFormat);break;
        default:fprintf(g_flv_tag,"FLV ��Ƶ�������ʹ���:   �޴���������\n");g_errors++;
        }
        switch(m_soundRate)
        {
        case  0:fprintf(g_flv_tag,"FLV   ��Ƶ������:   5.5 kHz %d\n",m_soundRate);break;
        case  1:fprintf(g_flv_tag,"FLV   ��Ƶ������:   11  kHz %d\n",m_soundRate);break;
        case  2:fprintf(g_flv_tag,"FLV   ��Ƶ������:   22  kHz %d\n",m_soundRate);break;
        case  3:fprintf(g_flv_tag,"FLV   ��Ƶ������:   44  kHz %d\n",m_soundRate);break;
        }
        switch(m_soundSize)
        {
        case  0:fprintf(g_flv_tag,"FLV ��Ƶ��������:   8λ  %d\n",m_soundSize);break;
        case  1:fprintf(g_flv_tag,"FLV ��Ƶ��������:   16λ %d\n",m_soundSize);break;
        }
        switch(m_SoundType)
        {
        case  0:fprintf(g_flv_tag,"FLV ��Ƶ������ʽ:   mono������  %d\n",m_SoundType);break;
        case  1:fprintf(g_flv_tag,"FLV ��Ƶ������ʽ:   stereo�������� %d\n",m_SoundType);break;
        }

        if(m_soundFormat == 10) //AAC
        {
            m_AACPacketType = m_tagData->data[m_count++]; //AVC������
            switch(m_AACPacketType)
            {
            case  0:fprintf(g_flv_tag,"AAC   ��Ƶ������:   AACͷ��Ϣ\n");break;
            case  1:fprintf(g_flv_tag,"AAC   ��Ƶ������:   AACԭʼ����\n");break;
            default:fprintf(g_flv_tag,"AAC ��Ƶ�����ʹ���:   AAC������ֻ��Ϊ0,1\n");g_errors++;
            }

             //����� AAC ����

        }

    }    
}