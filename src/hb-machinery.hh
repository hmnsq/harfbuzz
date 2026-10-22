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

#నిర్వచితం_కాకపోతే HB_MACHINERY_HH
#నిర్వచించు HB_MACHINERY_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-blob.hh"

#అన్తర్భావ "hb-dispatch.hh"
#అన్తర్భావ "hb-sanitize.hh"


/*
 * Casts
 */

/* StructAtOffset<T>(P,Ofs) returns the struct T& that is placed at memory
 * location pointed to by P plus Ofs bytes. */
మూస<రకం_పేరు Type>
స్థిర అంతఃవరుస స్థిరరాశి Type& Struct_At_స్థానభ్రంశం(స్థిరరాశి శూన్య *P, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం)
{ తిరిగి * reinterpret_cast<స్థిరరాశి Type*> ((స్థిరరాశి అక్షరం *) P + స్థానభ్రంశం); }
మూస<రకం_పేరు Type>
స్థిర అంతఃవరుస Type& Struct_At_స్థానభ్రంశం(శూన్య *P, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం)
{ తిరిగి * reinterpret_cast<Type*> ((అక్షరం *) P + స్థానభ్రంశం); }
మూస<రకం_పేరు Type>
స్థిర అంతఃవరుస స్థిరరాశి Type& Struct_At_స్థానభ్రంశం_Unaligned(స్థిరరాశి శూన్య *P, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం)
{
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-align"
  తిరిగి * reinterpret_cast<స్థిరరాశి Type*> ((స్థిరరాశి అక్షరం *) P + స్థానభ్రంశం);
#సూచన GCC diagnostic తీయి
}
మూస<రకం_పేరు Type>
స్థిర అంతఃవరుస Type& Struct_At_స్థానభ్రంశం_Unaligned(శూన్య *P, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం)
{
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-align"
  తిరిగి * reinterpret_cast<Type*> ((అక్షరం *) P + స్థానభ్రంశం);
#సూచన GCC diagnostic తీయి
}

/* StructAfter<T>(X) returns the struct T& that is placed after X.
 * Works with X of variable size also.  X must implement get_size().
 * Any extra arguments are forwarded to get_size, so for example
 * it can work with UnsizedArrayOf<> as well. */
మూస <రకం_పేరు Type, రకం_పేరు TObject, రకం_పేరు ...Ts>
స్థిర అంతఃవరుస స్వయంచాలక StructAfter(స్థిరరాశి TObject &X, Ts... వాదనలు) HB_AUTO_RETURN((
  Struct_At_స్థానభ్రంశం<Type>(&X, X.get_కొలత(std::forward<Ts> (వాదనలు)...))
))
/* The is_const shenanigans is to avoid ambiguous overload with gcc-8.
 * It disables this path when TObject is const.
 * See: https://github.com/harfbuzz/harfbuzz/issues/5429 */
మూస <రకం_పేరు Type, రకం_పేరు TObject, రకం_పేరు ...Ts>
స్థిర అంతఃవరుస స్వయంచాలక StructAfter(TObject &X, Ts... వాదనలు) HB_AUTO_RETURN((
  పరిమాణం(పూర్ణాంకం[std::is_const<TObject>::value ? -1 : +1]) > 0 ?
  Struct_At_స్థానభ్రంశం<Type>(&X, X.get_కొలత(std::forward<Ts> (వాదనలు)...))
  : *reinterpret_cast<Type*> (0)
))


/*
 * Size checking
 */

/* Size signifying variable-sized array */
#నిర్వచితం_కాకపోతే HB_VAR_ARRAY
#నిర్వచించు HB_VAR_ARRAY 1
#అయితే_ముగింపు

/* Check _assertion in a method environment */
#నిర్వచించు _DEFINE_INSTANCE_ASSERTION1(పంక్తి_2, _assertion) \
  శూన్య instance_assertion_on_పంక్తి##_line () స్థిరరాశి \
  { static_assert ((_assertion), ""); }
# define _DEFINE_INSTANCE_ASSERTION0(పంక్తి_2, _assertion) _DEFINE_INSTANCE_ASSERTION1 (పంక్తి_2, _assertion)
# define DEFINE_INSTANCE_ASSERTION(_assertion) _DEFINE_INSTANCE_ASSERTION0 (__LINE__, _assertion)

