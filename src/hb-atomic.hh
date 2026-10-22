/*
 * Copyright © 2007  Chris Wilson
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Contributor(s):
 *	Chris Wilson <chris@chris-wilson.co.uk>
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_ATOMIC_HH
#define HB_ATOMIC_HH

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-meta.hh"


#ifndef HB_NO_MT
#અન્તર્ભાવ <atomic>
#endif


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ifndef _hb_compiler_memory_r_barrier
#જો defined(__ATOMIC_ACQUIRE) // gcc-like
સ્થિર અંતઃરેખ શૂન્ય _hb_compiler_memory_r_barrier () { asm અસ્થિર("": : :"memory"); }
#અથવાજો !defined(_MSC_VER)
#અન્તર્ભાવ <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#નહીંતર
સ્થિર અંતઃરેખ શૂન્ય _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

template <typename T>
સંરચના hb_atomic_t
{
  hb_atomic_t () = મૂળભૂત;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (અચલ hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &operator= (અચલ hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); પાછા *આ; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); પાછા *આ; }
  hb_atomic_t &operator= (T v_)
  {
    set_relaxed (v_);
    પાછા *આ;
  }
  operator T () અચલ { પાછા get_relaxed (); }

  શૂન્ય set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  શૂન્ય set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () અચલ { પાછા v.load (std::memory_order_relaxed); }
  T get_acquire () અચલ { પાછા v.load (std::memory_order_acquire); }
  T inc () { પાછા v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { પાછા v.fetch_add (-1, std::memory_order_acq_rel); }

  પૂર્ણાંક operator++ (પૂર્ણાંક) { પાછા inc (); }
  પૂર્ણાંક operator-- (પૂર્ણાંક) { પાછા dec (); }

  friend શૂન્ય swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

template <typename T>
સંરચના hb_atomic_t<T *>
{
  hb_atomic_t () = મૂળભૂત;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (અચલ hb_atomic_t &other) = delete;

  શૂન્ય init (T *v_ = nullptr) { set_relaxed (v_); }
  શૂન્ય set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () અચલ { પાછા v.load (std::memory_order_relaxed); }
  T *get_acquire () અચલ { પાછા v.load (std::memory_order_acquire); }
  તાર્કિક cmpexch (T *old, T *new_) { પાછા v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &operator= (અચલ hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); પાછા *આ; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); પાછા *આ; }

  operator તાર્કિક () અચલ { પાછા get_acquire () != nullptr; }
  T *operator->() અચલ { પાછા get_acquire (); }
  template <typename C>
  operator C * () અચલ
  {
    પાછા get_acquire ();
  }

  friend શૂન્ય swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#નહીંતર /* defined(HB_NO_MT) */

template <typename T>
સંરચના hb_atomic_t
{
  hb_atomic_t () = મૂળભૂત;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& operator = (T v_) { set_relaxed (v_); પાછા *આ; }
  operator T () અચલ { પાછા get_relaxed (); }

  શૂન્ય set_relaxed (T v_) { v = v_; }
  શૂન્ય set_release (T v_) { v = v_; }
  T get_relaxed () અચલ { પાછા v; }
  T get_acquire () અચલ { પાછા v; }
  T inc () { પાછા v++; }
  T dec () { પાછા v--; }

  પૂર્ણાંક operator ++ (પૂર્ણાંક) { પાછા inc (); }
  પૂર્ણાંક operator -- (પૂર્ણાંક) { પાછા dec (); }

  T v = 0;
};

template <typename T>
સંરચના hb_atomic_t<T*>
{
  hb_atomic_t () = મૂળભૂત;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (અચલ hb_atomic_t &other) = delete;

  શૂન્ય init (T* v_ = nullptr) { set_relaxed (v_); }
  શૂન્ય set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () અચલ { પાછા v; }
  T *get_acquire () અચલ { પાછા v; }
  તાર્કિક cmpexch (T *old, T *new_)
  {
    જો (v != old) પાછા ખોટું;
    v = new_;
    પાછા સાચું;
  }

  operator તાર્કિક () અચલ { પાછા get_acquire () != nullptr; }
  T * operator -> () અચલ                    { પાછા get_acquire (); }
  template <typename C> operator C * () અચલ { પાછા get_acquire (); }

  T *v = nullptr;
};

#endif


સ્થિર અંતઃરેખ તાર્કિક hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  પાછા સાચું;
}


#endif /* HB_ATOMIC_HH */
