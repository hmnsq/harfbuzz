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

#অন্তর্ভাব "../../../hb-open-type.hh"
#অন্তর্ভাব "../../../hb-paint.hh"

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


নামাকাশ OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
সংরচনা cbdt_dedup_context_t
{
  সংরচনা entry_t
  {
    unsigned পূর্ণাঙ্ক base;		/* Offset of the data in cbdt_prime. */
    unsigned পূর্ণাঙ্ক lstart;	/* Start index in lengths. */
    unsigned পূর্ণাঙ্ক lcount;	/* Number of length entries. */
  };

  স্থির uint64_t hash (unsigned পূর্ণাঙ্ক image_format,
			ধ্রুব hb_vector_t<বর্ণ> &data,
			ধ্রুব hb_vector_t<unsigned পূর্ণাঙ্ক> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((ধ্রুব বর্ণ *) lengths.arrayZ,
				      lengths.length * পরিমাণ (unsigned পূর্ণাঙ্ক)));
    প্রত্যাগম ((uint64_t) image_format << 32) | h;
  }

  তার্কিক find (uint64_t key,
	     ধ্রুব hb_vector_t<বর্ণ> &data,
	     ধ্রুব hb_vector_t<unsigned পূর্ণাঙ্ক> &lengths,
	     ধ্রুব hb_vector_t<বর্ণ> &cbdt_মৌলিক,
	     unsigned পূর্ণাঙ্ক *base /* OUT */) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক *idx_p = nullptr;
    যদি (!map.has (key, &idx_p)) প্রত্যাগম অসত্য;
    ধ্রুব entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    যদি (e.lcount != lengths.length) প্রত্যাগম অসত্য;
    যদি (unlikely (e.base + data.length > (unsigned পূর্ণাঙ্ক) cbdt_মৌলিক.length)) প্রত্যাগম অসত্য;
    যদি (hb_memcmp (cbdt_মৌলিক.arrayZ + e.base, data.arrayZ, data.length)) প্রত্যাগম অসত্য;
    যদি (hb_memcmp (এতত্->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * পরিমাণ (unsigned পূর্ণাঙ্ক)))
      প্রত্যাগম অসত্য;
    *base = e.base;
    প্রত্যাগম সত্য;
  }

  তার্কিক add (uint64_t key,
	    unsigned পূর্ণাঙ্ক base,
	    ধ্রুব hb_vector_t<unsigned পূর্ণাঙ্ক> &lengths)
  {
    যদি (unlikely (entries.in_error () || এতত্->lengths.in_error () || map.in_error ()))
      প্রত্যাগম অসত্য;
    entry_t e = {base, এতত্->lengths.length, lengths.length};
    যদি (unlikely (!entries.ঠেলো_or_fail (e))) প্রত্যাগম অসত্য;
    প্রতি (unsigned পূর্ণাঙ্ক length : lengths)
      যদি (unlikely (!এতত্->lengths.ঠেলো_or_fail (length))) প্রত্যাগম অসত্য;
    প্রত্যাগম map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned পূর্ণাঙ্ক> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned পূর্ণাঙ্ক> lengths; /* Concatenated per-entry lengths. */
};

