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

#define NOT_COVERED             ((अचिन्हित पूर्णांक) -1)

ढाँचा <typename Types>
संरचना CoverageFormat1_3
{
  मित्र संरचना Coverage;

  सार्वजनिक:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::ढाँचा SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  खाजगी:
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (glyphArray.sanitize (c));
  }

  अचिन्हित पूर्णांक get_coverage (hb_codepoint_t glyph_id) स्थिरांक
  {
    अचिन्हित पूर्णांक i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    परत i;
  }

  अचिन्हित get_population () स्थिरांक
  {
    परत glyphArray.len;
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (हे);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    जर (glyphArray.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) glyphArray.len))
    {
      साठी (स्वतः g : *glyphs)
        जर (get_coverage (g) != NOT_COVERED)
	  परत सत्य;
      परत असत्य;
    }

    साठी (स्थिरांक स्वतः& g : glyphArray.as_array ())
      जर (glyphs->has (g))
        परत सत्य;
    परत असत्य;
  }
  तार्किक intersects_coverage (स्थिरांक hb_set_t *glyphs, अचिन्हित पूर्णांक index) स्थिरांक
  { परत glyphs->has (glyphArray[index]); }

  ढाँचा <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  शून्य intersect_set (स्थिरांक hb_set_t &glyphs, IterableOut&& intersect_glyphs) स्थिरांक
  {
    अचिन्हित मोजणी = glyphArray.len;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      जर (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  अचिन्हित cost () स्थिरांक { परत hb_bit_storage ((अचिन्हित) glyphArray.len); /* bsearch cost */ }

  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) स्थिरांक
  { परत glyphs->add_sorted_array (glyphArray.as_array ()); }

  सार्वजनिक:
  /* Older compilers need this to be public. */
  संरचना iter_t
  {
    शून्य init (स्थिरांक संरचना CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    तार्किक __more__ () स्थिरांक { परत i < c->glyphArray.len; }
    शून्य __next__ () { i++; }
    hb_codepoint_t get_glyph () स्थिरांक { परत c->glyphArray[i]; }
    तार्किक चालक != (स्थिरांक iter_t& o) स्थिरांक
    { परत i != o.i; }
    iter_t __end__ () स्थिरांक { iter_t it; it.init (*c); it.i = c->glyphArray.len; परत it; }

    खाजगी:
    स्थिरांक संरचना CoverageFormat1_3 *c;
    अचिन्हित पूर्णांक i;
  };
  खाजगी:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
