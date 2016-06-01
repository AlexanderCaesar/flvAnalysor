#ifndef XIAOC_TAG_H
#define XIAOC_TAG_H
//����  �����  20160601  xiaoc@pku.edu.cn

struct FlvHeader
{
	union
	{
		struct
		{
			unsigned char signature[3]; //�ֱ���ASCII���ʾ�� F L V
			unsigned char version;      //�汾��һ���ֽڱ�ʾ��һ��Ϊ0x01
			unsigned char present;      //4��ʾֻ����Ƶ 1��ʾֻ����Ƶ 5 ��ʾ������ƵҲ����Ƶ ����ֵ�Ƿ�
			unsigned char size[4];      //ΪFLV Header�ĳ��ȣ�Ϊ�̶�ֵ0x00000009  �ڱ�׼�й涨���汾1һ��Ϊ9����δ���汾�п��ܻ�����
		};
		unsigned char data[9];
	};
};

#endif