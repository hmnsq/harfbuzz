/*
 * Copyright © 2016  Google, Inc.
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
 * Google Author(s): Seigo Nonaka, Calder Kitagawa
 */

#اگر_تعریف_نشده OT_COLOR_CBDT_CBDT_HH
#تعریف_کن OT_COLOR_CBDT_CBDT_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-paint.hh"

/*
 * CBLC -- Color Bitmap Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cblc
 * https://docs.microsoft.com/en-us/typography/opentype/spec/eblc
 * CBDT -- Color Bitmap Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cbdt
 * https://docs.microsoft.com/en-us/typography/opentype/spec/ebdt
 */
#تعریف_کن HB_OT_برچسب_CBLC HB_برچسب('C','B','L','C')
#تعریف_کن HB_OT_برچسب_CBDT HB_برچسب('C','B','D','T')


نطاق OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
بنية cbdt_dedup_context_t
{
  بنية مدخل_t
  {
    بیعلامت صحيح پایه;		/* Offset of the data in cbdt_prime. */
    بیعلامت صحيح lstart;	/* Start index in lengths. */
    بیعلامت صحيح lcount;	/* Number of length entries. */
  };

  ساكن uint64_t hash (بیعلامت صحيح image_format,
			ثابت hb_وکتور_t<حرف> &داده,
			ثابت hb_وکتور_t<بیعلامت صحيح> &lengths)
  {
    uint32_t h = hb_hash (hb_بایتها_t (داده.arrayZ, داده.درازی));
    h = h * 31 + hb_hash (hb_بایتها_t ((ثابت حرف *) lengths.arrayZ,
				      lengths.درازی * حجم (بیعلامت صحيح)));
    أرجع ((uint64_t) image_format << 32) | h;
  }

  منطقي find (uint64_t key,
	     ثابت hb_وکتور_t<حرف> &داده,
	     ثابت hb_وکتور_t<بیعلامت صحيح> &lengths,
	     ثابت hb_وکتور_t<حرف> &cbdt_اول,
	     بیعلامت صحيح *پایه /*بیرون*/) ثابت
  {
    بیعلامت صحيح *idx_p = اشاره_تهی;
    إذا (!map.has (key, &idx_p)) أرجع خطأ;
    ثابت مدخل_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    إذا (e.lcount != lengths.درازی) أرجع خطأ;
    إذا (unlikely (e.پایه + داده.درازی > (بیعلامت صحيح) cbdt_اول.درازی)) أرجع خطأ;
    إذا (hb_memcmp (cbdt_اول.arrayZ + e.پایه, داده.arrayZ, داده.درازی)) أرجع خطأ;
    إذا (hb_memcmp (هذا->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.درازی * حجم (بیعلامت صحيح)))
      أرجع خطأ;
    *پایه = e.پایه;
    أرجع درست;
  }

  منطقي جمع_کن (uint64_t key,
	    بیعلامت صحيح پایه,
	    ثابت hb_وکتور_t<بیعلامت صحيح> &lengths)
  {
    إذا (unlikely (entries.درون_error () || هذا->lengths.درون_error () || map.درون_error ()))
      أرجع خطأ;
    مدخل_t e = {پایه, هذا->lengths.درازی, lengths.درازی};
    إذا (unlikely (!entries.افزودن_or_fail (e))) أرجع خطأ;
    لكل (بیعلامت صحيح درازی : lengths)
      إذا (unlikely (!هذا->lengths.افزودن_or_fail (درازی))) أرجع خطأ;
    أرجع map.مجموعه (key, entries.درازی - 1);
  }

  hb_hashmap_t<uint64_t, بیعلامت صحيح> map;
  hb_وکتور_t<مدخل_t> entries;
  hb_وکتور_t<بیعلامت صحيح> lengths; /* Concatenated per-entry lengths. */
};

بنية cblc_bitmap_اندازه_subset_context_t
{
  ثابت حرف *cbdt;
  بیعلامت صحيح cbdt_درازی;
  hb_وکتور_t<حرف> *cbdt_اول;
  بیعلامت صحيح اندازه;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  بیعلامت صحيح رقم_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t شروع_glyph;	/*بیرون*/
  hb_codepoint_t انتها_glyph;	/*بیرون*/
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ساكن مضمن منطقي
copy_داده_مقصد_cbdt (hb_وکتور_t<حرف> *cbdt_اول,
		    ثابت فراغ        *داده,
		    بیعلامت           درازی)
{
  بیعلامت صحيح new_len = cbdt_اول->درازی + درازی;
  إذا (unlikely (!cbdt_اول->alloc (new_len))) أرجع خطأ;
  hb_memcpy (cbdt_اول->arrayZ + cbdt_اول->درازی, داده, درازی);
  cbdt_اول->درازی = new_len;
  أرجع درست;
}

بنية SmallGlyphMetrics
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقي scale) ثابت
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->عرض = عرض;
    extents->ارتفاع = -static_cast<صحيح> (ارتفاع);

    إذا (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	ارتفاع;
  HBUINT8	عرض;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  عام:
  DEFINE_اندازه_STATIC (5);
};

