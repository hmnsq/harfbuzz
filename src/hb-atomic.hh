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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-meta.hh"


#ifndef HB_NO_MT
#അന്തര്ഭാവ <atomic>
#endif


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ifndef _hb_compiler_memory_r_barrier
#എങ്കിൽ defined(__ATOMIC_ACQUIRE) // gcc-like
സ്ഥിതം വരിയിൽ ശൂന്യം _hb_compiler_memory_r_barrier () { asm അസ്ഥിര("": : :"memory"); }
#അല്ലെങ്കിലെങ്കിൽ !defined(_MSC_VER)
#അന്തര്ഭാവ <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ ശൂന്യം _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

template <typename T>
ഘടന hb_atomic_t
{
  hb_atomic_t () = സ്ഥിരസ്ഥിതി;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (സ്ഥിരം hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &operator= (സ്ഥിരം hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); തിരികെ *ഇത്; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); തിരികെ *ഇത്; }
  hb_atomic_t &operator= (T v_)
  {
    set_relaxed (v_);
    തിരികെ *ഇത്;
  }
  operator T () സ്ഥിരം { തിരികെ get_relaxed (); }

  ശൂന്യം set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  ശൂന്യം set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () സ്ഥിരം { തിരികെ v.load (std::memory_order_relaxed); }
  T get_acquire () സ്ഥിരം { തിരികെ v.load (std::memory_order_acquire); }
  T inc () { തിരികെ v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { തിരികെ v.fetch_add (-1, std::memory_order_acq_rel); }

  പൂർണ്ണസംഖ്യ operator++ (പൂർണ്ണസംഖ്യ) { തിരികെ inc (); }
  പൂർണ്ണസംഖ്യ operator-- (പൂർണ്ണസംഖ്യ) { തിരികെ dec (); }

  friend ശൂന്യം swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

template <typename T>
ഘടന hb_atomic_t<T *>
{
  hb_atomic_t () = സ്ഥിരസ്ഥിതി;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (സ്ഥിരം hb_atomic_t &other) = delete;

  ശൂന്യം init (T *v_ = nullptr) { set_relaxed (v_); }
  ശൂന്യം set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () സ്ഥിരം { തിരികെ v.load (std::memory_order_relaxed); }
  T *get_acquire () സ്ഥിരം { തിരികെ v.load (std::memory_order_acquire); }
  തർക്കം cmpexch (T *old, T *new_) { തിരികെ v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &operator= (സ്ഥിരം hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); തിരികെ *ഇത്; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); തിരികെ *ഇത്; }

  operator തർക്കം () സ്ഥിരം { തിരികെ get_acquire () != nullptr; }
  T *operator->() സ്ഥിരം { തിരികെ get_acquire (); }
  template <typename C>
  operator C * () സ്ഥിരം
  {
    തിരികെ get_acquire ();
  }

  friend ശൂന്യം swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#അല്ലെങ്കിൽ /* defined(HB_NO_MT) */

template <typename T>
ഘടന hb_atomic_t
{
  hb_atomic_t () = സ്ഥിരസ്ഥിതി;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& operator = (T v_) { set_relaxed (v_); തിരികെ *ഇത്; }
  operator T () സ്ഥിരം { തിരികെ get_relaxed (); }

  ശൂന്യം set_relaxed (T v_) { v = v_; }
  ശൂന്യം set_release (T v_) { v = v_; }
  T get_relaxed () സ്ഥിരം { തിരികെ v; }
  T get_acquire () സ്ഥിരം { തിരികെ v; }
  T inc () { തിരികെ v++; }
  T dec () { തിരികെ v--; }

  പൂർണ്ണസംഖ്യ operator ++ (പൂർണ്ണസംഖ്യ) { തിരികെ inc (); }
  പൂർണ്ണസംഖ്യ operator -- (പൂർണ്ണസംഖ്യ) { തിരികെ dec (); }

  T v = 0;
};

template <typename T>
ഘടന hb_atomic_t<T*>
{
  hb_atomic_t () = സ്ഥിരസ്ഥിതി;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (സ്ഥിരം hb_atomic_t &other) = delete;

  ശൂന്യം init (T* v_ = nullptr) { set_relaxed (v_); }
  ശൂന്യം set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () സ്ഥിരം { തിരികെ v; }
  T *get_acquire () സ്ഥിരം { തിരികെ v; }
  തർക്കം cmpexch (T *old, T *new_)
  {
    എങ്കിൽ (v != old) തിരികെ അസത്യം;
    v = new_;
    തിരികെ സത്യം;
  }

  operator തർക്കം () സ്ഥിരം { തിരികെ get_acquire () != nullptr; }
  T * operator -> () സ്ഥിരം                    { തിരികെ get_acquire (); }
  template <typename C> operator C * () സ്ഥിരം { തിരികെ get_acquire (); }

  T *v = nullptr;
};

#endif


സ്ഥിതം വരിയിൽ തർക്കം hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  തിരികെ സത്യം;
}


#endif /* HB_ATOMIC_HH */
