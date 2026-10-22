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

#അന്തര്ഭാവ "../../../hb-open-type.hh"
#അന്തര്ഭാവ "../../../hb-paint.hh"

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


നാമാകാശ OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
ഘടന cbdt_dedup_context_t
{
  ഘടന entry_t
  {
    unsigned പൂർണ്ണസംഖ്യ base;		/* Offset of the data in cbdt_prime. */
    unsigned പൂർണ്ണസംഖ്യ lstart;	/* Start index in lengths. */
    unsigned പൂർണ്ണസംഖ്യ lcount;	/* Number of length entries. */
  };

  സ്ഥിതം uint64_t hash (unsigned പൂർണ്ണസംഖ്യ image_format,
			സ്ഥിരം hb_vector_t<അക്ഷരം> &data,
			സ്ഥിരം hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((സ്ഥിരം അക്ഷരം *) lengths.arrayZ,
				      lengths.length * വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ)));
    തിരികെ ((uint64_t) image_format << 32) | h;
  }

  തർക്കം find (uint64_t key,
	     സ്ഥിരം hb_vector_t<അക്ഷരം> &data,
	     സ്ഥിരം hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> &lengths,
	     സ്ഥിരം hb_vector_t<അക്ഷരം> &cbdt_അഭാജ്യം,
	     unsigned പൂർണ്ണസംഖ്യ *base /* OUT */) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ *idx_p = nullptr;
    എങ്കിൽ (!map.has (key, &idx_p)) തിരികെ അസത്യം;
    സ്ഥിരം entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    എങ്കിൽ (e.lcount != lengths.length) തിരികെ അസത്യം;
    എങ്കിൽ (unlikely (e.base + data.length > (unsigned പൂർണ്ണസംഖ്യ) cbdt_അഭാജ്യം.length)) തിരികെ അസത്യം;
    എങ്കിൽ (hb_memcmp (cbdt_അഭാജ്യം.arrayZ + e.base, data.arrayZ, data.length)) തിരികെ അസത്യം;
    എങ്കിൽ (hb_memcmp (ഇത്->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * വലുപ്പം (unsigned പൂർണ്ണസംഖ്യ)))
      തിരികെ അസത്യം;
    *base = e.base;
    തിരികെ സത്യം;
  }

  തർക്കം add (uint64_t key,
	    unsigned പൂർണ്ണസംഖ്യ base,
	    സ്ഥിരം hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> &lengths)
  {
    എങ്കിൽ (unlikely (entries.in_error () || ഇത്->lengths.in_error () || map.in_error ()))
      തിരികെ അസത്യം;
    entry_t e = {base, ഇത്->lengths.length, lengths.length};
    എങ്കിൽ (unlikely (!entries.തള്ളുക_or_fail (e))) തിരികെ അസത്യം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ length : lengths)
      എങ്കിൽ (unlikely (!ഇത്->lengths.തള്ളുക_or_fail (length))) തിരികെ അസത്യം;
    തിരികെ map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned പൂർണ്ണസംഖ്യ> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> lengths; /* Concatenated per-entry lengths. */
};

