#include "gifdisplay.h"
#include "EmWinHZFont.h"
#include "GUI.h"
#include "malloc.h"
#include "ff.h"
#include "tftlcd.h"
#include "includes.h"					


static FIL GIFFile;
static char gifBuffer[GIFPERLINESIZE];
/*******************************************************************
*
*       Static functions
*
********************************************************************
*/
/*********************************************************************
*
*       GifGetData
*
* Function description
*   This routine is called by GUI_GIF_DrawEx(). The routine is responsible
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
static int GifGetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int readaddress=0;
	FIL * phFile;
	UINT NumBytesRead;

	CPU_SR_ALLOC();
	
	phFile = (FIL *)p;
	
	if (NumBytesReq > sizeof(gifBuffer)) 
	{
		NumBytesReq = sizeof(gifBuffer);
	}

	//�ƶ�ָ�뵽Ӧ�ö�ȡ��λ��
	if(Off == 1) readaddress = 0;
	else readaddress=Off;
	
	OS_CRITICAL_ENTER();	//�ٽ���
	f_lseek(phFile,readaddress); 
	
	//��ȡ���ݵ���������
	f_read(phFile,gifBuffer,NumBytesReq,&NumBytesRead);
	
		
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	
	*ppData = (U8 *)gifBuffer;
	return NumBytesRead;//���ض�ȡ�����ֽ���
}

//��ָ��λ����ʾ���ص�RAM�е�GIFͼƬ
//GIFFileName:ͼƬ��SD�����������洢�豸�е�·��(���ļ�ϵͳ֧�֣�)
//mode:��ʾģʽ
//		0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
//		1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч��
//x:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
//y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
//member:  ���ű����ķ�����
//denom:���ű����ķ�ĸ��
//����ֵ:0 ��ʾ����,���� ʧ��
int displaygif(char *GIFFileName,u8 mode,u32 x,u32 y,int member,int denom)
{
	int i;
	u16 bread;
	char *gifbuffer;
	char result;
	int XSize,YSize;
	GUI_GIF_INFO GifInfo;
	GUI_GIF_IMAGE_INFO ImageInfo;
	float Xflag,Yflag;
	
	CPU_SR_ALLOC();

	result = f_open(&GIFFile,(const TCHAR*)GIFFileName,FA_READ);	//���ļ�
	//�ļ��򿪴�������ļ�����JPEGMEMORYSIZE
	if((result != FR_OK) || (GIFFile.fsize>GIFMEMORYSIZE)) 	return 1;
	
	gifbuffer=mymalloc(SRAMEX,GIFFile.fsize);
	if(gifbuffer == NULL) return 2;
	
	OS_CRITICAL_ENTER();	//�ٽ���
		
	result = f_read(&GIFFile,gifbuffer,GIFFile.fsize,(UINT *)&bread); //��ȡ����
	if(result != FR_OK) return 3;
	
	OS_CRITICAL_EXIT();//�˳��ٽ���
	
	GUI_GIF_GetInfo(gifbuffer,GIFFile.fsize,&GifInfo);	//��ȡGIFͼƬ��Ϣ
	
	XSize = GifInfo.xSize;	//��ȡGIFͼƬ��X���С
	YSize = GifInfo.ySize;	//��ȡGIFͼƬ��Y���С
	switch(mode)
	{
		case 0:	//��ָ��λ����ʾͼƬ
			if((member == 1) && (denom == 1)) //�������ţ�ֱ�ӻ���
			{
				//��ָ��λ����ʾJPEGͼƬ
				for(i=0;i<GifInfo.NumImages;i++)
				{
					GUI_GIF_DrawSub(gifbuffer,GIFFile.fsize,x,y,i);
					GUI_GIF_GetImageInfo(gifbuffer,GIFFile.fsize,&ImageInfo,i);
					GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10:100);//��ʱ
				}
			}else //����ͼƬ��Ҫ����
			{
				for(i=0;i<GifInfo.NumImages;i++)
				{
					GUI_GIF_DrawSubScaled(gifbuffer,GIFFile.fsize,x,y,i,member,denom);
					GUI_GIF_GetImageInfo(gifbuffer,GIFFile.fsize,&ImageInfo,i);
					GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10:100);//��ʱ
				}
			}
			break;
		case 1:	//��LCD�м���ʾͼƬ
			if((member == 1) && (denom == 1)) //�������ţ�ֱ�ӻ���
			{
				//��LCD�м���ʾͼƬ
				for(i=0;i<GifInfo.NumImages;i++)
				{
					GUI_GIF_DrawSub(gifbuffer,GIFFile.fsize,(tftlcd_data.width-XSize)/2-1,(tftlcd_data.height-YSize)/2-1,i);
					GUI_GIF_GetImageInfo(gifbuffer,GIFFile.fsize,&ImageInfo,i);
					GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10:100);//��ʱ
				}
			}else //����ͼƬ��Ҫ����
			{
				Xflag = (float)XSize*((float)member/(float)denom);
				Yflag = (float)YSize*((float)member/(float)denom);
				XSize = (tftlcd_data.width-(int)Xflag)/2-1;
				YSize = (tftlcd_data.height-(int)Yflag)/2-1;
				
				for(i=0;i<GifInfo.NumImages;i++)
				{
					GUI_GIF_DrawSubScaled(gifbuffer,GIFFile.fsize,XSize,YSize,i,member,denom);
					GUI_GIF_GetImageInfo(gifbuffer,GIFFile.fsize,&ImageInfo,i);
					GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10:100);//��ʱ
				}
			}
			break;
	}
	f_close(&GIFFile);		//�ر�JPEGFile�ļ�
	myfree(SRAMEX,gifbuffer);	//�ͷ��ڴ�
	return 0;
}

//��ָ��λ����ʾ������ص�RAM�е�GIFͼƬ(���ļ�ϵͳ֧�֣�����СRAM���Ƽ�ʹ�ô˷�����)
//GIFFileName:ͼƬ��SD�����������洢�豸�е�·��
//mode:��ʾģʽ
//		0 ��ָ��λ����ʾ���в���x,yȷ����ʾλ��
//		1 ��LCD�м���ʾͼƬ����ѡ���ģʽ��ʱ�����x,y��Ч��
//x:ͼƬ���Ͻ���LCD�е�x��λ��(������modeΪ1ʱ���˲�����Ч)
//y:ͼƬ���Ͻ���LCD�е�y��λ��(������modeΪ1ʱ���˲�����Ч)
//member:  ���ű����ķ�����
//denom:���ű����ķ�ĸ��
//����ֵ:0 ��ʾ����,���� ʧ��
int displaygifex(char *GIFFileName,u8 mode,u32 x,u32 y,int member,int denom)
{
	char result;
	int i;
	int XSize,YSize;
	float Xflag,Yflag;
	GUI_GIF_INFO GifInfo;
	GUI_GIF_IMAGE_INFO ImageInfo;

	result = f_open(&GIFFile,(const TCHAR*)GIFFileName,FA_READ);	//���ļ�
	//�ļ��򿪴���
	if(result != FR_OK) 	return 1;
		
	GUI_GIF_GetInfoEx(GifGetData,&GIFFile,&GifInfo);
	XSize = GifInfo.xSize;	//GIFͼƬX��С
	YSize = GifInfo.ySize;	//GIFͼƬY��С
	switch(mode)
	{
		case 0:	//��ָ��λ����ʾͼƬ
			if((member == 1) && (denom == 1)) //�������ţ�ֱ�ӻ���
			{
				//��ָ��λ����ʾBMPͼƬ
				for(i=0;i<GifInfo.NumImages;i++)
				{
					GUI_GIF_DrawSubEx(GifGetData,&GIFFile,x,y,i);
					GUI_GIF_GetImageInfoEx(GifGetData,&GIFFile,&ImageInfo,i);
					GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10:100);//��ʱ
				}
			}else //����ͼƬ��Ҫ����
			{
				for(i=0;i<GifInfo.NumImages;i++)
				{
					GUI_GIF_DrawSubScaledEx(GifGetData,&GIFFile,x,y,i,member,denom);
					GUI_GIF_GetImageInfoEx(GifGetData,&GIFFile,&ImageInfo,i);
					GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10:100);//��ʱ
				}
			}
			break;
		case 1:	//��LCD�м���ʾͼƬ
			if((member == 1) && (denom == 1)) //�������ţ�ֱ�ӻ���
			{
				//��LCD�м���ʾͼƬ
				for(i=0;i<GifInfo.NumImages;i++)
				{
					GUI_GIF_DrawSubEx(GifGetData,&GIFFile,(tftlcd_data.width-XSize)/2-1,(tftlcd_data.height-YSize)/2-1,i);
					GUI_GIF_GetImageInfoEx(GifGetData,&GIFFile,&ImageInfo,i);
					GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10:100);//��ʱ
				}
			}else //����ͼƬ��Ҫ����
			{
				Xflag = (float)XSize*((float)member/(float)denom);
				Yflag = (float)YSize*((float)member/(float)denom);
				XSize = (tftlcd_data.width-(int)Xflag)/2-1;
				YSize = (tftlcd_data.height-(int)Yflag)/2-1;
				
				for(i=0;i<GifInfo.NumImages;i++)
				{
					GUI_GIF_DrawSubScaledEx(GifGetData,&GIFFile,XSize,YSize,i,member,denom);
					GUI_GIF_GetImageInfoEx(GifGetData,&GIFFile,&ImageInfo,i);
					GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10:100);//��ʱ
				}
			}
			break;
	}

	
	f_close(&GIFFile);		//�ر�GIFFile�ļ�
	return 0;
}	

//GIFͼƬ��ʾ����
void gifdisplay_demo(void)
{
	
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetFont(&GUI_FontHZ16);
	GUI_SetColor(GUI_RED);
	GUI_Clear();
	while(1)
	{
		GUI_DispStringAt("Display GIF Picture",0,0);
		displaygif("0:/EMWINPICTURE/PICTURE/GIF/1.gif",0,0,0,1,1);
		GUI_Delay(1000);
		GUI_Clear();
	
		GUI_DispStringAt("Display GIF Picture 2/1",0,0);
		displaygif("0:/EMWINPICTURE/PICTURE/GIF/1.gif",1,0,0,2,1);	
		GUI_Delay(1000);
		GUI_Clear();
		
		GUI_DispStringAt("Display GIF Picture 1/2",0,0);
		displaygif("0:/EMWINPICTURE/PICTURE/GIF/1.gif",1,0,0,1,2);
		GUI_Delay(1000);
		GUI_Clear();
		
		GUI_DispStringAt("Display GIFEX Picture",0,0);
		displaygifex("0:/EMWINPICTURE/PICTURE/GIF/2.gif",0,0,0,1,1);
		GUI_Delay(1000);
		GUI_Clear();
		
		GUI_DispStringAt("Display GIFEX Picture 2/1",0,0);
		displaygifex("0:/EMWINPICTURE/PICTURE/GIF/2.gif",1,0,0,2,1);	
		GUI_Delay(1000);
		GUI_Clear();
	
		GUI_DispStringAt("Display GIFEX Picture 1/2",0,0);
		displaygifex("0:/EMWINPICTURE/PICTURE/GIF/2.gif",1,0,0,1,2);
		GUI_Delay(1000);
		GUI_Clear();
	}
}
	















