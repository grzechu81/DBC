// #include "user_display.h"
// #include "driver/spi.h"
// #include "gpio.h"

// void send_text(char* buffer);
// void send_data(uint8 data);
// void send_command(uint8 cmd);
// void hw_init();
// void wait_for_spi();
// void set_spi_mode(uint8 spi_cpha, uint8 spi_cpol);
// // void spi_write_data(uint8 data);
// // void spi_write_rs_rw(bool isCommand);
// void spi_clock_tick();


// void user_display_init()
// {
// 	hw_init();

// 	//setup
// 	send_command(CMD_FUNCTION_SET);
//     send_command(CMD_ENTRY_MODE);
// 	send_command(CMD_HOME);
// 	send_command(CMD_CLEAR);
// 	os_delay_us(30000);
// 	send_command(CMD_DISPLAY_ON);
// }

// void user_display_on()
// {
// 	send_command(CMD_DISPLAY_ON);
// }

// void user_display_off()
// {
// 	send_command(CMD_DISPLAY_OFF);
// }

// void user_display_clear()
// {
// 	send_command(CMD_HOME);
// 	//send_command(CMD_CLEAR);
// 	//os_delay_us(7000); //7ms
// }

// void user_display_test1()
// {
// 	send_command(0xc0);
// 	send_data('1');
// 	send_command(0x80);
// 	send_data('2');
// }

// void user_display_test2()
// {
// 	// send_text("test");
// 	send_data('2');
// }

// void send_command(uint8 cmd)
// {
// 	// GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 0);
// 	// spi_write_rs_rw(true);
// 	// spi_write_data(cmd);
// 	// GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 1);

// 	// os_delay_us(100); //100us

// 	//wait_for_spi();
// 	//SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_MOSI);
// 	//WRITE_PERI_REG(SPI_W0(HSPI), buffer<<(32-10));
	
// 	// WRITE_PERI_REG(SPI_USER2(HSPI), (((1&SPI_USR_COMMAND_BITLEN)<<SPI_USR_COMMAND_BITLEN_S) | 0x0 << 6));	

// 	// WRITE_PERI_REG(SPI_W0(HSPI), cmd<<(32-8));
// 	// SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);

// 	uint8 mask = 0x80;

// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 0);
// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 0);
// 	spi_clock_tick();
// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 0);
// 	spi_clock_tick();

// 	do  
// 	{
// 		if (cmd & mask)                			  		//Clock out current bit onto SPI Out line
// 		{
// 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 1);
// 		}
// 		else
// 		{
// 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 0);
// 		}  

//         spi_clock_tick();        				       
//         mask = mask >> 1;           				
//     } 
//     while (mask != 0);

// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 1);
// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 1);

// 	os_delay_us(1);
// }

// void send_data(uint8 data)
// {
// 	// uint32 buffer = 0;

// 	// buffer = (0x2 << 8) | data;
// 	// GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 0);
// 	// spi_write_rs_rw(false);
// 	// spi_write_data(data);
// 	// GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 1);

// 	// os_delay_us(100); //1ms

// 	//wait_for_spi();
// 	//SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_MOSI);
// 	//WRITE_PERI_REG(SPI_W0(HSPI), buffer<<(32-10));

// 	// WRITE_PERI_REG(SPI_USER2(HSPI), (((1&SPI_USR_COMMAND_BITLEN)<<SPI_USR_COMMAND_BITLEN_S) | 0x2 << 6));	

// 	// WRITE_PERI_REG(SPI_W0(HSPI), data<<(32-8));
// 	// SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);

// 	uint8 mask = 0x80;

// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 0);
// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 1);
// 	spi_clock_tick();
// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 0);
// 	spi_clock_tick();

// 	do  
// 	{
// 		if (data & mask)                			  		//Clock out current bit onto SPI Out line
// 		{
// 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 1);
// 		}
// 		else
// 		{
// 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 0);
// 		}  

//         spi_clock_tick();        				       
//         mask = mask >> 1;           				
//     } 
//     while (mask != 0);

// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 1);
// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 1);

// 	os_delay_us(1);
// }

// uint8 read_busy_flag()
// {
// 	uint8 mask = 0x80;
// 	uint8 ret = 0;

// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 0);

// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 0);
// 	spi_clock_tick();

// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 1);
// 	spi_clock_tick();

// 	do  
// 	{
// 		if (GPIO_INPUT_GET(SPI_IN_N))
// 		{
// 			ret |= mask; 	
// 		} 

//         spi_clock_tick();        				       
//         mask = mask >> 1;           				
//     } 
//     while (mask != 0);


// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 1);

// 	return ret;
// }

// void send_text(char* buffer)
// {
// 	// GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 0);
// 	// spi_write_rs_rw(false);

// 	while(*buffer != 0x00)
// 	{
// 		send_data(*buffer++);
// 	}
// 	// GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CS_N), 1);
// }

// void wait_for_spi()
// {
// 	while(READ_PERI_REG(SPI_CMD(HSPI))&SPI_USR);
// }

