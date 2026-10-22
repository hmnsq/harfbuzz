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

#شامل "hb.hh"
#شامل "hb-blob.hh"

#شامل "hb-dispatch.hh"
#شامل "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
template<typename Type>
ایستا درونخطی ثابت Type& StructAtOffset(ثابت تهی *P, unsigned صحیح offset)
{ بازگشت * reinterpret_cast<ثابت Type*> ((ثابت نویسه *) P + offset); }
template<typename Type>
ایستا درونخطی Type& StructAtOffset(تهی *P, unsigned صحیح offset)
{ بازگشت * reinterpret_cast<Type*> ((نویسه *) P + offset); }
template<typename Type>
ایستا درونخطی ثابت Type& StructAtOffsetUnaligned(ثابت تهی *P, unsigned صحیح offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  بازگشت * reinterpret_cast<ثابت Type*> ((ثابت نویسه *) P + offset);
#pragma GCC diagnostic برداشت
}
template<typename Type>
ایستا درونخطی Type& StructAtOffsetUnaligned(تهی *P, unsigned صحیح offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  بازگشت * reinterpret_cast<Type*> ((نویسه *) P + offset);
#pragma GCC diagnostic برداشت
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
ایستا درونخطی auto StructAfter(ثابت TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
ایستا درونخطی auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  اندازه(صحیح[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
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
#define _DEFINE_INSTANCE_ASSERTION1(سطر_2, _assertion) \
  تهی instance_assertion_on_سطر##_line () ثابت \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(سطر_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (سطر_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(سطر_2, _code) \
  تهی compiles_assertion_on_سطر##_line () ثابت \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(سطر_2, _code) _DEFINE_COMPILES_ASSERTION1 (سطر_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (اندازه (*این) == (size)) \
  size_t get_size () ثابت { بازگشت (size); } \
  ایستا constexpr unsigned null_size = (size); \
  ایستا constexpr unsigned min_size = (size); \
  ایستا constexpr unsigned static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((تهی) این->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (اندازه(این->u._member) == (size)) \
  ایستا constexpr unsigned null_size = (size); \
  ایستا constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (اندازه (*این) >= (size)) \
  ایستا constexpr unsigned null_size = (size); \
  ایستا constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (اندازه (*این) >= (size)) \
  ایستا constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((تهی) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (اندازه (*این) == (size) + (HB_VAR_ARRAY+0) * اندازه ((array)[0])) \
  ایستا constexpr unsigned null_size = (size); \
  ایستا constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  size_t get_size () ثابت { بازگشت (size - (array).min_size + (array).get_size ()); } \
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

template <typename Data, unsigned صحیح WheresData>
ساختار hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () ثابت
  { بازگشت *(((Data **) (تهی *) این) - WheresData); }

  منطقی is_inert () ثابت { بازگشت !get_data (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () ثابت { بازگشت Subclass::create (get_data ()); }
};
template <>
ساختار hb_data_wrapper_t<تهی, 0>
{
  منطقی is_inert () ثابت { بازگشت نادرست; }

  template <typename Stored, typename Funcs>
  Stored * call_create () ثابت { بازگشت Funcs::create (); }
};

template <typename T1, typename T2> ساختار hb_non_void_t { تعریفنوع T1 value; };
template <typename T2> ساختار hb_non_void_t<تهی, T2> { تعریفنوع T2 value; };

template <typename Returned,
	  typename Subclass = تهی,
	  typename Data = تهی,
	  unsigned صحیح WheresData = 0,
	  typename Stored = Returned>
ساختار hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  تعریفنوع typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = پیشفرض;
  hb_lazy_loader_t (ثابت hb_lazy_loader_t &other) = delete;

  تهی init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  تهی init ()  { instance.set_relaxed (nullptr); }
  تهی fini ()  { do_destroy (instance.get_acquire ()); init (); }

  تهی free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    اگر (unlikely (p && !cmpexch (p, nullptr)))
      برو retry;
    do_destroy (p);
  }

  ایستا تهی do_destroy (Stored *p)
  {
    اگر (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      بازگشت;
    Funcs::destroy (p);
  }

  ثابت Returned * operator -> () ثابت { بازگشت get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, تهی))>
  ثابت U & operator * () ثابت  { بازگشت *get (); }
  explicit operator منطقی () ثابت
  {
    Stored *p = get_stored ();
    اگر (Funcs::tag_null)
    {
      Stored *raw = این->instance.get_relaxed ();
      بازگشت raw && !is_tagged_null (raw);
    }
    بازگشت p != Funcs::get_null ();
  }
  template <typename C> operator ثابت C * () ثابت { بازگشت get (); }

  Stored * get_stored () ثابت
  {
  retry:
    Stored *p = این->instance.get_acquire ();
    اگر (unlikely (!p))
    {
      اگر (unlikely (این->is_inert ()))
	بازگشت const_cast<Stored *> (Funcs::get_null ());

      p = این->template call_create<Stored, Funcs> ();
      اگر (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      اگر (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	برو retry;
      }
    }
    بازگشت untagged (p);
  }
  Stored * get_stored_relaxed () ثابت
  {
    بازگشت untagged (این->instance.get_relaxed ());
  }

  منطقی cmpexch (Stored *current, Stored *value) ثابت
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    بازگشت این->instance.cmpexch (current, value);
  }

  ثابت Returned * get () ثابت { بازگشت Funcs::convert (get_stored ()); }
  ثابت Returned * get_relaxed () ثابت { بازگشت Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () ثابت { بازگشت const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  ایستا Returned* convert (Stored *p) { بازگشت p; }

  /* By default null/init/fini the object. */
  ایستا ثابت Stored* get_null () { بازگشت &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  ایستا constexpr منطقی tag_null = نادرست;
  ایستا منطقی is_tagged_null (Stored *p)
  { بازگشت Funcs::tag_null && ((uintptr_t) (تهی *) p & 1); }
  ایستا Stored *tagged_null (Stored *p)
  { بازگشت Funcs::tag_null ? (Stored *) (تهی *) ((uintptr_t) (تهی *) p | 1) : p; }
  ایستا Stored *untagged (Stored *p)
  { بازگشت Funcs::tag_null ? (Stored *) (تهی *) ((uintptr_t) (تهی *) p & ~(uintptr_t) 1) : p; }
  ایستا Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, اندازه (Stored));
    اگر (likely (p))
      p = جدید (p) Stored (data);
    بازگشت p;
  }
  ایستا Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, اندازه (Stored));
    اگر (likely (p))
      p = جدید (p) Stored ();
    بازگشت p;
  }
  ایستا تهی destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  خصوصی:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned صحیح WheresFace>
ساختار hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  ایستا constexpr منطقی tag_null = درست;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { بازگشت این->get ()->get_blob (); }
};

template <typename T, unsigned صحیح WheresFace>
ساختار hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  ایستا hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    بازگشت c.reference_table<T> (face);
  }
  ایستا تهی destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  ایستا ثابت hb_blob_t *get_null ()
  { بازگشت hb_blob_get_empty (); }

  ایستا ثابت T* convert (ثابت hb_blob_t *blob)
  { بازگشت blob->as<T> (); }

  hb_blob_t* get_blob () ثابت { بازگشت این->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  ساختار hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    ایستا تهی destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    ایستا ثابت hb_##Type##_funcs_t *get_null () \
    { بازگشت hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
