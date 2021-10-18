#ifndef MEDICINE_DISPENSER
#define MEDICINE_DISPENSER

/*
 * system running variable------------------------X
 * 
*/


#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define MAX_SIZE 3
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

typedef bool (*EventLamdaFunction)(int botton_type);
bool InternalEventFunction(int botton_type);

enum class ButtonType {
  None = 0,
  Up, 
  Down,
  Last,
  Enter,
  Reset
};

class ButtonState {
public:
  ButtonState();
  ~ButtonState();
  bool& operator[](ButtonType type);
  const bool& operator[](ButtonType type) const;
private:
  static bool botton_states[6];
};

class ButtonPressReleaseRecorder {
public:
  static void Run();
  static void SetButtonFunction(ButtonType type, EventLamdaFunction func);

  static bool RunButtonOf(ButtonType type);
  
  static int up;
  static int down;
  static int last;
  static int enter;
  static int _reset;
private:
  static int GetIndexOfButtonType(ButtonType type);
  static bool _pressed[6];
  
  static EventLamdaFunction up_f;
  static EventLamdaFunction down_f;
  static EventLamdaFunction last_f;
  static EventLamdaFunction enter_f;
  static EventLamdaFunction reset_f;
};

struct MConfig {
  unsigned int code;
  unsigned int count;
};

class OneDayTime {
public:
  OneDayTime();
  ~OneDayTime();
  MConfig& GetConfig(unsigned int index);
  void SetConfig(unsigned int index, MConfig _config);
  void SetTime(unsigned int time_by_seconds);
  unsigned int GetHour();
  unsigned int GetMinute();
  void AddAmount(unsigned int amount);
  void SubAmount(unsigned int amount);
  bool GetEdited(unsigned int index);
private:
  MConfig configs[MAX_SIZE];
  bool edited[MAX_SIZE];
  unsigned long m_time_by_seconds;
};

class DayTimeIterator {
public:
  DayTimeIterator();
  ~DayTimeIterator();

  void append(OneDayTime* daytime);
  OneDayTime* operator[](unsigned int index);
  unsigned int GetSize();
private:
  OneDayTime* pointers[5];
  unsigned int appended_size;
};

class DispenserConfiguration {
public:
  static OneDayTime& GetDayTime(unsigned int index);
  static void SetDayTime(unsigned int index, OneDayTime daytimes);
  static void SavePastData();
  static void BackToPastData();

  static bool GetEdited(unsigned int index);

  static void GetEditedIterator(DayTimeIterator* iterator);

  static void Delete(unsigned int index);
private:
  static OneDayTime daytimes[5];
  static OneDayTime past_info[5];
  static bool edited[5];
  
};

enum class LCD_Function {
  Default = 0,
  Config
};

struct lcd_position {
  unsigned int x;
  unsigned int y;
}; 

class LCD_Page {
public:
  static unsigned int GetDepth();
  static void GoDeep(unsigned int next_index);
  static void GoUp();
  static void ToLast();
  static unsigned int GetThisPageIndex();
  static unsigned int* GetThisPageList();
  static void Clear();

private:
  static unsigned int depth;
  static unsigned int indecies[8];
};

#define CHOICE_COUNT  5

class LCD_Manager {
public:
  static void Init();
  static void Test();
  static void Clear();
  static LiquidCrystal_I2C& Get();
  
  static void Call();

  static void ReInit();

  static void AppendPage(unsigned int index);
  static void Back();
  static void UpdateInfo();
  static lcd_position GetChoice(unsigned int index);

  static void SetCursor();

  static unsigned int choice_index;
  static lcd_position GetChoiceLoc();

  static void RunInternalEditFunction(unsigned int page_index, unsigned int button_number);

  static bool IsInEditPage();
  static void FinalPage();
private:
  static void CreatePage(unsigned int deep, unsigned int index);
  
  static void StartPage(unsigned int index);
  static void CommonInfoPage(unsigned int index);
  static void ConfigPage(unsigned int index);
  static void DaytimePage(unsigned int index);
  static void MConfigPage(unsigned int index);
  static void MConfigEditPage(unsigned int index);

  static OneDayTime& temp_daytime;
  static MConfig& temp_mconfig;
  static bool daytime_edited;
  static bool mconfig_edited;
  static bool inEditPage;

  static lcd_position choice_now[CHOICE_COUNT];

  static LCD_Function function;
  static LiquidCrystal_I2C lcd;
};

class ButtonManager {
public:
  ButtonManager();
  ButtonManager(int up, int down, int last, int enter, int _reset);
  ~ButtonManager();

  void Init();
  void onEvent();
};






#include <TimeLib.h>
#define YEAR__ (\
(__DATE__[7] - '0') * 1000 + \
(__DATE__[8] - '0') * 100 + \
(__DATE__[9] - '0') * 10 + \
(__DATE__[10] - '0') )

#define MONTH_IS(str) (\
__DATE__[0] == str[0] &&\
__DATE__[1] == str[1] &&\
__DATE__[2] == str[2])

#define MONTH__ (\
MONTH_IS("Jan") ? 1 : \
MONTH_IS("Feb") ? 2 : \
MONTH_IS("Mar") ? 3 : \
MONTH_IS("Apr") ? 4 : \
MONTH_IS("May") ? 5 : \
MONTH_IS("Jun") ? 6 : \
MONTH_IS("Jul") ? 7 : \
MONTH_IS("Aug") ? 8 : \
MONTH_IS("Sep") ? 9 : \
MONTH_IS("Oct") ? 10 : \
MONTH_IS("Nov") ? 11 : \
MONTH_IS("Dec") ? 12 : 0)

#define DAY__ \
    ( \
        ((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + \
        (__DATE__[5] - '0') \
    )

#define HOUR__ ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0')
#define MIN__  ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0')
#define SEC__  ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0')

class Time {
public:
  static void Init();
  static bool Test(unsigned int hour_, unsigned int minute_); 
};

#include <Servo.h>

class Motor {
public:
  Motor();
  Motor(unsigned int data_pin);
  ~Motor();

  void execute();
  void back();
  void _run(unsigned int times);
private:
  Servo m_motor;
};


class MotorManager {
public:
  void Run();
  void MotorSet(Motor a, Motor b, Motor c, Motor d);
private:
  Motor m_motors[4];
};


#endif
