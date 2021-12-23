/*=======================================================================
  Esse é o projeto de sistema de irrigação para apartamento usando uma
  bomba d'água para garrafão de água mineral. 

  Em caso de dúvidas, acesse:
  https://elcereza.com/sistema-de-irrigacao-para-apartamento
  Site  : https://elcereza.com/
   
  Autor : Gustavo Cereza
  Disponibilizado por : Elcereza
  Redes : @ellcereza
  Canal : https://t.me/elcereza
=======================================================================*/

#define uS_TO_S_FACTOR 1000000                                                    // Constante
#define TIME_TO_SLEEP  720                                                        // Tempo em minutos dormindo (12h)
#define Working_Time   15                                                         // Tempo de trabalho em segundos

#define feedbackPump   16                                                         // GPIO que verifica se a bomba está ligada.
#define controlPump    17                                                         // GPIO que liga ou desliga a bomba

void DeepSleep(){
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR * 60);             // Define e ativa o acordar por um tempo desejado.
  esp_deep_sleep_start();                                                         // Coloca o ESP32 em deep sleep
}

void pulse()
{
  digitalWrite(controlPump, 0);                                                   // O circuito da placa aciona em lógico baixo
  delay(100);
  digitalWrite(controlPump, 1);
  delay(100);
}

void onTime(double t){
  if(digitalRead(feedbackPump) == 0){                                             // Verifica se a bomba está ativada, se tiver desliga e volta a dormir
    pulse();                                                                      // Liga a bomba
    if(digitalRead(feedbackPump) == 0)                                            // Verifica se a bomba está desliga, se tiver irá ligar
      pulse();
    
    delay(t * 1000);                                                              // Tempo da bomba ligado

    pulse();                                                                      // Desliga a bomba
    if(digitalRead(feedbackPump) == 1)                                            // Verifica se a bomba ainda está ligada, se tiver desliga.
      pulse();
  }
  else
  {
    pulse();
    delay(100);
  }
}

void setup() {
  pinMode(feedbackPump, INPUT);
  pinMode(controlPump, OUTPUT);

  onTime(Working_Time);
  delay(1000);
  DeepSleep();                                                                    // Coloca o ESP32 para dormir
}

void loop() {
}
