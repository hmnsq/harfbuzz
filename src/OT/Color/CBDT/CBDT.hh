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

#અન્તર્ભાવ "../../../hb-open-type.hh"
#અન્તર્ભાવ "../../../hb-paint.hh"

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


નામાકાશ OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
સંરચના cbdt_dedup_context_t
{
  સંરચના entry_t
  {
    unsigned પૂર્ણાંક base;		/* Offset of the data in cbdt_prime. */
    unsigned પૂર્ણાંક lstart;	/* Start index in lengths. */
    unsigned પૂર્ણાંક lcount;	/* Number of length entries. */
  };

  સ્થિર uint64_t hash (unsigned પૂર્ણાંક image_format,
			અચલ hb_vector_t<અક્ષર> &data,
			અચલ hb_vector_t<unsigned પૂર્ણાંક> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((અચલ અક્ષર *) lengths.arrayZ,
				      lengths.length * કદ (unsigned પૂર્ણાંક)));
    પાછા ((uint64_t) image_format << 32) | h;
  }

  તાર્કિક find (uint64_t key,
	     અચલ hb_vector_t<અક્ષર> &data,
	     અચલ hb_vector_t<unsigned પૂર્ણાંક> &lengths,
	     અચલ hb_vector_t<અક્ષર> &cbdt_અવિભાજ્ય,
	     unsigned પૂર્ણાંક *base /* OUT */) અચલ
  {
    unsigned પૂર્ણાંક *idx_p = nullptr;
    જો (!map.has (key, &idx_p)) પાછા ખોટું;
    અચલ entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    જો (e.lcount != lengths.length) પાછા ખોટું;
    જો (unlikely (e.base + data.length > (unsigned પૂર્ણાંક) cbdt_અવિભાજ્ય.length)) પાછા ખોટું;
    જો (hb_memcmp (cbdt_અવિભાજ્ય.arrayZ + e.base, data.arrayZ, data.length)) પાછા ખોટું;
    જો (hb_memcmp (આ->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * કદ (unsigned પૂર્ણાંક)))
      પાછા ખોટું;
    *base = e.base;
    પાછા સાચું;
  }

  તાર્કિક add (uint64_t key,
	    unsigned પૂર્ણાંક base,
	    અચલ hb_vector_t<unsigned પૂર્ણાંક> &lengths)
  {
    જો (unlikely (entries.in_error () || આ->lengths.in_error () || map.in_error ()))
      પાછા ખોટું;
    entry_t e = {base, આ->lengths.length, lengths.length};
    જો (unlikely (!entries.ધકેલો_or_fail (e))) પાછા ખોટું;
    માટે (unsigned પૂર્ણાંક length : lengths)
      જો (unlikely (!આ->lengths.ધકેલો_or_fail (length))) પાછા ખોટું;
    પાછા map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned પૂર્ણાંક> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned પૂર્ણાંક> lengths; /* Concatenated per-entry lengths. */
};

