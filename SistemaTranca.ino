// Bibliotecas
#include <LiquidCrystal_I2C.h> 
#include <Keypad.h>

// Configurando LCD
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Configurando teclado
const byte LINHAS = 4;
const byte COLUNAS = 4;

char digitos[LINHAS][COLUNAS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}}; // Posicionando todos os caracteres do teclado em um array

byte pinosLinha[LINHAS] = { 13, 12, 11, 10 }; // Pinos de conexão das linhas no arduino 
byte pinosColuna[COLUNAS] = { 9, 8, 7, 6 }; // Pinos de conexão das colunas no arduino 

Keypad teclado = Keypad( makeKeymap(digitos), pinosLinha, pinosColuna, LINHAS, COLUNAS ); // Objeto de leitura das teclas com os parâmetros lidos

// Variáveis
int security = 0; 
int cursor = 0; 
int x; 
const byte porta = 6; 
const byte ledVermelho = 3; 
const byte ledVerde = 2; 
const byte tranca1 = 4; 
const byte tranca2 = 5; 
String estado; 
String senha = "123456";
String masterPass = "654321"; 
String digitada = ""; 

void (*reset)() = 0; 

void setup() { 
  pinMode(ledVermelho, OUTPUT); 
  pinMode(ledVerde, OUTPUT); 
  pinMode(tranca1, OUTPUT); 
  pinMode(tranca2, OUTPUT); 
  pinMode(porta, INPUT); 

  if(digitalRead(porta) == LOW){
    lcd.init(); 
    lcd.backlight(); 
    lcd.clear();
    estado = "aberta"; 
    loop(); 
  }
  
  digitalWrite(ledVerde, LOW); 
  digitalWrite(ledVermelho, HIGH); 
  digitalWrite(tranca1, LOW); 
  digitalWrite(tranca2, LOW); 

  
  lcd.init(); 
  lcd.backlight(); 
  lcd.clear();
  setupLcd(); 
}

void loop() {
  char tecla = teclado.getKey(); 

  if(estado == "aberta"){ 
    lcd.clear(); 
    lcd.print("  Porta Aberta  "); 
    delay(10000);
    reset(); 
  } else 
  { 
    if(security != 4 && security != 3){ 
      if(tecla){ 
        if(tecla != '*' && tecla != '#'){
          digitada += tecla;
          cursor++; 
          lcd.print(tecla); 
        }

        if(tecla == '#'){
          if (digitada != ""){
            confirmarSenha();
          } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("  Senha  Vazia  ");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Digite a senha ");
            apagar(); 
          }
        }

        if(tecla == '*'){ 
          apagar(); 
        }

        if(cursor == 7){ 
          apagar(); 
        }
      }
    }
  
    if(security == 4){ 
      if(tecla){ 
          if(tecla != '*' && tecla != '#'){
            digitada += tecla;
            cursor++;
            lcd.print(tecla);
          }
          
          if(tecla == '#'){
            if (digitada != ""){
            confirmarMaster(); 
          } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("  Senha  Vazia  ");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("     Master     ");
            apagar(); 
          }
          }
          
          if(tecla == '*'){
            apagar(); 
          }
          
          if(cursor == 7){
            apagar(); 
          }
      }
    }

    if(security == 3){ 
      master(); 
      security = 4; 
    }

  }

}

void setupLcd(){ 
  lcd.setCursor(0, 0);
  lcd.print(" Digite a senha ");
  apagar(); 
}

void confirmarSenha(){ 
  if(digitada == senha){
    estado = "aberta";
    apagar(); 
    
    lcd.setCursor(1, 0);
    lcd.print("Senha  Correta");
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);
    delay(3000);
    
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);
    
    security = 0;
  } else {
    senhaIncorreta(); 
  }
}

void senhaIncorreta(){ 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha  Incorreta");
    
    for(int i=0; i<=5; i++){ 
      digitalWrite(ledVermelho, LOW);
      delay(200);
      digitalWrite(ledVermelho, HIGH);
      delay(200);
    }
    
    security++;
    x = 3 - security; 
    
    lcd.setCursor(2, 1);
    lcd.print(x);
    lcd.print(" restantes");
    delay(3000);
    
    setupLcd(); 
}

void apagar() { 
  digitada = "";
  cursor = 0;
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(5, 1);
}

void master(){ 
  digitalWrite(tranca1, HIGH);
  digitalWrite(tranca2, HIGH);
  
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     Sistema    ");
  lcd.setCursor(0, 1);
  lcd.print("     Travado    ");
  delay(8000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     Master     ");
  lcd.setCursor(0, 1);
  apagar(); 
}

void senhaIncorretaMaster(){ /
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Senha  Incorreta");
  delay(5000);
  master();
}
r
void confirmarMaster(){ 
  if(digitada == masterPass){
    digitalWrite(tranca1, LOW);
  	digitalWrite(tranca2, LOW);
    
    apagar(); 
    lcd.setCursor(0, 0);
    lcd.print(" Senha  Correta ");
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);
    digitalWrite(tranca1, LOW);
    digitalWrite(tranca2, LOW);
    delay(5000);
    
    lcd.clear(); 
    lcd.print(" Seja bem-vindo "); 
    lcd.setCursor(0, 1); 
    lcd.print("    Aguarde.    ");
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);
    security = 0;
    delay(2000);

    setupLcd();
  } else {
    senhaIncorretaMaster();
  }
}