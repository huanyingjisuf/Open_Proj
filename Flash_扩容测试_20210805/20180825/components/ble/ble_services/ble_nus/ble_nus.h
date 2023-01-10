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
 * @brief    Nordic UART Service implementation.  //Nordic UART ·şÎñÊµÏÖ
 *
 * @details The Nordic UART Service is a simple GATT-based service with TX and RX characteristics. //Nordic UART ·şÎñÊÇÒ»ÖÖ¼òµ¥µÄ»ùÓÚgattrµÄ·şÎñ£¬¾ßÓĞTXºÍRXÌØĞÔ¡£
 *          Data received from the peer is passed to the application, and the data received // ´Ó¶ÔµÈµãÊÕµ½µÄÊı¾İ±»´«µ İ¸øÓ¦ÓÃ³ÌĞò£¬²¢ÇÒ´Ó¸Ã·şÎñµÄÓ¦ÓÃ³ÌĞò½ÓÊÕµ½µÄÊı¾İ±»
 *          from the application of this service is sent to the peer as Handle Value //·¢ËÍ¸ø¶ÔµÈµã×÷ÎªÍ¨ÖªÖµ¡£
 *          Notifications. This module demonstrates how to implement a custom GATT-based //Õâ¸öÄ£¿éÑİÊ¾ÁËÈçºÎ×Ô¶¨Òå»ùÓÚgatt-based Ê¹ÓÃS110Ğ­ÒéÕ»µÄ·şÎñºÍÌØĞÔ
 *          service and characteristics using the S110 SoftDevice. The service
 *          is used by the application to send and receive ASCII text strings to and from the //Ó¦ÓÃ³ÌĞòÊ¹ÓÃËüÀ´·¢ËÍºÍ½ÓÊÕASCIIÎÄ±¾×Ö·û´®¡£
 *          peer.
 *
 * @note The application must propagate S110 SoftDevice events to the Nordic UART Service module //Ó¦ÓÃ³ÌĞò±ØĞë½«S110Ğ­ÒéÕ»Ê±¼ä´«²¥µ½Nordic UART ·şÎñÄ£¿é ´Óble_stack_handler »Øµ÷ÖĞµ÷ÓÃble_nus_on_ble_evt() º¯Êı
 *       by calling the ble_nus_on_ble_evt() function from the ble_stack_handler callback.
 */

#ifndef BLE_NUS_H__
#define BLE_NUS_H__

#include "ble.h"
#include "ble_srv_common.h"
#include <stdint.h>
#include <stdbool.h>

#define BLE_UUID_NUS_SERVICE 0x5343                      /**< The UUID of the Nordic UART Service. Nordic UART Service µÄ·şÎñID*/
#define BLE_NUS_MAX_DATA_LEN (GATT_MTU_SIZE_DEFAULT - 3) /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
//Êı¾İµÄ×î´ó³¤¶È(ÒÔ×Ö½ÚÎªµ¥Î»)£¬¿ÉÒÔÍ¨¹ıNordic UART ·şÎñÄ£¿é´«Êäµ½¶ÔµÈµã¡£

/* Forward declaration of the ble_nus_t type. */ //ble_nus_t ÀàĞÍµÄ×ª·¢ÉùÃ÷
typedef struct ble_nus_s ble_nus_t;

/**@brief Nordic UART Service event handler type. */  //Nordic UART ·şÎñÊÂ¼ş´¦Àí³ÌĞòÀàĞÍ
typedef void (*ble_nus_data_handler_t) (ble_nus_t * p_nus, uint8_t * p_data, uint16_t length);

/**@brief Nordic UART Service initialization structure. //Nordic UART ·şÎñ³õÊ¼»¯½á¹¹
 *
 * @details This structure contains the initialization information for the service. The application //¸Ã½á¹¹°üº¬·şÎñµÄ³õÊ¼»¯ĞÅÏ¢¡£
 * must fill this structure and pass it to the service using the @ref ble_nus_init //Ó¦ÓÃ³ÌĞò±ØĞëÌîĞ´´Ë½á¹¹²¢Ê¹ÓÃ ble_nus_init ½«Æä´«µİ¸ø·şÎñ¡£
 *          function.
 */
typedef struct
{
    ble_nus_data_handler_t data_handler; /**< Event handler to be called for handling received data. */
} ble_nus_init_t; //ÓÃÓÚ´¦Àí½ÓÊÕµ½µÄÊı¾İºÍÊÂ¼ş´¦Àí³ÌĞò¡£

/**@brief Nordic UART Service structure.  Nordic UART ·şÎñ½á¹¹¡£
 *
 * @details This structure contains status information related to the service. //´Ë½á¹¹°üº¬Óë·şÎñÏà¹ØµÄ×´Ì¬ĞÅÏ¢¡£
 */
