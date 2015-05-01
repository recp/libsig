/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "sig.h"

#include <vector>
#include <map>
#include <string.h>

namespace sig {

#define SIG_REQ_TYPE_INT 0
#define SIG_REQ_TYPE_STR 1

#define sig_def_ctx sig_ctx_default()

typedef const char * CStringPtr;

struct sig_signal_req;

unsigned int s_last_sig_id = 1;
unsigned int s_sig_context_last_id = 1000;

struct cmp_str {
  bool operator()(char const *a, char const *b) const {
    return strcmp(a, b) < 0;
  }
};

std::vector<sig_signal_req *> signals_reqs;
std::map<CStringPtr, unsigned int, cmp_str> string_map;

struct sig_signal_req {
  typedef sig_observer_cb_t       sig_cb_t;
  typedef sig_mem_observer_base_t sig_mem_cb_t;
  typedef unsigned int            uid_t;
  typedef unsigned int            req_type_t;

  uid_t                 uid;
  req_type_t            req_type;
  sig_cb_t              cb;
  sig_mem_cb_t        * mem_cb;
  const sig_context_t * ctx;

  sig_signal_req(req_type_t req_type,
                 const sig_context_t * ctx,
                 uid_t _uid,
                 sig_cb_t _cb);

  sig_signal_req(req_type_t req_type,
                 const sig_context_t * ctx,
                 uid_t _uid,
                 sig_mem_cb_t * _cb);

  ~sig_signal_req();

