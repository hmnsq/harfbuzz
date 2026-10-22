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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-blob.hh"

#અન્તર્ભાવ "hb-dispatch.hh"
#અન્તર્ભાવ "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
template<typename Type>
સ્થિર અંતઃરેખ અચલ Type& StructAtOffset(અચલ શૂન્ય *P, unsigned પૂર્ણાંક offset)
{ પાછા * reinterpret_cast<અચલ Type*> ((અચલ અક્ષર *) P + offset); }
template<typename Type>
સ્થિર અંતઃરેખ Type& StructAtOffset(શૂન્ય *P, unsigned પૂર્ણાંક offset)
{ પાછા * reinterpret_cast<Type*> ((અક્ષર *) P + offset); }
template<typename Type>
સ્થિર અંતઃરેખ અચલ Type& StructAtOffsetUnaligned(અચલ શૂન્ય *P, unsigned પૂર્ણાંક offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  પાછા * reinterpret_cast<અચલ Type*> ((અચલ અક્ષર *) P + offset);
#pragma GCC diagnostic કાઢો
}
template<typename Type>
સ્થિર અંતઃરેખ Type& StructAtOffsetUnaligned(શૂન્ય *P, unsigned પૂર્ણાંક offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  પાછા * reinterpret_cast<Type*> ((અક્ષર *) P + offset);
#pragma GCC diagnostic કાઢો
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
સ્થિર અંતઃરેખ auto StructAfter(અચલ TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
સ્થિર અંતઃરેખ auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  કદ(પૂર્ણાંક[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
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
#define _DEFINE_INSTANCE_ASSERTION1(લીટી_2, _assertion) \
  શૂન્ય instance_assertion_on_લીટી##_line () અચલ \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(લીટી_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (લીટી_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(લીટી_2, _code) \
  શૂન્ય compiles_assertion_on_લીટી##_line () અચલ \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(લીટી_2, _code) _DEFINE_COMPILES_ASSERTION1 (લીટી_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (કદ (*આ) == (size)) \
  size_t get_size () અચલ { પાછા (size); } \
  સ્થિર constexpr unsigned null_size = (size); \
  સ્થિર constexpr unsigned min_size = (size); \
  સ્થિર constexpr unsigned static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((શૂન્ય) આ->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (કદ(આ->u._member) == (size)) \
  સ્થિર constexpr unsigned null_size = (size); \
  સ્થિર constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (કદ (*આ) >= (size)) \
  સ્થિર constexpr unsigned null_size = (size); \
  સ્થિર constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (કદ (*આ) >= (size)) \
  સ્થિર constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((શૂન્ય) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (કદ (*આ) == (size) + (HB_VAR_ARRAY+0) * કદ ((array)[0])) \
  સ્થિર constexpr unsigned null_size = (size); \
  સ્થિર constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  size_t get_size () અચલ { પાછા (size - (array).min_size + (array).get_size ()); } \
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

template <typename Data, unsigned પૂર્ણાંક WheresData>
સંરચના hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () અચલ
  { પાછા *(((Data **) (શૂન્ય *) આ) - WheresData); }

  તાર્કિક is_inert () અચલ { પાછા !get_data (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () અચલ { પાછા Subclass::create (get_data ()); }
};
template <>
સંરચના hb_data_wrapper_t<શૂન્ય, 0>
{
  તાર્કિક is_inert () અચલ { પાછા ખોટું; }

  template <typename Stored, typename Funcs>
  Stored * call_create () અચલ { પાછા Funcs::create (); }
};

template <typename T1, typename T2> સંરચના hb_non_void_t { પ્રકારવ્યાખ્યા T1 value; };
template <typename T2> સંરચના hb_non_void_t<શૂન્ય, T2> { પ્રકારવ્યાખ્યા T2 value; };

template <typename Returned,
	  typename Subclass = શૂન્ય,
	  typename Data = શૂન્ય,
	  unsigned પૂર્ણાંક WheresData = 0,
	  typename Stored = Returned>
સંરચના hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  પ્રકારવ્યાખ્યા typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = મૂળભૂત;
  hb_lazy_loader_t (અચલ hb_lazy_loader_t &other) = delete;

  શૂન્ય init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  શૂન્ય init ()  { instance.set_relaxed (nullptr); }
  શૂન્ય fini ()  { do_destroy (instance.get_acquire ()); init (); }

  શૂન્ય free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    જો (unlikely (p && !cmpexch (p, nullptr)))
      જાઓ retry;
    do_destroy (p);
  }

  સ્થિર શૂન્ય do_destroy (Stored *p)
  {
    જો (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      પાછા;
    Funcs::destroy (p);
  }

  અચલ Returned * operator -> () અચલ { પાછા get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, શૂન્ય))>
  અચલ U & operator * () અચલ  { પાછા *get (); }
  explicit operator તાર્કિક () અચલ
  {
    Stored *p = get_stored ();
    જો (Funcs::tag_null)
    {
      Stored *raw = આ->instance.get_relaxed ();
      પાછા raw && !is_tagged_null (raw);
    }
    પાછા p != Funcs::get_null ();
  }
  template <typename C> operator અચલ C * () અચલ { પાછા get (); }

  Stored * get_stored () અચલ
  {
  retry:
    Stored *p = આ->instance.get_acquire ();
    જો (unlikely (!p))
    {
      જો (unlikely (આ->is_inert ()))
	પાછા const_cast<Stored *> (Funcs::get_null ());

      p = આ->template call_create<Stored, Funcs> ();
      જો (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      જો (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	જાઓ retry;
      }
    }
    પાછા untagged (p);
  }
  Stored * get_stored_relaxed () અચલ
  {
    પાછા untagged (આ->instance.get_relaxed ());
  }

  તાર્કિક cmpexch (Stored *current, Stored *value) અચલ
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    પાછા આ->instance.cmpexch (current, value);
  }

  અચલ Returned * get () અચલ { પાછા Funcs::convert (get_stored ()); }
  અચલ Returned * get_relaxed () અચલ { પાછા Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () અચલ { પાછા const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  સ્થિર Returned* convert (Stored *p) { પાછા p; }

  /* By default null/init/fini the object. */
  સ્થિર અચલ Stored* get_null () { પાછા &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  સ્થિર constexpr તાર્કિક tag_null = ખોટું;
  સ્થિર તાર્કિક is_tagged_null (Stored *p)
  { પાછા Funcs::tag_null && ((uintptr_t) (શૂન્ય *) p & 1); }
  સ્થિર Stored *tagged_null (Stored *p)
  { પાછા Funcs::tag_null ? (Stored *) (શૂન્ય *) ((uintptr_t) (શૂન્ય *) p | 1) : p; }
  સ્થિર Stored *untagged (Stored *p)
  { પાછા Funcs::tag_null ? (Stored *) (શૂન્ય *) ((uintptr_t) (શૂન્ય *) p & ~(uintptr_t) 1) : p; }
  સ્થિર Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, કદ (Stored));
    જો (likely (p))
      p = નવું (p) Stored (data);
    પાછા p;
  }
  સ્થિર Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, કદ (Stored));
    જો (likely (p))
      p = નવું (p) Stored ();
    પાછા p;
  }
  સ્થિર શૂન્ય destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  ખાનગી:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned પૂર્ણાંક WheresFace>
સંરચના hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  સ્થિર constexpr તાર્કિક tag_null = સાચું;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { પાછા આ->get ()->get_blob (); }
};

template <typename T, unsigned પૂર્ણાંક WheresFace>
સંરચના hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  સ્થિર hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    પાછા c.reference_table<T> (face);
  }
  સ્થિર શૂન્ય destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  સ્થિર અચલ hb_blob_t *get_null ()
  { પાછા hb_blob_get_empty (); }

  સ્થિર અચલ T* convert (અચલ hb_blob_t *blob)
  { પાછા blob->as<T> (); }

  hb_blob_t* get_blob () અચલ { પાછા આ->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  સંરચના hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    સ્થિર શૂન્ય destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    સ્થિર અચલ hb_##Type##_funcs_t *get_null () \
    { પાછા hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
