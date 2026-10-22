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

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ Common {

#define NOT_COVERED             ((unsigned পূর্ণাঙ্ক) -1)

template <typename Types>
সংরচনা CoverageFormat1_3
{
  friend সংরচনা Coverage;

  সার্বজনিক:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  গুপ্ত:
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned পূর্ণাঙ্ক get_coverage (hb_codepoint_t glyph_id) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    প্রত্যাগম i;
  }

  unsigned get_population () ধ্রুব
  {
    প্রত্যাগম glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  {
    যদি (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      প্রতি (auto g : *glyphs)
        যদি (get_coverage (g) != NOT_COVERED)
	  প্রত্যাগম সত্য;
      প্রত্যাগম অসত্য;
    }

    প্রতি (ধ্রুব auto& g : glyphArray.as_array ())
      যদি (glyphs->has (g))
        প্রত্যাগম সত্য;
    প্রত্যাগম অসত্য;
  }
  তার্কিক intersects_coverage (ধ্রুব hb_set_t *glyphs, unsigned পূর্ণাঙ্ক index) ধ্রুব
  { প্রত্যাগম glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  শূন্য intersect_set (ধ্রুব hb_set_t &glyphs, IterableOut&& intersect_glyphs) ধ্রুব
  {
    unsigned গুনতি = glyphArray.len;
    প্রতি (unsigned i = 0; i < গুনতি; i++)
      যদি (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () ধ্রুব { প্রত্যাগম hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  তার্কিক collect_coverage (set_t *glyphs) ধ্রুব
  { প্রত্যাগম glyphs->add_sorted_array (glyphArray.as_array ()); }

  সার্বজনিক:
  /* Older compilers need this to be public. */
  সংরচনা iter_t
  {
    শূন্য init (ধ্রুব সংরচনা CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    তার্কিক __more__ () ধ্রুব { প্রত্যাগম i < c->glyphArray.len; }
    শূন্য __next__ () { i++; }
    hb_codepoint_t get_glyph () ধ্রুব { প্রত্যাগম c->glyphArray[i]; }
    তার্কিক operator != (ধ্রুব iter_t& o) ধ্রুব
    { প্রত্যাগম i != o.i; }
    iter_t __end__ () ধ্রুব { iter_t it; it.init (*c); it.i = c->glyphArray.len; প্রত্যাগম it; }

    গুপ্ত:
    ধ্রুব সংরচনা CoverageFormat1_3 *c;
    unsigned পূর্ণাঙ্ক i;
  };
  গুপ্ত:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
