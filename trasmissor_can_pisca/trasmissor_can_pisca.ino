#include <SPI.h>
#include <mcp_can.h>

const int spiCSPin = 10; // Declarando pino do SPI

int botao_pin = 5; // Declarando pino do botao

boolean estado_pisca_ligado = 0; //Declarando os estados
boolean estado_pisca_ligado_apos_1s = 0;//Declarando os estados
boolean estado_pisca_desligado = 0;//Declarando os estados
boolean estado_pisca_desligado_apos_1s = 0;//Declarando os estados

boolean botao = 0;//Declarando a variável botão

unsigned long time_old =0;//Declarando a variável para contagem do tempo

unsigned char stmp = 0;//Declarando a variável de envio da mensagem

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

    estado_pisca_desligado_apos_1s=1;//Estado que começa ativado

}


    
void loop()
{ 
  Serial.println("In loop");  
  
  botao = digitalRead(botao_pin);// Leitura do botão
   
           //CONFIGURANDO OS ESTADOS
  
  if(estado_pisca_desligado_apos_1s==1 and botao==0)
    {
      estado_pisca_desligado_apos_1s=0;
      estado_pisca_ligado=1;
      time_old = millis();//Zerando a contagem de tempo
   
    }
  else if (estado_pisca_ligado==1 and millis()-time_old >= 1000)
    {
      estado_pisca_ligado=0;
      estado_pisca_ligado_apos_1s=1;    
    }  
   else if(estado_pisca_ligado_apos_1s==1 and botao==0)
    {
      estado_pisca_ligado_apos_1s=0;
      estado_pisca_desligado=1; 
      time_old = millis();  
    }
    else if(estado_pisca_desligado==1 and millis()-time_old >= 1000)
    {
      estado_pisca_desligado=0;
      estado_pisca_desligado_apos_1s=1;
    }
   
              //CONFIGURANDO AS SAÍDAS
                      
    if(estado_pisca_desligado==1)
    {
      if (stmp==1)//Se a mensagem anterior foi 1
        {
           stmp = 0;
           CAN.sendMsgBuf(0x55, 0, 8, stmp);
        }
       else
       {
        
       }     
    }
  else if (estado_pisca_ligado==1)
    {
       if (stmp==0)//Se a mensagem anterior foi 0
        {
           stmp = 1;
           CAN.sendMsgBuf(0x55, 0, 8, stmp);
        }
       else
       {
        
       } 
    }  
  
}
