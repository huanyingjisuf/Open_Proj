/* 
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.6
 * 
 *   4. This software must only be used in a processor manufactured by Nordic
 *   Semiconductor ASA, or in a processor manufactured by a third party that
 *   is used in combination with a processor manufactured by Nordic Semiconductor.
 * 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/**
  @addtogroup BLE_GAP Generic Access Profile (GAP) //ͨ�ý���淶
  @{
  @brief Definitions and prototypes for the GAP interface. // GAP�ӿڵĶ����ģ��
 */

#ifndef BLE_GAP_H__
#define BLE_GAP_H__

#include "ble_types.h"
#include "ble_ranges.h"
#include "nrf_svc.h"

/**@addtogroup BLE_GAP_ENUMERATIONS Enumerations //ö��
 * @{ */

/**@brief GAP API SVC numbers. //GAP API SVC ���
 */
enum BLE_GAP_SVCS
{
  SD_BLE_GAP_ADDRESS_SET  = BLE_GAP_SVC_BASE,  /**< Set own Bluetooth Address. */ //�����Լ���������ַ
  SD_BLE_GAP_ADDRESS_GET,                      /**< Get own Bluetooth Address. */ //�õ��Լ���������ַ
  SD_BLE_GAP_ADV_DATA_SET,                     /**< Set Advertising Data. */ // ���ù㲥����
  SD_BLE_GAP_ADV_START,                        /**< Start Advertising. */ //��ʼ�㲥
  SD_BLE_GAP_ADV_STOP,                         /**< Stop Advertising. */ //ֹͣ�㲥
  SD_BLE_GAP_CONN_PARAM_UPDATE,                /**< Connection Parameter Update. */ //���Ӳ�������
  SD_BLE_GAP_DISCONNECT,                       /**< Disconnect. */ //�Ͽ�
  SD_BLE_GAP_TX_POWER_SET,                     /**< Set TX Power. */ //���� ���͹���
  SD_BLE_GAP_APPEARANCE_SET,                   /**< Set Appearance. */ //�������ֵ
  SD_BLE_GAP_APPEARANCE_GET,                   /**< Get Appearance. */ //��ȡ���ֵ
  SD_BLE_GAP_PPCP_SET,                         /**< Set PPCP. */ //���� PPCP
  SD_BLE_GAP_PPCP_GET,                         /**< Get PPCP. */ //��ȡ PPCP
  SD_BLE_GAP_DEVICE_NAME_SET,                  /**< Set Device Name. */ //�����豸��
  SD_BLE_GAP_DEVICE_NAME_GET,                  /**< Get Device Name. */ //��ȡ�豸�� 
  SD_BLE_GAP_AUTHENTICATE,                     /**< Initiate Pairing/Bonding. */ //�������/��
  SD_BLE_GAP_SEC_PARAMS_REPLY,                 /**< Reply with Security Parameters. */ //�ظ��밲ȫ����
  SD_BLE_GAP_AUTH_KEY_REPLY,                   /**< Reply with an authentication key. */ //�������֤��Կ�ظ�
  SD_BLE_GAP_ENCRYPT,                          /**< Initiate encryption procedure. */ //�������ܳ���
  SD_BLE_GAP_SEC_INFO_REPLY,                   /**< Reply with Security Information. */ //�԰�ȫ��Ϣ�ظ�
  SD_BLE_GAP_CONN_SEC_GET,                     /**< Obtain connection security level. */ // ������ӵİ�ȫ����
  SD_BLE_GAP_RSSI_START,                       /**< Start reporting of changes in RSSI. */  //��ʼ����RSSI�ı仯
  SD_BLE_GAP_RSSI_STOP,                        /**< Stop reporting of changes in RSSI. */  //ֹͣ����RSSI�ı仯
  SD_BLE_GAP_SCAN_START,                       /**< Start Scanning. */ //��ʼɨ��
  SD_BLE_GAP_SCAN_STOP,                        /**< Stop Scanning. */ //ֹͣɨ��
  SD_BLE_GAP_CONNECT,                          /**< Connect. */ //����
  SD_BLE_GAP_CONNECT_CANCEL,                   /**< Cancel ongoing connection procedure. */ //ȡ�����ڽ��е����ӹ���
  SD_BLE_GAP_RSSI_GET,                         /**< Get the last RSSI sample. */ //�õ����һ�� RSSI����
};

/**@brief GAP Event IDs. //GAP �¼� IDs.
 * IDs that uniquely identify an event coming from the stack to the application. //ʶ��Ӷ�ջ��Ӧ�ó�����¼�ID��
 */
enum BLE_GAP_EVTS
{
  BLE_GAP_EVT_CONNECTED  = BLE_GAP_EVT_BASE,    /**< Connection established. @ref ble_gap_evt_connected_t */ //���ӽ��� ble_gap_et_connected_t
  BLE_GAP_EVT_DISCONNECTED,                     /**< Disconnected from peer. @ref ble_gap_evt_disconnected_t */ //�Ͽ����� ble_gap_evt_disconnected_t
  BLE_GAP_EVT_CONN_PARAM_UPDATE,                /**< Connection Parameters updated. ble_gap_evt_conn_param_update_t */ //���Ӳ������¡� ble_gap_evt_conn_param_update_t
  BLE_GAP_EVT_SEC_PARAMS_REQUEST,               /**< Request to provide security parameters. @ref ble_gap_evt_sec_params_request_t */ //�����ṩ��ȫ������ ble_gap_evt_sec_params_request_t
  BLE_GAP_EVT_SEC_INFO_REQUEST,                 /**< Request to provide security information. @ref ble_gap_evt_sec_info_request_t */ //�����ṩ��ȫ��Ϣ��ble_gap_evt_sec_info_request_t
  BLE_GAP_EVT_PASSKEY_DISPLAY,                  /**< Request to display a passkey to the user. @ref ble_gap_evt_passkey_display_t */ //�������û���ʾһ���������� ble_gap_evt_passkey_display_t
  BLE_GAP_EVT_AUTH_KEY_REQUEST,                 /**< Request to provide an authentication key. @ref ble_gap_evt_auth_key_request_t */ //�����ṩ�����֤��Կ��ble_gap_evt_auth_key_request_t
  BLE_GAP_EVT_AUTH_STATUS,                      /**< Authentication procedure completed with status. @ref ble_gap_evt_auth_status_t */ // �����֤������ɡ� ble_gap_evt_auth_status_t
  BLE_GAP_EVT_CONN_SEC_UPDATE,                  /**< Connection security updated. @ref ble_gap_evt_conn_sec_update_t */ //���Ӱ�ȫ���¡� ble_gap_evt_conn_sec_update_t
  BLE_GAP_EVT_TIMEOUT,                          /**< Timeout expired. @ref ble_gap_evt_timeout_t */ //��ʱ���ڡ� ble_gap_evt_timeout_t
  BLE_GAP_EVT_RSSI_CHANGED,                     /**< RSSI report. @ref ble_gap_evt_rssi_changed_t */ //RSSI���� ble_gap_evt_rssi_changed_t 
  BLE_GAP_EVT_ADV_REPORT,                       /**< Advertising report. @ref ble_gap_evt_adv_report_t */  // �㲥�ı��� ble_gap_evt_adv_rport_t
  BLE_GAP_EVT_SEC_REQUEST,                      /**< Security Request. @ref ble_gap_evt_sec_request_t */ //��ȫҪ�� ble_gap_evt_sec_request_t
  BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST,        /**< Connection Parameter Update Request. @ref ble_gap_evt_conn_param_update_request_t */ //���Ӳ����������� ble_gap_evt_con_praam_update_request_t
  BLE_GAP_EVT_SCAN_REQ_REPORT,                  /**< Scan request report. @ref ble_gap_evt_scan_req_report_t */ //ɨ�豨������ ble_gap_evt_scan_req_report_t
};

/**@brief GAP Option IDs.  GAP ѡ��ID
 * IDs that uniquely identify a GAP option. //ʶ��հ�ѡ���ID
 */
enum BLE_GAP_OPTS
{
  BLE_GAP_OPT_CH_MAP  = BLE_GAP_OPT_BASE,       /**< Channel Map. @ref ble_gap_opt_ch_map_t  */ //ͨ����ͼ�� ble_gap_opt_ch_map_t
  BLE_GAP_OPT_LOCAL_CONN_LATENCY,               /**< Local connection latency. @ref ble_gap_opt_local_conn_latency_t */ //���������ӳ� ble_gap_opt_local_conn_latency_t
  BLE_GAP_OPT_PASSKEY,                          /**< Set passkey. @ref ble_gap_opt_passkey_t */ //��������Կ�� ble_gap_opt_passkey_t
  BLE_GAP_OPT_PRIVACY,                          /**< Custom privacy. @ref ble_gap_opt_privacy_t */ //�Զ�����˽ ble_gap_opt_privacy_t
  BLE_GAP_OPT_SCAN_REQ_REPORT,                  /**< Scan request report. @ref ble_gap_opt_scan_req_report_t */ //ɨ�豨������ ble_gap_opt_scan_req_report_t
  BLE_GAP_OPT_COMPAT_MODE                       /**< Compatibility mode. @ref ble_gap_opt_compat_mode_t */ //����ģʽ�� ble_gap_opt_compat_mode_t
};
/** @} */

/**@addtogroup BLE_GAP_DEFINES Defines //BLE_GAP_DEFINES ����
 * @{ */

/**@defgroup BLE_ERRORS_GAP SVC return values specific to GAP //SVC �����ض��� GAP  
 * @{ */ 
#define BLE_ERROR_GAP_UUID_LIST_MISMATCH            (NRF_GAP_ERR_BASE + 0x000)  /**< UUID list does not contain an integral number of UUIDs. */ //UUID�б�����UUIDs����������
#define BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST   (NRF_GAP_ERR_BASE + 0x001)  /**< Use of Whitelist not permitted with discoverable advertising. */ //������ʹ�ÿɷ��ֹ㲥�İ�����
#define BLE_ERROR_GAP_INVALID_BLE_ADDR              (NRF_GAP_ERR_BASE + 0x002)  /**< The upper two bits of the address do not correspond to the specified address type. */ //��ַ�ϵ�����������ָ���ĵ�ַ���Ͳ���Ӧ
/**@} */


/**@defgroup BLE_GAP_ROLES GAP Roles //GAP ��ɫ
 * @note Not explicitly used in peripheral API, but will be relevant for central API. //����ΧAPI��û����ʾʹ�ã�����������API���
 * @{ */
#define BLE_GAP_ROLE_INVALID     0x0            /**< Invalid Role. */ //��Ч��ɫ
#define BLE_GAP_ROLE_PERIPH      0x1            /**< Peripheral Role. */ //��Χ��ɫ
#define BLE_GAP_ROLE_CENTRAL     0x2            /**< Central Role. */ //��Ҫ��ɫ
/**@} */


/**@defgroup BLE_GAP_TIMEOUT_SOURCES GAP Timeout sources GAP ��ʱ��Դ
 * @{ */
#define BLE_GAP_TIMEOUT_SRC_ADVERTISING                0x00 /**< Advertising timeout. */ //�㲥��ʱ
#define BLE_GAP_TIMEOUT_SRC_SECURITY_REQUEST           0x01 /**< Security request timeout. */ //��ȫ����ʱ
#define BLE_GAP_TIMEOUT_SRC_SCAN                       0x02 /**< Scanning timeout. */ //ɨ�賬ʱ
#define BLE_GAP_TIMEOUT_SRC_CONN                       0x03 /**< Connection timeout. */ //���ӳ�ʱ
/**@} */


/**@defgroup BLE_GAP_ADDR_TYPES GAP Address types GAP ��ַ����
 * @{ */
#define BLE_GAP_ADDR_TYPE_PUBLIC                        0x00 /**< Public address. */ //������ַ
#define BLE_GAP_ADDR_TYPE_RANDOM_STATIC                 0x01 /**< Random Static address. */ //�����̬��ַ
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE     0x02 /**< Private Resolvable address. */ //˽�˵�ַ����
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE 0x03 /**< Private Non-Resolvable address. */ //˽�� Non-Resolvable ��ַ
/**@} */

/**@defgroup BLE_GAP_ADDR_CYCLE_MODES GAP Address cycle modes GAP ��ַ ѭ��ģʽ
 * @{ */
#define BLE_GAP_ADDR_CYCLE_MODE_NONE      0x00 /**< Set addresses directly, no automatic address cycling. */ //ֱ�����õ�ַ��û���Զ���ַѭ��
#define BLE_GAP_ADDR_CYCLE_MODE_AUTO      0x01 /**< Automatically generate and update private addresses. */ //�Զ����ɺ͸���˽�е�ַ
/** @} */

/**@brief The default interval in seconds at which a private address is refreshed when address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO.  */ // ����ַѭ��ģʽ�£�һ��˽�е�ַ��ˢ�µ�Ĭ���¼������ BLE_GAP_ADDR_CYCLE_MODE_AUTO
#define BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S (60 * 15)

/** @brief BLE address length. */
#define BLE_GAP_ADDR_LEN            6


/**@defgroup BLE_GAP_AD_TYPE_DEFINITIONS GAP Advertising and Scan Response Data format //GAP�㲥��ɨ����Ӧ���ݸ�ʽ
 * @note Found at https://www.bluetooth.org/Technical/AssignedNumbers/generic_access_profile.htm 
 * @{ */
