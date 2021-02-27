#include "NDKFFmpeg.h"

#include <android/log.h>

#define LOG_TAG "FFmpeg_Log"
#define LOG_I(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOG_I_ARGS(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG ,__VA_ARGS__) // 定义LOGE类型

//当前C++兼容C语言
extern "C"{
//avcodec:编解码(最重要的库)
#include "libavcodec/avcodec.h"
//avformat:封装格式处理
#include "libavformat/avformat.h"
//avutil:工具库(大部分库都需要这个库的支持)
#include "libavutil/avutil.h"
//swscale:视频像素数据格式转换
#include "libswscale/swscale.h"

#include "libavutil/frame.h"
#include "libavcodec/avcodec.h"

}

//1.NDK音视频编解码：FFmpeg-测试配置
extern "C" JNIEXPORT void JNICALL Java_com_sun_eg10_NDKFFmpeg_callFFmpegTestConfig
(JNIEnv *, jobject){
const char *configuration_info = avcodec_configuration();
LOG_I_ARGS("FFmpeg配置信息：%s",configuration_info);
}

//2.NDK音视频编解码：FFmpeg-读取视频信息-老版本实现
//测试的是手机(sdcard路径，需要从Android层传递)
extern "C" JNIEXPORT void JNICALL Java_com_sun_eg10_NDKFFmpeg_callFFmpegOldReadInfo
(JNIEnv *env, jobject jobj,jstring jFilePath){
//将Java的String转成C的字符串
const char* cFilePath = env->GetStringUTFChars(jFilePath,NULL);

//接下来就是读取视频信息
//分析音视频解码流程
//第一步：注册组件
av_register_all();

//第二步：打开输入视频文件
//初始化封装格式上下文
AVFormatContext* avFmtCtx = avformat_alloc_context();
int fmt_open_result = avformat_open_input(&avFmtCtx,cFilePath,NULL,NULL);
if(fmt_open_result != 0){
LOG_I("打开视频文件失败");
return;
}

//第三步：获取视频文件信息（文件流）
//很多流（例如：视频流、音频流、字幕流等等......）
//然后我的目的：我只需要视频流信息
int fmt_fd_info = avformat_find_stream_info(avFmtCtx,NULL);
if(fmt_fd_info < 0){
LOG_I("获取视频文件信息失败");
//打印错误码
//错误信息
char* error_info;
//根据错误码找到对应的错误信息s
av_strerror(fmt_fd_info,error_info,1024);
LOG_I_ARGS("错误信息：%s",error_info);
return;
}

//第四步：查找解码器
//1.获取视频流的索引位置
//遍历所有的流，找到视频流
int av_stream_index = -1;
//avFmtCtx->nb_streams:返回流的大小
for (int i = 0; i < avFmtCtx->nb_streams; ++i) {
//判断流的类型(老的API实现)
//是否是视频流
if(avFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
av_stream_index = i;
break;
}
}
if(av_stream_index == -1){
LOG_I("不能存在视频流......");
return;
}

//2.根据视频流的索引位置，查找视频流的解码器
//根据视频流的索引位置，获取到指定的参数上下文
//编码方式(编码上下文)
AVCodecContext *avCdCtx = avFmtCtx->streams[av_stream_index]->codec;
AVCodec *avCd = avcodec_find_decoder(avCdCtx->codec_id);
if(avCd == NULL){
LOG_I("没有找到这个解码器");
return;
}

//第五步：打开解码器
int av_cd_open_result = avcodec_open2(avCdCtx,avCd,NULL);
if(av_cd_open_result != 0){
LOG_I("解码器打开失败......");
return;
}

//获取配置视频信息
//文件格式、文件的宽高、解码器的名称等等......
LOG_I_ARGS("视频文件的格式：%s",avFmtCtx->iformat->name);
//返回的单位是：微秒(avFmtCtx->duration)
LOG_I_ARGS("视频的时长：%ld秒",(avFmtCtx->duration)/1000000);
//获取宽高
LOG_I_ARGS("视频的宽高：%d x %d = ",avCdCtx->width,avCdCtx->height);
//解码器的名称
LOG_I_ARGS("解码器的名称：%s",avCd->name);
}


