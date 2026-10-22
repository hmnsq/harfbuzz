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

#که_نه_تعریف_شوی OT_COLOR_CBDT_CBDT_HH
#تعریف_کړه OT_COLOR_CBDT_CBDT_HH

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
#تعریف_کړه HB_OT_نښان_CBLC HB_نښان('C','B','L','C')
#تعریف_کړه HB_OT_نښان_CBDT HB_نښان('C','B','D','T')


نطاق OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
بنية cbdt_dedup_context_t
{
  بنية ثبت_t
  {
    بینښه صحيح بنسټ;		/* Offset of the data in cbdt_prime. */
    بینښه صحيح lstart;	/* Start index in lengths. */
    بینښه صحيح lcount;	/* Number of length entries. */
  };

  ساكن uint64_t hash (بینښه صحيح image_format,
			ثابت hb_ویکټور_t<حرف> &معلومات,
			ثابت hb_ویکټور_t<بینښه صحيح> &lengths)
  {
    uint32_t h = hb_hash (hb_بایټونه_t (معلومات.arrayZ, معلومات.اوږدتیا));
    h = h * 31 + hb_hash (hb_بایټونه_t ((ثابت حرف *) lengths.arrayZ,
				      lengths.اوږدتیا * حجم (بینښه صحيح)));
    أرجع ((uint64_t) image_format << 32) | h;
  }

  منطقي find (uint64_t key,
	     ثابت hb_ویکټور_t<حرف> &معلومات,
	     ثابت hb_ویکټور_t<بینښه صحيح> &lengths,
	     ثابت hb_ویکټور_t<حرف> &cbdt_اولي,
	     بینښه صحيح *بنسټ /*بهر*/) ثابت
  {
    بینښه صحيح *idx_p = تش_اشاره;
    إذا (!map.has (key, &idx_p)) أرجع خطأ;
    ثابت ثبت_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    إذا (e.lcount != lengths.اوږدتیا) أرجع خطأ;
    إذا (unlikely (e.بنسټ + معلومات.اوږدتیا > (بینښه صحيح) cbdt_اولي.اوږدتیا)) أرجع خطأ;
    إذا (hb_memcmp (cbdt_اولي.arrayZ + e.بنسټ, معلومات.arrayZ, معلومات.اوږدتیا)) أرجع خطأ;
    إذا (hb_memcmp (هذا->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.اوږدتیا * حجم (بینښه صحيح)))
      أرجع خطأ;
    *بنسټ = e.بنسټ;
    أرجع ریښتیا;
  }

  منطقي جمع_کړه (uint64_t key,
	    بینښه صحيح بنسټ,
	    ثابت hb_ویکټور_t<بینښه صحيح> &lengths)
  {
    إذا (unlikely (entries.دننه_error () || هذا->lengths.دننه_error () || map.دننه_error ()))
      أرجع خطأ;
    ثبت_t e = {بنسټ, هذا->lengths.اوږدتیا, lengths.اوږدتیا};
    إذا (unlikely (!entries.ټېل_or_fail (e))) أرجع خطأ;
    لكل (بینښه صحيح اوږدتیا : lengths)
      إذا (unlikely (!هذا->lengths.ټېل_or_fail (اوږدتیا))) أرجع خطأ;
    أرجع map.سېټ (key, entries.اوږدتیا - 1);
  }

  hb_hashmap_t<uint64_t, بینښه صحيح> map;
  hb_ویکټور_t<ثبت_t> entries;
  hb_ویکټور_t<بینښه صحيح> lengths; /* Concatenated per-entry lengths. */
};

