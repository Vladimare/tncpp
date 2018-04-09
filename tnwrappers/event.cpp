#include "event.hpp"


eventpack::eventpack(int inimask)
{
  this->evt.id_event = 0;

  tn_event_create(&this->evt, TN_EVENT_ATTR_MULTI, inimask);
}

eventpack::~eventpack()
{
  tn_event_delete(&this->evt);        /* все задачи, ожидающие событие, получат ошибку  */
                                      /*   ожидания TERR_DLT                            */
}

int eventpack::set(int mask)
{
  if(tn_inside_int())
    return tn_event_iset(&this->evt, mask);

  return tn_event_set(&this->evt, mask);
}

int eventpack::reset(int mask)
{
  if(tn_inside_int())
    return tn_event_iclear(&this->evt, ~mask);

  return tn_event_clear(&this->evt, ~mask);
}

int eventpack::waitAny(int mask, int timeout, unsigned int* pattern)
{
  unsigned int p = 0;

  if(tn_inside_int())
    return tn_event_iwait(&this->evt, mask, TN_EVENT_WCOND_OR, pattern ? pattern : &p);

  return tn_event_wait(&this->evt, mask, TN_EVENT_WCOND_OR, pattern ? pattern : &p, timeout);
}

int eventpack:: waitAll(int mask, int timeout, unsigned int* pattern)
{
  unsigned int p = 0;

  if(tn_inside_int())
    return tn_event_iwait(&this->evt, mask, TN_EVENT_WCOND_AND, pattern ? pattern : &p);

  return tn_event_wait(&this->evt, mask, TN_EVENT_WCOND_AND, pattern ? pattern : &p, timeout);
}

int eventpack::waitAnyPoll(int mask, unsigned int* pattern)
{
  unsigned int p = 0;

  if(tn_inside_int())
    return tn_event_iwait(&this->evt, mask, TN_EVENT_WCOND_OR, pattern ? pattern : &p);

  return tn_event_wait_polling(&this->evt, mask, TN_EVENT_WCOND_OR, pattern ? pattern : &p);
}

int eventpack::waitAllPoll(int mask, unsigned int* pattern)
{
  unsigned int p = 0;

  if(tn_inside_int())
    return tn_event_iwait(&this->evt, mask, TN_EVENT_WCOND_AND, pattern ? pattern : &p);

  return tn_event_wait_polling(&this->evt, mask, TN_EVENT_WCOND_AND, pattern ? pattern : &p);
}
