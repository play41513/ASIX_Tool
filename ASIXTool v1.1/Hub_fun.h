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
	void IntiNetwordCard();
	bool ChangeLanSpeed(AnsiString VPID, AnsiString GUID,
	AnsiString LANname);//�������t
	void Delay(DWORD iMilliSeconds); // delay time
	char* lan_find_inf(char *pData, char *pname); // �d��r��^�ǫ᭱���r��
	AnsiString getLanMAC(AnsiString LANname);//���o���dMAC
	AnsiString GetLANName();
	void AddNewMAC(AnsiString LANHUID,AnsiString VPID, AnsiString LANname
		, AnsiString value);//�s�W�Ȯ�MAC
	bool cmd_restartdevice();//���ҳ]��
	bool Port_test::CheckDir();
	bool Port_test::CheckSpeed(AnsiString LANNAME,DWORD dwSpeed);
private:
	AnsiString getDHCP_Server_IP(AnsiString LANname); // �o��DHCP Server IP
	AnsiString getLanIP(AnsiString LANname); // �o����dIP
	AnsiString getLanGUID(AnsiString LANname); // �o����dGUID
	void Port_test::Search_System_Exist_Lan_Speed(AnsiString LANHUID,
		AnsiString VPID, AnsiString LANname, AnsiString value);



};
#endif