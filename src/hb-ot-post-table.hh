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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-var-mvar-table.hh"

#define HB_STRING_ARRAY_NAME format1_names
#define HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#अन्तर्भाव "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#define HB_OT_TAG_post HB_TAG('p','o','s','t')


नामाकाश OT {


संरचना postV2Tail
{
  मित्र संरचना post;

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (glyphNameIndex.sanitize (c));
  }

  ढाँचा<typename Iterator>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator it,
                  अचर शून्य* _post) अचर;

  तार्किक subset (hb_subset_context_t *c) अचर;

  रक्षित:
  Array16Of<HBUINT16>	glyphNameIndex;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, glyphNameIndex);
};

संरचना post
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_post;

  तार्किक serialize (hb_serialize_context_t *c, तार्किक glyph_names) अचर
  {
    TRACE_SERIALIZE (एतत्);
    post *post_अविभाज्य = c->allocate_min<post> ();
    यदि (unlikely (!post_अविभाज्य))  return_trace (असत्य);

    hb_memcpy (post_अविभाज्य, एतत्, post::min_size);
    यदि (!glyph_names)
      return_trace (c->check_assign (post_अविभाज्य->version.major, 3,
                                     HB_SERIALIZE_ERROR_INT_OVERFLOW)); // Version 3 does not have any glyph names.

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *post_अविभाज्य = c->serializer->start_embed<post> ();

    तार्किक glyph_names = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_NAMES;
    यदि (!serialize (c->serializer, glyph_names))
      return_trace (असत्य);

#ifndef HB_NO_VAR
    यदि (c->plan->normalized_coords)
    {
      स्वतः &MVAR = *c->plan->source->table.MVAR;
      स्वतः *table = post_अविभाज्य;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_SIZE,   underlineThickness);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_OFFSET, underlinePosition);
    }
