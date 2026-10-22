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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-var-mvar-table.hh"

#define HB_STRING_ARRAY_NAME format1_names
#define HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#അന്തര്ഭാവ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#define HB_OT_TAG_post HB_TAG('p','o','s','t')


നാമാകാശ OT {


ഘടന postV2Tail
{
  friend ഘടന post;

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (glyphNameIndex.sanitize (c));
  }

  template<typename Iterator>
  തർക്കം serialize (hb_serialize_context_t *c,
                  Iterator it,
                  സ്ഥിരം ശൂന്യം* _post) സ്ഥിരം;

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം;

  സംരക്ഷിത:
  Array16Of<HBUINT16>	glyphNameIndex;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  പൊതു:
  DEFINE_SIZE_ARRAY (2, glyphNameIndex);
};

ഘടന post
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_post;

  തർക്കം serialize (hb_serialize_context_t *c, തർക്കം glyph_names) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    post *post_അഭാജ്യം = c->allocate_min<post> ();
    എങ്കിൽ (unlikely (!post_അഭാജ്യം))  return_trace (അസത്യം);

    hb_memcpy (post_അഭാജ്യം, ഇത്, post::min_size);
    എങ്കിൽ (!glyph_names)
      return_trace (c->check_assign (post_അഭാജ്യം->version.major, 3,
                                     HB_SERIALIZE_ERROR_INT_OVERFLOW)); // Version 3 does not have any glyph names.

    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *post_അഭാജ്യം = c->serializer->start_embed<post> ();

    തർക്കം glyph_names = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_NAMES;
    എങ്കിൽ (!serialize (c->serializer, glyph_names))
      return_trace (അസത്യം);

#ifndef HB_NO_VAR
    എങ്കിൽ (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = post_അഭാജ്യം;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_SIZE,   underlineThickness);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_OFFSET, underlinePosition);
    }
