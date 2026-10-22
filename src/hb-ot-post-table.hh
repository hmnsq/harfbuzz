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

#جے_بنا_تعریف HB_OT_POST_جدول_HH
#تعریف_کرو HB_OT_POST_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-var-mvar-table.hh"

#تعریف_کرو HB_STRING_ARRAY_NAME format1_ناں
#تعریف_کرو HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#ضمن "hb-string-array.hh"
#تعریف_رد HB_STRING_ARRAY_LIST
#تعریف_رد HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#تعریف_کرو HB_OT_ٹیگ_post HB_ٹیگ('p','o','s','t')


نطاق OT {


بنية post_V2_پوچھ
{
  یار بنية post;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (glyph_Name_اشاریہ.sanitize (c));
  }

  سانچا<قسم_ناں Iterator>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ثابت فراغ* _post) ثابت;

  منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  Array16_دا<HBUINT16>	glyph_Name_اشاریہ;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  عام:
  DEFINE_ناپ_ARRAY (2, glyph_Name_اشاریہ);
};

بنية post
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_post;

  منطقي serialize (hb_serialize_context_t *c, منطقي glyph_ناں) ثابت
  {
    TRACE_SERIALIZE (هذا);
    post *post_مفرد = c->allocate_min<post> ();
    إذا (unlikely (!post_مفرد))  return_trace (خطأ);

    hb_memcpy (post_مفرد, هذا, post::min_ناپ);
    إذا (!glyph_ناں)
      return_trace (c->check_سونپو (post_مفرد->نسخہ.major, 3,
                                     HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)); // Version 3 does not have any glyph names.

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *post_مفرد = c->serializer->شروع_embed<post> ();

    منطقي glyph_ناں = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_ناں;
    إذا (!serialize (c->serializer, glyph_ناں))
      return_trace (خطأ);

#جے_بنا_تعریف HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خودکار &MVAR = *c->plan->سروت->جدول.MVAR;
      خودکار *جدول = post_مفرد;

      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_UNDERLINE_ناپ,   underlineThickness);
      HB_جوڑو_MVAR_VAR (HB_OT_METRICS_ٹیگ_UNDERLINE_وتھ, underlinePosition);
    }
