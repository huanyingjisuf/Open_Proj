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
 * @brief Enumerator used for selecting between port 0 - 3. ������ѡ��0-3��˿�
 */
typedef enum
{
    NRF_GPIO_PORT_SELECT_PORT0 = 0,           ///<  Port 0 (GPIO pin 0-7)
    NRF_GPIO_PORT_SELECT_PORT1,               ///<  Port 1 (GPIO pin 8-15)
    NRF_GPIO_PORT_SELECT_PORT2,               ///<  Port 2 (GPIO pin 16-23)
    NRF_GPIO_PORT_SELECT_PORT3,               ///<  Port 3 (GPIO pin 24-31)
} nrf_gpio_port_select_t;

/**
 * @brief Enumerator used for setting the direction of a GPIO port. GPIO���ö˿ڷ���(����˿�)
 */
typedef enum
{
    NRF_GPIO_PORT_DIR_OUTPUT,       ///<  Output
    NRF_GPIO_PORT_DIR_INPUT         ///<  Input
} nrf_gpio_port_dir_t;

/**
 * @brief Pin direction definitions. ���ŷ�����(��������)
 */
typedef enum
{
    NRF_GPIO_PIN_DIR_INPUT  = GPIO_PIN_CNF_DIR_Input,   ///< Input ����
    NRF_GPIO_PIN_DIR_OUTPUT = GPIO_PIN_CNF_DIR_Output   ///< Output  ���
} nrf_gpio_pin_dir_t;

/**
 * @brief Connection of input buffer ���뻺����
 */
typedef enum
{
    NRF_GPIO_PIN_INPUT_CONNECT    = GPIO_PIN_CNF_INPUT_Connect,   ///< Connect input buffer �������뻺����
    NRF_GPIO_PIN_INPUT_DISCONNECT = GPIO_PIN_CNF_INPUT_Disconnect ///< Disconnect input buffer  �Ͽ����뻺����
} nrf_gpio_pin_input_t;

/**
 * @brief Enumerator used for selecting the pin to be pulled down or up at the time of pin configuration  ��������IO���ڲ����������������ö��
 */
typedef enum
{
    NRF_GPIO_PIN_NOPULL   = GPIO_PIN_CNF_PULL_Disabled,                 ///<  Pin pullup resistor disabled ��������������
    NRF_GPIO_PIN_PULLDOWN = GPIO_PIN_CNF_PULL_Pulldown,                 ///<  Pin pulldown resistor enabled ��������������
    NRF_GPIO_PIN_PULLUP   = GPIO_PIN_CNF_PULL_Pullup,                   ///<  Pin pullup resistor enabled  ��������������
} nrf_gpio_pin_pull_t; 

/**
 * @brief Enumerator used for selecting output drive mode ����ѡ��ܽ��������ģʽ��ö��
 */
typedef enum
{
    NRF_GPIO_PIN_S0S1 = GPIO_PIN_CNF_DRIVE_S0S1, ///< !< Standard '0', standard '1' ��׼��0����׼��1.
    NRF_GPIO_PIN_H0S1 = GPIO_PIN_CNF_DRIVE_H0S1, ///< !< High drive '0', standard '1' ������0����׼1
    NRF_GPIO_PIN_S0H1 = GPIO_PIN_CNF_DRIVE_S0H1, ///< !< Standard '0', high drive '1'  ��׼0��������1
    NRF_GPIO_PIN_H0H1 = GPIO_PIN_CNF_DRIVE_H0H1, ///< !< High drive '0', high 'drive '1'' ������0��������1
    NRF_GPIO_PIN_D0S1 = GPIO_PIN_CNF_DRIVE_D0S1, ///< !< Disconnect '0' standard '1'  �Ͽ�0����׼1
    NRF_GPIO_PIN_D0H1 = GPIO_PIN_CNF_DRIVE_D0H1, ///< !< Disconnect '0', high drive '1' �Ͽ�0��������1
    NRF_GPIO_PIN_S0D1 = GPIO_PIN_CNF_DRIVE_S0D1, ///< !< Standard '0'. disconnect '1' ��׼0���Ͽ�1
    NRF_GPIO_PIN_H0D1 = GPIO_PIN_CNF_DRIVE_H0D1, ///< !< High drive '0', disconnect '1' ������0���Ͽ�1

} nrf_gpio_pin_drive_t;

