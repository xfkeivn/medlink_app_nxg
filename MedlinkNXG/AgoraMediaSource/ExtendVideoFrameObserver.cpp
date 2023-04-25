#include "stdafx.h"
#include "ExtendVideoFrameObserver.h"


VIDEO_BUFFER		buffer;










#include <stdio.h>

#define READ_MAX (1024)



typedef unsigned char  uInt8;

typedef unsigned short uInt16;

typedef unsigned int uInt32;

typedef char Int8;

typedef short Int16;

typedef int Int32;



typedef enum

{

	TYPE_YUV422I_UYVY,

	TYPE_YUV422I_YUYV,

	TYPE_YUV420SP_NV12,

	TYPE_YUV420SP_NV21,

	TYPE_YUV422P,

	TYPE_YUV444I,

	TYPE_YUV444P,

}enYuvType;



typedef enum

{

	YUV_GREEN,

	YUV_RED,

	YUV_BLUE,

	YUV_PURPLE,

	YUV_DARK_GREEN,

	YUV_YELLOW,

	YUV_LIGHT_BLUE,

	YUV_LIGHT_PURPLE,

	YUV_DARK_BLACK,

	YUV_GRAY,

	YUV_WHITE,

	YUV_COLOR_MAX,

}enYuvColorIdx;



typedef struct

{

	uInt8 Y;

	uInt8 U;

	uInt8 V;

}stYuvColor;



typedef struct

{

	uInt16 x;

	uInt16 y;

}stPoint;



typedef struct

{

	stPoint startPoint;

	stPoint endPoint;

	uInt16 lineWidth;

	enYuvColorIdx clrIdx;

}stDrawLineInfo;



typedef struct

{

	enYuvType yuvType;

	uInt8 *pYuvBuff;

	uInt16 width;

	uInt16 height;

}stYuvBuffInfo;



static stYuvColor s_color_table[YUV_COLOR_MAX] = {

	{0x00, 0x00, 0x00}, // green

	{0x00, 0x00, 0xff}, // red

	{0x00, 0xff, 0x00},	// blue

	{0x00, 0xff, 0xff},	// purple

	{0xff, 0x00, 0x00}, // dark green

	{0xff, 0x00, 0xff}, // yellow

	{0xff, 0xff, 0x00}, // light blue

	{0xff, 0xff, 0xff}, // light purple

	{0x00, 0x80, 0x80}, // dark black

	{0x80, 0x80, 0x80}, // gray

	{0xff, 0x80, 0x80}, // white

};


void yuv_setdata(

	uInt8* YBuff,

	uInt8* UVBuff,

	enYuvType yuvType,

	uInt16 width,

	uInt16 height,

	stPoint draw_point,

	enYuvColorIdx clrIdx)

