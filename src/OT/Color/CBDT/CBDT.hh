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
 * Google Author(s): Seigo Nonaka, Calder Kitagawa
 */

#ifndef OT_COLOR_CBDT_CBDT_HH
#define OT_COLOR_CBDT_CBDT_HH

#अन्तर्भाव "../../../hb-open-type.hh"
#अन्तर्भाव "../../../hb-paint.hh"

/*
 * CBLC -- Color Bitmap Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cblc
 * https://docs.microsoft.com/en-us/typography/opentype/spec/eblc
 * CBDT -- Color Bitmap Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cbdt
 * https://docs.microsoft.com/en-us/typography/opentype/spec/ebdt
 */
#define HB_OT_TAG_CBLC HB_TAG('C','B','L','C')
#define HB_OT_TAG_CBDT HB_TAG('C','B','D','T')


नामाकाश OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
संरचना cbdt_dedup_context_t
{
  संरचना entry_t
  {
    unsigned पूर्णाङ्क base;		/* Offset of the data in cbdt_prime. */
    unsigned पूर्णाङ्क lstart;	/* Start index in lengths. */
    unsigned पूर्णाङ्क lcount;	/* Number of length entries. */
  };

  स्थिर uint64_t hash (unsigned पूर्णाङ्क image_format,
			ध्रुव hb_vector_t<वर्ण> &data,
			ध्रुव hb_vector_t<unsigned पूर्णाङ्क> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((ध्रुव वर्ण *) lengths.arrayZ,
				      lengths.length * परिमाण (unsigned पूर्णाङ्क)));
    पटिनिवत्त ((uint64_t) image_format << 32) | h;
  }

  तार्किक find (uint64_t key,
	     ध्रुव hb_vector_t<वर्ण> &data,
	     ध्रुव hb_vector_t<unsigned पूर्णाङ्क> &lengths,
	     ध्रुव hb_vector_t<वर्ण> &cbdt_अविभाज्यः,
	     unsigned पूर्णाङ्क *base /* OUT */) ध्रुव
  {
    unsigned पूर्णाङ्क *idx_p = nullptr;
    सचे (!map.has (key, &idx_p)) पटिनिवत्त मिच्छा;
    ध्रुव entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    सचे (e.lcount != lengths.length) पटिनिवत्त मिच्छा;
    सचे (unlikely (e.base + data.length > (unsigned पूर्णाङ्क) cbdt_अविभाज्यः.length)) पटिनिवत्त मिच्छा;
    सचे (hb_memcmp (cbdt_अविभाज्यः.arrayZ + e.base, data.arrayZ, data.length)) पटिनिवत्त मिच्छा;
    सचे (hb_memcmp (एतत्->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * परिमाण (unsigned पूर्णाङ्क)))
      पटिनिवत्त मिच्छा;
    *base = e.base;
    पटिनिवत्त सच्च;
  }

  तार्किक add (uint64_t key,
	    unsigned पूर्णाङ्क base,
	    ध्रुव hb_vector_t<unsigned पूर्णाङ्क> &lengths)
  {
    सचे (unlikely (entries.in_error () || एतत्->lengths.in_error () || map.in_error ()))
      पटिनिवत्त मिच्छा;
    entry_t e = {base, एतत्->lengths.length, lengths.length};
    सचे (unlikely (!entries.प्रक्षिप_or_fail (e))) पटिनिवत्त मिच्छा;
    अत्थाय (unsigned पूर्णाङ्क length : lengths)
      सचे (unlikely (!एतत्->lengths.प्रक्षिप_or_fail (length))) पटिनिवत्त मिच्छा;
    पटिनिवत्त map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned पूर्णाङ्क> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned पूर्णाङ्क> lengths; /* Concatenated per-entry lengths. */
};

