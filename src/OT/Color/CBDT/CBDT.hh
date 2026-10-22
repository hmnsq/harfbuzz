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

#अन्तर्भाव "../../../hb-open-type.hh"
#अन्तर्भाव "../../../hb-paint.hh"

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


नामाकाश OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
संरचना cbdt_dedup_context_t
{
  संरचना entry_t
  {
    अचिन्हित पूर्णांक base;		/* Offset of the data in cbdt_prime. */
    अचिन्हित पूर्णांक lstart;	/* Start index in lengths. */
    अचिन्हित पूर्णांक lcount;	/* Number of length entries. */
  };

  स्थिर uint64_t hash (अचिन्हित पूर्णांक image_format,
			स्थिरांक hb_vector_t<अक्षर> &data,
			स्थिरांक hb_vector_t<अचिन्हित पूर्णांक> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (data.arrayZ, data.length));
    h = h * 31 + hb_hash (hb_bytes_t ((स्थिरांक अक्षर *) lengths.arrayZ,
				      lengths.length * आकार (अचिन्हित पूर्णांक)));
    परत ((uint64_t) image_format << 32) | h;
  }

  तार्किक find (uint64_t key,
	     स्थिरांक hb_vector_t<अक्षर> &data,
	     स्थिरांक hb_vector_t<अचिन्हित पूर्णांक> &lengths,
	     स्थिरांक hb_vector_t<अक्षर> &cbdt_मूळसंख्या,
	     अचिन्हित पूर्णांक *base /* OUT */) स्थिरांक
  {
    अचिन्हित पूर्णांक *idx_p = nullptr;
    जर (!map.has (key, &idx_p)) परत असत्य;
    स्थिरांक entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    जर (e.lcount != lengths.length) परत असत्य;
    जर (unlikely (e.base + data.length > (अचिन्हित पूर्णांक) cbdt_मूळसंख्या.length)) परत असत्य;
    जर (hb_memcmp (cbdt_मूळसंख्या.arrayZ + e.base, data.arrayZ, data.length)) परत असत्य;
    जर (hb_memcmp (हे->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.length * आकार (अचिन्हित पूर्णांक)))
      परत असत्य;
    *base = e.base;
    परत सत्य;
  }

  तार्किक add (uint64_t key,
	    अचिन्हित पूर्णांक base,
	    स्थिरांक hb_vector_t<अचिन्हित पूर्णांक> &lengths)
  {
    जर (unlikely (entries.in_error () || हे->lengths.in_error () || map.in_error ()))
      परत असत्य;
    entry_t e = {base, हे->lengths.length, lengths.length};
    जर (unlikely (!entries.ढकला_or_fail (e))) परत असत्य;
    साठी (अचिन्हित पूर्णांक length : lengths)
      जर (unlikely (!हे->lengths.ढकला_or_fail (length))) परत असत्य;
    परत map.set (key, entries.length - 1);
  }

  hb_hashmap_t<uint64_t, अचिन्हित पूर्णांक> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<अचिन्हित पूर्णांक> lengths; /* Concatenated per-entry lengths. */
};

संरचना cblc_bitmap_size_subset_context_t
{
  स्थिरांक अक्षर *cbdt;
  अचिन्हित पूर्णांक cbdt_length;
  hb_vector_t<अक्षर> *cbdt_मूळसंख्या;
  अचिन्हित पूर्णांक size;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  अचिन्हित पूर्णांक अंक_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t start_glyph;	/* OUT */
  hb_codepoint_t end_glyph;	/* OUT */
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

स्थिर ओळीत तार्किक
_copy_data_to_cbdt (hb_vector_t<अक्षर> *cbdt_मूळसंख्या,
		    स्थिरांक शून्य        *data,
		    अचिन्हित           length)
{
  अचिन्हित पूर्णांक new_len = cbdt_मूळसंख्या->length + length;
  जर (unlikely (!cbdt_मूळसंख्या->alloc (new_len))) परत असत्य;
  hb_memcpy (cbdt_मूळसंख्या->arrayZ + cbdt_मूळसंख्या->length, data, length);
  cbdt_मूळसंख्या->length = new_len;
  परत सत्य;
}

संरचना SmallGlyphMetrics
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  शून्य get_extents (hb_font_t *font, hb_glyph_extents_t *extents, तार्किक scale) स्थिरांक
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<पूर्णांक> (height);

    जर (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (5);
};

संरचना BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना SBit_ओळ_Metrics
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};


