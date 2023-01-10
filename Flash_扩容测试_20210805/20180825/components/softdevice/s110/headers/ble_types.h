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
  @defgroup ble_types Common types and macro definitions //常见类型和宏定义
  @{

  @brief Common types and macro definitions for the BLE SoftDevice. //用于协议栈的常见类型和宏定义。
 */

#ifndef BLE_TYPES_H__
#define BLE_TYPES_H__

#include <stdint.h>

/** @addtogroup BLE_TYPES_DEFINES Defines
 * @{ */

/** @defgroup BLE_CONN_HANDLES BLE Connection Handles //BLE连接句柄
 * @{ */
#define BLE_CONN_HANDLE_INVALID 0xFFFF  /**< Invalid Connection Handle. //无效的连接句柄*/
#define BLE_CONN_HANDLE_ALL     0xFFFE  /**< Applies to all Connection Handles. //适用于所有连接句柄*/
/** @} */


/** @defgroup BLE_UUID_VALUES Assigned Values for BLE UUIDs //为BLE UUIDs 分配的值
 * @{ */
/* Generic UUIDs, applicable to all services */ //通用UUIDs，适用于所有服务。
#define BLE_UUID_UNKNOWN                              0x0000 /**< Reserved UUID. //保留UUID*/
#define BLE_UUID_SERVICE_PRIMARY                      0x2800 /**< Primary Service. //主要服务*/
#define BLE_UUID_SERVICE_SECONDARY                    0x2801 /**< Secondary Service. //二次服务*/
#define BLE_UUID_SERVICE_INCLUDE                      0x2802 /**< Include. //包含*/
#define BLE_UUID_CHARACTERISTIC                       0x2803 /**< Characteristic. //特征*/
#define BLE_UUID_DESCRIPTOR_CHAR_EXT_PROP             0x2900 /**< Characteristic Extended Properties Descriptor. //扩展属性特征描述符。*/
#define BLE_UUID_DESCRIPTOR_CHAR_USER_DESC            0x2901 /**< Characteristic User Description Descriptor. //用户描述特征描述符。*/
#define BLE_UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG        0x2902 /**< Client Characteristic Configuration Descriptor.//客户特点配置描述符。 */
#define BLE_UUID_DESCRIPTOR_SERVER_CHAR_CONFIG        0x2903 /**< Server Characteristic Configuration Descriptor. //服务器配置特征描述符。*/
#define BLE_UUID_DESCRIPTOR_CHAR_PRESENTATION_FORMAT  0x2904 /**< Characteristic Presentation Format Descriptor. //特征表示格式描述符。*/
#define BLE_UUID_DESCRIPTOR_CHAR_AGGREGATE_FORMAT     0x2905 /**< Characteristic Aggregate Format Descriptor. //特征聚合格式描述符。*/
/* GATT specific UUIDs  GATT 特性 UUIDs*/
#define BLE_UUID_GATT                                 0x1801 /**< Generic Attribute Profile. //通用属性配置文件*/
#define BLE_UUID_GATT_CHARACTERISTIC_SERVICE_CHANGED  0x2A05 /**< Service Changed Characteristic. //服务改变特点*/
/* GAP specific UUIDs //GAP 特性 UUIDs*/
#define BLE_UUID_GAP                                  0x1800 /**< Generic Access Profile. //通用访问配置文件*/
#define BLE_UUID_GAP_CHARACTERISTIC_DEVICE_NAME       0x2A00 /**< Device Name Characteristic.//设备名称的特点 */
#define BLE_UUID_GAP_CHARACTERISTIC_APPEARANCE        0x2A01 /**< Appearance Characteristic. //外观特征 */
#define BLE_UUID_GAP_CHARACTERISTIC_PPF               0x2A02 /**< Peripheral Privacy Flag Characteristic. //外围设备隐私位特征*/
#define BLE_UUID_GAP_CHARACTERISTIC_RECONN_ADDR       0x2A03 /**< Reconnection Address Characteristic.//重新连接地址的特点 */
#define BLE_UUID_GAP_CHARACTERISTIC_PPCP              0x2A04 /**< Peripheral Preferred Connection Parameters Characteristic.//外围优先连接参数特性 */
/** @} */


/** @defgroup BLE_UUID_TYPES Types of UUID 类型的 UUID
 * @{ */
#define BLE_UUID_TYPE_UNKNOWN       0x00 /**< Invalid UUID type.//无效的UUID类型  */
#define BLE_UUID_TYPE_BLE           0x01 /**< Bluetooth SIG UUID (16-bit). //蓝牙技术联盟UUID(16-bit)*/
#define BLE_UUID_TYPE_VENDOR_BEGIN  0x02 /**< Vendor UUID types start at this index (128-bit). //供应商UUID类型从这个索引(128位)开始*/
/** @} */


/** @defgroup BLE_APPEARANCES Bluetooth Appearance values //蓝牙外观值。
 *  @note Retrieved from http://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
 * @{ */
#define BLE_APPEARANCE_UNKNOWN                                0 /**< Unknown. //未知，保留*/
#define BLE_APPEARANCE_GENERIC_PHONE                         64 /**< Generic Phone. //一般的手机*/
#define BLE_APPEARANCE_GENERIC_COMPUTER                     128 /**< Generic Computer.//一般的计算机 */
#define BLE_APPEARANCE_GENERIC_WATCH                        192 /**< Generic Watch. //一般的手表*/
#define BLE_APPEARANCE_WATCH_SPORTS_WATCH                   193 /**< Watch: Sports Watch. //手表：运动手表*/
#define BLE_APPEARANCE_GENERIC_CLOCK                        256 /**< Generic Clock. //通用的时钟*/
#define BLE_APPEARANCE_GENERIC_DISPLAY                      320 /**< Generic Display. //通用的显示*/
#define BLE_APPEARANCE_GENERIC_REMOTE_CONTROL               384 /**< Generic Remote Control. //通用遥控器*/
#define BLE_APPEARANCE_GENERIC_EYE_GLASSES                  448 /**< Generic Eye-glasses. //通用眼镜*/
#define BLE_APPEARANCE_GENERIC_TAG                          512 /**< Generic Tag. //通用 标签*/
#define BLE_APPEARANCE_GENERIC_KEYRING                      576 /**< Generic Keyring. //通用键盘*/
#define BLE_APPEARANCE_GENERIC_MEDIA_PLAYER                 640 /**< Generic Media Player. //通用播放器*/
#define BLE_APPEARANCE_GENERIC_BARCODE_SCANNER              704 /**< Generic Barcode Scanner.//通用条形码扫描器 */
#define BLE_APPEARANCE_GENERIC_THERMOMETER                  768 /**< Generic Thermometer. //普通的温度计*/
#define BLE_APPEARANCE_THERMOMETER_EAR                      769 /**< Thermometer: Ear. //耳朵温度计*/
#define BLE_APPEARANCE_GENERIC_HEART_RATE_SENSOR            832 /**< Generic Heart rate Sensor. //通用心率传感器*/
#define BLE_APPEARANCE_HEART_RATE_SENSOR_HEART_RATE_BELT    833 /**< Heart Rate Sensor: Heart Rate Belt. //心率传感器：心率带*/
#define BLE_APPEARANCE_GENERIC_BLOOD_PRESSURE               896 /**< Generic Blood Pressure. //通用血压计*/
#define BLE_APPEARANCE_BLOOD_PRESSURE_ARM                   897 /**< Blood Pressure: Arm. //血压计：手臂*/
#define BLE_APPEARANCE_BLOOD_PRESSURE_WRIST                 898 /**< Blood Pressure: Wrist. //血压计：手腕*/
#define BLE_APPEARANCE_GENERIC_HID                          960 /**< Human Interface Device (HID).//人体接口设备 */
#define BLE_APPEARANCE_HID_KEYBOARD                         961 /**< Keyboard (HID Subtype). //键盘*/
#define BLE_APPEARANCE_HID_MOUSE                            962 /**< Mouse (HID Subtype).//鼠标*/
#define BLE_APPEARANCE_HID_JOYSTICK                         963 /**< Joystiq (HID Subtype). */
#define BLE_APPEARANCE_HID_GAMEPAD                          964 /**< Gamepad (HID Subtype). //游戏手柄*/
#define BLE_APPEARANCE_HID_DIGITIZERSUBTYPE                 965 /**< Digitizer Tablet (HID Subtype). //数字板 */
#define BLE_APPEARANCE_HID_CARD_READER                      966 /**< Card Reader (HID Subtype). //读卡器*/
#define BLE_APPEARANCE_HID_DIGITAL_PEN                      967 /**< Digital Pen (HID Subtype). //数字笔*/
#define BLE_APPEARANCE_HID_BARCODE                          968 /**< Barcode Scanner (HID Subtype). //条形码扫描器*/
#define BLE_APPEARANCE_GENERIC_GLUCOSE_METER               1024 /**< Generic Glucose Meter. //普通血糖计*/
#define BLE_APPEARANCE_GENERIC_RUNNING_WALKING_SENSOR      1088 /**< Generic Running Walking Sensor. //普通跑步，步行传感器*/
#define BLE_APPEARANCE_RUNNING_WALKING_SENSOR_IN_SHOE      1089 /**< Running Walking Sensor: In-Shoe. //鞋子里的跑步，运动传感器*/
#define BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_SHOE      1090 /**< Running Walking Sensor: On-Shoe. //鞋子上的跑步，运动传感器*/
#define BLE_APPEARANCE_RUNNING_WALKING_SENSOR_ON_HIP       1091 /**< Running Walking Sensor: On-Hip. //臀部上的跑步，运动传感器。*/
#define BLE_APPEARANCE_GENERIC_CYCLING                     1152 /**< Generic Cycling. //普通自行车*/
#define BLE_APPEARANCE_CYCLING_CYCLING_COMPUTER            1153 /**< Cycling: Cycling Computer. 自行车：码表*/
#define BLE_APPEARANCE_CYCLING_SPEED_SENSOR                1154 /**< Cycling: Speed Sensor. //自行车：速度传感器*/
#define BLE_APPEARANCE_CYCLING_CADENCE_SENSOR              1155 /**< Cycling: Cadence Sensor. //自行车：节奏传感器*/
#define BLE_APPEARANCE_CYCLING_POWER_SENSOR                1156 /**< Cycling: Power Sensor. //自行车：功率传感器*/
#define BLE_APPEARANCE_CYCLING_SPEED_CADENCE_SENSOR        1157 /**< Cycling: Speed and Cadence Sensor. //自行车：速度和节奏传感器*/
#define BLE_APPEARANCE_GENERIC_PULSE_OXIMETER              3136 /**< Generic Pulse Oximeter. //自行车脉搏血氧传感器。*/
#define BLE_APPEARANCE_PULSE_OXIMETER_FINGERTIP            3137 /**< Fingertip (Pulse Oximeter subtype).指尖，脉搏血氧传感器 */
#define BLE_APPEARANCE_PULSE_OXIMETER_WRIST_WORN           3138 /**< Wrist Worn(Pulse Oximeter subtype). //手腕，脉搏血氧传感器*/
#define BLE_APPEARANCE_GENERIC_WEIGHT_SCALE                3200 /**< Generic Weight Scale. //一般体重秤*/
#define BLE_APPEARANCE_GENERIC_OUTDOOR_SPORTS_ACT          5184 /**< Generic Outdoor Sports Activity. //一般户外体育活动*/
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_DISP         5185 /**< Location Display Device (Outdoor Sports Activity subtype). //位置显示装置(户外运动活动亚型)*/
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_AND_NAV_DISP 5186 /**< Location and Navigation Display Device (Outdoor Sports Activity subtype). //位置和导航显示装置(户外运动活动子类型)*/
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_POD          5187 /**< Location Pod (Outdoor Sports Activity subtype). //定位吊舱(户外运动活动亚型)*/
#define BLE_APPEARANCE_OUTDOOR_SPORTS_ACT_LOC_AND_NAV_POD  5188 /**< Location and Navigation Pod (Outdoor Sports Activity subtype). //定位和导航吊舱(户外运动活动亚型)*/

/** @} */

/** @brief Set .type and .uuid fields of ble_uuid_struct to specified uuid value. //把ble_uuid_struct 的uuid字段设置为指定uuid值*/
#define BLE_UUID_BLE_ASSIGN(instance, value) do {\
            instance.type = BLE_UUID_TYPE_BLE; \
            instance.uuid = value;} while(0)

