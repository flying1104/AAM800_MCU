#ifndef __DEV_PIN_CONF
#define __DEV_PIN_CONF

#define LED_ON	0
#define LED_OFF	1

/**********************************************************
										UART PIN SET
***********************************************************

1.UART2	RX	--	PA10
2.UART2 TX	--	PA9

***********************************************************/

#define UART2_TX_PIN	GPIO_Pin_9
#define UART2_TX_GPIO 	GPIOA
#define UART2_TX_CLK	RCC_AHB1Periph_GPIOA

#define UART2_RX_PIN	GPIO_Pin_10
#define UART2_RX_GPIO	GPIOA
#define UART2_RX_CLK	RCC_AHB1Periph_GPIOA


/**********************************************************
					BASE PIN SET
***********************************************************

1.LED	BASE	--	PD9
2.SYSTEM RST 	-- 	PA6

***********************************************************/

#define LED_BASE_PIN			GPIO_Pin_9
#define LED_BASE_GPIO 			GPIOD
#define LED_BASE_GPIO_CLK		RCC_AHB1Periph_GPIOD

#define SYSTEM_RST_PIN			GPIO_Pin_6
#define SYSTEM_RST_GPIO 		GPIOA
#define SYSTEM_RST_GPIO_CLK		RCC_AHB1Periph_GPIOA

/**********************************************************
					I2C PIN SET
***********************************************************						
1.SCL	--	PB6
2.SDA	--	PB7	
***********************************************************/

#define SCL_PIN GPIO_Pin_6	
#define SDA_PIN	GPIO_Pin_7

#define SCL_GPIO	GPIOB
#define SDA_GPIO	GPIOB
#define I2C_GPIO	GPIOB

#define SCL_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define SDA_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define I2C_GPIO_CLK	RCC_AHB1Periph_GPIOB

/**********************************************************
										LCD1602 PIN SET
***********************************************************						
1.LCD_PWR			--	PE4
2.LCD_BL_PWR	--	PF8	
3.LCD_RS			--	PE5
4.LCD_RW			--	PE6
5.LCD_EN			--	PC13
6.LCD_D0			--	PF0
7.LCD_D1			--	PF1
8.LCD_D2			--	PF2
9.LCD_D3			--	PF3
10.LCD_D4			--	PF4
11.LCD_D5			--	PF5
12.LCD_D6			--	PF6
13.LCD_D7			--	PF7
***********************************************************/

#define LCD_PWR_PIN GPIO_Pin_4	
#define LCD_BL_PWR_PIN	GPIO_Pin_8
#define LCD_RS_PIN	GPIO_Pin_5
#define LCD_RW_PIN	GPIO_Pin_6
#define LCD_EN_PIN	GPIO_Pin_13
#define LCD_D0_PIN	GPIO_Pin_0
#define LCD_D1_PIN	GPIO_Pin_1
#define LCD_D2_PIN	GPIO_Pin_2
#define LCD_D3_PIN	GPIO_Pin_3
#define LCD_D4_PIN	GPIO_Pin_4
#define LCD_D5_PIN	GPIO_Pin_5
#define LCD_D6_PIN	GPIO_Pin_6
#define LCD_D7_PIN	GPIO_Pin_7
#define LCD_DATA_PIN (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 \
											| GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)

#define LCD_PWR_GPIO		GPIOE
#define LCD_BL_PWR_GPIO	GPIOF
#define LCD_RS_GPIO			GPIOE
#define LCD_RW_GPIO			GPIOE
#define LCD_EN_GPIO			GPIOC
#define LCD_DA_GPIO			GPIOF

#define LCD_PWR_GPIO_CLK		RCC_AHB1Periph_GPIOE
#define LCD_BL_PWR_GPIO_CLK	RCC_AHB1Periph_GPIOF
#define LCD_RS_GPIO_CLK			RCC_AHB1Periph_GPIOE
#define LCD_RW_GPIO_CLK			RCC_AHB1Periph_GPIOE
#define LCD_EN_GPIO_CLK			RCC_AHB1Periph_GPIOC
#define LCD_DA_GPIO_CLK			RCC_AHB1Periph_GPIOF 

/**********************************************************
										SPI PIN SET
***********************************************************						
1.MISO	--	PD15
2.MOSI	--	PG2
3.SCK		-- 	PG3
4.NSS		--	PG4
***********************************************************/