ഘടന cblc_bitmap_size_subset_context_t
{
  സ്ഥിരം അക്ഷരം *cbdt;
  unsigned പൂർണ്ണസംഖ്യ cbdt_length;
  hb_vector_t<അക്ഷരം> *cbdt_അഭാജ്യം;
  unsigned പൂർണ്ണസംഖ്യ size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned പൂർണ്ണസംഖ്യ അക്കം_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

സ്ഥിതം വരിയിൽ തർക്കം
_copy_data_to_cbdt (hb_vector_t<അക്ഷരം> *cbdt_അഭാജ്യം,
		    സ്ഥിരം ശൂന്യം        *data,
		    unsigned           length)
{
  unsigned പൂർണ്ണസംഖ്യ new_len = cbdt_അഭാജ്യം->length + length;
  എങ്കിൽ (unlikely (!cbdt_അഭാജ്യം->alloc (new_len))) തിരികെ അസത്യം;
  hb_memcpy (cbdt_അഭാജ്യം->arrayZ + cbdt_അഭാജ്യം->length, data, length);
  cbdt_അഭാജ്യം->length = new_len;
  തിരികെ സത്യം;
}

ഘടന SmallGlyphMetrics
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  ശൂന്യം get_extents (hb_font_t *font, hb_glyph_extents_t *extents, തർക്കം scale) സ്ഥിരം
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<പൂർണ്ണസംഖ്യ> (height);

    എങ്കിൽ (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  പൊതു:
  DEFINE_SIZE_STATIC (5);
};

ഘടന BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന SBit_വരി_Metrics
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
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
  പൊതു:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

ഘടന IndexSubtableHeader
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
ഘടന IndexSubtableFormat1Or3
{
  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ glyph_എണ്ണം) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  offsetArrayZ.sanitize (c, glyph_എണ്ണം + 1));
  }

  തർക്കം get_image_data (unsigned പൂർണ്ണസംഖ്യ idx,
		       unsigned പൂർണ്ണസംഖ്യ *offset,
		       unsigned പൂർണ്ണസംഖ്യ *length) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      തിരികെ അസത്യം;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    തിരികെ സത്യം;
  }

  തർക്കം add_offset (hb_serialize_context_t *c,
		   unsigned പൂർണ്ണസംഖ്യ offset,
		   unsigned പൂർണ്ണസംഖ്യ *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ഇത്);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += വലുപ്പം (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((തർക്കം) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  പൊതു:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

ഘടന IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
ഘടന IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

ഘടന IndexSubtable
{
  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ glyph_എണ്ണം) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.header.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.header.indexFormat)
    {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_എണ്ണം));
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_എണ്ണം));
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  തർക്കം
  finish_subtable (hb_serialize_context_t *c,
		   unsigned പൂർണ്ണസംഖ്യ end_offset,
		   unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs,
		   unsigned പൂർണ്ണസംഖ്യ *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ഇത്);

    തിരഞ്ഞെടുപ്പ് (u.header.indexFormat)
    {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    അവസ്ഥ 3: {
      hb_barrier ();
      എങ്കിൽ (!u.format3.add_offset (c, end_offset, size))
	return_trace (അസത്യം);
      എങ്കിൽ (!(അക്കം_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (സത്യം);
    }
    // TODO: implement 2, 4, 5.
    അവസ്ഥ 2: അവസ്ഥ 4:  // No-op.
    അവസ്ഥ 5:  // Pad to 32-bit aligned.
    സ്ഥിരസ്ഥിതി: return_trace (അസത്യം);
    }
  }

  തർക്കം
  add_offset (hb_serialize_context_t *c, unsigned പൂർണ്ണസംഖ്യ local_offset,
	      unsigned പൂർണ്ണസംഖ്യ *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ഇത്);
    തിരഞ്ഞെടുപ്പ് (u.header.indexFormat)
    {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    അവസ്ഥ 2:  // Should be a no-op.
    അവസ്ഥ 4: അവസ്ഥ 5:  // Handle sparse cases.
    സ്ഥിരസ്ഥിതി: return_trace (അസത്യം);
    }
  }

  തർക്കം get_extents (hb_glyph_extents_t *extents HB_UNUSED, തർക്കം scale HB_UNUSED) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.header.indexFormat)
    {
    അവസ്ഥ 2: അവസ്ഥ 5: /* TODO */
    അവസ്ഥ 1: അവസ്ഥ 3: അവസ്ഥ 4: /* Variable-metrics formats do not have metrics here. */
    സ്ഥിരസ്ഥിതി:തിരികെ (അസത്യം);
    }
  }

  തർക്കം
  get_image_data (unsigned പൂർണ്ണസംഖ്യ idx, unsigned പൂർണ്ണസംഖ്യ *offset,
		  unsigned പൂർണ്ണസംഖ്യ *length, unsigned പൂർണ്ണസംഖ്യ *format) സ്ഥിരം
  {
    *format = u.header.imageFormat;
    തിരഞ്ഞെടുപ്പ് (u.header.indexFormat)
    {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_image_data (idx, offset, length);
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_image_data (idx, offset, length);
    സ്ഥിരസ്ഥിതി: തിരികെ അസത്യം;
    }
  }

  സ്ഥിരം IndexSubtableHeader* get_header () സ്ഥിരം { തിരികെ &u.header; }

  ശൂന്യം populate_header (unsigned index_format,
			unsigned image_format,
			unsigned പൂർണ്ണസംഖ്യ image_data_offset,
			unsigned പൂർണ്ണസംഖ്യ *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    തിരഞ്ഞെടുപ്പ് (u.header.indexFormat)
    {
    അവസ്ഥ 1: *size += IndexSubtableFormat1::min_size; നിർത്തുക;
    അവസ്ഥ 3: *size += IndexSubtableFormat3::min_size; നിർത്തുക;
    }
  }

  സംരക്ഷിത:
  സംഘം {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (8, header);
};

