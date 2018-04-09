#ifndef _TYPESEXT_H_
#define _TYPESEXT_H_

/********************************** базовые callback'и **********************************/
struct callback0
{
  virtual void operator()() = 0;
};

struct callback1
{
  virtual void operator()(int arg) = 0;
};

struct callback2
{
  virtual void operator()(int arg1, int arg2) = 0;
};

template<class _T>
struct binder0
  : public callback0
{
  typedef void (_T::* _T_mem_func)();

  binder0  ()                     { this->method = 0; this->instance= 0;  }
  binder0  (_T_mem_func m, _T* i) { this->method = m; this->instance= i;  }

  void bind(_T_mem_func m, _T* i) { this->method = m; this->instance= i;  }

  virtual void operator()()
  {
    (instance->*method)();
  }

  _T_mem_func method;
  _T*         instance;
};

template<class _T>
struct binder1
  : public callback1
{
  typedef void (_T::* _T_mem_func)(int);

  binder1  ()                     { this->method = 0; this->instance= 0;  }
  binder1  (_T_mem_func m, _T* i) { this->method = m; this->instance= i;  }

  void bind(_T_mem_func m, _T* i) { this->method = m; this->instance= i;  }

  virtual void operator()(int arg)
  {
    (instance->*method)(arg);
  }

  _T_mem_func method;
  _T*         instance;
};

template<class _T>
struct binder2
  : public callback2
{
  typedef void (_T::* _T_mem_func)(int, int);

  binder2  ()                     { this->method = 0; this->instance= 0;  }
  binder2  (_T_mem_func m, _T* i) { this->method = m; this->instance= i;  }

  void bind(_T_mem_func m, _T* i) { this->method = m; this->instance= i;  }

  virtual void operator()(int arg1, int arg2)
  {
    (instance->*method)(arg1, arg2);
  }

  _T_mem_func method;
  _T*         instance;
};

#endif /*_TYPESEXT_H_*/
