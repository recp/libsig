/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libsig__signal_context__h_
#define __libsig__signal_context__h_

#include <vector>

namespace sig {

typedef const char * CStringPtr;

template <typename T = int>
class context_manager {
public:
  context_manager(const sig_signal_context_t * sig_ctx);
  ~context_manager();

  void add(T signal, sig_observer_cb2_t cb) const;
  void fire(T signal, void * object) const;

  const sig_signal_context_t * ctx() const;
private:
  struct sig_signal_req_t {
    const T m_signal_id;
    const sig_observer_cb2_t m_cb;

    sig_signal_req_t(T signal, sig_observer_cb2_t cb);
    bool operator==(const T& r1) const;

    template <typename ..._Types>
    void operator()(_Types ... _Args) const;
  };

  std::vector<sig_signal_req_t> * m_observers;
  const sig_signal_context_t * m_ctx;
};

extern context_manager<int> __sig_def_ctx_mngr;
extern context_manager<CStringPtr> __sig_def_ctx_mngr_s;

} // namespace sig

#endif /* defined(__libsig__signal_context__h_) */
