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

#अगर_अघोषित OT_COLOR_CBDT_CBDT_HH
#घोषणा OT_COLOR_CBDT_CBDT_HH

#समावेश "../../../hb-open-type.hh"
#समावेश "../../../hb-paint.hh"

/*
 * CBLC -- Color Bitmap Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cblc
 * https://docs.microsoft.com/en-us/typography/opentype/spec/eblc
 * CBDT -- Color Bitmap Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cbdt
 * https://docs.microsoft.com/en-us/typography/opentype/spec/ebdt
 */
#घोषणा HB_OT_TAG_CBLC HB_TAG('C','B','L','C')
#घोषणा HB_OT_TAG_CBDT HB_TAG('C','B','D','T')


namespace OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
काष्ठा cbdt_dedup_context_t
{
  काष्ठा entry_t
  {
    अचिन्हित पूर्णांक base;		/* Offset of the data in cbdt_prime. */
    अचिन्हित पूर्णांक lstart;	/* Start index in lengths. */
    अचिन्हित पूर्णांक lcount;	/* Number of length entries. */
  };

  अटल uint64_t hash (अचिन्हित पूर्णांक image_format,
			स्थिर hb_vector_t<अक्षर> &तथ्य,
			स्थिर hb_vector_t<अचिन्हित पूर्णांक> &lengths)
  {
    uint32_t h = hb_hash (hb_bytes_t (तथ्य.arrayZ, तथ्य.लंबाई));
    h = h * 31 + hb_hash (hb_bytes_t ((स्थिर अक्षर *) lengths.arrayZ,
				      lengths.लंबाई * माप (अचिन्हित पूर्णांक)));
    वापस ((uint64_t) image_format << 32) | h;
  }

  bool find (uint64_t key,
	     स्थिर hb_vector_t<अक्षर> &तथ्य,
	     स्थिर hb_vector_t<अचिन्हित पूर्णांक> &lengths,
	     स्थिर hb_vector_t<अक्षर> &cbdt_अभाज्य,
	     अचिन्हित पूर्णांक *base /* OUT */) स्थिर
  {
    अचिन्हित पूर्णांक *idx_p = nullptr;
    अगर (!map.has (key, &idx_p)) वापस false;
    स्थिर entry_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    अगर (e.lcount != lengths.लंबाई) वापस false;
    अगर (unlikely (e.base + तथ्य.लंबाई > (अचिन्हित पूर्णांक) cbdt_अभाज्य.लंबाई)) वापस false;
    अगर (hb_memcmp (cbdt_अभाज्य.arrayZ + e.base, तथ्य.arrayZ, तथ्य.लंबाई)) वापस false;
    अगर (hb_memcmp (यह->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.लंबाई * माप (अचिन्हित पूर्णांक)))
      वापस false;
    *base = e.base;
    वापस true;
  }

  bool add (uint64_t key,
	    अचिन्हित पूर्णांक base,
	    स्थिर hb_vector_t<अचिन्हित पूर्णांक> &lengths)
  {
    अगर (unlikely (entries.in_error () || यह->lengths.in_error () || map.in_error ()))
      वापस false;
    entry_t e = {base, यह->lengths.लंबाई, lengths.लंबाई};
    अगर (unlikely (!entries.धकेलो_or_fail (e))) वापस false;
    क्रम (अचिन्हित पूर्णांक लंबाई : lengths)
      अगर (unlikely (!यह->lengths.धकेलो_or_fail (लंबाई))) वापस false;
    वापस map.set (key, entries.लंबाई - 1);
  }

  hb_hashmap_t<uint64_t, अचिन्हित पूर्णांक> map;
  hb_vector_t<entry_t> entries;
  hb_vector_t<अचिन्हित पूर्णांक> lengths; /* Concatenated per-entry lengths. */
};

काष्ठा cblc_bitmap_आकार_subset_context_t
{
  स्थिर अक्षर *cbdt;
  अचिन्हित पूर्णांक cbdt_लंबाई;
  hb_vector_t<अक्षर> *cbdt_अभाज्य;
  अचिन्हित पूर्णांक आकार;		/* INOUT
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

अटल अंतरभूत bool
copy_तथ्य_to_cbdt (hb_vector_t<अक्षर> *cbdt_अभाज्य,
		    स्थिर व्योम        *तथ्य,
		    अचिन्हित           लंबाई)
{
  अचिन्हित पूर्णांक new_len = cbdt_अभाज्य->लंबाई + लंबाई;
  अगर (unlikely (!cbdt_अभाज्य->alloc (new_len))) वापस false;
  hb_memcpy (cbdt_अभाज्य->arrayZ + cbdt_अभाज्य->लंबाई, तथ्य, लंबाई);
  cbdt_अभाज्य->लंबाई = new_len;
  वापस true;
}

काष्ठा SmallGlyphMetrics
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम get_extents (hb_font_t *font, hb_glyph_extents_t *extents, bool scale) स्थिर
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->width = width;
    extents->height = -static_cast<पूर्णांक> (height);

    अगर (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	height;
  HBUINT8	width;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  खुला:
  DEFINE_आकार_STATIC (5);
};

काष्ठा BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा SBit_पंक्ति_Metrics
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
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
  खुला:
  DEFINE_आकार_STATIC (12);
};


/*
 * Index Subtables.
 */

काष्ठा सूचकांक_Subtable_Header
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  HBUINT16	सूचकांक_Format;
  HBUINT16	imageFormat;
  HBUINT32	image_तथ्य_Offset;
  खुला:
  DEFINE_आकार_STATIC (8);
};

