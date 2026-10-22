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

#அந்தர்பாவ "../../../hb-open-type.hh"
#அந்தர்பாவ "../../../hb-paint.hh"

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


நாமாகாஶ OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
கட்டமைப்பு cbdt_dedup_context_t
{
  கட்டமைப்பு entry_t
  {
    unsigned முழு base;		/* Offset of the data in cbdt_prime. */
    unsigned முழு lstart;	/* Start index in lengths. */
    unsigned முழு lcount;	/* Number of length entries. */
  };

  நிலையான uint64_t hash (unsigned முழு image_format,
			மாறிலி hb_vector_t<எழுத்து> &data,
			மாறிலி hb_vector_t<unsigned முழு> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((மாறிலி எழுத்து *) lengths.arrayZ,
				      lengths.length * அளவு (unsigned முழு)));
    திரும்பு ((uint64_t) image_format << 32) | h;
  }

  தருக்க find (uint64_t key,
	     மாறிலி hb_vector_t<எழுத்து> &data,
	     மாறிலி hb_vector_t<unsigned முழு> &lengths,
	     மாறிலி hb_vector_t<எழுத்து> &cbdt_பகாஎண்,
	     unsigned முழு *base /* OUT */) மாறிலி
  {
    unsigned முழு *idx_p = nullptr;
    எனில் (!map.has (key, &idx_p)) திரும்பு பொய்;
    மாறிலி entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    எனில் (e.lcount != lengths.length) திரும்பு பொய்;
    எனில் (unlikely (e.base + data.length > (unsigned முழு) cbdt_பகாஎண்.length)) திரும்பு பொய்;
    எனில் (hb_memcmp (cbdt_பகாஎண்.arrayZ + e.base, data.arrayZ, data.length)) திரும்பு பொய்;
    எனில் (hb_memcmp (இது->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * அளவு (unsigned முழு)))
      திரும்பு பொய்;
    *base = e.base;
    திரும்பு உண்மை;
  }

  தருக்க add (uint64_t key,
	    unsigned முழு base,
	    மாறிலி hb_vector_t<unsigned முழு> &lengths)
  {
    எனில் (unlikely (entries.in_error () || இது->lengths.in_error () || map.in_error ()))
      திரும்பு பொய்;
    entry_t e = {base, இது->lengths.length, lengths.length};
    எனில் (unlikely (!entries.தள்ளு_or_fail (e))) திரும்பு பொய்;
    க்கு (unsigned முழு length : lengths)
      எனில் (unlikely (!இது->lengths.தள்ளு_or_fail (length))) திரும்பு பொய்;
    திரும்பு map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned முழு> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned முழு> lengths; /* Concatenated per-entry lengths. */
};

