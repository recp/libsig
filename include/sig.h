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
typedef struct sig_context_s sig_context_t;

struct sig_context_s {
  const int ctx_id;
#ifdef __cplusplus
  sig_context_s(int id);
#endif
};

const sig_context_t * sig_ctx_new();

/* Predefined signal contexts */
extern const sig_context_t * sig_ctx_default();
extern const sig_context_t * sig_ctx_sys();

struct sig_signal_s {
  sig_signal_id_t signal_id;
  sig_signal_object_t object;
  const sig_context_t * context;
#ifdef __cplusplus
  sig_signal_s(sig_signal_id_t _signal_id,
               const sig_context_t * _context,
               const sig_signal_object_t _object)
    : context(_context),
      signal_id(_signal_id),
      object(_object)  { }
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
__SIG_C_DECL void sig_attachc(int signal,
                              sig_observer_cb_t cb,
                              const sig_context_t * ctx);

#ifdef __cplusplus
// Default context
void sig_attach(int signal, sig_observer_cb2_t cb);
void sig_attach(const char * signal, sig_observer_cb2_t cb);

// Attach by given context
void sig_attach(int signal,
                sig_observer_cb2_t cb,
                const sig_context_t * ctx);
void sig_attach(const char * signal,
                sig_observer_cb2_t cb,
                const sig_context_t * ctx);
#else
__SIG_C_DECL void sig_attach_s(const char * signal, sig_observer_cb_t cb);
__SIG_C_DECL void sig_attachc_s(const char * signal,
                                sig_observer_cb_t cb,
                                const sig_context_t * ctx);
#endif

/**
 * Detach a non-function or member-function to event
 */
__SIG_C_DECL void sig_detach(int signal, sig_observer_cb_t cb);
__SIG_C_DECL void sig_detachc(int signal,
                              sig_observer_cb_t cb,
                              const sig_context_t * ctx);

#ifdef __cplusplus
// Default context
void sig_detach(int signal, sig_observer_cb2_t cb);
void sig_detach(const char * signal, sig_observer_cb2_t cb);

// Default by given context
void sig_detach(int signal,
                sig_observer_cb2_t cb,
                const sig_context_t * ctx);
void sig_detach(const char * signal,
                sig_observer_cb2_t cb,
                const sig_context_t * ctx);
#else
__SIG_C_DECL void sig_detach_s(const char * signal, sig_observer_cb_t cb);
__SIG_C_DECL void sig_detachc_s(int signal,
                                sig_observer_cb_t cb,
                                const sig_context_t * ctx);
#endif

/**
 * Fire an event by given name and object
 */
__SIG_C_DECL void sig_fire(int signal, void * object);
__SIG_C_DECL void sig_firec(int signal,
                            void * object,
                            const sig_context_t * ctx);

#ifdef __cplusplus
void sig_fire(const char * signal, void * object);
void sig_fire(const char * signal,
              void * object,
              sig_context_t * ctx);
#else
__SIG_C_DECL void sig_fire_s(const char * signal, void * object);
__SIG_C_DECL void sig_firec_s(const char * signal,
                              void * object,
                              const sig_context_t * ctx);
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
