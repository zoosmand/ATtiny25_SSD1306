;
; ATtiny25_AS__I2C_Master_ssd1306_.asm
;
; Created: 16.06.2017 9:48:55
; Author : zoosman
;


;#define _horizontal_
#define _vertical_

.include "tn25def.inc"
.equ F_CPU = 8000000

.def dClock   = R12
.def dRate    = R13
.def cntHL    = R14
.def cntd     = R15
.def tmp      = R16
.def _EREG_   = R17
.def txByte   = R18
.def rxByte   = R19
.def tcntL    = R20
.def tcntH    = R21
.def tmpL     = R22
.def tmpH     = R23
.def cntLL    = R24
.def cntLH    = R25

.equ LEDDDR   = DDRB
.equ LEDPORT  = PORTB
.equ LEDPIN   = PINB
.equ LED0PIN  = PB3

.equ I2CDDR   = DDRB
.equ I2CPORT  = PORTB
.equ I2CPIN   = PINB
.equ SDA      = PB0
.equ SCL      = PB2

;/********************************* Event REGistry *******************************/
.equ _DRIF_   = 0 ; Display Run Interval Flag
.equ _I2CRWF_ = 1 ; I2C Read/Write Flag, 0 - write (m->s), 1 - read (m<-s)
.equ _I2CANF_ = 2 ; I2C Ack/Nack Flag, 0 - ACK, 1 - NACK
.equ _I2CERF_ = 3 ; I2C ERror Flag
.equ _LCDCF_  = 4 ; SSD1306 LCD Control, 0 - command, 1 - data

.cseg
.org 0


.include "./inc/vectors.inc"
.include "./inc/macroses.inc"
.include "./inc/init.inc"



;/*********************************** MAIN LOOP **********************************/
MAIN:
  sbrs _EREG_, _DRIF_
  rjmp _GO_TO_SLEEP

  cbr _EREG_, (1<<_DRIF_)
  sbrc _EREG_, _I2CERF_
  rcall SSD1306_INIT
  rcall SEND_HALLO

  _RESTORE_TIMER:
    rcall CLEAR_TIMER
    rcall INIT_TIMER

  _GO_TO_SLEEP:
    in tmp, MCUCR
    sbr tmp, (1<<SE)
    out MCUCR, tmp
    sei
    sleep

  rjmp MAIN
  rjmp THE_END



.include "./inc/i2c.inc"
.include "./inc/ssd1306.inc"
.include "./inc/interrupts.inc"
.include "./inc/utils.inc"



;/************************************ Hallo!!! **********************************/
SEND_HALLO:
  rcall CLEAR_TIMER

  ldi YH, high(SSD1306_pos)
  ldi YL, low(SSD1306_pos)

#if defined(_horizontal_)
  ; change column position
  ldi tmp, 0x20
  std Y+1, tmp
  ldi tmp, 0x50
  std Y+2, tmp
#endif

  ldi tmp, 0x06
  mov R0, tmp

  _SSD1306_SEND_COMMAND_BYTE:
    ld txByte, Y+
    rcall SSD1306_SEND_COMMAND
    dec R0
    brne _SSD1306_SEND_COMMAND_BYTE

  rcall SSD1306_BURST_DATA_START


#if defined(_horizontal_)
  ldi ZH, high(2*SSD1306_symbol_H)
  ldi ZL, low(2*SSD1306_symbol_H)
  rcall SSD1306_SEND_SYMBOL

  ldi ZH, high(2*SSD1306_symbol_a_)
  ldi ZL, low(2*SSD1306_symbol_a_)
  rcall SSD1306_SEND_SYMBOL

  ldi ZH, high(2*SSD1306_symbol_l_)
  ldi ZL, low(2*SSD1306_symbol_l_)
  rcall SSD1306_SEND_SYMBOL

  ldi ZH, high(2*SSD1306_symbol_l_)
  ldi ZL, low(2*SSD1306_symbol_l_)
  rcall SSD1306_SEND_SYMBOL

  ldi ZH, high(2*SSD1306_symbol_o_)
  ldi ZL, low(2*SSD1306_symbol_o_)
  rcall SSD1306_SEND_SYMBOL

  ldi ZH, high(2*SSD1306_symbol_01)
  ldi ZL, low(2*SSD1306_symbol_01)
  rcall SSD1306_SEND_SYMBOL

  ldi ZH, high(2*SSD1306_symbol_01)
  ldi ZL, low(2*SSD1306_symbol_01)
  rcall SSD1306_SEND_SYMBOL

  ldi ZH, high(2*SSD1306_symbol_01)
  ldi ZL, low(2*SSD1306_symbol_01)
  rcall SSD1306_SEND_SYMBOL

#elif defined(_vertical_)
  ldi ZH, high(2*SSD1306_symbol_degC)
  ldi ZL, low(2*SSD1306_symbol_degC)
  rcall SSD1306_SEND_SYMBOL

  ldi ZH, high(2*SSD1306_symbol_5)
  ldi ZL, low(2*SSD1306_symbol_5)
  rcall SSD1306_SEND_SYMBOL
  
  ldi ZH, high(2*SSD1306_symbol_2)
  ldi ZL, low(2*SSD1306_symbol_2)
  rcall SSD1306_SEND_SYMBOL

#endif

  rcall I2CM_STOP
  rcall INIT_TIMER

  ret

;/************************************ THE END ************************************/
THE_END:
  cli
  rjmp 0



.include "./inc/var.inc"

