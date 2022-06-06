

#include <SPI.h>

//#include <EthernetUdp.h> //fake entry to steer compiler.
//#include <Ethernet.h> //Wiznet Ethernet library, replace the main ethernet library in the arduino install directory, https://github.com/WIZnet-ArduinoEthernet/Ethernet
//#include <Modbus.h>   //Modbus   library, located in the local libraries, https://github.com/andresarmento/modbus-arduino 
//#include <ModbusIP.h> //ModbusIP library, located in the local libraries, https://github.com/andresarmento/modbus-arduino 

//#include <ModbusSerial.h>

//RE,DE     = GP14
//RO        = GP15
//DI (TX)   = GP4

#include <DigitLedDisplay.h> //Max7219 Display driver, located in the sketch directory. https://github.com/ozhantr/DigitLedDisplay  
#include <Wire.h>
//#include <SDL_Arduino_INA3221.h> //INA3221 current sensor driver, located in the sketch directory. https://github.com/semaf/INA3221-Arduino-Library
//#include <EEPROM24LC256_512.h>  //24CAT512 EEPROM library,  https://github.com/mikeneiderhauser/I2C-EEPROM-Arduino
//#include <SparkFun_External_EEPROM.h> //24CAT512 EEPROM library, https://github.com/sparkfun/SparkFun_External_EEPROM_Arduino_Library

//UART Serial2(4,5,0,0);

//Eeprom setup
//int addr1 = 0;
//int addr2 = 32;

//ExternalEEPROM myMem;
int myRead2 =0;
int tempcnt = 0;
int cnt;

    //unsigned int randomNum=0;
    //define eeprom chip (1 per chip)
    //EEPROM256_512 mem_2;
    
    //arrays act as buffers for the EEPROM Chips
    //byte storage512[128];

    


/* W5100s-EVB-Pico to Display Pin
   11 to DIN,
   13 to CS,
   10 to CLK */
DigitLedDisplay lcd = DigitLedDisplay(11, 13, 10);

//SDL_Arduino_INA3221 ina3221_0x40 (0x40,1); //Set INA3221 with address 0x40 and shunt of 1 ohm.
//SDL_Arduino_INA3221 ina3221_0x41 (0x41,1); //Set INA3221 with address 0x41 and shunt of 1 ohm.

//INA3221_READ ina3221_0x40;
//INA3221_READ ina3221_0x41;

// Three channels of the INA3221 at address 0x40
#define mA3 1
#define mA2 2
#define mA4 3

// Three channels of the INA3221 at address 0x41
#define mA_SNS0 1
#define mA_SNS1 2
#define mA1 3

bool dir = 1; //increamentor direction in the loop.

// Read Coils (Output) Status 0xxxx
  boolean QCoil0 = true;
  boolean QCoil1 = true;
  boolean QCoil2 = true;
  boolean QCoil3 = true;
  boolean QCoil4 = true; 
  boolean QCoil5 = true;
  boolean QCoil6 = true;
  boolean QCoil7 = true;  //DOES NOT WORK 00108
  boolean QCoil8 = true;
  boolean QCoil9 = true;
  boolean QCoil10 = true;
  boolean QCoil11 = true;
  boolean QCoil12 = true;
  boolean QCoil13 = true;
  boolean QCoil14 = true;
  boolean QCoil15 = true;
  boolean QCoil16 = true;
  boolean QCoil17 = true;
  boolean QCoil18 = true;
  boolean QCoil19 = true;

//Read Input Status (Discrete Inputs) 1xxxx
  boolean ICoil0 = true;
  boolean ICoil1 = true;
  boolean ICoil2 = true;
  boolean ICoil3 = true;
  boolean ICoil4 = true; 
  boolean ICoil5 = true;
  boolean ICoil6 = true;
  boolean ICoil7 = true;
  boolean ICoil8 = true;
  boolean ICoil9 = true;



