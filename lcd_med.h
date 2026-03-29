#ifndef LCD_MED_H
#define LCD_MED_H

void Write_LCD(char data);
void Cmd_LCD(char cmd);
void Char_LCD(char ch);
void Init_LCD(void);
void Str_LCD(char* p);
void Uint_LCD(unsigned int n);
void Sint_LCD(int n);
void Float_LCD(float num, char nDP);
void Hex_LCD(unsigned int n);
void Bin_LCD(unsigned int n, char nBB);
void Build_CGRAM(char* p, char nb);

#endif
