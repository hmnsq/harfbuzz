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
কাঠামো cbdt_dedup_context_t
{
  কাঠামো entry_t
  {
    unsigned পূর্ণসংখ্যা base;		/* Offset of the data in cbdt_prime. */
    unsigned পূর্ণসংখ্যা lstart;	/* Start index in lengths. */
    unsigned পূর্ণসংখ্যা lcount;	/* Number of length entries. */
  };

  স্থির uint64_t hash (unsigned পূর্ণসংখ্যা image_format,
			ধ্রুবক hb_vector_t<অক্ষর> &তথ্য,
			ধ্রুবক hb_vector_t<unsigned পূর্ণসংখ্যা> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (তথ্য.arrayZ, তথ্য.দৈর্ঘ্য));
    h = h * 31 + hb_hash (hb_bytes_t ((ধ্রুবক অক্ষর *) lengths.arrayZ,
				      lengths.দৈর্ঘ্য * আকার (unsigned পূর্ণসংখ্যা)));
    ফেরত ((uint64_t) image_format << 32) | h;
  }

  যৌক্তিক find (uint64_t key,
	     ধ্রুবক hb_vector_t<অক্ষর> &তথ্য,
	     ধ্রুবক hb_vector_t<unsigned পূর্ণসংখ্যা> &lengths,
	     ধ্রুবক hb_vector_t<অক্ষর> &cbdt_মৌলিক,
	     unsigned পূর্ণসংখ্যা *base /* OUT */) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা *idx_p = nullptr;
    যদি (!map.has (key, &idx_p)) ফেরত মিথ্যা;
    ধ্রুবক entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    যদি (e.lcount != lengths.দৈর্ঘ্য) ফেরত মিথ্যা;
    যদি (unlikely (e.base + তথ্য.দৈর্ঘ্য > (unsigned পূর্ণসংখ্যা) cbdt_মৌলিক.দৈর্ঘ্য)) ফেরত মিথ্যা;
    যদি (hb_memcmp (cbdt_মৌলিক.arrayZ + e.base, তথ্য.arrayZ, তথ্য.দৈর্ঘ্য)) ফেরত মিথ্যা;
    যদি (hb_memcmp (এই->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.দৈর্ঘ্য * আকার (unsigned পূর্ণসংখ্যা)))
      ফেরত মিথ্যা;
    *base = e.base;
    ফেরত সত্য;
  }

  যৌক্তিক add (uint64_t key,
	    unsigned পূর্ণসংখ্যা base,
	    ধ্রুবক hb_vector_t<unsigned পূর্ণসংখ্যা> &lengths)
  {
    যদি (unlikely (entries.in_error () || এই->lengths.in_error () || map.in_error ()))
      ফেরত মিথ্যা;
    entry_t e = {base, এই->lengths.দৈর্ঘ্য, lengths.দৈর্ঘ্য};
    যদি (unlikely (!entries.ঠেলো_or_fail (e))) ফেরত মিথ্যা;
    জন্য (unsigned পূর্ণসংখ্যা দৈর্ঘ্য : lengths)
      যদি (unlikely (!এই->lengths.ঠেলো_or_fail (দৈর্ঘ্য))) ফেরত মিথ্যা;
    ফেরত map.set (key, entries.দৈর্ঘ্য - 1);
  }

  hb_hashmap_t<uint64_t, unsigned পূর্ণসংখ্যা> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<unsigned পূর্ণসংখ্যা> lengths; /* Concatenated per-entry lengths. */
};

কাঠামো cblc_bitmap_আকার_subset_context_t
{
  ধ্রুবক অক্ষর *cbdt;
  unsigned পূর্ণসংখ্যা cbdt_দৈর্ঘ্য;
  hb_vector_t<অক্ষর> *cbdt_মৌলিক;
  unsigned পূর্ণসংখ্যা আকার_2;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  unsigned পূর্ণসংখ্যা অঙ্ক_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

স্থির অন্তঃরেখ যৌক্তিক
copy_তথ্য_to_cbdt (hb_vector_t<অক্ষর> *cbdt_মৌলিক,
		    ধ্রুবক শূন্য        *তথ্য,
		    unsigned           দৈর্ঘ্য)
{
  unsigned পূর্ণসংখ্যা new_len = cbdt_মৌলিক->দৈর্ঘ্য + দৈর্ঘ্য;
  যদি (unlikely (!cbdt_মৌলিক->alloc (new_len))) ফেরত মিথ্যা;
  hb_memcpy (cbdt_মৌলিক->arrayZ + cbdt_মৌলিক->দৈর্ঘ্য, তথ্য, দৈর্ঘ্য);
  cbdt_মৌলিক->দৈর্ঘ্য = new_len;
  ফেরত সত্য;
}

কাঠামো SmallGlyphMetrics
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য get_extents (hb_font_t *font, hb_glyph_extents_t *extents, যৌক্তিক scale) ধ্রুবক
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<পূর্ণসংখ্যা> (height);

    যদি (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  সর্বজনীন:
  DEFINE_আকার_STATIC (5);
};

কাঠামো BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো SBit_সারি_Metrics
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};


