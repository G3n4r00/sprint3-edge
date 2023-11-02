//Autor: Gabrel Genaro ; Paloma Mirela



#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient
 
//defines:

// Definindo o endereço I2C do LCD
#define I2C_ADDR 0x27


LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);

// Definindo as portas SDA e SCL
#define SDA_PIN 22
#define SCL_PIN 23


#define BUZZER_PIN 27

//defines de id mqtt e tópicos para publicação e subscribe denominado TEF(Telemetria e Monitoramento de Equipamentos)
#define TOPICO_SUBSCRIBE    "/TEF/cleancam002/cmd"        //tópico MQTT de escuta
#define TOPICO_PUBLISH      "/TEF/cleancam002/attrs"      //tópico MQTT de envio de informações para Broker
#define TOPICO_PUBLISH_2    "/TEF/cleancam002/attrs/l"    //tópico MQTT de envio de informações para Broker
                                                      //IMPORTANTE: recomendamos fortemente alterar os nomes
                                                      //            desses tópicos. Caso contrário, há grandes
                                                      //            chances de você controlar e monitorar o ESP32
                                                      //            de outra pessoa.
#define ID_MQTT  "fiware_cleancam_002"      //id mqtt (para identificação de sessão)
                                 //IMPORTANTE: este deve ser único no broker (ou seja, 
                                 //            se um client MQTT tentar entrar com o mesmo 
                                 //            id de outro já conectado ao broker, o broker 
                                 //            irá fechar a conexão de um deles).
                                 // o valor "n" precisa ser único!
                                

// WIFI
const char* SSID = "Wokwi-GUEST"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = ""; // Senha da rede WI-FI que deseja se conectar
  
// MQTT
const char* BROKER_MQTT = "46.17.108.113"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
 
//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
char EstadoSaida = '0';  //variável que armazena o estado atual da saída
#define TRIGGER_PIN1 26
#define ECHO_PIN1 25
#define TRIGGER_PIN2 21
#define ECHO_PIN2 19

// Define variables:
long duration1, duration2;
int distance1, distance2;
  
//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);
 
/* 
 *  Implementações das funções
 */
void setup() 
{
    //inicializações:
    initSerial();
    initWiFi();
    initMQTT();
    Wire.begin(SDA_PIN, SCL_PIN);
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.clear();    delay(5000);
    // Define inputs and outputs:
    pinMode(TRIGGER_PIN1, OUTPUT);
    pinMode(ECHO_PIN1, INPUT);
    pinMode(TRIGGER_PIN2, OUTPUT);
    pinMode(ECHO_PIN2, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    MQTT.publish(TOPICO_PUBLISH, "s|on");
}
  
//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial 
//        o que está acontecendo.
//Parâmetros: nenhum
//Retorno: nenhum
void initSerial() 
{
    Serial.begin(115200);
}
 
//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}
  
//Função: inicializa parâmetros de conexão MQTT(endereço do 
//        broker, porta e seta função de callback)
//Parâmetros: nenhum
//Retorno: nenhum
void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
  
//Função: função de callback 
//        esta função é chamada toda vez que uma informação de 
//        um dos tópicos subescritos chega)
//Parâmetros: nenhum
//Retorno: nenhum
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
     
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
    
     
}
  
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
  
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 
//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
 
bool mensagemMudou = false;

 
//programa principal
void loop() 
{   
    char msgBuffer[6];
    VerificaConexoesWiFIEMQTT();




  // Clear the trigPin by setting it LOW:
  digitalWrite(TRIGGER_PIN1, LOW);
  delayMicroseconds(6);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(TRIGGER_PIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN1, LOW);

  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration1 = pulseIn(ECHO_PIN1, HIGH);
  // Calculate the distance:
  distance1 = duration1 * 0.034 / 2;

  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
  Serial.print("Distance = ");
  Serial.print(distance1);
  Serial.println(" cm");

    // Clear the trigPin by setting it LOW:
  digitalWrite(TRIGGER_PIN2, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(TRIGGER_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN2, LOW);

  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration2 = pulseIn(ECHO_PIN2, HIGH);
  // Calculate the distance:
  distance2 = duration2 * 0.034 / 2;

  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
  Serial.print("Distance = ");
  Serial.print(distance2);
  Serial.println(" cm");

  delay(500);    

      String novaMensagem = "";
      String mensagemAnterior = "";

    if (distance1 > 0 && distance2 > 0) {
        float averageDistance = (distance1 + distance2) / 2.0;

        // Lógica para determinar o status da lixeira
        if (averageDistance < 50) {  // Adequada
            novaMensagem = "Adequada";
            digitalWrite(BUZZER_PIN, LOW);
        } else if (averageDistance < 100) { // A lixeira está quase cheia
            novaMensagem = "Quase Cheia";
            digitalWrite(BUZZER_PIN, LOW);
        } else {  // A lixeira está completamente cheia
            novaMensagem = "Lotada";
            digitalWrite(BUZZER_PIN, HIGH);
        }

      if (novaMensagem != mensagemAnterior) { 
      mensagemMudou = true; // A mensagem mudou
      mensagemAnterior = novaMensagem;
    }

    // Verifique se a mensagem mudou antes de limpar o display
    if (mensagemMudou) {
      lcd.clear();  // Limpa o display apenas quando a mensagem muda
      lcd.setCursor(0, 0);
      lcd.print("Status:");
      lcd.setCursor(0, 1);
      lcd.print(novaMensagem);
      char msgBuffer[5];
      dtostrf(averageDistance, 4, 2, msgBuffer);
      MQTT.publish(TOPICO_PUBLISH_2, msgBuffer);   
      
      // Reset a variável para indicar que a mensagem foi exibida
      mensagemMudou = false;

      delay(1000); // Aguarde 1 segundo após atualizar o display
    }

   }
   else {
        // Se houver erro na medição, exibir mensagem de erro no LCD
        if (mensagemAnterior != "Erro na medição.") {
            lcd.clear();  // Limpa o display apenas quando a mensagem muda
            lcd.setCursor(0, 0);
            lcd.print("Erro na medição.");
            lcd.setCursor(0, 1);
            lcd.print("Verifique sensores.");

            // Atualize a mensagem anterior para indicar que uma mensagem de erro está sendo exibida
            mensagemAnterior = "Erro na medição.";
        }
    }
}

