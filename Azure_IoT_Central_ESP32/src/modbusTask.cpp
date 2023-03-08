#include "weidosTasks.h"
#include "telemetryGlobalVariables.h"

#include <Arduino.h>
#include <Ethernet.h>
#include <ArduinoModbus.h>
#include <time.h>

#define ETHERNET_TIMEOUT            60000
#define ETHERNET_RESPONSE_TIMEOUT   4000

#define MODBUS_BEGIN_TRIES      3
#define MODBUS_REQUEST_TRIES    3

#define MODBUS_ADDRESS      1
#define MODBUS_TIMEOUT      5000
#define REG_ADDRESS         19000
#define NUM_REGISTERS       122
#define NUM_DATA            NUM_REGISTERS/2

#define REG_ADDRESS2         828
#define NUM_REGISTERS2       20
#define NUM_DATA2            NUM_REGISTERS2/2

#define REG_ADDRESS3         10085
#define NUM_REGISTERS3       2
#define NUM_DATA3            NUM_REGISTERS3/2


EthernetClient ethClient;
ModbusTCPClient modbusTCPClient(ethClient);


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xFF };   //MAC address


IPAddress serverIP(255, 255, 255, 255);        //Generic IP. To be changed

void weidosSetup(){
  Serial.begin(115200);

  delay(5000);
  Serial.println("Welcome");
  Ethernet.init(ETHERNET_CS);
  Ethernet.begin(mac, ETHERNET_TIMEOUT, ETHERNET_RESPONSE_TIMEOUT);

  Serial.println(Ethernet.localIP());

  modbusTCPClient.setTimeout(MODBUS_TIMEOUT);  
  if (!modbusTCPClient.begin(serverIP)) {
    Serial.println("Failed to start Modbus TCP Client!");
    while (1);
  }
  
  
  Serial.println("End of set up");    

};


void getData(){
    modbusTCPClient.begin(serverIP);
    
    for(int i=0; i<MODBUS_BEGIN_TRIES; i++)
    {
        if(!modbusTCPClient.connected())
        {
            modbusTCPClient.begin(serverIP);
        }else break;
    }

    for(int i=0; i<MODBUS_REQUEST_TRIES; i++)
    {
        int response = modbusTCPClient.requestFrom(MODBUS_ADDRESS, INPUT_REGISTERS, REG_ADDRESS, NUM_REGISTERS);    
        if(!response)
        {
            Serial.println("No response");
            Serial.print("Last error: ");
            Serial.println(modbusTCPClient.lastError());
            modbusTCPClient.begin(serverIP);
            comStatus = 0;
            continue;
        }
        assignDataToGlobalVariables();
        comStatus = 1;
        timestamp = time(NULL);
        break;
    }

    for(int i=0; i<MODBUS_REQUEST_TRIES; i++)
    {
        int response = modbusTCPClient.requestFrom(MODBUS_ADDRESS, INPUT_REGISTERS, REG_ADDRESS2, NUM_REGISTERS2);    
        if(!response)
        {
            Serial.println("No response for batch 2");
            Serial.print("Last error: ");
            Serial.println(modbusTCPClient.lastError());
            modbusTCPClient.begin(serverIP);
            comStatus = 0;
            continue;
        }
        assignDataToGlobalVariables2();
        comStatus = 1;
        break;
    }

    for(int i=0; i<MODBUS_REQUEST_TRIES; i++)
    {
        int response = modbusTCPClient.requestFrom(MODBUS_ADDRESS, INPUT_REGISTERS, REG_ADDRESS3, NUM_REGISTERS3);    
        if(!response)
        {
            Serial.println("No response for batch 3");
            Serial.print("Last error: ");
            Serial.println(modbusTCPClient.lastError());
            modbusTCPClient.begin(serverIP);
            comStatus = 0;
            continue;
        }
        assignDataToGlobalVariables3();
        comStatus = 1;
        break;
    }

    time_t now = time(NULL);

    return;
}





