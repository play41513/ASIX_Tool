﻿// Module:
// Notices: Copyright(c) 2018 Leno.
// Purpose:
// Compiler: Embarcadero RAD Studio 2010
// OS: Windows 8.1 x64
// Version: 1.0.0
//---------------------------------------------------------------------------

#pragma hdrstop

#include "Main.h"
#include "Hub_fun.h"
#include "MSGBOX.h"
#include "USBDevConnectionInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;

Port_test *PORT = new Port_test();
USBDevConnectionInfo *USBCONInfo = new USBDevConnectionInfo();

DWORD TestTime = 0,BarcodeTimeOut = 0;

DWORD dwTestCount = 0;
AnsiString strLoadMAC 		= "";
AnsiString strLoadLANName 	= "";
AnsiString LOGFilePath = "";
AnsiString LogDiskPath = "";
AnsiString RDLOGFilePath = "";
AnsiString RDLogDiskPath = "";
DWORD numBarcodeResult;
AnsiString MACVerifty = "";
bool bEnd = false;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
/////////////////////////////////////////// 限制只能執行一次
	mx = CreateMutex( NULL, true, "AbacusPosAP" );
	if(GetLastError()){
		Application->Terminate(); //Finsh Application
	}
	bReWork = false;
	//
	ReadRegValue();
	ShowSettingForm();
}
void __fastcall TfrmMain::btn_startClick(TObject *Sender)
{
	btn_start->Enabled = false;
	lbBarcode->Font->Size = 32;
	lbBarcode->Caption = "請掃描MAC條碼";
	TestTime = 0;
	plTime->Caption = "0";
	BarcodeTimeOut = 0;
	MACVerifty = "";
	//
	if(btn_start->Caption.Pos("Start"))
	{
		btn_start->Caption = "Stop";
		plResult->Color = clCream;
		plResult->Caption = "Wait";
		Edit_resetMAC->Enabled = false;
		ckbAutoIncrease->Enabled = false;
		//
		// 按下Start同時將設定值寫進LOG檔
		writeLOG("",true);
		//
		Test_Step = GET_DUT;
		TestWorking = true;
	}
	else
	{
		btn_start->Caption = "Start";
		if(!ckbBarcodeVerify->Checked)
			Edit_resetMAC->Enabled = true;
		if(!ckbBarcode->Checked)
			ckbAutoIncrease->Enabled = true;
		TestWorking = false;
	}
	btn_start->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	ReleaseMutex(mx);
}
//---------------------------------------------------------------------------
bool TfrmMain::DUTB()
{
	frmMain->CHECKClick(NULL);
	frmMain->WRITEClick(NULL);
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::TimerMainTimer(TObject *Sender)
{
	if(current_mac->Caption == "")
		gLAN_NAME = PORT->GetLANName(true);
	else gLAN_NAME = PORT->GetLANName(false);
	if(frmMain->TestWorking)
	{
		frmMain->TestWorking = false;
		switch (Test_Step)
		{
			case GET_DUT :
				btn_start->Enabled = true;
				if(ckbBarcodeVerify->Checked && MACVerifty == "")
				{
					numBarcodeResult = 0;
					edtBarcodeMAC->Text = "";
					plBarcode->Height = 138;
					plBarcode->Top = (frmMain->Height/2)-(plBarcode->Height/2);
					plBarcode->Left = (frmMain->Width/2)-(plBarcode->Width/2);
					edtBarcodeMAC->SetFocus();
					while(true)
					{
						if(numBarcodeResult == BARCODE_FINISH)
						{
							TestTime = GetTickCount();
							lbBarcode->Font->Size = 32;
							lbBarcode->Caption = "請掃描MAC驗證條碼";
							plBarcode->Height = 0;
							BarcodeTimeOut = GetTickCount()+10000;
							frmMain->Refresh();
							break;
						}
						if(numBarcodeResult == BARCODE_CHANEL)
						{
							plBarcode->Height = 0;
							frmMain->Refresh();
							btn_startClick(NULL);
							bEnd = true;
							break;
						}
					   PORT->Delay(100);
					}
				}
				if(BarcodeTimeOut !=0)
				{
					if(GetTickCount() > BarcodeTimeOut)
					{
						lbBarcode->Font->Size = 20;
						lbBarcode->Caption = "未偵測到待測物請重新掃描MAC";
						ERROR_MSG =  "(!)TimeOUT";
						Test_Step = BURN_TEST_RESULT;
					}
					else
					{
						plTime->Caption = AnsiString(double(GetTickCount()-TestTime)/(double)1000);
                    }
				}
				else if(gLAN_NAME != "")
				{
					g_DevVPID = USBCONInfo->DevNameGetVPID(gLAN_NAME);
					if(g_DevVPID == "")
					{
						ERROR_MSG = "(!)GET VPID FAIL";
						Test_Step = BURN_TEST_RESULT;
					}
					else
					{
						btn_start->Enabled = false;
						plResult->Caption = "Testing";
						ERROR_MSG		  = "";
						TestTime = GetTickCount();
						Test_Step = MAC_LANNAME_VERIFY;
						if(ckbBarcodeVerify->Checked)
						{
							if(frmMain->current_mac->Caption != MACVerifty)
							{
								ERROR_MSG = "(!)MAC Verifty Failed";
								Test_Step = BURN_TEST_RESULT;
                            }
						}
					}
				}
				break;
			case MAC_LANNAME_VERIFY:
				if(lab_devicename->Caption.Pos("88772"))
				{
					Test_Step = ASIX_MAC_BURN;
				}
				else
				{
					Test_Step = USB3_TEST;
				}
				break;
			case USB3_TEST:
				if(USBCONInfo->GetDevUSB(g_DevVPID.SubString(5,4), g_DevVPID.SubString(14,4))){
					Test_Step = ASIX_MAC_BURN;
				}else
				{
					Test_Step = BURN_TEST_RESULT;
					ERROR_MSG = "(!)USB3.0 FAIL";
				}
				break;
			case ASIX_MAC_BURN:
				RegDriver();
				//check driver
				if(!PORT->CheckDir())
				{
					ERROR_MSG = "(!)未安裝燒錄驅動";
					Test_Step = BURN_TEST_RESULT;
					break;
				}
				else
				{
					if(lab_devicename->Caption.Pos("AX88772"))
					{
						OSVERSIONINFO osvi;
						memset(&osvi,0,sizeof(OSVERSIONINFO));
						osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
						GetVersionEx(&osvi);
						int windowsVer = osvi.dwMajorVersion *10 + osvi.dwMinorVersion;
						if(windowsVer == 62 )
						{
							ERROR_MSG = "(!)OS需為Windows 7";
							Test_Step = BURN_TEST_RESULT;
							break;
						}
					}
				}
				//
				if(ckbBarcode->Checked)
				{
					numBarcodeResult = 0;
					edtBarcodeMAC->Text = "";
					plBarcode->Height = 138;
					plBarcode->Top = (frmMain->Height/2)-(plBarcode->Height/2);
					plBarcode->Left = (frmMain->Width/2)-(plBarcode->Width/2);
					lbBarcode->Caption = "請掃描MAC燒錄條碼";
					edtBarcodeMAC->SetFocus();
					while(true)
					{
						if(numBarcodeResult == BARCODE_FINISH)
						{
							plBarcode->Height = 0;
							switch(CheckMAC(Edit_resetMAC->Text))
							{
								case ERROR_MAC_OUTRANGE:
									ERROR_MSG = "(!)MAC超出範圍設定";
									break;
								case ERROR_MAC_REPEAT:
									ERROR_MSG = "(!)此MAC已燒錄過";
									break;
								default:
									frmMain->Refresh();
									DUTB();
									break;
							}
							break;
						}
						if(numBarcodeResult == BARCODE_CHANEL)
						{
							plBarcode->Height = 0;
							ERROR_MSG = "(!)Barcode輸入取消";
							break;
						}
					   PORT->Delay(100);
					}
				}
				else
				{
					DUTB();
				}
				if(ERROR_MSG == "")
				{
					BarcodeTimeOut = GetTickCount()+1000;
					Test_Step = CHECK_BURN_MAC;
				}
				else Test_Step = BURN_TEST_RESULT;
				break;
			case CHECK_BURN_MAC:
				PORT->GetLANName(true);
				if(current_mac->Caption != Edit_resetMAC->Text)
				{
					if(GetTickCount() > BarcodeTimeOut)
					{
						current_mac->Font->Color = clRed;
						ERROR_MSG = "(!)燒錄失敗";
						Test_Step = BURN_TEST_RESULT;
					}
				}
				else Test_Step = BURN_TEST_RESULT;
				break;
			case BURN_TEST_RESULT:
				if(ERROR_MSG!="")
				{
					//
					frmMain->plResult->Color = clRed;
					plResult->Caption = ERROR_MSG;

					Test_Step = WAIT_DUT_PLUGOUT;
					if(!ERROR_MSG.Pos("輸入取消"))
						writeLOG(ERROR_MSG+Edit_resetMAC->Text,false);
					else writeLOG(ERROR_MSG,false);
					break;
				}
				else
				{
					//
					plResult->Color = clGreen;
					plResult->Caption = "PASS";

					Test_Step = WAIT_DUT_PLUGOUT;
					current_mac->Caption = Edit_resetMAC->Text;
					if(!writeLOG("PASS",false))
					{   //寫檔失敗
						ERROR_MSG =  "(!)寫檔失敗";
						MessageBoxA(NULL,"(!)隨身碟LOG檔寫入失敗","ERROR", MB_ICONEXCLAMATION);
                    }
					break;
				}
				break;
			case WAIT_DUT_PLUGOUT:
				btn_start->Enabled = true;
				if(gLAN_NAME == "")
				{
					MACVerifty = "";
					//
					if(ERROR_MSG == "")
						AutoIncrease();

					frmMain->plTime->Caption = 0;
					current_mac->Caption    = "";
					current_mac->Font->Color = clBlue;
					lab_devicename->Caption = "";
					lab_devicename->Font->Color = clBlue;
					plResult->Color = clCream;
					plResult->Caption = "Wait";

					BarcodeTimeOut = 0;
					Test_Step = GET_DUT;
				}
				break;
		}
		frmMain->Refresh();
		if(!bEnd)
			frmMain->TestWorking = true;
	}
	if(frmMain->plResult->Caption == "Testing")
	{
		if(TestTime == 0)
			TestTime = GetTickCount();
		plTime->Caption = AnsiString(double(GetTickCount()-TestTime)/(double)1000);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnLoadMACClick(TObject *Sender)
{
	strLoadMAC =  current_mac->Caption;
	if(strLoadMAC != "")
	{
		btnLoadMAC->Caption = strLoadMAC;
		btnLoadMAC->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnLoadNetworkCardNameClick(TObject *Sender)
{
	int i = lab_devicename->Caption.Pos("#")-1;
	if(i>0)
		strLoadLANName =  lab_devicename->Caption.SubString(1,i).Trim();
	else
		strLoadLANName =  lab_devicename->Caption.Trim();
	if(strLoadLANName != "")
	{
		btnLoadNetworkCardName->Caption = strLoadLANName;
		btnLoadNetworkCardName->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::edtBarcodeMACChange(TObject *Sender)
{
	TEdit* edt=(TEdit*)Sender;
	TCHAR HexChar[15];

	edt->Text = edt->Text.UpperCase();// 把字串轉成全大寫
	_tcscpy_s(HexChar, 15, AnsiString(edt->Text).c_str());
	for(int i=0;i <edt->Text.Length();i++)
	{
		if(HexChar[i] < 0x30 || HexChar[i] > 0x39)//非數字
		{
			if(HexChar[i] < 0x41 || HexChar[i] > 0x46)//非大寫字母a~f
			{
				edtBarcodeMAC->Text =
					edtBarcodeMAC->Text.Delete(edtBarcodeMAC->Text.Length(),1);
            }
		}
	}
	if(edtBarcodeMAC->Text.Length()> 12)
	  edtBarcodeMAC->Text = "";
	edt-> SelStart=edt-> Text.Length();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnBarcodeChanelClick(TObject *Sender)
{
	numBarcodeResult = BARCODE_CHANEL;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::edtBarcodeMACKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(edtBarcodeMAC->Text.Length()==12&&Key==13)
	{
		Edit_resetMAC->Text = edtBarcodeMAC->Text;
		MACVerifty = Edit_resetMAC->Text;
		numBarcodeResult = BARCODE_FINISH;
	}
	else if(edtBarcodeMAC->Text.Length()<12&&Key==13)
		edtBarcodeMAC->Text = "";
	else if(Key == 8)
		edtBarcodeMAC->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::READClick(TObject *Sender)
{
	HANDLE	hReadDevice;
	unsigned int bytes;
	unsigned int romSize = 512;
	//BYTE pbyBuf[512+8];
	unsigned int DelayCount=1;
	EEPROM_DATA	temp;
	AnsiString SS;
	unsigned int offset;
	BYTE	inBuf[3];
	BYTE	outBuf;
	//CString	temp;
	int i,j;

//Create Handle for Reader
if (lab_devicename->Caption.Pos("AX88772")) {
	hReadDevice = CreateFile (
						TEXT("\\\\.\\AX88772"),
						GENERIC_READ | GENERIC_WRITE,//FILE_READ_ATTRIBUTES | SYNCHRONIZE,
						FILE_SHARE_READ,
						NULL, // no SECURITY_ATTRIBUTES structure
						OPEN_EXISTING, // No special create flags
						FILE_ATTRIBUTE_NORMAL, // No special attributes
						NULL);
	}
else if (lab_devicename->Caption.Pos("AX88179")) {
	hReadDevice = CreateFile (
						TEXT("\\\\.\\AX88179"),
						GENERIC_READ | GENERIC_WRITE,//FILE_READ_ATTRIBUTES | SYNCHRONIZE,
						FILE_SHARE_READ,
						NULL, // no SECURITY_ATTRIBUTES structure
						OPEN_EXISTING, // No special create flags
						FILE_ATTRIBUTE_NORMAL, // No special attributes
						NULL);
	}

if (hReadDevice) 	//IOCTL_EEPROM_CHANGE_DELAY
	{
	if(!DeviceIoControl (hReadDevice,
		  IOCTL_EEPROM_CHANGE_DELAY,
		  &DelayCount, sizeof(unsigned int),
		  NULL, 0,
		  (unsigned long *)&bytes, NULL))
		{
		hReadDevice=NULL;
		//Memo2->Lines->Add("IOCTL_EEPROM_CHANGE_DELAY failed!");
		}
	else
	{
		//Memo2->Lines->Add("IOCTL_EEPROM_CHANGE_DELAY PASS!");
	}
	}

	//Read
	if (INVALID_HANDLE_VALUE == hReadDevice)
	{
		//Memo2->Lines->Add("Create ASIX Fail");
		hReadDevice=NULL;
	}
	else
	{
		if (lab_devicename->Caption.Pos("AX88772"))
		{
			if(!DeviceIoControl (hReadDevice,
				  IOCTL_EEPROM_READ_DATA,
				  NULL, 0,
				  &ReadBuf[0], 512,
				  (unsigned long *)&bytes, NULL))
			{
				Memo2->Lines->Add("IOCTL_EEPROM_READ_DATA failed!");
				CloseHandle(hReadDevice);
				hReadDevice=NULL;
			}
		}
		else if (lab_devicename->Caption.Pos("AX88179"))
		{
			for (i = 0; i < 512/2; i++)
			{
				inBuf[0] = 0x07;
				inBuf[1] = 1;
				inBuf[2] = i;
				if(!DeviceIoControl (hReadDevice,
					  IOCTL_WRITE_MAC_REGISTER,
					  &inBuf, sizeof(inBuf),
					  NULL, 0,
					  (unsigned long *)&bytes, NULL))
				{
					//Memo2->Lines->Add("IOCTL_WRITE_MAC_REGISTER 0x07 failed!");
					CloseHandle(hReadDevice);
					hReadDevice=NULL;
					break;
				}

				inBuf[0] = 0x0a;
				inBuf[1] = 1;
				inBuf[2] = 0x04;
				if(!DeviceIoControl (hReadDevice,
					  IOCTL_WRITE_MAC_REGISTER,
					  &inBuf, sizeof(inBuf),
					  NULL, 0,
					  (unsigned long *)&bytes, NULL))
				{
					//Memo2->Lines->Add("IOCTL_WRITE_MAC_REGISTER 0x0a failed!");
					CloseHandle(hReadDevice);
					hReadDevice=NULL;
					break;
				}

				inBuf[0] = 0x0a;
				inBuf[1] = 1;
				inBuf[2] = 0x10;
				for ( j = 0; i < 1000; j++)
				{
					if(!DeviceIoControl (hReadDevice,
						  IOCTL_READ_MAC_REGISTER,
						  &inBuf, sizeof(inBuf),
						  &outBuf, 1,
						  (unsigned long *)&bytes, NULL))
					{
						//Memo2->Lines->Add("IOCTL_READ_MAC_REGISTER 0x0a failed 1!");
						CloseHandle(hReadDevice);
						hReadDevice=NULL;
						break;
					}
					if (outBuf & 0x10 && j < 999)
						continue;
					else if (outBuf & 0x10 && j == 999)
					{
						//Memo2->Lines->Add("IOCTL_READ_MAC_REGISTER 0x0a failed 2!");
						CloseHandle(hReadDevice);
						hReadDevice=NULL;
						break;
					}
					else
					{
						break;
					}
			}
				if (INVALID_HANDLE_VALUE == hReadDevice) break;

				inBuf[0] = 0x08;
				inBuf[1] = 1;
				inBuf[2] = 0x00;
				if(!DeviceIoControl (hReadDevice,
					  IOCTL_READ_MAC_REGISTER,
					  &inBuf, sizeof(inBuf),
					  &outBuf, 1,
					  (unsigned long *)&bytes, NULL))
				{
						//Memo2->Lines->Add("IOCTL_READ_MAC_REGISTER 0x08 failed!");
						CloseHandle(hReadDevice);
						hReadDevice=NULL;
						break;
				}
				ReadBuf[i * 2 + 8] = outBuf;
				inBuf[1] = 1;

				inBuf[0] = 0x09;
				inBuf[2] = 0x00;
				if(!DeviceIoControl (hReadDevice,
					  IOCTL_READ_MAC_REGISTER,
					  &inBuf, sizeof(inBuf),
					  &outBuf, 1,
					  (unsigned long *)&bytes, NULL))
				{
						//Memo2->Lines->Add("IOCTL_READ_MAC_REGISTER 0x09 failed!");
						CloseHandle(hReadDevice);
						hReadDevice=NULL;
						break;
				}
				ReadBuf[i * 2 + 9] = outBuf;
			}
			if (hReadDevice)   //AX88179的TAG並沒有存在EEPROM
			{
			ReadBuf[0]=AX88179ROM[0];
			ReadBuf[1]=AX88179ROM[1];
			ReadBuf[2]=AX88179ROM[2];
			ReadBuf[3]=AX88179ROM[3];
			ReadBuf[4]=AX88179ROM[4];
			ReadBuf[5]=AX88179ROM[5];
			ReadBuf[6]=AX88179ROM[6];
			ReadBuf[7]=AX88179ROM[7];
			}
		}//Read AX88179 END
	}
if (hReadDevice)
	{
	//AX88772C 與 AX88179 MAC位址存放相同
	Memo2->Lines->Add("IOCTL_EEPROM_READ_DATA OK!");
	MR5=ReadBuf[8];
	MR4=ReadBuf[9];
	MR3=ReadBuf[10];
	MR2=ReadBuf[11];
	MR1=ReadBuf[12];
	MR0=ReadBuf[13];
	SS.printf("%02X%02X%02X%02X%02X%02X",
	MR5,MR4,MR3,MR2,MR1,MR0);
	current_mac->Caption = SS; //顯示當前設備號
	Memo2->Lines->Add(SS);

if (ReadBuf[2]==0xEC)
	{
		Memo2->Lines->Add("is EC do Test");
	}
else
	{
		Memo2->Lines->Add("not EC do Program");
		}

		CloseHandle(hReadDevice);
		hReadDevice=NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::WRITEClick(TObject *Sender)
{
	HANDLE	hControlDevice;
	//unsigned char MR5,MR4,MR3,MR2,MR1,MR0;
	unsigned int bytes;
	unsigned int romSize = 512;

	char szBuf[12];
	unsigned int DelayCount=1;
	AnsiString SS;
	bool Verify;
	int j;

	if (lab_devicename->Caption.Pos("AX88772")) {
		hControlDevice = CreateFile (
							TEXT("\\\\.\\AX88772"),
							GENERIC_READ | GENERIC_WRITE,//FILE_READ_ATTRIBUTES | SYNCHRONIZE,
							FILE_SHARE_READ,
							NULL, // no SECURITY_ATTRIBUTES structure
							OPEN_EXISTING, // No special create flags
							FILE_ATTRIBUTE_NORMAL, // No special attributes
							NULL);
		}
	else if (lab_devicename->Caption.Pos("AX88179")) {
		hControlDevice = CreateFile (
							TEXT("\\\\.\\AX88179"),
							GENERIC_READ | GENERIC_WRITE,//FILE_READ_ATTRIBUTES | SYNCHRONIZE,
							FILE_SHARE_READ,
							NULL, // no SECURITY_ATTRIBUTES structure
							OPEN_EXISTING, // No special create flags
							FILE_ATTRIBUTE_NORMAL, // No special attributes
							NULL);
		}

	if (INVALID_HANDLE_VALUE == hControlDevice) {
		Memo2->Lines->Add("Create ASIX Fail");
		hControlDevice=NULL;
		}
	else
		{
	//	Memo2->Lines->Add("Create ASIX OK");
		if (lab_devicename->Caption.Pos("AX88772")) {
			memcpy(WriteBuf,AX88772CROM,512);
			WriteBuf[8] =MA5;
			WriteBuf[9] =MA4;
			WriteBuf[10]=MA3;
			WriteBuf[11]=MA2;
			WriteBuf[12]=MA1;
			WriteBuf[13]=MA0;
			sprintf(szBuf,"%02X%02X%02X",MA2,MA1,MA0);
			WriteBuf[52]=szBuf[0];
			WriteBuf[54]=szBuf[1];
			WriteBuf[56]=szBuf[2];
			WriteBuf[58]=szBuf[3];
			WriteBuf[60]=szBuf[4];
			WriteBuf[62]=szBuf[5];
			}
		else if (lab_devicename->Caption.Pos("AX88179")) {
			romSize = 520;
			memcpy(WriteBuf,AX88179ROM,520);
			WriteBuf[8] =MA5;
			WriteBuf[9] =MA4;
			WriteBuf[10]=MA3;
			WriteBuf[11]=MA2;
			WriteBuf[12]=MA1;
			WriteBuf[13]=MA0;
			sprintf(szBuf,"%02X%02X%02X%02X%02X%02X",MA5,MA4,MA3,MA2,MA1,MA0);
			WriteBuf[86]=szBuf[0];
			WriteBuf[87]=szBuf[1];
			WriteBuf[88]=szBuf[2];
			WriteBuf[89]=szBuf[3];
			WriteBuf[90]=szBuf[4];
			WriteBuf[91]=szBuf[5];
			WriteBuf[92]=szBuf[6];
			WriteBuf[93]=szBuf[7];
			WriteBuf[94]=szBuf[8];
			WriteBuf[95]=szBuf[9];
			WriteBuf[96]=szBuf[10];
			WriteBuf[97]=szBuf[11];
			}
		}
	if (hControlDevice) 	//IOCTL_EEPROM_CHANGE_DELAY
		{
		if(!DeviceIoControl (hControlDevice,
			  IOCTL_EEPROM_CHANGE_DELAY,
			  &DelayCount, sizeof(unsigned int),
			  NULL, 0,
			  (unsigned long *)&bytes, NULL))
			{
			hControlDevice=NULL;
			//Memo2->Lines->Add("IOCTL_EEPROM_CHANGE_DELAY failed!");
			}
		else
		{
			//Memo2->Lines->Add("IOCTL_EEPROM_CHANGE_DELAY PASS!");
		}
		}

	if(hControlDevice)	// IOCTL_EEPROM_WRITE_DATA
		{
		unsigned int WRITE_SIZE=512;
		if (lab_devicename->Caption.Pos("AX88179"))
			WRITE_SIZE=520;
		if (!DeviceIoControl(hControlDevice,
							IOCTL_EEPROM_WRITE_DATA,
							&WriteBuf[0], WRITE_SIZE,
							NULL, 0, (unsigned long *)&bytes, NULL))
			{
			//Memo2->Lines->Add("IOCTL_EEPROM_WRITE_DATA failed!");
			CloseHandle(hControlDevice);
			hControlDevice=NULL;
			}
		else
			{
			//Memo2->Lines->Add("IOCTL_EEPROM_WRITE_DATA PASS!");
			}
		//紀錄本次燒錄的MAC ADDRESS
		//SETMAC.printf("%02X:%02X:%02X:%02X:%02X:%02X",MA5,MA4,MA3,MA2,MA1,MA0);
		}

	if(hControlDevice)	// Verify
		{
		Sleep(1000);
		READClick(NULL);
		Verify=true;
		for(j=0;j<512;j++)
			{
			if (WriteBuf[j]!=ReadBuf[j])
			{
				Verify=false;
			}
			}
		if (!Verify)
			{
			//Memo2->Lines->Add("EEPROM Verify failed!");
			//ERRMSG="EEPROM Verify failed!";
			//GETERRClick(NULL);
			CloseHandle(hControlDevice);
			hControlDevice=NULL;
			}
		}

	if (hControlDevice) 	//IOCTL_EEPROM_CHANGE_DELAY
	{
	if(!DeviceIoControl (hControlDevice,
		  IOCTL_EEPROM_CHANGE_DELAY,
		  &DelayCount, sizeof(unsigned int),
		  NULL, 0,
		  (unsigned long *)&bytes, NULL))
		{
		hControlDevice=NULL;
		Memo2->Lines->Add("IOCTL_EEPROM_CHANGE_DELAY failed!");
		}
	else
		Memo2->Lines->Add("IOCTL_EEPROM_CHANGE_DELAY PASS!");
	}
	if(hControlDevice)
		{
		CloseHandle(hControlDevice);
		hControlDevice=NULL;

		SS.printf("%02X:%02X:%02X:%02X:%02X:%02X",
		MA5,MA4,MA3,MA2,MA1,MA0);
		}
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::CHECKClick(TObject *Sender)
{
	AnsiString SS;

	Edit1->Text = Edit_resetMAC->Text.SubString(1,2);
	Edit2->Text = Edit_resetMAC->Text.SubString(3,2);
	Edit3->Text = Edit_resetMAC->Text.SubString(5,2);
	Edit4->Text = Edit_resetMAC->Text.SubString(7,2);
	Edit5->Text = Edit_resetMAC->Text.SubString(9,2);
	Edit6->Text = Edit_resetMAC->Text.SubString(11,2);

	MA5 = ("0x"+Edit1->Text).ToInt();
	MA4 = ("0x"+Edit2->Text).ToInt();
	MA3 = ("0x"+Edit3->Text).ToInt();
	MA2 = ("0x"+Edit4->Text).ToInt();
	MA1 = ("0x"+Edit5->Text).ToInt();
	MA0 = ("0x"+Edit6->Text).ToInt();
	ME2 = ("0x"+Edit7->Text).ToInt();
	ME1 = ("0x"+Edit8->Text).ToInt();
	ME0 = ("0x"+Edit9->Text).ToInt();
	SS.printf("%02X",MA5); Edit1->Text=SS;
	SS.printf("%02X",MA4); Edit2->Text=SS;
	SS.printf("%02X",MA3); Edit3->Text=SS;
	SS.printf("%02X",MA2); Edit4->Text=SS;
	SS.printf("%02X",MA1); Edit5->Text=SS;
	SS.printf("%02X",MA0); Edit6->Text=SS;
	SS.printf("%02X",ME2); Edit7->Text=SS;
	SS.printf("%02X",ME1); Edit8->Text=SS;
	SS.printf("%02X",ME0); Edit9->Text=SS;
}
//---------------------------------------------------------------------------
void TfrmMain::AutoIncrease(){
	AnsiString SS,MAC_1,MAC_2;
	int tmp_1,tmp_2;

	if(!ckbBarcode->Checked)
	{
		SS = Edit_resetMAC->Text.SubString(1,2)+Edit_resetMAC->Text.SubString(3,2)
						+Edit_resetMAC->Text.SubString(5,2);
		tmp_1 = HexToInt(SS);
		SS = Edit_resetMAC->Text.SubString(7,2)+Edit_resetMAC->Text.SubString(9,2)
					+Edit_resetMAC->Text.SubString(11,2);
		tmp_2 = HexToInt(SS);
		if(ckbAutoIncrease->Checked)
		{
			tmp_2++;
			MAC_2 = IntToHex(tmp_2, 6);
			if(MAC_2=="1000000")
			{
				MAC_2 = "000000";
				tmp_1++;
			}
			MAC_1 = IntToHex(tmp_1, 6);
			if(MAC_1=="1000000")
				MAC_1 = "000000";
			SS = MAC_1.SubString(1,2)+MAC_1.SubString(3,2)+MAC_1.SubString(5,2)+MAC_2.SubString(1,2)+MAC_2.SubString(3,2)+MAC_2.SubString(5,2);
			Edit_resetMAC->Text = SS;
			SetRegValue();
		}
	}
}
int TfrmMain::HexToInt(AnsiString HexStr)
{
	int iIndex, iHex, totalChar;
	TCHAR HexChar[8];

	HexStr.UpperCase(); // 把字串轉成全大寫

	_tcscpy_s(HexChar, 8, HexStr.c_str());
	iHex = 0;
	totalChar = HexStr.Length(); // 取得字串的長度
	for (iIndex = 0; iIndex < totalChar; iIndex++) {
		iHex <<= 4;
		if ((HexChar[iIndex] >= 0x30) && (HexChar[iIndex] <= 0x39)) {
			iHex += (HexChar[iIndex] - 0x30); // 把 0 - 9 字元轉成數字
		}
		else if ((HexChar[iIndex] >= 0x41) && (HexChar[iIndex] <= 0x46)) {
			iHex += (HexChar[iIndex] - 0x37); // ­把 A - F 字元轉成數字
		}
		else {
			iHex = 0;
			break;
		}
	}
	return iHex;
}

void TfrmMain::ReadRegValue()
{
	AnsiString value;
	TRegistry *reg = new TRegistry();

	try
	{
		reg->RootKey = HKEY_CURRENT_USER;
		reg->OpenKey("SOFTWARE", false);
		bool bl = reg->OpenKey("ASIX MAC-IN", true);
		bl = reg->OpenKey("v1.0", false);
		if(bl)
		{
			if(reg->ValueExists("MAC")){
				value = reg->ReadString("MAC");
				Edit_resetMAC->Text = value;
			}
			else Edit_resetMAC->Text = "00249B000000";

			if(reg->ValueExists("BarcodeMAC")){
				value = reg->ReadString("BarcodeMAC");
				if(value == "true") ckbBarcode->Checked = true;
				else ckbBarcode->Checked = false;
			}
			else ckbBarcode->Checked = true;

			if(reg->ValueExists("AutoAdd")){
				value = reg->ReadString("AutoAdd");
				if(value == "true") ckbAutoIncrease->Checked = true;
				else ckbAutoIncrease->Checked = false;
			}
			else ckbAutoIncrease->Checked = false;

			if(reg->ValueExists("WorkOrderNumber")){
				value = reg->ReadString("WorkOrderNumber");
				edtSetWorkOrderNumber->Text = value;
			}
			if(reg->ValueExists("EmployeeID")){
				value = reg->ReadString("EmployeeID");
				edtSetEmployeeID->Text = value;
			}
			//
			if(reg->ValueExists("START_CODE")){
				value = reg->ReadString("START_CODE");
				mskedBurntMacStart->Text = value;
			}
			else mskedBurntMacStart->Text = "000000000000";

			if(reg->ValueExists("LAST_CODE")){
				value = reg->ReadString("LAST_CODE");
				mskedBurntMacLast->Text = value;
			}
			else mskedBurntMacLast->Text = "FFFFFFFFFFFF";
		}
		else
		{
			Edit_resetMAC->Text = "00249B000000";
			ckbBarcode->Checked = true;
			ckbAutoIncrease->Checked = false;
		}
	}__finally {
		delete reg;
	}
}
void TfrmMain::SetRegValue(){
	TRegistry *reg = new TRegistry;
	AnsiString SS;
	reg->RootKey = HKEY_CURRENT_USER;
	reg->OpenKey("SOFTWARE", false);
	bool bl = reg->OpenKey("ASIX MAC-IN", true);
	bl = reg->OpenKey("v1.0", true);
	if(bl)
	{
		SS = Edit_resetMAC->Text;
		reg->WriteString("MAC", SS);
		SS = ckbBarcode->Checked ? "true":"false";
		reg->WriteString("BarcodeMAC",  SS);
		SS = ckbAutoIncrease->Checked ? "true":"false";
		reg->WriteString("AutoAdd",  SS);

		reg->WriteString("WorkOrderNumber", edtSetWorkOrderNumber->Text );
		reg->WriteString("EmployeeID", edtSetEmployeeID->Text );
		reg->WriteString("START_CODE", mskedBurntMacStart->Text );
		reg->WriteString("LAST_CODE", mskedBurntMacLast->Text );
	}
	delete reg;
}
bool TfrmMain::writeLOG(AnsiString Msg,bool bAction)
{
	bool bPASS[4] = {true,true,true,true};
	AnsiString SS,Temp;
	AnsiString time = FormatDateTime("mm-dd-yyyy hh:nn:ss", Now());
	if(bAction)
	{
		Temp = ckbAutoIncrease->Checked ? "true":"false";
		SS = "\r\n===========\r\n "
			+time
			+" [BurnMAC]"+Edit_resetMAC->Text
			+" [BarcodeMAC]"+ckbBarcode->Caption
			+" [AutoAdd]"+Temp
			+" [EmployeeID]"+edtSetEmployeeID->Text+"\n "
			+" [MACStartCode]"+mskedBurntMacStart->Text
			+" [MACLastCode]"+mskedBurntMacLast->Text
			+"\r\n=================================\r\n";
			bPASS[0] = SaveUserLogLine(RDLOGFilePath,SS);
			if(LogDiskPath.Pos("log") && ImgDisk->Visible)
			{
			   bPASS[1] = 	SaveUserLogLine(RDLogDiskPath,SS);
			}
			for(int i = 0 ; i < 2 ; i++)
				if(!bPASS[i]) return false;
	}
	else
	{
		if(Msg=="PASS")
		{
			dwTestCount++;
			plCount->Caption = dwTestCount;
		}
		AnsiString strTemp;
		strTemp.sprintf("%04d",dwTestCount);
		if(Msg.Pos("PASS"))
		{
			SS = strTemp+" [PASS]"+time
				+" [MAC]"+current_mac->Caption;
			SS+= " [MACStartCode]"+mskedBurntMacStart->Text+" [MACLastCode]"+mskedBurntMacLast->Text+" [WorkOrder]"+edtSetWorkOrderNumber->Text	+" [EmployeeID]"+edtSetEmployeeID->Text;
			SS += " [TEST-TIME]"+AnsiString(plTime->Caption)+"\r\n";
		}
		else
		{
			SS =  strTemp+" [FAIL]"+time
				+" [MAC]"+current_mac->Caption;
			SS+= " [MACStartCode]"+mskedBurntMacStart->Text+" [MACLastCode]"+mskedBurntMacLast->Text+" [WorkOrder]"+edtSetWorkOrderNumber->Text	+" [EmployeeID]"+edtSetEmployeeID->Text;
			SS += " [TEST-TIME]"+AnsiString(plTime->Caption)+" [ERROR]"+Msg+"\r\n";
		}
		bPASS[0] = SaveUserLogLine(RDLOGFilePath,SS);
		bPASS[1] = SaveUserLogLine(LOGFilePath,SS);
		if(LogDiskPath.Pos("log") && ImgDisk->Visible )
		{
			bPASS[2] = SaveUserLogLine(RDLogDiskPath,SS);
			bPASS[3] = SaveUserLogLine(LogDiskPath,SS);
			if(!bPASS[2] || !bPASS[3])
				ImgDisk->Visible = false;
		}
		for(int i = 0 ; i < 4 ; i++)
			if(!bPASS[i]) return false;
	}
	return true;
}
bool  TfrmMain::SaveUserLogLine(AnsiString FileName,AnsiString FileLine)
{
	FILE * fp;
	fp = fopen(FileName.c_str(),"a+");
	if(NULL == fp)
		return false;
	fseek( fp, 0, SEEK_END);
	fwrite(FileLine.c_str(),FileLine.Length(),1,fp); //寫入一筆資料
	fclose(fp);
	return true;
}
bool TfrmMain::CheckDiskName()
{
	DWORD DiskInfo = GetLogicalDrives();
	AnsiString DiskName,DiskVerifty = "DEVICE_LOG",SS;
	char Disk;
	for(int i =3;i<=25;i++)
	{
		if((DiskInfo&(0x01<<i))!=0)
		{
			char Volumelabel[20];
			DWORD SerialNumber;
			DWORD MaxCLength;
			DWORD FileSysFlag;
			char FileSysName[10];
			Disk = 0x41+i;
			DiskName = AnsiString(Disk)+":\\";
			GetVolumeInformation(DiskName.c_str(),Volumelabel,255,&SerialNumber,&MaxCLength,&FileSysFlag,FileSysName,255);
			if(!strcmp(Volumelabel,DiskVerifty.c_str()))
			{
				LogDiskPath = DiskName+"\\ASMP\\log\\"+edtSetWorkOrderNumber->Text+"\\ASIXTool";
				if(!DirectoryExists(LogDiskPath.c_str())){
					_mkdir( DiskName.c_str());
					SS = DiskName + "\\ASMP";
					_mkdir( SS.c_str());
					SS = DiskName + "\\ASMP\\log";
					_mkdir( SS.c_str());
					SS = DiskName + "\\ASMP\\log\\"+edtSetWorkOrderNumber->Text;
					_mkdir(SS.c_str());
					_mkdir( LogDiskPath.c_str());
				}
				return true;
			}
		}
	}
	return false;
}
DWORD TfrmMain::CheckMAC(AnsiString MAC)
{   //換人員重計數 //不綁mac範圍 //
	if(bReWork) return 0;
	AnsiString Temp_Start,Temp_Last,Temp;
	if(MAC != "")
	{
	//範圍
		Temp_Start = HexToInt(mskedBurntMacStart->Text.SubString(1,6));
		Temp_Last  = HexToInt(mskedBurntMacLast->Text.SubString(1,6));
		Temp  	   = HexToInt(MAC.SubString(1,6));
		if(Temp.ToInt() <= Temp_Last.ToInt() && Temp.ToInt() >= Temp_Start.ToInt())
		{
			Temp_Start = HexToInt(mskedBurntMacStart->Text.SubString(7,6));
			Temp_Last  = HexToInt(mskedBurntMacLast->Text.SubString(7,6));
			Temp  	   = HexToInt(MAC.SubString(7,6));
			if(Temp.ToInt() > Temp_Last.ToInt() || Temp.ToInt() < Temp_Start.ToInt())
				return ERROR_MAC_OUTRANGE;
		}
		else return ERROR_MAC_OUTRANGE;
	//重複
		Temp = LOGFilePath;
		AnsiString TempRepeat,TempRepeat1;
		if(FileExists(Temp))
		{
			ifstream lanfile(Temp.c_str());
			std::string filemsg;
			if (lanfile.is_open()) {
				while (!lanfile.eof())
				{
					getline(lanfile, filemsg);
					if (strstr(filemsg.c_str(), "PASS")
					)
					{
						TempRepeat  = "[MAC]"+MAC;
						TempRepeat1 = "[MAC2]"+MAC;
						if(strstr(filemsg.c_str(), TempRepeat.c_str())
						|| strstr(filemsg.c_str(), TempRepeat1.c_str()))
						{
							lanfile.close();
							return ERROR_MAC_REPEAT;
						}
					}
				}
				lanfile.close();
			}
		}
		Temp = LogDiskPath;
		if(FileExists(Temp))
		{
			ifstream lanfile(Temp.c_str());
			std::string filemsg;
			if (lanfile.is_open()) {
				while (!lanfile.eof())
				{
					getline(lanfile, filemsg);
					if (strstr(filemsg.c_str(), "PASS")
					)
					{
						TempRepeat  = "[MAC]"+MAC;
						TempRepeat1 = "[MAC2]"+MAC;
						if(strstr(filemsg.c_str(), TempRepeat.c_str())
						|| strstr(filemsg.c_str(), TempRepeat1.c_str()))
						{
							lanfile.close();
							return ERROR_MAC_REPEAT;
						}
					}
				}
				lanfile.close();
			}
		}
		return VERIFTY_MAC_PASS;
	}
	else
	{   //計數
		DWORD dwCount = 0,dwCount1 = 0;
		Temp = LOGFilePath;
		AnsiString EmployeeID = "[EmployeeID]"+edtSetEmployeeID->Text;
		if(FileExists(Temp))
		{
			ifstream lanfile(Temp.c_str());
			std::string filemsg;
			if (lanfile.is_open()) {
				while (!lanfile.eof())
				{
					getline(lanfile, filemsg);
					if (strstr(filemsg.c_str(), "PASS"))
					{   //換工號重新計數  不綁mac範圍
						if(!AnsiString(filemsg.c_str()).Pos(EmployeeID))
							dwCount = 0;
						else
							dwCount++;
					}
				}
				lanfile.close();
			}
		}
		Temp = LogDiskPath;
		if(FileExists(Temp))
		{
			ifstream lanfile(Temp.c_str());
			std::string filemsg;
			if (lanfile.is_open()) {
				while (!lanfile.eof())
				{
					getline(lanfile, filemsg);
					if (strstr(filemsg.c_str(), "PASS"))
					{   //換工號重新計數  不綁mac範圍
						if(!AnsiString(filemsg.c_str()).Pos(EmployeeID))
							dwCount1 = 0;
						else
							dwCount1++;
					}
				}
				lanfile.close();
				return dwCount1;
			}
		}
		return dwCount;
	}
	return 0;
}
void TfrmMain::ShowSettingForm()
{
	if(frmMsg==NULL)  frmMsg = new TfrmMsg(Application);

	frmMsg->edtSetWorkOrderNumber->Text = edtSetWorkOrderNumber->Text;
	frmMsg->mskedBurntMacStart->Text = mskedBurntMacStart->Text;
	frmMsg->mskedBurntMacLast->Text = mskedBurntMacLast->Text;
	frmMsg->edtSetEmployeeID->Text = edtSetEmployeeID->Text.Trim();
	if(ckbBarcode->Checked)
	{
		frmMsg->plckbBarcode->Caption = "√";
	}
	else frmMsg->plckbBarcode->Caption = "";
	if(plTestMode->Caption.Pos("一般"))
	{
		frmMsg->plNormalMode->Color = clLime;
		frmMsg->plReworkMode->Color = clCream;
	}
	else
	{
		frmMsg->plNormalMode->Color = clCream;
		frmMsg->plReworkMode->Color = clLime;
	}

	if(frmMsg->ShowModal()== mrOk)
	{
		edtSetWorkOrderNumber->Text = frmMsg->edtSetWorkOrderNumber->Text.Trim();
		edtSetEmployeeID->Text = frmMsg->edtSetEmployeeID->Text.Trim();
		mskedBurntMacStart->Text = frmMsg->mskedBurntMacStart->Text;
		mskedBurntMacLast->Text = frmMsg->mskedBurntMacLast->Text;
		//
		LOGFilePath = "C:\\ASMP\\log\\"+edtSetWorkOrderNumber->Text+"\\ASIX";
		CheckDiskName();
		//
		if(frmMsg->plckbBarcode->Caption.Pos("√"))
		{
			Edit_resetMAC->Enabled = false;
			ckbBarcode->Checked = true;
			ckbBarcode->Enabled = false;
			plBarcodeSet->Top = 238;
			plBarcodeData->Height = 35;
			plTestMode->Width = 131;
			plTestMode->Caption = frmMsg->plNormalMode->Color == clLime ? "一般模式":"重工模式";
			if(frmMsg->plNormalMode->Color != clLime)
			{   //重工模式
				bReWork = true;
				LOGFilePath += "\\ReWork";
				if(LogDiskPath != "")
					LogDiskPath += "\\ReWork";
			}
			FindLogFile();
			//
			ckbAutoIncrease->Enabled = false;
			ckbAutoIncrease->Checked = false;
		}
		else
		{
			FindLogFile();
			//
			Edit_resetMAC->Enabled = true;
			ckbBarcode->Checked = false;
			ckbAutoIncrease->Enabled = true;
			ckbBarcode->Enabled = false;
			plBarcodeData->Height = 0;
			plTestMode->Width = 0;
		}
		delete frmMsg;
		frmMsg = NULL;
		SetRegValue();
		plResult->Color = clCream;
		plResult->Caption = "Wait";
	}
	else
	{
		delete frmMsg;
		frmMsg = NULL;
		Close();
	}
}
void TfrmMain::NewFilePath(AnsiString Path)
{
	Path+="\\";
	if(!DirectoryExists(Path.c_str()))
	{
		AnsiString SS,Temp = "";
		while(true)
		{
			SS = Path.SubString(1,Path.Pos("\\"));
			Path.Delete(1,Path.Pos("\\"));
			Temp+=SS;
			_mkdir( Temp.c_str());
			if(Path == "")
				break;
		}
	}
}
AnsiString TfrmMain::ReadASIX_MAC()
{
	HANDLE	hReadDevice;
	unsigned int bytes;
	unsigned int romSize = 512;
	//BYTE pbyBuf[512+8];
	unsigned int DelayCount=1;
	EEPROM_DATA	temp;
	AnsiString SS = "";;
	unsigned int offset;
	BYTE	inBuf[3];
	BYTE	outBuf;
	int i,j;

	//Create Handle for Reader
	if (lab_devicename->Caption.Pos("AX88772"))
	{
		hReadDevice = CreateFile (
							TEXT("\\\\.\\AX88772"),
							GENERIC_READ | GENERIC_WRITE,//FILE_READ_ATTRIBUTES | SYNCHRONIZE,
							FILE_SHARE_READ,
							NULL, // no SECURITY_ATTRIBUTES structure
							OPEN_EXISTING, // No special create flags
							FILE_ATTRIBUTE_NORMAL, // No special attributes
							NULL);
	}
	else if (lab_devicename->Caption.Pos("AX88179"))
	{
		hReadDevice = CreateFile (
							TEXT("\\\\.\\AX88179"),
							GENERIC_READ | GENERIC_WRITE,//FILE_READ_ATTRIBUTES | SYNCHRONIZE,
							FILE_SHARE_READ,
							NULL, // no SECURITY_ATTRIBUTES structure
							OPEN_EXISTING, // No special create flags
							FILE_ATTRIBUTE_NORMAL, // No special attributes
							NULL);
	}

	if (hReadDevice) 	//IOCTL_EEPROM_CHANGE_DELAY
	{
		if(!DeviceIoControl (hReadDevice,
			  IOCTL_EEPROM_CHANGE_DELAY,
			  &DelayCount, sizeof(unsigned int),
			  NULL, 0,
			  (unsigned long *)&bytes, NULL))
			{
			hReadDevice=NULL;
			//Memo2->Lines->Add("IOCTL_EEPROM_CHANGE_DELAY failed!");
			}
		else
		{
			//Memo2->Lines->Add("IOCTL_EEPROM_CHANGE_DELAY PASS!");
		}
	}

	//Read
	if (INVALID_HANDLE_VALUE == hReadDevice)
	{
		//Memo2->Lines->Add("Create ASIX Fail");
		hReadDevice=NULL;
	}
	else
	{
		if (lab_devicename->Caption.Pos("AX88772"))
		{
			if(!DeviceIoControl (hReadDevice,
				  IOCTL_EEPROM_READ_DATA,
				  NULL, 0,
				  &ReadBuf[0], 512,
				  (unsigned long *)&bytes, NULL))
			{
				//Memo2->Lines->Add("IOCTL_EEPROM_READ_DATA failed!");
				CloseHandle(hReadDevice);
				hReadDevice=NULL;
			}
		}
		else if (lab_devicename->Caption.Pos("AX88179"))
		{
			for (i = 0; i < 512/2; i++)
			{
				inBuf[0] = 0x07;
				inBuf[1] = 1;
				inBuf[2] = i;
				if(!DeviceIoControl (hReadDevice,
					  IOCTL_WRITE_MAC_REGISTER,
					  &inBuf, sizeof(inBuf),
					  NULL, 0,
					  (unsigned long *)&bytes, NULL))
				{
					//Memo2->Lines->Add("IOCTL_WRITE_MAC_REGISTER 0x07 failed!");
					CloseHandle(hReadDevice);
					hReadDevice=NULL;
					break;
				}

				inBuf[0] = 0x0a;
				inBuf[1] = 1;
				inBuf[2] = 0x04;
				if(!DeviceIoControl (hReadDevice,
					  IOCTL_WRITE_MAC_REGISTER,
					  &inBuf, sizeof(inBuf),
					  NULL, 0,
					  (unsigned long *)&bytes, NULL))
				{
					//Memo2->Lines->Add("IOCTL_WRITE_MAC_REGISTER 0x0a failed!");
					CloseHandle(hReadDevice);
					hReadDevice=NULL;
					break;
				}

				inBuf[0] = 0x0a;
				inBuf[1] = 1;
				inBuf[2] = 0x10;
				for ( j = 0; i < 1000; j++)
				{
					if(!DeviceIoControl (hReadDevice,
						  IOCTL_READ_MAC_REGISTER,
						  &inBuf, sizeof(inBuf),
						  &outBuf, 1,
						  (unsigned long *)&bytes, NULL))
					{
						//Memo2->Lines->Add("IOCTL_READ_MAC_REGISTER 0x0a failed 1!");
						CloseHandle(hReadDevice);
						hReadDevice=NULL;
						break;
					}
					if (outBuf & 0x10 && j < 999)
						continue;
					else if (outBuf & 0x10 && j == 999)
					{
						//Memo2->Lines->Add("IOCTL_READ_MAC_REGISTER 0x0a failed 2!");
						CloseHandle(hReadDevice);
						hReadDevice=NULL;
						break;
					}
					else
					{
						break;
					}
				}
				if (INVALID_HANDLE_VALUE == hReadDevice) break;

				inBuf[0] = 0x08;
				inBuf[1] = 1;
				inBuf[2] = 0x00;
				if(!DeviceIoControl (hReadDevice,
					  IOCTL_READ_MAC_REGISTER,
					  &inBuf, sizeof(inBuf),
					  &outBuf, 1,
					  (unsigned long *)&bytes, NULL))
				{
						//Memo2->Lines->Add("IOCTL_READ_MAC_REGISTER 0x08 failed!");
						CloseHandle(hReadDevice);
						hReadDevice=NULL;
						break;
				}
				ReadBuf[i * 2 + 8] = outBuf;
				inBuf[1] = 1;

				inBuf[0] = 0x09;
				inBuf[2] = 0x00;
				if(!DeviceIoControl (hReadDevice,
					  IOCTL_READ_MAC_REGISTER,
					  &inBuf, sizeof(inBuf),
					  &outBuf, 1,
					  (unsigned long *)&bytes, NULL))
				{
						//Memo2->Lines->Add("IOCTL_READ_MAC_REGISTER 0x09 failed!");
						CloseHandle(hReadDevice);
						hReadDevice=NULL;
						break;
				}
				ReadBuf[i * 2 + 9] = outBuf;
			}
			if (hReadDevice)   //AX88179的TAG並沒有存在EEPROM
			{
			ReadBuf[0]=AX88179ROM[0];
			ReadBuf[1]=AX88179ROM[1];
			ReadBuf[2]=AX88179ROM[2];
			ReadBuf[3]=AX88179ROM[3];
			ReadBuf[4]=AX88179ROM[4];
			ReadBuf[5]=AX88179ROM[5];
			ReadBuf[6]=AX88179ROM[6];
			ReadBuf[7]=AX88179ROM[7];
			}
		}//Read AX88179 END
	}
	if (hReadDevice)
	{
		//AX88772C 與 AX88179 MAC位址存放相同
		//Memo2->Lines->Add("IOCTL_EEPROM_READ_DATA OK!");
		MR5=ReadBuf[8];
		MR4=ReadBuf[9];
		MR3=ReadBuf[10];
		MR2=ReadBuf[11];
		MR1=ReadBuf[12];
		MR0=ReadBuf[13];
		SS.printf("%02X%02X%02X%02X%02X%02X",
		MR5,MR4,MR3,MR2,MR1,MR0);
		//Memo2->Lines->Add(SS);

		if (ReadBuf[2]==0xEC)
		{
			//Memo2->Lines->Add("is EC do Test");
		}
		else
		{
			//Memo2->Lines->Add("not EC do Program");
		}

		CloseHandle(hReadDevice);
		hReadDevice=NULL;
	}
	return SS;
}
void TfrmMain::RegDriver()
{
	if(g_DevVPID != "")
	{
		TRegistry *reg = new TRegistry();
		reg->RootKey = HKEY_LOCAL_MACHINE;
		AnsiString temppath = "SYSTEM\\CurrentControlSet\\Control\\usbflags";
		reg->OpenKey(temppath.c_str(), false);
		AnsiString tempIgnoreHWSerNum = "IgnoreHWSerNum"+g_DevVPID.SubString(5,4)+g_DevVPID.SubString(14,4);
		if(!reg->ValueExists (tempIgnoreHWSerNum))
		{
			BYTE buf[1];
			buf[0] = 0x01;
			reg->WriteBinaryData(tempIgnoreHWSerNum,buf,1);
		}
		reg->CloseKey();
		delete reg;
	}
}
void TfrmMain::FindLogFile()
{
	//
	NewFilePath(LOGFilePath);
	NewFilePath(LogDiskPath);
	//
	TSearchRec Sr;
	AnsiString time = FormatDateTime("yyyymmdd-hhnnss", Now());
	if(DirectoryExists(LogDiskPath.c_str()))
	{
		ImgDisk->Visible = true;
		if(FindFirst(LogDiskPath+"\\*.log",faAnyFile,Sr)==0)
		{
			do
			{
				if(Sr.Name.Pos("RD"))
				{
					if(Sr.Name.SubString(3,8) == time.SubString(1,8))
					{
						RDLogDiskPath = LogDiskPath + "\\"+Sr.Name;
						LogDiskPath += "\\"+Sr.Name.SubString(3,19);
						//
						RDLOGFilePath = LOGFilePath + "\\"+Sr.Name;
						LOGFilePath += "\\"+Sr.Name.SubString(3,19);
						break;
					}
				}
				else
				{
					if(Sr.Name.SubString(1,8) == time.SubString(1,8))
					{
						RDLogDiskPath = LogDiskPath +"\\RD"+Sr.Name;
						LogDiskPath += "\\"+Sr.Name;
						//
						RDLOGFilePath = LOGFilePath +"\\RD"+Sr.Name;
						LOGFilePath += "\\"+Sr.Name;
						break;
					}
				}
			}
			while(FindNext(Sr)==0);
			FindClose(Sr);
		}

		if(!LogDiskPath.Pos(".log"))
		{
			RDLogDiskPath = LogDiskPath +"\\RD"+time+".log";
			LogDiskPath += "\\"+time+".log";
			RDLOGFilePath = LOGFilePath +"\\RD"+time+".log";
			LOGFilePath += "\\"+time+".log";
		}
		else//檔案已存在於USBDisk
		{
			if(!FileExists(LOGFilePath))
			{
				CopyFile(LogDiskPath.c_str(),LOGFilePath.c_str(),true);
				CopyFile(RDLogDiskPath.c_str(),RDLOGFilePath.c_str(),true);
			}
		}
	}
	else
	{
		ImgDisk->Visible = false;
		if(DirectoryExists(LOGFilePath.c_str()))
		{
			if(FindFirst(LOGFilePath+"\\*.log",faAnyFile,Sr)==0)
			{
				do
				{
					if(Sr.Name.Pos("RD"))
					{
						if(Sr.Name.SubString(3,8) == time.SubString(1,8))
						{
							RDLOGFilePath = LOGFilePath + "\\"+Sr.Name;
							LOGFilePath += "\\"+Sr.Name.SubString(3,19);
							break;
						}
					}
					else
					{
						if(Sr.Name.SubString(1,8) == time.SubString(1,8))
						{
							RDLOGFilePath = LOGFilePath +"\\RD"+Sr.Name;
							LOGFilePath += "\\"+Sr.Name;
							break;
						}
					}
				}
				while(FindNext(Sr)==0);
				FindClose(Sr);
			}
		}
		if(!LOGFilePath.Pos(".log"))
		{
			RDLOGFilePath = LOGFilePath +"\\RD"+time+".log";
			LOGFilePath += "\\"+time+".log";
		}
	}
	//
	dwTestCount = CheckMAC("");
	plCount->Caption = dwTestCount;
}
void __fastcall TfrmMain::N1Click(TObject *Sender)
{
	if(btn_start->Enabled)
	{
		btn_start->Caption = "Start";
		Edit_resetMAC->Enabled = true;
		if(!ckbBarcode->Checked)
			ckbAutoIncrease->Enabled = true;
		TestWorking = false;
		ShowSettingForm();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnBarcodeClearClick(TObject *Sender)
{
	edtBarcodeMAC->Text = "";
}
//---------------------------------------------------------------------------


