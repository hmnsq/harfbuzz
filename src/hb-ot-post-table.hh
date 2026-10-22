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

#ifndef HB_OT_POST_TABLE_HH
#define HB_OT_POST_TABLE_HH

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-mvar-table.hh"

#define HB_STRING_ARRAY_NAME format1_names
#define HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#ಅನ್ತರ್ಭಾವ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#define HB_OT_TAG_post HB_TAG('p','o','s','t')


ನಾಮಾಕಾಶ OT {


ಸಂರಚನಾ postV2Tail
{
  friend ಸಂರಚನಾ post;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (glyphNameIndex.sanitize (c));
  }

  template<typename Iterator>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ಧ್ರುವ ಶೂನ್ಯ* _post) ಧ್ರುವ;

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ;

  ರಕ್ಷಿತ:
  Array16Of<HBUINT16>	glyphNameIndex;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, glyphNameIndex);
};

ಸಂರಚನಾ post
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_post;

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ತಾರ್ಕಿಕ glyph_names) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    post *post_ಅವಿಭಾಜ್ಯ = c->allocate_min<post> ();
    ಯದಿ (unlikely (!post_ಅವಿಭಾಜ್ಯ))  return_trace (ಅಸತ್ಯ);

    hb_memcpy (post_ಅವಿಭಾಜ್ಯ, ಏತತ್, post::min_size);
    ಯದಿ (!glyph_names)
      return_trace (c->check_assign (post_ಅವಿಭಾಜ್ಯ->version.major, 3,
                                     HB_SERIALIZE_ERROR_INT_OVERFLOW)); // Version 3 does not have any glyph names.

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *post_ಅವಿಭಾಜ್ಯ = c->serializer->start_embed<post> ();

    ತಾರ್ಕಿಕ glyph_names = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_NAMES;
    ಯದಿ (!serialize (c->serializer, glyph_names))
      return_trace (ಅಸತ್ಯ);

#ifndef HB_NO_VAR
    ಯದಿ (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = post_ಅವಿಭಾಜ್ಯ;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_SIZE,   underlineThickness);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_OFFSET, underlinePosition);
    }
