/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 *
 */
 
/** @file
 *
 * @defgroup ble_sdk_uart_over_ble_main main.c
 * @{
 * @ingroup  ble_sdk_app_nus_eval
 * @brief    UART over BLE application main file.
 *
 * This file contains the source code for a sample application that uses the Nordic UART service.
 * This application uses the @ref srvlib_conn_params module.
 *
 *
 */
 
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_nus.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "nrf_delay.h"
#include "mpr121.h"
#include "w25q32.h"
#include "BL5372.h"
#include "Moto_Run.h"
#include "Devic_Init.h"  //设备初始化
#include "Flash_RW.h"
#include "speak.h"
#include "ADC.H" 
#include <KEY_Run.h>
#include <NRF_WDT.H>
#include <Unix_Time.h>  //时间戳转换头文件
#include <BLE_Data.h>
#include <Act_PassWord.h>

#include "mcu.h"						//SKY1311 头文件
#include "stm8_eval.h"
#include "sky1311_drv.h"
#include "sky1311t.h"
#include "iso14443.h"
#include "cardoper.h"
#include "userdef.h"


#include "uart.h"

#include "simple_uart.h"  //定义外设串口

#include <Uart_Run.h>   // 串口

#include <F_print.h>  //指纹协议

#ifdef NB_Enable   //NB 使能时才可使用

#include <NB_Mode1.h>  // NB

#endif

//绑定时广播名 前缀

#if YFA			//人脸识别成立时

#define Devic_Name_H1	'Y'
#define Devic_Name_H2	'F'

//绑定时广播名 后缀
#define Devic_Name_E  'A'


#elif DSA			// 如家成立时

#define Devic_Name_H1	'D'
#define Devic_Name_H2	'S'

//绑定时广播名 后缀
#define Devic_Name_E  'A'


#else				//均不成立选择默认

#define Devic_Name_H1	'H'
#define Devic_Name_H2	'Y'

//绑定时广播名 后缀
#define Devic_Name_E  'P'


#endif

#define IS_SRVC_CHANGED_CHARACT_PRESENT  0                                           


/**< Include the service_changed characteristic. 

If not enabled, the server's database cannot be changed for the lifetime of the device. 
包含服务端特性，如果未启用，在设备的工作周期中，服务器的数据将不能被更改

*/

//#define DEVICE_NAME                     "HYO_C1EAD0DC8E42"      //广播名

#if defined ( USB_SYSTEM_CLOCK_32MHZ )  //使用32Mhz时钟

#define DEVICE_NAME1 "HYO_C1EADCDC8E42_32MHz"

#else

#define DEVIC_NAME1 "HYO_23333"

#endif

//#define DEVICE_NAME1										"HYO_C1EADCDC8E42"				//设备名

char DEVICE_NAME [28] = {"HYI_"};


/**
<  Name of device. Will be included in the advertising data. >
设备名称，将出现在广播数据中
*/

#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  
/**
< UUID type for the Nordic UART Service (vendor specific).
对于Nordic UART Service 的UUID类型 (特定于供应商)
*/

#if YFA			//人脸识别成立时



#define APP_ADV_INTERVAL            800   

#else


#define APP_ADV_INTERVAL            1200    


#endif

#define APP_AC_ADV_INTERVAL         100    
 

/**<
The advertising interval (in units of 0.625 ms. 
This value corresponds to 40 ms). 
广播间隔，一个单位为0.625ms
默认设定再40ms
*/


#define APP_ADV_TIMEOUT_IN_SECONDS      0
/**< 
The advertising timeout (in units of seconds). 
广播超时时间（最大允许连续广播时间，一个单位时间为秒，目前设置在无限时间）
*/

#define APP_TIMER_PRESCALER             0                                           
/**< 
Value of the RTC1 PRESCALER register. 
RTC1 时钟参考
*/

#define APP_TIMER_OP_QUEUE_SIZE         10   //4       增加了 键盘扫描 数据包定时清空 开门记录回传  灯闪烁                            

/**<
Size of timer operation queues. 
定时器队列大小
*/

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(15, UNIT_1_25_MS)             

/**<
Minimum acceptable connection interval (20 ms), 
Connection interval uses 1.25 ms units. 
可接受的最小时间间隔(预设20ms)
时间间隔的时间单位是1.25ms
*/


#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             
/**< 
Maximum acceptable connection interval (75 ms), 
Connection interval uses 1.25 ms units. 
可接受的最大时间间隔(预设75ms)
时间间隔的时间单位是1.25ms
*/
#define SLAVE_LATENCY                   0                                           
/**< 
Slave latency. 
从机潜伏期 (从机无连接时跳过的回应包)
*/
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(2000, UNIT_10_MS)             
/**<  
Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. 
连接超时管理(预设4秒)

超时管理时间单位是10ms
*/
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  
/**< 
Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). 
第一次通知时间(连接或开始通知) (更新连接信息) (默认5秒)
连接或开始通知
*/
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) 
/**< 
Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). 
每次通知间隔 在第一次通知之后  (更新连接信息) (默认30秒)
*/

#define MAX_CONN_PARAMS_UPDATE_COUNT    7 

/**< 
Number of attempts before giving up the connection parameter negotiation. 
在放弃连接参数协商前的重试次数
*/

#define    START_STRING       "BLE_Start!\n"                                
/**< 
The string that will be sent over the UART when the application starts. 
在应用程序启动时串口发送的字符串
*/

#define DEAD_BEEF                       0xDEADBEEF                                  
/**< 
Value used as error code on stack dump, 
can be used to identify stack location on stack unwind. 
在堆栈转储中用作错误代码的值
可以用来识别堆栈的堆栈位置
*/

#define UART_TX_BUF_SIZE                20                                       
/**< 

UART TX buffer size. 
UART TX 缓存区体积
*/
#define UART_RX_BUF_SIZE                20                                        
/**< 
UART RX buffer size. 
UART RX 缓存区体积
*/
static ble_nus_t                        m_nus;      


/**< 
Structure to identify the Nordic UART Service. 
Nordic UART 的鉴定结构
*/
static uint16_t                         m_conn_handle = BLE_CONN_HANDLE_INVALID;    
/**< 
Handle of the current connection. 
当前连接的处理
*/

static ble_uuid_t                       m_adv_uuids[] = {{BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}};  /**< Universally unique service identifier. 统一的唯一标识符*/

uint8_t tab_dat[tab_dat_size];  //flash 扇区缓存
uint8_t MAC_ADD[6];  //MAC地址缓存
static void advertising_init(void);  //广播初始化

#ifdef NB_Enable   //当使用NB模块时，允许调用此处程序，且必须使用RAM容量更大的芯片

uint8_t NB_Dat[NB_Dat_Size];
uint32_t NB_Dat_Len =0;   //NB Dat 字符表 当前使用到的位置
uint8_t NB_Run =0;

#endif

//定时输出Flash 与 时钟数据
APP_TIMER_DEF (my_test_timer);  //声明新的软件定时器名称  注册软件定时器

APP_TIMER_DEF(Uart_Close);

/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice. 在协议栈中将被调用
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse 
 *          how your product is suppoI``sed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


#if defined(USB_SYSTEM_CLOCK_32MHZ)

static void setClock ( void )
{
	// Set the external high frequency clock source to 32MHz
	
	NRF_CLOCK->XTALFREQ = 0XFFFFFF00;
	
	// Start the external high frequency crystal
	
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	
}

#endif


/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of 
 *          the device. It also sets the permissions and appearance.
 *  GAP一些参数的设置，设置设备名，设置PPCP(外围设备首选链接参数)。(手机连上某个蓝牙设备后可以从Generic Access Service中看到设置的这些参数)
 *
 */
 
static void gap_params_init(void)
{
	
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    
    err_code = sd_ble_gap_device_name_set(&sec_mode,  //设置设备名
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);

}

uint8_t TX_SUC =0;  //完成标志

uint8_t BLE_TX_Run(uint8_t *s,uint8_t len)  //发送数据到BLE
{
	
		uint32_t err_code;
//		uint8_t i;
	
		TX_SUC =1;
//	while(TX_SUC)
//	{
//		i++;
//		nrf_delay_ms(1);
//		
//		if(i>=10)return 0;
////		#ifdef uart_start
////		SEGGER_RTT_printf(0,"Delay_TX_OK.\r\n");
////		#endif
//	
//	
//	
//	}
//	#ifdef uart_start

//		for(i =0;i<len;i++)
//		{
//			SEGGER_RTT_printf(0,"%x,",*s);
//			s++;
//		}
//		SEGGER_RTT_printf(0,"\r\n");
//			s-=len;
//		#endif		
		err_code = ble_nus_string_send(&m_nus, s, len);
		if (err_code != NRF_ERROR_INVALID_STATE)
			{
					APP_ERROR_CHECK(err_code);
			}
				nrf_delay_ms(30);
			return 1;
}


/**@brief Function for handling the data from the Nordic UART Service.  //用于处理来自Nortic UART 服务的 数据
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send 
 *          it to the UART module.
 *
 * 这个函数将处理来自Nordic UART BLE   //服务 并发送到UART模块
 * @param[in] p_nus    Nordic UART Service structure.  //Nordic UART  服务模块
 * @param[in] p_data   Data to be send to UART module. //将数据发送到UART 模块
 * @param[in] length   Length of the data.   //数据的长度
 */
/**@snippet [Handling the data received over BLE] */   //处理接收到的数据


extern uint8_t  IN_BLE_Dat_SW;
static void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)  //BLE数据处理
{
//	uint32_t err_code;
//	uint8_t BLE_TX[20];	
#ifdef uart_start	  //选择是否通过串口发送来自BLE的数据

//    for (uint32_t i = 0; i < length; i++)  //撤销串口发送
//    {
//        while(app_uart_put(p_data[i]) != NRF_SUCCESS);
//    }
  //  while(app_uart_put('\n') != NRF_SUCCESS);
#endif
//			err_code = ble_nus_string_send(&m_nus, BLE_TX, 2);
//		if (err_code != NRF_ERROR_INVALID_STATE)
//			{
//					APP_ERROR_CHECK(err_code);
//			}
//	nrf_delay_ms(1);
//	for(uint8_t i =0;i<20;i++) BLE_TX [i] =0;
//	

	if(IN_BLE_Dat_SW==0)
	Data_Input(p_data,length);
}

/**@snippet [Handling the data received over BLE] */ //处理接收到的数据


/**@brief Function for initializing services that will be used by the application.
服务初始化，添加uart的串口服务。主要提供两个特征值来提供手机和板子以及电脑的通讯 
*/
static void services_init(void)
{
    uint32_t       err_code;
    ble_nus_init_t nus_init;
    
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;
    
    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling an event from the Connection Parameters Module.//从连接参数模块处理事件的函数。
 *
 * @details This function will be called for all events in the Connection Parameters Module //这个函数将调用连接参数中的所有事件，将其传递给应用程序。
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;
	
		if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}
  
void Dis_Conner()  //断开连接
{
	uint32_t err_code;
	
	err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
	APP_ERROR_CHECK(err_code);
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nDis_Conner!\r\n\r\n");
	
	#endif

}

/**@brief Function for handling errors from the Connection Parameters module.  //用于处理连接参数模块中错误的函数。
 *
 * @param[in] nrf_error  Error code containing information about what went wrong. //错误代码包含了错误的信息。
 */

static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for initializing the Connection Parameters module. //初始化连接参数模块的函数。
*/

static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;
    
    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt; 
    cp_init.error_handler                  = conn_params_error_handler;
    
    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for putting the chip into sleep mod //使芯片进入睡眠状态的功能。
e. 
 *
 * @note This function will not return. //这个函数不会返回。			`		
 */
static void sleep_mode_enter(void)
{

		uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);		

    // Prepare wakeup buttons. //准备唤醒按钮
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset). //进入系统关闭模式(这个函数不会返回；唤醒将导致重置)
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
	
}


/**@brief Function for handling advertising events. //用于处理广播事件的功能。
 *
 * @details This function will be called for advertising events which are passed to the application. //调用这个函数将被用于传递广播事件给应用程序。
 *
 * @param[in] ble_adv_evt  Advertising event. //广播事件
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
	
    uint32_t err_code;
		
    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;
        case BLE_ADV_EVT_IDLE:
            sleep_mode_enter();
            break;
        default:
					
            break;
    }
}


/**@brief Function for the Application's S110 SoftDevice event handler. //应用程序的S110协议栈事件处理程序的功能。
 *
 *@param[in] p_ble_evt S110 SoftDevice event. //S110协议栈事件。
 */


uint8_t BLE_LINK = 0;  //BLE连接状态实时更新
uint8_t Ble_Lin_T =0;  //蓝牙连接时间限制  预设一次蓝牙连接的时间不超过两分钟
extern uint16_t data_len;  //BLE数据长度
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t   err_code;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:  //BLE连接已建立
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
        BLE_LINK =true;    
				Ble_Lin_T =0;		//连接计时清零
				data_len =0;    //连接后清空BLE数据缓存
				BLE_Data_Len =0;
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Link_Y\r\n");
			#endif
//				input_speak(0);
//				input_speak(1);  //已连接
			//	input_speak(3);
				break;
				
        case BLE_GAP_EVT_DISCONNECTED: //BLE连接已断开
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            APP_ERROR_CHECK(err_code);
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
 
				BLE_LINK = false;
				
				data_len =0;    //断开连接 清空BLE数据缓存
				BLE_Data_Len =0;
						
			#ifdef uart_start
				
			SEGGER_RTT_printf(0,"Link_N\r\n");
			
			#endif
	//			input_speak(0); //先发送空指令
//				input_speak(2); //未连接
			//	input_speak(3);
				break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported //配对不支持
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored. //没有存储系统属性
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;
				case BLE_EVT_TX_COMPLETE:  //数据发送完成
					
					TX_SUC =0;
        default:
            // No implementation needed. //不需要实现。
            break;
    }
}


/**@brief Function for dispatching a S110 SoftDevice event to all modules with a S110 SoftDevice 
 *        event handler. //使用S110协议栈向所有模块发送S110协议栈事件的功能事件处理程序
 * 
 * @details This function is called from the S110 SoftDevice event interrupt handler after a S110 
 *          SoftDevice event has been received. //这个函数是从S110协议栈已经收到的软件事件中断处理程序中调用的，
 *
 * @param[in] p_ble_evt  S110 SoftDevice event. //S110 协议栈 事件
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{

		ble_conn_params_on_ble_evt(p_ble_evt);
    ble_nus_on_ble_evt(&m_nus, p_ble_evt);
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);
    bsp_btn_ble_on_ble_evt(p_ble_evt);

}


/**@brief Function for the S110 SoftDevice initialization.
 *
 * @details This function initializes the S110 SoftDevice and the BLE event interrupt.
 */
/*
设置LFCLK(32.768k)的时钟源(协议栈需要使用)，这里设置为外部晶振。flase为不使用调度。
softdvice_ble_evt_handler_set(ble_evt_dispatch);注册事件派发程序，基础1-协议栈概述说明
过，当BLE收到广播，链接请求，对端设备数据等后底层处理完后抛给上册app一个事件，这个事件
的上抛过程是协议栈触发SWI中断，在终端内部将事件放入队列，然后调用app中的SWI中断。App中的
SWI中断会get队列中的事件，并最终会调用注册的ble_evt_dispatch函数，这个函数再将事件发给各个服务
以及模块的事件处理函数来处理各个服务模块及模块自己感兴趣的事件。事关原理基础1-协议栈概述视频中有说明。
*/

static void ble_stack_init(void)
{
    uint32_t err_code;
    
    // Initialize SoftDevice. 初始系统时钟与相关参数
   // SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, NULL);	
	
	 SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION,NULL); //修改低频时钟源

	
	
    // Enable BLE stack. 使能协议栈 获取默认启动softdevice参数
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
#if (defined(S130) || defined(S132))
    ble_enable_params.gatts_enable_params.attr_tab_size   = BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;
#endif
    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
    
    // Subscribe for BLE events 调用BLE事件 BLE回调派发
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}


/******************************************

获取键盘灯亮和灯灭时的感应读卡阀值

输入： 无

输出： 在LEDOP_RC_VALUE 和 LEDCL_RC_VALUE 中载入获取到的值

******************************************/

extern uint8_t SPI0_Start; //确认SPI0 是否被启用

uint32_t LEDOP_RC_VALUE; // 键盘 LED打开时的低功耗寻卡阀值
uint32_t LEDCL_RC_VALUE;  //键盘 LED关闭时的低功耗寻卡阀值


uint8_t SKY1311_RC_VALUE ()
{
	
		uint32_t rc_value;
	
		if(SPI0_Start==0) //当SPI0接口未被占用时(无其他设备在使用SPI0接口)
		{
			
			Spi_Flash_Init();  // 开启 SPI0 接口
			
			irqClearAll(); 		
			sky1311Reset();
			rc_value =0;
			mpr121_Run();
			nrf_gpio_pin_set(LEDB);//开灯
	//		nrf_gpio_pin_set(LEDB1);//开灯
			nrf_delay_ms(500); //等待稳定
			
			rc_value = sky1311RCFreqCali();  //获取 灯亮时的 RC阀值
			
		//	if(rc_value!=0)
			LEDOP_RC_VALUE = rc_value;
			
//			mpr121_init();
			TSM12_Init();
			nrf_gpio_pin_clear(LEDB);
//			nrf_gpio_pin_clear(LEDB1);
			nrf_gpio_pin_clear(LEDR);
			
			nrf_delay_ms(100);  //等待稳定
			
			rc_value = sky1311RCFreqCali();
			
			//if(rc_value!=0)
			LEDCL_RC_VALUE = rc_value;
			
			
		
			sky1311Reset();
			//Spi_Flash_Init();
			Spi_Flash_Sleep(); //  操作完成后关闭 SPI0 接口
			
			if((LEDOP_RC_VALUE!=0)&&(LEDCL_RC_VALUE!=0))  //当两个数据都有效时
			return 1; //成功获取 RC 值 则返回 1
			
		}
	
	return 0;
	
}


uint32_t CL_LED_Cheak_RC_Value()   //仅采样 关闭LED 时的 RC 值
{
	uint32_t rc_value;
	
	if(SPI0_Start==0)
	{
	
	Spi_Flash_Init();  // 开启 SPI0 接口
	
	irqClearAll(); 		
	sky1311Reset();
	
		nrf_gpio_pin_clear(LEDB);
//			nrf_gpio_pin_clear(LEDB1);
	nrf_gpio_pin_clear(LEDR);
	
	nrf_delay_ms(100);  //等待稳定
	
	rc_value = sky1311RCFreqCali();
	
	//if(rc_value!=0)
	LEDCL_RC_VALUE = rc_value;
	
	sky1311Reset();
	//Spi_Flash_Init();
	Spi_Flash_Sleep(); //  操作完成后关闭 SPI0 接口
	
	if(rc_value!=0)		//当值合法时 返回新的值
	return rc_value;
}
	
	return LEDCL_RC_VALUE;   //  当采样出现异常时，返回原来的值作为采样基准

}


APP_TIMER_DEF (Read_My_Key);  //声明新的软件定时器名称

APP_TIMER_DEF (Timer_1);  //声明新的软件定时器名称 1秒唤醒一次，按需唤醒


uint8_t Init_Key_Num =0;
static void  Read_Key (void *params);
//void read_a_key(); //唤醒后第一次读取
static void AC_advertising_init(void);
                                             
uint16_t rcValue;
boolean hasCardIn=false;
#define  CARDREMOVED        0
#define  HASCARDIN          1
#define  READCARDOVER       2
#define  WAITREMOVE         4
#define  GOTOSLEEP          8

uint8_t  cardStatus = CARDREMOVED;

uint8_t read_TypeA_Data[128];  //A卡数据区域缓存

//关于 判断读取到 卡型号的对比 (字符表第一字节判断)

const uint8_t Admin_TypeA_Card =1;

const uint8_t User_TypeA_Card  =2;

const uint8_t Admin_TypeB_Card =3;

const uint8_t User_TypeB_Card  =4;


/*********************************************

read_TypeAB_SN 意义

当读到有效卡，且本地有保存有数据时，这个值就是本地此数据SN号 

最高位四字节 意义
 
 1 时表示这个为 管理员 TypeA 卡 
 2 时表示这个为 用户   TypeA 卡
 
 3 时表示这个为 管理员 TypeB 卡 
 4 时表示这个为 用户   TypeB 卡


**********************************************/

uint32_t read_TypeAB_SN; 

void Marnual_Add_Run(void);

/*当数据有效时用于保存 本地有效数据 的有效时间 第一字节用于表示 是否有效。
01 表示 有效 管理A 卡时间 
02 表示 有效 用户A 卡时间 
03 表示 有效 管理B 卡时间 
04 表示 有效 用户B 卡时间 

其他无效
*/
extern uint8_t TypeAB_Unix_T[9];

uint32_t TypeAB_Start_Unix;  //TypeAB_Unix_T 包含起始结束信息 的换算整合变量 

uint32_t TypeAB_Stop_Unix;

uint8_t FD_Key =0;			//防拆标志

uint32_t Unix_T;  //当前芯片内时间戳

void Start_Key_Run(void);  //直接唤醒键盘的函数
extern uint8_t Start_Manual_Add_Card;  

/**@brief Function for handling events from the BSP module. //用于处理来自BSP模块的事件的功能
 *
 * @param[in]   event   Event generated by button press. //由按钮按下产生的事件
 */
 
 void bsp_event_handler(bsp_event_t event)
 {
	 
	//bsp_event_handler_O();
	
 }
 
 
uint32_t Read_Card_Error_Num =0;				// 用于记录 单位时间内(My_test_timer_Time_Size)  读卡错误的次数 

uint8_t SKY1311_Error =0;		// 设置 SKY1311 出错时的恢复机制

extern u8 Op_Door_Buff[];				//用于缓存开门记录数据 待写入缓存

uint8_t DatCrd_AU;					//区分数据卡类型
 
//#define FC_OP_ Key  1   //防拆改 开门 宏定义 屏蔽后 防拆变为开门