সংরচনা cblc_bitmap_size_subset_context_t
{
  ধ্রুব বর্ণ *cbdt;
  unsigned পূর্ণাঙ্ক cbdt_length;
  hb_vector_t<বর্ণ> *cbdt_মৌলিক;
  unsigned পূর্ণাঙ্ক size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned পূর্ণাঙ্ক অঙ্ক_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

স্থির অন্তঃস্থ তার্কিক
_copy_data_to_cbdt (hb_vector_t<বর্ণ> *cbdt_মৌলিক,
		    ধ্রুব শূন্য        *data,
		    unsigned           length)
{
  unsigned পূর্ণাঙ্ক new_len = cbdt_মৌলিক->length + length;
  যদি (unlikely (!cbdt_মৌলিক->alloc (new_len))) প্রত্যাগম অসত্য;
  hb_memcpy (cbdt_মৌলিক->arrayZ + cbdt_মৌলিক->length, data, length);
  cbdt_মৌলিক->length = new_len;
  প্রত্যাগম সত্য;
}

সংরচনা SmallGlyphMetrics
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  শূন্য get_extents (hb_font_t *font, hb_glyph_extents_t *extents, তার্কিক scale) ধ্রুব
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<পূর্ণাঙ্ক> (height);

    যদি (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (5);
};

সংরচনা BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা SBit_সারি_Metrics
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
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
  সার্বজনিক:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

সংরচনা IndexSubtableHeader
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
সংরচনা IndexSubtableFormat1Or3
{
  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক glyph_গুনতি) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  offsetArrayZ.sanitize (c, glyph_গুনতি + 1));
  }

  তার্কিক get_image_data (unsigned পূর্ণাঙ্ক idx,
		       unsigned পূর্ণাঙ্ক *offset,
		       unsigned পূর্ণাঙ্ক *length) ধ্রুব
  {
    যদি (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      প্রত্যাগম অসত্য;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    প্রত্যাগম সত্য;
  }

  তার্কিক add_offset (hb_serialize_context_t *c,
		   unsigned পূর্ণাঙ্ক offset,
		   unsigned পূর্ণাঙ্ক *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (এতত্);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += পরিমাণ (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((তার্কিক) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

সংরচনা IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
সংরচনা IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

সংরচনা IndexSubtable
{
  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক glyph_গুনতি) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.header.sanitize (c)) return_trace (অসত্য);
    hb_barrier ();
    বিকল্প (u.header.indexFormat)
    {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_গুনতি));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_গুনতি));
    স্বভাব:return_trace (সত্য);
    }
  }

  তার্কিক
  finish_subtable (hb_serialize_context_t *c,
		   unsigned পূর্ণাঙ্ক end_offset,
		   unsigned পূর্ণাঙ্ক অঙ্ক_glyphs,
		   unsigned পূর্ণাঙ্ক *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (এতত্);

    বিকল্প (u.header.indexFormat)
    {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    পক্ষ 3: {
      hb_barrier ();
      যদি (!u.format3.add_offset (c, end_offset, size))
	return_trace (অসত্য);
      যদি (!(অঙ্ক_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (সত্য);
    }
    // TODO: implement 2, 4, 5.
    পক্ষ 2: পক্ষ 4:  // No-op.
    পক্ষ 5:  // Pad to 32-bit aligned.
    স্বভাব: return_trace (অসত্য);
    }
  }

  তার্কিক
  add_offset (hb_serialize_context_t *c, unsigned পূর্ণাঙ্ক local_offset,
	      unsigned পূর্ণাঙ্ক *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (এতত্);
    বিকল্প (u.header.indexFormat)
    {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    পক্ষ 2:  // Should be a no-op.
    পক্ষ 4: পক্ষ 5:  // Handle sparse cases.
    স্বভাব: return_trace (অসত্য);
    }
  }

  তার্কিক get_extents (hb_glyph_extents_t *extents HB_UNUSED, তার্কিক scale HB_UNUSED) ধ্রুব
  {
    বিকল্প (u.header.indexFormat)
    {
    পক্ষ 2: পক্ষ 5: /* TODO */
    পক্ষ 1: পক্ষ 3: পক্ষ 4: /* Variable-metrics formats do not have metrics here. */
    স্বভাব:প্রত্যাগম (অসত্য);
    }
  }

  তার্কিক
  get_image_data (unsigned পূর্ণাঙ্ক idx, unsigned পূর্ণাঙ্ক *offset,
		  unsigned পূর্ণাঙ্ক *length, unsigned পূর্ণাঙ্ক *format) ধ্রুব
  {
    *format = u.header.imageFormat;
    বিকল্প (u.header.indexFormat)
    {
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.format1.get_image_data (idx, offset, length);
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.get_image_data (idx, offset, length);
    স্বভাব: প্রত্যাগম অসত্য;
    }
  }

  ধ্রুব IndexSubtableHeader* get_header () ধ্রুব { প্রত্যাগম &u.header; }

  শূন্য populate_header (unsigned index_format,
			unsigned image_format,
			unsigned পূর্ণাঙ্ক image_data_offset,
			unsigned পূর্ণাঙ্ক *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    বিকল্প (u.header.indexFormat)
    {
    পক্ষ 1: *size += IndexSubtableFormat1::min_size; ভঙ্গ;
    পক্ষ 3: *size += IndexSubtableFormat3::min_size; ভঙ্গ;
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  সার্বজনিক:
  DEFINE_SIZE_UNION (8, header);
};

সংরচনা IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (ধ্রুব IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    প্রতিজ্ঞা (offsetToSubtable.is_null ());
    প্রত্যাগম *এতত্;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব শূন্য *base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  ধ্রুব IndexSubtable* get_subtable (ধ্রুব শূন্য *base) ধ্রুব
  {
    প্রত্যাগম &(base+offsetToSubtable);
  }

  তার্কিক add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 ধ্রুব hb_vector_t<hb_pair_t<hb_codepoint_t, ধ্রুব IndexSubtableRecord*>> *lookup, /* IN */
			 ধ্রুব শূন্য *base,
			 unsigned পূর্ণাঙ্ক *start /* INOUT */) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    যদি (unlikely (!c->serializer->extend_min (subtable))) return_trace (অসত্য);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned পূর্ণাঙ্ক image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<বর্ণ> data;
    hb_vector_t<unsigned পূর্ণাঙ্ক> lengths;
    unsigned পূর্ণাঙ্ক অঙ্ক_glyphs = 0;
    তার্কিক early_exit = অসত্য;
    প্রতি (unsigned পূর্ণাঙ্ক i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      ধ্রুব IndexSubtableRecord *next_record = (*lookup)[i].second;
      ধ্রুব IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      যদি (next_header != old_header)
      {
	*start = i;
	early_exit = সত্য;
	ভঙ্গ;
      }
      unsigned পূর্ণাঙ্ক অঙ্ক_missing = record->add_glyph_for_subset (new_gid);
      প্রতি (unsigned পূর্ণাঙ্ক j = 0; j < অঙ্ক_missing; j++)
	যদি (unlikely (!c->serializer->check_success (lengths.ঠেলো_or_fail (0))))
	  return_trace (অসত্য);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      যদি (old_gid < next_record->firstGlyphIndex)
	return_trace (অসত্য);

      unsigned পূর্ণাঙ্ক old_idx = (unsigned পূর্ণাঙ্ক) old_gid - next_record->firstGlyphIndex;
      unsigned পূর্ণাঙ্ক offset, length, format;
      যদি (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (অসত্য);
      যদি (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (অসত্য);
      যদি (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (অসত্য);
      যদি (unlikely (!c->serializer->check_success (lengths.ঠেলো_or_fail (length))))
	return_trace (অসত্য);
      অঙ্ক_glyphs += 1 + অঙ্ক_missing;
    }
    যদি (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned পূর্ণাঙ্ক image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    তার্কিক shared = অসত্য;
    uint64_t key = 0;
    যদি (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_মৌলিক,
			    &image_data_offset);
    }
    যদি (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_মৌলিক->length;
      যদি (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_মৌলিক,
					    data.arrayZ, data.length))))
	return_trace (অসত্য);
      যদি (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(শূন্য) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned পূর্ণাঙ্ক cumulative = 0;
    প্রতি (unsigned পূর্ণাঙ্ক length : lengths)
    {
      যদি (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (অসত্য);
      cumulative += length;
    }
    যদি (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      অঙ্ক_glyphs,
					      &bitmap_size_context->size)))
      return_trace (অসত্য);
    return_trace (সত্য);
  }

  তার্কিক add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       ধ্রুব hb_vector_t<hb_pair_t<hb_codepoint_t, ধ্রুব IndexSubtableRecord*>> *lookup, /* IN */
		       ধ্রুব শূন্য *base,
		       unsigned পূর্ণাঙ্ক *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto snap = c->serializer->snapshot ();
    unsigned পূর্ণাঙ্ক old_size = bitmap_size_context->size;
    unsigned পূর্ণাঙ্ক old_cbdt_মৌলিক_length = bitmap_size_context->cbdt_মৌলিক->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    যদি (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (অসত্য);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    যদি (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->বের_করো_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_মৌলিক->shrink (old_cbdt_মৌলিক_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (অসত্য);
    }

    bitmap_size_context->অঙ্ক_tables += 1;
    return_trace (সত্য);
  }

  unsigned পূর্ণাঙ্ক add_glyph_for_subset (hb_codepoint_t gid)
  {
    যদি (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      প্রত্যাগম 0;
    }
    // TODO maybe assert? this shouldn't occur.
    যদি (lastGlyphIndex > gid)
      প্রত্যাগম 0;
    unsigned পূর্ণাঙ্ক অঙ্ক_missing = (unsigned পূর্ণাঙ্ক) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    প্রত্যাগম অঙ্ক_missing;
  }

  তার্কিক get_extents (hb_glyph_extents_t *extents, ধ্রুব শূন্য *base, তার্কিক scale) ধ্রুব
  { প্রত্যাগম (base+offsetToSubtable).get_extents (extents, scale); }

  তার্কিক get_image_data (unsigned পূর্ণাঙ্ক  gid,
		       ধ্রুব শূন্য   *base,
		       unsigned পূর্ণাঙ্ক *offset,
		       unsigned পূর্ণাঙ্ক *length,
		       unsigned পূর্ণাঙ্ক *format) ধ্রুব
  {
    যদি (gid < firstGlyphIndex || gid > lastGlyphIndex) প্রত্যাগম অসত্য;
    প্রত্যাগম (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা IndexSubtableArray
{
  friend সংরচনা CBDT;

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক গুনতি) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (indexSubtablesZ.sanitize (c, গুনতি, এতত্));
  }

  শূন্য
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		ধ্রুব IndexSubtableRecord*>> *lookup /* OUT */) ধ্রুব
  {
    তার্কিক start_glyph_is_set = অসত্য;
    unsigned অঙ্ক_glyphs = c->plan->অঙ্ক_output_glyphs ();
    প্রতি (hb_codepoint_t new_gid = 0; new_gid < অঙ্ক_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      যদি (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) অনুবর্ত;

      ধ্রুব IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->অঙ্ক_tables);
      যদি (unlikely (!record)) অনুবর্ত;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned পূর্ণাঙ্ক offset, length, format;
      যদি (unlikely (!record->get_image_data (old_gid, এতত্, &offset, &length, &format))) অনুবর্ত;

      lookup->push (hb_pair_t<hb_codepoint_t, ধ্রুব IndexSubtableRecord*> (new_gid, record));

      যদি (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = সত্য;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  তার্কিক
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) ধ্রুব
  {
    TRACE_SUBSET (এতত্);

    hb_vector_t<hb_pair_t<hb_codepoint_t, ধ্রুব IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    যদি (unlikely (!c->serializer->propagate_error (lookup)))
      প্রত্যাগম অসত্য;

    bitmap_size_context->size = 0;
    bitmap_size_context->অঙ্ক_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    প্রতি (unsigned পূর্ণাঙ্ক start = 0; start < lookup.length;)
    {
      যদি (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, এতত্, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < records.length; i++)
	  c->serializer->বের_করো_discard ();
	return_trace (অসত্য);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < records.length; i++)
      objidxs.push (c->serializer->বের_করো_pack ());
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      যদি (unlikely (!record)) return_trace (অসত্য);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (সত্য);
  }

  সার্বজনিক:
  ধ্রুব IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned পূর্ণাঙ্ক অঙ্ক_Tables) ধ্রুব
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_Tables; ++i)
    {
      unsigned পূর্ণাঙ্ক firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned পূর্ণাঙ্ক lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      যদি (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	প্রত্যাগম &indexSubtablesZ[i];
    }
    প্রত্যাগম nullptr;
  }

  রক্ষিত:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

সংরচনা BitmapSizeTable
{
  friend সংরচনা CBLC;
  friend সংরচনা CBDT;

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব শূন্য *base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, সংখ্যা_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ধ্রুব IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, ধ্রুব শূন্য *base, ধ্রুব শূন্য **out_base) ধ্রুব
  {
    *out_base = &(base+indexSubtableArrayOffset);
    প্রত্যাগম (base+indexSubtableArrayOffset).find_table (glyph, সংখ্যা_Of_Index_Subtables);
  }

  তার্কিক
  subset (hb_subset_context_t *c, ধ্রুব শূন্য *base,
	  ধ্রুব বর্ণ *cbdt, unsigned পূর্ণাঙ্ক cbdt_length,
	  hb_vector_t<বর্ণ> *cbdt_মৌলিক /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out_table = c->serializer->embed (এতত্);
    যদি (unlikely (!out_table)) return_trace (অসত্য);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_মৌলিক = cbdt_মৌলিক;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.অঙ্ক_tables = সংখ্যা_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    যদি (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (অসত্য);
    যদি (!bitmap_size_context.size ||
	!bitmap_size_context.অঙ্ক_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (অসত্য);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->সংখ্যা_Of_Index_Subtables = bitmap_size_context.অঙ্ক_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (সত্য);
  }

  রক্ষিত:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		সংখ্যা_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_সারি_Metrics	horizontal;
  SBit_সারি_Metrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

সংরচনা GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (9, data);
};

সংরচনা GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (12, data);
};

সংরচনা GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (4, data);
};

সংরচনা CBLC
{
  friend সংরচনা CBDT;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, এতত্));
  }

  স্থির তার্কিক
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<বর্ণ>* cbdt_মৌলিক)
  {
    hb_blob_t *cbdt_মৌলিক_blob = hb_blob_create (cbdt_মৌলিক->arrayZ,
						 cbdt_মৌলিক->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_মৌলিক->arrayZ,
						 hb_free);
    cbdt_মৌলিক->init ();  // Leak arrayZ to the blob.
    তার্কিক ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_মৌলিক_blob);
    hb_blob_destroy (cbdt_মৌলিক_blob);
    প্রত্যাগম ret;
  }

  তার্কিক
  subset_size_table (hb_subset_context_t *c, ধ্রুব BitmapSizeTable& table,
		     ধ্রুব বর্ণ *cbdt /* IN */, unsigned পূর্ণাঙ্ক cbdt_length,
		     CBLC *cblc_মৌলিক /* INOUT */, hb_vector_t<বর্ণ> *cbdt_মৌলিক /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    cblc_মৌলিক->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_মৌলিক_len = cbdt_মৌলিক->length;

    যদি (!table.subset (c, এতত্, cbdt, cbdt_length, cbdt_মৌলিক, dedup))
    {
      cblc_মৌলিক->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_মৌলিক->shrink (cbdt_মৌলিক_len);
      return_trace (অসত্য);
    }
    return_trace (সত্য);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL তার্কিক subset (hb_subset_context_t *c) ধ্রুব;

  রক্ষিত:
  ধ্রুব BitmapSizeTable &choose_strike (hb_font_t *font) ধ্রুব
  {
    unsigned গুনতি = sizeTables.len;
    যদি (unlikely (!গুনতি))
      প্রত্যাগম Null (BitmapSizeTable);

    unsigned পূর্ণাঙ্ক requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    যদি (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned পূর্ণাঙ্ক best_i = 0;
    unsigned পূর্ণাঙ্ক best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    প্রতি (unsigned পূর্ণাঙ্ক i = 1; i < গুনতি; i++)
    {
      unsigned পূর্ণাঙ্ক ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      যদি ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    প্রত্যাগম sizeTables[best_i];
  }

  রক্ষিত:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

সংরচনা CBDT
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      এতত্->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      এতত্->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      এতত্->cblc.destroy ();
      এতত্->cbdt.destroy ();
    }

    তার্কিক
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, তার্কিক scale = সত্য) ধ্রুব
    {
      ধ্রুব শূন্য *base;
      ধ্রুব BitmapSizeTable &strike = এতত্->cblc->choose_strike (font);
      ধ্রুব IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      যদি (!subtable_record || !strike.ppemX || !strike.ppemY)
	প্রত্যাগম অসত্য;

      যদি (subtable_record->get_extents (extents, base, scale))
	প্রত্যাগম সত্য;

      unsigned পূর্ণাঙ্ক image_offset = 0, image_length = 0, image_format = 0;
      যদি (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	প্রত্যাগম অসত্য;

      unsigned পূর্ণাঙ্ক cbdt_len = cbdt.get_length ();
      যদি (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	প্রত্যাগম অসত্য;

      বিকল্প (image_format)
      {
      পক্ষ 17: {
	যদি (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  প্রত্যাগম অসত্য;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (এতত্->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	ভঙ্গ;
      }
      পক্ষ 18: {
	যদি (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  প্রত্যাগম অসত্য;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (এতত্->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	ভঙ্গ;
      }
      স্বভাব: প্রত্যাগম অসত্য; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      যদি (scale)
      {
	ভিন্ন x_scale = upem / (ভিন্ন) strike.ppemX;
	ভিন্ন y_scale = upem / (ভিন্ন) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      প্রত্যাগম সত্য;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ধ্রুব
    {
      ধ্রুব শূন্য *base;
      ধ্রুব BitmapSizeTable &strike = এতত্->cblc->choose_strike (font);
      ধ্রুব IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      যদি (!subtable_record || !strike.ppemX || !strike.ppemY)
	প্রত্যাগম hb_blob_get_empty ();

      unsigned পূর্ণাঙ্ক image_offset = 0, image_length = 0, image_format = 0;
      যদি (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	প্রত্যাগম hb_blob_get_empty ();

      unsigned পূর্ণাঙ্ক cbdt_len = cbdt.get_length ();
      যদি (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	প্রত্যাগম hb_blob_get_empty ();

      বিকল্প (image_format)
      {
      পক্ষ 17:
      {
	যদি (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  প্রত্যাগম hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (এতত্->cbdt, image_offset);
	প্রত্যাগম hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      পক্ষ 18:
      {
	যদি (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  প্রত্যাগম hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (এতত্->cbdt, image_offset);
	প্রত্যাগম hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      পক্ষ 19:
      {
	যদি (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  প্রত্যাগম hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (এতত্->cbdt, image_offset);
	প্রত্যাগম hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      স্বভাব: প্রত্যাগম hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    তার্কিক has_data () ধ্রুব { প্রত্যাগম cbdt->version.major; }

    তার্কিক paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, শূন্য *data) ধ্রুব
    {
      যদি (!has_data ()) প্রত্যাগম অসত্য;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      যদি (unlikely (!font->get_glyph_extents (glyph, &extents, অসত্য)))
        প্রত্যাগম অসত্য;

      যদি (unlikely (!get_extents (font, glyph, &pixel_extents, অসত্য)))
        প্রত্যাগম অসত্য;

      hb_blob_t *blob = reference_png (font, glyph);
      যদি (unlikely (hb_blob_is_immutable (blob)))
        প্রত্যাগম অসত্য;

      তার্কিক ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      প্রত্যাগম ret;
    }

    গুপ্ত:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned পূর্ণাঙ্ক upem;
  };

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  রক্ষিত:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

অন্তঃস্থ তার্কিক
CBLC::subset (hb_subset_context_t *c) ধ্রুব
{
  TRACE_SUBSET (এতত্);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<বর্ণ> cbdt_মৌলিক;

  auto *cblc_মৌলিক = c->serializer->start_embed<CBLC> ();
  যদি (unlikely (!c->serializer->extend_min (cblc_মৌলিক))) return_trace (অসত্য);
  cblc_মৌলিক->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned পূর্ণাঙ্ক cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  যদি (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (অসত্য);
  }
  _copy_data_to_cbdt (&cbdt_মৌলিক, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  প্রতি (ধ্রুব BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (ধ্রুব বর্ণ *) cbdt, cbdt_length, cblc_মৌলিক, &cbdt_মৌলিক, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_মৌলিক));
}

সংরচনা CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
