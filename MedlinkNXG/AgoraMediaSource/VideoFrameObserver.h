
#include <cstring>

#include "../SDK/include/IAgoraRtcEngine.h"
#include "../SDK/include/IAgoraMediaEngine.h"
#include <malloc.h>

int CutBandYUV(uint8_t *pInBuf, uint32_t width, uint32_t height, uint8_t **pOutBuf, int new_x, int new_y, int new_width, int new_height);


class AgoraVideoFrameObserver : public agora::media::IVideoFrameObserver
{
private:
	size_t buffersize;
	bool isFirst = false;
	uint8_t *buffer;
public:
	// 获取本地摄像头采集到的视频帧
	virtual bool onCaptureVideoFrame(VideoFrame& videoFrame) override
	{
		DWORD t1 = GetTickCount();
		int width = videoFrame.width;
		int height = videoFrame.height;
		memset(videoFrame.yBuffer, 0, videoFrame.yStride * height/2);
		memset(videoFrame.uBuffer, 128, videoFrame.uStride * height / 2);
		memset(videoFrame.vBuffer, 128, videoFrame.vStride * height / 2);
		/*
		if (!isFirst)
		{
			buffersize = videoFrame.yStride*height + videoFrame.uStride * height / 2 + videoFrame.vStride * height / 2;
			buffer = (uint8_t*)malloc(buffersize);
			isFirst = true;
		}
		memcpy(buffer, videoFrame.yBuffer, videoFrame.yStride*height);
		memcpy(buffer + videoFrame.yStride*height, videoFrame.uBuffer, videoFrame.uStride * height / 2);
		memcpy(buffer + videoFrame.yStride*height + videoFrame.uStride * height / 2, videoFrame.vBuffer, videoFrame.vStride * height / 2);
		uint8_t * output ;
		CutBandYUV(buffer, videoFrame.width, videoFrame.height, &output, 100, 200, 200, 200);
		videoFrame.width = 200;
		videoFrame.height = 200;
		videoFrame.yStride = 200;
		videoFrame.uStride = 100;
		videoFrame.vStride = 100;
		memcpy(videoFrame.yBuffer, output, videoFrame.yStride*videoFrame.height);
		memcpy(videoFrame.uBuffer, output+ videoFrame.yStride* videoFrame.height, videoFrame.uStride* videoFrame.height);
		memcpy(videoFrame.vBuffer, output + videoFrame.yStride* videoFrame.height + videoFrame.uStride * videoFrame.height / 2, videoFrame.vStride* videoFrame.height /2);
		free(output);
		DWORD t2 = GetTickCount() - t1;

		*/


		//not works only the buffer content can be channged.
		videoFrame.width = 100;
		videoFrame.height = 100;
		videoFrame.yStride = 100;
		videoFrame.uStride = 50;
		videoFrame.vStride = 50;





		
		return true;
	}

	// 获取远端用户发送的视频帧
	virtual bool onRenderVideoFrame(unsigned int uid, VideoFrame& videoFrame) override
	{
		return true;
	}
	// 获取本地视频编码前的视频帧
	virtual bool onPreEncodeVideoFrame(VideoFrame& videoFrame) override
	{
		return true;
	}
};