بنية BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية SBit_سطر_Metrics
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBINT8	ascender;
  HBINT8	decender;
  HBUINT8	عرض_Max;
  HBINT8	caret_شیب_Numerator;
  HBINT8	caret_شیب_Denominator;
  HBINT8	caret_جابه_جایی;
  HBINT8	minOriginSB;
  HBINT8	minAdvanceSB;
  HBINT8	maxBeforeBL;
  HBINT8	minAfterBL;
  HBINT8	padding1;
  HBINT8	padding2;
  عام:
  DEFINE_اندازه_STATIC (12);
};


/*
 * Index Subtables.
 */

بنية شاخص_Subtable_سرآیند
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	شاخص_Format;
  HBUINT16	imageFormat;
  HBUINT32	image_داده_جابه_جایی;
  عام:
  DEFINE_اندازه_STATIC (8);
};

قالب <نام_نوع جابه_جایی_Type>
بنية شاخص_Subtable_Format1_Or3
{
  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح glyph_شمار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  جابه_جایی_Array_Z.sanitize (c, glyph_شمار + 1));
  }

  منطقي get_image_داده (بیعلامت صحيح idx,
		       بیعلامت صحيح *جابه_جایی,
		       بیعلامت صحيح *درازی) ثابت
  {
    إذا (unlikely (جابه_جایی_Array_Z[idx + 1] <= جابه_جایی_Array_Z[idx]))
      أرجع خطأ;

    *جابه_جایی = سرآیند.image_داده_جابه_جایی + جابه_جایی_Array_Z[idx];
    *درازی = جابه_جایی_Array_Z[idx + 1] - جابه_جایی_Array_Z[idx];
    أرجع درست;
  }

  منطقي جمع_کن_جابه_جایی (hb_serialize_context_t *c,
		   بیعلامت صحيح جابه_جایی,
		   بیعلامت صحيح *اندازه /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    جابه_جایی_2<جابه_جایی_Type> embedded_جابه_جایی;
    embedded_جابه_جایی = جابه_جایی;
    *اندازه += حجم (جابه_جایی_Type);
    خودکار *o = c->embed (embedded_جابه_جایی);
    return_trace ((منطقي) o);
  }

  شاخص_Subtable_سرآیند	سرآیند;
  Unsized_Array_از<جابه_جایی_2<جابه_جایی_Type>>
			جابه_جایی_Array_Z;
  عام:
  DEFINE_اندازه_ARRAY (8, جابه_جایی_Array_Z);
};

بنية شاخص_Subtable_Format1 : شاخص_Subtable_Format1_Or3<HBUINT32> {};
بنية شاخص_Subtable_Format3 : شاخص_Subtable_Format1_Or3<HBUINT16> {};

بنية شاخص_Subtable
{
  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح glyph_شمار) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.سرآیند.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.سرآیند.شاخص_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_شمار));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_شمار));
    افتراضي:return_trace (درست);
    }
  }

  منطقي
  پایان_subtable (hb_serialize_context_t *c,
		   بیعلامت صحيح انتها_جابه_جایی,
		   بیعلامت صحيح رقم_glyphs,
		   بیعلامت صحيح *اندازه /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);

    بدل (u.سرآیند.شاخص_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.جمع_کن_جابه_جایی (c, انتها_جابه_جایی, اندازه));
    حالة 3: {
      hb_barrier ();
      إذا (!u.format3.جمع_کن_جابه_جایی (c, انتها_جابه_جایی, اندازه))
	return_trace (خطأ);
      إذا (!(رقم_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.جمع_کن_جابه_جایی (c, 0, اندازه));
      return_trace (درست);
    }
    // TODO: implement 2, 4, 5.
    حالة 2: حالة 4:  // No-op.
    حالة 5:  // Pad to 32-bit aligned.
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي
  جمع_کن_جابه_جایی (hb_serialize_context_t *c, بیعلامت صحيح local_جابه_جایی,
	      بیعلامت صحيح *اندازه /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.سرآیند.شاخص_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.جمع_کن_جابه_جایی (c, local_جابه_جایی, اندازه));
    حالة 3: hb_barrier (); return_trace (u.format3.جمع_کن_جابه_جایی (c, local_جابه_جایی, اندازه));
    // TODO: Implement tables 2, 4, 5
    حالة 2:  // Should be a no-op.
    حالة 4: حالة 5:  // Handle sparse cases.
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents HB_استفادهنشده, منطقي scale HB_استفادهنشده) ثابت
  {
    بدل (u.سرآیند.شاخص_Format)
    {
    حالة 2: حالة 5: /* TODO */
    حالة 1: حالة 3: حالة 4: /* Variable-metrics formats do not have metrics here. */
    افتراضي:أرجع (خطأ);
    }
  }

  منطقي
  get_image_داده (بیعلامت صحيح idx, بیعلامت صحيح *جابه_جایی,
		  بیعلامت صحيح *درازی, بیعلامت صحيح *format) ثابت
  {
    *format = u.سرآیند.imageFormat;
    بدل (u.سرآیند.شاخص_Format)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_image_داده (idx, جابه_جایی, درازی);
    حالة 3: hb_barrier (); أرجع u.format3.get_image_داده (idx, جابه_جایی, درازی);
    افتراضي: أرجع خطأ;
    }
  }

  ثابت شاخص_Subtable_سرآیند* get_سرآیند () ثابت { أرجع &u.سرآیند; }

  فراغ populate_سرآیند (بیعلامت شاخص_format,
			بیعلامت image_format,
			بیعلامت صحيح image_داده_جابه_جایی_2,
			بیعلامت صحيح *اندازه)
  {
    u.سرآیند.شاخص_Format = شاخص_format;
    u.سرآیند.imageFormat = image_format;
    u.سرآیند.image_داده_جابه_جایی = image_داده_جابه_جایی_2;
    بدل (u.سرآیند.شاخص_Format)
    {
    حالة 1: *اندازه += شاخص_Subtable_Format1::min_اندازه; اكسر;
    حالة 3: *اندازه += شاخص_Subtable_Format3::min_اندازه; اكسر;
    }
  }

  محمي:
  اتحاد {
  شاخص_Subtable_سرآیند	سرآیند;
  شاخص_Subtable_Format1	format1;
  شاخص_Subtable_Format3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  عام:
  DEFINE_اندازه_UNION (8, سرآیند);
};