void bsp_event_handler_O()
{
	  uint32_t err_code;
//  	uint32_t it2,i;
		
	if(!MPR121_RUN)  //当键盘未唤醒时，清空键盘计数
		MPR121_T =0;
	
		if(
			(!MPR121_RUN)&&
			(!nrf_gpio_pin_read(MPR_IRQ))
			&&(SPI0_Start==0)
			&&(Test_Ps_Mark==0)
			&&(Controllable_Power_State==0)
			&&(F_Print_Run_Flag==0)
			)			//未避免冲突，只有当指纹头关闭时才允许唤醒
			{
					//	read_a_key();
								
					err_code = app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);			//开始读键盘的定时器任务
					APP_ERROR_CHECK(err_code);
					

				
					if(SPI0_Start==0) //当SPI0关闭时，将扫描阀值调整到唤醒后的值
					{
						
						nrf_gpio_pin_set(LEDB);//开灯
						//nrf_gpio_pin_set(LEDB1);//开灯	
						
						#ifdef Key_By_MPR121
						mpr121_Run(); //同时启动键盘，用于建立键盘电场环境
						#endif
						
						#ifdef Key_By_TSM12c
						
						#ifdef uart_start
				
						SEGGER_RTT_printf(0,"TS12_Touch!\r\n");
						
						#endif
						
						nrf_gpio_pin_set(LEDB);//开灯
					//	nrf_gpio_pin_set(LEDB1);//开灯
						
					//	TSM12_read_tast();
						TSM12_Exit_Sleep_Mode();  //退出睡眠模式
						
						#endif
						
						Spi_Flash_Init();
						
						sky1311Reset();
						
						irqClearAll();              // 清除SKY1311的所有中断
						checkCardInit(LEDOP_RC_VALUE);    // 配置进入自动检卡状态
						cardStatus = GOTOSLEEP;
						
						nrf_delay_ms(100);  //预设500ms 的电场波动时间，期间的感应值无效
						
						irqClearAll();
						//nrf_delay_ms(500);	
						mpr121_Run();	 //再启动键盘以清空期间的误触
										
						Spi_Flash_Sleep();
						
						#ifdef uart_start
							
						SEGGER_RTT_printf(0,"Check_Card_Start_LEDOP_Value!\r\n");  //尝试开启亮灯时的寻卡阀值
							
						#endif
						
					}
					if(Devic_Static==false)  //当设备未绑定时
						{
							
							DEVICE_NAME[2] = Devic_Name_E;
									//GAP一些参数的设置，设置设备名，设置PPCP(外围设备首选链接参数)。
						//(主机连上某个蓝牙设备后可以从Gameric Access Service 中看到设置的这些参数)
							sd_ble_gap_adv_stop();
							gap_params_init();  //设置蓝牙名	  
							//advertising_init();
							AC_advertising_init();  //设置广播间隔
							err_code = ble_advertising_start(BLE_ADV_MODE_FAST); //同时开启广播
							APP_ERROR_CHECK(err_code);
								
						}
			}
			else if(!nrf_gpio_pin_read(MPR_IRQ))
			{
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"TSM12_Tuch!\r\n");
				
				SEGGER_RTT_printf(0,"MPR121_RUN: 0X%x.\r\n",MPR121_RUN);
				SEGGER_RTT_printf(0,"SPI0_Start: 0X%x.\r\n",SPI0_Start);
				SEGGER_RTT_printf(0,"Test_Ps_Mark: 0X%x.\r\n",Test_Ps_Mark);
				SEGGER_RTT_printf(0,"Controllable_Power_State: 0X%x.\r\n",Controllable_Power_State);
				SEGGER_RTT_printf(0,"F_Print_Run_Flag: 0X%x.\r\n",F_Print_Run_Flag);
				
				
				
				
	//			(!MPR121_RUN)&&
	//			(!nrf_gpio_pin_read(MPR_IRQ))
	//			&&(SPI0_Start==0)
	//			&&(Test_Ps_Mark==0)
	//			&&(Controllable_Power_State==0)
	//			&&(F_Print_Run_Flag==0)
							
				#endif
			}
		
	if ((!nrf_gpio_pin_read(BUTTON_2))&&(Test_Ps_Mark==0))  //判断按键2 (物理按键)状态
	{
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Button_On\r\n");
		#endif 
		
		//TSM12_Exit_Sleep_Mode();    //每次按下按钮重置一次触摸板

		Start_Key_Run();  //直接唤醒键盘
		
		nrf_gpio_pin_clear(LEDB);
//		nrf_gpio_pin_clear(LEDB1);
		
		nrf_gpio_pin_set(LEDR);
		//input_speak(5);
		nrf_delay_ms(20);
		nrf_gpio_pin_clear(LEDR);
		nrf_gpio_pin_clear(LEDB);
		if(MPR121_RUN)  //当键盘唤醒时继续点灯
		{
			nrf_gpio_pin_set(LEDB);
		}
//		nrf_gpio_pin_clear(LEDB1);
		
		
		if((!nrf_gpio_pin_read(BUTTON_2))&&(Init_Key_Num<1)&&(F_Print_Run_Flag==0))//&&(Controllable_Power_State==0))
		{
				Init_Key_Num =1;
				//input_speak(29); //仅在有效时触发声音
				err_code =app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//开启500ms间隔的定时软件定时器任务
				APP_ERROR_CHECK(err_code);
		}
//		if(Init_Key_Num>=6) //按下六次初始化按钮，清除绑定信息
//		{
//			Init_Key_Num =0;
//			Spi_Flash_Init();  //开启SPI_FLASH 通讯

//			SPIFlash_Erase_Sector(0,0);  //预先清空一次扇区0 以清空密码状态与绑定信息
//			CL_FlashWord_Num();  //清除Flash 目录信息(由于是重置设备，故清除目录信息)
//			CL_Flash_PassWord(); //重置设备时 清除Flash 密码信息 only Clean
//			CL_Flash_OPEN_DOOR_NOTE();  //重置设备时 清除Flash 开门记录信息(only Clean)
//			SPI_Flash_Busy();		
//			Devic_static_read();
//			
//			
//			
//			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
//			Spi_Flash_Sleep();		//完成Flash操作后，断开外部Flash连接
//		
//			nrf_gpio_pin_clear(LEDB);
//			nrf_gpio_pin_clear(LEDB1);
//			
//			nrf_gpio_pin_set(LEDR);
//			//input_speak(5);
//			nrf_delay_ms(500);
//			nrf_gpio_pin_clear(LEDR);
//			nrf_gpio_pin_clear(LEDB);
//			nrf_gpio_pin_clear(LEDB1);
//			
//			
//			input_speak(4);
//			ref_BLE_Adv();  //重置后刷新BLE广播名称
//								
//			#ifdef uart_start
//			SEGGER_RTT_printf(0,"BLE_KEY_Init!\r\n");
//			#endif
//			
//		}
		   // APP_ERROR_CHECK(err_code);		
		//nrf_gpio_pin_set(LEDR);//开灯
	}
	
	if(nrf_gpio_pin_read(SKY1311_IRQ_PIN)&&(SPI0_Start==0)&&(Test_Ps_Mark==0))//&&(F_Print_Run_Flag==0)&&(Controllable_Power_State==0))  //当SKY1311检测到场强变化时 开始尝试读卡
	{
		
	//	input_speak(2);
		
		
		#ifdef uart_start 
		SEGGER_RTT_printf(0,"Sky1311_IRQ!\r\n");
		#endif
		Spi_Flash_Init();  //开启SPI0接口
			irq_sta = sky1311ReadReg(ADDR_IRQ_STA);  // 读中断寄存器 确认是否为检卡中断
			if ((0==irq_sta) &&                      // 没有检测到中断寄存器跳转且
				 (0!=SKY1311_IRQ_READ())&&(cardStatus==GOTOSLEEP)) // IRQ 引脚变高电平
			{
					#ifdef Key_By_MPR121
					if(MPR121_RUN) //当键盘唤醒时读卡时先暂停键盘刷新
					{
						
							app_timer_stop(Read_My_Key);
					
					}
					#endif
					
				checkCardDeinit();          // 停止自动检卡
				read_TypeA_Data[0] =0xff;
				Type_AB_UID[0] =0xff;
					if(
							 ((Ok == Type_A_Rrun()||Ok == Type_B_Rrun())||(Ok == Type_A_Null_Key_Rrun()))
						) //完成A卡读取
					{
						
						//每次读取完卡片都重新写入一次卡数据开门记录详情
						
		//			extern u8 Op_Door_Buff[];				//用于缓存开门记录数据 待写入缓存
						
					for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff 长度
						Op_Door_Buff[i1] =0;									
			
						if(Type_AB_UID[0]==0x01)		//当数据为 A 卡时
							Op_Door_Buff[5] = 4;		//Type_A 卡长度为 4				
						else if(Type_AB_UID[0]==0X02)	//当数据为 B 卡时
							Op_Door_Buff[5] = 8;		//Type_B 卡长度为8
						for(uint8_t i1=0;i1<Op_Door_Buff[5];i1++)
							Op_Door_Buff[i1+6]=Type_AB_UID[i1+1];
						
						
						//input_speak(1);
						cardStatus = WAITREMOVE;  //标记读卡完成
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Card_Read_Ok!\r\n");
						
						#endif
						
						//判断 读到的是 A 卡 还是 B 卡
						if(Type_AB_UID[0]==0x01)  	  //当读到的卡为 A 卡时 
						{
								Devic_static_read();			//判断数据卡前先更新绑定信息
							if((Data_Card_Run()==1)&&(TIME_STATE==true))  //当数据卡满足时
							{
								
									#ifdef uart_start
								
									SEGGER_RTT_printf(0,"Data_Card_Open_Door!\r\n");
								
									#endif
								
									nrf_gpio_pin_set(LEDB);
								//	nrf_gpio_pin_set(LEDB1);  //直接开灯
									
									for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)
									{
										read_TypeA_Data[i] =0XFF;
									}
									Lock_Open();
									
									switch(DatCrd_AU)
									{
										case 1:  //管理员
										ADD_Op_Door_Note(0,0x0103,Unix_T);  //写入数据 卡(自定义卡)开门的操作记录,
										break;
										
										case 2:   //用户
											ADD_Op_Door_Note(0,0x0203,Unix_T);  //写入数据 卡(自定义卡)开门的操作记录,
										break;
										
										default :  //其他情况
											ADD_Op_Door_Note(0,0x03,Unix_T);  //写入数据 卡(自定义卡)开门的操作记录,
										break;
										
									}
									
									
									
									#ifdef uart_start
										
									SEGGER_RTT_printf(0,"TypeA_Card_Open_Door!\r\n");	
										
									#endif
										
									Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
									Start_Key_Run();  //直接唤醒键盘的函数
									Spi_Flash_Init();
								
							}
							else
							{
							
							//先判断此UID号本地是否存在
							read_TypeAB_SN = Need_TypeA_Data_SN(Type_AB_UID+1);  //指针偏移一位 跳过 AB 卡指示位
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Type_AB_UID: %x\r\n",read_TypeAB_SN);
							
							#endif
							//当 设备绑定时 读取到有效SN 码表示有本地数据，准备开门 当在手动发卡模式下（Start_Manual_Add_Card>0 时），跳过开门
							if((read_TypeAB_SN>0)&&(Start_Manual_Add_Card==0)&&((read_TypeAB_SN&0X1000000)||(read_TypeAB_SN&0x2000000))&&(Devic_Static==true))
							{
								
									Get_Time();
									
									if(TIME_STATE==true)  //当时间有效时
									{
									
										Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳	
									
								  	if((TypeAB_Unix_T[0]==Admin_TypeA_Card)||(TypeAB_Unix_T[0]==User_TypeA_Card))  //当时间戳字符表满足 管理员A 卡 或 用户A卡时
										 {
											
												TypeAB_Start_Unix = TypeAB_Unix_T[1];
												TypeAB_Start_Unix<<=8;
												
												TypeAB_Start_Unix |= TypeAB_Unix_T[2];
												TypeAB_Start_Unix<<=8;
												
												TypeAB_Start_Unix |= TypeAB_Unix_T[3];
												TypeAB_Start_Unix<<=8;
												
												TypeAB_Start_Unix |= TypeAB_Unix_T[4];
												
												#ifdef uart_start
												SEGGER_RTT_printf(0,"TypeAB_Start_Unix: %x\r\n",TypeAB_Start_Unix);
												#endif
												
												TypeAB_Stop_Unix = TypeAB_Unix_T[5];
												TypeAB_Stop_Unix<<=8;
												
												TypeAB_Stop_Unix |= TypeAB_Unix_T[6];
												TypeAB_Stop_Unix<<=8;
												
												TypeAB_Stop_Unix |= TypeAB_Unix_T[7];
												TypeAB_Stop_Unix<<=8;
												
												TypeAB_Stop_Unix |= TypeAB_Unix_T[8];
												
												#ifdef uart_start
												SEGGER_RTT_printf(0,"TypeAB_Stop_Unix: %x\r\n",TypeAB_Stop_Unix);
												SEGGER_RTT_printf(0,"TypeAB_AU: %x\r\n",TypeAB_Unix_T[0]);
												
												#endif
												
												
												
												nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口			//判断是否反锁
												nrf_delay_ms(1); //等待稳定
												
												if((nrf_gpio_pin_read(5)==0)&&(TypeAB_Unix_T[0]==User_TypeA_Card))   //当门反锁时 不允许开门
												{
													
											//		nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
													
													input_speak(17); //语音提示已反锁
													nrf_delay_ms(1200);
													
												}					
												if(((Unix_T<TypeAB_Stop_Unix)&&(Unix_T>TypeAB_Start_Unix)&&(TypeAB_Unix_T[0]==Admin_TypeA_Card))||
													((Unix_T<TypeAB_Stop_Unix)&&(Unix_T>TypeAB_Start_Unix)&&(TypeAB_Unix_T[0]==User_TypeA_Card)&&(nrf_gpio_pin_read(5)==1)))  //时间有效 且当用户卡是未反锁，允许 开门
													{
														
														nrf_gpio_pin_set(LEDB);
	//										//			nrf_gpio_pin_set(LEDB1);  //直接开灯
														
														Lock_Open();
														//写入对应类型开门记录
														
														switch(TypeAB_Unix_T[0])
														{
															
															case Admin_TypeA_Card: 		//管理员A卡
																
															ADD_Op_Door_Note(read_TypeAB_SN,0x0102,Unix_T);  //写入A 卡开门的操作记录(自定义卡)  高字节01 表示管理员
														
																break;
															
															case User_TypeA_Card:			//用户A卡
																
															ADD_Op_Door_Note(read_TypeAB_SN,0x0202,Unix_T);  //写入A 卡开门的操作记录(自定义卡)  高字节02 表示用户
														
																break;
															
														}
														
														

														
														#ifdef uart_start
															
														SEGGER_RTT_printf(0,"TypeA_Card_Open_Door!\r\n");	
															
														#endif
															
														Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
														Start_Key_Run();  //直接唤醒键盘的函数
														Spi_Flash_Init();
																				
													}
													else if((TypeAB_Unix_T[0]==Admin_TypeA_Card)||((TypeAB_Unix_T[0]==User_TypeA_Card)&&(nrf_gpio_pin_read(5)==1))) //时间过期 或未生效 且用户卡时门未反锁
													{
															input_speak(16); //时间无效，提示卡片到期
															nrf_gpio_pin_clear(LEDB);
	//														nrf_gpio_pin_clear(LEDB1);
															nrf_gpio_pin_set(LEDR);
															nrf_delay_ms(800);
															nrf_gpio_pin_clear(LEDR);
															nrf_gpio_pin_set(LEDB);
													//		nrf_gpio_pin_set(LEDB1);
															
															Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
															Start_Key_Run();  //直接唤醒键盘的函数
															Spi_Flash_Init();
	//														nrf_delay_ms(500);
	//														input_speak(10);  //额外提示操作失败表示内部时间错误
													}
													
													nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);   //重置  反锁标志位
													
												
												
											}
											else if(Start_Manual_Add_Card==0) //当卡片存在，时间无效时 提示错误
											{
														input_speak(10); //卡片无效，提示操作失败
														nrf_gpio_pin_clear(LEDB);
//														nrf_gpio_pin_clear(LEDB1);
														nrf_gpio_pin_set(LEDR);
														nrf_delay_ms(800);
														nrf_gpio_pin_clear(LEDR);
														nrf_gpio_pin_set(LEDB);
											//			nrf_gpio_pin_set(LEDB1);
														
												
															Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
															Start_Key_Run();  //直接唤醒键盘的函数
															Spi_Flash_Init();
//														nrf_delay_ms(500);
//														input_speak(10);  //额外提示操作失败表示内部时间错误
											}
								}
								else  //系统时间无效时
								{
											input_speak(16); //时间无效，提示卡片已过期
											nrf_gpio_pin_clear(LEDB);
//											nrf_gpio_pin_clear(LEDB1);
											nrf_gpio_pin_set(LEDR);
											nrf_delay_ms(800);
											nrf_gpio_pin_clear(LEDR);
											nrf_gpio_pin_set(LEDB);
										//	nrf_gpio_pin_set(LEDB1);
											
											nrf_delay_ms(500);
											input_speak(10);  //额外提示操作失败表示内部时间错误
									
											Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
											Start_Key_Run();  //直接唤醒键盘的函数
											Spi_Flash_Init();
														
								}
							}
							else if(Devic_Static==false)  //当设备未绑定时，可直接用加密过的卡开门
							{
									nrf_gpio_pin_set(LEDB);
							//		nrf_gpio_pin_set(LEDB1);  //直接开灯
								
									Lock_Open();
												
									#ifdef uart_start
										
									SEGGER_RTT_printf(0,"TypeA_Card_Open_Door!\r\n");	
										
									#endif
								
									Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
									Start_Key_Run();  //直接唤醒键盘的函数
									Spi_Flash_Init();
							}
							else if(Start_Manual_Add_Card==0)  // 当卡片不存且不在手动发卡状态时 提示错误
							{
											input_speak(10); //卡片无效，提示操作失败
											nrf_gpio_pin_clear(LEDB);
//											nrf_gpio_pin_clear(LEDB1);
											nrf_gpio_pin_set(LEDR);
											nrf_delay_ms(800);
											nrf_gpio_pin_clear(LEDR);
											nrf_gpio_pin_set(LEDB);
										//	nrf_gpio_pin_set(LEDB1);
								
											Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
											Start_Key_Run();  //直接唤醒键盘的函数
											Spi_Flash_Init();
							}
							else if(Start_Manual_Add_Card!=0)
							{
										Marnual_Add_Run();//自助发卡

							}
						}
							
						}
						else if (Type_AB_UID[0]==0x02)  //当读到的卡为 B 卡时
						{
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Need TypeB_Card!\r\n");
							
							#endif
							
								Get_Time();
							
							if(Devic_Static==true)  //当时间有效时
							{
								
								
								read_TypeAB_SN = Need_TypeB_Data_SN(Type_AB_UID+1);
								
								#ifdef uart_start
								
								SEGGER_RTT_printf(0,"Type_AB_UID: %x\r\n",read_TypeAB_SN);
								SEGGER_RTT_printf(0,"TIME_STATE: %x\r\n",TIME_STATE);
								SEGGER_RTT_printf(0,"Start_Manual_Add_Card: %x\r\n",Start_Manual_Add_Card);
								
								
								#endif
								
								if((read_TypeAB_SN>0)&&
									(Start_Manual_Add_Card==0)&&
									((read_TypeAB_SN&0X3000000)||(read_TypeAB_SN&0x4000000))
									&&(TIME_STATE==true))
								{
									Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳	
								
										if((TypeAB_Unix_T[0]==Admin_TypeB_Card)||(TypeAB_Unix_T[0]==User_TypeB_Card))  //当时间戳字符表满足 管理员B 卡 或 用户B卡时
										 {
											 
														TypeAB_Start_Unix = TypeAB_Unix_T[1];
														TypeAB_Start_Unix<<=8;
														
														TypeAB_Start_Unix |= TypeAB_Unix_T[2];
														TypeAB_Start_Unix<<=8;
														
														TypeAB_Start_Unix |= TypeAB_Unix_T[3];
														TypeAB_Start_Unix<<=8;
														
														TypeAB_Start_Unix |= TypeAB_Unix_T[4];
														
														#ifdef uart_start
														SEGGER_RTT_printf(0,"TypeAB_Start_Unix: %x\r\n",TypeAB_Start_Unix);
														#endif
														
														TypeAB_Stop_Unix = TypeAB_Unix_T[5];
														TypeAB_Stop_Unix<<=8;
														
														TypeAB_Stop_Unix |= TypeAB_Unix_T[6];
														TypeAB_Stop_Unix<<=8;
														
														TypeAB_Stop_Unix |= TypeAB_Unix_T[7];
														TypeAB_Stop_Unix<<=8;
														
														TypeAB_Stop_Unix |= TypeAB_Unix_T[8];
														
														#ifdef uart_start
														SEGGER_RTT_printf(0,"TypeAB_Stop_Unix: %x\r\n",TypeAB_Stop_Unix);
														#endif
														
														nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口			//判断是否反锁
														nrf_delay_ms(1); //等待稳定
														
														if((nrf_gpio_pin_read(5)==0)&&(TypeAB_Unix_T[0]==User_TypeB_Card))   //当门反锁时 不允许开门
														{
															
													//		nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
															
															input_speak(17); //语音提示已反锁
															nrf_delay_ms(1200);
															
														}
											
													if(((Unix_T<TypeAB_Stop_Unix)&&(Unix_T>TypeAB_Start_Unix)&&(TypeAB_Unix_T[0]==Admin_TypeB_Card))||
															((Unix_T<TypeAB_Stop_Unix)&&(Unix_T>TypeAB_Start_Unix)&&(TypeAB_Unix_T[0]==User_TypeB_Card)&&(nrf_gpio_pin_read(5)==1))
														)  //时间有效 开门
														{
															nrf_gpio_pin_set(LEDB);
															//nrf_gpio_pin_set(LEDB1);  //直接开灯
															
														Lock_Open();
														
														
														switch (TypeAB_Unix_T[0])
														{
															
															case Admin_TypeB_Card:
																	
															ADD_Op_Door_Note(read_TypeAB_SN,0x0102,Unix_T);  //写入B 卡(自定义卡)开门的操作记录高位 01 表示管理员
														
																break;
															
															case User_TypeB_Card:
															
															ADD_Op_Door_Note(read_TypeAB_SN,0x0202,Unix_T);  //写入B 卡(自定义卡)开门的操作记录高位 02 表示用户
														
																break;
															
															
															
														}
														
															
														#ifdef uart_start
															
														SEGGER_RTT_printf(0,"TypeA_Card_Open_Door!\r\n");	
															
														#endif
															
														Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
														Start_Key_Run();  //直接唤醒键盘的函数
														Spi_Flash_Init();
																					
														}
														else if((TypeAB_Unix_T[0]==Admin_TypeB_Card)||((TypeAB_Unix_T[0]==User_TypeB_Card)&&(nrf_gpio_pin_read(5)==1))) //时间过期 或未生效 且用户卡的情况下门未反锁
														{
																input_speak(16); //时间无效，提示卡片到期
																nrf_gpio_pin_clear(LEDB);
//																nrf_gpio_pin_clear(LEDB1);
																nrf_gpio_pin_set(LEDR);
																nrf_delay_ms(800);
																nrf_gpio_pin_clear(LEDR);
																nrf_gpio_pin_set(LEDB);
														//		nrf_gpio_pin_set(LEDB1);
																
																Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
																Start_Key_Run();  //直接唤醒键盘的函数
																Spi_Flash_Init();
		//														nrf_delay_ms(500);
		//														input_speak(10);  //额外提示操作失败表示内部时间错误
														}
														
													nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);   //重置  反锁标志位
												
											 
										 }
										 else
										 {
												input_speak(10); //卡无效，提示操作失败
												nrf_gpio_pin_clear(LEDB);
//												nrf_gpio_pin_clear(LEDB1);
												nrf_gpio_pin_set(LEDR);
												nrf_delay_ms(800);
												nrf_gpio_pin_clear(LEDR);
												nrf_gpio_pin_set(LEDB);
											//	nrf_gpio_pin_set(LEDB1);
											
												Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
												Start_Key_Run();  //直接唤醒键盘的函数
												Spi_Flash_Init();
										 }
								
									}
									else if (TIME_STATE==false) //时间失效时
									{
										
										input_speak(16); //时间无效，提示卡片已过期
										nrf_gpio_pin_clear(LEDB);
//										nrf_gpio_pin_clear(LEDB1);
										nrf_gpio_pin_set(LEDR);
										nrf_delay_ms(800);
										nrf_gpio_pin_clear(LEDR);
										nrf_gpio_pin_set(LEDB);
								//		nrf_gpio_pin_set(LEDB1);
										
										nrf_delay_ms(500);
										input_speak(10);  //额外提示操作失败表示内部时间错误
								
										Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
										Start_Key_Run();  //直接唤醒键盘的函数
										Spi_Flash_Init();
						
									}
									else if(Start_Manual_Add_Card==0)
									{
											input_speak(10); //卡无效，提示操作失败
											nrf_gpio_pin_clear(LEDB);
//											nrf_gpio_pin_clear(LEDB1);
											nrf_gpio_pin_set(LEDR);
											nrf_delay_ms(800);
											nrf_gpio_pin_clear(LEDR);
											nrf_gpio_pin_set(LEDB);
									//		nrf_gpio_pin_set(LEDB1);
										
											Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
											Start_Key_Run();  //直接唤醒键盘的函数
											Spi_Flash_Init();
									}

							}
							
//							else if(Devic_Static==false)  //当设备未绑定时 检测到身份证直接开门
//							{
//								Lock_Open();
//								Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
//								Start_Key_Run();  //直接唤醒键盘的函数
//								Spi_Flash_Init();
//							}
							else 
							{
								
								input_speak(16); //时间无效，提示卡片已过期
								nrf_gpio_pin_clear(LEDB);
//							nrf_gpio_pin_clear(LEDB1);
								nrf_gpio_pin_set(LEDR);          
								nrf_delay_ms(800);
								nrf_gpio_pin_clear(LEDR);
								nrf_gpio_pin_set(LEDB);
								
//							nrf_gpio_pin_set(LEDB1);
//							nrf_delay_ms(500);
//							input_speak(10);  //额外提示操作失败表示内部时间错误
								
								Spi_Flash_Sleep();  //唤醒前重启 SPI 否则无法唤醒
								Start_Key_Run();  //直接唤醒键盘的函数
								Spi_Flash_Init();
								  
							}
						
							Marnual_Add_Run();//自助发卡

						}
						
		
						for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//最后清空缓存数据
						{
							
							read_TypeA_Data[i] =0XFF;
						
						}		
	
						
						app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//开启500ms间隔的定时软件定时器任务
						
					}
					else //未读取到有效的A/B卡
					{
						
						cardStatus = WAITREMOVE; 
						app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//开启500ms间隔的定时软件定时器任务
						
						if(MPR121_RUN==0)			//在待机时 计数
						Read_Card_Error_Num++;			// 读取卡片错误次数 +1
						else
							Read_Card_Error_Num =0;  //在键盘触发时清空
						
						
					}
					#ifdef Key_By_MPR121
					if(MPR121_RUN)
					{
						app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	
					}
					#endif
					
			}
			else				//当中断不是检卡中断，清除所有中断，配置sky1311重新开始检测
			{
					irqClearAll();              // 清除SKY1311的所有中断
					checkCardInit(rcValue);     // 配置进入自动检卡状态
					Spi_Flash_Sleep();
			}
			
			Spi_Flash_Sleep();
	}
	else if((SPI0_Start==1)&&(nrf_gpio_pin_read(SKY1311_IRQ_PIN))&&(Test_Ps_Mark==0)) //当SPI0被占用时，启动定时器进入检测卡状态
	{
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Sky1311_SPI_Busy!!\r\n");
		#endif
		SKY1311_Error =1;
		
		cardStatus = CARDREMOVED; 
		
		app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//开启500ms间隔的定时软件定时器任务
						
//		SKY1311_ENABLE();
//		irqClearAll();
//		SKY1311_DISABLE();
		
	}
	
		//MSR_121_read_key();  // 获取key值
	
	//防拆
	
	if((nrf_gpio_pin_read(13)==0)&&(Test_Ps_Mark==0))				//当防拆被触发
	{
		#ifdef FC_OP_Key
		
		FD_Key =1;			//防拆标志置1
		nrf_delay_ms(200);
		input_speak(20);				//提示非法操作
	
		if(SPI0_Start==1)			// 当  SPI0 在运行时  
				{
						;		//运行时不做唤醒按键板响应
				}
				else
				{
					Start_Key_Run();			//当SPI0 未运行时，直接唤醒键盘
				}
		#else
				Lock_Open();				//防拆触发开门
		#endif
	}
	
	#ifdef PS_Enable 
	
	if(nrf_gpio_pin_read(F_Print_IRQ)&&(Controllable_Power_State==0)&&(F_Print_Run_Flag==0)&&(CL_PS_Print_Now==0)&&(F_print_Enable==1))					//指纹触发中断
	{
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"F_Print_IRQ_Input!\r\n");
		
		#endif
		Controllable_Power_No();		//给指纹模块上电
		
		if(Devic_Static==false)  //当设备未绑定时，可直接用指纹开门(不对数据做任何处理，开门即使指纹进入睡眠状态)
		{
				nrf_gpio_pin_set(LEDB);
		//		nrf_gpio_pin_set(LEDB1);  //直接开灯
			
				Lock_Open();
							
				#ifdef uart_start
					
				SEGGER_RTT_printf(0,"F_Print_Open_Door!\r\n");	
					
				#endif
			
				if(SPI0_Start==1)			// 当  SPI0 在运行时  
				{
						;		//运行时不做唤醒按键板响应
				}
				else
				{
					Start_Key_Run();			//当SPI0 未运行时，直接唤醒键盘
				}
			F_Print_Run_Flag =1;			//表示指纹头进入运行状态
		}
		else
		{
			
			F_Print_Run_Flag =1;			//使指纹头进入运行状态
			
		}
	}
	
	#endif
	
}

