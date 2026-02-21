/**
 * @file    software_uart.h
 * @brief   Bit-bang UART (software serial)
 *
 * Provides a Stream-like interface for GPIO bit-banging
 */

#ifndef SOFTWARE_UART_H
#define SOFTWARE_UART_H

#ifdef __cplusplus

#include <stdint.h>
#include <stddef.h>
#include "Stream.h"

// Forward declaration for timer handle (full definition in stm32f4xx_hal_tim.h)
struct __TIM_HandleTypeDef;

// Forward declaration for timer handle
extern "C" {
  extern void* htim14_ptr;  // Pointer to TIM_HandleTypeDef
}

// Bit-bang UART class inheriting from Arduino Stream
class Software_UART : public Stream {
  public:
    // Constructor: set up GPIO for bit-bang UART with configurable timer
    Software_UART(uint32_t tx_pin_mask, void* tx_port, struct __TIM_HandleTypeDef* htim);

    // Stream interface - minimal methods needed by TMCStepper
    size_t write(uint8_t byte) override;
    size_t write(const uint8_t* buffer, size_t length) override;
    int read() override;
    int available() override;
    void flush() override;

    // Initialize bit-bang UART (baud rate for timing calculation)
    void begin(uint32_t baud);

  public:
    // Called by timer interrupt (must be public for C linkage)
    void handleTimerInterrupt();
    
    // Public accessors for interrupt callbacks
    uint32_t getPinMask() const { return _tx_pin_mask; }
    void startRxFromExti()  // Called from EXTI when start bit detected
    {
      if (_rx_state == RX_IDLE)
      {
        _rx_state = RX_START_BIT;
        _rx_sample_counter = 0;
      }
    }

  private:
    uint32_t _tx_pin_mask;
    void* _tx_port;
    struct __TIM_HandleTypeDef* _htim;  // Timer handle for this instance
    
    // TX state machine
    enum TxState {
      TX_IDLE,
      TX_START_BIT,
      TX_DATA_BITS,
      TX_STOP_BIT
    };
    volatile TxState _tx_state;
    volatile uint8_t _tx_data;
    volatile uint8_t _tx_bit_index;
    static const int TX_BUFFER_SIZE = 32;
    uint8_t _tx_buffer[TX_BUFFER_SIZE];
    volatile int _tx_buffer_head;
    volatile int _tx_buffer_tail;
    
    // RX state machine
    enum RxState {
      RX_IDLE,
      RX_START_BIT,
      RX_DATA_BITS,
      RX_STOP_BIT
    };
    volatile RxState _rx_state;
    volatile uint8_t _rx_data;
    volatile uint8_t _rx_bit_index;
    static const int RX_BUFFER_SIZE = 16;
    uint8_t _rx_buffer[RX_BUFFER_SIZE];
    volatile int _rx_buffer_head;
    volatile int _rx_buffer_tail;
    
    // Sampling counter for RX (sample in middle of bit)
    volatile uint8_t _rx_sample_counter;

    // Debug counters
    volatile uint32_t _debug_irq_count;
    volatile uint32_t _debug_tx_bytes_sent;
    
    // Pin mode control
    void _set_pin_mode_output();
    void _set_pin_mode_input();
    void _set_pin_mode_input_it();  // Input with EXTI
    
    // Helper methods
    bool _tx_buffer_full();
    bool _tx_buffer_empty();
    bool _rx_buffer_full();
    bool _rx_buffer_empty();
    void _start_rx();
};

// C-linkage callback for EXTI interrupt
extern "C" void Software_UART_EXTI_Callback(uint16_t GPIO_Pin);

#endif // __cplusplus

#endif /* SOFTWARE_UART_H */
