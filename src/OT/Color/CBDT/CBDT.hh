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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_COLOR_CBDT_CBDT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_COLOR_CBDT_CBDT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-paint.hh"

/*
 * CBLC -- Color Bitmap Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cblc
 * https://docs.microsoft.com/en-us/typography/opentype/spec/eblc
 * CBDT -- Color Bitmap Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cbdt
 * https://docs.microsoft.com/en-us/typography/opentype/spec/ebdt
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_CBLC HB_ਟੈਗ('C','B','L','C')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_CBDT HB_ਟੈਗ('C','B','D','T')


ਨਾਮਾਕਾਸ਼ OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
ਬਣਤਰ cbdt_dedup_context_t
{
  ਬਣਤਰ ਇੰਦਰਾਜ_t
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ;		/* Offset of the data in cbdt_prime. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lstart;	/* Start index in lengths. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lcount;	/* Number of length entries. */
  };

  ਸਥਿਰ uint64_t hash (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ image_format,
			ਅਚਲ hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &ਅੰਕੜੇ,
			ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> &lengths)
  {
    uint32_t h = hb_hash (hb_ਬਾਈਟਾਂ_t (ਅੰਕੜੇ.arrayZ, ਅੰਕੜੇ.ਲੰਮਾਈ));
    h = h * 31 + hb_hash (hb_ਬਾਈਟਾਂ_t ((ਅਚਲ ਅੱਖਰ *) lengths.arrayZ,
				      lengths.ਲੰਮਾਈ * ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)));
    ਵਾਪਸ ((uint64_t) image_format << 32) | h;
  }

  ਤਾਰਕਿਕ find (uint64_t key,
	     ਅਚਲ hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &ਅੰਕੜੇ,
	     ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> &lengths,
	     ਅਚਲ hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &cbdt_ਅਭਾਜ,
	     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਅਧਾਰ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *idx_p = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (!map.has (key, &idx_p)) ਵਾਪਸ ਝੂਠ;
    ਅਚਲ ਇੰਦਰਾਜ_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    ਜੇ (e.lcount != lengths.ਲੰਮਾਈ) ਵਾਪਸ ਝੂਠ;
    ਜੇ (unlikely (e.ਅਧਾਰ + ਅੰਕੜੇ.ਲੰਮਾਈ > (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) cbdt_ਅਭਾਜ.ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;
    ਜੇ (hb_memcmp (cbdt_ਅਭਾਜ.arrayZ + e.ਅਧਾਰ, ਅੰਕੜੇ.arrayZ, ਅੰਕੜੇ.ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;
    ਜੇ (hb_memcmp (ਇਹ->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.ਲੰਮਾਈ * ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)))
      ਵਾਪਸ ਝੂਠ;
    *ਅਧਾਰ = e.ਅਧਾਰ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ ਜੋੜੋ (uint64_t key,
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ,
	    ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> &lengths)
  {
    ਜੇ (unlikely (entries.ਵਿੱਚ_error () || ਇਹ->lengths.ਵਿੱਚ_error () || map.ਵਿੱਚ_error ()))
      ਵਾਪਸ ਝੂਠ;
    ਇੰਦਰਾਜ_t e = {ਅਧਾਰ, ਇਹ->lengths.ਲੰਮਾਈ, lengths.ਲੰਮਾਈ};
    ਜੇ (unlikely (!entries.ਧੱਕੋ_or_fail (e))) ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ : lengths)
      ਜੇ (unlikely (!ਇਹ->lengths.ਧੱਕੋ_or_fail (ਲੰਮਾਈ))) ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ map.ਸੈੱਟ (key, entries.ਲੰਮਾਈ - 1);
  }

  hb_hashmap_t<uint64_t, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> map;
  hb_ਸਦਿਸ਼_t<ਇੰਦਰਾਜ_t> entries;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> lengths; /* Concatenated per-entry lengths. */
};

ਬਣਤਰ cblc_bitmap_ਮਾਪ_subset_context_t
{
  ਅਚਲ ਅੱਖਰ *cbdt;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cbdt_ਲੰਮਾਈ;
  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> *cbdt_ਅਭਾਜ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t ਸ਼ੁਰੂ_glyph;	/*ਬਾਹਰਲਾ*/
  hb_codepoint_t ਸਿਰਾ_glyph;	/*ਬਾਹਰਲਾ*/
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_cbdt (hb_ਸਦਿਸ਼_t<ਅੱਖਰ> *cbdt_ਅਭਾਜ,
		    ਅਚਲ ਖਾਲੀ        *ਅੰਕੜੇ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ           ਲੰਮਾਈ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_len = cbdt_ਅਭਾਜ->ਲੰਮਾਈ + ਲੰਮਾਈ;
  ਜੇ (unlikely (!cbdt_ਅਭਾਜ->alloc (new_len))) ਵਾਪਸ ਝੂਠ;
  hb_memcpy (cbdt_ਅਭਾਜ->arrayZ + cbdt_ਅਭਾਜ->ਲੰਮਾਈ, ਅੰਕੜੇ, ਲੰਮਾਈ);
  cbdt_ਅਭਾਜ->ਲੰਮਾਈ = new_len;
  ਵਾਪਸ ਸੱਚ;
}

ਬਣਤਰ SmallGlyphMetrics
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ get_extents (hb_font_t *font, hb_glyph_extents_t *extents, ਤਾਰਕਿਕ scale) ਅਚਲ
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->ਚੌੜਾਈ = ਚੌੜਾਈ;
    extents->ਉਚਾਈ = -static_cast<ਪੂਰਨ> (ਉਚਾਈ);

    ਜੇ (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	ਉਚਾਈ;
  HBUINT8	ਚੌੜਾਈ;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (5);
};

ਬਣਤਰ BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ SBit_ਸਤਰ_Metrics
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBINT8	ascender;
  HBINT8	decender;
  HBUINT8	ਚੌੜਾਈ_Max;
  HBINT8	caret_ਢਲਾਨ_Numerator;
  HBINT8	caret_ਢਲਾਨ_Denominator;
  HBINT8	caret_ਵਿੱਥ;
  HBINT8	minOriginSB;
  HBINT8	minAdvanceSB;
  HBINT8	maxBeforeBL;
  HBINT8	minAfterBL;
  HBINT8	padding1;
  HBINT8	padding2;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};


/*
 * Index Subtables.
 */

ਬਣਤਰ ਸੂਚਕਾਂਕ_Subtable_ਸਿਰਲੇਖ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT16	ਸੂਚਕਾਂਕ_Format;
  HBUINT16	imageFormat;
  HBUINT32	image_ਅੰਕੜੇ_ਵਿੱਥ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_Type>
ਬਣਤਰ ਸੂਚਕਾਂਕ_Subtable_Format1_Or3
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਗਿਣਤੀ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  ਵਿੱਥ_Array_Z.sanitize (c, glyph_ਗਿਣਤੀ + 1));
  }

  ਤਾਰਕਿਕ get_image_ਅੰਕੜੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਵਿੱਥ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਲੰਮਾਈ) ਅਚਲ
  {
    ਜੇ (unlikely (ਵਿੱਥ_Array_Z[idx + 1] <= ਵਿੱਥ_Array_Z[idx]))
      ਵਾਪਸ ਝੂਠ;

    *ਵਿੱਥ = ਸਿਰਲੇਖ.image_ਅੰਕੜੇ_ਵਿੱਥ + ਵਿੱਥ_Array_Z[idx];
    *ਲੰਮਾਈ = ਵਿੱਥ_Array_Z[idx + 1] - ਵਿੱਥ_Array_Z[idx];
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ ਜੋੜੋ_ਵਿੱਥ (hb_serialize_context_t *c,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਮਾਪ /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਵਿੱਥ_2<ਵਿੱਥ_Type> embedded_ਵਿੱਥ;
    embedded_ਵਿੱਥ = ਵਿੱਥ;
    *ਮਾਪ += ਆਕਾਰ (ਵਿੱਥ_Type);
    ਸਵੈਚਾਲਿਤ *o = c->embed (embedded_ਵਿੱਥ);
    return_trace ((ਤਾਰਕਿਕ) o);
  }

  ਸੂਚਕਾਂਕ_Subtable_ਸਿਰਲੇਖ	ਸਿਰਲੇਖ;
  Unsized_Array_ਦਾ<ਵਿੱਥ_2<ਵਿੱਥ_Type>>
			ਵਿੱਥ_Array_Z;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, ਵਿੱਥ_Array_Z);
};

