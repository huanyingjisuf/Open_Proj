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

/** @file
 *
 * @defgroup ble_sdk_srv_common Common service definitions  //��������Ķ��塣(ͨ�÷�����)
 * @{
 * @ingroup ble_sdk_srv
 * @brief Constants, type definitions, and functions that are common to all services. (���������Ͷ�������з���ͨ�õĺ�����)
 */

#ifndef BLE_SRV_COMMON_H__
#define BLE_SRV_COMMON_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble_types.h"
#include "app_util.h"
#include "ble.h"
#include "ble_gap.h"
#include "ble_gatt.h"

/** @defgroup UUID_SERVICES Service UUID definitions  //����UUID���� (��������UUID)
 * @{ */
#define BLE_UUID_ALERT_NOTIFICATION_SERVICE                      0x1811     /**< Alert Notification service UUID. ����֪ͨ����UUID*/
#define BLE_UUID_BATTERY_SERVICE                                 0x180F     /**< Battery service UUID. ��ط���UUID*/
#define BLE_UUID_BLOOD_PRESSURE_SERVICE                          0x1810     /**< Blood Pressure service UUID. Ѫѹ����UUID*/
#define BLE_UUID_CURRENT_TIME_SERVICE                            0x1805     /**< Current Time service UUID. ��ǰʱ�����UUID */
#define BLE_UUID_CYCLING_SPEED_AND_CADENCE                       0x1816     /**< Cycling Speed and Cadence service UUID. �����ٶȺ�̤ƵUUID*/
#define BLE_UUID_LOCATION_AND_NAVIGATION_SERVICE                 0x1819     /**< Location and Navigation service UUID. λ�ú͵�������UUID*/
#define BLE_UUID_DEVICE_INFORMATION_SERVICE                      0x180A     /**< Device Information service UUID. �豸��Ϣ����UUID*/
#define BLE_UUID_GLUCOSE_SERVICE                                 0x1808     /**< Glucose service UUID. Ѫ�Ƿ���UUID*/
#define BLE_UUID_HEALTH_THERMOMETER_SERVICE                      0x1809     /**< Health Thermometer service UUID. ���¼�UUID*/
#define BLE_UUID_HEART_RATE_SERVICE                              0x180D     /**< Heart Rate service UUID. ���ʷ���UUID*/
#define BLE_UUID_HUMAN_INTERFACE_DEVICE_SERVICE                  0x1812     /**< Human Interface Device service UUID. �˻��ӿ��豸UUID*/
#define BLE_UUID_IMMEDIATE_ALERT_SERVICE                         0x1802     /**< Immediate Alert service UUID. ��ʱ��������UUID*/
#define BLE_UUID_LINK_LOSS_SERVICE                               0x1803     /**< Link Loss service UUID. ����ʧ�ܷ���UUID*/
#define BLE_UUID_NEXT_DST_CHANGE_SERVICE                         0x1807     /**< Next Dst Change service UUID. ��һ��Dst ���ķ���UUID*/
#define BLE_UUID_PHONE_ALERT_STATUS_SERVICE                      0x180E     /**< Phone Alert Status service UUID. �绰����״̬����UUID*/
#define BLE_UUID_REFERENCE_TIME_UPDATE_SERVICE                   0x1806     /**< Reference Time Update service UUID. �ο�ʱ����·���UUID*/
#define BLE_UUID_RUNNING_SPEED_AND_CADENCE                       0x1814     /**< Running Speed and Cadence service UUID. �ܲ��ٶȺͽ������UUID*/
#define BLE_UUID_SCAN_PARAMETERS_SERVICE                         0x1813     /**< Scan Parameters service UUID. ɨ���������UUID*/
#define BLE_UUID_TX_POWER_SERVICE                                0x1804     /**< TX Power service UUID. ���书�ʷ���UUID*/
#define BLE_UUID_IPSP_SERVICE                                    0x1820     /**< Internet Protocol Support service UUID. ������Э��֧�ַ���UUID*/
/** @} */

/** @defgroup UUID_CHARACTERISTICS Characteristic UUID definitions UUID����ֵ����
 * @{ */
