//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MSGBOX.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMsg *frmMsg;
//---------------------------------------------------------------------------
__fastcall TfrmMsg::TfrmMsg(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmMsg::btnID_okClick(TObject *Sender)
{
	if(edtSetWorkOrderNumber->Text=="")
		MessageBoxA(NULL,"請輸入完整資訊","WARNING", MB_ICONEXCLAMATION);
	else if(edtSetEmployeeID->Text=="")
		MessageBoxA(NULL,"請輸入完整資訊","WARNING", MB_ICONEXCLAMATION);
	else
		ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMsg::plSwitchClick(TObject *Sender)
{
	if(plSwitch->Caption=="V")
	{
		plSwitch->Caption = "^";
		frmMsg->Height = 452;
	}
	else
	{
		plSwitch->Caption = "V";
		frmMsg->Height = 269;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMsg::edtSetEmployeeIDKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key==13)
	{
		if(edtSetEmployeeID->Text.Length()== 6)
		{
			if(edtSetEmployeeID->Text.SubString(1,1).Pos("T"))
				btnID_ok->SetFocus();
			else edtSetEmployeeID->Text = "";
		}
		else if(edtSetEmployeeID->Text.Length()== 7)
		{
			if(edtSetEmployeeID->Text.SubString(1,2).Pos("SA")
				||edtSetEmployeeID->Text.SubString(1,2).Pos("UA")
				||edtSetEmployeeID->Text.SubString(1,2).Pos("UB")
				||edtSetEmployeeID->Text.SubString(1,2).Pos("XB")
				||edtSetEmployeeID->Text.SubString(1,2).Pos("XA"))
					btnID_ok->SetFocus();
			else edtSetEmployeeID->Text = "";
		}
		else edtSetEmployeeID->Text = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMsg::edtSetWorkOrderNumberKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	if(Key==13)
		edtSetEmployeeID->SetFocus();
}
//---------------------------------------------------------------------------





void __fastcall TfrmMsg::FormShow(TObject *Sender)
{
	/*if(frmMsg->Tag)
	{
		frmMsg->plWOInfo->Enabled = false;
		frmMsg->plWOInfo->Height  = 0;
		frmMsg->plSwitch->Enabled = false;
		frmMsg->plSwitch->Height  = 0;
		frmMsg->TouchKeyboard->Top = 182;
		frmMsg->Height = 160;
	}*/
}
//---------------------------------------------------------------------------


void __fastcall TfrmMsg::edtSetEmployeeIDExit(TObject *Sender)
{
	edtSetEmployeeID->Text = edtSetEmployeeID->Text.UpperCase();
	if(edtSetEmployeeID->Text.Length()== 6)
	{
		if(edtSetEmployeeID->Text.SubString(1,1).Pos("T"))
			btnID_ok->SetFocus();
		else edtSetEmployeeID->Text = "";
	}
	else if(edtSetEmployeeID->Text.Length()== 7)
	{
		if(edtSetEmployeeID->Text.SubString(1,2).Pos("SA")
			||edtSetEmployeeID->Text.SubString(1,2).Pos("UA")
			||edtSetEmployeeID->Text.SubString(1,2).Pos("UB")
			||edtSetEmployeeID->Text.SubString(1,2).Pos("XB")
			||edtSetEmployeeID->Text.SubString(1,2).Pos("XA"))
				btnID_ok->SetFocus();
		else edtSetEmployeeID->Text = "";
	}
	else edtSetEmployeeID->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmMsg::edtSetWorkOrderNumberExit(TObject *Sender)
{
	edtSetWorkOrderNumber->Text = edtSetWorkOrderNumber->Text.UpperCase();
	AnsiString strTemp = "";
	if(edtSetWorkOrderNumber->Text.Length()== 15)
	{
		strTemp = edtSetWorkOrderNumber->Text.SubString(4,1);
		if(!strTemp.Pos("-"))
		{
			edtSetWorkOrderNumber->Text = "";
			edtSetWorkOrderNumber->SetFocus();
		}
		strTemp = edtSetWorkOrderNumber->Text.SubString(5,11);
		if(strTemp.Pos("-"))
		{
			edtSetWorkOrderNumber->Text = "";
			edtSetWorkOrderNumber->SetFocus();
		}
	}
	else
	{
		edtSetWorkOrderNumber->Text = "";
		edtSetWorkOrderNumber->SetFocus();
	}
}
//---------------------------------------------------------------------------