بنية شاخص_Subtable_رکورد
{
  /* XXX Remove this and fix by not inserting it into vector. */
  شاخص_Subtable_رکورد& عملگر = (ثابت شاخص_Subtable_رکورد &o)
  {
    اولی_Glyph_شاخص = o.اولی_Glyph_شاخص;
    آخرین_Glyph_شاخص = o.آخرین_Glyph_شاخص;
    جابه_جایی_مقصد_Subtable = (بیعلامت) o.جابه_جایی_مقصد_Subtable;
    أكد (جابه_جایی_مقصد_Subtable.is_null ());
    أرجع *هذا;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  اولی_Glyph_شاخص <= آخرین_Glyph_شاخص &&
		  جابه_جایی_مقصد_Subtable.sanitize (c, پایه, آخرین_Glyph_شاخص - اولی_Glyph_شاخص + 1));
  }

  ثابت شاخص_Subtable* get_subtable (ثابت فراغ *پایه) ثابت
  {
    أرجع &(پایه+جابه_جایی_مقصد_Subtable);
  }

  منطقي جمع_کن_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_اندازه_subset_context_t *bitmap_اندازه_context,
			 شاخص_Subtable_رکورد *رکورد,
			 ثابت hb_وکتور_t<hb_pair_t<hb_codepoint_t, ثابت شاخص_Subtable_رکورد*>> *lookup, /*درون*/
			 ثابت فراغ *پایه,
			 بیعلامت صحيح *شروع /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خودکار *subtable = c->serializer->شروع_embed<شاخص_Subtable> ();
    إذا (unlikely (!c->serializer->extend_min (subtable))) return_trace (خطأ);

    خودکار *old_subtable = get_subtable (پایه);
    خودکار *old_سرآیند = old_subtable->get_سرآیند ();
    بیعلامت صحيح image_format = old_سرآیند->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_وکتور_t<حرف> داده;
    hb_وکتور_t<بیعلامت صحيح> lengths;
    بیعلامت صحيح رقم_glyphs = 0;
    منطقي early_exit = خطأ;
    لكل (بیعلامت صحيح i = *شروع; i < lookup->درازی; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].اولی;
      ثابت شاخص_Subtable_رکورد *بعدی_رکورد = (*lookup)[i].second;
      ثابت شاخص_Subtable *بعدی_subtable = بعدی_رکورد->get_subtable (پایه);
      خودکار *بعدی_سرآیند = بعدی_subtable->get_سرآیند ();
      إذا (بعدی_سرآیند != old_سرآیند)
      {
	*شروع = i;
	early_exit = درست;
	اكسر;
      }
      بیعلامت صحيح رقم_missing = رکورد->جمع_کن_glyph_for_subset (new_gid);
      لكل (بیعلامت صحيح j = 0; j < رقم_missing; j++)
	إذا (unlikely (!c->serializer->check_success (lengths.افزودن_or_fail (0))))
	  return_trace (خطأ);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      إذا (old_gid < بعدی_رکورد->اولی_Glyph_شاخص)
	return_trace (خطأ);

      بیعلامت صحيح old_idx = (بیعلامت صحيح) old_gid - بعدی_رکورد->اولی_Glyph_شاخص;
      بیعلامت صحيح جابه_جایی, درازی, format;
      إذا (unlikely (!بعدی_subtable->get_image_داده (old_idx, &جابه_جایی, &درازی, &format)))
	return_trace (خطأ);
      إذا (unlikely (جابه_جایی > bitmap_اندازه_context->cbdt_درازی ||
		    bitmap_اندازه_context->cbdt_درازی - جابه_جایی < درازی))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (
			copy_داده_مقصد_cbdt (&داده,
					    bitmap_اندازه_context->cbdt + جابه_جایی,
					    درازی))))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (lengths.افزودن_or_fail (درازی))))
	return_trace (خطأ);
      رقم_glyphs += 1 + رقم_missing;
    }
    إذا (!early_exit)
      *شروع = lookup->درازی;

    /* Find or append the data block for this subtable. */
    بیعلامت صحيح image_داده_جابه_جایی_2;
    cbdt_dedup_context_t *dedup = bitmap_اندازه_context->dedup;
    منطقي shared = خطأ;
    uint64_t key = 0;
    إذا (dedup && داده.درازی)
    {
      key = cbdt_dedup_context_t::hash (image_format, داده, lengths);
      shared = dedup->find (key, داده, lengths,
			    *bitmap_اندازه_context->cbdt_اول,
			    &image_داده_جابه_جایی_2);
    }
    إذا (!shared)
    {
      image_داده_جابه_جایی_2 = bitmap_اندازه_context->cbdt_اول->درازی;
      إذا (unlikely (!c->serializer->check_success (
			copy_داده_مقصد_cbdt (bitmap_اندازه_context->cbdt_اول,
					    داده.arrayZ, داده.درازی))))
	return_trace (خطأ);
      إذا (dedup && داده.درازی)
	/* The bitmap data is already copied; caching is best-effort. */
	(فراغ) dedup->جمع_کن (key, image_داده_جابه_جایی_2, lengths);
    }

    subtable->populate_سرآیند (old_سرآیند->شاخص_Format,
			       image_format,
			       image_داده_جابه_جایی_2,
			       &bitmap_اندازه_context->اندازه);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    بیعلامت صحيح cumulative = 0;
    لكل (بیعلامت صحيح درازی : lengths)
    {
      إذا (unlikely (!subtable->جمع_کن_جابه_جایی (c->serializer, cumulative,
					   &bitmap_اندازه_context->اندازه)))
	return_trace (خطأ);
      cumulative += درازی;
    }
    إذا (unlikely (!subtable->پایان_subtable (c->serializer,
					      cumulative,
					      رقم_glyphs,
					      &bitmap_اندازه_context->اندازه)))
      return_trace (خطأ);
    return_trace (درست);
  }

  منطقي جمع_کن_new_رکورد (hb_subset_context_t *c,
		       cblc_bitmap_اندازه_subset_context_t *bitmap_اندازه_context,
		       ثابت hb_وکتور_t<hb_pair_t<hb_codepoint_t, ثابت شاخص_Subtable_رکورد*>> *lookup, /*درون*/
		       ثابت فراغ *پایه,
		       بیعلامت صحيح *شروع, /* INOUT */
		       hb_وکتور_t<شاخص_Subtable_رکورد>* رکوردها /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار snap = c->serializer->snapshot ();
    بیعلامت صحيح old_اندازه = bitmap_اندازه_context->اندازه;
    بیعلامت صحيح old_cbdt_اول_درازی = bitmap_اندازه_context->cbdt_اول->درازی;

    // Set to invalid state to indicate filling glyphs is not yet started.
    إذا (unlikely (!c->serializer->check_success (رکوردها->resize (رکوردها->درازی + 1))))
      return_trace (خطأ);

    رکوردها->دم ().اولی_Glyph_شاخص = 1;
    رکوردها->دم ().آخرین_Glyph_شاخص = 0;
    bitmap_اندازه_context->اندازه += شاخص_Subtable_رکورد::min_اندازه;

    c->serializer->push ();

    إذا (unlikely (!جمع_کن_new_subtable (c, bitmap_اندازه_context, &(رکوردها->دم ()), lookup, پایه, شروع)))
    {
      c->serializer->برداشت_discard ();
      c->serializer->revert (snap);
      bitmap_اندازه_context->cbdt_اول->shrink (old_cbdt_اول_درازی);
      bitmap_اندازه_context->اندازه = old_اندازه;
      رکوردها->resize (رکوردها->درازی - 1);
      return_trace (خطأ);
    }

    bitmap_اندازه_context->رقم_tables += 1;
    return_trace (درست);
  }

  بیعلامت صحيح جمع_کن_glyph_for_subset (hb_codepoint_t gid)
  {
    إذا (اولی_Glyph_شاخص > آخرین_Glyph_شاخص)
    {
      اولی_Glyph_شاخص = gid;
      آخرین_Glyph_شاخص = gid;
      أرجع 0;
    }
    // TODO maybe assert? this shouldn't occur.
    إذا (آخرین_Glyph_شاخص > gid)
      أرجع 0;
    بیعلامت صحيح رقم_missing = (بیعلامت صحيح) (gid - آخرین_Glyph_شاخص - 1);
    آخرین_Glyph_شاخص = gid;
    أرجع رقم_missing;
  }

  منطقي get_extents (hb_glyph_extents_t *extents, ثابت فراغ *پایه, منطقي scale) ثابت
  { أرجع (پایه+جابه_جایی_مقصد_Subtable).get_extents (extents, scale); }

  منطقي get_image_داده (بیعلامت صحيح  gid,
		       ثابت فراغ   *پایه,
		       بیعلامت صحيح *جابه_جایی,
		       بیعلامت صحيح *درازی,
		       بیعلامت صحيح *format) ثابت
  {
    إذا (gid < اولی_Glyph_شاخص || gid > آخرین_Glyph_شاخص) أرجع خطأ;
    أرجع (پایه+جابه_جایی_مقصد_Subtable).get_image_داده (gid - اولی_Glyph_شاخص,
						   جابه_جایی, درازی, format);
  }

  HBGlyphID16			اولی_Glyph_شاخص;
  HBGlyphID16			آخرین_Glyph_شاخص;
  Offset32_مقصد<شاخص_Subtable>	جابه_جایی_مقصد_Subtable;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية شاخص_Subtable_Array
{
  دوست بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح شمار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (شاخص_Subtables_Z.sanitize (c, شمار, هذا));
  }

  فراغ
  build_lookup (hb_subset_context_t *c, cblc_bitmap_اندازه_subset_context_t *bitmap_اندازه_context,
		hb_وکتور_t<hb_pair_t<hb_codepoint_t,
		ثابت شاخص_Subtable_رکورد*>> *lookup /*بیرون*/) ثابت
  {
    منطقي شروع_glyph_is_مجموعه = خطأ;
    بیعلامت رقم_glyphs = c->plan->رقم_output_glyphs ();
    لكل (hb_codepoint_t new_gid = 0; new_gid < رقم_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) استمر;

      ثابت شاخص_Subtable_رکورد* رکورد = پیدا_کن_جدول (old_gid, bitmap_اندازه_context->رقم_tables);
      إذا (unlikely (!رکورد)) استمر;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      بیعلامت صحيح جابه_جایی, درازی, format;
      إذا (unlikely (!رکورد->get_image_داده (old_gid, هذا, &جابه_جایی, &درازی, &format))) استمر;

      lookup->push (hb_pair_t<hb_codepoint_t, ثابت شاخص_Subtable_رکورد*> (new_gid, رکورد));

      إذا (!شروع_glyph_is_مجموعه)
      {
	bitmap_اندازه_context->شروع_glyph = new_gid;
	شروع_glyph_is_مجموعه = درست;
      }

      bitmap_اندازه_context->انتها_glyph = new_gid;
    }
  }

  منطقي
  subset (hb_subset_context_t *c,
	  cblc_bitmap_اندازه_subset_context_t *bitmap_اندازه_context) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_وکتور_t<hb_pair_t<hb_codepoint_t, ثابت شاخص_Subtable_رکورد*>> lookup;
    build_lookup (c, bitmap_اندازه_context, &lookup);
    إذا (unlikely (!c->serializer->propagate_error (lookup)))
      أرجع خطأ;

    bitmap_اندازه_context->اندازه = 0;
    bitmap_اندازه_context->رقم_tables = 0;
    hb_وکتور_t<شاخص_Subtable_رکورد> رکوردها;
    لكل (بیعلامت صحيح شروع = 0; شروع < lookup.درازی;)
    {
      إذا (unlikely (!lookup[شروع].second->جمع_کن_new_رکورد (c, bitmap_اندازه_context, &lookup, هذا, &شروع, &رکوردها)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	لكل (بیعلامت صحيح i = 0; i < رکوردها.درازی; i++)
	  c->serializer->برداشت_discard ();
	return_trace (خطأ);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_وکتور_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (بیعلامت صحيح i = 0; i < رکوردها.درازی; i++)
      objidxs.push (c->serializer->برداشت_pack ());
    لكل (بیعلامت صحيح i = 0; i < رکوردها.درازی; i++)
    {
      شاخص_Subtable_رکورد* رکورد = c->serializer->embed (رکوردها[i]);
      إذا (unlikely (!رکورد)) return_trace (خطأ);
      c->serializer->جمع_کن_link (رکورد->جابه_جایی_مقصد_Subtable, objidxs[رکوردها.درازی - 1 - i]);
    }
    return_trace (درست);
  }

  عام:
  ثابت شاخص_Subtable_رکورد* پیدا_کن_جدول (hb_codepoint_t glyph, بیعلامت صحيح رقم_Tables) ثابت
  {
    لكل (بیعلامت صحيح i = 0; i < رقم_Tables; ++i)
    {
      بیعلامت صحيح اولی_Glyph_شاخص = شاخص_Subtables_Z[i].اولی_Glyph_شاخص;
      بیعلامت صحيح آخرین_Glyph_شاخص = شاخص_Subtables_Z[i].آخرین_Glyph_شاخص;
      إذا (اولی_Glyph_شاخص <= glyph && glyph <= آخرین_Glyph_شاخص)
	أرجع &شاخص_Subtables_Z[i];
    }
    أرجع اشاره_تهی;
  }

  محمي:
  Unsized_Array_از<شاخص_Subtable_رکورد>	شاخص_Subtables_Z;
};

بنية Bitmap_اندازه_جدول
{
  دوست بنية CBLC;
  دوست بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  شاخص_Subtable_Array_جابه_جایی.sanitize (c, پایه, عدد_از_شاخص_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ثابت شاخص_Subtable_رکورد *
  پیدا_کن_جدول (hb_codepoint_t glyph, ثابت فراغ *پایه, ثابت فراغ **بیرون_پایه) ثابت
  {
    *بیرون_پایه = &(پایه+شاخص_Subtable_Array_جابه_جایی);
    أرجع (پایه+شاخص_Subtable_Array_جابه_جایی).پیدا_کن_جدول (glyph, عدد_از_شاخص_Subtables);
  }

  منطقي
  subset (hb_subset_context_t *c, ثابت فراغ *پایه,
	  ثابت حرف *cbdt, بیعلامت صحيح cbdt_درازی,
	  hb_وکتور_t<حرف> *cbdt_اول /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون_جدول = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون_جدول)) return_trace (خطأ);

    cblc_bitmap_اندازه_subset_context_t bitmap_اندازه_context;
    bitmap_اندازه_context.cbdt = cbdt;
    bitmap_اندازه_context.cbdt_درازی = cbdt_درازی;
    bitmap_اندازه_context.cbdt_اول = cbdt_اول;
    bitmap_اندازه_context.اندازه = شاخص_Tables_اندازه;
    bitmap_اندازه_context.رقم_tables = عدد_از_شاخص_Subtables;
    bitmap_اندازه_context.شروع_glyph = 1;
    bitmap_اندازه_context.انتها_glyph = 0;
    bitmap_اندازه_context.dedup = dedup;

    إذا (!بیرون_جدول->شاخص_Subtable_Array_جابه_جایی.serialize_subset (c,
							       شاخص_Subtable_Array_جابه_جایی,
							       پایه,
							       &bitmap_اندازه_context))
      return_trace (خطأ);
    إذا (!bitmap_اندازه_context.اندازه ||
	!bitmap_اندازه_context.رقم_tables ||
	bitmap_اندازه_context.شروع_glyph > bitmap_اندازه_context.انتها_glyph)
      return_trace (خطأ);

    بیرون_جدول->شاخص_Tables_اندازه = bitmap_اندازه_context.اندازه;
    بیرون_جدول->عدد_از_شاخص_Subtables = bitmap_اندازه_context.رقم_tables;
    بیرون_جدول->شروع_Glyph_شاخص = bitmap_اندازه_context.شروع_glyph;
    بیرون_جدول->انتها_Glyph_شاخص = bitmap_اندازه_context.انتها_glyph;
    return_trace (درست);
  }

  محمي:
  NNOffset32_مقصد<شاخص_Subtable_Array>
			شاخص_Subtable_Array_جابه_جایی;
  HBUINT32		شاخص_Tables_اندازه;
  HBUINT32		عدد_از_شاخص_Subtables;
  HBUINT32		colorRef;
  SBit_سطر_Metrics	horizontal;
  SBit_سطر_Metrics	vertical;
  HBGlyphID16		شروع_Glyph_شاخص;
  HBGlyphID16		انتها_Glyph_شاخص;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		بیت_عمق;
  HBINT8		flags;
  عام:
  DEFINE_اندازه_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

بنية Glyph_Bitmap_داده_Format17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32_از<HBUINT8>	داده;
  عام:
  DEFINE_اندازه_ARRAY (9, داده);
};

