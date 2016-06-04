//����  �����  20160603  xiaoc@pku.edu.cn
#include "metadata.h"
#include "tag.h"
#include "flvfile.h"

double MetaData::parseDouble()//����Double  ����0
{
    union
    {  
        double  number;  
        unsigned char data[8];  
    }; 

    //���Ĵ洢��ʽ
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

char MetaData::parseChar()//����Char  ����1
{
    return m_tagData->data[m_count++];
}
void MetaData::parseString()//����String  ����2
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

void MetaData::SCRIPTDATAOBJECT() //����ECMA����  ����3
{
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"====SCRIPTDATAOBJECT����==========\n");

        while(m_count < m_tagData->size)
        {
            if(m_tagData->data[m_count] ==0 && m_tagData->data[m_count+1]==0 && m_tagData->data[m_count+2]==9 )
            {
                m_count +=3;
                break;
            }

            //��ȡstring name
            parseString();
            fprintf(g_flv_tag,"%s:  ",str);

            unsigned char type = m_tagData->data[m_count++]; //�����������
            switch(type)
            {
            case  0:fprintf(g_flv_tag,"%lf\n",parseDouble());break;//Double ����
            case  1:fprintf(g_flv_tag,"%d\n",parseChar());break;//Char ����
            case  2:parseString();fprintf(g_flv_tag,"%s\n",str);break;//String ����
            case  3:SCRIPTDATAOBJECT();break;// ����
            case  4:fprintf(g_flv_tag,"MovieClip (reserved, not supported)\n");break;
            case  5:fprintf(g_flv_tag,"NULL\n");break;
            case  6:fprintf(g_flv_tag,"Undefined\n");break;
            case  7:fprintf(g_flv_tag,"%d\n",parseReference());break;//Short ����
            case  8:parseEcma();break;//ECMA ����
            case  9:fprintf(g_flv_tag,"Object end marker\n");break;
            case 10:SCRIPTDATASTRICTARRAY();break;//��ͨ����
            case 11:fprintf(g_flv_tag,"%lf\n",SCRIPTDATADATE());break;//Date ����
            case 12:SCRIPTDATALONGSTRING();fprintf(g_flv_tag,"%s\n",str);break;//�� String ����
            default:fprintf(g_flv_tag,"SCRIPTDATAOBJECT������Ϣ:    Meata�޴���������\n");g_errors++;
            }
        }
        fprintf(g_flv_tag,"====SCRIPTDATAOBJECT����������==\n");
    }
}

int  MetaData::parseReference()  //Short  ����7
{
    int s = m_tagData->data[m_count]*M16E2 + m_tagData->data[m_count+1];
    return s;
}

void MetaData::parseEcma()//����ECMA����  ����8
{
    int metaDataLength = m_tagData->data[m_count]*M16E6 + m_tagData->data[m_count+1]*M16E4 + m_tagData->data[m_count+2]*M16E2 + m_tagData->data[m_count+3];
    m_count += 4;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"====ECMA����==========\n");
        fprintf(g_flv_tag,"���ݸ���:    %d\n",metaDataLength);

        for (int i = 0; i < metaDataLength; i++)
        {
            //��ȡstring name
            parseString();
            fprintf(g_flv_tag,"%s:  ",str);

            unsigned char type = m_tagData->data[m_count++]; //�����������
            switch(type)
            {
            case  0:fprintf(g_flv_tag,"%lf\n",parseDouble());break;//Double ����
            case  1:fprintf(g_flv_tag,"%d\n",parseChar());break;//Char ����
            case  2:parseString();fprintf(g_flv_tag,"%s\n",str);break;//String ����
            case  3:SCRIPTDATAOBJECT();break;// ����
            case  4:fprintf(g_flv_tag,"MovieClip (reserved, not supported)\n");break;
            case  5:fprintf(g_flv_tag,"NULL\n");break;
            case  6:fprintf(g_flv_tag,"Undefined\n");break;
            case  7:fprintf(g_flv_tag,"%d\n",parseReference());break;//Short ����
            case  8:parseEcma();break;//ECMA ����
            case  9:fprintf(g_flv_tag,"Object end marker\n");break;
            case 10:SCRIPTDATASTRICTARRAY();break;//��ͨ����
            case 11:fprintf(g_flv_tag,"%lf\n",SCRIPTDATADATE());break;//Date ����
            case 12:SCRIPTDATALONGSTRING();fprintf(g_flv_tag,"%s\n",str);break;//�� String ����
            default:fprintf(g_flv_tag,"ECMA������Ϣ:    Meata�޴���������\n");g_errors++;
            }
        }
        fprintf(g_flv_tag,"====ECMA����������==\n");
        SCRIPTDATAOBJECTEND();
    }
}

