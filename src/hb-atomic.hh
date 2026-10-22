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

#شامل "hb.hh"
#شامل "hb-meta.hh"


#ifndef HB_NO_MT
#شامل <atomic>
#endif


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ifndef _hb_compiler_memory_r_barrier
#اگر defined(__ATOMIC_ACQUIRE) // gcc-like
ایستا درونخطی تهی _hb_compiler_memory_r_barrier () { asm فرار("": : :"memory"); }
#وگرنهاگر !defined(_MSC_VER)
#شامل <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#وگرنه
ایستا درونخطی تهی _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

template <typename T>
ساختار hb_atomic_t
{
  hb_atomic_t () = پیشفرض;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ثابت hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &operator= (ثابت hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); بازگشت *این; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); بازگشت *این; }
  hb_atomic_t &operator= (T v_)
  {
    set_relaxed (v_);
    بازگشت *این;
  }
  operator T () ثابت { بازگشت get_relaxed (); }

  تهی set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  تهی set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ثابت { بازگشت v.load (std::memory_order_relaxed); }
  T get_acquire () ثابت { بازگشت v.load (std::memory_order_acquire); }
  T inc () { بازگشت v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { بازگشت v.fetch_add (-1, std::memory_order_acq_rel); }

  صحیح operator++ (صحیح) { بازگشت inc (); }
  صحیح operator-- (صحیح) { بازگشت dec (); }

  friend تهی swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

template <typename T>
ساختار hb_atomic_t<T *>
{
  hb_atomic_t () = پیشفرض;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = delete;

  تهی init (T *v_ = nullptr) { set_relaxed (v_); }
  تهی set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ثابت { بازگشت v.load (std::memory_order_relaxed); }
  T *get_acquire () ثابت { بازگشت v.load (std::memory_order_acquire); }
  منطقی cmpexch (T *old, T *new_) { بازگشت v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &operator= (ثابت hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); بازگشت *این; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); بازگشت *این; }

  operator منطقی () ثابت { بازگشت get_acquire () != nullptr; }
  T *operator->() ثابت { بازگشت get_acquire (); }
  template <typename C>
  operator C * () ثابت
  {
    بازگشت get_acquire ();
  }

  friend تهی swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#وگرنه /* defined(HB_NO_MT) */

template <typename T>
ساختار hb_atomic_t
{
  hb_atomic_t () = پیشفرض;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& operator = (T v_) { set_relaxed (v_); بازگشت *این; }
  operator T () ثابت { بازگشت get_relaxed (); }

  تهی set_relaxed (T v_) { v = v_; }
  تهی set_release (T v_) { v = v_; }
  T get_relaxed () ثابت { بازگشت v; }
  T get_acquire () ثابت { بازگشت v; }
  T inc () { بازگشت v++; }
  T dec () { بازگشت v--; }

  صحیح operator ++ (صحیح) { بازگشت inc (); }
  صحیح operator -- (صحیح) { بازگشت dec (); }

  T v = 0;
};

template <typename T>
ساختار hb_atomic_t<T*>
{
  hb_atomic_t () = پیشفرض;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = delete;

  تهی init (T* v_ = nullptr) { set_relaxed (v_); }
  تهی set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ثابت { بازگشت v; }
  T *get_acquire () ثابت { بازگشت v; }
  منطقی cmpexch (T *old, T *new_)
  {
    اگر (v != old) بازگشت نادرست;
    v = new_;
    بازگشت درست;
  }

  operator منطقی () ثابت { بازگشت get_acquire () != nullptr; }
  T * operator -> () ثابت                    { بازگشت get_acquire (); }
  template <typename C> operator C * () ثابت { بازگشت get_acquire (); }

  T *v = nullptr;
};

#endif


ایستا درونخطی منطقی hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  بازگشت درست;
}


#endif /* HB_ATOMIC_HH */