સંરચના cblc_bitmap_size_subset_context_t
{
  અચલ અક્ષર *cbdt;
  unsigned પૂર્ણાંક cbdt_length;
  hb_vector_t<અક્ષર> *cbdt_અવિભાજ્ય;
  unsigned પૂર્ણાંક size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned પૂર્ણાંક અંક_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

સ્થિર અંતઃરેખ તાર્કિક
_copy_data_to_cbdt (hb_vector_t<અક્ષર> *cbdt_અવિભાજ્ય,
		    અચલ શૂન્ય        *data,
		    unsigned           length)
{
  unsigned પૂર્ણાંક new_len = cbdt_અવિભાજ્ય->length + length;
  જો (unlikely (!cbdt_અવિભાજ્ય->alloc (new_len))) પાછા ખોટું;
  hb_memcpy (cbdt_અવિભાજ્ય->arrayZ + cbdt_અવિભાજ્ય->length, data, length);
  cbdt_અવિભાજ્ય->length = new_len;
  પાછા સાચું;
}

સંરચના SmallGlyphMetrics
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય get_extents (hb_font_t *font, hb_glyph_extents_t *extents, તાર્કિક scale) અચલ
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<પૂર્ણાંક> (height);

    જો (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  જાહેર:
  DEFINE_SIZE_STATIC (5);
};

સંરચના BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના SBit_લીટી_Metrics
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
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
  જાહેર:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

સંરચના IndexSubtableHeader
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
સંરચના IndexSubtableFormat1Or3
{
  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક glyph_ગણતરી) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  offsetArrayZ.sanitize (c, glyph_ગણતરી + 1));
  }

  તાર્કિક get_image_data (unsigned પૂર્ણાંક idx,
		       unsigned પૂર્ણાંક *offset,
		       unsigned પૂર્ણાંક *length) અચલ
  {
    જો (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      પાછા ખોટું;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    પાછા સાચું;
  }

  તાર્કિક add_offset (hb_serialize_context_t *c,
		   unsigned પૂર્ણાંક offset,
		   unsigned પૂર્ણાંક *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (આ);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += કદ (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((તાર્કિક) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  જાહેર:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

સંરચના IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
સંરચના IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

સંરચના IndexSubtable
{
  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક glyph_ગણતરી) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.header.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.header.indexFormat)
    {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_ગણતરી));
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_ગણતરી));
    મૂળભૂત:return_trace (સાચું);
    }
  }

  તાર્કિક
  finish_subtable (hb_serialize_context_t *c,
		   unsigned પૂર્ણાંક end_offset,
		   unsigned પૂર્ણાંક અંક_glyphs,
		   unsigned પૂર્ણાંક *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (આ);

    પસંદગી (u.header.indexFormat)
    {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    સ્થિતિ 3: {
      hb_barrier ();
      જો (!u.format3.add_offset (c, end_offset, size))
	return_trace (ખોટું);
      જો (!(અંક_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (સાચું);
    }
    // TODO: implement 2, 4, 5.
    સ્થિતિ 2: સ્થિતિ 4:  // No-op.
    સ્થિતિ 5:  // Pad to 32-bit aligned.
    મૂળભૂત: return_trace (ખોટું);
    }
  }

  તાર્કિક
  add_offset (hb_serialize_context_t *c, unsigned પૂર્ણાંક local_offset,
	      unsigned પૂર્ણાંક *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (આ);
    પસંદગી (u.header.indexFormat)
    {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    સ્થિતિ 2:  // Should be a no-op.
    સ્થિતિ 4: સ્થિતિ 5:  // Handle sparse cases.
    મૂળભૂત: return_trace (ખોટું);
    }
  }

  તાર્કિક get_extents (hb_glyph_extents_t *extents HB_UNUSED, તાર્કિક scale HB_UNUSED) અચલ
  {
    પસંદગી (u.header.indexFormat)
    {
    સ્થિતિ 2: સ્થિતિ 5: /* TODO */
    સ્થિતિ 1: સ્થિતિ 3: સ્થિતિ 4: /* Variable-metrics formats do not have metrics here. */
    મૂળભૂત:પાછા (ખોટું);
    }
  }

  તાર્કિક
  get_image_data (unsigned પૂર્ણાંક idx, unsigned પૂર્ણાંક *offset,
		  unsigned પૂર્ણાંક *length, unsigned પૂર્ણાંક *format) અચલ
  {
    *format = u.header.imageFormat;
    પસંદગી (u.header.indexFormat)
    {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_image_data (idx, offset, length);
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_image_data (idx, offset, length);
    મૂળભૂત: પાછા ખોટું;
    }
  }

  અચલ IndexSubtableHeader* get_header () અચલ { પાછા &u.header; }

  શૂન્ય populate_header (unsigned index_format,
			unsigned image_format,
			unsigned પૂર્ણાંક image_data_offset,
			unsigned પૂર્ણાંક *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    પસંદગી (u.header.indexFormat)
    {
    સ્થિતિ 1: *size += IndexSubtableFormat1::min_size; તોડો;
    સ્થિતિ 3: *size += IndexSubtableFormat3::min_size; તોડો;
    }
  }

  સુરક્ષિત:
  સંઘ {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (8, header);
};

સંરચના IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (અચલ IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    પ્રતિજ્ઞા (offsetToSubtable.is_null ());
    પાછા *આ;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  અચલ IndexSubtable* get_subtable (અચલ શૂન્ય *base) અચલ
  {
    પાછા &(base+offsetToSubtable);
  }

  તાર્કિક add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 અચલ hb_vector_t<hb_pair_t<hb_codepoint_t, અચલ IndexSubtableRecord*>> *lookup, /* IN */
			 અચલ શૂન્ય *base,
			 unsigned પૂર્ણાંક *start /* INOUT */) અચલ
  {
    TRACE_SERIALIZE (આ);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    જો (unlikely (!c->serializer->extend_min (subtable))) return_trace (ખોટું);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned પૂર્ણાંક image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<અક્ષર> data;
    hb_vector_t<unsigned પૂર્ણાંક> lengths;
    unsigned પૂર્ણાંક અંક_glyphs = 0;
    તાર્કિક early_exit = ખોટું;
    માટે (unsigned પૂર્ણાંક i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      અચલ IndexSubtableRecord *next_record = (*lookup)[i].second;
      અચલ IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      જો (next_header != old_header)
      {
	*start = i;
	early_exit = સાચું;
	તોડો;
      }
      unsigned પૂર્ણાંક અંક_missing = record->add_glyph_for_subset (new_gid);
      માટે (unsigned પૂર્ણાંક j = 0; j < અંક_missing; j++)
	જો (unlikely (!c->serializer->check_success (lengths.ધકેલો_or_fail (0))))
	  return_trace (ખોટું);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      જો (old_gid < next_record->firstGlyphIndex)
	return_trace (ખોટું);

      unsigned પૂર્ણાંક old_idx = (unsigned પૂર્ણાંક) old_gid - next_record->firstGlyphIndex;
      unsigned પૂર્ણાંક offset, length, format;
      જો (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (ખોટું);
      જો (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (ખોટું);
      જો (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (ખોટું);
      જો (unlikely (!c->serializer->check_success (lengths.ધકેલો_or_fail (length))))
	return_trace (ખોટું);
      અંક_glyphs += 1 + અંક_missing;
    }
    જો (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned પૂર્ણાંક image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    તાર્કિક shared = ખોટું;
    uint64_t key = 0;
    જો (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_અવિભાજ્ય,
			    &image_data_offset);
    }
    જો (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_અવિભાજ્ય->length;
      જો (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_અવિભાજ્ય,
					    data.arrayZ, data.length))))
	return_trace (ખોટું);
      જો (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(શૂન્ય) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned પૂર્ણાંક cumulative = 0;
    માટે (unsigned પૂર્ણાંક length : lengths)
    {
      જો (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (ખોટું);
      cumulative += length;
    }
    જો (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      અંક_glyphs,
					      &bitmap_size_context->size)))
      return_trace (ખોટું);
    return_trace (સાચું);
  }

  તાર્કિક add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       અચલ hb_vector_t<hb_pair_t<hb_codepoint_t, અચલ IndexSubtableRecord*>> *lookup, /* IN */
		       અચલ શૂન્ય *base,
		       unsigned પૂર્ણાંક *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto snap = c->serializer->snapshot ();
    unsigned પૂર્ણાંક old_size = bitmap_size_context->size;
    unsigned પૂર્ણાંક old_cbdt_અવિભાજ્ય_length = bitmap_size_context->cbdt_અવિભાજ્ય->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    જો (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (ખોટું);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    જો (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->કાઢો_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_અવિભાજ્ય->shrink (old_cbdt_અવિભાજ્ય_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (ખોટું);
    }

    bitmap_size_context->અંક_tables += 1;
    return_trace (સાચું);
  }

  unsigned પૂર્ણાંક add_glyph_for_subset (hb_codepoint_t gid)
  {
    જો (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      પાછા 0;
    }
    // TODO maybe assert? this shouldn't occur.
    જો (lastGlyphIndex > gid)
      પાછા 0;
    unsigned પૂર્ણાંક અંક_missing = (unsigned પૂર્ણાંક) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    પાછા અંક_missing;
  }

  તાર્કિક get_extents (hb_glyph_extents_t *extents, અચલ શૂન્ય *base, તાર્કિક scale) અચલ
  { પાછા (base+offsetToSubtable).get_extents (extents, scale); }

  તાર્કિક get_image_data (unsigned પૂર્ણાંક  gid,
		       અચલ શૂન્ય   *base,
		       unsigned પૂર્ણાંક *offset,
		       unsigned પૂર્ણાંક *length,
		       unsigned પૂર્ણાંક *format) અચલ
  {
    જો (gid < firstGlyphIndex || gid > lastGlyphIndex) પાછા ખોટું;
    પાછા (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના IndexSubtableArray
{
  friend સંરચના CBDT;

  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક ગણતરી) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (indexSubtablesZ.sanitize (c, ગણતરી, આ));
  }

  શૂન્ય
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		અચલ IndexSubtableRecord*>> *lookup /* OUT */) અચલ
  {
    તાર્કિક start_glyph_is_set = ખોટું;
    unsigned અંક_glyphs = c->plan->અંક_output_glyphs ();
    માટે (hb_codepoint_t new_gid = 0; new_gid < અંક_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      જો (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) ચાલુ;

      અચલ IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->અંક_tables);
      જો (unlikely (!record)) ચાલુ;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned પૂર્ણાંક offset, length, format;
      જો (unlikely (!record->get_image_data (old_gid, આ, &offset, &length, &format))) ચાલુ;

      lookup->push (hb_pair_t<hb_codepoint_t, અચલ IndexSubtableRecord*> (new_gid, record));

      જો (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = સાચું;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  તાર્કિક
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) અચલ
  {
    TRACE_SUBSET (આ);

    hb_vector_t<hb_pair_t<hb_codepoint_t, અચલ IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    જો (unlikely (!c->serializer->propagate_error (lookup)))
      પાછા ખોટું;

    bitmap_size_context->size = 0;
    bitmap_size_context->અંક_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    માટે (unsigned પૂર્ણાંક start = 0; start < lookup.length;)
    {
      જો (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, આ, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	માટે (unsigned પૂર્ણાંક i = 0; i < records.length; i++)
	  c->serializer->કાઢો_discard ();
	return_trace (ખોટું);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    માટે (unsigned પૂર્ણાંક i = 0; i < records.length; i++)
      objidxs.push (c->serializer->કાઢો_pack ());
    માટે (unsigned પૂર્ણાંક i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      જો (unlikely (!record)) return_trace (ખોટું);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (સાચું);
  }

  જાહેર:
  અચલ IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned પૂર્ણાંક અંક_Tables) અચલ
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_Tables; ++i)
    {
      unsigned પૂર્ણાંક firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned પૂર્ણાંક lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      જો (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	પાછા &indexSubtablesZ[i];
    }
    પાછા nullptr;
  }

  સુરક્ષિત:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

સંરચના BitmapSizeTable
{
  friend સંરચના CBLC;
  friend સંરચના CBDT;

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, સંખ્યા_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  અચલ IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, અચલ શૂન્ય *base, અચલ શૂન્ય **out_base) અચલ
  {
    *out_base = &(base+indexSubtableArrayOffset);
    પાછા (base+indexSubtableArrayOffset).find_table (glyph, સંખ્યા_Of_Index_Subtables);
  }

  તાર્કિક
  subset (hb_subset_context_t *c, અચલ શૂન્ય *base,
	  અચલ અક્ષર *cbdt, unsigned પૂર્ણાંક cbdt_length,
	  hb_vector_t<અક્ષર> *cbdt_અવિભાજ્ય /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out_table = c->serializer->embed (આ);
    જો (unlikely (!out_table)) return_trace (ખોટું);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_અવિભાજ્ય = cbdt_અવિભાજ્ય;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.અંક_tables = સંખ્યા_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    જો (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (ખોટું);
    જો (!bitmap_size_context.size ||
	!bitmap_size_context.અંક_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (ખોટું);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->સંખ્યા_Of_Index_Subtables = bitmap_size_context.અંક_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (સાચું);
  }

  સુરક્ષિત:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		સંખ્યા_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_લીટી_Metrics	horizontal;
  SBit_લીટી_Metrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  જાહેર:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

સંરચના GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  જાહેર:
  DEFINE_SIZE_ARRAY (9, data);
};

સંરચના GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  જાહેર:
  DEFINE_SIZE_ARRAY (12, data);
};

સંરચના GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  જાહેર:
  DEFINE_SIZE_ARRAY (4, data);
};

સંરચના CBLC
{
  friend સંરચના CBDT;

  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, આ));
  }

  સ્થિર તાર્કિક
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<અક્ષર>* cbdt_અવિભાજ્ય)
  {
    hb_blob_t *cbdt_અવિભાજ્ય_blob = hb_blob_create (cbdt_અવિભાજ્ય->arrayZ,
						 cbdt_અવિભાજ્ય->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_અવિભાજ્ય->arrayZ,
						 hb_free);
    cbdt_અવિભાજ્ય->init ();  // Leak arrayZ to the blob.
    તાર્કિક ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_અવિભાજ્ય_blob);
    hb_blob_destroy (cbdt_અવિભાજ્ય_blob);
    પાછા ret;
  }

  તાર્કિક
  subset_size_table (hb_subset_context_t *c, અચલ BitmapSizeTable& table,
		     અચલ અક્ષર *cbdt /* IN */, unsigned પૂર્ણાંક cbdt_length,
		     CBLC *cblc_અવિભાજ્ય /* INOUT */, hb_vector_t<અક્ષર> *cbdt_અવિભાજ્ય /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) અચલ
  {
    TRACE_SUBSET (આ);
    cblc_અવિભાજ્ય->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_અવિભાજ્ય_len = cbdt_અવિભાજ્ય->length;

    જો (!table.subset (c, આ, cbdt, cbdt_length, cbdt_અવિભાજ્ય, dedup))
    {
      cblc_અવિભાજ્ય->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_અવિભાજ્ય->shrink (cbdt_અવિભાજ્ય_len);
      return_trace (ખોટું);
    }
    return_trace (સાચું);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL તાર્કિક subset (hb_subset_context_t *c) અચલ;

  સુરક્ષિત:
  અચલ BitmapSizeTable &choose_strike (hb_font_t *font) અચલ
  {
    unsigned ગણતરી = sizeTables.len;
    જો (unlikely (!ગણતરી))
      પાછા Null (BitmapSizeTable);

    unsigned પૂર્ણાંક requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    જો (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned પૂર્ણાંક best_i = 0;
    unsigned પૂર્ણાંક best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    માટે (unsigned પૂર્ણાંક i = 1; i < ગણતરી; i++)
    {
      unsigned પૂર્ણાંક ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      જો ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    પાછા sizeTables[best_i];
  }

  સુરક્ષિત:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  જાહેર:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

સંરચના CBDT
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  સંરચના accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      આ->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      આ->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      આ->cblc.destroy ();
      આ->cbdt.destroy ();
    }

    તાર્કિક
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, તાર્કિક scale = સાચું) અચલ
    {
      અચલ શૂન્ય *base;
      અચલ BitmapSizeTable &strike = આ->cblc->choose_strike (font);
      અચલ IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      જો (!subtable_record || !strike.ppemX || !strike.ppemY)
	પાછા ખોટું;

      જો (subtable_record->get_extents (extents, base, scale))
	પાછા સાચું;

      unsigned પૂર્ણાંક image_offset = 0, image_length = 0, image_format = 0;
      જો (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	પાછા ખોટું;

      unsigned પૂર્ણાંક cbdt_len = cbdt.get_length ();
      જો (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	પાછા ખોટું;

      પસંદગી (image_format)
      {
      સ્થિતિ 17: {
	જો (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  પાછા ખોટું;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (આ->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	તોડો;
      }
      સ્થિતિ 18: {
	જો (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  પાછા ખોટું;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (આ->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	તોડો;
      }
      મૂળભૂત: પાછા ખોટું; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      જો (scale)
      {
	દશાંશ x_scale = upem / (દશાંશ) strike.ppemX;
	દશાંશ y_scale = upem / (દશાંશ) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      પાછા સાચું;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) અચલ
    {
      અચલ શૂન્ય *base;
      અચલ BitmapSizeTable &strike = આ->cblc->choose_strike (font);
      અચલ IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      જો (!subtable_record || !strike.ppemX || !strike.ppemY)
	પાછા hb_blob_get_empty ();

      unsigned પૂર્ણાંક image_offset = 0, image_length = 0, image_format = 0;
      જો (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	પાછા hb_blob_get_empty ();

      unsigned પૂર્ણાંક cbdt_len = cbdt.get_length ();
      જો (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	પાછા hb_blob_get_empty ();

      પસંદગી (image_format)
      {
      સ્થિતિ 17:
      {
	જો (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  પાછા hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (આ->cbdt, image_offset);
	પાછા hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      સ્થિતિ 18:
      {
	જો (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  પાછા hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (આ->cbdt, image_offset);
	પાછા hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      સ્થિતિ 19:
      {
	જો (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  પાછા hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (આ->cbdt, image_offset);
	પાછા hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      મૂળભૂત: પાછા hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    તાર્કિક has_data () અચલ { પાછા cbdt->version.major; }

    તાર્કિક paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, શૂન્ય *data) અચલ
    {
      જો (!has_data ()) પાછા ખોટું;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      જો (unlikely (!font->get_glyph_extents (glyph, &extents, ખોટું)))
        પાછા ખોટું;

      જો (unlikely (!get_extents (font, glyph, &pixel_extents, ખોટું)))
        પાછા ખોટું;

      hb_blob_t *blob = reference_png (font, glyph);
      જો (unlikely (hb_blob_is_immutable (blob)))
        પાછા ખોટું;

      તાર્કિક ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      પાછા ret;
    }

    ખાનગી:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned પૂર્ણાંક upem;
  };

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  સુરક્ષિત:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  જાહેર:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

અંતઃરેખ તાર્કિક
CBLC::subset (hb_subset_context_t *c) અચલ
{
  TRACE_SUBSET (આ);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<અક્ષર> cbdt_અવિભાજ્ય;

  auto *cblc_અવિભાજ્ય = c->serializer->start_embed<CBLC> ();
  જો (unlikely (!c->serializer->extend_min (cblc_અવિભાજ્ય))) return_trace (ખોટું);
  cblc_અવિભાજ્ય->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned પૂર્ણાંક cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  જો (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (ખોટું);
  }
  _copy_data_to_cbdt (&cbdt_અવિભાજ્ય, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  માટે (અચલ BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (અચલ અક્ષર *) cbdt, cbdt_length, cblc_અવિભાજ્ય, &cbdt_અવિભાજ્ય, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_અવિભાજ્ય));
}

સંરચના CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