#define BLE_UUID_BATTERY_LEVEL_STATE_CHAR                        0x2A1B     /**< Battery Level State characteristic UUID. ��صȼ�״̬����UUID*/
#define BLE_UUID_BATTERY_POWER_STATE_CHAR                        0x2A1A     /**< Battery Power State characteristic UUID. ��Դ״̬����UUID*/
#define BLE_UUID_REMOVABLE_CHAR                                  0x2A3A     /**< Removable characteristic UUID. ���ƶ�����UUID */
#define BLE_UUID_SERVICE_REQUIRED_CHAR                           0x2A3B     /**< Service Required characteristic UUID. ������������UUID*/
#define BLE_UUID_ALERT_CATEGORY_ID_CHAR                          0x2A43     /**< Alert Category Id characteristic UUID. ������ID����UUID*/
#define BLE_UUID_ALERT_CATEGORY_ID_BIT_MASK_CHAR                 0x2A42     /**< Alert Category Id Bit Mask characteristic UUID. ������IDλ��������UUID*/
#define BLE_UUID_ALERT_LEVEL_CHAR                                0x2A06     /**< Alert Level characteristic UUID. ���伶������UUID*/
#define BLE_UUID_ALERT_NOTIFICATION_CONTROL_POINT_CHAR           0x2A44     /**< Alert Notification Control Point characteristic UUID. ����֪ͨ���Ƶ�����UUID*/
#define BLE_UUID_ALERT_STATUS_CHAR                               0x2A3F     /**< Alert Status characteristic UUID. ����״̬����UUID*/
#define BLE_UUID_BATTERY_LEVEL_CHAR                              0x2A19     /**< Battery Level characteristic UUID. ��صȼ�����UUID*/
#define BLE_UUID_BLOOD_PRESSURE_FEATURE_CHAR                     0x2A49     /**< Blood Pressure Feature characteristic UUID. Ѫѹ��������UUID*/
#define BLE_UUID_BLOOD_PRESSURE_MEASUREMENT_CHAR                 0x2A35     /**< Blood Pressure Measurement characteristic UUID. Ѫѹ��������UUID*/
#define BLE_UUID_BODY_SENSOR_LOCATION_CHAR                       0x2A38     /**< Body Sensor Location characteristic UUID. �������������UUID*/
#define BLE_UUID_BOOT_KEYBOARD_INPUT_REPORT_CHAR                 0x2A22     /**< Boot Keyboard Input Report characteristic UUID. �������뱨������*/
#define BLE_UUID_BOOT_KEYBOARD_OUTPUT_REPORT_CHAR                0x2A32     /**< Boot Keyboard Output Report characteristic UUID. ���������������*/
#define BLE_UUID_BOOT_MOUSE_INPUT_REPORT_CHAR                    0x2A33     /**< Boot Mouse Input Report characteristic UUID.������뱨������*/
#define BLE_UUID_CURRENT_TIME_CHAR                               0x2A2B     /**< Current Time characteristic UUID. ��������������*/
#define BLE_UUID_DATE_TIME_CHAR                                  0x2A08     /**< Date Time characteristic UUID. ����ʱ������UUID*/
#define BLE_UUID_DAY_DATE_TIME_CHAR                              0x2A0A     /**< Day Date Time characteristic UUID. ��������UUID*/
#define BLE_UUID_DAY_OF_WEEK_CHAR                                0x2A09     /**< Day Of Week characteristic UUID. ��������UUID*/
#define BLE_UUID_DST_OFFSET_CHAR                                 0x2A0D     /**< Dst Offset characteristic UUID. ����ʱ��������UUID*/
#define BLE_UUID_EXACT_TIME_256_CHAR                             0x2A0C     /**< Exact Time 256 characteristic UUID. ׼ȷʱ�� 256 ����UUID*/
#define BLE_UUID_FIRMWARE_REVISION_STRING_CHAR                   0x2A26     /**< Firmware Revision String characteristic UUID. �̼��汾�ַ�������UUID*/
#define BLE_UUID_GLUCOSE_FEATURE_CHAR                            0x2A51     /**< Glucose Feature characteristic UUID. Ѫ�ǹ�������UUID*/
#define BLE_UUID_GLUCOSE_MEASUREMENT_CHAR                        0x2A18     /**< Glucose Measurement characteristic UUID. Ѫ�ǲ�������UUID*/
#define BLE_UUID_GLUCOSE_MEASUREMENT_CONTEXT_CHAR                0x2A34     /**< Glucose Measurement Context characteristic UUID. Ѫ�ǲ�����������UUID*/
#define BLE_UUID_HARDWARE_REVISION_STRING_CHAR                   0x2A27     /**< Hardware Revision String characteristic UUID. Ӳ���޶��ַ�������UUID*/
#define BLE_UUID_HEART_RATE_CONTROL_POINT_CHAR                   0x2A39     /**< Heart Rate Control Point characteristic UUID. ���ʲ���������UUID*/
#define BLE_UUID_HEART_RATE_MEASUREMENT_CHAR                     0x2A37     /**< Heart Rate Measurement characteristic UUID. ���ʲ�������UUID*/
#define BLE_UUID_HID_CONTROL_POINT_CHAR                          0x2A4C     /**< Hid Control Point characteristic UUID. ���ؿ�������UUID*/
#define BLE_UUID_HID_INFORMATION_CHAR                            0x2A4A     /**< Hid Information characteristic UUID. ������Ϣ����UUID*/
#define BLE_UUID_IEEE_REGULATORY_CERTIFICATION_DATA_LIST_CHAR    0x2A2A     /**< IEEE Regulatory Certification Data List characteristic UUID. IEEE������֤�����б�����UUID*/
#define BLE_UUID_INTERMEDIATE_CUFF_PRESSURE_CHAR                 0x2A36     /**< Intermediate Cuff Pressure characteristic UUID. ������ѹ����UUID*/
#define BLE_UUID_INTERMEDIATE_TEMPERATURE_CHAR                   0x2A1E     /**< Intermediate Temperature characteristic UUID. ������������UUID*/
#define BLE_UUID_LOCAL_TIME_INFORMATION_CHAR                     0x2A0F     /**< Local Time Information characteristic UUID. ����ʱ����Ϣ����UUID*/
#define BLE_UUID_MANUFACTURER_NAME_STRING_CHAR                   0x2A29     /**< Manufacturer Name String characteristic UUID. �����������ַ�������UUID*/
#define BLE_UUID_MEASUREMENT_INTERVAL_CHAR                       0x2A21     /**< Measurement Interval characteristic UUID. �����������UUID*/
#define BLE_UUID_MODEL_NUMBER_STRING_CHAR                        0x2A24     /**< Model Number String characteristic UUID. �豸�ͺ�����UUID*/
#define BLE_UUID_UNREAD_ALERT_CHAR                               0x2A45     /**< Unread Alert characteristic UUID. δ����������UUID*/
#define BLE_UUID_NEW_ALERT_CHAR                                  0x2A46     /**< New Alert characteristic UUID. �¾�������UUID*/
#define BLE_UUID_PNP_ID_CHAR                                     0x2A50     /**< PNP Id characteristic UUID. PNP ID ����UUID*/
#define BLE_UUID_PROTOCOL_MODE_CHAR                              0x2A4E     /**< Protocol Mode characteristic UUID. Э��ģʽ����UUID*/
#define BLE_UUID_RECORD_ACCESS_CONTROL_POINT_CHAR                0x2A52     /**< Record Access Control Point characteristic UUID. ��¼������Ƶ�����UUID*/
#define BLE_UUID_REFERENCE_TIME_INFORMATION_CHAR                 0x2A14     /**< Reference Time Information characteristic UUID. �ο�ʱ����Ϣ����UUID*/
#define BLE_UUID_REPORT_CHAR                                     0x2A4D     /**< Report characteristic UUID. ��������UUID*/
#define BLE_UUID_REPORT_MAP_CHAR                                 0x2A4B     /**< Report Map characteristic UUID. �����ͼ����UUID*/
#define BLE_UUID_RINGER_CONTROL_POINT_CHAR                       0x2A40     /**< Ringer Control Point characteristic UUID. �������Ƶ�����UUID*/
#define BLE_UUID_RINGER_SETTING_CHAR                             0x2A41     /**< Ringer Setting characteristic UUID. ������������UUID*/
#define BLE_UUID_SCAN_INTERVAL_WINDOW_CHAR                       0x2A4F     /**< Scan Interval Window characteristic UUID. ɨ��������UUID*/
#define BLE_UUID_SCAN_REFRESH_CHAR                               0x2A31     /**< Scan Refresh characteristic UUID. ɨ��ˢ������UUID*/
#define BLE_UUID_SERIAL_NUMBER_STRING_CHAR                       0x2A25     /**< Serial Number String characteristic UUID. ���к��ַ�������UUID*/
#define BLE_UUID_SOFTWARE_REVISION_STRING_CHAR                   0x2A28     /**< Software Revision String characteristic UUID. ����޸��ַ�������UUID*/
#define BLE_UUID_SUPPORTED_NEW_ALERT_CATEGORY_CHAR               0x2A47     /**< Supported New Alert Category characteristic UUID. ֧���µľ����������UUID*/
#define BLE_UUID_SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR            0x2A48     /**< Supported Unread Alert Category characteristic UUID. ֧��δ�������������UUID*/
#define BLE_UUID_SYSTEM_ID_CHAR                                  0x2A23     /**< System Id characteristic UUID. ϵͳ��ʶ����UUID*/
#define BLE_UUID_TEMPERATURE_MEASUREMENT_CHAR                    0x2A1C     /**< Temperature Measurement characteristic UUID. �¶Ȳ�������UUID*/
#define BLE_UUID_TEMPERATURE_TYPE_CHAR                           0x2A1D     /**< Temperature Type characteristic UUID. �¶�����UUID����*/
#define BLE_UUID_TIME_ACCURACY_CHAR                              0x2A12     /**< Time Accuracy characteristic UUID. ʱ�侫������UUID*/
#define BLE_UUID_TIME_SOURCE_CHAR                                0x2A13     /**< Time Source characteristic UUID. ʱ��ԭ����UUID*/
#define BLE_UUID_TIME_UPDATE_CONTROL_POINT_CHAR                  0x2A16     /**< Time Update Control Point characteristic UUID. ʱ����¿��Ƶ�����UUID*/
#define BLE_UUID_TIME_UPDATE_STATE_CHAR                          0x2A17     /**< Time Update State characteristic UUID. ʱ�����״̬����UUID*/
#define BLE_UUID_TIME_WITH_DST_CHAR                              0x2A11     /**< Time With Dst characteristic UUID. ������ʱ��ʱ������UUID*/
#define BLE_UUID_TIME_ZONE_CHAR                                  0x2A0E     /**< Time Zone characteristic UUID. ʱ������UUID*/
#define BLE_UUID_TX_POWER_LEVEL_CHAR                             0x2A07     /**< TX Power Level characteristic UUID. TX���ʵȼ�����UUID*/
#define BLE_UUID_CSC_FEATURE_CHAR                                0x2A5C     /**< Cycling Speed and Cadence Feature characteristic UUID. �����ٶȺ�̤Ƶ����UUID*/
#define BLE_UUID_CSC_MEASUREMENT_CHAR                            0x2A5B     /**< Cycling Speed and Cadence Measurement characteristic UUID. �����ٶȺ�̤Ƶ����UUID*/
#define BLE_UUID_RSC_FEATURE_CHAR                                0x2A54     /**< Running Speed and Cadence Feature characteristic UUID. �ܲ��ٶȺͽ�������UUID*/
#define BLE_UUID_SC_CTRLPT_CHAR                                  0x2A55     /**< Speed and Cadence Control Point UUID. �ٶȺͽ�����Ƶ�UUID*/
#define BLE_UUID_RSC_MEASUREMENT_CHAR                            0x2A53     /**< Running Speed and Cadence Measurement characteristic UUID. �ܲ��ٶȺͽ����������UUID*/
#define BLE_UUID_SENSOR_LOCATION_CHAR                            0x2A5D     /**< Sensor Location characteristic UUID. ������λ������UUID*/
#define BLE_UUID_EXTERNAL_REPORT_REF_DESCR                       0x2907     /**< External Report Reference descriptor UUID. �ⲿ����ο�����UUID*/
#define BLE_UUID_REPORT_REF_DESCR                                0x2908     /**< Report Reference descriptor UUID. �ⲿ�ٿ�����UUID*/
#define BLE_UUID_LN_FEATURE_CHAR                                 0x2A6A     /**< Location Navigation Service, Feature characteristic UUID. λ�õ�����������UUID*/
#define BLE_UUID_LN_POSITION_QUALITY_CHAR                        0x2A69     /**< Location Navigation Service, Position quality UUID. λ�õ��з���λ������UUID*/
#define BLE_UUID_LN_LOCATION_AND_SPEED_CHAR                      0x2A67     /**< Location Navigation Service, Location and Speed characteristic UUID. λ�õ�������λ�ú��ٶ�����UUID*/
#define BLE_UUID_LN_NAVIGATION_CHAR                              0x2A68     /**< Location Navigation Service, Navigation characteristic UUID. ��λ�������񣬵�������UUID*/
#define BLE_UUID_LN_CONTROL_POINT_CHAR                           0x2A6B     /**< Location Navigation Service, Control point characteristic UUID. λ�õ������񣬿��Ƶ�����UUID*/
/** @} */

