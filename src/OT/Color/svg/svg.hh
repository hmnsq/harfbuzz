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

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-blob.hh"
#ضمن "../../../hb-limits.hh"
#ضمن "../../../hb-map.hh"
#ضمن "../../../hb-paint.hh"
#ضمن "../../../hb-zlib.hh"
#ضمن <ctype.h>
#ضمن <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


نطاق OT {

ساكن مضمن hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ثابت حرف **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  ثابت حرف *data = hb_blob_get_data (blob, &data_len);

  ܐܢ (!data || !data_len)
    اذهب fail;

  ܐܢ (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    ܐܢ (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      اذهب fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    ܐܢ (!uncompressed)
      اذهب fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    ܐܢ (!data || !data_len)
      اذهب fail;
  }

  ܐܢ (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    اذهب fail;

  ܐܢ (svg) *svg = data;
  ܐܢ (len) *len = data_len;
  ܦܢܐ blob;

fail:
  hb_blob_destroy (blob);
  ܐܢ (svg) *svg = nullptr;
  ܐܢ (len) *len = 0;
  ܦܢܐ nullptr;
}

بنية SVGDocumentIndexEntry
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { ܦܢܐ g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () ثابت
  { ܦܢܐ startGlyphID; }

  hb_codepoint_t get_end_glyph () ثابت
  { ܦܢܐ endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned صحيح index_offset) ثابت
  {
    ܦܢܐ hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned صحيح) svgDoc,
				    svgDocLength);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  محمي:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  عام:
  DEFINE_SIZE_STATIC (12);
};

بنية SVG
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  بنية svg_id_span_t
  {
    ثابت حرف *p;
    unsigned len;

    منطقي operator == (ثابت svg_id_span_t &o) ثابت
    {
      ܦܢܐ len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ثابت
    {
      uint32_t h = hb_hash (len);
      لكل (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned حرف) p[i];
      ܦܢܐ h;
    }
  };

  بنية svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  بنية svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    ثابت حرف *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  منطقي has_data () ثابت { ܦܢܐ svgDocEntries; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ثابت
    {
      ܦܢܐ table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_count () ثابت
    { ܦܢܐ table->get_document_count (); }

    منطقي get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ثابت
    { ܦܢܐ table->get_glyph_document_index (glyph_id, index); }

    منطقي get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) ثابت
    { ܦܢܐ table->get_document_glyph_range (index, start_glyph, end_glyph); }

    منطقي has_data () ثابت { ܦܢܐ table->has_data (); }

    ثابت svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ثابت حرف *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) ثابت;

    ثابت حرف *
    doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                       unsigned *len) ثابت;

    ثابت hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت;

    منطقي
    doc_cache_get_glyph_span (ثابت svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) ثابت;

    منطقي
    doc_cache_find_id_span (ثابت svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) ثابت;

    منطقي
    doc_cache_find_id_cstr (ثابت svg_doc_cache_t *doc,
                            ثابت حرف *id,
                            unsigned *start,
                            unsigned *end) ثابت;

    منطقي paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *data) ثابت
    {
      ܐܢ (!has_data ())
        ܦܢܐ ܕܓܠܐ;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      ܐܢ (blob == hb_blob_get_empty ())
        ܦܢܐ ܕܓܠܐ;

      منطقي ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      ܦܢܐ ret;
    }

    خاص:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ثابت حرف *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) ثابت;

    ساكن فراغ destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    عام:
    DEFINE_SIZE_STATIC (حجم (hb_blob_ptr_t<SVG>) +
                        حجم (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ثابت SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) ثابت
  { ܦܢܐ (هذا+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_count () ثابت
  {
    ܐܢ (!has_data ())
      ܦܢܐ 0;
    ܦܢܐ (هذا + svgDocEntries).len;
  }

  منطقي get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ثابت
  {
    ܐܢ (!has_data ())
      ܦܢܐ ܕܓܠܐ;
    ܦܢܐ (هذا + svgDocEntries).bfind (glyph_id, index);
  }

  منطقي get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) ثابت
  {
    ܐܢ (!has_data ())
      ܦܢܐ ܕܓܠܐ;

    ثابت auto &entries = هذا + svgDocEntries;
    ܐܢ (index >= entries.len)
      ܦܢܐ ܕܓܠܐ;

    ثابت auto &entry = entries.arrayZ[index];
    ܐܢ (start_glyph) *start_glyph = entry.get_start_glyph ();
    ܐܢ (end_glyph) *end_glyph = entry.get_end_glyph ();
    ܦܢܐ ܫܪܪܐ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  (هذا+svgDocEntries).sanitize_shallow (c)));
  }

  محمي:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  عام:
  DEFINE_SIZE_STATIC (10);
};

