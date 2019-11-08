#include <SPI.h>
#include "mcp_can.h"
#include <SimpleModbusSlave.h>



const int spiCSPin = 10;
int arduino_freio = 0x43;
int arduino_pisca = 0x55;

unsigned int holdingRegs[3]; 



MCP_CAN CAN(spiCSPin);

void setup()
{
    modbus_configure(&Serial, 9600, SERIAL_8N2, 1, 2, 3, holdingRegs);
    modbus_update_comms(9600, SERIAL_8N2, 1);

    Serial.begin(9600);
   

    //while (CAN_OK != CAN.begin(CAN_500KBPS))
    //{
        //Serial.println("CAN BUS Init Failed");
        //delay(100);
    //}
    //Serial.println("CAN BUS  Init OK!");

    //Serial.println(arduino_freio);


   
}


void loop()
{

   modbus_update();
  
     
    unsigned char len = 0;
    unsigned char buf;

    if(CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&len, buf);

        unsigned long canId = CAN.getCanId();

        if (canId == arduino_freio)

        {
           holdingRegs[1] = buf;
        }

        else if (canId == arduino_pisca)
        {
           holdingRegs[1] = buf;
        }
        
     }
}     