ढाँचा <typename OffsetType>
काष्ठा सूचकांक_Subtable_Format1_Or3
{
  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक glyph_गिनती) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  offsetArrayZ.sanitize (c, glyph_गिनती + 1));
  }

  bool get_image_तथ्य (अचिन्हित पूर्णांक idx,
		       अचिन्हित पूर्णांक *offset,
		       अचिन्हित पूर्णांक *लंबाई) स्थिर
  {
    अगर (unlikely (offsetArrayZ[idx + 1] <= offsetArrayZ[idx]))
      वापस false;

    *offset = header.image_तथ्य_Offset + offsetArrayZ[idx];
    *लंबाई = offsetArrayZ[idx + 1] - offsetArrayZ[idx];
    वापस true;
  }

  bool add_offset (hb_serialize_context_t *c,
		   अचिन्हित पूर्णांक offset,
		   अचिन्हित पूर्णांक *आकार /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (यह);
    Offset<OffsetType> embedded_offset;
    embedded_offset = offset;
    *आकार += माप (OffsetType);
    स्वतः *o = c->embed (embedded_offset);
    return_trace ((bool) o);
  }

  सूचकांक_Subtable_Header	header;
  UnsizedArrayOf<Offset<OffsetType>>
			offsetArrayZ;
  खुला:
  DEFINE_आकार_ARRAY (8, offsetArrayZ);
};

काष्ठा सूचकांक_Subtable_Format1 : सूचकांक_Subtable_Format1_Or3<HBUINT32> {};
काष्ठा सूचकांक_Subtable_Format3 : सूचकांक_Subtable_Format1_Or3<HBUINT16> {};

