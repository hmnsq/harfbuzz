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

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-paint.hh"

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


نطاق OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
بنية cbdt_dedup_context_t
{
  بنية entry_t
  {
    unsigned صحيح base;		/* Offset of the data in cbdt_prime. */
    unsigned صحيح lstart;	/* Start index in lengths. */
    unsigned صحيح lcount;	/* Number of length entries. */
  };

  ساكن uint64_t hash (unsigned صحيح image_format,
			ثابت hb_vector_t<حرف> &data,
			ثابت hb_vector_t<unsigned صحيح> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((ثابت حرف *) lengths.arrayZ,
				      lengths.length * حجم (unsigned صحيح)));
    החזר ((uint64_t) image_format << 32) | h;
  }

  منطقي find (uint64_t key,
	     ثابت hb_vector_t<حرف> &data,
	     ثابت hb_vector_t<unsigned صحيح> &lengths,
	     ثابت hb_vector_t<حرف> &cbdt_prime,
	     unsigned صحيح *base /* OUT */) ثابت
  {
    unsigned صحيح *idx_p = nullptr;
    אם (!map.has (key, &idx_p)) החזר שקר;
    ثابت entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    אם (e.lcount != lengths.length) החזר שקר;
    אם (unlikely (e.base + data.length > (unsigned صحيح) cbdt_prime.length)) החזר שקר;
    אם (hb_memcmp (cbdt_prime.arrayZ + e.base, data.arrayZ, data.length)) החזר שקר;
    אם (hb_memcmp (هذا->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * حجم (unsigned صحيح)))
      החזר שקר;
    *base = e.base;
    החזר אמת;
  }

  منطقي add (uint64_t key,
	    unsigned صحيح base,
	    ثابت hb_vector_t<unsigned صحيح> &lengths)
  {
    אם (unlikely (entries.in_error () || هذا->lengths.in_error () || map.in_error ()))
      החזר שקר;
    entry_t e = {base, هذا->lengths.length, lengths.length};
    אם (unlikely (!entries.push_or_fail (e))) החזר שקר;
    עבור (unsigned صحيح length : lengths)
      אם (unlikely (!هذا->lengths.push_or_fail (length))) החזר שקר;
    החזר map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned صحيح> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned صحيح> lengths; /* Concatenated per-entry lengths. */
};

بنية cblc_bitmap_size_subset_context_t
{
  ثابت حرف *cbdt;
  unsigned صحيح cbdt_length;
  hb_vector_t<حرف> *cbdt_prime;
  unsigned صحيح size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned صحيح num_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ساكن مضمن منطقي
_copy_data_to_cbdt (hb_vector_t<حرف> *cbdt_prime,
		    ثابت فراغ        *data,
		    unsigned           length)
{
  unsigned صحيح new_len = cbdt_prime->length + length;
  אם (unlikely (!cbdt_prime->alloc (new_len))) החזר שקר;
  hb_memcpy (cbdt_prime->arrayZ + cbdt_prime->length, data, length);
  cbdt_prime->length = new_len;
  החזר אמת;
}

بنية SmallGlyphMetrics
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقي scale) ثابت
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<صحيح> (height);

    אם (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  عام:
  DEFINE_SIZE_STATIC (5);
};

بنية BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية SBitLineMetrics
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
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
  عام:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