  bool operator==(const uid_t& r1) const;
  void operator()(sig_signal_t s) const;
};

sig_signal_req::sig_signal_req(req_type_t _req_type,
                               const sig_context_t * _ctx,
                               uid_t _uid,
                               sig_cb_t _cb)
  : req_type(_req_type),
    ctx(_ctx),
    uid(_uid),
    cb(_cb),
    mem_cb(NULL) { }

sig_signal_req::sig_signal_req(req_type_t _req_type,
                               const sig_context_t * _ctx,
                               uid_t _uid,
                               sig_mem_cb_t * _cb)
  : req_type(_req_type),
    ctx(_ctx),
    uid(_uid),
    cb(NULL),
    mem_cb(_cb) { }

sig_signal_req::~sig_signal_req() {
  if (this->mem_cb)
    delete this->mem_cb;
}

bool
sig::sig_signal_req::operator==(const uid_t &an_uid) const {
  return this->uid == an_uid;
}

void
sig::sig_signal_req::operator()(sig_signal_t s) const {
  // non-member functions
  if (this->cb)
    this->cb(s);

  // Member functions
  if (this->mem_cb)
    this->mem_cb->operator()(s);
}

unsigned int
get_mapped_uid(CStringPtr signal) {
  unsigned int signal_uid = 0;
  std::map<CStringPtr, unsigned int>::iterator it = string_map.find(signal);
  if (it == string_map.end()) {
    signal_uid = ++s_last_sig_id;
    string_map.insert(std::make_pair(signal, signal_uid));
  } else {
    signal_uid = it->second;
  }
  return signal_uid;
}

template <typename _FuncType>
void
perform_attach(int signal,
               _FuncType cb,
               const sig_context_t * ctx = sig_def_ctx) {
  sig::sig_signal_req * sig_req =
       new sig::sig_signal_req(SIG_REQ_TYPE_INT, ctx, signal, cb);
  sig::signals_reqs.push_back(sig_req);
}

template <typename _FuncType>
void
perform_attach(CStringPtr signal,
               _FuncType cb,
               const sig_context_t * ctx = sig_def_ctx) {
  unsigned int signal_uid = get_mapped_uid(signal);

  sig::sig_signal_req * sig_req =
       new sig::sig_signal_req(SIG_REQ_TYPE_STR, ctx, signal_uid, cb);
  sig::signals_reqs.push_back(sig_req);
}

void
perform_detach(int signal,
               sig_observer_cb_t cb,
               const sig_context_t * ctx = sig_def_ctx) {
  std::vector<sig_signal_req *>::iterator it = sig::signals_reqs.begin();
  for (; it != sig::signals_reqs.end(); it++) {
     sig_signal_req * sig_req = *it;
     if (!sig_req->cb)
       continue;

     if (sig_req->req_type == SIG_REQ_TYPE_INT
         && sig_req->ctx->ctx_id == ctx->ctx_id
         && *sig_req == signal
         && sig_req->cb == cb) {

        // free resources
        delete sig_req;
        sig::signals_reqs.erase(it);
        it--;
     }
  }
}

void
perform_detach(int signal,
               sig_observer_cb2_t cb,
               const sig_context_t * ctx = sig_def_ctx) {
  std::vector<sig_signal_req *>::iterator it = sig::signals_reqs.begin();
  for (; it != sig::signals_reqs.end(); it++) {
     sig_signal_req * sig_req = *it;
     if (!sig_req->mem_cb)
       continue;

     if (sig_req->req_type == SIG_REQ_TYPE_INT
         && sig_req->ctx->ctx_id == ctx->ctx_id
         && *sig_req == signal
         && *sig_req->mem_cb == cb) {

        // free resources
        delete sig_req;
        sig::signals_reqs.erase(it);
        it--;
     }
  }
}

void
perform_detach(CStringPtr signal,
               sig_observer_cb_t cb,
               const sig_context_t * ctx = sig_def_ctx) {
  unsigned int signal_uid = get_mapped_uid(signal);

  std::vector<sig_signal_req *>::iterator it = sig::signals_reqs.begin();
  for (; it != sig::signals_reqs.end(); it++) {
     sig_signal_req * sig_req = *it;
     if (!sig_req->cb)
       continue;

     if (sig_req->req_type == SIG_REQ_TYPE_STR
         && sig_req->ctx->ctx_id == ctx->ctx_id
         && *sig_req == signal_uid
         && sig_req->cb == cb) {

        // free resources
        delete sig_req;
        sig::signals_reqs.erase(it);
        it--;
     }
  }
}

void
perform_detach(CStringPtr signal,
               sig_observer_cb2_t cb,
               const sig_context_t * ctx = sig_def_ctx) {
  unsigned int signal_uid = get_mapped_uid(signal);

  std::vector<sig_signal_req *>::iterator it = sig::signals_reqs.begin();
  for (; it != sig::signals_reqs.end(); it++) {
     sig_signal_req * sig_req = *it;
     if (!sig_req->mem_cb) // prevent comparing fn-ptr and mem-fn-ptr
       continue;

     if (sig_req->req_type == SIG_REQ_TYPE_STR
         && sig_req->ctx->ctx_id == ctx->ctx_id
         && *sig_req == signal_uid
         && *sig_req->mem_cb == cb) {

        // free resources
        delete sig_req;
        sig::signals_reqs.erase(it);
        it--;
     }
  }
}

void
perform_detach_memfn(void * observer,
                     int signal = 0,
                     const sig_context_t * ctx = NULL) {
  std::vector<sig_signal_req *>::iterator it = sig::signals_reqs.begin();
  for (; it != sig::signals_reqs.end(); it++) {
     sig_signal_req * sig_req = *it;
     if (!sig_req->mem_cb)
       continue;

     if (sig_req->mem_cb->get_obj_addr() == observer
         && sig_req->req_type == SIG_REQ_TYPE_INT
         && (ctx == NULL || sig_req->ctx->ctx_id == ctx->ctx_id)
         && (signal == 0 || *sig_req == signal)) {

        // free resources
        delete sig_req;
        sig::signals_reqs.erase(it);
        it--;
     }
  }
}

void
perform_detach_memfn2(void * observer,
                      CStringPtr signal,
                      const sig_context_t * ctx = NULL) {
  unsigned int signal_uid = get_mapped_uid(signal);

  std::vector<sig_signal_req *>::iterator it = sig::signals_reqs.begin();
  for (; it != sig::signals_reqs.end(); it++) {
     sig_signal_req * sig_req = *it;
     if (!sig_req->mem_cb)
       continue;

     if (sig_req->mem_cb->get_obj_addr() == observer
         && sig_req->req_type == SIG_REQ_TYPE_STR
         && (ctx == NULL || sig_req->ctx->ctx_id == ctx->ctx_id)
         && *sig_req == signal_uid) {

        // free resources
        delete sig_req;
        sig::signals_reqs.erase(it);
        it--;
     }
  }
}

void
perform_fire(int signal,
             void * object,
             const sig_context_t * ctx = sig_def_ctx) {
  std::vector<sig_signal_req *>::iterator it = sig::signals_reqs.begin();
  for (; it != sig::signals_reqs.end(); it++) {
     sig_signal_req * sig_req = *it;
     if (sig_req->req_type == SIG_REQ_TYPE_INT
         && sig_req->ctx->ctx_id == ctx->ctx_id
         && *sig_req == signal) {
       sig_signal_t signal_object(object,
                                  ctx,
                                  object);
       (*sig_req)(signal_object);
     }
  }
}

void
perform_fire(CStringPtr signal,
             void * object,
             const sig_context_t * ctx = sig_def_ctx) {
  unsigned int signal_uid = get_mapped_uid(signal);

  std::vector<sig_signal_req *>::iterator it = sig::signals_reqs.begin();
  for (; it != sig::signals_reqs.end(); it++) {
     sig_signal_req * sig_req = *it;
     if (sig_req->req_type == SIG_REQ_TYPE_STR
         && sig_req->ctx->ctx_id == ctx->ctx_id
         && *sig_req == signal_uid) {
       sig_signal_t signal_object(object,
                                  ctx,
                                  object);
       (*sig_req)(signal_object);
     }
  }
}

} // namespace sig

