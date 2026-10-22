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

#اگر_غیر_تعریف_شدہ OT_COLOR_CBDT_CBDT_HH
#تعریف_کرو OT_COLOR_CBDT_CBDT_HH

#شامل "../../../hb-open-type.hh"
#شامل "../../../hb-paint.hh"

/*
 * CBLC -- Color Bitmap Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cblc
 * https://docs.microsoft.com/en-us/typography/opentype/spec/eblc
 * CBDT -- Color Bitmap Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cbdt
 * https://docs.microsoft.com/en-us/typography/opentype/spec/ebdt
 */
#تعریف_کرو HB_OT_لیبل_CBLC HB_لیبل('C','B','L','C')
#تعریف_کرو HB_OT_لیبل_CBDT HB_لیبل('C','B','D','T')


نامخلا OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
ساخت cbdt_dedup_context_t
{
  ساخت اندراج_t
  {
    غیرعلامتی صحیح بنیاد;		/* Offset of the data in cbdt_prime. */
    غیرعلامتی صحیح lstart;	/* Start index in lengths. */
    غیرعلامتی صحیح lcount;	/* Number of length entries. */
  };

  ساکن uint64_t hash (غیرعلامتی صحیح image_format,
			ثابت hb_سمتیہ_t<حرف> &ڈیٹا,
			ثابت hb_سمتیہ_t<غیرعلامتی صحیح> &lengths)
  {
    uint32_t h = hb_hash (hb_بائٹیں_t (ڈیٹا.arrayZ, ڈیٹا.لمبائی));
    h = h * 31 + hb_hash (hb_بائٹیں_t ((ثابت حرف *) lengths.arrayZ,
				      lengths.لمبائی * حجم (غیرعلامتی صحیح)));
    واپس ((uint64_t) image_format << 32) | h;
  }

  منطقی find (uint64_t key,
	     ثابت hb_سمتیہ_t<حرف> &ڈیٹا,
	     ثابت hb_سمتیہ_t<غیرعلامتی صحیح> &lengths,
	     ثابت hb_سمتیہ_t<حرف> &cbdt_مفرد,
	     غیرعلامتی صحیح *بنیاد /*برآمد*/) ثابت
  {
    غیرعلامتی صحیح *idx_p = خالی_اشاریہ;
    اگر (!map.has (key, &idx_p)) واپس جھوٹ;
    ثابت اندراج_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    اگر (e.lcount != lengths.لمبائی) واپس جھوٹ;
    اگر (unlikely (e.بنیاد + ڈیٹا.لمبائی > (غیرعلامتی صحیح) cbdt_مفرد.لمبائی)) واپس جھوٹ;
    اگر (hb_memcmp (cbdt_مفرد.arrayZ + e.بنیاد, ڈیٹا.arrayZ, ڈیٹا.لمبائی)) واپس جھوٹ;
    اگر (hb_memcmp (یہ->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.لمبائی * حجم (غیرعلامتی صحیح)))
      واپس جھوٹ;
    *بنیاد = e.بنیاد;
    واپس سچ;
  }

  منطقی جوڑو (uint64_t key,
	    غیرعلامتی صحیح بنیاد,
	    ثابت hb_سمتیہ_t<غیرعلامتی صحیح> &lengths)
  {
    اگر (unlikely (entries.اندر_error () || یہ->lengths.اندر_error () || map.اندر_error ()))
      واپس جھوٹ;
    اندراج_t e = {بنیاد, یہ->lengths.لمبائی, lengths.لمبائی};
    اگر (unlikely (!entries.ڈالو_or_fail (e))) واپس جھوٹ;
    برائے (غیرعلامتی صحیح لمبائی : lengths)
      اگر (unlikely (!یہ->lengths.ڈالو_or_fail (لمبائی))) واپس جھوٹ;
    واپس map.سیٹ (key, entries.لمبائی - 1);
  }

  hb_hashmap_t<uint64_t, غیرعلامتی صحیح> map;
  hb_سمتیہ_t<اندراج_t> entries;
  hb_سمتیہ_t<غیرعلامتی صحیح> lengths; /* Concatenated per-entry lengths. */
};

ساخت cblc_bitmap_سائز_subset_context_t
{
  ثابت حرف *cbdt;
  غیرعلامتی صحیح cbdt_لمبائی;
  hb_سمتیہ_t<حرف> *cbdt_مفرد;
  غیرعلامتی صحیح سائز;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  غیرعلامتی صحیح عدد_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t آغاز_glyph;	/*برآمد*/
  hb_codepoint_t انتہا_glyph;	/*برآمد*/
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ساکن اندرونی منطقی
copy_ڈیٹا_مقصد_cbdt (hb_سمتیہ_t<حرف> *cbdt_مفرد,
		    ثابت خالی        *ڈیٹا,
		    غیرعلامتی           لمبائی)
{
  غیرعلامتی صحیح new_len = cbdt_مفرد->لمبائی + لمبائی;
  اگر (unlikely (!cbdt_مفرد->alloc (new_len))) واپس جھوٹ;
  hb_memcpy (cbdt_مفرد->arrayZ + cbdt_مفرد->لمبائی, ڈیٹا, لمبائی);
  cbdt_مفرد->لمبائی = new_len;
  واپس سچ;
}

ساخت SmallGlyphMetrics
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقی scale) ثابت
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->چوڑائی = چوڑائی;
    extents->اونچائی = -static_cast<صحیح> (اونچائی);

    اگر (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	اونچائی;
  HBUINT8	چوڑائی;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  عوامی:
  DEFINE_سائز_STATIC (5);
};

