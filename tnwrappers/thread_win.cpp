#include "thread.hpp"

//constructor
thread::thread(const threadCreationDisposition& cd)
  : stack(0)
{
  unsigned int* sptr = 0;

  if(!cd.sp)
  {                                   /* если стек не задан, то создаем его сами        *
                                       *   при этом указатель this->stack станет        *
                                       *   ненулевым, и деструктор его зачистит         */
    sptr = new unsigned int[cd.sp_size];
    this->stack = sptr;
  }
  else
    sptr = cd.sp;

  this->name = cd.name;

  this->tcb.user_data = this;
  this->tcb.id_task   = 0;
  _beginthread(thread::runner, 1024, NULL);
  CreateThread(
    __in_opt  LPSECURITY_ATTRIBUTES lpThreadAttributes,
    cd.sp_size,
    thread::runner,
    this,
    __in      DWORD dwCreationFlags,
    __out_opt LPDWORD lpThreadId
    );
  tn_task_create(&this->tcb, thread::runner, cd.priority, &(sptr[cd.sp_size - 1]), cd.sp_size, this, cd.options);
}

//destructor
thread::~thread()
{
  if(this->stack)
    delete[] this->stack;

  tn_task_delete(&this->tcb);
}

//thread proxy
void thread::runner(void* _this)
{
  thread* th = (thread*)_this;
  th->run();
}

//change priority
int thread::changePriority(int priority)
{
  return tn_task_change_priority(&this->tcb, priority);
}

//terminate
int thread::terminate()
{
  return tn_task_terminate(&this->tcb);
}

//task terminate
void thread::exit(int attr)
{
  return tn_task_exit(attr);
}

//task activate
int thread::activate()
{
  if(tn_inside_int())
    return tn_task_iactivate(&this->tcb);
  else
    return tn_task_activate(&this->tcb);
}

//task suspend
int thread::suspend()
{
  return tn_task_suspend(&this->tcb);
}

//task resume
int thread::resume()
{
  return tn_task_resume(&this->tcb);
}

//task sleep
int thread::sleep(unsigned int timeout)
{
  return tn_task_sleep(timeout);
}

//task wakeup
int thread::wakeup()
{
  if(tn_inside_int())
    return tn_task_iwakeup(&this->tcb);
  else
    return tn_task_wakeup(&this->tcb);
}

//forcibly release task from waiting (including sleep),
//  but not from the SUSPENDED state
int thread::releaseWait()
{
  if(tn_inside_int())
    return tn_task_irelease_wait(&this->tcb);
  else
    return tn_task_release_wait(&this->tcb);
}

int thread::getStackUsage()
{
  return thread::getStackUsage(&this->tcb);
}

int thread::getStackUsage(TN_TCB* t)
{
  int scnt = t->stk_size;
  unsigned int* pstk = t->stk_start - t->stk_size + 1;

  while(scnt && ((*pstk) == TN_FILL_STACK_VAL))
  {
    scnt--;
    pstk++;
  }

  return scnt;
}
