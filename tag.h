#ifndef XIAOC_TAG_H
#define XIAOC_TAG_H
//作者  蔡砚刚  20160601  xiaoc@pku.edu.cn

#include <stdio.h>
#include "flv.h"
#include "metadata.h"
#include "video.h"
#pragma warning(disable: 4996) // POSIX setmode and fileno deprecated

#define M16E6 16777216 //16^6
#define M16E4 65536    //16^4
#define M16E2 256      //16^4
struct FlvHeader
{
private:
    union
    {
        struct
        {
            unsigned char signature[3]; //分别是ASCII码表示的 F L V
            unsigned char version;      //版本用一个字节表示：一般为0x01
            unsigned char present;      //4表示只有音频 1表示只有视频 5 表示既有视频也有音频 其他值非法
            unsigned char size[4];      //为FLV Header的长度，为固定值0x00000009  在标准中规定，版本1一定为9，在未来版本中可能会修正
        };
        unsigned char data[9];          //存储整个FLV头的数据 与上述数据共享内存
    };
public:
    unsigned char* getSignature()//获取签名
    {
        return signature;
    }
    unsigned char getVersion()//获取版本信息
    {
        return version;
    }
    unsigned char getPresent()//获取流信息
    {
        return present;
    }
    unsigned int getSize()//获取size
    {
        int s = size[0]*M16E6 + size[1]*M16E4 + size[2]*M16E2 + size[3];
        return s;
    }
    unsigned char* getData()//获取全部数据区域
    {
        return data;
    }
};

struct TagHeader
{
    private:
    union
    {
        struct
        {
            unsigned char tagType;      //TAG类型: 8 表示音频tag 9表示视频tag 18表示脚本数据 40 表示经过滤波的音频 41表示经过滤波的视频 50表示经过滤波的脚本数据 其它值错误
            unsigned char datasize[3];  //占24位 表示当前tag的后续长度等于当前整个tag长度减去11（tag头信息）
            unsigned char timestamp[4]; //占24位为相对第一个Tag的时间戳，因此第一个Tag的时间戳为0。也可以将所有Tag的时间戳全配置为0，解码器会自动处理。 每个tag的第五个字节开始
            unsigned char streamID;     //一直为0
        };
        unsigned char data[11];
    };
public:
    unsigned char getTagType()//TAG类型
    {
        return tagType;
    }
    unsigned int getSize()//获取size
    {
        int s = datasize[0]*M16E4 + datasize[1]*M16E2 + datasize[2];
        return s + 11;
    }
    unsigned int getTimeStamp() //获取时间戳
    {
        unsigned int s = timestamp[3]*M16E6 + timestamp[0]*M16E4 + timestamp[1]*M16E2 + timestamp[2];
        return s;
    }
    void printfTimeStamp(char *p)//打印时间戳
    {
        unsigned int timestamp = getTimeStamp();  

        int hour = timestamp/(3600*1000);  
        timestamp-= hour * 3600*1000;  
        int minutes = timestamp/(60*1000);  
        timestamp-= minutes * 60*1000;   
        int seconds = timestamp/1000;    
        timestamp-= seconds*1000;  
        sprintf(p,"%2d:%2d:%2d:%3d",hour,minutes,seconds,timestamp); //millisecond 
    }
    unsigned char getStreamID()//获取流类型
    {
        return streamID;
    }
    unsigned char* getData()//获取全部数据区域
    {
        return data;
    }
};

class TagData
{
public:
    TagHeader*      m_tagHeader; //当前的Tag头信息
    unsigned char*  data; //存放TagData信息
    unsigned int    size; //存放TagData的size
    unsigned int    decoderTimeStamp; //解码时间戳
    flv_param*      m_param;

    MetaData        m_metaData;   //脚本数据
    Video           m_video;      //视频数据

    void anlysisTag();//分析tag
};

#endif