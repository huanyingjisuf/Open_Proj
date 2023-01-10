/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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
#ifndef NRF_GPIO_H__
#define NRF_GPIO_H__

#include "nrf.h"
#include <stdbool.h>

/**
 * @defgroup nrf_gpio GPIO abstraction
 * @{
 * @ingroup nrf_drivers
 * @brief GPIO pin abstraction and port abstraction for reading and writing byte-wise to GPIO ports.
 *
 * Here, the GPIO ports are defined as follows:
 * - Port 0 -> pin 0-7
 * - Port 1 -> pin 8-15
 * - Port 2 -> pin 16-23
 * - Port 3 -> pin 24-31
 */

#define NUMBER_OF_PINS 32

/**
 * @brief Enumerator used for selecting between port 0 - 3. 可用于选择0-3组端口
 */
typedef enum
{
    NRF_GPIO_PORT_SELECT_PORT0 = 0,           ///<  Port 0 (GPIO pin 0-7)
    NRF_GPIO_PORT_SELECT_PORT1,               ///<  Port 1 (GPIO pin 8-15)
    NRF_GPIO_PORT_SELECT_PORT2,               ///<  Port 2 (GPIO pin 16-23)
    NRF_GPIO_PORT_SELECT_PORT3,               ///<  Port 3 (GPIO pin 24-31)
} nrf_gpio_port_select_t;

/**
 * @brief Enumerator used for setting the direction of a GPIO port. GPIO设置端口方向(整组端口)
 */
typedef enum
{
    NRF_GPIO_PORT_DIR_OUTPUT,       ///<  Output
    NRF_GPIO_PORT_DIR_INPUT         ///<  Input
} nrf_gpio_port_dir_t;

/**
 * @brief Pin direction definitions. 引脚方向定义(单个引脚)
 */
typedef enum
{
    NRF_GPIO_PIN_DIR_INPUT  = GPIO_PIN_CNF_DIR_Input,   ///< Input 输入
    NRF_GPIO_PIN_DIR_OUTPUT = GPIO_PIN_CNF_DIR_Output   ///< Output  输出
} nrf_gpio_pin_dir_t;

/**
 * @brief Connection of input buffer 输入缓存区
 */
typedef enum
{
    NRF_GPIO_PIN_INPUT_CONNECT    = GPIO_PIN_CNF_INPUT_Connect,   ///< Connect input buffer 连接输入缓存区
    NRF_GPIO_PIN_INPUT_DISCONNECT = GPIO_PIN_CNF_INPUT_Disconnect ///< Disconnect input buffer  断开输入缓存区
} nrf_gpio_pin_input_t;

/**
 * @brief Enumerator used for selecting the pin to be pulled down or up at the time of pin configuration  用于配制IO口内部上拉或下拉电阻的枚举
 */
typedef enum
{
    NRF_GPIO_PIN_NOPULL   = GPIO_PIN_CNF_PULL_Disabled,                 ///<  Pin pullup resistor disabled 禁用上拉电阻器
    NRF_GPIO_PIN_PULLDOWN = GPIO_PIN_CNF_PULL_Pulldown,                 ///<  Pin pulldown resistor enabled 启用下拉电阻器
    NRF_GPIO_PIN_PULLUP   = GPIO_PIN_CNF_PULL_Pullup,                   ///<  Pin pullup resistor enabled  启用上拉电阻器
} nrf_gpio_pin_pull_t; 

/**
 * @brief Enumerator used for selecting output drive mode 用于选择管脚输出驱动模式的枚举
 */
