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
#include "Devic_Init.h"  //�豸��ʼ��
#include "Flash_RW.h"
#include "speak.h"
#include "ADC.H" 
#include <KEY_Run.h>
#include <NRF_WDT.H>
#include <Unix_Time.h>  //ʱ���ת��ͷ�ļ�
#include <BLE_Data.h>
#include <Act_PassWord.h>

#include "mcu.h"						//SKY1311 ͷ�ļ�
#include "stm8_eval.h"
#include "sky1311_drv.h"
#include "sky1311t.h"
#include "iso14443.h"
#include "cardoper.h"
#include "userdef.h"


#include "uart.h"

#include "simple_uart.h"  //�������贮��

#include <Uart_Run.h>   // ����

#include <F_print.h>  //ָ��Э��

#ifdef NB_Enable   //NB ʹ��ʱ�ſ�ʹ��

#include <NB_Mode1.h>  // NB

#endif

//��ʱ�㲥�� ǰ׺

#if YFA			//����ʶ�����ʱ

#define Devic_Name_H1	'Y'
#define Devic_Name_H2	'F'

//��ʱ�㲥�� ��׺
#define Devic_Name_E  'A'


#elif DSA			// ��ҳ���ʱ

#define Devic_Name_H1	'D'
#define Devic_Name_H2	'S'

//��ʱ�㲥�� ��׺
#define Devic_Name_E  'A'


#else				//��������ѡ��Ĭ��

#define Devic_Name_H1	'H'
#define Devic_Name_H2	'Y'

//��ʱ�㲥�� ��׺
#define Devic_Name_E  'P'


#endif

#define IS_SRVC_CHANGED_CHARACT_PRESENT  0                                           


/**< Include the service_changed characteristic. 

If not enabled, the server's database cannot be changed for the lifetime of the device. 
������������ԣ����δ���ã����豸�Ĺ��������У������������ݽ����ܱ�����

*/

//#define DEVICE_NAME                     "HYO_C1EAD0DC8E42"      //�㲥��

#if defined ( USB_SYSTEM_CLOCK_32MHZ )  //ʹ��32Mhzʱ��

#define DEVICE_NAME1 "HYO_C1EADCDC8E42_32MHz"

#else

#define DEVIC_NAME1 "HYO_23333"

#endif

//#define DEVICE_NAME1										"HYO_C1EADCDC8E42"				//�豸��

char DEVICE_NAME [28] = {"HYI_"};


/**
<  Name of device. Will be included in the advertising data. >
�豸���ƣ��������ڹ㲥������
*/

#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  
/**
< UUID type for the Nordic UART Service (vendor specific).
����Nordic UART Service ��UUID���� (�ض��ڹ�Ӧ��)
*/

#if YFA			//����ʶ�����ʱ



#define APP_ADV_INTERVAL            800   

#else


#define APP_ADV_INTERVAL            1200    


#endif

#define APP_AC_ADV_INTERVAL         100    
 

/**<
The advertising interval (in units of 0.625 ms. 
This value corresponds to 40 ms). 
�㲥�����һ����λΪ0.625ms
Ĭ���趨��40ms
*/


#define APP_ADV_TIMEOUT_IN_SECONDS      0
/**< 
The advertising timeout (in units of seconds). 
�㲥��ʱʱ�䣨������������㲥ʱ�䣬һ����λʱ��Ϊ�룬Ŀǰ����������ʱ�䣩
*/

#define APP_TIMER_PRESCALER             0                                           
/**< 
Value of the RTC1 PRESCALER register. 
RTC1 ʱ�Ӳο�
*/

#define APP_TIMER_OP_QUEUE_SIZE         10   //4       ������ ����ɨ�� ���ݰ���ʱ��� ���ż�¼�ش�  ����˸                            

/**<
Size of timer operation queues. 
��ʱ�����д�С
*/

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(15, UNIT_1_25_MS)             

/**<
Minimum acceptable connection interval (20 ms), 
Connection interval uses 1.25 ms units. 
�ɽ��ܵ���Сʱ����(Ԥ��20ms)
ʱ������ʱ�䵥λ��1.25ms
*/


#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             
/**< 
Maximum acceptable connection interval (75 ms), 
Connection interval uses 1.25 ms units. 
�ɽ��ܵ����ʱ����(Ԥ��75ms)
ʱ������ʱ�䵥λ��1.25ms
*/
#define SLAVE_LATENCY                   0                                           
/**< 
Slave latency. 
�ӻ�Ǳ���� (�ӻ�������ʱ�����Ļ�Ӧ��)
*/
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(2000, UNIT_10_MS)             
/**<  
Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. 
���ӳ�ʱ����(Ԥ��4��)

��ʱ����ʱ�䵥λ��10ms
*/
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  
/**< 
Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). 
��һ��֪ͨʱ��(���ӻ�ʼ֪ͨ) (����������Ϣ) (Ĭ��5��)
���ӻ�ʼ֪ͨ
*/
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) 
/**< 
Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). 
ÿ��֪ͨ��� �ڵ�һ��֪֮ͨ��  (����������Ϣ) (Ĭ��30��)
*/

#define MAX_CONN_PARAMS_UPDATE_COUNT    7 

/**< 
Number of attempts before giving up the connection parameter negotiation. 
�ڷ������Ӳ���Э��ǰ�����Դ���
*/

#define    START_STRING       "BLE_Start!\n"                                
/**< 
The string that will be sent over the UART when the application starts. 
��Ӧ�ó�������ʱ���ڷ��͵��ַ���
*/

#define DEAD_BEEF                       0xDEADBEEF                                  
/**< 
Value used as error code on stack dump, 
can be used to identify stack location on stack unwind. 
�ڶ�ջת����������������ֵ
��������ʶ���ջ�Ķ�ջλ��
*/

#define UART_TX_BUF_SIZE                20                                       
/**< 

UART TX buffer size. 
UART TX ���������
*/
#define UART_RX_BUF_SIZE                20                                        
/**< 
UART RX buffer size. 
UART RX ���������
*/
static ble_nus_t                        m_nus;      


/**< 
Structure to identify the Nordic UART Service. 
Nordic UART �ļ����ṹ
*/
static uint16_t                         m_conn_handle = BLE_CONN_HANDLE_INVALID;    
/**< 
Handle of the current connection. 
��ǰ���ӵĴ���
*/

static ble_uuid_t                       m_adv_uuids[] = {{BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}};  /**< Universally unique service identifier. ͳһ��Ψһ��ʶ��*/

uint8_t tab_dat[tab_dat_size];  //flash ��������
uint8_t MAC_ADD[6];  //MAC��ַ����
static void advertising_init(void);  //�㲥��ʼ��

#ifdef NB_Enable   //��ʹ��NBģ��ʱ��������ô˴������ұ���ʹ��RAM���������оƬ

uint8_t NB_Dat[NB_Dat_Size];
uint32_t NB_Dat_Len =0;   //NB Dat �ַ��� ��ǰʹ�õ���λ��
uint8_t NB_Run =0;

#endif

//��ʱ���Flash �� ʱ������
APP_TIMER_DEF (my_test_timer);  //�����µ������ʱ������  ע�������ʱ��

APP_TIMER_DEF(Uart_Close);

/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice. ��Э��ջ�н�������
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
 *  GAPһЩ���������ã������豸��������PPCP(��Χ�豸��ѡ���Ӳ���)��(�ֻ�����ĳ�������豸����Դ�Generic Access Service�п������õ���Щ����)
 *
 */
 
static void gap_params_init(void)
{
	
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    
    err_code = sd_ble_gap_device_name_set(&sec_mode,  //�����豸��
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

uint8_t TX_SUC =0;  //��ɱ�־

uint8_t BLE_TX_Run(uint8_t *s,uint8_t len)  //�������ݵ�BLE
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


/**@brief Function for handling the data from the Nordic UART Service.  //���ڴ�������Nortic UART ����� ����
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send 
 *          it to the UART module.
 *
 * �����������������Nordic UART BLE   //���� �����͵�UARTģ��
 * @param[in] p_nus    Nordic UART Service structure.  //Nordic UART  ����ģ��
 * @param[in] p_data   Data to be send to UART module. //�����ݷ��͵�UART ģ��
 * @param[in] length   Length of the data.   //���ݵĳ���
 */
/**@snippet [Handling the data received over BLE] */   //������յ�������


extern uint8_t  IN_BLE_Dat_SW;
static void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)  //BLE���ݴ���
{
//	uint32_t err_code;
//	uint8_t BLE_TX[20];	
#ifdef uart_start	  //ѡ���Ƿ�ͨ�����ڷ�������BLE������

//    for (uint32_t i = 0; i < length; i++)  //�������ڷ���
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

/**@snippet [Handling the data received over BLE] */ //������յ�������


/**@brief Function for initializing services that will be used by the application.
�����ʼ�������uart�Ĵ��ڷ�����Ҫ�ṩ��������ֵ���ṩ�ֻ��Ͱ����Լ����Ե�ͨѶ 
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


/**@brief Function for handling an event from the Connection Parameters Module.//�����Ӳ���ģ�鴦���¼��ĺ�����
 *
 * @details This function will be called for all events in the Connection Parameters Module //����������������Ӳ����е������¼������䴫�ݸ�Ӧ�ó���
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
  
void Dis_Conner()  //�Ͽ�����
{
	uint32_t err_code;
	
	err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
	APP_ERROR_CHECK(err_code);
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"\r\nDis_Conner!\r\n\r\n");
	
	#endif

}

/**@brief Function for handling errors from the Connection Parameters module.  //���ڴ������Ӳ���ģ���д���ĺ�����
 *
 * @param[in] nrf_error  Error code containing information about what went wrong. //�����������˴������Ϣ��
 */

static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for initializing the Connection Parameters module. //��ʼ�����Ӳ���ģ��ĺ�����
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


/**@brief Function for putting the chip into sleep mod //ʹоƬ����˯��״̬�Ĺ��ܡ�
e. 
 *
 * @note This function will not return. //����������᷵�ء�			`		
 */
static void sleep_mode_enter(void)
{

		uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);		

    // Prepare wakeup buttons. //׼�����Ѱ�ť
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset). //����ϵͳ�ر�ģʽ(����������᷵�أ����ѽ���������)
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
	
}


/**@brief Function for handling advertising events. //���ڴ���㲥�¼��Ĺ��ܡ�
 *
 * @details This function will be called for advertising events which are passed to the application. //������������������ڴ��ݹ㲥�¼���Ӧ�ó���
 *
 * @param[in] ble_adv_evt  Advertising event. //�㲥�¼�
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


/**@brief Function for the Application's S110 SoftDevice event handler. //Ӧ�ó����S110Э��ջ�¼��������Ĺ��ܡ�
 *
 *@param[in] p_ble_evt S110 SoftDevice event. //S110Э��ջ�¼���
 */


uint8_t BLE_LINK = 0;  //BLE����״̬ʵʱ����
uint8_t Ble_Lin_T =0;  //��������ʱ������  Ԥ��һ���������ӵ�ʱ�䲻����������
extern uint16_t data_len;  //BLE���ݳ���
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t   err_code;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:  //BLE�����ѽ���
            err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
            APP_ERROR_CHECK(err_code);
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
        BLE_LINK =true;    
				Ble_Lin_T =0;		//���Ӽ�ʱ����
				data_len =0;    //���Ӻ����BLE���ݻ���
				BLE_Data_Len =0;
			#ifdef uart_start
			SEGGER_RTT_printf(0,"Link_Y\r\n");
			#endif
//				input_speak(0);
//				input_speak(1);  //������
			//	input_speak(3);
				break;
				
        case BLE_GAP_EVT_DISCONNECTED: //BLE�����ѶϿ�
            err_code = bsp_indication_set(BSP_INDICATE_IDLE);
            APP_ERROR_CHECK(err_code);
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
 
				BLE_LINK = false;
				
				data_len =0;    //�Ͽ����� ���BLE���ݻ���
				BLE_Data_Len =0;
						
			#ifdef uart_start
				
			SEGGER_RTT_printf(0,"Link_N\r\n");
			
			#endif
	//			input_speak(0); //�ȷ��Ϳ�ָ��
//				input_speak(2); //δ����
			//	input_speak(3);
				break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported //��Բ�֧��
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored. //û�д洢ϵͳ����
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;
				case BLE_EVT_TX_COMPLETE:  //���ݷ������
					
					TX_SUC =0;
        default:
            // No implementation needed. //����Ҫʵ�֡�
            break;
    }
}


/**@brief Function for dispatching a S110 SoftDevice event to all modules with a S110 SoftDevice 
 *        event handler. //ʹ��S110Э��ջ������ģ�鷢��S110Э��ջ�¼��Ĺ����¼��������
 * 
 * @details This function is called from the S110 SoftDevice event interrupt handler after a S110 
 *          SoftDevice event has been received. //��������Ǵ�S110Э��ջ�Ѿ��յ�������¼��жϴ�������е��õģ�
 *
 * @param[in] p_ble_evt  S110 SoftDevice event. //S110 Э��ջ �¼�
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
����LFCLK(32.768k)��ʱ��Դ(Э��ջ��Ҫʹ��)����������Ϊ�ⲿ����flaseΪ��ʹ�õ��ȡ�
softdvice_ble_evt_handler_set(ble_evt_dispatch);ע���¼��ɷ����򣬻���1-Э��ջ����˵��
������BLE�յ��㲥���������󣬶Զ��豸���ݵȺ�ײ㴦������׸��ϲ�appһ���¼�������¼�
�����׹�����Э��ջ����SWI�жϣ����ն��ڲ����¼�������У�Ȼ�����app�е�SWI�жϡ�App�е�
SWI�жϻ�get�����е��¼��������ջ����ע���ble_evt_dispatch��������������ٽ��¼�������������
�Լ�ģ����¼��������������������ģ�鼰ģ���Լ�����Ȥ���¼����¹�ԭ�����1-Э��ջ������Ƶ����˵����
*/

static void ble_stack_init(void)
{
    uint32_t err_code;
    
    // Initialize SoftDevice. ��ʼϵͳʱ������ز���
   // SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, NULL);	
	
	 SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_8000MS_CALIBRATION,NULL); //�޸ĵ�Ƶʱ��Դ

	
	
    // Enable BLE stack. ʹ��Э��ջ ��ȡĬ������softdevice����
    ble_enable_params_t ble_enable_params;
    memset(&ble_enable_params, 0, sizeof(ble_enable_params));
#if (defined(S130) || defined(S132))
    ble_enable_params.gatts_enable_params.attr_tab_size   = BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;
#endif
    ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;
    err_code = sd_ble_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
    
    // Subscribe for BLE events ����BLE�¼� BLE�ص��ɷ�
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}


/******************************************

��ȡ���̵����͵���ʱ�ĸ�Ӧ������ֵ

���룺 ��

����� ��LEDOP_RC_VALUE �� LEDCL_RC_VALUE �������ȡ����ֵ

******************************************/

extern uint8_t SPI0_Start; //ȷ��SPI0 �Ƿ�����

uint32_t LEDOP_RC_VALUE; // ���� LED��ʱ�ĵ͹���Ѱ����ֵ
uint32_t LEDCL_RC_VALUE;  //���� LED�ر�ʱ�ĵ͹���Ѱ����ֵ


uint8_t SKY1311_RC_VALUE ()
{
	
		uint32_t rc_value;
	
		if(SPI0_Start==0) //��SPI0�ӿ�δ��ռ��ʱ(�������豸��ʹ��SPI0�ӿ�)
		{
			
			Spi_Flash_Init();  // ���� SPI0 �ӿ�
			
			irqClearAll(); 		
			sky1311Reset();
			rc_value =0;
			mpr121_Run();
			nrf_gpio_pin_set(LEDB);//����
	//		nrf_gpio_pin_set(LEDB1);//����
			nrf_delay_ms(500); //�ȴ��ȶ�
			
			rc_value = sky1311RCFreqCali();  //��ȡ ����ʱ�� RC��ֵ
			
		//	if(rc_value!=0)
			LEDOP_RC_VALUE = rc_value;
			
//			mpr121_init();
			TSM12_Init();
			nrf_gpio_pin_clear(LEDB);
//			nrf_gpio_pin_clear(LEDB1);
			nrf_gpio_pin_clear(LEDR);
			
			nrf_delay_ms(100);  //�ȴ��ȶ�
			
			rc_value = sky1311RCFreqCali();
			
			//if(rc_value!=0)
			LEDCL_RC_VALUE = rc_value;
			
			
		
			sky1311Reset();
			//Spi_Flash_Init();
			Spi_Flash_Sleep(); //  ������ɺ�ر� SPI0 �ӿ�
			
			if((LEDOP_RC_VALUE!=0)&&(LEDCL_RC_VALUE!=0))  //���������ݶ���Чʱ
			return 1; //�ɹ���ȡ RC ֵ �򷵻� 1
			
		}
	
	return 0;
	
}


uint32_t CL_LED_Cheak_RC_Value()   //������ �ر�LED ʱ�� RC ֵ
{
	uint32_t rc_value;
	
	if(SPI0_Start==0)
	{
	
	Spi_Flash_Init();  // ���� SPI0 �ӿ�
	
	irqClearAll(); 		
	sky1311Reset();
	
		nrf_gpio_pin_clear(LEDB);
//			nrf_gpio_pin_clear(LEDB1);
	nrf_gpio_pin_clear(LEDR);
	
	nrf_delay_ms(100);  //�ȴ��ȶ�
	
	rc_value = sky1311RCFreqCali();
	
	//if(rc_value!=0)
	LEDCL_RC_VALUE = rc_value;
	
	sky1311Reset();
	//Spi_Flash_Init();
	Spi_Flash_Sleep(); //  ������ɺ�ر� SPI0 �ӿ�
	
	if(rc_value!=0)		//��ֵ�Ϸ�ʱ �����µ�ֵ
	return rc_value;
}
	
	return LEDCL_RC_VALUE;   //  �����������쳣ʱ������ԭ����ֵ��Ϊ������׼

}


APP_TIMER_DEF (Read_My_Key);  //�����µ������ʱ������

APP_TIMER_DEF (Timer_1);  //�����µ������ʱ������ 1�뻽��һ�Σ����軽��


uint8_t Init_Key_Num =0;
static void  Read_Key (void *params);
//void read_a_key(); //���Ѻ��һ�ζ�ȡ
static void AC_advertising_init(void);
                                             
uint16_t rcValue;
boolean hasCardIn=false;
#define  CARDREMOVED        0
#define  HASCARDIN          1
#define  READCARDOVER       2
#define  WAITREMOVE         4
#define  GOTOSLEEP          8

uint8_t  cardStatus = CARDREMOVED;

uint8_t read_TypeA_Data[128];  //A���������򻺴�

//���� �ж϶�ȡ�� ���ͺŵĶԱ� (�ַ����һ�ֽ��ж�)

const uint8_t Admin_TypeA_Card =1;

const uint8_t User_TypeA_Card  =2;

const uint8_t Admin_TypeB_Card =3;

const uint8_t User_TypeB_Card  =4;


/*********************************************

read_TypeAB_SN ����

��������Ч�����ұ����б���������ʱ�����ֵ���Ǳ��ش�����SN�� 

���λ���ֽ� ����
 
 1 ʱ��ʾ���Ϊ ����Ա TypeA �� 
 2 ʱ��ʾ���Ϊ �û�   TypeA ��
 
 3 ʱ��ʾ���Ϊ ����Ա TypeB �� 
 4 ʱ��ʾ���Ϊ �û�   TypeB ��


**********************************************/

uint32_t read_TypeAB_SN; 

void Marnual_Add_Run(void);

/*��������Чʱ���ڱ��� ������Ч���� ����Чʱ�� ��һ�ֽ����ڱ�ʾ �Ƿ���Ч��
01 ��ʾ ��Ч ����A ��ʱ�� 
02 ��ʾ ��Ч �û�A ��ʱ�� 
03 ��ʾ ��Ч ����B ��ʱ�� 
04 ��ʾ ��Ч �û�B ��ʱ�� 

������Ч
*/
extern uint8_t TypeAB_Unix_T[9];

uint32_t TypeAB_Start_Unix;  //TypeAB_Unix_T ������ʼ������Ϣ �Ļ������ϱ��� 

uint32_t TypeAB_Stop_Unix;

uint8_t FD_Key =0;			//�����־

