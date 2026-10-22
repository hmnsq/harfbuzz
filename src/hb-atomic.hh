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

#నిర్వచితం_కాకపోతే HB_ATOMIC_HH
#నిర్వచించు HB_ATOMIC_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-meta.hh"


#నిర్వచితం_కాకపోతే HB_NO_MT
#అన్తర్భావ <atomic>
#అయితే_ముగింపు


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#నిర్వచితం_కాకపోతే hb_compiler_జ్ఞాపకం_r_barrier
#ఒకవేళ defined(__ATOMIC_ACQUIRE) // gcc-like
స్థిర అంతఃవరుస శూన్య hb_compiler_జ్ఞాపకం_r_barrier () { asm అస్థిర("": : :"memory"); }
#లేదావేళ !defined(_MSC_VER)
#అన్తర్భావ <atomic>
#నిర్వచించు hb_compiler_జ్ఞాపకం_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#లేకపోతే
స్థిర అంతఃవరుస శూన్య hb_compiler_జ్ఞాపకం_r_barrier () {}
#అయితే_ముగింపు
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే HB_NO_MT

మూస <రకం_పేరు T>
నిర్మాణం hb_atomic_t
{
  hb_atomic_t () = అప్రమేయ;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (స్థిరరాశి hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.సమితి_relaxed ({}); }

  hb_atomic_t &క్రియాకారకం= (స్థిరరాశి hb_atomic_t& o) { సమితి_relaxed (o.get_relaxed ()); తిరిగి *ఇది; }
  hb_atomic_t &క్రియాకారకం= (hb_atomic_t&& o){ సమితి_relaxed (o.get_relaxed ()); o.సమితి_relaxed ({}); తిరిగి *ఇది; }
  hb_atomic_t &క్రియాకారకం= (T v_)
  {
    సమితి_relaxed (v_);
    తిరిగి *ఇది;
  }
  క్రియాకారకం T () స్థిరరాశి { తిరిగి get_relaxed (); }

  శూన్య సమితి_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  శూన్య సమితి_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () స్థిరరాశి { తిరిగి v.load (std::memory_order_relaxed); }
  T get_acquire () స్థిరరాశి { తిరిగి v.load (std::memory_order_acquire); }
  T inc () { తిరిగి v.fetch_add (1, std::memory_order_acq_rel); }
  T dec () { తిరిగి v.fetch_add (-1, std::memory_order_acq_rel); }

  పూర్ణాంకం క్రియాకారకం++ (పూర్ణాంకం) { తిరిగి inc (); }
  పూర్ణాంకం క్రియాకారకం-- (పూర్ణాంకం) { తిరిగి dec (); }

  మిత్ర శూన్య swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.సమితి_relaxed (b.get_acquire ());
    b.సమితి_relaxed (v);
  }

  std::atomic<T> v {0};
};

మూస <రకం_పేరు T>
నిర్మాణం hb_atomic_t<T *>
{
  hb_atomic_t () = అప్రమేయ;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (స్థిరరాశి hb_atomic_t &other) = తొలగించు;

  శూన్య init (T *v_ = శూన్యసూచిక) { సమితి_relaxed (v_); }
  శూన్య సమితి_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () స్థిరరాశి { తిరిగి v.load (std::memory_order_relaxed); }
  T *get_acquire () స్థిరరాశి { తిరిగి v.load (std::memory_order_acquire); }
  తార్కిక cmpexch (T *old, T *new_) { తిరిగి v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &క్రియాకారకం= (స్థిరరాశి hb_atomic_t& o) { సమితి_relaxed (o.get_relaxed ()); తిరిగి *ఇది; }
  hb_atomic_t &క్రియాకారకం= (hb_atomic_t&& o){ సమితి_relaxed (o.get_relaxed ()); o.సమితి_relaxed ({}); తిరిగి *ఇది; }

  క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి get_acquire () != శూన్యసూచిక; }
  T *క్రియాకారకం->() స్థిరరాశి { తిరిగి get_acquire (); }
  మూస <రకం_పేరు C>
  క్రియాకారకం C * () స్థిరరాశి
  {
    తిరిగి get_acquire ();
  }

  మిత్ర శూన్య swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.సమితి_relaxed (b.get_acquire ());
    b.సమితి_relaxed (p);
  }

  std::atomic<T *> v {శూన్యసూచిక};
};

#లేకపోతే /* defined(HB_NO_MT) */

మూస <రకం_పేరు T>
నిర్మాణం hb_atomic_t
{
  hb_atomic_t () = అప్రమేయ;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& క్రియాకారకం = (T v_) { సమితి_relaxed (v_); తిరిగి *ఇది; }
  క్రియాకారకం T () స్థిరరాశి { తిరిగి get_relaxed (); }

  శూన్య సమితి_relaxed (T v_) { v = v_; }
  శూన్య సమితి_release (T v_) { v = v_; }
  T get_relaxed () స్థిరరాశి { తిరిగి v; }
  T get_acquire () స్థిరరాశి { తిరిగి v; }
  T inc () { తిరిగి v++; }
  T dec () { తిరిగి v--; }

  పూర్ణాంకం క్రియాకారకం ++ (పూర్ణాంకం) { తిరిగి inc (); }
  పూర్ణాంకం క్రియాకారకం -- (పూర్ణాంకం) { తిరిగి dec (); }

  T v = 0;
};

మూస <రకం_పేరు T>
నిర్మాణం hb_atomic_t<T*>
{
  hb_atomic_t () = అప్రమేయ;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (స్థిరరాశి hb_atomic_t &other) = తొలగించు;

  శూన్య init (T* v_ = శూన్యసూచిక) { సమితి_relaxed (v_); }
  శూన్య సమితి_relaxed (T* v_) { v = v_; }
  T *get_relaxed () స్థిరరాశి { తిరిగి v; }
  T *get_acquire () స్థిరరాశి { తిరిగి v; }
  తార్కిక cmpexch (T *old, T *new_)
  {
    ఒకవేళ (v != old) తిరిగి అబద్ధం;
    v = new_;
    తిరిగి నిజం;
  }

  క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి get_acquire () != శూన్యసూచిక; }
  T * క్రియాకారకం -> () స్థిరరాశి                    { తిరిగి get_acquire (); }
  మూస <రకం_పేరు C> క్రియాకారకం C * () స్థిరరాశి { తిరిగి get_acquire (); }

  T *v = శూన్యసూచిక;
};

#అయితే_ముగింపు


స్థిర అంతఃవరుస తార్కిక hb_barrier ()
{
  hb_compiler_జ్ఞాపకం_r_barrier ();
  తిరిగి నిజం;
}


#అయితే_ముగింపు /* HB_ATOMIC_HH */
