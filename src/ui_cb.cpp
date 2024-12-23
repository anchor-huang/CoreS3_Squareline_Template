
#include "lvgl.h"
#include "util.h"
#include "UI/ui.h"
#include "UI/ui_events.h"
#include "qrcode.h"
#include "global.hpp"
#include <cstring>
#include "wifi_config.hpp"

#ifdef __cplusplus
extern "C" {
#endif
lv_obj_t* ui_qrCode;
lv_timer_t* wifi_timer;

void onResetWifi(lv_event_t * e){
    WifiConfig::resetSettings();
    ESP.restart();
}

void PlayStartupMusic(lv_event_t * e){
    play_wave("/poweron_2_5s.wav");
}

void onWifiConnected(lv_event_t * e){
    // Implement logic to handle wifi connected event
    lv_obj_add_flag(ui_SpinnerWifiConnecting, LV_OBJ_FLAG_HIDDEN);   
    lv_obj_remove_flag(ui_PanelWifiStatus, LV_OBJ_FLAG_HIDDEN);   
    lv_obj_add_flag(ui_qrCode, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text_fmt(ui_LabelWifiStatus, "Connected");
 
    const char* ip = WifiConfig::getIPAddress();
    lv_label_set_text_fmt(ui_LabelSSID, "SSID: %s", WiFi.SSID().c_str());
    lv_label_set_text_fmt(ui_LabelRSSI, "RSSI: %d dBm", WiFi.RSSI());
    lv_label_set_text_fmt(ui_LabelIP, "IP: %s", ip);
}

void onWifiConfig(lv_event_t * e){
    // Implement logic to handle wifi config page event
    lv_obj_add_flag(ui_SpinnerWifiConnecting, LV_OBJ_FLAG_HIDDEN);   
    lv_obj_add_flag(ui_PanelWifiStatus, LV_OBJ_FLAG_HIDDEN);   
    lv_obj_remove_flag(ui_qrCode, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text_fmt(ui_LabelWifiStatus, "Scan QRCode for Wifi Config");
}
void onWifiConnecting(lv_event_t * e){
    // Implement logic to handle wifi connecting event
    lv_obj_remove_flag(ui_SpinnerWifiConnecting, LV_OBJ_FLAG_HIDDEN);   
    lv_obj_add_flag(ui_PanelWifiStatus, LV_OBJ_FLAG_HIDDEN);   
    lv_obj_add_flag(ui_qrCode, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text_fmt(ui_LabelWifiStatus, "Connecting");
}

void onWifiTimerUpdate(lv_timer_t* timer) {
    switch(WifiConfig::getState()){
        case WifiState::CONNECTED:
            onWifiConnected(nullptr);
            break;
        case WifiState::PROVISIONING:
            onWifiConfig(nullptr);
            break;
        case WifiState::CONNECTING:
            onWifiConnecting(nullptr);
            break;
        case WifiState::LOST_CONNECTION:
            onWifiConnecting(nullptr);
            lv_label_set_text_fmt(ui_LabelWifiStatus, "Lost Connection");
            break;
    }
}

void onWifiConfigPageLoaded(lv_event_t * e){
    ui_qrCode = lv_qrcode_create(ui_WifiConfigPage);
    lv_qrcode_set_size(ui_qrCode, 100);
    lv_qrcode_set_dark_color(ui_qrCode, lv_color_black());
    lv_qrcode_set_light_color(ui_qrCode, lv_color_white());
    const char* qrcode_str=WIFI_QRCODE;
    lv_qrcode_update(ui_qrCode, qrcode_str,  strlen(qrcode_str));
    lv_obj_set_align( ui_qrCode, LV_ALIGN_CENTER );
    lv_obj_set_x( ui_qrCode, 65 );
    lv_obj_set_y( ui_qrCode, -21 );
    lv_obj_add_flag(ui_qrCode, LV_OBJ_FLAG_HIDDEN);   

    wifi_timer = lv_timer_create(onWifiTimerUpdate, 30, ui_WifiConfigPage);

}

void onWifiConfigPageUnloaded(lv_event_t * e){
    lv_timer_del(wifi_timer);
}


#ifdef __cplusplus
} /*extern "C"*/
#endif