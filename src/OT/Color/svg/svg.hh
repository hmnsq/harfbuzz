/*
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_COLOR_SVG_SVG_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_COLOR_SVG_SVG_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-blob.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-limits.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-paint.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-zlib.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ <ctype.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_SVG HB_ਟੈਗ('S','V','G',' ')


ਨਾਮਾਕਾਸ਼ OT {

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ਅਚਲ ਅੱਖਰ **svg,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_len = 0;
  ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &ਅੰਕੜੇ_len);

  ਜੇ (!ਅੰਕੜੇ || !ਅੰਕੜੇ_len)
    ਜਾਓ fail;

  ਜੇ (hb_blob_is_gzip (ਅੰਕੜੇ, ਅੰਕੜੇ_len))
  {
    uint32_t expected_ਮਾਪ = 0;
    ਜੇ (hb_gzip_get_uncompressed_ਮਾਪ (ਅੰਕੜੇ, ਅੰਕੜੇ_len, &expected_ਮਾਪ) &&
        unlikely ((size_t) expected_ਮਾਪ > (size_t) HB_SVG_MAX_DOCUMENT_ਮਾਪ))
      ਜਾਓ fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_ਮਾਪ);
    ਜੇ (!uncompressed)
      ਜਾਓ fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &ਅੰਕੜੇ_len);
    ਜੇ (!ਅੰਕੜੇ || !ਅੰਕੜੇ_len)
      ਜਾਓ fail;
  }

  ਜੇ (unlikely ((size_t) ਅੰਕੜੇ_len > (size_t) HB_SVG_MAX_DOCUMENT_ਮਾਪ))
    ਜਾਓ fail;

  ਜੇ (svg) *svg = ਅੰਕੜੇ;
  ਜੇ (len) *len = ਅੰਕੜੇ_len;
  ਵਾਪਸ blob;

fail:
  hb_blob_destroy (blob);
  ਜੇ (svg) *svg = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (len) *len = 0;
  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}

ਬਣਤਰ SVGDocument_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ
{
  ਪੂਰਨ cmp (hb_codepoint_t g) ਅਚਲ
  { ਵਾਪਸ g < ਸ਼ੁਰੂ_Glyph_ID ? -1 : g > ਸਿਰਾ_Glyph_ID ? 1 : 0; }

  hb_codepoint_t get_ਸ਼ੁਰੂ_glyph () ਅਚਲ
  { ਵਾਪਸ ਸ਼ੁਰੂ_Glyph_ID; }

  hb_codepoint_t get_ਸਿਰਾ_glyph () ਅਚਲ
  { ਵਾਪਸ ਸਿਰਾ_Glyph_ID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_ਵਿੱਥ) ਅਚਲ
  {
    ਵਾਪਸ hb_blob_create_sub_blob (svg_blob,
				    ਸੂਚਕਾਂਕ_ਵਿੱਥ + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) svgDoc,
				    svg_Doc_ਲੰਮਾਈ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, ਅਧਾਰ, svg_Doc_ਲੰਮਾਈ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸ਼ੁਰੂ_Glyph_ID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	ਸਿਰਾ_Glyph_ID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svg_Doc_ਲੰਮਾਈ;	/* Length of the SVG document.
				 * Must be non-zero. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

ਬਣਤਰ SVG
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_SVG;

  ਬਣਤਰ svg_id_ਵਿਸਤਾਰ_t
  {
    ਅਚਲ ਅੱਖਰ *p;
    ਬਿਨਾਨਿਸ਼ਾਨੀ len;

    ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ svg_id_ਵਿਸਤਾਰ_t &o) ਅਚਲ
    {
      ਵਾਪਸ len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ਅਚਲ
    {
      uint32_t h = hb_hash (len);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len; i++)
        h = h * 33u + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) p[i];
      ਵਾਪਸ h;
    }
  };

  ਬਣਤਰ svg_defs_ਇੰਦਰਾਜ_t
  {
    svg_id_ਵਿਸਤਾਰ_t id;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ;
  };

  ਬਣਤਰ svg_doc_cache_t
  {
    hb_blob_t *blob = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ ਅੱਖਰ *svg = ਖਾਲੀ_ਸੂਚਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ len = 0;
    hb_ਸਦਿਸ਼_t<svg_defs_ਇੰਦਰਾਜ_t> defs_entries;
    hb_codepoint_t ਸ਼ੁਰੂ_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t ਸਿਰਾ_glyph = HB_CODEPOINT_INVALID;
    hb_ਸਦਿਸ਼_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_ਵਿਸਤਾਰ_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ svgDocEntries; }

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ਅਚਲ
    {
      ਵਾਪਸ ਸਾਰਣੀ->get_glyph_ਇੰਦਰਾਜ (glyph_id).reference_blob (ਸਾਰਣੀ.get_blob (),
							       ਸਾਰਣੀ->svgDocEntries);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ get_document_ਗਿਣਤੀ () ਅਚਲ
    { ਵਾਪਸ ਸਾਰਣੀ->get_document_ਗਿਣਤੀ (); }

    ਤਾਰਕਿਕ get_glyph_document_ਸੂਚਕਾਂਕ (hb_codepoint_t glyph_id, ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸੂਚਕਾਂਕ_2) ਅਚਲ
    { ਵਾਪਸ ਸਾਰਣੀ->get_glyph_document_ਸੂਚਕਾਂਕ (glyph_id, ਸੂਚਕਾਂਕ_2); }

    ਤਾਰਕਿਕ get_document_glyph_range (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2,
                                   hb_codepoint_t *ਸ਼ੁਰੂ_glyph,
                                   hb_codepoint_t *ਸਿਰਾ_glyph) ਅਚਲ
    { ਵਾਪਸ ਸਾਰਣੀ->get_document_glyph_range (ਸੂਚਕਾਂਕ_2, ਸ਼ੁਰੂ_glyph, ਸਿਰਾ_glyph); }

    ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ->has_ਅੰਕੜੇ (); }

    ਅਚਲ svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ਅਚਲ ਅੱਖਰ *svg,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ len,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ doc_ਸੂਚਕਾਂਕ,
                             hb_codepoint_t ਸ਼ੁਰੂ_glyph,
                             hb_codepoint_t ਸਿਰਾ_glyph) ਅਚਲ;

    ਅਚਲ ਅੱਖਰ *
    doc_cache_get_svg (ਅਚਲ svg_doc_cache_t *doc,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ *len) ਅਚਲ;

    ਅਚਲ hb_ਸਦਿਸ਼_t<svg_defs_ਇੰਦਰਾਜ_t> *
    doc_cache_get_defs_entries (ਅਚਲ svg_doc_cache_t *doc) ਅਚਲ;

    ਤਾਰਕਿਕ
    doc_cache_get_glyph_ਵਿਸਤਾਰ (ਅਚਲ svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸ਼ੁਰੂ,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸਿਰਾ) ਅਚਲ;

    ਤਾਰਕਿਕ
    doc_cache_ਲੱਭੋ_id_ਵਿਸਤਾਰ (ਅਚਲ svg_doc_cache_t *doc,
                            svg_id_ਵਿਸਤਾਰ_t id,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸ਼ੁਰੂ,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸਿਰਾ) ਅਚਲ;

    ਤਾਰਕਿਕ
    doc_cache_ਲੱਭੋ_id_cstr (ਅਚਲ svg_doc_cache_t *doc,
                            ਅਚਲ ਅੱਖਰ *id,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸ਼ੁਰੂ,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸਿਰਾ) ਅਚਲ;

    ਤਾਰਕਿਕ paint_glyph (hb_font_t *font HB_ਅਣਵਰਤਿਆ, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ਖਾਲੀ *ਅੰਕੜੇ) ਅਚਲ
    {
      ਜੇ (!has_ਅੰਕੜੇ ())
        ਵਾਪਸ ਝੂਠ;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      ਜੇ (blob == hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ())
        ਵਾਪਸ ਝੂਠ;

      ਤਾਰਕਿਕ ret = funcs->image (ਅੰਕੜੇ,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       ਖਾਲੀ_ਸੂਚਕ);

      hb_blob_destroy (blob);

      ਵਾਪਸ ret;
    }

    ਨਿਜੀ:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ਅਚਲ ਅੱਖਰ *svg,
                    ਬਿਨਾਨਿਸ਼ਾਨੀ len,
                    hb_codepoint_t ਸ਼ੁਰੂ_glyph,
                    hb_codepoint_t ਸਿਰਾ_glyph) ਅਚਲ;

    ਸਥਿਰ ਖਾਲੀ destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> ਸਾਰਣੀ;
    ਬਦਲਣਯੋਗ hb_ਸਦਿਸ਼_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    ਜਨਤਕ:
    DEFINE_ਮਾਪ_STATIC (ਆਕਾਰ (hb_blob_ptr_t<SVG>) +
                        ਆਕਾਰ (hb_ਸਦਿਸ਼_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ਅਚਲ SVGDocument_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ &get_glyph_ਇੰਦਰਾਜ (hb_codepoint_t glyph_id) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+svgDocEntries).bsearch (glyph_id); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_document_ਗਿਣਤੀ () ਅਚਲ
  {
    ਜੇ (!has_ਅੰਕੜੇ ())
      ਵਾਪਸ 0;
    ਵਾਪਸ (ਇਹ + svgDocEntries).len;
  }

  ਤਾਰਕਿਕ get_glyph_document_ਸੂਚਕਾਂਕ (hb_codepoint_t glyph_id, ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
    ਜੇ (!has_ਅੰਕੜੇ ())
      ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ (ਇਹ + svgDocEntries).bfind (glyph_id, ਸੂਚਕਾਂਕ_2);
  }

  ਤਾਰਕਿਕ get_document_glyph_range (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2,
                                 hb_codepoint_t *ਸ਼ੁਰੂ_glyph,
                                 hb_codepoint_t *ਸਿਰਾ_glyph) ਅਚਲ
  {
    ਜੇ (!has_ਅੰਕੜੇ ())
      ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &entries = ਇਹ + svgDocEntries;
    ਜੇ (ਸੂਚਕਾਂਕ_2 >= entries.len)
      ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਇੰਦਰਾਜ = entries.arrayZ[ਸੂਚਕਾਂਕ_2];
    ਜੇ (ਸ਼ੁਰੂ_glyph) *ਸ਼ੁਰੂ_glyph = ਇੰਦਰਾਜ.get_ਸ਼ੁਰੂ_glyph ();
    ਜੇ (ਸਿਰਾ_glyph) *ਸਿਰਾ_glyph = ਇੰਦਰਾਜ.get_ਸਿਰਾ_glyph ();
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  (ਇਹ+svgDocEntries).sanitize_shallow (c)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* Table version (starting at 0). */
  Offset32_ਮੰਜ਼ਿਲ<Sorted_Array16_ਦਾ<SVGDocument_ਸੂਚਕਾਂਕ_ਇੰਦਰਾਜ>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);
};

