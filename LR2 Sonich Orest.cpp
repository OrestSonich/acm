#define _CRT_SECURE_NO_WARNINGS 
#define WIN32_LEAN_AND_MEAN



#ifndef TO_BMP_IMPLEMENTATION
#define TO_BMP_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "LR2 Sonich Orest.cpp"

#define OUTPUT_SIZE   512
#define OUTPUT_WIDTH  OUTPUT_SIZE
#define OUTPUT_HEIGHT OUTPUT_SIZE
#define NEIGHBOURHOOD 0
#define OUTPUT_SCALE 16


void mark(BMP* bmpPtr, UINT neighbourhood, UINT xSize, UINT ySize, UCHAR pt, UINT scale) {
	UINT neighbourhood2 = 2 * neighbourhood;
	UINT 	xO, yO;

	for (UINT x = 0; x < xSize; ++x) {
		for (UINT y = 0; !y || (!(x % scale) && y < 10); ++y) {
			for (xO = 0; xO <= neighbourhood2; ++xO) {
				for (yO = 0; yO <= neighbourhood2; ++yO) {
					if (x + xO >= neighbourhood && x + xO - neighbourhood < xSize && y + yO >= neighbourhood && y + yO - neighbourhood < ySize) {
						BMP_SetPixelIndex(bmpPtr, x + xO - neighbourhood, ySize - (y + yO - neighbourhood), pt);
					}
				}
			}
		}
	}

	for (UINT y = 0; y < ySize; ++y) {
		for (UINT x = 0; !x || (!(y % scale) && x < 10); ++x) {
			for (xO = 0; xO <= neighbourhood2; ++xO) {
				for (yO = 0; yO <= neighbourhood2; ++yO) {
					if (x + xO >= neighbourhood && x + xO - neighbourhood < xSize && y + yO >= neighbourhood && y + yO - neighbourhood < ySize) {
						BMP_SetPixelIndex(bmpPtr, x + xO - neighbourhood, ySize - (y + yO - neighbourhood), pt);
					}
				}
			}
		}
	}
}

double factorial(int n) {
	double fact = 1;
	for (int i = 1; i <= n; i++) {
		fact *= i;
	}
	return fact;
}

double functionForTabulateK3(double arg) {
	return factorial((int)round(arg)) / pow(arg, 3.);
	//return pow(2, arg) / pow(arg, 3.);
}

double functionForTabulateK4(double arg) {
	return factorial((int)round(arg)) / pow(arg, 4.);
	//return pow(2,arg) / pow(arg, 4.);

}

double functionForTabulateK5(double arg) {
	return factorial((int)round(arg)) / pow(arg, 5.);
	//return pow(2, arg) / pow(arg, 5.);

}