#define BLE_GAP_AD_TYPE_FLAGS                               0x01 /**< Flags for discoverability. */ //��ǵĿɷ�����
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE   0x02 /**< Partial list of 16 bit service UUIDs. */ //16λ����UUIDs�Ĳ����б�
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE         0x03 /**< Complete list of 16 bit service UUIDs. */ // ������16λ����UUIDs�б�
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE   0x04 /**< Partial list of 32 bit service UUIDs. */ //32λ����UUID�Ĳ����б�
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_COMPLETE         0x05 /**< Complete list of 32 bit service UUIDs. */ //������32λ����UUIDs�б�
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE  0x06 /**< Partial list of 128 bit service UUIDs. */ //128λ����UUIDs�Ĳ����б�
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE        0x07 /**< Complete list of 128 bit service UUIDs. *///������128λ����UUIDs�б�
#define BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME                    0x08 /**< Short local device name. */ //�����豸�Ķ���
#define BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME                 0x09 /**< Complete local device name. */ //���������豸��
#define BLE_GAP_AD_TYPE_TX_POWER_LEVEL                      0x0A /**< Transmit power level. */ //���书��
#define BLE_GAP_AD_TYPE_CLASS_OF_DEVICE                     0x0D /**< Class of device. */ //�豸����
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C               0x0E /**< Simple Pairing Hash C. */ //����򵥵�ɢ�� C
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R         0x0F /**< Simple Pairing Randomizer R. */ //�򵥵����������������� R
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_TK_VALUE           0x10 /**< Security Manager TK Value. */ //��ȫ������ TKֵ
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_OOB_FLAGS          0x11 /**< Security Manager Out Of Band Flags. */ //��ȫ����ͬƵ���źŴ���λ
#define BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE     0x12 /**< Slave Connection Interval Range. */ //�ӻ��������䷶Χ
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_16BIT       0x14 /**< List of 16-bit Service Solicitation UUIDs. */ //16λ ��������UUID�б�
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_128BIT      0x15 /**< List of 128-bit Service Solicitation UUIDs. */ //128λ �������� UUID�б�
#define BLE_GAP_AD_TYPE_SERVICE_DATA                        0x16 /**< Service Data - 16-bit UUID. */ //16λ UUID ��������
#define BLE_GAP_AD_TYPE_PUBLIC_TARGET_ADDRESS               0x17 /**< Public Target Address. */ //����Ŀ���ַ
#define BLE_GAP_AD_TYPE_RANDOM_TARGET_ADDRESS               0x18 /**< Random Target Address. */ //���Ŀ���ַ
#define BLE_GAP_AD_TYPE_APPEARANCE                          0x19 /**< Appearance. */ //���
#define BLE_GAP_AD_TYPE_ADVERTISING_INTERVAL                0x1A /**< Advertising Interval. */ //�㲥ʱ����
#define BLE_GAP_AD_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS         0x1B /**< LE Bluetooth Device Address. */ //LE�����豸��ַ
#define BLE_GAP_AD_TYPE_LE_ROLE                             0x1C /**< LE Role. */ //LE��ɫ
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C256            0x1D /**< Simple Pairing Hash C-256. */ //����� ɢ�� C-256
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R256      0x1E /**< Simple Pairing Randomizer R-256. */ //����� ����������� R-256
#define BLE_GAP_AD_TYPE_SERVICE_DATA_32BIT_UUID             0x20 /**< Service Data - 32-bit UUID. */ //�������� 32 bit UUID
#define BLE_GAP_AD_TYPE_SERVICE_DATA_128BIT_UUID            0x21 /**< Service Data - 128-bit UUID. */ //�������� 128 bit UUID
#define BLE_GAP_AD_TYPE_3D_INFORMATION_DATA                 0x3D /**< 3D Information Data. */ // 3D ��Ϣ����
#define BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA          0xFF /**< Manufacturer Specific Data. */ // �������ض�������
/**@} */


/**@defgroup BLE_GAP_ADV_FLAGS GAP Advertisement Flags //GAP �㲥 ��־
 * @{ */
#define BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE         (0x01)   /**< LE Limited Discoverable Mode. */ //LE ���޷���ģʽ
#define BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE         (0x02)   /**< LE General Discoverable Mode. */ //LE һ�㷢��ģʽ
#define BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED         (0x04)   /**< BR/EDR not supported. */ // BR/EDR ��֧��
#define BLE_GAP_ADV_FLAG_LE_BR_EDR_CONTROLLER         (0x08)   /**< Simultaneous LE and BR/EDR, Controller. */ //ͬ�� LE �� BR/EDR��������
#define BLE_GAP_ADV_FLAG_LE_BR_EDR_HOST               (0x10)   /**< Simultaneous LE and BR/EDR, Host. */ // ͬʱ LE �� BR/EDR������
#define BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE   (BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE | BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED)   /**< LE Limited Discoverable Mode, BR/EDR not supported. */ //���޵Ŀɷ���ģʽ��br/edr ����֧��
#define BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE   (BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE | BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED)   /**< LE General Discoverable Mode, BR/EDR not supported. *///һ��ɷ���ģʽ��br/edr ����֧��
/**@} */


/**@defgroup BLE_GAP_ADV_INTERVALS GAP Advertising interval max and min �㲥���������Сʱ��
 * @{ */
#define BLE_GAP_ADV_INTERVAL_MIN        0x0020 /**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */ //��С�㲥����� 20000us��Ҳ���� 20ms
#define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 /**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */ //�ڷ�����ģʽ����С�����100000us Ҳ���� 100ms
#define BLE_GAP_ADV_INTERVAL_MAX        0x4000 /**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */ //���㲥����� 10240000us ,Ҳ���� 10.24s
 /**@}  */


/**@defgroup BLE_GAP_SCAN_INTERVALS GAP Scan interval max and min //GAP ɨ�������ֵ����Сֵ
 * @{ */
#define BLE_GAP_SCAN_INTERVAL_MIN       0x0004 /**< Minimum Scan interval in 625 us units, i.e. 2.5 ms. */ // ��Сɨ������ 2500us��Ҳ����2.5ms��
#define BLE_GAP_SCAN_INTERVAL_MAX       0x4000 /**< Maximum Scan interval in 625 us units, i.e. 10.24 s. */ //���ɨ������10240000s��Ҳ���� 10.24s��
 /** @}  */


/**@defgroup BLE_GAP_SCAN_WINDOW GAP Scan window max and min //������СGAPɨ�贰��
 * @{ */
#define BLE_GAP_SCAN_WINDOW_MIN         0x0004 /**< Minimum Scan window in 625 us units, i.e. 2.5 ms. */ //��Сɨ�贰����2500us ,Ҳ���� 2.5ms
#define BLE_GAP_SCAN_WINDOW_MAX         0x4000 /**< Maximum Scan window in 625 us units, i.e. 10.24 s. */ // ���ɨ�贰���� 10240000��Ҳ���� 10.24s
 /** @}  */


/**@defgroup BLE_GAP_SCAN_TIMEOUT GAP Scan timeout max and min //GAP ɨ�賬ʱ���ֵ����Сֵ
 * @{ */
#define BLE_GAP_SCAN_TIMEOUT_MIN        0x0001 /**< Minimum Scan timeout in seconds. */ //��С��ɨ�賬ʱʱ��
#define BLE_GAP_SCAN_TIMEOUT_MAX        0xFFFF /**< Maximum Scan timeout in seconds. */ //����ɨ�賬ʱʱ��
 /** @}  */


/**@brief Maximum size of advertising data in octets. */ //���ֽڹ�����ݵ����ߴ�
#define  BLE_GAP_ADV_MAX_SIZE           31


/**@defgroup BLE_GAP_ADV_TYPES GAP Advertising types
 * @{ */
#define BLE_GAP_ADV_TYPE_ADV_IND          0x00   /**< Connectable undirected. */ //�޷���������
#define BLE_GAP_ADV_TYPE_ADV_DIRECT_IND   0x01   /**< Connectable directed. */ // �з���������
#define BLE_GAP_ADV_TYPE_ADV_SCAN_IND     0x02   /**< Scannable undirected. */ //�޷�����ɨ��
#define BLE_GAP_ADV_TYPE_ADV_NONCONN_IND  0x03   /**< Non connectable undirected. */ //�з�����ɨ��
/**@} */


/**@defgroup BLE_GAP_ADV_FILTER_POLICIES GAP Advertising filter policies // GAP �㲥���˲���
 * @{ */
#define BLE_GAP_ADV_FP_ANY                0x00   /**< Allow scan requests and connect requests from any device. */ //����ɨ�����������κ��豸������
#define BLE_GAP_ADV_FP_FILTER_SCANREQ     0x01   /**< Filter scan requests with whitelist. */ // �ð���������ɨ������
#define BLE_GAP_ADV_FP_FILTER_CONNREQ     0x02   /**< Filter connect requests with whitelist. */ //���������������������
#define BLE_GAP_ADV_FP_FILTER_BOTH        0x03   /**< Filter both scan and connect requests with whitelist. */ //���������������������
/**@} */


/**@defgroup BLE_GAP_ADV_TIMEOUT_VALUES GAP Advertising timeout values //GAP �㲥�Ĳ�೬ʱֵ
 * @{ */
#define BLE_GAP_ADV_TIMEOUT_LIMITED_MAX      180 /**< Maximum advertising time in limited discoverable mode (TGAP(lim_adv_timeout) = 180s). */ //�����޿ɷ���ģʽ�µ������ʱ��(TGAP(lim_adv_timeout) = 180s).
#define BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED  0 /**< Unlimited advertising in general discoverable mode. */ //���޵Ĺ㲥��һ��Ŀɷ���ģʽ
/**@} */


/**@defgroup BLE_GAP_DISC_MODES GAP Discovery modes GAP ����ģʽ
 * @{ */
#define BLE_GAP_DISC_MODE_NOT_DISCOVERABLE  0x00   /**< Not discoverable discovery Mode. */ //���ɷ��ֵķ���ģʽ
#define BLE_GAP_DISC_MODE_LIMITED           0x01   /**< Limited Discovery Mode. */ //���޵ķ���ģʽ
#define BLE_GAP_DISC_MODE_GENERAL           0x02   /**< General Discovery Mode. */ // һ�㷢��ģʽ
/**@} */

/**@defgroup BLE_GAP_IO_CAPS GAP IO Capabilities GAP �����������
 * @{ */
#define BLE_GAP_IO_CAPS_DISPLAY_ONLY      0x00   /**< Display Only. */ //����ʾ
#define BLE_GAP_IO_CAPS_DISPLAY_YESNO     0x01   /**< Display and Yes/No entry. */ //��/�� ��ʾ��Ŀ
#define BLE_GAP_IO_CAPS_KEYBOARD_ONLY     0x02   /**< Keyboard Only. */ //������
#define BLE_GAP_IO_CAPS_NONE              0x03   /**< No I/O capabilities. */ //�� I/O ����
#define BLE_GAP_IO_CAPS_KEYBOARD_DISPLAY  0x04   /**< Keyboard and Display. */ //���̺���ʾ
/**@} */


/**@defgroup BLE_GAP_AUTH_KEY_TYPES GAP Authentication Key Types //GAP ��֤��Կ����
 * @{ */
#define BLE_GAP_AUTH_KEY_TYPE_NONE        0x00   /**< No key (may be used to reject). */ //�� ��Կ��(���ܱ������ܾ�)
#define BLE_GAP_AUTH_KEY_TYPE_PASSKEY     0x01   /**< 6-digit Passkey. */ //6λ�� ����Կ��
#define BLE_GAP_AUTH_KEY_TYPE_OOB         0x02   /**< Out Of Band data. */ //��ͬƵ���źŴ�������
/**@} */

/**@defgroup BLE_GAP_SEC_STATUS GAP Security status //GAP ��ȫ״̬
 * @{ */ 
#define BLE_GAP_SEC_STATUS_SUCCESS                0x00  /**< Procedure completed with success. */ //���̳ɹ����
#define BLE_GAP_SEC_STATUS_TIMEOUT                0x01  /**< Procedure timed out. */ //���̳�ʱ
#define BLE_GAP_SEC_STATUS_PDU_INVALID            0x02  /**< Invalid PDU received. */ //��Ч��PDU
#define BLE_GAP_SEC_STATUS_PASSKEY_ENTRY_FAILED   0x81  /**< Passkey entry failed (user cancelled or other). */ //��������ʧ�� (�û�ȡ��������)
#define BLE_GAP_SEC_STATUS_OOB_NOT_AVAILABLE      0x82  /**< Out of Band Key not available. */ //��Կ�޷�ʹ��
#define BLE_GAP_SEC_STATUS_AUTH_REQ               0x83  /**< Authentication requirements not met. */ //�����֤��������
#define BLE_GAP_SEC_STATUS_CONFIRM_VALUE          0x84  /**< Confirm value failed. */ // ��֤ʧ��ֵ
#define BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP       0x85  /**< Pairing not supported.  */ //����֧�ֵ����
#define BLE_GAP_SEC_STATUS_ENC_KEY_SIZE           0x86  /**< Encryption key size. */ //������Կ��С
#define BLE_GAP_SEC_STATUS_SMP_CMD_UNSUPPORTED    0x87  /**< Unsupported SMP command. */ //��֧�ֵ�SMPָ��
#define BLE_GAP_SEC_STATUS_UNSPECIFIED            0x88  /**< Unspecified reason. */ //����ԭ��
#define BLE_GAP_SEC_STATUS_REPEATED_ATTEMPTS      0x89  /**< Too little time elapsed since last attempt. */ //�ϴγ���ʱ��̫��
#define BLE_GAP_SEC_STATUS_INVALID_PARAMS         0x8A  /**< Invalid parameters. */ // //��Ч�Ĳ���
/**@} */

/**@defgroup BLE_GAP_SEC_STATUS_SOURCES GAP Security status sources //GAP ��ȫ״̬��Դ
 * @{ */