#endif

    Triple *axis_range;
    ಯದಿ (c->plan->user_axes_location.has (HB_TAG ('s','l','n','t'), &axis_range))
    {
      ಭಿನ್ನ italic_angle = hb_max (-90.f, hb_min (axis_range->ಮಧ್ಯ, 90.f));
      ಯದಿ (post_ಅವಿಭಾಜ್ಯ->italicAngle.to_float () != italic_angle)
        post_ಅವಿಭಾಜ್ಯ->italicAngle.set_float (italic_angle);
    }

    ಯದಿ (glyph_names && version.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (ಸತ್ಯ);
  }

  ಸಂರಚನಾ accelerator_t
  {
    friend ಸಂರಚನಾ postV2Tail;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<post> (face);
      unsigned ಪೂರ್ಣಾಙ್ಕ table_length = table.get_length ();

      version = table->version.to_int ();
      ಯದಿ (version != 0x00020000) ಪ್ರತ್ಯಾಗಮ;
      hb_barrier ();

      ಧ್ರುವ postV2Tail &v2 = table->v2X;

      glyphNameIndex = &v2.glyphNameIndex;
      pool = &StructAfter<uint8_t> (v2.glyphNameIndex);

      ಧ್ರುವ uint8_t *end = (ಧ್ರುವ uint8_t *) (ಧ್ರುವ ಶೂನ್ಯ *) table + table_length;
      index_to_offset.alloc (hb_min (face->get_ಅಂಕೆ_glyphs (), table_length / 8));
      ಪ್ರತಿ (ಧ್ರುವ uint8_t *data = pool;
	   index_to_offset.length < 65535 && data < end && data + *data < end;
	   data += 1 + *data)
	index_to_offset.push (data - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      table.destroy ();
    }

    ತಾರ್ಕಿಕ get_glyph_name (hb_codepoint_t glyph,
			 ವರ್ಣ *buf, unsigned ಪೂರ್ಣಾಙ್ಕ buf_len) ಧ್ರುವ
    {
      hb_bytes_t s = find_glyph_name (glyph);
      ಯದಿ (!s.length) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (!buf_len) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      unsigned ಪೂರ್ಣಾಙ್ಕ len = hb_min (buf_len - 1, s.length);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ get_glyph_from_name (ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ len,
			      hb_codepoint_t *glyph) ಧ್ರುವ
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = get_glyph_ಎಣಿಕೆ ();
      ಯದಿ (unlikely (!ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (len < 0) len = strlen (name);

      ಯದಿ (unlikely (!len)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      ಯದಿ (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (ಎಣಿಕೆ, ಪರಿಮಾಣ (gids[0]));
	ಯದಿ (unlikely (!gids))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; /* Anything better?! */

	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
	  gids[i] = i;
	auto thiz = ಏತತ್;
	hb_array_t<uint16_t> (gids, ಎಣಿಕೆ)
	  .qsort ([thiz] (ಧ್ರುವ uint16_t &a, ಧ್ರುವ uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    ಪ್ರತ್ಯಾಗಮ thiz->find_glyph_name (b).cmp (thiz->find_glyph_name (a));
	  });

	ಯದಿ (unlikely (!gids_sorted_by_name.cmpexch (nullptr, gids)))
	{
	  hb_free (gids);
	  ಗಚ್ಛ retry;
	}
      }

      hb_bytes_t st (name, len);
      auto* gid = hb_bsearch (st, gids, ಎಣಿಕೆ, ಪರಿಮಾಣ (gids[0]), cmp_ಕೀಲಿ, (ಶೂನ್ಯ *) ಏತತ್);
      ಯದಿ (gid)
      {
	*glyph = *gid;
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }

      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    hb_blob_ptr_t<post> table;

    ರಕ್ಷಿತ:

    unsigned ಪೂರ್ಣಾಙ್ಕ get_glyph_ಎಣಿಕೆ () ಧ್ರುವ
    {
      ಯದಿ (version == 0x00010000)
      {
        hb_barrier ();
	ಪ್ರತ್ಯಾಗಮ format1_names_length;
      }

      ಯದಿ (version == 0x00020000)
      {
        hb_barrier ();
	ಪ್ರತ್ಯಾಗಮ glyphNameIndex->len;
      }

      ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp_ಕೀಲಿ (ಧ್ರುವ ಶೂನ್ಯ *pk, ಧ್ರುವ ಶೂನ್ಯ *po, ಶೂನ್ಯ *arg)
    {
      ಧ್ರುವ accelerator_t *thiz = (ಧ್ರುವ accelerator_t *) arg;
      ಧ್ರುವ hb_bytes_t *key = (ಧ್ರುವ hb_bytes_t *) pk;
      uint16_t o = * (ಧ್ರುವ uint16_t *) po;
      ಪ್ರತ್ಯಾಗಮ thiz->find_glyph_name (o).cmp (*key);
    }

    hb_bytes_t find_glyph_name (hb_codepoint_t glyph) ಧ್ರುವ
    {
      ಯದಿ (version == 0x00010000)
      {
        hb_barrier ();
	ಯದಿ (glyph >= format1_names_length)
	  ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();

	ಪ್ರತ್ಯಾಗಮ format1_names (glyph);
      }

      ಯದಿ (version != 0x00020000)
	ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();
      hb_barrier ();

      ಯದಿ (glyph >= glyphNameIndex->len)
	ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();

      unsigned ಪೂರ್ಣಾಙ್ಕ index = glyphNameIndex->arrayZ[glyph];
      ಯದಿ (index < format1_names_length)
	ಪ್ರತ್ಯಾಗಮ format1_names (index);
      index -= format1_names_length;

      ಯದಿ (index >= index_to_offset.length)
	ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();
      unsigned ಪೂರ್ಣಾಙ್ಕ offset = index_to_offset[index];

      ಧ್ರುವ uint8_t *data = pool + offset;
      unsigned ಪೂರ್ಣಾಙ್ಕ name_length = *data;
      data++;

      ಪ್ರತ್ಯಾಗಮ hb_bytes_t ((ಧ್ರುವ ವರ್ಣ *) data, name_length);
    }

    ಗುಪ್ತ:
    uint32_t version;
    ಧ್ರುವ Array16Of<HBUINT16> *glyphNameIndex = nullptr;
    hb_vector_t<uint32_t> index_to_offset;
    ಧ್ರುವ uint8_t *pool = nullptr;
    mutable hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  (version.to_int () == 0x00010000 ||
		   (version.to_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   version.to_int () == 0x00030000));
  }

  ಸಾರ್ವಜನಿಕ:
  FixedVersion<>version;		/* 0x00010000 for version 1.0
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
  postV2Tail	v2X;
  DEFINE_SIZE_MIN (32);
};

ಸಂರಚನಾ post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#endif /* HB_OT_POST_TABLE_HH */
