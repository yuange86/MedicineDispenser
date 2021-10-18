#include "MedicineDispenser.h"

bool InternalEventFunction(int button_type) { return false; }

//_ButtonState--------------------------------------------------------
bool ButtonState::botton_states[6] = { false, false, false, false, false, false };

ButtonState::ButtonState()
{
}

ButtonState::~ButtonState()
{
}

bool& ButtonState::operator[](ButtonType type)
{
  switch(type)
  {
    case ButtonType::None : break;
    case ButtonType::Up   : return botton_states[0];
    case ButtonType::Down : return botton_states[1];
    case ButtonType::Last : return botton_states[2];
    case ButtonType::Enter: return botton_states[3];
    case ButtonType::Reset: return botton_states[4];
  }
  return botton_states[5];
}

const bool& ButtonState::operator[](ButtonType type) const
{
  switch(type)
  {
    case ButtonType::None : break;
    case ButtonType::Up   : return botton_states[0];
    case ButtonType::Down : return botton_states[1];
    case ButtonType::Last : return botton_states[2];
    case ButtonType::Enter: return botton_states[3];
    case ButtonType::Reset: return botton_states[4];
  }
  return botton_states[5];
}

//_ButtonPressReleaseRecorder-----------------------------------------

bool ButtonPressReleaseRecorder::_pressed[6] = { false, false, false, false, false, false };
int ButtonPressReleaseRecorder::up = 0;
int ButtonPressReleaseRecorder::down = 0;
int ButtonPressReleaseRecorder::last = 0;
int ButtonPressReleaseRecorder::enter = 0;
int ButtonPressReleaseRecorder::_reset = 0;
EventLamdaFunction ButtonPressReleaseRecorder::up_f = InternalEventFunction;
EventLamdaFunction ButtonPressReleaseRecorder::down_f = InternalEventFunction;
EventLamdaFunction ButtonPressReleaseRecorder::last_f = InternalEventFunction;
EventLamdaFunction ButtonPressReleaseRecorder::enter_f = InternalEventFunction;
EventLamdaFunction ButtonPressReleaseRecorder::reset_f = InternalEventFunction;

int ButtonPressReleaseRecorder::GetIndexOfButtonType(ButtonType type)
{
  switch(type)
  {
    case ButtonType::None : break;
    case ButtonType::Up   : return 0;
    case ButtonType::Down : return 1;
    case ButtonType::Last : return 2;
    case ButtonType::Enter: return 3;
    case ButtonType::Reset: return 4;
  }
  return 5;
}

bool ButtonPressReleaseRecorder::RunButtonOf(ButtonType type)
{
  ButtonState st = ButtonState();
  if(st[type] == true)
  {
    _pressed[GetIndexOfButtonType(type)] = true;
  }
  else if(_pressed[GetIndexOfButtonType(type)] == true && st[type] == false)
  {
    _pressed[GetIndexOfButtonType(type)] = false;
    return true;
  }
  return false;
}

void ButtonPressReleaseRecorder::Run()
{
  up_f(up);
  down_f(down);
  last_f(last);
  enter_f(enter);
  reset_f(_reset);
}

void ButtonPressReleaseRecorder::SetButtonFunction(ButtonType type, EventLamdaFunction func)
{
  switch(type)
  {
    case ButtonType::None : break;
    case ButtonType::Up   : up_f = func; break;
    case ButtonType::Down : down_f = func; break;
    case ButtonType::Last : last_f = func; break;
    case ButtonType::Enter: enter_f = func; break;
    case ButtonType::Reset: reset_f = func; break;
  }
}

//_OneDayTime----------------------------------------------------------

OneDayTime::OneDayTime()
{
  configs[0] = {0, 0};
  configs[1] = {0, 0};
  configs[2] = {0, 0};
}
OneDayTime::~OneDayTime()
{}

MConfig& OneDayTime::GetConfig(unsigned int index)
{
  return configs[index];
}

void OneDayTime::SetConfig(unsigned int index, MConfig _config)
{
  configs[index] = _config;
}

