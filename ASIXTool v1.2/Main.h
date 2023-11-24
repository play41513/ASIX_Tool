//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
/////////////////////////////////////////
#include <windows.h>
#include <basetyps.h>
#include <winioctl.h>
#include <setupapi.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <pngimage.hpp>
#include <Buttons.hpp>
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdIcmpClient.hpp"
#include "IdRawBase.hpp"
#include "IdRawClient.hpp"
#include <Mask.hpp>

#define D_MAX_TEST_ITEM		4

#define WAIT_SCAN		0
#define SCAN_OVER		1
#define SCAN_CHANCEL	2

#define GET_DUT				0
#define BURN_TEST_RESULT	1
#define LAN_TEST            2
#define LAN_TEST_RESULT		3
#define USB3_TEST			4
#define WAIT_DUT_PLUGOUT	5
#define MAC_LANNAME_VERIFY	6
#define ASIX_MAC_BURN		7

#define BARCODE_FINISH	1
#define BARCODE_CHANEL  2



#define IOCTL_EEPROM_READ_DATA			CTL_CODE (FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_EEPROM_WRITE_DATA			CTL_CODE (FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_EFUSE_READ_DATA			CTL_CODE (FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_EFUSE_WRITE_DATA			CTL_CODE (FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_EEPROM_CHANGE_DELAY       CTL_CODE (FILE_DEVICE_UNKNOWN, 0x816, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_DEVICE				CTL_CODE (FILE_DEVICE_UNKNOWN, 0x80F, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SPEED_CHANGE				CTL_CODE (FILE_DEVICE_UNKNOWN, 0x810, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CHECK_LINK				CTL_CODE (FILE_DEVICE_UNKNOWN, 0x811, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_TO_MAC_MODE			CTL_CODE (FILE_DEVICE_UNKNOWN, 0x821, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_TO_PHY_MODE			CTL_CODE (FILE_DEVICE_UNKNOWN, 0x822, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_MAC_REGISTER		CTL_CODE (FILE_DEVICE_UNKNOWN, 0x830, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_MAC_REGISTER			CTL_CODE (FILE_DEVICE_UNKNOWN, 0x831, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_PHY_REGISTER		CTL_CODE (FILE_DEVICE_UNKNOWN, 0x840, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_PHY_REGISTER			CTL_CODE (FILE_DEVICE_UNKNOWN, 0x841, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_USBDC_REGISTER		CTL_CODE (FILE_DEVICE_UNKNOWN, 0x850, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_USBDC_REGISTER		CTL_CODE (FILE_DEVICE_UNKNOWN, 0x851, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SET_WAKEUP_FRAME			CTL_CODE (FILE_DEVICE_UNKNOWN, 0x860, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DEVICE_CYCLE_PORT_179		CTL_CODE (FILE_DEVICE_UNKNOWN, 0x861, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DEVICE_CYCLE_PORT_772		CTL_CODE (FILE_DEVICE_UNKNOWN, 0x8A0, METHOD_BUFFERED, FILE_ANY_ACCESS)

//MAC 燒錄區段 開始~結束 字元
unsigned char MA5=0x00;
unsigned char MA4=0x01;
unsigned char MA3=0x8E;
unsigned char MA2=0x04;
unsigned char MA1=0x00;
unsigned char MA0=0x00;
unsigned char ME2=0x04;
unsigned char ME1=0x00;
unsigned char ME0=0x00;

unsigned char MR5,MR4,MR3,MR2,MR1,MR0;
BYTE WriteBuf[512+8];
BYTE ReadBuf[512+8];

//for 88179
typedef struct _EEPROM_DATA
{
	ULONG		Tag;
	USHORT		Offset;
	USHORT		Length;
	USHORT		Data[2];
} EEPROM_DATA, *PEEPROM_DATA;