بنية cblc_bitmap_اندازه_subset_context_t
{
  ثابت حرف *cbdt;
  بینښه صحيح cbdt_اوږدتیا;
  hb_ویکټور_t<حرف> *cbdt_اولي;
  بینښه صحيح اندازه;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  بینښه صحيح عدد_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t پیل_glyph;	/*بهر*/
  hb_codepoint_t پای_ټکی_glyph;	/*بهر*/
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ساكن مضمن منطقي
copy_معلومات_منزل_cbdt (hb_ویکټور_t<حرف> *cbdt_اولي,
		    ثابت فراغ        *معلومات,
		    بینښه           اوږدتیا)
{
  بینښه صحيح new_len = cbdt_اولي->اوږدتیا + اوږدتیا;
  إذا (unlikely (!cbdt_اولي->alloc (new_len))) أرجع خطأ;
  hb_memcpy (cbdt_اولي->arrayZ + cbdt_اولي->اوږدتیا, معلومات, اوږدتیا);
  cbdt_اولي->اوږدتیا = new_len;
  أرجع ریښتیا;
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
    extents->پلنوالی = پلنوالی;
    extents->لوړوالی = -static_cast<صحيح> (لوړوالی);

    إذا (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	لوړوالی;
  HBUINT8	پلنوالی;
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

بنية SBit_کرښه_Metrics
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBINT8	ascender;
  HBINT8	decender;
  HBUINT8	پلنوالی_Max;
  HBINT8	caret_میلان_Numerator;
  HBINT8	caret_میلان_Denominator;
  HBINT8	caret_دننه_واټن;
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

بنية شاخص_Subtable_سرلیک
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	شاخص_Format;
  HBUINT16	imageFormat;
  HBUINT32	image_معلومات_دننه_واټن;
  عام:
  DEFINE_اندازه_STATIC (8);
};

کينډۍ <ډول_نوم دننه_واټن_Type>
بنية شاخص_Subtable_Format1_Or3
{
  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح glyph_شمېر) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  دننه_واټن_Array_Z.sanitize (c, glyph_شمېر + 1));
  }

  منطقي get_image_معلومات (بینښه صحيح idx,
		       بینښه صحيح *دننه_واټن,
		       بینښه صحيح *اوږدتیا) ثابت
  {
    إذا (unlikely (دننه_واټن_Array_Z[idx + 1] <= دننه_واټن_Array_Z[idx]))
      أرجع خطأ;

    *دننه_واټن = سرلیک.image_معلومات_دننه_واټن + دننه_واټن_Array_Z[idx];
    *اوږدتیا = دننه_واټن_Array_Z[idx + 1] - دننه_واټن_Array_Z[idx];
    أرجع ریښتیا;
  }

  منطقي جمع_کړه_دننه_واټن (hb_serialize_context_t *c,
		   بینښه صحيح دننه_واټن,
		   بینښه صحيح *اندازه /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    دننه_واټن_2<دننه_واټن_Type> embedded_دننه_واټن;
    embedded_دننه_واټن = دننه_واټن;
    *اندازه += حجم (دننه_واټن_Type);
    خپلکاری *o = c->embed (embedded_دننه_واټن);
    return_trace ((منطقي) o);
  }

  شاخص_Subtable_سرلیک	سرلیک;
  Unsized_Array_د<دننه_واټن_2<دننه_واټن_Type>>
			دننه_واټن_Array_Z;
  عام:
  DEFINE_اندازه_ARRAY (8, دننه_واټن_Array_Z);
};

بنية شاخص_Subtable_Format1 : شاخص_Subtable_Format1_Or3<HBUINT32> {};
بنية شاخص_Subtable_Format3 : شاخص_Subtable_Format1_Or3<HBUINT16> {};

بنية شاخص_Subtable
{
  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح glyph_شمېر) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.سرلیک.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.سرلیک.شاخص_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_شمېر));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_شمېر));
    افتراضي:return_trace (ریښتیا);
    }
  }

  منطقي
  پای_subtable (hb_serialize_context_t *c,
		   بینښه صحيح پای_ټکی_دننه_واټن,
		   بینښه صحيح عدد_glyphs,
		   بینښه صحيح *اندازه /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);

    بدل (u.سرلیک.شاخص_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.جمع_کړه_دننه_واټن (c, پای_ټکی_دننه_واټن, اندازه));
    حالة 3: {
      hb_barrier ();
      إذا (!u.format3.جمع_کړه_دننه_واټن (c, پای_ټکی_دننه_واټن, اندازه))
	return_trace (خطأ);
      إذا (!(عدد_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.جمع_کړه_دننه_واټن (c, 0, اندازه));
      return_trace (ریښتیا);
    }
    // TODO: implement 2, 4, 5.
    حالة 2: حالة 4:  // No-op.
    حالة 5:  // Pad to 32-bit aligned.
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي
  جمع_کړه_دننه_واټن (hb_serialize_context_t *c, بینښه صحيح local_دننه_واټن,
	      بینښه صحيح *اندازه /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.سرلیک.شاخص_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.جمع_کړه_دننه_واټن (c, local_دننه_واټن, اندازه));
    حالة 3: hb_barrier (); return_trace (u.format3.جمع_کړه_دننه_واټن (c, local_دننه_واټن, اندازه));
    // TODO: Implement tables 2, 4, 5
    حالة 2:  // Should be a no-op.
    حالة 4: حالة 5:  // Handle sparse cases.
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents HB_ناکارول_شوی, منطقي scale HB_ناکارول_شوی) ثابت
  {
    بدل (u.سرلیک.شاخص_Format)
    {
    حالة 2: حالة 5: /* TODO */
    حالة 1: حالة 3: حالة 4: /* Variable-metrics formats do not have metrics here. */
    افتراضي:أرجع (خطأ);
    }
  }

  منطقي
  get_image_معلومات (بینښه صحيح idx, بینښه صحيح *دننه_واټن,
		  بینښه صحيح *اوږدتیا, بینښه صحيح *format) ثابت
  {
    *format = u.سرلیک.imageFormat;
    بدل (u.سرلیک.شاخص_Format)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_image_معلومات (idx, دننه_واټن, اوږدتیا);
    حالة 3: hb_barrier (); أرجع u.format3.get_image_معلومات (idx, دننه_واټن, اوږدتیا);
    افتراضي: أرجع خطأ;
    }
  }

  ثابت شاخص_Subtable_سرلیک* get_سرلیک () ثابت { أرجع &u.سرلیک; }

  فراغ populate_سرلیک (بینښه شاخص_format,
			بینښه image_format,
			بینښه صحيح image_معلومات_دننه_واټن_2,
			بینښه صحيح *اندازه)
  {
    u.سرلیک.شاخص_Format = شاخص_format;
    u.سرلیک.imageFormat = image_format;
    u.سرلیک.image_معلومات_دننه_واټن = image_معلومات_دننه_واټن_2;
    بدل (u.سرلیک.شاخص_Format)
    {
    حالة 1: *اندازه += شاخص_Subtable_Format1::min_اندازه; اكسر;
    حالة 3: *اندازه += شاخص_Subtable_Format3::min_اندازه; اكسر;
    }
  }

  محمي:
  اتحاد {
  شاخص_Subtable_سرلیک	سرلیک;
  شاخص_Subtable_Format1	format1;
  شاخص_Subtable_Format3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  عام:
  DEFINE_اندازه_UNION (8, سرلیک);
};

