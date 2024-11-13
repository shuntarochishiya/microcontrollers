#include <reg51.h>             // Include standard 8051 microcontroller register definitions
#include "lcd.h"               // Include custom library for LCD functions

// Define row pins for the keypad
sbit r1 = P1^0;                // Set r1 to bit 0 of port P1
sbit r2 = P1^1;                // Set r2 to bit 1 of port P1
sbit r3 = P1^2;                // Set r3 to bit 2 of port P1
sbit r4 = P1^3;                // Set r4 to bit 3 of port P1

// Define column pins for the keypad
sbit c1 = P1^4;                // Set c1 to bit 4 of port P1
sbit c2 = P1^5;                // Set c2 to bit 5 of port P1
sbit c3 = P1^6;                // Set c3 to bit 6 of port P1
sbit c4 = P1^7;                // Set c4 to bit 7 of port P1

// Declare function prototypes
void row_scan();               // Function to scan keypad rows for input
void delay(unsigned char);     // Delay function
void send_num(unsigned int);   // Function to send a number to the LCD
void calculate();              // Function to perform calculations

// Global variables for calculation
unsigned int y, a = 0, b = 0;  // Initialize variables for calculation operands
unsigned char operator, flag = 0; // Operator and flag for calculation state
unsigned char error = 0;       // Error flag for invalid input

void main()
{
    P1 = 0xf0;                 // Set rows as output and columns as input on port P1
    lcd_init();                // Initialize the LCD display
    lcd_cmd(0x85);             // Position cursor for "Tatiana" display
    lcd_string("Tatiana");     // Display "Tatiana" on LCD
    lcd_cmd(0xc3);             // Position cursor for "Kazakevich" display
    lcd_string("Kazakevich");  // Display "Kazakevich" on LCD
    delay(200);                // Delay for message display
    lcd_cmd(0x01);             // Clear LCD display
    while(1)
    {
        row_scan();            // Continuously scan the keypad rows
    }
}

void row_scan()
{
    if (error)                 // If an error is detected
    {
        lcd_cmd(0x01);         // Clear LCD display
        lcd_string("Invalid Input"); // Display error message
        delay(200);            // Delay for error message
        error = 0;             // Reset error flag
        lcd_cmd(0x01);         // Clear LCD again
        flag = a = b = 0;      // Reset operands and flag
        return;
    }

    //---ROW 1---//
    r1 = 0;                    // Set row 1 to low
    r2 = r3 = r4 = 1;          // Set other rows to high
    if(c1 == 0)                // If column 1 is low
    {
        delay(32);             // Small debounce delay
        while(c1 == 0)         // Wait while button is pressed
        {
            lcd_dat('7');      // Display '7' on LCD
            if(flag == 0)      // If first operand
                a = a*10 + 7;  // Add 7 to operand a
            else               // If second operand
                b = b*10 + 7;  // Add 7 to operand b
        }
    }
    if(c2 == 0)                // If column 2 is low
    {
        delay(32);
        while(c2 == 0)
        {
            lcd_dat('8');      // Display '8'
            if(flag == 0)
                a = a*10 + 8;
            else
                b = b*10 + 8;
        }
    }
    if(c3 == 0)                // If column 3 is low
    {
        delay(32);
        while(c3 == 0)
        {
            lcd_dat('9');      // Display '9'
            if(flag == 0)
                a = a*10 + 9;
            else
                b = b*10 + 9;
        }
    }
    if(c4 == 0)                // If column 4 is low
    {
        delay(32);
        while(c4 == 0)
        {
            if (flag == 1)     // If operator already set
            {
                error = 1;     // Set error flag
                break;
            }
            flag = 1;          // Set flag to indicate operator is selected
            lcd_dat('/');      // Display '/' operator
            operator = '/';    // Set operator to division
        }
    }

    //---ROW 2---//
    r2 = 0;                    // Set row 2 to low
    r1 = r3 = r4 = 1;          // Set other rows to high
    if(c1 == 0)
    {
        delay(32);
        while(c1 == 0)
        {
            lcd_dat('4');
            if(flag == 0)
                a = a*10 + 4;
            else
                b = b*10 + 4;
        }
    }
    if(c2 == 0)
    {
        delay(32);
        while(c2 == 0)
        {
            lcd_dat('5');
            if(flag == 0)
                a = a*10 + 5;
            else
                b = b*10 + 5;
        }
    }
    if(c3 == 0)
    {
        delay(32);
        while(c3 == 0)
        {
            lcd_dat('6');
            if(flag == 0)
                a = a*10 + 6;
            else
                b = b*10 + 6;
        }
    }
    if(c4 == 0)
    {
        delay(32);
        while(c4 == 0)
        {
            if (flag == 1)
            {
                error = 1;
                break;
            }
            flag = 1;
            lcd_dat('x');      // Display '*' operator
            operator = 'x';    // Set operator to multiplication
        }
    }

    //---ROW 3---//
    r3 = 0;                    // Set row 3 to low
    r2 = r1 = r4 = 1;          // Set other rows to high
    if(c1 == 0)
    {
        delay(32);
        while(c1 == 0)
        {
            lcd_dat('1');
            if(flag == 0)
                a = a*10 + 1;
            else
                b = b*10 + 1;
        }
    }
    if(c2 == 0)
    {
        delay(32);
        while(c2 == 0)
        {
            lcd_dat('2');
            if(flag == 0)
                a = a*10 + 2;
            else
                b = b*10 + 2;
        }
    }
    if(c3 == 0)
    {
        delay(32);
        while(c3 == 0)
        {
            lcd_dat('3');
            if(flag == 0)
                a = a*10 + 3;
            else
                b = b*10 + 3;
        }
    }
    if(c4 == 0)
    {
        delay(32);
        while(c4 == 0)
        {
            if (flag == 1)
            {
                error = 1;
                break;
            }
            flag = 1;
            lcd_dat('-');      // Display '-' operator
            operator = '-';    // Set operator to subtraction
        }
    }

    //---ROW 4---//
    r4 = 0;                    // Set row 4 to low
    r2 = r3 = r1 = 1;          // Set other rows to high
    if(c1 == 0)
    {
        delay(32);
        while(c1 == 0)
        {
            lcd_cmd(0x01);     // Clear LCD display
            flag = a = b = 0;  // Reset calculation variables
        }
    }
    if(c2 == 0)
    {
        delay(32);
        while(c2 == 0)
        {
            lcd_dat('0');      // Display '0'
            if(flag == 0)
                a = a*10 + 0;
            else
                b = b*10 + 0;
        }
    }
    if(c3 == 0)
    {
        delay(32);
        while(c3 == 0)
        {
            lcd_dat('=');      // Display '=' and perform calculation
            calculate();
        }
    }
    if(c4 == 0)
    {
        delay(32);
        while(c4 == 0)
        {
            if (flag == 1)
            {
                error = 1;
                break;
            }
            flag = 1;
            lcd_dat('+');      // Display '+' operator
            operator = '+';    // Set operator to addition
        }
    }
}