typedef enum
{
    NRF_GPIO_PIN_S0S1 = GPIO_PIN_CNF_DRIVE_S0S1, ///< !< Standard '0', standard '1' 标准的0，标准的1.
    NRF_GPIO_PIN_H0S1 = GPIO_PIN_CNF_DRIVE_H0S1, ///< !< High drive '0', standard '1' 高驱动0，标准1
    NRF_GPIO_PIN_S0H1 = GPIO_PIN_CNF_DRIVE_S0H1, ///< !< Standard '0', high drive '1'  标准0，高驱动1
    NRF_GPIO_PIN_H0H1 = GPIO_PIN_CNF_DRIVE_H0H1, ///< !< High drive '0', high 'drive '1'' 高驱动0，高驱动1
    NRF_GPIO_PIN_D0S1 = GPIO_PIN_CNF_DRIVE_D0S1, ///< !< Disconnect '0' standard '1'  断开0，标准1
    NRF_GPIO_PIN_D0H1 = GPIO_PIN_CNF_DRIVE_D0H1, ///< !< Disconnect '0', high drive '1' 断开0，高驱动1
    NRF_GPIO_PIN_S0D1 = GPIO_PIN_CNF_DRIVE_S0D1, ///< !< Standard '0'. disconnect '1' 标准0，断开1
    NRF_GPIO_PIN_H0D1 = GPIO_PIN_CNF_DRIVE_H0D1, ///< !< High drive '0', disconnect '1' 高驱动0，断开1

} nrf_gpio_pin_drive_t;

/**
 * @brief Enumerator used for selecting the pin to sense high or low level on the pin input. 用于配制管脚输入模式下输入高低水平测试的枚举
 */
typedef enum
{
    NRF_GPIO_PIN_NOSENSE    = GPIO_PIN_CNF_SENSE_Disabled,              ///<  Pin sense level disabled. 管脚输入等级检测禁用
    NRF_GPIO_PIN_SENSE_LOW  = GPIO_PIN_CNF_SENSE_Low,                   ///<  Pin sense low level. 管脚检测低等级
    NRF_GPIO_PIN_SENSE_HIGH = GPIO_PIN_CNF_SENSE_High,                  ///<  Pin sense high level. 管脚检测高等级
} nrf_gpio_pin_sense_t;


/**
 * @brief Function for configuring the GPIO pin range as outputs with normal drive strength. 
 *	摘要：	将范围内GPIO配制为有正常驱动强度的输出
 *        This function can be used to configure pin range as simple output with gate driving GPIO_PIN_CNF_DRIVE_S0S1 (normal cases).
 *				这个函数可以将pin配制为带有门驱动的简单输出 GPIO_PIN_CNF_DRIVE_S0S1(正常情况下)
 * @param pin_range_start specifies the start number (inclusive) in the range of pin numbers to be configured (allowed values 0-30)
 *				参数：  pin_range_start  指定开始数 (包括自身) 配制值要在pin 数量范围内 (0-30)
 *			
 * @param pin_range_end specifies the end number (inclusive) in the range of pin numbers to be configured (allowed values 0-30)
 *        参数： pin_rang_end 指定结束数(包括自身) 配制值要在pin 数量范围内(0-30)
 *
 * @note For configuring only one pin as output use @ref nrf_gpio_cfg_output
 *       Sense capability on the pin is disabled, and input is disconnected from the buffer as the pins are configured as output.
 *       如果只需要配制一个IO口作为输出，可以调用 nrf_gpio_cfg_output 这个函数
 *			 当端口被配置为输出时，端口的输入感知能力被禁用，与缓存区断开连接
 */
__STATIC_INLINE void nrf_gpio_range_cfg_output(uint32_t pin_range_start, uint32_t pin_range_end);

/**
 * @brief Function for configuring the GPIO pin range as inputs with given initial value set, hiding inner details.
 *			摘要： 将范围内GPIO pin 配制为有预设值的输入端口，隐藏细节
 *        This function can be used to configure pin range as simple input.
 *				这个函数可以将函数配制为简单的输入。
 * @param pin_range_start specifies the start number (inclusive) in the range of pin numbers to be configured (allowed values 0-30)
 *	参数： pin_range_start 制定开始数(包括自身)，配制值要在pin数量范围内(0-30)
 * @param pin_range_end specifies the end number (inclusive) in the range of pin numbers to be configured (allowed values 0-30)
 *	参数： pin_range_end 制定结束数(包括自身)，配置值要在pin 数量范围内(0-30)
 * @param pull_config State of the pin range pull resistor (no pull, pulled down or pulled high)
 *	参数： pull_config 配制范围内端口连接的拉电阻(无连接，下拉或上拉)
 * @note  For configuring only one pin as input use @ref nrf_gpio_cfg_input
 *				只配置一个端口为输入时，可以调用 nrf_gpio_cfg_input 这个函数
 *        Sense capability on the pin is disabled, and input is connected to buffer so that the GPIO->IN register is readable
 *				端口上的感觉能力被禁用，输入被连到缓存区， GPIO->IN 寄存器可读
 */