ഘടന IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (സ്ഥിരം IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    പ്രതിജ്ഞാ (offsetToSubtable.is_null ());
    തിരികെ *ഇത്;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  സ്ഥിരം IndexSubtable* get_subtable (സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    തിരികെ &(base+offsetToSubtable);
  }

  തർക്കം add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 സ്ഥിരം hb_vector_t<hb_pair_t<hb_codepoint_t, സ്ഥിരം IndexSubtableRecord*>> *lookup, /* IN */
			 സ്ഥിരം ശൂന്യം *base,
			 unsigned പൂർണ്ണസംഖ്യ *start /* INOUT */) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    എങ്കിൽ (unlikely (!c->serializer->extend_min (subtable))) return_trace (അസത്യം);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned പൂർണ്ണസംഖ്യ image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<അക്ഷരം> data;
    hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> lengths;
    unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs = 0;
    തർക്കം early_exit = അസത്യം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      സ്ഥിരം IndexSubtableRecord *next_record = (*lookup)[i].second;
      സ്ഥിരം IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      എങ്കിൽ (next_header != old_header)
      {
	*start = i;
	early_exit = സത്യം;
	നിർത്തുക;
      }
      unsigned പൂർണ്ണസംഖ്യ അക്കം_missing = record->add_glyph_for_subset (new_gid);
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < അക്കം_missing; j++)
	എങ്കിൽ (unlikely (!c->serializer->check_success (lengths.തള്ളുക_or_fail (0))))
	  return_trace (അസത്യം);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      എങ്കിൽ (old_gid < next_record->firstGlyphIndex)
	return_trace (അസത്യം);

      unsigned പൂർണ്ണസംഖ്യ old_idx = (unsigned പൂർണ്ണസംഖ്യ) old_gid - next_record->firstGlyphIndex;
      unsigned പൂർണ്ണസംഖ്യ offset, length, format;
      എങ്കിൽ (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (അസത്യം);
      എങ്കിൽ (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (അസത്യം);
      എങ്കിൽ (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (അസത്യം);
      എങ്കിൽ (unlikely (!c->serializer->check_success (lengths.തള്ളുക_or_fail (length))))
	return_trace (അസത്യം);
      അക്കം_glyphs += 1 + അക്കം_missing;
    }
    എങ്കിൽ (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned പൂർണ്ണസംഖ്യ image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    തർക്കം shared = അസത്യം;
    uint64_t key = 0;
    എങ്കിൽ (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_അഭാജ്യം,
			    &image_data_offset);
    }
    എങ്കിൽ (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_അഭാജ്യം->length;
      എങ്കിൽ (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_അഭാജ്യം,
					    data.arrayZ, data.length))))
	return_trace (അസത്യം);
      എങ്കിൽ (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(ശൂന്യം) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned പൂർണ്ണസംഖ്യ cumulative = 0;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ length : lengths)
    {
      എങ്കിൽ (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (അസത്യം);
      cumulative += length;
    }
    എങ്കിൽ (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      അക്കം_glyphs,
					      &bitmap_size_context->size)))
      return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  തർക്കം add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       സ്ഥിരം hb_vector_t<hb_pair_t<hb_codepoint_t, സ്ഥിരം IndexSubtableRecord*>> *lookup, /* IN */
		       സ്ഥിരം ശൂന്യം *base,
		       unsigned പൂർണ്ണസംഖ്യ *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto snap = c->serializer->snapshot ();
    unsigned പൂർണ്ണസംഖ്യ old_size = bitmap_size_context->size;
    unsigned പൂർണ്ണസംഖ്യ old_cbdt_അഭാജ്യം_length = bitmap_size_context->cbdt_അഭാജ്യം->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    എങ്കിൽ (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (അസത്യം);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    എങ്കിൽ (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->എടുക്കുക_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_അഭാജ്യം->shrink (old_cbdt_അഭാജ്യം_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (അസത്യം);
    }

    bitmap_size_context->അക്കം_tables += 1;
    return_trace (സത്യം);
  }

  unsigned പൂർണ്ണസംഖ്യ add_glyph_for_subset (hb_codepoint_t gid)
  {
    എങ്കിൽ (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      തിരികെ 0;
    }
    // TODO maybe assert? this shouldn't occur.
    എങ്കിൽ (lastGlyphIndex > gid)
      തിരികെ 0;
    unsigned പൂർണ്ണസംഖ്യ അക്കം_missing = (unsigned പൂർണ്ണസംഖ്യ) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    തിരികെ അക്കം_missing;
  }

  തർക്കം get_extents (hb_glyph_extents_t *extents, സ്ഥിരം ശൂന്യം *base, തർക്കം scale) സ്ഥിരം
  { തിരികെ (base+offsetToSubtable).get_extents (extents, scale); }

  തർക്കം get_image_data (unsigned പൂർണ്ണസംഖ്യ  gid,
		       സ്ഥിരം ശൂന്യം   *base,
		       unsigned പൂർണ്ണസംഖ്യ *offset,
		       unsigned പൂർണ്ണസംഖ്യ *length,
		       unsigned പൂർണ്ണസംഖ്യ *format) സ്ഥിരം
  {
    എങ്കിൽ (gid < firstGlyphIndex || gid > lastGlyphIndex) തിരികെ അസത്യം;
    തിരികെ (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന IndexSubtableArray
{
  friend ഘടന CBDT;

  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ എണ്ണം) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (indexSubtablesZ.sanitize (c, എണ്ണം, ഇത്));
  }

  ശൂന്യം
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		സ്ഥിരം IndexSubtableRecord*>> *lookup /* OUT */) സ്ഥിരം
  {
    തർക്കം start_glyph_is_set = അസത്യം;
    unsigned അക്കം_glyphs = c->plan->അക്കം_output_glyphs ();
    വേണ്ടി (hb_codepoint_t new_gid = 0; new_gid < അക്കം_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      എങ്കിൽ (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) തുടരുക;

      സ്ഥിരം IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->അക്കം_tables);
      എങ്കിൽ (unlikely (!record)) തുടരുക;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned പൂർണ്ണസംഖ്യ offset, length, format;
      എങ്കിൽ (unlikely (!record->get_image_data (old_gid, ഇത്, &offset, &length, &format))) തുടരുക;

      lookup->push (hb_pair_t<hb_codepoint_t, സ്ഥിരം IndexSubtableRecord*> (new_gid, record));

      എങ്കിൽ (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = സത്യം;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  തർക്കം
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    hb_vector_t<hb_pair_t<hb_codepoint_t, സ്ഥിരം IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    എങ്കിൽ (unlikely (!c->serializer->propagate_error (lookup)))
      തിരികെ അസത്യം;

    bitmap_size_context->size = 0;
    bitmap_size_context->അക്കം_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ start = 0; start < lookup.length;)
    {
      എങ്കിൽ (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, ഇത്, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < records.length; i++)
	  c->serializer->എടുക്കുക_discard ();
	return_trace (അസത്യം);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < records.length; i++)
      objidxs.push (c->serializer->എടുക്കുക_pack ());
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      എങ്കിൽ (unlikely (!record)) return_trace (അസത്യം);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (സത്യം);
  }

  പൊതു:
  സ്ഥിരം IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned പൂർണ്ണസംഖ്യ അക്കം_Tables) സ്ഥിരം
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_Tables; ++i)
    {
      unsigned പൂർണ്ണസംഖ്യ firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned പൂർണ്ണസംഖ്യ lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      എങ്കിൽ (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	തിരികെ &indexSubtablesZ[i];
    }
    തിരികെ nullptr;
  }

  സംരക്ഷിത:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