ਬਣਤਰ ਸੂਚਕਾਂਕ_Subtable_Format1 : ਸੂਚਕਾਂਕ_Subtable_Format1_Or3<HBUINT32> {};
ਬਣਤਰ ਸੂਚਕਾਂਕ_Subtable_Format3 : ਸੂਚਕਾਂਕ_Subtable_Format1_Or3<HBUINT16> {};

ਬਣਤਰ ਸੂਚਕਾਂਕ_Subtable
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਗਿਣਤੀ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.ਸਿਰਲੇਖ.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.ਸਿਰਲੇਖ.ਸੂਚਕਾਂਕ_Format)
    {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_ਗਿਣਤੀ));
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_ਗਿਣਤੀ));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਤਾਰਕਿਕ
  ਅੰਤ_subtable (hb_serialize_context_t *c,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ_ਵਿੱਥ,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਮਾਪ /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ਇਹ);

    ਚੋਣ (u.ਸਿਰਲੇਖ.ਸੂਚਕਾਂਕ_Format)
    {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.ਜੋੜੋ_ਵਿੱਥ (c, ਸਿਰਾ_ਵਿੱਥ, ਮਾਪ));
    ਹਾਲਤ 3: {
      hb_barrier ();
      ਜੇ (!u.format3.ਜੋੜੋ_ਵਿੱਥ (c, ਸਿਰਾ_ਵਿੱਥ, ਮਾਪ))
	return_trace (ਝੂਠ);
      ਜੇ (!(ਅੰਕ_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.ਜੋੜੋ_ਵਿੱਥ (c, 0, ਮਾਪ));
      return_trace (ਸੱਚ);
    }
    // TODO: implement 2, 4, 5.
    ਹਾਲਤ 2: ਹਾਲਤ 4:  // No-op.
    ਹਾਲਤ 5:  // Pad to 32-bit aligned.
    ਮੂਲ: return_trace (ਝੂਠ);
    }
  }

  ਤਾਰਕਿਕ
  ਜੋੜੋ_ਵਿੱਥ (hb_serialize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ local_ਵਿੱਥ,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਮਾਪ /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਚੋਣ (u.ਸਿਰਲੇਖ.ਸੂਚਕਾਂਕ_Format)
    {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.ਜੋੜੋ_ਵਿੱਥ (c, local_ਵਿੱਥ, ਮਾਪ));
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.ਜੋੜੋ_ਵਿੱਥ (c, local_ਵਿੱਥ, ਮਾਪ));
    // TODO: Implement tables 2, 4, 5
    ਹਾਲਤ 2:  // Should be a no-op.
    ਹਾਲਤ 4: ਹਾਲਤ 5:  // Handle sparse cases.
    ਮੂਲ: return_trace (ਝੂਠ);
    }
  }

  ਤਾਰਕਿਕ get_extents (hb_glyph_extents_t *extents HB_ਅਣਵਰਤਿਆ, ਤਾਰਕਿਕ scale HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    ਚੋਣ (u.ਸਿਰਲੇਖ.ਸੂਚਕਾਂਕ_Format)
    {
    ਹਾਲਤ 2: ਹਾਲਤ 5: /* TODO */
    ਹਾਲਤ 1: ਹਾਲਤ 3: ਹਾਲਤ 4: /* Variable-metrics formats do not have metrics here. */
    ਮੂਲ:ਵਾਪਸ (ਝੂਠ);
    }
  }

  ਤਾਰਕਿਕ
  get_image_ਅੰਕੜੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਵਿੱਥ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਲੰਮਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *format) ਅਚਲ
  {
    *format = u.ਸਿਰਲੇਖ.imageFormat;
    ਚੋਣ (u.ਸਿਰਲੇਖ.ਸੂਚਕਾਂਕ_Format)
    {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_image_ਅੰਕੜੇ (idx, ਵਿੱਥ, ਲੰਮਾਈ);
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_image_ਅੰਕੜੇ (idx, ਵਿੱਥ, ਲੰਮਾਈ);
    ਮੂਲ: ਵਾਪਸ ਝੂਠ;
    }
  }

  ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਸਿਰਲੇਖ* get_ਸਿਰਲੇਖ () ਅਚਲ { ਵਾਪਸ &u.ਸਿਰਲੇਖ; }

  ਖਾਲੀ populate_ਸਿਰਲੇਖ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_format,
			ਬਿਨਾਨਿਸ਼ਾਨੀ image_format,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ image_ਅੰਕੜੇ_ਵਿੱਥ_2,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਮਾਪ)
  {
    u.ਸਿਰਲੇਖ.ਸੂਚਕਾਂਕ_Format = ਸੂਚਕਾਂਕ_format;
    u.ਸਿਰਲੇਖ.imageFormat = image_format;
    u.ਸਿਰਲੇਖ.image_ਅੰਕੜੇ_ਵਿੱਥ = image_ਅੰਕੜੇ_ਵਿੱਥ_2;
    ਚੋਣ (u.ਸਿਰਲੇਖ.ਸੂਚਕਾਂਕ_Format)
    {
    ਹਾਲਤ 1: *ਮਾਪ += ਸੂਚਕਾਂਕ_Subtable_Format1::min_ਮਾਪ; ਰੋਕੋ;
    ਹਾਲਤ 3: *ਮਾਪ += ਸੂਚਕਾਂਕ_Subtable_Format3::min_ਮਾਪ; ਰੋਕੋ;
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਸੂਚਕਾਂਕ_Subtable_ਸਿਰਲੇਖ	ਸਿਰਲੇਖ;
  ਸੂਚਕਾਂਕ_Subtable_Format1	format1;
  ਸੂਚਕਾਂਕ_Subtable_Format3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (8, ਸਿਰਲੇਖ);
};

ਬਣਤਰ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ
{
  /* XXX Remove this and fix by not inserting it into vector. */
  ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ& ਸੰਚਾਲਕ = (ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ &o)
  {
    ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ = o.ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ;
    ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ = o.ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ;
    ਵਿੱਥ_ਮੰਜ਼ਿਲ_Subtable = (ਬਿਨਾਨਿਸ਼ਾਨੀ) o.ਵਿੱਥ_ਮੰਜ਼ਿਲ_Subtable;
    ਪ੍ਰਤਿਜ੍ਞਾ (ਵਿੱਥ_ਮੰਜ਼ਿਲ_Subtable.is_null ());
    ਵਾਪਸ *ਇਹ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ <= ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ &&
		  ਵਿੱਥ_ਮੰਜ਼ਿਲ_Subtable.sanitize (c, ਅਧਾਰ, ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ - ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ + 1));
  }

  ਅਚਲ ਸੂਚਕਾਂਕ_Subtable* get_subtable (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਵਾਪਸ &(ਅਧਾਰ+ਵਿੱਥ_ਮੰਜ਼ਿਲ_Subtable);
  }

  ਤਾਰਕਿਕ ਜੋੜੋ_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_ਮਾਪ_subset_context_t *bitmap_ਮਾਪ_context,
			 ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ,
			 ਅਚਲ hb_ਸਦਿਸ਼_t<hb_pair_t<hb_codepoint_t, ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ*>> *lookup, /*ਵਿੱਚ*/
			 ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸ਼ੁਰੂ /* INOUT */) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਸਵੈਚਾਲਿਤ *subtable = c->serializer->ਸ਼ੁਰੂ_embed<ਸੂਚਕਾਂਕ_Subtable> ();
    ਜੇ (unlikely (!c->serializer->extend_min (subtable))) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ *old_subtable = get_subtable (ਅਧਾਰ);
    ਸਵੈਚਾਲਿਤ *old_ਸਿਰਲੇਖ = old_subtable->get_ਸਿਰਲੇਖ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ image_format = old_ਸਿਰਲੇਖ->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> ਅੰਕੜੇ;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> lengths;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = 0;
    ਤਾਰਕਿਕ early_exit = ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = *ਸ਼ੁਰੂ; i < lookup->ਲੰਮਾਈ; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].ਪਹਿਲਾ;
      ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ *ਅਗਲਾ_ਰਿਕਾਰਡ = (*lookup)[i].second;
      ਅਚਲ ਸੂਚਕਾਂਕ_Subtable *ਅਗਲਾ_subtable = ਅਗਲਾ_ਰਿਕਾਰਡ->get_subtable (ਅਧਾਰ);
      ਸਵੈਚਾਲਿਤ *ਅਗਲਾ_ਸਿਰਲੇਖ = ਅਗਲਾ_subtable->get_ਸਿਰਲੇਖ ();
      ਜੇ (ਅਗਲਾ_ਸਿਰਲੇਖ != old_ਸਿਰਲੇਖ)
      {
	*ਸ਼ੁਰੂ = i;
	early_exit = ਸੱਚ;
	ਰੋਕੋ;
      }
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_missing = ਰਿਕਾਰਡ->ਜੋੜੋ_glyph_for_subset (new_gid);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ਅੰਕ_missing; j++)
	ਜੇ (unlikely (!c->serializer->check_success (lengths.ਧੱਕੋ_or_fail (0))))
	  return_trace (ਝੂਠ);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      ਜੇ (old_gid < ਅਗਲਾ_ਰਿਕਾਰਡ->ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ)
	return_trace (ਝੂਠ);

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_idx = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) old_gid - ਅਗਲਾ_ਰਿਕਾਰਡ->ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ, ਲੰਮਾਈ, format;
      ਜੇ (unlikely (!ਅਗਲਾ_subtable->get_image_ਅੰਕੜੇ (old_idx, &ਵਿੱਥ, &ਲੰਮਾਈ, &format)))
	return_trace (ਝੂਠ);
      ਜੇ (unlikely (ਵਿੱਥ > bitmap_ਮਾਪ_context->cbdt_ਲੰਮਾਈ ||
		    bitmap_ਮਾਪ_context->cbdt_ਲੰਮਾਈ - ਵਿੱਥ < ਲੰਮਾਈ))
	return_trace (ਝੂਠ);
      ਜੇ (unlikely (!c->serializer->check_success (
			copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_cbdt (&ਅੰਕੜੇ,
					    bitmap_ਮਾਪ_context->cbdt + ਵਿੱਥ,
					    ਲੰਮਾਈ))))
	return_trace (ਝੂਠ);
      ਜੇ (unlikely (!c->serializer->check_success (lengths.ਧੱਕੋ_or_fail (ਲੰਮਾਈ))))
	return_trace (ਝੂਠ);
      ਅੰਕ_glyphs += 1 + ਅੰਕ_missing;
    }
    ਜੇ (!early_exit)
      *ਸ਼ੁਰੂ = lookup->ਲੰਮਾਈ;

    /* Find or append the data block for this subtable. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ image_ਅੰਕੜੇ_ਵਿੱਥ_2;
    cbdt_dedup_context_t *dedup = bitmap_ਮਾਪ_context->dedup;
    ਤਾਰਕਿਕ shared = ਝੂਠ;
    uint64_t key = 0;
    ਜੇ (dedup && ਅੰਕੜੇ.ਲੰਮਾਈ)
    {
      key = cbdt_dedup_context_t::hash (image_format, ਅੰਕੜੇ, lengths);
      shared = dedup->find (key, ਅੰਕੜੇ, lengths,
			    *bitmap_ਮਾਪ_context->cbdt_ਅਭਾਜ,
			    &image_ਅੰਕੜੇ_ਵਿੱਥ_2);
    }
    ਜੇ (!shared)
    {
      image_ਅੰਕੜੇ_ਵਿੱਥ_2 = bitmap_ਮਾਪ_context->cbdt_ਅਭਾਜ->ਲੰਮਾਈ;
      ਜੇ (unlikely (!c->serializer->check_success (
			copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_cbdt (bitmap_ਮਾਪ_context->cbdt_ਅਭਾਜ,
					    ਅੰਕੜੇ.arrayZ, ਅੰਕੜੇ.ਲੰਮਾਈ))))
	return_trace (ਝੂਠ);
      ਜੇ (dedup && ਅੰਕੜੇ.ਲੰਮਾਈ)
	/* The bitmap data is already copied; caching is best-effort. */
	(ਖਾਲੀ) dedup->ਜੋੜੋ (key, image_ਅੰਕੜੇ_ਵਿੱਥ_2, lengths);
    }

    subtable->populate_ਸਿਰਲੇਖ (old_ਸਿਰਲੇਖ->ਸੂਚਕਾਂਕ_Format,
			       image_format,
			       image_ਅੰਕੜੇ_ਵਿੱਥ_2,
			       &bitmap_ਮਾਪ_context->ਮਾਪ);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cumulative = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ : lengths)
    {
      ਜੇ (unlikely (!subtable->ਜੋੜੋ_ਵਿੱਥ (c->serializer, cumulative,
					   &bitmap_ਮਾਪ_context->ਮਾਪ)))
	return_trace (ਝੂਠ);
      cumulative += ਲੰਮਾਈ;
    }
    ਜੇ (unlikely (!subtable->ਅੰਤ_subtable (c->serializer,
					      cumulative,
					      ਅੰਕ_glyphs,
					      &bitmap_ਮਾਪ_context->ਮਾਪ)))
      return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ ਜੋੜੋ_new_ਰਿਕਾਰਡ (hb_subset_context_t *c,
		       cblc_bitmap_ਮਾਪ_subset_context_t *bitmap_ਮਾਪ_context,
		       ਅਚਲ hb_ਸਦਿਸ਼_t<hb_pair_t<hb_codepoint_t, ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ*>> *lookup, /*ਵਿੱਚ*/
		       ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸ਼ੁਰੂ, /* INOUT */
		       hb_ਸਦਿਸ਼_t<ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ>* ਰਿਕਾਰਡਾਂ /* INOUT */) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_ਮਾਪ = bitmap_ਮਾਪ_context->ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_cbdt_ਅਭਾਜ_ਲੰਮਾਈ = bitmap_ਮਾਪ_context->cbdt_ਅਭਾਜ->ਲੰਮਾਈ;

    // Set to invalid state to indicate filling glyphs is not yet started.
    ਜੇ (unlikely (!c->serializer->check_success (ਰਿਕਾਰਡਾਂ->resize (ਰਿਕਾਰਡਾਂ->ਲੰਮਾਈ + 1))))
      return_trace (ਝੂਠ);

    ਰਿਕਾਰਡਾਂ->ਪੂਛ ().ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ = 1;
    ਰਿਕਾਰਡਾਂ->ਪੂਛ ().ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ = 0;
    bitmap_ਮਾਪ_context->ਮਾਪ += ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ::min_ਮਾਪ;

    c->serializer->push ();

    ਜੇ (unlikely (!ਜੋੜੋ_new_subtable (c, bitmap_ਮਾਪ_context, &(ਰਿਕਾਰਡਾਂ->ਪੂਛ ()), lookup, ਅਧਾਰ, ਸ਼ੁਰੂ)))
    {
      c->serializer->ਕੱਢੋ_discard ();
      c->serializer->revert (snap);
      bitmap_ਮਾਪ_context->cbdt_ਅਭਾਜ->shrink (old_cbdt_ਅਭਾਜ_ਲੰਮਾਈ);
      bitmap_ਮਾਪ_context->ਮਾਪ = old_ਮਾਪ;
      ਰਿਕਾਰਡਾਂ->resize (ਰਿਕਾਰਡਾਂ->ਲੰਮਾਈ - 1);
      return_trace (ਝੂਠ);
    }

    bitmap_ਮਾਪ_context->ਅੰਕ_tables += 1;
    return_trace (ਸੱਚ);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਜੋੜੋ_glyph_for_subset (hb_codepoint_t gid)
  {
    ਜੇ (ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ > ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ)
    {
      ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ = gid;
      ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ = gid;
      ਵਾਪਸ 0;
    }
    // TODO maybe assert? this shouldn't occur.
    ਜੇ (ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ > gid)
      ਵਾਪਸ 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_missing = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (gid - ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ - 1);
    ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ = gid;
    ਵਾਪਸ ਅੰਕ_missing;
  }

  ਤਾਰਕਿਕ get_extents (hb_glyph_extents_t *extents, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, ਤਾਰਕਿਕ scale) ਅਚਲ
  { ਵਾਪਸ (ਅਧਾਰ+ਵਿੱਥ_ਮੰਜ਼ਿਲ_Subtable).get_extents (extents, scale); }

  ਤਾਰਕਿਕ get_image_ਅੰਕੜੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  gid,
		       ਅਚਲ ਖਾਲੀ   *ਅਧਾਰ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਵਿੱਥ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਲੰਮਾਈ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *format) ਅਚਲ
  {
    ਜੇ (gid < ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ || gid > ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ) ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ (ਅਧਾਰ+ਵਿੱਥ_ਮੰਜ਼ਿਲ_Subtable).get_image_ਅੰਕੜੇ (gid - ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ,
						   ਵਿੱਥ, ਲੰਮਾਈ, format);
  }

  HBGlyphID16			ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ;
  HBGlyphID16			ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ;
  Offset32_ਮੰਜ਼ਿਲ<ਸੂਚਕਾਂਕ_Subtable>	ਵਿੱਥ_ਮੰਜ਼ਿਲ_Subtable;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ ਸੂਚਕਾਂਕ_Subtable_Array
{
  ਮਿੱਤਰ ਬਣਤਰ CBDT;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੂਚਕਾਂਕ_Subtables_Z.sanitize (c, ਗਿਣਤੀ_2, ਇਹ));
  }

  ਖਾਲੀ
  build_lookup (hb_subset_context_t *c, cblc_bitmap_ਮਾਪ_subset_context_t *bitmap_ਮਾਪ_context,
		hb_ਸਦਿਸ਼_t<hb_pair_t<hb_codepoint_t,
		ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ*>> *lookup /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਤਾਰਕਿਕ ਸ਼ੁਰੂ_glyph_is_ਸੈੱਟ = ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = c->plan->ਅੰਕ_output_glyphs ();
    ਲਈ (hb_codepoint_t new_gid = 0; new_gid < ਅੰਕ_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      ਜੇ (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) ਜਾਰੀ;

      ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ* ਰਿਕਾਰਡ = ਲੱਭੋ_ਸਾਰਣੀ (old_gid, bitmap_ਮਾਪ_context->ਅੰਕ_tables);
      ਜੇ (unlikely (!ਰਿਕਾਰਡ)) ਜਾਰੀ;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ, ਲੰਮਾਈ, format;
      ਜੇ (unlikely (!ਰਿਕਾਰਡ->get_image_ਅੰਕੜੇ (old_gid, ਇਹ, &ਵਿੱਥ, &ਲੰਮਾਈ, &format))) ਜਾਰੀ;

      lookup->push (hb_pair_t<hb_codepoint_t, ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ*> (new_gid, ਰਿਕਾਰਡ));

      ਜੇ (!ਸ਼ੁਰੂ_glyph_is_ਸੈੱਟ)
      {
	bitmap_ਮਾਪ_context->ਸ਼ੁਰੂ_glyph = new_gid;
	ਸ਼ੁਰੂ_glyph_is_ਸੈੱਟ = ਸੱਚ;
      }

      bitmap_ਮਾਪ_context->ਸਿਰਾ_glyph = new_gid;
    }
  }

  ਤਾਰਕਿਕ
  subset (hb_subset_context_t *c,
	  cblc_bitmap_ਮਾਪ_subset_context_t *bitmap_ਮਾਪ_context) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    hb_ਸਦਿਸ਼_t<hb_pair_t<hb_codepoint_t, ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ*>> lookup;
    build_lookup (c, bitmap_ਮਾਪ_context, &lookup);
    ਜੇ (unlikely (!c->serializer->propagate_error (lookup)))
      ਵਾਪਸ ਝੂਠ;

    bitmap_ਮਾਪ_context->ਮਾਪ = 0;
    bitmap_ਮਾਪ_context->ਅੰਕ_tables = 0;
    hb_ਸਦਿਸ਼_t<ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ> ਰਿਕਾਰਡਾਂ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0; ਸ਼ੁਰੂ < lookup.ਲੰਮਾਈ;)
    {
      ਜੇ (unlikely (!lookup[ਸ਼ੁਰੂ].second->ਜੋੜੋ_new_ਰਿਕਾਰਡ (c, bitmap_ਮਾਪ_context, &lookup, ਇਹ, &ਸ਼ੁਰੂ, &ਰਿਕਾਰਡਾਂ)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ; i++)
	  c->serializer->ਕੱਢੋ_discard ();
	return_trace (ਝੂਠ);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_ਸਦਿਸ਼_t<hb_serialize_context_t::objidx_t> objidxs;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ; i++)
      objidxs.push (c->serializer->ਕੱਢੋ_pack ());
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ; i++)
    {
      ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ* ਰਿਕਾਰਡ = c->serializer->embed (ਰਿਕਾਰਡਾਂ[i]);
      ਜੇ (unlikely (!ਰਿਕਾਰਡ)) return_trace (ਝੂਠ);
      c->serializer->ਜੋੜੋ_link (ਰਿਕਾਰਡ->ਵਿੱਥ_ਮੰਜ਼ਿਲ_Subtable, objidxs[ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ - 1 - i]);
    }
    return_trace (ਸੱਚ);
  }

  ਜਨਤਕ:
  ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ* ਲੱਭੋ_ਸਾਰਣੀ (hb_codepoint_t glyph, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_Tables) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_Tables; ++i)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_Subtables_Z[i].ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_Subtables_Z[i].ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ;
      ਜੇ (ਪਹਿਲਾ_Glyph_ਸੂਚਕਾਂਕ <= glyph && glyph <= ਆਖਰੀ_Glyph_ਸੂਚਕਾਂਕ)
	ਵਾਪਸ &ਸੂਚਕਾਂਕ_Subtables_Z[i];
    }
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸੁਰੱਖਿਅਤ:
  Unsized_Array_ਦਾ<ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ>	ਸੂਚਕਾਂਕ_Subtables_Z;
};

