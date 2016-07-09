#include "InputHandler.h"

KeyboardController  myKeyboard;
PadController myPad;
//DualShock3Controller myDualShock3;

#define DEBOUNCE_MS 5

InputHandler::InputHandler():
  _valid(false),
  _lastChange(0),
  _isPressed(false),
  _mappingString(""),
  _mappingColor(0xFFFF)
{}

void InputHandler::setIsPressed(bool isPressed)
{
  if(isPressed == _isPressed)
    return;

  int currentTime = millis();

  if(currentTime - _lastChange < DEBOUNCE_MS)
    return;

  _lastChange = currentTime;
  _isPressed = isPressed;
  commitStatus();
}

InputHandlerKeyboard::InputHandlerKeyboard(char codedKey, bool specialKey, bool fKey):
  InputHandler(),
  _key(KEY_RETURN)
{
  _valid = true;
  if(specialKey)
  {
    _mappingColor = 0b1111110000010000;
    switch(codedKey)
    {
      case 'l': _key = KEY_LEFT_CTRL; _mappingString = "LCt"; break;
      case 'L': _key = KEY_LEFT_SHIFT; _mappingString = "LSh"; break;
      case 'a': _key = KEY_LEFT_ALT; _mappingString = "LAl"; break;
      case 'r': _key = KEY_RIGHT_CTRL; _mappingString = "RCt"; break;
      case 'R': _key = KEY_RIGHT_SHIFT; _mappingString = "RSh"; break;
      case 'A': _key = KEY_RIGHT_ALT; _mappingString = "RAl"; break;
      case 'z': _key = KEY_UP_ARROW; _mappingString = "Up"; break;
      case 's': _key = KEY_DOWN_ARROW; _mappingString = "Dwn"; break;
      case 'q': _key = KEY_LEFT_ARROW; _mappingString = "Lft"; break;
      case 'd': _key = KEY_RIGHT_ARROW; _mappingString = "Rgt"; break;
      case 'b': _key = KEY_BACKSPACE; _mappingString = "BSp"; break;
      case 't': _key = KEY_TAB; _mappingString = "Tab"; break;
      case 'e': _key = KEY_RETURN; _mappingString = "Ent"; break;
      case 'X': _key = KEY_ESC; _mappingString = "Esc"; break;
      case 'D': _key = KEY_DELETE; _mappingString = "Del"; break;
      case 'P': _key = KEY_PAGE_UP; _mappingString = "PgU"; break;
      case 'p': _key = KEY_PAGE_DOWN; _mappingString = "PDn"; break;
      case '_': _key = ' '; _mappingString = "Spc"; break;
      case 'H': _key = KEY_HOME; _mappingString = "Hme"; break;
      case 'E': _key = KEY_END; _mappingString = "End"; break;
      default: _valid = false; break;
    }
  }
  else if(fKey)
  {
    _mappingColor = 0b1000010000011111;
    switch(codedKey)
    {
      case '1': _key = KEY_F1; _mappingString = "F1"; break;
      case '2': _key = KEY_F2; _mappingString = "F2"; break;
      case '3': _key = KEY_F3; _mappingString = "F3"; break;
      case '4': _key = KEY_F4; _mappingString = "F4"; break;
      case '5': _key = KEY_F5; _mappingString = "F5"; break;
      case '6': _key = KEY_F6; _mappingString = "F6"; break;
      case '7': _key = KEY_F7; _mappingString = "F7"; break;
      case '8': _key = KEY_F8; _mappingString = "F8"; break;
      case '9': _key = KEY_F9; _mappingString = "F9"; break;
      case 'A': _key = KEY_F10; _mappingString = "F10"; break;
      case 'B': _key = KEY_F11; _mappingString = "F11"; break;
      case 'C': _key = KEY_F12; _mappingString = "F12"; break;
      default: _valid = false; break;
    }
  }
  else
  {
    _mappingColor = 0b1111111111110000;
    _mappingString = "";
    _mappingString += (codedKey);
    _key=(uint8_t)codedKey;
  }
}

void InputHandlerKeyboard::commitStatus()
{
/*  if(!myKeyboard)
  {
    return;
  }*/
  
  if(_isPressed)
  {
    myKeyboard.press(_key);
  }
  else
  {
    myKeyboard.release(_key);
  }
}

