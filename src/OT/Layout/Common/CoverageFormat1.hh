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

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ Common {

#define NOT_COVERED             ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) -1)

template <typename Types>
ସଂରଚନା CoverageFormat1_3
{
  friend ସଂରଚନା Coverage;

  ସର୍ବସାଧାରଣ:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  ବ୍ୟକ୍ତିଗତ:
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_coverage (hb_codepoint_t glyph_id) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    ଫେରନ୍ତୁ i;
  }

  unsigned get_population () ଅଚଳ
  {
    ଫେରନ୍ତୁ glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଯଦି (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      ପାଇଁ (auto g : *glyphs)
        ଯଦି (get_coverage (g) != NOT_COVERED)
	  ଫେରନ୍ତୁ ସତ୍ୟ;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ପାଇଁ (ଅଚଳ auto& g : glyphArray.as_array ())
      ଯଦି (glyphs->has (g))
        ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ intersects_coverage (ଅଚଳ hb_set_t *glyphs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index) ଅଚଳ
  { ଫେରନ୍ତୁ glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  ଶୂନ୍ୟ intersect_set (ଅଚଳ hb_set_t &glyphs, IterableOut&& intersect_glyphs) ଅଚଳ
  {
    unsigned ଗଣନା_2 = glyphArray.len;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      ଯଦି (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () ଅଚଳ { ଫେରନ୍ତୁ hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  ତାର୍କିକ collect_coverage (set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ glyphs->add_sorted_array (glyphArray.as_array ()); }

  ସର୍ବସାଧାରଣ:
  /* Older compilers need this to be public. */
  ସଂରଚନା iter_t
  {
    ଶୂନ୍ୟ init (ଅଚଳ ସଂରଚନା CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ i < c->glyphArray.len; }
    ଶୂନ୍ୟ __next__ () { i++; }
    hb_codepoint_t get_glyph () ଅଚଳ { ଫେରନ୍ତୁ c->glyphArray[i]; }
    ତାର୍କିକ operator != (ଅଚଳ iter_t& o) ଅଚଳ
    { ଫେରନ୍ତୁ i != o.i; }
    iter_t __end__ () ଅଚଳ { iter_t it; it.init (*c); it.i = c->glyphArray.len; ଫେରନ୍ତୁ it; }

    ବ୍ୟକ୍ତିଗତ:
    ଅଚଳ ସଂରଚନା CoverageFormat1_3 *c;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  };
  ବ୍ୟକ୍ତିଗତ:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