ਨਾਮਾਕਾਸ਼ _hb_svg_cache_impl {

ਬਣਤਰ glyph_ਇੰਦਰਾਜ_t
{
  hb_codepoint_t glyph;
  uint32_t ਸ਼ੁਰੂ;
  uint32_t ਸਿਰਾ;
};

ਬਣਤਰ id_ਇੰਦਰਾਜ_t
{
  SVG::svg_id_ਵਿਸਤਾਰ_t id;
  uint32_t ਸ਼ੁਰੂ;
  uint32_t ਸਿਰਾ;
};

ਬਣਤਰ open_elem_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ;
  SVG::svg_id_ਵਿਸਤਾਰ_t id;
  ਤਾਰਕਿਕ ਵਿੱਚ_defs_content;
  ਤਾਰਕਿਕ is_defs;
};

ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ MAX_ਡੂੰਘਾਈ = 128;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
ਲੱਭੋ_substr (ਅਚਲ ਅੱਖਰ *s,
             ਬਿਨਾਨਿਸ਼ਾਨੀ n,
             ਬਿਨਾਨਿਸ਼ਾਨੀ from,
             ਅਚਲ ਅੱਖਰ *needle,
             ਬਿਨਾਨਿਸ਼ਾਨੀ needle_len)
{
  ਜੇ (!needle_len || from >= n || needle_len > n)
    ਵਾਪਸ -1;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = from; i + needle_len <= n; i++)
    ਜੇ (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      ਵਾਪਸ (ਪੂਰਨ) i;
  ਵਾਪਸ -1;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
parse_id_ਵਿੱਚ_ਸ਼ੁਰੂ_ਟੈਗ (ਅਚਲ ਅੱਖਰ *svg,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਟੈਗ_ਸ਼ੁਰੂ,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਟੈਗ_ਸਿਰਾ,
                       SVG::svg_id_ਵਿਸਤਾਰ_t *id)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ p = ਟੈਗ_ਸ਼ੁਰੂ;
  ਜਦੋਂ (p + 4 <= ਟੈਗ_ਸਿਰਾ)
  {
    ਜੇ (!memcmp (svg + p, "id=\"", 4))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ b = p + 4;
      ਬਿਨਾਨਿਸ਼ਾਨੀ e = b;
      ਜਦੋਂ (e < ਟੈਗ_ਸਿਰਾ && svg[e] != '"') e++;
      ਜੇ (e <= ਟੈਗ_ਸਿਰਾ && e > b)
      {
        *id = {svg + b, e - b};
        ਵਾਪਸ ਸੱਚ;
      }
    }
    ਜੇ (!memcmp (svg + p, "id='", 4))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ b = p + 4;
      ਬਿਨਾਨਿਸ਼ਾਨੀ e = b;
      ਜਦੋਂ (e < ਟੈਗ_ਸਿਰਾ && svg[e] != '\'') e++;
      ਜੇ (e <= ਟੈਗ_ਸਿਰਾ && e > b)
      {
        *id = {svg + b, e - b};
        ਵਾਪਸ ਸੱਚ;
      }
    }
    p++;
  }
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
parse_glyph_id_ਵਿਸਤਾਰ (ਅਚਲ SVG::svg_id_ਵਿਸਤਾਰ_t &id,
                     hb_codepoint_t *glyph)
{
  ਜੇ (id.len <= 5 || memcmp (id.p, "glyph", 5))
    ਵਾਪਸ ਝੂਠ;

  hb_codepoint_t gid = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 5; i < id.len; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) id.p[i];
    ਜੇ (c < '0' || c > '9')
      ਵਾਪਸ ਝੂਠ;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    ਜੇ (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      ਵਾਪਸ ਝੂਠ;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
parse_cache_entries_ਰੇਖਿਕ (ਅਚਲ ਅੱਖਰ *svg,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ len,
                            hb_ਸਦਿਸ਼_t<SVG::svg_defs_ਇੰਦਰਾਜ_t> *defs_entries,
                            hb_ਸਦਿਸ਼_t<glyph_ਇੰਦਰਾਜ_t> *glyph_spans,
                            hb_ਸਦਿਸ਼_t<id_ਇੰਦਰਾਜ_t> *id_entries)
{
  open_elem_t ਢੇਰ[MAX_ਡੂੰਘਾਈ] = {};
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ = 0;
  ਜੇ (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ defs_ਡੂੰਘਾਈ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
  ਜਦੋਂ (i < len)
  {
    ਜੇ (svg[i] != '<')
    {
      i++;
      ਜਾਰੀ;
    }

    ਜੇ (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      ਪੂਰਨ cend = ਲੱਭੋ_substr (svg, len, i + 4, "-->", 3);
      ਜੇ (cend < 0) ਵਾਪਸ ਝੂਠ;
      i = (ਬਿਨਾਨਿਸ਼ਾਨੀ) cend + 3;
      ਜਾਰੀ;
    }
    ਜੇ (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      ਪੂਰਨ cend = ਲੱਭੋ_substr (svg, len, i + 9, "]]>", 3);
      ਜੇ (cend < 0) ਵਾਪਸ ਝੂਠ;
      i = (ਬਿਨਾਨਿਸ਼ਾਨੀ) cend + 3;
      ਜਾਰੀ;
    }

    ਤਾਰਕਿਕ closing = (i + 1 < len && svg[i + 1] == '/');
    ਤਾਰਕਿਕ special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    ਬਿਨਾਨਿਸ਼ਾਨੀ gt = i + 1;
    ਅੱਖਰ quote = 0;
    ਜਦੋਂ (gt < len)
    {
      ਅੱਖਰ c = svg[gt];
      ਜੇ (quote)
      {
        ਜੇ (c == quote) quote = 0;
      }
      ਨਹੀਂਤਾਂ
      {
        ਜੇ (c == '"' || c == '\'')
          quote = c;
        ਨਹੀਂਤਾਂ ਜੇ (c == '>')
          ਰੋਕੋ;
      }
      gt++;
    }
    ਜੇ (gt >= len)
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (special)
    {
      i = gt + 1;
      ਜਾਰੀ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ p = i + (closing ? 2 : 1);
    ਜਦੋਂ (p < gt && ਖਾਲੀ_ਥਾਂ_ਹੈ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) svg[p])) p++;
    ਅਚਲ ਅੱਖਰ *name = svg + p;
    ਬਿਨਾਨਿਸ਼ਾਨੀ name_len = 0;
    ਜਦੋਂ (p + name_len < gt)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) name[name_len];
      ਜੇ (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        ਰੋਕੋ;
      name_len++;
    }
    ਤਾਰਕਿਕ is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    ਜੇ (closing)
    {
      ਜੇ (!ਡੂੰਘਾਈ)
      {
        i = gt + 1;
        ਜਾਰੀ;
      }

      open_elem_t e = ਢੇਰ[--ਡੂੰਘਾਈ];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = gt + 1;

      ਜੇ (e.id.len)
      {
        ਜੇ (unlikely (!id_entries->ਧੱਕੋ_or_fail (id_ਇੰਦਰਾਜ_t {e.id, (uint32_t) e.ਸ਼ੁਰੂ, (uint32_t) ਸਿਰਾ})))
          ਵਾਪਸ ਝੂਠ;

        ਜੇ (e.ਵਿੱਚ_defs_content)
        {
          ਜੇ (unlikely (!defs_entries->ਧੱਕੋ_or_fail ()))
            ਵਾਪਸ ਝੂਠ;
          ਸਵੈਚਾਲਿਤ &ਥਾਂ = defs_entries->ਪੂਛ ();
          ਥਾਂ.id = e.id;
          ਥਾਂ.ਸ਼ੁਰੂ = e.ਸ਼ੁਰੂ;
          ਥਾਂ.ਸਿਰਾ = ਸਿਰਾ;
        }

        hb_codepoint_t gid;
        ਜੇ (parse_glyph_id_ਵਿਸਤਾਰ (e.id, &gid))
        {
          ਜੇ (unlikely (!glyph_spans->ਧੱਕੋ_or_fail (glyph_ਇੰਦਰਾਜ_t {gid, (uint32_t) e.ਸ਼ੁਰੂ, (uint32_t) ਸਿਰਾ})))
            ਵਾਪਸ ਝੂਠ;
        }
      }

      ਜੇ (e.is_defs && defs_ਡੂੰਘਾਈ)
        defs_ਡੂੰਘਾਈ--;

      i = ਸਿਰਾ;
      ਜਾਰੀ;
    }

    SVG::svg_id_ਵਿਸਤਾਰ_t id = {};
    parse_id_ਵਿੱਚ_ਸ਼ੁਰੂ_ਟੈਗ (svg, i, gt, &id);

    ਬਿਨਾਨਿਸ਼ਾਨੀ r = gt;
    ਜਦੋਂ (r > i && ਖਾਲੀ_ਥਾਂ_ਹੈ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) svg[r - 1])) r--;
    ਤਾਰਕਿਕ self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.ਸ਼ੁਰੂ = i;
    e.id = id;
    e.ਵਿੱਚ_defs_content = defs_ਡੂੰਘਾਈ > 0;
    e.is_defs = is_defs;

    ਜੇ (self_closing)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = gt + 1;
      ਜੇ (e.id.len)
      {
        ਜੇ (unlikely (!id_entries->ਧੱਕੋ_or_fail (id_ਇੰਦਰਾਜ_t {e.id, (uint32_t) e.ਸ਼ੁਰੂ, (uint32_t) ਸਿਰਾ})))
          ਵਾਪਸ ਝੂਠ;

        ਜੇ (e.ਵਿੱਚ_defs_content)
        {
          ਜੇ (unlikely (!defs_entries->ਧੱਕੋ_or_fail ()))
            ਵਾਪਸ ਝੂਠ;
          ਸਵੈਚਾਲਿਤ &ਥਾਂ = defs_entries->ਪੂਛ ();
          ਥਾਂ.id = e.id;
          ਥਾਂ.ਸ਼ੁਰੂ = e.ਸ਼ੁਰੂ;
          ਥਾਂ.ਸਿਰਾ = ਸਿਰਾ;
        }

        hb_codepoint_t gid;
        ਜੇ (parse_glyph_id_ਵਿਸਤਾਰ (e.id, &gid))
        {
          ਜੇ (unlikely (!glyph_spans->ਧੱਕੋ_or_fail (glyph_ਇੰਦਰਾਜ_t {gid, (uint32_t) e.ਸ਼ੁਰੂ, (uint32_t) ਸਿਰਾ})))
            ਵਾਪਸ ਝੂਠ;
        }
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (unlikely (ਡੂੰਘਾਈ >= MAX_ਡੂੰਘਾਈ))
        ਵਾਪਸ ਝੂਠ;
      ਢੇਰ[ਡੂੰਘਾਈ++] = e;
      ਜੇ (is_defs)
        defs_ਡੂੰਘਾਈ++;
    }

    i = gt + 1;
  }

  ਵਾਪਸ ਸੱਚ;
}

} /* namespace _hb_svg_cache_impl */

