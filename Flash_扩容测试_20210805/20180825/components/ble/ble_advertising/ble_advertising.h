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
 * @defgroup ble_sdk_lib_advertising Advertising Module  //广播模块
 * @{
 * @ingroup  ble_sdk_lib
 * @brief    Module for handling connectable BLE advertising. 处理可连接BLE广播的模块
 *
 * @details  The Advertising Module handles connectable advertising for your application. It can 
 *           be configured with advertising modes to suit most typical use cases.
 *           Your main application can react to changes in advertising modes
 *           if an event handler is provided.
 广播模块处理应用程序的可连接广播。它可以配置广播模式以适应大多数典型的用例。
 你的主程序可以对广播模式的变化做出反应 如果提供事件处理程序
 *
 * @note     The Advertising Module supports only applications with a single slave link.
 广告模块只支持具有单个从属链接的应用程序。
 *
 * The application must propagate BLE stack events to this module by calling
 应用程序必须通过调用将BLE堆栈事件传播到此模块
 * @ref ble_advertising_on_ble_evt() and system events by calling 以及调用系统事件
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

/**@brief Advertising modes.  //广播模式
*/
typedef enum
{
    BLE_ADV_MODE_IDLE,          /**< Idle; no connectable advertising is ongoing.*/  //闲置的，没有可连接的广播正在进行中
    BLE_ADV_MODE_DIRECTED,      /**< Directed advertising attempts to connect to the most recently disconnected peer. */ //定向广播，试图连接到最近断开连接的主设备
    BLE_ADV_MODE_DIRECTED_SLOW, /**< Directed advertising (low duty cycle) attempts to connect to the most recently disconnected peer. */ //定向广播，(低工作周期) 试图连接到最近断开连接的点
    BLE_ADV_MODE_FAST,          /**< Fast advertising will connect to any peer device, or filter with a whitelist if one exists. */ //快速广播讲连接到任何设备，或过滤或白名单，如果存在
    BLE_ADV_MODE_SLOW,          /**< Slow advertising is similar to fast advertising. By default, it uses a longer advertising interval and time-out than fast advertising. However, these options are defined by the user. */
									//慢广播，与快广播相似，默认情况下，它比快速使用更长的广播间隔和暂停时间。但是这些都是由用户决定的
} ble_adv_mode_t;

/**@brief Advertising events.  //广播事件
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


/**@brief BLE advertising event handler type. */  //BLE广播事件处理程序
typedef void (*ble_advertising_evt_handler_t) (ble_adv_evt_t const adv_evt);

/**@brief BLE advertising error handler type. */ //BLE广播错误处理程序
typedef void (*ble_advertising_error_handler_t) (uint32_t nrf_error);

/**@brief Initialization parameters for the Advertising Module. //广播模块初始化参数
 * @details This structure is used to pass advertising options, advertising data, and an event handler to the Advertising Module during initialization. */ //此结构用于在初始化期间将广播选项、广播数据，和事件处理程序传递给广播模块
typedef struct
{
    ble_adv_modes_config_t        options;     /**< Parameters for advertising modes.*/ //广播模式参数
    ble_advdata_t                 advdata;     /**< Advertising data. *///广播数据
    ble_advertising_evt_handler_t evt_handler; /**< Event handler. */ //事件处理程序
}ble_adv_init_t;


/* Defines to make the mode options easier to set during advertising init.*/ //定义模式选项，在广播初始化期间更容易设置
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


/**@brief Function for handling BLE events.  //处理BLE事件的函数
 *
 * @details This function must be called from the BLE stack event dispatcher for
 *          the module to handle BLE events that are relevant for the Advertising Module.
	必须从BLE堆栈事件调度器中调用此函数
 *
 * @param[in] p_ble_evt BLE stack event.  //输入 BLE回调事件
 */
void ble_advertising_on_ble_evt(const ble_evt_t * const p_ble_evt);


/**@brief Function for handling system events. //处理系统事件的功能
 *
 * @details This function must be called to handle system events that are relevant
 *          for the Advertising Module. Specifically, the advertising module can not use the
 *          softdevice as long as there are pending writes to the flash memory. This
 *          event handler is designed to delay advertising until there is no flash operation.
必须调用此函数来处理相关的系统事件用于广播模块。具体来说，广播模块不能使用协议栈，只要有暂挂写入闪存。这事件处理程序是为了延迟广播，知道没有flash操作

 *
 * @param[in] sys_evt  System event. //系统事件
 */
void ble_advertising_on_sys_evt(uint32_t sys_evt);


/**@brief Function for initializing the Advertising Module. //用于初始化广播模块的函数
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 *          The supplied advertising data is copied to a local structure and is manipulated
 *          depending on what advertising modes are started in @ref ble_advertising_start.
 输入所需的广播数据并将其传递到堆栈。还构建了一个结构，以便在开始广播时传递到堆栈。
 所提供的广播数据被复制到本地结构中并进行操作 取决于 ble_advertising_start 中启动的广播模式
 *
 * @param[in] p_advdata     Advertising data: name, appearance, discovery flags, and more. //广播数据： 名称、外观、发现标志等
 * @param[in] p_srdata      Scan response data: Supplement to advertising data. //扫描响应数据： 补充广播数据
 * @param[in] p_config      Select which advertising modes and intervals will be utilized. // 选择广播模式和时段
 * @param[in] evt_handler   Event handler that will be called upon advertising events. //将在广播事件上调用事件处理程序。
 * @param[in] error_handler Error handler that will propogate internal errors to the main applications. //错误处理程序，它将把内部错误导入主应用程序
 *
 * @retval NRF_SUCCESS If initialization was successful. Otherwise, an error code is returned. //初始化成功返回 NRF_SUCCES //否则，将返回错误代码
 */
uint32_t ble_advertising_init(ble_advdata_t const                 * p_advdata,
                              ble_advdata_t const                 * p_srdata,
                              ble_adv_modes_config_t const        * p_config,
                              ble_advertising_evt_handler_t const   evt_handler,
                              ble_advertising_error_handler_t const error_handler);


/**@brief Function for starting advertising. //启动广播功能
 *
 * @details You can start advertising in any of the advertising modes that you enabled
 *          during initialization.
	//您可以在启用的任何广播模式的初始化期间开始广播。
 *
 * @param[in] advertising_mode  Advertising mode.  //广播模式
 *
 * @retval @ref NRF_SUCCESS On success, else an error code indicating reason for failure. //成功返回NRF_SUCCESS 否则返回错误代码
 * @retval @ref NRF_ERROR_INVALID_STATE                              
 */
uint32_t ble_advertising_start(ble_adv_mode_t advertising_mode);
uint32_t ble_advertising_stop(ble_adv_mode_t advertising_mode);
															
/**@brief Function for setting the peer address. //用于设置配对设备的地址
 *
 * @details The peer address must be set by the application upon receiving a  //应用程序必须在收到 BLE_ADV_EVT_PEER_ADDR_REQUEST 事件时设置配对设备地址
 *          @ref BLE_ADV_EVT_PEER_ADDR_REQUEST event. Without the peer address, the directed
 *          advertising mode will not be run. //没有配对地址，则不运行定向广播模式
 *
 * @param[in] p_peer_addr  Pointer to a peer address. //指向配对地址的指针
 *
 * @retval @ref NRF_SUCCESS Successfully stored the peer address pointer in the advertising module. //成功返回NRF_SUCCESS 否则返回错误代码
 * @retval @ref NRF_ERROR_INVALID_STATE If a reply was not expected.
 */
uint32_t ble_advertising_peer_addr_reply(ble_gap_addr_t * p_peer_addr);


/**@brief Function for setting a whitelist. //用于设置白名单的函数
 *
 * @details The whitelist must be set by the application upon receiving a
 *          @ref BLE_ADV_EVT_WHITELIST_REQUEST event. Without the whitelist, the whitelist
 *          advertising for fast and slow modes will not be run.
 //白名单必须由应用程序在收到 BLE_ADV_EVT_WHITELIST_REQUEST 事件时设置。没有白名单，白名单广播不会运行
 *
 * @param[in] p_whitelist  Pointer to a whitelist. //指向白名单的指针
 *
 * @retval @ref NRF_SUCCESS Successfully stored pointers to the whitelist into the advertising module.//成功返回NRF_SUCCESS 否则返回错误代码
 * @retval @ref NRF_ERROR_INVALID_STATE If a reply was not expected.
 */
uint32_t ble_advertising_whitelist_reply(ble_gap_whitelist_t * p_whitelist);


/**@brief Function for disabling whitelist advertising.  //禁用白名单广播功能
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