/**
 * @brief Enumerator used for selecting the pin to sense high or low level on the pin input. �������ƹܽ�����ģʽ������ߵ�ˮƽ���Ե�ö��
 */
typedef enum
{
    NRF_GPIO_PIN_NOSENSE    = GPIO_PIN_CNF_SENSE_Disabled,              ///<  Pin sense level disabled. �ܽ�����ȼ�������
    NRF_GPIO_PIN_SENSE_LOW  = GPIO_PIN_CNF_SENSE_Low,                   ///<  Pin sense low level. �ܽż��͵ȼ�
    NRF_GPIO_PIN_SENSE_HIGH = GPIO_PIN_CNF_SENSE_High,                  ///<  Pin sense high level. �ܽż��ߵȼ�
} nrf_gpio_pin_sense_t;


/**
 * @brief Function for configuring the GPIO pin range as outputs with normal drive strength. 
 *	ժҪ��	����Χ��GPIO����Ϊ����������ǿ�ȵ����
 *        This function can be used to configure pin range as simple output with gate driving GPIO_PIN_CNF_DRIVE_S0S1 (normal cases).
 *				����������Խ�pin����Ϊ�����������ļ���� GPIO_PIN_CNF_DRIVE_S0S1(���������)
 * @param pin_range_start specifies the start number (inclusive) in the range of pin numbers to be configured (allowed values 0-30)
 *				������  pin_range_start  ָ����ʼ�� (��������) ����ֵҪ��pin ������Χ�� (0-30)
 *			
 * @param pin_range_end specifies the end number (inclusive) in the range of pin numbers to be configured (allowed values 0-30)
 *        ������ pin_rang_end ָ��������(��������) ����ֵҪ��pin ������Χ��(0-30)
 *
 * @note For configuring only one pin as output use @ref nrf_gpio_cfg_output
 *       Sense capability on the pin is disabled, and input is disconnected from the buffer as the pins are configured as output.
 *       ���ֻ��Ҫ����һ��IO����Ϊ��������Ե��� nrf_gpio_cfg_output �������
 *			 ���˿ڱ�����Ϊ���ʱ���˿ڵ������֪���������ã��뻺�����Ͽ�����
 */
__STATIC_INLINE void nrf_gpio_range_cfg_output(uint32_t pin_range_start, uint32_t pin_range_end);

/**
 * @brief Function for configuring the GPIO pin range as inputs with given initial value set, hiding inner details.
 *			ժҪ�� ����Χ��GPIO pin ����Ϊ��Ԥ��ֵ������˿ڣ�����ϸ��
 *        This function can be used to configure pin range as simple input.
 *				����������Խ���������Ϊ�򵥵����롣
 * @param pin_range_start specifies the start number (inclusive) in the range of pin numbers to be configured (allowed values 0-30)
 *	������ pin_range_start �ƶ���ʼ��(��������)������ֵҪ��pin������Χ��(0-30)
 * @param pin_range_end specifies the end number (inclusive) in the range of pin numbers to be configured (allowed values 0-30)
 *	������ pin_range_end �ƶ�������(��������)������ֵҪ��pin ������Χ��(0-30)
 * @param pull_config State of the pin range pull resistor (no pull, pulled down or pulled high)
 *	������ pull_config ���Ʒ�Χ�ڶ˿����ӵ�������(�����ӣ�����������)
 * @note  For configuring only one pin as input use @ref nrf_gpio_cfg_input
 *				ֻ����һ���˿�Ϊ����ʱ�����Ե��� nrf_gpio_cfg_input �������
 *        Sense capability on the pin is disabled, and input is connected to buffer so that the GPIO->IN register is readable
 *				�˿��ϵĸо����������ã����뱻������������ GPIO->IN �Ĵ����ɶ�
 */