struct ble_nus_s
{
    uint8_t                  uuid_type;               /**< UUID type for Nordic UART Service Base UUID. */ //ÓÃÓÚNordic UART ·şÎñ»ù´¡UUIDµÄUUIDÀàĞÍ¡£
    uint16_t                 service_handle;          /**< Handle of Nordic UART Service (as provided by the S110 SoftDevice). */ //NORDIC·şÎñµÄ´¦Àí(ÓÉS110Ğ­ÒéÕ»Ìá¹©)
    ble_gatts_char_handles_t tx_handles;              /**< Handles related to the TX characteristic (as provided by the S110 SoftDevice). */// ÓëTXÌØĞÔÏà¹ØµÄ¾ä±ú¡£(ÓÉS110Ğ­ÒéÕ»Ìá¹©)
    ble_gatts_char_handles_t rx_handles;              /**< Handles related to the RX characteristic (as provided by the S110 SoftDevice). */ //ÓëRXÌØĞÔÏà¹ØµÄ¾ä±ú¡£(ÓÉS110Ğ­ÒéÕ»Ìá¹©)
    uint16_t                 conn_handle;             /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */ //µ±Ç°Á¬½ÓµÄ¾ä±ú(ÓÉS110Ğ­ÒéÕ»Ìá¹©)£¬Èç¹û²»ÊÇÔÚÁ¬½ÓÉÏ£¬¾Í»áÊ§Ğ§¡£
    bool                     is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*///±äÁ¿À´Ö¸Ê¾¶ÔµÈµãÊÇ·ñÆôÓÃÁËRXÌØĞÔµÄÍ¨Öª¡£
    ble_nus_data_handler_t   data_handler;            /**< Event handler to be called for handling received data. */ //ÓÃÓÚ´¦Àí½ÓÊÕµ½µÄÊı¾İµÄÊÂ¼ş´¦Àí³ÌĞò¡£
};

/**@brief Function for initializing the Nordic UART Service. Æô¶¯Nordic UART ·şÎñ¹¦ÄÜ¡£
 *
 * @param[out] p_nus      Nordic UART Service structure. This structure must be supplied //Nordic UART ·şÎñ½á¹¹Ìå£¬±ØĞëÌá¹©ÕâÖÖÊı¾İ½á¹¹µÄÓ¦ÓÃ³ÌĞò£¬
 *                        by the application. It is initialized by this function and will//ËüÊÇÓÉÕâ¸öº¯Êı³õÊ¼»¯µÄ,ÉÔºó½«ÓÃÓÚÊ¶±ğÕâ¸öÌØ¶¨µÄ·şÎñÊµÀı¡£
 *                        later be used to identify this particular service instance.
 * @param[in] p_nus_init  Information needed to initialize the service.  //³õÊ¼»¯·şÎñËùĞèµÄĞÅÏ¢¡£
 *
 * @retval NRF_SUCCESS If the service was successfully initialized. Otherwise, an error code is returned. ·şÎñ³É¹¦³õÊ¼»¯·µ»ØNRF_SUCCESS£¬·ñÔò»á·µ»ØÒ»¸ö´íÎó´úÂë¡£
 * @retval NRF_ERROR_NULL If either of the pointers p_nus or p_nus_init is NULL. //Èç¹ûÓĞ p_nus»òp_nus_initÖĞÓĞÈÎºÎÒ»¸öÖ¸ÕëÊÇ¿ÕµÄ¡£ ·µ»Ø NRF_ERROR_NULL
 */
uint32_t ble_nus_init(ble_nus_t * p_nus, const ble_nus_init_t * p_nus_init);

/**@brief Function for handling the Nordic UART Service's BLE events. //ÓÃÓÚ´¦ÀíNordic UART·şÎñµÄ»î¶¯¡£
 *
 * @details The Nordic UART Service expects the application to call this function each time an //Nordic UART ·şÎñÆÚÍûÓ¦ÓÃ³ÌĞòÃ¿´Î¶¼µ÷ÓÃÕâ¸öº¯Êı£¬ÊÂ¼şÊÇ´ÓS110Ğ­ÒéÕ»½ÓÊÕµÄ¡£
 * event is received from the S110 SoftDevice. This function processes the event if it //Õâ¸öº¯Êı´¦ÀíÊÂ¼şºÍNordic Service ÊÂ¼ş¾ä±úÊÇÏà¹ØµÄ£¬»áÔÚ±ØÒªÊ±µ÷ÓÃ¡£
 * is relevant and calls the Nordic UART Service event handler of the
 * application if necessary.
 *
 * @param[in] p_nus       Nordic UART Service structure.  //Nordic UART ·şÎñ½á¹¹¡£
 * @param[in] p_ble_evt   Event received from the S110 SoftDevice. //´ÓS110Ğ­ÒéÕ»½ÓÊÕµÄÊÂ¼ş¡£
 */
void ble_nus_on_ble_evt(ble_nus_t * p_nus, ble_evt_t * p_ble_evt);

/**@brief Function for sending a string to the peer. //½«×Ö·û´®·¢ËÍ¸ø¶ÔµÈµãµÄº¯Êı¡£
 *
 * @details This function sends the input string as an RX characteristic notification to the //Õâ¸öº¯Êı½«ÊäÈë×Ö·û´®£¬×÷ÎªRXÌØĞÔÍ¨Öª·¢ËÍ¸øÍ¬ĞĞ¡£
 *          peer.
 *
 * @param[in] p_nus       Pointer to the Nordic UART Service structure. //Í¨¹ıÖ¸ÏòNordic UART·şÎñ½á¹¹µÄÖ¸Õë¡£
 * @param[in] p_string    String to be sent.  //Òª·¢ËÍ×Ö·û´®
 * @param[in] length      Length of the string. //×Ö·û´®µÄ³¤¶È¡£
 * 
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned. //Èç¹û×Ö·û´®±»³É¹¦·¢ËÍ£¬·µ»ØNRF_SUCCESS¡£·ñÔò»á·µ»ØÒ»¸ö´íÎóÂë¡£
 */
uint32_t ble_nus_string_send(ble_nus_t * p_nus, uint8_t * p_string, uint16_t length);

#endif // BLE_NUS_H__

/** @} */