काष्ठा सूचकांक_Subtable
{
  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक glyph_गिनती) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.header.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.header.सूचकांक_Format)
    {
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_गिनती));
    हाल 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_गिनती));
    शेष:return_trace (true);
    }
  }

  bool
  finish_subtable (hb_serialize_context_t *c,
		   अचिन्हित पूर्णांक end_offset,
		   अचिन्हित पूर्णांक अंक_glyphs,
		   अचिन्हित पूर्णांक *आकार /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (यह);

    चयन (u.header.सूचकांक_Format)
    {
    हाल 1: hb_barrier (); return_trace (u.format1.add_offset (c, end_offset, आकार));
    हाल 3: {
      hb_barrier ();
      अगर (!u.format3.add_offset (c, end_offset, आकार))
	return_trace (false);
      अगर (!(अंक_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.add_offset (c, 0, आकार));
      return_trace (true);
    }
    // TODO: implement 2, 4, 5.
    हाल 2: हाल 4:  // No-op.
    हाल 5:  // Pad to 32-bit aligned.
    शेष: return_trace (false);
    }
  }

  bool
  add_offset (hb_serialize_context_t *c, अचिन्हित पूर्णांक local_offset,
	      अचिन्हित पूर्णांक *आकार /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (यह);
    चयन (u.header.सूचकांक_Format)
    {
    हाल 1: hb_barrier (); return_trace (u.format1.add_offset (c, local_offset, आकार));
    हाल 3: hb_barrier (); return_trace (u.format3.add_offset (c, local_offset, आकार));
    // TODO: Implement tables 2, 4, 5
    हाल 2:  // Should be a no-op.
    हाल 4: हाल 5:  // Handle sparse cases.
    शेष: return_trace (false);
    }
  }

  bool get_extents (hb_glyph_extents_t *extents HB_UNUSED, bool scale HB_UNUSED) स्थिर
  {
    चयन (u.header.सूचकांक_Format)
    {
    हाल 2: हाल 5: /* TODO */
    हाल 1: हाल 3: हाल 4: /* Variable-metrics formats do not have metrics here. */
    शेष:वापस (false);
    }
  }

  bool
  get_image_तथ्य (अचिन्हित पूर्णांक idx, अचिन्हित पूर्णांक *offset,
		  अचिन्हित पूर्णांक *लंबाई, अचिन्हित पूर्णांक *format) स्थिर
  {
    *format = u.header.imageFormat;
    चयन (u.header.सूचकांक_Format)
    {
    हाल 1: hb_barrier (); वापस u.format1.get_image_तथ्य (idx, offset, लंबाई);
    हाल 3: hb_barrier (); वापस u.format3.get_image_तथ्य (idx, offset, लंबाई);
    शेष: वापस false;
    }
  }

  स्थिर सूचकांक_Subtable_Header* get_header () स्थिर { वापस &u.header; }

  व्योम populate_header (अचिन्हित सूचकांक_format,
			अचिन्हित image_format,
			अचिन्हित पूर्णांक image_तथ्य_offset,
			अचिन्हित पूर्णांक *आकार)
  {
    u.header.सूचकांक_Format = सूचकांक_format;
    u.header.imageFormat = image_format;
    u.header.image_तथ्य_Offset = image_तथ्य_offset;
    चयन (u.header.सूचकांक_Format)
    {
    हाल 1: *आकार += सूचकांक_Subtable_Format1::min_आकार; अवरोध;
    हाल 3: *आकार += सूचकांक_Subtable_Format3::min_आकार; अवरोध;
    }
  }

  रक्षित:
  जोड़ {
  सूचकांक_Subtable_Header	header;
  सूचकांक_Subtable_Format1	format1;
  सूचकांक_Subtable_Format3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  खुला:
  DEFINE_आकार_UNION (8, header);
};

काष्ठा सूचकांक_Subtable_Record
{
  /* XXX Remove this and fix by not inserting it into vector. */
  सूचकांक_Subtable_Record& चालक = (स्थिर सूचकांक_Subtable_Record &o)
  {
    first_Glyph_सूचकांक = o.first_Glyph_सूचकांक;
    last_Glyph_सूचकांक = o.last_Glyph_सूचकांक;
    offsetToSubtable = (अचिन्हित) o.offsetToSubtable;
    निश्चित (offsetToSubtable.is_null ());
    वापस *यह;
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  first_Glyph_सूचकांक <= last_Glyph_सूचकांक &&
		  offsetToSubtable.sanitize (c, base, last_Glyph_सूचकांक - first_Glyph_सूचकांक + 1));
  }

  स्थिर सूचकांक_Subtable* get_subtable (स्थिर व्योम *base) स्थिर
  {
    वापस &(base+offsetToSubtable);
  }

  bool add_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_आकार_subset_context_t *bitmap_आकार_context,
			 सूचकांक_Subtable_Record *record,
			 स्थिर hb_vector_t<hb_pair_t<hb_codepoint_t, स्थिर सूचकांक_Subtable_Record*>> *lookup, /* IN */
			 स्थिर व्योम *base,
			 अचिन्हित पूर्णांक *start /* INOUT */) स्थिर
  {
    TRACE_SERIALIZE (यह);

    स्वतः *subtable = c->serializer->start_embed<सूचकांक_Subtable> ();
    अगर (unlikely (!c->serializer->extend_min (subtable))) return_trace (false);

    स्वतः *old_subtable = get_subtable (base);
    स्वतः *old_header = old_subtable->get_header ();
    अचिन्हित पूर्णांक image_format = old_header->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_vector_t<अक्षर> तथ्य;
    hb_vector_t<अचिन्हित पूर्णांक> lengths;
    अचिन्हित पूर्णांक अंक_glyphs = 0;
    bool early_exit = false;
    क्रम (अचिन्हित पूर्णांक i = *start; i < lookup->लंबाई; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].first;
      स्थिर सूचकांक_Subtable_Record *अग्र_record = (*lookup)[i].second;
      स्थिर सूचकांक_Subtable *अग्र_subtable = अग्र_record->get_subtable (base);
      स्वतः *अग्र_header = अग्र_subtable->get_header ();
      अगर (अग्र_header != old_header)
      {
	*start = i;
	early_exit = true;
	अवरोध;
      }
      अचिन्हित पूर्णांक अंक_missing = record->add_glyph_for_subset (new_gid);
      क्रम (अचिन्हित पूर्णांक j = 0; j < अंक_missing; j++)
	अगर (unlikely (!c->serializer->check_success (lengths.धकेलो_or_fail (0))))
	  return_trace (false);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      अगर (old_gid < अग्र_record->first_Glyph_सूचकांक)
	return_trace (false);

      अचिन्हित पूर्णांक old_idx = (अचिन्हित पूर्णांक) old_gid - अग्र_record->first_Glyph_सूचकांक;
      अचिन्हित पूर्णांक offset, लंबाई, format;
      अगर (unlikely (!अग्र_subtable->get_image_तथ्य (old_idx, &offset, &लंबाई, &format)))
	return_trace (false);
      अगर (unlikely (offset > bitmap_आकार_context->cbdt_लंबाई ||
		    bitmap_आकार_context->cbdt_लंबाई - offset < लंबाई))
	return_trace (false);
      अगर (unlikely (!c->serializer->check_success (
			copy_तथ्य_to_cbdt (&तथ्य,
					    bitmap_आकार_context->cbdt + offset,
					    लंबाई))))
	return_trace (false);
      अगर (unlikely (!c->serializer->check_success (lengths.धकेलो_or_fail (लंबाई))))
	return_trace (false);
      अंक_glyphs += 1 + अंक_missing;
    }
    अगर (!early_exit)
      *start = lookup->लंबाई;

    /* Find or append the data block for this subtable. */
    अचिन्हित पूर्णांक image_तथ्य_offset;
    cbdt_dedup_context_t *dedup = bitmap_आकार_context->dedup;
    bool shared = false;
    uint64_t key = 0;
    अगर (dedup && तथ्य.लंबाई)
    {
      key = cbdt_dedup_context_t::hash (image_format, तथ्य, lengths);
      shared = dedup->find (key, तथ्य, lengths,
			    *bitmap_आकार_context->cbdt_अभाज्य,
			    &image_तथ्य_offset);
    }
    अगर (!shared)
    {
      image_तथ्य_offset = bitmap_आकार_context->cbdt_अभाज्य->लंबाई;
      अगर (unlikely (!c->serializer->check_success (
			copy_तथ्य_to_cbdt (bitmap_आकार_context->cbdt_अभाज्य,
					    तथ्य.arrayZ, तथ्य.लंबाई))))
	return_trace (false);
      अगर (dedup && तथ्य.लंबाई)
	/* The bitmap data is already copied; caching is best-effort. */
	(व्योम) dedup->add (key, image_तथ्य_offset, lengths);
    }

    subtable->populate_header (old_header->सूचकांक_Format,
			       image_format,
			       image_तथ्य_offset,
			       &bitmap_आकार_context->आकार);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    अचिन्हित पूर्णांक cumulative = 0;
    क्रम (अचिन्हित पूर्णांक लंबाई : lengths)
    {
      अगर (unlikely (!subtable->add_offset (c->serializer, cumulative,
					   &bitmap_आकार_context->आकार)))
	return_trace (false);
      cumulative += लंबाई;
    }
    अगर (unlikely (!subtable->finish_subtable (c->serializer,
					      cumulative,
					      अंक_glyphs,
					      &bitmap_आकार_context->आकार)))
      return_trace (false);
    return_trace (true);
  }

  bool add_new_record (hb_subset_context_t *c,
		       cblc_bitmap_आकार_subset_context_t *bitmap_आकार_context,
		       स्थिर hb_vector_t<hb_pair_t<hb_codepoint_t, स्थिर सूचकांक_Subtable_Record*>> *lookup, /* IN */
		       स्थिर व्योम *base,
		       अचिन्हित पूर्णांक *start, /* INOUT */
		       hb_vector_t<सूचकांक_Subtable_Record>* records /* INOUT */) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः snap = c->serializer->snapshot ();
    अचिन्हित पूर्णांक old_आकार = bitmap_आकार_context->आकार;
    अचिन्हित पूर्णांक old_cbdt_अभाज्य_लंबाई = bitmap_आकार_context->cbdt_अभाज्य->लंबाई;

    // Set to invalid state to indicate filling glyphs is not yet started.
    अगर (unlikely (!c->serializer->check_success (records->resize (records->लंबाई + 1))))
      return_trace (false);

    records->पुच्छ ().first_Glyph_सूचकांक = 1;
    records->पुच्छ ().last_Glyph_सूचकांक = 0;
    bitmap_आकार_context->आकार += सूचकांक_Subtable_Record::min_आकार;

    c->serializer->push ();

    अगर (unlikely (!add_new_subtable (c, bitmap_आकार_context, &(records->पुच्छ ()), lookup, base, start)))
    {
      c->serializer->निकालो_discard ();
      c->serializer->revert (snap);
      bitmap_आकार_context->cbdt_अभाज्य->shrink (old_cbdt_अभाज्य_लंबाई);
      bitmap_आकार_context->आकार = old_आकार;
      records->resize (records->लंबाई - 1);
      return_trace (false);
    }

    bitmap_आकार_context->अंक_tables += 1;
    return_trace (true);
  }

  अचिन्हित पूर्णांक add_glyph_for_subset (hb_codepoint_t gid)
  {
    अगर (first_Glyph_सूचकांक > last_Glyph_सूचकांक)
    {
      first_Glyph_सूचकांक = gid;
      last_Glyph_सूचकांक = gid;
      वापस 0;
    }
    // TODO maybe assert? this shouldn't occur.
    अगर (last_Glyph_सूचकांक > gid)
      वापस 0;
    अचिन्हित पूर्णांक अंक_missing = (अचिन्हित पूर्णांक) (gid - last_Glyph_सूचकांक - 1);
    last_Glyph_सूचकांक = gid;
    वापस अंक_missing;
  }

  bool get_extents (hb_glyph_extents_t *extents, स्थिर व्योम *base, bool scale) स्थिर
  { वापस (base+offsetToSubtable).get_extents (extents, scale); }

  bool get_image_तथ्य (अचिन्हित पूर्णांक  gid,
		       स्थिर व्योम   *base,
		       अचिन्हित पूर्णांक *offset,
		       अचिन्हित पूर्णांक *लंबाई,
		       अचिन्हित पूर्णांक *format) स्थिर
  {
    अगर (gid < first_Glyph_सूचकांक || gid > last_Glyph_सूचकांक) वापस false;
    वापस (base+offsetToSubtable).get_image_तथ्य (gid - first_Glyph_सूचकांक,
						   offset, लंबाई, format);
  }

  HBGlyphID16			first_Glyph_सूचकांक;
  HBGlyphID16			last_Glyph_सूचकांक;
  Offset32To<सूचकांक_Subtable>	offsetToSubtable;
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा सूचकांक_Subtable_Array
{
  मित्र काष्ठा CBDT;

  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक गिनती) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (सूचकांक_Subtables_Z.sanitize (c, गिनती, यह));
  }

  व्योम
  build_lookup (hb_subset_context_t *c, cblc_bitmap_आकार_subset_context_t *bitmap_आकार_context,
		hb_vector_t<hb_pair_t<hb_codepoint_t,
		स्थिर सूचकांक_Subtable_Record*>> *lookup /* OUT */) स्थिर
  {
    bool start_glyph_is_set = false;
    अचिन्हित अंक_glyphs = c->plan->अंक_output_glyphs ();
    क्रम (hb_codepoint_t new_gid = 0; new_gid < अंक_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      अगर (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) जारी;

      स्थिर सूचकांक_Subtable_Record* record = find_table (old_gid, bitmap_आकार_context->अंक_tables);
      अगर (unlikely (!record)) जारी;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      अचिन्हित पूर्णांक offset, लंबाई, format;
      अगर (unlikely (!record->get_image_तथ्य (old_gid, यह, &offset, &लंबाई, &format))) जारी;

      lookup->push (hb_pair_t<hb_codepoint_t, स्थिर सूचकांक_Subtable_Record*> (new_gid, record));

      अगर (!start_glyph_is_set)
      {
	bitmap_आकार_context->start_glyph = new_gid;
	start_glyph_is_set = true;
      }

      bitmap_आकार_context->end_glyph = new_gid;
    }
  }

  bool
  subset (hb_subset_context_t *c,
	  cblc_bitmap_आकार_subset_context_t *bitmap_आकार_context) स्थिर
  {
    TRACE_SUBSET (यह);

    hb_vector_t<hb_pair_t<hb_codepoint_t, स्थिर सूचकांक_Subtable_Record*>> lookup;
    build_lookup (c, bitmap_आकार_context, &lookup);
    अगर (unlikely (!c->serializer->propagate_error (lookup)))
      वापस false;

    bitmap_आकार_context->आकार = 0;
    bitmap_आकार_context->अंक_tables = 0;
    hb_vector_t<सूचकांक_Subtable_Record> records;
    क्रम (अचिन्हित पूर्णांक start = 0; start < lookup.लंबाई;)
    {
      अगर (unlikely (!lookup[start].second->add_new_record (c, bitmap_आकार_context, &lookup, यह, &start, &records)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	क्रम (अचिन्हित पूर्णांक i = 0; i < records.लंबाई; i++)
	  c->serializer->निकालो_discard ();
	return_trace (false);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    क्रम (अचिन्हित पूर्णांक i = 0; i < records.लंबाई; i++)
      objidxs.push (c->serializer->निकालो_pack ());
    क्रम (अचिन्हित पूर्णांक i = 0; i < records.लंबाई; i++)
    {
      सूचकांक_Subtable_Record* record = c->serializer->embed (records[i]);
      अगर (unlikely (!record)) return_trace (false);
      c->serializer->add_link (record->offsetToSubtable, objidxs[records.लंबाई - 1 - i]);
    }
    return_trace (true);
  }

  खुला:
  स्थिर सूचकांक_Subtable_Record* find_table (hb_codepoint_t glyph, अचिन्हित पूर्णांक अंक_Tables) स्थिर
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_Tables; ++i)
    {
      अचिन्हित पूर्णांक first_Glyph_सूचकांक = सूचकांक_Subtables_Z[i].first_Glyph_सूचकांक;
      अचिन्हित पूर्णांक last_Glyph_सूचकांक = सूचकांक_Subtables_Z[i].last_Glyph_सूचकांक;
      अगर (first_Glyph_सूचकांक <= glyph && glyph <= last_Glyph_सूचकांक)
	वापस &सूचकांक_Subtables_Z[i];
    }
    वापस nullptr;
  }

  रक्षित:
  UnsizedArrayOf<सूचकांक_Subtable_Record>	सूचकांक_Subtables_Z;
};