بنية Glyph_Bitmap_داده_Format18
{
  BigGlyphMetrics	glyphMetrics;
  Array32_از<HBUINT8>	داده;
  عام:
  DEFINE_اندازه_ARRAY (12, داده);
};

بنية Glyph_Bitmap_داده_Format19
{
  Array32_از<HBUINT8>	داده;
  عام:
  DEFINE_اندازه_ARRAY (4, داده);
};

بنية CBLC
{
  دوست بنية CBDT;

  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_CBLC;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخه.major == 2 || نسخه.major == 3) &&
		  hb_barrier () &&
		  اندازه_Tables.sanitize (c, هذا));
  }

  ساكن منطقي
  sink_cbdt (hb_subset_context_t *c, hb_وکتور_t<حرف>* cbdt_اول)
  {
    hb_blob_t *cbdt_اول_blob = hb_blob_create (cbdt_اول->arrayZ,
						 cbdt_اول->درازی,
						 HB_حافظه_نما_WRITABLE,
						 cbdt_اول->arrayZ,
						 hb_free);
    cbdt_اول->init ();  // Leak arrayZ to the blob.
    منطقي ret = c->plan->جمع_کن_جدول (HB_OT_برچسب_CBDT, cbdt_اول_blob);
    hb_blob_destroy (cbdt_اول_blob);
    أرجع ret;
  }

  منطقي
  subset_اندازه_جدول (hb_subset_context_t *c, ثابت Bitmap_اندازه_جدول& جدول,
		     ثابت حرف *cbdt /*درون*/, بیعلامت صحيح cbdt_درازی,
		     CBLC *cblc_اول /* INOUT */, hb_وکتور_t<حرف> *cbdt_اول /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    cblc_اول->اندازه_Tables.len++;

    خودکار snap = c->serializer->snapshot ();
    خودکار cbdt_اول_len = cbdt_اول->درازی;

    إذا (!جدول.subset (c, هذا, cbdt, cbdt_درازی, cbdt_اول, dedup))
    {
      cblc_اول->اندازه_Tables.len--;
      c->serializer->revert (snap);
      cbdt_اول->shrink (cbdt_اول_len);
      return_trace (خطأ);
    }
    return_trace (درست);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_داخلی منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  ثابت Bitmap_اندازه_جدول &انتخاب_strike (hb_font_t *font) ثابت
  {
    بیعلامت شمار = اندازه_Tables.len;
    إذا (unlikely (!شمار))
      أرجع Null (Bitmap_اندازه_جدول);

    بیعلامت صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    إذا (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    بیعلامت صحيح بهترین_i = 0;
    بیعلامت صحيح بهترین_ppem = hb_max (اندازه_Tables[0].ppemX, اندازه_Tables[0].ppemY);

    لكل (بیعلامت صحيح i = 1; i < شمار; i++)
    {
      بیعلامت صحيح ppem = hb_max (اندازه_Tables[i].ppemX, اندازه_Tables[i].ppemY);
      إذا ((requested_ppem <= ppem && ppem < بهترین_ppem) ||
	  (requested_ppem > بهترین_ppem && ppem > بهترین_ppem))
      {
	بهترین_i = i;
	بهترین_ppem = ppem;
      }
    }

    أرجع اندازه_Tables[بهترین_i];
  }

  محمي:
  Fixed_نسخه<>		نسخه;
  Array32_از<Bitmap_اندازه_جدول>	اندازه_Tables;
  عام:
  DEFINE_اندازه_ARRAY (8, اندازه_Tables);
};

بنية CBDT
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_CBDT;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      هذا->cblc = hb_sanitize_context_t ().reference_جدول<CBLC> (face);
      هذا->cbdt = hb_sanitize_context_t ().reference_جدول<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      هذا->cblc.destroy ();
      هذا->cbdt.destroy ();
    }

    منطقي
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, منطقي scale = درست) ثابت
    {
      ثابت فراغ *پایه;
      ثابت Bitmap_اندازه_جدول &strike = هذا->cblc->انتخاب_strike (font);
      ثابت شاخص_Subtable_رکورد *subtable_رکورد = strike.پیدا_کن_جدول (glyph, cblc, &پایه);
      إذا (!subtable_رکورد || !strike.ppemX || !strike.ppemY)
	أرجع خطأ;

      إذا (subtable_رکورد->get_extents (extents, پایه, scale))
	أرجع درست;

      بیعلامت صحيح image_جابه_جایی = 0, image_درازی = 0, image_format = 0;
      إذا (!subtable_رکورد->get_image_داده (glyph, پایه, &image_جابه_جایی, &image_درازی, &image_format))
	أرجع خطأ;

      بیعلامت صحيح cbdt_len = cbdt.get_درازی ();
      إذا (unlikely (image_جابه_جایی > cbdt_len || cbdt_len - image_جابه_جایی < image_درازی))
	أرجع خطأ;

      بدل (image_format)
      {
      حالة 17: {
	إذا (unlikely (image_درازی < Glyph_Bitmap_داده_Format17::min_اندازه))
	  أرجع خطأ;
	خودکار &glyphFormat17 = Struct_At_جابه_جایی<Glyph_Bitmap_داده_Format17> (هذا->cbdt, image_جابه_جایی);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	اكسر;
      }
      حالة 18: {
	إذا (unlikely (image_درازی < Glyph_Bitmap_داده_Format18::min_اندازه))
	  أرجع خطأ;
	خودکار &glyphFormat18 = Struct_At_جابه_جایی<Glyph_Bitmap_داده_Format18> (هذا->cbdt, image_جابه_جایی);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	اكسر;
      }
      افتراضي: أرجع خطأ; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      إذا (scale)
      {
	عائم x_scale = upem / (عائم) strike.ppemX;
	عائم y_scale = upem / (عائم) strike.ppemY;
	extents->x_bearing = hb_clamp_مقصد<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_مقصد<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->عرض = hb_clamp_مقصد<hb_position_t> (roundf (extents->عرض * x_scale));
	extents->ارتفاع = hb_clamp_مقصد<hb_position_t> (roundf (extents->ارتفاع * y_scale));
      }

      أرجع درست;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ثابت
    {
      ثابت فراغ *پایه;
      ثابت Bitmap_اندازه_جدول &strike = هذا->cblc->انتخاب_strike (font);
      ثابت شاخص_Subtable_رکورد *subtable_رکورد = strike.پیدا_کن_جدول (glyph, cblc, &پایه);
      إذا (!subtable_رکورد || !strike.ppemX || !strike.ppemY)
	أرجع hb_blob_get_خالی ();

      بیعلامت صحيح image_جابه_جایی = 0, image_درازی = 0, image_format = 0;
      إذا (!subtable_رکورد->get_image_داده (glyph, پایه, &image_جابه_جایی, &image_درازی, &image_format))
	أرجع hb_blob_get_خالی ();

      بیعلامت صحيح cbdt_len = cbdt.get_درازی ();
      إذا (unlikely (image_جابه_جایی > cbdt_len || cbdt_len - image_جابه_جایی < image_درازی))
	أرجع hb_blob_get_خالی ();

      بدل (image_format)
      {
      حالة 17:
      {
	إذا (unlikely (image_درازی < Glyph_Bitmap_داده_Format17::min_اندازه))
	  أرجع hb_blob_get_خالی ();
	خودکار &glyphFormat17 = Struct_At_جابه_جایی<Glyph_Bitmap_داده_Format17> (هذا->cbdt, image_جابه_جایی);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_جابه_جایی + Glyph_Bitmap_داده_Format17::min_اندازه,
					glyphFormat17.داده.len);
      }
      حالة 18:
      {
	إذا (unlikely (image_درازی < Glyph_Bitmap_داده_Format18::min_اندازه))
	  أرجع hb_blob_get_خالی ();
	خودکار &glyphFormat18 = Struct_At_جابه_جایی<Glyph_Bitmap_داده_Format18> (هذا->cbdt, image_جابه_جایی);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_جابه_جایی + Glyph_Bitmap_داده_Format18::min_اندازه,
					glyphFormat18.داده.len);
      }
      حالة 19:
      {
	إذا (unlikely (image_درازی < Glyph_Bitmap_داده_Format19::min_اندازه))
	  أرجع hb_blob_get_خالی ();
	خودکار &glyphFormat19 = Struct_At_جابه_جایی<Glyph_Bitmap_داده_Format19> (هذا->cbdt, image_جابه_جایی);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_جابه_جایی + Glyph_Bitmap_داده_Format19::min_اندازه,
					glyphFormat19.داده.len);
      }
      افتراضي: أرجع hb_blob_get_خالی (); /* TODO: Support other image formats. */
      }
    }

    منطقي has_داده () ثابت { أرجع cbdt->نسخه.major; }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *داده) ثابت
    {
      إذا (!has_داده ()) أرجع خطأ;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      إذا (unlikely (!font->get_glyph_extents (glyph, &extents, خطأ)))
        أرجع خطأ;

      إذا (unlikely (!get_extents (font, glyph, &pixel_extents, خطأ)))
        أرجع خطأ;

      hb_blob_t *blob = reference_png (font, glyph);
      إذا (unlikely (hb_blob_is_immutable (blob)))
        أرجع خطأ;

      منطقي ret = funcs->image (داده,
			       blob,
			       pixel_extents.عرض, -pixel_extents.ارتفاع,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    بیعلامت صحيح upem;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخه.major == 2 || نسخه.major == 3));
  }

  محمي:
  Fixed_نسخه<>		نسخه;
  Unsized_Array_از<HBUINT8>	داده_Z;
  عام:
  DEFINE_اندازه_ARRAY (4, داده_Z);
};

مضمن منطقي
CBLC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_وکتور_t<حرف> cbdt_اول;

  خودکار *cblc_اول = c->serializer->شروع_embed<CBLC> ();
  إذا (unlikely (!c->serializer->extend_min (cblc_اول))) return_trace (خطأ);
  cblc_اول->نسخه = نسخه;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_جدول<CBDT> (c->plan->منبع);
  بیعلامت صحيح cbdt_درازی;
  CBDT* cbdt = (CBDT *) hb_blob_get_داده (cbdt_blob, &cbdt_درازی);
  إذا (unlikely (cbdt_درازی < CBDT::min_اندازه))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (خطأ);
  }
  copy_داده_مقصد_cbdt (&cbdt_اول, cbdt, CBDT::min_اندازه);

  cbdt_dedup_context_t dedup;
  لكل (ثابت Bitmap_اندازه_جدول& جدول : + اندازه_Tables.iter ())
    subset_اندازه_جدول (c, جدول, (ثابت حرف *) cbdt, cbdt_درازی, cblc_اول, &cbdt_اول, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_اول));
}

بنية CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#پایان_اگر /* OT_COLOR_CBDT_CBDT_HH */