/* Check that _code compiles in a method environment */
#నిర్వచించు _DEFINE_COMPILES_ASSERTION1(పంక్తి_2, సంకేతం_4) \
  శూన్య compiles_assertion_on_పంక్తి##_line () స్థిరరాశి \
  { సంకేతం_4; }
# define _DEFINE_COMPILES_ASSERTION0(పంక్తి_2, సంకేతం_4) _DEFINE_COMPILES_ASSERTION1 (పంక్తి_2, సంకేతం_4)
# define DEFINE_COMPILES_ASSERTION(సంకేతం_4) _DEFINE_COMPILES_ASSERTION0 (__LINE__, సంకేతం_4)


#నిర్వచించు DEFINE_కొలత_STATIC(కొలత) \
  DEFINE_INSTANCE_ASSERTION (పరిమాణం (*ఇది) == (కొలత)) \
  size_t get_కొలత () స్థిరరాశి { తిరిగి (కొలత); } \
  స్థిర constexpr గుర్తులేని null_కొలత = (కొలత); \
  స్థిర constexpr గుర్తులేని min_కొలత = (కొలత); \
  స్థిర constexpr గుర్తులేని static_కొలత = (కొలత)

#నిర్వచించు DEFINE_కొలత_UNION(కొలత, _member) \
  DEFINE_COMPILES_ASSERTION ((శూన్య) ఇది->u._member.static_కొలత) \
  DEFINE_INSTANCE_ASSERTION (పరిమాణం(ఇది->u._member) == (కొలత)) \
  స్థిర constexpr గుర్తులేని null_కొలత = (కొలత); \
  స్థిర constexpr గుర్తులేని min_కొలత = (కొలత)

#నిర్వచించు DEFINE_కొలత_MIN(కొలత) \
  DEFINE_INSTANCE_ASSERTION (పరిమాణం (*ఇది) >= (కొలత)) \
  స్థిర constexpr గుర్తులేని null_కొలత = (కొలత); \
  స్థిర constexpr గుర్తులేని min_కొలత = (కొలత)

#నిర్వచించు DEFINE_కొలత_UNBOUNDED(కొలత) \
  DEFINE_INSTANCE_ASSERTION (పరిమాణం (*ఇది) >= (కొలత)) \
  స్థిర constexpr గుర్తులేని min_కొలత = (కొలత)

#నిర్వచించు DEFINE_కొలత_ARRAY(కొలత, array) \
  DEFINE_COMPILES_ASSERTION ((శూన్య) (array)[0].static_కొలత) \
  DEFINE_INSTANCE_ASSERTION (పరిమాణం (*ఇది) == (కొలత) + (HB_VAR_ARRAY+0) * పరిమాణం ((array)[0])) \
  స్థిర constexpr గుర్తులేని null_కొలత = (కొలత); \
  స్థిర constexpr గుర్తులేని min_కొలత = (కొలత)

#నిర్వచించు DEFINE_కొలత_ARRAY_SIZED(కొలత, array) \
  size_t get_కొలత () స్థిరరాశి { తిరిగి (కొలత - (array).min_కొలత + (array).get_కొలత ()); } \
  DEFINE_కొలత_ARRAY(కొలత, array)


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

మూస <రకం_పేరు దత్తాంశం_3, గుర్తులేని పూర్ణాంకం Wheres_దత్తాంశం>
నిర్మాణం hb_దత్తాంశం_wrapper_t
{
  static_assert (Wheres_దత్తాంశం > 0, "");

  దత్తాంశం_3 * get_దత్తాంశం () స్థిరరాశి
  { తిరిగి *(((దత్తాంశం_3 **) (శూన్య *) ఇది) - Wheres_దత్తాంశం); }

  తార్కిక is_inert () స్థిరరాశి { తిరిగి !get_దత్తాంశం (); }

  మూస <రకం_పేరు Stored, రకం_పేరు Subclass>
  Stored * call_create () స్థిరరాశి { తిరిగి Subclass::create (get_దత్తాంశం ()); }
};
మూస <>
నిర్మాణం hb_దత్తాంశం_wrapper_t<శూన్య, 0>
{
  తార్కిక is_inert () స్థిరరాశి { తిరిగి అబద్ధం; }

  మూస <రకం_పేరు Stored, రకం_పేరు Funcs>
  Stored * call_create () స్థిరరాశి { తిరిగి Funcs::create (); }
};