#جے_انت

    Triple *axis_range;
    إذا (c->plan->user_axes_location.has (HB_ٹیگ ('s','l','n','t'), &axis_range))
    {
      عائم italic_angle = hb_max (-90.f, hb_min (axis_range->درمیان, 90.f));
      إذا (post_مفرد->italicAngle.منزل_float () != italic_angle)
        post_مفرد->italicAngle.سیٹ_float (italic_angle);
    }

    إذا (glyph_ناں && نسخہ.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (سچ);
  }

  بنية accelerator_t
  {
    یار بنية post_V2_پوچھ;

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<post> (face);
      بنانشانی صحيح جدول_لمائی = جدول.get_لمائی ();

      نسخہ = جدول->نسخہ.منزل_int ();
      إذا (نسخہ != 0x00020000) أرجع;
      hb_barrier ();

      ثابت post_V2_پوچھ &v2 = جدول->v2X;

      glyph_Name_اشاریہ = &v2.glyph_Name_اشاریہ;
      pool = &StructAfter<uint8_t> (v2.glyph_Name_اشاریہ);

      ثابت uint8_t *سرا = (ثابت uint8_t *) (ثابت فراغ *) جدول + جدول_لمائی;
      اشاریہ_منزل_وتھ.alloc (hb_min (face->get_عدد_glyphs (), جدول_لمائی / 8));
      لكل (ثابت uint8_t *انکڑے = pool;
	   اشاریہ_منزل_وتھ.لمائی < 65535 && انکڑے < سرا && انکڑے + *انکڑے < سرا;
	   انکڑے += 1 + *انکڑے)
	اشاریہ_منزل_وتھ.push (انکڑے - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      جدول.destroy ();
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, بنانشانی صحيح buf_len) ثابت
    {
      hb_بائٹاں_t s = لبھو_glyph_name (glyph);
      إذا (!s.لمائی) أرجع خطأ;
      إذا (!buf_len) أرجع سچ;
      بنانشانی صحيح len = hb_min (buf_len - 1, s.لمائی);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      أرجع سچ;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      بنانشانی صحيح گنتی = get_glyph_گنتی ();
      إذا (unlikely (!گنتی)) أرجع خطأ;

      إذا (len < 0) len = لڑی_لمبائی (name);

      إذا (unlikely (!len)) أرجع خطأ;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      إذا (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (گنتی, حجم (gids[0]));
	إذا (unlikely (!gids))
	  أرجع خطأ; /* Anything better?! */

	لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
	  gids[i] = i;
	خودکار thiz = هذا;
	hb_array_t<uint16_t> (gids, گنتی)
	  .qsort ([thiz] (ثابت uint16_t &a, ثابت uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    أرجع thiz->لبھو_glyph_name (b).cmp (thiz->لبھو_glyph_name (a));
	  });

	إذا (unlikely (!gids_sorted_by_name.cmpexch (خالی_سوچک, gids)))
	{
	  hb_free (gids);
	  اذهب retry;
	}
      }

      hb_بائٹاں_t st (name, len);
      خودکار* gid = hb_bsearch (st, gids, گنتی, حجم (gids[0]), cmp_کلید, (فراغ *) هذا);
      إذا (gid)
      {
	*glyph = *gid;
	أرجع سچ;
      }

      أرجع خطأ;
    }

    hb_blob_ptr_t<post> جدول;

    محمي:

    بنانشانی صحيح get_glyph_گنتی () ثابت
    {
      إذا (نسخہ == 0x00010000)
      {
        hb_barrier ();
	أرجع format1_ناں_لمائی;
      }

      إذا (نسخہ == 0x00020000)
      {
        hb_barrier ();
	أرجع glyph_Name_اشاریہ->len;
      }

      أرجع 0;
    }

    ساكن صحيح cmp_کلید (ثابت فراغ *pk, ثابت فراغ *po, فراغ *arg)
    {
      ثابت accelerator_t *thiz = (ثابت accelerator_t *) arg;
      ثابت hb_بائٹاں_t *key = (ثابت hb_بائٹاں_t *) pk;
      uint16_t o = * (ثابت uint16_t *) po;
      أرجع thiz->لبھو_glyph_name (o).cmp (*key);
    }

    hb_بائٹاں_t لبھو_glyph_name (hb_codepoint_t glyph) ثابت
    {
      إذا (نسخہ == 0x00010000)
      {
        hb_barrier ();
	إذا (glyph >= format1_ناں_لمائی)
	  أرجع hb_بائٹاں_t ();

	أرجع format1_ناں (glyph);
      }

      إذا (نسخہ != 0x00020000)
	أرجع hb_بائٹاں_t ();
      hb_barrier ();

      إذا (glyph >= glyph_Name_اشاریہ->len)
	أرجع hb_بائٹاں_t ();

      بنانشانی صحيح اشاریہ_2 = glyph_Name_اشاریہ->arrayZ[glyph];
      إذا (اشاریہ_2 < format1_ناں_لمائی)
	أرجع format1_ناں (اشاریہ_2);
      اشاریہ_2 -= format1_ناں_لمائی;

      إذا (اشاریہ_2 >= اشاریہ_منزل_وتھ.لمائی)
	أرجع hb_بائٹاں_t ();
      بنانشانی صحيح وتھ = اشاریہ_منزل_وتھ[اشاریہ_2];

      ثابت uint8_t *انکڑے = pool + وتھ;
      بنانشانی صحيح name_لمائی = *انکڑے;
      انکڑے++;

      أرجع hb_بائٹاں_t ((ثابت حرف *) انکڑے, name_لمائی);
    }

    خاص:
    uint32_t نسخہ;
    ثابت Array16_دا<HBUINT16> *glyph_Name_اشاریہ = خالی_سوچک;
    hb_سمتیہ_t<uint32_t> اشاریہ_منزل_وتھ;
    ثابت uint8_t *pool = خالی_سوچک;
    بدلنجوگ hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  منطقي has_انکڑے () ثابت { أرجع نسخہ.منزل_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (نسخہ.منزل_int () == 0x00010000 ||
		   (نسخہ.منزل_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   نسخہ.منزل_int () == 0x00030000));
  }

  عام:
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
  post_V2_پوچھ	v2X;
  DEFINE_ناپ_MIN (32);
};

بنية post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#جے_انت /* HB_OT_POST_TABLE_HH */
