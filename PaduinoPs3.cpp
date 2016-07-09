#include "PaduinoPs3.h"
#include "HID.h"

//http://eleccelerator.com/wiki/index.php?title=DualShock_3
static const uint8_t _hidReportDescriptorPs3[] PROGMEM = {
  0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
  0x09, 0x04,        // Usage (Joystick)
  0xA1, 0x01,        // Collection (Physical)
  0xA1, 0x02,        //   Collection (Application)
  0x85, 0x01,        //     Report ID (1)
  0x75, 0x08,        //     Report Size (8)
  0x95, 0x01,        //     Report Count (1)
  0x15, 0x00,        //     Logical Minimum (0)
  0x26, 0xFF, 0x00,  //     Logical Maximum (255)
  0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
                     //     NOTE: reserved byte
  0x75, 0x01,        //     Report Size (1)
  0x95, 0x13,        //     Report Count (19)
  0x15, 0x00,        //     Logical Minimum (0)
  0x25, 0x01,        //     Logical Maximum (1)
  0x35, 0x00,        //     Physical Minimum (0)
  0x45, 0x01,        //     Physical Maximum (1)
  0x05, 0x09,        //     Usage Page (Button)
  0x19, 0x01,        //     Usage Minimum (0x01)
  0x29, 0x13,        //     Usage Maximum (0x13)
  0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x01,        //     Report Size (1)
  0x95, 0x0D,        //     Report Count (13)
  0x06, 0x00, 0xFF,  //     Usage Page (Vendor Defined 0xFF00)
  0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
                     //     NOTE: 32 bit integer, where 0:18 are buttons and 19:31 are reserved
  0x15, 0x00,        //     Logical Minimum (0)
  0x26, 0xFF, 0x00,  //     Logical Maximum (255)
  0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
  0x09, 0x01,        //     Usage (Pointer)
  0xA1, 0x00,        //     Collection (Undefined)
  0x75, 0x08,        //       Report Size (8)
  0x95, 0x04,        //       Report Count (4)
  0x35, 0x00,        //       Physical Minimum (0)
  0x46, 0xFF, 0x00,  //       Physical Maximum (255)
  0x09, 0x30,        //       Usage (X)
  0x09, 0x31,        //       Usage (Y)
  0x09, 0x32,        //       Usage (Z)
  0x09, 0x35,        //       Usage (Rz)
  0x81, 0x02,        //       Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
                     //       NOTE: four joysticks
  0xC0,              //     End Collection
  0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
  0x75, 0x08,        //     Report Size (8)
  0x95, 0x27,        //     Report Count (39)
  0x09, 0x01,        //     Usage (Pointer)
  0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x08,        //     Report Size (8)
  0x95, 0x30,        //     Report Count (48)
  0x09, 0x01,        //     Usage (Pointer)
  0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x75, 0x08,        //     Report Size (8)
  0x95, 0x30,        //     Report Count (48)
  0x09, 0x01,        //     Usage (Pointer)
  0xB1, 0x02,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0xC0,              //   End Collection
  0xA1, 0x02,        //   Collection (Application)
  0x85, 0x02,        //     Report ID (2)
  0x75, 0x08,        //     Report Size (8)
  0x95, 0x30,        //     Report Count (48)
  0x09, 0x01,        //     Usage (Pointer)
  0xB1, 0x02,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0xC0,              //   End Collection
  0xA1, 0x02,        //   Collection (Application)
  0x85, 0xEE,        //     Report ID (238)
  0x75, 0x08,        //     Report Size (8)
  0x95, 0x30,        //     Report Count (48)
  0x09, 0x01,        //     Usage (Pointer)
  0xB1, 0x02,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0xC0,              //   End Collection
  0xA1, 0x02,        //   Collection (Application)
  0x85, 0xEF,        //     Report ID (239)
  0x75, 0x08,        //     Report Size (8)
  0x95, 0x30,        //     Report Count (48)
  0x09, 0x01,        //     Usage (Pointer)
  0xB1, 0x02,        //     Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0xC0,              //   End Collection
  0xC0,              // End Collection
};

class DualShock3Controller::Private
{
  public:
  Private()
  {
    memset(previousReport, 0x0, 144);
    memset(report, 0x0, 144);
  }
  
  uint8_t report[144];

  void send(void)
  {
    if(memcmp(previousReport, report, 144))
    {
      HID().SendReport(1,report,/*144*/6);
      memcpy(previousReport, report, 144);
    }
  }

  private:
  uint8_t previousReport[144];
};