uint8_t WAITEMOVE_Time =0;

void Start_Key_Run()  //直接唤醒键盘的函数
{
		  uint32_t err_code;
  //	uint32_t it1,it2,i;
	if((!MPR121_RUN)&&(SPI0_Start==0))
	{
			
			err_code = app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);				//开始读键盘的定时器任务
	    APP_ERROR_CHECK(err_code);
			
			if(SPI0_Start==0) //当SPI0关闭时，将扫描阀值调整到唤醒后的值
			{
			
				nrf_gpio_pin_set(LEDB);//开灯
				//nrf_gpio_pin_set(LEDB1);//开灯
				
				#ifdef key_By_MPR121
				mpr121_Run(); //同时启动键盘，用于建立键盘电场环境
				nrf_delay_ms(500);	
				mpr121_Run();	 //再启动键盘以清空期间的误触
				#endif
				
				#ifdef Key_By_TSM12c
				
				TSM12_Exit_Sleep_Mode();//	MPR121_RUN =1;
				
				
				
				#endif		
				
				
				Spi_Flash_Init();
	
				sky1311Reset();
				
				irqClearAll();              // 清除SKY1311的所有中断
				checkCardInit(LEDOP_RC_VALUE);    // 配置进入自动检卡状态
				cardStatus = GOTOSLEEP;
				
				nrf_delay_ms(1000);  //预设500ms 的电场波动时间，期间的感应值无效
				
				irqClearAll();
				
				Spi_Flash_Sleep();
				
				#ifdef uart_start
					
				SEGGER_RTT_printf(0,"Check_Card_Start_LEDOP_Value!\r\n");  //尝试开启亮灯时的寻卡阀值
				
				#endif
				
			}
			
			if(Devic_Static==false)  //当设备未绑定时
				{
					
					DEVICE_NAME[2] = Devic_Name_E;
							//GAP一些参数的设置，设置设备名，设置PPCP(外围设备首选链接参数)。
				//(主机连上某个蓝牙设备后可以从Gameric Access Service 中看到设置的这些参数)
					sd_ble_gap_adv_stop();
					gap_params_init();  //设置蓝牙名	  
					//advertising_init();
					AC_advertising_init();  //设置广播间隔
					err_code = ble_advertising_start(BLE_ADV_MODE_FAST); //同时开启广播
					APP_ERROR_CHECK(err_code);
						
				}
		}
	
	MPR121_T =0;  //清空 键盘自动待机计时
		
		
}


uint32_t Wait_Card_Remove_Timeout =0; //等待卡离开超时
const uint8_t Wait_Card_Remove_Timeout_Value = 6;
uint16_t Wait_SPI0_Run_Time =0;		//定义等待 SPI0 时间超时
const uint16_t SPI0_Run_Time_Out =50;

void SKY1311_State() //SKY1311状态切换
{
	
	switch (cardStatus)
	{
		case CARDREMOVED :  //没卡在场内 重新开始低功耗寻卡
			if(SPI0_Start==0)
			{
				Wait_SPI0_Run_Time =0;
				Spi_Flash_Init();
				sky1311Reset();
				irqClearAll();              // 清除SKY1311的所有中断
				
				if(MPR121_RUN)
					rcValue = LEDOP_RC_VALUE;
				else
					rcValue = CL_LED_Cheak_RC_Value();
				
				
				checkCardInit(rcValue);    // 配置进入自动检卡状态
			//	cardStatus = GOTOSLEEP;
							
				Spi_Flash_Sleep();
				cardStatus = GOTOSLEEP;
				
			}
			else
			{
				#ifdef uart_start
					SEGGER_RTT_printf(0,"SPI0_Start: %x!\r\n",SPI0_Start);
				#endif
				Wait_SPI0_Run_Time++;
				if(Wait_SPI0_Run_Time>=SPI0_Run_Time_Out)			//等待 SPI0 超时
				{
					Wait_SPI0_Run_Time =0;		//清空标志位
					Spi_Flash_Sleep();			//强制停止 SPI0 运行
				}
			}
			break;
		
		case GOTOSLEEP : //SKY1311休眠
			
			break;
		
		case HASCARDIN :  //当有卡在场内
		
//				Wait_Card_Remove_Timeout++;
//		if(Wait_Card_Remove_Timeout>5)
//		{
//				Wait_Card_Remove_Timeout =0;
//				sky1311Reset();
//				if(SKY1311_RC_VALUE()==0);  //当初始化失败时
//				else 											//获取到新的值时
//				{
//						sky1311Reset();
////						cardStatus =WAITREMOVE;
//						Wait_Card_Remove_Timeout =0;
//					
//						irqClearAll();              // 清除SKY1311的所有中断
//						checkCardInit(LEDCL_RC_VALUE);    // 配置进入自动检卡状态
//						irqClearAll();
//						cardStatus = GOTOSLEEP;
//										
//				}
//		}
		
		break;
		
		case WAITREMOVE:  //等待卡离开
			
			if(SPI0_Start==0)
			{
				if(MPR121_RUN) //当键盘唤醒时读卡时先暂停键盘刷新
					{
						app_timer_stop(Read_My_Key);
					}
				
				Spi_Flash_Init();
				
				if(MPR121_RUN)
					rcValue = LEDOP_RC_VALUE;
				else
					rcValue = LEDCL_RC_VALUE;
				
				if(checkCardRemove(rcValue))
				{
					cardStatus = CARDREMOVED; //标记卡已经离开进入 CARDREMOVED 状态
					//rcValue = sky1311RCFreqCali();
					sky1311Reset();
					cardStatus =CARDREMOVED;
					Wait_Card_Remove_Timeout =0;
					WAITEMOVE_Time =0;
					
				}
				else
					WAITEMOVE_Time++;
				
					if( WAITEMOVE_Time>=2 )
					{
						
						WAITEMOVE_Time =0;
						cardStatus = CARDREMOVED; //标记卡已经离开进入 CARDREMOVED 状态
						//rcValue = sky1311RCFreqCali();
						sky1311Reset();
						cardStatus =CARDREMOVED;
						Wait_Card_Remove_Timeout =0;
						
					}
				
//				else
//				{
//					Wait_Card_Remove_Timeout++;
//					if(Wait_Card_Remove_Timeout>Wait_Card_Remove_Timeout_Value)  //当超时时 尝试重新获取
//					{
//						
//						
//							cardStatus = HASCARDIN;
//							Wait_Card_Remove_Timeout =0;
////							while(SKY1311_RC_VALUE()==0)nrf_delay_ms(2000);
////							sky1311Reset();
////							cardStatus =CARDREMOVED;
//					}
//				}
				
				if(MPR121_RUN)
					{
						
						app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);			//开始读键盘的定时器任务
						
					}
						Spi_Flash_Sleep();
			}
			
		break;
	}
	
	//	NRF_WDT_Feed(); //这里不用喂狗
}


/*

开启 手动载入卡的标志位 由外部 BLE_Data.c 中Manual_Add_Card 置位 并启动
Timer_1_Handler  开始等待用户刷卡录入
当值为 1 时 为录入 管理员A卡
当值为 2 时 为录入 管理员B卡
当值为 3 时 为录入 用户A卡
当值为 4 时 为录入 用户B卡

*/
extern uint32_t Manual_Add_Card_Start_Unix; //手动载入卡的起始时间
extern uint32_t Manual_Add_Card_Stop_Unix; //手动载入卡的结束时间

uint8_t Manual_Add_Card_Num =0;// 记录自助发卡轮询次数

uint8_t Type_AB_ID =0;   // Type AB ID 表示保存的 Type AB 卡的 ID号

uint8_t Manual_Err_num =0;



void Marnual_Add_Run()
{

	//自助发卡
	
	if ((Start_Manual_Add_Card>0)&&(cardStatus == WAITREMOVE)&& (Type_AB_UID[0]==0x01)&&(BLE_LINK==1))  //当读到有效 A 卡 且有自助发卡指令 蓝牙连接时
	{
			
		if(((Start_Manual_Add_Card&0x0F)==1)||((Start_Manual_Add_Card&0x0F)==2))  //当载入管理员卡时 在 A 卡管理卡区域写入
		{
			
					Spi_Flash_Init();
					
			  	Type_AB_ID =	Add_Flash_AD_TypeA_Data(Type_AB_UID+1,Manual_Add_Card_Start_Unix,Manual_Add_Card_Stop_Unix);
						//		 Card_ID = Add_Flash_AD_TypeA_Data (PassWord,Start_Unix,Stop_Unix);
					SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
					Spi_Flash_Sleep();
//					input_speak(9);

			if(Type_AB_ID!=0)
				{
					
					input_speak(9);
					Manual_Add_Card_ACK(0x01,Type_AB_ID,0X02);  //返回操作成功的报文
					
				}
				else
				{
				
					input_speak(10);
					Manual_Add_Card_ACK(0x02,Type_AB_ID,0);  //返回操作失败的报文
					
				}
						
				Start_Manual_Add_Card =0;
			
		}
		else if(((Start_Manual_Add_Card&0x0F)==3)||((Start_Manual_Add_Card&0x0F)==4)) //当载入用户卡时 在 A 卡用户卡区域写入
		{
			
					Spi_Flash_Init();
					
			  	Type_AB_ID =	Add_Flash_US_TypeA_Data(Type_AB_UID+1,Manual_Add_Card_Start_Unix,Manual_Add_Card_Stop_Unix);
					
					SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
					Spi_Flash_Sleep();
					//input_speak(9);
			
				if(Type_AB_ID!=0)
				{
					
					input_speak(9);
					Manual_Add_Card_ACK(0x01,Type_AB_ID,0X02);  //返回操作成功的报文
				
				}
				else
				{
				
					input_speak(10);
					Manual_Add_Card_ACK(0x02,Type_AB_ID,0);  //返回操作失败的报文
					
				}
				Start_Manual_Add_Card =0;
		}
		Manual_Err_num ++;
	}
	else if((Start_Manual_Add_Card>0)&&(cardStatus == WAITREMOVE)&&(Type_AB_UID[0]==0x02)&&(BLE_LINK==1))
	{
			
		Manual_Err_num++;
			if(((Start_Manual_Add_Card&0x0F)==1)||((Start_Manual_Add_Card&0x0F)==2))
			{
				
					Spi_Flash_Init();
					
			  	Type_AB_ID =	Add_Flash_AD_TypeB_Data(Type_AB_UID+1,Manual_Add_Card_Start_Unix,Manual_Add_Card_Stop_Unix);
					
					SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
					Spi_Flash_Sleep();
		//			input_speak(9);
				
					if(Type_AB_ID!=0)
					{
						input_speak(9); //操作成功
						Manual_Add_Card_ACK(0x01,Type_AB_ID,0X03);  //返回操作成功的报文
					}
					else
					{
						input_speak(10);  //操作失败
						Manual_Add_Card_ACK(0x02,Type_AB_ID,0X00);  //返回操作失败的报文
					}
					Start_Manual_Add_Card =0;
				
			}
			else if(((Start_Manual_Add_Card&0x0F)==3)||((Start_Manual_Add_Card&0x0F)==4))
			{
				
					Spi_Flash_Init();
					
			  	Type_AB_ID =	Add_Flash_US_TypeB_Data(Type_AB_UID+1,Manual_Add_Card_Start_Unix,Manual_Add_Card_Stop_Unix);
					
					SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
					Spi_Flash_Sleep();
				//	input_speak(9);
				
					if(Type_AB_ID!=0)
					{
						input_speak(9);
						Manual_Add_Card_ACK(0x01,Type_AB_ID,0X04);  //返回操作成功的报文
					}
					else
					{
						input_speak(10);
						Manual_Add_Card_ACK(0x02,Type_AB_ID,0X00);  //返回操作失败的报文
					}
					Start_Manual_Add_Card =0;
				
			}
	}
	else if(Manual_Err_num>5)  //当尝试次数超过五次
	{
		Manual_Err_num =0;
		Start_Manual_Add_Card =0;
		if(BLE_LINK==1)
		{
		
			if(BLE_LINK==1)
				Manual_Add_Card_ACK(0x02,0,0);  //返回操作失败的报文
			
		}
	}
	else
	{
			Manual_Err_num ++;
	}
}




uint8_t Init_Key_T =0;  //用于缓冲复位按键
uint8_t Lock_Open_EN =0;    //播放开关



void Timer_1_Handler(void *params)
{
	
	#ifdef uart_start
	SEGGER_RTT_printf(0,"\r\nTimer_1_Hander!\r\n");
	#endif
	
	if((Init_Key_Num>=1)&&(!nrf_gpio_pin_read(BUTTON_2)))
	{
		Init_Key_T++;
		if(Init_Key_T>5)
		{
			Init_Key_T =0;
			Init_Key_Num++;
	//		if((Init_Key_Num%2)==0)
			input_speak(29);
		}
	}
	else
	{
		Init_Key_Num =0;
		Init_Key_T =0;
	}
	
	if(Init_Key_Num>6) //经过六次初始化按钮轮询确定按下这么多时间，清除绑定信息
	{
		 
		Init_Key_Num =0;
		Spi_Flash_Init();  //开启SPI_FLASH 通讯

		SPIFlash_Erase_Sector(0,0);  //预先清空一次扇区0 以清空密码状态与绑定信息
		CL_FlashWord_Num();  //清除Flash 目录信息(由于是重置设备，故清除目录信息)
		CL_Flash_PassWord(); //重置设备时 清除Flash 密码信息 only Clean
		CL_Flash_OPEN_DOOR_NOTE();  //重置设备时 清除Flash 开门记录信息(only Clean)
		
		Erase_AD_TypeA_Sector();
		Erase_US_TypeA_Sector();
		Erase_AD_TypeB_Sector();
		Erase_US_TypeB_Sector();
		
		SPI_Flash_Busy();		
		Devic_static_read();
		
		Lock_Open_EN =0;					//获取是否开启未关好门语音数据

		
		SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
		Spi_Flash_Sleep();		//完成Flash操作后，断开外部Flash连接
	
		nrf_gpio_pin_clear(LEDB);
//		nrf_gpio_pin_clear(LEDB1);
		
		nrf_gpio_pin_set(LEDR);
		//input_speak(5);
		nrf_delay_ms(500);
		nrf_gpio_pin_clear(LEDR);
		nrf_gpio_pin_clear(LEDB);
//		nrf_gpio_pin_clear(LEDB1);
		
		if(MPR121_RUN)  //当键盘唤醒时继续点灯
		{
			nrf_gpio_pin_set(LEDB);
		}
		input_speak(4);
		ref_BLE_Adv();  //重置后刷新BLE广播名称
							
		#ifdef uart_start
		SEGGER_RTT_printf(0,"BLE_KEY_Init!\r\n");
		#endif
		
	}
	
	SKY1311_State();  //SKY1311_状态机
	
	if(
	(cardStatus==GOTOSLEEP)&&
	(Init_Key_Num==0))
	{
		
			app_timer_stop(Timer_1);
	
	}

}


/**@brief   Function for handling app_uart events. //用于处理 app_uart　事件的函数
 *
 * @details This function will receive a single character from the app_uart module and append it to 
 *          a string. The string will be be sent over BLE when the last character received was a 
 *          'new line' i.e '\n' (hex 0x0D) or if the string has reached a length of 
 *          @ref NUS_MAX_DATA_LENGTH.
这个函数将从app_uart 模块疏导一个字符，并将其附加到一个字符串里。当最后一个字符串被接收到时，字符串将会被发送出去，'新的行' 也就是 \n(hex 0x0D) 或者字符串已经达到了 NUS_MAX_DATA_LENGTH.

 */
/**@snippet [Handling the data received over UART] */ //处理通过UART接收的数据。  串口事件


//void uart_event_handle(app_uart_evt_t * p_event)
//{
//	
//	#ifdef uart_start
//    static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
//    static uint8_t index = 0;
//    uint32_t       err_code;

//    switch (p_event->evt_type)
//    {
//        case APP_UART_DATA_READY:
//            UNUSED_VARIABLE(app_uart_get(&data_array[index]));
//            index++;

//            if ((data_array[index - 1] == '\n') || (index >= (BLE_NUS_MAX_DATA_LEN)))
//            {
//                err_code = ble_nus_string_send(&m_nus, data_array, index);  //BLE数据发送
//                if (err_code != NRF_ERROR_INVALID_STATE)
//                {
//                    APP_ERROR_CHECK(err_code);
//                }
//                
//                index = 0;
//            }
//            break;

//        case APP_UART_COMMUNICATION_ERROR:
//            APP_ERROR_HANDLER(p_event->data.error_communication);
//            break;

//        case APP_UART_FIFO_ERROR:
//            APP_ERROR_HANDLER(p_event->data.error_code);
//            break;

//        default:
//            break;
//    }

//	#endif
//}


/**@snippet [Handling the data received over UART] */ //处理通过UART接收的数据。

 
/**@brief  Function for initializing the UART module. //初始化UART模块的功能。
 */
/**@snippet [UART Initialization] */
/*
初始化uart设置输入输出引脚，是否关闭流控。一般使用官方例子的时候都要先将流控关掉，HWFC为False。然后
打开uart的接收中断，打开uart模块的中断功能，以及设置优先级。
波特率在simple_uart_config中设置，该函数设置完引脚后使能uart开启uart的接收和发送功能。
*/

uint8_t UART_RUN =0;   //用于表示串口开启状态

 void uart_init(void)
{
	
	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
	
	nrf_delay_ms(10);   //等待串口稳定
	
	UART_RUN =1;
	
//    uint32_t                     err_code;
//    const app_uart_comm_params_t comm_params =
//    {
//        RX_PIN_NUMBER,
//        TX_PIN_NUMBER,
//        RTS_PIN_NUMBER,
//        CTS_PIN_NUMBER,
//        APP_UART_FLOW_CONTROL_DISABLED,  //流控制 ENABLED 使能
//        false,
//        UART_BAUDRATE_BAUDRATE_Baud115200
//    };

		
			
//    APP_UART_FIFO_INIT( &comm_params,
//                       UART_RX_BUF_SIZE,
//                       UART_TX_BUF_SIZE,
//                       uart_event_handle,  //串口回调
//                       APP_IRQ_PRIORITY_LOW,
//                       err_code);
//    APP_ERROR_CHECK(err_code);
	//simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
}
/**@snippet [UART Initialization] */ //UART初始化


//开启串口，带定时器关闭

void Uart_Start_TC()
{
	
	uint32_t err_code;
	
	
	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
	nrf_delay_ms(10);
	UART_RUN = 1;
	
	err_code = app_timer_start(Uart_Close,APP_TIMER_TICKS(Uart_Close_Time_Size,APP_TIMER_PRESCALER),NULL);   //注册自动关闭串口的定时器任务
	
	APP_ERROR_CHECK(err_code);
	
}

void close_uart()
{
		UART_RUN =0;
		NRF_UART0->ENABLE    				= (UART_ENABLE_ENABLE_Disabled<<UART_ENABLE_ENABLE_Pos);  //关闭串口

}



/**@brief Function for initializing the Advertising functionality.
设置广播数据以及扫描响应数据
 */
