#ifndef DualShock3Controller_H
#define DualShock3Controller_H

class DualShock3Controller
{
  public:

  enum PadButton
  {
    PadUp,
    PadDown,
    PadLeft,
    PadRight,
    PadX,
    PadCircle,
    PadSquare,
    PadTriangle,
    PadStart,
    PadSelect,
    PadR1,
    PadL1,
    PadR2,
    PadL2,
    PadR3,
    PadL3
  } ;
  
  DualShock3Controller(void);
  ~DualShock3Controller(void);

  void init(void);
  void press(PadButton button);
  void release(PadButton button);
  void releaseAll(void);

  private:
  class Private;
  Private* d;
};

//extern DualShock3Controller MyDualShock3Controller;

#endif
