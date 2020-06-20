//Circuito usando em um teclado basico CASIO MA-150 com saida MIDI_OUT
#include <SoftwareSerial.h>

//Conectar a saida do circuito que vem do teclado ao pino D2 do arduino (RX) 
SoftwareSerial mySerial(2, 3); // RX, TX

byte message; //Referente ao NoteOn 144 ou NoteOff 128, teclados MIDI com Saidas MIDI OUT podem nao enviar o codigo 128 (NoteOff) no lugar é enviado o 144 (NoteOn) com Velocity / Volume = 0 indicando sem som.
byte note; //Nota a ser tocada. Exemplo 60 = C3...
byte intensity; //Intensidade / Volume - Esse numero vai de 0-127 indicando a intensidade de uma nota ou de uma batida no caso de bateria.
int contador = 1; //Apenas usado em um dos modelos abaixo, quando recebemos uma sequencia midi de um teclado, ela vem dividida em 3 envios: 1. NoteOn/Off - 2. Nota e o 3.Envio a intensidade, o contador serve para saber qual o envio
int canalMidi = 0; //Canal Midi vai de 0-15 (1-16) nos teclados e softwares, servira para usar em um dos modelos abaixo.
int statusLed = 5;   // LED que pisca ao receber as Notas do teclado (LED VERDE) - Pino D5
int statusLedOn = 6;   // LED que vai ficar ligado o tempo todo para saber que a placa esta energizada (LED VERMELHO) - Pino D6

void setup() 
{
    pinMode(statusLed,OUTPUT);  //Setando o pino do LED de status como Output
    pinMode(statusLedOn,OUTPUT);  //Setando o pino do LED de statusOn como Output

    digitalWrite(statusLedOn, HIGH); //Liga o LED de StatusOn
    
    for (int x = 0; x < 10; x = x + 1) { //Rotina para piscar o LED 10x na inicializacao
      digitalWrite(statusLed, HIGH); delay(50); digitalWrite(statusLed, LOW); delay(50);}
  
    mySerial.begin(31250); //Velocidade do BaudRate de um teclado com saida MIDI_OUT (usado na recepcao dos dados do teclado no arduino)
    Serial.begin(115200); //Velocidade para enviar os dados recebidos do teclado para o soft (hairless Midi <-> Serial Midi), velocidades menores causam erros.
}


void loop()
{
    if(mySerial.available()){

                //Se quiser enviar para o canal Midi que esta setado no teclado basta o comando abaixo, se o teclado estiver no canal midi 1 sera enviado para o canal midi 1 e assim por diante:
                //Serial.write(mySerial.read());
                //piscaLed(); //Funcao para piscar o LED ao receber as notas pela Serial
      
                //Caso queira especificar um Canal Midi use o comando abaixo alterando a variavel canalMidi nas variaveis acima, nesse exemplo ele ira pegar o canal midi que esta configurado no telado e adicionar + 9:
                //Util se quiser setar o envio para o Canal 10 que é de percursao, novamente isso serve apenas para os casos onde nao se pode alterar o canal MIDI no teclado.
                if (contador == 1) {message = mySerial.read(); contador++; Serial.write(message + canalMidi);} //Capturando o Primeiro envio de 3 (NoteOn/Off)
                else if (contador == 2) {note = mySerial.read(); contador++; Serial.write(note);} //Capturando o Segundo envio de 3 (Nota)
                else if (contador == 3) {intensity = mySerial.read(); contador = 1; Serial.write(intensity); piscaLed();} //Capturando o Terceiro envio de 3 (Velocidade/Intensidade) e voltando o contador para 1 para aguardar o proximo envio
    }
}

void piscaLed() 
{
    digitalWrite(statusLed, HIGH);
    delay(5);
    digitalWrite(statusLed, LOW);
}
