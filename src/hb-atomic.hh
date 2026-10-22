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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-meta.hh"


#ifndef HB_NO_MT
#ಅನ್ತರ್ಭಾವ <atomic>
#endif


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ifndef _hb_compiler_memory_r_barrier
#ಯದಿ defined(__ATOMIC_ACQUIRE) // gcc-like
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ _hb_compiler_memory_r_barrier () { asm ಚಞ್ಚಲ("": : :"memory"); }
#ನೋಚೇದ್ಯದಿ !defined(_MSC_VER)
#ಅನ್ತರ್ಭಾವ <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

template <typename T>
ಸಂರಚನಾ hb_atomic_t
{
  hb_atomic_t () = ಸ್ವಭಾವ;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ಧ್ರುವ hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &operator= (ಧ್ರುವ hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  hb_atomic_t &operator= (T v_)
  {
    set_relaxed (v_);
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }
  operator T () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_relaxed (); }

  ಶೂನ್ಯ set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  ಶೂನ್ಯ set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v.load (std::memory_order_relaxed); }
  T get_acquire () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v.load (std::memory_order_acquire); }
  T inc () { ಪ್ರತ್ಯಾಗಮ v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { ಪ್ರತ್ಯಾಗಮ v.fetch_add (-1, std::memory_order_acq_rel); }

  ಪೂರ್ಣಾಙ್ಕ operator++ (ಪೂರ್ಣಾಙ್ಕ) { ಪ್ರತ್ಯಾಗಮ inc (); }
  ಪೂರ್ಣಾಙ್ಕ operator-- (ಪೂರ್ಣಾಙ್ಕ) { ಪ್ರತ್ಯಾಗಮ dec (); }

  friend ಶೂನ್ಯ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

template <typename T>
ಸಂರಚನಾ hb_atomic_t<T *>
{
  hb_atomic_t () = ಸ್ವಭಾವ;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ಧ್ರುವ hb_atomic_t &other) = delete;

  ಶೂನ್ಯ init (T *v_ = nullptr) { set_relaxed (v_); }
  ಶೂನ್ಯ set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v.load (std::memory_order_relaxed); }
  T *get_acquire () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v.load (std::memory_order_acquire); }
  ತಾರ್ಕಿಕ cmpexch (T *old, T *new_) { ಪ್ರತ್ಯಾಗಮ v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &operator= (ಧ್ರುವ hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_acquire () != nullptr; }
  T *operator->() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_acquire (); }
  template <typename C>
  operator C * () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ get_acquire ();
  }

  friend ಶೂನ್ಯ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#ಅನ್ಯಥಾ /* defined(HB_NO_MT) */

template <typename T>
ಸಂರಚನಾ hb_atomic_t
{
  hb_atomic_t () = ಸ್ವಭಾವ;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& operator = (T v_) { set_relaxed (v_); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  operator T () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_relaxed (); }

  ಶೂನ್ಯ set_relaxed (T v_) { v = v_; }
  ಶೂನ್ಯ set_release (T v_) { v = v_; }
  T get_relaxed () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
  T get_acquire () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
  T inc () { ಪ್ರತ್ಯಾಗಮ v++; }
  T dec () { ಪ್ರತ್ಯಾಗಮ v--; }

  ಪೂರ್ಣಾಙ್ಕ operator ++ (ಪೂರ್ಣಾಙ್ಕ) { ಪ್ರತ್ಯಾಗಮ inc (); }
  ಪೂರ್ಣಾಙ್ಕ operator -- (ಪೂರ್ಣಾಙ್ಕ) { ಪ್ರತ್ಯಾಗಮ dec (); }

  T v = 0;
};

template <typename T>
ಸಂರಚನಾ hb_atomic_t<T*>
{
  hb_atomic_t () = ಸ್ವಭಾವ;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ಧ್ರುವ hb_atomic_t &other) = delete;

  ಶೂನ್ಯ init (T* v_ = nullptr) { set_relaxed (v_); }
  ಶೂನ್ಯ set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
  T *get_acquire () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
  ತಾರ್ಕಿಕ cmpexch (T *old, T *new_)
  {
    ಯದಿ (v != old) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    v = new_;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_acquire () != nullptr; }
  T * operator -> () ಧ್ರುವ                    { ಪ್ರತ್ಯಾಗಮ get_acquire (); }
  template <typename C> operator C * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_acquire (); }

  T *v = nullptr;
};

#endif


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}


#endif /* HB_ATOMIC_HH */
