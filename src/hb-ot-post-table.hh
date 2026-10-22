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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_OT_POST_جدول_HH
#تعریف_کرو HB_OT_POST_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-var-mvar-table.hh"

#تعریف_کرو HB_STRING_ARRAY_NAME format1_نام
#تعریف_کرو HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#شامل "hb-string-array.hh"
#تعریف_منسوخ HB_STRING_ARRAY_LIST
#تعریف_منسوخ HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#تعریف_کرو HB_OT_لیبل_post HB_لیبل('p','o','s','t')


نامخلا OT {


ساخت post_V2_دم
{
  دوست ساخت post;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (glyph_Name_اشاریہ.sanitize (c));
  }

  سانچہ<قسم_نام Iterator>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ثابت خالی* _post) ثابت;

  منطقی subset (hb_subset_context_t *c) ثابت;

  محفوظ:
  Array16_کا<HBUINT16>	glyph_Name_اشاریہ;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  عوامی:
  DEFINE_سائز_ARRAY (2, glyph_Name_اشاریہ);
};

ساخت post
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_post;

  منطقی serialize (hb_serialize_context_t *c, منطقی glyph_نام) ثابت
  {
    TRACE_SERIALIZE (یہ);
    post *post_مفرد = c->allocate_min<post> ();
    اگر (unlikely (!post_مفرد))  return_trace (جھوٹ);

    hb_memcpy (post_مفرد, یہ, post::min_سائز);
    اگر (!glyph_نام)
      return_trace (c->check_تفویض (post_مفرد->نسخہ.major, 3,
                                     HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)); // Version 3 does not have any glyph names.

    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *post_مفرد = c->serializer->آغاز_embed<post> ();

    منطقی glyph_نام = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_نام;
    اگر (!serialize (c->serializer, glyph_نام))
      return_trace (جھوٹ);

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (c->plan->normalized_coords)
    {
      خودکار &MVAR = *c->plan->ماخذ->جدول.MVAR;
      خودکار *جدول = post_مفرد;

      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_UNDERLINE_سائز,   underlineThickness);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_لیبل_UNDERLINE_انحراف, underlinePosition);
    }