#define BLE_GAP_SEC_STATUS_SOURCE_LOCAL           0x00  /**< Local failure. */ //����ʧ��
#define BLE_GAP_SEC_STATUS_SOURCE_REMOTE          0x01  /**< Remote failure. */ //Զ��ʧ��
/**@} */

/**@defgroup BLE_GAP_CP_LIMITS GAP Connection Parameters Limits //GAP ���Ӳ�������
 * @{ */
#define BLE_GAP_CP_MIN_CONN_INTVL_NONE           0xFFFF  /**< No new minimum connction interval specified in connect parameters. */ //�����Ӳ�����û��ָ���µ���С���Ӽ��
#define BLE_GAP_CP_MIN_CONN_INTVL_MIN            0x0006  /**< Lowest mimimum connection interval permitted, in units of 1.25 ms, i.e. 7.5 ms. */ //��С�����Ӽ����7.5ms
#define BLE_GAP_CP_MIN_CONN_INTVL_MAX            0x0C80  /**< Highest minimum connection interval permitted, in units of 1.25 ms, i.e. 4 s. */ // �������Ӽ�� 4s
#define BLE_GAP_CP_MAX_CONN_INTVL_NONE           0xFFFF  /**< No new maximum connction interval specified in connect parameters. */ //�����Ӳ�����û��ָ���µ�������Ӽ��
#define BLE_GAP_CP_MAX_CONN_INTVL_MIN            0x0006  /**< Lowest maximum connection interval permitted, in units of 1.25 ms, i.e. 7.5 ms. */ // �������С�����Ӽ�� 7.5ms
#define BLE_GAP_CP_MAX_CONN_INTVL_MAX            0x0C80  /**< Highest maximum connection interval permitted, in units of 1.25 ms, i.e. 4 s. */ //������������Ӽ��  4s
#define BLE_GAP_CP_SLAVE_LATENCY_MAX             0x01F3  /**< Highest slave latency permitted, in connection events. */ //�������¼����������ߵĴ�����Ǳ����
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_NONE         0xFFFF  /**< No new supervision timeout specified in connect parameters. */ //�����Ӳ�����û��ָ���µļල��ʱ
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_MIN          0x000A  /**< Lowest supervision timeout permitted, in units of 10 ms, i.e. 100 ms. */ // ��ͼල��ʱ������λΪ100ms
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX          0x0C80  /**< Highest supervision timeout permitted, in units of 10 ms, i.e. 32 s. */ //��ߵļල��ʱ������λΪ10ms����32s
/**@} */


/**@brief GAP device name maximum length. */ //GAP�豸����󳤶�
#define BLE_GAP_DEVNAME_MAX_LEN           31

/**@brief Disable RSSI events for connections */ // Ϊ���ӽ��� RSSI�¼�
#define BLE_GAP_RSSI_THRESHOLD_INVALID    0xFF

/**@defgroup BLE_GAP_CONN_SEC_MODE_SET_MACROS GAP attribute security requirement setters //GAP ���԰�ȫ��Ҫ������
 *
 * See @ref ble_gap_conn_sec_mode_t.
 * @{ */
/**@brief Set sec_mode pointed to by ptr to have no access rights.*/ //�� ptrָ�� sec_mode û�з���Ȩ��
#define BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(ptr)         do {(ptr)->sm = 0; (ptr)->lv = 0;} while(0)
/**@brief Set sec_mode pointed to by ptr to require no protection, open link.*/ //��ptr ָ���� sec_mode,����Ҫ�����������ӡ�
#define BLE_GAP_CONN_SEC_MODE_SET_OPEN(ptr)              do {(ptr)->sm = 1; (ptr)->lv = 1;} while(0)
/**@brief Set sec_mode pointed to by ptr to require encryption, but no MITM protection.*/ //��ptrָ���� sec_mode Ҫ����ܣ���û��MITM����
#define BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(ptr)       do {(ptr)->sm = 1; (ptr)->lv = 2;} while(0)
/**@brief Set sec_mode pointed to by ptr to require encryption and MITM protection.*/ // ��ptrָ����sec_modeҪ����ܺ�MITM����
#define BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(ptr)     do {(ptr)->sm = 1; (ptr)->lv = 3;} while(0)
/**@brief Set sec_mode pointed to by ptr to require signing or encryption, no MITM protection needed.*/ //��ptrָ����sec_mode Ҫ��ǩ������ܣ�����ҪMITM����
#define BLE_GAP_CONN_SEC_MODE_SET_SIGNED_NO_MITM(ptr)    do {(ptr)->sm = 2; (ptr)->lv = 1;} while(0)
/**@brief Set sec_mode pointed to by ptr to require signing or encryption with MITM protection.*/ //��ptrָ����sec_mode Ҫ��ʹ��MITM��������ǩ������ܡ�
#define BLE_GAP_CONN_SEC_MODE_SET_SIGNED_WITH_MITM(ptr)  do {(ptr)->sm = 2; (ptr)->lv = 2;} while(0)
/**@} */


/**@brief GAP Security Random Number Length. */ //GAP ��ȫ���������
#define BLE_GAP_SEC_RAND_LEN 8

/**@brief GAP Security Key Length. */ //GAP ��ȫ��Կ����
#define BLE_GAP_SEC_KEY_LEN 16

/**@brief GAP Passkey Length. */ //GAP ����Կ�׳���
#define BLE_GAP_PASSKEY_LEN 6

/**@brief Maximum amount of addresses in a whitelist. */ //�ڰ����������ĵ�ַ
#define BLE_GAP_WHITELIST_ADDR_MAX_COUNT (8)
 
/**@brief Maximum amount of IRKs in a whitelist. //�ڰ���������������� IRKs
 * @note  The number of IRKs is limited to 8, even if the hardware supports more. //IRKs����������Ϊ8����ʱӲ��֧�ָ��ࡣ
 */
#define BLE_GAP_WHITELIST_IRK_MAX_COUNT (8)

/**@defgroup GAP_SEC_MODES GAP Security Modes GAP ��ȫģʽ
 * @{ */
#define BLE_GAP_SEC_MODE 0x00 /**< No key (may be used to reject). */ //û����Կ��(���ܱ������ܾ�)
/**@} */

/**@} */

/**@addtogroup BLE_GAP_STRUCTURES Structures //�ṹ
 * @{ */

/**@brief Bluetooth Low Energy address. */ //�͹���������ַ
typedef struct
{
  uint8_t addr_type;                    /**< See @ref BLE_GAP_ADDR_TYPES. */ //�μ� BLE_GAP_ADDR_TYPES.
  uint8_t addr[BLE_GAP_ADDR_LEN];       /**< 48-bit address, LSB format. */ //48λ��ַ��LSB��ʽ
} ble_gap_addr_t;


/**@brief GAP connection parameters. //GAP ���Ӳ���
 *
 * @note  When ble_conn_params_t is received in an event, both min_conn_interval and
 *        max_conn_interval will be equal to the connection interval set by the central. 
//�� ble_conn_params_t ��һ���¼��б�����ʱ��min_conn_interval��max_conn_interval�������豸���õ����Ӽ����
 *
 * @note If both conn_sup_timeout and max_conn_interval are specified, then the following constraint applies:
 *       conn_sup_timeout * 4 > (1 + slave_latency) * max_conn_interval
 *       that corresponds to the following Bluetooth Spec requirement:
 *       The Supervision_Timeout in milliseconds shall be larger than
 *       (1 + Conn_Latency) * Conn_Interval_Max * 2, where Conn_Interval_Max is given in milliseconds.

���ָ�� conn_sup_timeout �� max_conn_interval����ô�����Լ�������ǣ� conn_sup_timeout * 4 > (1+ slave_latency) * max_conn_interval
���������������Ҫ������� 
�Ժ���Ϊ��λ�Ĺ��ʱӦ�ô��� (1+ Conn_Latency) * Conn_Interval_Max * 2,���� Conn_Intervaal_Max �Ժ���Ϊ��λ��

 */
typedef struct
{
  uint16_t min_conn_interval;         /**< Minimum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS.*/ //��С���Ӽ���� 1.25ms���μ� BLE_GAP_CP_LIMITS
  uint16_t max_conn_interval;         /**< Maximum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS.*/ //������Ӽ���� 1.25ms���μ� BLE_GAP_CP_LINITS.
  uint16_t slave_latency;             /**< Slave Latency in number of connection events, see @ref BLE_GAP_CP_LIMITS.*/ //�������¼��������ϵĴ����ӳ٣��μ� BLE_GAP_CP_LIMITS.
  uint16_t conn_sup_timeout;          /**< Connection Supervision Timeout in 10 ms units, see @ref BLE_GAP_CP_LIMITS.*/ //���Ӽ��ӳ�ʱ��10ms��Ԫ���μ�BLE_GAP_CP_LIMITS.
} ble_gap_conn_params_t;


/**@brief GAP connection security modes. //GAP ���Ӱ�ȫģʽ
 *
 * Security Mode 0 Level 0: No access permissions at all (this level is not defined by the Bluetooth Core specification).\n
��ȫģʽ0���ȼ�0�� ����û�з���Ȩ��(������������������Ĺ淶�����)
 * Security Mode 1 Level 1: No security is needed (aka open link).\n
��ȫģʽ1���ȼ�1������Ҫ��ȫ��(Ҳ���ǿ�������)
 * Security Mode 1 Level 2: Encrypted link required, MITM protection not necessary.\n
��ȫģʽ1���ȼ�2����Ҫ���ܵı���������ҪMITM������
 * Security Mode 1 Level 3: MITM protected encrypted link required.\n��
��ȫģʽ1���ȼ�3��MITM������������
 * Security Mode 2 Level 1: Signing or encryption required, MITM protection not necessary.\n
��ȫģʽ2���ȼ�1����Ҫǩ������ܣ�����ҪMITM����
 * Security Mode 2 Level 2: MITM protected signing required, unless link is MITM protected encrypted.\n
��ȫģʽ2���ȼ�2��MITM����ǩ���������������ܱ����ļ��ܡ�
 */
typedef struct
{
  uint8_t sm : 4;                     /**< Security Mode (1 or 2), 0 for no permissions at all. */
	//��ȫģʽ(1��2)��0ʱ��ȫû��Ȩ�ޡ�
  uint8_t lv : 4;                     /**< Level (1, 2 or 3), 0 for no permissions at all. */
	//�ȼ�(1��2��3)��0ʱ��ȫû��Ȩ�ޡ�

} ble_gap_conn_sec_mode_t;


/**@brief GAP connection security status.*/ //GAP ���Ӱ�ȫ״̬
typedef struct
{
  ble_gap_conn_sec_mode_t sec_mode;           /**< Currently active security mode for this connection.*/
	//��ǰ�������ӵĻ��ȫģʽ
  uint8_t                 encr_key_size;      /**< Length of currently active encryption key, 7 to 16 octets (only applicable for bonding procedures). */
	//��ǰ��Դ������Կ�ĳ��ȣ�7��16����λ�ֽ�(�������ڽ�ϳ���)

} ble_gap_conn_sec_t;


/**@brief Identity Resolving Key. */ //��ݷ���Կ�ס�
typedef struct
{
  uint8_t irk[BLE_GAP_SEC_KEY_LEN];   /**< Array containing IRK. */ //���ݰ��� IRK��
} ble_gap_irk_t;


/**@brief Whitelist structure. */ //�������Ľṹ
typedef struct
{
  ble_gap_addr_t    **pp_addrs;        /**< Pointer to an array of device address pointers, pointing to addresses to be used in whitelist. NULL if none are given. */
  //ָ��һ���豸��ַָ�������ָ�룬ָ��Ҫ�ڰ�������ʹ�õĵ�ַ�����û�и�������Ϊ�ա�
	uint8_t             addr_count;      /**< Count of device addresses in array, up to @ref BLE_GAP_WHITELIST_ADDR_MAX_COUNT. */
  //�������豸��ַ�ļ�����һֱ�� BLE_GAP_WHITELIST_ADDR_MAX_COUNT.
	ble_gap_irk_t     **pp_irks;         /**< Pointer to an array of Identity Resolving Key (IRK) pointers, each pointing to an IRK in the whitelist. NULL if none are given. */
// ָ��һ��ʡ�ݽ���KEY(IRK)ָ���ָ�룬û��ָ���ָ��������е�IRK�����û�и�������Ϊ�ա�
	uint8_t             irk_count;       /**< Count of IRKs in array, up to @ref BLE_GAP_WHITELIST_IRK_MAX_COUNT. */
	//�����е�IRKs������ֱ�� BLE_GAP_WHITELIST_IRK_MAX_COUNT.
} ble_gap_whitelist_t;

/**@brief Channel mask for RF channels used in advertising and scanning. */ 
//���ڹ㲥��ɨ�����Ƶͨ����ͨ���ڸǡ�
typedef struct
{
  uint8_t ch_37_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 37 */ 
	//�����λ����Ϊ1���رյ�37Ƶ���Ĺ㲥
  uint8_t ch_38_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 38 */
	//�����λ����Ϊ1���رյ�38Ƶ���Ĺ㲥
  uint8_t ch_39_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 39 */
	//�����λ����Ϊ1���رյ�39Ƶ���Ĺ㲥
} ble_gap_adv_ch_mask_t;

