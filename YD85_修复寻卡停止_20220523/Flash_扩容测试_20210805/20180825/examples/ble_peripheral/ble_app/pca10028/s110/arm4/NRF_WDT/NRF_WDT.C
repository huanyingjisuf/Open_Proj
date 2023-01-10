#include <NRF_WDT.H>
#include <NRF_WDT_RUN.H>

void NRF_WDT_Init() //初始化看门狗
{
	
	NRF_WDT->CONFIG |= WDT_CONFIG_SLEEP_Pause << WDT_CONFIG_SLEEP_Pos;  //在CPU休眠时看门狗暂停  
	NRF_WDT->CONFIG |= WDT_CONFIG_HALT_Pause << WDT_CONFIG_SLEEP_Pos;  //在调试暂停时看门狗暂停
	
	NRF_WDT->CRV =  32768*35;  //25秒
	NRF_WDT->RREN = WDT_RREN_RR0_Enabled << WDT_RREN_RR0_Pos;
	
	NRF_WDT->TASKS_START =1;  //使能看门狗

}

void NRF_WDT_Feed() //喂狗
{
	if(NRF_WDT->RUNSTATUS & WDT_RUNSTATUS_RUNSTATUS_Msk)
	NRF_WDT->RR[0] = WDT_RR_RR_Reload;
}

