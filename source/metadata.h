#ifndef XIAOC_METADATA_H
#define XIAOC_METADATA_H
//����  �����  20160603  xiaoc@pku.edu.cn

/*
AMF(Action Message Format)��Flash������ͨ�ŵ�һ�ֳ����Ķ����Ʊ���ģʽ���䴫��Ч�ʸߣ�������HTTP�����ϴ��䡣���ںܶ�Flash WebGame��������������Ϣ��ʽ��
AMFЭ���ǻ���HttpЭ���.
�������ݴ�����̴���������:
1.�ӿͻ��˻�ȡHttp����(Request)��.
2.�������н⴮�л�(Deserialize)���õ��������˳����ܹ�ʶ������ݣ�������һ����Ӧ(Response)��Ϣ
3.Debug��ʼ
4. �������и��ִ���(��¼����ɡ�����)�õ�����ֵ
5.����Ӧ�����д��л�
6.����Http��Ӧ���ͻ��˱�׼�ĵ���

������Tag��ͨ������ΪMetadata Tag�����һЩ����FLV��Ƶ����Ƶ�Ĳ�����Ϣ����duration��width��height�ȡ�ͨ��������Tag�����File Header������Ϊ��һ��Tag���֣�����ֻ��һ����(��tag��Ϣһ����Ҫ��������Լ������������ֵ��ϵ�����û��Լ���ã���������ֱ�Ӷ���)

����������صĹ��߾��ǣ�yamdi

yamdiΪflv�ļ������˺ܶ�metadata��Ϣ�����紴���ߡ��Ƿ��йؼ�֡���Ƿ�����Ƶ���Ƿ�����Ƶ����Ƶ�߶ȺͿ�ȵȵȡ���yamdi�����meta�����У�����Ч��Ҫ���ؼ�֡����ע���˹ؼ�֡��flv����ʵ�������������ſ����ȴ�����Ƶ��վһ���ġ��Ͻ��ȡ�����ǰ�ϵ����廹δ���ص���λ�ÿ�ʼ���š�

һ����˵����Tag Data�ṹ��������AMF����AMF��Action Message Format����Adobe��Ƶ�һ��ͨ�����ݷ�װ��ʽ����Adobe�ĺܶ��Ʒ��Ӧ�ã�����˵��AMF����ͬ���͵�������ͳһ�ĸ�ʽ����������һ��AMF����װ�ַ����������ݣ�����װ��һ����onMetaData����־�������־��Adobe��һЩAPI�����У��ڴ˲�ϸ�����ڶ���AMF����װһ���������ͣ���������а���������Ƶ��Ϣ������ƺ�ֵ������˵�����£���ҿ��Բ���ͼƬ�ϵ����ݽ�����⡣

��һ��AMF����
��1���ֽڱ�ʾAMF�����ͣ�һ������0x02����ʾ�ַ���������ֵ��ʾ����������ĵ���
��2-3���ֽ�ΪUI16����ֵ����ʾ�ַ����ĳ��ȣ�һ������0x000A����onMetaData�����ȣ���
�����ֽ�Ϊ�ַ������ݣ�һ����Ϊ��onMetaData����

�ڶ���AMF����
��1���ֽڱ�ʾAMF�����ͣ�һ������0x08����ʾ���顣
��2-5���ֽ�ΪUI32����ֵ����ʾ����Ԫ�صĸ�����
���漴Ϊ������Ԫ�صķ�װ������Ԫ��ΪԪ�����ƺ�ֵ��ɵĶԡ���ʾ�������£�
��1-2���ֽڱ�ʾԪ�����Ƶĳ��ȣ�����ΪL��
�������Ϊ����ΪL���ַ�����
��L+3���ֽڱ�ʾԪ��ֵ�����͡�
�������Ϊ��Ӧֵ��ռ���ֽ���ȡ����ֵ������
**/

class TagData;

class MetaData
{
public:
    TagData*     m_tagData;//ָ��ǰ��Tag�ļ�
    unsigned int m_count;  //ָʾ��ǰ���ݶ�ȡ��λ��

    char      str[1000];//������ʱ�洢�ַ���

    double parseDouble();//����Double  ����0
    char   parseChar();  //����Char    ����1
    void   parseString();//����String  ����2
    void   SCRIPTDATAOBJECT();//����   ����3

    int    parseReference();  //Short  ����7 
    void   parseEcma(); //����ECMA���� ����8

    void   SCRIPTDATASTRICTARRAY();//��ͨ���� ����10
    double SCRIPTDATADATE(); //ʱ�� ����11 ��1970.1.1�����ڵ�����
    void   SCRIPTDATALONGSTRING();//����String  ����12

    void   SCRIPTDATAOBJECTEND();

    void   anlysisMetaData(TagData*  tagData);//�����ű����� 

    

    //SCRIPTDATAOBJECTEND

};

#endif