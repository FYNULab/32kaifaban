#ifndef _PNGDISPLAY_H
#define _PNGDISPLAY_H
#include "system.h"



//ʹ��GUI_PNG_Draw()��������PNGͼƬ�Ļ�
//ͼƬ�Ǽ��ص�RAM�еģ���˲��ܴ���PNGMEMORYSIZE
//ע�⣺��ʾPNGͼƬʱ�ڴ�����ʹ�õ�SRAM���ڴ����뺯�������
//PNGMEMORYSIZE���ܴ������Ǹ�SRAM������ڴ�ռ�-EMWIN������ڴ�ش�С
#define PNGMEMORYSIZE	300*1024	//ͼƬ��С������300kb

//����������ص�RAM�е�PNGͼƬʱ��ͼƬÿ�е��ֽ���
#define PNGPERLINESIZE	5*1024	

int displaypng(char *PNGFileName,u8 mode,u32 x,u32 y);
int displaypngex(char *PNGFileName,u8 mode,u32 x,u32 y);
void pngdisplay_demo(void);
#endif
