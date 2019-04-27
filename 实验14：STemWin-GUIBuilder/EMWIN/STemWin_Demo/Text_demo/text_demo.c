#include "text_demo.h"
#include "GUI.h"


void STemWIN_Text_Test(void)   //STEMWIN�ı���ʾ���Գ���
{
	int i;
	char text[]="Hello!";
	GUI_RECT Rect={0,200,30,240};  //���������ʾ����
	//���廻��ģʽ
	GUI_WRAPMODE aWm[]={GUI_WRAPMODE_NONE,  //������
						GUI_WRAPMODE_CHAR,  //���ַ�����
						GUI_WRAPMODE_WORD};  //���ֻ���
	

	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetColor(GUI_YELLOW);
	GUI_DispString("Hello World!\n");

	GUI_SetFont(&GUI_Font8x16);
	GUI_SetPenSize(10);
	GUI_SetColor(GUI_RED);
	GUI_DrawLine(100,50,300,130);
	GUI_DrawLine(100,130,300,50);

	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_NORMAL);  //����ģʽ
	GUI_DispStringHCenterAt("GUI_TM_NORMAL",200,50);
	
	GUI_SetTextMode(GUI_TM_REV);  //��ת�ı�
	GUI_DispStringHCenterAt("GUI_TM_REV",200,66);
	
	GUI_SetTextMode(GUI_TM_TRANS);//͸���ı�
	GUI_DispStringHCenterAt("GUI_TM_TRANS",200,82);

	GUI_SetTextMode(GUI_TM_XOR);  //����ı�
	GUI_DispStringHCenterAt("GUI_TM_XOR",200,98);
	
	GUI_SetTextMode(GUI_TM_REV|GUI_TM_TRANS);  //��ת+͸��
	GUI_DispStringHCenterAt("GUI_TM_REV|GUI_TM_TRANS",200,114);

	GUI_SetTextMode(GUI_TM_TRANS);  //͸��
	for(i=0;i<3;i++)
	{
		GUI_SetColor(GUI_BLACK);	
		GUI_FillRectEx(&Rect);
		GUI_SetColor(GUI_WHITE);
		//��ָ������������ʾ�ַ����������л���ģʽ���л�
		GUI_DispStringInRectWrap(text,&Rect,GUI_TA_LEFT,aWm[i]);
		Rect.x0+=40;
		Rect.x1+=40;
	}

}

