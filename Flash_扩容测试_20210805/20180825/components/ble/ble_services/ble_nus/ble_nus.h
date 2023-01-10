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
 * @brief    Nordic UART Service implementation.  //Nordic UART ����ʵ��
 *
 * @details The Nordic UART Service is a simple GATT-based service with TX and RX characteristics. //Nordic UART ������һ�ּ򵥵Ļ���gattr�ķ��񣬾���TX��RX���ԡ�
 *          Data received from the peer is passed to the application, and the data received // �ӶԵȵ��յ������ݱ���� ݸ�Ӧ�ó��򣬲��ҴӸ÷����Ӧ�ó�����յ������ݱ�
 *          from the application of this service is sent to the peer as Handle Value //���͸��Եȵ���Ϊֵ֪ͨ��
 *          Notifications. This module demonstrates how to implement a custom GATT-based //���ģ����ʾ������Զ������gatt-based ʹ��S110Э��ջ�ķ��������
 *          service and characteristics using the S110 SoftDevice. The service
 *          is used by the application to send and receive ASCII text strings to and from the //Ӧ�ó���ʹ���������ͺͽ���ASCII�ı��ַ�����
 *          peer.
 *
 * @note The application must propagate S110 SoftDevice events to the Nordic UART Service module //Ӧ�ó�����뽫S110Э��ջʱ�䴫����Nordic UART ����ģ�� ��ble_stack_handler �ص��е���ble_nus_on_ble_evt() ����
 *       by calling the ble_nus_on_ble_evt() function from the ble_stack_handler callback.
 */

#ifndef BLE_NUS_H__
#define BLE_NUS_H__

#include "ble.h"
#include "ble_srv_common.h"
#include <stdint.h>
#include <stdbool.h>

#define BLE_UUID_NUS_SERVICE 0x5343                      /**< The UUID of the Nordic UART Service. Nordic UART Service �ķ���ID*/
#define BLE_NUS_MAX_DATA_LEN (GATT_MTU_SIZE_DEFAULT - 3) /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
//���ݵ���󳤶�(���ֽ�Ϊ��λ)������ͨ��Nordic UART ����ģ�鴫�䵽�Եȵ㡣

/* Forward declaration of the ble_nus_t type. */ //ble_nus_t ���͵�ת������
typedef struct ble_nus_s ble_nus_t;

/**@brief Nordic UART Service event handler type. */  //Nordic UART �����¼������������
typedef void (*ble_nus_data_handler_t) (ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);

/**@brief Nordic UART Service initialization structure. //Nordic UART �����ʼ���ṹ
 *
 * @details This structure contains the initialization information for the service. The application //�ýṹ��������ĳ�ʼ����Ϣ��
 * must fill this structure and pass it to the service using the @ref ble_nus_init //Ӧ�ó��������д�˽ṹ��ʹ�� ble_nus_init ���䴫�ݸ�����
 *          function.
 */
typedef struct
{
    ble_nus_data_handler_t data_handler; /**< Event handler to be called for handling received data. */
} ble_nus_init_t; //���ڴ�����յ������ݺ��¼��������

/**@brief Nordic UART Service structure.  Nordic UART ����ṹ��
 *
 * @details This structure contains status information related to the service. //�˽ṹ�����������ص�״̬��Ϣ��
 */
struct ble_nus_s
{
    uint8_t                  uuid_type;               /**< UUID type for Nordic UART Service Base UUID. */ //����Nordic UART �������UUID��UUID���͡�
    uint16_t                 service_handle;          /**< Handle of Nordic UART Service (as provided by the S110 SoftDevice). */ //NORDIC����Ĵ���(��S110Э��ջ�ṩ)
    ble_gatts_char_handles_t tx_handles;              /**< Handles related to the TX characteristic (as provided by the S110 SoftDevice). */// ��TX������صľ����(��S110Э��ջ�ṩ)
    ble_gatts_char_handles_t rx_handles;              /**< Handles related to the RX characteristic (as provided by the S110 SoftDevice). */ //��RX������صľ����(��S110Э��ջ�ṩ)
    uint16_t                 conn_handle;             /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */ //��ǰ���ӵľ��(��S110Э��ջ�ṩ)����������������ϣ��ͻ�ʧЧ��
    bool                     is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*///������ָʾ�Եȵ��Ƿ�������RX���Ե�֪ͨ��
    ble_nus_data_handler_t   data_handler;            /**< Event handler to be called for handling received data. */ //���ڴ�����յ������ݵ��¼��������
};

/**@brief Function for initializing the Nordic UART Service. ����Nordic UART �����ܡ�
 *
 * @param[out] p_nus      Nordic UART Service structure. This structure must be supplied //Nordic UART ����ṹ�壬�����ṩ�������ݽṹ��Ӧ�ó���
 *                        by the application. It is initialized by this function and will//���������������ʼ����,�Ժ�����ʶ������ض��ķ���ʵ����
 *                        later be used to identify this particular service instance.
 * @param[in] p_nus_init  Information needed to initialize the service.  //��ʼ�������������Ϣ��
 *
 * @retval NRF_SUCCESS If the service was successfully initialized. Otherwise, an error code is returned. ����ɹ���ʼ������NRF_SUCCESS������᷵��һ��������롣
 * @retval NRF_ERROR_NULL If either of the pointers p_nus or p_nus_init is NULL. //����� p_nus��p_nus_init�����κ�һ��ָ���ǿյġ� ���� NRF_ERROR_NULL
 */
uint32_t ble_nus_init(ble_nus_t * p_nus, const ble_nus_init_t * p_nus_init);

/**@brief Function for handling the Nordic UART Service's BLE events. //���ڴ���Nordic UART����Ļ��
 *
 * @details The Nordic UART Service expects the application to call this function each time an //Nordic UART ��������Ӧ�ó���ÿ�ζ���������������¼��Ǵ�S110Э��ջ���յġ�
 * event is received from the S110 SoftDevice. This function processes the event if it //������������¼���Nordic Service �¼��������صģ����ڱ�Ҫʱ���á�
 * is relevant and calls the Nordic UART Service event handler of the
 * application if necessary.
 *
 * @param[in] p_nus       Nordic UART Service structure.  //Nordic UART ����ṹ��
 * @param[in] p_ble_evt   Event received from the S110 SoftDevice. //��S110Э��ջ���յ��¼���
 */
void ble_nus_on_ble_evt(ble_nus_t * p_nus, ble_evt_t * p_ble_evt);

/**@brief Function for sending a string to the peer. //���ַ������͸��Եȵ�ĺ�����
 *
 * @details This function sends the input string as an RX characteristic notification to the //��������������ַ�������ΪRX����֪ͨ���͸�ͬ�С�
 *          peer.
 *
 * @param[in] p_nus       Pointer to the Nordic UART Service structure. //ͨ��ָ��Nordic UART����ṹ��ָ�롣
 * @param[in] p_string    String to be sent.  //Ҫ�����ַ���
 * @param[in] length      Length of the string. //�ַ����ĳ��ȡ�
 * 
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned. //����ַ������ɹ����ͣ�����NRF_SUCCESS������᷵��һ�������롣
 */
uint32_t ble_nus_string_send(ble_nus_t * p_nus, uint8_t * p_string, uint16_t length);

#endif // BLE_NUS_H__

/** @} */