/*
 * Index Subtables.
 */

কাঠামো সূচক_Subtable_Header
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  HBUINT16	সূচক_Format;
  HBUINT16	imageFormat;
  HBUINT32	image_তথ্য_Offset;
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

template <typename OffsetType>
কাঠামো সূচক_Subtable_Format1_Or3
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা glyph_গুনতি) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  offsetArrayZ.sanitize (c, glyph_গুনতি + 1));
  }

  যৌক্তিক get_image_তথ্য (unsigned পূর্ণসংখ্যা idx,
		       unsigned পূর্ণসংখ্যা *offset,
		       unsigned পূর্ণসংখ্যা *দৈর্ঘ্য) ধ্রুবক
  {
    যদি (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      ফেরত মিথ্যা;

    *offset = header.image_তথ্য_Offset + offsetArrayZ[idx];
    *দৈর্ঘ্য = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    ফেরত সত্য;
  }

  যৌক্তিক add_offset (hb_serialize_context_t *c,
		   unsigned পূর্ণসংখ্যা offset,
		   unsigned পূর্ণসংখ্যা *আকার_2 /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (এই);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *আকার_2 += আকার (OffsetType);
    auto *o = c->embed (embedded_offset);
    return_trace ((যৌক্তিক) o);
  }

  সূচক_Subtable_Header	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, offsetArrayZ);
};

কাঠামো সূচক_Subtable_Format1 : সূচক_Subtable_Format1_Or3<HBUINT32> {};
কাঠামো সূচক_Subtable_Format3 : সূচক_Subtable_Format1_Or3<HBUINT16> {};

কাঠামো সূচক_Subtable
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা glyph_গুনতি) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.header.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.header.সূচক_Format)
    {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_গুনতি));
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_গুনতি));
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  যৌক্তিক
  finish_subtable (hb_serialize_context_t *c,
		   unsigned পূর্ণসংখ্যা end_offset,
		   unsigned পূর্ণসংখ্যা অঙ্ক_glyphs,
		   unsigned পূর্ণসংখ্যা *আকার_2 /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (এই);

    নির্বাচন (u.header.সূচক_Format)
    {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, আকার_2));
    ক্ষেত্র 3: {
      hb_barrier ();
      যদি (!u.format3.add_offset (c, end_offset, আকার_2))
	return_trace (মিথ্যা);
      যদি (!(অঙ্ক_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, আকার_2));
      return_trace (সত্য);
    }
    // TODO: implement 2, 4, 5.
    ক্ষেত্র 2: ক্ষেত্র 4:  // No-op.
    ক্ষেত্র 5:  // Pad to 32-bit aligned.
    পূর্বনির্ধারিত: return_trace (মিথ্যা);
    }
  }

  যৌক্তিক
  add_offset (hb_serialize_context_t *c, unsigned পূর্ণসংখ্যা local_offset,
	      unsigned পূর্ণসংখ্যা *আকার_2 /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (এই);
    নির্বাচন (u.header.সূচক_Format)
    {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, আকার_2));
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, আকার_2));
    // TODO: Implement tables 2, 4, 5
    ক্ষেত্র 2:  // Should be a no-op.
    ক্ষেত্র 4: ক্ষেত্র 5:  // Handle sparse cases.
    পূর্বনির্ধারিত: return_trace (মিথ্যা);
    }
  }

  যৌক্তিক get_extents (hb_glyph_extents_t *extents HB_UNUSED, যৌক্তিক scale HB_UNUSED) ধ্রুবক
  {
    নির্বাচন (u.header.সূচক_Format)
    {
    ক্ষেত্র 2: ক্ষেত্র 5: /* TODO */
    ক্ষেত্র 1: ক্ষেত্র 3: ক্ষেত্র 4: /* Variable-metrics formats do not have metrics here. */
    পূর্বনির্ধারিত:ফেরত (মিথ্যা);
    }
  }

  যৌক্তিক
  get_image_তথ্য (unsigned পূর্ণসংখ্যা idx, unsigned পূর্ণসংখ্যা *offset,
		  unsigned পূর্ণসংখ্যা *দৈর্ঘ্য, unsigned পূর্ণসংখ্যা *format) ধ্রুবক
  {
    *format = u.header.imageFormat;
    নির্বাচন (u.header.সূচক_Format)
    {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_image_তথ্য (idx, offset, দৈর্ঘ্য);
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_image_তথ্য (idx, offset, দৈর্ঘ্য);
    পূর্বনির্ধারিত: ফেরত মিথ্যা;
    }
  }

  ধ্রুবক সূচক_Subtable_Header* get_header () ধ্রুবক { ফেরত &u.header; }

  শূন্য populate_header (unsigned সূচক_format,
			unsigned image_format,
			unsigned পূর্ণসংখ্যা image_তথ্য_offset,
			unsigned পূর্ণসংখ্যা *আকার_2)
  {
    u.header.সূচক_Format = সূচক_format;
    u.header.imageFormat = image_format;
    u.header.image_তথ্য_Offset = image_তথ্য_offset;
    নির্বাচন (u.header.সূচক_Format)
    {
    ক্ষেত্র 1: *আকার_2 += সূচক_Subtable_Format1::min_আকার; বিরতি;
    ক্ষেত্র 3: *আকার_2 += সূচক_Subtable_Format3::min_আকার; বিরতি;
    }
  }

  সুরক্ষিত:
  সংঘ {
  সূচক_Subtable_Header	header;
  সূচক_Subtable_Format1	format1;
  সূচক_Subtable_Format3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (8, header);
};