/*
 * Index Subtables.
 */

संरचना IndexSubtableHeader
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  HBUINT16	indexFormat;
  HBUINT16	imageFormat;
  HBUINT32	imageDataOffset;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

ढाँचा <typename OffsetType>
संरचना IndexSubtableFormat1Or3
{
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक glyph_मोजणी) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  offsetArrayZ.sanitize (c, glyph_मोजणी + 1));
  }

  तार्किक get_image_data (अचिन्हित पूर्णांक idx,
		       अचिन्हित पूर्णांक *offset,
		       अचिन्हित पूर्णांक *length) स्थिरांक
  {
    जर (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      परत असत्य;

    *offset = header.imageDataOffset + offsetArrayZ[idx];
    *length = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    परत सत्य;
  }

  तार्किक add_offset (hb_serialize_context_t *c,
		   अचिन्हित पूर्णांक offset,
		   अचिन्हित पूर्णांक *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (हे);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *size += आकार (OffsetType);
    स्वतः *o = c->embed (embedded_offset);
    return_trace ((तार्किक) o);
  }

  IndexSubtableHeader	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, offsetArrayZ);
};

संरचना IndexSubtableFormat1 : IndexSubtableFormat1Or3<HBUINT32> {};
संरचना IndexSubtableFormat3 : IndexSubtableFormat1Or3<HBUINT16> {};

संरचना IndexSubtable
{
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक glyph_मोजणी) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.header.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    निवड (u.header.indexFormat)
    {
    स्थिती 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_मोजणी));
    स्थिती 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_मोजणी));
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }

  तार्किक
  finish_subtable (hb_serialize_context_t *c,
		   अचिन्हित पूर्णांक end_offset,
		   अचिन्हित पूर्णांक अंक_glyphs,
		   अचिन्हित पूर्णांक *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (हे);

    निवड (u.header.indexFormat)
    {
    स्थिती 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, size));
    स्थिती 3: {
      hb_barrier ();
      जर (!u.format3.add_offset (c, end_offset, size))
	return_trace (असत्य);
      जर (!(अंक_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, size));
      return_trace (सत्य);
    }
    // TODO: implement 2, 4, 5.
    स्थिती 2: स्थिती 4:  // No-op.
    स्थिती 5:  // Pad to 32-bit aligned.
    पूर्वनिर्धारित: return_trace (असत्य);
    }
  }

  तार्किक
  add_offset (hb_serialize_context_t *c, अचिन्हित पूर्णांक local_offset,
	      अचिन्हित पूर्णांक *size /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (हे);
    निवड (u.header.indexFormat)
    {
    स्थिती 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, size));
    स्थिती 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, size));
    // TODO: Implement tables 2, 4, 5
    स्थिती 2:  // Should be a no-op.
    स्थिती 4: स्थिती 5:  // Handle sparse cases.
    पूर्वनिर्धारित: return_trace (असत्य);
    }
  }

  तार्किक get_extents (hb_glyph_extents_t *extents HB_UNUSED, तार्किक scale HB_UNUSED) स्थिरांक
  {
    निवड (u.header.indexFormat)
    {
    स्थिती 2: स्थिती 5: /* TODO */
    स्थिती 1: स्थिती 3: स्थिती 4: /* Variable-metrics formats do not have metrics here. */
    पूर्वनिर्धारित:परत (असत्य);
    }
  }

  तार्किक
  get_image_data (अचिन्हित पूर्णांक idx, अचिन्हित पूर्णांक *offset,
		  अचिन्हित पूर्णांक *length, अचिन्हित पूर्णांक *format) स्थिरांक
  {
    *format = u.header.imageFormat;
    निवड (u.header.indexFormat)
    {
    स्थिती 1: hb_barrier (); परत u.format1.get_image_data (idx, offset, length);
    स्थिती 3: hb_barrier (); परत u.format3.get_image_data (idx, offset, length);
    पूर्वनिर्धारित: परत असत्य;
    }
  }

  स्थिरांक IndexSubtableHeader* get_header () स्थिरांक { परत &u.header; }

  शून्य populate_header (अचिन्हित index_format,
			अचिन्हित image_format,
			अचिन्हित पूर्णांक image_data_offset,
			अचिन्हित पूर्णांक *size)
  {
    u.header.indexFormat = index_format;
    u.header.imageFormat = image_format;
    u.header.imageDataOffset = image_data_offset;
    निवड (u.header.indexFormat)
    {
    स्थिती 1: *size += IndexSubtableFormat1::min_size; थांबा;
    स्थिती 3: *size += IndexSubtableFormat3::min_size; थांबा;
    }
  }

  संरक्षित:
  संघ {
  IndexSubtableHeader	header;
  IndexSubtableFormat1	format1;
  IndexSubtableFormat3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (8, header);
};

