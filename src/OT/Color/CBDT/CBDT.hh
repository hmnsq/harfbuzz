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
    أرجع ((uint64_t) image_format << 32) | h;
  }

  منطقي find (uint64_t key,
	     ثابت hb_vector_t<حرف> &data,
	     ثابت hb_vector_t<unsigned صحيح> &lengths,
	     ثابت hb_vector_t<حرف> &cbdt_ڤردان,
	     unsigned صحيح *base /* OUT */) ثابت
  {
    unsigned صحيح *idx_p = nullptr;
    إذا (!map.has (key, &idx_p)) أرجع خطأ;
    ثابت entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    إذا (e.lcount != lengths.length) أرجع خطأ;
    إذا (unlikely (e.base + data.length > (unsigned صحيح) cbdt_ڤردان.length)) أرجع خطأ;
    إذا (hb_memcmp (cbdt_ڤردان.arrayZ + e.base, data.arrayZ, data.length)) أرجع خطأ;
    إذا (hb_memcmp (هذا->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * حجم (unsigned صحيح)))
      أرجع خطأ;
    *base = e.base;
    أرجع true;
  }

  منطقي add (uint64_t key,
	    unsigned صحيح base,
	    ثابت hb_vector_t<unsigned صحيح> &lengths)
  {
    إذا (unlikely (entries.in_error () || هذا->lengths.in_error () || map.in_error ()))
      أرجع خطأ;
    entry_t e = {base, هذا->lengths.length, lengths.length};
    إذا (unlikely (!entries.تولق_or_fail (e))) أرجع خطأ;
    لكل (unsigned صحيح length : lengths)
      إذا (unlikely (!هذا->lengths.تولق_or_fail (length))) أرجع خطأ;
    أرجع map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned صحيح> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned صحيح> lengths; /* Concatenated per-entry lengths. */
};

