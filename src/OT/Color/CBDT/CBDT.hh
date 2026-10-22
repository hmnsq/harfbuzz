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

#شامل "../../../hb-open-type.hh"
#شامل "../../../hb-paint.hh"

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


فضاینام OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
ساختار cbdt_dedup_context_t
{
  ساختار entry_t
  {
    unsigned صحیح base;		/* Offset of the data in cbdt_prime. */
    unsigned صحیح lstart;	/* Start index in lengths. */
    unsigned صحیح lcount;	/* Number of length entries. */
  };

  ایستا uint64_t hash (unsigned صحیح image_format,
			ثابت hb_vector_t<نویسه> &data,
			ثابت hb_vector_t<unsigned صحیح> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((ثابت نویسه *) lengths.arrayZ,
				      lengths.length * اندازه (unsigned صحیح)));
    بازگشت ((uint64_t) image_format << 32) | h;
  }

  منطقی find (uint64_t key,
	     ثابت hb_vector_t<نویسه> &data,
	     ثابت hb_vector_t<unsigned صحیح> &lengths,
	     ثابت hb_vector_t<نویسه> &cbdt_اول,
	     unsigned صحیح *base /* OUT */) ثابت
  {
    unsigned صحیح *idx_p = nullptr;
    اگر (!map.has (key, &idx_p)) بازگشت نادرست;
    ثابت entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    اگر (e.lcount != lengths.length) بازگشت نادرست;
    اگر (unlikely (e.base + data.length > (unsigned صحیح) cbdt_اول.length)) بازگشت نادرست;
    اگر (hb_memcmp (cbdt_اول.arrayZ + e.base, data.arrayZ, data.length)) بازگشت نادرست;
    اگر (hb_memcmp (این->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * اندازه (unsigned صحیح)))
      بازگشت نادرست;
    *base = e.base;
    بازگشت درست;
  }

  منطقی add (uint64_t key,
	    unsigned صحیح base,
	    ثابت hb_vector_t<unsigned صحیح> &lengths)
  {
    اگر (unlikely (entries.in_error () || این->lengths.in_error () || map.in_error ()))
      بازگشت نادرست;
    entry_t e = {base, این->lengths.length, lengths.length};
    اگر (unlikely (!entries.افزودن_or_fail (e))) بازگشت نادرست;
    برای (unsigned صحیح length : lengths)
      اگر (unlikely (!این->lengths.افزودن_or_fail (length))) بازگشت نادرست;
    بازگشت map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned صحیح> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned صحیح> lengths; /* Concatenated per-entry lengths. */
};

