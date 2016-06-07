#ifndef XIAOC_TAG_H
#define XIAOC_TAG_H
//����  �����  20160601  xiaoc@pku.edu.cn

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
            unsigned char signature[3]; //�ֱ���ASCII���ʾ�� F L V
            unsigned char version;      //�汾��һ���ֽڱ�ʾ��һ��Ϊ0x01
            unsigned char present;      //4��ʾֻ����Ƶ 1��ʾֻ����Ƶ 5 ��ʾ������ƵҲ����Ƶ ����ֵ�Ƿ�
            unsigned char size[4];      //ΪFLV Header�ĳ��ȣ�Ϊ�̶�ֵ0x00000009  �ڱ�׼�й涨���汾1һ��Ϊ9����δ���汾�п��ܻ�����
        };
        unsigned char data[9];          //�洢����FLVͷ������ ���������ݹ����ڴ�
    };
public:
    unsigned char* getSignature()//��ȡǩ��
    {
        return signature;
    }
    unsigned char getVersion()//��ȡ�汾��Ϣ
    {
        return version;
    }
    unsigned char getPresent()//��ȡ����Ϣ
    {
        return present;
    }
    unsigned int getSize()//��ȡsize
    {
        int s = size[0]*M16E6 + size[1]*M16E4 + size[2]*M16E2 + size[3];
        return s;
    }
    unsigned char* getData()//��ȡȫ����������
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
            unsigned char tagType;      //TAG����: 8 ��ʾ��Ƶtag 9��ʾ��Ƶtag 18��ʾ�ű����� 40 ��ʾ�����˲�����Ƶ 41��ʾ�����˲�����Ƶ 50��ʾ�����˲��Ľű����� ����ֵ����
            unsigned char datasize[3];  //ռ24λ ��ʾ��ǰtag�ĺ������ȵ��ڵ�ǰ����tag���ȼ�ȥ11��tagͷ��Ϣ��
            unsigned char timestamp[4]; //ռ24λΪ��Ե�һ��Tag��ʱ�������˵�һ��Tag��ʱ���Ϊ0��Ҳ���Խ�����Tag��ʱ���ȫ����Ϊ0�����������Զ����� ÿ��tag�ĵ�����ֽڿ�ʼ
            unsigned char streamID;     //һֱΪ0
        };
        unsigned char data[11];
    };
public:
    unsigned char getTagType()//TAG����
    {
        return tagType;
    }
    unsigned int getSize()//��ȡsize
    {
        int s = datasize[0]*M16E4 + datasize[1]*M16E2 + datasize[2];
        return s + 11;
    }
    unsigned int getTimeStamp() //��ȡʱ���
    {
        unsigned int s = timestamp[3]*M16E6 + timestamp[0]*M16E4 + timestamp[1]*M16E2 + timestamp[2];
        return s;
    }
    void printfTimeStamp(char *p)//��ӡʱ���
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
    unsigned char getStreamID()//��ȡ������
    {
        return streamID;
    }
    unsigned char* getData()//��ȡȫ����������
    {
        return data;
    }
};

class TagData
{
public:
    TagHeader*      m_tagHeader; //��ǰ��Tagͷ��Ϣ
    unsigned char*  data; //���TagData��Ϣ
    unsigned int    size; //���TagData��size
    unsigned int    decoderTimeStamp; //����ʱ���
    flv_param*      m_param;

    MetaData        m_metaData;   //�ű�����
    Video           m_video;      //��Ƶ����

    void anlysisTag();//����tag
};

#endif