ഘടന BitmapSizeTable
{
  friend ഘടന CBLC;
  friend ഘടന CBDT;

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, സംഖ്യ_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  സ്ഥിരം IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, സ്ഥിരം ശൂന്യം *base, സ്ഥിരം ശൂന്യം **out_base) സ്ഥിരം
  {
    *out_base = &(base+indexSubtableArrayOffset);
    തിരികെ (base+indexSubtableArrayOffset).find_table (glyph, സംഖ്യ_Of_Index_Subtables);
  }

  തർക്കം
  subset (hb_subset_context_t *c, സ്ഥിരം ശൂന്യം *base,
	  സ്ഥിരം അക്ഷരം *cbdt, unsigned പൂർണ്ണസംഖ്യ cbdt_length,
	  hb_vector_t<അക്ഷരം> *cbdt_അഭാജ്യം /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out_table = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out_table)) return_trace (അസത്യം);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_അഭാജ്യം = cbdt_അഭാജ്യം;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.അക്കം_tables = സംഖ്യ_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    എങ്കിൽ (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (അസത്യം);
    എങ്കിൽ (!bitmap_size_context.size ||
	!bitmap_size_context.അക്കം_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (അസത്യം);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->സംഖ്യ_Of_Index_Subtables = bitmap_size_context.അക്കം_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (സത്യം);
  }

  സംരക്ഷിത:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		സംഖ്യ_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_വരി_Metrics	horizontal;
  SBit_വരി_Metrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  പൊതു:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

ഘടന GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  പൊതു:
  DEFINE_SIZE_ARRAY (9, data);
};

