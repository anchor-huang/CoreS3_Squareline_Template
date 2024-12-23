#include "wifi_config.hpp"
#include "time.h"
#include "UI/ui.h"


WifiState WifiConfig::currentState = WifiState::CONNECTING;
WiFiManager WifiConfig::wm;
bool WifiConfig::has_connected = false; 


WifiConfig::WifiConfig() :
      ntpServer("pool.ntp.org")
    , gmtOffset_sec(28800)  // UTC+8
    , daylightOffset_sec(0)
{}

WifiConfig& WifiConfig::getInstance() {
    static WifiConfig instance;
    return instance;
}

void WifiConfig::onConnectHandler(WiFiEvent_t event, WiFiEventInfo_t info) {
    WifiConfig::getInstance().handleConnect();
}

void WifiConfig::onLostHandler(WiFiEvent_t event, WiFiEventInfo_t info) {
    WifiConfig::getInstance().handleDisconnect();
}

void WifiConfig::onConfigHandler(WiFiManager* myWiFiManager) {
    WifiConfig::getInstance().handleConfigPortal();
}

void WifiConfig::handleConnect() {
    currentState = WifiState::CONNECTED;
    has_connected = true;
    log_i("WiFi Connected");
}

void WifiConfig::handleDisconnect() {
    if(has_connected) {
        log_i("WiFi Connection Lost");
        currentState = WifiState::LOST_CONNECTION;
    } else{
         currentState = WifiState::CONNECTING;
         log_i("WiFi Connection In Progress");
    }
    
}

void WifiConfig::handleConfigPortal() {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    currentState = WifiState::PROVISIONING;
    log_i("Entered config portal");
}

void WifiConfig::resetESP(uint8_t delaySeconds) {
    log_i("Restarting ESP32 in %d seconds...", delaySeconds);
    delay(delaySeconds * 1000);
    ESP.restart();
}

void WifiConfig::onTimeoutHandler() {
    log_w("Config Portal Timeout - Restarting device");
    WifiConfig::resetESP();
}

void WifiConfig::setup() {
    WifiConfig& self = getInstance();
    WiFi.mode(WIFI_STA);
    wm.setDebugOutput(true);
    wm.setConnectRetries(3);
    wm.setCaptivePortalEnable(true);
    wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(300);
    wm.setConnectTimeout(120);
    wm.setAPCallback(onConfigHandler);

    wm.setSaveConfigCallback([]() {
        log_i("WiFi Config Saved");
    });
    wm.setConfigPortalTimeoutCallback(onTimeoutHandler);
    WiFi.onEvent(onConnectHandler, ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(onLostHandler, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);


    if(wm.autoConnect(WIFI_SSID, WIFI_PASS)) {
        currentState = WifiState::CONNECTED;
        configTime(self.gmtOffset_sec, self.daylightOffset_sec, self.ntpServer);
        Serial.println("Connected successfully");
    } else {
        currentState = WifiState::PROVISIONING;
        Serial.println("Config portal running");
    }
}

void WifiConfig::resetSettings() {
    wm.resetSettings();
}
 
void WifiConfig::loop() {
    wm.process();
}

WifiState WifiConfig::getState()  {
    return currentState;
}

bool WifiConfig::isConfigPortalActive()  {
    return wm.getConfigPortalActive();
}

const char* WifiConfig::getSSID() {
    return WiFi.SSID().c_str();
}

int WifiConfig::getRSSI() {
    return WiFi.RSSI();
}

const char* WifiConfig::getIPAddress() {
    static char ip[16];
    IPAddress localIP = WiFi.localIP();
    snprintf(ip, sizeof(ip), "%d.%d.%d.%d", 
        localIP[0], localIP[1], localIP[2], localIP[3]);
    return ip;
}

