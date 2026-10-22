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

#اگر_غیر_تعریف_شدہ HB_MACHINERY_HH
#تعریف_کرو HB_MACHINERY_HH

#شامل "hb.hh"
#شامل "hb-blob.hh"

#شامل "hb-dispatch.hh"
#شامل "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
سانچہ<قسم_نام Type>
ساکن اندرونی ثابت Type& Struct_At_انحراف(ثابت خالی *P, غیرعلامتی صحیح انحراف)
{ واپس * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + انحراف); }
سانچہ<قسم_نام Type>
ساکن اندرونی Type& Struct_At_انحراف(خالی *P, غیرعلامتی صحیح انحراف)
{ واپس * reinterpret_cast<Type*> ((حرف *) P + انحراف); }
سانچہ<قسم_نام Type>
ساکن اندرونی ثابت Type& Struct_At_انحراف_Unaligned(ثابت خالی *P, غیرعلامتی صحیح انحراف)
{
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
  واپس * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + انحراف);
#ہدایت GCC diagnostic نکالو
}
سانچہ<قسم_نام Type>
ساکن اندرونی Type& Struct_At_انحراف_Unaligned(خالی *P, غیرعلامتی صحیح انحراف)
{
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
  واپس * reinterpret_cast<Type*> ((حرف *) P + انحراف);
#ہدایت GCC diagnostic نکالو
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
سانچہ <قسم_نام Type, قسم_نام TObject, قسم_نام ...Ts>
ساکن اندرونی خودکار StructAfter(ثابت TObject &X, Ts... دلائل) HB_AUTO_RETURN((
  Struct_At_انحراف<Type>(&X, X.get_سائز(std::forward<Ts> (دلائل)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
سانچہ <قسم_نام Type, قسم_نام TObject, قسم_نام ...Ts>
ساکن اندرونی خودکار StructAfter(TObject &X, Ts... دلائل) HB_AUTO_RETURN((
  حجم(صحیح[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
  Struct_At_انحراف<Type>(&X, X.get_سائز(std::forward<Ts> (دلائل)...))
  : *reinterpret_cast<Type*> (0)
))


/*
 * Size checking
 */

/* Size signifying variable-sized array */
#اگر_غیر_تعریف_شدہ HB_VAR_ARRAY
#تعریف_کرو HB_VAR_ARRAY 1
#اگر_ختم

/* Check _assertion in a method environment */
#تعریف_کرو _DEFINE_INSTANCE_ASSERTION1(سطر_2, _assertion) \
  خالی instance_assertion_on_سطر##_line () ثابت \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(سطر_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (سطر_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#تعریف_کرو _DEFINE_COMPILES_ASSERTION1(سطر_2, رمز_3) \
  خالی compiles_assertion_on_سطر##_line () ثابت \
  { رمز_3; }
# define _DEFINE_COMPILES_ASSERTION0(سطر_2, رمز_3) _DEFINE_COMPILES_ASSERTION1 (سطر_2, رمز_3)
# define DEFINE_COMPILES_ASSERTION(رمز_3) _DEFINE_COMPILES_ASSERTION0 (__LINE__, رمز_3)


#تعریف_کرو DEFINE_سائز_STATIC(سائز) \
  DEFINE_INSTANCE_ASSERTION (حجم (*یہ) == (سائز)) \
  size_t get_سائز () ثابت { واپس (سائز); } \
  ساکن constexpr غیرعلامتی null_سائز = (سائز); \
  ساکن constexpr غیرعلامتی min_سائز = (سائز); \
  ساکن constexpr غیرعلامتی static_سائز = (سائز)

#تعریف_کرو DEFINE_سائز_UNION(سائز, _member) \
  DEFINE_COMPILES_ASSERTION ((خالی) یہ->u._member.static_سائز) \
  DEFINE_INSTANCE_ASSERTION (حجم(یہ->u._member) == (سائز)) \
  ساکن constexpr غیرعلامتی null_سائز = (سائز); \
  ساکن constexpr غیرعلامتی min_سائز = (سائز)

#تعریف_کرو DEFINE_سائز_MIN(سائز) \
  DEFINE_INSTANCE_ASSERTION (حجم (*یہ) >= (سائز)) \
  ساکن constexpr غیرعلامتی null_سائز = (سائز); \
  ساکن constexpr غیرعلامتی min_سائز = (سائز)

#تعریف_کرو DEFINE_سائز_UNBOUNDED(سائز) \
  DEFINE_INSTANCE_ASSERTION (حجم (*یہ) >= (سائز)) \
  ساکن constexpr غیرعلامتی min_سائز = (سائز)

#تعریف_کرو DEFINE_سائز_ARRAY(سائز, array) \
  DEFINE_COMPILES_ASSERTION ((خالی) (array)[0].static_سائز) \
  DEFINE_INSTANCE_ASSERTION (حجم (*یہ) == (سائز) + (HB_VAR_ARRAY+0) * حجم ((array)[0])) \
  ساکن constexpr غیرعلامتی null_سائز = (سائز); \
  ساکن constexpr غیرعلامتی min_سائز = (سائز)

#تعریف_کرو DEFINE_سائز_ARRAY_SIZED(سائز, array) \
  size_t get_سائز () ثابت { واپس (سائز - (array).min_سائز + (array).get_سائز ()); } \
  DEFINE_سائز_ARRAY(سائز, array)


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

سانچہ <قسم_نام ڈیٹا_3, غیرعلامتی صحیح Wheres_ڈیٹا>
ساخت hb_ڈیٹا_wrapper_t
{
  static_assert (Wheres_ڈیٹا > 0, "");

  ڈیٹا_3 * get_ڈیٹا () ثابت
  { واپس *(((ڈیٹا_3 **) (خالی *) یہ) - Wheres_ڈیٹا); }

  منطقی is_inert () ثابت { واپس !get_ڈیٹا (); }

  سانچہ <قسم_نام Stored, قسم_نام Subclass>
  Stored * call_create () ثابت { واپس Subclass::create (get_ڈیٹا ()); }
};
سانچہ <>
ساخت hb_ڈیٹا_wrapper_t<خالی, 0>
{
  منطقی is_inert () ثابت { واپس جھوٹ; }

  سانچہ <قسم_نام Stored, قسم_نام Funcs>
  Stored * call_create () ثابت { واپس Funcs::create (); }
};

سانچہ <قسم_نام T1, قسم_نام T2> ساخت hb_non_void_t { قسمتعریف T1 value; };
سانچہ <قسم_نام T2> ساخت hb_non_void_t<خالی, T2> { قسمتعریف T2 value; };

سانچہ <قسم_نام Returned,
	  قسم_نام Subclass = خالی,
	  قسم_نام ڈیٹا_3 = خالی,
	  غیرعلامتی صحیح Wheres_ڈیٹا = 0,
	  قسم_نام Stored = Returned>
ساخت hb_lazy_loader_t : hb_ڈیٹا_wrapper_t<ڈیٹا_3, Wheres_ڈیٹا>
{
  قسمتعریف قسم_نام hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,ڈیٹا_3,Wheres_ڈیٹا,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = طےشدہ;
  hb_lazy_loader_t (ثابت hb_lazy_loader_t &other) = حذف;

  خالی init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  خالی init ()  { instance.سیٹ_relaxed (خالی_اشاریہ); }
  خالی fini ()  { do_destroy (instance.get_acquire ()); init (); }

  خالی free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    اگر (unlikely (p && !cmpexch (p, خالی_اشاریہ)))
      جاؤ retry;
    do_destroy (p);
  }

  ساکن خالی do_destroy (Stored *p)
  {
    اگر (Funcs::لیبل_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      واپس;
    Funcs::destroy (p);
  }

  ثابت Returned * عامل -> () ثابت { واپس get (); }
  سانچہ <قسم_نام U = Returned, hb_enable_if (!hb_is_same (U, خالی))>
  ثابت U & عامل * () ثابت  { واپس *get (); }
  صریح عامل منطقی () ثابت
  {
    Stored *p = get_stored ();
    اگر (Funcs::لیبل_null)
    {
      Stored *raw = یہ->instance.get_relaxed ();
      واپس raw && !is_tagged_null (raw);
    }
    واپس p != Funcs::get_null ();
  }
  سانچہ <قسم_نام C> عامل ثابت C * () ثابت { واپس get (); }

  Stored * get_stored () ثابت
  {
  retry:
    Stored *p = یہ->instance.get_acquire ();
    اگر (unlikely (!p))
    {
      اگر (unlikely (یہ->is_inert ()))
	واپس const_cast<Stored *> (Funcs::get_null ());

      p = یہ->سانچہ call_create<Stored, Funcs> ();
      اگر (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      اگر (unlikely (!cmpexch (خالی_اشاریہ, p)))
      {
	do_destroy (p);
	جاؤ retry;
      }
    }
    واپس untagged (p);
  }
  Stored * get_stored_relaxed () ثابت
  {
    واپس untagged (یہ->instance.get_relaxed ());
  }

  منطقی cmpexch (Stored *current, Stored *value) ثابت
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    واپس یہ->instance.cmpexch (current, value);
  }

  ثابت Returned * get () ثابت { واپس Funcs::convert (get_stored ()); }
  ثابت Returned * get_relaxed () ثابت { واپس Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () ثابت { واپس const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  ساکن Returned* convert (Stored *p) { واپس p; }

  /* By default null/init/fini the object. */
  ساکن ثابت Stored* get_null () { واپس &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  ساکن constexpr منطقی لیبل_null = جھوٹ;
  ساکن منطقی is_tagged_null (Stored *p)
  { واپس Funcs::لیبل_null && ((uintptr_t) (خالی *) p & 1); }
  ساکن Stored *tagged_null (Stored *p)
  { واپس Funcs::لیبل_null ? (Stored *) (خالی *) ((uintptr_t) (خالی *) p | 1) : p; }
  ساکن Stored *untagged (Stored *p)
  { واپس Funcs::لیبل_null ? (Stored *) (خالی *) ((uintptr_t) (خالی *) p & ~(uintptr_t) 1) : p; }
  ساکن Stored *create (ڈیٹا_3 *ڈیٹا)
  {
    Stored *p = (Stored *) hb_calloc (1, حجم (Stored));
    اگر (likely (p))
      p = نیا (p) Stored (ڈیٹا);
    واپس p;
  }
  ساکن Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, حجم (Stored));
    اگر (likely (p))
      p = نیا (p) Stored ();
    واپس p;
  }
  ساکن خالی destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  نجی:
  /* Must only have one pointer. */
  تغیرپذیر hb_atomic_t<Stored *> instance;
};

/* Specializations. */

سانچہ <قسم_نام T, غیرعلامتی صحیح WheresFace>
ساخت hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  ساکن constexpr منطقی لیبل_null = سچ;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { واپس یہ->get ()->get_blob (); }
};

سانچہ <قسم_نام T, غیرعلامتی صحیح WheresFace>
ساخت hb_جدول_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_جدول_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  ساکن hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    واپس c.reference_جدول<T> (face);
  }
  ساکن خالی destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  ساکن ثابت hb_blob_t *get_null ()
  { واپس hb_blob_get_خالی_خانے (); }

  ساکن ثابت T* convert (ثابت hb_blob_t *blob)
  { واپس blob->as<T> (); }

  hb_blob_t* get_blob () ثابت { واپس یہ->get_stored (); }
};

#تعریف_کرو HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  سانچہ <قسم_نام Subclass> \
  ساخت hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    ساکن خالی destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    ساکن ثابت hb_##Type##_funcs_t *get_null () \
    { واپس hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#تعریف_منسوخ HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#اگر_ختم /* HB_MACHINERY_HH */
