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

#اگر_تعریف_نشده HB_OT_POST_جدول_HH
#تعریف_کن HB_OT_POST_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-var-mvar-table.hh"

#تعریف_کن HB_STRING_ARRAY_NAME format1_نامها
#تعریف_کن HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#ضمن "hb-string-array.hh"
#لغو_تعریف HB_STRING_ARRAY_LIST
#لغو_تعریف HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#تعریف_کن HB_OT_برچسب_post HB_برچسب('p','o','s','t')


نطاق OT {


بنية post_V2_دم
{
  دوست بنية post;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (glyph_Name_شاخص.sanitize (c));
  }

  قالب<نام_نوع Iterator>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ثابت فراغ* _post) ثابت;

  منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  Array16_از<HBUINT16>	glyph_Name_شاخص;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  عام:
  DEFINE_اندازه_ARRAY (2, glyph_Name_شاخص);
};

بنية post
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_post;

  منطقي serialize (hb_serialize_context_t *c, منطقي glyph_نامها) ثابت
  {
    TRACE_SERIALIZE (هذا);
    post *post_اول = c->allocate_min<post> ();
    إذا (unlikely (!post_اول))  return_trace (خطأ);

    hb_memcpy (post_اول, هذا, post::min_اندازه);
    إذا (!glyph_نامها)
      return_trace (c->check_تخصیص_بده (post_اول->نسخه.major, 3,
                                     HB_SERIALIZE_ERROR_INT_سرریز)); // Version 3 does not have any glyph names.

    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *post_اول = c->serializer->شروع_embed<post> ();

    منطقي glyph_نامها = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_نامها;
    إذا (!serialize (c->serializer, glyph_نامها))
      return_trace (خطأ);

#اگر_تعریف_نشده HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خودکار &MVAR = *c->plan->منبع->جدول.MVAR;
      خودکار *جدول = post_اول;

      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_UNDERLINE_اندازه,   underlineThickness);
      HB_جمع_کن_MVAR_VAR (HB_OT_METRICS_برچسب_UNDERLINE_جابه_جایی, underlinePosition);
    }
