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
স্থির অন্তঃস্থ ধ্রুব Type& StructAtOffset(ধ্রুব শূন্য *P, unsigned পূর্ণাঙ্ক offset)
{ প্রত্যাগম * reinterpret_cast<ধ্রুব Type*> ((ধ্রুব বর্ণ *) P + offset); }
template<typename Type>
স্থির অন্তঃস্থ Type& StructAtOffset(শূন্য *P, unsigned পূর্ণাঙ্ক offset)
{ প্রত্যাগম * reinterpret_cast<Type*> ((বর্ণ *) P + offset); }
template<typename Type>
স্থির অন্তঃস্থ ধ্রুব Type& StructAtOffsetUnaligned(ধ্রুব শূন্য *P, unsigned পূর্ণাঙ্ক offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  প্রত্যাগম * reinterpret_cast<ধ্রুব Type*> ((ধ্রুব বর্ণ *) P + offset);
#pragma GCC diagnostic বের_করো
}
template<typename Type>
স্থির অন্তঃস্থ Type& StructAtOffsetUnaligned(শূন্য *P, unsigned পূর্ণাঙ্ক offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  প্রত্যাগম * reinterpret_cast<Type*> ((বর্ণ *) P + offset);
#pragma GCC diagnostic বের_করো
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
স্থির অন্তঃস্থ auto StructAfter(ধ্রুব TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
স্থির অন্তঃস্থ auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  পরিমাণ(পূর্ণাঙ্ক[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
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
#define _DEFINE_INSTANCE_ASSERTION1(সারি_2, _assertion) \
  শূন্য instance_assertion_on_সারি##_line () ধ্রুব \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(সারি_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (সারি_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(সারি_2, _code) \
  শূন্য compiles_assertion_on_সারি##_line () ধ্রুব \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(সারি_2, _code) _DEFINE_COMPILES_ASSERTION1 (সারি_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (পরিমাণ (*এতত্) == (size)) \
  size_t get_size () ধ্রুব { প্রত্যাগম (size); } \
  স্থির constexpr unsigned null_size = (size); \
  স্থির constexpr unsigned min_size = (size); \
  স্থির constexpr unsigned static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((শূন্য) এতত্->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (পরিমাণ(এতত্->u._member) == (size)) \
  স্থির constexpr unsigned null_size = (size); \
  স্থির constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (পরিমাণ (*এতত্) >= (size)) \
  স্থির constexpr unsigned null_size = (size); \
  স্থির constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (পরিমাণ (*এতত্) >= (size)) \
  স্থির constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((শূন্য) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (পরিমাণ (*এতত্) == (size) + (HB_VAR_ARRAY+0) * পরিমাণ ((array)[0])) \
  স্থির constexpr unsigned null_size = (size); \
  স্থির constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  size_t get_size () ধ্রুব { প্রত্যাগম (size - (array).min_size + (array).get_size ()); } \
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

template <typename Data, unsigned পূর্ণাঙ্ক WheresData>
সংরচনা hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () ধ্রুব
  { প্রত্যাগম *(((Data **) (শূন্য *) এতত্) - WheresData); }

  তার্কিক is_inert () ধ্রুব { প্রত্যাগম !get_data (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () ধ্রুব { প্রত্যাগম Subclass::create (get_data ()); }
};
template <>
সংরচনা hb_data_wrapper_t<শূন্য, 0>
{
  তার্কিক is_inert () ধ্রুব { প্রত্যাগম অসত্য; }

  template <typename Stored, typename Funcs>
  Stored * call_create () ধ্রুব { প্রত্যাগম Funcs::create (); }
};

template <typename T1, typename T2> সংরচনা hb_non_void_t { প্রকারনির্দেশ T1 value; };
template <typename T2> সংরচনা hb_non_void_t<শূন্য, T2> { প্রকারনির্দেশ T2 value; };

template <typename Returned,
	  typename Subclass = শূন্য,
	  typename Data = শূন্য,
	  unsigned পূর্ণাঙ্ক WheresData = 0,
	  typename Stored = Returned>
সংরচনা hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  প্রকারনির্দেশ typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = স্বভাব;
  hb_lazy_loader_t (ধ্রুব hb_lazy_loader_t &other) = delete;

  শূন্য init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  শূন্য init ()  { instance.set_relaxed (nullptr); }
  শূন্য fini ()  { do_destroy (instance.get_acquire ()); init (); }

  শূন্য free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    যদি (unlikely (p && !cmpexch (p, nullptr)))
      গচ্ছ retry;
    do_destroy (p);
  }

  স্থির শূন্য do_destroy (Stored *p)
  {
    যদি (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      প্রত্যাগম;
    Funcs::destroy (p);
  }

  ধ্রুব Returned * operator -> () ধ্রুব { প্রত্যাগম get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, শূন্য))>
  ধ্রুব U & operator * () ধ্রুব  { প্রত্যাগম *get (); }
  explicit operator তার্কিক () ধ্রুব
  {
    Stored *p = get_stored ();
    যদি (Funcs::tag_null)
    {
      Stored *raw = এতত্->instance.get_relaxed ();
      প্রত্যাগম raw && !is_tagged_null (raw);
    }
    প্রত্যাগম p != Funcs::get_null ();
  }
  template <typename C> operator ধ্রুব C * () ধ্রুব { প্রত্যাগম get (); }

  Stored * get_stored () ধ্রুব
  {
  retry:
    Stored *p = এতত্->instance.get_acquire ();
    যদি (unlikely (!p))
    {
      যদি (unlikely (এতত্->is_inert ()))
	প্রত্যাগম const_cast<Stored *> (Funcs::get_null ());

      p = এতত্->template call_create<Stored, Funcs> ();
      যদি (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      যদি (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	গচ্ছ retry;
      }
    }
    প্রত্যাগম untagged (p);
  }
  Stored * get_stored_relaxed () ধ্রুব
  {
    প্রত্যাগম untagged (এতত্->instance.get_relaxed ());
  }

  তার্কিক cmpexch (Stored *current, Stored *value) ধ্রুব
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    প্রত্যাগম এতত্->instance.cmpexch (current, value);
  }

  ধ্রুব Returned * get () ধ্রুব { প্রত্যাগম Funcs::convert (get_stored ()); }
  ধ্রুব Returned * get_relaxed () ধ্রুব { প্রত্যাগম Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () ধ্রুব { প্রত্যাগম const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  স্থির Returned* convert (Stored *p) { প্রত্যাগম p; }

  /* By default null/init/fini the object. */
  স্থির ধ্রুব Stored* get_null () { প্রত্যাগম &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  স্থির constexpr তার্কিক tag_null = অসত্য;
  স্থির তার্কিক is_tagged_null (Stored *p)
  { প্রত্যাগম Funcs::tag_null && ((uintptr_t) (শূন্য *) p & 1); }
  স্থির Stored *tagged_null (Stored *p)
  { প্রত্যাগম Funcs::tag_null ? (Stored *) (শূন্য *) ((uintptr_t) (শূন্য *) p | 1) : p; }
  স্থির Stored *untagged (Stored *p)
  { প্রত্যাগম Funcs::tag_null ? (Stored *) (শূন্য *) ((uintptr_t) (শূন্য *) p & ~(uintptr_t) 1) : p; }
  স্থির Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, পরিমাণ (Stored));
    যদি (likely (p))
      p = নূতন (p) Stored (data);
    প্রত্যাগম p;
  }
  স্থির Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, পরিমাণ (Stored));
    যদি (likely (p))
      p = নূতন (p) Stored ();
    প্রত্যাগম p;
  }
  স্থির শূন্য destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  গুপ্ত:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned পূর্ণাঙ্ক WheresFace>
সংরচনা hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  স্থির constexpr তার্কিক tag_null = সত্য;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { প্রত্যাগম এতত্->get ()->get_blob (); }
};

template <typename T, unsigned পূর্ণাঙ্ক WheresFace>
সংরচনা hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  স্থির hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    প্রত্যাগম c.reference_table<T> (face);
  }
  স্থির শূন্য destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  স্থির ধ্রুব hb_blob_t *get_null ()
  { প্রত্যাগম hb_blob_get_empty (); }

  স্থির ধ্রুব T* convert (ধ্রুব hb_blob_t *blob)
  { প্রত্যাগম blob->as<T> (); }

  hb_blob_t* get_blob () ধ্রুব { প্রত্যাগম এতত্->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  সংরচনা hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    স্থির শূন্য destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    স্থির ধ্রুব hb_##Type##_funcs_t *get_null () \
    { প্রত্যাগম hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
