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


#ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
#define OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH

فضاینام OT {
فضاینام Layout {
فضاینام Common {

#define NOT_COVERED             ((unsigned صحیح) -1)

template <typename Types>
ساختار CoverageFormat1_3
{
  friend ساختار Coverage;

  عمومی:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  خصوصی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned صحیح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    unsigned صحیح i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    بازگشت i;
  }

  unsigned get_population () ثابت
  {
    بازگشت glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (این);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    اگر (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      برای (auto g : *glyphs)
        اگر (get_coverage (g) != NOT_COVERED)
	  بازگشت درست;
      بازگشت نادرست;
    }

    برای (ثابت auto& g : glyphArray.as_array ())
      اگر (glyphs->has (g))
        بازگشت درست;
    بازگشت نادرست;
  }
  منطقی intersects_coverage (ثابت hb_set_t *glyphs, unsigned صحیح index) ثابت
  { بازگشت glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  تهی intersect_set (ثابت hb_set_t &glyphs, IterableOut&& intersect_glyphs) ثابت
  {
    unsigned شمار = glyphArray.len;
    برای (unsigned i = 0; i < شمار; i++)
      اگر (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () ثابت { بازگشت hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  منطقی collect_coverage (set_t *glyphs) ثابت
  { بازگشت glyphs->add_sorted_array (glyphArray.as_array ()); }

  عمومی:
  /* Older compilers need this to be public. */
  ساختار iter_t
  {
    تهی init (ثابت ساختار CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    منطقی __more__ () ثابت { بازگشت i < c->glyphArray.len; }
    تهی __next__ () { i++; }
    hb_codepoint_t get_glyph () ثابت { بازگشت c->glyphArray[i]; }
    منطقی operator != (ثابت iter_t& o) ثابت
    { بازگشت i != o.i; }
    iter_t __end__ () ثابت { iter_t it; it.init (*c); it.i = c->glyphArray.len; بازگشت it; }

    خصوصی:
    ثابت ساختار CoverageFormat1_3 *c;
    unsigned صحیح i;
  };
  خصوصی:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
