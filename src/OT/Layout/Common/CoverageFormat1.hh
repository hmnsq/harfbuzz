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

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ Common {

#define NOT_COVERED             ((unsigned ಪೂರ್ಣಾಙ್ಕ) -1)

template <typename Types>
ಸಂರಚನಾ CoverageFormat1_3
{
  friend ಸಂರಚನಾ Coverage;

  ಸಾರ್ವಜನಿಕ:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  ಗುಪ್ತ:
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_coverage (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    ಪ್ರತ್ಯಾಗಮ i;
  }

  unsigned get_population () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಯದಿ (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      ಪ್ರತಿ (auto g : *glyphs)
        ಯದಿ (get_coverage (g) != NOT_COVERED)
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತಿ (ಧ್ರುವ auto& g : glyphArray.as_array ())
      ಯದಿ (glyphs->has (g))
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ intersects_coverage (ಧ್ರುವ hb_set_t *glyphs, unsigned ಪೂರ್ಣಾಙ್ಕ index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  ಶೂನ್ಯ intersect_set (ಧ್ರುವ hb_set_t &glyphs, IterableOut&& intersect_glyphs) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = glyphArray.len;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  ತಾರ್ಕಿಕ collect_coverage (set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ glyphs->add_sorted_array (glyphArray.as_array ()); }

  ಸಾರ್ವಜನಿಕ:
  /* Older compilers need this to be public. */
  ಸಂರಚನಾ iter_t
  {
    ಶೂನ್ಯ init (ಧ್ರುವ ಸಂರಚನಾ CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ i < c->glyphArray.len; }
    ಶೂನ್ಯ __next__ () { i++; }
    hb_codepoint_t get_glyph () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ c->glyphArray[i]; }
    ತಾರ್ಕಿಕ operator != (ಧ್ರುವ iter_t& o) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ i != o.i; }
    iter_t __end__ () ಧ್ರುವ { iter_t it; it.init (*c); it.i = c->glyphArray.len; ಪ್ರತ್ಯಾಗಮ it; }

    ಗುಪ್ತ:
    ಧ್ರುವ ಸಂರಚನಾ CoverageFormat1_3 *c;
    unsigned ಪೂರ್ಣಾಙ್ಕ i;
  };
  ಗುಪ್ತ:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