DualShock3Controller::DualShock3Controller():
  d(new Private)
{
  static HIDSubDescriptor node(_hidReportDescriptorPs3, sizeof(_hidReportDescriptorPs3));
  HID().AppendDescriptor(&node);
}

DualShock3Controller::~DualShock3Controller()
{
  releaseAll();
  delete d;
}

void DualShock3Controller::init(void)
{
  
}

void DualShock3Controller::press(DualShock3Controller::PadButton button)
{Serial.println("Press");
  switch(button)
  {
    
/*bit  7 6 5 4 3 2 1 0
byte 2  D Left  D Down  D Right D Up  Start R3  L3  Select
byte 3  Square  X Circle  Triangle  R1  L1  R2  L2
byte 4   ?   ?   ?   ?   ?   ?   ?  PS
byte 5   ?   ?   ?   ?   ?   ?   ?   ?
*/
    case PadDown:
    d->report[1] = (d->report[1] & 0b10111111) | 0b01000000;
    break;
    
    case PadUp:
    d->report[1] = (d->report[1] & 0b11101111) | 0b00010000;
    break;
    
    case PadLeft:
    d->report[1] = (d->report[1] & 0b01111111) | 0b10000000;
    break;
    
    case PadRight:
    d->report[1] = (d->report[1] & 0b11011111) | 0b00100000;
    break;

    case PadX:
    d->report[2] = (d->report[2] & 0b10111111) | 0b01000000;
    break;

    case PadCircle:
    d->report[2] = (d->report[2] & 0b11011111) | 0b00100000;
    break;
   
    case PadSquare:
    d->report[2] = (d->report[2] & 0b01111111) | 0b10000000;
    break;
    
    case PadTriangle:
    d->report[2] = (d->report[2] & 0b11101111) | 0b00010000;
    break;
    
    case PadStart:
    d->report[1] = (d->report[1] & 0b11110111) | 0b00001000;
    break;
    
    case PadSelect:
    d->report[1] = (d->report[1] & 0b11111110) | 0b00000001;
    break;
    
    case PadR1:
    d->report[2] = (d->report[2] & 0b11110111) | 0b00001000;
    break;
    
    case PadL1:
    d->report[2] = (d->report[2] & 0b11111011) | 0b00000100;
    break;
    
    case PadR2:
    d->report[2] = (d->report[2] & 0b11111101) | 0b00000010;
    break;
    
    case PadL2:
    d->report[2] = (d->report[2] & 0b11111110) | 0b00000001;
    break;
    
    case PadR3:
    d->report[1] = (d->report[1] & 0b11111011) | 0b00000100;
    break;
    
    case PadL3:
    d->report[1] = (d->report[1] & 0b11111101) | 0b00000010;
    break;

    default: break;
  }

  d->send();
}

void DualShock3Controller::release(DualShock3Controller::PadButton button)
{Serial.println("Release");
    switch(button)
  {
    case PadDown:
    d->report[1] = (d->report[1] & 0b10111111);
    break;
    
    case PadUp:
    d->report[1] = (d->report[1] & 0b11101111);
    break;
    
    case PadLeft:
    d->report[1] = (d->report[1] & 0b01111111);
    break;
    
    case PadRight:
    d->report[1] = (d->report[1] & 0b11011111);
    break;

    case PadX:
    d->report[2] = (d->report[2] & 0b10111111);
    break;

    case PadCircle:
    d->report[2] = (d->report[2] & 0b11011111);
    break;
   
    case PadSquare:
    d->report[2] = (d->report[2] & 0b01111111);
    break;
    
    case PadTriangle:
    d->report[2] = (d->report[2] & 0b11101111);
    break;
    
    case PadStart:
    d->report[1] = (d->report[1] & 0b11110111);
    break;
    
    case PadSelect:
    d->report[1] = (d->report[1] & 0b11111110);
    break;
    
    case PadR1:
    d->report[2] = (d->report[2] & 0b11110111);
    break;
    
    case PadL1:
    d->report[2] = (d->report[2] & 0b11111011);
    break;
    
    case PadR2:
    d->report[2] = (d->report[2] & 0b11111101);
    break;
    
    case PadL2:
    d->report[2] = (d->report[2] & 0b11111110);
    break;
    
    case PadR3:
    d->report[1] = (d->report[1] & 0b11111011);
    break;
    
    case PadL3:
    d->report[1] = (d->report[1] & 0b11111101);
    break;

    default: break;
  }

  d->send();
}

void DualShock3Controller::releaseAll(void)
{

}

//DualShock3Controller MyDualShock3Controller;