uint32_t Unix_T;  //��ǰоƬ��ʱ���

void Start_Key_Run(void);  //ֱ�ӻ��Ѽ��̵ĺ���
extern uint8_t Start_Manual_Add_Card;  

/**@brief Function for handling events from the BSP module. //���ڴ�������BSPģ����¼��Ĺ���
 *
 * @param[in]   event   Event generated by button press. //�ɰ�ť���²������¼�
 */
 
 void bsp_event_handler(bsp_event_t event)
 {
	 
	//bsp_event_handler_O();
	
 }
 
 
uint32_t Read_Card_Error_Num =0;				// ���ڼ�¼ ��λʱ����(My_test_timer_Time_Size)  ��������Ĵ��� 

uint8_t SKY1311_Error =0;		// ���� SKY1311 ����ʱ�Ļָ�����

extern u8 Op_Door_Buff[];				//���ڻ��濪�ż�¼���� ��д�뻺��

uint8_t DatCrd_AU;					//�������ݿ�����
 
//#define FC_OP_ Key  1   //����� ���� �궨�� ���κ� �����Ϊ����

void bsp_event_handler_O()
{
	  uint32_t err_code;
//  	uint32_t it2,i;
		
	if(!MPR121_RUN)  //������δ����ʱ����ռ��̼���
		MPR121_T =0;
	
		if(
			(!MPR121_RUN)&&
			(!nrf_gpio_pin_read(MPR_IRQ))
			&&(SPI0_Start==0)
			&&(Test_Ps_Mark==0)
			&&(Controllable_Power_State==0)
			&&(F_Print_Run_Flag==0)
			)			//δ�����ͻ��ֻ�е�ָ��ͷ�ر�ʱ��������
			{
					//	read_a_key();
								
					err_code = app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);			//��ʼ�����̵Ķ�ʱ������
					APP_ERROR_CHECK(err_code);
					

				
					if(SPI0_Start==0) //��SPI0�ر�ʱ����ɨ�跧ֵ���������Ѻ��ֵ
					{
						
						nrf_gpio_pin_set(LEDB);//����
						//nrf_gpio_pin_set(LEDB1);//����	
						
						#ifdef Key_By_MPR121
						mpr121_Run(); //ͬʱ�������̣����ڽ������̵糡����
						#endif
						
						#ifdef Key_By_TSM12c
						
						#ifdef uart_start
				
						SEGGER_RTT_printf(0,"TS12_Touch!\r\n");
						
						#endif
						
						nrf_gpio_pin_set(LEDB);//����
					//	nrf_gpio_pin_set(LEDB1);//����
						
					//	TSM12_read_tast();
						TSM12_Exit_Sleep_Mode();  //�˳�˯��ģʽ
						
						#endif
						
						Spi_Flash_Init();
						
						sky1311Reset();
						
						irqClearAll();              // ���SKY1311�������ж�
						checkCardInit(LEDOP_RC_VALUE);    // ���ý����Զ��쿨״̬
						cardStatus = GOTOSLEEP;
						
						nrf_delay_ms(100);  //Ԥ��500ms �ĵ糡����ʱ�䣬�ڼ�ĸ�Ӧֵ��Ч
						
						irqClearAll();
						//nrf_delay_ms(500);	
						mpr121_Run();	 //����������������ڼ����
										
						Spi_Flash_Sleep();
						
						#ifdef uart_start
							
						SEGGER_RTT_printf(0,"Check_Card_Start_LEDOP_Value!\r\n");  //���Կ�������ʱ��Ѱ����ֵ
							
						#endif
						
					}
					if(Devic_Static==false)  //���豸δ��ʱ
						{
							
							DEVICE_NAME[2] = Devic_Name_E;
									//GAPһЩ���������ã������豸��������PPCP(��Χ�豸��ѡ���Ӳ���)��
						//(��������ĳ�������豸����Դ�Gameric Access Service �п������õ���Щ����)
							sd_ble_gap_adv_stop();
							gap_params_init();  //����������	  
							//advertising_init();
							AC_advertising_init();  //���ù㲥���
							err_code = ble_advertising_start(BLE_ADV_MODE_FAST); //ͬʱ�����㲥
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
		
	if ((!nrf_gpio_pin_read(BUTTON_2))&&(Test_Ps_Mark==0))  //�жϰ���2 (������)״̬
	{
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Button_On\r\n");
		#endif 
		
		//TSM12_Exit_Sleep_Mode();    //ÿ�ΰ��°�ť����һ�δ�����

		Start_Key_Run();  //ֱ�ӻ��Ѽ���
		
		nrf_gpio_pin_clear(LEDB);
//		nrf_gpio_pin_clear(LEDB1);
		
		nrf_gpio_pin_set(LEDR);
		//input_speak(5);
		nrf_delay_ms(20);
		nrf_gpio_pin_clear(LEDR);
		nrf_gpio_pin_clear(LEDB);
		if(MPR121_RUN)  //�����̻���ʱ�������
		{
			nrf_gpio_pin_set(LEDB);
		}
//		nrf_gpio_pin_clear(LEDB1);
		
		
		if((!nrf_gpio_pin_read(BUTTON_2))&&(Init_Key_Num<1)&&(F_Print_Run_Flag==0))//&&(Controllable_Power_State==0))
		{
				Init_Key_Num =1;
				//input_speak(29); //������Чʱ��������
				err_code =app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//����500ms����Ķ�ʱ�����ʱ������
				APP_ERROR_CHECK(err_code);
		}
//		if(Init_Key_Num>=6) //�������γ�ʼ����ť���������Ϣ
//		{
//			Init_Key_Num =0;
//			Spi_Flash_Init();  //����SPI_FLASH ͨѶ

//			SPIFlash_Erase_Sector(0,0);  //Ԥ�����һ������0 ���������״̬�����Ϣ
//			CL_FlashWord_Num();  //���Flash Ŀ¼��Ϣ(�����������豸�������Ŀ¼��Ϣ)
//			CL_Flash_PassWord(); //�����豸ʱ ���Flash ������Ϣ only Clean
//			CL_Flash_OPEN_DOOR_NOTE();  //�����豸ʱ ���Flash ���ż�¼��Ϣ(only Clean)
//			SPI_Flash_Busy();		
//			Devic_static_read();
//			
//			
//			
//			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
//			Spi_Flash_Sleep();		//���Flash�����󣬶Ͽ��ⲿFlash����
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
//			ref_BLE_Adv();  //���ú�ˢ��BLE�㲥����
//								
//			#ifdef uart_start
//			SEGGER_RTT_printf(0,"BLE_KEY_Init!\r\n");
//			#endif
//			
//		}
		   // APP_ERROR_CHECK(err_code);		
		//nrf_gpio_pin_set(LEDR);//����
	}
	
	if(nrf_gpio_pin_read(SKY1311_IRQ_PIN)&&(SPI0_Start==0)&&(Test_Ps_Mark==0))//&&(F_Print_Run_Flag==0)&&(Controllable_Power_State==0))  //��SKY1311��⵽��ǿ�仯ʱ ��ʼ���Զ���
	{
		
	//	input_speak(2);
		
		
		#ifdef uart_start 
		SEGGER_RTT_printf(0,"Sky1311_IRQ!\r\n");
		#endif
		Spi_Flash_Init();  //����SPI0�ӿ�
			irq_sta = sky1311ReadReg(ADDR_IRQ_STA);  // ���жϼĴ��� ȷ���Ƿ�Ϊ�쿨�ж�
			if ((0==irq_sta) &&                      // û�м�⵽�жϼĴ�����ת��
				 (0!=SKY1311_IRQ_READ())&&(cardStatus==GOTOSLEEP)) // IRQ ���ű�ߵ�ƽ
			{
					#ifdef Key_By_MPR121
					if(MPR121_RUN) //�����̻���ʱ����ʱ����ͣ����ˢ��
					{
						
							app_timer_stop(Read_My_Key);
					
					}
					#endif
					
				checkCardDeinit();          // ֹͣ�Զ��쿨
				read_TypeA_Data[0] =0xff;
				Type_AB_UID[0] =0xff;
					if(
							 ((Ok == Type_A_Rrun()||Ok == Type_B_Rrun())||(Ok == Type_A_Null_Key_Rrun()))
						) //���A����ȡ
					{
						
						//ÿ�ζ�ȡ�꿨Ƭ������д��һ�ο����ݿ��ż�¼����
						
		//			extern u8 Op_Door_Buff[];				//���ڻ��濪�ż�¼���� ��д�뻺��
						
					for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff ����
						Op_Door_Buff[i1] =0;									
			
						if(Type_AB_UID[0]==0x01)		//������Ϊ A ��ʱ
							Op_Door_Buff[5] = 4;		//Type_A ������Ϊ 4				
						else if(Type_AB_UID[0]==0X02)	//������Ϊ B ��ʱ
							Op_Door_Buff[5] = 8;		//Type_B ������Ϊ8
						for(uint8_t i1=0;i1<Op_Door_Buff[5];i1++)
							Op_Door_Buff[i1+6]=Type_AB_UID[i1+1];
						
						
						//input_speak(1);
						cardStatus = WAITREMOVE;  //��Ƕ������
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Card_Read_Ok!\r\n");
						
						#endif
						
						//�ж� �������� A �� ���� B ��
						if(Type_AB_UID[0]==0x01)  	  //�������Ŀ�Ϊ A ��ʱ 
						{
								Devic_static_read();			//�ж����ݿ�ǰ�ȸ��°���Ϣ
							if((Data_Card_Run()==1)&&(TIME_STATE==true))  //�����ݿ�����ʱ
							{
								
									#ifdef uart_start
								
									SEGGER_RTT_printf(0,"Data_Card_Open_Door!\r\n");
								
									#endif
								
									nrf_gpio_pin_set(LEDB);
								//	nrf_gpio_pin_set(LEDB1);  //ֱ�ӿ���
									
									for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)
									{
										read_TypeA_Data[i] =0XFF;
									}
									Lock_Open();
									
									switch(DatCrd_AU)
									{
										case 1:  //����Ա
										ADD_Op_Door_Note(0,0x0103,Unix_T);  //д������ ��(�Զ��忨)���ŵĲ�����¼,
										break;
										
										case 2:   //�û�
											ADD_Op_Door_Note(0,0x0203,Unix_T);  //д������ ��(�Զ��忨)���ŵĲ�����¼,
										break;
										
										default :  //�������
											ADD_Op_Door_Note(0,0x03,Unix_T);  //д������ ��(�Զ��忨)���ŵĲ�����¼,
										break;
										
									}
									
									
									
									#ifdef uart_start
										
									SEGGER_RTT_printf(0,"TypeA_Card_Open_Door!\r\n");	
										
									#endif
										
									Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
									Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
									Spi_Flash_Init();
								
							}
							else
							{
							
							//���жϴ�UID�ű����Ƿ����
							read_TypeAB_SN = Need_TypeA_Data_SN(Type_AB_UID+1);  //ָ��ƫ��һλ ���� AB ��ָʾλ
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Type_AB_UID: %x\r\n",read_TypeAB_SN);
							
							#endif
							//�� �豸��ʱ ��ȡ����ЧSN ���ʾ�б������ݣ�׼������ �����ֶ�����ģʽ�£�Start_Manual_Add_Card>0 ʱ������������
							if((read_TypeAB_SN>0)&&(Start_Manual_Add_Card==0)&&((read_TypeAB_SN&0X1000000)||(read_TypeAB_SN&0x2000000))&&(Devic_Static==true))
							{
								
									Get_Time();
									
									if(TIME_STATE==true)  //��ʱ����Чʱ
									{
									
										Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���	
									
								  	if((TypeAB_Unix_T[0]==Admin_TypeA_Card)||(TypeAB_Unix_T[0]==User_TypeA_Card))  //��ʱ����ַ������� ����ԱA �� �� �û�A��ʱ
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
												
												
												
												nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�			//�ж��Ƿ���
												nrf_delay_ms(1); //�ȴ��ȶ�
												
												if((nrf_gpio_pin_read(5)==0)&&(TypeAB_Unix_T[0]==User_TypeA_Card))   //���ŷ���ʱ ��������
												{
													
											//		nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
													
													input_speak(17); //������ʾ�ѷ���
													nrf_delay_ms(1200);
													
												}					
												if(((Unix_T<TypeAB_Stop_Unix)&&(Unix_T>TypeAB_Start_Unix)&&(TypeAB_Unix_T[0]==Admin_TypeA_Card))||
													((Unix_T<TypeAB_Stop_Unix)&&(Unix_T>TypeAB_Start_Unix)&&(TypeAB_Unix_T[0]==User_TypeA_Card)&&(nrf_gpio_pin_read(5)==1)))  //ʱ����Ч �ҵ��û�����δ���������� ����
													{
														
														nrf_gpio_pin_set(LEDB);
	//										//			nrf_gpio_pin_set(LEDB1);  //ֱ�ӿ���
														
														Lock_Open();
														//д���Ӧ���Ϳ��ż�¼
														
														switch(TypeAB_Unix_T[0])
														{
															
															case Admin_TypeA_Card: 		//����ԱA��
																
															ADD_Op_Door_Note(read_TypeAB_SN,0x0102,Unix_T);  //д��A �����ŵĲ�����¼(�Զ��忨)  ���ֽ�01 ��ʾ����Ա
														
																break;
															
															case User_TypeA_Card:			//�û�A��
																
															ADD_Op_Door_Note(read_TypeAB_SN,0x0202,Unix_T);  //д��A �����ŵĲ�����¼(�Զ��忨)  ���ֽ�02 ��ʾ�û�
														
																break;
															
														}
														
														

														
														#ifdef uart_start
															
														SEGGER_RTT_printf(0,"TypeA_Card_Open_Door!\r\n");	
															
														#endif
															
														Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
														Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
														Spi_Flash_Init();
																				
													}
													else if((TypeAB_Unix_T[0]==Admin_TypeA_Card)||((TypeAB_Unix_T[0]==User_TypeA_Card)&&(nrf_gpio_pin_read(5)==1))) //ʱ����� ��δ��Ч ���û���ʱ��δ����
													{
															input_speak(16); //ʱ����Ч����ʾ��Ƭ����
															nrf_gpio_pin_clear(LEDB);
	//														nrf_gpio_pin_clear(LEDB1);
															nrf_gpio_pin_set(LEDR);
															nrf_delay_ms(800);
															nrf_gpio_pin_clear(LEDR);
															nrf_gpio_pin_set(LEDB);
													//		nrf_gpio_pin_set(LEDB1);
															
															Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
															Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
															Spi_Flash_Init();
	//														nrf_delay_ms(500);
	//														input_speak(10);  //������ʾ����ʧ�ܱ�ʾ�ڲ�ʱ�����
													}
													
													nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);   //����  ������־λ
													
												
												
											}
											else if(Start_Manual_Add_Card==0) //����Ƭ���ڣ�ʱ����Чʱ ��ʾ����
											{
														input_speak(10); //��Ƭ��Ч����ʾ����ʧ��
														nrf_gpio_pin_clear(LEDB);
//														nrf_gpio_pin_clear(LEDB1);
														nrf_gpio_pin_set(LEDR);
														nrf_delay_ms(800);
														nrf_gpio_pin_clear(LEDR);
														nrf_gpio_pin_set(LEDB);
											//			nrf_gpio_pin_set(LEDB1);
														
												
															Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
															Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
															Spi_Flash_Init();
//														nrf_delay_ms(500);
//														input_speak(10);  //������ʾ����ʧ�ܱ�ʾ�ڲ�ʱ�����
											}
								}
								else  //ϵͳʱ����Чʱ
								{
											input_speak(16); //ʱ����Ч����ʾ��Ƭ�ѹ���
											nrf_gpio_pin_clear(LEDB);
//											nrf_gpio_pin_clear(LEDB1);
											nrf_gpio_pin_set(LEDR);
											nrf_delay_ms(800);
											nrf_gpio_pin_clear(LEDR);
											nrf_gpio_pin_set(LEDB);
										//	nrf_gpio_pin_set(LEDB1);
											
											nrf_delay_ms(500);
											input_speak(10);  //������ʾ����ʧ�ܱ�ʾ�ڲ�ʱ�����
									
											Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
											Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
											Spi_Flash_Init();
														
								}
							}
							else if(Devic_Static==false)  //���豸δ��ʱ����ֱ���ü��ܹ��Ŀ�����
							{
									nrf_gpio_pin_set(LEDB);
							//		nrf_gpio_pin_set(LEDB1);  //ֱ�ӿ���
								
									Lock_Open();
												
									#ifdef uart_start
										
									SEGGER_RTT_printf(0,"TypeA_Card_Open_Door!\r\n");	
										
									#endif
								
									Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
									Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
									Spi_Flash_Init();
							}
							else if(Start_Manual_Add_Card==0)  // ����Ƭ�����Ҳ����ֶ�����״̬ʱ ��ʾ����
							{
											input_speak(10); //��Ƭ��Ч����ʾ����ʧ��
											nrf_gpio_pin_clear(LEDB);
//											nrf_gpio_pin_clear(LEDB1);
											nrf_gpio_pin_set(LEDR);
											nrf_delay_ms(800);
											nrf_gpio_pin_clear(LEDR);
											nrf_gpio_pin_set(LEDB);
										//	nrf_gpio_pin_set(LEDB1);
								
											Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
											Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
											Spi_Flash_Init();
							}
							else if(Start_Manual_Add_Card!=0)
							{
										Marnual_Add_Run();//��������

							}
						}
							
						}
						else if (Type_AB_UID[0]==0x02)  //�������Ŀ�Ϊ B ��ʱ
						{
							
							#ifdef uart_start
							
							SEGGER_RTT_printf(0,"Need TypeB_Card!\r\n");
							
							#endif
							
								Get_Time();
							
							if(Devic_Static==true)  //��ʱ����Чʱ
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
									Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���	
								
										if((TypeAB_Unix_T[0]==Admin_TypeB_Card)||(TypeAB_Unix_T[0]==User_TypeB_Card))  //��ʱ����ַ������� ����ԱB �� �� �û�B��ʱ
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
														
														nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�			//�ж��Ƿ���
														nrf_delay_ms(1); //�ȴ��ȶ�
														
														if((nrf_gpio_pin_read(5)==0)&&(TypeAB_Unix_T[0]==User_TypeB_Card))   //���ŷ���ʱ ��������
														{
															
													//		nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
															
															input_speak(17); //������ʾ�ѷ���
															nrf_delay_ms(1200);
															
														}
											
													if(((Unix_T<TypeAB_Stop_Unix)&&(Unix_T>TypeAB_Start_Unix)&&(TypeAB_Unix_T[0]==Admin_TypeB_Card))||
															((Unix_T<TypeAB_Stop_Unix)&&(Unix_T>TypeAB_Start_Unix)&&(TypeAB_Unix_T[0]==User_TypeB_Card)&&(nrf_gpio_pin_read(5)==1))
														)  //ʱ����Ч ����
														{
															nrf_gpio_pin_set(LEDB);
															//nrf_gpio_pin_set(LEDB1);  //ֱ�ӿ���
															
														Lock_Open();
														
														
														switch (TypeAB_Unix_T[0])
														{
															
															case Admin_TypeB_Card:
																	
															ADD_Op_Door_Note(read_TypeAB_SN,0x0102,Unix_T);  //д��B ��(�Զ��忨)���ŵĲ�����¼��λ 01 ��ʾ����Ա
														
																break;
															
															case User_TypeB_Card:
															
															ADD_Op_Door_Note(read_TypeAB_SN,0x0202,Unix_T);  //д��B ��(�Զ��忨)���ŵĲ�����¼��λ 02 ��ʾ�û�
														
																break;
															
															
															
														}
														
															
														#ifdef uart_start
															
														SEGGER_RTT_printf(0,"TypeA_Card_Open_Door!\r\n");	
															
														#endif
															
														Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
														Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
														Spi_Flash_Init();
																					
														}
														else if((TypeAB_Unix_T[0]==Admin_TypeB_Card)||((TypeAB_Unix_T[0]==User_TypeB_Card)&&(nrf_gpio_pin_read(5)==1))) //ʱ����� ��δ��Ч ���û������������δ����
														{
																input_speak(16); //ʱ����Ч����ʾ��Ƭ����
																nrf_gpio_pin_clear(LEDB);
//																nrf_gpio_pin_clear(LEDB1);
																nrf_gpio_pin_set(LEDR);
																nrf_delay_ms(800);
																nrf_gpio_pin_clear(LEDR);
																nrf_gpio_pin_set(LEDB);
														//		nrf_gpio_pin_set(LEDB1);
																
																Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
																Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
																Spi_Flash_Init();
		//														nrf_delay_ms(500);
		//														input_speak(10);  //������ʾ����ʧ�ܱ�ʾ�ڲ�ʱ�����
														}
														
													nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);   //����  ������־λ
												
											 
										 }
										 else
										 {
												input_speak(10); //����Ч����ʾ����ʧ��
												nrf_gpio_pin_clear(LEDB);
//												nrf_gpio_pin_clear(LEDB1);
												nrf_gpio_pin_set(LEDR);
												nrf_delay_ms(800);
												nrf_gpio_pin_clear(LEDR);
												nrf_gpio_pin_set(LEDB);
											//	nrf_gpio_pin_set(LEDB1);
											
												Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
												Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
												Spi_Flash_Init();
										 }
								
									}
									else if (TIME_STATE==false) //ʱ��ʧЧʱ
									{
										
										input_speak(16); //ʱ����Ч����ʾ��Ƭ�ѹ���
										nrf_gpio_pin_clear(LEDB);
//										nrf_gpio_pin_clear(LEDB1);
										nrf_gpio_pin_set(LEDR);
										nrf_delay_ms(800);
										nrf_gpio_pin_clear(LEDR);
										nrf_gpio_pin_set(LEDB);
								//		nrf_gpio_pin_set(LEDB1);
										
										nrf_delay_ms(500);
										input_speak(10);  //������ʾ����ʧ�ܱ�ʾ�ڲ�ʱ�����
								
										Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
										Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
										Spi_Flash_Init();
						
									}
									else if(Start_Manual_Add_Card==0)
									{
											input_speak(10); //����Ч����ʾ����ʧ��
											nrf_gpio_pin_clear(LEDB);
//											nrf_gpio_pin_clear(LEDB1);
											nrf_gpio_pin_set(LEDR);
											nrf_delay_ms(800);
											nrf_gpio_pin_clear(LEDR);
											nrf_gpio_pin_set(LEDB);
									//		nrf_gpio_pin_set(LEDB1);
										
											Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
											Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
											Spi_Flash_Init();
									}

							}
							