/**@brief GAP advertising parameters.*/
typedef struct
{
  uint8_t               type;                 /**< See @ref BLE_GAP_ADV_TYPES. */ //�μ� BLE_GAP_ADV_TYPES.
  ble_gap_addr_t       *p_peer_addr;          /**< For @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND mode only, known peer address. */
	//������ BLE_GAP_ADV_TYPE_ADV_DIRECT_INDģʽ��֪��(�Ե�)�Է���ַ��
  uint8_t               fp;                   /**< Filter Policy, see @ref BLE_GAP_ADV_FILTER_POLICIES. */
	//���˲��ԣ��μ� BLE_GAP_ADV_FILTER_POLICIES.
  ble_gap_whitelist_t  *p_whitelist;          /**< Pointer to whitelist, NULL if none is given. */
	//ָ��������ĵ�ַ�����û�У���Ϊ�ա�
  uint16_t              interval;             /**< Advertising interval between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s), see @ref BLE_GAP_ADV_INTERVALS.
                                                   - If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, this parameter must be set to 0 for high duty cycle directed advertising.
                                                   - If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, set @ref BLE_GAP_ADV_INTERVAL_MIN <= interval <= @ref BLE_GAP_ADV_INTERVAL_MAX for low duty cycle advertising.*/
//0x0020��0x4000֮��Ĺ㲥���Ϊ0.625msһ����λ��(20ms��10.24s)���μ� BLE_GAP_ADV_INTERVALS.
//������͵��� BLE_GAP_ADV_TYPE_ADV_DIRECT_IND,����������뱻����Ϊ0�����ڸ߸���ѭ�������档
	//������͵��� BLE_GAP_ADV_TYPE_ADV_DIRECT_IND,���� BLE_GAP_ADV_INTERVAL_MIN <= interval <= BLE_GAP_ADV_INTERVAL_MAX ,���ڵ͸���ѭ���㲥��
	

	uint16_t              timeout;              /**< Advertising timeout between 0x0001 and 0x3FFF in seconds, 0x0000 disables timeout. See also @ref BLE_GAP_ADV_TIMEOUT_VALUES. If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, this parameter must be set to 0 for High duty cycle directed advertising. */
  //��0x0001��0x3FFF֮��Ĺ�泬ʱʱ��Ϊ�룬0x0000���ó�ʱ���μ� BLE_GAP_ADV_TIMEOUT_VALUES.������͵���BLE_GAP_ADV_TYPE_ADV_DIRECT_IND����������뱻����Ϊ0�����ڸ߸���ѭ������㲥��
	
	ble_gap_adv_ch_mask_t channel_mask;         /**< Advertising channel mask. @see ble_gap_channel_mask_t for documentation. */
	//�㲥ͨ�����ڸǡ��μ� ble_gap_channel_mask_t �ĵ���
} ble_gap_adv_params_t;


/**@brief GAP scanning parameters. */ //GAP ɨ�����
typedef struct
{
  uint8_t                 active    : 1;        /**< If 1, perform active scanning (scan requests). */ //���Ϊ1��ִ������ɨ��(ɨ������)
  uint8_t                 selective : 1;        /**< If 1, ignore unknown devices (non whitelisted). */  //���Ϊ1������δ֪���豸(�ǰ�����)
  ble_gap_whitelist_t *   p_whitelist;          /**< Pointer to whitelist, NULL if none is given. */ //ָ���������ָ�룬���û�У���Ϊ�ա�
  uint16_t                interval;             /**< Scan interval between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */ //ɨ���� 0X0004��0x4000֮�䣬0.625msһ����Ԫ(25ms��10.24s)��
  uint16_t                window;               /**< Scan window between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */ // ɨ�贰����0x0004��0x4000֮�䣬0.625msһ����Ԫ(2.5ms��10.24s).
  uint16_t                timeout;              /**< Scan timeout between 0x0001 and 0xFFFF in seconds, 0x0000 disables timeout. */ //ɨ�賬ʱ �� 0x0001��0xFFFF֮��ɨ�賬ʱ��0x0000���ó�ʱ��
} ble_gap_scan_params_t;


/** @brief Keys that can be exchanged during a bonding procedure. */ //KEY �����ڰ󶨹����̽�����
typedef struct
{
  uint8_t enc     : 1;                        /**< Long Term Key and Master Identification. */ //������Կ������ʶ��
  uint8_t id      : 1;                        /**< Identity Resolving Key and Identity Address Information. */ //��ݽ�����Կ����ݵ�ַ��Ϣ��
  uint8_t sign    : 1;                        /**< Connection Signature Resolving Key. */ //����ǩ������KEY��
} ble_gap_sec_kdist_t;


/**@brief GAP security parameters. */ //GAP ��ȫ������
typedef struct
{
  uint8_t               bond    : 1;               /**< Perform bonding. */ //ִ�м���
  uint8_t               mitm    : 1;               /**< Man In The Middle protection required. */ //�û����м���Ҫ������
  uint8_t               io_caps : 3;               /**< IO capabilities, see @ref BLE_GAP_IO_CAPS. */ //IO ���ܣ��μ� BLE_GAP_IO_CAPS.
  uint8_t               oob     : 1;               /**< Out Of Band data available. */ //���õĴ����ݡ�
  uint8_t               min_key_size;              /**< Minimum encryption key size in octets between 7 and 16. If 0 then not applicable in this instance. */ //��7��16֮�����С���ֽڼ�����Կ��С�������0�������ʵ���в����á�
  uint8_t               max_key_size;              /**< Maximum encryption key size in octets between min_key_size and 16. */ //����Сֵ��16֮��������ֽ���Կ��С��
  ble_gap_sec_kdist_t   kdist_periph;              /**< Key distribution bitmap: keys that the peripheral device will distribute. */  //��Կ�ַ�λͼ����Χ�豸���ַ�����Կ��
  ble_gap_sec_kdist_t   kdist_central;             /**< Key distribution bitmap: keys that the central device will distribute. */ //��Կ�ַ�λͼ�������豸����ַ�����Կ��
} ble_gap_sec_params_t;


/**@brief GAP Encryption Information. */ //GAP ������Ϣ��
typedef struct
{
  uint8_t   ltk[BLE_GAP_SEC_KEY_LEN];   /**< Long Term Key. */ //������Կ��
  uint8_t   auth : 1;                   /**< Authenticated Key. */  //����֤ ��Կ��
  uint8_t   ltk_len : 7;                /**< LTK length in octets. */ // //���ֽ�LTK���ȡ�
} ble_gap_enc_info_t;


/**@brief GAP Master Identification. */  //GAP ���豸��֤��
typedef struct
{
  uint16_t  ediv;                       /**< Encrypted Diversifier. */ //���ܵĶ�Ԫ��
  uint8_t   rand[BLE_GAP_SEC_RAND_LEN]; /**< Random Number. */ //�������
} ble_gap_master_id_t;

 
/**@brief GAP Signing Information. */ //GAP ǩ����Ϣ��
typedef struct
{
  uint8_t   csrk[BLE_GAP_SEC_KEY_LEN];        /**< Connection Signature Resolving Key. */ //��������ǩ����Կ
} ble_gap_sign_info_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONNECTED. */ // BLE_GAP_EVT_CONNECTED �¼��ṹ��
typedef struct
{
  ble_gap_addr_t        peer_addr;              /**< Bluetooth address of the peer device. */ //�Ե�(��)�豸��������ַ
  ble_gap_addr_t        own_addr;               /**< Bluetooth address of the local device used during connection setup. */ //�����豸����������ʱ��������ַ��
  uint8_t               irk_match :1;           /**< If 1, peer device's address resolved using an IRK. */ //���Ϊ1���Ե�(��)�豸�ĵ�ַʹ��IRK������
  uint8_t               irk_match_idx  :7;      /**< Index in IRK list where the address was matched. */ //��IRK�б���ƥ���ַ�������� 
  ble_gap_conn_params_t conn_params;            /**< GAP Connection Parameters. */ // GAP ���Ӳ�����
} ble_gap_evt_connected_t;


/**@brief Event structure for @ref BLE_GAP_EVT_DISCONNECTED. */ //BLE_GAP_EVT_DISCONNECTED ���¼��ṹ��
typedef struct
{
  uint8_t reason;                               /**< HCI error code, see @ref BLE_HCI_STATUS_CODES. */ //HCL������룬�μ� BLE_HCL_STATUS_CODES.
} ble_gap_evt_disconnected_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONN_PARAM_UPDATE. */ //BLE_GAP_EVT_CONN_PARAM_UPDATE ���¼��ṹ��
typedef struct
{
  ble_gap_conn_params_t conn_params;            /**<  GAP Connection Parameters. */ //GAP���Ӳ�����
} ble_gap_evt_conn_param_update_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SEC_PARAMS_REQUEST. */ //BLE_GAP_EVT_SEC_PARAMS_REQUEST���¼��ṹ��
typedef struct
{
  ble_gap_sec_params_t peer_params;             /**< Initiator Security Parameters. */ //��������ȫ������
} ble_gap_evt_sec_params_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SEC_INFO_REQUEST. */ //BLE_GAP_EVT_SEC_INFO_REQUEST �¼��ṹ��
typedef struct
{
  ble_gap_addr_t      peer_addr;                     /**< Bluetooth address of the peer device. */ //�Ե�(��)�豸��������ַ��
  ble_gap_master_id_t master_id;                     /**< Master Identification for LTK lookup. */ //����LTK���ҵ����豸��ʶ��
  uint8_t             enc_info  : 1;                 /**< If 1, Encryption Information required. */ //���Ϊ1������Ҫ���ܵ���Ϣ��
  uint8_t             id_info   : 1;                 /**< If 1, Identity Information required. */ //���Ϊ1����Ҫ�����Ϣ��
  uint8_t             sign_info : 1;                 /**< If 1, Signing Information required. */ //���Ϊ1����Ҫǩ����Ϣ��
} ble_gap_evt_sec_info_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_PASSKEY_DISPLAY. */ // BLE_GAP_EVT_PASSKEY_DISPLAY �¼��ṹ��
typedef struct
{
  uint8_t passkey[BLE_GAP_PASSKEY_LEN];         /**< 6-digit passkey in ASCII ('0'-'9' digits only). */ //ASCII����ɵ�6λ����( 0-9 ���� )
} ble_gap_evt_passkey_display_t;


/**@brief Event structure for @ref BLE_GAP_EVT_AUTH_KEY_REQUEST. */ //BLE_GAP_EVT_AUTH_KEY_REQUEST ���¼��ṹ��
typedef struct
{
  uint8_t key_type;                             /**< See @ref BLE_GAP_AUTH_KEY_TYPES. */ //�μ� BLE_GAP_AUTH_KEY_TYPES.
} ble_gap_evt_auth_key_request_t;


/**@brief Security levels supported. //��ȫ�����֧��
 * @note See Bluetooth Specification Version 4.1 Volume 3, Part C, Chapter 10. //�μ������淶 V4.1 �� 3��C���֣���10�¡�
*/
typedef struct
{
  uint8_t lv1 : 1;                              /**< If 1: Level 1 is supported. */ //���Ϊ1������1��֧�֡�
  uint8_t lv2 : 1;                              /**< If 1: Level 2 is supported. */ //���Ϊ1������2��֧�֡�
  uint8_t lv3 : 1;                              /**< If 1: Level 3 is supported. */ //���Ϊ1������3��֧�֡�
} ble_gap_sec_levels_t;


/**@brief Encryption Key. */ //��Կ
typedef struct
{
  ble_gap_enc_info_t    enc_info;             /**< Encryption Information. */ //������Ϣ��
  ble_gap_master_id_t   master_id;            /**< Master Identification. */ //���豸ʶ��
} ble_gap_enc_key_t;


/**@brief Identity Key. */ //�����Ϣ��
typedef struct
{
  ble_gap_irk_t         id_info;              /**< Identity Information. */ //�����Ϣ��
  ble_gap_addr_t        id_addr_info;         /**< Identity Address Information. */ //��ݵ�ַ��Ϣ��
} ble_gap_id_key_t;


/**@brief Security Keys. */ //��Կ
typedef struct
{
  ble_gap_enc_key_t     *p_enc_key;           /**< Encryption Key, or NULL. */ //����Կ�ף���NULL
  ble_gap_id_key_t      *p_id_key;            /**< Identity Key, or NULL. */ //���Կ�ף���NULL
  ble_gap_sign_info_t   *p_sign_key;          /**< Signing Key, or NULL. */ // ǩ��Կ��,��NULL
} ble_gap_sec_keys_t;


/**@brief Security key set (both Peripheral and Central keys). //��ȫ��Կ��(��Χ�������豸��KEY)
 *         Note that when distributing Bluetooth addresses pertaining to the local device those
 *         will have to be filled in by the user. */ //ע�⣬���ַ��뱾���豸��ص�������ַʱ�������û���д��
typedef struct
{
  ble_gap_sec_keys_t keys_periph;     /**< Keys distributed by the device in the Peripheral role. */ //����Χ�豸�����KEY
  ble_gap_sec_keys_t keys_central;    /**< Keys distributed by the device in the Central role. */ // �������豸�����KEY
} ble_gap_sec_keyset_t;


