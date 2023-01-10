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
  @addtogroup BLE_GAP Generic Access Profile (GAP) //通用接入规范
  @{
  @brief Definitions and prototypes for the GAP interface. // GAP接口的定义和模型
 */

#ifndef BLE_GAP_H__
#define BLE_GAP_H__

#include "ble_types.h"
#include "ble_ranges.h"
#include "nrf_svc.h"

/**@addtogroup BLE_GAP_ENUMERATIONS Enumerations //枚举
 * @{ */

/**@brief GAP API SVC numbers. //GAP API SVC 表格
 */
enum BLE_GAP_SVCS
{
  SD_BLE_GAP_ADDRESS_SET  = BLE_GAP_SVC_BASE,  /**< Set own Bluetooth Address. */ //设置自己的蓝牙地址
  SD_BLE_GAP_ADDRESS_GET,                      /**< Get own Bluetooth Address. */ //得到自己的蓝牙地址
  SD_BLE_GAP_ADV_DATA_SET,                     /**< Set Advertising Data. */ // 设置广播数据
  SD_BLE_GAP_ADV_START,                        /**< Start Advertising. */ //开始广播
  SD_BLE_GAP_ADV_STOP,                         /**< Stop Advertising. */ //停止广播
  SD_BLE_GAP_CONN_PARAM_UPDATE,                /**< Connection Parameter Update. */ //连接参数更新
  SD_BLE_GAP_DISCONNECT,                       /**< Disconnect. */ //断开
  SD_BLE_GAP_TX_POWER_SET,                     /**< Set TX Power. */ //设置 发送功率
  SD_BLE_GAP_APPEARANCE_SET,                   /**< Set Appearance. */ //设置外观值
  SD_BLE_GAP_APPEARANCE_GET,                   /**< Get Appearance. */ //获取外观值
  SD_BLE_GAP_PPCP_SET,                         /**< Set PPCP. */ //设置 PPCP
  SD_BLE_GAP_PPCP_GET,                         /**< Get PPCP. */ //获取 PPCP
  SD_BLE_GAP_DEVICE_NAME_SET,                  /**< Set Device Name. */ //设置设备名
  SD_BLE_GAP_DEVICE_NAME_GET,                  /**< Get Device Name. */ //获取设备名 
  SD_BLE_GAP_AUTHENTICATE,                     /**< Initiate Pairing/Bonding. */ //启动配对/键
  SD_BLE_GAP_SEC_PARAMS_REPLY,                 /**< Reply with Security Parameters. */ //回复与安全参数
  SD_BLE_GAP_AUTH_KEY_REPLY,                   /**< Reply with an authentication key. */ //用身份验证密钥回复
  SD_BLE_GAP_ENCRYPT,                          /**< Initiate encryption procedure. */ //启动加密程序
  SD_BLE_GAP_SEC_INFO_REPLY,                   /**< Reply with Security Information. */ //以安全信息回复
  SD_BLE_GAP_CONN_SEC_GET,                     /**< Obtain connection security level. */ // 获得连接的安全级别
  SD_BLE_GAP_RSSI_START,                       /**< Start reporting of changes in RSSI. */  //开始报告RSSI的变化
  SD_BLE_GAP_RSSI_STOP,                        /**< Stop reporting of changes in RSSI. */  //停止报告RSSI的变化
  SD_BLE_GAP_SCAN_START,                       /**< Start Scanning. */ //开始扫描
  SD_BLE_GAP_SCAN_STOP,                        /**< Stop Scanning. */ //停止扫描
  SD_BLE_GAP_CONNECT,                          /**< Connect. */ //连接
  SD_BLE_GAP_CONNECT_CANCEL,                   /**< Cancel ongoing connection procedure. */ //取消正在进行的连接过程
  SD_BLE_GAP_RSSI_GET,                         /**< Get the last RSSI sample. */ //得到最后一个 RSSI样本
};

/**@brief GAP Event IDs. //GAP 事件 IDs.
 * IDs that uniquely identify an event coming from the stack to the application. //识别从堆栈到应用程序的事件ID。
 */
enum BLE_GAP_EVTS
{
  BLE_GAP_EVT_CONNECTED  = BLE_GAP_EVT_BASE,    /**< Connection established. @ref ble_gap_evt_connected_t */ //连接建立 ble_gap_et_connected_t
  BLE_GAP_EVT_DISCONNECTED,                     /**< Disconnected from peer. @ref ble_gap_evt_disconnected_t */ //断开连接 ble_gap_evt_disconnected_t
  BLE_GAP_EVT_CONN_PARAM_UPDATE,                /**< Connection Parameters updated. ble_gap_evt_conn_param_update_t */ //连接参数更新。 ble_gap_evt_conn_param_update_t
  BLE_GAP_EVT_SEC_PARAMS_REQUEST,               /**< Request to provide security parameters. @ref ble_gap_evt_sec_params_request_t */ //请求提供安全参数。 ble_gap_evt_sec_params_request_t
  BLE_GAP_EVT_SEC_INFO_REQUEST,                 /**< Request to provide security information. @ref ble_gap_evt_sec_info_request_t */ //请求提供安全信息。ble_gap_evt_sec_info_request_t
  BLE_GAP_EVT_PASSKEY_DISPLAY,                  /**< Request to display a passkey to the user. @ref ble_gap_evt_passkey_display_t */ //请求向用户显示一个万能密码 ble_gap_evt_passkey_display_t
  BLE_GAP_EVT_AUTH_KEY_REQUEST,                 /**< Request to provide an authentication key. @ref ble_gap_evt_auth_key_request_t */ //请求提供身份验证密钥。ble_gap_evt_auth_key_request_t
  BLE_GAP_EVT_AUTH_STATUS,                      /**< Authentication procedure completed with status. @ref ble_gap_evt_auth_status_t */ // 身份验证过程完成。 ble_gap_evt_auth_status_t
  BLE_GAP_EVT_CONN_SEC_UPDATE,                  /**< Connection security updated. @ref ble_gap_evt_conn_sec_update_t */ //连接安全更新。 ble_gap_evt_conn_sec_update_t
  BLE_GAP_EVT_TIMEOUT,                          /**< Timeout expired. @ref ble_gap_evt_timeout_t */ //超时过期。 ble_gap_evt_timeout_t
  BLE_GAP_EVT_RSSI_CHANGED,                     /**< RSSI report. @ref ble_gap_evt_rssi_changed_t */ //RSSI报告 ble_gap_evt_rssi_changed_t 
  BLE_GAP_EVT_ADV_REPORT,                       /**< Advertising report. @ref ble_gap_evt_adv_report_t */  // 广播的报告 ble_gap_evt_adv_rport_t
  BLE_GAP_EVT_SEC_REQUEST,                      /**< Security Request. @ref ble_gap_evt_sec_request_t */ //安全要求 ble_gap_evt_sec_request_t
  BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST,        /**< Connection Parameter Update Request. @ref ble_gap_evt_conn_param_update_request_t */ //连接参数更新请求。 ble_gap_evt_con_praam_update_request_t
  BLE_GAP_EVT_SCAN_REQ_REPORT,                  /**< Scan request report. @ref ble_gap_evt_scan_req_report_t */ //扫描报告请求。 ble_gap_evt_scan_req_report_t
};

/**@brief GAP Option IDs.  GAP 选项ID
 * IDs that uniquely identify a GAP option. //识别空白选项的ID
 */
enum BLE_GAP_OPTS
{
  BLE_GAP_OPT_CH_MAP  = BLE_GAP_OPT_BASE,       /**< Channel Map. @ref ble_gap_opt_ch_map_t  */ //通道地图。 ble_gap_opt_ch_map_t
  BLE_GAP_OPT_LOCAL_CONN_LATENCY,               /**< Local connection latency. @ref ble_gap_opt_local_conn_latency_t */ //本地连接延迟 ble_gap_opt_local_conn_latency_t
  BLE_GAP_OPT_PASSKEY,                          /**< Set passkey. @ref ble_gap_opt_passkey_t */ //设置万能钥匙 ble_gap_opt_passkey_t
  BLE_GAP_OPT_PRIVACY,                          /**< Custom privacy. @ref ble_gap_opt_privacy_t */ //自定义隐私 ble_gap_opt_privacy_t
  BLE_GAP_OPT_SCAN_REQ_REPORT,                  /**< Scan request report. @ref ble_gap_opt_scan_req_report_t */ //扫描报告请求 ble_gap_opt_scan_req_report_t
  BLE_GAP_OPT_COMPAT_MODE                       /**< Compatibility mode. @ref ble_gap_opt_compat_mode_t */ //兼容模式。 ble_gap_opt_compat_mode_t
};
/** @} */

/**@addtogroup BLE_GAP_DEFINES Defines //BLE_GAP_DEFINES 定义
 * @{ */

/**@defgroup BLE_ERRORS_GAP SVC return values specific to GAP //SVC 返回特定于 GAP  
 * @{ */ 
#define BLE_ERROR_GAP_UUID_LIST_MISMATCH            (NRF_GAP_ERR_BASE + 0x000)  /**< UUID list does not contain an integral number of UUIDs. */ //UUID列表不包含UUIDs的整数数量
#define BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST   (NRF_GAP_ERR_BASE + 0x001)  /**< Use of Whitelist not permitted with discoverable advertising. */ //不允许使用可发现广播的白名单
#define BLE_ERROR_GAP_INVALID_BLE_ADDR              (NRF_GAP_ERR_BASE + 0x002)  /**< The upper two bits of the address do not correspond to the specified address type. */ //地址上的两个部分与指定的地址类型不对应
/**@} */


/**@defgroup BLE_GAP_ROLES GAP Roles //GAP 角色
 * @note Not explicitly used in peripheral API, but will be relevant for central API. //在外围API中没有显示使用，但将与中央API相关
 * @{ */
#define BLE_GAP_ROLE_INVALID     0x0            /**< Invalid Role. */ //无效角色
#define BLE_GAP_ROLE_PERIPH      0x1            /**< Peripheral Role. */ //外围角色
#define BLE_GAP_ROLE_CENTRAL     0x2            /**< Central Role. */ //主要角色
/**@} */


/**@defgroup BLE_GAP_TIMEOUT_SOURCES GAP Timeout sources GAP 超时来源
 * @{ */
#define BLE_GAP_TIMEOUT_SRC_ADVERTISING                0x00 /**< Advertising timeout. */ //广播超时
#define BLE_GAP_TIMEOUT_SRC_SECURITY_REQUEST           0x01 /**< Security request timeout. */ //安全需求超时
#define BLE_GAP_TIMEOUT_SRC_SCAN                       0x02 /**< Scanning timeout. */ //扫描超时
#define BLE_GAP_TIMEOUT_SRC_CONN                       0x03 /**< Connection timeout. */ //连接超时
/**@} */


/**@defgroup BLE_GAP_ADDR_TYPES GAP Address types GAP 地址类型
 * @{ */
#define BLE_GAP_ADDR_TYPE_PUBLIC                        0x00 /**< Public address. */ //公共地址
#define BLE_GAP_ADDR_TYPE_RANDOM_STATIC                 0x01 /**< Random Static address. */ //随机静态地址
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE     0x02 /**< Private Resolvable address. */ //私人地址解析
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE 0x03 /**< Private Non-Resolvable address. */ //私人 Non-Resolvable 地址
/**@} */

/**@defgroup BLE_GAP_ADDR_CYCLE_MODES GAP Address cycle modes GAP 地址 循环模式
 * @{ */
#define BLE_GAP_ADDR_CYCLE_MODE_NONE      0x00 /**< Set addresses directly, no automatic address cycling. */ //直接设置地址，没有自动地址循环
#define BLE_GAP_ADDR_CYCLE_MODE_AUTO      0x01 /**< Automatically generate and update private addresses. */ //自动生成和更新私有地址
/** @} */

/**@brief The default interval in seconds at which a private address is refreshed when address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO.  */ // 当地址循环模式下，一个私有地址被刷新的默认事件间隔是 BLE_GAP_ADDR_CYCLE_MODE_AUTO
#define BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S (60 * 15)

/** @brief BLE address length. */
#define BLE_GAP_ADDR_LEN            6


/**@defgroup BLE_GAP_AD_TYPE_DEFINITIONS GAP Advertising and Scan Response Data format //GAP广播和扫描响应数据格式
 * @note Found at https://www.bluetooth.org/Technical/AssignedNumbers/generic_access_profile.htm 
 * @{ */
#define BLE_GAP_AD_TYPE_FLAGS                               0x01 /**< Flags for discoverability. */ //标记的可发现性
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE   0x02 /**< Partial list of 16 bit service UUIDs. */ //16位服务UUIDs的部分列表
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE         0x03 /**< Complete list of 16 bit service UUIDs. */ // 完整的16位服务UUIDs列表
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE   0x04 /**< Partial list of 32 bit service UUIDs. */ //32位服务UUID的部分列表
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_COMPLETE         0x05 /**< Complete list of 32 bit service UUIDs. */ //完整的32位服务UUIDs列表
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE  0x06 /**< Partial list of 128 bit service UUIDs. */ //128位服务UUIDs的部分列表
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE        0x07 /**< Complete list of 128 bit service UUIDs. *///完整的128位服务UUIDs列表
#define BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME                    0x08 /**< Short local device name. */ //本地设备的短名
#define BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME                 0x09 /**< Complete local device name. */ //完整本地设备名
#define BLE_GAP_AD_TYPE_TX_POWER_LEVEL                      0x0A /**< Transmit power level. */ //传输功率
#define BLE_GAP_AD_TYPE_CLASS_OF_DEVICE                     0x0D /**< Class of device. */ //设备种类
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C               0x0E /**< Simple Pairing Hash C. */ //搭配简单的散列 C
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R         0x0F /**< Simple Pairing Randomizer R. */ //简单的配对随机函数发生器 R
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_TK_VALUE           0x10 /**< Security Manager TK Value. */ //安全管理器 TK值
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_OOB_FLAGS          0x11 /**< Security Manager Out Of Band Flags. */ //安全管理不同频道信号传输位
#define BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE     0x12 /**< Slave Connection Interval Range. */ //从机连接区间范围
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_16BIT       0x14 /**< List of 16-bit Service Solicitation UUIDs. */ //16位 服务请求UUID列表
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_128BIT      0x15 /**< List of 128-bit Service Solicitation UUIDs. */ //128位 服务请求 UUID列表
#define BLE_GAP_AD_TYPE_SERVICE_DATA                        0x16 /**< Service Data - 16-bit UUID. */ //16位 UUID 服务数据
#define BLE_GAP_AD_TYPE_PUBLIC_TARGET_ADDRESS               0x17 /**< Public Target Address. */ //公共目标地址
#define BLE_GAP_AD_TYPE_RANDOM_TARGET_ADDRESS               0x18 /**< Random Target Address. */ //随机目标地址
#define BLE_GAP_AD_TYPE_APPEARANCE                          0x19 /**< Appearance. */ //外观
#define BLE_GAP_AD_TYPE_ADVERTISING_INTERVAL                0x1A /**< Advertising Interval. */ //广播时间间隔
#define BLE_GAP_AD_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS         0x1B /**< LE Bluetooth Device Address. */ //LE蓝牙设备地址
#define BLE_GAP_AD_TYPE_LE_ROLE                             0x1C /**< LE Role. */ //LE角色
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C256            0x1D /**< Simple Pairing Hash C-256. */ //简单配对 散列 C-256
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R256      0x1E /**< Simple Pairing Randomizer R-256. */ //简单配对 随机数发生器 R-256
#define BLE_GAP_AD_TYPE_SERVICE_DATA_32BIT_UUID             0x20 /**< Service Data - 32-bit UUID. */ //服务数据 32 bit UUID
#define BLE_GAP_AD_TYPE_SERVICE_DATA_128BIT_UUID            0x21 /**< Service Data - 128-bit UUID. */ //服务数据 128 bit UUID
#define BLE_GAP_AD_TYPE_3D_INFORMATION_DATA                 0x3D /**< 3D Information Data. */ // 3D 信息数据
#define BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA          0xFF /**< Manufacturer Specific Data. */ // 制造商特定的数据
/**@} */


/**@defgroup BLE_GAP_ADV_FLAGS GAP Advertisement Flags //GAP 广播 标志
 * @{ */
#define BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE         (0x01)   /**< LE Limited Discoverable Mode. */ //LE 有限发现模式
#define BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE         (0x02)   /**< LE General Discoverable Mode. */ //LE 一般发现模式
#define BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED         (0x04)   /**< BR/EDR not supported. */ // BR/EDR 不支持
#define BLE_GAP_ADV_FLAG_LE_BR_EDR_CONTROLLER         (0x08)   /**< Simultaneous LE and BR/EDR, Controller. */ //同步 LE 和 BR/EDR，控制器
#define BLE_GAP_ADV_FLAG_LE_BR_EDR_HOST               (0x10)   /**< Simultaneous LE and BR/EDR, Host. */ // 同时 LE 和 BR/EDR，主机
#define BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE   (BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE | BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED)   /**< LE Limited Discoverable Mode, BR/EDR not supported. */ //有限的可发现模式，br/edr 不受支持
#define BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE   (BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE | BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED)   /**< LE General Discoverable Mode, BR/EDR not supported. *///一般可发现模式，br/edr 不受支持
/**@} */


/**@defgroup BLE_GAP_ADV_INTERVALS GAP Advertising interval max and min 广播间隔最大和最小时间
 * @{ */
#define BLE_GAP_ADV_INTERVAL_MIN        0x0020 /**< Minimum Advertising interval in 625 us units, i.e. 20 ms. */ //最小广播间隔在 20000us，也就是 20ms
#define BLE_GAP_ADV_NONCON_INTERVAL_MIN 0x00A0 /**< Minimum Advertising interval in 625 us units for non connectable mode, i.e. 100 ms. */ //在非连接模式的最小间隔在100000us 也就是 100ms
#define BLE_GAP_ADV_INTERVAL_MAX        0x4000 /**< Maximum Advertising interval in 625 us units, i.e. 10.24 s. */ //最大广播间隔在 10240000us ,也就是 10.24s
 /**@}  */


/**@defgroup BLE_GAP_SCAN_INTERVALS GAP Scan interval max and min //GAP 扫描间隔最大值和最小值
 * @{ */
#define BLE_GAP_SCAN_INTERVAL_MIN       0x0004 /**< Minimum Scan interval in 625 us units, i.e. 2.5 ms. */ // 最小扫描间隔在 2500us，也就是2.5ms。
#define BLE_GAP_SCAN_INTERVAL_MAX       0x4000 /**< Maximum Scan interval in 625 us units, i.e. 10.24 s. */ //最大扫描间隔在10240000s，也就是 10.24s。
 /** @}  */


/**@defgroup BLE_GAP_SCAN_WINDOW GAP Scan window max and min //最大和最小GAP扫描窗口
 * @{ */
#define BLE_GAP_SCAN_WINDOW_MIN         0x0004 /**< Minimum Scan window in 625 us units, i.e. 2.5 ms. */ //最小扫描窗口在2500us ,也就是 2.5ms
#define BLE_GAP_SCAN_WINDOW_MAX         0x4000 /**< Maximum Scan window in 625 us units, i.e. 10.24 s. */ // 最大扫描窗口在 10240000，也就是 10.24s
 /** @}  */


/**@defgroup BLE_GAP_SCAN_TIMEOUT GAP Scan timeout max and min //GAP 扫描超时最大值和最小值
 * @{ */
#define BLE_GAP_SCAN_TIMEOUT_MIN        0x0001 /**< Minimum Scan timeout in seconds. */ //最小的扫描超时时间
#define BLE_GAP_SCAN_TIMEOUT_MAX        0xFFFF /**< Maximum Scan timeout in seconds. */ //最大的扫描超时时间
 /** @}  */


/**@brief Maximum size of advertising data in octets. */ //八字节广告数据的最大尺寸
#define  BLE_GAP_ADV_MAX_SIZE           31


/**@defgroup BLE_GAP_ADV_TYPES GAP Advertising types
 * @{ */
#define BLE_GAP_ADV_TYPE_ADV_IND          0x00   /**< Connectable undirected. */ //无方向性连接
#define BLE_GAP_ADV_TYPE_ADV_DIRECT_IND   0x01   /**< Connectable directed. */ // 有方向性连接
#define BLE_GAP_ADV_TYPE_ADV_SCAN_IND     0x02   /**< Scannable undirected. */ //无方向性扫描
#define BLE_GAP_ADV_TYPE_ADV_NONCONN_IND  0x03   /**< Non connectable undirected. */ //有方向性扫描
/**@} */


/**@defgroup BLE_GAP_ADV_FILTER_POLICIES GAP Advertising filter policies // GAP 广播过滤策略
 * @{ */
#define BLE_GAP_ADV_FP_ANY                0x00   /**< Allow scan requests and connect requests from any device. */ //允许扫描请求并连接任何设备的请求。
#define BLE_GAP_ADV_FP_FILTER_SCANREQ     0x01   /**< Filter scan requests with whitelist. */ // 用白名单过滤扫描请求
#define BLE_GAP_ADV_FP_FILTER_CONNREQ     0x02   /**< Filter connect requests with whitelist. */ //过滤器连接请求与白名单
#define BLE_GAP_ADV_FP_FILTER_BOTH        0x03   /**< Filter both scan and connect requests with whitelist. */ //过滤器连接请求与白名单
/**@} */


/**@defgroup BLE_GAP_ADV_TIMEOUT_VALUES GAP Advertising timeout values //GAP 广播的差距超时值
 * @{ */
#define BLE_GAP_ADV_TIMEOUT_LIMITED_MAX      180 /**< Maximum advertising time in limited discoverable mode (TGAP(lim_adv_timeout) = 180s). */ //在有限可发现模式下的最大广告时间(TGAP(lim_adv_timeout) = 180s).
#define BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED  0 /**< Unlimited advertising in general discoverable mode. */ //无限的广播在一般的可发现模式
/**@} */


/**@defgroup BLE_GAP_DISC_MODES GAP Discovery modes GAP 发现模式
 * @{ */
#define BLE_GAP_DISC_MODE_NOT_DISCOVERABLE  0x00   /**< Not discoverable discovery Mode. */ //不可发现的发现模式
#define BLE_GAP_DISC_MODE_LIMITED           0x01   /**< Limited Discovery Mode. */ //有限的发现模式
#define BLE_GAP_DISC_MODE_GENERAL           0x02   /**< General Discovery Mode. */ // 一般发现模式
/**@} */

/**@defgroup BLE_GAP_IO_CAPS GAP IO Capabilities GAP 输入输出功能
 * @{ */
#define BLE_GAP_IO_CAPS_DISPLAY_ONLY      0x00   /**< Display Only. */ //仅显示
#define BLE_GAP_IO_CAPS_DISPLAY_YESNO     0x01   /**< Display and Yes/No entry. */ //是/否 显示条目
#define BLE_GAP_IO_CAPS_KEYBOARD_ONLY     0x02   /**< Keyboard Only. */ //仅键盘
#define BLE_GAP_IO_CAPS_NONE              0x03   /**< No I/O capabilities. */ //无 I/O 功能
#define BLE_GAP_IO_CAPS_KEYBOARD_DISPLAY  0x04   /**< Keyboard and Display. */ //键盘和显示
/**@} */


/**@defgroup BLE_GAP_AUTH_KEY_TYPES GAP Authentication Key Types //GAP 认证密钥类型
 * @{ */
#define BLE_GAP_AUTH_KEY_TYPE_NONE        0x00   /**< No key (may be used to reject). */ //无 密钥，(可能被用来拒绝)
#define BLE_GAP_AUTH_KEY_TYPE_PASSKEY     0x01   /**< 6-digit Passkey. */ //6位数 万能钥匙
#define BLE_GAP_AUTH_KEY_TYPE_OOB         0x02   /**< Out Of Band data. */ //不同频道信号传输数据
/**@} */

/**@defgroup BLE_GAP_SEC_STATUS GAP Security status //GAP 安全状态
 * @{ */ 
#define BLE_GAP_SEC_STATUS_SUCCESS                0x00  /**< Procedure completed with success. */ //过程成功完成
#define BLE_GAP_SEC_STATUS_TIMEOUT                0x01  /**< Procedure timed out. */ //过程超时
#define BLE_GAP_SEC_STATUS_PDU_INVALID            0x02  /**< Invalid PDU received. */ //无效的PDU
#define BLE_GAP_SEC_STATUS_PASSKEY_ENTRY_FAILED   0x81  /**< Passkey entry failed (user cancelled or other). */ //密码输入失败 (用户取消或其他)
#define BLE_GAP_SEC_STATUS_OOB_NOT_AVAILABLE      0x82  /**< Out of Band Key not available. */ //密钥无法使用
#define BLE_GAP_SEC_STATUS_AUTH_REQ               0x83  /**< Authentication requirements not met. */ //身份验证需求不满足
#define BLE_GAP_SEC_STATUS_CONFIRM_VALUE          0x84  /**< Confirm value failed. */ // 认证失败值
#define BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP       0x85  /**< Pairing not supported.  */ //不受支持的配对
#define BLE_GAP_SEC_STATUS_ENC_KEY_SIZE           0x86  /**< Encryption key size. */ //加密密钥大小
#define BLE_GAP_SEC_STATUS_SMP_CMD_UNSUPPORTED    0x87  /**< Unsupported SMP command. */ //不支持的SMP指令
#define BLE_GAP_SEC_STATUS_UNSPECIFIED            0x88  /**< Unspecified reason. */ //不明原因
#define BLE_GAP_SEC_STATUS_REPEATED_ATTEMPTS      0x89  /**< Too little time elapsed since last attempt. */ //上次尝试时间太少
#define BLE_GAP_SEC_STATUS_INVALID_PARAMS         0x8A  /**< Invalid parameters. */ // //无效的参数
/**@} */

/**@defgroup BLE_GAP_SEC_STATUS_SOURCES GAP Security status sources //GAP 安全状态来源
 * @{ */
#define BLE_GAP_SEC_STATUS_SOURCE_LOCAL           0x00  /**< Local failure. */ //本地失败
#define BLE_GAP_SEC_STATUS_SOURCE_REMOTE          0x01  /**< Remote failure. */ //远程失败
/**@} */

/**@defgroup BLE_GAP_CP_LIMITS GAP Connection Parameters Limits //GAP 连接参数限制
 * @{ */
#define BLE_GAP_CP_MIN_CONN_INTVL_NONE           0xFFFF  /**< No new minimum connction interval specified in connect parameters. */ //在连接参数中没有指定新的最小连接间隔
#define BLE_GAP_CP_MIN_CONN_INTVL_MIN            0x0006  /**< Lowest mimimum connection interval permitted, in units of 1.25 ms, i.e. 7.5 ms. */ //最小低连接间隔，7.5ms
#define BLE_GAP_CP_MIN_CONN_INTVL_MAX            0x0C80  /**< Highest minimum connection interval permitted, in units of 1.25 ms, i.e. 4 s. */ // 最大低连接间隔 4s
#define BLE_GAP_CP_MAX_CONN_INTVL_NONE           0xFFFF  /**< No new maximum connction interval specified in connect parameters. */ //在连接参数中没有指定新的最大连接间隔
#define BLE_GAP_CP_MAX_CONN_INTVL_MIN            0x0006  /**< Lowest maximum connection interval permitted, in units of 1.25 ms, i.e. 7.5 ms. */ // 允许的最小高连接间隔 7.5ms
#define BLE_GAP_CP_MAX_CONN_INTVL_MAX            0x0C80  /**< Highest maximum connection interval permitted, in units of 1.25 ms, i.e. 4 s. */ //允许的最大高连接间隔  4s
#define BLE_GAP_CP_SLAVE_LATENCY_MAX             0x01F3  /**< Highest slave latency permitted, in connection events. */ //在连接事件中允许的最高的从属性潜伏期
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_NONE         0xFFFF  /**< No new supervision timeout specified in connect parameters. */ //在连接参数中没有指定新的监督超时
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_MIN          0x000A  /**< Lowest supervision timeout permitted, in units of 10 ms, i.e. 100 ms. */ // 最低监督超时允许，单位为100ms
#define BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX          0x0C80  /**< Highest supervision timeout permitted, in units of 10 ms, i.e. 32 s. */ //最高的监督超时允许，单位为10ms，即32s
/**@} */


/**@brief GAP device name maximum length. */ //GAP设备名最大长度
#define BLE_GAP_DEVNAME_MAX_LEN           31

/**@brief Disable RSSI events for connections */ // 为连接禁用 RSSI事件
#define BLE_GAP_RSSI_THRESHOLD_INVALID    0xFF

/**@defgroup BLE_GAP_CONN_SEC_MODE_SET_MACROS GAP attribute security requirement setters //GAP 属性安全性要求设置
 *
 * See @ref ble_gap_conn_sec_mode_t.
 * @{ */
/**@brief Set sec_mode pointed to by ptr to have no access rights.*/ //由 ptr指向 sec_mode 没有访问权限
#define BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(ptr)         do {(ptr)->sm = 0; (ptr)->lv = 0;} while(0)
/**@brief Set sec_mode pointed to by ptr to require no protection, open link.*/ //按ptr 指定的 sec_mode,不需要保护，打开链接。
#define BLE_GAP_CONN_SEC_MODE_SET_OPEN(ptr)              do {(ptr)->sm = 1; (ptr)->lv = 1;} while(0)
/**@brief Set sec_mode pointed to by ptr to require encryption, but no MITM protection.*/ //按ptr指定的 sec_mode 要求加密，但没有MITM保户
#define BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(ptr)       do {(ptr)->sm = 1; (ptr)->lv = 2;} while(0)
/**@brief Set sec_mode pointed to by ptr to require encryption and MITM protection.*/ // 由ptr指定的sec_mode要求加密和MITM保护
#define BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(ptr)     do {(ptr)->sm = 1; (ptr)->lv = 3;} while(0)
/**@brief Set sec_mode pointed to by ptr to require signing or encryption, no MITM protection needed.*/ //按ptr指定的sec_mode 要求签名或加密，不需要MITM保护
#define BLE_GAP_CONN_SEC_MODE_SET_SIGNED_NO_MITM(ptr)    do {(ptr)->sm = 2; (ptr)->lv = 1;} while(0)
/**@brief Set sec_mode pointed to by ptr to require signing or encryption with MITM protection.*/ //按ptr指定的sec_mode 要求使用MITM保护进行签名或加密。
#define BLE_GAP_CONN_SEC_MODE_SET_SIGNED_WITH_MITM(ptr)  do {(ptr)->sm = 2; (ptr)->lv = 2;} while(0)
/**@} */


/**@brief GAP Security Random Number Length. */ //GAP 安全随机数长度
#define BLE_GAP_SEC_RAND_LEN 8

/**@brief GAP Security Key Length. */ //GAP 安全密钥长度
#define BLE_GAP_SEC_KEY_LEN 16

/**@brief GAP Passkey Length. */ //GAP 万能钥匙长度
#define BLE_GAP_PASSKEY_LEN 6

/**@brief Maximum amount of addresses in a whitelist. */ //在白名单中最多的地址
#define BLE_GAP_WHITELIST_ADDR_MAX_COUNT (8)
 
/**@brief Maximum amount of IRKs in a whitelist. //在白名单中最大数量的 IRKs
 * @note  The number of IRKs is limited to 8, even if the hardware supports more. //IRKs的数量限制为8，即时硬件支持更多。
 */
#define BLE_GAP_WHITELIST_IRK_MAX_COUNT (8)

/**@defgroup GAP_SEC_MODES GAP Security Modes GAP 安全模式
 * @{ */
#define BLE_GAP_SEC_MODE 0x00 /**< No key (may be used to reject). */ //没有密钥，(可能被用来拒绝)
/**@} */

/**@} */

/**@addtogroup BLE_GAP_STRUCTURES Structures //结构
 * @{ */

/**@brief Bluetooth Low Energy address. */ //低功耗蓝牙地址
typedef struct
{
  uint8_t addr_type;                    /**< See @ref BLE_GAP_ADDR_TYPES. */ //参见 BLE_GAP_ADDR_TYPES.
  uint8_t addr[BLE_GAP_ADDR_LEN];       /**< 48-bit address, LSB format. */ //48位地址，LSB格式
} ble_gap_addr_t;


/**@brief GAP connection parameters. //GAP 连接参数
 *
 * @note  When ble_conn_params_t is received in an event, both min_conn_interval and
 *        max_conn_interval will be equal to the connection interval set by the central. 
//当 ble_conn_params_t 在一个事件中被接受时，min_conn_interval和max_conn_interval等于主设备设置的连接间隔。
 *
 * @note If both conn_sup_timeout and max_conn_interval are specified, then the following constraint applies:
 *       conn_sup_timeout * 4 > (1 + slave_latency) * max_conn_interval
 *       that corresponds to the following Bluetooth Spec requirement:
 *       The Supervision_Timeout in milliseconds shall be larger than
 *       (1 + Conn_Latency) * Conn_Interval_Max * 2, where Conn_Interval_Max is given in milliseconds.

如果指定 conn_sup_timeout 和 max_conn_interval，那么下面的约束条件是： conn_sup_timeout * 4 > (1+ slave_latency) * max_conn_interval
这与以下蓝牙规格要求相符： 
以毫秒为单位的规格超时应该大于 (1+ Conn_Latency) * Conn_Interval_Max * 2,其中 Conn_Intervaal_Max 以毫秒为单位。

 */
typedef struct
{
  uint16_t min_conn_interval;         /**< Minimum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS.*/ //最小连接间隔在 1.25ms，参见 BLE_GAP_CP_LIMITS
  uint16_t max_conn_interval;         /**< Maximum Connection Interval in 1.25 ms units, see @ref BLE_GAP_CP_LIMITS.*/ //最大连接间隔在 1.25ms，参见 BLE_GAP_CP_LINITS.
  uint16_t slave_latency;             /**< Slave Latency in number of connection events, see @ref BLE_GAP_CP_LIMITS.*/ //在连接事件的数量上的从属延迟，参见 BLE_GAP_CP_LIMITS.
  uint16_t conn_sup_timeout;          /**< Connection Supervision Timeout in 10 ms units, see @ref BLE_GAP_CP_LIMITS.*/ //连接监视超时，10ms单元，参见BLE_GAP_CP_LIMITS.
} ble_gap_conn_params_t;


/**@brief GAP connection security modes. //GAP 连接安全模式
 *
 * Security Mode 0 Level 0: No access permissions at all (this level is not defined by the Bluetooth Core specification).\n
安全模式0，等级0： 根本没有访问权限(这个级别不是由蓝牙核心规范定义的)
 * Security Mode 1 Level 1: No security is needed (aka open link).\n
安全模式1，等级1：不需要安全性(也就是开放连接)
 * Security Mode 1 Level 2: Encrypted link required, MITM protection not necessary.\n
安全模式1，等级2：需要加密的保护，不需要MITM保护。
 * Security Mode 1 Level 3: MITM protected encrypted link required.\n、
安全模式1，等级3：MITM保护加密连接
 * Security Mode 2 Level 1: Signing or encryption required, MITM protection not necessary.\n
安全模式2，等级1：需要签名或加密，不需要MITM保护
 * Security Mode 2 Level 2: MITM protected signing required, unless link is MITM protected encrypted.\n
安全模式2，等级2：MITM保护签名，除非连接是受保护的加密。
 */
typedef struct
{
  uint8_t sm : 4;                     /**< Security Mode (1 or 2), 0 for no permissions at all. */
	//安全模式(1或2)，0时完全没有权限。
  uint8_t lv : 4;                     /**< Level (1, 2 or 3), 0 for no permissions at all. */
	//等级(1，2，3)，0时完全没有权限。

} ble_gap_conn_sec_mode_t;


/**@brief GAP connection security status.*/ //GAP 连接安全状态
typedef struct
{
  ble_gap_conn_sec_mode_t sec_mode;           /**< Currently active security mode for this connection.*/
	//当前这种连接的活动安全模式
  uint8_t                 encr_key_size;      /**< Length of currently active encryption key, 7 to 16 octets (only applicable for bonding procedures). */
	//当前有源加密密钥的长度，7到16个八位字节(仅适用于结合程序)

} ble_gap_conn_sec_t;


/**@brief Identity Resolving Key. */ //身份分析钥匙。
typedef struct
{
  uint8_t irk[BLE_GAP_SEC_KEY_LEN];   /**< Array containing IRK. */ //数据包含 IRK。
} ble_gap_irk_t;


/**@brief Whitelist structure. */ //白名单的结构
typedef struct
{
  ble_gap_addr_t    **pp_addrs;        /**< Pointer to an array of device address pointers, pointing to addresses to be used in whitelist. NULL if none are given. */
  //指向一个设备地址指针数组的指针，指向要在白名单中使用的地址。如果没有给出，则为空。
	uint8_t             addr_count;      /**< Count of device addresses in array, up to @ref BLE_GAP_WHITELIST_ADDR_MAX_COUNT. */
  //数组中设备地址的计数，一直到 BLE_GAP_WHITELIST_ADDR_MAX_COUNT.
	ble_gap_irk_t     **pp_irks;         /**< Pointer to an array of Identity Resolving Key (IRK) pointers, each pointing to an IRK in the whitelist. NULL if none are given. */
// 指向一组省份解析KEY(IRK)指针的指针，没个指针会指向白名单中的IRK，如果没有给出，则为空。
	uint8_t             irk_count;       /**< Count of IRKs in array, up to @ref BLE_GAP_WHITELIST_IRK_MAX_COUNT. */
	//数组中的IRKs计数，直到 BLE_GAP_WHITELIST_IRK_MAX_COUNT.
} ble_gap_whitelist_t;

/**@brief Channel mask for RF channels used in advertising and scanning. */ 
//用于广播和扫描的射频通道的通道掩盖、
typedef struct
{
  uint8_t ch_37_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 37 */ 
	//将这个位设置为1将关闭第37频道的广播
  uint8_t ch_38_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 38 */
	//将这个位设置为1将关闭第38频道的广播
  uint8_t ch_39_off : 1;  /**< Setting this bit to 1 will turn off advertising on channel 39 */
	//将这个位设置为1将关闭第39频道的广播
} ble_gap_adv_ch_mask_t;

/**@brief GAP advertising parameters.*/
typedef struct
{
  uint8_t               type;                 /**< See @ref BLE_GAP_ADV_TYPES. */ //参见 BLE_GAP_ADV_TYPES.
  ble_gap_addr_t       *p_peer_addr;          /**< For @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND mode only, known peer address. */
	//仅用于 BLE_GAP_ADV_TYPE_ADV_DIRECT_IND模式，知道(对等)对方地址。
  uint8_t               fp;                   /**< Filter Policy, see @ref BLE_GAP_ADV_FILTER_POLICIES. */
	//过滤策略，参见 BLE_GAP_ADV_FILTER_POLICIES.
  ble_gap_whitelist_t  *p_whitelist;          /**< Pointer to whitelist, NULL if none is given. */
	//指向白名单的地址，如果没有，则为空。
  uint16_t              interval;             /**< Advertising interval between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s), see @ref BLE_GAP_ADV_INTERVALS.
                                                   - If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, this parameter must be set to 0 for high duty cycle directed advertising.
                                                   - If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, set @ref BLE_GAP_ADV_INTERVAL_MIN <= interval <= @ref BLE_GAP_ADV_INTERVAL_MAX for low duty cycle advertising.*/
//0x0020和0x4000之间的广播间隔为0.625ms一个单位，(20ms到10.24s)，参见 BLE_GAP_ADV_INTERVALS.
//如果类型等于 BLE_GAP_ADV_TYPE_ADV_DIRECT_IND,这个参数必须被设置为0，用于高负荷循环定向广告。
	//如果类型等于 BLE_GAP_ADV_TYPE_ADV_DIRECT_IND,设置 BLE_GAP_ADV_INTERVAL_MIN <= interval <= BLE_GAP_ADV_INTERVAL_MAX ,用于低负荷循环广播。
	

	uint16_t              timeout;              /**< Advertising timeout between 0x0001 and 0x3FFF in seconds, 0x0000 disables timeout. See also @ref BLE_GAP_ADV_TIMEOUT_VALUES. If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, this parameter must be set to 0 for High duty cycle directed advertising. */
  //在0x0001和0x3FFF之间的广告超时时间为秒，0x0000禁用超时。参见 BLE_GAP_ADV_TIMEOUT_VALUES.如果类型等于BLE_GAP_ADV_TYPE_ADV_DIRECT_IND这个参数必须被设置为0，用于高负荷循环定向广播。
	
	ble_gap_adv_ch_mask_t channel_mask;         /**< Advertising channel mask. @see ble_gap_channel_mask_t for documentation. */
	//广播通道的掩盖。参见 ble_gap_channel_mask_t 文档。
} ble_gap_adv_params_t;


/**@brief GAP scanning parameters. */ //GAP 扫描参数
typedef struct
{
  uint8_t                 active    : 1;        /**< If 1, perform active scanning (scan requests). */ //如果为1，执行主动扫描(扫描请求)
  uint8_t                 selective : 1;        /**< If 1, ignore unknown devices (non whitelisted). */  //如果为1，忽略未知的设备(非白名单)
  ble_gap_whitelist_t *   p_whitelist;          /**< Pointer to whitelist, NULL if none is given. */ //指向白名单的指针，如果没有，则为空。
  uint16_t                interval;             /**< Scan interval between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */ //扫描间隔 0X0004和0x4000之间，0.625ms一个单元(25ms到10.24s)。
  uint16_t                window;               /**< Scan window between 0x0004 and 0x4000 in 0.625ms units (2.5ms to 10.24s). */ // 扫描窗口在0x0004和0x4000之间，0.625ms一个单元(2.5ms到10.24s).
  uint16_t                timeout;              /**< Scan timeout between 0x0001 and 0xFFFF in seconds, 0x0000 disables timeout. */ //扫描超时 在 0x0001和0xFFFF之间扫描超时，0x0000禁用超时。
} ble_gap_scan_params_t;


/** @brief Keys that can be exchanged during a bonding procedure. */ //KEY 可以在绑定过过程交换。
typedef struct
{
  uint8_t enc     : 1;                        /**< Long Term Key and Master Identification. */ //长期密钥和主机识别。
  uint8_t id      : 1;                        /**< Identity Resolving Key and Identity Address Information. */ //身份解析密钥和身份地址信息。
  uint8_t sign    : 1;                        /**< Connection Signature Resolving Key. */ //连接签名解析KEY。
} ble_gap_sec_kdist_t;


/**@brief GAP security parameters. */ //GAP 安全参数。
typedef struct
{
  uint8_t               bond    : 1;               /**< Perform bonding. */ //执行键。
  uint8_t               mitm    : 1;               /**< Man In The Middle protection required. */ //用户在中间需要保护。
  uint8_t               io_caps : 3;               /**< IO capabilities, see @ref BLE_GAP_IO_CAPS. */ //IO 功能，参见 BLE_GAP_IO_CAPS.
  uint8_t               oob     : 1;               /**< Out Of Band data available. */ //可用的带数据。
  uint8_t               min_key_size;              /**< Minimum encryption key size in octets between 7 and 16. If 0 then not applicable in this instance. */ //在7到16之间的最小八字节加密密钥大小，如果是0，在这个实例中不适用。
  uint8_t               max_key_size;              /**< Maximum encryption key size in octets between min_key_size and 16. */ //在最小值和16之间的最大八字节密钥大小。
  ble_gap_sec_kdist_t   kdist_periph;              /**< Key distribution bitmap: keys that the peripheral device will distribute. */  //密钥分发位图：外围设备将分发的密钥。
  ble_gap_sec_kdist_t   kdist_central;             /**< Key distribution bitmap: keys that the central device will distribute. */ //密钥分发位图：中央设备将会分发的密钥。
} ble_gap_sec_params_t;


/**@brief GAP Encryption Information. */ //GAP 加密信息。
typedef struct
{
  uint8_t   ltk[BLE_GAP_SEC_KEY_LEN];   /**< Long Term Key. */ //长期密钥。
  uint8_t   auth : 1;                   /**< Authenticated Key. */  //已验证 密钥。
  uint8_t   ltk_len : 7;                /**< LTK length in octets. */ // //八字节LTK长度。
} ble_gap_enc_info_t;


/**@brief GAP Master Identification. */  //GAP 主设备认证。
typedef struct
{
  uint16_t  ediv;                       /**< Encrypted Diversifier. */ //加密的多元化
  uint8_t   rand[BLE_GAP_SEC_RAND_LEN]; /**< Random Number. */ //随机数。
} ble_gap_master_id_t;

 
/**@brief GAP Signing Information. */ //GAP 签名信息。
typedef struct
{
  uint8_t   csrk[BLE_GAP_SEC_KEY_LEN];        /**< Connection Signature Resolving Key. */ //分析连接签名密钥
} ble_gap_sign_info_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONNECTED. */ // BLE_GAP_EVT_CONNECTED 事件结构。
typedef struct
{
  ble_gap_addr_t        peer_addr;              /**< Bluetooth address of the peer device. */ //对等(方)设备的蓝牙地址
  ble_gap_addr_t        own_addr;               /**< Bluetooth address of the local device used during connection setup. */ //本地设备在连接设置时的蓝牙地址。
  uint8_t               irk_match :1;           /**< If 1, peer device's address resolved using an IRK. */ //如果为1，对等(方)设备的地址使用IRK解析。
  uint8_t               irk_match_idx  :7;      /**< Index in IRK list where the address was matched. */ //在IRK列表中匹配地址的索引。 
  ble_gap_conn_params_t conn_params;            /**< GAP Connection Parameters. */ // GAP 连接参数。
} ble_gap_evt_connected_t;


/**@brief Event structure for @ref BLE_GAP_EVT_DISCONNECTED. */ //BLE_GAP_EVT_DISCONNECTED 的事件结构。
typedef struct
{
  uint8_t reason;                               /**< HCI error code, see @ref BLE_HCI_STATUS_CODES. */ //HCL错误代码，参见 BLE_HCL_STATUS_CODES.
} ble_gap_evt_disconnected_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONN_PARAM_UPDATE. */ //BLE_GAP_EVT_CONN_PARAM_UPDATE 的事件结构。
typedef struct
{
  ble_gap_conn_params_t conn_params;            /**<  GAP Connection Parameters. */ //GAP连接参数。
} ble_gap_evt_conn_param_update_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SEC_PARAMS_REQUEST. */ //BLE_GAP_EVT_SEC_PARAMS_REQUEST的事件结构。
typedef struct
{
  ble_gap_sec_params_t peer_params;             /**< Initiator Security Parameters. */ //启动程序安全参数。
} ble_gap_evt_sec_params_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SEC_INFO_REQUEST. */ //BLE_GAP_EVT_SEC_INFO_REQUEST 事件结构。
typedef struct
{
  ble_gap_addr_t      peer_addr;                     /**< Bluetooth address of the peer device. */ //对等(方)设备的蓝牙地址。
  ble_gap_master_id_t master_id;                     /**< Master Identification for LTK lookup. */ //用于LTK查找的主设备标识。
  uint8_t             enc_info  : 1;                 /**< If 1, Encryption Information required. */ //如果为1，则需要加密的信息。
  uint8_t             id_info   : 1;                 /**< If 1, Identity Information required. */ //如果为1，需要身份信息。
  uint8_t             sign_info : 1;                 /**< If 1, Signing Information required. */ //如果为1，需要签名信息。
} ble_gap_evt_sec_info_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_PASSKEY_DISPLAY. */ // BLE_GAP_EVT_PASSKEY_DISPLAY 事件结构。
typedef struct
{
  uint8_t passkey[BLE_GAP_PASSKEY_LEN];         /**< 6-digit passkey in ASCII ('0'-'9' digits only). */ //ASCII码组成的6位密码( 0-9 数字 )
} ble_gap_evt_passkey_display_t;


/**@brief Event structure for @ref BLE_GAP_EVT_AUTH_KEY_REQUEST. */ //BLE_GAP_EVT_AUTH_KEY_REQUEST 的事件结构。
typedef struct
{
  uint8_t key_type;                             /**< See @ref BLE_GAP_AUTH_KEY_TYPES. */ //参见 BLE_GAP_AUTH_KEY_TYPES.
} ble_gap_evt_auth_key_request_t;


/**@brief Security levels supported. //安全级别的支持
 * @note See Bluetooth Specification Version 4.1 Volume 3, Part C, Chapter 10. //参见蓝牙规范 V4.1 卷 3，C部分，第10章。
*/
typedef struct
{
  uint8_t lv1 : 1;                              /**< If 1: Level 1 is supported. */ //如果为1，级别1被支持。
  uint8_t lv2 : 1;                              /**< If 1: Level 2 is supported. */ //如果为1，级别2被支持。
  uint8_t lv3 : 1;                              /**< If 1: Level 3 is supported. */ //如果为1，级别3被支持。
} ble_gap_sec_levels_t;


/**@brief Encryption Key. */ //密钥
typedef struct
{
  ble_gap_enc_info_t    enc_info;             /**< Encryption Information. */ //加密信息。
  ble_gap_master_id_t   master_id;            /**< Master Identification. */ //主设备识别。
} ble_gap_enc_key_t;


/**@brief Identity Key. */ //身份信息。
typedef struct
{
  ble_gap_irk_t         id_info;              /**< Identity Information. */ //身份信息。
  ble_gap_addr_t        id_addr_info;         /**< Identity Address Information. */ //身份地址信息。
} ble_gap_id_key_t;


/**@brief Security Keys. */ //密钥
typedef struct
{
  ble_gap_enc_key_t     *p_enc_key;           /**< Encryption Key, or NULL. */ //加密钥匙，或NULL
  ble_gap_id_key_t      *p_id_key;            /**< Identity Key, or NULL. */ //身份钥匙，或NULL
  ble_gap_sign_info_t   *p_sign_key;          /**< Signing Key, or NULL. */ // 签名钥匙,或NULL
} ble_gap_sec_keys_t;


/**@brief Security key set (both Peripheral and Central keys). //安全密钥集(外围和中央设备的KEY)
 *         Note that when distributing Bluetooth addresses pertaining to the local device those
 *         will have to be filled in by the user. */ //注意，当分发与本地设备相关的蓝牙地址时必须由用户填写。
typedef struct
{
  ble_gap_sec_keys_t keys_periph;     /**< Keys distributed by the device in the Peripheral role. */ //由外围设备分配的KEY
  ble_gap_sec_keys_t keys_central;    /**< Keys distributed by the device in the Central role. */ // 由中心设备分配的KEY
} ble_gap_sec_keyset_t;


/**@brief Event structure for @ref BLE_GAP_EVT_AUTH_STATUS. */ //BLE_GAP_EVT_AUTH_STATUS的事件结构
typedef struct
{
  uint8_t               auth_status;            /**< Authentication status, see @ref BLE_GAP_SEC_STATUS. */ //身份验证状态，参考BLE_GAP_SEC_STATUS
  uint8_t               error_src : 2;          /**< On error, source that caused the failure, see @ref BLE_GAP_SEC_STATUS_SOURCES. */ //在错误的情况下，导致失败的源，参考BLE_GAP_SEC_STATUS_SOURCES
  uint8_t               bonded : 1;             /**< Procedure resulted in a bond. *///程序结果在了一个KEY中
  ble_gap_sec_levels_t  sm1_levels;             /**< Levels supported in Security Mode 1. */ // //在安全模式1中支持的级别。
  ble_gap_sec_levels_t  sm2_levels;             /**< Levels supported in Security Mode 2. */ //在安全模式2中支持的级别
  ble_gap_sec_kdist_t   kdist_periph;           /**< Bitmap stating which keys were exchanged (distributed) by the peripheral. */ //位图表示由外围设备交换(分布式)键
  ble_gap_sec_kdist_t   kdist_central;          /**< Bitmap stating which keys were exchanged (distributed) by the central. */ //位图表示由中央交换(分布式)键
} ble_gap_evt_auth_status_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONN_SEC_UPDATE. */ //BLE_GAP_EVT_CONN_SEC_UPDATE 的事件结构
typedef struct
{
  ble_gap_conn_sec_t conn_sec;                  /**< Connection security level. */ //连接安全级别
} ble_gap_evt_conn_sec_update_t;


/**@brief Event structure for @ref BLE_GAP_EVT_TIMEOUT. */ //BLE_GAP_EVT_TIMEOUT 的事件结构
typedef struct
{
  uint8_t src;                                  /**< Source of timeout event, see @ref BLE_GAP_TIMEOUT_SOURCES. */ //超时事件的来源，参见 BLE_GAP_TIMEOUT_SOURCES
} ble_gap_evt_timeout_t;


/**@brief Event structure for @ref BLE_GAP_EVT_RSSI_CHANGED. */ //BLE_GAP_EVT_RESI_CHANGED的事件结构。
typedef struct
{
  int8_t  rssi;                               /**< Received Signal Strength Indication in dBm. */ //接收信号强度dbm的指示
} ble_gap_evt_rssi_changed_t;


/**@brief Event structure for @ref BLE_GAP_EVT_ADV_REPORT. */ //BLE_GAP_EVT_ADV_REPORT的事件结构
typedef struct
{
  ble_gap_addr_t peer_addr;                     /**< Bluetooth address of the peer device. */ //对等设备的蓝牙地址
  int8_t         rssi;                          /**< Received Signal Strength Indication in dBm. */ //接收信号的强度dbm指示
  uint8_t        scan_rsp : 1;                  /**< If 1, the report corresponds to a scan response and the type field may be ignored. */ //如果是1，广播对应于扫描响应，而类型字段可能被忽略
  uint8_t        type     : 2;                  /**< See @ref BLE_GAP_ADV_TYPES. Only valid if the scan_rsp field is 0. *///参见 BLE_GAP_ADV_TYPES.只有当scan_rsp 字段为0时才有效。
  uint8_t        dlen     : 5;                  /**< Advertising or scan response data length. */ //广播或扫描响应数据长度。
  uint8_t        data[BLE_GAP_ADV_MAX_SIZE];    /**< Advertising or scan response data. */ //广播或扫描响应数据。
} ble_gap_evt_adv_report_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SEC_REQUEST. */ //BLE_GAP_EVT_SEC_REQUEST 的事件结构。
typedef struct
{
  uint8_t    bond    : 1;                       /**< Perform bonding. *///执行键
  uint8_t    mitm    : 1;                       /**< Man In The Middle protection required. */ //在中间的保护需求。
} ble_gap_evt_sec_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST. */ //BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST 的事件结构。
typedef struct 
{
  ble_gap_conn_params_t conn_params;            /**<  GAP Connection Parameters. */ // GAP 连接参数。
} ble_gap_evt_conn_param_update_request_t;


/**@brief Event structure for @ref BLE_GAP_EVT_SCAN_REQ_REPORT. */ //BLE_GAP_EVT_SCAN_REQ_REPORT 的事件结构。
typedef struct
{
  int8_t                  rssi;              /**< Received Signal Strength Indication in dBm. */ //接收信号强度dbm指示。
  ble_gap_addr_t          peer_addr;         /**< Bluetooth address of the peer device. */ //对等蓝牙设备的地址。
} ble_gap_evt_scan_req_report_t;



/**@brief GAP event structure. */ //GAP 事件结构
typedef struct
{
  uint16_t conn_handle;                                     /**< Connection Handle on which event occured. */ //事件发生的连接句柄
  union                                                     /**< union alternative identified by evt_id in enclosing struct. */ //由evt_id在封闭结构中识别的联合替代方案。
  {
    ble_gap_evt_connected_t                   connected;                    /**< Connected Event Parameters. */ //连接事件参数。
    ble_gap_evt_disconnected_t                disconnected;                 /**< Disconnected Event Parameters. */ //断开连接的事件参数
    ble_gap_evt_conn_param_update_t           conn_param_update;            /**< Connection Parameter Update Parameters. */ //连接参数更新参数
    ble_gap_evt_sec_params_request_t          sec_params_request;           /**< Security Parameters Request Event Parameters. */ //安全参数请求事件参数
    ble_gap_evt_sec_info_request_t            sec_info_request;             /**< Security Information Request Event Parameters. */ //安全信息请求事件参数
    ble_gap_evt_passkey_display_t             passkey_display;              /**< Passkey Display Event Parameters. */ //万能钥匙显示时间参数
    ble_gap_evt_auth_key_request_t            auth_key_request;             /**< Authentication Key Request Event Parameters. */ //身份验证密钥请求事件参数
    ble_gap_evt_auth_status_t                 auth_status;                  /**< Authentication Status Event Parameters. */ //身份验证状态时间参数。
    ble_gap_evt_conn_sec_update_t             conn_sec_update;              /**< Connection Security Update Event Parameters. */ //连接安全更新时间参数
    ble_gap_evt_timeout_t                     timeout;                      /**< Timeout Event Parameters. */ //超时事件参数
    ble_gap_evt_rssi_changed_t                rssi_changed;                 /**< RSSI Event parameters. */ //RSSI事件参数
    ble_gap_evt_adv_report_t                  adv_report;                   /**< Advertising Report Event Parameters. */ //广播报告事件参数
    ble_gap_evt_sec_request_t                 sec_request;                  /**< Security Request Event Parameters. */ //安全请求事件参数
    ble_gap_evt_conn_param_update_request_t   conn_param_update_request;    /**< Connection Parameter Update Parameters. */ //连接参数更新参数
    ble_gap_evt_scan_req_report_t             scan_req_report;              /**< Scan Request Report parameters. */ //扫描请求报告参数
  } params;                                                                 /**< Event Parameters. */ //事件参数

} ble_gap_evt_t;


/**@brief Channel Map option. //事件参数
 *        
 *
 * @note  Setting the channel map may take some time, depending on connection parameters. //设置通道映射可能需要一些时间，这取决于连接参数。
 *        The time taken may be different for each connection and the get operation will
 *        return the previous channel map until the new one has taken effect. //对于每个连接的时间可能不同而获取操作将会返回之前的通道映射，直到新通道生效。
 *
 * @note  After setting the channel map, by spec it can not be set again until at least 1 s has passed. //在设置了通道映射之后，根据范围，至少在经过1S之前，它不能再被设置。
 *        See Bluetooth Specification Version 4.1 Volume 2, Part E, Section 7.3.46. //参见蓝牙规范V4.1卷2，部分E，第7.3.46节
 *
 * @retval ::NRF_SUCCESS Get or set successful. //NRF_SUCESS 获取 或 设置成功
 * @retval ::NRF_ERROR_BUSY Channel map was set again before enough time had passed. //NRF_ERROR_BUSY  在足够的时间过去之前，频道地图又被设置了。
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation. //NRF_ERROR_INVALID_STATE 无效状态执行操作
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied for get. //BLE_ERROR_INVALID_CONN_HANDLE 为获取提供的无效连接句柄。
 * @retval ::NRF_ERROR_NOT_SUPPORTED Returned by sd_ble_opt_set in peripheral-only SoftDevices. // NRF_ERROR_NOT_SUPPORTED 由sd_ble_opt_set 在外围设备的支持下返回。
 *
 */
typedef struct
{
  uint16_t conn_handle;                   /**< Connection Handle (only applicable for get) */ //连接句柄 (仅适用于get)
  uint8_t ch_map[5];                      /**< Channel Map (37-bit). */ //通道图 (37-bit)
} ble_gap_opt_ch_map_t;


/**@brief Local connection latency option. //本地连接延迟选项
 *
 *        Local connection latency is a feature which enables the slave to improve
 *        current consumption by ignoring the slave latency set by the peer. The //本地连接延迟是一个特性，它使从机能够通过对等点设置的从属延迟来减小电流消耗。
 *        local connection latency can only be set to a multiple of the slave latency, 
 *        and cannot be longer than half of the supervision timeout. //本地连接延迟只能被设置为一个从属性延迟的倍数，而不能超过一半的监督超时。

 *
 *        Used with @ref sd_ble_opt_set to set the local connection latency. The //使用 sd_ble_opt_set 来设置本地连接延迟。
 *        @ref sd_ble_opt_get is not supported for this option, but the actual //这个选项不支持 sd_ble_opt_set,但是实际的本地延迟(除非设置为NULL)在设置选项时被设置为返回参数。
 *        local connection latency (unless set to NULL) is set as a return parameter
 *        when setting the option. 
 *
 * @note  The latency set will be truncated down to the closest slave latency event
 *        multiple, or the nearest multiple before half of the supervision timeout. //延迟集将被截断为最近的多个从属延迟事件，或者是最近在监督超时前的多个。
 *
 * @note  The local connection latency is disabled by default, and needs to be enabled for new
 *        connections and whenever the connection is updated. //本地连接延迟在缺省状态下是禁用的，并且需要为新的连接和当连接被更新时启用。
 *
 * @retval ::NRF_SUCCESS Set successfully. //NRF_SUCCESS 已成功设置。
 * @retval ::NRF_ERROR_NOT_SUPPORTED Get is not supported. //NRF_ERROR_NOT_SUPPORTED 不支持
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle parameter. //BLE_ERROR_INVALID_CONN_HANDLE 无效的连接句柄参数。
 */
typedef struct
{
  uint16_t   conn_handle;                       /**< Connection Handle */ //连接句柄
  uint16_t   requested_latency;                 /**< Requested local connection latency. */ //本地连接请求延迟
  uint16_t * p_actual_latency;                  /**< Pointer to storage for the actual local connection latency (can be set to NULL to skip return value). */ 
	//用于实际本地连接延迟的指针(可以设置为NULL，以跳过返回值).
} ble_gap_opt_local_conn_latency_t;


/**@brief Passkey Option. //万能钥匙的选择
 *
 *        Structure containing the passkey to be used during pairing. This can be used with @ref 
 *        sd_ble_opt_set to make the SoftDevice use a pre-programmed passkey for authentication
 *        instead of generating a random one.
 *
包含在配对过程中使用的密码的结构。这个可以与 sd_ble_opt_set 一起使用，使协议栈使用预先编程的密码进行身份验证而不是随机生成一个。


 * @note  @ref sd_ble_opt_get is not supported for this option. //这个选项不支持 sd_ble_opt_get
 *
 */
typedef struct
{
  uint8_t * p_passkey;                          /**< Pointer to 6-digit ASCII string (digit 0..9 only, no NULL termination) passkey to be used during pairing. If this is NULL, the SoftDevice will generate a random passkey if required.*/
} ble_gap_opt_passkey_t; //指向6位ASCII字符串的指针 (数字0..9，只有在配对时使用的密码才会被使用。如果这是NULL，那么协议栈将在需要时生成一个随机密码)。


/**@brief Custom Privacy Option. //自定义隐私选项。
 *
 *        This structure is used with both @ref sd_ble_opt_set (as input) and with
 *        @ref sd_ble_opt_get (as output). //这个结构同时使用 sd_ble_opt_set (作为输入) 和 sd_ble_opt_get (作为输出)。
 *
 *        Structure containing: //结构包含：
 *        - A pointer to an IRK to set (if input), or a place to store a read IRK (if output). //一个指向IPK设置的指针(如果输入)，或者一个存储读取IRK(如果输出)的位置。
 *        - A private address refresh cycle. //一个私有的地址刷新周期。
 *
 * @note  The specified address cycle interval is used when the address cycle mode is //当地址循环模式时使用指定地址的循环间隔是 BLE_GAP_ADDR_CYCLE_MODE_AUTO。
 *        @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO. If 0 is given, the address will not be automatically  //如果给0，地址将不会自动刷新。
 *        refreshed at all. The default interval is @ref BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S. //默认间隔是 BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S.
 *
 * @note  If the current address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO, the address will immediately be
 *        refreshed when a custom privacy option is set. A new address can be generated manually by calling
 *        @ref sd_ble_gap_address_set with the same type again. 
//如果当前的地址循环模式是 BLE_GAP_ADDR_CYCLE_MODE_AUTO，地址将在设置自定义隐私选项时刷新。可以通过调用 sd_ble_gap_address_set 手动生成同样类型的新地址

 *
 * @note  If the IRK is updated, the new IRK becomes the one to be distributed in all
 *        bonding procedures performed after @ref sd_ble_opt_set returns. //如果IRK被刷新，新的IRK就会变成一个被分发在所有绑定过程之后 sd_ble_opt_set 的返回。
 *
 * @retval ::NRF_SUCCESS Set or read successfully. //设置或读取成功
 * @retval ::NRF_ERROR_INVALID_ADDR The pointer to IRK storage is invalid. //NRF_ERROR_INVALID_ADDR 指向IRK存储器的指针是无效的。
 */
typedef struct
{
  ble_gap_irk_t * p_irk;        /**< When input: Pointer to custom IRK, or NULL to use/reset to the device's default IRK. When output: Pointer to where the current IRK is to be stored, or NULL to not read out the IRK. */
	//输入时：指向自定义IRK的指针，或NULL的使用/重置到设备的默认IRK。输出：指针指向当前IRK要存储的位置，或者NULL不读出IRK。
  uint16_t        interval_s;   /**< When input: Custom private address cycle interval in seconds. When output: The current private address cycle interval. */
	//输入时：定制的私有地址循环间隔在几秒钟内。输出时：当前的私有地址循环间隔。
} ble_gap_opt_privacy_t;


/**@brief Scan request report option. //扫描请求报告选项。
 *
 *        This can be used with @ref sd_ble_opt_set to make the SoftDevice send
 *        @ref BLE_GAP_EVT_SCAN_REQ_REPORT events. //这可以与 sd_ble_opt_set 一起使用，以使软设备发送BLE_GAP_EVT_SCAN_REQ_REPORT事件。
 *
 *  @note   Due to the limited space reserved for scan request report events,//由于为扫描请求报告时间预留的空间有限,
 *          not all received scan requests will be reported.//并不是所有收到的扫描请求都将被报告。

 *
 *  @note   If whitelisting is used, only whitelisted requests are reported. //如果使用白名单，只报告白名单的请求。
 *
 *  @retval ::NRF_SUCCESS Set successfully. //NRF_SUCCESS 已成功设置。
 *  @retval ::NRF_ERROR_INVALID_STATE When advertising is ongoing while the option is set. //NRF_ERROR_INVALID_STATE 当选项被设置的时候,在进行广播。
 */
typedef struct
{
   uint8_t enable : 1;                           /**< Enable scan request reports. */ //使扫描请求报告。
} ble_gap_opt_scan_req_report_t;

/**@brief Compatibility mode option. //兼容模式的选择。
 *
 *        This can be used with @ref sd_ble_opt_set to enable and disable //这可以与 sd_ble_opt_set 一起使用来启用或禁用兼容模式。兼容模式在默认情况下时禁用的。
 *        compatibility modes. Compatibility modes are disabled by default.
 *
 *  @note  Compatibility mode 1 enables interoperability with devices that do not support 
 *         a value of 0 for the WinOffset parameter in the Link Layer CONNECT_REQ packet. 
 *
//兼容模式1 支持与不支持的设备的互操作性在链路层 CONNECT_REQ	包中的win偏移参数值为0.

 *  @retval ::NRF_SUCCESS Set successfully. //NRF_SUCCESS 已成功设置。
 *  @retval ::NRF_ERROR_INVALID_STATE When connection creation is ongoing while mode 1 is set. //NRF_ERROR_INVALID_STATE 当模式1被设置时，连接创建正在进行中。
 */
typedef struct
{
   uint8_t mode_1_enable : 1;                           /**< Enable compatibility mode 1.*/ //使能兼容模式1
} ble_gap_opt_compat_mode_t;


/**@brief Option structure for GAP options. */ //GAP 结构选择
typedef union
{
  ble_gap_opt_ch_map_t                  ch_map;                    /**< Parameters for the Channel Map option. */ //通道映射选项的参数
  ble_gap_opt_local_conn_latency_t      local_conn_latency;        /**< Parameters for the Local connection latency option */ //本地连接延迟选项的参数
  ble_gap_opt_passkey_t                 passkey;                   /**< Parameters for the Passkey option.*/ //万能钥匙选项的参数。
  ble_gap_opt_privacy_t                 privacy;                   /**< Parameters for the Custom privacy option. *///自定义隐私选项的参数
  ble_gap_opt_scan_req_report_t         scan_req_report;           /**< Parameters for the scan request report option.*/ //扫描请求报告选项的参数。
  ble_gap_opt_compat_mode_t             compat_mode;               /**< Parameters for the compatibility mode option.*/ //兼容性模式选项的参数。
} ble_gap_opt_t;
/**@} */


/**@addtogroup BLE_GAP_FUNCTIONS Functions //BLE_GAP_FUNCTIONS 功能函数。
 * @{ */

/**@brief Set local Bluetooth address. //设置本地蓝牙地址
 *
 * @note If the address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_AUTO, the address type is required to
 * be @ref BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE or
 * @ref BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE. The given address is ignored and the
 * SoftDevice will generate a new private address automatically every 
 * @ref BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S seconds. If this API
 * call is used again with the same parameters, the SoftDevice will immediately
 * generate a new private address to replace the current address.
 *
//如果地址循环模式是 BLE_GAP_ADDR_CYCLE_MODE_AUTO，地址类型必须是 BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE或 BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE.
//给定的地址被忽略了，协议栈将自动生成一个新的私人地址 BLE_GAP_DEFAULT_PRIVATE_ADDR_CYCLE_INTERVAL_S 秒。
//如果这个API调用再次使用相同的参数，软设备将立即使用生成一个新的私人地址来替换当前地址。

 * @note If the application wishes to use a @ref BLE_GAP_ADDR_TYPE_PUBLIC or
 * @ref BLE_GAP_ADDR_TYPE_RANDOM_STATIC address, the cycle mode must be
 * @ref BLE_GAP_ADDR_CYCLE_MODE_NONE.
 *
 如果应用程序希望使用 BLE_GAP_ADDR_TYPE_PUBLIC 或 BLE_GAP_ADDR_TYPE_RANDOM_STATIC 地址，循环模式必须是 BLE_GAP_ADDR_CYCLE_MODE_NOTE.
 
 * @note If this API function is called while advertising or scanning, the softdevice will immediately update the
 * advertising or scanning address without the need to stop the procedure in the following cases:
 *   - If the previously set address is of type @ref BLE_GAP_ADDR_TYPE_PUBLIC and the new address
 *   is also of type @ref BLE_GAP_ADDR_TYPE_PUBLIC
 *   - If the previously set address is not @ref BLE_GAP_ADDR_TYPE_PUBLIC and the new address is
 *   also not @ref BLE_GAP_ADDR_TYPE_PUBLIC.
 //如果在广播或扫描时调用这个API函数，那么协议栈将立刻更新广播或扫描地址，不需要在下列情况下停止程序：
 //如果先前设置的地址是 BLE_GAP_ADDR_TYPE_PUBLIC和新地址也是 BLE_GAP_ADDR_TYPE_PUBLIC 类型。
 //如果先前设置的地址不是BLE_GAP_ADDR_TYPE_PUBLIC，新地址也不是 BLE_GAP_ADDR_TYPE_PUBLIC类型。
 
 * If the address is changed from a @ref BLE_GAP_ADDR_TYPE_PUBLIC address to another type or from
 * another type to a @ref BLE_GAP_ADDR_TYPE_PUBLIC address, the change will take effect the next
 * time an advertising or scanning procedure is started.
 *
 //如果地址是从 BLE_GAP_ADDR_TYPE_PUBLIC 地址改为另一种类型，另一种类型是 BLE_GAP_ADDR_TYPE_PUBLIC地址，更改将在下一次生效，启动广播或扫描程序的时间。
 
 * @note If the address cycle mode is @ref BLE_GAP_ADDR_CYCLE_MODE_NONE and the application is
 *       using privacy, the application must take care to generate and set new private addresses
 *       periodically to comply with the Privacy specification in Bluetooth Core Spec.
 
 //如果地址循环模式是 BLE_GAP_ADDR_CYCLE_MODE_NONE,应用程序使用隐私，应用程序必须注意生成和设置新的私有地址定期遵循蓝牙核心规范中的隐私规范。
 
 *
 * @param[in] addr_cycle_mode Address cycle mode, see @ref BLE_GAP_ADDR_CYCLE_MODES. //地址循环模式，参考 BLE_GAP_ADDR_CYCLE_MODES。
 * @param[in] p_addr          Pointer to address structure. //指针指向的地址结构。
 *
 * @retval ::NRF_SUCCESS Address successfully set. //NRF_SUCCESS 地址设置成功。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADDR 提供无效的指针。
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameters. //NRF_ERROR_INVALID_PARAM 无效的参数。
 * @retval ::BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid address. //BLE_ERROR_GAP_INVALID_BLE_ADDR 无效的地址
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry. //NRF_ERROR_BUSY 堆栈忙，处理 未决事件和重试。
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation. //NRF_ERROR_INVALID_STATE 无效状态执行操作。
 */
SVCALL(SD_BLE_GAP_ADDRESS_SET, uint32_t, sd_ble_gap_address_set(uint8_t addr_cycle_mode, const ble_gap_addr_t *p_addr));


/**@brief Get local Bluetooth address. //本地蓝牙地址。
 *
 * @param[out] p_addr Pointer to address structure to be filled in. // p_addr 指向要填充的地址结构的指针。
 *
 * @retval ::NRF_SUCCESS Address successfully retrieved. //NRF_SUCCESS 地址成功地检索
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADDR 提供的指针无效。
 */
SVCALL(SD_BLE_GAP_ADDRESS_GET, uint32_t, sd_ble_gap_address_get(ble_gap_addr_t *p_addr));


/**@brief Set, clear or update advertising and scan response data. //设置、清除或更新广播和扫描响应数据。
 *
 * @note The format of the advertising data will be checked by this call to ensure interoperability. //广播数据的格式将通过这个调用进行检查，以确保互操作性。
 *       Limitations imposed by this API call to the data provided include having a flags data type in the scan response data and
 *       duplicating the local name in the advertising data and scan response data. 
 *
 //这个API调用所提供的数据的限制包括在扫描响应数据中有一个标志数据类型。
 //在广播数据和扫描响应数据中复制本地名称。
 
 * @note To clear the advertising data and set it to a 0-length packet, simply provide a valid pointer (p_data/p_sr_data) with its corresponding 
 *        length (dlen/srdlen) set to 0.
 //为了清除广播数据并将其设置为0长度的数据包，只需提供一个有效的指针 (p_data/p_sr_data)及其对应的数据长度(dlen/srdlen)设为0。
 
 *
 * @note The call will fail if p_data and p_sr_data are both NULL since this would have no effect.
 
 //如果p_data 和 p_sr_data都是NULL，那么调用就会失败，因为这不会产生任何影响。
 
 *
 * @param[in] p_data    Raw data to be placed in advertising packet. If NULL, no changes are made to the current advertising packet data.
 //将原始数据放在广播包中，如果NULL，对当前的广播包数据没有任何更改。
 * @param[in] dlen      Data length for p_data. Max size: @ref BLE_GAP_ADV_MAX_SIZE octets. Should be 0 if p_data is NULL, can be 0 if p_data is not NULL.
 //p_data 数据长度.最大尺寸： BLE_GAP_ADV_MAX_SIZE 字节。如果 p_data 为空，则可以为0，如果p_data不是NULL，则可以是0.
 
 * @param[in] p_sr_data Raw data to be placed in scan response packet. If NULL, no changes are made to the current scan response packet data.
 //将原始数据放置在扫描响应包中。如果NULL，对当前扫描响应包数据没有做任何更改。
 
 * @param[in] srdlen    Data length for p_sr_data. Max size: @ref BLE_GAP_ADV_MAX_SIZE octets. Should be 0 if p_sr_data is NULL, can be 0 if p_data is not NULL.
 //p_sr_data数据长度。最大尺寸： BLE_GAP_ADV_MAX_SIZE 字节。 如果 p_sr_data 为空，则应该为0，如果p_data 不是NULL，则可以为0.
 *
 * @retval ::NRF_SUCCESS Advertising data successfully updated or cleared. //NRF_SUCCESS 广播数据成功更新或清除。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADR 提供无效的指针。
 * @retval ::NRF_ERROR_INVALID_FLAGS Invalid combination of advertising flags supplied. //NRF_ERROR_INVALID_FLAGS 提供了无效的广播标志组合。
 * @retval ::NRF_ERROR_INVALID_DATA Invalid data type(s) supplied, check the advertising data format specification. //NRF_ERROR_INVALID_DATA 提供了无效的数据类型(s) ,检查广播数格式规范。
 * @retval ::NRF_ERROR_INVALID_LENGTH Invalid data length(s) supplied. //NRF_ERROR_INVALID_LENGTH 提供的数据长度无效。
 * @retval ::NRF_ERROR_NOT_SUPPORTED Unsupported data type. //NRF_ERROR_NOT_SUPPORTED 不支持的数据类型。
 * @retval ::BLE_ERROR_GAP_UUID_LIST_MISMATCH Invalid UUID list supplied. //BLE_ERROR_GAP_UUID_LIST_MISMATCH 无效的UUID提供列表。
 */
SVCALL(SD_BLE_GAP_ADV_DATA_SET, uint32_t, sd_ble_gap_adv_data_set(uint8_t const *p_data, uint8_t dlen, uint8_t const *p_sr_data, uint8_t srdlen));


/**@brief Start advertising (GAP Discoverable, Connectable modes, Broadcast Procedure). //开始广播 (GAP 发现，连接性模式，广播程序)
 *
 * @note An application can start an advertising procedure for broadcasting purposes while a connection
 *       is active. After a @ref BLE_GAP_EVT_CONNECTED event is received, this function may therefore
 *       be called to start a broadcast advertising procedure. The advertising procedure
 *       cannot however be connectable (it must be of type @ref BLE_GAP_ADV_TYPE_ADV_SCAN_IND or
 *       @ref BLE_GAP_ADV_TYPE_ADV_NONCONN_IND). @note Only one advertiser may be active at any time.
 *
 //应用程序可以在连接时启动用于广播的广播程序的一个连接是活跃的。在收到 BLE_GAP_EVT_CONNECTED 事件之后，这个函数可能会出现被要求启动
 //一个广播程序，广播但是不可连接。它必须是 BLE_GAP_ADV_TYPE_ADV_SCAN_IND或BLE_GAP_ADV_TYPE_ADV_NONCONN_IND.任何时候，只有一个广播可能是活跃的。
 
 
 * @param[in] p_adv_params Pointer to advertising parameters structure. //p_adv_parms  指向广告参数结构的指针。
 *
 * @retval ::NRF_SUCCESS The BLE stack has started advertising. //NRF_SUCCESS BLE 已经开始广播了。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADDR 提供的指针无效。
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation. //NRF_ERROR_INVALID_STATE 无效状态执行操作。
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check the accepted ranges and limits. //NRF_ERROR_INVALID_PARAM 提供无效的参数，检查已接受的规范和限制。
 * @retval ::BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid Bluetooth address supplied. //BLE_ERROR_GAP_INVALID_BLE_ADD 提供的蓝牙地址无效。
 * @retval ::BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST Discoverable mode and whitelist incompatible. //BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST 可发现模式和白名单不兼容。
 * @retval ::NRF_ERROR_BUSY The stack is busy, process pending events and retry. //NRF_ERROR_BUSY 堆栈忙，处理未决事件和重试。
 */
SVCALL(SD_BLE_GAP_ADV_START, uint32_t, sd_ble_gap_adv_start(ble_gap_adv_params_t const *p_adv_params));


/**@brief Stop advertising (GAP Discoverable, Connectable modes, Broadcast Procedure). //停止广播 (GAP发现，连接性模式，广播程序)
 *
 * @retval ::NRF_SUCCESS The BLE stack has stopped advertising. // NRF_SUCCESS  BLE已停止了广播。
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation (most probably not in advertising state).
 //NRF_ERROR_INVALID_STATE 无效状态执行操作 (很可能不再广播状态。)
 */
SVCALL(SD_BLE_GAP_ADV_STOP, uint32_t, sd_ble_gap_adv_stop(void));


/**@brief Update connection parameters. //更新连接参数
 *
 * @details In the central role this will initiate a Link Layer connection parameter update procedure, //在中心角色，这将启动一个链路层的连接参数更新过程。
 *          otherwise in the peripheral role, this will send the corresponding L2CAP request and wait for //否则在外围角色中，这将发送相应的L2CAP请求并等待执行这个过程的中心。
 *          the central to perform the procedure. In both cases, and regardless of success or failure, the application
 *          will be informed of the result with a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE event. //在这两种情况下，不管成功或失败，应用程序将通过 BLE_GAP_EVT_CONN_PARAM_UPDATE 事件通知结果。
 *
 * @details This function can be used as a central both to reply to a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST or to start the procedure unrequested.
 *
	//这个函数可以用作应答 BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST的中心，也可以用来启动未请求的过程。
	
 
 * @param[in] conn_handle Connection handle. //连接句柄
 * @param[in] p_conn_params  Pointer to desired connection parameters. If NULL is provided on a peripheral role,
 *                           the parameters in the PPCP characteristic of the GAP service will be used instead. //指向所需连接参数的指针，如果在外围角色上提供了NULL，将使用差距服务的PPCP特性的参数来代替。
 *                           If NULL is provided on a central role and in response to a @ref BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST, the peripheral request will be rejected
		如果在一个中心角色上提供了NULL，并且响应 BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST，外围请求将被拒绝。
 *
 * @retval ::NRF_SUCCESS The Connection Update procedure has been started successfully. //NRF_SUCCESS 连接更新过程已经成功启动。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //NRF_ERROR_INVALID_ADDR 提供无效的指针。
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check parameter limits and constraints. //NRF_ERROR_INVALID_PARAM 提供了无效的参数，检查参数限制和约束。
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.//NRF_ERROR_INVALID_STATE 无效状态执行操作。
 * @retval ::NRF_ERROR_BUSY Procedure already in progress or not allowed at this time, process pending events and wait for pending procedures to complete and retry.
	//NRF_ERROR_BUSY 在这个时候已经在进行或者不允许的过程，处理未决事件，等待的过程完成和重试。
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied. //BLE_ERROR_INVALID_CONN_HANDLE 提供的连接句柄无效。
 * @retval ::NRF_ERROR_NO_MEM Not enough memory to complete operation. //NRF_ERROR_NO_MEM 没有足够的内存来完成操作。
 */
SVCALL(SD_BLE_GAP_CONN_PARAM_UPDATE, uint32_t, sd_ble_gap_conn_param_update(uint16_t conn_handle, ble_gap_conn_params_t const *p_conn_params));


/**@brief Disconnect (GAP Link Termination). //断开 (停止GAP 连接)
 *
 * @details This call initiates the disconnection procedure, and its completion will be communicated to the application
 *          with a @ref BLE_GAP_EVT_DISCONNECTED event.
 //这个调用启动断开过程，它的完成将被传递给应用程序使用 BLE_GAP_EVT断开事件。
 *
 * @param[in] conn_handle Connection handle. //连接句柄
 * @param[in] hci_status_code HCI status code, see @ref BLE_HCI_STATUS_CODES (accepted values are @ref BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION and @ref BLE_HCI_CONN_INTERVAL_UNACCEPTABLE).
 *
 //HCL状态码，参见 BLE_HCL_STATUS_CODES (接收的值是 BLE_HCI_REMOTE_USER_TERMINATED_CONNCTION 和 BLE_HCL_CONN_INTERVAL_UNACCEPTABLE).
 
 * @retval ::NRF_SUCCESS The disconnection procedure has been started successfully.
 //NRF_SUCCESS 断开的过程已经成功启动
 
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 //NRF_ERROR_INVALID_PARAM 提供的参数无效。
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 //BLE_ERROR_INVALID_CONN_HANDLE 提供的连接句柄无效。
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation (disconnection is already in progress).
 //NRF_ERROR_INVALID_STATE 无效状态执行操作(断开已经在进行中。)
 */
SVCALL(SD_BLE_GAP_DISCONNECT, uint32_t, sd_ble_gap_disconnect(uint16_t conn_handle, uint8_t hci_status_code));


/**@brief Set the radio's transmit power. 设置无线传输功率
 *
 * @param[in] tx_power Radio transmit power in dBm (accepted values are -40, -30, -20, -16, -12, -8, -4, 0, and 4 dBm). tx_power 无线传输功率 dbm 接收的值
 *
 * @note -40 dBm will not actually give -40 dBm, but will instead be remapped to -30 dBm. -40 dbm实际上不会提供-40dbm，而是会被重映射到-30dbm
 *
 * @retval ::NRF_SUCCESS Successfully changed the transmit power. 返回 NRF_SUCESS Successfuily 表示成功改变了功率值
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied. 返回 NRF_ERROR_INVALTD_PARAM 表示参数无效
 */
SVCALL(SD_BLE_GAP_TX_POWER_SET, uint32_t, sd_ble_gap_tx_power_set(int8_t tx_power));


/**@brief Set GAP Appearance value. //设置GAP 外观值。
 *
 * @param[in] appearance Appearance (16-bit), see @ref BLE_APPEARANCES. //外观(16bit)，参见 BLE_APPEARANCES。
 *
 * @retval ::NRF_SUCCESS  Appearance value set successfully.//NRF_SUCCESS 外观值已成功设置
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied. //NRF_ERROR_INVALID_PARAM 提供的参数无效。
 */
SVCALL(SD_BLE_GAP_APPEARANCE_SET, uint32_t, sd_ble_gap_appearance_set(uint16_t appearance));


/**@brief Get GAP Appearance value. //获取 GAT 外观值
 *
 * @param[out] p_appearance Pointer to appearance (16-bit) to be filled in, see @ref BLE_APPEARANCES. //指向外观值(16bit)的指针，参见 BLE_APPEARANCES.
 *
 * @retval ::NRF_SUCCESS Appearance value retrieved successfully. // NRF_SUCESS 外观值获取成功。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //提供的指针无效。
 */
SVCALL(SD_BLE_GAP_APPEARANCE_GET, uint32_t, sd_ble_gap_appearance_get(uint16_t *p_appearance));


/**@brief Set GAP Peripheral Preferred Connection Parameters. //设置GAP 外围首选的连接参数。
 *
 * @param[in] p_conn_params Pointer to a @ref ble_gap_conn_params_t structure with the desired parameters.
 //p_conn_params 指针指向 ble_agp_conn_params_t 结构，并带有所需的参数。
 *
 * @retval ::NRF_SUCCESS Peripheral Preferred Connection Parameters set successfully.
 // NRF_SUCCESS 外围首选链接参数成功设置。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 //提供的指针无效。
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 //提供的参数无效。
 */
SVCALL(SD_BLE_GAP_PPCP_SET, uint32_t, sd_ble_gap_ppcp_set(ble_gap_conn_params_t const *p_conn_params));


/**@brief Get GAP Peripheral Preferred Connection Parameters.
//获得 GAP 外围首选的连接参数。
 *
 * @param[out] p_conn_params Pointer to a @ref ble_gap_conn_params_t structure where the parameters will be stored.
 //指针指向一个 ble_gap_conn_params_t 结构，其中将存储参数。
 *
 * @retval ::NRF_SUCCESS Peripheral Preferred Connection Parameters retrieved successfully.
 //成功检索外围优先连接参数
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 //提供无效的指针。
 */
SVCALL(SD_BLE_GAP_PPCP_GET, uint32_t, sd_ble_gap_ppcp_get(ble_gap_conn_params_t *p_conn_params));


/**@brief Set GAP device name. //设置 GAP 设备名
 *
 * @param[in] p_write_perm Write permissions for the Device Name characteristic, see @ref ble_gap_conn_sec_mode_t.
 //为设备名特征写权限，请参见 ble_gap_conn_sec_mode_t.
 
 * @param[in] p_dev_name Pointer to a UTF-8 encoded, <b>non NULL-terminated</b> string.
 //指向UTF - 8 编码的，非空终止的字符串的指针。
 
 * @param[in] len Length of the UTF-8, <b>non NULL-terminated</b> string pointed to by p_dev_name in octets (must be smaller or equal than @ref BLE_GAP_DEVNAME_MAX_LEN).
 *
//在字符串中有 p_dev_name 指向的非空终止的字符串长度(必须比 BLE_GAP_DEVNAME_MAX_LEN更小或相等)。 
 
 * @retval ::NRF_SUCCESS GAP device name and permissions set successfully. //成功设置了GAP设备名和权限。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //提供的指针无效。
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied. //提供的参数无效。
 * @retval ::NRF_ERROR_DATA_SIZE Invalid data size(s) supplied.//提供的数据大小无效。
 */
SVCALL(SD_BLE_GAP_DEVICE_NAME_SET, uint32_t, sd_ble_gap_device_name_set(ble_gap_conn_sec_mode_t const *p_write_perm, uint8_t const *p_dev_name, uint16_t len));


/**@brief Get GAP device name. //获取 GAP 设备名
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


/**@brief Initiate the GAP Authentication procedure. //启动 GAP认证过程
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


/**@brief Reply with GAP security parameters. //使用GAP安全参数进行应答
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


/**@brief Reply with an authentication key. //用身份验证密钥回复。
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


/**@brief Initiate GAP Encryption procedure. //启动GAP加密程序。
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


/**@brief Reply with GAP security information. //用GAP安全信息回复
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


/**@brief Get the current connection security. //获得当前的连接安全性
 *
 * @param[in]  conn_handle Connection handle.
 * @param[out] p_conn_sec  Pointer to a @ref ble_gap_conn_sec_t structure to be filled in.
 *
 * @retval ::NRF_SUCCESS Current connection security successfully retrieved.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid connection handle supplied.
 */
SVCALL(SD_BLE_GAP_CONN_SEC_GET, uint32_t, sd_ble_gap_conn_sec_get(uint16_t conn_handle, ble_gap_conn_sec_t *p_conn_sec));


/**@brief Start reporting the received signal strength to the application. //开始向应用程序报告接收到的信号强度。
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


/**@brief Stop reporting the received signal strength. //停止报告接收到的信号强度。
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


/**@brief Get the received signal strength for the last connection event. //获得最后一个链接事件的接收信号强度。
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


/**@brief Start scanning (GAP Discovery procedure, Observer Procedure). //开始扫描(GAP发现程序，观察者程序)
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


/**@brief Stop scanning (GAP Discovery procedure, Observer Procedure). //停止扫描 (GAP发现程序，观察者程序)
 *
 * @retval ::NRF_SUCCESS Successfully stopped scanning procedure.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation (most probably not in scanning state).
 */
SVCALL(SD_BLE_GAP_SCAN_STOP, uint32_t, sd_ble_gap_scan_stop(void));


/**@brief Create a connection (GAP Link Establishment). //创建一个连接(GAP 链路建立)
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


/**@brief Cancel a connection establishment. //取消连接建立。
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
