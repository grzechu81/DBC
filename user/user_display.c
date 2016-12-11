#include "user_display.h"
#include "driver/spi.h"

void send_data(uint8 data);
void send_command(uint8 cmd);
void hw_init();
void wait_for_spi();
void set_spi_mode(uint8 spi_cpha, uint8 spi_cpol);

void user_display_init()
{
	hw_init();

	os_delay_us(200000); //500ms

	//wake up call
	send_command(0b00110000);
	os_delay_us(5000); //5ms
	send_command(0b00110000);
	os_delay_us(5000); //5ms
	send_command(0b00110000);
	os_delay_us(5000); //5ms

	//setup
	send_command(CMD_FUNCTION_SET);
	os_delay_us(500000); //100ms
	send_command(CMD_DISPLAY_ON);
	send_command(CMD_CLEAR);
	send_command(CMD_ENTRY_MODE);
	send_command(CMD_HOME);
}

void user_display_on()
{
	send_command(CMD_DISPLAY_ON);
}

void user_display_off()
{
	send_command(CMD_DISPLAY_OFF);
}

void user_display_clear()
{
	send_command(CMD_CLEAR);
	os_delay_us(10000); //500ms
	send_command(CMD_HOME);
}

void user_display_test()
{
	send_data('1');
	send_data('2');
	send_data('3');
	send_data('4');
}

void send_command(uint8 cmd)
{
	uint32 buffer = 0;
	buffer = cmd;

	wait_for_spi();
	//T_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_MOSI);
	//WRITE_PERI_REG(SPI_W0(HSPI), buffer<<(32-10));
	
	WRITE_PERI_REG(SPI_USER2(HSPI), (((1&SPI_USR_COMMAND_BITLEN)<<SPI_USR_COMMAND_BITLEN_S) | 0x0 << 6));	

	WRITE_PERI_REG(SPI_W0(HSPI), cmd<<(32-8));
	SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);
}

void send_data(uint8 data)
{
	uint32 buffer = 0;

	buffer = (0x2 << 8) | data;

	wait_for_spi();
	//T_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_MOSI);
	//WRITE_PERI_REG(SPI_W0(HSPI), buffer<<(32-10));

	WRITE_PERI_REG(SPI_USER2(HSPI), (((1&SPI_USR_COMMAND_BITLEN)<<SPI_USR_COMMAND_BITLEN_S) | 0x2 << 6));	

	WRITE_PERI_REG(SPI_W0(HSPI), data<<(32-8));
	SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);
}

void wait_for_spi()
{
	while(READ_PERI_REG(SPI_CMD(HSPI))&SPI_USR);
}

void hw_init()
{
	//Pin setup
	WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105); //Set bit 9 if 80MHz sysclock required
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2); //GPIO12 is HSPI MISO pin (Master Data In)
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2); //GPIO13 is HSPI MOSI pin (Master Data Out)
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2); //GPIO14 is HSPI CLK pin (Clock)
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2); //GPIO15 is HSPI CS pin (Chip Select / Slave Select)


	//Clock setup - 1MHz
	// WRITE_PERI_REG(SPI_CLOCK(HSPI), 
	// 				( ( 7 & SPI_CLKDIV_PRE ) << SPI_CLKDIV_PRE_S) |
	// 				( ( 9 & SPI_CLKCNT_N ) << SPI_CLKCNT_N_S ) |
	// 				( ( 1 & SPI_CLKCNT_H) << SPI_CLKCNT_H_S ) |
	// 				( ( 6 & SPI_CLKCNT_L ) << SPI_CLKCNT_L_S ) );

	//Clock setup - 1MHz
	WRITE_PERI_REG(SPI_CLOCK(HSPI), 
					( ( 7 & SPI_CLKDIV_PRE ) << SPI_CLKDIV_PRE_S) |
					( ( 9 & SPI_CLKCNT_N ) << SPI_CLKCNT_N_S ) |
					( ( 1 & SPI_CLKCNT_H) << SPI_CLKCNT_H_S ) |
					( ( 6 & SPI_CLKCNT_L ) << SPI_CLKCNT_L_S ) );


	//Tx byte order high to low
	SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_WR_BYTE_ORDER);

	// //Rx byte order high to low
	SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_RD_BYTE_ORDER);

	set_spi_mode(0, 0);
	//set_spi_mode(0, 1);
	// set_spi_mode(1, 0);
	// set_spi_mode(1, 1);

	//disable MOSI, MISO, ADDR, COMMAND, DUMMY in case previously set.
	CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_MOSI|SPI_USR_MISO|SPI_USR_COMMAND|SPI_USR_ADDR|SPI_USR_DUMMY|SPI_FLASH_MODE);

	//enable MOSI function in SPI module
	SET_PERI_REG_MASK(SPI_USER(HSPI),  SPI_CS_SETUP | SPI_CS_HOLD | SPI_USR_COMMAND | SPI_USR_MOSI); 
	WRITE_PERI_REG(SPI_USER1(HSPI), (7 & SPI_USR_MOSI_BITLEN)<<SPI_USR_MOSI_BITLEN_S );
}

void set_spi_mode(uint8 spi_cpha, uint8 spi_cpol)
{
	// switch(mode)
	// {
	// 	case 0:
	// 		SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_WR_BYTE_ORDER);
	// 		CLEAR_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);
	// 		break;
	// 	case 1:
	// 		SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_CK_OUT_EDGE);
	// 		CLEAR_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);
	// 		break;
	// 	case 2:
	// 		SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_CK_OUT_EDGE);
	// 		SET_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);
	// 		break;
	// 	case 3:
	// 	default:
	// 		CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_CK_OUT_EDGE);
	// 		SET_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);		
	// 	break;
	// }

	if(!spi_cpha == !spi_cpol) {
		CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_CK_OUT_EDGE);
	} else {
		SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_CK_OUT_EDGE);
	}

	if (spi_cpol) {
		SET_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);
	} else {
		CLEAR_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);
	}
}

