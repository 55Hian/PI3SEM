
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "max6675.h" //bibliotéca sensor de temperatura
#include <Adafruit_NeoPixel.h>

#define numLeds   8    // Quantidade de leds 
#define pinNumber 12    // Pino do arduino 

#define BRIGHTNESS 210

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numLeds, pinNumber, NEO_GRB + NEO_KHZ800);

WiFiClientSecure client; //Cria um cliente seguro (para ter acesso ao HTTPS)

String textFix = "GET /forms/d/e/1FAIpQLSdg3z6ex3eqmtq50QU3exj6IZ96m6zCcOUUZiX2aO1Va6UClw/formResponse?ifq&entry.353115888="; //link formulario ( envio de dados para planilha )
                                                                                                                  

int ktcSO = 19; //PINO DIGITAL (SO)
int ktcCS = 23; //PINO DIGITAL (CS)
int ktcCLK = 5; //PINO DIGITAL (CLK / SCK)

MAX6675 ktc(ktcCLK, ktcCS, ktcSO); //CRIA UMA INSTÂNCIA UTILIZANDO OS PINOS (CLK, CS, SO)

boolean sled;

void setup() {

  pinMode(12, OUTPUT);
  pixels.begin();

  Serial.begin(115200);//Inicia a comunicacao serial

  WiFi.mode(WIFI_STA);//Habilita o modo estaçao
  WiFi.begin("iPhone de Hian", "1234hian");//Conecta na rede

  delay(2000);//Espera um tempo para se conectar no WiFi

  client.setInsecure(); // seta a conexão do cliente como segura.

  for (int x = 0; x <= 1; x++) {
    for (int x = 0; x <= 7; x++) {
      pixels.setPixelColor(x, pixels.Color(0, 255, 0)); // Pixel número "0" - Cores (Red = 0, Green = 255, Blue = 0)
      pixels.show();
      delay(100);
      pixels.setPixelColor(x, pixels.Color(0, 0, 0)); // Pixel número "0" - Cores (Red = 0, Green = 0, Blue = 0)
      pixels.show();
      delay(100);
    }

    delay(300);
  }
}

void loop() {
  if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
  {
    String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
    toSend +=   ktc.readCelsius();
    toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.

    client.println(toSend);//Enviamos o GET ao servidor-
    client.println("Host: docs.google.com");//-
    client.println();//-
    client.stop();//Encerramos a conexao com o servidor
    Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
  }
  else
  {
    Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
  }


  //sinais temperatura

  

  if ( ktc.readCelsius() >= 68.2 ) {


      for (int x = 0; x <= 7; x++) {
        pixels.setPixelColor(x, pixels.Color(255, 0, 0)); // Pixel número "0" - Cores (Red = 0, Green = 255, Blue = 0)
        pixels.show();
      }
      delay(250);
      
      for (int x = 0; x <= 7; x++) {
        pixels.setPixelColor(x, pixels.Color(0, 0, 0)); // Pixel número "0" - Cores (Red = 0, Green = 0, Blue = 0)
        pixels.show();
    }
      delay(250);
  }


  if ( ktc.readCelsius() <= 62.2 ) {

 
      for (int x = 0; x <= 7; x++) {
        pixels.setPixelColor(x, pixels.Color(0, 0, 255)); // Pixel número "0" - Cores (Red = 0, Green = 0, Blue = 0)
        pixels.show();
      }
      delay(500);
      
      for (int x = 0; x <= 7; x++) {
        pixels.setPixelColor(x, pixels.Color(0, 0, 0)); // Pixel número "0" - Cores (Red = 0, Green = 0, Blue = 0)
        pixels.show();
    }
      delay(500);
  }


  if ( ktc.readCelsius() < 68.2 && ktc.readCelsius() > 62.2 ) {


      for (int x = 0; x <= 7; x++) {
        pixels.setPixelColor(x, pixels.Color(0, 255, 0)); // Pixel número "0" - Cores (Red = 0, Green = 255, Blue = 0)
        pixels.show();
      }
  }

}
