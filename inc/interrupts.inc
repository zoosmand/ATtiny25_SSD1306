/*
 * interrupts.inc
 *
 *  Created: 30.04.2018 13:20:00
 *   Author: zoosman
 */ 


 /***************************************** WATHDOG ***************************************/
_WDT:
  push R1
  push R0
  in R0, SREG
  push R0
	push tmp

  wdr
  in tmp, WDTCR
  sbr tmp, (1<<WDIE)
  out WDTCR, tmp

	pop tmp
  pop R0
  out SREG, R0
  pop R0
  pop R1
  reti

 
/************************************ Timer Counter ***************************************/
_TIMER0_OVF:
  push R1
  push R0
  in R0, SREG
  push R0
  push tmp

  clr R1
  out TCNT0, R1

  clc
  sbci tcntL, 0x01
  in tmp, SREG
  sbrc tmp, SREG_C
  subi tcntH, 0x01

  _CHECK_COUNTER:
    tst tcntH
    brne _OUT_OF_TIMER0
    tst tcntL
    breq _OUT_OF_SLEEP
    rjmp _OUT_OF_TIMER0

  _OUT_OF_SLEEP:
  	rcall LED0_BLINK
    in tmp, MCUCR
    cbr tmp, (1<<SE)
    out MCUCR, tmp
	set_tcnt
	sbr _EREG_, (1<<_DRIF_)

  _OUT_OF_TIMER0:
    nop
  
  pop tmp
  pop R0
  out SREG, R0
  pop R0
  pop R1
  reti