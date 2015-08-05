#pragma once


#ifndef __WLAN_H
#define __WLAN_H


#include "windows.h"
#include "wlanapi.h"


class Wlan {
protected:
    HANDLE m_handle = NULL;

    bool OpenHandle();
    void CloseHandle();

public:
    Wlan();
    ~Wlan();

    bool IsNetAvailable( const char * ssid );
    bool StartHostedNet( const char * ssid, const char * key, int maxPeers );
    void StopHostedNet();
};


#endif
