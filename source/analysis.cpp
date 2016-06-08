//����  �����  20160601  xiaoc@pku.edu.cn
#include <stdlib.h>
#include <string.h>
#include "analysis.h"
void author(flv_param *param)
{
    if(!param->b_tag) return;
    fprintf(g_flv_tag,"==================���� ��Ϣ=========================\n");
    fprintf(g_flv_tag,"��       ��:    xiaoC/cabbage/Alexander/Caesar\n");
    fprintf(g_flv_tag,"��       ��:    xiaoc@pku.edu.cn\n");
    fprintf(g_flv_tag,"��       ��:    http://blog.csdn.net/cabbage2008\n");
}
Anlysis::Anlysis()
{
    m_param          = NULL;
    m_flvheader      = NULL;
    m_mend_flvheader = NULL;
    m_tagHeader      = NULL;
    m_mend_tagHeader = NULL;
    
};
/* �����ڴ� */
void Anlysis::create(flv_param *param)
{
    bool ok = true;
    m_param = param;
    m_flvheader = (FlvHeader*)malloc(sizeof(FlvHeader)); //����flvͷ�ڴ�
    m_tagHeader = (TagHeader*)malloc(sizeof(TagHeader)); //����tagͷ�ڴ�
    ok = ok&&m_flvheader&&m_tagHeader;
    m_tagData.m_tagHeader = m_tagHeader;

    if(m_param->b_nflv)//�����Ҫ����µ�flv�ļ�
    {
        m_mend_flvheader = (FlvHeader*)malloc(sizeof(FlvHeader)); //������flvͷ�ڴ�
        m_mend_tagHeader = (TagHeader*)malloc(sizeof(TagHeader)); //������tagͷ�ڴ�
        ok = ok&&m_mend_flvheader&&m_mend_tagHeader;

        m_mend_tagData.m_tagHeader = m_mend_tagHeader;
    }

    if(!ok)
    {
        printf("Anlysis�������ڴ�ʧ��\n");
        system("pause");
        exit(0);
    }
}
/* �ͷ��ڴ� */
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

/* ����flv�ļ�ͷ */
void Anlysis::anlysisFlvHeader()
{
    fread(m_flvheader->getData(),1,9,g_flv_file); //��ȡͷ��Ϣ

    if(m_param->b_tag)
    {
        fprintf(g_flv_tag,"==================FLV ͷ��Ϣ=========================\n");
        fprintf(g_flv_tag,"FLV����ǩ��:    %c%c%c\n",m_flvheader->getSignature()[0],m_flvheader->getSignature()[1],m_flvheader->getSignature()[2]);
        fprintf(g_flv_tag,"FLV�汾��Ϣ:    %d\n",m_flvheader->getVersion());
        switch(m_flvheader->getPresent())
        {
        case 1:fprintf(g_flv_tag,"FLV������Ϣ:    ֻӵ����Ƶ��\n");break;
        case 4:fprintf(g_flv_tag,"FLV������Ϣ:    ֻӵ����Ƶ��\n");break;
        case 5:fprintf(g_flv_tag,"FLV������Ϣ:    ӵ����Ƶ����Ƶ��\n");break;
        default:fprintf(g_flv_tag,"FLV������Ϣ:   flvͷ��Ϣ����\n");g_errors++;
        }
        fprintf(g_flv_tag,"FLV  ͷ����:    %d\n",m_flvheader->getSize());
    }
        
    if(m_param->b_nflv)
    {
        memcpy(m_mend_flvheader->getData(),m_flvheader->getData(),9);//����Ԫ����ͷ��Ϣ
        {
            //flv ͷ��Ϣ �޸� �����
        }
        fwrite(m_mend_flvheader->getData(),1,9,g_flv_out); //д�µ�ͷ��Ϣ
    }
}

/* ������һ��Tag Size */
void Anlysis::readFirstTagSize()
{
    unsigned char data[4];

    fread(data,1,4,g_flv_file);

    if(data[0]!= 0||data[1]!= 0||data[2]!= 0 ||data[3]!= 0 )
    {
        fprintf(g_flv_tag,"��һTAG  ��:    ���󣺵�һ��TAG���ȱ���Ϊ0��%X%X%X%X\n",data[0],data[1],data[2],data[3]);g_errors++;
    }

    if(m_param->b_nflv)
    {
        fwrite(data,1,4,g_flv_out);
    }
}