ਸਤਰਅੰਦਰ
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<SVG> (face);
  doc_caches.init ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ doc_ਗਿਣਤੀ = ਸਾਰਣੀ->get_document_ਗਿਣਤੀ ();
  ਜੇ (doc_ਗਿਣਤੀ && unlikely (!doc_caches.resize (doc_ਗਿਣਤੀ)))
    doc_caches.clear ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < doc_caches.ਲੰਮਾਈ; i++)
    doc_caches.arrayZ[i].ਸੈੱਟ_relaxed (ਖਾਲੀ_ਸੂਚਕ);
}

ਸਤਰਅੰਦਰ
SVG::accelerator_t::~accelerator_t ()
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < doc_caches.ਲੰਮਾਈ; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  ਸਾਰਣੀ.destroy ();
}

ਸਤਰਅੰਦਰ ਖਾਲੀ
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  ਜੇ (!doc)
    ਵਾਪਸ;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

ਸਤਰਅੰਦਰ SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ਅਚਲ ਅੱਖਰ *svg,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ len,
                                    hb_codepoint_t ਸ਼ੁਰੂ_glyph,
                                    hb_codepoint_t ਸਿਰਾ_glyph) ਅਚਲ
{
  ਸਥਿਰ ਅਚਲ uint32_t INVALID_ਵਿਸਤਾਰ = 0xFFFFFFFFu;

  ਸਵੈਚਾਲਿਤ *doc = (svg_doc_cache_t *) hb_malloc (ਆਕਾਰ (svg_doc_cache_t));
  ਜੇ (!doc)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  doc->blob = ਖਾਲੀ_ਸੂਚਕ;
  doc->svg = ਖਾਲੀ_ਸੂਚਕ;
  doc->len = 0;
  doc->defs_entries.init ();
  doc->ਸ਼ੁਰੂ_glyph = HB_CODEPOINT_INVALID;
  doc->ਸਿਰਾ_glyph = HB_CODEPOINT_INVALID;
  doc->glyph_spans.init ();
  doc->id_spans.init ();

  doc->blob = hb_blob_reference (image);
  doc->svg = svg;
  doc->len = len;
  doc->ਸ਼ੁਰੂ_glyph = ਸ਼ੁਰੂ_glyph;
  doc->ਸਿਰਾ_glyph = ਸਿਰਾ_glyph;

  ਜੇ (unlikely (ਸ਼ੁਰੂ_glyph == HB_CODEPOINT_INVALID || ਸਿਰਾ_glyph < ਸ਼ੁਰੂ_glyph))
  {
    destroy_doc_cache (doc);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ = ਸਿਰਾ_glyph - ਸ਼ੁਰੂ_glyph + 1;
  ਜੇ (!doc->glyph_spans.resize ((ਪੂਰਨ) glyph_ਗਿਣਤੀ))
  {
    destroy_doc_cache (doc);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < glyph_ਗਿਣਤੀ; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_ਵਿਸਤਾਰ, INVALID_ਵਿਸਤਾਰ);

  hb_ਸਦਿਸ਼_t<_hb_svg_cache_impl::glyph_ਇੰਦਰਾਜ_t> glyph_spans;
  glyph_spans.init ();
  hb_ਸਦਿਸ਼_t<_hb_svg_cache_impl::id_ਇੰਦਰਾਜ_t> id_entries;
  id_entries.init ();
  ਜੇ (!_hb_svg_cache_impl::parse_cache_entries_ਰੇਖਿਕ (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < glyph_spans.ਲੰਮਾਈ; i++)
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿਸਤਾਰ = glyph_spans.arrayZ[i];
    ਜੇ (unlikely (ਵਿਸਤਾਰ.glyph < ਸ਼ੁਰੂ_glyph || ਵਿਸਤਾਰ.glyph > ਸਿਰਾ_glyph))
      ਜਾਰੀ;
    doc->glyph_spans.arrayZ[ਵਿਸਤਾਰ.glyph - ਸ਼ੁਰੂ_glyph] = hb_pair_t<uint32_t, uint32_t> (ਵਿਸਤਾਰ.ਸ਼ੁਰੂ, ਵਿਸਤਾਰ.ਸਿਰਾ);
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < id_entries.ਲੰਮਾਈ; i++)
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *ਬਾਹਰਲਾ = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (doc->id_spans.has (e.id, &ਬਾਹਰਲਾ))
      ਜਾਰੀ;
    ਜੇ (unlikely (!doc->id_spans.ਸੈੱਟ (e.id, hb_pair_t<uint32_t, uint32_t> (e.ਸ਼ੁਰੂ, e.ਸਿਰਾ))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  ਵਾਪਸ doc;
}

ਸਤਰਅੰਦਰ ਅਚਲ SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ਅਚਲ ਅੱਖਰ *svg,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ len,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ doc_ਸੂਚਕਾਂਕ,
                                              hb_codepoint_t ਸ਼ੁਰੂ_glyph,
                                              hb_codepoint_t ਸਿਰਾ_glyph) ਅਚਲ
{
  ਜੇ (doc_ਸੂਚਕਾਂਕ >= doc_caches.ਲੰਮਾਈ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਸਵੈਚਾਲਿਤ &ਥਾਂ = doc_caches.arrayZ[doc_ਸੂਚਕਾਂਕ];
  ਸਵੈਚਾਲਿਤ *doc = ਥਾਂ.get_acquire ();
  ਜੇ (doc)
    ਵਾਪਸ doc;

  ਸਵੈਚਾਲਿਤ *fresh = make_doc_cache (image, svg, len, ਸ਼ੁਰੂ_glyph, ਸਿਰਾ_glyph);
  ਜੇ (!fresh)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਸਵੈਚਾਲਿਤ *expected = (svg_doc_cache_t *) ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (ਥਾਂ.cmpexch (expected, fresh))
    ਵਾਪਸ fresh;

  destroy_doc_cache (fresh);
  ਵਾਪਸ expected;
}

ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *
SVG::accelerator_t::doc_cache_get_svg (ਅਚਲ svg_doc_cache_t *doc,
                                       ਬਿਨਾਨਿਸ਼ਾਨੀ *len) ਅਚਲ
{
  ਜੇ (!doc)
  {
    ਜੇ (len) *len = 0;
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਜੇ (len) *len = doc->len;
  ਵਾਪਸ doc->svg;
}

ਸਤਰਅੰਦਰ ਅਚਲ hb_ਸਦਿਸ਼_t<SVG::svg_defs_ਇੰਦਰਾਜ_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ਅਚਲ svg_doc_cache_t *doc) ਅਚਲ
{
  ਵਾਪਸ doc ? &doc->defs_entries : ਖਾਲੀ_ਸੂਚਕ;
}

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
SVG::accelerator_t::doc_cache_get_glyph_ਵਿਸਤਾਰ (ਅਚਲ svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸ਼ੁਰੂ,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸਿਰਾ) ਅਚਲ
{
  ਸਥਿਰ ਅਚਲ uint32_t INVALID_ਵਿਸਤਾਰ = 0xFFFFFFFFu;
  ਜੇ (!doc || doc->ਸ਼ੁਰੂ_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->ਸ਼ੁਰੂ_glyph || glyph > doc->ਸਿਰਾ_glyph)
    ਵਾਪਸ ਝੂਠ;

  ਅਚਲ ਸਵੈਚਾਲਿਤ &ਵਿਸਤਾਰ = doc->glyph_spans.arrayZ[glyph - doc->ਸ਼ੁਰੂ_glyph];
  ਜੇ (ਵਿਸਤਾਰ.ਪਹਿਲਾ == INVALID_ਵਿਸਤਾਰ)
    ਵਾਪਸ ਝੂਠ;
  ਜੇ (unlikely (ਵਿਸਤਾਰ.ਪਹਿਲਾ > ਵਿਸਤਾਰ.second || ਵਿਸਤਾਰ.second > doc->len))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (ਸ਼ੁਰੂ) *ਸ਼ੁਰੂ = ਵਿਸਤਾਰ.ਪਹਿਲਾ;
  ਜੇ (ਸਿਰਾ) *ਸਿਰਾ = ਵਿਸਤਾਰ.second;
  ਵਾਪਸ ਸੱਚ;
}

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
SVG::accelerator_t::doc_cache_ਲੱਭੋ_id_ਵਿਸਤਾਰ (ਅਚਲ svg_doc_cache_t *doc,
                                            svg_id_ਵਿਸਤਾਰ_t id,
                                            ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸ਼ੁਰੂ,
                                            ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸਿਰਾ) ਅਚਲ
{
  ਜੇ (!doc || !id.p || !id.len)
    ਵਾਪਸ ਝੂਠ;
  hb_pair_t<uint32_t, uint32_t> *ਵਿਸਤਾਰ = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (!doc->id_spans.has (id, &ਵਿਸਤਾਰ))
    ਵਾਪਸ ਝੂਠ;
  ਜੇ (unlikely (ਵਿਸਤਾਰ->ਪਹਿਲਾ > ਵਿਸਤਾਰ->second || ਵਿਸਤਾਰ->second > doc->len))
    ਵਾਪਸ ਝੂਠ;
  ਜੇ (ਸ਼ੁਰੂ) *ਸ਼ੁਰੂ = ਵਿਸਤਾਰ->ਪਹਿਲਾ;
  ਜੇ (ਸਿਰਾ) *ਸਿਰਾ = ਵਿਸਤਾਰ->second;
  ਵਾਪਸ ਸੱਚ;
}

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
SVG::accelerator_t::doc_cache_ਲੱਭੋ_id_cstr (ਅਚਲ svg_doc_cache_t *doc,
                                            ਅਚਲ ਅੱਖਰ *id,
                                            ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸ਼ੁਰੂ,
                                            ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸਿਰਾ) ਅਚਲ
{
  ਜੇ (!id) ਵਾਪਸ ਝੂਠ;
  svg_id_ਵਿਸਤਾਰ_t key = {id, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀ_ਲੰਬਾਈ (id)};
  ਵਾਪਸ doc_cache_ਲੱਭੋ_id_ਵਿਸਤਾਰ (doc, key, ਸ਼ੁਰੂ, ਸਿਰਾ);
}

ਬਣਤਰ SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_COLOR_SVG_SVG_HH */
