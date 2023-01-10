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
 *   derived from this software without specific prior written permission.
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
  @addtogroup BLE_COMMON
  @{
  @defgroup ble_types Common types and macro definitions //�������ͺͺ궨��
  @{

  @brief Common types and macro definitions for the BLE SoftDevice. //����Э��ջ�ĳ������ͺͺ궨�塣
 */

#ifndef BLE_TYPES_H__
#define BLE_TYPES_H__

#include <stdint.h>

/** @addtogroup BLE_TYPES_DEFINES Defines
 * @{ */

/** @defgroup BLE_CONN_HANDLES BLE Connection Handles //BLE���Ӿ��
 * @{ */
#define BLE_CONN_HANDLE_INVALID 0xFFFF  /**< Invalid Connection Handle. //��Ч�����Ӿ��*/
#define BLE_CONN_HANDLE_ALL     0xFFFE  /**< Applies to all Connection Handles. //�������������Ӿ��*/
/** @} */


/** @defgroup BLE_UUID_VALUES Assigned Values for BLE UUIDs //ΪBLE UUIDs �����ֵ
 * @{ */
/* Generic UUIDs, applicable to all services */ //ͨ��UUIDs�����������з���
#define BLE_UUID_UNKNOWN                              0x0000 /**< Reserved UUID. //����UUID*/
#define BLE_UUID_SERVICE_PRIMARY                      0x2800 /**< Primary Service. //��Ҫ����*/
#define BLE_UUID_SERVICE_SECONDARY                    0x2801 /**< Secondary Service. //���η���*/
#define BLE_UUID_SERVICE_INCLUDE                      0x2802 /**< Include. //����*/
#define BLE_UUID_CHARACTERISTIC                       0x2803 /**< Characteristic. //����*/
#define BLE_UUID_DESCRIPTOR_CHAR_EXT_PROP             0x2900 /**< Characteristic Extended Properties Descriptor. //��չ����������������*/
#define BLE_UUID_DESCRIPTOR_CHAR_USER_DESC            0x2901 /**< Characteristic User Description Descriptor. //�û�����������������*/
#define BLE_UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG        0x2902 /**< Client Characteristic Configuration Descriptor.//�ͻ��ص������������� */
#define BLE_UUID_DESCRIPTOR_SERVER_CHAR_CONFIG        0x2903 /**< Server Characteristic Configuration Descriptor. //����������������������*/
#define BLE_UUID_DESCRIPTOR_CHAR_PRESENTATION_FORMAT  0x2904 /**< Characteristic Presentation Format Descriptor. //������ʾ��ʽ��������*/
#define BLE_UUID_DESCRIPTOR_CHAR_AGGREGATE_FORMAT     0x2905 /**< Characteristic Aggregate Format Descriptor. //�����ۺϸ�ʽ��������*/
/* GATT specific UUIDs  GATT ���� UUIDs*/
#define BLE_UUID_GATT                                 0x1801 /**< Generic Attribute Profile. //ͨ�����������ļ�*/
#define BLE_UUID_GATT_CHARACTERISTIC_SERVICE_CHANGED  0x2A05 /**< Service Changed Characteristic. //����ı��ص�*/
/* GAP specific UUIDs //GAP ���� UUIDs*/
#define BLE_UUID_GAP                                  0x1800 /**< Generic Access Profile. //ͨ�÷��������ļ�*/
#define BLE_UUID_GAP_CHARACTERISTIC_DEVICE_NAME       0x2A00 /**< Device Name Characteristic.//�豸���Ƶ��ص� */
#define BLE_UUID_GAP_CHARACTERISTIC_APPEARANCE        0x2A01 /**< Appearance Characteristic. //������� */
#define BLE_UUID_GAP_CHARACTERISTIC_PPF               0x2A02 /**< Peripheral Privacy Flag Characteristic. //��Χ�豸��˽λ����*/
#define BLE_UUID_GAP_CHARACTERISTIC_RECONN_ADDR       0x2A03 /**< Reconnection Address Characteristic.//�������ӵ�ַ���ص� */
#define BLE_UUID_GAP_CHARACTERISTIC_PPCP              0x2A04 /**< Peripheral Preferred Connection Parameters Characteristic.//��Χ�������Ӳ������� */
/** @} */


/** @defgroup BLE_UUID_TYPES Types of UUID ���͵� UUID
 * @{ */
#define BLE_UUID_TYPE_UNKNOWN       0x00 /**< Invalid UUID type.//��Ч��UUID����  */
#define BLE_UUID_TYPE_BLE           0x01 /**< Bluetooth SIG UUID (16-bit). //������������UUID(16-bit)*/
#define BLE_UUID_TYPE_VENDOR_BEGIN  0x02 /**< Vendor UUID types start at this index (128-bit). //��Ӧ��UUID���ʹ��������(128λ)��ʼ*/
/** @} */


/** @defgroup BLE_APPEARANCES Bluetooth Appearance values //�������ֵ��
 *  @note Retrieved from http://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
 * @{ */
#define BLE_APPEARANCE_UNKNOWN                                0 /**< Unknown. //δ֪������*/
#define BLE_APPEARANCE_GENERIC_PHONE                         64 /**< Generic Phone. //һ����ֻ�*/
#define BLE_APPEARANCE_GENERIC_COMPUTER                     128 /**< Generic Computer.//һ��ļ���� */
#define BLE_APPEARANCE_GENERIC_WATCH                        192 /**< Generic Watch. //һ����ֱ�*/
#define BLE_APPEARANCE_WATCH_SPORTS_WATCH                   193 /**< Watch: Sports Watch. //�ֱ��˶��ֱ�*/
#define BLE_APPEARANCE_GENERIC_CLOCK                        256 /**< Generic Clock. //ͨ�õ�ʱ��*/
#define BLE_APPEARANCE_GENERIC_DISPLAY                      320 /**< Generic Display. //ͨ�õ���ʾ*/
#define BLE_APPEARANCE_GENERIC_REMOTE_CONTROL               384 /**< Generic Remote Control. //ͨ��ң����*/
#define BLE_APPEARANCE_GENERIC_EYE_GLASSES                  448 /**< Generic Eye-glasses. //ͨ���۾�*/
#define BLE_APPEARANCE_GENERIC_TAG                          512 /**< Generic Tag. //ͨ�� ��ǩ*/
#define BLE_APPEARANCE_GENERIC_KEYRING                      576 /**< Generic Keyring. //ͨ�ü���*/
#define BLE_APPEARANCE_GENERIC_MEDIA_PLAYER                 640 /**< Generic Media Player. //ͨ�ò�����*/
#define BLE_APPEARANCE_GENERIC_BARCODE_SCANNER              704 /**< Generic Barcode Scanner.//ͨ��������ɨ���� */
#define BLE_APPEARANCE_GENERIC_THERMOMETER                  768 /**< Generic Thermometer. //��ͨ���¶ȼ�*/
#define BLE_APPEARANCE_THERMOMETER_EAR                      769 /**< Thermometer: Ear. //�����¶ȼ�*/
#define BLE_APPEARANCE_GENERIC_HEART_RATE_SENSOR            832 /**< Generic Heart rate Sensor. //ͨ�����ʴ�����*/
#define BLE_APPEARANCE_HEART_RATE_SENSOR_HEART_RATE_BELT    833 /**< Heart Rate Sensor: Heart Rate Belt. //���ʴ����������ʴ�*/
#define BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE               896 /**< Generic Blood Pressure. //ͨ��Ѫѹ��*/
#define BLE_APPEARANCE_BLOOD_PRESSURE_ARM                   897 /**< Blood Pressure: Arm. //Ѫѹ�ƣ��ֱ�*/
#define BLE_APPEARANCE_BLOOD_PRESSURE_WRIST                 898 /**< Blood Pressure: Wrist. //Ѫѹ�ƣ�����*/
#define BLE_APPEARANCE_GENERIC_HID                          960 /**< Human Interface Device (HID).//����ӿ��豸 */
#define BLE_APPEARANCE_HID_KEYBOARD                         961 /**< Keyboard (HID Subtype). //����*/
#define BLE_APPEARANCE_HID_MOUSE                            962 /**< Mouse (HID Subtype).//���*/
#define BLE_APPEARANCE_HID_JOYSTICK                         963 /**< Joystiq (HID Subtype). */
#define BLE_APPEARANCE_HID_GAMEPAD                          964 /**< Gamepad (HID Subtype). //��Ϸ�ֱ�*/
#define BLE_APPEARANCE_HID_DIGITIZERSUBTYPE                 965 /**< Digitizer Tablet (HID Subtype). //���ְ� */
#define BLE_APPEARANCE_HID_CARD_READER                      966 /**< Card Reader (HID Subtype). //������*/
#define BLE_APPEARANCE_HID_DIGITAL_PEN                      967 /**< Digital Pen (HID Subtype). //���ֱ�*/
#define BLE_APPEARANCE_HID_BARCODE                          968 /**< Barcode Scanner (HID Subtype). //������ɨ����*/
#define BLE_APPEARANCE_GENERIC_GLUCOSE_METER               1024 /**< Generic Glucose Meter. //��ͨѪ�Ǽ�*/
#define BLE_APPEARANCE_GENERIC_RUNNING_WALKING_SENSOR      1088 /**< Generic Running Walking Sensor. //��ͨ�ܲ������д�����*/
#define BLE_APPEARANCE_RUNNING_WALKING_SENSOR_IN_SHOE      1089 /**< Running Walking Sensor: In-Shoe. //Ь������ܲ����˶�������*/
#define BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_SHOE      1090 /**< Running Walking Sensor: On-Shoe. //Ь���ϵ��ܲ����˶�������*/
#define BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_HIP       1091 /**< Running Walking Sensor: On-Hip. //�β��ϵ��ܲ����˶���������*/
#define BLE_APPEARANCE_GENERIC_CYCLING                     1152 /**< Generic Cycling. //��ͨ���г�*/
#define BLE_APPEARANCE_CYCLING_CYCLING_COMPUTER            1153 /**< Cycling: Cycling Computer. ���г������*/
#define BLE_APPEARANCE_CYCLING_SPEED_SENSOR                1154 /**< Cycling: Speed Sensor. //���г����ٶȴ�����*/
#define BLE_APPEARANCE_CYCLING_CADENCE_SENSOR              1155 /**< Cycling: Cadence Sensor. //���г������ഫ����*/
#define BLE_APPEARANCE_CYCLING_POWER_SENSOR                1156 /**< Cycling: Power Sensor. //���г������ʴ�����*/
#define BLE_APPEARANCE_CYCLING_SPEED_CADENCE_SENSOR        1157 /**< Cycling: Speed and Cadence Sensor. //���г����ٶȺͽ��ഫ����*/
#define BLE_APPEARANCE_GENERIC_PULSE_OXIMETER              3136 /**< Generic Pulse Oximeter. //���г�����Ѫ����������*/
#define BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP            3137 /**< Fingertip (Pulse Oximeter subtype).ָ�⣬����Ѫ�������� */
#define BLE_APPEARANCE_PULSE_OXIMETER_WRIST_WORN           3138 /**< Wrist Worn(Pulse Oximeter subtype). //��������Ѫ��������*/
#define BLE_APPEARANCE_GENERIC_WEIGHT_SCALE                3200 /**< Generic Weight Scale. //һ�����س�*/
#define BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS_ACT          5184 /**< Generic Outdoor Sports Activity. //һ�㻧�������*/
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_DISP         5185 /**< Location Display Device (Outdoor Sports Activity subtype). //λ����ʾװ��(�����˶������)*/
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_AND_NAV_DISP 5186 /**< Location and Navigation Display Device (Outdoor Sports Activity subtype). //λ�ú͵�����ʾװ��(�����˶��������)*/
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_POD          5187 /**< Location Pod (Outdoor Sports Activity subtype). //��λ����(�����˶������)*/
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_AND_NAV_POD  5188 /**< Location and Navigation Pod (Outdoor Sports Activity subtype). //��λ�͵�������(�����˶������)*/

/** @} */

/** @brief Set .type and .uuid fields of ble_uuid_struct to specified uuid value. //��ble_uuid_struct ��uuid�ֶ�����Ϊָ��uuidֵ*/
#define BLE_UUID_BLE_ASSIGN(instance, value) do {\
            instance.type = BLE_UUID_TYPE_BLE; \
            instance.uuid = value;} while(0)

/** @brief Copy type and uuid members from src to dst ble_uuid_t pointer. Both pointers must be valid/non-null. //�������ͺ�uuid��Ա��src��dst ble_uuid_t ָ�롣����ָ�붼��������Ч��/�ǿյġ�*/
#define BLE_UUID_COPY_PTR(dst, src) do {\
            (dst)->type = (src)->type; \
            (dst)->uuid = (src)->uuid;} while(0)