ਬਣਤਰ Bitmap_ਮਾਪ_ਸਾਰਣੀ
{
  ਮਿੱਤਰ ਬਣਤਰ CBLC;
  ਮਿੱਤਰ ਬਣਤਰ CBDT;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਸੂਚਕਾਂਕ_Subtable_Array_ਵਿੱਥ.sanitize (c, ਅਧਾਰ, ਸੰਖਿਆ_ਦਾ_ਸੂਚਕਾਂਕ_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ *
  ਲੱਭੋ_ਸਾਰਣੀ (hb_codepoint_t glyph, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, ਅਚਲ ਖਾਲੀ **ਬਾਹਰਲਾ_ਅਧਾਰ) ਅਚਲ
  {
    *ਬਾਹਰਲਾ_ਅਧਾਰ = &(ਅਧਾਰ+ਸੂਚਕਾਂਕ_Subtable_Array_ਵਿੱਥ);
    ਵਾਪਸ (ਅਧਾਰ+ਸੂਚਕਾਂਕ_Subtable_Array_ਵਿੱਥ).ਲੱਭੋ_ਸਾਰਣੀ (glyph, ਸੰਖਿਆ_ਦਾ_ਸੂਚਕਾਂਕ_Subtables);
  }

  ਤਾਰਕਿਕ
  subset (hb_subset_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
	  ਅਚਲ ਅੱਖਰ *cbdt, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cbdt_ਲੰਮਾਈ,
	  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> *cbdt_ਅਭਾਜ /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ_ਸਾਰਣੀ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ_ਸਾਰਣੀ)) return_trace (ਝੂਠ);

    cblc_bitmap_ਮਾਪ_subset_context_t bitmap_ਮਾਪ_context;
    bitmap_ਮਾਪ_context.cbdt = cbdt;
    bitmap_ਮਾਪ_context.cbdt_ਲੰਮਾਈ = cbdt_ਲੰਮਾਈ;
    bitmap_ਮਾਪ_context.cbdt_ਅਭਾਜ = cbdt_ਅਭਾਜ;
    bitmap_ਮਾਪ_context.ਮਾਪ = ਸੂਚਕਾਂਕ_Tables_ਮਾਪ;
    bitmap_ਮਾਪ_context.ਅੰਕ_tables = ਸੰਖਿਆ_ਦਾ_ਸੂਚਕਾਂਕ_Subtables;
    bitmap_ਮਾਪ_context.ਸ਼ੁਰੂ_glyph = 1;
    bitmap_ਮਾਪ_context.ਸਿਰਾ_glyph = 0;
    bitmap_ਮਾਪ_context.dedup = dedup;

    ਜੇ (!ਬਾਹਰਲਾ_ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_Subtable_Array_ਵਿੱਥ.serialize_subset (c,
							       ਸੂਚਕਾਂਕ_Subtable_Array_ਵਿੱਥ,
							       ਅਧਾਰ,
							       &bitmap_ਮਾਪ_context))
      return_trace (ਝੂਠ);
    ਜੇ (!bitmap_ਮਾਪ_context.ਮਾਪ ||
	!bitmap_ਮਾਪ_context.ਅੰਕ_tables ||
	bitmap_ਮਾਪ_context.ਸ਼ੁਰੂ_glyph > bitmap_ਮਾਪ_context.ਸਿਰਾ_glyph)
      return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ_ਸਾਰਣੀ->ਸੂਚਕਾਂਕ_Tables_ਮਾਪ = bitmap_ਮਾਪ_context.ਮਾਪ;
    ਬਾਹਰਲਾ_ਸਾਰਣੀ->ਸੰਖਿਆ_ਦਾ_ਸੂਚਕਾਂਕ_Subtables = bitmap_ਮਾਪ_context.ਅੰਕ_tables;
    ਬਾਹਰਲਾ_ਸਾਰਣੀ->ਸ਼ੁਰੂ_Glyph_ਸੂਚਕਾਂਕ = bitmap_ਮਾਪ_context.ਸ਼ੁਰੂ_glyph;
    ਬਾਹਰਲਾ_ਸਾਰਣੀ->ਸਿਰਾ_Glyph_ਸੂਚਕਾਂਕ = bitmap_ਮਾਪ_context.ਸਿਰਾ_glyph;
    return_trace (ਸੱਚ);
  }

  ਸੁਰੱਖਿਅਤ:
  NNOffset32_ਮੰਜ਼ਿਲ<ਸੂਚਕਾਂਕ_Subtable_Array>
			ਸੂਚਕਾਂਕ_Subtable_Array_ਵਿੱਥ;
  HBUINT32		ਸੂਚਕਾਂਕ_Tables_ਮਾਪ;
  HBUINT32		ਸੰਖਿਆ_ਦਾ_ਸੂਚਕਾਂਕ_Subtables;
  HBUINT32		colorRef;
  SBit_ਸਤਰ_Metrics	horizontal;
  SBit_ਸਤਰ_Metrics	vertical;
  HBGlyphID16		ਸ਼ੁਰੂ_Glyph_ਸੂਚਕਾਂਕ;
  HBGlyphID16		ਸਿਰਾ_Glyph_ਸੂਚਕਾਂਕ;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		ਦੋਆਧਾਰੀ_ਅੰਕ_ਡੂੰਘਾਈ;
  HBINT8		flags;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

