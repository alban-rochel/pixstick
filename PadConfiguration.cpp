#include "PadConfiguration.h"

#define lcd_cs 10
#define dc     9
#define rst    8

#include <SPI.h>
#include <TFT.h>

TFT TFTscreen = TFT(lcd_cs, dc, rst);
bool TFTInit = false;

String readLine(File& file)
{
  String res;

  while(file.available())
  {
    char current = (char)file.read();
    if(current == '\n')
    {
      return res;
    }

    res += current;
  }

  return res;
}

PadConfiguration::PadConfiguration(const char* fileName):
  _valid(false),
  _picture(nullptr)
{
Serial.println("[PadConfiguration start]");
  if(!TFTInit)
  {
    TFTscreen.begin();
    TFTInit = true;
  }
  for(uint8_t index; index < BUTTON_ARRAY_SIZE; ++index)
  {
    _mapping[index] = nullptr;
  }

  File myFile;
  myFile = SD.open(fileName);

  if(!myFile)
  {
    Serial.println("[PadConfiguration done: could not open file]");
    return;
  }

  String padConf = readLine(myFile);

  Serial.println(padConf.c_str());

  Serial.println(padConf.length());

  if(padConf.length() != BUTTON_ARRAY_SIZE*2+1)
  {
    Serial.println("[PadConfiguration done: parse error length]");
    Serial.println(padConf.length());
    myFile.close();
    return;
  }

  for(uint8_t index = 0; index < BUTTON_ARRAY_SIZE; ++index)
  {
    switch(padConf.charAt(index*2))
    {
      case 'k':
      {
        _mapping[index] = new InputHandlerKeyboard(padConf.charAt(index*2+1), false, false);
        if(!_mapping[index]->_valid)
        {
          Serial.println("[PadConfiguration done: parse error k]");
          myFile.close(); return;
        }
        break;
      }
      case 'K':
      {
        _mapping[index] = new InputHandlerKeyboard(padConf.charAt(index*2+1), true, false);
        if(!_mapping[index]->_valid)
        {
          Serial.println("[PadConfiguration done: parse error K]");
          myFile.close(); return;
        }
        break;
      }
      case 'F':
      {
        _mapping[index] = new InputHandlerKeyboard(padConf.charAt(index*2+1), false, true);
        if(!_mapping[index]->_valid)
        {
          Serial.println("[PadConfiguration done: parse error F]");
          myFile.close(); return;
        }
        break;
      }
      case 'P':
      {
        _mapping[index] = new InputHandlerPad(padConf.charAt(index*2+1));
        if(!_mapping[index]->_valid)
        {
          Serial.println("[PadConfiguration done: parse error P]");
          myFile.close(); return;
        }
        break;
      }
      case 'X':
      /*{
        _mapping[index] = new InputHandlerDualShock3(padConf.charAt(index*2+1));
        if(!_mapping[index]->_valid)
        {
          Serial.println("[PadConfiguration done: parse error X]");
          myFile.close(); return;
        }
        break;
      }*/
      case '-':
      {
        _mapping[index] = nullptr;
        break;
      }
      default:
      {myFile.close(); Serial.println("[PadConfiguration done: parse error]"); return;}
    }
  }

  String imageFilePath = readLine(myFile);

  myFile.close();

  myFile = SD.open(imageFilePath);
  if(myFile)
  {
    uint32_t len = myFile.size();

    _picture = new uint8_t[len];

    myFile.read(_picture, len);
    
    myFile.close();
    Serial.println(imageFilePath);
    Serial.println("Image read");
  }
  
  Serial.println(fileName);
  Serial.println("Valid");
  
  _valid = true;
  Serial.println("[PadConfiguration done]");
}

PadConfiguration::~PadConfiguration()
{
  for(uint8_t index; index < BUTTON_ARRAY_SIZE; ++index)
  {
    if(_mapping[index] != nullptr)
    {
      delete _mapping[index];
    }
  }

  if(_picture != nullptr)
  {
    delete _picture;
  }
}

void PadConfiguration::setIsPressed(uint8_t index, bool isPressed)
{
  if(_mapping[index])
  {
    _mapping[index]->setIsPressed(isPressed);
  }
}

void PadConfiguration::releaseAll()
{
  for(uint32_t index = 0; index < BUTTON_ARRAY_SIZE; ++index)
  {
    if(_mapping[index])
    {
      _mapping[index]->setIsPressed(false);
    }
  }
}

