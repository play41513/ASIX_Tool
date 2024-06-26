//---------------------------------------------------------------------------

#ifndef Hub_funH
#define Hub_funH
#include <vcl.h>
#include <fstream>
#include <string>
#include <Registry.hpp>
#include "Iphlpapi.h"
#include <iostream>
#pragma link "Iphlpapi.lib"
using namespace std;
//---------------------------------------------------------------------------
class Port_test {
public:
	bool ChangeLanSpeed(AnsiString VPID, AnsiString GUID,
	AnsiString LANname);//切換網速
	void Delay(DWORD iMilliSeconds); // delay time
	char* lan_find_inf(char *pData, char *pname); // 查找字串回傳後面的字串
	AnsiString getLanMAC(AnsiString LANname);//取得網卡MAC
	AnsiString GetLANName(bool GetMAC);

	bool cmd_restartdevice();//重啟設備
	bool Port_test::CheckDir();
	bool Port_test::CheckSpeed(AnsiString LANNAME,DWORD dwSpeed);
private:
	AnsiString getDHCP_Server_IP(AnsiString LANname); // 得到DHCP Server IP
	AnsiString getLanIP(AnsiString LANname); // 得到網卡IP
	AnsiString getLanGUID(AnsiString LANname); // 得到網卡GUID
	void Port_test::Search_System_Exist_Lan_Speed(AnsiString LANHUID,
		AnsiString VPID, AnsiString LANname, AnsiString value);



};
#endif
