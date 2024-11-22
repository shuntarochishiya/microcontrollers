#include <reg51.h>                // Include the register definitions for the 8051 microcontroller
#include "lcd.h"                   // Include the header file for controlling the LCD

// Pin assignments for the keypad rows and columns
sbit r1 = P1^0;                  
sbit r2 = P1^1;                  
sbit r3 = P1^2;                  
sbit r4 = P1^3;                  
sbit c1 = P1^4;                  
sbit c2 = P1^5;                  
sbit c3 = P1^6;                  
sbit c4 = P1^7;                  

// Variables for storing operands, operator, flags, and error handling
unsigned int a = 0, b = 0, y;    
unsigned char flag = 0, operator = 0, error = 0, result_displayed = 0; 

// Function to create a delay
void delay(unsigned char t) {
    unsigned int i, j;
    for (i = 0; i < t; i++)         // Outer loop for controlling the delay
        for (j = 0; j < 700; j++); // Inner loop for controlling the delay duration
}

// Function to send a number to the LCD
void send_num(unsigned int x) {
    char buf[6] = {0};             // Buffer to hold the string representation of the number
    char i = 0;
    while (x > 0) {                
        buf[i++] = (x % 10) + '0'; // Convert the digit to a character and store it in the buffer
        x /= 10;                    // Reduce x by a factor of 10
    }
    while (--i >= 0)                // Send the digits in reverse order to the LCD
        lcd_dat(buf[i]);
}

// Function to send a floating-point number to the LCD
void send_float(float x) {
    int integer_part = (int)x;                        // Extract the integer part of the float
    int decimal_part = (int)((x - integer_part) * 100); // Extract the decimal part and multiply by 100
    send_num(integer_part);                          // Send the integer part to the LCD
    lcd_dat('.');                                    // Send the decimal point to the LCD
    lcd_dat(decimal_part / 10 + '0');                // Send the first digit of the decimal part
    lcd_dat(decimal_part % 10 + '0');                // Send the second digit of the decimal part
}

// Function to reset the calculator
void reset_calc() {
    lcd_cmd(0x01);        // Clear the LCD screen
    flag = operator = error = result_displayed = a = b = 0; // Reset all variables
}

// Function to perform the calculation based on the operator
void calculate() {
    float result = 0;
    lcd_cmd(0xc0);       // Move the cursor to the second row of the LCD

    // Perform the calculation based on the operator
    if (operator == '+') {
        result = (float)a + (float)b;
    } else if (operator == '-') {
        result = (float)a - (float)b;
    } else if (operator == 'x') {
        result = (float)a * (float)b;
    } else if (operator == '/') {
        if (b != 0) {                     // Check for division by zero
            result = (float)a / (float)b;
        } else {
            lcd_string("Error: Div By 0");  // Display error message if division by zero
            delay(200);
            reset_calc();
            return;
        }
    }

    // Check for overflow
    if (result > 65535) {
        lcd_string("Error: Overflow");
        delay(200);
        reset_calc();
        return;
    }

    // Check for negative result and display the negative sign
    if (result < 0) {
        lcd_dat('-');
        result = -result;
    }

    // Send the result to the LCD
    send_float(result);
    result_displayed = 1; // Mark the result as displayed
}

// Function to process the key pressed on the keypad
void process_key(unsigned char key) {
    if (key == 'C') {    // If 'C' is pressed, reset the calculator
        reset_calc();
        return;
    }

    if (result_displayed) reset_calc(); // If result was displayed, reset the calculator

    if (key >= '0' && key <= '9') {    // If a number key is pressed
        unsigned int *target = flag ? &b : &a; // Determine whether to update a or b based on the flag
        if (*target > 6553 || (*target == 6553 && key > '5')) { // Prevent overflow
            lcd_cmd(0x01);
            lcd_string("Error: Overflow");
            delay(200);
            reset_calc();
        } else {
            lcd_dat(key); // Display the key on the LCD
            *target = *target * 10 + (key - '0'); // Update the target value (a or b)
        }
    } else if (key == '+' || key == '-' || key == 'x' || key == '/') { // If an operator is pressed
        if (flag) { // If the second operand is already entered, show an error
            lcd_cmd(0x01);
            lcd_string("Invalid Input");
            delay(200);
            reset_calc();
        } else {
            lcd_dat(key); // Display the operator
            operator = key; // Set the operator
            flag = 1; // Set the flag to indicate that the first operand is entered
        }
    } else if (key == '=') { // If '=' is pressed, calculate the result
        calculate();
    } else { // If an invalid key is pressed, show an error
        lcd_cmd(0x01);
        lcd_string("Invalid Input");
        delay(200);
        reset_calc();
    }
}

// Function to scan the keypad rows and columns to detect the pressed key
void row_scan() {
    const unsigned char keys[4][4] = {   // Define the keypad layout
        {'7', '8', '9', '/'},
        {'4', '5', '6', 'x'},
        {'1', '2', '3', '-'},
        {'C', '0', '=', '+'}
    };
    unsigned char row, col;
    for (row = 0; row < 4; row++) {         // Loop through the rows
        P1 = ~(1 << row);                  // Activate the current row
        for (col = 0; col < 4; col++) {    // Loop through the columns
            if (!(P1 & (1 << (col + 4)))) { // If a key is pressed in the current row and column
                delay(32);               // Debounce the key
                while (!(P1 & (1 << (col + 4)))); // Wait for the key release
                process_key(keys[row][col]); // Process the key press
                return;
            }
        }
    }
}

// Main function
void main() {
    P1 = 0xF0;             // Set the upper nibble of Port 1 to 1 (input) and the lower nibble to 0 (output)
    lcd_init();            // Initialize the LCD
    lcd_string("Tatiana"); // Display "Tatiana" on the first row of the LCD
    lcd_cmd(0xc0);         // Move the cursor to the second row of the LCD
    lcd_string("Kazakevich"); // Display "Kazakevich" on the second row
    delay(200);            // Delay for a brief moment
    lcd_cmd(0x01);         // Clear the LCD screen

    while (1) row_scan();  // Continuously scan the keypad for input
}
