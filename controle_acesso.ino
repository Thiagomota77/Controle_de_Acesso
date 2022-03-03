// Programa : RFID - Controle de acesso / cancela
// Autor : Arduino e Cia
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 
 
Servo microservo9g;
#define SS_PIN 10
#define RST_PIN 9
// Definicoes pino modulo RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 
// Leds indicadores acesso liberado ou negado
int led_liberado = 5;
int led_negado = 6;
const int pinoBuzzer = 7;
char st[20];
void setup() 
{
    pinMode(pinoBuzzer, OUTPUT);

  pinMode(led_liberado, OUTPUT);
  pinMode(led_negado, OUTPUT);
  // Define que o servo esta ligado a porta digital 3
  microservo9g.attach(3);
  // Move o servo para a posicao inicial (cancela fechada)
  microservo9g.write(90);
  // Inicia a serial
  Serial.begin(9600);
  // Inicia  SPI bus
  SPI.begin();
  // Inicia MFRC522
  mfrc522.PCD_Init(); 
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
}
void loop() 
{
  // Aguarda a aproximacao do cartao
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona um dos cartoes
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  // Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
  // Testa se o cartao1 foi lido
  if (conteudo.substring(1) == "C9 DC DC 24")
  {
    // Levanta a cancela e acende o led verde
    microservo9g.write(-90);
    digitalWrite(led_liberado, HIGH);
    digitalWrite(pinoBuzzer, HIGH); 
    delay(2000);
    Serial.println("RenaM - Acesso liberado !");
     Serial.println("Boa aula !");
    Serial.println();
    delay(3000);
    microservo9g.write(90);
    digitalWrite(led_liberado, LOW);
    }
    
  // Testa se o cartao2 foi lido
  else  
  {
    Serial.println("Vitor - Acesso negado !!");
    Serial.println("Entre em contato com a tessouraria  !!");
    Serial.println();
    // Pisca o led vermelho
    for (int i= 1; i<5 ; i++)
    {
      digitalWrite(led_negado, HIGH);
      delay(200);
      digitalWrite(led_negado, LOW);
      delay(200);
    }
  }
  delay(1000);
}