void OneDayTime::SetTime(unsigned int time_by_seconds)
{
  m_time_by_seconds = time_by_seconds;
}

unsigned int OneDayTime::GetHour()
{
  return (m_time_by_seconds / 3600);
}

unsigned int OneDayTime::GetMinute()
{
  return (m_time_by_seconds % 3600) / 60;
}

void OneDayTime::AddAmount(unsigned int amount)
{
  m_time_by_seconds += amount;
}

void OneDayTime::SubAmount(unsigned int amount)
{
  m_time_by_seconds -= amount;
}

bool OneDayTime::GetEdited(unsigned int index)
{
  return edited[index];
}

//_DayTimeIterator----------------------------------------------------------

DayTimeIterator::DayTimeIterator()
{
  appended_size = 0;
}

DayTimeIterator::~DayTimeIterator()
{
  
}

void DayTimeIterator::append(OneDayTime* daytime)
{
  pointers[appended_size] = daytime;
  appended_size += 1;
}

OneDayTime* DayTimeIterator::operator[](unsigned int index)
{
  return pointers[index];
}

unsigned int DayTimeIterator::GetSize()
{
  return appended_size;
}

//_DispenserConfiguration----------------------------------------------

OneDayTime DispenserConfiguration::daytimes[5] = { OneDayTime(), OneDayTime(), OneDayTime(), OneDayTime(), OneDayTime() };
OneDayTime DispenserConfiguration::past_info[5] = { OneDayTime(), OneDayTime(), OneDayTime(), OneDayTime(), OneDayTime() };

bool DispenserConfiguration::edited[5] = { false, false, false, false, false };

OneDayTime& DispenserConfiguration::GetDayTime(unsigned int index)
{
  return daytimes[index];
}

void DispenserConfiguration::SetDayTime(unsigned int index, OneDayTime daytime)
{
  daytimes[index] = daytime;
}

void DispenserConfiguration::SavePastData()
{
  past_info[0] = daytimes[0];
  past_info[1] = daytimes[1];
  past_info[2] = daytimes[2];
  past_info[3] = daytimes[3];
  past_info[4] = daytimes[4];
}

void DispenserConfiguration::BackToPastData()
{
  daytimes[0] = past_info[0];
  daytimes[1] = past_info[1];
  daytimes[2] = past_info[2];
  daytimes[3] = past_info[3];
  daytimes[4] = past_info[4];
}

bool DispenserConfiguration::GetEdited(unsigned int index)
{
  return edited[index];
}

void DispenserConfiguration::GetEditedIterator(DayTimeIterator* iterator)
{
  while(true) {
    static unsigned int index = 0;
    switch(GetEdited(index))
    {
      case false: continue;
      case true:  iterator->append(daytimes + index);
    }
    index++;
  }
}

void DispenserConfiguration::Delete(unsigned int index)
{
  edited[index] = false;
  daytimes[index] = OneDayTime();
}

//_LCD_Page------------------------------------------------------------------