/** @defgroup ALERT_LEVEL_VALUES Definitions for the Alert Level characteristic values //������������ֵ�Ķ���
 * @{ */
#define BLE_CHAR_ALERT_LEVEL_NO_ALERT                            0x00       /**< No Alert. �޾���*/
#define BLE_CHAR_ALERT_LEVEL_MILD_ALERT                          0x01       /**< Mild Alert. ��Ⱦ���*/
#define BLE_CHAR_ALERT_LEVEL_HIGH_ALERT                          0x02       /**< High Alert. �߶Ⱦ���*/
/** @} */

#define BLE_SRV_ENCODED_REPORT_REF_LEN                           2          /**< The length of an encoded Report Reference Descriptor. //���뱨������������*/
#define BLE_CCCD_VALUE_LEN                                       2          /**< The length of a CCCD value. //CCCDֵ����*/

/**@brief Type definition for error handler function that will be called in case of an error in //���Ͷ�������������ڳ��������±���Ϊ���������ģ�顣
 *        a service or a service library module. */
typedef void (*ble_srv_error_handler_t) (uint32_t nrf_error);



/**@brief Value of a Report Reference descriptor.  //����������������ֵ
 *
 * @details This is mapping information that maps the parent characteristic to the Report ID(s) and //�⽫�Ǹ�����ӳ�䵽����ID(s)��ӳ����Ϣ��������(s) �����ڱ���ӳ�������С�
 *          Report Type(s) defined within a Report Map characteristic.
 */