void delay(unsigned char t)
{
    unsigned int j;
    unsigned char i;
    for(i = 0; i < t; i++)     // Loop for delay time
        for(j = 0; j < 700; j++); // Nested loop to increase delay
}

void calculate()
{
    lcd_cmd(0xc0);             // Move LCD cursor to new line
    if(operator == '+')
    {
        send_num(a + b);       // Perform addition
    }
    else if(operator == '-')
    {
        if(a > b)
            send_num(a - b);   // Perform subtraction if a > b
        else
        {
            lcd_dat('-');      // Display negative sign
            send_num(b - a);   // Perform subtraction if b > a
        }
    }
    else if (operator == '/')
    {
        if (b != 0)
        {
            unsigned int integer_part = a / b;
            unsigned int remainder = a % b;
            unsigned int decimal_part1 = (remainder * 10) / b;
            unsigned int remainder2 = (remainder * 10) % b;
            unsigned int decimal_part2 = (remainder2 * 10) / b;

            send_num(integer_part);  // Display integer part
            lcd_dat('.');            // Display decimal point
            send_num(decimal_part1);  // Display first decimal digit
            send_num(decimal_part2);  // Display second decimal digit
        }
        else
        {
            lcd_cmd(0x01);
            lcd_string("Error: div by 0"); // Display division error
        }
    }
    else if(operator == 'x')
    {
        send_num(a * b);       // Perform multiplication
    }

    delay(1000);                // Delay to show result
    flag = 0;                  // Reset flag and operands
    a = b = 0;
    operator = 0;
    lcd_cmd(0x01);             // Clear LCD
}

void send_num(unsigned int x)
{
    if(x > 9999)               // Condition to display a 5-digit number
    {
        y = x/10000;           // Extract each digit and display
        lcd_dat(y + 0x30);
        x = x%10000;
        y = x/1000;
        lcd_dat(y + 0x30);
        x = x%1000;
        y = x/100;
        lcd_dat(y + 0x30);
        x = x%100;
        y = x/10;
        lcd_dat(y + 0x30);
        y = x%10;
        lcd_dat(y + 0x30);
    }
    else if(x > 999 && x < 9999)  // Condition to display a 4-digit number
    {
        y = x/1000;
        lcd_dat(y + 0x30);
        x = x%1000;
        y = x/100;
        lcd_dat(y + 0x30);
        x = x%100;
        y = x/10;
        lcd_dat(y + 0x30);
        y = x%10;
        lcd_dat(y + 0x30);
    }
    else if(x > 99 && x < 999)    // Condition to display a 3-digit number
    {
        y = x/100;
        lcd_dat(y + 0x30);
        x = x%100;
        y = x/10;
        lcd_dat(y + 0x30);
        y = x%10;
        lcd_dat(y + 0x30);
    }
    else if(x > 9 && x < 99)      // Condition to display a 2-digit number
    {
        y = x/10;
        lcd_dat(y + 0x30);
        y = x%10;
        lcd_dat(y + 0x30);
    }
    else
    {
        lcd_dat(x + 0x30);        // Display a single-digit number
    }
}
