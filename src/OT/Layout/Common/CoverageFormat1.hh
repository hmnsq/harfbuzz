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

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ Common {

#define NOT_COVERED             ((unsigned முழு) -1)

template <typename Types>
கட்டமைப்பு CoverageFormat1_3
{
  friend கட்டமைப்பு Coverage;

  பொது:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  தனிப்பட்ட:
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned முழு get_coverage (hb_codepoint_t glyph_id) மாறிலி
  {
    unsigned முழு i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    திரும்பு i;
  }

  unsigned get_population () மாறிலி
  {
    திரும்பு glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (இது);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    எனில் (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      க்கு (auto g : *glyphs)
        எனில் (get_coverage (g) != NOT_COVERED)
	  திரும்பு உண்மை;
      திரும்பு பொய்;
    }

    க்கு (மாறிலி auto& g : glyphArray.as_array ())
      எனில் (glyphs->has (g))
        திரும்பு உண்மை;
    திரும்பு பொய்;
  }
  தருக்க intersects_coverage (மாறிலி hb_set_t *glyphs, unsigned முழு index) மாறிலி
  { திரும்பு glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  வெறுமை intersect_set (மாறிலி hb_set_t &glyphs, IterableOut&& intersect_glyphs) மாறிலி
  {
    unsigned மொத்தம் = glyphArray.len;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      எனில் (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () மாறிலி { திரும்பு hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  தருக்க collect_coverage (set_t *glyphs) மாறிலி
  { திரும்பு glyphs->add_sorted_array (glyphArray.as_array ()); }

  பொது:
  /* Older compilers need this to be public. */
  கட்டமைப்பு iter_t
  {
    வெறுமை init (மாறிலி கட்டமைப்பு CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    தருக்க __more__ () மாறிலி { திரும்பு i < c->glyphArray.len; }
    வெறுமை __next__ () { i++; }
    hb_codepoint_t get_glyph () மாறிலி { திரும்பு c->glyphArray[i]; }
    தருக்க operator != (மாறிலி iter_t& o) மாறிலி
    { திரும்பு i != o.i; }
    iter_t __end__ () மாறிலி { iter_t it; it.init (*c); it.i = c->glyphArray.len; திரும்பு it; }

    தனிப்பட்ட:
    மாறிலி கட்டமைப்பு CoverageFormat1_3 *c;
    unsigned முழு i;
  };
  தனிப்பட்ட:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
