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

#அந்தர்பாவ "../../../hb-open-type.hh"
#அந்தர்பாவ "../../../hb-blob.hh"
#அந்தர்பாவ "../../../hb-limits.hh"
#அந்தர்பாவ "../../../hb-map.hh"
#அந்தர்பாவ "../../../hb-paint.hh"
#அந்தர்பாவ "../../../hb-zlib.hh"
#அந்தர்பாவ <ctype.h>
#அந்தர்பாவ <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


நாமாகாஶ OT {

நிலையான உள்வரி hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     மாறிலி எழுத்து **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  மாறிலி எழுத்து *data = hb_blob_get_data (blob, &data_len);

  எனில் (!data || !data_len)
    செல் fail;

  எனில் (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    எனில் (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      செல் fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    எனில் (!uncompressed)
      செல் fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    எனில் (!data || !data_len)
      செல் fail;
  }

  எனில் (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    செல் fail;

  எனில் (svg) *svg = data;
  எனில் (len) *len = data_len;
  திரும்பு blob;

fail:
  hb_blob_destroy (blob);
  எனில் (svg) *svg = nullptr;
  எனில் (len) *len = 0;
  திரும்பு nullptr;
}

கட்டமைப்பு SVGDocumentIndexEntry
{
  முழு cmp (hb_codepoint_t g) மாறிலி
  { திரும்பு g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () மாறிலி
  { திரும்பு startGlyphID; }

  hb_codepoint_t get_end_glyph () மாறிலி
  { திரும்பு endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned முழு index_offset) மாறிலி
  {
    திரும்பு hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned முழு) svgDoc,
				    svgDocLength);
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  பொது:
  DEFINE_SIZE_STATIC (12);
};

கட்டமைப்பு SVG
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  கட்டமைப்பு svg_id_span_t
  {
    மாறிலி எழுத்து *p;
    unsigned len;

    தருக்க operator == (மாறிலி svg_id_span_t &o) மாறிலி
    {
      திரும்பு len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () மாறிலி
    {
      uint32_t h = hb_hash (len);
      க்கு (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned எழுத்து) p[i];
      திரும்பு h;
    }
  };

  கட்டமைப்பு svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  கட்டமைப்பு svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    மாறிலி எழுத்து *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  தருக்க has_data () மாறிலி { திரும்பு svgDocEntries; }

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) மாறிலி
    {
      திரும்பு table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_மொத்தம் () மாறிலி
    { திரும்பு table->get_document_மொத்தம் (); }

    தருக்க get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) மாறிலி
    { திரும்பு table->get_glyph_document_index (glyph_id, index); }

    தருக்க get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) மாறிலி
    { திரும்பு table->get_document_glyph_range (index, start_glyph, end_glyph); }

    தருக்க has_data () மாறிலி { திரும்பு table->has_data (); }

    மாறிலி svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             மாறிலி எழுத்து *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) மாறிலி;

    மாறிலி எழுத்து *
    doc_cache_get_svg (மாறிலி svg_doc_cache_t *doc,
                       unsigned *len) மாறிலி;

    மாறிலி hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (மாறிலி svg_doc_cache_t *doc) மாறிலி;

    தருக்க
    doc_cache_get_glyph_span (மாறிலி svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) மாறிலி;

    தருக்க
    doc_cache_find_id_span (மாறிலி svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) மாறிலி;

    தருக்க
    doc_cache_find_id_cstr (மாறிலி svg_doc_cache_t *doc,
                            மாறிலி எழுத்து *id,
                            unsigned *start,
                            unsigned *end) மாறிலி;

    தருக்க paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, வெறுமை *data) மாறிலி
    {
      எனில் (!has_data ())
        திரும்பு பொய்;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      எனில் (blob == hb_blob_get_empty ())
        திரும்பு பொய்;

      தருக்க ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      திரும்பு ret;
    }

    தனிப்பட்ட:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    மாறிலி எழுத்து *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) மாறிலி;

    நிலையான வெறுமை destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    பொது:
    DEFINE_SIZE_STATIC (அளவு (hb_blob_ptr_t<SVG>) +
                        அளவு (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  மாறிலி SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) மாறிலி
  { திரும்பு (இது+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_மொத்தம் () மாறிலி
  {
    எனில் (!has_data ())
      திரும்பு 0;
    திரும்பு (இது + svgDocEntries).len;
  }

  தருக்க get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) மாறிலி
  {
    எனில் (!has_data ())
      திரும்பு பொய்;
    திரும்பு (இது + svgDocEntries).bfind (glyph_id, index);
  }

  தருக்க get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) மாறிலி
  {
    எனில் (!has_data ())
      திரும்பு பொய்;

    மாறிலி auto &entries = இது + svgDocEntries;
    எனில் (index >= entries.len)
      திரும்பு பொய்;

    மாறிலி auto &entry = entries.arrayZ[index];
    எனில் (start_glyph) *start_glyph = entry.get_start_glyph ();
    எனில் (end_glyph) *end_glyph = entry.get_end_glyph ();
    திரும்பு உண்மை;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  (இது+svgDocEntries).sanitize_shallow (c)));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  பொது:
  DEFINE_SIZE_STATIC (10);
};