__STATIC_INLINE void nrf_gpio_range_cfg_input(uint32_t pin_range_start, uint32_t pin_range_end, nrf_gpio_pin_pull_t pull_config);

/**
 * @brief Pin configuration function  ժҪ���������ƹ���
 *
 * The main pin configuration function.  ��Ҫ�������ƹ���
 * This function allows to set any aspect in PIN_CNF register. ������������������� PIN_CNF�Ĵ���
 * @param pin_number Specifies the pin number (allowed values 0-31).  
 ������ pin_number �ƶ����ź� (��Χ0-31)
 
 * @param dir   Pin direction
	������ dir �˿ڷ���
 
 * @param input Connect or disconnect input buffer
	���������ӻ�Ͽ����뻺���� 
 
 * @param pull  Pull configuration
 ������ pull  ��������������
 
 * @param drive Drive configuration
 ������ drive ��������
 
 * @param sense Pin sensing mechanism
 ������ sense  ���Ÿ�֪���� (���������)
 
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
ժҪ�� ��ָ��GPIO�����Ƴɾ��м������IO�� ����ϸ��

 *        This function can be used to configure pin range as simple input with gate driving GPIO_PIN_CNF_DRIVE_S0S1 (normal cases).
 *				����������Խ��ƶ�pin ���Ƴɾ��м������������ IO��  GPIO_PIN_CNF_DRIVE_SOS1 (���������)
 * @param pin_number specifies the pin number (allowed values 0-31)
 *		������ pin_number �ƶ����ź� (��0-31��Χ��)
 * @note  Sense capability on the pin is disabled, and input is disconnected from the buffer as the pins are configured as output.
 *   ��¼�� ���ż�����������ã������ű����Ƴ����ʱ�����뻺�������Ͽ�����
 */
__STATIC_INLINE void nrf_gpio_cfg_output(uint32_t pin_number);

/**
 * @brief Function for configuring the given GPIO pin number as input with given initial value set, hiding inner details.
	ժҪ ���ƶ�GPIO�����Ƴɾ���ָ����ʼֵ�������
 *        This function can be used to configure pin range as simple input with gate driving GPIO_PIN_CNF_DRIVE_S0S1 (normal cases).
 *				����������Խ��������Ƴɼ򵥵����� GPIO_PIN_CNF_DRIVE_S0S1 (���������)
 * @param pin_number Specifies the pin number (allowed values 0-30).
 ���� pin_number ָ������  (��0-31��Χ��)
 
 * @param pull_config State of the pin range pull resistor (no pull, pulled down or pulled high).
 ����  pull_config  ���������� (��������������������)��
 
 *
 * @note  Sense capability on the pin is disabled, and input is connected to buffer so that the GPIO->IN register is readable
	��¼  ���������������ã����뱻���ӵ������������� GPIO->IN �Ĵ����ǿɶ���
 */
__STATIC_INLINE void nrf_gpio_cfg_input(uint32_t pin_number, nrf_gpio_pin_pull_t pull_config);

/**
 * @brief Function for reseting pin configuration to its default state.
 ժҪ�� ���������Ƴ�Ĭ��״̬�ĺ���
 *
 * @param pin_number Specifies the pin number (allowed values 0-31).
 ������ pin_number ��Ҫ���õ����źš� (��Χ 0-31)
 */
__STATIC_INLINE void nrf_gpio_cfg_default(uint32_t pin_number);