{

	switch (yuvType)

	{

	case TYPE_YUV422I_UYVY:

	case TYPE_YUV422I_YUYV:

	{

		/*

			UYVY UYVY UYVY UYVY

		*/

		uInt32 tmp = draw_point.y * width * 2;

		uInt32 y_offset = 0, u_offset = 0, v_offset = 0;

		if (yuvType == TYPE_YUV422I_UYVY) {

			u_offset = tmp + draw_point.x / 2 * 4;

			v_offset = u_offset + 2;

			y_offset = u_offset + 1;

		}

		else {

			y_offset = tmp + draw_point.x / 2 * 4;

			u_offset = y_offset + 1;

			v_offset = u_offset + 2;

		}

		YBuff[y_offset] = s_color_table[clrIdx].Y;

		YBuff[y_offset + 2] = s_color_table[clrIdx].Y;

		YBuff[u_offset] = s_color_table[clrIdx].U;

		YBuff[v_offset] = s_color_table[clrIdx].V;

	}break;

	case TYPE_YUV420SP_NV12:

	case TYPE_YUV420SP_NV21:

	{

		/*

			YY YY

			YY YY

			UV UV

		*/

		uInt32 y_offset = draw_point.y * width + draw_point.x;

		uInt32 u_offset = 0, v_offset = 0;

		YBuff[y_offset] = s_color_table[clrIdx].Y;

#if 0

		Int32 x_flag = 1, y_flag = 1;

		if (draw_point.y % 2 == 0) {

			YBuff[y_offset + width] = s_color_table[clrIdx].Y;

			y_flag = 1;

		}

		else {

			YBuff[y_offset - width] = s_color_table[clrIdx].Y;

			y_flag = -1;

		}



		if (draw_point.x % 2 == 0) {

			YBuff[y_offset + 1] = s_color_table[clrIdx].Y;

			x_flag = 1;

		}

		else {

			YBuff[y_offset - 1] = s_color_table[clrIdx].Y;

			x_flag = -1;

		}

		YBuff[y_offset + width * y_flag + 1 * x_flag] = s_color_table[clrIdx].Y;

#endif



		if (yuvType == TYPE_YUV420SP_NV12) {

			u_offset = (draw_point.y / 2) * width + draw_point.x / 2 * 2;

			v_offset = u_offset + 1;

		}

		else {

			v_offset = (draw_point.y / 2) * width + draw_point.x / 2 * 2;

			u_offset = v_offset + 1;

		}

		UVBuff[u_offset] = s_color_table[clrIdx].U;

		UVBuff[v_offset] = s_color_table[clrIdx].V;

		//printf("[%d, %d]: y_offset = %d, u_offset = %d, v_offset = %d\n",

		//	draw_point.x, draw_point.y, y_offset, u_offset, v_offset);

	}break;

	case TYPE_YUV444P:

	{

		/*

			YYYYYYYY

			UUUUUUUU

			VVVVVVVV

		*/

		uInt32 y_offset = 0, u_offset = 0, v_offset = 0;

		uInt32 plane_size = width * height;

		y_offset = draw_point.y * width + draw_point.x;

		u_offset = y_offset;

		v_offset = plane_size + u_offset;

		YBuff[y_offset] = s_color_table[clrIdx].Y;

		UVBuff[u_offset] = s_color_table[clrIdx].U;

		UVBuff[v_offset] = s_color_table[clrIdx].V;

	}break;

	case TYPE_YUV444I:

	{

		/*

			YUV YUV YUV YUV YUV YUV YUV YUV

		*/

		uInt32 y_offset = 0, u_offset = 0, v_offset = 0;

		y_offset = draw_point.y * width * 3 + draw_point.x * 3;

		u_offset = y_offset + 1;

		v_offset = u_offset + 1;

		YBuff[y_offset] = s_color_table[clrIdx].Y;

		YBuff[u_offset] = s_color_table[clrIdx].U;

		YBuff[v_offset] = s_color_table[clrIdx].V;

	}break;

	case TYPE_YUV422P:

	{

		/*

			YYYYYYYY

			UUUU

			VVVV

		*/

		uInt32 y_offset = 0, u_offset = 0, v_offset = 0;

		uInt32 plane_size = width * height / 2;

		y_offset = draw_point.y * width + draw_point.x;

		u_offset = (draw_point.y / 2) * width + draw_point.x / 2;

		v_offset = plane_size + u_offset;

		YBuff[y_offset] = s_color_table[clrIdx].Y;

		UVBuff[u_offset] = s_color_table[clrIdx].U;

		UVBuff[v_offset] = s_color_table[clrIdx].V;

	}break;

	}

}



void yuv_drawline(stYuvBuffInfo *pYuvBuffInfo, stDrawLineInfo *pDrawLineInfo)

