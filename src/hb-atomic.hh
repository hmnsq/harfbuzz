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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ATOMIC_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ATOMIC_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-meta.hh"


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_MT
#ਅਨ੍ਤਰ੍ਭਾਵ <atomic>
#ਜੇ_ਅੰਤ


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ hb_compiler_ਸਮ੍ਰਿਤੀ_r_barrier
#ਜੇ defined(__ATOMIC_ACQUIRE) // gcc-like
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hb_compiler_ਸਮ੍ਰਿਤੀ_r_barrier () { asm ਅਸਥਿਰ("": : :"memory"); }
#ਜਾਂਜੇ !defined(_MSC_VER)
#ਅਨ੍ਤਰ੍ਭਾਵ <atomic>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_compiler_ਸਮ੍ਰਿਤੀ_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hb_compiler_ਸਮ੍ਰਿਤੀ_r_barrier () {}
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_MT

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_atomic_t
{
  hb_atomic_t () = ਮੂਲ;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ਅਚਲ hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.ਸੈੱਟ_relaxed ({}); }

  hb_atomic_t &ਸੰਚਾਲਕ= (ਅਚਲ hb_atomic_t& o) { ਸੈੱਟ_relaxed (o.get_relaxed ()); ਵਾਪਸ *ਇਹ; }
  hb_atomic_t &ਸੰਚਾਲਕ= (hb_atomic_t&& o){ ਸੈੱਟ_relaxed (o.get_relaxed ()); o.ਸੈੱਟ_relaxed ({}); ਵਾਪਸ *ਇਹ; }
  hb_atomic_t &ਸੰਚਾਲਕ= (T v_)
  {
    ਸੈੱਟ_relaxed (v_);
    ਵਾਪਸ *ਇਹ;
  }
  ਸੰਚਾਲਕ T () ਅਚਲ { ਵਾਪਸ get_relaxed (); }

  ਖਾਲੀ ਸੈੱਟ_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  ਖਾਲੀ ਸੈੱਟ_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ਅਚਲ { ਵਾਪਸ v.load (std::memory_order_relaxed); }
  T get_acquire () ਅਚਲ { ਵਾਪਸ v.load (std::memory_order_acquire); }
  T inc () { ਵਾਪਸ v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { ਵਾਪਸ v.fetch_add (-1, std::memory_order_acq_rel); }

  ਪੂਰਨ ਸੰਚਾਲਕ++ (ਪੂਰਨ) { ਵਾਪਸ inc (); }
  ਪੂਰਨ ਸੰਚਾਲਕ-- (ਪੂਰਨ) { ਵਾਪਸ dec (); }

  ਮਿੱਤਰ ਖਾਲੀ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.ਸੈੱਟ_relaxed (b.get_acquire ());
    b.ਸੈੱਟ_relaxed (v);
  }

  std::atomic<T> v {0};
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_atomic_t<T *>
{
  hb_atomic_t () = ਮੂਲ;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ਅਚਲ hb_atomic_t &other) = ਮਿਟਾਓ;

  ਖਾਲੀ init (T *v_ = ਖਾਲੀ_ਸੂਚਕ) { ਸੈੱਟ_relaxed (v_); }
  ਖਾਲੀ ਸੈੱਟ_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ਅਚਲ { ਵਾਪਸ v.load (std::memory_order_relaxed); }
  T *get_acquire () ਅਚਲ { ਵਾਪਸ v.load (std::memory_order_acquire); }
  ਤਾਰਕਿਕ cmpexch (T *old, T *new_) { ਵਾਪਸ v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &ਸੰਚਾਲਕ= (ਅਚਲ hb_atomic_t& o) { ਸੈੱਟ_relaxed (o.get_relaxed ()); ਵਾਪਸ *ਇਹ; }
  hb_atomic_t &ਸੰਚਾਲਕ= (hb_atomic_t&& o){ ਸੈੱਟ_relaxed (o.get_relaxed ()); o.ਸੈੱਟ_relaxed ({}); ਵਾਪਸ *ਇਹ; }

  ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ get_acquire () != ਖਾਲੀ_ਸੂਚਕ; }
  T *ਸੰਚਾਲਕ->() ਅਚਲ { ਵਾਪਸ get_acquire (); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ C>
  ਸੰਚਾਲਕ C * () ਅਚਲ
  {
    ਵਾਪਸ get_acquire ();
  }

  ਮਿੱਤਰ ਖਾਲੀ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.ਸੈੱਟ_relaxed (b.get_acquire ());
    b.ਸੈੱਟ_relaxed (p);
  }

  std::atomic<T *> v {ਖਾਲੀ_ਸੂਚਕ};
};

#ਨਹੀਂਤਾਂ /* defined(HB_NO_MT) */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_atomic_t
{
  hb_atomic_t () = ਮੂਲ;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& ਸੰਚਾਲਕ = (T v_) { ਸੈੱਟ_relaxed (v_); ਵਾਪਸ *ਇਹ; }
  ਸੰਚਾਲਕ T () ਅਚਲ { ਵਾਪਸ get_relaxed (); }

  ਖਾਲੀ ਸੈੱਟ_relaxed (T v_) { v = v_; }
  ਖਾਲੀ ਸੈੱਟ_release (T v_) { v = v_; }
  T get_relaxed () ਅਚਲ { ਵਾਪਸ v; }
  T get_acquire () ਅਚਲ { ਵਾਪਸ v; }
  T inc () { ਵਾਪਸ v++; }
  T dec () { ਵਾਪਸ v--; }

  ਪੂਰਨ ਸੰਚਾਲਕ ++ (ਪੂਰਨ) { ਵਾਪਸ inc (); }
  ਪੂਰਨ ਸੰਚਾਲਕ -- (ਪੂਰਨ) { ਵਾਪਸ dec (); }

  T v = 0;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_atomic_t<T*>
{
  hb_atomic_t () = ਮੂਲ;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ਅਚਲ hb_atomic_t &other) = ਮਿਟਾਓ;

  ਖਾਲੀ init (T* v_ = ਖਾਲੀ_ਸੂਚਕ) { ਸੈੱਟ_relaxed (v_); }
  ਖਾਲੀ ਸੈੱਟ_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ਅਚਲ { ਵਾਪਸ v; }
  T *get_acquire () ਅਚਲ { ਵਾਪਸ v; }
  ਤਾਰਕਿਕ cmpexch (T *old, T *new_)
  {
    ਜੇ (v != old) ਵਾਪਸ ਝੂਠ;
    v = new_;
    ਵਾਪਸ ਸੱਚ;
  }

  ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ get_acquire () != ਖਾਲੀ_ਸੂਚਕ; }
  T * ਸੰਚਾਲਕ -> () ਅਚਲ                    { ਵਾਪਸ get_acquire (); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ C> ਸੰਚਾਲਕ C * () ਅਚਲ { ਵਾਪਸ get_acquire (); }

  T *v = ਖਾਲੀ_ਸੂਚਕ;
};

#ਜੇ_ਅੰਤ


ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ hb_barrier ()
{
  hb_compiler_ਸਮ੍ਰਿਤੀ_r_barrier ();
  ਵਾਪਸ ਸੱਚ;
}


#ਜੇ_ਅੰਤ /* HB_ATOMIC_HH */
