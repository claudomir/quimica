#include <SoftwareSerial.h>
// Bibliotecas do leitor RFID
#include <SPI.h>
#include <MFRC522.h>
// Bibliotecas do player de audio
#include <DFRobotDFPlayerMini.h>

// Definindo as portas SDA e RST do módulo RFID
#define SS 10
#define RST 9
MFRC522 sensor(SS, RST);

// Definindo as portas TX e RX do módulo MP3
#define RX 7
#define TX 6
DFRobotDFPlayerMini playerMini;

#define VOLUME 27 // Volume inicial do alto-falante

SoftwareSerial sf(RX, TX);
char st[20];

void setup() {
  //INICIAR MÓDULOS
  Serial.begin(9600); // Inicializa a Serial
  sf.begin(9600);     // Inicializa o SoftwareSerial
  SPI.begin();        //Inicializa o RFID
  sensor.PCD_Init();

  Serial.println("Inicializando, aguarde...");
  Serial.println();

//SEGUIR O PROGRAMA APENAS SE O MP3 ESTIVER FUNCIONANDO
  if (!playerMini.begin(sf)) {
    Serial.println("Não foi possível inicializar.");
    Serial.println("Verifique as conexões e se o cartão está inserido!");
    while (true);
  }

  //INICIAR O MÓDULO MP3 E ATRELAR A FUNÇÃO DO VOLUME À VARIÁVEL DE VOLUME DO INÍCIO
  playerMini.setTimeOut(500);
  playerMini.volume(VOLUME);

//INICIALIZADO COM SUCESSO 
  Serial.println("Inicializado com sucesso!");
  Serial.println();
  playerMini.play(6); //REPRODUZIR ÁUDIO QUANDO INICIAR
  delay(1000);

  Serial.println("Aproxime a Tag do leitor...");
  Serial.println();
  mensagemInicial();
}

//FUNÇÃO PARA O RFID BUSCAR SINAL DO CARTÃO
void loop() {
    // Procura por novos cartões
    if (!sensor.PICC_IsNewCardPresent()) {
    return;
  }
    // Seleciona um dos cartões detectados
    if (!sensor.PICC_ReadCardSerial()) {
    return;
  }
  // Exibir o ID traduzido e definir a tradução do sinal para um ID
  Serial.print("ID  :");
  String conteudo = "";
  for (byte i = 0; i < sensor.uid.size; i++) {
    Serial.print(sensor.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(sensor.uid.uidByte[i], HEX);
    conteudo.concat(String(sensor.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(sensor.uid.uidByte[i], HEX));
  }
  Serial.println();
  conteudo.toUpperCase();
  conteudo.trim();         // (Opcional, mas recomendado)
  String uid = conteudo;   // UID final para comparação

  Serial.print(uid); //printar o valor do uid
  verificaFigura(uid);   // Verificar qual figura corresponde
}

void mensagemInicial() {
  Serial.println("Aproxime a Tag do leitor");
  Serial.println(); 
}

//VERIFICAR O VALOR DO CARTÃO COM O VALORES ABAIXO
void verificaFigura(String uid) {
  if (uid == "53 F4 AD 61 01 00 01") {
    Serial.println("H2O");
    sensor.PCD_StopCrypto1();
    delay(150);
    playerMini.play(1);
  } else if (uid == "53 EF AD 61 01 00 01") {
    Serial.println("CO2");
    sensor.PCD_StopCrypto1();
    delay(150);
    playerMini.play(2);
  } else if (uid == "53 04 AE 61 01 00 01") {
    Serial.println("NaCL");
    sensor.PCD_StopCrypto1();
    delay(150);
    playerMini.play(3);
  } else if (uid == "53 F6 AD 61 01 00 01") {
    Serial.println("Mg(OH)2");
    sensor.PCD_StopCrypto1();
    delay(150);
    playerMini.play(4);
  } else if (uid == "53 F6 AD 61 01 00 01") {
    Serial.println("HNO2");
    sensor.PCD_StopCrypto1();
    delay(150);
    playerMini.play(5);
  }
  delay(500); // Dá tempo para o som iniciar antes de reativar a leitura
  Serial.println();
  mensagemInicial();
  }