{

	if (!pYuvBuffInfo || !pYuvBuffInfo->pYuvBuff) return;



	uInt8 *YBuff = NULL, *UVBuff = NULL;

	uInt16 x0 = pDrawLineInfo->startPoint.x, y0 = pDrawLineInfo->startPoint.y;

	uInt16 x1 = pDrawLineInfo->endPoint.x, y1 = pDrawLineInfo->endPoint.y;



	if (pDrawLineInfo->lineWidth == 0) pDrawLineInfo->lineWidth = 1;

	x0 = (x0 >= pYuvBuffInfo->width) ? (x0 - pDrawLineInfo->lineWidth) : x0;

	x1 = (x1 >= pYuvBuffInfo->width) ? (x1 - pDrawLineInfo->lineWidth) : x1;

	y0 = (y0 >= pYuvBuffInfo->height) ? (y0 - pDrawLineInfo->lineWidth) : y0;

	y1 = (y1 >= pYuvBuffInfo->height) ? (y1 - pDrawLineInfo->lineWidth) : y1;



	uInt16 dx = (x0 > x1) ? (x0 - x1) : (x1 - x0);

	uInt16 dy = (y0 > y1) ? (y0 - y1) : (y1 - y0);



	Int16 xstep = (x0 < x1) ? 1 : -1;

	Int16 ystep = (y0 < y1) ? 1 : -1;

	Int16 nstep = 0, eps = 0;



	stPoint draw_point;

	draw_point.x = x0;

	draw_point.y = y0;



	switch (pYuvBuffInfo->yuvType)

	{

	case TYPE_YUV422I_UYVY:

	case TYPE_YUV422I_YUYV:

	case TYPE_YUV444I:

	{

		YBuff = pYuvBuffInfo->pYuvBuff;

		UVBuff = NULL;

	}break;

	case TYPE_YUV420SP_NV12:

	case TYPE_YUV420SP_NV21:

	case TYPE_YUV444P:

	case TYPE_YUV422P:

	{

		YBuff = pYuvBuffInfo->pYuvBuff;

		UVBuff = pYuvBuffInfo->pYuvBuff + pYuvBuffInfo->width * pYuvBuffInfo->height;

	}break;

	default:

		return;

	}



	// ²¼À×É­ººÄ·Ëã·¨»­Ïß

	if (dx > dy) {

		while (nstep <= dx) {

			yuv_setdata(YBuff, UVBuff, pYuvBuffInfo->yuvType, pYuvBuffInfo->width, pYuvBuffInfo->height, draw_point, pDrawLineInfo->clrIdx);

			eps += dy;

			if ((eps << 1) >= dx) {

				draw_point.y += ystep;

				eps -= dx;

			}

			draw_point.x += xstep;

			nstep++;

		}

	}
	else {

		while (nstep <= dy) {

			yuv_setdata(YBuff, UVBuff, pYuvBuffInfo->yuvType, pYuvBuffInfo->width, pYuvBuffInfo->height, draw_point, pDrawLineInfo->clrIdx);

			eps += dx;

			if ((eps << 1) >= dy) {

				draw_point.x += xstep;

				eps -= dy;

			}

			draw_point.y += ystep;

			nstep++;

		}

	}

}



void draw_rect(stYuvBuffInfo* yuvBuffInfo)

{

	stDrawLineInfo drawLineInfo;

	drawLineInfo.clrIdx = YUV_RED;

	drawLineInfo.lineWidth = 1;

	drawLineInfo.startPoint.x = 160;

	drawLineInfo.startPoint.y = 140;

	drawLineInfo.endPoint.x = 560;

	drawLineInfo.endPoint.y = 340;

	yuv_drawline(yuvBuffInfo, &drawLineInfo);



	drawLineInfo.clrIdx = YUV_PURPLE;

	drawLineInfo.lineWidth = 1;

	drawLineInfo.startPoint.x = 560;

	drawLineInfo.startPoint.y = 140;

	drawLineInfo.endPoint.x = 160;

	drawLineInfo.endPoint.y = 340;

	yuv_drawline(yuvBuffInfo, &drawLineInfo);



	drawLineInfo.clrIdx = YUV_YELLOW;

	drawLineInfo.lineWidth = 1;

	drawLineInfo.startPoint.x = 160;

	drawLineInfo.startPoint.y = 140;

	drawLineInfo.endPoint.x = 560;

	drawLineInfo.endPoint.y = 140;

	yuv_drawline(yuvBuffInfo, &drawLineInfo);



	drawLineInfo.clrIdx = YUV_GREEN;

	drawLineInfo.lineWidth = 1;

	drawLineInfo.startPoint.x = 160;

	drawLineInfo.startPoint.y = 140;

	drawLineInfo.endPoint.x = 160;

	drawLineInfo.endPoint.y = 340;

	yuv_drawline(yuvBuffInfo, &drawLineInfo);



	drawLineInfo.clrIdx = YUV_BLUE;

	drawLineInfo.lineWidth = 1;

	drawLineInfo.startPoint.x = 160;

	drawLineInfo.startPoint.y = 340;

	drawLineInfo.endPoint.x = 560;

	drawLineInfo.endPoint.y = 340;

	yuv_drawline(yuvBuffInfo, &drawLineInfo);



	drawLineInfo.clrIdx = YUV_WHITE;

	drawLineInfo.lineWidth = 1;

	drawLineInfo.startPoint.x = 560;

	drawLineInfo.startPoint.y = 140;

	drawLineInfo.endPoint.x = 560;

	drawLineInfo.endPoint.y = 340;

	yuv_drawline(yuvBuffInfo, &drawLineInfo);

}






















CExtendVideoFrameObserver::CExtendVideoFrameObserver()
{
	m_lpImageBuffer = new BYTE[VIDEO_BUF_SIZE];
}


CExtendVideoFrameObserver::~CExtendVideoFrameObserver()
{
    if (m_lpImageBuffer) {
        delete[] m_lpImageBuffer;
        m_lpImageBuffer = nullptr;
    }
}

