/* Copyright (c) 2014, Recep Aslantas <info@recp.me>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef libsig_signal_h
#define libsig_signal_h

#ifdef __cplusplus
#include <functional>
#endif

#ifdef __cplusplus
#define __SIG_C_DECL extern "C"
#else
#define __SIG_C_DECL
#endif

typedef const void * const sig_signal_id_t;
typedef void * sig_signal_object_t;
typedef struct sig_signal_s sig_signal_t;
typedef struct sig_signal_context_s sig_signal_context_t;

struct sig_signal_context_s {
  int ctx_id;
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
  sig_signal_s(sig_signal_id_t _signal_id, const sig_signal_context_t * context, const  sig_signal_object_t object)
    : signal_id(_signal_id)  { }
#endif
};

#ifdef __cplusplus
typedef std::function<void (const sig_signal_t * signal)> sig_observer_cb;
#else
typedef void (*sig_observer_cb)(const sig_signal_t * signal);
#endif

#ifdef __cplusplus
#define SIG_CB_DEFINER(cb_type) const cb_type& /* Lvalue reference; TODO: This can be change  */
#else
#define SIG_CB_DEFINER(cb_type) cb_type 
#endif

typedef SIG_CB_DEFINER(sig_observer_cb) sig_observer_cb_t;

#define SIG_STATUS_T int
#define SIG_STATUS_SUCCESS 0
#define SIG_STATUS_FAILURE 1

/**
 * Attach a non-function or member-function to event
 */ 
__SIG_C_DECL  void sig_attach(int signal, sig_observer_cb_t cb);

#ifdef __cplusplus
void sig_attach(const char * signal, sig_observer_cb_t cb);
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
    virtual attach_stream_base_t & operator << (sig_observer_cb_t cb) const;
    virtual attach_stream_base_t & operator >> (sig_observer_cb_t cb) const;
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
};
#endif

#endif
