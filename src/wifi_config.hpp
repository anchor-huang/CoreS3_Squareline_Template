#pragma once

#include <WiFiManager.h>
#include "global.hpp"

enum class WifiState {
    CONNECTING,
    PROVISIONING,
    CONNECTED,
    LOST_CONNECTION
};

class  WifiConfig {
private:
    static WiFiManager wm;
    static WifiState currentState;
    static bool has_connected;
    const char* ntpServer;
    const long gmtOffset_sec;
    const int daylightOffset_sec;

    static void onConnectHandler(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onLostHandler(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onConfigHandler(WiFiManager* myWiFiManager);
    static void onTimeoutHandler();
    static void resetESP(uint8_t delaySeconds = 1);

    void handleConnect();
    void handleDisconnect();
    void handleConfigPortal();

    WifiConfig();
    
public:
    static WifiConfig& getInstance();
    static void setup();
    static void loop();
    static WifiState getState() ;
    static bool isConfigPortalActive() ;
    static const char* getSSID();
    static int getRSSI();
    static const char* getIPAddress();
    static void resetSettings();
};