__STATIC_INLINE void nrf_gpio_range_cfg_input(uint32_t pin_range_start, uint32_t pin_range_end, nrf_gpio_pin_pull_t pull_config);

/**
 * @brief Pin configuration function  摘要：引脚配制功能
 *
 * The main pin configuration function.  主要引脚配制功能
 * This function allows to set any aspect in PIN_CNF register. 这个函数允许配制整个 PIN_CNF寄存器
 * @param pin_number Specifies the pin number (allowed values 0-31).  
 参数： pin_number 制定引脚号 (范围0-31)
 
 * @param dir   Pin direction
	参数： dir 端口方向
 
 * @param input Connect or disconnect input buffer
	参数：连接或断开输入缓存器 
 
 * @param pull  Pull configuration
 参数： pull  上下拉电阻配制
 
 * @param drive Drive configuration
 参数： drive 驱动配制
 
 * @param sense Pin sensing mechanism
 参数： sense  引脚感知配制 (输入检测机制)
 
 */
__STATIC_INLINE void nrf_gpio_cfg(
        uint32_t             pin_number,
        nrf_gpio_pin_dir_t   dir,
        nrf_gpio_pin_input_t input,
        nrf_gpio_pin_pull_t  pull,
        nrf_gpio_pin_drive_t drive,
        nrf_gpio_pin_sense_t sense);

/**
 * @brief Function for configuring the given GPIO pin number as output with given initial value set, hiding inner details.
摘要： 将指定GPIO口配制成具有简单输出的IO口 隐藏细节

 *        This function can be used to configure pin range as simple input with gate driving GPIO_PIN_CNF_DRIVE_S0S1 (normal cases).
 *				这个函数可以将制定pin 配制成具有简单门驱动输出的 IO口  GPIO_PIN_CNF_DRIVE_SOS1 (正常情况下)
 * @param pin_number specifies the pin number (allowed values 0-31)
 *		参数： pin_number 制定引脚号 (在0-31范围内)
 * @note  Sense capability on the pin is disabled, and input is disconnected from the buffer as the pins are configured as output.
 *   记录： 引脚检测能力被禁用，当引脚被配制成输出时，输入缓存区被断开连接
 */
__STATIC_INLINE void nrf_gpio_cfg_output(uint32_t pin_number);

/**
 * @brief Function for configuring the given GPIO pin number as input with given initial value set, hiding inner details.
	摘要 将制定GPIO口配制成具有指定初始值的输入口
 *        This function can be used to configure pin range as simple input with gate driving GPIO_PIN_CNF_DRIVE_S0S1 (normal cases).
 *				这个函数可以将引脚配制成简单的输入 GPIO_PIN_CNF_DRIVE_S0S1 (正常情况下)
 * @param pin_number Specifies the pin number (allowed values 0-30).
 参数 pin_number 指定引脚  (在0-31范围内)
 
 * @param pull_config State of the pin range pull resistor (no pull, pulled down or pulled high).
 参数  pull_config  配制拉电阻 (无上下拉，下拉或上拉)。
 
 *
 * @note  Sense capability on the pin is disabled, and input is connected to buffer so that the GPIO->IN register is readable
	记录  输入检测能力被禁用，输入被连接到缓存区，这样 GPIO->IN 寄存器是可读的
 */
__STATIC_INLINE void nrf_gpio_cfg_input(uint32_t pin_number, nrf_gpio_pin_pull_t pull_config);

/**
 * @brief Function for reseting pin configuration to its default state.
 摘要： 将引脚配制成默认状态的函数
 *
 * @param pin_number Specifies the pin number (allowed values 0-31).
 参数： pin_number 需要配置的引脚号。 (范围 0-31)
 */
__STATIC_INLINE void nrf_gpio_cfg_default(uint32_t pin_number);

/**
 * @brief Function for configuring the given GPIO pin number as a watcher. Only input is connected.
 摘要： 配制指定GPIO口作为检测端口 。只有输入连接。
 *
 * @param pin_number Specifies the pin number (allowed values 0-31).
 参数： pin_number 指定端口号(范围 0-31)
 *
 */
