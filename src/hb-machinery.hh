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

#include "hb.hh"
#include "hb-blob.hh"

#include "hb-dispatch.hh"
#include "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
template<typename Type>
static anta_hstha const Type& StructAtOffset(const void *P, unsigned puur_naa_nka offset)
{ ᨀᨘᨒ * reinterpret_cast<const Type*> ((const var_na *) P + offset); }
template<typename Type>
static anta_hstha Type& StructAtOffset(void *P, unsigned puur_naa_nka offset)
{ ᨀᨘᨒ * reinterpret_cast<Type*> ((var_na *) P + offset); }
template<typename Type>
static anta_hstha const Type& StructAtOffsetUnaligned(const void *P, unsigned puur_naa_nka offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ᨀᨘᨒ * reinterpret_cast<const Type*> ((const var_na *) P + offset);
#pragma GCC diagnostic pop
}
template<typename Type>
static anta_hstha Type& StructAtOffsetUnaligned(void *P, unsigned puur_naa_nka offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ᨀᨘᨒ * reinterpret_cast<Type*> ((var_na *) P + offset);
#pragma GCC diagnostic pop
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
static anta_hstha auto StructAfter(const TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
static anta_hstha auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  parimaa_na(puur_naa_nka[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
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
#define _DEFINE_INSTANCE_ASSERTION1(_line, _assertion) \
  void _instance_assertion_on_line_##_line () const \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(_line, _assertion) _DEFINE_INSTANCE_ASSERTION1 (_line, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(_line, _code) \
  void _compiles_assertion_on_line_##_line () const \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(_line, _code) _DEFINE_COMPILES_ASSERTION1 (_line, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (parimaa_na (*this) == (size)) \
  size_t get_size () const { ᨀᨘᨒ (size); } \
  static constexpr unsigned null_size = (size); \
  static constexpr unsigned min_size = (size); \
  static constexpr unsigned static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((void) this->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (parimaa_na(this->u._member) == (size)) \
  static constexpr unsigned null_size = (size); \
  static constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (parimaa_na (*this) >= (size)) \
  static constexpr unsigned null_size = (size); \
  static constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (parimaa_na (*this) >= (size)) \
  static constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((void) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (parimaa_na (*this) == (size) + (HB_VAR_ARRAY+0) * parimaa_na ((array)[0])) \
  static constexpr unsigned null_size = (size); \
  static constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  size_t get_size () const { ᨀᨘᨒ (size - (array).min_size + (array).get_size ()); } \
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

template <typename Data, unsigned puur_naa_nka WheresData>
sa_nracanaa hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () const
  { ᨀᨘᨒ *(((Data **) (void *) this) - WheresData); }

  bool is_inert () const { ᨀᨘᨒ !get_data (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () const { ᨀᨘᨒ Subclass::create (get_data ()); }
};
template <>
sa_nracanaa hb_data_wrapper_t<void, 0>
{
  bool is_inert () const { ᨀᨘᨒ false; }

  template <typename Stored, typename Funcs>
  Stored * call_create () const { ᨀᨘᨒ Funcs::create (); }
};

template <typename T1, typename T2> sa_nracanaa hb_non_void_t { typedef T1 value; };
template <typename T2> sa_nracanaa hb_non_void_t<void, T2> { typedef T2 value; };

template <typename Returned,
	  typename Subclass = void,
	  typename Data = void,
	  unsigned puur_naa_nka WheresData = 0,
	  typename Stored = Returned>
sa_nracanaa hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  typedef typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = default;
  hb_lazy_loader_t (const hb_lazy_loader_t &other) = delete;

  void init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  void init ()  { instance.set_relaxed (nullptr); }
  void fini ()  { do_destroy (instance.get_acquire ()); init (); }

  void free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    ᨀᨘ (unlikely (p && !cmpexch (p, nullptr)))
      goto retry;
    do_destroy (p);
  }

  static void do_destroy (Stored *p)
  {
    ᨀᨘ (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      ᨀᨘᨒ;
    Funcs::destroy (p);
  }

  const Returned * operator -> () const { ᨀᨘᨒ get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, void))>
  const U & operator * () const  { ᨀᨘᨒ *get (); }
  explicit operator bool () const
  {
    Stored *p = get_stored ();
    ᨀᨘ (Funcs::tag_null)
    {
      Stored *raw = this->instance.get_relaxed ();
      ᨀᨘᨒ raw && !is_tagged_null (raw);
    }
    ᨀᨘᨒ p != Funcs::get_null ();
  }
  template <typename C> operator const C * () const { ᨀᨘᨒ get (); }

  Stored * get_stored () const
  {
  retry:
    Stored *p = this->instance.get_acquire ();
    ᨀᨘ (unlikely (!p))
    {
      ᨀᨘ (unlikely (this->is_inert ()))
	ᨀᨘᨒ const_cast<Stored *> (Funcs::get_null ());

      p = this->template call_create<Stored, Funcs> ();
      ᨀᨘ (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      ᨀᨘ (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	goto retry;
      }
    }
    ᨀᨘᨒ untagged (p);
  }
  Stored * get_stored_relaxed () const
  {
    ᨀᨘᨒ untagged (this->instance.get_relaxed ());
  }

  bool cmpexch (Stored *current, Stored *value) const
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    ᨀᨘᨒ this->instance.cmpexch (current, value);
  }

  const Returned * get () const { ᨀᨘᨒ Funcs::convert (get_stored ()); }
  const Returned * get_relaxed () const { ᨀᨘᨒ Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () const { ᨀᨘᨒ const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  static Returned* convert (Stored *p) { ᨀᨘᨒ p; }

  /* By default null/init/fini the object. */
  static const Stored* get_null () { ᨀᨘᨒ &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  static constexpr bool tag_null = false;
  static bool is_tagged_null (Stored *p)
  { ᨀᨘᨒ Funcs::tag_null && ((uintptr_t) (void *) p & 1); }
  static Stored *tagged_null (Stored *p)
  { ᨀᨘᨒ Funcs::tag_null ? (Stored *) (void *) ((uintptr_t) (void *) p | 1) : p; }
  static Stored *untagged (Stored *p)
  { ᨀᨘᨒ Funcs::tag_null ? (Stored *) (void *) ((uintptr_t) (void *) p & ~(uintptr_t) 1) : p; }
  static Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, parimaa_na (Stored));
    ᨀᨘ (likely (p))
      p = new (p) Stored (data);
    ᨀᨘᨒ p;
  }
  static Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, parimaa_na (Stored));
    ᨀᨘ (likely (p))
      p = new (p) Stored ();
    ᨀᨘᨒ p;
  }
  static void destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  private:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned puur_naa_nka WheresFace>
sa_nracanaa hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  static constexpr bool tag_null = true;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { ᨀᨘᨒ this->get ()->get_blob (); }
};

template <typename T, unsigned puur_naa_nka WheresFace>
sa_nracanaa hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  static hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    ᨀᨘᨒ c.reference_table<T> (face);
  }
  static void destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  static const hb_blob_t *get_null ()
  { ᨀᨘᨒ hb_blob_get_empty (); }

  static const T* convert (const hb_blob_t *blob)
  { ᨀᨘᨒ blob->as<T> (); }

  hb_blob_t* get_blob () const { ᨀᨘᨒ this->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  sa_nracanaa hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    static void destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    static const hb_##Type##_funcs_t *get_null () \
    { ᨀᨘᨒ hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
