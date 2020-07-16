#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_pid.h"
#include "Efm8lb12_adc.h"
#include "Efm8lb12_timer.h"
#include "Efm8lb12_task_sched.h"

volatile SOFT_TIMER_T soft_timer;

volatile uint32_t jiffies = 0;

static TIMER_LIST  timer_list[MAX_SOFT_TIMER_NUM];


static void _soft_timer_init(void);
static void _soft_timer_add(TIMER_LIST **timer, FUNC func, uint32_t timeout);
static void _soft_timer_mod(TIMER_LIST ** timer, uint32_t timeout);
static void _pid_timer_handle(void);
static void _temp_slow_boot_handle(void);
static void _update_aging_duration_handle(void);
static void _soft_timer_jiffies_zero(uint32_t timeout);

extern volatile bit  SMB_BUSY;

//Timer0 is used for Smbus clock source
static void _timer0_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = 0x0;

    TMOD     |= 0x02;					// Sets to 8 bit mode,it is mode2 ,the TL1 is  responsible for counter,TH1 is responsible for Initial value in mode2
    CKCON0   |= 0x01;					// Use system clock divided by 4,if it is not setted ,default system clock value is divided by 12
    TH0       = -(SYSCLK/SMB_FREQUENCY/4/3);  //smbus 的时钟为200khz,根据200Khz算出来的应该是253,因为SCL的时钟频率是定时器溢出率的1/3,所以定时器溢出的频率必须是600khz
	TL0       = TH0;					// Reload the initial value into TL1
	IE_ET0    = 0x0;                 	// disable Timer 0 interrupt(TF0),this bit is the 第1 bit of IE register,IE page addr is all
	TCON_TR0  = 0x1;					// Enable Timer0 ,this bit is the fourth bit of TCON register
    SFRPAGE   = SFRPAGE_SAVE;
}


//Timer1 is used for uart0 clock source, now uart0 is disable
void TIMER1_init(int  counter)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = 0x0;

	TMOD 	 &= ~0xf0;
	TMOD     |= 0x20;   				// Sets to 8 bit mode,it is mode2 ,the TL1 is  responsible for counter,TH1 is responsible for Initial value in mode2
	//CKCON0   |= 0x00;   			    // Use system clock divided by 12,if it is not setted ,default system clock value is divided by 12
	TL1       = counter;                // Initial Value
	TH1       = TL1;					// Reload the initial value into TL1
	IE_ET1    = 0x0;                    // Disable Timer 1 interrupt(TF1)
	TCON     |= 0x40;   			    // Enable Timer1
    SFRPAGE = SFRPAGE_SAVE;
}

//Configure Timer2 for use delay and polling
static void _timer2_init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = 0x0;

	TMR2CN0 &= ~0x04;					//disable timer2
	TMR2CN0 &= ~0x08;					//set timer2 into 16 bit modes

	CKCON0 &= ~0x30;					//Use system clock divided by 12
	TMR2CN0 &= ~0x03;

    TMR2RL = -(SYSCLK/12/1000);         // Reload value to be used in Timer2
    TMR2 = TMR2RL;                      // Init the Timer2 register,Every 1 milliseconds（1ms >>> 3*(1/18000000)） overflow a time

    TMR2CN0 |= 0x04;                    // Enable Timer2 in auto-reload mode
    IE_ET2 = 1;                         // Timer2 interrupt enable
    SFRPAGE = SFRPAGE_SAVE;
}

#if 0
//Configure Timer3 for use with SMBus SCL low timeout detect
//Particular attention :when Timer3 is used to SCL low timeout detect,if SCL low is not timeout , the Timer3 do't enter interrupt
static void Timer3_Init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = 0x0;

	TMR3CN0 &= ~0x04;					//disable timer3
	TMR3CN0 &= ~0x08;					//set timer3 into 16 bit modes

	CKCON0 &= ~0xC0;					//Use system clock divided by 12
	TMR3CN0 &= ~0x03;

	TMR3RL = -(SYSCLK/12/50);           // Reload value to be used in Timer2
	TMR3 = TMR3RL;                      // Init the Timer3 register,Every 20 milliseconds（1ms >>> 3*(1/18000000)） overflow a time

	TMR3CN0 |= 0x04;                    // Enable Timer3 in auto-reload mode
	EIE1 |= 0x80;                       // Timer3 interrupt enable

	TMR3CN0 &= ~0x04;					//disable timer3
	SFRPAGE = SFRPAGE_SAVE;
}


//Configure Timer4 for use with i2c slave SCL low timeout detect
//Particular attention :when Timer4 is used to SCL low timeout detect,if SCL low is not timeout , the Timer4 do't enter interrupt
static void Timer4_Init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = 0x10;

	TMR4CN0 &= ~0x04;					//disable timer4
	TMR4CN0 &= ~0x08;					//set timer4 into 16 bit modes

	CKCON1 &= ~0x03;					//Use system clock divided by 12
	TMR4CN0 &= ~0x03;

	//TMR4RL = 0x3CB0;
	TMR4RL = -(SYSCLK/12/50);           // Reload value to be used in Timer4
	TMR4 = TMR4RL;                      // Init the Timer4 register,Every 20 milliseconds（1ms >>> 3*(1/18000000)） overflow a time

	//TMR4CN0 |= 0x04;                    // Enable Timer4
	EIE2 |= 0x04;                       // Timer4 interrupt enable
	SFRPAGE = SFRPAGE_SAVE;
}
#endif

