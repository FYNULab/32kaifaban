#ifndef _JIFDISPLAY_H
#define _JIFDISPLAY_H
#include "system.h"



//ʹ��GUI_GIF_Draw()��������BMPͼƬ�Ļ�
//ͼƬ�Ǽ��ص�RAM�еģ���˲��ܴ���GIFMEMORYSIZE
//ע�⣺��ʾGIFͼƬʱ�ڴ�����ʹ�õ�SRAM���ڴ����뺯�������
//GIFMEMORYSIZE���ܴ������Ǹ�SRAM����Ŀռ�-EMWIN������ڴ�ش�С
#define GIFMEMORYSIZE	500*1024	//ͼƬ��С������500kb

//����������ص�RAM�е�GIFͼƬʱ��ͼƬÿ�е��ֽ���
#define GIFPERLINESIZE	2*1024	

int displaygif(char *GIFFileName,u8 mode,u32 x,u32 y,int member,int denom);
int displaygifex(char *GIFFileName,u8 mode,u32 x,u32 y,int member,int denom);
void gifdisplay_demo(void);
#endif