#پایان_اگر

    Triple *axis_range;
    إذا (c->plan->user_axes_location.has (HB_برچسب ('s','l','n','t'), &axis_range))
    {
      عائم italic_angle = hb_max (-90.f, hb_min (axis_range->میانه, 90.f));
      إذا (post_اول->italicAngle.مقصد_float () != italic_angle)
        post_اول->italicAngle.مجموعه_float (italic_angle);
    }

    إذا (glyph_نامها && نسخه.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (درست);
  }

  بنية accelerator_t
  {
    دوست بنية post_V2_دم;

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<post> (face);
      بیعلامت صحيح جدول_درازی = جدول.get_درازی ();

      نسخه = جدول->نسخه.مقصد_int ();
      إذا (نسخه != 0x00020000) أرجع;
      hb_barrier ();

      ثابت post_V2_دم &v2 = جدول->v2X;

      glyph_Name_شاخص = &v2.glyph_Name_شاخص;
      pool = &StructAfter<uint8_t> (v2.glyph_Name_شاخص);

      ثابت uint8_t *انتها = (ثابت uint8_t *) (ثابت فراغ *) جدول + جدول_درازی;
      شاخص_مقصد_جابه_جایی.alloc (hb_min (face->get_رقم_glyphs (), جدول_درازی / 8));
      لكل (ثابت uint8_t *داده = pool;
	   شاخص_مقصد_جابه_جایی.درازی < 65535 && داده < انتها && داده + *داده < انتها;
	   داده += 1 + *داده)
	شاخص_مقصد_جابه_جایی.push (داده - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      جدول.destroy ();
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, بیعلامت صحيح buf_len) ثابت
    {
      hb_بایتها_t s = پیدا_کن_glyph_name (glyph);
      إذا (!s.درازی) أرجع خطأ;
      إذا (!buf_len) أرجع درست;
      بیعلامت صحيح len = hb_min (buf_len - 1, s.درازی);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      أرجع درست;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      بیعلامت صحيح شمار = get_glyph_شمار ();
      إذا (unlikely (!شمار)) أرجع خطأ;

      إذا (len < 0) len = طول_رشته (name);

      إذا (unlikely (!len)) أرجع خطأ;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      إذا (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (شمار, حجم (gids[0]));
	إذا (unlikely (!gids))
	  أرجع خطأ; /* Anything better?! */

	لكل (بیعلامت صحيح i = 0; i < شمار; i++)
	  gids[i] = i;
	خودکار thiz = هذا;
	hb_array_t<uint16_t> (gids, شمار)
	  .qsort ([thiz] (ثابت uint16_t &a, ثابت uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    أرجع thiz->پیدا_کن_glyph_name (b).cmp (thiz->پیدا_کن_glyph_name (a));
	  });

	إذا (unlikely (!gids_sorted_by_name.cmpexch (اشاره_تهی, gids)))
	{
	  hb_free (gids);
	  اذهب retry;
	}
      }

      hb_بایتها_t st (name, len);
      خودکار* gid = hb_bsearch (st, gids, شمار, حجم (gids[0]), cmp_کلید, (فراغ *) هذا);
      إذا (gid)
      {
	*glyph = *gid;
	أرجع درست;
      }

      أرجع خطأ;
    }

    hb_blob_ptr_t<post> جدول;

    محمي:

    بیعلامت صحيح get_glyph_شمار () ثابت
    {
      إذا (نسخه == 0x00010000)
      {
        hb_barrier ();
	أرجع format1_نامها_درازی;
      }

      إذا (نسخه == 0x00020000)
      {
        hb_barrier ();
	أرجع glyph_Name_شاخص->len;
      }

      أرجع 0;
    }

    ساكن صحيح cmp_کلید (ثابت فراغ *pk, ثابت فراغ *po, فراغ *arg)
    {
      ثابت accelerator_t *thiz = (ثابت accelerator_t *) arg;
      ثابت hb_بایتها_t *key = (ثابت hb_بایتها_t *) pk;
      uint16_t o = * (ثابت uint16_t *) po;
      أرجع thiz->پیدا_کن_glyph_name (o).cmp (*key);
    }

    hb_بایتها_t پیدا_کن_glyph_name (hb_codepoint_t glyph) ثابت
    {
      إذا (نسخه == 0x00010000)
      {
        hb_barrier ();
	إذا (glyph >= format1_نامها_درازی)
	  أرجع hb_بایتها_t ();

	أرجع format1_نامها (glyph);
      }

      إذا (نسخه != 0x00020000)
	أرجع hb_بایتها_t ();
      hb_barrier ();

      إذا (glyph >= glyph_Name_شاخص->len)
	أرجع hb_بایتها_t ();

      بیعلامت صحيح شاخص_2 = glyph_Name_شاخص->arrayZ[glyph];
      إذا (شاخص_2 < format1_نامها_درازی)
	أرجع format1_نامها (شاخص_2);
      شاخص_2 -= format1_نامها_درازی;

      إذا (شاخص_2 >= شاخص_مقصد_جابه_جایی.درازی)
	أرجع hb_بایتها_t ();
      بیعلامت صحيح جابه_جایی = شاخص_مقصد_جابه_جایی[شاخص_2];

      ثابت uint8_t *داده = pool + جابه_جایی;
      بیعلامت صحيح name_درازی = *داده;
      داده++;

      أرجع hb_بایتها_t ((ثابت حرف *) داده, name_درازی);
    }

    خاص:
    uint32_t نسخه;
    ثابت Array16_از<HBUINT16> *glyph_Name_شاخص = اشاره_تهی;
    hb_وکتور_t<uint32_t> شاخص_مقصد_جابه_جایی;
    ثابت uint8_t *pool = اشاره_تهی;
    تغییرپذیر hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (نسخه.مقصد_int () == 0x00010000 ||
		   (نسخه.مقصد_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   نسخه.مقصد_int () == 0x00030000));
  }

  عام:
  Fixed_نسخه<>نسخه;		/* 0x00010000 for version 1.0
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
  DEFINE_اندازه_MIN (32);
};

بنية post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#پایان_اگر /* HB_OT_POST_TABLE_HH */