//30,000 registers
  int SetPoint0 = 1;
  int SetPoint1 = 1;
  int SetPoint2 = 1;
  int SetPoint3 = 1;
  int SetPoint4 = 1;
  int SetPoint5 = 1;
  int SetPoint6 = 1;
  int SetPoint7 = 1;
  int SetPoint8 = 1;
  int SetPoint9 = 1;

//coils
  const int SetPoint_CL0 = 100;   //00101 = 00,000 (input registers) + 100 (address) + 1 Modbus Offset
  const int SetPoint_CL1 = 101;   //00102
  const int SetPoint_CL2 = 102;   //00103
  const int SetPoint_CL3 = 103;   //00104
  const int SetPoint_CL4 = 104;   //00105
  const int SetPoint_CL5 = 105;   //00106
  const int SetPoint_CL6 = 106;   //00107
  const int SetPoint_CL7 = 107;   //00108  ////DOES NOT WORK 00108
  const int SetPoint_CL8 = 108;   //00109
  const int SetPoint_CL9 = 109;   //00110
  const int SetPoint_CL10 = 110;   //00111
  const int SetPoint_CL11 = 111;   //00112
  const int SetPoint_CL12 = 112;   //00113
  const int SetPoint_CL13 = 113;   //00114
  const int SetPoint_CL14 = 114;   //00115
  const int SetPoint_CL15 = 115;   //00116
  const int SetPoint_CL16 = 116;   //00117
  const int SetPoint_CL17 = 117;   //00118
  const int SetPoint_CL18 = 118;   //00119
  const int SetPoint_CL19 = 119;   //00120

//registers
  const int SetPoint_IR0 = 100;   //30101 = 30,000 (input registers) + 100 (address) + 1 Modbus Offset
  const int SetPoint_IR1 = 101;   //30102
  const int SetPoint_IR2 = 102;   //30103
  const int SetPoint_IR3 = 103;   //30104
  const int SetPoint_IR4 = 104;   //30105
  const int SetPoint_IR5 = 105;   //30106
  const int SetPoint_IR6 = 106;   //30107
  const int SetPoint_IR7 = 107;   //30108
  const int SetPoint_IR8 = 108;   //30109
  const int SetPoint_IR9 = 109;   //30110

// ModbusTCP object
ModbusIP mb;

// ModbusSerial object
ModbusSerial mbs;

