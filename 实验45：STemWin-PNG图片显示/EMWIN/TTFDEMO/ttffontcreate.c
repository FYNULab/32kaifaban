#include "ttffontcreate.h"
#include "ff.h"
#include "malloc.h"
#include "includes.h"				  
#include "usart.h"

static GUI_TTF_CS	Cs0,Cs1,Cs2,Cs3;

FIL TTFFontFile;

GUI_FONT TTF12_Font;
GUI_FONT TTF18_Font;
GUI_FONT TTF24_Font;
GUI_FONT TTF36_Font;

GUI_TTF_DATA TTFData;

//����TTF���壬��EMWINʹ��
//fxpath:���TTF�ֿ��·��
//����ֵ��0,�ɹ�������ֵ��ʧ��
int Create_TTFFont(u8 *fxpath) 
{
	int result;
	u16 bread;
	char *TtfBuffer;
	
	CPU_SR_ALLOC();


	result = f_open(&TTFFontFile,(const TCHAR*)fxpath,FA_READ);	//���ֿ��ļ�
	if(result != FR_OK) return 1;
	
	if(TTFFontFile.fsize>500*1024) return 2; //�ļ�����500k������!
	else
	{
		TtfBuffer=mymalloc(SRAMEX,TTFFontFile.fsize);//�����ڴ�
		if(TtfBuffer==NULL) return 3;//�ڴ�����ʧ��
	}

	//��ȡ��������

	OS_CRITICAL_ENTER();	//�ٽ���
		
	result = f_read(&TTFFontFile,TtfBuffer,TTFFontFile.fsize,(UINT *)&bread); //��ȡ����
	if(result != FR_OK) return 4;	//�ļ���ʧ�ܣ�����
	
	f_close(&TTFFontFile);	//�ر�TTFFointFile�ļ�

	OS_CRITICAL_EXIT();	//�˳��ٽ���

	
	TTFData.pData=TtfBuffer;	//ָ���ļ���ַ
	TTFData.NumBytes=TTFFontFile.fsize; //�ļ���С
	
	Cs0.pTTF		= &TTFData;	
	Cs0.PixelHeight	= 12;
	Cs0.FaceIndex	= 0;
	
	Cs1.pTTF		= &TTFData;
	Cs1.PixelHeight	= 18;
	Cs1.FaceIndex	= 0;
	
	Cs2.pTTF		= &TTFData;
	Cs2.PixelHeight	= 24;
	Cs2.FaceIndex	= 0;
	
	Cs3.pTTF		= &TTFData;
	Cs3.PixelHeight	= 36;
	Cs3.FaceIndex	= 0;
	
	result = GUI_TTF_CreateFont(&TTF12_Font,&Cs0);	//��������
	if(result) return 5;	//���崴��ʧ��
	result = GUI_TTF_CreateFont(&TTF18_Font,&Cs1);	//��������
	if(result) return 5;	//���崴��ʧ��
	result = GUI_TTF_CreateFont(&TTF24_Font,&Cs2);	//��������
	if(result) return 5;	//���崴��ʧ��
	result = GUI_TTF_CreateFont(&TTF36_Font,&Cs3);	//��������
	if(result) return 5;	//���崴��ʧ��

	return 0;
}

void STemWin_TTFFont_Demo(void)
{
	int result;
	result=Create_TTFFont("0:/EMWINFONT/TTF/calibrib.ttf");
	printf("result=%d\r\n",result);
	if(result) printf("TTF���崴��ʧ��\r\n");
	
	GUI_SetBkColor(GUI_CYAN);
	GUI_SetColor(GUI_RED);
	GUI_Clear();
	GUI_SetFont(&TTF12_Font);
	GUI_DispStringAt("PRECHIN STemWin TTF Font Test ",5,10);
	GUI_SetFont(&TTF18_Font);
	GUI_DispStringAt("0123456789ABCDEF",5,25);
	GUI_SetFont(&TTF24_Font);
	GUI_DispStringAt("0123456789ABCDEF",5,45);
	GUI_SetFont(&TTF36_Font);
	GUI_DispStringAt("0123456789ABCDEF!",5,70);
	while(1)
	{
		GUI_Delay(100);
	}
}



