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

#अगर_अघोषित HB_OT_POST_TABLE_HH
#घोषणा HB_OT_POST_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-var-mvar-table.hh"

#घोषणा HB_STRING_ARRAY_NAME format1_names
#घोषणा HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#समावेश "hb-string-array.hh"
#अघोषित HB_STRING_ARRAY_LIST
#अघोषित HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#घोषणा HB_OT_TAG_post HB_TAG('p','o','s','t')


namespace OT {


काष्ठा post_V2_पुच्छ
{
  मित्र काष्ठा post;

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (glyph_Name_सूचकांक.sanitize (c));
  }

  ढाँचा<typename Iterator>
  bool serialize (hb_serialize_context_t *c,
                  Iterator it,
                  स्थिर व्योम* _post) स्थिर;

  bool subset (hb_subset_context_t *c) स्थिर;

  रक्षित:
  Array16Of<HBUINT16>	glyph_Name_सूचकांक;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  खुला:
  DEFINE_आकार_ARRAY (2, glyph_Name_सूचकांक);
};

काष्ठा post
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_post;

  bool serialize (hb_serialize_context_t *c, bool glyph_names) स्थिर
  {
    TRACE_SERIALIZE (यह);
    post *post_अभाज्य = c->allocate_min<post> ();
    अगर (unlikely (!post_अभाज्य))  return_trace (false);

    hb_memcpy (post_अभाज्य, यह, post::min_आकार);
    अगर (!glyph_names)
      return_trace (c->check_assign (post_अभाज्य->version.major, 3,
                                     HB_SERIALIZE_ERROR_INT_OVERFLOW)); // Version 3 does not have any glyph names.

    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *post_अभाज्य = c->serializer->start_embed<post> ();

    bool glyph_names = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_NAMES;
    अगर (!serialize (c->serializer, glyph_names))
      return_trace (false);

#अगर_अघोषित HB_NO_VAR
    अगर (c->plan->normalized_coords)
    {
      स्वतः &MVAR = *c->plan->source->table.MVAR;
      स्वतः *table = post_अभाज्य;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_आकार,   underlineThickness);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_OFFSET, underlinePosition);
    }
