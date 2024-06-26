﻿// Module:
// Notices: Copyright(c) 2019 Leno.
// Purpose:
// Compiler: Embarcadero RAD Studio 2010
// OS: Windows 8.1 x64
// Version: 1.1
//---------------------------------------------------------------------------

#pragma hdrstop

#include "Main.h"
#include "Hub_fun.h"
#include "MSGBOX.h"
#include "USBDevConnectionInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdIcmpClient"
#pragma link "IdRawBase"
#pragma link "IdRawClient"
#pragma resource "*.dfm"
TfrmMain *frmMain;

Port_test *PORT = new Port_test();
USBDevConnectionInfo *USBCONInfo = new USBDevConnectionInfo();
HANDLE lan;

DWORD TestTime = 0,BarcodeTimeOut = 0;


AnsiString strTemp  = "";
GUID guid;
AnsiString szLanMsg = "";
DWORD dwTestCount = 0;
AnsiString strLoadMAC 		= "";
AnsiString strLoadLANName 	= "";
AnsiString LogDiskPath = "";
DWORD numBarcodeResult;
bool m_bAutoIncrease = false;
AnsiString LOGFile,MACVerifty = "";
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
	N1Click(NULL);


	TListInitLAN = new TStringList;
	frmMain->TListInitLAN->Clear();
	PORT->IntiNetwordCard();
	gFileName = "C:\\ASMP\\ASIXTool\\TEST_LOG.txt";
	_mkdir( "C:\\ASMP\\");
	_mkdir( "C:\\ASMP\\ASIXTool\\" );
	//plSetting->Top = 276;
	if(frmMain->ckbBurn->Caption.Pos("√"))
	{
		new_mac->Caption = Edit_resetMAC->Text;
		new_mac->Visible = true;
	}
	else new_mac->Visible = false;
	cbMACVeriftyCountChange(NULL);
	strTime = FormatDateTime("nnssms", Now());
	strTime = "00249B"+strTime.SubString(1,6);
	LOGFile = AnsiString(FormatDateTime("mm-dd-yyyy", Now()))+".log";
	//
	if(frmMsg==NULL)  frmMsg = new TfrmMsg(Application);
	ReadRegValue();
	if(frmMsg->ShowModal()== mrOk)
	{
		edtSetWorkOrderNumber->Caption  = frmMsg->edtSetWorkOrderNumber->Text;
		edtSetEmployeeID->Caption = frmMsg->edtSetEmployeeID->Text;
		delete frmMsg;
		frmMsg = NULL;
		CheckDiskName();
	}
	else
	{
		delete frmMsg;
		frmMsg = NULL;
		Close();
	}
}