//							else if(Devic_Static==false)  //���豸δ��ʱ ��⵽���ֱ֤�ӿ���
//							{
//								Lock_Open();
//								Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
//								Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
//								Spi_Flash_Init();
//							}
							else 
							{
								
								input_speak(16); //ʱ����Ч����ʾ��Ƭ�ѹ���
								nrf_gpio_pin_clear(LEDB);
//							nrf_gpio_pin_clear(LEDB1);
								nrf_gpio_pin_set(LEDR);          
								nrf_delay_ms(800);
								nrf_gpio_pin_clear(LEDR);
								nrf_gpio_pin_set(LEDB);
								
//							nrf_gpio_pin_set(LEDB1);
//							nrf_delay_ms(500);
//							input_speak(10);  //������ʾ����ʧ�ܱ�ʾ�ڲ�ʱ�����
								
								Spi_Flash_Sleep();  //����ǰ���� SPI �����޷�����
								Start_Key_Run();  //ֱ�ӻ��Ѽ��̵ĺ���
								Spi_Flash_Init();
								  
							}
						
							Marnual_Add_Run();//��������

						}
						
		
						for(uint8_t i =0;i<sizeof(read_TypeA_Data);i++)				//�����ջ�������
						{
							
							read_TypeA_Data[i] =0XFF;
						
						}		
	
						
						app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//����500ms����Ķ�ʱ�����ʱ������
						
					}
					else //δ��ȡ����Ч��A/B��
					{
						
						cardStatus = WAITREMOVE; 
						app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//����500ms����Ķ�ʱ�����ʱ������
						
						if(MPR121_RUN==0)			//�ڴ���ʱ ����
						Read_Card_Error_Num++;			// ��ȡ��Ƭ������� +1
						else
							Read_Card_Error_Num =0;  //�ڼ��̴���ʱ���
						
						
					}
					#ifdef Key_By_MPR121
					if(MPR121_RUN)
					{
						app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	
					}
					#endif
					
			}
			else				//���жϲ��Ǽ쿨�жϣ���������жϣ�����sky1311���¿�ʼ���
			{
					irqClearAll();              // ���SKY1311�������ж�
					checkCardInit(rcValue);     // ���ý����Զ��쿨״̬
					Spi_Flash_Sleep();
			}
			
			Spi_Flash_Sleep();
	}
	else if((SPI0_Start==1)&&(nrf_gpio_pin_read(SKY1311_IRQ_PIN))&&(Test_Ps_Mark==0)) //��SPI0��ռ��ʱ��������ʱ�������⿨״̬
	{
		
		#ifdef uart_start
		SEGGER_RTT_printf(0,"Sky1311_SPI_Busy!!\r\n");
		#endif
		SKY1311_Error =1;
		
		cardStatus = CARDREMOVED; 
		
		app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//����500ms����Ķ�ʱ�����ʱ������
						
//		SKY1311_ENABLE();
//		irqClearAll();
//		SKY1311_DISABLE();
		
	}
	
		//MSR_121_read_key();  // ��ȡkeyֵ
	
	//����
	
	if((nrf_gpio_pin_read(13)==0)&&(Test_Ps_Mark==0))				//�����𱻴���
	{
		#ifdef FC_OP_Key
		
		FD_Key =1;			//�����־��1
		nrf_delay_ms(200);
		input_speak(20);				//��ʾ�Ƿ�����
	
		if(SPI0_Start==1)			// ��  SPI0 ������ʱ  
				{
						;		//����ʱ�������Ѱ�������Ӧ
				}
				else
				{
					Start_Key_Run();			//��SPI0 δ����ʱ��ֱ�ӻ��Ѽ���
				}
		#else
				Lock_Open();				//���𴥷�����
		#endif
	}
	
	#ifdef PS_Enable 
	
	if(nrf_gpio_pin_read(F_Print_IRQ)&&(Controllable_Power_State==0)&&(F_Print_Run_Flag==0)&&(CL_PS_Print_Now==0)&&(F_print_Enable==1))					//ָ�ƴ����ж�
	{
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"F_Print_IRQ_Input!\r\n");
		
		#endif
		Controllable_Power_No();		//��ָ��ģ���ϵ�
		
		if(Devic_Static==false)  //���豸δ��ʱ����ֱ����ָ�ƿ���(�����������κδ������ż�ʹָ�ƽ���˯��״̬)
		{
				nrf_gpio_pin_set(LEDB);
		//		nrf_gpio_pin_set(LEDB1);  //ֱ�ӿ���
			
				Lock_Open();
							
				#ifdef uart_start
					
				SEGGER_RTT_printf(0,"F_Print_Open_Door!\r\n");	
					
				#endif
			
				if(SPI0_Start==1)			// ��  SPI0 ������ʱ  
				{
						;		//����ʱ�������Ѱ�������Ӧ
				}
				else
				{
					Start_Key_Run();			//��SPI0 δ����ʱ��ֱ�ӻ��Ѽ���
				}
			F_Print_Run_Flag =1;			//��ʾָ��ͷ��������״̬
		}
		else
		{
			
			F_Print_Run_Flag =1;			//ʹָ��ͷ��������״̬
			
		}
	}
	
	#endif
	
}

uint8_t WAITEMOVE_Time =0;

void Start_Key_Run()  //ֱ�ӻ��Ѽ��̵ĺ���
{
		  uint32_t err_code;
  //	uint32_t it1,it2,i;
	if((!MPR121_RUN)&&(SPI0_Start==0))
	{
			
			err_code = app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);				//��ʼ�����̵Ķ�ʱ������
	    APP_ERROR_CHECK(err_code);
			
			if(SPI0_Start==0) //��SPI0�ر�ʱ����ɨ�跧ֵ���������Ѻ��ֵ
			{
			
				nrf_gpio_pin_set(LEDB);//����
				//nrf_gpio_pin_set(LEDB1);//����
				
				#ifdef key_By_MPR121
				mpr121_Run(); //ͬʱ�������̣����ڽ������̵糡����
				nrf_delay_ms(500);	
				mpr121_Run();	 //����������������ڼ����
				#endif
				
				#ifdef Key_By_TSM12c
				
				TSM12_Exit_Sleep_Mode();//	MPR121_RUN =1;
				
				
				
				#endif		
				
				
				Spi_Flash_Init();
	
				sky1311Reset();
				
				irqClearAll();              // ���SKY1311�������ж�
				checkCardInit(LEDOP_RC_VALUE);    // ���ý����Զ��쿨״̬
				cardStatus = GOTOSLEEP;
				
				nrf_delay_ms(1000);  //Ԥ��500ms �ĵ糡����ʱ�䣬�ڼ�ĸ�Ӧֵ��Ч
				
				irqClearAll();
				
				Spi_Flash_Sleep();
				
				#ifdef uart_start
					
				SEGGER_RTT_printf(0,"Check_Card_Start_LEDOP_Value!\r\n");  //���Կ�������ʱ��Ѱ����ֵ
				
				#endif
				
			}
			
			if(Devic_Static==false)  //���豸δ��ʱ
				{
					
					DEVICE_NAME[2] = Devic_Name_E;
							//GAPһЩ���������ã������豸��������PPCP(��Χ�豸��ѡ���Ӳ���)��
				//(��������ĳ�������豸����Դ�Gameric Access Service �п������õ���Щ����)
					sd_ble_gap_adv_stop();
					gap_params_init();  //����������	  
					//advertising_init();
					AC_advertising_init();  //���ù㲥���
					err_code = ble_advertising_start(BLE_ADV_MODE_FAST); //ͬʱ�����㲥
					APP_ERROR_CHECK(err_code);
						
				}
		}
	
	MPR121_T =0;  //��� �����Զ�������ʱ
		
		
}


uint32_t Wait_Card_Remove_Timeout =0; //�ȴ����뿪��ʱ
const uint8_t Wait_Card_Remove_Timeout_Value = 6;
uint16_t Wait_SPI0_Run_Time =0;		//����ȴ� SPI0 ʱ�䳬ʱ
const uint16_t SPI0_Run_Time_Out =50;

void SKY1311_State() //SKY1311״̬�л�
{
	
	switch (cardStatus)
	{
		case CARDREMOVED :  //û���ڳ��� ���¿�ʼ�͹���Ѱ��
			if(SPI0_Start==0)
			{
				Wait_SPI0_Run_Time =0;
				Spi_Flash_Init();
				sky1311Reset();
				irqClearAll();              // ���SKY1311�������ж�
				
				if(MPR121_RUN)
					rcValue = LEDOP_RC_VALUE;
				else
					rcValue = CL_LED_Cheak_RC_Value();
				
				
				checkCardInit(rcValue);    // ���ý����Զ��쿨״̬
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
				if(Wait_SPI0_Run_Time>=SPI0_Run_Time_Out)			//�ȴ� SPI0 ��ʱ
				{
					Wait_SPI0_Run_Time =0;		//��ձ�־λ
					Spi_Flash_Sleep();			//ǿ��ֹͣ SPI0 ����
				}
			}
			break;
		
		case GOTOSLEEP : //SKY1311����
			
			break;
		
		case HASCARDIN :  //���п��ڳ���
		
//				Wait_Card_Remove_Timeout++;
//		if(Wait_Card_Remove_Timeout>5)
//		{
//				Wait_Card_Remove_Timeout =0;
//				sky1311Reset();
//				if(SKY1311_RC_VALUE()==0);  //����ʼ��ʧ��ʱ
//				else 											//��ȡ���µ�ֵʱ
//				{
//						sky1311Reset();
////						cardStatus =WAITREMOVE;
//						Wait_Card_Remove_Timeout =0;
//					
//						irqClearAll();              // ���SKY1311�������ж�
//						checkCardInit(LEDCL_RC_VALUE);    // ���ý����Զ��쿨״̬
//						irqClearAll();
//						cardStatus = GOTOSLEEP;
//										
//				}
//		}
		
		break;
		
		case WAITREMOVE:  //�ȴ����뿪
			
			if(SPI0_Start==0)
			{
				if(MPR121_RUN) //�����̻���ʱ����ʱ����ͣ����ˢ��
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
					cardStatus = CARDREMOVED; //��ǿ��Ѿ��뿪���� CARDREMOVED ״̬
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
						cardStatus = CARDREMOVED; //��ǿ��Ѿ��뿪���� CARDREMOVED ״̬
						//rcValue = sky1311RCFreqCali();
						sky1311Reset();
						cardStatus =CARDREMOVED;
						Wait_Card_Remove_Timeout =0;
						
					}
				
//				else
//				{
//					Wait_Card_Remove_Timeout++;
//					if(Wait_Card_Remove_Timeout>Wait_Card_Remove_Timeout_Value)  //����ʱʱ �������»�ȡ
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
						
						app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);			//��ʼ�����̵Ķ�ʱ������
						
					}
						Spi_Flash_Sleep();
			}
			
		break;
	}
	
	//	NRF_WDT_Feed(); //���ﲻ��ι��
}


/*

���� �ֶ����뿨�ı�־λ ���ⲿ BLE_Data.c ��Manual_Add_Card ��λ ������
Timer_1_Handler  ��ʼ�ȴ��û�ˢ��¼��
��ֵΪ 1 ʱ Ϊ¼�� ����ԱA��
��ֵΪ 2 ʱ Ϊ¼�� ����ԱB��
��ֵΪ 3 ʱ Ϊ¼�� �û�A��
��ֵΪ 4 ʱ Ϊ¼�� �û�B��

*/
extern uint32_t Manual_Add_Card_Start_Unix; //�ֶ����뿨����ʼʱ��
extern uint32_t Manual_Add_Card_Stop_Unix; //�ֶ����뿨�Ľ���ʱ��

uint8_t Manual_Add_Card_Num =0;// ��¼����������ѯ����

uint8_t Type_AB_ID =0;   // Type AB ID ��ʾ����� Type AB ���� ID��

uint8_t Manual_Err_num =0;



void Marnual_Add_Run()
{

	//��������
	
	if ((Start_Manual_Add_Card>0)&&(cardStatus == WAITREMOVE)&& (Type_AB_UID[0]==0x01)&&(BLE_LINK==1))  //��������Ч A �� ������������ָ�� ��������ʱ
	{
			
		if(((Start_Manual_Add_Card&0x0F)==1)||((Start_Manual_Add_Card&0x0F)==2))  //���������Ա��ʱ �� A ����������д��
		{
			
					Spi_Flash_Init();
					
			  	Type_AB_ID =	Add_Flash_AD_TypeA_Data(Type_AB_UID+1,Manual_Add_Card_Start_Unix,Manual_Add_Card_Stop_Unix);
						//		 Card_ID = Add_Flash_AD_TypeA_Data (PassWord,Start_Unix,Stop_Unix);
					SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
					Spi_Flash_Sleep();
//					input_speak(9);

			if(Type_AB_ID!=0)
				{
					
					input_speak(9);
					Manual_Add_Card_ACK(0x01,Type_AB_ID,0X02);  //���ز����ɹ��ı���
					
				}
				else
				{
				
					input_speak(10);
					Manual_Add_Card_ACK(0x02,Type_AB_ID,0);  //���ز���ʧ�ܵı���
					
				}
						
				Start_Manual_Add_Card =0;
			
		}
		else if(((Start_Manual_Add_Card&0x0F)==3)||((Start_Manual_Add_Card&0x0F)==4)) //�������û���ʱ �� A ���û�������д��
		{
			
					Spi_Flash_Init();
					
			  	Type_AB_ID =	Add_Flash_US_TypeA_Data(Type_AB_UID+1,Manual_Add_Card_Start_Unix,Manual_Add_Card_Stop_Unix);
					
					SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
					Spi_Flash_Sleep();
					//input_speak(9);
			
				if(Type_AB_ID!=0)
				{
					
					input_speak(9);
					Manual_Add_Card_ACK(0x01,Type_AB_ID,0X02);  //���ز����ɹ��ı���
				
				}
				else
				{
				
					input_speak(10);
					Manual_Add_Card_ACK(0x02,Type_AB_ID,0);  //���ز���ʧ�ܵı���
					
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
					
					SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
					Spi_Flash_Sleep();
		//			input_speak(9);
				
					if(Type_AB_ID!=0)
					{
						input_speak(9); //�����ɹ�
						Manual_Add_Card_ACK(0x01,Type_AB_ID,0X03);  //���ز����ɹ��ı���
					}
					else
					{
						input_speak(10);  //����ʧ��
						Manual_Add_Card_ACK(0x02,Type_AB_ID,0X00);  //���ز���ʧ�ܵı���
					}
					Start_Manual_Add_Card =0;
				
			}
			else if(((Start_Manual_Add_Card&0x0F)==3)||((Start_Manual_Add_Card&0x0F)==4))
			{
				
					Spi_Flash_Init();
					
			  	Type_AB_ID =	Add_Flash_US_TypeB_Data(Type_AB_UID+1,Manual_Add_Card_Start_Unix,Manual_Add_Card_Stop_Unix);
					
					SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
					Spi_Flash_Sleep();
				//	input_speak(9);
				
					if(Type_AB_ID!=0)
					{
						input_speak(9);
						Manual_Add_Card_ACK(0x01,Type_AB_ID,0X04);  //���ز����ɹ��ı���
					}
					else
					{
						input_speak(10);
						Manual_Add_Card_ACK(0x02,Type_AB_ID,0X00);  //���ز���ʧ�ܵı���
					}
					Start_Manual_Add_Card =0;
				
			}
	}
	else if(Manual_Err_num>5)  //�����Դ����������
	{
		Manual_Err_num =0;
		Start_Manual_Add_Card =0;
		if(BLE_LINK==1)
		{
		
			if(BLE_LINK==1)
				Manual_Add_Card_ACK(0x02,0,0);  //���ز���ʧ�ܵı���
			
		}
	}
	else
	{
			Manual_Err_num ++;
	}
}




uint8_t Init_Key_T =0;  //���ڻ��帴λ����
uint8_t Lock_Open_EN =0;    //���ſ���



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
	
	if(Init_Key_Num>6) //�������γ�ʼ����ť��ѯȷ��������ô��ʱ�䣬�������Ϣ
	{
		 
		Init_Key_Num =0;
		Spi_Flash_Init();  //����SPI_FLASH ͨѶ

		SPIFlash_Erase_Sector(0,0);  //Ԥ�����һ������0 ���������״̬�����Ϣ
		CL_FlashWord_Num();  //���Flash Ŀ¼��Ϣ(�����������豸�������Ŀ¼��Ϣ)
		CL_Flash_PassWord(); //�����豸ʱ ���Flash ������Ϣ only Clean
		CL_Flash_OPEN_DOOR_NOTE();  //�����豸ʱ ���Flash ���ż�¼��Ϣ(only Clean)
		
		Erase_AD_TypeA_Sector();
		Erase_US_TypeA_Sector();
		Erase_AD_TypeB_Sector();
		Erase_US_TypeB_Sector();
		
		SPI_Flash_Busy();		
		Devic_static_read();
		
		Lock_Open_EN =0;					//��ȡ�Ƿ���δ�غ�����������

		
		SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
		Spi_Flash_Sleep();		//���Flash�����󣬶Ͽ��ⲿFlash����
	
		nrf_gpio_pin_clear(LEDB);
//		nrf_gpio_pin_clear(LEDB1);
		
		nrf_gpio_pin_set(LEDR);
		//input_speak(5);
		nrf_delay_ms(500);
		nrf_gpio_pin_clear(LEDR);
		nrf_gpio_pin_clear(LEDB);
//		nrf_gpio_pin_clear(LEDB1);
		
		if(MPR121_RUN)  //�����̻���ʱ�������
		{
			nrf_gpio_pin_set(LEDB);
		}
		input_speak(4);
		ref_BLE_Adv();  //���ú�ˢ��BLE�㲥����
							
		#ifdef uart_start
		SEGGER_RTT_printf(0,"BLE_KEY_Init!\r\n");
		#endif
		
	}
	
	SKY1311_State();  //SKY1311_״̬��
	
	if(
	(cardStatus==GOTOSLEEP)&&
	(Init_Key_Num==0))
	{
		
			app_timer_stop(Timer_1);
	
	}

}