#पूर्ण_अगर

    Triple *axis_range;
    अगर (c->plan->user_axes_location.has (HB_TAG ('s','l','n','t'), &axis_range))
    {
      भग्न italic_angle = hb_max (-90.f, hb_min (axis_range->मध्य, 90.f));
      अगर (post_अभाज्य->italicAngle.to_float () != italic_angle)
        post_अभाज्य->italicAngle.set_float (italic_angle);
    }

    अगर (glyph_names && version.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (true);
  }

  काष्ठा accelerator_t
  {
    मित्र काष्ठा post_V2_पुच्छ;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<post> (face);
      अचिन्हित पूर्णांक table_लंबाई = table.get_लंबाई ();

      version = table->version.to_int ();
      अगर (version != 0x00020000) वापस;
      hb_barrier ();

      स्थिर post_V2_पुच्छ &v2 = table->v2X;

      glyph_Name_सूचकांक = &v2.glyph_Name_सूचकांक;
      pool = &StructAfter<uint8_t> (v2.glyph_Name_सूचकांक);

      स्थिर uint8_t *end = (स्थिर uint8_t *) (स्थिर व्योम *) table + table_लंबाई;
      सूचकांक_to_offset.alloc (hb_min (face->get_अंक_glyphs (), table_लंबाई / 8));
      क्रम (स्थिर uint8_t *तथ्य = pool;
	   सूचकांक_to_offset.लंबाई < 65535 && तथ्य < end && तथ्य + *तथ्य < end;
	   तथ्य += 1 + *तथ्य)
	सूचकांक_to_offset.push (तथ्य - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      table.destroy ();
    }

    bool get_glyph_name (hb_codepoint_t glyph,
			 अक्षर *buf, अचिन्हित पूर्णांक buf_len) स्थिर
    {
      hb_bytes_t s = find_glyph_name (glyph);
      अगर (!s.लंबाई) वापस false;
      अगर (!buf_len) वापस true;
      अचिन्हित पूर्णांक len = hb_min (buf_len - 1, s.लंबाई);
      म_नकलन (buf, s.arrayZ, len);
      buf[len] = '\0';
      वापस true;
    }

    bool get_glyph_from_name (स्थिर अक्षर *name, पूर्णांक len,
			      hb_codepoint_t *glyph) स्थिर
    {
      अचिन्हित पूर्णांक गिनती = get_glyph_गिनती ();
      अगर (unlikely (!गिनती)) वापस false;

      अगर (len < 0) len = म_माप (name);

      अगर (unlikely (!len)) वापस false;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      अगर (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (गिनती, माप (gids[0]));
	अगर (unlikely (!gids))
	  वापस false; /* Anything better?! */

	क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
	  gids[i] = i;
	स्वतः thiz = यह;
	hb_array_t<uint16_t> (gids, गिनती)
	  .क्विक ([thiz] (स्थिर uint16_t &a, स्थिर uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    वापस thiz->find_glyph_name (b).cmp (thiz->find_glyph_name (a));
	  });

	अगर (unlikely (!gids_sorted_by_name.cmpexch (nullptr, gids)))
	{
	  hb_free (gids);
	  जाओ retry;
	}
      }

      hb_bytes_t st (name, len);
      स्वतः* gid = hb_bsearch (st, gids, गिनती, माप (gids[0]), cmp_कुंजी, (व्योम *) यह);
      अगर (gid)
      {
	*glyph = *gid;
	वापस true;
      }

      वापस false;
    }

    hb_blob_ptr_t<post> table;

    रक्षित:

    अचिन्हित पूर्णांक get_glyph_गिनती () स्थिर
    {
      अगर (version == 0x00010000)
      {
        hb_barrier ();
	वापस format1_names_लंबाई;
      }

      अगर (version == 0x00020000)
      {
        hb_barrier ();
	वापस glyph_Name_सूचकांक->len;
      }

      वापस 0;
    }

    अटल पूर्णांक cmp_कुंजी (स्थिर व्योम *pk, स्थिर व्योम *po, व्योम *arg)
    {
      स्थिर accelerator_t *thiz = (स्थिर accelerator_t *) arg;
      स्थिर hb_bytes_t *key = (स्थिर hb_bytes_t *) pk;
      uint16_t o = * (स्थिर uint16_t *) po;
      वापस thiz->find_glyph_name (o).cmp (*key);
    }

    hb_bytes_t find_glyph_name (hb_codepoint_t glyph) स्थिर
    {
      अगर (version == 0x00010000)
      {
        hb_barrier ();
	अगर (glyph >= format1_names_लंबाई)
	  वापस hb_bytes_t ();

	वापस format1_names (glyph);
      }

      अगर (version != 0x00020000)
	वापस hb_bytes_t ();
      hb_barrier ();

      अगर (glyph >= glyph_Name_सूचकांक->len)
	वापस hb_bytes_t ();

      अचिन्हित पूर्णांक सूचकांक_2 = glyph_Name_सूचकांक->arrayZ[glyph];
      अगर (सूचकांक_2 < format1_names_लंबाई)
	वापस format1_names (सूचकांक_2);
      सूचकांक_2 -= format1_names_लंबाई;

      अगर (सूचकांक_2 >= सूचकांक_to_offset.लंबाई)
	वापस hb_bytes_t ();
      अचिन्हित पूर्णांक offset = सूचकांक_to_offset[सूचकांक_2];

      स्थिर uint8_t *तथ्य = pool + offset;
      अचिन्हित पूर्णांक name_लंबाई = *तथ्य;
      तथ्य++;

      वापस hb_bytes_t ((स्थिर अक्षर *) तथ्य, name_लंबाई);
    }

    निजी:
    uint32_t version;
    स्थिर Array16Of<HBUINT16> *glyph_Name_सूचकांक = nullptr;
    hb_vector_t<uint32_t> सूचकांक_to_offset;
    स्थिर uint8_t *pool = nullptr;
    mutable hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  bool has_तथ्य () स्थिर { वापस version.to_int (); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  (version.to_int () == 0x00010000 ||
		   (version.to_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   version.to_int () == 0x00030000));
  }

  खुला:
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
  post_V2_पुच्छ	v2X;
  DEFINE_आकार_MIN (32);
};

काष्ठा post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#पूर्ण_अगर /* HB_OT_POST_TABLE_HH */
