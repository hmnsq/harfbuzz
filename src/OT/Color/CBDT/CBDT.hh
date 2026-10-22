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

#جے_بنا_تعریف OT_COLOR_CBDT_CBDT_HH
#تعریف_کرو OT_COLOR_CBDT_CBDT_HH

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
#تعریف_کرو HB_OT_ٹیگ_CBLC HB_ٹیگ('C','B','L','C')
#تعریف_کرو HB_OT_ٹیگ_CBDT HB_ٹیگ('C','B','D','T')


نطاق OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
بنية cbdt_dedup_context_t
{
  بنية اندراج_t
  {
    بنانشانی صحيح ادھار;		/* Offset of the data in cbdt_prime. */
    بنانشانی صحيح lstart;	/* Start index in lengths. */
    بنانشانی صحيح lcount;	/* Number of length entries. */
  };

  ساكن uint64_t hash (بنانشانی صحيح image_format,
			ثابت hb_سمتیہ_t<حرف> &انکڑے,
			ثابت hb_سمتیہ_t<بنانشانی صحيح> &lengths)
  {
    uint32_t h = hb_hash (hb_بائٹاں_t (انکڑے.arrayZ, انکڑے.لمائی));
    h = h * 31 + hb_hash (hb_بائٹاں_t ((ثابت حرف *) lengths.arrayZ,
				      lengths.لمائی * حجم (بنانشانی صحيح)));
    أرجع ((uint64_t) image_format << 32) | h;
  }

  منطقي find (uint64_t key,
	     ثابت hb_سمتیہ_t<حرف> &انکڑے,
	     ثابت hb_سمتیہ_t<بنانشانی صحيح> &lengths,
	     ثابت hb_سمتیہ_t<حرف> &cbdt_مفرد,
	     بنانشانی صحيح *ادھار /*باہرلا*/) ثابت
  {
    بنانشانی صحيح *idx_p = خالی_سوچک;
    إذا (!map.has (key, &idx_p)) أرجع خطأ;
    ثابت اندراج_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    إذا (e.lcount != lengths.لمائی) أرجع خطأ;
    إذا (unlikely (e.ادھار + انکڑے.لمائی > (بنانشانی صحيح) cbdt_مفرد.لمائی)) أرجع خطأ;
    إذا (hb_memcmp (cbdt_مفرد.arrayZ + e.ادھار, انکڑے.arrayZ, انکڑے.لمائی)) أرجع خطأ;
    إذا (hb_memcmp (هذا->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.لمائی * حجم (بنانشانی صحيح)))
      أرجع خطأ;
    *ادھار = e.ادھار;
    أرجع سچ;
  }

  منطقي جوڑو (uint64_t key,
	    بنانشانی صحيح ادھار,
	    ثابت hb_سمتیہ_t<بنانشانی صحيح> &lengths)
  {
    إذا (unlikely (entries.وچ_error () || هذا->lengths.وچ_error () || map.وچ_error ()))
      أرجع خطأ;
    اندراج_t e = {ادھار, هذا->lengths.لمائی, lengths.لمائی};
    إذا (unlikely (!entries.ڈالو_or_fail (e))) أرجع خطأ;
    لكل (بنانشانی صحيح لمائی : lengths)
      إذا (unlikely (!هذا->lengths.ڈالو_or_fail (لمائی))) أرجع خطأ;
    أرجع map.سیٹ (key, entries.لمائی - 1);
  }

  hb_hashmap_t<uint64_t, بنانشانی صحيح> map;
  hb_سمتیہ_t<اندراج_t> entries;
  hb_سمتیہ_t<بنانشانی صحيح> lengths; /* Concatenated per-entry lengths. */
};

بنية cblc_bitmap_ناپ_subset_context_t
{
  ثابت حرف *cbdt;
  بنانشانی صحيح cbdt_لمائی;
  hb_سمتیہ_t<حرف> *cbdt_مفرد;
  بنانشانی صحيح ناپ;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  بنانشانی صحيح عدد_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t شروع_glyph;	/*باہرلا*/
  hb_codepoint_t سرا_glyph;	/*باہرلا*/
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ساكن مضمن منطقي
copy_انکڑے_منزل_cbdt (hb_سمتیہ_t<حرف> *cbdt_مفرد,
		    ثابت فراغ        *انکڑے,
		    بنانشانی           لمائی)
{
  بنانشانی صحيح new_len = cbdt_مفرد->لمائی + لمائی;
  إذا (unlikely (!cbdt_مفرد->alloc (new_len))) أرجع خطأ;
  hb_memcpy (cbdt_مفرد->arrayZ + cbdt_مفرد->لمائی, انکڑے, لمائی);
  cbdt_مفرد->لمائی = new_len;
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
    extents->چوڑائی = چوڑائی;
    extents->اچائی = -static_cast<صحيح> (اچائی);

    إذا (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	اچائی;
  HBUINT8	چوڑائی;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  عام:
  DEFINE_ناپ_STATIC (5);
};

بنية BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  عام:
  DEFINE_ناپ_STATIC (8);
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
  HBUINT8	چوڑائی_Max;
  HBINT8	caret_ڈھلوان_Numerator;
  HBINT8	caret_ڈھلوان_Denominator;
  HBINT8	caret_وتھ;
  HBINT8	minOriginSB;
  HBINT8	minAdvanceSB;
  HBINT8	maxBeforeBL;
  HBINT8	minAfterBL;
  HBINT8	padding1;
  HBINT8	padding2;
  عام:
  DEFINE_ناپ_STATIC (12);
};


/*
 * Index Subtables.
 */

بنية اشاریہ_Subtable_سرخی
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	اشاریہ_Format;
  HBUINT16	imageFormat;
  HBUINT32	image_انکڑے_وتھ;
  عام:
  DEFINE_ناپ_STATIC (8);
};

