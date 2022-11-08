#define N_WE 10 // (negation)write enable pin
#define N_OE 11 // (negation)output enable pin

#define ADDR_WIDTH 15 //address bus width
#define ADDR_START 22 //first address pin on arduino

#define DATA_WIDTH 8 //data bus width
#define DATA_START 40 //first data pin on arduino

void setup() {
  Serial.begin(9600);
  init();
  setAddress(10,1);
  printContents();
}

void loop() {
  int i;
  for(i=0;i<255;i++){
     setAddress(i,i);
  printContents(); 
  }
}

void setAddress(int addr, byte val){
  if(checkAddr(addr)){
    setAddrPins(addr);
    writeByte(val);
  }
}

byte readAddress(int addr){
  if(checkAddr(addr)){
    setAddrPins(addr);
    return readByte();
  } 
}

void init(){
  int pin;
  
  for(pin=ADDR_START;pin<(ADDR_START + ADDR_WIDTH);pin++){
    pinMode(pin,OUTPUT);
    digitalWrite(pin, LOW);
  }
  pinMode(N_WE,OUTPUT);
  digitalWrite(N_WE,HIGH);
  pinMode(N_OE,OUTPUT);
  digitalWrite(N_OE,HIGH);
  
}

void setAddrPins(int addr){
  byte res;
  int pin;
  for(pin=ADDR_START;pin<(ADDR_START + ADDR_WIDTH);pin++){
    res = ((addr >> (pin-ADDR_START)) & 0x01);
    if(res){
      digitalWrite(pin, HIGH);
    } else{
      digitalWrite(pin, LOW);
    }
  }
}

int checkAddr(int addr){
    if(addr > 32768 || addr < 0){
    Serial.write("ERR: Wrong address");
    return 0;
  }
  return 1;
}

void writeByte(byte val){
  int pin,i;
  byte res;
    for(pin=DATA_START;pin<(DATA_START + DATA_WIDTH);pin++){
    pinMode(pin,OUTPUT);
    digitalWrite(pin, val & 1);
    val = val >> 1;
  }
  digitalWrite(N_WE,LOW);
  for(i=0;i<100;i++);
  digitalWrite(N_WE,HIGH); 
}

byte readByte(){
  int pin;
  byte val=0;
  digitalWrite(N_OE,LOW);
    for(pin=DATA_START;pin<(DATA_START + DATA_WIDTH);pin++){
    pinMode(pin,INPUT);
  } 

   for(pin=(DATA_START + DATA_WIDTH);pin>DATA_START;pin--){
    val = (val << 1) + digitalRead(pin);
  } 
   digitalWrite(N_OE,HIGH);
  return val;
}

void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readAddress(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}
