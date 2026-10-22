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

#include "../../../hb-open-type.hh"
#include "../../../hb-paint.hh"

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


namespace OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
sa_nracanaa cbdt_dedup_context_t
{
  sa_nracanaa entry_t
  {
    unsigned puur_naa_nka base;		/* Offset of the data in cbdt_prime. */
    unsigned puur_naa_nka lstart;	/* Start index in lengths. */
    unsigned puur_naa_nka lcount;	/* Number of length entries. */
  };

  static uint64_t hash (unsigned puur_naa_nka image_format,
			const hb_vector_t<var_na> &data,
			const hb_vector_t<unsigned puur_naa_nka> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((const var_na *) lengths.arrayZ,
				      lengths.length * parimaa_na (unsigned puur_naa_nka)));
    ᤀᤡᤴ ((uint64_t) image_format << 32) | h;
  }

  bool find (uint64_t key,
	     const hb_vector_t<var_na> &data,
	     const hb_vector_t<unsigned puur_naa_nka> &lengths,
	     const hb_vector_t<var_na> &cbdt_prime,
	     unsigned puur_naa_nka *base /* OUT */) const
  {
    unsigned puur_naa_nka *idx_p = nullptr;
    ᤀᤡ᤺ (!map.has (key, &idx_p)) ᤀᤡᤴ false;
    const entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    ᤀᤡ᤺ (e.lcount != lengths.length) ᤀᤡᤴ false;
    ᤀᤡ᤺ (unlikely (e.base + data.length > (unsigned puur_naa_nka) cbdt_prime.length)) ᤀᤡᤴ false;
    ᤀᤡ᤺ (hb_memcmp (cbdt_prime.arrayZ + e.base, data.arrayZ, data.length)) ᤀᤡᤴ false;
    ᤀᤡ᤺ (hb_memcmp (this->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * parimaa_na (unsigned puur_naa_nka)))
      ᤀᤡᤴ false;
    *base = e.base;
    ᤀᤡᤴ true;
  }

  bool add (uint64_t key,
	    unsigned puur_naa_nka base,
	    const hb_vector_t<unsigned puur_naa_nka> &lengths)
  {
    ᤀᤡ᤺ (unlikely (entries.in_error () || this->lengths.in_error () || map.in_error ()))
      ᤀᤡᤴ false;
    entry_t e = {base, this->lengths.length, lengths.length};
    ᤀᤡ᤺ (unlikely (!entries.push_or_fail (e))) ᤀᤡᤴ false;
    for (unsigned puur_naa_nka length : lengths)
      ᤀᤡ᤺ (unlikely (!this->lengths.push_or_fail (length))) ᤀᤡᤴ false;
    ᤀᤡᤴ map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned puur_naa_nka> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned puur_naa_nka> lengths; /* Concatenated per-entry lengths. */
};