/** @brief Copy type and uuid members from src to dst ble_uuid_t pointer. Both pointers must be valid/non-null. //复制类型和uuid成员从src到dst ble_uuid_t 指针。两个指针都必须是有效的/非空的。*/
#define BLE_UUID_COPY_PTR(dst, src) do {\
            (dst)->type = (src)->type; \
            (dst)->uuid = (src)->uuid;} while(0)

/** @brief Copy type and uuid members from src to dst ble_uuid_t struct. //复制类型和uuid成员从src到dst ble_uuid_t 结构*/
#define BLE_UUID_COPY_INST(dst, src) do {\
            (dst).type = (src).type; \
            (dst).uuid = (src).uuid;} while(0)

/** @brief Compare for equality both type and uuid members of two (valid, non-null) ble_uuid_t pointers.//比较两种(有效的，非空的)ble_uuid_t 指针类型和uuid成员 */
#define BLE_UUID_EQ(p_uuid1, p_uuid2) \
            (((p_uuid1)->type == (p_uuid2)->type) && ((p_uuid1)->uuid == (p_uuid2)->uuid))

/** @brief Compare for difference both type and uuid members of two (valid, non-null) ble_uuid_t pointers.//比较两种(有效的，非空的)ble_uuid_t指针类型和uuid成员的差异 */
#define BLE_UUID_NEQ(p_uuid1, p_uuid2) \
            (((p_uuid1)->type != (p_uuid2)->type) || ((p_uuid1)->uuid != (p_uuid2)->uuid))

/** @} */

/** @addtogroup BLE_TYPES_STRUCTURES Structures  //BLE_TYPES_STRUCTURES 结构
 * @{ */

/** @brief 128 bit UUID values. //128位的uuid值*/
typedef struct
{ 
    unsigned char uuid128[16]; /**< Little-Endian UUID bytes. 低位优先UUID字节*/
} ble_uuid128_t;

/** @brief  Bluetooth Low Energy UUID type, encapsulates both 16-bit and 128-bit UUIDs. //低功耗蓝牙uuid类型，封装了16位和128位uuid*/
typedef struct
{
    uint16_t    uuid; /**< 16-bit UUID value or octets 12-13 of 128-bit UUID. //16位uuid值，或128位uuid的12-13字节*/
    uint8_t     type; /**< UUID type, see @ref BLE_UUID_TYPES. If type is @ref BLE_UUID_TYPE_UNKNOWN, the value of uuid is undefined. */
	//uuid类型，参考 BLE_UUID_TYPES.如果类型是 BLE_UUID_TYPE_UNKNOWN,uuid的值是未知的
} ble_uuid_t;

/** @} */

#endif /* BLE_TYPES_H__ */

/**
  @}
  @}
*/