ساخت BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت SBit_سطر_Metrics
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBINT8	ascender;
  HBINT8	decender;
  HBUINT8	چوڑائی_Max;
  HBINT8	caret_ڈھلان_Numerator;
  HBINT8	caret_ڈھلان_Denominator;
  HBINT8	caret_انحراف;
  HBINT8	minOriginSB;
  HBINT8	minAdvanceSB;
  HBINT8	maxBeforeBL;
  HBINT8	minAfterBL;
  HBINT8	padding1;
  HBINT8	padding2;
  عوامی:
  DEFINE_سائز_STATIC (12);
};


/*
 * Index Subtables.
 */

ساخت اشاریہ_Subtable_سرخی
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBUINT16	اشاریہ_Format;
  HBUINT16	imageFormat;
  HBUINT32	image_ڈیٹا_انحراف;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

سانچہ <قسم_نام انحراف_Type>
ساخت اشاریہ_Subtable_Format1_Or3
{
  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح glyph_گنتی) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  انحراف_Array_Z.sanitize (c, glyph_گنتی + 1));
  }

  منطقی get_image_ڈیٹا (غیرعلامتی صحیح idx,
		       غیرعلامتی صحیح *انحراف,
		       غیرعلامتی صحیح *لمبائی) ثابت
  {
    اگر (unlikely (انحراف_Array_Z[idx + 1] <= انحراف_Array_Z[idx]))
      واپس جھوٹ;

    *انحراف = سرخی.image_ڈیٹا_انحراف + انحراف_Array_Z[idx];
    *لمبائی = انحراف_Array_Z[idx + 1] - انحراف_Array_Z[idx];
    واپس سچ;
  }

  منطقی جوڑو_انحراف (hb_serialize_context_t *c,
		   غیرعلامتی صحیح انحراف,
		   غیرعلامتی صحیح *سائز /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (یہ);
    انحراف_2<انحراف_Type> embedded_انحراف;
    embedded_انحراف = انحراف;
    *سائز += حجم (انحراف_Type);
    خودکار *o = c->embed (embedded_انحراف);
    return_trace ((منطقی) o);
  }

  اشاریہ_Subtable_سرخی	سرخی;
  Unsized_Array_کا<انحراف_2<انحراف_Type>>
			انحراف_Array_Z;
  عوامی:
  DEFINE_سائز_ARRAY (8, انحراف_Array_Z);
};

ساخت اشاریہ_Subtable_Format1 : اشاریہ_Subtable_Format1_Or3<HBUINT32> {};
ساخت اشاریہ_Subtable_Format3 : اشاریہ_Subtable_Format1_Or3<HBUINT16> {};

ساخت اشاریہ_Subtable
{
  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح glyph_گنتی) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.سرخی.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.سرخی.اشاریہ_Format)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_گنتی));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_گنتی));
    طےشدہ:return_trace (سچ);
    }
  }

  منطقی
  اختتام_subtable (hb_serialize_context_t *c,
		   غیرعلامتی صحیح انتہا_انحراف,
		   غیرعلامتی صحیح عدد_glyphs,
		   غیرعلامتی صحیح *سائز /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (یہ);

    انتخاب (u.سرخی.اشاریہ_Format)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.جوڑو_انحراف (c, انتہا_انحراف, سائز));
    حالت 3: {
      hb_barrier ();
      اگر (!u.format3.جوڑو_انحراف (c, انتہا_انحراف, سائز))
	return_trace (جھوٹ);
      اگر (!(عدد_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.جوڑو_انحراف (c, 0, سائز));
      return_trace (سچ);
    }
    // TODO: implement 2, 4, 5.
    حالت 2: حالت 4:  // No-op.
    حالت 5:  // Pad to 32-bit aligned.
    طےشدہ: return_trace (جھوٹ);
    }
  }

  منطقی
  جوڑو_انحراف (hb_serialize_context_t *c, غیرعلامتی صحیح local_انحراف,
	      غیرعلامتی صحیح *سائز /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (یہ);
    انتخاب (u.سرخی.اشاریہ_Format)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.جوڑو_انحراف (c, local_انحراف, سائز));
    حالت 3: hb_barrier (); return_trace (u.format3.جوڑو_انحراف (c, local_انحراف, سائز));
    // TODO: Implement tables 2, 4, 5
    حالت 2:  // Should be a no-op.
    حالت 4: حالت 5:  // Handle sparse cases.
    طےشدہ: return_trace (جھوٹ);
    }
  }

  منطقی get_extents (hb_glyph_extents_t *extents HB_غیر_مستعمل, منطقی scale HB_غیر_مستعمل) ثابت
  {
    انتخاب (u.سرخی.اشاریہ_Format)
    {
    حالت 2: حالت 5: /* TODO */
    حالت 1: حالت 3: حالت 4: /* Variable-metrics formats do not have metrics here. */
    طےشدہ:واپس (جھوٹ);
    }
  }

  منطقی
  get_image_ڈیٹا (غیرعلامتی صحیح idx, غیرعلامتی صحیح *انحراف,
		  غیرعلامتی صحیح *لمبائی, غیرعلامتی صحیح *format) ثابت
  {
    *format = u.سرخی.imageFormat;
    انتخاب (u.سرخی.اشاریہ_Format)
    {
    حالت 1: hb_barrier (); واپس u.format1.get_image_ڈیٹا (idx, انحراف, لمبائی);
    حالت 3: hb_barrier (); واپس u.format3.get_image_ڈیٹا (idx, انحراف, لمبائی);
    طےشدہ: واپس جھوٹ;
    }
  }

  ثابت اشاریہ_Subtable_سرخی* get_سرخی () ثابت { واپس &u.سرخی; }

  خالی populate_سرخی (غیرعلامتی اشاریہ_format,
			غیرعلامتی image_format,
			غیرعلامتی صحیح image_ڈیٹا_انحراف_2,
			غیرعلامتی صحیح *سائز)
  {
    u.سرخی.اشاریہ_Format = اشاریہ_format;
    u.سرخی.imageFormat = image_format;
    u.سرخی.image_ڈیٹا_انحراف = image_ڈیٹا_انحراف_2;
    انتخاب (u.سرخی.اشاریہ_Format)
    {
    حالت 1: *سائز += اشاریہ_Subtable_Format1::min_سائز; توڑو;
    حالت 3: *سائز += اشاریہ_Subtable_Format3::min_سائز; توڑو;
    }
  }

  محفوظ:
  اتحاد {
  اشاریہ_Subtable_سرخی	سرخی;
  اشاریہ_Subtable_Format1	format1;
  اشاریہ_Subtable_Format3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  عوامی:
  DEFINE_سائز_UNION (8, سرخی);
};

