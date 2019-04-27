#include "pngdisplay.h"
#include "EmWinHZFont.h"
#include "GUI.h"
#include "malloc.h"
#include "ff.h"
#include "tftlcd.h"
#include "includes.h"				 


static FIL PNGFile;
//static char pngBuffer[PNGPERLINESIZE];
/*******************************************************************
*
*       Static functions
*
********************************************************************
*/
/*********************************************************************
*
*       PngGetData
*
* Function description
*   This routine is called by GUI_PNG_DrawEx(). The routine is responsible
*   for setting the data pointer to a valid data location with at least
*   one valid byte.
*
* Parameters:
*   p           - Pointer to application defined data.
*   NumBytesReq - Number of bytes requested.
*   ppData      - Pointer to data pointer. This pointer should be set to
*                 a valid location.
*   StartOfFile - If this flag is 1, the data pointer should be set to the
*                 beginning of the data stream.
*
* Return value:
*   Number of data bytes available.
*/
static int PngGetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int readaddress=0;
	FIL * phFile;
	U8 *pData;
	UINT NumBytesRead;
	CPU_SR_ALLOC();
	
	pData = (U8*)*ppData;
	phFile = (FIL *)p;
	
	//�ƶ�ָ�뵽Ӧ�ö�ȡ��λ��
	if(Off == 1) readaddress = 0;
	else readaddress=Off;
	
	OS_CRITICAL_ENTER();	//�ٽ���
	f_lseek(phFile,readaddress); 
	
	//��ȡ���ݵ���������
	f_read(phFile,pData,NumBytesReq,&NumBytesRead);
	
		
	OS_CRITICAL_EXIT();//�˳��ٽ���

	return NumBytesRead;//���ض�ȡ�����ֽ���
}

//��ָ��λ����ʾ���ص�RAM�е�PNGͼƬ(PNGͼƬ�������ţ�)
//PNGFileName:ͼƬ��SD�����������洢�豸�е�·��(���ļ�ϵͳ֧�֣�)
//mode:��ʾģʽ
//		0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
//		1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч��
//x:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
//y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
//����ֵ:0 ��ʾ����,���� ʧ��
int displaypng(char *PNGFileName,u8 mode,u32 x,u32 y)
{
	u16 bread;
	char *pngbuffer;
	char result;
	int XSize,YSize;
	
	CPU_SR_ALLOC();

	result = f_open(&PNGFile,(const TCHAR*)PNGFileName,FA_READ);	//���ļ�
	//�ļ��򿪴�������ļ�����JPEGMEMORYSIZE
	if((result != FR_OK) || (PNGFile.fsize>PNGMEMORYSIZE)) 	return 1;

	pngbuffer=mymalloc(SRAMEX,PNGFile.fsize);
	if(pngbuffer == NULL) return 2;
	
	OS_CRITICAL_ENTER();	//�ٽ���
		
	result = f_read(&PNGFile,pngbuffer,PNGFile.fsize,(UINT *)&bread); //��ȡ����
	if(result != FR_OK) return 3;
	
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	
	XSize = GUI_PNG_GetXSize(pngbuffer,PNGFile.fsize);	//��ȡPNGͼƬ��X���С
	YSize = GUI_PNG_GetYSize(pngbuffer,PNGFile.fsize);	//��ȡPNGͼƬ��Y���С
	switch(mode)
	{
		case 0:	//��ָ��λ����ʾͼƬ
			GUI_PNG_Draw(pngbuffer,PNGFile.fsize,x,y);//��ʾPNGͼƬ
			break;
		case 1:	//��LCD�м���ʾͼƬ
			GUI_PNG_Draw(pngbuffer,PNGFile.fsize,(tftlcd_data.width-XSize)/2-1,(tftlcd_data.height-YSize)/2-1);	
			break;
	}
	f_close(&PNGFile);		//�ر�PNGFile�ļ�
	myfree(SRAMEX,pngbuffer);
	return 0;
}

//��ָ��λ����ʾ������ص�RAM�е�PNGͼƬ(���ļ�ϵͳ֧�֣�����СRAM���Ƽ�ʹ�ô˷�����PNGͼƬ�������ţ�)
//PNGFileName:ͼƬ��SD�����������洢�豸�е�·��
//mode:��ʾģʽ
//		0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
//		1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч��
//x:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
//y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
//����ֵ:0 ��ʾ����,���� ʧ��
int displaypngex(char *PNGFileName,u8 mode,u32 x,u32 y)
{
	char result;
	int XSize,YSize;

	result = f_open(&PNGFile,(const TCHAR*)PNGFileName,FA_READ);	//���ļ�
	//�ļ��򿪴���
	if(result != FR_OK)	return 1;
	
	XSize = GUI_PNG_GetXSizeEx(PngGetData,&PNGFile);//PNGͼƬX��С
	YSize = GUI_PNG_GetYSizeEx(PngGetData,&PNGFile);//PNGͼƬY��С
	switch(mode)
	{
		case 0:	//��ָ��λ����ʾͼƬ
			GUI_PNG_DrawEx(PngGetData,&PNGFile,x,y);
			break;
		case 1:	//��LCD�м���ʾͼƬ
			GUI_PNG_DrawEx(PngGetData,&PNGFile,(tftlcd_data.width-XSize)/2-1,(tftlcd_data.height-YSize)/2-1);
			break;
	}
	f_close(&PNGFile);	//�ر�PNGFile�ļ�
	return 0;
}

//PNGͼƬ��ʾ����
void pngdisplay_demo(void)
{
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetFont(&GUI_FontHZ16);
	GUI_SetColor(GUI_RED);
	GUI_Clear();
	while(1)
	{
		GUI_DispStringAt("Display PNG Picture",0,0);
		displaypng("0:/EMWINPICTURE/PICTURE/PNG/�ʹ�.png",1,0,0);
		GUI_Delay(1000);
		GUI_Clear();

		displaypng("0:/EMWINPICTURE/PICTURE/PNG/CS.png",1,0,0);
		GUI_Delay(1000);
		GUI_Clear();	
		
		displaypng("0:/EMWINPICTURE/PICTURE/PNG/�㽶.png",1,0,0);
		GUI_Delay(1000);
		GUI_Clear();
		
		GUI_DispStringAt("Display PNGEX Picture",0,0);
		displaypngex("0:/EMWINPICTURE/PICTURE/PNG/¿.png",0,0,0);
		GUI_Delay(1000);
		GUI_Clear();
		
		displaypngex("0:/EMWINPICTURE/PICTURE/PNG/�ļ���.png",1,0,0);
		GUI_Delay(1000);
		GUI_Clear();
		
		displaypngex("0:/EMWINPICTURE/PICTURE/PNG/����.png",1,0,0);
		GUI_Delay(1000);
		GUI_Clear();
	}
}