#اگر_ختم

    Triple *axis_range;
    اگر (c->plan->user_axes_location.has (HB_لیبل ('s','l','n','t'), &axis_range))
    {
      اعشاری italic_angle = hb_max (-90.f, hb_min (axis_range->درمیان, 90.f));
      اگر (post_مفرد->italicAngle.مقصد_float () != italic_angle)
        post_مفرد->italicAngle.سیٹ_float (italic_angle);
    }

    اگر (glyph_نام && نسخہ.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (سچ);
  }

  ساخت accelerator_t
  {
    دوست ساخت post_V2_دم;

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<post> (face);
      غیرعلامتی صحیح جدول_لمبائی = جدول.get_لمبائی ();

      نسخہ = جدول->نسخہ.مقصد_int ();
      اگر (نسخہ != 0x00020000) واپس;
      hb_barrier ();

      ثابت post_V2_دم &v2 = جدول->v2X;

      glyph_Name_اشاریہ = &v2.glyph_Name_اشاریہ;
      pool = &StructAfter<uint8_t> (v2.glyph_Name_اشاریہ);

      ثابت uint8_t *انتہا = (ثابت uint8_t *) (ثابت خالی *) جدول + جدول_لمبائی;
      اشاریہ_مقصد_انحراف.alloc (hb_min (face->get_عدد_glyphs (), جدول_لمبائی / 8));
      برائے (ثابت uint8_t *ڈیٹا = pool;
	   اشاریہ_مقصد_انحراف.لمبائی < 65535 && ڈیٹا < انتہا && ڈیٹا + *ڈیٹا < انتہا;
	   ڈیٹا += 1 + *ڈیٹا)
	اشاریہ_مقصد_انحراف.push (ڈیٹا - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      جدول.destroy ();
    }

    منطقی get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, غیرعلامتی صحیح buf_len) ثابت
    {
      hb_بائٹیں_t s = ڈھونڈو_glyph_name (glyph);
      اگر (!s.لمبائی) واپس جھوٹ;
      اگر (!buf_len) واپس سچ;
      غیرعلامتی صحیح len = hb_min (buf_len - 1, s.لمبائی);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      واپس سچ;
    }

    منطقی get_glyph_from_name (ثابت حرف *name, صحیح len,
			      hb_codepoint_t *glyph) ثابت
    {
      غیرعلامتی صحیح گنتی = get_glyph_گنتی ();
      اگر (unlikely (!گنتی)) واپس جھوٹ;

      اگر (len < 0) len = لڑی_لمبائی (name);

      اگر (unlikely (!len)) واپس جھوٹ;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      اگر (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (گنتی, حجم (gids[0]));
	اگر (unlikely (!gids))
	  واپس جھوٹ; /* Anything better?! */

	برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
	  gids[i] = i;
	خودکار thiz = یہ;
	hb_array_t<uint16_t> (gids, گنتی)
	  .qsort ([thiz] (ثابت uint16_t &a, ثابت uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    واپس thiz->ڈھونڈو_glyph_name (b).cmp (thiz->ڈھونڈو_glyph_name (a));
	  });

	اگر (unlikely (!gids_sorted_by_name.cmpexch (خالی_اشاریہ, gids)))
	{
	  hb_free (gids);
	  جاؤ retry;
	}
      }

      hb_بائٹیں_t st (name, len);
      خودکار* gid = hb_bsearch (st, gids, گنتی, حجم (gids[0]), cmp_کلید, (خالی *) یہ);
      اگر (gid)
      {
	*glyph = *gid;
	واپس سچ;
      }

      واپس جھوٹ;
    }

    hb_blob_ptr_t<post> جدول;

    محفوظ:

    غیرعلامتی صحیح get_glyph_گنتی () ثابت
    {
      اگر (نسخہ == 0x00010000)
      {
        hb_barrier ();
	واپس format1_نام_لمبائی;
      }

      اگر (نسخہ == 0x00020000)
      {
        hb_barrier ();
	واپس glyph_Name_اشاریہ->len;
      }

      واپس 0;
    }

    ساکن صحیح cmp_کلید (ثابت خالی *pk, ثابت خالی *po, خالی *arg)
    {
      ثابت accelerator_t *thiz = (ثابت accelerator_t *) arg;
      ثابت hb_بائٹیں_t *key = (ثابت hb_بائٹیں_t *) pk;
      uint16_t o = * (ثابت uint16_t *) po;
      واپس thiz->ڈھونڈو_glyph_name (o).cmp (*key);
    }

    hb_بائٹیں_t ڈھونڈو_glyph_name (hb_codepoint_t glyph) ثابت
    {
      اگر (نسخہ == 0x00010000)
      {
        hb_barrier ();
	اگر (glyph >= format1_نام_لمبائی)
	  واپس hb_بائٹیں_t ();

	واپس format1_نام (glyph);
      }

      اگر (نسخہ != 0x00020000)
	واپس hb_بائٹیں_t ();
      hb_barrier ();

      اگر (glyph >= glyph_Name_اشاریہ->len)
	واپس hb_بائٹیں_t ();

      غیرعلامتی صحیح اشاریہ_2 = glyph_Name_اشاریہ->arrayZ[glyph];
      اگر (اشاریہ_2 < format1_نام_لمبائی)
	واپس format1_نام (اشاریہ_2);
      اشاریہ_2 -= format1_نام_لمبائی;

      اگر (اشاریہ_2 >= اشاریہ_مقصد_انحراف.لمبائی)
	واپس hb_بائٹیں_t ();
      غیرعلامتی صحیح انحراف = اشاریہ_مقصد_انحراف[اشاریہ_2];

      ثابت uint8_t *ڈیٹا = pool + انحراف;
      غیرعلامتی صحیح name_لمبائی = *ڈیٹا;
      ڈیٹا++;

      واپس hb_بائٹیں_t ((ثابت حرف *) ڈیٹا, name_لمبائی);
    }

    نجی:
    uint32_t نسخہ;
    ثابت Array16_کا<HBUINT16> *glyph_Name_اشاریہ = خالی_اشاریہ;
    hb_سمتیہ_t<uint32_t> اشاریہ_مقصد_انحراف;
    ثابت uint8_t *pool = خالی_اشاریہ;
    تغیرپذیر hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  (نسخہ.مقصد_int () == 0x00010000 ||
		   (نسخہ.مقصد_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   نسخہ.مقصد_int () == 0x00030000));
  }

  عوامی:
  Fixed_نسخہ<>نسخہ;		/* 0x00010000 for version 1.0
					 * 0x00020000 for version 2.0
					 * 0x00025000 for version 2.5 (deprecated)
					 * 0x00030000 for version 3.0 */
  F16DOT16	italicAngle;		/* Italic angle in counter-clockwise degrees
					 * from the vertical. Zero for upright text,
					 * negative for text that leans to the right
					 * (forward). */
  FWORD		underlinePosition;	/* This is the suggested distance of the top
					 * of the underline from the baseline
					 * (negative values indicate below baseline).
					 * The PostScript definition of this FontInfo
					 * dictionary key (the y coordinate of the
					 * center of the stroke) is not used for
					 * historical reasons. The value of the
					 * PostScript key may be calculated by
					 * subtracting half the underlineThickness
					 * from the value of this field. */
  FWORD		underlineThickness;	/* Suggested values for the underline
					   thickness. */
  HBUINT32	isFixedPitch;		/* Set to 0 if the font is proportionally
					 * spaced, non-zero if the font is not
					 * proportionally spaced (i.e. monospaced). */
  HBUINT32	minMemType42;		/* Minimum memory usage when an OpenType font
					 * is downloaded. */
  HBUINT32	maxMemType42;		/* Maximum memory usage when an OpenType font
					 * is downloaded. */
  HBUINT32	minMemType1;		/* Minimum memory usage when an OpenType font
					 * is downloaded as a Type 1 font. */
  HBUINT32	maxMemType1;		/* Maximum memory usage when an OpenType font
					 * is downloaded as a Type 1 font. */
  post_V2_دم	v2X;
  DEFINE_سائز_MIN (32);
};

ساخت post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#اگر_ختم /* HB_OT_POST_TABLE_HH */
