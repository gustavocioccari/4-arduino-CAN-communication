#include <SPI.h>
#include <mcp_can.h>

const int spiCSPin = 10;//Declarando pino SPI
int analogpin = 5;//Declarando pino de leitura do potenciometro

int valor_atual_pot=0;// Declarando variável de leitura do potenciometro

byte dado_enviar=0;// Declarando variável de envio do valor do potenciometro

MCP_CAN CAN(spiCSPin);

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS init Failed");
        delay(100);
    }
    Serial.println("CAN BUS Shield Init OK!");   
}

unsigned char stmp = 0;
    
void loop()
{ 
  valor_atual_pot = analogRead(analogpin);//Leitura do valor do potenciometro
  
  if(valor_atual_pot>10)//Caso tenha pelo menos um valor de 10
    {
      dado_enviar =  valor_atual_pot/2; //Como o valor do potenciômetro nesse caso varia apenas de 0 a 512 em função do esquema elétrico, basta dividirmos a tensão por 2;
      stmp = dado_enviar;
      CAN.sendMsgBuf(0x43, 0, 8, stmp);  
    }
}