sig_context_s::sig_context_s(int id)
  : ctx_id(id) { }

enum sig_reserved_context_id {
  kSigReservedContextId_Default = 0,
  kSigReservedContextId_Sys     = 1
};


const sig_context_t *
sig_ctx_default() {
  static  sig_context_t * __sig_default_ctx;
  if (!__sig_default_ctx) {
    __sig_default_ctx = new sig_context_t(kSigReservedContextId_Default);
  }
  return __sig_default_ctx;
}

const sig_context_t *
sig_ctx_sys() {
  static  sig_context_t * __sig_sys_ctx;
  if (!__sig_sys_ctx) {
    __sig_sys_ctx = new sig_context_t(kSigReservedContextId_Sys);
  }
  return __sig_sys_ctx;
}

const sig_context_t *
sig_ctx_new() {
  int next_id = ++sig::s_sig_context_last_id;
  sig_context_t * new_ctx = new sig_context_t(next_id);
  return new_ctx;
}

// default contexts

// attach

__SIG_C_DECL void
sig_attach(int signal, sig_observer_cb_t cb) {
  sig::perform_attach(signal, cb);
}

void
sig_attach(int signal, sig_observer_cb2_t cb) {
  sig::perform_attach(signal, cb);
}

void
sig_attach(const char * signal, sig_observer_cb_t cb) {
  sig::perform_attach(signal, cb);
}

void
sig_attach(const char * signal, sig_observer_cb2_t cb) {
  sig::perform_attach(signal, cb);
}

__SIG_C_DECL void
sig_attach_s(const char * signal, sig_observer_cb_t cb) {
  sig::perform_attach(signal, cb);
}

// fire

__SIG_C_DECL void
sig_fire(int signal, void * object) {
  sig::perform_fire(signal, object);
}

void
sig_fire(const char * signal, void * object) {
  sig::perform_fire(signal, object);
}

__SIG_C_DECL void
sig_fire_s(const char * signal, void * object) {
  sig::perform_fire(signal, object);
}

// detach

__SIG_C_DECL void
sig_detach(int signal, sig_observer_cb_t cb) {
  sig::perform_detach(signal, cb);
}