கட்டமைப்பு cblc_bitmap_size_subset_context_t
{
  மாறிலி எழுத்து *cbdt;
  unsigned முழு cbdt_length;
  hb_vector_t<எழுத்து> *cbdt_பகாஎண்;
  unsigned முழு size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned முழு இலக்கம்_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

நிலையான உள்வரி தருக்க
_copy_data_to_cbdt (hb_vector_t<எழுத்து> *cbdt_பகாஎண்,
		    மாறிலி வெறுமை        *data,
		    unsigned           length)
{
  unsigned முழு new_len = cbdt_பகாஎண்->length + length;
  எனில் (unlikely (!cbdt_பகாஎண்->alloc (new_len))) திரும்பு பொய்;
  hb_memcpy (cbdt_பகாஎண்->arrayZ + cbdt_பகாஎண்->length, data, length);
  cbdt_பகாஎண்->length = new_len;
  திரும்பு உண்மை;
}

கட்டமைப்பு SmallGlyphMetrics
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  வெறுமை get_extents (hb_font_t *font, hb_glyph_extents_t *extents, தருக்க scale) மாறிலி
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<முழு> (height);

    எனில் (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  பொது:
  DEFINE_SIZE_STATIC (5);
};

கட்டமைப்பு BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு SBit_வரி_Metrics
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
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
  பொது:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

கட்டமைப்பு IndexSubtableHeader
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
கட்டமைப்பு IndexSubtableFormat1Or3
{
  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு glyph_மொத்தம்) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  offsetArrayZ.sanitize (c, glyph_மொத்தம் + 1));
  }

  தருக்க get_image_data (unsigned முழு idx,
		       unsigned முழு *offset,
		       unsigned முழு *length) மாறிலி
  {
    எனில் (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      திரும்பு பொய்;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    திரும்பு உண்மை;
  }

  தருக்க add_offset (hb_serialize_context_t *c,
		   unsigned முழு offset,
		   unsigned முழு *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (இது);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += அளவு (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((தருக்க) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  பொது:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

கட்டமைப்பு IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
கட்டமைப்பு IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

கட்டமைப்பு IndexSubtable
{
  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு glyph_மொத்தம்) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.header.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.header.indexFormat)
    {
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_மொத்தம்));
    நிலை 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_மொத்தம்));
    இயல்பு:return_trace (உண்மை);
    }
  }

  தருக்க
  finish_subtable (hb_serialize_context_t *c,
		   unsigned முழு end_offset,
		   unsigned முழு இலக்கம்_glyphs,
		   unsigned முழு *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (இது);

    தேர்வு (u.header.indexFormat)
    {
    நிலை 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    நிலை 3: {
      hb_barrier ();
      எனில் (!u.format3.add_offset (c, end_offset, size))
	return_trace (பொய்);
      எனில் (!(இலக்கம்_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (உண்மை);
    }
    // TODO: implement 2, 4, 5.
    நிலை 2: நிலை 4:  // No-op.
    நிலை 5:  // Pad to 32-bit aligned.
    இயல்பு: return_trace (பொய்);
    }
  }

  தருக்க
  add_offset (hb_serialize_context_t *c, unsigned முழு local_offset,
	      unsigned முழு *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (இது);
    தேர்வு (u.header.indexFormat)
    {
    நிலை 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    நிலை 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    நிலை 2:  // Should be a no-op.
    நிலை 4: நிலை 5:  // Handle sparse cases.
    இயல்பு: return_trace (பொய்);
    }
  }

  தருக்க get_extents (hb_glyph_extents_t *extents HB_UNUSED, தருக்க scale HB_UNUSED) மாறிலி
  {
    தேர்வு (u.header.indexFormat)
    {
    நிலை 2: நிலை 5: /* TODO */
    நிலை 1: நிலை 3: நிலை 4: /* Variable-metrics formats do not have metrics here. */
    இயல்பு:திரும்பு (பொய்);
    }
  }

  தருக்க
  get_image_data (unsigned முழு idx, unsigned முழு *offset,
		  unsigned முழு *length, unsigned முழு *format) மாறிலி
  {
    *format = u.header.imageFormat;
    தேர்வு (u.header.indexFormat)
    {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_image_data (idx, offset, length);
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_image_data (idx, offset, length);
    இயல்பு: திரும்பு பொய்;
    }
  }

  மாறிலி IndexSubtableHeader* get_header () மாறிலி { திரும்பு &u.header; }

  வெறுமை populate_header (unsigned index_format,
			unsigned image_format,
			unsigned முழு image_data_offset,
			unsigned முழு *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    தேர்வு (u.header.indexFormat)
    {
    நிலை 1: *size += IndexSubtableFormat1::min_size; நிறுத்து;
    நிலை 3: *size += IndexSubtableFormat3::min_size; நிறுத்து;
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  பொது:
  DEFINE_SIZE_UNION (8, header);
};

கட்டமைப்பு IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (மாறிலி IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    ப்ரதிஜ்ஞா (offsetToSubtable.is_null ());
    திரும்பு *இது;
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  மாறிலி IndexSubtable* get_subtable (மாறிலி வெறுமை *base) மாறிலி
  {
    திரும்பு &(base+offsetToSubtable);
  }

  தருக்க add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 மாறிலி hb_vector_t<hb_pair_t<hb_codepoint_t, மாறிலி IndexSubtableRecord*>> *lookup, /* IN */
			 மாறிலி வெறுமை *base,
			 unsigned முழு *start /* INOUT */) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    எனில் (unlikely (!c->serializer->extend_min (subtable))) return_trace (பொய்);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned முழு image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<எழுத்து> data;
    hb_vector_t<unsigned முழு> lengths;
    unsigned முழு இலக்கம்_glyphs = 0;
    தருக்க early_exit = பொய்;
    க்கு (unsigned முழு i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      மாறிலி IndexSubtableRecord *next_record = (*lookup)[i].second;
      மாறிலி IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      எனில் (next_header != old_header)
      {
	*start = i;
	early_exit = உண்மை;
	நிறுத்து;
      }
      unsigned முழு இலக்கம்_missing = record->add_glyph_for_subset (new_gid);
      க்கு (unsigned முழு j = 0; j < இலக்கம்_missing; j++)
	எனில் (unlikely (!c->serializer->check_success (lengths.தள்ளு_or_fail (0))))
	  return_trace (பொய்);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      எனில் (old_gid < next_record->firstGlyphIndex)
	return_trace (பொய்);

      unsigned முழு old_idx = (unsigned முழு) old_gid - next_record->firstGlyphIndex;
      unsigned முழு offset, length, format;
      எனில் (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (பொய்);
      எனில் (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (பொய்);
      எனில் (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (பொய்);
      எனில் (unlikely (!c->serializer->check_success (lengths.தள்ளு_or_fail (length))))
	return_trace (பொய்);
      இலக்கம்_glyphs += 1 + இலக்கம்_missing;
    }
    எனில் (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned முழு image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    தருக்க shared = பொய்;
    uint64_t key = 0;
    எனில் (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_பகாஎண்,
			    &image_data_offset);
    }
    எனில் (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_பகாஎண்->length;
      எனில் (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_பகாஎண்,
					    data.arrayZ, data.length))))
	return_trace (பொய்);
      எனில் (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(வெறுமை) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned முழு cumulative = 0;
    க்கு (unsigned முழு length : lengths)
    {
      எனில் (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (பொய்);
      cumulative += length;
    }
    எனில் (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      இலக்கம்_glyphs,
					      &bitmap_size_context->size)))
      return_trace (பொய்);
    return_trace (உண்மை);
  }

  தருக்க add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       மாறிலி hb_vector_t<hb_pair_t<hb_codepoint_t, மாறிலி IndexSubtableRecord*>> *lookup, /* IN */
		       மாறிலி வெறுமை *base,
		       unsigned முழு *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto snap = c->serializer->snapshot ();
    unsigned முழு old_size = bitmap_size_context->size;
    unsigned முழு old_cbdt_பகாஎண்_length = bitmap_size_context->cbdt_பகாஎண்->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    எனில் (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (பொய்);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    எனில் (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->எடு_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_பகாஎண்->shrink (old_cbdt_பகாஎண்_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (பொய்);
    }

    bitmap_size_context->இலக்கம்_tables += 1;
    return_trace (உண்மை);
  }

  unsigned முழு add_glyph_for_subset (hb_codepoint_t gid)
  {
    எனில் (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      திரும்பு 0;
    }
    // TODO maybe assert? this shouldn't occur.
    எனில் (lastGlyphIndex > gid)
      திரும்பு 0;
    unsigned முழு இலக்கம்_missing = (unsigned முழு) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    திரும்பு இலக்கம்_missing;
  }

  தருக்க get_extents (hb_glyph_extents_t *extents, மாறிலி வெறுமை *base, தருக்க scale) மாறிலி
  { திரும்பு (base+offsetToSubtable).get_extents (extents, scale); }

  தருக்க get_image_data (unsigned முழு  gid,
		       மாறிலி வெறுமை   *base,
		       unsigned முழு *offset,
		       unsigned முழு *length,
		       unsigned முழு *format) மாறிலி
  {
    எனில் (gid < firstGlyphIndex || gid > lastGlyphIndex) திரும்பு பொய்;
    திரும்பு (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு IndexSubtableArray
{
  friend கட்டமைப்பு CBDT;

  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு மொத்தம்) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (indexSubtablesZ.sanitize (c, மொத்தம், இது));
  }

  வெறுமை
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		மாறிலி IndexSubtableRecord*>> *lookup /* OUT */) மாறிலி
  {
    தருக்க start_glyph_is_set = பொய்;
    unsigned இலக்கம்_glyphs = c->plan->இலக்கம்_output_glyphs ();
    க்கு (hb_codepoint_t new_gid = 0; new_gid < இலக்கம்_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      எனில் (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) தொடர்;

      மாறிலி IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->இலக்கம்_tables);
      எனில் (unlikely (!record)) தொடர்;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned முழு offset, length, format;
      எனில் (unlikely (!record->get_image_data (old_gid, இது, &offset, &length, &format))) தொடர்;

      lookup->push (hb_pair_t<hb_codepoint_t, மாறிலி IndexSubtableRecord*> (new_gid, record));

      எனில் (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = உண்மை;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  தருக்க
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) மாறிலி
  {
    TRACE_SUBSET (இது);

    hb_vector_t<hb_pair_t<hb_codepoint_t, மாறிலி IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    எனில் (unlikely (!c->serializer->propagate_error (lookup)))
      திரும்பு பொய்;

    bitmap_size_context->size = 0;
    bitmap_size_context->இலக்கம்_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    க்கு (unsigned முழு start = 0; start < lookup.length;)
    {
      எனில் (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, இது, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	க்கு (unsigned முழு i = 0; i < records.length; i++)
	  c->serializer->எடு_discard ();
	return_trace (பொய்);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    க்கு (unsigned முழு i = 0; i < records.length; i++)
      objidxs.push (c->serializer->எடு_pack ());
    க்கு (unsigned முழு i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      எனில் (unlikely (!record)) return_trace (பொய்);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (உண்மை);
  }

  பொது:
  மாறிலி IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned முழு இலக்கம்_Tables) மாறிலி
  {
    க்கு (unsigned முழு i = 0; i < இலக்கம்_Tables; ++i)
    {
      unsigned முழு firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned முழு lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      எனில் (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	திரும்பு &indexSubtablesZ[i];
    }
    திரும்பு nullptr;
  }

  பாதுகாக்கப்பட்ட:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

கட்டமைப்பு BitmapSizeTable
{
  friend கட்டமைப்பு CBLC;
  friend கட்டமைப்பு CBDT;

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, எண்_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  மாறிலி IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, மாறிலி வெறுமை *base, மாறிலி வெறுமை **out_base) மாறிலி
  {
    *out_base = &(base+indexSubtableArrayOffset);
    திரும்பு (base+indexSubtableArrayOffset).find_table (glyph, எண்_Of_Index_Subtables);
  }

  தருக்க
  subset (hb_subset_context_t *c, மாறிலி வெறுமை *base,
	  மாறிலி எழுத்து *cbdt, unsigned முழு cbdt_length,
	  hb_vector_t<எழுத்து> *cbdt_பகாஎண் /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out_table = c->serializer->embed (இது);
    எனில் (unlikely (!out_table)) return_trace (பொய்);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_பகாஎண் = cbdt_பகாஎண்;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.இலக்கம்_tables = எண்_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    எனில் (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (பொய்);
    எனில் (!bitmap_size_context.size ||
	!bitmap_size_context.இலக்கம்_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (பொய்);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->எண்_Of_Index_Subtables = bitmap_size_context.இலக்கம்_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (உண்மை);
  }

  பாதுகாக்கப்பட்ட:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		எண்_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_வரி_Metrics	horizontal;
  SBit_வரி_Metrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  பொது:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

கட்டமைப்பு GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  பொது:
  DEFINE_SIZE_ARRAY (9, data);
};

கட்டமைப்பு GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  பொது:
  DEFINE_SIZE_ARRAY (12, data);
};

கட்டமைப்பு GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  பொது:
  DEFINE_SIZE_ARRAY (4, data);
};

கட்டமைப்பு CBLC
{
  friend கட்டமைப்பு CBDT;

  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, இது));
  }

  நிலையான தருக்க
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<எழுத்து>* cbdt_பகாஎண்)
  {
    hb_blob_t *cbdt_பகாஎண்_blob = hb_blob_create (cbdt_பகாஎண்->arrayZ,
						 cbdt_பகாஎண்->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_பகாஎண்->arrayZ,
						 hb_free);
    cbdt_பகாஎண்->init ();  // Leak arrayZ to the blob.
    தருக்க ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_பகாஎண்_blob);
    hb_blob_destroy (cbdt_பகாஎண்_blob);
    திரும்பு ret;
  }

  தருக்க
  subset_size_table (hb_subset_context_t *c, மாறிலி BitmapSizeTable& table,
		     மாறிலி எழுத்து *cbdt /* IN */, unsigned முழு cbdt_length,
		     CBLC *cblc_பகாஎண் /* INOUT */, hb_vector_t<எழுத்து> *cbdt_பகாஎண் /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) மாறிலி
  {
    TRACE_SUBSET (இது);
    cblc_பகாஎண்->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_பகாஎண்_len = cbdt_பகாஎண்->length;

    எனில் (!table.subset (c, இது, cbdt, cbdt_length, cbdt_பகாஎண், dedup))
    {
      cblc_பகாஎண்->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_பகாஎண்->shrink (cbdt_பகாஎண்_len);
      return_trace (பொய்);
    }
    return_trace (உண்மை);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL தருக்க subset (hb_subset_context_t *c) மாறிலி;

  பாதுகாக்கப்பட்ட:
  மாறிலி BitmapSizeTable &choose_strike (hb_font_t *font) மாறிலி
  {
    unsigned மொத்தம் = sizeTables.len;
    எனில் (unlikely (!மொத்தம்))
      திரும்பு Null (BitmapSizeTable);

    unsigned முழு requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    எனில் (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned முழு best_i = 0;
    unsigned முழு best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    க்கு (unsigned முழு i = 1; i < மொத்தம்; i++)
    {
      unsigned முழு ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      எனில் ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    திரும்பு sizeTables[best_i];
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  பொது:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

கட்டமைப்பு CBDT
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      இது->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      இது->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      இது->cblc.destroy ();
      இது->cbdt.destroy ();
    }

    தருக்க
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, தருக்க scale = உண்மை) மாறிலி
    {
      மாறிலி வெறுமை *base;
      மாறிலி BitmapSizeTable &strike = இது->cblc->choose_strike (font);
      மாறிலி IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      எனில் (!subtable_record || !strike.ppemX || !strike.ppemY)
	திரும்பு பொய்;

      எனில் (subtable_record->get_extents (extents, base, scale))
	திரும்பு உண்மை;

      unsigned முழு image_offset = 0, image_length = 0, image_format = 0;
      எனில் (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	திரும்பு பொய்;

      unsigned முழு cbdt_len = cbdt.get_length ();
      எனில் (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	திரும்பு பொய்;

      தேர்வு (image_format)
      {
      நிலை 17: {
	எனில் (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  திரும்பு பொய்;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (இது->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	நிறுத்து;
      }
      நிலை 18: {
	எனில் (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  திரும்பு பொய்;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (இது->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	நிறுத்து;
      }
      இயல்பு: திரும்பு பொய்; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      எனில் (scale)
      {
	தசம x_scale = upem / (தசம) strike.ppemX;
	தசம y_scale = upem / (தசம) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      திரும்பு உண்மை;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) மாறிலி
    {
      மாறிலி வெறுமை *base;
      மாறிலி BitmapSizeTable &strike = இது->cblc->choose_strike (font);
      மாறிலி IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      எனில் (!subtable_record || !strike.ppemX || !strike.ppemY)
	திரும்பு hb_blob_get_empty ();

      unsigned முழு image_offset = 0, image_length = 0, image_format = 0;
      எனில் (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	திரும்பு hb_blob_get_empty ();

      unsigned முழு cbdt_len = cbdt.get_length ();
      எனில் (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	திரும்பு hb_blob_get_empty ();

      தேர்வு (image_format)
      {
      நிலை 17:
      {
	எனில் (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  திரும்பு hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (இது->cbdt, image_offset);
	திரும்பு hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      நிலை 18:
      {
	எனில் (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  திரும்பு hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (இது->cbdt, image_offset);
	திரும்பு hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      நிலை 19:
      {
	எனில் (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  திரும்பு hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (இது->cbdt, image_offset);
	திரும்பு hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      இயல்பு: திரும்பு hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    தருக்க has_data () மாறிலி { திரும்பு cbdt->version.major; }

    தருக்க paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, வெறுமை *data) மாறிலி
    {
      எனில் (!has_data ()) திரும்பு பொய்;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      எனில் (unlikely (!font->get_glyph_extents (glyph, &extents, பொய்)))
        திரும்பு பொய்;

      எனில் (unlikely (!get_extents (font, glyph, &pixel_extents, பொய்)))
        திரும்பு பொய்;

      hb_blob_t *blob = reference_png (font, glyph);
      எனில் (unlikely (hb_blob_is_immutable (blob)))
        திரும்பு பொய்;

      தருக்க ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      திரும்பு ret;
    }

    தனிப்பட்ட:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned முழு upem;
  };

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  பொது:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

உள்வரி தருக்க
CBLC::subset (hb_subset_context_t *c) மாறிலி
{
  TRACE_SUBSET (இது);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<எழுத்து> cbdt_பகாஎண்;

  auto *cblc_பகாஎண் = c->serializer->start_embed<CBLC> ();
  எனில் (unlikely (!c->serializer->extend_min (cblc_பகாஎண்))) return_trace (பொய்);
  cblc_பகாஎண்->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned முழு cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  எனில் (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (பொய்);
  }
  _copy_data_to_cbdt (&cbdt_பகாஎண், cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  க்கு (மாறிலி BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (மாறிலி எழுத்து *) cbdt, cbdt_length, cblc_பகாஎண், &cbdt_பகாஎண், &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_பகாஎண்));
}

கட்டமைப்பு CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
