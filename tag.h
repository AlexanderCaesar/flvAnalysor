#ifndef XIAOC_TAG_H
#define XIAOC_TAG_H
//作者  蔡砚刚  20160601  xiaoc@pku.edu.cn

struct FlvHeader
{
	union
	{
		struct
		{
			unsigned char signature[3]; //分别是ASCII码表示的 F L V
			unsigned char version;      //版本用一个字节表示：一般为0x01
			unsigned char present;      //4表示只有音频 1表示只有视频 5 表示既有视频也有音频 其他值非法
			unsigned char size[4];      //为FLV Header的长度，为固定值0x00000009  在标准中规定，版本1一定为9，在未来版本中可能会修正
		};
		unsigned char data[9];
	};
};

#endif