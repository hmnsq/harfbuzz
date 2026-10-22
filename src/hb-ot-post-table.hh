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

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-var-mvar-table.hh"

#define HB_STRING_ARRAY_NAME format1_names
#define HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#ضمن "hb-string-array.hh"
#undef HB_STRING_ARRAY_LIST
#undef HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#define HB_OT_TAG_post HB_TAG('p','o','s','t')


نطاق OT {


بنية postV2Tail
{
  friend بنية post;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (glyphNameIndex.sanitize (c));
  }

  template<typename Iterator>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator it,
                  ثابت فراغ* _post) ثابت;

  منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  Array16Of<HBUINT16>	glyphNameIndex;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  عام:
  DEFINE_SIZE_ARRAY (2, glyphNameIndex);
};

بنية post
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_post;

  منطقي serialize (hb_serialize_context_t *c, منطقي glyph_names) ثابت
  {
    TRACE_SERIALIZE (هذا);
    post *post_أولي = c->allocate_min<post> ();
    إذا (unlikely (!post_أولي))  return_trace (خطأ);

    hb_memcpy (post_أولي, هذا, post::min_size);
    إذا (!glyph_names)
      return_trace (c->check_assign (post_أولي->version.major, 3,
                                     HB_SERIALIZE_ERROR_INT_OVERFLOW)); // Version 3 does not have any glyph names.

    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *post_أولي = c->serializer->start_embed<post> ();

    منطقي glyph_names = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_NAMES;
    إذا (!serialize (c->serializer, glyph_names))
      return_trace (خطأ);

#ifndef HB_NO_VAR
    إذا (c->plan->normalized_coords)
    {
      auto &MVAR = *c->plan->source->table.MVAR;
      auto *table = post_أولي;

      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_SIZE,   underlineThickness);
      HB_ADD_MVAR_VAR (HB_OT_METRICS_TAG_UNDERLINE_OFFSET, underlinePosition);
    }
#endif

    Triple *axis_range;
    إذا (c->plan->user_axes_location.has (HB_TAG ('s','l','n','t'), &axis_range))
    {
      عائم italic_angle = hb_max (-90.f, hb_min (axis_range->وسط, 90.f));
      إذا (post_أولي->italicAngle.to_float () != italic_angle)
        post_أولي->italicAngle.set_float (italic_angle);
    }

    إذا (glyph_names && version.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (true);
  }

  بنية accelerator_t
  {
    friend بنية postV2Tail;

    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<post> (face);
      unsigned صحيح table_length = table.get_length ();

      version = table->version.to_int ();
      إذا (version != 0x00020000) إرجاع;
      hb_barrier ();

      ثابت postV2Tail &v2 = table->v2X;

      glyphNameIndex = &v2.glyphNameIndex;
      pool = &StructAfter<uint8_t> (v2.glyphNameIndex);

      ثابت uint8_t *end = (ثابت uint8_t *) (ثابت فراغ *) table + table_length;
      index_to_offset.alloc (hb_min (face->get_رقم_glyphs (), table_length / 8));
      لكل (ثابت uint8_t *data = pool;
	   index_to_offset.length < 65535 && data < end && data + *data < end;
	   data += 1 + *data)
	index_to_offset.push (data - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      table.destroy ();
    }

    منطقي get_glyph_name (hb_codepoint_t glyph,
			 حرف *buf, unsigned صحيح buf_len) ثابت
    {
      hb_bytes_t s = find_glyph_name (glyph);
      إذا (!s.length) إرجاع خطأ;
      إذا (!buf_len) إرجاع true;
      unsigned صحيح len = hb_min (buf_len - 1, s.length);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      إرجاع true;
    }

    منطقي get_glyph_from_name (ثابت حرف *name, صحيح len,
			      hb_codepoint_t *glyph) ثابت
    {
      unsigned صحيح تعداد_2 = get_glyph_تعداد ();
      إذا (unlikely (!تعداد_2)) إرجاع خطأ;

      إذا (len < 0) len = strlen (name);

      إذا (unlikely (!len)) إرجاع خطأ;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      إذا (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (تعداد_2, حجم (gids[0]));
	إذا (unlikely (!gids))
	  إرجاع خطأ; /* Anything better?! */

	لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
	  gids[i] = i;
	auto thiz = هذا;
	hb_array_t<uint16_t> (gids, تعداد_2)
	  .qsort ([thiz] (ثابت uint16_t &a, ثابت uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    إرجاع thiz->find_glyph_name (b).cmp (thiz->find_glyph_name (a));
	  });

	إذا (unlikely (!gids_sorted_by_name.cmpexch (nullptr, gids)))
	{
	  hb_free (gids);
	  اذهب retry;
	}
      }

      hb_bytes_t st (name, len);
      auto* gid = hb_bsearch (st, gids, تعداد_2, حجم (gids[0]), cmp_مفتاح, (فراغ *) هذا);
      إذا (gid)
      {
	*glyph = *gid;
	إرجاع true;
      }

      إرجاع خطأ;
    }

    hb_blob_ptr_t<post> table;

    محمي:

    unsigned صحيح get_glyph_تعداد () ثابت
    {
      إذا (version == 0x00010000)
      {
        hb_barrier ();
	إرجاع format1_names_length;
      }

      إذا (version == 0x00020000)
      {
        hb_barrier ();
	إرجاع glyphNameIndex->len;
      }

      إرجاع 0;
    }

    ساكن صحيح cmp_مفتاح (ثابت فراغ *pk, ثابت فراغ *po, فراغ *arg)
    {
      ثابت accelerator_t *thiz = (ثابت accelerator_t *) arg;
      ثابت hb_bytes_t *key = (ثابت hb_bytes_t *) pk;
      uint16_t o = * (ثابت uint16_t *) po;
      إرجاع thiz->find_glyph_name (o).cmp (*key);
    }

    hb_bytes_t find_glyph_name (hb_codepoint_t glyph) ثابت
    {
      إذا (version == 0x00010000)
      {
        hb_barrier ();
	إذا (glyph >= format1_names_length)
	  إرجاع hb_bytes_t ();

	إرجاع format1_names (glyph);
      }

      إذا (version != 0x00020000)
	إرجاع hb_bytes_t ();
      hb_barrier ();

      إذا (glyph >= glyphNameIndex->len)
	إرجاع hb_bytes_t ();

      unsigned صحيح index = glyphNameIndex->arrayZ[glyph];
      إذا (index < format1_names_length)
	إرجاع format1_names (index);
      index -= format1_names_length;

      إذا (index >= index_to_offset.length)
	إرجاع hb_bytes_t ();
      unsigned صحيح offset = index_to_offset[index];

      ثابت uint8_t *data = pool + offset;
      unsigned صحيح name_length = *data;
      data++;

      إرجاع hb_bytes_t ((ثابت حرف *) data, name_length);
    }

    خاص:
    uint32_t version;
    ثابت Array16Of<HBUINT16> *glyphNameIndex = nullptr;
    hb_vector_t<uint32_t> index_to_offset;
    ثابت uint8_t *pool = nullptr;
    mutable hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  منطقي has_data () ثابت { إرجاع version.to_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  (version.to_int () == 0x00010000 ||
		   (version.to_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   version.to_int () == 0x00030000));
  }

  عام:
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

بنية post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#endif /* HB_OT_POST_TABLE_HH */