سانچا <قسم_ناں وتھ_Type>
بنية اشاریہ_Subtable_Format1_Or3
{
  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح glyph_گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  وتھ_Array_Z.sanitize (c, glyph_گنتی + 1));
  }

  منطقي get_image_انکڑے (بنانشانی صحيح idx,
		       بنانشانی صحيح *وتھ,
		       بنانشانی صحيح *لمائی) ثابت
  {
    إذا (unlikely (وتھ_Array_Z[idx + 1] <= وتھ_Array_Z[idx]))
      أرجع خطأ;

    *وتھ = سرخی.image_انکڑے_وتھ + وتھ_Array_Z[idx];
    *لمائی = وتھ_Array_Z[idx + 1] - وتھ_Array_Z[idx];
    أرجع سچ;
  }

  منطقي جوڑو_وتھ (hb_serialize_context_t *c,
		   بنانشانی صحيح وتھ,
		   بنانشانی صحيح *ناپ /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    وتھ_2<وتھ_Type> embedded_وتھ;
    embedded_وتھ = وتھ;
    *ناپ += حجم (وتھ_Type);
    خودکار *o = c->embed (embedded_وتھ);
    return_trace ((منطقي) o);
  }

  اشاریہ_Subtable_سرخی	سرخی;
  Unsized_Array_دا<وتھ_2<وتھ_Type>>
			وتھ_Array_Z;
  عام:
  DEFINE_ناپ_ARRAY (8, وتھ_Array_Z);
};

بنية اشاریہ_Subtable_Format1 : اشاریہ_Subtable_Format1_Or3<HBUINT32> {};
بنية اشاریہ_Subtable_Format3 : اشاریہ_Subtable_Format1_Or3<HBUINT16> {};

بنية اشاریہ_Subtable
{
  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح glyph_گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.سرخی.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.سرخی.اشاریہ_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_گنتی));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_گنتی));
    افتراضي:return_trace (سچ);
    }
  }

  منطقي
  انت_subtable (hb_serialize_context_t *c,
		   بنانشانی صحيح سرا_وتھ,
		   بنانشانی صحيح عدد_glyphs,
		   بنانشانی صحيح *ناپ /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);

    بدل (u.سرخی.اشاریہ_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.جوڑو_وتھ (c, سرا_وتھ, ناپ));
    حالة 3: {
      hb_barrier ();
      إذا (!u.format3.جوڑو_وتھ (c, سرا_وتھ, ناپ))
	return_trace (خطأ);
      إذا (!(عدد_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.جوڑو_وتھ (c, 0, ناپ));
      return_trace (سچ);
    }
    // TODO: implement 2, 4, 5.
    حالة 2: حالة 4:  // No-op.
    حالة 5:  // Pad to 32-bit aligned.
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي
  جوڑو_وتھ (hb_serialize_context_t *c, بنانشانی صحيح local_وتھ,
	      بنانشانی صحيح *ناپ /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.سرخی.اشاریہ_Format)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.جوڑو_وتھ (c, local_وتھ, ناپ));
    حالة 3: hb_barrier (); return_trace (u.format3.جوڑو_وتھ (c, local_وتھ, ناپ));
    // TODO: Implement tables 2, 4, 5
    حالة 2:  // Should be a no-op.
    حالة 4: حالة 5:  // Handle sparse cases.
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents HB_بن_ورتیا, منطقي scale HB_بن_ورتیا) ثابت
  {
    بدل (u.سرخی.اشاریہ_Format)
    {
    حالة 2: حالة 5: /* TODO */
    حالة 1: حالة 3: حالة 4: /* Variable-metrics formats do not have metrics here. */
    افتراضي:أرجع (خطأ);
    }
  }

  منطقي
  get_image_انکڑے (بنانشانی صحيح idx, بنانشانی صحيح *وتھ,
		  بنانشانی صحيح *لمائی, بنانشانی صحيح *format) ثابت
  {
    *format = u.سرخی.imageFormat;
    بدل (u.سرخی.اشاریہ_Format)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_image_انکڑے (idx, وتھ, لمائی);
    حالة 3: hb_barrier (); أرجع u.format3.get_image_انکڑے (idx, وتھ, لمائی);
    افتراضي: أرجع خطأ;
    }
  }

  ثابت اشاریہ_Subtable_سرخی* get_سرخی () ثابت { أرجع &u.سرخی; }

  فراغ populate_سرخی (بنانشانی اشاریہ_format,
			بنانشانی image_format,
			بنانشانی صحيح image_انکڑے_وتھ_2,
			بنانشانی صحيح *ناپ)
  {
    u.سرخی.اشاریہ_Format = اشاریہ_format;
    u.سرخی.imageFormat = image_format;
    u.سرخی.image_انکڑے_وتھ = image_انکڑے_وتھ_2;
    بدل (u.سرخی.اشاریہ_Format)
    {
    حالة 1: *ناپ += اشاریہ_Subtable_Format1::min_ناپ; اكسر;
    حالة 3: *ناپ += اشاریہ_Subtable_Format3::min_ناپ; اكسر;
    }
  }

  محمي:
  اتحاد {
  اشاریہ_Subtable_سرخی	سرخی;
  اشاریہ_Subtable_Format1	format1;
  اشاریہ_Subtable_Format3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  عام:
  DEFINE_ناپ_UNION (8, سرخی);
};