/**@brief Event structure for @ref BLE_GAP_EVT_AUTH_STATUS. */ //BLE_GAP_EVT_AUTH_STATUS���¼��ṹ
typedef struct
{
  uint8_t               auth_status;            /**< Authentication status, see @ref BLE_GAP_SEC_STATUS. */ //�����֤״̬���ο�BLE_GAP_SEC_STATUS
  uint8_t               error_src : 2;          /**< On error, source that caused the failure, see @ref BLE_GAP_SEC_STATUS_SOURCES. */ //�ڴ��������£�����ʧ�ܵ�Դ���ο�BLE_GAP_SEC_STATUS_SOURCES
  uint8_t               bonded : 1;             /**< Procedure resulted in a bond. *///����������һ��KEY��
  ble_gap_sec_levels_t  sm1_levels;             /**< Levels supported in Security Mode 1. */ // //�ڰ�ȫģʽ1��֧�ֵļ���
  ble_gap_sec_levels_t  sm2_levels;             /**< Levels supported in Security Mode 2. */ //�ڰ�ȫģʽ2��֧�ֵļ���
  ble_gap_sec_kdist_t   kdist_periph;           /**< Bitmap stating which keys were exchanged (distributed) by the peripheral. */ //λͼ��ʾ����Χ�豸����(�ֲ�ʽ)��
  ble_gap_sec_kdist_t   kdist_central;          /**< Bitmap stating which keys were exchanged (distributed) by the central. */ //λͼ��ʾ�����뽻��(�ֲ�ʽ)��
} ble_gap_evt_auth_status_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONN_SEC_UPDATE. */ //BLE_GAP_EVT_CONN_SEC_UPDATE ���¼��ṹ
typedef struct
{
  ble_gap_conn_sec_t conn_sec;                  /**< Connection security level. */ //���Ӱ�ȫ����
} ble_gap_evt_conn_sec_update_t;


/**@brief Event structure for @ref BLE_GAP_EVT_TIMEOUT. */ //BLE_GAP_EVT_TIMEOUT ���¼��ṹ
typedef struct
{
  uint8_t src;                                  /**< Source of timeout event, see @ref BLE_GAP_TIMEOUT_SOURCES. */ //��ʱ�¼�����Դ���μ� BLE_GAP_TIMEOUT_SOURCES
} ble_gap_evt_timeout_t;


/**@brief Event structure for @ref BLE_GAP_EVT_RSSI_CHANGED. */ //BLE_GAP_EVT_RESI_CHANGED���¼��ṹ��
typedef struct
{
  int8_t  rssi;                               /**< Received Signal Strength Indication in dBm. */ //�����ź�ǿ��dbm��ָʾ
} ble_gap_evt_rssi_changed_t;


/**@brief Event structure for @ref BLE_GAP_EVT_ADV_REPORT. */ //BLE_GAP_EVT_ADV_REPORT���¼��ṹ
typedef struct
{
  ble_gap_addr_t peer_addr;                     /**< Bluetooth address of the peer device. */ //�Ե��豸��������ַ
  int8_t         rssi;                          /**< Received Signal Strength Indication in dBm. */ //�����źŵ�ǿ��dbmָʾ
  uint8_t        scan_rsp : 1;                  /**< If 1, the report corresponds to a scan response and the type field may be ignored. */ //�����1���㲥��Ӧ��ɨ����Ӧ���������ֶο��ܱ�����
  uint8_t        type     : 2;                  /**< See @ref BLE_GAP_ADV_TYPES. Only valid if the scan_rsp field is 0. *///�μ� BLE_GAP_ADV_TYPES.ֻ�е�scan_rsp �ֶ�Ϊ0ʱ����Ч��
  uint8_t        dlen     : 5;                  /**< Advertising or scan response data length. */ //�㲥��ɨ����Ӧ���ݳ��ȡ�
  uint8_t        data[BLE_GAP_ADV_MAX_SIZE];    /**< Advertising or scan response data. */ //�㲥��ɨ����Ӧ���ݡ�
} ble_gap_evt_adv_report_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SEC_REQUEST. */ //BLE_GAP_EVT_SEC_REQUEST ���¼��ṹ��
typedef struct
{
  uint8_t    bond    : 1;                       /**< Perform bonding. *///ִ�м�
  uint8_t    mitm    : 1;                       /**< Man In The Middle protection required. */ //���м�ı�������
} ble_gap_evt_sec_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST. */ //BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST ���¼��ṹ��
typedef struct 
{
  ble_gap_conn_params_t conn_params;            /**<  GAP Connection Parameters. */ // GAP ���Ӳ�����
} ble_gap_evt_conn_param_update_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SCAN_REQ_REPORT. */ //BLE_GAP_EVT_SCAN_REQ_REPORT ���¼��ṹ��
typedef struct
{
  int8_t                  rssi;              /**< Received Signal Strength Indication in dBm. */ //�����ź�ǿ��dbmָʾ��
  ble_gap_addr_t          peer_addr;         /**< Bluetooth address of the peer device. */ //�Ե������豸�ĵ�ַ��
} ble_gap_evt_scan_req_report_t;



/**@brief GAP event structure. */ //GAP �¼��ṹ
typedef struct
{
  uint16_t conn_handle;                                     /**< Connection Handle on which event occured. */ //�¼����������Ӿ��
  union                                                     /**< union alternative identified by evt_id in enclosing struct. */ //��evt_id�ڷ�սṹ��ʶ����������������
  {
    ble_gap_evt_connected_t                   connected;                    /**< Connected Event Parameters. */ //�����¼�������
    ble_gap_evt_disconnected_t                disconnected;                 /**< Disconnected Event Parameters. */ //�Ͽ����ӵ��¼�����
    ble_gap_evt_conn_param_update_t           conn_param_update;            /**< Connection Parameter Update Parameters. */ //���Ӳ������²���
    ble_gap_evt_sec_params_request_t          sec_params_request;           /**< Security Parameters Request Event Parameters. */ //��ȫ���������¼�����
    ble_gap_evt_sec_info_request_t            sec_info_request;             /**< Security Information Request Event Parameters. */ //��ȫ��Ϣ�����¼�����
    ble_gap_evt_passkey_display_t             passkey_display;              /**< Passkey Display Event Parameters. */ //����Կ����ʾʱ�����
    ble_gap_evt_auth_key_request_t            auth_key_request;             /**< Authentication Key Request Event Parameters. */ //�����֤��Կ�����¼�����
    ble_gap_evt_auth_status_t                 auth_status;                  /**< Authentication Status Event Parameters. */ //�����֤״̬ʱ�������
    ble_gap_evt_conn_sec_update_t             conn_sec_update;              /**< Connection Security Update Event Parameters. */ //���Ӱ�ȫ����ʱ�����
    ble_gap_evt_timeout_t                     timeout;                      /**< Timeout Event Parameters. */ //��ʱ�¼�����
    ble_gap_evt_rssi_changed_t                rssi_changed;                 /**< RSSI Event parameters. */ //RSSI�¼�����
    ble_gap_evt_adv_report_t                  adv_report;                   /**< Advertising Report Event Parameters. */ //�㲥�����¼�����
    ble_gap_evt_sec_request_t                 sec_request;                  /**< Security Request Event Parameters. */ //��ȫ�����¼�����
    ble_gap_evt_conn_param_update_request_t   conn_param_update_request;    /**< Connection Parameter Update Parameters. */ //���Ӳ������²���
    ble_gap_evt_scan_req_report_t             scan_req_report;              /**< Scan Request Report parameters. */ //ɨ�����󱨸����
  } params;                                                                 /**< Event Parameters. */ //�¼�����

} ble_gap_evt_t;


/**@brief Channel Map option. //�¼�����
 *        
 *
 * @note  Setting the channel map may take some time, depending on connection parameters. //����ͨ��ӳ�������ҪһЩʱ�䣬��ȡ�������Ӳ�����
 *        The time taken may be different for each connection and the get operation will
 *        return the previous channel map until the new one has taken effect. //����ÿ�����ӵ�ʱ����ܲ�ͬ����ȡ�������᷵��֮ǰ��ͨ��ӳ�䣬ֱ����ͨ����Ч��
 *
 * @note  After setting the channel map, by spec it can not be set again until at least 1 s has passed. //��������ͨ��ӳ��֮�󣬸��ݷ�Χ�������ھ���1S֮ǰ���������ٱ����á�
 *        See Bluetooth Specification Version 4.1 Volume 2, Part E, Section 7.3.46. //�μ������淶V4.1��2������E����7.3.46��
 *
 * @retval ::NRF_SUCCESS Get or set successful. //NRF_SUCESS ��ȡ �� ���óɹ�
 * @retval ::NRF_ERROR_BUSY Channel map was set again before enough time had passed. //NRF_ERROR_BUSY  ���㹻��ʱ���ȥ֮ǰ��Ƶ����ͼ�ֱ������ˡ�
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation. //NRF_ERROR_INVALID_STATE ��Ч״ִ̬�в���
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied for get. //BLE_ERROR_INVALID_CONN_HANDLE Ϊ��ȡ�ṩ����Ч���Ӿ����
 * @retval ::NRF_ERROR_NOT_SUPPORTED Returned by sd_ble_opt_set in peripheral-only SoftDevices. // NRF_ERROR_NOT_SUPPORTED ��sd_ble_opt_set ����Χ�豸��֧���·��ء�
 *
 */
typedef struct
{
  uint16_t conn_handle;                   /**< Connection Handle (only applicable for get) */ //���Ӿ�� (��������get)
  uint8_t ch_map[5];                      /**< Channel Map (37-bit). */ //ͨ��ͼ (37-bit)
} ble_gap_opt_ch_map_t;


/**@brief Local connection latency option. //���������ӳ�ѡ��
 *
 *        Local connection latency is a feature which enables the slave to improve
 *        current consumption by ignoring the slave latency set by the peer. The //���������ӳ���һ�����ԣ���ʹ�ӻ��ܹ�ͨ���Եȵ����õĴ����ӳ�����С�������ġ�
 *        local connection latency can only be set to a multiple of the slave latency, 
 *        and cannot be longer than half of the supervision timeout. //���������ӳ�ֻ�ܱ�����Ϊһ���������ӳٵı����������ܳ���һ��ļල��ʱ��

 *
 *        Used with @ref sd_ble_opt_set to set the local connection latency. The //ʹ�� sd_ble_opt_set �����ñ��������ӳ١�
 *        @ref sd_ble_opt_get is not supported for this option, but the actual //���ѡ�֧�� sd_ble_opt_set,����ʵ�ʵı����ӳ�(��������ΪNULL)������ѡ��ʱ������Ϊ���ز�����
 *        local connection latency (unless set to NULL) is set as a return parameter
 *        when setting the option. 
 *
 * @note  The latency set will be truncated down to the closest slave latency event
 *        multiple, or the nearest multiple before half of the supervision timeout. //�ӳټ������ض�Ϊ����Ķ�������ӳ��¼�������������ڼල��ʱǰ�Ķ����
 *
 * @note  The local connection latency is disabled by default, and needs to be enabled for new
 *        connections and whenever the connection is updated. //���������ӳ���ȱʡ״̬���ǽ��õģ�������ҪΪ�µ����Ӻ͵����ӱ�����ʱ���á�
 *
 * @retval ::NRF_SUCCESS Set successfully. //NRF_SUCCESS �ѳɹ����á�
 * @retval ::NRF_ERROR_NOT_SUPPORTED Get is not supported. //NRF_ERROR_NOT_SUPPORTED ��֧��
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle parameter. //BLE_ERROR_INVALID_CONN_HANDLE ��Ч�����Ӿ��������
 */
typedef struct
{
  uint16_t   conn_handle;                       /**< Connection Handle */ //���Ӿ��
  uint16_t   requested_latency;                 /**< Requested local connection latency. */ //�������������ӳ�
  uint16_t * p_actual_latency;                  /**< Pointer to storage for the actual local connection latency (can be set to NULL to skip return value). */ 
	//����ʵ�ʱ��������ӳٵ�ָ��(��������ΪNULL������������ֵ).
} ble_gap_opt_local_conn_latency_t;


/**@brief Passkey Option. //����Կ�׵�ѡ��
 *
 *        Structure containing the passkey to be used during pairing. This can be used with @ref 
 *        sd_ble_opt_set to make the SoftDevice use a pre-programmed passkey for authentication
 *        instead of generating a random one.
 *
��������Թ�����ʹ�õ�����Ľṹ����������� sd_ble_opt_set һ��ʹ�ã�ʹЭ��ջʹ��Ԥ�ȱ�̵�������������֤�������������һ����


 * @note  @ref sd_ble_opt_get is not supported for this option. //���ѡ�֧�� sd_ble_opt_get
 *
 */
typedef struct
{
  uint8_t * p_passkey;                          /**< Pointer to 6-digit ASCII string (digit 0..9 only, no NULL termination) passkey to be used during pairing. If this is NULL, the SoftDevice will generate a random passkey if required.*/
} ble_gap_opt_passkey_t; //ָ��6λASCII�ַ�����ָ�� (����0..9��ֻ�������ʱʹ�õ�����Żᱻʹ�á��������NULL����ôЭ��ջ������Ҫʱ����һ���������)��


/**@brief Custom Privacy Option. //�Զ�����˽ѡ�
 *
 *        This structure is used with both @ref sd_ble_opt_set (as input) and with
 *        @ref sd_ble_opt_get (as output). //����ṹͬʱʹ�� sd_ble_opt_set (��Ϊ����) �� sd_ble_opt_get (��Ϊ���)��
 *
 *        Structure containing: //�ṹ������
 *        - A pointer to an IRK to set (if input), or a place to store a read IRK (if output). //һ��ָ��IPK���õ�ָ��(�������)������һ���洢��ȡIRK(������)��λ�á�
 *        - A private address refresh cycle. //һ��˽�еĵ�ַˢ�����ڡ�
 *
 * @note  The specified address cycle interval is used when the address cycle mode is //����ַѭ��ģʽʱʹ��ָ����ַ��ѭ������� BLE_GAP_ADDR_CYCLE_MODE_AUTO��
 *        @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO. If 0 is given, the address will not be automatically  //�����0����ַ�������Զ�ˢ�¡�
 *        refreshed at all. The default interval is @ref BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S. //Ĭ�ϼ���� BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S.
 *
 * @note  If the current address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO, the address will immediately be
 *        refreshed when a custom privacy option is set. A new address can be generated manually by calling
 *        @ref sd_ble_gap_address_set with the same type again. 
//�����ǰ�ĵ�ַѭ��ģʽ�� BLE_GAP_ADDR_CYCLE_MODE_AUTO����ַ���������Զ�����˽ѡ��ʱˢ�¡�����ͨ������ sd_ble_gap_address_set �ֶ�����ͬ�����͵��µ�ַ

 *
 * @note  If the IRK is updated, the new IRK becomes the one to be distributed in all
 *        bonding procedures performed after @ref sd_ble_opt_set returns. //���IRK��ˢ�£��µ�IRK�ͻ���һ�����ַ������а󶨹���֮�� sd_ble_opt_set �ķ��ء�
 *
 * @retval ::NRF_SUCCESS Set or read successfully. //���û��ȡ�ɹ�
 * @retval ::NRF_ERROR_INVALID_ADDR The pointer to IRK storage is invalid. //NRF_ERROR_INVALID_ADDR ָ��IRK�洢����ָ������Ч�ġ�
 */