/**
 * @brief Function for configuring the given GPIO pin number as a watcher. Only input is connected.
 ժҪ�� ����ָ��GPIO����Ϊ���˿� ��ֻ���������ӡ�
 *
 * @param pin_number Specifies the pin number (allowed values 0-31).
 ������ pin_number ָ���˿ں�(��Χ 0-31)
 *
 */
__STATIC_INLINE void nrf_gpio_cfg_watcher(uint32_t pin_number);

/**
 * @brief Function for disconnecting input for the given GPIO.
	ժҪ�� Ϊָ��GPIO�ڶϿ�����ĺ���
 *
 * @param pin_number Specifies the pin number (allowed values 0-31).
 ������ pin_number ָ���˿ں�(��Χ 0-31)
 *
 */
__STATIC_INLINE void nrf_gpio_input_disconnect(uint32_t pin_number);

/**
 * @brief Function for configuring the given GPIO pin number as input with given initial value set, hiding inner details.
 ժҪ��  ��GPIO�����Ƴɾ���ָ����ʼֵ����״̬�ĺ����������ڲ�ϸ��
 
 *        This function can be used to configure pin range as simple input with gate driving GPIO_PIN_CNF_DRIVE_S0S1 (normal cases).
				�ú��������ڽ���Χ�ڵ�pin����Ϊ���м��������������IO�� GPIO_PIN_CNF_DRIVE_SOS1 (���������)
 
 *        Sense capability on the pin is configurable, and input is connected to buffer so that the GPIO->IN register is readable.
		�����ϵļ�⹦���ǿ������õģ����뱻���ӵ������� ���� GPIO->IN �Ĵ����Ϳɶ���
 
 *
 * @param pin_number   Specifies the pin number (allowed values 0-30).
 ������ pin_number ָ�����ź� (��Χ 0-30)��
 
 * @param pull_config  State of the pin pull resistor (no pull, pulled down or pulled high).
 ������ pin_config  �������������� (�����������裬��������������)��
 
 * @param sense_config Sense level of the pin (no sense, sense low or sense high).
 ������ sense_config �������ż��ȼ� (�޵ȼ����͵ȼ��͸ߵȼ�)
 */
__STATIC_INLINE void nrf_gpio_cfg_sense_input(uint32_t pin_number, nrf_gpio_pin_pull_t pull_config, nrf_gpio_pin_sense_t sense_config);

/**
 * @brief Function for configuring sense level for the given GPIO.
 ժҪ�� ��ָ��IO�����Ƽ��ȼ��ĺ���
 
 *
 * @param pin_number   Specifies the pin number of gpio pin numbers to be configured (allowed values 0-30).
 ������ pin_number  ָ�����ź� (��Χ 0-30)
 
 * @param sense_config Sense configuration
 ������ sense_config �������
 *
 */
__STATIC_INLINE void nrf_gpio_cfg_sense_set(uint32_t pin_number, nrf_gpio_pin_sense_t sense_config);

/**
 * @brief Function for setting the direction for a GPIO pin.
 ժҪ�� ��������GPIO����ĺ���
 
 *
 * @param pin_number specifies the pin number [0:31] for which to
 ������ pin_number ��Ҫ���õ����ź� [0:31] ��Χ�ڵķ��������
 
 * set the direction.
 
 
 *
 * @param direction specifies the direction
 ������ direction ָ������
 
 */
__STATIC_INLINE void nrf_gpio_pin_dir_set(uint32_t pin_number, nrf_gpio_pin_dir_t direction);

/**
 * @brief Function for setting a GPIO pin.
 ժҪ ��λGPIO pinֵ�ĺ���
 *
 * Note that the pin must be configured as an output for this
 * function to have any effect.
 *��Ӧ���ű��뱻����Ϊ�������Ч����
 
 * @param pin_number specifies the pin number [0:31] to
 * set.
 ������ pin_numbe Ҫ���õ����ź� [0��31] ��λ
 */
__STATIC_INLINE void nrf_gpio_pin_set(uint32_t pin_number);

