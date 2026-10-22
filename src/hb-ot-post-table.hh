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

#که_نه_تعریف_شوی HB_OT_POST_جدول_HH
#تعریف_کړه HB_OT_POST_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-var-mvar-table.hh"

#تعریف_کړه HB_STRING_ARRAY_NAME format1_نومونه
#تعریف_کړه HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#ضمن "hb-string-array.hh"
#تعریف_لغوه HB_STRING_ARRAY_LIST
#تعریف_لغوه HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#تعریف_کړه HB_OT_نښان_post HB_نښان('p','o','s','t')


نطاق OT {


بنية post_V2_لکۍ
{
  ملګری بنية post;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (glyph_Name_شاخص.sanitize (c));
  }

  کينډۍ<ډول_نوم Iterator>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ثابت فراغ* _post) ثابت;

  منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  Array16_د<HBUINT16>	glyph_Name_شاخص;	/* This is not an offset, but is the
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
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_post;

  منطقي serialize (hb_serialize_context_t *c, منطقي glyph_نومونه) ثابت
  {
    TRACE_SERIALIZE (هذا);
    post *post_اولي = c->allocate_min<post> ();
    إذا (unlikely (!post_اولي))  return_trace (خطأ);

    hb_memcpy (post_اولي, هذا, post::min_اندازه);
    إذا (!glyph_نومونه)
      return_trace (c->check_ټاکل (post_اولي->نسخه.major, 3,
                                     HB_SERIALIZE_ERROR_INT_ډکېدنه)); // Version 3 does not have any glyph names.

    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *post_اولي = c->serializer->پیل_embed<post> ();

    منطقي glyph_نومونه = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_نومونه;
    إذا (!serialize (c->serializer, glyph_نومونه))
      return_trace (خطأ);

#که_نه_تعریف_شوی HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خپلکاری &MVAR = *c->plan->سرچینه->جدول.MVAR;
      خپلکاری *جدول = post_اولي;

      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_UNDERLINE_اندازه,   underlineThickness);
      HB_جمع_کړه_MVAR_VAR (HB_OT_METRICS_نښان_UNDERLINE_دننه_واټن, underlinePosition);
    }
#د_که_پای

    Triple *axis_range;
    إذا (c->plan->user_axes_location.has (HB_نښان ('s','l','n','t'), &axis_range))
    {
      عائم italic_angle = hb_max (-90.f, hb_min (axis_range->منځ, 90.f));
      إذا (post_اولي->italicAngle.منزل_float () != italic_angle)
        post_اولي->italicAngle.سېټ_float (italic_angle);
    }

    إذا (glyph_نومونه && نسخه.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (ریښتیا);
  }

  بنية accelerator_t
  {
    ملګری بنية post_V2_لکۍ;

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<post> (face);
      بینښه صحيح جدول_اوږدتیا = جدول.get_اوږدتیا ();

      نسخه = جدول->نسخه.منزل_int ();
      إذا (نسخه != 0x00020000) أرجع;
      hb_barrier ();

      ثابت post_V2_لکۍ &v2 = جدول->v2X;

      glyph_Name_شاخص = &v2.glyph_Name_شاخص;
      pool = &StructAfter<uint8_t> (v2.glyph_Name_شاخص);

      ثابت uint8_t *پای_ټکی = (ثابت uint8_t *) (ثابت فراغ *) جدول + جدول_اوږدتیا;
      شاخص_منزل_دننه_واټن.alloc (hb_min (face->get_عدد_glyphs (), جدول_اوږدتیا / 8));
      لكل (ثابت uint8_t *معلومات = pool;
	   شاخص_منزل_دننه_واټن.اوږدتیا < 65535 && معلومات < پای_ټکی && معلومات + *معلومات < پای_ټکی;
	   معلومات += 1 + *معلومات)
	شاخص_منزل_دننه_واټن.push (معلومات - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      جدول.destroy ();
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, بینښه صحيح buf_len) ثابت
    {
      hb_بایټونه_t s = پیدا_کړه_glyph_name (glyph);
      إذا (!s.اوږدتیا) أرجع خطأ;
      إذا (!buf_len) أرجع ریښتیا;
      بینښه صحيح len = hb_min (buf_len - 1, s.اوږدتیا);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      أرجع ریښتیا;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      بینښه صحيح شمېر = get_glyph_شمېر ();
      إذا (unlikely (!شمېر)) أرجع خطأ;

      إذا (len < 0) len = مزي_اوږدوالی (name);

      إذا (unlikely (!len)) أرجع خطأ;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      إذا (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (شمېر, حجم (gids[0]));
	إذا (unlikely (!gids))
	  أرجع خطأ; /* Anything better?! */

	لكل (بینښه صحيح i = 0; i < شمېر; i++)
	  gids[i] = i;
	خپلکاری thiz = هذا;
	hb_array_t<uint16_t> (gids, شمېر)
	  .qsort ([thiz] (ثابت uint16_t &a, ثابت uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    أرجع thiz->پیدا_کړه_glyph_name (b).cmp (thiz->پیدا_کړه_glyph_name (a));
	  });

	إذا (unlikely (!gids_sorted_by_name.cmpexch (تش_اشاره, gids)))
	{
	  hb_free (gids);
	  اذهب retry;
	}
      }

      hb_بایټونه_t st (name, len);
      خپلکاری* gid = hb_bsearch (st, gids, شمېر, حجم (gids[0]), cmp_کلی, (فراغ *) هذا);
      إذا (gid)
      {
	*glyph = *gid;
	أرجع ریښتیا;
      }

      أرجع خطأ;
    }

    hb_blob_ptr_t<post> جدول;

    محمي:

    بینښه صحيح get_glyph_شمېر () ثابت
    {
      إذا (نسخه == 0x00010000)
      {
        hb_barrier ();
	أرجع format1_نومونه_اوږدتیا;
      }

      إذا (نسخه == 0x00020000)
      {
        hb_barrier ();
	أرجع glyph_Name_شاخص->len;
      }

      أرجع 0;
    }

    ساكن صحيح cmp_کلی (ثابت فراغ *pk, ثابت فراغ *po, فراغ *arg)
    {
      ثابت accelerator_t *thiz = (ثابت accelerator_t *) arg;
      ثابت hb_بایټونه_t *key = (ثابت hb_بایټونه_t *) pk;
      uint16_t o = * (ثابت uint16_t *) po;
      أرجع thiz->پیدا_کړه_glyph_name (o).cmp (*key);
    }

    hb_بایټونه_t پیدا_کړه_glyph_name (hb_codepoint_t glyph) ثابت
    {
      إذا (نسخه == 0x00010000)
      {
        hb_barrier ();
	إذا (glyph >= format1_نومونه_اوږدتیا)
	  أرجع hb_بایټونه_t ();

	أرجع format1_نومونه (glyph);
      }

      إذا (نسخه != 0x00020000)
	أرجع hb_بایټونه_t ();
      hb_barrier ();

      إذا (glyph >= glyph_Name_شاخص->len)
	أرجع hb_بایټونه_t ();

      بینښه صحيح شاخص_2 = glyph_Name_شاخص->arrayZ[glyph];
      إذا (شاخص_2 < format1_نومونه_اوږدتیا)
	أرجع format1_نومونه (شاخص_2);
      شاخص_2 -= format1_نومونه_اوږدتیا;

      إذا (شاخص_2 >= شاخص_منزل_دننه_واټن.اوږدتیا)
	أرجع hb_بایټونه_t ();
      بینښه صحيح دننه_واټن = شاخص_منزل_دننه_واټن[شاخص_2];

      ثابت uint8_t *معلومات = pool + دننه_واټن;
      بینښه صحيح name_اوږدتیا = *معلومات;
      معلومات++;

      أرجع hb_بایټونه_t ((ثابت حرف *) معلومات, name_اوږدتیا);
    }

    خاص:
    uint32_t نسخه;
    ثابت Array16_د<HBUINT16> *glyph_Name_شاخص = تش_اشاره;
    hb_ویکټور_t<uint32_t> شاخص_منزل_دننه_واټن;
    ثابت uint8_t *pool = تش_اشاره;
    بدلیدونکی hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (نسخه.منزل_int () == 0x00010000 ||
		   (نسخه.منزل_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   نسخه.منزل_int () == 0x00030000));
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
  post_V2_لکۍ	v2X;
  DEFINE_اندازه_MIN (32);
};

بنية post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#د_که_پای /* HB_OT_POST_TABLE_HH */