ഘടന GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  പൊതു:
  DEFINE_SIZE_ARRAY (12, data);
};

ഘടന GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  പൊതു:
  DEFINE_SIZE_ARRAY (4, data);
};

ഘടന CBLC
{
  friend ഘടന CBDT;

  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, ഇത്));
  }

  സ്ഥിതം തർക്കം
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<അക്ഷരം>* cbdt_അഭാജ്യം)
  {
    hb_blob_t *cbdt_അഭാജ്യം_blob = hb_blob_create (cbdt_അഭാജ്യം->arrayZ,
						 cbdt_അഭാജ്യം->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_അഭാജ്യം->arrayZ,
						 hb_free);
    cbdt_അഭാജ്യം->init ();  // Leak arrayZ to the blob.
    തർക്കം ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_അഭാജ്യം_blob);
    hb_blob_destroy (cbdt_അഭാജ്യം_blob);
    തിരികെ ret;
  }

  തർക്കം
  subset_size_table (hb_subset_context_t *c, സ്ഥിരം BitmapSizeTable& table,
		     സ്ഥിരം അക്ഷരം *cbdt /* IN */, unsigned പൂർണ്ണസംഖ്യ cbdt_length,
		     CBLC *cblc_അഭാജ്യം /* INOUT */, hb_vector_t<അക്ഷരം> *cbdt_അഭാജ്യം /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    cblc_അഭാജ്യം->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_അഭാജ്യം_len = cbdt_അഭാജ്യം->length;

    എങ്കിൽ (!table.subset (c, ഇത്, cbdt, cbdt_length, cbdt_അഭാജ്യം, dedup))
    {
      cblc_അഭാജ്യം->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_അഭാജ്യം->shrink (cbdt_അഭാജ്യം_len);
      return_trace (അസത്യം);
    }
    return_trace (സത്യം);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം;

  സംരക്ഷിത:
  സ്ഥിരം BitmapSizeTable &choose_strike (hb_font_t *font) സ്ഥിരം
  {
    unsigned എണ്ണം = sizeTables.len;
    എങ്കിൽ (unlikely (!എണ്ണം))
      തിരികെ Null (BitmapSizeTable);

    unsigned പൂർണ്ണസംഖ്യ requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    എങ്കിൽ (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned പൂർണ്ണസംഖ്യ best_i = 0;
    unsigned പൂർണ്ണസംഖ്യ best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      എങ്കിൽ ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    തിരികെ sizeTables[best_i];
  }

  സംരക്ഷിത:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  പൊതു:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

ഘടന CBDT
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  ഘടന accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ഇത്->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      ഇത്->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      ഇത്->cblc.destroy ();
      ഇത്->cbdt.destroy ();
    }

    തർക്കം
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, തർക്കം scale = സത്യം) സ്ഥിരം
    {
      സ്ഥിരം ശൂന്യം *base;
      സ്ഥിരം BitmapSizeTable &strike = ഇത്->cblc->choose_strike (font);
      സ്ഥിരം IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      എങ്കിൽ (!subtable_record || !strike.ppemX || !strike.ppemY)
	തിരികെ അസത്യം;

      എങ്കിൽ (subtable_record->get_extents (extents, base, scale))
	തിരികെ സത്യം;

      unsigned പൂർണ്ണസംഖ്യ image_offset = 0, image_length = 0, image_format = 0;
      എങ്കിൽ (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	തിരികെ അസത്യം;

      unsigned പൂർണ്ണസംഖ്യ cbdt_len = cbdt.get_length ();
      എങ്കിൽ (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	തിരികെ അസത്യം;

      തിരഞ്ഞെടുപ്പ് (image_format)
      {
      അവസ്ഥ 17: {
	എങ്കിൽ (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  തിരികെ അസത്യം;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (ഇത്->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	നിർത്തുക;
      }
      അവസ്ഥ 18: {
	എങ്കിൽ (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  തിരികെ അസത്യം;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (ഇത്->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	നിർത്തുക;
      }
      സ്ഥിരസ്ഥിതി: തിരികെ അസത്യം; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      എങ്കിൽ (scale)
      {
	ദശാംശം x_scale = upem / (ദശാംശം) strike.ppemX;
	ദശാംശം y_scale = upem / (ദശാംശം) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      തിരികെ സത്യം;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) സ്ഥിരം
    {
      സ്ഥിരം ശൂന്യം *base;
      സ്ഥിരം BitmapSizeTable &strike = ഇത്->cblc->choose_strike (font);
      സ്ഥിരം IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      എങ്കിൽ (!subtable_record || !strike.ppemX || !strike.ppemY)
	തിരികെ hb_blob_get_empty ();

      unsigned പൂർണ്ണസംഖ്യ image_offset = 0, image_length = 0, image_format = 0;
      എങ്കിൽ (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	തിരികെ hb_blob_get_empty ();

      unsigned പൂർണ്ണസംഖ്യ cbdt_len = cbdt.get_length ();
      എങ്കിൽ (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	തിരികെ hb_blob_get_empty ();

      തിരഞ്ഞെടുപ്പ് (image_format)
      {
      അവസ്ഥ 17:
      {
	എങ്കിൽ (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  തിരികെ hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (ഇത്->cbdt, image_offset);
	തിരികെ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      അവസ്ഥ 18:
      {
	എങ്കിൽ (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  തിരികെ hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (ഇത്->cbdt, image_offset);
	തിരികെ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      അവസ്ഥ 19:
      {
	എങ്കിൽ (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  തിരികെ hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (ഇത്->cbdt, image_offset);
	തിരികെ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      സ്ഥിരസ്ഥിതി: തിരികെ hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    തർക്കം has_data () സ്ഥിരം { തിരികെ cbdt->version.major; }

    തർക്കം paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ശൂന്യം *data) സ്ഥിരം
    {
      എങ്കിൽ (!has_data ()) തിരികെ അസത്യം;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      എങ്കിൽ (unlikely (!font->get_glyph_extents (glyph, &extents, അസത്യം)))
        തിരികെ അസത്യം;

      എങ്കിൽ (unlikely (!get_extents (font, glyph, &pixel_extents, അസത്യം)))
        തിരികെ അസത്യം;

      hb_blob_t *blob = reference_png (font, glyph);
      എങ്കിൽ (unlikely (hb_blob_is_immutable (blob)))
        തിരികെ അസത്യം;

      തർക്കം ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      തിരികെ ret;
    }

    സ്വകാര്യ:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned പൂർണ്ണസംഖ്യ upem;
  };

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  സംരക്ഷിത:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  പൊതു:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

വരിയിൽ തർക്കം
CBLC::subset (hb_subset_context_t *c) സ്ഥിരം
{
  TRACE_SUBSET (ഇത്);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<അക്ഷരം> cbdt_അഭാജ്യം;

  auto *cblc_അഭാജ്യം = c->serializer->start_embed<CBLC> ();
  എങ്കിൽ (unlikely (!c->serializer->extend_min (cblc_അഭാജ്യം))) return_trace (അസത്യം);
  cblc_അഭാജ്യം->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned പൂർണ്ണസംഖ്യ cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  എങ്കിൽ (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (അസത്യം);
  }
  _copy_data_to_cbdt (&cbdt_അഭാജ്യം, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  വേണ്ടി (സ്ഥിരം BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (സ്ഥിരം അക്ഷരം *) cbdt, cbdt_length, cblc_അഭാജ്യം, &cbdt_അഭാജ്യം, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_അഭാജ്യം));
}

ഘടന CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
