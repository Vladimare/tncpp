#ifndef _LPC13xxDEFS_H_
#define _LPC13xxDEFS_H_

#include <lpc13xx.h>


/* NVIC registers */
#define IRQ0_WAKEUP0_00     ( 0 + (32 * 0))
#define IRQ0_WAKEUP0_01     ( 1 + (32 * 0))
#define IRQ0_WAKEUP0_02     ( 2 + (32 * 0))
#define IRQ0_WAKEUP0_03     ( 3 + (32 * 0))
#define IRQ0_WAKEUP0_04     ( 4 + (32 * 0))
#define IRQ0_WAKEUP0_05     ( 5 + (32 * 0))
#define IRQ0_WAKEUP0_06     ( 6 + (32 * 0))
#define IRQ0_WAKEUP0_07     ( 7 + (32 * 0))
#define IRQ0_WAKEUP0_08     ( 8 + (32 * 0))
#define IRQ0_WAKEUP0_09     ( 9 + (32 * 0))
#define IRQ0_WAKEUP0_10     (10 + (32 * 0))
#define IRQ0_WAKEUP0_11     (11 + (32 * 0))
#define IRQ0_WAKEUP1_00     (12 + (32 * 0))
#define IRQ0_WAKEUP1_01     (13 + (32 * 0))
#define IRQ0_WAKEUP1_02     (14 + (32 * 0))
#define IRQ0_WAKEUP1_03     (15 + (32 * 0))
#define IRQ0_WAKEUP1_04     (16 + (32 * 0))
#define IRQ0_WAKEUP1_05     (17 + (32 * 0))
#define IRQ0_WAKEUP1_06     (18 + (32 * 0))
#define IRQ0_WAKEUP1_07     (19 + (32 * 0))
#define IRQ0_WAKEUP1_08     (20 + (32 * 0))
#define IRQ0_WAKEUP1_09     (21 + (32 * 0))
#define IRQ0_WAKEUP1_10     (22 + (32 * 0))
#define IRQ0_WAKEUP1_11     (23 + (32 * 0))
#define IRQ0_WAKEUP2_00     (24 + (32 * 0))
#define IRQ0_WAKEUP2_01     (25 + (32 * 0))
#define IRQ0_WAKEUP2_02     (26 + (32 * 0))
#define IRQ0_WAKEUP2_03     (27 + (32 * 0))
#define IRQ0_WAKEUP2_04     (28 + (32 * 0))
#define IRQ0_WAKEUP2_05     (29 + (32 * 0))
#define IRQ0_WAKEUP2_06     (30 + (32 * 0))
#define IRQ0_WAKEUP2_07     (31 + (32 * 0))
#define IRQ1_WAKEUP2_08     ( 0 + (32 * 1))
#define IRQ1_WAKEUP2_09     ( 1 + (32 * 1))
#define IRQ1_WAKEUP2_10     ( 2 + (32 * 1))
#define IRQ1_WAKEUP2_11     ( 3 + (32 * 1))
#define IRQ1_WAKEUP3_00     ( 4 + (32 * 1))
#define IRQ1_WAKEUP3_01     ( 5 + (32 * 1))
#define IRQ1_WAKEUP3_02     ( 6 + (32 * 1))
#define IRQ1_WAKEUP3_03     ( 7 + (32 * 1))
#define IRQ1_I2C            ( 8 + (32 * 1))
#define IRQ1_TIMER16_0      ( 9 + (32 * 1))
#define IRQ1_TIMER16_1      (10 + (32 * 1))
#define IRQ1_TIMER32_0      (11 + (32 * 1))
#define IRQ1_TIMER32_1      (12 + (32 * 1))
#define IRQ1_SSP            (13 + (32 * 1))
#define IRQ1_UART           (14 + (32 * 1))
#define IRQ1_USB_IRQ        (15 + (32 * 1))
#define IRQ1_USB_FIQ        (16 + (32 * 1))
#define IRQ1_ADC            (17 + (32 * 1))
#define IRQ1_WDT            (18 + (32 * 1))
#define IRQ1_BOD            (19 + (32 * 1))
#define IRQ1_FMC            (20 + (32 * 1))
#define IRQ1_PIOINT3        (21 + (32 * 1))
#define IRQ1_PIOINT2        (22 + (32 * 1))
#define IRQ1_PIOINT1        (23 + (32 * 1))
#define IRQ1_PIOINT0        (24 + (32 * 1))