typedef struct
{
    uint8_t report_id;                                  /**< Non-zero value if there is more than one instance of the same Report Type  �ж����ͬ���ͱ����ʵ��*/
    uint8_t report_type;                                /**< Type of Report characteristic (see @ref BLE_HIDS_REPORT_TYPE) ������������(�μ� BLE_HID_REPORT_TYPE)*/
} ble_srv_report_ref_t;

/**@brief UTF-8 string data type. // �ַ�����������
 *
 * @note The type can only hold a pointer to the string data (i.e. not the actual data). //����ֻ�ܳ���һ��ָ���ַ������ݵ�ָ�롣
 */
typedef struct
{
    uint16_t  length;                                   /**< String length. //�ַ�������*/
    uint8_t * p_str;                                    /**< String data. //�ַ�������*/
} ble_srv_utf8_str_t;

 
/**@brief Security settings structure. //��ȫ���õĽṹ��
 * @details This structure contains the security options needed during initialization of the
 *          service. //����ṹ�������ڳ�ʼ����������Ҫ�İ�ȫѡ�����
 */
typedef struct
{
    ble_gap_conn_sec_mode_t read_perm;                  /**< Read permissions. */
    ble_gap_conn_sec_mode_t write_perm;                 /**< Write permissions. */
} ble_srv_security_mode_t;

