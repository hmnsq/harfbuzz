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


#اگر_غیر_تعریف_شدہ OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
#تعریف_کرو OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH

نامخلا OT {
نامخلا Layout {
نامخلا Common {

#تعریف_کرو NOT_COVERED             ((غیرعلامتی صحیح) -1)

سانچہ <قسم_نام Types>
ساخت CoverageFormat1_3
{
  دوست ساخت Coverage;

  عوامی:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ Sorted_Array_کا<قسم_نام Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_سائز_ARRAY (2 + Types::HBUINT::static_سائز, glyphArray);

  نجی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (glyphArray.sanitize (c));
  }

  غیرعلامتی صحیح get_coverage (hb_codepoint_t glyph_id) ثابت
  {
    غیرعلامتی صحیح i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_ملا_STORE, NOT_COVERED);
    واپس i;
  }

  غیرعلامتی get_population () ثابت
  {
    واپس glyphArray.len;
  }

  سانچہ <قسم_نام Iterator,
      hb_requires (hb_is_sorted_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (یہ);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    اگر (glyphArray.len > glyphs->get_population () * hb_ثنائی_ہندسہ_storage ((غیرعلامتی) glyphArray.len))
    {
      برائے (خودکار g : *glyphs)
        اگر (get_coverage (g) != NOT_COVERED)
	  واپس سچ;
      واپس جھوٹ;
    }

    برائے (ثابت خودکار& g : glyphArray.as_array ())
      اگر (glyphs->has (g))
        واپس سچ;
    واپس جھوٹ;
  }
  منطقی intersects_coverage (ثابت hb_سیٹ_t *glyphs, غیرعلامتی صحیح اشاریہ_2) ثابت
  { واپس glyphs->has (glyphArray[اشاریہ_2]); }

  سانچہ <قسم_نام Iterable_برآمد,
	    hb_requires (hb_is_sink_کا (Iterable_برآمد, hb_codepoint_t))>
  خالی intersect_سیٹ (ثابت hb_سیٹ_t &glyphs, Iterable_برآمد&& intersect_glyphs) ثابت
  {
    غیرعلامتی گنتی = glyphArray.len;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      اگر (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  غیرعلامتی لاگت () ثابت { واپس hb_ثنائی_ہندسہ_storage ((غیرعلامتی) glyphArray.len); /* bsearch cost */ }

  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_coverage (سیٹ_t *glyphs) ثابت
  { واپس glyphs->جوڑو_sorted_array (glyphArray.as_array ()); }

  عوامی:
  /* Older compilers need this to be public. */
  ساخت iter_t
  {
    خالی init (ثابت ساخت CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    منطقی __more__ () ثابت { واپس i < c->glyphArray.len; }
    خالی __next__ () { i++; }
    hb_codepoint_t get_glyph () ثابت { واپس c->glyphArray[i]; }
    منطقی عامل != (ثابت iter_t& o) ثابت
    { واپس i != o.i; }
    iter_t __end__ () ثابت { iter_t it; it.init (*c); it.i = c->glyphArray.len; واپس it; }

    نجی:
    ثابت ساخت CoverageFormat1_3 *c;
    غیرعلامتی صحیح i;
  };
  نجی:
};

}
}
}

#اگر_ختم  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
