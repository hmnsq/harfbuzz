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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-blob.hh"

#ಅನ್ತರ್ಭಾವ "hb-dispatch.hh"
#ಅನ್ತರ್ಭಾವ "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
template<typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ Type& StructAtOffset(ಧ್ರುವ ಶೂನ್ಯ *P, unsigned ಪೂರ್ಣಾಙ್ಕ offset)
{ ಪ್ರತ್ಯಾಗಮ * reinterpret_cast<ಧ್ರುವ Type*> ((ಧ್ರುವ ವರ್ಣ *) P + offset); }
template<typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ Type& StructAtOffset(ಶೂನ್ಯ *P, unsigned ಪೂರ್ಣಾಙ್ಕ offset)
{ ಪ್ರತ್ಯಾಗಮ * reinterpret_cast<Type*> ((ವರ್ಣ *) P + offset); }
template<typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ Type& StructAtOffsetUnaligned(ಧ್ರುವ ಶೂನ್ಯ *P, unsigned ಪೂರ್ಣಾಙ್ಕ offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ಪ್ರತ್ಯಾಗಮ * reinterpret_cast<ಧ್ರುವ Type*> ((ಧ್ರುವ ವರ್ಣ *) P + offset);
#pragma GCC diagnostic ತೆಗೆ
}
template<typename Type>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ Type& StructAtOffsetUnaligned(ಶೂನ್ಯ *P, unsigned ಪೂರ್ಣಾಙ್ಕ offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ಪ್ರತ್ಯಾಗಮ * reinterpret_cast<Type*> ((ವರ್ಣ *) P + offset);
#pragma GCC diagnostic ತೆಗೆ
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto StructAfter(ಧ್ರುವ TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
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
#define _DEFINE_INSTANCE_ASSERTION1(ಸಾಲು_2, _assertion) \
  ಶೂನ್ಯ instance_assertion_on_ಸಾಲು##_line () ಧ್ರುವ \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(ಸಾಲು_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (ಸಾಲು_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(ಸಾಲು_2, _code) \
  ಶೂನ್ಯ compiles_assertion_on_ಸಾಲು##_line () ಧ್ರುವ \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(ಸಾಲು_2, _code) _DEFINE_COMPILES_ASSERTION1 (ಸಾಲು_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (ಪರಿಮಾಣ (*ಏತತ್) == (size)) \
  size_t get_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (size); } \
  ಸ್ಥಿರ constexpr unsigned null_size = (size); \
  ಸ್ಥಿರ constexpr unsigned min_size = (size); \
  ಸ್ಥಿರ constexpr unsigned static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((ಶೂನ್ಯ) ಏತತ್->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (ಪರಿಮಾಣ(ಏತತ್->u._member) == (size)) \
  ಸ್ಥಿರ constexpr unsigned null_size = (size); \
  ಸ್ಥಿರ constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (ಪರಿಮಾಣ (*ಏತತ್) >= (size)) \
  ಸ್ಥಿರ constexpr unsigned null_size = (size); \
  ಸ್ಥಿರ constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (ಪರಿಮಾಣ (*ಏತತ್) >= (size)) \
  ಸ್ಥಿರ constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((ಶೂನ್ಯ) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (ಪರಿಮಾಣ (*ಏತತ್) == (size) + (HB_VAR_ARRAY+0) * ಪರಿಮಾಣ ((array)[0])) \
  ಸ್ಥಿರ constexpr unsigned null_size = (size); \
  ಸ್ಥಿರ constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  size_t get_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (size - (array).min_size + (array).get_size ()); } \
  DEFINE_SIZE_ARRAY(size, array)


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

template <typename Data, unsigned ಪೂರ್ಣಾಙ್ಕ WheresData>
ಸಂರಚನಾ hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ *(((Data **) (ಶೂನ್ಯ *) ಏತತ್) - WheresData); }

  ತಾರ್ಕಿಕ is_inert () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !get_data (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ Subclass::create (get_data ()); }
};
template <>
ಸಂರಚನಾ hb_data_wrapper_t<ಶೂನ್ಯ, 0>
{
  ತಾರ್ಕಿಕ is_inert () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }

  template <typename Stored, typename Funcs>
  Stored * call_create () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ Funcs::create (); }
};

template <typename T1, typename T2> ಸಂರಚನಾ hb_non_void_t { ಪ್ರಕಾರನಿರ್ದೇಶ T1 value; };
template <typename T2> ಸಂರಚನಾ hb_non_void_t<ಶೂನ್ಯ, T2> { ಪ್ರಕಾರನಿರ್ದೇಶ T2 value; };

template <typename Returned,
	  typename Subclass = ಶೂನ್ಯ,
	  typename Data = ಶೂನ್ಯ,
	  unsigned ಪೂರ್ಣಾಙ್ಕ WheresData = 0,
	  typename Stored = Returned>
