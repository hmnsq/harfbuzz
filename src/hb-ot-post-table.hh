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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_POST_جدول_HH
#وصف_ڪريو HB_OT_POST_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-var-mvar-table.hh"

#وصف_ڪريو HB_STRING_ARRAY_NAME format1_نالا
#وصف_ڪريو HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#ضمن "hb-string-array.hh"
#وصف_رد HB_STRING_ARRAY_LIST
#وصف_رد HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#وصف_ڪريو HB_OT_ٽيگ_post HB_ٽيگ('p','o','s','t')


نطاق OT {


بنية post_V2_پڇ
{
  دوست بنية post;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (glyph_Name_اشاريو.sanitize (c));
  }

  سانچو<قسم_نالو Iterator>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ثابت فراغ* _post) ثابت;

  منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  Array16_جو<HBUINT16>	glyph_Name_اشاريو;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  عام:
  DEFINE_سائيز_ARRAY (2, glyph_Name_اشاريو);
};

بنية post
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_post;

  منطقي serialize (hb_serialize_context_t *c, منطقي glyph_نالا) ثابت
  {
    TRACE_SERIALIZE (هذا);
    post *post_مفرد = c->allocate_min<post> ();
    إذا (unlikely (!post_مفرد))  return_trace (خطأ);

    hb_memcpy (post_مفرد, هذا, post::min_سائيز);
    إذا (!glyph_نالا)
      return_trace (c->check_سونپيو (post_مفرد->نسخو.major, 3,
                                     HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)); // Version 3 does not have any glyph names.

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *post_مفرد = c->serializer->شروعات_embed<post> ();

    منطقي glyph_نالا = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_نالا;
    إذا (!serialize (c->serializer, glyph_نالا))
      return_trace (خطأ);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      خودڪار &MVAR = *c->plan->ذريعو->جدول.MVAR;
      خودڪار *جدول = post_مفرد;

      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_UNDERLINE_سائيز,   underlineThickness);
      HB_جوڙيو_MVAR_VAR (HB_OT_METRICS_ٽيگ_UNDERLINE_وٿي, underlinePosition);
    }