typedef struct
{
  ble_gap_irk_t * p_irk;        /**< When input: Pointer to custom IRK, or NULL to use/reset to the device's default IRK. When output: Pointer to where the current IRK is to be stored, or NULL to not read out the IRK. */
	//����ʱ��ָ���Զ���IRK��ָ�룬��NULL��ʹ��/���õ��豸��Ĭ��IRK�������ָ��ָ��ǰIRKҪ�洢��λ�ã�����NULL������IRK��
  uint16_t        interval_s;   /**< When input: Custom private address cycle interval in seconds. When output: The current private address cycle interval. */
	//����ʱ�����Ƶ�˽�е�ַѭ������ڼ������ڡ����ʱ����ǰ��˽�е�ַѭ�������
} ble_gap_opt_privacy_t;


/**@brief Scan request report option. //ɨ�����󱨸�ѡ�
 *
 *        This can be used with @ref sd_ble_opt_set to make the SoftDevice send
 *        @ref BLE_GAP_EVT_SCAN_REQ_REPORT events. //������� sd_ble_opt_set һ��ʹ�ã���ʹ���豸����BLE_GAP_EVT_SCAN_REQ_REPORT�¼���
 *
 *  @note   Due to the limited space reserved for scan request report events,//����Ϊɨ�����󱨸�ʱ��Ԥ���Ŀռ�����,
 *          not all received scan requests will be reported.//�����������յ���ɨ�����󶼽������档

 *
 *  @note   If whitelisting is used, only whitelisted requests are reported. //���ʹ�ð�������ֻ���������������
 *
 *  @retval ::NRF_SUCCESS Set successfully. //NRF_SUCCESS �ѳɹ����á�
 *  @retval ::NRF_ERROR_INVALID_STATE When advertising is ongoing while the option is set. //NRF_ERROR_INVALID_STATE ��ѡ����õ�ʱ��,�ڽ��й㲥��
 */
typedef struct
{
   uint8_t enable : 1;                           /**< Enable scan request reports. */ //ʹɨ�����󱨸档
} ble_gap_opt_scan_req_report_t;

/**@brief Compatibility mode option. //����ģʽ��ѡ��
 *
 *        This can be used with @ref sd_ble_opt_set to enable and disable //������� sd_ble_opt_set һ��ʹ�������û���ü���ģʽ������ģʽ��Ĭ�������ʱ���õġ�
 *        compatibility modes. Compatibility modes are disabled by default.
 *
 *  @note  Compatibility mode 1 enables interoperability with devices that do not support 
 *         a value of 0 for the WinOffset parameter in the Link Layer CONNECT_REQ packet. 
 *
//����ģʽ1 ֧���벻֧�ֵ��豸�Ļ�����������·�� CONNECT_REQ	���е�winƫ�Ʋ���ֵΪ0.

 *  @retval ::NRF_SUCCESS Set successfully. //NRF_SUCCESS �ѳɹ����á�
 *  @retval ::NRF_ERROR_INVALID_STATE When connection creation is ongoing while mode 1 is set. //NRF_ERROR_INVALID_STATE ��ģʽ1������ʱ�����Ӵ������ڽ����С�
 */
typedef struct
{
   uint8_t mode_1_enable : 1;                           /**< Enable compatibility mode 1.*/ //ʹ�ܼ���ģʽ1
} ble_gap_opt_compat_mode_t;


/**@brief Option structure for GAP options. */ //GAP �ṹѡ��
typedef union
{
  ble_gap_opt_ch_map_t                  ch_map;                    /**< Parameters for the Channel Map option. */ //ͨ��ӳ��ѡ��Ĳ���
  ble_gap_opt_local_conn_latency_t      local_conn_latency;        /**< Parameters for the Local connection latency option */ //���������ӳ�ѡ��Ĳ���
  ble_gap_opt_passkey_t                 passkey;                   /**< Parameters for the Passkey option.*/ //����Կ��ѡ��Ĳ�����
  ble_gap_opt_privacy_t                 privacy;                   /**< Parameters for the Custom privacy option. *///�Զ�����˽ѡ��Ĳ���
  ble_gap_opt_scan_req_report_t         scan_req_report;           /**< Parameters for the scan request report option.*/ //ɨ�����󱨸�ѡ��Ĳ�����
  ble_gap_opt_compat_mode_t             compat_mode;               /**< Parameters for the compatibility mode option.*/ //������ģʽѡ��Ĳ�����
} ble_gap_opt_t;
/**@} */


/**@addtogroup BLE_GAP_FUNCTIONS Functions //BLE_GAP_FUNCTIONS ���ܺ�����
 * @{ */

/**@brief Set local Bluetooth address. //���ñ���������ַ
 *
 * @note If the address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO, the address type is required to
 * be @ref BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE or
 * @ref BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE. The given address is ignored and the
 * SoftDevice will generate a new private address automatically every 
 * @ref BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S seconds. If this API
 * call is used again with the same parameters, the SoftDevice will immediately
 * generate a new private address to replace the current address.
 *
//�����ַѭ��ģʽ�� BLE_GAP_ADDR_CYCLE_MODE_AUTO����ַ���ͱ����� BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE�� BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE.
//�����ĵ�ַ�������ˣ�Э��ջ���Զ�����һ���µ�˽�˵�ַ BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S �롣
//������API�����ٴ�ʹ����ͬ�Ĳ��������豸������ʹ������һ���µ�˽�˵�ַ���滻��ǰ��ַ��

 * @note If the application wishes to use a @ref BLE_GAP_ADDR_TYPE_PUBLIC or
 * @ref BLE_GAP_ADDR_TYPE_RANDOM_STATIC address, the cycle mode must be
 * @ref BLE_GAP_ADDR_CYCLE_MODE_NONE.
 *
 ���Ӧ�ó���ϣ��ʹ�� BLE_GAP_ADDR_TYPE_PUBLIC �� BLE_GAP_ADDR_TYPE_RANDOM_STATIC ��ַ��ѭ��ģʽ������ BLE_GAP_ADDR_CYCLE_MODE_NOTE.
 
 * @note If this API function is called while advertising or scanning, the softdevice will immediately update the
 * advertising or scanning address without the need to stop the procedure in the following cases:
 *   - If the previously set address is of type @ref BLE_GAP_ADDR_TYPE_PUBLIC and the new address
 *   is also of type @ref BLE_GAP_ADDR_TYPE_PUBLIC
 *   - If the previously set address is not @ref BLE_GAP_ADDR_TYPE_PUBLIC and the new address is
 *   also not @ref BLE_GAP_ADDR_TYPE_PUBLIC.
 //����ڹ㲥��ɨ��ʱ�������API��������ôЭ��ջ�����̸��¹㲥��ɨ���ַ������Ҫ�����������ֹͣ����
 //�����ǰ���õĵ�ַ�� BLE_GAP_ADDR_TYPE_PUBLIC���µ�ַҲ�� BLE_GAP_ADDR_TYPE_PUBLIC ���͡�
 //�����ǰ���õĵ�ַ����BLE_GAP_ADDR_TYPE_PUBLIC���µ�ַҲ���� BLE_GAP_ADDR_TYPE_PUBLIC���͡�
 
 * If the address is changed from a @ref BLE_GAP_ADDR_TYPE_PUBLIC address to another type or from
 * another type to a @ref BLE_GAP_ADDR_TYPE_PUBLIC address, the change will take effect the next
 * time an advertising or scanning procedure is started.
 *
 //�����ַ�Ǵ� BLE_GAP_ADDR_TYPE_PUBLIC ��ַ��Ϊ��һ�����ͣ���һ�������� BLE_GAP_ADDR_TYPE_PUBLIC��ַ�����Ľ�����һ����Ч�������㲥��ɨ������ʱ�䡣
 
 * @note If the address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_NONE and the application is
 *       using privacy, the application must take care to generate and set new private addresses
 *       periodically to comply with the Privacy specification in Bluetooth Core Spec.
 
 //�����ַѭ��ģʽ�� BLE_GAP_ADDR_CYCLE_MODE_NONE,Ӧ�ó���ʹ����˽��Ӧ�ó������ע�����ɺ������µ�˽�е�ַ������ѭ�������Ĺ淶�е���˽�淶��
 
 *
 * @param[in] addr_cycle_mode Address cycle mode, see @ref BLE_GAP_ADDR_CYCLE_MODES. //��ַѭ��ģʽ���ο� BLE_GAP_ADDR_CYCLE_MODES��
 * @param[in] p_addr          Pointer to address structure. //ָ��ָ��ĵ�ַ�ṹ��
 *
 * @retval ::NRF_SUCCESS Address successfully set. //NRF_SUCCESS ��ַ���óɹ���
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADDR �ṩ��Ч��ָ�롣
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameters. //NRF_ERROR_INVALID_PARAM ��Ч�Ĳ�����
 * @retval ::BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid address. //BLE_ERROR_GAP_INVALID_BLE_ADDR ��Ч�ĵ�ַ
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry. //NRF_ERROR_BUSY ��ջæ������ δ���¼������ԡ�
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation. //NRF_ERROR_INVALID_STATE ��Ч״ִ̬�в�����
 */
SVCALL(SD_BLE_GAP_ADDRESS_SET, uint32_t, sd_ble_gap_address_set(uint8_t addr_cycle_mode, const ble_gap_addr_t *p_addr));


/**@brief Get local Bluetooth address. //����������ַ��
 *
 * @param[out] p_addr Pointer to address structure to be filled in. // p_addr ָ��Ҫ���ĵ�ַ�ṹ��ָ�롣
 *
 * @retval ::NRF_SUCCESS Address successfully retrieved. //NRF_SUCCESS ��ַ�ɹ��ؼ���
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADDR �ṩ��ָ����Ч��
 */
SVCALL(SD_BLE_GAP_ADDRESS_GET, uint32_t, sd_ble_gap_address_get(ble_gap_addr_t *p_addr));


/**@brief Set, clear or update advertising and scan response data. //���á��������¹㲥��ɨ����Ӧ���ݡ�
 *
 * @note The format of the advertising data will be checked by this call to ensure interoperability. //�㲥���ݵĸ�ʽ��ͨ��������ý��м�飬��ȷ���������ԡ�
 *       Limitations imposed by this API call to the data provided include having a flags data type in the scan response data and
 *       duplicating the local name in the advertising data and scan response data. 
 *
 //���API�������ṩ�����ݵ����ư�����ɨ����Ӧ��������һ����־�������͡�
 //�ڹ㲥���ݺ�ɨ����Ӧ�����и��Ʊ������ơ�
 
 * @note To clear the advertising data and set it to a 0-length packet, simply provide a valid pointer (p_data/p_sr_data) with its corresponding 
 *        length (dlen/srdlen) set to 0.
 //Ϊ������㲥���ݲ���������Ϊ0���ȵ����ݰ���ֻ���ṩһ����Ч��ָ�� (p_data/p_sr_data)�����Ӧ�����ݳ���(dlen/srdlen)��Ϊ0��
 
 *
 * @note The call will fail if p_data and p_sr_data are both NULL since this would have no effect.
 
 //���p_data �� p_sr_data����NULL����ô���þͻ�ʧ�ܣ���Ϊ�ⲻ������κ�Ӱ�졣
 
 *
 * @param[in] p_data    Raw data to be placed in advertising packet. If NULL, no changes are made to the current advertising packet data.
 //��ԭʼ���ݷ��ڹ㲥���У����NULL���Ե�ǰ�Ĺ㲥������û���κθ��ġ�
 * @param[in] dlen      Data length for p_data. Max size: @ref BLE_GAP_ADV_MAX_SIZE octets. Should be 0 if p_data is NULL, can be 0 if p_data is not NULL.
 //p_data ���ݳ���.���ߴ磺 BLE_GAP_ADV_MAX_SIZE �ֽڡ���� p_data Ϊ�գ������Ϊ0�����p_data����NULL���������0.
 
 * @param[in] p_sr_data Raw data to be placed in scan response packet. If NULL, no changes are made to the current scan response packet data.
 //��ԭʼ���ݷ�����ɨ����Ӧ���С����NULL���Ե�ǰɨ����Ӧ������û�����κθ��ġ�
 
 * @param[in] srdlen    Data length for p_sr_data. Max size: @ref BLE_GAP_ADV_MAX_SIZE octets. Should be 0 if p_sr_data is NULL, can be 0 if p_data is not NULL.
 //p_sr_data���ݳ��ȡ����ߴ磺 BLE_GAP_ADV_MAX_SIZE �ֽڡ� ��� p_sr_data Ϊ�գ���Ӧ��Ϊ0�����p_data ����NULL�������Ϊ0.
 *
 * @retval ::NRF_SUCCESS Advertising data successfully updated or cleared. //NRF_SUCCESS �㲥���ݳɹ����»������
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADR �ṩ��Ч��ָ�롣
 * @retval ::NRF_ERROR_INVALID_FLAGS Invalid combination of advertising flags supplied. //NRF_ERROR_INVALID_FLAGS �ṩ����Ч�Ĺ㲥��־��ϡ�
 * @retval ::NRF_ERROR_INVALID_DATA Invalid data type(s) supplied, check the advertising data format specification. //NRF_ERROR_INVALID_DATA �ṩ����Ч����������(s) ,���㲥����ʽ�淶��
 * @retval ::NRF_ERROR_INVALID_LENGTH Invalid data length(s) supplied. //NRF_ERROR_INVALID_LENGTH �ṩ�����ݳ�����Ч��
 * @retval ::NRF_ERROR_NOT_SUPPORTED Unsupported data type. //NRF_ERROR_NOT_SUPPORTED ��֧�ֵ��������͡�
 * @retval ::BLE_ERROR_GAP_UUID_LIST_MISMATCH Invalid UUID list supplied. //BLE_ERROR_GAP_UUID_LIST_MISMATCH ��Ч��UUID�ṩ�б�
 */