بنية اشاریہ_Subtable_ریکارڈ
{
  /* XXX Remove this and fix by not inserting it into vector. */
  اشاریہ_Subtable_ریکارڈ& عامل = (ثابت اشاریہ_Subtable_ریکارڈ &o)
  {
    پہلا_Glyph_اشاریہ = o.پہلا_Glyph_اشاریہ;
    آخری_Glyph_اشاریہ = o.آخری_Glyph_اشاریہ;
    وتھ_منزل_Subtable = (بنانشانی) o.وتھ_منزل_Subtable;
    أكد (وتھ_منزل_Subtable.is_null ());
    أرجع *هذا;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  پہلا_Glyph_اشاریہ <= آخری_Glyph_اشاریہ &&
		  وتھ_منزل_Subtable.sanitize (c, ادھار, آخری_Glyph_اشاریہ - پہلا_Glyph_اشاریہ + 1));
  }

  ثابت اشاریہ_Subtable* get_subtable (ثابت فراغ *ادھار) ثابت
  {
    أرجع &(ادھار+وتھ_منزل_Subtable);
  }

  منطقي جوڑو_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_ناپ_subset_context_t *bitmap_ناپ_context,
			 اشاریہ_Subtable_ریکارڈ *ریکارڈ,
			 ثابت hb_سمتیہ_t<hb_pair_t<hb_codepoint_t, ثابت اشاریہ_Subtable_ریکارڈ*>> *lookup, /*وچ*/
			 ثابت فراغ *ادھار,
			 بنانشانی صحيح *شروع /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خودکار *subtable = c->serializer->شروع_embed<اشاریہ_Subtable> ();
    إذا (unlikely (!c->serializer->extend_min (subtable))) return_trace (خطأ);

    خودکار *old_subtable = get_subtable (ادھار);
    خودکار *old_سرخی = old_subtable->get_سرخی ();
    بنانشانی صحيح image_format = old_سرخی->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_سمتیہ_t<حرف> انکڑے;
    hb_سمتیہ_t<بنانشانی صحيح> lengths;
    بنانشانی صحيح عدد_glyphs = 0;
    منطقي early_exit = خطأ;
    لكل (بنانشانی صحيح i = *شروع; i < lookup->لمائی; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].پہلا;
      ثابت اشاریہ_Subtable_ریکارڈ *اگلا_ریکارڈ = (*lookup)[i].second;
      ثابت اشاریہ_Subtable *اگلا_subtable = اگلا_ریکارڈ->get_subtable (ادھار);
      خودکار *اگلا_سرخی = اگلا_subtable->get_سرخی ();
      إذا (اگلا_سرخی != old_سرخی)
      {
	*شروع = i;
	early_exit = سچ;
	اكسر;
      }
      بنانشانی صحيح عدد_missing = ریکارڈ->جوڑو_glyph_for_subset (new_gid);
      لكل (بنانشانی صحيح j = 0; j < عدد_missing; j++)
	إذا (unlikely (!c->serializer->check_success (lengths.ڈالو_or_fail (0))))
	  return_trace (خطأ);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      إذا (old_gid < اگلا_ریکارڈ->پہلا_Glyph_اشاریہ)
	return_trace (خطأ);

      بنانشانی صحيح old_idx = (بنانشانی صحيح) old_gid - اگلا_ریکارڈ->پہلا_Glyph_اشاریہ;
      بنانشانی صحيح وتھ, لمائی, format;
      إذا (unlikely (!اگلا_subtable->get_image_انکڑے (old_idx, &وتھ, &لمائی, &format)))
	return_trace (خطأ);
      إذا (unlikely (وتھ > bitmap_ناپ_context->cbdt_لمائی ||
		    bitmap_ناپ_context->cbdt_لمائی - وتھ < لمائی))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (
			copy_انکڑے_منزل_cbdt (&انکڑے,
					    bitmap_ناپ_context->cbdt + وتھ,
					    لمائی))))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (lengths.ڈالو_or_fail (لمائی))))
	return_trace (خطأ);
      عدد_glyphs += 1 + عدد_missing;
    }
    إذا (!early_exit)
      *شروع = lookup->لمائی;

    /* Find or append the data block for this subtable. */
    بنانشانی صحيح image_انکڑے_وتھ_2;
    cbdt_dedup_context_t *dedup = bitmap_ناپ_context->dedup;
    منطقي shared = خطأ;
    uint64_t key = 0;
    إذا (dedup && انکڑے.لمائی)
    {
      key = cbdt_dedup_context_t::hash (image_format, انکڑے, lengths);
      shared = dedup->find (key, انکڑے, lengths,
			    *bitmap_ناپ_context->cbdt_مفرد,
			    &image_انکڑے_وتھ_2);
    }
    إذا (!shared)
    {
      image_انکڑے_وتھ_2 = bitmap_ناپ_context->cbdt_مفرد->لمائی;
      إذا (unlikely (!c->serializer->check_success (
			copy_انکڑے_منزل_cbdt (bitmap_ناپ_context->cbdt_مفرد,
					    انکڑے.arrayZ, انکڑے.لمائی))))
	return_trace (خطأ);
      إذا (dedup && انکڑے.لمائی)
	/* The bitmap data is already copied; caching is best-effort. */
	(فراغ) dedup->جوڑو (key, image_انکڑے_وتھ_2, lengths);
    }

    subtable->populate_سرخی (old_سرخی->اشاریہ_Format,
			       image_format,
			       image_انکڑے_وتھ_2,
			       &bitmap_ناپ_context->ناپ);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    بنانشانی صحيح cumulative = 0;
    لكل (بنانشانی صحيح لمائی : lengths)
    {
      إذا (unlikely (!subtable->جوڑو_وتھ (c->serializer, cumulative,
					   &bitmap_ناپ_context->ناپ)))
	return_trace (خطأ);
      cumulative += لمائی;
    }
    إذا (unlikely (!subtable->انت_subtable (c->serializer,
					      cumulative,
					      عدد_glyphs,
					      &bitmap_ناپ_context->ناپ)))
      return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي جوڑو_new_ریکارڈ (hb_subset_context_t *c,
		       cblc_bitmap_ناپ_subset_context_t *bitmap_ناپ_context,
		       ثابت hb_سمتیہ_t<hb_pair_t<hb_codepoint_t, ثابت اشاریہ_Subtable_ریکارڈ*>> *lookup, /*وچ*/
		       ثابت فراغ *ادھار,
		       بنانشانی صحيح *شروع, /* INOUT */
		       hb_سمتیہ_t<اشاریہ_Subtable_ریکارڈ>* ریکارڈاں /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار snap = c->serializer->snapshot ();
    بنانشانی صحيح old_ناپ = bitmap_ناپ_context->ناپ;
    بنانشانی صحيح old_cbdt_مفرد_لمائی = bitmap_ناپ_context->cbdt_مفرد->لمائی;

    // Set to invalid state to indicate filling glyphs is not yet started.
    إذا (unlikely (!c->serializer->check_success (ریکارڈاں->resize (ریکارڈاں->لمائی + 1))))
      return_trace (خطأ);

    ریکارڈاں->پوچھ ().پہلا_Glyph_اشاریہ = 1;
    ریکارڈاں->پوچھ ().آخری_Glyph_اشاریہ = 0;
    bitmap_ناپ_context->ناپ += اشاریہ_Subtable_ریکارڈ::min_ناپ;

    c->serializer->push ();

    إذا (unlikely (!جوڑو_new_subtable (c, bitmap_ناپ_context, &(ریکارڈاں->پوچھ ()), lookup, ادھار, شروع)))
    {
      c->serializer->نکالو_discard ();
      c->serializer->revert (snap);
      bitmap_ناپ_context->cbdt_مفرد->shrink (old_cbdt_مفرد_لمائی);
      bitmap_ناپ_context->ناپ = old_ناپ;
      ریکارڈاں->resize (ریکارڈاں->لمائی - 1);
      return_trace (خطأ);
    }

    bitmap_ناپ_context->عدد_tables += 1;
    return_trace (سچ);
  }

  بنانشانی صحيح جوڑو_glyph_for_subset (hb_codepoint_t gid)
  {
    إذا (پہلا_Glyph_اشاریہ > آخری_Glyph_اشاریہ)
    {
      پہلا_Glyph_اشاریہ = gid;
      آخری_Glyph_اشاریہ = gid;
      أرجع 0;
    }
    // TODO maybe assert? this shouldn't occur.
    إذا (آخری_Glyph_اشاریہ > gid)
      أرجع 0;
    بنانشانی صحيح عدد_missing = (بنانشانی صحيح) (gid - آخری_Glyph_اشاریہ - 1);
    آخری_Glyph_اشاریہ = gid;
    أرجع عدد_missing;
  }

  منطقي get_extents (hb_glyph_extents_t *extents, ثابت فراغ *ادھار, منطقي scale) ثابت
  { أرجع (ادھار+وتھ_منزل_Subtable).get_extents (extents, scale); }

  منطقي get_image_انکڑے (بنانشانی صحيح  gid,
		       ثابت فراغ   *ادھار,
		       بنانشانی صحيح *وتھ,
		       بنانشانی صحيح *لمائی,
		       بنانشانی صحيح *format) ثابت
  {
    إذا (gid < پہلا_Glyph_اشاریہ || gid > آخری_Glyph_اشاریہ) أرجع خطأ;
    أرجع (ادھار+وتھ_منزل_Subtable).get_image_انکڑے (gid - پہلا_Glyph_اشاریہ,
						   وتھ, لمائی, format);
  }

  HBGlyphID16			پہلا_Glyph_اشاریہ;
  HBGlyphID16			آخری_Glyph_اشاریہ;
  Offset32_منزل<اشاریہ_Subtable>	وتھ_منزل_Subtable;
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية اشاریہ_Subtable_Array
{
  یار بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (اشاریہ_Subtables_Z.sanitize (c, گنتی, هذا));
  }

  فراغ
  build_lookup (hb_subset_context_t *c, cblc_bitmap_ناپ_subset_context_t *bitmap_ناپ_context,
		hb_سمتیہ_t<hb_pair_t<hb_codepoint_t,
		ثابت اشاریہ_Subtable_ریکارڈ*>> *lookup /*باہرلا*/) ثابت
  {
    منطقي شروع_glyph_is_سیٹ = خطأ;
    بنانشانی عدد_glyphs = c->plan->عدد_output_glyphs ();
    لكل (hb_codepoint_t new_gid = 0; new_gid < عدد_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) استمر;

      ثابت اشاریہ_Subtable_ریکارڈ* ریکارڈ = لبھو_جدول (old_gid, bitmap_ناپ_context->عدد_tables);
      إذا (unlikely (!ریکارڈ)) استمر;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      بنانشانی صحيح وتھ, لمائی, format;
      إذا (unlikely (!ریکارڈ->get_image_انکڑے (old_gid, هذا, &وتھ, &لمائی, &format))) استمر;

      lookup->push (hb_pair_t<hb_codepoint_t, ثابت اشاریہ_Subtable_ریکارڈ*> (new_gid, ریکارڈ));

      إذا (!شروع_glyph_is_سیٹ)
      {
	bitmap_ناپ_context->شروع_glyph = new_gid;
	شروع_glyph_is_سیٹ = سچ;
      }

      bitmap_ناپ_context->سرا_glyph = new_gid;
    }
  }

  منطقي
  subset (hb_subset_context_t *c,
	  cblc_bitmap_ناپ_subset_context_t *bitmap_ناپ_context) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_سمتیہ_t<hb_pair_t<hb_codepoint_t, ثابت اشاریہ_Subtable_ریکارڈ*>> lookup;
    build_lookup (c, bitmap_ناپ_context, &lookup);
    إذا (unlikely (!c->serializer->propagate_error (lookup)))
      أرجع خطأ;

    bitmap_ناپ_context->ناپ = 0;
    bitmap_ناپ_context->عدد_tables = 0;
    hb_سمتیہ_t<اشاریہ_Subtable_ریکارڈ> ریکارڈاں;
    لكل (بنانشانی صحيح شروع = 0; شروع < lookup.لمائی;)
    {
      إذا (unlikely (!lookup[شروع].second->جوڑو_new_ریکارڈ (c, bitmap_ناپ_context, &lookup, هذا, &شروع, &ریکارڈاں)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	لكل (بنانشانی صحيح i = 0; i < ریکارڈاں.لمائی; i++)
	  c->serializer->نکالو_discard ();
	return_trace (خطأ);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_سمتیہ_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (بنانشانی صحيح i = 0; i < ریکارڈاں.لمائی; i++)
      objidxs.push (c->serializer->نکالو_pack ());
    لكل (بنانشانی صحيح i = 0; i < ریکارڈاں.لمائی; i++)
    {
      اشاریہ_Subtable_ریکارڈ* ریکارڈ = c->serializer->embed (ریکارڈاں[i]);
      إذا (unlikely (!ریکارڈ)) return_trace (خطأ);
      c->serializer->جوڑو_link (ریکارڈ->وتھ_منزل_Subtable, objidxs[ریکارڈاں.لمائی - 1 - i]);
    }
    return_trace (سچ);
  }

  عام:
  ثابت اشاریہ_Subtable_ریکارڈ* لبھو_جدول (hb_codepoint_t glyph, بنانشانی صحيح عدد_Tables) ثابت
  {
    لكل (بنانشانی صحيح i = 0; i < عدد_Tables; ++i)
    {
      بنانشانی صحيح پہلا_Glyph_اشاریہ = اشاریہ_Subtables_Z[i].پہلا_Glyph_اشاریہ;
      بنانشانی صحيح آخری_Glyph_اشاریہ = اشاریہ_Subtables_Z[i].آخری_Glyph_اشاریہ;
      إذا (پہلا_Glyph_اشاریہ <= glyph && glyph <= آخری_Glyph_اشاریہ)
	أرجع &اشاریہ_Subtables_Z[i];
    }
    أرجع خالی_سوچک;
  }

  محمي:
  Unsized_Array_دا<اشاریہ_Subtable_ریکارڈ>	اشاریہ_Subtables_Z;
};

بنية Bitmap_ناپ_جدول
{
  یار بنية CBLC;
  یار بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  اشاریہ_Subtable_Array_وتھ.sanitize (c, ادھار, نمبر_دا_اشاریہ_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ثابت اشاریہ_Subtable_ریکارڈ *
  لبھو_جدول (hb_codepoint_t glyph, ثابت فراغ *ادھار, ثابت فراغ **باہرلا_ادھار) ثابت
  {
    *باہرلا_ادھار = &(ادھار+اشاریہ_Subtable_Array_وتھ);
    أرجع (ادھار+اشاریہ_Subtable_Array_وتھ).لبھو_جدول (glyph, نمبر_دا_اشاریہ_Subtables);
  }

  منطقي
  subset (hb_subset_context_t *c, ثابت فراغ *ادھار,
	  ثابت حرف *cbdt, بنانشانی صحيح cbdt_لمائی,
	  hb_سمتیہ_t<حرف> *cbdt_مفرد /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا_جدول = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا_جدول)) return_trace (خطأ);

    cblc_bitmap_ناپ_subset_context_t bitmap_ناپ_context;
    bitmap_ناپ_context.cbdt = cbdt;
    bitmap_ناپ_context.cbdt_لمائی = cbdt_لمائی;
    bitmap_ناپ_context.cbdt_مفرد = cbdt_مفرد;
    bitmap_ناپ_context.ناپ = اشاریہ_Tables_ناپ;
    bitmap_ناپ_context.عدد_tables = نمبر_دا_اشاریہ_Subtables;
    bitmap_ناپ_context.شروع_glyph = 1;
    bitmap_ناپ_context.سرا_glyph = 0;
    bitmap_ناپ_context.dedup = dedup;

    إذا (!باہرلا_جدول->اشاریہ_Subtable_Array_وتھ.serialize_subset (c,
							       اشاریہ_Subtable_Array_وتھ,
							       ادھار,
							       &bitmap_ناپ_context))
      return_trace (خطأ);
    إذا (!bitmap_ناپ_context.ناپ ||
	!bitmap_ناپ_context.عدد_tables ||
	bitmap_ناپ_context.شروع_glyph > bitmap_ناپ_context.سرا_glyph)
      return_trace (خطأ);

    باہرلا_جدول->اشاریہ_Tables_ناپ = bitmap_ناپ_context.ناپ;
    باہرلا_جدول->نمبر_دا_اشاریہ_Subtables = bitmap_ناپ_context.عدد_tables;
    باہرلا_جدول->شروع_Glyph_اشاریہ = bitmap_ناپ_context.شروع_glyph;
    باہرلا_جدول->سرا_Glyph_اشاریہ = bitmap_ناپ_context.سرا_glyph;
    return_trace (سچ);
  }

  محمي:
  NNOffset32_منزل<اشاریہ_Subtable_Array>
			اشاریہ_Subtable_Array_وتھ;
  HBUINT32		اشاریہ_Tables_ناپ;
  HBUINT32		نمبر_دا_اشاریہ_Subtables;
  HBUINT32		colorRef;
  SBit_سطر_Metrics	horizontal;
  SBit_سطر_Metrics	vertical;
  HBGlyphID16		شروع_Glyph_اشاریہ;
  HBGlyphID16		سرا_Glyph_اشاریہ;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		ثنائی_ہندسہ_ڈونگھائی;
  HBINT8		flags;
  عام:
  DEFINE_ناپ_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

بنية Glyph_Bitmap_انکڑے_Format17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32_دا<HBUINT8>	انکڑے;
  عام:
  DEFINE_ناپ_ARRAY (9, انکڑے);
};

