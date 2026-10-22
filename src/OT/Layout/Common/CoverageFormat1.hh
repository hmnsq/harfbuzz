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

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ Common {

#define NOT_COVERED             ((unsigned പൂർണ്ണസംഖ്യ) -1)

template <typename Types>
ഘടന CoverageFormat1_3
{
  friend ഘടന Coverage;

  പൊതു:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  സ്വകാര്യ:
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned പൂർണ്ണസംഖ്യ get_coverage (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    തിരികെ i;
  }

  unsigned get_population () സ്ഥിരം
  {
    തിരികെ glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    എങ്കിൽ (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      വേണ്ടി (auto g : *glyphs)
        എങ്കിൽ (get_coverage (g) != NOT_COVERED)
	  തിരികെ സത്യം;
      തിരികെ അസത്യം;
    }

    വേണ്ടി (സ്ഥിരം auto& g : glyphArray.as_array ())
      എങ്കിൽ (glyphs->has (g))
        തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }
  തർക്കം intersects_coverage (സ്ഥിരം hb_set_t *glyphs, unsigned പൂർണ്ണസംഖ്യ index) സ്ഥിരം
  { തിരികെ glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  ശൂന്യം intersect_set (സ്ഥിരം hb_set_t &glyphs, IterableOut&& intersect_glyphs) സ്ഥിരം
  {
    unsigned എണ്ണം = glyphArray.len;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () സ്ഥിരം { തിരികെ hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  തർക്കം collect_coverage (set_t *glyphs) സ്ഥിരം
  { തിരികെ glyphs->add_sorted_array (glyphArray.as_array ()); }

  പൊതു:
  /* Older compilers need this to be public. */
  ഘടന iter_t
  {
    ശൂന്യം init (സ്ഥിരം ഘടന CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    തർക്കം __more__ () സ്ഥിരം { തിരികെ i < c->glyphArray.len; }
    ശൂന്യം __next__ () { i++; }
    hb_codepoint_t get_glyph () സ്ഥിരം { തിരികെ c->glyphArray[i]; }
    തർക്കം operator != (സ്ഥിരം iter_t& o) സ്ഥിരം
    { തിരികെ i != o.i; }
    iter_t __end__ () സ്ഥിരം { iter_t it; it.init (*c); it.i = c->glyphArray.len; തിരികെ it; }

    സ്വകാര്യ:
    സ്ഥിരം ഘടന CoverageFormat1_3 *c;
    unsigned പൂർണ്ണസംഖ്യ i;
  };
  സ്വകാര്യ:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
