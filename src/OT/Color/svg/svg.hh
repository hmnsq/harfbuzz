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

#അന്തര്ഭാവ "../../../hb-open-type.hh"
#അന്തര്ഭാവ "../../../hb-blob.hh"
#അന്തര്ഭാവ "../../../hb-limits.hh"
#അന്തര്ഭാവ "../../../hb-map.hh"
#അന്തര്ഭാവ "../../../hb-paint.hh"
#അന്തര്ഭാവ "../../../hb-zlib.hh"
#അന്തര്ഭാവ <ctype.h>
#അന്തര്ഭാവ <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


നാമാകാശ OT {

സ്ഥിതം വരിയിൽ hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     സ്ഥിരം അക്ഷരം **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  സ്ഥിരം അക്ഷരം *data = hb_blob_get_data (blob, &data_len);

  എങ്കിൽ (!data || !data_len)
    പോകുക fail;

  എങ്കിൽ (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    എങ്കിൽ (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      പോകുക fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    എങ്കിൽ (!uncompressed)
      പോകുക fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    എങ്കിൽ (!data || !data_len)
      പോകുക fail;
  }

  എങ്കിൽ (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    പോകുക fail;

  എങ്കിൽ (svg) *svg = data;
  എങ്കിൽ (len) *len = data_len;
  തിരികെ blob;

fail:
  hb_blob_destroy (blob);
  എങ്കിൽ (svg) *svg = nullptr;
  എങ്കിൽ (len) *len = 0;
  തിരികെ nullptr;
}

ഘടന SVGDocumentIndexEntry
{
  പൂർണ്ണസംഖ്യ cmp (hb_codepoint_t g) സ്ഥിരം
  { തിരികെ g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () സ്ഥിരം
  { തിരികെ startGlyphID; }

  hb_codepoint_t get_end_glyph () സ്ഥിരം
  { തിരികെ endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned പൂർണ്ണസംഖ്യ index_offset) സ്ഥിരം
  {
    തിരികെ hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned പൂർണ്ണസംഖ്യ) svgDoc,
				    svgDocLength);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  സംരക്ഷിത:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  പൊതു:
  DEFINE_SIZE_STATIC (12);
};

ഘടന SVG
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  ഘടന svg_id_span_t
  {
    സ്ഥിരം അക്ഷരം *p;
    unsigned len;

    തർക്കം operator == (സ്ഥിരം svg_id_span_t &o) സ്ഥിരം
    {
      തിരികെ len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () സ്ഥിരം
    {
      uint32_t h = hb_hash (len);
      വേണ്ടി (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned അക്ഷരം) p[i];
      തിരികെ h;
    }
  };

  ഘടന svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  ഘടന svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    സ്ഥിരം അക്ഷരം *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  തർക്കം has_data () സ്ഥിരം { തിരികെ svgDocEntries; }

  ഘടന accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) സ്ഥിരം
    {
      തിരികെ table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_എണ്ണം () സ്ഥിരം
    { തിരികെ table->get_document_എണ്ണം (); }

    തർക്കം get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) സ്ഥിരം
    { തിരികെ table->get_glyph_document_index (glyph_id, index); }

    തർക്കം get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) സ്ഥിരം
    { തിരികെ table->get_document_glyph_range (index, start_glyph, end_glyph); }

    തർക്കം has_data () സ്ഥിരം { തിരികെ table->has_data (); }

    സ്ഥിരം svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             സ്ഥിരം അക്ഷരം *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) സ്ഥിരം;

    സ്ഥിരം അക്ഷരം *
    doc_cache_get_svg (സ്ഥിരം svg_doc_cache_t *doc,
                       unsigned *len) സ്ഥിരം;

    സ്ഥിരം hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (സ്ഥിരം svg_doc_cache_t *doc) സ്ഥിരം;

    തർക്കം
    doc_cache_get_glyph_span (സ്ഥിരം svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) സ്ഥിരം;

    തർക്കം
    doc_cache_find_id_span (സ്ഥിരം svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) സ്ഥിരം;

    തർക്കം
    doc_cache_find_id_cstr (സ്ഥിരം svg_doc_cache_t *doc,
                            സ്ഥിരം അക്ഷരം *id,
                            unsigned *start,
                            unsigned *end) സ്ഥിരം;

    തർക്കം paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ശൂന്യം *data) സ്ഥിരം
    {
      എങ്കിൽ (!has_data ())
        തിരികെ അസത്യം;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      എങ്കിൽ (blob == hb_blob_get_empty ())
        തിരികെ അസത്യം;

      തർക്കം ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      തിരികെ ret;
    }

    സ്വകാര്യ:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    സ്ഥിരം അക്ഷരം *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) സ്ഥിരം;

    സ്ഥിതം ശൂന്യം destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    പൊതു:
    DEFINE_SIZE_STATIC (വലുപ്പം (hb_blob_ptr_t<SVG>) +
                        വലുപ്പം (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  സ്ഥിരം SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) സ്ഥിരം
  { തിരികെ (ഇത്+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_എണ്ണം () സ്ഥിരം
  {
    എങ്കിൽ (!has_data ())
      തിരികെ 0;
    തിരികെ (ഇത് + svgDocEntries).len;
  }

  തർക്കം get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) സ്ഥിരം
  {
    എങ്കിൽ (!has_data ())
      തിരികെ അസത്യം;
    തിരികെ (ഇത് + svgDocEntries).bfind (glyph_id, index);
  }

  തർക്കം get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) സ്ഥിരം
  {
    എങ്കിൽ (!has_data ())
      തിരികെ അസത്യം;

    സ്ഥിരം auto &entries = ഇത് + svgDocEntries;
    എങ്കിൽ (index >= entries.len)
      തിരികെ അസത്യം;

    സ്ഥിരം auto &entry = entries.arrayZ[index];
    എങ്കിൽ (start_glyph) *start_glyph = entry.get_start_glyph ();
    എങ്കിൽ (end_glyph) *end_glyph = entry.get_end_glyph ();
    തിരികെ സത്യം;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  (ഇത്+svgDocEntries).sanitize_shallow (c)));
  }

  സംരക്ഷിത:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  പൊതു:
  DEFINE_SIZE_STATIC (10);
};