/**@brief   Function for handling app_uart events. //���ڴ��� app_uart���¼��ĺ���
 *
 * @details This function will receive a single character from the app_uart module and append it to 
 *          a string. The string will be be sent over BLE when the last character received was a 
 *          'new line' i.e '\n' (hex 0x0D) or if the string has reached a length of 
 *          @ref NUS_MAX_DATA_LENGTH.
�����������app_uart ģ���赼һ���ַ��������丽�ӵ�һ���ַ���������һ���ַ��������յ�ʱ���ַ������ᱻ���ͳ�ȥ��'�µ���' Ҳ���� \n(hex 0x0D) �����ַ����Ѿ��ﵽ�� NUS_MAX_DATA_LENGTH.

 */
/**@snippet [Handling the data received over UART] */ //����ͨ��UART���յ����ݡ�  �����¼�


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
//                err_code = ble_nus_string_send(&m_nus, data_array, index);  //BLE���ݷ���
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


/**@snippet [Handling the data received over UART] */ //����ͨ��UART���յ����ݡ�

 
/**@brief  Function for initializing the UART module. //��ʼ��UARTģ��Ĺ��ܡ�
 */
/**@snippet [UART Initialization] */
/*
��ʼ��uart��������������ţ��Ƿ�ر����ء�һ��ʹ�ùٷ����ӵ�ʱ��Ҫ�Ƚ����عص���HWFCΪFalse��Ȼ��
��uart�Ľ����жϣ���uartģ����жϹ��ܣ��Լ��������ȼ���
��������simple_uart_config�����ã��ú������������ź�ʹ��uart����uart�Ľ��պͷ��͹��ܡ�
*/

uint8_t UART_RUN =0;   //���ڱ�ʾ���ڿ���״̬

 void uart_init(void)
{
	
	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
	
	nrf_delay_ms(10);   //�ȴ������ȶ�
	
	UART_RUN =1;
	
//    uint32_t                     err_code;
//    const app_uart_comm_params_t comm_params =
//    {
//        RX_PIN_NUMBER,
//        TX_PIN_NUMBER,
//        RTS_PIN_NUMBER,
//        CTS_PIN_NUMBER,
//        APP_UART_FLOW_CONTROL_DISABLED,  //������ ENABLED ʹ��
//        false,
//        UART_BAUDRATE_BAUDRATE_Baud115200
//    };

		
			
//    APP_UART_FIFO_INIT( &comm_params,
//                       UART_RX_BUF_SIZE,
//                       UART_TX_BUF_SIZE,
//                       uart_event_handle,  //���ڻص�
//                       APP_IRQ_PRIORITY_LOW,
//                       err_code);
//    APP_ERROR_CHECK(err_code);
	//simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
}
/**@snippet [UART Initialization] */ //UART��ʼ��


//�������ڣ�����ʱ���ر�

void Uart_Start_TC()
{
	
	uint32_t err_code;
	
	
	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
	nrf_delay_ms(10);
	UART_RUN = 1;
	
	err_code = app_timer_start(Uart_Close,APP_TIMER_TICKS(Uart_Close_Time_Size,APP_TIMER_PRESCALER),NULL);   //ע���Զ��رմ��ڵĶ�ʱ������
	
	APP_ERROR_CHECK(err_code);
	
}

void close_uart()
{
		UART_RUN =0;
		NRF_UART0->ENABLE    				= (UART_ENABLE_ENABLE_Disabled<<UART_ENABLE_ENABLE_Pos);  //�رմ���

}



/**@brief Function for initializing the Advertising functionality.
���ù㲥�����Լ�ɨ����Ӧ����
 */
static void advertising_init(void)
{
	 
		uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;

    // Build advertising data struct to pass into @ref ble_advertising_init. �ܹ��㲥���� �Դ��䵽 ble_advertising_init
    memset(&advdata, 0, sizeof(advdata));  //��սṹ��
     advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = false;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;  //һ��ɷ���ģʽ

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

    // Build advertising data struct to pass into @ref ble_advertising_init. �ܹ��㲥���� �Դ��䵽 ble_advertising_init
    memset(&advdata, 0, sizeof(advdata));  //��սṹ��
     advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = false;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;  //һ��ɷ���ģʽ

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

    // Build advertising data struct to pass into @ref ble_advertising_init. �ܹ��㲥���� �Դ��䵽 ble_advertising_init
  memset(&advdata, 0, sizeof(advdata));  //��սṹ��
	advdata.name_type          = BLE_ADVDATA_FULL_NAME;
	advdata.include_appearance = false;
	advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;  //һ��ɷ���ģʽ

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
 * ��ʼ��������LED�Ĺ���
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */

static void buttons_leds_init(bool * p_erase_bonds)
{
		bsp_event_t startup_event;
	//�����͵Ƴ�ʼ��������APP_TIMER_TICKS(100,APP_TIMER_PRESCALER)������ʱ�� bsp_event_handler: �ص�����  
	//�ڶ�ӦIO��������������ص� bsp_event_handler
	
		uint32_t err_code = bsp_init(BSP_INIT_BUTTONS,//BSP_INIT_LED | 
																 APP_TIMER_TICKS(10, APP_TIMER_PRESCALER), 
																 bsp_event_handler);
		APP_ERROR_CHECK(err_code);
	
		err_code = bsp_btn_ble_init(NULL, &startup_event);
		APP_ERROR_CHECK(err_code);

		*p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}

/**@brief Function for placing the application in low power state while waiting for events. //�ڵȴ��¼�ʱ��Ӧ�ó������ڵ͹���״̬�Ĺ��ܡ�
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

//�޸�MAC��ַ
 void mac_set(void)
{
	ble_gap_addr_t addr;
	uint32_t err_code = sd_ble_gap_address_get(&addr);  //��ȡMAC��ַ
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

uint8_t MPR121_RUN; //״̬
uint16_t MPR121_T;   //��ʱ��

uint32_t ADC_Value;
uint32_t VCC_Value,VCC_Value_H,VCC_Value_L1,VCC_Value_L2; //������
#define VDD_Value 120000  //оƬADC�ο���ѹ*10000�� ��������
#define RES_d  10 //������ѹ����

uint16_t ADC_Div;  //ADC��С�ֱ���
uint16_t MPR_Del;  //�ڵ͹���ģʽ�£���ʱ�������̰� 
uint16_t MPR_Value;  //��ȡ��ֵ

uint8_t MOT_RL =0;
uint16_t CL_Init_Key_Num =0;

extern uint32_t Once_Act_Run;  //һ�������ʹ�ü�¼��������24Сʱδʹ��һ������ʱ�����һ������
extern uint32_t Once_Act_Num;  //һ����������
uint32_t Once_Act_CL =0;  //һ�����붨ʱ���

uint32_t Board_Unix =0xffffffff;  //����Unixʱ�䱸��
uint64_t Board_Time;  //��������ʱ�� ����д��
extern uint8_t PassWord_Error_Num;  //��¼��������������

extern uint8_t Update_Time_Flag;

uint8_t updat_Time_T =0;  //���ڼ�¼ʱ���������

extern uint16_t time1[6];

uint16_t Err_PassWord_CL;  //��ʱ��մ����������

/*
����ʱ�仺�� 
time1[0] year
time1[1] mon
time1[2] day
time1[3] hour
time1[4] min
time1[5] sec

*/

uint16_t LOCK_OPEN_Time=0;  //��¼б����������ʱ��
uint16_t Lock_OPEN_Spean =0;  //���ż��


uint32_t  Auto_Card_Init =0;  //�Զ���ʱ����ˢ��Ѱ��

static void  my_timer_handler (void *params)  //my_timerͨ�ö�ʱ���ص�
{
	
	if(PassWord_Error_Num>0)
	{
		Err_PassWord_CL++;
		if(Err_PassWord_CL>((5*60)/(My_test_timer_Time_Size/1000)))  //��������Ӻ����
		{
			Err_PassWord_CL =0;
			PassWord_Error_Num =0;
		}
	}
	else
		{
			Err_PassWord_CL =0;
		}
		
		
	if (Once_Act_Num>0)  //����һ������ʱ
	{
		
		Once_Act_CL++;
		if(Once_Act_Run==1)  //����ʱ�ڼ�ʹ�ù�һ������ʱ���¿�ʼ��ʱ���ﵽ24δʹ��һ����������ձ���һ������Ĺ���
			Once_Act_CL =0;
		
		if(Once_Act_CL>((25*3600)/(My_test_timer_Time_Size/1000))) //Ԥ��25Сʱ��δʹ��һ�����룬�Զ����
		{
			//������25Сʱδʹ��һ�����룬�����һ�������¼
			Once_Act_CL =0;
			Spi_Flash_Init();  //����SPIͨѶ
			nrf_delay_ms(50); //�ȴ��ȶ�
			
			Ev_Cat_Dat(0x02,0);  //����̬�����������
			Once_Act_Num =0;
			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();	
		}
	}
	else 
	{
		Once_Act_CL =0;
	}
	
	if(MPR121_RUN==0)    //����ʱ�Զ�����ˢ��оƬ����Ѱ��״̬��ֹ����ֹͣѰ��
	{
		Auto_Card_Init++;
		if(Auto_Card_Init>((6*3600)/(My_test_timer_Time_Size/1000)))   //���������6Сʱ���Զ�����Ѱ��
		{
			if(SPI0_Start==0) //��SPI0�ر�ʱ����ɨ�跧ֵ�������������ֵ
				{
		
					Spi_Flash_Init();
			
					sky1311Reset();
					
					irqClearAll();              // ���SKY1311�������ж�
					checkCardInit(LEDCL_RC_VALUE);    // ���ý����Զ��쿨״̬
					cardStatus = GOTOSLEEP;

					Spi_Flash_Sleep();
										
					#ifdef uart_start
						
					SEGGER_RTT_printf(0,"Check_Card_Start_LEDCL_Value!\r\n");  //���Կ�������ʱ��Ѱ����ֵ
						
					#endif
					Auto_Card_Init =0;
					
				}
		}
	}
	else
		Auto_Card_Init =0;
	
	if(MPR121_RUN)		//���豸���������ģʽ
	{
		MPR_Del =0;
	}
	else
	{
		MPR121_T =0;
		MPR_Del ++;
		if (MPR_Del>450)  //ÿ2����Сʱ����ˢ��һ�μ���
		{
				
				MPR_Del =0;
//			mpr121_Run_I();
//			mpr121_init_I();  //�͹��Ķ�ʱ���ü���
//			mpr121_init();
				TSM12_Init();
			
		}
	}
	
	if(Init_Key_Num>2)  //��ʼ����ť�������
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
//				printf("MPR121_8BASVALUE: %d\r\n",MPR_Value); //��ȡ����ֵ	
//				MPR_Value =	read_i2c(I2C_ADD,0x15);
//				MPR_Value <<=8;
//				MPR_Value |=	read_i2c(I2C_ADD,0x14);
//				MPR_Value>>=2;   //ֻ��ȡ���8�ֽ�
//				printf("MPR121_8VALUE: %d\r\n",MPR_Value); //��ȡ��ֵ
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
			Dis_Conner();  //������ʱ�䳬��1���ӣ��Զ��Ͽ�����
		}
	}
	else
	{
		
		Ble_Lin_T =0;
	
	}
	
		if(Lock_Open_EN==0)   //������������������ʱ
		{
			
			if(nrf_gpio_pin_read(7)==0) //�����࿨ס��δ�غ�ʱ��
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
			if(Read_Card_Error_Num<Read_Card_Error_Num_Full)			// ������������  Read_Card_Error_Num_Full ����ʱ�������ȡ
			{
				
				Read_Card_Error_Num =0;
			
			}
			else						// �������������� Read_Card_Error_Num_Full    ʱ����Ϊ�����쳣״̬����Ҫͨ���ض���ʽʹϵͳ�ָ�ԭ״
			{
				
					if((SPI0_Start==0)&&(MPR121_RUN==0))		// ��SPI ������δ����ʱ
					{
						
						while(SKY1311_RC_VALUE()==0)nrf_delay_ms(1000);

						Spi_Flash_Init();
						sky1311Reset();         
						irqClearAll();              // ���SKY1311�������ж�
						checkCardInit(LEDCL_RC_VALUE);    // ���ý����Զ��쿨״̬
						cardStatus = GOTOSLEEP;
						
						SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
//	
						Spi_Flash_Sleep ();
						
						Read_Card_Error_Num =0;			//��ɺ����
						
					}
				
			}
	}	
	
	
	
	
	
	//TSM12_read_tast();
	#ifdef uart_start
	
		SEGGER_RTT_printf(0,"heart!\r\n");
	
	#endif
//	if(Update_Time_Flag==1) //���������¹�ʱ��ʱ
//	{
//		Update_Time_Flag =0;
//		updat_Time_T =0;  		//���ø���ʱ��
//		Get_Time();			//У׼���ر���ʱ��
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
//		)  //������ʱ����Чʱ  У�԰���ʱ�� (���ھ����ܱ��ϰ���ʱ������Ȼ���)
//	{
//		
//		Get_Time();
//		Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���			
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
//				Board_Unix+=(My_test_timer_Time_Size*updat_Time_T/1000);  //����ʱ�Ӽ��������ʱ��ʱ��
//				Unix_Time_to_Time(Board_Unix);  //������ʱ��תΪֱ����ʾ����
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
//				Board_Unix+=(My_test_timer_Time_Size*updat_Time_T/1000);  //����ʱ�Ӽ��������ʱ��ʱ��
//				Unix_Time_to_Time(Board_Unix);  //������ʱ��תΪֱ����ʾ����
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
//		updat_Time_T =0;  //������ ��ʱ����
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

uint8_t Key_Num_Tab[Key_Num_Tab_Len]; //�������뻺��(���Key_Num_Tab_Len��)
uint8_t Key_Num_n =0;  //���������ֵ����
uint8_t PassWord_Error_Num =0;  //��¼��������������
uint32_t PassWord_Error_Time =0;  //����������ʱ
#define PassWord_Error_Num_Max 6

void Pass_Word_Run(void); //�������봦����


void read_a_key() //���Ѻ��һ�ζ�ȡ
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
		case 10:  //������*��ʱ ����������������ֵ��
						//	mpr121_Run();
			Key_Num_n =0;
			for(i =0;i<Key_Num_Tab_Len;i++) //��ʼ����ʱ�������뻺��
			{
				Key_Num_Tab[i] = 0xFF;  //������뻺��
			}
			break;

		case 11: //������0��ʱ (��ͨ0����)
			
			Key_Num_Tab[Key_Num_n++] = 0;
		
		break;
		
		case 12: //������#��ʱ (������ɣ���ʼ����)
				//			mpr121_Run();
	//	Pass_Word_Run();  //���Ѻ����벻��#������
			break;
		
		default:  //������ֵ
			
			if(key_num<10) //��ֵ����ʱ
			{
				Key_Num_Tab[Key_Num_n++] = key_num;
			}
			
		break;
	}
	
}


uint32_t Manual_Add_Card_T =0;  //��¼��������ʱ��

