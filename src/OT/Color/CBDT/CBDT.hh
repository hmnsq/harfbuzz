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

#جيڪڏهن_وصف_نه_ٿيل OT_COLOR_CBDT_CBDT_HH
#وصف_ڪريو OT_COLOR_CBDT_CBDT_HH

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
#وصف_ڪريو HB_OT_ٽيگ_CBLC HB_ٽيگ('C','B','L','C')
#وصف_ڪريو HB_OT_ٽيگ_CBDT HB_ٽيگ('C','B','D','T')


نطاق OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
بنية cbdt_dedup_context_t
{
  بنية داخلا_t
  {
    بنانشان صحيح بنياد;		/* Offset of the data in cbdt_prime. */
    بنانشان صحيح lstart;	/* Start index in lengths. */
    بنانشان صحيح lcount;	/* Number of length entries. */
  };

  ساكن uint64_t hash (بنانشان صحيح image_format,
			ثابت hb_ويڪٽر_t<حرف> &معلومات,
			ثابت hb_ويڪٽر_t<بنانشان صحيح> &lengths)
  {
    uint32_t h = hb_hash (hb_بائيٽون_t (معلومات.arrayZ, معلومات.ڊيگهائي));
    h = h * 31 + hb_hash (hb_بائيٽون_t ((ثابت حرف *) lengths.arrayZ,
				      lengths.ڊيگهائي * حجم (بنانشان صحيح)));
    أرجع ((uint64_t) image_format << 32) | h;
  }

  منطقي find (uint64_t key,
	     ثابت hb_ويڪٽر_t<حرف> &معلومات,
	     ثابت hb_ويڪٽر_t<بنانشان صحيح> &lengths,
	     ثابت hb_ويڪٽر_t<حرف> &cbdt_مفرد,
	     بنانشان صحيح *بنياد /*ٻاهريون*/) ثابت
  {
    بنانشان صحيح *idx_p = خالي_اشارو;
    إذا (!map.has (key, &idx_p)) أرجع خطأ;
    ثابت داخلا_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    إذا (e.lcount != lengths.ڊيگهائي) أرجع خطأ;
    إذا (unlikely (e.بنياد + معلومات.ڊيگهائي > (بنانشان صحيح) cbdt_مفرد.ڊيگهائي)) أرجع خطأ;
    إذا (hb_memcmp (cbdt_مفرد.arrayZ + e.بنياد, معلومات.arrayZ, معلومات.ڊيگهائي)) أرجع خطأ;
    إذا (hb_memcmp (هذا->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.ڊيگهائي * حجم (بنانشان صحيح)))
      أرجع خطأ;
    *بنياد = e.بنياد;
    أرجع سچ;
  }

  منطقي جوڙيو (uint64_t key,
	    بنانشان صحيح بنياد,
	    ثابت hb_ويڪٽر_t<بنانشان صحيح> &lengths)
  {
    إذا (unlikely (entries.اندريون_error () || هذا->lengths.اندريون_error () || map.اندريون_error ()))
      أرجع خطأ;
    داخلا_t e = {بنياد, هذا->lengths.ڊيگهائي, lengths.ڊيگهائي};
    إذا (unlikely (!entries.ڌڪو_or_fail (e))) أرجع خطأ;
    لكل (بنانشان صحيح ڊيگهائي : lengths)
      إذا (unlikely (!هذا->lengths.ڌڪو_or_fail (ڊيگهائي))) أرجع خطأ;
    أرجع map.سيٽ (key, entries.ڊيگهائي - 1);
  }

  hb_hashmap_t<uint64_t, بنانشان صحيح> map;
  hb_ويڪٽر_t<داخلا_t> entries;
  hb_ويڪٽر_t<بنانشان صحيح> lengths; /* Concatenated per-entry lengths. */
};

بنية cblc_bitmap_سائيز_subset_context_t
{
  ثابت حرف *cbdt;
  بنانشان صحيح cbdt_ڊيگهائي;
  hb_ويڪٽر_t<حرف> *cbdt_مفرد;
  بنانشان صحيح سائيز;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  بنانشان صحيح انگ_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t شروعات_glyph;	/*ٻاهريون*/
  hb_codepoint_t پڇاڙي_glyph;	/*ٻاهريون*/
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ساكن مضمن منطقي
copy_معلومات_منزل_cbdt (hb_ويڪٽر_t<حرف> *cbdt_مفرد,
		    ثابت فراغ        *معلومات,
		    بنانشان           ڊيگهائي)
{
  بنانشان صحيح new_len = cbdt_مفرد->ڊيگهائي + ڊيگهائي;
  إذا (unlikely (!cbdt_مفرد->alloc (new_len))) أرجع خطأ;
  hb_memcpy (cbdt_مفرد->arrayZ + cbdt_مفرد->ڊيگهائي, معلومات, ڊيگهائي);
  cbdt_مفرد->ڊيگهائي = new_len;
  أرجع سچ;
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
    extents->ويڪر = ويڪر;
    extents->اوچائي = -static_cast<صحيح> (اوچائي);

    إذا (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	اوچائي;
  HBUINT8	ويڪر;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  عام:
  DEFINE_سائيز_STATIC (5);
};

بنية BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية SBit_سٽ_Metrics
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBINT8	ascender;
  HBINT8	decender;
  HBUINT8	ويڪر_Max;
  HBINT8	caret_لاهي_Numerator;
  HBINT8	caret_لاهي_Denominator;
  HBINT8	caret_وٿي;
  HBINT8	minOriginSB;
  HBINT8	minAdvanceSB;
  HBINT8	maxBeforeBL;
  HBINT8	minAfterBL;
  HBINT8	padding1;
  HBINT8	padding2;
  عام:
  DEFINE_سائيز_STATIC (12);
};


/*
 * Index Subtables.
 */

بنية اشاريو_Subtable_سرو
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	اشاريو_Format;
  HBUINT16	imageFormat;
  HBUINT32	image_معلومات_وٿي;
  عام:
  DEFINE_سائيز_STATIC (8);
};

