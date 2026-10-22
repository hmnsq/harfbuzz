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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-blob.hh"

#अन्तर्भाव "hb-dispatch.hh"
#अन्तर्भाव "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
ढाँचा<typename Type>
स्थिर अन्तःस्थ ध्रुव Type& StructAtOffset(ध्रुव शून्य *P, अचिन्हित पूर्णाङ्क offset)
{ प्रत्यागम * reinterpret_cast<ध्रुव Type*> ((ध्रुव वर्ण *) P + offset); }
ढाँचा<typename Type>
स्थिर अन्तःस्थ Type& StructAtOffset(शून्य *P, अचिन्हित पूर्णाङ्क offset)
{ प्रत्यागम * reinterpret_cast<Type*> ((वर्ण *) P + offset); }
ढाँचा<typename Type>
स्थिर अन्तःस्थ ध्रुव Type& StructAtOffsetUnaligned(ध्रुव शून्य *P, अचिन्हित पूर्णाङ्क offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  प्रत्यागम * reinterpret_cast<ध्रुव Type*> ((ध्रुव वर्ण *) P + offset);
#pragma GCC diagnostic निकालो
}
ढाँचा<typename Type>
स्थिर अन्तःस्थ Type& StructAtOffsetUnaligned(शून्य *P, अचिन्हित पूर्णाङ्क offset)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  प्रत्यागम * reinterpret_cast<Type*> ((वर्ण *) P + offset);
#pragma GCC diagnostic निकालो
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
ढाँचा <typename Type, typename TObject, typename ...Ts>
स्थिर अन्तःस्थ स्वतः StructAfter(ध्रुव TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_size(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
ढाँचा <typename Type, typename TObject, typename ...Ts>
स्थिर अन्तःस्थ स्वतः StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  परिमाण(पूर्णाङ्क[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
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
#define _DEFINE_INSTANCE_ASSERTION1(पंक्ति_2, _assertion) \
  शून्य instance_assertion_on_पंक्ति##_line () ध्रुव \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(पंक्ति_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (पंक्ति_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#define _DEFINE_COMPILES_ASSERTION1(पंक्ति_2, _code) \
  शून्य compiles_assertion_on_पंक्ति##_line () ध्रुव \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(पंक्ति_2, _code) _DEFINE_COMPILES_ASSERTION1 (पंक्ति_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#define DEFINE_SIZE_STATIC(size) \
  DEFINE_INSTANCE_ASSERTION (परिमाण (*एतत्) == (size)) \
  माप_प्रकार get_size () ध्रुव { प्रत्यागम (size); } \
  स्थिर constexpr अचिन्हित null_size = (size); \
  स्थिर constexpr अचिन्हित min_size = (size); \
  स्थिर constexpr अचिन्हित static_size = (size)

#define DEFINE_SIZE_UNION(size, _member) \
  DEFINE_COMPILES_ASSERTION ((शून्य) एतत्->u._member.static_size) \
  DEFINE_INSTANCE_ASSERTION (परिमाण(एतत्->u._member) == (size)) \
  स्थिर constexpr अचिन्हित null_size = (size); \
  स्थिर constexpr अचिन्हित min_size = (size)

#define DEFINE_SIZE_MIN(size) \
  DEFINE_INSTANCE_ASSERTION (परिमाण (*एतत्) >= (size)) \
  स्थिर constexpr अचिन्हित null_size = (size); \
  स्थिर constexpr अचिन्हित min_size = (size)

#define DEFINE_SIZE_UNBOUNDED(size) \
  DEFINE_INSTANCE_ASSERTION (परिमाण (*एतत्) >= (size)) \
  स्थिर constexpr अचिन्हित min_size = (size)

#define DEFINE_SIZE_ARRAY(size, array) \
  DEFINE_COMPILES_ASSERTION ((शून्य) (array)[0].static_size) \
  DEFINE_INSTANCE_ASSERTION (परिमाण (*एतत्) == (size) + (HB_VAR_ARRAY+0) * परिमाण ((array)[0])) \
  स्थिर constexpr अचिन्हित null_size = (size); \
  स्थिर constexpr अचिन्हित min_size = (size)

#define DEFINE_SIZE_ARRAY_SIZED(size, array) \
  माप_प्रकार get_size () ध्रुव { प्रत्यागम (size - (array).min_size + (array).get_size ()); } \
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

ढाँचा <typename Data, अचिन्हित पूर्णाङ्क WheresData>
संरचना hb_data_wrapper_t
{
  static_assert (WheresData > 0, "");

  Data * get_data () ध्रुव
  { प्रत्यागम *(((Data **) (शून्य *) एतत्) - WheresData); }

  तार्किक is_inert () ध्रुव { प्रत्यागम !get_data (); }

  ढाँचा <typename Stored, typename Subclass>
  Stored * call_create () ध्रुव { प्रत्यागम Subclass::create (get_data ()); }
};
ढाँचा <>
संरचना hb_data_wrapper_t<शून्य, 0>
{
  तार्किक is_inert () ध्रुव { प्रत्यागम असत्य; }

  ढाँचा <typename Stored, typename Funcs>
  Stored * call_create () ध्रुव { प्रत्यागम Funcs::create (); }
};

ढाँचा <typename T1, typename T2> संरचना hb_non_void_t { प्रकारनिर्देश T1 value; };
ढाँचा <typename T2> संरचना hb_non_void_t<शून्य, T2> { प्रकारनिर्देश T2 value; };

ढाँचा <typename Returned,
	  typename Subclass = शून्य,
	  typename Data = शून्य,
	  अचिन्हित पूर्णाङ्क WheresData = 0,
	  typename Stored = Returned>
संरचना hb_lazy_loader_t : hb_data_wrapper_t<Data, WheresData>
{
  प्रकारनिर्देश typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,Data,WheresData,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = स्वभाव;
  hb_lazy_loader_t (ध्रुव hb_lazy_loader_t &other) = delete;

  शून्य init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  शून्य init ()  { instance.set_relaxed (nullptr); }
  शून्य fini ()  { do_destroy (instance.get_acquire ()); init (); }

  शून्य free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    यदि (unlikely (p && !cmpexch (p, nullptr)))
      गच्छ retry;
    do_destroy (p);
  }

  स्थिर शून्य do_destroy (Stored *p)
  {
    यदि (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      प्रत्यागम;
    Funcs::destroy (p);
  }

  ध्रुव Returned * चालक -> () ध्रुव { प्रत्यागम get (); }
  ढाँचा <typename U = Returned, hb_enable_if (!hb_is_same (U, शून्य))>
  ध्रुव U & चालक * () ध्रुव  { प्रत्यागम *get (); }
  explicit चालक तार्किक () ध्रुव
  {
    Stored *p = get_stored ();
    यदि (Funcs::tag_null)
    {
      Stored *raw = एतत्->instance.get_relaxed ();
      प्रत्यागम raw && !is_tagged_null (raw);
    }
    प्रत्यागम p != Funcs::get_null ();
  }
  ढाँचा <typename C> चालक ध्रुव C * () ध्रुव { प्रत्यागम get (); }

  Stored * get_stored () ध्रुव
  {
  retry:
    Stored *p = एतत्->instance.get_acquire ();
    यदि (unlikely (!p))
    {
      यदि (unlikely (एतत्->is_inert ()))
	प्रत्यागम const_cast<Stored *> (Funcs::get_null ());

      p = एतत्->ढाँचा call_create<Stored, Funcs> ();
      यदि (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      यदि (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	गच्छ retry;
      }
    }
    प्रत्यागम untagged (p);
  }
  Stored * get_stored_relaxed () ध्रुव
  {
    प्रत्यागम untagged (एतत्->instance.get_relaxed ());
  }

  तार्किक cmpexch (Stored *current, Stored *value) ध्रुव
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    प्रत्यागम एतत्->instance.cmpexch (current, value);
  }

  ध्रुव Returned * get () ध्रुव { प्रत्यागम Funcs::convert (get_stored ()); }
  ध्रुव Returned * get_relaxed () ध्रुव { प्रत्यागम Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () ध्रुव { प्रत्यागम const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  स्थिर Returned* convert (Stored *p) { प्रत्यागम p; }

  /* By default null/init/fini the object. */
  स्थिर ध्रुव Stored* get_null () { प्रत्यागम &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  स्थिर constexpr तार्किक tag_null = असत्य;
  स्थिर तार्किक is_tagged_null (Stored *p)
  { प्रत्यागम Funcs::tag_null && ((uintptr_t) (शून्य *) p & 1); }
  स्थिर Stored *tagged_null (Stored *p)
  { प्रत्यागम Funcs::tag_null ? (Stored *) (शून्य *) ((uintptr_t) (शून्य *) p | 1) : p; }
  स्थिर Stored *untagged (Stored *p)
  { प्रत्यागम Funcs::tag_null ? (Stored *) (शून्य *) ((uintptr_t) (शून्य *) p & ~(uintptr_t) 1) : p; }
  स्थिर Stored *create (Data *data)
  {
    Stored *p = (Stored *) hb_calloc (1, परिमाण (Stored));
    यदि (likely (p))
      p = नूतन (p) Stored (data);
    प्रत्यागम p;
  }
  स्थिर Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, परिमाण (Stored));
    यदि (likely (p))
      p = नूतन (p) Stored ();
    प्रत्यागम p;
  }
  स्थिर शून्य destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  गुप्त:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

ढाँचा <typename T, अचिन्हित पूर्णाङ्क WheresFace>
संरचना hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  स्थिर constexpr तार्किक tag_null = सत्य;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { प्रत्यागम एतत्->get ()->get_blob (); }
};

ढाँचा <typename T, अचिन्हित पूर्णाङ्क WheresFace>
संरचना hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  स्थिर hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    प्रत्यागम c.reference_table<T> (face);
  }
  स्थिर शून्य destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  स्थिर ध्रुव hb_blob_t *get_null ()
  { प्रत्यागम hb_blob_get_empty (); }

  स्थिर ध्रुव T* convert (ध्रुव hb_blob_t *blob)
  { प्रत्यागम blob->as<T> (); }

  hb_blob_t* get_blob () ध्रुव { प्रत्यागम एतत्->get_stored (); }
};

#define HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  ढाँचा <typename Subclass> \
  संरचना hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    स्थिर शून्य destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    स्थिर ध्रुव hb_##Type##_funcs_t *get_null () \
    { प्रत्यागम hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#undef HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#endif /* HB_MACHINERY_HH */
