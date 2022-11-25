#include <Keypad.h>
#include <EEPROM.h>
#include <Servo.h>
#define offset 10
Servo lock_servo ;
const uint8_t ROWS = 4;
const uint8_t COLS = 4;
String input_code ;
char keypressed;
int lock_statement=0 ; 
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
uint8_t colPins[COLS] = { 5, 4, 3, 2 }; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = { 9, 8, 7, 6 }; // Pins connected to R1, R2, R3, R4
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
bool lock_state=false;
String password="1224";
String get_password(){
  String input_str="";
  while(keypressed!='B' && input_str.length()<4){
    char keypressed=keypad.getKey();
  if(keypressed !=NO_KEY && keypressed!='B'){
    if(keypressed>='0' && keypressed<='9'){
    input_str+=keypressed;
    }  
    }

  }
  return input_str ;
}

bool change_password(){
  Serial.flush();
  Serial.println("Enter New Password");
  String new_pass=get_password();
  delay(100);
  Serial.println("Confrim your password");
  String confrim_pass=get_password();
  if(confrim_pass.equals(new_pass)){
    password=confrim_pass;
    EEPROM.write(offset,4);
    for(int k=0;k<4;k++){
    EEPROM.write(offset+5+k,password[k]);
  }
    Serial.println("confrimed");
    delay(100);
    return true ;
  };
  
}
void lock(){
  lock_servo.write(90);
}
void unlock(){
  lock_servo.write(0);
}
String read_password(){
  char pass_data[4];
  for(int j=0;j<4;j++){
    pass_data[j]=EEPROM.read(offset+5+j);
    
  }
  String pass_value=String(pass_data);
  return pass_value ;
}
void setup() {
  Serial.begin(9600);
  Serial.println("****configuring Safe box*****");
  lock_servo.attach(11);
  byte len=password.length();
  EEPROM.write(offset,4);
  for(int k=0;k<4;k++){
    EEPROM.write(offset+5+k,password[k]);
  }
  Serial.println("****password saved*****");

}

void loop() {
  String pass_value=read_password();
  delay(10);
  char keypressed=keypad.getKey();
  if(keypressed=='#'&&keypressed){
    password=change_password();
  }
  if(keypressed){
    String input_code=get_password();
  Serial.print(input_code);
  Serial.println();
  delay(10);
  int a=input_code.compareTo(pass_value);
  if(a<=0){
      lock_statement=!lock_statement;
      if(lock_statement==1){
        unlock();
        Serial.println("unlocked");
      }
      if(lock_statement==0){
        lock();
        Serial.println("locked");

      }

    }
  }
  
  }
  