/**
 * @brief Function for setting GPIO pins.
 ժҪ�� ������λ���GPIO pinֵ�ĺ���
 *
 * Note that pins must be configured as an output for this
 * function to have any effect.
 ��Ӧ���ű��뱻���Ƴ��������Ч����
 
 *
 * @param pin_mask Specifies the pins to set.
 ������ pin_mask ��Ҫ���ƵĶ�����ź�
 * set.
 */
__STATIC_INLINE void nrf_gpio_pins_set(uint32_t pin_mask);

/**
 * @brief Function for clearing a GPIO pin.
 *ժҪ�� ���GPIO�ڵĺ���
 
 * Note that the pin must be configured as an output for this
 * function to have any effect.
 *��Ӧ���ű��뱻���Ƴ��������Ч��
 
 
 * @param pin_number specifies the pin number [0:31] to
 * clear.
 ������ pin_number ָ�����ź�[0-31]�������
 
 */
__STATIC_INLINE void nrf_gpio_pin_clear(uint32_t pin_number);

/**
 * @brief Function for clearing GPIO pins.
 ժҪ�� ����������GPIO��
 
 *
 * Note that pins must be configured as an output for this
 * function to have any effect.
 ��Ӧ���ű��뱻���ó��������Ч
 
 *
 * @param pin_mask Specifies the pins to clear.
 * set.
 ������ pin_mask ��Ҫ���������
 
 */
__STATIC_INLINE void nrf_gpio_pins_clear(uint32_t pin_mask);

/**
 * @brief Function for toggling a GPIO pin.
 ժҪ�� �����л�GPIO�ĺ���
 *
 * Note that the pin must be configured as an output for this
 * function to have any effect.
 *�ܽű��뱻���Ƴ���� ������ܲŻ���Ч��
 
 * @param pin_number specifies the pin number [0:31] to
 * toggle.
 ������ pin_number ָ�����ź� 0��31 ȥ�л� ����ת��
 
 */
__STATIC_INLINE void nrf_gpio_pin_toggle(uint32_t pin_number);

/**
 * @brief Function for writing a value to a GPIO pin.
 ժҪ�� ��ֵд��GPIO�ĺ���
 *
 * Note that the pin must be configured as an output for this
 * function to have any effect.
 ֻ�������ű�����Ϊ���ʱ����Ч
 
 *
 * @param pin_number specifies the pin number [0:31] to
 * write.
 *������ pin_number ��Ŀ�����ź� ��Χ[0:31] д��
 
 * @param value specifies the value to be written to the pin.
 ������ value ָ��Ҫд���ֵ
 
 * @arg 0 clears the pin
  �������0 �������
 
 * @arg >=1 sets the pin.
 �������1 ��λ����
 */
__STATIC_INLINE void nrf_gpio_pin_write(uint32_t pin_number, uint32_t value);

/**
 * @brief Function for reading the input level of a GPIO pin.
 *ժҪ�����ڶ�ȡGPIO���ŵ�ƽ�ĺ���
 
 * Note that the pin must have input connected for the value
 * returned from this function to be valid.
 ֻ�е�����������ֵʱ ����������ص����ݲ���Ч
 
 *
 * @param pin_number specifies the pin number [0:31] to
 * read.
 *������ pin_number ָ��Ҫ��ȡ�����ź� [0:31]ȥ��ȡ
 
 * @return  //��������
 * @retval 0 if the pin input level is low.
 ������0ʱ��������ǵ͵�ƽ
 * @retval 1 if the pin input level is high.
 ������1ʱ��������¸ߵ�ƽ
 * @retval > 1 should never occur.
 ��Ӧ�ò�������1�ķ���ֵ
 */
__STATIC_INLINE uint32_t nrf_gpio_pin_read(uint32_t pin_number);