void tabulate(double(*functionPtr)(double arg), BMP* bmpPtr, UINT neighbourhood, UINT xSize, UINT ySize, UCHAR pt, UINT scale) {
	UINT neighbourhood2 = 2 * neighbourhood;
	UINT 	xO, yO;
	UINT 	fraction;
	UINT 	scaledXSize = (UINT)((double)xSize / (double)scale);
	for (UINT x_ = 0; x_ < scaledXSize; x_++) {
		for (fraction = 0; fraction < scale; ++fraction) {
			double fX = (double)fraction / (double)scale + (double)x_;
			UINT x = (UINT)(fX * (double)scale);
			UINT y = (UINT)(functionPtr(fX) * (double)scale);
			for (xO = 0; xO <= neighbourhood2; xO++) {
				for (yO = 0; yO <= neighbourhood2; yO++) {
					if (x + xO >= neighbourhood && x + xO - neighbourhood < xSize && y + yO >= neighbourhood && y + yO - neighbourhood < ySize) {
						BMP_SetPixelIndex(bmpPtr, x + xO - neighbourhood, ySize - (y + yO - neighbourhood), pt);
					}
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	BMP* outputForK3, * outputForK4, * outputForK5;
	UCHAR 	r = 0xff, g = 0xff, b = 0xff;

	outputForK3 = BMP_Create(OUTPUT_WIDTH, OUTPUT_HEIGHT, 8);
	BMP_CHECK_ERROR(stderr, -3);

	outputForK4 = BMP_Create(OUTPUT_WIDTH, OUTPUT_HEIGHT, 8);
	BMP_CHECK_ERROR(stderr, -3);

	outputForK5 = BMP_Create(OUTPUT_WIDTH, OUTPUT_HEIGHT, 8);
	BMP_CHECK_ERROR(stderr, -3);

	BMP_SetPaletteColor(outputForK3, 2, 0, 255, 0);
	BMP_SetPaletteColor(outputForK3, 1, 255, 0, 0);
	BMP_SetPaletteColor(outputForK3, 0, 0, 0, 0);

	BMP_SetPaletteColor(outputForK4, 2, 0, 255, 0);
	BMP_SetPaletteColor(outputForK4, 1, 255, 0, 0);
	BMP_SetPaletteColor(outputForK4, 0, 0, 0, 0);

	BMP_SetPaletteColor(outputForK5, 2, 0, 255, 0);
	BMP_SetPaletteColor(outputForK5, 1, 255, 0, 0);
	BMP_SetPaletteColor(outputForK5, 0, 0, 0, 0);

	mark(outputForK3, 0, OUTPUT_WIDTH, OUTPUT_HEIGHT, 1, OUTPUT_SCALE);
	tabulate(functionForTabulateK3, outputForK3, NEIGHBOURHOOD, OUTPUT_WIDTH, OUTPUT_HEIGHT, 2, OUTPUT_SCALE);

	mark(outputForK4, 0, OUTPUT_WIDTH, OUTPUT_HEIGHT, 1, OUTPUT_SCALE);
	tabulate(functionForTabulateK4, outputForK4, NEIGHBOURHOOD, OUTPUT_WIDTH, OUTPUT_HEIGHT, 2, OUTPUT_SCALE);

	mark(outputForK5, 0, OUTPUT_WIDTH, OUTPUT_HEIGHT, 1, OUTPUT_SCALE);
	tabulate(functionForTabulateK5, outputForK5, NEIGHBOURHOOD, OUTPUT_WIDTH, OUTPUT_HEIGHT, 2, OUTPUT_SCALE);

	BMP_WriteFile(outputForK3, "K3.bmp");
	BMP_CHECK_ERROR(stderr, -5);

	BMP_WriteFile(outputForK4, "K4.bmp");
	BMP_CHECK_ERROR(stderr, -5);

	BMP_WriteFile(outputForK5, "K5.bmp");
	BMP_CHECK_ERROR(stderr, -5);

	BMP_Free(outputForK3);
	BMP_Free(outputForK4);
	BMP_Free(outputForK5);

	printf("Result writed to \"out.bmp\".\r\n");
	printf("After closing the program, open it manually.\r\n\r\n");
	(void)getchar();

	return 0;
}


/*********************************** BMP Implementation **********************************/
/****************************** http://qdbmp.sourceforge.net/  ***************************/
/**************************************************************
QDBMP - Quick n' Dirty BMP
v1.0.0 - 2007-04-07
https://github.com/cbraudo/qdbmp
The library supports the following BMP variants:
1. Uncompressed 32 BPP (alpha values are ignored)
2. Uncompressed 24 BPP
3. Uncompressed 8 BPP (indexed color)
QDBMP is free and open source software, distributed
under the MIT licence.
Copyright (c) 2007 Chai Braudo (braudo@gmail.com)
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**************************************************************/

#else

#ifndef UINT
#define UINT	unsigned long int
#endif

#ifndef USHORT
#define USHORT	unsigned short
#endif

#ifndef UCHAR
#define UCHAR	unsigned char
#endif

#define QDBMP_VERSION_MAJOR		1
#define QDBMP_VERSION_MINOR		0
#define QDBMP_VERSION_PATCH		1

typedef enum
{
	BMP_OK = 0,
	BMP_ERROR,
	BMP_OUT_OF_MEMORY,
	BMP_IO_ERROR,
	BMP_FILE_NOT_FOUND,
	BMP_FILE_NOT_SUPPORTED,
	BMP_FILE_INVALID,
	BMP_INVALID_ARGUMENT,
	BMP_TYPE_MISMATCH,
	BMP_ERROR_NUM
} BMP_STATUS;

typedef struct _BMP BMP;

BMP* BMP_Create(UINT width, UINT height, USHORT depth);
void			BMP_Free(BMP* bmp);

BMP* BMP_ReadFile(const char* filename);
void			BMP_WriteFile(BMP* bmp, const char* filename);

UINT			BMP_GetWidth(BMP* bmp);
UINT			BMP_GetHeight(BMP* bmp);
USHORT			BMP_GetDepth(BMP* bmp);

void			BMP_GetPixelRGB(BMP* bmp, UINT x, UINT y, UCHAR* r, UCHAR* g, UCHAR* b);
void			BMP_SetPixelRGB(BMP* bmp, UINT x, UINT y, UCHAR r, UCHAR g, UCHAR b);
void			BMP_GetPixelIndex(BMP* bmp, UINT x, UINT y, UCHAR* val);
void			BMP_SetPixelIndex(BMP* bmp, UINT x, UINT y, UCHAR val);

void			BMP_GetPaletteColor(BMP* bmp, UCHAR index, UCHAR* r, UCHAR* g, UCHAR* b);
void			BMP_SetPaletteColor(BMP* bmp, UCHAR index, UCHAR r, UCHAR g, UCHAR b);

BMP_STATUS		BMP_GetError();
const char* BMP_GetErrorDescription();


#define BMP_CHECK_ERROR( output_file, return_value ) \
if (BMP_GetError() != BMP_OK)													\
{																				\
	fprintf((output_file), "BMP error: %s\n", BMP_GetErrorDescription());	\
	return(return_value);														\
}


typedef struct _BMP_Header
{
	USHORT		Magic;
	UINT		FileSize;
	USHORT		Reserved1;
	USHORT		Reserved2;
	UINT		DataOffset;
	UINT		HeaderSize;
	UINT		Width;
	UINT		Height;
	USHORT		Planes;
	USHORT		BitsPerPixel;
	UINT		CompressionType;
	UINT		ImageDataSize;
	UINT		HPixelsPerMeter;
	UINT		VPixelsPerMeter;
	UINT		ColorsUsed;
	UINT		ColorsRequired;
} BMP_Header;

/* Private data structure */
struct _BMP
{
	BMP_Header	Header;
	UCHAR* Palette;
	UCHAR* Data;
};

static BMP_STATUS BMP_LAST_ERROR_CODE = (BMP_STATUS)0;

static const char* BMP_ERROR_STRING[] =
{
	"",
	"General error",
	"Could not allocate enough memory to complete the operation",
	"File input/output error",
	"File not found",
	"File is not a supported BMP variant (must be uncompressed 8, 24 or 32 BPP)",
	"File is not a valid BMP image",
	"An argument is invalid or out of range",
	"The requested action is not compatible with the BMP's type"
};

#define BMP_PALETTE_SIZE	( 256 * 4 )

int		ReadHeader(BMP* bmp, FILE* f);
int		WriteHeader(BMP* bmp, FILE* f);

int		ReadUINT(UINT* x, FILE* f);
int		ReadUSHORT(USHORT* x, FILE* f);

int		WriteUINT(UINT x, FILE* f);
int		WriteUSHORT(USHORT x, FILE* f);

BMP* BMP_Create(UINT width, UINT height, USHORT depth)
{
	BMP* bmp;
	int		bytes_per_pixel = depth >> 3;
	UINT	bytes_per_row;

	if (height <= 0 || width <= 0)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return NULL;
	}

	if (depth != 8 && depth != 24 && depth != 32)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_SUPPORTED;
		return NULL;
	}

	bmp = (BMP*)calloc(1, sizeof(BMP));
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		return NULL;
	}

	bmp->Header.Magic = 0x4D42;
	bmp->Header.Reserved1 = 0;
	bmp->Header.Reserved2 = 0;
	bmp->Header.HeaderSize = 40;
	bmp->Header.Planes = 1;
	bmp->Header.CompressionType = 0;
	bmp->Header.HPixelsPerMeter = 0;
	bmp->Header.VPixelsPerMeter = 0;
	bmp->Header.ColorsUsed = 0;
	bmp->Header.ColorsRequired = 0;

	bytes_per_row = width * bytes_per_pixel;
	bytes_per_row += (bytes_per_row % 4 ? 4 - bytes_per_row % 4 : 0);

	bmp->Header.Width = width;
	bmp->Header.Height = height;
	bmp->Header.BitsPerPixel = depth;
	bmp->Header.ImageDataSize = bytes_per_row * height;
	bmp->Header.FileSize = bmp->Header.ImageDataSize + 54 + (depth == 8 ? BMP_PALETTE_SIZE : 0);
	bmp->Header.DataOffset = 54 + (depth == 8 ? BMP_PALETTE_SIZE : 0);

	if (bmp->Header.BitsPerPixel == 8)
	{
		bmp->Palette = (UCHAR*)calloc(BMP_PALETTE_SIZE, sizeof(UCHAR));
		if (bmp->Palette == NULL)
		{
			BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
			free(bmp);
			return NULL;
		}
	}
	else
	{
		bmp->Palette = NULL;
	}

	bmp->Data = (UCHAR*)calloc(bmp->Header.ImageDataSize, sizeof(UCHAR));
	if (bmp->Data == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		free(bmp->Palette);
		free(bmp);
		return NULL;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return bmp;
}

void BMP_Free(BMP* bmp)
{
	if (bmp == NULL)
	{
		return;
	}

	if (bmp->Palette != NULL)
	{
		free(bmp->Palette);
	}

	if (bmp->Data != NULL)
	{
		free(bmp->Data);
	}

	free(bmp);

	BMP_LAST_ERROR_CODE = BMP_OK;
}

BMP* BMP_ReadFile(const char* filename)
{
	BMP* bmp;
	FILE* f;

	if (filename == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return NULL;
	}

	bmp = (BMP*)calloc(1, sizeof(BMP));
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		return NULL;
	}

	f = fopen(filename, "rb");
	if (f == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_FOUND;
		free(bmp);
		return NULL;
	}

	if (ReadHeader(bmp, f) != BMP_OK || bmp->Header.Magic != 0x4D42)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_INVALID;
		fclose(f);
		free(bmp);
		return NULL;
	}

	if ((bmp->Header.BitsPerPixel != 32 && bmp->Header.BitsPerPixel != 24 && bmp->Header.BitsPerPixel != 8)
		|| bmp->Header.CompressionType != 0 || bmp->Header.HeaderSize != 40)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_SUPPORTED;
		fclose(f);
		free(bmp);
		return NULL;
	}

	if (bmp->Header.BitsPerPixel == 8)
	{
		bmp->Palette = (UCHAR*)malloc(BMP_PALETTE_SIZE * sizeof(UCHAR));
		if (bmp->Palette == NULL)
		{
			BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
			fclose(f);
			free(bmp);
			return NULL;
		}

		if (fread(bmp->Palette, sizeof(UCHAR), BMP_PALETTE_SIZE, f) != BMP_PALETTE_SIZE)
		{
			BMP_LAST_ERROR_CODE = BMP_FILE_INVALID;
			fclose(f);
			free(bmp->Palette);
			free(bmp);
			return NULL;
		}
	}
	else
	{
		bmp->Palette = NULL;
	}

	bmp->Data = (UCHAR*)malloc(bmp->Header.ImageDataSize);
	if (bmp->Data == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_OUT_OF_MEMORY;
		fclose(f);
		free(bmp->Palette);
		free(bmp);
		return NULL;
	}

	if (fread(bmp->Data, sizeof(UCHAR), bmp->Header.ImageDataSize, f) != bmp->Header.ImageDataSize)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_INVALID;
		fclose(f);
		free(bmp->Data);
		free(bmp->Palette);
		free(bmp);
		return NULL;
	}

	fclose(f);

	BMP_LAST_ERROR_CODE = BMP_OK;

	return bmp;
}

