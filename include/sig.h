/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libsig__signal__h_
#define __libsig__signal__h_

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

#define SIG_STATUS_T int
#define SIG_STATUS_SUCCESS 0
#define SIG_STATUS_FAILURE 1

#ifdef __cplusplus
struct sig_mem_observer_base_t {
  virtual void operator()(const sig_signal_t signal) const = 0;
  virtual bool operator==(sig_mem_observer_base_t * o) const = 0;
  virtual void * get_obj_addr() const = 0;
  virtual ~sig_mem_observer_base_t() { };
};

template <typename T>
sig_mem_observer_base_t *
sig_make_mem_observer(T * inst, void (T::*memFn)(const sig_signal_t signal)) {
  struct sig_mem_observer_t : public sig_mem_observer_base_t {
    T * m_inst;

    void (T::*m_cb)(const sig_signal_t signal);
    void * get_obj_addr() const {
      return m_inst;
    }

    void operator()(const sig_signal_t signal) const {
      (m_inst->*m_cb)(signal);
    }

    bool operator==(sig_mem_observer_base_t * o) const {
      sig_mem_observer_t * observer2 = static_cast<sig_mem_observer_t *>(o);
      return m_inst == observer2->m_inst
          && m_cb == observer2->m_cb;
    }

    ~sig_mem_observer_t(){ }
  };

  sig_mem_observer_t * _observer = new sig_mem_observer_t;
  _observer->m_inst = inst;
  _observer->m_cb = memFn;

  return _observer;
}

typedef sig_mem_observer_base_t * sig_observer_cb2_t;

#define sig_slot(t, f) sig_make_mem_observer(t, f)

#endif

typedef void (*sig_observer_cb_t)(const sig_signal_t signal);

/**
 * Attach a non-function or member-function to event
 */
__SIG_C_DECL void sig_attach(int signal, sig_observer_cb_t cb);

#ifdef __cplusplus
// Default context
void sig_attach(int signal, sig_observer_cb2_t cb);
void sig_attach(const char * signal, sig_observer_cb_t cb);
void sig_attach(const char * signal, sig_observer_cb2_t cb);

// Attach by given context
void sig_attach(int signal,
                sig_observer_cb_t cb,
                const sig_context_t * ctx);
void sig_attach(int signal,
                sig_observer_cb2_t cb,
                const sig_context_t * ctx);
void sig_attach(const char * signal,
                sig_observer_cb_t cb,
                const sig_context_t * ctx);
void sig_attach(const char * signal,
                sig_observer_cb2_t cb,
                const sig_context_t * ctx);
#else
__SIG_C_DECL void sig_attachc(int signal,
                              sig_observer_cb_t cb,
                              const sig_context_t * ctx);
__SIG_C_DECL void sig_attach_s(const char * signal, sig_observer_cb_t cb);
__SIG_C_DECL void sig_attachc_s(const char * signal,
                                sig_observer_cb_t cb,
                                const sig_context_t * ctx);
#endif

/**
 * Detach a non-function or member-function to event
 */
__SIG_C_DECL void sig_detach(int signal, sig_observer_cb_t cb);

#ifdef __cplusplus
// Default context
void sig_detach(int signal, sig_observer_cb2_t cb);
void sig_detach(const char * signal, sig_observer_cb_t cb);
void sig_detach(const char * signal, sig_observer_cb2_t cb);

// Default by given context
void sig_detach(int signal,
                sig_observer_cb_t cb,
                const sig_context_t * ctx);
void sig_detach(int signal,
                sig_observer_cb2_t cb,
                const sig_context_t * ctx);
void sig_detach(const char * signal,
                sig_observer_cb2_t cb,
                const sig_context_t * ctx);
void sig_detach(const char * signal,
                sig_observer_cb2_t cb,
                const sig_context_t * ctx);
#else
__SIG_C_DECL void sig_detachc(int signal,
                              sig_observer_cb_t cb,
                              const sig_context_t * ctx);
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
              const sig_context_t * ctx);
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