/*delay_us is range from 0 ~ 10922*/
void TIMER5_hw_delay_us(uint16_t delay_us)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	uint16_t counter = 0;

	/*1s    counter:72000000/12*/
	/*1ms   counter:72000/12*/
	/*1us   counter:72/12 = 6*/

	counter = 65535 - delay_us * 6;

	SFRPAGE  = 0x10;

	TMR5CN0_TF5H = 0;   	// Clear Timer5 overflow flag

	TMR5CN0 &= ~0x04;    	//disable timer5
	TMR5CN0 &= ~0x08; 		//set timer5 into 16 bit modes

	CKCON1  &= ~0x0C;		//set timer5 clock is system clock
	TMR5CN0 &= ~0x03;       //clock divided by 12

	TMR5     = counter;     //set timer5 init value
	TMR5RL   = TMR5;        //set timer5 reload value

	TMR5CN0 |= 0x04;          // Enable Timer5
	//EIE2  |= 0x08;          // Timer5 interrupt enable

	IE_ET2 = 0;                  // Disable Timer2 interrupt in order to prevent preemptive CPU resources,ensure delay is exact
	while(TMR5CN0_TF5H != 0x1)   //wait timer5 overflow
	IE_ET2 = 1;                  // Enable Timer2 interrupt
	TMR5CN0_TF5H = 0;   		 // Clear Timer5 overflow flag

	//TMR5CN0 &= ~0x04;    		 //don't disable timer5, must be enable

	SFRPAGE = SFRPAGE_SAVE;
}


float TIMER_temp_slow_boot_cal()
{
	if((jiffies < 180000) && (soft_timer.slow_boot_timer->enable)) //max 180 seconds
	{
		return (float)jiffies / (float)180000;
	}
	else
	{
		return (float)1;
	}
}

void TIMER_soft_on_poll(void)
{
	uint8_t i = 0;

	for(i = 0; i < MAX_SOFT_TIMER_NUM; i++)
	{
		if((timer_list[i].timeout_flag) && (timer_list[i].function != NULL))
		{
			timer_list[i].function();
			timer_list[i].timeout_flag = false;
		}
	}
}

void WDT_init(uint8_t interval)
{
	WDTCN = 0xFF;    //enable locked out function
	WDTCN &= ~0x80;  //must be logic 0 when setting this interval
	WDTCN = interval;
}

void WDT_disable(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	bool ea = IE_EA;

	//disable the watch dog timer
	SFRPAGE = 0x00;
	IE_EA = 0;      			  //disable global interrupts prevent the write operation is interrupt
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key
	IE_EA = ea;
	SFRPAGE  = SFRPAGE_SAVE;
}


void TIMER_init(void)
{
	_timer0_init();
	_timer2_init();
	//Timer3_Init();
	//Timer4_Init();
	_soft_timer_init();
}


static void _soft_timer_init(void)
{
	uint8_t i = 0;

	for(i = 0; i < MAX_SOFT_TIMER_NUM; i++)
	{
		timer_list[i].timeout_flag  = false;
		timer_list[i].timeout  		= 0;
		timer_list[i].enable  		= 0;
		timer_list[i].expires 		= 0;
		timer_list[i].function 		= NULL;
	}

	_soft_timer_add(&soft_timer.update_aging_duration_timer, _update_aging_duration_handle, 300000); //定时5mins
	//aging start cmd enable timer

	_soft_timer_add(&soft_timer.slow_boot_timer, _temp_slow_boot_handle, 180000); //定时180s
	//_msa_p4_get_reset_reason API detect boot reason and enable timer


	_soft_timer_add(&soft_timer.pid_timer, _pid_timer_handle, 150); //定时150ms
#ifdef SUPPORT_TEC_FUNC
	TIMER_soft_start(&soft_timer.pid_timer);
#endif
}


static void _update_aging_duration_handle(void)
{
	MSA_p9_update_prev_aging_total_time();
}

static void _pid_timer_handle(void)
{
#ifdef SUPPORT_TEC_FUNC
	PID_cal();
#endif
}

static void _temp_slow_boot_handle(void)
{
	soft_timer.slow_boot_timer->enable = 0;  //when slow_boot_timer is timeout, stop it
}

/*alloc TIMER_LIST struct, there must be expires = jiffts + varible, because when jiffts = 0, expires is also changed */
static void _soft_timer_add(TIMER_LIST **timer, FUNC func, uint32_t timeout)
{
	static uint8_t index = 0;

	if(index < MAX_SOFT_TIMER_NUM)
	{
		*timer = &timer_list[index++];
	}
	else
	{
		return;
	}

	(*timer)->function = func;
	(*timer)->timeout  = timeout;

	_soft_timer_jiffies_zero(timeout);
	(*timer)->expires  = timeout + jiffies;
}

