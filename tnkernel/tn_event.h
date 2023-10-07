#ifndef _TN_EVENT_H_
#define _TN_EVENT_H_

#define TN_EVENT_ATTR_MULTI 1
#define TN_EVENT_WCOND_OR 1
#define TN_EVENT_WCOND_AND 2

typedef struct _TN_EVENT {
    int id_event;
} TN_EVENT;

void tn_event_create(TN_EVENT *evt, int attr, int mask);
void tn_event_delete(TN_EVENT *evt);
int tn_inside_int(void);
int tn_event_iset(TN_EVENT *evt, int mask);
int tn_event_set(TN_EVENT *evt, int mask);
int tn_event_iclear(TN_EVENT *evt, int mask);
int tn_event_clear(TN_EVENT *evt, int mask);
int tn_event_iwait(TN_EVENT *evt, int mask, int cond, unsigned int *pattern);
int tn_event_wait(TN_EVENT *evt, int mask, int cond, unsigned int *pattern, int timeout);
int tn_event_wait_polling(TN_EVENT *evt, int mask, int cond, unsigned int *pattern);

#endif