//作者  蔡砚刚  20160604  xiaoc@pku.edu.cn
#include "video.h"
#include "tag.h"
#include "flvfile.h"

void Video::printfTimeStamp(char *p, int timestamp)//打印时间戳
{
    int hour = timestamp/(3600*1000);  
    timestamp-= hour * 3600*1000;  
    int minutes = timestamp/(60*1000);  
    timestamp-= minutes * 60*1000;   
    int seconds = timestamp/1000;    
    timestamp-= seconds*1000;  
    sprintf(p,"%2d:%2d:%2d:%3d",hour,minutes,seconds,timestamp); //millisecond 
}

void Video::AVCDecoderConfigurationRecord()//H264 SPS PPS 等信息
{
    unsigned char configurationVersion             = m_tagData->data[m_count++];//配置版本 占用8位  一定为1 当前值为：0x 01
    unsigned char AVCLevelIndicationProfile_idc    = m_tagData->data[m_count++];// profile_idc 占用8位 从H.264标准SPS第一个字段profile_idc拷贝而来 指明所用 profile  当前值：0x64 表示所用的是high档次
    unsigned char profile_compatibility            = m_tagData->data[m_count++];//占用8位 表示遵从H.264的哪个附录标准 0表示不限定
    unsigned char AVCLevelIndicationLevel_idc      = m_tagData->data[m_count++];//level_idc 占用8位 从H.264标准SPS第一个字段level_idc拷贝而来 指明所用 level 当前值

    unsigned char reserveBit0                      = m_tagData->data[m_count]&0xFC;//保留位 占6位  值一定为'111111'  当前值：0xFF 前6位为 '111111' 必须为十进制252
    unsigned char lengthSizeMinusOne               = m_tagData->data[m_count]&0x03;// 占用2位， 表示NAL单元头的长度  0 表示1字节 1表示2字节 2表示3字节  3 表示4字节
    m_count++;
    unsigned char reserveBit1                      = m_tagData->data[m_count]&0xE0;//保留位 占3位  值一定为'111' 必须为十进制224
    unsigned char numOfSequenceParameterSets       = m_tagData->data[m_count]&0x1F;//占用5位 表示当前SPS的个数  
    m_count++;

    unsigned int tmep_count = m_count;
    for(int i = 0; i < numOfSequenceParameterSets; i++)
    {
        unsigned int sequenceParameterSetLength = m_tagData->data[tmep_count]*M16E2 + m_tagData->data[tmep_count + 1];
        tmep_count += sequenceParameterSetLength + 2;
    }
    unsigned char numOfPictureParameterSets       = m_tagData->data[tmep_count];//占用5位 表示当前PPS的个数 

    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"+++++++++++AVC配置信息+++++++++++\n");
        if(configurationVersion == 1)
        {
            fprintf(g_flv_tag,"configurationVersion：1 版本信息\n");
        }
        else
        {
            fprintf(g_flv_tag,"configurationVersion：1 版本信息错误\n");g_errors++;
        }
        switch(AVCLevelIndicationProfile_idc)
        {
        case  66:fprintf(g_flv_tag,"profile_idc: 66  A.2.1 基准档次\n");break;
        case  77:fprintf(g_flv_tag,"profile_idc: 77  A.2.2 主要档次\n");break;
        case  88:fprintf(g_flv_tag,"profile_idc: 88  A.2.3 扩展档次\n");break;
        case 100:fprintf(g_flv_tag,"profile_idc: 100 A.2.4 高级简表\n");break;
        case 110:fprintf(g_flv_tag,"profile_idc: 110 A.2.5 高级10简表\n");break;
        case 122:fprintf(g_flv_tag,"profile_idc: 122 A.2.6 高级 4:2:2 简表\n");break;
        case 144:fprintf(g_flv_tag,"profile_idc: 144 A.2.7 高级 4:4:4 简表 \n");break;
        default:fprintf(g_flv_tag,"档次错误错误:   无此档次 %d\n",AVCLevelIndicationProfile_idc);g_errors++;
        }
        fprintf(g_flv_tag,"level_idc: %d  级别，定义当前最大码率等信息\n",AVCLevelIndicationLevel_idc);
        fprintf(g_flv_tag,"NAL头占用字节数: %d 字节\n",lengthSizeMinusOne+1);
        if(reserveBit0!=252||reserveBit1!=224)
        {
            fprintf(g_flv_tag,"保留位字段错误，应该全为1\n",AVCLevelIndicationProfile_idc);g_errors++;
        }
        fprintf(g_flv_tag,"SPS个数: %d\n",numOfSequenceParameterSets);
        fprintf(g_flv_tag,"PPS个数: %d\n",numOfPictureParameterSets);
    }

    for(int i = 0; i < numOfSequenceParameterSets; i++)
    {
        unsigned int sequenceParameterSetLength = m_tagData->data[m_count]*M16E2 + m_tagData->data[m_count + 1];
        if(m_tagData->m_param->b_tag)
        {
            fprintf(g_flv_tag,"+++++++++++SPS %d +++++++++++\n",i);
            fprintf(g_flv_tag,"SPS长度:%d\n",sequenceParameterSetLength);
        }
        m_count += sequenceParameterSetLength + 2;
    }
    //numOfPictureParameterSets       = m_tagData->data[m_count];//占用5位 表示当前PPS的个数 

}
void Video::decodeH264() //解码H264码流
{
    if(m_AVCPacketType == 0) //序列头信息
    {
        AVCDecoderConfigurationRecord();
    }

    if(m_AVCPacketType == 1)//视频数据
    {

    }
}
void Video::anlysisVideo(TagData*  tagData)
{
    m_tagData = tagData;
    m_count   = 0;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"++++++++++++++++++视频数据++++++++++++++++++\n");

        unsigned char type = m_tagData->data[m_count++]; //存放编码器类型以及帧类型

        m_frameType = (type & 0xF0)>>4; //帧类型
        m_codecID   = (type & 0x0F);    //编码器类型
        switch(m_frameType)
        {
        case  1:fprintf(g_flv_tag,"FLV 视频帧类型:   关键帧 （应用于AVC）\n");break;
        case  2:fprintf(g_flv_tag,"FLV 视频帧类型:   非关键帧（应用于AVC）\n");break;
        case  3:fprintf(g_flv_tag,"FLV 视频帧类型:   h263的非关键帧\n");break;
        case  4:fprintf(g_flv_tag,"FLV 视频帧类型:   服务器生成关键帧\n");break;
        case  5:fprintf(g_flv_tag,"FLV 视频帧类型:   视频信息或命令帧\n");break;
        default:fprintf(g_flv_tag,"FLV 视频帧类型错误:   无此帧类型\n");g_errors++;
        }
        switch(m_codecID)
        {
        case  2:fprintf(g_flv_tag,"FLV 视频编码器:   H.263\n");break;
        case  3:fprintf(g_flv_tag,"FLV 视频编码器:   屏幕视频\n");break;
        case  4:fprintf(g_flv_tag,"FLV 视频编码器:   VP6\n");break;
        case  5:fprintf(g_flv_tag,"FLV 视频编码器:   VP6 并且有alpha通道\n");break;
        case  6:fprintf(g_flv_tag,"FLV 视频编码器:   屏幕视频版本2\n");break;
        case  7:fprintf(g_flv_tag,"FLV 视频编码器:   AVC（H.264）\n");break;
        default:fprintf(g_flv_tag,"FLV 视频编码器错误:   无此编码器类型或者不支持\n");g_errors++;
        }
        if(m_codecID == 7)
        {
            m_AVCPacketType = m_tagData->data[m_count++]; //AVC包类型
            switch(m_AVCPacketType)
            {
            case  0:fprintf(g_flv_tag,"AVC 视频包类型:   AVC 序列头信息\n");break;
            case  1:fprintf(g_flv_tag,"AVC 视频包类型:   AVC NAL单元\n");break;
            case  2:fprintf(g_flv_tag,"AVC 视频包类型:   AVC 序列结束\n");break;
            default:fprintf(g_flv_tag,"AVC 视频包类型错误:   AVC包错误，只能为0,1,2\n");g_errors++;
            }

            if(m_AVCPacketType == 1)
            {
                m_CompositionTime = m_tagData->data[m_count + 0]*M16E4 + m_tagData->data[m_count + 1]*M16E4 + m_tagData->data[m_count + 2];
                printfTimeStamp(str,m_tagData->m_tagHeader->getTimeStamp() + m_CompositionTime);
                fprintf(g_flv_tag,"AVC 显示时间戳:   %s   offset = %d\n",str,m_CompositionTime);
            }
            else
            {
                m_CompositionTime = m_tagData->data[m_count + 0]*M16E4 + m_tagData->data[m_count + 1]*M16E4 + m_tagData->data[m_count + 2];
                printfTimeStamp(str,m_tagData->m_tagHeader->getTimeStamp() + m_CompositionTime);
                fprintf(g_flv_tag,"AVC 显示时间戳:   %s   offset = %d\n",str,m_CompositionTime);
                if(m_CompositionTime!=0)
                {
                    fprintf(g_flv_tag,"AVC 显示错误戳:  当前为序列头offset必须为0\n",str,m_CompositionTime);
                }
            }
            m_count += 3;

            if(m_frameType == 5)
            {
                //待添加
            }
            else
            {
                switch(m_codecID)
                {
                //待添加
                case  7:decodeH264();break;
                default:fprintf(g_flv_tag,"FLV 视频编码器错误:   目前只支持H264编码器，后面待添加\n");g_errors++;
                }
            }

        }

    }    
}