మూస <రకం_పేరు T1, రకం_పేరు T2> నిర్మాణం hb_non_void_t { రకనిర్వచనం T1 value; };
మూస <రకం_పేరు T2> నిర్మాణం hb_non_void_t<శూన్య, T2> { రకనిర్వచనం T2 value; };

మూస <రకం_పేరు Returned,
	  రకం_పేరు Subclass = శూన్య,
	  రకం_పేరు దత్తాంశం_3 = శూన్య,
	  గుర్తులేని పూర్ణాంకం Wheres_దత్తాంశం = 0,
	  రకం_పేరు Stored = Returned>
నిర్మాణం hb_lazy_loader_t : hb_దత్తాంశం_wrapper_t<దత్తాంశం_3, Wheres_దత్తాంశం>
{
  రకనిర్వచనం రకం_పేరు hb_non_void_t<Subclass,
				 hb_lazy_loader_t<Returned,Subclass,దత్తాంశం_3,Wheres_దత్తాంశం,Stored>
				>::value Funcs;

  hb_lazy_loader_t () = అప్రమేయ;
  hb_lazy_loader_t (స్థిరరాశి hb_lazy_loader_t &other) = తొలగించు;

  శూన్య init0 () {} /* Init, when memory is already set to 0. No-op for us. */
  శూన్య init ()  { instance.సమితి_relaxed (శూన్యసూచిక); }
  శూన్య fini ()  { do_destroy (instance.get_acquire ()); init (); }

  శూన్య free_instance ()
  {
  retry:
    Stored *p = instance.get_acquire ();
    ఒకవేళ (unlikely (p && !cmpexch (p, శూన్యసూచిక)))
      వెళ్ళు retry;
    do_destroy (p);
  }

  స్థిర శూన్య do_destroy (Stored *p)
  {
    ఒకవేళ (Funcs::ట్యాగ్_null ? !p || is_tagged_null (p) :
			  !p || p == const_cast<Stored *> (Funcs::get_null ()))
      తిరిగి;
    Funcs::destroy (p);
  }

  స్థిరరాశి Returned * క్రియాకారకం -> () స్థిరరాశి { తిరిగి get (); }
  మూస <రకం_పేరు U = Returned, hb_enable_if (!hb_is_same (U, శూన్య))>
  స్థిరరాశి U & క్రియాకారకం * () స్థిరరాశి  { తిరిగి *get (); }
  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి
  {
    Stored *p = get_stored ();
    ఒకవేళ (Funcs::ట్యాగ్_null)
    {
      Stored *raw = ఇది->instance.get_relaxed ();
      తిరిగి raw && !is_tagged_null (raw);
    }
    తిరిగి p != Funcs::get_null ();
  }
  మూస <రకం_పేరు C> క్రియాకారకం స్థిరరాశి C * () స్థిరరాశి { తిరిగి get (); }

  Stored * get_stored () స్థిరరాశి
  {
  retry:
    Stored *p = ఇది->instance.get_acquire ();
    ఒకవేళ (unlikely (!p))
    {
      ఒకవేళ (unlikely (ఇది->is_inert ()))
	తిరిగి const_cast<Stored *> (Funcs::get_null ());

      p = ఇది->మూస call_create<Stored, Funcs> ();
      ఒకవేళ (unlikely (!p))
	p = tagged_null (const_cast<Stored *> (Funcs::get_null ()));

      ఒకవేళ (unlikely (!cmpexch (శూన్యసూచిక, p)))
      {
	do_destroy (p);
	వెళ్ళు retry;
      }
    }
    తిరిగి untagged (p);
  }
  Stored * get_stored_relaxed () స్థిరరాశి
  {
    తిరిగి untagged (ఇది->instance.get_relaxed ());
  }

  తార్కిక cmpexch (Stored *current, Stored *value) స్థిరరాశి
  {
    /* This function can only be safely called directly if no
     * other thread is accessing. */
    తిరిగి ఇది->instance.cmpexch (current, value);
  }

  స్థిరరాశి Returned * get () స్థిరరాశి { తిరిగి Funcs::convert (get_stored ()); }
  స్థిరరాశి Returned * get_relaxed () స్థిరరాశి { తిరిగి Funcs::convert (get_stored_relaxed ()); }
  Returned * get_unconst () స్థిరరాశి { తిరిగి const_cast<Returned *> (Funcs::convert (get_stored ())); }

  /* To be possibly overloaded by subclasses. */
  స్థిర Returned* convert (Stored *p) { తిరిగి p; }

  /* By default null/init/fini the object. */
  స్థిర స్థిరరాశి Stored* get_null () { తిరిగి &Null (Stored); }

  /* When tag_null is set, a failed create () is cached as get_null () with
   * its low bit set, and destroy recognizes it by that bit rather than by
   * address.  Needed when get_null () is a per-library Null object: another
   * library sharing the object would not recognize its address. */
  స్థిర constexpr తార్కిక ట్యాగ్_null = అబద్ధం;
  స్థిర తార్కిక is_tagged_null (Stored *p)
  { తిరిగి Funcs::ట్యాగ్_null && ((uintptr_t) (శూన్య *) p & 1); }
  స్థిర Stored *tagged_null (Stored *p)
  { తిరిగి Funcs::ట్యాగ్_null ? (Stored *) (శూన్య *) ((uintptr_t) (శూన్య *) p | 1) : p; }
  స్థిర Stored *untagged (Stored *p)
  { తిరిగి Funcs::ట్యాగ్_null ? (Stored *) (శూన్య *) ((uintptr_t) (శూన్య *) p & ~(uintptr_t) 1) : p; }
  స్థిర Stored *create (దత్తాంశం_3 *దత్తాంశం)
  {
    Stored *p = (Stored *) hb_calloc (1, పరిమాణం (Stored));
    ఒకవేళ (likely (p))
      p = కొత్త (p) Stored (దత్తాంశం);
    తిరిగి p;
  }
  స్థిర Stored *create ()
  {
    Stored *p = (Stored *) hb_calloc (1, పరిమాణం (Stored));
    ఒకవేళ (likely (p))
      p = కొత్త (p) Stored ();
    తిరిగి p;
  }
  స్థిర శూన్య destroy (Stored *p)
  {
    p->~Stored ();
    hb_free (p);
  }

  అంతరంగ:
  /* Must only have one pointer. */
  మార్చదగిన hb_atomic_t<Stored *> instance;
};

