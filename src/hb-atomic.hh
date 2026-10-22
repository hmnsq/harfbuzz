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

#اگر_غیر_تعریف_شدہ HB_ATOMIC_HH
#تعریف_کرو HB_ATOMIC_HH

#شامل "hb.hh"
#شامل "hb-meta.hh"


#اگر_غیر_تعریف_شدہ HB_NO_MT
#شامل <atomic>
#اگر_ختم


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#اگر_غیر_تعریف_شدہ hb_compiler_حافظہ_r_barrier
#اگر defined(__ATOMIC_ACQUIRE) // gcc-like
ساکن اندرونی خالی hb_compiler_حافظہ_r_barrier () { asm متلون("": : :"memory"); }
#وگرنہ_اگر !defined(_MSC_VER)
#شامل <atomic>
#تعریف_کرو hb_compiler_حافظہ_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#ورنہ
ساکن اندرونی خالی hb_compiler_حافظہ_r_barrier () {}
#اگر_ختم
#اگر_ختم


#اگر_غیر_تعریف_شدہ HB_NO_MT

سانچہ <قسم_نام T>
ساخت hb_atomic_t
{
  hb_atomic_t () = طےشدہ;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ثابت hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.سیٹ_relaxed ({}); }

  hb_atomic_t &عامل= (ثابت hb_atomic_t& o) { سیٹ_relaxed (o.get_relaxed ()); واپس *یہ; }
  hb_atomic_t &عامل= (hb_atomic_t&& o){ سیٹ_relaxed (o.get_relaxed ()); o.سیٹ_relaxed ({}); واپس *یہ; }
  hb_atomic_t &عامل= (T v_)
  {
    سیٹ_relaxed (v_);
    واپس *یہ;
  }
  عامل T () ثابت { واپس get_relaxed (); }

  خالی سیٹ_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  خالی سیٹ_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ثابت { واپس v.load (std::memory_order_relaxed); }
  T get_acquire () ثابت { واپس v.load (std::memory_order_acquire); }
  T inc () { واپس v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { واپس v.fetch_add (-1, std::memory_order_acq_rel); }

  صحیح عامل++ (صحیح) { واپس inc (); }
  صحیح عامل-- (صحیح) { واپس dec (); }

  دوست خالی swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.سیٹ_relaxed (b.get_acquire ());
    b.سیٹ_relaxed (v);
  }

  std::atomic<T> v {0};
};

سانچہ <قسم_نام T>
ساخت hb_atomic_t<T *>
{
  hb_atomic_t () = طےشدہ;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = حذف;

  خالی init (T *v_ = خالی_اشاریہ) { سیٹ_relaxed (v_); }
  خالی سیٹ_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ثابت { واپس v.load (std::memory_order_relaxed); }
  T *get_acquire () ثابت { واپس v.load (std::memory_order_acquire); }
  منطقی cmpexch (T *old, T *new_) { واپس v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &عامل= (ثابت hb_atomic_t& o) { سیٹ_relaxed (o.get_relaxed ()); واپس *یہ; }
  hb_atomic_t &عامل= (hb_atomic_t&& o){ سیٹ_relaxed (o.get_relaxed ()); o.سیٹ_relaxed ({}); واپس *یہ; }

  عامل منطقی () ثابت { واپس get_acquire () != خالی_اشاریہ; }
  T *عامل->() ثابت { واپس get_acquire (); }
  سانچہ <قسم_نام C>
  عامل C * () ثابت
  {
    واپس get_acquire ();
  }

  دوست خالی swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.سیٹ_relaxed (b.get_acquire ());
    b.سیٹ_relaxed (p);
  }

  std::atomic<T *> v {خالی_اشاریہ};
};

#ورنہ /* defined(HB_NO_MT) */

سانچہ <قسم_نام T>
ساخت hb_atomic_t
{
  hb_atomic_t () = طےشدہ;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& عامل = (T v_) { سیٹ_relaxed (v_); واپس *یہ; }
  عامل T () ثابت { واپس get_relaxed (); }

  خالی سیٹ_relaxed (T v_) { v = v_; }
  خالی سیٹ_release (T v_) { v = v_; }
  T get_relaxed () ثابت { واپس v; }
  T get_acquire () ثابت { واپس v; }
  T inc () { واپس v++; }
  T dec () { واپس v--; }

  صحیح عامل ++ (صحیح) { واپس inc (); }
  صحیح عامل -- (صحیح) { واپس dec (); }

  T v = 0;
};

سانچہ <قسم_نام T>
ساخت hb_atomic_t<T*>
{
  hb_atomic_t () = طےشدہ;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = حذف;

  خالی init (T* v_ = خالی_اشاریہ) { سیٹ_relaxed (v_); }
  خالی سیٹ_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ثابت { واپس v; }
  T *get_acquire () ثابت { واپس v; }
  منطقی cmpexch (T *old, T *new_)
  {
    اگر (v != old) واپس جھوٹ;
    v = new_;
    واپس سچ;
  }

  عامل منطقی () ثابت { واپس get_acquire () != خالی_اشاریہ; }
  T * عامل -> () ثابت                    { واپس get_acquire (); }
  سانچہ <قسم_نام C> عامل C * () ثابت { واپس get_acquire (); }

  T *v = خالی_اشاریہ;
};

#اگر_ختم


ساکن اندرونی منطقی hb_barrier ()
{
  hb_compiler_حافظہ_r_barrier ();
  واپس سچ;
}


#اگر_ختم /* HB_ATOMIC_HH */
