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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_POST_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_POST_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-mvar-table.hh"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STRING_ARRAY_NAME format1_ਨਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-string-array.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STRING_ARRAY_LIST
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_post HB_ਟੈਗ('p','o','s','t')


ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ post_V2_ਪੂਛ
{
  ਮਿੱਤਰ ਬਣਤਰ post;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (glyph_Name_ਸੂਚਕਾਂਕ.sanitize (c));
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ਅਚਲ ਖਾਲੀ* _post) ਅਚਲ;

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ;

  ਸੁਰੱਖਿਅਤ:
  Array16_ਦਾ<HBUINT16>	glyph_Name_ਸੂਚਕਾਂਕ;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, glyph_Name_ਸੂਚਕਾਂਕ);
};

ਬਣਤਰ post
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_post;

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਤਾਰਕਿਕ glyph_ਨਾਂ) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    post *post_ਅਭਾਜ = c->allocate_min<post> ();
    ਜੇ (unlikely (!post_ਅਭਾਜ))  return_trace (ਝੂਠ);

    hb_memcpy (post_ਅਭਾਜ, ਇਹ, post::min_ਮਾਪ);
    ਜੇ (!glyph_ਨਾਂ)
      return_trace (c->check_ਸੌਂਪੋ (post_ਅਭਾਜ->ਸੰਸਕਰਣ.major, 3,
                                     HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)); // Version 3 does not have any glyph names.

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *post_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed<post> ();

    ਤਾਰਕਿਕ glyph_ਨਾਂ = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_ਨਾਂ;
    ਜੇ (!serialize (c->serializer, glyph_ਨਾਂ))
      return_trace (ਝੂਠ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (c->plan->normalized_coords)
    {
      ਸਵੈਚਾਲਿਤ &MVAR = *c->plan->ਸਰੋਤ->ਸਾਰਣੀ.MVAR;
      ਸਵੈਚਾਲਿਤ *ਸਾਰਣੀ = post_ਅਭਾਜ;

      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_UNDERLINE_ਮਾਪ,   underlineThickness);
      HB_ਜੋੜੋ_MVAR_VAR (HB_OT_METRICS_ਟੈਗ_UNDERLINE_ਵਿੱਥ, underlinePosition);
    }
