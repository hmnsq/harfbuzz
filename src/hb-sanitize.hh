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

#నిర్వచితం_కాకపోతే HB_SANITIZE_HH
#నిర్వచించు HB_SANITIZE_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-blob.hh"
#అన్తర్భావ "hb-dispatch.hh"


/*
 * Sanitize
 *
 *
 * === Introduction ===
 *
 * The sanitize machinery is at the core of our zero-cost font loading.  We
 * mmap() font file into memory and create a blob out of it.  Font subtables
 * are returned as a readonly sub-blob of the main font blob.  These table
 * blobs are then sanitized before use, to ensure invalid memory access does
 * not happen.  The toplevel sanitize API use is like, eg. to load the 'head'
 * table:
 *
 *   hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<OT::head> (face);
 *
 * The blob then can be converted to a head table struct with:
 *
 *   const head *head_table = head_blob->as<head> ();
 *
 * What the reference_table does is, to call hb_face_reference_table() to load
 * the table blob, sanitize it and return either the sanitized blob, or empty
 * blob if sanitization failed.  The blob->as() function returns the null
 * object of its template type argument if the blob is empty.  Otherwise, it
 * just casts the blob contents to the desired type.
 *
 * Sanitizing a blob of data with a type T works as follows (with minor
 * simplification):
 *
 *   - Cast blob content to T*, call sanitize() method of it,
 *   - If sanitize succeeded, return blob.
 *   - Return empty blob otherwise.
 *
 *
 * === The sanitize() contract ===
 *
 * The sanitize() method of each object type shall return `true` if it's safe to
 * call other methods of the object, and `false` otherwise.
 *
 * Note that what sanitize() checks for might align with what the specification
 * describes as valid table data, but does not have to be.  In particular, we
 * do NOT want to be pedantic and concern ourselves with validity checks that
 * are irrelevant to our use of the table.  On the contrary, we want to be
 * lenient with error handling and accept invalid data to the extent that it
 * does not impose extra burden on us.
 *
 * Based on the sanitize contract, one can see that what we check for depends
 * on how we use the data in other table methods.  Ie. if other table methods
 * assume that offsets do NOT point out of the table data block, then that's
 * something sanitize() must check for (GSUB/GPOS/GDEF/etc work this way).  On
 * the other hand, if other methods do such checks themselves, then sanitize()
 * does not have to bother with them (glyf/local work this way).  The choice
 * depends on the table structure and sanitize() performance.  For example, to
 * check glyf/loca offsets in sanitize() would cost O(num-glyphs).  We try hard
 * to avoid such costs during font loading.  By postponing such checks to the
 * actual glyph loading, we reduce the sanitize cost to O(1) and total runtime
 * cost to O(used-glyphs).  As such, this is preferred.
 *
 * The same argument can be made re GSUB/GPOS/GDEF, but there, the table
 * structure is so complicated that by checking all offsets at sanitize() time,
 * we make the code much simpler in other methods, as offsets and referenced
 * objects do not need to be validated at each use site.
 *
 * Note:
 * Sanitize was named so because it used to try to recover from errors by
 * modifying the data to make it valid.  This is no longer the case, as it
 * could make HarfBuzz hallucinate new rules if there was aliasing in the
 * data.  However, the name stuck.  See: https://behdad.github.io/harfbust/
 */

/* This limits sanitizing time on really broken fonts. */
#నిర్వచితం_కాకపోతే HB_SANITIZE_MAX_EDITS
#నిర్వచించు HB_SANITIZE_MAX_EDITS 32
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_SANITIZE_MAX_OPS_గుణకం
#నిర్వచించు HB_SANITIZE_MAX_OPS_గుణకం 64
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_SANITIZE_MAX_OPS_MIN
#నిర్వచించు HB_SANITIZE_MAX_OPS_MIN 16384
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_SANITIZE_MAX_OPS_MAX
#నిర్వచించు HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_SANITIZE_MAX_SUBTABLES
#నిర్వచించు HB_SANITIZE_MAX_SUBTABLES 0x4000
#అయితే_ముగింపు

