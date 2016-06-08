//作者  蔡砚刚  20160601  xiaoc@pku.edu.cn
#include <stdlib.h>
#include <string.h>
#include "analysis.h"
void author(flv_param *param)
{
    if(!param->b_tag) return;
    fprintf(g_flv_tag,"==================作者 信息=========================\n");
    fprintf(g_flv_tag,"网       名:    xiaoC/cabbage/Alexander/Caesar\n");
    fprintf(g_flv_tag,"邮       箱:    xiaoc@pku.edu.cn\n");
    fprintf(g_flv_tag,"博       客:    http://blog.csdn.net/cabbage2008\n");
}
Anlysis::Anlysis()
{
    m_param          = NULL;
    m_flvheader      = NULL;
    m_mend_flvheader = NULL;
    m_tagHeader      = NULL;
    m_mend_tagHeader = NULL;
    
};
/* 申请内存 */
void Anlysis::create(flv_param *param)
{
    bool ok = true;
    m_param = param;
    m_flvheader = (FlvHeader*)malloc(sizeof(FlvHeader)); //申请flv头内存
    m_tagHeader = (TagHeader*)malloc(sizeof(TagHeader)); //申请tag头内存
    ok = ok&&m_flvheader&&m_tagHeader;
    m_tagData.m_tagHeader = m_tagHeader;

    if(m_param->b_nflv)//如果需要输出新的flv文件
    {
        m_mend_flvheader = (FlvHeader*)malloc(sizeof(FlvHeader)); //申请新flv头内存
        m_mend_tagHeader = (TagHeader*)malloc(sizeof(TagHeader)); //申请新tag头内存
        ok = ok&&m_mend_flvheader&&m_mend_tagHeader;

        m_mend_tagData.m_tagHeader = m_mend_tagHeader;
    }

    if(!ok)
    {
        printf("Anlysis类申请内存失败\n");
        system("pause");
        exit(0);
    }
}
/* 释放内存 */
void Anlysis::destory()
{
    if(m_flvheader)
    {
        free(m_flvheader);
    }
    if(m_tagHeader)
    {
        free(m_tagHeader);
    }
    if(m_mend_flvheader)
    {
        free(m_mend_flvheader);
    }
    if(m_mend_tagHeader)
    {
        free(m_mend_tagHeader);
    }
}

/* 分析flv文件头 */
void Anlysis::anlysisFlvHeader()
{
    fread(m_flvheader->getData(),1,9,g_flv_file); //读取头信息

    if(m_param->b_tag)
    {
        fprintf(g_flv_tag,"==================FLV 头信息=========================\n");
        fprintf(g_flv_tag,"FLV数据签名:    %c%c%c\n",m_flvheader->getSignature()[0],m_flvheader->getSignature()[1],m_flvheader->getSignature()[2]);
        fprintf(g_flv_tag,"FLV版本信息:    %d\n",m_flvheader->getVersion());
        switch(m_flvheader->getPresent())
        {
        case 1:fprintf(g_flv_tag,"FLV数据信息:    只拥有视频流\n");break;
        case 4:fprintf(g_flv_tag,"FLV数据信息:    只拥有音频流\n");break;
        case 5:fprintf(g_flv_tag,"FLV数据信息:    拥有视频和音频流\n");break;
        default:fprintf(g_flv_tag,"FLV数据信息:   flv头信息错误\n");g_errors++;
        }
        fprintf(g_flv_tag,"FLV  头长度:    %d\n",m_flvheader->getSize());
    }
        
    if(m_param->b_nflv)
    {
        memcpy(m_mend_flvheader->getData(),m_flvheader->getData(),9);//拷贝元数据头信息
        {
            //flv 头信息 修改 待添加
        }
        fwrite(m_mend_flvheader->getData(),1,9,g_flv_out); //写新的头信息
    }
}

/* 分析第一个Tag Size */
void Anlysis::readFirstTagSize()
{
    unsigned char data[4];

    fread(data,1,4,g_flv_file);

    if(data[0]!= 0||data[1]!= 0||data[2]!= 0 ||data[3]!= 0 )
    {
        fprintf(g_flv_tag,"第一TAG  长:    错误：第一个TAG长度必须为0：%X%X%X%X\n",data[0],data[1],data[2],data[3]);g_errors++;
    }

    if(m_param->b_nflv)
    {
        fwrite(data,1,4,g_flv_out);
    }
}