কাঠামো সূচক_Subtable_Record
{
  /* XXX Remove this and fix by not inserting it into vector. */
  সূচক_Subtable_Record& operator = (ধ্রুবক সূচক_Subtable_Record &o)
  {
    first_Glyph_সূচক = o.first_Glyph_সূচক;
    last_Glyph_সূচক = o.last_Glyph_সূচক;
    offsetToSubtable = (unsigned) o.offsetToSubtable;
    প্রতিজ্ঞা (offsetToSubtable.is_null ());
    ফেরত *এই;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  first_Glyph_সূচক <= last_Glyph_সূচক &&
		  offsetToSubtable.sanitize (c, base, last_Glyph_সূচক - first_Glyph_সূচক + 1));
  }

  ধ্রুবক সূচক_Subtable* get_subtable (ধ্রুবক শূন্য *base) ধ্রুবক
  {
    ফেরত &(base+offsetToSubtable);
  }

  যৌক্তিক add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_আকার_subset_context_t *bitmap_আকার_context,
			 সূচক_Subtable_Record *record,
			 ধ্রুবক hb_vector_t<hb_pair_t<hb_codepoint_t, ধ্রুবক সূচক_Subtable_Record*>> *lookup, /* IN */
			 ধ্রুবক শূন্য *base,
			 unsigned পূর্ণসংখ্যা *start /* INOUT */) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    auto *subtable = c->serializer->start_embed<সূচক_Subtable> ();
    যদি (unlikely (!c->serializer->extend_min (subtable))) return_trace (মিথ্যা);

    auto *old_subtable = get_subtable (base);
    auto *old_header = old_subtable->get_header ();
    unsigned পূর্ণসংখ্যা image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<অক্ষর> তথ্য;
    hb_vector_t<unsigned পূর্ণসংখ্যা> lengths;
    unsigned পূর্ণসংখ্যা অঙ্ক_glyphs = 0;
    যৌক্তিক early_exit = মিথ্যা;
    জন্য (unsigned পূর্ণসংখ্যা i = *start; i < lookup->দৈর্ঘ্য; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      ধ্রুবক সূচক_Subtable_Record *পরবর্তী_record = (*lookup)[i].second;
      ধ্রুবক সূচক_Subtable *পরবর্তী_subtable = পরবর্তী_record->get_subtable (base);
      auto *পরবর্তী_header = পরবর্তী_subtable->get_header ();
      যদি (পরবর্তী_header != old_header)
      {
	*start = i;
	early_exit = সত্য;
	বিরতি;
      }
      unsigned পূর্ণসংখ্যা অঙ্ক_missing = record->add_glyph_for_subset (new_gid);
      জন্য (unsigned পূর্ণসংখ্যা j = 0; j < অঙ্ক_missing; j++)
	যদি (unlikely (!c->serializer->check_success (lengths.ঠেলো_or_fail (0))))
	  return_trace (মিথ্যা);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      যদি (old_gid < পরবর্তী_record->first_Glyph_সূচক)
	return_trace (মিথ্যা);

      unsigned পূর্ণসংখ্যা old_idx = (unsigned পূর্ণসংখ্যা) old_gid - পরবর্তী_record->first_Glyph_সূচক;
      unsigned পূর্ণসংখ্যা offset, দৈর্ঘ্য, format;
      যদি (unlikely (!পরবর্তী_subtable->get_image_তথ্য (old_idx, &offset, &দৈর্ঘ্য, &format)))
	return_trace (মিথ্যা);
      যদি (unlikely (offset > bitmap_আকার_context->cbdt_দৈর্ঘ্য ||
		    bitmap_আকার_context->cbdt_দৈর্ঘ্য - offset < দৈর্ঘ্য))
	return_trace (মিথ্যা);
      যদি (unlikely (!c->serializer->check_success (
			copy_তথ্য_to_cbdt (&তথ্য,
					    bitmap_আকার_context->cbdt + offset,
					    দৈর্ঘ্য))))
	return_trace (মিথ্যা);
      যদি (unlikely (!c->serializer->check_success (lengths.ঠেলো_or_fail (দৈর্ঘ্য))))
	return_trace (মিথ্যা);
      অঙ্ক_glyphs += 1 + অঙ্ক_missing;
    }
    যদি (!early_exit)
      *start = lookup->দৈর্ঘ্য;

    /* Find or append the data block for this subtable. */
    unsigned পূর্ণসংখ্যা image_তথ্য_offset;
    cbdt_dedup_context_t *dedup = bitmap_আকার_context->dedup;
    যৌক্তিক shared = মিথ্যা;
    uint64_t key = 0;
    যদি (dedup && তথ্য.দৈর্ঘ্য)
    {
      key = cbdt_dedup_context_t::hash (image_format, তথ্য, lengths);
      shared = dedup->find (key, তথ্য, lengths,
			    *bitmap_আকার_context->cbdt_মৌলিক,
			    &image_তথ্য_offset);
    }
    যদি (!shared)
    {
      image_তথ্য_offset = bitmap_আকার_context->cbdt_মৌলিক->দৈর্ঘ্য;
      যদি (unlikely (!c->serializer->check_success (
			copy_তথ্য_to_cbdt (bitmap_আকার_context->cbdt_মৌলিক,
					    তথ্য.arrayZ, তথ্য.দৈর্ঘ্য))))
	return_trace (মিথ্যা);
      যদি (dedup && তথ্য.দৈর্ঘ্য)
	/* The bitmap data is already copied; caching is best-effort. */
	(শূন্য) dedup->add (key, image_তথ্য_offset, lengths);
    }

    subtable->populate_header (old_header->সূচক_Format,
			       image_format,
			       image_তথ্য_offset,
			       &bitmap_আকার_context->আকার_2);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    unsigned পূর্ণসংখ্যা cumulative = 0;
    জন্য (unsigned পূর্ণসংখ্যা দৈর্ঘ্য : lengths)
    {
      যদি (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_আকার_context->আকার_2)))
	return_trace (মিথ্যা);
      cumulative += দৈর্ঘ্য;
    }
    যদি (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      অঙ্ক_glyphs,
					      &bitmap_আকার_context->আকার_2)))
      return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  যৌক্তিক add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_আকার_subset_context_t *bitmap_আকার_context,
		       ধ্রুবক hb_vector_t<hb_pair_t<hb_codepoint_t, ধ্রুবক সূচক_Subtable_Record*>> *lookup, /* IN */
		       ধ্রুবক শূন্য *base,
		       unsigned পূর্ণসংখ্যা *start, /* INOUT */
		       hb_vector_t<সূচক_Subtable_Record>* records /* INOUT */) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto snap = c->serializer->snapshot ();
    unsigned পূর্ণসংখ্যা old_আকার = bitmap_আকার_context->আকার_2;
    unsigned পূর্ণসংখ্যা old_cbdt_মৌলিক_দৈর্ঘ্য = bitmap_আকার_context->cbdt_মৌলিক->দৈর্ঘ্য;

    // Set to invalid state to indicate filling glyphs is not yet started.
    যদি (unlikely (!c->serializer->check_success (records->resize (records->দৈর্ঘ্য + 1))))
      return_trace (মিথ্যা);

    records->লেজ ().first_Glyph_সূচক = 1;
    records->লেজ ().last_Glyph_সূচক = 0;
    bitmap_আকার_context->আকার_2 += সূচক_Subtable_Record::min_আকার;

    c->serializer->push ();

    যদি (unlikely (!add_new_subtable (c, bitmap_আকার_context, &(records->লেজ ()), lookup, base, start)))
    {
      c->serializer->বের_করো_discard ();
      c->serializer->revert (snap);
      bitmap_আকার_context->cbdt_মৌলিক->shrink (old_cbdt_মৌলিক_দৈর্ঘ্য);
      bitmap_আকার_context->আকার_2 = old_আকার;
      records->resize (records->দৈর্ঘ্য - 1);
      return_trace (মিথ্যা);
    }

    bitmap_আকার_context->অঙ্ক_tables += 1;
    return_trace (সত্য);
  }

  unsigned পূর্ণসংখ্যা add_glyph_for_subset (hb_codepoint_t gid)
  {
    যদি (first_Glyph_সূচক > last_Glyph_সূচক)
    {
      first_Glyph_সূচক = gid;
      last_Glyph_সূচক = gid;
      ফেরত 0;
    }
    // TODO maybe assert? this shouldn't occur.
    যদি (last_Glyph_সূচক > gid)
      ফেরত 0;
    unsigned পূর্ণসংখ্যা অঙ্ক_missing = (unsigned পূর্ণসংখ্যা) (gid - last_Glyph_সূচক - 1);
    last_Glyph_সূচক = gid;
    ফেরত অঙ্ক_missing;
  }

  যৌক্তিক get_extents (hb_glyph_extents_t *extents, ধ্রুবক শূন্য *base, যৌক্তিক scale) ধ্রুবক
  { ফেরত (base+offsetToSubtable).get_extents (extents, scale); }

  যৌক্তিক get_image_তথ্য (unsigned পূর্ণসংখ্যা  gid,
		       ধ্রুবক শূন্য   *base,
		       unsigned পূর্ণসংখ্যা *offset,
		       unsigned পূর্ণসংখ্যা *দৈর্ঘ্য,
		       unsigned পূর্ণসংখ্যা *format) ধ্রুবক
  {
    যদি (gid < first_Glyph_সূচক || gid > last_Glyph_সূচক) ফেরত মিথ্যা;
    ফেরত (base+offsetToSubtable).get_image_তথ্য (gid - first_Glyph_সূচক,
						   offset, দৈর্ঘ্য, format);
  }

  HBGlyphID16			first_Glyph_সূচক;
  HBGlyphID16			last_Glyph_সূচক;
  Offset32To<সূচক_Subtable>	offsetToSubtable;
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো সূচক_Subtable_Array
{
  friend কাঠামো CBDT;

  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা গুনতি) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (সূচক_Subtables_Z.sanitize (c, গুনতি, এই));
  }

  শূন্য
  build_lookup (hb_subset_context_t *c, cblc_bitmap_আকার_subset_context_t *bitmap_আকার_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		ধ্রুবক সূচক_Subtable_Record*>> *lookup /* OUT */) ধ্রুবক
  {
    যৌক্তিক start_glyph_is_set = মিথ্যা;
    unsigned অঙ্ক_glyphs = c->plan->অঙ্ক_output_glyphs ();
    জন্য (hb_codepoint_t new_gid = 0; new_gid < অঙ্ক_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      যদি (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) চালু;

      ধ্রুবক সূচক_Subtable_Record* record = find_table (old_gid, bitmap_আকার_context->অঙ্ক_tables);
      যদি (unlikely (!record)) চালু;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      unsigned পূর্ণসংখ্যা offset, দৈর্ঘ্য, format;
      যদি (unlikely (!record->get_image_তথ্য (old_gid, এই, &offset, &দৈর্ঘ্য, &format))) চালু;

      lookup->push (hb_pair_t<hb_codepoint_t, ধ্রুবক সূচক_Subtable_Record*> (new_gid, record));

      যদি (!start_glyph_is_set)
      {
	bitmap_আকার_context->start_glyph = new_gid;
	start_glyph_is_set = সত্য;
      }

      bitmap_আকার_context->end_glyph = new_gid;
    }
  }

  যৌক্তিক
  subset (hb_subset_context_t *c,
	  cblc_bitmap_আকার_subset_context_t *bitmap_আকার_context) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    hb_vector_t<hb_pair_t<hb_codepoint_t, ধ্রুবক সূচক_Subtable_Record*>> lookup;
    build_lookup (c, bitmap_আকার_context, &lookup);
    যদি (unlikely (!c->serializer->propagate_error (lookup)))
      ফেরত মিথ্যা;

    bitmap_আকার_context->আকার_2 = 0;
    bitmap_আকার_context->অঙ্ক_tables = 0;
    hb_vector_t<সূচক_Subtable_Record> records;
    জন্য (unsigned পূর্ণসংখ্যা start = 0; start < lookup.দৈর্ঘ্য;)
    {
      যদি (unlikely (!lookup[start].second->add_new_record (c, bitmap_আকার_context, &lookup, এই, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < records.দৈর্ঘ্য; i++)
	  c->serializer->বের_করো_discard ();
	return_trace (মিথ্যা);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < records.দৈর্ঘ্য; i++)
      objidxs.push (c->serializer->বের_করো_pack ());
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < records.দৈর্ঘ্য; i++)
    {
      সূচক_Subtable_Record* record = c->serializer->embed (records[i]);
      যদি (unlikely (!record)) return_trace (মিথ্যা);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.দৈর্ঘ্য - 1 - i]);
    }
    return_trace (সত্য);
  }

  সর্বজনীন:
  ধ্রুবক সূচক_Subtable_Record* find_table (hb_codepoint_t glyph, unsigned পূর্ণসংখ্যা অঙ্ক_Tables) ধ্রুবক
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_Tables; ++i)
    {
      unsigned পূর্ণসংখ্যা first_Glyph_সূচক = সূচক_Subtables_Z[i].first_Glyph_সূচক;
      unsigned পূর্ণসংখ্যা last_Glyph_সূচক = সূচক_Subtables_Z[i].last_Glyph_সূচক;
      যদি (first_Glyph_সূচক <= glyph && glyph <= last_Glyph_সূচক)
	ফেরত &সূচক_Subtables_Z[i];
    }
    ফেরত nullptr;
  }

  সুরক্ষিত:
  UnsizedArrayOf<সূচক_Subtable_Record>	সূচক_Subtables_Z;
};

