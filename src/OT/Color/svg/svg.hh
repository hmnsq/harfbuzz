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

#include "../../../hb-open-type.hh"
#include "../../../hb-blob.hh"
#include "../../../hb-limits.hh"
#include "../../../hb-map.hh"
#include "../../../hb-paint.hh"
#include "../../../hb-zlib.hh"
#include <ctype.h>
#include <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


namespace OT {

static anta_hstha hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     const var_na **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  const var_na *data = hb_blob_get_data (blob, &data_len);

  ꦪꦼꦤ꧀ (!data || !data_len)
    goto fail;

  ꦪꦼꦤ꧀ (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    ꦪꦼꦤ꧀ (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      goto fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    ꦪꦼꦤ꧀ (!uncompressed)
      goto fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    ꦪꦼꦤ꧀ (!data || !data_len)
      goto fail;
  }

  ꦪꦼꦤ꧀ (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    goto fail;

  ꦪꦼꦤ꧀ (svg) *svg = data;
  ꦪꦼꦤ꧀ (len) *len = data_len;
  ꦮꦭꦶ blob;

fail:
  hb_blob_destroy (blob);
  ꦪꦼꦤ꧀ (svg) *svg = nullptr;
  ꦪꦼꦤ꧀ (len) *len = 0;
  ꦮꦭꦶ nullptr;
}

sa_nracanaa SVGDocumentIndexEntry
{
  puur_naa_nka cmp (hb_codepoint_t g) const
  { ꦮꦭꦶ g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () const
  { ꦮꦭꦶ startGlyphID; }

  hb_codepoint_t get_end_glyph () const
  { ꦮꦭꦶ endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned puur_naa_nka index_offset) const
  {
    ꦮꦭꦶ hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned puur_naa_nka) svgDoc,
				    svgDocLength);
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  rak_sita:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  public:
  DEFINE_SIZE_STATIC (12);
};

sa_nracanaa SVG
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  sa_nracanaa svg_id_span_t
  {
    const var_na *p;
    unsigned len;

    bool operator == (const svg_id_span_t &o) const
    {
      ꦮꦭꦶ len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () const
    {
      uint32_t h = hb_hash (len);
      ꦏꦁꦒꦺꦴ (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned var_na) p[i];
      ꦮꦭꦶ h;
    }
  };

  sa_nracanaa svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  sa_nracanaa svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    const var_na *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  bool has_data () const { ꦮꦭꦶ svgDocEntries; }

  sa_nracanaa accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) const
    {
      ꦮꦭꦶ table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_count () const
    { ꦮꦭꦶ table->get_document_count (); }

    bool get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) const
    { ꦮꦭꦶ table->get_glyph_document_index (glyph_id, index); }

    bool get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) const
    { ꦮꦭꦶ table->get_document_glyph_range (index, start_glyph, end_glyph); }

    bool has_data () const { ꦮꦭꦶ table->has_data (); }

    const svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             const var_na *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) const;

    const var_na *
    doc_cache_get_svg (const svg_doc_cache_t *doc,
                       unsigned *len) const;

    const hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (const svg_doc_cache_t *doc) const;

    bool
    doc_cache_get_glyph_span (const svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) const;

    bool
    doc_cache_find_id_span (const svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) const;

    bool
    doc_cache_find_id_cstr (const svg_doc_cache_t *doc,
                            const var_na *id,
                            unsigned *start,
                            unsigned *end) const;

    bool paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, void *data) const
    {
      ꦪꦼꦤ꧀ (!has_data ())
        ꦮꦭꦶ false;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      ꦪꦼꦤ꧀ (blob == hb_blob_get_empty ())
        ꦮꦭꦶ false;

      bool ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      ꦮꦭꦶ ret;
    }

    private:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    const var_na *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) const;

    static void destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    public:
    DEFINE_SIZE_STATIC (parimaa_na (hb_blob_ptr_t<SVG>) +
                        parimaa_na (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  const SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) const
  { ꦮꦭꦶ (this+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_count () const
  {
    ꦪꦼꦤ꧀ (!has_data ())
      ꦮꦭꦶ 0;
    ꦮꦭꦶ (this + svgDocEntries).len;
  }

  bool get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) const
  {
    ꦪꦼꦤ꧀ (!has_data ())
      ꦮꦭꦶ false;
    ꦮꦭꦶ (this + svgDocEntries).bfind (glyph_id, index);
  }

  bool get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) const
  {
    ꦪꦼꦤ꧀ (!has_data ())
      ꦮꦭꦶ false;

    const auto &entries = this + svgDocEntries;
    ꦪꦼꦤ꧀ (index >= entries.len)
      ꦮꦭꦶ false;

    const auto &entry = entries.arrayZ[index];
    ꦪꦼꦤ꧀ (start_glyph) *start_glyph = entry.get_start_glyph ();
    ꦪꦼꦤ꧀ (end_glyph) *end_glyph = entry.get_end_glyph ();
    ꦮꦭꦶ true;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  (this+svgDocEntries).sanitize_shallow (c)));
  }

  rak_sita:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  public:
  DEFINE_SIZE_STATIC (10);
};