//3.NDK音视频编解码：FFmpeg-读取视频信息-新版本实现
extern "C" JNIEXPORT void JNICALL Java_com_sun_eg10_NDKFFmpeg_callFFmpegNewReadInfo
(JNIEnv *env, jobject jobj,jstring jFilePath){
//将Java的String转成C的字符串
const char* cFilePath = env->GetStringUTFChars(jFilePath,NULL);

//接下来就是读取视频信息
//分析音视频解码流程
//第一步：注册组件
av_register_all();

//第二步：打开输入视频文件
//初始化封装格式上下文
AVFormatContext* avFmtCtx = avformat_alloc_context();
int fmt_open_result = avformat_open_input(&avFmtCtx,cFilePath,NULL,NULL);
if(fmt_open_result != 0){
LOG_I("打开视频文件失败");
return;
}

//第三步：获取视频文件信息（文件流）
//很多流（例如：视频流、音频流、字幕流等等......）
//然后我的目的：我只需要视频流信息
int fmt_fd_info = avformat_find_stream_info(avFmtCtx,NULL);
if(fmt_fd_info < 0){
LOG_I("获取视频文件信息失败");
//打印错误码
//错误信息
char* error_info;
//根据错误码找到对应的错误信息s
av_strerror(fmt_fd_info,error_info,1024);
LOG_I_ARGS("错误信息：%s",error_info);
return;
}

//第四步：查找解码器
//1.获取视频流的索引位置
//遍历所有的流，找到视频流
int av_stream_index = -1;
//avFmtCtx->nb_streams:返回流的大小
for (int i = 0; i < avFmtCtx->nb_streams; ++i) {
//判断流的类型(老的API实现)
//是否是视频流
//第一个地方：
//新的API: avFmtCtx->streams[i]->codecpar->codec_type
//老的API: avFmtCtx->streams[i]->codec->codec_type
if(avFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
av_stream_index = i;
break;
}
}
if(av_stream_index == -1){
LOG_I("不能存在视频流......");
return;
}

//2.根据视频流的索引位置，查找视频流的解码器
//根据视频流的索引位置，获取到指定的参数上下文
//编码方式(编码上下文)
//老的API:AVCodecContext *avCdCtx = avFmtCtx->streams[av_stream_index]->codec;
//新的API:AVCodecParameters *avCdPm = avFmtCtx->streams[av_stream_index]->codecpar;
//解码器参数列表
AVCodecParameters *avCdPm = avFmtCtx->streams[av_stream_index]->codecpar;
AVCodec *avCd = avcodec_find_decoder(avCdPm->codec_id);
if(avCd == NULL){
LOG_I("没有找到这个解码器");
return;
}

//第五步：打开解码器
AVCodecContext *avCdCtx = avcodec_alloc_context3(avCd);
int av_cd_open_result = avcodec_open2(avCdCtx,avCd,NULL);
if(av_cd_open_result != 0){
LOG_I("解码器打开失败......");
return;
}

//获取配置视频信息
//文件格式、文件的宽高、解码器的名称等等......
LOG_I_ARGS("视频文件的格式：%s",avFmtCtx->iformat->name);
//返回的单位是：微秒(avFmtCtx->duration)
LOG_I_ARGS("视频的时长：%ld秒",(avFmtCtx->duration)/1000000);
//获取宽高
LOG_I_ARGS("视频的宽高：%d x %d = ",avCdPm->width,avCdPm->height);
//解码器的名称
LOG_I_ARGS("解码器的名称：%s",avCd->name);
}