بنية Glyph_Bitmap_انکڑے_Format18
{
  BigGlyphMetrics	glyphMetrics;
  Array32_دا<HBUINT8>	انکڑے;
  عام:
  DEFINE_ناپ_ARRAY (12, انکڑے);
};

بنية Glyph_Bitmap_انکڑے_Format19
{
  Array32_دا<HBUINT8>	انکڑے;
  عام:
  DEFINE_ناپ_ARRAY (4, انکڑے);
};

بنية CBLC
{
  یار بنية CBDT;

  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_CBLC;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 2 || نسخہ.major == 3) &&
		  hb_barrier () &&
		  ناپ_Tables.sanitize (c, هذا));
  }

  ساكن منطقي
  sink_cbdt (hb_subset_context_t *c, hb_سمتیہ_t<حرف>* cbdt_مفرد)
  {
    hb_blob_t *cbdt_مفرد_blob = hb_blob_create (cbdt_مفرد->arrayZ,
						 cbdt_مفرد->لمائی,
						 HB_حافظہ_منوال_WRITABLE,
						 cbdt_مفرد->arrayZ,
						 hb_free);
    cbdt_مفرد->init ();  // Leak arrayZ to the blob.
    منطقي ret = c->plan->جوڑو_جدول (HB_OT_ٹیگ_CBDT, cbdt_مفرد_blob);
    hb_blob_destroy (cbdt_مفرد_blob);
    أرجع ret;
  }

  منطقي
  subset_ناپ_جدول (hb_subset_context_t *c, ثابت Bitmap_ناپ_جدول& جدول,
		     ثابت حرف *cbdt /*وچ*/, بنانشانی صحيح cbdt_لمائی,
		     CBLC *cblc_مفرد /* INOUT */, hb_سمتیہ_t<حرف> *cbdt_مفرد /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    cblc_مفرد->ناپ_Tables.len++;

    خودکار snap = c->serializer->snapshot ();
    خودکار cbdt_مفرد_len = cbdt_مفرد->لمائی;

    إذا (!جدول.subset (c, هذا, cbdt, cbdt_لمائی, cbdt_مفرد, dedup))
    {
      cblc_مفرد->ناپ_Tables.len--;
      c->serializer->revert (snap);
      cbdt_مفرد->shrink (cbdt_مفرد_len);
      return_trace (خطأ);
    }
    return_trace (سچ);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_اندرونی منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  ثابت Bitmap_ناپ_جدول &چنو_strike (hb_font_t *font) ثابت
  {
    بنانشانی گنتی = ناپ_Tables.len;
    إذا (unlikely (!گنتی))
      أرجع Null (Bitmap_ناپ_جدول);

    بنانشانی صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    إذا (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    بنانشانی صحيح بہترین_i = 0;
    بنانشانی صحيح بہترین_ppem = hb_max (ناپ_Tables[0].ppemX, ناپ_Tables[0].ppemY);

    لكل (بنانشانی صحيح i = 1; i < گنتی; i++)
    {
      بنانشانی صحيح ppem = hb_max (ناپ_Tables[i].ppemX, ناپ_Tables[i].ppemY);
      إذا ((requested_ppem <= ppem && ppem < بہترین_ppem) ||
	  (requested_ppem > بہترین_ppem && ppem > بہترین_ppem))
      {
	بہترین_i = i;
	بہترین_ppem = ppem;
      }
    }

    أرجع ناپ_Tables[بہترین_i];
  }

  محمي:
  Fixed_نسخہ<>		نسخہ;
  Array32_دا<Bitmap_ناپ_جدول>	ناپ_Tables;
  عام:
  DEFINE_ناپ_ARRAY (8, ناپ_Tables);
};

بنية CBDT
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_CBDT;

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
      ثابت فراغ *ادھار;
      ثابت Bitmap_ناپ_جدول &strike = هذا->cblc->چنو_strike (font);
      ثابت اشاریہ_Subtable_ریکارڈ *subtable_ریکارڈ = strike.لبھو_جدول (glyph, cblc, &ادھار);
      إذا (!subtable_ریکارڈ || !strike.ppemX || !strike.ppemY)
	أرجع خطأ;

      إذا (subtable_ریکارڈ->get_extents (extents, ادھار, scale))
	أرجع سچ;

      بنانشانی صحيح image_وتھ = 0, image_لمائی = 0, image_format = 0;
      إذا (!subtable_ریکارڈ->get_image_انکڑے (glyph, ادھار, &image_وتھ, &image_لمائی, &image_format))
	أرجع خطأ;

      بنانشانی صحيح cbdt_len = cbdt.get_لمائی ();
      إذا (unlikely (image_وتھ > cbdt_len || cbdt_len - image_وتھ < image_لمائی))
	أرجع خطأ;

      بدل (image_format)
      {
      حالة 17: {
	إذا (unlikely (image_لمائی < Glyph_Bitmap_انکڑے_Format17::min_ناپ))
	  أرجع خطأ;
	خودکار &glyphFormat17 = Struct_At_وتھ<Glyph_Bitmap_انکڑے_Format17> (هذا->cbdt, image_وتھ);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	اكسر;
      }
      حالة 18: {
	إذا (unlikely (image_لمائی < Glyph_Bitmap_انکڑے_Format18::min_ناپ))
	  أرجع خطأ;
	خودکار &glyphFormat18 = Struct_At_وتھ<Glyph_Bitmap_انکڑے_Format18> (هذا->cbdt, image_وتھ);
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
	extents->چوڑائی = hb_clamp_منزل<hb_position_t> (roundf (extents->چوڑائی * x_scale));
	extents->اچائی = hb_clamp_منزل<hb_position_t> (roundf (extents->اچائی * y_scale));
      }

      أرجع سچ;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ثابت
    {
      ثابت فراغ *ادھار;
      ثابت Bitmap_ناپ_جدول &strike = هذا->cblc->چنو_strike (font);
      ثابت اشاریہ_Subtable_ریکارڈ *subtable_ریکارڈ = strike.لبھو_جدول (glyph, cblc, &ادھار);
      إذا (!subtable_ریکارڈ || !strike.ppemX || !strike.ppemY)
	أرجع hb_blob_get_خالی_تھاواں ();

      بنانشانی صحيح image_وتھ = 0, image_لمائی = 0, image_format = 0;
      إذا (!subtable_ریکارڈ->get_image_انکڑے (glyph, ادھار, &image_وتھ, &image_لمائی, &image_format))
	أرجع hb_blob_get_خالی_تھاواں ();

      بنانشانی صحيح cbdt_len = cbdt.get_لمائی ();
      إذا (unlikely (image_وتھ > cbdt_len || cbdt_len - image_وتھ < image_لمائی))
	أرجع hb_blob_get_خالی_تھاواں ();

      بدل (image_format)
      {
      حالة 17:
      {
	إذا (unlikely (image_لمائی < Glyph_Bitmap_انکڑے_Format17::min_ناپ))
	  أرجع hb_blob_get_خالی_تھاواں ();
	خودکار &glyphFormat17 = Struct_At_وتھ<Glyph_Bitmap_انکڑے_Format17> (هذا->cbdt, image_وتھ);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_وتھ + Glyph_Bitmap_انکڑے_Format17::min_ناپ,
					glyphFormat17.انکڑے.len);
      }
      حالة 18:
      {
	إذا (unlikely (image_لمائی < Glyph_Bitmap_انکڑے_Format18::min_ناپ))
	  أرجع hb_blob_get_خالی_تھاواں ();
	خودکار &glyphFormat18 = Struct_At_وتھ<Glyph_Bitmap_انکڑے_Format18> (هذا->cbdt, image_وتھ);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_وتھ + Glyph_Bitmap_انکڑے_Format18::min_ناپ,
					glyphFormat18.انکڑے.len);
      }
      حالة 19:
      {
	إذا (unlikely (image_لمائی < Glyph_Bitmap_انکڑے_Format19::min_ناپ))
	  أرجع hb_blob_get_خالی_تھاواں ();
	خودکار &glyphFormat19 = Struct_At_وتھ<Glyph_Bitmap_انکڑے_Format19> (هذا->cbdt, image_وتھ);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_وتھ + Glyph_Bitmap_انکڑے_Format19::min_ناپ,
					glyphFormat19.انکڑے.len);
      }
      افتراضي: أرجع hb_blob_get_خالی_تھاواں (); /* TODO: Support other image formats. */
      }
    }

    منطقي has_انکڑے () ثابت { أرجع cbdt->نسخہ.major; }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *انکڑے) ثابت
    {
      إذا (!has_انکڑے ()) أرجع خطأ;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      إذا (unlikely (!font->get_glyph_extents (glyph, &extents, خطأ)))
        أرجع خطأ;

      إذا (unlikely (!get_extents (font, glyph, &pixel_extents, خطأ)))
        أرجع خطأ;

      hb_blob_t *blob = reference_png (font, glyph);
      إذا (unlikely (hb_blob_is_immutable (blob)))
        أرجع خطأ;

      منطقي ret = funcs->image (انکڑے,
			       blob,
			       pixel_extents.چوڑائی, -pixel_extents.اچائی,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    بنانشانی صحيح upem;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 2 || نسخہ.major == 3));
  }

  محمي:
  Fixed_نسخہ<>		نسخہ;
  Unsized_Array_دا<HBUINT8>	انکڑے_Z;
  عام:
  DEFINE_ناپ_ARRAY (4, انکڑے_Z);
};

مضمن منطقي
CBLC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_سمتیہ_t<حرف> cbdt_مفرد;

  خودکار *cblc_مفرد = c->serializer->شروع_embed<CBLC> ();
  إذا (unlikely (!c->serializer->extend_min (cblc_مفرد))) return_trace (خطأ);
  cblc_مفرد->نسخہ = نسخہ;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_جدول<CBDT> (c->plan->سروت);
  بنانشانی صحيح cbdt_لمائی;
  CBDT* cbdt = (CBDT *) hb_blob_get_انکڑے (cbdt_blob, &cbdt_لمائی);
  إذا (unlikely (cbdt_لمائی < CBDT::min_ناپ))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (خطأ);
  }
  copy_انکڑے_منزل_cbdt (&cbdt_مفرد, cbdt, CBDT::min_ناپ);

  cbdt_dedup_context_t dedup;
  لكل (ثابت Bitmap_ناپ_جدول& جدول : + ناپ_Tables.iter ())
    subset_ناپ_جدول (c, جدول, (ثابت حرف *) cbdt, cbdt_لمائی, cblc_مفرد, &cbdt_مفرد, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_مفرد));
}

بنية CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#جے_انت /* OT_COLOR_CBDT_CBDT_HH */
