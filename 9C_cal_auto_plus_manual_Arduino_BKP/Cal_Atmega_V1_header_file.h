

#define prescaller_setting      2


#define enable_PCI_on_SCK_pin    PCICR |= (1 << PCIE0);
#define disable_PCI_on_SCK_pin   PCICR &= (~(1 << PCIE0));

#define set_PCI_mask_on_SCK     PCMSK0 |= (1 << PCINT5);
#define clear_PCI_mask_on_SCK   PCMSK0 &= (~(1 << PCINT5));

#define Enable_Timer_1_Interrupt    TIMSK1 |= (1 << OCIE1A);
#define Disable_Timer_1_Interrupt   TIMSK1 &= (~(1 << OCIE1A));


volatile long TCNT1_sum;
volatile char int_counter;
volatile long error_sum;


/***********************************************************************************/
//Only include if using Atmega 32 or device with similar UART

/*************************************************************************************/

#include "Cal_resources/Basic_HW_setup.h"
#include "Cal_resources/Basic_IO_and_timer_subs.c"
#include "Cal_resources/Cal_subroutines.c"









	