/**@brief Security settings structure. //��ȫ���õĽṹ��
 * @details This structure contains the security options needed during initialization of the
 *          service. It can be used when the characteristics contains a CCCD. //����ṹ�������ڳ�ʼ����������Ҫ�İ�ȫѡ����񡣵���������CCCDʱ������ʹ������
 */
typedef struct
{
    ble_gap_conn_sec_mode_t cccd_write_perm;            /**< Write permissions for Client Characteristic Configuration Descriptor. //Ϊ�ͻ���������������дȨ��*/
    ble_gap_conn_sec_mode_t read_perm;                  /**< Read permissions. //��Ȩ��*/
    ble_gap_conn_sec_mode_t write_perm;                 /**< Write permissions. //дȨ��*/
} ble_srv_cccd_security_mode_t;

/**@brief Function for decoding a CCCD value, and then testing if notification is
 *        enabled. //����CCCDֵ�ĺ�����Ȼ������Ƿ�����֪ͨ��
 *
 * @param[in]   p_encoded_data   Buffer where the encoded CCCD is stored. //�洢�ѱ����CCCD�Ļ�������
 *
 * @retval      TRUE If notification is enabled. //���֪ͨ�����ã������档
 * @retval      FALSE Otherwise.        //������Ǵ���
 */
static __INLINE bool ble_srv_is_notification_enabled(uint8_t * p_encoded_data)
{
    uint16_t cccd_value = uint16_decode(p_encoded_data);
    return ((cccd_value & BLE_GATT_HVX_NOTIFICATION) != 0);
}
    