SVCALL(SD_BLE_GAP_ADV_DATA_SET, uint32_t, sd_ble_gap_adv_data_set(uint8_t const *p_data, uint8_t dlen, uint8_t const *p_sr_data, uint8_t srdlen));


/**@brief Start advertising (GAP Discoverable, Connectable modes, Broadcast Procedure). //��ʼ�㲥 (GAP ���֣�������ģʽ���㲥����)
 *
 * @note An application can start an advertising procedure for broadcasting purposes while a connection
 *       is active. After a @ref BLE_GAP_EVT_CONNECTED event is received, this function may therefore
 *       be called to start a broadcast advertising procedure. The advertising procedure
 *       cannot however be connectable (it must be of type @ref BLE_GAP_ADV_TYPE_ADV_SCAN_IND or
 *       @ref BLE_GAP_ADV_TYPE_ADV_NONCONN_IND). @note Only one advertiser may be active at any time.
 *
 //Ӧ�ó������������ʱ�������ڹ㲥�Ĺ㲥�����һ�������ǻ�Ծ�ġ����յ� BLE_GAP_EVT_CONNECTED �¼�֮������������ܻ���ֱ�Ҫ������
 //һ���㲥���򣬹㲥���ǲ������ӡ��������� BLE_GAP_ADV_TYPE_ADV_SCAN_IND��BLE_GAP_ADV_TYPE_ADV_NONCONN_IND.�κ�ʱ��ֻ��һ���㲥�����ǻ�Ծ�ġ�
 
 
 * @param[in] p_adv_params Pointer to advertising parameters structure. //p_adv_parms  ָ��������ṹ��ָ�롣
 *
 * @retval ::NRF_SUCCESS The BLE stack has started advertising. //NRF_SUCCESS BLE �Ѿ���ʼ�㲥�ˡ�
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADDR �ṩ��ָ����Ч��
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation. //NRF_ERROR_INVALID_STATE ��Ч״ִ̬�в�����
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check the accepted ranges and limits. //NRF_ERROR_INVALID_PARAM �ṩ��Ч�Ĳ���������ѽ��ܵĹ淶�����ơ�
 * @retval ::BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid Bluetooth address supplied. //BLE_ERROR_GAP_INVALID_BLE_ADD �ṩ��������ַ��Ч��
 * @retval ::BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST Discoverable mode and whitelist incompatible. //BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST �ɷ���ģʽ�Ͱ����������ݡ�
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry. //NRF_ERROR_BUSY ��ջæ������δ���¼������ԡ�
 */
SVCALL(SD_BLE_GAP_ADV_START, uint32_t, sd_ble_gap_adv_start(ble_gap_adv_params_t const *p_adv_params));


/**@brief Stop advertising (GAP Discoverable, Connectable modes, Broadcast Procedure). //ֹͣ�㲥 (GAP���֣�������ģʽ���㲥����)
 *
 * @retval ::NRF_SUCCESS The BLE stack has stopped advertising. // NRF_SUCCESS  BLE��ֹͣ�˹㲥��
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation (most probably not in advertising state).
 //NRF_ERROR_INVALID_STATE ��Ч״ִ̬�в��� (�ܿ��ܲ��ٹ㲥״̬��)
 */
SVCALL(SD_BLE_GAP_ADV_STOP, uint32_t, sd_ble_gap_adv_stop(void));


/**@brief Update connection parameters. //�������Ӳ���
 *
 * @details In the central role this will initiate a Link Layer connection parameter update procedure, //�����Ľ�ɫ���⽫����һ����·������Ӳ������¹��̡�
 *          otherwise in the peripheral role, this will send the corresponding L2CAP request and wait for //��������Χ��ɫ�У��⽫������Ӧ��L2CAP���󲢵ȴ�ִ��������̵����ġ�
 *          the central to perform the procedure. In both cases, and regardless of success or failure, the application
 *          will be informed of the result with a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE event. //������������£����ܳɹ���ʧ�ܣ�Ӧ�ó���ͨ�� BLE_GAP_EVT_CONN_PARAM_UPDATE �¼�֪ͨ�����
 *
 * @details This function can be used as a central both to reply to a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST or to start the procedure unrequested.
 *
	//���������������Ӧ�� BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST�����ģ�Ҳ������������δ����Ĺ��̡�
	
 
 * @param[in] conn_handle Connection handle. //���Ӿ��
 * @param[in] p_conn_params  Pointer to desired connection parameters. If NULL is provided on a peripheral role,
 *                           the parameters in the PPCP characteristic of the GAP service will be used instead. //ָ���������Ӳ�����ָ�룬�������Χ��ɫ���ṩ��NULL����ʹ�ò������PPCP���ԵĲ��������档
 *                           If NULL is provided on a central role and in response to a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST, the peripheral request will be rejected
		�����һ�����Ľ�ɫ���ṩ��NULL��������Ӧ BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST����Χ���󽫱��ܾ���
 *
 * @retval ::NRF_SUCCESS The Connection Update procedure has been started successfully. //NRF_SUCCESS ���Ӹ��¹����Ѿ��ɹ�������
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADDR �ṩ��Ч��ָ�롣
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check parameter limits and constraints. //NRF_ERROR_INVALID_PARAM �ṩ����Ч�Ĳ��������������ƺ�Լ����
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.//NRF_ERROR_INVALID_STATE ��Ч״ִ̬�в�����
 * @retval ::NRF_ERROR_BUSY Procedure already in progress or not allowed at this time, process pending events and wait for pending procedures to complete and retry.
	//NRF_ERROR_BUSY �����ʱ���Ѿ��ڽ��л��߲�����Ĺ��̣�����δ���¼����ȴ��Ĺ�����ɺ����ԡ�
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied. //BLE_ERROR_INVALID_CONN_HANDLE �ṩ�����Ӿ����Ч��
 * @retval ::NRF_ERROR_NO_MEM Not enough memory to complete operation. //NRF_ERROR_NO_MEM û���㹻���ڴ�����ɲ�����
 */
SVCALL(SD_BLE_GAP_CONN_PARAM_UPDATE, uint32_t, sd_ble_gap_conn_param_update(uint16_t conn_handle, ble_gap_conn_params_t const *p_conn_params));


/**@brief Disconnect (GAP Link Termination). //�Ͽ� (ֹͣGAP ����)
 *
 * @details This call initiates the disconnection procedure, and its completion will be communicated to the application
 *          with a @ref BLE_GAP_EVT_DISCONNECTED event.
 //������������Ͽ����̣�������ɽ������ݸ�Ӧ�ó���ʹ�� BLE_GAP_EVT�Ͽ��¼���
 *
 * @param[in] conn_handle Connection handle. //���Ӿ��
 * @param[in] hci_status_code HCI status code, see @ref BLE_HCI_STATUS_CODES (accepted values are @ref BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION and @ref BLE_HCI_CONN_INTERVAL_UNACCEPTABLE).
 *
 //HCL״̬�룬�μ� BLE_HCL_STATUS_CODES (���յ�ֵ�� BLE_HCI_REMOTE_USER_TERMINATED_CONNCTION �� BLE_HCL_CONN_INTERVAL_UNACCEPTABLE).
 
 * @retval ::NRF_SUCCESS The disconnection procedure has been started successfully.
 //NRF_SUCCESS �Ͽ��Ĺ����Ѿ��ɹ�����
 
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 //NRF_ERROR_INVALID_PARAM �ṩ�Ĳ�����Ч��
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 //BLE_ERROR_INVALID_CONN_HANDLE �ṩ�����Ӿ����Ч��
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation (disconnection is already in progress).
 //NRF_ERROR_INVALID_STATE ��Ч״ִ̬�в���(�Ͽ��Ѿ��ڽ����С�)
 */
SVCALL(SD_BLE_GAP_DISCONNECT, uint32_t, sd_ble_gap_disconnect(uint16_t conn_handle, uint8_t hci_status_code));


/**@brief Set the radio's transmit power. �������ߴ��书��
 *
 * @param[in] tx_power Radio transmit power in dBm (accepted values are -40, -30, -20, -16, -12, -8, -4, 0, and 4 dBm). tx_power ���ߴ��书�� dbm ���յ�ֵ
 *
 * @note -40 dBm will not actually give -40 dBm, but will instead be remapped to -30 dBm. -40 dbmʵ���ϲ����ṩ-40dbm�����ǻᱻ��ӳ�䵽-30dbm
 *
 * @retval ::NRF_SUCCESS Successfully changed the transmit power. ���� NRF_SUCESS Successfuily ��ʾ�ɹ��ı��˹���ֵ
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied. ���� NRF_ERROR_INVALTD_PARAM ��ʾ������Ч
 */
SVCALL(SD_BLE_GAP_TX_POWER_SET, uint32_t, sd_ble_gap_tx_power_set(int8_t tx_power));


/**@brief Set GAP Appearance value. //����GAP ���ֵ��
 *
 * @param[in] appearance Appearance (16-bit), see @ref BLE_APPEARANCES. //���(16bit)���μ� BLE_APPEARANCES��
 *
 * @retval ::NRF_SUCCESS  Appearance value set successfully.//NRF_SUCCESS ���ֵ�ѳɹ�����
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied. //NRF_ERROR_INVALID_PARAM �ṩ�Ĳ�����Ч��
 */
SVCALL(SD_BLE_GAP_APPEARANCE_SET, uint32_t, sd_ble_gap_appearance_set(uint16_t appearance));


/**@brief Get GAP Appearance value. //��ȡ GAT ���ֵ
 *
 * @param[out] p_appearance Pointer to appearance (16-bit) to be filled in, see @ref BLE_APPEARANCES. //ָ�����ֵ(16bit)��ָ�룬�μ� BLE_APPEARANCES.
 *
 * @retval ::NRF_SUCCESS Appearance value retrieved successfully. // NRF_SUCESS ���ֵ��ȡ�ɹ���
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //�ṩ��ָ����Ч��
 */
SVCALL(SD_BLE_GAP_APPEARANCE_GET, uint32_t, sd_ble_gap_appearance_get(uint16_t *p_appearance));


/**@brief Set GAP Peripheral Preferred Connection Parameters. //����GAP ��Χ��ѡ�����Ӳ�����
 *
 * @param[in] p_conn_params Pointer to a @ref ble_gap_conn_params_t structure with the desired parameters.
 //p_conn_params ָ��ָ�� ble_agp_conn_params_t �ṹ������������Ĳ�����
 *
 * @retval ::NRF_SUCCESS Peripheral Preferred Connection Parameters set successfully.
 // NRF_SUCCESS ��Χ��ѡ���Ӳ����ɹ����á�
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 //�ṩ��ָ����Ч��
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 //�ṩ�Ĳ�����Ч��
 */
SVCALL(SD_BLE_GAP_PPCP_SET, uint32_t, sd_ble_gap_ppcp_set(ble_gap_conn_params_t const *p_conn_params));


/**@brief Get GAP Peripheral Preferred Connection Parameters.
//��� GAP ��Χ��ѡ�����Ӳ�����
 *
 * @param[out] p_conn_params Pointer to a @ref ble_gap_conn_params_t structure where the parameters will be stored.
 //ָ��ָ��һ�� ble_gap_conn_params_t �ṹ�����н��洢������
 *
 * @retval ::NRF_SUCCESS Peripheral Preferred Connection Parameters retrieved successfully.
 //�ɹ�������Χ�������Ӳ���
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 //�ṩ��Ч��ָ�롣
 */
SVCALL(SD_BLE_GAP_PPCP_GET, uint32_t, sd_ble_gap_ppcp_get(ble_gap_conn_params_t *p_conn_params));


/**@brief Set GAP device name. //���� GAP �豸��
 *
 * @param[in] p_write_perm Write permissions for the Device Name characteristic, see @ref ble_gap_conn_sec_mode_t.
 //Ϊ�豸������дȨ�ޣ���μ� ble_gap_conn_sec_mode_t.
 
 * @param[in] p_dev_name Pointer to a UTF-8 encoded, <b>non NULL-terminated</b> string.
 //ָ��UTF - 8 ����ģ��ǿ���ֹ���ַ�����ָ�롣
 
 * @param[in] len Length of the UTF-8, <b>non NULL-terminated</b> string pointed to by p_dev_name in octets (must be smaller or equal than @ref BLE_GAP_DEVNAME_MAX_LEN).
 *
//���ַ������� p_dev_name ָ��ķǿ���ֹ���ַ�������(����� BLE_GAP_DEVNAME_MAX_LEN��С�����)�� 
 
 * @retval ::NRF_SUCCESS GAP device name and permissions set successfully. //�ɹ�������GAP�豸����Ȩ�ޡ�
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //�ṩ��ָ����Ч��
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied. //�ṩ�Ĳ�����Ч��
 * @retval ::NRF_ERROR_DATA_SIZE Invalid data size(s) supplied.//�ṩ�����ݴ�С��Ч��
 */
