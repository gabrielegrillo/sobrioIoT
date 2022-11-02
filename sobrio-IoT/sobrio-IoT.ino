#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "sobrio-costanti.h"  
#include "nesea-wan-config.h"  
#include <dht11.h>
#define DHT11PIN 4

dht11 sensoreTemperatura;

/*
 * Wemos D1 ESP8266 based board
 * 
 * http://www.esp8266learning.com/wemos-d1-esp8266-based-board.php
 * 
Pin     Function      ESP-8266 Pin
D0      RX            GPIO3
D1      TX            GPIO1
D2      IO            GPIO16
D3(D15) IO,SCL        GPIO5
D4(D14) IO,SDA        GPIO4
D5(D13) IO,SCK        GPIO14
D6(D12) IO,MISO       GPIO12
D7(D11) IO,MOSI       GPIO13
D8      IO,Pull-up    GPIO0
D9      IO,pull-up, BUILTIN_LED   GPIO2
D10     IO,pull-down,SS           GPIO15
A0      Analog Input              A0
 */



WiFiClient arduinoClient;
String stato = "";
int tempo;
String payload;
HTTPClient sobrioSoapClient;
int httpResponseCode;



void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  uint8_t newMACAddress[] = {0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66};
  wifi_set_macaddr(STATION_IF, &newMACAddress[0]);
  WiFi.begin(SISID, pwd);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connessa");
  pinMode(pinT, INPUT);
  pinMode(pinU, INPUT);
  pinMode(pinP, INPUT);
  pinMode(pinH, INPUT);
  pinMode(pinDV, INPUT);
  pinMode(pinTimer, INPUT);

  // Ricava la configurazione

  /*
  String bustaSoap = makeBustaSoap("GetConfCentralina", "identificatoreCentralina", IdentificativoCentralina);
  sobrioSoapClient.addHeader("Accept-Encoding","gzip,deflate");
  sobrioSoapClient.addHeader("Content-Type", "text/xml;charset=UTF-8");
  sobrioSoapClient.addHeader("SOAPAction","GetConfCentralina");
  sobrioSoapClient.addHeader("Content-Length", String(bustaSoap.length()));
  sobrioSoapClient.addHeader("Host",soapServer + ":" + portaServer);
  httpResponseCode = sobrioSoapClient.POST(bustaSoap);
  payload = sobrioSoapClient.getString();
  sobrioSoapClient.end();

  String startTag = "<tns:GetConfCentralinaResult>";
  String endTag = "</tns:GetConfCentralinaResult>";
  int st = payload.indexOf(payload,startTag)+startTag.lenght();
  int fn = payload.indexOf(payload,endTag);
  String configurazioneCentralina_json = payload.substring(ts,fn);

  String configurazioneCentralina_json = "{\"idCentralina\":\"1\",\"Localita\":\"Cosenza\",\"Temperatura\":\"true\",\"Umidita\":\"true\",\"Precipitazioni\":\"false\",\"Direzione_Vento\":\"False\"}";
*/
}

void loop() {
    payload = "";
    Serial.println("Rileva i parametri fisici");
    int ck = sensoreTemperatura.read(pinT);
    if (pinT != 0)
      {
        T = (float)sensoreTemperatura.temperature;
      }
    if (pinU != 0)
      {
        // Rileva Unidita
        U = (float)sensoreTemperatura.humidity;
      }

    //String datiSoap = "{\"Temperatura\":" + String(T) + ", \"Umidita\":" + String(U) + ", \"Pressione\":" + String(H) + ", \"Precipitazioni:" + String(P) + ", \"Direzione_Vento:" + String(DV) + "}";
    String datiSoap = "{\"Temperatura\":";
    datiSoap += String(T,0);
    datiSoap += ", \"Umidita\":";
    datiSoap += String(U,0);
    datiSoap += ", \"Pressione\":";
    datiSoap += String(H,0);
    datiSoap += ", \"Precipitazioni\":";
    datiSoap += String(P,0);
    datiSoap += ", \"Direzione_Vento\":\"";
    datiSoap += DV;
    datiSoap += "\",\"identificatoreCentralina\":\"";
    datiSoap += IdentificativoCentralina;
    datiSoap += "\"}";
    
    String bustaSoap = makeBustaSoap("MemorizzaRilevazione", "stringaJson", datiSoap);
    //String bustaSoap = makeBustaSoap("GetConfCentralina", "identificatoreCentralina", IdentificativoCentralina);

    Serial.println(datiSoap);
    Serial.println("-------------------------------");
    Serial.println(bustaSoap);
    Serial.println("-------------------------------");
    
    if (sobrioSoapClient.begin(soapServer.c_str(),portaServer)!=NULL)
    {
      sobrioSoapClient.addHeader("Accept-Encoding","gzip,deflate");
      sobrioSoapClient.addHeader("Content-Type", "text/xml;charset=UTF-8");
      //sobrioSoapClient.addHeader("SOAPAction","GetConfContralina");
      sobrioSoapClient.addHeader("SOAPAction","MemorizzaRilevazione");
      sobrioSoapClient.addHeader("Content-Length", String(bustaSoap.length()));
      sobrioSoapClient.addHeader("Host",soapServer + ":" + portaServer);
      httpResponseCode = sobrioSoapClient.POST(bustaSoap);
      Serial.println(httpResponseCode);
      Serial.println(sobrioSoapClient.errorToString(httpResponseCode).c_str());
      payload = sobrioSoapClient.getString();
      sobrioSoapClient.end();
    }
    else
      Serial.println("Errore di conessione al ws");
    Serial.println(payload);
    delay(intervallo);
}

String makeBustaSoap(String Metodo, String Parametro, String Valore) {
  String ParteMetodoIniziale = "<mon:" + Metodo + ">";
  String ParteParametroValore = "<mon:" + Parametro + ">" + Valore + "</mon:" + Parametro + ">";
  String ParteMetodoFinale = "</mon:" + Metodo + ">";
  String Busta = bustaDiRichiesta_ParteIniziale + ParteMetodoIniziale + ParteParametroValore + ParteMetodoFinale + bustaDiRichiesta_ParteFinale;
  return Busta;
}
/*
 * 
Pin   Code        Arduino alias       Pin   Code        Arduino alias
A0    A0          A0                  D6    GPIO 12     12
D0    GPIO 16     16                  D7    GPIO 13     13
D1    GPIO 5      5                   D8    GPIO 15     15
D2    GPIO 4      4                   SD2   GPIO 9      9
D3    GPIO 0      0                   SD3   GPIO 10     10
D4    GPIO 2      2                   RX    GPIO 3      3
D5    GPIO 14     14                  TX    GPIO 1      1
 */

 
