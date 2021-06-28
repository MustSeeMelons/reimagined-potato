#include "Routes.h"

String getForm(String ssid, String password, String city, String lat, String lon, bool showSuccess)
{
    String content = "";
    content += "<!DOCTYPE html>";
    content += "<html>";
    content += "<head>";
    content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    content += "</head>";
    content += "<style>";
    content += "body {";
    content += "padding: 0;";
    content += "margin: 0;";
    content += "width: 100%;";
    content += "height: 100vh;";
    content += "display: flex;";
    content += "justify-content: center;";
    content += "}";
    content += ".main {";
    content += "box-sizing: border-box;";
    content += "display: flex;";
    content += "flex-direction: column;";
    content += "align-items: stretch;";
    content += "width: 100%;";
    content += "}";
    content += ".row {";
    content += "padding: 10px;";
    content += "box-sizing: border-box;";
    content += "display: grid;";
    content += "grid-template-columns: repeat(2, 1fr);";
    content += "width: 100%;";
    content += "}";
    content += ".success {";
    content += "display: none;";
    content += "background-color: #1ef801;";
    content += "color: #ffffff;";
    content += "text-transform: uppercase;";
    content += "text-align: center;";
    content += "font-weight: 700;";
    content += "padding: 10px;";
    content += "margin-top: 10px;";
    content += "}";
    content += ".show-success {";
    content += "display: inline-block;";
    content += "}";
    content += "@media only screen and (min-width: 300px) {";
    content += ".main {";
    content += "max-width: 300px;";
    content += "}";
    content += "}";
    content += "</style>";
    content += "<body>";
    content += "<form class=\"main\" action=\"submit\">";
    content += "<div class=\"row\">";
    content += "<label for=\"ssid\">SSID:</label>";
    content += "<input id=\"ssid\" name=\"ssid\" type=\"text\" value=\"{{ssid}}\">";
    content += "</div>";
    content += "<div class=\"row\">";
    content += "<label for=\"password\">Password:</label>";
    content += "<input id=\"password\" name=\"password\" type=\"text\" value=\"{{password}}\">";
    content += "</div>";
    content += "<div class=\"row\">";
    content += "<label for=\"city\">City:</label>";
    content += "<input id=\"city\" name=\"city\" type=\"text\" value=\"{{city}}\">";
    content += "</div>";
    content += "<div class=\"row\">";
    content += "<label for=\"lon\">Longitude:</label>";
    content += "<input id=\"lon\" name=\"lon\" type=\"text\" value=\"{{lon}}\">";
    content += "</div>";
    content += "<div class=\"row\">";
    content += "<label for=\"lat\">Lattitude:</label>";
    content += "<input id=\"lat\" name=\"lat\" type=\"text\" value=\"{{lat}}\">";
    content += "</div>";
    content += "<button>Submit</button>";
    content += "<div class=\"success {{success}}\">";
    content += "success!";
    content += "</div>";
    content += "</form>";
    content += "</body>";
    content += "</html>";

    content.replace("{{ssid}}", ssid);
    content.replace("{{password}}", password);
    content.replace("{{city}}", city);
    content.replace("{{lon}}", lon);
    content.replace("{{lat}}", lat);

    content.replace("{{success}}", showSuccess ? "show-success" : "");

    return content;
}

std::function<void(void)> handleRoot(ESP8266WebServer *server)
{
    return [server]()
    {
        EEPROMData data = getEEPROMData();

        server->send(200, "text/html", getForm(data.ssid.data, data.password.data, data.city.data, data.lat.data, data.lng.data));
        return;
    };
}

std::function<void(void)> handleSubmit(ESP8266WebServer *server, std::function<void(void)> callback)
{
    return [server, callback]()
    {
        Serial.println(server->arg("ssid"));
        Serial.println(server->arg("password"));
        Serial.println(server->arg("city"));
        Serial.println(server->arg("lat"));
        Serial.println(server->arg("lon"));

        String sSsid = server->arg("ssid");
        char ssid[sSsid.length() + 1];
        sSsid.toCharArray(ssid, sSsid.length() + 1);
        EEPROMRecord ssidRecord = getSaveEEPROMStringData(0, ssid, true);

        String sPassword = server->arg("password");
        char password[sPassword.length() + 1];
        sPassword.toCharArray(password, sPassword.length() + 1);
        EEPROMRecord passwordRecord = getSaveEEPROMStringData(ssidRecord.nextOffset, password, true);

        String sCity = server->arg("city");
        char city[sCity.length() + 1];
        sCity.toCharArray(city, sCity.length() + 1);
        EEPROMRecord cityRecord = getSaveEEPROMStringData(passwordRecord.nextOffset, city, true);

        String sLat = server->arg("lat");
        char lat[sLat.length() + 1];
        sLat.toCharArray(lat, sLat.length() + 1);
        EEPROMRecord latRecord = getSaveEEPROMStringData(cityRecord.nextOffset, lat, true);

        String sLon = server->arg("lon");
        char lon[sLon.length() + 1];
        sLon.toCharArray(lon, sLon.length() + 1);
        getSaveEEPROMStringData(latRecord.nextOffset, lon, true);

        server->send(200, "text/html", getForm(sSsid, password, city, lat, lon, true));
        callback();
        return;
    };
}