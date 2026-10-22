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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-var-mvar-table.hh"

#define HB_STRING_ARRAY_NAME format1_names
#define HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#அந்தர்பாவ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#define HB_OT_TAG_post HB_TAG('p','o','s','t')


நாமாகாஶ OT {


கட்டமைப்பு postV2Tail
{
  friend கட்டமைப்பு post;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (glyphNameIndex.sanitize (c));
  }

  template<typename Iterator>
  தருக்க serialize (hb_serialize_context_t *c,
                  Iterator it,
                  மாறிலி வெறுமை* _post) மாறிலி;

  தருக்க subset (hb_subset_context_t *c) மாறிலி;

  பாதுகாக்கப்பட்ட:
  Array16Of<HBUINT16>	glyphNameIndex;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  பொது:
  DEFINE_SIZE_ARRAY (2, glyphNameIndex);
};

கட்டமைப்பு post
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_post;

  தருக்க serialize (hb_serialize_context_t *c, தருக்க glyph_names) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    post *post_பகாஎண் = c->allocate_min<post> ();
    எனில் (unlikely (!post_பகாஎண்))  return_trace (பொய்);

    hb_memcpy (post_பகாஎண், இது, post::min_size);
    எனில் (!glyph_names)
      return_trace (c->check_assign (post_பகாஎண்->version.major, 3,
                                     HB_SERIALIZE_ERROR_INT_OVERFLOW)); // Version 3 does not have any glyph names.

    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *post_பகாஎண் = c->serializer->start_embed<post> ();

    தருக்க glyph_names = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_NAMES;
    எனில் (!serialize (c->serializer, glyph_names))
      return_trace (பொய்);

#ifndef HB_NO_VAR
    எனில் (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = post_பகாஎண்;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_SIZE,   underlineThickness);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_OFFSET, underlinePosition);
    }
