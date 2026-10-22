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

#ifndef OT_COLOR_SVG_SVG_HH
#define OT_COLOR_SVG_SVG_HH

#ಅನ್ತರ್ಭಾವ "../../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-blob.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-limits.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-map.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-paint.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-zlib.hh"
#ಅನ್ತರ್ಭಾವ <ctype.h>
#ಅನ್ತರ್ಭಾವ <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


ನಾಮಾಕಾಶ OT {

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ಧ್ರುವ ವರ್ಣ **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  ಧ್ರುವ ವರ್ಣ *data = hb_blob_get_data (blob, &data_len);

  ಯದಿ (!data || !data_len)
    ಗಚ್ಛ fail;

  ಯದಿ (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    ಯದಿ (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      ಗಚ್ಛ fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    ಯದಿ (!uncompressed)
      ಗಚ್ಛ fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    ಯದಿ (!data || !data_len)
      ಗಚ್ಛ fail;
  }

  ಯದಿ (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    ಗಚ್ಛ fail;

  ಯದಿ (svg) *svg = data;
  ಯದಿ (len) *len = data_len;
  ಪ್ರತ್ಯಾಗಮ blob;

fail:
  hb_blob_destroy (blob);
  ಯದಿ (svg) *svg = nullptr;
  ಯದಿ (len) *len = 0;
  ಪ್ರತ್ಯಾಗಮ nullptr;
}

ಸಂರಚನಾ SVGDocumentIndexEntry
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t g) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ startGlyphID; }

  hb_codepoint_t get_end_glyph () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned ಪೂರ್ಣಾಙ್ಕ index_offset) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned ಪೂರ್ಣಾಙ್ಕ) svgDoc,
				    svgDocLength);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  ರಕ್ಷಿತ:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};

ಸಂರಚನಾ SVG
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  ಸಂರಚನಾ svg_id_span_t
  {
    ಧ್ರುವ ವರ್ಣ *p;
    unsigned len;

    ತಾರ್ಕಿಕ operator == (ಧ್ರುವ svg_id_span_t &o) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ಧ್ರುವ
    {
      uint32_t h = hb_hash (len);
      ಪ್ರತಿ (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned ವರ್ಣ) p[i];
      ಪ್ರತ್ಯಾಗಮ h;
    }
  };

  ಸಂರಚನಾ svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  ಸಂರಚನಾ svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    ಧ್ರುವ ವರ್ಣ *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ svgDocEntries; }

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_ಎಣಿಕೆ () ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ table->get_document_ಎಣಿಕೆ (); }

    ತಾರ್ಕಿಕ get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ table->get_glyph_document_index (glyph_id, index); }

    ತಾರ್ಕಿಕ get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ table->get_document_glyph_range (index, start_glyph, end_glyph); }

    ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table->has_data (); }

    ಧ್ರುವ svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ಧ್ರುವ ವರ್ಣ *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) ಧ್ರುವ;

    ಧ್ರುವ ವರ್ಣ *
    doc_cache_get_svg (ಧ್ರುವ svg_doc_cache_t *doc,
                       unsigned *len) ಧ್ರುವ;

    ಧ್ರುವ hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (ಧ್ರುವ svg_doc_cache_t *doc) ಧ್ರುವ;

    ತಾರ್ಕಿಕ
    doc_cache_get_glyph_span (ಧ್ರುವ svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) ಧ್ರುವ;

    ತಾರ್ಕಿಕ
    doc_cache_find_id_span (ಧ್ರುವ svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) ಧ್ರುವ;

    ತಾರ್ಕಿಕ
    doc_cache_find_id_cstr (ಧ್ರುವ svg_doc_cache_t *doc,
                            ಧ್ರುವ ವರ್ಣ *id,
                            unsigned *start,
                            unsigned *end) ಧ್ರುವ;

    ತಾರ್ಕಿಕ paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ಶೂನ್ಯ *data) ಧ್ರುವ
    {
      ಯದಿ (!has_data ())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      ಯದಿ (blob == hb_blob_get_empty ())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ತಾರ್ಕಿಕ ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      ಪ್ರತ್ಯಾಗಮ ret;
    }

    ಗುಪ್ತ:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ಧ್ರುವ ವರ್ಣ *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) ಧ್ರುವ;

    ಸ್ಥಿರ ಶೂನ್ಯ destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    ಸಾರ್ವಜನಿಕ:
    DEFINE_SIZE_STATIC (ಪರಿಮಾಣ (hb_blob_ptr_t<SVG>) +
                        ಪರಿಮಾಣ (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ಧ್ರುವ SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_ಎಣಿಕೆ () ಧ್ರುವ
  {
    ಯದಿ (!has_data ())
      ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್ + svgDocEntries).len;
  }

  ತಾರ್ಕಿಕ get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ಧ್ರುವ
  {
    ಯದಿ (!has_data ())
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್ + svgDocEntries).bfind (glyph_id, index);
  }

  ತಾರ್ಕಿಕ get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) ಧ್ರುವ
  {
    ಯದಿ (!has_data ())
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ auto &entries = ಏತತ್ + svgDocEntries;
    ಯದಿ (index >= entries.len)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ auto &entry = entries.arrayZ[index];
    ಯದಿ (start_glyph) *start_glyph = entry.get_start_glyph ();
    ಯದಿ (end_glyph) *end_glyph = entry.get_end_glyph ();
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  (ಏತತ್+svgDocEntries).sanitize_shallow (c)));
  }

  ರಕ್ಷಿತ:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (10);
};