InputHandlerPad::InputHandlerPad(char codedButton):
  InputHandler(),
  _button(PadController::Pad1)
{
  _valid = true;
  _mappingColor = 0xFFFF;
  switch(codedButton)
  {
    case 'U': _button = PadController::PadUp; _mappingString = "Up"; break;
    case 'D': _button = PadController::PadDown; _mappingString = "Dwn"; break;
    case 'L': _button = PadController::PadLeft; _mappingString = "Lft"; break;
    case 'R': _button = PadController::PadRight; _mappingString = "Rgt"; break;
    case '1': _button = PadController::Pad1; _mappingString = "1"; break;
    case '2': _button = PadController::Pad2; _mappingString = "2"; break;
    case '3': _button = PadController::Pad3; _mappingString = "3"; break;
    case '4': _button = PadController::Pad4; _mappingString = "4"; break;
    case '5': _button = PadController::Pad5; _mappingString = "5"; break;
    case '6': _button = PadController::Pad6; _mappingString = "6"; break;
    case '7': _button = PadController::Pad7; _mappingString = "7"; break;
    case '8': _button = PadController::Pad8; _mappingString = "8"; break;
    case '9': _button = PadController::Pad9; _mappingString = "9"; break;
    case 'a': _button = PadController::Pad10; _mappingString = "10"; break;
    case 'b': _button = PadController::Pad11; _mappingString = "11"; break;
    case 'c': _button = PadController::Pad12; _mappingString = "12"; break;
    case 'd': _button = PadController::Pad13; _mappingString = "13"; break;
    case 'e': _button = PadController::Pad14; _mappingString = "14"; break;
    case 'f': _button = PadController::Pad15; _mappingString = "15"; break;
    case 'g': _button = PadController::Pad16; _mappingString = "16"; break;
    case 'h': _button = PadController::Pad17; _mappingString = "17"; break;
    case 'i': _button = PadController::Pad18; _mappingString = "18"; break;
    default: _valid = false; break;
  }
}

void InputHandlerPad::commitStatus()
{
/*  if(!myPad)
  {
    return;
  }*/
  
  if(_isPressed)
  {
    myPad.press(_button);
  }
  else
  {
    myPad.release(_button);
  }
}

/*InputHandlerDualShock3::InputHandlerDualShock3(char codedButton):
  InputHandler(),
  _button(DualShock3Controller::PadX)
{
  _valid = true;
  _mappingColor = 0b1111111111110000;
  switch(codedButton)
  {
    case 'U': _button = DualShock3Controller::PadUp; _mappingString = "Up"; break;
    case 'D': _button = DualShock3Controller::PadDown; _mappingString = "Dwn"; break;
    case 'L': _button = DualShock3Controller::PadLeft; _mappingString = "Lft"; break;
    case 'R': _button = DualShock3Controller::PadRight; _mappingString = "Rgt"; break;
    case '1': _button = DualShock3Controller::PadX; _mappingString = "X"; break;
    case '2': _button = DualShock3Controller::PadCircle; _mappingString = "O"; break;
    case '3': _button = DualShock3Controller::PadSquare; _mappingString = "[]"; break;
    case '4': _button = DualShock3Controller::PadTriangle; _mappingString = "/\\"; break;
    case '5': _button = DualShock3Controller::PadR1; _mappingString = "R1"; break;
    case '6': _button = DualShock3Controller::PadL1; _mappingString = "L1"; break;
    case '7': _button = DualShock3Controller::PadR2; _mappingString = "R2"; break;
    case '8': _button = DualShock3Controller::PadL2; _mappingString = "L2"; break;
    case '9': _button = DualShock3Controller::PadR3; _mappingString = "R3"; break;
    case 'a': _button = DualShock3Controller::PadL3; _mappingString = "L3"; break;
    case 'S': _button = DualShock3Controller::PadStart; _mappingString = "St"; break;
    case 's': _button = DualShock3Controller::PadSelect; _mappingString = "Se"; break;
    default: _valid = false; break;
  }
}

void InputHandlerDualShock3::commitStatus()
{
  if(_isPressed)
  {
    myDualShock3.press(_button);
  }
  else
  {
    myDualShock3.release(_button);
  }
}*/
