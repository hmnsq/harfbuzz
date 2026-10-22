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

#অন্তর্ভাব "../../../hb-open-type.hh"
#অন্তর্ভাব "../../../hb-blob.hh"
#অন্তর্ভাব "../../../hb-limits.hh"
#অন্তর্ভাব "../../../hb-map.hh"
#অন্তর্ভাব "../../../hb-paint.hh"
#অন্তর্ভাব "../../../hb-zlib.hh"
#অন্তর্ভাব <ctype.h>
#অন্তর্ভাব <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


নামাকাশ OT {

স্থির অন্তঃস্থ hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ধ্রুব বর্ণ **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  ধ্রুব বর্ণ *data = hb_blob_get_data (blob, &data_len);

  যদি (!data || !data_len)
    গচ্ছ fail;

  যদি (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    যদি (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      গচ্ছ fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    যদি (!uncompressed)
      গচ্ছ fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    যদি (!data || !data_len)
      গচ্ছ fail;
  }

  যদি (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    গচ্ছ fail;

  যদি (svg) *svg = data;
  যদি (len) *len = data_len;
  প্রত্যাগম blob;

fail:
  hb_blob_destroy (blob);
  যদি (svg) *svg = nullptr;
  যদি (len) *len = 0;
  প্রত্যাগম nullptr;
}

সংরচনা SVGDocumentIndexEntry
{
  পূর্ণাঙ্ক cmp (hb_codepoint_t g) ধ্রুব
  { প্রত্যাগম g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () ধ্রুব
  { প্রত্যাগম startGlyphID; }

  hb_codepoint_t get_end_glyph () ধ্রুব
  { প্রত্যাগম endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned পূর্ণাঙ্ক index_offset) ধ্রুব
  {
    প্রত্যাগম hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned পূর্ণাঙ্ক) svgDoc,
				    svgDocLength);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব শূন্য *base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  রক্ষিত:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (12);
};

সংরচনা SVG
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  সংরচনা svg_id_span_t
  {
    ধ্রুব বর্ণ *p;
    unsigned len;

    তার্কিক operator == (ধ্রুব svg_id_span_t &o) ধ্রুব
    {
      প্রত্যাগম len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ধ্রুব
    {
      uint32_t h = hb_hash (len);
      প্রতি (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned বর্ণ) p[i];
      প্রত্যাগম h;
    }
  };

  সংরচনা svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  সংরচনা svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    ধ্রুব বর্ণ *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  তার্কিক has_data () ধ্রুব { প্রত্যাগম svgDocEntries; }

  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ধ্রুব
    {
      প্রত্যাগম table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_গুনতি () ধ্রুব
    { প্রত্যাগম table->get_document_গুনতি (); }

    তার্কিক get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ধ্রুব
    { প্রত্যাগম table->get_glyph_document_index (glyph_id, index); }

    তার্কিক get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) ধ্রুব
    { প্রত্যাগম table->get_document_glyph_range (index, start_glyph, end_glyph); }

    তার্কিক has_data () ধ্রুব { প্রত্যাগম table->has_data (); }

    ধ্রুব svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ধ্রুব বর্ণ *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) ধ্রুব;

    ধ্রুব বর্ণ *
    doc_cache_get_svg (ধ্রুব svg_doc_cache_t *doc,
                       unsigned *len) ধ্রুব;

    ধ্রুব hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (ধ্রুব svg_doc_cache_t *doc) ধ্রুব;

    তার্কিক
    doc_cache_get_glyph_span (ধ্রুব svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) ধ্রুব;

    তার্কিক
    doc_cache_find_id_span (ধ্রুব svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) ধ্রুব;

    তার্কিক
    doc_cache_find_id_cstr (ধ্রুব svg_doc_cache_t *doc,
                            ধ্রুব বর্ণ *id,
                            unsigned *start,
                            unsigned *end) ধ্রুব;

    তার্কিক paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, শূন্য *data) ধ্রুব
    {
      যদি (!has_data ())
        প্রত্যাগম অসত্য;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      যদি (blob == hb_blob_get_empty ())
        প্রত্যাগম অসত্য;

      তার্কিক ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      প্রত্যাগম ret;
    }

    গুপ্ত:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ধ্রুব বর্ণ *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) ধ্রুব;

    স্থির শূন্য destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    সার্বজনিক:
    DEFINE_SIZE_STATIC (পরিমাণ (hb_blob_ptr_t<SVG>) +
                        পরিমাণ (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ধ্রুব SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) ধ্রুব
  { প্রত্যাগম (এতত্+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_গুনতি () ধ্রুব
  {
    যদি (!has_data ())
      প্রত্যাগম 0;
    প্রত্যাগম (এতত্ + svgDocEntries).len;
  }

  তার্কিক get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ধ্রুব
  {
    যদি (!has_data ())
      প্রত্যাগম অসত্য;
    প্রত্যাগম (এতত্ + svgDocEntries).bfind (glyph_id, index);
  }

  তার্কিক get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) ধ্রুব
  {
    যদি (!has_data ())
      প্রত্যাগম অসত্য;

    ধ্রুব auto &entries = এতত্ + svgDocEntries;
    যদি (index >= entries.len)
      প্রত্যাগম অসত্য;

    ধ্রুব auto &entry = entries.arrayZ[index];
    যদি (start_glyph) *start_glyph = entry.get_start_glyph ();
    যদি (end_glyph) *end_glyph = entry.get_end_glyph ();
    প্রত্যাগম সত্য;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  (এতত্+svgDocEntries).sanitize_shallow (c)));
  }

  রক্ষিত:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (10);
};