void BMP_WriteFile(BMP* bmp, const char* filename)
{
	FILE* f;

	if (filename == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return;
	}

	f = fopen(filename, "wb");
	if (f == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_FILE_NOT_FOUND;
		return;
	}

	if (WriteHeader(bmp, f) != BMP_OK)
	{
		BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
		fclose(f);
		return;
	}

	if (bmp->Palette)
	{
		if (fwrite(bmp->Palette, sizeof(UCHAR), BMP_PALETTE_SIZE, f) != BMP_PALETTE_SIZE)
		{
			BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
			fclose(f);
			return;
		}
	}

	if (fwrite(bmp->Data, sizeof(UCHAR), bmp->Header.ImageDataSize, f) != bmp->Header.ImageDataSize)
	{
		BMP_LAST_ERROR_CODE = BMP_IO_ERROR;
		fclose(f);
		return;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;
	fclose(f);
}

UINT BMP_GetWidth(BMP* bmp)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return -1;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return (bmp->Header.Width);
}

UINT BMP_GetHeight(BMP* bmp)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return -1;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return (bmp->Header.Height);
}

USHORT BMP_GetDepth(BMP* bmp)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
		return -1;
	}

	BMP_LAST_ERROR_CODE = BMP_OK;

	return (bmp->Header.BitsPerPixel);
}