ਬਣਤਰ Glyph_Bitmap_ਅੰਕੜੇ_Format17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32_ਦਾ<HBUINT8>	ਅੰਕੜੇ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (9, ਅੰਕੜੇ);
};

ਬਣਤਰ Glyph_Bitmap_ਅੰਕੜੇ_Format18
{
  BigGlyphMetrics	glyphMetrics;
  Array32_ਦਾ<HBUINT8>	ਅੰਕੜੇ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (12, ਅੰਕੜੇ);
};

ਬਣਤਰ Glyph_Bitmap_ਅੰਕੜੇ_Format19
{
  Array32_ਦਾ<HBUINT8>	ਅੰਕੜੇ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, ਅੰਕੜੇ);
};

ਬਣਤਰ CBLC
{
  ਮਿੱਤਰ ਬਣਤਰ CBDT;

  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_CBLC;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 2 || ਸੰਸਕਰਣ.major == 3) &&
		  hb_barrier () &&
		  ਮਾਪ_Tables.sanitize (c, ਇਹ));
  }

  ਸਥਿਰ ਤਾਰਕਿਕ
  sink_cbdt (hb_subset_context_t *c, hb_ਸਦਿਸ਼_t<ਅੱਖਰ>* cbdt_ਅਭਾਜ)
  {
    hb_blob_t *cbdt_ਅਭਾਜ_blob = hb_blob_create (cbdt_ਅਭਾਜ->arrayZ,
						 cbdt_ਅਭਾਜ->ਲੰਮਾਈ,
						 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE,
						 cbdt_ਅਭਾਜ->arrayZ,
						 hb_free);
    cbdt_ਅਭਾਜ->init ();  // Leak arrayZ to the blob.
    ਤਾਰਕਿਕ ret = c->plan->ਜੋੜੋ_ਸਾਰਣੀ (HB_OT_ਟੈਗ_CBDT, cbdt_ਅਭਾਜ_blob);
    hb_blob_destroy (cbdt_ਅਭਾਜ_blob);
    ਵਾਪਸ ret;
  }

  ਤਾਰਕਿਕ
  subset_ਮਾਪ_ਸਾਰਣੀ (hb_subset_context_t *c, ਅਚਲ Bitmap_ਮਾਪ_ਸਾਰਣੀ& ਸਾਰਣੀ,
		     ਅਚਲ ਅੱਖਰ *cbdt /*ਵਿੱਚ*/, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cbdt_ਲੰਮਾਈ,
		     CBLC *cblc_ਅਭਾਜ /* INOUT */, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> *cbdt_ਅਭਾਜ /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    cblc_ਅਭਾਜ->ਮਾਪ_Tables.len++;

    ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
    ਸਵੈਚਾਲਿਤ cbdt_ਅਭਾਜ_len = cbdt_ਅਭਾਜ->ਲੰਮਾਈ;

    ਜੇ (!ਸਾਰਣੀ.subset (c, ਇਹ, cbdt, cbdt_ਲੰਮਾਈ, cbdt_ਅਭਾਜ, dedup))
    {
      cblc_ਅਭਾਜ->ਮਾਪ_Tables.len--;
      c->serializer->revert (snap);
      cbdt_ਅਭਾਜ->shrink (cbdt_ਅਭਾਜ_len);
      return_trace (ਝੂਠ);
    }
    return_trace (ਸੱਚ);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ;

  ਸੁਰੱਖਿਅਤ:
  ਅਚਲ Bitmap_ਮਾਪ_ਸਾਰਣੀ &ਚੁਣੋ_strike (hb_font_t *font) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਮਾਪ_Tables.len;
    ਜੇ (unlikely (!ਗਿਣਤੀ_2))
      ਵਾਪਸ Null (Bitmap_ਮਾਪ_ਸਾਰਣੀ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    ਜੇ (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉੱਤਮ_i = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉੱਤਮ_ppem = hb_max (ਮਾਪ_Tables[0].ppemX, ਮਾਪ_Tables[0].ppemY);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ppem = hb_max (ਮਾਪ_Tables[i].ppemX, ਮਾਪ_Tables[i].ppemY);
      ਜੇ ((requested_ppem <= ppem && ppem < ਉੱਤਮ_ppem) ||
	  (requested_ppem > ਉੱਤਮ_ppem && ppem > ਉੱਤਮ_ppem))
      {
	ਉੱਤਮ_i = i;
	ਉੱਤਮ_ppem = ppem;
      }
    }

    ਵਾਪਸ ਮਾਪ_Tables[ਉੱਤਮ_i];
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>		ਸੰਸਕਰਣ;
  Array32_ਦਾ<Bitmap_ਮਾਪ_ਸਾਰਣੀ>	ਮਾਪ_Tables;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, ਮਾਪ_Tables);
};

