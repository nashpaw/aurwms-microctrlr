#define H 500   // device hieght from reservoir bottom level

//defining variables
char text[7];
int i,level,distance;
int avg,temp = 0;
int address=10;
int lastlevel=0;
unsigned int temp_res[5];

const unsigned char para[5]="AT\r\n";
unsigned char textmode[12]="AT+CMGF=1\r\n";               //select TEXT mode
unsigned char smsc[24] ="AT+CSCA=\"+947100003\"\r\n" ;    //message center no of the network
unsigned char recepient[24]="AT+CMGS=\"+94719301732\"\r"; //Destination Number



void sendSMS(int x);

void display(char *text)
{
	//  Lcd_Init(&PORTD);  // Initialize LCD on PORTD
	//	Lcd_Cmd(Lcd_CURSOR_OFF); // Turn off cursor
	Lcd_Out(1, 1, text);
}

void main() {

	TRISD = 0;
	ADCON1 = 0x80;  // Configure analog inputs and Vref
	TRISA  = 0xFF;  // PORTA is input

	Lcd_Init(&PORTD);  // Initialize LCD on PORTD
	Lcd_Cmd(Lcd_CURSOR_OFF); // Turn off cursor

	Usart_Init(9600);

	for(i=0;i<12;i++)
	{
		Usart_Write(textmode[i]);
	}
	Delay_ms(100);

	/* Sets the SERVICE CENTER NO */
	for(i=0;i<24; i++)
	{
		Usart_Write(smsc[i]);
	}
	Delay_ms(100);

	Eeprom_Write(address, 0);

	while(1)
	{
		for (i=0;i<5;i++)
		{
			temp_res[i] = Adc_Read(0);
			temp += temp_res[i];
			Delay_1sec();
		}

		avg = (int)(temp/5);

		distance = (int)(avg/2);
		level = H - distance;

		IntToStr(level, text);


		lastlevel = Eeprom_Read(address);
		Delay_ms(20);
		if (((lastlevel +1) < level) || ((lastlevel -1)>level))
		{
			Eeprom_Write(address, level);
			sendSMS(level);
			Delay_ms(100);
			display(text);
		}
		for (i=0;i<30;i++)
		{
			Delay_1sec();
		}

	}
}

void sendSMS(int x)
{

	//  Usart_Init(9600);
	//
	//		for(i=0;i<5; i++){
	//			Usart_Write(para[i]);//Test AT command
	//		}
	//		/*set textmode for sending SMS*/
	//		for(i=0;i<12;i++)
	//		{
	//			Usart_Write(textmode[i]);
	//		}
	//		Delay_ms(100);
	//
	//		/* Sets the SERVICE CENTER NO */
	//		for(i=0;i<24; i++)
	//		{
	//			Usart_Write(smsc[i]);
	//		}
	//		Delay_ms(100);


	/* Sets the RECEPIENT NO */
	for(i=0;i<24;i++)
	{
		Usart_Write(recepient[i]);
	}

	Delay_ms(50);

	/* send message */
	Usart_Write(x);

	Delay_ms(50);

	Usart_Write(0x1A); //ctrl+z
	Delay_ms(50);
	Usart_Write(0x0D); //enter
}
