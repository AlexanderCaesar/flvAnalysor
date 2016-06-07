//����  �����  20160604  xiaoc@pku.edu.cn
#include "video.h"
#include "tag.h"
#include "flvfile.h"

void Video::printfTimeStamp(char *p, int timestamp)//��ӡʱ���
{
    int hour = timestamp/(3600*1000);  
    timestamp-= hour * 3600*1000;  
    int minutes = timestamp/(60*1000);  
    timestamp-= minutes * 60*1000;   
    int seconds = timestamp/1000;    
    timestamp-= seconds*1000;  
    sprintf(p,"%2d:%2d:%2d:%3d",hour,minutes,seconds,timestamp); //millisecond 
}

void Video::AVCDecoderConfigurationRecord()//H264 SPS PPS ����Ϣ
{
    unsigned char configurationVersion             = m_tagData->data[m_count++];//���ð汾 ռ��8λ  һ��Ϊ1 ��ǰֵΪ��0x 01
    unsigned char AVCLevelIndicationProfile_idc    = m_tagData->data[m_count++];// profile_idc ռ��8λ ��H.264��׼SPS��һ���ֶ�profile_idc�������� ָ������ profile  ��ǰֵ��0x64 ��ʾ���õ���high����
    unsigned char profile_compatibility            = m_tagData->data[m_count++];//ռ��8λ ��ʾ���H.264���ĸ���¼��׼ 0��ʾ���޶�
    unsigned char AVCLevelIndicationLevel_idc      = m_tagData->data[m_count++];//level_idc ռ��8λ ��H.264��׼SPS��һ���ֶ�level_idc�������� ָ������ level ��ǰֵ

    unsigned char reserveBit0                      = m_tagData->data[m_count]&0xFC;//����λ ռ6λ  ֵһ��Ϊ'111111'  ��ǰֵ��0xFF ǰ6λΪ '111111' ����Ϊʮ����252
    unsigned char lengthSizeMinusOne               = m_tagData->data[m_count]&0x03;// ռ��2λ�� ��ʾNAL��Ԫͷ�ĳ���  0 ��ʾ1�ֽ� 1��ʾ2�ֽ� 2��ʾ3�ֽ�  3 ��ʾ4�ֽ�
    m_count++;
    unsigned char reserveBit1                      = m_tagData->data[m_count]&0xE0;//����λ ռ3λ  ֵһ��Ϊ'111' ����Ϊʮ����224
    unsigned char numOfSequenceParameterSets       = m_tagData->data[m_count]&0x1F;//ռ��5λ ��ʾ��ǰSPS�ĸ���  
    m_count++;

    unsigned int tmep_count = m_count;
    for(int i = 0; i < numOfSequenceParameterSets; i++)
    {
        unsigned int sequenceParameterSetLength = m_tagData->data[tmep_count]*M16E2 + m_tagData->data[tmep_count + 1];
        tmep_count += sequenceParameterSetLength + 2;
    }
    unsigned char numOfPictureParameterSets       = m_tagData->data[tmep_count];//ռ��5λ ��ʾ��ǰPPS�ĸ��� 

    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"+++++++++++AVC������Ϣ+++++++++++\n");
        if(configurationVersion == 1)
        {
            fprintf(g_flv_tag,"configurationVersion��1 �汾��Ϣ\n");
        }
        else
        {
            fprintf(g_flv_tag,"configurationVersion��1 �汾��Ϣ����\n");g_errors++;
        }
        switch(AVCLevelIndicationProfile_idc)
        {
        case  66:fprintf(g_flv_tag,"profile_idc: 66  A.2.1 ��׼����\n");break;
        case  77:fprintf(g_flv_tag,"profile_idc: 77  A.2.2 ��Ҫ����\n");break;
        case  88:fprintf(g_flv_tag,"profile_idc: 88  A.2.3 ��չ����\n");break;
        case 100:fprintf(g_flv_tag,"profile_idc: 100 A.2.4 �߼����\n");break;
        case 110:fprintf(g_flv_tag,"profile_idc: 110 A.2.5 �߼�10���\n");break;
        case 122:fprintf(g_flv_tag,"profile_idc: 122 A.2.6 �߼� 4:2:2 ���\n");break;
        case 144:fprintf(g_flv_tag,"profile_idc: 144 A.2.7 �߼� 4:4:4 ��� \n");break;
        default:fprintf(g_flv_tag,"���δ������:   �޴˵��� %d\n",AVCLevelIndicationProfile_idc);g_errors++;
        }
        fprintf(g_flv_tag,"level_idc: %d  ���𣬶��嵱ǰ������ʵ���Ϣ\n",AVCLevelIndicationLevel_idc);
        fprintf(g_flv_tag,"NALͷռ���ֽ���: %d �ֽ�\n",lengthSizeMinusOne+1);
        if(reserveBit0!=252||reserveBit1!=224)
        {
            fprintf(g_flv_tag,"����λ�ֶδ���Ӧ��ȫΪ1\n",AVCLevelIndicationProfile_idc);g_errors++;
        }
        fprintf(g_flv_tag,"SPS����: %d\n",numOfSequenceParameterSets);
        fprintf(g_flv_tag,"PPS����: %d\n",numOfPictureParameterSets);
    }

    for(int i = 0; i < numOfSequenceParameterSets; i++)
    {
        unsigned int sequenceParameterSetLength = m_tagData->data[m_count]*M16E2 + m_tagData->data[m_count + 1];
        if(m_tagData->m_param->b_tag)
        {
            fprintf(g_flv_tag,"+++++++++++SPS %d +++++++++++\n",i);
            fprintf(g_flv_tag,"SPS����:%d\n",sequenceParameterSetLength);
        }
        m_count += sequenceParameterSetLength + 2;
    }
    //numOfPictureParameterSets       = m_tagData->data[m_count];//ռ��5λ ��ʾ��ǰPPS�ĸ��� 

}
void Video::decodeH264() //����H264����
{
    if(m_AVCPacketType == 0) //����ͷ��Ϣ
    {
        AVCDecoderConfigurationRecord();
    }

    if(m_AVCPacketType == 1)//��Ƶ����
    {

    }
}
void Video::anlysisVideo(TagData*  tagData)
{
    m_tagData = tagData;
    m_count   = 0;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"++++++++++++++++++��Ƶ����++++++++++++++++++\n");

        unsigned char type = m_tagData->data[m_count++]; //��ű����������Լ�֡����

        m_frameType = (type & 0xF0)>>4; //֡����
        m_codecID   = (type & 0x0F);    //����������
        switch(m_frameType)
        {
        case  1:fprintf(g_flv_tag,"FLV ��Ƶ֡����:   �ؼ�֡ ��Ӧ����AVC��\n");break;
        case  2:fprintf(g_flv_tag,"FLV ��Ƶ֡����:   �ǹؼ�֡��Ӧ����AVC��\n");break;
        case  3:fprintf(g_flv_tag,"FLV ��Ƶ֡����:   h263�ķǹؼ�֡\n");break;
        case  4:fprintf(g_flv_tag,"FLV ��Ƶ֡����:   ���������ɹؼ�֡\n");break;
        case  5:fprintf(g_flv_tag,"FLV ��Ƶ֡����:   ��Ƶ��Ϣ������֡\n");break;
        default:fprintf(g_flv_tag,"FLV ��Ƶ֡���ʹ���:   �޴�֡����\n");g_errors++;
        }
        switch(m_codecID)
        {
        case  2:fprintf(g_flv_tag,"FLV ��Ƶ������:   H.263\n");break;
        case  3:fprintf(g_flv_tag,"FLV ��Ƶ������:   ��Ļ��Ƶ\n");break;
        case  4:fprintf(g_flv_tag,"FLV ��Ƶ������:   VP6\n");break;
        case  5:fprintf(g_flv_tag,"FLV ��Ƶ������:   VP6 ������alphaͨ��\n");break;
        case  6:fprintf(g_flv_tag,"FLV ��Ƶ������:   ��Ļ��Ƶ�汾2\n");break;
        case  7:fprintf(g_flv_tag,"FLV ��Ƶ������:   AVC��H.264��\n");break;
        default:fprintf(g_flv_tag,"FLV ��Ƶ����������:   �޴˱��������ͻ��߲�֧��\n");g_errors++;
        }
        if(m_codecID == 7)
        {
            m_AVCPacketType = m_tagData->data[m_count++]; //AVC������
            switch(m_AVCPacketType)
            {
            case  0:fprintf(g_flv_tag,"AVC ��Ƶ������:   AVC ����ͷ��Ϣ\n");break;
            case  1:fprintf(g_flv_tag,"AVC ��Ƶ������:   AVC NAL��Ԫ\n");break;
            case  2:fprintf(g_flv_tag,"AVC ��Ƶ������:   AVC ���н���\n");break;
            default:fprintf(g_flv_tag,"AVC ��Ƶ�����ʹ���:   AVC������ֻ��Ϊ0,1,2\n");g_errors++;
            }

            if(m_AVCPacketType == 1)
            {
                m_CompositionTime = m_tagData->data[m_count + 0]*M16E4 + m_tagData->data[m_count + 1]*M16E4 + m_tagData->data[m_count + 2];
                printfTimeStamp(str,m_tagData->m_tagHeader->getTimeStamp() + m_CompositionTime);
                fprintf(g_flv_tag,"AVC ��ʾʱ���:   %s   offset = %d\n",str,m_CompositionTime);
            }
            else
            {
                m_CompositionTime = m_tagData->data[m_count + 0]*M16E4 + m_tagData->data[m_count + 1]*M16E4 + m_tagData->data[m_count + 2];
                printfTimeStamp(str,m_tagData->m_tagHeader->getTimeStamp() + m_CompositionTime);
                fprintf(g_flv_tag,"AVC ��ʾʱ���:   %s   offset = %d\n",str,m_CompositionTime);
                if(m_CompositionTime!=0)
                {
                    fprintf(g_flv_tag,"AVC ��ʾ�����:  ��ǰΪ����ͷoffset����Ϊ0\n",str,m_CompositionTime);
                }
            }
            m_count += 3;

            if(m_frameType == 5)
            {
                //�����
            }
            else
            {
                switch(m_codecID)
                {
                //�����
                case  7:decodeH264();break;
                default:fprintf(g_flv_tag,"FLV ��Ƶ����������:   Ŀǰֻ֧��H264����������������\n");g_errors++;
                }
            }

        }

    }    
}