بنية IndexSubtableHeader
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  عام:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
بنية IndexSubtableFormat1Or3
{
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح glyph_count) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  offsetArrayZ.sanitize (c, glyph_count + 1));
  }

  منطقي get_image_data (unsigned صحيح idx,
		       unsigned صحيح *offset,
		       unsigned صحيح *length) ثابت
  {
    אם (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      החזר שקר;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    החזר אמת;
  }

  منطقي add_offset (hb_serialize_context_t *c,
		   unsigned صحيح offset,
		   unsigned صحيح *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += حجم (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((منطقي) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  عام:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

بنية IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
بنية IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

بنية IndexSubtable
{
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح glyph_count) ثابت
  {
    TRACE_SANITIZE (هذا);
    אם (!u.header.sanitize (c)) return_trace (שקר);
    hb_barrier ();
    بدل (u.header.indexFormat)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_count));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_count));
    افتراضي:return_trace (אמת);
    }
  }

  منطقي
  finish_subtable (hb_serialize_context_t *c,
		   unsigned صحيح end_offset,
		   unsigned صحيح num_glyphs,
		   unsigned صحيح *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);

    بدل (u.header.indexFormat)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    حالة 3: {
      hb_barrier ();
      אם (!u.format3.add_offset (c, end_offset, size))
	return_trace (שקר);
      אם (!(num_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (אמת);
    }
    // TODO: implement 2, 4, 5.
    حالة 2: حالة 4:  // No-op.
    حالة 5:  // Pad to 32-bit aligned.
    افتراضي: return_trace (שקר);
    }
  }

  منطقي
  add_offset (hb_serialize_context_t *c, unsigned صحيح local_offset,
	      unsigned صحيح *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.header.indexFormat)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    حالة 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    حالة 2:  // Should be a no-op.
    حالة 4: حالة 5:  // Handle sparse cases.
    افتراضي: return_trace (שקר);
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents HB_UNUSED, منطقي scale HB_UNUSED) ثابت
  {
    بدل (u.header.indexFormat)
    {
    حالة 2: حالة 5: /* TODO */
    حالة 1: حالة 3: حالة 4: /* Variable-metrics formats do not have metrics here. */
    افتراضي:החזר (שקר);
    }
  }

  منطقي
  get_image_data (unsigned صحيح idx, unsigned صحيح *offset,
		  unsigned صحيح *length, unsigned صحيح *format) ثابت
  {
    *format = u.header.imageFormat;
    بدل (u.header.indexFormat)
    {
    حالة 1: hb_barrier (); החזר u.format1.get_image_data (idx, offset, length);
    حالة 3: hb_barrier (); החזר u.format3.get_image_data (idx, offset, length);
    افتراضي: החזר שקר;
    }
  }

  ثابت IndexSubtableHeader* get_header () ثابت { החזר &u.header; }

  فراغ populate_header (unsigned index_format,
			unsigned image_format,
			unsigned صحيح image_data_offset,
			unsigned صحيح *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    بدل (u.header.indexFormat)
    {
    حالة 1: *size += IndexSubtableFormat1::min_size; اكسر;
    حالة 3: *size += IndexSubtableFormat3::min_size; اكسر;
    }
  }

  محمي:
  اتحاد {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  عام:
  DEFINE_SIZE_UNION (8, header);
};