سانچو <قسم_نالو وٿي_Type>
بنية اشاريو_Subtable_Format1_Or3
{
  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح glyph_ڳڻپ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  وٿي_Array_Z.sanitize (c, glyph_ڳڻپ + 1));
  }

  منطقي get_image_معلومات (بنانشان صحيح idx,
		       بنانشان صحيح *وٿي,
		       بنانشان صحيح *ڊيگهائي) ثابت
  {
    إذا (unlikely (وٿي_Array_Z[idx + 1] <= وٿي_Array_Z[idx]))
      أرجع خطأ;

    *وٿي = سرو.image_معلومات_وٿي + وٿي_Array_Z[idx];
    *ڊيگهائي = وٿي_Array_Z[idx + 1] - وٿي_Array_Z[idx];
    أرجع سچ;
  }

  منطقي جوڙيو_وٿي (hb_serialize_context_t *c,
		   بنانشان صحيح وٿي,
		   بنانشان صحيح *سائيز /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    وٿي_2<وٿي_Type> embedded_وٿي;
    embedded_وٿي = وٿي;
    *سائيز += حجم (وٿي_Type);
    خودڪار *o = c->embed (embedded_وٿي);
    return_trace ((منطقي) o);
  }

  اشاريو_Subtable_سرو	سرو;
  Unsized_Array_جو<وٿي_2<وٿي_Type>>
			وٿي_Array_Z;
  عام:
  DEFINE_سائيز_ARRAY (8, وٿي_Array_Z);
};

بنية اشاريو_Subtable_Format1 : اشاريو_Subtable_Format1_Or3<HBUINT32> {};
بنية اشاريو_Subtable_Format3 : اشاريو_Subtable_Format1_Or3<HBUINT16> {};

بنية اشاريو_Subtable
{
  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح glyph_ڳڻپ) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.سرو.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.سرو.اشاريو_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_ڳڻپ));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_ڳڻپ));
    افتراضي:return_trace (سچ);
    }
  }

  منطقي
  پڄاڻي_subtable (hb_serialize_context_t *c,
		   بنانشان صحيح پڇاڙي_وٿي,
		   بنانشان صحيح انگ_glyphs,
		   بنانشان صحيح *سائيز /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);

    بدل (u.سرو.اشاريو_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.جوڙيو_وٿي (c, پڇاڙي_وٿي, سائيز));
    حالة 3: {
      hb_barrier ();
      إذا (!u.format3.جوڙيو_وٿي (c, پڇاڙي_وٿي, سائيز))
	return_trace (خطأ);
      إذا (!(انگ_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.جوڙيو_وٿي (c, 0, سائيز));
      return_trace (سچ);
    }
    // TODO: implement 2, 4, 5.
    حالة 2: حالة 4:  // No-op.
    حالة 5:  // Pad to 32-bit aligned.
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي
  جوڙيو_وٿي (hb_serialize_context_t *c, بنانشان صحيح local_وٿي,
	      بنانشان صحيح *سائيز /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.سرو.اشاريو_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.جوڙيو_وٿي (c, local_وٿي, سائيز));
    حالة 3: hb_barrier (); return_trace (u.format3.جوڙيو_وٿي (c, local_وٿي, سائيز));
    // TODO: Implement tables 2, 4, 5
    حالة 2:  // Should be a no-op.
    حالة 4: حالة 5:  // Handle sparse cases.
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents HB_اڻ_استعمال_ٿيل, منطقي scale HB_اڻ_استعمال_ٿيل) ثابت
  {
    بدل (u.سرو.اشاريو_Format)
    {
    حالة 2: حالة 5: /* TODO */
    حالة 1: حالة 3: حالة 4: /* Variable-metrics formats do not have metrics here. */
    افتراضي:أرجع (خطأ);
    }
  }

  منطقي
  get_image_معلومات (بنانشان صحيح idx, بنانشان صحيح *وٿي,
		  بنانشان صحيح *ڊيگهائي, بنانشان صحيح *format) ثابت
  {
    *format = u.سرو.imageFormat;
    بدل (u.سرو.اشاريو_Format)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_image_معلومات (idx, وٿي, ڊيگهائي);
    حالة 3: hb_barrier (); أرجع u.format3.get_image_معلومات (idx, وٿي, ڊيگهائي);
    افتراضي: أرجع خطأ;
    }
  }

  ثابت اشاريو_Subtable_سرو* get_سرو () ثابت { أرجع &u.سرو; }

  فراغ populate_سرو (بنانشان اشاريو_format,
			بنانشان image_format,
			بنانشان صحيح image_معلومات_وٿي_2,
			بنانشان صحيح *سائيز)
  {
    u.سرو.اشاريو_Format = اشاريو_format;
    u.سرو.imageFormat = image_format;
    u.سرو.image_معلومات_وٿي = image_معلومات_وٿي_2;
    بدل (u.سرو.اشاريو_Format)
    {
    حالة 1: *سائيز += اشاريو_Subtable_Format1::min_سائيز; اكسر;
    حالة 3: *سائيز += اشاريو_Subtable_Format3::min_سائيز; اكسر;
    }
  }

  محمي:
  اتحاد {
  اشاريو_Subtable_سرو	سرو;
  اشاريو_Subtable_Format1	format1;
  اشاريو_Subtable_Format3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  عام:
  DEFINE_سائيز_UNION (8, سرو);
};

