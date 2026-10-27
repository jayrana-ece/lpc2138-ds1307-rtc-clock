#include <LPC213x.h>

/* ---------------- LCD PIN DEFINITIONS ---------------- */

#define LCD_RS  (1 << 16)
#define LCD_EN  (1 << 17)

#define LCD_D4  (1 << 18)
#define LCD_D5  (1 << 19)
#define LCD_D6  (1 << 20)
#define LCD_D7  (1 << 21)

#define LCD_DATA  (LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)


/* ---------------- DELAY FUNCTION ---------------- */

void delay(unsigned int count)
{
    volatile unsigned int i, j;

    for(i = 0; i < count; i++)
    {
        for(j = 0; j < 5000; j++);
    }
}


/* ---------------- SHORT LCD DELAY ---------------- */

void lcd_delay(void)
{
    volatile unsigned int i;

    for(i = 0; i < 100; i++);
}


/* =====================================================
                    LCD FUNCTIONS
   ===================================================== */

void lcd_nibble(unsigned char data)
{
    IO0CLR = LCD_DATA;

    if(data & 0x01)
        IO0SET = LCD_D4;

    if(data & 0x02)
        IO0SET = LCD_D5;

    if(data & 0x04)
        IO0SET = LCD_D6;

    if(data & 0x08)
        IO0SET = LCD_D7;

    /* Enable pulse */

    IO0SET = LCD_EN;

    lcd_delay();

    IO0CLR = LCD_EN;

    lcd_delay();
}


void lcd_cmd(unsigned char cmd)
{
    IO0CLR = LCD_RS;

    lcd_nibble(cmd >> 4);
    lcd_nibble(cmd & 0x0F);

    lcd_delay();
}


void lcd_data(unsigned char data)
{
    IO0SET = LCD_RS;

    lcd_nibble(data >> 4);
    lcd_nibble(data & 0x0F);

    lcd_delay();
}


void lcd_string(char *str)
{
    while(*str)
    {
        lcd_data(*str);
        str++;
    }
}


void lcd_init(void)
{
    /* Set LCD pins as output */

    IO0DIR |= LCD_RS | LCD_EN | LCD_DATA;

    delay(20);

    /* LCD initialization sequence */

    lcd_cmd(0x02);     /* 4-bit mode */
    lcd_cmd(0x28);     /* 4-bit, 2 line, 5x7 */
    lcd_cmd(0x0C);     /* Display ON, Cursor OFF */
    lcd_cmd(0x06);     /* Increment cursor */
    lcd_cmd(0x01);     /* Clear LCD */

    delay(5);
}


/* =====================================================
                    I2C FUNCTIONS
   ===================================================== */

void I2C0_Init(void)
{
    /*
       LPC2138:

       P0.2 = SCL0
       P0.3 = SDA0
    */

    PINSEL0 &= ~(0x000000F0);

    PINSEL0 |= 0x00000050;

    /* I2C Clock */

    I2C0SCLH = 75;
    I2C0SCLL = 75;

    /* Enable I2C */

    I2C0CONSET = 0x40;
}


void I2C_Start(void)
{
    I2C0CONSET = 0x20;

    I2C0CONCLR = 0x08;

    while(!(I2C0CONSET & 0x08));

    I2C0CONCLR = 0x20;
}


void I2C_Write(unsigned char data)
{
    I2C0DAT = data;

    I2C0CONCLR = 0x08;

    while(!(I2C0CONSET & 0x08));
}


unsigned char I2C_Read(unsigned char ack)
{
    if(ack)
    {
        I2C0CONSET = 0x04;
    }
    else
    {
        I2C0CONCLR = 0x04;
    }

    I2C0CONCLR = 0x08;

    while(!(I2C0CONSET & 0x08));

    return I2C0DAT;
}


void I2C_Stop(void)
{
    I2C0CONSET = 0x10;

    I2C0CONCLR = 0x08;
}


/* =====================================================
                BCD CONVERSION FUNCTIONS
   ===================================================== */