নামাকাশ _hb_svg_cache_impl {

সংরচনা glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

সংরচনা id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

সংরচনা open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  তার্কিক in_defs_content;
  তার্কিক is_defs;
};

স্থির ধ্রুব unsigned MAX_DEPTH = 128;

স্থির অন্তঃস্থ পূর্ণাঙ্ক
find_substr (ধ্রুব বর্ণ *s,
             unsigned n,
             unsigned from,
             ধ্রুব বর্ণ *needle,
             unsigned needle_len)
{
  যদি (!needle_len || from >= n || needle_len > n)
    প্রত্যাগম -1;
  প্রতি (unsigned i = from; i + needle_len <= n; i++)
    যদি (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      প্রত্যাগম (পূর্ণাঙ্ক) i;
  প্রত্যাগম -1;
}

স্থির অন্তঃস্থ তার্কিক
parse_id_in_start_tag (ধ্রুব বর্ণ *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  যাবত্ (p + 4 <= tag_end)
  {
    যদি (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      যাবত্ (e < tag_end && svg[e] != '"') e++;
      যদি (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        প্রত্যাগম সত্য;
      }
    }
    যদি (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      যাবত্ (e < tag_end && svg[e] != '\'') e++;
      যদি (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        প্রত্যাগম সত্য;
      }
    }
    p++;
  }
  প্রত্যাগম অসত্য;
}

স্থির অন্তঃস্থ তার্কিক
parse_glyph_id_span (ধ্রুব SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  যদি (id.len <= 5 || memcmp (id.p, "glyph", 5))
    প্রত্যাগম অসত্য;

  hb_codepoint_t gid = 0;
  প্রতি (unsigned i = 5; i < id.len; i++)
  {
    unsigned বর্ণ c = (unsigned বর্ণ) id.p[i];
    যদি (c < '0' || c > '9')
      প্রত্যাগম অসত্য;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    যদি (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      প্রত্যাগম অসত্য;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  প্রত্যাগম সত্য;
}

স্থির অন্তঃস্থ তার্কিক
parse_cache_entries_linear (ধ্রুব বর্ণ *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t স্তূপ[MAX_DEPTH] = {};
  unsigned depth = 0;
  যদি (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    প্রত্যাগম অসত্য;

  unsigned defs_depth = 0;
  unsigned i = 0;
  যাবত্ (i < len)
  {
    যদি (svg[i] != '<')
    {
      i++;
      অনুবর্ত;
    }

    যদি (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      পূর্ণাঙ্ক cend = find_substr (svg, len, i + 4, "-->", 3);
      যদি (cend < 0) প্রত্যাগম অসত্য;
      i = (unsigned) cend + 3;
      অনুবর্ত;
    }
    যদি (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      পূর্ণাঙ্ক cend = find_substr (svg, len, i + 9, "]]>", 3);
      যদি (cend < 0) প্রত্যাগম অসত্য;
      i = (unsigned) cend + 3;
      অনুবর্ত;
    }

    তার্কিক closing = (i + 1 < len && svg[i + 1] == '/');
    তার্কিক special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    বর্ণ quote = 0;
    যাবত্ (gt < len)
    {
      বর্ণ c = svg[gt];
      যদি (quote)
      {
        যদি (c == quote) quote = 0;
      }
      অন্যথা
      {
        যদি (c == '"' || c == '\'')
          quote = c;
        অন্যথা যদি (c == '>')
          ভঙ্গ;
      }
      gt++;
    }
    যদি (gt >= len)
      প্রত্যাগম অসত্য;

    যদি (special)
    {
      i = gt + 1;
      অনুবর্ত;
    }

    unsigned p = i + (closing ? 2 : 1);
    যাবত্ (p < gt && isspace ((unsigned বর্ণ) svg[p])) p++;
    ধ্রুব বর্ণ *name = svg + p;
    unsigned name_len = 0;
    যাবত্ (p + name_len < gt)
    {
      unsigned বর্ণ c = (unsigned বর্ণ) name[name_len];
      যদি (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        ভঙ্গ;
      name_len++;
    }
    তার্কিক is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    যদি (closing)
    {
      যদি (!depth)
      {
        i = gt + 1;
        অনুবর্ত;
      }

      open_elem_t e = স্তূপ[--depth];
      unsigned end = gt + 1;

      যদি (e.id.len)
      {
        যদি (unlikely (!id_entries->ঠেলো_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          প্রত্যাগম অসত্য;

        যদি (e.in_defs_content)
        {
          যদি (unlikely (!defs_entries->ঠেলো_or_fail ()))
            প্রত্যাগম অসত্য;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        যদি (parse_glyph_id_span (e.id, &gid))
        {
          যদি (unlikely (!glyph_spans->ঠেলো_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            প্রত্যাগম অসত্য;
        }
      }

      যদি (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      অনুবর্ত;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    যাবত্ (r > i && isspace ((unsigned বর্ণ) svg[r - 1])) r--;
    তার্কিক self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    যদি (self_closing)
    {
      unsigned end = gt + 1;
      যদি (e.id.len)
      {
        যদি (unlikely (!id_entries->ঠেলো_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          প্রত্যাগম অসত্য;

        যদি (e.in_defs_content)
        {
          যদি (unlikely (!defs_entries->ঠেলো_or_fail ()))
            প্রত্যাগম অসত্য;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        যদি (parse_glyph_id_span (e.id, &gid))
        {
          যদি (unlikely (!glyph_spans->ঠেলো_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            প্রত্যাগম অসত্য;
        }
      }
    }
    অন্যথা
    {
      যদি (unlikely (depth >= MAX_DEPTH))
        প্রত্যাগম অসত্য;
      স্তূপ[depth++] = e;
      যদি (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  প্রত্যাগম সত্য;
}

} /* namespace _hb_svg_cache_impl */

অন্তঃস্থ
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_গুনতি = table->get_document_গুনতি ();
  যদি (doc_গুনতি && unlikely (!doc_caches.resize (doc_গুনতি)))
    doc_caches.clear ();
  প্রতি (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

অন্তঃস্থ
SVG::accelerator_t::~accelerator_t ()
{
  প্রতি (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

অন্তঃস্থ শূন্য
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  যদি (!doc)
    প্রত্যাগম;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

অন্তঃস্থ SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ধ্রুব বর্ণ *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) ধ্রুব
{
  স্থির ধ্রুব uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (পরিমাণ (svg_doc_cache_t));
  যদি (!doc)
    প্রত্যাগম nullptr;

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

  যদি (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    প্রত্যাগম nullptr;
  }

  unsigned glyph_গুনতি = end_glyph - start_glyph + 1;
  যদি (!doc->glyph_spans.resize ((পূর্ণাঙ্ক) glyph_গুনতি))
  {
    destroy_doc_cache (doc);
    প্রত্যাগম nullptr;
  }
  প্রতি (unsigned i = 0; i < glyph_গুনতি; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  যদি (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    প্রত্যাগম nullptr;
  }

  প্রতি (unsigned i = 0; i < glyph_spans.length; i++)
  {
    ধ্রুব auto &span = glyph_spans.arrayZ[i];
    যদি (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      অনুবর্ত;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  প্রতি (unsigned i = 0; i < id_entries.length; i++)
  {
    ধ্রুব auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    যদি (doc->id_spans.has (e.id, &out))
      অনুবর্ত;
    যদি (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      প্রত্যাগম nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  প্রত্যাগম doc;
}

অন্তঃস্থ ধ্রুব SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ধ্রুব বর্ণ *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) ধ্রুব
{
  যদি (doc_index >= doc_caches.length)
    প্রত্যাগম nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  যদি (doc)
    প্রত্যাগম doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  যদি (!fresh)
    প্রত্যাগম nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  যদি (slot.cmpexch (expected, fresh))
    প্রত্যাগম fresh;

  destroy_doc_cache (fresh);
  প্রত্যাগম expected;
}

অন্তঃস্থ ধ্রুব বর্ণ *
SVG::accelerator_t::doc_cache_get_svg (ধ্রুব svg_doc_cache_t *doc,
                                       unsigned *len) ধ্রুব
{
  যদি (!doc)
  {
    যদি (len) *len = 0;
    প্রত্যাগম nullptr;
  }
  যদি (len) *len = doc->len;
  প্রত্যাগম doc->svg;
}

অন্তঃস্থ ধ্রুব hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ধ্রুব svg_doc_cache_t *doc) ধ্রুব
{
  প্রত্যাগম doc ? &doc->defs_entries : nullptr;
}

অন্তঃস্থ তার্কিক
SVG::accelerator_t::doc_cache_get_glyph_span (ধ্রুব svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) ধ্রুব
{
  স্থির ধ্রুব uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  যদি (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    প্রত্যাগম অসত্য;

  ধ্রুব auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  যদি (span.first == INVALID_SPAN)
    প্রত্যাগম অসত্য;
  যদি (unlikely (span.first > span.second || span.second > doc->len))
    প্রত্যাগম অসত্য;

  যদি (start) *start = span.first;
  যদি (end) *end = span.second;
  প্রত্যাগম সত্য;
}

অন্তঃস্থ তার্কিক
SVG::accelerator_t::doc_cache_find_id_span (ধ্রুব svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) ধ্রুব
{
  যদি (!doc || !id.p || !id.len)
    প্রত্যাগম অসত্য;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  যদি (!doc->id_spans.has (id, &span))
    প্রত্যাগম অসত্য;
  যদি (unlikely (span->first > span->second || span->second > doc->len))
    প্রত্যাগম অসত্য;
  যদি (start) *start = span->first;
  যদি (end) *end = span->second;
  প্রত্যাগম সত্য;
}

অন্তঃস্থ তার্কিক
SVG::accelerator_t::doc_cache_find_id_cstr (ধ্রুব svg_doc_cache_t *doc,
                                            ধ্রুব বর্ণ *id,
                                            unsigned *start,
                                            unsigned *end) ধ্রুব
{
  যদি (!id) প্রত্যাগম অসত্য;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  প্রত্যাগম doc_cache_find_id_span (doc, key, start, end);
}

সংরচনা SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
