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


#اگر_تعریف_نشده OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
#تعریف_کن OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH

نطاق OT {
نطاق Layout {
نطاق Common {

#تعریف_کن NOT_COVERED             ((بیعلامت صحيح) -1)

قالب <نام_نوع Types>
بنية CoverageFormat1_3
{
  دوست بنية Coverage;

  عام:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  نام_نوع Types::قالب Sorted_Array_از<نام_نوع Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_اندازه_ARRAY (2 + Types::HBUINT::static_اندازه, glyphArray);

  خاص:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (glyphArray.sanitize (c));
  }

  بیعلامت صحيح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    بیعلامت صحيح i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_یافت_STORE, NOT_COVERED);
    أرجع i;
  }

  بیعلامت get_population () ثابت
  {
    أرجع glyphArray.len;
  }

  قالب <نام_نوع Iterator,
      hb_requires (hb_is_sorted_منبع_از (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    إذا (glyphArray.len > glyphs->get_population () * hb_بیت_storage ((بیعلامت) glyphArray.len))
    {
      لكل (خودکار g : *glyphs)
        إذا (get_coverage (g) != NOT_COVERED)
	  أرجع درست;
      أرجع خطأ;
    }

    لكل (ثابت خودکار& g : glyphArray.as_array ())
      إذا (glyphs->has (g))
        أرجع درست;
    أرجع خطأ;
  }
  منطقي intersects_coverage (ثابت hb_مجموعه_t *glyphs, بیعلامت صحيح شاخص_2) ثابت
  { أرجع glyphs->has (glyphArray[شاخص_2]); }

  قالب <نام_نوع Iterable_بیرون,
	    hb_requires (hb_is_sink_از (Iterable_بیرون, hb_codepoint_t))>
  فراغ intersect_مجموعه (ثابت hb_مجموعه_t &glyphs, Iterable_بیرون&& intersect_glyphs) ثابت
  {
    بیعلامت شمار = glyphArray.len;
    لكل (بیعلامت i = 0; i < شمار; i++)
      إذا (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  بیعلامت هزینه () ثابت { أرجع hb_بیت_storage ((بیعلامت) glyphArray.len); /* bsearch cost */ }

  قالب <نام_نوع مجموعه_t>
  منطقي collect_coverage (مجموعه_t *glyphs) ثابت
  { أرجع glyphs->جمع_کن_sorted_array (glyphArray.as_array ()); }

  عام:
  /* Older compilers need this to be public. */
  بنية iter_t
  {
    فراغ init (ثابت بنية CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    منطقي __more__ () ثابت { أرجع i < c->glyphArray.len; }
    فراغ __next__ () { i++; }
    hb_codepoint_t get_glyph () ثابت { أرجع c->glyphArray[i]; }
    منطقي عملگر != (ثابت iter_t& o) ثابت
    { أرجع i != o.i; }
    iter_t __end__ () ثابت { iter_t it; it.init (*c); it.i = c->glyphArray.len; أرجع it; }

    خاص:
    ثابت بنية CoverageFormat1_3 *c;
    بیعلامت صحيح i;
  };
  خاص:
};

}
}
}

#پایان_اگر  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