sa_nracanaa cblc_bitmap_size_subset_context_t
{
  const var_na *cbdt;
  unsigned puur_naa_nka cbdt_length;
  hb_vector_t<var_na> *cbdt_prime;
  unsigned puur_naa_nka size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned puur_naa_nka num_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

static anta_hstha bool
_copy_data_to_cbdt (hb_vector_t<var_na> *cbdt_prime,
		    const void        *data,
		    unsigned           length)
{
  unsigned puur_naa_nka new_len = cbdt_prime->length + length;
  ᤀᤡ᤺ (unlikely (!cbdt_prime->alloc (new_len))) ᤀᤡᤴ false;
  hb_memcpy (cbdt_prime->arrayZ + cbdt_prime->length, data, length);
  cbdt_prime->length = new_len;
  ᤀᤡᤴ true;
}

sa_nracanaa SmallGlyphMetrics
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  void get_extents (hb_font_t *font, hb_glyph_extents_t *extents, bool scale) const
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<puur_naa_nka> (height);

    ᤀᤡ᤺ (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  public:
  DEFINE_SIZE_STATIC (5);
};

sa_nracanaa BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa SBitLineMetrics
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
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
  public:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

sa_nracanaa IndexSubtableHeader
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  public:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
sa_nracanaa IndexSubtableFormat1Or3
{
  bool sanitize (hb_sanitize_context_t *c, unsigned puur_naa_nka glyph_count) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  offsetArrayZ.sanitize (c, glyph_count + 1));
  }

  bool get_image_data (unsigned puur_naa_nka idx,
		       unsigned puur_naa_nka *offset,
		       unsigned puur_naa_nka *length) const
  {
    ᤀᤡ᤺ (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      ᤀᤡᤴ false;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    ᤀᤡᤴ true;
  }

  bool add_offset (hb_serialize_context_t *c,
		   unsigned puur_naa_nka offset,
		   unsigned puur_naa_nka *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (this);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += parimaa_na (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((bool) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  public:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

sa_nracanaa IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
sa_nracanaa IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

sa_nracanaa IndexSubtable
{
  bool sanitize (hb_sanitize_context_t *c, unsigned puur_naa_nka glyph_count) const
  {
    TRACE_SANITIZE (this);
    ᤀᤡ᤺ (!u.header.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.header.indexFormat)
    {
    pak_sa 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_count));
    pak_sa 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_count));
    default:return_trace (true);
    }
  }

  bool
  finish_subtable (hb_serialize_context_t *c,
		   unsigned puur_naa_nka end_offset,
		   unsigned puur_naa_nka num_glyphs,
		   unsigned puur_naa_nka *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (this);

    switch (u.header.indexFormat)
    {
    pak_sa 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    pak_sa 3: {
      hb_barrier ();
      ᤀᤡ᤺ (!u.format3.add_offset (c, end_offset, size))
	return_trace (false);
      ᤀᤡ᤺ (!(num_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (true);
    }
    // TODO: implement 2, 4, 5.
    pak_sa 2: pak_sa 4:  // No-op.
    pak_sa 5:  // Pad to 32-bit aligned.
    default: return_trace (false);
    }
  }

  bool
  add_offset (hb_serialize_context_t *c, unsigned puur_naa_nka local_offset,
	      unsigned puur_naa_nka *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (this);
    switch (u.header.indexFormat)
    {
    pak_sa 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    pak_sa 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    pak_sa 2:  // Should be a no-op.
    pak_sa 4: pak_sa 5:  // Handle sparse cases.
    default: return_trace (false);
    }
  }

  bool get_extents (hb_glyph_extents_t *extents HB_UNUSED, bool scale HB_UNUSED) const
  {
    switch (u.header.indexFormat)
    {
    pak_sa 2: pak_sa 5: /* TODO */
    pak_sa 1: pak_sa 3: pak_sa 4: /* Variable-metrics formats do not have metrics here. */
    default:ᤀᤡᤴ (false);
    }
  }

  bool
  get_image_data (unsigned puur_naa_nka idx, unsigned puur_naa_nka *offset,
		  unsigned puur_naa_nka *length, unsigned puur_naa_nka *format) const
  {
    *format = u.header.imageFormat;
    switch (u.header.indexFormat)
    {
    pak_sa 1: hb_barrier (); ᤀᤡᤴ u.format1.get_image_data (idx, offset, length);
    pak_sa 3: hb_barrier (); ᤀᤡᤴ u.format3.get_image_data (idx, offset, length);
    default: ᤀᤡᤴ false;
    }
  }

  const IndexSubtableHeader* get_header () const { ᤀᤡᤴ &u.header; }

  void populate_header (unsigned index_format,
			unsigned image_format,
			unsigned puur_naa_nka image_data_offset,
			unsigned puur_naa_nka *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    switch (u.header.indexFormat)
    {
    pak_sa 1: *size += IndexSubtableFormat1::min_size; bha_nga;
    pak_sa 3: *size += IndexSubtableFormat3::min_size; bha_nga;
    }
  }

  rak_sita:
  sa_ngha {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  public:
  DEFINE_SIZE_UNION (8, header);
};

sa_nracanaa IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (const IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    assert (offsetToSubtable.is_null ());
    ᤀᤡᤴ *this;
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  const IndexSubtable* get_subtable (const void *base) const
  {
    ᤀᤡᤴ &(base+offsetToSubtable);
  }

  bool add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 const hb_vector_t<hb_pair_t<hb_codepoint_t, const IndexSubtableRecord*>> *lookup, /* IN */
			 const void *base,
			 unsigned puur_naa_nka *start /* INOUT */) const
  {
    TRACE_SERIALIZE (this);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    ᤀᤡ᤺ (unlikely (!c->serializer->extend_min (subtable))) return_trace (false);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned puur_naa_nka image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<var_na> data;
    hb_vector_t<unsigned puur_naa_nka> lengths;
    unsigned puur_naa_nka num_glyphs = 0;
    bool early_exit = false;
    for (unsigned puur_naa_nka i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      const IndexSubtableRecord *next_record = (*lookup)[i].second;
      const IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      ᤀᤡ᤺ (next_header != old_header)
      {
	*start = i;
	early_exit = true;
	bha_nga;
      }
      unsigned puur_naa_nka num_missing = record->add_glyph_for_subset (new_gid);
      for (unsigned puur_naa_nka j = 0; j < num_missing; j++)
	ᤀᤡ᤺ (unlikely (!c->serializer->check_success (lengths.push_or_fail (0))))
	  return_trace (false);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      ᤀᤡ᤺ (old_gid < next_record->firstGlyphIndex)
	return_trace (false);

      unsigned puur_naa_nka old_idx = (unsigned puur_naa_nka) old_gid - next_record->firstGlyphIndex;
      unsigned puur_naa_nka offset, length, format;
      ᤀᤡ᤺ (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (false);
      ᤀᤡ᤺ (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (false);
      ᤀᤡ᤺ (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (false);
      ᤀᤡ᤺ (unlikely (!c->serializer->check_success (lengths.push_or_fail (length))))
	return_trace (false);
      num_glyphs += 1 + num_missing;
    }
    ᤀᤡ᤺ (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned puur_naa_nka image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    bool shared = false;
    uint64_t key = 0;
    ᤀᤡ᤺ (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_prime,
			    &image_data_offset);
    }
    ᤀᤡ᤺ (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_prime->length;
      ᤀᤡ᤺ (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_prime,
					    data.arrayZ, data.length))))
	return_trace (false);
      ᤀᤡ᤺ (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(void) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned puur_naa_nka cumulative = 0;
    for (unsigned puur_naa_nka length : lengths)
    {
      ᤀᤡ᤺ (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (false);
      cumulative += length;
    }
    ᤀᤡ᤺ (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      num_glyphs,
					      &bitmap_size_context->size)))
      return_trace (false);
    return_trace (true);
  }

  bool add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       const hb_vector_t<hb_pair_t<hb_codepoint_t, const IndexSubtableRecord*>> *lookup, /* IN */
		       const void *base,
		       unsigned puur_naa_nka *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) const
  {
    TRACE_SERIALIZE (this);
    auto snap = c->serializer->snapshot ();
    unsigned puur_naa_nka old_size = bitmap_size_context->size;
    unsigned puur_naa_nka old_cbdt_prime_length = bitmap_size_context->cbdt_prime->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    ᤀᤡ᤺ (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (false);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    ᤀᤡ᤺ (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->pop_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_prime->shrink (old_cbdt_prime_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (false);
    }

    bitmap_size_context->num_tables += 1;
    return_trace (true);
  }

  unsigned puur_naa_nka add_glyph_for_subset (hb_codepoint_t gid)
  {
    ᤀᤡ᤺ (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      ᤀᤡᤴ 0;
    }
    // TODO maybe assert? this shouldn't occur.
    ᤀᤡ᤺ (lastGlyphIndex > gid)
      ᤀᤡᤴ 0;
    unsigned puur_naa_nka num_missing = (unsigned puur_naa_nka) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    ᤀᤡᤴ num_missing;
  }

  bool get_extents (hb_glyph_extents_t *extents, const void *base, bool scale) const
  { ᤀᤡᤴ (base+offsetToSubtable).get_extents (extents, scale); }

  bool get_image_data (unsigned puur_naa_nka  gid,
		       const void   *base,
		       unsigned puur_naa_nka *offset,
		       unsigned puur_naa_nka *length,
		       unsigned puur_naa_nka *format) const
  {
    ᤀᤡ᤺ (gid < firstGlyphIndex || gid > lastGlyphIndex) ᤀᤡᤴ false;
    ᤀᤡᤴ (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa IndexSubtableArray
{
  friend sa_nracanaa CBDT;

  bool sanitize (hb_sanitize_context_t *c, unsigned puur_naa_nka count) const
  {
    TRACE_SANITIZE (this);
    return_trace (indexSubtablesZ.sanitize (c, count, this));
  }

  void
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		const IndexSubtableRecord*>> *lookup /* OUT */) const
  {
    bool start_glyph_is_set = false;
    unsigned num_glyphs = c->plan->num_output_glyphs ();
    for (hb_codepoint_t new_gid = 0; new_gid < num_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      ᤀᤡ᤺ (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) continue;

      const IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->num_tables);
      ᤀᤡ᤺ (unlikely (!record)) continue;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned puur_naa_nka offset, length, format;
      ᤀᤡ᤺ (unlikely (!record->get_image_data (old_gid, this, &offset, &length, &format))) continue;

      lookup->push (hb_pair_t<hb_codepoint_t, const IndexSubtableRecord*> (new_gid, record));

      ᤀᤡ᤺ (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = true;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  bool
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) const
  {
    TRACE_SUBSET (this);

    hb_vector_t<hb_pair_t<hb_codepoint_t, const IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    ᤀᤡ᤺ (unlikely (!c->serializer->propagate_error (lookup)))
      ᤀᤡᤴ false;

    bitmap_size_context->size = 0;
    bitmap_size_context->num_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    for (unsigned puur_naa_nka start = 0; start < lookup.length;)
    {
      ᤀᤡ᤺ (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, this, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	for (unsigned puur_naa_nka i = 0; i < records.length; i++)
	  c->serializer->pop_discard ();
	return_trace (false);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    for (unsigned puur_naa_nka i = 0; i < records.length; i++)
      objidxs.push (c->serializer->pop_pack ());
    for (unsigned puur_naa_nka i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      ᤀᤡ᤺ (unlikely (!record)) return_trace (false);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (true);
  }

  public:
  const IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned puur_naa_nka numTables) const
  {
    for (unsigned puur_naa_nka i = 0; i < numTables; ++i)
    {
      unsigned puur_naa_nka firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned puur_naa_nka lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      ᤀᤡ᤺ (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	ᤀᤡᤴ &indexSubtablesZ[i];
    }
    ᤀᤡᤴ nullptr;
  }

  rak_sita:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

sa_nracanaa BitmapSizeTable
{
  friend sa_nracanaa CBLC;
  friend sa_nracanaa CBDT;

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, numberOfIndexSubtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  const IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, const void *base, const void **out_base) const
  {
    *out_base = &(base+indexSubtableArrayOffset);
    ᤀᤡᤴ (base+indexSubtableArrayOffset).find_table (glyph, numberOfIndexSubtables);
  }

  bool
  subset (hb_subset_context_t *c, const void *base,
	  const var_na *cbdt, unsigned puur_naa_nka cbdt_length,
	  hb_vector_t<var_na> *cbdt_prime /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) const
  {
    TRACE_SUBSET (this);
    auto *out_table = c->serializer->embed (this);
    ᤀᤡ᤺ (unlikely (!out_table)) return_trace (false);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_prime = cbdt_prime;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.num_tables = numberOfIndexSubtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    ᤀᤡ᤺ (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (false);
    ᤀᤡ᤺ (!bitmap_size_context.size ||
	!bitmap_size_context.num_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (false);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->numberOfIndexSubtables = bitmap_size_context.num_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (true);
  }

  rak_sita:
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
  public:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

sa_nracanaa GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  public:
  DEFINE_SIZE_ARRAY (9, data);
};

sa_nracanaa GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  public:
  DEFINE_SIZE_ARRAY (12, data);
};

sa_nracanaa GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  public:
  DEFINE_SIZE_ARRAY (4, data);
};

sa_nracanaa CBLC
{
  friend sa_nracanaa CBDT;

  static constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, this));
  }

  static bool
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<var_na>* cbdt_prime)
  {
    hb_blob_t *cbdt_prime_blob = hb_blob_create (cbdt_prime->arrayZ,
						 cbdt_prime->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_prime->arrayZ,
						 hb_free);
    cbdt_prime->init ();  // Leak arrayZ to the blob.
    bool ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_prime_blob);
    hb_blob_destroy (cbdt_prime_blob);
    ᤀᤡᤴ ret;
  }

  bool
  subset_size_table (hb_subset_context_t *c, const BitmapSizeTable& table,
		     const var_na *cbdt /* IN */, unsigned puur_naa_nka cbdt_length,
		     CBLC *cblc_prime /* INOUT */, hb_vector_t<var_na> *cbdt_prime /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) const
  {
    TRACE_SUBSET (this);
    cblc_prime->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_prime_len = cbdt_prime->length;

    ᤀᤡ᤺ (!table.subset (c, this, cbdt, cbdt_length, cbdt_prime, dedup))
    {
      cblc_prime->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_prime->shrink (cbdt_prime_len);
      return_trace (false);
    }
    return_trace (true);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL bool subset (hb_subset_context_t *c) const;

  rak_sita:
  const BitmapSizeTable &choose_strike (hb_font_t *font) const
  {
    unsigned count = sizeTables.len;
    ᤀᤡ᤺ (unlikely (!count))
      ᤀᤡᤴ Null (BitmapSizeTable);

    unsigned puur_naa_nka requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    ᤀᤡ᤺ (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned puur_naa_nka best_i = 0;
    unsigned puur_naa_nka best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    for (unsigned puur_naa_nka i = 1; i < count; i++)
    {
      unsigned puur_naa_nka ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      ᤀᤡ᤺ ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    ᤀᤡᤴ sizeTables[best_i];
  }

  rak_sita:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  public:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

sa_nracanaa CBDT
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  sa_nracanaa accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      this->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      this->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      this->cblc.destroy ();
      this->cbdt.destroy ();
    }

    bool
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, bool scale = true) const
    {
      const void *base;
      const BitmapSizeTable &strike = this->cblc->choose_strike (font);
      const IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      ᤀᤡ᤺ (!subtable_record || !strike.ppemX || !strike.ppemY)
	ᤀᤡᤴ false;

      ᤀᤡ᤺ (subtable_record->get_extents (extents, base, scale))
	ᤀᤡᤴ true;

      unsigned puur_naa_nka image_offset = 0, image_length = 0, image_format = 0;
      ᤀᤡ᤺ (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	ᤀᤡᤴ false;

      unsigned puur_naa_nka cbdt_len = cbdt.get_length ();
      ᤀᤡ᤺ (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	ᤀᤡᤴ false;

      switch (image_format)
      {
      pak_sa 17: {
	ᤀᤡ᤺ (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  ᤀᤡᤴ false;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (this->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	bha_nga;
      }
      pak_sa 18: {
	ᤀᤡ᤺ (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  ᤀᤡᤴ false;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (this->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	bha_nga;
      }
      default: ᤀᤡᤴ false; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      ᤀᤡ᤺ (scale)
      {
	float x_scale = upem / (float) strike.ppemX;
	float y_scale = upem / (float) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      ᤀᤡᤴ true;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) const
    {
      const void *base;
      const BitmapSizeTable &strike = this->cblc->choose_strike (font);
      const IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      ᤀᤡ᤺ (!subtable_record || !strike.ppemX || !strike.ppemY)
	ᤀᤡᤴ hb_blob_get_empty ();

      unsigned puur_naa_nka image_offset = 0, image_length = 0, image_format = 0;
      ᤀᤡ᤺ (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	ᤀᤡᤴ hb_blob_get_empty ();

      unsigned puur_naa_nka cbdt_len = cbdt.get_length ();
      ᤀᤡ᤺ (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	ᤀᤡᤴ hb_blob_get_empty ();

      switch (image_format)
      {
      pak_sa 17:
      {
	ᤀᤡ᤺ (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  ᤀᤡᤴ hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (this->cbdt, image_offset);
	ᤀᤡᤴ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      pak_sa 18:
      {
	ᤀᤡ᤺ (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  ᤀᤡᤴ hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (this->cbdt, image_offset);
	ᤀᤡᤴ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      pak_sa 19:
      {
	ᤀᤡ᤺ (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  ᤀᤡᤴ hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (this->cbdt, image_offset);
	ᤀᤡᤴ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      default: ᤀᤡᤴ hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    bool has_data () const { ᤀᤡᤴ cbdt->version.major; }

    bool paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, void *data) const
    {
      ᤀᤡ᤺ (!has_data ()) ᤀᤡᤴ false;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      ᤀᤡ᤺ (unlikely (!font->get_glyph_extents (glyph, &extents, false)))
        ᤀᤡᤴ false;

      ᤀᤡ᤺ (unlikely (!get_extents (font, glyph, &pixel_extents, false)))
        ᤀᤡᤴ false;

      hb_blob_t *blob = reference_png (font, glyph);
      ᤀᤡ᤺ (unlikely (hb_blob_is_immutable (blob)))
        ᤀᤡᤴ false;

      bool ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      ᤀᤡᤴ ret;
    }

    private:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned puur_naa_nka upem;
  };

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  rak_sita:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  public:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

anta_hstha bool
CBLC::subset (hb_subset_context_t *c) const
{
  TRACE_SUBSET (this);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<var_na> cbdt_prime;

  auto *cblc_prime = c->serializer->start_embed<CBLC> ();
  ᤀᤡ᤺ (unlikely (!c->serializer->extend_min (cblc_prime))) return_trace (false);
  cblc_prime->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned puur_naa_nka cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  ᤀᤡ᤺ (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (false);
  }
  _copy_data_to_cbdt (&cbdt_prime, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  for (const BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (const var_na *) cbdt, cbdt_length, cblc_prime, &cbdt_prime, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_prime));
}

sa_nracanaa CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