DWORD WINAPI LANExecute(LPVOID Param)
{
	PORT->ChangeLanSpeed(frmMain->g_DevVPID, "{4d36e972-e325-11ce-bfc1-08002be10318}", frmMain->gLAN_NAME);
	return 0;
}
void __fastcall TfrmMain::btn_startClick(TObject *Sender)
{
	lbBarcode->Font->Size = 32;
	lbBarcode->Caption = "請掃描MAC條碼";
	lbBarcode->Color = clTeal;
	TestTime = 0;
	plTime->Caption = "0";
	BarcodeTimeOut = 0;
	MACVerifty = "";
	if(!TestWorking && !AutoTest)
	{
		SaveLogLine(gFileName,"Start");
		btn_start->Enabled 	= false;
		btn_set->Enabled	= false;
		AutoTest = true;
		//初始元件
		plResult->Color = clWhite;
		plResult->Caption = "Wait";
		pan_1000->Color = clBtnFace;
		pan_100->Color = clBtnFace;
		pan_10->Color = clBtnFace;
		frmMain->Panel_mac->Color = clWhite;
		frmMain->Panel_lan->Color = clWhite;
		frmMain->plResult->Color = clCream;
		frmMain->Height = 253;
		bBurnPASS = false;
		bEnd  = false;
		if(AutoTest){
			btn_start->Caption = "停止";
		}else{
			btn_start->Enabled = false;
		}
		// 呼叫LAN Timer
		TestWorking = true;
		Test_Step = GET_DUT;
		btn_start->Enabled = true;

	}else{
		SaveLogLine(gFileName,"Stop");
		btn_start->Enabled 	= false;
		frmMain->g_bError 	= true;
		btn_set->Enabled	= true;
		AutoTest = false;
		TestWorking = false;
		btn_start->Caption = "開始";
		N1Click(NULL);
		btn_start->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	N1Click(NULL);
	if(lan!=NULL) CloseHandle(lan);
	ReleaseMutex(mx);
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::N1Click(TObject *Sender)
{
	AnsiString temppath, temp, LANnum;
	AnsiString lanGUID = "{4d36e972-e325-11ce-bfc1-08002be10318}";
	TRegistry *reg = new TRegistry();
	TStringList *ts = new TStringList;
	reg->RootKey = HKEY_LOCAL_MACHINE;
	temppath = "SYSTEM\\CurrentControlSet\\Control\\Class\\" + lanGUID;
	reg->OpenKey(temppath.c_str(), false);
	reg->GetKeyNames(ts);
	for (int i = 0; i < ts->Count; i++) {
		LANnum = ts->Strings[i];
		temppath = "SYSTEM\\CurrentControlSet\\Control\\Class\\" + lanGUID +
			"\\" + LANnum;
		reg->CloseKey();
		reg->OpenKey(temppath.c_str(), false);
		if(reg->ValueExists ("NetworkAddress"))
			reg->DeleteValue("NetworkAddress");
	}
	reg->CloseKey();
	delete reg;
	delete ts;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ckbNoMACClick(TObject *Sender)
{
	N1Click(NULL);
}
bool TfrmMain::DUTB()
{
	frmMain->LOADClick(NULL);
	frmMain->CHECKClick(NULL);
	frmMain->WRITEClick(NULL);
	//檢查MAC號
	frmMain->READClick(NULL);
	if(frmMain->current_mac->Caption == frmMain->new_mac->Caption){
		return true;
	}else{
		return false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::TimerMainTimer(TObject *Sender)
{
	gLAN_NAME = PORT->GetLANName();
	if(frmMain->TestWorking)
	{
		frmMain->TestWorking = false;
		switch (Test_Step)
		{
			case GET_DUT :
				if(ckbBarcodeVerifty->Caption.Pos("√") && MACVerifty == "")
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
							lbBarcode->Color = clTeal;
							plBarcode->Height = 0;
							BarcodeTimeOut = GetTickCount()+(cbVetiftyWaitTime->Text.ToDouble()*1000);
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
				if(gLAN_NAME != "" && !bEnd)
				{
					ProgressBar->Position = 5;
					g_DevVPID = USBCONInfo->DevNameGetVPID(gLAN_NAME);
					if(g_DevVPID == "" && !gLAN_NAME.Pos("Intel"))
					{
						frmMain->plResult->Color = clRed;
						plResult->Caption = "FAIL";
						frmMain->Height = 293;
						frmMain->plErrMsg->Caption = "(!)GET VPID FAIL";
						frmMain->g_bError	= true;
						Test_Step = WAIT_DUT_PLUGOUT;
						SaveLogLine(gFileName,"(!)GET VPID FAIL");
						writeLOG("(!)GET VPID FAIL",false);
					}
					else
					{
						bBurnPASS = false;
						bPlugOut = false;
						btn_start->Enabled = false;
						plResult->Caption = "Testing";
						ERROR_MSG		  = "";
						TestTime = GetTickCount();
						current_mac->Font->Color = clBlack;
						frmMain->g_bError	= false;
						frmMain->current_mac->Caption = PORT->getLanMAC(gLAN_NAME); //顯示當前設備號
						Test_Step = MAC_LANNAME_VERIFY;
						SaveLogLine(gFileName,"Get NetworkCard: [VPID]"+g_DevVPID+"[NAME]"+gLAN_NAME+"[MAC]"+AnsiString(current_mac->Caption));
						if(ckbBarcodeVerifty->Caption.Pos("√"))
						{
							if(frmMain->current_mac->Caption != MACVerifty)
							{
								frmMain->plResult->Color = clRed;
								plResult->Caption = "FAIL";
								frmMain->Height = 293;
								frmMain->plErrMsg->Caption = "(!)MAC Verifty Failed:"+MACVerifty;
								frmMain->g_bError	= true;
								Test_Step = WAIT_DUT_PLUGOUT;
								SaveLogLine(gFileName,"(!)MAC Verifty Failed");
								writeLOG("(!)MAC Verifty Failed:"+MACVerifty,false);
                            }
						}
					}
				}
				else if(BarcodeTimeOut !=0)
				{
					if(GetTickCount() > BarcodeTimeOut)
					{
						lbBarcode->Font->Size = 20;
						lbBarcode->Caption = "未偵測到待測物請重新掃描MAC";
						lbBarcode->Color = clRed;
						frmMain->plResult->Color = clRed;
						plResult->Caption = "FAIL";
						frmMain->Height = 293;
						frmMain->plErrMsg->Caption = "(!)TimeOUT";
						frmMain->g_bError	= true;
						Test_Step = WAIT_DUT_PLUGOUT;
						SaveLogLine(gFileName,"(!)TimeOUT");
						writeLOG("(!)TimeOUT",false);
					}
					else
					{
						plTime->Caption = AnsiString(double(GetTickCount()-TestTime)/(double)1000);
					}
				}
				break;
			case MAC_LANNAME_VERIFY:
				if(strLoadMAC != "")
				{
					strTemp = current_mac->Caption;
					strTemp = strTemp.SubString(1,strLoadMAC.Length());
					if(!strTemp.Pos(strLoadMAC))
						current_mac->Font->Color = clRed;
				}
				if(strLoadLANName != "")
					if(!lab_devicename->Caption.Pos(strLoadLANName))
						lab_devicename->Font->Color = clRed;
				if(current_mac->Font->Color == clRed || lab_devicename->Font->Color == clRed)
				{
					frmMain->plResult->Color = clRed;
					frmMain->plResult->Caption = "FAIL";
					frmMain->Height = 293;
					frmMain->plErrMsg->Caption = "(!)MAC Verifty Failed";
					frmMain->g_bError	= true;
					Test_Step = WAIT_DUT_PLUGOUT;
					SaveLogLine(gFileName,"(!)MAC Verifty Failed");
					writeLOG("(!)MAC Verifty Failed",false);
				}
				else
				{
					if(lab_devicename->Caption.Pos("88772"))
					{
						Test_Step = frmMain->ckbBurn->Caption.Pos("√") ?  ASIX_MAC_BURN:LAN_TEST;
					}
					else
					{
						if(gLAN_NAME.Pos("Intel"))
							Test_Step = frmMain->ckbBurn->Caption.Pos("√") ?  ASIX_MAC_BURN:LAN_TEST;
						else Test_Step = USB3_TEST;
					}
				}
				ProgressBar->Position = 10;
				break;
			case USB3_TEST:
				btn_start->Enabled = false;
				SaveLogLine(gFileName,"Check USB3.0");
				if(USBCONInfo->GetDevUSB(g_DevVPID.SubString(5,4), g_DevVPID.SubString(14,4))){
					SaveLogLine(gFileName,"USB3.0 PASS");
					Test_Step = frmMain->ckbBurn->Caption.Pos("√") ?  ASIX_MAC_BURN:LAN_TEST;
				}else{
					frmMain->plResult->Color = clRed;
					frmMain->plResult->Caption = "FAIL";
					frmMain->Height = 293;
					frmMain->plErrMsg->Caption = "USB3.0 FAIL";
					frmMain->g_bError	= true;
					Test_Step = WAIT_DUT_PLUGOUT;
					SaveLogLine(gFileName,"USB3.0 FAIL");
					writeLOG("(!)USB3.0 FAIL",false);
				}
				ProgressBar->Position = 15;
				break;
			case ASIX_MAC_BURN:
				popRegDriverClick(NULL);
				if(!PORT->CheckDir())
				{
					ERROR_MSG = "(!)燒錄驅動異常";
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
							ERROR_MSG = "Windows版本不支援";
							break;
						}
					}
                }
				if(ckbBarcode->Caption.Pos("√"))
				{
					numBarcodeResult = 0;
					edtBarcodeMAC->Text = "";
					plBarcode->Height = 138;
					plBarcode->Top = (frmMain->Height/2)-(plBarcode->Height/2);
					plBarcode->Left = (frmMain->Width/2)-(plBarcode->Width/2);
					lbBarcode->Caption = "請掃描MAC燒錄條碼";
					lbBarcode->Color = clTeal;
					edtBarcodeMAC->SetFocus();
					while(true)
					{
						if(numBarcodeResult == BARCODE_FINISH)
						{
							plBarcode->Height = 0;
							frmMain->Refresh();
							if(!DUTB())
								ERROR_MSG = "(!)燒錄失敗";
							else bBurnPASS = true;
							break;
						}
						if(numBarcodeResult == BARCODE_CHANEL)
						{
							plBarcode->Height = 0;
							ERROR_MSG = "(!)Barcode輸入異常";
							break;
						}
					   PORT->Delay(100);
					}
				}
				else
				{
					if(!DUTB())
						ERROR_MSG = "(!)燒錄失敗";
					else bBurnPASS = true;
				}
				current_mac->Caption = new_mac->Caption;
				Test_Step = BURN_TEST_RESULT;
				ProgressBar->Position = 80;
				break;
			case LAN_TEST:
				ProgressBar->Position = 20;
				if(!gLAN_NAME.Pos("Intel"))
					popRegDriverClick(NULL);
				SaveLogLine(gFileName,"Device Driver Regedit Check");
				//
				frmMain->plResult->Caption = "Testing";
				//網路測試
				lan = ::CreateThread(0, 1024, LANExecute, NULL, 0,NULL);
				SaveLogLine(gFileName,"Lan Test Start");
				Test_Step = LAN_TEST_RESULT;
				break;
			case BURN_TEST_RESULT:
				if(ERROR_MSG!="")
				{
					frmMain->ProgressBar->Position = 80;
					Panel_mac->Color = clRed;
					//
					frmMain->plErrMsg->Caption = ERROR_MSG;
					plResult->Color = clRed;
					plResult->Caption = "FAIL";
					frmMain->Height = 293;
					lan = NULL;
					CloseHandle(lan);
					frmMain->SaveLogLine(gFileName,"TEST FAIL");
					Test_Step = WAIT_DUT_PLUGOUT;
					frmMain->SaveLogLine(gFileName,"Waiting PlugOut");
					writeLOG(ERROR_MSG,false);
					break;
				}
				else
				{
					frmMain->ProgressBar->Position = 80;
					Panel_mac->Color = clGreen;
					//
					m_bAutoIncrease = true;
					plResult->Color = clGreen;
					plResult->Caption = "PASS";
					lan = NULL;
					CloseHandle(lan);
					frmMain->SaveLogLine(gFileName,"TEST PASS");
					Test_Step = WAIT_DUT_PLUGOUT;
					frmMain->SaveLogLine(gFileName,"Waiting PlugOut");
					writeLOG("PASS",false);
					current_mac->Caption = new_mac->Caption;
					break;
				}
				break;
			case LAN_TEST_RESULT:
				if(ERROR_MSG!="")
				{
					frmMain->ProgressBar->Position = 90;
					frmMain->plResult->Color = clRed;
					//
					frmMain->plErrMsg->Caption = ERROR_MSG;
					plResult->Color = clRed;
					plResult->Caption = "FAIL";
					frmMain->Height = 293;
					lan = NULL;
					CloseHandle(lan);
					frmMain->SaveLogLine(gFileName,"TEST FAIL");
					Test_Step = WAIT_DUT_PLUGOUT;
					frmMain->SaveLogLine(gFileName,"Waiting PlugOut");
					writeLOG(ERROR_MSG,false);
					break;
				}
				else if(frmMain->plResult->Color == clGreen)
				{
					frmMain->ProgressBar->Position = 90;
					//
					plResult->Color = clGreen;
					plResult->Caption = "PASS";
					lan = NULL;
					CloseHandle(lan);
					frmMain->SaveLogLine(gFileName,"TEST PASS");
					Test_Step = WAIT_DUT_PLUGOUT;
					frmMain->SaveLogLine(gFileName,"Waiting PlugOut");
					writeLOG("PASS",false);
					break;
				}

				break;
			case WAIT_DUT_PLUGOUT:
				btn_start->Enabled = true;
				ProgressBar->Position = 100;
				if(gLAN_NAME == ""||frmMain->bPlugOut)
				{
					bBurnPASS = false;
					MACVerifty = "";
					ProgressBar->Position = 0;
					btn_start->Enabled = true;
					lab_devicename->Font->Color = clBackground;
					current_mac->Font->Color = clBackground;
					frmMain->SaveLogLine(gFileName,"Device PlugOut");
					frmMain->current_mac->Font->Color == clBlue;
					frmMain->lab_devicename->Font->Color == clBlue;
					frmMain->plResult->Color = clWhite;
					frmMain->plResult->Caption = "Wait";
					frmMain->pan_1000->Color = clBtnFace;
					frmMain->pan_100->Color = clBtnFace;
					frmMain->pan_10->Color = clBtnFace;
					AutoIncrease();
					frmMain->Panel_mac->Color = clWhite;
					frmMain->Panel_lan->Color = clWhite;
					frmMain->Height = 253;
					frmMain->plErrMsg->Caption = "";
					frmMain->plTime->Caption = 0;
					if(frmMain->ckbNoMAC->Caption.Pos("√")) frmMain->N1Click(NULL);
					Test_Step = GET_DUT;
					frmMain->SaveLogLine(gFileName,"Waiting Device PlugIn...");
					frmMain->SaveLogLine(gFileName,"==============");
					current_mac->Caption = "未插入設備";
					lab_devicename->Caption = "";
					frmMain->Refresh();
					BarcodeTimeOut = 0;
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


void __fastcall TfrmMain::popRegDriverClick(TObject *Sender)
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
//---------------------------------------------------------------------------
void  TfrmMain::SaveLogLine(AnsiString FileName,AnsiString FileLine)
{
	FileLine = (FormatDateTime("hh:nn:ss", Now()))+"  "+FileLine+="\r\n";
	FILE * fp;
	fp = fopen(FileName.c_str(),"a+");
	fseek( fp, 0, SEEK_END);
	fwrite(FileLine.c_str(),FileLine.Length(),1,fp); //寫入一筆資料
	fclose(fp);
}
bool TfrmMain::Ping(AnsiString IP,bool bLast,DWORD dwSpeed)
{
	szLanMsg = "";
   moDebug->Lines->Add("Pinging \""+IP+"\"...");
   int avg_time = 0;
   gPingTime = 0;
   try
   {
	  IdIcmpClient->Host=IP;
	  for(int i=0; i<3; i++)
      {
		 try
		 {
			AnsiString buff;
			IdIcmpClient->Ping(buff,0);
			Application->ProcessMessages();
         }
		 catch(Exception &e)
		 {
			szLanMsg = "PING 失敗(PING FAIL)";
			moDebug->Lines->Add("Request timed out.");
		 }
      }
   }
   __finally
   {
	  avg_time = gPingTime/3;
	  moDebug->Lines->Add(avg_time);
   }
   moDebug->Lines->Add("End!");
   if(szLanMsg == "")
   {
		//不看速度 108/5/24
		return true;
		if(dwSpeed == 6 && avg_time<5)
		{
			return true;
		}
		else if(dwSpeed == 4 && avg_time<20)
		{
			return true;
		}
		else if(dwSpeed == 2 && avg_time<150)
		{
			return true;
		}
		else
			szLanMsg = "網速未達到(Speed of PING Fail)";
   }
   if(bLast)
		frmMain->ERROR_MSG = szLanMsg;
   return false;

}
void __fastcall TfrmMain::IdIcmpClientReply(TComponent *ASender, const TReplyStatus *AReplyStatus)

{
	AnsiString szReport;
   if(AReplyStatus->FromIpAddress=="0.0.0.0" && AReplyStatus->TimeToLive==0)
   {
		szLanMsg = "PING 失敗(PING FAIL)";
		szReport="Request timed out.";
   }
   else
   {
	  if(AReplyStatus->MsRoundTripTime==0)
	  {
		 szReport.sprintf("Reply from %s: bytes=%d time<1ms TTL=%d",
			AReplyStatus->FromIpAddress, AReplyStatus->BytesReceived,
			AReplyStatus->TimeToLive);
	  }
	  else
	  {
		 szReport.sprintf("Reply from %s: bytes=%d time=%dms TTL=%d",
			AReplyStatus->FromIpAddress, AReplyStatus->BytesReceived,
			AReplyStatus->MsRoundTripTime, AReplyStatus->TimeToLive);
	  }
	  gPingTime+= AReplyStatus->MsRoundTripTime;
   }
   moDebug->Lines->Add(szReport);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnEndClick(TObject *Sender)
{
	SetRegValue();
	ProgressBar->Position = 0;
	MACVerifty = "";
	plSetting->Top = 276;
	btn_start->Enabled = true;
	btn_set->Enabled   = true;
	if(frmMain->ckbBurn->Caption.Pos("√"))
	{
		new_mac->Caption = Edit_resetMAC->Text;
		new_mac->Visible = true;
	}
	else new_mac->Visible = false;
	if(ckbAutoMACVerifty->Caption.Pos("√"))
	{
		lbBarcode->Caption = "請掃描MAC驗證條碼";
		lbBarcode->Color = clTeal;
		strLoadMAC =  maskeditMACCount->Text;
		plBarcodeVerifty->Caption = "MAC 比對碼:"+strLoadMAC;
		if(strLoadMAC != "")
		{
			btnLoadMAC->Caption = strLoadMAC;
			btnLoadMAC->Enabled = false;
		}
	}
	else
	{
		plBarcodeVerifty->Caption = "";
		btnLoadMAC->Caption = "比對MAC";
		strLoadMAC = "";
		btnLoadMAC->Enabled = true;
	}
	//plBarcodeVerifty
	writeLOG("",true);
	SetRegValue();
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

void __fastcall TfrmMain::ckbBurnClick(TObject *Sender)
{
	TPanel *pl = (TPanel *)Sender;
	if(pl->Caption.Pos("√"))
	{
		pl->Caption = "";
		if(pl->Name.Pos("Burn"))
		{
			ckbBarcode->Caption = "";
			ckbAutoAdd->Caption = "";
			ckbBarcode->Enabled = false;
			ckbAutoAdd->Enabled = false;
			ckbNoMAC->Enabled = true;
			ckbAutoMACVerifty->Enabled = true;
			cbVetiftyWaitTime->Enabled 	= true;
			ckbBarcodeVerifty->Enabled 	= true;
			ckbLan->Caption = "√";
		}
		else if(pl->Name.Pos("Lan"))
		{
			ckbBarcode->Enabled = true;
			ckbAutoAdd->Enabled = true;
			ckbNoMAC->Enabled = false;
			ckbAutoMACVerifty->Enabled = false;
			cbVetiftyWaitTime->Enabled 	= false;
			ckbBarcodeVerifty->Enabled 	= false;
			ckbNoMAC->Caption = "";
			ckbAutoMACVerifty->Caption = "";
			ckbBarcodeVerifty->Caption = "";
			ckbBurn->Caption = "√";
		}
	}
	else
	{
		pl->Caption = "√";
		if(pl->Name.Pos("Burn"))
		{
			ckbBarcode->Enabled = true;
			ckbAutoAdd->Enabled = true;
			ckbNoMAC->Enabled = false;
			ckbAutoMACVerifty->Enabled = false;
			cbVetiftyWaitTime->Enabled 	= false;
			ckbBarcodeVerifty->Enabled 	= false;

			ckbNoMAC->Caption = "";
			ckbAutoMACVerifty->Caption = "";
			ckbBarcodeVerifty->Caption = "";
			ckbLan->Caption = "";
		}
		else if(pl->Name.Pos("Lan"))
		{
			ckbBarcode->Enabled = false;
			ckbAutoAdd->Enabled = false;
			ckbNoMAC->Enabled = true;
			ckbAutoMACVerifty->Enabled = true;
			cbVetiftyWaitTime->Enabled 	= true;
			ckbBarcodeVerifty->Enabled 	= true;

			ckbBarcode->Caption = "";
			ckbAutoAdd->Caption = "";
			ckbBurn->Caption = "";
		}
		else if(pl->Name.Pos("ckbAutoAdd"))
		{
			ckbBarcode->Caption = "";
		}
		else if(pl->Name.Pos("ckbBarcode"))
		{
			ckbAutoAdd->Caption = "";
		}
    }
	N1Click(NULL);
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::btn_setClick(TObject *Sender)
{
	plSetting->Width = 612;
	plSetting->Top = 0;
	plSetting->Left = 0;
	btn_start->Enabled = false;
	btn_set->Enabled   = false;
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

void __fastcall TfrmMain::btnBarcodeClearClick(TObject *Sender)
{
	edtBarcodeMAC->Text = "";
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
		new_mac->Caption = edtBarcodeMAC->Text.SubString(1,2)+":"
							+edtBarcodeMAC->Text.SubString(3,2)+":"
							+edtBarcodeMAC->Text.SubString(5,2)+":"
							+edtBarcodeMAC->Text.SubString(7,2)+":"
							+edtBarcodeMAC->Text.SubString(9,2)+":"
							+edtBarcodeMAC->Text.SubString(11,2);
		MACVerifty = new_mac->Caption;
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

	ReadGood=false;
	isEC=false;

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
				current_mac->Caption = PORT->getLanMAC(gLAN_NAME); //顯示當前設備號
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
	SS.printf("%02X:%02X:%02X:%02X:%02X:%02X",
	MR5,MR4,MR3,MR2,MR1,MR0);
	current_mac->Caption = SS; //顯示當前設備號
	Memo2->Lines->Add(SS);

if (ReadBuf[2]==0xEC)
	{
	isEC = true;
	Memo2->Lines->Add("is EC do Test");
	}
else
	{
	isEC = false;
	Memo2->Lines->Add("not EC do Program");
	}
	ReadGood=true;
	CloseHandle(hReadDevice);
	hReadDevice=NULL;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::MACPClick(TObject *Sender)
{
	AnsiString SS;
	if (MA0==0xFF) {
		MA0=0x00;
		if (MA1==0xFF) {
			MA1=0x00;
			if (MA2==0xFF)
				{
				MA2=0x00;
				} else MA2++;
			} else MA1++;
		} else MA0++;
	SS.printf("%02X",MA2); Edit4->Text=SS;
	SS.printf("%02X",MA1); Edit5->Text=SS;
	SS.printf("%02X",MA0); Edit6->Text=SS;
	SS.printf("%02X:%02X:%02X:%02X:%02X:%02X",MA5,MA4,MA3,MA2,MA1,MA0);
	Memo2->Lines->Add("Next MAC Address is "+SS);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::WRITEClick(TObject *Sender)
{
	HANDLE	hControlDevice;
	//unsigned char MR5,MR4,MR3,MR2,MR1,MR0;
	unsigned int bytes;
	unsigned int romSize = 512;
	PROGPass=false;
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
		PROGPass=true;
		SS.printf("%02X:%02X:%02X:%02X:%02X:%02X",
		MA5,MA4,MA3,MA2,MA1,MA0);
		}
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::LOADClick(TObject *Sender)
{
	Edit1->Text = new_mac->Caption.SubString(0,2);
	Edit2->Text = new_mac->Caption.SubString(4,2);
	Edit3->Text = new_mac->Caption.SubString(7,2);
	Edit4->Text = new_mac->Caption.SubString(10,2);
	Edit5->Text = new_mac->Caption.SubString(13,2);
	Edit6->Text = new_mac->Caption.SubString(16,2);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::CHECKClick(TObject *Sender)
{
	AnsiString SS;
	unsigned int SV,EV;

	CHKERR=false;
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

	if(!ckbBarcode->Caption.Pos("√") && m_bAutoIncrease)
	{
		m_bAutoIncrease = false;
		SS = Edit_resetMAC->Text.SubString(1,2)+Edit_resetMAC->Text.SubString(4,2)
						+Edit_resetMAC->Text.SubString(7,2);
		tmp_1 = HexToInt(SS);
		SS = Edit_resetMAC->Text.SubString(10,2)+Edit_resetMAC->Text.SubString(13,2)
					+Edit_resetMAC->Text.SubString(16,2);
		tmp_2 = HexToInt(SS);
		if(ckbBurn->Caption.Pos("√")&&ckbAutoAdd->Caption.Pos("√"))
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
			SS = MAC_1.SubString(1,2)+":"+MAC_1.SubString(3,2)+":"+MAC_1.SubString(5,2)+":"
				+MAC_2.SubString(1,2)+":"+MAC_2.SubString(3,2)+":"+MAC_2.SubString(5,2);
			Edit_resetMAC->Text = SS;
			new_mac->Caption = SS;
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
		bool bl = reg->OpenKey("ASIXTool", true);
		bl = reg->OpenKey("v1.1", false);
		if(bl)
		{
			if(reg->ValueExists("MAC_BURN")){
				value = reg->ReadString("MAC_BURN");
				if(value == "true") ckbBurn->Caption = "√";
				else ckbBurn->Caption = "";
			}
			else
				ckbBurn->Caption = "";
			if(reg->ValueExists("MAC")){
				value = reg->ReadString("MAC");
				Edit_resetMAC->Text = value;
				new_mac->Caption = Edit_resetMAC->Text;
			}
			else Edit_resetMAC->Text = "00:24:9B:00:00:00";

			if(reg->ValueExists("BarcodeMAC")){
				value = reg->ReadString("BarcodeMAC");
				if(value == "true") ckbBarcode->Caption = "√";
				else ckbBarcode->Caption = "";
			}
			else ckbBarcode->Caption = "";

			if(reg->ValueExists("AutoAdd")){
				value = reg->ReadString("AutoAdd");
				if(value == "true") ckbAutoAdd->Caption = "√";
				else ckbAutoAdd->Caption = "";
			}
			else ckbAutoAdd->Caption = "";

			if(reg->ValueExists("LAN_TEST")){
				value = reg->ReadString("LAN_TEST");
				if(value == "true") ckbLan->Caption = "√";
				else ckbLan->Caption = "";
			}
			else
			{
				ckbLan->Enabled 	= true;
				ckbLan->Caption = "√";
				ckbLan->Enabled 	= false;
			}

			if(reg->ValueExists("NO_MAC")){
				value = reg->ReadString("NO_MAC");
				if(value == "true") ckbNoMAC->Caption = "√";
				else ckbNoMAC->Caption = "";
			}
			else ckbLan->Caption = "√";

			if(reg->ValueExists("MAC_AUTO_VERIFTY")){
				value = reg->ReadString("MAC_AUTO_VERIFTY");
				if(value == "true") ckbAutoMACVerifty->Caption = "√";
				else ckbAutoMACVerifty->Caption = "";
			}
			else ckbAutoMACVerifty->Caption = "";

			if(reg->ValueExists("MAC_AUTO_VERIFTY_COUNT")){
				value = reg->ReadString("MAC_AUTO_VERIFTY_COUNT");
				for(int i =0;i<=6;i++)
				{
					cbMACVeriftyCount->ItemIndex = i;
					if(cbMACVeriftyCount->Text == value)
						break;
				}
				cbMACVeriftyCount->Text = value;
			}
			else cbMACVeriftyCount->Text = "6";

			if(reg->ValueExists("MAC_AUTO_VERIFTY_SEC")){
				value = reg->ReadString("MAC_AUTO_VERIFTY_SEC");
				for(int i =0;i<=15;i++)
				{
					cbVetiftyWaitTime->ItemIndex = i;
					if(cbVetiftyWaitTime->Text == value)
						break;
				}
				cbVetiftyWaitTime->Text = value;
			}
			else cbVetiftyWaitTime->Text = "10";

			if(reg->ValueExists("MAC_BARCODE_VERIFTY")){
				value = reg->ReadString("MAC_BARCODE_VERIFTY");
				if(value == "true") ckbBarcodeVerifty->Caption = "√";
				else ckbBarcodeVerifty->Caption = "";
			}
			else ckbBarcodeVerifty->Caption = "";

			if(reg->ValueExists("MAC_AUTO_VERIFTY_TEXT")){
				value = reg->ReadString("MAC_AUTO_VERIFTY_TEXT");
				maskeditMACCount->Text = value;
			}
			else maskeditMACCount->Text = "00:24:9B";

			if(reg->ValueExists("WorkOrderNumber")){
				value = reg->ReadString("WorkOrderNumber");
				frmMsg->edtSetWorkOrderNumber->Text = value;
			}
			if(reg->ValueExists("EmployeeID")){
				value = reg->ReadString("EmployeeID");
				frmMsg->edtSetEmployeeID->Text = value;
			}
		}
		else
		{
			ckbBurn->Caption = "";
			Edit_resetMAC->Text = "00:24:9B:00:00:00";
			ckbBarcode->Caption = "";
			ckbAutoAdd->Caption = "";
			ckbLan->Caption = "√";
			ckbAutoMACVerifty->Caption = "";
			cbMACVeriftyCount->ItemIndex = 2;
			maskeditMACCount->Text = "00:24:9B:00";
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
	bool bl = reg->OpenKey("ASIXTool", true);
	bl = reg->OpenKey("v1.1", true);
	if(bl)
	{
		SS = ckbBurn->Caption == "√" ? "true":"false";
		reg->WriteString("MAC_BURN", SS);
		SS = Edit_resetMAC->Text;
		reg->WriteString("MAC", SS);
		SS = ckbBarcode->Caption == "√" ? "true":"false";
		reg->WriteString("BarcodeMAC",  SS);
		SS = ckbAutoAdd->Caption == "√" ? "true":"false";
		reg->WriteString("AutoAdd",  SS);
		SS = ckbLan->Caption == "√" ? "true":"false";
		reg->WriteString("LAN_TEST", SS);
		SS = ckbNoMAC->Caption == "√" ? "true":"false";
		reg->WriteString("NO_MAC", SS);
		SS = ckbAutoMACVerifty->Caption == "√" ? "true":"false";
		reg->WriteString("MAC_AUTO_VERIFTY", SS);
		SS = cbMACVeriftyCount->Text;
		reg->WriteString("MAC_AUTO_VERIFTY_COUNT", SS);
		SS = maskeditMACCount->Text;
		reg->WriteString("MAC_AUTO_VERIFTY_TEXT", SS);
		SS = ckbBarcodeVerifty->Caption == "√" ? "true":"false";
		reg->WriteString("MAC_BARCODE_VERIFTY", SS);
		SS = cbVetiftyWaitTime->Text;
		reg->WriteString("MAC_AUTO_VERIFTY_SEC", SS);

		reg->WriteString("WorkOrderNumber", edtSetWorkOrderNumber->Caption );
		reg->WriteString("EmployeeID", edtSetEmployeeID->Caption );
	}
	delete reg;
}
void __fastcall TfrmMain::Panel_macDblClick(TObject *Sender)
{
		frmMain->Width = frmMain->Width==1025 ? 616 : 1025;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::BitBtn2Click(TObject *Sender)
{
	if(BitBtn2->Caption == "Enter")
	{
		if(edtPassWord->Text.UpperCase() == "SET")
		{
			BitBtn2->Caption		 = "END";
			if(ckbBurn->Caption != "")
			{
				ckbBurn->Enabled 	= true;
				ckbBarcode->Enabled = true;
				ckbAutoAdd->Enabled = true;
				Edit_resetMAC->Enabled 	= true;
				ckbLan->Enabled 	= true;
				ckbNoMAC->Enabled 	= false;
				ckbAutoMACVerifty->Enabled 	= false;
				cbVetiftyWaitTime->Enabled 	= false;
				cbMACVeriftyCount->Enabled 	= true;
				maskeditMACCount->Enabled 	= true;
			}
			else
			{
				ckbBurn->Enabled 	= true;
				ckbBarcode->Enabled = false;
				ckbAutoAdd->Enabled = false;
				Edit_resetMAC->Enabled 	= true;
				ckbLan->Enabled 	= true;
				ckbNoMAC->Enabled 	= true;
				ckbAutoMACVerifty->Enabled 	= true;
				cbVetiftyWaitTime->Enabled 	= true;
				cbMACVeriftyCount->Enabled 	= true;
				maskeditMACCount->Enabled 	= true;
				ckbBarcodeVerifty->Enabled 	= true;
            }
			btnEnd->Enabled 	= false;
		}
		else edtPassWord->Font->Color = clRed;
	}
	else
	{
		BitBtn2->Caption	  	= "Enter";
		edtPassWord->Text 	= "";
			ckbBurn->Enabled 	= false;
			ckbBarcode->Enabled = false;
			ckbAutoAdd->Enabled = false;
			ckbLan->Enabled 	= false;
			ckbNoMAC->Enabled 	= false;
			Edit_resetMAC->Enabled 	= false;
			ckbAutoMACVerifty->Enabled 	= false;
			cbVetiftyWaitTime->Enabled 	= false;
			cbMACVeriftyCount->Enabled 	= false;
			maskeditMACCount->Enabled 	= false;
			btnEnd->Enabled 	= true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::edtPassWordEnter(TObject *Sender)
{
	if(edtPassWord->Font->Color == clRed)
	{
		edtPassWord->Font->Color = clBlue;
		edtPassWord->Text 	= "";
	}
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::cbMACVeriftyCountChange(TObject *Sender)
{
	AnsiString SS = "!>AA:AA:AA";
	AnsiString SS2 = "00:24:9B";
	for(int i = 6 ; i < cbMACVeriftyCount->Text ; i++)
	{
		SS  += i%2==0 ? ":A":"A";
		SS2 += i%2==0 ? ":0":"0";
	}
	SS += ";1;_";
	maskeditMACCount->EditMask = SS;
	maskeditMACCount->Text = SS2;
}
//---------------------------------------------------------------------------
bool TfrmMain::writeLOG(AnsiString Msg,bool bAction)
{
	AnsiString SS;
	AnsiString LOGFilePath = "C:\\ASMP\\log\\"+edtSetWorkOrderNumber->Caption+"\\ASIXTool";
	if(bAction)
	{  //創LOG資料夾
		if(!FileExists(LOGFilePath.c_str())){
			_mkdir( "C:\\ASMP\\");
			_mkdir( "C:\\ASMP\\log\\" );
			SS = "C:\\ASMP\\log\\"+edtSetWorkOrderNumber->Caption;
			_mkdir(SS.c_str());
			_mkdir(LOGFilePath.c_str());
		}
		SS = "\r\n===========\r\n"
			+AnsiString(FormatDateTime("hh-mm-ss", Now()))
			+"\r\n [Burn]"+ckbBurn->Caption
			+" [BurnMAC]"+Edit_resetMAC->Text
			+"\r\n [BarcodeMAC]"+ckbBarcode->Caption
			+" [AutoAdd]"+ckbAutoAdd->Caption
			+"\r\n [LAN]"+ckbLan->Caption
			+" [ZeroMAC]"+ckbNoMAC->Caption
			+" [AutoMACVerifty]"+ckbAutoMACVerifty->Caption
			+" [AutoMACVeriftyCount]"+cbMACVeriftyCount->Text
			+"\r\n [AutoMACVeriftyText]"+maskeditMACCount->Text
			+"\r\n===========\r\n"
			+" [操作人員工號]"+edtSetEmployeeID->Caption+"\r\n=================================\r\n";
			SaveUserLogLine(LOGFilePath+"\\LOG_RD.log",SS);
			if(LogDiskPath != "")
				SaveUserLogLine(LogDiskPath+"\\LOG_RD.log",SS);
	}
	else
	{
		dwTestCount++;
		AnsiString strTemp;
		strTemp.sprintf("%04d ",dwTestCount);
		if(Msg.Pos("PASS"))
		{
			SS = "\r\n"+strTemp+"[PASS]"+AnsiString(FormatDateTime("mm-dd-yyyy hh:mm:ss", Now()))
				+" [NAME]"+lab_devicename->Caption+" [MAC]"+current_mac->Caption;
			if(ckbBurn->Caption.Pos("√"))
				SS+=" [BurnMAC]"+new_mac->Caption;
			SS+= " [WorkOrder]"+edtSetEmployeeID->Caption+" [EmployeeID]"+edtSetEmployeeID->Caption+" [TEST-TIME]"+plTime->Caption;
		}
		else
		{
			SS = "\r\n"+strTemp+"[FAIL]"+AnsiString(FormatDateTime("mm-dd-yyyy hh:mm:ss", Now()))
				+" [NAME]"+lab_devicename->Caption+" [MAC]"+current_mac->Caption;
			if(ckbBurn->Caption.Pos("√"))
				SS+=" [BurnMAC]"+new_mac->Caption;
			SS+= " [WorkOrder]"+edtSetEmployeeID->Caption+" [EmployeeID]"+edtSetEmployeeID->Caption+" [TEST-TIME]"+plTime->Caption;
			SS+="[ErrorMsg]"+Msg;
		}
		SaveUserLogLine(LOGFilePath+"\\LOG_RD.log",SS);
		SaveUserLogLine(LOGFilePath+"\\LOG.log",SS);
		if(LogDiskPath != "")
		{
			SaveUserLogLine(LogDiskPath+"\\LOG_RD.log",SS);
			SaveUserLogLine(LogDiskPath+"\\LOG.log",SS);
		}
    }
}
void  TfrmMain::SaveUserLogLine(AnsiString FileName,AnsiString FileLine)
{
	FILE * fp;
	fp = fopen(FileName.c_str(),"a+");
	fseek( fp, 0, SEEK_END);
	fwrite(FileLine.c_str(),FileLine.Length(),1,fp); //寫入一筆資料
	fclose(fp);
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
				LogDiskPath = DiskName+"\\ASMP\\log\\"+edtSetWorkOrderNumber->Caption+"\\ASIXTool";
				if(!FileExists(LogDiskPath.c_str())){
					_mkdir( DiskName.c_str());
					SS = DiskName + "\\ASMP";
					_mkdir( SS.c_str());
					SS = DiskName + "\\ASMP\\log";
					_mkdir( SS.c_str());
					SS = DiskName + "\\ASMP\\log\\"+edtSetWorkOrderNumber->Caption;
					_mkdir(SS.c_str());
					_mkdir(LogDiskPath.c_str());
				}
				return true;
			}
		}
	}
	return false;
}