void BMP_GetPixelRGB(BMP* bmp, UINT x, UINT y, UCHAR* r, UCHAR* g, UCHAR* b)
{
	UCHAR* pixel;
	UINT	bytes_per_row;
	UCHAR	bytes_per_pixel;

	if (bmp == NULL || x < 0 || x >= bmp->Header.Width || y < 0 || y >= bmp->Header.Height)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}
	else
	{
		BMP_LAST_ERROR_CODE = BMP_OK;

		bytes_per_pixel = bmp->Header.BitsPerPixel >> 3;

		bytes_per_row = bmp->Header.ImageDataSize / bmp->Header.Height;

		pixel = bmp->Data + ((bmp->Header.Height - y - 1) * bytes_per_row + x * bytes_per_pixel);


		if (bmp->Header.BitsPerPixel == 8)
		{
			pixel = bmp->Palette + *pixel * 4;
		}

		if (r)	*r = *(pixel + 2);
		if (g)	*g = *(pixel + 1);
		if (b)	*b = *(pixel + 0);
	}
}

void BMP_SetPixelRGB(BMP* bmp, UINT x, UINT y, UCHAR r, UCHAR g, UCHAR b)
{
	UCHAR* pixel;
	UINT	bytes_per_row;
	UCHAR	bytes_per_pixel;

	if (bmp == NULL || x < 0 || x >= bmp->Header.Width || y < 0 || y >= bmp->Header.Height)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 24 && bmp->Header.BitsPerPixel != 32)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		BMP_LAST_ERROR_CODE = BMP_OK;

		bytes_per_pixel = bmp->Header.BitsPerPixel >> 3;

		bytes_per_row = bmp->Header.ImageDataSize / bmp->Header.Height;

		pixel = bmp->Data + ((bmp->Header.Height - y - 1) * bytes_per_row + x * bytes_per_pixel);

		*(pixel + 2) = r;
		*(pixel + 1) = g;
		*(pixel + 0) = b;
	}
}

