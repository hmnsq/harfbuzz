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

#अगर_अघोषित HB_MACHINERY_HH
#घोषणा HB_MACHINERY_HH

#समावेश "hb.hh"
#समावेश "hb-blob.hh"

#समावेश "hb-dispatch.hh"
#समावेश "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
ढाँचा<typename Type>
अटल अंतरभूत स्थिर Type& StructAtOffset(स्थिर व्योम *P, अचिन्हित पूर्णांक offset)
{ वापस * reinterpret_cast<स्थिर Type*> ((स्थिर अक्षर *) P + offset); }
ढाँचा<typename Type>
अटल अंतरभूत Type& StructAtOffset(व्योम *P, अचिन्हित पूर्णांक offset)
{ वापस * reinterpret_cast<Type*> ((अक्षर *) P + offset); }
ढाँचा<typename Type>
अटल अंतरभूत स्थिर Type& StructAtOffsetUnaligned(स्थिर व्योम *P, अचिन्हित पूर्णांक offset)
{
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-align"
  वापस * reinterpret_cast<स्थिर Type*> ((स्थिर अक्षर *) P + offset);
#आशय GCC diagnostic निकालो
}
ढाँचा<typename Type>
अटल अंतरभूत Type& StructAtOffsetUnaligned(व्योम *P, अचिन्हित पूर्णांक offset)
{
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-align"
  वापस * reinterpret_cast<Type*> ((अक्षर *) P + offset);
#आशय GCC diagnostic निकालो
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
ढाँचा <typename Type, typename TObject, typename ...Ts>
अटल अंतरभूत स्वतः StructAfter(स्थिर TObject &X, Ts... args) HB_AUTO_RETURN((
  StructAtOffset<Type>(&X, X.get_आकार(std::forward<Ts> (args)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
ढाँचा <typename Type, typename TObject, typename ...Ts>
अटल अंतरभूत स्वतः StructAfter(TObject &X, Ts... args) HB_AUTO_RETURN((
  माप(पूर्णांक[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
  StructAtOffset<Type>(&X, X.get_आकार(std::forward<Ts> (args)...))
  : *reinterpret_cast<Type*> (0)
))


/*
 * Size checking
 */

/* Size signifying variable-sized array */
#अगर_अघोषित HB_VAR_ARRAY
#घोषणा HB_VAR_ARRAY 1
#पूर्ण_अगर

/* Check _assertion in a method environment */
#घोषणा _DEFINE_INSTANCE_ASSERTION1(पंक्ति_2, _assertion) \
  व्योम instance_assertion_on_पंक्ति##_line () स्थिर \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(पंक्ति_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (पंक्ति_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#घोषणा _DEFINE_COMPILES_ASSERTION1(पंक्ति_2, _code) \
  व्योम compiles_assertion_on_पंक्ति##_line () स्थिर \
  { _code; }
# define _DEFINE_COMPILES_ASSERTION0(पंक्ति_2, _code) _DEFINE_COMPILES_ASSERTION1 (पंक्ति_2, _code)
# define DEFINE_COMPILES_ASSERTION(_code) _DEFINE_COMPILES_ASSERTION0 (__LINE__, _code)


#घोषणा DEFINE_आकार_STATIC(आकार) \
  DEFINE_INSTANCE_ASSERTION (माप (*यह) == (आकार)) \
  माप_प्रकार get_आकार () स्थिर { वापस (आकार); } \
  अटल constexpr अचिन्हित null_आकार = (आकार); \
  अटल constexpr अचिन्हित min_आकार = (आकार); \
  अटल constexpr अचिन्हित static_आकार = (आकार)

#घोषणा DEFINE_आकार_UNION(आकार, _member) \
  DEFINE_COMPILES_ASSERTION ((व्योम) यह->u._member.static_आकार) \
  DEFINE_INSTANCE_ASSERTION (माप(यह->u._member) == (आकार)) \
  अटल constexpr अचिन्हित null_आकार = (आकार); \
  अटल constexpr अचिन्हित min_आकार = (आकार)

#घोषणा DEFINE_आकार_MIN(आकार) \
  DEFINE_INSTANCE_ASSERTION (माप (*यह) >= (आकार)) \
  अटल constexpr अचिन्हित null_आकार = (आकार); \
  अटल constexpr अचिन्हित min_आकार = (आकार)

#घोषणा DEFINE_आकार_UNBOUNDED(आकार) \
  DEFINE_INSTANCE_ASSERTION (माप (*यह) >= (आकार)) \
  अटल constexpr अचिन्हित min_आकार = (आकार)

#घोषणा DEFINE_आकार_ARRAY(आकार, array) \
  DEFINE_COMPILES_ASSERTION ((व्योम) (array)[0].static_आकार) \
  DEFINE_INSTANCE_ASSERTION (माप (*यह) == (आकार) + (HB_VAR_ARRAY+0) * माप ((array)[0])) \
  अटल constexpr अचिन्हित null_आकार = (आकार); \
  अटल constexpr अचिन्हित min_आकार = (आकार)

#घोषणा DEFINE_आकार_ARRAY_SIZED(आकार, array) \
  माप_प्रकार get_आकार () स्थिर { वापस (आकार - (array).min_आकार + (array).get_आकार ()); } \
  DEFINE_आकार_ARRAY(आकार, array)


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

ढाँचा <typename तथ्य_3, अचिन्हित पूर्णांक Wheres_तथ्य>
काष्ठा hb_तथ्य_wrapper_t
{
  static_assert (Wheres_तथ्य > 0, "");

  तथ्य_3 * get_तथ्य () स्थिर
  { वापस *(((तथ्य_3 **) (व्योम *) यह) - Wheres_तथ्य); }

  bool is_inert () स्थिर { वापस !get_तथ्य (); }

  ढाँचा <typename Stored, typename Subclass>
  Stored * call_create () स्थिर { वापस Subclass::create (get_तथ्य ()); }
};
ढाँचा <>
काष्ठा hb_तथ्य_wrapper_t<व्योम, 0>
{
  bool is_inert () स्थिर { वापस false; }

  ढाँचा <typename Stored, typename Funcs>
  Stored * call_create () स्थिर { वापस Funcs::create (); }
};

ढाँचा <typename T1, typename T2> काष्ठा hb_non_void_t { प्रकार T1 value; };
ढाँचा <typename T2> काष्ठा hb_non_void_t<व्योम, T2> { प्रकार T2 value; };

ढाँचा <typename Returned,
	  typename Subclass = व्योम,
	  typename तथ्य_3 = व्योम,
	  अचिन्हित पूर्णांक Wheres_तथ्य = 0,
	  typename Stored = Returned>
काष्ठा hb_lazy_loader_t : hb_तथ्य_wrapper_t<तथ्य_3, Wheres_तथ्य>
{
  प्रकार typename hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,तथ्य_3,Wheres_तथ्य,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = शेष;
  hb_lazy_loader_t (स्थिर hb_lazy_loader_t &other) = मिटाओ;

  व्योम init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  व्योम init ()  { instance.set_relaxed (nullptr); }
  व्योम fini ()  { do_destroy (instance.get_acquire ()); init (); }

  व्योम free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    अगर (unlikely (p && !cmpexch (p, nullptr)))
      जाओ retry;
    do_destroy (p);
  }

  अटल व्योम do_destroy (Stored *p)
  {
    अगर (Funcs::tag_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      वापस;
    Funcs::destroy (p);
  }

  स्थिर Returned * चालक -> () स्थिर { वापस लो (); }
  ढाँचा <typename U = Returned, hb_enable_if (!hb_is_same (U, व्योम))>
  स्थिर U & चालक * () स्थिर  { वापस *लो (); }
  explicit चालक bool () स्थिर
  {
    Stored *p = get_stored ();
    अगर (Funcs::tag_null)
    {
      Stored *raw = यह->instance.get_relaxed ();
      वापस raw && !is_tagged_null (raw);
    }
    वापस p != Funcs::get_null ();
  }
  ढाँचा <typename C> चालक स्थिर C * () स्थिर { वापस लो (); }

  Stored * get_stored () स्थिर
  {
  retry:
    Stored *p = यह->instance.get_acquire ();
    अगर (unlikely (!p))
    {
      अगर (unlikely (यह->is_inert ()))
	वापस const_cast<Stored *> (Funcs::get_null ());

      p = यह->ढाँचा call_create<Stored, Funcs> ();
      अगर (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      अगर (unlikely (!cmpexch (nullptr, p)))
      {
	do_destroy (p);
	जाओ retry;
      }
    }
    वापस untagged (p);
  }
  Stored * get_stored_relaxed () स्थिर
  {
    वापस untagged (यह->instance.get_relaxed ());
  }

  bool cmpexch (Stored *current, Stored *value) स्थिर
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    वापस यह->instance.cmpexch (current, value);
  }

  स्थिर Returned * लो () स्थिर { वापस Funcs::convert (get_stored ()); }
  स्थिर Returned * get_relaxed () स्थिर { वापस Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () स्थिर { वापस const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  अटल Returned* convert (Stored *p) { वापस p; }

  /* By default null/init/fini the object. */
  अटल स्थिर Stored* get_null () { वापस &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  अटल constexpr bool tag_null = false;
  अटल bool is_tagged_null (Stored *p)
  { वापस Funcs::tag_null && ((uintptr_t) (व्योम *) p & 1); }
  अटल Stored *tagged_null (Stored *p)
  { वापस Funcs::tag_null ? (Stored *) (व्योम *) ((uintptr_t) (व्योम *) p | 1) : p; }
  अटल Stored *untagged (Stored *p)
  { वापस Funcs::tag_null ? (Stored *) (व्योम *) ((uintptr_t) (व्योम *) p & ~(uintptr_t) 1) : p; }
  अटल Stored *create (तथ्य_3 *तथ्य)
  {
    Stored *p = (Stored *) hb_calloc (1, माप (Stored));
    अगर (likely (p))
      p = नया (p) Stored (तथ्य);
    वापस p;
  }
  अटल Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, माप (Stored));
    अगर (likely (p))
      p = नया (p) Stored ();
    वापस p;
  }
  अटल व्योम destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  निजी:
  /* Must only have one pointer. */
  mutable hb_atomic_t<Stored *> instance;
};

/* Specializations. */

ढाँचा <typename T, अचिन्हित पूर्णांक WheresFace>
काष्ठा hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  अटल constexpr bool tag_null = true;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { वापस यह->लो ()->get_blob (); }
};

ढाँचा <typename T, अचिन्हित पूर्णांक WheresFace>
काष्ठा hb_table_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_table_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  अटल hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    वापस c.reference_table<T> (face);
  }
  अटल व्योम destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  अटल स्थिर hb_blob_t *get_null ()
  { वापस hb_blob_get_empty (); }

  अटल स्थिर T* convert (स्थिर hb_blob_t *blob)
  { वापस blob->as<T> (); }

  hb_blob_t* get_blob () स्थिर { वापस यह->get_stored (); }
};

#घोषणा HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  ढाँचा <typename Subclass> \
  काष्ठा hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    अटल व्योम destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    अटल स्थिर hb_##Type##_funcs_t *get_null () \
    { वापस hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#अघोषित HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#पूर्ण_अगर /* HB_MACHINERY_HH */
