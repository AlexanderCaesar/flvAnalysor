//����  �����  20160601  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "flv.h"
#include "flvfile.h"
#include "analysis.h"
int main(int argc, char **argv)
{
    flv_param m_param; //���ò�����Ϣ
    bool ret = false;

    Anlysis *anlysis = new Anlysis();

    flv_param_default(&m_param);//����Ĭ�ϲ���
    ret = parse(&m_param,argc,argv);  //��������
    if(!ret) //�������ô���
    {
        system("pause");
        return 0;
    }

    openFlvFiles();//����ض�д�ļ�

    anlysis->create(&m_param); //�����ڴ�
    anlysis->anlysis();//����flv�ļ�
    anlysis->destory();//�ͷ��ڴ�

    delete anlysis;
    closeFlvFiles();//�ر���ض�д�ļ�
    system("pause");
    return 0;
}