/* 分析tag头 */
bool Anlysis::anlysisFlvTagHeader()
{
    if(!fread(m_tagHeader->getData(),1,11,g_flv_file)) //读取头信息
    {
        return false;
    }

    if(m_param->b_tag)
    {
        fprintf(g_flv_tag,"==================TAG %5d 头信息=========================\n",m_tagConter);
        switch(m_tagHeader->getTagType())
        {
        case 8:fprintf(g_flv_tag,"TAG类型信息:    音频TAG TAG无滤波\n");break;
        case 9:fprintf(g_flv_tag,"TAG类型信息:    视频TAG TAG无滤波\n");break;
        case 18:fprintf(g_flv_tag,"TAG类型信息:    脚本TAG TAG无滤波\n");break;
        case 40:fprintf(g_flv_tag,"TAG类型信息:    音频TAG TAG有滤波\n");break;
        case 41:fprintf(g_flv_tag,"TAG类型信息:    视频TAG TAG有滤波\n");break;
        case 50:fprintf(g_flv_tag,"TAG类型信息:    脚本TAG TAG有滤波\n");break;
        default:fprintf(g_flv_tag,"TAG类型信息:    TAG类型错误，请检查当前TAG的头信息\n");g_errors++;return false;
        }
        fprintf(g_flv_tag,"TAG    长度:    %d\n",m_tagHeader->getSize());
        char timestamp[100];
        m_tagHeader->printfTimeStamp(timestamp);
        fprintf(g_flv_tag,"解码时间戳 :    %d(%s) \n",m_tagHeader->getTimeStamp(),timestamp);
        if(m_tagHeader->getTimeStamp()==0)
        {
            fprintf(g_flv_tag,"注意：第一个Tag的时间戳为0。也可以将所有Tag的时间戳全配置为0，解码器会自动处理。 每个tag的第五个字节开始\n");
        }
        fprintf(g_flv_tag,"TAG    流ID:    %d 一般值固定为0\n",m_tagHeader->getStreamID());
        if(m_tagHeader->getStreamID())
        {
            fprintf(g_flv_tag,"TAG  流信息:    TAG流信息错误，一般值固定为0\n");g_errors++;
        }
    }

    if(m_param->b_nflv)
    {
        memcpy(m_mend_tagHeader->getData(),m_tagHeader->getData(),11);//拷贝元数据头信息
        {
            //tag 头信息 修改 待添加
        }
        fwrite(m_mend_tagHeader->getData(),1,11,g_flv_out); //写新的头信息
    }
    return true;
}

/* 分析前一个Tag Size */
void Anlysis::readPreTagSize()
{
    unsigned char size[4];

    fread(size,1,4,g_flv_file);

    unsigned int s = size[0]*M16E6 + size[1]*M16E4 + size[2]*M16E2 + size[3];

    if(s!=m_tagHeader->getSize())
    {
        fprintf(g_flv_tag,"前一TAG  长:    错误与前一TAG长度不一致：当前%5d TAG头数据%5d\n",s,m_tagHeader->getSize());g_errors++;
    }

    if(m_param->b_nflv)
    {
        fwrite(size,1,4,g_flv_out);
    }

}
/* 分析flv文件 */
void Anlysis::anlysis()
{
    author(m_param);
    anlysisFlvHeader();//分析flv头
    readFirstTagSize();//分析第一个TAG长度
    m_tagConter = 0;   //初始化为0
    while(1)
    {
        if(!anlysisFlvTagHeader())
        {
            break;
        }
        if(m_tagConter%100==0)
        {
            printf("%5d   Tags 分析完毕\n",m_tagConter);
        }

        //if(m_tagConter == 100)
        //{
        //    m_param->b_tag = 0;
        //}

        m_tagData.m_param = m_param;
        m_tagData.size = m_tagHeader->getSize()-11;             //获取Tag数据长度
        m_tagData.data = (unsigned char*)malloc(m_tagData.size);//申请Tag数据内存
        if(!m_tagData.data)
        {
            fprintf(g_flv_tag,"错误:m_tagData.data分配内存失败\n");
            break;
        }
        fread(m_tagData.data,1,m_tagData.size,g_flv_file);      //获取Tag数据

        if(m_param->b_tag)//分析tag
        {
            m_tagData.anlysisTag(); //分析具体Tag数据
        }

        if(m_param->b_nflv)
        {
            m_mend_tagData.m_param = m_param;
            m_mend_tagData.size = m_mend_tagHeader->getSize()-11;    //获取Tag数据长度
            m_mend_tagData.data = (unsigned char*)malloc(m_mend_tagData.size);//申请Tag数据内存
            if(!m_mend_tagData.data)
            {
                fprintf(g_flv_tag,"错误:m_mend_tagData.data分配内存失败\n");
                break;
            }
            memcpy(m_mend_tagData.data,m_tagData.data,m_mend_tagData.size);   //获取Tag数据
            {
                //修改待 添加
            }
            fwrite(m_mend_tagData.data,1,m_mend_tagData.size,g_flv_out); //写tag信息

            free(m_mend_tagData.data);//释放内存
        }

        free(m_tagData.data);//释放内存
        readPreTagSize();//分析前一个Tag Size
        m_tagConter++;   //计数Tag个数

        //break;        
    }
    printf("errors:    %d\n",g_errors);

    //FLV概要信息
    fprintf(g_flv_tag,"\n\n==================FLV 概要信息=========================\n");
    fprintf(g_flv_tag,"FLV Tag个数:    %d\n",m_tagConter);
    fprintf(g_flv_tag,"FLV错误个数:    %d\n",g_errors);
}