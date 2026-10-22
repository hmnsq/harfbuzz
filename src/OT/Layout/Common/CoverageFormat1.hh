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

namespace OT {
namespace Layout {
namespace Common {

#define NOT_COVERED             ((unsigned จำนวนเต็ม) -1)

template <typename Types>
sa_nracanaa CoverageFormat1_3
{
  friend sa_nracanaa Coverage;

  public:
  HBUINT16      coverageFormat; /* Format identifier--format = 1 */
  typename Types::template SortedArrayOf<typename Types::HBGlyphID>
                glyphArray;     /* Array of GlyphIDs--in numerical order */

  DEFINE_SIZE_ARRAY (2 + Types::HBUINT::static_size, glyphArray);

  private:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (glyphArray.sanitize (c));
  }

  unsigned จำนวนเต็ม get_coverage (hb_codepoint_t glyph_id) const
  {
    unsigned จำนวนเต็ม i;
    glyphArray.bfind (glyph_id, &i, HB_NOT_FOUND_STORE, NOT_COVERED);
    คืน i;
  }

  unsigned get_population () const
  {
    คืน glyphArray.len;
  }

  template <typename Iterator,
      hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c, Iterator glyphs)
  {
    TRACE_SERIALIZE (this);
    return_trace (glyphArray.serialize (c, glyphs));
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    ถ้า (glyphArray.len > glyphs->get_population () * hb_bit_storage ((unsigned) glyphArray.len))
    {
      สำหรับ (auto g : *glyphs)
        ถ้า (get_coverage (g) != NOT_COVERED)
	  คืน true;
      คืน false;
    }

    สำหรับ (const auto& g : glyphArray.as_array ())
      ถ้า (glyphs->has (g))
        คืน true;
    คืน false;
  }
  bool intersects_coverage (const hb_set_t *glyphs, unsigned จำนวนเต็ม index) const
  { คืน glyphs->has (glyphArray[index]); }

  template <typename IterableOut,
	    hb_requires (hb_is_sink_of (IterableOut, hb_codepoint_t))>
  void intersect_set (const hb_set_t &glyphs, IterableOut&& intersect_glyphs) const
  {
    unsigned count = glyphArray.len;
    สำหรับ (unsigned i = 0; i < count; i++)
      ถ้า (glyphs.has (glyphArray[i]))
        intersect_glyphs << glyphArray[i];
  }

  unsigned cost () const { คืน hb_bit_storage ((unsigned) glyphArray.len); /* bsearch cost */ }

  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  { คืน glyphs->add_sorted_array (glyphArray.as_array ()); }

  public:
  /* Older compilers need this to be public. */
  sa_nracanaa iter_t
  {
    void init (const sa_nracanaa CoverageFormat1_3 &c_) { c = &c_; i = 0; }
    bool __more__ () const { คืน i < c->glyphArray.len; }
    void __next__ () { i++; }
    hb_codepoint_t get_glyph () const { คืน c->glyphArray[i]; }
    bool operator != (const iter_t& o) const
    { คืน i != o.i; }
    iter_t __end__ () const { iter_t it; it.init (*c); it.i = c->glyphArray.len; คืน it; }

    private:
    const sa_nracanaa CoverageFormat1_3 *c;
    unsigned จำนวนเต็ม i;
  };
  private:
};

}
}
}

#endif  // #ifndef OT_LAYOUT_COMMON_COVERAGEFORMAT1_HH
