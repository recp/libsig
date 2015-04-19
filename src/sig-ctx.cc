/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "sig.h"
#include "sig-ctx.h"

#include <vector>

static int s_sig_context_last_id = 1000;

enum sig_reserved_context_id {
  kSigReservedContextId_Default = 0,
  kSigReservedContextId_Sys     = 1
};

sig_signal_context_s::sig_signal_context_s(int id) : ctx_id(id) { }

const sig_signal_context_t *
sig_ctx_default() {
  static  sig_signal_context_t  * __sig_default_ctx;
  if (!__sig_default_ctx) {
    __sig_default_ctx = new sig_signal_context_t(kSigReservedContextId_Default);
  }
  return __sig_default_ctx;
}

const sig_signal_context_t *
sig_ctx_sys() {
  static  sig_signal_context_t  * __sig_sys_ctx;
  if (!__sig_sys_ctx) {
    __sig_sys_ctx = new sig_signal_context_t(kSigReservedContextId_Sys);
  }
  return __sig_sys_ctx;
}

const sig_signal_context_t *
sig_ctx_new() {
  int next_id = ++s_sig_context_last_id;
  sig_signal_context_t * new_ctx = new sig_signal_context_t(next_id);
  return new_ctx;
}

namespace sig {

template <typename T>
context_manager<T>::sig_signal_req_t::sig_signal_req_t(T signal,
                                                       sig_observer_cb_t cb)
  : m_signal_id(signal),
    m_cb(cb) { }

template <typename T>
template <typename ... _Types>
void
context_manager<T>::sig_signal_req_t::
operator()(_Types ... _Args) const {
  m_cb(_Args...);
}
  
template <typename T>
bool
context_manager<T>::sig_signal_req_t::operator== (const T& r1) const {
  return m_signal_id == r1;
}

template < >
bool
context_manager<CStringPtr>::sig_signal_req_t::operator==(const CStringPtr& r1) const {
  return strcmp(m_signal_id, r1) == 0;
}

template <typename T>
context_manager<T>::context_manager(const sig_signal_context_t * sig_ctx) {
  m_observers = new std::vector<sig_signal_req_t>;
}

template <typename T>
void
context_manager<T>::add(T signal, sig_observer_cb_t cb) const {
  sig_signal_req_t sig_req(signal, cb);
  m_observers->push_back(sig_req);
}

template <typename T>
void
context_manager<T>::fire(T signal, void * object) const {
  typename std::vector<sig_signal_req_t>:: iterator it = m_observers->template begin();
  for (; it != m_observers->end(); it++) {
     decltype(*it) sig_req = *it;
     if (sig_req == signal) {
       sig_signal_t signal_object(nullptr, // TODO:
                                  m_ctx,
                                  object);
       sig_req.m_cb(signal_object);
     }
  }
}

template <typename T>
const sig_signal_context_t *  context_manager<T>::ctx() const {
  return m_ctx;
}

template <typename T>
context_manager<T>::~context_manager() {
  m_observers->clear();
  delete m_observers;
}

// default context managers
context_manager<int> __sig_def_ctx_mngr(sig_ctx_default());
context_manager<CStringPtr> __sig_def_ctx_mngr_s(sig_ctx_sys());

// explicit instantiation
template class context_manager<int>;
template class context_manager<CStringPtr>;

};
