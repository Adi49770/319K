; SysTick.s
; Module written by: Jerry Yang/Aditya Tyagi
; Date Created: 2/19/2017
; Last Modified: 2/19/2017 
; Brief Description: Initializes SysTick

NVIC_ST_CTRL_R        EQU 0xE000E010
NVIC_ST_RELOAD_R      EQU 0xE000E014
NVIC_ST_CURRENT_R     EQU 0xE000E018

        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
; ;-UUU-Export routine(s) from SysTick.s to callers
		EXPORT SysTick_Init
		EXPORT Read_SysTick

;------------SysTick_Init------------
; ;-UUU-Complete this subroutine
; Initialize SysTick with busy wait running at bus clock.
; Input: none
; Output: none
; Modifies: ??
SysTick_Init
	LDR R1,=NVIC_ST_CTRL_R
	LDR R0,[R1]
	AND R0,#0xE
	STR R0,[R1]
	
	LDR R1,=NVIC_ST_RELOAD_R
	ORR R0,#0x00FFFFFF
	STR R0,[R1]
	
	LDR R1,=NVIC_ST_CURRENT_R
	STR R0,[R1]
	
	LDR R1,=NVIC_ST_CTRL_R
	LDR R0,[R1]
	AND R0, #0xD
	ORR R0,#0x5
	STR R0,[R1]	
 
    BX  LR                          ; return

Read_SysTick
	LDR R0,=NVIC_ST_CURRENT_R
	LDR R0,[R0]
	BX LR


    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