SVCALL(SD_BLE_GAP_DEVICE_NAME_SET, uint32_t, sd_ble_gap_device_name_set(ble_gap_conn_sec_mode_t const *p_write_perm, uint8_t const *p_dev_name, uint16_t len));


/**@brief Get GAP device name. //��ȡ GAP �豸��
 *
 * @param[out]    p_dev_name Pointer to an empty buffer where the UTF-8 <b>non NULL-terminated</b> string will be placed. Set to NULL to obtain the complete device name length.
 * @param[in,out] p_len      Length of the buffer pointed by p_dev_name, complete device name length on output.
 *
 * @note          If the device name is longer than the size of the supplied buffer,
 *                p_len will return the complete device name length,
 *                and not the number of bytes actually returned in p_dev_name.
 *                The application may use this information to allocate a suitable buffer size.
 *
 * @retval ::NRF_SUCCESS GAP device name retrieved successfully.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_DATA_SIZE Invalid data size(s) supplied.
 */
SVCALL(SD_BLE_GAP_DEVICE_NAME_GET, uint32_t, sd_ble_gap_device_name_get(uint8_t *p_dev_name, uint16_t *p_len));


/**@brief Initiate the GAP Authentication procedure. //���� GAP��֤����
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_sec_params Pointer to the @ref ble_gap_sec_params_t structure with the security parameters to be used during the pairing or bonding procedure.
 *                         In the peripheral role, only the timeout, bond and mitm fields of this structure are used.
 *                         In the central role, this pointer may be NULL to reject a Security Request.
 *
 * @details In the central role, this function will send an SMP Pairing Request (or an SMP Pairing Failed if rejected), 
 *          otherwise in the peripheral role, an SMP Security Request will be sent.
 *
 * @note    Calling this function may result in the following events depending on the outcome and parameters: @ref BLE_GAP_EVT_SEC_PARAMS_REQUEST,
 *          @ref BLE_GAP_EVT_SEC_INFO_REQUEST, @ref BLE_GAP_EVT_AUTH_KEY_REQUEST, @ref BLE_GAP_EVT_CONN_SEC_UPDATE, @ref BLE_GAP_EVT_AUTH_STATUS.
 *
 *
 * @retval ::NRF_SUCCESS Successfully initiated authentication procedure.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @retval ::NRF_ERROR_TIMEOUT A SMP timout has occured, and further SMP operations on this link is prohibited.
 */
SVCALL(SD_BLE_GAP_AUTHENTICATE, uint32_t, sd_ble_gap_authenticate(uint16_t conn_handle, ble_gap_sec_params_t const *p_sec_params));


/**@brief Reply with GAP security parameters. //ʹ��GAP��ȫ��������Ӧ��
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] sec_status Security status, see @ref BLE_GAP_SEC_STATUS.
 * @param[in] p_sec_params Pointer to a @ref ble_gap_sec_params_t security parameters structure. In the central role this must be set to NULL, as the parameters have
 *                         already been provided during a previous call to @ref sd_ble_gap_authenticate.
 * @param[in,out] p_sec_keyset Pointer to a @ref ble_gap_sec_keyset_t security keyset structure. Any keys distributed as a result of the ongoing security procedure 
 *                         will be stored into the memory referenced by the pointers inside this structure. The keys will be stored and available to the application 
 *                         upon reception of a @ref BLE_GAP_EVT_AUTH_STATUS event.
 *                         The pointer to the ID key data distributed by the <b>local device</b> constitutes however an exception. It can either point to ID key data 
 *                         filled in by the user before calling this function, in which case a user-supplied Bluetooth address and IRK will be distributed, 
 *                         or the pointer to the ID key data structure can be NULL, in which case the device's configured (or default, if none is configured) 
 *                         Bluetooth address and IRK will be distributed. 
 *
 * @details This function is only used to reply to a @ref BLE_GAP_EVT_SEC_PARAMS_REQUEST, calling it at other times will result in an @ref NRF_ERROR_INVALID_STATE.
 * @note    If the call returns an error code, the request is still pending, and the reply call may be repeated with corrected parameters.
 *
 * @retval ::NRF_SUCCESS Successfully accepted security parameter from the application.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_SEC_PARAMS_REPLY, uint32_t, sd_ble_gap_sec_params_reply(uint16_t conn_handle, uint8_t sec_status, ble_gap_sec_params_t const *p_sec_params, ble_gap_sec_keyset_t const *p_sec_keyset));


/**@brief Reply with an authentication key. //�������֤��Կ�ظ���
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] key_type See @ref BLE_GAP_AUTH_KEY_TYPES.
 * @param[in] p_key If key type is @ref BLE_GAP_AUTH_KEY_TYPE_NONE, then NULL.
 *                If key type is @ref BLE_GAP_AUTH_KEY_TYPE_PASSKEY, then a 6-byte ASCII string (digit 0..9 only, no NULL termination).
 *                If key type is @ref BLE_GAP_AUTH_KEY_TYPE_OOB, then a 16-byte OOB key value in Little Endian format.
 *
 * @details This function is only used to reply to a @ref BLE_GAP_EVT_AUTH_KEY_REQUEST, calling it at other times will result in an @ref NRF_ERROR_INVALID_STATE.
 * @note    If the call returns an error code, the request is still pending, and the reply call may be repeated with corrected parameters.
 *
 * @retval ::NRF_SUCCESS Authentication key successfully set.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_AUTH_KEY_REPLY, uint32_t, sd_ble_gap_auth_key_reply(uint16_t conn_handle, uint8_t key_type, uint8_t const *p_key));


/**@brief Initiate GAP Encryption procedure. //����GAP���ܳ���
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_master_id Pointer to a @ref ble_gap_master_id_t master identification structure.
 * @param[in] p_enc_info  Pointer to a @ref ble_gap_enc_info_t encryption information structure.
 *
 * @details In the central role, this function will initiate the encryption procedure using the encryption information provided.
 *
 * @note    Calling this function may result in the following event depending on the outcome and parameters: @ref BLE_GAP_EVT_CONN_SEC_UPDATE.
 *
 * @retval ::NRF_SUCCESS Successfully initiated authentication procedure.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @retval ::BLE_ERROR_INVALID_ROLE Operation is not supported in the Peripheral role.
 * @retval ::NRF_ERROR_BUSY Procedure already in progress or not allowed at this time, wait for pending procedures to complete and retry.
 */
SVCALL(SD_BLE_GAP_ENCRYPT, uint32_t, sd_ble_gap_encrypt(uint16_t conn_handle, ble_gap_master_id_t const *p_master_id, ble_gap_enc_info_t const *p_enc_info));


/**@brief Reply with GAP security information. //��GAP��ȫ��Ϣ�ظ�
 *
 * @param[in] conn_handle Connection handle.
 * @param[in] p_enc_info Pointer to a @ref ble_gap_enc_info_t encryption information structure. May be NULL to signal none is available.
 * @param[in] p_id_info Pointer to a @ref ble_gap_irk_t identity information structure. May be NULL to signal none is available.
 * @param[in] p_sign_info Pointer to a @ref ble_gap_sign_info_t signing information structure. May be NULL to signal none is available.
 *
 * @details This function is only used to reply to a @ref BLE_GAP_EVT_SEC_INFO_REQUEST, calling it at other times will result in @ref NRF_ERROR_INVALID_STATE.
 * @note    If the call returns an error code, the request is still pending, and the reply call may be repeated with corrected parameters.
 * @note    Data signing is not yet supported, and p_sign_info must therefore be NULL.
 *
 * @retval ::NRF_SUCCESS Successfully accepted security information.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_SEC_INFO_REPLY, uint32_t, sd_ble_gap_sec_info_reply(uint16_t conn_handle, ble_gap_enc_info_t const *p_enc_info, ble_gap_irk_t const *p_id_info, ble_gap_sign_info_t const *p_sign_info));


/**@brief Get the current connection security. //��õ�ǰ�����Ӱ�ȫ��
 *
 * @param[in]  conn_handle Connection handle.
 * @param[out] p_conn_sec  Pointer to a @ref ble_gap_conn_sec_t structure to be filled in.
 *
 * @retval ::NRF_SUCCESS Current connection security successfully retrieved.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_CONN_SEC_GET, uint32_t, sd_ble_gap_conn_sec_get(uint16_t conn_handle, ble_gap_conn_sec_t *p_conn_sec));


/**@brief Start reporting the received signal strength to the application. //��ʼ��Ӧ�ó��򱨸���յ����ź�ǿ�ȡ�
 *
 * A new event is reported whenever the RSSI value changes, until @ref sd_ble_gap_rssi_stop is called.
 *
 * @param[in] conn_handle        Connection handle.
 * @param[in] threshold_dbm      Minimum change in dBm before triggering the @ref BLE_GAP_EVT_RSSI_CHANGED event. Events are disabled if threshold_dbm equals @ref BLE_GAP_RSSI_THRESHOLD_INVALID.
 * @param[in] skip_count         Number of RSSI samples with a change of threshold_dbm or more before sending a new @ref BLE_GAP_EVT_RSSI_CHANGED event.
 *
 * @retval ::NRF_SUCCESS                   Successfully activated RSSI reporting.
 * @retval ::NRF_ERROR_INVALID_STATE       Disconnection in progress. Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_RSSI_START, uint32_t, sd_ble_gap_rssi_start(uint16_t conn_handle, uint8_t threshold_dbm, uint8_t skip_count));


/**@brief Stop reporting the received signal strength. //ֹͣ������յ����ź�ǿ�ȡ�
 *
 * @note An RSSI change detected before the call but not yet received by the application 
 * may be reported after @ref sd_ble_gap_rssi_stop has been called.
 *
 * @param[in] conn_handle Connection handle.
 *
 * @retval ::NRF_SUCCESS                   Successfully deactivated RSSI reporting.
 * @retval ::NRF_ERROR_INVALID_STATE       Invalid state to perform operation.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_RSSI_STOP, uint32_t, sd_ble_gap_rssi_stop(uint16_t conn_handle));


/**@brief Get the received signal strength for the last connection event. //������һ�������¼��Ľ����ź�ǿ�ȡ�
 *
 * @param[in]  conn_handle Connection handle.
 * @param[out] p_rssi      Pointer to the location where the RSSI measurement shall be stored.
 *
 * @ref sd_ble_gap_rssi_start must be called to start reporting RSSI before using this function. @ref NRF_ERROR_NOT_FOUND
 * will be returned until RSSI was sampled for the first time after calling @ref sd_ble_gap_rssi_start.
 *
 * @retval ::NRF_SUCCESS                   Successfully read the RSSI.
 * @retval ::NRF_ERROR_NOT_FOUND           No sample is available.
 * @retval ::NRF_ERROR_INVALID_ADDR        Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 * @retval ::NRF_ERROR_INVALID_STATE       RSSI reporting is not ongoing, or disconnection in progress.
 */
SVCALL(SD_BLE_GAP_RSSI_GET, uint32_t, sd_ble_gap_rssi_get(uint16_t conn_handle, int8_t *p_rssi));


/**@brief Start scanning (GAP Discovery procedure, Observer Procedure). //��ʼɨ��(GAP���ֳ��򣬹۲��߳���)
 *
 * @param[in] p_scan_params Pointer to scan parameters structure.
 *
 * @retval ::NRF_SUCCESS Successfully initiated scanning procedure.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry.
 * @retval ::NRF_ERROR_NOT_SUPPORTED A selected feature is not supported (selective scanning).
 */
SVCALL(SD_BLE_GAP_SCAN_START, uint32_t, sd_ble_gap_scan_start(ble_gap_scan_params_t const *p_scan_params));


/**@brief Stop scanning (GAP Discovery procedure, Observer Procedure). //ֹͣɨ�� (GAP���ֳ��򣬹۲��߳���)
 *
 * @retval ::NRF_SUCCESS Successfully stopped scanning procedure.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation (most probably not in scanning state).
 */
SVCALL(SD_BLE_GAP_SCAN_STOP, uint32_t, sd_ble_gap_scan_stop(void));


/**@brief Create a connection (GAP Link Establishment). //����һ������(GAP ��·����)
 *
 * @param[in] p_peer_addr   Pointer to peer address. If the selective bit is set in @ref ble_gap_scan_params_t, then this must be NULL.
 * @param[in] p_scan_params Pointer to scan parameters structure.
 * @param[in] p_conn_params Pointer to desired connection parameters.
 *
 * @retval ::NRF_SUCCESS Successfully initiated connection procedure.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid parameter(s) pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 * @retval ::BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid Peer address.
 * @retval ::NRF_ERROR_NO_MEM limit of available connections reached.
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry.
*/
SVCALL(SD_BLE_GAP_CONNECT, uint32_t, sd_ble_gap_connect(ble_gap_addr_t const *p_peer_addr, ble_gap_scan_params_t const *p_scan_params, ble_gap_conn_params_t const *p_conn_params));


/**@brief Cancel a connection establishment. //ȡ�����ӽ�����
 *
 * @retval ::NRF_SUCCESS Successfully cancelled an ongoing connection procedure.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.
 */
SVCALL(SD_BLE_GAP_CONNECT_CANCEL, uint32_t, sd_ble_gap_connect_cancel(void));

/** @} */

#endif // BLE_GAP_H__

/**
  @}
*/
