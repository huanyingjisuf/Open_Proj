/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**@file
 *
 * @defgroup ble_sdk_srv_nus Nordic UART Service
 * @{
 * @ingroup  ble_sdk_srv
 * @brief    Nordic UART Service implementation.  //Nordic UART 服务实现
 *
 * @details The Nordic UART Service is a simple GATT-based service with TX and RX characteristics. //Nordic UART 服务是一种简单的基于gattr的服务，具有TX和RX特性。
 *          Data received from the peer is passed to the application, and the data received // 从对等点收到的数据被传� 莞τ贸绦颍⑶掖痈梅竦挠τ贸绦蚪邮盏降氖荼�
 *          from the application of this service is sent to the peer as Handle Value //发送给对等点作为通知值。
 *          Notifications. This module demonstrates how to implement a custom GATT-based //这个模块演示了如何自定义基于gatt-based 使用S110协议栈的服务和特性
 *          service and characteristics using the S110 SoftDevice. The service
 *          is used by the application to send and receive ASCII text strings to and from the //应用程序使用它来发送和接收ASCII文本字符串。
 *          peer.
 *
 * @note The application must propagate S110 SoftDevice events to the Nordic UART Service module //应用程序必须将S110协议栈时间传播到Nordic UART 服务模块 从ble_stack_handler 回调中调用ble_nus_on_ble_evt() 函数
 *       by calling the ble_nus_on_ble_evt() function from the ble_stack_handler callback.
 */

#ifndef BLE_NUS_H__
#define BLE_NUS_H__

#include "ble.h"
#include "ble_srv_common.h"
#include <stdint.h>
#include <stdbool.h>

#define BLE_UUID_NUS_SERVICE 0x5343                      /**< The UUID of the Nordic UART Service. Nordic UART Service 的服务ID*/
#define BLE_NUS_MAX_DATA_LEN (GATT_MTU_SIZE_DEFAULT - 3) /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
//数据的最大长度(以字节为单位)，可以通过Nordic UART 服务模块传输到对等点。

/* Forward declaration of the ble_nus_t type. */ //ble_nus_t 类型的转发声明
typedef struct ble_nus_s ble_nus_t;

/**@brief Nordic UART Service event handler type. */  //Nordic UART 服务事件处理程序类型
typedef void (*ble_nus_data_handler_t) (ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);

/**@brief Nordic UART Service initialization structure. //Nordic UART 服务初始化结构
 *
 * @details This structure contains the initialization information for the service. The application //该结构包含服务的初始化信息。
 * must fill this structure and pass it to the service using the @ref ble_nus_init //应用程序必须填写此结构并使用 ble_nus_init 将其传递给服务。
 *          function.
 */
typedef struct
{
    ble_nus_data_handler_t data_handler; /**< Event handler to be called for handling received data. */
} ble_nus_init_t; //用于处理接收到的数据和事件处理程序。

/**@brief Nordic UART Service structure.  Nordic UART 服务结构。
 *
 * @details This structure contains status information related to the service. //此结构包含与服务相关的状态信息。
 */
struct ble_nus_s
{
    uint8_t                  uuid_type;               /**< UUID type for Nordic UART Service Base UUID. */ //用于Nordic UART 服务基础UUID的UUID类型。
    uint16_t                 service_handle;          /**< Handle of Nordic UART Service (as provided by the S110 SoftDevice). */ //NORDIC服务的处理(由S110协议栈提供)
    ble_gatts_char_handles_t tx_handles;              /**< Handles related to the TX characteristic (as provided by the S110 SoftDevice). */// 与TX特性相关的句柄。(由S110协议栈提供)
    ble_gatts_char_handles_t rx_handles;              /**< Handles related to the RX characteristic (as provided by the S110 SoftDevice). */ //与RX特性相关的句柄。(由S110协议栈提供)
    uint16_t                 conn_handle;             /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */ //当前连接的句柄(由S110协议栈提供)，如果不是在连接上，就会失效。
    bool                     is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*///变量来指示对等点是否启用了RX特性的通知。
    ble_nus_data_handler_t   data_handler;            /**< Event handler to be called for handling received data. */ //用于处理接收到的数据的事件处理程序。
};

/**@brief Function for initializing the Nordic UART Service. 启动Nordic UART 服务功能。
 *
 * @param[out] p_nus      Nordic UART Service structure. This structure must be supplied //Nordic UART 服务结构体，必须提供这种数据结构的应用程序，
 *                        by the application. It is initialized by this function and will//它是由这个函数初始化的,稍后将用于识别这个特定的服务实例。
 *                        later be used to identify this particular service instance.
 * @param[in] p_nus_init  Information needed to initialize the service.  //初始化服务所需的信息。
 *
 * @retval NRF_SUCCESS If the service was successfully initialized. Otherwise, an error code is returned. 服务成功初始化返回NRF_SUCCESS，否则会返回一个错误代码。
 * @retval NRF_ERROR_NULL If either of the pointers p_nus or p_nus_init is NULL. //如果有 p_nus或p_nus_init中有任何一个指针是空的。 返回 NRF_ERROR_NULL
 */
uint32_t ble_nus_init(ble_nus_t * p_nus, const ble_nus_init_t * p_nus_init);

/**@brief Function for handling the Nordic UART Service's BLE events. //用于处理Nordic UART服务的活动。
 *
 * @details The Nordic UART Service expects the application to call this function each time an //Nordic UART 服务期望应用程序每次都调用这个函数，事件是从S110协议栈接收的。
 * event is received from the S110 SoftDevice. This function processes the event if it //这个函数处理事件和Nordic Service 事件句柄是相关的，会在必要时调用。
 * is relevant and calls the Nordic UART Service event handler of the
 * application if necessary.
 *
 * @param[in] p_nus       Nordic UART Service structure.  //Nordic UART 服务结构。
 * @param[in] p_ble_evt   Event received from the S110 SoftDevice. //从S110协议栈接收的事件。
 */
void ble_nus_on_ble_evt(ble_nus_t * p_nus, ble_evt_t * p_ble_evt);

/**@brief Function for sending a string to the peer. //将字符串发送给对等点的函数。
 *
 * @details This function sends the input string as an RX characteristic notification to the //这个函数将输入字符串，作为RX特性通知发送给同行。
 *          peer.
 *
 * @param[in] p_nus       Pointer to the Nordic UART Service structure. //通过指向Nordic UART服务结构的指针。
 * @param[in] p_string    String to be sent.  //要发送字符串
 * @param[in] length      Length of the string. //字符串的长度。
 * 
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned. //如果字符串被成功发送，返回NRF_SUCCESS。否则会返回一个错误码。
 */
uint32_t ble_nus_string_send(ble_nus_t * p_nus, uint8_t * p_string, uint16_t length);

#endif // BLE_NUS_H__

/** @} */
