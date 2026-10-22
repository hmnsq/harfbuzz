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

#جيڪڏهن_وصف_نه_ٿيل HB_ATOMIC_HH
#وصف_ڪريو HB_ATOMIC_HH

#ضمن "hb.hh"
#ضمن "hb-meta.hh"


#جيڪڏهن_وصف_نه_ٿيل HB_NO_MT
#ضمن <atomic>
#جيڪڏهن_پورو


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#جيڪڏهن_وصف_نه_ٿيل hb_compiler_يادگيري_r_barrier
#إذا defined(__ATOMIC_ACQUIRE) // gcc-like
ساكن مضمن فراغ hb_compiler_يادگيري_r_barrier () { asm متطاير("": : :"memory"); }
#وإلاإذا !defined(_MSC_VER)
#ضمن <atomic>
#وصف_ڪريو hb_compiler_يادگيري_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#وإلا
ساكن مضمن فراغ hb_compiler_يادگيري_r_barrier () {}
#جيڪڏهن_پورو
#جيڪڏهن_پورو


#جيڪڏهن_وصف_نه_ٿيل HB_NO_MT

سانچو <قسم_نالو T>
بنية hb_atomic_t
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ثابت hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.سيٽ_relaxed ({}); }

  hb_atomic_t &عامل= (ثابت hb_atomic_t& o) { سيٽ_relaxed (o.get_relaxed ()); أرجع *هذا; }
  hb_atomic_t &عامل= (hb_atomic_t&& o){ سيٽ_relaxed (o.get_relaxed ()); o.سيٽ_relaxed ({}); أرجع *هذا; }
  hb_atomic_t &عامل= (T v_)
  {
    سيٽ_relaxed (v_);
    أرجع *هذا;
  }
  عامل T () ثابت { أرجع get_relaxed (); }

  فراغ سيٽ_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  فراغ سيٽ_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ثابت { أرجع v.load (std::memory_order_relaxed); }
  T get_acquire () ثابت { أرجع v.load (std::memory_order_acquire); }
  T inc () { أرجع v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { أرجع v.fetch_add (-1, std::memory_order_acq_rel); }

  صحيح عامل++ (صحيح) { أرجع inc (); }
  صحيح عامل-- (صحيح) { أرجع dec (); }

  دوست فراغ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.سيٽ_relaxed (b.get_acquire ());
    b.سيٽ_relaxed (v);
  }

  std::atomic<T> v {0};
};

سانچو <قسم_نالو T>
بنية hb_atomic_t<T *>
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = ميٽيو;

  فراغ init (T *v_ = خالي_اشارو) { سيٽ_relaxed (v_); }
  فراغ سيٽ_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ثابت { أرجع v.load (std::memory_order_relaxed); }
  T *get_acquire () ثابت { أرجع v.load (std::memory_order_acquire); }
  منطقي cmpexch (T *old, T *new_) { أرجع v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &عامل= (ثابت hb_atomic_t& o) { سيٽ_relaxed (o.get_relaxed ()); أرجع *هذا; }
  hb_atomic_t &عامل= (hb_atomic_t&& o){ سيٽ_relaxed (o.get_relaxed ()); o.سيٽ_relaxed ({}); أرجع *هذا; }

  عامل منطقي () ثابت { أرجع get_acquire () != خالي_اشارو; }
  T *عامل->() ثابت { أرجع get_acquire (); }
  سانچو <قسم_نالو C>
  عامل C * () ثابت
  {
    أرجع get_acquire ();
  }

  دوست فراغ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.سيٽ_relaxed (b.get_acquire ());
    b.سيٽ_relaxed (p);
  }

  std::atomic<T *> v {خالي_اشارو};
};

#وإلا /* defined(HB_NO_MT) */

سانچو <قسم_نالو T>
بنية hb_atomic_t
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& عامل = (T v_) { سيٽ_relaxed (v_); أرجع *هذا; }
  عامل T () ثابت { أرجع get_relaxed (); }

  فراغ سيٽ_relaxed (T v_) { v = v_; }
  فراغ سيٽ_release (T v_) { v = v_; }
  T get_relaxed () ثابت { أرجع v; }
  T get_acquire () ثابت { أرجع v; }
  T inc () { أرجع v++; }
  T dec () { أرجع v--; }

  صحيح عامل ++ (صحيح) { أرجع inc (); }
  صحيح عامل -- (صحيح) { أرجع dec (); }

  T v = 0;
};

سانچو <قسم_نالو T>
بنية hb_atomic_t<T*>
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = ميٽيو;

  فراغ init (T* v_ = خالي_اشارو) { سيٽ_relaxed (v_); }
  فراغ سيٽ_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ثابت { أرجع v; }
  T *get_acquire () ثابت { أرجع v; }
  منطقي cmpexch (T *old, T *new_)
  {
    إذا (v != old) أرجع خطأ;
    v = new_;
    أرجع سچ;
  }

  عامل منطقي () ثابت { أرجع get_acquire () != خالي_اشارو; }
  T * عامل -> () ثابت                    { أرجع get_acquire (); }
  سانچو <قسم_نالو C> عامل C * () ثابت { أرجع get_acquire (); }

  T *v = خالي_اشارو;
};

#جيڪڏهن_پورو


ساكن مضمن منطقي hb_barrier ()
{
  hb_compiler_يادگيري_r_barrier ();
  أرجع سچ;
}


#جيڪڏهن_پورو /* HB_ATOMIC_HH */
