//************************************************************
// this is a simple example that uses the painlessMesh library to
// connect to a another network and broadcast message from a webpage to the edges of the mesh network.
// This sketch can be extended further using all the abilities of the AsyncWebserver library (WS, events, ...)
// for more details
// https://gitlab.com/painlessMesh/painlessMesh/wikis/bridge-between-mesh-and-another-network
// for more details about my version
// https://gitlab.com/Assassynv__V/painlessMesh
// and for more details about the AsyncWebserver library
// https://github.com/me-no-dev/ESPAsyncWebServer
//************************************************************

#include "IPAddress.h"
#include "painlessMesh.h"
#ifdef ESP8266
#include "Hash.h"
#include "ESPAsyncTCP.h"
#else
#include <AsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   STATION_SSID     "mySSID"
#define   STATION_PASSWORD "myPASSWORD"
#define HOSTNAME "HTTP_BRIDGE"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
const char *ssid="rajeshgupta",*ssid2="Ayush",*ssid3="VITC-HOS2-4";
const char *password="Ayush",*pass2="ayush2004",*pass3="2021043019";

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 33);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char *PARAM_INPUT_1 = "h";
const char *PARAM_INPUT_2 = "s";
const char *PARAM_INPUT_3 = "l";
String msg ="Hi from node1";

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Prototype1
// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

IPAddress getlocalIP();

const char index_html[] PROGMEM = R"rawliteral(
{

<style>
    :root {
        --link-color-h: 30;
        --link-color-s: 80%;
        --link-color-l: 50%;
        --scale: 1;

        --link-color: hsl(var(--link-color-h), var(--link-color-s), var(--link-color-l));

        --link-color-10: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) / .1));
        --link-color-20: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) / .2));
        --link-color-30: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) / .3));
        --link-color-40: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) / .4));
        --link-color-50: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) / .5));
        --link-color-60: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) / .6));
        --link-color-70: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) / .7));
        --link-color-80: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) / .8));
        --link-color-90: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) / .9));

        --link-color-d-10: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) * .1));
        --link-color-d-20: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) * .2));
        --link-color-d-30: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) * .3));
        --link-color-d-40: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) * .4));
        --link-color-d-50: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) * .5));
        --link-color-d-60: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) * .6));
        --link-color-d-70: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) * .7));
        --link-color-d-80: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) * .8));
        --link-color-d-90: hsl(var(--link-color-h), var(--link-color-s), calc(var(--link-color-l) * .9));
    }

    html,
    body {
        margin: 0;
        font-family: system-ui;
    }

    .title-bar {
        height: calc(50px * var(--scale));
        color: var(--link-color-70);
        background: linear-gradient(90deg, var(--link-color-d-80), var(--link-color-d-90));
        border-bottom: 1px var(--link-color-d-60) solid;
        display: flex;
        align-items: center;
        justify-content: flex-start;
        padding: 0px calc(18px * var(--scale));
        font-size: calc(18px * var(--scale));
    }

    .content {
        color: var(--link-color-d-50);
        background-color: var(--link-color);
        display: grid;
        justify-content: center;
        justify-items: center;
        height: calc(100vh - calc(51px * var(--scale)));
        font-size: calc(16px * var(--scale));
    }

    .content h1 {
        color: var(--link-color-d-20);
        font-size: calc(32px * var(--scale));
    }

    .panel {
        width: calc(244px * var(--scale));
        height: max-content;
        padding: calc(8px * var(--scale));
        margin: calc(8px * var(--scale));
        background: var(--link-color-d-90);
        border: 1px var(--link-color-d-80) solid;
        border-radius: calc(4px * var(--scale));
    }

    .row {
        color: var(--link-color-d-40);
        padding: 0 calc(2px * var(--scale));
        display: flex;
        align-items: baseline;
        justify-content: space-between;
    }

    input {
        width: calc(220px * var(--scale));
    }

    #scale {
        width: calc(190px * var(--scale));
        
    }
</style>
<header>
    <div class="title-bar">SWADDLE</div>
