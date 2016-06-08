#ifndef XIAOC_AUDIO_H
#define XIAOC_AUDIO_H
//作者  蔡砚刚  20160608  xiaoc@pku.edu.cn

class TagData;

class Audio
{
public:
    TagData*     m_tagData;//指向当前的Tag文件
    unsigned int m_count;  //指示当前数据读取的位置

    /*
    0 表示 Linear PCM, platform endian
    1 表示 ADPCM
    2 表示 MP3
    3 表示 Linear PCM, little endian
    4 表示 Nellymoser 16 kHz mono
    5 表示 Nellymoser 8 kHz mono
    6 表示 Nellymoser
    7 表示 G.711 A-law logarithmic PCM
    8 表示 G.711 mu-law logarithmic PCM
    9 表示 保留字段 未来使用
    10表示 AAC
    11表示 Speex
    12表示 未定义
    13表示 未定义
    14表示 MP3 8 kHz
    15表示Device-specific sound    **/
    unsigned char m_soundFormat;//占4位  第一个字节的 0~3位 音频数据格式 其中7 8 14 15为保留字段 aac在Flash Player9版本以上支持 speex在Flash Player10版本以上支持
    unsigned char m_soundRate;  //占2位  第一个字节的（4~5位）表示音频采样率 0 = 5.5 kHz，1 = 11 kHz，2 = 22 kHz，3 = 44 kHz
    unsigned char m_soundSize;  //占1位 第一个字节的第6位 表示采样精度，0 = 8bits，1 = 16bits。
    unsigned char m_SoundType;  //占1位第一个字节的第7位 表示音频类型，0 = mono单声道，1 = stereo立体声道。
    unsigned char m_AACPacketType;//占8位（只在aac情况下解析，否则跳过 SoundFormat ==10时）0 aac头信息  1 aac 原始数据

    void   anlysisAudio(TagData*  tagData);//分析视频数据

};


#endif