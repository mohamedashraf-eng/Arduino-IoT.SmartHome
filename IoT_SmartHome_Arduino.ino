  //--->>>> DIGITAL & LOGIC CIRCUITS FINALE PROJECT <<<<--- @CR/This code Developed by AMC.@ SMART HOME DEMO - IOT - COMMUNICATION.
  //==== @Copy rights :- Eng Mohamed Ashraf <Coding:Circuits:Hardware> # Electrical Communication & Electronics Department - CIC
  //********************************************************************************************************************************************************************************************
  /************************ Sensors & Modules used_list **************************
                            ===========================
 ********************************************************************************************************
 * - 0  => * Microcontroller.        *   -> Arduino UNO Based-(ATMega328P@Microchip).         *   (Y)   *          
 * - 1  => * RFID Sensor.            *  -> For Security main door.                            *   (Y)   *
 * - 2  => * Temp Sensor.            *  -> For Safety inside house.                           *   (Y)   *
 * - 3  => * LDR Sensor.             *  -> Smart house auto light detector with action.       *   (Y)   *
 * - 4  => * Gas / Smoke Sensor.     *  -> For safety inside house.                           *   (Y)   *
 * - 5  => * Air Quality Sensor.     *  -> For safety inside house.                           *   (N)   *
 * - 6  => * Wireless Control.       *  -> Smart house manual control from the Owner (User).  *   (N)   *
 * - 7  => * Solar power.            *  -> Smart house Renewable Energy with control.         *   (N)   *
 * - 8  => * IR Sensor.              *  -> Smart auto-door.                                   *   (Y)   *
 * - 9  => * Sound Sensor.           *  -> Smart sound system.                                *   (Y)   *
 ********************************************************************************************************[electrical_components(interfacing)]*/ 
   bool GAK = true; // General Access Key (Global SHUTDOWN for all the system !) /- DANGEROUS -/
  //==== Libraries include ===>
  #include <SPI.h> // SPI Communication protocol header file.
  #include <MFRC522.h> // RFID header file to use the RFID built-in functions.
  
  #include <SoftwareSerial.h> // Serial communication build-in functions (UART).

  #include <Servo.h> // Servo motor library for easy use.
  //-----------------------
  // Defines ==>
  //--------- Temp & LDR & Gas Sensor Defines -----------
  #define LM35_V A5      // Analog pin to read data from tempreture sensor.
  #define LDR A4        // Analog pin to read data from light dependent resistor sensor.
  #define SMOKE_mq2 A3 // Analog pin to read data from gas sensor.
  //----- RFID Sensor-Conf Defines ----
  #define SS_PIN 10  // Set pin for module.
  #define RST_PIN 9 // Reset pin for module.
  #define Buzzer 5 // Buzzer indicator Access Suc/Den.
  #define Wx_ID "62 2C B1 1F"   //Hex Format for owner's ID.
