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

#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-ot-var-mvar-table.hh"

#define HB_STRING_ARRAY_NAME format1_names
#define HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#অন্তর্ভাৱ "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#define HB_OT_TAG_post HB_TAG('p','o','s','t')


নামাকাশ OT {


সংরচনা postV2Tail
{
  friend সংরচনা post;

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (glyphNameIndex.sanitize (c));
  }

  template<typename Iterator>
  তার্কিক serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ধ্রুৱ শূন্য* _post) ধ্রুৱ;

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ;

  রক্ষিত:
  Array16Of<HBUINT16>	glyphNameIndex;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2, glyphNameIndex);
};

সংরচনা post
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_post;

  তার্কিক serialize (hb_serialize_context_t *c, তার্কিক glyph_names) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    post *post_মৌলিক = c->allocate_min<post> ();
    যদি (unlikely (!post_মৌলিক))  return_trace (মিছা);

    hb_memcpy (post_মৌলিক, এতত্, post::min_size);
    যদি (!glyph_names)
      return_trace (c->check_assign (post_মৌলিক->version.major, 3,
                                     HB_SERIALIZE_ERROR_INT_OVERFLOW)); // Version 3 does not have any glyph names.

    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *post_মৌলিক = c->serializer->start_embed<post> ();

    তার্কিক glyph_names = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_NAMES;
    যদি (!serialize (c->serializer, glyph_names))
      return_trace (মিছা);

#ifndef HB_NO_VAR
    যদি (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = post_মৌলিক;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_SIZE,   underlineThickness);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_OFFSET, underlinePosition);
    }
