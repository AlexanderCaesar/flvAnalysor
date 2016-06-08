#ifndef XIAOC_AUDIO_H
#define XIAOC_AUDIO_H
//����  �����  20160608  xiaoc@pku.edu.cn

class TagData;

class Audio
{
public:
    TagData*     m_tagData;//ָ��ǰ��Tag�ļ�
    unsigned int m_count;  //ָʾ��ǰ���ݶ�ȡ��λ��

    /*
    0 ��ʾ Linear PCM, platform endian
    1 ��ʾ ADPCM
    2 ��ʾ MP3
    3 ��ʾ Linear PCM, little endian
    4 ��ʾ Nellymoser 16 kHz mono
    5 ��ʾ Nellymoser 8 kHz mono
    6 ��ʾ Nellymoser
    7 ��ʾ G.711 A-law logarithmic PCM
    8 ��ʾ G.711 mu-law logarithmic PCM
    9 ��ʾ �����ֶ� δ��ʹ��
    10��ʾ AAC
    11��ʾ Speex
    12��ʾ δ����
    13��ʾ δ����
    14��ʾ MP3 8 kHz
    15��ʾDevice-specific sound    **/
    unsigned char m_soundFormat;//ռ4λ  ��һ���ֽڵ� 0~3λ ��Ƶ���ݸ�ʽ ����7 8 14 15Ϊ�����ֶ� aac��Flash Player9�汾����֧�� speex��Flash Player10�汾����֧��
    unsigned char m_soundRate;  //ռ2λ  ��һ���ֽڵģ�4~5λ����ʾ��Ƶ������ 0 = 5.5 kHz��1 = 11 kHz��2 = 22 kHz��3 = 44 kHz
    unsigned char m_soundSize;  //ռ1λ ��һ���ֽڵĵ�6λ ��ʾ�������ȣ�0 = 8bits��1 = 16bits��
    unsigned char m_SoundType;  //ռ1λ��һ���ֽڵĵ�7λ ��ʾ��Ƶ���ͣ�0 = mono��������1 = stereo����������
    unsigned char m_AACPacketType;//ռ8λ��ֻ��aac����½������������� SoundFormat ==10ʱ��0 aacͷ��Ϣ  1 aac ԭʼ����

    void   anlysisAudio(TagData*  tagData);//������Ƶ����

};


#endif