/**
 * @brief Function for reading the input level of all GPIO pins.
 ժҪ�����ڶ�ȡ����GPIO�ڵĵ�ƽ
 *
 * Note that the pin must have input connected for the value
 * returned from this function to be valid.
 �����������ƽʱ����ȡ�����ݲ���Ч
 *
 * @retval Status of input of all pins
 �����������ŵĵ�ƽ
 */
__STATIC_INLINE uint32_t nrf_gpio_pins_read(void);

/**
 * @brief Function for reading the sense configuration of a GPIO pin.
 *ժҪ�� ���ڶ�ȡGPIO�ڵ�ƽ������Ƶĺ���
 
 * @param pin_number specifies the pin number [0:31] to
 * read.
 ������ pin_number �������ź�[0:31] ȥ��ȡ
 *
 * @retval Sense configuration
 ���ؼ������
 */
__STATIC_INLINE nrf_gpio_pin_sense_t nrf_gpio_pin_sense_get(uint32_t pin_number);

/**
 * @brief Generic function for writing a single byte of a 32 bit word at a given
 * address.
 
 * This function should not be called from outside the nrf_gpio
 * abstraction layer.  ���������Ӧ����nrf_gpio�����
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
 * abstraction layer. ���������Ӧ����nrf_gpio�����
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
 *ժҪ�� �������ö˿ڷ���ĺ���
 
 * @param port is the port for which to set the direction.
 *������port ��Ҫ���÷���Ķ˿�
 
 * @param dir direction to be set for this port.
 ������ dir Ϊ����˿����õķ���
 */
__STATIC_INLINE void nrf_gpio_port_dir_set(nrf_gpio_port_select_t port, nrf_gpio_port_dir_t dir);

/**
 * @brief Function for reading a GPIO port.
 ժҪ�� ��ȡGPIO�˿ڵĺ���
 *
 * @param port is the port to read.
 ������port ��Ҫ��ȡ�Ķ˿� 
 *
 * @return the input value on this port.
 ���أ� ��������������˿�
 */
__STATIC_INLINE uint8_t nrf_gpio_port_read(nrf_gpio_port_select_t port);

/**
 * @brief Function for writing to a GPIO port.
 ժҪ�� ����д��GPIO�˿ڵĺ���
 *
 * @param port is the port to write.
 ������ port  Ҫд��Ķ˿�
 *
 * @param value is the value to write to this port.
 ������ value Ҫд��˿ڵ�ֵ
 *
 * @sa nrf_gpio_port_dir_set()
 */
__STATIC_INLINE void nrf_gpio_port_write(nrf_gpio_port_select_t port, uint8_t value);

/**
 * @brief Function for setting individual pins on GPIO port.
 *ժҪ������GPIO�ϵ����˿ڹ��ܵĺ���
 * @param port is the port for which to set the pins.
 ����: port ���õ�Ŀ��˿�
 *
 * @param set_mask is a mask specifying which pins to set. A bit
 * set to 1 indicates that the corresponding port pin shall be
 * set.
 *������ set_mask ��һ�����룬ָ��Ҫ���õ����š�һ�ֽڱ�����Ϊ1������Ӧ�����ű�����
 
 * @sa nrf_gpio_port_dir_set()
 ������ nrf_gpio_port_dir_set()
 */
__STATIC_INLINE void nrf_gpio_port_set(nrf_gpio_port_select_t port, uint8_t set_mask);

/**
 * @brief Function for clearing individual pins on GPIO port.
 *ժҪ�����GPIO���ϵ����˿ڹ��ܵĺ���
 * @param port is the port for which to clear the pins.
 *������ port ��Ҫ������ܵ��������ڵĶ˿�
 
 * @param clr_mask is a mask specifying which pins to clear. A bit
 * set to 1 indicates that the corresponding port pin shall be
 * cleared.
 ������ clr_mask ʱһ�����룬ָ��Ҫ������ܵ����ţ�һ�ֽڱ�����Ϊ1������Ӧ�����ű�����
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
