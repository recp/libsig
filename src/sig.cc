/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "sig.h"
#include "sig-ctx.h"

__SIG_C_DECL void sig_attach(int signal, sig_observer_cb_t cb) {
  sig::__sig_def_ctx_mngr.add(signal, cb);
}

void sig_attach(const char * signal, sig_observer_cb_t cb) {
  sig::__sig_def_ctx_mngr_s.add(signal, cb);
}

__SIG_C_DECL void sig_attach_s(const char * signal, sig_observer_cb_t cb) {
  sig::__sig_def_ctx_mngr_s.add(signal, cb);
}

__SIG_C_DECL void sig_fire(int signal, void * object) {
  sig::__sig_def_ctx_mngr.fire(signal, object);
}

void sig_fire(const char * signal, void * object) {
  sig::__sig_def_ctx_mngr_s.fire(signal, object);
}

__SIG_C_DECL void sig_fire_s(const char * signal, void * object) {
  sig::__sig_def_ctx_mngr_s.fire(signal, object);
}