संरचना IndexSubtableRecord
{
  /* XXX Remove this and fix by not inserting it into vector. */
  IndexSubtableRecord& चालक = (स्थिरांक IndexSubtableRecord &o)
  {
    firstGlyphIndex = o.firstGlyphIndex;
    lastGlyphIndex = o.lastGlyphIndex;
    offsetToSubtable = (अचिन्हित) o.offsetToSubtable;
    प्रतिज्ञा (offsetToSubtable.is_null ());
    परत *हे;
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  firstGlyphIndex <= lastGlyphIndex &&
		  offsetToSubtable.sanitize (c, base, lastGlyphIndex - firstGlyphIndex + 1));
  }

  स्थिरांक IndexSubtable* get_subtable (स्थिरांक शून्य *base) स्थिरांक
  {
    परत &(base+offsetToSubtable);
  }

  तार्किक add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_size_subset_context_t *bitmap_size_context,
			 IndexSubtableRecord *record,
			 स्थिरांक hb_vector_t<hb_pair_t<hb_codepoint_t, स्थिरांक IndexSubtableRecord*>> *lookup, /* IN */
			 स्थिरांक शून्य *base,
			 अचिन्हित पूर्णांक *start /* INOUT */) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    स्वतः *subtable = c->serializer->start_embed<IndexSubtable> ();
    जर (unlikely (!c->serializer->extend_min (subtable))) return_trace (असत्य);

    स्वतः *old_subtable = get_subtable (base);
    स्वतः *old_header = old_subtable->get_header ();
    अचिन्हित पूर्णांक image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<अक्षर> data;
    hb_vector_t<अचिन्हित पूर्णांक> lengths;
    अचिन्हित पूर्णांक अंक_glyphs = 0;
    तार्किक early_exit = असत्य;
    साठी (अचिन्हित पूर्णांक i = *start; i < lookup->length; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      स्थिरांक IndexSubtableRecord *next_record = (*lookup)[i].second;
      स्थिरांक IndexSubtable *next_subtable = next_record->get_subtable (base);
      स्वतः *next_header = next_subtable->get_header ();
      जर (next_header != old_header)
      {
	*start = i;
	early_exit = सत्य;
	थांबा;
      }
      अचिन्हित पूर्णांक अंक_missing = record->add_glyph_for_subset (new_gid);
      साठी (अचिन्हित पूर्णांक j = 0; j < अंक_missing; j++)
	जर (unlikely (!c->serializer->check_success (lengths.ढकला_or_fail (0))))
	  return_trace (असत्य);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      जर (old_gid < next_record->firstGlyphIndex)
	return_trace (असत्य);

      अचिन्हित पूर्णांक old_idx = (अचिन्हित पूर्णांक) old_gid - next_record->firstGlyphIndex;
      अचिन्हित पूर्णांक offset, length, format;
      जर (unlikely (!next_subtable->get_image_data (old_idx, &offset, &length, &format)))
	return_trace (असत्य);
      जर (unlikely (offset > bitmap_size_context->cbdt_length ||
		    bitmap_size_context->cbdt_length - offset < length))
	return_trace (असत्य);
      जर (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (&data,
					    bitmap_size_context->cbdt + offset,
					    length))))
	return_trace (असत्य);
      जर (unlikely (!c->serializer->check_success (lengths.ढकला_or_fail (length))))
	return_trace (असत्य);
      अंक_glyphs += 1 + अंक_missing;
    }
    जर (!early_exit)
      *start = lookup->length;

    /* Find or append the data block for this subtable. */
    अचिन्हित पूर्णांक image_data_offset;
    cbdt_dedup_context_t *dedup = bitmap_size_context->dedup;
    तार्किक shared = असत्य;
    uint64_t key = 0;
    जर (dedup && data.length)
    {
      key = cbdt_dedup_context_t::hash (image_format, data, lengths);
      shared = dedup->find (key, data, lengths,
			    *bitmap_size_context->cbdt_मूळसंख्या,
			    &image_data_offset);
    }
    जर (!shared)
    {
      image_data_offset = bitmap_size_context->cbdt_मूळसंख्या->length;
      जर (unlikely (!c->serializer->check_success (
			_copy_data_to_cbdt (bitmap_size_context->cbdt_मूळसंख्या,
					    data.arrayZ, data.length))))
	return_trace (असत्य);
      जर (dedup && data.length)
	/* The bitmap data is already copied; caching is best-effort. */
	(शून्य) dedup->add (key, image_data_offset, lengths);
    }

    subtable->populate_header (old_header->indexFormat,
			       image_format,
			       image_data_offset,
			       &bitmap_size_context->size);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    अचिन्हित पूर्णांक cumulative = 0;
    साठी (अचिन्हित पूर्णांक length : lengths)
    {
      जर (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_size_context->size)))
	return_trace (असत्य);
      cumulative += length;
    }
    जर (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      अंक_glyphs,
					      &bitmap_size_context->size)))
      return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_size_subset_context_t *bitmap_size_context,
		       स्थिरांक hb_vector_t<hb_pair_t<hb_codepoint_t, स्थिरांक IndexSubtableRecord*>> *lookup, /* IN */
		       स्थिरांक शून्य *base,
		       अचिन्हित पूर्णांक *start, /* INOUT */
		       hb_vector_t<IndexSubtableRecord>* records /* INOUT */) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः snap = c->serializer->snapshot ();
    अचिन्हित पूर्णांक old_size = bitmap_size_context->size;
    अचिन्हित पूर्णांक old_cbdt_मूळसंख्या_length = bitmap_size_context->cbdt_मूळसंख्या->length;

    // Set to invalid state to indicate filling glyphs is not yet started.
    जर (unlikely (!c->serializer->check_success (records->resize (records->length + 1))))
      return_trace (असत्य);

    records->tail ().firstGlyphIndex = 1;
    records->tail ().lastGlyphIndex = 0;
    bitmap_size_context->size += IndexSubtableRecord::min_size;

    c->serializer->push ();

    जर (unlikely (!add_new_subtable (c, bitmap_size_context, &(records->tail ()), lookup, base, start)))
    {
      c->serializer->काढा_discard ();
      c->serializer->revert (snap);
      bitmap_size_context->cbdt_मूळसंख्या->shrink (old_cbdt_मूळसंख्या_length);
      bitmap_size_context->size = old_size;
      records->resize (records->length - 1);
      return_trace (असत्य);
    }

    bitmap_size_context->अंक_tables += 1;
    return_trace (सत्य);
  }

  अचिन्हित पूर्णांक add_glyph_for_subset (hb_codepoint_t gid)
  {
    जर (firstGlyphIndex > lastGlyphIndex)
    {
      firstGlyphIndex = gid;
      lastGlyphIndex = gid;
      परत 0;
    }
    // TODO maybe assert? this shouldn't occur.
    जर (lastGlyphIndex > gid)
      परत 0;
    अचिन्हित पूर्णांक अंक_missing = (अचिन्हित पूर्णांक) (gid - lastGlyphIndex - 1);
    lastGlyphIndex = gid;
    परत अंक_missing;
  }

  तार्किक get_extents (hb_glyph_extents_t *extents, स्थिरांक शून्य *base, तार्किक scale) स्थिरांक
  { परत (base+offsetToSubtable).get_extents (extents, scale); }

  तार्किक get_image_data (अचिन्हित पूर्णांक  gid,
		       स्थिरांक शून्य   *base,
		       अचिन्हित पूर्णांक *offset,
		       अचिन्हित पूर्णांक *length,
		       अचिन्हित पूर्णांक *format) स्थिरांक
  {
    जर (gid < firstGlyphIndex || gid > lastGlyphIndex) परत असत्य;
    परत (base+offsetToSubtable).get_image_data (gid - firstGlyphIndex,
						   offset, length, format);
  }

  HBGlyphID16			firstGlyphIndex;
  HBGlyphID16			lastGlyphIndex;
  Offset32To<IndexSubtable>	offsetToSubtable;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना IndexSubtableArray
{
  मित्र संरचना CBDT;

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक मोजणी) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (indexSubtablesZ.sanitize (c, मोजणी, हे));
  }

  शून्य
  build_lookup (hb_subset_context_t *c, cblc_bitmap_size_subset_context_t *bitmap_size_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		स्थिरांक IndexSubtableRecord*>> *lookup /* OUT */) स्थिरांक
  {
    तार्किक start_glyph_is_set = असत्य;
    अचिन्हित अंक_glyphs = c->plan->अंक_output_glyphs ();
    साठी (hb_codepoint_t new_gid = 0; new_gid < अंक_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      जर (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) सुरूठेवा;

      स्थिरांक IndexSubtableRecord* record = find_table (old_gid, bitmap_size_context->अंक_tables);
      जर (unlikely (!record)) सुरूठेवा;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      अचिन्हित पूर्णांक offset, length, format;
      जर (unlikely (!record->get_image_data (old_gid, हे, &offset, &length, &format))) सुरूठेवा;

      lookup->push (hb_pair_t<hb_codepoint_t, स्थिरांक IndexSubtableRecord*> (new_gid, record));

      जर (!start_glyph_is_set)
      {
	bitmap_size_context->start_glyph = new_gid;
	start_glyph_is_set = सत्य;
      }

      bitmap_size_context->end_glyph = new_gid;
    }
  }

  तार्किक
  subset (hb_subset_context_t *c,
	  cblc_bitmap_size_subset_context_t *bitmap_size_context) स्थिरांक
  {
    TRACE_SUBSET (हे);

    hb_vector_t<hb_pair_t<hb_codepoint_t, स्थिरांक IndexSubtableRecord*>> lookup;
    build_lookup (c, bitmap_size_context, &lookup);
    जर (unlikely (!c->serializer->propagate_error (lookup)))
      परत असत्य;

    bitmap_size_context->size = 0;
    bitmap_size_context->अंक_tables = 0;
    hb_vector_t<IndexSubtableRecord> records;
    साठी (अचिन्हित पूर्णांक start = 0; start < lookup.length;)
    {
      जर (unlikely (!lookup[start].second->add_new_record (c, bitmap_size_context, &lookup, हे, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	साठी (अचिन्हित पूर्णांक i = 0; i < records.length; i++)
	  c->serializer->काढा_discard ();
	return_trace (असत्य);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    साठी (अचिन्हित पूर्णांक i = 0; i < records.length; i++)
      objidxs.push (c->serializer->काढा_pack ());
    साठी (अचिन्हित पूर्णांक i = 0; i < records.length; i++)
    {
      IndexSubtableRecord* record = c->serializer->embed (records[i]);
      जर (unlikely (!record)) return_trace (असत्य);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.length - 1 - i]);
    }
    return_trace (सत्य);
  }

  सार्वजनिक:
  स्थिरांक IndexSubtableRecord* find_table (hb_codepoint_t glyph, अचिन्हित पूर्णांक अंक_Tables) स्थिरांक
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < अंक_Tables; ++i)
    {
      अचिन्हित पूर्णांक firstGlyphIndex = indexSubtablesZ[i].firstGlyphIndex;
      अचिन्हित पूर्णांक lastGlyphIndex = indexSubtablesZ[i].lastGlyphIndex;
      जर (firstGlyphIndex <= glyph && glyph <= lastGlyphIndex)
	परत &indexSubtablesZ[i];
    }
    परत nullptr;
  }

  संरक्षित:
  UnsizedArrayOf<IndexSubtableRecord>	indexSubtablesZ;
};