संरचना cblc_bitmap_size_subset_context_t
{
  ध्रुव वर्ण *cbdt;
  unsigned पूर्णाङ्क cbdt_length;
  hb_vector_t<वर्ण> *cbdt_अविभाज्यः;
  unsigned पूर्णाङ्क size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned पूर्णाङ्क अङ्कः_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

स्थिर अन्तःस्थ तार्किक
_copy_data_to_cbdt (hb_vector_t<वर्ण> *cbdt_अविभाज्यः,
		    ध्रुव शून्य        *data,
		    unsigned           length)
{
  unsigned पूर्णाङ्क new_len = cbdt_अविभाज्यः->length + length;
  सचे (unlikely (!cbdt_अविभाज्यः->alloc (new_len))) पटिनिवत्त मिच्छा;
  hb_memcpy (cbdt_अविभाज्यः->arrayZ + cbdt_अविभाज्यः->length, data, length);
  cbdt_अविभाज्यः->length = new_len;
  पटिनिवत्त सच्च;
}

संरचना SmallGlyphMetrics
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य get_extents (hb_font_t *font, hb_glyph_extents_t *extents, तार्किक scale) ध्रुव
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<पूर्णाङ्क> (height);

    सचे (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (5);
};

संरचना BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना SBit_पङ्क्तिः_Metrics
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBINT8	ascender;
  HBINT8	decender;
  HBUINT8	widthMax;
  HBINT8	caretSlopeNumerator;
  HBINT8	caretSlopeDenominator;
  HBINT8	caretOffset;
  HBINT8	minOriginSB;
  HBINT8	minAdvanceSB;
  HBINT8	maxBeforeBL;
  HBINT8	minAfterBL;
  HBINT8	padding1;
  HBINT8	padding2;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

संरचना IndexSubtableHeader
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
संरचना IndexSubtableFormat1Or3
{
  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क glyph_गणनम्) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  offsetArrayZ.sanitize (c, glyph_गणनम् + 1));
  }

  तार्किक get_image_data (unsigned पूर्णाङ्क idx,
		       unsigned पूर्णाङ्क *offset,
		       unsigned पूर्णाङ्क *length) ध्रुव
  {
    सचे (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      पटिनिवत्त मिच्छा;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    पटिनिवत्त सच्च;
  }

  तार्किक add_offset (hb_serialize_context_t *c,
		   unsigned पूर्णाङ्क offset,
		   unsigned पूर्णाङ्क *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (एतत्);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += परिमाण (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((तार्किक) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

संरचना IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
संरचना IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

संरचना IndexSubtable
{
  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क glyph_गणनम्) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!u.header.sanitize (c)) return_trace (मिच्छा);
    hb_barrier ();
    विकल्प (u.header.indexFormat)
    {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_गणनम्));
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_गणनम्));
    स्वभाव:return_trace (सच्च);
    }
  }

  तार्किक
  finish_subtable (hb_serialize_context_t *c,
		   unsigned पूर्णाङ्क end_offset,
		   unsigned पूर्णाङ्क अङ्कः_glyphs,
		   unsigned पूर्णाङ्क *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (एतत्);

    विकल्प (u.header.indexFormat)
    {
    पक्ष 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    पक्ष 3: {
      hb_barrier ();
      सचे (!u.format3.add_offset (c, end_offset, size))
	return_trace (मिच्छा);
      सचे (!(अङ्कः_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (सच्च);
    }
    // TODO: implement 2, 4, 5.
    पक्ष 2: पक्ष 4:  // No-op.
    पक्ष 5:  // Pad to 32-bit aligned.
    स्वभाव: return_trace (मिच्छा);
    }
  }

  तार्किक
  add_offset (hb_serialize_context_t *c, unsigned पूर्णाङ्क local_offset,
	      unsigned पूर्णाङ्क *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (एतत्);
    विकल्प (u.header.indexFormat)
    {
    पक्ष 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    पक्ष 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    पक्ष 2:  // Should be a no-op.
    पक्ष 4: पक्ष 5:  // Handle sparse cases.
    स्वभाव: return_trace (मिच्छा);
    }
  }

  तार्किक get_extents (hb_glyph_extents_t *extents HB_UNUSED, तार्किक scale HB_UNUSED) ध्रुव
  {
    विकल्प (u.header.indexFormat)
    {
    पक्ष 2: पक्ष 5: /* TODO */
    पक्ष 1: पक्ष 3: पक्ष 4: /* Variable-metrics formats do not have metrics here. */
    स्वभाव:पटिनिवत्त (मिच्छा);
    }
  }

  तार्किक
  get_image_data (unsigned पूर्णाङ्क idx, unsigned पूर्णाङ्क *offset,
		  unsigned पूर्णाङ्क *length, unsigned पूर्णाङ्क *format) ध्रुव
  {
    *format = u.header.imageFormat;
    विकल्प (u.header.indexFormat)
    {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_image_data (idx, offset, length);
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_image_data (idx, offset, length);
    स्वभाव: पटिनिवत्त मिच्छा;
    }
  }

  ध्रुव IndexSubtableHeader* get_header () ध्रुव { पटिनिवत्त &u.header; }

  शून्य populate_header (unsigned index_format,
			unsigned image_format,
			unsigned पूर्णाङ्क image_data_offset,
			unsigned पूर्णाङ्क *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    विकल्प (u.header.indexFormat)
    {
    पक्ष 1: *size += IndexSubtableFormat1::min_size; भङ्ग;
    पक्ष 3: *size += IndexSubtableFormat3::min_size; भङ्ग;
    }
  }

  रक्षित:
  सङ्घ {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (8, header);
};

संरचना IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (ध्रुव IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    प्रतिज्ञा (offsetToSubtable.is_null ());
    पटिनिवत्त *एतत्;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  ध्रुव IndexSubtable* get_subtable (ध्रुव शून्य *base) ध्रुव
  {
    पटिनिवत्त &(base+offsetToSubtable);
  }

  तार्किक add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 ध्रुव hb_vector_t<hb_pair_t<hb_codepoint_t, ध्रुव IndexSubtableRecord*>> *lookup, /* IN */
			 ध्रुव शून्य *base,
			 unsigned पूर्णाङ्क *start /* INOUT */) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    सचे (unlikely (!c->serializer->extend_min (subtable))) return_trace (मिच्छा);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned पूर्णाङ्क image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<वर्ण> data;
    hb_vector_t<unsigned पूर्णाङ्क> lengths;
    unsigned पूर्णाङ्क अङ्कः_glyphs = 0;
    तार्किक early_exit = मिच्छा;
    अत्थाय (unsigned पूर्णाङ्क i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      ध्रुव IndexSubtableRecord *next_record = (*lookup)[i].second;
      ध्रुव IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      सचे (next_header != old_header)
      {
	*start = i;
	early_exit = सच्च;
	भङ्ग;
      }
      unsigned पूर्णाङ्क अङ्कः_missing = record->add_glyph_for_subset (new_gid);
      अत्थाय (unsigned पूर्णाङ्क j = 0; j < अङ्कः_missing; j++)
	सचे (unlikely (!c->serializer->check_success (lengths.प्रक्षिप_or_fail (0))))
	  return_trace (मिच्छा);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      सचे (old_gid < next_record->firstGlyphIndex)
	return_trace (मिच्छा);

      unsigned पूर्णाङ्क old_idx = (unsigned पूर्णाङ्क) old_gid - next_record->firstGlyphIndex;
      unsigned पूर्णाङ्क offset, length, format;
      सचे (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (मिच्छा);
      सचे (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (मिच्छा);
      सचे (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (मिच्छा);
      सचे (unlikely (!c->serializer->check_success (lengths.प्रक्षिप_or_fail (length))))
	return_trace (मिच्छा);
      अङ्कः_glyphs += 1 + अङ्कः_missing;
    }
    सचे (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned पूर्णाङ्क image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    तार्किक shared = मिच्छा;
    uint64_t key = 0;
    सचे (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_अविभाज्यः,
			    &image_data_offset);
    }
    सचे (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_अविभाज्यः->length;
      सचे (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_अविभाज्यः,
					    data.arrayZ, data.length))))
	return_trace (मिच्छा);
      सचे (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(शून्य) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned पूर्णाङ्क cumulative = 0;
    अत्थाय (unsigned पूर्णाङ्क length : lengths)
    {
      सचे (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (मिच्छा);
      cumulative += length;
    }
    सचे (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      अङ्कः_glyphs,
					      &bitmap_size_context->size)))
      return_trace (मिच्छा);
    return_trace (सच्च);
  }

  तार्किक add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       ध्रुव hb_vector_t<hb_pair_t<hb_codepoint_t, ध्रुव IndexSubtableRecord*>> *lookup, /* IN */
		       ध्रुव शून्य *base,
		       unsigned पूर्णाङ्क *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto snap = c->serializer->snapshot ();
    unsigned पूर्णाङ्क old_size = bitmap_size_context->size;
    unsigned पूर्णाङ्क old_cbdt_अविभाज्यः_length = bitmap_size_context->cbdt_अविभाज्यः->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    सचे (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (मिच्छा);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    सचे (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->निष्कासय_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_अविभाज्यः->shrink (old_cbdt_अविभाज्यः_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (मिच्छा);
    }

    bitmap_size_context->अङ्कः_tables += 1;
    return_trace (सच्च);
  }

  unsigned पूर्णाङ्क add_glyph_for_subset (hb_codepoint_t gid)
  {
    सचे (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      पटिनिवत्त 0;
    }
    // TODO maybe assert? this shouldn't occur.
    सचे (lastGlyphIndex > gid)
      पटिनिवत्त 0;
    unsigned पूर्णाङ्क अङ्कः_missing = (unsigned पूर्णाङ्क) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    पटिनिवत्त अङ्कः_missing;
  }

  तार्किक get_extents (hb_glyph_extents_t *extents, ध्रुव शून्य *base, तार्किक scale) ध्रुव
  { पटिनिवत्त (base+offsetToSubtable).get_extents (extents, scale); }

  तार्किक get_image_data (unsigned पूर्णाङ्क  gid,
		       ध्रुव शून्य   *base,
		       unsigned पूर्णाङ्क *offset,
		       unsigned पूर्णाङ्क *length,
		       unsigned पूर्णाङ्क *format) ध्रुव
  {
    सचे (gid < firstGlyphIndex || gid > lastGlyphIndex) पटिनिवत्त मिच्छा;
    पटिनिवत्त (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना IndexSubtableArray
{
  friend संरचना CBDT;

  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क गणनम्) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (indexSubtablesZ.sanitize (c, गणनम्, एतत्));
  }

  शून्य
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		ध्रुव IndexSubtableRecord*>> *lookup /* OUT */) ध्रुव
  {
    तार्किक start_glyph_is_set = मिच्छा;
    unsigned अङ्कः_glyphs = c->plan->अङ्कः_output_glyphs ();
    अत्थाय (hb_codepoint_t new_gid = 0; new_gid < अङ्कः_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      सचे (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) अनुवर्त;

      ध्रुव IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->अङ्कः_tables);
      सचे (unlikely (!record)) अनुवर्त;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned पूर्णाङ्क offset, length, format;
      सचे (unlikely (!record->get_image_data (old_gid, एतत्, &offset, &length, &format))) अनुवर्त;

      lookup->push (hb_pair_t<hb_codepoint_t, ध्रुव IndexSubtableRecord*> (new_gid, record));

      सचे (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = सच्च;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  तार्किक
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    hb_vector_t<hb_pair_t<hb_codepoint_t, ध्रुव IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    सचे (unlikely (!c->serializer->propagate_error (lookup)))
      पटिनिवत्त मिच्छा;

    bitmap_size_context->size = 0;
    bitmap_size_context->अङ्कः_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    अत्थाय (unsigned पूर्णाङ्क start = 0; start < lookup.length;)
    {
      सचे (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, एतत्, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	अत्थाय (unsigned पूर्णाङ्क i = 0; i < records.length; i++)
	  c->serializer->निष्कासय_discard ();
	return_trace (मिच्छा);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < records.length; i++)
      objidxs.push (c->serializer->निष्कासय_pack ());
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      सचे (unlikely (!record)) return_trace (मिच्छा);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (सच्च);
  }

  सार्वजनिक:
  ध्रुव IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned पूर्णाङ्क अङ्कः_Tables) ध्रुव
  {
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < अङ्कः_Tables; ++i)
    {
      unsigned पूर्णाङ्क firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned पूर्णाङ्क lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      सचे (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	पटिनिवत्त &indexSubtablesZ[i];
    }
    पटिनिवत्त nullptr;
  }

  रक्षित:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

संरचना BitmapSizeTable
{
  friend संरचना CBLC;
  friend संरचना CBDT;

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, सङ्ख्या_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ध्रुव IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, ध्रुव शून्य *base, ध्रुव शून्य **out_base) ध्रुव
  {
    *out_base = &(base+indexSubtableArrayOffset);
    पटिनिवत्त (base+indexSubtableArrayOffset).find_table (glyph, सङ्ख्या_Of_Index_Subtables);
  }

  तार्किक
  subset (hb_subset_context_t *c, ध्रुव शून्य *base,
	  ध्रुव वर्ण *cbdt, unsigned पूर्णाङ्क cbdt_length,
	  hb_vector_t<वर्ण> *cbdt_अविभाज्यः /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out_table = c->serializer->embed (एतत्);
    सचे (unlikely (!out_table)) return_trace (मिच्छा);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_अविभाज्यः = cbdt_अविभाज्यः;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.अङ्कः_tables = सङ्ख्या_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    सचे (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (मिच्छा);
    सचे (!bitmap_size_context.size ||
	!bitmap_size_context.अङ्कः_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (मिच्छा);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->सङ्ख्या_Of_Index_Subtables = bitmap_size_context.अङ्कः_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (सच्च);
  }

  रक्षित:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		सङ्ख्या_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_पङ्क्तिः_Metrics	horizontal;
  SBit_पङ्क्तिः_Metrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

संरचना GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (9, data);
};

संरचना GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (12, data);
};

संरचना GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, data);
};

संरचना CBLC
{
  friend संरचना CBDT;

  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, एतत्));
  }

  स्थिर तार्किक
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<वर्ण>* cbdt_अविभाज्यः)
  {
    hb_blob_t *cbdt_अविभाज्यः_blob = hb_blob_create (cbdt_अविभाज्यः->arrayZ,
						 cbdt_अविभाज्यः->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_अविभाज्यः->arrayZ,
						 hb_free);
    cbdt_अविभाज्यः->init ();  // Leak arrayZ to the blob.
    तार्किक ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_अविभाज्यः_blob);
    hb_blob_destroy (cbdt_अविभाज्यः_blob);
    पटिनिवत्त ret;
  }

  तार्किक
  subset_size_table (hb_subset_context_t *c, ध्रुव BitmapSizeTable& table,
		     ध्रुव वर्ण *cbdt /* IN */, unsigned पूर्णाङ्क cbdt_length,
		     CBLC *cblc_अविभाज्यः /* INOUT */, hb_vector_t<वर्ण> *cbdt_अविभाज्यः /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    cblc_अविभाज्यः->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_अविभाज्यः_len = cbdt_अविभाज्यः->length;

    सचे (!table.subset (c, एतत्, cbdt, cbdt_length, cbdt_अविभाज्यः, dedup))
    {
      cblc_अविभाज्यः->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_अविभाज्यः->shrink (cbdt_अविभाज्यः_len);
      return_trace (मिच्छा);
    }
    return_trace (सच्च);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL तार्किक subset (hb_subset_context_t *c) ध्रुव;

  रक्षित:
  ध्रुव BitmapSizeTable &choose_strike (hb_font_t *font) ध्रुव
  {
    unsigned गणनम् = sizeTables.len;
    सचे (unlikely (!गणनम्))
      पटिनिवत्त Null (BitmapSizeTable);

    unsigned पूर्णाङ्क requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    सचे (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned पूर्णाङ्क best_i = 0;
    unsigned पूर्णाङ्क best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    अत्थाय (unsigned पूर्णाङ्क i = 1; i < गणनम्; i++)
    {
      unsigned पूर्णाङ्क ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      सचे ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    पटिनिवत्त sizeTables[best_i];
  }

  रक्षित:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

संरचना CBDT
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      एतत्->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      एतत्->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      एतत्->cblc.destroy ();
      एतत्->cbdt.destroy ();
    }

    तार्किक
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, तार्किक scale = सच्च) ध्रुव
    {
      ध्रुव शून्य *base;
      ध्रुव BitmapSizeTable &strike = एतत्->cblc->choose_strike (font);
      ध्रुव IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      सचे (!subtable_record || !strike.ppemX || !strike.ppemY)
	पटिनिवत्त मिच्छा;

      सचे (subtable_record->get_extents (extents, base, scale))
	पटिनिवत्त सच्च;

      unsigned पूर्णाङ्क image_offset = 0, image_length = 0, image_format = 0;
      सचे (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	पटिनिवत्त मिच्छा;

      unsigned पूर्णाङ्क cbdt_len = cbdt.get_length ();
      सचे (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	पटिनिवत्त मिच्छा;

      विकल्प (image_format)
      {
      पक्ष 17: {
	सचे (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  पटिनिवत्त मिच्छा;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (एतत्->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	भङ्ग;
      }
      पक्ष 18: {
	सचे (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  पटिनिवत्त मिच्छा;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (एतत्->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	भङ्ग;
      }
      स्वभाव: पटिनिवत्त मिच्छा; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      सचे (scale)
      {
	भिन्न x_scale = upem / (भिन्न) strike.ppemX;
	भिन्न y_scale = upem / (भिन्न) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      पटिनिवत्त सच्च;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ध्रुव
    {
      ध्रुव शून्य *base;
      ध्रुव BitmapSizeTable &strike = एतत्->cblc->choose_strike (font);
      ध्रुव IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      सचे (!subtable_record || !strike.ppemX || !strike.ppemY)
	पटिनिवत्त hb_blob_get_empty ();

      unsigned पूर्णाङ्क image_offset = 0, image_length = 0, image_format = 0;
      सचे (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	पटिनिवत्त hb_blob_get_empty ();

      unsigned पूर्णाङ्क cbdt_len = cbdt.get_length ();
      सचे (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	पटिनिवत्त hb_blob_get_empty ();

      विकल्प (image_format)
      {
      पक्ष 17:
      {
	सचे (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  पटिनिवत्त hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (एतत्->cbdt, image_offset);
	पटिनिवत्त hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      पक्ष 18:
      {
	सचे (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  पटिनिवत्त hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (एतत्->cbdt, image_offset);
	पटिनिवत्त hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      पक्ष 19:
      {
	सचे (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  पटिनिवत्त hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (एतत्->cbdt, image_offset);
	पटिनिवत्त hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      स्वभाव: पटिनिवत्त hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    तार्किक has_data () ध्रुव { पटिनिवत्त cbdt->version.major; }

    तार्किक paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, शून्य *data) ध्रुव
    {
      सचे (!has_data ()) पटिनिवत्त मिच्छा;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      सचे (unlikely (!font->get_glyph_extents (glyph, &extents, मिच्छा)))
        पटिनिवत्त मिच्छा;

      सचे (unlikely (!get_extents (font, glyph, &pixel_extents, मिच्छा)))
        पटिनिवत्त मिच्छा;

      hb_blob_t *blob = reference_png (font, glyph);
      सचे (unlikely (hb_blob_is_immutable (blob)))
        पटिनिवत्त मिच्छा;

      तार्किक ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      पटिनिवत्त ret;
    }

    गुप्त:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned पूर्णाङ्क upem;
  };

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  रक्षित:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

अन्तःस्थ तार्किक
CBLC::subset (hb_subset_context_t *c) ध्रुव
{
  TRACE_SUBSET (एतत्);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<वर्ण> cbdt_अविभाज्यः;

  auto *cblc_अविभाज्यः = c->serializer->start_embed<CBLC> ();
  सचे (unlikely (!c->serializer->extend_min (cblc_अविभाज्यः))) return_trace (मिच्छा);
  cblc_अविभाज्यः->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned पूर्णाङ्क cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  सचे (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (मिच्छा);
  }
  _copy_data_to_cbdt (&cbdt_अविभाज्यः, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  अत्थाय (ध्रुव BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (ध्रुव वर्ण *) cbdt, cbdt_length, cblc_अविभाज्यः, &cbdt_अविभाज्यः, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_अविभाज्यः));
}

संरचना CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
