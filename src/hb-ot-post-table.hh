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

#شامل "hb-open-type.hh"
#شامل "hb-ot-var-mvar-table.hh"

#define HB_STRING_ARRAY_NAME format1_names
#define HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#شامل "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#define HB_OT_TAG_post HB_TAG('p','o','s','t')


فضاینام OT {


ساختار postV2Tail
{
  friend ساختار post;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (glyphNameIndex.sanitize (c));
  }

  template<typename Iterator>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ثابت تهی* _post) ثابت;

  منطقی subset (hb_subset_context_t *c) ثابت;

  محافظتشده:
  Array16Of<HBUINT16>	glyphNameIndex;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  عمومی:
  DEFINE_SIZE_ARRAY (2, glyphNameIndex);
};

ساختار post
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_post;

  منطقی serialize (hb_serialize_context_t *c, منطقی glyph_names) ثابت
  {
    TRACE_SERIALIZE (این);
    post *post_اول = c->allocate_min<post> ();
    اگر (unlikely (!post_اول))  return_trace (نادرست);

    hb_memcpy (post_اول, این, post::min_size);
    اگر (!glyph_names)
      return_trace (c->check_assign (post_اول->version.major, 3,
                                     HB_SERIALIZE_ERROR_INT_OVERFLOW)); // Version 3 does not have any glyph names.

    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *post_اول = c->serializer->start_embed<post> ();

    منطقی glyph_names = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_NAMES;
    اگر (!serialize (c->serializer, glyph_names))
      return_trace (نادرست);

#ifndef HB_NO_VAR
    اگر (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = post_اول;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_SIZE,   underlineThickness);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_OFFSET, underlinePosition);
    }
#endif

    Triple *axis_range;
    اگر (c->plan->user_axes_location.has (HB_TAG ('s','l','n','t'), &axis_range))
    {
      اعشاری italic_angle = hb_max (-90.f, hb_min (axis_range->میانه, 90.f));
      اگر (post_اول->italicAngle.to_float () != italic_angle)
        post_اول->italicAngle.set_float (italic_angle);
    }

    اگر (glyph_names && version.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (درست);
  }

  ساختار accelerator_t
  {
    friend ساختار postV2Tail;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<post> (face);
      unsigned صحیح table_length = table.get_length ();

      version = table->version.to_int ();
      اگر (version != 0x00020000) بازگشت;
      hb_barrier ();

      ثابت postV2Tail &v2 = table->v2X;

      glyphNameIndex = &v2.glyphNameIndex;
      pool = &StructAfter<uint8_t> (v2.glyphNameIndex);

      ثابت uint8_t *end = (ثابت uint8_t *) (ثابت تهی *) table + table_length;
      index_to_offset.alloc (hb_min (face->get_رقم_glyphs (), table_length / 8));
      برای (ثابت uint8_t *data = pool;
	   index_to_offset.length < 65535 && data < end && data + *data < end;
	   data += 1 + *data)
	index_to_offset.push (data - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      table.destroy ();
    }

    منطقی get_glyph_name (hb_codepoint_t glyph,
			 نویسه *buf, unsigned صحیح buf_len) ثابت
    {
      hb_bytes_t s = find_glyph_name (glyph);
      اگر (!s.length) بازگشت نادرست;
      اگر (!buf_len) بازگشت درست;
      unsigned صحیح len = hb_min (buf_len - 1, s.length);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      بازگشت درست;
    }

    منطقی get_glyph_from_name (ثابت نویسه *name, صحیح len,
			      hb_codepoint_t *glyph) ثابت
    {
      unsigned صحیح شمار = get_glyph_شمار ();
      اگر (unlikely (!شمار)) بازگشت نادرست;

      اگر (len < 0) len = strlen (name);

      اگر (unlikely (!len)) بازگشت نادرست;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      اگر (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (شمار, اندازه (gids[0]));
	اگر (unlikely (!gids))
	  بازگشت نادرست; /* Anything better?! */

	برای (unsigned صحیح i = 0; i < شمار; i++)
	  gids[i] = i;
	auto thiz = این;
	hb_array_t<uint16_t> (gids, شمار)
	  .qsort ([thiz] (ثابت uint16_t &a, ثابت uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    بازگشت thiz->find_glyph_name (b).cmp (thiz->find_glyph_name (a));
	  });

	اگر (unlikely (!gids_sorted_by_name.cmpexch (nullptr, gids)))
	{
	  hb_free (gids);
	  برو retry;
	}
      }

      hb_bytes_t st (name, len);
      auto* gid = hb_bsearch (st, gids, شمار, اندازه (gids[0]), cmp_کلید, (تهی *) این);
      اگر (gid)
      {
	*glyph = *gid;
	بازگشت درست;
      }

      بازگشت نادرست;
    }

    hb_blob_ptr_t<post> table;

    محافظتشده:

    unsigned صحیح get_glyph_شمار () ثابت
    {
      اگر (version == 0x00010000)
      {
        hb_barrier ();
	بازگشت format1_names_length;
      }

      اگر (version == 0x00020000)
      {
        hb_barrier ();
	بازگشت glyphNameIndex->len;
      }

      بازگشت 0;
    }

    ایستا صحیح cmp_کلید (ثابت تهی *pk, ثابت تهی *po, تهی *arg)
    {
      ثابت accelerator_t *thiz = (ثابت accelerator_t *) arg;
      ثابت hb_bytes_t *key = (ثابت hb_bytes_t *) pk;
      uint16_t o = * (ثابت uint16_t *) po;
      بازگشت thiz->find_glyph_name (o).cmp (*key);
    }

    hb_bytes_t find_glyph_name (hb_codepoint_t glyph) ثابت
    {
      اگر (version == 0x00010000)
      {
        hb_barrier ();
	اگر (glyph >= format1_names_length)
	  بازگشت hb_bytes_t ();

	بازگشت format1_names (glyph);
      }

      اگر (version != 0x00020000)
	بازگشت hb_bytes_t ();
      hb_barrier ();

      اگر (glyph >= glyphNameIndex->len)
	بازگشت hb_bytes_t ();

      unsigned صحیح index = glyphNameIndex->arrayZ[glyph];
      اگر (index < format1_names_length)
	بازگشت format1_names (index);
      index -= format1_names_length;

      اگر (index >= index_to_offset.length)
	بازگشت hb_bytes_t ();
      unsigned صحیح offset = index_to_offset[index];

      ثابت uint8_t *data = pool + offset;
      unsigned صحیح name_length = *data;
      data++;

      بازگشت hb_bytes_t ((ثابت نویسه *) data, name_length);
    }

    خصوصی:
    uint32_t version;
    ثابت Array16Of<HBUINT16> *glyphNameIndex = nullptr;
    hb_vector_t<uint32_t> index_to_offset;
    ثابت uint8_t *pool = nullptr;
    mutable hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  منطقی has_data () ثابت { بازگشت version.to_int (); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  (version.to_int () == 0x00010000 ||
		   (version.to_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   version.to_int () == 0x00030000));
  }

  عمومی:
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

ساختار post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#endif /* HB_OT_POST_TABLE_HH */
