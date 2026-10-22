/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod, Garret Rieger
 */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ Common {

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOT_COVERED             ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1)

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ CoverageFormat1_3
{
  ਮਿੱਤਰ ਬਣਤਰ Coverage;

  ਜਨਤਕ:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Sorted_Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_ਮਾਪ_ARRAY (2 + Types::HBUINT::static_ਮਾਪ, glyphArray);

  ਨਿਜੀ:
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (glyphArray.sanitize (c));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_coverage (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_ਮਿਲਿਆ_STORE, NOT_COVERED);
    ਵਾਪਸ i;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_population () ਅਚਲ
  {
    ਵਾਪਸ glyphArray.len;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
      hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਜੇ (glyphArray.len > glyphs->get_population () * hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage ((ਬਿਨਾਨਿਸ਼ਾਨੀ) glyphArray.len))
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ g : *glyphs)
        ਜੇ (get_coverage (g) != NOT_COVERED)
	  ਵਾਪਸ ਸੱਚ;
      ਵਾਪਸ ਝੂਠ;
    }

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& g : glyphArray.as_array ())
      ਜੇ (glyphs->has (g))
        ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }
  ਤਾਰਕਿਕ intersects_coverage (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2) ਅਚਲ
  { ਵਾਪਸ glyphs->has (glyphArray[ਸੂਚਕਾਂਕ_2]); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable_ਬਾਹਰਲਾ,
	    hb_requires (hb_is_sink_ਦਾ (Iterable_ਬਾਹਰਲਾ, hb_codepoint_t))>
  ਖਾਲੀ intersect_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t &glyphs, Iterable_ਬਾਹਰਲਾ&& intersect_glyphs) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = glyphArray.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ () ਅਚਲ { ਵਾਪਸ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage ((ਬਿਨਾਨਿਸ਼ਾਨੀ) glyphArray.len); /* bsearch cost */ }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਤਾਰਕਿਕ collect_coverage (ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ glyphs->ਜੋੜੋ_sorted_array (glyphArray.as_array ()); }

  ਜਨਤਕ:
  /* Older compilers need this to be public. */
  ਬਣਤਰ iter_t
  {
    ਖਾਲੀ init (ਅਚਲ ਬਣਤਰ CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    ਤਾਰਕਿਕ __more__ () ਅਚਲ { ਵਾਪਸ i < c->glyphArray.len; }
    ਖਾਲੀ __next__ () { i++; }
    hb_codepoint_t get_glyph () ਅਚਲ { ਵਾਪਸ c->glyphArray[i]; }
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ iter_t& o) ਅਚਲ
    { ਵਾਪਸ i != o.i; }
    iter_t __end__ () ਅਚਲ { iter_t it; it.init (*c); it.i = c->glyphArray.len; ਵਾਪਸ it; }

    ਨਿਜੀ:
    ਅਚਲ ਬਣਤਰ CoverageFormat1_3 *c;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
  };
  ਨਿਜੀ:
};

}
}
}

#ਜੇ_ਅੰਤ  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