#endif

    Triple *axis_range;
    எனில் (c->plan->user_axes_location.has (HB_TAG ('s','l','n','t'), &axis_range))
    {
      தசம italic_angle = hb_max (-90.f, hb_min (axis_range->நடு, 90.f));
      எனில் (post_பகாஎண்->italicAngle.to_float () != italic_angle)
        post_பகாஎண்->italicAngle.set_float (italic_angle);
    }

    எனில் (glyph_names && version.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (உண்மை);
  }

  கட்டமைப்பு accelerator_t
  {
    friend கட்டமைப்பு postV2Tail;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<post> (face);
      unsigned முழு table_length = table.get_length ();

      version = table->version.to_int ();
      எனில் (version != 0x00020000) திரும்பு;
      hb_barrier ();

      மாறிலி postV2Tail &v2 = table->v2X;

      glyphNameIndex = &v2.glyphNameIndex;
      pool = &StructAfter<uint8_t> (v2.glyphNameIndex);

      மாறிலி uint8_t *end = (மாறிலி uint8_t *) (மாறிலி வெறுமை *) table + table_length;
      index_to_offset.alloc (hb_min (face->get_இலக்கம்_glyphs (), table_length / 8));
      க்கு (மாறிலி uint8_t *data = pool;
	   index_to_offset.length < 65535 && data < end && data + *data < end;
	   data += 1 + *data)
	index_to_offset.push (data - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      table.destroy ();
    }

    தருக்க get_glyph_name (hb_codepoint_t glyph,
			 எழுத்து *buf, unsigned முழு buf_len) மாறிலி
    {
      hb_bytes_t s = find_glyph_name (glyph);
      எனில் (!s.length) திரும்பு பொய்;
      எனில் (!buf_len) திரும்பு உண்மை;
      unsigned முழு len = hb_min (buf_len - 1, s.length);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      திரும்பு உண்மை;
    }

    தருக்க get_glyph_from_name (மாறிலி எழுத்து *name, முழு len,
			      hb_codepoint_t *glyph) மாறிலி
    {
      unsigned முழு மொத்தம் = get_glyph_மொத்தம் ();
      எனில் (unlikely (!மொத்தம்)) திரும்பு பொய்;

      எனில் (len < 0) len = strlen (name);

      எனில் (unlikely (!len)) திரும்பு பொய்;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      எனில் (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (மொத்தம், அளவு (gids[0]));
	எனில் (unlikely (!gids))
	  திரும்பு பொய்; /* Anything better?! */

	க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
	  gids[i] = i;
	auto thiz = இது;
	hb_array_t<uint16_t> (gids, மொத்தம்)
	  .qsort ([thiz] (மாறிலி uint16_t &a, மாறிலி uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    திரும்பு thiz->find_glyph_name (b).cmp (thiz->find_glyph_name (a));
	  });

	எனில் (unlikely (!gids_sorted_by_name.cmpexch (nullptr, gids)))
	{
	  hb_free (gids);
	  செல் retry;
	}
      }

      hb_bytes_t st (name, len);
      auto* gid = hb_bsearch (st, gids, மொத்தம், அளவு (gids[0]), cmp_சாவி, (வெறுமை *) இது);
      எனில் (gid)
      {
	*glyph = *gid;
	திரும்பு உண்மை;
      }

      திரும்பு பொய்;
    }

    hb_blob_ptr_t<post> table;

    பாதுகாக்கப்பட்ட:

    unsigned முழு get_glyph_மொத்தம் () மாறிலி
    {
      எனில் (version == 0x00010000)
      {
        hb_barrier ();
	திரும்பு format1_names_length;
      }

      எனில் (version == 0x00020000)
      {
        hb_barrier ();
	திரும்பு glyphNameIndex->len;
      }

      திரும்பு 0;
    }

    நிலையான முழு cmp_சாவி (மாறிலி வெறுமை *pk, மாறிலி வெறுமை *po, வெறுமை *arg)
    {
      மாறிலி accelerator_t *thiz = (மாறிலி accelerator_t *) arg;
      மாறிலி hb_bytes_t *key = (மாறிலி hb_bytes_t *) pk;
      uint16_t o = * (மாறிலி uint16_t *) po;
      திரும்பு thiz->find_glyph_name (o).cmp (*key);
    }

    hb_bytes_t find_glyph_name (hb_codepoint_t glyph) மாறிலி
    {
      எனில் (version == 0x00010000)
      {
        hb_barrier ();
	எனில் (glyph >= format1_names_length)
	  திரும்பு hb_bytes_t ();

	திரும்பு format1_names (glyph);
      }

      எனில் (version != 0x00020000)
	திரும்பு hb_bytes_t ();
      hb_barrier ();

      எனில் (glyph >= glyphNameIndex->len)
	திரும்பு hb_bytes_t ();

      unsigned முழு index = glyphNameIndex->arrayZ[glyph];
      எனில் (index < format1_names_length)
	திரும்பு format1_names (index);
      index -= format1_names_length;

      எனில் (index >= index_to_offset.length)
	திரும்பு hb_bytes_t ();
      unsigned முழு offset = index_to_offset[index];

      மாறிலி uint8_t *data = pool + offset;
      unsigned முழு name_length = *data;
      data++;

      திரும்பு hb_bytes_t ((மாறிலி எழுத்து *) data, name_length);
    }

    தனிப்பட்ட:
    uint32_t version;
    மாறிலி Array16Of<HBUINT16> *glyphNameIndex = nullptr;
    hb_vector_t<uint32_t> index_to_offset;
    மாறிலி uint8_t *pool = nullptr;
    mutable hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  தருக்க has_data () மாறிலி { திரும்பு version.to_int (); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  (version.to_int () == 0x00010000 ||
		   (version.to_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   version.to_int () == 0x00030000));
  }

  பொது:
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

கட்டமைப்பு post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#endif /* HB_OT_POST_TABLE_HH */