/* ����tagͷ */
bool Anlysis::anlysisFlvTagHeader()
{
    if(!fread(m_tagHeader->getData(),1,11,g_flv_file)) //��ȡͷ��Ϣ
    {
        return false;
    }

    if(m_param->b_tag)
    {
        fprintf(g_flv_tag,"==================TAG %5d ͷ��Ϣ=========================\n",m_tagConter);
        switch(m_tagHeader->getTagType())
        {
        case 8:fprintf(g_flv_tag,"TAG������Ϣ:    ��ƵTAG TAG���˲�\n");break;
        case 9:fprintf(g_flv_tag,"TAG������Ϣ:    ��ƵTAG TAG���˲�\n");break;
        case 18:fprintf(g_flv_tag,"TAG������Ϣ:    �ű�TAG TAG���˲�\n");break;
        case 40:fprintf(g_flv_tag,"TAG������Ϣ:    ��ƵTAG TAG���˲�\n");break;
        case 41:fprintf(g_flv_tag,"TAG������Ϣ:    ��ƵTAG TAG���˲�\n");break;
        case 50:fprintf(g_flv_tag,"TAG������Ϣ:    �ű�TAG TAG���˲�\n");break;
        default:fprintf(g_flv_tag,"TAG������Ϣ:    TAG���ʹ������鵱ǰTAG��ͷ��Ϣ\n");g_errors++;return false;
        }
        fprintf(g_flv_tag,"TAG    ����:    %d\n",m_tagHeader->getSize());
        char timestamp[100];
        m_tagHeader->printfTimeStamp(timestamp);
        fprintf(g_flv_tag,"����ʱ��� :    %d(%s) \n",m_tagHeader->getTimeStamp(),timestamp);
        if(m_tagHeader->getTimeStamp()==0)
        {
            fprintf(g_flv_tag,"ע�⣺��һ��Tag��ʱ���Ϊ0��Ҳ���Խ�����Tag��ʱ���ȫ����Ϊ0�����������Զ����� ÿ��tag�ĵ�����ֽڿ�ʼ\n");
        }
        fprintf(g_flv_tag,"TAG    ��ID:    %d һ��ֵ�̶�Ϊ0\n",m_tagHeader->getStreamID());
        if(m_tagHeader->getStreamID())
        {
            fprintf(g_flv_tag,"TAG  ����Ϣ:    TAG����Ϣ����һ��ֵ�̶�Ϊ0\n");g_errors++;
        }
    }

    if(m_param->b_nflv)
    {
        memcpy(m_mend_tagHeader->getData(),m_tagHeader->getData(),11);//����Ԫ����ͷ��Ϣ
        {
            //tag ͷ��Ϣ �޸� �����
        }
        fwrite(m_mend_tagHeader->getData(),1,11,g_flv_out); //д�µ�ͷ��Ϣ
    }
    return true;
}

/* ����ǰһ��Tag Size */
void Anlysis::readPreTagSize()
{
    unsigned char size[4];

    fread(size,1,4,g_flv_file);

    unsigned int s = size[0]*M16E6 + size[1]*M16E4 + size[2]*M16E2 + size[3];

    if(s!=m_tagHeader->getSize())
    {
        fprintf(g_flv_tag,"ǰһTAG  ��:    ������ǰһTAG���Ȳ�һ�£���ǰ%5d TAGͷ����%5d\n",s,m_tagHeader->getSize());g_errors++;
    }

    if(m_param->b_nflv)
    {
        fwrite(size,1,4,g_flv_out);
    }

}
/* ����flv�ļ� */
void Anlysis::anlysis()
{
    author(m_param);
    anlysisFlvHeader();//����flvͷ
    readFirstTagSize();//������һ��TAG����
    m_tagConter = 0;   //��ʼ��Ϊ0
    while(1)
    {
        if(!anlysisFlvTagHeader())
        {
            break;
        }
        if(m_tagConter%100==0)
        {
            printf("%5d   Tags �������\n",m_tagConter);
        }

        //if(m_tagConter == 100)
        //{
        //    m_param->b_tag = 0;
        //}

        m_tagData.m_param = m_param;
        m_tagData.size = m_tagHeader->getSize()-11;             //��ȡTag���ݳ���
        m_tagData.data = (unsigned char*)malloc(m_tagData.size);//����Tag�����ڴ�
        if(!m_tagData.data)
        {
            fprintf(g_flv_tag,"����:m_tagData.data�����ڴ�ʧ��\n");
            break;
        }
        fread(m_tagData.data,1,m_tagData.size,g_flv_file);      //��ȡTag����

        if(m_param->b_tag)//����tag
        {
            m_tagData.anlysisTag(); //��������Tag����
        }

        if(m_param->b_nflv)
        {
            m_mend_tagData.m_param = m_param;
            m_mend_tagData.size = m_mend_tagHeader->getSize()-11;    //��ȡTag���ݳ���
            m_mend_tagData.data = (unsigned char*)malloc(m_mend_tagData.size);//����Tag�����ڴ�
            if(!m_mend_tagData.data)
            {
                fprintf(g_flv_tag,"����:m_mend_tagData.data�����ڴ�ʧ��\n");
                break;
            }
            memcpy(m_mend_tagData.data,m_tagData.data,m_mend_tagData.size);   //��ȡTag����
            {
                //�޸Ĵ� ���
            }
            fwrite(m_mend_tagData.data,1,m_mend_tagData.size,g_flv_out); //дtag��Ϣ

            free(m_mend_tagData.data);//�ͷ��ڴ�
        }

        free(m_tagData.data);//�ͷ��ڴ�
        readPreTagSize();//����ǰһ��Tag Size
        m_tagConter++;   //����Tag����

        //break;        
    }
    printf("errors:    %d\n",g_errors);

    //FLV��Ҫ��Ϣ
    fprintf(g_flv_tag,"\n\n==================FLV ��Ҫ��Ϣ=========================\n");
    fprintf(g_flv_tag,"FLV Tag����:    %d\n",m_tagConter);
    fprintf(g_flv_tag,"FLV�������:    %d\n",g_errors);
}