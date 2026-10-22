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

#اگر_تعریف_نشده HB_ATOMIC_HH
#تعریف_کن HB_ATOMIC_HH

#ضمن "hb.hh"
#ضمن "hb-meta.hh"


#اگر_تعریف_نشده HB_NO_MT
#ضمن <atomic>
#پایان_اگر


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#اگر_تعریف_نشده hb_compiler_حافظه_r_barrier
#إذا defined(__ATOMIC_ACQUIRE) // gcc-like
ساكن مضمن فراغ hb_compiler_حافظه_r_barrier () { asm متطاير("": : :"memory"); }
#وإلاإذا !defined(_MSC_VER)
#ضمن <atomic>
#تعریف_کن hb_compiler_حافظه_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#وإلا
ساكن مضمن فراغ hb_compiler_حافظه_r_barrier () {}
#پایان_اگر
#پایان_اگر


#اگر_تعریف_نشده HB_NO_MT

قالب <نام_نوع T>
بنية hb_atomic_t
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ثابت hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.مجموعه_relaxed ({}); }

  hb_atomic_t &عملگر= (ثابت hb_atomic_t& o) { مجموعه_relaxed (o.get_relaxed ()); أرجع *هذا; }
  hb_atomic_t &عملگر= (hb_atomic_t&& o){ مجموعه_relaxed (o.get_relaxed ()); o.مجموعه_relaxed ({}); أرجع *هذا; }
  hb_atomic_t &عملگر= (T v_)
  {
    مجموعه_relaxed (v_);
    أرجع *هذا;
  }
  عملگر T () ثابت { أرجع get_relaxed (); }

  فراغ مجموعه_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  فراغ مجموعه_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ثابت { أرجع v.load (std::memory_order_relaxed); }
  T get_acquire () ثابت { أرجع v.load (std::memory_order_acquire); }
  T inc () { أرجع v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { أرجع v.fetch_add (-1, std::memory_order_acq_rel); }

  صحيح عملگر++ (صحيح) { أرجع inc (); }
  صحيح عملگر-- (صحيح) { أرجع dec (); }

  دوست فراغ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.مجموعه_relaxed (b.get_acquire ());
    b.مجموعه_relaxed (v);
  }

  std::atomic<T> v {0};
};

قالب <نام_نوع T>
بنية hb_atomic_t<T *>
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = حذف;

  فراغ init (T *v_ = اشاره_تهی) { مجموعه_relaxed (v_); }
  فراغ مجموعه_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ثابت { أرجع v.load (std::memory_order_relaxed); }
  T *get_acquire () ثابت { أرجع v.load (std::memory_order_acquire); }
  منطقي cmpexch (T *old, T *new_) { أرجع v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &عملگر= (ثابت hb_atomic_t& o) { مجموعه_relaxed (o.get_relaxed ()); أرجع *هذا; }
  hb_atomic_t &عملگر= (hb_atomic_t&& o){ مجموعه_relaxed (o.get_relaxed ()); o.مجموعه_relaxed ({}); أرجع *هذا; }

  عملگر منطقي () ثابت { أرجع get_acquire () != اشاره_تهی; }
  T *عملگر->() ثابت { أرجع get_acquire (); }
  قالب <نام_نوع C>
  عملگر C * () ثابت
  {
    أرجع get_acquire ();
  }

  دوست فراغ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.مجموعه_relaxed (b.get_acquire ());
    b.مجموعه_relaxed (p);
  }

  std::atomic<T *> v {اشاره_تهی};
};

#وإلا /* defined(HB_NO_MT) */

قالب <نام_نوع T>
بنية hb_atomic_t
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& عملگر = (T v_) { مجموعه_relaxed (v_); أرجع *هذا; }
  عملگر T () ثابت { أرجع get_relaxed (); }

  فراغ مجموعه_relaxed (T v_) { v = v_; }
  فراغ مجموعه_release (T v_) { v = v_; }
  T get_relaxed () ثابت { أرجع v; }
  T get_acquire () ثابت { أرجع v; }
  T inc () { أرجع v++; }
  T dec () { أرجع v--; }

  صحيح عملگر ++ (صحيح) { أرجع inc (); }
  صحيح عملگر -- (صحيح) { أرجع dec (); }

  T v = 0;
};

قالب <نام_نوع T>
بنية hb_atomic_t<T*>
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = حذف;

  فراغ init (T* v_ = اشاره_تهی) { مجموعه_relaxed (v_); }
  فراغ مجموعه_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ثابت { أرجع v; }
  T *get_acquire () ثابت { أرجع v; }
  منطقي cmpexch (T *old, T *new_)
  {
    إذا (v != old) أرجع خطأ;
    v = new_;
    أرجع درست;
  }

  عملگر منطقي () ثابت { أرجع get_acquire () != اشاره_تهی; }
  T * عملگر -> () ثابت                    { أرجع get_acquire (); }
  قالب <نام_نوع C> عملگر C * () ثابت { أرجع get_acquire (); }

  T *v = اشاره_تهی;
};

#پایان_اگر


ساكن مضمن منطقي hb_barrier ()
{
  hb_compiler_حافظه_r_barrier ();
  أرجع درست;
}


#پایان_اگر /* HB_ATOMIC_HH */
