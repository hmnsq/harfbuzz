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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-blob.hh"

#ଅନ୍ତର୍ଭାଵ "hb-dispatch.hh"
#ଅନ୍ତର୍ଭାଵ "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
template<typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ Type& StructAtOffset(ଅଚଳ ଶୂନ୍ୟ *P, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset)
{ ଫେରନ୍ତୁ * reinterpret_cast<ଅଚଳ Type*> ((ଅଚଳ ଅକ୍ଷର *) P + offset); }
template<typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ Type& StructAtOffset(ଶୂନ୍ୟ *P, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset)
{ ଫେରନ୍ତୁ * reinterpret_cast<Type*> ((ଅକ୍ଷର *) P + offset); }
template<typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ Type& StructAtOffsetUnaligned(ଅଚଳ ଶୂନ୍ୟ *P, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ଫେରନ୍ତୁ * reinterpret_cast<ଅଚଳ Type*> ((ଅଚଳ ଅକ୍ଷର *) P + offset);
#pragma GCC diagnostic କାଢ଼
}
template<typename Type>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ Type& StructAtOffsetUnaligned(ଶୂନ୍ୟ *P, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ଫେରନ୍ତୁ * reinterpret_cast<Type*> ((ଅକ୍ଷର *) P + offset);
#pragma GCC diagnostic କାଢ଼
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto StructAfter(ଅଚଳ TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
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
#define _DEFINE_INSTANCE_ASSERTION1(ଧାଡ଼ି_2, _assertion) \
  ଶୂନ୍ୟ instance_assertion_on_ଧାଡ଼ି##_line () ଅଚଳ \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(ଧାଡ଼ି_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (ଧାଡ଼ି_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(ଧାଡ଼ି_2, _code) \
  ଶୂନ୍ୟ compiles_assertion_on_ଧାଡ଼ି##_line () ଅଚଳ \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(ଧାଡ଼ି_2, _code) _DEFINE_COMPILES_ASSERTION1 (ଧାଡ଼ି_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (ଆକାର (*ଏହି) == (size)) \
  size_t get_size () ଅଚଳ { ଫେରନ୍ତୁ (size); } \
  ସ୍ଥିର constexpr unsigned null_size = (size); \
  ସ୍ଥିର constexpr unsigned min_size = (size); \
  ସ୍ଥିର constexpr unsigned static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((ଶୂନ୍ୟ) ଏହି->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (ଆକାର(ଏହି->u._member) == (size)) \
  ସ୍ଥିର constexpr unsigned null_size = (size); \
  ସ୍ଥିର constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (ଆକାର (*ଏହି) >= (size)) \
  ସ୍ଥିର constexpr unsigned null_size = (size); \
  ସ୍ଥିର constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (ଆକାର (*ଏହି) >= (size)) \
  ସ୍ଥିର constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((ଶୂନ୍ୟ) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (ଆକାର (*ଏହି) == (size) + (HB_VAR_ARRAY+0) * ଆକାର ((array)[0])) \
  ସ୍ଥିର constexpr unsigned null_size = (size); \
  ସ୍ଥିର constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  size_t get_size () ଅଚଳ { ଫେରନ୍ତୁ (size - (array).min_size + (array).get_size ()); } \
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

template <typename Data, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା WheresData>
ସଂରଚନା hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () ଅଚଳ
  { ଫେରନ୍ତୁ *(((Data **) (ଶୂନ୍ୟ *) ଏହି) - WheresData); }

  ତାର୍କିକ is_inert () ଅଚଳ { ଫେରନ୍ତୁ !get_data (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () ଅଚଳ { ଫେରନ୍ତୁ Subclass::create (get_data ()); }
};
template <>
ସଂରଚନା hb_data_wrapper_t<ଶୂନ୍ୟ, 0>
{
  ତାର୍କିକ is_inert () ଅଚଳ { ଫେରନ୍ତୁ ମିଥ୍ୟା; }

  template <typename Stored, typename Funcs>
  Stored * call_create () ଅଚଳ { ଫେରନ୍ତୁ Funcs::create (); }
};

template <typename T1, typename T2> ସଂରଚନା hb_non_void_t { ପ୍ରକାରବ୍ୟାଖ୍ୟା T1 value; };
template <typename T2> ସଂରଚନା hb_non_void_t<ଶୂନ୍ୟ, T2> { ପ୍ରକାରବ୍ୟାଖ୍ୟା T2 value; };

template <typename Returned,
	  typename Subclass = ଶୂନ୍ୟ,
	  typename Data = ଶୂନ୍ୟ,
	  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା WheresData = 0,
	  typename Stored = Returned>
ସଂରଚନା hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_lazy_loader_t (ଅଚଳ hb_lazy_loader_t &other) = delete;

  ଶୂନ୍ୟ init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  ଶୂନ୍ୟ init ()  { instance.set_relaxed (nullptr); }
  ଶୂନ୍ୟ fini ()  { do_destroy (instance.get_acquire ()); init (); }

  ଶୂନ୍ୟ free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    ଯଦି (unlikely (p && !cmpexch (p, nullptr)))
      ଯାଅ retry;
    do_destroy (p);
  }

  ସ୍ଥିର ଶୂନ୍ୟ do_destroy (Stored *p)
  {
    ଯଦି (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      ଫେରନ୍ତୁ;
    Funcs::destroy (p);
  }

  ଅଚଳ Returned * operator -> () ଅଚଳ { ଫେରନ୍ତୁ get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, ଶୂନ୍ୟ))>
  ଅଚଳ U & operator * () ଅଚଳ  { ଫେରନ୍ତୁ *get (); }
  explicit operator ତାର୍କିକ () ଅଚଳ
  {
    Stored *p = get_stored ();
    ଯଦି (Funcs::tag_null)
    {
      Stored *raw = ଏହି->instance.get_relaxed ();
      ଫେରନ୍ତୁ raw && !is_tagged_null (raw);
    }
    ଫେରନ୍ତୁ p != Funcs::get_null ();
  }
  template <typename C> operator ଅଚଳ C * () ଅଚଳ { ଫେରନ୍ତୁ get (); }

  Stored * get_stored () ଅଚଳ
  {
  retry:
    Stored *p = ଏହି->instance.get_acquire ();
    ଯଦି (unlikely (!p))
    {
      ଯଦି (unlikely (ଏହି->is_inert ()))
	ଫେରନ୍ତୁ const_cast<Stored *> (Funcs::get_null ());

      p = ଏହି->template call_create<Stored, Funcs> ();
      ଯଦି (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      ଯଦି (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	ଯାଅ retry;
      }
    }
    ଫେରନ୍ତୁ untagged (p);
  }
  Stored * get_stored_relaxed () ଅଚଳ
  {
    ଫେରନ୍ତୁ untagged (ଏହି->instance.get_relaxed ());
  }

  ତାର୍କିକ cmpexch (Stored *current, Stored *value) ଅଚଳ
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    ଫେରନ୍ତୁ ଏହି->instance.cmpexch (current, value);
  }

  ଅଚଳ Returned * get () ଅଚଳ { ଫେରନ୍ତୁ Funcs::convert (get_stored ()); }
  ଅଚଳ Returned * get_relaxed () ଅଚଳ { ଫେରନ୍ତୁ Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () ଅଚଳ { ଫେରନ୍ତୁ const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  ସ୍ଥିର Returned* convert (Stored *p) { ଫେରନ୍ତୁ p; }

  /* By default null/init/fini the object. */
  ସ୍ଥିର ଅଚଳ Stored* get_null () { ଫେରନ୍ତୁ &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  ସ୍ଥିର constexpr ତାର୍କିକ tag_null = ମିଥ୍ୟା;
  ସ୍ଥିର ତାର୍କିକ is_tagged_null (Stored *p)
  { ଫେରନ୍ତୁ Funcs::tag_null && ((uintptr_t) (ଶୂନ୍ୟ *) p & 1); }
  ସ୍ଥିର Stored *tagged_null (Stored *p)
  { ଫେରନ୍ତୁ Funcs::tag_null ? (Stored *) (ଶୂନ୍ୟ *) ((uintptr_t) (ଶୂନ୍ୟ *) p | 1) : p; }
  ସ୍ଥିର Stored *untagged (Stored *p)
  { ଫେରନ୍ତୁ Funcs::tag_null ? (Stored *) (ଶୂନ୍ୟ *) ((uintptr_t) (ଶୂନ୍ୟ *) p & ~(uintptr_t) 1) : p; }
  ସ୍ଥିର Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, ଆକାର (Stored));
    ଯଦି (likely (p))
      p = ନୂତନ (p) Stored (data);
    ଫେରନ୍ତୁ p;
  }
  ସ୍ଥିର Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, ଆକାର (Stored));
    ଯଦି (likely (p))
      p = ନୂତନ (p) Stored ();
    ଫେରନ୍ତୁ p;
  }
  ସ୍ଥିର ଶୂନ୍ୟ destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  ବ୍ୟକ୍ତିଗତ:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା WheresFace>
ସଂରଚନା hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  ସ୍ଥିର constexpr ତାର୍କିକ tag_null = ସତ୍ୟ;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { ଫେରନ୍ତୁ ଏହି->get ()->get_blob (); }
};

template <typename T, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା WheresFace>
ସଂରଚନା hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  ସ୍ଥିର hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    ଫେରନ୍ତୁ c.reference_table<T> (face);
  }
  ସ୍ଥିର ଶୂନ୍ୟ destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  ସ୍ଥିର ଅଚଳ hb_blob_t *get_null ()
  { ଫେରନ୍ତୁ hb_blob_get_empty (); }

  ସ୍ଥିର ଅଚଳ T* convert (ଅଚଳ hb_blob_t *blob)
  { ଫେରନ୍ତୁ blob->as<T> (); }

  hb_blob_t* get_blob () ଅଚଳ { ଫେରନ୍ତୁ ଏହି->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  ସଂରଚନା hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    ସ୍ଥିର ଶୂନ୍ୟ destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    ସ୍ଥିର ଅଚଳ hb_##Type##_funcs_t *get_null () \
    { ଫେରନ୍ତୁ hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
