//����  �����  20160604  xiaoc@pku.edu.cn
#include "video.h"
#include "tag.h"
#include "flvfile.h"

//��ָ�����ײ���  ue(v)
void Video::xReadUvlc(unsigned int& ruiVal)
{
    unsigned int uiVal = 0;
    unsigned int uiCode = 0;
    unsigned int uiLength;       

    uiCode = (m_tagData->data[m_count]>>(m_left-1))%2;
    m_left--;
    if(m_left == 0)
    {
        m_count++; //��ȡ�µ��ֽ�
        m_left = 8;
    }

    if( 0 == uiCode )
    {
        uiLength = 0;

        while( ! ( uiCode & 1 ))
        {
            uiCode = (m_tagData->data[m_count]>>(m_left-1))%2;  //��ǰ׺�룬֪��λ��
            m_left--;
            if(m_left == 0)
            {
                m_count++; //��ȡ�µ��ֽ�
                m_left = 8;
            }
            uiLength++;
        }

        for(unsigned int i = 0; i< uiLength; i++)
        {
            uiCode = (m_tagData->data[m_count]>>(m_left-1))%2;  //��ǰ׺�룬֪��λ��
            m_left--;
            if(m_left == 0)
            {
                m_count++; //��ȡ�µ��ֽ�
                m_left = 8;
            }

            uiVal = uiVal*2 + uiCode;//��סǰ���Ѿ�����һλ
        }
        
        uiVal += (1 << uiLength)-1;  //����-1���õ���ʵֵ
    }
    ruiVal = uiVal;
}


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
            fprintf(g_flv_tag,"+++++++++++SPS [%d] +++++++++++\n",i);
            fprintf(g_flv_tag,"SPS����:%d\n",sequenceParameterSetLength);
        }
        if(m_tagData->m_param->b_video) //�������
        {
            unsigned char NALHeader[4]={0,0,0,1};
            fwrite(NALHeader,1,4,g_flv_video);
            fwrite(m_tagData->data+m_count+2,1,sequenceParameterSetLength,g_flv_video);
        }
        m_count += sequenceParameterSetLength + 2;
    }
    numOfPictureParameterSets       = m_tagData->data[m_count++];//ռ��5λ ��ʾ��ǰPPS�ĸ��� 

    for(int i = 0; i < numOfPictureParameterSets; i++)
    {
        unsigned int  pictureParameterSetLength = m_tagData->data[m_count]*M16E2 + m_tagData->data[m_count + 1];
        if(m_tagData->m_param->b_tag)
        {
            fprintf(g_flv_tag,"+++++++++++PPS  [%d] +++++++++++\n",i);
            fprintf(g_flv_tag,"PPS����:%d\n",  pictureParameterSetLength);
        }
        if(m_tagData->m_param->b_video) //�������
        {
            unsigned char NALHeader[4]={0,0,0,1};
            fwrite(NALHeader,1,4,g_flv_video);
            fwrite(m_tagData->data+m_count+2,1, pictureParameterSetLength,g_flv_video);
        }
        m_count +=  pictureParameterSetLength + 2;
    } 
    if(m_count != m_tagData->size && m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"AVCDecoderConfigurationRecord��������δ�������\n");g_errors++;
    }

}