/* Specializations. */

మూస <రకం_పేరు T, గుర్తులేని పూర్ణాంకం WheresFace>
నిర్మాణం hb_face_lazy_loader_t : hb_lazy_loader_t<T,
						hb_face_lazy_loader_t<T, WheresFace>,
						hb_face_t, WheresFace>
{
  /* get_null () is this library's Null object; see tag_null. */
  స్థిర constexpr తార్కిక ట్యాగ్_null = నిజం;

  // Hack; have them here for API parity with hb_table_lazy_loader_t
  hb_blob_t *get_blob () { తిరిగి ఇది->get ()->get_blob (); }
};

మూస <రకం_పేరు T, గుర్తులేని పూర్ణాంకం WheresFace>
నిర్మాణం hb_పట్టిక_lazy_loader_t : hb_lazy_loader_t<T,
						 hb_పట్టిక_lazy_loader_t<T, WheresFace>,
						 hb_face_t, WheresFace,
						 hb_blob_t>
{
  స్థిర hb_blob_t *create (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    తిరిగి c.reference_పట్టిక<T> (face);
  }
  స్థిర శూన్య destroy (hb_blob_t *p) { hb_blob_destroy (p); }

  స్థిర స్థిరరాశి hb_blob_t *get_null ()
  { తిరిగి hb_blob_get_ఖాళీలు (); }

  స్థిర స్థిరరాశి T* convert (స్థిరరాశి hb_blob_t *blob)
  { తిరిగి blob->as<T> (); }

  hb_blob_t* get_blob () స్థిరరాశి { తిరిగి ఇది->get_stored (); }
};

#నిర్వచించు HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T(Type) \
  మూస <రకం_పేరు Subclass> \
  నిర్మాణం hb_##Type##_funcs_lazy_loader_t : hb_lazy_loader_t<hb_##Type##_funcs_t, Subclass> \
  { \
    స్థిర శూన్య destroy (hb_##Type##_funcs_t *p) \
    { hb_##Type##_funcs_destroy (p); } \
    స్థిర స్థిరరాశి hb_##Type##_funcs_t *get_null () \
    { తిరిగి hb_##Type##_funcs_get_empty (); } \
  }

HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (font);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (unicode);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (draw);
HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T (paint);

#నిర్వచనం_రద్దు HB_DEFINE_TYPE_FUNCS_LAZY_LOADER_T


#అయితే_ముగింపు /* HB_MACHINERY_HH */