/* STCTRL register */
#define ENABLE               0
#define TICKINT              1
#define CLKSOURCE            2
#define COUNTFLAG            16

/* SYSPLLSTAT register */
#define SYSPLL_LOCK          0

/* MAINCLKSEL register */
#define nCLKSRC_INTRC        0
#define nCLKSRC_PLLIN        1
#define nCLKSRC_WDT          2
#define nCLKSRC_PLLOUT       3

/* WDMOD register */
#define WDEN                 0
#define WDRESET              1
#define WDTOF                2
#define WDINT                3

/* PDRUNCFG register */
#define IRCOUT_PD            0
#define IRC_PD               1
#define FLASH_PD             2
#define BOD_PD               3
#define ADC_PD               4
#define SYSOSC_PD            5
#define WDTOSC_PD            6
#define SYSPLL_PD            7
#define USBPLL_PD            8
#define USBPAD_PD            10

/* SYSAHBCLKCTRL register */
#define SYS                  0
#define ROM                  1
#define RAM                  2
#define FLASHREG             3
#define FLASHARRAY           4
#define I2C                  5
#define GPIO                 6
#define CT16B0               7
#define CT16B1               8
#define CT32B0               9
#define CT32B1               10
#define SSP                  11
#define UART                 12
#define ADC                  13
#define USB_REG              14
#define WDT                  15
#define IOCON                16

/* UxLCR registers */
#define LCR_WORDLENGTH_5     (0ul << 0)
#define LCR_WORDLENGTH_6     (1ul << 0)
#define LCR_WORDLENGTH_7     (2ul << 0)
#define LCR_WORDLENGTH_8     (3ul << 0)

#define LCR_STOPBITS_1       (0ul << 2)
#define LCR_STOPBITS_2       (1ul << 2)

#define LCR_PARITY_NONE      (0ul << 3)
#define LCR_PARITY_ODD       (1ul << 3)
#define LCR_PARITY_EVEN      (3ul << 3)
#define LCR_PARITY_MARK      (5ul << 3)
#define LCR_PARITY_SPACE     (7ul << 3)

#define LCR_BREAKCONTROL     (1ul << 6)

#define LCR_DLAB             (1ul << 7)

/* UxIER register */
#define RBRIE                0
#define THREIE               1
#define RxLineIE             2
#define MSIE                 3
#define CTSIE                7
#define ABEOIntEn            8
#define ABTOIntEn            9

/* UxFCR register */
#define FIFOEn               0
#define RxFIFOReset          1
#define TxFIFOReset          2
#define DMAModeSelect        3

#define RxTRIGGERLVL_1      (0ul << 6)
#define RxTRIGGERLVL_4      (1ul << 6)
#define RxTRIGGERLVL_8      (2ul << 6)
#define RxTRIGGERLVL_14     (3ul << 6)

/* UxTER registers */
#define TXEN                 7

/* UxLSR registers */
#define RDR                  0
#define OE                   1
#define PE                   2
#define FE                   3
#define BI                   4
#define THRE                 5
#define TEMT                 6
#define RXFE                 7

/* UxMCR registers */
#define DTRControl           0
#define RTSControl           1
#define LoopbackEnable       4
#define RTSEnable            6
#define CTSEnable            7

/* UxMSR registers */
#define DeltaCTS             0
#define DeltaDSR             1
#define TrailingEdgeRI       2
#define DeltaDCD             3
#define MSR_CTS              4
#define MSR_DSR              5
#define MSR_RI               6
#define MSR_DCD              7

/* TxCR registers */
#define CNTR_ENABLE          0
#define CNTR_RESET           1

/* TxMCR registers */
#define MR0I                 0
#define MR0R                 1
#define MR0S                 2
#define MR1I                 3
#define MR1R                 4
#define MR1S                 5
#define MR2I                 6
#define MR2R                 7
#define MR2S                 8
#define MR3I                 9
#define MR3R                 10
#define MR3S                 11

/* TxCCR registers */
#define CAP0RE               0
#define CAP0FE               1
#define CAP0I                2

/* TxIR registers */
#define _MR0                 0
#define _MR1                 1
#define _MR2                 2
#define _MR3                 3
#define _CR0                 4

#endif /*_LPC13xxDEFS_H_*/