__STATIC_INLINE void nrf_gpio_cfg_watcher(uint32_t pin_number);

/**
 * @brief Function for disconnecting input for the given GPIO.
	摘要： 为指定GPIO口断开输入的函数
 *
 * @param pin_number Specifies the pin number (allowed values 0-31).
 参数： pin_number 指定端口号(范围 0-31)
 *
 */
__STATIC_INLINE void nrf_gpio_input_disconnect(uint32_t pin_number);

/**
 * @brief Function for configuring the given GPIO pin number as input with given initial value set, hiding inner details.
 摘要：  将GPIO口配制成具有指定初始值输入状态的函数，隐藏内部细节
 
 *        This function can be used to configure pin range as simple input with gate driving GPIO_PIN_CNF_DRIVE_S0S1 (normal cases).
				该函数可用于将范围内的pin配制为具有简单输入和门驱动的IO口 GPIO_PIN_CNF_DRIVE_SOS1 (正常情况下)
 
 *        Sense capability on the pin is configurable, and input is connected to buffer so that the GPIO->IN register is readable.
		引脚上的检测功能是可以配置的，输入被连接到缓存区 这样 GPIO->IN 寄存器就可读了
 
 *
 * @param pin_number   Specifies the pin number (allowed values 0-30).
 参数： pin_number 指定引脚号 (范围 0-30)。
 
 * @param pull_config  State of the pin pull resistor (no pull, pulled down or pulled high).
 参数： pin_config  配制上下拉电阻 (无上下拉电阻，下拉或上拉电阻)。
 
 * @param sense_config Sense level of the pin (no sense, sense low or sense high).
 参数： sense_config 配制引脚检测等级 (无等级，低等级和高等级)
 */
__STATIC_INLINE void nrf_gpio_cfg_sense_input(uint32_t pin_number, nrf_gpio_pin_pull_t pull_config, nrf_gpio_pin_sense_t sense_config);

/**
 * @brief Function for configuring sense level for the given GPIO.
 摘要： 给指定IO口配制检测等级的函数
 
 *
 * @param pin_number   Specifies the pin number of gpio pin numbers to be configured (allowed values 0-30).
 参数： pin_number  指定引脚号 (范围 0-30)
 
 * @param sense_config Sense configuration
 参数： sense_config 检测配制
 *
 */
__STATIC_INLINE void nrf_gpio_cfg_sense_set(uint32_t pin_number, nrf_gpio_pin_sense_t sense_config);

/**
 * @brief Function for setting the direction for a GPIO pin.
 摘要： 用于配制GPIO方向的函数
 
 *
 * @param pin_number specifies the pin number [0:31] for which to
 参数： pin_number 需要配置的引脚号 [0:31] 范围内的方向的设置
 
 * set the direction.
 
 
 *
 * @param direction specifies the direction
 参数： direction 指定方向
 
 */
__STATIC_INLINE void nrf_gpio_pin_dir_set(uint32_t pin_number, nrf_gpio_pin_dir_t direction);

/**
 * @brief Function for setting a GPIO pin.
 摘要 置位GPIO pin值的函数
 *
 * Note that the pin must be configured as an output for this
 * function to have any effect.
 *对应引脚必须被配制为输出才有效果。
 
 * @param pin_number specifies the pin number [0:31] to
 * set.
 参数； pin_numbe 要配置的引脚号 [0；31] 置位
 */
__STATIC_INLINE void nrf_gpio_pin_set(uint32_t pin_number);

/**
 * @brief Function for setting GPIO pins.
 摘要： 用于置位多个GPIO pin值的函数
 *
 * Note that pins must be configured as an output for this
 * function to have any effect.
 对应引脚必须被配制成输出才有效果。
 
 *
 * @param pin_mask Specifies the pins to set.
 参数： pin_mask 需要配制的多个引脚号
 * set.
 */
__STATIC_INLINE void nrf_gpio_pins_set(uint32_t pin_mask);

/**
 * @brief Function for clearing a GPIO pin.
 *摘要： 清除GPIO口的函数
 
 * Note that the pin must be configured as an output for this
 * function to have any effect.
 *对应引脚必须被配制成输出才有效果
 
 
 * @param pin_number specifies the pin number [0:31] to
 * clear.
 参数： pin_number 指定引脚号[0-31]进行清除
 
 */