#define SPI1_SW_MISO_PIN	GPIO_Pin_15
#define SPI1_SW_MOSI_PIN	GPIO_Pin_2
#define SPI1_SW_SCK_PIN		GPIO_Pin_3
#define SPI1_SW_NSS_PIN		GPIO_Pin_4

#define SPI1_SW_GPIO_CLK_D	RCC_AHB1Periph_GPIOD
#define SPI1_SW_GPIO_CLK_G	RCC_AHB1Periph_GPIOG

#define SPI1_SW_GPIO_D	GPIOD
#define SPI1_SW_GPIO_G	GPIOG

/**********************************************************
										KEY PIN SET
***********************************************************						
1.KEY0	--	PE15			 	BYPASS		
2.KEY1	--	PE14			 	MUTE
3.KEY2	-- 	PE13			 	COUGH
4.KEY3	--	PE12				2~8S
5.KEY4	--	PE11				DUMP
6.KEY5	--	PE10				EXIT
7.KEY6	--	PE9					START
8.KEY7	--	PE8					DOWN
9.KEY8	--	PE7					ENTER	
10.KEY9	--	PG1					UP
***********************************************************/

#define KEY0_PIN				GPIO_Pin_15
#define KEY0_GPIO 			GPIOE
#define KEY0_GPIO_CLK		RCC_AHB1Periph_GPIOE

#define KEY1_PIN				GPIO_Pin_14
#define KEY1_GPIO 			GPIOE
#define KEY1_GPIO_CLK		RCC_AHB1Periph_GPIOE

#define KEY2_PIN				GPIO_Pin_13
#define KEY2_GPIO 			GPIOE
#define KEY2_GPIO_CLK		RCC_AHB1Periph_GPIOE

#define KEY3_PIN				GPIO_Pin_12
#define KEY3_GPIO 			GPIOE
#define KEY3_GPIO_CLK		RCC_AHB1Periph_GPIOE

#define KEY4_PIN				GPIO_Pin_11
#define KEY4_GPIO 			GPIOE
#define KEY4_GPIO_CLK		RCC_AHB1Periph_GPIOE

#define KEY5_PIN				GPIO_Pin_10
#define KEY5_GPIO 			GPIOE
#define KEY5_GPIO_CLK		RCC_AHB1Periph_GPIOE

#define KEY6_PIN				GPIO_Pin_9
#define KEY6_GPIO 			GPIOE
#define KEY6_GPIO_CLK		RCC_AHB1Periph_GPIOE

#define KEY7_PIN				GPIO_Pin_8
#define KEY7_GPIO 			GPIOE
#define KEY7_GPIO_CLK		RCC_AHB1Periph_GPIOE

#define KEY8_PIN				GPIO_Pin_7
#define KEY8_GPIO 			GPIOE
#define KEY8_GPIO_CLK		RCC_AHB1Periph_GPIOE

#define KEY9_PIN				GPIO_Pin_1
#define KEY9_GPIO 			GPIOG
#define KEY9_GPIO_CLK		RCC_AHB1Periph_GPIOG

#define KEY_GPIOE_PIN		KEY0_PIN | KEY1_PIN | KEY4_PIN | KEY5_PIN | KEY6_PIN | KEY7_PIN | KEY8_PIN
#define KEY_GPIOE				GPIOE
#define KEY_GPIOE_CLK		RCC_AHB1Periph_GPIOE

#define KEY_GPIOG_PIN		KEY9_PIN
#define KEY_GPIOG				GPIOG
#define KEY_GPIOG_CLK		RCC_AHB1Periph_GPIOG

#define KEY_LED_GPIO_PIN	KEY2_PIN | KEY3_PIN
#define KEY_LED_GPIO			GPIOE
#define KEY_LED_GPIO_CLK	RCC_AHB1Periph_GPIOE

/**********************************************************
										SDCARD PIN SET
***********************************************************						
1.SDCARD_MOSI	--	PA7
2.SDCARD_MISO	-- 	PA6
3.SDCARD_SCK	--	PA5
4.SDCARD_CS		--	PA4
***********************************************************/

#define SDCARD_SPI_CLK		RCC_APB2Periph_SPI1

#define SDCARD_MOSI_PIN		GPIO_Pin_7
#define SDCARD_MISO_PIN		GPIO_Pin_6
#define SDCARD_SCK_PIN		GPIO_Pin_5
#define SDCARD_CS_PIN			GPIO_Pin_4

#define SDCARD_GPIO_CLK		RCC_APB2Periph_GPIOA

#define SDCARD_GPIO				GPIOA



#endif