#endif

    Triple *axis_range;
    यदि (c->plan->user_axes_location.has (HB_TAG ('s','l','n','t'), &axis_range))
    {
      भिन्न italic_angle = hb_max (-90.f, hb_min (axis_range->बीच, 90.f));
      यदि (post_अविभाज्य->italicAngle.to_float () != italic_angle)
        post_अविभाज्य->italicAngle.set_float (italic_angle);
    }

    यदि (glyph_names && version.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (सत्य);
  }

  संरचना accelerator_t
  {
    मित्र संरचना postV2Tail;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<post> (face);
      अचिन्हित पूर्णाङ्क table_length = table.get_length ();

      version = table->version.to_int ();
      यदि (version != 0x00020000) फर्काऊ;
      hb_barrier ();

      अचर postV2Tail &v2 = table->v2X;

      glyphNameIndex = &v2.glyphNameIndex;
      pool = &StructAfter<uint8_t> (v2.glyphNameIndex);

      अचर uint8_t *end = (अचर uint8_t *) (अचर शून्य *) table + table_length;
      index_to_offset.alloc (hb_min (face->get_अङ्क_glyphs (), table_length / 8));
      लागि (अचर uint8_t *data = pool;
	   index_to_offset.length < 65535 && data < end && data + *data < end;
	   data += 1 + *data)
	index_to_offset.push (data - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      table.destroy ();
    }

    तार्किक get_glyph_name (hb_codepoint_t glyph,
			 वर्ण *buf, अचिन्हित पूर्णाङ्क buf_len) अचर
    {
      hb_bytes_t s = find_glyph_name (glyph);
      यदि (!s.length) फर्काऊ असत्य;
      यदि (!buf_len) फर्काऊ सत्य;
      अचिन्हित पूर्णाङ्क len = hb_min (buf_len - 1, s.length);
      म_नकलन (buf, s.arrayZ, len);
      buf[len] = '\0';
      फर्काऊ सत्य;
    }

    तार्किक get_glyph_from_name (अचर वर्ण *name, पूर्णाङ्क len,
			      hb_codepoint_t *glyph) अचर
    {
      अचिन्हित पूर्णाङ्क गन्ती = get_glyph_गन्ती ();
      यदि (unlikely (!गन्ती)) फर्काऊ असत्य;

      यदि (len < 0) len = म_माप (name);

      यदि (unlikely (!len)) फर्काऊ असत्य;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      यदि (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (गन्ती, परिमाण (gids[0]));
	यदि (unlikely (!gids))
	  फर्काऊ असत्य; /* Anything better?! */

	लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
	  gids[i] = i;
	स्वतः thiz = एतत्;
	hb_array_t<uint16_t> (gids, गन्ती)
	  .क्विक ([thiz] (अचर uint16_t &a, अचर uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    फर्काऊ thiz->find_glyph_name (b).cmp (thiz->find_glyph_name (a));
	  });

	यदि (unlikely (!gids_sorted_by_name.cmpexch (nullptr, gids)))
	{
	  hb_free (gids);
	  गच्छ retry;
	}
      }

      hb_bytes_t st (name, len);
      स्वतः* gid = hb_bsearch (st, gids, गन्ती, परिमाण (gids[0]), cmp_साँचो, (शून्य *) एतत्);
      यदि (gid)
      {
	*glyph = *gid;
	फर्काऊ सत्य;
      }

      फर्काऊ असत्य;
    }

    hb_blob_ptr_t<post> table;

    रक्षित:

    अचिन्हित पूर्णाङ्क get_glyph_गन्ती () अचर
    {
      यदि (version == 0x00010000)
      {
        hb_barrier ();
	फर्काऊ format1_names_length;
      }

      यदि (version == 0x00020000)
      {
        hb_barrier ();
	फर्काऊ glyphNameIndex->len;
      }

      फर्काऊ 0;
    }

    स्थिर पूर्णाङ्क cmp_साँचो (अचर शून्य *pk, अचर शून्य *po, शून्य *arg)
    {
      अचर accelerator_t *thiz = (अचर accelerator_t *) arg;
      अचर hb_bytes_t *key = (अचर hb_bytes_t *) pk;
      uint16_t o = * (अचर uint16_t *) po;
      फर्काऊ thiz->find_glyph_name (o).cmp (*key);
    }

    hb_bytes_t find_glyph_name (hb_codepoint_t glyph) अचर
    {
      यदि (version == 0x00010000)
      {
        hb_barrier ();
	यदि (glyph >= format1_names_length)
	  फर्काऊ hb_bytes_t ();

	फर्काऊ format1_names (glyph);
      }

      यदि (version != 0x00020000)
	फर्काऊ hb_bytes_t ();
      hb_barrier ();

      यदि (glyph >= glyphNameIndex->len)
	फर्काऊ hb_bytes_t ();

      अचिन्हित पूर्णाङ्क index = glyphNameIndex->arrayZ[glyph];
      यदि (index < format1_names_length)
	फर्काऊ format1_names (index);
      index -= format1_names_length;

      यदि (index >= index_to_offset.length)
	फर्काऊ hb_bytes_t ();
      अचिन्हित पूर्णाङ्क offset = index_to_offset[index];

      अचर uint8_t *data = pool + offset;
      अचिन्हित पूर्णाङ्क name_length = *data;
      data++;

      फर्काऊ hb_bytes_t ((अचर वर्ण *) data, name_length);
    }

    गुप्त:
    uint32_t version;
    अचर Array16Of<HBUINT16> *glyphNameIndex = nullptr;
    hb_vector_t<uint32_t> index_to_offset;
    अचर uint8_t *pool = nullptr;
    mutable hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  तार्किक has_data () अचर { फर्काऊ version.to_int (); }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  (version.to_int () == 0x00010000 ||
		   (version.to_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   version.to_int () == 0x00030000));
  }

  सार्वजनिक:
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

संरचना post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#endif /* HB_OT_POST_TABLE_HH */
