#include "MedicineDispenser.h"


/////////////////////////////////////////////////////////////////////////////

#define Button_Up       2
#define Button_Down     3
#define Button_Last     4
#define Button_Enter    5
#define Button_Reset    6

#define Motor_1 7
#define Motor_2 8
#define Motor_3 9
#define Motor_4 10

ButtonManager manager(Button_Up, Button_Down, Button_Last, Button_Enter, Button_Reset);
MotorManager motorManager = MotorManager();

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
      if(LCD_Manager::IsInEditPage() && LCD_Page::GetDepth() == 6)//count code edit
      {
        unsigned int page_ = LCD_Page::GetThisPageIndex();
        LCD_Manager::RunInternalEditFunction(page_, 0);
      }
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
      if(LCD_Manager::IsInEditPage() && LCD_Page::GetDepth() == 6)
      {
        unsigned int page_ = LCD_Page::GetThisPageIndex();
        LCD_Manager::RunInternalEditFunction(page_, 1);
      }
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
      if(LCD_Page::GetDepth() == 8)
        return;
      LCD_Manager::Back();
    }
    return true;
    });

  ButtonPressReleaseRecorder::SetButtonFunction(ButtonType::Enter, [](int buttonType){
    if(ButtonPressReleaseRecorder::RunButtonOf(ButtonType::Enter))
    {
      if(LCD_Manager::IsInEditPage() && LCD_Page::GetDepth() == 5)
      {
        LCD_Manager::RunInternalEditFunction(2, LCD_Manager::choice_index);
        return true;
      }
      if(LCD_Manager::IsInEditPage() && LCD_Page::GetDepth() == 6)
      {
        LCD_Manager::Clear();
        LCD_Manager::FinalPage();
        return true;
      }
      if(LCD_Page::GetDepth() == 8)
      {
        switch(LCD_Manager::choice_index)
        {
          case 0: {
            DispenserConfiguration::BackToPastData();
            while(LCD_Page::GetThisPageIndex() == 0)
            {
              LCD_Manager::Back();
            }
          } break;
          case 1: {
            DispenserConfiguration::SavePastData();
            while(LCD_Page::GetThisPageIndex() == 0)
            {
              LCD_Manager::Back();
            }
          } break;
          case 2: {
            DispenserConfiguration::BackToPastData();
            while(LCD_Page::GetThisPageIndex() == 0)
            {
              LCD_Manager::Back();
            }
          } break;
          case 3: {
            DispenserConfiguration::SavePastData();
            while(LCD_Page::GetThisPageIndex() == 0)
            {
              LCD_Manager::Back();
            }
          } break;
          case 4: {
            DispenserConfiguration::BackToPastData();
            LCD_Manager::ReInit();
          } break;
        }
        return true;
      }
      LCD_Manager::AppendPage(LCD_Manager::choice_index);
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
  Time::Init();
  motorManager.MotorSet(Motor(Motor_1), Motor(Motor_2), Motor(Motor_3), Motor(Motor_4));
}



void loop() {

  manager.onEvent();
  motorManager.Run();
  Serial.print(hour());
  Serial.println(minute());
  
}