ಸಂರಚನಾ hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = ಸ್ವಭಾವ;
  hb_lazy_loader_t (ಧ್ರುವ hb_lazy_loader_t &other) = delete;

  ಶೂನ್ಯ init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  ಶೂನ್ಯ init ()  { instance.set_relaxed (nullptr); }
  ಶೂನ್ಯ fini ()  { do_destroy (instance.get_acquire ()); init (); }

  ಶೂನ್ಯ free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    ಯದಿ (unlikely (p && !cmpexch (p, nullptr)))
      ಗಚ್ಛ retry;
    do_destroy (p);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ do_destroy (Stored *p)
  {
    ಯದಿ (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      ಪ್ರತ್ಯಾಗಮ;
    Funcs::destroy (p);
  }

  ಧ್ರುವ Returned * operator -> () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, ಶೂನ್ಯ))>
  ಧ್ರುವ U & operator * () ಧ್ರುವ  { ಪ್ರತ್ಯಾಗಮ *get (); }
  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ
  {
    Stored *p = get_stored ();
    ಯದಿ (Funcs::tag_null)
    {
      Stored *raw = ಏತತ್->instance.get_relaxed ();
      ಪ್ರತ್ಯಾಗಮ raw && !is_tagged_null (raw);
    }
    ಪ್ರತ್ಯಾಗಮ p != Funcs::get_null ();
  }
  template <typename C> operator ಧ್ರುವ C * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (); }

  Stored * get_stored () ಧ್ರುವ
  {
  retry:
    Stored *p = ಏತತ್->instance.get_acquire ();
    ಯದಿ (unlikely (!p))
    {
      ಯದಿ (unlikely (ಏತತ್->is_inert ()))
	ಪ್ರತ್ಯಾಗಮ const_cast<Stored *> (Funcs::get_null ());

      p = ಏತತ್->template call_create<Stored, Funcs> ();
      ಯದಿ (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      ಯದಿ (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	ಗಚ್ಛ retry;
      }
    }
    ಪ್ರತ್ಯಾಗಮ untagged (p);
  }
  Stored * get_stored_relaxed () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ untagged (ಏತತ್->instance.get_relaxed ());
  }

  ತಾರ್ಕಿಕ cmpexch (Stored *current, Stored *value) ಧ್ರುವ
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->instance.cmpexch (current, value);
  }

  ಧ್ರುವ Returned * get () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ Funcs::convert (get_stored ()); }
  ಧ್ರುವ Returned * get_relaxed () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  ಸ್ಥಿರ Returned* convert (Stored *p) { ಪ್ರತ್ಯಾಗಮ p; }

  /* By default null/init/fini the object. */
  ಸ್ಥಿರ ಧ್ರುವ Stored* get_null () { ಪ್ರತ್ಯಾಗಮ &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ tag_null = ಅಸತ್ಯ;
  ಸ್ಥಿರ ತಾರ್ಕಿಕ is_tagged_null (Stored *p)
  { ಪ್ರತ್ಯಾಗಮ Funcs::tag_null && ((uintptr_t) (ಶೂನ್ಯ *) p & 1); }
  ಸ್ಥಿರ Stored *tagged_null (Stored *p)
  { ಪ್ರತ್ಯಾಗಮ Funcs::tag_null ? (Stored *) (ಶೂನ್ಯ *) ((uintptr_t) (ಶೂನ್ಯ *) p | 1) : p; }
  ಸ್ಥಿರ Stored *untagged (Stored *p)
  { ಪ್ರತ್ಯಾಗಮ Funcs::tag_null ? (Stored *) (ಶೂನ್ಯ *) ((uintptr_t) (ಶೂನ್ಯ *) p & ~(uintptr_t) 1) : p; }
  ಸ್ಥಿರ Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, ಪರಿಮಾಣ (Stored));
    ಯದಿ (likely (p))
      p = ನೂತನ (p) Stored (data);
    ಪ್ರತ್ಯಾಗಮ p;
  }
  ಸ್ಥಿರ Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, ಪರಿಮಾಣ (Stored));
    ಯದಿ (likely (p))
      p = ನೂತನ (p) Stored ();
    ಪ್ರತ್ಯಾಗಮ p;
  }
  ಸ್ಥಿರ ಶೂನ್ಯ destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  ಗುಪ್ತ:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned ಪೂರ್ಣಾಙ್ಕ WheresFace>
ಸಂರಚನಾ hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ tag_null = ಸತ್ಯ;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { ಪ್ರತ್ಯಾಗಮ ಏತತ್->get ()->get_blob (); }
};

template <typename T, unsigned ಪೂರ್ಣಾಙ್ಕ WheresFace>
ಸಂರಚನಾ hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  ಸ್ಥಿರ hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    ಪ್ರತ್ಯಾಗಮ c.reference_table<T> (face);
  }
  ಸ್ಥಿರ ಶೂನ್ಯ destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  ಸ್ಥಿರ ಧ್ರುವ hb_blob_t *get_null ()
  { ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty (); }

  ಸ್ಥಿರ ಧ್ರುವ T* convert (ಧ್ರುವ hb_blob_t *blob)
  { ಪ್ರತ್ಯಾಗಮ blob->as<T> (); }

  hb_blob_t* get_blob () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  ಸಂರಚನಾ hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    ಸ್ಥಿರ ಶೂನ್ಯ destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    ಸ್ಥಿರ ಧ್ರುವ hb_##Type##_funcs_t *get_null () \
    { ಪ್ರತ್ಯಾಗಮ hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
