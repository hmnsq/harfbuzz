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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-meta.hh"


#ifndef HB_NO_MT
#অন্তর্ভাব <atomic>
#endif


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ifndef _hb_compiler_memory_r_barrier
#যদি defined(__ATOMIC_ACQUIRE) // gcc-like
স্থির অন্তঃরেখ শূন্য _hb_compiler_memory_r_barrier () { asm অস্থির("": : :"memory"); }
#নতুবা_যদি !defined(_MSC_VER)
#অন্তর্ভাব <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#অন্যথা
স্থির অন্তঃরেখ শূন্য _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

template <typename T>
কাঠামো hb_atomic_t
{
  hb_atomic_t () = পূর্বনির্ধারিত;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ধ্রুবক hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &operator= (ধ্রুবক hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); ফেরত *এই; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); ফেরত *এই; }
  hb_atomic_t &operator= (T v_)
  {
    set_relaxed (v_);
    ফেরত *এই;
  }
  operator T () ধ্রুবক { ফেরত get_relaxed (); }

  শূন্য set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  শূন্য set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ধ্রুবক { ফেরত v.load (std::memory_order_relaxed); }
  T get_acquire () ধ্রুবক { ফেরত v.load (std::memory_order_acquire); }
  T inc () { ফেরত v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { ফেরত v.fetch_add (-1, std::memory_order_acq_rel); }

  পূর্ণসংখ্যা operator++ (পূর্ণসংখ্যা) { ফেরত inc (); }
  পূর্ণসংখ্যা operator-- (পূর্ণসংখ্যা) { ফেরত dec (); }

  friend শূন্য swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

template <typename T>
কাঠামো hb_atomic_t<T *>
{
  hb_atomic_t () = পূর্বনির্ধারিত;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ধ্রুবক hb_atomic_t &other) = delete;

  শূন্য init (T *v_ = nullptr) { set_relaxed (v_); }
  শূন্য set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ধ্রুবক { ফেরত v.load (std::memory_order_relaxed); }
  T *get_acquire () ধ্রুবক { ফেরত v.load (std::memory_order_acquire); }
  যৌক্তিক cmpexch (T *old, T *new_) { ফেরত v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &operator= (ধ্রুবক hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); ফেরত *এই; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); ফেরত *এই; }

  operator যৌক্তিক () ধ্রুবক { ফেরত get_acquire () != nullptr; }
  T *operator->() ধ্রুবক { ফেরত get_acquire (); }
  template <typename C>
  operator C * () ধ্রুবক
  {
    ফেরত get_acquire ();
  }

  friend শূন্য swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#অন্যথা /* defined(HB_NO_MT) */

template <typename T>
কাঠামো hb_atomic_t
{
  hb_atomic_t () = পূর্বনির্ধারিত;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& operator = (T v_) { set_relaxed (v_); ফেরত *এই; }
  operator T () ধ্রুবক { ফেরত get_relaxed (); }

  শূন্য set_relaxed (T v_) { v = v_; }
  শূন্য set_release (T v_) { v = v_; }
  T get_relaxed () ধ্রুবক { ফেরত v; }
  T get_acquire () ধ্রুবক { ফেরত v; }
  T inc () { ফেরত v++; }
  T dec () { ফেরত v--; }

  পূর্ণসংখ্যা operator ++ (পূর্ণসংখ্যা) { ফেরত inc (); }
  পূর্ণসংখ্যা operator -- (পূর্ণসংখ্যা) { ফেরত dec (); }

  T v = 0;
};

template <typename T>
কাঠামো hb_atomic_t<T*>
{
  hb_atomic_t () = পূর্বনির্ধারিত;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ধ্রুবক hb_atomic_t &other) = delete;

  শূন্য init (T* v_ = nullptr) { set_relaxed (v_); }
  শূন্য set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ধ্রুবক { ফেরত v; }
  T *get_acquire () ধ্রুবক { ফেরত v; }
  যৌক্তিক cmpexch (T *old, T *new_)
  {
    যদি (v != old) ফেরত মিথ্যা;
    v = new_;
    ফেরত সত্য;
  }

  operator যৌক্তিক () ধ্রুবক { ফেরত get_acquire () != nullptr; }
  T * operator -> () ধ্রুবক                    { ফেরত get_acquire (); }
  template <typename C> operator C * () ধ্রুবক { ফেরত get_acquire (); }

  T *v = nullptr;
};

#endif


স্থির অন্তঃরেখ যৌক্তিক hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  ফেরত সত্য;
}


#endif /* HB_ATOMIC_HH */
