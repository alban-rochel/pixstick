#ifndef PAD_CONFIGURATION_H
#define PAD_CONFIGURATION_H

#include <SD.h>
#include "InputHandler.h"

String readLine(File& file);

#define STICK_UP_INDEX 0
#define STICK_DOWN_INDEX 1
#define STICK_LEFT_INDEX 2
#define STICK_RIGHT_INDEX 3
#define BUTTON_1_INDEX 4
#define BUTTON_2_INDEX 5
#define BUTTON_3_INDEX 6
#define BUTTON_4_INDEX 7
#define BUTTON_5_INDEX 8
#define BUTTON_6_INDEX 9
#define BUTTON_7_INDEX 10
#define BUTTON_8_INDEX 11
#define BUTTON_START_INDEX 12
#define ALT_BUTTON_1_INDEX 13
#define ALT_BUTTON_2_INDEX 14
#define ALT_BUTTON_3_INDEX 15
#define ALT_BUTTON_4_INDEX 16
#define ALT_BUTTON_5_INDEX 17
#define ALT_BUTTON_6_INDEX 18
#define ALT_BUTTON_7_INDEX 19
#define ALT_BUTTON_8_INDEX 20
#define ALT_BUTTON_START_INDEX 21
#define BUTTON_ARRAY_SIZE 22

class PadConfiguration
{
  public:
    PadConfiguration(const char* fileName);

    ~PadConfiguration();

    void setIsPressed(uint8_t index, bool isPressed);
    void releaseAll();

    bool _valid;

    void drawPicture();
    void drawMapping();

  protected:

    void defaultConfiguration();

    InputHandler* _mapping[BUTTON_ARRAY_SIZE];
    uint8_t* _picture;
};

#endif // PAD_CONFIGURATION_H