//4.NDK音视频编解码：FFmpeg-视频解码-视频像素数据(YUV420P)-老版本
extern "C" JNIEXPORT void JNICALL Java_com_sun_eg10_NDKFFmpeg_callFFmpegOldDecode
(JNIEnv *env, jobject jobj,jstring jInFilePath,jstring jOutFilePath){
//将Java的String转成C的字符串
const char* cInFilePath = env->GetStringUTFChars(jInFilePath,NULL);
const char* cOutFilePath = env->GetStringUTFChars(jOutFilePath,NULL);

//接下来就是读取视频信息
//分析音视频解码流程
//第一步：注册组件
av_register_all();

//第二步：打开输入视频文件
//初始化封装格式上下文
AVFormatContext* avFmtCtx = avformat_alloc_context();
int fmt_open_result = avformat_open_input(&avFmtCtx,cInFilePath,NULL,NULL);
if(fmt_open_result != 0){
LOG_I("打开视频文件失败");
return;
}

//第三步：获取视频文件信息（文件流）
//很多流（例如：视频流、音频流、字幕流等等......）
//然后我的目的：我只需要视频流信息
int fmt_fd_info = avformat_find_stream_info(avFmtCtx,NULL);
if(fmt_fd_info < 0){
LOG_I("获取视频文件信息失败");
//打印错误码
//错误信息
char* error_info;
//根据错误码找到对应的错误信息s
av_strerror(fmt_fd_info,error_info,1024);
LOG_I_ARGS("错误信息：%s",error_info);
return;
}

//第四步：查找解码器
//1.获取视频流的索引位置
//遍历所有的流，找到视频流
int av_stream_index = -1;
//avFmtCtx->nb_streams:返回流的大小
for (int i = 0; i < avFmtCtx->nb_streams; ++i) {
//判断流的类型(老的API实现)
//是否是视频流
if(avFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
av_stream_index = i;
break;
}
}
if(av_stream_index == -1){
LOG_I("不能存在视频流......");
return;
}

//2.根据视频流的索引位置，查找视频流的解码器
//根据视频流的索引位置，获取到指定的参数上下文
//编码方式(编码上下文)
AVCodecContext *avCdCtx = avFmtCtx->streams[av_stream_index]->codec;
AVCodec *avCd = avcodec_find_decoder(avCdCtx->codec_id);
if(avCd == NULL){
LOG_I("没有找到这个解码器");
return;
}

//第五步：打开解码器
int av_cd_open_result = avcodec_open2(avCdCtx,avCd,NULL);
if(av_cd_open_result != 0){
LOG_I("解码器打开失败......");
return;
}

//获取配置视频信息
//文件格式、文件的宽高、解码器的名称等等......
LOG_I_ARGS("视频文件的格式：%s",avFmtCtx->iformat->name);
//返回的单位是：微秒(avFmtCtx->duration)
LOG_I_ARGS("视频的时长：%lld秒",(avFmtCtx->duration)/1000000);
//获取宽高
LOG_I_ARGS("视频的宽高：%d x %d = ",avCdCtx->width,avCdCtx->height);
//解码器的名称
LOG_I_ARGS("解码器的名称：%s",avCd->name);


//第六步：从输入文件读取一帧压缩数据(解压缩:一帧一帧读取解压缩)
//循环读取每一帧数据
//读取的帧数据缓存到那里(开辟一块内存空间用于保存)
AVPacket* packet = (AVPacket*)av_malloc(sizeof(AVPacket));
//缓存一帧数据(就是一张图片)
AVFrame* in_frame_picture = av_frame_alloc();

//定义输出一帧数据(缓冲区:YUV420p类型)
AVFrame* out_frame_picture_YUV42P = av_frame_alloc();
//指定缓冲区的类型(像素格式:YUV420P)
//开启空间的大小是：YUV420P格式数据大小
uint8_t *out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P,avCdCtx->width,avCdCtx->height));
//指定填充数据(YUV420P数据)
avpicture_fill((AVPicture *)out_frame_picture_YUV42P,out_buffer,AV_PIX_FMT_YUV420P,avCdCtx->width,avCdCtx->height);

