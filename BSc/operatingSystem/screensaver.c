#include "include/random.h"
#include "include/asm.h"
#include "include/tty.h"
#include "include/screensaver.h"

static int scr_handle;

static unsigned int i,j;
static char c[2];
static char disp[10][25];

void screensaver()
{
  c[1] = 0;
  
  while(1)
  {
    for (j=0;j<10;j++)
    {
      for (i=1;i<25;i++)
      {
        disp[j][i] = disp[j][i-1];
      }
    }
    
    for (j=0;j<10;j++)
    {
      
      i = uniform(3)+1;
    
      switch (i)
      {
        case 1:      
            disp[j][0] = uniform(2)+48;            
        break;
        case 2:
            disp[j][0] = ' ';
        break;        
        case 3:
            disp[j][0] = ' ';
        break;        
      
      }
          
    }
    for (i=0;i<25;i++)
    {
      for (j=0;j<10;j++)
      {
        tty_gotoxy(scr_handle,(j+1)*(6+uniform(4)),i);
        c[0] = disp[j][i] + uniform(1);
        tty_print(scr_handle,c);
      }
      //delay(2);
    }    
    delay(3);
  }
  delay(1); 
}

void need_scr()
{
  scr_handle = tty_open();    
  tty_set_attribute(scr_handle,0x02,0x00);
  tty_clear_display(scr_handle);
  set_focus(0,scr_handle,1);
  seed();
}