unsigned int LCD_Page::depth = 0;
unsigned int LCD_Page::indecies[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

unsigned int LCD_Page::GetDepth()
{
  return depth;
}

void LCD_Page::GoDeep(unsigned int next_index)
{
  if(depth == 8)
    return;
  indecies[depth] = next_index+1;
  depth += 1;
}

void LCD_Page::GoUp()
{
  if(depth == 0)
    return;
  depth -= 1;
  indecies[depth] = 0;
}

unsigned int LCD_Page::GetThisPageIndex()
{
  if(depth == 0)
    return 0;
  return indecies[depth-1];
}

unsigned int* LCD_Page::GetThisPageList()
{
  return (unsigned int*)indecies;
}

void LCD_Page::ToLast()
{
  depth = 8;
}

void LCD_Page::Clear()
{
  depth = 0;
  indecies[0] = 0;
  indecies[1] = 0;
  indecies[2] = 0;
  indecies[3] = 0;
  indecies[4] = 0;
  indecies[5] = 0;
  indecies[6] = 0;
  indecies[7] = 0;
}


//_LCD_Manager---------------------------------------------------------

LiquidCrystal_I2C LCD_Manager::lcd = LiquidCrystal_I2C(0x27, 16, 2);
LCD_Function LCD_Manager::function = LCD_Function::Default;

lcd_position LCD_Manager::choice_now[CHOICE_COUNT] = {{0,0},{0,0},{0,0},{0,0}};
unsigned int LCD_Manager::choice_index = 0;

bool LCD_Manager::daytime_edited = false;
bool LCD_Manager::mconfig_edited = false;
bool LCD_Manager::inEditPage = false;

OneDayTime& LCD_Manager::temp_daytime = DispenserConfiguration::GetDayTime(0);
MConfig& LCD_Manager::temp_mconfig = temp_daytime.GetConfig(0);

void LCD_Manager::Init()
{
  choice_index = 0;
  
  lcd = LiquidCrystal_I2C(0x27, 16, 2);
  lcd.init();
  lcd.backlight();
  lcd.cursor();
  lcd.clear();

  uint8_t configMark[8] = {
    0b00000,
    0b01110,
    0b11001,
    0b10101,
    0b10111,
    0b11001,
    0b01110,
    0b00000
  };
  uint8_t selectMark[8] = {
    0b00000,
    0b01000,
    0b01100,
    0b01110,
    0b01110,
    0b01100,
    0b01000,
    0b00000
  };

  uint8_t checkMark[8] = {
    0b00000,
    0b00001,
    0b00001,
    0b00010,
    0b11010,
    0b01010,
    0b00100,
    0b00000
  };

  uint8_t antiselectMark[8] = {
    0b00000,
    0b00010,
    0b00110,
    0b01110,
    0b01110,
    0b00110,
    0b00010,
    0b00000
  };

  uint8_t homeMark[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b01110,
    0b01010,
    0b01010,
    0b01110,
    0b00000
  };

  lcd.createChar(0, configMark);
  lcd.createChar(1, selectMark);
  lcd.createChar(2, checkMark);
  lcd.createChar(3, antiselectMark);
  lcd.createChar(4, homeMark);

  //lcd.print("LCD_Manager init");
  //lcd.setCursor(0, 1);
  //lcd.print("Welcome-succeed");
  AppendPage(0);
}

void LCD_Manager::Test()
{
  lcd.backlight();
  delay(100);
  lcd.noBacklight();
  delay(100);
  lcd.backlight();
  delay(100);
  lcd.noBacklight();
  delay(100);
  lcd.backlight();
}

void LCD_Manager::Clear()
{
  lcd.clear();
  lcd.backlight();
  choice_index = 0;
}

LiquidCrystal_I2C& LCD_Manager::Get()
{
  return lcd;
}

lcd_position LCD_Manager::GetChoice(unsigned int index)
{
  return choice_now[index];
}

void LCD_Manager::CreatePage(unsigned int deep, unsigned int index)
{
  lcd.cursor_off();
  switch(deep)
  {
    case 0:
    {
      lcd.print("Cancel succeed");
      lcd.setCursor(0, 1);
      lcd.print(">>>Press Enter !");
    } break;
    case 1: StartPage(index); break;
    case 2: CommonInfoPage(index); break;
    case 3: ConfigPage(index); break;
    case 4: DaytimePage(index); break;
    case 5: MConfigPage(index); break;
    case 6: MConfigEditPage(index); break;
  }
  SetCursor();
  lcd.cursor_on();
}

void LCD_Manager::Call()
{
  ReInit();
  Test();
  CreatePage(LCD_Page::GetDepth(), LCD_Page::GetThisPageIndex());
}

void LCD_Manager::ReInit()
{
  Clear();
  temp_daytime = DispenserConfiguration::GetDayTime(0);
  temp_mconfig = temp_daytime.GetConfig(0);
  LCD_Page::Clear();
  CreatePage(0, 0);
}

void LCD_Manager::AppendPage(unsigned int index)
{
  LCD_Page::GoDeep(index);
  UpdateInfo();
  Clear();
  CreatePage(LCD_Page::GetDepth(), index);
}

void LCD_Manager::Back()
{
  LCD_Page::GoUp();
  UpdateInfo();
  Clear();
  CreatePage(LCD_Page::GetDepth(), LCD_Page::GetThisPageIndex());
}

void LCD_Manager::UpdateInfo()
{
  auto list = LCD_Page::GetThisPageList();
  unsigned int depth = LCD_Page::GetDepth();

  if( depth <= 2 )
    return;

  unsigned int config_index = *(list+2);        //--------------------------------------------TODO()
  unsigned int daytime_index = *(list+3);
  unsigned int mconfig_index = *(list+4);
  
  if(daytime_index != 0)
  {
    temp_daytime = DispenserConfiguration::GetDayTime(daytime_index-1);
    daytime_edited = DispenserConfiguration::GetEdited(daytime_index-1);
  }
  if(mconfig_index != 0)
  {
    temp_mconfig = temp_daytime.GetConfig(mconfig_index-1);
    mconfig_edited = temp_daytime.GetEdited(mconfig_index-1);
  }
  
}

lcd_position LCD_Manager::GetChoiceLoc()
{
  return choice_now[choice_index];
}

void LCD_Manager::SetCursor()
{
  LCD_Manager::Get().setCursor(LCD_Manager::GetChoiceLoc().x, LCD_Manager::GetChoiceLoc().y);
}

bool LCD_Manager::IsInEditPage()
{
  return inEditPage;
}





void LCD_Manager::StartPage(unsigned int index)
{
  inEditPage = false;
  switch(index)
  {
    case 0:
    {
      lcd.print("LCD_Manager init");
      lcd.setCursor(0, 1);
      lcd.print("Welcome! 01234");

      choice_now[0] = {9,1};
      choice_now[1] = {10,1};
      choice_now[2] = {11,1};
      choice_now[3] = {12, 1};
      choice_now[4] = {13, 1};
    } break;
  }
}

void LCD_Manager::CommonInfoPage(unsigned int index)
{
  inEditPage = false;
  int edited_time = 0;
  for(int i = 0; i < 5; i++)
  {
    if(DispenserConfiguration::GetEdited(i))
      edited_time +=1;
  }
  {
    lcd.print("MedicineDispense");
    lcd.setCursor(0, 1);
    lcd.print("DayTimes : "); lcd.print(edited_time);
    lcd.setCursor(15, 1);
    lcd.printByte(0);
    
    choice_now[0] = {15, 1};
    choice_now[1] = {15, 0};
    choice_now[2] = {15, 0};
    choice_now[3] = {15, 0};
    choice_now[4] = {15, 0};
    }
}

void LCD_Manager::ConfigPage(unsigned int index)
{
  inEditPage = false;
  switch(index)
  {
    case 0:
    {
      lcd.print("DayTime Edit 000");
      lcd.setCursor(0, 1);
      lcd.print("Edit : 0 1 2 3 4");
      
      choice_now[0] = {7, 1};
      choice_now[1] = {9, 1};
      choice_now[2] = {11, 1};
      choice_now[3] = {13, 1};
      choice_now[4] = {15, 1};
    } break;
  }
}

void LCD_Manager::DaytimePage(unsigned int index)
{
  inEditPage = false;
  lcd.print("DayTime00"); lcd.print(index);
  if(daytime_edited)
  {
    lcd.print(" <O>");
  }
  else
  {
    lcd.print(" <X>");
  }
  lcd.setCursor(15,0); lcd.printByte(0);
  lcd.setCursor(0, 1);
  lcd.print("Code 1 2 3 4");
    
  choice_now[0] = {5, 1};
  choice_now[1] = {7, 1};
  choice_now[2] = {9, 1};
  choice_now[3] = {11, 1};
  choice_now[4] = {15, 0};
}

void LCD_Manager::MConfigPage(unsigned int index)
{ 
  inEditPage = false;
  int daytime_index = *(LCD_Page::GetThisPageList()+3);
  if(index == 4)
  {
    inEditPage = true;
    lcd.print("DayTime");lcd.print(daytime_index); lcd.print(">"); lcd.print("Time");
    lcd.setCursor(15, 0); lcd.printByte(4);
    lcd.setCursor(5, 1);
    int hour = temp_daytime.GetHour();
    int minute = temp_daytime.GetMinute();
    lcd.printByte(3);
    if(hour < 10)
    {
      lcd.print("0"); lcd.print(hour);
    }
    else
    {
      lcd.print(hour);
    }
    lcd.print(":");
    if(minute < 10)
    {
      lcd.print("0"); lcd.print(minute);
    }
    else
    {
      lcd.print(minute);
    }
    lcd.printByte(1);

    choice_now[0] = {5, 1};
    choice_now[1] = {7, 1};
    choice_now[2] = {9, 1};
    choice_now[3] = {11, 1};
    choice_now[4] = {15, 0};
    
    return;
  }
  lcd.print("DayTime"); lcd.print(daytime_index); lcd.print(">"); lcd.print(index+1);
  lcd.setCursor(15, 0); lcd.printByte(0);
  
  lcd.setCursor(0, 1);
  lcd.print(" "); lcd.printByte(1); lcd.print("code  "); lcd.printByte(1); lcd.print("count");
  
  choice_now[0] = {1, 1};
  choice_now[1] = {8, 1};
  choice_now[2] = {1, 1};
  choice_now[3] = {8, 1};
  choice_now[4] = {15, 0};
}

void LCD_Manager::MConfigEditPage(unsigned int index)
{
  inEditPage = true;
  int daytime_index = *(LCD_Page::GetThisPageList()+3);
  int config_index = *(LCD_Page::GetThisPageList()+4);
  lcd.print("DayTime"); lcd.print(daytime_index); lcd.print(">"); lcd.print(config_index+1);
  lcd.setCursor(15, 0); lcd.printByte(0); lcd.setCursor(15, 1); lcd.printByte(2);

  lcd.setCursor(0, 1);
  switch(index)
  {
    case 0: lcd.print("  code : "); lcd.printByte(1); lcd.print(temp_mconfig.code); break;
    case 1: lcd.print(" count : "); lcd.printByte(1); lcd.print(temp_mconfig.count); break;
    case 2: lcd.print("  code : "); lcd.printByte(1); lcd.print(temp_mconfig.code); break;
    case 3: lcd.print(" count : "); lcd.printByte(1); lcd.print(temp_mconfig.count); break;
    case 4: LCD_Manager::Back(); return;
  }

  choice_now[0] = {9, 1};
  choice_now[1] = {9, 1};
  choice_now[2] = {9, 1};
  choice_now[3] = {9, 1};
  choice_now[4] = {9, 1};
}

void LCD_Manager::FinalPage()
{
  LCD_Page::ToLast();
  lcd.print("Save the change"); lcd.printByte(4);
  lcd.setCursor(0, 1); lcd.print("      X  "); lcd.printByte(2);
  choice_now[0] = {6, 1};
  choice_now[1] = {9, 1};
  choice_now[2] = {6, 1};
  choice_now[3] = {9, 1};
  choice_now[4] = {15, 0};
  SetCursor();

}




void LCD_Manager::RunInternalEditFunction(unsigned int page_index, unsigned int button_number)
{
  if(button_number == 4)
  {
    Back();
    return;
  }
  switch(page_index)
  {
    case 2: // DayTime time setting
    {
      switch(button_number)
      {
        case 0: temp_daytime.SubAmount(1800); break;
        case 1: temp_daytime.AddAmount(3600); break;
        case 2: temp_daytime.AddAmount(60); break;
        case 3: temp_daytime.AddAmount(1800); break;
      }
      UpdateInfo();
      Clear();
      CreatePage(LCD_Page::GetDepth(), LCD_Page::GetThisPageIndex());
      choice_index = button_number;
      SetCursor();
    } return;
    case 0: // code setting
    {
      switch(button_number)
      {
        case 0: temp_mconfig.code += 1; break; //increase
        case 1: temp_mconfig.code -= 1; break; //decrease
      }
    } break;
    case 1: // count setting
    {
      switch(button_number)
      {
        case 0: temp_mconfig.count += 1; break; //increase
        case 1: temp_mconfig.count -= 1; break; //decrease
      }
    } break;
  }
  UpdateInfo();
  Clear();
  CreatePage(LCD_Page::GetDepth(), LCD_Page::GetThisPageIndex());
}

//_ButtonManager-------------------------------------------------------

ButtonManager::ButtonManager()
{
  ButtonPressReleaseRecorder::up = 0;
  ButtonPressReleaseRecorder::down = 0;
  ButtonPressReleaseRecorder::last = 0;
  ButtonPressReleaseRecorder::enter = 0;
  ButtonPressReleaseRecorder::_reset = 0;
}

ButtonManager::ButtonManager(int up, int down, int last, int enter, int _reset)
{
  ButtonPressReleaseRecorder::up = up;
  ButtonPressReleaseRecorder::down = down;
  ButtonPressReleaseRecorder::last = last;
  ButtonPressReleaseRecorder::enter = enter;
  ButtonPressReleaseRecorder::_reset = _reset;
}

ButtonManager::~ButtonManager()
{
}

void ButtonManager::Init()
{
  LCD_Manager::Init();
}

void ButtonManager::onEvent()
{
  ButtonState st = ButtonState();
  if(digitalRead(ButtonPressReleaseRecorder::up) == HIGH)
  {
    Serial.println("Up");
    st[ButtonType::Up] = true;
  }
  else
  {
    st[ButtonType::Up] = false;
  }

  if(digitalRead(ButtonPressReleaseRecorder::down) == HIGH)
  {
    Serial.println("Down");
    st[ButtonType::Down] = true;
  }
  else
  {
    st[ButtonType::Down] = false;
  }

  if(digitalRead(ButtonPressReleaseRecorder::last) == HIGH)
  {
    Serial.println("Last");
    st[ButtonType::Last] = true;
  }
  else
  {
    st[ButtonType::Last] = false;
  }

  if(digitalRead(ButtonPressReleaseRecorder::enter) == HIGH)
    {
      Serial.println("Enter");
      st[ButtonType::Enter] = true;
    }
    else
    {
      st[ButtonType::Enter] = false;
  }

  if(digitalRead(ButtonPressReleaseRecorder::_reset) == HIGH)
  {
    Serial.println("Reset");
    st[ButtonType::Reset] = true;
  }
  else
  {
    st[ButtonType::Reset] = false;
  }
  
  ButtonPressReleaseRecorder::Run();
}






void Time::Init()
{
  setTime(HOUR__, MIN__, SEC__, DAY__, MONTH__, YEAR__);
}

bool Time::Test(unsigned int hour_, unsigned int minute_)
{
  return hour() == hour_ && minute() == minute_;
}


Motor::Motor() { m_motor.attach(3); }
Motor::Motor(unsigned data_pin) { m_motor.attach(data_pin); }
Motor::~Motor() {}

void Motor::execute()
{
  m_motor.write(90);
}

void Motor::back()
{
  m_motor.write(0);
}

void Motor::_run(unsigned int times)
{
  for(int i = 0; i < times; i++)
  {
    execute();
    delay(60);
    back();
    delay(60);
  }
}

void MotorManager::Run() 
{
  DayTimeIterator iterator;
  DispenserConfiguration::GetEditedIterator(&iterator);
  for(int i = 0; i < iterator.GetSize(); i++)
  {
    OneDayTime* daytime_ = iterator[i];
    if(Time::Test(daytime_->GetHour(), daytime_->GetMinute()))
    {
      for(int i = 0; i < 5; i++) {
        switch(daytime_->GetConfig(i).code)
        {
          case 0 : continue;
          case 1 : m_motors[0]._run(daytime_->GetConfig(i).count); break;
          case 2 : m_motors[1]._run(daytime_->GetConfig(i).count); break;
          case 3 : m_motors[2]._run(daytime_->GetConfig(i).count); break;
          case 4 : m_motors[3]._run(daytime_->GetConfig(i).count); break;        }
      }
    }
  }
}