int got_picture_ptr,ret,y_size = 0,u_size = 0,v_size = 0,frame_index = 0;

//打开文件
FILE* out_file_yuv = fopen(cOutFilePath,"wb");
if(out_file_yuv == NULL){
LOG_I("文件不存在!");
return;
}

//视频像素数据格式转换上下文
//参数一：输入的宽度
//参数二：输入的高度
//参数三：输入的数据
//参数四：输出的宽度
//参数五：输出的高度
//参数六：输出的数据
//参数七：视频像素数据格式转换算法类型(使用什么算法)
//参数八：字节对齐类型，一般都是默认1（字节对齐类型：提高读取效率）
SwsContext *sws_ctx =
        sws_getContext(avCdCtx->width,avCdCtx->height, avCdCtx->pix_fmt,
                       avCdCtx->width,avCdCtx->height,AV_PIX_FMT_YUV420P,
                       SWS_BICUBIC,NULL,NULL,NULL);

//读取返回值
//>=0：正在读取
//<0:读取失败或者说读取完毕
while (av_read_frame(avFmtCtx,packet) >= 0){
//有视频流帧数据、音频流帧数据、字幕流......
if(packet->stream_index == av_stream_index){
//我们只需要解码一帧视频压缩数据，得到视频像素数据
//老的API
//参数一：解码器上下文
//参数二：一帧数据
//参数三：是否正在解码（0：代表解码完毕，非0：正在解码）
//返回值：小于0解码失败（错误、异常），否则成功解码一帧数据
ret = avcodec_decode_video2(avCdCtx,in_frame_picture,&got_picture_ptr,packet);
if(ret < 0){
LOG_I("解码失败!");
return;
}

//0：代表解码完毕，非0：正在解码
if(got_picture_ptr){

//接下来我要将解码后的数据（视频像素数据，保存为YUV420P文件）
//在这个地方需要指定输出文件的类型(格式转换)
//我要将AVFrame转成视频像素数YUV420P格式
//参数一：视频像素数据格式上下文(SwsContext)
//参数二：输入的数据（转格式前的视频像素数据）
//参数三：输入画面每一行的大小(视频像素数据转换一行一行的转)
//参数四：输入画面每一行的要转码的开始位置
//参数五：输出画面数据(转格式后的视频像素数据)
//参数六：输出画面每一行的大小
sws_scale(sws_ctx,(const uint8_t *const*)in_frame_picture->data,
in_frame_picture->linesize,0,avCdCtx->height,
out_frame_picture_YUV42P->data,out_frame_picture_YUV42P->linesize);


//普及: YUV420P格式结构
//Y代表亮度，UV代表色度（人的眼睛对亮度敏感，对色度不敏感）
//再深入：计算机图像学相关
//YUV420P格式规定一：Y结构表示一个像素点(一个像素点就是一个Y)
//YUV420P格式规定二：四个Y对应一个U和一个V(也就是四个像素点，对应一个U和V)
//Y默认情况下：灰度
//计算Y大小：y = 宽x高
y_size = avCdCtx->width * avCdCtx->height;
u_size = y_size / 4;
v_size = y_size / 4;



//写入文件
//首先写入Y，再是U，再是V
//in_frame_picture->data[0]表示Y
fwrite(in_frame_picture->data[0],1,y_size,out_file_yuv);
//in_frame_picture->data[1]表示U
fwrite(in_frame_picture->data[1],1,u_size,out_file_yuv);
//in_frame_picture->data[2]表示V
fwrite(in_frame_picture->data[2],1,v_size,out_file_yuv);


frame_index++;

LOG_I_ARGS("当前是第%d帧",frame_index);
}
}

//关闭流
av_free_packet(packet);
}

//关闭流
fclose(out_file_yuv);
av_frame_free(&in_frame_picture);
av_frame_free(&out_frame_picture_YUV42P);
avcodec_close(avCdCtx);
avformat_free_context(avFmtCtx);

}