কাঠামো Bitmap_আকার_Table
{
  friend কাঠামো CBLC;
  friend কাঠামো CBDT;

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  সূচক_Subtable_Array_Offset.sanitize (c, base, সংখ্যা_Of_সূচক_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ধ্রুবক সূচক_Subtable_Record *
  find_table (hb_codepoint_t glyph, ধ্রুবক শূন্য *base, ধ্রুবক শূন্য **out_base) ধ্রুবক
  {
    *out_base = &(base+সূচক_Subtable_Array_Offset);
    ফেরত (base+সূচক_Subtable_Array_Offset).find_table (glyph, সংখ্যা_Of_সূচক_Subtables);
  }

  যৌক্তিক
  subset (hb_subset_context_t *c, ধ্রুবক শূন্য *base,
	  ধ্রুবক অক্ষর *cbdt, unsigned পূর্ণসংখ্যা cbdt_দৈর্ঘ্য,
	  hb_vector_t<অক্ষর> *cbdt_মৌলিক /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out_table = c->serializer->embed (এই);
    যদি (unlikely (!out_table)) return_trace (মিথ্যা);

    cblc_bitmap_আকার_subset_context_t bitmap_আকার_context;
    bitmap_আকার_context.cbdt = cbdt;
    bitmap_আকার_context.cbdt_দৈর্ঘ্য = cbdt_দৈর্ঘ্য;
    bitmap_আকার_context.cbdt_মৌলিক = cbdt_মৌলিক;
    bitmap_আকার_context.আকার_2 = সূচক_Tables_আকার;
    bitmap_আকার_context.অঙ্ক_tables = সংখ্যা_Of_সূচক_Subtables;
    bitmap_আকার_context.start_glyph = 1;
    bitmap_আকার_context.end_glyph = 0;
    bitmap_আকার_context.dedup = dedup;

    যদি (!out_table->সূচক_Subtable_Array_Offset.serialize_subset (c,
							       সূচক_Subtable_Array_Offset,
							       base,
							       &bitmap_আকার_context))
      return_trace (মিথ্যা);
    যদি (!bitmap_আকার_context.আকার_2 ||
	!bitmap_আকার_context.অঙ্ক_tables ||
	bitmap_আকার_context.start_glyph > bitmap_আকার_context.end_glyph)
      return_trace (মিথ্যা);

    out_table->সূচক_Tables_আকার = bitmap_আকার_context.আকার_2;
    out_table->সংখ্যা_Of_সূচক_Subtables = bitmap_আকার_context.অঙ্ক_tables;
    out_table->start_Glyph_সূচক = bitmap_আকার_context.start_glyph;
    out_table->end_Glyph_সূচক = bitmap_আকার_context.end_glyph;
    return_trace (সত্য);
  }

  সুরক্ষিত:
  NNOffset32To<সূচক_Subtable_Array>
			সূচক_Subtable_Array_Offset;
  HBUINT32		সূচক_Tables_আকার;
  HBUINT32		সংখ্যা_Of_সূচক_Subtables;
  HBUINT32		colorRef;
  SBit_সারি_Metrics	horizontal;
  SBit_সারি_Metrics	vertical;
  HBGlyphID16		start_Glyph_সূচক;
  HBGlyphID16		end_Glyph_সূচক;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  সর্বজনীন:
  DEFINE_আকার_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

কাঠামো Glyph_Bitmap_তথ্য_Format17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	তথ্য;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (9, তথ্য);
};

কাঠামো Glyph_Bitmap_তথ্য_Format18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	তথ্য;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (12, তথ্য);
};