/**@brief Function for decoding a CCCD value, and then testing if indication is
 *        enabled. //����CCCDֵ�ĺ�����Ȼ������Ƿ���ָʾ���á�
 *
 * @param[in]   p_encoded_data   Buffer where the encoded CCCD is stored. //�洢�ѱ����CCCD�Ļ�������
 *
 * @retval      TRUE If indication is enabled. �����ָʾ�����ã�������
 * @retval      FALSE Otherwise. //�����Ǽ١�
 */
static __INLINE bool ble_srv_is_indication_enabled(uint8_t * p_encoded_data)
{
    uint16_t cccd_value = uint16_decode(p_encoded_data);
    return ((cccd_value & BLE_GATT_HVX_INDICATION) != 0);
}

/**@brief Function for encoding a Report Reference Descriptor. //���ڱ��뱨�������������ĺ�����
 *
 * @param[in]   p_encoded_buffer  The buffer of the encoded data. //���ݱ���ĵĻ�������
 * @param[in]   p_report_ref      Report Reference value to be encoded. //����ο�ֵ�����롣
 *
 * @return      Length of the encoded data. //�������ݵĳ��ȡ�
 */
uint8_t ble_srv_report_ref_encode(uint8_t *                    p_encoded_buffer,
                                  const ble_srv_report_ref_t * p_report_ref);

/**@brief Function for making a UTF-8 structure refer to an ASCII string. //��������UTF - 8 �ṹ�ĺ�����ָASCII�ַ�����
 *
 * @param[out]  p_utf8   UTF-8 structure to be set.  //Ҫ����UTF - 8 �ṹ
 * @param[in]   p_ascii  ASCII string to be referred to. // Ҫ���õ�ASCII���ַ�����
 */
void ble_srv_ascii_to_utf8(ble_srv_utf8_str_t * p_utf8, char * p_ascii);


/**@brief Security Access enumeration. //��ȫ����ö��
 * @details This enumeration gives the possible requirements for accessing a characteristic value. //���ö�ٸ����˷�������ֵ�Ŀ�������
 */
typedef enum
{
    SEC_NO_ACCESS    = 0,            /**< Not possible to access. //�޷�����*/
    SEC_OPEN         = 1,            /**< Access open. //���ŷ���*/
		SEC_JUST_WORKS   = 2,            /**< Access possible with 'Just Works' security at least. //���ٿ���ʹ��"ֻ�ǹ���"�İ�ȫ*/
    SEC_MITM         = 3,            /**< Access possible with 'MITM' security at least.  //���ٿ���ʹ��"MTM"��ȫ����*/
    SEC_SIGNED       = 4,            /**< Access possible with 'signed' security at least. //���ٿ���ʹ��"ǩ���İ�ȫ����"*/
    SEC_SIGNED_MITM  = 5             /**< Access possible with 'signed and MITM' security at least. //���ٿ���ʹ��"ǩ����MITM�İ�ȫ��"*/
}security_req_t;


/**@brief Characteristic User Descriptor parameters. //�û���������������
 * @details This structure contains the parameters for User Descriptor. //����ṹ�����û��������Ĳ���
 */
