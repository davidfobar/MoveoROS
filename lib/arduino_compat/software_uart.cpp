/**
 * @file    software_uart.cpp
 * @brief   Interrupt-driven bit-bang UART implementation for TMC2209
 *
 * Uses TIM14 interrupt for non-blocking TX/RX with state machine
 */

#include "main.h"
#include "software_uart.h"
#include "gpio.h"
#include "tim.h"

// External timer handle for bit timing
extern TIM_HandleTypeDef htim14;

// Map of timer instances to their associated Software_UART instances
// Supports up to 7 timers (one per axis)
static Software_UART* g_timer_instances[7] = {nullptr};

// Helper to get array index for a timer instance
static int _get_timer_index(TIM_TypeDef* instance)
{
  if (instance == TIM1) return 0;
  if (instance == TIM2) return 1;
  if (instance == TIM3) return 2;
  if (instance == TIM4) return 3;
  if (instance == TIM5) return 4;
  if (instance == TIM9) return 5;
  if (instance == TIM14) return 6;
  return -1;
}

/**
 * @brief Constructor - initialize buffers and state
 * @param tx_pin_mask GPIO pin mask (e.g., GPIO_PIN_1 for PE1)
 * @param tx_port GPIO port (e.g., GPIOE)
 * @param htim Timer handle to use for bit timing
 */
Software_UART::Software_UART(uint32_t tx_pin_mask, void* tx_port, struct __TIM_HandleTypeDef* htim)
    : _tx_pin_mask(tx_pin_mask), _tx_port(tx_port), _htim(htim),
      _tx_state(TX_IDLE), _tx_data(0), _tx_bit_index(0),
      _tx_buffer_head(0), _tx_buffer_tail(0),
      _rx_state(RX_IDLE), _rx_data(0), _rx_bit_index(0),
      _rx_buffer_head(0), _rx_buffer_tail(0),
      _rx_sample_counter(0),
      _debug_irq_count(0), _debug_tx_bytes_sent(0)
{
}

/**
 * @brief Initialize interrupt-driven UART with desired baud rate
 * @param baud Baud rate (e.g., 115200) - TIM14 is configured for this
 */
void Software_UART::begin(uint32_t baud)
{
  (void)baud;  // TIM14 already configured with PSC=0, ARR=138 for 115200 baud
  
  // Start in idle state with pin as output HIGH
  _tx_state = TX_IDLE;
  _rx_state = RX_IDLE;
  _set_pin_mode_output();
  HAL_GPIO_WritePin((GPIO_TypeDef*)_tx_port, _tx_pin_mask, GPIO_PIN_SET);
  
  // DON'T start timer yet - only enable when transmitting
  // Timer will be started in write() when data needs to be sent
}

/**
 * @brief Configure pin as output (for TX)
 */
void Software_UART::_set_pin_mode_output()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = _tx_pin_mask;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init((GPIO_TypeDef*)_tx_port, &GPIO_InitStruct);
}

/**
 * @brief Configure pin as input (for RX)
 */
void Software_UART::_set_pin_mode_input()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = _tx_pin_mask;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init((GPIO_TypeDef*)_tx_port, &GPIO_InitStruct);
}

/**
 * @brief Configure pin as input with EXTI interrupt for RX start bit detection
 */
void Software_UART::_set_pin_mode_input_it()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = _tx_pin_mask;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;  // Interrupt on falling edge (start bit)
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init((GPIO_TypeDef*)_tx_port, &GPIO_InitStruct);
}

/**
 * @brief Check if TX buffer is full
 */
bool Software_UART::_tx_buffer_full()
{
  return ((_tx_buffer_head + 1) % TX_BUFFER_SIZE) == _tx_buffer_tail;
}

/**
 * @brief Check if TX buffer is empty
 */
bool Software_UART::_tx_buffer_empty()
{
  return _tx_buffer_head == _tx_buffer_tail;
}

/**
 * @brief Check if RX buffer is full
 */
bool Software_UART::_rx_buffer_full()
{
  return ((_rx_buffer_head + 1) % RX_BUFFER_SIZE) == _rx_buffer_tail;
}

/**
 * @brief Check if RX buffer is empty
 */
bool Software_UART::_rx_buffer_empty()
{
  return _rx_buffer_head == _rx_buffer_tail;
}

/**
 * @brief Start RX operation by switching to input with EXTI
 */
