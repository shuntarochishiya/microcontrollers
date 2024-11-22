#define lcd P2 // Define 'lcd' as an alias for Port 2, which will be used to send data and commands to the LCD

sbit rs = P3^0; // Define 'rs' as the bit P3.0, used to select command (0) or data (1) mode
sbit rw = P3^1; // Define 'rw' as the bit P3.1, used to set read (1) or write (0) mode
sbit en = P3^2; // Define 'en' as the bit P3.2, used as an enable pin for LCD communication

// Function declarations for LCD operations
void lcd_init(); // Initialize LCD function
void lcd_cmd(unsigned char); // Send command to LCD
void lcd_dat(unsigned char); // Send data to LCD
void msdelay(unsigned char); // Delay function for timing
void lcd_string(unsigned char *); // Send a string to be displayed on the LCD

// LCD initialization function
void lcd_init()
{
	lcd_cmd(0x80); // Set cursor to the first line, first position
	lcd_cmd(0x38); // Function set: 2 lines, 5x7 matrix
	lcd_cmd(0x01); // Clear the LCD display
	lcd_cmd(0x0c); // Display ON, cursor OFF
	lcd_cmd(0x06); // Entry mode set: move cursor right after each character
}

// Send command to the LCD
void lcd_cmd(unsigned char ch)
{
	lcd = ch; // Place the command in the 'lcd' port (P2)
	rs = 0; // Set 'rs' to 0, selecting command mode
	rw = 0; // Set 'rw' to 0, selecting write mode
	en = 1; // Set 'en' to 1, starting the enable pulse
	msdelay(20); // Wait for a short delay to let LCD process the command
	en = 0; // Set 'en' to 0, completing the enable pulse
}

// Send data to the LCD
void lcd_dat(unsigned char ch)
{
	lcd = ch; // Place the data in the 'lcd' port (P2)
	rs = 1; // Set 'rs' to 1, selecting data mode
	rw = 0; // Set 'rw' to 0, selecting write mode
	en = 1; // Set 'en' to 1, starting the enable pulse
	msdelay(20); // Wait for a short delay to let LCD process the data
	en = 0; // Set 'en' to 0, completing the enable pulse
}

// Delay function
void msdelay(unsigned char t)
{
	unsigned int i, j; // Loop variables for delay generation
	for(i = 0; i < t; i++) // Outer loop repeats for 't' milliseconds
		for(j = 0; j < 1275; j++); // Inner loop generates a small delay
}

// Send a string of characters to be displayed on the LCD
void lcd_string(unsigned char *p)
{
	while(*p != '\0') // Loop until the null terminator is reached
	{
		lcd_dat(*p); // Send the current character to the LCD
		p++; // Move to the next character in the string
	}
}
