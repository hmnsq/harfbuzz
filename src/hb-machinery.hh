/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012,2018  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_MACHINERY_HH
#define HB_MACHINERY_HH

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-blob.hh"

#অন্তর্ভাব "hb-dispatch.hh"
#অন্তর্ভাব "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
template<typename Type>
স্থির অন্তঃরেখ ধ্রুবক Type& StructAtOffset(ধ্রুবক শূন্য *P, unsigned পূর্ণসংখ্যা offset)
{ ফেরত * reinterpret_cast<ধ্রুবক Type*> ((ধ্রুবক অক্ষর *) P + offset); }
template<typename Type>
স্থির অন্তঃরেখ Type& StructAtOffset(শূন্য *P, unsigned পূর্ণসংখ্যা offset)
{ ফেরত * reinterpret_cast<Type*> ((অক্ষর *) P + offset); }
template<typename Type>
স্থির অন্তঃরেখ ধ্রুবক Type& StructAtOffsetUnaligned(ধ্রুবক শূন্য *P, unsigned পূর্ণসংখ্যা offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ফেরত * reinterpret_cast<ধ্রুবক Type*> ((ধ্রুবক অক্ষর *) P + offset);
#pragma GCC diagnostic বের_করো
}
template<typename Type>
স্থির অন্তঃরেখ Type& StructAtOffsetUnaligned(শূন্য *P, unsigned পূর্ণসংখ্যা offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ফেরত * reinterpret_cast<Type*> ((অক্ষর *) P + offset);
#pragma GCC diagnostic বের_করো
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
স্থির অন্তঃরেখ auto StructAfter(ধ্রুবক TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_আকার(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
স্থির অন্তঃরেখ auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  আকার(পূর্ণসংখ্যা[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
  StructAtOffset<Type>(&X, X.get_আকার(std::forward<Ts> (args)...))
  : *reinterpret_cast<Type*> (0)
))


/*
 * Size checking
 */

/* Size signifying variable-sized array */
#ifndef HB_VAR_ARRAY
#define HB_VAR_ARRAY 1
#endif

/* Check _assertion in a method environment */
#define _DEFINE_INSTANCE_ASSERTION1(সারি_2, _assertion) \
  শূন্য instance_assertion_on_সারি##_line () ধ্রুবক \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(সারি_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (সারি_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(সারি_2, _code) \
  শূন্য compiles_assertion_on_সারি##_line () ধ্রুবক \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(সারি_2, _code) _DEFINE_COMPILES_ASSERTION1 (সারি_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_আকার_STATIC(আকার_2) \
  DEFINE_INSTANCE_ASSERTION (আকার (*এই) == (আকার_2)) \
  size_t get_আকার () ধ্রুবক { ফেরত (আকার_2); } \
  স্থির constexpr unsigned null_আকার = (আকার_2); \
  স্থির constexpr unsigned min_আকার = (আকার_2); \
  স্থির constexpr unsigned static_আকার = (আকার_2)

#define DEFINE_আকার_UNION(আকার_2, _member) \
  DEFINE_COMPILES_ASSERTION ((শূন্য) এই->u._member.static_আকার) \
  DEFINE_INSTANCE_ASSERTION (আকার(এই->u._member) == (আকার_2)) \
  স্থির constexpr unsigned null_আকার = (আকার_2); \
  স্থির constexpr unsigned min_আকার = (আকার_2)

#define DEFINE_আকার_MIN(আকার_2) \
  DEFINE_INSTANCE_ASSERTION (আকার (*এই) >= (আকার_2)) \
  স্থির constexpr unsigned null_আকার = (আকার_2); \
  স্থির constexpr unsigned min_আকার = (আকার_2)

#define DEFINE_আকার_UNBOUNDED(আকার_2) \
  DEFINE_INSTANCE_ASSERTION (আকার (*এই) >= (আকার_2)) \
  স্থির constexpr unsigned min_আকার = (আকার_2)

#define DEFINE_আকার_ARRAY(আকার_2, array) \
  DEFINE_COMPILES_ASSERTION ((শূন্য) (array)[0].static_আকার) \
  DEFINE_INSTANCE_ASSERTION (আকার (*এই) == (আকার_2) + (HB_VAR_ARRAY+0) * আকার ((array)[0])) \
  স্থির constexpr unsigned null_আকার = (আকার_2); \
  স্থির constexpr unsigned min_আকার = (আকার_2)

#define DEFINE_আকার_ARRAY_SIZED(আকার_2, array) \
  size_t get_আকার () ধ্রুবক { ফেরত (আকার_2 - (array).min_আকার + (array).get_আকার ()); } \
  DEFINE_আকার_ARRAY(আকার_2, array)


/*
 * Lazy loaders.
 *
 * The lazy-loaders are thread-safe pointer-like objects that create their
 * instead on-demand.  They also support access to a "data" object that is
 * necessary for creating their instance.  The data object, if specified,
 * is accessed via pointer math, located at a location before the position
 * of the loader itself.  This avoids having to store a pointer to data
 * for every lazy-loader.  Multiple lazy-loaders can access the same data.
 */

template <typename তথ্য_3, unsigned পূর্ণসংখ্যা Wheres_তথ্য>
কাঠামো hb_তথ্য_wrapper_t
{
  static_assert (Wheres_তথ্য > 0, "");

  তথ্য_3 * get_তথ্য () ধ্রুবক
  { ফেরত *(((তথ্য_3 **) (শূন্য *) এই) - Wheres_তথ্য); }

  যৌক্তিক is_inert () ধ্রুবক { ফেরত !get_তথ্য (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () ধ্রুবক { ফেরত Subclass::create (get_তথ্য ()); }
};
template <>
কাঠামো hb_তথ্য_wrapper_t<শূন্য, 0>
{
  যৌক্তিক is_inert () ধ্রুবক { ফেরত মিথ্যা; }

  template <typename Stored, typename Funcs>
  Stored * call_create () ধ্রুবক { ফেরত Funcs::create (); }
};

template <typename T1, typename T2> কাঠামো hb_non_void_t { প্রকারলেখ T1 value; };
template <typename T2> কাঠামো hb_non_void_t<শূন্য, T2> { প্রকারলেখ T2 value; };

template <typename Returned,
	  typename Subclass = শূন্য,
	  typename তথ্য_3 = শূন্য,
	  unsigned পূর্ণসংখ্যা Wheres_তথ্য = 0,
	  typename Stored = Returned>
কাঠামো hb_lazy_loader_t : hb_তথ্য_wrapper_t<তথ্য_3, Wheres_তথ্য>
{
  প্রকারলেখ typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,তথ্য_3,Wheres_তথ্য,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = পূর্বনির্ধারিত;
  hb_lazy_loader_t (ধ্রুবক hb_lazy_loader_t &other) = delete;

  শূন্য init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  শূন্য init ()  { instance.set_relaxed (nullptr); }
  শূন্য fini ()  { do_destroy (instance.get_acquire ()); init (); }

  শূন্য free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    যদি (unlikely (p && !cmpexch (p, nullptr)))
      যাও retry;
    do_destroy (p);
  }

  স্থির শূন্য do_destroy (Stored *p)
  {
    যদি (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      ফেরত;
    Funcs::destroy (p);
  }

  ধ্রুবক Returned * operator -> () ধ্রুবক { ফেরত get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, শূন্য))>
  ধ্রুবক U & operator * () ধ্রুবক  { ফেরত *get (); }
  explicit operator যৌক্তিক () ধ্রুবক
  {
    Stored *p = get_stored ();
    যদি (Funcs::tag_null)
    {
      Stored *raw = এই->instance.get_relaxed ();
      ফেরত raw && !is_tagged_null (raw);
    }
    ফেরত p != Funcs::get_null ();
  }
  template <typename C> operator ধ্রুবক C * () ধ্রুবক { ফেরত get (); }

  Stored * get_stored () ধ্রুবক
  {
  retry:
    Stored *p = এই->instance.get_acquire ();
    যদি (unlikely (!p))
    {
      যদি (unlikely (এই->is_inert ()))
	ফেরত const_cast<Stored *> (Funcs::get_null ());

      p = এই->template call_create<Stored, Funcs> ();
      যদি (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      যদি (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	যাও retry;
      }
    }
    ফেরত untagged (p);
  }
  Stored * get_stored_relaxed () ধ্রুবক
  {
    ফেরত untagged (এই->instance.get_relaxed ());
  }

  যৌক্তিক cmpexch (Stored *current, Stored *value) ধ্রুবক
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    ফেরত এই->instance.cmpexch (current, value);
  }

  ধ্রুবক Returned * get () ধ্রুবক { ফেরত Funcs::convert (get_stored ()); }
  ধ্রুবক Returned * get_relaxed () ধ্রুবক { ফেরত Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () ধ্রুবক { ফেরত const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  স্থির Returned* convert (Stored *p) { ফেরত p; }

  /* By default null/init/fini the object. */
  স্থির ধ্রুবক Stored* get_null () { ফেরত &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  স্থির constexpr যৌক্তিক tag_null = মিথ্যা;
  স্থির যৌক্তিক is_tagged_null (Stored *p)
  { ফেরত Funcs::tag_null && ((uintptr_t) (শূন্য *) p & 1); }
  স্থির Stored *tagged_null (Stored *p)
  { ফেরত Funcs::tag_null ? (Stored *) (শূন্য *) ((uintptr_t) (শূন্য *) p | 1) : p; }
  স্থির Stored *untagged (Stored *p)
  { ফেরত Funcs::tag_null ? (Stored *) (শূন্য *) ((uintptr_t) (শূন্য *) p & ~(uintptr_t) 1) : p; }
  স্থির Stored *create (তথ্য_3 *তথ্য)
  {
    Stored *p = (Stored *) hb_calloc (1, আকার (Stored));
    যদি (likely (p))
      p = নতুন (p) Stored (তথ্য);
    ফেরত p;
  }
  স্থির Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, আকার (Stored));
    যদি (likely (p))
      p = নতুন (p) Stored ();
    ফেরত p;
  }
  স্থির শূন্য destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  ব্যক্তিগত:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned পূর্ণসংখ্যা WheresFace>
কাঠামো hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  স্থির constexpr যৌক্তিক tag_null = সত্য;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { ফেরত এই->get ()->get_blob (); }
};

template <typename T, unsigned পূর্ণসংখ্যা WheresFace>
কাঠামো hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  স্থির hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    ফেরত c.reference_table<T> (face);
  }
  স্থির শূন্য destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  স্থির ধ্রুবক hb_blob_t *get_null ()
  { ফেরত hb_blob_get_empty (); }

  স্থির ধ্রুবক T* convert (ধ্রুবক hb_blob_t *blob)
  { ফেরত blob->as<T> (); }

  hb_blob_t* get_blob () ধ্রুবক { ফেরত এই->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  কাঠামো hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    স্থির শূন্য destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    স্থির ধ্রুবক hb_##Type##_funcs_t *get_null () \
    { ফেরত hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