void MetaData::SCRIPTDATASTRICTARRAY()//��ͨ���� ����10
{
    int metaDataLength = m_tagData->data[m_count]*M16E6 + m_tagData->data[m_count+1]*M16E4 + m_tagData->data[m_count+2]*M16E2 + m_tagData->data[m_count+3];
    m_count += 4;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"====SCRIPTDATASTRICTARRAY����==========\n");
        fprintf(g_flv_tag,"���ݸ���:    %d\n",metaDataLength);

        for (int i = 0; i < metaDataLength; i++)
        {
            unsigned char type = m_tagData->data[m_count++]; //�����������
            switch(type)
            {
            case  0:fprintf(g_flv_tag,"%lf\n",parseDouble());break;//Double ����
            case  1:fprintf(g_flv_tag,"%d\n",parseChar());break;//Char ����
            case  2:parseString();fprintf(g_flv_tag,"%s\n",str);break;//String ����
            case  3:SCRIPTDATAOBJECT();break;// ����
            case  4:fprintf(g_flv_tag,"MovieClip (reserved, not supported)\n");break;
            case  5:fprintf(g_flv_tag,"NULL\n");break;
            case  6:fprintf(g_flv_tag,"Undefined\n");break;
            case  7:fprintf(g_flv_tag,"%d\n",parseReference());break;//Short ����
            case  8:parseEcma();break;//ECMA ����
            case  9:fprintf(g_flv_tag,"Object end marker\n");break;
            case 10:SCRIPTDATASTRICTARRAY();break;//��ͨ����
            case 11:fprintf(g_flv_tag,"%lf\n",SCRIPTDATADATE());break;//Date ����
            case 12:SCRIPTDATALONGSTRING();fprintf(g_flv_tag,"%s\n",str);break;//�� String ����
            default:fprintf(g_flv_tag,"ECMA������Ϣ:    Meata�޴���������\n");g_errors++;
            }
        }
        fprintf(g_flv_tag,"====SCRIPTDATASTRICTARRAY����������==\n");
    }
}

double MetaData::SCRIPTDATADATE() //ʱ�� ����11 ��1970.1.1�����ڵ�����
{
    union
    {  
        double  number;  
        unsigned char data[8];  
    }; 

    //���Ĵ洢��ʽ
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

    //���Ĵ洢��ʽ
    d[0] = m_tagData->data[m_count+1];
    d[1] = m_tagData->data[m_count+0];

    m_count += 2;
    return number + offset;
}

void MetaData::SCRIPTDATALONGSTRING()//����String  ����12
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
        if(m_tagData->data[m_count]!=0||m_tagData->data[m_count+1]!=0||m_tagData->data[m_count+2]!=9) //������ 0 0 9
        {
            fprintf(g_flv_tag,"�ű����ݽ���������ȷ\n");g_errors++;
        }
    }
    m_count += 3;
}
void MetaData::anlysisMetaData(TagData*  tagData)
{
    m_tagData = tagData;
    if(m_tagData->m_param->b_tag)
    {
        fprintf(g_flv_tag,"++++++++++++++++++�ű�����++++++++++++++++++\n");

        for(m_count =0; m_count < tagData->size;)
        {
            unsigned char type = tagData->data[m_count++]; //�����������
            switch(type)
            {
            case  0:fprintf(g_flv_tag,"%lf\n",parseDouble());break;//Double ����
            case  1:fprintf(g_flv_tag,"%d\n",parseChar());break;//Char ����
            case  2:parseString();fprintf(g_flv_tag,"%s\n",str);break;//String ����
            case  3:SCRIPTDATAOBJECT();break;// ����
            case  4:fprintf(g_flv_tag,"MovieClip (reserved, not supported)\n");break;
            case  5:fprintf(g_flv_tag,"NULL\n");break;
            case  6:fprintf(g_flv_tag,"Undefined\n");break;
            case  7:fprintf(g_flv_tag,"%d\n",parseReference());break;//Short ����
            case  8:parseEcma();break;//ECMA ����
            case  9:fprintf(g_flv_tag,"Object end marker\n");break;
            case 10:SCRIPTDATASTRICTARRAY();break;//��ͨ����
            case 11:fprintf(g_flv_tag,"%lf\n",SCRIPTDATADATE());break;//Date ����
            case 12:SCRIPTDATALONGSTRING();fprintf(g_flv_tag,"%s\n",str);break;//�� String ����
            default:fprintf(g_flv_tag,"TAG������Ϣ:    Meata�޴���������\n");g_errors++;
            }
        }
        if(m_count != tagData->size)
        {
            fprintf(g_flv_tag,"TAG������Ϣ:    Meataδ�������\n");g_errors++;
        }
    }    
}