static void advertising_init(void)
{
	 
		uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;

    // Build advertising data struct to pass into @ref ble_advertising_init. 架构广播数据 以传输到 ble_advertising_init
    memset(&advdata, 0, sizeof(advdata));  //清空结构体
     advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = false;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;  //一般可发现模式

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = m_adv_uuids;

    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = BLE_ADV_FAST_ENABLED;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
		options.ble_adv_directed_slow_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = ble_advertising_init(&advdata, &scanrsp, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
		
}

static void AC_advertising_init(void)
{

		uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;

    // Build advertising data struct to pass into @ref ble_advertising_init. 架构广播数据 以传输到 ble_advertising_init
    memset(&advdata, 0, sizeof(advdata));  //清空结构体
     advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = false;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;  //一般可发现模式

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = m_adv_uuids;

    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = BLE_ADV_FAST_ENABLED;
    options.ble_adv_fast_interval = APP_AC_ADV_INTERVAL;
		options.ble_adv_directed_slow_interval =APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = ble_advertising_init(&advdata, &scanrsp, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);

}

 void advertising_IDLE_Init(void)
{

		uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;

    // Build advertising data struct to pass into @ref ble_advertising_init. 架构广播数据 以传输到 ble_advertising_init
  memset(&advdata, 0, sizeof(advdata));  //清空结构体
	advdata.name_type          = BLE_ADVDATA_FULL_NAME;
	advdata.include_appearance = false;
	advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;  //一般可发现模式

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = m_adv_uuids;

   ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = BLE_ADV_FAST_DISABLED;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    err_code = ble_advertising_init(&advdata, &scanrsp, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);

}




/**@brief Function for initializing buttons and leds.
 * 初始化按键和LED的功能
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */

static void buttons_leds_init(bool * p_erase_bonds)
{
		bsp_event_t startup_event;
	//按键和灯初始化函数，APP_TIMER_TICKS(100,APP_TIMER_PRESCALER)：消抖时间 bsp_event_handler: 回调函数  
	//在对应IO触发，在消抖后回调 bsp_event_handler
	
		uint32_t err_code = bsp_init(BSP_INIT_BUTTONS,//BSP_INIT_LED | 
																 APP_TIMER_TICKS(10, APP_TIMER_PRESCALER), 
																 bsp_event_handler);
		APP_ERROR_CHECK(err_code);
	
		err_code = bsp_btn_ble_init(NULL, &startup_event);
		APP_ERROR_CHECK(err_code);

		*p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}

/**@brief Function for placing the application in low power state while waiting for events. //在等待事件时将应用程序置于低功耗状态的功能。
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

//修改MAC地址
 void mac_set(void)
{
	ble_gap_addr_t addr;
	uint32_t err_code = sd_ble_gap_address_get(&addr);  //获取MAC地址
	APP_ERROR_CHECK(err_code);
	
	addr.addr[0] =0x33;
	addr.addr[1] =0x33;
	addr.addr[2] =0x33;
	addr.addr[3] =0x33;
	addr.addr[4] =0x33;
	
	err_code = 
	sd_ble_gap_address_set (BLE_GAP_ADDR_CYCLE_MODE_NONE,
	&addr);
	APP_ERROR_CHECK(err_code);
}
 
//#ifdef Debug 

uint8_t MPR121_RUN; //状态
uint16_t MPR121_T;   //计时，

uint32_t ADC_Value;
uint32_t VCC_Value,VCC_Value_H,VCC_Value_L1,VCC_Value_L2; //运算结果
#define VDD_Value 120000  //芯片ADC参考电压*10000倍 方便运算
#define RES_d  10 //采样电压倍数

uint16_t ADC_Div;  //ADC最小分辨率
uint16_t MPR_Del;  //在低功耗模式下，定时重启键盘板 
uint16_t MPR_Value;  //获取的值

uint8_t MOT_RL =0;
uint16_t CL_Init_Key_Num =0;

extern uint32_t Once_Act_Run;  //一次密码的使用记录，当超过24小时未使用一次密码时，清空一次密码
extern uint32_t Once_Act_Num;  //一次密码数量
uint32_t Once_Act_CL =0;  //一次密码定时清除

uint32_t Board_Unix =0xffffffff;  //板载Unix时间备份
uint64_t Board_Time;  //板载数字时间 用于写入
extern uint8_t PassWord_Error_Num;  //记录密码输入错误次数

extern uint8_t Update_Time_Flag;

uint8_t updat_Time_T =0;  //用于记录时间更新周期

extern uint16_t time1[6];

uint16_t Err_PassWord_CL;  //定时清空错误密码次数

/*
生成时间缓存 
time1[0] year
time1[1] mon
time1[2] day
time1[3] hour
time1[4] min
time1[5] sec

*/

uint16_t LOCK_OPEN_Time=0;  //记录斜舌连续触发时间
uint16_t Lock_OPEN_Spean =0;  //播放间隔


uint32_t  Auto_Card_Init =0;  //自动定时重置刷卡寻卡

static void  my_timer_handler (void *params)  //my_timer通用定时器回调
{
	
	if(PassWord_Error_Num>0)
	{
		Err_PassWord_CL++;
		if(Err_PassWord_CL>((5*60)/(My_test_timer_Time_Size/1000)))  //大于五分钟后清空
		{
			Err_PassWord_CL =0;
			PassWord_Error_Num =0;
		}
	}
	else
		{
			Err_PassWord_CL =0;
		}
		
		
	if (Once_Act_Num>0)  //当有一次密码时
	{
		
		Once_Act_CL++;
		if(Once_Act_Run==1)  //当计时期间使用过一次密码时重新开始计时，达到24未使用一次密码则清空本地一次密码的功能
			Once_Act_CL =0;
		
		if(Once_Act_CL>((25*3600)/(My_test_timer_Time_Size/1000))) //预设25小时内未使用一次密码，自动清空
		{
			//当超过25小时未使用一次密码，则清空一次密码记录
			Once_Act_CL =0;
			Spi_Flash_Init();  //开启SPI通讯
			nrf_delay_ms(50); //等待稳定
			
			Ev_Cat_Dat(0x02,0);  //将动态密码数量清除
			Once_Act_Num =0;
			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();	
		}
	}
	else 
	{
		Once_Act_CL =0;
	}
	
	if(MPR121_RUN==0)    //待机时自动重置刷卡芯片待机寻卡状态防止意外停止寻卡
	{
		Auto_Card_Init++;
		if(Auto_Card_Init>((6*3600)/(My_test_timer_Time_Size/1000)))   //当满足待机6小时后，自动重置寻卡
		{
			if(SPI0_Start==0) //当SPI0关闭时，将扫描阀值调整到待机后的值
				{
		
					Spi_Flash_Init();
			
					sky1311Reset();
					
					irqClearAll();              // 清除SKY1311的所有中断
					checkCardInit(LEDCL_RC_VALUE);    // 配置进入自动检卡状态
					cardStatus = GOTOSLEEP;

					Spi_Flash_Sleep();
										
					#ifdef uart_start
						
					SEGGER_RTT_printf(0,"Check_Card_Start_LEDCL_Value!\r\n");  //尝试开启亮灯时的寻卡阀值
						
					#endif
					Auto_Card_Init =0;
					
				}
		}
	}
	else
		Auto_Card_Init =0;
	
	if(MPR121_RUN)		//当设备进入高性能模式
	{
		MPR_Del =0;
	}
	else
	{
		MPR121_T =0;
		MPR_Del ++;
		if (MPR_Del>450)  //每2个半小时重新刷新一次键盘
		{
				
				MPR_Del =0;
//			mpr121_Run_I();
//			mpr121_init_I();  //低功耗定时重置键盘
//			mpr121_init();
				TSM12_Init();
			
		}
	}
	
	if(Init_Key_Num>2)  //初始化按钮次数清空
	{
		CL_Init_Key_Num++;
		if(CL_Init_Key_Num>1)
		{
			CL_Init_Key_Num =0;
			Init_Key_Num =0;
		}
	}
	#ifdef uart_start
//				MPR_Value = read_i2c(I2C_ADD,0X26);
//				printf("MPR121_8BASVALUE: %d\r\n",MPR_Value); //读取基线值	
//				MPR_Value =	read_i2c(I2C_ADD,0x15);
//				MPR_Value <<=8;
//				MPR_Value |=	read_i2c(I2C_ADD,0x14);
//				MPR_Value>>=2;   //只获取最高8字节
//				printf("MPR121_8VALUE: %d\r\n",MPR_Value); //读取键值
			//	MPR_Value =	read_i2c(I2C_ADD,0x27);
			//	printf("MPR121_9VALUE: %d\r\n",MPR_Value);
			//	MPR_Value =	read_i2c(I2C_ADD,0x28);
			//	printf("MPR121_10VALUE: %d\r\n",MPR_Value);
			//	MPR_Value =	read_i2c(I2C_ADD,0x29);
			//	printf("MPR121_11VALUE: %d\r\n",MPR_Value);
	
	#endif
	
	if(BLE_LINK==1)
	{
		Ble_Lin_T++;
		if(Ble_Lin_T>(60/(My_test_timer_Time_Size/1000)))
		{
			Ble_Lin_T =0;
			Dis_Conner();  //当连接时间超过1分钟，自动断开连接
		}
	}
	else
	{
		
		Ble_Lin_T =0;
	
	}
	
		if(Lock_Open_EN==0)   //当开启锁舌语音播放时
		{
			
			if(nrf_gpio_pin_read(7)==0) //当锁舌卡住（未关好时）
				{                                                                                   
					
				#ifdef uart_start
				
				SEGGER_RTT_printf(0,"LOCK_OPEN_EN!\r\n");
				
				#endif
					
					
					if(LOCK_OPEN_Time>(285/(My_test_timer_Time_Size/1000)))
					{
						if(Lock_OPEN_Spean<=(60/(My_test_timer_Time_Size/1000)))
								Lock_OPEN_Spean++;
						
						if(Lock_OPEN_Spean==(60/(My_test_timer_Time_Size/1000)))
						{
							input_speak(51);
					//		input_speak(29);
							
						//	Lock_OPEN_Spean =0;
						}
					}
					else if (LOCK_OPEN_Time==(285/(My_test_timer_Time_Size/1000)))
					{
						input_speak(51);
						LOCK_OPEN_Time++;
					}
					else
					{
						LOCK_OPEN_Time++;
					}
					

					
				}
				else
				{
					Lock_OPEN_Spean =0;
					LOCK_OPEN_Time =0;
				}
				

		}
	
	

	if(Read_Card_Error_Num>0)
	{
			if(Read_Card_Error_Num<Read_Card_Error_Num_Full)			// 当读卡次数在  Read_Card_Error_Num_Full 以内时，允许读取
			{
				
				Read_Card_Error_Num =0;
			
			}
			else						// 当读卡次数大于 Read_Card_Error_Num_Full    时，视为读卡异常状态，需要通过特定方式使系统恢复原状
			{
				
					if((SPI0_Start==0)&&(MPR121_RUN==0))		// 当SPI 空闲且未唤醒时
					{
						
						while(SKY1311_RC_VALUE()==0)nrf_delay_ms(1000);

						Spi_Flash_Init();
						sky1311Reset();         
						irqClearAll();              // 清除SKY1311的所有中断
						checkCardInit(LEDCL_RC_VALUE);    // 配置进入自动检卡状态
						cardStatus = GOTOSLEEP;
						
						SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
//	
						Spi_Flash_Sleep ();
						
						Read_Card_Error_Num =0;			//完成后清空
						
					}
				
			}
	}	
	
	
	
	
	
	//TSM12_read_tast();
	#ifdef uart_start
	
		SEGGER_RTT_printf(0,"heart!\r\n");
	
	#endif
//	if(Update_Time_Flag==1) //当联网更新过时间时
//	{
//		Update_Time_Flag =0;
//		updat_Time_T =0;  		//重置更新时间
//		Get_Time();			//校准板载备份时间
//		
//		if(TIME_STATE==true)
//		{
//		
//			Board_Unix = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);
//		
//		}
//		else
//		{
//		#ifdef uart_start
//		
//		SEGGER_RTT_printf(0,"Board_Unix_Cheak_Error-0!\r\n");
//		
//		#endif
//		}
//		#ifdef uart_start
//		SEGGER_RTT_printf(0,"CL_Update_Time_Flag!\r\n");
//		SEGGER_RTT_printf(0,"Board_Unix:%d\r\n",Board_Unix);
//		#endif
//		
//	}
//	else
//	if((TIME_STATE==true)&&
//		(updat_Time_T>=(300/(My_test_timer_Time_Size/1000)))
//		)  //当板载时间有效时  校对板载时间 (用于尽可能保障板载时钟在自然误差)
//	{
//		
//		Get_Time();
//		Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳			
//		#ifdef uart_start
//		
//		SEGGER_RTT_printf(0,"Unix_T:%d\r\n",Unix_T);
//		SEGGER_RTT_printf(0,"Board_Unix:%d\r\n",Board_Unix);
//		SEGGER_RTT_printf(0,"Unix_T-Board_Unix:%d\r\n",Unix_T-Board_Unix);
//		SEGGER_RTT_printf(0,"My_test_timer_Time_Size*updat_Time_T/1000-5:%d\r\n",My_test_timer_Time_Size*updat_Time_T/1000-5);
//		SEGGER_RTT_printf(0,"My_test_timer_Time_Size*updat_Time_T/1000+5:%d\r\n",My_test_timer_Time_Size*updat_Time_T/1000+5);

//		#endif
//		if(Unix_T>Board_Unix)
//		{
//			if(
//				((Unix_T-Board_Unix)>((My_test_timer_Time_Size*updat_Time_T)/1000)-5)&&
//				((Unix_T-Board_Unix)<((My_test_timer_Time_Size*updat_Time_T)/1000)+5)
//				)
//			{
//					#ifdef uart_start
//				
//						SEGGER_RTT_printf(0,"Board_Unix_Cheak_Ok!\r\n");
//				
//					#endif
//				
//					Board_Unix = Unix_T;
//			}
//			else
//			{
//				
//				#ifdef uart_start
//				
//				SEGGER_RTT_printf(0,"Board_Unix_Cheak_Error-1!\r\n");
//				
//				#endif
//				
//				Board_Unix+=(My_test_timer_Time_Size*updat_Time_T/1000);  //板载时钟加上软件定时器时间
//				Unix_Time_to_Time(Board_Unix);  //将板载时间转为直接显示数字
//			
//				Board_Time =0;
//				
//				Board_Time+= (time1[0]-2000)*10000000000;
//				Board_Time+= time1[1]*100000000;
//				Board_Time+= time1[2]*1000000;
//				Board_Time+= time1[3]*10000;
//				Board_Time+= time1[4]*100;
//				Board_Time+= time1[5];
//				
//				#ifdef uart_start
//				
//				for(uint8_t i=0;i<6;i++)
//				{
//					SEGGER_RTT_printf(0," %d ",time1[i]);
//				
//				}
//					SEGGER_RTT_printf(0,"\r\n");
//				
//					SEGGER_RTT_printf(0,"%d\r\n",Board_Time);
//				
//				#endif
//				
//				Update_Time(Board_Time);
//				Get_Time(); 
//				
//			}
//		}
//		else
//		{
//					#ifdef uart_start
//				
//					SEGGER_RTT_printf(0,"Board_Unix_Cheak_Error-2!\r\n");
//					
//					#endif
//					
//				Board_Unix+=(My_test_timer_Time_Size*updat_Time_T/1000);  //板载时钟加上软件定时器时间
//				Unix_Time_to_Time(Board_Unix);  //将板载时间转为直接显示数字
//			
//				Board_Time =0;
//				
//				Board_Time+= (time1[0]-2000)*10000000000;
//				Board_Time+= time1[1]*100000000;
//				Board_Time+= time1[2]*1000000;
//				Board_Time+= time1[3]*10000;
//				Board_Time+= time1[4]*100;
//				Board_Time+= time1[5];
//				
//				#ifdef uart_start
//				
//				for(uint8_t i=0;i<6;i++)
//				{
//					SEGGER_RTT_printf(0," %d ",time1[i]);
//				
//				}
//					SEGGER_RTT_printf(0,"\r\n");
//				
//					SEGGER_RTT_printf(0,"%d\r\n",Board_Time);
//				
//				#endif
//				
//				Update_Time(Board_Time);
//				Get_Time(); 
//		}
//		updat_Time_T =0;  //最后清空 计时周期
//	}
//	else
//	{
////			updat_Time_T =0;
//			#ifdef uart_start
//		
//			SEGGER_RTT_printf(0,"Board_Unix_Cheak_Error-3!\r\n");
//			
//			#endif
//		
//	}

}

//#endif  ADC_res << ADC_CONFIG_RES_Pos 

const uint8_t Key_Num_Tab_Len  =51;

uint8_t Key_Num_Tab[Key_Num_Tab_Len]; //按键输入缓存(最多Key_Num_Tab_Len个)
uint8_t Key_Num_n =0;  //按键输入键值个数
uint8_t PassWord_Error_Num =0;  //记录密码输入错误次数
uint32_t PassWord_Error_Time =0;  //错误锁定定时
#define PassWord_Error_Num_Max 6

void Pass_Word_Run(void); //声明密码处理函数


void read_a_key() //唤醒后第一次读取
{
	
	uint8_t i,key_num;
		
	//	MSR_121_read_key();
	
	if(!nrf_gpio_pin_read(MPR_IRQ))
	{
		
		key_num = MPR121_Key_Run();
		#ifdef uart_start
		SEGGER_RTT_printf(0,"  return  Key_Num: %d\r\n",key_num);
		#endif
		if(key_num<=12)
		{
			input_speak(29);
		nrf_gpio_pin_set(LEDB);
//		nrf_gpio_pin_clear(LEDB1);
		nrf_gpio_pin_clear(LEDR);
			
//				nrf_gpio_pin_set(LEDR);
		//input_speak(5);
		nrf_delay_ms(200);

		nrf_gpio_pin_clear(LEDR);
		nrf_gpio_pin_set(LEDB);
		//nrf_gpio_pin_set(LEDB1);
		
		}
	}
	
	switch (key_num)
	{
		case 10:  //当键入*号时 （用于清空已输入键值）
						//	mpr121_Run();
			Key_Num_n =0;
			for(i =0;i<Key_Num_Tab_Len;i++) //初始化即时输入密码缓存
			{
				Key_Num_Tab[i] = 0xFF;  //清空密码缓存
			}
			break;

		case 11: //当键入0号时 (普通0按键)
			
			Key_Num_Tab[Key_Num_n++] = 0;
		
		break;
		
		case 12: //当键入#号时 (输入完成，开始处理)
				//			mpr121_Run();
	//	Pass_Word_Run();  //唤醒后输入不对#做处理
			break;
		
		default:  //其他键值
			
			if(key_num<10) //键值合理时
			{
				Key_Num_Tab[Key_Num_n++] = key_num;
			}
			
		break;
	}
	
}


uint32_t Manual_Add_Card_T =0;  //记录自助发卡时间

static void  Read_Key (void *params)  //唤醒后开始扫描键盘  间隔等于    Read_Key_Time_Size
{
	uint8_t i,key_num;
	
		if(Start_Manual_Add_Card>0)  //处于自助发卡状态时 定时清空自助发卡标志位 以结束自助发卡状态
		{
			
				Manual_Add_Card_T++;
			
				if(Manual_Add_Card_T>(10*1000/Read_Key_Time_Size))
				{
					
					Start_Manual_Add_Card =0;
					Manual_Add_Card_T =0;
					if(BLE_LINK==1)  //当BLE还在连接时，返回自助发卡失败报文
					{
							
							input_speak(10);
							Manual_Add_Card_ACK(0x02,Type_AB_ID,0X00);  //返回操作失败的报文
							
					}
					
				}
		}
		else
		{
			
				Manual_Add_Card_T =0;
		
		}
		if(
			
				MPR121_RUN&&(PassWord_Error_Num<PassWord_Error_Num_Max)
		
		 )
		{
				
			//	MSR_121_read_key();


			
			app_timer_stop(Read_My_Key);  //先停止 定时器
			
			if(1//(!nrf_gpio_pin_read(MPR_IRQ))
				
					
				)
			{
				
//				nrf_delay_ms(5);
				
				
				#ifdef Key_By_MPR121
				
				key_num = MPR121_Key_Run();
				
				#endif
				
				#ifdef Key_By_TSM12c
				
				key_num = 0XFF;
				
				key_num = TSM12_read_key_Num();
				
				
				
				#endif
				
				#ifdef uart_start
				if(key_num!=0xFF)
				SEGGER_RTT_printf(0,"  return  Key_Num: %d\r\n",key_num);
				
				#endif
				
				if((key_num<=12)&&(key_num!=10))
				{
						input_speak(29);
//					nrf_gpio_pin_set(LEDB);
//					nrf_gpio_pin_clear(LEDB1);
//					nrf_gpio_pin_clear(LEDR);
//						
////				nrf_gpio_pin_set(LEDR);
//				//input_speak(5);
//					nrf_delay_ms(200); 
//					
//					nrf_gpio_pin_clear(LEDR);
//					nrf_gpio_pin_set(LEDB);
//					nrf_gpio_pin_set(LEDB1);
//					
				//	nrf_delay_ms(300);
					
				}
					
			}
			#ifdef CN_TSM12C
			
//			else if(nrf_gpio_pin_read(MPR_IRQ))
//			{
//				
//				CN_TSM12C_TH =0;				//清空触发标志
//				
//			}
			#endif
			

			
			switch (key_num)
			{
				case 10:  //当键入*号时 （用于清空已输入键值） 
								//	mpr121_Run();
//				if(Key_Num_n>2)
//				{
					input_speak(49);  //反馈密码被清空的声音
					nrf_gpio_pin_set(LEDR);
					nrf_delay_ms(300);
					nrf_gpio_pin_clear(LEDR);				
//				}
//				else
//				input_speak(29);
//				
				Key_Num_n =0;
					for(i =0;i<Key_Num_Tab_Len;i++) //初始化即时输入密码缓存
					{
						Key_Num_Tab[i] = 0xFF;  //清空密码缓存
					}
					break;
				case 11: //当键入0号时 (普通0按键)
					
					Key_Num_Tab[Key_Num_n++] = 0;
					break;
				case 12: //当键入#号时 (输入完成，开始处理)
		//	mpr121_Run();
				
				Pass_Word_Run();
					break;
				default:  //其他键值
					if(key_num<10) //键值合理时
					{
						Key_Num_Tab[Key_Num_n++] = key_num;
					}
				break;
			}
			
			if(Key_Num_n>=Key_Num_Tab_Len)   //超过50个清空缓存
			{
					Key_Num_n =0;
					for(i =0;i<Key_Num_Tab_Len;i++) //初始化即时输入密码缓存
					{
						Key_Num_Tab[i] = 0xFF;  //清空密码缓存
					}
					input_speak(49);
			}
			
			
			MPR_Del =0;
			MPR121_T++;
			
		if(Controllable_Power_State!=0)			//开启指纹头后直接关闭键盘
			{
				MPR121_T  =Devic_Static_True_Mpr121_T_Size;
			}
			
				if(
					(((Devic_Static==true)&&(MPR121_T>=Devic_Static_True_Mpr121_T_Size)) || 
						((Devic_Static==false)&&(MPR121_T>=Devic_Static_False_Mpr121_T_Size)&&(BLE_LINK==false))		
					)&&
					(
						Start_Manual_Add_Card==0)//当一定时间内未触发键盘，且不在自助发卡状态关闭键盘灯
					)						
				{
				app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	 //关闭前先打开定时器
				MPR121_T =0;
				//PassWord_Error_Num =0; //正常关灯时清空密码输错次数
				#ifdef Key_By_MPR121
				mpr121_init();
				#endif
				
				#ifdef Key_By_TSM12c
				
				TSM12_Init();
				
				#endif
				
				for(i =0;i<Key_Num_Tab_Len;i++)
				{
					
					Key_Num_Tab[i] = 0xFF;  //清空密码缓存
				
				}

				Key_Num_n =0;
				app_timer_stop(Read_My_Key);
				nrf_gpio_pin_clear(LEDB);
//				nrf_gpio_pin_clear(LEDB1);
				nrf_gpio_pin_clear(LEDR);
				
				if(SPI0_Start==0) //当SPI0关闭时，将扫描阀值调整到待机后的值
					{
			
						Spi_Flash_Init();
				
						sky1311Reset();
						
						irqClearAll();              // 清除SKY1311的所有中断
						checkCardInit(LEDCL_RC_VALUE);    // 配置进入自动检卡状态
						cardStatus = GOTOSLEEP;

						Spi_Flash_Sleep();
						
						#ifdef uart_start
							
						SEGGER_RTT_printf(0,"Check_Card_Start_LEDCL_Value!\r\n");  //尝试开启亮灯时的寻卡阀值
							
						#endif
						
					}
				
				if(Devic_Static==false)
				{
				//	DEVICE_NAME[2] = 'I';
					//GAP一些参数的设置，设置设备名，设置PPCP(外围设备首选链接参数)。
				//(主机连上某个蓝牙设备后可以从Gameric Access Service 中看到设置的这些参数)
			//	gap_params_init();  //设置蓝牙名	  
		//		advertising_init();
					ref_BLE_Adv();  //重置BLE广播
			
				}
				Bsp_Set();   //在键盘运行结束时重置 BSP
			}
			else  //当不关闭键盘灯时，重新开启键盘定时器
			app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	

			
		}
		else
			
		if(PassWord_Error_Num>=PassWord_Error_Num_Max)  //密码达到上限时 开始解锁倒计时
		{
			
			app_timer_stop(Read_My_Key);  //先停止 定时器
				nrf_gpio_pin_clear(LEDB);
//				nrf_gpio_pin_clear(LEDB1);
				nrf_gpio_pin_clear(LEDR);
			
//				sky1311Reset();
//					
//				irqClearAll();              // 清除SKY1311的所有中断
//				
//				checkCardInit(LEDCL_RC_VALUE);    // 配置进入自动检卡状态
//				
			//	cardStatus = GOTOSLEEP;
					
			
				PassWord_Error_Time ++;
			//	mpr121_init();
			TSM12_Init();
			if(PassWord_Error_Time>=60000)  //时间到后解除锁定
			{
				TSM12_Exit_Sleep_Mode();
				PassWord_Error_Time =0;
				PassWord_Error_Num  =0;  //清空密码错误记录
				//input_speak(24);
				
				nrf_gpio_pin_clear(LEDR); //恢复亮灯状态
				nrf_gpio_pin_set(LEDB);
//				nrf_gpio_pin_set(LEDB1);
			}
			if(!nrf_gpio_pin_read(MPR_IRQ)) //当有按键触发时
			{
				
				#ifdef Key_By_MPR121
				key_num = MPR121_Key_Run(); //伪读取
				#endif
				
				#ifdef Key_By_TSM12c
				key_num = TSM12_read_key_Num();
				#endif
//				nrf_gpio_pin_set(LEDB1);
				nrf_delay_ms(100);
				
			}
			app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	

		}
		else  //键盘未触发则关闭定时器与灯(一般不会进入)
		{
			
				MPR121_T =0;
//			PassWord_Error_Num =0;

				mpr121_init();
				for(i =0;i<Key_Num_Tab_Len;i++)
				{
					Key_Num_Tab[i] = 0xFF;  //清空密码缓存
				}
				Key_Num_n =0;
				app_timer_stop(Read_My_Key);
				nrf_gpio_pin_clear(LEDB);
//				nrf_gpio_pin_clear(LEDB1);
				nrf_gpio_pin_clear(LEDR);
				
		}
}





uint16_t PassWord_Con(uint32_t PassWord_Num);  //寻找与输入值对应的密码 返回值为找到的密码号，0表示没有
//extern uint16_t Note_Num_1;
extern uint8_t  Act_PassWord_rese;   //当发现输入密码与本地密码有相似时，置1，之后将不再允许此密码进行动态密码运算

//#define  History_Time_Act_PassWord_Run  1				//开启历史动态密码屏蔽功能 （覆盖动态密码）

void Pass_Word_Run() //密码处理
{
	
//	uint8_t psd; //密码是否错误 1为错误，0为正确
	

	uint32_t Unix_Start;  //相同密码的起始时间
	uint32_t Unix_Stop;  //相同密码的结束时间
	
	uint32_t PassWord_Num;
	
//	uint32_t len;
//	uint32_t Sector,Page;
	
	uint32_t i,is;
	uint32_t it;
	
	i =0xFFFFFFFF;

	Spi_Flash_Init();  //开启SPI_FLASH 通讯
	
	Devic_static_read();  //(获取设备绑定状态)
	
	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();
	
	if(Devic_Static==false)//未绑定时，用默认密码开门
	{
		
		
		if(Key_Num_n>3)
		{
			
			if((Key_Num_Tab[0]==1)&&(Key_Num_Tab[1]==2)&&
				(Key_Num_Tab[2]==3)&&(Key_Num_Tab[3]==4)&&
				(Key_Num_Tab[4]==5)&&(Key_Num_Tab[5]==6)&&(Key_Num_n==6))
			{
				
				Lock_Open();
				Key_Num_n =0;
					
			}
			else
			{
				Key_Num_n=0;
				nrf_gpio_pin_clear(LEDB);
//				nrf_gpio_pin_clear(LEDB1);
				
				nrf_gpio_pin_set(LEDR);
				input_speak(5);
				nrf_delay_ms(800);
				nrf_gpio_pin_clear(LEDR);
				nrf_gpio_pin_set(LEDB);
//				nrf_gpio_pin_set(LEDB1);
				
			}
		}
		else
		{			input_speak(49);
			Key_Num_n =0;  //键入值小于3位不进行处理，清空内容
		}
		
	}
	else if(Devic_Static==true) //设备已绑定时，开始密码校对
	{
		
		Get_Time(); //获取板载时间
		
		if(Key_Num_n>3) //密码最小输入长度
		{
			
			if((TIME_STATE==true)&&(Key_Num_n>5)) //当时间有效时 且输入值能满足六位数
			{
			//	SEC,MIN,HOUR,DAY,MON,YEAR;
				
				Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳			
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"%d,\r\n",Unix_T);			
				#endif
				Spi_Flash_Init();  //开启SPI_FLASH 通讯
				
				Devic_static_read();			//判断密码前先更新绑定信息

				PassWord_Num = Get_Cat_Dat (0x01); //获取当前存储的密码数量
//				len = PassWord_Num * PassWord_Len; //获取密码存储区域总长度
				
//				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取扇区数据
//				SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //获取与密码数据长度等量的数据 放到本地缓存
//				

				
//				#ifdef uart_start
//				i =0;
//				SEGGER_RTT_printf(0,"PassWord:\r\n");	
//				for(i =0;i<len;i++)
//				{
//					it =tab_dat[i];
//					SEGGER_RTT_printf(0,"%d,",it);	
//				}
//				SEGGER_RTT_printf(0,"\r\n");	
//				#endif
				
				it =0;		//it 用于表示本地密码是否有效
				it = PassWord_Con(PassWord_Num);  //寻找与输入值对应的密码 返回值为找到的密码号，0表示没有 
				
				if((it==0)&&(Act_PassWord_Verify(Key_Num_n,Unix_T)==0)&&(Per_Act_PassWord_Verify(Key_Num_n)==0)&&
					(More_Act_PassWord_Verify(Key_Num_n,Unix_T)==0)
				)  //没有相同本地密码,且密码与本地有效动态密码不符时
				{
					
					#ifdef uart_start  //当没有相同密码时，开始尝试判断输入的密码是否为动态密码
					
					SEGGER_RTT_printf(0,"Start_Act_PassWord!\r\n"); 
					
					#endif
					
					Get_Time(); //获取板载时间
					Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳			
					
					Unix_T /=3600;  //将时间戳转换成小时形式
					
					Spi_Flash_Init();  //开启SPI_FLASH 通讯 准备开始动态密码的处理
					
					i =0;
					
					if((Key_Num_n>=6)&&
							(Key_Num_n<=9)&&(Act_PassWord_rese!=1))  //当输入长度大于等于动态密码最小长度小于等于动态密码最大长度 且不与本地密码相似
						
					i = des_psw(Key_Num_Tab,Key_Num_n,Unix_T); //计算当前密码是否可能是合法的动态密码
					
					else if(Act_PassWord_rese==1)
					{
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Error!\r\n Act_PassWord_Rase!\r\n");    //提示密码相似
						
						#endif
						
						i =0;
						
					}
					
					Act_PassWord_rese =0; //完成后清空
						
					#ifdef uart_start
					
						SEGGER_RTT_printf(0,"Des_Psw_T: %x\r\n",i);
					
					#endif
						
						if(i>0) //当密码有效时 //将i 内的动态密码生效时间提取出来用于接下来的运算
						{
							
								is =i;
								is>>=24;
								is&=0xff; //如果动态密码有效，则提取生效时间
								i&=0x00ffffff;  //将 i 变为真正的有效时长
//							if(i>is)
//							i-= is;  //将i 换算成当前时间的多少小时以后到期
//							else
//							i =0;
							
							#ifdef uart_start
							
								SEGGER_RTT_printf(0,"is: %x!\r\n",is);
							
							#endif
							
							
						}
						
						nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口			//判断是否反锁
						nrf_delay_ms(1); //等待稳定
			
						if((nrf_gpio_pin_read(5)==0)&&(i>0))   //当门反锁时提示门已反锁  反锁时不允许动态密码开门
						{
							
							input_speak(17); //语音提示已反锁
							nrf_delay_ms(1200);
							
						}
				
						if((i>0)&&(nrf_gpio_pin_read(5)==1))  //当i>0时，说明密码未过期，开始判断密码属性
						{
							
							if((i==6)&&(Key_Num_n==8))  //输入当长度为8,有效时间为6时，密码为一次密码(开门后需要写入本地不可二次使用)
							{   
								
								if(Once_Act_PassWord_Verify()==1)			//载入开门记录详情 在该函数中
								{								
									ADD_Op_Door_Note(0,0x00,Unix_T*3600);  //写入动态密码开门记录  (密码号为0既视为动态密码)
									Lock_Open();  //一次密码一天内有效，故不减除
									PassWord_Error_Num =0;  //开门过后 重置错误密码次数
									
								}
								else
								{
									i =0;
								}
							}
							else if((i==8)&&(Key_Num_n==8)) //当输入长度为8，有效时间为8时，密码为永久密码(开门后保存不会过期，直至下一个有效动态密码替换)
							{
//								if(History_Time_Act_PassWord()==1)
//								{
										//将永久 密码内容 载入开门记录详情
										for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff 长度
											Op_Door_Buff[i1] =0;									
								
											Op_Door_Buff[5] = 8;		//本地密码长度都是6
			
											Op_Door_Buff[6] = Key_Num_Tab[0];		//本地密码第一位
											Op_Door_Buff[7] = Key_Num_Tab[1];	//本地密码第二位
											Op_Door_Buff[8] = Key_Num_Tab[2];	//本地密码第三位
											Op_Door_Buff[9] = Key_Num_Tab[3];		//本地密码第四位
											Op_Door_Buff[10] = Key_Num_Tab[4];		//本地密码第五位
											Op_Door_Buff[11] = Key_Num_Tab[5];	//本地密码第六位
											Op_Door_Buff[12] = Key_Num_Tab[6];	//本地密码第七位
											Op_Door_Buff[13] = Key_Num_Tab[7];	//本地密码第八位
													
								
								
									Per_Act_PassWord_Save(0x02,Key_Num_Tab,Key_Num_n,0xFFFFFFFF);  //载入数据 保存永久密码
									ADD_Op_Door_Note(0,0x00,Unix_T*3600);  //写入动态密码开门记录  (密码号为0既视为动态密码)
									Lock_Open();  //永久密码不会过期，故不减除
									PassWord_Error_Num =0;  //开门过后 重置错误密码次数
									
//								}
//								
//								else
//								{
//									is =0;
//									i =0;
//								}
							}
							else if(i>is)  	//当密码不是永久密码和一次密码时，密码为普通时效动态密码，新密码进入可替换旧密码
							{
								
								if(
										(is<i)
								#ifdef History_Time_Act_PassWord_Run
								&&(History_Time_Act_PassWord()==1)  //屏蔽 动态密码替换失效功能
								#endif
								
									)  //通过生成时间判断密码是否过期
								{
									//将密码写入Flash
								
								#ifdef History_Time_Act_PassWord_Run
									
								Del_User_Key();  //写入前删除用户钥匙
								
								#endif
									//写入 开门记录详情
									//将时效动态密码内容 载入开门记录详情
								for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff 长度
									Op_Door_Buff[i1] =0;									
									
									Op_Door_Buff[5] = Key_Num_n;		//密码长度 =  Key_Num_n
									
									for(uint8_t i1=0;i1<Key_Num_n;i1++)		//将时效动态密码数据载入
									{
										 Op_Door_Buff[6+i1] = Key_Num_Tab[i1];
									}
										
									Act_PassWord_Save(0x02,Key_Num_Tab,Key_Num_n,(Unix_T-is+i)*3600);  //载入数据(类型：普通密码，来源：键盘输入,长度：键盘输入总数(6-9),过期时间：当前时间减去几小时前生成加上有效时间) 
									ADD_Op_Door_Note(0,0x00,Unix_T*3600);  //写入动态密码开门记录  (密码号为0既视为动态密码)
									//新的动态密码开门后清空本地用户数据
									Lock_Open();  //动态密码合法且未过期时，允许开门
									PassWord_Error_Num =0;  //开门过后 重置错误密码次数
									
								}
								else
								{
									
									is =0;  
									i =0;
								
								}
								
							}
							else
							{
								
								is =0;
								i =0;
								
							}
							
						}
						
						nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//复位反锁 IO
						
						if (i==0)
						{
						
							#ifdef uart_start
							SEGGER_RTT_printf(0,"PassWord_Error:%d\r\n",it); //密码错误
							#endif
							
							nrf_gpio_pin_clear(LEDB);
//							nrf_gpio_pin_clear(LEDB1);
							
							nrf_gpio_pin_set(LEDR);
							
							PassWord_Error_Num++;
							if(PassWord_Error_Num==(PassWord_Error_Num_Max-2))
							{
								
								input_speak(28); //当输入次数达到一定值，提示再次输错将锁定门锁
								
							}
							else
								input_speak(5); //密码无效，提示密码错误
							
							//input_speak(5);
							
							nrf_delay_ms(800);
							nrf_gpio_pin_clear(LEDR);
							nrf_gpio_pin_set(LEDB);
//							nrf_gpio_pin_set(LEDB1);
								
							
					}
					else if(i ==0xFFFFFFFF)
					{
						input_speak(49);
					}
					SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
					Spi_Flash_Sleep();	

				}
				else
				{
					
						if(it!=0)  //当密码是本地密码时
						{
						#ifdef uart_start
						SEGGER_RTT_printf(0,"PassWord_Con:%d\r\n",it);	//密码正确，开始比对时间				
						#endif
						it-=1;
						if(it>=A_Sector_PassWord_Num)
						{
							it-=A_Sector_PassWord_Num;  //此时缓存区数据为第二扇区
						}
						Unix_Start =0;
						Unix_Start |= tab_dat[it*PassWord_Len+3];
						Unix_Start<<=8;
						Unix_Start |= tab_dat[it*PassWord_Len+4];
						Unix_Start<<=8; 
						Unix_Start |= tab_dat[it*PassWord_Len+5];
						Unix_Start<<=8;
						Unix_Start |= tab_dat[it*PassWord_Len+6];
						
						Unix_Stop =0;
						Unix_Stop |= tab_dat[it*PassWord_Len+7];
						Unix_Stop<<=8;
						Unix_Stop |= tab_dat[it*PassWord_Len+8];
						Unix_Stop<<=8;
						Unix_Stop |= tab_dat[it*PassWord_Len+9];
						Unix_Stop<<=8;
						Unix_Stop |= tab_dat[it*PassWord_Len+10];
						#ifdef uart_start 
						SEGGER_RTT_printf(0,"Unix_Start:%d\r\n",Unix_Start);	
						SEGGER_RTT_printf(0,"Unix_Stop:%d\r\n",Unix_Stop);						
						#endif
						
						
						


						
						if((Unix_T>Unix_Start)&&(Unix_T<Unix_Stop))
						{
							
							if(tab_dat[it*PassWord_Len+2]==User_PassWord)				//当密码是用户密码时判断是否为反锁
							{
									nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//先开启反锁接口			//判断是否反锁
									nrf_delay_ms(1); //等待稳定
										
								if((nrf_gpio_pin_read(5)==0))   //当门反锁时提示门已反锁
								{
									
									
									input_speak(17); //语音提示已反锁
									nrf_delay_ms(1200);
									
								}
								else			//未反锁允许开门
								{
																//写入一个开门记录
									//	Spi_Flash_Init();
			//							NRF_WDT_Feed();  //喂狗
										
										Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳			
										ADD_Op_Door_Note(tab_dat[it*PassWord_Len+17],0x0200,Unix_T);   //用户密码 高位为02
										Lock_Open();
				//						read_Soctor(500);
				//						NRF_WDT_Feed();
										PassWord_Error_Num =0;
								}
								nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
							}
							else if(tab_dat[it*PassWord_Len+2]==Admin_PassWord)
							{
							//写入一个开门记录
						//	Spi_Flash_Init();
//							NRF_WDT_Feed();  //喂狗
							Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳			
							ADD_Op_Door_Note(tab_dat[it*PassWord_Len+17],0x0100,Unix_T);			//管理员密码 高位为01
							Lock_Open();
	//						read_Soctor(500);
	//						NRF_WDT_Feed();
							PassWord_Error_Num =0;
							}
						}
						else
						{
							PassWord_Error_Num++;
							if(PassWord_Error_Num==(PassWord_Error_Num_Max-2))
							{
								input_speak(28);
							}
							else
								input_speak(15); //时间无效，提示密码已过期
							
							nrf_gpio_pin_clear(LEDB);
//							nrf_gpio_pin_clear(LEDB1);
							nrf_gpio_pin_set(LEDR);
							nrf_delay_ms(800);
							nrf_gpio_pin_clear(LEDR);
							nrf_gpio_pin_set(LEDB);
//							nrf_gpio_pin_set(LEDB1);
						}
					}
					else if(it==0) //当密码不是本地密码时,选择动态密码开门
					{
//							NRF_WDT_Feed();  //喂狗
							Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳			
							ADD_Op_Door_Note(0,0x00,Unix_T);
							Lock_Open();
							PassWord_Error_Num =0;  //开门过后 重置错误密码次数
					}
						
					
					SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
					Spi_Flash_Sleep();		

				}
				
//				SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
//				Spi_Flash_Sleep();
			}
			else if(TIME_STATE==false)
			{
					input_speak(15); //时间无效，提示密码已过期
					nrf_gpio_pin_clear(LEDB);
//					nrf_gpio_pin_clear(LEDB1);
					nrf_gpio_pin_set(LEDR);
					nrf_delay_ms(800);
					nrf_gpio_pin_clear(LEDR);
					nrf_gpio_pin_set(LEDB);
//					nrf_gpio_pin_set(LEDB1);
					
					nrf_delay_ms(500);
					input_speak(10);  //额外提示操作失败表示内部时间错误
			}
			else
			{
					#ifdef uart_start
					SEGGER_RTT_printf(0,"PassWord_Len_Short!\r\n"); //密码错误
					#endif
					nrf_gpio_pin_clear(LEDB);
//					nrf_gpio_pin_clear(LEDB1);
				
					nrf_gpio_pin_set(LEDR);
					PassWord_Error_Num++;
					if(PassWord_Error_Num==(PassWord_Error_Num_Max-2))
					{
						input_speak(28);
					}
					else
						input_speak(5); //时间无效，提示密码错误
					nrf_delay_ms(800);
					nrf_gpio_pin_clear(LEDR);
					nrf_gpio_pin_set(LEDB);
//					nrf_gpio_pin_set(LEDB1);
				
			}
			Key_Num_n =0;// 处理完后清空
//			//密码错误时的处理 
//			nrf_gpio_pin_clear(LEDB);
//			nrf_gpio_pin_set(LEDR);
//			input_speak(5);
//			nrf_delay_ms(800);
//			nrf_gpio_pin_clear(LEDR);
//			nrf_gpio_pin_set(LEDB);
		}
		else
		{
			input_speak(49);  //当密码输入数量小于起始值时按# 提示密码清空的提示音
			Key_Num_n =0; //键入值小于三位不进行处理，清空内容
		
		}
	}
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"i: %x!\r\n",i);
	SEGGER_RTT_printf(0,"PassWord_Run_End!\r\n");
	
	#endif
	
	
	