কাঠামো Glyph_Bitmap_তথ্য_Format19
{
  Array32Of<HBUINT8>	তথ্য;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, তথ্য);
};

কাঠামো CBLC
{
  friend কাঠামো CBDT;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  আকার_Tables.sanitize (c, এই));
  }

  স্থির যৌক্তিক
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<অক্ষর>* cbdt_মৌলিক)
  {
    hb_blob_t *cbdt_মৌলিক_blob = hb_blob_create (cbdt_মৌলিক->arrayZ,
						 cbdt_মৌলিক->দৈর্ঘ্য,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_মৌলিক->arrayZ,
						 hb_free);
    cbdt_মৌলিক->init ();  // Leak arrayZ to the blob.
    যৌক্তিক ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_মৌলিক_blob);
    hb_blob_destroy (cbdt_মৌলিক_blob);
    ফেরত ret;
  }

  যৌক্তিক
  subset_আকার_table (hb_subset_context_t *c, ধ্রুবক Bitmap_আকার_Table& table,
		     ধ্রুবক অক্ষর *cbdt /* IN */, unsigned পূর্ণসংখ্যা cbdt_দৈর্ঘ্য,
		     CBLC *cblc_মৌলিক /* INOUT */, hb_vector_t<অক্ষর> *cbdt_মৌলিক /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    cblc_মৌলিক->আকার_Tables.len++;

    auto snap = c->serializer->snapshot ();
    auto cbdt_মৌলিক_len = cbdt_মৌলিক->দৈর্ঘ্য;

    যদি (!table.subset (c, এই, cbdt, cbdt_দৈর্ঘ্য, cbdt_মৌলিক, dedup))
    {
      cblc_মৌলিক->আকার_Tables.len--;
      c->serializer->revert (snap);
      cbdt_মৌলিক->shrink (cbdt_মৌলিক_len);
      return_trace (মিথ্যা);
    }
    return_trace (সত্য);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক;

  সুরক্ষিত:
  ধ্রুবক Bitmap_আকার_Table &choose_strike (hb_font_t *font) ধ্রুবক
  {
    unsigned গুনতি = আকার_Tables.len;
    যদি (unlikely (!গুনতি))
      ফেরত Null (Bitmap_আকার_Table);

    unsigned পূর্ণসংখ্যা requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    যদি (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    unsigned পূর্ণসংখ্যা best_i = 0;
    unsigned পূর্ণসংখ্যা best_ppem = hb_max (আকার_Tables[0].ppemX, আকার_Tables[0].ppemY);

    জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
    {
      unsigned পূর্ণসংখ্যা ppem = hb_max (আকার_Tables[i].ppemX, আকার_Tables[i].ppemY);
      যদি ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    ফেরত আকার_Tables[best_i];
  }

  সুরক্ষিত:
  FixedVersion<>		version;
  Array32Of<Bitmap_আকার_Table>	আকার_Tables;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, আকার_Tables);
};

কাঠামো CBDT
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  কাঠামো accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      এই->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      এই->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      এই->cblc.destroy ();
      এই->cbdt.destroy ();
    }

    যৌক্তিক
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, যৌক্তিক scale = সত্য) ধ্রুবক
    {
      ধ্রুবক শূন্য *base;
      ধ্রুবক Bitmap_আকার_Table &strike = এই->cblc->choose_strike (font);
      ধ্রুবক সূচক_Subtable_Record *subtable_record = strike.find_table (glyph, cblc, &base);
      যদি (!subtable_record || !strike.ppemX || !strike.ppemY)
	ফেরত মিথ্যা;

      যদি (subtable_record->get_extents (extents, base, scale))
	ফেরত সত্য;

      unsigned পূর্ণসংখ্যা image_offset = 0, image_দৈর্ঘ্য = 0, image_format = 0;
      যদি (!subtable_record->get_image_তথ্য (glyph, base, &image_offset, &image_দৈর্ঘ্য, &image_format))
	ফেরত মিথ্যা;

      unsigned পূর্ণসংখ্যা cbdt_len = cbdt.get_দৈর্ঘ্য ();
      যদি (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_দৈর্ঘ্য))
	ফেরত মিথ্যা;

      নির্বাচন (image_format)
      {
      ক্ষেত্র 17: {
	যদি (unlikely (image_দৈর্ঘ্য < Glyph_Bitmap_তথ্য_Format17::min_আকার))
	  ফেরত মিথ্যা;
	auto &glyphFormat17 = StructAtOffset<Glyph_Bitmap_তথ্য_Format17> (এই->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	বিরতি;
      }
      ক্ষেত্র 18: {
	যদি (unlikely (image_দৈর্ঘ্য < Glyph_Bitmap_তথ্য_Format18::min_আকার))
	  ফেরত মিথ্যা;
	auto &glyphFormat18 = StructAtOffset<Glyph_Bitmap_তথ্য_Format18> (এই->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	বিরতি;
      }
      পূর্বনির্ধারিত: ফেরত মিথ্যা; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      যদি (scale)
      {
	দশমিক x_scale = upem / (দশমিক) strike.ppemX;
	দশমিক y_scale = upem / (দশমিক) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      ফেরত সত্য;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ধ্রুবক
    {
      ধ্রুবক শূন্য *base;
      ধ্রুবক Bitmap_আকার_Table &strike = এই->cblc->choose_strike (font);
      ধ্রুবক সূচক_Subtable_Record *subtable_record = strike.find_table (glyph, cblc, &base);
      যদি (!subtable_record || !strike.ppemX || !strike.ppemY)
	ফেরত hb_blob_get_empty ();

      unsigned পূর্ণসংখ্যা image_offset = 0, image_দৈর্ঘ্য = 0, image_format = 0;
      যদি (!subtable_record->get_image_তথ্য (glyph, base, &image_offset, &image_দৈর্ঘ্য, &image_format))
	ফেরত hb_blob_get_empty ();

      unsigned পূর্ণসংখ্যা cbdt_len = cbdt.get_দৈর্ঘ্য ();
      যদি (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_দৈর্ঘ্য))
	ফেরত hb_blob_get_empty ();

      নির্বাচন (image_format)
      {
      ক্ষেত্র 17:
      {
	যদি (unlikely (image_দৈর্ঘ্য < Glyph_Bitmap_তথ্য_Format17::min_আকার))
	  ফেরত hb_blob_get_empty ();
	auto &glyphFormat17 = StructAtOffset<Glyph_Bitmap_তথ্য_Format17> (এই->cbdt, image_offset);
	ফেরত hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + Glyph_Bitmap_তথ্য_Format17::min_আকার,
					glyphFormat17.তথ্য.len);
      }
      ক্ষেত্র 18:
      {
	যদি (unlikely (image_দৈর্ঘ্য < Glyph_Bitmap_তথ্য_Format18::min_আকার))
	  ফেরত hb_blob_get_empty ();
	auto &glyphFormat18 = StructAtOffset<Glyph_Bitmap_তথ্য_Format18> (এই->cbdt, image_offset);
	ফেরত hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + Glyph_Bitmap_তথ্য_Format18::min_আকার,
					glyphFormat18.তথ্য.len);
      }
      ক্ষেত্র 19:
      {
	যদি (unlikely (image_দৈর্ঘ্য < Glyph_Bitmap_তথ্য_Format19::min_আকার))
	  ফেরত hb_blob_get_empty ();
	auto &glyphFormat19 = StructAtOffset<Glyph_Bitmap_তথ্য_Format19> (এই->cbdt, image_offset);
	ফেরত hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + Glyph_Bitmap_তথ্য_Format19::min_আকার,
					glyphFormat19.তথ্য.len);
      }
      পূর্বনির্ধারিত: ফেরত hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত cbdt->version.major; }

    যৌক্তিক paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, শূন্য *তথ্য) ধ্রুবক
    {
      যদি (!has_তথ্য ()) ফেরত মিথ্যা;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      যদি (unlikely (!font->get_glyph_extents (glyph, &extents, মিথ্যা)))
        ফেরত মিথ্যা;

      যদি (unlikely (!get_extents (font, glyph, &pixel_extents, মিথ্যা)))
        ফেরত মিথ্যা;

      hb_blob_t *blob = reference_png (font, glyph);
      যদি (unlikely (hb_blob_is_immutable (blob)))
        ফেরত মিথ্যা;

      যৌক্তিক ret = funcs->image (তথ্য,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      ফেরত ret;
    }

    ব্যক্তিগত:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    unsigned পূর্ণসংখ্যা upem;
  };

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  সুরক্ষিত:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	তথ্য_Z;
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, তথ্য_Z);
};

