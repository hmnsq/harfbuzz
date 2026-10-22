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


#अगर_अघोषित OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
#घोषणा OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH

namespace OT {
namespace Layout {
namespace Common {

#घोषणा NOT_COVERED             ((अचिन्हित पूर्णांक) -1)

ढाँचा <typename Types>
काष्ठा CoverageFormat1_3
{
  मित्र काष्ठा Coverage;

  खुला:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::ढाँचा SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_आकार_ARRAY (2 + Types::HBUINT::static_आकार, glyphArray);

  निजी:
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (glyphArray.sanitize (c));
  }

  अचिन्हित पूर्णांक get_coverage (hb_codepoint_t glyph_id) स्थिर
  {
    अचिन्हित पूर्णांक i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    वापस i;
  }

  अचिन्हित get_population () स्थिर
  {
    वापस glyphArray.len;
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (यह);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    अगर (glyphArray.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) glyphArray.len))
    {
      क्रम (स्वतः g : *glyphs)
        अगर (get_coverage (g) != NOT_COVERED)
	  वापस true;
      वापस false;
    }

    क्रम (स्थिर स्वतः& g : glyphArray.as_array ())
      अगर (glyphs->has (g))
        वापस true;
    वापस false;
  }
  bool intersects_coverage (स्थिर hb_set_t *glyphs, अचिन्हित पूर्णांक सूचकांक_2) स्थिर
  { वापस glyphs->has (glyphArray[सूचकांक_2]); }

  ढाँचा <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  व्योम intersect_set (स्थिर hb_set_t &glyphs, IterableOut&& intersect_glyphs) स्थिर
  {
    अचिन्हित गिनती = glyphArray.len;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      अगर (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  अचिन्हित cost () स्थिर { वापस hb_bit_storage ((अचिन्हित) glyphArray.len); /* bsearch cost */ }

  ढाँचा <typename set_t>
  bool collect_coverage (set_t *glyphs) स्थिर
  { वापस glyphs->add_sorted_array (glyphArray.as_array ()); }

  खुला:
  /* Older compilers need this to be public. */
  काष्ठा iter_t
  {
    व्योम init (स्थिर काष्ठा CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    bool __more__ () स्थिर { वापस i < c->glyphArray.len; }
    व्योम __next__ () { i++; }
    hb_codepoint_t get_glyph () स्थिर { वापस c->glyphArray[i]; }
    bool चालक != (स्थिर iter_t& o) स्थिर
    { वापस i != o.i; }
    iter_t __end__ () स्थिर { iter_t it; it.init (*c); it.i = c->glyphArray.len; वापस it; }

    निजी:
    स्थिर काष्ठा CoverageFormat1_3 *c;
    अचिन्हित पूर्णांक i;
  };
  निजी:
};

}
}
}

#पूर्ण_अगर  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