#جيڪڏهن_پورو

    Triple *axis_range;
    إذا (c->plan->user_axes_location.has (HB_ٽيگ ('s','l','n','t'), &axis_range))
    {
      عائم italic_angle = hb_max (-90.f, hb_min (axis_range->وچ, 90.f));
      إذا (post_مفرد->italicAngle.منزل_float () != italic_angle)
        post_مفرد->italicAngle.سيٽ_float (italic_angle);
    }

    إذا (glyph_نالا && نسخو.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (سچ);
  }

  بنية accelerator_t
  {
    دوست بنية post_V2_پڇ;

    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<post> (face);
      بنانشان صحيح جدول_ڊيگهائي = جدول.get_ڊيگهائي ();

      نسخو = جدول->نسخو.منزل_int ();
      إذا (نسخو != 0x00020000) أرجع;
      hb_barrier ();

      ثابت post_V2_پڇ &v2 = جدول->v2X;

      glyph_Name_اشاريو = &v2.glyph_Name_اشاريو;
      pool = &StructAfter<uint8_t> (v2.glyph_Name_اشاريو);

      ثابت uint8_t *پڇاڙي = (ثابت uint8_t *) (ثابت فراغ *) جدول + جدول_ڊيگهائي;
      اشاريو_منزل_وٿي.alloc (hb_min (face->get_انگ_glyphs (), جدول_ڊيگهائي / 8));
      لكل (ثابت uint8_t *معلومات = pool;
	   اشاريو_منزل_وٿي.ڊيگهائي < 65535 && معلومات < پڇاڙي && معلومات + *معلومات < پڇاڙي;
	   معلومات += 1 + *معلومات)
	اشاريو_منزل_وٿي.push (معلومات - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      جدول.destroy ();
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, بنانشان صحيح buf_len) ثابت
    {
      hb_بائيٽون_t s = ڳوليو_glyph_name (glyph);
      إذا (!s.ڊيگهائي) أرجع خطأ;
      إذا (!buf_len) أرجع سچ;
      بنانشان صحيح len = hb_min (buf_len - 1, s.ڊيگهائي);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      أرجع سچ;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      بنانشان صحيح ڳڻپ = get_glyph_ڳڻپ ();
      إذا (unlikely (!ڳڻپ)) أرجع خطأ;

      إذا (len < 0) len = لڙي_ڊيگهه (name);

      إذا (unlikely (!len)) أرجع خطأ;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      إذا (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (ڳڻپ, حجم (gids[0]));
	إذا (unlikely (!gids))
	  أرجع خطأ; /* Anything better?! */

	لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
	  gids[i] = i;
	خودڪار thiz = هذا;
	hb_array_t<uint16_t> (gids, ڳڻپ)
	  .qsort ([thiz] (ثابت uint16_t &a, ثابت uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    أرجع thiz->ڳوليو_glyph_name (b).cmp (thiz->ڳوليو_glyph_name (a));
	  });

	إذا (unlikely (!gids_sorted_by_name.cmpexch (خالي_اشارو, gids)))
	{
	  hb_free (gids);
	  اذهب retry;
	}
      }

      hb_بائيٽون_t st (name, len);
      خودڪار* gid = hb_bsearch (st, gids, ڳڻپ, حجم (gids[0]), cmp_ڪنجي, (فراغ *) هذا);
      إذا (gid)
      {
	*glyph = *gid;
	أرجع سچ;
      }

      أرجع خطأ;
    }

    hb_blob_ptr_t<post> جدول;

    محمي:

    بنانشان صحيح get_glyph_ڳڻپ () ثابت
    {
      إذا (نسخو == 0x00010000)
      {
        hb_barrier ();
	أرجع format1_نالا_ڊيگهائي;
      }

      إذا (نسخو == 0x00020000)
      {
        hb_barrier ();
	أرجع glyph_Name_اشاريو->len;
      }

      أرجع 0;
    }

    ساكن صحيح cmp_ڪنجي (ثابت فراغ *pk, ثابت فراغ *po, فراغ *arg)
    {
      ثابت accelerator_t *thiz = (ثابت accelerator_t *) arg;
      ثابت hb_بائيٽون_t *key = (ثابت hb_بائيٽون_t *) pk;
      uint16_t o = * (ثابت uint16_t *) po;
      أرجع thiz->ڳوليو_glyph_name (o).cmp (*key);
    }

    hb_بائيٽون_t ڳوليو_glyph_name (hb_codepoint_t glyph) ثابت
    {
      إذا (نسخو == 0x00010000)
      {
        hb_barrier ();
	إذا (glyph >= format1_نالا_ڊيگهائي)
	  أرجع hb_بائيٽون_t ();

	أرجع format1_نالا (glyph);
      }

      إذا (نسخو != 0x00020000)
	أرجع hb_بائيٽون_t ();
      hb_barrier ();

      إذا (glyph >= glyph_Name_اشاريو->len)
	أرجع hb_بائيٽون_t ();

      بنانشان صحيح اشاريو_2 = glyph_Name_اشاريو->arrayZ[glyph];
      إذا (اشاريو_2 < format1_نالا_ڊيگهائي)
	أرجع format1_نالا (اشاريو_2);
      اشاريو_2 -= format1_نالا_ڊيگهائي;

      إذا (اشاريو_2 >= اشاريو_منزل_وٿي.ڊيگهائي)
	أرجع hb_بائيٽون_t ();
      بنانشان صحيح وٿي = اشاريو_منزل_وٿي[اشاريو_2];

      ثابت uint8_t *معلومات = pool + وٿي;
      بنانشان صحيح name_ڊيگهائي = *معلومات;
      معلومات++;

      أرجع hb_بائيٽون_t ((ثابت حرف *) معلومات, name_ڊيگهائي);
    }

    خاص:
    uint32_t نسخو;
    ثابت Array16_جو<HBUINT16> *glyph_Name_اشاريو = خالي_اشارو;
    hb_ويڪٽر_t<uint32_t> اشاريو_منزل_وٿي;
    ثابت uint8_t *pool = خالي_اشارو;
    بدلجندڙ hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (نسخو.منزل_int () == 0x00010000 ||
		   (نسخو.منزل_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   نسخو.منزل_int () == 0x00030000));
  }

  عام:
  Fixed_نسخو<>نسخو;		/* 0x00010000 for version 1.0
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
  post_V2_پڇ	v2X;
  DEFINE_سائيز_MIN (32);
};

بنية post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_POST_TABLE_HH */
