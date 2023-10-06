#ifndef _TYPES_H_
#define _TYPES_H_


/*************************************** callback ***************************************/
typedef void (*CALLBACK0)(void);
typedef void (*CALLBACK1)(void*);
typedef void (*CALLBACK2)(void*, void*);

/************************* указатель на обработчики прерываний **************************/
typedef void (*IRQ_HANDLER)(void*);
typedef struct tagIRQ_DATA
{
  IRQ_HANDLER h;
  void*       instance;
}IRQ_DATA;

typedef int (*SERVICE)(void*, void*);
typedef struct tagSERVICE_DATA
{
  SERVICE h;
  void*   instance;
}SERVICE_DATA;


/************************* указатель на функции настройки ножек *************************/
typedef void (*PIN_SETUP)(unsigned char setDefault);

/*********************************** кольцевой буфер ************************************/
typedef struct tagCIRC_BUF16  {unsigned long wr_idx; unsigned long rd_idx; unsigned char data[  16];}CIRC_BUF16;
typedef struct tagCIRC_BUF32  {unsigned long wr_idx; unsigned long rd_idx; unsigned char data[  32];}CIRC_BUF32;
typedef struct tagCIRC_BUF64  {unsigned long wr_idx; unsigned long rd_idx; unsigned char data[  64];}CIRC_BUF64;
typedef struct tagCIRC_BUF128 {unsigned long wr_idx; unsigned long rd_idx; unsigned char data[ 128];}CIRC_BUF128;
typedef struct tagCIRC_BUF256 {unsigned long wr_idx; unsigned long rd_idx; unsigned char data[ 256];}CIRC_BUF256;
typedef struct tagCIRC_BUF512 {unsigned long wr_idx; unsigned long rd_idx; unsigned char data[ 512];}CIRC_BUF512;
typedef struct tagCIRC_BUF1024{unsigned long wr_idx; unsigned long rd_idx; unsigned char data[1024];}CIRC_BUF1024;
typedef struct tagCIRC_BUF2048{unsigned long wr_idx; unsigned long rd_idx; unsigned char data[2048];}CIRC_BUF2048;
typedef struct tagCIRC_BUF4096{unsigned long wr_idx; unsigned long rd_idx; unsigned char data[4096];}CIRC_BUF4096;

#define CIRC_BUF(x) CIRC_BUF##x

#define CBUF_SIZE(buf)      (sizeof((buf).data))
#define CBUF_RESET(buf)     ((buf).rd_idx = (buf).wr_idx = 0)
#define CBUF_COUNT(buf)     ((CBUF_SIZE(buf) - 1) & ((buf).wr_idx - (buf).rd_idx))
#define CBUF_RD(buf)        ((buf).data[(CBUF_SIZE(buf) - 1) & ((buf).rd_idx)++])
#define CBUF_PRERD(buf)     ((buf).data[(CBUF_SIZE(buf) - 1) & ++((buf).rd_idx)])
#define CBUF_WR(buf, byte)  ((buf).data[(CBUF_SIZE(buf) - 1) & (buf).wr_idx++] = (byte))
#define CBUF_PEEK(buf)      ((buf).data[(CBUF_SIZE(buf) - 1) & (buf).rd_idx])
#define CBUF_PREVIDX(buf)   (((buf).rd_idx - 1) & (CBUF_SIZE(buf) - 1))
#define CBUF_FULL(buf)      (CBUF_COUNT(buf) == (CBUF_SIZE(buf) - 1))


/*************************** двунаправленный кольцевой список ***************************/
typedef struct tagCIRC_DLL
{
  struct tagCIRC_DLL* next;
  struct tagCIRC_DLL* prev;
  void*               data;
}CIRC_DLL;


#define CDLL_RESET  (pcdll)        (pcdll->prev = pcdll->next = pcdll)
#define CDLL_ISEMPTY(pcdll)       ((pcdll->prev == pcdll) && (pcdll->next == pcdll))
#define CDLL_ADDHEAD(pcdll, pnew) {                                   \
                                    pnew->next      = pcdll->next;    \
                                    pnew->prev      = pcdll;          \
                                    pnew->next->prev= pnew;           \
                                    pcdll->next     = pnew;           \
                                  }
#define CDLL_ADDTAIL(pcdll, pnew) {                                   \
                                    pnew->next      = pcdll;          \
                                    pnew->prev      = pcdll->prev;    \
                                    pnew->prev->next= pnew;           \
                                    pcdll->prev     = pnew;           \
                                  }
#define CDLL_REMHEAD(pcdll)       {                                   \
                                    pcdll->next->next->prev = pcdll;  \
                                    pcdll->next = pcdll->next->next;  \
                                  }
#define CDLL_REMTAIL(pcdll)       {                                   \
                                    pcdll->prev->prev->next = pcdll;  \
                                    pcdll->prev = pcdll->prev->prev;  \
                                  }
#define CDLL_REMOVE (pentry)      {                                   \
                                    pentry->prev->next = pentry->next;\
                                    pentry->next->prev = pentry->prev;\
                                  }
__inline unsigned char CDLL_SEARCH(CIRC_DLL* pcdll, CIRC_DLL* pentry)
{
  CIRC_DLL* curr_dll = pcdll->next;

  while(curr_dll != pcdll)
  {
    if(curr_dll == pentry)
      return 0x01;

    curr_dll = curr_dll->next;
  }

  return 0x00;
}

/************************ данные для создания объектов драйверов ************************/
struct basicCreationDisposition
{
  unsigned char irqNumber;
  unsigned char irqPriority;
  PIN_SETUP     pinProc;
  unsigned long pwrMask;
};


#endif /*_TYPES_H_*/
