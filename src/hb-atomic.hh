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
स्थिर अन्तःस्थ शून्य _hb_compiler_memory_r_barrier () { यंत्र चञ्चल("": : :"memory"); }
#अथवायदि !defined(_MSC_VER)
#अन्तर्भाव <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#अन्यथा
स्थिर अन्तःस्थ शून्य _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

ढाँचा <typename T>
संरचना hb_atomic_t
{
  hb_atomic_t () = स्वभाव;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (अचर hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &चालक= (अचर hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); फर्काऊ *एतत्; }
  hb_atomic_t &चालक= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); फर्काऊ *एतत्; }
  hb_atomic_t &चालक= (T v_)
  {
    set_relaxed (v_);
    फर्काऊ *एतत्;
  }
  चालक T () अचर { फर्काऊ get_relaxed (); }

  शून्य set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  शून्य set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () अचर { फर्काऊ v.load (std::memory_order_relaxed); }
  T get_acquire () अचर { फर्काऊ v.load (std::memory_order_acquire); }
  T inc () { फर्काऊ v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { फर्काऊ v.fetch_add (-1, std::memory_order_acq_rel); }

  पूर्णाङ्क चालक++ (पूर्णाङ्क) { फर्काऊ inc (); }
  पूर्णाङ्क चालक-- (पूर्णाङ्क) { फर्काऊ dec (); }

  मित्र शून्य swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

ढाँचा <typename T>
संरचना hb_atomic_t<T *>
{
  hb_atomic_t () = स्वभाव;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (अचर hb_atomic_t &other) = delete;

  शून्य init (T *v_ = nullptr) { set_relaxed (v_); }
  शून्य set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () अचर { फर्काऊ v.load (std::memory_order_relaxed); }
  T *get_acquire () अचर { फर्काऊ v.load (std::memory_order_acquire); }
  तार्किक cmpexch (T *old, T *new_) { फर्काऊ v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &चालक= (अचर hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); फर्काऊ *एतत्; }
  hb_atomic_t &चालक= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); फर्काऊ *एतत्; }

  चालक तार्किक () अचर { फर्काऊ get_acquire () != nullptr; }
  T *चालक->() अचर { फर्काऊ get_acquire (); }
  ढाँचा <typename C>
  चालक C * () अचर
  {
    फर्काऊ get_acquire ();
  }

  मित्र शून्य swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#अन्यथा /* defined(HB_NO_MT) */

ढाँचा <typename T>
संरचना hb_atomic_t
{
  hb_atomic_t () = स्वभाव;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& चालक = (T v_) { set_relaxed (v_); फर्काऊ *एतत्; }
  चालक T () अचर { फर्काऊ get_relaxed (); }

  शून्य set_relaxed (T v_) { v = v_; }
  शून्य set_release (T v_) { v = v_; }
  T get_relaxed () अचर { फर्काऊ v; }
  T get_acquire () अचर { फर्काऊ v; }
  T inc () { फर्काऊ v++; }
  T dec () { फर्काऊ v--; }

  पूर्णाङ्क चालक ++ (पूर्णाङ्क) { फर्काऊ inc (); }
  पूर्णाङ्क चालक -- (पूर्णाङ्क) { फर्काऊ dec (); }

  T v = 0;
};

ढाँचा <typename T>
संरचना hb_atomic_t<T*>
{
  hb_atomic_t () = स्वभाव;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (अचर hb_atomic_t &other) = delete;

  शून्य init (T* v_ = nullptr) { set_relaxed (v_); }
  शून्य set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () अचर { फर्काऊ v; }
  T *get_acquire () अचर { फर्काऊ v; }
  तार्किक cmpexch (T *old, T *new_)
  {
    यदि (v != old) फर्काऊ असत्य;
    v = new_;
    फर्काऊ सत्य;
  }

  चालक तार्किक () अचर { फर्काऊ get_acquire () != nullptr; }
  T * चालक -> () अचर                    { फर्काऊ get_acquire (); }
  ढाँचा <typename C> चालक C * () अचर { फर्काऊ get_acquire (); }

  T *v = nullptr;
};

#endif


स्थिर अन्तःस्थ तार्किक hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  फर्काऊ सत्य;
}


#endif /* HB_ATOMIC_HH */