ਬਣਤਰ CBDT
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_CBDT;

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ਇਹ->cblc = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<CBLC> (face);
      ਇਹ->cbdt = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      ਇਹ->cblc.destroy ();
      ਇਹ->cbdt.destroy ();
    }

    ਤਾਰਕਿਕ
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, ਤਾਰਕਿਕ scale = ਸੱਚ) ਅਚਲ
    {
      ਅਚਲ ਖਾਲੀ *ਅਧਾਰ;
      ਅਚਲ Bitmap_ਮਾਪ_ਸਾਰਣੀ &strike = ਇਹ->cblc->ਚੁਣੋ_strike (font);
      ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ *subtable_ਰਿਕਾਰਡ = strike.ਲੱਭੋ_ਸਾਰਣੀ (glyph, cblc, &ਅਧਾਰ);
      ਜੇ (!subtable_ਰਿਕਾਰਡ || !strike.ppemX || !strike.ppemY)
	ਵਾਪਸ ਝੂਠ;

      ਜੇ (subtable_ਰਿਕਾਰਡ->get_extents (extents, ਅਧਾਰ, scale))
	ਵਾਪਸ ਸੱਚ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ image_ਵਿੱਥ = 0, image_ਲੰਮਾਈ = 0, image_format = 0;
      ਜੇ (!subtable_ਰਿਕਾਰਡ->get_image_ਅੰਕੜੇ (glyph, ਅਧਾਰ, &image_ਵਿੱਥ, &image_ਲੰਮਾਈ, &image_format))
	ਵਾਪਸ ਝੂਠ;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cbdt_len = cbdt.get_ਲੰਮਾਈ ();
      ਜੇ (unlikely (image_ਵਿੱਥ > cbdt_len || cbdt_len - image_ਵਿੱਥ < image_ਲੰਮਾਈ))
	ਵਾਪਸ ਝੂਠ;

      ਚੋਣ (image_format)
      {
      ਹਾਲਤ 17: {
	ਜੇ (unlikely (image_ਲੰਮਾਈ < Glyph_Bitmap_ਅੰਕੜੇ_Format17::min_ਮਾਪ))
	  ਵਾਪਸ ਝੂਠ;
	ਸਵੈਚਾਲਿਤ &glyphFormat17 = Struct_At_ਵਿੱਥ<Glyph_Bitmap_ਅੰਕੜੇ_Format17> (ਇਹ->cbdt, image_ਵਿੱਥ);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	ਰੋਕੋ;
      }
      ਹਾਲਤ 18: {
	ਜੇ (unlikely (image_ਲੰਮਾਈ < Glyph_Bitmap_ਅੰਕੜੇ_Format18::min_ਮਾਪ))
	  ਵਾਪਸ ਝੂਠ;
	ਸਵੈਚਾਲਿਤ &glyphFormat18 = Struct_At_ਵਿੱਥ<Glyph_Bitmap_ਅੰਕੜੇ_Format18> (ਇਹ->cbdt, image_ਵਿੱਥ);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	ਰੋਕੋ;
      }
      ਮੂਲ: ਵਾਪਸ ਝੂਠ; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      ਜੇ (scale)
      {
	ਦਸ਼ਮਲਵ x_scale = upem / (ਦਸ਼ਮਲਵ) strike.ppemX;
	ਦਸ਼ਮਲਵ y_scale = upem / (ਦਸ਼ਮਲਵ) strike.ppemY;
	extents->x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->ਚੌੜਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (roundf (extents->ਚੌੜਾਈ * x_scale));
	extents->ਉਚਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (roundf (extents->ਉਚਾਈ * y_scale));
      }

      ਵਾਪਸ ਸੱਚ;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) ਅਚਲ
    {
      ਅਚਲ ਖਾਲੀ *ਅਧਾਰ;
      ਅਚਲ Bitmap_ਮਾਪ_ਸਾਰਣੀ &strike = ਇਹ->cblc->ਚੁਣੋ_strike (font);
      ਅਚਲ ਸੂਚਕਾਂਕ_Subtable_ਰਿਕਾਰਡ *subtable_ਰਿਕਾਰਡ = strike.ਲੱਭੋ_ਸਾਰਣੀ (glyph, cblc, &ਅਧਾਰ);
      ਜੇ (!subtable_ਰਿਕਾਰਡ || !strike.ppemX || !strike.ppemY)
	ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ image_ਵਿੱਥ = 0, image_ਲੰਮਾਈ = 0, image_format = 0;
      ਜੇ (!subtable_ਰਿਕਾਰਡ->get_image_ਅੰਕੜੇ (glyph, ਅਧਾਰ, &image_ਵਿੱਥ, &image_ਲੰਮਾਈ, &image_format))
	ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cbdt_len = cbdt.get_ਲੰਮਾਈ ();
      ਜੇ (unlikely (image_ਵਿੱਥ > cbdt_len || cbdt_len - image_ਵਿੱਥ < image_ਲੰਮਾਈ))
	ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

      ਚੋਣ (image_format)
      {
      ਹਾਲਤ 17:
      {
	ਜੇ (unlikely (image_ਲੰਮਾਈ < Glyph_Bitmap_ਅੰਕੜੇ_Format17::min_ਮਾਪ))
	  ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
	ਸਵੈਚਾਲਿਤ &glyphFormat17 = Struct_At_ਵਿੱਥ<Glyph_Bitmap_ਅੰਕੜੇ_Format17> (ਇਹ->cbdt, image_ਵਿੱਥ);
	ਵਾਪਸ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_ਵਿੱਥ + Glyph_Bitmap_ਅੰਕੜੇ_Format17::min_ਮਾਪ,
					glyphFormat17.ਅੰਕੜੇ.len);
      }
      ਹਾਲਤ 18:
      {
	ਜੇ (unlikely (image_ਲੰਮਾਈ < Glyph_Bitmap_ਅੰਕੜੇ_Format18::min_ਮਾਪ))
	  ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
	ਸਵੈਚਾਲਿਤ &glyphFormat18 = Struct_At_ਵਿੱਥ<Glyph_Bitmap_ਅੰਕੜੇ_Format18> (ਇਹ->cbdt, image_ਵਿੱਥ);
	ਵਾਪਸ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_ਵਿੱਥ + Glyph_Bitmap_ਅੰਕੜੇ_Format18::min_ਮਾਪ,
					glyphFormat18.ਅੰਕੜੇ.len);
      }
      ਹਾਲਤ 19:
      {
	ਜੇ (unlikely (image_ਲੰਮਾਈ < Glyph_Bitmap_ਅੰਕੜੇ_Format19::min_ਮਾਪ))
	  ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
	ਸਵੈਚਾਲਿਤ &glyphFormat19 = Struct_At_ਵਿੱਥ<Glyph_Bitmap_ਅੰਕੜੇ_Format19> (ਇਹ->cbdt, image_ਵਿੱਥ);
	ਵਾਪਸ hb_blob_create_sub_blob (cbdt.get_blob (),
					image_ਵਿੱਥ + Glyph_Bitmap_ਅੰਕੜੇ_Format19::min_ਮਾਪ,
					glyphFormat19.ਅੰਕੜੇ.len);
      }
      ਮੂਲ: ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ (); /* TODO: Support other image formats. */
      }
    }

    ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ cbdt->ਸੰਸਕਰਣ.major; }

    ਤਾਰਕਿਕ paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ਖਾਲੀ *ਅੰਕੜੇ) ਅਚਲ
    {
      ਜੇ (!has_ਅੰਕੜੇ ()) ਵਾਪਸ ਝੂਠ;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      ਜੇ (unlikely (!font->get_glyph_extents (glyph, &extents, ਝੂਠ)))
        ਵਾਪਸ ਝੂਠ;

      ਜੇ (unlikely (!get_extents (font, glyph, &pixel_extents, ਝੂਠ)))
        ਵਾਪਸ ਝੂਠ;

      hb_blob_t *blob = reference_png (font, glyph);
      ਜੇ (unlikely (hb_blob_is_immutable (blob)))
        ਵਾਪਸ ਝੂਠ;

      ਤਾਰਕਿਕ ret = funcs->image (ਅੰਕੜੇ,
			       blob,
			       pixel_extents.ਚੌੜਾਈ, -pixel_extents.ਉਚਾਈ,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      ਵਾਪਸ ret;
    }

    ਨਿਜੀ:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ upem;
  };

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 2 || ਸੰਸਕਰਣ.major == 3));
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>		ਸੰਸਕਰਣ;
  Unsized_Array_ਦਾ<HBUINT8>	ਅੰਕੜੇ_Z;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, ਅੰਕੜੇ_Z);
};

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
CBLC::subset (hb_subset_context_t *c) ਅਚਲ
{
  TRACE_SUBSET (ਇਹ);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> cbdt_ਅਭਾਜ;

  ਸਵੈਚਾਲਿਤ *cblc_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed<CBLC> ();
  ਜੇ (unlikely (!c->serializer->extend_min (cblc_ਅਭਾਜ))) return_trace (ਝੂਠ);
  cblc_ਅਭਾਜ->ਸੰਸਕਰਣ = ਸੰਸਕਰਣ;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<CBDT> (c->plan->ਸਰੋਤ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cbdt_ਲੰਮਾਈ;
  CBDT* cbdt = (CBDT *) hb_blob_get_ਅੰਕੜੇ (cbdt_blob, &cbdt_ਲੰਮਾਈ);
  ਜੇ (unlikely (cbdt_ਲੰਮਾਈ < CBDT::min_ਮਾਪ))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (ਝੂਠ);
  }
  copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_cbdt (&cbdt_ਅਭਾਜ, cbdt, CBDT::min_ਮਾਪ);

  cbdt_dedup_context_t dedup;
  ਲਈ (ਅਚਲ Bitmap_ਮਾਪ_ਸਾਰਣੀ& ਸਾਰਣੀ : + ਮਾਪ_Tables.iter ())
    subset_ਮਾਪ_ਸਾਰਣੀ (c, ਸਾਰਣੀ, (ਅਚਲ ਅੱਖਰ *) cbdt, cbdt_ਲੰਮਾਈ, cblc_ਅਭਾਜ, &cbdt_ਅਭਾਜ, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_ਅਭਾਜ));
}

ਬਣਤਰ CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#ਜੇ_ਅੰਤ /* OT_COLOR_CBDT_CBDT_HH */
