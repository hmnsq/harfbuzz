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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MACHINERY_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MACHINERY_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-blob.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-dispatch.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ Type& Struct_At_ਵਿੱਥ(ਅਚਲ ਖਾਲੀ *P, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ)
{ ਵਾਪਸ * reinterpret_cast<ਅਚਲ Type*> ((ਅਚਲ ਅੱਖਰ *) P + ਵਿੱਥ); }
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ Type& Struct_At_ਵਿੱਥ(ਖਾਲੀ *P, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ)
{ ਵਾਪਸ * reinterpret_cast<Type*> ((ਅੱਖਰ *) P + ਵਿੱਥ); }
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ Type& Struct_At_ਵਿੱਥ_Unaligned(ਅਚਲ ਖਾਲੀ *P, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ)
{
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-align"
  ਵਾਪਸ * reinterpret_cast<ਅਚਲ Type*> ((ਅਚਲ ਅੱਖਰ *) P + ਵਿੱਥ);
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
}
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Type>
ਸਥਿਰ ਸਤਰਅੰਦਰ Type& Struct_At_ਵਿੱਥ_Unaligned(ਖਾਲੀ *P, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ)
{
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-align"
  ਵਾਪਸ * reinterpret_cast<Type*> ((ਅੱਖਰ *) P + ਵਿੱਥ);
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ TObject, ਕਿਸਮ_ਨਾਂ ...Ts>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ StructAfter(ਅਚਲ TObject &X, Ts... ਦਲੀਲਾਂ) HB_AUTO_RETURN((
  Struct_At_ਵਿੱਥ<Type>(&X, X.get_ਮਾਪ(std::forward<Ts> (ਦਲੀਲਾਂ)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type, ਕਿਸਮ_ਨਾਂ TObject, ਕਿਸਮ_ਨਾਂ ...Ts>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਸਵੈਚਾਲਿਤ StructAfter(TObject &X, Ts... ਦਲੀਲਾਂ) HB_AUTO_RETURN((
  ਆਕਾਰ(ਪੂਰਨ[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
  Struct_At_ਵਿੱਥ<Type>(&X, X.get_ਮਾਪ(std::forward<Ts> (ਦਲੀਲਾਂ)...))
  : *reinterpret_cast<Type*> (0)
))


/*
 * Size checking
 */

/* Size signifying variable-sized array */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_VAR_ARRAY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_VAR_ARRAY 1
#ਜੇ_ਅੰਤ

/* Check _assertion in a method environment */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _DEFINE_INSTANCE_ASSERTION1(ਸਤਰ_2, _assertion) \
  ਖਾਲੀ instance_assertion_on_ਸਤਰ##_line () ਅਚਲ \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(ਸਤਰ_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (ਸਤਰ_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _DEFINE_COMPILES_ASSERTION1(ਸਤਰ_2, ਕੋਡ_3) \
  ਖਾਲੀ compiles_assertion_on_ਸਤਰ##_line () ਅਚਲ \
  { ਕੋਡ_3; }
# define _DEFINE_COMPILES_ASSERTION0(ਸਤਰ_2, ਕੋਡ_3) _DEFINE_COMPILES_ASSERTION1 (ਸਤਰ_2, ਕੋਡ_3)
# define DEFINE_COMPILES_ASSERTION(ਕੋਡ_3) _DEFINE_COMPILES_ASSERTION0 (__LINE__, ਕੋਡ_3)


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਮਾਪ_STATIC(ਮਾਪ) \
  DEFINE_INSTANCE_ASSERTION (ਆਕਾਰ (*ਇਹ) == (ਮਾਪ)) \
  size_t get_ਮਾਪ () ਅਚਲ { ਵਾਪਸ (ਮਾਪ); } \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ null_ਮਾਪ = (ਮਾਪ); \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = (ਮਾਪ); \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ static_ਮਾਪ = (ਮਾਪ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਮਾਪ_UNION(ਮਾਪ, _member) \
  DEFINE_COMPILES_ASSERTION ((ਖਾਲੀ) ਇਹ->u._member.static_ਮਾਪ) \
  DEFINE_INSTANCE_ASSERTION (ਆਕਾਰ(ਇਹ->u._member) == (ਮਾਪ)) \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ null_ਮਾਪ = (ਮਾਪ); \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = (ਮਾਪ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਮਾਪ_MIN(ਮਾਪ) \
  DEFINE_INSTANCE_ASSERTION (ਆਕਾਰ (*ਇਹ) >= (ਮਾਪ)) \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ null_ਮਾਪ = (ਮਾਪ); \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = (ਮਾਪ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਮਾਪ_UNBOUNDED(ਮਾਪ) \
  DEFINE_INSTANCE_ASSERTION (ਆਕਾਰ (*ਇਹ) >= (ਮਾਪ)) \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = (ਮਾਪ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਮਾਪ_ARRAY(ਮਾਪ, array) \
  DEFINE_COMPILES_ASSERTION ((ਖਾਲੀ) (array)[0].static_ਮਾਪ) \
  DEFINE_INSTANCE_ASSERTION (ਆਕਾਰ (*ਇਹ) == (ਮਾਪ) + (HB_VAR_ARRAY+0) * ਆਕਾਰ ((array)[0])) \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ null_ਮਾਪ = (ਮਾਪ); \
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = (ਮਾਪ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਮਾਪ_ARRAY_SIZED(ਮਾਪ, array) \
  size_t get_ਮਾਪ () ਅਚਲ { ਵਾਪਸ (ਮਾਪ - (array).min_ਮਾਪ + (array).get_ਮਾਪ ()); } \
  DEFINE_ਮਾਪ_ARRAY(ਮਾਪ, array)


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

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਅੰਕੜੇ_3, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ Wheres_ਅੰਕੜੇ>
ਬਣਤਰ hb_ਅੰਕੜੇ_wrapper_t
{
  static_assert (Wheres_ਅੰਕੜੇ > 0, "");

  ਅੰਕੜੇ_3 * get_ਅੰਕੜੇ () ਅਚਲ
  { ਵਾਪਸ *(((ਅੰਕੜੇ_3 **) (ਖਾਲੀ *) ਇਹ) - Wheres_ਅੰਕੜੇ); }

  ਤਾਰਕਿਕ is_inert () ਅਚਲ { ਵਾਪਸ !get_ਅੰਕੜੇ (); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Stored, ਕਿਸਮ_ਨਾਂ Subclass>
  Stored * call_create () ਅਚਲ { ਵਾਪਸ Subclass::create (get_ਅੰਕੜੇ ()); }
};
ਸਾਂਚਾ <>
ਬਣਤਰ hb_ਅੰਕੜੇ_wrapper_t<ਖਾਲੀ, 0>
{
  ਤਾਰਕਿਕ is_inert () ਅਚਲ { ਵਾਪਸ ਝੂਠ; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Stored, ਕਿਸਮ_ਨਾਂ Funcs>
  Stored * call_create () ਅਚਲ { ਵਾਪਸ Funcs::create (); }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T1, ਕਿਸਮ_ਨਾਂ T2> ਬਣਤਰ hb_non_void_t { ਕਿਸਮਪਰਿਭਾਸ਼ਾ T1 value; };
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T2> ਬਣਤਰ hb_non_void_t<ਖਾਲੀ, T2> { ਕਿਸਮਪਰਿਭਾਸ਼ਾ T2 value; };

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Returned,
	  ਕਿਸਮ_ਨਾਂ Subclass = ਖਾਲੀ,
	  ਕਿਸਮ_ਨਾਂ ਅੰਕੜੇ_3 = ਖਾਲੀ,
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ Wheres_ਅੰਕੜੇ = 0,
	  ਕਿਸਮ_ਨਾਂ Stored = Returned>
ਬਣਤਰ hb_lazy_loader_t : hb_ਅੰਕੜੇ_wrapper_t<ਅੰਕੜੇ_3, Wheres_ਅੰਕੜੇ>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,ਅੰਕੜੇ_3,Wheres_ਅੰਕੜੇ,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = ਮੂਲ;
  hb_lazy_loader_t (ਅਚਲ hb_lazy_loader_t &other) = ਮਿਟਾਓ;

  ਖਾਲੀ init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  ਖਾਲੀ init ()  { instance.ਸੈੱਟ_relaxed (ਖਾਲੀ_ਸੂਚਕ); }
  ਖਾਲੀ fini ()  { do_destroy (instance.get_acquire ()); init (); }

  ਖਾਲੀ free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    ਜੇ (unlikely (p && !cmpexch (p, ਖਾਲੀ_ਸੂਚਕ)))
      ਜਾਓ retry;
    do_destroy (p);
  }

  ਸਥਿਰ ਖਾਲੀ do_destroy (Stored *p)
  {
    ਜੇ (Funcs::ਟੈਗ_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      ਵਾਪਸ;
    Funcs::destroy (p);
  }

  ਅਚਲ Returned * ਸੰਚਾਲਕ -> () ਅਚਲ { ਵਾਪਸ get (); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U = Returned, hb_enable_if (!hb_is_same (U, ਖਾਲੀ))>
  ਅਚਲ U & ਸੰਚਾਲਕ * () ਅਚਲ  { ਵਾਪਸ *get (); }
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ
  {
    Stored *p = get_stored ();
    ਜੇ (Funcs::ਟੈਗ_null)
    {
      Stored *raw = ਇਹ->instance.get_relaxed ();
      ਵਾਪਸ raw && !is_tagged_null (raw);
    }
    ਵਾਪਸ p != Funcs::get_null ();
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ C> ਸੰਚਾਲਕ ਅਚਲ C * () ਅਚਲ { ਵਾਪਸ get (); }

  Stored * get_stored () ਅਚਲ
  {
  retry:
    Stored *p = ਇਹ->instance.get_acquire ();
    ਜੇ (unlikely (!p))
    {
      ਜੇ (unlikely (ਇਹ->is_inert ()))
	ਵਾਪਸ const_cast<Stored *> (Funcs::get_null ());

      p = ਇਹ->ਸਾਂਚਾ call_create<Stored, Funcs> ();
      ਜੇ (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      ਜੇ (unlikely (!cmpexch (ਖਾਲੀ_ਸੂਚਕ, p)))
      {
	do_destroy (p);
	ਜਾਓ retry;
      }
    }
    ਵਾਪਸ untagged (p);
  }
  Stored * get_stored_relaxed () ਅਚਲ
  {
    ਵਾਪਸ untagged (ਇਹ->instance.get_relaxed ());
  }

  ਤਾਰਕਿਕ cmpexch (Stored *current, Stored *value) ਅਚਲ
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    ਵਾਪਸ ਇਹ->instance.cmpexch (current, value);
  }

  ਅਚਲ Returned * get () ਅਚਲ { ਵਾਪਸ Funcs::convert (get_stored ()); }
  ਅਚਲ Returned * get_relaxed () ਅਚਲ { ਵਾਪਸ Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () ਅਚਲ { ਵਾਪਸ const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  ਸਥਿਰ Returned* convert (Stored *p) { ਵਾਪਸ p; }

  /* By default null/init/fini the object. */
  ਸਥਿਰ ਅਚਲ Stored* get_null () { ਵਾਪਸ &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ ਟੈਗ_null = ਝੂਠ;
  ਸਥਿਰ ਤਾਰਕਿਕ is_tagged_null (Stored *p)
  { ਵਾਪਸ Funcs::ਟੈਗ_null && ((uintptr_t) (ਖਾਲੀ *) p & 1); }
  ਸਥਿਰ Stored *tagged_null (Stored *p)
  { ਵਾਪਸ Funcs::ਟੈਗ_null ? (Stored *) (ਖਾਲੀ *) ((uintptr_t) (ਖਾਲੀ *) p | 1) : p; }
  ਸਥਿਰ Stored *untagged (Stored *p)
  { ਵਾਪਸ Funcs::ਟੈਗ_null ? (Stored *) (ਖਾਲੀ *) ((uintptr_t) (ਖਾਲੀ *) p & ~(uintptr_t) 1) : p; }
  ਸਥਿਰ Stored *create (ਅੰਕੜੇ_3 *ਅੰਕੜੇ)
  {
    Stored *p = (Stored *) hb_calloc (1, ਆਕਾਰ (Stored));
    ਜੇ (likely (p))
      p = ਨਵਾਂ (p) Stored (ਅੰਕੜੇ);
    ਵਾਪਸ p;
  }
  ਸਥਿਰ Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, ਆਕਾਰ (Stored));
    ਜੇ (likely (p))
      p = ਨਵਾਂ (p) Stored ();
    ਵਾਪਸ p;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  ਨਿਜੀ:
  /* Must only have one pointer. */
  ਬਦਲਣਯੋਗ hb_atomic_t<Stored *> instance;
};

/* Specializations. */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ WheresFace>
ਬਣਤਰ hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ ਟੈਗ_null = ਸੱਚ;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { ਵਾਪਸ ਇਹ->get ()->get_blob (); }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ WheresFace>
ਬਣਤਰ hb_ਸਾਰਣੀ_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_ਸਾਰਣੀ_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  ਸਥਿਰ hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    ਵਾਪਸ c.reference_ਸਾਰਣੀ<T> (face);
  }
  ਸਥਿਰ ਖਾਲੀ destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  ਸਥਿਰ ਅਚਲ hb_blob_t *get_null ()
  { ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ (); }

  ਸਥਿਰ ਅਚਲ T* convert (ਅਚਲ hb_blob_t *blob)
  { ਵਾਪਸ blob->as<T> (); }

  hb_blob_t* get_blob () ਅਚਲ { ਵਾਪਸ ਇਹ->get_stored (); }
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Subclass> \
  ਬਣਤਰ hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    ਸਥਿਰ ਖਾਲੀ destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    ਸਥਿਰ ਅਚਲ hb_##Type##_funcs_t *get_null () \
    { ਵਾਪਸ hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#ਜੇ_ਅੰਤ /* HB_MACHINERY_HH */