நாமாகாஶ _hb_svg_cache_impl {

கட்டமைப்பு glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

கட்டமைப்பு id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

கட்டமைப்பு open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  தருக்க in_defs_content;
  தருக்க is_defs;
};

நிலையான மாறிலி unsigned MAX_DEPTH = 128;

நிலையான உள்வரி முழு
find_substr (மாறிலி எழுத்து *s,
             unsigned n,
             unsigned from,
             மாறிலி எழுத்து *needle,
             unsigned needle_len)
{
  எனில் (!needle_len || from >= n || needle_len > n)
    திரும்பு -1;
  க்கு (unsigned i = from; i + needle_len <= n; i++)
    எனில் (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      திரும்பு (முழு) i;
  திரும்பு -1;
}

நிலையான உள்வரி தருக்க
parse_id_in_start_tag (மாறிலி எழுத்து *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  போது (p + 4 <= tag_end)
  {
    எனில் (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      போது (e < tag_end && svg[e] != '"') e++;
      எனில் (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        திரும்பு உண்மை;
      }
    }
    எனில் (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      போது (e < tag_end && svg[e] != '\'') e++;
      எனில் (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        திரும்பு உண்மை;
      }
    }
    p++;
  }
  திரும்பு பொய்;
}

நிலையான உள்வரி தருக்க
parse_glyph_id_span (மாறிலி SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  எனில் (id.len <= 5 || memcmp (id.p, "glyph", 5))
    திரும்பு பொய்;

  hb_codepoint_t gid = 0;
  க்கு (unsigned i = 5; i < id.len; i++)
  {
    unsigned எழுத்து c = (unsigned எழுத்து) id.p[i];
    எனில் (c < '0' || c > '9')
      திரும்பு பொய்;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    எனில் (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      திரும்பு பொய்;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  திரும்பு உண்மை;
}

நிலையான உள்வரி தருக்க
parse_cache_entries_linear (மாறிலி எழுத்து *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t அடுக்கு[MAX_DEPTH] = {};
  unsigned depth = 0;
  எனில் (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    திரும்பு பொய்;

  unsigned defs_depth = 0;
  unsigned i = 0;
  போது (i < len)
  {
    எனில் (svg[i] != '<')
    {
      i++;
      தொடர்;
    }

    எனில் (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      முழு cend = find_substr (svg, len, i + 4, "-->", 3);
      எனில் (cend < 0) திரும்பு பொய்;
      i = (unsigned) cend + 3;
      தொடர்;
    }
    எனில் (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      முழு cend = find_substr (svg, len, i + 9, "]]>", 3);
      எனில் (cend < 0) திரும்பு பொய்;
      i = (unsigned) cend + 3;
      தொடர்;
    }

    தருக்க closing = (i + 1 < len && svg[i + 1] == '/');
    தருக்க special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    எழுத்து quote = 0;
    போது (gt < len)
    {
      எழுத்து c = svg[gt];
      எனில் (quote)
      {
        எனில் (c == quote) quote = 0;
      }
      இல்லையெனில்
      {
        எனில் (c == '"' || c == '\'')
          quote = c;
        இல்லையெனில் எனில் (c == '>')
          நிறுத்து;
      }
      gt++;
    }
    எனில் (gt >= len)
      திரும்பு பொய்;

    எனில் (special)
    {
      i = gt + 1;
      தொடர்;
    }

    unsigned p = i + (closing ? 2 : 1);
    போது (p < gt && isspace ((unsigned எழுத்து) svg[p])) p++;
    மாறிலி எழுத்து *name = svg + p;
    unsigned name_len = 0;
    போது (p + name_len < gt)
    {
      unsigned எழுத்து c = (unsigned எழுத்து) name[name_len];
      எனில் (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        நிறுத்து;
      name_len++;
    }
    தருக்க is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    எனில் (closing)
    {
      எனில் (!depth)
      {
        i = gt + 1;
        தொடர்;
      }

      open_elem_t e = அடுக்கு[--depth];
      unsigned end = gt + 1;

      எனில் (e.id.len)
      {
        எனில் (unlikely (!id_entries->தள்ளு_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          திரும்பு பொய்;

        எனில் (e.in_defs_content)
        {
          எனில் (unlikely (!defs_entries->தள்ளு_or_fail ()))
            திரும்பு பொய்;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        எனில் (parse_glyph_id_span (e.id, &gid))
        {
          எனில் (unlikely (!glyph_spans->தள்ளு_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            திரும்பு பொய்;
        }
      }

      எனில் (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      தொடர்;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    போது (r > i && isspace ((unsigned எழுத்து) svg[r - 1])) r--;
    தருக்க self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    எனில் (self_closing)
    {
      unsigned end = gt + 1;
      எனில் (e.id.len)
      {
        எனில் (unlikely (!id_entries->தள்ளு_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          திரும்பு பொய்;

        எனில் (e.in_defs_content)
        {
          எனில் (unlikely (!defs_entries->தள்ளு_or_fail ()))
            திரும்பு பொய்;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        எனில் (parse_glyph_id_span (e.id, &gid))
        {
          எனில் (unlikely (!glyph_spans->தள்ளு_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            திரும்பு பொய்;
        }
      }
    }
    இல்லையெனில்
    {
      எனில் (unlikely (depth >= MAX_DEPTH))
        திரும்பு பொய்;
      அடுக்கு[depth++] = e;
      எனில் (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  திரும்பு உண்மை;
}

} /* namespace _hb_svg_cache_impl */

உள்வரி
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_மொத்தம் = table->get_document_மொத்தம் ();
  எனில் (doc_மொத்தம் && unlikely (!doc_caches.resize (doc_மொத்தம்)))
    doc_caches.clear ();
  க்கு (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

உள்வரி
SVG::accelerator_t::~accelerator_t ()
{
  க்கு (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

உள்வரி வெறுமை
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  எனில் (!doc)
    திரும்பு;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

உள்வரி SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    மாறிலி எழுத்து *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) மாறிலி
{
  நிலையான மாறிலி uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (அளவு (svg_doc_cache_t));
  எனில் (!doc)
    திரும்பு nullptr;

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

  எனில் (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    திரும்பு nullptr;
  }

  unsigned glyph_மொத்தம் = end_glyph - start_glyph + 1;
  எனில் (!doc->glyph_spans.resize ((முழு) glyph_மொத்தம்))
  {
    destroy_doc_cache (doc);
    திரும்பு nullptr;
  }
  க்கு (unsigned i = 0; i < glyph_மொத்தம்; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  எனில் (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    திரும்பு nullptr;
  }

  க்கு (unsigned i = 0; i < glyph_spans.length; i++)
  {
    மாறிலி auto &span = glyph_spans.arrayZ[i];
    எனில் (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      தொடர்;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  க்கு (unsigned i = 0; i < id_entries.length; i++)
  {
    மாறிலி auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    எனில் (doc->id_spans.has (e.id, &out))
      தொடர்;
    எனில் (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      திரும்பு nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  திரும்பு doc;
}

உள்வரி மாறிலி SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              மாறிலி எழுத்து *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) மாறிலி
{
  எனில் (doc_index >= doc_caches.length)
    திரும்பு nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  எனில் (doc)
    திரும்பு doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  எனில் (!fresh)
    திரும்பு nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  எனில் (slot.cmpexch (expected, fresh))
    திரும்பு fresh;

  destroy_doc_cache (fresh);
  திரும்பு expected;
}

உள்வரி மாறிலி எழுத்து *
SVG::accelerator_t::doc_cache_get_svg (மாறிலி svg_doc_cache_t *doc,
                                       unsigned *len) மாறிலி
{
  எனில் (!doc)
  {
    எனில் (len) *len = 0;
    திரும்பு nullptr;
  }
  எனில் (len) *len = doc->len;
  திரும்பு doc->svg;
}

உள்வரி மாறிலி hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (மாறிலி svg_doc_cache_t *doc) மாறிலி
{
  திரும்பு doc ? &doc->defs_entries : nullptr;
}

உள்வரி தருக்க
SVG::accelerator_t::doc_cache_get_glyph_span (மாறிலி svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) மாறிலி
{
  நிலையான மாறிலி uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  எனில் (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    திரும்பு பொய்;

  மாறிலி auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  எனில் (span.first == INVALID_SPAN)
    திரும்பு பொய்;
  எனில் (unlikely (span.first > span.second || span.second > doc->len))
    திரும்பு பொய்;

  எனில் (start) *start = span.first;
  எனில் (end) *end = span.second;
  திரும்பு உண்மை;
}

உள்வரி தருக்க
SVG::accelerator_t::doc_cache_find_id_span (மாறிலி svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) மாறிலி
{
  எனில் (!doc || !id.p || !id.len)
    திரும்பு பொய்;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  எனில் (!doc->id_spans.has (id, &span))
    திரும்பு பொய்;
  எனில் (unlikely (span->first > span->second || span->second > doc->len))
    திரும்பு பொய்;
  எனில் (start) *start = span->first;
  எனில் (end) *end = span->second;
  திரும்பு உண்மை;
}

உள்வரி தருக்க
SVG::accelerator_t::doc_cache_find_id_cstr (மாறிலி svg_doc_cache_t *doc,
                                            மாறிலி எழுத்து *id,
                                            unsigned *start,
                                            unsigned *end) மாறிலி
{
  எனில் (!id) திரும்பு பொய்;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  திரும்பு doc_cache_find_id_span (doc, key, start, end);
}

கட்டமைப்பு SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