BYTE AX88772CROM[512]=
{
//                                                 8     9    10    11    12    13
//                                                00    01    8E    AE    04    4F
0x15, 0x5A, 0xEC, 0x75, 0x20, 0x12, 0x29, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x09, 0x04,
0x60, 0x22, 0x71, 0x12, 0x19, 0x0E, 0x3D, 0x04, 0x3D, 0x04, 0x3D, 0x04, 0x3D, 0x04, 0x80, 0x05,
0x00, 0x06, 0x10, 0xE0, 0x42, 0x24, 0x40, 0x12, 0x49, 0x27, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF,
0xC0, 0x09, 0x0E, 0x03, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x32, 0x00,
0x12, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x40, 0x95, 0x0B, 0x2B, 0x77, 0x02, 0x00, 0x01, 0x02,
0x03, 0x01, 0x09, 0x02, 0x27, 0x00, 0x01, 0x01, 0x04, 0xA0, 0x64, 0x09, 0x04, 0x00, 0x00, 0x03,
0xFF, 0xFF, 0x00, 0x07, 0x07, 0x05, 0x81, 0x03, 0x08, 0x00, 0x0B, 0x07, 0x05, 0x82, 0x02, 0x00,
0x02, 0x00, 0x07, 0x05, 0x03, 0x02, 0x00, 0x02, 0x00, 0xFF, 0x04, 0x03, 0x30, 0x00, 0xFF, 0xFF,
0x12, 0x01, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x08, 0x95, 0x0B, 0x2B, 0x77, 0x02, 0x00, 0x01, 0x02,
0x03, 0x01, 0x09, 0x02, 0x27, 0x00, 0x01, 0x01, 0x04, 0xA0, 0x64, 0x09, 0x04, 0x00, 0x00, 0x03,
0xFF, 0xFF, 0x00, 0x07, 0x07, 0x05, 0x81, 0x03, 0x08, 0x00, 0xA0, 0x07, 0x05, 0x82, 0x02, 0x40,
0x00, 0x00, 0x07, 0x05, 0x03, 0x02, 0x40, 0x00, 0x00, 0xDD, 0xFF, 0xFF, 0xAA, 0xAA, 0xBB, 0xBB,
0x22, 0x03, 0x41, 0x00, 0x53, 0x00, 0x49, 0x00, 0x58, 0x00, 0x20, 0x00, 0x45, 0x00, 0x6C, 0x00,
0x65, 0x00, 0x63, 0x00, 0x2E, 0x00, 0x20, 0x00, 0x43, 0x00, 0x6F, 0x00, 0x72, 0x00, 0x70, 0x00,
0x2E, 0x00, 0x12, 0x03, 0x41, 0x00, 0x58, 0x00, 0x38, 0x00, 0x38, 0x00, 0x37, 0x00, 0x37, 0x00,
0x32, 0x00, 0x42, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


BYTE AX88179ROM[512+8]=				//AX88179  [4TAG+2Len+2Offset+512Flash]
{//0x58495341
//                                                 8     9    10    11    12    13
//                                                00    01    8E    AB    B0    2B
0x41, 0x53, 0x49, 0x58, 0x00, 0x00, 0x04, 0x01, 0x00, 0x01, 0x8E, 0xAB, 0xB0, 0x2B, 0x90, 0x17,
0x95, 0x0B, 0xB7, 0x73, 0x00, 0xE0, 0x3E, 0x01, 0x80, 0x0B, 0x09, 0x04, 0x0E, 0x07, 0x1A, 0x10,
0x26, 0x0E, 0x2D, 0x16, 0x41, 0x58, 0x38, 0x38, 0x31, 0x37, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x53, 0x49, 0x58,
0x20, 0x45, 0x6C, 0x65, 0x63, 0x2E, 0x20, 0x43, 0x6F, 0x72, 0x70, 0x2E, 0x00, 0x00, 0x00, 0x00,
//80                      84    85    86    87    88    89    90    91    92    93    94    95
//                         X     X     0     0     0     1     8     E     A     B     B     0
0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x38, 0x45, 0x41, 0x42, 0x42, 0x30,
//96    97
// 2     B
0x32, 0x42, 0x05, 0x0F, 0x16, 0x00, 0x02, 0x07, 0x10, 0x02, 0x02, 0x00, 0x00, 0x00, 0x0A, 0x10,
0x03, 0x00, 0x0E, 0x00, 0x01, 0x01, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF3, 0xFF, 0x40, 0x4A, 0x40, 0x00, 0x40, 0x30, 0x0D, 0x49, 0x90, 0x41, 0xC1, 0xBD, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TButton *btn_start;
	TPanel *plResult;
	TPanel *plErrMsg;
	TPanel *plSet;
	TPopupMenu *PopupMenu1;
	TMenuItem *N1;
	TPanel *Panel6;
	TGroupBox *GroupBox2;
	TPanel *Panel10;
	TLabel *Label2;
	TPanel *plTime;
	TTimer *TimerMain;
	TMenuItem *popRegDriver;
	TIdIcmpClient *IdIcmpClient;
	TPanel *plDebug;
	TMemo *moDebug;
	TBitBtn *btnLoadMAC;
	TBitBtn *btnLoadNetworkCardName;
	TButton *btn_set;
	TPanel *Panel5;
	TPanel *pan_10;
	TPanel *pan_100;
	TPanel *pan_1000;
	TPanel *Panel8;
	TGroupBox *current_mac_group;
	TLabel *current_mac;
	TPanel *Panel_mac;
	TPanel *Panel_lan;
	TGroupBox *GroupBox1;
	TLabel *new_mac;
	TPanel *Panel9;
	TLabel *Label1;
	TProgressBar *ProgressBar;
	TPanel *plSetting;
	TPanel *Panel7;
	TPanel *Panel13;
	TPanel *Panel15;
	TPanel *ckbLan;
	TPanel *Panel18;
	TPanel *Panel11;
	TPanel *ckbNoMAC;
	TPanel *Panel19;
	TPanel *Panel22;
	TPanel *plBarcode;
	TLabel *lbBarcode;
	TPanel *Panel12;
	TBitBtn *btnBarcodeChanel;
	TBitBtn *btnBarcodeClear;
	TEdit *edtBarcodeMAC;
	TPanel *Panel14;
	TButton *WRITE;
	TButton *CHECK;
	TButton *LOAD;
	TButton *SAVE;
	TButton *READ;
	TButton *MACP;
	TButton *CHKDEVICE;
	TButton *changedevice;
	TButton *SaveLog;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	TEdit *Edit6;
	TEdit *Edit9;
	TEdit *Edit8;
	TEdit *Edit7;
	TMemo *Memo2;
	TCheckBox *SpeedTest;
	TPanel *Panel16;
	TPanel *ckbBarcode;
	TPanel *Panel21;
	TPanel *ckbAutoAdd;
	TPanel *Panel20;
	TPanel *Panel17;
	TPanel *ckbBurn;
	TPanel *Panel23;
	TBitBtn *btnEnd;
	TEdit *edtPassWord;
	TBitBtn *BitBtn2;
	TMaskEdit *Edit_resetMAC;
	TPanel *Panel24;
	TPanel *ckbAutoMACVerifty;
	TPanel *Panel30;
	TMaskEdit *maskeditMACCount;
	TPanel *Panel27;
	TComboBox *cbMACVeriftyCount;
	TPanel *ckbBarcodeVerifty;
	TPanel *Panel26;
	TPanel *Panel25;
	TPanel *edtSetEmployeeID;
	TPanel *edtSetModel;
	TPanel *edtWorkStation;
	TPanel *edtSetWorkOrderNumber;
	TComboBox *cbVetiftyWaitTime;
	TLabel *lab_devicename;
	TPanel *plBarcodeVerifty;
	void __fastcall btn_startClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall ckbNoMACClick(TObject *Sender);
	void __fastcall TimerMainTimer(TObject *Sender);
	void __fastcall popRegDriverClick(TObject *Sender);
	void __fastcall IdIcmpClientReply(TComponent *ASender, const TReplyStatus *AReplyStatus);
	void __fastcall btnEndClick(TObject *Sender);
	void __fastcall btnLoadMACClick(TObject *Sender);
	void __fastcall btnLoadNetworkCardNameClick(TObject *Sender);
	void __fastcall ckbBurnClick(TObject *Sender);
	void __fastcall btn_setClick(TObject *Sender);
	void __fastcall edtBarcodeMACChange(TObject *Sender);
	void __fastcall btnBarcodeClearClick(TObject *Sender);
	void __fastcall btnBarcodeChanelClick(TObject *Sender);
	void __fastcall edtBarcodeMACKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall READClick(TObject *Sender);
	void __fastcall MACPClick(TObject *Sender);
	void __fastcall WRITEClick(TObject *Sender);
	void __fastcall LOADClick(TObject *Sender);
	void __fastcall CHECKClick(TObject *Sender);
	void __fastcall Panel_macDblClick(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall edtPassWordEnter(TObject *Sender);
	void __fastcall cbMACVeriftyCountChange(TObject *Sender);


private:	// User declarations
	bool PROGPass, ReadGood, isEC, CHKERR, GETDEVICE;
		AnsiString CHIPSET;
	bool DUTB();
	void TfrmMain::AutoIncrease();
	int TfrmMain::HexToInt(AnsiString HexStr);
	void TfrmMain::ReadRegValue();
	void TfrmMain::SetRegValue();
	bool TfrmMain::writeLOG(AnsiString Msg,bool bAction);
	void  TfrmMain::SaveUserLogLine(AnsiString FileName,AnsiString FileLine);


public:		// User declarations
	HANDLE mx;
	int   Test_Step;
	AnsiString ERROR_MSG;
	AnsiString strTime;
	int  numScanResult,gPingTime;
	AnsiString gFileName;
	AnsiString g_DevVPID;
	bool AutoTest;
	bool TestWorking;
	bool g_bError;
	AnsiString gLAN_NAME;
	bool Ping(AnsiString IP,bool bLast,DWORD dwSpeed);
	bool bPlugOut;
	bool bBurnPASS;
	bool CheckDiskName();

	TStringList *TListInitLAN;
	void SaveLogLine(AnsiString FileName,AnsiString FileLine);
	__fastcall TfrmMain(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
extern DWORD WINAPI LANExecute(LPVOID Param);
//---------------------------------------------------------------------------
#endif