نطاق _hb_svg_cache_impl {

بنية glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

بنية id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

بنية open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  منطقي in_defs_content;
  منطقي is_defs;
};

ساكن ثابت unsigned MAX_DEPTH = 128;

ساكن مضمن صحيح
find_substr (ثابت حرف *s,
             unsigned n,
             unsigned from,
             ثابت حرف *needle,
             unsigned needle_len)
{
  ܐܢ (!needle_len || from >= n || needle_len > n)
    ܦܢܐ -1;
  لكل (unsigned i = from; i + needle_len <= n; i++)
    ܐܢ (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      ܦܢܐ (صحيح) i;
  ܦܢܐ -1;
}

ساكن مضمن منطقي
parse_id_in_start_tag (ثابت حرف *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  ܟܡܐ (p + 4 <= tag_end)
  {
    ܐܢ (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ܟܡܐ (e < tag_end && svg[e] != '"') e++;
      ܐܢ (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ܦܢܐ ܫܪܪܐ;
      }
    }
    ܐܢ (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ܟܡܐ (e < tag_end && svg[e] != '\'') e++;
      ܐܢ (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ܦܢܐ ܫܪܪܐ;
      }
    }
    p++;
  }
  ܦܢܐ ܕܓܠܐ;
}

ساكن مضمن منطقي
parse_glyph_id_span (ثابت SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  ܐܢ (id.len <= 5 || memcmp (id.p, "glyph", 5))
    ܦܢܐ ܕܓܠܐ;

  hb_codepoint_t gid = 0;
  لكل (unsigned i = 5; i < id.len; i++)
  {
    unsigned حرف c = (unsigned حرف) id.p[i];
    ܐܢ (c < '0' || c > '9')
      ܦܢܐ ܕܓܠܐ;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    ܐܢ (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      ܦܢܐ ܕܓܠܐ;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  ܦܢܐ ܫܪܪܐ;
}

ساكن مضمن منطقي
parse_cache_entries_linear (ثابت حرف *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t stack[MAX_DEPTH] = {};
  unsigned depth = 0;
  ܐܢ (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    ܦܢܐ ܕܓܠܐ;

  unsigned defs_depth = 0;
  unsigned i = 0;
  ܟܡܐ (i < len)
  {
    ܐܢ (svg[i] != '<')
    {
      i++;
      استمر;
    }

    ܐܢ (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      صحيح cend = find_substr (svg, len, i + 4, "-->", 3);
      ܐܢ (cend < 0) ܦܢܐ ܕܓܠܐ;
      i = (unsigned) cend + 3;
      استمر;
    }
    ܐܢ (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      صحيح cend = find_substr (svg, len, i + 9, "]]>", 3);
      ܐܢ (cend < 0) ܦܢܐ ܕܓܠܐ;
      i = (unsigned) cend + 3;
      استمر;
    }

    منطقي closing = (i + 1 < len && svg[i + 1] == '/');
    منطقي special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    حرف quote = 0;
    ܟܡܐ (gt < len)
    {
      حرف c = svg[gt];
      ܐܢ (quote)
      {
        ܐܢ (c == quote) quote = 0;
      }
      ܐܚܪܢܐ
      {
        ܐܢ (c == '"' || c == '\'')
          quote = c;
        ܐܚܪܢܐ ܐܢ (c == '>')
          اكسر;
      }
      gt++;
    }
    ܐܢ (gt >= len)
      ܦܢܐ ܕܓܠܐ;

    ܐܢ (special)
    {
      i = gt + 1;
      استمر;
    }

    unsigned p = i + (closing ? 2 : 1);
    ܟܡܐ (p < gt && isspace ((unsigned حرف) svg[p])) p++;
    ثابت حرف *name = svg + p;
    unsigned name_len = 0;
    ܟܡܐ (p + name_len < gt)
    {
      unsigned حرف c = (unsigned حرف) name[name_len];
      ܐܢ (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        اكسر;
      name_len++;
    }
    منطقي is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    ܐܢ (closing)
    {
      ܐܢ (!depth)
      {
        i = gt + 1;
        استمر;
      }

      open_elem_t e = stack[--depth];
      unsigned end = gt + 1;

      ܐܢ (e.id.len)
      {
        ܐܢ (unlikely (!id_entries->push_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          ܦܢܐ ܕܓܠܐ;

        ܐܢ (e.in_defs_content)
        {
          ܐܢ (unlikely (!defs_entries->push_or_fail ()))
            ܦܢܐ ܕܓܠܐ;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        ܐܢ (parse_glyph_id_span (e.id, &gid))
        {
          ܐܢ (unlikely (!glyph_spans->push_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ܦܢܐ ܕܓܠܐ;
        }
      }

      ܐܢ (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      استمر;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    ܟܡܐ (r > i && isspace ((unsigned حرف) svg[r - 1])) r--;
    منطقي self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    ܐܢ (self_closing)
    {
      unsigned end = gt + 1;
      ܐܢ (e.id.len)
      {
        ܐܢ (unlikely (!id_entries->push_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          ܦܢܐ ܕܓܠܐ;

        ܐܢ (e.in_defs_content)
        {
          ܐܢ (unlikely (!defs_entries->push_or_fail ()))
            ܦܢܐ ܕܓܠܐ;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        ܐܢ (parse_glyph_id_span (e.id, &gid))
        {
          ܐܢ (unlikely (!glyph_spans->push_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ܦܢܐ ܕܓܠܐ;
        }
      }
    }
    ܐܚܪܢܐ
    {
      ܐܢ (unlikely (depth >= MAX_DEPTH))
        ܦܢܐ ܕܓܠܐ;
      stack[depth++] = e;
      ܐܢ (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  ܦܢܐ ܫܪܪܐ;
}

} /* namespace _hb_svg_cache_impl */

مضمن
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_count = table->get_document_count ();
  ܐܢ (doc_count && unlikely (!doc_caches.resize (doc_count)))
    doc_caches.clear ();
  لكل (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

مضمن
SVG::accelerator_t::~accelerator_t ()
{
  لكل (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

مضمن فراغ
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  ܐܢ (!doc)
    ܦܢܐ;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

مضمن SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ثابت حرف *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) ثابت
{
  ساكن ثابت uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (حجم (svg_doc_cache_t));
  ܐܢ (!doc)
    ܦܢܐ nullptr;

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

  ܐܢ (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    ܦܢܐ nullptr;
  }

  unsigned glyph_count = end_glyph - start_glyph + 1;
  ܐܢ (!doc->glyph_spans.resize ((صحيح) glyph_count))
  {
    destroy_doc_cache (doc);
    ܦܢܐ nullptr;
  }
  لكل (unsigned i = 0; i < glyph_count; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  ܐܢ (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    ܦܢܐ nullptr;
  }

  لكل (unsigned i = 0; i < glyph_spans.length; i++)
  {
    ثابت auto &span = glyph_spans.arrayZ[i];
    ܐܢ (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      استمر;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  لكل (unsigned i = 0; i < id_entries.length; i++)
  {
    ثابت auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    ܐܢ (doc->id_spans.has (e.id, &out))
      استمر;
    ܐܢ (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      ܦܢܐ nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  ܦܢܐ doc;
}

مضمن ثابت SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ثابت حرف *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) ثابت
{
  ܐܢ (doc_index >= doc_caches.length)
    ܦܢܐ nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  ܐܢ (doc)
    ܦܢܐ doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  ܐܢ (!fresh)
    ܦܢܐ nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  ܐܢ (slot.cmpexch (expected, fresh))
    ܦܢܐ fresh;

  destroy_doc_cache (fresh);
  ܦܢܐ expected;
}

مضمن ثابت حرف *
SVG::accelerator_t::doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                                       unsigned *len) ثابت
{
  ܐܢ (!doc)
  {
    ܐܢ (len) *len = 0;
    ܦܢܐ nullptr;
  }
  ܐܢ (len) *len = doc->len;
  ܦܢܐ doc->svg;
}

مضمن ثابت hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت
{
  ܦܢܐ doc ? &doc->defs_entries : nullptr;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_get_glyph_span (ثابت svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) ثابت
{
  ساكن ثابت uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  ܐܢ (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    ܦܢܐ ܕܓܠܐ;

  ثابت auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  ܐܢ (span.first == INVALID_SPAN)
    ܦܢܐ ܕܓܠܐ;
  ܐܢ (unlikely (span.first > span.second || span.second > doc->len))
    ܦܢܐ ܕܓܠܐ;

  ܐܢ (start) *start = span.first;
  ܐܢ (end) *end = span.second;
  ܦܢܐ ܫܪܪܐ;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_find_id_span (ثابت svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) ثابت
{
  ܐܢ (!doc || !id.p || !id.len)
    ܦܢܐ ܕܓܠܐ;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  ܐܢ (!doc->id_spans.has (id, &span))
    ܦܢܐ ܕܓܠܐ;
  ܐܢ (unlikely (span->first > span->second || span->second > doc->len))
    ܦܢܐ ܕܓܠܐ;
  ܐܢ (start) *start = span->first;
  ܐܢ (end) *end = span->second;
  ܦܢܐ ܫܪܪܐ;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_find_id_cstr (ثابت svg_doc_cache_t *doc,
                                            ثابت حرف *id,
                                            unsigned *start,
                                            unsigned *end) ثابت
{
  ܐܢ (!id) ܦܢܐ ܕܓܠܐ;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  ܦܢܐ doc_cache_find_id_span (doc, key, start, end);
}

بنية SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
