;/*
; * ssd1306.inc
; *
; *  Created: 30.04.2018 13:17:03
; *   Author: zoosman
; */ 



;/********************************* SSD1306 Routines *************************************/
SSD1306_INIT:
  cbr _EREG_, (1<<_I2CERF_) ; Clear error flag
  rcall CLEAR_TIMER
  DELAY_MACRO 0x05, 0xc4 ; TCCR0B (1<<CS02)|(1<<CS00) - 1024, 196, 25ms
  DELAY_MACRO 0x05, 0xc4 ; TCCR0B (1<<CS02)|(1<<CS00) - 1024, 196, 25ms
  ldi ZH, high(2*SSD1306_init_commands)
  ldi ZL, low(2*SSD1306_init_commands)
  ldi tmp, 0x17
  rcall SSD1306_BURST_COMMAND

  SSD1306_CLEAR_DISPLAY:  
    ldi ZH, high(2*SSD1306_clear)
    ldi ZL, low(2*SSD1306_clear)
    ldi tmp, 0x08
    rcall SSD1306_BURST_COMMAND
    rcall SSD1306_BURST_DATA_START
    ldi tmp, 0x08
    mov R1, tmp
    _SSD1306_SEND_CLEAR_BATCH:
      ldi tmp, 0x80
      mov R0, tmp
      _SSD1306_SEND_CLEAR:
        ldi txByte, 0
        rcall I2CM_SEND_PACKET
        dec R0
        brne _SSD1306_SEND_CLEAR
      dec R1
      brne _SSD1306_SEND_CLEAR_BATCH
    rcall I2CM_STOP
    rcall INIT_TIMER
  ret

;/* ////////////////////////////// SDD1306 Send Packet ////////////////////////////////// */
SSD1306_SEND_PACKET:
  sbrc _EREG_, _I2CERF_ ; Check error flag
  ret
  push txByte
  ldi txByte, 0
  sbrc _EREG_, _LCDCF_
  ldi txByte, 0x40
  rcall I2CM_SEND_PACKET
  pop txByte
  rcall I2CM_SEND_PACKET
  ret

;/* ////////////////////////////// SDD1306 Send Command ///////////////////////////////// */
SSD1306_SEND_COMMAND:
  sbrc _EREG_, _I2CERF_ ; Check error flag
  ret
  push txByte
  rcall I2CM_START
  DELAY_MACRO 0x03, 0x06 ; TCCR0B (1<<CS01)|(1<<CS00) - 64, 6, 48us
  lds txByte, lcdAddr
  rcall I2CM_SEND_ADDRESS
  ldi txByte, 0
  rcall I2CM_SEND_PACKET
  pop txByte
  rcall I2CM_SEND_PACKET
  rcall I2CM_STOP
  ret

;/* ////////////////////////////// SDD1306 Send Symbol ////////////////////////////////// */
SSD1306_SEND_SYMBOL:
  sbrc _EREG_, _I2CERF_ ; Check error flag
  ret
#if defined(_horizontal_)
  ldi tmp, 0x0c
#elif defined(_vertical_)
  ldi tmp, 0x18
#endif
  mov R0, tmp
  _SSD1306_SEND_DATA_BYTE:
;    ldi txByte, 0b01010101
    lpm txByte, Z+
    rcall I2CM_SEND_PACKET
    dec R0
    brne _SSD1306_SEND_DATA_BYTE
  ret

;/* ////////////////////////////// SDD1306 Burst Command //////////////////////////////// */
SSD1306_BURST_COMMAND:
  sbrc _EREG_, _I2CERF_ ; Check error flag
  ret
  mov R0, tmp
  _INIT_SSD1306_BYTE:
    lpm txByte, Z+
    rcall SSD1306_SEND_COMMAND
    dec R0
    brne _INIT_SSD1306_BYTE
  ret

;/* ///////////////////////////// SDD1306 Burst Data Start ////////////////////////////// */
SSD1306_BURST_DATA_START:
  sbrc _EREG_, _I2CERF_ ; Check error flag
  ret
  rcall I2CM_START
  DELAY_MACRO 0x03, 0x06 ; TCCR0B (1<<CS01)|(1<<CS00) - 64, 6, 48us
  lds txByte, lcdAddr
  rcall I2CM_SEND_ADDRESS
  ldi txByte, 0x40
  rcall I2CM_SEND_PACKET
  ret