void PadConfiguration::drawPicture()
{
  if(_picture == nullptr)
  {
    drawMapping();
    return;
  }

          uint8_t* pointer = _picture;
        uint16_t current(0);
#define READ_8 current = *pointer; ++pointer;
#define READ_16 current = *((uint16_t*)pointer); ++pointer; ++pointer;

        READ_16;
        TFTscreen.fillScreen(current);

        READ_8;
        uint16_t numColors(current);
        for(uint16_t currentColor = 0; currentColor < numColors; ++currentColor)
        {
            READ_16;
            uint16_t color = current;

            READ_16;
            uint32_t numRects(current);
            for(uint16_t currentRect = 0; currentRect < numRects; ++currentRect)
            {
                READ_8;
                int16_t x(current);
                READ_8;
                int16_t y(current);
                READ_8;
                int16_t w(current);
                READ_8;
                int16_t h(current);
                TFTscreen.fillRect(x, y, w, h, color);
            }

            READ_16;
            uint16_t numVLines(current);
            for(uint16_t currentVLine = 0; currentVLine < numVLines; ++currentVLine)
            {
                READ_8;
                int16_t x(current);
                READ_8;
                int16_t y(current);
                READ_8;
                int16_t l(current);

                TFTscreen.drawFastVLine(x, y, l, color);
            }

            READ_16;
            uint16_t numHLines(current);
            for(uint16_t currentHLine = 0; currentHLine < numHLines; ++currentHLine)
            {
                READ_8;
                int16_t x(current);
                READ_8;
                int16_t y(current);
                READ_8;
                int16_t l(current);

                TFTscreen.drawFastHLine(x, y, l, color);
            }

            READ_16;
            uint16_t numPix(current);
            for(uint16_t currentPix = 0; currentPix < numPix; ++currentPix)
            {
                READ_8;
                int16_t x(current);
                READ_8;
                int16_t y(current);

                TFTscreen.drawPixel(x, y, color);
            }
        }

#undef READ_8
#undef READ_16
}

void drawText(int16_t x, int16_t y, const String& str, uint16_t color)
{
  int newX = x - str.length() * 5 / 2;
  for(uint32_t ii = 0; ii < str.length(); ++ii)
  {
    TFTscreen.drawChar(newX + ii * 5, y - 4, str[ii], color, 0x0, 1);
  }
}

