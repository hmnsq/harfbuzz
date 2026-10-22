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

#ଅନ୍ତର୍ଭାଵ "../../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-paint.hh"

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


ନାମାକାଶ OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
ସଂରଚନା cbdt_dedup_context_t
{
  ସଂରଚନା entry_t
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା base;		/* Offset of the data in cbdt_prime. */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lstart;	/* Start index in lengths. */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lcount;	/* Number of length entries. */
  };

  ସ୍ଥିର uint64_t hash (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା image_format,
			ଅଚଳ hb_vector_t<ଅକ୍ଷର> &data,
			ଅଚଳ hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((ଅଚଳ ଅକ୍ଷର *) lengths.arrayZ,
				      lengths.length * ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)));
    ଫେରନ୍ତୁ ((uint64_t) image_format << 32) | h;
  }

  ତାର୍କିକ find (uint64_t key,
	     ଅଚଳ hb_vector_t<ଅକ୍ଷର> &data,
	     ଅଚଳ hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &lengths,
	     ଅଚଳ hb_vector_t<ଅକ୍ଷର> &cbdt_ମୌଳିକ,
	     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *base /* OUT */) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *idx_p = nullptr;
    ଯଦି (!map.has (key, &idx_p)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    ଯଦି (e.lcount != lengths.length) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (unlikely (e.base + data.length > (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) cbdt_ମୌଳିକ.length)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (hb_memcmp (cbdt_ମୌଳିକ.arrayZ + e.base, data.arrayZ, data.length)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (hb_memcmp (ଏହି->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * ଆକାର (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *base = e.base;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ add (uint64_t key,
	    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା base,
	    ଅଚଳ hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &lengths)
  {
    ଯଦି (unlikely (entries.in_error () || ଏହି->lengths.in_error () || map.in_error ()))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    entry_t e = {base, ଏହି->lengths.length, lengths.length};
    ଯଦି (unlikely (!entries.ଠେଲ_or_fail (e))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length : lengths)
      ଯଦି (unlikely (!ଏହି->lengths.ଠେଲ_or_fail (length))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> lengths; /* Concatenated per-entry lengths. */
};

ସଂରଚନା cblc_bitmap_size_subset_context_t
{
  ଅଚଳ ଅକ୍ଷର *cbdt;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cbdt_length;
  hb_vector_t<ଅକ୍ଷର> *cbdt_ମୌଳିକ;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
_copy_data_to_cbdt (hb_vector_t<ଅକ୍ଷର> *cbdt_ମୌଳିକ,
		    ଅଚଳ ଶୂନ୍ୟ        *data,
		    unsigned           length)
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_len = cbdt_ମୌଳିକ->length + length;
  ଯଦି (unlikely (!cbdt_ମୌଳିକ->alloc (new_len))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  hb_memcpy (cbdt_ମୌଳିକ->arrayZ + cbdt_ମୌଳିକ->length, data, length);
  cbdt_ମୌଳିକ->length = new_len;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସଂରଚନା SmallGlyphMetrics
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, ତାର୍କିକ scale) ଅଚଳ
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (height);

    ଯଦି (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (5);
};

ସଂରଚନା BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା SBit_ଧାଡ଼ି_Metrics
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

ସଂରଚନା IndexSubtableHeader
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
ସଂରଚନା IndexSubtableFormat1Or3
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_ଗଣନା) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  offsetArrayZ.sanitize (c, glyph_ଗଣନା + 1));
  }

  ତାର୍କିକ get_image_data (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *offset,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *length) ଅଚଳ
  {
    ଯଦି (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ add_offset (hb_serialize_context_t *c,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ଏହି);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += ଆକାର (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((ତାର୍କିକ) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

ସଂରଚନା IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
ସଂରଚନା IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

ସଂରଚନା IndexSubtable
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_ଗଣନା) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.header.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.header.indexFormat)
    {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_ଗଣନା));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_ଗଣନା));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  ତାର୍କିକ
  finish_subtable (hb_serialize_context_t *c,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end_offset,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ଏହି);

    ବାଛ (u.header.indexFormat)
    {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    ସ୍ଥିତି 3: {
      hb_barrier ();
      ଯଦି (!u.format3.add_offset (c, end_offset, size))
	return_trace (ମିଥ୍ୟା);
      ଯଦି (!(ଅଙ୍କ_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (ସତ୍ୟ);
    }
    // TODO: implement 2, 4, 5.
    ସ୍ଥିତି 2: ସ୍ଥିତି 4:  // No-op.
    ସ୍ଥିତି 5:  // Pad to 32-bit aligned.
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: return_trace (ମିଥ୍ୟା);
    }
  }

  ତାର୍କିକ
  add_offset (hb_serialize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା local_offset,
	      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ଏହି);
    ବାଛ (u.header.indexFormat)
    {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    ସ୍ଥିତି 2:  // Should be a no-op.
    ସ୍ଥିତି 4: ସ୍ଥିତି 5:  // Handle sparse cases.
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: return_trace (ମିଥ୍ୟା);
    }
  }

  ତାର୍କିକ get_extents (hb_glyph_extents_t *extents HB_UNUSED, ତାର୍କିକ scale HB_UNUSED) ଅଚଳ
  {
    ବାଛ (u.header.indexFormat)
    {
    ସ୍ଥିତି 2: ସ୍ଥିତି 5: /* TODO */
    ସ୍ଥିତି 1: ସ୍ଥିତି 3: ସ୍ଥିତି 4: /* Variable-metrics formats do not have metrics here. */
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ (ମିଥ୍ୟା);
    }
  }

  ତାର୍କିକ
  get_image_data (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *offset,
		  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *length, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *format) ଅଚଳ
  {
    *format = u.header.imageFormat;
    ବାଛ (u.header.indexFormat)
    {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_image_data (idx, offset, length);
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_image_data (idx, offset, length);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ଅଚଳ IndexSubtableHeader* get_header () ଅଚଳ { ଫେରନ୍ତୁ &u.header; }

  ଶୂନ୍ୟ populate_header (unsigned index_format,
			unsigned image_format,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା image_data_offset,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    ବାଛ (u.header.indexFormat)
    {
    ସ୍ଥିତି 1: *size += IndexSubtableFormat1::min_size; ଭାଙ୍ଗ;
    ସ୍ଥିତି 3: *size += IndexSubtableFormat3::min_size; ଭାଙ୍ଗ;
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (8, header);
};

ସଂରଚନା IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (ଅଚଳ IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    ପ୍ରତିଜ୍ଞା (offsetToSubtable.is_null ());
    ଫେରନ୍ତୁ *ଏହି;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  ଅଚଳ IndexSubtable* get_subtable (ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଫେରନ୍ତୁ &(base+offsetToSubtable);
  }

  ତାର୍କିକ add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 ଅଚଳ hb_vector_t<hb_pair_t<hb_codepoint_t, ଅଚଳ IndexSubtableRecord*>> *lookup, /* IN */
			 ଅଚଳ ଶୂନ୍ୟ *base,
			 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *start /* INOUT */) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    ଯଦି (unlikely (!c->serializer->extend_min (subtable))) return_trace (ମିଥ୍ୟା);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<ଅକ୍ଷର> data;
    hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> lengths;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs = 0;
    ତାର୍କିକ early_exit = ମିଥ୍ୟା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      ଅଚଳ IndexSubtableRecord *next_record = (*lookup)[i].second;
      ଅଚଳ IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      ଯଦି (next_header != old_header)
      {
	*start = i;
	early_exit = ସତ୍ୟ;
	ଭାଙ୍ଗ;
      }
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_missing = record->add_glyph_for_subset (new_gid);
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < ଅଙ୍କ_missing; j++)
	ଯଦି (unlikely (!c->serializer->check_success (lengths.ଠେଲ_or_fail (0))))
	  return_trace (ମିଥ୍ୟା);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      ଯଦି (old_gid < next_record->firstGlyphIndex)
	return_trace (ମିଥ୍ୟା);

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_idx = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) old_gid - next_record->firstGlyphIndex;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset, length, format;
      ଯଦି (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (ମିଥ୍ୟା);
      ଯଦି (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (ମିଥ୍ୟା);
      ଯଦି (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (ମିଥ୍ୟା);
      ଯଦି (unlikely (!c->serializer->check_success (lengths.ଠେଲ_or_fail (length))))
	return_trace (ମିଥ୍ୟା);
      ଅଙ୍କ_glyphs += 1 + ଅଙ୍କ_missing;
    }
    ଯଦି (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    ତାର୍କିକ shared = ମିଥ୍ୟା;
    uint64_t key = 0;
    ଯଦି (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_ମୌଳିକ,
			    &image_data_offset);
    }
    ଯଦି (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_ମୌଳିକ->length;
      ଯଦି (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_ମୌଳିକ,
					    data.arrayZ, data.length))))
	return_trace (ମିଥ୍ୟା);
      ଯଦି (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(ଶୂନ୍ୟ) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cumulative = 0;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length : lengths)
    {
      ଯଦି (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (ମିଥ୍ୟା);
      cumulative += length;
    }
    ଯଦି (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      ଅଙ୍କ_glyphs,
					      &bitmap_size_context->size)))
      return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       ଅଚଳ hb_vector_t<hb_pair_t<hb_codepoint_t, ଅଚଳ IndexSubtableRecord*>> *lookup, /* IN */
		       ଅଚଳ ଶୂନ୍ୟ *base,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto snap = c->serializer->snapshot ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_size = bitmap_size_context->size;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_cbdt_ମୌଳିକ_length = bitmap_size_context->cbdt_ମୌଳିକ->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    ଯଦି (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (ମିଥ୍ୟା);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    ଯଦି (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->କାଢ଼_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_ମୌଳିକ->shrink (old_cbdt_ମୌଳିକ_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (ମିଥ୍ୟା);
    }

    bitmap_size_context->ଅଙ୍କ_tables += 1;
    return_trace (ସତ୍ୟ);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_glyph_for_subset (hb_codepoint_t gid)
  {
    ଯଦି (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      ଫେରନ୍ତୁ 0;
    }
    // TODO maybe assert? this shouldn't occur.
    ଯଦି (lastGlyphIndex > gid)
      ଫେରନ୍ତୁ 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_missing = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    ଫେରନ୍ତୁ ଅଙ୍କ_missing;
  }

  ତାର୍କିକ get_extents (hb_glyph_extents_t *extents, ଅଚଳ ଶୂନ୍ୟ *base, ତାର୍କିକ scale) ଅଚଳ
  { ଫେରନ୍ତୁ (base+offsetToSubtable).get_extents (extents, scale); }

  ତାର୍କିକ get_image_data (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  gid,
		       ଅଚଳ ଶୂନ୍ୟ   *base,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *offset,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *length,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *format) ଅଚଳ
  {
    ଯଦି (gid < firstGlyphIndex || gid > lastGlyphIndex) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା IndexSubtableArray
{
  friend ସଂରଚନା CBDT;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (indexSubtablesZ.sanitize (c, ଗଣନା_2, ଏହି));
  }

  ଶୂନ୍ୟ
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		ଅଚଳ IndexSubtableRecord*>> *lookup /* OUT */) ଅଚଳ
  {
    ତାର୍କିକ start_glyph_is_set = ମିଥ୍ୟା;
    unsigned ଅଙ୍କ_glyphs = c->plan->ଅଙ୍କ_output_glyphs ();
    ପାଇଁ (hb_codepoint_t new_gid = 0; new_gid < ଅଙ୍କ_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      ଯଦି (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) ଜାରି;

      ଅଚଳ IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->ଅଙ୍କ_tables);
      ଯଦି (unlikely (!record)) ଜାରି;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset, length, format;
      ଯଦି (unlikely (!record->get_image_data (old_gid, ଏହି, &offset, &length, &format))) ଜାରି;

      lookup->push (hb_pair_t<hb_codepoint_t, ଅଚଳ IndexSubtableRecord*> (new_gid, record));

      ଯଦି (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = ସତ୍ୟ;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  ତାର୍କିକ
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    hb_vector_t<hb_pair_t<hb_codepoint_t, ଅଚଳ IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    ଯଦି (unlikely (!c->serializer->propagate_error (lookup)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    bitmap_size_context->size = 0;
    bitmap_size_context->ଅଙ୍କ_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = 0; start < lookup.length;)
    {
      ଯଦି (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, ଏହି, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < records.length; i++)
	  c->serializer->କାଢ଼_discard ();
	return_trace (ମିଥ୍ୟା);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < records.length; i++)
      objidxs.push (c->serializer->କାଢ଼_pack ());
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      ଯଦି (unlikely (!record)) return_trace (ମିଥ୍ୟା);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (ସତ୍ୟ);
  }

  ସର୍ବସାଧାରଣ:
  ଅଚଳ IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_Tables) ଅଚଳ
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_Tables; ++i)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      ଯଦି (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	ଫେରନ୍ତୁ &indexSubtablesZ[i];
    }
    ଫେରନ୍ତୁ nullptr;
  }

  ସୁରକ୍ଷିତ:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

ସଂରଚନା BitmapSizeTable
{
  friend ସଂରଚନା CBLC;
  friend ସଂରଚନା CBDT;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, ସଂଖ୍ୟା_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ଅଚଳ IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, ଅଚଳ ଶୂନ୍ୟ *base, ଅଚଳ ଶୂନ୍ୟ **out_base) ଅଚଳ
  {
    *out_base = &(base+indexSubtableArrayOffset);
    ଫେରନ୍ତୁ (base+indexSubtableArrayOffset).find_table (glyph, ସଂଖ୍ୟା_Of_Index_Subtables);
  }

  ତାର୍କିକ
  subset (hb_subset_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base,
	  ଅଚଳ ଅକ୍ଷର *cbdt, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cbdt_length,
	  hb_vector_t<ଅକ୍ଷର> *cbdt_ମୌଳିକ /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out_table = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out_table)) return_trace (ମିଥ୍ୟା);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_ମୌଳିକ = cbdt_ମୌଳିକ;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.ଅଙ୍କ_tables = ସଂଖ୍ୟା_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    ଯଦି (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (ମିଥ୍ୟା);
    ଯଦି (!bitmap_size_context.size ||
	!bitmap_size_context.ଅଙ୍କ_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (ମିଥ୍ୟା);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->ସଂଖ୍ୟା_Of_Index_Subtables = bitmap_size_context.ଅଙ୍କ_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (ସତ୍ୟ);
  }

  ସୁରକ୍ଷିତ:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		ସଂଖ୍ୟା_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_ଧାଡ଼ି_Metrics	horizontal;
  SBit_ଧାଡ଼ି_Metrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

ସଂରଚନା GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (9, data);
};

ସଂରଚନା GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (12, data);
};

ସଂରଚନା GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, data);
};

ସଂରଚନା CBLC
{
  friend ସଂରଚନା CBDT;

  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, ଏହି));
  }

  ସ୍ଥିର ତାର୍କିକ
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<ଅକ୍ଷର>* cbdt_ମୌଳିକ)
  {
    hb_blob_t *cbdt_ମୌଳିକ_blob = hb_blob_create (cbdt_ମୌଳିକ->arrayZ,
						 cbdt_ମୌଳିକ->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_ମୌଳିକ->arrayZ,
						 hb_free);
    cbdt_ମୌଳିକ->init ();  // Leak arrayZ to the blob.
    ତାର୍କିକ ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_ମୌଳିକ_blob);
    hb_blob_destroy (cbdt_ମୌଳିକ_blob);
    ଫେରନ୍ତୁ ret;
  }

  ତାର୍କିକ
  subset_size_table (hb_subset_context_t *c, ଅଚଳ BitmapSizeTable& table,
		     ଅଚଳ ଅକ୍ଷର *cbdt /* IN */, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cbdt_length,
		     CBLC *cblc_ମୌଳିକ /* INOUT */, hb_vector_t<ଅକ୍ଷର> *cbdt_ମୌଳିକ /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    cblc_ମୌଳିକ->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_ମୌଳିକ_len = cbdt_ମୌଳିକ->length;

    ଯଦି (!table.subset (c, ଏହି, cbdt, cbdt_length, cbdt_ମୌଳିକ, dedup))
    {
      cblc_ମୌଳିକ->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_ମୌଳିକ->shrink (cbdt_ମୌଳିକ_len);
      return_trace (ମିଥ୍ୟା);
    }
    return_trace (ସତ୍ୟ);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ;

  ସୁରକ୍ଷିତ:
  ଅଚଳ BitmapSizeTable &choose_strike (hb_font_t *font) ଅଚଳ
  {
    unsigned ଗଣନା_2 = sizeTables.len;
    ଯଦି (unlikely (!ଗଣନା_2))
      ଫେରନ୍ତୁ Null (BitmapSizeTable);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    ଯଦି (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_i = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ଗଣନା_2; i++)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      ଯଦି ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    ଫେରନ୍ତୁ sizeTables[best_i];
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

ସଂରଚନା CBDT
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ଏହି->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      ଏହି->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      ଏହି->cblc.destroy ();
      ଏହି->cbdt.destroy ();
    }

    ତାର୍କିକ
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, ତାର୍କିକ scale = ସତ୍ୟ) ଅଚଳ
    {
      ଅଚଳ ଶୂନ୍ୟ *base;
      ଅଚଳ BitmapSizeTable &strike = ଏହି->cblc->choose_strike (font);
      ଅଚଳ IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      ଯଦି (!subtable_record || !strike.ppemX || !strike.ppemY)
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (subtable_record->get_extents (extents, base, scale))
	ଫେରନ୍ତୁ ସତ୍ୟ;

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା image_offset = 0, image_length = 0, image_format = 0;
      ଯଦି (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cbdt_len = cbdt.get_length ();
      ଯଦି (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ବାଛ (image_format)
      {
      ସ୍ଥିତି 17: {
	ଯଦି (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (ଏହି->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	ଭାଙ୍ଗ;
      }
      ସ୍ଥିତି 18: {
	ଯଦି (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (ଏହି->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	ଭାଙ୍ଗ;
      }
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ ମିଥ୍ୟା; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      ଯଦି (scale)
      {
	ଦଶମିକ x_scale = upem / (ଦଶମିକ) strike.ppemX;
	ଦଶମିକ y_scale = upem / (ଦଶମିକ) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ଅଚଳ
    {
      ଅଚଳ ଶୂନ୍ୟ *base;
      ଅଚଳ BitmapSizeTable &strike = ଏହି->cblc->choose_strike (font);
      ଅଚଳ IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      ଯଦି (!subtable_record || !strike.ppemX || !strike.ppemY)
	ଫେରନ୍ତୁ hb_blob_get_empty ();

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା image_offset = 0, image_length = 0, image_format = 0;
      ଯଦି (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	ଫେରନ୍ତୁ hb_blob_get_empty ();

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cbdt_len = cbdt.get_length ();
      ଯଦି (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	ଫେରନ୍ତୁ hb_blob_get_empty ();

      ବାଛ (image_format)
      {
      ସ୍ଥିତି 17:
      {
	ଯଦି (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  ଫେରନ୍ତୁ hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (ଏହି->cbdt, image_offset);
	ଫେରନ୍ତୁ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      ସ୍ଥିତି 18:
      {
	ଯଦି (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  ଫେରନ୍ତୁ hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (ଏହି->cbdt, image_offset);
	ଫେରନ୍ତୁ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      ସ୍ଥିତି 19:
      {
	ଯଦି (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  ଫେରନ୍ତୁ hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (ଏହି->cbdt, image_offset);
	ଫେରନ୍ତୁ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ cbdt->version.major; }

    ତାର୍କିକ paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ଶୂନ୍ୟ *data) ଅଚଳ
    {
      ଯଦି (!has_data ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      ଯଦି (unlikely (!font->get_glyph_extents (glyph, &extents, ମିଥ୍ୟା)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (unlikely (!get_extents (font, glyph, &pixel_extents, ମିଥ୍ୟା)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      hb_blob_t *blob = reference_png (font, glyph);
      ଯଦି (unlikely (hb_blob_is_immutable (blob)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ତାର୍କିକ ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      ଫେରନ୍ତୁ ret;
    }

    ବ୍ୟକ୍ତିଗତ:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା upem;
  };

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

ଧାଡ଼ିଭିତରେ ତାର୍କିକ
CBLC::subset (hb_subset_context_t *c) ଅଚଳ
{
  TRACE_SUBSET (ଏହି);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<ଅକ୍ଷର> cbdt_ମୌଳିକ;

  auto *cblc_ମୌଳିକ = c->serializer->start_embed<CBLC> ();
  ଯଦି (unlikely (!c->serializer->extend_min (cblc_ମୌଳିକ))) return_trace (ମିଥ୍ୟା);
  cblc_ମୌଳିକ->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  ଯଦି (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (ମିଥ୍ୟା);
  }
  _copy_data_to_cbdt (&cbdt_ମୌଳିକ, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  ପାଇଁ (ଅଚଳ BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (ଅଚଳ ଅକ୍ଷର *) cbdt, cbdt_length, cblc_ମୌଳିକ, &cbdt_ମୌଳିକ, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_ମୌଳିକ));
}

ସଂରଚନା CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