void setup() {

Serial.begin(38400);


//TODO: define I2C pins.
//MBedI2C i2c(8,9);

//Wire.setSDA(8);  //SDA is pin GP08
//Wire.setSCL(9);  //SCL is Pin GP09
Wire.begin();

//---------eeprom---------------------------------------
/*
EEPROM.begin(512);

if(EEPROM.read(addr1) != 1)
  {
    EEPROM.write(addr1, 1);
    EEPROM.write(addr2,0);
    if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
       myRead2 = 0;
    } else {
      Serial.println("ERROR! EEPROM commit failed");
    }
  }
  else
  {
   myRead2 = EEPROM.read(addr2); 
  }

/*  if (myMem.begin() == false)
  {
    .println("No memory detected. Freezing.");
    while (1)
      ;
  }
  .println("Memory detected!");

  .print("Mem size in bytes: ");
//  .println(myMem.length());
  
//begin EEPROM with I2C Address 

//mem_2.begin(1,1);//addr 1 (DEC) type 1 (defined as 24LC512)
//mem_2.writeByte(randomNum, 0b10111101);

//print page size
 // .print("24LC512 Page Size: ");
 // .println(mem_2.getPageSize());

myMem.get(10, myRead2); //location to read, thing to put data into
  
*/
//---------eeprom---------------------------------------



  ina3221_0x40.begin();
  ina3221_0x41.begin();

    Serial.println("SDA_Arduino_INA3221_Test");
    Serial.println("Measuring voltage and current with ina3221 ...");
    //ina3221_0x40.begin();
    Serial.print("Manufactures ID=0x");
    int MID;
    MID = ina3221_0x40.getManufID();
    Serial.println(MID,HEX);

    Serial.println("SDA_Arduino_INA3221_Test");
    Serial.println("Measuring voltage and current with ina3221 ...");
    //ina3221_0x40.begin();
    Serial.print("Manufactures ID=0x");
    //int MID;
    MID = ina3221_0x41.getManufID();
    Serial.println(MID,HEX);

    pinMode(15, OUTPUT); //Local LED indicator
    digitalWrite(15, LOW);
    
    pinMode(28, INPUT); //555 Circuit running
    digitalWrite(28, LOW);
    
    //Local LED Display Pins
    pinMode(22, OUTPUT); //Local LED indicator
    digitalWrite(22, LOW);
    
    pinMode(26, OUTPUT); //Local LED indicator
    digitalWrite(26, LOW);
    
    pinMode(27, OUTPUT); //Local LED indicator
    digitalWrite(27, LOW);

    //Local SOV Pins
    pinMode(0, OUTPUT); //SOV 01
    digitalWrite(0, LOW);
    
    pinMode(1, OUTPUT); //SOV 02
    digitalWrite(1, LOW);
    
    pinMode(2, OUTPUT); //SOV 03
    digitalWrite(2, LOW);
    
    pinMode(3, OUTPUT); //SOV 04
    digitalWrite(3, LOW);  

    //SNS reter pins
    pinMode(6, OUTPUT); //SNS 0
    digitalWrite(6, LOW);
    
    pinMode(7, OUTPUT); //SNS 1
    digitalWrite(7, LOW);
    
  lcd.setBright(1);    // Max7219 Display Brightness   
  lcd.setDigitLimit(8);  // Max7219 Display digit count

// Config Modbus Serial (port, speed, byte format , TX Pin) 
  mbs.config(&Serial2, 38400, SERIAL_8N1,4);
// Set the Slave ID (1-247)
  mbs.setSlaveId(123); 
  
  Ethernet.init(17);  // w5100s-evb-pico SPI0 PIN
  Serial.begin(115200);

  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  byte ip[] = {192,168,1,123};
 // byte subnet[] = {255,255,255,0}; //using same network so not required

mb.config (mac, ip);  //  mb.config (mac, ip, subnet);

//add coils - 00,000
//mbs.addCoil(SetPoint_CL0);
//mbs.addCoil(SetPoint_CL1);
//mbs.addCoil(SetPoint_CL2);
//mbs.addCoil(SetPoint_CL3);
//mbs.addCoil(SetPoint_CL4);
//mbs.addCoil(SetPoint_CL5);
//mbs.addCoil(SetPoint_CL6);
//mbs.addCoil(SetPoint_CL7);
//mbs.addCoil(SetPoint_CL8);
//mbs.addCoil(SetPoint_CL9);
//mbs.addCoil(SetPoint_CL10);
//mbs.addCoil(SetPoint_CL11);
//mbs.addCoil(SetPoint_CL12);
//mbs.addCoil(SetPoint_CL13);
//mbs.addCoil(SetPoint_CL14);
//mbs.addCoil(SetPoint_CL15);
//mbs.addCoil(SetPoint_CL16);
//mbs.addCoil(SetPoint_CL17);
//mbs.addCoil(SetPoint_CL18);
//mbs.addCoil(SetPoint_CL19);


//add registers - 30,000
  mb.addIreg(SetPoint_IR0);
  mb.addIreg(SetPoint_IR1);
  mb.addIreg(SetPoint_IR2);
  mb.addIreg(SetPoint_IR3);
  mb.addIreg(SetPoint_IR4);
  mb.addIreg(SetPoint_IR5);
  mb.addIreg(SetPoint_IR6);
  mb.addIreg(SetPoint_IR7);
  mb.addIreg(SetPoint_IR8);
  mb.addIreg(SetPoint_IR9);


}