بنية اشاريو_Subtable_رڪارڊ
{
  /* XXX Remove this and fix by not inserting it into vector. */
  اشاريو_Subtable_رڪارڊ& عامل = (ثابت اشاريو_Subtable_رڪارڊ &o)
  {
    پهريون_Glyph_اشاريو = o.پهريون_Glyph_اشاريو;
    آخري_Glyph_اشاريو = o.آخري_Glyph_اشاريو;
    وٿي_منزل_Subtable = (بنانشان) o.وٿي_منزل_Subtable;
    أكد (وٿي_منزل_Subtable.is_null ());
    أرجع *هذا;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  پهريون_Glyph_اشاريو <= آخري_Glyph_اشاريو &&
		  وٿي_منزل_Subtable.sanitize (c, بنياد, آخري_Glyph_اشاريو - پهريون_Glyph_اشاريو + 1));
  }

  ثابت اشاريو_Subtable* get_subtable (ثابت فراغ *بنياد) ثابت
  {
    أرجع &(بنياد+وٿي_منزل_Subtable);
  }

  منطقي جوڙيو_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_سائيز_subset_context_t *bitmap_سائيز_context,
			 اشاريو_Subtable_رڪارڊ *رڪارڊ,
			 ثابت hb_ويڪٽر_t<hb_pair_t<hb_codepoint_t, ثابت اشاريو_Subtable_رڪارڊ*>> *lookup, /*اندريون*/
			 ثابت فراغ *بنياد,
			 بنانشان صحيح *شروعات /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خودڪار *subtable = c->serializer->شروعات_embed<اشاريو_Subtable> ();
    إذا (unlikely (!c->serializer->extend_min (subtable))) return_trace (خطأ);

    خودڪار *old_subtable = get_subtable (بنياد);
    خودڪار *old_سرو = old_subtable->get_سرو ();
    بنانشان صحيح image_format = old_سرو->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_ويڪٽر_t<حرف> معلومات;
    hb_ويڪٽر_t<بنانشان صحيح> lengths;
    بنانشان صحيح انگ_glyphs = 0;
    منطقي early_exit = خطأ;
    لكل (بنانشان صحيح i = *شروعات; i < lookup->ڊيگهائي; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].پهريون;
      ثابت اشاريو_Subtable_رڪارڊ *ايندڙ_رڪارڊ = (*lookup)[i].second;
      ثابت اشاريو_Subtable *ايندڙ_subtable = ايندڙ_رڪارڊ->get_subtable (بنياد);
      خودڪار *ايندڙ_سرو = ايندڙ_subtable->get_سرو ();
      إذا (ايندڙ_سرو != old_سرو)
      {
	*شروعات = i;
	early_exit = سچ;
	اكسر;
      }
      بنانشان صحيح انگ_missing = رڪارڊ->جوڙيو_glyph_for_subset (new_gid);
      لكل (بنانشان صحيح j = 0; j < انگ_missing; j++)
	إذا (unlikely (!c->serializer->check_success (lengths.ڌڪو_or_fail (0))))
	  return_trace (خطأ);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      إذا (old_gid < ايندڙ_رڪارڊ->پهريون_Glyph_اشاريو)
	return_trace (خطأ);

      بنانشان صحيح old_idx = (بنانشان صحيح) old_gid - ايندڙ_رڪارڊ->پهريون_Glyph_اشاريو;
      بنانشان صحيح وٿي, ڊيگهائي, format;
      إذا (unlikely (!ايندڙ_subtable->get_image_معلومات (old_idx, &وٿي, &ڊيگهائي, &format)))
	return_trace (خطأ);
      إذا (unlikely (وٿي > bitmap_سائيز_context->cbdt_ڊيگهائي ||
		    bitmap_سائيز_context->cbdt_ڊيگهائي - وٿي < ڊيگهائي))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (
			copy_معلومات_منزل_cbdt (&معلومات,
					    bitmap_سائيز_context->cbdt + وٿي,
					    ڊيگهائي))))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (lengths.ڌڪو_or_fail (ڊيگهائي))))
	return_trace (خطأ);
      انگ_glyphs += 1 + انگ_missing;
    }
    إذا (!early_exit)
      *شروعات = lookup->ڊيگهائي;

    /* Find or append the data block for this subtable. */
    بنانشان صحيح image_معلومات_وٿي_2;
    cbdt_dedup_context_t *dedup = bitmap_سائيز_context->dedup;
    منطقي shared = خطأ;
    uint64_t key = 0;
    إذا (dedup && معلومات.ڊيگهائي)
    {
      key = cbdt_dedup_context_t::hash (image_format, معلومات, lengths);
      shared = dedup->find (key, معلومات, lengths,
			    *bitmap_سائيز_context->cbdt_مفرد,
			    &image_معلومات_وٿي_2);
    }
    إذا (!shared)
    {
      image_معلومات_وٿي_2 = bitmap_سائيز_context->cbdt_مفرد->ڊيگهائي;
      إذا (unlikely (!c->serializer->check_success (
			copy_معلومات_منزل_cbdt (bitmap_سائيز_context->cbdt_مفرد,
					    معلومات.arrayZ, معلومات.ڊيگهائي))))
	return_trace (خطأ);
      إذا (dedup && معلومات.ڊيگهائي)
	/* The bitmap data is already copied; caching is best-effort. */
	(فراغ) dedup->جوڙيو (key, image_معلومات_وٿي_2, lengths);
    }

    subtable->populate_سرو (old_سرو->اشاريو_Format,
			       image_format,
			       image_معلومات_وٿي_2,
			       &bitmap_سائيز_context->سائيز);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    بنانشان صحيح cumulative = 0;
    لكل (بنانشان صحيح ڊيگهائي : lengths)
    {
      إذا (unlikely (!subtable->جوڙيو_وٿي (c->serializer, cumulative,
					   &bitmap_سائيز_context->سائيز)))
	return_trace (خطأ);
      cumulative += ڊيگهائي;
    }
    إذا (unlikely (!subtable->پڄاڻي_subtable (c->serializer,
					      cumulative,
					      انگ_glyphs,
					      &bitmap_سائيز_context->سائيز)))
      return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي جوڙيو_new_رڪارڊ (hb_subset_context_t *c,
		       cblc_bitmap_سائيز_subset_context_t *bitmap_سائيز_context,
		       ثابت hb_ويڪٽر_t<hb_pair_t<hb_codepoint_t, ثابت اشاريو_Subtable_رڪارڊ*>> *lookup, /*اندريون*/
		       ثابت فراغ *بنياد,
		       بنانشان صحيح *شروعات, /* INOUT */
		       hb_ويڪٽر_t<اشاريو_Subtable_رڪارڊ>* رڪارڊون /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار snap = c->serializer->snapshot ();
    بنانشان صحيح old_سائيز = bitmap_سائيز_context->سائيز;
    بنانشان صحيح old_cbdt_مفرد_ڊيگهائي = bitmap_سائيز_context->cbdt_مفرد->ڊيگهائي;

    // Set to invalid state to indicate filling glyphs is not yet started.
    إذا (unlikely (!c->serializer->check_success (رڪارڊون->resize (رڪارڊون->ڊيگهائي + 1))))
      return_trace (خطأ);

    رڪارڊون->پڇ ().پهريون_Glyph_اشاريو = 1;
    رڪارڊون->پڇ ().آخري_Glyph_اشاريو = 0;
    bitmap_سائيز_context->سائيز += اشاريو_Subtable_رڪارڊ::min_سائيز;

    c->serializer->push ();

    إذا (unlikely (!جوڙيو_new_subtable (c, bitmap_سائيز_context, &(رڪارڊون->پڇ ()), lookup, بنياد, شروعات)))
    {
      c->serializer->ڪڍو_discard ();
      c->serializer->revert (snap);
      bitmap_سائيز_context->cbdt_مفرد->shrink (old_cbdt_مفرد_ڊيگهائي);
      bitmap_سائيز_context->سائيز = old_سائيز;
      رڪارڊون->resize (رڪارڊون->ڊيگهائي - 1);
      return_trace (خطأ);
    }

    bitmap_سائيز_context->انگ_tables += 1;
    return_trace (سچ);
  }

  بنانشان صحيح جوڙيو_glyph_for_subset (hb_codepoint_t gid)
  {
    إذا (پهريون_Glyph_اشاريو > آخري_Glyph_اشاريو)
    {
      پهريون_Glyph_اشاريو = gid;
      آخري_Glyph_اشاريو = gid;
      أرجع 0;
    }
    // TODO maybe assert? this shouldn't occur.
    إذا (آخري_Glyph_اشاريو > gid)
      أرجع 0;
    بنانشان صحيح انگ_missing = (بنانشان صحيح) (gid - آخري_Glyph_اشاريو - 1);
    آخري_Glyph_اشاريو = gid;
    أرجع انگ_missing;
  }

  منطقي get_extents (hb_glyph_extents_t *extents, ثابت فراغ *بنياد, منطقي scale) ثابت
  { أرجع (بنياد+وٿي_منزل_Subtable).get_extents (extents, scale); }

  منطقي get_image_معلومات (بنانشان صحيح  gid,
		       ثابت فراغ   *بنياد,
		       بنانشان صحيح *وٿي,
		       بنانشان صحيح *ڊيگهائي,
		       بنانشان صحيح *format) ثابت
  {
    إذا (gid < پهريون_Glyph_اشاريو || gid > آخري_Glyph_اشاريو) أرجع خطأ;
    أرجع (بنياد+وٿي_منزل_Subtable).get_image_معلومات (gid - پهريون_Glyph_اشاريو,
						   وٿي, ڊيگهائي, format);
  }

  HBGlyphID16			پهريون_Glyph_اشاريو;
  HBGlyphID16			آخري_Glyph_اشاريو;
  Offset32_منزل<اشاريو_Subtable>	وٿي_منزل_Subtable;
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية اشاريو_Subtable_Array
{
  دوست بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح ڳڻپ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (اشاريو_Subtables_Z.sanitize (c, ڳڻپ, هذا));
  }

  فراغ
  build_lookup (hb_subset_context_t *c, cblc_bitmap_سائيز_subset_context_t *bitmap_سائيز_context,
		hb_ويڪٽر_t<hb_pair_t<hb_codepoint_t,
		ثابت اشاريو_Subtable_رڪارڊ*>> *lookup /*ٻاهريون*/) ثابت
  {
    منطقي شروعات_glyph_is_سيٽ = خطأ;
    بنانشان انگ_glyphs = c->plan->انگ_output_glyphs ();
    لكل (hb_codepoint_t new_gid = 0; new_gid < انگ_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) استمر;

      ثابت اشاريو_Subtable_رڪارڊ* رڪارڊ = ڳوليو_جدول (old_gid, bitmap_سائيز_context->انگ_tables);
      إذا (unlikely (!رڪارڊ)) استمر;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      بنانشان صحيح وٿي, ڊيگهائي, format;
      إذا (unlikely (!رڪارڊ->get_image_معلومات (old_gid, هذا, &وٿي, &ڊيگهائي, &format))) استمر;

      lookup->push (hb_pair_t<hb_codepoint_t, ثابت اشاريو_Subtable_رڪارڊ*> (new_gid, رڪارڊ));

      إذا (!شروعات_glyph_is_سيٽ)
      {
	bitmap_سائيز_context->شروعات_glyph = new_gid;
	شروعات_glyph_is_سيٽ = سچ;
      }

      bitmap_سائيز_context->پڇاڙي_glyph = new_gid;
    }
  }

  منطقي
  subset (hb_subset_context_t *c,
	  cblc_bitmap_سائيز_subset_context_t *bitmap_سائيز_context) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_ويڪٽر_t<hb_pair_t<hb_codepoint_t, ثابت اشاريو_Subtable_رڪارڊ*>> lookup;
    build_lookup (c, bitmap_سائيز_context, &lookup);
    إذا (unlikely (!c->serializer->propagate_error (lookup)))
      أرجع خطأ;

    bitmap_سائيز_context->سائيز = 0;
    bitmap_سائيز_context->انگ_tables = 0;
    hb_ويڪٽر_t<اشاريو_Subtable_رڪارڊ> رڪارڊون;
    لكل (بنانشان صحيح شروعات = 0; شروعات < lookup.ڊيگهائي;)
    {
      إذا (unlikely (!lookup[شروعات].second->جوڙيو_new_رڪارڊ (c, bitmap_سائيز_context, &lookup, هذا, &شروعات, &رڪارڊون)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	لكل (بنانشان صحيح i = 0; i < رڪارڊون.ڊيگهائي; i++)
	  c->serializer->ڪڍو_discard ();
	return_trace (خطأ);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_ويڪٽر_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (بنانشان صحيح i = 0; i < رڪارڊون.ڊيگهائي; i++)
      objidxs.push (c->serializer->ڪڍو_pack ());
    لكل (بنانشان صحيح i = 0; i < رڪارڊون.ڊيگهائي; i++)
    {
      اشاريو_Subtable_رڪارڊ* رڪارڊ = c->serializer->embed (رڪارڊون[i]);
      إذا (unlikely (!رڪارڊ)) return_trace (خطأ);
      c->serializer->جوڙيو_link (رڪارڊ->وٿي_منزل_Subtable, objidxs[رڪارڊون.ڊيگهائي - 1 - i]);
    }
    return_trace (سچ);
  }

  عام:
  ثابت اشاريو_Subtable_رڪارڊ* ڳوليو_جدول (hb_codepoint_t glyph, بنانشان صحيح انگ_Tables) ثابت
  {
    لكل (بنانشان صحيح i = 0; i < انگ_Tables; ++i)
    {
      بنانشان صحيح پهريون_Glyph_اشاريو = اشاريو_Subtables_Z[i].پهريون_Glyph_اشاريو;
      بنانشان صحيح آخري_Glyph_اشاريو = اشاريو_Subtables_Z[i].آخري_Glyph_اشاريو;
      إذا (پهريون_Glyph_اشاريو <= glyph && glyph <= آخري_Glyph_اشاريو)
	أرجع &اشاريو_Subtables_Z[i];
    }
    أرجع خالي_اشارو;
  }

  محمي:
  Unsized_Array_جو<اشاريو_Subtable_رڪارڊ>	اشاريو_Subtables_Z;
};

بنية Bitmap_سائيز_جدول
{
  دوست بنية CBLC;
  دوست بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  اشاريو_Subtable_Array_وٿي.sanitize (c, بنياد, عدد_جو_اشاريو_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ثابت اشاريو_Subtable_رڪارڊ *
  ڳوليو_جدول (hb_codepoint_t glyph, ثابت فراغ *بنياد, ثابت فراغ **ٻاهريون_بنياد) ثابت
  {
    *ٻاهريون_بنياد = &(بنياد+اشاريو_Subtable_Array_وٿي);
    أرجع (بنياد+اشاريو_Subtable_Array_وٿي).ڳوليو_جدول (glyph, عدد_جو_اشاريو_Subtables);
  }

  منطقي
  subset (hb_subset_context_t *c, ثابت فراغ *بنياد,
	  ثابت حرف *cbdt, بنانشان صحيح cbdt_ڊيگهائي,
	  hb_ويڪٽر_t<حرف> *cbdt_مفرد /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون_جدول = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون_جدول)) return_trace (خطأ);

    cblc_bitmap_سائيز_subset_context_t bitmap_سائيز_context;
    bitmap_سائيز_context.cbdt = cbdt;
    bitmap_سائيز_context.cbdt_ڊيگهائي = cbdt_ڊيگهائي;
    bitmap_سائيز_context.cbdt_مفرد = cbdt_مفرد;
    bitmap_سائيز_context.سائيز = اشاريو_Tables_سائيز;
    bitmap_سائيز_context.انگ_tables = عدد_جو_اشاريو_Subtables;
    bitmap_سائيز_context.شروعات_glyph = 1;
    bitmap_سائيز_context.پڇاڙي_glyph = 0;
    bitmap_سائيز_context.dedup = dedup;

    إذا (!ٻاهريون_جدول->اشاريو_Subtable_Array_وٿي.serialize_subset (c,
							       اشاريو_Subtable_Array_وٿي,
							       بنياد,
							       &bitmap_سائيز_context))
      return_trace (خطأ);
    إذا (!bitmap_سائيز_context.سائيز ||
	!bitmap_سائيز_context.انگ_tables ||
	bitmap_سائيز_context.شروعات_glyph > bitmap_سائيز_context.پڇاڙي_glyph)
      return_trace (خطأ);

    ٻاهريون_جدول->اشاريو_Tables_سائيز = bitmap_سائيز_context.سائيز;
    ٻاهريون_جدول->عدد_جو_اشاريو_Subtables = bitmap_سائيز_context.انگ_tables;
    ٻاهريون_جدول->شروعات_Glyph_اشاريو = bitmap_سائيز_context.شروعات_glyph;
    ٻاهريون_جدول->پڇاڙي_Glyph_اشاريو = bitmap_سائيز_context.پڇاڙي_glyph;
    return_trace (سچ);
  }

  محمي:
  NNOffset32_منزل<اشاريو_Subtable_Array>
			اشاريو_Subtable_Array_وٿي;
  HBUINT32		اشاريو_Tables_سائيز;
  HBUINT32		عدد_جو_اشاريو_Subtables;
  HBUINT32		colorRef;
  SBit_سٽ_Metrics	horizontal;
  SBit_سٽ_Metrics	vertical;
  HBGlyphID16		شروعات_Glyph_اشاريو;
  HBGlyphID16		پڇاڙي_Glyph_اشاريو;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		ٻٽو_انگ_اونهائي;
  HBINT8		flags;
  عام:
  DEFINE_سائيز_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

بنية Glyph_Bitmap_معلومات_Format17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32_جو<HBUINT8>	معلومات;
  عام:
  DEFINE_سائيز_ARRAY (9, معلومات);
};