#endif

    Triple *axis_range;
    എങ്കിൽ (c->plan->user_axes_location.has (HB_TAG ('s','l','n','t'), &axis_range))
    {
      ദശാംശം italic_angle = hb_max (-90.f, hb_min (axis_range->നടുവ്, 90.f));
      എങ്കിൽ (post_അഭാജ്യം->italicAngle.to_float () != italic_angle)
        post_അഭാജ്യം->italicAngle.set_float (italic_angle);
    }

    എങ്കിൽ (glyph_names && version.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (സത്യം);
  }

  ഘടന accelerator_t
  {
    friend ഘടന postV2Tail;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<post> (face);
      unsigned പൂർണ്ണസംഖ്യ table_length = table.get_length ();

      version = table->version.to_int ();
      എങ്കിൽ (version != 0x00020000) തിരികെ;
      hb_barrier ();

      സ്ഥിരം postV2Tail &v2 = table->v2X;

      glyphNameIndex = &v2.glyphNameIndex;
      pool = &StructAfter<uint8_t> (v2.glyphNameIndex);

      സ്ഥിരം uint8_t *end = (സ്ഥിരം uint8_t *) (സ്ഥിരം ശൂന്യം *) table + table_length;
      index_to_offset.alloc (hb_min (face->get_അക്കം_glyphs (), table_length / 8));
      വേണ്ടി (സ്ഥിരം uint8_t *data = pool;
	   index_to_offset.length < 65535 && data < end && data + *data < end;
	   data += 1 + *data)
	index_to_offset.push (data - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      table.destroy ();
    }

    തർക്കം get_glyph_name (hb_codepoint_t glyph,
			 അക്ഷരം *buf, unsigned പൂർണ്ണസംഖ്യ buf_len) സ്ഥിരം
    {
      hb_bytes_t s = find_glyph_name (glyph);
      എങ്കിൽ (!s.length) തിരികെ അസത്യം;
      എങ്കിൽ (!buf_len) തിരികെ സത്യം;
      unsigned പൂർണ്ണസംഖ്യ len = hb_min (buf_len - 1, s.length);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      തിരികെ സത്യം;
    }

    തർക്കം get_glyph_from_name (സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ len,
			      hb_codepoint_t *glyph) സ്ഥിരം
    {
      unsigned പൂർണ്ണസംഖ്യ എണ്ണം = get_glyph_എണ്ണം ();
      എങ്കിൽ (unlikely (!എണ്ണം)) തിരികെ അസത്യം;

      എങ്കിൽ (len < 0) len = strlen (name);

      എങ്കിൽ (unlikely (!len)) തിരികെ അസത്യം;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      എങ്കിൽ (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (എണ്ണം, വലുപ്പം (gids[0]));
	എങ്കിൽ (unlikely (!gids))
	  തിരികെ അസത്യം; /* Anything better?! */

	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
	  gids[i] = i;
	auto thiz = ഇത്;
	hb_array_t<uint16_t> (gids, എണ്ണം)
	  .qsort ([thiz] (സ്ഥിരം uint16_t &a, സ്ഥിരം uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    തിരികെ thiz->find_glyph_name (b).cmp (thiz->find_glyph_name (a));
	  });

	എങ്കിൽ (unlikely (!gids_sorted_by_name.cmpexch (nullptr, gids)))
	{
	  hb_free (gids);
	  പോകുക retry;
	}
      }

      hb_bytes_t st (name, len);
      auto* gid = hb_bsearch (st, gids, എണ്ണം, വലുപ്പം (gids[0]), cmp_താക്കോൽ, (ശൂന്യം *) ഇത്);
      എങ്കിൽ (gid)
      {
	*glyph = *gid;
	തിരികെ സത്യം;
      }

      തിരികെ അസത്യം;
    }

    hb_blob_ptr_t<post> table;

    സംരക്ഷിത:

    unsigned പൂർണ്ണസംഖ്യ get_glyph_എണ്ണം () സ്ഥിരം
    {
      എങ്കിൽ (version == 0x00010000)
      {
        hb_barrier ();
	തിരികെ format1_names_length;
      }

      എങ്കിൽ (version == 0x00020000)
      {
        hb_barrier ();
	തിരികെ glyphNameIndex->len;
      }

      തിരികെ 0;
    }

    സ്ഥിതം പൂർണ്ണസംഖ്യ cmp_താക്കോൽ (സ്ഥിരം ശൂന്യം *pk, സ്ഥിരം ശൂന്യം *po, ശൂന്യം *arg)
    {
      സ്ഥിരം accelerator_t *thiz = (സ്ഥിരം accelerator_t *) arg;
      സ്ഥിരം hb_bytes_t *key = (സ്ഥിരം hb_bytes_t *) pk;
      uint16_t o = * (സ്ഥിരം uint16_t *) po;
      തിരികെ thiz->find_glyph_name (o).cmp (*key);
    }

    hb_bytes_t find_glyph_name (hb_codepoint_t glyph) സ്ഥിരം
    {
      എങ്കിൽ (version == 0x00010000)
      {
        hb_barrier ();
	എങ്കിൽ (glyph >= format1_names_length)
	  തിരികെ hb_bytes_t ();

	തിരികെ format1_names (glyph);
      }

      എങ്കിൽ (version != 0x00020000)
	തിരികെ hb_bytes_t ();
      hb_barrier ();

      എങ്കിൽ (glyph >= glyphNameIndex->len)
	തിരികെ hb_bytes_t ();

      unsigned പൂർണ്ണസംഖ്യ index = glyphNameIndex->arrayZ[glyph];
      എങ്കിൽ (index < format1_names_length)
	തിരികെ format1_names (index);
      index -= format1_names_length;

      എങ്കിൽ (index >= index_to_offset.length)
	തിരികെ hb_bytes_t ();
      unsigned പൂർണ്ണസംഖ്യ offset = index_to_offset[index];

      സ്ഥിരം uint8_t *data = pool + offset;
      unsigned പൂർണ്ണസംഖ്യ name_length = *data;
      data++;

      തിരികെ hb_bytes_t ((സ്ഥിരം അക്ഷരം *) data, name_length);
    }

    സ്വകാര്യ:
    uint32_t version;
    സ്ഥിരം Array16Of<HBUINT16> *glyphNameIndex = nullptr;
    hb_vector_t<uint32_t> index_to_offset;
    സ്ഥിരം uint8_t *pool = nullptr;
    mutable hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  (version.to_int () == 0x00010000 ||
		   (version.to_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   version.to_int () == 0x00030000));
  }

  പൊതു:
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

ഘടന post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#endif /* HB_OT_POST_TABLE_HH */
