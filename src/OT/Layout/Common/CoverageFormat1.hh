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


#جے_بنا_تعریف OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
#تعریف_کرو OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH

نطاق OT {
نطاق Layout {
نطاق Common {

#تعریف_کرو NOT_COVERED             ((بنانشانی صحيح) -1)

سانچا <قسم_ناں Types>
بنية CoverageFormat1_3
{
  یار بنية Coverage;

  عام:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا Sorted_Array_دا<قسم_ناں Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_ناپ_ARRAY (2 + Types::HBUINT::static_ناپ, glyphArray);

  خاص:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (glyphArray.sanitize (c));
  }

  بنانشانی صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    بنانشانی صحيح i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_لبھیا_STORE, NOT_COVERED);
    أرجع i;
  }

  بنانشانی get_population () ثابت
  {
    أرجع glyphArray.len;
  }

  سانچا <قسم_ناں Iterator,
      hb_requires (hb_is_sorted_سروت_دا (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    إذا (glyphArray.len > glyphs->get_population () * hb_ثنائی_ہندسہ_storage ((بنانشانی) glyphArray.len))
    {
      لكل (خودکار g : *glyphs)
        إذا (get_coverage (g) != NOT_COVERED)
	  أرجع سچ;
      أرجع خطأ;
    }

    لكل (ثابت خودکار& g : glyphArray.as_array ())
      إذا (glyphs->has (g))
        أرجع سچ;
    أرجع خطأ;
  }
  منطقي intersects_coverage (ثابت hb_سیٹ_t *glyphs, بنانشانی صحيح اشاریہ_2) ثابت
  { أرجع glyphs->has (glyphArray[اشاریہ_2]); }

  سانچا <قسم_ناں Iterable_باہرلا,
	    hb_requires (hb_is_sink_دا (Iterable_باہرلا, hb_codepoint_t))>
  فراغ intersect_سیٹ (ثابت hb_سیٹ_t &glyphs, Iterable_باہرلا&& intersect_glyphs) ثابت
  {
    بنانشانی گنتی = glyphArray.len;
    لكل (بنانشانی i = 0; i < گنتی; i++)
      إذا (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  بنانشانی لاگت () ثابت { أرجع hb_ثنائی_ہندسہ_storage ((بنانشانی) glyphArray.len); /* bsearch cost */ }

  سانچا <قسم_ناں سیٹ_t>
  منطقي collect_coverage (سیٹ_t *glyphs) ثابت
  { أرجع glyphs->جوڑو_sorted_array (glyphArray.as_array ()); }

  عام:
  /* Older compilers need this to be public. */
  بنية iter_t
  {
    فراغ init (ثابت بنية CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    منطقي __more__ () ثابت { أرجع i < c->glyphArray.len; }
    فراغ __next__ () { i++; }
    hb_codepoint_t get_glyph () ثابت { أرجع c->glyphArray[i]; }
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع i != o.i; }
    iter_t __end__ () ثابت { iter_t it; it.init (*c); it.i = c->glyphArray.len; أرجع it; }

    خاص:
    ثابت بنية CoverageFormat1_3 *c;
    بنانشانی صحيح i;
  };
  خاص:
};

}
}
}

#جے_انت  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