typedef struct
{
    uint16_t               max_size;                      /**< Maximum size of the user descriptor //�û�������������С */
    uint16_t               size;                          /**< Size of the user descriptor //�û��������Ĵ�С */
    uint8_t                *p_char_user_desc;             /**< User descriptor content, pointer to a UTF-8 encoded string (non-NULL terminated) //�û������������ݣ�ָ��UTF - 8 �����ַ�����ָ��(�ǿ���ֹ)*/
    bool                   is_var_len;                    /**< Indicates if the user descriptor has variable length.//��ʾ�û��������Ƿ���пɱ䳤��*/
    ble_gatt_char_props_t  char_props;                    /**< user descriptor properties.//�û�����������*/
    bool                   is_defered_read;               /**< Indicate if deferred read operations are supported.//ָʾ�Ƿ�֧���ӳٶ�����*/
    bool                   is_defered_write;              /**< Indicate if deferred write operations are supported.//ָʾ�Ƿ�֧���ӳ�д����*/
    security_req_t         read_access;                   /**< Security requirement for reading the user descriptor.//��ȡ�û��������İ�ȫ����*/
    security_req_t         write_access;                  /**< Security requirement for writing the user descriptor.//��д�û������İ�ȫ����*/
    bool                   is_value_user;                 /**< Indicate if the content of the characteristic is to be stored in the application (user) or in the stack.//ָʾ�����Ե������Ƿ�洢��Ӧ�ó���(�û�)���ջ�С� */
}ble_add_char_user_desc_t;


/**@brief Add characteristic parameters structure.//������������ṹ
 * @details This structure contains the parameters needed to use the @ref characteristic_add function. //����ṹ������ʹ�� ������Ӻ�������Ĳ�����
 */
typedef struct
{
    uint16_t                    uuid;                     /**< Characteristic UUID (16 bits UUIDs).//����UUID(16λUUID)*/
    uint8_t                     uuid_type;                /**< Base UUID. If 0, the Bluetooth SIG UUID will be used. Otherwise, this should be a value returned by @ref sd_ble_uuid_vs_add when adding the base UUID.*/
																														//����UUID��Ϊ0ʱ����ʹ����������UUID��������Ӧ����sd_ble_uuid_vs_add ����ӻ���UUIDʱ�ķ��ص�ֵ��
    uint16_t                    max_len;                  /**< Maximum length of the characteristic value.//����ֵ����󳤶�*/
    uint16_t                    init_len;                 /**< Initial length of the characteristic value.//����ֵ�ĳ�ʼ����*/
    uint8_t *                   p_init_value;             /**< Initial encoded value of the characteristic.//�����Եĳ�ʼ����ֵ*/
    bool                        is_var_len;               /**< Indicates if the characteristic value has variable length.//��ʾ����ֵ�Ƿ���пɱ䳤�ȡ�*/
    ble_gatt_char_props_t       char_props;               /**< Characteristic properties.//��������*/
    bool                        is_defered_read;          /**< Indicate if deferred read operations are supported.//ָʾ�Ƿ�֧���ӳٶ�����*/
    bool                        is_defered_write;         /**< Indicate if deferred write operations are supported.//ָʾ�Ƿ�֧���ӳ�д����*/
    security_req_t              read_access;              /**< Security requirement for reading the characteristic value.//��ȡ����ֵ�İ�ȫ��Ҫ��*/
    security_req_t              write_access;             /**< Security requirement for writing the characteristic value.//��д����ֵ�İ�ȫ��Ҫ��*/
    security_req_t              cccd_write_access;        /**< Security requirement for writing the characteristic's CCCD.//��д���Ե�CCCD�İ�ȫ��Ҫ��*/
    bool                        is_value_user;            /**< Indicate if the content of the characteristic is to be stored in the application (user) or in the stack.//ָʾ�����Ե������Ƿ�洢��Ӧ�ó���(�û�)���ջ�С�*/
    ble_add_char_user_desc_t    *p_user_descr;            /**< Pointer to user descriptor if needed //�����Ҫ��ָ��ָ���û�������*/
    ble_gatts_char_pf_t         *p_presentation_format;   /**< Pointer to characteristic format if needed //�����Ҫ��ָ��ָ��������ʽ */
} ble_add_char_params_t;


/**@brief Add descriptor parameters structure. //��������������ṹ
 * @details This structure contains the parameters needed to use the @ref descriptor_add function. //����ṹ����ʹ�� descriptor_add ��������Ĳ�����
 */
