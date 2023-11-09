; Define symbolic names for registers
.equ USER_MODE, 0x10  ; User mode
.equ CPSR, 0x00  ; Current Program Status Register
.equ R0, 0  ; General-purpose register R0
.equ R1, 1  ; General-purpose register R1

; Function to switch to User mode
switchToUserMode:
    MRS R0, CPSR  ; Read CPSR into R0
    BIC R0, R0, #0x1F  ; Clear the mode bits
    ORR R0, R0, USER_MODE  ; Set the mode bits to User mode
    MSR CPSR, R0  ; Write R0 back to CPSR
    BX LR

; Your main program
main:
    ; Check if a condition is met to determine whether to jump to the user application
    ; ...

    ; Switch to User mode
    BL switchToUserMode

    ; Jump to the user application
    LDR R1, =USER_APP_START_ADDR
    LDR R0, [R1]
    BX R0

    ; Stay in the bootloader
    ; ...

USER_APP_START_ADDR: .word 0x00008000