بنية Glyph_Bitmap_معلومات_Format18
{
  BigGlyphMetrics	glyphMetrics;
  Array32_جو<HBUINT8>	معلومات;
  عام:
  DEFINE_سائيز_ARRAY (12, معلومات);
};

بنية Glyph_Bitmap_معلومات_Format19
{
  Array32_جو<HBUINT8>	معلومات;
  عام:
  DEFINE_سائيز_ARRAY (4, معلومات);
};

بنية CBLC
{
  دوست بنية CBDT;

  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_CBLC;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخو.major == 2 || نسخو.major == 3) &&
		  hb_barrier () &&
		  سائيز_Tables.sanitize (c, هذا));
  }

  ساكن منطقي
  sink_cbdt (hb_subset_context_t *c, hb_ويڪٽر_t<حرف>* cbdt_مفرد)
  {
    hb_blob_t *cbdt_مفرد_blob = hb_blob_create (cbdt_مفرد->arrayZ,
						 cbdt_مفرد->ڊيگهائي,
						 HB_يادگيري_گهڻائي_WRITABLE,
						 cbdt_مفرد->arrayZ,
						 hb_free);
    cbdt_مفرد->init ();  // Leak arrayZ to the blob.
    منطقي ret = c->plan->جوڙيو_جدول (HB_OT_ٽيگ_CBDT, cbdt_مفرد_blob);
    hb_blob_destroy (cbdt_مفرد_blob);
    أرجع ret;
  }

  منطقي
  subset_سائيز_جدول (hb_subset_context_t *c, ثابت Bitmap_سائيز_جدول& جدول,
		     ثابت حرف *cbdt /*اندريون*/, بنانشان صحيح cbdt_ڊيگهائي,
		     CBLC *cblc_مفرد /* INOUT */, hb_ويڪٽر_t<حرف> *cbdt_مفرد /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    cblc_مفرد->سائيز_Tables.len++;

    خودڪار snap = c->serializer->snapshot ();
    خودڪار cbdt_مفرد_len = cbdt_مفرد->ڊيگهائي;

    إذا (!جدول.subset (c, هذا, cbdt, cbdt_ڊيگهائي, cbdt_مفرد, dedup))
    {
      cblc_مفرد->سائيز_Tables.len--;
      c->serializer->revert (snap);
      cbdt_مفرد->shrink (cbdt_مفرد_len);
      return_trace (خطأ);
    }
    return_trace (سچ);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_اندروني منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  ثابت Bitmap_سائيز_جدول &چونڊيو_strike (hb_font_t *font) ثابت
  {
    بنانشان ڳڻپ = سائيز_Tables.len;
    إذا (unlikely (!ڳڻپ))
      أرجع Null (Bitmap_سائيز_جدول);

    بنانشان صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    إذا (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    بنانشان صحيح بهترين_i = 0;
    بنانشان صحيح بهترين_ppem = hb_max (سائيز_Tables[0].ppemX, سائيز_Tables[0].ppemY);

    لكل (بنانشان صحيح i = 1; i < ڳڻپ; i++)
    {
      بنانشان صحيح ppem = hb_max (سائيز_Tables[i].ppemX, سائيز_Tables[i].ppemY);
      إذا ((requested_ppem <= ppem && ppem < بهترين_ppem) ||
	  (requested_ppem > بهترين_ppem && ppem > بهترين_ppem))
      {
	بهترين_i = i;
	بهترين_ppem = ppem;
      }
    }

    أرجع سائيز_Tables[بهترين_i];
  }

  محمي:
  Fixed_نسخو<>		نسخو;
  Array32_جو<Bitmap_سائيز_جدول>	سائيز_Tables;
  عام:
  DEFINE_سائيز_ARRAY (8, سائيز_Tables);
};

بنية CBDT
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_CBDT;

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
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, منطقي scale = سچ) ثابت
    {
      ثابت فراغ *بنياد;
      ثابت Bitmap_سائيز_جدول &strike = هذا->cblc->چونڊيو_strike (font);
      ثابت اشاريو_Subtable_رڪارڊ *subtable_رڪارڊ = strike.ڳوليو_جدول (glyph, cblc, &بنياد);
      إذا (!subtable_رڪارڊ || !strike.ppemX || !strike.ppemY)
	أرجع خطأ;

      إذا (subtable_رڪارڊ->get_extents (extents, بنياد, scale))
	أرجع سچ;

      بنانشان صحيح image_وٿي = 0, image_ڊيگهائي = 0, image_format = 0;
      إذا (!subtable_رڪارڊ->get_image_معلومات (glyph, بنياد, &image_وٿي, &image_ڊيگهائي, &image_format))
	أرجع خطأ;

      بنانشان صحيح cbdt_len = cbdt.get_ڊيگهائي ();
      إذا (unlikely (image_وٿي > cbdt_len || cbdt_len - image_وٿي < image_ڊيگهائي))
	أرجع خطأ;

      بدل (image_format)
      {
      حالة 17: {
	إذا (unlikely (image_ڊيگهائي < Glyph_Bitmap_معلومات_Format17::min_سائيز))
	  أرجع خطأ;
	خودڪار &glyphFormat17 = Struct_At_وٿي<Glyph_Bitmap_معلومات_Format17> (هذا->cbdt, image_وٿي);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	اكسر;
      }
      حالة 18: {
	إذا (unlikely (image_ڊيگهائي < Glyph_Bitmap_معلومات_Format18::min_سائيز))
	  أرجع خطأ;
	خودڪار &glyphFormat18 = Struct_At_وٿي<Glyph_Bitmap_معلومات_Format18> (هذا->cbdt, image_وٿي);
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
	extents->x_bearing = hb_clamp_منزل<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_منزل<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->ويڪر = hb_clamp_منزل<hb_position_t> (roundf (extents->ويڪر * x_scale));
	extents->اوچائي = hb_clamp_منزل<hb_position_t> (roundf (extents->اوچائي * y_scale));
      }

      أرجع سچ;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ثابت
    {
      ثابت فراغ *بنياد;
      ثابت Bitmap_سائيز_جدول &strike = هذا->cblc->چونڊيو_strike (font);
      ثابت اشاريو_Subtable_رڪارڊ *subtable_رڪارڊ = strike.ڳوليو_جدول (glyph, cblc, &بنياد);
      إذا (!subtable_رڪارڊ || !strike.ppemX || !strike.ppemY)
	أرجع hb_blob_get_خالي_جايون ();

      بنانشان صحيح image_وٿي = 0, image_ڊيگهائي = 0, image_format = 0;
      إذا (!subtable_رڪارڊ->get_image_معلومات (glyph, بنياد, &image_وٿي, &image_ڊيگهائي, &image_format))
	أرجع hb_blob_get_خالي_جايون ();

      بنانشان صحيح cbdt_len = cbdt.get_ڊيگهائي ();
      إذا (unlikely (image_وٿي > cbdt_len || cbdt_len - image_وٿي < image_ڊيگهائي))
	أرجع hb_blob_get_خالي_جايون ();

      بدل (image_format)
      {
      حالة 17:
      {
	إذا (unlikely (image_ڊيگهائي < Glyph_Bitmap_معلومات_Format17::min_سائيز))
	  أرجع hb_blob_get_خالي_جايون ();
	خودڪار &glyphFormat17 = Struct_At_وٿي<Glyph_Bitmap_معلومات_Format17> (هذا->cbdt, image_وٿي);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_وٿي + Glyph_Bitmap_معلومات_Format17::min_سائيز,
					glyphFormat17.معلومات.len);
      }
      حالة 18:
      {
	إذا (unlikely (image_ڊيگهائي < Glyph_Bitmap_معلومات_Format18::min_سائيز))
	  أرجع hb_blob_get_خالي_جايون ();
	خودڪار &glyphFormat18 = Struct_At_وٿي<Glyph_Bitmap_معلومات_Format18> (هذا->cbdt, image_وٿي);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_وٿي + Glyph_Bitmap_معلومات_Format18::min_سائيز,
					glyphFormat18.معلومات.len);
      }
      حالة 19:
      {
	إذا (unlikely (image_ڊيگهائي < Glyph_Bitmap_معلومات_Format19::min_سائيز))
	  أرجع hb_blob_get_خالي_جايون ();
	خودڪار &glyphFormat19 = Struct_At_وٿي<Glyph_Bitmap_معلومات_Format19> (هذا->cbdt, image_وٿي);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_وٿي + Glyph_Bitmap_معلومات_Format19::min_سائيز,
					glyphFormat19.معلومات.len);
      }
      افتراضي: أرجع hb_blob_get_خالي_جايون (); /* TODO: Support other image formats. */
      }
    }

    منطقي has_معلومات () ثابت { أرجع cbdt->نسخو.major; }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *معلومات) ثابت
    {
      إذا (!has_معلومات ()) أرجع خطأ;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      إذا (unlikely (!font->get_glyph_extents (glyph, &extents, خطأ)))
        أرجع خطأ;

      إذا (unlikely (!get_extents (font, glyph, &pixel_extents, خطأ)))
        أرجع خطأ;

      hb_blob_t *blob = reference_png (font, glyph);
      إذا (unlikely (hb_blob_is_immutable (blob)))
        أرجع خطأ;

      منطقي ret = funcs->image (معلومات,
			       blob,
			       pixel_extents.ويڪر, -pixel_extents.اوچائي,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    بنانشان صحيح upem;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخو.major == 2 || نسخو.major == 3));
  }

  محمي:
  Fixed_نسخو<>		نسخو;
  Unsized_Array_جو<HBUINT8>	معلومات_Z;
  عام:
  DEFINE_سائيز_ARRAY (4, معلومات_Z);
};

