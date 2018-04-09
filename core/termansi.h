#ifndef _TERMANSI_H_
#define _TERMANSI_H_


                                      /* атрибуты текста                                */
#define TERMANSI_TATTR_ALLOFF     0   /* все атрибуты отключены                         */
#define TERMANSI_TATTR_BOLD       1   /* жирный текст                                   */
#define TERMANSI_TATTR_UNDERSCORE 4   /* подчеркивание (только на монохромных)          */
#define TERMANSI_TATTR_BLINK      5   /* мигание                                        */
#define TERMANSI_TATTR_REVVIDEO   7   /* инверсия                                       */
#define TERMANSI_TATTR_CONCEALED  8   /* утопленный                                     */

                                      /* foreground цвета                                */
#define TERMANSI_FC_BLACK         30
#define TERMANSI_FC_RED           31
#define TERMANSI_FC_GREEN         32
#define TERMANSI_FC_YELLOW        33
#define TERMANSI_FC_BLUE          34
#define TERMANSI_FC_MAGENTA       35
#define TERMANSI_FC_CYAN          36
#define TERMANSI_FC_WHITE         37

                                      /* background цвета                               */
#define TERMANSI_BC_BLACK         40
#define TERMANSI_BC_RED           41
#define TERMANSI_BC_GREEN         42
#define TERMANSI_BC_YELLOW        43
#define TERMANSI_BC_BLUE          44
#define TERMANSI_BC_MAGENTA       45
#define TERMANSI_BC_CYAN          46
#define TERMANSI_BC_WHITE         47

                                      /* параметры терминала                            */
#define TERMANSI_DISP_TXT_MONO_40x25      0
#define TERMANSI_DISP_TXT_COLOR_40x25     1
#define TERMANSI_DISP_TXT_MONO_80x25      2
#define TERMANSI_DISP_TXT_COLOR_80x25     3
#define TERMANSI_DISP_GR_COLOR4_320x200   4
#define TERMANSI_DISP_GR_MONO_320x200     5
#define TERMANSI_DISP_GR_MONO_640x200     6
#define TERMANSI_DISP_LINE_WRAPPING       7
#define TERMANSI_DISP_GR_COLOR_320x200    13
#define TERMANSI_DISP_GR_COLOR16_640x200  14
#define TERMANSI_DISP_GR_MONO_640x350     15
#define TERMANSI_DISP_GR_COLOR16_640x350  16
#define TERMANSI_DISP_GR_MONO_640x480     17
#define TERMANSI_DISP_GR_COLOR16_640x480  18
#define TERMANSI_DISP_GR_COLOR256_320x200 19


#define TERMANSI_SET_CURSOR(l, c)       "\x1B["#l";"#c"H"
#define TERMANSI_CURSOR_UP(n)           "\x1B["#n"A"
#define TERMANSI_CURSOR_DOWN(n)         "\x1B["#n"B"
#define TERMANSI_CURSOR_FWD(n)          "\x1B["#n"C"
#define TERMANSI_CURSOR_BKWD(n)         "\x1B["#n"D"
#define TERMANSI_CURSOR_SAVE            "\x1B[s"
#define TERMANSI_CURSOR_RESTORE         "\x1B[u"
#define TERMANSI_ERASE                  "\x1B[2J"
#define TERMANSI_ERASE_LINE             "\x1B[K"
#define TERMANSI_SET_GRMODE(ta, fc, bc) "\x1B["#ta";"#fc";"#bc"m"
#define TERMANSI_SET_MODE(m)            "\x1B[="#m"h"
#define TERMANSI_RESET_MODE(m)          "\x1B[="#m"l"


#endif /*_TERMANSI_H_*/