void BMP_GetPixelIndex(BMP* bmp, UINT x, UINT y, UCHAR* val)
{
	UCHAR* pixel;
	UINT	bytes_per_row;

	if (bmp == NULL || x < 0 || x >= bmp->Header.Width || y < 0 || y >= bmp->Header.Height)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 8)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		BMP_LAST_ERROR_CODE = BMP_OK;

		bytes_per_row = bmp->Header.ImageDataSize / bmp->Header.Height;

		pixel = bmp->Data + ((bmp->Header.Height - y - 1) * bytes_per_row + x);


		if (val)	*val = *pixel;
	}
}

void BMP_SetPixelIndex(BMP* bmp, UINT x, UINT y, UCHAR val)
{
	UCHAR* pixel;
	UINT	bytes_per_row;

	if (bmp == NULL || x < 0 || x >= bmp->Header.Width || y < 0 || y >= bmp->Header.Height)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 8)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		BMP_LAST_ERROR_CODE = BMP_OK;

		bytes_per_row = bmp->Header.ImageDataSize / bmp->Header.Height;

		pixel = bmp->Data + ((bmp->Header.Height - y - 1) * bytes_per_row + x);

		*pixel = val;
	}
}

void BMP_GetPaletteColor(BMP* bmp, UCHAR index, UCHAR* r, UCHAR* g, UCHAR* b)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 8)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		if (r)	*r = *(bmp->Palette + index * 4 + 2);
		if (g)	*g = *(bmp->Palette + index * 4 + 1);
		if (b)	*b = *(bmp->Palette + index * 4 + 0);

		BMP_LAST_ERROR_CODE = BMP_OK;
	}
}

void BMP_SetPaletteColor(BMP* bmp, UCHAR index, UCHAR r, UCHAR g, UCHAR b)
{
	if (bmp == NULL)
	{
		BMP_LAST_ERROR_CODE = BMP_INVALID_ARGUMENT;
	}

	else if (bmp->Header.BitsPerPixel != 8)
	{
		BMP_LAST_ERROR_CODE = BMP_TYPE_MISMATCH;
	}

	else
	{
		*(bmp->Palette + index * 4 + 2) = r;
		*(bmp->Palette + index * 4 + 1) = g;
		*(bmp->Palette + index * 4 + 0) = b;

		BMP_LAST_ERROR_CODE = BMP_OK;
	}
}

