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

نطاق OT {
نطاق Layout {
نطاق Common {

#define NOT_COVERED             ((unsigned صحيح) -1)

template <typename Types>
بنية CoverageFormat1_3
{
  friend بنية Coverage;

  عام:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  خاص:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    unsigned صحيح i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    أرجع i;
  }

  unsigned get_population () ثابت
  {
    أرجع glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    إذا (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      لكل (auto g : *glyphs)
        إذا (get_coverage (g) != NOT_COVERED)
	  أرجع true;
      أرجع خطأ;
    }

    لكل (ثابت auto& g : glyphArray.as_array ())
      إذا (glyphs->has (g))
        أرجع true;
    أرجع خطأ;
  }
  منطقي intersects_coverage (ثابت hb_set_t *glyphs, unsigned صحيح index) ثابت
  { أرجع glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  فراغ intersect_set (ثابت hb_set_t &glyphs, IterableOut&& intersect_glyphs) ثابت
  {
    unsigned عدد = glyphArray.len;
    لكل (unsigned i = 0; i < عدد; i++)
      إذا (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () ثابت { أرجع hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  منطقي collect_coverage (set_t *glyphs) ثابت
  { أرجع glyphs->add_sorted_array (glyphArray.as_array ()); }

  عام:
  /* Older compilers need this to be public. */
  بنية iter_t
  {
    فراغ init (ثابت بنية CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    منطقي __more__ () ثابت { أرجع i < c->glyphArray.len; }
    فراغ __next__ () { i++; }
    hb_codepoint_t get_glyph () ثابت { أرجع c->glyphArray[i]; }
    منطقي operator != (ثابت iter_t& o) ثابت
    { أرجع i != o.i; }
    iter_t __end__ () ثابت { iter_t it; it.init (*c); it.i = c->glyphArray.len; أرجع it; }

    خاص:
    ثابت بنية CoverageFormat1_3 *c;
    unsigned صحيح i;
  };
  خاص:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
