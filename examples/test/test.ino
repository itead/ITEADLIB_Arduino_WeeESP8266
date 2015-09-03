/**
 * @example test.ino
 * @brief The test demo of library WeeESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.03
 * 
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "ESP8266.h"

#define SSID        "ITEAD"
#define PASSWORD    "12345678"

ESP8266 wifi(Serial1,9600);

void setup(void)
{
    Serial.begin(9600);
    Serial.print("setup begin\r\n");
    if(wifi.setUart(9600,2)){
    Serial.println("set uart is ok ");
    }
    else{
    Serial.println("set uart is error");
    }
   Serial.println(wifi.getWifiModeList().c_str());
     if(wifi.setOprToSoftAP(2,2)){
       Serial.println("it is STA");
     }
     if(wifi.setOprToStation(2,2)){
         Serial.println("it is AP");
     }
     if(wifi.setOprToStationSoftAP(2,2)){
         Serial.println("it is AP+SoftAP");
     }
     Serial.println(wifi.getAPList().c_str());
     wifi.joinAP(SSID,PASSWORD);
     Serial.println(wifi.getNowConecAp(1).c_str());
     if(wifi.leaveAP())
     {
       Serial.println("it is leave");
     }
     Serial.println(wifi.getNowConecAp(1).c_str());
     if(wifi.setSoftAPParam("aaa","12345678"))
     {
         Serial.println("it is set param ok");
     }
     Serial.println(wifi.getSoftAPParam());
     Serial.println(wifi.getJoinedDeviceIP().c_str());
     Serial.print("the state of DHCP:");
     Serial.println(wifi.getDHCP().c_str());
     if(wifi.setDHCP(2,1)){
       Serial.println("it is set DHCP OK");
     }
     Serial.println(wifi.getDHCP().c_str());
     if(wifi.setAutoConnect(0)){
       Serial.println("take off auto connect ok");
     }
     Serial.print("get the station mac: ");
     Serial.println(wifi.getStationMac().c_str());
     if(wifi.setStationMac("18:fe:35:98:d3:7b")){
       Serial.println("set station mac is ok ");
     }
     else {
       Serial.println("it is error");
     }
     Serial.print("get the station mac: ");
     Serial.println(wifi.getStationMac().c_str());
     Serial.print("get the station's ip");
     Serial.println(wifi.getStationIp().c_str());
   
    if(wifi.setStationIp("192.168.1.6","192.168.1.1","255.255.255.0")){
      Serial.println("set station's ip is ok");
    }
    else{
      Serial.println("set station's ip is error");
    }
       Serial.print("get the station's ip");
     Serial.println(wifi.getStationIp().c_str());
     
     Serial.print("get the ap's ap");
     Serial.println(wifi.getAPIp().c_str());
    
     if(wifi.setAPIp("192.168.1.1")){
       Serial.println("set ap's ip is ok");
     }
     else
     {
       Serial.println("set ap's is is error");
     }
       Serial.print("get the ap's ap");
     Serial.println(wifi.getAPIp().c_str());
     
     if(wifi.startSmartConfig(1)){
       Serial.println("start smartconfig is ok");
     }
     else{
       Serial.println("start smartconfig is error");
     }
        if(wifi.stopSmartConfig()){
       Serial.println("stop smartconfig is ok");
     }
     else{
       Serial.println("stop smartconfig is error");
     }
     Serial.print("get the current status of connection:");
     Serial.println(wifi.getIPStatus().c_str());
     if(wifi.saveTransLink(1,"192.168.1.18",1006)){
       Serial.println("save trans link is ok");
     }
     else{
       Serial.println("save trans link is error");
     }
      Serial.println("setup end\r\n");
}

void loop(void)
{
}