ساختار cblc_bitmap_size_subset_context_t
{
  ثابت نویسه *cbdt;
  unsigned صحیح cbdt_length;
  hb_vector_t<نویسه> *cbdt_اول;
  unsigned صحیح size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned صحیح رقم_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ایستا درونخطی منطقی
_copy_data_to_cbdt (hb_vector_t<نویسه> *cbdt_اول,
		    ثابت تهی        *data,
		    unsigned           length)
{
  unsigned صحیح new_len = cbdt_اول->length + length;
  اگر (unlikely (!cbdt_اول->alloc (new_len))) بازگشت نادرست;
  hb_memcpy (cbdt_اول->arrayZ + cbdt_اول->length, data, length);
  cbdt_اول->length = new_len;
  بازگشت درست;
}

ساختار SmallGlyphMetrics
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی get_extents (hb_font_t *font, hb_glyph_extents_t *extents, منطقی scale) ثابت
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<صحیح> (height);

    اگر (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  عمومی:
  DEFINE_SIZE_STATIC (5);
};

ساختار BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار SBit_سطر_Metrics
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
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
  عمومی:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

ساختار IndexSubtableHeader
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
ساختار IndexSubtableFormat1Or3
{
  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح glyph_شمار) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  offsetArrayZ.sanitize (c, glyph_شمار + 1));
  }

  منطقی get_image_data (unsigned صحیح idx,
		       unsigned صحیح *offset,
		       unsigned صحیح *length) ثابت
  {
    اگر (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      بازگشت نادرست;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    بازگشت درست;
  }

  منطقی add_offset (hb_serialize_context_t *c,
		   unsigned صحیح offset,
		   unsigned صحیح *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (این);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += اندازه (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((منطقی) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  عمومی:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

ساختار IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
ساختار IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

ساختار IndexSubtable
{
  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح glyph_شمار) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.header.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.header.indexFormat)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_شمار));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_شمار));
    پیشفرض:return_trace (درست);
    }
  }

  منطقی
  finish_subtable (hb_serialize_context_t *c,
		   unsigned صحیح end_offset,
		   unsigned صحیح رقم_glyphs,
		   unsigned صحیح *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (این);

    انتخاب (u.header.indexFormat)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    حالت 3: {
      hb_barrier ();
      اگر (!u.format3.add_offset (c, end_offset, size))
	return_trace (نادرست);
      اگر (!(رقم_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (درست);
    }
    // TODO: implement 2, 4, 5.
    حالت 2: حالت 4:  // No-op.
    حالت 5:  // Pad to 32-bit aligned.
    پیشفرض: return_trace (نادرست);
    }
  }

  منطقی
  add_offset (hb_serialize_context_t *c, unsigned صحیح local_offset,
	      unsigned صحیح *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (این);
    انتخاب (u.header.indexFormat)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    حالت 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    حالت 2:  // Should be a no-op.
    حالت 4: حالت 5:  // Handle sparse cases.
    پیشفرض: return_trace (نادرست);
    }
  }

  منطقی get_extents (hb_glyph_extents_t *extents HB_UNUSED, منطقی scale HB_UNUSED) ثابت
  {
    انتخاب (u.header.indexFormat)
    {
    حالت 2: حالت 5: /* TODO */
    حالت 1: حالت 3: حالت 4: /* Variable-metrics formats do not have metrics here. */
    پیشفرض:بازگشت (نادرست);
    }
  }

  منطقی
  get_image_data (unsigned صحیح idx, unsigned صحیح *offset,
		  unsigned صحیح *length, unsigned صحیح *format) ثابت
  {
    *format = u.header.imageFormat;
    انتخاب (u.header.indexFormat)
    {
    حالت 1: hb_barrier (); بازگشت u.format1.get_image_data (idx, offset, length);
    حالت 3: hb_barrier (); بازگشت u.format3.get_image_data (idx, offset, length);
    پیشفرض: بازگشت نادرست;
    }
  }

  ثابت IndexSubtableHeader* get_header () ثابت { بازگشت &u.header; }

  تهی populate_header (unsigned index_format,
			unsigned image_format,
			unsigned صحیح image_data_offset,
			unsigned صحیح *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    انتخاب (u.header.indexFormat)
    {
    حالت 1: *size += IndexSubtableFormat1::min_size; بشکن;
    حالت 3: *size += IndexSubtableFormat3::min_size; بشکن;
    }
  }

  محافظتشده:
  اجتماع {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  عمومی:
  DEFINE_SIZE_UNION (8, header);
};