#endif

    Triple *axis_range;
    যদি (c->plan->user_axes_location.has (HB_TAG ('s','l','n','t'), &axis_range))
    {
      ভিন্ন italic_angle = hb_max (-90.f, hb_min (axis_range->মধ্য, 90.f));
      যদি (post_মৌলিক->italicAngle.to_float () != italic_angle)
        post_মৌলিক->italicAngle.set_float (italic_angle);
    }

    যদি (glyph_names && version.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (সঁচা);
  }

  সংরচনা accelerator_t
  {
    friend সংরচনা postV2Tail;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<post> (face);
      unsigned পূর্ণাঙ্ক table_length = table.get_length ();

      version = table->version.to_int ();
      যদি (version != 0x00020000) ঘূৰাই;
      hb_barrier ();

      ধ্রুৱ postV2Tail &v2 = table->v2X;

      glyphNameIndex = &v2.glyphNameIndex;
      pool = &StructAfter<uint8_t> (v2.glyphNameIndex);

      ধ্রুৱ uint8_t *end = (ধ্রুৱ uint8_t *) (ধ্রুৱ শূন্য *) table + table_length;
      index_to_offset.alloc (hb_min (face->get_অঙ্ক_glyphs (), table_length / 8));
      বাবে (ধ্রুৱ uint8_t *data = pool;
	   index_to_offset.length < 65535 && data < end && data + *data < end;
	   data += 1 + *data)
	index_to_offset.push (data - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      table.destroy ();
    }

    তার্কিক get_glyph_name (hb_codepoint_t glyph,
			 ৱর্ণ *buf, unsigned পূর্ণাঙ্ক buf_len) ধ্রুৱ
    {
      hb_bytes_t s = find_glyph_name (glyph);
      যদি (!s.length) ঘূৰাই মিছা;
      যদি (!buf_len) ঘূৰাই সঁচা;
      unsigned পূর্ণাঙ্ক len = hb_min (buf_len - 1, s.length);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      ঘূৰাই সঁচা;
    }

    তার্কিক get_glyph_from_name (ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক len,
			      hb_codepoint_t *glyph) ধ্রুৱ
    {
      unsigned পূর্ণাঙ্ক গুনতি = get_glyph_গুনতি ();
      যদি (unlikely (!গুনতি)) ঘূৰাই মিছা;

      যদি (len < 0) len = strlen (name);

      যদি (unlikely (!len)) ঘূৰাই মিছা;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      যদি (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (গুনতি, পরিমাণ (gids[0]));
	যদি (unlikely (!gids))
	  ঘূৰাই মিছা; /* Anything better?! */

	বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
	  gids[i] = i;
	auto thiz = এতত্;
	hb_array_t<uint16_t> (gids, গুনতি)
	  .qsort ([thiz] (ধ্রুৱ uint16_t &a, ধ্রুৱ uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    ঘূৰাই thiz->find_glyph_name (b).cmp (thiz->find_glyph_name (a));
	  });

	যদি (unlikely (!gids_sorted_by_name.cmpexch (nullptr, gids)))
	{
	  hb_free (gids);
	  গচ্ছ retry;
	}
      }

      hb_bytes_t st (name, len);
      auto* gid = hb_bsearch (st, gids, গুনতি, পরিমাণ (gids[0]), cmp_চাবি, (শূন্য *) এতত্);
      যদি (gid)
      {
	*glyph = *gid;
	ঘূৰাই সঁচা;
      }

      ঘূৰাই মিছা;
    }

    hb_blob_ptr_t<post> table;

    রক্ষিত:

    unsigned পূর্ণাঙ্ক get_glyph_গুনতি () ধ্রুৱ
    {
      যদি (version == 0x00010000)
      {
        hb_barrier ();
	ঘূৰাই format1_names_length;
      }

      যদি (version == 0x00020000)
      {
        hb_barrier ();
	ঘূৰাই glyphNameIndex->len;
      }

      ঘূৰাই 0;
    }

    স্থির পূর্ণাঙ্ক cmp_চাবি (ধ্রুৱ শূন্য *pk, ধ্রুৱ শূন্য *po, শূন্য *arg)
    {
      ধ্রুৱ accelerator_t *thiz = (ধ্রুৱ accelerator_t *) arg;
      ধ্রুৱ hb_bytes_t *key = (ধ্রুৱ hb_bytes_t *) pk;
      uint16_t o = * (ধ্রুৱ uint16_t *) po;
      ঘূৰাই thiz->find_glyph_name (o).cmp (*key);
    }

    hb_bytes_t find_glyph_name (hb_codepoint_t glyph) ধ্রুৱ
    {
      যদি (version == 0x00010000)
      {
        hb_barrier ();
	যদি (glyph >= format1_names_length)
	  ঘূৰাই hb_bytes_t ();

	ঘূৰাই format1_names (glyph);
      }

      যদি (version != 0x00020000)
	ঘূৰাই hb_bytes_t ();
      hb_barrier ();

      যদি (glyph >= glyphNameIndex->len)
	ঘূৰাই hb_bytes_t ();

      unsigned পূর্ণাঙ্ক index = glyphNameIndex->arrayZ[glyph];
      যদি (index < format1_names_length)
	ঘূৰাই format1_names (index);
      index -= format1_names_length;

      যদি (index >= index_to_offset.length)
	ঘূৰাই hb_bytes_t ();
      unsigned পূর্ণাঙ্ক offset = index_to_offset[index];

      ধ্রুৱ uint8_t *data = pool + offset;
      unsigned পূর্ণাঙ্ক name_length = *data;
      data++;

      ঘূৰাই hb_bytes_t ((ধ্রুৱ ৱর্ণ *) data, name_length);
    }

    গুপ্ত:
    uint32_t version;
    ধ্রুৱ Array16Of<HBUINT16> *glyphNameIndex = nullptr;
    hb_vector_t<uint32_t> index_to_offset;
    ধ্রুৱ uint8_t *pool = nullptr;
    mutable hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই version.to_int (); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  (version.to_int () == 0x00010000 ||
		   (version.to_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   version.to_int () == 0x00030000));
  }

  সার্ৱজনিক:
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

সংরচনা post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#endif /* HB_OT_POST_TABLE_HH */
