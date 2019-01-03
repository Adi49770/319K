; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
input EQU 0
divisor EQU 4
fixinput EQU 8
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11

LCD_OutDec
	PUSH {R4-R11} ;for later restoring
	MOV R11, LR ; this was an issue so I'm going to use this
	MOV R1, #0 ; I don't know why this is here but oh well 
	PUSH{R1,R0} ; To set the EQU files (binding)
	;PUSH {} ; ro for input
	STR R0, [SP, #input] ; store the R0 value into input
	MOV R2, #1
	MOV R3, #9
	MOV R1, #10 ;all these were to create a loop to store 1 billion into R2
	CMP R0, #0 ;check if the passed in value is 0 (will save us some time)
	BEQ FoundFirstDigit0
storeit ;store 1 billion into R2 because thats the right most divisor in a 32 bit register (nothing can be bigger than this)
	MUL R2, R1 ;10^9
	SUB R3, #1  
	CMP R3, #0
	BNE storeit
	STR R2, [SP, #divisor] ;end loop to store 1 billion, store it into divisor
FindFirstDigit
	LDR R2, [SP, #divisor] ; load the divisor 
	UDIV R3, R0, R2; divide Ro by the divisor 
	CMP R3, #0 ; if it is 0, then it means go to the next lower bit 
	BEQ NotFound ; if the answer is 0, go to the next bit
	B FoundFirstDigit ; first number found
NotFound
	MOV R4, #10
	UDIV R2, R4 ; divide the divisor by 10 to take out a 0
	STR R2, [SP, #divisor] ; store back the new divisor into the binded value
	B FindFirstDigit ;loop back up 
FoundFirstDigit ;found the first digit
	MOV R0, R3 ; AAPCS
	ADD R0, #0x30 ;to get the correct hex value to be printed
	MOV R9, R3 ; R9 was not modified by the pushes and the pops, this was added later to avoid stupid errors
	B LOOP ;go to LOOP
FoundFirstDigit0 ;the number was 0
	MOV R0, #0
	ADD R0, #0x30
	BL ST7735_OutChar ;print out 0 on screen
	B DONE
	;BL ST7735_OutChar
LOOP
	PUSH{R0,R11} ; to again store them in order to avoid issues
	BL ST7735_OutChar ; print out the current value in R0
	POP {R0,R11} ;restore the registers (this fucks up R3 [not restoring properly])
	MOV R3, R9 ; used this to correctly resotre R3
	LDR R2, [SP, #divisor] ;load the value of divisor
	LDR R0, [SP, #input] ; load the value of input
	MUL R5, R2, R3 ; multiply the digit that was just printed times the divisor 
	SUB R0, R0, R5 ; subtract that value from the input
	STR R0,  [SP, #input] ; store the new value of input in the binded memory location
	MOV R6, #10
	UDIV R2,R2, R6 ; take out one more 0 from the divisor
	CMP R2, #0 ;make sure the divisor is still valid 
	BEQ	DONE ; only happens when all the digits are printed off
	UDIV R0, R0, R2 ; divide the new input by the divisor to get the value to be printed
	STR R2,  [SP, #divisor] ;store the new value of divisor into the binded location
	MOV R9, R0 ; this is to make sure no values are modified for R3 (right after the POP on line 72)
	ADD R0, #0x30 ; get the accurate value to print out
	;BL ST7735_OutChar
	B LOOP ; loop back up
	
DONE
	MOV LR, R11 ;restore LR (this was fucking up because BL ST7735_OutChar was modifying the LR)
	POP {R0,R1} ; this is to reset the SP back to where it was
	;POP {R1}
	POP {R4-R11}; restore the original values thanks to AAPCS

      BX  LR ; end subroutine 
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	PUSH {R4-R11} ; to restore later
	MOV R11, LR ; to make sure other BL don't screw with this
	MOV R1, #0 ; why not
	PUSH {R1, R0} ; to use the binding
	STR R0, [SP, #fixinput] ; store the input into the binded location
	CMP R0, #0
	BEQ ZeroDecimal ; if the input is 0
	
	MOV R3, #10
	UDIV R2, R0, R3 ; see if its one digit
	CMP R2, #0 
	BEQ OneDigit
	MOV R3, #100
	UDIV R2, R0, R3 ;see if its two digits 
	CMP R2, #0
	BEQ TwoDigit
	MOV R3, #1000
	UDIV R2, R0, R3 ;see if its three digits
	CMP R2, #0
	BEQ ThreeDigit
	B FourDigit
ZeroDecimal
	MOV R5, #3
	MOV R0, #0
	BL LCD_OutDec
	MOV R0, #0x2E ; this is decimal point in hex
	BL ST7735_OutChar  
zeroloop	
	MOV R0, #0
	BL LCD_OutDec
	SUB R5, #1
	CMP R5, #0
	BNE zeroloop ; 3 zeroes 
	
	
	B Done 
	
OneDigit
	;PUSH{R0-R11}
	LDR R0, [SP,#fixinput]

	MOV R7, R0; have the decimal ready
	MOV R0, #0
	BL LCD_OutDec
	MOV R0, #0x2E ; decimal point
	BL ST7735_OutChar 
	MOV R0, #0
	BL LCD_OutDec
	MOV R0, #0
	BL LCD_OutDec
	MOV R0, R7 ;restore the decimal value
	BL LCD_OutDec
;	POP{R0-R11}
	B Done
	
TwoDigit
	LDR R0, [SP,#fixinput]
	MOV R7, R0 ;have the number ready
	MOV R0, #0
	BL LCD_OutDec
	MOV R0, #0x2E ; decimal point
	BL ST7735_OutChar 
	MOV R0, #0
	BL LCD_OutDec
	MOV R0, R7;restore the number 
	BL LCD_OutDec
	B Done
	
ThreeDigit
	LDR R0, [SP,#fixinput]
	MOV R7, R0 ;have the number ready
	MOV R0, #0
	BL LCD_OutDec
	MOV R0, #0x2E ; decimal point
	BL ST7735_OutChar 
	MOV R0, R7 ; restore number
	BL LCD_OutDec
	B Done
	
FourDigit
	LDR R0, [SP,#fixinput]
	MOV R2, #9000
	ADD R2, #1000
	CMP R0, R2
	BCS Stars ;if it's greater or equal to 10000
	MOV R2, #1000
	UDIV R7, R0, R2; get the first digit
	MOV R0, R7
	BL LCD_OutDec ; print out the fist digit
	MOV R0, #0x2E ; decimal point
	BL ST7735_OutChar  
	LDR R0, [SP, #fixinput]
	MOV R1, #1000
	MUL R7, R1; the first digit times 1000
	SUB R0, R7; R0 -= R7 to get the last three digits
	BL LCD_OutDec ; print out the last 3 digits
	B Done
	
	
Stars
	MOV R0, #0x2A; stars
	BL ST7735_OutChar  ;print the star out
	MOV R0, #0x2E ; decimal point
	BL ST7735_OutChar  
	MOV R0, #0x2A; stars
	BL ST7735_OutChar
	MOV R0, #0x2A; stars
	BL ST7735_OutChar
	MOV R0, #0x2A; stars
	BL ST7735_OutChar
	B Done
	

	
     
	 
Done	 
	MOV R0, #0
	;STR R0, [SP, #fixinput]
	POP {R1, R0} ; to restore SP
	MOV LR, R11 ; restore LR
	POP {R4- R11} ; AAPCS
	 BX   LR ; end subroutine 
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
