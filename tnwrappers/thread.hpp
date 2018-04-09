#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#include "tn.h"

struct threadCreationDisposition
{
  int priority;
  int sp_size;
  int options;
  unsigned int* sp;
  const char* name;
};


class thread
{
public:

  thread(const threadCreationDisposition& cd);
  virtual ~thread();

  int  changePriority(int priority);

  int  terminate();
  void exit(int attr);
  int  activate();

  int  suspend();
  int  resume();

  int  sleep(unsigned int timeout);
  int  wakeup();

  int  releaseWait();

  const char* taskname()  {return this->name;}

  static int getStackUsage(TN_TCB* t);
         int getStackUsage();

protected:
  virtual void run() = 0;

private:
  TN_TCB tcb;
  unsigned int* stack;
  const char* name;

  static void runner(void* _this);
};

#endif /*_THREAD_HPP_*/