నిర్మాణం hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, తార్కిక, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (స్థిరరాశి అక్షరం *ప్రారంభం_2 = శూన్యసూచిక, స్థిరరాశి అక్షరం *చివర_4 = శూన్యసూచిక) :
	ప్రారంభం (ప్రారంభం_2), చివర (చివర_4),
	నిడివి (0),
	max_ops (0), max_subtables (0),
        recursion_లోతు (0),
	writable (అబద్ధం),
	blob (శూన్యసూచిక),
	అంకె_glyphs (65536),
	అంకె_glyphs_సమితి (అబద్ధం),
	lazy_some_gpos (అబద్ధం) {}

  స్థిరరాశి అక్షరం *get_name () { తిరిగి "SANITIZE"; }
  మూస <రకం_పేరు T, రకం_పేరు F>
  తార్కిక may_dispatch (స్థిరరాశి T *obj HB_వాడని, స్థిరరాశి F *format)
  {
    తిరిగి format->sanitize (ఇది) &&
	   hb_barrier ();
  }
  స్థిర return_t default_return_విలువ () { తిరిగి నిజం; }
  స్థిర return_t no_dispatch_return_విలువ () { తిరిగి అబద్ధం; }
  తార్కిక stop_sublookup_iteration (స్థిరరాశి return_t r) స్థిరరాశి { తిరిగి !r; }

  తార్కిక సందర్శించు_subtables (గుర్తులేని లెక్క)
  {
    max_subtables += లెక్క;
    తిరిగి max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  అంతరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (ఇది, std::forward<Ts> (ds)...) )
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  _dispatch (స్థిరరాశి T &obj, hb_ప్రాధాన్యత<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (ఇది, std::forward<Ts> (ds)...) )
  బహిరంగ:
  మూస <రకం_పేరు T, రకం_పేరు ...Ts> స్వయంచాలక
  dispatch (స్థిరరాశి T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    ఒకవేళ (blob)
      ప్రారంభం_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    ఒకవేళ (blob)
      చివర_processing ();
  }

  శూన్య init (hb_blob_t *b)
  {
    ఇది->blob = hb_blob_reference (b);
    ఇది->writable = అబద్ధం;
  }

  శూన్య సమితి_అంకె_glyphs (గుర్తులేని పూర్ణాంకం అంకె_glyphs_2)
  {
    అంకె_glyphs = అంకె_glyphs_2;
    అంకె_glyphs_సమితి = నిజం;
  }
  గుర్తులేని పూర్ణాంకం get_అంకె_glyphs () { తిరిగి అంకె_glyphs; }

  శూన్య సమితి_max_ops (పూర్ణాంకం max_ops_) { max_ops = max_ops_; }

  మూస <రకం_పేరు T>
  శూన్య సమితి_object (స్థిరరాశి T *obj)
  {
    reset_object ();

    ఒకవేళ (!obj) తిరిగి;

    స్థిరరాశి అక్షరం *obj_ప్రారంభం = (స్థిరరాశి అక్షరం *) obj;
    ఒకవేళ (unlikely (obj_ప్రారంభం < ఇది->ప్రారంభం || ఇది->చివర <= obj_ప్రారంభం))
    {
      ఇది->ప్రారంభం = ఇది->చివర = శూన్యసూచిక;
      ఇది->నిడివి = 0;
    }
    లేకపోతే
    {
      ఇది->ప్రారంభం = obj_ప్రారంభం;
      ఇది->చివర   = obj_ప్రారంభం + hb_min (size_t (ఇది->చివర - obj_ప్రారంభం), obj->get_కొలత ());
      ఇది->నిడివి = ఇది->చివర - ఇది->ప్రారంభం;
    }
  }

  శూన్య reset_object ()
  {
    ఒకవేళ (ఇది->blob)
    {
      ఇది->ప్రారంభం = ఇది->blob->దత్తాంశం;
      ఇది->చివర = ఇది->ప్రారంభం + ఇది->blob->నిడివి;
    }
    ఇది->నిడివి = ఇది->చివర - ఇది->ప్రారంభం;
    ప్రతిజ్ఞా (ఇది->ప్రారంభం <= ఇది->చివర); /* Must not overflow. */
  }

  శూన్య ప్రారంభం_processing (స్థిరరాశి అక్షరం *ప్రారంభం_2 = శూన్యసూచిక, స్థిరరాశి అక్షరం *చివర_4 = శూన్యసూచిక)
  {
    ఒకవేళ (ప్రారంభం_2)
    {
      ఇది->ప్రారంభం = ప్రారంభం_2;
      ఇది->చివర = చివర_4;
    }
    reset_object ();
    గుర్తులేని m;
    ఒకవేళ (unlikely (hb_unsigned_mul_overflows (ఇది->చివర - ఇది->ప్రారంభం, HB_SANITIZE_MAX_OPS_గుణకం, &m)))
      ఇది->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    లేకపోతే
      ఇది->max_ops = hb_clamp (m,
				(గుర్తులేని) HB_SANITIZE_MAX_OPS_MIN,
				(గుర్తులేని) HB_SANITIZE_MAX_OPS_MAX);
    ఇది->debug_లోతు = 0;
    ఇది->recursion_లోతు = 0;

    DEBUG_సందేశం_LEVEL (SANITIZE, ప్రారంభం, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ఇది->ప్రారంభం, ఇది->చివర,
		     (గుర్తులేని దీర్ఘ) (ఇది->చివర - ఇది->ప్రారంభం));
  }

  శూన్య చివర_processing ()
  {
    DEBUG_సందేశం_LEVEL (SANITIZE, ఇది->ప్రారంభం, 0, -1,
		     "end [%p..%p]",
		     ఇది->ప్రారంభం, ఇది->చివర);

    hb_blob_destroy (ఇది->blob);
    ఇది->blob = శూన్యసూచిక;
    ఇది->ప్రారంభం = ఇది->చివర = శూన్యసూచిక;
    ఇది->నిడివి = 0;
  }

  తార్కిక check_ops(గుర్తులేని లెక్క)
  {
    /* Avoid underflow */
    ఒకవేళ (unlikely (ఇది->max_ops < 0 || లెక్క >= (గుర్తులేని) ఇది->max_ops))
    {
      ఇది->max_ops = -1;
      తిరిగి అబద్ధం;
    }
    ఇది->max_ops -= (పూర్ణాంకం) లెక్క;
    తిరిగి నిజం;
  }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  తార్కిక check_range (స్థిరరాశి శూన్య *పునాది,
		    గుర్తులేని పూర్ణాంకం len) స్థిరరాశి
  {
    స్థిరరాశి అక్షరం *p = (స్థిరరాశి అక్షరం *) పునాది;
    తార్కిక ok = (uintptr_t) (p - ఇది->ప్రారంభం) <= ఇది->నిడివి &&
	      (గుర్తులేని పూర్ణాంకం) (ఇది->చివర - p) >= len &&
	      ((ఇది->max_ops -= len) > 0);

    DEBUG_సందేశం_LEVEL (SANITIZE, p, ఇది->debug_లోతు+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ఇది->ప్రారంభం, ఇది->చివర,
		     ok ? "OK" : "OUT-OF-RANGE");

    తిరిగి likely (ok);
  }
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  తార్కిక check_range_fast (స్థిరరాశి శూన్య *పునాది,
			 గుర్తులేని పూర్ణాంకం len) స్థిరరాశి
  {
    స్థిరరాశి అక్షరం *p = (స్థిరరాశి అక్షరం *) పునాది;
    తార్కిక ok = ((uintptr_t) (p - ఇది->ప్రారంభం) <= ఇది->నిడివి &&
	       (గుర్తులేని పూర్ణాంకం) (ఇది->చివర - p) >= len);

    DEBUG_సందేశం_LEVEL (SANITIZE, p, ఇది->debug_లోతు+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ఇది->ప్రారంభం, ఇది->చివర,
		     ok ? "OK" : "OUT-OF-RANGE");

    తిరిగి likely (ok);
  }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  తార్కిక check_బిందువు (స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    స్థిరరాశి అక్షరం *p = (స్థిరరాశి అక్షరం *) పునాది;
    తార్కిక ok = (uintptr_t) (p - ఇది->ప్రారంభం) <= ఇది->నిడివి;

    DEBUG_సందేశం_LEVEL (SANITIZE, p, ఇది->debug_లోతు+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     ఇది->ప్రారంభం, ఇది->చివర,
		     ok ? "OK" : "OUT-OF-RANGE");

    తిరిగి likely (ok);
  }

  మూస <రకం_పేరు T>
  తార్కిక check_range (స్థిరరాశి T *పునాది,
		    గుర్తులేని పూర్ణాంకం a,
		    గుర్తులేని పూర్ణాంకం b) స్థిరరాశి
  {
    గుర్తులేని m;
    తిరిగి !hb_unsigned_mul_overflows (a, b, &m) &&
	   ఇది->check_range (పునాది, m);
  }

  మూస <రకం_పేరు T>
  తార్కిక check_range (స్థిరరాశి T *పునాది,
		    గుర్తులేని పూర్ణాంకం a,
		    గుర్తులేని పూర్ణాంకం b,
		    గుర్తులేని పూర్ణాంకం c) స్థిరరాశి
  {
    గుర్తులేని m;
    తిరిగి !hb_unsigned_mul_overflows (a, b, &m) &&
	   ఇది->check_range (పునాది, m, c);
  }

  మూస <రకం_పేరు T>
  HB_ALWAYS_INLINE
  తార్కిక check_array_sized (స్థిరరాశి T *పునాది, గుర్తులేని పూర్ణాంకం len, గుర్తులేని len_కొలత) స్థిరరాశి
  {
    ఒకవేళ (len_కొలత >= 4)
    {
      ఒకవేళ (unlikely (hb_unsigned_mul_overflows (len, hb_static_కొలత (T), &len)))
	తిరిగి అబద్ధం;
    }
    లేకపోతే
      len = len * hb_static_కొలత (T);
    తిరిగి ఇది->check_range (పునాది, len);
  }

  మూస <రకం_పేరు T>
  తార్కిక check_array (స్థిరరాశి T *పునాది, గుర్తులేని పూర్ణాంకం len) స్థిరరాశి
  {
    తిరిగి ఇది->check_range (పునాది, len, hb_static_కొలత (T));
  }

  మూస <రకం_పేరు T>
  తార్కిక check_array (స్థిరరాశి T *పునాది,
		    గుర్తులేని పూర్ణాంకం a,
		    గుర్తులేని పూర్ణాంకం b) స్థిరరాశి
  {
    తిరిగి ఇది->check_range (పునాది, hb_static_కొలత (T), a, b);
  }

  తార్కిక check_ప్రారంభం_recursion (పూర్ణాంకం max_లోతు)
  {
    ఒకవేళ (unlikely (recursion_లోతు >= max_లోతు)) తిరిగి అబద్ధం;
    తిరిగి ++recursion_లోతు;
  }

  తార్కిక చివర_recursion (తార్కిక ఫలితం)
  {
    recursion_లోతు--;
    తిరిగి ఫలితం;
  }

  మూస <రకం_పేరు Type>
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  తార్కిక check_struct (స్థిరరాశి Type *obj) స్థిరరాశి
  {
    ఒకవేళ (పరిమాణం (uintptr_t) == పరిమాణం (uint32_t))
      తిరిగి likely (ఇది->check_range_fast (obj, obj->min_కొలత));
    లేకపోతే
      తిరిగి likely (ఇది->check_బిందువు ((స్థిరరాశి అక్షరం *) obj + obj->min_కొలత));
  }

  మూస <రకం_పేరు Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    తార్కిక sane;

    init (blob);

    DEBUG_సందేశం_FUNC (SANITIZE, ప్రారంభం, "start");

    ప్రారంభం_processing ();

    ఒకవేళ (unlikely (!ప్రారంభం))
    {
      చివర_processing ();
      తిరిగి blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<అక్షరం *> (ప్రారంభం));

    sane = t->sanitize (ఇది);

    చివర_processing ();

    DEBUG_సందేశం_FUNC (SANITIZE, ప్రారంభం, sane ? "PASSED" : "FAILED");
    ఒకవేళ (sane)
    {
      hb_blob_make_immutable (blob);
      తిరిగి blob;
    }
    లేకపోతే
    {
      hb_blob_destroy (blob);
      తిరిగి hb_blob_get_ఖాళీలు ();
    }
  }

  మూస <రకం_పేరు Type>
  hb_blob_t *reference_పట్టిక (స్థిరరాశి hb_face_t *face, hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = Type::పట్టిక_ట్యాగ్)
  {
    ఒకవేళ (!అంకె_glyphs_సమితి)
      సమితి_అంకె_glyphs (hb_face_get_glyph_లెక్క (face));
    తిరిగి sanitize_blob<Type> (hb_face_reference_పట్టిక (face, పట్టిక_ట్యాగ్));
  }

  స్థిరరాశి అక్షరం *ప్రారంభం, *చివర;
  గుర్తులేని నిడివి;
  మార్చదగిన పూర్ణాంకం max_ops, max_subtables;
  అంతరంగ:
  పూర్ణాంకం recursion_లోతు;
  తార్కిక writable;
  hb_blob_t *blob;
  గుర్తులేని పూర్ణాంకం అంకె_glyphs;
  తార్కిక  అంకె_glyphs_సమితి;
  బహిరంగ:
  తార్కిక lazy_some_gpos;
};

నిర్మాణం hb_sanitize_with_object_t
{
  మూస <రకం_పేరు T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, స్థిరరాశి T& obj) : c (c)
  { c->సమితి_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  అంతరంగ:
  hb_sanitize_context_t *c;
};


#అయితే_ముగింపు /* HB_SANITIZE_HH */