//		SpiFlash_PowerDown();   //完成所有操作时，使其进入低功耗模式 1ua
//		Spi_Flash_Sleep();
}



/**********************************************
寻找与输入值对应的密码 返回值为找到的密码号，0表示没有

返回： 与键入值密码相同的密码号
********************************************/
uint8_t PassWord_Con_User =0;				//寻找密码结果为用户密码标志

//#define Admin_PassWord  0x12		//定义管理员密码前缀
//#define User_PassWord  0x22		//定义用户密码前缀

extern u8 Op_Door_Buff[];				//用于缓存开门记录数据 待写入缓存


uint16_t PassWord_Con(uint32_t PassWord_Num)  //寻找与输入值对应的密码 返回值为找到的密码号，0表示没有
{
	
	uint32_t i,i1;
	 
	//uint32_t PassWord_Num;
	uint32_t len;
	uint32_t Block,Page;
//	uint32_t it;
	
	PassWord_Con_User =0;			//清空用户密码标志
	
	len = PassWord_Num*PassWord_Len;
	if(PassWord_Num<=A_Sector_PassWord_Num) //密码数量在一个扇区容量内
	{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取扇区数据
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //获取与密码数据长度等量的数据 放到本地缓存		
		
		#ifdef uart_start 
		SEGGER_RTT_printf(0,"Start_Con_1!\r\n");	
		#endif
		
		for(i1 =0;i1<(Key_Num_n-5);i1++)
		{
			for(i =0;i<PassWord_Num;i++) //轮询密码
			{
				if(
					(tab_dat[i*PassWord_Len+11]==Key_Num_Tab[i1])&&
					(tab_dat[i*PassWord_Len+12]==Key_Num_Tab[i1+1])&&
					(tab_dat[i*PassWord_Len+13]==Key_Num_Tab[i1+2])&&
					(tab_dat[i*PassWord_Len+14]==Key_Num_Tab[i1+3])&&
					(tab_dat[i*PassWord_Len+15]==Key_Num_Tab[i1+4])&&
					(tab_dat[i*PassWord_Len+16]==Key_Num_Tab[i1+5])
					)
				{
				
				for(uint32_t i2 =0;i2<Open_Note_Num_Len;i2++)   //Open_Note_Num_Len = Op_Door_Buff 长度
					Op_Door_Buff[i2] =0;
					
					Op_Door_Buff[5] = 6;		//本地密码长度都是6
					
					Op_Door_Buff[6] = tab_dat[i*PassWord_Len+11];		//本地密码第一位
					Op_Door_Buff[7] = tab_dat[i*PassWord_Len+12];		//本地密码第二位
					Op_Door_Buff[8] = tab_dat[i*PassWord_Len+13];		//本地密码第三位
					Op_Door_Buff[9] = tab_dat[i*PassWord_Len+14];		//本地密码第四位
					Op_Door_Buff[10] = tab_dat[i*PassWord_Len+15];		//本地密码第五位
					Op_Door_Buff[11] = tab_dat[i*PassWord_Len+16];		//本地密码第六位
					
					return i+1; //返回一样的密码号
				}
			}
		}
	}
	else if(PassWord_Num<=(2*A_Sector_PassWord_Num))
	{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //获取扇区数据
		SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //获取与密码数据长度等量的数据 放到本地缓存		
		
		#ifdef uart_start 
		SEGGER_RTT_printf(0,"Start_Con_1!\r\n");	
		#endif
		
		for(i1 =0;i1<(Key_Num_n-5);i1++)
		{
			for(i =0;i<A_Sector_PassWord_Num;i++) //轮询密码
			{
				if(
					(tab_dat[i*PassWord_Len+11]==Key_Num_Tab[i1])&&
					(tab_dat[i*PassWord_Len+12]==Key_Num_Tab[i1+1])&&
					(tab_dat[i*PassWord_Len+13]==Key_Num_Tab[i1+2])&&
					(tab_dat[i*PassWord_Len+14]==Key_Num_Tab[i1+3])&&
					(tab_dat[i*PassWord_Len+15]==Key_Num_Tab[i1+4])&&
					(tab_dat[i*PassWord_Len+16]==Key_Num_Tab[i1+5])
					)
				//将密码写入 开门记录 详情
				{
					
				for(uint32_t i2 =0;i2<Open_Note_Num_Len;i2++)   //Open_Note_Num_Len = Op_Door_Buff 长度
					Op_Door_Buff[i2] =0;
					
					Op_Door_Buff[5] = 6;		//本地密码长度都是6
					
					Op_Door_Buff[6] = tab_dat[i*PassWord_Len+11];		//本地密码第一位
					Op_Door_Buff[7] = tab_dat[i*PassWord_Len+12];		//本地密码第二位
					Op_Door_Buff[8] = tab_dat[i*PassWord_Len+13];		//本地密码第三位
					Op_Door_Buff[9] = tab_dat[i*PassWord_Len+14];		//本地密码第四位
					Op_Door_Buff[10] = tab_dat[i*PassWord_Len+15];		//本地密码第五位
					Op_Door_Buff[11] = tab_dat[i*PassWord_Len+16];		//本地密码第六位
					
					return i+1; //返回一样的密码号
					
				}
			}
			  //第二扇区
		} //对比一样的扇区

		#ifdef uart_start		
		SEGGER_RTT_printf(0,"Start_Con_2!\r\n");
		#endif
		
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //获取扇区数据
		SPIFlash_Read_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len);  //获取与密码数据长度等量的数据 放到本地缓存		
		
		for(i1 =0;i1<(Key_Num_n-5);i1++)
		{
			for(i =0;i<=(PassWord_Num-A_Sector_PassWord_Num);i++) //获取扇区数据
			{
				if(
				(tab_dat[i*PassWord_Len+11]==Key_Num_Tab[i1])&&
				(tab_dat[i*PassWord_Len+12]==Key_Num_Tab[i1+1])&&
				(tab_dat[i*PassWord_Len+13]==Key_Num_Tab[i1+2])&&
				(tab_dat[i*PassWord_Len+14]==Key_Num_Tab[i1+3])&&
				(tab_dat[i*PassWord_Len+15]==Key_Num_Tab[i1+4])&&
				(tab_dat[i*PassWord_Len+16]==Key_Num_Tab[i1+5])
				)
				{
				for(uint32_t i2 =0;i2<Open_Note_Num_Len;i2++)   //Open_Note_Num_Len = Op_Door_Buff 长度
					Op_Door_Buff[i2] =0;
					
					Op_Door_Buff[5] = 6;		//本地密码长度都是6
					
					Op_Door_Buff[6] = tab_dat[i*PassWord_Len+11];		//本地密码第一位
					Op_Door_Buff[7] = tab_dat[i*PassWord_Len+12];		//本地密码第二位
					Op_Door_Buff[8] = tab_dat[i*PassWord_Len+13];		//本地密码第三位
					Op_Door_Buff[9] = tab_dat[i*PassWord_Len+14];		//本地密码第四位
					Op_Door_Buff[10] = tab_dat[i*PassWord_Len+15];		//本地密码第五位
					Op_Door_Buff[11] = tab_dat[i*PassWord_Len+16];		//本地密码第六位
					
					i+=A_Sector_PassWord_Num;
					return i+1; //返回一样的密码号
				}
			}
		}
	}
	return 0;
}