مضمن منطقي
CBLC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_ويڪٽر_t<حرف> cbdt_مفرد;

  خودڪار *cblc_مفرد = c->serializer->شروعات_embed<CBLC> ();
  إذا (unlikely (!c->serializer->extend_min (cblc_مفرد))) return_trace (خطأ);
  cblc_مفرد->نسخو = نسخو;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_جدول<CBDT> (c->plan->ذريعو);
  بنانشان صحيح cbdt_ڊيگهائي;
  CBDT* cbdt = (CBDT *) hb_blob_get_معلومات (cbdt_blob, &cbdt_ڊيگهائي);
  إذا (unlikely (cbdt_ڊيگهائي < CBDT::min_سائيز))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (خطأ);
  }
  copy_معلومات_منزل_cbdt (&cbdt_مفرد, cbdt, CBDT::min_سائيز);

  cbdt_dedup_context_t dedup;
  لكل (ثابت Bitmap_سائيز_جدول& جدول : + سائيز_Tables.iter ())
    subset_سائيز_جدول (c, جدول, (ثابت حرف *) cbdt, cbdt_ڊيگهائي, cblc_مفرد, &cbdt_مفرد, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_مفرد));
}

بنية CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#جيڪڏهن_پورو /* OT_COLOR_CBDT_CBDT_HH */