//when jiffies is setted 0, there must be init timer_list[i].expires again
static void _soft_timer_jiffies_zero_by_isr(uint32_t timeout)
{
	uint8_t i = 0;
	//JIFFIES_MAX - (timeout + jiffies) > timeout;
	if(JIFFIES_MAX - jiffies < 2 * timeout)
	{
		jiffies = 0;

		for(i = 0; i < MAX_SOFT_TIMER_NUM; i++)
		{
			if(timer_list[i].function != NULL)
			{
				timer_list[i].expires = timer_list[i].timeout;
			}
		}
	}
}

//when jiffies is setted 0, there must be init timer_list[i].expires again
static void _soft_timer_jiffies_zero(uint32_t timeout)
{
	uint8_t i = 0;
	//JIFFIES_MAX - (timeout + jiffies) > timeout;
	if(JIFFIES_MAX - jiffies < 2 * timeout)
	{
		jiffies = 0;

		for(i = 0; i < MAX_SOFT_TIMER_NUM; i++)
		{
			if(timer_list[i].function != NULL)
				timer_list[i].expires = timer_list[i].timeout;
		}
	}
}

/*modify the timer timeout time*/
static void _soft_timer_mod(TIMER_LIST ** timer, uint32_t timeout)
{
	if(*timer == NULL)
	{
		return;
	}

	_soft_timer_jiffies_zero_by_isr(timeout);
	(*timer)->timeout  = timeout;
	(*timer)->expires  = timeout + jiffies;
}

/*start timer*/
void TIMER_soft_start(TIMER_LIST ** timer)
{
	if(*timer != NULL)
		(*timer)->enable = 1;
}

/*stop timer*/
void TIMER_soft_stop(TIMER_LIST ** timer)
{
	if(*timer != NULL)
		(*timer)->enable = 0;
}

/*max delay 65。535s*/
void TIMER_soft_delay_ms(uint16_t ms)
{
	//确保调用这个延时函数的时候。Timer2可以正常工作并正常进入中断计数,否则程序会一直等待下去
	uint32_t current_time = 0;
	uint8_t i = 0, timer_enable_save[MAX_SOFT_TIMER_NUM];

	IE_EA = 1;      			  		// Enable global interrupts
    TMR2CN0 |= 0x04;                    // Enable Timer2 in auto-reload mode
    IE_ET2 = 1;                         // Timer2 interrupt enable

    _soft_timer_jiffies_zero(ms);

    //disable all soft_timer, in order to prevent current_time = JIFFIES_MAX, jiffies is setted 0 right now
    for(i = 0; i < MAX_SOFT_TIMER_NUM; i++)
    {
    	timer_enable_save[i] = timer_list[i].enable;
    	timer_list[i].enable = 0;
    }

	current_time = jiffies;
    while(jiffies <  ms + current_time);

    //restore all soft_timer status
    for(i = 0; i < MAX_SOFT_TIMER_NUM; i++)
    {
		timer_list[i].enable = timer_enable_save[i];
    }
}


SI_INTERRUPT_USING(TIMER2_ISR, TIMER2_IRQn, 2)
{
	uint8_t i = 0;
	TIMER_LIST *ptr = NULL;

	for(i = 0; i < MAX_SOFT_TIMER_NUM; i++)
	{
		if((timer_list[i].enable) && (timer_list[i].expires <= jiffies))
		{
			timer_list[i].timeout_flag = true;
			ptr = &timer_list[i];
			_soft_timer_mod(&ptr, timer_list[i].timeout); //update timeout time
		}
	}

	jiffies ++;

	TMR2CN0_TF2H = 0;  // Clear the interrupt flag
}

SI_INTERRUPT_USING(TIMER0_ISR, TIMER0_IRQn, 0)
{

}

SI_INTERRUPT_USING(TIMER1_ISR, TIMER1_IRQn, 1)
{

}


SI_INTERRUPT_USING(TIMER3_ISR, TIMER3_IRQn, 3)
{
   SMB0CF  &= ~0x80;                    // Disable SMBus
   SMB0CF  |= 0x80;                     // Re-enable SMBus
   TMR3CN0 &= ~0xC0;                    // Clear Timer3 overflow flag
   SMB0CN0_STA = 0;
   SMB_BUSY = 0;                        // Free SMBus
}

SI_INTERRUPT (TIMER4_ISR, TIMER4_IRQn)
{
	SFRPAGE = PG3_PAGE;

	I2C0CN0 &= ~I2C0CN0_I2C0EN__BMASK;			// Disable I2C module
	I2C0CN0 |= I2C0CN0_I2C0EN__ENABLED;			// Re-enable I2C module

	SFRPAGE = PG2_PAGE;
	TMR4CN0 &= ~0xC0;							// Clear Timer4 interrupt-pending flag
}






