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
#जर defined(__ATOMIC_ACQUIRE) // gcc-like
स्थिर ओळीत शून्य _hb_compiler_memory_r_barrier () { यंत्र अस्थिर("": : :"memory"); }
#किंवाजर !defined(_MSC_VER)
#अन्तर्भाव <atomic>
#define _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#अन्यथा
स्थिर ओळीत शून्य _hb_compiler_memory_r_barrier () {}
#endif
#endif


#ifndef HB_NO_MT

ढाँचा <typename T>
संरचना hb_atomic_t
{
  hb_atomic_t () = पूर्वनिर्धारित;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (स्थिरांक hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &चालक= (स्थिरांक hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); परत *हे; }
  hb_atomic_t &चालक= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); परत *हे; }
  hb_atomic_t &चालक= (T v_)
  {
    set_relaxed (v_);
    परत *हे;
  }
  चालक T () स्थिरांक { परत get_relaxed (); }

  शून्य set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  शून्य set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () स्थिरांक { परत v.load (std::memory_order_relaxed); }
  T get_acquire () स्थिरांक { परत v.load (std::memory_order_acquire); }
  T inc () { परत v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { परत v.fetch_add (-1, std::memory_order_acq_rel); }

  पूर्णांक चालक++ (पूर्णांक) { परत inc (); }
  पूर्णांक चालक-- (पूर्णांक) { परत dec (); }

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
  hb_atomic_t () = पूर्वनिर्धारित;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (स्थिरांक hb_atomic_t &other) = delete;

  शून्य init (T *v_ = nullptr) { set_relaxed (v_); }
  शून्य set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () स्थिरांक { परत v.load (std::memory_order_relaxed); }
  T *get_acquire () स्थिरांक { परत v.load (std::memory_order_acquire); }
  तार्किक cmpexch (T *old, T *new_) { परत v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &चालक= (स्थिरांक hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); परत *हे; }
  hb_atomic_t &चालक= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); परत *हे; }

  चालक तार्किक () स्थिरांक { परत get_acquire () != nullptr; }
  T *चालक->() स्थिरांक { परत get_acquire (); }
  ढाँचा <typename C>
  चालक C * () स्थिरांक
  {
    परत get_acquire ();
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
  hb_atomic_t () = पूर्वनिर्धारित;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& चालक = (T v_) { set_relaxed (v_); परत *हे; }
  चालक T () स्थिरांक { परत get_relaxed (); }

  शून्य set_relaxed (T v_) { v = v_; }
  शून्य set_release (T v_) { v = v_; }
  T get_relaxed () स्थिरांक { परत v; }
  T get_acquire () स्थिरांक { परत v; }
  T inc () { परत v++; }
  T dec () { परत v--; }

  पूर्णांक चालक ++ (पूर्णांक) { परत inc (); }
  पूर्णांक चालक -- (पूर्णांक) { परत dec (); }

  T v = 0;
};

ढाँचा <typename T>
संरचना hb_atomic_t<T*>
{
  hb_atomic_t () = पूर्वनिर्धारित;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (स्थिरांक hb_atomic_t &other) = delete;

  शून्य init (T* v_ = nullptr) { set_relaxed (v_); }
  शून्य set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () स्थिरांक { परत v; }
  T *get_acquire () स्थिरांक { परत v; }
  तार्किक cmpexch (T *old, T *new_)
  {
    जर (v != old) परत असत्य;
    v = new_;
    परत सत्य;
  }

  चालक तार्किक () स्थिरांक { परत get_acquire () != nullptr; }
  T * चालक -> () स्थिरांक                    { परत get_acquire (); }
  ढाँचा <typename C> चालक C * () स्थिरांक { परत get_acquire (); }

  T *v = nullptr;
};

#endif


स्थिर ओळीत तार्किक hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  परत सत्य;
}


#endif /* HB_ATOMIC_HH */