void loop() {
  // put your main code here, to run repeatedly:
   
  //----------------------Eeprom-----------------------------------

  
  
  /*if (myMem.begin() == false)
  {
    Serial.println("No memory detected. Freezing.");
    while (1)
      ;
  }
  Serial.println("Memory detected!");

  Serial.print("Mem size in bytes: ");
  Serial.println(myMem.length());

  Serial.print("myRead2 = ");
  Serial.println(myRead2);

  if(myRead2 == 0){
    //myMem.get(10, myRead2); //location to read, thing to put data into
    }
  //You should use gets and puts. This will automatically and correctly arrange
  //the bytes for larger variable types.
 /*
  int myValue2 = -366;
  myMem.put(10, myValue2); //(location, data)
  int myRead2;
  myMem.get(10, myRead2); //location to read, thing to put data into
  Serial.print("I read: ");
  Serial.println(myRead2);
  */

  //--------------------------------------------------------------



  // Read INA3221 curent transformer.
PrintIna();

  //modbusRTU
  mbs.task();

  //modbusTCP
  mb.task();

SetOutputsLow();


//add coils - 00,000
mb.Coil(SetPoint_CL0,QCoil0);
mb.Coil(SetPoint_CL1,QCoil1);
mb.Coil(SetPoint_CL2,QCoil1);
mb.Coil(SetPoint_CL3,QCoil3);
mb.Coil(SetPoint_CL4,QCoil4);
mb.Coil(SetPoint_CL5,QCoil5);
mb.Coil(SetPoint_CL6,QCoil6);
mb.Coil(SetPoint_CL7,QCoil7); //DOES NOT WORK 00108
mb.Coil(SetPoint_CL8,QCoil8);
mb.Coil(SetPoint_CL9,QCoil9);
mb.Coil(SetPoint_CL10,QCoil10);
mb.Coil(SetPoint_CL11,QCoil11);
mb.Coil(SetPoint_CL12,QCoil12);
mb.Coil(SetPoint_CL13,QCoil13);
mb.Coil(SetPoint_CL14,QCoil14);
mb.Coil(SetPoint_CL15,QCoil15);
mb.Coil(SetPoint_CL16,QCoil16);
mb.Coil(SetPoint_CL17,QCoil17);
mb.Coil(SetPoint_CL18,QCoil18);
mb.Coil(SetPoint_CL19,QCoil19);

//mbs.Coil(SetPoint_CL0,QCoil0);
//mbs.Coil(SetPoint_CL1,QCoil1);
//mbs.Coil(SetPoint_CL2,QCoil1);
//mbs.Coil(SetPoint_CL3,QCoil3);
//mbs.Coil(SetPoint_CL4,QCoil4);
//mbs.Coil(SetPoint_CL5,QCoil5);
//mbs.Coil(SetPoint_CL6,QCoil6);
//mbs.Coil(SetPoint_CL7,QCoil7); //DOES NOT WORK 00108
//mbs.Coil(SetPoint_CL8,QCoil8);
//mbs.Coil(SetPoint_CL9,QCoil9);
//mbs.Coil(SetPoint_CL10,QCoil10);
//mbs.Coil(SetPoint_CL11,QCoil11);
//mbs.Coil(SetPoint_CL12,QCoil12);
//mbs.Coil(SetPoint_CL13,QCoil13);
//mbs.Coil(SetPoint_CL14,QCoil14);
//mbs.Coil(SetPoint_CL15,QCoil15);
//mbs.Coil(SetPoint_CL16,QCoil16);
//mbs.Coil(SetPoint_CL17,QCoil17);
//mbs.Coil(SetPoint_CL18,QCoil18);
//mbs.Coil(SetPoint_CL19,QCoil19);


  
// registers - 30,000
  mb.Ireg(SetPoint_IR0, SetPoint0);
  mb.Ireg(SetPoint_IR1, SetPoint1);
  mb.Ireg(SetPoint_IR2, SetPoint2);
  mb.Ireg(SetPoint_IR3, SetPoint3);
  mb.Ireg(SetPoint_IR4, SetPoint4);
  mb.Ireg(SetPoint_IR5, SetPoint5);
  mb.Ireg(SetPoint_IR6, SetPoint6);
  mb.Ireg(SetPoint_IR7, SetPoint7); //DOES NOT WORK 00108
  mb.Ireg(SetPoint_IR8, SetPoint8);
  mb.Ireg(SetPoint_IR9, SetPoint9);
//  mb1.Ireg(SetPoint_IR1, SetPoint1);



  
// registers - 30,000
  mbs.Ireg(SetPoint_IR0, SetPoint0);
  mbs.Ireg(SetPoint_IR1, SetPoint1);
  mbs.Ireg(SetPoint_IR2, SetPoint2);
  mbs.Ireg(SetPoint_IR3, SetPoint3);
  mbs.Ireg(SetPoint_IR4, SetPoint4);
  mbs.Ireg(SetPoint_IR5, SetPoint5);
  mbs.Ireg(SetPoint_IR6, SetPoint6);
  mbs.Ireg(SetPoint_IR7, SetPoint7); //DOES NOT WORK 00108
  mbs.Ireg(SetPoint_IR8, SetPoint8);
  mbs.Ireg(SetPoint_IR9, SetPoint9);
//  mb1.Ireg(SetPoint_IR1, SetPoint1);

/*
    tempcnt++;
    cnt = myRead2 + tempcnt;
    if(tempcnt > 100){
    EEPROM.write(addr2, cnt);
          if (EEPROM.commit()) {
            Serial.println("EEPROM successfully committed");
            delay(5000);
          } else {
            Serial.println("ERROR! EEPROM commit failed");
            delay(5000);
          }
         //myMem.put(10, cnt); //(location, data) not working, able to get the size of the EEPROM
         //myMem.get(10, myRead2);
         myRead2 = cnt;
         tempcnt = 0;
      }
*/

  if(dir == 1){
       //lcd.printDigit(SetPoint0); //Max7219 Display Prints data to the display
       //lcd.printDigit(myRead2);
    
    locled(SetPoint0);  
    
    lcd.printDigit(SetPoint0);
    
    SetPoint0++;
    SetPoint1++;
    SetPoint2++;
    SetPoint3++;
    SetPoint4++;
    SetPoint5++;
    SetPoint6++;
    SetPoint7++;
    SetPoint8++;
    SetPoint9++;
    }
    else{
         //lcd.printDigit(SetPoint0); //Max7219 Display Prints data to the display
         //lcd.printDigit(myRead2); //Max7219 Display Prints data to the display
    
    //tempcnt++;
   /* if(tempcnt > 100){
         myMem.put(10, cnt); //(location, data) not working, able to get the size of the EEPROM
         myMem.get(10, myRead2);
         myRead2 = cnt;
         tempcnt = 0;
      }

     */ 

      // lcd.printDigit(SetPoint0);

       
             locled(SetPoint0); 
        
    
    
             
    SetPoint0--;
    SetPoint1--;
    SetPoint2--;
    SetPoint3--;
    SetPoint4--;
    SetPoint5--;
    SetPoint6--;
    SetPoint7--;
    SetPoint8--;
    SetPoint9--;
    }

  if(SetPoint0>32000){
    dir = 0;
    }
  if(SetPoint0<0){dir = 1;}

  if(SetPoint0%13==0 ||
  SetPoint0%13==1 ||
  SetPoint0%13==2 ||
  SetPoint0%13==3){


    //lcd.printDigit(0); //Max7219 Display Prints data to the display
    
    QCoil0 = false;
    QCoil1 = false;
    QCoil2 = false;
    QCoil3 = false;
    QCoil4 = false;
    QCoil5 = false;
    QCoil6 = false;
    QCoil7 = false; //DOES NOT WORK 00108
    QCoil8 = false;
    QCoil9 = false;
    QCoil10 = false;
    QCoil11 = false;
    QCoil12 = false;
    QCoil13 = false;
    QCoil14 = false;
    QCoil15 = false;
    QCoil16 = false;
    QCoil17 = false;
    QCoil18 = false;
    QCoil19 = false;

    //delay(2500);
  }
else{

 
      
    QCoil0 = true;
    QCoil1 = true;
    QCoil2 = true;
    QCoil3 = true;
    QCoil4 = true;
    QCoil5 = true;
    QCoil6 = true;
    QCoil7 = true; //DOES NOT WORK 00108
    QCoil8 = true;
    QCoil9 = true;
    QCoil10 = true;
    QCoil11 = true;
    QCoil12 = true;
    QCoil13 = true;
    QCoil14 = true;
    QCoil15 = true;
    QCoil16 = true;
    QCoil17 = true;
    QCoil18 = true;
    QCoil19 = true;
    }

  DisplaySetPointValues();



// lcd.printDigit(12345678); //Max7219 Display Prints data to the display
  
  delay(250);

  lcd.clear();
}









