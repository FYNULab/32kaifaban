#include "color_demo.h"
#include "GUI.h"
#include "math.h"
#include "stdlib.h"

#define X_START 60
#define Y_START 40   //��ɫ����ʼ����

typedef struct
{
	int numbar;  //��ɫ������
	GUI_COLOR color;  //��ɫ
	const char *s;   //��ɫ�ַ�
}BAR_DATA;

static const BAR_DATA abardata[]=
{
	{2,GUI_RED,"RED"},
	{2,GUI_GREEN,"GREEN"},
	{2,GUI_BLUE,"BLUE"},
	{1,GUI_WHITE,"WHITE"},
	{2,GUI_YELLOW,"YELLOW"},
	{2,GUI_CYAN,"CYAN"},
	{2,GUI_MAGENTA,"MAGENTA"}
}; 
static const int acolorstart[]={GUI_BLACK,GUI_WHITE};



void STemWin_ColorBar_Test(void)   //��ɫ����ʾ����
{
	GUI_RECT Rect;
	int ystep;
	int i,j;
	int xsize,ysize;
	int numbar;    //�ܹ�����ɫ����  2+2+2+1+2+2+2=13
	int numcolor;  //�ܹ�����ɫ��  7
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);  //����͸��
	GUI_DispStringHCenterAt("PRECHIN ColorBar Test",200,0);

	//��ȡ��ʾ�ߴ�
	xsize=LCD_GetXSize();
	ysize=LCD_GetYSize();

	//��ȡ��ɫ������
	numcolor=GUI_COUNTOF(abardata);
	for(i=0,numbar=0;i<numcolor;i++)
	{
		numbar+=abardata[i].numbar;
	}
	ystep=(ysize-Y_START)/numbar;  //ÿһ����ɫ��Yֵ

	//��ʾ�ı�
	Rect.x0=0;
	Rect.x1=X_START-1;
	Rect.y0=Y_START;
	GUI_SetFont(&GUI_Font8x16);
	for(i=0;i<numcolor;i++)
	{
		Rect.y1=Rect.y0+ystep*abardata[i].numbar-1;
		GUI_DispStringInRect(abardata[i].s,&Rect,GUI_TA_LEFT|GUI_TA_VCENTER); //��ʾ�ı�
		Rect.y0=Rect.y1+1;
	}
	
	//��ʾ��ɫ��
	Rect.x0=X_START;
	Rect.x1=xsize-1;
	Rect.y0=Y_START;
	for(i=0;i<numcolor;i++)
	{
		for(j=0;j<abardata[i].numbar;j++)
		{
			Rect.y1=Rect.y0+ystep-1;
			//��ʾ��ɫ ˮƽ�ݶ�������ɫ
			GUI_DrawGradientH(Rect.x0,Rect.y0,Rect.x1,Rect.y1,acolorstart[j],abardata[i].color);
			Rect.y0=Rect.y1+1;
		}
	}
}

void STemWIN_Color_Test(void)   
{
	STemWin_ColorBar_Test();
	while(1)
	{
		GUI_Delay(10);
	}
}

