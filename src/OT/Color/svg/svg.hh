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

#شامل "../../../hb-open-type.hh"
#شامل "../../../hb-blob.hh"
#شامل "../../../hb-limits.hh"
#شامل "../../../hb-map.hh"
#شامل "../../../hb-paint.hh"
#شامل "../../../hb-zlib.hh"
#شامل <ctype.h>
#شامل <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


فضاینام OT {

ایستا درونخطی hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ثابت نویسه **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  ثابت نویسه *data = hb_blob_get_data (blob, &data_len);

  اگر (!data || !data_len)
    برو fail;

  اگر (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    اگر (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      برو fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    اگر (!uncompressed)
      برو fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    اگر (!data || !data_len)
      برو fail;
  }

  اگر (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    برو fail;

  اگر (svg) *svg = data;
  اگر (len) *len = data_len;
  بازگشت blob;

fail:
  hb_blob_destroy (blob);
  اگر (svg) *svg = nullptr;
  اگر (len) *len = 0;
  بازگشت nullptr;
}

ساختار SVGDocumentIndexEntry
{
  صحیح cmp (hb_codepoint_t g) ثابت
  { بازگشت g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () ثابت
  { بازگشت startGlyphID; }

  hb_codepoint_t get_end_glyph () ثابت
  { بازگشت endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned صحیح index_offset) ثابت
  {
    بازگشت hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned صحیح) svgDoc,
				    svgDocLength);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  محافظتشده:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  عمومی:
  DEFINE_SIZE_STATIC (12);
};

ساختار SVG
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  ساختار svg_id_span_t
  {
    ثابت نویسه *p;
    unsigned len;

    منطقی operator == (ثابت svg_id_span_t &o) ثابت
    {
      بازگشت len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ثابت
    {
      uint32_t h = hb_hash (len);
      برای (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned نویسه) p[i];
      بازگشت h;
    }
  };

  ساختار svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  ساختار svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    ثابت نویسه *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  منطقی has_data () ثابت { بازگشت svgDocEntries; }

  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ثابت
    {
      بازگشت table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_شمار () ثابت
    { بازگشت table->get_document_شمار (); }

    منطقی get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ثابت
    { بازگشت table->get_glyph_document_index (glyph_id, index); }

    منطقی get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) ثابت
    { بازگشت table->get_document_glyph_range (index, start_glyph, end_glyph); }

    منطقی has_data () ثابت { بازگشت table->has_data (); }

    ثابت svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ثابت نویسه *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) ثابت;

    ثابت نویسه *
    doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                       unsigned *len) ثابت;

    ثابت hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت;

    منطقی
    doc_cache_get_glyph_span (ثابت svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) ثابت;

    منطقی
    doc_cache_find_id_span (ثابت svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) ثابت;

    منطقی
    doc_cache_find_id_cstr (ثابت svg_doc_cache_t *doc,
                            ثابت نویسه *id,
                            unsigned *start,
                            unsigned *end) ثابت;

    منطقی paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, تهی *data) ثابت
    {
      اگر (!has_data ())
        بازگشت نادرست;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      اگر (blob == hb_blob_get_empty ())
        بازگشت نادرست;

      منطقی ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      بازگشت ret;
    }

    خصوصی:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ثابت نویسه *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) ثابت;

    ایستا تهی destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    عمومی:
    DEFINE_SIZE_STATIC (اندازه (hb_blob_ptr_t<SVG>) +
                        اندازه (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ثابت SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) ثابت
  { بازگشت (این+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_شمار () ثابت
  {
    اگر (!has_data ())
      بازگشت 0;
    بازگشت (این + svgDocEntries).len;
  }

  منطقی get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ثابت
  {
    اگر (!has_data ())
      بازگشت نادرست;
    بازگشت (این + svgDocEntries).bfind (glyph_id, index);
  }

  منطقی get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) ثابت
  {
    اگر (!has_data ())
      بازگشت نادرست;

    ثابت auto &entries = این + svgDocEntries;
    اگر (index >= entries.len)
      بازگشت نادرست;

    ثابت auto &entry = entries.arrayZ[index];
    اگر (start_glyph) *start_glyph = entry.get_start_glyph ();
    اگر (end_glyph) *end_glyph = entry.get_end_glyph ();
    بازگشت درست;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  (این+svgDocEntries).sanitize_shallow (c)));
  }

  محافظتشده:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  عمومی:
  DEFINE_SIZE_STATIC (10);
};

