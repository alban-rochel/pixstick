#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Paduino.h"
//#include "PaduinoPs3.h"
#include "KeyboardPix.h"

extern KeyboardController  myKeyboard;
extern PadController myPad;
//extern DualShock3Controller myDualShock3;

class InputHandler
{
  public:

  InputHandler();
  
  void setIsPressed(bool);

  String mappingString() const {return _mappingString;}
  uint16_t mappingColor() const {return _mappingColor;}

  bool _valid;

protected:

  virtual void commitStatus() = 0;

  int _lastChange; // for debouncing
  bool _isPressed;
  String _mappingString;
  uint16_t _mappingColor;
};

class InputHandlerKeyboard: public InputHandler
{
    public:
    InputHandlerKeyboard(char codedKey, bool specialKey, bool fKey);

  protected:
    virtual void commitStatus();
    uint8_t _key;
};

class InputHandlerPad: public InputHandler
{
  public:
    InputHandlerPad(char codedButton);

  protected:
    virtual void commitStatus();
    PadController::PadButton _button;
};

/*class InputHandlerDualShock3: public InputHandler
{
  public:
    InputHandlerDualShock3(char codedButton);

  protected:
    virtual void commitStatus();
    DualShock3Controller::PadButton _button;
};*/

#endif // INPUT_HANDLER_H
