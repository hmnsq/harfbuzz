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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-meta.hh"


#ifndef HB_NO_MT
#ଅନ୍ତର୍ଭାଵ <atomic>
#endif


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ifndef _hb_compiler_memory_r_barrier
#ଯଦି defined(__ATOMIC_ACQUIRE) // gcc-like
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ _hb_compiler_memory_r_barrier () { asm ଅସ୍ଥିର("": : :"memory"); }
#ନଚେତଯଦି !defined(_MSC_VER)
#ଅନ୍ତର୍ଭାଵ <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

template <typename T>
ସଂରଚନା hb_atomic_t
{
  hb_atomic_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ଅଚଳ hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &operator= (ଅଚଳ hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); ଫେରନ୍ତୁ *ଏହି; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); ଫେରନ୍ତୁ *ଏହି; }
  hb_atomic_t &operator= (T v_)
  {
    set_relaxed (v_);
    ଫେରନ୍ତୁ *ଏହି;
  }
  operator T () ଅଚଳ { ଫେରନ୍ତୁ get_relaxed (); }

  ଶୂନ୍ୟ set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  ଶୂନ୍ୟ set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ଅଚଳ { ଫେରନ୍ତୁ v.load (std::memory_order_relaxed); }
  T get_acquire () ଅଚଳ { ଫେରନ୍ତୁ v.load (std::memory_order_acquire); }
  T inc () { ଫେରନ୍ତୁ v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { ଫେରନ୍ତୁ v.fetch_add (-1, std::memory_order_acq_rel); }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator++ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) { ଫେରନ୍ତୁ inc (); }
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator-- (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) { ଫେରନ୍ତୁ dec (); }

  friend ଶୂନ୍ୟ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

template <typename T>
ସଂରଚନା hb_atomic_t<T *>
{
  hb_atomic_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ଅଚଳ hb_atomic_t &other) = delete;

  ଶୂନ୍ୟ init (T *v_ = nullptr) { set_relaxed (v_); }
  ଶୂନ୍ୟ set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ଅଚଳ { ଫେରନ୍ତୁ v.load (std::memory_order_relaxed); }
  T *get_acquire () ଅଚଳ { ଫେରନ୍ତୁ v.load (std::memory_order_acquire); }
  ତାର୍କିକ cmpexch (T *old, T *new_) { ଫେରନ୍ତୁ v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &operator= (ଅଚଳ hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); ଫେରନ୍ତୁ *ଏହି; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); ଫେରନ୍ତୁ *ଏହି; }

  operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ get_acquire () != nullptr; }
  T *operator->() ଅଚଳ { ଫେରନ୍ତୁ get_acquire (); }
  template <typename C>
  operator C * () ଅଚଳ
  {
    ଫେରନ୍ତୁ get_acquire ();
  }

  friend ଶୂନ୍ୟ swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#ଅନ୍ୟଥା /* defined(HB_NO_MT) */

template <typename T>
ସଂରଚନା hb_atomic_t
{
  hb_atomic_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& operator = (T v_) { set_relaxed (v_); ଫେରନ୍ତୁ *ଏହି; }
  operator T () ଅଚଳ { ଫେରନ୍ତୁ get_relaxed (); }

  ଶୂନ୍ୟ set_relaxed (T v_) { v = v_; }
  ଶୂନ୍ୟ set_release (T v_) { v = v_; }
  T get_relaxed () ଅଚଳ { ଫେରନ୍ତୁ v; }
  T get_acquire () ଅଚଳ { ଫେରନ୍ତୁ v; }
  T inc () { ଫେରନ୍ତୁ v++; }
  T dec () { ଫେରନ୍ତୁ v--; }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator ++ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) { ଫେରନ୍ତୁ inc (); }
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator -- (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) { ଫେରନ୍ତୁ dec (); }

  T v = 0;
};

template <typename T>
ସଂରଚନା hb_atomic_t<T*>
{
  hb_atomic_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ଅଚଳ hb_atomic_t &other) = delete;

  ଶୂନ୍ୟ init (T* v_ = nullptr) { set_relaxed (v_); }
  ଶୂନ୍ୟ set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ଅଚଳ { ଫେରନ୍ତୁ v; }
  T *get_acquire () ଅଚଳ { ଫେରନ୍ତୁ v; }
  ତାର୍କିକ cmpexch (T *old, T *new_)
  {
    ଯଦି (v != old) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    v = new_;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ get_acquire () != nullptr; }
  T * operator -> () ଅଚଳ                    { ଫେରନ୍ତୁ get_acquire (); }
  template <typename C> operator C * () ଅଚଳ { ଫେରନ୍ତୁ get_acquire (); }

  T *v = nullptr;
};

#endif


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  ଫେରନ୍ତୁ ସତ୍ୟ;
}


#endif /* HB_ATOMIC_HH */
