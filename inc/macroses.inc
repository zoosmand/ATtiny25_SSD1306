;/*
; * macroses.inc
; *
; *  Created: 30.04.2018 13:08:12
; *   Author: zoosman
; */ 

;/********************************** Long Time Counter ************************************/
.MACRO set_tcnt
  ldi tcntH, 0x00
  ldi tcntL, 0x7f ; approximately 4.5s
.ENDMACRO

;/**************************************** Delay ******************************************/
.MACRO DELAY_MACRO
  ldi tmp, @0
  mov dClock, tmp
  ldi tmp, @1
  mov dRate, tmp
  rcall DELAY
.ENDMACRO
