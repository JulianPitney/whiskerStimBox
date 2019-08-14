#include <LiquidCrystal.h>

const int rs = 8, en = 9, d4 = 13, d5 = 12, d6 = 11, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool button_down;
static uint8_t prevNextCode = 0;

int menu_index = 0;
String menu_pages[] = {"Cycles = ","On-Time = ","Off-Time = ","Start Delay = ","Push To Start"};
String parameterNames[] = {"    Cycles","    On-Time","    Off-Time","   Start Delay"};
int parameterValues[] = {1,1,1,1};




void setup() {

  lcd.begin(16,2);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(A2, OUTPUT);
  lcd.setCursor(4,0);
  lcd.print("Welcome!");
  lcd.setCursor(0,1);
  lcd.print("Rotate To Begin!");
}

int run_program()
{
  for(int i = parameterValues[3]; i > 0; i--)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Starting in...");
    lcd.setCursor(7,1);
    lcd.print(i);
    delay(1000);
  }

  
  for(int i = 0; i < parameterValues[0]; i++)
  {

    digitalWrite(A2, HIGH);
    for(int x = parameterValues[1]; x > 0; x--)
    {
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("ON");
      lcd.setCursor(0,1);
      lcd.print("Time Left: " + String(x));
      delay(1000);
    }
    
    digitalWrite(A2, LOW);
    for(int x = parameterValues[2]; x > 0; x--)
    {
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("OFF");
      lcd.setCursor(0,1);
      lcd.print("Time Left: " + String(x));
      delay(1000);
    }
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Program Complete");
  lcd.setCursor(0,1);
  lcd.print("Returning...");
  delay(2000);
}


int8_t read_rotary() 
{
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  prevNextCode <<= 2;
  if (digitalRead(A1)) prevNextCode |= 0x02;
  if (digitalRead(A0)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

  return ( rot_enc_table[( prevNextCode & 0x0f )]);
}

void check_button_pressed()
{
    int pushedCount = 0;
    for(unsigned int i = 0; i < 150; i++)
    {
       pushedCount += int(digitalRead(3));
    }
    
    if(pushedCount < 50 && !button_down)
    {
       button_down = true;
    }
    else if(pushedCount >= 51)
    {
       button_down = false;
    }    
}

void update_menu_pos(bool increment)
{
  if(increment)
  {
    if(menu_index >= 4)
    {
      menu_index = 0;
    }
    else
    {
      menu_index += 1;
    }
  }
  else
  {
    if(menu_index <= 0)
    {
      menu_index = 4;
    }
    else
    {
      menu_index -= 1;
    }
  }

  print_menu();
}

void print_menu()
{

  lcd.setCursor(0,0);
  lcd.clear();
  switch(menu_index)
  {
    
    case 0:
      lcd.print(menu_pages[menu_index] + String(parameterValues[0]));
      lcd.setCursor(0,1); 
      lcd.print("Push to select");
      break;
    case 1:
      lcd.print(menu_pages[menu_index] + String(parameterValues[1]));
      lcd.setCursor(0,1); 
      lcd.print("Push to select");
      break;
    case 2:
      lcd.print(menu_pages[menu_index] + String(parameterValues[2]));
      lcd.setCursor(0,1); 
      lcd.print("Push to select");
      break;
    case 3:
      lcd.print(menu_pages[menu_index] + String(parameterValues[3]));
      lcd.setCursor(0,1); 
      lcd.print("Push to select");
      break;
    case 4:
      lcd.print(menu_pages[menu_index]);
      break;
    default:
      break;
  }  
}

void print_parameter(int parameter_index)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(parameterNames[parameter_index] + ": ");
  lcd.setCursor(8,1);
  lcd.print(parameterValues[parameter_index]); 
}

int set_parameter(int parameter_index)
{

  while(button_down)
  {
    check_button_pressed();
  }

  print_parameter(parameter_index);
  
  while(1)
  {
    if(read_rotary()) 
    {
      if (prevNextCode==0x0b) 
      {
         if(parameterValues[parameter_index] > 0)
         {
            parameterValues[parameter_index] -= 1;
         }
         print_parameter(parameter_index);     
      }
      else if (prevNextCode==0x07) 
      {
         parameterValues[parameter_index] += 1;
         print_parameter(parameter_index);    
      }
      delay(1);
   }

   check_button_pressed();
   if(button_down)
   {
      while(button_down)
      {
        check_button_pressed();
      }
      return 0;
   }
 }
}

void menu_select()
{
  switch(menu_index)
  {
    case 0:
      set_parameter(0);
      break;
    case 1:
      set_parameter(1);
      break;
    case 2:
      set_parameter(2);
      break;
    case 3:
      set_parameter(3);
      break;
    case 4:
      run_program();
      break;
    default:
      break;
  }
}

void loop() {

   if(read_rotary()) 
   {
      if ( prevNextCode==0x0b) 
      {
         update_menu_pos(false);
      }
      else if ( prevNextCode==0x07) 
      {
         update_menu_pos(true);
      }
      delay(1);
   }

   check_button_pressed();
   if(button_down)
   {
    menu_select();
    print_menu();
   }
}