</header>
<div class="content">
    <form action="/get">
        <div class="panel">

            <div class="row">
                h:
                <input type="range" name="h" min="0" max="360" step="1" value="30" id="color">
            </div>
            <div class="row">
                s:
                <input type="range" name="s" min="0" max="100" step="1" value="80" id="saturation">
            </div>
            <div class="row">
                l:
                <input type="range" name="l" min="0" max="100" step="1" value="50" id="lightness">
            </div>
            <br>
            <input style="padding: 5px;border-radius:20px;align-self: center;align-items: center;margin:auto" type="submit" value="Apply">
        </div>
        
    </form>
</div>
<script>
    //properties
    var h = 30;
    var s = 80;
    var l = 50;
    var scale = 1;

    document.body.addEventListener("input", function (event) { UpdateLayout(event.target) });

    function UpdateLayout(elem) {
        switch (elem.id) {
            case "color":
                h = elem.value;
                break;
            case "saturation":
                s = elem.value;
                break;
            case "lightness":
                l = elem.value;
                break;
            case "scale":
                scale = elem.value;
                break;
        }

        document.documentElement.style.cssText = "--link-color-h: " + h + "; --link-color-s: " + s + "%" + "; --link-color-l: " + l + "%" + "; --scale: " + scale;
    }
</script>})rawliteral";


IPAddress myIP(0,0,0,0);
IPAddress myAPIP(0,0,0,0);

void setup() {
    Serial.begin(115200);
    pinMode(D5,OUTPUT);
    pinMode(A0,INPUT);
    pinMode(D6,OUTPUT);
    pinMode(D7,OUTPUT);
    analogWrite(D5,0);
    analogWrite(D6,0);
    analogWrite(D7,0);
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

    // Channel set to 6. Make sure to use the same channel for your mesh and for you other
    // network (STATION_SSID)
    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  mesh.stationManual(STATION_SSID, STATION_PASSWORD);
  mesh.setHostname(HOSTNAME);
//    Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
    mesh.setRoot(true);
//    This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
    mesh.setContainsRoot(true);

    myAPIP = IPAddress(mesh.getAPIP());
    Serial.println("My AP IP is " + myAPIP.toString());

    //Async webserver
    //Send web page with input fields to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send_P(200, "text/html", index_html); });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      String inputMessage1;
      String inputMessage2;
      String inputMessage3;
      double h;
      double s;
      double v;
      // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
      if (request->hasParam(PARAM_INPUT_1)) {
        inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      }
      // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
      if (request->hasParam(PARAM_INPUT_2)) {
        inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      }
      // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
      if (request->hasParam(PARAM_INPUT_3)) {
        inputMessage3 = request->getParam(PARAM_INPUT_3)->value();
      }
//      else {
//        inputMessage1=inputMessage2=inputMessage3= "No message sent";
//      }
      h= inputMessage1.toDouble()/360;
      s= inputMessage2.toDouble()/100;
      v= inputMessage3.toDouble()/100;
      double r, g, b;
      int i = int(h * 6);
      double f = h * 6 - i;
      double p = v * (1 - s);
      double q = v * (1 - f * s);
      double t = v * (1 - (1 - f) * s);

      switch(i % 6){
         case 0: r = v, g = t, b = p; break;
          case 1: r = q, g = v, b = p; break;
          case 2: r = p, g = v, b = t; break;
          case 3: r = p, g = q, b = v; break;
          case 4: r = t, g = p, b = v; break;
          case 5: r = v, g = p, b = q; break;
          msg = String(h,4)+","+String(s,4)+","+String(v,4);
      }

      r = r * 255;
      g = g * 255;
      b = b * 255;
      analogWrite(D5,255-r);
      analogWrite(D6,255-g);
      analogWrite(D7,255-b);
      
      
      
      
      digitalWrite(D5,h);
      digitalWrite(D6,s);
      digitalWrite(D7,v);
      
      
      
      
      Serial.print("r:");
      Serial.println(r);
      Serial.print("g:");
      Serial.println(g);
      Serial.print("b:");
      Serial.println(b);
      request->send(200, "text/html", index_html);
    });
  
    server.begin();
    userScheduler.addTask( taskSendMessage );
    taskSendMessage.enable();

}

void loop() {
  mesh.update();
}