BMP_STATUS BMP_GetError()
{
	return BMP_LAST_ERROR_CODE;
}

const char* BMP_GetErrorDescription()
{
	if (BMP_LAST_ERROR_CODE > 0 && BMP_LAST_ERROR_CODE < BMP_ERROR_NUM)
	{
		return BMP_ERROR_STRING[BMP_LAST_ERROR_CODE];
	}
	else
	{
		return NULL;
	}
}

int	ReadHeader(BMP* bmp, FILE* f)
{
	if (bmp == NULL || f == NULL)
	{
		return BMP_INVALID_ARGUMENT;
	}

	if (!ReadUSHORT(&(bmp->Header.Magic), f))			return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.FileSize), f))		return BMP_IO_ERROR;
	if (!ReadUSHORT(&(bmp->Header.Reserved1), f))		return BMP_IO_ERROR;
	if (!ReadUSHORT(&(bmp->Header.Reserved2), f))		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.DataOffset), f))		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.HeaderSize), f))		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.Width), f))			return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.Height), f))			return BMP_IO_ERROR;
	if (!ReadUSHORT(&(bmp->Header.Planes), f))		return BMP_IO_ERROR;
	if (!ReadUSHORT(&(bmp->Header.BitsPerPixel), f))	return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.CompressionType), f))	return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.ImageDataSize), f))	return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.HPixelsPerMeter), f))	return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.VPixelsPerMeter), f))	return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.ColorsUsed), f))		return BMP_IO_ERROR;
	if (!ReadUINT(&(bmp->Header.ColorsRequired), f))	return BMP_IO_ERROR;

	return BMP_OK;
}


int	WriteHeader(BMP* bmp, FILE* f)
{
	if (bmp == NULL || f == NULL)
	{
		return BMP_INVALID_ARGUMENT;
	}

	if (!WriteUSHORT(bmp->Header.Magic, f))			return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.FileSize, f))		return BMP_IO_ERROR;
	if (!WriteUSHORT(bmp->Header.Reserved1, f))		return BMP_IO_ERROR;
	if (!WriteUSHORT(bmp->Header.Reserved2, f))		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.DataOffset, f))		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.HeaderSize, f))		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.Width, f))			return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.Height, f))			return BMP_IO_ERROR;
	if (!WriteUSHORT(bmp->Header.Planes, f))		return BMP_IO_ERROR;
	if (!WriteUSHORT(bmp->Header.BitsPerPixel, f))	return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.CompressionType, f))	return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.ImageDataSize, f))	return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.HPixelsPerMeter, f))	return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.VPixelsPerMeter, f))	return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.ColorsUsed, f))		return BMP_IO_ERROR;
	if (!WriteUINT(bmp->Header.ColorsRequired, f))	return BMP_IO_ERROR;

	return BMP_OK;
}

int	ReadUINT(UINT* x, FILE* f)
{
	UCHAR little[4];

	if (x == NULL || f == NULL)
	{
		return 0;
	}

	if (fread(little, 4, 1, f) != 1)
	{
		return 0;
	}

	*x = (little[3] << 24 | little[2] << 16 | little[1] << 8 | little[0]);

	return 1;
}

int	ReadUSHORT(USHORT* x, FILE* f)
{
	UCHAR little[2];

	if (x == NULL || f == NULL)
	{
		return 0;
	}

	if (fread(little, 2, 1, f) != 1)
	{
		return 0;
	}

	*x = (little[1] << 8 | little[0]);

	return 1;
}

int	WriteUINT(UINT x, FILE* f)
{
	UCHAR little[4];

	little[3] = (UCHAR)((x & 0xff000000) >> 24);
	little[2] = (UCHAR)((x & 0x00ff0000) >> 16);
	little[1] = (UCHAR)((x & 0x0000ff00) >> 8);
	little[0] = (UCHAR)((x & 0x000000ff) >> 0);

	return (f && fwrite(little, 4, 1, f) == 1);
}

int	WriteUSHORT(USHORT x, FILE* f)
{
	UCHAR little[2];

	little[1] = (UCHAR)((x & 0xff00) >> 8);
	little[0] = (UCHAR)((x & 0x00ff) >> 0);

	return (f && fwrite(little, 2, 1, f) == 1);
}

#endif