/** @brief Copy type and uuid members from src to dst ble_uuid_t struct. //�������ͺ�uuid��Ա��src��dst ble_uuid_t �ṹ*/
#define BLE_UUID_COPY_INST(dst, src) do {\
            (dst).type = (src).type; \
            (dst).uuid = (src).uuid;} while(0)

/** @brief Compare for equality both type and uuid members of two (valid, non-null) ble_uuid_t pointers.//�Ƚ�����(��Ч�ģ��ǿյ�)ble_uuid_t ָ�����ͺ�uuid��Ա */
#define BLE_UUID_EQ(p_uuid1, p_uuid2) \
            (((p_uuid1)->type == (p_uuid2)->type) && ((p_uuid1)->uuid == (p_uuid2)->uuid))

/** @brief Compare for difference both type and uuid members of two (valid, non-null) ble_uuid_t pointers.//�Ƚ�����(��Ч�ģ��ǿյ�)ble_uuid_tָ�����ͺ�uuid��Ա�Ĳ��� */
#define BLE_UUID_NEQ(p_uuid1, p_uuid2) \
            (((p_uuid1)->type != (p_uuid2)->type) || ((p_uuid1)->uuid != (p_uuid2)->uuid))

/** @} */

/** @addtogroup BLE_TYPES_STRUCTURES Structures  //BLE_TYPES_STRUCTURES �ṹ
 * @{ */

/** @brief 128 bit UUID values. //128λ��uuidֵ*/
typedef struct
{ 
    unsigned char uuid128[16]; /**< Little-Endian UUID bytes. ��λ����UUID�ֽ�*/
} ble_uuid128_t;

/** @brief  Bluetooth Low Energy UUID type, encapsulates both 16-bit and 128-bit UUIDs. //�͹�������uuid���ͣ���װ��16λ��128λuuid*/
typedef struct
{
    uint16_t    uuid; /**< 16-bit UUID value or octets 12-13 of 128-bit UUID. //16λuuidֵ����128λuuid��12-13�ֽ�*/
    uint8_t     type; /**< UUID type, see @ref BLE_UUID_TYPES. If type is @ref BLE_UUID_TYPE_UNKNOWN, the value of uuid is undefined. */
	//uuid���ͣ��ο� BLE_UUID_TYPES.��������� BLE_UUID_TYPE_UNKNOWN,uuid��ֵ��δ֪��
} ble_uuid_t;

/** @} */

#endif /* BLE_TYPES_H__ */

/**
  @}
  @}
*/