संरचना BitmapSizeTable
{
  मित्र संरचना CBLC;
  मित्र संरचना CBDT;

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  indexSubtableArrayOffset.sanitize (c, base, संख्या_Of_Index_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  स्थिरांक IndexSubtableRecord *
  find_table (hb_codepoint_t glyph, स्थिरांक शून्य *base, स्थिरांक शून्य **out_base) स्थिरांक
  {
    *out_base = &(base+indexSubtableArrayOffset);
    परत (base+indexSubtableArrayOffset).find_table (glyph, संख्या_Of_Index_Subtables);
  }

  तार्किक
  subset (hb_subset_context_t *c, स्थिरांक शून्य *base,
	  स्थिरांक अक्षर *cbdt, अचिन्हित पूर्णांक cbdt_length,
	  hb_vector_t<अक्षर> *cbdt_मूळसंख्या /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out_table = c->serializer->embed (हे);
    जर (unlikely (!out_table)) return_trace (असत्य);

    cblc_bitmap_size_subset_context_t bitmap_size_context;
    bitmap_size_context.cbdt = cbdt;
    bitmap_size_context.cbdt_length = cbdt_length;
    bitmap_size_context.cbdt_मूळसंख्या = cbdt_मूळसंख्या;
    bitmap_size_context.size = indexTablesSize;
    bitmap_size_context.अंक_tables = संख्या_Of_Index_Subtables;
    bitmap_size_context.start_glyph = 1;
    bitmap_size_context.end_glyph = 0;
    bitmap_size_context.dedup = dedup;

    जर (!out_table->indexSubtableArrayOffset.serialize_subset (c,
							       indexSubtableArrayOffset,
							       base,
							       &bitmap_size_context))
      return_trace (असत्य);
    जर (!bitmap_size_context.size ||
	!bitmap_size_context.अंक_tables ||
	bitmap_size_context.start_glyph > bitmap_size_context.end_glyph)
      return_trace (असत्य);

    out_table->indexTablesSize = bitmap_size_context.size;
    out_table->संख्या_Of_Index_Subtables = bitmap_size_context.अंक_tables;
    out_table->startGlyphIndex = bitmap_size_context.start_glyph;
    out_table->endGlyphIndex = bitmap_size_context.end_glyph;
    return_trace (सत्य);
  }

  संरक्षित:
  NNOffset32To<IndexSubtableArray>
			indexSubtableArrayOffset;
  HBUINT32		indexTablesSize;
  HBUINT32		संख्या_Of_Index_Subtables;
  HBUINT32		colorRef;
  SBit_ओळ_Metrics	horizontal;
  SBit_ओळ_Metrics	vertical;
  HBGlyphID16		startGlyphIndex;
  HBGlyphID16		endGlyphIndex;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

संरचना GlyphBitmapDataFormat17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (9, data);
};

संरचना GlyphBitmapDataFormat18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	data;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (12, data);
};