__STATIC_INLINE void nrf_gpio_pin_clear(uint32_t pin_number);

/**
 * @brief Function for clearing GPIO pins.
 摘要： 用于清除多个GPIO口
 
 *
 * Note that pins must be configured as an output for this
 * function to have any effect.
 对应引脚必须被配置成输出才有效
 
 *
 * @param pin_mask Specifies the pins to clear.
 * set.
 参数： pin_mask 需要清除的引脚
 
 */
__STATIC_INLINE void nrf_gpio_pins_clear(uint32_t pin_mask);

/**
 * @brief Function for toggling a GPIO pin.
 摘要： 用于切换GPIO的函数
 *
 * Note that the pin must be configured as an output for this
 * function to have any effect.
 *管脚必须被配制成输出 输出功能才会有效果
 
 * @param pin_number specifies the pin number [0:31] to
 * toggle.
 参数： pin_number 指定引脚号 0：31 去切换 （翻转）
 
 */
__STATIC_INLINE void nrf_gpio_pin_toggle(uint32_t pin_number);

/**
 * @brief Function for writing a value to a GPIO pin.
 摘要： 把值写入GPIO的函数
 *
 * Note that the pin must be configured as an output for this
 * function to have any effect.
 只有在引脚被配制为输出时才有效
 
 *
 * @param pin_number specifies the pin number [0:31] to
 * write.
 *参数： pin_number 对目标引脚号 范围[0:31] 写入
 
 * @param value specifies the value to be written to the pin.
 参数： value 指定要写入的值
 
 * @arg 0 clears the pin
  输入变量0 清除引脚
 
 * @arg >=1 sets the pin.
 输入变量1 置位引脚
 */
__STATIC_INLINE void nrf_gpio_pin_write(uint32_t pin_number, uint32_t value);

/**
 * @brief Function for reading the input level of a GPIO pin.
 *摘要：用于读取GPIO引脚电平的函数
 
 * Note that the pin must have input connected for the value
 * returned from this function to be valid.
 只有当引脚有输入值时 这个函数返回的数据才有效
 
 *
 * @param pin_number specifies the pin number [0:31] to
 * read.
 *参数： pin_number 指定要读取的引脚号 [0:31]去读取
 
 * @return  //返回数据
 * @retval 0 if the pin input level is low.
 当返回0时，输入的是低电平
 * @retval 1 if the pin input level is high.
 当返回1时，输入的事高电平
 * @retval > 1 should never occur.
 不应该产生大于1的返回值
 */
__STATIC_INLINE uint32_t nrf_gpio_pin_read(uint32_t pin_number);

/**
 * @brief Function for reading the input level of all GPIO pins.
 摘要：用于读取所有GPIO口的电平
 *
 * Note that the pin must have input connected for the value
 * returned from this function to be valid.
 引脚有输入电平时，读取的数据才有效
 *
 * @retval Status of input of all pins
 返回所有引脚的电平
 */
__STATIC_INLINE uint32_t nrf_gpio_pins_read(void);

/**
 * @brief Function for reading the sense configuration of a GPIO pin.
 *摘要： 用于读取GPIO口电平检测配制的函数
 
 * @param pin_number specifies the pin number [0:31] to
 * read.
 参数： pin_number 配制引脚号[0:31] 去读取
 *
 * @retval Sense configuration
 返回检测配制
 */
__STATIC_INLINE nrf_gpio_pin_sense_t nrf_gpio_pin_sense_get(uint32_t pin_number);

/**
 * @brief Generic function for writing a single byte of a 32 bit word at a given
 * address.
 
 * This function should not be called from outside the nrf_gpio
 * abstraction layer.  这个函数不应该在nrf_gpio外调用
 *
 * @param word_address is the address of the word to be written.
 *
 * @param byte_no is the word byte number (0-3) to be written.
 *
 * @param value is the value to be written to byte "byte_no" of word
 * at address "word_address"
 */
__STATIC_INLINE void nrf_gpio_word_byte_write(volatile uint32_t * word_address, uint8_t byte_no, uint8_t value);