ساخت اشاریہ_Subtable_ریکارڈ
{
  /* XXX Remove this and fix by not inserting it into vector. */
  اشاریہ_Subtable_ریکارڈ& عامل = (ثابت اشاریہ_Subtable_ریکارڈ &o)
  {
    پہلا_Glyph_اشاریہ = o.پہلا_Glyph_اشاریہ;
    آخری_Glyph_اشاریہ = o.آخری_Glyph_اشاریہ;
    انحراف_مقصد_Subtable = (غیرعلامتی) o.انحراف_مقصد_Subtable;
    أكد (انحراف_مقصد_Subtable.is_null ());
    واپس *یہ;
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  پہلا_Glyph_اشاریہ <= آخری_Glyph_اشاریہ &&
		  انحراف_مقصد_Subtable.sanitize (c, بنیاد, آخری_Glyph_اشاریہ - پہلا_Glyph_اشاریہ + 1));
  }

  ثابت اشاریہ_Subtable* get_subtable (ثابت خالی *بنیاد) ثابت
  {
    واپس &(بنیاد+انحراف_مقصد_Subtable);
  }

  منطقی جوڑو_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_سائز_subset_context_t *bitmap_سائز_context,
			 اشاریہ_Subtable_ریکارڈ *ریکارڈ,
			 ثابت hb_سمتیہ_t<hb_pair_t<hb_codepoint_t, ثابت اشاریہ_Subtable_ریکارڈ*>> *lookup, /*اندر*/
			 ثابت خالی *بنیاد,
			 غیرعلامتی صحیح *آغاز /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (یہ);

    خودکار *subtable = c->serializer->آغاز_embed<اشاریہ_Subtable> ();
    اگر (unlikely (!c->serializer->extend_min (subtable))) return_trace (جھوٹ);

    خودکار *old_subtable = get_subtable (بنیاد);
    خودکار *old_سرخی = old_subtable->get_سرخی ();
    غیرعلامتی صحیح image_format = old_سرخی->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_سمتیہ_t<حرف> ڈیٹا;
    hb_سمتیہ_t<غیرعلامتی صحیح> lengths;
    غیرعلامتی صحیح عدد_glyphs = 0;
    منطقی early_exit = جھوٹ;
    برائے (غیرعلامتی صحیح i = *آغاز; i < lookup->لمبائی; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].پہلا;
      ثابت اشاریہ_Subtable_ریکارڈ *اگلا_ریکارڈ = (*lookup)[i].second;
      ثابت اشاریہ_Subtable *اگلا_subtable = اگلا_ریکارڈ->get_subtable (بنیاد);
      خودکار *اگلا_سرخی = اگلا_subtable->get_سرخی ();
      اگر (اگلا_سرخی != old_سرخی)
      {
	*آغاز = i;
	early_exit = سچ;
	توڑو;
      }
      غیرعلامتی صحیح عدد_missing = ریکارڈ->جوڑو_glyph_for_subset (new_gid);
      برائے (غیرعلامتی صحیح j = 0; j < عدد_missing; j++)
	اگر (unlikely (!c->serializer->check_success (lengths.ڈالو_or_fail (0))))
	  return_trace (جھوٹ);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      اگر (old_gid < اگلا_ریکارڈ->پہلا_Glyph_اشاریہ)
	return_trace (جھوٹ);

      غیرعلامتی صحیح old_idx = (غیرعلامتی صحیح) old_gid - اگلا_ریکارڈ->پہلا_Glyph_اشاریہ;
      غیرعلامتی صحیح انحراف, لمبائی, format;
      اگر (unlikely (!اگلا_subtable->get_image_ڈیٹا (old_idx, &انحراف, &لمبائی, &format)))
	return_trace (جھوٹ);
      اگر (unlikely (انحراف > bitmap_سائز_context->cbdt_لمبائی ||
		    bitmap_سائز_context->cbdt_لمبائی - انحراف < لمبائی))
	return_trace (جھوٹ);
      اگر (unlikely (!c->serializer->check_success (
			copy_ڈیٹا_مقصد_cbdt (&ڈیٹا,
					    bitmap_سائز_context->cbdt + انحراف,
					    لمبائی))))
	return_trace (جھوٹ);
      اگر (unlikely (!c->serializer->check_success (lengths.ڈالو_or_fail (لمبائی))))
	return_trace (جھوٹ);
      عدد_glyphs += 1 + عدد_missing;
    }
    اگر (!early_exit)
      *آغاز = lookup->لمبائی;

    /* Find or append the data block for this subtable. */
    غیرعلامتی صحیح image_ڈیٹا_انحراف_2;
    cbdt_dedup_context_t *dedup = bitmap_سائز_context->dedup;
    منطقی shared = جھوٹ;
    uint64_t key = 0;
    اگر (dedup && ڈیٹا.لمبائی)
    {
      key = cbdt_dedup_context_t::hash (image_format, ڈیٹا, lengths);
      shared = dedup->find (key, ڈیٹا, lengths,
			    *bitmap_سائز_context->cbdt_مفرد,
			    &image_ڈیٹا_انحراف_2);
    }
    اگر (!shared)
    {
      image_ڈیٹا_انحراف_2 = bitmap_سائز_context->cbdt_مفرد->لمبائی;
      اگر (unlikely (!c->serializer->check_success (
			copy_ڈیٹا_مقصد_cbdt (bitmap_سائز_context->cbdt_مفرد,
					    ڈیٹا.arrayZ, ڈیٹا.لمبائی))))
	return_trace (جھوٹ);
      اگر (dedup && ڈیٹا.لمبائی)
	/* The bitmap data is already copied; caching is best-effort. */
	(خالی) dedup->جوڑو (key, image_ڈیٹا_انحراف_2, lengths);
    }

    subtable->populate_سرخی (old_سرخی->اشاریہ_Format,
			       image_format,
			       image_ڈیٹا_انحراف_2,
			       &bitmap_سائز_context->سائز);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    غیرعلامتی صحیح cumulative = 0;
    برائے (غیرعلامتی صحیح لمبائی : lengths)
    {
      اگر (unlikely (!subtable->جوڑو_انحراف (c->serializer, cumulative,
					   &bitmap_سائز_context->سائز)))
	return_trace (جھوٹ);
      cumulative += لمبائی;
    }
    اگر (unlikely (!subtable->اختتام_subtable (c->serializer,
					      cumulative,
					      عدد_glyphs,
					      &bitmap_سائز_context->سائز)))
      return_trace (جھوٹ);
    return_trace (سچ);
  }

  منطقی جوڑو_new_ریکارڈ (hb_subset_context_t *c,
		       cblc_bitmap_سائز_subset_context_t *bitmap_سائز_context,
		       ثابت hb_سمتیہ_t<hb_pair_t<hb_codepoint_t, ثابت اشاریہ_Subtable_ریکارڈ*>> *lookup, /*اندر*/
		       ثابت خالی *بنیاد,
		       غیرعلامتی صحیح *آغاز, /* INOUT */
		       hb_سمتیہ_t<اشاریہ_Subtable_ریکارڈ>* اندراجات /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار snap = c->serializer->snapshot ();
    غیرعلامتی صحیح old_سائز = bitmap_سائز_context->سائز;
    غیرعلامتی صحیح old_cbdt_مفرد_لمبائی = bitmap_سائز_context->cbdt_مفرد->لمبائی;

    // Set to invalid state to indicate filling glyphs is not yet started.
    اگر (unlikely (!c->serializer->check_success (اندراجات->resize (اندراجات->لمبائی + 1))))
      return_trace (جھوٹ);

    اندراجات->دم ().پہلا_Glyph_اشاریہ = 1;
    اندراجات->دم ().آخری_Glyph_اشاریہ = 0;
    bitmap_سائز_context->سائز += اشاریہ_Subtable_ریکارڈ::min_سائز;

    c->serializer->push ();

    اگر (unlikely (!جوڑو_new_subtable (c, bitmap_سائز_context, &(اندراجات->دم ()), lookup, بنیاد, آغاز)))
    {
      c->serializer->نکالو_discard ();
      c->serializer->revert (snap);
      bitmap_سائز_context->cbdt_مفرد->shrink (old_cbdt_مفرد_لمبائی);
      bitmap_سائز_context->سائز = old_سائز;
      اندراجات->resize (اندراجات->لمبائی - 1);
      return_trace (جھوٹ);
    }

    bitmap_سائز_context->عدد_tables += 1;
    return_trace (سچ);
  }

  غیرعلامتی صحیح جوڑو_glyph_for_subset (hb_codepoint_t gid)
  {
    اگر (پہلا_Glyph_اشاریہ > آخری_Glyph_اشاریہ)
    {
      پہلا_Glyph_اشاریہ = gid;
      آخری_Glyph_اشاریہ = gid;
      واپس 0;
    }
    // TODO maybe assert? this shouldn't occur.
    اگر (آخری_Glyph_اشاریہ > gid)
      واپس 0;
    غیرعلامتی صحیح عدد_missing = (غیرعلامتی صحیح) (gid - آخری_Glyph_اشاریہ - 1);
    آخری_Glyph_اشاریہ = gid;
    واپس عدد_missing;
  }

  منطقی get_extents (hb_glyph_extents_t *extents, ثابت خالی *بنیاد, منطقی scale) ثابت
  { واپس (بنیاد+انحراف_مقصد_Subtable).get_extents (extents, scale); }

  منطقی get_image_ڈیٹا (غیرعلامتی صحیح  gid,
		       ثابت خالی   *بنیاد,
		       غیرعلامتی صحیح *انحراف,
		       غیرعلامتی صحیح *لمبائی,
		       غیرعلامتی صحیح *format) ثابت
  {
    اگر (gid < پہلا_Glyph_اشاریہ || gid > آخری_Glyph_اشاریہ) واپس جھوٹ;
    واپس (بنیاد+انحراف_مقصد_Subtable).get_image_ڈیٹا (gid - پہلا_Glyph_اشاریہ,
						   انحراف, لمبائی, format);
  }

  HBGlyphID16			پہلا_Glyph_اشاریہ;
  HBGlyphID16			آخری_Glyph_اشاریہ;
  Offset32_مقصد<اشاریہ_Subtable>	انحراف_مقصد_Subtable;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت اشاریہ_Subtable_Array
{
  دوست ساخت CBDT;

  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح گنتی) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (اشاریہ_Subtables_Z.sanitize (c, گنتی, یہ));
  }

  خالی
  build_lookup (hb_subset_context_t *c, cblc_bitmap_سائز_subset_context_t *bitmap_سائز_context,
		hb_سمتیہ_t<hb_pair_t<hb_codepoint_t,
		ثابت اشاریہ_Subtable_ریکارڈ*>> *lookup /*برآمد*/) ثابت
  {
    منطقی آغاز_glyph_is_سیٹ = جھوٹ;
    غیرعلامتی عدد_glyphs = c->plan->عدد_output_glyphs ();
    برائے (hb_codepoint_t new_gid = 0; new_gid < عدد_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      اگر (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) جاری;

      ثابت اشاریہ_Subtable_ریکارڈ* ریکارڈ = ڈھونڈو_جدول (old_gid, bitmap_سائز_context->عدد_tables);
      اگر (unlikely (!ریکارڈ)) جاری;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      غیرعلامتی صحیح انحراف, لمبائی, format;
      اگر (unlikely (!ریکارڈ->get_image_ڈیٹا (old_gid, یہ, &انحراف, &لمبائی, &format))) جاری;

      lookup->push (hb_pair_t<hb_codepoint_t, ثابت اشاریہ_Subtable_ریکارڈ*> (new_gid, ریکارڈ));

      اگر (!آغاز_glyph_is_سیٹ)
      {
	bitmap_سائز_context->آغاز_glyph = new_gid;
	آغاز_glyph_is_سیٹ = سچ;
      }

      bitmap_سائز_context->انتہا_glyph = new_gid;
    }
  }

  منطقی
  subset (hb_subset_context_t *c,
	  cblc_bitmap_سائز_subset_context_t *bitmap_سائز_context) ثابت
  {
    TRACE_SUBSET (یہ);

    hb_سمتیہ_t<hb_pair_t<hb_codepoint_t, ثابت اشاریہ_Subtable_ریکارڈ*>> lookup;
    build_lookup (c, bitmap_سائز_context, &lookup);
    اگر (unlikely (!c->serializer->propagate_error (lookup)))
      واپس جھوٹ;

    bitmap_سائز_context->سائز = 0;
    bitmap_سائز_context->عدد_tables = 0;
    hb_سمتیہ_t<اشاریہ_Subtable_ریکارڈ> اندراجات;
    برائے (غیرعلامتی صحیح آغاز = 0; آغاز < lookup.لمبائی;)
    {
      اگر (unlikely (!lookup[آغاز].second->جوڑو_new_ریکارڈ (c, bitmap_سائز_context, &lookup, یہ, &آغاز, &اندراجات)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	برائے (غیرعلامتی صحیح i = 0; i < اندراجات.لمبائی; i++)
	  c->serializer->نکالو_discard ();
	return_trace (جھوٹ);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_سمتیہ_t<hb_serialize_context_t::objidx_t> objidxs;
    برائے (غیرعلامتی صحیح i = 0; i < اندراجات.لمبائی; i++)
      objidxs.push (c->serializer->نکالو_pack ());
    برائے (غیرعلامتی صحیح i = 0; i < اندراجات.لمبائی; i++)
    {
      اشاریہ_Subtable_ریکارڈ* ریکارڈ = c->serializer->embed (اندراجات[i]);
      اگر (unlikely (!ریکارڈ)) return_trace (جھوٹ);
      c->serializer->جوڑو_link (ریکارڈ->انحراف_مقصد_Subtable, objidxs[اندراجات.لمبائی - 1 - i]);
    }
    return_trace (سچ);
  }

  عوامی:
  ثابت اشاریہ_Subtable_ریکارڈ* ڈھونڈو_جدول (hb_codepoint_t glyph, غیرعلامتی صحیح عدد_Tables) ثابت
  {
    برائے (غیرعلامتی صحیح i = 0; i < عدد_Tables; ++i)
    {
      غیرعلامتی صحیح پہلا_Glyph_اشاریہ = اشاریہ_Subtables_Z[i].پہلا_Glyph_اشاریہ;
      غیرعلامتی صحیح آخری_Glyph_اشاریہ = اشاریہ_Subtables_Z[i].آخری_Glyph_اشاریہ;
      اگر (پہلا_Glyph_اشاریہ <= glyph && glyph <= آخری_Glyph_اشاریہ)
	واپس &اشاریہ_Subtables_Z[i];
    }
    واپس خالی_اشاریہ;
  }

  محفوظ:
  Unsized_Array_کا<اشاریہ_Subtable_ریکارڈ>	اشاریہ_Subtables_Z;
};

ساخت Bitmap_سائز_جدول
{
  دوست ساخت CBLC;
  دوست ساخت CBDT;

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  اشاریہ_Subtable_Array_انحراف.sanitize (c, بنیاد, نمبر_کا_اشاریہ_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ثابت اشاریہ_Subtable_ریکارڈ *
  ڈھونڈو_جدول (hb_codepoint_t glyph, ثابت خالی *بنیاد, ثابت خالی **برآمد_بنیاد) ثابت
  {
    *برآمد_بنیاد = &(بنیاد+اشاریہ_Subtable_Array_انحراف);
    واپس (بنیاد+اشاریہ_Subtable_Array_انحراف).ڈھونڈو_جدول (glyph, نمبر_کا_اشاریہ_Subtables);
  }

  منطقی
  subset (hb_subset_context_t *c, ثابت خالی *بنیاد,
	  ثابت حرف *cbdt, غیرعلامتی صحیح cbdt_لمبائی,
	  hb_سمتیہ_t<حرف> *cbdt_مفرد /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد_جدول = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد_جدول)) return_trace (جھوٹ);

    cblc_bitmap_سائز_subset_context_t bitmap_سائز_context;
    bitmap_سائز_context.cbdt = cbdt;
    bitmap_سائز_context.cbdt_لمبائی = cbdt_لمبائی;
    bitmap_سائز_context.cbdt_مفرد = cbdt_مفرد;
    bitmap_سائز_context.سائز = اشاریہ_Tables_سائز;
    bitmap_سائز_context.عدد_tables = نمبر_کا_اشاریہ_Subtables;
    bitmap_سائز_context.آغاز_glyph = 1;
    bitmap_سائز_context.انتہا_glyph = 0;
    bitmap_سائز_context.dedup = dedup;

    اگر (!برآمد_جدول->اشاریہ_Subtable_Array_انحراف.serialize_subset (c,
							       اشاریہ_Subtable_Array_انحراف,
							       بنیاد,
							       &bitmap_سائز_context))
      return_trace (جھوٹ);
    اگر (!bitmap_سائز_context.سائز ||
	!bitmap_سائز_context.عدد_tables ||
	bitmap_سائز_context.آغاز_glyph > bitmap_سائز_context.انتہا_glyph)
      return_trace (جھوٹ);

    برآمد_جدول->اشاریہ_Tables_سائز = bitmap_سائز_context.سائز;
    برآمد_جدول->نمبر_کا_اشاریہ_Subtables = bitmap_سائز_context.عدد_tables;
    برآمد_جدول->آغاز_Glyph_اشاریہ = bitmap_سائز_context.آغاز_glyph;
    برآمد_جدول->انتہا_Glyph_اشاریہ = bitmap_سائز_context.انتہا_glyph;
    return_trace (سچ);
  }

  محفوظ:
  NNOffset32_مقصد<اشاریہ_Subtable_Array>
			اشاریہ_Subtable_Array_انحراف;
  HBUINT32		اشاریہ_Tables_سائز;
  HBUINT32		نمبر_کا_اشاریہ_Subtables;
  HBUINT32		colorRef;
  SBit_سطر_Metrics	horizontal;
  SBit_سطر_Metrics	vertical;
  HBGlyphID16		آغاز_Glyph_اشاریہ;
  HBGlyphID16		انتہا_Glyph_اشاریہ;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		ثنائی_ہندسہ_گہرائی;
  HBINT8		flags;
  عوامی:
  DEFINE_سائز_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

ساخت Glyph_Bitmap_ڈیٹا_Format17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32_کا<HBUINT8>	ڈیٹا;
  عوامی:
  DEFINE_سائز_ARRAY (9, ڈیٹا);
};

ساخت Glyph_Bitmap_ڈیٹا_Format18
{
  BigGlyphMetrics	glyphMetrics;
  Array32_کا<HBUINT8>	ڈیٹا;
  عوامی:
  DEFINE_سائز_ARRAY (12, ڈیٹا);
};

ساخت Glyph_Bitmap_ڈیٹا_Format19
{
  Array32_کا<HBUINT8>	ڈیٹا;
  عوامی:
  DEFINE_سائز_ARRAY (4, ڈیٹا);
};

ساخت CBLC
{
  دوست ساخت CBDT;

  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_CBLC;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 2 || نسخہ.major == 3) &&
		  hb_barrier () &&
		  سائز_Tables.sanitize (c, یہ));
  }

  ساکن منطقی
  sink_cbdt (hb_subset_context_t *c, hb_سمتیہ_t<حرف>* cbdt_مفرد)
  {
    hb_blob_t *cbdt_مفرد_blob = hb_blob_create (cbdt_مفرد->arrayZ,
						 cbdt_مفرد->لمبائی,
						 HB_حافظہ_منوال_WRITABLE,
						 cbdt_مفرد->arrayZ,
						 hb_free);
    cbdt_مفرد->init ();  // Leak arrayZ to the blob.
    منطقی ret = c->plan->جوڑو_جدول (HB_OT_لیبل_CBDT, cbdt_مفرد_blob);
    hb_blob_destroy (cbdt_مفرد_blob);
    واپس ret;
  }

  منطقی
  subset_سائز_جدول (hb_subset_context_t *c, ثابت Bitmap_سائز_جدول& جدول,
		     ثابت حرف *cbdt /*اندر*/, غیرعلامتی صحیح cbdt_لمبائی,
		     CBLC *cblc_مفرد /* INOUT */, hb_سمتیہ_t<حرف> *cbdt_مفرد /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (یہ);
    cblc_مفرد->سائز_Tables.len++;

    خودکار snap = c->serializer->snapshot ();
    خودکار cbdt_مفرد_len = cbdt_مفرد->لمبائی;

    اگر (!جدول.subset (c, یہ, cbdt, cbdt_لمبائی, cbdt_مفرد, dedup))
    {
      cblc_مفرد->سائز_Tables.len--;
      c->serializer->revert (snap);
      cbdt_مفرد->shrink (cbdt_مفرد_len);
      return_trace (جھوٹ);
    }
    return_trace (سچ);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_اندرونی منطقی subset (hb_subset_context_t *c) ثابت;

  محفوظ:
  ثابت Bitmap_سائز_جدول &انتخاب_strike (hb_font_t *font) ثابت
  {
    غیرعلامتی گنتی = سائز_Tables.len;
    اگر (unlikely (!گنتی))
      واپس Null (Bitmap_سائز_جدول);

    غیرعلامتی صحیح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    اگر (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    غیرعلامتی صحیح بہترین_i = 0;
    غیرعلامتی صحیح بہترین_ppem = hb_max (سائز_Tables[0].ppemX, سائز_Tables[0].ppemY);

    برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
    {
      غیرعلامتی صحیح ppem = hb_max (سائز_Tables[i].ppemX, سائز_Tables[i].ppemY);
      اگر ((requested_ppem <= ppem && ppem < بہترین_ppem) ||
	  (requested_ppem > بہترین_ppem && ppem > بہترین_ppem))
      {
	بہترین_i = i;
	بہترین_ppem = ppem;
      }
    }

    واپس سائز_Tables[بہترین_i];
  }

  محفوظ:
  Fixed_نسخہ<>		نسخہ;
  Array32_کا<Bitmap_سائز_جدول>	سائز_Tables;
  عوامی:
  DEFINE_سائز_ARRAY (8, سائز_Tables);
};

ساخت CBDT
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_CBDT;

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      یہ->cblc = hb_sanitize_context_t ().reference_جدول<CBLC> (face);
      یہ->cbdt = hb_sanitize_context_t ().reference_جدول<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      یہ->cblc.destroy ();
      یہ->cbdt.destroy ();
    }

    منطقی
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, منطقی scale = سچ) ثابت
    {
      ثابت خالی *بنیاد;
      ثابت Bitmap_سائز_جدول &strike = یہ->cblc->انتخاب_strike (font);
      ثابت اشاریہ_Subtable_ریکارڈ *subtable_ریکارڈ = strike.ڈھونڈو_جدول (glyph, cblc, &بنیاد);
      اگر (!subtable_ریکارڈ || !strike.ppemX || !strike.ppemY)
	واپس جھوٹ;

      اگر (subtable_ریکارڈ->get_extents (extents, بنیاد, scale))
	واپس سچ;

      غیرعلامتی صحیح image_انحراف = 0, image_لمبائی = 0, image_format = 0;
      اگر (!subtable_ریکارڈ->get_image_ڈیٹا (glyph, بنیاد, &image_انحراف, &image_لمبائی, &image_format))
	واپس جھوٹ;

      غیرعلامتی صحیح cbdt_len = cbdt.get_لمبائی ();
      اگر (unlikely (image_انحراف > cbdt_len || cbdt_len - image_انحراف < image_لمبائی))
	واپس جھوٹ;

      انتخاب (image_format)
      {
      حالت 17: {
	اگر (unlikely (image_لمبائی < Glyph_Bitmap_ڈیٹا_Format17::min_سائز))
	  واپس جھوٹ;
	خودکار &glyphFormat17 = Struct_At_انحراف<Glyph_Bitmap_ڈیٹا_Format17> (یہ->cbdt, image_انحراف);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	توڑو;
      }
      حالت 18: {
	اگر (unlikely (image_لمبائی < Glyph_Bitmap_ڈیٹا_Format18::min_سائز))
	  واپس جھوٹ;
	خودکار &glyphFormat18 = Struct_At_انحراف<Glyph_Bitmap_ڈیٹا_Format18> (یہ->cbdt, image_انحراف);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	توڑو;
      }
      طےشدہ: واپس جھوٹ; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      اگر (scale)
      {
	اعشاری x_scale = upem / (اعشاری) strike.ppemX;
	اعشاری y_scale = upem / (اعشاری) strike.ppemY;
	extents->x_bearing = hb_clamp_مقصد<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_مقصد<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->چوڑائی = hb_clamp_مقصد<hb_position_t> (roundf (extents->چوڑائی * x_scale));
	extents->اونچائی = hb_clamp_مقصد<hb_position_t> (roundf (extents->اونچائی * y_scale));
      }

      واپس سچ;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ثابت
    {
      ثابت خالی *بنیاد;
      ثابت Bitmap_سائز_جدول &strike = یہ->cblc->انتخاب_strike (font);
      ثابت اشاریہ_Subtable_ریکارڈ *subtable_ریکارڈ = strike.ڈھونڈو_جدول (glyph, cblc, &بنیاد);
      اگر (!subtable_ریکارڈ || !strike.ppemX || !strike.ppemY)
	واپس hb_blob_get_خالی_خانے ();

      غیرعلامتی صحیح image_انحراف = 0, image_لمبائی = 0, image_format = 0;
      اگر (!subtable_ریکارڈ->get_image_ڈیٹا (glyph, بنیاد, &image_انحراف, &image_لمبائی, &image_format))
	واپس hb_blob_get_خالی_خانے ();

      غیرعلامتی صحیح cbdt_len = cbdt.get_لمبائی ();
      اگر (unlikely (image_انحراف > cbdt_len || cbdt_len - image_انحراف < image_لمبائی))
	واپس hb_blob_get_خالی_خانے ();

      انتخاب (image_format)
      {
      حالت 17:
      {
	اگر (unlikely (image_لمبائی < Glyph_Bitmap_ڈیٹا_Format17::min_سائز))
	  واپس hb_blob_get_خالی_خانے ();
	خودکار &glyphFormat17 = Struct_At_انحراف<Glyph_Bitmap_ڈیٹا_Format17> (یہ->cbdt, image_انحراف);
	واپس hb_blob_create_sub_blob (cbdt.get_blob (),
					image_انحراف + Glyph_Bitmap_ڈیٹا_Format17::min_سائز,
					glyphFormat17.ڈیٹا.len);
      }
      حالت 18:
      {
	اگر (unlikely (image_لمبائی < Glyph_Bitmap_ڈیٹا_Format18::min_سائز))
	  واپس hb_blob_get_خالی_خانے ();
	خودکار &glyphFormat18 = Struct_At_انحراف<Glyph_Bitmap_ڈیٹا_Format18> (یہ->cbdt, image_انحراف);
	واپس hb_blob_create_sub_blob (cbdt.get_blob (),
					image_انحراف + Glyph_Bitmap_ڈیٹا_Format18::min_سائز,
					glyphFormat18.ڈیٹا.len);
      }
      حالت 19:
      {
	اگر (unlikely (image_لمبائی < Glyph_Bitmap_ڈیٹا_Format19::min_سائز))
	  واپس hb_blob_get_خالی_خانے ();
	خودکار &glyphFormat19 = Struct_At_انحراف<Glyph_Bitmap_ڈیٹا_Format19> (یہ->cbdt, image_انحراف);
	واپس hb_blob_create_sub_blob (cbdt.get_blob (),
					image_انحراف + Glyph_Bitmap_ڈیٹا_Format19::min_سائز,
					glyphFormat19.ڈیٹا.len);
      }
      طےشدہ: واپس hb_blob_get_خالی_خانے (); /* TODO: Support other image formats. */
      }
    }

    منطقی has_ڈیٹا () ثابت { واپس cbdt->نسخہ.major; }

    منطقی paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, خالی *ڈیٹا) ثابت
    {
      اگر (!has_ڈیٹا ()) واپس جھوٹ;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      اگر (unlikely (!font->get_glyph_extents (glyph, &extents, جھوٹ)))
        واپس جھوٹ;

      اگر (unlikely (!get_extents (font, glyph, &pixel_extents, جھوٹ)))
        واپس جھوٹ;

      hb_blob_t *blob = reference_png (font, glyph);
      اگر (unlikely (hb_blob_is_immutable (blob)))
        واپس جھوٹ;

      منطقی ret = funcs->image (ڈیٹا,
			       blob,
			       pixel_extents.چوڑائی, -pixel_extents.اونچائی,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      واپس ret;
    }

    نجی:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    غیرعلامتی صحیح upem;
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 2 || نسخہ.major == 3));
  }

  محفوظ:
  Fixed_نسخہ<>		نسخہ;
  Unsized_Array_کا<HBUINT8>	ڈیٹا_Z;
  عوامی:
  DEFINE_سائز_ARRAY (4, ڈیٹا_Z);
};

