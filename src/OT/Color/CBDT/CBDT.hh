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

#ಅನ್ತರ್ಭಾವ "../../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-paint.hh"

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


ನಾಮಾಕಾಶ OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
ಸಂರಚನಾ cbdt_dedup_context_t
{
  ಸಂರಚನಾ entry_t
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ base;		/* Offset of the data in cbdt_prime. */
    unsigned ಪೂರ್ಣಾಙ್ಕ lstart;	/* Start index in lengths. */
    unsigned ಪೂರ್ಣಾಙ್ಕ lcount;	/* Number of length entries. */
  };

  ಸ್ಥಿರ uint64_t hash (unsigned ಪೂರ್ಣಾಙ್ಕ image_format,
			ಧ್ರುವ hb_vector_t<ವರ್ಣ> &data,
			ಧ್ರುವ hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((ಧ್ರುವ ವರ್ಣ *) lengths.arrayZ,
				      lengths.length * ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ)));
    ಪ್ರತ್ಯಾಗಮ ((uint64_t) image_format << 32) | h;
  }

  ತಾರ್ಕಿಕ find (uint64_t key,
	     ಧ್ರುವ hb_vector_t<ವರ್ಣ> &data,
	     ಧ್ರುವ hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> &lengths,
	     ಧ್ರುವ hb_vector_t<ವರ್ಣ> &cbdt_ಅವಿಭಾಜ್ಯ,
	     unsigned ಪೂರ್ಣಾಙ್ಕ *base /* OUT */) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ *idx_p = nullptr;
    ಯದಿ (!map.has (key, &idx_p)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    ಯದಿ (e.lcount != lengths.length) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (unlikely (e.base + data.length > (unsigned ಪೂರ್ಣಾಙ್ಕ) cbdt_ಅವಿಭಾಜ್ಯ.length)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (hb_memcmp (cbdt_ಅವಿಭಾಜ್ಯ.arrayZ + e.base, data.arrayZ, data.length)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (hb_memcmp (ಏತತ್->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * ಪರಿಮಾಣ (unsigned ಪೂರ್ಣಾಙ್ಕ)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *base = e.base;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ add (uint64_t key,
	    unsigned ಪೂರ್ಣಾಙ್ಕ base,
	    ಧ್ರುವ hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> &lengths)
  {
    ಯದಿ (unlikely (entries.in_error () || ಏತತ್->lengths.in_error () || map.in_error ()))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    entry_t e = {base, ಏತತ್->lengths.length, lengths.length};
    ಯದಿ (unlikely (!entries.ತಳ್ಳು_or_fail (e))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ length : lengths)
      ಯದಿ (unlikely (!ಏತತ್->lengths.ತಳ್ಳು_or_fail (length))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, unsigned ಪೂರ್ಣಾಙ್ಕ> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> lengths; /* Concatenated per-entry lengths. */
};

ಸಂರಚನಾ cblc_bitmap_size_subset_context_t
{
  ಧ್ರುವ ವರ್ಣ *cbdt;
  unsigned ಪೂರ್ಣಾಙ್ಕ cbdt_length;
  hb_vector_t<ವರ್ಣ> *cbdt_ಅವಿಭಾಜ್ಯ;
  unsigned ಪೂರ್ಣಾಙ್ಕ size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
_copy_data_to_cbdt (hb_vector_t<ವರ್ಣ> *cbdt_ಅವಿಭಾಜ್ಯ,
		    ಧ್ರುವ ಶೂನ್ಯ        *data,
		    unsigned           length)
{
  unsigned ಪೂರ್ಣಾಙ್ಕ new_len = cbdt_ಅವಿಭಾಜ್ಯ->length + length;
  ಯದಿ (unlikely (!cbdt_ಅವಿಭಾಜ್ಯ->alloc (new_len))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  hb_memcpy (cbdt_ಅವಿಭಾಜ್ಯ->arrayZ + cbdt_ಅವಿಭಾಜ್ಯ->length, data, length);
  cbdt_ಅವಿಭಾಜ್ಯ->length = new_len;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸಂರಚನಾ SmallGlyphMetrics
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, ತಾರ್ಕಿಕ scale) ಧ್ರುವ
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<ಪೂರ್ಣಾಙ್ಕ> (height);

    ಯದಿ (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (5);
};

ಸಂರಚನಾ BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ SBit_ಸಾಲು_Metrics
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

ಸಂರಚನಾ IndexSubtableHeader
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

template <typename OffsetType>
ಸಂರಚನಾ IndexSubtableFormat1Or3
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ glyph_ಎಣಿಕೆ) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  offsetArrayZ.sanitize (c, glyph_ಎಣಿಕೆ + 1));
  }

  ತಾರ್ಕಿಕ get_image_data (unsigned ಪೂರ್ಣಾಙ್ಕ idx,
		       unsigned ಪೂರ್ಣಾಙ್ಕ *offset,
		       unsigned ಪೂರ್ಣಾಙ್ಕ *length) ಧ್ರುವ
  {
    ಯದಿ (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ add_offset (hb_serialize_context_t *c,
		   unsigned ಪೂರ್ಣಾಙ್ಕ offset,
		   unsigned ಪೂರ್ಣಾಙ್ಕ *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ಏತತ್);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += ಪರಿಮಾಣ (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((ತಾರ್ಕಿಕ) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

ಸಂರಚನಾ IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
ಸಂರಚನಾ IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

ಸಂರಚನಾ IndexSubtable
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ glyph_ಎಣಿಕೆ) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.header.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.header.indexFormat)
    {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_ಎಣಿಕೆ));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_ಎಣಿಕೆ));
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  ತಾರ್ಕಿಕ
  finish_subtable (hb_serialize_context_t *c,
		   unsigned ಪೂರ್ಣಾಙ್ಕ end_offset,
		   unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs,
		   unsigned ಪೂರ್ಣಾಙ್ಕ *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ಏತತ್);

    ವಿಕಲ್ಪ (u.header.indexFormat)
    {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    ಪಕ್ಷ 3: {
      hb_barrier ();
      ಯದಿ (!u.format3.add_offset (c, end_offset, size))
	return_trace (ಅಸತ್ಯ);
      ಯದಿ (!(ಅಂಕೆ_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (ಸತ್ಯ);
    }
    // TODO: implement 2, 4, 5.
    ಪಕ್ಷ 2: ಪಕ್ಷ 4:  // No-op.
    ಪಕ್ಷ 5:  // Pad to 32-bit aligned.
    ಸ್ವಭಾವ: return_trace (ಅಸತ್ಯ);
    }
  }

  ತಾರ್ಕಿಕ
  add_offset (hb_serialize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ local_offset,
	      unsigned ಪೂರ್ಣಾಙ್ಕ *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ವಿಕಲ್ಪ (u.header.indexFormat)
    {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    ಪಕ್ಷ 2:  // Should be a no-op.
    ಪಕ್ಷ 4: ಪಕ್ಷ 5:  // Handle sparse cases.
    ಸ್ವಭಾವ: return_trace (ಅಸತ್ಯ);
    }
  }

  ತಾರ್ಕಿಕ get_extents (hb_glyph_extents_t *extents HB_UNUSED, ತಾರ್ಕಿಕ scale HB_UNUSED) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.header.indexFormat)
    {
    ಪಕ್ಷ 2: ಪಕ್ಷ 5: /* TODO */
    ಪಕ್ಷ 1: ಪಕ್ಷ 3: ಪಕ್ಷ 4: /* Variable-metrics formats do not have metrics here. */
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ (ಅಸತ್ಯ);
    }
  }

  ತಾರ್ಕಿಕ
  get_image_data (unsigned ಪೂರ್ಣಾಙ್ಕ idx, unsigned ಪೂರ್ಣಾಙ್ಕ *offset,
		  unsigned ಪೂರ್ಣಾಙ್ಕ *length, unsigned ಪೂರ್ಣಾಙ್ಕ *format) ಧ್ರುವ
  {
    *format = u.header.imageFormat;
    ವಿಕಲ್ಪ (u.header.indexFormat)
    {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_image_data (idx, offset, length);
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_image_data (idx, offset, length);
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ಧ್ರುವ IndexSubtableHeader* get_header () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ &u.header; }

  ಶೂನ್ಯ populate_header (unsigned index_format,
			unsigned image_format,
			unsigned ಪೂರ್ಣಾಙ್ಕ image_data_offset,
			unsigned ಪೂರ್ಣಾಙ್ಕ *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    ವಿಕಲ್ಪ (u.header.indexFormat)
    {
    ಪಕ್ಷ 1: *size += IndexSubtableFormat1::min_size; ಭಙ್ಗ;
    ಪಕ್ಷ 3: *size += IndexSubtableFormat3::min_size; ಭಙ್ಗ;
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (8, header);
};

ಸಂರಚನಾ IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& operator = (ಧ್ರುವ IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    ಪ್ರತಿಜ್ಞಾ (offsetToSubtable.is_null ());
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  ಧ್ರುವ IndexSubtable* get_subtable (ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ &(base+offsetToSubtable);
  }

  ತಾರ್ಕಿಕ add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 ಧ್ರುವ hb_vector_t<hb_pair_t<hb_codepoint_t, ಧ್ರುವ IndexSubtableRecord*>> *lookup, /* IN */
			 ಧ್ರುವ ಶೂನ್ಯ *base,
			 unsigned ಪೂರ್ಣಾಙ್ಕ *start /* INOUT */) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    auto *subtable = c->serializer->start_embed<IndexSubtable> ();
    ಯದಿ (unlikely (!c->serializer->extend_min (subtable))) return_trace (ಅಸತ್ಯ);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned ಪೂರ್ಣಾಙ್ಕ image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<ವರ್ಣ> data;
    hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> lengths;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs = 0;
    ತಾರ್ಕಿಕ early_exit = ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      ಧ್ರುವ IndexSubtableRecord *next_record = (*lookup)[i].second;
      ಧ್ರುವ IndexSubtable *next_subtable = next_record->get_subtable (base);
      auto *next_header = next_subtable->get_header ();
      ಯದಿ (next_header != old_header)
      {
	*start = i;
	early_exit = ಸತ್ಯ;
	ಭಙ್ಗ;
      }
      unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_missing = record->add_glyph_for_subset (new_gid);
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < ಅಂಕೆ_missing; j++)
	ಯದಿ (unlikely (!c->serializer->check_success (lengths.ತಳ್ಳು_or_fail (0))))
	  return_trace (ಅಸತ್ಯ);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      ಯದಿ (old_gid < next_record->firstGlyphIndex)
	return_trace (ಅಸತ್ಯ);

      unsigned ಪೂರ್ಣಾಙ್ಕ old_idx = (unsigned ಪೂರ್ಣಾಙ್ಕ) old_gid - next_record->firstGlyphIndex;
      unsigned ಪೂರ್ಣಾಙ್ಕ offset, length, format;
      ಯದಿ (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (ಅಸತ್ಯ);
      ಯದಿ (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (ಅಸತ್ಯ);
      ಯದಿ (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (ಅಸತ್ಯ);
      ಯದಿ (unlikely (!c->serializer->check_success (lengths.ತಳ್ಳು_or_fail (length))))
	return_trace (ಅಸತ್ಯ);
      ಅಂಕೆ_glyphs += 1 + ಅಂಕೆ_missing;
    }
    ಯದಿ (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    unsigned ಪೂರ್ಣಾಙ್ಕ image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    ತಾರ್ಕಿಕ shared = ಅಸತ್ಯ;
    uint64_t key = 0;
    ಯದಿ (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_ಅವಿಭಾಜ್ಯ,
			    &image_data_offset);
    }
    ಯದಿ (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_ಅವಿಭಾಜ್ಯ->length;
      ಯದಿ (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_ಅವಿಭಾಜ್ಯ,
					    data.arrayZ, data.length))))
	return_trace (ಅಸತ್ಯ);
      ಯದಿ (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(ಶೂನ್ಯ) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned ಪೂರ್ಣಾಙ್ಕ cumulative = 0;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ length : lengths)
    {
      ಯದಿ (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (ಅಸತ್ಯ);
      cumulative += length;
    }
    ಯದಿ (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      ಅಂಕೆ_glyphs,
					      &bitmap_size_context->size)))
      return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       ಧ್ರುವ hb_vector_t<hb_pair_t<hb_codepoint_t, ಧ್ರುವ IndexSubtableRecord*>> *lookup, /* IN */
		       ಧ್ರುವ ಶೂನ್ಯ *base,
		       unsigned ಪೂರ್ಣಾಙ್ಕ *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto snap = c->serializer->snapshot ();
    unsigned ಪೂರ್ಣಾಙ್ಕ old_size = bitmap_size_context->size;
    unsigned ಪೂರ್ಣಾಙ್ಕ old_cbdt_ಅವಿಭಾಜ್ಯ_length = bitmap_size_context->cbdt_ಅವಿಭಾಜ್ಯ->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    ಯದಿ (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (ಅಸತ್ಯ);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    ಯದಿ (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->ತೆಗೆ_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_ಅವಿಭಾಜ್ಯ->shrink (old_cbdt_ಅವಿಭಾಜ್ಯ_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (ಅಸತ್ಯ);
    }

    bitmap_size_context->ಅಂಕೆ_tables += 1;
    return_trace (ಸತ್ಯ);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ add_glyph_for_subset (hb_codepoint_t gid)
  {
    ಯದಿ (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      ಪ್ರತ್ಯಾಗಮ 0;
    }
    // TODO maybe assert? this shouldn't occur.
    ಯದಿ (lastGlyphIndex > gid)
      ಪ್ರತ್ಯಾಗಮ 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_missing = (unsigned ಪೂರ್ಣಾಙ್ಕ) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_missing;
  }

  ತಾರ್ಕಿಕ get_extents (hb_glyph_extents_t *extents, ಧ್ರುವ ಶೂನ್ಯ *base, ತಾರ್ಕಿಕ scale) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (base+offsetToSubtable).get_extents (extents, scale); }

  ತಾರ್ಕಿಕ get_image_data (unsigned ಪೂರ್ಣಾಙ್ಕ  gid,
		       ಧ್ರುವ ಶೂನ್ಯ   *base,
		       unsigned ಪೂರ್ಣಾಙ್ಕ *offset,
		       unsigned ಪೂರ್ಣಾಙ್ಕ *length,
		       unsigned ಪೂರ್ಣಾಙ್ಕ *format) ಧ್ರುವ
  {
    ಯದಿ (gid < firstGlyphIndex || gid > lastGlyphIndex) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ IndexSubtableArray
{
  friend ಸಂರಚನಾ CBDT;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (indexSubtablesZ.sanitize (c, ಎಣಿಕೆ, ಏತತ್));
  }

  ಶೂನ್ಯ
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		ಧ್ರುವ IndexSubtableRecord*>> *lookup /* OUT */) ಧ್ರುವ
  {
    ತಾರ್ಕಿಕ start_glyph_is_set = ಅಸತ್ಯ;
    unsigned ಅಂಕೆ_glyphs = c->plan->ಅಂಕೆ_output_glyphs ();
    ಪ್ರತಿ (hb_codepoint_t new_gid = 0; new_gid < ಅಂಕೆ_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      ಯದಿ (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) ಅನುವರ್ತ;

      ಧ್ರುವ IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->ಅಂಕೆ_tables);
      ಯದಿ (unlikely (!record)) ಅನುವರ್ತ;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned ಪೂರ್ಣಾಙ್ಕ offset, length, format;
      ಯದಿ (unlikely (!record->get_image_data (old_gid, ಏತತ್, &offset, &length, &format))) ಅನುವರ್ತ;

      lookup->push (hb_pair_t<hb_codepoint_t, ಧ್ರುವ IndexSubtableRecord*> (new_gid, record));

      ಯದಿ (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = ಸತ್ಯ;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  ತಾರ್ಕಿಕ
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    hb_vector_t<hb_pair_t<hb_codepoint_t, ಧ್ರುವ IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    ಯದಿ (unlikely (!c->serializer->propagate_error (lookup)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    bitmap_size_context->size = 0;
    bitmap_size_context->ಅಂಕೆ_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ start = 0; start < lookup.length;)
    {
      ಯದಿ (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, ಏತತ್, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < records.length; i++)
	  c->serializer->ತೆಗೆ_discard ();
	return_trace (ಅಸತ್ಯ);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < records.length; i++)
      objidxs.push (c->serializer->ತೆಗೆ_pack ());
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      ಯದಿ (unlikely (!record)) return_trace (ಅಸತ್ಯ);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (ಸತ್ಯ);
  }

  ಸಾರ್ವಜನಿಕ:
  ಧ್ರುವ IndexSubtableRecord* find_table (hb_codepoint_t glyph, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_Tables) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_Tables; ++i)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      unsigned ಪೂರ್ಣಾಙ್ಕ lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      ಯದಿ (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	ಪ್ರತ್ಯಾಗಮ &indexSubtablesZ[i];
    }
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ರಕ್ಷಿತ:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

ಸಂರಚನಾ BitmapSizeTable
{
  friend ಸಂರಚನಾ CBLC;
  friend ಸಂರಚನಾ CBDT;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, ಸಂಖ್ಯೆ_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ಧ್ರುವ IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, ಧ್ರುವ ಶೂನ್ಯ *base, ಧ್ರುವ ಶೂನ್ಯ **out_base) ಧ್ರುವ
  {
    *out_base = &(base+indexSubtableArrayOffset);
    ಪ್ರತ್ಯಾಗಮ (base+indexSubtableArrayOffset).find_table (glyph, ಸಂಖ್ಯೆ_Of_Index_Subtables);
  }

  ತಾರ್ಕಿಕ
  subset (hb_subset_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base,
	  ಧ್ರುವ ವರ್ಣ *cbdt, unsigned ಪೂರ್ಣಾಙ್ಕ cbdt_length,
	  hb_vector_t<ವರ್ಣ> *cbdt_ಅವಿಭಾಜ್ಯ /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out_table = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out_table)) return_trace (ಅಸತ್ಯ);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_ಅವಿಭಾಜ್ಯ = cbdt_ಅವಿಭಾಜ್ಯ;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.ಅಂಕೆ_tables = ಸಂಖ್ಯೆ_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    ಯದಿ (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (ಅಸತ್ಯ);
    ಯದಿ (!bitmap_size_context.size ||
	!bitmap_size_context.ಅಂಕೆ_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (ಅಸತ್ಯ);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->ಸಂಖ್ಯೆ_Of_Index_Subtables = bitmap_size_context.ಅಂಕೆ_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (ಸತ್ಯ);
  }

  ರಕ್ಷಿತ:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		ಸಂಖ್ಯೆ_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_ಸಾಲು_Metrics	horizontal;
  SBit_ಸಾಲು_Metrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

ಸಂರಚನಾ GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (9, data);
};

ಸಂರಚನಾ GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (12, data);
};

ಸಂರಚನಾ GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, data);
};

ಸಂರಚನಾ CBLC
{
  friend ಸಂರಚನಾ CBDT;

  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, ಏತತ್));
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<ವರ್ಣ>* cbdt_ಅವಿಭಾಜ್ಯ)
  {
    hb_blob_t *cbdt_ಅವಿಭಾಜ್ಯ_blob = hb_blob_create (cbdt_ಅವಿಭಾಜ್ಯ->arrayZ,
						 cbdt_ಅವಿಭಾಜ್ಯ->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_ಅವಿಭಾಜ್ಯ->arrayZ,
						 hb_free);
    cbdt_ಅವಿಭಾಜ್ಯ->init ();  // Leak arrayZ to the blob.
    ತಾರ್ಕಿಕ ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_ಅವಿಭಾಜ್ಯ_blob);
    hb_blob_destroy (cbdt_ಅವಿಭಾಜ್ಯ_blob);
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ತಾರ್ಕಿಕ
  subset_size_table (hb_subset_context_t *c, ಧ್ರುವ BitmapSizeTable& table,
		     ಧ್ರುವ ವರ್ಣ *cbdt /* IN */, unsigned ಪೂರ್ಣಾಙ್ಕ cbdt_length,
		     CBLC *cblc_ಅವಿಭಾಜ್ಯ /* INOUT */, hb_vector_t<ವರ್ಣ> *cbdt_ಅವಿಭಾಜ್ಯ /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    cblc_ಅವಿಭಾಜ್ಯ->sizeTables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_ಅವಿಭಾಜ್ಯ_len = cbdt_ಅವಿಭಾಜ್ಯ->length;

    ಯದಿ (!table.subset (c, ಏತತ್, cbdt, cbdt_length, cbdt_ಅವಿಭಾಜ್ಯ, dedup))
    {
      cblc_ಅವಿಭಾಜ್ಯ->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_ಅವಿಭಾಜ್ಯ->shrink (cbdt_ಅವಿಭಾಜ್ಯ_len);
      return_trace (ಅಸತ್ಯ);
    }
    return_trace (ಸತ್ಯ);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ;

  ರಕ್ಷಿತ:
  ಧ್ರುವ BitmapSizeTable &choose_strike (hb_font_t *font) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = sizeTables.len;
    ಯದಿ (unlikely (!ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ Null (BitmapSizeTable);

    unsigned ಪೂರ್ಣಾಙ್ಕ requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    ಯದಿ (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned ಪೂರ್ಣಾಙ್ಕ best_i = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ಎಣಿಕೆ; i++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      ಯದಿ ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    ಪ್ರತ್ಯಾಗಮ sizeTables[best_i];
  }

  ರಕ್ಷಿತ:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

ಸಂರಚನಾ CBDT
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ಏತತ್->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      ಏತತ್->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      ಏತತ್->cblc.destroy ();
      ಏತತ್->cbdt.destroy ();
    }

    ತಾರ್ಕಿಕ
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, ತಾರ್ಕಿಕ scale = ಸತ್ಯ) ಧ್ರುವ
    {
      ಧ್ರುವ ಶೂನ್ಯ *base;
      ಧ್ರುವ BitmapSizeTable &strike = ಏತತ್->cblc->choose_strike (font);
      ಧ್ರುವ IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      ಯದಿ (!subtable_record || !strike.ppemX || !strike.ppemY)
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (subtable_record->get_extents (extents, base, scale))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      unsigned ಪೂರ್ಣಾಙ್ಕ image_offset = 0, image_length = 0, image_format = 0;
      ಯದಿ (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      unsigned ಪೂರ್ಣಾಙ್ಕ cbdt_len = cbdt.get_length ();
      ಯದಿ (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ವಿಕಲ್ಪ (image_format)
      {
      ಪಕ್ಷ 17: {
	ಯದಿ (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (ಏತತ್->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	ಭಙ್ಗ;
      }
      ಪಕ್ಷ 18: {
	ಯದಿ (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (ಏತತ್->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	ಭಙ್ಗ;
      }
      ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      ಯದಿ (scale)
      {
	ಭಿನ್ನ x_scale = upem / (ಭಿನ್ನ) strike.ppemX;
	ಭಿನ್ನ y_scale = upem / (ಭಿನ್ನ) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ಧ್ರುವ
    {
      ಧ್ರುವ ಶೂನ್ಯ *base;
      ಧ್ರುವ BitmapSizeTable &strike = ಏತತ್->cblc->choose_strike (font);
      ಧ್ರುವ IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      ಯದಿ (!subtable_record || !strike.ppemX || !strike.ppemY)
	ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();

      unsigned ಪೂರ್ಣಾಙ್ಕ image_offset = 0, image_length = 0, image_format = 0;
      ಯದಿ (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();

      unsigned ಪೂರ್ಣಾಙ್ಕ cbdt_len = cbdt.get_length ();
      ಯದಿ (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();

      ವಿಕಲ್ಪ (image_format)
      {
      ಪಕ್ಷ 17:
      {
	ಯದಿ (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (ಏತತ್->cbdt, image_offset);
	ಪ್ರತ್ಯಾಗಮ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      ಪಕ್ಷ 18:
      {
	ಯದಿ (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (ಏತತ್->cbdt, image_offset);
	ಪ್ರತ್ಯಾಗಮ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      ಪಕ್ಷ 19:
      {
	ಯದಿ (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (ಏತತ್->cbdt, image_offset);
	ಪ್ರತ್ಯಾಗಮ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ cbdt->version.major; }

    ತಾರ್ಕಿಕ paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ಶೂನ್ಯ *data) ಧ್ರುವ
    {
      ಯದಿ (!has_data ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      ಯದಿ (unlikely (!font->get_glyph_extents (glyph, &extents, ಅಸತ್ಯ)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (unlikely (!get_extents (font, glyph, &pixel_extents, ಅಸತ್ಯ)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      hb_blob_t *blob = reference_png (font, glyph);
      ಯದಿ (unlikely (hb_blob_is_immutable (blob)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ತಾರ್ಕಿಕ ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      ಪ್ರತ್ಯಾಗಮ ret;
    }

    ಗುಪ್ತ:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned ಪೂರ್ಣಾಙ್ಕ upem;
  };

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  ರಕ್ಷಿತ:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
CBLC::subset (hb_subset_context_t *c) ಧ್ರುವ
{
  TRACE_SUBSET (ಏತತ್);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<ವರ್ಣ> cbdt_ಅವಿಭಾಜ್ಯ;

  auto *cblc_ಅವಿಭಾಜ್ಯ = c->serializer->start_embed<CBLC> ();
  ಯದಿ (unlikely (!c->serializer->extend_min (cblc_ಅವಿಭಾಜ್ಯ))) return_trace (ಅಸತ್ಯ);
  cblc_ಅವಿಭಾಜ್ಯ->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned ಪೂರ್ಣಾಙ್ಕ cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  ಯದಿ (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (ಅಸತ್ಯ);
  }
  _copy_data_to_cbdt (&cbdt_ಅವಿಭಾಜ್ಯ, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  ಪ್ರತಿ (ಧ್ರುವ BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (ಧ್ರುವ ವರ್ಣ *) cbdt, cbdt_length, cblc_ಅವಿಭಾಜ್ಯ, &cbdt_ಅವಿಭಾಜ್ಯ, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_ಅವಿಭಾಜ್ಯ));
}

ಸಂರಚನಾ CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