بنية IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (ثابت IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    أكد (offsetToSubtable.is_null ());
    החזר *هذا;
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  ثابت IndexSubtable* get_subtable (ثابت فراغ *base) ثابت
  {
    החזר &(base+offsetToSubtable);
  }

  منطقي add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 ثابت hb_vector_t<hb_pair_t<hb_codepoint_t, ثابت IndexSubtableRecord*>> *lookup, /* IN */
			 ثابت فراغ *base,
			 unsigned صحيح *start /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    אם (unlikely (!c->serializer->extend_min (subtable))) return_trace (שקר);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned صحيح image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<حرف> data;
    hb_vector_t<unsigned صحيح> lengths;
    unsigned صحيح num_glyphs = 0;
    منطقي early_exit = שקר;
    עבור (unsigned صحيح i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      ثابت IndexSubtableRecord *next_record = (*lookup)[i].second;
      ثابت IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      אם (next_header != old_header)
      {
	*start = i;
	early_exit = אמת;
	اكسر;
      }
      unsigned صحيح num_missing = record->add_glyph_for_subset (new_gid);
      עבור (unsigned صحيح j = 0; j < num_missing; j++)
	אם (unlikely (!c->serializer->check_success (lengths.push_or_fail (0))))
	  return_trace (שקר);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      אם (old_gid < next_record->firstGlyphIndex)
	return_trace (שקר);

      unsigned صحيح old_idx = (unsigned صحيح) old_gid - next_record->firstGlyphIndex;
      unsigned صحيح offset, length, format;
      אם (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (שקר);
      אם (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (שקר);
      אם (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (שקר);
      אם (unlikely (!c->serializer->check_success (lengths.push_or_fail (length))))
	return_trace (שקר);
      num_glyphs += 1 + num_missing;
    }
    אם (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned صحيح image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    منطقي shared = שקר;
    uint64_t key = 0;
    אם (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_prime,
			    &image_data_offset);
    }
    אם (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_prime->length;
      אם (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_prime,
					    data.arrayZ, data.length))))
	return_trace (שקר);
      אם (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(فراغ) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned صحيح cumulative = 0;
    עבור (unsigned صحيح length : lengths)
    {
      אם (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (שקר);
      cumulative += length;
    }
    אם (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      num_glyphs,
					      &bitmap_size_context->size)))
      return_trace (שקר);
    return_trace (אמת);
  }

  منطقي add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       ثابت hb_vector_t<hb_pair_t<hb_codepoint_t, ثابت IndexSubtableRecord*>> *lookup, /* IN */
		       ثابت فراغ *base,
		       unsigned صحيح *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto snap = c->serializer->snapshot ();
    unsigned صحيح old_size = bitmap_size_context->size;
    unsigned صحيح old_cbdt_prime_length = bitmap_size_context->cbdt_prime->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    אם (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (שקר);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    אם (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->pop_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_prime->shrink (old_cbdt_prime_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (שקר);
    }

    bitmap_size_context->num_tables += 1;
    return_trace (אמת);
  }

  unsigned صحيح add_glyph_for_subset (hb_codepoint_t gid)
  {
    אם (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      החזר 0;
    }
    // TODO maybe assert? this shouldn't occur.
    אם (lastGlyphIndex > gid)
      החזר 0;
    unsigned صحيح num_missing = (unsigned صحيح) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    החזר num_missing;
  }

  منطقي get_extents (hb_glyph_extents_t *extents, ثابت فراغ *base, منطقي scale) ثابت
  { החזר (base+offsetToSubtable).get_extents (extents, scale); }

  منطقي get_image_data (unsigned صحيح  gid,
		       ثابت فراغ   *base,
		       unsigned صحيح *offset,
		       unsigned صحيح *length,
		       unsigned صحيح *format) ثابت
  {
    אם (gid < firstGlyphIndex || gid > lastGlyphIndex) החזר שקר;
    החזר (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية IndexSubtableArray
{
  friend بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح count) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (indexSubtablesZ.sanitize (c, count, هذا));
  }

  فراغ
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		ثابت IndexSubtableRecord*>> *lookup /* OUT */) ثابت
  {
    منطقي start_glyph_is_set = שקר;
    unsigned num_glyphs = c->plan->num_output_glyphs ();
    עבור (hb_codepoint_t new_gid = 0; new_gid < num_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      אם (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) استمر;

      ثابت IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->num_tables);
      אם (unlikely (!record)) استمر;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned صحيح offset, length, format;
      אם (unlikely (!record->get_image_data (old_gid, هذا, &offset, &length, &format))) استمر;

      lookup->push (hb_pair_t<hb_codepoint_t, ثابت IndexSubtableRecord*> (new_gid, record));

      אם (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = אמת;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  منطقي
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_vector_t<hb_pair_t<hb_codepoint_t, ثابت IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    אם (unlikely (!c->serializer->propagate_error (lookup)))
      החזר שקר;

    bitmap_size_context->size = 0;
    bitmap_size_context->num_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    עבור (unsigned صحيح start = 0; start < lookup.length;)
    {
      אם (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, هذا, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	עבור (unsigned صحيح i = 0; i < records.length; i++)
	  c->serializer->pop_discard ();
	return_trace (שקר);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    עבור (unsigned صحيح i = 0; i < records.length; i++)
      objidxs.push (c->serializer->pop_pack ());
    עבור (unsigned صحيح i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      אם (unlikely (!record)) return_trace (שקר);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (אמת);
  }

  عام:
  ثابت IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned صحيح numTables) ثابت
  {
    עבור (unsigned صحيح i = 0; i < numTables; ++i)
    {
      unsigned صحيح firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned صحيح lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      אם (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	החזר &indexSubtablesZ[i];
    }
    החזר nullptr;
  }

  محمي:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

بنية BitmapSizeTable
{
  friend بنية CBLC;
  friend بنية CBDT;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, numberOfIndexSubtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ثابت IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, ثابت فراغ *base, ثابت فراغ **out_base) ثابت
  {
    *out_base = &(base+indexSubtableArrayOffset);
    החזר (base+indexSubtableArrayOffset).find_table (glyph, numberOfIndexSubtables);
  }

  منطقي
  subset (hb_subset_context_t *c, ثابت فراغ *base,
	  ثابت حرف *cbdt, unsigned صحيح cbdt_length,
	  hb_vector_t<حرف> *cbdt_prime /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out_table = c->serializer->embed (هذا);
    אם (unlikely (!out_table)) return_trace (שקר);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_prime = cbdt_prime;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.num_tables = numberOfIndexSubtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    אם (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (שקר);
    אם (!bitmap_size_context.size ||
	!bitmap_size_context.num_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (שקר);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->numberOfIndexSubtables = bitmap_size_context.num_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (אמת);
  }

  محمي:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		numberOfIndexSubtables;
  HBUINT32		colorRef;
  SBitLineMetrics	horizontal;
  SBitLineMetrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  عام:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

بنية GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  عام:
  DEFINE_SIZE_ARRAY (9, data);
};

بنية GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  عام:
  DEFINE_SIZE_ARRAY (12, data);
};

بنية GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  عام:
  DEFINE_SIZE_ARRAY (4, data);
};

بنية CBLC
{
  friend بنية CBDT;

  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, هذا));
  }

  ساكن منطقي
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<حرف>* cbdt_prime)
  {
    hb_blob_t *cbdt_prime_blob = hb_blob_create (cbdt_prime->arrayZ,
						 cbdt_prime->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_prime->arrayZ,
						 hb_free);
    cbdt_prime->init ();  // Leak arrayZ to the blob.
    منطقي ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_prime_blob);
    hb_blob_destroy (cbdt_prime_blob);
    החזר ret;
  }

  منطقي
  subset_size_table (hb_subset_context_t *c, ثابت BitmapSizeTable& table,
		     ثابت حرف *cbdt /* IN */, unsigned صحيح cbdt_length,
		     CBLC *cblc_prime /* INOUT */, hb_vector_t<حرف> *cbdt_prime /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    cblc_prime->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_prime_len = cbdt_prime->length;

    אם (!table.subset (c, هذا, cbdt, cbdt_length, cbdt_prime, dedup))
    {
      cblc_prime->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_prime->shrink (cbdt_prime_len);
      return_trace (שקר);
    }
    return_trace (אמת);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  ثابت BitmapSizeTable &choose_strike (hb_font_t *font) ثابت
  {
    unsigned count = sizeTables.len;
    אם (unlikely (!count))
      החזר Null (BitmapSizeTable);

    unsigned صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    אם (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned صحيح best_i = 0;
    unsigned صحيح best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    עבור (unsigned صحيح i = 1; i < count; i++)
    {
      unsigned صحيح ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      אם ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    החזר sizeTables[best_i];
  }

  محمي:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  عام:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

بنية CBDT
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      هذا->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      هذا->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      هذا->cblc.destroy ();
      هذا->cbdt.destroy ();
    }

    منطقي
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, منطقي scale = אמת) ثابت
    {
      ثابت فراغ *base;
      ثابت BitmapSizeTable &strike = هذا->cblc->choose_strike (font);
      ثابت IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      אם (!subtable_record || !strike.ppemX || !strike.ppemY)
	החזר שקר;

      אם (subtable_record->get_extents (extents, base, scale))
	החזר אמת;

      unsigned صحيح image_offset = 0, image_length = 0, image_format = 0;
      אם (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	החזר שקר;

      unsigned صحيح cbdt_len = cbdt.get_length ();
      אם (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	החזר שקר;

      بدل (image_format)
      {
      حالة 17: {
	אם (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  החזר שקר;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (هذا->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	اكسر;
      }
      حالة 18: {
	אם (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  החזר שקר;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (هذا->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	اكسر;
      }
      افتراضي: החזר שקר; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      אם (scale)
      {
	عائم x_scale = upem / (عائم) strike.ppemX;
	عائم y_scale = upem / (عائم) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      החזר אמת;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ثابت
    {
      ثابت فراغ *base;
      ثابت BitmapSizeTable &strike = هذا->cblc->choose_strike (font);
      ثابت IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      אם (!subtable_record || !strike.ppemX || !strike.ppemY)
	החזר hb_blob_get_empty ();

      unsigned صحيح image_offset = 0, image_length = 0, image_format = 0;
      אם (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	החזר hb_blob_get_empty ();

      unsigned صحيح cbdt_len = cbdt.get_length ();
      אם (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	החזר hb_blob_get_empty ();

      بدل (image_format)
      {
      حالة 17:
      {
	אם (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  החזר hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (هذا->cbdt, image_offset);
	החזר hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      حالة 18:
      {
	אם (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  החזר hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (هذا->cbdt, image_offset);
	החזר hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      حالة 19:
      {
	אם (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  החזר hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (هذا->cbdt, image_offset);
	החזר hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      افتراضي: החזר hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    منطقي has_data () ثابت { החזר cbdt->version.major; }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *data) ثابت
    {
      אם (!has_data ()) החזר שקר;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      אם (unlikely (!font->get_glyph_extents (glyph, &extents, שקר)))
        החזר שקר;

      אם (unlikely (!get_extents (font, glyph, &pixel_extents, שקר)))
        החזר שקר;

      hb_blob_t *blob = reference_png (font, glyph);
      אם (unlikely (hb_blob_is_immutable (blob)))
        החזר שקר;

      منطقي ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      החזר ret;
    }

    خاص:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned صحيح upem;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  محمي:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  عام:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

مضمن منطقي
CBLC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<حرف> cbdt_prime;

  auto *cblc_prime = c->serializer->start_embed<CBLC> ();
  אם (unlikely (!c->serializer->extend_min (cblc_prime))) return_trace (שקר);
  cblc_prime->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned صحيح cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  אם (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (שקר);
  }
  _copy_data_to_cbdt (&cbdt_prime, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  עבור (ثابت BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (ثابت حرف *) cbdt, cbdt_length, cblc_prime, &cbdt_prime, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_prime));
}

بنية CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
