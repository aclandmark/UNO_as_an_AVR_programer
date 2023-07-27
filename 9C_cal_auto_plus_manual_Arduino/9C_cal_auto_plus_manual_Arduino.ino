




/*
Watch Xtal mode
T1 clocks at 8000KHz (no prescaling)
T2 clocks at 32.768KHz (No prescaling)
T1 counts to 62500 when T2 counts to 256

UNO clock mode	
The UNO puts a  65.536mS square wave on the UNO clock programming pin	PB5 (SCK/PCINT5) ISR (PCINT0_vect)
Generates a pin change intterrupt (PCI) every 32.768mS and calls ISR (PCINT0_vect)
7.8125 KHz clock counts to 256 in 32.768mS 
Set T1 prescaler to 8 for a 1MHz clock
T1 counts to 32768   


*/


#include "Cal_Atmega_V1_header_file.h"


int main (void)

{long error_up, error_down;
  int EEP_MAX = 0x2000;
  
  setup_HW;
  OCR1A = 0xFF;
  //ASSR = (1 << AS2);                                //initialise T2 for crystal
  OSCCAL_DV = OSCCAL;                               //Save default value off OSCCAL

  Auto_cal (0);                                 //Search starting from 0xF0
  OSCCAL += 1;                                  //Check performance for OSCCAL_WV +/- 1
  Get_ready_to_calibrate;
  sei();
  error_up = compute_error_UNO(0,2,0);                        //Check that OSCCAL_WV is not at a
  OSCCAL -=2; error_down = compute_error_UNO(0,2,0);                  //discontinuity on the
  close_calibration;                                //OSCCAL v error plot
  cli();
  OSCCAL += 1;                                  //Restore OSCCAL to OSCCAL_WV
  if ((error_up > 1000) || (error_down > 1000))
  {sendString("\r\nPoor result: Searching for alternative value\r\n");
  Auto_cal (1);}                                  //Repeat search starting from 0x10
  
  set_device_type_and_memory_size;
  sendString("\r\n\r\nCalibrating Atmega ");
  sendString (Device_type[device_ptr]);
  
  save_cal_values(OSCCAL_WV);                           //Save result of the auto calibration

  sendString("\r\nNew OSCCAL value ");
  Num_to_PC(10,OSCCAL_WV);

  sendString("\r\nPress 'x' to finish or AOK for manual cal\r\n");        //Omit for 4K device
  if (waitforkeypress() == 'x')                         //Omit for 4K device
    {printout_cal_values();}
  

  else Manual_cal();                              //Omit for 4K device

  sendString("\r\nAll done: AK to repeat.\r\n");

  waitforkeypress();

SW_reset;}




/************************************************************************************************/
void Auto_cal (char direction){
  unsigned char counter_1, counter_2;
  unsigned char OSCCAL_mem = 0;
  long  error_mag;
  int limit;

  UCSR0B &= (~(1 << RXEN0));                        //avoid unwanted keypresses

  Timer_T1_sub(T1_delay_1sec);                      //Crystal warm up time

  sei();
  cal_mode = 1;
  //Get_ready_to_calibrate;
  
  if (!(direction))
    {counter_1 = 0xF1;
    while(1){sendChar('.');
      counter_1 -= 1;
      OSCCAL = counter_1; error_mag = compute_error_UNO(0,cal_mode,0);
      if(counter_1 > 0xE0)continue;
      if(error_mag < 1000)break;}
    OSCCAL_mem = OSCCAL;
    counter_2 = 0;
    cal_mode = 5;
    limit = 1000;
    for(int m = 1; m <= 9; m++){sendChar('.');
      limit -= 100;
      Minimise_error_down(limit, &counter_1, &counter_2,\
       &error_mag, &OSCCAL_mem, cal_mode);}}
  
  if (direction)
    {counter_1 = 0x0F;
      while(1){sendChar('.');
        counter_1 += 1;
        OSCCAL = counter_1; error_mag = compute_error_UNO(0,cal_mode,0);
        if(counter_1 < 0x20)continue;
        if(error_mag < 1000)break;}
      OSCCAL_mem = OSCCAL;
      counter_2 = 0;
      cal_mode = 5;
      limit = 1000;
      for(int m = 1; m <= 9; m++){sendChar('.');
        limit -= 100;
        Minimise_error_up(limit, &counter_1, &counter_2,\
         &error_mag, &OSCCAL_mem, cal_mode);}}
  
  error_mag = compute_error_UNO(0,cal_mode,0);
  OSCCAL_WV = OSCCAL;
  close_calibration;
Num_to_PC(10, error_mag);sendChar('A');Num_to_PC(10, OSCCAL_mem);
  UCSR0B |= (1 << RXEN0);
cli();}





