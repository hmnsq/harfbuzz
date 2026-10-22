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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-blob.hh"

#അന്തര്ഭാവ "hb-dispatch.hh"
#അന്തര്ഭാവ "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
template<typename Type>
സ്ഥിതം വരിയിൽ സ്ഥിരം Type& StructAtOffset(സ്ഥിരം ശൂന്യം *P, unsigned പൂർണ്ണസംഖ്യ offset)
{ തിരികെ * reinterpret_cast<സ്ഥിരം Type*> ((സ്ഥിരം അക്ഷരം *) P + offset); }
template<typename Type>
സ്ഥിതം വരിയിൽ Type& StructAtOffset(ശൂന്യം *P, unsigned പൂർണ്ണസംഖ്യ offset)
{ തിരികെ * reinterpret_cast<Type*> ((അക്ഷരം *) P + offset); }
template<typename Type>
സ്ഥിതം വരിയിൽ സ്ഥിരം Type& StructAtOffsetUnaligned(സ്ഥിരം ശൂന്യം *P, unsigned പൂർണ്ണസംഖ്യ offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  തിരികെ * reinterpret_cast<സ്ഥിരം Type*> ((സ്ഥിരം അക്ഷരം *) P + offset);
#pragma GCC diagnostic എടുക്കുക
}
template<typename Type>
സ്ഥിതം വരിയിൽ Type& StructAtOffsetUnaligned(ശൂന്യം *P, unsigned പൂർണ്ണസംഖ്യ offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  തിരികെ * reinterpret_cast<Type*> ((അക്ഷരം *) P + offset);
#pragma GCC diagnostic എടുക്കുക
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
സ്ഥിതം വരിയിൽ auto StructAfter(സ്ഥിരം TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
സ്ഥിതം വരിയിൽ auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  വലുപ്പം(പൂർണ്ണസംഖ്യ[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
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
#define _DEFINE_INSTANCE_ASSERTION1(വരി_2, _assertion) \
  ശൂന്യം instance_assertion_on_വരി##_line () സ്ഥിരം \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(വരി_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (വരി_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(വരി_2, _code) \
  ശൂന്യം compiles_assertion_on_വരി##_line () സ്ഥിരം \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(വരി_2, _code) _DEFINE_COMPILES_ASSERTION1 (വരി_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (വലുപ്പം (*ഇത്) == (size)) \
  size_t get_size () സ്ഥിരം { തിരികെ (size); } \
  സ്ഥിതം constexpr unsigned null_size = (size); \
  സ്ഥിതം constexpr unsigned min_size = (size); \
  സ്ഥിതം constexpr unsigned static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((ശൂന്യം) ഇത്->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (വലുപ്പം(ഇത്->u._member) == (size)) \
  സ്ഥിതം constexpr unsigned null_size = (size); \
  സ്ഥിതം constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (വലുപ്പം (*ഇത്) >= (size)) \
  സ്ഥിതം constexpr unsigned null_size = (size); \
  സ്ഥിതം constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (വലുപ്പം (*ഇത്) >= (size)) \
  സ്ഥിതം constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((ശൂന്യം) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (വലുപ്പം (*ഇത്) == (size) + (HB_VAR_ARRAY+0) * വലുപ്പം ((array)[0])) \
  സ്ഥിതം constexpr unsigned null_size = (size); \
  സ്ഥിതം constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  size_t get_size () സ്ഥിരം { തിരികെ (size - (array).min_size + (array).get_size ()); } \
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

template <typename Data, unsigned പൂർണ്ണസംഖ്യ WheresData>
ഘടന hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () സ്ഥിരം
  { തിരികെ *(((Data **) (ശൂന്യം *) ഇത്) - WheresData); }

  തർക്കം is_inert () സ്ഥിരം { തിരികെ !get_data (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () സ്ഥിരം { തിരികെ Subclass::create (get_data ()); }
};
template <>
ഘടന hb_data_wrapper_t<ശൂന്യം, 0>
{
  തർക്കം is_inert () സ്ഥിരം { തിരികെ അസത്യം; }

  template <typename Stored, typename Funcs>
  Stored * call_create () സ്ഥിരം { തിരികെ Funcs::create (); }
};

template <typename T1, typename T2> ഘടന hb_non_void_t { തരനിർവചനം T1 value; };
template <typename T2> ഘടന hb_non_void_t<ശൂന്യം, T2> { തരനിർവചനം T2 value; };

template <typename Returned,
	  typename Subclass = ശൂന്യം,
	  typename Data = ശൂന്യം,
	  unsigned പൂർണ്ണസംഖ്യ WheresData = 0,
	  typename Stored = Returned>
ഘടന hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  തരനിർവചനം typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = സ്ഥിരസ്ഥിതി;
  hb_lazy_loader_t (സ്ഥിരം hb_lazy_loader_t &other) = delete;

  ശൂന്യം init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  ശൂന്യം init ()  { instance.set_relaxed (nullptr); }
  ശൂന്യം fini ()  { do_destroy (instance.get_acquire ()); init (); }

  ശൂന്യം free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    എങ്കിൽ (unlikely (p && !cmpexch (p, nullptr)))
      പോകുക retry;
    do_destroy (p);
  }

  സ്ഥിതം ശൂന്യം do_destroy (Stored *p)
  {
    എങ്കിൽ (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      തിരികെ;
    Funcs::destroy (p);
  }

  സ്ഥിരം Returned * operator -> () സ്ഥിരം { തിരികെ get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, ശൂന്യം))>
  സ്ഥിരം U & operator * () സ്ഥിരം  { തിരികെ *get (); }
  explicit operator തർക്കം () സ്ഥിരം
  {
    Stored *p = get_stored ();
    എങ്കിൽ (Funcs::tag_null)
    {
      Stored *raw = ഇത്->instance.get_relaxed ();
      തിരികെ raw && !is_tagged_null (raw);
    }
    തിരികെ p != Funcs::get_null ();
  }
  template <typename C> operator സ്ഥിരം C * () സ്ഥിരം { തിരികെ get (); }

  Stored * get_stored () സ്ഥിരം
  {
  retry:
    Stored *p = ഇത്->instance.get_acquire ();
    എങ്കിൽ (unlikely (!p))
    {
      എങ്കിൽ (unlikely (ഇത്->is_inert ()))
	തിരികെ const_cast<Stored *> (Funcs::get_null ());

      p = ഇത്->template call_create<Stored, Funcs> ();
      എങ്കിൽ (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      എങ്കിൽ (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	പോകുക retry;
      }
    }
    തിരികെ untagged (p);
  }
  Stored * get_stored_relaxed () സ്ഥിരം
  {
    തിരികെ untagged (ഇത്->instance.get_relaxed ());
  }

  തർക്കം cmpexch (Stored *current, Stored *value) സ്ഥിരം
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    തിരികെ ഇത്->instance.cmpexch (current, value);
  }

  സ്ഥിരം Returned * get () സ്ഥിരം { തിരികെ Funcs::convert (get_stored ()); }
  സ്ഥിരം Returned * get_relaxed () സ്ഥിരം { തിരികെ Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () സ്ഥിരം { തിരികെ const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  സ്ഥിതം Returned* convert (Stored *p) { തിരികെ p; }

  /* By default null/init/fini the object. */
  സ്ഥിതം സ്ഥിരം Stored* get_null () { തിരികെ &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  സ്ഥിതം constexpr തർക്കം tag_null = അസത്യം;
  സ്ഥിതം തർക്കം is_tagged_null (Stored *p)
  { തിരികെ Funcs::tag_null && ((uintptr_t) (ശൂന്യം *) p & 1); }
  സ്ഥിതം Stored *tagged_null (Stored *p)
  { തിരികെ Funcs::tag_null ? (Stored *) (ശൂന്യം *) ((uintptr_t) (ശൂന്യം *) p | 1) : p; }
  സ്ഥിതം Stored *untagged (Stored *p)
  { തിരികെ Funcs::tag_null ? (Stored *) (ശൂന്യം *) ((uintptr_t) (ശൂന്യം *) p & ~(uintptr_t) 1) : p; }
  സ്ഥിതം Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, വലുപ്പം (Stored));
    എങ്കിൽ (likely (p))
      p = പുതിയ (p) Stored (data);
    തിരികെ p;
  }
  സ്ഥിതം Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, വലുപ്പം (Stored));
    എങ്കിൽ (likely (p))
      p = പുതിയ (p) Stored ();
    തിരികെ p;
  }
  സ്ഥിതം ശൂന്യം destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  സ്വകാര്യ:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned പൂർണ്ണസംഖ്യ WheresFace>
ഘടന hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  സ്ഥിതം constexpr തർക്കം tag_null = സത്യം;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { തിരികെ ഇത്->get ()->get_blob (); }
};

template <typename T, unsigned പൂർണ്ണസംഖ്യ WheresFace>
ഘടന hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  സ്ഥിതം hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    തിരികെ c.reference_table<T> (face);
  }
  സ്ഥിതം ശൂന്യം destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  സ്ഥിതം സ്ഥിരം hb_blob_t *get_null ()
  { തിരികെ hb_blob_get_empty (); }

  സ്ഥിതം സ്ഥിരം T* convert (സ്ഥിരം hb_blob_t *blob)
  { തിരികെ blob->as<T> (); }

  hb_blob_t* get_blob () സ്ഥിരം { തിരികെ ഇത്->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  ഘടന hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    സ്ഥിതം ശൂന്യം destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    സ്ഥിതം സ്ഥിരം hb_##Type##_funcs_t *get_null () \
    { തിരികെ hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