संरचना GlyphBitmapDataFormat19
{
  Array32Of<HBUINT8>	data;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, data);
};

संरचना CBLC
{
  मित्र संरचना CBDT;

  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  sizeTables.sanitize (c, हे));
  }

  स्थिर तार्किक
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<अक्षर>* cbdt_मूळसंख्या)
  {
    hb_blob_t *cbdt_मूळसंख्या_blob = hb_blob_create (cbdt_मूळसंख्या->arrayZ,
						 cbdt_मूळसंख्या->length,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_मूळसंख्या->arrayZ,
						 hb_free);
    cbdt_मूळसंख्या->init ();  // Leak arrayZ to the blob.
    तार्किक ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_मूळसंख्या_blob);
    hb_blob_destroy (cbdt_मूळसंख्या_blob);
    परत ret;
  }

  तार्किक
  subset_size_table (hb_subset_context_t *c, स्थिरांक BitmapSizeTable& table,
		     स्थिरांक अक्षर *cbdt /* IN */, अचिन्हित पूर्णांक cbdt_length,
		     CBLC *cblc_मूळसंख्या /* INOUT */, hb_vector_t<अक्षर> *cbdt_मूळसंख्या /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) स्थिरांक
  {
    TRACE_SUBSET (हे);
    cblc_मूळसंख्या->sizeTables.len++;

    स्वतः snap = c->serializer->snapshot ();
    स्वतः cbdt_मूळसंख्या_len = cbdt_मूळसंख्या->length;

    जर (!table.subset (c, हे, cbdt, cbdt_length, cbdt_मूळसंख्या, dedup))
    {
      cblc_मूळसंख्या->sizeTables.len--;
      c->serializer->revert (snap);
      cbdt_मूळसंख्या->shrink (cbdt_मूळसंख्या_len);
      return_trace (असत्य);
    }
    return_trace (सत्य);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL तार्किक subset (hb_subset_context_t *c) स्थिरांक;

  संरक्षित:
  स्थिरांक BitmapSizeTable &choose_strike (hb_font_t *font) स्थिरांक
  {
    अचिन्हित मोजणी = sizeTables.len;
    जर (unlikely (!मोजणी))
      परत Null (BitmapSizeTable);

    अचिन्हित पूर्णांक requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    जर (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    अचिन्हित पूर्णांक best_i = 0;
    अचिन्हित पूर्णांक best_ppem = hb_max (sizeTables[0].ppemX, sizeTables[0].ppemY);

    साठी (अचिन्हित पूर्णांक i = 1; i < मोजणी; i++)
    {
      अचिन्हित पूर्णांक ppem = hb_max (sizeTables[i].ppemX, sizeTables[i].ppemY);
      जर ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    परत sizeTables[best_i];
  }

  संरक्षित:
  FixedVersion<>		version;
  Array32Of<BitmapSizeTable>	sizeTables;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, sizeTables);
};

संरचना CBDT
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      हे->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      हे->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      हे->cblc.destroy ();
      हे->cbdt.destroy ();
    }

    तार्किक
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, तार्किक scale = सत्य) स्थिरांक
    {
      स्थिरांक शून्य *base;
      स्थिरांक BitmapSizeTable &strike = हे->cblc->choose_strike (font);
      स्थिरांक IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      जर (!subtable_record || !strike.ppemX || !strike.ppemY)
	परत असत्य;

      जर (subtable_record->get_extents (extents, base, scale))
	परत सत्य;

      अचिन्हित पूर्णांक image_offset = 0, image_length = 0, image_format = 0;
      जर (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	परत असत्य;

      अचिन्हित पूर्णांक cbdt_len = cbdt.get_length ();
      जर (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	परत असत्य;

      निवड (image_format)
      {
      स्थिती 17: {
	जर (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  परत असत्य;
	स्वतः &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (हे->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	थांबा;
      }
      स्थिती 18: {
	जर (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  परत असत्य;
	स्वतः &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (हे->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	थांबा;
      }
      पूर्वनिर्धारित: परत असत्य; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      जर (scale)
      {
	दशांश x_scale = upem / (दशांश) strike.ppemX;
	दशांश y_scale = upem / (दशांश) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      परत सत्य;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) स्थिरांक
    {
      स्थिरांक शून्य *base;
      स्थिरांक BitmapSizeTable &strike = हे->cblc->choose_strike (font);
      स्थिरांक IndexSubtableRecord *subtable_record = strike.find_table (glyph, cblc, &base);
      जर (!subtable_record || !strike.ppemX || !strike.ppemY)
	परत hb_blob_get_empty ();

      अचिन्हित पूर्णांक image_offset = 0, image_length = 0, image_format = 0;
      जर (!subtable_record->get_image_data (glyph, base, &image_offset, &image_length, &image_format))
	परत hb_blob_get_empty ();

      अचिन्हित पूर्णांक cbdt_len = cbdt.get_length ();
      जर (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_length))
	परत hb_blob_get_empty ();

      निवड (image_format)
      {
      स्थिती 17:
      {
	जर (unlikely (image_length < GlyphBitmapDataFormat17::min_size))
	  परत hb_blob_get_empty ();
	स्वतः &glyphFormat17 = StructAtOffset<GlyphBitmapDataFormat17> (हे->cbdt, image_offset);
	परत hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat17::min_size,
					glyphFormat17.data.len);
      }
      स्थिती 18:
      {
	जर (unlikely (image_length < GlyphBitmapDataFormat18::min_size))
	  परत hb_blob_get_empty ();
	स्वतः &glyphFormat18 = StructAtOffset<GlyphBitmapDataFormat18> (हे->cbdt, image_offset);
	परत hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat18::min_size,
					glyphFormat18.data.len);
      }
      स्थिती 19:
      {
	जर (unlikely (image_length < GlyphBitmapDataFormat19::min_size))
	  परत hb_blob_get_empty ();
	स्वतः &glyphFormat19 = StructAtOffset<GlyphBitmapDataFormat19> (हे->cbdt, image_offset);
	परत hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + GlyphBitmapDataFormat19::min_size,
					glyphFormat19.data.len);
      }
      पूर्वनिर्धारित: परत hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    तार्किक has_data () स्थिरांक { परत cbdt->version.major; }

    तार्किक paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, शून्य *data) स्थिरांक
    {
      जर (!has_data ()) परत असत्य;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      जर (unlikely (!font->get_glyph_extents (glyph, &extents, असत्य)))
        परत असत्य;

      जर (unlikely (!get_extents (font, glyph, &pixel_extents, असत्य)))
        परत असत्य;

      hb_blob_t *blob = reference_png (font, glyph);
      जर (unlikely (hb_blob_is_immutable (blob)))
        परत असत्य;

      तार्किक ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      परत ret;
    }

    खाजगी:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    अचिन्हित पूर्णांक upem;
  };

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  संरक्षित:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	dataZ;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, dataZ);
};

ओळीत तार्किक
CBLC::subset (hb_subset_context_t *c) स्थिरांक
{
  TRACE_SUBSET (हे);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<अक्षर> cbdt_मूळसंख्या;

  स्वतः *cblc_मूळसंख्या = c->serializer->start_embed<CBLC> ();
  जर (unlikely (!c->serializer->extend_min (cblc_मूळसंख्या))) return_trace (असत्य);
  cblc_मूळसंख्या->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  अचिन्हित पूर्णांक cbdt_length;
  CBDT* cbdt = (CBDT *) hb_blob_get_data (cbdt_blob, &cbdt_length);
  जर (unlikely (cbdt_length < CBDT::min_size))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (असत्य);
  }
  _copy_data_to_cbdt (&cbdt_मूळसंख्या, cbdt, CBDT::min_size);

  cbdt_dedup_context_t dedup;
  साठी (स्थिरांक BitmapSizeTable& table : + sizeTables.iter ())
    subset_size_table (c, table, (स्थिरांक अक्षर *) cbdt, cbdt_length, cblc_मूळसंख्या, &cbdt_मूळसंख्या, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_मूळसंख्या));
}

संरचना CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_CBDT_CBDT_HH */
