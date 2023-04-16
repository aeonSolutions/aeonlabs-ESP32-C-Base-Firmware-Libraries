#ifndef ROUTES_H
#define ROUTES_H

#include <WebServer.h>

#define HTML_HEAD "<head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Settings</title><link rel='stylesheet' href='/css'></head>"
#define MIME_HTML F("text/html")

class Routes {
  public:
    Routes(WebServer* webServer);
    void handleRoot();
    void handleWiFi();
    void handleWiFiScript();
    void handleWiFiResult();
    void handleWiFiSave();
    void handleRoomName();
    void handleRoomNameSave();
    void handleWeather();
    void handleWeatherSave();
    void handleRequestRestart();
    void handleStatus();
    void handleCommand();
    void handleCss();
    void handleNotFound();
    static bool shouldRestart;

    uint8_t RSSIToPercent(long rssi);
  private:
    WebServer* server;
};

#endif
