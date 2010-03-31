#ifndef LCD_HPP
#define LCD_HPP

void drawString(const char* str, double x, double y, double s);
void drawString(const char* str, double x, double y, double s, double r, double g, double b);
void drawString(const char* str, double x, double y, double s, double r, double g, double b, double r2, double g2, double b2);
void initLCD();

#endif