unsigned char DEC_to_BCD(unsigned char value)
{
    return ((value / 10) << 4) | (value % 10);
}


unsigned char BCD_to_DEC(unsigned char value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}


/* =====================================================
                  SET DS1307 TIME
   ===================================================== */

void DS1307_SetTime(void)
{
    I2C_Start();

    /* DS1307 Write Address */

    I2C_Write(0xD0);

    /* Start from register 00 */

    I2C_Write(0x00);

    /* Seconds = 00 */

    I2C_Write(DEC_to_BCD(0));

    /* Minutes = 30 */

    I2C_Write(DEC_to_BCD(30));

    /* Hours = 08 */

    I2C_Write(DEC_to_BCD(8));

    /* Day of week = 6 */

    I2C_Write(DEC_to_BCD(6));

    /* Date = 22 */

    I2C_Write(DEC_to_BCD(22));

    /* Month = 08 */

    I2C_Write(DEC_to_BCD(8));

    /* Year = 2026 */

    I2C_Write(DEC_to_BCD(26));

    I2C_Stop();
}


/* =====================================================
                  READ DS1307
   ===================================================== */

void DS1307_Read(unsigned char *time)
{
    I2C_Start();

    /* DS1307 Write Address */

    I2C_Write(0xD0);

    /* Start from register 00 */

    I2C_Write(0x00);

    /* Repeated START */

    I2C_Start();

    /* DS1307 Read Address */

    I2C_Write(0xD1);

    /* Read Seconds */

    time[0] = I2C_Read(1);

    /* Read Minutes */

    time[1] = I2C_Read(1);

    /* Read Hours */

    time[2] = I2C_Read(1);

    /* Read Day */

    time[3] = I2C_Read(1);

    /* Read Date */

    time[4] = I2C_Read(1);

    /* Read Month */

    time[5] = I2C_Read(1);

    /* Read Year */

    time[6] = I2C_Read(0);

    I2C_Stop();
}


/* =====================================================
                       MAIN
   ===================================================== */

int main(void)
{
    unsigned char time[7];

    unsigned char second;
    unsigned char minute;
    unsigned char hour;

    unsigned char date;
    unsigned char month;
    unsigned char year;

    char buffer[11];


    /* Initialize LCD */

    lcd_init();


    /* Initialize I2C */

    I2C0_Init();


    /* Set initial RTC time */

    DS1307_SetTime();


    while(1)
    {
        /* Read time from DS1307 */

        DS1307_Read(time);


        /* Convert BCD to Decimal */

        second = BCD_to_DEC(time[0] & 0x7F);

        minute = BCD_to_DEC(time[1] & 0x7F);

        hour = BCD_to_DEC(time[2] & 0x3F);

        date = BCD_to_DEC(time[4] & 0x3F);

        month = BCD_to_DEC(time[5] & 0x1F);

        year = BCD_to_DEC(time[6]);


        /* -------- DISPLAY TIME -------- */

        buffer[0] = '0' + (hour / 10);
        buffer[1] = '0' + (hour % 10);

        buffer[2] = ':';

        buffer[3] = '0' + (minute / 10);
        buffer[4] = '0' + (minute % 10);

        buffer[5] = ':';

        buffer[6] = '0' + (second / 10);
        buffer[7] = '0' + (second % 10);

        buffer[8] = '\0';


        lcd_cmd(0x80);

        lcd_string("Time:");
        lcd_string(buffer);


        /* -------- DISPLAY DATE -------- */

        buffer[0] = '0' + (date / 10);
        buffer[1] = '0' + (date % 10);

        buffer[2] = '/';

        buffer[3] = '0' + (month / 10);
        buffer[4] = '0' + (month % 10);

        buffer[5] = '/';

        buffer[6] = '2';
        buffer[7] = '0';

        buffer[8] = '0' + (year / 10);
        buffer[9] = '0' + (year % 10);

        buffer[10] = '\0';


        lcd_cmd(0xC0);

        lcd_string("Date:");
        lcd_string(buffer);


        /* Small delay only */

        delay(1);
    }
}