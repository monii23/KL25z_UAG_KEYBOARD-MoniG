/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */

#define GPIOB_EN_MASK (1<<2)
#define GPIOB_RS_MASK (1)
#define GPIOB_RW_MASK (1<<1)
#define GPIOB_MASK 7
#define GPIOD_DATA_MASK (0xFF)
#define GPIOC_DATA_MASK (0xFF0)
#define CLEAR             (0x01)
#define TURN_ON           (0X0C)
#define LINE_FEED         (0xC0)
#define SECOND_LINE_EN    (0x38)
#define INCREASE_CURSOR   (0x06)
#define DECREASE_CURSOR   (0x04)
#define BACKSPACE         (0x10)
#define DDRAM_ADDRESS_0   (0x80)
#define DDRAM_ADDRESS_1   (0x81)
#define DDRAM_ADDRESS_15  (0x8F)
#define DDRAM_ADDRESS_64  (0xC0)
#define DDRAM_ADDRESS_67  (0xC3)
#define DDRAM_ADDRESS_71  (0xC7)
#define DDRAM_ADDRESS_74  (0xCA)
#define DDRAM_ADDRESS_79  (0xCF)
#define CGRAM_ADDRESS_0   (0x40)
#define SPACE             (0x20)
#define UP                (0x80)
#define DOWN              (0xC0)
#define HOME              (0x02)
#define CURSOR_ON 		  (0xD)
#define CURSOR_OFF        (0xC)
#define WAIT_TIME (0xFFF)
#define INS (1)
#define CHAR (0)


char Receive,Write=0,Release=0,Mayusk=0,Shiftk=0,Comandk=0,Count=0;
int Offset = 0,contadorf=0,contadorpos=0;
int Delay=WAIT_TIME;
char Key[49]= 
{
	0x0E,0x16,0x1E,0x26,0x25,0x2E,0x36,0x3D,0x3E,0x46,0x45,0x4E,0x55,
	0x15,0x1D,0x24,0x2D,0x2C,0x35,0x3C,0x43,0x44,0x4D,0x54,0x5B,
	0x1C,0x1B,0x23,0x2B,0x34,0x33,0x3B,0x42,0x4B,0x4C,0x52,0x5D,
	0x61,0x1A,0x22,0x21,0x2A,0x32,0x31,0x3A,0x41,0x49,0x4A,0x29
};

char Lower[49]=
{
	"|1234567890'¿qwertyuiop´+asdfghjklñ{}<zxcvbnm,.- "
};

char Mayus[49]=
{
	"|1234567890'¿QWERTYUIOP´+ASDFGHJKLÑ{}<ZXCVBNM,.- "	
};

char ShiftLower[49]=
{
	0xDF,'!','"','#','$','%','&','/','(',')','=','?','¡',
	'Q','W','E','R','T','Y','U','I','O','P','¨','*',
	'A','S','D','F','G','H','J','K','L','Ñ','[',']',
	'>','Z','X','C','V','B','N','M',';',':','_',' '
};

char ShiftMayus[49]=
{
	0xDF,'!','"','#','$','%','&','/','(',')','=','?','¡',
	'q','w','e','r','t','y','u','i','o','p','¨','*',
	'a','d','d','f','g','h','j','k','l','ñ','[',']',
	'>','z','x','c','v','b','n','m',';',':','_',' '
};

int contadores[8]={0,0,0,0,0,0,0};

char vector[8][32]={
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};

void LCD_Init(void);
void Set_RS(void);
void Clear_RS(void);
void Enable(void);
void LCD_Write(char Command, char Data);
void UART_Init(void);
void imprimir(int vector1);
void TecladoU(void);


int main(void)
{
	UART_Init();
	LCD_Init();
	LCD_Write(INS,UP+3);
	LCD_Write(CHAR,'T');
	LCD_Write(CHAR,'E');
	LCD_Write(CHAR,'C');
	LCD_Write(CHAR,'L');
	LCD_Write(CHAR,'A');
	LCD_Write(CHAR,'D');
	LCD_Write(CHAR,'O');
	LCD_Write(INS,DOWN+5);
	LCD_Write(CHAR,'U');
	LCD_Write(CHAR,'A');
	LCD_Write(CHAR,'R');
	LCD_Write(CHAR,'T');
	while(Delay--); 
	LCD_Write(INS,CLEAR);
	
	
	for(;;) {	   
	   TecladoU();
	}
	
	return 0;
}

void TecladoU(void)
{
	
	
	if(Write)
	{
		if(Count<32)
		{	
			while((Receive  !=  Key[Offset]) &&  (Offset<49))
			{
				Offset++;
			}
			
			if(Mayusk&&Shiftk)
			{
				LCD_Write(CHAR,ShiftMayus[Offset]);
				vector[contadorpos][contadores[contadorf]]=ShiftMayus[Offset];
			}
			
			else if(Mayusk)
			{
				LCD_Write(CHAR,Mayus[Offset]);
				vector[contadorpos][contadores[contadorf]]=ShiftMayus[Offset];
			}
			
			else if(Shiftk)
			{
				LCD_Write(CHAR,ShiftLower[Offset]);
				vector[contadorpos][contadores[contadorf]]=ShiftMayus[Offset];
			}
			
			else
			{
				LCD_Write(CHAR,Lower[Offset]);
				vector[contadorpos][contadores[contadorf]]=ShiftMayus[Offset];
			}
			
			Count++;
			
			if(Count==16)
			{
				LCD_Write(INS,DOWN);
			}
			contadores[contadorf]++;
			if(Count==31)
			{
			LCD_Write(INS,CLEAR);
			Count=0;
			contadorpos++;
			contadorf++;
			}
			
			
			
		}

		Offset = 0;
		Write  = 0;
	}
	
}



