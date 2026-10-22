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

#ଅନ୍ତର୍ଭାଵ "../../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-blob.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-limits.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-map.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-paint.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-zlib.hh"
#ଅନ୍ତର୍ଭାଵ <ctype.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#define HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


ନାମାକାଶ OT {

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ଅଚଳ ଅକ୍ଷର **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned data_len = 0;
  ଅଚଳ ଅକ୍ଷର *data = hb_blob_get_data (blob, &data_len);

  ଯଦି (!data || !data_len)
    ଯାଅ fail;

  ଯଦି (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    ଯଦି (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((size_t) expected_size > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
      ଯାଅ fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    ଯଦି (!uncompressed)
      ଯାଅ fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    ଯଦି (!data || !data_len)
      ଯାଅ fail;
  }

  ଯଦି (unlikely ((size_t) data_len > (size_t) HB_SVG_MAX_DOCUMENT_SIZE))
    ଯାଅ fail;

  ଯଦି (svg) *svg = data;
  ଯଦି (len) *len = data_len;
  ଫେରନ୍ତୁ blob;

fail:
  hb_blob_destroy (blob);
  ଯଦି (svg) *svg = nullptr;
  ଯଦି (len) *len = 0;
  ଫେରନ୍ତୁ nullptr;
}

ସଂରଚନା SVGDocumentIndexEntry
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t g) ଅଚଳ
  { ଫେରନ୍ତୁ g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () ଅଚଳ
  { ଫେରନ୍ତୁ startGlyphID; }

  hb_codepoint_t get_end_glyph () ଅଚଳ
  { ଫେରନ୍ତୁ endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index_offset) ଅଚଳ
  {
    ଫେରନ୍ତୁ hb_blob_create_sub_blob (svg_blob,
				    index_offset + (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) svgDoc,
				    svgDocLength);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svgDocLength;	/* Length of the SVG document.
				 * Must be non-zero. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};

ସଂରଚନା SVG
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  ସଂରଚନା svg_id_span_t
  {
    ଅଚଳ ଅକ୍ଷର *p;
    unsigned len;

    ତାର୍କିକ operator == (ଅଚଳ svg_id_span_t &o) ଅଚଳ
    {
      ଫେରନ୍ତୁ len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ଅଚଳ
    {
      uint32_t h = hb_hash (len);
      ପାଇଁ (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned ଅକ୍ଷର) p[i];
      ଫେରନ୍ତୁ h;
    }
  };

  ସଂରଚନା svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  ସଂରଚନା svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    ଅଚଳ ଅକ୍ଷର *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ svgDocEntries; }

  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ଅଚଳ
    {
      ଫେରନ୍ତୁ table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_ଗଣନା () ଅଚଳ
    { ଫେରନ୍ତୁ table->get_document_ଗଣନା (); }

    ତାର୍କିକ get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ଅଚଳ
    { ଫେରନ୍ତୁ table->get_glyph_document_index (glyph_id, index); }

    ତାର୍କିକ get_document_glyph_range (unsigned index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) ଅଚଳ
    { ଫେରନ୍ତୁ table->get_document_glyph_range (index, start_glyph, end_glyph); }

    ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ table->has_data (); }

    ଅଚଳ svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ଅଚଳ ଅକ୍ଷର *svg,
                             unsigned len,
                             unsigned doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) ଅଚଳ;

    ଅଚଳ ଅକ୍ଷର *
    doc_cache_get_svg (ଅଚଳ svg_doc_cache_t *doc,
                       unsigned *len) ଅଚଳ;

    ଅଚଳ hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (ଅଚଳ svg_doc_cache_t *doc) ଅଚଳ;

    ତାର୍କିକ
    doc_cache_get_glyph_span (ଅଚଳ svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) ଅଚଳ;

    ତାର୍କିକ
    doc_cache_find_id_span (ଅଚଳ svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) ଅଚଳ;

    ତାର୍କିକ
    doc_cache_find_id_cstr (ଅଚଳ svg_doc_cache_t *doc,
                            ଅଚଳ ଅକ୍ଷର *id,
                            unsigned *start,
                            unsigned *end) ଅଚଳ;

    ତାର୍କିକ paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ଶୂନ୍ୟ *data) ଅଚଳ
    {
      ଯଦି (!has_data ())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      ଯଦି (blob == hb_blob_get_empty ())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ତାର୍କିକ ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      ଫେରନ୍ତୁ ret;
    }

    ବ୍ୟକ୍ତିଗତ:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ଅଚଳ ଅକ୍ଷର *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) ଅଚଳ;

    ସ୍ଥିର ଶୂନ୍ୟ destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    ସର୍ବସାଧାରଣ:
    DEFINE_SIZE_STATIC (ଆକାର (hb_blob_ptr_t<SVG>) +
                        ଆକାର (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ଅଚଳ SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_ଗଣନା () ଅଚଳ
  {
    ଯଦି (!has_data ())
      ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ (ଏହି + svgDocEntries).len;
  }

  ତାର୍କିକ get_glyph_document_index (hb_codepoint_t glyph_id, unsigned *index) ଅଚଳ
  {
    ଯଦି (!has_data ())
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ (ଏହି + svgDocEntries).bfind (glyph_id, index);
  }

  ତାର୍କିକ get_document_glyph_range (unsigned index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) ଅଚଳ
  {
    ଯଦି (!has_data ())
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ auto &entries = ଏହି + svgDocEntries;
    ଯଦି (index >= entries.len)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ auto &entry = entries.arrayZ[index];
    ଯଦି (start_glyph) *start_glyph = entry.get_start_glyph ();
    ଯଦି (end_glyph) *end_glyph = entry.get_end_glyph ();
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  (ଏହି+svgDocEntries).sanitize_shallow (c)));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocumentIndexEntry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (10);
};

ନାମାକାଶ _hb_svg_cache_impl {

ସଂରଚନା glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

ସଂରଚନା id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

ସଂରଚନା open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  ତାର୍କିକ in_defs_content;
  ତାର୍କିକ is_defs;
};

ସ୍ଥିର ଅଚଳ unsigned MAX_DEPTH = 128;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
find_substr (ଅଚଳ ଅକ୍ଷର *s,
             unsigned n,
             unsigned from,
             ଅଚଳ ଅକ୍ଷର *needle,
             unsigned needle_len)
{
  ଯଦି (!needle_len || from >= n || needle_len > n)
    ଫେରନ୍ତୁ -1;
  ପାଇଁ (unsigned i = from; i + needle_len <= n; i++)
    ଯଦି (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i;
  ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
parse_id_in_start_tag (ଅଚଳ ଅକ୍ଷର *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  ଯେତେବେଳେ (p + 4 <= tag_end)
  {
    ଯଦି (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ଯେତେବେଳେ (e < tag_end && svg[e] != '"') e++;
      ଯଦି (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }
    ଯଦି (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      ଯେତେବେଳେ (e < tag_end && svg[e] != '\'') e++;
      ଯଦି (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }
    p++;
  }
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
parse_glyph_id_span (ଅଚଳ SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  ଯଦି (id.len <= 5 || memcmp (id.p, "glyph", 5))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_codepoint_t gid = 0;
  ପାଇଁ (unsigned i = 5; i < id.len; i++)
  {
    unsigned ଅକ୍ଷର c = (unsigned ଅକ୍ଷର) id.p[i];
    ଯଦି (c < '0' || c > '9')
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    ଯଦି (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
parse_cache_entries_linear (ଅଚଳ ଅକ୍ଷର *svg,
                            unsigned len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t ଗଦା[MAX_DEPTH] = {};
  unsigned depth = 0;
  ଯଦି (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  unsigned defs_depth = 0;
  unsigned i = 0;
  ଯେତେବେଳେ (i < len)
  {
    ଯଦି (svg[i] != '<')
    {
      i++;
      ଜାରି;
    }

    ଯଦି (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cend = find_substr (svg, len, i + 4, "-->", 3);
      ଯଦି (cend < 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      i = (unsigned) cend + 3;
      ଜାରି;
    }
    ଯଦି (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cend = find_substr (svg, len, i + 9, "]]>", 3);
      ଯଦି (cend < 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      i = (unsigned) cend + 3;
      ଜାରି;
    }

    ତାର୍କିକ closing = (i + 1 < len && svg[i + 1] == '/');
    ତାର୍କିକ special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    ଅକ୍ଷର quote = 0;
    ଯେତେବେଳେ (gt < len)
    {
      ଅକ୍ଷର c = svg[gt];
      ଯଦି (quote)
      {
        ଯଦି (c == quote) quote = 0;
      }
      ଅନ୍ୟଥା
      {
        ଯଦି (c == '"' || c == '\'')
          quote = c;
        ଅନ୍ୟଥା ଯଦି (c == '>')
          ଭାଙ୍ଗ;
      }
      gt++;
    }
    ଯଦି (gt >= len)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (special)
    {
      i = gt + 1;
      ଜାରି;
    }

    unsigned p = i + (closing ? 2 : 1);
    ଯେତେବେଳେ (p < gt && isspace ((unsigned ଅକ୍ଷର) svg[p])) p++;
    ଅଚଳ ଅକ୍ଷର *name = svg + p;
    unsigned name_len = 0;
    ଯେତେବେଳେ (p + name_len < gt)
    {
      unsigned ଅକ୍ଷର c = (unsigned ଅକ୍ଷର) name[name_len];
      ଯଦି (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        ଭାଙ୍ଗ;
      name_len++;
    }
    ତାର୍କିକ is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    ଯଦି (closing)
    {
      ଯଦି (!depth)
      {
        i = gt + 1;
        ଜାରି;
      }

      open_elem_t e = ଗଦା[--depth];
      unsigned end = gt + 1;

      ଯଦି (e.id.len)
      {
        ଯଦି (unlikely (!id_entries->ଠେଲ_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଯଦି (e.in_defs_content)
        {
          ଯଦି (unlikely (!defs_entries->ଠେଲ_or_fail ()))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        ଯଦି (parse_glyph_id_span (e.id, &gid))
        {
          ଯଦି (unlikely (!glyph_spans->ଠେଲ_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
      }

      ଯଦି (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      ଜାରି;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    ଯେତେବେଳେ (r > i && isspace ((unsigned ଅକ୍ଷର) svg[r - 1])) r--;
    ତାର୍କିକ self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    ଯଦି (self_closing)
    {
      unsigned end = gt + 1;
      ଯଦି (e.id.len)
      {
        ଯଦି (unlikely (!id_entries->ଠେଲ_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଯଦି (e.in_defs_content)
        {
          ଯଦି (unlikely (!defs_entries->ଠେଲ_or_fail ()))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
          auto &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        ଯଦି (parse_glyph_id_span (e.id, &gid))
        {
          ଯଦି (unlikely (!glyph_spans->ଠେଲ_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
      }
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (unlikely (depth >= MAX_DEPTH))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଗଦା[depth++] = e;
      ଯଦି (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

} /* namespace _hb_svg_cache_impl */

ଧାଡ଼ିଭିତରେ
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_ଗଣନା = table->get_document_ଗଣନା ();
  ଯଦି (doc_ଗଣନା && unlikely (!doc_caches.resize (doc_ଗଣନା)))
    doc_caches.clear ();
  ପାଇଁ (unsigned i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

ଧାଡ଼ିଭିତରେ
SVG::accelerator_t::~accelerator_t ()
{
  ପାଇଁ (unsigned i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  ଯଦି (!doc)
    ଫେରନ୍ତୁ;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

ଧାଡ଼ିଭିତରେ SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ଅଚଳ ଅକ୍ଷର *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) ଅଚଳ
{
  ସ୍ଥିର ଅଚଳ uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (ଆକାର (svg_doc_cache_t));
  ଯଦି (!doc)
    ଫେରନ୍ତୁ nullptr;

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

  ଯଦି (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    ଫେରନ୍ତୁ nullptr;
  }

  unsigned glyph_ଗଣନା = end_glyph - start_glyph + 1;
  ଯଦି (!doc->glyph_spans.resize ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) glyph_ଗଣନା))
  {
    destroy_doc_cache (doc);
    ଫେରନ୍ତୁ nullptr;
  }
  ପାଇଁ (unsigned i = 0; i < glyph_ଗଣନା; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  ଯଦି (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    ଫେରନ୍ତୁ nullptr;
  }

  ପାଇଁ (unsigned i = 0; i < glyph_spans.length; i++)
  {
    ଅଚଳ auto &span = glyph_spans.arrayZ[i];
    ଯଦି (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      ଜାରି;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  ପାଇଁ (unsigned i = 0; i < id_entries.length; i++)
  {
    ଅଚଳ auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    ଯଦି (doc->id_spans.has (e.id, &out))
      ଜାରି;
    ଯଦି (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      ଫେରନ୍ତୁ nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  ଫେରନ୍ତୁ doc;
}

ଧାଡ଼ିଭିତରେ ଅଚଳ SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ଅଚଳ ଅକ୍ଷର *svg,
                                              unsigned len,
                                              unsigned doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) ଅଚଳ
{
  ଯଦି (doc_index >= doc_caches.length)
    ଫେରନ୍ତୁ nullptr;

  auto &slot = doc_caches.arrayZ[doc_index];
  auto *doc = slot.get_acquire ();
  ଯଦି (doc)
    ଫେରନ୍ତୁ doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  ଯଦି (!fresh)
    ଫେରନ୍ତୁ nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  ଯଦି (slot.cmpexch (expected, fresh))
    ଫେରନ୍ତୁ fresh;

  destroy_doc_cache (fresh);
  ଫେରନ୍ତୁ expected;
}

ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *
SVG::accelerator_t::doc_cache_get_svg (ଅଚଳ svg_doc_cache_t *doc,
                                       unsigned *len) ଅଚଳ
{
  ଯଦି (!doc)
  {
    ଯଦି (len) *len = 0;
    ଫେରନ୍ତୁ nullptr;
  }
  ଯଦି (len) *len = doc->len;
  ଫେରନ୍ତୁ doc->svg;
}

ଧାଡ଼ିଭିତରେ ଅଚଳ hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ଅଚଳ svg_doc_cache_t *doc) ଅଚଳ
{
  ଫେରନ୍ତୁ doc ? &doc->defs_entries : nullptr;
}

ଧାଡ଼ିଭିତରେ ତାର୍କିକ
SVG::accelerator_t::doc_cache_get_glyph_span (ଅଚଳ svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) ଅଚଳ
{
  ସ୍ଥିର ଅଚଳ uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  ଯଦି (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଅଚଳ auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  ଯଦି (span.first == INVALID_SPAN)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (unlikely (span.first > span.second || span.second > doc->len))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (start) *start = span.first;
  ଯଦି (end) *end = span.second;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ଧାଡ଼ିଭିତରେ ତାର୍କିକ
SVG::accelerator_t::doc_cache_find_id_span (ଅଚଳ svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) ଅଚଳ
{
  ଯଦି (!doc || !id.p || !id.len)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  ଯଦି (!doc->id_spans.has (id, &span))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (unlikely (span->first > span->second || span->second > doc->len))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (start) *start = span->first;
  ଯଦି (end) *end = span->second;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ଧାଡ଼ିଭିତରେ ତାର୍କିକ
SVG::accelerator_t::doc_cache_find_id_cstr (ଅଚଳ svg_doc_cache_t *doc,
                                            ଅଚଳ ଅକ୍ଷର *id,
                                            unsigned *start,
                                            unsigned *end) ଅଚଳ
{
  ଯଦି (!id) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  ଫେରନ୍ତୁ doc_cache_find_id_span (doc, key, start, end);
}

ସଂରଚନା SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
