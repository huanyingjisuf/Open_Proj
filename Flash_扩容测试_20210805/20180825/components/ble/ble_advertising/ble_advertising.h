/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */


/**@file
 *
 * @defgroup ble_sdk_lib_advertising Advertising Module  //�㲥ģ��
 * @{
 * @ingroup  ble_sdk_lib
 * @brief    Module for handling connectable BLE advertising. ���������BLE�㲥��ģ��
 *
 * @details  The Advertising Module handles connectable advertising for your application. It can 
 *           be configured with advertising modes to suit most typical use cases.
 *           Your main application can react to changes in advertising modes
 *           if an event handler is provided.
 �㲥ģ�鴦��Ӧ�ó���Ŀ����ӹ㲥�����������ù㲥ģʽ����Ӧ��������͵�������
 �����������ԶԹ㲥ģʽ�ı仯������Ӧ ����ṩ�¼��������
 *
 * @note     The Advertising Module supports only applications with a single slave link.
 ���ģ��ֻ֧�־��е����������ӵ�Ӧ�ó���
 *
 * The application must propagate BLE stack events to this module by calling
 Ӧ�ó������ͨ�����ý�BLE��ջ�¼���������ģ��
 * @ref ble_advertising_on_ble_evt() and system events by calling �Լ�����ϵͳ�¼�
 * @ref ble_advertising_on_sys_evt().
 *
 */

#ifndef BLE_ADVERTISING_H__
#define BLE_ADVERTISING_H__

#include <stdint.h>
#include "ble_gattc.h"
#include "ble.h"
#include "nrf_error.h"
#include "ble_advdata.h"

/**@brief Advertising modes.  //�㲥ģʽ
*/
typedef enum
{
    BLE_ADV_MODE_IDLE,          /**< Idle; no connectable advertising is ongoing.*/  //���õģ�û�п����ӵĹ㲥���ڽ�����
    BLE_ADV_MODE_DIRECTED,      /**< Directed advertising attempts to connect to the most recently disconnected peer. */ //����㲥����ͼ���ӵ�����Ͽ����ӵ����豸
    BLE_ADV_MODE_DIRECTED_SLOW, /**< Directed advertising (low duty cycle) attempts to connect to the most recently disconnected peer. */ //����㲥��(�͹�������) ��ͼ���ӵ�����Ͽ����ӵĵ�
    BLE_ADV_MODE_FAST,          /**< Fast advertising will connect to any peer device, or filter with a whitelist if one exists. */ //���ٹ㲥�����ӵ��κ��豸������˻���������������
    BLE_ADV_MODE_SLOW,          /**< Slow advertising is similar to fast advertising. By default, it uses a longer advertising interval and time-out than fast advertising. However, these options are defined by the user. */
									//���㲥�����㲥���ƣ�Ĭ������£����ȿ���ʹ�ø����Ĺ㲥�������ͣʱ�䡣������Щ�������û�������
} ble_adv_mode_t;

/**@brief Advertising events.  //�㲥�¼�
 *
 * @details These events are propagated to the main application if a handler was provided during
 *          initialization of the Advertising Module. Events for modes that are not used can be
 *          ignored. Similarly, BLE_ADV_EVT_WHITELIST_REQUEST and BLE_ADV_EVT_PEER_ADDR_REQUEST
 *          can be ignored if whitelist and direct advertising is not used.
 */
typedef enum
{
    BLE_ADV_EVT_IDLE,                /**< Idle; no connectable advertising is ongoing.*/
    BLE_ADV_EVT_DIRECTED,            /**< Direct advertising mode has started. */
    BLE_ADV_EVT_DIRECTED_SLOW,       /**< Directed advertising (low duty cycle) has started. */
    BLE_ADV_EVT_FAST,                /**< Fast advertising mode has started. */
    BLE_ADV_EVT_SLOW,                /**< Slow advertising mode has started.*/
    BLE_ADV_EVT_FAST_WHITELIST,      /**< Fast advertising mode using the whitelist has started. */
    BLE_ADV_EVT_SLOW_WHITELIST,      /**< Slow advertising mode using the whitelist has started.*/
    BLE_ADV_EVT_WHITELIST_REQUEST,   /**< Request a whitelist from the main application. For whitelist advertising to work, the whitelist must be set when this event occurs. */
    BLE_ADV_EVT_PEER_ADDR_REQUEST    /**< Request a peer address from the main application. For directed advertising to work, the peer address must be set when this event occurs. */
} ble_adv_evt_t;

/**@brief Options for the different advertisement modes.
 *
 * @details This structure is used to enable or disable advertising modes and to configure time-out
 *          periods and advertising intervals.
 */
