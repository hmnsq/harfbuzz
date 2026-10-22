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

  אם (!data || !data_len)
    اذهب fail;

  אם (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    אם (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      اذهب fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    אם (!uncompressed)
      اذهب fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    אם (!data || !data_len)
      اذهب fail;
  }

  אם (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    اذهب fail;

  אם (svg) *svg = data;
  אם (len) *len = data_len;
  החזר blob;

fail:
  hb_blob_destroy (blob);
  אם (svg) *svg = nullptr;
  אם (len) *len = 0;
  החזר nullptr;
}

بنية SVGDocumentIndexEntry
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { החזר g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () ثابت
  { החזר startGlyphID; }

  hb_codepoint_t get_end_glyph () ثابت
  { החזר endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned صحيح index_offset) ثابت
  {
    החזר hb_blob_create_sub_blob (svg_blob,
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
      החזר len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ثابت
    {
      uint32_t h = hb_hash (len);
      עבור (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned حرف) p[i];
      החזר h;
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

  منطقي has_data () ثابت { החזר svgDocEntries; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ثابت
    {
      החזר table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_count () ثابت
    { החזר table->get_document_count (); }

    منطقي get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ثابت
    { החזר table->get_glyph_document_index (glyph_id, index); }

    منطقي get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) ثابت
    { החזר table->get_document_glyph_range (index, start_glyph, end_glyph); }

    منطقي has_data () ثابت { החזר table->has_data (); }

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
      אם (!has_data ())
        החזר שקר;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      אם (blob == hb_blob_get_empty ())
        החזר שקר;

      منطقي ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      החזר ret;
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
  { החזר (هذا+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_count () ثابت
  {
    אם (!has_data ())
      החזר 0;
    החזר (هذا + svgDocEntries).len;
  }

  منطقي get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ثابت
  {
    אם (!has_data ())
      החזר שקר;
    החזר (هذا + svgDocEntries).bfind (glyph_id, index);
  }

  منطقي get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) ثابت
  {
    אם (!has_data ())
      החזר שקר;

    ثابت auto &entries = هذا + svgDocEntries;
    אם (index >= entries.len)
      החזר שקר;

    ثابت auto &entry = entries.arrayZ[index];
    אם (start_glyph) *start_glyph = entry.get_start_glyph ();
    אם (end_glyph) *end_glyph = entry.get_end_glyph ();
    החזר אמת;
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
  אם (!needle_len || from >= n || needle_len > n)
    החזר -1;
  עבור (unsigned i = from; i + needle_len <= n; i++)
    אם (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      החזר (صحيح) i;
  החזר -1;
}

ساكن مضمن منطقي
parse_id_in_start_tag (ثابت حرف *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  כלעוד (p + 4 <= tag_end)
  {
    אם (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      כלעוד (e < tag_end && svg[e] != '"') e++;
      אם (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        החזר אמת;
      }
    }
    אם (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      כלעוד (e < tag_end && svg[e] != '\'') e++;
      אם (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        החזר אמת;
      }
    }
    p++;
  }
  החזר שקר;
}

ساكن مضمن منطقي
parse_glyph_id_span (ثابت SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  אם (id.len <= 5 || memcmp (id.p, "glyph", 5))
    החזר שקר;

  hb_codepoint_t gid = 0;
  עבור (unsigned i = 5; i < id.len; i++)
  {
    unsigned حرف c = (unsigned حرف) id.p[i];
    אם (c < '0' || c > '9')
      החזר שקר;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    אם (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      החזר שקר;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  החזר אמת;
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
  אם (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    החזר שקר;

  unsigned defs_depth = 0;
  unsigned i = 0;
  כלעוד (i < len)
  {
    אם (svg[i] != '<')
    {
      i++;
      استمر;
    }

    אם (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      صحيح cend = find_substr (svg, len, i + 4, "-->", 3);
      אם (cend < 0) החזר שקר;
      i = (unsigned) cend + 3;
      استمر;
    }
    אם (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      صحيح cend = find_substr (svg, len, i + 9, "]]>", 3);
      אם (cend < 0) החזר שקר;
      i = (unsigned) cend + 3;
      استمر;
    }

    منطقي closing = (i + 1 < len && svg[i + 1] == '/');
    منطقي special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    حرف quote = 0;
    כלעוד (gt < len)
    {
      حرف c = svg[gt];
      אם (quote)
      {
        אם (c == quote) quote = 0;
      }
      אחרת
      {
        אם (c == '"' || c == '\'')
          quote = c;
        אחרת אם (c == '>')
          اكسر;
      }
      gt++;
    }
    אם (gt >= len)
      החזר שקר;

    אם (special)
    {
      i = gt + 1;
      استمر;
    }

    unsigned p = i + (closing ? 2 : 1);
    כלעוד (p < gt && isspace ((unsigned حرف) svg[p])) p++;
    ثابت حرف *name = svg + p;
    unsigned name_len = 0;
    כלעוד (p + name_len < gt)
    {
      unsigned حرف c = (unsigned حرف) name[name_len];
      אם (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        اكسر;
      name_len++;
    }
    منطقي is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    אם (closing)
    {
      אם (!depth)
      {
        i = gt + 1;
        استمر;
      }

      open_elem_t e = stack[--depth];
      unsigned end = gt + 1;

      אם (e.id.len)
      {
        אם (unlikely (!id_entries->push_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          החזר שקר;

        אם (e.in_defs_content)
        {
          אם (unlikely (!defs_entries->push_or_fail ()))
            החזר שקר;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        אם (parse_glyph_id_span (e.id, &gid))
        {
          אם (unlikely (!glyph_spans->push_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            החזר שקר;
        }
      }

      אם (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      استمر;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    כלעוד (r > i && isspace ((unsigned حرف) svg[r - 1])) r--;
    منطقي self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    אם (self_closing)
    {
      unsigned end = gt + 1;
      אם (e.id.len)
      {
        אם (unlikely (!id_entries->push_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          החזר שקר;

        אם (e.in_defs_content)
        {
          אם (unlikely (!defs_entries->push_or_fail ()))
            החזר שקר;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        אם (parse_glyph_id_span (e.id, &gid))
        {
          אם (unlikely (!glyph_spans->push_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            החזר שקר;
        }
      }
    }
    אחרת
    {
      אם (unlikely (depth >= MAX_DEPTH))
        החזר שקר;
      stack[depth++] = e;
      אם (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  החזר אמת;
}

} /* namespace _hb_svg_cache_impl */

مضمن
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_count = table->get_document_count ();
  אם (doc_count && unlikely (!doc_caches.resize (doc_count)))
    doc_caches.clear ();
  עבור (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

مضمن
SVG::accelerator_t::~accelerator_t ()
{
  עבור (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

مضمن فراغ
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  אם (!doc)
    החזר;
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
  אם (!doc)
    החזר nullptr;

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

  אם (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    החזר nullptr;
  }

  unsigned glyph_count = end_glyph - start_glyph + 1;
  אם (!doc->glyph_spans.resize ((صحيح) glyph_count))
  {
    destroy_doc_cache (doc);
    החזר nullptr;
  }
  עבור (unsigned i = 0; i < glyph_count; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  אם (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    החזר nullptr;
  }

  עבור (unsigned i = 0; i < glyph_spans.length; i++)
  {
    ثابت auto &span = glyph_spans.arrayZ[i];
    אם (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      استمر;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  עבור (unsigned i = 0; i < id_entries.length; i++)
  {
    ثابت auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    אם (doc->id_spans.has (e.id, &out))
      استمر;
    אם (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      החזר nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  החזר doc;
}

مضمن ثابت SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ثابت حرف *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) ثابت
{
  אם (doc_index >= doc_caches.length)
    החזר nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  אם (doc)
    החזר doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  אם (!fresh)
    החזר nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  אם (slot.cmpexch (expected, fresh))
    החזר fresh;

  destroy_doc_cache (fresh);
  החזר expected;
}

مضمن ثابت حرف *
SVG::accelerator_t::doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                                       unsigned *len) ثابت
{
  אם (!doc)
  {
    אם (len) *len = 0;
    החזר nullptr;
  }
  אם (len) *len = doc->len;
  החזר doc->svg;
}

مضمن ثابت hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت
{
  החזר doc ? &doc->defs_entries : nullptr;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_get_glyph_span (ثابت svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) ثابت
{
  ساكن ثابت uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  אם (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    החזר שקר;

  ثابت auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  אם (span.first == INVALID_SPAN)
    החזר שקר;
  אם (unlikely (span.first > span.second || span.second > doc->len))
    החזר שקר;

  אם (start) *start = span.first;
  אם (end) *end = span.second;
  החזר אמת;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_find_id_span (ثابت svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) ثابت
{
  אם (!doc || !id.p || !id.len)
    החזר שקר;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  אם (!doc->id_spans.has (id, &span))
    החזר שקר;
  אם (unlikely (span->first > span->second || span->second > doc->len))
    החזר שקר;
  אם (start) *start = span->first;
  אם (end) *end = span->second;
  החזר אמת;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_find_id_cstr (ثابت svg_doc_cache_t *doc,
                                            ثابت حرف *id,
                                            unsigned *start,
                                            unsigned *end) ثابت
{
  אם (!id) החזר שקר;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  החזר doc_cache_find_id_span (doc, key, start, end);
}

بنية SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
