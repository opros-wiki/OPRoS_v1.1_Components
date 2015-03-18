#pragma once

extern unsigned char *ReadBmp256 (const char *fileName, int &width, int &height, unsigned char palette[256][4]);
extern int WriteBmp256 (const char *fileName, int width, int height, unsigned char *image, unsigned char palette[256][4]);
extern void FreeImage (unsigned char *image);
