/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_ARRAY_HH
#నిర్వచించు HB_ARRAY_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-algs.hh"
#అన్తర్భావ "hb-iter.hh"
#అన్తర్భావ "hb-null.hh"


మూస <రకం_పేరు Type>
నిర్మాణం hb_sorted_array_t;

గణన hb_not_దొరికింది_t
{
  HB_NOT_దొరికింది_DONT_STORE,
  HB_NOT_దొరికింది_STORE,
  HB_NOT_దొరికింది_STORE_CLOSEST,
};


మూస <రకం_పేరు Type>
నిర్మాణం hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  స్థిర constexpr తార్కిక realloc_move = నిజం;

  /*
   * Constructors.
   */
  hb_array_t () = అప్రమేయ;
  hb_array_t (స్థిరరాశి hb_array_t&) = అప్రమేయ;
  ~hb_array_t () = అప్రమేయ;
  hb_array_t& క్రియాకారకం= (స్థిరరాశి hb_array_t&) = అప్రమేయ;
  hb_array_t& క్రియాకారకం= (hb_array_t&&) = అప్రమేయ;

  constexpr hb_array_t (Type *array_, గుర్తులేని పూర్ణాంకం నిడివి_2) : arrayZ (array_), నిడివి (నిడివి_2) {}
  మూస <గుర్తులేని పూర్ణాంకం నిడివి_2>
  constexpr hb_array_t (Type (&array_)[నిడివి_2]) : hb_array_t (array_, నిడివి_2) {}

  మూస <రకం_పేరు U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (స్థిరరాశి hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), నిడివి (o.నిడివి), backwards_నిడివి (o.backwards_నిడివి) {}
  మూస <రకం_పేరు U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& క్రియాకారకం = (స్థిరరాశి hb_array_t<U> &o)
  { arrayZ = o.arrayZ; నిడివి = o.నిడివి; backwards_నిడివి = o.backwards_నిడివి; తిరిగి *ఇది; }

  /*
   * Iterator implementation.
   */
  రకనిర్వచనం Type& __item_t__;
  స్థిర constexpr తార్కిక is_random_access_iterator = నిజం;
  స్థిర constexpr తార్కిక has_fast_len = నిజం;
  Type& __item__ () స్థిరరాశి
  {
    ఒకవేళ (unlikely (!నిడివి)) తిరిగి CrapOrNull (Type);
    తిరిగి *arrayZ;
  }
  Type& __item_at__ (గుర్తులేని i) స్థిరరాశి
  {
    ఒకవేళ (unlikely (i >= నిడివి)) తిరిగి CrapOrNull (Type);
    తిరిగి arrayZ[i];
  }
  శూన్య __next__ ()
  {
    ఒకవేళ (unlikely (!నిడివి))
      తిరిగి;
    నిడివి--;
    backwards_నిడివి++;
    arrayZ++;
  }
  శూన్య __forward__ (గుర్తులేని n)
  {
    ఒకవేళ (unlikely (n > నిడివి))
      n = నిడివి;
    నిడివి -= n;
    backwards_నిడివి += n;
    arrayZ += n;
  }
  శూన్య __prev__ ()
  {
    ఒకవేళ (unlikely (!backwards_నిడివి))
      తిరిగి;
    నిడివి++;
    backwards_నిడివి--;
    arrayZ--;
  }
  శూన్య __rewind__ (గుర్తులేని n)
  {
    ఒకవేళ (unlikely (n > backwards_నిడివి))
      n = backwards_నిడివి;
    నిడివి += n;
    backwards_నిడివి -= n;
    arrayZ -= n;
  }
  గుర్తులేని __len__ () స్థిరరాశి { తిరిగి నిడివి; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_array_t& o) స్థిరరాశి
  { తిరిగి ఇది->arrayZ != o.arrayZ || ఇది->నిడివి != o.నిడివి; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () స్థిరరాశి { తిరిగి arrayZ; }
  Type *చివర () స్థిరరాశి { తిరిగి arrayZ + నిడివి; }


  /* Extra operators.
   */
  Type * క్రియాకారకం & () స్థిరరాశి { తిరిగి arrayZ; }
  క్రియాకారకం hb_array_t<స్థిరరాశి Type> () { తిరిగి hb_array_t<స్థిరరాశి Type> (arrayZ, నిడివి); }
  మూస <రకం_పేరు T> క్రియాకారకం T * () స్థిరరాశి { తిరిగి arrayZ; }

  HB_అంతర్గత తార్కిక క్రియాకారకం == (స్థిరరాశి hb_array_t &o) స్థిరరాశి;

  uint32_t hash () స్థిరరాశి
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    కొరకు (స్వయంచాలక &v : *ఇది)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    తిరిగి current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  పూర్ణాంకం cmp (స్థిరరాశి hb_array_t &a) స్థిరరాశి
  {
    ఒకవేళ (నిడివి != a.నిడివి)
      తిరిగి (పూర్ణాంకం) a.నిడివి - (పూర్ణాంకం) నిడివి;
    తిరిగి hb_memcmp (a.arrayZ, arrayZ, get_కొలత ());
  }
  HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    తిరిగి b->cmp (*a);
  }

  మూస <రకం_పేరు T>
  Type *lsearch (స్థిరరాశి T &x, Type *not_దొరికింది = శూన్యసూచిక)
  {
    గుర్తులేని i;
    తిరిగి lfind (x, &i) ? &ఇది->arrayZ[i] : not_దొరికింది;
  }
  మూస <రకం_పేరు T>
  స్థిరరాశి Type *lsearch (స్థిరరాశి T &x, స్థిరరాశి Type *not_దొరికింది = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని i;
    తిరిగి lfind (x, &i) ? &ఇది->arrayZ[i] : not_దొరికింది;
  }
  మూస <రకం_పేరు T>
  తార్కిక lfind (స్థిరరాశి T &x, గుర్తులేని *pos = శూన్యసూచిక,
	      hb_not_దొరికింది_t not_దొరికింది = HB_NOT_దొరికింది_DONT_STORE,
	      గుర్తులేని పూర్ణాంకం గమ్యం_store = (గుర్తులేని పూర్ణాంకం) -1) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < నిడివి; ++i)
      ఒకవేళ (hb_equal (x, ఇది->arrayZ[i]))
      {
	ఒకవేళ (pos)
	  *pos = i;
	తిరిగి నిజం;
      }

    ఒకవేళ (pos)
    {
      ఎంపిక (not_దొరికింది)
      {
	స్థితి HB_NOT_దొరికింది_DONT_STORE:
	  ఆపు;

	స్థితి HB_NOT_దొరికింది_STORE:
	  *pos = గమ్యం_store;
	  ఆపు;

	స్థితి HB_NOT_దొరికింది_STORE_CLOSEST:
	  *pos = నిడివి;
	  ఆపు;
      }
    }
    తిరిగి అబద్ధం;
  }

  hb_sorted_array_t<Type> qsort (పూర్ణాంకం (*cmp)(స్థిరరాశి శూన్య*, స్థిరరాశి శూన్య*))
  {
    ఒకవేళ (likely (నిడివి))
      hb_qsort (arrayZ, నిడివి, ఇది->get_అంశం_కొలత (), cmp);
    తిరిగి hb_sorted_array_t<Type> (*ఇది);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  మూస <రకం_పేరు Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    ఒకవేళ (likely (నిడివి))
      hb_qsort_inline (arrayZ, నిడివి, compar);
    తిరిగి hb_sorted_array_t<Type> (*ఇది);
  }

  అంతరంగ:
  మూస <రకం_పేరు T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_ప్రాధాన్యత<1>)
  {
    తిరిగి qsort ([] (స్థిరరాశి Type &a, స్థిరరాశి Type &b) { తిరిగి Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_ప్రాధాన్యత<0>)
  {
    తిరిగి qsort ((పూర్ణాంకం(*)(స్థిరరాశి శూన్య*, స్థిరరాశి శూన్య*)) Type::cmp);
  }
  బహిరంగ:

  hb_sorted_array_t<Type> qsort ()
  {
    తిరిగి _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_కొలత () స్థిరరాశి { తిరిగి నిడివి * ఇది->get_అంశం_కొలత (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  శూన్య reverse (గుర్తులేని ప్రారంభం = 0, గుర్తులేని చివర = -1)
  {
    ప్రారంభం = hb_min (ప్రారంభం, నిడివి);
    చివర = hb_min (చివర, నిడివి);

    ఒకవేళ (చివర < ప్రారంభం + 2)
      తిరిగి;

    గుర్తులేని stop = ప్రారంభం + (చివర - ప్రారంభం) / 2;
    కొరకు (గుర్తులేని lhs = ప్రారంభం, rhs = చివర - 1; lhs < stop; lhs++, rhs--)
      hb_మార్పిడి (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం = 0, గుర్తులేని పూర్ణాంకం *seg_లెక్క = శూన్యసూచిక /* IN/OUT */) స్థిరరాశి
  {
    ఒకవేళ (!ప్రారంభం_స్థానభ్రంశం && !seg_లెక్క)
      తిరిగి *ఇది;

    గుర్తులేని పూర్ణాంకం లెక్క = నిడివి;
    ఒకవేళ (unlikely (ప్రారంభం_స్థానభ్రంశం > లెక్క))
      లెక్క = 0;
    లేకపోతే
      లెక్క -= ప్రారంభం_స్థానభ్రంశం;
    ఒకవేళ (seg_లెక్క)
      లెక్క = *seg_లెక్క = hb_min (లెక్క, *seg_లెక్క);
    తిరిగి hb_array_t (arrayZ + ప్రారంభం_స్థానభ్రంశం, లెక్క);
  }
  hb_array_t sub_array (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం, గుర్తులేని పూర్ణాంకం seg_లెక్క) స్థిరరాశి
  { తిరిగి sub_array (ప్రారంభం_స్థానభ్రంశం, &seg_లెక్క); }

  hb_array_t truncate (గుర్తులేని నిడివి) స్థిరరాశి { తిరిగి sub_array (0, నిడివి); }

  మూస <రకం_పేరు T,
	    గుర్తులేని P = పరిమాణం (Type),
	    hb_enable_if (P == 1)>
  స్థిరరాశి T *as () స్థిరరాశి
  { తిరిగి నిడివి < hb_min_కొలత (T) ? &Null (T) : reinterpret_cast<స్థిరరాశి T *> (arrayZ); }

  మూస <రకం_పేరు T,
	    గుర్తులేని P = పరిమాణం (Type),
	    hb_enable_if (P == 1)>
  తార్కిక check_range (స్థిరరాశి T *p, గుర్తులేని పూర్ణాంకం కొలత = T::static_కొలత) స్థిరరాశి
  {
    తిరిగి arrayZ <= ((స్థిరరాశి అక్షరం *) p)
	&& ((స్థిరరాశి అక్షరం *) p) <= arrayZ + నిడివి
	&& (గుర్తులేని పూర్ణాంకం) (arrayZ + నిడివి - (స్థిరరాశి అక్షరం *) p) >= కొలత;
  }

  మూస <గుర్తులేని P = పరిమాణం (Type),
	    hb_enable_if (P == 1)>
  తార్కిక check_చివర (స్థిరరాశి శూన్య *p) స్థిరరాశి
  {
    తిరిగి (uintptr_t) (((స్థిరరాశి అక్షరం *) p) - arrayZ) <= నిడివి;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  శూన్య fini ()
  { hb_free ((శూన్య *) arrayZ); arrayZ = శూన్యసూచిక; నిడివి = 0; }

  మూస <రకం_పేరు hb_serialize_context_t,
	    రకం_పేరు U = Type,
	    hb_enable_if (!(పరిమాణం (U) < పరిమాణం (దీర్ఘ దీర్ఘ) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక* బహిర్గతం = c->ప్రారంభం_embed (arrayZ);
    ఒకవేళ (unlikely (!c->extend_కొలత (బహిర్గతం, get_కొలత (), అబద్ధం))) return_trace (hb_array_t ());
    కొరకు (గుర్తులేని i = 0; i < నిడివి; i++)
      బహిర్గతం[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (బహిర్గతం, నిడివి));
  }

  మూస <రకం_పేరు hb_serialize_context_t,
	    రకం_పేరు U = Type,
	    hb_enable_if (పరిమాణం (U) < పరిమాణం (దీర్ఘ దీర్ఘ) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక* బహిర్గతం = c->ప్రారంభం_embed (arrayZ);
    ఒకవేళ (unlikely (!c->extend_కొలత (బహిర్గతం, get_కొలత (), అబద్ధం))) return_trace (hb_array_t ());
    hb_memcpy (బహిర్గతం, arrayZ, get_కొలత ());
    return_trace (hb_array_t (బహిర్గతం, నిడివి));
  }

  మూస <రకం_పేరు hb_sanitize_context_t>
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  { తిరిగి c->check_array (arrayZ, నిడివి); }

  /*
   * Members
   */

  బహిరంగ:
  Type *arrayZ = శూన్యసూచిక;
  గుర్తులేని పూర్ణాంకం నిడివి = 0;
  గుర్తులేని పూర్ణాంకం backwards_నిడివి = 0;
};
మూస <రకం_పేరు T> అంతఃవరుస hb_array_t<T>
hb_array ()
{ తిరిగి hb_array_t<T> (); }
మూస <రకం_పేరు T> అంతఃవరుస hb_array_t<T>
hb_array (T *array, గుర్తులేని పూర్ణాంకం నిడివి)
{ తిరిగి hb_array_t<T> (array, నిడివి); }
మూస <రకం_పేరు T, గుర్తులేని పూర్ణాంకం నిడివి_2> అంతఃవరుస hb_array_t<T>
hb_array (T (&array_)[నిడివి_2])
{ తిరిగి hb_array_t<T> (array_); }

మూస <రకం_పేరు Type>
నిర్మాణం hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  రకనిర్వచనం hb_iter_t<hb_sorted_array_t, Type&> iter_పునాది_t;
  HB_ITER_USING (iter_పునాది_t);
  స్థిర constexpr తార్కిక is_random_access_iterator = నిజం;
  స్థిర constexpr తార్కిక is_sorted_iterator = నిజం;
  స్థిర constexpr తార్కిక has_fast_len = నిజం;

  hb_sorted_array_t () = అప్రమేయ;
  hb_sorted_array_t (స్థిరరాశి hb_sorted_array_t&) = అప్రమేయ;
  ~hb_sorted_array_t () = అప్రమేయ;
  hb_sorted_array_t& క్రియాకారకం= (స్థిరరాశి hb_sorted_array_t&) = అప్రమేయ;
  hb_sorted_array_t& క్రియాకారకం= (hb_sorted_array_t&&) = అప్రమేయ;

  constexpr hb_sorted_array_t (Type *array_, గుర్తులేని పూర్ణాంకం నిడివి_2) : hb_array_t<Type> (array_, నిడివి_2) {}
  మూస <గుర్తులేని పూర్ణాంకం నిడివి_2>
  constexpr hb_sorted_array_t (Type (&array_)[నిడివి_2]) : hb_array_t<Type> (array_) {}

  మూస <రకం_పేరు U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (స్థిరరాశి hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  మూస <రకం_పేరు U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& క్రియాకారకం = (స్థిరరాశి hb_array_t<U> &o)
  { hb_array_t<Type> (*ఇది) = o; తిరిగి *ఇది; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_sorted_array_t& o) స్థిరరాశి
  { తిరిగి ఇది->arrayZ != o.arrayZ || ఇది->నిడివి != o.నిడివి; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () స్థిరరాశి { తిరిగి ఇది->arrayZ; }
  Type *చివర () స్థిరరాశి { తిరిగి ఇది->arrayZ + ఇది->నిడివి; }


  hb_sorted_array_t sub_array (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం, గుర్తులేని పూర్ణాంకం *seg_లెక్క /* IN/OUT */) స్థిరరాశి
  { తిరిగి hb_sorted_array_t (((స్థిరరాశి hb_array_t<Type> *) (ఇది))->sub_array (ప్రారంభం_స్థానభ్రంశం, seg_లెక్క)); }
  hb_sorted_array_t sub_array (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం, గుర్తులేని పూర్ణాంకం seg_లెక్క) స్థిరరాశి
  { తిరిగి sub_array (ప్రారంభం_స్థానభ్రంశం, &seg_లెక్క); }

  hb_sorted_array_t truncate (గుర్తులేని నిడివి) స్థిరరాశి { తిరిగి sub_array (0, నిడివి); }

  మూస <రకం_పేరు T>
  Type *bsearch (స్థిరరాశి T &x, Type *not_దొరికింది = శూన్యసూచిక)
  {
    గుర్తులేని పూర్ణాంకం i;
    తిరిగి bfind (x, &i) ? &ఇది->arrayZ[i] : not_దొరికింది;
  }
  మూస <రకం_పేరు T>
  స్థిరరాశి Type *bsearch (స్థిరరాశి T &x, స్థిరరాశి Type *not_దొరికింది = శూన్యసూచిక) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i;
    తిరిగి bfind (x, &i) ? &ఇది->arrayZ[i] : not_దొరికింది;
  }
  మూస <రకం_పేరు T>
  తార్కిక bfind (స్థిరరాశి T &x, గుర్తులేని పూర్ణాంకం *i = శూన్యసూచిక,
	      hb_not_దొరికింది_t not_దొరికింది = HB_NOT_దొరికింది_DONT_STORE,
	      గుర్తులేని పూర్ణాంకం గమ్యం_store = (గుర్తులేని పూర్ణాంకం) -1) స్థిరరాశి
  {
    గుర్తులేని pos;

    ఒకవేళ (bsearch_impl (x, &pos))
    {
      ఒకవేళ (i)
	*i = pos;
      తిరిగి నిజం;
    }

    ఒకవేళ (i)
    {
      ఎంపిక (not_దొరికింది)
      {
	స్థితి HB_NOT_దొరికింది_DONT_STORE:
	  ఆపు;

	స్థితి HB_NOT_దొరికింది_STORE:
	  *i = గమ్యం_store;
	  ఆపు;

	స్థితి HB_NOT_దొరికింది_STORE_CLOSEST:
	  *i = pos;
	  ఆపు;
      }
    }
    తిరిగి అబద్ధం;
  }
  మూస <రకం_పేరు T, రకం_పేరు ...Ts>
  తార్కిక bsearch_impl (స్థిరరాశి T &x, గుర్తులేని *pos, Ts... ds) స్థిరరాశి
  {
    తిరిగి hb_bsearch_impl (pos,
			    x,
			    ఇది->arrayZ,
			    ఇది->నిడివి,
			    పరిమాణం (Type),
			    hb_cmp_పద్ధతి<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
మూస <రకం_పేరు T> అంతఃవరుస hb_sorted_array_t<T>
hb_sorted_array (T *array, గుర్తులేని పూర్ణాంకం నిడివి)
{ తిరిగి hb_sorted_array_t<T> (array, నిడివి); }
మూస <రకం_పేరు T, గుర్తులేని పూర్ణాంకం నిడివి_2> అంతఃవరుస hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[నిడివి_2])
{ తిరిగి hb_sorted_array_t<T> (array_); }

మూస <రకం_పేరు T>
అంతఃవరుస తార్కిక hb_array_t<T>::క్రియాకారకం == (స్థిరరాశి hb_array_t<T> &o) స్థిరరాశి
{
  ఒకవేళ (o.నిడివి != ఇది->నిడివి) తిరిగి అబద్ధం;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ఇది->నిడివి; i++) {
    ఒకవేళ (ఇది->arrayZ[i] != o.arrayZ[i]) తిరిగి అబద్ధం;
  }
  తిరిగి నిజం;
}
మూస <>
అంతఃవరుస తార్కిక hb_array_t<స్థిరరాశి అక్షరం>::క్రియాకారకం == (స్థిరరాశి hb_array_t<స్థిరరాశి అక్షరం> &o) స్థిరరాశి
{
  ఒకవేళ (o.నిడివి != ఇది->నిడివి) తిరిగి అబద్ధం;
  తిరిగి 0 == hb_memcmp (arrayZ, o.arrayZ, నిడివి);
}
మూస <>
అంతఃవరుస తార్కిక hb_array_t<స్థిరరాశి గుర్తులేని అక్షరం>::క్రియాకారకం == (స్థిరరాశి hb_array_t<స్థిరరాశి గుర్తులేని అక్షరం> &o) స్థిరరాశి
{
  ఒకవేళ (o.నిడివి != ఇది->నిడివి) తిరిగి అబద్ధం;
  తిరిగి 0 == hb_memcmp (arrayZ, o.arrayZ, నిడివి);
}


/* Specialize hash() for byte arrays. */

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత_మరిన్ని
మూస <>
అంతఃవరుస uint32_t hb_array_t<స్థిరరాశి అక్షరం>::hash () స్థిరరాశి
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  తిరిగి fasthash32(arrayZ, నిడివి, 0xf437ffe6 /* magic? */);
}

మూస <>
అంతఃవరుస uint32_t hb_array_t<స్థిరరాశి గుర్తులేని అక్షరం>::hash () స్థిరరాశి
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  తిరిగి fasthash32(arrayZ, నిడివి, 0xf437ffe6 /* magic? */);
}
#అయితే_ముగింపు


రకనిర్వచనం hb_array_t<స్థిరరాశి అక్షరం> hb_బైట్లు_t;
రకనిర్వచనం hb_array_t<స్థిరరాశి గుర్తులేని అక్షరం> hb_ubytes_t;



#అయితే_ముగింపు /* HB_ARRAY_HH */
