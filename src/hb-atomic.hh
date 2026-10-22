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

#अगर_अघोषित HB_ATOMIC_HH
#घोषणा HB_ATOMIC_HH

#समावेश "hb.hh"
#समावेश "hb-meta.hh"


#अगर_अघोषित HB_NO_MT
#समावेश <atomic>
#पूर्ण_अगर


/* This should never be disabled, even under HB_NO_MT.
 * except that MSVC gives me an internal compiler error, so disabled there.
 *
 * https://github.com/harfbuzz/harfbuzz/pull/4119
 */
#अगर_अघोषित _hb_compiler_memory_r_barrier
#अगर defined(__ATOMIC_ACQUIRE) // gcc-like
अटल अंतरभूत व्योम _hb_compiler_memory_r_barrier () { यंत्र अस्थिर("": : :"memory"); }
#या_अगर !defined(_MSC_VER)
#समावेश <atomic>
#घोषणा _hb_compiler_memory_r_barrier() std::atomic_signal_fence (std::memory_order_acquire)
#अन्यथा
अटल अंतरभूत व्योम _hb_compiler_memory_r_barrier () {}
#पूर्ण_अगर
#पूर्ण_अगर


#अगर_अघोषित HB_NO_MT

ढाँचा <typename T>
काष्ठा hb_atomic_t
{
  hb_atomic_t () = शेष;
  constexpr hb_atomic_t (T v) : v (v) {}
  constexpr hb_atomic_t (स्थिर hb_atomic_t& o) : v (o.get_relaxed ()) {}
  hb_atomic_t (hb_atomic_t&& o) : v (o.get_relaxed ()) { o.set_relaxed ({}); }

  hb_atomic_t &चालक= (स्थिर hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); वापस *यह; }
  hb_atomic_t &चालक= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); वापस *यह; }
  hb_atomic_t &चालक= (T v_)
  {
    set_relaxed (v_);
    वापस *यह;
  }
  चालक T () स्थिर { वापस get_relaxed (); }

  व्योम set_relaxed (T v_) { v.store (v_, std::memory_order_relaxed); }
  व्योम set_release (T v_) { v.store (v_, std::memory_order_release); }
  T get_relaxed () स्थिर { वापस v.load (std::memory_order_relaxed); }
  T get_acquire () स्थिर { वापस v.load (std::memory_order_acquire); }
  T inc () { वापस v.fetch_add (1, std::memory_order_acq_rel); }
  T डेसी () { वापस v.fetch_add (-1, std::memory_order_acq_rel); }

  पूर्णांक चालक++ (पूर्णांक) { वापस inc (); }
  पूर्णांक चालक-- (पूर्णांक) { वापस डेसी (); }

  मित्र व्योम swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T v = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (v);
  }

  std::atomic<T> v {0};
};

ढाँचा <typename T>
काष्ठा hb_atomic_t<T *>
{
  hb_atomic_t () = शेष;
  constexpr hb_atomic_t (T *v) : v (v) {}
  hb_atomic_t (स्थिर hb_atomic_t &other) = मिटाओ;

  व्योम init (T *v_ = nullptr) { set_relaxed (v_); }
  व्योम set_relaxed (T *v_) { v.store (v_, std::memory_order_relaxed); }
  T *get_relaxed () स्थिर { वापस v.load (std::memory_order_relaxed); }
  T *get_acquire () स्थिर { वापस v.load (std::memory_order_acquire); }
  bool cmpexch (T *old, T *new_) { वापस v.compare_exchange_weak (old, new_, std::memory_order_acq_rel, std::memory_order_relaxed); }

  hb_atomic_t &चालक= (स्थिर hb_atomic_t& o) { set_relaxed (o.get_relaxed ()); वापस *यह; }
  hb_atomic_t &चालक= (hb_atomic_t&& o){ set_relaxed (o.get_relaxed ()); o.set_relaxed ({}); वापस *यह; }

  चालक bool () स्थिर { वापस get_acquire () != nullptr; }
  T *चालक->() स्थिर { वापस get_acquire (); }
  ढाँचा <typename C>
  चालक C * () स्थिर
  {
    वापस get_acquire ();
  }

  मित्र व्योम swap (hb_atomic_t &a, hb_atomic_t &b) noexcept
  {
    T *p = a.get_acquire ();
    a.set_relaxed (b.get_acquire ());
    b.set_relaxed (p);
  }

  std::atomic<T *> v {nullptr};
};

#अन्यथा /* defined(HB_NO_MT) */

ढाँचा <typename T>
काष्ठा hb_atomic_t
{
  hb_atomic_t () = शेष;
  constexpr hb_atomic_t (T v) : v (v) {}

  hb_atomic_t& चालक = (T v_) { set_relaxed (v_); वापस *यह; }
  चालक T () स्थिर { वापस get_relaxed (); }

  व्योम set_relaxed (T v_) { v = v_; }
  व्योम set_release (T v_) { v = v_; }
  T get_relaxed () स्थिर { वापस v; }
  T get_acquire () स्थिर { वापस v; }
  T inc () { वापस v++; }
  T डेसी () { वापस v--; }

  पूर्णांक चालक ++ (पूर्णांक) { वापस inc (); }
  पूर्णांक चालक -- (पूर्णांक) { वापस डेसी (); }

  T v = 0;
};

ढाँचा <typename T>
काष्ठा hb_atomic_t<T*>
{
  hb_atomic_t () = शेष;
  constexpr hb_atomic_t (T* v) : v (v) {}
  hb_atomic_t (स्थिर hb_atomic_t &other) = मिटाओ;

  व्योम init (T* v_ = nullptr) { set_relaxed (v_); }
  व्योम set_relaxed (T* v_) { v = v_; }
  T *get_relaxed () स्थिर { वापस v; }
  T *get_acquire () स्थिर { वापस v; }
  bool cmpexch (T *old, T *new_)
  {
    अगर (v != old) वापस false;
    v = new_;
    वापस true;
  }

  चालक bool () स्थिर { वापस get_acquire () != nullptr; }
  T * चालक -> () स्थिर                    { वापस get_acquire (); }
  ढाँचा <typename C> चालक C * () स्थिर { वापस get_acquire (); }

  T *v = nullptr;
};

#पूर्ण_अगर


अटल अंतरभूत bool hb_barrier ()
{
  _hb_compiler_memory_r_barrier ();
  वापस true;
}


#पूर्ण_अगर /* HB_ATOMIC_HH */