/**
 * @brief Generic function for reading a single byte of a 32 bit word at a given
 * address.
 *
 * This function should not be called from outside the nrf_gpio
 * abstraction layer. 这个函数不应该在nrf_gpio外调用
 *
 * @param word_address is the address of the word to be read.
 *
 * @param byte_no is the byte number (0-3) of the word to be read.
 *
 * @return byte "byte_no" of word at address "word_address".
 */
__STATIC_INLINE uint8_t nrf_gpio_word_byte_read(const volatile uint32_t* word_address, uint8_t byte_no);

/**
 * @brief Function for setting the direction of a port.
 *摘要： 用于设置端口方向的函数
 
 * @param port is the port for which to set the direction.
 *参数：port 需要配置方向的端口
 
 * @param dir direction to be set for this port.
 参数： dir 为这个端口设置的方向
 */
__STATIC_INLINE void nrf_gpio_port_dir_set(nrf_gpio_port_select_t port, nrf_gpio_port_dir_t dir);

/**
 * @brief Function for reading a GPIO port.
 摘要： 读取GPIO端口的函数
 *
 * @param port is the port to read.
 参数：port 需要读取的端口 
 *
 * @return the input value on this port.
 返回： 输入数据在这个端口
 */
__STATIC_INLINE uint8_t nrf_gpio_port_read(nrf_gpio_port_select_t port);

/**
 * @brief Function for writing to a GPIO port.
 摘要： 用于写入GPIO端口的函数
 *
 * @param port is the port to write.
 参数： port  要写入的端口
 *
 * @param value is the value to write to this port.
 参数： value 要写入端口的值
 *
 * @sa nrf_gpio_port_dir_set()
 */
__STATIC_INLINE void nrf_gpio_port_write(nrf_gpio_port_select_t port, uint8_t value);

/**
 * @brief Function for setting individual pins on GPIO port.
 *摘要：设置GPIO上单个端口功能的函数
 * @param port is the port for which to set the pins.
 参数: port 设置的目标端口
 *
 * @param set_mask is a mask specifying which pins to set. A bit
 * set to 1 indicates that the corresponding port pin shall be
 * set.
 *参数： set_mask 是一个掩码，指定要设置的引脚。一字节被设置为1表明对应的引脚被设置
 
 * @sa nrf_gpio_port_dir_set()
 声明： nrf_gpio_port_dir_set()
 */
__STATIC_INLINE void nrf_gpio_port_set(nrf_gpio_port_select_t port, uint8_t set_mask);

/**
 * @brief Function for clearing individual pins on GPIO port.
 *摘要：清除GPIO口上单个端口功能的函数
 * @param port is the port for which to clear the pins.
 *参数： port 需要清除功能的引脚所在的端口
 
 * @param clr_mask is a mask specifying which pins to clear. A bit
 * set to 1 indicates that the corresponding port pin shall be
 * cleared.
 参数： clr_mask 时一个掩码，指定要清除功能的引脚，一字节被设置为1表明对应的引脚被设置
 *
 * @sa nrf_gpio_port_dir_set()
 */
__STATIC_INLINE void nrf_gpio_port_clear(nrf_gpio_port_select_t port, uint8_t clr_mask);

#ifndef SUPPRESS_INLINE_IMPLEMENTATION
__STATIC_INLINE void nrf_gpio_range_cfg_output(uint32_t pin_range_start, uint32_t pin_range_end)
{
    /*lint -e{845} // A zero has been given as right argument to operator '|'" */
    for (; pin_range_start <= pin_range_end; pin_range_start++)
    {
        nrf_gpio_cfg_output(pin_range_start);
    }
}

__STATIC_INLINE void nrf_gpio_range_cfg_input(uint32_t pin_range_start, uint32_t pin_range_end, nrf_gpio_pin_pull_t pull_config)
{
    /*lint -e{845} // A zero has been given as right argument to operator '|'" */
    for (; pin_range_start <= pin_range_end; pin_range_start++)
    {
        nrf_gpio_cfg_input(pin_range_start, pull_config);
    }
}

