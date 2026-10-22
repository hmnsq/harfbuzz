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

#define NOT_COVERED             ((अचिन्हित पूर्णाङ्क) -1)

ढाँचा <typename Types>
संरचना CoverageFormat1_3
{
  मित्र संरचना Coverage;

  सार्वजनिक:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::ढाँचा SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  गुप्त:
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (glyphArray.sanitize (c));
  }

  अचिन्हित पूर्णाङ्क get_coverage (hb_codepoint_t glyph_id) अचर
  {
    अचिन्हित पूर्णाङ्क i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    फर्काऊ i;
  }

  अचिन्हित get_population () अचर
  {
    फर्काऊ glyphArray.len;
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  तार्किक intersects (अचर hb_set_t *glyphs) अचर
  {
    यदि (glyphArray.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) glyphArray.len))
    {
      लागि (स्वतः g : *glyphs)
        यदि (get_coverage (g) != NOT_COVERED)
	  फर्काऊ सत्य;
      फर्काऊ असत्य;
    }

    लागि (अचर स्वतः& g : glyphArray.as_array ())
      यदि (glyphs->has (g))
        फर्काऊ सत्य;
    फर्काऊ असत्य;
  }
  तार्किक intersects_coverage (अचर hb_set_t *glyphs, अचिन्हित पूर्णाङ्क index) अचर
  { फर्काऊ glyphs->has (glyphArray[index]); }

  ढाँचा <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  शून्य intersect_set (अचर hb_set_t &glyphs, IterableOut&& intersect_glyphs) अचर
  {
    अचिन्हित गन्ती = glyphArray.len;
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      यदि (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  अचिन्हित cost () अचर { फर्काऊ hb_bit_storage ((अचिन्हित) glyphArray.len); /* bsearch cost */ }

  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) अचर
  { फर्काऊ glyphs->add_sorted_array (glyphArray.as_array ()); }

  सार्वजनिक:
  /* Older compilers need this to be public. */
  संरचना iter_t
  {
    शून्य init (अचर संरचना CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    तार्किक __more__ () अचर { फर्काऊ i < c->glyphArray.len; }
    शून्य __next__ () { i++; }
    hb_codepoint_t get_glyph () अचर { फर्काऊ c->glyphArray[i]; }
    तार्किक चालक != (अचर iter_t& o) अचर
    { फर्काऊ i != o.i; }
    iter_t __end__ () अचर { iter_t it; it.init (*c); it.i = c->glyphArray.len; फर्काऊ it; }

    गुप्त:
    अचर संरचना CoverageFormat1_3 *c;
    अचिन्हित पूर्णाङ्क i;
  };
  गुप्त:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
