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
  @addtogroup BLE_COMMON BLE SoftDevice Common
  @{
  @defgroup ble_api Events, type definitions and API calls //事件、类型定义和API调用。
  @{

  @brief Module independent events, type definitions and API calls for the BLE SoftDevice.
	//模块独立事件、类型定义和用于软设备的API调用。

 */

#ifndef BLE_H__
#define BLE_H__

#include "ble_ranges.h"
#include "ble_types.h"
#include "ble_gap.h"
#include "ble_l2cap.h"
#include "ble_gatt.h"
#include "ble_gattc.h"
#include "ble_gatts.h"

/** @addtogroup BLE_COMMON_ENUMERATIONS Enumerations
 * @{ */

/**
 * @brief Common API SVC numbers. //通用 API SVC 的数字。
 */
enum BLE_COMMON_SVCS
{
  SD_BLE_ENABLE = BLE_SVC_BASE,         /**< Enable and initialize the BLE stack //启用和初始化BLE栈*/
  SD_BLE_EVT_GET,                       /**< Get an event from the pending events queue. //从等待事件队列中，获得一个事件。 */
  SD_BLE_TX_BUFFER_COUNT_GET,           /**< Get the total number of available application transmission buffers from the BLE stack. //从BLE栈中获得可用的应用程序传输缓冲区总数*/
  SD_BLE_UUID_VS_ADD,                   /**< Add a Vendor Specific UUID. //添加供应商特定UUID*/
  SD_BLE_UUID_DECODE,                   /**< Decode UUID bytes.  //解码UUID字节*/
  SD_BLE_UUID_ENCODE,                   /**< Encode UUID bytes. //编码UUID字节 */
  SD_BLE_VERSION_GET,                   /**< Get the local version information (company id, Link Layer Version, Link Layer Subversion). //获得本地版本信息(公司id，链路层版本，链路层版本控制)*/
  SD_BLE_USER_MEM_REPLY,                /**< User Memory Reply. //回复用户信息*/
  SD_BLE_OPT_SET,                       /**< Set a BLE option. //设置一个BLE选项*/
  SD_BLE_OPT_GET,                       /**< Get a BLE option. //获得一个BLE选项*/
};

/**
 * @brief BLE Module Independent Event IDs. //BLE模块独立事件ID。
 */
enum BLE_COMMON_EVTS
{
  BLE_EVT_TX_COMPLETE  = BLE_EVT_BASE,  /**< Transmission Complete. @ref ble_evt_tx_complete_t  //传输完成。 引用 ble_evt_tx_complete_t*/
  BLE_EVT_USER_MEM_REQUEST,             /**< User Memory request. @ref ble_evt_user_mem_request_t //用户存储请求。 引用 ble_evt_user_mem_request*/
  BLE_EVT_USER_MEM_RELEASE              /**< User Memory release. @ref ble_evt_user_mem_release_t //用户存储释放。 引用 ble_evt_user_mem_release_t*/
};

/**@brief Common Option IDs. //通用选项ID
 * IDs that uniquely identify a common option.  //确定一个常见选项的唯一ID。
 */
enum BLE_COMMON_OPTS
{
  BLE_COMMON_OPT_RADIO_CPU_MUTEX = BLE_OPT_BASE    /**< Radio CPU mutex option. @ref ble_common_opt_radio_cpu_mutex_t  //无线电 CPU 互斥 选项 */
};
/** @} */

/** @addtogroup BLE_COMMON_DEFINES Defines //BLE_COMMON_DEFINES 定义
 * @{ */

/** @brief  Required pointer alignment for BLE Events. //BLE事件所需指针对齐。
*/
#define BLE_EVTS_PTR_ALIGNMENT    4

/** @defgroup BLE_USER_MEM_TYPES User Memory Types //用户内存类型
 * @{ */
#define BLE_USER_MEM_TYPE_INVALID               0x00  /**< Invalid User Memory Types.  //无效的用户内存类型*/
#define BLE_USER_MEM_TYPE_GATTS_QUEUED_WRITES   0x01  /**< User Memory for GATTS queued writes. GATTS队列写入用户存储*/
/** @} */

/** @brief  Maximum number of Vendor Specific UUIDs. //供应商特定UUID的最大数量。
*/
#define BLE_UUID_VS_MAX_COUNT     10

/** @} */

/** @addtogroup BLE_COMMON_STRUCTURES Structures  BLE_COMMON_STRUCTURES 结构。
 * @{ */

/**@brief User Memory Block. //用户内存块*/
typedef struct
{
  uint8_t          *p_mem;      /**< Pointer to the start of the user memory block. //指向用户内存块开始的指针*/
  uint16_t          len;        /**< Length in bytes of the user memory block. //用户内存块的字节长度*/
} ble_user_mem_block_t;

/**
 * @brief Event structure for @ref BLE_EVT_TX_COMPLETE. BLE_EVT_TX_COMPLETE 事件结构。
 */
typedef struct
{
  uint8_t count;                        /**< Number of packets transmitted. //传输的数据包数量 */
} ble_evt_tx_complete_t;

/**@brief Event structure for @ref BLE_EVT_USER_MEM_REQUEST.  事件结构 引用 BLE_EVT_USER_MEM_REQUEST */
typedef struct
{
  uint8_t                     type;     /**< User memory type, see @ref BLE_USER_MEM_TYPES. 用户内存类型，参考 BLE_USER_MEM_TYPES*/
} ble_evt_user_mem_request_t;

/**@brief Event structure for @ref BLE_EVT_USER_MEM_RELEASE.  //事件结构 引用 BLE_EVT_USER_MEM_RELEASE*/
typedef struct
{
  uint8_t                     type;       /**< User memory type, see @ref BLE_USER_MEM_TYPES.//用户内存类型，参考 BLE_USER_MEM_TYPES */
  ble_user_mem_block_t        mem_block;  /**< User memory block */  //用户内存块
} ble_evt_user_mem_release_t;


/**@brief Event structure for events not associated with a specific function module. */ //与特定功能模块无关的事件的事件结构
typedef struct
{
  uint16_t conn_handle;                 /**< Connection Handle on which this event occurred. */ //这个事件发生的连接处理
  union
  {
    ble_evt_tx_complete_t           tx_complete;        /**< Transmission Complete. */ //传输完成
    ble_evt_user_mem_request_t      user_mem_request;   /**< User Memory Request Event Parameters. */ //用户内存请求事件参数
    ble_evt_user_mem_release_t      user_mem_release;   /**< User Memory Release Event Parameters. */ //用户内存释放事件参数
  } params;
} ble_common_evt_t;

/**@brief BLE Event header. */ //BLE事件标题
typedef struct
{
  uint16_t evt_id;                      /**< Value from a BLE_<module>_EVT series. */ //值来自 BLE_<module>_EVT 系列
  uint16_t evt_len;                     /**< Length in octets excluding this header. */ //不包括这个头的八字节长度。
} ble_evt_hdr_t;

/**@brief Common BLE Event type, wrapping the module specific event reports. */ //通用BLE事件类型，打包特定于模块的事件报告。
typedef struct
{
  ble_evt_hdr_t header;                 /**< Event header. */ //事件标题
  union
  {
    ble_common_evt_t  common_evt;         /**< Common Event, evt_id in BLE_EVT_* series. */ //普通事件，evt_id 在 BLE_EVT_* 系列
    ble_gap_evt_t     gap_evt;            /**< GAP originated event, evt_id in BLE_GAP_EVT_* series. */ //GAP 发起事件，evt_id 在 BLE_GAP_EVT_* 系列
    ble_l2cap_evt_t   l2cap_evt;          /**< L2CAP originated event, evt_id in BLE_L2CAP_EVT* series. */ //L2CAP 发起事件，evt_id 在 BLE_L2CAP_EVT* 系列
    ble_gattc_evt_t   gattc_evt;          /**< GATT client originated event, evt_id in BLE_GATTC_EVT* series. */ //GATT客户端发起事件，evt_id 在 BLE_GATTC_EVT* 系列
    ble_gatts_evt_t   gatts_evt;          /**< GATT server originated event, evt_id in BLE_GATTS_EVT* series. */ //GATT服务端发起事件，evt_id 在 BLE_GATTS_EVT* 系列
  } evt;
} ble_evt_t;


/**
 * @brief Version Information. //版本信息
 */
typedef struct
{
  uint8_t   version_number;             /**< Link Layer Version number for BT 4.1 spec is 7 (https://www.bluetooth.org/en-us/specification/assigned-numbers/link-layer).//BT4.1规范的链接层，版本号是7 */
  uint16_t  company_id;                 /**< Company ID, Nordic Semiconductor's company ID is 89 (0x0059) (https://www.bluetooth.org/apps/content/Default.aspx?doc_id=49708). //公司ID，Nordic公司ID为89(0x0059)*/
  uint16_t  subversion_number;          /**< Link Layer Sub Version number, corresponds to the SoftDevice Config ID or Firmware ID (FWID). *///链接层子版本号，对应于软设备配置ID或固件ID（FWID）
} ble_version_t;

/**@brief Mutual exclusion of radio activity and CPU execution. //无线电活动和CPU执行，互斥。
 *
 *        This option configures the application's access to the CPU when the radio is active. The
 *        application can configure itself to be blocked from using the CPU while the radio is
 *        active. By default, the application will be able to share CPU time with the SoftDevice
 *        during radio activity. This parameter structure is used together with @ref sd_ble_opt_set
 *        to configure the @ref BLE_COMMON_OPT_RADIO_CPU_MUTEX option.
					//这个选项在广播处于活动状态时配置应用程序对CPU的访问。应用程序可以在广播的时候将自己配置为阻止使用CPU活动。
					//默认情况下，应用程序将能够与协议栈共享CPU时间。
					//在无线电活动，这个参数结构与 sd_ble_opt_set一起使用。要配置 BLE_COMMON_OPT_RADIO_CPU_MUTEX 选项。
 *
 * @note  Note that the application should use this option to configure the SoftDevice to block the
 *        CPU during radio activity (i.e enable mutual exclusion) when running the SoftDevice on
 *        hardware affected by PAN #44 "CCM may exceed real time requirements"and PAN #45 "AAR may
 *        exceed real time requirements".
					//注意，应用程序一个使用这个选项来配置软设备来阻止CPU在无线电活动期间活动。(也就是 实现互斥)
					//在运行设备时，硬件受到 PAN #44 'CMM可能超过实际时间要求'和 PAN#45  'AAR可能超过实际时间要求'影响。

 *
 * @note  Note that when acting as a scanner, the mutex is only enabled for radio TX activity.
				//注意，当作为扫描器时，互斥锁只支持无线电TX活动。
 *
 * @note  @ref sd_ble_opt_get is not supported for this option. //sd_ble_opt_get 不支持这个选项。
 *
 */
typedef struct
{
  uint8_t enable : 1;                          /**< Enable mutual exclusion of radio activity and the CPU execution. */ 
	//允许无线电活动和CPU执行相互排斥。
} ble_common_opt_radio_cpu_mutex_t;

/**@brief Option structure for common options. */
//常见选项的选项结构。
typedef union
{
  ble_common_opt_radio_cpu_mutex_t  radio_cpu_mutex;        /**< Parameters for the option for the mutual exclusion of radio activity and CPU execution. */
	//用于互斥无线电活动和CPU执行的选项的参数。
} ble_common_opt_t;

/**@brief Common BLE Option type, wrapping the module specific options. */
//常见的选项类型，包装模块的特定选项。
typedef union
{
  ble_common_opt_t  common_opt;         /**< Common option, opt_id in BLE_COMMON_OPT_* series. */ //通用选项，opt_id 在 BLE_COMMON_OPT_*系列 
  ble_gap_opt_t     gap_opt;            /**< GAP option, opt_id in BLE_GAP_OPT_* series. */ //GAP 选项，opt_id 在 BLE_GAP_OPT_* 系列。
} ble_opt_t;

/**
 * @brief BLE GATTS init options //BLE GATTS 初始化选项。
 */
typedef struct
{
  ble_gatts_enable_params_t  gatts_enable_params; /**< GATTS init options @ref ble_gatts_enable_params_t. */ //GATTS 初始化 选项 引用 ble_gatts_enable_params_t
} ble_enable_params_t;

/** @} */

/** @addtogroup BLE_COMMON_FUNCTIONS Functions  // BLE_COMMON_FUNCTIONS 功能
 * @{ */

/**@brief Enable the BLE stack //使能 BLE 堆栈
 *
 * @param[in] p_ble_enable_params Pointer to ble_enable_params_t // p_ble_enable_params 指向了 ble_enable_params_t
 *
 * @details This call initializes the BLE stack, no other BLE related function can be called before this one. //这个调用初始化BLE堆栈，在此之前，没有其他BLE相关的函数可以调用。
 *
 * @return @ref NRF_SUCCESS BLE the BLE stack has been initialized successfully //BLE栈被成功初始化。
 * @retval @ref NRF_ERROR_INVALID_STATE the BLE stack had already been initialized and cannot be reinitialized. //已经初始化了BLE栈，并且不能重新初始化。
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid or not sufficiently aligned pointer supplied. //提供了无效的或没有足够对其的指针。
 * @return @ref NRF_ERROR_INVALID_LENGTH The specified Attribute Table size is either too small or not a multiple of 4.  //指定的属性表太大或太小，或者不是4的倍数。
 *                                       The minimum acceptable size is defined by @ref BLE_GATTS_ATTR_TAB_SIZE_MIN. //最小可接受的大小是由 BLE_GATTS_ATTR_TAB_SIZE_MIN.定义的。
 * @return @ref NRF_ERROR_NO_MEM         The Attribute Table size is too large. Decrease size in @ref ble_gatts_enable_params_t. //属性表的大小太大了，在 ble_gatts_enable_params_t 减小尺寸 
 */
SVCALL(SD_BLE_ENABLE, uint32_t, sd_ble_enable(ble_enable_params_t * p_ble_enable_params));

/**@brief Get an event from the pending events queue. //从未处理的事件队列中获得一个事件。
 *
 * @param[out] p_dest Pointer to buffer to be filled in with an event, or NULL to retrieve the event length. This buffer <b>must be 4-byte aligned in memory</b>.
 //指向缓冲区的指针，以一个事件填充，或者事件长度是空的。这个缓冲区必须是在内存中四字节对齐。
 * @param[in, out] p_len Pointer the length of the buffer, on return it is filled with the event length.
 //指针指向缓冲区的长度，返回时它将填满事件长度。
 *
 * @details This call allows the application to pull a BLE event from the BLE stack. The application is signalled that an event is 
 * available from the BLE stack by the triggering of the SD_EVT_IRQn interrupt.
 * The application is free to choose whether to call this function from thread mode (main context) or directly from the Interrupt Service Routine
 * that maps to SD_EVT_IRQn. In any case however, and because the BLE stack runs at a higher priority than the application, this function should be called
 * in a loop (until @ref NRF_ERROR_NOT_FOUND is returned) every time SD_EVT_IRQn is raised to ensure that all available events are pulled from the BLE stack. 
 * Failure to do so could potentially leave events in the internal queue without the application being aware of this fact.
 * Sizing the p_dest buffer is equally important, since the application needs to provide all the memory necessary for the event to be copied into
 * application memory. If the buffer provided is not large enough to fit the entire contents of the event, @ref NRF_ERROR_DATA_SIZE will be returned
 * and the application can then call again with a larger buffer size.
 * Please note that because of the variable length nature of some events, sizeof(ble_evt_t) will not always be large enough to fit certain events, 
 * and so it is the application's responsibility to provide an amount of memory large enough so that the relevant event is copied in full.
 * The application may "peek" the event length by providing p_dest as a NULL pointer and inspecting the value of *p_len upon return.
 *
 这个调用允许应用程序从BLE栈中拉出一个BLE事件，应用程序表示一个事件是通过SD_EVT_IRQ	n中断触发，可以从BLE栈中获得。
 应用程序可以自由选择是否从线程模式调用此函数，或者从中断服务程序中映射到 SD_EVT_IRQn. 无论如何，由于这个堆栈比应用程序的优先级更高，
 这个函数应该在一个循环中被调用 ，(直到 NRF_ERROR_NOT_FOUND 返回)，一直提高SD_EVT_IRQn,以确保所有可用的事件都从BLE栈中提取出来。
 * @note The pointer supplied must be aligned to the extend defined by @ref BLE_EVTS_PTR_ALIGNMENT
 *
 //所提供的指针必须与由 BLE_EVTS_PTR_ALLGNMENT 对齐定义的扩展保持一致。
 
 * @retval ::NRF_SUCCESS Event pulled and stored into the supplied buffer. //事件被拉取并存储到提供的缓冲区中。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid or not sufficiently aligned pointer supplied. //提供了无效的或没有足够对其的指针。
 * @retval ::NRF_ERROR_NOT_FOUND No events ready to be pulled. //没有准备好被拉取的事件。
 * @retval ::NRF_ERROR_DATA_SIZE Event ready but could not fit into the supplied buffer. //事件准备好了，但是不能适应提供的缓存区。
 */
SVCALL(SD_BLE_EVT_GET, uint32_t, sd_ble_evt_get(uint8_t *p_dest, uint16_t *p_len));


/**@brief Get the total number of available application transmission buffers per link in the BLE stack. //在BLE栈中获取每个连接的可用应用程序传输缓冲区的总数。
 *
 * @details This call allows the application to obtain the total number of
 *          transmission buffers available per link for application data. Please note that
 *          this does not give the number of free buffers, but rather the total amount of them.
 *          The application has two options to handle its own application transmission buffers:
 *          - Use a simple arithmetic calculation: at boot time the application should use this function
 *          to find out the total amount of buffers available to it and store it in a variable.
 *          Every time a packet that consumes an application buffer is sent using any of the 
 *          exposed functions in this BLE API, the application should decrement that variable.
 *          Conversely, whenever a @ref BLE_EVT_TX_COMPLETE event is received by the application
 *          it should retrieve the count field in such event and add that number to the same
 *          variable storing the number of available packets.
 *          This mechanism allows the application to be aware at any time of the number of
 *          application packets available in the BLE stack's internal buffers, and therefore
 *          it can know with certainty whether it is possible to send more data or it has to
 *          wait for a @ref BLE_EVT_TX_COMPLETE event before it proceeds.
 *          - Choose to simply not keep track of available buffers at all, and instead handle the 
 *          @ref BLE_ERROR_NO_TX_BUFFERS error by queueing the packet to be transmitted and 
 *          try again as soon as a @ref BLE_EVT_TX_COMPLETE event arrives.
 *
	这个调用允许获得应用程序总数量，应用程序的每个链接可以获得传输缓冲区。请注意，这里并没有给出自由缓冲区的数量，而是它们的总数量。
	应用程序有两个选项来处理自己的应用程序缓冲区:使用一个简单的算术计算：在启动时应用程序应该使用这个函数要找出可用的缓冲区的总量，并将其存储在一个变量中。
 每一次使用一个应用程序缓冲区的数据包都是通过以下方式发送的在这个BLE API中公开的函数，应用程序应该减少这个变量。
 相反，每当应用程序接收 BLE_EVT_TX_COMPLETE事件时它应该在这样的事件中检索计数字段并将该数字添加到相同的位置变量存储可用数据包的数量。
 
 这个机制允许应用程序在任何时候都知道应用程序包可以在BLE栈的内部缓冲区中使用，因此它可用肯定地知道是否有可能发送更多的数据或者它必须等待一个 BLE_EVT_TX_COMPLETE事件发生。
 
 
 *          The API functions that <b>may</b> consume an application buffer depending on 
 *          the parameters supplied to them can be found below:
 API函数可能会消耗一个应用程序缓冲区，这取决于提供给他们的参数可以在下面找到：
 *
 *          - @ref sd_ble_gattc_write (write without response only) //只写没有响应
 *          - @ref sd_ble_gatts_hvx (notifications only)  //只通知
 *          - @ref sd_ble_l2cap_tx (all packets) // 所有数据包
 *
 * @param[out] p_count Pointer to a uint8_t which will contain the number of application transmission buffers upon
 *                     successful return. //指向uint8_t 的指针，它将返回包含应用程序传输缓冲区成功的数量
 *
 * @retval ::NRF_SUCCESS Number of application transmission buffers retrieved successfully. //成功检索的应用程序传输缓冲区的数量。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //提供无效的指针。
 */
SVCALL(SD_BLE_TX_BUFFER_COUNT_GET, uint32_t, sd_ble_tx_buffer_count_get(uint8_t *p_count));


/**@brief Add a Vendor Specific UUID.  //添加供应商特定UUID。
 *
 * @details This call enables the application to add a vendor specific UUID to the BLE stack's table,
 *          for later use all other modules and APIs. This then allows the application to use the shorter,
 *          24-bit @ref ble_uuid_t format when dealing with both 16-bit and 128-bit UUIDs without having to
 *          check for lengths and having split code paths. The way that this is accomplished is by extending the 
 *          grouping mechanism that the Bluetooth SIG standard base UUID uses for all other 128-bit UUIDs. The 
 *          type field in the @ref ble_uuid_t structure is an index (relative to @ref BLE_UUID_TYPE_VENDOR_BEGIN) 
 *          to the table populated by multiple calls to this function, and the uuid field in the same structure 
 *          contains the 2 bytes at indices 12 and 13. The number of possible 128-bit UUIDs available to the 
 *          application is therefore the number of Vendor Specific UUIDs added with the help of this function times 65536, 
 *          although restricted to modifying bytes 12 and 13 for each of the entries in the supplied array.
 
	这个调用使应用程序可以向BLE栈的表添加特定于供应商的UUID，供以后所有的模块和API使用。
	这样就可以让应用程序使用更短的 24位 引用 ble_uuid_t 格式 ，而不需要处理16位和128位uuids时，检查长度和分割代码路径。
  这个方法的实现时通过扩展蓝牙团体标准基础UUID为所有其他128位UUID使用的分组机制。
	ble_uuid_t 结构中 type字段是一个索引(相对于 BLE_UUID_TYPE_VENDOR_BEGIN)对于这个函数的多次调用，以及同一个结构中的uuid字段包含索引12和13两个字节。
	可能的128位UUIDs的数量，在这个函数的帮助下，应用程序是特定于供应商的UUIDs的数量乘以65536，尽管限制为所提供的阵列中的每个条目修改字节12和13.
 *
 * @note Bytes 12 and 13 of the provided UUID will not be used internally, since those are always replaced by 
 * the 16-bit uuid field in @ref ble_uuid_t.
 所提供的UUID的字节12和13将不会在内部使用，应为它们总是被替换为 ble_uuid_t 中的uuid 字段。
 *
 *
 * @param[in]  p_vs_uuid    Pointer to a 16-octet (128-bit) little endian Vendor Specific UUID disregarding
 *                          bytes 12 and 13.
 指向16字节的供应商特定UUID，忽略字节12和13.
 * @param[out] p_uuid_type  Pointer to a uint8_t where the type field in @ref ble_uuid_t corresponding to this UUID will be stored.
 
 指向uint8_t的指针，其中ble_uuid_t 对应于这个uuid的type字段将被储存。
 
 *
 * @retval ::NRF_SUCCESS Successfully added the Vendor Specific UUID. //成功地添加了供应商特定UUID
 * @retval ::NRF_ERROR_INVALID_ADDR If p_vs_uuid or p_uuid_type is NULL or invalid. //在 p_vs_uuid 或 p_uuid_type 为空或无效。
 * @retval ::NRF_ERROR_NO_MEM If there are no more free slots for VS UUIDs. //在没有更多的空闲时间用于VS UUIDS时。
 * @retval ::NRF_ERROR_FORBIDDEN If p_vs_uuid has already been added to the VS UUID table. //在p_vs_uuid 已经被添加到VS UUID表中时。
 */
SVCALL(SD_BLE_UUID_VS_ADD, uint32_t, sd_ble_uuid_vs_add(ble_uuid128_t const *p_vs_uuid, uint8_t *p_uuid_type));


/** @brief Decode little endian raw UUID bytes (16-bit or 128-bit) into a 24 bit @ref ble_uuid_t structure.
将小的原始UUID字节(16位或128位)解码成一个24为 ble_uuid_t结构。
 * 
 * @details The raw UUID bytes excluding bytes 12 and 13 (i.e. bytes 0-11 and 14-15) of p_uuid_le are compared 
 * to the corresponding ones in each entry of the table of vendor specific UUIDs populated with @ref sd_ble_uuid_vs_add 
 * to look for a match. If there is such a match, bytes 12 and 13 are returned as p_uuid->uuid and the index 
 * relative to @ref BLE_UUID_TYPE_VENDOR_BEGIN as p_uuid->type. 
 
 不包括12和13字节的原始UUID字节 (也就是 0-11 和 14-15) 和p_uuid_le 的比较在供应商特定的UUIDs表的每一项中对应的对应项。
 sd_ble_uuid_vs_add在供应商特定的UUIDs表的每个对应的条目中去寻找一个匹配的。如果有匹配的，那么字节12和13就会返回为puuid->uuid的索引，相对于
 ble_uuid_type_vendor_begin 作为 p_uuid->type的开始。
 
 *
 * @note If the UUID length supplied is 2, then the type set by this call will always be @ref BLE_UUID_TYPE_BLE.
 如果提供的UUID长度是2，那么这个调用所设置的类型将始终是 BLE_UUID_TYPE_BLE.
 
 
 *
 * @param[in]      uuid_le_len Length in bytes of the buffer pointed to by p_uuid_le (must be 2 or 16 bytes).
 
 //缓冲字节的长度指向 p_uuid_le (一定要在 2 - 16 字节之间)。
 
 * @param[in]      p_uuid_le   Pointer pointing to little endian raw UUID bytes.
 指向原始短UUID字节的指针。
 
 * @param[out]  p_uuid      Pointer to a @ref ble_uuid_t structure to be filled in.
 指向一个 ble_uuit_t 结构的指针。
 *
 * @retval ::NRF_SUCCESS Successfully decoded into the @ref ble_uuid_t structure.
 成功地解码成 ble_uuid_t 结构
 
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.  //提供的指针无效。
 * @retval ::NRF_ERROR_INVALID_LENGTH Invalid UUID length. //无效的UUID长度
 * @retval ::NRF_ERROR_NOT_FOUND For a 128-bit UUID, no match in the populated table of UUIDs.
 //对于128位UUID，在填充的UUIDs表中没有匹配。
 */                                                 
SVCALL(SD_BLE_UUID_DECODE, uint32_t, sd_ble_uuid_decode(uint8_t uuid_le_len, uint8_t const *p_uuid_le, ble_uuid_t *p_uuid));


/** @brief Encode a @ref ble_uuid_t structure into little endian raw UUID bytes (16-bit or 128-bit).
将 ble_uuid_t 结构编码成 短UUID字节 (16位或128位)
 *
 * @note The pointer to the destination buffer p_uuid_le may be NULL, in which case only the validity and size of p_uuid is computed.
 指向目的地缓冲区 p_uuid_le 的指针可能是空的，在这种情况下，只计算p_uuid 的有效性和大小。
 
 *
 * @param[in]      p_uuid        Pointer to a @ref ble_uuid_t structure that will be encoded into bytes.
 //指向一个 ble_uuid_t 指针，它将被编码成字节。
 * @param[out]     p_uuid_le_len Pointer to a uint8_t that will be filled with the encoded length (2 or 16 bytes).
 //指向一个uint8_t 的指针，它将被按编码的长度(2或16个字节)填满。
 
 * @param[out]     p_uuid_le     Pointer to a buffer where the little endian raw UUID bytes (2 or 16) will be stored.
 指针指向一个缓冲区，其中短的原始UUID(2或16字节)将被存储。
 *
 * @retval ::NRF_SUCCESS Successfully encoded into the buffer. //成功地编码到缓冲区。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //提供无效的指针。
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid UUID type. //无效的UUID类型。
 */
SVCALL(SD_BLE_UUID_ENCODE, uint32_t, sd_ble_uuid_encode(ble_uuid_t const *p_uuid, uint8_t *p_uuid_le_len, uint8_t *p_uuid_le));


/**@brief Get Version Information. //获取版本信息。
 *
 * @details This call allows the application to get the BLE stack version information.
 *
 //这个调用允许应用程序获得BLE堆栈版本信息
 
 * @param[out] p_version Pointer to a ble_version_t structure to be filled in.
 
 //指向一个 ble_version_t 结构的指针。
 
 *
 * @retval ::NRF_SUCCESS  Version information stored successfully. //版本信息保存成功。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //提供无效的指针
 * @retval ::NRF_ERROR_BUSY The BLE stack is busy (typically doing a locally-initiated disconnection procedure). //堆栈忙碌状态，(通常是在本地启动的断开过程)
 */
SVCALL(SD_BLE_VERSION_GET, uint32_t, sd_ble_version_get(ble_version_t *p_version));


/**@brief Provide a user memory block. //提供一个用户内存块。
 *
 * @note This call can only be used as a response to a @ref BLE_EVT_USER_MEM_REQUEST event issued to the application.
 *
 //这个调用只能作为对发出给应用程序的 BLE_EVT_MEM_REQUEST 事件的响应。
 
 * @param[in] conn_handle Connection handle. //连接句柄
 * @param[in,out] p_block Pointer to a user memory block structure. //指向用户内存块结构的指针。
 *
 * @retval ::NRF_SUCCESS Successfully queued a response to the peer. //成功的回应。
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle. //无效的连接句柄
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection state or no execute write request pending. //无效连接状态或未执行写请求。
 * @retval ::NRF_ERROR_BUSY The BLE stack is busy. Retry at later time. //堆栈忙，稍后重试。
 */
SVCALL(SD_BLE_USER_MEM_REPLY, uint32_t, sd_ble_user_mem_reply(uint16_t conn_handle, ble_user_mem_block_t const *p_block));

/**@brief Set a BLE option. //设置一个BLE选项。
 *
 * @details This call allows the application to set the value of an option. //该调用允许应用程序设定选项的值。
 *
 * @param[in] opt_id Option ID. 选项ID。
 * @param[in] p_opt Pointer to a ble_opt_t structure containing the option value. //指针指向一个包含选项值的 ble_opt_t结构
 *
 * @retval ::NRF_SUCCESS  Option set successfully. //选项设置成功
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //提供的指针无效
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle. //无效的连接句柄
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check parameter limits and constraints. 
 //提供了无效的参数(s) ，检查参数限制和约束。
 * @retval ::NRF_ERROR_INVALID_STATE Unable to set the parameter at this time. 
 //无法在此时设置参数。
 * @retval ::NRF_ERROR_BUSY The BLE stack is busy or the previous procedure has not completed.
 //堆栈忙，或者之前的程序没有完成。
 */
SVCALL(SD_BLE_OPT_SET, uint32_t, sd_ble_opt_set(uint32_t opt_id, ble_opt_t const *p_opt));


/**@brief Get a BLE option. 
//获取 BLE 选项。
 *
 * @details This call allows the application to retrieve the value of an option.
 //这个调用允许应用程序检索选项的值。
 *
 * @param[in] opt_id Option ID. //选项ID
 * @param[out] p_opt Pointer to a ble_opt_t structure to be filled in. 
 //指向一个 ble_opt_t 结构的指针。
 *
 * @retval ::NRF_SUCCESS  Option retrieved successfully. //选择检索成功。
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied. //提供无效的指针
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.//无效的连接句柄。
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check parameter limits and constraints.
 //提供了无效的参数(s)，检查参数极限和约束。
 * @retval ::NRF_ERROR_INVALID_STATE Unable to retrieve the parameter at this time.
 //无法在此时检索参数。
 * @retval ::NRF_ERROR_BUSY The BLE stack is busy or the previous procedure has not completed.
 //堆栈忙，或者之前的程序还没有完成。
 * @retval ::NRF_ERROR_NOT_SUPPORTED This option is not supported.
 //这个选项不受支持。
 *
 */
SVCALL(SD_BLE_OPT_GET, uint32_t, sd_ble_opt_get(uint32_t opt_id, ble_opt_t *p_opt));

/** @} */

#endif /* BLE_H__ */

/**
  @}
  @}
*/
