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

#که_نه_تعریف_شوی HB_ATOMIC_HH
#تعریف_کړه HB_ATOMIC_HH

#ضمن "hb.hh"
#ضمن "hb-meta.hh"


#که_نه_تعریف_شوی HB_NO_MT
#ضمن <atomic>
#د_که_پای


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#که_نه_تعریف_شوی hb_compiler_حافظه_r_barrier
#إذا defined(__ATOMIC_ACQUIRE) // gcc-like
ساكن مضمن فراغ hb_compiler_حافظه_r_barrier () { asm متطاير("": : :"memory"); }
#وإلاإذا !defined(_MSC_VER)
#ضمن <atomic>
#تعریف_کړه hb_compiler_حافظه_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#وإلا
ساكن مضمن فراغ hb_compiler_حافظه_r_barrier () {}
#د_که_پای
#د_که_پای


#که_نه_تعریف_شوی HB_NO_MT

کينډۍ <ډول_نوم T>
بنية hb_atomic_t
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ثابت hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.سېټ_relaxed ({}); }

  hb_atomic_t &چلوونکی= (ثابت hb_atomic_t& o) { سېټ_relaxed (o.get_relaxed ()); أرجع *هذا; }
  hb_atomic_t &چلوونکی= (hb_atomic_t&& o){ سېټ_relaxed (o.get_relaxed ()); o.سېټ_relaxed ({}); أرجع *هذا; }
  hb_atomic_t &چلوونکی= (T v_)
  {
    سېټ_relaxed (v_);
    أرجع *هذا;
  }
  چلوونکی T () ثابت { أرجع get_relaxed (); }

  فراغ سېټ_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  فراغ سېټ_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ثابت { أرجع v.load (std::memory_order_relaxed); }
  T get_acquire () ثابت { أرجع v.load (std::memory_order_acquire); }
  T inc () { أرجع v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { أرجع v.fetch_add (-1, std::memory_order_acq_rel); }

  صحيح چلوونکی++ (صحيح) { أرجع inc (); }
  صحيح چلوونکی-- (صحيح) { أرجع dec (); }

  ملګری فراغ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.سېټ_relaxed (b.get_acquire ());
    b.سېټ_relaxed (v);
  }

  std::atomic<T> v {0};
};

کينډۍ <ډول_نوم T>
بنية hb_atomic_t<T *>
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = ړنګول;

  فراغ init (T *v_ = تش_اشاره) { سېټ_relaxed (v_); }
  فراغ سېټ_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ثابت { أرجع v.load (std::memory_order_relaxed); }
  T *get_acquire () ثابت { أرجع v.load (std::memory_order_acquire); }
  منطقي cmpexch (T *old, T *new_) { أرجع v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &چلوونکی= (ثابت hb_atomic_t& o) { سېټ_relaxed (o.get_relaxed ()); أرجع *هذا; }
  hb_atomic_t &چلوونکی= (hb_atomic_t&& o){ سېټ_relaxed (o.get_relaxed ()); o.سېټ_relaxed ({}); أرجع *هذا; }

  چلوونکی منطقي () ثابت { أرجع get_acquire () != تش_اشاره; }
  T *چلوونکی->() ثابت { أرجع get_acquire (); }
  کينډۍ <ډول_نوم C>
  چلوونکی C * () ثابت
  {
    أرجع get_acquire ();
  }

  ملګری فراغ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.سېټ_relaxed (b.get_acquire ());
    b.سېټ_relaxed (p);
  }

  std::atomic<T *> v {تش_اشاره};
};

#وإلا /* defined(HB_NO_MT) */

کينډۍ <ډول_نوم T>
بنية hb_atomic_t
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& چلوونکی = (T v_) { سېټ_relaxed (v_); أرجع *هذا; }
  چلوونکی T () ثابت { أرجع get_relaxed (); }

  فراغ سېټ_relaxed (T v_) { v = v_; }
  فراغ سېټ_release (T v_) { v = v_; }
  T get_relaxed () ثابت { أرجع v; }
  T get_acquire () ثابت { أرجع v; }
  T inc () { أرجع v++; }
  T dec () { أرجع v--; }

  صحيح چلوونکی ++ (صحيح) { أرجع inc (); }
  صحيح چلوونکی -- (صحيح) { أرجع dec (); }

  T v = 0;
};

کينډۍ <ډول_نوم T>
بنية hb_atomic_t<T*>
{
  hb_atomic_t () = افتراضي;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ثابت hb_atomic_t &other) = ړنګول;

  فراغ init (T* v_ = تش_اشاره) { سېټ_relaxed (v_); }
  فراغ سېټ_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ثابت { أرجع v; }
  T *get_acquire () ثابت { أرجع v; }
  منطقي cmpexch (T *old, T *new_)
  {
    إذا (v != old) أرجع خطأ;
    v = new_;
    أرجع ریښتیا;
  }

  چلوونکی منطقي () ثابت { أرجع get_acquire () != تش_اشاره; }
  T * چلوونکی -> () ثابت                    { أرجع get_acquire (); }
  کينډۍ <ډول_نوم C> چلوونکی C * () ثابت { أرجع get_acquire (); }

  T *v = تش_اشاره;
};

#د_که_پای


ساكن مضمن منطقي hb_barrier ()
{
  hb_compiler_حافظه_r_barrier ();
  أرجع ریښتیا;
}


#د_که_پای /* HB_ATOMIC_HH */