void assignDataToGlobalVariables(){
    voltageL1N = getNextData();
    voltageL2N = getNextData();
    voltageL3N = getNextData();
    voltageL1L2 = getNextData();
    voltageL2L3 = getNextData();
    voltageL1L3 = getNextData(); 
    currentL1 = getNextData();
    currentL2 = getNextData();
    currentL3 = getNextData();
    currentTotal = getNextData();
    realPowerL1N = getNextData();
    realPowerL2N = getNextData();
    realPowerL3N = getNextData();
    realPowerTotal = getNextData();
    apparentPowerL1N = getNextData();
    apparentPowerL2N = getNextData();
    apparentPowerL3N = getNextData();
    apparentPowerTotal = getNextData();
    reactivePowerL1N = getNextData();
    reactivePowerL2N = getNextData();
    reactivePowerL3N = getNextData();
    reactivePowerTotal = getNextData();
    cosPhiL1 = getNextData();
    cosPhiL2 = getNextData();
    cosPhiL3 = getNextData();
    frequency = getNextData();
    rotField = getNextData();
    realEnergyL1N = getNextData()/1000.0f;
    realEnergyL2N = getNextData()/1000.0f;
    realEnergyL3N = getNextData()/1000.0f;
    realEnergyTotal = getNextData()/1000.0f;
    getNextData();       //realEnergyConsL1 deleted variable
    getNextData();       //realEnergyConsL2 deleted variable
    getNextData();       //realEnergyConsL3 deleted variable
    getNextData();      //realEnergyConsTotal deleted variable
    getNextData();      //realEnergyDelivL1 deleted variable     
    getNextData();      //realEnergyDelivL2 deleted variable
    getNextData();      //realEnergyDelivL3 deleted variable
    getNextData();      //realEnergyDelivTotal deleted variable
    apparentEnergyL1 = getNextData()/1000.0f;
    apparentEnergyL2 = getNextData()/1000.0f;
    apparentEnergyL3 = getNextData()/1000.0f;
    apparentEnergyTotal = getNextData()/1000.0f;
    reactiveEnergyL1 = getNextData()/1000.0f;
    reactiveEnergyL2 = getNextData()/1000.0f;
    reactiveEnergyL3 = getNextData()/1000.0f;
    reactiveEnergyTotal = getNextData()/1000.0f;
    getNextData();      //reactiveEnergyIndL1 deleted variable
    getNextData();      //reactiveEnergyIndL2 deleted variable   
    getNextData();      //reactiveEnergyIndL3 deleted variable    
    getNextData();      //reactiveEnergyIndTotal deleted variable
    getNextData();      //reactiveEnergyCapL1 deleted variable
    getNextData();      //reactiveEnergyCapL2 deleted variable
    getNextData();      //reactiveEnergyCapL3 deleted variable
    getNextData();      //reactiveEnergyCapTotal deleted variable
    THDVoltsL1N = getNextData();
    THDVoltsL2N = getNextData();
    THDVoltsL3N = getNextData();
    THDCurrentL1N = getNextData();
    THDCurrentL2N = getNextData();
    THDCurrentL3N = getNextData();
}

void assignDataToGlobalVariables2(){
    powerFactorL1N = getNextData();
    powerFactorL2N = getNextData();
    powerFactorL3N = getNextData();
    powerFactorTotal = getNextData();
    THDVoltsL1L2 = getNextData();
    THDVoltsL2L3 = getNextData();
    THDVoltsL1L3 = getNextData();
}

void assignDataToGlobalVariables3(){
    currentNeutral = getNextData();
}

void computeData(){
    avgVoltageLN = (voltageL1N + voltageL2N + voltageL3N)/3.0f;
    avgVoltageLL = (voltageL1L2 + voltageL2L3 + voltageL1L3)/3.0f;
    avgCurrentL = (currentL1 + currentL2 + currentL3)/3.0f;
    avgCosPhi = realPowerTotal/apparentPowerTotal;
    avgTHDVoltsLN = (THDVoltsL1N + THDVoltsL2N + THDVoltsL3N)/3.0f;
    avgTHDCurrentLN = (THDCurrentL1N + THDCurrentL2N + THDCurrentL3N)/3.0f;
    avgTHDVoltsLL = (THDVoltsL1L2 + THDVoltsL2L3 + THDVoltsL1L3)/3.0f;
}



float getNextData(){
    long msb = modbusTCPClient.read();
    long lsb =  modbusTCPClient.read();
    uint32_t rawData = (msb << 16) + lsb; // Bit Shift operation to join both registers
    float data = *(float *)&rawData; 
    return data;
};
