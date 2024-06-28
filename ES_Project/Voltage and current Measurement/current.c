#include <reg51.h>
#include <stdio.h>

// LCD Control Pins
sbit RS = P3^0;
sbit RW = P3^1;
sbit EN = P3^2;

// ADC Control Pins
sbit ALE = P3^3;
sbit OE = P3^4;
sbit START = P3^5;
sbit EOC = P3^6;

void delay(unsigned int time);
void lcd_init(void);
void lcd_command(unsigned char command);
void lcd_data(unsigned char Data);
void lcd_display_string(char *str);
void lcd_display_voltage(float voltage);
unsigned char adc_read(void);

// Lookup table for voltage values stored in code memory
float code adc_to_voltage[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0.3,
 0.6,
 0.8999999999999999,
 1.2,
 1.5,
 1.8,
 2.0999999999999996,
 2.4,
 2.6999999999999997,
 2.9999999999999996,
 3.3,
 3.5999999999999996,
 3.8999999999999995,
 4.2,
 4.5,
 4.8,
 5.1,
 5.3999999999999995,
 5.699999999999999,
 6.0,
 6.3,
 6.6,
 6.8999999999999995,
 7.199999999999999,
 7.499999999999999,
 7.8,
 8.1,
 8.4,
 8.700000000000001,
 9.0,
 9.3,
 9.6,
 9.9,
 10.200000000000001,
 10.5,
 10.8,
 11.1,
 11.4,
 11.700000000000001,
 12.0,
 12.3,
 12.6,
 12.9,
 13.200000000000001,
 13.5,
 13.8,
 14.1,
 14.4,
 14.7,
 15.0,
 15.3,
 15.6,
 15.9,
 16.2,
 16.5,
 16.8,
 17.1,
 17.4,
 17.7,
 18.0,
 18.3,
 18.6,
 18.9,
 19.2,
 19.5,
 19.8,
 20.1,
 20.4,
 20.7,
 21.0,
 21.3,
 21.6,
 21.9,
 22.2,
 22.5,
 22.8,
 23.1,
 23.4,
 23.7,
 24.0,
 24.3,
 24.6,
 24.9,
 25.2,
 25.5,
 25.8,
 26.1,
 26.4,
 26.7,
 27.0,
 27.3,
 27.6,
 27.9,
 28.2,
 28.5,
 28.8,
 29.099999999999998,
 29.4,
 29.7,
 30.0,
};

void main(void) {
    unsigned char adc_value;
    float voltage;
    
    lcd_init();  // Initialize the LCD
    lcd_display_string("Current: ");
    
    while(1) {
        adc_value = adc_read();  // Read ADC value
        voltage = adc_to_voltage[adc_value];  // Get voltage from lookup table

        lcd_command(0xC0);  // Move cursor to the second line of LCD
        lcd_display_voltage(voltage);  // Display the voltage on LCD
    }
}

// Function to initialize the LCD
void lcd_init(void) {
    lcd_command(0x38);  // Initialize LCD in 8-Bit mode
    delay(1);
    lcd_command(0x0C);  // Display ON, Cursor OFF
    delay(1);
    lcd_command(0x06);  // Auto increment cursor
    delay(1);
    lcd_command(0x01);  // Clear display
    delay(1);
    lcd_command(0x80);  // Move cursor to the first line
    delay(1);
}

// Function to send command to LCD
void lcd_command(unsigned char command) {
    P2 = command;
    RS = 0;  // Select command Register
    RW = 0;  // Write operation
    EN = 1;
    delay(1);
    EN = 0;
    delay(1);
}

// Function to send data to LCD
void lcd_data(unsigned char Data) {
    P2 = Data;
    RS = 1;  // Select Data Register
    RW = 0;  // Write operation
    EN = 1;
    delay(1);
    EN = 0;
    delay(1);
}

// Function to display string on LCD
void lcd_display_string(char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}

// Function to display voltage on LCD
void lcd_display_voltage(float voltage) {
    char buffer[16];
    sprintf(buffer, "%1.1f A", voltage);  // Format voltage with 4 decimal places
    lcd_display_string(buffer);
}

// Function to read Data from ADC
unsigned char adc_read(void) {
    unsigned char adc_value;

    ALE = 1;
    START = 1;
    delay(1);
    ALE = 0;
    START = 0;

    while (EOC == 1);  // Wait for end of conversion

    OE = 1;
    adc_value = P1;  // Read ADC value from Port 1
    OE = 0;

    return adc_value;
}

// Function to generate delay
void delay(unsigned int time) {
    unsigned int i, j;
    for(i = 0; i < time; i++) {
        for(j = 0; j < 1275; j++);
    }
}