static void  Read_Key (void *params)  //���Ѻ�ʼɨ�����  �������    Read_Key_Time_Size
{
	uint8_t i,key_num;
	
		if(Start_Manual_Add_Card>0)  //������������״̬ʱ ��ʱ�������������־λ �Խ�����������״̬
		{
			
				Manual_Add_Card_T++;
			
				if(Manual_Add_Card_T>(10*1000/Read_Key_Time_Size))
				{
					
					Start_Manual_Add_Card =0;
					Manual_Add_Card_T =0;
					if(BLE_LINK==1)  //��BLE��������ʱ��������������ʧ�ܱ���
					{
							
							input_speak(10);
							Manual_Add_Card_ACK(0x02,Type_AB_ID,0X00);  //���ز���ʧ�ܵı���
							
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


			
			app_timer_stop(Read_My_Key);  //��ֹͣ ��ʱ��
			
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
//				CN_TSM12C_TH =0;				//��մ�����־
//				
//			}
			#endif
			

			
			switch (key_num)
			{
				case 10:  //������*��ʱ ����������������ֵ�� 
								//	mpr121_Run();
//				if(Key_Num_n>2)
//				{
					input_speak(49);  //�������뱻��յ�����
					nrf_gpio_pin_set(LEDR);
					nrf_delay_ms(300);
					nrf_gpio_pin_clear(LEDR);				
//				}
//				else
//				input_speak(29);
//				
				Key_Num_n =0;
					for(i =0;i<Key_Num_Tab_Len;i++) //��ʼ����ʱ�������뻺��
					{
						Key_Num_Tab[i] = 0xFF;  //������뻺��
					}
					break;
				case 11: //������0��ʱ (��ͨ0����)
					
					Key_Num_Tab[Key_Num_n++] = 0;
					break;
				case 12: //������#��ʱ (������ɣ���ʼ����)
		//	mpr121_Run();
				
				Pass_Word_Run();
					break;
				default:  //������ֵ
					if(key_num<10) //��ֵ����ʱ
					{
						Key_Num_Tab[Key_Num_n++] = key_num;
					}
				break;
			}
			
			if(Key_Num_n>=Key_Num_Tab_Len)   //����50����ջ���
			{
					Key_Num_n =0;
					for(i =0;i<Key_Num_Tab_Len;i++) //��ʼ����ʱ�������뻺��
					{
						Key_Num_Tab[i] = 0xFF;  //������뻺��
					}
					input_speak(49);
			}
			
			
			MPR_Del =0;
			MPR121_T++;
			
		if(Controllable_Power_State!=0)			//����ָ��ͷ��ֱ�ӹرռ���
			{
				MPR121_T  =Devic_Static_True_Mpr121_T_Size;
			}
			
				if(
					(((Devic_Static==true)&&(MPR121_T>=Devic_Static_True_Mpr121_T_Size)) || 
						((Devic_Static==false)&&(MPR121_T>=Devic_Static_False_Mpr121_T_Size)&&(BLE_LINK==false))		
					)&&
					(
						Start_Manual_Add_Card==0)//��һ��ʱ����δ�������̣��Ҳ�����������״̬�رռ��̵�
					)						
				{
				app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	 //�ر�ǰ�ȴ򿪶�ʱ��
				MPR121_T =0;
				//PassWord_Error_Num =0; //�����ص�ʱ�������������
				#ifdef Key_By_MPR121
				mpr121_init();
				#endif
				
				#ifdef Key_By_TSM12c
				
				TSM12_Init();
				
				#endif
				
				for(i =0;i<Key_Num_Tab_Len;i++)
				{
					
					Key_Num_Tab[i] = 0xFF;  //������뻺��
				
				}

				Key_Num_n =0;
				app_timer_stop(Read_My_Key);
				nrf_gpio_pin_clear(LEDB);
//				nrf_gpio_pin_clear(LEDB1);
				nrf_gpio_pin_clear(LEDR);
				
				if(SPI0_Start==0) //��SPI0�ر�ʱ����ɨ�跧ֵ�������������ֵ
					{
			
						Spi_Flash_Init();
				
						sky1311Reset();
						
						irqClearAll();              // ���SKY1311�������ж�
						checkCardInit(LEDCL_RC_VALUE);    // ���ý����Զ��쿨״̬
						cardStatus = GOTOSLEEP;

						Spi_Flash_Sleep();
						
						#ifdef uart_start
							
						SEGGER_RTT_printf(0,"Check_Card_Start_LEDCL_Value!\r\n");  //���Կ�������ʱ��Ѱ����ֵ
							
						#endif
						
					}
				
				if(Devic_Static==false)
				{
				//	DEVICE_NAME[2] = 'I';
					//GAPһЩ���������ã������豸��������PPCP(��Χ�豸��ѡ���Ӳ���)��
				//(��������ĳ�������豸����Դ�Gameric Access Service �п������õ���Щ����)
			//	gap_params_init();  //����������	  
		//		advertising_init();
					ref_BLE_Adv();  //����BLE�㲥
			
				}
				Bsp_Set();   //�ڼ������н���ʱ���� BSP
			}
			else  //�����رռ��̵�ʱ�����¿������̶�ʱ��
			app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	

			
		}
		else
			
		if(PassWord_Error_Num>=PassWord_Error_Num_Max)  //����ﵽ����ʱ ��ʼ��������ʱ
		{
			
			app_timer_stop(Read_My_Key);  //��ֹͣ ��ʱ��
				nrf_gpio_pin_clear(LEDB);
//				nrf_gpio_pin_clear(LEDB1);
				nrf_gpio_pin_clear(LEDR);
			
//				sky1311Reset();
//					
//				irqClearAll();              // ���SKY1311�������ж�
//				
//				checkCardInit(LEDCL_RC_VALUE);    // ���ý����Զ��쿨״̬
//				
			//	cardStatus = GOTOSLEEP;
					
			
				PassWord_Error_Time ++;
			//	mpr121_init();
			TSM12_Init();
			if(PassWord_Error_Time>=60000)  //ʱ�䵽��������
			{
				TSM12_Exit_Sleep_Mode();
				PassWord_Error_Time =0;
				PassWord_Error_Num  =0;  //�����������¼
				//input_speak(24);
				
				nrf_gpio_pin_clear(LEDR); //�ָ�����״̬
				nrf_gpio_pin_set(LEDB);
//				nrf_gpio_pin_set(LEDB1);
			}
			if(!nrf_gpio_pin_read(MPR_IRQ)) //���а�������ʱ
			{
				
				#ifdef Key_By_MPR121
				key_num = MPR121_Key_Run(); //α��ȡ
				#endif
				
				#ifdef Key_By_TSM12c
				key_num = TSM12_read_key_Num();
				#endif
//				nrf_gpio_pin_set(LEDB1);
				nrf_delay_ms(100);
				
			}
			app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	

		}
		else  //����δ������رն�ʱ�����(һ�㲻�����)
		{
			
				MPR121_T =0;
//			PassWord_Error_Num =0;

				mpr121_init();
				for(i =0;i<Key_Num_Tab_Len;i++)
				{
					Key_Num_Tab[i] = 0xFF;  //������뻺��
				}
				Key_Num_n =0;
				app_timer_stop(Read_My_Key);
				nrf_gpio_pin_clear(LEDB);
//				nrf_gpio_pin_clear(LEDB1);
				nrf_gpio_pin_clear(LEDR);
				
		}
}





uint16_t PassWord_Con(uint32_t PassWord_Num);  //Ѱ��������ֵ��Ӧ������ ����ֵΪ�ҵ�������ţ�0��ʾû��
//extern uint16_t Note_Num_1;
extern uint8_t  Act_PassWord_rese;   //���������������뱾������������ʱ����1��֮�󽫲��������������ж�̬��������

//#define  History_Time_Act_PassWord_Run  1				//������ʷ��̬�������ι��� �����Ƕ�̬���룩

void Pass_Word_Run() //���봦��
{
	
//	uint8_t psd; //�����Ƿ���� 1Ϊ����0Ϊ��ȷ
	

	uint32_t Unix_Start;  //��ͬ�������ʼʱ��
	uint32_t Unix_Stop;  //��ͬ����Ľ���ʱ��
	
	uint32_t PassWord_Num;
	
//	uint32_t len;
//	uint32_t Sector,Page;
	
	uint32_t i,is;
	uint32_t it;
	
	i =0xFFFFFFFF;

	Spi_Flash_Init();  //����SPI_FLASH ͨѶ
	
	Devic_static_read();  //(��ȡ�豸��״̬)
	
	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	Spi_Flash_Sleep();
	
	if(Devic_Static==false)//δ��ʱ����Ĭ�����뿪��
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
			Key_Num_n =0;  //����ֵС��3λ�����д����������
		}
		
	}
	else if(Devic_Static==true) //�豸�Ѱ�ʱ����ʼ����У��
	{
		
		Get_Time(); //��ȡ����ʱ��
		
		if(Key_Num_n>3) //������С���볤��
		{
			
			if((TIME_STATE==true)&&(Key_Num_n>5)) //��ʱ����Чʱ ������ֵ��������λ��
			{
			//	SEC,MIN,HOUR,DAY,MON,YEAR;
				
				Unix_T = Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���			
				
				#ifdef uart_start
				SEGGER_RTT_printf(0,"%d,\r\n",Unix_T);			
				#endif
				Spi_Flash_Init();  //����SPI_FLASH ͨѶ
				
				Devic_static_read();			//�ж�����ǰ�ȸ��°���Ϣ

				PassWord_Num = Get_Cat_Dat (0x01); //��ȡ��ǰ�洢����������
//				len = PassWord_Num * PassWord_Len; //��ȡ����洢�����ܳ���
				
//				Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��������
//				SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ���������ݳ��ȵ��������� �ŵ����ػ���
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
				
				it =0;		//it ���ڱ�ʾ���������Ƿ���Ч
				it = PassWord_Con(PassWord_Num);  //Ѱ��������ֵ��Ӧ������ ����ֵΪ�ҵ�������ţ�0��ʾû�� 
				
				if((it==0)&&(Act_PassWord_Verify(Key_Num_n,Unix_T)==0)&&(Per_Act_PassWord_Verify(Key_Num_n)==0)&&
					(More_Act_PassWord_Verify(Key_Num_n,Unix_T)==0)
				)  //û����ͬ��������,�������뱾����Ч��̬���벻��ʱ
				{
					
					#ifdef uart_start  //��û����ͬ����ʱ����ʼ�����ж�����������Ƿ�Ϊ��̬����
					
					SEGGER_RTT_printf(0,"Start_Act_PassWord!\r\n"); 
					
					#endif
					
					Get_Time(); //��ȡ����ʱ��
					Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���			
					
					Unix_T /=3600;  //��ʱ���ת����Сʱ��ʽ
					
					Spi_Flash_Init();  //����SPI_FLASH ͨѶ ׼����ʼ��̬����Ĵ���
					
					i =0;
					
					if((Key_Num_n>=6)&&
							(Key_Num_n<=9)&&(Act_PassWord_rese!=1))  //�����볤�ȴ��ڵ��ڶ�̬������С����С�ڵ��ڶ�̬������󳤶� �Ҳ��뱾����������
						
					i = des_psw(Key_Num_Tab,Key_Num_n,Unix_T); //���㵱ǰ�����Ƿ�����ǺϷ��Ķ�̬����
					
					else if(Act_PassWord_rese==1)
					{
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Error!\r\n Act_PassWord_Rase!\r\n");    //��ʾ��������
						
						#endif
						
						i =0;
						
					}
					
					Act_PassWord_rese =0; //��ɺ����
						
					#ifdef uart_start
					
						SEGGER_RTT_printf(0,"Des_Psw_T: %x\r\n",i);
					
					#endif
						
						if(i>0) //��������Чʱ //��i �ڵĶ�̬������Чʱ����ȡ�������ڽ�����������
						{
							
								is =i;
								is>>=24;
								is&=0xff; //�����̬������Ч������ȡ��Чʱ��
								i&=0x00ffffff;  //�� i ��Ϊ��������Чʱ��
//							if(i>is)
//							i-= is;  //��i ����ɵ�ǰʱ��Ķ���Сʱ�Ժ���
//							else
//							i =0;
							
							#ifdef uart_start
							
								SEGGER_RTT_printf(0,"is: %x!\r\n",is);
							
							#endif
							
							
						}
						
						nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�			//�ж��Ƿ���
						nrf_delay_ms(1); //�ȴ��ȶ�
			
						if((nrf_gpio_pin_read(5)==0)&&(i>0))   //���ŷ���ʱ��ʾ���ѷ���  ����ʱ������̬���뿪��
						{
							
							input_speak(17); //������ʾ�ѷ���
							nrf_delay_ms(1200);
							
						}
				
						if((i>0)&&(nrf_gpio_pin_read(5)==1))  //��i>0ʱ��˵������δ���ڣ���ʼ�ж���������
						{
							
							if((i==6)&&(Key_Num_n==8))  //���뵱����Ϊ8,��Чʱ��Ϊ6ʱ������Ϊһ������(���ź���Ҫд�뱾�ز��ɶ���ʹ��)
							{   
								
								if(Once_Act_PassWord_Verify()==1)			//���뿪�ż�¼���� �ڸú�����
								{								
									ADD_Op_Door_Note(0,0x00,Unix_T*3600);  //д�붯̬���뿪�ż�¼  (�����Ϊ0����Ϊ��̬����)
									Lock_Open();  //һ������һ������Ч���ʲ�����
									PassWord_Error_Num =0;  //���Ź��� ���ô����������
									
								}
								else
								{
									i =0;
								}
							}
							else if((i==8)&&(Key_Num_n==8)) //�����볤��Ϊ8����Чʱ��Ϊ8ʱ������Ϊ��������(���ź󱣴治����ڣ�ֱ����һ����Ч��̬�����滻)
							{
//								if(History_Time_Act_PassWord()==1)
//								{
										//������ �������� ���뿪�ż�¼����
										for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff ����
											Op_Door_Buff[i1] =0;									
								
											Op_Door_Buff[5] = 8;		//�������볤�ȶ���6
			
											Op_Door_Buff[6] = Key_Num_Tab[0];		//���������һλ
											Op_Door_Buff[7] = Key_Num_Tab[1];	//��������ڶ�λ
											Op_Door_Buff[8] = Key_Num_Tab[2];	//�����������λ
											Op_Door_Buff[9] = Key_Num_Tab[3];		//�����������λ
											Op_Door_Buff[10] = Key_Num_Tab[4];		//�����������λ
											Op_Door_Buff[11] = Key_Num_Tab[5];	//�����������λ
											Op_Door_Buff[12] = Key_Num_Tab[6];	//�����������λ
											Op_Door_Buff[13] = Key_Num_Tab[7];	//��������ڰ�λ
													
								
								
									Per_Act_PassWord_Save(0x02,Key_Num_Tab,Key_Num_n,0xFFFFFFFF);  //�������� ������������
									ADD_Op_Door_Note(0,0x00,Unix_T*3600);  //д�붯̬���뿪�ż�¼  (�����Ϊ0����Ϊ��̬����)
									Lock_Open();  //�������벻����ڣ��ʲ�����
									PassWord_Error_Num =0;  //���Ź��� ���ô����������
									
//								}
//								
//								else
//								{
//									is =0;
//									i =0;
//								}
							}
							else if(i>is)  	//�����벻�����������һ������ʱ������Ϊ��ͨʱЧ��̬���룬�����������滻������
							{
								
								if(
										(is<i)
								#ifdef History_Time_Act_PassWord_Run
								&&(History_Time_Act_PassWord()==1)  //���� ��̬�����滻ʧЧ����
								#endif
								
									)  //ͨ������ʱ���ж������Ƿ����
								{
									//������д��Flash
								
								#ifdef History_Time_Act_PassWord_Run
									
								Del_User_Key();  //д��ǰɾ���û�Կ��
								
								#endif
									//д�� ���ż�¼����
									//��ʱЧ��̬�������� ���뿪�ż�¼����
								for(uint8_t i1 =0;i1<Open_Note_Num_Len;i1++)   //Open_Note_Num_Len = Op_Door_Buff ����
									Op_Door_Buff[i1] =0;									
									
									Op_Door_Buff[5] = Key_Num_n;		//���볤�� =  Key_Num_n
									
									for(uint8_t i1=0;i1<Key_Num_n;i1++)		//��ʱЧ��̬������������
									{
										 Op_Door_Buff[6+i1] = Key_Num_Tab[i1];
									}
										
									Act_PassWord_Save(0x02,Key_Num_Tab,Key_Num_n,(Unix_T-is+i)*3600);  //��������(���ͣ���ͨ���룬��Դ����������,���ȣ�������������(6-9),����ʱ�䣺��ǰʱ���ȥ��Сʱǰ���ɼ�����Чʱ��) 
									ADD_Op_Door_Note(0,0x00,Unix_T*3600);  //д�붯̬���뿪�ż�¼  (�����Ϊ0����Ϊ��̬����)
									//�µĶ�̬���뿪�ź���ձ����û�����
									Lock_Open();  //��̬����Ϸ���δ����ʱ��������
									PassWord_Error_Num =0;  //���Ź��� ���ô����������
									
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
						
						nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);			//��λ���� IO
						
						if (i==0)
						{
						
							#ifdef uart_start
							SEGGER_RTT_printf(0,"PassWord_Error:%d\r\n",it); //�������
							#endif
							
							nrf_gpio_pin_clear(LEDB);
//							nrf_gpio_pin_clear(LEDB1);
							
							nrf_gpio_pin_set(LEDR);
							
							PassWord_Error_Num++;
							if(PassWord_Error_Num==(PassWord_Error_Num_Max-2))
							{
								
								input_speak(28); //����������ﵽһ��ֵ����ʾ�ٴ������������
								
							}
							else
								input_speak(5); //������Ч����ʾ�������
							
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
					SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
					Spi_Flash_Sleep();	

				}
				else
				{
					
						if(it!=0)  //�������Ǳ�������ʱ
						{
						#ifdef uart_start
						SEGGER_RTT_printf(0,"PassWord_Con:%d\r\n",it);	//������ȷ����ʼ�ȶ�ʱ��				
						#endif
						it-=1;
						if(it>=A_Sector_PassWord_Num)
						{
							it-=A_Sector_PassWord_Num;  //��ʱ����������Ϊ�ڶ�����
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
							
							if(tab_dat[it*PassWord_Len+2]==User_PassWord)				//���������û�����ʱ�ж��Ƿ�Ϊ����
							{
									nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLUP);	//�ȿ��������ӿ�			//�ж��Ƿ���
									nrf_delay_ms(1); //�ȴ��ȶ�
										
								if((nrf_gpio_pin_read(5)==0))   //���ŷ���ʱ��ʾ���ѷ���
								{
									
									
									input_speak(17); //������ʾ�ѷ���
									nrf_delay_ms(1200);
									
								}
								else			//δ����������
								{
																//д��һ�����ż�¼
									//	Spi_Flash_Init();
			//							NRF_WDT_Feed();  //ι��
										
										Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���			
										ADD_Op_Door_Note(tab_dat[it*PassWord_Len+17],0x0200,Unix_T);   //�û����� ��λΪ02
										Lock_Open();
				//						read_Soctor(500);
				//						NRF_WDT_Feed();
										PassWord_Error_Num =0;
								}
								nrf_gpio_cfg_input(5,NRF_GPIO_PIN_PULLDOWN);
							}
							else if(tab_dat[it*PassWord_Len+2]==Admin_PassWord)
							{
							//д��һ�����ż�¼
						//	Spi_Flash_Init();
//							NRF_WDT_Feed();  //ι��
							Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���			
							ADD_Op_Door_Note(tab_dat[it*PassWord_Len+17],0x0100,Unix_T);			//����Ա���� ��λΪ01
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
								input_speak(15); //ʱ����Ч����ʾ�����ѹ���
							
							nrf_gpio_pin_clear(LEDB);
//							nrf_gpio_pin_clear(LEDB1);
							nrf_gpio_pin_set(LEDR);
							nrf_delay_ms(800);
							nrf_gpio_pin_clear(LEDR);
							nrf_gpio_pin_set(LEDB);
//							nrf_gpio_pin_set(LEDB1);
						}
					}
					else if(it==0) //�����벻�Ǳ�������ʱ,ѡ��̬���뿪��
					{
//							NRF_WDT_Feed();  //ι��
							Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���			
							ADD_Op_Door_Note(0,0x00,Unix_T);
							Lock_Open();
							PassWord_Error_Num =0;  //���Ź��� ���ô����������
					}
						
					
					SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
					Spi_Flash_Sleep();		

				}
				
//				SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
//				Spi_Flash_Sleep();
			}
			else if(TIME_STATE==false)
			{
					input_speak(15); //ʱ����Ч����ʾ�����ѹ���
					nrf_gpio_pin_clear(LEDB);
//					nrf_gpio_pin_clear(LEDB1);
					nrf_gpio_pin_set(LEDR);
					nrf_delay_ms(800);
					nrf_gpio_pin_clear(LEDR);
					nrf_gpio_pin_set(LEDB);
//					nrf_gpio_pin_set(LEDB1);
					
					nrf_delay_ms(500);
					input_speak(10);  //������ʾ����ʧ�ܱ�ʾ�ڲ�ʱ�����
			}
			else
			{
					#ifdef uart_start
					SEGGER_RTT_printf(0,"PassWord_Len_Short!\r\n"); //�������
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
						input_speak(5); //ʱ����Ч����ʾ�������
					nrf_delay_ms(800);
					nrf_gpio_pin_clear(LEDR);
					nrf_gpio_pin_set(LEDB);
//					nrf_gpio_pin_set(LEDB1);
				
			}
			Key_Num_n =0;// ����������
//			//�������ʱ�Ĵ��� 
//			nrf_gpio_pin_clear(LEDB);
//			nrf_gpio_pin_set(LEDR);
//			input_speak(5);
//			nrf_delay_ms(800);
//			nrf_gpio_pin_clear(LEDR);
//			nrf_gpio_pin_set(LEDB);
		}
		else
		{
			input_speak(49);  //��������������С����ʼֵʱ��# ��ʾ������յ���ʾ��
			Key_Num_n =0; //����ֵС����λ�����д����������
		
		}
	}
	
	#ifdef uart_start
	
	SEGGER_RTT_printf(0,"i: %x!\r\n",i);
	SEGGER_RTT_printf(0,"PassWord_Run_End!\r\n");
	
	#endif
	
	
	
//		SpiFlash_PowerDown();   //������в���ʱ��ʹ�����͹���ģʽ 1ua
//		Spi_Flash_Sleep();
}



/**********************************************
Ѱ��������ֵ��Ӧ������ ����ֵΪ�ҵ�������ţ�0��ʾû��

���أ� �����ֵ������ͬ�������
********************************************/
uint8_t PassWord_Con_User =0;				//Ѱ��������Ϊ�û������־

//#define Admin_PassWord  0x12		//�������Ա����ǰ׺
//#define User_PassWord  0x22		//�����û�����ǰ׺

extern u8 Op_Door_Buff[];				//���ڻ��濪�ż�¼���� ��д�뻺��


uint16_t PassWord_Con(uint32_t PassWord_Num)  //Ѱ��������ֵ��Ӧ������ ����ֵΪ�ҵ�������ţ�0��ʾû��
{
	
	uint32_t i,i1;
	 
	//uint32_t PassWord_Num;
	uint32_t len;
	uint32_t Block,Page;
//	uint32_t it;
	
	PassWord_Con_User =0;			//����û������־
	
	len = PassWord_Num*PassWord_Len;
	if(PassWord_Num<=A_Sector_PassWord_Num) //����������һ������������
	{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��������
		SPIFlash_Read_Tab(tab_dat,Block,Page,len);  //��ȡ���������ݳ��ȵ��������� �ŵ����ػ���		
		
		#ifdef uart_start 
		SEGGER_RTT_printf(0,"Start_Con_1!\r\n");	
		#endif
		
		for(i1 =0;i1<(Key_Num_n-5);i1++)
		{
			for(i =0;i<PassWord_Num;i++) //��ѯ����
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
				
				for(uint32_t i2 =0;i2<Open_Note_Num_Len;i2++)   //Open_Note_Num_Len = Op_Door_Buff ����
					Op_Door_Buff[i2] =0;
					
					Op_Door_Buff[5] = 6;		//�������볤�ȶ���6
					
					Op_Door_Buff[6] = tab_dat[i*PassWord_Len+11];		//���������һλ
					Op_Door_Buff[7] = tab_dat[i*PassWord_Len+12];		//��������ڶ�λ
					Op_Door_Buff[8] = tab_dat[i*PassWord_Len+13];		//�����������λ
					Op_Door_Buff[9] = tab_dat[i*PassWord_Len+14];		//�����������λ
					Op_Door_Buff[10] = tab_dat[i*PassWord_Len+15];		//�����������λ
					Op_Door_Buff[11] = tab_dat[i*PassWord_Len+16];		//�����������λ
					
					return i+1; //����һ���������
				}
			}
		}
	}
	else if(PassWord_Num<=(2*A_Sector_PassWord_Num))
	{
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr1); //��ȡ��������
		SPIFlash_Read_Tab(tab_dat,Block,Page,A_Sector_PassWord_Num*PassWord_Len);  //��ȡ���������ݳ��ȵ��������� �ŵ����ػ���		
		
		#ifdef uart_start 
		SEGGER_RTT_printf(0,"Start_Con_1!\r\n");	
		#endif
		
		for(i1 =0;i1<(Key_Num_n-5);i1++)
		{
			for(i =0;i<A_Sector_PassWord_Num;i++) //��ѯ����
			{
				if(
					(tab_dat[i*PassWord_Len+11]==Key_Num_Tab[i1])&&
					(tab_dat[i*PassWord_Len+12]==Key_Num_Tab[i1+1])&&
					(tab_dat[i*PassWord_Len+13]==Key_Num_Tab[i1+2])&&
					(tab_dat[i*PassWord_Len+14]==Key_Num_Tab[i1+3])&&
					(tab_dat[i*PassWord_Len+15]==Key_Num_Tab[i1+4])&&
					(tab_dat[i*PassWord_Len+16]==Key_Num_Tab[i1+5])
					)
				//������д�� ���ż�¼ ����
				{
					
				for(uint32_t i2 =0;i2<Open_Note_Num_Len;i2++)   //Open_Note_Num_Len = Op_Door_Buff ����
					Op_Door_Buff[i2] =0;
					
					Op_Door_Buff[5] = 6;		//�������볤�ȶ���6
					
					Op_Door_Buff[6] = tab_dat[i*PassWord_Len+11];		//���������һλ
					Op_Door_Buff[7] = tab_dat[i*PassWord_Len+12];		//��������ڶ�λ
					Op_Door_Buff[8] = tab_dat[i*PassWord_Len+13];		//�����������λ
					Op_Door_Buff[9] = tab_dat[i*PassWord_Len+14];		//�����������λ
					Op_Door_Buff[10] = tab_dat[i*PassWord_Len+15];		//�����������λ
					Op_Door_Buff[11] = tab_dat[i*PassWord_Len+16];		//�����������λ
					
					return i+1; //����һ���������
					
				}
			}
			  //�ڶ�����
		} //�Ա�һ��������

		#ifdef uart_start		
		SEGGER_RTT_printf(0,"Start_Con_2!\r\n");
		#endif
		
		Sector_to_Block_Page(&Block,&Page,PassWord_Secotr2); //��ȡ��������
		SPIFlash_Read_Tab(tab_dat,Block,Page,len-A_Sector_PassWord_Num*PassWord_Len);  //��ȡ���������ݳ��ȵ��������� �ŵ����ػ���		
		
		for(i1 =0;i1<(Key_Num_n-5);i1++)
		{
			for(i =0;i<=(PassWord_Num-A_Sector_PassWord_Num);i++) //��ȡ��������
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
				for(uint32_t i2 =0;i2<Open_Note_Num_Len;i2++)   //Open_Note_Num_Len = Op_Door_Buff ����
					Op_Door_Buff[i2] =0;
					
					Op_Door_Buff[5] = 6;		//�������볤�ȶ���6
					
					Op_Door_Buff[6] = tab_dat[i*PassWord_Len+11];		//���������һλ
					Op_Door_Buff[7] = tab_dat[i*PassWord_Len+12];		//��������ڶ�λ
					Op_Door_Buff[8] = tab_dat[i*PassWord_Len+13];		//�����������λ
					Op_Door_Buff[9] = tab_dat[i*PassWord_Len+14];		//�����������λ
					Op_Door_Buff[10] = tab_dat[i*PassWord_Len+15];		//�����������λ
					Op_Door_Buff[11] = tab_dat[i*PassWord_Len+16];		//�����������λ
					
					i+=A_Sector_PassWord_Num;
					return i+1; //����һ���������
				}
			}
		}
	}
	return 0;
}

