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

#include "hb.hh"
#include "hb-meta.hh"


#ifndef HB_NO_MT
#include <atomic>
#endif


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ifndef _hb_compiler_memory_r_barrier
#ຖ້າ defined(__ATOMIC_ACQUIRE) // gcc-like
static anta_hstha void _hb_compiler_memory_r_barrier () { asm ca_ncala("": : :"memory"); }
#elif !defined(_MSC_VER)
#include <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#ບໍ່ດັ່ງນັ້ນ
static anta_hstha void _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

template <typename T>
sa_nracanaa hb_atomic_t
{
  hb_atomic_t () = default;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (const hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &operator= (const hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); ສົ່ງຄືນ *this; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); ສົ່ງຄືນ *this; }
  hb_atomic_t &operator= (T v_)
  {
    set_relaxed (v_);
    ສົ່ງຄືນ *this;
  }
  operator T () const { ສົ່ງຄືນ get_relaxed (); }

  void set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  void set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () const { ສົ່ງຄືນ v.load (std::memory_order_relaxed); }
  T get_acquire () const { ສົ່ງຄືນ v.load (std::memory_order_acquire); }
  T inc () { ສົ່ງຄືນ v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { ສົ່ງຄືນ v.fetch_add (-1, std::memory_order_acq_rel); }

  ຈຳນວນເຕັມ operator++ (ຈຳນວນເຕັມ) { ສົ່ງຄືນ inc (); }
  ຈຳນວນເຕັມ operator-- (ຈຳນວນເຕັມ) { ສົ່ງຄືນ dec (); }

  friend void swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

template <typename T>
sa_nracanaa hb_atomic_t<T *>
{
  hb_atomic_t () = default;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (const hb_atomic_t &other) = delete;

  void init (T *v_ = nullptr) { set_relaxed (v_); }
  void set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () const { ສົ່ງຄືນ v.load (std::memory_order_relaxed); }
  T *get_acquire () const { ສົ່ງຄືນ v.load (std::memory_order_acquire); }
  bool cmpexch (T *old, T *new_) { ສົ່ງຄືນ v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &operator= (const hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); ສົ່ງຄືນ *this; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); ສົ່ງຄືນ *this; }

  operator bool () const { ສົ່ງຄືນ get_acquire () != nullptr; }
  T *operator->() const { ສົ່ງຄືນ get_acquire (); }
  template <typename C>
  operator C * () const
  {
    ສົ່ງຄືນ get_acquire ();
  }

  friend void swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#ບໍ່ດັ່ງນັ້ນ /* defined(HB_NO_MT) */

template <typename T>
sa_nracanaa hb_atomic_t
{
  hb_atomic_t () = default;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& operator = (T v_) { set_relaxed (v_); ສົ່ງຄືນ *this; }
  operator T () const { ສົ່ງຄືນ get_relaxed (); }

  void set_relaxed (T v_) { v = v_; }
  void set_release (T v_) { v = v_; }
  T get_relaxed () const { ສົ່ງຄືນ v; }
  T get_acquire () const { ສົ່ງຄືນ v; }
  T inc () { ສົ່ງຄືນ v++; }
  T dec () { ສົ່ງຄືນ v--; }

  ຈຳນວນເຕັມ operator ++ (ຈຳນວນເຕັມ) { ສົ່ງຄືນ inc (); }
  ຈຳນວນເຕັມ operator -- (ຈຳນວນເຕັມ) { ສົ່ງຄືນ dec (); }

  T v = 0;
};

template <typename T>
sa_nracanaa hb_atomic_t<T*>
{
  hb_atomic_t () = default;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (const hb_atomic_t &other) = delete;

  void init (T* v_ = nullptr) { set_relaxed (v_); }
  void set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () const { ສົ່ງຄືນ v; }
  T *get_acquire () const { ສົ່ງຄືນ v; }
  bool cmpexch (T *old, T *new_)
  {
    ຖ້າ (v != old) ສົ່ງຄືນ false;
    v = new_;
    ສົ່ງຄືນ true;
  }

  operator bool () const { ສົ່ງຄືນ get_acquire () != nullptr; }
  T * operator -> () const                    { ສົ່ງຄືນ get_acquire (); }
  template <typename C> operator C * () const { ສົ່ງຄືນ get_acquire (); }

  T *v = nullptr;
};

#endif


static anta_hstha bool hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  ສົ່ງຄືນ true;
}


#endif /* HB_ATOMIC_HH */
