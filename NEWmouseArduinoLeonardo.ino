#include <Mouse.h>
#include <Keyboard.h>

/*-----------------------------------------------------------------------*/
#define verde 2
#define azul 3
#define vermelho 4
#define amarelo 5
#define right 6
#define left 7
#define down 8
#define up 9
#define potenciometro A2
#define botao_prog 10
#define led 16
/*-----------------------------------------------------------------------*/
bool mouse=1;
int sens, timer; 
char player;

struct Botao {
  bool estadoAtual = 1;
  bool estadoPassado = 1;
  bool debounce = 1;
  char opcao;
};

Botao b1, b2, b3, b4;

void estadoBotao();
void prog();
void select_opcao(char x,Botao *p);

void setup() {
  Serial.begin(9600);
  Mouse.begin();
  Keyboard.begin();

  pinMode(verde, INPUT_PULLUP);
  pinMode(azul, INPUT_PULLUP);
  pinMode(vermelho, INPUT_PULLUP);
  pinMode(amarelo, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(botao_prog,INPUT_PULLUP);
  pinMode(led,OUTPUT);

  digitalWrite(led, LOW);

  //preset dos comandos dos botoes
  b1.opcao = 4;
  b2.opcao = 1;
  b3.opcao = 3;
  b4.opcao = 2;
}

void loop() {
  estadoBotao();
  
if (mouse){//modo mouse
  digitalWrite(led,LOW);//led apagado para modo MOUSE
  
  if (!b1.debounce) select_opcao(b1.opcao, &b1); //switch VERDE press
  if (!b2.debounce) select_opcao(b2.opcao, &b2); //switch AZUL press
  if (!b3.debounce) select_opcao(b3.opcao, &b3); //switch VERMELHO press
  if (!b4.debounce) select_opcao(b4.opcao, &b4); //switch AMARELO press
  
  
  while (!digitalRead(botao_prog)){  //switch de programacao press
    estadoBotao();
    Serial.print("botao prog press\n");
    timer++;
    if (!b1.debounce){timer=0;mouse=0;player=1;}
    if (!b2.debounce){timer=0;mouse=0;player=2;}
    if (!b3.debounce){timer=0;mouse=0;player=3;}
    if (!b4.debounce){timer=0;mouse=0;player=4;}
    if (timer>300){timer=0; prog();} //espera um determinado tempo para q a funçao prog seja executada
  }

  
  sens=map(analogRead(potenciometro),0,900 ,1012,1023); //faz a leitura do potenciometro e ja converte para uma faixa de 1012 até 1023
  if(sens==1023)sens=0;//quando o valor for 1023 dizemos q ele é zero para nao haver movimento do cursor
  Serial.print(analogRead(potenciometro));
  Serial.print("\n");
  
  if (!digitalRead(right))Mouse.move(-sens, 0, 0); //movimento right, porta 6
  if (!digitalRead(left))Mouse.move(sens, 0, 0); //movimento left, porta 7
  if (!digitalRead(down))Mouse.move(0, -sens, 0); //movimento down, porta 8
  if (!digitalRead(up))Mouse.move(0, sens, 0); //movimento up, porta 9

}

else{ //MODO FLIPERAMA
  digitalWrite(led,HIGH); //led aceso para modo FLIPERAMA

  //troca para função mouse
  while (!digitalRead(botao_prog)){  //switch de programacao press
    estadoBotao();
    Serial.print("botao prog press\n");
    if (!b1.debounce)mouse=1;
    if (!b2.debounce)mouse=1;
    if (!b3.debounce)mouse=1;
    if (!b4.debounce)mouse=1;
  }
  
  if(player==1){
    if (!digitalRead(right))Keyboard.print("d");
    if (!digitalRead(left))Keyboard.print("a");
    if (!digitalRead(down))Keyboard.print("s");
    if (!digitalRead(up))Keyboard.print("w");
    if (!b1.debounce)Keyboard.print("q");
    if (!b2.debounce)Keyboard.print("e");
    if (!b3.debounce)Keyboard.print("z");
    if (!b4.debounce)Keyboard.print("x");
  }
    if(player==2){
    if (!digitalRead(right))Keyboard.print("h");
    if (!digitalRead(left))Keyboard.print("f");
    if (!digitalRead(down))Keyboard.print("g");
    if (!digitalRead(up))Keyboard.print("t");
    if (!b1.debounce)Keyboard.print("r");
    if (!b2.debounce)Keyboard.print("y");
    if (!b3.debounce)Keyboard.print("c");
    if (!b4.debounce)Keyboard.print("v");
  }
    if(player==3){
    if (!digitalRead(right))Keyboard.print("l");
    if (!digitalRead(left))Keyboard.print("j");
    if (!digitalRead(down))Keyboard.print("k");
    if (!digitalRead(up))Keyboard.print("i");
    if (!b1.debounce)Keyboard.print("u");
    if (!b2.debounce)Keyboard.print("o");
    if (!b3.debounce)Keyboard.print("b");
    if (!b4.debounce)Keyboard.print("n");
  }
    if(player==4){
    if (!digitalRead(right))Keyboard.print("6");
    if (!digitalRead(left))Keyboard.print("4");
    if (!digitalRead(down))Keyboard.print("5");
    if (!digitalRead(up))Keyboard.print("8");
    if (!b1.debounce)Keyboard.print("7");
    if (!b2.debounce)Keyboard.print("9");
    if (!b3.debounce)Keyboard.print("1");
    if (!b4.debounce)Keyboard.print("2");
  }
  
}

}

void prog() {
  int ok;
  b1.opcao = 0;
  b2.opcao = 0;
  b3.opcao = 0;
  b4.opcao = 0;
  for (int cont = 1; cont < 5; cont++){
    ok=1;
    do {
      estadoBotao();
      timer++;
      if (timer>50){digitalWrite(led,!digitalRead(led));timer=0;}
      
      if (((b1.debounce == 0) && (b1.opcao == 0))){b1.opcao = cont; ok=0;}
      if (((b2.debounce == 0) && (b2.opcao == 0))){b2.opcao = cont; ok=0;}
      if (((b3.debounce == 0) && (b3.opcao == 0))){b3.opcao = cont; ok=0;}
      if (((b4.debounce == 0) && (b4.opcao == 0))){b4.opcao = cont; ok=0;}
    } while (ok);
  }
}


void estadoBotao() {
  b1.estadoPassado = b1.estadoAtual;
  b1.estadoAtual = digitalRead(verde);
  if (b1.estadoAtual == 0 && b1.estadoPassado == 1)b1.debounce = 0;
  else b1.debounce = 1;

  b2.estadoPassado = b2.estadoAtual;
  b2.estadoAtual = digitalRead(azul);
  if (b2.estadoAtual == 0 && b2.estadoPassado == 1)b2.debounce = 0;
  else b2.debounce = 1;

  b3.estadoPassado = b3.estadoAtual;
  b3.estadoAtual = digitalRead(vermelho);
  if (b3.estadoAtual == 0 && b3.estadoPassado == 1)b3.debounce = 0;
  else b3.debounce = 1;

  b4.estadoPassado = b4.estadoAtual;
  b4.estadoAtual = digitalRead(amarelo);
  if (b4.estadoAtual == 0 && b4.estadoPassado == 1)b4.debounce = 0;
  else b4.debounce = 1;
  
  delay (10);
}

void select_opcao(char x, Botao *p){
  switch (x) {
      case 1: Mouse.click(MOUSE_LEFT); break;
      case 2: Mouse.click(MOUSE_RIGHT); break;
      case 3: Mouse.click(MOUSE_LEFT); delay(200); Mouse.click(MOUSE_LEFT); break;
      case 4: Mouse.press(MOUSE_LEFT);p->opcao=5; break;
      case 5: Mouse.release(MOUSE_LEFT);p->opcao=4; break;
      default: break;
  }
}