// void hw_init()
// {
// 	// //Pin setup
// 	// WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105); //Set bit 9 if 80MHz sysclock required
// 	// PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2); //GPIO12 is HSPI MISO pin (Master Data In)
// 	// PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2); //GPIO13 is HSPI MOSI pin (Master Data Out)
// 	// PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2); //GPIO14 is HSPI CLK pin (Clock)
// 	// PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2); //GPIO15 is HSPI CS pin (Chip Select / Slave Select)

// 	// //Clock setup - 1MHz
// 	// WRITE_PERI_REG(SPI_CLOCK(HSPI), 
// 	// 				( ( 7 & SPI_CLKDIV_PRE ) << SPI_CLKDIV_PRE_S) |
// 	// 				( ( 9 & SPI_CLKCNT_N ) << SPI_CLKCNT_N_S ) |
// 	// 				( ( 1 & SPI_CLKCNT_H) << SPI_CLKCNT_H_S ) |
// 	// 				( ( 6 & SPI_CLKCNT_L ) << SPI_CLKCNT_L_S ) );


// 	// //Tx byte order high to low
// 	// SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_WR_BYTE_ORDER);

// 	// // //Rx byte order high to low
// 	// SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_RD_BYTE_ORDER);

// 	// set_spi_mode(0, 0);
// 	// //set_spi_mode(0, 1);
// 	// //set_spi_mode(1, 0);
// 	// //set_spi_mode(1, 1);

// 	// //disable MOSI, MISO, ADDR, COMMAND, DUMMY in case previously set.
// 	// CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_USR_MOSI|SPI_USR_MISO|SPI_USR_COMMAND|SPI_USR_ADDR|SPI_USR_DUMMY|SPI_FLASH_MODE);

// 	// //enable MOSI function in SPI module
// 	// SET_PERI_REG_MASK(SPI_USER(HSPI),  SPI_CS_SETUP | SPI_CS_HOLD | SPI_USR_COMMAND | SPI_USR_MOSI); 
// 	// WRITE_PERI_REG(SPI_USER1(HSPI), (7 & SPI_USR_MOSI_BITLEN)<<SPI_USR_MOSI_BITLEN_S );

// 	    //Disable interrupts
//     ETS_GPIO_INTR_DISABLE();

//     //Set pin functions
//     PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
//     PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
//     PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
//     PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);

//     //enable pull ups
//     PIN_PULLUP_EN(PERIPHS_IO_MUX_MTDI_U);
//     PIN_PULLUP_EN(PERIPHS_IO_MUX_MTCK_U);
//     PIN_PULLUP_EN(PERIPHS_IO_MUX_MTMS_U);
//     PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U);

//     //Turn interrupt back on
//     ETS_GPIO_INTR_ENABLE();
// }

// void spi_clock_tick()
// {
// 	GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CLK_N), 0);
//     os_delay_us(1);
//     GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CLK_N), 1);
//     os_delay_us(1);   
// }

// // void spi_write_rs_rw(bool isCommand) 
// // {
// // 	uint8 mask = 0x2;
// // 	uint8 data;

// // 	if(isCommand)
// // 		data = 0x0;
// // 	else
// // 		data = 0x2;
// // 	do  
// // 	{
// //         GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CLK_N), 0);                //Start clock bit     
// //         os_delay_us(1);                      				  //Ensure minimum delay of 500nS between SPI Clock Low and SPI Clock High

// // 		if (data & mask)                			  		//Clock out current bit onto SPI Out line
// // 		{
// // 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 1);
// // 		}
// // 		else
// // 		{
// // 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 0);
// // 		}       

// //         GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CLK_N), 1);                //End clock bit
// //         os_delay_us(1);  
// //         mask = mask >> 1;           				  //Shift mask so that next bit is written and read from SPI lines
// //                    				  //Ensure minimum delay of 1000ns between bits
// //     } 
// //     while (mask != 0);
// // }

// // void spi_write_data(uint8 data)
// // {
// // 	uint8 mask = 0x80;

// // 	do  
// // 	{
// // 		GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CLK_N), 0);                //Start clock bit     
// //         os_delay_us(1);     

// // 		if (data & mask)                			  		//Clock out current bit onto SPI Out line
// // 		{
// // 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 1);
// // 		}
// // 		else
// // 		{
// // 			GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_OUT_N), 0);
// // 		}  

// //                  				  //Ensure minimum delay of 500nS between SPI Clock Low and SPI Clock High
// //         GPIO_OUTPUT_SET(GPIO_ID_PIN(SPI_CLK_N), 1);                //End clock bit
// //         os_delay_us(1);  
// //         mask = mask >> 1;           				  //Shift mask so that next bit is written and read from SPI lines
// //                    				  //Ensure minimum delay of 1000ns between bits
// //     } 
// //     while (mask != 0);

// // }

// // void set_spi_mode(uint8 spi_cpha, uint8 spi_cpol)
// // {
// // 	if(!spi_cpha == !spi_cpol) {
// // 		CLEAR_PERI_REG_MASK(SPI_USER(HSPI), SPI_CK_OUT_EDGE);
// // 	} else {
// // 		SET_PERI_REG_MASK(SPI_USER(HSPI), SPI_CK_OUT_EDGE);
// // 	}

// // 	if (spi_cpol) {
// // 		SET_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);
// // 	} else {
// // 		CLEAR_PERI_REG_MASK(SPI_PIN(HSPI), SPI_IDLE_EDGE);
// // 	}
// // }

