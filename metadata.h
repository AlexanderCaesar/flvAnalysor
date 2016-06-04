#ifndef XIAOC_METADATA_H
#define XIAOC_METADATA_H
//作者  蔡砚刚  20160603  xiaoc@pku.edu.cn

/*
AMF(Action Message Format)是Flash与服务端通信的一种常见的二进制编码模式，其传输效率高，可以在HTTP层面上传输。现在很多Flash WebGame都采用这样的消息格式。
AMF协议是基于Http协议的.
它的内容处理过程大致是这样:
1.从客户端获取Http请求(Request)流.
2.对流进行解串行化(Deserialize)，得到服务器端程序能够识别的数据，并建立一个响应(Response)消息
3.Debug开始
4. 对流进行各种处理(记录、许可、服务)得到返回值
5.对响应流进行串行化
6.发送Http响应给客户端标准文档：

该类型Tag又通常被称为Metadata Tag，会放一些关于FLV视频和音频的参数信息，如duration、width、height等。通常该类型Tag会跟在File Header后面作为第一个Tag出现，而且只有一个。(该tag信息一般需要跟播放器约定好名称与数值关系，如果没有约定好，播放器会直接丢弃)

其中与其相关的工具就是：yamdi

yamdi为flv文件增加了很多metadata信息，比如创建者、是否有关键帧、是否有视频、是否有音频，视频高度和宽度等等。而yamdi加入的meta数据中，最有效的要数关键帧。被注入了关键帧的flv可以实现像土豆网、优酷网等大型视频网站一样的“拖进度”，提前拖到缓冲还未加载到的位置开始播放。

一般来说，该Tag Data结构包含两个AMF包。AMF（Action Message Format）是Adobe设计的一种通用数据封装格式，在Adobe的很多产品中应用，简单来说，AMF将不同类型的数据用统一的格式来描述。第一个AMF包封装字符串类型数据，用来装入一个“onMetaData”标志，这个标志与Adobe的一些API调用有，在此不细述。第二个AMF包封装一个数组类型，这个数组中包含了音视频信息项的名称和值。具体说明如下，大家可以参照图片上的数据进行理解。

第一个AMF包：
第1个字节表示AMF包类型，一般总是0x02，表示字符串，其他值表示意义请查阅文档。
第2-3个字节为UI16类型值，表示字符串的长度，一般总是0x000A（“onMetaData”长度）。
后面字节为字符串数据，一般总为“onMetaData”。

第二个AMF包：
第1个字节表示AMF包类型，一般总是0x08，表示数组。
第2-5个字节为UI32类型值，表示数组元素的个数。
后面即为各数组元素的封装，数组元素为元素名称和值组成的对。表示方法如下：
第1-2个字节表示元素名称的长度，假设为L。
后面跟着为长度为L的字符串。
第L+3个字节表示元素值的类型。
后面跟着为对应值，占用字节数取决于值的类型
**/

class TagData;

class MetaData
{
public:
    TagData*     m_tagData;//指向当前的Tag文件
    unsigned int m_count;  //指示当前数据读取的位置

    char      str[1000];//用于临时存储字符串

    double parseDouble();//分析Double  类型0
    char   parseChar();  //分析Char    类型1
    void   parseString();//分析String  类型2
    void   SCRIPTDATAOBJECT();//数组   类型3

    int    parseReference();  //Short  类型7 
    void   parseEcma(); //分析ECMA数组 类型8

    void   SCRIPTDATASTRICTARRAY();//普通数组 类型10
    double SCRIPTDATADATE(); //时间 类型11 从1970.1.1到现在的秒数
    void   SCRIPTDATALONGSTRING();//分析String  类型12

    void   SCRIPTDATAOBJECTEND();

    void   anlysisMetaData(TagData*  tagData);//分析脚本数据 

    

    //SCRIPTDATAOBJECTEND

};

#endif