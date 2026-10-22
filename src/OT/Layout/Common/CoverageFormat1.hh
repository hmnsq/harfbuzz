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

नामाकाश OT {
नामाकाश Layout {
नामाकाश Common {

#define NOT_COVERED             ((unsigned पूर्णाङ्क) -1)

template <typename Types>
संरचना CoverageFormat1_3
{
  friend संरचना Coverage;

  सार्वजनिक:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  गुप्त:
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned पूर्णाङ्क get_coverage (hb_codepoint_t glyph_id) ध्रुव
  {
    unsigned पूर्णाङ्क i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    प्रत्यागम i;
  }

  unsigned get_population () ध्रुव
  {
    प्रत्यागम glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    यदि (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      प्रति (auto g : *glyphs)
        यदि (get_coverage (g) != NOT_COVERED)
	  प्रत्यागम सत्य;
      प्रत्यागम असत्य;
    }

    प्रति (ध्रुव auto& g : glyphArray.as_array ())
      यदि (glyphs->has (g))
        प्रत्यागम सत्य;
    प्रत्यागम असत्य;
  }
  तार्किक intersects_coverage (ध्रुव hb_set_t *glyphs, unsigned पूर्णाङ्क index) ध्रुव
  { प्रत्यागम glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  शून्य intersect_set (ध्रुव hb_set_t &glyphs, IterableOut&& intersect_glyphs) ध्रुव
  {
    unsigned मोजणी = glyphArray.len;
    प्रति (unsigned i = 0; i < मोजणी; i++)
      यदि (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () ध्रुव { प्रत्यागम hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) ध्रुव
  { प्रत्यागम glyphs->add_sorted_array (glyphArray.as_array ()); }

  सार्वजनिक:
  /* Older compilers need this to be public. */
  संरचना iter_t
  {
    शून्य init (ध्रुव संरचना CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    तार्किक __more__ () ध्रुव { प्रत्यागम i < c->glyphArray.len; }
    शून्य __next__ () { i++; }
    hb_codepoint_t get_glyph () ध्रुव { प्रत्यागम c->glyphArray[i]; }
    तार्किक operator != (ध्रुव iter_t& o) ध्रुव
    { प्रत्यागम i != o.i; }
    iter_t __end__ () ध्रुव { iter_t it; it.init (*c); it.i = c->glyphArray.len; प्रत्यागम it; }

    गुप्त:
    ध्रुव संरचना CoverageFormat1_3 *c;
    unsigned पूर्णाङ्क i;
  };
  गुप्त:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