void
sig_detach(int signal, sig_observer_cb2_t cb) {
  sig::perform_detach(signal, cb);
}

void
sig_detach(const char * signal, sig_observer_cb_t cb) {
  sig::perform_detach(signal, cb);
}

void
sig_detach(const char * signal, sig_observer_cb2_t cb) {
  sig::perform_detach(signal, cb);
}

__SIG_C_DECL void
sig_detach_s(const char * signal, sig_observer_cb_t cb) {
  sig::perform_detach(signal, cb);
}

void
sig_detach(void * observer) {
  sig::perform_detach_memfn(observer);
}

void
sig_detach(int signal, void * observer) {
  sig::perform_detach_memfn(observer, signal);
}

void
sig_detach(const char * signal, void * observer) {
  sig::perform_detach_memfn2(observer, signal);
}

// custom contexts

// attach

__SIG_C_DECL void
sig_attachc(int signal,
            sig_observer_cb_t cb,
            const sig_context_t * ctx) {
  sig::perform_attach(signal, cb, ctx);
}

void
sig_attach(int signal,
           sig_observer_cb_t cb,
           const sig_context_t * ctx) {
  sig::perform_attach(signal, cb, ctx);
}

void
sig_attach(int signal,
           sig_observer_cb2_t cb,
           const sig_context_t * ctx) {
  sig::perform_attach(signal, cb, ctx);
}

void
sig_attach(const char * signal,
           sig_observer_cb_t cb,
           const sig_context_t * ctx) {
  sig::perform_attach(signal, cb, ctx);
}

void
sig_attach(const char * signal,
           sig_observer_cb2_t cb,
           const sig_context_t * ctx) {
  sig::perform_attach(signal, cb, ctx);
}

__SIG_C_DECL void
sig_attachc_s(const char * signal,
              sig_observer_cb_t cb,
              const sig_context_t * ctx) {
  sig::perform_attach(signal, cb, ctx);
}

// fire

__SIG_C_DECL void
sig_firec(int signal,
          void * object,
          const sig_context_t * ctx) {
  sig::perform_fire(signal, object, ctx);
}

void
sig_fire(const char * signal,
         void * object,
         const sig_context_t * ctx) {
  sig::perform_fire(signal, object, ctx);
}

__SIG_C_DECL void
sig_firec_s(const char * signal,
            void * object,
            const sig_context_t * ctx) {
  sig::perform_fire(signal, object, ctx);
}

// detach

__SIG_C_DECL void
sig_detachc(int signal,
            sig_observer_cb_t cb,
            const sig_context_t * ctx) {
  sig::perform_detach(signal, cb);
}

void
sig_detach(int signal,
           sig_observer_cb_t cb,
           const sig_context_t * ctx) {
  sig::perform_detach(signal, cb);
}

void
sig_detach(int signal,
           sig_observer_cb2_t cb,
           const sig_context_t * ctx) {
  sig::perform_detach(signal, cb);
}

void
sig_detach(const char * signal,
           sig_observer_cb_t cb,
           const sig_context_t * ctx) {
  sig::perform_detach(signal, cb);
}

void
sig_detach(const char * signal,
           sig_observer_cb2_t cb,
           const sig_context_t * ctx) {
  sig::perform_detach(signal, cb);
}

__SIG_C_DECL void
sig_detachc_s(int signal,
              sig_observer_cb_t cb,
              const sig_context_t * ctx) {
  sig::perform_detach(signal, cb);
}

void
sig_detach(void * observer,
           const sig_context_t * ctx) {
  sig::perform_detach_memfn(observer, 0, ctx);
}

void 
sig_detach(int signal,
           void * observer,
           const sig_context_t * ctx) {
  sig::perform_detach_memfn(observer, signal, ctx);
}

void
sig_detach(const char * signal,
           void * observer,
           const sig_context_t * ctx) {
  sig::perform_detach_memfn2(observer, signal, ctx);
}
