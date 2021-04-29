#include "stdafx.h"
#include "VideoFrameObserver.h"



int CutBandYUV(uint8_t *pInBuf, uint32_t width, uint32_t height, uint8_t **pOutBuf, int new_x, int new_y, int new_width, int new_height)

{
	int x, y;

	if (NULL == pInBuf || 0 == width || 0 == height)

		return -1;



	int length = new_width * new_height * 3 / 2;

	*pOutBuf = (uint8_t *)malloc(length);

	if (NULL == *pOutBuf)

	{

		printf("malloc new size memory failed! size=%d\n", length);

		return -1;

	}

	memset(*pOutBuf, 0, length);



	uint8_t *pUBuf = *pOutBuf + new_width * new_height;

	uint8_t *pVBuf = *pOutBuf + new_width * new_height * 5 / 4;

	for (x = 0; x < new_width; x++)

	{

		for (y = 0; y < new_height; y++)	//每个循环写一列

		{

			*(*pOutBuf + y * new_width + x) = *(pInBuf + (x + new_x) + width * (y + new_y));	//cope Y

			//int ret = (y + new_y) % 2;

			if (1 == (x + new_x) % 2 && 1 == (y + new_y) % 2)

			{

				long pix = width * height + (width >> 1) * ((y + new_y) >> 1) + (((x + new_x)) >> 1);

				*(pUBuf + (new_width / 2)*(y / 2) + x / 2) = *(pInBuf + pix);	//cope U



				pix += width * height / 4;

				*(pVBuf + (new_width / 2)*(y / 2) + x / 2) = *(pInBuf + pix);	//cope V

			}

		}

	}

	return 0;

}


