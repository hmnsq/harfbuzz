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

#ضمن "hb.hh"
#ضمن "hb-blob.hh"

#ضمن "hb-dispatch.hh"
#ضمن "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
template<typename Type>
ساكن مضمن ثابت Type& StructAtOffset(ثابت فراغ *P, unsigned صحيح offset)
{ أرجع * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + offset); }
template<typename Type>
ساكن مضمن Type& StructAtOffset(فراغ *P, unsigned صحيح offset)
{ أرجع * reinterpret_cast<Type*> ((حرف *) P + offset); }
template<typename Type>
ساكن مضمن ثابت Type& StructAtOffsetUnaligned(ثابت فراغ *P, unsigned صحيح offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  أرجع * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + offset);
#pragma GCC diagnostic نکالو
}
template<typename Type>
ساكن مضمن Type& StructAtOffsetUnaligned(فراغ *P, unsigned صحيح offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  أرجع * reinterpret_cast<Type*> ((حرف *) P + offset);
#pragma GCC diagnostic نکالو
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
template <typename Type, typename TObject, typename ...Ts>
ساكن مضمن auto StructAfter(ثابت TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
template <typename Type, typename TObject, typename ...Ts>
ساكن مضمن auto StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  حجم(صحيح[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
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
  فراغ instance_assertion_on_سطر##_line () ثابت \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(سطر_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (سطر_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(سطر_2, _code) \
  فراغ compiles_assertion_on_سطر##_line () ثابت \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(سطر_2, _code) _DEFINE_COMPILES_ASSERTION1 (سطر_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (حجم (*هذا) == (size)) \
  size_t get_size () ثابت { أرجع (size); } \
  ساكن constexpr unsigned null_size = (size); \
  ساكن constexpr unsigned min_size = (size); \
  ساكن constexpr unsigned static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((فراغ) هذا->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (حجم(هذا->u._member) == (size)) \
  ساكن constexpr unsigned null_size = (size); \
  ساكن constexpr unsigned min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (حجم (*هذا) >= (size)) \
  ساكن constexpr unsigned null_size = (size); \
  ساكن constexpr unsigned min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (حجم (*هذا) >= (size)) \
  ساكن constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((فراغ) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (حجم (*هذا) == (size) + (HB_VAR_ARRAY+0) * حجم ((array)[0])) \
  ساكن constexpr unsigned null_size = (size); \
  ساكن constexpr unsigned min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  size_t get_size () ثابت { أرجع (size - (array).min_size + (array).get_size ()); } \
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

template <typename Data, unsigned صحيح WheresData>
بنية hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () ثابت
  { أرجع *(((Data **) (فراغ *) هذا) - WheresData); }

  منطقي is_inert () ثابت { أرجع !get_data (); }

  template <typename Stored, typename Subclass>
  Stored * call_create () ثابت { أرجع Subclass::create (get_data ()); }
};
template <>
بنية hb_data_wrapper_t<فراغ, 0>
{
  منطقي is_inert () ثابت { أرجع خطأ; }

  template <typename Stored, typename Funcs>
  Stored * call_create () ثابت { أرجع Funcs::create (); }
};

template <typename T1, typename T2> بنية hb_non_void_t { تعريفنوع T1 value; };
template <typename T2> بنية hb_non_void_t<فراغ, T2> { تعريفنوع T2 value; };

template <typename Returned,
	  typename Subclass = فراغ,
	  typename Data = فراغ,
	  unsigned صحيح WheresData = 0,
	  typename Stored = Returned>
بنية hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  تعريفنوع typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = افتراضي;
  hb_lazy_loader_t (ثابت hb_lazy_loader_t &other) = delete;

  فراغ init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  فراغ init ()  { instance.set_relaxed (nullptr); }
  فراغ fini ()  { do_destroy (instance.get_acquire ()); init (); }

  فراغ free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    إذا (unlikely (p && !cmpexch (p, nullptr)))
      اذهب retry;
    do_destroy (p);
  }

  ساكن فراغ do_destroy (Stored *p)
  {
    إذا (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      أرجع;
    Funcs::destroy (p);
  }

  ثابت Returned * operator -> () ثابت { أرجع get (); }
  template <typename U = Returned, hb_enable_if (!hb_is_same (U, فراغ))>
  ثابت U & operator * () ثابت  { أرجع *get (); }
  explicit operator منطقي () ثابت
  {
    Stored *p = get_stored ();
    إذا (Funcs::tag_null)
    {
      Stored *raw = هذا->instance.get_relaxed ();
      أرجع raw && !is_tagged_null (raw);
    }
    أرجع p != Funcs::get_null ();
  }
  template <typename C> operator ثابت C * () ثابت { أرجع get (); }

  Stored * get_stored () ثابت
  {
  retry:
    Stored *p = هذا->instance.get_acquire ();
    إذا (unlikely (!p))
    {
      إذا (unlikely (هذا->is_inert ()))
	أرجع const_cast<Stored *> (Funcs::get_null ());

      p = هذا->template call_create<Stored, Funcs> ();
      إذا (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      إذا (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	اذهب retry;
      }
    }
    أرجع untagged (p);
  }
  Stored * get_stored_relaxed () ثابت
  {
    أرجع untagged (هذا->instance.get_relaxed ());
  }

  منطقي cmpexch (Stored *current, Stored *value) ثابت
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    أرجع هذا->instance.cmpexch (current, value);
  }

  ثابت Returned * get () ثابت { أرجع Funcs::convert (get_stored ()); }
  ثابت Returned * get_relaxed () ثابت { أرجع Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () ثابت { أرجع const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  ساكن Returned* convert (Stored *p) { أرجع p; }

  /* By default null/init/fini the object. */
  ساكن ثابت Stored* get_null () { أرجع &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  ساكن constexpr منطقي tag_null = خطأ;
  ساكن منطقي is_tagged_null (Stored *p)
  { أرجع Funcs::tag_null && ((uintptr_t) (فراغ *) p & 1); }
  ساكن Stored *tagged_null (Stored *p)
  { أرجع Funcs::tag_null ? (Stored *) (فراغ *) ((uintptr_t) (فراغ *) p | 1) : p; }
  ساكن Stored *untagged (Stored *p)
  { أرجع Funcs::tag_null ? (Stored *) (فراغ *) ((uintptr_t) (فراغ *) p & ~(uintptr_t) 1) : p; }
  ساكن Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, حجم (Stored));
    إذا (likely (p))
      p = جديد (p) Stored (data);
    أرجع p;
  }
  ساكن Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, حجم (Stored));
    إذا (likely (p))
      p = جديد (p) Stored ();
    أرجع p;
  }
  ساكن فراغ destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  خاص:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

template <typename T, unsigned صحيح WheresFace>
بنية hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  ساكن constexpr منطقي tag_null = true;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { أرجع هذا->get ()->get_blob (); }
};

template <typename T, unsigned صحيح WheresFace>
بنية hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  ساكن hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    أرجع c.reference_table<T> (face);
  }
  ساكن فراغ destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  ساكن ثابت hb_blob_t *get_null ()
  { أرجع hb_blob_get_empty (); }

  ساكن ثابت T* convert (ثابت hb_blob_t *blob)
  { أرجع blob->as<T> (); }

  hb_blob_t* get_blob () ثابت { أرجع هذا->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  template <typename Subclass> \
  بنية hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    ساكن فراغ destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    ساكن ثابت hb_##Type##_funcs_t *get_null () \
    { أرجع hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
