//definição dos pinos da bomba, do sensor e de interrupção
const int INTERRUPCAO_SENSOR = 0; //interrupt = 0 equivale ao pino digital 2
const int PINO_SENSOR = 2;
const int PINO_BOMBA = 6;
 
//definição da variável de contagem de voltas
unsigned long contador = 0;
 
//definição do fator de calibração para conversão do valor lido
const float FATOR_CALIBRACAO = 4.5;
 
//definicao das variaveis de fluxo e volume
float fluxo = 0;
float volume = 0;
float volume_total = 0;
float volume_mL = 0;
float volume_Desejado = 0;
 
//definição da variável de intervalo de tempo
unsigned long tempo_antes = 0;
 
void setup(){
 
  //inicialização do monitor serial
  Serial.begin(9600);
 
  //mensagem de inicialização
  Serial.println("Medidor de Volume de Líquidos\n");
 
   
  Serial.println("Insira a quantidade de líquido desejada: ");
 
 
 
  //configuração do pino do sensor como entrada em nivel logico alto
  pinMode(PINO_SENSOR, INPUT_PULLUP);
  pinMode(PINO_BOMBA, OUTPUT);
 
}
 
void loop() {
    // apenas responde quando dados são recebidos:
  if (Serial.available() > 0) {
   // lê do buffer o dado recebido:
    volume_total = 0;
    volume_Desejado =  Serial.parseInt();
 
   // responde com o dado recebido:
   Serial.print("Bombeando: ");
   Serial.print(volume_Desejado);
   Serial.println(" mL");
  }
 
  volume_mL = (volume_total*1000);
 
  if (volume_Desejado > 0)
  {
    //Cria pwm acionamento da bomba
    analogWrite(PINO_BOMBA, 1);
 
    //executa a contagem de pulsos uma vez por segundo
    if((millis() - tempo_antes) > 1000){
 
      //desabilita a interrupção para realizar a conversão do valor de pulsos
      detachInterrupt(INTERRUPCAO_SENSOR);
 
      //conversão do valor de pulsos para L/min
      fluxo = ((1000.0 / (millis() - tempo_antes)) * contador) / FATOR_CALIBRACAO;
 
      //calculo do volume em L passado pelo sensor
      volume = fluxo / 60;
 
      //armazenamento do volume
      volume_total += volume;
 
      //exibição do valor de volume
      Serial.print("Volume: ");
      Serial.print(volume_mL);
      Serial.println(" mL");
   
      //reinicialização do contador de pulsos
      contador = 0;
 
      //atualização da variável tempo antes
      tempo_antes = millis();
 
      //contagem de pulsos do sensor
      attachInterrupt(INTERRUPCAO_SENSOR, contador_pulso, FALLING);
     
    }
 
    //desliga a bomba e zera as quantidades de líquido desejada e total
    if (volume_mL >= volume_Desejado)
    {
      Serial.print("Volume de: ");
      Serial.print(volume_Desejado);
      Serial.println(" mL");
      Serial.println("Aguarde um momento");
      analogWrite(PINO_BOMBA, 0);
      volume_Desejado = 0;
      delay(10000);
      volume_total = 0;
      Serial.println("Insira a quantidade de liquido desejada: ");
    }
  }
}
 
//função chamada pela interrupção para contagem de pulsos
void contador_pulso() {
  contador++;
}