काष्ठा Bitmap_आकार_Table
{
  मित्र काष्ठा CBLC;
  मित्र काष्ठा CBDT;

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  सूचकांक_Subtable_Array_Offset.sanitize (c, base, संख्या_Of_सूचकांक_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  स्थिर सूचकांक_Subtable_Record *
  find_table (hb_codepoint_t glyph, स्थिर व्योम *base, स्थिर व्योम **out_base) स्थिर
  {
    *out_base = &(base+सूचकांक_Subtable_Array_Offset);
    वापस (base+सूचकांक_Subtable_Array_Offset).find_table (glyph, संख्या_Of_सूचकांक_Subtables);
  }

  bool
  subset (hb_subset_context_t *c, स्थिर व्योम *base,
	  स्थिर अक्षर *cbdt, अचिन्हित पूर्णांक cbdt_लंबाई,
	  hb_vector_t<अक्षर> *cbdt_अभाज्य /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out_table = c->serializer->embed (यह);
    अगर (unlikely (!out_table)) return_trace (false);

    cblc_bitmap_आकार_subset_context_t bitmap_आकार_context;
    bitmap_आकार_context.cbdt = cbdt;
    bitmap_आकार_context.cbdt_लंबाई = cbdt_लंबाई;
    bitmap_आकार_context.cbdt_अभाज्य = cbdt_अभाज्य;
    bitmap_आकार_context.आकार = सूचकांक_Tables_आकार;
    bitmap_आकार_context.अंक_tables = संख्या_Of_सूचकांक_Subtables;
    bitmap_आकार_context.start_glyph = 1;
    bitmap_आकार_context.end_glyph = 0;
    bitmap_आकार_context.dedup = dedup;

    अगर (!out_table->सूचकांक_Subtable_Array_Offset.serialize_subset (c,
							       सूचकांक_Subtable_Array_Offset,
							       base,
							       &bitmap_आकार_context))
      return_trace (false);
    अगर (!bitmap_आकार_context.आकार ||
	!bitmap_आकार_context.अंक_tables ||
	bitmap_आकार_context.start_glyph > bitmap_आकार_context.end_glyph)
      return_trace (false);

    out_table->सूचकांक_Tables_आकार = bitmap_आकार_context.आकार;
    out_table->संख्या_Of_सूचकांक_Subtables = bitmap_आकार_context.अंक_tables;
    out_table->start_Glyph_सूचकांक = bitmap_आकार_context.start_glyph;
    out_table->end_Glyph_सूचकांक = bitmap_आकार_context.end_glyph;
    return_trace (true);
  }

  रक्षित:
  NNOffset32To<सूचकांक_Subtable_Array>
			सूचकांक_Subtable_Array_Offset;
  HBUINT32		सूचकांक_Tables_आकार;
  HBUINT32		संख्या_Of_सूचकांक_Subtables;
  HBUINT32		colorRef;
  SBit_पंक्ति_Metrics	horizontal;
  SBit_पंक्ति_Metrics	vertical;
  HBGlyphID16		start_Glyph_सूचकांक;
  HBGlyphID16		end_Glyph_सूचकांक;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		bitDepth;
  HBINT8		flags;
  खुला:
  DEFINE_आकार_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

काष्ठा Glyph_Bitmap_तथ्य_Format17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	तथ्य;
  खुला:
  DEFINE_आकार_ARRAY (9, तथ्य);
};

काष्ठा Glyph_Bitmap_तथ्य_Format18
{
  BigGlyphMetrics	glyphMetrics;
  Array32Of<HBUINT8>	तथ्य;
  खुला:
  DEFINE_आकार_ARRAY (12, तथ्य);
};

काष्ठा Glyph_Bitmap_तथ्य_Format19
{
  Array32Of<HBUINT8>	तथ्य;
  खुला:
  DEFINE_आकार_ARRAY (4, तथ्य);
};

काष्ठा CBLC
{
  मित्र काष्ठा CBDT;

  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_CBLC;

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3) &&
		  hb_barrier () &&
		  आकार_Tables.sanitize (c, यह));
  }

  अटल bool
  sink_cbdt (hb_subset_context_t *c, hb_vector_t<अक्षर>* cbdt_अभाज्य)
  {
    hb_blob_t *cbdt_अभाज्य_blob = hb_blob_create (cbdt_अभाज्य->arrayZ,
						 cbdt_अभाज्य->लंबाई,
						 HB_MEMORY_MODE_WRITABLE,
						 cbdt_अभाज्य->arrayZ,
						 hb_free);
    cbdt_अभाज्य->init ();  // Leak arrayZ to the blob.
    bool ret = c->plan->add_table (HB_OT_TAG_CBDT, cbdt_अभाज्य_blob);
    hb_blob_destroy (cbdt_अभाज्य_blob);
    वापस ret;
  }

  bool
  subset_आकार_table (hb_subset_context_t *c, स्थिर Bitmap_आकार_Table& table,
		     स्थिर अक्षर *cbdt /* IN */, अचिन्हित पूर्णांक cbdt_लंबाई,
		     CBLC *cblc_अभाज्य /* INOUT */, hb_vector_t<अक्षर> *cbdt_अभाज्य /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) स्थिर
  {
    TRACE_SUBSET (यह);
    cblc_अभाज्य->आकार_Tables.len++;

    स्वतः snap = c->serializer->snapshot ();
    स्वतः cbdt_अभाज्य_len = cbdt_अभाज्य->लंबाई;

    अगर (!table.subset (c, यह, cbdt, cbdt_लंबाई, cbdt_अभाज्य, dedup))
    {
      cblc_अभाज्य->आकार_Tables.len--;
      c->serializer->revert (snap);
      cbdt_अभाज्य->shrink (cbdt_अभाज्य_len);
      return_trace (false);
    }
    return_trace (true);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_INTERNAL bool subset (hb_subset_context_t *c) स्थिर;

  रक्षित:
  स्थिर Bitmap_आकार_Table &choose_strike (hb_font_t *font) स्थिर
  {
    अचिन्हित गिनती = आकार_Tables.len;
    अगर (unlikely (!गिनती))
      वापस Null (Bitmap_आकार_Table);

    अचिन्हित पूर्णांक requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    अगर (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    अचिन्हित पूर्णांक best_i = 0;
    अचिन्हित पूर्णांक best_ppem = hb_max (आकार_Tables[0].ppemX, आकार_Tables[0].ppemY);

    क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
    {
      अचिन्हित पूर्णांक ppem = hb_max (आकार_Tables[i].ppemX, आकार_Tables[i].ppemY);
      अगर ((requested_ppem <= ppem && ppem < best_ppem) ||
	  (requested_ppem > best_ppem && ppem > best_ppem))
      {
	best_i = i;
	best_ppem = ppem;
      }
    }

    वापस आकार_Tables[best_i];
  }

  रक्षित:
  FixedVersion<>		version;
  Array32Of<Bitmap_आकार_Table>	आकार_Tables;
  खुला:
  DEFINE_आकार_ARRAY (8, आकार_Tables);
};

काष्ठा CBDT
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_CBDT;

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      यह->cblc = hb_sanitize_context_t ().reference_table<CBLC> (face);
      यह->cbdt = hb_sanitize_context_t ().reference_table<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      यह->cblc.destroy ();
      यह->cbdt.destroy ();
    }

    bool
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, bool scale = true) स्थिर
    {
      स्थिर व्योम *base;
      स्थिर Bitmap_आकार_Table &strike = यह->cblc->choose_strike (font);
      स्थिर सूचकांक_Subtable_Record *subtable_record = strike.find_table (glyph, cblc, &base);
      अगर (!subtable_record || !strike.ppemX || !strike.ppemY)
	वापस false;

      अगर (subtable_record->get_extents (extents, base, scale))
	वापस true;

      अचिन्हित पूर्णांक image_offset = 0, image_लंबाई = 0, image_format = 0;
      अगर (!subtable_record->get_image_तथ्य (glyph, base, &image_offset, &image_लंबाई, &image_format))
	वापस false;

      अचिन्हित पूर्णांक cbdt_len = cbdt.get_लंबाई ();
      अगर (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_लंबाई))
	वापस false;

      चयन (image_format)
      {
      हाल 17: {
	अगर (unlikely (image_लंबाई < Glyph_Bitmap_तथ्य_Format17::min_आकार))
	  वापस false;
	स्वतः &glyphFormat17 = StructAtOffset<Glyph_Bitmap_तथ्य_Format17> (यह->cbdt, image_offset);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	अवरोध;
      }
      हाल 18: {
	अगर (unlikely (image_लंबाई < Glyph_Bitmap_तथ्य_Format18::min_आकार))
	  वापस false;
	स्वतः &glyphFormat18 = StructAtOffset<Glyph_Bitmap_तथ्य_Format18> (यह->cbdt, image_offset);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	अवरोध;
      }
      शेष: वापस false; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      अगर (scale)
      {
	भग्न x_scale = upem / (भग्न) strike.ppemX;
	भग्न y_scale = upem / (भग्न) strike.ppemY;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * x_scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * y_scale));
      }

      वापस true;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) स्थिर
    {
      स्थिर व्योम *base;
      स्थिर Bitmap_आकार_Table &strike = यह->cblc->choose_strike (font);
      स्थिर सूचकांक_Subtable_Record *subtable_record = strike.find_table (glyph, cblc, &base);
      अगर (!subtable_record || !strike.ppemX || !strike.ppemY)
	वापस hb_blob_get_empty ();

      अचिन्हित पूर्णांक image_offset = 0, image_लंबाई = 0, image_format = 0;
      अगर (!subtable_record->get_image_तथ्य (glyph, base, &image_offset, &image_लंबाई, &image_format))
	वापस hb_blob_get_empty ();

      अचिन्हित पूर्णांक cbdt_len = cbdt.get_लंबाई ();
      अगर (unlikely (image_offset > cbdt_len || cbdt_len - image_offset < image_लंबाई))
	वापस hb_blob_get_empty ();

      चयन (image_format)
      {
      हाल 17:
      {
	अगर (unlikely (image_लंबाई < Glyph_Bitmap_तथ्य_Format17::min_आकार))
	  वापस hb_blob_get_empty ();
	स्वतः &glyphFormat17 = StructAtOffset<Glyph_Bitmap_तथ्य_Format17> (यह->cbdt, image_offset);
	वापस hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + Glyph_Bitmap_तथ्य_Format17::min_आकार,
					glyphFormat17.तथ्य.len);
      }
      हाल 18:
      {
	अगर (unlikely (image_लंबाई < Glyph_Bitmap_तथ्य_Format18::min_आकार))
	  वापस hb_blob_get_empty ();
	स्वतः &glyphFormat18 = StructAtOffset<Glyph_Bitmap_तथ्य_Format18> (यह->cbdt, image_offset);
	वापस hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + Glyph_Bitmap_तथ्य_Format18::min_आकार,
					glyphFormat18.तथ्य.len);
      }
      हाल 19:
      {
	अगर (unlikely (image_लंबाई < Glyph_Bitmap_तथ्य_Format19::min_आकार))
	  वापस hb_blob_get_empty ();
	स्वतः &glyphFormat19 = StructAtOffset<Glyph_Bitmap_तथ्य_Format19> (यह->cbdt, image_offset);
	वापस hb_blob_create_sub_blob (cbdt.get_blob (),
					image_offset + Glyph_Bitmap_तथ्य_Format19::min_आकार,
					glyphFormat19.तथ्य.len);
      }
      शेष: वापस hb_blob_get_empty (); /* TODO: Support other image formats. */
      }
    }

    bool has_तथ्य () स्थिर { वापस cbdt->version.major; }

    bool paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, व्योम *तथ्य) स्थिर
    {
      अगर (!has_तथ्य ()) वापस false;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      अगर (unlikely (!font->get_glyph_extents (glyph, &extents, false)))
        वापस false;

      अगर (unlikely (!get_extents (font, glyph, &pixel_extents, false)))
        वापस false;

      hb_blob_t *blob = reference_png (font, glyph);
      अगर (unlikely (hb_blob_is_immutable (blob)))
        वापस false;

      bool ret = funcs->image (तथ्य,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      वापस ret;
    }

    निजी:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    अचिन्हित पूर्णांक upem;
  };

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  likely (version.major == 2 || version.major == 3));
  }

  रक्षित:
  FixedVersion<>		version;
  UnsizedArrayOf<HBUINT8>	तथ्य_Z;
  खुला:
  DEFINE_आकार_ARRAY (4, तथ्य_Z);
};