ساختار IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (ثابت IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    أكد (offsetToSubtable.is_null ());
    بازگشت *این;
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  ثابت IndexSubtable* get_subtable (ثابت تهی *base) ثابت
  {
    بازگشت &(base+offsetToSubtable);
  }

  منطقی add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 ثابت hb_vector_t<hb_pair_t<hb_codepoint_t, ثابت IndexSubtableRecord*>> *lookup, /* IN */
			 ثابت تهی *base,
			 unsigned صحیح *start /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (این);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    اگر (unlikely (!c->serializer->extend_min (subtable))) return_trace (نادرست);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned صحیح image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<نویسه> data;
    hb_vector_t<unsigned صحیح> lengths;
    unsigned صحیح رقم_glyphs = 0;
    منطقی early_exit = نادرست;
    برای (unsigned صحیح i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      ثابت IndexSubtableRecord *next_record = (*lookup)[i].second;
      ثابت IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      اگر (next_header != old_header)
      {
	*start = i;
	early_exit = درست;
	بشکن;
      }
      unsigned صحیح رقم_missing = record->add_glyph_for_subset (new_gid);
      برای (unsigned صحیح j = 0; j < رقم_missing; j++)
	اگر (unlikely (!c->serializer->check_success (lengths.افزودن_or_fail (0))))
	  return_trace (نادرست);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      اگر (old_gid < next_record->firstGlyphIndex)
	return_trace (نادرست);

      unsigned صحیح old_idx = (unsigned صحیح) old_gid - next_record->firstGlyphIndex;
      unsigned صحیح offset, length, format;
      اگر (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (نادرست);
      اگر (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (نادرست);
      اگر (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (نادرست);
      اگر (unlikely (!c->serializer->check_success (lengths.افزودن_or_fail (length))))
	return_trace (نادرست);
      رقم_glyphs += 1 + رقم_missing;
    }
    اگر (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned صحیح image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    منطقی shared = نادرست;
    uint64_t key = 0;
    اگر (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_اول,
			    &image_data_offset);
    }
    اگر (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_اول->length;
      اگر (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_اول,
					    data.arrayZ, data.length))))
	return_trace (نادرست);
      اگر (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(تهی) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned صحیح cumulative = 0;
    برای (unsigned صحیح length : lengths)
    {
      اگر (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (نادرست);
      cumulative += length;
    }
    اگر (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      رقم_glyphs,
					      &bitmap_size_context->size)))
      return_trace (نادرست);
    return_trace (درست);
  }

  منطقی add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       ثابت hb_vector_t<hb_pair_t<hb_codepoint_t, ثابت IndexSubtableRecord*>> *lookup, /* IN */
		       ثابت تهی *base,
		       unsigned صحیح *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) ثابت
  {
    TRACE_SERIALIZE (این);
    auto snap = c->serializer->snapshot ();
    unsigned صحیح old_size = bitmap_size_context->size;
    unsigned صحیح old_cbdt_اول_length = bitmap_size_context->cbdt_اول->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    اگر (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (نادرست);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    اگر (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->برداشت_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_اول->shrink (old_cbdt_اول_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (نادرست);
    }

    bitmap_size_context->رقم_tables += 1;
    return_trace (درست);
  }

  unsigned صحیح add_glyph_for_subset (hb_codepoint_t gid)
  {
    اگر (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      بازگشت 0;
    }
    // TODO maybe assert? this shouldn't occur.
    اگر (lastGlyphIndex > gid)
      بازگشت 0;
    unsigned صحیح رقم_missing = (unsigned صحیح) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    بازگشت رقم_missing;
  }

  منطقی get_extents (hb_glyph_extents_t *extents, ثابت تهی *base, منطقی scale) ثابت
  { بازگشت (base+offsetToSubtable).get_extents (extents, scale); }

  منطقی get_image_data (unsigned صحیح  gid,
		       ثابت تهی   *base,
		       unsigned صحیح *offset,
		       unsigned صحیح *length,
		       unsigned صحیح *format) ثابت
  {
    اگر (gid < firstGlyphIndex || gid > lastGlyphIndex) بازگشت نادرست;
    بازگشت (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار IndexSubtableArray
{
  friend ساختار CBDT;

  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح شمار) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (indexSubtablesZ.sanitize (c, شمار, این));
  }

  تهی
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		ثابت IndexSubtableRecord*>> *lookup /* OUT */) ثابت
  {
    منطقی start_glyph_is_set = نادرست;
    unsigned رقم_glyphs = c->plan->رقم_output_glyphs ();
    برای (hb_codepoint_t new_gid = 0; new_gid < رقم_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      اگر (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) ادامه;

      ثابت IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->رقم_tables);
      اگر (unlikely (!record)) ادامه;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned صحیح offset, length, format;
      اگر (unlikely (!record->get_image_data (old_gid, این, &offset, &length, &format))) ادامه;

      lookup->push (hb_pair_t<hb_codepoint_t, ثابت IndexSubtableRecord*> (new_gid, record));

      اگر (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = درست;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  منطقی
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) ثابت
  {
    TRACE_SUBSET (این);

    hb_vector_t<hb_pair_t<hb_codepoint_t, ثابت IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    اگر (unlikely (!c->serializer->propagate_error (lookup)))
      بازگشت نادرست;

    bitmap_size_context->size = 0;
    bitmap_size_context->رقم_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    برای (unsigned صحیح start = 0; start < lookup.length;)
    {
      اگر (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, این, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	برای (unsigned صحیح i = 0; i < records.length; i++)
	  c->serializer->برداشت_discard ();
	return_trace (نادرست);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    برای (unsigned صحیح i = 0; i < records.length; i++)
      objidxs.push (c->serializer->برداشت_pack ());
    برای (unsigned صحیح i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      اگر (unlikely (!record)) return_trace (نادرست);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (درست);
  }

  عمومی:
  ثابت IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned صحیح رقم_Tables) ثابت
  {
    برای (unsigned صحیح i = 0; i < رقم_Tables; ++i)
    {
      unsigned صحیح firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned صحیح lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      اگر (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	بازگشت &indexSubtablesZ[i];
    }
    بازگشت nullptr;
  }

  محافظتشده:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

ساختار BitmapSizeTable
{
  friend ساختار CBLC;
  friend ساختار CBDT;

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, عدد_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ثابت IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, ثابت تهی *base, ثابت تهی **out_base) ثابت
  {
    *out_base = &(base+indexSubtableArrayOffset);
    بازگشت (base+indexSubtableArrayOffset).find_table (glyph, عدد_Of_Index_Subtables);
  }

  منطقی
  subset (hb_subset_context_t *c, ثابت تهی *base,
	  ثابت نویسه *cbdt, unsigned صحیح cbdt_length,
	  hb_vector_t<نویسه> *cbdt_اول /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (این);
    auto *out_table = c->serializer->embed (این);
    اگر (unlikely (!out_table)) return_trace (نادرست);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_اول = cbdt_اول;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.رقم_tables = عدد_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    اگر (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (نادرست);
    اگر (!bitmap_size_context.size ||
	!bitmap_size_context.رقم_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (نادرست);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->عدد_Of_Index_Subtables = bitmap_size_context.رقم_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (درست);
  }

  محافظتشده:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		عدد_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_سطر_Metrics	horizontal;
  SBit_سطر_Metrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  عمومی:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

ساختار GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  عمومی:
  DEFINE_SIZE_ARRAY (9, data);
};

ساختار GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  عمومی:
  DEFINE_SIZE_ARRAY (12, data);
};

ساختار GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  عمومی:
  DEFINE_SIZE_ARRAY (4, data);
};

ساختار CBLC
{
  friend ساختار CBDT;

  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, این));
  }

  ایستا منطقی
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<نویسه>* cbdt_اول)
  {
    hb_blob_t *cbdt_اول_blob = hb_blob_create (cbdt_اول->arrayZ,
						 cbdt_اول->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_اول->arrayZ,
						 hb_free);
    cbdt_اول->init ();  // Leak arrayZ to the blob.
    منطقی ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_اول_blob);
    hb_blob_destroy (cbdt_اول_blob);
    بازگشت ret;
  }

  منطقی
  subset_size_table (hb_subset_context_t *c, ثابت BitmapSizeTable& table,
		     ثابت نویسه *cbdt /* IN */, unsigned صحیح cbdt_length,
		     CBLC *cblc_اول /* INOUT */, hb_vector_t<نویسه> *cbdt_اول /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (این);
    cblc_اول->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_اول_len = cbdt_اول->length;

    اگر (!table.subset (c, این, cbdt, cbdt_length, cbdt_اول, dedup))
    {
      cblc_اول->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_اول->shrink (cbdt_اول_len);
      return_trace (نادرست);
    }
    return_trace (درست);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL منطقی subset (hb_subset_context_t *c) ثابت;

  محافظتشده:
  ثابت BitmapSizeTable &choose_strike (hb_font_t *font) ثابت
  {
    unsigned شمار = sizeTables.len;
    اگر (unlikely (!شمار))
      بازگشت Null (BitmapSizeTable);

    unsigned صحیح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    اگر (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned صحیح best_i = 0;
    unsigned صحیح best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    برای (unsigned صحیح i = 1; i < شمار; i++)
    {
      unsigned صحیح ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      اگر ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    بازگشت sizeTables[best_i];
  }

  محافظتشده:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  عمومی:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

ساختار CBDT
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      این->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      این->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      این->cblc.destroy ();
      این->cbdt.destroy ();
    }

    منطقی
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, منطقی scale = درست) ثابت
    {
      ثابت تهی *base;
      ثابت BitmapSizeTable &strike = این->cblc->choose_strike (font);
      ثابت IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      اگر (!subtable_record || !strike.ppemX || !strike.ppemY)
	بازگشت نادرست;

      اگر (subtable_record->get_extents (extents, base, scale))
	بازگشت درست;

      unsigned صحیح image_offset = 0, image_length = 0, image_format = 0;
      اگر (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	بازگشت نادرست;

      unsigned صحیح cbdt_len = cbdt.get_length ();
      اگر (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	بازگشت نادرست;

      انتخاب (image_format)
      {
      حالت 17: {
	اگر (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  بازگشت نادرست;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (این->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	بشکن;
      }
      حالت 18: {
	اگر (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  بازگشت نادرست;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (این->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	بشکن;
      }
      پیشفرض: بازگشت نادرست; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      اگر (scale)
      {
	اعشاری x_scale = upem / (اعشاری) strike.ppemX;
	اعشاری y_scale = upem / (اعشاری) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      بازگشت درست;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ثابت
    {
      ثابت تهی *base;
      ثابت BitmapSizeTable &strike = این->cblc->choose_strike (font);
      ثابت IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      اگر (!subtable_record || !strike.ppemX || !strike.ppemY)
	بازگشت hb_blob_get_empty ();

      unsigned صحیح image_offset = 0, image_length = 0, image_format = 0;
      اگر (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	بازگشت hb_blob_get_empty ();

      unsigned صحیح cbdt_len = cbdt.get_length ();
      اگر (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	بازگشت hb_blob_get_empty ();

      انتخاب (image_format)
      {
      حالت 17:
      {
	اگر (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  بازگشت hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (این->cbdt, image_offset);
	بازگشت hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      حالت 18:
      {
	اگر (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  بازگشت hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (این->cbdt, image_offset);
	بازگشت hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      حالت 19:
      {
	اگر (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  بازگشت hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (این->cbdt, image_offset);
	بازگشت hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      پیشفرض: بازگشت hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    منطقی has_data () ثابت { بازگشت cbdt->version.major; }

    منطقی paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, تهی *data) ثابت
    {
      اگر (!has_data ()) بازگشت نادرست;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      اگر (unlikely (!font->get_glyph_extents (glyph, &extents, نادرست)))
        بازگشت نادرست;

      اگر (unlikely (!get_extents (font, glyph, &pixel_extents, نادرست)))
        بازگشت نادرست;

      hb_blob_t *blob = reference_png (font, glyph);
      اگر (unlikely (hb_blob_is_immutable (blob)))
        بازگشت نادرست;

      منطقی ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      بازگشت ret;
    }

    خصوصی:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned صحیح upem;
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  محافظتشده:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  عمومی:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

درونخطی منطقی
CBLC::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (این);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<نویسه> cbdt_اول;

  auto *cblc_اول = c->serializer->start_embed<CBLC> ();
  اگر (unlikely (!c->serializer->extend_min (cblc_اول))) return_trace (نادرست);
  cblc_اول->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned صحیح cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  اگر (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (نادرست);
  }
  _copy_data_to_cbdt (&cbdt_اول, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  برای (ثابت BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (ثابت نویسه *) cbdt, cbdt_length, cblc_اول, &cbdt_اول, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_اول));
}

ساختار CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
