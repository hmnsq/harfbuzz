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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-var-mvar-table.hh"

#define HB_STRING_ARRAY_NAME format1_names
#define HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#અન્તર્ભાવ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#define HB_OT_TAG_post HB_TAG('p','o','s','t')


નામાકાશ OT {


સંરચના postV2Tail
{
  friend સંરચના post;

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (glyphNameIndex.sanitize (c));
  }

  template<typename Iterator>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  Iterator it,
                  અચલ શૂન્ય* _post) અચલ;

  તાર્કિક subset (hb_subset_context_t *c) અચલ;

  સુરક્ષિત:
  Array16Of<HBUINT16>	glyphNameIndex;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  જાહેર:
  DEFINE_SIZE_ARRAY (2, glyphNameIndex);
};

સંરચના post
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_post;

  તાર્કિક serialize (hb_serialize_context_t *c, તાર્કિક glyph_names) અચલ
  {
    TRACE_SERIALIZE (આ);
    post *post_અવિભાજ્ય = c->allocate_min<post> ();
    જો (unlikely (!post_અવિભાજ્ય))  return_trace (ખોટું);

    hb_memcpy (post_અવિભાજ્ય, આ, post::min_size);
    જો (!glyph_names)
      return_trace (c->check_assign (post_અવિભાજ્ય->version.major, 3,
                                     HB_SERIALIZE_ERROR_INT_OVERFLOW)); // Version 3 does not have any glyph names.

    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *post_અવિભાજ્ય = c->serializer->start_embed<post> ();

    તાર્કિક glyph_names = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_NAMES;
    જો (!serialize (c->serializer, glyph_names))
      return_trace (ખોટું);

#ifndef HB_NO_VAR
    જો (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = post_અવિભાજ્ય;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_SIZE,   underlineThickness);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_OFFSET, underlinePosition);
    }
#endif

    Triple *axis_range;
    જો (c->plan->user_axes_location.has (HB_TAG ('s','l','n','t'), &axis_range))
    {
      દશાંશ italic_angle = hb_max (-90.f, hb_min (axis_range->મધ્ય, 90.f));
      જો (post_અવિભાજ્ય->italicAngle.to_float () != italic_angle)
        post_અવિભાજ્ય->italicAngle.set_float (italic_angle);
    }

    જો (glyph_names && version.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (સાચું);
  }

  સંરચના accelerator_t
  {
    friend સંરચના postV2Tail;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<post> (face);
      unsigned પૂર્ણાંક table_length = table.get_length ();

      version = table->version.to_int ();
      જો (version != 0x00020000) પાછા;
      hb_barrier ();

      અચલ postV2Tail &v2 = table->v2X;

      glyphNameIndex = &v2.glyphNameIndex;
      pool = &StructAfter<uint8_t> (v2.glyphNameIndex);

      અચલ uint8_t *end = (અચલ uint8_t *) (અચલ શૂન્ય *) table + table_length;
      index_to_offset.alloc (hb_min (face->get_અંક_glyphs (), table_length / 8));
      માટે (અચલ uint8_t *data = pool;
	   index_to_offset.length < 65535 && data < end && data + *data < end;
	   data += 1 + *data)
	index_to_offset.push (data - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      table.destroy ();
    }

    તાર્કિક get_glyph_name (hb_codepoint_t glyph,
			 અક્ષર *buf, unsigned પૂર્ણાંક buf_len) અચલ
    {
      hb_bytes_t s = find_glyph_name (glyph);
      જો (!s.length) પાછા ખોટું;
      જો (!buf_len) પાછા સાચું;
      unsigned પૂર્ણાંક len = hb_min (buf_len - 1, s.length);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      પાછા સાચું;
    }

    તાર્કિક get_glyph_from_name (અચલ અક્ષર *name, પૂર્ણાંક len,
			      hb_codepoint_t *glyph) અચલ
    {
      unsigned પૂર્ણાંક ગણતરી = get_glyph_ગણતરી ();
      જો (unlikely (!ગણતરી)) પાછા ખોટું;

      જો (len < 0) len = strlen (name);

      જો (unlikely (!len)) પાછા ખોટું;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      જો (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (ગણતરી, કદ (gids[0]));
	જો (unlikely (!gids))
	  પાછા ખોટું; /* Anything better?! */

	માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
	  gids[i] = i;
	auto thiz = આ;
	hb_array_t<uint16_t> (gids, ગણતરી)
	  .qsort ([thiz] (અચલ uint16_t &a, અચલ uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    પાછા thiz->find_glyph_name (b).cmp (thiz->find_glyph_name (a));
	  });

	જો (unlikely (!gids_sorted_by_name.cmpexch (nullptr, gids)))
	{
	  hb_free (gids);
	  જાઓ retry;
	}
      }

      hb_bytes_t st (name, len);
      auto* gid = hb_bsearch (st, gids, ગણતરી, કદ (gids[0]), cmp_ચાવી, (શૂન્ય *) આ);
      જો (gid)
      {
	*glyph = *gid;
	પાછા સાચું;
      }

      પાછા ખોટું;
    }

    hb_blob_ptr_t<post> table;

    સુરક્ષિત:

    unsigned પૂર્ણાંક get_glyph_ગણતરી () અચલ
    {
      જો (version == 0x00010000)
      {
        hb_barrier ();
	પાછા format1_names_length;
      }

      જો (version == 0x00020000)
      {
        hb_barrier ();
	પાછા glyphNameIndex->len;
      }

      પાછા 0;
    }

    સ્થિર પૂર્ણાંક cmp_ચાવી (અચલ શૂન્ય *pk, અચલ શૂન્ય *po, શૂન્ય *arg)
    {
      અચલ accelerator_t *thiz = (અચલ accelerator_t *) arg;
      અચલ hb_bytes_t *key = (અચલ hb_bytes_t *) pk;
      uint16_t o = * (અચલ uint16_t *) po;
      પાછા thiz->find_glyph_name (o).cmp (*key);
    }

    hb_bytes_t find_glyph_name (hb_codepoint_t glyph) અચલ
    {
      જો (version == 0x00010000)
      {
        hb_barrier ();
	જો (glyph >= format1_names_length)
	  પાછા hb_bytes_t ();

	પાછા format1_names (glyph);
      }

      જો (version != 0x00020000)
	પાછા hb_bytes_t ();
      hb_barrier ();

      જો (glyph >= glyphNameIndex->len)
	પાછા hb_bytes_t ();

      unsigned પૂર્ણાંક index = glyphNameIndex->arrayZ[glyph];
      જો (index < format1_names_length)
	પાછા format1_names (index);
      index -= format1_names_length;

      જો (index >= index_to_offset.length)
	પાછા hb_bytes_t ();
      unsigned પૂર્ણાંક offset = index_to_offset[index];

      અચલ uint8_t *data = pool + offset;
      unsigned પૂર્ણાંક name_length = *data;
      data++;

      પાછા hb_bytes_t ((અચલ અક્ષર *) data, name_length);
    }

    ખાનગી:
    uint32_t version;
    અચલ Array16Of<HBUINT16> *glyphNameIndex = nullptr;
    hb_vector_t<uint32_t> index_to_offset;
    અચલ uint8_t *pool = nullptr;
    mutable hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  તાર્કિક has_data () અચલ { પાછા version.to_int (); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  (version.to_int () == 0x00010000 ||
		   (version.to_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   version.to_int () == 0x00030000));
  }

  જાહેર:
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

સંરચના post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#endif /* HB_OT_POST_TABLE_HH */