void Software_UART::_start_rx()
{
  _rx_state = RX_IDLE;
  _rx_sample_counter = 0;
  _set_pin_mode_input_it();
  
  TIM_HandleTypeDef* htim = (TIM_HandleTypeDef*)_htim;
  int idx = _get_timer_index(htim->Instance);
  if (idx >= 0)
  {
    g_timer_instances[idx] = this;
    HAL_TIM_Base_Start_IT(htim);  // Start timer for RX sampling
  }
}

/**
 * @brief Write a single byte (non-blocking, queues to TX buffer)
 * @param byte Data byte to send
 * @return Number of bytes written (1 on success, 0 if buffer full)
 */
size_t Software_UART::write(uint8_t byte)
{
  // Wait if buffer is full (should rarely happen with 32-byte buffer)
  while (_tx_buffer_full()) { }
  
  // Add byte to TX buffer
  _tx_buffer[_tx_buffer_head] = byte;
  _tx_buffer_head = (_tx_buffer_head + 1) % TX_BUFFER_SIZE;
  
  // If TX idle, register this instance and start timer
  if (_tx_state == TX_IDLE)
  {
    TIM_HandleTypeDef* htim = (TIM_HandleTypeDef*)_htim;
    int idx = _get_timer_index(htim->Instance);
    if (idx >= 0)
    {
      g_timer_instances[idx] = this;
      _set_pin_mode_output();
      HAL_TIM_Base_Start_IT(htim);  // Start timer interrupt
    }
  }
  
  return 1;
}

/**
 * @brief Write multiple bytes (queues all to TX buffer, then waits for RX)
 * @param buffer Buffer containing data
 * @param length Number of bytes to send
 * @return Number of bytes written
 */
size_t Software_UART::write(const uint8_t* buffer, size_t length)
{
  // Queue all bytes for transmission
  for (size_t i = 0; i < length; i++)
  {
    write(buffer[i]);
  }
  
  // Wait for TX to complete
  while (_tx_state != TX_IDLE || !_tx_buffer_empty()) { }
  
  // Switch to RX mode to receive TMC2209 response
  _start_rx();
  
  // Wait briefly for response (up to ~2ms for 8-byte response at 115200)
  uint32_t start_time = HAL_GetTick();
  while ((HAL_GetTick() - start_time) < 3)
  {
    if (available() > 0)
    {
      break;  // Got response
    }
  }
  
  // Return to idle (output HIGH)
  TIM_HandleTypeDef* htim = (TIM_HandleTypeDef*)_htim;
  HAL_TIM_Base_Stop_IT(htim);  // Stop timer
  int idx = _get_timer_index(htim->Instance);
  if (idx >= 0)
  {
    g_timer_instances[idx] = nullptr;
  }
  _rx_state = RX_IDLE;
  _set_pin_mode_output();
  HAL_GPIO_WritePin((GPIO_TypeDef*)_tx_port, _tx_pin_mask, GPIO_PIN_SET);
  
  return length;
}

/**
 * @brief Read a byte from RX buffer
 * @return Byte from buffer, or -1 if buffer empty
 */
int Software_UART::read()
{
  if (_rx_buffer_head == _rx_buffer_tail)
  {
    return -1; // Buffer empty
  }
  
  uint8_t byte = _rx_buffer[_rx_buffer_tail];
  _rx_buffer_tail = (_rx_buffer_tail + 1) % RX_BUFFER_SIZE;
  return byte;
}

/**
 * @brief Check available data in RX buffer
 * @return Number of bytes available
 */
int Software_UART::available()
{
  return (_rx_buffer_head - _rx_buffer_tail + RX_BUFFER_SIZE) % RX_BUFFER_SIZE;
}

/**
 * @brief Flush TX buffer and clear RX buffer
 */
void Software_UART::flush()
{
  // Clear RX buffer
  _rx_buffer_head = 0;
  _rx_buffer_tail = 0;
  
  // Ensure line goes idle (HIGH) in output mode
  _set_pin_mode_output();
  HAL_GPIO_WritePin((GPIO_TypeDef*)_tx_port, _tx_pin_mask, GPIO_PIN_SET);
}

/**
 * @brief Timer interrupt handler - called every 8.68 µs by TIM14
 *        Handles TX and RX state machines
 */
