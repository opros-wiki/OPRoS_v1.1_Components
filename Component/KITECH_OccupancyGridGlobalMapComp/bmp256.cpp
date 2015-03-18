#if defined(WIN32)
#pragma warning (Disable:4996)
#endif

#include <stdio.h>
#include <string.h>

#if defined(WIN32)
#include <windows.h>
#endif


#pragma pack( push, packing_alignments, 1 )

struct BMP_FILE_HEAD { 
	short bfType;
	long  bfSize;
	short bfReserved0;
	short bfReserved2;
	long  bfOffBits;
};

struct BMP_INFO_HEAD{ 
	long  biSize;
	long  biWidth;
	long  biHeight;
	short biPlanes;
	short biBitCount;
	long  biCompression;
	long  biSizeImage;
	long  biXPelsPerMeter;
	long  biYPelsPerMeter;
	long  biClrUsed;
	long  biClrImportant;
};

#if defined(_OPROS_LINUX_)
struct RGBQUAD {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
};
#endif

#pragma pack( pop, packing_alignments )


unsigned char *ReadBmp256(const char *fileName, int &width, int &height, unsigned char palette[256][4])
{
	FILE *fp = fopen (fileName, "rb");
	if (fp == NULL) {
		printf ("BMP ERROR: Error opening %s. \n", fileName);
		return NULL;
	}
	BMP_FILE_HEAD BMPh;
	int ret = fread( (char *)&BMPh, 1, sizeof(BMP_FILE_HEAD), fp);
	if (ret!=sizeof(BMP_FILE_HEAD) || BMPh.bfType!='MB' ) {
		printf ("BMP ERROR: Error reading %s's header.\n", fileName);
		if (fp) fclose (fp);
		return NULL;
	}
	BMP_INFO_HEAD BMPih;
	ret = fread( (char *)&BMPih, 1, sizeof(BMP_INFO_HEAD), fp);
	if (ret!=sizeof(BMP_INFO_HEAD) || BMPih.biPlanes!=1 || BMPih.biBitCount!=8 || BMPih.biCompression!=0){
		printf ("BMP ERROR: Not supported format. \n");
		if (fp) fclose (fp);
		return NULL;
	}

	ret = fread (palette, 1, 256 * 4, fp);
	if (ret != 256*4){
		printf ("BMP ERROR: palette. \n");
		if (fp) fclose (fp);
		return NULL;
	}
	int gap = (int)(BMPih.biWidth%4);
	if (gap) gap = 4-gap;

	unsigned char *image = new unsigned char [BMPih.biHeight * BMPih.biWidth];
	if (NULL == image) {
		printf ("BMP ERROR: Memory allocating error [image]\n");
		if (fp) fclose (fp);
		return NULL;
	}
	fseek (fp, BMPh.bfOffBits, SEEK_SET);
	for (int i=BMPih.biHeight-1; 0<=i; i--) {
		ret = fread (image + BMPih.biWidth*i, 1, BMPih.biWidth, fp);
		if (ret != BMPih.biWidth){
			printf ("BMP ERROR: read image. \n");
			if (fp) fclose (fp);
			return NULL;
		}
		if (gap){
			char tmp[4];
			ret = fread (tmp, 1, gap, fp);
		}
	}
	width = BMPih.biWidth;
	height = BMPih.biHeight;

	fclose(fp);
	return image;
}

int WriteBmp256 (const char *fileName, int width, int height, unsigned char *image, unsigned char palette[256][4])
{
	BMP_FILE_HEAD BMPh;
	BMP_INFO_HEAD BMPih;
	RGBQUAD Palette[256];
	FILE *fp;

	if ((fp = fopen (fileName, "wb")) == NULL) {
		printf ("BMP ERROR: Error opening %s. \n", fileName);
		return -1;
	}

	memset (&BMPh, 0, sizeof (BMP_FILE_HEAD));
	memset (&BMPih, 0, sizeof (BMP_INFO_HEAD));

	BMPh.bfType = 'MB';
	BMPh.bfSize = sizeof (BMP_FILE_HEAD) + sizeof (BMP_INFO_HEAD) + sizeof (Palette) + (width*height);
	BMPh.bfOffBits = sizeof (BMP_FILE_HEAD) + sizeof (BMP_INFO_HEAD) + sizeof (Palette);
	int ret = fwrite( (char *)&BMPh, 1, sizeof(BMP_FILE_HEAD), fp);
	if (ret!=sizeof(BMP_FILE_HEAD) || BMPh.bfType!='MB' ) {
		printf ("BMP ERROR: Error writing %s's file header.\n", fileName);
		if (fp) fclose (fp);
		return -1;
	}

	BMPih.biSize = sizeof (BMP_INFO_HEAD);
	BMPih.biWidth = width;
	BMPih.biHeight = height;
	BMPih.biSizeImage = BMPih.biWidth * BMPih.biHeight;
	BMPih.biPlanes = 1;
	BMPih.biBitCount = 8;
	ret = fwrite( (char *)&BMPih, 1, sizeof(BMP_INFO_HEAD), fp);
	if (ret!=sizeof(BMP_INFO_HEAD) || BMPih.biPlanes!=1 || BMPih.biBitCount!=8 || BMPih.biCompression!=0){
		printf ("BMP ERROR: Error writing %s's info header.\n", fileName);
		if (fp) fclose (fp);
		return -1;
	}

	ret = fwrite (palette, 1, 256 * 4, fp);
	if (ret != 256*4){
		printf ("BMP ERROR: Error writing palette. \n");
		if (fp) fclose (fp);
		return -1;
	}

	int gap = (int)(BMPih.biWidth%4);
	if (gap) gap = 4-gap;

	fseek (fp, BMPh.bfOffBits, SEEK_SET);
	for (int i = BMPih.biHeight - 1; 0 <= i; i--) {
		ret = fwrite (image + BMPih.biWidth*i, 1, BMPih.biWidth, fp);
		if (ret != BMPih.biWidth){
			printf ("BMP ERROR: Error writing image. \n");
			if (fp) fclose (fp);
			return -1;
		}
		if (gap){
			char tmp[4];
			ret = fwrite (tmp, 1, gap, fp);
		}
	}

	fclose(fp);
	return 0;
}

void FreeImage (unsigned char *image)
{
	if (image) delete [] image;
}
