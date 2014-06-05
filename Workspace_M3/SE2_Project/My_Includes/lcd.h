#ifndef lcd
#define lcd

int LCD_Init();
int LCD_WriteChar(char data,int x, int y);
int LCD_WriteString(char * data,int x, int y);
int LCD_CleanDisplay(char color);
#endif
