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
  bool GetEdited(unsigned int index);
private:
  MConfig configs[MAX_SIZE];
  bool edited[MAX_SIZE];
  unsigned int m_time_by_seconds;
};

class DispenserConfiguration {
public:
  static OneDayTime& GetDayTime(unsigned int index);
  static void SetDayTime(unsigned int index, OneDayTime daytimes);
  static void SavePastData();
  static void BackToPastData();

  static bool GetEdited(unsigned int index);

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



#endif