void Video::decodeH264NALs()               //H264 NAL ��Ƶ������Ϣ
{
    unsigned int NALs = 0;
    unsigned int temp_count = m_count;
    while(temp_count < m_tagData->size)
    {
        unsigned int length = m_tagData->data[temp_count + 0]*M16E6 + m_tagData->data[temp_count + 1]*M16E4 + m_tagData->data[temp_count + 2]*M16E2 + m_tagData->data[temp_count + 3];
        if(m_tagData->m_param->b_video) //�������
        {
            unsigned char NALHeader[4]={0,0,0,1};
            fwrite(NALHeader,1,4,g_flv_video);
            fwrite(m_tagData->data+temp_count+4,1, length,g_flv_video);
        }
        temp_count += length + 4;
        NALs ++;
    }
    if(temp_count != m_tagData->size && m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"decodeH264NALs��������δ�������\n");g_errors++;
    }
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"+++++++++++NAL��Ԫ��Ϣ+++++++++++\n");
        fprintf(g_flv_tag,"NAL��Ԫ����: %d \n",NALs);
        for(unsigned int i = 0; i < NALs; i++)
        {
            fprintf(g_flv_tag,"+++++++++++NAL  [%d] +++++++++++\n",i);
            unsigned int length = m_tagData->data[m_count + 0]*M16E6 + m_tagData->data[m_count + 1]*M16E4 + m_tagData->data[m_count + 2]*M16E2 + m_tagData->data[m_count + 3];
            fprintf(g_flv_tag,"NAL����: %d \n",length);
            m_count += 4;
            unsigned char nal_ref_idc = (m_tagData->data[m_count]&0x60)>>5;//NAL���ȼ�  ȡֵ��Χ0~3��ֵԽ�߱�ʾNALԽ��Ҫ����Ҫ�����յ�������
            fprintf(g_flv_tag,"NALȨ��: %d  \n",nal_ref_idc);
            unsigned char nal_unit_type = m_tagData->data[m_count]&0x1F;
            m_count++;

            switch(nal_unit_type)
            {
            case  0:fprintf(g_flv_tag,"NAL���ͣ�%d δʹ��\n",nal_unit_type);break;
            case  1:fprintf(g_flv_tag,"NAL���ͣ�%d ����������IDRͼ��Ƭ\n",nal_unit_type);break;
            case  2:fprintf(g_flv_tag,"NAL���ͣ�%d Ƭ����A\n",nal_unit_type);break;
            case  3:fprintf(g_flv_tag,"NAL���ͣ�%d Ƭ����B\n",nal_unit_type);break;
            case  4:fprintf(g_flv_tag,"NAL���ͣ�%d Ƭ����C\n",nal_unit_type);break;
            case  5:fprintf(g_flv_tag,"NAL���ͣ�%d IDRͼ���е�Ƭ\n",nal_unit_type);break;
            case  6:fprintf(g_flv_tag,"NAL���ͣ�%d ������ǿ��Ϣ��ԪSEI\n",nal_unit_type);break;
            case  7:fprintf(g_flv_tag,"NAL���ͣ�%d ���в�����SPS\n",nal_unit_type);break;
            case  8:fprintf(g_flv_tag,"NAL���ͣ�%d ͼ�������PPS\n",nal_unit_type);break;
            case  9:fprintf(g_flv_tag,"NAL���ͣ�%d �ֽ��\n",nal_unit_type);break;
            case 10:fprintf(g_flv_tag,"NAL���ͣ�%d ���н���\n",nal_unit_type);break;
            case 11:fprintf(g_flv_tag,"NAL���ͣ�%d ��������\n",nal_unit_type);break;
            case 12:fprintf(g_flv_tag,"NAL���ͣ�%d ���\n",nal_unit_type);break;
            default:fprintf(g_flv_tag,"NAL���ͣ�%d ��������δʹ��\n",nal_unit_type);
            }

            temp_count = m_count;//��ʱ��� count��Ϣ������ᱻ����
            m_left = 8; //��ǰ�ֽڻ�ʣ��8λδ��
            if(nal_unit_type == 1||nal_unit_type == 5) //����sliceheader
            {
                unsigned int first_mb_in_slice = 0;
                unsigned int slice_type = 0;
                xReadUvlc(first_mb_in_slice);
                xReadUvlc(slice_type);
                fprintf(g_flv_tag,"first_mb_in_slice��%d \n",first_mb_in_slice);
                switch(slice_type)
                {
                case  0:fprintf(g_flv_tag,"Slice���ͣ�%d P Slice\n",slice_type);break;
                case  1:fprintf(g_flv_tag,"Slice���ͣ�%d B Slice\n",slice_type);break;
                case  2:fprintf(g_flv_tag,"Slice���ͣ�%d I Slice\n",slice_type);break;
                case  3:fprintf(g_flv_tag,"Slice���ͣ�%d SP Slice\n",slice_type);break;
                case  4:fprintf(g_flv_tag,"Slice���ͣ�%d SI Slice\n",slice_type);break;
                case  5:fprintf(g_flv_tag,"Slice���ͣ�%d P Slice\n",slice_type);break;
                case  6:fprintf(g_flv_tag,"Slice���ͣ�%d B Slice\n",slice_type);break;
                case  7:fprintf(g_flv_tag,"Slice���ͣ�%d I Slice\n",slice_type);break;
                case  8:fprintf(g_flv_tag,"Slice���ͣ�%d SP Slice\n",slice_type);break;
                case  9:fprintf(g_flv_tag,"Slice���ͣ�%d SI Slice\n",slice_type);break;
                default:fprintf(g_flv_tag,"Slice���ʹ���%d �޴�֡����\n",slice_type);
                }
            }
            m_count = temp_count + length -1;//ǰ����ȡ��һ��ͷ��Ϣ
            //����NAL��Ԫ
        }        
    }
}
void Video::decodeH264() //����H264����
{
    if(m_AVCPacketType == 0) //����ͷ��Ϣ
    {
        AVCDecoderConfigurationRecord();
    }

    if(m_AVCPacketType == 1)//��Ƶ����
    {
        decodeH264NALs();
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