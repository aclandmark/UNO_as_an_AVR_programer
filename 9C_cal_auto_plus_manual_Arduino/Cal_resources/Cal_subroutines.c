

long compute_error_UNO(char, char, char);

/********************************************************************************************************************************/
void save_cal_values(unsigned char OSCCAL_user){
eeprom_write_byte((uint8_t*)(EE_size - 1), OSCCAL_user); 
eeprom_write_byte((uint8_t*)(EE_size - 2), OSCCAL_user); 
eeprom_write_byte((uint8_t*)(EE_size - 3), OSCCAL_DV);}




/********************************************************************************************************************************/
void printout_cal_values(void){
sendString("\r\nValues saved to EEPROM  ");	
Num_to_PC(10,eeprom_read_byte((uint8_t*)EE_size-1)); sendChar('\t');
Num_to_PC(10,eeprom_read_byte((uint8_t*)EE_size-2)); sendString("\tUser value\r\n\t\t\t");
Num_to_PC(10,eeprom_read_byte((uint8_t*)EE_size-3)); sendString("\tDefault value\r\n");}




/*********************************************************************************************************************************/
void Minimise_error_down(int limit, unsigned char *counter_1, unsigned char *counter_2, long *error_mag, unsigned char *OSCCAL_mem, char local_cal_mode )
{while(*counter_2 < 20){ OSCCAL = *counter_1; *error_mag = compute_error_UNO(0,local_cal_mode,0); 
if(*error_mag < limit)break;
*counter_1 -= 1;
*counter_2 +=1;}
if (*counter_2 < 20)*OSCCAL_mem = OSCCAL;else OSCCAL = *OSCCAL_mem;}


void Minimise_error_up(int limit, unsigned char *counter_1, unsigned char *counter_2, long *error_mag, unsigned char *OSCCAL_mem, char local_cal_mode )
{while(*counter_2 < 20){ OSCCAL = *counter_1; *error_mag = compute_error_UNO(0,local_cal_mode,0);
	if(*error_mag < limit)break;
	*counter_1 += 1;
*counter_2 +=1;}
if (*counter_2 < 20)*OSCCAL_mem = OSCCAL;else OSCCAL = *OSCCAL_mem;}



/**************************************************************************************************************************************/		
long compute_error_Xtal(char local_error_mode, char local_cal_mode, char sign)			//Watch Xtal				
{long error;
char Num_1, Num_2;
	
if (local_cal_mode == 1){Num_1 = 2; Num_2 = 1;}
if (local_cal_mode == 2){Num_1 = 6; Num_2 = 4;}
if (local_cal_mode == 5){Num_1 = 15; Num_2 = 10;}
		
EA_counter = 0;															//Compute error for each value of OSCCAL 10 times
error_SUM = 0;
while(EA_counter < Num_1);EA_counter = 0;
error = error_SUM;
if ((!sign) && (error < 0)) error *= (-1);
if (local_error_mode)
{buffer[EA_buff_ptr] = error/Num_2; EA_buff_ptr++;}
return error/Num_2;}



long compute_error_UNO(char local_error_mode, char Num_Av, char sign)				//UNO provides time standard
{long error;
	
	error_sum = 0;
	int_counter = 0;													//Initialise all registers
	TCNT1_sum = 0;
	TCCR1B = 0;															//Ensure T1 is halted
	TCNT1 = 0;															//clear Timer 1
	enable_PCI_on_SCK_pin;
	set_PCI_mask_on_SCK;	
	while (int_counter < Num_Av);										//Pause here for interrupts: Average the result over several 32.768mS periods
	disable_PCI_on_SCK_pin;
	clear_PCI_mask_on_SCK;	
	error = error_sum/Num_Av;											//Obtain average result
	if (!(sign) && error < 0) error = error * (-1);						//Set sign if required
	
	if (local_error_mode)
{buffer[EA_buff_ptr] = error; EA_buff_ptr++;}
	
		return error;	}







