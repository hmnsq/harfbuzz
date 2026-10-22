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

#جے_بنا_تعریف HB_MACHINERY_HH
#تعریف_کرو HB_MACHINERY_HH

#ضمن "hb.hh"
#ضمن "hb-blob.hh"

#ضمن "hb-dispatch.hh"
#ضمن "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
سانچا<قسم_ناں Type>
ساكن مضمن ثابت Type& Struct_At_وتھ(ثابت فراغ *P, بنانشانی صحيح وتھ)
{ أرجع * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + وتھ); }
سانچا<قسم_ناں Type>
ساكن مضمن Type& Struct_At_وتھ(فراغ *P, بنانشانی صحيح وتھ)
{ أرجع * reinterpret_cast<Type*> ((حرف *) P + وتھ); }
سانچا<قسم_ناں Type>
ساكن مضمن ثابت Type& Struct_At_وتھ_Unaligned(ثابت فراغ *P, بنانشانی صحيح وتھ)
{
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
  أرجع * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + وتھ);
#ہدایت GCC diagnostic نکالو
}
سانچا<قسم_ناں Type>
ساكن مضمن Type& Struct_At_وتھ_Unaligned(فراغ *P, بنانشانی صحيح وتھ)
{
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
  أرجع * reinterpret_cast<Type*> ((حرف *) P + وتھ);
#ہدایت GCC diagnostic نکالو
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
سانچا <قسم_ناں Type, قسم_ناں TObject, قسم_ناں ...Ts>
ساكن مضمن خودکار StructAfter(ثابت TObject &X, Ts... دلیلاں) HB_AUTO_RETURN((
  Struct_At_وتھ<Type>(&X, X.get_ناپ(std::forward<Ts> (دلیلاں)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
سانچا <قسم_ناں Type, قسم_ناں TObject, قسم_ناں ...Ts>
ساكن مضمن خودکار StructAfter(TObject &X, Ts... دلیلاں) HB_AUTO_RETURN((
  حجم(صحيح[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
  Struct_At_وتھ<Type>(&X, X.get_ناپ(std::forward<Ts> (دلیلاں)...))
  : *reinterpret_cast<Type*> (0)
))


/*
 * Size checking
 */

/* Size signifying variable-sized array */
#جے_بنا_تعریف HB_VAR_ARRAY
#تعریف_کرو HB_VAR_ARRAY 1
#جے_انت

/* Check _assertion in a method environment */
#تعریف_کرو _DEFINE_INSTANCE_ASSERTION1(سطر_2, _assertion) \
  فراغ instance_assertion_on_سطر##_line () ثابت \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(سطر_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (سطر_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#تعریف_کرو _DEFINE_COMPILES_ASSERTION1(سطر_2, کوڈ_3) \
  فراغ compiles_assertion_on_سطر##_line () ثابت \
  { کوڈ_3; }
# define _DEFINE_COMPILES_ASSERTION0(سطر_2, کوڈ_3) _DEFINE_COMPILES_ASSERTION1 (سطر_2, کوڈ_3)
# define DEFINE_COMPILES_ASSERTION(کوڈ_3) _DEFINE_COMPILES_ASSERTION0 (__LINE__, کوڈ_3)


#تعریف_کرو DEFINE_ناپ_STATIC(ناپ) \
  DEFINE_INSTANCE_ASSERTION (حجم (*هذا) == (ناپ)) \
  size_t get_ناپ () ثابت { أرجع (ناپ); } \
  ساكن constexpr بنانشانی null_ناپ = (ناپ); \
  ساكن constexpr بنانشانی min_ناپ = (ناپ); \
  ساكن constexpr بنانشانی static_ناپ = (ناپ)

#تعریف_کرو DEFINE_ناپ_UNION(ناپ, _member) \
  DEFINE_COMPILES_ASSERTION ((فراغ) هذا->u._member.static_ناپ) \
  DEFINE_INSTANCE_ASSERTION (حجم(هذا->u._member) == (ناپ)) \
  ساكن constexpr بنانشانی null_ناپ = (ناپ); \
  ساكن constexpr بنانشانی min_ناپ = (ناپ)

#تعریف_کرو DEFINE_ناپ_MIN(ناپ) \
  DEFINE_INSTANCE_ASSERTION (حجم (*هذا) >= (ناپ)) \
  ساكن constexpr بنانشانی null_ناپ = (ناپ); \
  ساكن constexpr بنانشانی min_ناپ = (ناپ)

#تعریف_کرو DEFINE_ناپ_UNBOUNDED(ناپ) \
  DEFINE_INSTANCE_ASSERTION (حجم (*هذا) >= (ناپ)) \
  ساكن constexpr بنانشانی min_ناپ = (ناپ)

#تعریف_کرو DEFINE_ناپ_ARRAY(ناپ, array) \
  DEFINE_COMPILES_ASSERTION ((فراغ) (array)[0].static_ناپ) \
  DEFINE_INSTANCE_ASSERTION (حجم (*هذا) == (ناپ) + (HB_VAR_ARRAY+0) * حجم ((array)[0])) \
  ساكن constexpr بنانشانی null_ناپ = (ناپ); \
  ساكن constexpr بنانشانی min_ناپ = (ناپ)

#تعریف_کرو DEFINE_ناپ_ARRAY_SIZED(ناپ, array) \
  size_t get_ناپ () ثابت { أرجع (ناپ - (array).min_ناپ + (array).get_ناپ ()); } \
  DEFINE_ناپ_ARRAY(ناپ, array)


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

سانچا <قسم_ناں انکڑے_3, بنانشانی صحيح Wheres_انکڑے>
بنية hb_انکڑے_wrapper_t
{
  static_assert (Wheres_انکڑے > 0, "");

  انکڑے_3 * get_انکڑے () ثابت
  { أرجع *(((انکڑے_3 **) (فراغ *) هذا) - Wheres_انکڑے); }

  منطقي is_inert () ثابت { أرجع !get_انکڑے (); }

  سانچا <قسم_ناں Stored, قسم_ناں Subclass>
  Stored * call_create () ثابت { أرجع Subclass::create (get_انکڑے ()); }
};
سانچا <>
بنية hb_انکڑے_wrapper_t<فراغ, 0>
{
  منطقي is_inert () ثابت { أرجع خطأ; }

  سانچا <قسم_ناں Stored, قسم_ناں Funcs>
  Stored * call_create () ثابت { أرجع Funcs::create (); }
};

سانچا <قسم_ناں T1, قسم_ناں T2> بنية hb_non_void_t { تعريفنوع T1 value; };
سانچا <قسم_ناں T2> بنية hb_non_void_t<فراغ, T2> { تعريفنوع T2 value; };

سانچا <قسم_ناں Returned,
	  قسم_ناں Subclass = فراغ,
	  قسم_ناں انکڑے_3 = فراغ,
	  بنانشانی صحيح Wheres_انکڑے = 0,
	  قسم_ناں Stored = Returned>
بنية hb_lazy_loader_t : hb_انکڑے_wrapper_t<انکڑے_3, Wheres_انکڑے>
{
  تعريفنوع قسم_ناں hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,انکڑے_3,Wheres_انکڑے,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = افتراضي;
  hb_lazy_loader_t (ثابت hb_lazy_loader_t &other) = مٹاؤ;

  فراغ init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  فراغ init ()  { instance.سیٹ_relaxed (خالی_سوچک); }
  فراغ fini ()  { do_destroy (instance.get_acquire ()); init (); }

  فراغ free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    إذا (unlikely (p && !cmpexch (p, خالی_سوچک)))
      اذهب retry;
    do_destroy (p);
  }

  ساكن فراغ do_destroy (Stored *p)
  {
    إذا (Funcs::ٹیگ_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      أرجع;
    Funcs::destroy (p);
  }

  ثابت Returned * عامل -> () ثابت { أرجع get (); }
  سانچا <قسم_ناں U = Returned, hb_enable_if (!hb_is_same (U, فراغ))>
  ثابت U & عامل * () ثابت  { أرجع *get (); }
  واضح عامل منطقي () ثابت
  {
    Stored *p = get_stored ();
    إذا (Funcs::ٹیگ_null)
    {
      Stored *raw = هذا->instance.get_relaxed ();
      أرجع raw && !is_tagged_null (raw);
    }
    أرجع p != Funcs::get_null ();
  }
  سانچا <قسم_ناں C> عامل ثابت C * () ثابت { أرجع get (); }

  Stored * get_stored () ثابت
  {
  retry:
    Stored *p = هذا->instance.get_acquire ();
    إذا (unlikely (!p))
    {
      إذا (unlikely (هذا->is_inert ()))
	أرجع const_cast<Stored *> (Funcs::get_null ());

      p = هذا->سانچا call_create<Stored, Funcs> ();
      إذا (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      إذا (unlikely (!cmpexch (خالی_سوچک, p)))
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
  ساكن constexpr منطقي ٹیگ_null = خطأ;
  ساكن منطقي is_tagged_null (Stored *p)
  { أرجع Funcs::ٹیگ_null && ((uintptr_t) (فراغ *) p & 1); }
  ساكن Stored *tagged_null (Stored *p)
  { أرجع Funcs::ٹیگ_null ? (Stored *) (فراغ *) ((uintptr_t) (فراغ *) p | 1) : p; }
  ساكن Stored *untagged (Stored *p)
  { أرجع Funcs::ٹیگ_null ? (Stored *) (فراغ *) ((uintptr_t) (فراغ *) p & ~(uintptr_t) 1) : p; }
  ساكن Stored *create (انکڑے_3 *انکڑے)
  {
    Stored *p = (Stored *) hb_calloc (1, حجم (Stored));
    إذا (likely (p))
      p = جديد (p) Stored (انکڑے);
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
  بدلنجوگ hb_atomic_t<Stored *> instance;
};

/* Specializations. */

سانچا <قسم_ناں T, بنانشانی صحيح WheresFace>
بنية hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  ساكن constexpr منطقي ٹیگ_null = سچ;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { أرجع هذا->get ()->get_blob (); }
};

سانچا <قسم_ناں T, بنانشانی صحيح WheresFace>
بنية hb_جدول_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_جدول_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  ساكن hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    أرجع c.reference_جدول<T> (face);
  }
  ساكن فراغ destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  ساكن ثابت hb_blob_t *get_null ()
  { أرجع hb_blob_get_خالی_تھاواں (); }

  ساكن ثابت T* convert (ثابت hb_blob_t *blob)
  { أرجع blob->as<T> (); }

  hb_blob_t* get_blob () ثابت { أرجع هذا->get_stored (); }
};

#تعریف_کرو HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  سانچا <قسم_ناں Subclass> \
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

#تعریف_رد HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#جے_انت /* HB_MACHINERY_HH */