void ref_BLE_Adv()  //ˢ��BLE�㲥����
{
		uint8_t i,it1,it2;
//	   uint32_t err_code;
		if(Devic_Static==false)  //���豸δ��ʱ�������㲥��
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
			}  //��MAC��ַ����㲥��
	//		sd_ble_gap_adv_stop(); // ����ͬʱ�رչ㲥
			gap_params_init();  //����������	  
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
//				sd_ble_gap_adv_stop(); // ����ͬʱ�رչ㲥
				gap_params_init();  //����������	  
				advertising_init();	
//				err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
//				APP_ERROR_CHECK(err_code);
		}
}


//APP_TIMER_DEF (my_Op_Door_timer);  //�����µ������ʱ������  ע�������ʱ��

//uint8_t opdt =0;
//uint32_t TX_Note_T =0;  //���ż�¼����

//static void  my_Op_Door_Out(void *params)
//{
//	
//	uint32_t err_code;
//		
////		err_code =app_timer_stop(my_Op_Door_timer);
////		APP_ERROR_CHECK(err_code);
//	if((Note_Num_1>0)&&(Note_Num_1<=Open_Note_Size))  //TX_SUC  z��ʾ�������
//		{

//					TX_Door_note(); //��ȡ���ż�¼ָ�� һ����������
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
//		if((Note_Num_1>0)&&(Note_Num_1<=Open_Note_Size))  //����Ҫ�����ϴ����ż�¼ʱ�������������ż�¼�ϴ���ʱ��
//		{
////				err_code = app_timer_start(my_Op_Door_timer,APP_TIMER_TICKS(my_Op_Door_timer_Size,APP_TIMER_PRESCALER),NULL);			//ע���µ������ʱ������
////					APP_ERROR_CHECK(err_code);
//		}
//		else  //����ֹͣ�ϴ�
//		{
//					Note_Num_1 =0;
//			
//					
//					opdt =0;
//			
////				if(MPR121_RUN==1)//�����̻���ʱ��������ɨ�趨ʱ������
////				{
////							err_code =app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	  //�������̶�ʱ������
////							APP_ERROR_CHECK(err_code);
////				}
//			
//			err_code =app_timer_stop(my_Op_Door_timer);
//			APP_ERROR_CHECK(err_code);
//			
//		}
//	
//		//NRF_WDT_Feed();  //���ﲻ��ι��
//		#ifdef uart_start  //������Ҫ�������ڼ���	
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
//	irqClearAll();              // ���SKY1311�������ж�
////		checkCardInit(rcValue);     // ���ý����Զ��쿨״̬
////		cardStatus = GOTOSLEEP;
//	
//	#ifdef uart_start
//	SEGGER_RTT_printf(0,"\r\n rcValue=%x\r\n",rcValue);
//	#endif
//		irqClearAll();              // ���SKY1311�������ж�
//		checkCardInit(rcValue);     // ���ý����Զ��쿨״̬
//		cardStatus = GOTOSLEEP;

	Spi_Flash_Sleep();
}

