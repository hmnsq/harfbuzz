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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-meta.hh"


#ifndef HB_NO_MT
#அந்தர்பாவ <atomic>
#endif


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ifndef _hb_compiler_memory_r_barrier
#எனில் defined(__ATOMIC_ACQUIRE) // gcc-like
நிலையான உள்வரி வெறுமை _hb_compiler_memory_r_barrier () { asm நிலையற்ற("": : :"memory"); }
#அல்லதெனில் !defined(_MSC_VER)
#அந்தர்பாவ <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#இல்லையெனில்
நிலையான உள்வரி வெறுமை _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

template <typename T>
கட்டமைப்பு hb_atomic_t
{
  hb_atomic_t () = இயல்பு;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (மாறிலி hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &operator= (மாறிலி hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); திரும்பு *இது; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); திரும்பு *இது; }
  hb_atomic_t &operator= (T v_)
  {
    set_relaxed (v_);
    திரும்பு *இது;
  }
  operator T () மாறிலி { திரும்பு get_relaxed (); }

  வெறுமை set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  வெறுமை set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () மாறிலி { திரும்பு v.load (std::memory_order_relaxed); }
  T get_acquire () மாறிலி { திரும்பு v.load (std::memory_order_acquire); }
  T inc () { திரும்பு v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { திரும்பு v.fetch_add (-1, std::memory_order_acq_rel); }

  முழு operator++ (முழு) { திரும்பு inc (); }
  முழு operator-- (முழு) { திரும்பு dec (); }

  friend வெறுமை swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

template <typename T>
கட்டமைப்பு hb_atomic_t<T *>
{
  hb_atomic_t () = இயல்பு;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (மாறிலி hb_atomic_t &other) = delete;

  வெறுமை init (T *v_ = nullptr) { set_relaxed (v_); }
  வெறுமை set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () மாறிலி { திரும்பு v.load (std::memory_order_relaxed); }
  T *get_acquire () மாறிலி { திரும்பு v.load (std::memory_order_acquire); }
  தருக்க cmpexch (T *old, T *new_) { திரும்பு v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &operator= (மாறிலி hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); திரும்பு *இது; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); திரும்பு *இது; }

  operator தருக்க () மாறிலி { திரும்பு get_acquire () != nullptr; }
  T *operator->() மாறிலி { திரும்பு get_acquire (); }
  template <typename C>
  operator C * () மாறிலி
  {
    திரும்பு get_acquire ();
  }

  friend வெறுமை swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#இல்லையெனில் /* defined(HB_NO_MT) */

template <typename T>
கட்டமைப்பு hb_atomic_t
{
  hb_atomic_t () = இயல்பு;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& operator = (T v_) { set_relaxed (v_); திரும்பு *இது; }
  operator T () மாறிலி { திரும்பு get_relaxed (); }

  வெறுமை set_relaxed (T v_) { v = v_; }
  வெறுமை set_release (T v_) { v = v_; }
  T get_relaxed () மாறிலி { திரும்பு v; }
  T get_acquire () மாறிலி { திரும்பு v; }
  T inc () { திரும்பு v++; }
  T dec () { திரும்பு v--; }

  முழு operator ++ (முழு) { திரும்பு inc (); }
  முழு operator -- (முழு) { திரும்பு dec (); }

  T v = 0;
};

template <typename T>
கட்டமைப்பு hb_atomic_t<T*>
{
  hb_atomic_t () = இயல்பு;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (மாறிலி hb_atomic_t &other) = delete;

  வெறுமை init (T* v_ = nullptr) { set_relaxed (v_); }
  வெறுமை set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () மாறிலி { திரும்பு v; }
  T *get_acquire () மாறிலி { திரும்பு v; }
  தருக்க cmpexch (T *old, T *new_)
  {
    எனில் (v != old) திரும்பு பொய்;
    v = new_;
    திரும்பு உண்மை;
  }

  operator தருக்க () மாறிலி { திரும்பு get_acquire () != nullptr; }
  T * operator -> () மாறிலி                    { திரும்பு get_acquire (); }
  template <typename C> operator C * () மாறிலி { திரும்பு get_acquire (); }

  T *v = nullptr;
};

#endif


நிலையான உள்வரி தருக்க hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  திரும்பு உண்மை;
}


#endif /* HB_ATOMIC_HH */
