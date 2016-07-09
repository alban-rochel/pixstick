#define PixStickPinUp 22
#define PixStickPinDown 24
#define PixStickPinLeft 26
#define PixStickPinRight 28
#define PixStickPin1 30
#define PixStickPin2 32
#define PixStickPin3 34
#define PixStickPin4 36
#define PixStickPin5 38
#define PixStickPin6 40
#define PixStickPin7 42
#define PixStickPin8 44
#define PixStickPinStart 46
#define PixStickPinSelect 48

#define sd_cs  7

#include "PadConfiguration.h"
#include "InputHandler.h"

PadConfiguration* pads[50];
uint32_t padCount;
uint32_t currentPad;

void readConfigFile(const String& fileName)
{
  Serial.println("[readConfigFile start]");
  Serial.println(fileName);
  PadConfiguration* pad = new PadConfiguration(fileName.c_str());
  if(pad->_valid)
  {
    Serial.println("Pad is valid");
    pads[padCount] = pad;
    ++padCount;
  }
  else
  {
    Serial.println("Pad is invalid");
    delete pad;
  }
  Serial.println("[readConfigFile done]");
}

void readConfig()
{
  Serial.println("[readConfig start]");
  File myFile;
  myFile = SD.open("config.txt");


  while(myFile.available())
  {
    String configFile = readLine(myFile);
    if(configFile != "")
    {
       readConfigFile(configFile);
    }

  }

  myFile.close();
  Serial.println("[readConfig done]");
}

void normalSetup()
{
  // Read config

  readConfig();

  // Init pad

  //myPad = new PadController;
  myPad.init();
  
  // Init keyboard

  //myKeyboard = new KeyboardController;
  myKeyboard.begin();
}

/*void dualShock3Setup()
{
  readConfigFile("ps3.cfg");
  
  //myDualShock3 = new DualShock3Controller;
  myDualShock3.init();
}
*/
void setup() {
  
  Serial.begin(9600);

  // Init vars

  padCount = 0;
  currentPad = 0;
  
  // Init Stick pins

  pinMode(PixStickPinUp, INPUT_PULLUP);
  pinMode(PixStickPinDown, INPUT_PULLUP);
  pinMode(PixStickPinLeft, INPUT_PULLUP);
  pinMode(PixStickPinRight, INPUT_PULLUP);
  pinMode(PixStickPin1, INPUT_PULLUP);
  pinMode(PixStickPin2, INPUT_PULLUP);
  pinMode(PixStickPin3, INPUT_PULLUP);
  pinMode(PixStickPin4, INPUT_PULLUP);
  pinMode(PixStickPin5, INPUT_PULLUP);
  pinMode(PixStickPin6, INPUT_PULLUP);
  pinMode(PixStickPin7, INPUT_PULLUP);
  pinMode(PixStickPin8, INPUT_PULLUP);
  pinMode(PixStickPinStart, INPUT_PULLUP);
  pinMode(PixStickPinSelect, INPUT_PULLUP);

  // Init TFT pins

  // Init SD pins

  if (!SD.begin(sd_cs))
  {
    Serial.println(F("failed!"));
  }

  // Read config

  readConfig();

  // Init pad

  myKeyboard.begin();
  myPad.init();
  //myDualShock3.init();


  // Load first config

  pads[currentPad]->drawPicture();
  //pads[currentPad]->drawMapping();
}

void selectLoop()
{
  bool mappingMode(false);
  
  while(true)
  {
    bool isSelect = (digitalRead(PixStickPinSelect) == LOW);

    if(!isSelect)
    {
      pads[currentPad]->releaseAll();
      return;
    }

    bool changed = false;
    
    if(digitalRead(PixStickPinLeft) == LOW)
    {
      if(currentPad == 0)
      {
        currentPad = padCount - 1;
      }
      else
      {
        --currentPad;
      }
      changed = true;
      mappingMode = false;
    }

    if(digitalRead(PixStickPinRight) == LOW)
    {
      if(currentPad == padCount - 1)
      {
        currentPad = 0;
      }
      else
      {
        ++currentPad;
      }
      changed = true;
      mappingMode = false;
    }

    if(digitalRead(PixStickPinUp) == LOW || digitalRead(PixStickPinDown) == LOW)
    {
      mappingMode = !mappingMode;
      changed = true;
    }

    if(changed)
    {
      if(mappingMode)
      {
        pads[currentPad]->drawMapping();
      }
      else
      {
        pads[currentPad]->drawPicture();
      }
      delay(500);
    }

    pads[currentPad]->setIsPressed(ALT_BUTTON_1_INDEX, digitalRead(PixStickPin1) == LOW);
    pads[currentPad]->setIsPressed(ALT_BUTTON_2_INDEX, digitalRead(PixStickPin2) == LOW);
    pads[currentPad]->setIsPressed(ALT_BUTTON_3_INDEX, digitalRead(PixStickPin3) == LOW);
    pads[currentPad]->setIsPressed(ALT_BUTTON_4_INDEX, digitalRead(PixStickPin4) == LOW);
  
    pads[currentPad]->setIsPressed(ALT_BUTTON_5_INDEX, digitalRead(PixStickPin5) == LOW);
    pads[currentPad]->setIsPressed(ALT_BUTTON_6_INDEX, digitalRead(PixStickPin6) == LOW);
    pads[currentPad]->setIsPressed(ALT_BUTTON_7_INDEX, digitalRead(PixStickPin7) == LOW);
    pads[currentPad]->setIsPressed(ALT_BUTTON_8_INDEX, digitalRead(PixStickPin8) == LOW);
  
    pads[currentPad]->setIsPressed(ALT_BUTTON_START_INDEX, digitalRead(PixStickPinStart) == LOW);
  }
}

void loop()
{
  bool isSelect = (digitalRead(PixStickPinSelect) == LOW);

  if(isSelect)
  {
    pads[currentPad]->releaseAll();
    selectLoop();
  }

  pads[currentPad]->setIsPressed(STICK_UP_INDEX, digitalRead(PixStickPinUp) == LOW);
  pads[currentPad]->setIsPressed(STICK_DOWN_INDEX, digitalRead(PixStickPinDown) == LOW);
  pads[currentPad]->setIsPressed(STICK_LEFT_INDEX, digitalRead(PixStickPinLeft) == LOW);
  pads[currentPad]->setIsPressed(STICK_RIGHT_INDEX, digitalRead(PixStickPinRight) == LOW);

  pads[currentPad]->setIsPressed(BUTTON_1_INDEX, digitalRead(PixStickPin1) == LOW);
  pads[currentPad]->setIsPressed(BUTTON_2_INDEX, digitalRead(PixStickPin2) == LOW);
  pads[currentPad]->setIsPressed(BUTTON_3_INDEX, digitalRead(PixStickPin3) == LOW);
  pads[currentPad]->setIsPressed(BUTTON_4_INDEX, digitalRead(PixStickPin4) == LOW);

  pads[currentPad]->setIsPressed(BUTTON_5_INDEX, digitalRead(PixStickPin5) == LOW);
  pads[currentPad]->setIsPressed(BUTTON_6_INDEX, digitalRead(PixStickPin6) == LOW);
  pads[currentPad]->setIsPressed(BUTTON_7_INDEX, digitalRead(PixStickPin7) == LOW);
  pads[currentPad]->setIsPressed(BUTTON_8_INDEX, digitalRead(PixStickPin8) == LOW);

  pads[currentPad]->setIsPressed(BUTTON_START_INDEX, digitalRead(PixStickPinStart) == LOW);

  delay(4);
}
