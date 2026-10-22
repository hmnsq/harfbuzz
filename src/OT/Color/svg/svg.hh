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

#અન્તર્ભાવ "../../../hb-open-type.hh"
#અન્તર્ભાવ "../../../hb-blob.hh"
#અન્તર્ભાવ "../../../hb-limits.hh"
#અન્તર્ભાવ "../../../hb-map.hh"
#અન્તર્ભાવ "../../../hb-paint.hh"
#અન્તર્ભાવ "../../../hb-zlib.hh"
#અન્તર્ભાવ <ctype.h>
#અન્તર્ભાવ <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


નામાકાશ OT {

સ્થિર અંતઃરેખ hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     અચલ અક્ષર **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  અચલ અક્ષર *data = hb_blob_get_data (blob, &data_len);

  જો (!data || !data_len)
    જાઓ fail;

  જો (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    જો (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      જાઓ fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    જો (!uncompressed)
      જાઓ fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    જો (!data || !data_len)
      જાઓ fail;
  }

  જો (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    જાઓ fail;

  જો (svg) *svg = data;
  જો (len) *len = data_len;
  પાછા blob;

fail:
  hb_blob_destroy (blob);
  જો (svg) *svg = nullptr;
  જો (len) *len = 0;
  પાછા nullptr;
}

સંરચના SVGDocumentIndexEntry
{
  પૂર્ણાંક cmp (hb_codepoint_t g) અચલ
  { પાછા g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () અચલ
  { પાછા startGlyphID; }

  hb_codepoint_t get_end_glyph () અચલ
  { પાછા endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned પૂર્ણાંક index_offset) અચલ
  {
    પાછા hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned પૂર્ણાંક) svgDoc,
				    svgDocLength);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  સુરક્ષિત:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  જાહેર:
  DEFINE_SIZE_STATIC (12);
};

સંરચના SVG
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  સંરચના svg_id_span_t
  {
    અચલ અક્ષર *p;
    unsigned len;

    તાર્કિક operator == (અચલ svg_id_span_t &o) અચલ
    {
      પાછા len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () અચલ
    {
      uint32_t h = hb_hash (len);
      માટે (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned અક્ષર) p[i];
      પાછા h;
    }
  };

  સંરચના svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  સંરચના svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    અચલ અક્ષર *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  તાર્કિક has_data () અચલ { પાછા svgDocEntries; }

  સંરચના accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) અચલ
    {
      પાછા table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_ગણતરી () અચલ
    { પાછા table->get_document_ગણતરી (); }

    તાર્કિક get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) અચલ
    { પાછા table->get_glyph_document_index (glyph_id, index); }

    તાર્કિક get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) અચલ
    { પાછા table->get_document_glyph_range (index, start_glyph, end_glyph); }

    તાર્કિક has_data () અચલ { પાછા table->has_data (); }

    અચલ svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             અચલ અક્ષર *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) અચલ;

    અચલ અક્ષર *
    doc_cache_get_svg (અચલ svg_doc_cache_t *doc,
                       unsigned *len) અચલ;

    અચલ hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (અચલ svg_doc_cache_t *doc) અચલ;

    તાર્કિક
    doc_cache_get_glyph_span (અચલ svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) અચલ;

    તાર્કિક
    doc_cache_find_id_span (અચલ svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) અચલ;

    તાર્કિક
    doc_cache_find_id_cstr (અચલ svg_doc_cache_t *doc,
                            અચલ અક્ષર *id,
                            unsigned *start,
                            unsigned *end) અચલ;

    તાર્કિક paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, શૂન્ય *data) અચલ
    {
      જો (!has_data ())
        પાછા ખોટું;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      જો (blob == hb_blob_get_empty ())
        પાછા ખોટું;

      તાર્કિક ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      પાછા ret;
    }

    ખાનગી:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    અચલ અક્ષર *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) અચલ;

    સ્થિર શૂન્ય destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    જાહેર:
    DEFINE_SIZE_STATIC (કદ (hb_blob_ptr_t<SVG>) +
                        કદ (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  અચલ SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) અચલ
  { પાછા (આ+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_ગણતરી () અચલ
  {
    જો (!has_data ())
      પાછા 0;
    પાછા (આ + svgDocEntries).len;
  }

  તાર્કિક get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) અચલ
  {
    જો (!has_data ())
      પાછા ખોટું;
    પાછા (આ + svgDocEntries).bfind (glyph_id, index);
  }

  તાર્કિક get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) અચલ
  {
    જો (!has_data ())
      પાછા ખોટું;

    અચલ auto &entries = આ + svgDocEntries;
    જો (index >= entries.len)
      પાછા ખોટું;

    અચલ auto &entry = entries.arrayZ[index];
    જો (start_glyph) *start_glyph = entry.get_start_glyph ();
    જો (end_glyph) *end_glyph = entry.get_end_glyph ();
    પાછા સાચું;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  (આ+svgDocEntries).sanitize_shallow (c)));
  }

  સુરક્ષિત:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  જાહેર:
  DEFINE_SIZE_STATIC (10);
};