ನಾಮಾಕಾಶ _hb_svg_cache_impl {

ಸಂರಚನಾ glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

ಸಂರಚನಾ id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

ಸಂರಚನಾ open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  ತಾರ್ಕಿಕ in_defs_content;
  ತಾರ್ಕಿಕ is_defs;
};

ಸ್ಥಿರ ಧ್ರುವ unsigned MAX_DEPTH = 128;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
find_substr (ಧ್ರುವ ವರ್ಣ *s,
             unsigned n,
             unsigned from,
             ಧ್ರುವ ವರ್ಣ *needle,
             unsigned needle_len)
{
  ಯದಿ (!needle_len || from >= n || needle_len > n)
    ಪ್ರತ್ಯಾಗಮ -1;
  ಪ್ರತಿ (unsigned i = from; i + needle_len <= n; i++)
    ಯದಿ (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) i;
  ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
parse_id_in_start_tag (ಧ್ರುವ ವರ್ಣ *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  ಯಾವತ್ (p + 4 <= tag_end)
  {
    ಯದಿ (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ಯಾವತ್ (e < tag_end && svg[e] != '"') e++;
      ಯದಿ (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }
    ಯದಿ (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ಯಾವತ್ (e < tag_end && svg[e] != '\'') e++;
      ಯದಿ (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }
    p++;
  }
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
parse_glyph_id_span (ಧ್ರುವ SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  ಯದಿ (id.len <= 5 || memcmp (id.p, "glyph", 5))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_codepoint_t gid = 0;
  ಪ್ರತಿ (unsigned i = 5; i < id.len; i++)
  {
    unsigned ವರ್ಣ c = (unsigned ವರ್ಣ) id.p[i];
    ಯದಿ (c < '0' || c > '9')
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    ಯದಿ (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
parse_cache_entries_linear (ಧ್ರುವ ವರ್ಣ *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t ರಾಶಿ[MAX_DEPTH] = {};
  unsigned depth = 0;
  ಯದಿ (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  unsigned defs_depth = 0;
  unsigned i = 0;
  ಯಾವತ್ (i < len)
  {
    ಯದಿ (svg[i] != '<')
    {
      i++;
      ಅನುವರ್ತ;
    }

    ಯದಿ (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      ಪೂರ್ಣಾಙ್ಕ cend = find_substr (svg, len, i + 4, "-->", 3);
      ಯದಿ (cend < 0) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      i = (unsigned) cend + 3;
      ಅನುವರ್ತ;
    }
    ಯದಿ (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      ಪೂರ್ಣಾಙ್ಕ cend = find_substr (svg, len, i + 9, "]]>", 3);
      ಯದಿ (cend < 0) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      i = (unsigned) cend + 3;
      ಅನುವರ್ತ;
    }

    ತಾರ್ಕಿಕ closing = (i + 1 < len && svg[i + 1] == '/');
    ತಾರ್ಕಿಕ special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    ವರ್ಣ quote = 0;
    ಯಾವತ್ (gt < len)
    {
      ವರ್ಣ c = svg[gt];
      ಯದಿ (quote)
      {
        ಯದಿ (c == quote) quote = 0;
      }
      ಅನ್ಯಥಾ
      {
        ಯದಿ (c == '"' || c == '\'')
          quote = c;
        ಅನ್ಯಥಾ ಯದಿ (c == '>')
          ಭಙ್ಗ;
      }
      gt++;
    }
    ಯದಿ (gt >= len)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (special)
    {
      i = gt + 1;
      ಅನುವರ್ತ;
    }

    unsigned p = i + (closing ? 2 : 1);
    ಯಾವತ್ (p < gt && isspace ((unsigned ವರ್ಣ) svg[p])) p++;
    ಧ್ರುವ ವರ್ಣ *name = svg + p;
    unsigned name_len = 0;
    ಯಾವತ್ (p + name_len < gt)
    {
      unsigned ವರ್ಣ c = (unsigned ವರ್ಣ) name[name_len];
      ಯದಿ (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        ಭಙ್ಗ;
      name_len++;
    }
    ತಾರ್ಕಿಕ is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    ಯದಿ (closing)
    {
      ಯದಿ (!depth)
      {
        i = gt + 1;
        ಅನುವರ್ತ;
      }

      open_elem_t e = ರಾಶಿ[--depth];
      unsigned end = gt + 1;

      ಯದಿ (e.id.len)
      {
        ಯದಿ (unlikely (!id_entries->ತಳ್ಳು_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಯದಿ (e.in_defs_content)
        {
          ಯದಿ (unlikely (!defs_entries->ತಳ್ಳು_or_fail ()))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        ಯದಿ (parse_glyph_id_span (e.id, &gid))
        {
          ಯದಿ (unlikely (!glyph_spans->ತಳ್ಳು_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
      }

      ಯದಿ (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      ಅನುವರ್ತ;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    ಯಾವತ್ (r > i && isspace ((unsigned ವರ್ಣ) svg[r - 1])) r--;
    ತಾರ್ಕಿಕ self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    ಯದಿ (self_closing)
    {
      unsigned end = gt + 1;
      ಯದಿ (e.id.len)
      {
        ಯದಿ (unlikely (!id_entries->ತಳ್ಳು_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಯದಿ (e.in_defs_content)
        {
          ಯದಿ (unlikely (!defs_entries->ತಳ್ಳು_or_fail ()))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        ಯದಿ (parse_glyph_id_span (e.id, &gid))
        {
          ಯದಿ (unlikely (!glyph_spans->ತಳ್ಳು_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
      }
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (unlikely (depth >= MAX_DEPTH))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ರಾಶಿ[depth++] = e;
      ಯದಿ (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

} /* namespace _hb_svg_cache_impl */

ಅನ್ತಃಸ್ಥ
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_ಎಣಿಕೆ = table->get_document_ಎಣಿಕೆ ();
  ಯದಿ (doc_ಎಣಿಕೆ && unlikely (!doc_caches.resize (doc_ಎಣಿಕೆ)))
    doc_caches.clear ();
  ಪ್ರತಿ (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

ಅನ್ತಃಸ್ಥ
SVG::accelerator_t::~accelerator_t ()
{
  ಪ್ರತಿ (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  ಯದಿ (!doc)
    ಪ್ರತ್ಯಾಗಮ;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

ಅನ್ತಃಸ್ಥ SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ಧ್ರುವ ವರ್ಣ *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) ಧ್ರುವ
{
  ಸ್ಥಿರ ಧ್ರುವ uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (ಪರಿಮಾಣ (svg_doc_cache_t));
  ಯದಿ (!doc)
    ಪ್ರತ್ಯಾಗಮ nullptr;

  doc->blob = nullptr;
  doc->svg = nullptr;
  doc->len = 0;
  doc->defs_entries.init ();
  doc->start_glyph = HB_CODEPOINT_INVALID;
  doc->end_glyph = HB_CODEPOINT_INVALID;
  doc->glyph_spans.init ();
  doc->id_spans.init ();

  doc->blob = hb_blob_reference (image);
  doc->svg = svg;
  doc->len = len;
  doc->start_glyph = start_glyph;
  doc->end_glyph = end_glyph;

  ಯದಿ (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  unsigned glyph_ಎಣಿಕೆ = end_glyph - start_glyph + 1;
  ಯದಿ (!doc->glyph_spans.resize ((ಪೂರ್ಣಾಙ್ಕ) glyph_ಎಣಿಕೆ))
  {
    destroy_doc_cache (doc);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  ಪ್ರತಿ (unsigned i = 0; i < glyph_ಎಣಿಕೆ; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  ಯದಿ (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಪ್ರತಿ (unsigned i = 0; i < glyph_spans.length; i++)
  {
    ಧ್ರುವ auto &span = glyph_spans.arrayZ[i];
    ಯದಿ (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      ಅನುವರ್ತ;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  ಪ್ರತಿ (unsigned i = 0; i < id_entries.length; i++)
  {
    ಧ್ರುವ auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    ಯದಿ (doc->id_spans.has (e.id, &out))
      ಅನುವರ್ತ;
    ಯದಿ (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  ಪ್ರತ್ಯಾಗಮ doc;
}

ಅನ್ತಃಸ್ಥ ಧ್ರುವ SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ಧ್ರುವ ವರ್ಣ *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) ಧ್ರುವ
{
  ಯದಿ (doc_index >= doc_caches.length)
    ಪ್ರತ್ಯಾಗಮ nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  ಯದಿ (doc)
    ಪ್ರತ್ಯಾಗಮ doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  ಯದಿ (!fresh)
    ಪ್ರತ್ಯಾಗಮ nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  ಯದಿ (slot.cmpexch (expected, fresh))
    ಪ್ರತ್ಯಾಗಮ fresh;

  destroy_doc_cache (fresh);
  ಪ್ರತ್ಯಾಗಮ expected;
}

ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *
SVG::accelerator_t::doc_cache_get_svg (ಧ್ರುವ svg_doc_cache_t *doc,
                                       unsigned *len) ಧ್ರುವ
{
  ಯದಿ (!doc)
  {
    ಯದಿ (len) *len = 0;
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  ಯದಿ (len) *len = doc->len;
  ಪ್ರತ್ಯಾಗಮ doc->svg;
}

ಅನ್ತಃಸ್ಥ ಧ್ರುವ hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ಧ್ರುವ svg_doc_cache_t *doc) ಧ್ರುವ
{
  ಪ್ರತ್ಯಾಗಮ doc ? &doc->defs_entries : nullptr;
}

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
SVG::accelerator_t::doc_cache_get_glyph_span (ಧ್ರುವ svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) ಧ್ರುವ
{
  ಸ್ಥಿರ ಧ್ರುವ uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  ಯದಿ (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಧ್ರುವ auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  ಯದಿ (span.first == INVALID_SPAN)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (unlikely (span.first > span.second || span.second > doc->len))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯದಿ (start) *start = span.first;
  ಯದಿ (end) *end = span.second;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
SVG::accelerator_t::doc_cache_find_id_span (ಧ್ರುವ svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) ಧ್ರುವ
{
  ಯದಿ (!doc || !id.p || !id.len)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  ಯದಿ (!doc->id_spans.has (id, &span))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (unlikely (span->first > span->second || span->second > doc->len))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (start) *start = span->first;
  ಯದಿ (end) *end = span->second;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
SVG::accelerator_t::doc_cache_find_id_cstr (ಧ್ರುವ svg_doc_cache_t *doc,
                                            ಧ್ರುವ ವರ್ಣ *id,
                                            unsigned *start,
                                            unsigned *end) ಧ್ರುವ
{
  ಯದಿ (!id) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  ಪ್ರತ್ಯಾಗಮ doc_cache_find_id_span (doc, key, start, end);
}

ಸಂರಚನಾ SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