typedef struct
{
    bool     ble_adv_whitelist_enabled;       /**< Enable or disable use of the whitelist. */
    bool     ble_adv_directed_enabled;        /**< Enable or disable direct advertising mode. */
    bool     ble_adv_directed_slow_enabled;   /**< Enable or disable direct advertising mode. */
    uint32_t ble_adv_directed_slow_interval;  /**< Advertising interval for directed advertising. */
    uint32_t ble_adv_directed_slow_timeout;   /**< Time-out (number of tries) for direct advertising. */
    bool     ble_adv_fast_enabled;            /**< Enable or disable fast advertising mode. */
    uint32_t ble_adv_fast_interval;           /**< Advertising interval for fast advertising. */
    uint32_t ble_adv_fast_timeout;            /**< Time-out (in seconds) for fast advertising. */
    bool     ble_adv_slow_enabled;            /**< Enable or disable slow advertising mode. */
    uint32_t ble_adv_slow_interval;           /**< Advertising interval for slow advertising. */
    uint32_t ble_adv_slow_timeout;            /**< Time-out (in seconds) for slow advertising. */
}ble_adv_modes_config_t;


/**@brief BLE advertising event handler type. */  //BLE�㲥�¼��������
typedef void (*ble_advertising_evt_handler_t) (ble_adv_evt_t const adv_evt);

/**@brief BLE advertising error handler type. */ //BLE�㲥���������
typedef void (*ble_advertising_error_handler_t) (uint32_t nrf_error);

/**@brief Initialization parameters for the Advertising Module. //�㲥ģ���ʼ������
 * @details This structure is used to pass advertising options, advertising data, and an event handler to the Advertising Module during initialization. */ //�˽ṹ�����ڳ�ʼ���ڼ佫�㲥ѡ��㲥���ݣ����¼�������򴫵ݸ��㲥ģ��
typedef struct
{
    ble_adv_modes_config_t        options;     /**< Parameters for advertising modes.*/ //�㲥ģʽ����
    ble_advdata_t                 advdata;     /**< Advertising data. *///�㲥����
    ble_advertising_evt_handler_t evt_handler; /**< Event handler. */ //�¼��������
}ble_adv_init_t;


/* Defines to make the mode options easier to set during advertising init.*/ //����ģʽѡ��ڹ㲥��ʼ���ڼ����������
#define BLE_ADV_DIRECTED_ENABLED       true
#define BLE_ADV_DIRECTED_DISABLED      false

#define BLE_ADV_DIRECTED_SLOW_ENABLED  true
#define BLE_ADV_DIRECTED_SLOW_DISABLED false

#define BLE_ADV_FAST_ENABLED           true
#define BLE_ADV_FAST_DISABLED          false
    
#define BLE_ADV_SLOW_ENABLED           true
#define BLE_ADV_SLOW_DISABLED          false

#define BLE_ADV_WHITELIST_ENABLED      true
#define BLE_ADV_WHITELIST_DISABLED     false


/**@brief Function for handling BLE events.  //����BLE�¼��ĺ���
 *
 * @details This function must be called from the BLE stack event dispatcher for
 *          the module to handle BLE events that are relevant for the Advertising Module.
	�����BLE��ջ�¼��������е��ô˺���
 *
 * @param[in] p_ble_evt BLE stack event.  //���� BLE�ص��¼�
 */
void ble_advertising_on_ble_evt(const ble_evt_t * const p_ble_evt);


/**@brief Function for handling system events. //����ϵͳ�¼��Ĺ���
 *
 * @details This function must be called to handle system events that are relevant
 *          for the Advertising Module. Specifically, the advertising module can not use the
 *          softdevice as long as there are pending writes to the flash memory. This
 *          event handler is designed to delay advertising until there is no flash operation.
������ô˺�����������ص�ϵͳ�¼����ڹ㲥ģ�顣������˵���㲥ģ�鲻��ʹ��Э��ջ��ֻҪ���ݹ�д�����档���¼����������Ϊ���ӳٹ㲥��֪��û��flash����

 *
 * @param[in] sys_evt  System event. //ϵͳ�¼�
 */
void ble_advertising_on_sys_evt(uint32_t sys_evt);


