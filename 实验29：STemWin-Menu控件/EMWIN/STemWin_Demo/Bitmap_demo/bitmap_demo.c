#include "bitmap_demo.h"
#include "GUI.h"
#include "math.h"
#include "stdlib.h"


extern GUI_BITMAP bmprechin;

void STemWin_DrawBitmap_Test(void)   //λͼ��ʾ����
{
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);  //����͸��
	GUI_DispStringHCenterAt("PRECHIN Bitmap Test",200,0);
	GUI_EnableAlpha(1); //����Alpha
	GUI_SetAlpha(50);
	GUI_DrawBitmap(&bmprechin,0,0);
	GUI_EnableAlpha(0);  //�ر�Alpha
}

void STemWin_ZoomBitmap_Test()  //����ʾ��������λͼ
{
	int i;
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);  //����͸��
	GUI_DispStringHCenterAt("PRECHIN ZoomBitmap Test",200,0);
	while(1)
	{
		for(i=0;i<=1000;i+=100)
		{
			GUI_DrawBitmapEx(&bmprechin,0,0,0,0,i,i);
			GUI_Delay(1000);
		}
		GUI_Clear();
	}	
}

void STemWIN_Bitmap_Test(void)   
{
	STemWin_ZoomBitmap_Test();
	while(1)
	{
		GUI_Delay(10);
	}

}