void ref_BLE_Adv()  //刷新BLE广播名称
{
		uint8_t i,it1,it2;
//	   uint32_t err_code;
		if(Devic_Static==false)  //当设备未绑定时的蓝牙广播名
		{
			DEVICE_NAME[0] = Devic_Name_H1;
			DEVICE_NAME[1] = Devic_Name_H2;
			
			if(MPR121_RUN)
				DEVICE_NAME[2]=Devic_Name_E;
			else
				DEVICE_NAME[2]='I';			

			DEVICE_NAME[3]='_';
						
			for(i =4;i<sizeof(DEVICE_NAME);i++)
				DEVICE_NAME[i] =0;
			
			for(i =0;i<6;i++)
			{
//				DEVICE_NAME[i*2+4] = i+0x30;
//				DEVICE_NAME[i*2+5] = i+0x30;
//				
				it1 = MAC_ADD[5-i];
				
				it2 = it1&0xF0;
				
				it2>>=4;
				
				if (it2<0x0A)
				{
					it2+=0x30;
					DEVICE_NAME[i*2+4]=it2;
				}
				else
				{
				
					it2+=0x41;
					it2-=10;
					DEVICE_NAME[i*2+4] =it2;
				
				}				
				it2 = it1&0x0f;
				if(it2<0x0A)
				{
					
					it2+=0x30;
					DEVICE_NAME[i*2+5] = it2;
				
				}
				else
				{
				
					it2+=0x41;
					it2-=10;
					DEVICE_NAME[i*2+5] = it2;

				}
			}  //将MAC地址载入广播名
	//		sd_ble_gap_adv_stop(); // 解绑后同时关闭广播
			gap_params_init();  //设置蓝牙名	  
	//		advertising_init();
			AC_advertising_init();
		}
		else
		{
			for(i =0;i<sizeof(DEVICE_NAME);i++)
			DEVICE_NAME[i] =0;
			for(i =0;i<24;i++)
			{
				DEVICE_NAME[i] = Devic_Num[i+4];
			}
//				sd_ble_gap_adv_stop(); // 解绑后同时关闭广播
				gap_params_init();  //设置蓝牙名	  
				advertising_init();	
//				err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
//				APP_ERROR_CHECK(err_code);
		}
}


//APP_TIMER_DEF (my_Op_Door_timer);  //声明新的软件定时器名称  注册软件定时器

//uint8_t opdt =0;
//uint32_t TX_Note_T =0;  //开门记录返回

//static void  my_Op_Door_Out(void *params)
//{
//	
//	uint32_t err_code;
//		
////		err_code =app_timer_stop(my_Op_Door_timer);
////		APP_ERROR_CHECK(err_code);
//	if((Note_Num_1>0)&&(Note_Num_1<=Open_Note_Size))  //TX_SUC  z表示发送完成
//		{

//					TX_Door_note(); //读取开门记录指令 一个个·返回
//			
////nrf_delay_ms(10);
//				
//				
////				#ifdef uart_start
////				SEGGER_RTT_printf(0,"TX_Note_T: %d\r\n",TX_Note_T);
////				#endif
////				
////			else
////			{
////						Note_Num_1 =0;
////						app_timer_stop(my_Op_Door_timer);
////						opdt =0;
////			}
//		}
//		else if(TX_SUC==1)
//		{
//			#ifdef uart_start
//			
//			SEGGER_RTT_printf(0,"\r\nTX_SUC==1!\r\n");
//			
//			#endif
//		}
//		
//		if((Note_Num_1>0)&&(Note_Num_1<=Open_Note_Size))  //当需要继续上传开门记录时，继续开启开门记录上传定时器
//		{
////				err_code = app_timer_start(my_Op_Door_timer,APP_TIMER_TICKS(my_Op_Door_timer_Size,APP_TIMER_PRESCALER),NULL);			//注册新的软件定时器任务
////					APP_ERROR_CHECK(err_code);
//		}
//		else  //否则停止上传
//		{
//					Note_Num_1 =0;
//			
//					
//					opdt =0;
//			
////				if(MPR121_RUN==1)//当键盘唤醒时继续键盘扫描定时器运行
////				{
////							err_code =app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	  //继续键盘定时器运行
////							APP_ERROR_CHECK(err_code);
////				}
//			
//			err_code =app_timer_stop(my_Op_Door_timer);
//			APP_ERROR_CHECK(err_code);
//			
//		}
//	
//		//NRF_WDT_Feed();  //这里不用喂狗
//		#ifdef uart_start  //根据需要开启串口监听	
//		SEGGER_RTT_printf(0,"My_Op_Door_Out!\r\n");
//		#endif
//		
//}




uint8_t sky1311_In_Read()
{

	if (SKY1311_IRQ_READ()==0)
		return 1;
	
	return 0;

}



void sky1311_Init()
{
	Spi_Flash_Init();
	
	system_Init();
	sky1311Enable();
	

	#ifdef uart_start
	uint8_t dat = sky1311ReadReg(0x04);
	SEGGER_RTT_printf(0,"dat:%x\r\n",dat);	
	#endif
	
//	rcValue = sky1311RCFreqCali();
//	irqClearAll();              // 清除SKY1311的所有中断
////		checkCardInit(rcValue);     // 配置进入自动检卡状态
////		cardStatus = GOTOSLEEP;
//	
//	#ifdef uart_start
//	SEGGER_RTT_printf(0,"\r\n rcValue=%x\r\n",rcValue);
//	#endif
//		irqClearAll();              // 清除SKY1311的所有中断
//		checkCardInit(rcValue);     // 配置进入自动检卡状态
//		cardStatus = GOTOSLEEP;

	Spi_Flash_Sleep();
}

void sky1311_Read()
{
		if(cardStatus == CARDREMOVED)           // 没卡在场内（已经移走）
			{
					irqClearAll();              // 清除SKY1311的所有中断
					checkCardInit(rcValue);     // 配置进入自动检卡状态
					cardStatus = GOTOSLEEP;
			//	system_ActiveHalt();        // MCU 进入休眠状态
				
//					while(sky1311_In_Read());
//					hasCardIn =1;
			}
		 else if(cardStatus == GOTOSLEEP)
			{
				
				
					/* 以下是MCU 休眠后唤醒 */
					if(hasCardIn)           // 检卡中断产生，设置了标记
					{
							 irq_sta = sky1311ReadReg(ADDR_IRQ_STA);  // 读中断寄存器
							if((0==irq_sta) &&                      // 没有任何中断产生，且
								 (0!=SKY1311_IRQ_READ())) // IRQ 引脚变高电平
							{
									checkCardDeinit();          // 停止自动检卡
									cardStatus = HASCARDIN;     // 设置标记，有卡靠近
							}
							else
							{
									irqClearAll();
									cardStatus = CARDREMOVED;
							}
						}
					 else
						{
								cardStatus = CARDREMOVED;
						}
				
			}
		else if(cardStatus == HASCARDIN)  //有卡在场内
		{
				 if((Ok == Type_A_Rrun())||(Ok == Type_B_Rrun()))          // 尝试读卡，判断是否Type A 卡，
					{
							cardStatus = WAITREMOVE; // 标记读卡完成
					}
					else        // 没有读到卡
					{
							rcValue = sky1311RCFreqCali(); // 重新校准RC频率
							cardStatus = CARDREMOVED;
					}
		}
	else if(cardStatus == WAITREMOVE)       // 等待卡离开
		{
				DelayMS(300);                   // 稍微延时一下，不用太频繁判断是否卡离开
				if(checkCardRemove(rcValue))    // 判断卡是否离开
				{
						cardStatus = CARDREMOVED;       // 标记卡已经离开
//					rcValue = sky1311RCFreqCali(); // 重新校准RC频率
						DelayMS(300);
						sky1311Reset();
				}
	//		cardStatus = CARDREMOVED;       // 标记卡已经离开
		}
//			NRF_WDT_Feed();
}

void sky1311_run()
{
	Spi_Flash_Init();
	system_Init();
	sky1311Enable();


		#ifdef uart_start
	
	uint8_t dat = sky1311ReadReg(0x04);
	
		SEGGER_RTT_printf(0,"dat:%x\r\n",dat);
		
		#endif	

//	   while(1)
//    {
//        if(Ok != TypeA_test())              // read Type A card
//        {
//            if(Ok!= TypeB_test())      // read Type B card
//            {
//                LED_OFF();
////								#ifdef uart_start
////							SEGGER_RTT_printf(0,"Not_Card!\r\n");
////							#endif
//            }
//						else
//						{
//							#ifdef uart_start
//							SEGGER_RTT_printf(0,"Has_B!\r\n");
//							#endif
//							nrf_delay_ms(1500);
//						}
//        }
//				else
//					{
//						#ifdef uart_start
//						SEGGER_RTT_printf(0,"Has_A!\r\n");
//						#endif
//						nrf_delay_ms(1500);
//					}
//				
//        sky1311Reset();
//				NRF_WDT_Feed();
//	    	power_manage(); 
//        DelayMS(300);
//    }

		rcValue = sky1311RCFreqCali();
		irqClearAll();              // 清除SKY1311的所有中断
//		checkCardInit(rcValue);     // 配置进入自动检卡状态
//		cardStatus = GOTOSLEEP;
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"\r\n rcValue=%d\r\n",rcValue);
		
		#endif
		
		while(1)
		{
			
//			while(1)
//			{
//				checkCardRemove(rcValue);
//				nrf_delay_ms(500);
//				NRF_WDT_Feed();
//			}
			 if(cardStatus == CARDREMOVED)           // 没卡在场内（已经移走）
				{
						irqClearAll();              // 清除SKY1311的所有中断
						checkCardInit(rcValue);     // 配置进入自动检卡状态
						cardStatus = GOTOSLEEP;
					//	system_ActiveHalt();        // MCU 进入休眠状态
					
						while(sky1311_In_Read());
						hasCardIn =1;
				}
			 else if(cardStatus == GOTOSLEEP)
				{
						/* 以下是MCU 休眠后唤醒 */
						if(hasCardIn)           // 检卡中断产生，设置了标记
						{
								 irq_sta = sky1311ReadReg(ADDR_IRQ_STA);  // 读中断寄存器
								if((0==irq_sta) &&                      // 没有任何中断产生，且
									 (0!=SKY1311_IRQ_READ())) // IRQ 引脚变高电平
								{
										checkCardDeinit();          // 停止自动检卡
										cardStatus = HASCARDIN;     // 设置标记，有卡靠近
								}
								else
								{
										irqClearAll();
										cardStatus = CARDREMOVED;
								}
							}
						 else
							{
									cardStatus = CARDREMOVED;
							}
					
				}
			else if(cardStatus == HASCARDIN)  //有卡在场内
			{
					 if((Ok == Type_A_Rrun()) || (Ok == Type_B_Rrun()))          // 尝试读卡，判断是否Type A 卡，
						{
								cardStatus = WAITREMOVE; // 标记读卡完成
						}
						else        // 没有读到卡
						{
								rcValue = sky1311RCFreqCali(); // 重新校准RC频率
								cardStatus = CARDREMOVED;
						}
			}
		else if(cardStatus == WAITREMOVE)       // 等待卡离开
			{
					DelayMS(300);                   // 稍微延时一下，不用太频繁判断是否卡离开
					if(checkCardRemove(rcValue))    // 判断卡是否离开
					{
							cardStatus = CARDREMOVED;       // 标记卡已经离开
//										rcValue = sky1311RCFreqCali(); // 重新校准RC频率
							DelayMS(300);
							sky1311Reset();
						
					}
		//		cardStatus = CARDREMOVED;       // 标记卡已经离开
			}
//				NRF_WDT_Feed();
		}
}

void CL_SKY1311_IRQ()  //清除SKY1311中断
{
		SKY1311_ENABLE();
		irqClearAll();
		SKY1311_DISABLE();
}

/***********************************************************

Uart_Close 回调

//当定时时间达到了关闭串口的时间 则自动关闭串口

输入： 无
输出：无

可在串口开启时随时通过清空Uart_Close_T 来延续串口的开启时间

*********************************************************/

uint32_t Uart_Close_T =0;

void Uart_Close_Hander(void *params)	  //关闭串口中断回调
{
	
	#ifdef NB_Enable
	
	if(NB_Dat_Len>0)   		//判断是否有  NB 数据写入
	{
		
			Uart_Close_T =0;				//  NB 缓存有数据时清空关闭串口计时值
			
	}
	else								//仅在没有 NB 数据载入时才允许关闭串口
		Uart_Close_T ++;
	
	#else
									//未使能NB 模块时
	Uart_Close_T ++;
	
	if(Controllable_Power_State==0)	//当指纹头电源关闭时关闭串口(当前仅有指纹)
	{
		
		Uart_Close_T = Uart_Close_Time*1000;
		
	}
	
	#endif
	
	if(Uart_Close_T>(Uart_Close_Time*1000/Uart_Close_Time_Size))   
	{	
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Close_Uart!\r\n");
		
		#endif
		
		#ifdef NB_Enable    // NB 串口  在设备进入待机模式前，串口不进入休眠
		
		if((MPR121_RUN==1)||(SPI0_Start==1))    //当设备仍然处于唤醒状态或Flash接口在运行状态。
		{
			
				Uart_Close_T =0;   // 未待机时重新开始串口关闭计时
			
			#ifdef uart_start
			
			
			if(MPR121_RUN==1)
			{
				
				SEGGER_RTT_printf(0,"Close_Uart_Err!\r\n MPR121_RUN!\r\n");
				
			}
			else if(SPI0_Start==1)
			{
				
				SEGGER_RTT_printf(0,"Close_Uart_Err!\r\n SPI0_Start!\r\n");
				
			}
			
			#endif
				
		}
		
		else   //当设备·待机时，允许关闭串口，并开始处理 NB缓存 内的数据指令
		{
			
			NB_Run =0;   //在关闭串口时，将NB 标志位清零
		 	Uart_Close_T =0;
			close_uart();
			app_timer_stop(Uart_Close);   //结束接收后停止定时器
			
		}
		
		#else   //一般开启串口
		
		Uart_Close_T =0;
		F_Print_Run_Flag =0;
		Test_Ps_Mark =0;		//关闭串口时清空 测试标志
		close_uart();
		
		Controllable_Power_Off();			//关闭指纹电源
		
		app_timer_stop(Uart_Close);   //结束接收后停止定时器
		
		
		#endif

		
	}
	
}


/**@brief Application main function.//应用程序的主要功能。
 */

//范围内函数声明区

void F_Print_Input_Ps_Run(void);				//运行指纹录入程序(直接放在主函数里一定条件下自动唤醒调用)

void PS_Print_Run(void);				//指纹头运行状态

void SKY1311_Error_Run(void);			//SPI_1311 错误恢复机制

void CL_PS_Print_Run(void);				//运行删除指纹

void FD_Key_Run(void);			//防拆语音

uint32_t id1,id2;  //ID号缓存，直接作为设备号使用

extern uint8_t etp[10]; //定义生成密码的字符串

uint8_t TX_BUFF[64];

uint32_t Input_PS_Lmage_t =0;		//指纹录入运行间隔

uint32_t PS_Goto_Sleep_t =0;			//使指纹进入睡眠模式的运行间隔

uint8_t Type_Card_Id[16]={1,2,3,4,5,6,7,8};
uint32_t Note_Num_Delay;   // 开门记录返回 用的 延迟

extern uint16_t Buzz_Data_T;

