

                EXPORT  tn_startup_hardware_init
                EXPORT  tn_arm_disable_interrupts
                EXPORT  tn_arm_enable_interrupts

;-----------------------------------------------------------------------------

                AREA    HardwareInit, CODE, READONLY
                THUMB

; ----------------------------------------------------------------------------
;  This routine is called immediately after reset to setup hardware that is
; vital for processor's functionality (for instance,SDRAM controller setup,
; PLL setup,etc.)
;  It is assumed that other hardware's init routine(s) will be invoked later
; by C-language function call.
; ----------------------------------------------------------------------------

tn_startup_hardware_init
                BX      LR

; ----------------------------------------------------------------------------
tn_arm_disable_interrupts

                CPSID   I
                BX      LR

; ----------------------------------------------------------------------------
tn_arm_enable_interrupts

                CPSIE   I
                BX      LR


                END
