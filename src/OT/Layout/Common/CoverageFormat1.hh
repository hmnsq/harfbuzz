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

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ Common {

#define NOT_COVERED             ((unsigned પૂર્ણાંક) -1)

template <typename Types>
સંરચના CoverageFormat1_3
{
  friend સંરચના Coverage;

  જાહેર:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  ખાનગી:
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned પૂર્ણાંક get_coverage (hb_codepoint_t glyph_id) અચલ
  {
    unsigned પૂર્ણાંક i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    પાછા i;
  }

  unsigned get_population () અચલ
  {
    પાછા glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  તાર્કિક serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (આ);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    જો (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      માટે (auto g : *glyphs)
        જો (get_coverage (g) != NOT_COVERED)
	  પાછા સાચું;
      પાછા ખોટું;
    }

    માટે (અચલ auto& g : glyphArray.as_array ())
      જો (glyphs->has (g))
        પાછા સાચું;
    પાછા ખોટું;
  }
  તાર્કિક intersects_coverage (અચલ hb_set_t *glyphs, unsigned પૂર્ણાંક index) અચલ
  { પાછા glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  શૂન્ય intersect_set (અચલ hb_set_t &glyphs, IterableOut&& intersect_glyphs) અચલ
  {
    unsigned ગણતરી = glyphArray.len;
    માટે (unsigned i = 0; i < ગણતરી; i++)
      જો (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () અચલ { પાછા hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  તાર્કિક collect_coverage (set_t *glyphs) અચલ
  { પાછા glyphs->add_sorted_array (glyphArray.as_array ()); }

  જાહેર:
  /* Older compilers need this to be public. */
  સંરચના iter_t
  {
    શૂન્ય init (અચલ સંરચના CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    તાર્કિક __more__ () અચલ { પાછા i < c->glyphArray.len; }
    શૂન્ય __next__ () { i++; }
    hb_codepoint_t get_glyph () અચલ { પાછા c->glyphArray[i]; }
    તાર્કિક operator != (અચલ iter_t& o) અચલ
    { પાછા i != o.i; }
    iter_t __end__ () અચલ { iter_t it; it.init (*c); it.i = c->glyphArray.len; પાછા it; }

    ખાનગી:
    અચલ સંરચના CoverageFormat1_3 *c;
    unsigned પૂર્ણાંક i;
  };
  ખાનગી:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