अंतरभूत bool
CBLC::subset (hb_subset_context_t *c) स्थिर
{
  TRACE_SUBSET (यह);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_vector_t<अक्षर> cbdt_अभाज्य;

  स्वतः *cblc_अभाज्य = c->serializer->start_embed<CBLC> ();
  अगर (unlikely (!c->serializer->extend_min (cblc_अभाज्य))) return_trace (false);
  cblc_अभाज्य->version = version;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_table<CBDT> (c->plan->source);
  अचिन्हित पूर्णांक cbdt_लंबाई;
  CBDT* cbdt = (CBDT *) hb_blob_get_तथ्य (cbdt_blob, &cbdt_लंबाई);
  अगर (unlikely (cbdt_लंबाई < CBDT::min_आकार))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (false);
  }
  copy_तथ्य_to_cbdt (&cbdt_अभाज्य, cbdt, CBDT::min_आकार);

  cbdt_dedup_context_t dedup;
  क्रम (स्थिर Bitmap_आकार_Table& table : + आकार_Tables.iter ())
    subset_आकार_table (c, table, (स्थिर अक्षर *) cbdt, cbdt_लंबाई, cblc_अभाज्य, &cbdt_अभाज्य, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_अभाज्य));
}

काष्ठा CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#पूर्ण_अगर /* OT_COLOR_CBDT_CBDT_HH */
