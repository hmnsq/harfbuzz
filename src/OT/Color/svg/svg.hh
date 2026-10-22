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

#अन्तर्भाव "../../../hb-open-type.hh"
#अन्तर्भाव "../../../hb-blob.hh"
#अन्तर्भाव "../../../hb-limits.hh"
#अन्तर्भाव "../../../hb-map.hh"
#अन्तर्भाव "../../../hb-paint.hh"
#अन्तर्भाव "../../../hb-zlib.hh"
#अन्तर्भाव <ctype.h>
#अन्तर्भाव <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


नामाकाश OT {

स्थिर अन्तःस्थ hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ध्रुव वर्ण **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  ध्रुव वर्ण *data = hb_blob_get_data (blob, &data_len);

  सचे (!data || !data_len)
    गच्छ fail;

  सचे (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    सचे (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      गच्छ fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    सचे (!uncompressed)
      गच्छ fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    सचे (!data || !data_len)
      गच्छ fail;
  }

  सचे (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    गच्छ fail;

  सचे (svg) *svg = data;
  सचे (len) *len = data_len;
  पटिनिवत्त blob;

fail:
  hb_blob_destroy (blob);
  सचे (svg) *svg = nullptr;
  सचे (len) *len = 0;
  पटिनिवत्त nullptr;
}

संरचना SVGDocumentIndexEntry
{
  पूर्णाङ्क cmp (hb_codepoint_t g) ध्रुव
  { पटिनिवत्त g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () ध्रुव
  { पटिनिवत्त startGlyphID; }

  hb_codepoint_t get_end_glyph () ध्रुव
  { पटिनिवत्त endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned पूर्णाङ्क index_offset) ध्रुव
  {
    पटिनिवत्त hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned पूर्णाङ्क) svgDoc,
				    svgDocLength);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  रक्षित:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

संरचना SVG
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  संरचना svg_id_span_t
  {
    ध्रुव वर्ण *p;
    unsigned len;

    तार्किक operator == (ध्रुव svg_id_span_t &o) ध्रुव
    {
      पटिनिवत्त len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ध्रुव
    {
      uint32_t h = hb_hash (len);
      अत्थाय (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned वर्ण) p[i];
      पटिनिवत्त h;
    }
  };

  संरचना svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  संरचना svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    ध्रुव वर्ण *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  तार्किक has_data () ध्रुव { पटिनिवत्त svgDocEntries; }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ध्रुव
    {
      पटिनिवत्त table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_गणनम् () ध्रुव
    { पटिनिवत्त table->get_document_गणनम् (); }

    तार्किक get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ध्रुव
    { पटिनिवत्त table->get_glyph_document_index (glyph_id, index); }

    तार्किक get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) ध्रुव
    { पटिनिवत्त table->get_document_glyph_range (index, start_glyph, end_glyph); }

    तार्किक has_data () ध्रुव { पटिनिवत्त table->has_data (); }

    ध्रुव svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ध्रुव वर्ण *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) ध्रुव;

    ध्रुव वर्ण *
    doc_cache_get_svg (ध्रुव svg_doc_cache_t *doc,
                       unsigned *len) ध्रुव;

    ध्रुव hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (ध्रुव svg_doc_cache_t *doc) ध्रुव;

    तार्किक
    doc_cache_get_glyph_span (ध्रुव svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) ध्रुव;

    तार्किक
    doc_cache_find_id_span (ध्रुव svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) ध्रुव;

    तार्किक
    doc_cache_find_id_cstr (ध्रुव svg_doc_cache_t *doc,
                            ध्रुव वर्ण *id,
                            unsigned *start,
                            unsigned *end) ध्रुव;

    तार्किक paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, शून्य *data) ध्रुव
    {
      सचे (!has_data ())
        पटिनिवत्त मिच्छा;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      सचे (blob == hb_blob_get_empty ())
        पटिनिवत्त मिच्छा;

      तार्किक ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      पटिनिवत्त ret;
    }

    गुप्त:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ध्रुव वर्ण *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) ध्रुव;

    स्थिर शून्य destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    सार्वजनिक:
    DEFINE_SIZE_STATIC (परिमाण (hb_blob_ptr_t<SVG>) +
                        परिमाण (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ध्रुव SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) ध्रुव
  { पटिनिवत्त (एतत्+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_गणनम् () ध्रुव
  {
    सचे (!has_data ())
      पटिनिवत्त 0;
    पटिनिवत्त (एतत् + svgDocEntries).len;
  }

  तार्किक get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ध्रुव
  {
    सचे (!has_data ())
      पटिनिवत्त मिच्छा;
    पटिनिवत्त (एतत् + svgDocEntries).bfind (glyph_id, index);
  }

  तार्किक get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) ध्रुव
  {
    सचे (!has_data ())
      पटिनिवत्त मिच्छा;

    ध्रुव auto &entries = एतत् + svgDocEntries;
    सचे (index >= entries.len)
      पटिनिवत्त मिच्छा;

    ध्रुव auto &entry = entries.arrayZ[index];
    सचे (start_glyph) *start_glyph = entry.get_start_glyph ();
    सचे (end_glyph) *end_glyph = entry.get_end_glyph ();
    पटिनिवत्त सच्च;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  (एतत्+svgDocEntries).sanitize_shallow (c)));
  }

  रक्षित:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (10);
};

