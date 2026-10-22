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

#define NOT_COVERED             ((unsigned পূর্ণসংখ্যা) -1)

template <typename Types>
কাঠামো CoverageFormat1_3
{
  friend কাঠামো Coverage;

  সর্বজনীন:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_আকার_ARRAY (2 + Types::HBUINT::static_আকার, glyphArray);

  ব্যক্তিগত:
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned পূর্ণসংখ্যা get_coverage (hb_codepoint_t glyph_id) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    ফেরত i;
  }

  unsigned get_population () ধ্রুবক
  {
    ফেরত glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (এই);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    যদি (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      জন্য (auto g : *glyphs)
        যদি (get_coverage (g) != NOT_COVERED)
	  ফেরত সত্য;
      ফেরত মিথ্যা;
    }

    জন্য (ধ্রুবক auto& g : glyphArray.as_array ())
      যদি (glyphs->has (g))
        ফেরত সত্য;
    ফেরত মিথ্যা;
  }
  যৌক্তিক intersects_coverage (ধ্রুবক hb_set_t *glyphs, unsigned পূর্ণসংখ্যা সূচক_2) ধ্রুবক
  { ফেরত glyphs->has (glyphArray[সূচক_2]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  শূন্য intersect_set (ধ্রুবক hb_set_t &glyphs, IterableOut&& intersect_glyphs) ধ্রুবক
  {
    unsigned গুনতি = glyphArray.len;
    জন্য (unsigned i = 0; i < গুনতি; i++)
      যদি (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () ধ্রুবক { ফেরত hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  যৌক্তিক collect_coverage (set_t *glyphs) ধ্রুবক
  { ফেরত glyphs->add_sorted_array (glyphArray.as_array ()); }

  সর্বজনীন:
  /* Older compilers need this to be public. */
  কাঠামো iter_t
  {
    শূন্য init (ধ্রুবক কাঠামো CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    যৌক্তিক __more__ () ধ্রুবক { ফেরত i < c->glyphArray.len; }
    শূন্য __next__ () { i++; }
    hb_codepoint_t get_glyph () ধ্রুবক { ফেরত c->glyphArray[i]; }
    যৌক্তিক operator != (ধ্রুবক iter_t& o) ধ্রুবক
    { ফেরত i != o.i; }
    iter_t __end__ () ধ্রুবক { iter_t it; it.init (*c); it.i = c->glyphArray.len; ফেরত it; }

    ব্যক্তিগত:
    ধ্রুবক কাঠামো CoverageFormat1_3 *c;
    unsigned পূর্ণসংখ্যা i;
  };
  ব্যক্তিগত:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