bool CExtendVideoFrameObserver::onCaptureVideoFrame(VideoFrame& videoFrame)
{
    int bufSize = videoFrame.width*videoFrame.height * 3 / 2;
    
    int timestamp = GetTickCount();
    if (CAgVideoBuffer::GetInstance()->readBuffer(m_lpImageBuffer, bufSize, timestamp)) {
        memcpy_s(buffer.m_lpImageBuffer, bufSize, m_lpImageBuffer, bufSize);
        buffer.timestamp = timestamp;
    }
    else
        OutputDebugString(L"readBuffer failed");

	/*BOOL bSuccess = CVideoPackageQueue::GetInstance()->PopVideoPackage(m_lpImageBuffer, &nBufferSize);
	if (!bSuccess)
		return false;*/

	m_lpY = buffer.m_lpImageBuffer;
	m_lpU = buffer.m_lpImageBuffer + videoFrame.height*videoFrame.width;
	m_lpV = buffer.m_lpImageBuffer + 5 * videoFrame.height*videoFrame.width / 4;

	memcpy_s(videoFrame.yBuffer, videoFrame.height*videoFrame.width, m_lpY, videoFrame.height*videoFrame.width);
	videoFrame.yStride = videoFrame.width;
	
	memcpy_s(videoFrame.uBuffer, videoFrame.height*videoFrame.width / 4, m_lpU, videoFrame.height*videoFrame.width / 4);
	videoFrame.uStride = videoFrame.width/2;

	memcpy_s(videoFrame.vBuffer, videoFrame.height*videoFrame.width / 4, m_lpV, videoFrame.height*videoFrame.width / 4);
	videoFrame.vStride = videoFrame.width/2;

	videoFrame.type = FRAME_TYPE_YUV420;
	videoFrame.rotation = 0;
	onRenderVideoFrame(0, videoFrame);
	return true;
}

bool CExtendVideoFrameObserver::onRenderVideoFrame(unsigned int uid, VideoFrame& videoFrame)
{

	 stYuvBuffInfo bufferinfo;
	 bufferinfo.width = videoFrame.width;
	 bufferinfo.height = videoFrame.height;
	 bufferinfo.yuvType = TYPE_YUV420SP_NV21;
	 bufferinfo.pYuvBuff = (uInt8*)buffer.m_lpImageBuffer;


	 m_lpY = buffer.m_lpImageBuffer;
	 m_lpU = buffer.m_lpImageBuffer + videoFrame.height*videoFrame.width;
	 m_lpV = buffer.m_lpImageBuffer + 5 * videoFrame.height*videoFrame.width / 4;


	 memcpy_s(m_lpY, videoFrame.height*videoFrame.width, videoFrame.yBuffer, videoFrame.height*videoFrame.width);
	 videoFrame.yStride = videoFrame.width;

	 memcpy_s(m_lpU, videoFrame.height*videoFrame.width / 4, videoFrame.uBuffer, videoFrame.height*videoFrame.width / 4);
	 videoFrame.uStride = videoFrame.width / 2;

	 memcpy_s(m_lpV, videoFrame.height*videoFrame.width / 4, videoFrame.yBuffer, videoFrame.height*videoFrame.width / 4);
	 videoFrame.vStride = videoFrame.width / 2;

	 stDrawLineInfo lineinfo;
	 lineinfo.startPoint = { 0,0 };
	 lineinfo.endPoint = { 100,100 };
	 lineinfo.lineWidth = 50;
	 lineinfo.clrIdx = YUV_YELLOW;

	 yuv_drawline(&bufferinfo, &lineinfo);

	 m_lpY = buffer.m_lpImageBuffer;
	 m_lpU = buffer.m_lpImageBuffer + videoFrame.height*videoFrame.width;
	 m_lpV = buffer.m_lpImageBuffer + 5 * videoFrame.height*videoFrame.width / 4;

	 memcpy_s(videoFrame.yBuffer, videoFrame.height*videoFrame.width, m_lpY, videoFrame.height*videoFrame.width);
	 videoFrame.yStride = videoFrame.width;

	 memcpy_s(videoFrame.uBuffer, videoFrame.height*videoFrame.width / 4, m_lpU, videoFrame.height*videoFrame.width / 4);
	 videoFrame.uStride = videoFrame.width / 2;

	 memcpy_s(videoFrame.vBuffer, videoFrame.height*videoFrame.width / 4, m_lpV, videoFrame.height*videoFrame.width / 4);
	 videoFrame.vStride = videoFrame.width / 2;

	 videoFrame.type = FRAME_TYPE_YUV420;
	 videoFrame.rotation = 0;






	return true;
}