void DisplaySetPointValues(){
  String Printstr;
  Printstr = "Setpoint Value = " + String(SetPoint0) + " SetPoint_IR = 30" + String(SetPoint_IR0 + 1);
  Serial.println(Printstr);
}

void locled(int val){

  //local LED Display
    if(val%5 == 0 && val == 0){  
         digitalWrite(22, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
    else{     
        digitalWrite(22, LOW);
        }
    if(val%6 == 0 && val == 0){  
         digitalWrite(26, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
    else{     
        digitalWrite(26, LOW);
        }
    if(val%7 == 0 && val == 0){  
         digitalWrite(27, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
    else{     
        digitalWrite(27, LOW);
        }

    //SOV 01
    if(val%23 == 0 && val == 0){  
         digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
    else{     
        digitalWrite(0, LOW);
        }

    //SOV 02
    if(val%17 == 0 && val == 0){  
         digitalWrite(1, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
    else{     
        digitalWrite(1, LOW);
        }

    //SOV 03
    if(val%19 == 0 && val == 0){  
         digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
    else{     
        digitalWrite(2, LOW);
        }

    //SOV 04
    if(val%27 == 0 && val == 0){  
         digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
    else{     
        digitalWrite(3, LOW);
        }

    //SNS0 RST
    if(val%6 != 0 ){  
         digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
         digitalWrite(15, HIGH);  // trigger GP15
        }
    else{     
        digitalWrite(6, LOW);
        digitalWrite(15, LOW);
        }

        //SNS1 RST
    if(val%7 != 0 ){  
         digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
    else{     
        digitalWrite(7, LOW);
        } 

    if(digitalRead(28)==1)
        {
        Serial.println("GP28 High");    
        }
        else
        {
        Serial.println("GP28 LOW");  
        }
        
}


void SetOutputsLow()
{
    //pinMode(15, OUTPUT); //Local LED indicator
    digitalWrite(15, LOW);
    
    //pinMode(28, INPUT); //555 Circuit running
    digitalWrite(28, LOW);
    
    //Local LED Display Pins
    //pinMode(22, OUTPUT); //Local LED indicator
    digitalWrite(22, LOW);
    
    //pinMode(26, OUTPUT); //Local LED indicator
    digitalWrite(26, LOW);
    
    //pinMode(27, OUTPUT); //Local LED indicator
    digitalWrite(27, LOW);

    //Local SOV Pins
    //pinMode(0, OUTPUT); //SOV 01
    digitalWrite(0, LOW);
    
    //pinMode(1, OUTPUT); //SOV 02
    digitalWrite(1, LOW);
    
    //pinMode(2, OUTPUT); //SOV 03
    digitalWrite(2, LOW);
    
    //pinMode(3, OUTPUT); //SOV 04
    digitalWrite(3, LOW);  

    //SNS reter pins
    //pinMode(6, OUTPUT); //SNS 0
    digitalWrite(6, LOW);
    
    //pinMode(7, OUTPUT); //SNS 1
    digitalWrite(7, LOW);  
}

void PrintIna()
{
  Serial.println("-----------------------INA3221_0x40------------------------------------");

  Serial.println("SDA_Arduino_INA3221_Test");
    Serial.println("Measuring voltage and current with ina3221 ...");
    //ina3221_0x40.begin();
    Serial.print("Manufactures ID=0x");
    int MID;
    MID = ina3221_0x40.getManufID();
    Serial.println(MID,HEX);

    MID = ina3221_0x41.getManufID();
    Serial.print("Manufactures ID=0x");
    Serial.println(MID,HEX);

  
 
  
  float shuntvoltage_mA_SNS0 = 0;
  float busvoltage_mA_SNS0 = 0;
  float current_mA_SNS0 = 0;
  float loadvoltage_mA_SNS0 = 0;

  busvoltage_mA_SNS0 = ina3221_0x40.getBusVoltage_V(mA_SNS0);
  shuntvoltage_mA_SNS0 = ina3221_0x40.getShuntVoltage_mV(mA_SNS0);
  current_mA_SNS0 = -ina3221_0x40.getCurrent_mA(mA_SNS0);  // minus is to get the "sense" right.   - means the battery is charging, + that it is discharging
  loadvoltage_mA_SNS0 = busvoltage_mA_SNS0 + (shuntvoltage_mA_SNS0 / 1000);
  
  Serial.print("mA_SNS0 Bus Voltage:   "); Serial.print(busvoltage_mA_SNS0); Serial.println(" V");
  Serial.print("mA_SNS0 Shunt Voltage: "); Serial.print(shuntvoltage_mA_SNS0); Serial.println(" mV");
  Serial.print("mA_SNS0 Load Voltage:  "); Serial.print(loadvoltage_mA_SNS0); Serial.println(" V");
  Serial.print("mA_SNS0 Current:       "); Serial.print(current_mA_SNS0); Serial.println(" mA");
  Serial.println("");

  float shuntvoltage_mA_SNS1 = 0;
  float busvoltage_mA_SNS1 = 0;
  float current_mA_SNS1 = 0;
  float loadvoltage_mA_SNS1 = 0;

  busvoltage_mA_SNS1 = ina3221_0x40.getBusVoltage_V(mA_SNS1);
  shuntvoltage_mA_SNS1 = ina3221_0x40.getShuntVoltage_mV(mA_SNS1);
  current_mA_SNS1 = -ina3221_0x40.getCurrent_mA(mA_SNS1);
  loadvoltage_mA_SNS1 = busvoltage_mA_SNS1 + (shuntvoltage_mA_SNS1 / 1000);
  
  Serial.print("mA_SNS1 Bus Voltage:   "); Serial.print(busvoltage_mA_SNS1); Serial.println(" V");
  Serial.print("mA_SNS1 Shunt Voltage: "); Serial.print(shuntvoltage_mA_SNS1); Serial.println(" mV");
  Serial.print("mA_SNS1 Load Voltage:  "); Serial.print(loadvoltage_mA_SNS1); Serial.println(" V");
  Serial.print("mA_SNS1 Current:       "); Serial.print(current_mA_SNS1); Serial.println(" mA");
  Serial.println("");

  float shuntvoltage_mA1 = 0;
  float busvoltage_mA1 = 0;
  float current_mA1 = 0;
  float loadvoltage_mA1 = 0;

  busvoltage_mA1 = ina3221_0x40.getBusVoltage_V(mA1);
  shuntvoltage_mA1 = ina3221_0x40.getShuntVoltage_mV(mA1);
  current_mA1 = ina3221_0x40.getCurrent_mA(mA1);
  loadvoltage_mA1 = busvoltage_mA1 + (shuntvoltage_mA1 / 1000);
  
  Serial.print("mA1 Bus Voltage:   "); Serial.print(busvoltage_mA1); Serial.println(" V");
  Serial.print("mA1 Shunt Voltage: "); Serial.print(shuntvoltage_mA1); Serial.println(" mV");
  Serial.print("mA1 Load Voltage:  "); Serial.print(loadvoltage_mA1); Serial.println(" V");
  Serial.print("mA1 Current:       "); Serial.print(current_mA1); Serial.println(" mA");
  Serial.println("");

   float shuntvoltage_mA3 = 0;
  float busvoltage_mA3 = 0;
  float current_mA3 = 0;
  float loadvoltage_mA3 = 0;

  busvoltage_mA3 = ina3221_0x41.getBusVoltage_V(mA3);
  shuntvoltage_mA3 = ina3221_0x41.getShuntVoltage_mV(mA3);
  current_mA3 = -ina3221_0x41.getCurrent_mA(mA3);  // minus is to get the "sense" right.   - means the battery is charging, + that it is discharging
  loadvoltage_mA3 = busvoltage_mA3 + (shuntvoltage_mA3 / 1000);
  
  Serial.print("mA3 Bus Voltage:   "); Serial.print(busvoltage_mA3); Serial.println(" V");
  Serial.print("mA3 Shunt Voltage: "); Serial.print(shuntvoltage_mA3); Serial.println(" mV");
  Serial.print("mA3 Load Voltage:  "); Serial.print(loadvoltage_mA3); Serial.println(" V");
  Serial.print("mA Current  :      "); Serial.print(current_mA3); Serial.println(" mA");
  Serial.println("");

  float shuntvoltage_mA2 = 0;
  float busvoltage_mA2 = 0;
  float current_mA2 = 0;
  float loadvoltage_mA2 = 0;

  busvoltage_mA2 = ina3221_0x41.getBusVoltage_V(mA2);
  shuntvoltage_mA2 = ina3221_0x41.getShuntVoltage_mV(mA2);
  current_mA2 = -ina3221_0x41.getCurrent_mA(mA2);
  loadvoltage_mA2 = busvoltage_mA2 + (shuntvoltage_mA2 / 1000);
  
  Serial.print("mA2 Bus Voltage:   "); Serial.print(busvoltage_mA2); Serial.println(" V");
  Serial.print("mA2 Shunt Voltage: "); Serial.print(shuntvoltage_mA2); Serial.println(" mV");
  Serial.print("mA2 Load Voltage:  "); Serial.print(loadvoltage_mA2); Serial.println(" V");
  Serial.print("mA2 Current:       "); Serial.print(current_mA2); Serial.println(" mA");
  Serial.println("");

  float shuntvoltage_mA4 = 0;
  float busvoltage_mA4 = 0;
  float current_mA4 = 0;
  float loadvoltage_mA4 = 0;

  busvoltage_mA4 = ina3221_0x41.getBusVoltage_V(mA1);
  shuntvoltage_mA4 = ina3221_0x41.getShuntVoltage_mV(mA1);
  current_mA4 = ina3221_0x41.getCurrent_mA(mA1);
  loadvoltage_mA4 = busvoltage_mA4 + (shuntvoltage_mA4 / 1000);
  
  Serial.print("mA4 Bus Voltage:   "); Serial.print(busvoltage_mA4); Serial.println(" V");
  Serial.print("mA4 Shunt Voltage: "); Serial.print(shuntvoltage_mA4); Serial.println(" mV");
  Serial.print("mA4 Load Voltage:  "); Serial.print(loadvoltage_mA4); Serial.println(" V");
  Serial.print("mA4 Current:       "); Serial.print(current_mA4); Serial.println(" mA");
  Serial.println("");


  
  Serial.println("------------------------------------------------------------");
}
       