اندرونی منطقی
CBLC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (یہ);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_سمتیہ_t<حرف> cbdt_مفرد;

  خودکار *cblc_مفرد = c->serializer->آغاز_embed<CBLC> ();
  اگر (unlikely (!c->serializer->extend_min (cblc_مفرد))) return_trace (جھوٹ);
  cblc_مفرد->نسخہ = نسخہ;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_جدول<CBDT> (c->plan->ماخذ);
  غیرعلامتی صحیح cbdt_لمبائی;
  CBDT* cbdt = (CBDT *) hb_blob_get_ڈیٹا (cbdt_blob, &cbdt_لمبائی);
  اگر (unlikely (cbdt_لمبائی < CBDT::min_سائز))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (جھوٹ);
  }
  copy_ڈیٹا_مقصد_cbdt (&cbdt_مفرد, cbdt, CBDT::min_سائز);

  cbdt_dedup_context_t dedup;
  برائے (ثابت Bitmap_سائز_جدول& جدول : + سائز_Tables.iter ())
    subset_سائز_جدول (c, جدول, (ثابت حرف *) cbdt, cbdt_لمبائی, cblc_مفرد, &cbdt_مفرد, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_مفرد));
}

ساخت CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#اگر_ختم /* OT_COLOR_CBDT_CBDT_HH */
