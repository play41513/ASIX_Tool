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
#define CHECK_BURN_MAC		8

#define BARCODE_FINISH	1
#define BARCODE_CHANEL  2

#define ERROR_MAC_OUTRANGE		1
#define ERROR_MAC_REPEAT    	2
#define VERIFTY_MAC_PASS     	3



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

//MAC �N���Ϭq �}�l~���� �r��
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
	TPopupMenu *PopupMenu1;
	TMenuItem *N1;
	TTimer *TimerMain;
	TPanel *plDebug;
	TMemo *moDebug;
	TBitBtn *btnLoadMAC;
	TBitBtn *btnLoadNetworkCardName;
	TPanel *plBarcode;
	TLabel *lbBarcode;
	TPanel *Panel12;
	TBitBtn *btnBarcodeChanel;
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
	TPanel *plBarcodeData;
	TPanel *Panel28;
	TMaskEdit *mskedBurntMacStart;
	TPanel *Panel29;
	TMaskEdit *mskedBurntMacLast;
	TPanel *Panel31;
	TPanel *plCount;
	TPanel *plBarcodeSet;
	TPanel *Panel43;
	TEdit *edtSetWorkOrderNumber;
	TPanel *Panel38;
	TEdit *edtSetEmployeeID;
	TPanel *plLeft;
	TGroupBox *gbBurnContent;
	TLabel *Label16;
	TCheckBox *ckbAutoIncrease;
	TCheckBox *ckbBarcode;
	TCheckBox *ckbBarcodeVerify;
	TPanel *Panel41;
	TButton *btn_start;
	TMaskEdit *Edit_resetMAC;
	TGroupBox *GroupBox2;
	TPanel *Panel10;
	TLabel *Label2;
	TPanel *plTime;
	TPanel *Panel8;
	TPanel *Panel9;
	TPanel *lab_devicename;
	TPanel *Panel2;
	TPanel *current_mac;
	TPanel *plResult;
	TImage *Image1;
	TBitBtn *btnBarcodeClear;
	TPanel *plTestMode;
	TPanel *plUSBLOG;
	TImage *ImgDisk;
	void __fastcall btn_startClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TimerMainTimer(TObject *Sender);
	void __fastcall btnLoadMACClick(TObject *Sender);
	void __fastcall btnLoadNetworkCardNameClick(TObject *Sender);
	void __fastcall edtBarcodeMACChange(TObject *Sender);
	void __fastcall btnBarcodeChanelClick(TObject *Sender);
	void __fastcall edtBarcodeMACKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall READClick(TObject *Sender);
	void __fastcall WRITEClick(TObject *Sender);
	void __fastcall CHECKClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall btnBarcodeClearClick(TObject *Sender);


private:	// User declarations
	bool writeLOG(AnsiString Msg,bool bAction);
	bool SaveUserLogLine(AnsiString FileName,AnsiString FileLine);
	bool CheckDiskName();
	bool DUTB();

	void AutoIncrease();
	void ReadRegValue();
	void SetRegValue();
	void ShowSettingForm();
	void NewFilePath(AnsiString Path);
	void RegDriver();
	void FindLogFile();


	int HexToInt(AnsiString HexStr);
	DWORD CheckMAC(AnsiString MAC);
	DWORD g_Test_Number;

public:		// User declarations
	HANDLE mx;

	bool bReWork;
	bool TestWorking;
	int Test_Step;
	AnsiString ERROR_MSG;
	AnsiString g_DevVPID;
	AnsiString gLAN_NAME;

	AnsiString ReadASIX_MAC();
	__fastcall TfrmMain(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif