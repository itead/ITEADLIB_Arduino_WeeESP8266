/**
 * @file ESP8266.cpp
 * @brief The implementation of class ESP8266. 
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.02
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

#define LOG_OUTPUT_DEBUG            (1)
#define LOG_OUTPUT_DEBUG_PREFIX     (1)

#define logDebug(arg)\
    do {\
        if (LOG_OUTPUT_DEBUG)\
        {\
            if (LOG_OUTPUT_DEBUG_PREFIX)\
            {\
                Serial.print("[LOG Debug: ");\
                Serial.print((const char*)__FILE__);\
                Serial.print(",");\
                Serial.print((unsigned int)__LINE__);\
                Serial.print(",");\
                Serial.print((const char*)__FUNCTION__);\
                Serial.print("] ");\
            }\
            Serial.print(arg);\
        }\
    } while(0)

#ifdef ESP8266_USE_SOFTWARE_SERIAL
ESP8266::ESP8266(SoftwareSerial &uart, uint32_t baud): m_puart(&uart)
{
    m_puart->begin(baud);
    rx_empty();
}
#else
ESP8266::ESP8266(HardwareSerial &uart, uint32_t baud): m_puart(&uart)
{
    m_puart->begin(baud);
    rx_empty();
}
#endif

bool ESP8266::kick(void)
{
    return eAT();
}

bool ESP8266::restart(void)
{
    unsigned long start;
    if (eATRST()) {
        delay(2000);
        start = millis();
        while (millis() - start < 3000) {
            if (eAT()) {
                delay(1500); /* Waiting for stable */
                return true;
            }
            delay(100);
        }
    }
    return false;
}

String ESP8266::getVersion(void)
{
    String version;
    eATGMR(version);
    return version;
}

bool ESP8266::setOprToStation(void)
{
    uint8_t mode;
    if (!qATCWMODE(&mode)) {
        return false;
    }
    if (mode == 1) {
        return true;
    } else {
        if (sATCWMODE(1) && restart()) {
            return true;
        } else {
            return false;
        }
    }
}

bool ESP8266::setOprToSoftAP(void)
{
    uint8_t mode;
    if (!qATCWMODE(&mode)) {
        return false;
    }
    if (mode == 2) {
        return true;
    } else {
        if (sATCWMODE(2) && restart()) {
            return true;
        } else {
            return false;
        }
    }
}

bool ESP8266::setOprToStationSoftAP(void)
{
    uint8_t mode;
    if (!qATCWMODE(&mode)) {
        return false;
    }
    if (mode == 3) {
        return true;
    } else {
        if (sATCWMODE(3) && restart()) {
            return true;
        } else {
            return false;
        }
    }
}

String ESP8266::getAPList(void)
{
    String list;
    eATCWLAP(list);
    return list;
}

bool ESP8266::joinAP(String ssid, String pwd)
{
    return sATCWJAP(ssid, pwd);
}

bool ESP8266::enableClientDHCP(uint8_t mode, boolean enabled)
{
    return sATCWDHCP(mode, enabled);
}

bool ESP8266::leaveAP(void)
{
    return eATCWQAP();
}

bool ESP8266::setSoftAPParam(String ssid, String pwd, uint8_t chl, uint8_t ecn)
{
    return sATCWSAP(ssid, pwd, chl, ecn);
}

String ESP8266::getJoinedDeviceIP(void)
{
    String list;
    eATCWLIF(list);
    return list;
}

String ESP8266::getIPStatus(void)
{
    String list;
    eATCIPSTATUS(list);
    return list;
}

String ESP8266::getLocalIP(void)
{
    String list;
    eATCIFSR(list);
    return list;
}

bool ESP8266::enableMUX(void)
{
    return sATCIPMUX(1);
}

bool ESP8266::disableMUX(void)
{
    return sATCIPMUX(0);
}

bool ESP8266::createTCP(String addr, uint32_t port)
{
    return sATCIPSTARTSingle("TCP", addr, port);
}

bool ESP8266::releaseTCP(void)
{
    return eATCIPCLOSESingle();
}

bool ESP8266::registerUDP(String addr, uint32_t port)
{
    return sATCIPSTARTSingle("UDP", addr, port);
}

bool ESP8266::unregisterUDP(void)
{
    return eATCIPCLOSESingle();
}

bool ESP8266::createTCP(uint8_t mux_id, String addr, uint32_t port)
{
    return sATCIPSTARTMultiple(mux_id, "TCP", addr, port);
}

bool ESP8266::releaseTCP(uint8_t mux_id)
{
    return sATCIPCLOSEMulitple(mux_id);
}

bool ESP8266::registerUDP(uint8_t mux_id, String addr, uint32_t port)
{
    return sATCIPSTARTMultiple(mux_id, "UDP", addr, port);
}

bool ESP8266::unregisterUDP(uint8_t mux_id)
{
    return sATCIPCLOSEMulitple(mux_id);
}

bool ESP8266::setTCPServerTimeout(uint32_t timeout)
{
    return sATCIPSTO(timeout);
}

bool ESP8266::startTCPServer(uint32_t port)
{
    if (sATCIPSERVER(1, port)) {
        return true;
    }
    return false;
}

bool ESP8266::stopTCPServer(void)
{
    sATCIPSERVER(0);
    restart();
    return false;
}

bool ESP8266::startServer(uint32_t port)
{
    return startTCPServer(port);
}

bool ESP8266::stopServer(void)
{
    return stopTCPServer();
}

bool ESP8266::send(const uint8_t *buffer, uint32_t len)
{
    return sATCIPSENDSingle(buffer, len);
}

bool ESP8266::send(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
    return sATCIPSENDMultiple(mux_id, buffer, len);
}

uint32_t ESP8266::recv(uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    return recvPkg(buffer, buffer_size, NULL, timeout, NULL);
}

uint32_t ESP8266::recv(uint8_t mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    uint8_t id;
    uint32_t ret;
    ret = recvPkg(buffer, buffer_size, NULL, timeout, &id);
    if (ret > 0 && id == mux_id) {
        return ret;
    }
    return 0;
}

uint32_t ESP8266::recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
    return recvPkg(buffer, buffer_size, NULL, timeout, coming_mux_id);
}

/*----------------------------------------------------------------------------*/
/* +IPD,<id>,<len>:<data> */
/* +IPD,<len>:<data> */

uint32_t ESP8266::recvPkg(uint8_t *buffer, uint32_t buffer_size, uint32_t *data_len, uint32_t timeout, uint8_t *coming_mux_id)
{
    String data;
    char a;
    int32_t index_PIPDcomma = -1;
    int32_t index_colon = -1; /* : */
    int32_t index_comma = -1; /* , */
    int32_t len = -1;
    int8_t id = -1;
    bool has_data = false;
    uint32_t ret;
    unsigned long start;
    uint32_t i;
    
    if (buffer == NULL) {
        return 0;
    }
    
    start = millis();
    while (millis() - start < timeout) {
        if(m_puart->available() > 0) {
            a = m_puart->read();
            data += a;
        }
        
        index_PIPDcomma = data.indexOf("+IPD,");
        if (index_PIPDcomma != -1) {
            index_colon = data.indexOf(':', index_PIPDcomma + 5);
            if (index_colon != -1) {
                index_comma = data.indexOf(',', index_PIPDcomma + 5);
                /* +IPD,id,len:data */
                if (index_comma != -1 && index_comma < index_colon) { 
                    id = data.substring(index_PIPDcomma + 5, index_comma).toInt();
                    if (id < 0 || id > 4) {
                        return 0;
                    }
                    len = data.substring(index_comma + 1, index_colon).toInt();
                    if (len <= 0) {
                        return 0;
                    }
                } else { /* +IPD,len:data */
                    len = data.substring(index_PIPDcomma + 5, index_colon).toInt();
                    if (len <= 0) {
                        return 0;
                    }
                }
                has_data = true;
                break;
            }
        }
    }
    
    if (has_data) {
        i = 0;
        ret = len > buffer_size ? buffer_size : len;
        start = millis();
        while (millis() - start < 3000) {
            while(m_puart->available() > 0 && i < ret) {
                a = m_puart->read();
                buffer[i++] = a;
            }
            if (i == ret) {
                rx_empty();
                if (data_len) {
                    *data_len = len;    
                }
                if (index_comma != -1 && coming_mux_id) {
                    *coming_mux_id = id;
                }
                return ret;
            }
        }
    }
    return 0;
}

void ESP8266::rx_empty(void) 
{
    while(m_puart->available() > 0) {
        m_puart->read();
    }
}

String ESP8266::recvString(String target, uint32_t timeout)
{
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(m_puart->available() > 0) {
            a = m_puart->read();
			if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target) != -1) {
            break;
        }   
    }
    return data;
}

String ESP8266::recvString(String target1, String target2, uint32_t timeout)
{
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(m_puart->available() > 0) {
            a = m_puart->read();
			if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target1) != -1) {
            break;
        } else if (data.indexOf(target2) != -1) {
            break;
        }
    }
    return data;
}

String ESP8266::recvString(String target1, String target2, String target3, uint32_t timeout)
{
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(m_puart->available() > 0) {
            a = m_puart->read();
			if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target1) != -1) {
            break;
        } else if (data.indexOf(target2) != -1) {
            break;
        } else if (data.indexOf(target3) != -1) {
            break;
        }
    }
    return data;
}

bool ESP8266::recvFind(String target, uint32_t timeout)
{
    String data_tmp;
    data_tmp = recvString(target, timeout);
    if (data_tmp.indexOf(target) != -1) {
        return true;
    }
    return false;
}

bool ESP8266::recvFindAndFilter(String target, String begin, String end, String &data, uint32_t timeout)
{
    String data_tmp;
    data_tmp = recvString(target, timeout);
    if (data_tmp.indexOf(target) != -1) {
        int32_t index1 = data_tmp.indexOf(begin);
        int32_t index2 = data_tmp.indexOf(end);
        if (index1 != -1 && index2 != -1) {
            index1 += begin.length();
            data = data_tmp.substring(index1, index2);
            return true;
        }
    }
    data = "";
    return false;
}

bool ESP8266::eAT(void)
{
    rx_empty();
    m_puart->println("AT");
    return recvFind("OK");
}

bool ESP8266::eATRST(void) 
{
    rx_empty();
    m_puart->println("AT+RST");
    return recvFind("OK");
}

bool ESP8266::eATGMR(String &version)
{
    rx_empty();
    m_puart->println("AT+GMR");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", version); 
}

bool ESP8266::qATCWMODE(uint8_t *mode) 
{
    String str_mode;
    bool ret;
    if (!mode) {
        return false;
    }
    rx_empty();
    m_puart->println("AT+CWMODE?");
    ret = recvFindAndFilter("OK", "+CWMODE:", "\r\n\r\nOK", str_mode); 
    if (ret) {
        *mode = (uint8_t)str_mode.toInt();
        return true;
    } else {
        return false;
    }
}

bool ESP8266::sATCWMODE(uint8_t mode)
{
    String data;
    rx_empty();
    m_puart->print("AT+CWMODE=");
    m_puart->println(mode);
    
    data = recvString("OK", "no change");
    if (data.indexOf("OK") != -1 || data.indexOf("no change") != -1) {
        return true;
    }
    return false;
}

bool ESP8266::sATCWJAP(String ssid, String pwd)
{
    String data;
    rx_empty();
    m_puart->print("AT+CWJAP=\"");
    m_puart->print(ssid);
    m_puart->print("\",\"");
    m_puart->print(pwd);
    m_puart->println("\"");
    
    data = recvString("OK", "FAIL", 10000);
    if (data.indexOf("OK") != -1) {
        return true;
    }
    return false;
}

bool ESP8266::sATCWDHCP(uint8_t mode, boolean enabled)
{
	String strEn = "0";
	if (enabled) {
		strEn = "1";
	}
	
	
    String data;
    rx_empty();
    m_puart->print("AT+CWDHCP=");
    m_puart->print(strEn);
    m_puart->print(",");
    m_puart->println(mode);
    
    data = recvString("OK", "FAIL", 10000);
    if (data.indexOf("OK") != -1) {
        return true;
    }
    return false;
}

bool ESP8266::eATCWLAP(String &list)
{
    String data;
    rx_empty();
    m_puart->println("AT+CWLAP");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list, 10000);
}

bool ESP8266::eATCWQAP(void)
{
    String data;
    rx_empty();
    m_puart->println("AT+CWQAP");
    return recvFind("OK");
}

bool ESP8266::sATCWSAP(String ssid, String pwd, uint8_t chl, uint8_t ecn)
{
    String data;
    rx_empty();
    m_puart->print("AT+CWSAP=\"");
    m_puart->print(ssid);
    m_puart->print("\",\"");
    m_puart->print(pwd);
    m_puart->print("\",");
    m_puart->print(chl);
    m_puart->print(",");
    m_puart->println(ecn);
    
    data = recvString("OK", "ERROR", 5000);
    if (data.indexOf("OK") != -1) {
        return true;
    }
    return false;
}

bool ESP8266::eATCWLIF(String &list)
{
    String data;
    rx_empty();
    m_puart->println("AT+CWLIF");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list);
}
bool ESP8266::eATCIPSTATUS(String &list)
{
    String data;
    delay(100);
    rx_empty();
    m_puart->println("AT+CIPSTATUS");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list);
}
bool ESP8266::sATCIPSTARTSingle(String type, String addr, uint32_t port)
{
    String data;
    rx_empty();
    m_puart->print("AT+CIPSTART=\"");
    m_puart->print(type);
    m_puart->print("\",\"");
    m_puart->print(addr);
    m_puart->print("\",");
    m_puart->println(port);
    
    data = recvString("OK", "ERROR", "ALREADY CONNECT", 10000);
    if (data.indexOf("OK") != -1 || data.indexOf("ALREADY CONNECT") != -1) {
        return true;
    }
    return false;
}
bool ESP8266::sATCIPSTARTMultiple(uint8_t mux_id, String type, String addr, uint32_t port)
{
    String data;
    rx_empty();
    m_puart->print("AT+CIPSTART=");
    m_puart->print(mux_id);
    m_puart->print(",\"");
    m_puart->print(type);
    m_puart->print("\",\"");
    m_puart->print(addr);
    m_puart->print("\",");
    m_puart->println(port);
    
    data = recvString("OK", "ERROR", "ALREADY CONNECT", 10000);
    if (data.indexOf("OK") != -1 || data.indexOf("ALREADY CONNECT") != -1) {
        return true;
    }
    return false;
}
bool ESP8266::sATCIPSENDSingle(const uint8_t *buffer, uint32_t len)
{
    rx_empty();
    m_puart->print("AT+CIPSEND=");
    m_puart->println(len);
    if (recvFind(">", 5000)) {
        rx_empty();
        for (uint32_t i = 0; i < len; i++) {
            m_puart->write(buffer[i]);
        }
        return recvFind("SEND OK", 10000);
    }
    return false;
}
bool ESP8266::sATCIPSENDMultiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
    rx_empty();
    m_puart->print("AT+CIPSEND=");
    m_puart->print(mux_id);
    m_puart->print(",");
    m_puart->println(len);
    if (recvFind(">", 5000)) {
        rx_empty();
        for (uint32_t i = 0; i < len; i++) {
            m_puart->write(buffer[i]);
        }
        return recvFind("SEND OK", 10000);
    }
    return false;
}
bool ESP8266::sATCIPCLOSEMulitple(uint8_t mux_id)
{
    String data;
    rx_empty();
    m_puart->print("AT+CIPCLOSE=");
    m_puart->println(mux_id);
    
    data = recvString("OK", "link is not", 5000);
    if (data.indexOf("OK") != -1 || data.indexOf("link is not") != -1) {
        return true;
    }
    return false;
}
bool ESP8266::eATCIPCLOSESingle(void)
{
    rx_empty();
    m_puart->println("AT+CIPCLOSE");
    return recvFind("OK", 5000);
}
bool ESP8266::eATCIFSR(String &list)
{
    rx_empty();
    m_puart->println("AT+CIFSR");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list);
}
bool ESP8266::sATCIPMUX(uint8_t mode)
{
    String data;
    rx_empty();
    m_puart->print("AT+CIPMUX=");
    m_puart->println(mode);
    
    data = recvString("OK", "Link is builded");
    if (data.indexOf("OK") != -1) {
        return true;
    }
    return false;
}
bool ESP8266::sATCIPSERVER(uint8_t mode, uint32_t port)
{
    String data;
    if (mode) {
        rx_empty();
        m_puart->print("AT+CIPSERVER=1,");
        m_puart->println(port);
        
        data = recvString("OK", "no change");
        if (data.indexOf("OK") != -1 || data.indexOf("no change") != -1) {
            return true;
        }
        return false;
    } else {
        rx_empty();
        m_puart->println("AT+CIPSERVER=0");
        return recvFind("\r\r\n");
    }
}
bool ESP8266::sATCIPSTO(uint32_t timeout)
{
    rx_empty();
    m_puart->print("AT+CIPSTO=");
    m_puart->println(timeout);
    return recvFind("OK");
}