void UART(void)
{
	
	if(Release)
	{
		Release = 0;
		Receive = UART0_D;
		if((Receive == 0x12) || (Receive == 0x59))
		{
			Shiftk = 0;
			
		}
	}
	else
	{
		Receive = UART0_D;
		
		if(Receive == 0xF0)
		{
			Release = 1;
		}
		
		else if((Receive == 0x12) || (Receive == 0x59))
		{
			Shiftk = 1;
		}
		
		else if(Receive == 0x58)
		{
			if(Mayusk)
			{
				Mayusk = 0;
			}
			
			else
			{
				Mayusk = 1;
			}
		}
		
		else if(Receive == 0x66)
		{
			if(Count)
			{
				LCD_Write(INS,BACKSPACE);
				LCD_Write(CHAR,' ');
				LCD_Write(INS,BACKSPACE);
				Count--;
			}
			
			if(Count==15)
			{
				LCD_Write(INS,UP+15);
				LCD_Write(CHAR,' ');
				LCD_Write(INS,BACKSPACE);
			}
		}
		
		else if(Receive == 0x5A)
		{
			LCD_Write(INS,DOWN);
			Count = 16;
		}
		
		else if(Receive == 0x05)
				{
					contadorf=0;
					contadorpos=0;
					imprimir(0);
				}
		else if(Receive == 0x06)
						{
							contadorf=1;
							contadorpos=1;
							imprimir(1);
							LCD_Write(INS,UP);
							Count = 0;
						}
		else if(Receive == 0x04)
						{
			LCD_Write(INS,CLEAR);
							contadorf=2;
							contadorpos=2;
							imprimir(2);
							LCD_Write(INS,UP);
							Count = 0;
						}
		else if(Receive == 0x0C)
						{
			LCD_Write(INS,CLEAR);
							contadorf=3;
							contadorpos=3;
							imprimir(3);
							LCD_Write(INS,UP);
							Count = 0;
						}
		else if(Receive == 0x03)
						{
							contadorf=4;
							contadorpos=4;
							imprimir(4);
							LCD_Write(INS,UP);
							Count = 0;
						}
		else if(Receive == 0x0B)
						{
							contadorf=5;
							contadorpos=5;
							imprimir(5);
							LCD_Write(INS,UP);
							Count = 0;
						}
		else if(Receive == 0x83)
						{
							contadorf=6;
							contadorpos=6;
							imprimir(6);
							LCD_Write(INS,UP);
							Count = 0;
						}
		else if(Receive == 0x0A)
						{
							contadorf=7;
							contadorpos=7;
							imprimir(7);
							LCD_Write(INS,UP);
							Count = 0;
						}
		else if(Receive == 0x01)
								{
			LCD_Write(INS,CLEAR);
			LCD_Write(INS,UP);
								}
		
		else
		{
			Write=1;
		}
	}

}

void imprimir(int vector1)
{
int x;
char var;
LCD_Write(INS,UP);

for(x=0;x<=31;x++)
{
	var=vector[vector1][x];
	LCD_Write(CHAR,var);
	
	if(x==16) 
	{
		LCD_Write(INS,DOWN);
	}
}
Count = 0;
}


void UART_Init(void)
{
	SIM_SOPT2 = SIM_SOPT2_UART0SRC(1);
	SIM_SCGC4 = SIM_SCGC4_UART0_MASK;
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK;
	
	UART0_BDL = UART0_BDL_SBR(100);
	UART0_C1 = UART0_C1_PE_MASK | UART0_C1_PT_MASK;
	UART0_C2 = UART0_C2_RE_MASK | UART0_C2_RIE_MASK;
	
	NVIC_ISER = 1<<12;
	
	PORTA_PCR1= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK; 
	PORTA_PCR2= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK;
}

void LCD_Init(void)
{
	
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;
	
	
		PORTB_PCR0= PORT_PCR_MUX(1);
		PORTB_PCR1= PORT_PCR_MUX(1);
		PORTB_PCR2= PORT_PCR_MUX(1);
		PORTC_PCR4= PORT_PCR_MUX(1);
		PORTC_PCR5= PORT_PCR_MUX(1);
		PORTC_PCR6= PORT_PCR_MUX(1);      
		PORTC_PCR7= PORT_PCR_MUX(1);
		PORTC_PCR8= PORT_PCR_MUX(1);
		PORTC_PCR9= PORT_PCR_MUX(1);
		PORTC_PCR10= PORT_PCR_MUX(1);
		PORTC_PCR11= PORT_PCR_MUX(1);
	
	GPIOB_PDDR=GPIOB_EN_MASK | GPIOB_RS_MASK | GPIOB_RW_MASK; 
	GPIOC_PDDR=GPIOC_DATA_MASK; 
	
	LCD_Write(INS,CLEAR); 
	LCD_Write(INS,0x0E);
	LCD_Write(INS,SECOND_LINE_EN);

}


void Set_RS(void)
{
	GPIOB_PSOR=GPIOB_RS_MASK; 
}


void Clear_RS(void)
{
	GPIOB_PCOR=GPIOB_RS_MASK; 
}


void Enable(void)
{
	short hwDelay=WAIT_TIME;
	
	GPIOB_PSOR=GPIOB_EN_MASK;  
	while(hwDelay--);  
	GPIOB_PCOR=GPIOB_EN_MASK; 
}


void LCD_Write(char bINS, char bData)
{
	if(bINS) 
	{
		Clear_RS();  
		GPIOC_PDOR=bData<<4;
		Enable();    
		Set_RS(); 
	}
	else 
	{
		GPIOC_PDOR=bData<<4;
		Enable();
	}
}