//  #define N_ID  "77 B3 3F 27" // Hex format for another ID.
  //----------- IR Sensor-Conf Defines ----------
  #define addA 2 // Address A for multiplexer.
  #define addB 3 // Address B for multiplexer.
  #define addC 4 // Address C for multiplexer.
  #define AN A0  // Analog input pin interfacing.
  //------------- SOUND Sensor-Conf Defines ----------
  //#define SOUND_DDATA 3 //  Data INPUT from the Sound detector sensor.
  //-------------------------------------
  // Initializing ==>
    unsigned long T0 = 0;   // millis controlling interrupt. Timer 0 , Mask 0
    unsigned long T1 = 0;  // millis controlling interrput.  Timer 1 , Mask 1
    unsigned long T3 = 0; // millis controlling interruput.  Timer 2 , Mask 2
  

  //  byte SOUND_DATA_VALUE = 0; // the sound detection sensor input digital data.
  
    byte temp  = 0;    // the LM35_variable with 8-bit size.
    byte light = 0;  // the LDR_variable with 8-bit size.
    byte Smoke = 0; // the MQ-2_Variable with 8-bit size.
    
    byte IR1_DATA_IN; // Analog input data from IR sensor 1.
    byte IR2_DATA_IN; // Analog input data from IR sensor 2.
    byte IR3_DATA_IN; // Analog input data from IR sensor 3.
    byte IR4_DATA_IN; // Analog input data from IR sensor 3.
  
  //  byte FLAG_RX = 0; // Serial comm - UART data RX flag.
  //  char DATA_UART; // Reading data serial-comm UART_RX.
    
    bool FLAG_A    = false; // INIT Flag_Access_Authorized Data for RFID.
    bool FLAG_D    = false; // INIT Flag_Access_Denied Data for RFID.
    
    bool FLAG_LDR  = false; // INIT Flag_LDR Action.
    
    bool FLAG_LM35_U1 = false; // INIT Flag_LM35_UNIT1 Action.
  //bool FLAG_LM35_U2 = false; // INIT Flag_LM35_UNIT2 Action. ***Disabled.
  //bool FLAG_LM35_U3 = false; // INIT Flag_LM35_UNIT3 Action. ***Disabled.
    
    bool FLAG_MQ2  = false; // INIT Flag_MQ2 Action.
    MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
  //============================== Program F-Declarations ==>
  void Temp_Sensor();             // Global define for temp sensor.
  void LDR_Sensor();             //Global define for LDR sensor.
  void RFID_Sensor();           //Global define for rfid sensor.
  void SMOKE_Sensor();         // Global define for smoke sensor.
  void IR_CONF();             // Global define for IR Sensor.
 // void DATA_RX();          // Global define for UART RX Unit.
 // void SOUND_DETECTION(); // Global define for sound sensor.
  //======================= Main Program ==>
    void setup(){ Serial.begin(9600); // starting the serial monitor on baudrate :: 9600.
  //--RFID-Conf setup.
    SPI.begin();      // Initiate  SPI bus
    rfid.PCD_Init(); // Init MFRC522 
    //--------------- END RFID CONF.
    pinMode(Buzzer,OUTPUT);    // Digital Pin state for buzzer is OUTPUT.
    
    pinMode(LM35_V,INPUT);     // Analog Pin state for Temprature sensor is INPUT.
    pinMode(LDR,INPUT);       // Analog Pin state for Light Dependant Resistor is INPUT.
    pinMode(SMOKE_mq2,INPUT); // Analog Pin state for Smoke detector sensor is INPUT.
    
    pinMode(addA,OUTPUT); // Address A for multiplexer as output Digital.
    pinMode(addB,OUTPUT); // Address B for multiplexer as output Digital.
    pinMode(addC,OUTPUT); // Address C for multiplexer as output Digital.
    
    pinMode(IR1_DATA_IN,INPUT); // Digital Pin state for Infra red sensor is INPUT.
    pinMode(IR2_DATA_IN,INPUT); // Digital Pin state for Infra red sensor is INPUT.
    pinMode(IR3_DATA_IN,INPUT); // Digital Pin state for Infra red sensor is INPUT.
    pinMode(IR4_DATA_IN,INPUT); // Digital Pin state for Infra red sensor is INPUT.
   //pinMode(SOUND_DATA_VALUE,INPUT); // Digital Pin state for Sound detection sensor is INPUT.
  //----------------------------------------------------------------------
 //-- setup
    T0 = millis(); T1 = millis(); T3 = millis();} //end main void setup. & End with 3 time states for general using.
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    void loop(){ // Void loop start --------------<
     //-------------------------------------------
     while(GAK){ // Start General Access Key .
        if(millis() - T0 >= 100){
          Temp_Sensor();          // LM35 Sensor Function.
          LDR_Sensor();          // LDR Sensor Function.
          SMOKE_Sensor();       // SMOKE Sensor Function.
            T0 = millis();} // ==> end if1.
        if(millis() - T1 >= 50){
          RFID_Sensor();         // RFID Sensor sound Functions.
            T1 = millis();} // ==> end if2.
        if(millis() - T3 >= 15){
      //  DATA_RX();         // UART RX Function.
          IR1_CONF();         // IR1 Sensor Function.
          IR2_CONF();         // IR2 Sensor Function.
          IR3_CONF();         // IR3 Sensor Function.
          IR4_CONF();         // IR4 Sensor Function.
        //  SOUND_DETECTION();// SOUND Sensor Function.
            T3 = millis();}  // ==> end if3.
      }// END if - General Access Key.
    //---------------------------------------------
    } //end main void loop.
  /*
  *
  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/  
  //=====================> Functions <===================
   void RFID_Sensor(){// RFID action and Access.
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String UID  = "";
    for (byte i = 0;  i < rfid.uid.size;  i++)  {
       Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");// E8 07 C7 D2
       Serial.print(rfid.uid.uidByte[i], HEX);
       UID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ")); // 0E c2 12 
       UID.concat(String(rfid.uid.uidByte[i], HEX));} // end for loop.
       Serial.println(); Serial.print("Message : "); // printing the messege
    UID.toUpperCase();
    //change here the UID of the card/cards that you want to give access
    if      (UID.substring(1) == Wx_ID)      {Serial.println(" Mohasmed Wx :: 20196563 "); Serial.println(" Authorized access"); FLAG_A  = true; Access_Suc();} // Mohamed Ashraf 20196563
    else                                     {Serial.println(" Access denied"); FLAG_D = true; Access_Den();} // if none of these owners.  
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();} //Stop encryption on PCD & end if
   }//end RFID Sensor.
    //---------- RFID-Conf Functions ----------
    void Access_Suc(){ // if the flag(Right access == True(1)) will make this action(sound).
      if(FLAG_A){ //Action start.
    digitalWrite(Buzzer,HIGH); delay(50);
    digitalWrite(Buzzer,LOW);  delay(50);
    digitalWrite(Buzzer,HIGH); delay(50);
    digitalWrite(Buzzer,LOW);  delay(1000);
      FLAG_A  = false; // set the flag to 0 to reset it.
     } //end if.
    }//end Access Suc.
   void Access_Den(){ // Action start.
      if(FLAG_D){
    Serial.println(" Access denied");
    digitalWrite(Buzzer,HIGH);
    delay(500);
    digitalWrite(Buzzer,LOW);
    delay(800);
      FLAG_D = false;
     } // end if.
    }//end Access Den.
    //---------- Temp Sensor-Conf Fucntion ------------
   void Temp_Sensor(){
      temp = (analogRead(LM35_V) * 0.48828125); // converting the volt value to tempreture data.
      Serial.print("TEMP = ");
      Serial.print(temp); // printing the output tempreture.
      Serial.println(" C");
      if(temp >= 0  || temp <= 10){
        //Action
        }
      else if(temp > 10 || temp <= 25){
        //Action
        }
      else if(temp > 25 || temp <= 50){
        //Action
        }
      else {
        //Action
        }
   } //end Temp sensor.
   //----------- LDR Sensor-Conf Function -------------
   void LDR_Sensor(){
      light = analogRead(LDR); // reading the data coming from sensor.
      byte lvl = map(light,54,206,0,100); // mapping range for easy use. (0-100) intensety.
      Serial.print("Light intensity = "); Serial.print(lvl); Serial.println(" candela");
      if(lvl > 50){ FLAG_LDR = true; LDR_FLAG(); } // Starting Action.
   } // END LDR_Sensor.
   void LDR_FLAG(){
    if(FLAG_LDR){

      //Action.

      FLAG_LDR = false;}//END IF.
    }//END LDR_FLAG.
   //end LDR Sensor.
    //----------- Smoke Sensor-Conf Function -------------
   void SMOKE_Sensor(){
    Smoke = analogRead(SMOKE_mq2);
    Serial.print("Smoke sensor value = "); Serial.println(Smoke);
    delay(50); //delay for reading values.
    if(Smoke > 200 ){ FLAG_MQ2 = true; MQ2_FLAG(); } // Starting Action.
    } // END SMOKE_Sensor.
   void MQ2_FLAG(){
    if(FLAG_MQ2){

    //Action.
      
    FLAG_MQ2 = false;}//END IF.
    }//END MQ2_FLAG.
    //end Smoke Sensor.
    
    //----------- DATA Received UART -------------
   /*void DATA_RX(){  //---*** Disabled Wirless Controlling ***
    if(Serial.available() > 0){ //if the Serial comm flag goes high.
      DATA_UART = Serial.read(); // reading the data recieved. ->D<0:1:2:3>H<00:01:03>B<0000$0000:0000$0001:0000$0010:0000$0011><-
      Serial.print("Data received = "); Serial.println(DATA_UART);
      switch(DATA_UART){ // switching cases between the needed values (Actions from user - Manual Control).
//--------------------------------------------------------------------------------      
        case 'S':               //DATA_CONTROL_S_VALUE = Shutdown the system.
            GAK = false;
          break;
        case 'R':              //DATA_CONTROL_R_VALUE = Restore the system.
            GAK = true;
            //ActionSD.
          break;
 //--------------------------------------------------------------------------------
        case '0':               //DATA_CONTROL_1_VALUE =
            //Action0.
         break;
        case '1':             //DATA_CONTROL_2_VALUE = 
            //Action1.
         break;
        case '2':           //DATA_CONTROL_3_VALUE = 
            //Action2.
         break;
        case '3':         //DATA_CONTROL_4_VALUE = 
            //Action3.
         break;
        case '4':       //DATA_CONTROL_4_VALUE = 
            //Action4.

        default: break; //DEFAULT_VALUE.  
       }// end switch.
      }//end if.
        delay(50);} //end DATA RX. */
        
   //------------------- IR Sensor ---------------
   byte IR_READ(byte n){
    digitalWrite(addA,(n&1) ? HIGH:LOW);
    digitalWrite(addB,(n&2) ? HIGH:LOW);
    digitalWrite(addC,(n&4) ? HIGH:LOW);
    return analogRead(AN); } // Dnd Addressing Conditions Multiplexer.
    
   void IR1_CONF(){ // IR DETECT UNIT == 1.
    IR1_DATA_IN = IR_READ(0);
    if(IR1_DATA_IN < 200){ // if someone passed the door (interrupted the IR signal).    
      // Action.
      } // end if.
      else{
      // Action.
        } // end else.
   delay(30);}//end IR1 CONF.
   void IR2_CONF(){ // IR DETECT UNIT == 2.
    IR2_DATA_IN = IR_READ(1);
    if(IR2_DATA_IN < 200){ // if someone passed the door (interrupted the IR signal).    
       
      // Action.
      } // end if.
      else{
      // Action.
        } // end else.
   delay(30);}//end IR2 CONF.
   void IR3_CONF(){ // IR DETECT UNIT == 3.
    IR3_DATA_IN = IR_READ(2);
    if(IR3_DATA_IN < 200){ // if someone passed the door (interrupted the IR signal).    
       
      // Action.
      } // end if.
      else{
      // Action.
        } // end else.
   delay(30);}//end IR3 CONF.
   void IR4_CONF(){ // IR DETECT UNIT == 4.
    IR4_DATA_IN = IR_READ(3);
    if(IR4_DATA_IN < 200){ // if someone passed the door (interrupted the IR signal).    
       
      // Action.
      } // end if.
      else{
      // Action.
        } // end else.
   delay(30);}//end IR4 CONF.
   
   //--------------------- Sound Sensor ---------------------
   /*void SOUND_DETECTION(){ / *** Disabled Sound Detection ***
    SOUND_DATA_VALUE = digitalRead(SOUND_DDATA); // reading the digital data input from sensor.
    if(SOUND_DATA_VALUE == HIGH){ // if the sound value became high.
      
      // Action.
      
      }//end if.
    else {
      // Action.
      }//end else.
    delay(25);}//end SOUND DETECTION. */
    
   //-----------------------------------------------------------------------------------------------------------------------------------#END_CODE 
   /*- Best regards ♥  * Develobed by @cr-AMC Group * 
                        ****************************
***********************************************************************************************************************************************************************************************/