فضاینام _hb_svg_cache_impl {

ساختار glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

ساختار id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

ساختار open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  منطقی in_defs_content;
  منطقی is_defs;
};

ایستا ثابت unsigned MAX_DEPTH = 128;

ایستا درونخطی صحیح
find_substr (ثابت نویسه *s,
             unsigned n,
             unsigned from,
             ثابت نویسه *needle,
             unsigned needle_len)
{
  اگر (!needle_len || from >= n || needle_len > n)
    بازگشت -1;
  برای (unsigned i = from; i + needle_len <= n; i++)
    اگر (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      بازگشت (صحیح) i;
  بازگشت -1;
}

ایستا درونخطی منطقی
parse_id_in_start_tag (ثابت نویسه *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  وقتی (p + 4 <= tag_end)
  {
    اگر (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      وقتی (e < tag_end && svg[e] != '"') e++;
      اگر (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        بازگشت درست;
      }
    }
    اگر (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      وقتی (e < tag_end && svg[e] != '\'') e++;
      اگر (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        بازگشت درست;
      }
    }
    p++;
  }
  بازگشت نادرست;
}

ایستا درونخطی منطقی
parse_glyph_id_span (ثابت SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  اگر (id.len <= 5 || memcmp (id.p, "glyph", 5))
    بازگشت نادرست;

  hb_codepoint_t gid = 0;
  برای (unsigned i = 5; i < id.len; i++)
  {
    unsigned نویسه c = (unsigned نویسه) id.p[i];
    اگر (c < '0' || c > '9')
      بازگشت نادرست;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    اگر (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      بازگشت نادرست;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  بازگشت درست;
}

ایستا درونخطی منطقی
parse_cache_entries_linear (ثابت نویسه *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t پشته[MAX_DEPTH] = {};
  unsigned depth = 0;
  اگر (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    بازگشت نادرست;

  unsigned defs_depth = 0;
  unsigned i = 0;
  وقتی (i < len)
  {
    اگر (svg[i] != '<')
    {
      i++;
      ادامه;
    }

    اگر (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      صحیح cend = find_substr (svg, len, i + 4, "-->", 3);
      اگر (cend < 0) بازگشت نادرست;
      i = (unsigned) cend + 3;
      ادامه;
    }
    اگر (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      صحیح cend = find_substr (svg, len, i + 9, "]]>", 3);
      اگر (cend < 0) بازگشت نادرست;
      i = (unsigned) cend + 3;
      ادامه;
    }

    منطقی closing = (i + 1 < len && svg[i + 1] == '/');
    منطقی special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    نویسه quote = 0;
    وقتی (gt < len)
    {
      نویسه c = svg[gt];
      اگر (quote)
      {
        اگر (c == quote) quote = 0;
      }
      وگرنه
      {
        اگر (c == '"' || c == '\'')
          quote = c;
        وگرنه اگر (c == '>')
          بشکن;
      }
      gt++;
    }
    اگر (gt >= len)
      بازگشت نادرست;

    اگر (special)
    {
      i = gt + 1;
      ادامه;
    }

    unsigned p = i + (closing ? 2 : 1);
    وقتی (p < gt && isspace ((unsigned نویسه) svg[p])) p++;
    ثابت نویسه *name = svg + p;
    unsigned name_len = 0;
    وقتی (p + name_len < gt)
    {
      unsigned نویسه c = (unsigned نویسه) name[name_len];
      اگر (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        بشکن;
      name_len++;
    }
    منطقی is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    اگر (closing)
    {
      اگر (!depth)
      {
        i = gt + 1;
        ادامه;
      }

      open_elem_t e = پشته[--depth];
      unsigned end = gt + 1;

      اگر (e.id.len)
      {
        اگر (unlikely (!id_entries->افزودن_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          بازگشت نادرست;

        اگر (e.in_defs_content)
        {
          اگر (unlikely (!defs_entries->افزودن_or_fail ()))
            بازگشت نادرست;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        اگر (parse_glyph_id_span (e.id, &gid))
        {
          اگر (unlikely (!glyph_spans->افزودن_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            بازگشت نادرست;
        }
      }

      اگر (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      ادامه;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    وقتی (r > i && isspace ((unsigned نویسه) svg[r - 1])) r--;
    منطقی self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    اگر (self_closing)
    {
      unsigned end = gt + 1;
      اگر (e.id.len)
      {
        اگر (unlikely (!id_entries->افزودن_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          بازگشت نادرست;

        اگر (e.in_defs_content)
        {
          اگر (unlikely (!defs_entries->افزودن_or_fail ()))
            بازگشت نادرست;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        اگر (parse_glyph_id_span (e.id, &gid))
        {
          اگر (unlikely (!glyph_spans->افزودن_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            بازگشت نادرست;
        }
      }
    }
    وگرنه
    {
      اگر (unlikely (depth >= MAX_DEPTH))
        بازگشت نادرست;
      پشته[depth++] = e;
      اگر (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  بازگشت درست;
}

} /* namespace _hb_svg_cache_impl */

درونخطی
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_شمار = table->get_document_شمار ();
  اگر (doc_شمار && unlikely (!doc_caches.resize (doc_شمار)))
    doc_caches.clear ();
  برای (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

درونخطی
SVG::accelerator_t::~accelerator_t ()
{
  برای (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

درونخطی تهی
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  اگر (!doc)
    بازگشت;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

درونخطی SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ثابت نویسه *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) ثابت
{
  ایستا ثابت uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (اندازه (svg_doc_cache_t));
  اگر (!doc)
    بازگشت nullptr;

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

  اگر (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    بازگشت nullptr;
  }

  unsigned glyph_شمار = end_glyph - start_glyph + 1;
  اگر (!doc->glyph_spans.resize ((صحیح) glyph_شمار))
  {
    destroy_doc_cache (doc);
    بازگشت nullptr;
  }
  برای (unsigned i = 0; i < glyph_شمار; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  اگر (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    بازگشت nullptr;
  }

  برای (unsigned i = 0; i < glyph_spans.length; i++)
  {
    ثابت auto &span = glyph_spans.arrayZ[i];
    اگر (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      ادامه;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  برای (unsigned i = 0; i < id_entries.length; i++)
  {
    ثابت auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    اگر (doc->id_spans.has (e.id, &out))
      ادامه;
    اگر (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      بازگشت nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  بازگشت doc;
}

درونخطی ثابت SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ثابت نویسه *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) ثابت
{
  اگر (doc_index >= doc_caches.length)
    بازگشت nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  اگر (doc)
    بازگشت doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  اگر (!fresh)
    بازگشت nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  اگر (slot.cmpexch (expected, fresh))
    بازگشت fresh;

  destroy_doc_cache (fresh);
  بازگشت expected;
}

درونخطی ثابت نویسه *
SVG::accelerator_t::doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                                       unsigned *len) ثابت
{
  اگر (!doc)
  {
    اگر (len) *len = 0;
    بازگشت nullptr;
  }
  اگر (len) *len = doc->len;
  بازگشت doc->svg;
}

درونخطی ثابت hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت
{
  بازگشت doc ? &doc->defs_entries : nullptr;
}

درونخطی منطقی
SVG::accelerator_t::doc_cache_get_glyph_span (ثابت svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) ثابت
{
  ایستا ثابت uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  اگر (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    بازگشت نادرست;

  ثابت auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  اگر (span.first == INVALID_SPAN)
    بازگشت نادرست;
  اگر (unlikely (span.first > span.second || span.second > doc->len))
    بازگشت نادرست;

  اگر (start) *start = span.first;
  اگر (end) *end = span.second;
  بازگشت درست;
}

درونخطی منطقی
SVG::accelerator_t::doc_cache_find_id_span (ثابت svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) ثابت
{
  اگر (!doc || !id.p || !id.len)
    بازگشت نادرست;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  اگر (!doc->id_spans.has (id, &span))
    بازگشت نادرست;
  اگر (unlikely (span->first > span->second || span->second > doc->len))
    بازگشت نادرست;
  اگر (start) *start = span->first;
  اگر (end) *end = span->second;
  بازگشت درست;
}

درونخطی منطقی
SVG::accelerator_t::doc_cache_find_id_cstr (ثابت svg_doc_cache_t *doc,
                                            ثابت نویسه *id,
                                            unsigned *start,
                                            unsigned *end) ثابت
{
  اگر (!id) بازگشت نادرست;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  بازگشت doc_cache_find_id_span (doc, key, start, end);
}

ساختار SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
