#include "checkboxdemo.h"
#include "GUI.h"
#include "DIALOG.h"
#include "led.h"
#include "beep.h"

//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Check box sample",   0,   0,  0, 320, 480, FRAMEWIN_CF_MOVEABLE},
  { TEXT_CreateIndirect,     "Enabled:",           0,    5,  10, 120,   0 },
  { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK0,    5,  30, 120,   0 },
  { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK1,    5,  60, 120,   0 },
  { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK2,    5,  90, 120,   20 },
  { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK3,    5, 125, 120,   26 },
  { TEXT_CreateIndirect,     "Disabled:",          0,  120,  10, 120,   0 },
  { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK4,  120,  30, 120,   0 },
  { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK5,  120,  60, 120,   0 },
  { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK6,  120,  90, 120,   26 },
  { CHECKBOX_CreateIndirect, 0,        GUI_ID_CHECK7,  120, 125, 120,   26 },
  { BUTTON_CreateIndirect,   "OK",         GUI_ID_OK,   10, 165,  80,  30 },
  { BUTTON_CreateIndirect,   "Cancel", GUI_ID_CANCEL,  120, 165,  80,  30 },
};

//��ѡ���ı�
static const char* _apLabel[]={
	"Default",
	"3 States",
	"Box XL",
	"Box XXL"
};

//�ص�����
static void _cbCallback(WM_MESSAGE *pMsg)
{
	int i;
	int NCode,Id;
	WM_HWIN hDlg,hItem;
	hDlg = pMsg->hWin;
	switch(pMsg->MsgId)
	{
		case WM_INIT_DIALOG:
			hItem = WM_GetDialogItem(hDlg,GUI_ID_CHECK0);	//��ȡCHECKBOX�ľ��
			for(i=0;i<8;i++)
			{
				int Index=i%4;
				hItem = WM_GetDialogItem(hDlg,GUI_ID_CHECK0+i); //��ȡCHECKBOX�ľ��
				CHECKBOX_SetText(hItem,_apLabel[Index]);
				switch(Index)
				{
					case 1:
						CHECKBOX_SetNumStates(hItem,3);
						CHECKBOX_SetImage(hItem,&_abmBar[0],CHECKBOX_BI_INACTIV_3STATE);
						CHECKBOX_SetImage(hItem,&_abmBar[1],CHECKBOX_BI_ACTIV_3STATE);
						CHECKBOX_SetState(hItem,2);
						break;
					case 2:
						CHECKBOX_SetState(hItem,1);
						CHECKBOX_SetImage(hItem,&_abmXL[0],CHECKBOX_BI_INACTIV_CHECKED);
						CHECKBOX_SetImage(hItem,&_abmXL[1],CHECKBOX_BI_ACTIV_CHECKED);
						CHECKBOX_SetFont(hItem,&GUI_FontComic18B_ASCII);
						break;
					case 3:
						CHECKBOX_SetState(hItem,1);
						CHECKBOX_SetImage(hItem,&_abmXXL[0],CHECKBOX_BI_INACTIV_CHECKED);
						CHECKBOX_SetImage(hItem,&_abmXXL[1],CHECKBOX_BI_ACTIV_CHECKED);
						CHECKBOX_SetFont(hItem,&GUI_FontComic24B_ASCII);
						break;
				}
				//ʧ�ܶԻ����ұߵ�����CHECKС����
				if(i>=4)
				{
					WM_DisableWindow(hItem);
				}	
			}
			break;
		case WM_NOTIFY_PARENT:
			Id=WM_GetId(pMsg->hWinSrc);	//С����ID			 
			NCode=pMsg->Data.v;			//֪ͨ����
			switch(NCode)
			{
				case WM_NOTIFICATION_RELEASED:
					if(Id==GUI_ID_CHECK0)
					{
						led1=!led1;
					}
					if(Id==GUI_ID_CHECK2)
					{
						beep=!beep;
					}
					if(Id==GUI_ID_OK)
					{
						GUI_EndDialog(hDlg,0);	//�رնԻ���
						WM_SetDesktopColor(GUI_BLUE);
						GUI_Delay(1);
						GUI_SetTextMode(GUI_TM_TRANS);
						GUI_SetFont(&GUI_Font24B_ASCII);
						GUI_SetColor(GUI_WHITE);
						GUI_DispStringHCenterAt("CheckBox Test OVER",120,180);
					}
					if(Id==GUI_ID_CANCEL)
					{
						GUI_EndDialog(hDlg,1);	//�رնԻ���
						WM_SetDesktopColor(GUI_RED);
						GUI_Delay(1);
						GUI_SetTextMode(GUI_TM_TRANS);
						GUI_SetFont(&GUI_Font24B_ASCII);
						GUI_SetColor(GUI_WHITE);
						GUI_DispStringHCenterAt("CheckBox Test OVER",120,180);
					}
					break;
			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

//CHECKBOXС������ʾDemo
void CheckBoxDemo(void)
{
	WM_SetDesktopColor(GUI_BLACK);
	GUI_ExecDialogBox(_aDialogCreate,GUI_COUNTOF(_aDialogCreate),&_cbCallback,0,0,0);
	//����������ʽ�Ի���
	//GUI_CreateDialogBox(_aDialogCreate,GUI_COUNTOF(_aDialogCreate),&_cbCallback,0,0,0);
}

void STemWin_CheckBox_Test(void)
{
	//����Ƥ��
#if 1
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
#endif
	CheckBoxDemo();
	while(1)
	{
		GUI_Delay(10);
	}
}