#ਜੇ_ਅੰਤ

    Triple *axis_range;
    ਜੇ (c->plan->user_axes_location.has (HB_ਟੈਗ ('s','l','n','t'), &axis_range))
    {
      ਦਸ਼ਮਲਵ italic_angle = hb_max (-90.f, hb_min (axis_range->ਵਿਚਕਾਰ, 90.f));
      ਜੇ (post_ਅਭਾਜ->italicAngle.ਮੰਜ਼ਿਲ_float () != italic_angle)
        post_ਅਭਾਜ->italicAngle.ਸੈੱਟ_float (italic_angle);
    }

    ਜੇ (glyph_ਨਾਂ && ਸੰਸਕਰਣ.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (ਸੱਚ);
  }

  ਬਣਤਰ accelerator_t
  {
    ਮਿੱਤਰ ਬਣਤਰ post_V2_ਪੂਛ;

    accelerator_t (hb_face_t *face)
    {
      ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<post> (face);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਾਰਣੀ_ਲੰਮਾਈ = ਸਾਰਣੀ.get_ਲੰਮਾਈ ();

      ਸੰਸਕਰਣ = ਸਾਰਣੀ->ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int ();
      ਜੇ (ਸੰਸਕਰਣ != 0x00020000) ਵਾਪਸ;
      hb_barrier ();

      ਅਚਲ post_V2_ਪੂਛ &v2 = ਸਾਰਣੀ->v2X;

      glyph_Name_ਸੂਚਕਾਂਕ = &v2.glyph_Name_ਸੂਚਕਾਂਕ;
      pool = &StructAfter<uint8_t> (v2.glyph_Name_ਸੂਚਕਾਂਕ);

      ਅਚਲ uint8_t *ਸਿਰਾ = (ਅਚਲ uint8_t *) (ਅਚਲ ਖਾਲੀ *) ਸਾਰਣੀ + ਸਾਰਣੀ_ਲੰਮਾਈ;
      ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਵਿੱਥ.alloc (hb_min (face->get_ਅੰਕ_glyphs (), ਸਾਰਣੀ_ਲੰਮਾਈ / 8));
      ਲਈ (ਅਚਲ uint8_t *ਅੰਕੜੇ = pool;
	   ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਵਿੱਥ.ਲੰਮਾਈ < 65535 && ਅੰਕੜੇ < ਸਿਰਾ && ਅੰਕੜੇ + *ਅੰਕੜੇ < ਸਿਰਾ;
	   ਅੰਕੜੇ += 1 + *ਅੰਕੜੇ)
	ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਵਿੱਥ.push (ਅੰਕੜੇ - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      ਸਾਰਣੀ.destroy ();
    }

    ਤਾਰਕਿਕ get_glyph_name (hb_codepoint_t glyph,
			 ਅੱਖਰ *buf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ buf_len) ਅਚਲ
    {
      hb_ਬਾਈਟਾਂ_t s = ਲੱਭੋ_glyph_name (glyph);
      ਜੇ (!s.ਲੰਮਾਈ) ਵਾਪਸ ਝੂਠ;
      ਜੇ (!buf_len) ਵਾਪਸ ਸੱਚ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = hb_min (buf_len - 1, s.ਲੰਮਾਈ);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      ਵਾਪਸ ਸੱਚ;
    }

    ਤਾਰਕਿਕ get_glyph_from_name (ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len,
			      hb_codepoint_t *glyph) ਅਚਲ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = get_glyph_ਗਿਣਤੀ ();
      ਜੇ (unlikely (!ਗਿਣਤੀ_2)) ਵਾਪਸ ਝੂਠ;

      ਜੇ (len < 0) len = ਲੜੀ_ਲੰਬਾਈ (name);

      ਜੇ (unlikely (!len)) ਵਾਪਸ ਝੂਠ;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      ਜੇ (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (ਗਿਣਤੀ_2, ਆਕਾਰ (gids[0]));
	ਜੇ (unlikely (!gids))
	  ਵਾਪਸ ਝੂਠ; /* Anything better?! */

	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
	  gids[i] = i;
	ਸਵੈਚਾਲਿਤ thiz = ਇਹ;
	hb_array_t<uint16_t> (gids, ਗਿਣਤੀ_2)
	  .qsort ([thiz] (ਅਚਲ uint16_t &a, ਅਚਲ uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    ਵਾਪਸ thiz->ਲੱਭੋ_glyph_name (b).cmp (thiz->ਲੱਭੋ_glyph_name (a));
	  });

	ਜੇ (unlikely (!gids_sorted_by_name.cmpexch (ਖਾਲੀ_ਸੂਚਕ, gids)))
	{
	  hb_free (gids);
	  ਜਾਓ retry;
	}
      }

      hb_ਬਾਈਟਾਂ_t st (name, len);
      ਸਵੈਚਾਲਿਤ* gid = hb_bsearch (st, gids, ਗਿਣਤੀ_2, ਆਕਾਰ (gids[0]), cmp_ਕੁੰਜੀ, (ਖਾਲੀ *) ਇਹ);
      ਜੇ (gid)
      {
	*glyph = *gid;
	ਵਾਪਸ ਸੱਚ;
      }

      ਵਾਪਸ ਝੂਠ;
    }

    hb_blob_ptr_t<post> ਸਾਰਣੀ;

    ਸੁਰੱਖਿਅਤ:

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_glyph_ਗਿਣਤੀ () ਅਚਲ
    {
      ਜੇ (ਸੰਸਕਰਣ == 0x00010000)
      {
        hb_barrier ();
	ਵਾਪਸ format1_ਨਾਂ_ਲੰਮਾਈ;
      }

      ਜੇ (ਸੰਸਕਰਣ == 0x00020000)
      {
        hb_barrier ();
	ਵਾਪਸ glyph_Name_ਸੂਚਕਾਂਕ->len;
      }

      ਵਾਪਸ 0;
    }

    ਸਥਿਰ ਪੂਰਨ cmp_ਕੁੰਜੀ (ਅਚਲ ਖਾਲੀ *pk, ਅਚਲ ਖਾਲੀ *po, ਖਾਲੀ *arg)
    {
      ਅਚਲ accelerator_t *thiz = (ਅਚਲ accelerator_t *) arg;
      ਅਚਲ hb_ਬਾਈਟਾਂ_t *key = (ਅਚਲ hb_ਬਾਈਟਾਂ_t *) pk;
      uint16_t o = * (ਅਚਲ uint16_t *) po;
      ਵਾਪਸ thiz->ਲੱਭੋ_glyph_name (o).cmp (*key);
    }

    hb_ਬਾਈਟਾਂ_t ਲੱਭੋ_glyph_name (hb_codepoint_t glyph) ਅਚਲ
    {
      ਜੇ (ਸੰਸਕਰਣ == 0x00010000)
      {
        hb_barrier ();
	ਜੇ (glyph >= format1_ਨਾਂ_ਲੰਮਾਈ)
	  ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();

	ਵਾਪਸ format1_ਨਾਂ (glyph);
      }

      ਜੇ (ਸੰਸਕਰਣ != 0x00020000)
	ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();
      hb_barrier ();

      ਜੇ (glyph >= glyph_Name_ਸੂਚਕਾਂਕ->len)
	ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = glyph_Name_ਸੂਚਕਾਂਕ->arrayZ[glyph];
      ਜੇ (ਸੂਚਕਾਂਕ_2 < format1_ਨਾਂ_ਲੰਮਾਈ)
	ਵਾਪਸ format1_ਨਾਂ (ਸੂਚਕਾਂਕ_2);
      ਸੂਚਕਾਂਕ_2 -= format1_ਨਾਂ_ਲੰਮਾਈ;

      ਜੇ (ਸੂਚਕਾਂਕ_2 >= ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਵਿੱਥ.ਲੰਮਾਈ)
	ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਵਿੱਥ[ਸੂਚਕਾਂਕ_2];

      ਅਚਲ uint8_t *ਅੰਕੜੇ = pool + ਵਿੱਥ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ name_ਲੰਮਾਈ = *ਅੰਕੜੇ;
      ਅੰਕੜੇ++;

      ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ((ਅਚਲ ਅੱਖਰ *) ਅੰਕੜੇ, name_ਲੰਮਾਈ);
    }

    ਨਿਜੀ:
    uint32_t ਸੰਸਕਰਣ;
    ਅਚਲ Array16_ਦਾ<HBUINT16> *glyph_Name_ਸੂਚਕਾਂਕ = ਖਾਲੀ_ਸੂਚਕ;
    hb_ਸਦਿਸ਼_t<uint32_t> ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_ਵਿੱਥ;
    ਅਚਲ uint8_t *pool = ਖਾਲੀ_ਸੂਚਕ;
    ਬਦਲਣਯੋਗ hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () == 0x00010000 ||
		   (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () == 0x00030000));
  }

  ਜਨਤਕ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;		/* 0x00010000 for version 1.0
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
  post_V2_ਪੂਛ	v2X;
  DEFINE_ਮਾਪ_MIN (32);
};

ਬਣਤਰ post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_POST_TABLE_HH */
