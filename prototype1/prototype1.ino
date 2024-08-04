#include "painlessMesh.h"
#include <ArduinoJson.h>
#define MESH_PREFIX "SWADDLE"
#define MESH_PASSWORD "SWADDLE#123"
#define MESH_PORT 5555
#define red D2
#define green D5
#define blue D6
StaticJsonDocument<300> json_dict;
Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

// User stub
void sendMessage(); // Prototype so PlatformIO doesn't complain
int mode_no = 0;
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);
float rgb[3] = {0, 0, 0};

void aura()
{
    if (mode_no == 0)
    {
    }
    else if (mode_no == 1)
    {
        delay(500);
        analogWrite(red, 255 - rgb[0]);
        analogWrite(green, 255 - rgb[1]);
        analogWrite(blue, 255 - rgb[2]);
        delay(500);
        analogWrite(red, 255);
        analogWrite(green, 255);
        analogWrite(blue, 255);
    }
    else if (mode_no == 2)
    {
        for (int i = 0; i <= 256; i += 1.5)
        {
            if (i <= rgb[0] + 1)
                analogWrite(red, i);
            if (i <= rgb[1] + 1)
                analogWrite(green, i);
            if (i <= rgb[2] + 1)
                analogWrite(blue, i);
            delay(10);
        }
        int temp[3] = {rgb[0], rgb[1], rgb[2]};
        for (int i = 0; i < 256; i += 1.5)
        {
            analogWrite(red, temp[0]);
            analogWrite(green, temp[1]);
            analogWrite(blue, temp[2]);
            temp[0]--;
            temp[1]--;
            temp[2]--;
            delay(10);
        }
    }
    else if (mode_no == 3)
    {
        for (int i = 0; i <= rgb[0]; i += 1.8)
        {
            analogWrite(red, i);
            delay(10);
        }
        for (int i = 0; i <= rgb[1]; i += 1.8)
        {
            analogWrite(green, i);
            delay(10);
        }
        for (int i = 0; i <= rgb[2]; i += 1.8)
        {
            analogWrite(blue, i);
            delay(10);
        }
    }
}

void sendMessage()
{
    String msg = "{\"r\":" + String(rgb[0]) + ",\"g\":" + String(rgb[1]) + ",\"b\":" + String(rgb[2]) + ",\"brodcast\":" + String(0) + ",mode:" + String(mode_no) + "}";
    mesh.sendBroadcast(msg);
    Serial.println("Working as normal node");
    Serial.println(msg);
    aura();
    taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5));
}

void receivedCallback(uint32_t from, String &msg)
{
    DeserializationError error = deserializeJson(json_dict, msg);
    if (error)
    {
        Serial.println("parseObject() failed");
        return;
    }
    bool brodcast = json_dict["brodcast"];
    if ((brodcast == 1) && (rgb[0] != json_dict["r"] || mode_no != json_dict["mode"] || rgb[1] != json_dict["g"] || rgb[2] != json_dict["b"]))
    {
        rgb[0] = json_dict["r"];
        rgb[1] = json_dict["g"];
        rgb[2] = json_dict["b"];
        mode_no = json_dict["mode"];
        analogWrite(red, 255 - rgb[0]);
        analogWrite(green, 255 - rgb[1]);
        analogWrite(blue, 255 - rgb[2]);
        String msg = "{\"r\":" + String(rgb[0]) + ",\"g\":" + String(rgb[1]) + ",\"b\":" + String(rgb[2]) + ",\"brodcast\":" + String(brodcast) + "}";
        mesh.sendBroadcast(msg);
        brodcast = 0;
    }

    aura();
    //   else if ((brodcast==2) && ( rgb[0]!=json_dict["r"] ||  rgb[1]!=json_dict["g"] || rgb[2]!=json_dict["b"])){
    //    }
    Serial.printf("\n\nReceived from [ nodeId = %u ]\n\t\tmsg : %s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId)
{
    aura();
    Serial.printf("\n\nNew Connection,[ nodeId = %u ]\n", nodeId);
}

void changedConnectionCallback()
{
    Serial.printf("\n\nChanged connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset)
{
    aura();
    Serial.printf("\n\nAdjusted time [ nodeId = %u ][Offset = %d]\n\n", mesh.getNodeTime(), offset);
}

void setup()
{
    Serial.begin(115200);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);
    analogWrite(red, 0);
    analogWrite(green, 0);
    analogWrite(blue, 0);
    // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes(ERROR | STARTUP | DEBUG); // set before init() so that you can see startup messages

    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    mesh.initOTAReceive("update_mesh");
    userScheduler.addTask(taskSendMessage);
    taskSendMessage.enable();
}

void loop()
{
    // it will run the user scheduler as well
    mesh.update();
}