namespace _hb_svg_cache_impl {

sa_nracanaa glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

sa_nracanaa id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

sa_nracanaa open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  bool in_defs_content;
  bool is_defs;
};

static const unsigned MAX_DEPTH = 128;

static anta_hstha puur_naa_nka
find_substr (const var_na *s,
             unsigned n,
             unsigned from,
             const var_na *needle,
             unsigned needle_len)
{
  ꦪꦼꦤ꧀ (!needle_len || from >= n || needle_len > n)
    ꦮꦭꦶ -1;
  ꦏꦁꦒꦺꦴ (unsigned i = from; i + needle_len <= n; i++)
    ꦪꦼꦤ꧀ (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      ꦮꦭꦶ (puur_naa_nka) i;
  ꦮꦭꦶ -1;
}

static anta_hstha bool
parse_id_in_start_tag (const var_na *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  ꦱꦼꦢꦼꦭ (p + 4 <= tag_end)
  {
    ꦪꦼꦤ꧀ (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ꦱꦼꦢꦼꦭ (e < tag_end && svg[e] != '"') e++;
      ꦪꦼꦤ꧀ (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ꦮꦭꦶ true;
      }
    }
    ꦪꦼꦤ꧀ (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ꦱꦼꦢꦼꦭ (e < tag_end && svg[e] != '\'') e++;
      ꦪꦼꦤ꧀ (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ꦮꦭꦶ true;
      }
    }
    p++;
  }
  ꦮꦭꦶ false;
}

static anta_hstha bool
parse_glyph_id_span (const SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  ꦪꦼꦤ꧀ (id.len <= 5 || memcmp (id.p, "glyph", 5))
    ꦮꦭꦶ false;

  hb_codepoint_t gid = 0;
  ꦏꦁꦒꦺꦴ (unsigned i = 5; i < id.len; i++)
  {
    unsigned var_na c = (unsigned var_na) id.p[i];
    ꦪꦼꦤ꧀ (c < '0' || c > '9')
      ꦮꦭꦶ false;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    ꦪꦼꦤ꧀ (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      ꦮꦭꦶ false;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  ꦮꦭꦶ true;
}

static anta_hstha bool
parse_cache_entries_linear (const var_na *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t stack[MAX_DEPTH] = {};
  unsigned depth = 0;
  ꦪꦼꦤ꧀ (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    ꦮꦭꦶ false;

  unsigned defs_depth = 0;
  unsigned i = 0;
  ꦱꦼꦢꦼꦭ (i < len)
  {
    ꦪꦼꦤ꧀ (svg[i] != '<')
    {
      i++;
      continue;
    }

    ꦪꦼꦤ꧀ (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      puur_naa_nka cend = find_substr (svg, len, i + 4, "-->", 3);
      ꦪꦼꦤ꧀ (cend < 0) ꦮꦭꦶ false;
      i = (unsigned) cend + 3;
      continue;
    }
    ꦪꦼꦤ꧀ (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      puur_naa_nka cend = find_substr (svg, len, i + 9, "]]>", 3);
      ꦪꦼꦤ꧀ (cend < 0) ꦮꦭꦶ false;
      i = (unsigned) cend + 3;
      continue;
    }

    bool closing = (i + 1 < len && svg[i + 1] == '/');
    bool special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    var_na quote = 0;
    ꦱꦼꦢꦼꦭ (gt < len)
    {
      var_na c = svg[gt];
      ꦪꦼꦤ꧀ (quote)
      {
        ꦪꦼꦤ꧀ (c == quote) quote = 0;
      }
      ꦭꦶꦪ
      {
        ꦪꦼꦤ꧀ (c == '"' || c == '\'')
          quote = c;
        ꦭꦶꦪ ꦪꦼꦤ꧀ (c == '>')
          bha_nga;
      }
      gt++;
    }
    ꦪꦼꦤ꧀ (gt >= len)
      ꦮꦭꦶ false;

    ꦪꦼꦤ꧀ (special)
    {
      i = gt + 1;
      continue;
    }

    unsigned p = i + (closing ? 2 : 1);
    ꦱꦼꦢꦼꦭ (p < gt && isspace ((unsigned var_na) svg[p])) p++;
    const var_na *name = svg + p;
    unsigned name_len = 0;
    ꦱꦼꦢꦼꦭ (p + name_len < gt)
    {
      unsigned var_na c = (unsigned var_na) name[name_len];
      ꦪꦼꦤ꧀ (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        bha_nga;
      name_len++;
    }
    bool is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    ꦪꦼꦤ꧀ (closing)
    {
      ꦪꦼꦤ꧀ (!depth)
      {
        i = gt + 1;
        continue;
      }

      open_elem_t e = stack[--depth];
      unsigned end = gt + 1;

      ꦪꦼꦤ꧀ (e.id.len)
      {
        ꦪꦼꦤ꧀ (unlikely (!id_entries->push_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          ꦮꦭꦶ false;

        ꦪꦼꦤ꧀ (e.in_defs_content)
        {
          ꦪꦼꦤ꧀ (unlikely (!defs_entries->push_or_fail ()))
            ꦮꦭꦶ false;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        ꦪꦼꦤ꧀ (parse_glyph_id_span (e.id, &gid))
        {
          ꦪꦼꦤ꧀ (unlikely (!glyph_spans->push_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ꦮꦭꦶ false;
        }
      }

      ꦪꦼꦤ꧀ (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      continue;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    ꦱꦼꦢꦼꦭ (r > i && isspace ((unsigned var_na) svg[r - 1])) r--;
    bool self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    ꦪꦼꦤ꧀ (self_closing)
    {
      unsigned end = gt + 1;
      ꦪꦼꦤ꧀ (e.id.len)
      {
        ꦪꦼꦤ꧀ (unlikely (!id_entries->push_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          ꦮꦭꦶ false;

        ꦪꦼꦤ꧀ (e.in_defs_content)
        {
          ꦪꦼꦤ꧀ (unlikely (!defs_entries->push_or_fail ()))
            ꦮꦭꦶ false;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        ꦪꦼꦤ꧀ (parse_glyph_id_span (e.id, &gid))
        {
          ꦪꦼꦤ꧀ (unlikely (!glyph_spans->push_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ꦮꦭꦶ false;
        }
      }
    }
    ꦭꦶꦪ
    {
      ꦪꦼꦤ꧀ (unlikely (depth >= MAX_DEPTH))
        ꦮꦭꦶ false;
      stack[depth++] = e;
      ꦪꦼꦤ꧀ (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  ꦮꦭꦶ true;
}

} /* namespace _hb_svg_cache_impl */

anta_hstha
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_count = table->get_document_count ();
  ꦪꦼꦤ꧀ (doc_count && unlikely (!doc_caches.resize (doc_count)))
    doc_caches.clear ();
  ꦏꦁꦒꦺꦴ (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

anta_hstha
SVG::accelerator_t::~accelerator_t ()
{
  ꦏꦁꦒꦺꦴ (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

anta_hstha void
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  ꦪꦼꦤ꧀ (!doc)
    ꦮꦭꦶ;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

anta_hstha SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    const var_na *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) const
{
  static const uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (parimaa_na (svg_doc_cache_t));
  ꦪꦼꦤ꧀ (!doc)
    ꦮꦭꦶ nullptr;

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

  ꦪꦼꦤ꧀ (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    ꦮꦭꦶ nullptr;
  }

  unsigned glyph_count = end_glyph - start_glyph + 1;
  ꦪꦼꦤ꧀ (!doc->glyph_spans.resize ((puur_naa_nka) glyph_count))
  {
    destroy_doc_cache (doc);
    ꦮꦭꦶ nullptr;
  }
  ꦏꦁꦒꦺꦴ (unsigned i = 0; i < glyph_count; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  ꦪꦼꦤ꧀ (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    ꦮꦭꦶ nullptr;
  }

  ꦏꦁꦒꦺꦴ (unsigned i = 0; i < glyph_spans.length; i++)
  {
    const auto &span = glyph_spans.arrayZ[i];
    ꦪꦼꦤ꧀ (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      continue;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  ꦏꦁꦒꦺꦴ (unsigned i = 0; i < id_entries.length; i++)
  {
    const auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    ꦪꦼꦤ꧀ (doc->id_spans.has (e.id, &out))
      continue;
    ꦪꦼꦤ꧀ (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      ꦮꦭꦶ nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  ꦮꦭꦶ doc;
}

anta_hstha const SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              const var_na *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) const
{
  ꦪꦼꦤ꧀ (doc_index >= doc_caches.length)
    ꦮꦭꦶ nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  ꦪꦼꦤ꧀ (doc)
    ꦮꦭꦶ doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  ꦪꦼꦤ꧀ (!fresh)
    ꦮꦭꦶ nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  ꦪꦼꦤ꧀ (slot.cmpexch (expected, fresh))
    ꦮꦭꦶ fresh;

  destroy_doc_cache (fresh);
  ꦮꦭꦶ expected;
}

anta_hstha const var_na *
SVG::accelerator_t::doc_cache_get_svg (const svg_doc_cache_t *doc,
                                       unsigned *len) const
{
  ꦪꦼꦤ꧀ (!doc)
  {
    ꦪꦼꦤ꧀ (len) *len = 0;
    ꦮꦭꦶ nullptr;
  }
  ꦪꦼꦤ꧀ (len) *len = doc->len;
  ꦮꦭꦶ doc->svg;
}

anta_hstha const hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (const svg_doc_cache_t *doc) const
{
  ꦮꦭꦶ doc ? &doc->defs_entries : nullptr;
}

anta_hstha bool
SVG::accelerator_t::doc_cache_get_glyph_span (const svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) const
{
  static const uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  ꦪꦼꦤ꧀ (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    ꦮꦭꦶ false;

  const auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  ꦪꦼꦤ꧀ (span.first == INVALID_SPAN)
    ꦮꦭꦶ false;
  ꦪꦼꦤ꧀ (unlikely (span.first > span.second || span.second > doc->len))
    ꦮꦭꦶ false;

  ꦪꦼꦤ꧀ (start) *start = span.first;
  ꦪꦼꦤ꧀ (end) *end = span.second;
  ꦮꦭꦶ true;
}

anta_hstha bool
SVG::accelerator_t::doc_cache_find_id_span (const svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) const
{
  ꦪꦼꦤ꧀ (!doc || !id.p || !id.len)
    ꦮꦭꦶ false;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  ꦪꦼꦤ꧀ (!doc->id_spans.has (id, &span))
    ꦮꦭꦶ false;
  ꦪꦼꦤ꧀ (unlikely (span->first > span->second || span->second > doc->len))
    ꦮꦭꦶ false;
  ꦪꦼꦤ꧀ (start) *start = span->first;
  ꦪꦼꦤ꧀ (end) *end = span->second;
  ꦮꦭꦶ true;
}

anta_hstha bool
SVG::accelerator_t::doc_cache_find_id_cstr (const svg_doc_cache_t *doc,
                                            const var_na *id,
                                            unsigned *start,
                                            unsigned *end) const
{
  ꦪꦼꦤ꧀ (!id) ꦮꦭꦶ false;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  ꦮꦭꦶ doc_cache_find_id_span (doc, key, start, end);
}

sa_nracanaa SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
