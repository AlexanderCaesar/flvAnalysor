#ifndef XIAOC_VIDEO_H
#define XIAOC_VIDEO_H
//作者  蔡砚刚  20160604  xiaoc@pku.edu.cn

class TagData;

class Video
{
public:
    TagData*     m_tagData;//指向当前的Tag文件
    unsigned int m_count;  //指示当前数据读取的位置
    char         str[1000];//用于临时存储字符串

    /*
    1 关键帧 （应用于AVC）
    2 非关键帧（应用于AVC）
    3 h263的非关键帧
    4 服务器生成关键帧
    5 视频信息或命令帧   **/
    int          m_frameType;//帧类型

    /*
    2 H.263
    3 屏幕视频
    4 VP6
    5 VP6 并且有alpha通道
    6 屏幕视频版本2
    7 AVC（H.264）    **/
    int          m_codecID; //编码器类型

    /*
    仅在AVC时有此字段
    0  AVC sequence header (SPS、PPS信息等)
    1 AVC NALU
    2 AVC end of sequence (lower level NALU sequence ender is
    not required or supported)**/
    int          m_AVCPacketType; //AVC包类型

    /*
    相对时间戳 如果AVCPacketType=0x01， 为相对时间戳；
    其它，均为0；  
    该值表示当前帧的显示时间  tag的时间为解码时间    
    显示时间等于 解码时间 + CompositionTime    **/
    int          m_CompositionTime; //显示时间偏移

    void   printfTimeStamp(char *p, int timestamp);//打印时间戳

    void   AVCDecoderConfigurationRecord();//H264 SPS PPS 等信息
    void   decodeH264();                           //H264解码
    void   anlysisVideo(TagData*  tagData);//分析视频数据

};

#endif