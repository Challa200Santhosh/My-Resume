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
float code adc_to_voltage[256] = {
    0.0000, 0.0980, 0.1961, 0.2941, 0.3922, 0.4902, 0.5882, 0.6863, 0.7843, 0.8824,
		0.9804, 1.0784, 1.1765, 1.2745, 1.3725, 1.4706, 1.5686, 1.6667, 1.7647, 1.8627,
		1.9608, 2.0588, 2.1569, 2.2549, 2.3529, 2.4510, 2.5490, 2.6471, 2.7451, 2.8431,
		2.9412, 3.0392, 3.1373, 3.2353, 3.3333, 3.4314, 3.5294, 3.6275, 3.7255, 3.8235,
		3.9216, 4.0196, 4.1176, 4.2157, 4.3137, 4.4118, 4.5098, 4.6078, 4.7059, 4.8039,
		4.9020, 5.0000, 5.0980, 5.1961, 5.2941, 5.3922, 5.4902, 5.5882, 5.6863, 5.7843,
		5.8824, 5.9804, 6.0784, 6.1765, 6.2745, 6.3725, 6.4706, 6.5686, 6.6667, 6.7647,
		6.8627, 6.9608, 7.0588, 7.1569, 7.2549, 7.3529, 7.4510, 7.5490, 7.6471, 7.7451,
		7.8431, 7.9412, 8.0392, 8.1373, 8.2353, 8.3333, 8.4314, 8.5294, 8.6275, 8.7255,
		8.8235, 8.9216, 9.0196, 9.1176, 9.2157, 9.3137, 9.4118, 9.5098, 9.6078, 9.7059,
		9.8039, 9.9020, 10.0000, 10.0980, 10.1961, 10.2941, 10.3922, 10.4902, 10.5882,
		10.6863, 10.7843, 10.8824, 10.9804, 11.0784, 11.1765, 11.2745, 11.3725, 11.4706,
		11.5686, 11.6667, 11.7647, 11.8627, 11.9608, 12.0588, 12.1569, 12.2549, 12.3529,
		12.4510, 12.5490, 12.6471, 12.7451, 12.8431, 12.9412, 13.0392, 13.1373, 13.2353,
		13.3333, 13.4314, 13.5294, 13.6275, 13.7255, 13.8235, 13.9216, 14.0196, 14.1176,
		14.2157, 14.3137, 14.4118, 14.5098, 14.6078, 14.7059, 14.8039, 14.9020, 15.0000,
		15.0980, 15.1961, 15.2941, 15.3922, 15.4902, 15.5882, 15.6863, 15.7843, 15.8824,
		15.9804, 16.0784, 16.1765, 16.2745, 16.3725, 16.4706, 16.5686, 16.6667, 16.7647,
		16.8627, 16.9608, 17.0588, 17.1569, 17.2549, 17.3529, 17.4510, 17.5490, 17.6471,
		17.7451, 17.8431, 17.9412, 18.0392, 18.1373, 18.2353, 18.3333, 18.4314, 18.5294,
		18.6275, 18.7255, 18.8235, 18.9216, 19.0196, 19.1176, 19.2157, 19.3137, 19.4118,
		19.5098, 19.6078, 19.7059, 19.8039, 19.9020, 20.0000, 20.0980, 20.1961, 20.2941,
		20.3922, 20.4902, 20.5882, 20.6863, 20.7843, 20.8824, 20.9804, 21.0784, 21.1765,
		21.2745, 21.3725, 21.4706, 21.5686, 21.6667, 21.7647, 21.8627, 21.9608, 22.0588,
		22.1569, 22.2549, 22.3529, 22.4510, 22.5490, 22.6471, 22.7451, 22.8431, 22.9412,
		23.0392, 23.1373, 23.2353, 23.3333, 23.4314, 23.5294, 23.6275, 23.7255, 23.8235,
		23.9216, 24.0196, 24.1176, 24.2157, 24.3137, 24.4118, 24.5098, 24.6078, 24.7059,
		24.8039, 24.9020, 25.0000
};

void main(void) {
    unsigned char adc_value;
    float voltage;
    
    lcd_init();  // Initialize the LCD
    lcd_display_string("Voltage: ");
    
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
    sprintf(buffer, "%1.4fV", voltage);  // Format voltage with 4 decimal places
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