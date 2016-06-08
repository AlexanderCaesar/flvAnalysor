//作者  蔡砚刚  20160603  xiaoc@pku.edu.cn
#include "metadata.h"
#include "tag.h"
#include "flvfile.h"

double MetaData::parseDouble()//分析Double  类型0
{
    union
    {  
        double  number;  
        unsigned char data[8];  
    }; 

    //更改存储方式
    data[0] = m_tagData->data[m_count+7];
    data[1] = m_tagData->data[m_count+6];
    data[2] = m_tagData->data[m_count+5];
    data[3] = m_tagData->data[m_count+4];
    data[4] = m_tagData->data[m_count+3];
    data[5] = m_tagData->data[m_count+2];
    data[6] = m_tagData->data[m_count+1];
    data[7] = m_tagData->data[m_count+0];

    m_count += 8;
    return number;
}

char MetaData::parseChar()//分析Char  类型1
{
    return m_tagData->data[m_count++];
}
void MetaData::parseString()//分析String  类型2
{
    int length = m_tagData->data[m_count]*M16E2 + m_tagData->data[m_count+1];
    m_count += 2;

    if(m_tagData->m_param->b_tag)
    {
        for(int i =0; i<length;i++)
        {
            sprintf(str+i,"%c",m_tagData->data[m_count+i]);
        }
    }
    m_count += length;
}

void MetaData::SCRIPTDATAOBJECT() //分析ECMA数组  类型3
{
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"====SCRIPTDATAOBJECT数组==========\n");

        while(m_count < m_tagData->size)
        {
            if(m_tagData->data[m_count] ==0 && m_tagData->data[m_count+1]==0 && m_tagData->data[m_count+2]==9 )
            {
                m_count +=3;
                break;
            }

            //读取string name
            parseString();
            fprintf(g_flv_tag,"%s:  ",str);

            unsigned char type = m_tagData->data[m_count++]; //存放数据类型
            switch(type)
            {
            case  0:fprintf(g_flv_tag,"%lf\n",parseDouble());break;//Double 类型
            case  1:fprintf(g_flv_tag,"%d\n",parseChar());break;//Char 类型
            case  2:parseString();fprintf(g_flv_tag,"%s\n",str);break;//String 类型
            case  3:SCRIPTDATAOBJECT();break;// 数组
            case  4:fprintf(g_flv_tag,"MovieClip (reserved, not supported)\n");break;
            case  5:fprintf(g_flv_tag,"NULL\n");break;
            case  6:fprintf(g_flv_tag,"Undefined\n");break;
            case  7:fprintf(g_flv_tag,"%d\n",parseReference());break;//Short 类型
            case  8:parseEcma();break;//ECMA 数组
            case  9:fprintf(g_flv_tag,"Object end marker\n");break;
            case 10:SCRIPTDATASTRICTARRAY();break;//普通数组
            case 11:fprintf(g_flv_tag,"%lf\n",SCRIPTDATADATE());break;//Date 类型
            case 12:SCRIPTDATALONGSTRING();fprintf(g_flv_tag,"%s\n",str);break;//长 String 类型
            default:fprintf(g_flv_tag,"SCRIPTDATAOBJECT数组信息:    Meata无此数据类型\n");g_errors++;
            }
        }
        fprintf(g_flv_tag,"====SCRIPTDATAOBJECT数组分析完毕==\n");
    }
}

int  MetaData::parseReference()  //Short  类型7
{
    int s = m_tagData->data[m_count]*M16E2 + m_tagData->data[m_count+1];
    return s;
}

void MetaData::parseEcma()//分析ECMA数组  类型8
{
    int metaDataLength = m_tagData->data[m_count]*M16E6 + m_tagData->data[m_count+1]*M16E4 + m_tagData->data[m_count+2]*M16E2 + m_tagData->data[m_count+3];
    m_count += 4;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"====ECMA数组==========\n");
        fprintf(g_flv_tag,"数据个数:    %d\n",metaDataLength);

        for (int i = 0; i < metaDataLength; i++)
        {
            //读取string name
            parseString();
            fprintf(g_flv_tag,"%s:  ",str);

            unsigned char type = m_tagData->data[m_count++]; //存放数据类型
            switch(type)
            {
            case  0:fprintf(g_flv_tag,"%lf\n",parseDouble());break;//Double 类型
            case  1:fprintf(g_flv_tag,"%d\n",parseChar());break;//Char 类型
            case  2:parseString();fprintf(g_flv_tag,"%s\n",str);break;//String 类型
            case  3:SCRIPTDATAOBJECT();break;// 数组
            case  4:fprintf(g_flv_tag,"MovieClip (reserved, not supported)\n");break;
            case  5:fprintf(g_flv_tag,"NULL\n");break;
            case  6:fprintf(g_flv_tag,"Undefined\n");break;
            case  7:fprintf(g_flv_tag,"%d\n",parseReference());break;//Short 类型
            case  8:parseEcma();break;//ECMA 数组
            case  9:fprintf(g_flv_tag,"Object end marker\n");break;
            case 10:SCRIPTDATASTRICTARRAY();break;//普通数组
            case 11:fprintf(g_flv_tag,"%lf\n",SCRIPTDATADATE());break;//Date 类型
            case 12:SCRIPTDATALONGSTRING();fprintf(g_flv_tag,"%s\n",str);break;//长 String 类型
            default:fprintf(g_flv_tag,"ECMA数组信息:    Meata无此数据类型\n");g_errors++;
            }
        }
        fprintf(g_flv_tag,"====ECMA数组分析完毕==\n");
        SCRIPTDATAOBJECTEND();
    }
}

