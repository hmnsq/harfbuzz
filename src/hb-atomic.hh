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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-meta.hh"


#ifndef HB_NO_MT
#अन्तर्भाव <atomic>
#endif


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#ifndef _hb_compiler_memory_r_barrier
#यदि defined(__ATOMIC_ACQUIRE) // gcc-like
स्थिर अन्तःस्थ शून्य _hb_compiler_memory_r_barrier () { asm चञ्चल("": : :"memory"); }
#नोचेद्यदि !defined(_MSC_VER)
#अन्तर्भाव <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#अन्यथा
स्थिर अन्तःस्थ शून्य _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

template <typename T>
संरचना hb_atomic_t
{
  hb_atomic_t () = स्वभाव;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (ध्रुव hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &operator= (ध्रुव hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); प्रत्यागम *एतत्; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); प्रत्यागम *एतत्; }
  hb_atomic_t &operator= (T v_)
  {
    set_relaxed (v_);
    प्रत्यागम *एतत्;
  }
  operator T () ध्रुव { प्रत्यागम get_relaxed (); }

  शून्य set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  शून्य set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () ध्रुव { प्रत्यागम v.load (std::memory_order_relaxed); }
  T get_acquire () ध्रुव { प्रत्यागम v.load (std::memory_order_acquire); }
  T inc () { प्रत्यागम v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { प्रत्यागम v.fetch_add (-1, std::memory_order_acq_rel); }

  पूर्णाङ्क operator++ (पूर्णाङ्क) { प्रत्यागम inc (); }
  पूर्णाङ्क operator-- (पूर्णाङ्क) { प्रत्यागम dec (); }

  friend शून्य swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

template <typename T>
संरचना hb_atomic_t<T *>
{
  hb_atomic_t () = स्वभाव;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (ध्रुव hb_atomic_t &other) = delete;

  शून्य init (T *v_ = nullptr) { set_relaxed (v_); }
  शून्य set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () ध्रुव { प्रत्यागम v.load (std::memory_order_relaxed); }
  T *get_acquire () ध्रुव { प्रत्यागम v.load (std::memory_order_acquire); }
  तार्किक cmpexch (T *old, T *new_) { प्रत्यागम v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &operator= (ध्रुव hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); प्रत्यागम *एतत्; }
  hb_atomic_t &operator= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); प्रत्यागम *एतत्; }

  operator तार्किक () ध्रुव { प्रत्यागम get_acquire () != nullptr; }
  T *operator->() ध्रुव { प्रत्यागम get_acquire (); }
  template <typename C>
  operator C * () ध्रुव
  {
    प्रत्यागम get_acquire ();
  }

  friend शून्य swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#अन्यथा /* defined(HB_NO_MT) */

template <typename T>
संरचना hb_atomic_t
{
  hb_atomic_t () = स्वभाव;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& operator = (T v_) { set_relaxed (v_); प्रत्यागम *एतत्; }
  operator T () ध्रुव { प्रत्यागम get_relaxed (); }

  शून्य set_relaxed (T v_) { v = v_; }
  शून्य set_release (T v_) { v = v_; }
  T get_relaxed () ध्रुव { प्रत्यागम v; }
  T get_acquire () ध्रुव { प्रत्यागम v; }
  T inc () { प्रत्यागम v++; }
  T dec () { प्रत्यागम v--; }

  पूर्णाङ्क operator ++ (पूर्णाङ्क) { प्रत्यागम inc (); }
  पूर्णाङ्क operator -- (पूर्णाङ्क) { प्रत्यागम dec (); }

  T v = 0;
};

template <typename T>
संरचना hb_atomic_t<T*>
{
  hb_atomic_t () = स्वभाव;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (ध्रुव hb_atomic_t &other) = delete;

  शून्य init (T* v_ = nullptr) { set_relaxed (v_); }
  शून्य set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () ध्रुव { प्रत्यागम v; }
  T *get_acquire () ध्रुव { प्रत्यागम v; }
  तार्किक cmpexch (T *old, T *new_)
  {
    यदि (v != old) प्रत्यागम असत्य;
    v = new_;
    प्रत्यागम सत्य;
  }

  operator तार्किक () ध्रुव { प्रत्यागम get_acquire () != nullptr; }
  T * operator -> () ध्रुव                    { प्रत्यागम get_acquire (); }
  template <typename C> operator C * () ध्रुव { प्रत्यागम get_acquire (); }

  T *v = nullptr;
};

#endif


स्थिर अन्तःस्थ तार्किक hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  प्रत्यागम सत्य;
}


#endif /* HB_ATOMIC_HH */
