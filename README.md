libsig
======

Signal/Event handling lib for C/C++

[![Build Status](https://travis-ci.org/recp/libsig.svg?branch=master)](https://travis-ci.org/recp/libsig)

#--This is still under development!--#

ligsig is a signal/event library for C and C++ that allow to handling signals/events like observer pattern. C/C++ and C++ member functions can be used as callbacks. 

signals/events can observed by `sig_attach` functions. The signals/events can be triggered by `sig_fire` functions. The both sig_attach and sig_fire functions are overloaded. The functions can be called by given name (string) or id (integer). 

Each signal has a signal context which allows signals can be use in their own context. If signal context has not been specified then observing/firing will handling in default signal context.

Also the system signals such as SIGKILL... can be observed in system context (sig_sys_ctx (This will be defined) ).

####For C:####
The functions are overloaded by `_s` postfix for C because C language does not support function overloading:
```C
/* Observe a signal by signal name or id */
void sig_attach(int signal, sig_observer_cb_t cb);
void sig_attach_s(const char * signal, sig_observer_cb_t cb);

/* Fire a signal by signal name or id */
void sig_fire(int signal, void * object);
void sig_fire_s(const char * signal, void * object);
```

####For C++:####
```C++
/* Observe a signal by signal name or id */
void sig_attach(int signal, sig_observer_cb_t cb);
void sig_attach(const char * signal, sig_observer_cb_t cb);

/* Fire a signal by signal name or id */
void sig_fire(int signal, void * object);
void sig_fire(const char * signal, void * object);
```

Signals also can be observed or can be fired by the following style for C++:

```C++
/* Observe a signal */
sig::attach[1234] << fn_callback1;
sig::attach["signal_name"] << fn_callback1 << fn_callback2;

/* Stop observe */
sig::attach[1234] >> fn_callback1;
sig::attach["signal_name"] >> fn_callback1 >> fn_callback2;

/* Fire (trigger) a signal */
sig:fire["signal_name"] << (void *)"Signal object (void *)";
```

###Sample###

```C++
#include <sig.h>
#include <functional>

void do_somethings(const sig_signal_t * const signal) {
	fprintf(stderr, "%s", (const char *)signal->object);
}

int main(int argc, const char * argv[]) {
  // Observe a signal/event
  sig_attach("signal-1", do_somethings);
  
  // Or 
  sig::attach["signal-1"] << do_somethings;
  
  
  // Fire signal
  sig_fire("signal-1", (void *)"Hello World!");
  
  // Or
  sig::fire["signal-1"] << (void *)"Hello World!";
  
  return 0;
}
```