നാമാകാശ _hb_svg_cache_impl {

ഘടന glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

ഘടന id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

ഘടന open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  തർക്കം in_defs_content;
  തർക്കം is_defs;
};

സ്ഥിതം സ്ഥിരം unsigned MAX_DEPTH = 128;

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
find_substr (സ്ഥിരം അക്ഷരം *s,
             unsigned n,
             unsigned from,
             സ്ഥിരം അക്ഷരം *needle,
             unsigned needle_len)
{
  എങ്കിൽ (!needle_len || from >= n || needle_len > n)
    തിരികെ -1;
  വേണ്ടി (unsigned i = from; i + needle_len <= n; i++)
    എങ്കിൽ (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      തിരികെ (പൂർണ്ണസംഖ്യ) i;
  തിരികെ -1;
}

സ്ഥിതം വരിയിൽ തർക്കം
parse_id_in_start_tag (സ്ഥിരം അക്ഷരം *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  ആകുമ്പോൾ (p + 4 <= tag_end)
  {
    എങ്കിൽ (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ആകുമ്പോൾ (e < tag_end && svg[e] != '"') e++;
      എങ്കിൽ (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        തിരികെ സത്യം;
      }
    }
    എങ്കിൽ (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ആകുമ്പോൾ (e < tag_end && svg[e] != '\'') e++;
      എങ്കിൽ (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        തിരികെ സത്യം;
      }
    }
    p++;
  }
  തിരികെ അസത്യം;
}

സ്ഥിതം വരിയിൽ തർക്കം
parse_glyph_id_span (സ്ഥിരം SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  എങ്കിൽ (id.len <= 5 || memcmp (id.p, "glyph", 5))
    തിരികെ അസത്യം;

  hb_codepoint_t gid = 0;
  വേണ്ടി (unsigned i = 5; i < id.len; i++)
  {
    unsigned അക്ഷരം c = (unsigned അക്ഷരം) id.p[i];
    എങ്കിൽ (c < '0' || c > '9')
      തിരികെ അസത്യം;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    എങ്കിൽ (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      തിരികെ അസത്യം;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  തിരികെ സത്യം;
}

സ്ഥിതം വരിയിൽ തർക്കം
parse_cache_entries_linear (സ്ഥിരം അക്ഷരം *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t അട്ടി[MAX_DEPTH] = {};
  unsigned depth = 0;
  എങ്കിൽ (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    തിരികെ അസത്യം;

  unsigned defs_depth = 0;
  unsigned i = 0;
  ആകുമ്പോൾ (i < len)
  {
    എങ്കിൽ (svg[i] != '<')
    {
      i++;
      തുടരുക;
    }

    എങ്കിൽ (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      പൂർണ്ണസംഖ്യ cend = find_substr (svg, len, i + 4, "-->", 3);
      എങ്കിൽ (cend < 0) തിരികെ അസത്യം;
      i = (unsigned) cend + 3;
      തുടരുക;
    }
    എങ്കിൽ (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      പൂർണ്ണസംഖ്യ cend = find_substr (svg, len, i + 9, "]]>", 3);
      എങ്കിൽ (cend < 0) തിരികെ അസത്യം;
      i = (unsigned) cend + 3;
      തുടരുക;
    }

    തർക്കം closing = (i + 1 < len && svg[i + 1] == '/');
    തർക്കം special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    അക്ഷരം quote = 0;
    ആകുമ്പോൾ (gt < len)
    {
      അക്ഷരം c = svg[gt];
      എങ്കിൽ (quote)
      {
        എങ്കിൽ (c == quote) quote = 0;
      }
      അല്ലെങ്കിൽ
      {
        എങ്കിൽ (c == '"' || c == '\'')
          quote = c;
        അല്ലെങ്കിൽ എങ്കിൽ (c == '>')
          നിർത്തുക;
      }
      gt++;
    }
    എങ്കിൽ (gt >= len)
      തിരികെ അസത്യം;

    എങ്കിൽ (special)
    {
      i = gt + 1;
      തുടരുക;
    }

    unsigned p = i + (closing ? 2 : 1);
    ആകുമ്പോൾ (p < gt && isspace ((unsigned അക്ഷരം) svg[p])) p++;
    സ്ഥിരം അക്ഷരം *name = svg + p;
    unsigned name_len = 0;
    ആകുമ്പോൾ (p + name_len < gt)
    {
      unsigned അക്ഷരം c = (unsigned അക്ഷരം) name[name_len];
      എങ്കിൽ (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        നിർത്തുക;
      name_len++;
    }
    തർക്കം is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    എങ്കിൽ (closing)
    {
      എങ്കിൽ (!depth)
      {
        i = gt + 1;
        തുടരുക;
      }

      open_elem_t e = അട്ടി[--depth];
      unsigned end = gt + 1;

      എങ്കിൽ (e.id.len)
      {
        എങ്കിൽ (unlikely (!id_entries->തള്ളുക_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          തിരികെ അസത്യം;

        എങ്കിൽ (e.in_defs_content)
        {
          എങ്കിൽ (unlikely (!defs_entries->തള്ളുക_or_fail ()))
            തിരികെ അസത്യം;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        എങ്കിൽ (parse_glyph_id_span (e.id, &gid))
        {
          എങ്കിൽ (unlikely (!glyph_spans->തള്ളുക_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            തിരികെ അസത്യം;
        }
      }

      എങ്കിൽ (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      തുടരുക;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    ആകുമ്പോൾ (r > i && isspace ((unsigned അക്ഷരം) svg[r - 1])) r--;
    തർക്കം self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    എങ്കിൽ (self_closing)
    {
      unsigned end = gt + 1;
      എങ്കിൽ (e.id.len)
      {
        എങ്കിൽ (unlikely (!id_entries->തള്ളുക_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          തിരികെ അസത്യം;

        എങ്കിൽ (e.in_defs_content)
        {
          എങ്കിൽ (unlikely (!defs_entries->തള്ളുക_or_fail ()))
            തിരികെ അസത്യം;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        എങ്കിൽ (parse_glyph_id_span (e.id, &gid))
        {
          എങ്കിൽ (unlikely (!glyph_spans->തള്ളുക_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            തിരികെ അസത്യം;
        }
      }
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (unlikely (depth >= MAX_DEPTH))
        തിരികെ അസത്യം;
      അട്ടി[depth++] = e;
      എങ്കിൽ (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  തിരികെ സത്യം;
}

} /* namespace _hb_svg_cache_impl */

വരിയിൽ
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_എണ്ണം = table->get_document_എണ്ണം ();
  എങ്കിൽ (doc_എണ്ണം && unlikely (!doc_caches.resize (doc_എണ്ണം)))
    doc_caches.clear ();
  വേണ്ടി (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

വരിയിൽ
SVG::accelerator_t::~accelerator_t ()
{
  വേണ്ടി (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

വരിയിൽ ശൂന്യം
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  എങ്കിൽ (!doc)
    തിരികെ;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

വരിയിൽ SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    സ്ഥിരം അക്ഷരം *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) സ്ഥിരം
{
  സ്ഥിതം സ്ഥിരം uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (വലുപ്പം (svg_doc_cache_t));
  എങ്കിൽ (!doc)
    തിരികെ nullptr;

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

  എങ്കിൽ (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    തിരികെ nullptr;
  }

  unsigned glyph_എണ്ണം = end_glyph - start_glyph + 1;
  എങ്കിൽ (!doc->glyph_spans.resize ((പൂർണ്ണസംഖ്യ) glyph_എണ്ണം))
  {
    destroy_doc_cache (doc);
    തിരികെ nullptr;
  }
  വേണ്ടി (unsigned i = 0; i < glyph_എണ്ണം; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  എങ്കിൽ (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    തിരികെ nullptr;
  }

  വേണ്ടി (unsigned i = 0; i < glyph_spans.length; i++)
  {
    സ്ഥിരം auto &span = glyph_spans.arrayZ[i];
    എങ്കിൽ (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      തുടരുക;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  വേണ്ടി (unsigned i = 0; i < id_entries.length; i++)
  {
    സ്ഥിരം auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    എങ്കിൽ (doc->id_spans.has (e.id, &out))
      തുടരുക;
    എങ്കിൽ (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      തിരികെ nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  തിരികെ doc;
}

വരിയിൽ സ്ഥിരം SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              സ്ഥിരം അക്ഷരം *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) സ്ഥിരം
{
  എങ്കിൽ (doc_index >= doc_caches.length)
    തിരികെ nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  എങ്കിൽ (doc)
    തിരികെ doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  എങ്കിൽ (!fresh)
    തിരികെ nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  എങ്കിൽ (slot.cmpexch (expected, fresh))
    തിരികെ fresh;

  destroy_doc_cache (fresh);
  തിരികെ expected;
}

വരിയിൽ സ്ഥിരം അക്ഷരം *
SVG::accelerator_t::doc_cache_get_svg (സ്ഥിരം svg_doc_cache_t *doc,
                                       unsigned *len) സ്ഥിരം
{
  എങ്കിൽ (!doc)
  {
    എങ്കിൽ (len) *len = 0;
    തിരികെ nullptr;
  }
  എങ്കിൽ (len) *len = doc->len;
  തിരികെ doc->svg;
}

വരിയിൽ സ്ഥിരം hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (സ്ഥിരം svg_doc_cache_t *doc) സ്ഥിരം
{
  തിരികെ doc ? &doc->defs_entries : nullptr;
}

വരിയിൽ തർക്കം
SVG::accelerator_t::doc_cache_get_glyph_span (സ്ഥിരം svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) സ്ഥിരം
{
  സ്ഥിതം സ്ഥിരം uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  എങ്കിൽ (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    തിരികെ അസത്യം;

  സ്ഥിരം auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  എങ്കിൽ (span.first == INVALID_SPAN)
    തിരികെ അസത്യം;
  എങ്കിൽ (unlikely (span.first > span.second || span.second > doc->len))
    തിരികെ അസത്യം;

  എങ്കിൽ (start) *start = span.first;
  എങ്കിൽ (end) *end = span.second;
  തിരികെ സത്യം;
}

വരിയിൽ തർക്കം
SVG::accelerator_t::doc_cache_find_id_span (സ്ഥിരം svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) സ്ഥിരം
{
  എങ്കിൽ (!doc || !id.p || !id.len)
    തിരികെ അസത്യം;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  എങ്കിൽ (!doc->id_spans.has (id, &span))
    തിരികെ അസത്യം;
  എങ്കിൽ (unlikely (span->first > span->second || span->second > doc->len))
    തിരികെ അസത്യം;
  എങ്കിൽ (start) *start = span->first;
  എങ്കിൽ (end) *end = span->second;
  തിരികെ സത്യം;
}

വരിയിൽ തർക്കം
SVG::accelerator_t::doc_cache_find_id_cstr (സ്ഥിരം svg_doc_cache_t *doc,
                                            സ്ഥിരം അക്ഷരം *id,
                                            unsigned *start,
                                            unsigned *end) സ്ഥിരം
{
  എങ്കിൽ (!id) തിരികെ അസത്യം;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  തിരികെ doc_cache_find_id_span (doc, key, start, end);
}

ഘടന SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