int main(void)
{
	  
    uint32_t err_code;
		uint32_t i;
	
//		uint32_t it;
	
		NRF_POWER->DCDCEN =1; 
		
		Note_Num_Delay =0;
	
		Lock_OPEN_Spean =0;
		LOCK_OPEN_Time =0;

	
		#if defined (USB_SYSTEM_CLOCK_32MHZ)
		
		setClock();
		
		#endif
		auart_init();
		NRF_WDT_Init();
		nrf_delay_ms(100);
		ble_gap_addr_t addr;  //用于获取mac地址
	
		id1 = NRF_FICR->DEVICEID[0];
		id2 = NRF_FICR->DEVICEID[1];

		bool erase_bonds;  // 按键初始化
	
//	#ifdef uart_Enable
//	
//	uart_init();
//	
//	#endif
	
//		#ifdef uart_start  //根据需要开启串口监听		
//		uart_init();
//		#endif
		//printf("Uart_t!\r\n");
//		NRF_WDT_Feed();

		start_init();  //外设初始化
	

		
//		Controllable_Power_Off();
		
		NRF_WDT_Feed();
	
		Key_Num_n =0; //复位键入数初值
		for(i =0;i<Key_Num_Tab_Len;i++) //初始化即时输入密码缓存
		{
			
			Key_Num_Tab[i] = 0xFF;  //清空密码缓存
		
		}

//		read_Soctor(0);
//		read_Soctor(1);  //获取Flash 扇区数据
//		read_Soctor(2);
//		read_Soctor(1023);


//		uint8_t  start_string[] = START_STRING;  // start_string 开启设备后串口输出的数据包

//			nrf_delay_ms(100);
//			SPIFlash_Erase_Sector(FLASH_BLOCK_NUMBLE,0); // (块/扇) 擦除
//			nrf_delay_ms(100);
	#ifdef Debug
		w25q32_run();
	#endif

	// Initialize. 初始化
   /* 定时器初始化
	
	初始化软件定时器模块，该定时器模块并不是使用timer0-2来实现定时功能。而是使用51822中的RTC1 来软件模拟出定时器
	模块。RTC1使用32.768k时钟经过分频后来作为时钟源。所以该函数内部实现就是设置RTC1相关的寄存器和做一些初始化。
	其原理和timer定时/计数器模块类似。具体细节参考芯片数据手册。
*/
	
	APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
		
		//串口初始化，设置uart引脚，波特率，接收，发送中断等。并开启uart模块中断
  
		//按键，LED初始化 LED用于指示广播和连接状态，按键用于唤醒BLE设备
		buttons_leds_init(&erase_bonds);
		
	 //协议栈初始化，demo里设置为外部时钟。并且注册事件派发函数
	  ble_stack_init();
     
		//GAP一些参数的设置，设置设备名，设置PPCP(外围设备首选链接参数)。
	//(主机连上某个蓝牙设备后可以从Gameric Access Service 中看到设置的这些参数)
	  gap_params_init();  //设置蓝牙名
    
		//服务初始化，添加uart的串口服务。主要提供两个特征值来供手机和板子以及电脑的通信
	  services_init();
    
		//设置广播数据以及扫描响应数据。
	  advertising_init();
		
		//设置MAC地址
   	//mac_set();
		
		//链接参数设置。主要设置什么时候发起更新链接参数请求以及间隔和最大尝试次数。
		conn_params_init();
    
		#ifdef uart_start
		
    SEGGER_RTT_printf(0,"%s\r\n",START_STRING);
		SEGGER_RTT_printf(0,"start\r\n");
	 
	 #endif
	//设置广播类型，白名单，间隔，超时等特性，并开始广播
	
//		if(Devic_Static==true) /  /设备绑定时开启蓝牙
//		{
			err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
			APP_ERROR_CHECK(err_code);
//		}
//		else  //否则关闭蓝牙广播
//			sd_ble_gap_adv_stop();
//		    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
//    APP_ERROR_CHECK(err_code);
//		advertising_IDLE_Init();
////		 err_code = ble_advertising_stop(BLE_ADV_MODE_FAST);
////    APP_ERROR_CHECK(err_code);
		sd_ble_gap_tx_power_set(4);  		//设置蓝牙发送功率(accepted values are -40, -30, -20, -16, -12, -8, -4, 0, and 4 dBm)
	
//	    ble_gap_conn_sec_mode_t sec_mode; //定义结构体 
			
//		  err_code = sd_ble_gap_device_name_set(&sec_mode,  //设备安全模式 修改设备名
//                                         (const uint8_t *) DEVICE_NAME1,
//		 strlen(DEVICE_NAME1));
//     APP_ERROR_CHECK(err_code);
																			 
	err_code =
           app_timer_create (&my_test_timer, APP_TIMER_MODE_REPEATED ,my_timer_handler);
				APP_ERROR_CHECK(err_code);
	err_code = 
					app_timer_start (my_test_timer,APP_TIMER_TICKS(My_test_timer_Time_Size,APP_TIMER_PRESCALER),NULL);			//注册新的软件定时器任务
		    APP_ERROR_CHECK(err_code);
													
		err_code =
				app_timer_create(&Read_My_Key, APP_TIMER_MODE_REPEATED ,Read_Key); //注册键盘定时器任务
						APP_ERROR_CHECK(err_code);
//	err_code =
//				app_timer_create(&my_Op_Door_timer, APP_TIMER_MODE_REPEATED ,my_Op_Door_Out);  //注册返回开门记录的定时器任务
//			    APP_ERROR_CHECK(err_code);
													
	err_code =
				app_timer_create(&Timer_1, APP_TIMER_MODE_REPEATED ,Timer_1_Handler);  //注册返回开门记录的定时器任务
			    APP_ERROR_CHECK(err_code);
					
	err_code =
				app_timer_create(&Uart_Close, APP_TIMER_MODE_REPEATED ,Uart_Close_Hander);  //注册自动关闭串口的定时器任务
			    APP_ERROR_CHECK(err_code);
					
	
	
//	app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//开启500ms间隔的定时软件定时器任务
//	
	//	    APP_ERROR_CHECK(err_code);
	
	//	LOCK_OPEN_Init(); //开启锁体斜舌输入GPIO
																				 
//		for(i =1;i<16;i++)
//			input_speak(i);
																				 
		#ifdef uart_start																			 
		
		i = Time_to_Unix(2007,1,1,8,1,0);																			 
		SEGGER_RTT_printf(0,"%d,\r\n",i);
		
		Unix_Time_to_Time(1547710465);
		i = time1[0];
		SEGGER_RTT_printf(0,"%d,\r\n",i);
		i = time1[1];
		SEGGER_RTT_printf(0,"%d,\r\n",i);
		i = time1[2];
		SEGGER_RTT_printf(0,"%d,\r\n",i);
		i = time1[3];
		SEGGER_RTT_printf(0,"%d,\r\n",i);
		i = time1[4];
		SEGGER_RTT_printf(0,"%d\r\n,",i);
		i = time1[5];
		SEGGER_RTT_printf(0,"%d\r\n",i);
		
		SEGGER_RTT_printf(0,"IC_ID:\r\n");	
		SEGGER_RTT_printf(0,"%d\r\n",id1);
		SEGGER_RTT_printf(0,"%d\r\n",id2);
		
		#endif
		

		Spi_Flash_Init();  //开启SPI_FLASH 通讯

		Lock_Open_EN =Get_Cat_Dat(0x50);					//获取是否开启未关好门语音数据

//		
//			NRF_WDT_Feed();
//			read_Soctor(0);
//			
//			NRF_WDT_Feed();
//			read_Soctor(1);

//			NRF_WDT_Feed();
//			read_Soctor(AD_TypeA_Data_Secotr1);  //获取管理员扇区数据




//		  NRF_WDT_Feed();
//			read_Soctor(6);

//		NRF_WDT_Feed();
//		read_Soctor(2);
//		NRF_WDT_Feed();
//		read_Soctor(3);
//		NRF_WDT_Feed();
//		read_Soctor(4);
//		NRF_WDT_Feed();

//		read_Soctor(1023);
//		err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
// 	  APP_ERROR_CHECK(err_code);

//	#ifdef uart_start

//		SEGGER_RTT_printf(0,"\r\n");
//		
//		#endif

		err_code = sd_ble_gap_address_get(&addr);  //获取MAC地址
		APP_ERROR_CHECK(err_code);
		
		for(i =0;i<6;i++)
		MAC_ADD[i] = addr.addr[i];  //将MAC地址载入 MAC_ADD
		ref_BLE_Adv();

	//	DEVICE_NAME[2] = 'O';
	//GAP一些参数的设置，设置设备名，设置PPCP(外围设备首选链接参数)。
	//(主机连上某个蓝牙设备后可以从Gameric Access Service 中看到设置的这些参数)
	//	  gap_params_init();  //设置蓝牙名	  
	//		advertising_init();


//			BLE_TX[i] = TX_Buff[i+20]; //赋值第一个数据包

		PassWord_Error_Num =0;
//	input_speak(28);

//		Get_Time(); //获取板载时间
//		Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //将板载时间转换为时间戳			
//				Unix_T/=3600;

//		for (it =0;it<2000;it++)
//		{
//		#ifdef uart_start
//	//	SEGGER_RTT_printf(0,"%d\r\n",Unix_T);
//		
//		enc_time_psw(Unix_T+it,(Unix_T+3+it),3);
//		NRF_WDT_Feed();
//			
//		#endif	
//	
//		#ifdef uart_start
//		
//			if((etp[0]==5)&&(etp[1]==6)&&(etp[2]==9)&&(etp[3]==4)&&(etp[4]==1)&&(etp[5]==2)&&(etp[6]==0))
//			{
//			SEGGER_RTT_printf(0,"ETP_Tab:\r\n");
//			for(i =0;i<sizeof(etp);i++)
//			SEGGER_RTT_printf(0,"%d",etp[i]);
//			
//			SEGGER_RTT_printf(0,"\r\n");
//			SEGGER_RTT_printf(0,"it:%d\r\n",it);
//			}
//			else if(it==0)  //输出第一组
//			{
//			SEGGER_RTT_printf(0,"ETP_Tab:\r\n");
//			for(i =0;i<sizeof(etp);i++)
//			SEGGER_RTT_printf(0,"%d",etp[i]);
//			
//			SEGGER_RTT_printf(0,"\r\n");
//			SEGGER_RTT_printf(0,"it:%d\r\n",it);
//			}
//		#endif
//		}
				//void des_psw(uint8_t *psw,uint8_t psl,uint32_t unix_t);

#ifdef uart_start


//des_psw();

#endif

//			etp[0] = 8;
//			etp[1] = 5;
//			etp[2] = 6;
//			etp[3] = 9;
//			etp[4] = 6;
//			etp[5] = 0;
//			etp[6] = 6;
//			etp[7] = 8;
//			etp[8] = 8;

//		i =	des_psw(etp,7,Unix_T);
//		
//		SEGGER_RTT_printf(0,"Act_PassWord_T(ST + AcT HEX):\r\n");
//		SEGGER_RTT_printf(0,"%x\r\n",i);
//
//		#endif

//	#ifdef uart_start
//	
//	close_uart();   //操作完成后关闭串口
//	
//	#endif


	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();
	
	sky1311_Init();
	
	while(SKY1311_RC_VALUE()==0)nrf_delay_ms(1000);

	Spi_Flash_Init();


	sky1311Reset();         
	

	irqClearAll();              // 清除SKY1311的所有中断
	checkCardInit(LEDCL_RC_VALUE);    // 配置进入自动检卡状态
	cardStatus = GOTOSLEEP;

	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
//	
	Spi_Flash_Sleep ();
	
	//nrf_delay_ms(100);

		
	Spi_Flash_Init();
	
	//CL_FlashWord_Num();
	
//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,25);  //将新的数据量写入目录
//	
//	i =Get_Cat_Dat(AD_TypeA_Data_Num_Code);
//	
//	
	
//	Add_Flash_AD_TypeA_Data(Type_Card_Id,123456,567890);
//	
//	Add_Flash_AD_TypeA_Data(Type_Card_Id,567890,123456);
//	
//	Type_Card_Id[0] = 9;
//	
//	Add_Flash_AD_TypeA_Data(Type_Card_Id,567890,123456);
//	
//	Type_Card_Id[0] = 8;
//	
//	Add_Flash_AD_TypeA_Data(Type_Card_Id,567890,123456);

//		Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //将新的数据量写入目录
//		
//		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //将新的数据量写入目录
//		
//		Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //将新的数据量写入目录
//		
//		Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //将新的数据量写入目录
//		

//		for(i =0;i<(20);i++)
//		{
//			
//			Type_Card_Id[0] =i;
//			
//			Add_Flash_AD_TypeA_Data(Type_Card_Id,2,7);
//			NRF_WDT_Feed();
//		}

//		Type_Card_Id[1] = 16;
//		Add_Flash_AD_TypeA_Data(Type_Card_Id,0xFFFF,0XFFFF);

		NRF_WDT_Feed();
		
//		
//		for(i =0;i<(500);i++)
//		{
//			
//			Type_Card_Id[0] =i&0xFF;
//			Type_Card_Id[1] =i>>8&0XFF;
//			
//			Add_Flash_US_TypeA_Data(Type_Card_Id,3,6);
//			NRF_WDT_Feed();
//		}
//		
//		Type_Card_Id[0] = 16;
//		Add_Flash_US_TypeA_Data(Type_Card_Id,0XFFFF,0XFFFF);
//		NRF_WDT_Feed();
//		
//		for (i =0;i<(20);i++)
//		{
//			
//			Type_Card_Id[0] =i;
//			
//			Add_Flash_AD_TypeB_Data(Type_Card_Id,5,6);
//			NRF_WDT_Feed();
//			
//		}
//		Type_Card_Id[0] = 16;
//		Add_Flash_AD_TypeB_Data(Type_Card_Id,0XFFFF,0XFFFF);
//		NRF_WDT_Feed();

//		
//		
//		for (i =0;i<(20);i++)
//		{
//			
//			Type_Card_Id[0] =i;
//			Type_Card_Id[7] =i;
//			
//			Add_Flash_US_TypeB_Data(Type_Card_Id,5,6);
//			NRF_WDT_Feed();
//		}
//		
//		Type_Card_Id[0] = 16;
//		Type_Card_Id[7] = 16;
//		
//		Add_Flash_US_TypeB_Data(Type_Card_Id,0XFFFF,0XFFFF);
//		NRF_WDT_Feed();
//	
//		Del_AD_Type_A(3);
//		
//		Del_US_Type_A(5);
//		
//		Del_AD_Type_B(7);
//		
//		Del_US_Type_B(9);
//		
//			read_Soctor(1);

//			NRF_WDT_Feed();
//		
//////		
//		
		//Se 25
		
//		#ifdef uart_start
//		
//		SEGGER_RTT_printf(0,"\r\n AD_TypeA_Data_Secotr1:\r\n");
//		
//		#endif
//		
//		read_Soctor(AD_TypeA_Data_Secotr1);  //获取管理员扇区数据
//		
//		NRF_WDT_Feed();
//////		
		
		//Se 65
		
		
//		#ifdef uart_start
//		
//		SEGGER_RTT_printf(0,"\r\n US_TypeA_Data_Secotr1:\r\n");
//		
//		#endif
		
		
//		read_Soctor(US_TypeA_Data_Secotr1);  //获取管理员扇区数据
//		
//		NRF_WDT_Feed();

////		

//		//Se 45
//		
//		
//		#ifdef uart_start
//		
//		SEGGER_RTT_printf(0,"\r\n AD_TypeB_Data_Secotr1:\r\n");
//		
//		#endif
//		
//		
//		read_Soctor(AD_TypeB_Data_Secotr1);  //获取管理员扇区数据
//		
//		NRF_WDT_Feed();
//		
		
		//Se 85
		
		
//		#ifdef uart_start
//		
//		SEGGER_RTT_printf(0,"\r\n US_TypeB_Data_Secotr1:\r\n");
//		
//		#endif
//		
//		
//		read_Soctor(US_TypeB_Data_Secotr1);  //获取管理员扇区数据
//		
//
//		nrf_gpio_pin_clear(LEDR);
//		nrf_gpio_pin_set(LEDB);
	//	nrf_gpio_pin_set(LEDB1);
	
		SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua	
		Spi_Flash_Sleep();
	
		nrf_delay_ms(100);
		TSM12_Init();

		nrf_gpio_cfg_input(7,NRF_GPIO_PIN_PULLUP);  //配置GPIO为输入

	
	//Uart_Start_TC();
		
//	A71_Send_Init();


	
	//A71_Data_TX(TX_BUFF);
	//nrf_delay_ms(300);
	//A71_Data_TX(TX_BUFF);
//	nrf_delay_ms(300);
//	A71_Data_TX(TX_BUFF);
//	nrf_delay_ms(300);
//	A71_Data_TX(TX_BUFF);
//	nrf_delay_ms(300);
//	A71_Data_TX(TX_BUFF);
//	nrf_delay_ms(300);
//	A71_Data_TX(TX_BUFF);
	
	
	//input_speak(28);
	
	#ifdef PS_Enable   //开启串口功能
	
//	for(i =0;i<sizeof(TX_BUFF);i++)
//		TX_BUFF[i] =i;
	
//	if(A71_RF_RX_MODE()==1)  //当模块进入接收模式后
//	{
//		
//	}
	
//	NRF_WDT_Feed();
//	
//	
//	A71_ID_Tab[0] =0x34;
//	A71_ID_Tab[1] =0x75;
//	A71_ID_Tab[2] =0xC5;
//	A71_ID_Tab[3] =0x8C;
//	 
//	
//	A71_Send_ID();
	
//	Need_Of_RX_Dat();
	
//	for(i =0;i<7;i++)
//	{
//		A71_TX_Dat(TX_BUFF);
//		//nrf_delay_ms(200);
//		NRF_WDT_Feed();
//	}
	
	
/*		测试指纹头协议
	TX_BUFF[0] = 0XEF;
	TX_BUFF[1] = 0X01;

	TX_BUFF[2] = 0XFF;
	TX_BUFF[3] = 0XFF;
	TX_BUFF[4] = 0XFF;
	TX_BUFF[5] = 0XFF;
	
	TX_BUFF[6] = 0X01;
	TX_BUFF[7] = 0X00;
	TX_BUFF[8] = 0X03;
	TX_BUFF[9] = 0X01;
	TX_BUFF[10] = 0X00;
	TX_BUFF[11] = 0X05;

	
	Uart_Put_String(TX_BUFF,12);

*/
		Test_Ps_Mark =1;
		Controllable_Power_No();
		
		nrf_delay_ms(500);

for (i =0;i<sizeof(PS_PowDlow);i++)			//在发送缓存区写入握手指令
	{
		
		TX_BUFF[i] = PS_PowDlow[i];
		
	}
	
	Uart_Put_String(TX_BUFF,sizeof(PS_PowDlow));
	F_print_Enable =0;
	Test_Ps_Mark =1;		// 测试指纹头是否存在的标志
	
	
	Input_Ps_Lmage_Run_Now = 0;		//清空状态机
	Input_Ps_Lmage_Type = 0;				//初始化指纹录入相关值
	Start_PS_Unix = 0;
	Stop_PS_Unix = 0;

	
	#endif
	
#ifdef uart_Enable
	
TX_BUFF[0] = 0xFF;
	
	Uart_Put_String(TX_BUFF,1);

#endif
	
#ifdef NB_Enable
	
	NB_Weak_Up();

#endif

//	FD_Key =0;
//	nrf_gpio_cfg_input(13,NRF_GPIO_PIN_PULLUP);			//定义防拆按键
	
	//动态密码保存程序
//	More_Act_PassWord_Save(uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t); 

/******************************************************
	#ifdef uart_start    //测试模式
	
	Spi_Flash_Init();  //开启SPI_FLASH 通讯	

	Ev_Cat_Dat(More_Act_PassWord_Num_Code,0xFFFF0000);  //改为多组模式
	
	u8 tab[20];
	
	tab[0] = 6;
	tab[1] = 6;
	tab[2] = 6;
	tab[3] = 6;
	tab[4] = 6;
	tab[5] = 0;
	
	More_Act_PassWord_Save(tab,6,0x23333333);
	
	tab[5] = 1;
	More_Act_PassWord_Save(tab,6,0x23333333);
	
	tab[5] = 2;
	More_Act_PassWord_Save(tab,6,0x23333333);
	
	tab[5] = 3;
	More_Act_PassWord_Save(tab,6,0x23333333);

	tab[5] = 4;
	More_Act_PassWord_Save(tab,6,0x23333333);

	tab[5] = 5;
	More_Act_PassWord_Save(tab,6,0x23333333);

	tab[5] = 6;
	More_Act_PassWord_Save(tab,6,0x23333333);

	tab[5] = 7;
	More_Act_PassWord_Save(tab,6,0x23333333);

	tab[5] = 8;
	More_Act_PassWord_Save(tab,6,0x23333333);

	tab[5] = 9;
	More_Act_PassWord_Save(tab,6,0x23333333);
	
	
	//多组动态密码中单个删除
		//uint8_t Dle_A_More_Act_PassWord_Verify(uint8_t *PassWord);				//从多组动态密码中删除一个
	
	
	
//	Dle_A_More_Act_PassWord_Verify(tab);
	
	tab[0] = 6;
	tab[1] = 6;
	tab[2] = 6;
	tab[3] = 6;
	tab[4] = 6;
	tab[5] = 6;
	tab[6] = 5;
	
	Dle_A_More_Act_PassWord_Verify(tab);

	tab[6] = 7;
	Dle_A_More_Act_PassWord_Verify(tab);

	tab[6] = 3;
	Dle_A_More_Act_PassWord_Verify(tab);

	tab[6] = 9;
	Dle_A_More_Act_PassWord_Verify(tab);

	tab[6] = 7;
	Dle_A_More_Act_PassWord_Verify(tab);
	
	tab[6] = 7;
	Dle_A_More_Act_PassWord_Verify(tab);
	
	tab[6] = 7;
	Dle_A_More_Act_PassWord_Verify(tab);
	
	SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
	Spi_Flash_Sleep();
			
	#endif	
	*******************************************************************/
	
	for(;;)
  {
				
//	Get_Time();
//	nrf_adc_read();
//	sky1311_run();
		
			NRF_WDT_Feed();
			
			if(!MPR121_RUN)
			Bsp_Set();   //在运行中重置 BSP
//			sky1311_Read();
			if((BLE_LINK==0)&&(Devic_Static==false)&&(!MPR121_RUN)) //当蓝牙断开，且设备未绑定,键盘未触发时关闭广播
			{
				
				sd_ble_gap_adv_stop();
			
			}
			
//			if((Note_Num_1>0)&&(Note_Num_1<=Open_Note_Size)&&(opdt == 0))		//输出开门记录   改为逐条返回
//			{
//				Note_Num_Delay++;
//				
//				if(Note_Num_Delay>=1)   // 等待蓝牙稳定
//				{
//						err_code = app_timer_start(my_Op_Door_timer,APP_TIMER_TICKS(my_Op_Door_timer_Size,APP_TIMER_PRESCALER),NULL);			//注册新的软件定时器任务
//						APP_ERROR_CHECK(err_code);
//						opdt =1;
//						Note_Num_Delay =0;
//					
//				}
//			}
			
//			#ifdef NB_Enable     ///有 NB 模组存在时
//			
//			if((UART_RUN==0)&&(NB_Dat_Len==0)&&(Input_Ps_Lmage_Type==0))   //当串口关闭且NB 缓存没有数据时
//			{
//				
//					power_manage();
//				
//			}
//			else if((UART_RUN==0)&&(NB_Dat_Len>0))   //串口关闭后， NB 缓存区有数据时，开始处理NB 缓存区的数据
//			{
//				
//				
//				
//			}
//			
//			#endif
//			

			#ifdef uart_Enable
			
			if((UART_RUN==0)&&((F_print_Enable==0)||((Input_Ps_Lmage_Type==0)&&(Controllable_Power_State==0)&&(F_Print_Run_Flag==0)&&(CL_PS_Print_Now==0)&&(CL_PS_Run==CL_PS_Run_Del_Nor_Mode)))&&(FD_Key==0))     //普通串口模式	
							power_manage();
			
			#else
			
			if(FD_Key==0)
				power_manage();
			
			#endif
			
			FD_Key_Run();
			
			#ifdef PS_Enable
			
//			if((F_print_Enable==1)||Test_Ps_Mark==1)
//			{
				
				F_Print_Input_Ps_Run();		//录入指纹 运行
				
				PS_Print_Run();			//指纹头触发外部唤醒运行
				
				CL_PS_Print_Run();		//清除指纹头数据 运行
				
//			}
//			else
//			{
//				Input_Ps_Lmage_Type=0;
//				Controllable_Power_State =0;
//				F_Print_Run_Flag =0;
//				CL_PS_Print_Now =0;
//				CL_PS_Run = CL_PS_Run_Del_Nor_Mode;
//				
//			}
			
			#endif
//		
			
	//	APP_ERROR_CHECK(err_code);
			
		//	input_speak(29);
			
//			#ifdef uart_start
//			SEGGER_RTT_printf(0,"Heart!\r\n");
//			#endif                               
			
	}
	
//		ADC_F();

}


uint32_t FD_Key_Time =0;
void FD_Key_Run()
{
	
	if(FD_Key==1)
	{
		FD_Key_Time ++;
		
		if(FD_Key_Time>170000)
		{
			
			FD_Key_Time =0;
			input_speak(20);				//提示非法操作
			
		}
	}
	
	if(nrf_gpio_pin_read(13)==1)
	{
		FD_Key =0;
		FD_Key_Time =0;
	}
	else
	{
		
		FD_Key =1;
	
	}
}

uint8_t MPR121_Flag =0;

void F_Print_Input_Ps_Run()				//运行指纹录入程序(直接放在主函数里一定条件下自动唤醒调用)
{
			if(UART_RUN==1)					//串口接收程序
			{
				if((MPR121_RUN==1)&&(MPR121_Flag ==0))			//当键盘开启时，为了串口通信流程，需要暂时关闭该定时器
				{
					MPR121_Flag =1;
					app_timer_stop(Read_My_Key);

				}
				
				Uart_Get_String();
				
			}
			else if((MPR121_RUN==1)&&(MPR121_Flag==1))
			{
				
				MPR121_Flag =0;
				app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	//通信结束时，回复定时器状态

			}
			
			if((Input_Ps_Lmage_Type!=0)&&
	//			(Controllable_Power_State==0)&&		//指纹头未被唤醒时			
				(F_print_Enable==1)			//当开启指纹录入时实现指纹录入间隔
				&&(CL_PS_Print_Now==0))		//删除指纹未运行
				Input_PS_Lmage_t++;
			else
			{
				Input_Ps_Lmage_Type =0;
				Input_PS_Lmage_t =0;			//其他情况下清空这个数值
			}
			
			if(Input_PS_Lmage_t>1500)
			{
			
				Input_PS_Lmage();				//录入指纹程序
				Input_PS_Lmage_t =0;
			
			}
}

/*********************************
void PS_Print_Run()

 指纹模块在运行状态下的状态

*********************************/

uint32_t OP_PS_AU;		//判断指纹是管理员还是用户

uint8_t PS_Print_Run_Now =0;
uint16_t PS_Delay_T =0;			//用于配置状态机工作的间隔时间
uint8_t PS_Int_T =0;			//指纹中断脚触发超时处理
uint16_t Need_For_Ps_Print_ID =0XFFFF;		//从指纹头获取到的 ID 号（内部有匹配指纹时）