بنية شاخص_Subtable_ریکارډ
{
  /* XXX Remove this and fix by not inserting it into vector. */
  شاخص_Subtable_ریکارډ& چلوونکی = (ثابت شاخص_Subtable_ریکارډ &o)
  {
    لومړی_Glyph_شاخص = o.لومړی_Glyph_شاخص;
    وروستی_Glyph_شاخص = o.وروستی_Glyph_شاخص;
    دننه_واټن_منزل_Subtable = (بینښه) o.دننه_واټن_منزل_Subtable;
    أكد (دننه_واټن_منزل_Subtable.is_null ());
    أرجع *هذا;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  لومړی_Glyph_شاخص <= وروستی_Glyph_شاخص &&
		  دننه_واټن_منزل_Subtable.sanitize (c, بنسټ, وروستی_Glyph_شاخص - لومړی_Glyph_شاخص + 1));
  }

  ثابت شاخص_Subtable* get_subtable (ثابت فراغ *بنسټ) ثابت
  {
    أرجع &(بنسټ+دننه_واټن_منزل_Subtable);
  }

  منطقي جمع_کړه_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_اندازه_subset_context_t *bitmap_اندازه_context,
			 شاخص_Subtable_ریکارډ *ریکارډ,
			 ثابت hb_ویکټور_t<hb_pair_t<hb_codepoint_t, ثابت شاخص_Subtable_ریکارډ*>> *lookup, /*دننه*/
			 ثابت فراغ *بنسټ,
			 بینښه صحيح *پیل /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خپلکاری *subtable = c->serializer->پیل_embed<شاخص_Subtable> ();
    إذا (unlikely (!c->serializer->extend_min (subtable))) return_trace (خطأ);

    خپلکاری *old_subtable = get_subtable (بنسټ);
    خپلکاری *old_سرلیک = old_subtable->get_سرلیک ();
    بینښه صحيح image_format = old_سرلیک->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_ویکټور_t<حرف> معلومات;
    hb_ویکټور_t<بینښه صحيح> lengths;
    بینښه صحيح عدد_glyphs = 0;
    منطقي early_exit = خطأ;
    لكل (بینښه صحيح i = *پیل; i < lookup->اوږدتیا; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].لومړی;
      ثابت شاخص_Subtable_ریکارډ *بل_ریکارډ = (*lookup)[i].second;
      ثابت شاخص_Subtable *بل_subtable = بل_ریکارډ->get_subtable (بنسټ);
      خپلکاری *بل_سرلیک = بل_subtable->get_سرلیک ();
      إذا (بل_سرلیک != old_سرلیک)
      {
	*پیل = i;
	early_exit = ریښتیا;
	اكسر;
      }
      بینښه صحيح عدد_missing = ریکارډ->جمع_کړه_glyph_for_subset (new_gid);
      لكل (بینښه صحيح j = 0; j < عدد_missing; j++)
	إذا (unlikely (!c->serializer->check_success (lengths.ټېل_or_fail (0))))
	  return_trace (خطأ);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      إذا (old_gid < بل_ریکارډ->لومړی_Glyph_شاخص)
	return_trace (خطأ);

      بینښه صحيح old_idx = (بینښه صحيح) old_gid - بل_ریکارډ->لومړی_Glyph_شاخص;
      بینښه صحيح دننه_واټن, اوږدتیا, format;
      إذا (unlikely (!بل_subtable->get_image_معلومات (old_idx, &دننه_واټن, &اوږدتیا, &format)))
	return_trace (خطأ);
      إذا (unlikely (دننه_واټن > bitmap_اندازه_context->cbdt_اوږدتیا ||
		    bitmap_اندازه_context->cbdt_اوږدتیا - دننه_واټن < اوږدتیا))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (
			copy_معلومات_منزل_cbdt (&معلومات,
					    bitmap_اندازه_context->cbdt + دننه_واټن,
					    اوږدتیا))))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (lengths.ټېل_or_fail (اوږدتیا))))
	return_trace (خطأ);
      عدد_glyphs += 1 + عدد_missing;
    }
    إذا (!early_exit)
      *پیل = lookup->اوږدتیا;

    /* Find or append the data block for this subtable. */
    بینښه صحيح image_معلومات_دننه_واټن_2;
    cbdt_dedup_context_t *dedup = bitmap_اندازه_context->dedup;
    منطقي shared = خطأ;
    uint64_t key = 0;
    إذا (dedup && معلومات.اوږدتیا)
    {
      key = cbdt_dedup_context_t::hash (image_format, معلومات, lengths);
      shared = dedup->find (key, معلومات, lengths,
			    *bitmap_اندازه_context->cbdt_اولي,
			    &image_معلومات_دننه_واټن_2);
    }
    إذا (!shared)
    {
      image_معلومات_دننه_واټن_2 = bitmap_اندازه_context->cbdt_اولي->اوږدتیا;
      إذا (unlikely (!c->serializer->check_success (
			copy_معلومات_منزل_cbdt (bitmap_اندازه_context->cbdt_اولي,
					    معلومات.arrayZ, معلومات.اوږدتیا))))
	return_trace (خطأ);
      إذا (dedup && معلومات.اوږدتیا)
	/* The bitmap data is already copied; caching is best-effort. */
	(فراغ) dedup->جمع_کړه (key, image_معلومات_دننه_واټن_2, lengths);
    }

    subtable->populate_سرلیک (old_سرلیک->شاخص_Format,
			       image_format,
			       image_معلومات_دننه_واټن_2,
			       &bitmap_اندازه_context->اندازه);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    بینښه صحيح cumulative = 0;
    لكل (بینښه صحيح اوږدتیا : lengths)
    {
      إذا (unlikely (!subtable->جمع_کړه_دننه_واټن (c->serializer, cumulative,
					   &bitmap_اندازه_context->اندازه)))
	return_trace (خطأ);
      cumulative += اوږدتیا;
    }
    إذا (unlikely (!subtable->پای_subtable (c->serializer,
					      cumulative,
					      عدد_glyphs,
					      &bitmap_اندازه_context->اندازه)))
      return_trace (خطأ);
    return_trace (ریښتیا);
  }

  منطقي جمع_کړه_new_ریکارډ (hb_subset_context_t *c,
		       cblc_bitmap_اندازه_subset_context_t *bitmap_اندازه_context,
		       ثابت hb_ویکټور_t<hb_pair_t<hb_codepoint_t, ثابت شاخص_Subtable_ریکارډ*>> *lookup, /*دننه*/
		       ثابت فراغ *بنسټ,
		       بینښه صحيح *پیل, /* INOUT */
		       hb_ویکټور_t<شاخص_Subtable_ریکارډ>* ریکارډونه /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری snap = c->serializer->snapshot ();
    بینښه صحيح old_اندازه = bitmap_اندازه_context->اندازه;
    بینښه صحيح old_cbdt_اولي_اوږدتیا = bitmap_اندازه_context->cbdt_اولي->اوږدتیا;

    // Set to invalid state to indicate filling glyphs is not yet started.
    إذا (unlikely (!c->serializer->check_success (ریکارډونه->resize (ریکارډونه->اوږدتیا + 1))))
      return_trace (خطأ);

    ریکارډونه->لکۍ ().لومړی_Glyph_شاخص = 1;
    ریکارډونه->لکۍ ().وروستی_Glyph_شاخص = 0;
    bitmap_اندازه_context->اندازه += شاخص_Subtable_ریکارډ::min_اندازه;

    c->serializer->push ();

    إذا (unlikely (!جمع_کړه_new_subtable (c, bitmap_اندازه_context, &(ریکارډونه->لکۍ ()), lookup, بنسټ, پیل)))
    {
      c->serializer->راوباسه_discard ();
      c->serializer->revert (snap);
      bitmap_اندازه_context->cbdt_اولي->shrink (old_cbdt_اولي_اوږدتیا);
      bitmap_اندازه_context->اندازه = old_اندازه;
      ریکارډونه->resize (ریکارډونه->اوږدتیا - 1);
      return_trace (خطأ);
    }

    bitmap_اندازه_context->عدد_tables += 1;
    return_trace (ریښتیا);
  }

  بینښه صحيح جمع_کړه_glyph_for_subset (hb_codepoint_t gid)
  {
    إذا (لومړی_Glyph_شاخص > وروستی_Glyph_شاخص)
    {
      لومړی_Glyph_شاخص = gid;
      وروستی_Glyph_شاخص = gid;
      أرجع 0;
    }
    // TODO maybe assert? this shouldn't occur.
    إذا (وروستی_Glyph_شاخص > gid)
      أرجع 0;
    بینښه صحيح عدد_missing = (بینښه صحيح) (gid - وروستی_Glyph_شاخص - 1);
    وروستی_Glyph_شاخص = gid;
    أرجع عدد_missing;
  }

  منطقي get_extents (hb_glyph_extents_t *extents, ثابت فراغ *بنسټ, منطقي scale) ثابت
  { أرجع (بنسټ+دننه_واټن_منزل_Subtable).get_extents (extents, scale); }

  منطقي get_image_معلومات (بینښه صحيح  gid,
		       ثابت فراغ   *بنسټ,
		       بینښه صحيح *دننه_واټن,
		       بینښه صحيح *اوږدتیا,
		       بینښه صحيح *format) ثابت
  {
    إذا (gid < لومړی_Glyph_شاخص || gid > وروستی_Glyph_شاخص) أرجع خطأ;
    أرجع (بنسټ+دننه_واټن_منزل_Subtable).get_image_معلومات (gid - لومړی_Glyph_شاخص,
						   دننه_واټن, اوږدتیا, format);
  }

  HBGlyphID16			لومړی_Glyph_شاخص;
  HBGlyphID16			وروستی_Glyph_شاخص;
  Offset32_منزل<شاخص_Subtable>	دننه_واټن_منزل_Subtable;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية شاخص_Subtable_Array
{
  ملګری بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح شمېر) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (شاخص_Subtables_Z.sanitize (c, شمېر, هذا));
  }

  فراغ
  build_lookup (hb_subset_context_t *c, cblc_bitmap_اندازه_subset_context_t *bitmap_اندازه_context,
		hb_ویکټور_t<hb_pair_t<hb_codepoint_t,
		ثابت شاخص_Subtable_ریکارډ*>> *lookup /*بهر*/) ثابت
  {
    منطقي پیل_glyph_is_سېټ = خطأ;
    بینښه عدد_glyphs = c->plan->عدد_output_glyphs ();
    لكل (hb_codepoint_t new_gid = 0; new_gid < عدد_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) استمر;

      ثابت شاخص_Subtable_ریکارډ* ریکارډ = پیدا_کړه_جدول (old_gid, bitmap_اندازه_context->عدد_tables);
      إذا (unlikely (!ریکارډ)) استمر;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      بینښه صحيح دننه_واټن, اوږدتیا, format;
      إذا (unlikely (!ریکارډ->get_image_معلومات (old_gid, هذا, &دننه_واټن, &اوږدتیا, &format))) استمر;

      lookup->push (hb_pair_t<hb_codepoint_t, ثابت شاخص_Subtable_ریکارډ*> (new_gid, ریکارډ));

      إذا (!پیل_glyph_is_سېټ)
      {
	bitmap_اندازه_context->پیل_glyph = new_gid;
	پیل_glyph_is_سېټ = ریښتیا;
      }

      bitmap_اندازه_context->پای_ټکی_glyph = new_gid;
    }
  }

  منطقي
  subset (hb_subset_context_t *c,
	  cblc_bitmap_اندازه_subset_context_t *bitmap_اندازه_context) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_ویکټور_t<hb_pair_t<hb_codepoint_t, ثابت شاخص_Subtable_ریکارډ*>> lookup;
    build_lookup (c, bitmap_اندازه_context, &lookup);
    إذا (unlikely (!c->serializer->propagate_error (lookup)))
      أرجع خطأ;

    bitmap_اندازه_context->اندازه = 0;
    bitmap_اندازه_context->عدد_tables = 0;
    hb_ویکټور_t<شاخص_Subtable_ریکارډ> ریکارډونه;
    لكل (بینښه صحيح پیل = 0; پیل < lookup.اوږدتیا;)
    {
      إذا (unlikely (!lookup[پیل].second->جمع_کړه_new_ریکارډ (c, bitmap_اندازه_context, &lookup, هذا, &پیل, &ریکارډونه)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	لكل (بینښه صحيح i = 0; i < ریکارډونه.اوږدتیا; i++)
	  c->serializer->راوباسه_discard ();
	return_trace (خطأ);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_ویکټور_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (بینښه صحيح i = 0; i < ریکارډونه.اوږدتیا; i++)
      objidxs.push (c->serializer->راوباسه_pack ());
    لكل (بینښه صحيح i = 0; i < ریکارډونه.اوږدتیا; i++)
    {
      شاخص_Subtable_ریکارډ* ریکارډ = c->serializer->embed (ریکارډونه[i]);
      إذا (unlikely (!ریکارډ)) return_trace (خطأ);
      c->serializer->جمع_کړه_link (ریکارډ->دننه_واټن_منزل_Subtable, objidxs[ریکارډونه.اوږدتیا - 1 - i]);
    }
    return_trace (ریښتیا);
  }

  عام:
  ثابت شاخص_Subtable_ریکارډ* پیدا_کړه_جدول (hb_codepoint_t glyph, بینښه صحيح عدد_Tables) ثابت
  {
    لكل (بینښه صحيح i = 0; i < عدد_Tables; ++i)
    {
      بینښه صحيح لومړی_Glyph_شاخص = شاخص_Subtables_Z[i].لومړی_Glyph_شاخص;
      بینښه صحيح وروستی_Glyph_شاخص = شاخص_Subtables_Z[i].وروستی_Glyph_شاخص;
      إذا (لومړی_Glyph_شاخص <= glyph && glyph <= وروستی_Glyph_شاخص)
	أرجع &شاخص_Subtables_Z[i];
    }
    أرجع تش_اشاره;
  }

  محمي:
  Unsized_Array_د<شاخص_Subtable_ریکارډ>	شاخص_Subtables_Z;
};

بنية Bitmap_اندازه_جدول
{
  ملګری بنية CBLC;
  ملګری بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  شاخص_Subtable_Array_دننه_واټن.sanitize (c, بنسټ, شمېره_د_شاخص_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ثابت شاخص_Subtable_ریکارډ *
  پیدا_کړه_جدول (hb_codepoint_t glyph, ثابت فراغ *بنسټ, ثابت فراغ **بهر_بنسټ) ثابت
  {
    *بهر_بنسټ = &(بنسټ+شاخص_Subtable_Array_دننه_واټن);
    أرجع (بنسټ+شاخص_Subtable_Array_دننه_واټن).پیدا_کړه_جدول (glyph, شمېره_د_شاخص_Subtables);
  }

  منطقي
  subset (hb_subset_context_t *c, ثابت فراغ *بنسټ,
	  ثابت حرف *cbdt, بینښه صحيح cbdt_اوږدتیا,
	  hb_ویکټور_t<حرف> *cbdt_اولي /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر_جدول = c->serializer->embed (هذا);
    إذا (unlikely (!بهر_جدول)) return_trace (خطأ);

    cblc_bitmap_اندازه_subset_context_t bitmap_اندازه_context;
    bitmap_اندازه_context.cbdt = cbdt;
    bitmap_اندازه_context.cbdt_اوږدتیا = cbdt_اوږدتیا;
    bitmap_اندازه_context.cbdt_اولي = cbdt_اولي;
    bitmap_اندازه_context.اندازه = شاخص_Tables_اندازه;
    bitmap_اندازه_context.عدد_tables = شمېره_د_شاخص_Subtables;
    bitmap_اندازه_context.پیل_glyph = 1;
    bitmap_اندازه_context.پای_ټکی_glyph = 0;
    bitmap_اندازه_context.dedup = dedup;

    إذا (!بهر_جدول->شاخص_Subtable_Array_دننه_واټن.serialize_subset (c,
							       شاخص_Subtable_Array_دننه_واټن,
							       بنسټ,
							       &bitmap_اندازه_context))
      return_trace (خطأ);
    إذا (!bitmap_اندازه_context.اندازه ||
	!bitmap_اندازه_context.عدد_tables ||
	bitmap_اندازه_context.پیل_glyph > bitmap_اندازه_context.پای_ټکی_glyph)
      return_trace (خطأ);

    بهر_جدول->شاخص_Tables_اندازه = bitmap_اندازه_context.اندازه;
    بهر_جدول->شمېره_د_شاخص_Subtables = bitmap_اندازه_context.عدد_tables;
    بهر_جدول->پیل_Glyph_شاخص = bitmap_اندازه_context.پیل_glyph;
    بهر_جدول->پای_ټکی_Glyph_شاخص = bitmap_اندازه_context.پای_ټکی_glyph;
    return_trace (ریښتیا);
  }

  محمي:
  NNOffset32_منزل<شاخص_Subtable_Array>
			شاخص_Subtable_Array_دننه_واټن;
  HBUINT32		شاخص_Tables_اندازه;
  HBUINT32		شمېره_د_شاخص_Subtables;
  HBUINT32		colorRef;
  SBit_کرښه_Metrics	horizontal;
  SBit_کرښه_Metrics	vertical;
  HBGlyphID16		پیل_Glyph_شاخص;
  HBGlyphID16		پای_ټکی_Glyph_شاخص;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		بټ_ژوروالی;
  HBINT8		flags;
  عام:
  DEFINE_اندازه_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

بنية Glyph_Bitmap_معلومات_Format17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32_د<HBUINT8>	معلومات;
  عام:
  DEFINE_اندازه_ARRAY (9, معلومات);
};

بنية Glyph_Bitmap_معلومات_Format18
{
  BigGlyphMetrics	glyphMetrics;
  Array32_د<HBUINT8>	معلومات;
  عام:
  DEFINE_اندازه_ARRAY (12, معلومات);
};

بنية Glyph_Bitmap_معلومات_Format19
{
  Array32_د<HBUINT8>	معلومات;
  عام:
  DEFINE_اندازه_ARRAY (4, معلومات);
};

بنية CBLC
{
  ملګری بنية CBDT;

  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_CBLC;

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
  sink_cbdt (hb_subset_context_t *c, hb_ویکټور_t<حرف>* cbdt_اولي)
  {
    hb_blob_t *cbdt_اولي_blob = hb_blob_create (cbdt_اولي->arrayZ,
						 cbdt_اولي->اوږدتیا,
						 HB_حافظه_ډېرپېښ_WRITABLE,
						 cbdt_اولي->arrayZ,
						 hb_free);
    cbdt_اولي->init ();  // Leak arrayZ to the blob.
    منطقي ret = c->plan->جمع_کړه_جدول (HB_OT_نښان_CBDT, cbdt_اولي_blob);
    hb_blob_destroy (cbdt_اولي_blob);
    أرجع ret;
  }

  منطقي
  subset_اندازه_جدول (hb_subset_context_t *c, ثابت Bitmap_اندازه_جدول& جدول,
		     ثابت حرف *cbdt /*دننه*/, بینښه صحيح cbdt_اوږدتیا,
		     CBLC *cblc_اولي /* INOUT */, hb_ویکټور_t<حرف> *cbdt_اولي /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    cblc_اولي->اندازه_Tables.len++;

    خپلکاری snap = c->serializer->snapshot ();
    خپلکاری cbdt_اولي_len = cbdt_اولي->اوږدتیا;

    إذا (!جدول.subset (c, هذا, cbdt, cbdt_اوږدتیا, cbdt_اولي, dedup))
    {
      cblc_اولي->اندازه_Tables.len--;
      c->serializer->revert (snap);
      cbdt_اولي->shrink (cbdt_اولي_len);
      return_trace (خطأ);
    }
    return_trace (ریښتیا);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_دننی منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  ثابت Bitmap_اندازه_جدول &ټاکنه_strike (hb_font_t *font) ثابت
  {
    بینښه شمېر = اندازه_Tables.len;
    إذا (unlikely (!شمېر))
      أرجع Null (Bitmap_اندازه_جدول);

    بینښه صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    إذا (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    بینښه صحيح غوره_i = 0;
    بینښه صحيح غوره_ppem = hb_max (اندازه_Tables[0].ppemX, اندازه_Tables[0].ppemY);

    لكل (بینښه صحيح i = 1; i < شمېر; i++)
    {
      بینښه صحيح ppem = hb_max (اندازه_Tables[i].ppemX, اندازه_Tables[i].ppemY);
      إذا ((requested_ppem <= ppem && ppem < غوره_ppem) ||
	  (requested_ppem > غوره_ppem && ppem > غوره_ppem))
      {
	غوره_i = i;
	غوره_ppem = ppem;
      }
    }

    أرجع اندازه_Tables[غوره_i];
  }

  محمي:
  Fixed_نسخه<>		نسخه;
  Array32_د<Bitmap_اندازه_جدول>	اندازه_Tables;
  عام:
  DEFINE_اندازه_ARRAY (8, اندازه_Tables);
};

بنية CBDT
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_CBDT;

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
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, منطقي scale = ریښتیا) ثابت
    {
      ثابت فراغ *بنسټ;
      ثابت Bitmap_اندازه_جدول &strike = هذا->cblc->ټاکنه_strike (font);
      ثابت شاخص_Subtable_ریکارډ *subtable_ریکارډ = strike.پیدا_کړه_جدول (glyph, cblc, &بنسټ);
      إذا (!subtable_ریکارډ || !strike.ppemX || !strike.ppemY)
	أرجع خطأ;

      إذا (subtable_ریکارډ->get_extents (extents, بنسټ, scale))
	أرجع ریښتیا;

      بینښه صحيح image_دننه_واټن = 0, image_اوږدتیا = 0, image_format = 0;
      إذا (!subtable_ریکارډ->get_image_معلومات (glyph, بنسټ, &image_دننه_واټن, &image_اوږدتیا, &image_format))
	أرجع خطأ;

      بینښه صحيح cbdt_len = cbdt.get_اوږدتیا ();
      إذا (unlikely (image_دننه_واټن > cbdt_len || cbdt_len - image_دننه_واټن < image_اوږدتیا))
	أرجع خطأ;

      بدل (image_format)
      {
      حالة 17: {
	إذا (unlikely (image_اوږدتیا < Glyph_Bitmap_معلومات_Format17::min_اندازه))
	  أرجع خطأ;
	خپلکاری &glyphFormat17 = Struct_At_دننه_واټن<Glyph_Bitmap_معلومات_Format17> (هذا->cbdt, image_دننه_واټن);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	اكسر;
      }
      حالة 18: {
	إذا (unlikely (image_اوږدتیا < Glyph_Bitmap_معلومات_Format18::min_اندازه))
	  أرجع خطأ;
	خپلکاری &glyphFormat18 = Struct_At_دننه_واټن<Glyph_Bitmap_معلومات_Format18> (هذا->cbdt, image_دننه_واټن);
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
	extents->پلنوالی = hb_clamp_منزل<hb_position_t> (roundf (extents->پلنوالی * x_scale));
	extents->لوړوالی = hb_clamp_منزل<hb_position_t> (roundf (extents->لوړوالی * y_scale));
      }

      أرجع ریښتیا;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ثابت
    {
      ثابت فراغ *بنسټ;
      ثابت Bitmap_اندازه_جدول &strike = هذا->cblc->ټاکنه_strike (font);
      ثابت شاخص_Subtable_ریکارډ *subtable_ریکارډ = strike.پیدا_کړه_جدول (glyph, cblc, &بنسټ);
      إذا (!subtable_ریکارډ || !strike.ppemX || !strike.ppemY)
	أرجع hb_blob_get_تشه ();

      بینښه صحيح image_دننه_واټن = 0, image_اوږدتیا = 0, image_format = 0;
      إذا (!subtable_ریکارډ->get_image_معلومات (glyph, بنسټ, &image_دننه_واټن, &image_اوږدتیا, &image_format))
	أرجع hb_blob_get_تشه ();

      بینښه صحيح cbdt_len = cbdt.get_اوږدتیا ();
      إذا (unlikely (image_دننه_واټن > cbdt_len || cbdt_len - image_دننه_واټن < image_اوږدتیا))
	أرجع hb_blob_get_تشه ();

      بدل (image_format)
      {
      حالة 17:
      {
	إذا (unlikely (image_اوږدتیا < Glyph_Bitmap_معلومات_Format17::min_اندازه))
	  أرجع hb_blob_get_تشه ();
	خپلکاری &glyphFormat17 = Struct_At_دننه_واټن<Glyph_Bitmap_معلومات_Format17> (هذا->cbdt, image_دننه_واټن);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_دننه_واټن + Glyph_Bitmap_معلومات_Format17::min_اندازه,
					glyphFormat17.معلومات.len);
      }
      حالة 18:
      {
	إذا (unlikely (image_اوږدتیا < Glyph_Bitmap_معلومات_Format18::min_اندازه))
	  أرجع hb_blob_get_تشه ();
	خپلکاری &glyphFormat18 = Struct_At_دننه_واټن<Glyph_Bitmap_معلومات_Format18> (هذا->cbdt, image_دننه_واټن);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_دننه_واټن + Glyph_Bitmap_معلومات_Format18::min_اندازه,
					glyphFormat18.معلومات.len);
      }
      حالة 19:
      {
	إذا (unlikely (image_اوږدتیا < Glyph_Bitmap_معلومات_Format19::min_اندازه))
	  أرجع hb_blob_get_تشه ();
	خپلکاری &glyphFormat19 = Struct_At_دننه_واټن<Glyph_Bitmap_معلومات_Format19> (هذا->cbdt, image_دننه_واټن);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_دننه_واټن + Glyph_Bitmap_معلومات_Format19::min_اندازه,
					glyphFormat19.معلومات.len);
      }
      افتراضي: أرجع hb_blob_get_تشه (); /* TODO: Support other image formats. */
      }
    }

    منطقي has_معلومات () ثابت { أرجع cbdt->نسخه.major; }

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
			       pixel_extents.پلنوالی, -pixel_extents.لوړوالی,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    بینښه صحيح upem;
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
  Unsized_Array_د<HBUINT8>	معلومات_Z;
  عام:
  DEFINE_اندازه_ARRAY (4, معلومات_Z);
};

مضمن منطقي
CBLC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_ویکټور_t<حرف> cbdt_اولي;

  خپلکاری *cblc_اولي = c->serializer->پیل_embed<CBLC> ();
  إذا (unlikely (!c->serializer->extend_min (cblc_اولي))) return_trace (خطأ);
  cblc_اولي->نسخه = نسخه;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_جدول<CBDT> (c->plan->سرچینه);
  بینښه صحيح cbdt_اوږدتیا;
  CBDT* cbdt = (CBDT *) hb_blob_get_معلومات (cbdt_blob, &cbdt_اوږدتیا);
  إذا (unlikely (cbdt_اوږدتیا < CBDT::min_اندازه))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (خطأ);
  }
  copy_معلومات_منزل_cbdt (&cbdt_اولي, cbdt, CBDT::min_اندازه);

  cbdt_dedup_context_t dedup;
  لكل (ثابت Bitmap_اندازه_جدول& جدول : + اندازه_Tables.iter ())
    subset_اندازه_جدول (c, جدول, (ثابت حرف *) cbdt, cbdt_اوږدتیا, cblc_اولي, &cbdt_اولي, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_اولي));
}

بنية CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#د_که_پای /* OT_COLOR_CBDT_CBDT_HH */