/**********************************************************************************************************************/
void Manual_cal(void){
  long cal_error;
  unsigned char osccal_MIN;
  long percentage_error;

  UCSR0B &= (~(1 << RXEN0));                          //avoid unwanted keypresses
  sendString("\r\nManual cal: please wait 5 seconds\r\n");
  Timer_T0_sub(T0_delay_5ms);                          //gives printer time to finish

  sei();

  EA_buff_ptr = 0;
  cal_mode = 5;
  ////Get_ready_to_calibrate;
  OSCCAL -=20;
  osccal_MIN = OSCCAL;                            //Compute cal error for 41 values of OSCCAL
  for(int m = 0; m <= 40; m++)
  {cal_error = compute_error_UNO(1,cal_mode,1);OSCCAL++;}
  OSCCAL = OSCCAL_WV;                             //Restore working value of OSCCAL
  close_calibration;

  newline();
  sendString("OSCCAL\t Error\r\n");


  for(int m = 0; m <= 40; m++)                        //Print table of values
  {Num_to_PC(10,osccal_MIN); osccal_MIN++;
    sendChar('\t');Num_to_PC(10,buffer[m]);
    sendChar('\t');
    percentage_error = buffer[m];
    Num_to_PC(10,percentage_error*100/32768);sendChar('%');
    newline();
    Timer_T0_sub(T0_delay_20ms);
  Timer_T0_sub(T0_delay_20ms);}                       //stops PC being overwhelmed with data

  sendString("\r\nEnter new user cal value or x to escape.");


  UCSR0B |= (1 << RXEN0);                           //keypresses required again

  if ((OSCCAL_UV = UC_from_KBD()) == 'x')
    OSCCAL_UV = OSCCAL_WV;                          //stick with result of auto cal

  
  /*********************************************/
  else
    {Get_ready_to_calibrate;                        //Test value of OSCCAL entered by user
    OSCCAL = OSCCAL_UV;                           //Test new OSCCAL value
    calibrate_without_sign_plus_warm_up_time;
    close_calibration;

    if(cal_error > 1750)                          //Error resulting from User OSCCAL exceeds 1750
      {OSCCAL = OSCCAL_WV;                        //Reinstate working value
      OSCCAL_UV = OSCCAL_WV;
      sendString("\tChange rejected: error too great");}}


    cli();
    save_cal_values(OSCCAL_UV);
  printout_cal_values();}







  /*************************************************************************************************************************************/
 /* ISR(TIMER2_OVF_vect) {                      //For use with watch Xtal
    long TCNT1_BKP, overflow = 0, target_res;

    target_res = 62500;
    TCCR1B = 0;                                 //Halt T1
    TCNT1_BKP = TCNT1;                              //Copy the value of TCNT1
    TCNT1 = 0;                                  //Clear TCNT1
    TCCR1B = 1;                                 //Get T1 running again ASAP (Note T2 has not stopped running)
    
    if(EA_counter < cal_mode)T1_OVF = 0;                     //Ignore first few results
    else
    {
      switch(T1_OVF){
        case 0: overflow = 0; break;
        case 1: overflow = 65536; break;
      case 2: overflow = 131072; break;}
      error_SUM = error_SUM + (TCNT1_BKP - target_res + overflow);
    T1_OVF = 0;}
  EA_counter++;}

*/


ISR (PCINT0_vect){                       //UNO provides time standard   Pin change interrupt on SCK pin
                    
      if (!(TCCR1B)) {
      TCNT1_sum = 0;
      TCNT1 = 0;
      TCCR1B = prescaller_setting;}             //1MHz clock
  
    else {TCCR1B = 0; int_counter += 1;
      error_sum = error_sum + TCNT1_sum - 32768 + TCNT1;
    }}
 



  /************************************************************************************************************************************/
//ISR(TIMER1_OVF_vect) {T1_OVF += 1;}       //For use with watch Xtal
ISR(TIMER1_COMPA_vect){TCNT1_sum += 0x100; }    //UNO provides time standard

  /************************************************************************************************************************************/