بنية cblc_bitmap_size_subset_context_t
{
  ثابت حرف *cbdt;
  unsigned صحيح cbdt_length;
  hb_vector_t<حرف> *cbdt_ڤردان;
  unsigned صحيح size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned صحيح اڠک_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ساكن مضمن منطقي
_copy_data_to_cbdt (hb_vector_t<حرف> *cbdt_ڤردان,
		    ثابت فراغ        *data,
		    unsigned           length)
{
  unsigned صحيح new_len = cbdt_ڤردان->length + length;
  إذا (unlikely (!cbdt_ڤردان->alloc (new_len))) أرجع خطأ;
  hb_memcpy (cbdt_ڤردان->arrayZ + cbdt_ڤردان->length, data, length);
  cbdt_ڤردان->length = new_len;
  أرجع true;
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

    إذا (scale)
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

بنية SBit_باريس_Metrics
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
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح glyph_کيراءن) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  offsetArrayZ.sanitize (c, glyph_کيراءن + 1));
  }

  منطقي get_image_data (unsigned صحيح idx,
		       unsigned صحيح *offset,
		       unsigned صحيح *length) ثابت
  {
    إذا (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      أرجع خطأ;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    أرجع true;
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
  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح glyph_کيراءن) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.header.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.header.indexFormat)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_کيراءن));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_کيراءن));
    افتراضي:return_trace (true);
    }
  }

  منطقي
  finish_subtable (hb_serialize_context_t *c,
		   unsigned صحيح end_offset,
		   unsigned صحيح اڠک_glyphs,
		   unsigned صحيح *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (هذا);

    بدل (u.header.indexFormat)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    حالة 3: {
      hb_barrier ();
      إذا (!u.format3.add_offset (c, end_offset, size))
	return_trace (خطأ);
      إذا (!(اڠک_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (true);
    }
    // TODO: implement 2, 4, 5.
    حالة 2: حالة 4:  // No-op.
    حالة 5:  // Pad to 32-bit aligned.
    افتراضي: return_trace (خطأ);
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
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي get_extents (hb_glyph_extents_t *extents HB_UNUSED, منطقي scale HB_UNUSED) ثابت
  {
    بدل (u.header.indexFormat)
    {
    حالة 2: حالة 5: /* TODO */
    حالة 1: حالة 3: حالة 4: /* Variable-metrics formats do not have metrics here. */
    افتراضي:أرجع (خطأ);
    }
  }

  منطقي
  get_image_data (unsigned صحيح idx, unsigned صحيح *offset,
		  unsigned صحيح *length, unsigned صحيح *format) ثابت
  {
    *format = u.header.imageFormat;
    بدل (u.header.indexFormat)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_image_data (idx, offset, length);
    حالة 3: hb_barrier (); أرجع u.format3.get_image_data (idx, offset, length);
    افتراضي: أرجع خطأ;
    }
  }

  ثابت IndexSubtableHeader* get_header () ثابت { أرجع &u.header; }

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
    أرجع *هذا;
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
    أرجع &(base+offsetToSubtable);
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
    إذا (unlikely (!c->serializer->extend_min (subtable))) return_trace (خطأ);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned صحيح image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<حرف> data;
    hb_vector_t<unsigned صحيح> lengths;
    unsigned صحيح اڠک_glyphs = 0;
    منطقي early_exit = خطأ;
    لكل (unsigned صحيح i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      ثابت IndexSubtableRecord *next_record = (*lookup)[i].second;
      ثابت IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      إذا (next_header != old_header)
      {
	*start = i;
	early_exit = true;
	اكسر;
      }
      unsigned صحيح اڠک_missing = record->add_glyph_for_subset (new_gid);
      لكل (unsigned صحيح j = 0; j < اڠک_missing; j++)
	إذا (unlikely (!c->serializer->check_success (lengths.تولق_or_fail (0))))
	  return_trace (خطأ);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      إذا (old_gid < next_record->firstGlyphIndex)
	return_trace (خطأ);

      unsigned صحيح old_idx = (unsigned صحيح) old_gid - next_record->firstGlyphIndex;
      unsigned صحيح offset, length, format;
      إذا (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (خطأ);
      إذا (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (خطأ);
      إذا (unlikely (!c->serializer->check_success (lengths.تولق_or_fail (length))))
	return_trace (خطأ);
      اڠک_glyphs += 1 + اڠک_missing;
    }
    إذا (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned صحيح image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    منطقي shared = خطأ;
    uint64_t key = 0;
    إذا (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_ڤردان,
			    &image_data_offset);
    }
    إذا (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_ڤردان->length;
      إذا (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_ڤردان,
					    data.arrayZ, data.length))))
	return_trace (خطأ);
      إذا (dedup && data.length)
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
    لكل (unsigned صحيح length : lengths)
    {
      إذا (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (خطأ);
      cumulative += length;
    }
    إذا (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      اڠک_glyphs,
					      &bitmap_size_context->size)))
      return_trace (خطأ);
    return_trace (true);
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
    unsigned صحيح old_cbdt_ڤردان_length = bitmap_size_context->cbdt_ڤردان->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    إذا (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (خطأ);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    إذا (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->کلوار_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_ڤردان->shrink (old_cbdt_ڤردان_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (خطأ);
    }

    bitmap_size_context->اڠک_tables += 1;
    return_trace (true);
  }

  unsigned صحيح add_glyph_for_subset (hb_codepoint_t gid)
  {
    إذا (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      أرجع 0;
    }
    // TODO maybe assert? this shouldn't occur.
    إذا (lastGlyphIndex > gid)
      أرجع 0;
    unsigned صحيح اڠک_missing = (unsigned صحيح) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    أرجع اڠک_missing;
  }

  منطقي get_extents (hb_glyph_extents_t *extents, ثابت فراغ *base, منطقي scale) ثابت
  { أرجع (base+offsetToSubtable).get_extents (extents, scale); }

  منطقي get_image_data (unsigned صحيح  gid,
		       ثابت فراغ   *base,
		       unsigned صحيح *offset,
		       unsigned صحيح *length,
		       unsigned صحيح *format) ثابت
  {
    إذا (gid < firstGlyphIndex || gid > lastGlyphIndex) أرجع خطأ;
    أرجع (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
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

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح کيراءن) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (indexSubtablesZ.sanitize (c, کيراءن, هذا));
  }

  فراغ
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		ثابت IndexSubtableRecord*>> *lookup /* OUT */) ثابت
  {
    منطقي start_glyph_is_set = خطأ;
    unsigned اڠک_glyphs = c->plan->اڠک_output_glyphs ();
    لكل (hb_codepoint_t new_gid = 0; new_gid < اڠک_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) استمر;

      ثابت IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->اڠک_tables);
      إذا (unlikely (!record)) استمر;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned صحيح offset, length, format;
      إذا (unlikely (!record->get_image_data (old_gid, هذا, &offset, &length, &format))) استمر;

      lookup->push (hb_pair_t<hb_codepoint_t, ثابت IndexSubtableRecord*> (new_gid, record));

      إذا (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = true;
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
    إذا (unlikely (!c->serializer->propagate_error (lookup)))
      أرجع خطأ;

    bitmap_size_context->size = 0;
    bitmap_size_context->اڠک_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    لكل (unsigned صحيح start = 0; start < lookup.length;)
    {
      إذا (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, هذا, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	لكل (unsigned صحيح i = 0; i < records.length; i++)
	  c->serializer->کلوار_discard ();
	return_trace (خطأ);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (unsigned صحيح i = 0; i < records.length; i++)
      objidxs.push (c->serializer->کلوار_pack ());
    لكل (unsigned صحيح i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      إذا (unlikely (!record)) return_trace (خطأ);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (true);
  }

  عام:
  ثابت IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned صحيح اڠک_Tables) ثابت
  {
    لكل (unsigned صحيح i = 0; i < اڠک_Tables; ++i)
    {
      unsigned صحيح firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned صحيح lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      إذا (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	أرجع &indexSubtablesZ[i];
    }
    أرجع nullptr;
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
		  indexSubtableArrayOffset.sanitize (c, base, نومبور_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ثابت IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, ثابت فراغ *base, ثابت فراغ **out_base) ثابت
  {
    *out_base = &(base+indexSubtableArrayOffset);
    أرجع (base+indexSubtableArrayOffset).find_table (glyph, نومبور_Of_Index_Subtables);
  }

  منطقي
  subset (hb_subset_context_t *c, ثابت فراغ *base,
	  ثابت حرف *cbdt, unsigned صحيح cbdt_length,
	  hb_vector_t<حرف> *cbdt_ڤردان /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out_table = c->serializer->embed (هذا);
    إذا (unlikely (!out_table)) return_trace (خطأ);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_ڤردان = cbdt_ڤردان;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.اڠک_tables = نومبور_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    إذا (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (خطأ);
    إذا (!bitmap_size_context.size ||
	!bitmap_size_context.اڠک_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (خطأ);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->نومبور_Of_Index_Subtables = bitmap_size_context.اڠک_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (true);
  }

  محمي:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		نومبور_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_باريس_Metrics	horizontal;
  SBit_باريس_Metrics	vertical;
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
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<حرف>* cbdt_ڤردان)
  {
    hb_blob_t *cbdt_ڤردان_blob = hb_blob_create (cbdt_ڤردان->arrayZ,
						 cbdt_ڤردان->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_ڤردان->arrayZ,
						 hb_free);
    cbdt_ڤردان->init ();  // Leak arrayZ to the blob.
    منطقي ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_ڤردان_blob);
    hb_blob_destroy (cbdt_ڤردان_blob);
    أرجع ret;
  }

  منطقي
  subset_size_table (hb_subset_context_t *c, ثابت BitmapSizeTable& table,
		     ثابت حرف *cbdt /* IN */, unsigned صحيح cbdt_length,
		     CBLC *cblc_ڤردان /* INOUT */, hb_vector_t<حرف> *cbdt_ڤردان /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    cblc_ڤردان->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_ڤردان_len = cbdt_ڤردان->length;

    إذا (!table.subset (c, هذا, cbdt, cbdt_length, cbdt_ڤردان, dedup))
    {
      cblc_ڤردان->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_ڤردان->shrink (cbdt_ڤردان_len);
      return_trace (خطأ);
    }
    return_trace (true);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL منطقي subset (hb_subset_context_t *c) ثابت;

  محمي:
  ثابت BitmapSizeTable &choose_strike (hb_font_t *font) ثابت
  {
    unsigned کيراءن = sizeTables.len;
    إذا (unlikely (!کيراءن))
      أرجع Null (BitmapSizeTable);

    unsigned صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    إذا (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned صحيح best_i = 0;
    unsigned صحيح best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    لكل (unsigned صحيح i = 1; i < کيراءن; i++)
    {
      unsigned صحيح ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      إذا ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    أرجع sizeTables[best_i];
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
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, منطقي scale = true) ثابت
    {
      ثابت فراغ *base;
      ثابت BitmapSizeTable &strike = هذا->cblc->choose_strike (font);
      ثابت IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      إذا (!subtable_record || !strike.ppemX || !strike.ppemY)
	أرجع خطأ;

      إذا (subtable_record->get_extents (extents, base, scale))
	أرجع true;

      unsigned صحيح image_offset = 0, image_length = 0, image_format = 0;
      إذا (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	أرجع خطأ;

      unsigned صحيح cbdt_len = cbdt.get_length ();
      إذا (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	أرجع خطأ;

      بدل (image_format)
      {
      حالة 17: {
	إذا (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  أرجع خطأ;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (هذا->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	اكسر;
      }
      حالة 18: {
	إذا (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  أرجع خطأ;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (هذا->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	اكسر;
      }
      افتراضي: أرجع خطأ; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      إذا (scale)
      {
	عائم x_scale = upem / (عائم) strike.ppemX;
	عائم y_scale = upem / (عائم) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      أرجع true;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ثابت
    {
      ثابت فراغ *base;
      ثابت BitmapSizeTable &strike = هذا->cblc->choose_strike (font);
      ثابت IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      إذا (!subtable_record || !strike.ppemX || !strike.ppemY)
	أرجع hb_blob_get_empty ();

      unsigned صحيح image_offset = 0, image_length = 0, image_format = 0;
      إذا (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	أرجع hb_blob_get_empty ();

      unsigned صحيح cbdt_len = cbdt.get_length ();
      إذا (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	أرجع hb_blob_get_empty ();

      بدل (image_format)
      {
      حالة 17:
      {
	إذا (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  أرجع hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (هذا->cbdt, image_offset);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      حالة 18:
      {
	إذا (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  أرجع hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (هذا->cbdt, image_offset);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      حالة 19:
      {
	إذا (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  أرجع hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (هذا->cbdt, image_offset);
	أرجع hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      افتراضي: أرجع hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    منطقي has_data () ثابت { أرجع cbdt->version.major; }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *data) ثابت
    {
      إذا (!has_data ()) أرجع خطأ;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      إذا (unlikely (!font->get_glyph_extents (glyph, &extents, خطأ)))
        أرجع خطأ;

      إذا (unlikely (!get_extents (font, glyph, &pixel_extents, خطأ)))
        أرجع خطأ;

      hb_blob_t *blob = reference_png (font, glyph);
      إذا (unlikely (hb_blob_is_immutable (blob)))
        أرجع خطأ;

      منطقي ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
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
  hb_vector_t<حرف> cbdt_ڤردان;

  auto *cblc_ڤردان = c->serializer->start_embed<CBLC> ();
  إذا (unlikely (!c->serializer->extend_min (cblc_ڤردان))) return_trace (خطأ);
  cblc_ڤردان->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned صحيح cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  إذا (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (خطأ);
  }
  _copy_data_to_cbdt (&cbdt_ڤردان, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  لكل (ثابت BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (ثابت حرف *) cbdt, cbdt_length, cblc_ڤردان, &cbdt_ڤردان, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_ڤردان));
}

بنية CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