typedef struct
{
    uint16_t       uuid;                     /**< descriptor UUID (16 bits UUIDs).//������UUID(16λUUID)*/
    uint8_t        uuid_type;                /**< Base UUID. If 0, the Bluetooth SIG UUID will be used. Otherwise, this should be a value returned by @ref sd_ble_uuid_vs_add when adding the base UUID.*/
																							//����UUID��Ϊ0ʱ����ʹ������SIGUUID��������Ӧ���� sd_ble_uuid_vs_add ����ӻ���UUIDʱ���ص�ֵ��
		bool           is_defered_read;          /**< Indicate if deferred read operations are supported.//ָʾ�Ƿ�֧���ӳٶ�����*/
    bool           is_defered_write;         /**< Indicate if deferred write operations are supported.//ָʾ�Ƿ�֧���ӳ�д����*/
    bool           is_var_len;               /**< Indicates if the descriptor value has variable length.//��ʾ������ֵ�Ƿ���пɱ䳤��*/
    security_req_t read_access;              /**< Security requirement for reading the descriptor value.//��ȡ������ֵ�İ�ȫ��Ҫ��*/
    security_req_t write_access;             /**< Security requirement for writing the descriptor value.//��д������ֵ�İ�ȫ��Ҫ��*/
    bool           is_value_user;            /**< Indicate if the content of the characteristic is to be stored in the application (user) or in the stack.//ָʾ�����Ե������Ƿ�洢��Ӧ�ó���(�û�)���ջ��*/
    uint16_t       init_len;                 /**< Initial descriptor value length in bytes. //��ʼ������ֵ�������ֽ�Ϊ��λ*/
    uint16_t       init_offs;                /**< Initial descriptor value offset in bytes. If different from zero, the first init_offs bytes of the attribute value will be left uninitialized. */
																							//��ʼ����ֵ���ֽ���ƫ�ơ������ͬ���㣬��ô����ֵ�ĵ�һ��init_offs �ֽڽ����ᱻ��ʼ����
    uint16_t       max_len;                  /**< Maximum descriptor value length in bytes, see @ref BLE_GATTS_ATTR_LENS_MAX for maximum values. */
																						//����������ֵ���ȣ������ BLE_GATTS_ATTR_LENS_MAX �Ի������ֵ��
    uint8_t*       p_value;                  /**< Pointer to the value of the descriptor*/
																						//ָ��ָ����������ֵ
} ble_add_descr_params_t;


/**@brief Function for adding a characteristic to a given service.  //Ϊ����������������ĺ���
 *
 * If no pointer is given for the initial value, //���û�и�����ʼֵ��ָ��
 * the initial length parameter will be ignored and the initial length will be 0. 
 //��ʼֵ���Ȳ����������ԣ���ʼ���Ƚ�Ϊ0.


 *
 * @param[in]  service_handle Handle of the service to which the characteristic is to be added. //Ҫ������Է���ľ��

 * @param[in]  p_char_props   Information needed to add the characteristic. //��������������Ϣ
 * @param[out] p_char_handle  Handle of the added characteristic. //�������ԵĴ���
 *
 * @retval      NRF_SUCCESS If the characteristic was added successfully. Otherwise, an error code is returned. //�����Ա��ɹ�������ˣ����ش�ֵ������᷵��һ�������롣
 */
uint32_t characteristic_add(uint16_t                   service_handle,
                            ble_add_char_params_t *    p_char_props,
                            ble_gatts_char_handles_t * p_char_handle);


/**@brief Function for adding a characteristic's descriptor to a given characteristic. //��������������ӵ����������ĺ�����
 *
 * @param[in]  char_handle    Handle of the characteristic to which the descriptor is to be added, if @ref BLE_GATT_HANDLE_INVALID is used, it will be placed sequentially.
 //Ҫ��������������ľ����������� BLE_GATT_HANDLE_INVALID ��Ч��������˳����á�
 * @param[in]  p_descr_props  Information needed to add the descriptor.
 //����������������Ϣ��
 * @param[out] p_descr_handle Handle of the added descriptor.
 //����������ľ����
 *
 * @retval      NRF_SUCCESS If the characteristic was added successfully. Otherwise, an error code is returned.
 //��������Ա��ɹ�������ˣ��������������᷵��һ�������롣
 */
uint32_t descriptor_add(uint16_t                   char_handle,
                        ble_add_descr_params_t *   p_descr_props,
                        uint16_t *                 p_descr_handle);


#endif // BLE_SRV_COMMON_H__

/** @} */