void MetaData::SCRIPTDATASTRICTARRAY()//普通数组 类型10
{
    int metaDataLength = m_tagData->data[m_count]*M16E6 + m_tagData->data[m_count+1]*M16E4 + m_tagData->data[m_count+2]*M16E2 + m_tagData->data[m_count+3];
    m_count += 4;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"====SCRIPTDATASTRICTARRAY数组==========\n");
        fprintf(g_flv_tag,"数据个数:    %d\n",metaDataLength);

        for (int i = 0; i < metaDataLength; i++)
        {
            unsigned char type = m_tagData->data[m_count++]; //存放数据类型
            switch(type)
            {
            case  0:fprintf(g_flv_tag,"%lf\n",parseDouble());break;//Double 类型
            case  1:fprintf(g_flv_tag,"%d\n",parseChar());break;//Char 类型
            case  2:parseString();fprintf(g_flv_tag,"%s\n",str);break;//String 类型
            case  3:SCRIPTDATAOBJECT();break;// 数组
            case  4:fprintf(g_flv_tag,"MovieClip (reserved, not supported)\n");break;
            case  5:fprintf(g_flv_tag,"NULL\n");break;
            case  6:fprintf(g_flv_tag,"Undefined\n");break;
            case  7:fprintf(g_flv_tag,"%d\n",parseReference());break;//Short 类型
            case  8:parseEcma();break;//ECMA 数组
            case  9:fprintf(g_flv_tag,"Object end marker\n");break;
            case 10:SCRIPTDATASTRICTARRAY();break;//普通数组
            case 11:fprintf(g_flv_tag,"%lf\n",SCRIPTDATADATE());break;//Date 类型
            case 12:SCRIPTDATALONGSTRING();fprintf(g_flv_tag,"%s\n",str);break;//长 String 类型
            default:fprintf(g_flv_tag,"ECMA数组信息:    Meata无此数据类型\n");g_errors++;
            }
        }
        fprintf(g_flv_tag,"====SCRIPTDATASTRICTARRAY数组分析完毕==\n");
    }
}

double MetaData::SCRIPTDATADATE() //时间 类型11 从1970.1.1到现在的秒数
{
    union
    {  
        double  number;  
        unsigned char data[8];  
    }; 

    //更改存储方式
    data[0] = m_tagData->data[m_count+7];
    data[1] = m_tagData->data[m_count+6];
    data[2] = m_tagData->data[m_count+5];
    data[3] = m_tagData->data[m_count+4];
    data[4] = m_tagData->data[m_count+3];
    data[5] = m_tagData->data[m_count+2];
    data[6] = m_tagData->data[m_count+1];
    data[7] = m_tagData->data[m_count+0];

    m_count += 8;

    union
    {
        short         offset;
        unsigned char d[2];
    };

    //更改存储方式
    d[0] = m_tagData->data[m_count+1];
    d[1] = m_tagData->data[m_count+0];

    m_count += 2;
    return number + offset;
}

void MetaData::SCRIPTDATALONGSTRING()//分析String  类型12
{
    int length = m_tagData->data[m_count]*M16E6 + m_tagData->data[m_count+1]*M16E4 + m_tagData->data[m_count+2]*M16E2 + m_tagData->data[m_count+3];
    m_count += 4;

    if(m_tagData->m_param->b_tag)
    {
        for(int i =0; i<length;i++)
        {
            sprintf(str+i,"%c",m_tagData->data[m_count+i]);
        }
    }
    m_count += length;
}

void MetaData::SCRIPTDATAOBJECTEND()
{
    if(m_tagData->m_param->b_tag)
    {
        if(m_tagData->data[m_count]!=0||m_tagData->data[m_count+1]!=0||m_tagData->data[m_count+2]!=9) //结束符 0 0 9
        {
            fprintf(g_flv_tag,"脚本数据结束符不正确\n");g_errors++;
        }
    }
    m_count += 3;
}
void MetaData::anlysisMetaData(TagData*  tagData)
{
    m_tagData = tagData;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"++++++++++++++++++脚本数据++++++++++++++++++\n");

        for(m_count =0; m_count < tagData->size;)
        {
            unsigned char type = tagData->data[m_count++]; //存放数据类型
            switch(type)
            {
            case  0:fprintf(g_flv_tag,"%lf\n",parseDouble());break;//Double 类型
            case  1:fprintf(g_flv_tag,"%d\n",parseChar());break;//Char 类型
            case  2:parseString();fprintf(g_flv_tag,"%s\n",str);break;//String 类型
            case  3:SCRIPTDATAOBJECT();break;// 数组
            case  4:fprintf(g_flv_tag,"MovieClip (reserved, not supported)\n");break;
            case  5:fprintf(g_flv_tag,"NULL\n");break;
            case  6:fprintf(g_flv_tag,"Undefined\n");break;
            case  7:fprintf(g_flv_tag,"%d\n",parseReference());break;//Short 类型
            case  8:parseEcma();break;//ECMA 数组
            case  9:fprintf(g_flv_tag,"Object end marker\n");break;
            case 10:SCRIPTDATASTRICTARRAY();break;//普通数组
            case 11:fprintf(g_flv_tag,"%lf\n",SCRIPTDATADATE());break;//Date 类型
            case 12:SCRIPTDATALONGSTRING();fprintf(g_flv_tag,"%s\n",str);break;//长 String 类型
            default:fprintf(g_flv_tag,"TAG错误信息:    Meata无此数据类型\n");g_errors++;
            }
        }
        if(m_count != tagData->size)
        {
            fprintf(g_flv_tag,"TAG错误信息:    Meata未分析完毕\n");g_errors++;
        }
    }    
}