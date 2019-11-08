#include <SPI.h>
#include "mcp_can.h"

const int spiCSPin = 10;//Definindo pino SPI

int arduino_freio = 0x43;//Declarando endereço do freio
int arduino_pisca = 0x55;//Declarando endereço do pisca

int led_freio = 3;//Declarando LED do freio
int led_pisca = 4;//Declarando LED do pisca

boolean estado_desligado = 0;//Declarando estado desligado
boolean estado_ligado_ON = 0;//Declarando estado ligado com o LED aceso
boolean estado_ligado_OFF = 0;//Declarando estado ligado com o LED apagado

unsigned long time_old = 0;//Declarando variável de tempo

MCP_CAN CAN(spiCSPin);

void setup()
{
    Serial.begin(115200);
   

    while (CAN_OK != CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS Init Failed");
        delay(100);
    }
    Serial.println("CAN BUS  Init OK!");

    Serial.println(arduino_freio);

    estado_desligado = 1;//Setando estado de desligado
    digitalWrite(led_pisca,LOW);//LED começa apagado
    
    pinMode(led_freio, OUTPUT);//Delarando como pinos de saída
    pinMode(led_pisca, OUTPUT);//Delarando como pinos de saída

}


void loop()
{
    unsigned char len = 0;
    unsigned char buf[8];

    if(CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&len, buf);

        unsigned long canId = CAN.getCanId();

        if (canId == arduino_freio)//Se a mensagem recebida é do freio

        {
          Serial.print(buf[0]);
          analogWrite(led_freio,buf[0]);//Escreve no LED o PWM
        }

        else if (canId == arduino_pisca)//Se a mensagem recebida é do pisca executa a máquina de estados
        {
             
                  //CONFIGURANDO ESTADOS
          
          if (estado_desligado==1 and buf[0]==1 )
          {
            estado_desligado=0;
            estado_ligado_ON=1;

            time_old = millis();
          }
          else if (estado_ligado_ON==1 and millis()-time_old>1000)
          {
            estado_ligado_ON=0;
            estado_ligado_OFF=1;

            time_old = millis();
          }
           else if (estado_ligado_ON==1 and buf[1]==0)
          {
            estado_ligado_ON=0;
            estado_desligado=1;

          }

           else if (estado_ligado_OFF==1 and millis()-time_old>1000)
          {
            estado_ligado_OFF=0;
            estado_ligado_ON=1;

            time_old = millis();
          }
           else if (estado_ligado_OFF==1 and buf[1]==0)
          {
            estado_ligado_OFF=0;
            estado_desligado=1;

          }

                  //CONFIGURANDO SAÍDAS
          
          if (estado_desligado==1)
            {
              digitalWrite(led_pisca,LOW);
            }
          else if (estado_ligado_ON==1)
            {
              digitalWrite(led_pisca,HIGH);
            }
          else if (estado_ligado_OFF==1)
            {
              digitalWrite(led_pisca,LOW);

            }
          
          
          
        }

       

        
    }
}