__STATIC_INLINE void nrf_gpio_cfg(
        uint32_t             pin_number,
        nrf_gpio_pin_dir_t   dir,
        nrf_gpio_pin_input_t input,
        nrf_gpio_pin_pull_t  pull,
        nrf_gpio_pin_drive_t drive,
        nrf_gpio_pin_sense_t sense)
{
    NRF_GPIO->PIN_CNF[pin_number] = ((uint32_t)dir   << GPIO_PIN_CNF_DIR_Pos)
                                  | ((uint32_t)input << GPIO_PIN_CNF_INPUT_Pos)
                                  | ((uint32_t)pull  << GPIO_PIN_CNF_PULL_Pos)
                                  | ((uint32_t)drive << GPIO_PIN_CNF_DRIVE_Pos)
                                  | ((uint32_t)sense << GPIO_PIN_CNF_SENSE_Pos);
}

__STATIC_INLINE void nrf_gpio_cfg_output(uint32_t pin_number)
{
    nrf_gpio_cfg(
            pin_number,
            NRF_GPIO_PIN_DIR_OUTPUT,
            NRF_GPIO_PIN_INPUT_DISCONNECT,
            NRF_GPIO_PIN_NOPULL,
            NRF_GPIO_PIN_S0S1,
            NRF_GPIO_PIN_NOSENSE);
}

__STATIC_INLINE void nrf_gpio_cfg_input(uint32_t pin_number, nrf_gpio_pin_pull_t pull_config)
{
    nrf_gpio_cfg(
            pin_number,
            NRF_GPIO_PIN_DIR_INPUT,
            NRF_GPIO_PIN_INPUT_CONNECT,
            pull_config,
            NRF_GPIO_PIN_S0S1,
            NRF_GPIO_PIN_NOSENSE);
}

__STATIC_INLINE void nrf_gpio_cfg_default(uint32_t pin_number)
{
    nrf_gpio_cfg(
            pin_number,
            NRF_GPIO_PIN_DIR_INPUT,
            NRF_GPIO_PIN_INPUT_DISCONNECT,
            NRF_GPIO_PIN_NOPULL,
            NRF_GPIO_PIN_S0S1,
            NRF_GPIO_PIN_NOSENSE);
}

__STATIC_INLINE void nrf_gpio_cfg_watcher(uint32_t pin_number)
{
    /*lint -e{845} // A zero has been given as right argument to operator '|'" */
    uint32_t cnf = NRF_GPIO->PIN_CNF[pin_number] & ~GPIO_PIN_CNF_INPUT_Msk;
    NRF_GPIO->PIN_CNF[pin_number] = cnf | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos);
}

__STATIC_INLINE void nrf_gpio_input_disconnect(uint32_t pin_number)
{
    /*lint -e{845} // A zero has been given as right argument to operator '|'" */
    uint32_t cnf = NRF_GPIO->PIN_CNF[pin_number] & ~GPIO_PIN_CNF_INPUT_Msk;
    NRF_GPIO->PIN_CNF[pin_number] = cnf | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
}

__STATIC_INLINE void nrf_gpio_cfg_sense_input(uint32_t pin_number, nrf_gpio_pin_pull_t pull_config, nrf_gpio_pin_sense_t sense_config)
{
    nrf_gpio_cfg(
            pin_number,
            NRF_GPIO_PIN_DIR_INPUT,
            NRF_GPIO_PIN_INPUT_CONNECT,
            pull_config,
            NRF_GPIO_PIN_S0S1,
            sense_config);
}

__STATIC_INLINE void nrf_gpio_cfg_sense_set(uint32_t pin_number, nrf_gpio_pin_sense_t sense_config)
{
    /*lint -e{845} // A zero has been given as right argument to operator '|'" */
    //uint32_t cnf = NRF_GPIO->PIN_CNF[pin_number] & ~GPIO_PIN_CNF_SENSE_Msk;
    NRF_GPIO->PIN_CNF[pin_number] &= ~GPIO_PIN_CNF_SENSE_Msk;
    NRF_GPIO->PIN_CNF[pin_number] |= (sense_config << GPIO_PIN_CNF_SENSE_Pos);
}

__STATIC_INLINE void nrf_gpio_pin_dir_set(uint32_t pin_number, nrf_gpio_pin_dir_t direction)
{
    if(direction == NRF_GPIO_PIN_DIR_INPUT)
    {
        nrf_gpio_cfg(
                pin_number,
                NRF_GPIO_PIN_DIR_INPUT,
                NRF_GPIO_PIN_INPUT_CONNECT,
                NRF_GPIO_PIN_NOPULL,
                NRF_GPIO_PIN_S0S1,
                NRF_GPIO_PIN_NOSENSE);
    }
    else
    {
        NRF_GPIO->DIRSET = (1UL << pin_number);
    }
}