void sky1311_Read()
{
		if(cardStatus == CARDREMOVED)           // û���ڳ��ڣ��Ѿ����ߣ�
			{
					irqClearAll();              // ���SKY1311�������ж�
					checkCardInit(rcValue);     // ���ý����Զ��쿨״̬
					cardStatus = GOTOSLEEP;
			//	system_ActiveHalt();        // MCU ��������״̬
				
//					while(sky1311_In_Read());
//					hasCardIn =1;
			}
		 else if(cardStatus == GOTOSLEEP)
			{
				
				
					/* ������MCU ���ߺ��� */
					if(hasCardIn)           // �쿨�жϲ����������˱��
					{
							 irq_sta = sky1311ReadReg(ADDR_IRQ_STA);  // ���жϼĴ���
							if((0==irq_sta) &&                      // û���κ��жϲ�������
								 (0!=SKY1311_IRQ_READ())) // IRQ ���ű�ߵ�ƽ
							{
									checkCardDeinit();          // ֹͣ�Զ��쿨
									cardStatus = HASCARDIN;     // ���ñ�ǣ��п�����
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
		else if(cardStatus == HASCARDIN)  //�п��ڳ���
		{
				 if((Ok == Type_A_Rrun())||(Ok == Type_B_Rrun()))          // ���Զ������ж��Ƿ�Type A ����
					{
							cardStatus = WAITREMOVE; // ��Ƕ������
					}
					else        // û�ж�����
					{
							rcValue = sky1311RCFreqCali(); // ����У׼RCƵ��
							cardStatus = CARDREMOVED;
					}
		}
	else if(cardStatus == WAITREMOVE)       // �ȴ����뿪
		{
				DelayMS(300);                   // ��΢��ʱһ�£�����̫Ƶ���ж��Ƿ��뿪
				if(checkCardRemove(rcValue))    // �жϿ��Ƿ��뿪
				{
						cardStatus = CARDREMOVED;       // ��ǿ��Ѿ��뿪
//					rcValue = sky1311RCFreqCali(); // ����У׼RCƵ��
						DelayMS(300);
						sky1311Reset();
				}
	//		cardStatus = CARDREMOVED;       // ��ǿ��Ѿ��뿪
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
		irqClearAll();              // ���SKY1311�������ж�
//		checkCardInit(rcValue);     // ���ý����Զ��쿨״̬
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
			 if(cardStatus == CARDREMOVED)           // û���ڳ��ڣ��Ѿ����ߣ�
				{
						irqClearAll();              // ���SKY1311�������ж�
						checkCardInit(rcValue);     // ���ý����Զ��쿨״̬
						cardStatus = GOTOSLEEP;
					//	system_ActiveHalt();        // MCU ��������״̬
					
						while(sky1311_In_Read());
						hasCardIn =1;
				}
			 else if(cardStatus == GOTOSLEEP)
				{
						/* ������MCU ���ߺ��� */
						if(hasCardIn)           // �쿨�жϲ����������˱��
						{
								 irq_sta = sky1311ReadReg(ADDR_IRQ_STA);  // ���жϼĴ���
								if((0==irq_sta) &&                      // û���κ��жϲ�������
									 (0!=SKY1311_IRQ_READ())) // IRQ ���ű�ߵ�ƽ
								{
										checkCardDeinit();          // ֹͣ�Զ��쿨
										cardStatus = HASCARDIN;     // ���ñ�ǣ��п�����
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
			else if(cardStatus == HASCARDIN)  //�п��ڳ���
			{
					 if((Ok == Type_A_Rrun()) || (Ok == Type_B_Rrun()))          // ���Զ������ж��Ƿ�Type A ����
						{
								cardStatus = WAITREMOVE; // ��Ƕ������
						}
						else        // û�ж�����
						{
								rcValue = sky1311RCFreqCali(); // ����У׼RCƵ��
								cardStatus = CARDREMOVED;
						}
			}
		else if(cardStatus == WAITREMOVE)       // �ȴ����뿪
			{
					DelayMS(300);                   // ��΢��ʱһ�£�����̫Ƶ���ж��Ƿ��뿪
					if(checkCardRemove(rcValue))    // �жϿ��Ƿ��뿪
					{
							cardStatus = CARDREMOVED;       // ��ǿ��Ѿ��뿪
//										rcValue = sky1311RCFreqCali(); // ����У׼RCƵ��
							DelayMS(300);
							sky1311Reset();
						
					}
		//		cardStatus = CARDREMOVED;       // ��ǿ��Ѿ��뿪
			}
//				NRF_WDT_Feed();
		}
}

void CL_SKY1311_IRQ()  //���SKY1311�ж�
{
		SKY1311_ENABLE();
		irqClearAll();
		SKY1311_DISABLE();
}

/***********************************************************

Uart_Close �ص�

//����ʱʱ��ﵽ�˹رմ��ڵ�ʱ�� ���Զ��رմ���

���룺 ��
�������

���ڴ��ڿ���ʱ��ʱͨ�����Uart_Close_T ���������ڵĿ���ʱ��

*********************************************************/

uint32_t Uart_Close_T =0;

void Uart_Close_Hander(void *params)	  //�رմ����жϻص�
{
	
	#ifdef NB_Enable
	
	if(NB_Dat_Len>0)   		//�ж��Ƿ���  NB ����д��
	{
		
			Uart_Close_T =0;				//  NB ����������ʱ��չرմ��ڼ�ʱֵ
			
	}
	else								//����û�� NB ��������ʱ������رմ���
		Uart_Close_T ++;
	
	#else
									//δʹ��NB ģ��ʱ
	Uart_Close_T ++;
	
	if(Controllable_Power_State==0)	//��ָ��ͷ��Դ�ر�ʱ�رմ���(��ǰ����ָ��)
	{
		
		Uart_Close_T = Uart_Close_Time*1000;
		
	}
	
	#endif
	
	if(Uart_Close_T>(Uart_Close_Time*1000/Uart_Close_Time_Size))   
	{	
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Close_Uart!\r\n");
		
		#endif
		
		#ifdef NB_Enable    // NB ����  ���豸�������ģʽǰ�����ڲ���������
		
		if((MPR121_RUN==1)||(SPI0_Start==1))    //���豸��Ȼ���ڻ���״̬��Flash�ӿ�������״̬��
		{
			
				Uart_Close_T =0;   // δ����ʱ���¿�ʼ���ڹرռ�ʱ
			
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
		
		else   //���豸������ʱ������رմ��ڣ�����ʼ���� NB���� �ڵ�����ָ��
		{
			
			NB_Run =0;   //�ڹرմ���ʱ����NB ��־λ����
		 	Uart_Close_T =0;
			close_uart();
			app_timer_stop(Uart_Close);   //�������պ�ֹͣ��ʱ��
			
		}
		
		#else   //һ�㿪������
		
		Uart_Close_T =0;
		F_Print_Run_Flag =0;
		Test_Ps_Mark =0;		//�رմ���ʱ��� ���Ա�־
		close_uart();
		
		Controllable_Power_Off();			//�ر�ָ�Ƶ�Դ
		
		app_timer_stop(Uart_Close);   //�������պ�ֹͣ��ʱ��
		
		
		#endif

		
	}
	
}


/**@brief Application main function.//Ӧ�ó������Ҫ���ܡ�
 */

//��Χ�ں���������

void F_Print_Input_Ps_Run(void);				//����ָ��¼�����(ֱ�ӷ�����������һ���������Զ����ѵ���)

void PS_Print_Run(void);				//ָ��ͷ����״̬

void SKY1311_Error_Run(void);			//SPI_1311 ����ָ�����

void CL_PS_Print_Run(void);				//����ɾ��ָ��

void FD_Key_Run(void);			//��������

uint32_t id1,id2;  //ID�Ż��棬ֱ����Ϊ�豸��ʹ��

extern uint8_t etp[10]; //��������������ַ���

uint8_t TX_BUFF[64];

uint32_t Input_PS_Lmage_t =0;		//ָ��¼�����м��

uint32_t PS_Goto_Sleep_t =0;			//ʹָ�ƽ���˯��ģʽ�����м��

uint8_t Type_Card_Id[16]={1,2,3,4,5,6,7,8};
uint32_t Note_Num_Delay;   // ���ż�¼���� �õ� �ӳ�

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
		ble_gap_addr_t addr;  //���ڻ�ȡmac��ַ
	
		id1 = NRF_FICR->DEVICEID[0];
		id2 = NRF_FICR->DEVICEID[1];

		bool erase_bonds;  // ������ʼ��
	
//	#ifdef uart_Enable
//	
//	uart_init();
//	
//	#endif
	
//		#ifdef uart_start  //������Ҫ�������ڼ���		
//		uart_init();
//		#endif
		//printf("Uart_t!\r\n");
//		NRF_WDT_Feed();

		start_init();  //�����ʼ��
	

		
//		Controllable_Power_Off();
		
		NRF_WDT_Feed();
	
		Key_Num_n =0; //��λ��������ֵ
		for(i =0;i<Key_Num_Tab_Len;i++) //��ʼ����ʱ�������뻺��
		{
			
			Key_Num_Tab[i] = 0xFF;  //������뻺��
		
		}

//		read_Soctor(0);
//		read_Soctor(1);  //��ȡFlash ��������
//		read_Soctor(2);
//		read_Soctor(1023);


//		uint8_t  start_string[] = START_STRING;  // start_string �����豸�󴮿���������ݰ�

//			nrf_delay_ms(100);
//			SPIFlash_Erase_Sector(FLASH_BLOCK_NUMBLE,0); // (��/��) ����
//			nrf_delay_ms(100);
	#ifdef Debug
		w25q32_run();
	#endif

	// Initialize. ��ʼ��
   /* ��ʱ����ʼ��
	
	��ʼ�������ʱ��ģ�飬�ö�ʱ��ģ�鲢����ʹ��timer0-2��ʵ�ֶ�ʱ���ܡ�����ʹ��51822�е�RTC1 �����ģ�����ʱ��
	ģ�顣RTC1ʹ��32.768kʱ�Ӿ�����Ƶ������Ϊʱ��Դ�����Ըú����ڲ�ʵ�־�������RTC1��صļĴ�������һЩ��ʼ����
	��ԭ���timer��ʱ/������ģ�����ơ�����ϸ�ڲο�оƬ�����ֲᡣ
*/
	
	APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
		
		//���ڳ�ʼ��������uart���ţ������ʣ����գ������жϵȡ�������uartģ���ж�
  
		//������LED��ʼ�� LED����ָʾ�㲥������״̬���������ڻ���BLE�豸
		buttons_leds_init(&erase_bonds);
		
	 //Э��ջ��ʼ����demo������Ϊ�ⲿʱ�ӡ�����ע���¼��ɷ�����
	  ble_stack_init();
     
		//GAPһЩ���������ã������豸��������PPCP(��Χ�豸��ѡ���Ӳ���)��
	//(��������ĳ�������豸����Դ�Gameric Access Service �п������õ���Щ����)
	  gap_params_init();  //����������
    
		//�����ʼ�������uart�Ĵ��ڷ�����Ҫ�ṩ��������ֵ�����ֻ��Ͱ����Լ����Ե�ͨ��
	  services_init();
    
		//���ù㲥�����Լ�ɨ����Ӧ���ݡ�
	  advertising_init();
		
		//����MAC��ַ
   	//mac_set();
		
		//���Ӳ������á���Ҫ����ʲôʱ����������Ӳ��������Լ����������Դ�����
		conn_params_init();
    
		#ifdef uart_start
		
    SEGGER_RTT_printf(0,"%s\r\n",START_STRING);
		SEGGER_RTT_printf(0,"start\r\n");
	 
	 #endif
	//���ù㲥���ͣ����������������ʱ�����ԣ�����ʼ�㲥
	
//		if(Devic_Static==true) /  /�豸��ʱ��������
//		{
			err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
			APP_ERROR_CHECK(err_code);
//		}
//		else  //����ر������㲥
//			sd_ble_gap_adv_stop();
//		    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
//    APP_ERROR_CHECK(err_code);
//		advertising_IDLE_Init();
////		 err_code = ble_advertising_stop(BLE_ADV_MODE_FAST);
////    APP_ERROR_CHECK(err_code);
		sd_ble_gap_tx_power_set(4);  		//�����������͹���(accepted values are -40, -30, -20, -16, -12, -8, -4, 0, and 4 dBm)
	
//	    ble_gap_conn_sec_mode_t sec_mode; //����ṹ�� 
			
//		  err_code = sd_ble_gap_device_name_set(&sec_mode,  //�豸��ȫģʽ �޸��豸��
//                                         (const uint8_t *) DEVICE_NAME1,
//		 strlen(DEVICE_NAME1));
//     APP_ERROR_CHECK(err_code);
																			 
	err_code =
           app_timer_create (&my_test_timer, APP_TIMER_MODE_REPEATED ,my_timer_handler);
				APP_ERROR_CHECK(err_code);
	err_code = 
					app_timer_start (my_test_timer,APP_TIMER_TICKS(My_test_timer_Time_Size,APP_TIMER_PRESCALER),NULL);			//ע���µ������ʱ������
		    APP_ERROR_CHECK(err_code);
													
		err_code =
				app_timer_create(&Read_My_Key, APP_TIMER_MODE_REPEATED ,Read_Key); //ע����̶�ʱ������
						APP_ERROR_CHECK(err_code);
//	err_code =
//				app_timer_create(&my_Op_Door_timer, APP_TIMER_MODE_REPEATED ,my_Op_Door_Out);  //ע�᷵�ؿ��ż�¼�Ķ�ʱ������
//			    APP_ERROR_CHECK(err_code);
													
	err_code =
				app_timer_create(&Timer_1, APP_TIMER_MODE_REPEATED ,Timer_1_Handler);  //ע�᷵�ؿ��ż�¼�Ķ�ʱ������
			    APP_ERROR_CHECK(err_code);
					
	err_code =
				app_timer_create(&Uart_Close, APP_TIMER_MODE_REPEATED ,Uart_Close_Hander);  //ע���Զ��رմ��ڵĶ�ʱ������
			    APP_ERROR_CHECK(err_code);
					
	
	
//	app_timer_start(Timer_1,APP_TIMER_TICKS(Timer1_Time_Size,APP_TIMER_PRESCALER),NULL);			//����500ms����Ķ�ʱ�����ʱ������
//	
	//	    APP_ERROR_CHECK(err_code);
	
	//	LOCK_OPEN_Init(); //��������б������GPIO
																				 
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
		

		Spi_Flash_Init();  //����SPI_FLASH ͨѶ

		Lock_Open_EN =Get_Cat_Dat(0x50);					//��ȡ�Ƿ���δ�غ�����������

//		
//			NRF_WDT_Feed();
//			read_Soctor(0);
//			
//			NRF_WDT_Feed();
//			read_Soctor(1);

//			NRF_WDT_Feed();
//			read_Soctor(AD_TypeA_Data_Secotr1);  //��ȡ����Ա��������




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

		err_code = sd_ble_gap_address_get(&addr);  //��ȡMAC��ַ
		APP_ERROR_CHECK(err_code);
		
		for(i =0;i<6;i++)
		MAC_ADD[i] = addr.addr[i];  //��MAC��ַ���� MAC_ADD
		ref_BLE_Adv();

	//	DEVICE_NAME[2] = 'O';
	//GAPһЩ���������ã������豸��������PPCP(��Χ�豸��ѡ���Ӳ���)��
	//(��������ĳ�������豸����Դ�Gameric Access Service �п������õ���Щ����)
	//	  gap_params_init();  //����������	  
	//		advertising_init();


//			BLE_TX[i] = TX_Buff[i+20]; //��ֵ��һ�����ݰ�

		PassWord_Error_Num =0;
//	input_speak(28);

//		Get_Time(); //��ȡ����ʱ��
//		Unix_T =Time_to_Unix(YEAR+2000,MON,DAY,HOUR,MIN,SEC);	 //������ʱ��ת��Ϊʱ���			
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
//			else if(it==0)  //�����һ��
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
//	close_uart();   //������ɺ�رմ���
//	
//	#endif


	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
	Spi_Flash_Sleep();
	
	sky1311_Init();
	
	while(SKY1311_RC_VALUE()==0)nrf_delay_ms(1000);

	Spi_Flash_Init();


	sky1311Reset();         
	

	irqClearAll();              // ���SKY1311�������ж�
	checkCardInit(LEDCL_RC_VALUE);    // ���ý����Զ��쿨״̬
	cardStatus = GOTOSLEEP;

	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
//	
	Spi_Flash_Sleep ();
	
	//nrf_delay_ms(100);

		
	Spi_Flash_Init();
	
	//CL_FlashWord_Num();
	
//	Ev_Cat_Dat(AD_TypeA_Data_Num_Code,25);  //���µ�������д��Ŀ¼
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

//		Ev_Cat_Dat(AD_TypeA_Data_Num_Code,0);  //���µ�������д��Ŀ¼
//		
//		Ev_Cat_Dat(US_TypeA_Data_Num_Code,0);  //���µ�������д��Ŀ¼
//		
//		Ev_Cat_Dat(AD_TypeB_Data_Num_Code,0);  //���µ�������д��Ŀ¼
//		
//		Ev_Cat_Dat(US_TypeB_Data_Num_Code,0);  //���µ�������д��Ŀ¼
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
//		read_Soctor(AD_TypeA_Data_Secotr1);  //��ȡ����Ա��������
//		
//		NRF_WDT_Feed();
//////		
		
		//Se 65
		
		
//		#ifdef uart_start
//		
//		SEGGER_RTT_printf(0,"\r\n US_TypeA_Data_Secotr1:\r\n");
//		
//		#endif
		
		
//		read_Soctor(US_TypeA_Data_Secotr1);  //��ȡ����Ա��������
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
//		read_Soctor(AD_TypeB_Data_Secotr1);  //��ȡ����Ա��������
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
//		read_Soctor(US_TypeB_Data_Secotr1);  //��ȡ����Ա��������
//		
//
//		nrf_gpio_pin_clear(LEDR);
//		nrf_gpio_pin_set(LEDB);
	//	nrf_gpio_pin_set(LEDB1);
	
		SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua	
		Spi_Flash_Sleep();
	
		nrf_delay_ms(100);
		TSM12_Init();

		nrf_gpio_cfg_input(7,NRF_GPIO_PIN_PULLUP);  //����GPIOΪ����

	
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
	
	#ifdef PS_Enable   //�������ڹ���
	
//	for(i =0;i<sizeof(TX_BUFF);i++)
//		TX_BUFF[i] =i;
	
//	if(A71_RF_RX_MODE()==1)  //��ģ��������ģʽ��
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
	
	
/*		����ָ��ͷЭ��
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

for (i =0;i<sizeof(PS_PowDlow);i++)			//�ڷ��ͻ�����д������ָ��
	{
		
		TX_BUFF[i] = PS_PowDlow[i];
		
	}
	
	Uart_Put_String(TX_BUFF,sizeof(PS_PowDlow));
	F_print_Enable =0;
	Test_Ps_Mark =1;		// ����ָ��ͷ�Ƿ���ڵı�־
	
	
	Input_Ps_Lmage_Run_Now = 0;		//���״̬��
	Input_Ps_Lmage_Type = 0;				//��ʼ��ָ��¼�����ֵ
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
//	nrf_gpio_cfg_input(13,NRF_GPIO_PIN_PULLUP);			//������𰴼�
	
	//��̬���뱣�����
//	More_Act_PassWord_Save(uint8_t *Act_PassWord,uint8_t Act_PassWord_Len,uint32_t past_unix_t); 

/******************************************************
	#ifdef uart_start    //����ģʽ
	
	Spi_Flash_Init();  //����SPI_FLASH ͨѶ	

	Ev_Cat_Dat(More_Act_PassWord_Num_Code,0xFFFF0000);  //��Ϊ����ģʽ
	
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
	
	
	//���鶯̬�����е���ɾ��
		//uint8_t Dle_A_More_Act_PassWord_Verify(uint8_t *PassWord);				//�Ӷ��鶯̬������ɾ��һ��
	
	
	
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
	
	SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
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
			Bsp_Set();   //������������ BSP
//			sky1311_Read();
			if((BLE_LINK==0)&&(Devic_Static==false)&&(!MPR121_RUN)) //�������Ͽ������豸δ��,����δ����ʱ�رչ㲥
			{
				
				sd_ble_gap_adv_stop();
			
			}
			
//			if((Note_Num_1>0)&&(Note_Num_1<=Open_Note_Size)&&(opdt == 0))		//������ż�¼   ��Ϊ��������
//			{
//				Note_Num_Delay++;
//				
//				if(Note_Num_Delay>=1)   // �ȴ������ȶ�
//				{
//						err_code = app_timer_start(my_Op_Door_timer,APP_TIMER_TICKS(my_Op_Door_timer_Size,APP_TIMER_PRESCALER),NULL);			//ע���µ������ʱ������
//						APP_ERROR_CHECK(err_code);
//						opdt =1;
//						Note_Num_Delay =0;
//					
//				}
//			}
			
//			#ifdef NB_Enable     ///�� NB ģ�����ʱ
//			
//			if((UART_RUN==0)&&(NB_Dat_Len==0)&&(Input_Ps_Lmage_Type==0))   //�����ڹر���NB ����û������ʱ
//			{
//				
//					power_manage();
//				
//			}
//			else if((UART_RUN==0)&&(NB_Dat_Len>0))   //���ڹرպ� NB ������������ʱ����ʼ����NB ������������
//			{
//				
//				
//				
//			}
//			
//			#endif
//			

			#ifdef uart_Enable
			
			if((UART_RUN==0)&&((F_print_Enable==0)||((Input_Ps_Lmage_Type==0)&&(Controllable_Power_State==0)&&(F_Print_Run_Flag==0)&&(CL_PS_Print_Now==0)&&(CL_PS_Run==CL_PS_Run_Del_Nor_Mode)))&&(FD_Key==0))     //��ͨ����ģʽ	
							power_manage();
			
			#else
			
			if(FD_Key==0)
				power_manage();
			
			#endif
			
			FD_Key_Run();
			
			#ifdef PS_Enable
			
//			if((F_print_Enable==1)||Test_Ps_Mark==1)
//			{
				
				F_Print_Input_Ps_Run();		//¼��ָ�� ����
				
				PS_Print_Run();			//ָ��ͷ�����ⲿ��������
				
				CL_PS_Print_Run();		//���ָ��ͷ���� ����
				
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
			input_speak(20);				//��ʾ�Ƿ�����
			
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

void F_Print_Input_Ps_Run()				//����ָ��¼�����(ֱ�ӷ�����������һ���������Զ����ѵ���)
{
			if(UART_RUN==1)					//���ڽ��ճ���
			{
				if((MPR121_RUN==1)&&(MPR121_Flag ==0))			//�����̿���ʱ��Ϊ�˴���ͨ�����̣���Ҫ��ʱ�رոö�ʱ��
				{
					MPR121_Flag =1;
					app_timer_stop(Read_My_Key);

				}
				
				Uart_Get_String();
				
			}
			else if((MPR121_RUN==1)&&(MPR121_Flag==1))
			{
				
				MPR121_Flag =0;
				app_timer_start(Read_My_Key,APP_TIMER_TICKS(Read_Key_Time_Size,APP_TIMER_PRESCALER),NULL);	//ͨ�Ž���ʱ���ظ���ʱ��״̬

			}
			
			if((Input_Ps_Lmage_Type!=0)&&
	//			(Controllable_Power_State==0)&&		//ָ��ͷδ������ʱ			
				(F_print_Enable==1)			//������ָ��¼��ʱʵ��ָ��¼����
				&&(CL_PS_Print_Now==0))		//ɾ��ָ��δ����
				Input_PS_Lmage_t++;
			else
			{
				Input_Ps_Lmage_Type =0;
				Input_PS_Lmage_t =0;			//�����������������ֵ
			}
			
			if(Input_PS_Lmage_t>1500)
			{
			
				Input_PS_Lmage();				//¼��ָ�Ƴ���
				Input_PS_Lmage_t =0;
			
			}
}

/*********************************
void PS_Print_Run()

 ָ��ģ��������״̬�µ�״̬

*********************************/

uint32_t OP_PS_AU;		//�ж�ָ���ǹ���Ա�����û�

uint8_t PS_Print_Run_Now =0;
uint16_t PS_Delay_T =0;			//��������״̬�������ļ��ʱ��
uint8_t PS_Int_T =0;			//ָ���жϽŴ�����ʱ����
uint16_t Need_For_Ps_Print_ID =0XFFFF;		//��ָ��ͷ��ȡ���� ID �ţ��ڲ���ƥ��ָ��ʱ��

const uint16_t PS_Delay_Out_T	= 300;			//����״̬���ӳ�ʱ��
const uint16_t PS_Dat_T_Out = 20000;				//����״̬���ȴ����ݰ������ʱ��
void PS_Print_Run()
{
	
	
	#ifdef PS_Enable 
	
	if(nrf_gpio_pin_read(F_Print_IRQ)&&(Controllable_Power_State==0)&&(F_Print_Run_Flag==0)&&(CL_PS_Print_Now==0)&&(F_print_Enable==1))					//ָ�ƴ����ж�
	{
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"F_Print_IRQ_Input!\r\n");
		
		#endif
		Controllable_Power_No();		//��ָ��ģ���ϵ�
		
		if(Devic_Static==false)  //���豸δ��ʱ����ֱ����ָ�ƿ���(�����������κδ������ż�ʹָ�ƽ���˯��״̬)
		{
				nrf_gpio_pin_set(LEDB);
		//		nrf_gpio_pin_set(LEDB1);  //ֱ�ӿ���
			
				Lock_Open();
							
				#ifdef uart_start
					
				SEGGER_RTT_printf(0,"F_Print_Open_Door!\r\n");	
					
				#endif
			
				if(SPI0_Start==1)			// ��  SPI0 ������ʱ  
				{
						;		//����ʱ�������Ѱ�������Ӧ
				}
				else
				{
					Start_Key_Run();			//��SPI0 δ����ʱ��ֱ�ӻ��Ѽ���
				}
			F_Print_Run_Flag =1;			//��ʾָ��ͷ��������״̬
		}
		else
		{
			
			F_Print_Run_Flag =1;			//ʹָ��ͷ��������״̬
			
		}
		
	}
	
	#endif
	
	
	if((PS_Print_Run_Now==0)&&(F_Print_Run_Flag==1))
	{
		
		PS_Print_Run_Now =1;			//�� F_Print_Run_Flag ����1 ʱ����ʾ ָ��ͷ��������״̬����Ҫ��ʼһ�����й���
		
	}
	
	if(Input_Ps_Lmage_Type!=0)			//����¼��ָ�Ƶ�״̬ʱ������ֹͣ����״̬
	{
		
		PS_Print_Run_Now =0;
		F_Print_Run_Flag =0;
		
	}
	
	switch(PS_Print_Run_Now)
	{

		case 0:
		;								//�ȴ�״̬
		break;
		
		case 1:			//�����һ��״̬���ж��Ƿ񱻰�
		
		if(Devic_Static==1)		//������ʱ
		{
		
			PS_Print_Run_Now =8;				//�ڱ���ʱ�����Զ�ȡָ���Ƿ�Ϸ�
			nrf_delay_ms(100);				//��������100ms�ӳ٣����ڵȴ���ָ�ȶ�
		}
		else			//δ����ʱ  ��ȷ����ָ�뿪ģ������˯��ģʽ
		{
			PS_Print_Run_Now=2;		//����ȴ���ָ�뿪��˯�ߵ�״̬
		}
		break;
		
		case 2:				//������ָ֤���Ƿ���ڵ�ָ�ָ��ģ�����˯�ߵĿ�ʼ�׶Σ�
		
		PS_Delay_T =0;		//����ӳ�ʱ��
		PS_Print_Run_Now =3;		//������һ��	
		
		break;
		
		case 3:
		
		PS_Delay_T++;			//�����ӳ�	
		if(PS_Delay_T>=PS_Delay_Out_T)			//�����ʱ
		{
			PS_Delay_T =0;
			PS_Print_Run_Now=4;			//ʱ�����������һ��
		}
		
		break;
		
		case 4:			//����ָ�ƶ�ȡָ���ȷ����ָ�Ƿ��뿪
		
			for(uint8_t i =0;i<sizeof(PS_Getlmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		
		}
		PS_Delay_T =0;		//��һ�����ڷ�ֹ����
		PS_Print_Run_Now =5; //ָ����һ��
		
		break;

		case 5:
		PS_Delay_T++;			//��¼�ȴ�ʱ��
		if(PS_Delay_T>=PS_Dat_T_Out)		//ʱ�����ʱ���·���
		{
			PS_Print_Run_Now =2;		//����󷵻���һ��
			PS_Delay_T =0;	
		}
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//���յ������ݰ�Ϊ��ָ̧��ʱ��������һ��
			)					//����õ�����
		{
			
			PS_Print_Run_Now=6;
			PS_Delay_T =0;
		}
		else if(
		(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)//&&
		//(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ��ָ����ʱ���ص���һ��
				)
		{
			
			PS_Print_Run_Now = 2;		//�ص���һ����ʼ�׶�
			PS_Delay_T =0;
			
		}
		
		break;
		
		case 6:				//����˯��״̬
		
		PS_Delay_T++;
		
		if(PS_Delay_T>=PS_Delay_Out_T)			//����ʱ���ʱ
		{		
			PS_Delay_T =0;
			PS_Print_Run_Now=7;			//ʱ�����������һ��
		}
		
		
		break;
		
		case 7:				//ָ��ģ�����˯�ߵ����׶�
		
		if(F_print_Goto_Sleep()==1)			//�������
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Print_Run_End!\r\n");		//��ʾ���н������Ҷϵ�ָ��ͷ��Դ
			
			#endif
			PS_Print_Run_Now =0;
			Controllable_Power_Off();			//�رյ�Դ
			F_Print_Run_Flag =0;			//������б�־

		}
		else
		{
			PS_Delay_T++;
			if(PS_Delay_T>=PS_Delay_Out_T)			//ʱ�����ʱ������һ��
			{
				PS_Delay_T =0;
				PS_Print_Run_Now =6;
				F_Print_Run_Flag =0;			//������б�־

			}
		}
			
		break;
		
		
		case 8: //��ȡ����
			
		PS_Delay_T =0;		//����ӳ�ʱ��
		PS_Print_Run_Now =9;		//������һ��	
		
		#ifdef uart_start
		
		
		SEGGER_RTT_printf(0,"GOTO_PS_9!\r\n");
		
		#endif
		
		break;
		
		case 9:			//��ʼ��ȡָ�ƣ����ڶԱȱ���ģ��
			//���ƶ�ȡָ�Ƽ��
		PS_Delay_T++;
		
		if(PS_Delay_T>=PS_Delay_Out_T)
		{
			
			PS_Delay_T =0;
			PS_Print_Run_Now = 10;
			
		}
		
		break;
		
		case 10:
		
		for(uint8_t i =0;i<sizeof(PS_Getlmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
		{
			
			TX_BUFF[i] = PS_Getlmage[i];
			
		}
		
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		}
		
		PS_Delay_T =0;		//��һ�����ڷ�ֹ����
		PS_Print_Run_Now =11; //ָ����һ��
		
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"GOTO_PS_11!\r\n");
			
			#endif
		
		
		break;
		
		case 11:
			
		PS_Delay_T++;			//��¼�ȴ�ʱ��
		
		if(PS_Delay_T>=PS_Dat_T_Out)		//ʱ�����ʱ���·���
		{
			
			PS_Print_Run_Now =9;		//����󷵻���һ��
			PS_Delay_T =0;
			
		}
		//��ʼ�ж��յ������ݰ�
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//���յ������ݰ�Ϊ��ָ̧��ʱ��׼���������״̬
			)					//����õ�����
		{
			PS_Print_Run_Now =2;			//δ��⵽��ָʱ��׼�����·���(����)
		//	input_speak(42);		//��ʾָ�ƴ���
			PS_Delay_T =0;		//��� ��ʱ
		}
		else if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00)					//���յ������ݰ�Ϊ��ָ����ʱ��������һ��
					)
					{	
						PS_Print_Run_Now =12;			//������һ��
						PS_Delay_T =0;		//��� ��ʱ
					}					
		else if(
					(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)		//�յ�������ʱ��׼���������״̬
					)
					{
						
						PS_Print_Run_Now = 2;			//׼���������״̬
	//			input_speak(42);			//��ʾָ�ƴ���
						PS_Delay_T =0;		//��� ��ʱ
						
					}
					
		break;
					
		case 12:			//���ͱ������ݵ�����1 (���)
		
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
		
		case 13:			//��ʼ���Ա������ݵ����� 1	
		
		for(uint16_t i =0;i<sizeof(Get_Dat_Save_1);i++)			//�ڷ��ͻ�����д�뱣��ָ��
			{
				
				TX_BUFF[i] = Get_Dat_Save_1[i];
				
			}
			
		Uart_Put_String(TX_BUFF,sizeof(Get_Dat_Save_1));

		for(uint16_t i =0;i<sizeof(Ps_Data_Tab);i++)
			{
			
				Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
			
			}
		PS_Delay_T =0;
		PS_Print_Run_Now = 14;		//������һ��
		
		break;
		
		case 14:			//�ȴ�����ȷ�ϰ�
				
		PS_Delay_T++;			//��¼�ȴ�ʱ��
		if(PS_Delay_T>=PS_Dat_T_Out)		//ʱ�����ʱ���·���
		{
			PS_Print_Run_Now =12;		//����󷵻ط��ͱ��浽����1�����
			PS_Delay_T =0;
			
		}
		
		//�յ�ȷ�ϰ��ж��Ƿ���Ч
		if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ�洢�ɹ���Ӧ��ʱ������һ��
					)
				{
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_1_OK!\r\n");
					
					#endif
					PS_Print_Run_Now =15;				//�յ�ȷ�ϰ��������һ��
					PS_Delay_T =0;		//��ն�ʱ
				}
				else if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01))
				{
					
					#ifdef uart_start
			
					SEGGER_RTT_printf(0,"\r\nGen_Dat_Save_1_Error!\r\n");
					
					#endif
					
					PS_Print_Run_Now =9;		//�������ݰ� ��ʼ��������¼��ָ������
					
					PS_Delay_T =0;			//��ն�ʱ
					
				}
		
		
		break;
		
		case 15:			//����ȡ����ָ����ָ�ƿ����ƥ�� ������Ҫ�ӳ٣�
		
		for(uint8_t i =0;i<sizeof(Se_Lmage_Dat);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
		{
			
			TX_BUFF[i] = Se_Lmage_Dat[i];
			
		}
		
		Uart_Put_String(TX_BUFF,sizeof(Se_Lmage_Dat));
		
		for(uint8_t i =0;i<sizeof(Ps_Data_Tab);i++)
		{
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		}
		
		PS_Delay_T =0;		//��һ�����ڷ�ֹ����
		PS_Print_Run_Now =16; //ָ����һ��
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"GOTO_PS_16!\r\n");
		
		#endif
		
		break;
		
		case 16:
			
		PS_Delay_T++;			//��¼�ȴ�ʱ��
		if(PS_Delay_T>=PS_Dat_T_Out)		//ʱ�����ʱ���·���
		{
			PS_Print_Run_Now =12;		//����󷵻ط��ͱ��浽����1�����
			PS_Delay_T =0;	
			
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"From16_GOTO_12!\r\n");
			
			#endif
		}
		//��ȡָ��ƥ����
		if(
			(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)
			)
		{
			if(
				(Ps_Data_Tab[8]==0x07)&&((Ps_Data_Tab[9]==0x00)||(Ps_Data_Tab[10]==0x01))		//����ȡ��ƥ���IDʱ
				
				)
				{
					//����ȡ���� ID �ŵ� Need_For_Ps_Print_ID 
					Need_For_Ps_Print_ID =0XFFFF;
					Need_For_Ps_Print_ID = Ps_Data_Tab[10];
					Need_For_Ps_Print_ID += Ps_Data_Tab[11];
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"\r\n\r\nPS_ID: %x!\r\n\r\n",Need_For_Ps_Print_ID);
					
					#endif
					
					if(Need_For_Ps_Print_ID>PS_Print_Num)		//����ȡ���� ID ���ڱ�����������ʱ
					{
						
						input_speak(42);		//��ʾָ�ƴ���
						PS_Print_Run_Now =2;		//�ȴ�ָ��ͷ�������
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"Print_ID_Error!\r\n");		//��ʾ��ȡ����ָ��ID ��Ч
						
						#endif
						
					}
					else		//�����ݺ���ʱ
					{
						PS_Print_Run_Now =17;			//������һ������ʼ��֤ ID ���Ƿ�������
						
						#ifdef uart_start
						
						SEGGER_RTT_printf(0,"\r\n GOTO_17!\r\n");
						
						#endif
						
					}
				}
				else			//δƥ�䵽 һ�µ�ָ�� ʹָ��ͷ����˯��
				{
					
						input_speak(42);		//��ʾָ�ƴ���
						PS_Print_Run_Now =2;		//�ȴ�ָ��ͷ�������
					
				}
		}
		
		break;
		
		case 17:			//��ʼ��֤ ID ���Ƿ�������
			
		OP_PS_AU = Opinion_PS_Print_Valid(Need_For_Ps_Print_ID);
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"OP_PS_AU: %x\r\n",OP_PS_AU);
		
		#endif
		
		if(OP_PS_AU!=0xFFFF)		//��ָ����Чʱ
		{
			//������		
			Spi_Flash_Init();  //����SPI_FLASH ͨѶ (��Ϊ�˴�SPI�ӿ�δ��)
			OP_PS_AU&=0XFF0000;
			
			if(OP_PS_AU==0X010000)		//����Աָ�ƿ���
			{
				ADD_Op_Door_Note(Need_For_Ps_Print_ID,0x0101,Unix_T);
			}
			else if(OP_PS_AU==0X020000)  //�û�ָ�ƿ���
			{
				ADD_Op_Door_Note(Need_For_Ps_Print_ID,0x0201,Unix_T);
			}

			SpiFlash_PowerDown();   //��ɲ���ʱ��ʹ�����͹���ģʽ 1ua
			Spi_Flash_Sleep();
			
			Lock_Open();			//����
		}
		else
		{
			input_speak(42);		//��ʾָ�ƴ���
		}
		
		//���ָ��׼��ָ��ͷ����
		
		PS_Print_Run_Now =2;			//�͵������������
		
		break;
		
		
		default:			//�������ֱ����ģ��������ߵ�״̬ ��һ�㲻���ܽ��룩
			
			PS_Print_Run_Now =2;			//����˯�ߵȴ�״̬
		
		break;

	}
	
}






