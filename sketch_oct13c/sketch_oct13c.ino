#include "MedicineDispenser.h"


/////////////////////////////////////////////////////////////////////////////

#define Button_Up       2
#define Button_Down     3
#define Button_Last     4
#define Button_Enter    5
#define Button_Reset    6

ButtonManager manager(Button_Up, Button_Down, Button_Last, Button_Enter, Button_Reset);

void setup() {
  Serial.begin(9600);
  
  pinMode(Button_Up, INPUT);
  pinMode(Button_Down, INPUT);
  pinMode(Button_Last, INPUT);
  pinMode(Button_Enter, INPUT);
  pinMode(Button_Reset, INPUT);

  ButtonPressReleaseRecorder::SetButtonFunction(ButtonType::Up, [](int buttonType){
    if(ButtonPressReleaseRecorder::RunButtonOf(ButtonType::Up))
    {
      LCD_Manager::choice_index += 1;
      if(LCD_Manager::choice_index == CHOICE_COUNT)//Choice count ----------------------------------------------TODO()
        LCD_Manager::choice_index = 0;
      LCD_Manager::SetCursor();
    }
    return true;
    });

  ButtonPressReleaseRecorder::SetButtonFunction(ButtonType::Down, [](int buttonType){
    if(ButtonPressReleaseRecorder::RunButtonOf(ButtonType::Down))
    {
      if(LCD_Manager::choice_index == 0)//Choice count ----------------------------------------------TODO()
        LCD_Manager::choice_index = CHOICE_COUNT;
      LCD_Manager::choice_index -= 1;
      LCD_Manager::SetCursor();
    }
    return true;
    });

  ButtonPressReleaseRecorder::SetButtonFunction(ButtonType::Last, [](int buttonType){
    if(ButtonPressReleaseRecorder::RunButtonOf(ButtonType::Last))
    {
      LCD_Manager::AppendPage(LCD_Manager::choice_index);
    }
    return true;
    });

  ButtonPressReleaseRecorder::SetButtonFunction(ButtonType::Enter, [](int buttonType){
    if(ButtonPressReleaseRecorder::RunButtonOf(ButtonType::Enter))
    {
      LCD_Manager::Back();
    }
    return true;
    });

  ButtonPressReleaseRecorder::SetButtonFunction(ButtonType::Reset, [](int buttonType){
    if(ButtonPressReleaseRecorder::RunButtonOf(ButtonType::Reset))
    {
      LCD_Manager::Call();
    }
    return true;
    });

  manager.Init();
}



void loop() {

  manager.onEvent();
  
}
