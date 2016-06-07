#ifndef XIAOC_VIDEO_H
#define XIAOC_VIDEO_H
//����  �����  20160604  xiaoc@pku.edu.cn

class TagData;

class Video
{
public:
    TagData*     m_tagData;//ָ��ǰ��Tag�ļ�
    unsigned int m_count;  //ָʾ��ǰ���ݶ�ȡ��λ��
    char         str[1000];//������ʱ�洢�ַ���

    /*
    1 �ؼ�֡ ��Ӧ����AVC��
    2 �ǹؼ�֡��Ӧ����AVC��
    3 h263�ķǹؼ�֡
    4 ���������ɹؼ�֡
    5 ��Ƶ��Ϣ������֡   **/
    int          m_frameType;//֡����

    /*
    2 H.263
    3 ��Ļ��Ƶ
    4 VP6
    5 VP6 ������alphaͨ��
    6 ��Ļ��Ƶ�汾2
    7 AVC��H.264��    **/
    int          m_codecID; //����������

    /*
    ����AVCʱ�д��ֶ�
    0  AVC sequence header (SPS��PPS��Ϣ��)
    1 AVC NALU
    2 AVC end of sequence (lower level NALU sequence ender is
    not required or supported)**/
    int          m_AVCPacketType; //AVC������

    /*
    ���ʱ��� ���AVCPacketType=0x01�� Ϊ���ʱ�����
    ��������Ϊ0��  
    ��ֵ��ʾ��ǰ֡����ʾʱ��  tag��ʱ��Ϊ����ʱ��    
    ��ʾʱ����� ����ʱ�� + CompositionTime    **/
    int          m_CompositionTime; //��ʾʱ��ƫ��

    void   printfTimeStamp(char *p, int timestamp);//��ӡʱ���

    void   AVCDecoderConfigurationRecord();//H264 SPS PPS ����Ϣ
    void   decodeH264();                           //H264����
    void   anlysisVideo(TagData*  tagData);//������Ƶ����

};

#endif