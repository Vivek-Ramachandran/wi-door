#include "stdafx.h"

#include "Wlan.h"


Wlan::Wlan()
{
}

Wlan::~Wlan()
{
    CloseHandle();
}

bool Wlan::IsNetAvailable( const char * ssid )
{
    bool result = false;

    if (OpenHandle()) {
        PWLAN_INTERFACE_INFO_LIST wlanInterfaces;

        if (WlanEnumInterfaces( m_handle, NULL, &wlanInterfaces ) == ERROR_SUCCESS) {
            for (int i = 0; i < wlanInterfaces->dwNumberOfItems; i++) {
                if (wlanInterfaces->InterfaceInfo[i].isState == wlan_interface_state_connected) {
                    PWLAN_AVAILABLE_NETWORK_LIST wlanNetworks;

                    if (WlanGetAvailableNetworkList( m_handle, &wlanInterfaces->InterfaceInfo[i].InterfaceGuid, 0, NULL, &wlanNetworks ) == ERROR_SUCCESS) {
                        for (int j = 0; j < wlanNetworks->dwNumberOfItems; j++) {
                            if (_strnicmp( (const char *) wlanNetworks->Network[j].dot11Ssid.ucSSID, ssid, wlanNetworks->Network[j].dot11Ssid.uSSIDLength ) == 0) {
                                result = true;
                                break;
                            }
                        }

                        WlanFreeMemory( wlanNetworks );
                    }
                }
            }

            WlanFreeMemory( wlanInterfaces );
        }
    }

    return result;
}

bool Wlan::StartHostedNet( const char * ssid, const char * key, int maxPeers )
{
    bool result = false;

    if (OpenHandle()) {
        WLAN_HOSTED_NETWORK_REASON reason = {};
        WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS settings = {};

        settings.dwMaxNumberOfPeers = maxPeers;
        size_t ssidLen = strlen( ssid );
        settings.hostedNetworkSSID.uSSIDLength = ssidLen;
        memcpy( settings.hostedNetworkSSID.ucSSID, (void*) ssid, ssidLen );

        if (WlanHostedNetworkSetProperty(
            m_handle,
            WLAN_HOSTED_NETWORK_OPCODE::wlan_hosted_network_opcode_connection_settings,
            sizeof( WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS ),
            &settings,
            &reason,
            NULL ) == ERROR_SUCCESS) {

            size_t keyLength = strlen( key ) + 1;

            if (WlanHostedNetworkSetSecondaryKey( m_handle, keyLength, (PUCHAR) key, TRUE, TRUE, &reason, NULL ) == ERROR_SUCCESS) {
                result = (WlanHostedNetworkStartUsing( m_handle, &reason, NULL ) == ERROR_SUCCESS);
            }
        }
    }

    return result;
}

void Wlan::StopHostedNet()
{
    if (OpenHandle()) {
        WLAN_HOSTED_NETWORK_REASON reason = {};

        WlanHostedNetworkStopUsing( m_handle, &reason, NULL );
    }
}

bool Wlan::OpenHandle()
{
    bool result = true;
    DWORD version;

    if (m_handle == NULL) {
        if (WlanOpenHandle( 2, NULL, &version, &m_handle ) != ERROR_SUCCESS) {
            m_handle = NULL;
            result = false;
        }
    }

    return result;
}

void Wlan::CloseHandle()
{
    if (m_handle != NULL) {
        WlanCloseHandle( m_handle, NULL );
        m_handle = NULL;
    }
}