नामाकाश _hb_svg_cache_impl {

संरचना glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

संरचना id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

संरचना open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  तार्किक in_defs_content;
  तार्किक is_defs;
};

स्थिर ध्रुव unsigned MAX_DEPTH = 128;

स्थिर अन्तःस्थ पूर्णाङ्क
find_substr (ध्रुव वर्ण *s,
             unsigned n,
             unsigned from,
             ध्रुव वर्ण *needle,
             unsigned needle_len)
{
  सचे (!needle_len || from >= n || needle_len > n)
    पटिनिवत्त -1;
  अत्थाय (unsigned i = from; i + needle_len <= n; i++)
    सचे (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      पटिनिवत्त (पूर्णाङ्क) i;
  पटिनिवत्त -1;
}

स्थिर अन्तःस्थ तार्किक
parse_id_in_start_tag (ध्रुव वर्ण *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  याव (p + 4 <= tag_end)
  {
    सचे (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      याव (e < tag_end && svg[e] != '"') e++;
      सचे (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        पटिनिवत्त सच्च;
      }
    }
    सचे (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      याव (e < tag_end && svg[e] != '\'') e++;
      सचे (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        पटिनिवत्त सच्च;
      }
    }
    p++;
  }
  पटिनिवत्त मिच्छा;
}

स्थिर अन्तःस्थ तार्किक
parse_glyph_id_span (ध्रुव SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  सचे (id.len <= 5 || memcmp (id.p, "glyph", 5))
    पटिनिवत्त मिच्छा;

  hb_codepoint_t gid = 0;
  अत्थाय (unsigned i = 5; i < id.len; i++)
  {
    unsigned वर्ण c = (unsigned वर्ण) id.p[i];
    सचे (c < '0' || c > '9')
      पटिनिवत्त मिच्छा;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    सचे (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      पटिनिवत्त मिच्छा;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  पटिनिवत्त सच्च;
}

स्थिर अन्तःस्थ तार्किक
parse_cache_entries_linear (ध्रुव वर्ण *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t राशिः[MAX_DEPTH] = {};
  unsigned depth = 0;
  सचे (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    पटिनिवत्त मिच्छा;

  unsigned defs_depth = 0;
  unsigned i = 0;
  याव (i < len)
  {
    सचे (svg[i] != '<')
    {
      i++;
      अनुवर्त;
    }

    सचे (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      पूर्णाङ्क cend = find_substr (svg, len, i + 4, "-->", 3);
      सचे (cend < 0) पटिनिवत्त मिच्छा;
      i = (unsigned) cend + 3;
      अनुवर्त;
    }
    सचे (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      पूर्णाङ्क cend = find_substr (svg, len, i + 9, "]]>", 3);
      सचे (cend < 0) पटिनिवत्त मिच्छा;
      i = (unsigned) cend + 3;
      अनुवर्त;
    }

    तार्किक closing = (i + 1 < len && svg[i + 1] == '/');
    तार्किक special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    वर्ण quote = 0;
    याव (gt < len)
    {
      वर्ण c = svg[gt];
      सचे (quote)
      {
        सचे (c == quote) quote = 0;
      }
      अञ्ञथा
      {
        सचे (c == '"' || c == '\'')
          quote = c;
        अञ्ञथा सचे (c == '>')
          भङ्ग;
      }
      gt++;
    }
    सचे (gt >= len)
      पटिनिवत्त मिच्छा;

    सचे (special)
    {
      i = gt + 1;
      अनुवर्त;
    }

    unsigned p = i + (closing ? 2 : 1);
    याव (p < gt && isspace ((unsigned वर्ण) svg[p])) p++;
    ध्रुव वर्ण *name = svg + p;
    unsigned name_len = 0;
    याव (p + name_len < gt)
    {
      unsigned वर्ण c = (unsigned वर्ण) name[name_len];
      सचे (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        भङ्ग;
      name_len++;
    }
    तार्किक is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    सचे (closing)
    {
      सचे (!depth)
      {
        i = gt + 1;
        अनुवर्त;
      }

      open_elem_t e = राशिः[--depth];
      unsigned end = gt + 1;

      सचे (e.id.len)
      {
        सचे (unlikely (!id_entries->प्रक्षिप_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          पटिनिवत्त मिच्छा;

        सचे (e.in_defs_content)
        {
          सचे (unlikely (!defs_entries->प्रक्षिप_or_fail ()))
            पटिनिवत्त मिच्छा;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        सचे (parse_glyph_id_span (e.id, &gid))
        {
          सचे (unlikely (!glyph_spans->प्रक्षिप_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            पटिनिवत्त मिच्छा;
        }
      }

      सचे (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      अनुवर्त;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    याव (r > i && isspace ((unsigned वर्ण) svg[r - 1])) r--;
    तार्किक self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    सचे (self_closing)
    {
      unsigned end = gt + 1;
      सचे (e.id.len)
      {
        सचे (unlikely (!id_entries->प्रक्षिप_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          पटिनिवत्त मिच्छा;

        सचे (e.in_defs_content)
        {
          सचे (unlikely (!defs_entries->प्रक्षिप_or_fail ()))
            पटिनिवत्त मिच्छा;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        सचे (parse_glyph_id_span (e.id, &gid))
        {
          सचे (unlikely (!glyph_spans->प्रक्षिप_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            पटिनिवत्त मिच्छा;
        }
      }
    }
    अञ्ञथा
    {
      सचे (unlikely (depth >= MAX_DEPTH))
        पटिनिवत्त मिच्छा;
      राशिः[depth++] = e;
      सचे (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  पटिनिवत्त सच्च;
}

} /* namespace _hb_svg_cache_impl */

अन्तःस्थ
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_गणनम् = table->get_document_गणनम् ();
  सचे (doc_गणनम् && unlikely (!doc_caches.resize (doc_गणनम्)))
    doc_caches.clear ();
  अत्थाय (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

अन्तःस्थ
SVG::accelerator_t::~accelerator_t ()
{
  अत्थाय (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

अन्तःस्थ शून्य
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  सचे (!doc)
    पटिनिवत्त;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

अन्तःस्थ SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ध्रुव वर्ण *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) ध्रुव
{
  स्थिर ध्रुव uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (परिमाण (svg_doc_cache_t));
  सचे (!doc)
    पटिनिवत्त nullptr;

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

  सचे (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    पटिनिवत्त nullptr;
  }

  unsigned glyph_गणनम् = end_glyph - start_glyph + 1;
  सचे (!doc->glyph_spans.resize ((पूर्णाङ्क) glyph_गणनम्))
  {
    destroy_doc_cache (doc);
    पटिनिवत्त nullptr;
  }
  अत्थाय (unsigned i = 0; i < glyph_गणनम्; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  सचे (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    पटिनिवत्त nullptr;
  }

  अत्थाय (unsigned i = 0; i < glyph_spans.length; i++)
  {
    ध्रुव auto &span = glyph_spans.arrayZ[i];
    सचे (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      अनुवर्त;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  अत्थाय (unsigned i = 0; i < id_entries.length; i++)
  {
    ध्रुव auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    सचे (doc->id_spans.has (e.id, &out))
      अनुवर्त;
    सचे (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      पटिनिवत्त nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  पटिनिवत्त doc;
}

अन्तःस्थ ध्रुव SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ध्रुव वर्ण *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) ध्रुव
{
  सचे (doc_index >= doc_caches.length)
    पटिनिवत्त nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  सचे (doc)
    पटिनिवत्त doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  सचे (!fresh)
    पटिनिवत्त nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  सचे (slot.cmpexch (expected, fresh))
    पटिनिवत्त fresh;

  destroy_doc_cache (fresh);
  पटिनिवत्त expected;
}

अन्तःस्थ ध्रुव वर्ण *
SVG::accelerator_t::doc_cache_get_svg (ध्रुव svg_doc_cache_t *doc,
                                       unsigned *len) ध्रुव
{
  सचे (!doc)
  {
    सचे (len) *len = 0;
    पटिनिवत्त nullptr;
  }
  सचे (len) *len = doc->len;
  पटिनिवत्त doc->svg;
}

अन्तःस्थ ध्रुव hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ध्रुव svg_doc_cache_t *doc) ध्रुव
{
  पटिनिवत्त doc ? &doc->defs_entries : nullptr;
}

अन्तःस्थ तार्किक
SVG::accelerator_t::doc_cache_get_glyph_span (ध्रुव svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) ध्रुव
{
  स्थिर ध्रुव uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  सचे (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    पटिनिवत्त मिच्छा;

  ध्रुव auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  सचे (span.first == INVALID_SPAN)
    पटिनिवत्त मिच्छा;
  सचे (unlikely (span.first > span.second || span.second > doc->len))
    पटिनिवत्त मिच्छा;

  सचे (start) *start = span.first;
  सचे (end) *end = span.second;
  पटिनिवत्त सच्च;
}

अन्तःस्थ तार्किक
SVG::accelerator_t::doc_cache_find_id_span (ध्रुव svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) ध्रुव
{
  सचे (!doc || !id.p || !id.len)
    पटिनिवत्त मिच्छा;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  सचे (!doc->id_spans.has (id, &span))
    पटिनिवत्त मिच्छा;
  सचे (unlikely (span->first > span->second || span->second > doc->len))
    पटिनिवत्त मिच्छा;
  सचे (start) *start = span->first;
  सचे (end) *end = span->second;
  पटिनिवत्त सच्च;
}

अन्तःस्थ तार्किक
SVG::accelerator_t::doc_cache_find_id_cstr (ध्रुव svg_doc_cache_t *doc,
                                            ध्रुव वर्ण *id,
                                            unsigned *start,
                                            unsigned *end) ध्रुव
{
  सचे (!id) पटिनिवत्त मिच्छा;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  पटिनिवत्त doc_cache_find_id_span (doc, key, start, end);
}

संरचना SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