નામાકાશ _hb_svg_cache_impl {

સંરચના glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

સંરચના id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

સંરચના open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  તાર્કિક in_defs_content;
  તાર્કિક is_defs;
};

સ્થિર અચલ unsigned MAX_DEPTH = 128;

સ્થિર અંતઃરેખ પૂર્ણાંક
find_substr (અચલ અક્ષર *s,
             unsigned n,
             unsigned from,
             અચલ અક્ષર *needle,
             unsigned needle_len)
{
  જો (!needle_len || from >= n || needle_len > n)
    પાછા -1;
  માટે (unsigned i = from; i + needle_len <= n; i++)
    જો (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      પાછા (પૂર્ણાંક) i;
  પાછા -1;
}

સ્થિર અંતઃરેખ તાર્કિક
parse_id_in_start_tag (અચલ અક્ષર *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  જ્યારે (p + 4 <= tag_end)
  {
    જો (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      જ્યારે (e < tag_end && svg[e] != '"') e++;
      જો (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        પાછા સાચું;
      }
    }
    જો (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      જ્યારે (e < tag_end && svg[e] != '\'') e++;
      જો (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        પાછા સાચું;
      }
    }
    p++;
  }
  પાછા ખોટું;
}

સ્થિર અંતઃરેખ તાર્કિક
parse_glyph_id_span (અચલ SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  જો (id.len <= 5 || memcmp (id.p, "glyph", 5))
    પાછા ખોટું;

  hb_codepoint_t gid = 0;
  માટે (unsigned i = 5; i < id.len; i++)
  {
    unsigned અક્ષર c = (unsigned અક્ષર) id.p[i];
    જો (c < '0' || c > '9')
      પાછા ખોટું;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    જો (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      પાછા ખોટું;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  પાછા સાચું;
}

સ્થિર અંતઃરેખ તાર્કિક
parse_cache_entries_linear (અચલ અક્ષર *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t ઢગલો[MAX_DEPTH] = {};
  unsigned depth = 0;
  જો (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    પાછા ખોટું;

  unsigned defs_depth = 0;
  unsigned i = 0;
  જ્યારે (i < len)
  {
    જો (svg[i] != '<')
    {
      i++;
      ચાલુ;
    }

    જો (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      પૂર્ણાંક cend = find_substr (svg, len, i + 4, "-->", 3);
      જો (cend < 0) પાછા ખોટું;
      i = (unsigned) cend + 3;
      ચાલુ;
    }
    જો (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      પૂર્ણાંક cend = find_substr (svg, len, i + 9, "]]>", 3);
      જો (cend < 0) પાછા ખોટું;
      i = (unsigned) cend + 3;
      ચાલુ;
    }

    તાર્કિક closing = (i + 1 < len && svg[i + 1] == '/');
    તાર્કિક special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    અક્ષર quote = 0;
    જ્યારે (gt < len)
    {
      અક્ષર c = svg[gt];
      જો (quote)
      {
        જો (c == quote) quote = 0;
      }
      નહીંતર
      {
        જો (c == '"' || c == '\'')
          quote = c;
        નહીંતર જો (c == '>')
          તોડો;
      }
      gt++;
    }
    જો (gt >= len)
      પાછા ખોટું;

    જો (special)
    {
      i = gt + 1;
      ચાલુ;
    }

    unsigned p = i + (closing ? 2 : 1);
    જ્યારે (p < gt && isspace ((unsigned અક્ષર) svg[p])) p++;
    અચલ અક્ષર *name = svg + p;
    unsigned name_len = 0;
    જ્યારે (p + name_len < gt)
    {
      unsigned અક્ષર c = (unsigned અક્ષર) name[name_len];
      જો (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        તોડો;
      name_len++;
    }
    તાર્કિક is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    જો (closing)
    {
      જો (!depth)
      {
        i = gt + 1;
        ચાલુ;
      }

      open_elem_t e = ઢગલો[--depth];
      unsigned end = gt + 1;

      જો (e.id.len)
      {
        જો (unlikely (!id_entries->ધકેલો_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          પાછા ખોટું;

        જો (e.in_defs_content)
        {
          જો (unlikely (!defs_entries->ધકેલો_or_fail ()))
            પાછા ખોટું;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        જો (parse_glyph_id_span (e.id, &gid))
        {
          જો (unlikely (!glyph_spans->ધકેલો_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            પાછા ખોટું;
        }
      }

      જો (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      ચાલુ;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    જ્યારે (r > i && isspace ((unsigned અક્ષર) svg[r - 1])) r--;
    તાર્કિક self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    જો (self_closing)
    {
      unsigned end = gt + 1;
      જો (e.id.len)
      {
        જો (unlikely (!id_entries->ધકેલો_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          પાછા ખોટું;

        જો (e.in_defs_content)
        {
          જો (unlikely (!defs_entries->ધકેલો_or_fail ()))
            પાછા ખોટું;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        જો (parse_glyph_id_span (e.id, &gid))
        {
          જો (unlikely (!glyph_spans->ધકેલો_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            પાછા ખોટું;
        }
      }
    }
    નહીંતર
    {
      જો (unlikely (depth >= MAX_DEPTH))
        પાછા ખોટું;
      ઢગલો[depth++] = e;
      જો (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  પાછા સાચું;
}

} /* namespace _hb_svg_cache_impl */

અંતઃરેખ
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_ગણતરી = table->get_document_ગણતરી ();
  જો (doc_ગણતરી && unlikely (!doc_caches.resize (doc_ગણતરી)))
    doc_caches.clear ();
  માટે (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

અંતઃરેખ
SVG::accelerator_t::~accelerator_t ()
{
  માટે (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

અંતઃરેખ શૂન્ય
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  જો (!doc)
    પાછા;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

અંતઃરેખ SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    અચલ અક્ષર *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) અચલ
{
  સ્થિર અચલ uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (કદ (svg_doc_cache_t));
  જો (!doc)
    પાછા nullptr;

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

  જો (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    પાછા nullptr;
  }

  unsigned glyph_ગણતરી = end_glyph - start_glyph + 1;
  જો (!doc->glyph_spans.resize ((પૂર્ણાંક) glyph_ગણતરી))
  {
    destroy_doc_cache (doc);
    પાછા nullptr;
  }
  માટે (unsigned i = 0; i < glyph_ગણતરી; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  જો (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    પાછા nullptr;
  }

  માટે (unsigned i = 0; i < glyph_spans.length; i++)
  {
    અચલ auto &span = glyph_spans.arrayZ[i];
    જો (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      ચાલુ;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  માટે (unsigned i = 0; i < id_entries.length; i++)
  {
    અચલ auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    જો (doc->id_spans.has (e.id, &out))
      ચાલુ;
    જો (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      પાછા nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  પાછા doc;
}

અંતઃરેખ અચલ SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              અચલ અક્ષર *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) અચલ
{
  જો (doc_index >= doc_caches.length)
    પાછા nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  જો (doc)
    પાછા doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  જો (!fresh)
    પાછા nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  જો (slot.cmpexch (expected, fresh))
    પાછા fresh;

  destroy_doc_cache (fresh);
  પાછા expected;
}

અંતઃરેખ અચલ અક્ષર *
SVG::accelerator_t::doc_cache_get_svg (અચલ svg_doc_cache_t *doc,
                                       unsigned *len) અચલ
{
  જો (!doc)
  {
    જો (len) *len = 0;
    પાછા nullptr;
  }
  જો (len) *len = doc->len;
  પાછા doc->svg;
}

અંતઃરેખ અચલ hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (અચલ svg_doc_cache_t *doc) અચલ
{
  પાછા doc ? &doc->defs_entries : nullptr;
}

અંતઃરેખ તાર્કિક
SVG::accelerator_t::doc_cache_get_glyph_span (અચલ svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) અચલ
{
  સ્થિર અચલ uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  જો (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    પાછા ખોટું;

  અચલ auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  જો (span.first == INVALID_SPAN)
    પાછા ખોટું;
  જો (unlikely (span.first > span.second || span.second > doc->len))
    પાછા ખોટું;

  જો (start) *start = span.first;
  જો (end) *end = span.second;
  પાછા સાચું;
}

અંતઃરેખ તાર્કિક
SVG::accelerator_t::doc_cache_find_id_span (અચલ svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) અચલ
{
  જો (!doc || !id.p || !id.len)
    પાછા ખોટું;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  જો (!doc->id_spans.has (id, &span))
    પાછા ખોટું;
  જો (unlikely (span->first > span->second || span->second > doc->len))
    પાછા ખોટું;
  જો (start) *start = span->first;
  જો (end) *end = span->second;
  પાછા સાચું;
}

અંતઃરેખ તાર્કિક
SVG::accelerator_t::doc_cache_find_id_cstr (અચલ svg_doc_cache_t *doc,
                                            અચલ અક્ષર *id,
                                            unsigned *start,
                                            unsigned *end) અચલ
{
  જો (!id) પાછા ખોટું;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  પાછા doc_cache_find_id_span (doc, key, start, end);
}

સંરચના SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
