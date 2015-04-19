/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libsig__signal__h_
#define __libsig__signal__h_

#ifdef __cplusplus
#  include <functional>
#endif

#ifdef __cplusplus
#  define __SIG_C_DECL extern "C"
#else
#  define __SIG_C_DECL
#endif

typedef const void * const sig_signal_id_t;
typedef void * sig_signal_object_t;
typedef struct sig_signal_s sig_signal_t;
typedef struct sig_signal_context_s sig_signal_context_t;

struct sig_signal_context_s {
  const int ctx_id;
#ifdef __cplusplus
  sig_signal_context_s(int id);
#endif
};

const sig_signal_context_t * sig_ctx_new();

/* Predefined signal contexts */
extern const sig_signal_context_t * sig_ctx_default();
extern const sig_signal_context_t * sig_ctx_sys();

struct sig_signal_s {
  sig_signal_id_t signal_id;
  sig_signal_object_t object;
  sig_signal_context_t * context;
#ifdef __cplusplus
  sig_signal_s(sig_signal_id_t _signal_id,
               const sig_signal_context_t * context,
               const  sig_signal_object_t object)
    : signal_id(_signal_id)  { }
#endif
};

typedef void (*sig_observer_cb_t)(const sig_signal_t signal);

#ifdef __cplusplus
typedef std::function<void (const sig_signal_t signal)> sig_observer_cb2_t;
#endif

#define SIG_STATUS_T int
#define SIG_STATUS_SUCCESS 0
#define SIG_STATUS_FAILURE 1

/**
 * Attach a non-function or member-function to event
 */
__SIG_C_DECL void sig_attach(int signal, sig_observer_cb_t cb);

#ifdef __cplusplus
void sig_attach(int signal, sig_observer_cb2_t cb);
void sig_attach(const char * signal, sig_observer_cb2_t cb);
#else
__SIG_C_DECL void sig_attach_s(const char * signal, sig_observer_cb_t cb);
#endif

/**
 * Fire an event by given name and object
 */
__SIG_C_DECL void sig_fire(int signal, void * object);

#ifdef __cplusplus
void sig_fire(const char * signal, void * object);
#else
__SIG_C_DECL void sig_fire_s(const char * signal, void * object);
#endif

#ifdef __cplusplus
namespace sig {

struct attach_stream_base_t {
  virtual attach_stream_base_t & operator << (sig_observer_cb2_t cb) const;
  virtual attach_stream_base_t & operator >> (sig_observer_cb2_t cb) const;
};

struct attach_stream_signal_base_t {
  attach_stream_base_t operator [] (const char * signal) const;
  attach_stream_base_t operator [] (int signal) const;
};

struct fire_stream_base_t {
  fire_stream_base_t & operator << (void * object) const;
};

struct fire_stream_signal_base_t {
  fire_stream_base_t operator [] (const char * signal) const;
  fire_stream_base_t operator [] (int signal) const;
};

extern attach_stream_signal_base_t attach;
extern fire_stream_base_t fire;

}; // namespace sig
#endif

#endif /* defined(__libsig__signal__h_) */