/**@brief Function for initializing the Advertising Module. //���ڳ�ʼ���㲥ģ��ĺ���
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 *          The supplied advertising data is copied to a local structure and is manipulated
 *          depending on what advertising modes are started in @ref ble_advertising_start.
 ��������Ĺ㲥���ݲ����䴫�ݵ���ջ����������һ���ṹ���Ա��ڿ�ʼ�㲥ʱ���ݵ���ջ��
 ���ṩ�Ĺ㲥���ݱ����Ƶ����ؽṹ�в����в��� ȡ���� ble_advertising_start �������Ĺ㲥ģʽ
 *
 * @param[in] p_advdata     Advertising data: name, appearance, discovery flags, and more. //�㲥���ݣ� ���ơ���ۡ����ֱ�־��
 * @param[in] p_srdata      Scan response data: Supplement to advertising data. //ɨ����Ӧ���ݣ� ����㲥����
 * @param[in] p_config      Select which advertising modes and intervals will be utilized. // ѡ��㲥ģʽ��ʱ��
 * @param[in] evt_handler   Event handler that will be called upon advertising events. //���ڹ㲥�¼��ϵ����¼��������
 * @param[in] error_handler Error handler that will propogate internal errors to the main applications. //����������������ڲ���������Ӧ�ó���
 *
 * @retval NRF_SUCCESS If initialization was successful. Otherwise, an error code is returned. //��ʼ���ɹ����� NRF_SUCCES //���򣬽����ش������
 */
uint32_t ble_advertising_init(ble_advdata_t const                 * p_advdata,
                              ble_advdata_t const                 * p_srdata,
                              ble_adv_modes_config_t const        * p_config,
                              ble_advertising_evt_handler_t const   evt_handler,
                              ble_advertising_error_handler_t const error_handler);


/**@brief Function for starting advertising. //�����㲥����
 *
 * @details You can start advertising in any of the advertising modes that you enabled
 *          during initialization.
	//�����������õ��κι㲥ģʽ�ĳ�ʼ���ڼ俪ʼ�㲥��
 *
 * @param[in] advertising_mode  Advertising mode.  //�㲥ģʽ
 *
 * @retval @ref NRF_SUCCESS On success, else an error code indicating reason for failure. //�ɹ�����NRF_SUCCESS ���򷵻ش������
 * @retval @ref NRF_ERROR_INVALID_STATE                              
 */
uint32_t ble_advertising_start(ble_adv_mode_t advertising_mode);
uint32_t ble_advertising_stop(ble_adv_mode_t advertising_mode);
															
/**@brief Function for setting the peer address. //������������豸�ĵ�ַ
 *
 * @details The peer address must be set by the application upon receiving a  //Ӧ�ó���������յ� BLE_ADV_EVT_PEER_ADDR_REQUEST �¼�ʱ��������豸��ַ
 *          @ref BLE_ADV_EVT_PEER_ADDR_REQUEST event. Without the peer address, the directed
 *          advertising mode will not be run. //û����Ե�ַ�������ж���㲥ģʽ
 *
 * @param[in] p_peer_addr  Pointer to a peer address. //ָ����Ե�ַ��ָ��
 *
 * @retval @ref NRF_SUCCESS Successfully stored the peer address pointer in the advertising module. //�ɹ�����NRF_SUCCESS ���򷵻ش������
 * @retval @ref NRF_ERROR_INVALID_STATE If a reply was not expected.
 */
uint32_t ble_advertising_peer_addr_reply(ble_gap_addr_t * p_peer_addr);


/**@brief Function for setting a whitelist. //�������ð������ĺ���
 *
 * @details The whitelist must be set by the application upon receiving a
 *          @ref BLE_ADV_EVT_WHITELIST_REQUEST event. Without the whitelist, the whitelist
 *          advertising for fast and slow modes will not be run.
 //������������Ӧ�ó������յ� BLE_ADV_EVT_WHITELIST_REQUEST �¼�ʱ���á�û�а��������������㲥��������
 *
 * @param[in] p_whitelist  Pointer to a whitelist. //ָ���������ָ��
 *
 * @retval @ref NRF_SUCCESS Successfully stored pointers to the whitelist into the advertising module.//�ɹ�����NRF_SUCCESS ���򷵻ش������
 * @retval @ref NRF_ERROR_INVALID_STATE If a reply was not expected.
 */
uint32_t ble_advertising_whitelist_reply(ble_gap_whitelist_t * p_whitelist);


/**@brief Function for disabling whitelist advertising.  //���ð������㲥����
 *
 * @details This function temporarily disables whitelist advertising.
 *          Calling this function resets the current time-out countdown.
 *
 * @retval @ref NRF_SUCCESS On success, else an error message propogated from the Softdevice.
 */
uint32_t ble_advertising_restart_without_whitelist(void);
/** @} */

#endif // BLE_ADVERTISING_H__

/** @} */