/************************
ɾ�� ָ�� ״̬��

��������ָ��ͷ�ڲ����ݣ�

************************/

uint16_t CL_PS_ID =0;			//�趨 Ҫɾ���� ָ�� ID  
uint16_t CL_PS_Run =0;				//�趨 ɾ��ָ��ͷ����ģʽ

uint8_t CL_PS_Print_Now =0;
uint16_t CL_PS_Delay_T =0;


const uint16_t CL_PS_Delay_T_Out =300;
const uint16_t CL_PS_Time_Out = 20000;		//�趨�ȴ���ʱ

//��Ϊ����������� Ŀǰֻ����ɾ������ ID �� ȫ������ķ�ʽ ����ɾ�������´�һ������~

//����ɾ��ģʽ�ĳ���

const uint8_t CL_PS_Run_Del_Nor_Mode =0;					//ɾ�� ָ��ID ���ܹ���
const uint8_t CL_PS_Run_Del_Single_Mode =1;				//ɾ������ ָ��ID ��ģʽ
const uint8_t CL_PS_Run_Del_All_Mode =2;					//ɾ������ ָ��ID ��ģʽ
const uint8_t CL_PS_Run_Del_Batch_Mode =3;				//����ɾ�� ָ��ID ��ģʽ

void CL_PS_Print_Run()
{
	
	uint16_t Dat_cheak;				//У�����ݱ���
	
//	if((F_Print_Run_Flag==0)&&(Input_Ps_Lmage_Type==0)&&(CL_PS_Run!=CL_PS_Run_Del_Nor_Mode)&&(Controllable_Power_State==0)) //����������δ������ CL_PS_Run δ������ʱ
//	{
//		
//		#ifdef uart_start
		
		
//		SEGGER_RTT_printf(0,"CL_PS_Run: %x!\r\n",CL_PS_Run);
//		SEGGER_RTT_printf(0,"CL_PS_ID: %x!\r\n",CL_PS_ID);
//		
//		while(1);
//		#endif
		
		if((Controllable_Power_State==0)&&(F_Print_Run_Flag==0)&&(Input_Ps_Lmage_Type==0)&&(CL_PS_Print_Now==0)&&(MPR121_RUN==0))			//��ָ�ƹر�ʱ
		{
			switch(CL_PS_Run)			//����  Run ֵ ȷ��ɾ����ʽ
			{
				case 0:						//Ϊ 0 ʱ�޹���
				
			//	CL_PS_Print_Now =0;											//ʹ�����״̬����Ч
			//	CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//ͬʱ���ģʽת��λ
			//	CL_PS_Delay_T =0;						//��ն�ʱֵ
				
				break;
				
				case CL_PS_Run_Del_Single_Mode:			//ɾ������ָ�Ƶ�ģʽ
				
				CL_PS_Print_Now =7;					//ʹ CL_PS_Print_Now ָ��ɾ��һ��ָ��
				if(Controllable_Power_State==0)			//��ָ�Ƶ�Դ���ر�ʱ��ָ��ģ���Դ
					{
						Controllable_Power_No();				
					}
					
		//		CL_PS_Delay_T =0;						//��ն�ʱֵpw
				
				CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//ͬʱ���ģʽת��λ
					
				break;
				
				case CL_PS_Run_Del_All_Mode:				//ɾ������ָ�Ƶ�ģʽ
				
				CL_PS_Print_Now =10;
				
				if(Controllable_Power_State==0)			//��ָ�Ƶ�Դ���ر�ʱ��ָ��ģ���Դ
					{
						Controllable_Power_No();				
					}
		//		CL_PS_Delay_T =0;						//��ն�ʱֵ
				
				CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//ͬʱ���ģʽת��λ
				
				break;
				
				case CL_PS_Run_Del_Batch_Mode: 
					
				CL_PS_Print_Now =0;
			//	CL_PS_Delay_T =0;						//��ն�ʱֵ
				
				CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//ͬʱ���ģʽת��λ
				
				break;
				
				
				default:			//������������״̬λ
				
				CL_PS_Print_Now =0;
				CL_PS_Run = CL_PS_Run_Del_Nor_Mode;
			//	CL_PS_Delay_T =0;						//��ն�ʱֵ
				
				
				break;
				
			}
	}
		
//	}
//	else			//����������ʱ���
//	{
////		CL_PS_Print_Now =0;
//		CL_PS_Run =CL_PS_Run_Del_Nor_Mode;			//ͬʱ���ģʽת��λ
////		CL_PS_Delay_T =0;						//��ն�ʱֵ
//	}
	
	switch (CL_PS_Print_Now)
	{
		case 0:			//����
				CL_PS_Delay_T =0;
			break;
		case 1:			//�������ģʽ ���
			CL_PS_Delay_T =0;				//��յȴ�ʱ�����
			CL_PS_Print_Now =2;		//������һ��
		
			break;
		case 2:			//�����ӳ�
			
			CL_PS_Delay_T ++;
			if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//��ʱ�����������һ��
			{
				
				CL_PS_Delay_T =0;
				CL_PS_Print_Now =3;
				
			}
		
			break;
			
		case 3:			//����ָ�ƶ�ȡָ���ȷ����ָ�Ƿ��뿪
			
			for(uint8_t i =0;i<sizeof(PS_Getlmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
			
		}
		
		CL_PS_Delay_T =0;		//��һ�����ڷ�ֹ����
		CL_PS_Print_Now =4; //ָ����һ��
		
		break;
		
		case 4:
			
			CL_PS_Delay_T++;			//��¼�ȴ�ʱ��
			if(CL_PS_Delay_T>=CL_PS_Time_Out)		//ʱ�����ʱ���·���
			{
				CL_PS_Print_Now =5;		//��������������һ��
				CL_PS_Delay_T =0;	
			}
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//���յ������ݰ�Ϊ��ָ̧��ʱ��������һ��
			)					//����õ�����
		{
			
			CL_PS_Print_Now=5;
			CL_PS_Delay_T =0;
		}
		else if(
		(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)//&&
		//(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ��ָ����ʱ���ص���һ��
				)
		{
			
			CL_PS_Print_Now = 1;		//�ص���һ����ʼ�׶�
			CL_PS_Delay_T =0;
			
		}
			
		break;
		
		case 5:				//��ָ�ƴ���ʱ����˯��״̬
		
		CL_PS_Delay_T++;
		
		if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//����ʱ���ʱ
		{		
			
			CL_PS_Delay_T =0;
			CL_PS_Print_Now =6;			//ʱ�����������һ��
		
		}
		
		break;
		
		case 6:
		
		if(F_print_Goto_Sleep()==1)			//�������
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Print_Run_End!\r\n");		//��ʾ���н������Ҷϵ�ָ��ͷ��Դ
			
			#endif
			CL_PS_Print_Now =0;
			Controllable_Power_Off();			//�رյ�Դ
			
		}
		else
		{
			
			CL_PS_Delay_T++;
			if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//ʱ�����ʱ������һ��
			{
				
				CL_PS_Delay_T =0;
				CL_PS_Print_Now =5;
				
			}
		}
		
		break;
		
		case 7:						//ɾ��һ��ָ�ƴ����￪ʼ
			
		CL_PS_Delay_T++;					//�ӳ�
		
		if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//����ʱ���ʱ
		{		
			
			CL_PS_Delay_T =0;
			CL_PS_Print_Now =8;			//ʱ�����������һ��
			

		}
		
		break;
		
		case 8:						//���͵���ָ��ɾ�����ݰ�
		
			for(uint8_t i =0;i<sizeof(PS_Del_A_Lmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
			{
				
				TX_BUFF[i] = PS_Del_A_Lmage[i];
				
			}
	
		TX_BUFF[10] =0;					// id ��λ����Ϊ 0  
		TX_BUFF[11]	=CL_PS_ID;  //�趨 ID ��
			
		//��У���
		
			Dat_cheak =0;			//���У��ֵ
		for(uint8_t i;i<(sizeof(PS_Del_A_Lmage)-8);i++)
			{
				Dat_cheak += TX_BUFF[i+6];
			}
		
		#ifdef uart_start
		
		SEGGER_RTT_printf(0,"Dat_cheak: %x.\r\n",Dat_cheak);			//��� У��ֵ
			
		#endif
			
		TX_BUFF[14] = Dat_cheak&0XFF>>8;
		TX_BUFF[15] = Dat_cheak&0xFF;				//��У��λ�ŵ�������
		
		Uart_Put_String(TX_BUFF,sizeof(PS_Del_A_Lmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
			
		}
		CL_PS_Print_Now =9;
		CL_PS_Delay_T =0;
		break;
		
		case 9:				//�ȴ�ָ�ƽ�������
		
			CL_PS_Delay_T++;			//��¼�ȴ�ʱ��
			if(CL_PS_Delay_T>=CL_PS_Time_Out)		//ʱ�����ʱ���·���
			{
				CL_PS_Print_Now =7;		//����󷵻���һ�����
				CL_PS_Delay_T =0;	
			}
		
		//��ʼ�ȴ���������
			
		if((Ps_Data_Tab[0]==0XEF)&&(Ps_Data_Tab[1]==0x01))
		{
			if((Ps_Data_Tab[8]==0x03)&&((Ps_Data_Tab[9]==0x00)||(Ps_Data_Tab[9]==0x01)))				//��ɾ���ɹ�ʱ
			{
					CL_PS_Print_Now =1;		//ɾ���ɹ������������
					CL_PS_Delay_T =0;	
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Del_A_PS_Lmage_OK!\r\n");
					
					#endif
					
				
			}
			else				//�����������
			{
					CL_PS_Print_Now =7;		//����󷵻���һ�����
					CL_PS_Delay_T =0;	
			
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Del_A_PS_Lmage_Warning!\r\n");
					
					#endif
							
			}
		}
		
		break;
		
		case 10:					//ɾ������ָ�Ƶ����
			
		CL_PS_Delay_T++;					//�ӳ�
		
		if(CL_PS_Delay_T>=CL_PS_Delay_T_Out)			//����ʱ���ʱ
		{		
			
			CL_PS_Delay_T =0;
			CL_PS_Print_Now =11;			//ʱ�����������һ��
			
		}
		
		break;
		
		case 11:
		
			for(uint8_t i =0;i<sizeof(PS_Del_All_Lmage);i++)			//�ڷ��ͻ�����д��ɾ������ָ��
			{
				
				TX_BUFF[i] = PS_Del_All_Lmage[i];
				
			}
	
			Uart_Put_String(TX_BUFF,sizeof(PS_Del_All_Lmage));
			
			for(uint8_t i =0;i<50;i++)
			{
				
				 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
			
			}
			
		CL_PS_Delay_T =0;
		CL_PS_Print_Now =12;
		
		break;
		
		case 12:
		
		CL_PS_Delay_T++;			//��¼�ȴ�ʱ��	
		
		if(CL_PS_Delay_T>=CL_PS_Time_Out)		//ʱ�����ʱ���·���
		{
			
			CL_PS_Print_Now =1;		//����󷵻���һ�����
			CL_PS_Delay_T =0;	
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"CL_PS_ALL_Time_Out!\r\n");
			
			#endif
			
		}
	
		//��ʼ�ȴ���������
		
		if((Ps_Data_Tab[0]==0XEF)&&(Ps_Data_Tab[1]==0x01))
		{
			if((Ps_Data_Tab[8]==0x03)&&((Ps_Data_Tab[9]==0x00)||(Ps_Data_Tab[9]==0x01)))				//��ɾ���ɹ�ʱ
			{
				
					CL_PS_Print_Now =1;		//ɾ���ɹ������������
					CL_PS_Delay_T =0;	
					
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Del_All_PS_Lmage_OK!\r\n");
					
					#endif	
				
			}
			else				//�����������
			{
					CL_PS_Print_Now =1;		//����󷵻ش������
					CL_PS_Delay_T =0;	
			
					#ifdef uart_start
					
					SEGGER_RTT_printf(0,"Del_All_PS_Lmage_Warning!\r\n");
					
					#endif
							
			}
		}
		
		break;
		
		default:		//�������������������״̬��

		#ifdef uart_start
			
		SEGGER_RTT_printf(0,"Del_A_PS_Lmage_Error!\r\n");
			
		#endif

		CL_PS_Print_Now =1;
		
		break;
			
	}
	
}

/*********

		case 2:				//������ָ֤���Ƿ���ڵ�ָ�ָ��ģ�����˯�ߵĿ�ʼ�׶Σ�
		
		PS_Delay_T =0;		//����ӳ�ʱ��
		PS_Print_Run_Now =3;		//������һ��	
		
		break;
		
		case 3:
		
		PS_Delay_T++;			//�����ӳ�	
		if(PS_Delay_T>=PS_Delay_Out_T)			//�����ʱ
		{
			PS_Delay_T =0;
			PS_Print_Run_Now=4;			//ʱ�����������һ��
		}
		
		break;
		
		case 4:			//����ָ�ƶ�ȡָ���ȷ����ָ�Ƿ��뿪
		
			for(uint8_t i =0;i<sizeof(PS_Getlmage);i++)			//�ڷ��ͻ�����д���ȡָ��ָ��
			{
				
				TX_BUFF[i] = PS_Getlmage[i];
				
			}
	
		Uart_Put_String(TX_BUFF,sizeof(PS_Getlmage));
		
		for(uint8_t i =0;i<50;i++)
		{
			
			 Ps_Data_Tab[i] =0;		//���ָ��ģ����ջ���
		
		}
		PS_Delay_T =0;		//��һ�����ڷ�ֹ����
		PS_Print_Run_Now =5; //ָ����һ��
		
		break;

		case 5:
		PS_Delay_T++;			//��¼�ȴ�ʱ��
		if(PS_Delay_T>=PS_Dat_T_Out)		//ʱ�����ʱ���·���
		{
			PS_Print_Run_Now =2;		//����󷵻���һ��
			PS_Delay_T =0;	
		}
		
			if((Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)&&
			(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x02)				//���յ������ݰ�Ϊ��ָ̧��ʱ��������һ��
			)					//����õ�����
		{
			
			PS_Print_Run_Now=6;
			PS_Delay_T =0;
		}
		else if(
		(Ps_Data_Tab[0]==0xEF)&&(Ps_Data_Tab[1]==0x01)//&&
		//(Ps_Data_Tab[8]==0x03)&&(Ps_Data_Tab[9]==0x00) //���յ������ݰ�Ϊ��ָ����ʱ���ص���һ��
				)
		{
			
			PS_Print_Run_Now = 2;		//�ص���һ����ʼ�׶�
			PS_Delay_T =0;
			
		}
		
		break;
		
		case 6:				//����˯��״̬
		
		PS_Delay_T++;
		
		if(PS_Delay_T>=PS_Delay_Out_T)			//����ʱ���ʱ
		{		
			PS_Delay_T =0;
			PS_Print_Run_Now=7;			//ʱ�����������һ��
		}
		
		
		break;
		
		case 7:				//ָ��ģ�����˯�ߵ����׶�
		
		if(F_print_Goto_Sleep()==1)			//�������
		{
			#ifdef uart_start
			
			SEGGER_RTT_printf(0,"PS_Print_Run_End!\r\n");		//��ʾ���н������Ҷϵ�ָ��ͷ��Դ
			
			#endif
			PS_Print_Run_Now =0;
			Controllable_Power_Off();			//�رյ�Դ
			F_Print_Run_Flag =0;			//������б�־

		}
		else
		{
			PS_Delay_T++;
			if(PS_Delay_T>=PS_Delay_Out_T)			//ʱ�����ʱ������һ��
			{
				PS_Delay_T =0;
				PS_Print_Run_Now =6;
				F_Print_Run_Flag =0;			//������б�־

			}
		}
			
		break;

********/

/** 
 * @}
 */