অন্তঃরেখ যৌক্তিক
CBLC::subset (hb_subset_context_t *c) ধ্রুবক
{
  TRACE_SUBSET (এই);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<অক্ষর> cbdt_মৌলিক;

  auto *cblc_মৌলিক = c->serializer->start_embed<CBLC> ();
  যদি (unlikely (!c->serializer->extend_min (cblc_মৌলিক))) return_trace (মিথ্যা);
  cblc_মৌলিক->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  unsigned পূর্ণসংখ্যা cbdt_দৈর্ঘ্য;
  CBDT* cbdt = (CBDT *) hb_blob_get_তথ্য (cbdt_blob, &cbdt_দৈর্ঘ্য);
  যদি (unlikely (cbdt_দৈর্ঘ্য < CBDT::min_আকার))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (মিথ্যা);
  }
  copy_তথ্য_to_cbdt (&cbdt_মৌলিক, cbdt, CBDT::min_আকার);

  cbdt_dedup_context_t dedup;
  জন্য (ধ্রুবক Bitmap_আকার_Table& table : + আকার_Tables.iter ())
    subset_আকার_table (c, table, (ধ্রুবক অক্ষর *) cbdt, cbdt_দৈর্ঘ্য, cblc_মৌলিক, &cbdt_মৌলিক, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_মৌলিক));
}

কাঠামো CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