__STATIC_INLINE void nrf_gpio_pin_set(uint32_t pin_number)
{
    NRF_GPIO->OUTSET = (1UL << pin_number);
}

__STATIC_INLINE void nrf_gpio_pins_set(uint32_t pin_mask)
{
    NRF_GPIO->OUTSET = pin_mask;
}

__STATIC_INLINE void nrf_gpio_pin_clear(uint32_t pin_number)
{
    NRF_GPIO->OUTCLR = (1UL << pin_number);
}

__STATIC_INLINE void nrf_gpio_pins_clear(uint32_t pin_mask)
{
    NRF_GPIO->OUTCLR = pin_mask;
}

__STATIC_INLINE void nrf_gpio_pin_toggle(uint32_t pin_number)
{
    const uint32_t pin_bit   = 1UL << pin_number;
    const uint32_t pin_state = ((NRF_GPIO->OUT >> pin_number) & 1UL);

    if (pin_state == 0)
    {
        // Current state low, set high.
        NRF_GPIO->OUTSET = pin_bit;
    }
    else
    {
        // Current state high, set low.
        NRF_GPIO->OUTCLR = pin_bit;
    }
}

__STATIC_INLINE void nrf_gpio_pin_write(uint32_t pin_number, uint32_t value)
{
    if (value == 0)
    {
        nrf_gpio_pin_clear(pin_number);
    }
    else
    {
        nrf_gpio_pin_set(pin_number);
    }
}

__STATIC_INLINE uint32_t nrf_gpio_pin_read(uint32_t pin_number)
{
    return  ((NRF_GPIO->IN >> pin_number) & 1UL);
}

__STATIC_INLINE uint32_t nrf_gpio_pins_read(void)
{
    return NRF_GPIO->IN;
}

__STATIC_INLINE nrf_gpio_pin_sense_t nrf_gpio_pin_sense_get(uint32_t pin_number)
{
    return (nrf_gpio_pin_sense_t)((NRF_GPIO->PIN_CNF[pin_number] & GPIO_PIN_CNF_SENSE_Msk) >> GPIO_PIN_CNF_SENSE_Pos);
}

__STATIC_INLINE void nrf_gpio_word_byte_write(volatile uint32_t * word_address, uint8_t byte_no, uint8_t value)
{
    *((volatile uint8_t*)(word_address) + byte_no) = value;
}

__STATIC_INLINE uint8_t nrf_gpio_word_byte_read(const volatile uint32_t* word_address, uint8_t byte_no)
{
    return (*((const volatile uint8_t*)(word_address) + byte_no));
}

__STATIC_INLINE void nrf_gpio_port_dir_set(nrf_gpio_port_select_t port, nrf_gpio_port_dir_t dir)
{
    if (dir == NRF_GPIO_PORT_DIR_OUTPUT)
    {
        nrf_gpio_word_byte_write(&NRF_GPIO->DIRSET, port, 0xFF);
    }
    else
    {
        nrf_gpio_range_cfg_input(port*8, (port+1)*8-1, NRF_GPIO_PIN_NOPULL);
    }
}

__STATIC_INLINE uint8_t nrf_gpio_port_read(nrf_gpio_port_select_t port)
{
    return nrf_gpio_word_byte_read(&NRF_GPIO->IN, port);
}

__STATIC_INLINE void nrf_gpio_port_write(nrf_gpio_port_select_t port, uint8_t value)
{
    nrf_gpio_word_byte_write(&NRF_GPIO->OUT, port, value);
}

__STATIC_INLINE void nrf_gpio_port_set(nrf_gpio_port_select_t port, uint8_t set_mask)
{
    nrf_gpio_word_byte_write(&NRF_GPIO->OUTSET, port, set_mask);
}

__STATIC_INLINE void nrf_gpio_port_clear(nrf_gpio_port_select_t port, uint8_t clr_mask)
{
    nrf_gpio_word_byte_write(&NRF_GPIO->OUTCLR, port, clr_mask);
}
#endif //SUPPRESS_INLINE_IMPLEMENTATION
/** @} */

#endif
