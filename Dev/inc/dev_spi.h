#ifndef __DEV_SPI
#define __DEV_SPI

#include "dev_io.h"

/**********************************************************
										SPI PIN SET
***********************************************************						
1.MISO	--	PD15
2.MOSI	--	PG2
3.SCK		-- 	PG3
4.NSS		--	PG4
***********************************************************/

#define SPI1_SW_MISO	PDin(15)
#define SPI1_SW_MOSI 	PGout(2)
#define SPI1_SW_SCK		PGout(3)
#define SPI1_SW_NSS		PGout(4)

/**********************************************************
										SDCARD PIN SET
***********************************************************						
1.SDCARD_MOSI	--	PA7
2.SDCARD_MISO	-- 	PA6
3.SDCARD_SCK	--	PA5
4.SDCARD_CS		--	PA4
***********************************************************/

#define SDCARD_CS		PAout(4)

void Spi_SW_Init(void);
void Spi1_SW_Init(void);
void Spi_HW_Init(void);
void SPI1_SetSpeed(uchar SPI_BaudRatePrescaler);
uchar SPI1_ReadWriteByte(uchar TxData);

void Spi1_SW_Wr(ushort addr, ushort data);
ushort Spi1_SW_Rd(ushort addr);

#endif



