void PadConfiguration::drawMapping()
{
  TFTscreen.fillScreen(0);

  // Draw grid

  /*TFTscreen.drawFastHLine(0, 78, 160, 0xFFFF);
  TFTscreen.drawFastHLine(0, 103, 160, 0xFFFF);
  TFTscreen.drawFastVLine(40, 78, 50, 0xFFFF);
  TFTscreen.drawFastVLine(80, 78, 50, 0xFFFF);
  TFTscreen.drawFastVLine(120, 78, 50, 0xFFFF);*/

#define BUTTON_RADIUS 10
#define BUTTON1_X 60
#define BUTTON1_Y 90
#define BUTTON2_X (BUTTON1_X + 2 * BUTTON_RADIUS + 4)
#define BUTTON2_Y (BUTTON1_Y - BUTTON_RADIUS)
#define BUTTON3_X (BUTTON2_X + 2 * BUTTON_RADIUS + 4)
#define BUTTON3_Y (BUTTON2_Y - 0.5 * BUTTON_RADIUS)
#define BUTTON4_X (BUTTON3_X + 2 * BUTTON_RADIUS + 4)
#define BUTTON4_Y BUTTON3_Y
#define BUTTON5_X BUTTON1_X
#define BUTTON5_Y (BUTTON1_Y - 2 * BUTTON_RADIUS - 4)
#define BUTTON6_X BUTTON2_X
#define BUTTON6_Y (BUTTON2_Y - 2 * BUTTON_RADIUS - 4)
#define BUTTON7_X BUTTON3_X
#define BUTTON7_Y (BUTTON3_Y - 2 * BUTTON_RADIUS - 4)
#define BUTTON8_X BUTTON4_X
#define BUTTON8_Y (BUTTON4_Y - 2 * BUTTON_RADIUS - 4)
#define BUTTONSTART_X (BUTTON1_X - 3 * BUTTON_RADIUS)
#define BUTTONSTART_Y (BUTTON1_Y + 2 * BUTTON_RADIUS)
#define Y_OFFSET (-5)
#define ALT_Y_OFFSET 5

#define GET_STRING(i) (_mapping[i] ? _mapping[i]->mappingString() : String("-"))
#define GET_COLOR(i) (_mapping[i] ? _mapping[i]->mappingColor() : 0b1000010000010000)

  TFTscreen.drawCircle(BUTTON1_X, BUTTON1_Y, BUTTON_RADIUS, 0xFFFF);
  drawText(BUTTON1_X, BUTTON1_Y + Y_OFFSET, GET_STRING(BUTTON_1_INDEX), GET_COLOR(BUTTON_1_INDEX));
  drawText(BUTTON1_X, BUTTON1_Y + ALT_Y_OFFSET, GET_STRING(ALT_BUTTON_1_INDEX), GET_COLOR(ALT_BUTTON_1_INDEX));
  
  TFTscreen.drawCircle(BUTTON2_X, BUTTON2_Y, BUTTON_RADIUS, 0xFFFF);
  drawText(BUTTON2_X, BUTTON2_Y + Y_OFFSET, GET_STRING(BUTTON_2_INDEX), GET_COLOR(BUTTON_2_INDEX));
  drawText(BUTTON2_X, BUTTON2_Y + ALT_Y_OFFSET, GET_STRING(ALT_BUTTON_2_INDEX), GET_COLOR(ALT_BUTTON_2_INDEX));
  
  TFTscreen.drawCircle(BUTTON3_X, BUTTON3_Y, BUTTON_RADIUS, 0xFFFF);
  drawText(BUTTON3_X, BUTTON3_Y + Y_OFFSET, GET_STRING(BUTTON_3_INDEX), GET_COLOR(BUTTON_3_INDEX));
  drawText(BUTTON3_X, BUTTON3_Y + ALT_Y_OFFSET, GET_STRING(ALT_BUTTON_3_INDEX), GET_COLOR(ALT_BUTTON_3_INDEX));
  
  TFTscreen.drawCircle(BUTTON4_X, BUTTON4_Y, BUTTON_RADIUS, 0xFFFF);
  drawText(BUTTON4_X, BUTTON4_Y + Y_OFFSET, GET_STRING(BUTTON_4_INDEX), GET_COLOR(BUTTON_4_INDEX));
  drawText(BUTTON4_X, BUTTON4_Y + ALT_Y_OFFSET, GET_STRING(ALT_BUTTON_4_INDEX), GET_COLOR(ALT_BUTTON_4_INDEX));
  
  TFTscreen.drawCircle(BUTTON5_X, BUTTON5_Y, BUTTON_RADIUS, 0xFFFF);
  drawText(BUTTON5_X, BUTTON5_Y + Y_OFFSET, GET_STRING(BUTTON_5_INDEX), GET_COLOR(BUTTON_5_INDEX));
  drawText(BUTTON5_X, BUTTON5_Y + ALT_Y_OFFSET, GET_STRING(ALT_BUTTON_5_INDEX), GET_COLOR(ALT_BUTTON_5_INDEX));
  
  TFTscreen.drawCircle(BUTTON6_X, BUTTON6_Y, BUTTON_RADIUS, 0xFFFF);
  drawText(BUTTON6_X, BUTTON6_Y + Y_OFFSET, GET_STRING(BUTTON_6_INDEX), GET_COLOR(BUTTON_6_INDEX));
  drawText(BUTTON6_X, BUTTON6_Y + ALT_Y_OFFSET, GET_STRING(ALT_BUTTON_6_INDEX), GET_COLOR(ALT_BUTTON_6_INDEX));
  
  TFTscreen.drawCircle(BUTTON7_X, BUTTON7_Y, BUTTON_RADIUS, 0xFFFF);
  drawText(BUTTON7_X, BUTTON7_Y + Y_OFFSET, GET_STRING(BUTTON_7_INDEX), GET_COLOR(BUTTON_7_INDEX));
  drawText(BUTTON7_X, BUTTON7_Y + ALT_Y_OFFSET, GET_STRING(ALT_BUTTON_7_INDEX), GET_COLOR(ALT_BUTTON_7_INDEX));
  
  TFTscreen.drawCircle(BUTTON8_X, BUTTON8_Y, BUTTON_RADIUS, 0xFFFF);
  drawText(BUTTON8_X, BUTTON8_Y + Y_OFFSET, GET_STRING(BUTTON_8_INDEX), GET_COLOR(BUTTON_8_INDEX));
  drawText(BUTTON8_X, BUTTON8_Y + ALT_Y_OFFSET, GET_STRING(ALT_BUTTON_8_INDEX), GET_COLOR(ALT_BUTTON_8_INDEX));
  
  TFTscreen.drawCircle(BUTTONSTART_X, BUTTONSTART_Y, BUTTON_RADIUS, 0xFFFF);
  drawText(BUTTONSTART_X, BUTTONSTART_Y + Y_OFFSET, GET_STRING(BUTTON_START_INDEX), GET_COLOR(BUTTON_START_INDEX));
  drawText(BUTTONSTART_X, BUTTONSTART_Y + ALT_Y_OFFSET, GET_STRING(ALT_BUTTON_START_INDEX), GET_COLOR(ALT_BUTTON_START_INDEX));

#define STICK_X 36
#define STICK_Y 36
#define STICK_WIDTH 6
#define STICK_LENGTH 12

  TFTscreen.drawRect(STICK_X - STICK_LENGTH,
                     STICK_Y - STICK_WIDTH/2,
                     2 * STICK_LENGTH,
                     STICK_WIDTH,
                     0xFFFF);

  TFTscreen.drawRect(STICK_X - STICK_WIDTH/2,
                     STICK_Y - STICK_LENGTH,
                     STICK_WIDTH,
                     2 * STICK_LENGTH,
                     0xFFFF);

  drawText(STICK_X, STICK_Y - STICK_LENGTH - 5, _mapping[0]->mappingString(), _mapping[0]->mappingColor());
  drawText(STICK_X, STICK_Y + STICK_LENGTH + 5, _mapping[1]->mappingString(), _mapping[1]->mappingColor());
  drawText(STICK_X - STICK_LENGTH - 10, STICK_Y, _mapping[2]->mappingString(), _mapping[2]->mappingColor());
  drawText(STICK_X + STICK_LENGTH + 10, STICK_Y, _mapping[3]->mappingString(), _mapping[3]->mappingColor());

}