const uint16_t PS_Delay_Out_T	= 300;			//配置状态机延迟时间
const uint16_t PS_Dat_T_Out = 20000;				//配置状态机等待数据包的溢出时间
void PS_Print_Run()
{
	
	
	#ifdef PS_Enable 
	
	if(nrf_gpio_pin_read(F_Print_IRQ)&&(Controllable_Power_State==0)&&(F_Print_Run_Flag==0)&&(CL_PS_Print_Now==0)&&(F_print_Enable==1))					//指纹触发中断
	{
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"F_Print_IRQ_Input!\r\n");
		
		#endif
		Controllable_Power_No();		//给指纹模块上电
		
		if(Devic_Static==false)  //当设备未绑定时，可直接用指纹开门(不对数据做任何处理，开门即使指纹进入睡眠状态)
		{
				nrf_gpio_pin_set(LEDB);
		//		nrf_gpio_pin_set(LEDB1);  //直接开灯
			
				Lock_Open();
							
				#ifdef uart_start
					
				SEGGER_RTT_printf(0,"F_Print_Open_Door!\r\n");	
					
				#endif
			
				if(SPI0_Start==1)			// 当  SPI0 在运行时  
				{
						;		//运行时不做唤醒按键板响应
				}
				else
				{
					Start_Key_Run();			//当SPI0 未运行时，直接唤醒键盘
				}
			F_Print_Run_Flag =1;			//表示指纹头进入运行状态
		}
		else
		{
			
			F_Print_Run_Flag =1;			//使指纹头进入运行状态
			
		}
		
	}
	
	#endif
	
	
	if((PS_Print_Run_Now==0)&&(F_Print_Run_Flag==1))
	{
		
		PS_Print_Run_Now =1;			//当 F_Print_Run_Flag 被置1 时，表示 指纹头进入运行状态，需要开始一次运行过程
		
	}
	
	if(Input_Ps_Lmage_Type!=0)			//当有录入指纹的状态时，即刻停止运行状态
	{
		
		PS_Print_Run_Now =0;
		F_Print_Run_Flag =0;
		
	}
	
	switch(PS_Print_Run_Now)
	{

		case 0:
		;								//等待状态
		break;
		
		case 1:			//进入第一个状态，判断是否被绑定
		
		if(Devic_Static==1)		//当被绑定时
		{
		
			PS_Print_Run_Now =8;				//在被绑定时，尝试读取指纹是否合法
			nrf_delay_ms(100);				//额外增加100ms延迟，用于等待手指稳定
		}
		else			//未被绑定时  在确认手指离开模块后进入睡眠模式
		{
			PS_Print_Run_Now=2;		//进入等待手指离开并睡眠的状态
		}
		break;
		
		case 2:				//发送验证指纹是否存在的指令（指纹模组进入睡眠的开始阶段）
		
		PS_Delay_T =0;		//清空延迟时间
		PS_Print_Run_Now =3;		//进入下一步	
		
		break;
		
		case 3:
		
		PS_Delay_T++;			//增加延迟	
		if(PS_Delay_T>=PS_Delay_Out_T)			//当溢出时
		{
			PS_Delay_T =0;
			PS_Print_Run_Now=4;			//时间满足进入下一步
		}
		
		break;
		
		case 4:			//发送指纹读取指令，以确定手指是否离开
		
			for(uint8_t i =0;i<sizeof(PS_Getlmage);i++)			//在发送缓存区写入读取指纹指令
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		
		}
		PS_Delay_T =0;		//下一步用于防止丢包
		PS_Print_Run_Now =5; //指向下一步
		
		break;

		case 5:
		PS_Delay_T++;			//记录等待时间
		if(PS_Delay_T>=PS_Dat_T_Out)		//时间溢出时重新发送
		{
			PS_Print_Run_Now =2;		//溢出后返回上一步
			PS_Delay_T =0;	
		}
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//当收到的数据包为手指抬起时，进入下一步
			)					//当获得的数据
		{
			
			PS_Print_Run_Now=6;
			PS_Delay_T =0;
		}
		else if(
		(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)//&&
		//(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为手指按下时，回到上一步
				)
		{
			
			PS_Print_Run_Now = 2;		//回到上一步起始阶段
			PS_Delay_T =0;
			
		}
		
		break;
		
		case 6:				//进入睡眠状态
		
		PS_Delay_T++;
		
		if(PS_Delay_T>=PS_Delay_Out_T)			//当延时溢出时
		{		
			PS_Delay_T =0;
			PS_Print_Run_Now=7;			//时间满足进入下一步
		}
		
		
		break;
		
		case 7:				//指纹模组进入睡眠的最后阶段
		
		if(F_print_Goto_Sleep()==1)			//操作完成
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Print_Run_End!\r\n");		//提示运行结束并且断掉指纹头电源
			
			#endif
			PS_Print_Run_Now =0;
			Controllable_Power_Off();			//关闭电源
			F_Print_Run_Flag =0;			//清空运行标志

		}
		else
		{
			PS_Delay_T++;
			if(PS_Delay_T>=PS_Delay_Out_T)			//时间溢出时返回上一步
			{
				PS_Delay_T =0;
				PS_Print_Run_Now =6;
				F_Print_Run_Flag =0;			//清空运行标志

			}
		}
			
		break;
		
		
		case 8: //获取数据
			
		PS_Delay_T =0;		//清空延迟时间
		PS_Print_Run_Now =9;		//进入下一步	
		
		#ifdef uart_start
		
		
		SEGGER_RTT_printf(0,"GOTO_PS_9!\r\n");
		
		#endif
		
		break;
		
		case 9:			//开始读取指纹，用于对比本地模板
			//控制读取指纹间隔
		PS_Delay_T++;
		
		if(PS_Delay_T>=PS_Delay_Out_T)
		{
			
			PS_Delay_T =0;
			PS_Print_Run_Now = 10;
			
		}
		
		break;
		
		case 10:
		
		for(uint8_t i =0;i<sizeof(PS_Getlmage);i++)			//在发送缓存区写入读取指纹指令
		{
			
			TX_BUFF[i] = PS_Getlmage[i];
			
		}
		
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		}
		
		PS_Delay_T =0;		//下一步用于防止丢包
		PS_Print_Run_Now =11; //指向下一步
		
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"GOTO_PS_11!\r\n");
			
			#endif
		
		
		break;
		
		case 11:
			
		PS_Delay_T++;			//记录等待时间
		
		if(PS_Delay_T>=PS_Dat_T_Out)		//时间溢出时重新发送
		{
			
			PS_Print_Run_Now =9;		//溢出后返回上一步
			PS_Delay_T =0;
			
		}
		//开始判断收到的数据包
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//当收到的数据包为手指抬起时，准备进入待机状态
			)					//当获得的数据
		{
			PS_Print_Run_Now =2;			//未检测到手指时，准备重新发送(待机)
		//	input_speak(42);		//提示指纹错误
			PS_Delay_T =0;		//清空 计时
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)					//当收到的数据包为手指按下时，进入下一步
					)
					{	
						PS_Print_Run_Now =12;			//进入下一步
						PS_Delay_T =0;		//清空 计时
					}					
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//收到其他包时，准备进入待机状态
					)
					{
						
						PS_Print_Run_Now = 2;			//准备进入待机状态
	//			input_speak(42);			//提示指纹错误
						PS_Delay_T =0;		//清空 计时
						
					}
					
		break;
					
		case 12:			//发送保存数据到缓存1 (起点)
		
		PS_Delay_T++;
		if(PS_Delay_T>=PS_Delay_Out_T)
		{
			
			PS_Delay_T =0;
			PS_Print_Run_Now = 13;
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"GOTO_PS_13!\r\n");
			
			#endif
			
		}
		
		
		break;
		
		case 13:			//开始尝试保存数据到缓存 1	
		
		for(uint16_t i =0;i<sizeof(Get_Dat_Save_1);i++)			//在发送缓存区写入保存指令
			{
				
				TX_BUFF[i] = Get_Dat_Save_1[i];
				
			}
			
		Uart_Put_String(TX_BUFF,sizeof(Get_Dat_Save_1));

		for(uint16_t i =0;i<sizeof(Ps_Data_Tab);i++)
			{
			
				Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
			
			}
		PS_Delay_T =0;
		PS_Print_Run_Now = 14;		//进入下一步
		
		break;
		
		case 14:			//等待接收确认包
				
		PS_Delay_T++;			//记录等待时间
		if(PS_Delay_T>=PS_Dat_T_Out)		//时间溢出时重新发送
		{
			PS_Print_Run_Now =12;		//溢出后返回发送保存到缓存1的起点
			PS_Delay_T =0;
			
		}
		
		//收到确认包判断是否有效
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为存储成功的应答时进入下一步
					)
				{
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_1_OK!\r\n");
					
					#endif
					PS_Print_Run_Now =15;				//收到确认包后进入下一步
					PS_Delay_T =0;		//清空定时
				}
				else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_1_Error!\r\n");
					
					#endif
					
					PS_Print_Run_Now =9;		//其他数据包 开始尝试重新录入指纹数据
					
					PS_Delay_T =0;			//清空定时
					
				}
		
		
		break;
		
		case 15:			//将获取到的指纹与指纹库进行匹配 （不需要延迟）
		
		for(uint8_t i =0;i<sizeof(Se_Lmage_Dat);i++)			//在发送缓存区写入读取指纹指令
		{
			
			TX_BUFF[i] = Se_Lmage_Dat[i];
			
		}
		
		Uart_Put_String(TX_BUFF,sizeof(Se_Lmage_Dat));
		
		for(uint8_t i =0;i<sizeof(Ps_Data_Tab);i++)
		{
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		}
		
		PS_Delay_T =0;		//下一步用于防止丢包
		PS_Print_Run_Now =16; //指向下一步
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"GOTO_PS_16!\r\n");
		
		#endif
		
		break;
		
		case 16:
			
		PS_Delay_T++;			//记录等待时间
		if(PS_Delay_T>=PS_Dat_T_Out)		//时间溢出时重新发送
		{
			PS_Print_Run_Now =12;		//溢出后返回发送保存到缓存1的起点
			PS_Delay_T =0;	
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"From16_GOTO_12!\r\n");
			
			#endif
		}
		//获取指纹匹配结果
		if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)
			)
		{
			if(
				(Ps_Data_Tab[8]==0x07)&&((Ps_Data_Tab[9]==0x00)||(Ps_Data_Tab[10]==0x01))		//当获取到匹配的ID时
				
				)
				{
					//将获取到的 ID 放到 Need_For_Ps_Print_ID 
					Need_For_Ps_Print_ID =0XFFFF;
					Need_For_Ps_Print_ID = Ps_Data_Tab[10];
					Need_For_Ps_Print_ID += Ps_Data_Tab[11];
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"\r\n\r\nPS_ID: %x!\r\n\r\n",Need_For_Ps_Print_ID);
					
					#endif
					
					if(Need_For_Ps_Print_ID>PS_Print_Num)		//当获取到的 ID 大于本地容量极限时
					{
						
						input_speak(42);		//提示指纹错误
						PS_Print_Run_Now =2;		//等待指纹头进入待机
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Print_ID_Error!\r\n");		//提示获取到的指纹ID 无效
						
						#endif
						
					}
					else		//当数据合理时
					{
						PS_Print_Run_Now =17;			//进入下一步，开始验证 ID 号是否允许开门
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\n GOTO_17!\r\n");
						
						#endif
						
					}
				}
				else			//未匹配到 一致的指纹 使指纹头进入睡眠
				{
					
						input_speak(42);		//提示指纹错误
						PS_Print_Run_Now =2;		//等待指纹头进入待机
					
				}
		}
		
		break;
		
		case 17:			//开始验证 ID 号是否允许开门
			
		OP_PS_AU = Opinion_PS_Print_Valid(Need_For_Ps_Print_ID);
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"OP_PS_AU: %x\r\n",OP_PS_AU);
		
		#endif
		
		if(OP_PS_AU!=0xFFFF)		//当指纹有效时
		{
			//允许开门		
			Spi_Flash_Init();  //开启SPI_FLASH 通讯 (因为此处SPI接口未打开)
			OP_PS_AU&=0XFF0000;
			
			if(OP_PS_AU==0X010000)		//管理员指纹开门
			{
				ADD_Op_Door_Note(Need_For_Ps_Print_ID,0x0101,Unix_T);
			}
			else if(OP_PS_AU==0X020000)  //用户指纹开门
			{
				ADD_Op_Door_Note(Need_For_Ps_Print_ID,0x0201,Unix_T);
			}

			SpiFlash_PowerDown();   //完成操作时，使其进入低功耗模式 1ua
			Spi_Flash_Sleep();
			
			Lock_Open();			//开门
		}
		else
		{
			input_speak(42);		//提示指纹错误
		}
		
		//最后都指向准备指纹头待机
		
		PS_Print_Run_Now =2;			//送到待机步骤起点
		
		break;
		
		
		default:			//进入这里，直接让模块进入休眠的状态 （一般不可能进入）
			
			PS_Print_Run_Now =2;			//进入睡眠等待状态
		
		break;

	}
	
}






/************************
删除 指纹 状态机

（仅干涉指纹头内部数据）

************************/

uint16_t CL_PS_ID =0;			//设定 要删除的 指纹 ID  
uint16_t CL_PS_Run =0;				//设定 删除指纹头运行模式

uint8_t CL_PS_Print_Now =0;
uint16_t CL_PS_Delay_T =0;


const uint16_t CL_PS_Delay_T_Out =300;
const uint16_t CL_PS_Time_Out = 20000;		//设定等待超时

//因为相关限制因素 目前只开放删除单个 ID 和 全部清除的方式 批量删除功能下次一定更！~

//定义删除模式的常量

const uint8_t CL_PS_Run_Del_Nor_Mode =0;					//删除 指纹ID 功能挂起
const uint8_t CL_PS_Run_Del_Single_Mode =1;				//删除单个 指纹ID 的模式
const uint8_t CL_PS_Run_Del_All_Mode =2;					//删除所有 指纹ID 的模式
const uint8_t CL_PS_Run_Del_Batch_Mode =3;				//批量删除 指纹ID 的模式

void CL_PS_Print_Run()
{
	
	uint16_t Dat_cheak;				//校验数据变量
	
//	if((F_Print_Run_Flag==0)&&(Input_Ps_Lmage_Type==0)&&(CL_PS_Run!=CL_PS_Run_Del_Nor_Mode)&&(Controllable_Power_State==0)) //当其他两项未运行且 CL_PS_Run 未被设置时
//	{
//		
//		#ifdef uart_start
		
		
//		SEGGER_RTT_printf(0,"CL_PS_Run: %x!\r\n",CL_PS_Run);
//		SEGGER_RTT_printf(0,"CL_PS_ID: %x!\r\n",CL_PS_ID);
//		
//		while(1);
//		#endif
		
		if((Controllable_Power_State==0)&&(F_Print_Run_Flag==0)&&(Input_Ps_Lmage_Type==0)&&(CL_PS_Print_Now==0)&&(MPR121_RUN==0))			//当指纹关闭时
		{
			switch(CL_PS_Run)			//根据  Run 值 确定删除方式
			{
				case 0:						//为 0 时无功能
				
			//	CL_PS_Print_Now =0;											//使后面的状态机无效
			//	CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//同时清空模式转化位
			//	CL_PS_Delay_T =0;						//清空定时值
				
				break;
				
				case CL_PS_Run_Del_Single_Mode:			//删除单个指纹的模式
				
				CL_PS_Print_Now =7;					//使 CL_PS_Print_Now 指向删除一个指纹
				if(Controllable_Power_State==0)			//当指纹电源被关闭时打开指纹模块电源
					{
						Controllable_Power_No();				
					}
					
		//		CL_PS_Delay_T =0;						//清空定时值pw
				
				CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//同时清空模式转化位
					
				break;
				
				case CL_PS_Run_Del_All_Mode:				//删除所有指纹的模式
				
				CL_PS_Print_Now =10;
				
				if(Controllable_Power_State==0)			//当指纹电源被关闭时打开指纹模块电源
					{
						Controllable_Power_No();				
					}
		//		CL_PS_Delay_T =0;						//清空定时值
				
				CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//同时清空模式转化位
				
				break;
				
				case CL_PS_Run_Del_Batch_Mode: 
					
				CL_PS_Print_Now =0;
			//	CL_PS_Delay_T =0;						//清空定时值
				
				CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//同时清空模式转化位
				
				break;
				
				
				default:			//其他情况下清空状态位
				
				CL_PS_Print_Now =0;
				CL_PS_Run = CL_PS_Run_Del_Nor_Mode;
			//	CL_PS_Delay_T =0;						//清空定时值
				
				
				break;
				
			}
	}
		
//	}
//	else			//不满足条件时清空
//	{
////		CL_PS_Print_Now =0;
//		CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//同时清空模式转化位
////		CL_PS_Delay_T =0;						//清空定时值
//	}
	
	switch (CL_PS_Print_Now)
	{
		case 0:			//空闲
				CL_PS_Delay_T =0;
			break;
		case 1:			//进入待机模式 起点
			CL_PS_Delay_T =0;				//清空等待时间变量
			CL_PS_Print_Now =2;		//进入下一步
		
			break;
		case 2:			//发送延迟
			
			CL_PS_Delay_T ++;
			if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//当时间溢出进入下一步
			{
				
				CL_PS_Delay_T =0;
				CL_PS_Print_Now =3;
				
			}
		
			break;
			
		case 3:			//发送指纹读取指令，以确定手指是否离开
			
			for(uint8_t i =0;i<sizeof(PS_Getlmage);i++)			//在发送缓存区写入读取指纹指令
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
			
		}
		
		CL_PS_Delay_T =0;		//下一步用于防止丢包
		CL_PS_Print_Now =4; //指向下一步
		
		break;
		
		case 4:
			
			CL_PS_Delay_T++;			//记录等待时间
			if(CL_PS_Delay_T>=CL_PS_Time_Out)		//时间溢出时重新发送
			{
				CL_PS_Print_Now =5;		//溢出后进跳过入下一步
				CL_PS_Delay_T =0;	
			}
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//当收到的数据包为手指抬起时，进入下一步
			)					//当获得的数据
		{
			
			CL_PS_Print_Now=5;
			CL_PS_Delay_T =0;
		}
		else if(
		(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)//&&
		//(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为手指按下时，回到上一步
				)
		{
			
			CL_PS_Print_Now = 1;		//回到上一步起始阶段
			CL_PS_Delay_T =0;
			
		}
			
		break;
		
		case 5:				//无指纹触发时进入睡眠状态
		
		CL_PS_Delay_T++;
		
		if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//当延时溢出时
		{		
			
			CL_PS_Delay_T =0;
			CL_PS_Print_Now =6;			//时间满足进入下一步
		
		}
		
		break;
		
		case 6:
		
		if(F_print_Goto_Sleep()==1)			//操作完成
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Print_Run_End!\r\n");		//提示运行结束并且断掉指纹头电源
			
			#endif
			CL_PS_Print_Now =0;
			Controllable_Power_Off();			//关闭电源
			
		}
		else
		{
			
			CL_PS_Delay_T++;
			if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//时间溢出时返回上一步
			{
				
				CL_PS_Delay_T =0;
				CL_PS_Print_Now =5;
				
			}
		}
		
		break;
		
		case 7:						//删除一个指纹从这里开始
			
		CL_PS_Delay_T++;					//延迟
		
		if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//当延时溢出时
		{		
			
			CL_PS_Delay_T =0;
			CL_PS_Print_Now =8;			//时间满足进入下一步
			

		}
		
		break;
		
		case 8:						//发送单个指纹删除数据包
		
			for(uint8_t i =0;i<sizeof(PS_Del_A_Lmage);i++)			//在发送缓存区写入读取指纹指令
			{
				
				TX_BUFF[i] = PS_Del_A_Lmage[i];
				
			}
	
		TX_BUFF[10] =0;					// id 高位设置为 0  
		TX_BUFF[11]	=CL_PS_ID;  //设定 ID 号
			
		//求校验和
		
			Dat_cheak =0;			//清空校验值
		for(uint8_t i;i<(sizeof(PS_Del_A_Lmage)-8);i++)
			{
				Dat_cheak += TX_BUFF[i+6];
			}
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Dat_cheak: %x.\r\n",Dat_cheak);			//输出 校验值
			
		#endif
			
		TX_BUFF[14] = Dat_cheak&0XFF>>8;
		TX_BUFF[15] = Dat_cheak&0xFF;				//将校验位放到数据里
		
		Uart_Put_String(TX_BUFF,sizeof(PS_Del_A_Lmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
			
		}
		CL_PS_Print_Now =9;
		CL_PS_Delay_T =0;
		break;
		
		case 9:				//等待指纹接收数据
		
			CL_PS_Delay_T++;			//记录等待时间
			if(CL_PS_Delay_T>=CL_PS_Time_Out)		//时间溢出时重新发送
			{
				CL_PS_Print_Now =7;		//溢出后返回上一步起点
				CL_PS_Delay_T =0;	
			}
		
		//开始等待接收数据
			
		if((Ps_Data_Tab[0]==0XEF)&&(Ps_Data_Tab[1]==0x01))
		{
			if((Ps_Data_Tab[8]==0x03)&&((Ps_Data_Tab[9]==0x00)||(Ps_Data_Tab[9]==0x01)))				//当删除成功时
			{
					CL_PS_Print_Now =1;		//删除成功后进入待机起点
					CL_PS_Delay_T =0;	
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Del_A_PS_Lmage_OK!\r\n");
					
					#endif
					
				
			}
			else				//其他情况返回
			{
					CL_PS_Print_Now =7;		//溢出后返回上一步起点
					CL_PS_Delay_T =0;	
			
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Del_A_PS_Lmage_Warning!\r\n");
					
					#endif
							
			}
		}
		
		break;
		
		case 10:					//删除所有指纹的起点
			
		CL_PS_Delay_T++;					//延迟
		
		if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//当延时溢出时
		{		
			
			CL_PS_Delay_T =0;
			CL_PS_Print_Now =11;			//时间满足进入下一步
			
		}
		
		break;
		
		case 11:
		
			for(uint8_t i =0;i<sizeof(PS_Del_All_Lmage);i++)			//在发送缓存区写入删除所有指纹
			{
				
				TX_BUFF[i] = PS_Del_All_Lmage[i];
				
			}
	
			Uart_Put_String(TX_BUFF,sizeof(PS_Del_All_Lmage));
			
			for(uint8_t i =0;i<50;i++)
			{
				
				 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
			
			}
			
		CL_PS_Delay_T =0;
		CL_PS_Print_Now =12;
		
		break;
		
		case 12:
		
		CL_PS_Delay_T++;			//记录等待时间	
		
		if(CL_PS_Delay_T>=CL_PS_Time_Out)		//时间溢出时重新发送
		{
			
			CL_PS_Print_Now =1;		//溢出后返回上一步起点
			CL_PS_Delay_T =0;	
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"CL_PS_ALL_Time_Out!\r\n");
			
			#endif
			
		}
	
		//开始等待接收数据
		
		if((Ps_Data_Tab[0]==0XEF)&&(Ps_Data_Tab[1]==0x01))
		{
			if((Ps_Data_Tab[8]==0x03)&&((Ps_Data_Tab[9]==0x00)||(Ps_Data_Tab[9]==0x01)))				//当删除成功时
			{
				
					CL_PS_Print_Now =1;		//删除成功后进入待机起点
					CL_PS_Delay_T =0;	
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Del_All_PS_Lmage_OK!\r\n");
					
					#endif	
				
			}
			else				//其他情况返回
			{
					CL_PS_Print_Now =1;		//溢出后返回待机起点
					CL_PS_Delay_T =0;	
			
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Del_All_PS_Lmage_Warning!\r\n");
					
					#endif
							
			}
		}
		
		break;
		
		default:		//其他情况进入待机区域的状态机

		#ifdef uart_start
			
		SEGGER_RTT_printf(0,"Del_A_PS_Lmage_Error!\r\n");
			
		#endif

		CL_PS_Print_Now =1;
		
		break;
			
	}
	
}

/*********

		case 2:				//发送验证指纹是否存在的指令（指纹模组进入睡眠的开始阶段）
		
		PS_Delay_T =0;		//清空延迟时间
		PS_Print_Run_Now =3;		//进入下一步	
		
		break;
		
		case 3:
		
		PS_Delay_T++;			//增加延迟	
		if(PS_Delay_T>=PS_Delay_Out_T)			//当溢出时
		{
			PS_Delay_T =0;
			PS_Print_Run_Now=4;			//时间满足进入下一步
		}
		
		break;
		
		case 4:			//发送指纹读取指令，以确定手指是否离开
		
			for(uint8_t i =0;i<sizeof(PS_Getlmage);i++)			//在发送缓存区写入读取指纹指令
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//清空指纹模块接收缓存
		
		}
		PS_Delay_T =0;		//下一步用于防止丢包
		PS_Print_Run_Now =5; //指向下一步
		
		break;

		case 5:
		PS_Delay_T++;			//记录等待时间
		if(PS_Delay_T>=PS_Dat_T_Out)		//时间溢出时重新发送
		{
			PS_Print_Run_Now =2;		//溢出后返回上一步
			PS_Delay_T =0;	
		}
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//当收到的数据包为手指抬起时，进入下一步
			)					//当获得的数据
		{
			
			PS_Print_Run_Now=6;
			PS_Delay_T =0;
		}
		else if(
		(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)//&&
		//(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //当收到的数据包为手指按下时，回到上一步
				)
		{
			
			PS_Print_Run_Now = 2;		//回到上一步起始阶段
			PS_Delay_T =0;
			
		}
		
		break;
		
		case 6:				//进入睡眠状态
		
		PS_Delay_T++;
		
		if(PS_Delay_T>=PS_Delay_Out_T)			//当延时溢出时
		{		
			PS_Delay_T =0;
			PS_Print_Run_Now=7;			//时间满足进入下一步
		}
		
		
		break;
		
		case 7:				//指纹模组进入睡眠的最后阶段
		
		if(F_print_Goto_Sleep()==1)			//操作完成
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Print_Run_End!\r\n");		//提示运行结束并且断掉指纹头电源
			
			#endif
			PS_Print_Run_Now =0;
			Controllable_Power_Off();			//关闭电源
			F_Print_Run_Flag =0;			//清空运行标志

		}
		else
		{
			PS_Delay_T++;
			if(PS_Delay_T>=PS_Delay_Out_T)			//时间溢出时返回上一步
			{
				PS_Delay_T =0;
				PS_Print_Run_Now =6;
				F_Print_Run_Flag =0;			//清空运行标志

			}
		}
			
		break;

********/

/** 
 * @}
 */


