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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-blob.hh"

#அந்தர்பாவ "hb-dispatch.hh"
#அந்தர்பாவ "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
template<typename Type>
நிலையான உள்வரி மாறிலி Type& StructAtOffset(மாறிலி வெறுமை *P, unsigned முழு offset)
{ திரும்பு * reinterpret_cast<மாறிலி Type*> ((மாறிலி எழுத்து *) P + offset); }
template<typename Type>
நிலையான உள்வரி Type& StructAtOffset(வெறுமை *P, unsigned முழு offset)
{ திரும்பு * reinterpret_cast<Type*> ((எழுத்து *) P + offset); }
template<typename Type>
நிலையான உள்வரி மாறிலி Type& StructAtOffsetUnaligned(மாறிலி வெறுமை *P, unsigned முழு offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  திரும்பு * reinterpret_cast<மாறிலி Type*> ((மாறிலி எழுத்து *) P + offset);
#pragma GCC diagnostic எடு
}
template<typename Type>
நிலையான உள்வரி Type& StructAtOffsetUnaligned(வெறுமை *P, unsigned முழு offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  திரும்பு * reinterpret_cast<Type*> ((எழுத்து *) P + offset);
#pragma GCC diagnostic எடு
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
நிலையான உள்வரி auto StructAfter(மாறிலி TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
நிலையான உள்வரி auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  அளவு(முழு[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
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
#define _DEFINE_INSTANCE_ASSERTION1(வரி_2, _assertion) \
  வெறுமை instance_assertion_on_வரி##_line () மாறிலி \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(வரி_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (வரி_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(வரி_2, _code) \
  வெறுமை compiles_assertion_on_வரி##_line () மாறிலி \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(வரி_2, _code) _DEFINE_COMPILES_ASSERTION1 (வரி_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (அளவு (*இது) == (size)) \
  size_t get_size () மாறிலி { திரும்பு (size); } \
  நிலையான constexpr unsigned null_size = (size); \
  நிலையான constexpr unsigned min_size = (size); \
  நிலையான constexpr unsigned static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((வெறுமை) இது->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (அளவு(இது->u._member) == (size)) \
  நிலையான constexpr unsigned null_size = (size); \
  நிலையான constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (அளவு (*இது) >= (size)) \
  நிலையான constexpr unsigned null_size = (size); \
  நிலையான constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (அளவு (*இது) >= (size)) \
  நிலையான constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((வெறுமை) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (அளவு (*இது) == (size) + (HB_VAR_ARRAY+0) * அளவு ((array)[0])) \
  நிலையான constexpr unsigned null_size = (size); \
  நிலையான constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  size_t get_size () மாறிலி { திரும்பு (size - (array).min_size + (array).get_size ()); } \
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

template <typename Data, unsigned முழு WheresData>
கட்டமைப்பு hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () மாறிலி
  { திரும்பு *(((Data **) (வெறுமை *) இது) - WheresData); }

  தருக்க is_inert () மாறிலி { திரும்பு !get_data (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () மாறிலி { திரும்பு Subclass::create (get_data ()); }
};
template <>
கட்டமைப்பு hb_data_wrapper_t<வெறுமை, 0>
{
  தருக்க is_inert () மாறிலி { திரும்பு பொய்; }

  template <typename Stored, typename Funcs>
  Stored * call_create () மாறிலி { திரும்பு Funcs::create (); }
};

template <typename T1, typename T2> கட்டமைப்பு hb_non_void_t { வகைவரை T1 value; };
template <typename T2> கட்டமைப்பு hb_non_void_t<வெறுமை, T2> { வகைவரை T2 value; };

template <typename Returned,
	  typename Subclass = வெறுமை,
	  typename Data = வெறுமை,
	  unsigned முழு WheresData = 0,
	  typename Stored = Returned>
கட்டமைப்பு hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  வகைவரை typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = இயல்பு;
  hb_lazy_loader_t (மாறிலி hb_lazy_loader_t &other) = delete;

  வெறுமை init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  வெறுமை init ()  { instance.set_relaxed (nullptr); }
  வெறுமை fini ()  { do_destroy (instance.get_acquire ()); init (); }

  வெறுமை free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    எனில் (unlikely (p && !cmpexch (p, nullptr)))
      செல் retry;
    do_destroy (p);
  }

  நிலையான வெறுமை do_destroy (Stored *p)
  {
    எனில் (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      திரும்பு;
    Funcs::destroy (p);
  }

  மாறிலி Returned * operator -> () மாறிலி { திரும்பு get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, வெறுமை))>
  மாறிலி U & operator * () மாறிலி  { திரும்பு *get (); }
  explicit operator தருக்க () மாறிலி
  {
    Stored *p = get_stored ();
    எனில் (Funcs::tag_null)
    {
      Stored *raw = இது->instance.get_relaxed ();
      திரும்பு raw && !is_tagged_null (raw);
    }
    திரும்பு p != Funcs::get_null ();
  }
  template <typename C> operator மாறிலி C * () மாறிலி { திரும்பு get (); }

  Stored * get_stored () மாறிலி
  {
  retry:
    Stored *p = இது->instance.get_acquire ();
    எனில் (unlikely (!p))
    {
      எனில் (unlikely (இது->is_inert ()))
	திரும்பு const_cast<Stored *> (Funcs::get_null ());

      p = இது->template call_create<Stored, Funcs> ();
      எனில் (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      எனில் (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	செல் retry;
      }
    }
    திரும்பு untagged (p);
  }
  Stored * get_stored_relaxed () மாறிலி
  {
    திரும்பு untagged (இது->instance.get_relaxed ());
  }

  தருக்க cmpexch (Stored *current, Stored *value) மாறிலி
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    திரும்பு இது->instance.cmpexch (current, value);
  }

  மாறிலி Returned * get () மாறிலி { திரும்பு Funcs::convert (get_stored ()); }
  மாறிலி Returned * get_relaxed () மாறிலி { திரும்பு Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () மாறிலி { திரும்பு const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  நிலையான Returned* convert (Stored *p) { திரும்பு p; }

  /* By default null/init/fini the object. */
  நிலையான மாறிலி Stored* get_null () { திரும்பு &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  நிலையான constexpr தருக்க tag_null = பொய்;
  நிலையான தருக்க is_tagged_null (Stored *p)
  { திரும்பு Funcs::tag_null && ((uintptr_t) (வெறுமை *) p & 1); }
  நிலையான Stored *tagged_null (Stored *p)
  { திரும்பு Funcs::tag_null ? (Stored *) (வெறுமை *) ((uintptr_t) (வெறுமை *) p | 1) : p; }
  நிலையான Stored *untagged (Stored *p)
  { திரும்பு Funcs::tag_null ? (Stored *) (வெறுமை *) ((uintptr_t) (வெறுமை *) p & ~(uintptr_t) 1) : p; }
  நிலையான Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, அளவு (Stored));
    எனில் (likely (p))
      p = புதிய (p) Stored (data);
    திரும்பு p;
  }
  நிலையான Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, அளவு (Stored));
    எனில் (likely (p))
      p = புதிய (p) Stored ();
    திரும்பு p;
  }
  நிலையான வெறுமை destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  தனிப்பட்ட:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned முழு WheresFace>
கட்டமைப்பு hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  நிலையான constexpr தருக்க tag_null = உண்மை;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { திரும்பு இது->get ()->get_blob (); }
};

template <typename T, unsigned முழு WheresFace>
கட்டமைப்பு hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  நிலையான hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    திரும்பு c.reference_table<T> (face);
  }
  நிலையான வெறுமை destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  நிலையான மாறிலி hb_blob_t *get_null ()
  { திரும்பு hb_blob_get_empty (); }

  நிலையான மாறிலி T* convert (மாறிலி hb_blob_t *blob)
  { திரும்பு blob->as<T> (); }

  hb_blob_t* get_blob () மாறிலி { திரும்பு இது->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  கட்டமைப்பு hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    நிலையான வெறுமை destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    நிலையான மாறிலி hb_##Type##_funcs_t *get_null () \
    { திரும்பு hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