void Software_UART::handleTimerInterrupt()
{
  _debug_irq_count++;
  
  // TX State Machine
  switch (_tx_state)
  {
    case TX_IDLE:
      // Check if there's data to send
      if (!_tx_buffer_empty())
      {
        // Get next byte from buffer
        _tx_data = _tx_buffer[_tx_buffer_tail];
        _tx_buffer_tail = (_tx_buffer_tail + 1) % TX_BUFFER_SIZE;
        _tx_bit_index = 0;
        _tx_state = TX_START_BIT;
      }
      break;
      
    case TX_START_BIT:
      // Send start bit (LOW)
      HAL_GPIO_WritePin((GPIO_TypeDef*)_tx_port, _tx_pin_mask, GPIO_PIN_RESET);
      _tx_state = TX_DATA_BITS;
      _tx_bit_index = 0;
      break;
      
    case TX_DATA_BITS:
      // Send data bit (LSB first)
      if (_tx_data & (1 << _tx_bit_index))
        HAL_GPIO_WritePin((GPIO_TypeDef*)_tx_port, _tx_pin_mask, GPIO_PIN_SET);
      else
        HAL_GPIO_WritePin((GPIO_TypeDef*)_tx_port, _tx_pin_mask, GPIO_PIN_RESET);
      
      _tx_bit_index++;
      if (_tx_bit_index >= 8)
      {
        _tx_state = TX_STOP_BIT;
      }
      break;
      
    case TX_STOP_BIT:
      // Send stop bit (HIGH)
      HAL_GPIO_WritePin((GPIO_TypeDef*)_tx_port, _tx_pin_mask, GPIO_PIN_SET);
      _tx_state = TX_IDLE;
      _debug_tx_bytes_sent++;
      
      // If no more data to send and not receiving, stop timer
      if (_tx_buffer_empty() && _rx_state == RX_IDLE)
      {
        TIM_HandleTypeDef* htim = (TIM_HandleTypeDef*)_htim;
        HAL_TIM_Base_Stop_IT(htim);
        int idx = _get_timer_index(htim->Instance);
        if (idx >= 0)
        {
          g_timer_instances[idx] = nullptr;
        }
      }
      break;
  }
  
  // RX State Machine
  switch (_rx_state)
  {
    case RX_IDLE:
      // Waiting for EXTI to detect start bit
      break;
      
    case RX_START_BIT:
      // Sample counter: we want to sample in the middle of each bit
      // Start bit detected by EXTI, wait 1.5 bit periods to get to center of first data bit
      _rx_sample_counter++;
      if (_rx_sample_counter >= 2)  // 1.5 bit periods (rounded to 2)
      {
        _rx_sample_counter = 0;
        _rx_bit_index = 0;
        _rx_data = 0;
        _rx_state = RX_DATA_BITS;
      }
      break;
      
    case RX_DATA_BITS:
      // Sample data bit
      if (HAL_GPIO_ReadPin((GPIO_TypeDef*)_tx_port, _tx_pin_mask) == GPIO_PIN_SET)
      {
        _rx_data |= (1 << _rx_bit_index);
      }
      
      _rx_bit_index++;
      if (_rx_bit_index >= 8)
      {
        _rx_state = RX_STOP_BIT;
      }
      break;
      
    case RX_STOP_BIT:
      // Store received byte in buffer
      if (!_rx_buffer_full())
      {
        _rx_buffer[_rx_buffer_head] = _rx_data;
        _rx_buffer_head = (_rx_buffer_head + 1) % RX_BUFFER_SIZE;
      }
      
      // Could continue receiving more bytes or go idle
      // For now, stay in IDLE waiting for next start bit
      _rx_state = RX_IDLE;
      _rx_sample_counter = 0;
      break;
  }
}

/**
 * @brief Timer interrupt handler (C linkage) - supports multiple timers
 */
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Find which Software_UART instance owns this timer
  int idx = _get_timer_index(htim->Instance);
  if (idx >= 0 && g_timer_instances[idx] != nullptr)
  {
    g_timer_instances[idx]->handleTimerInterrupt();
  }
}

/**
 * @brief EXTI callback for start bit detection (C linkage)
 * @param GPIO_Pin Pin that triggered interrupt
 */
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  // Check all active instances for matching pin
  for (int i = 0; i < 7; i++)
  {
    if (g_timer_instances[i] != nullptr && GPIO_Pin == g_timer_instances[i]->getPinMask())
    {
      // Start bit detected (falling edge)
      g_timer_instances[i]->startRxFromExti();
      break;
    }
  }
}
