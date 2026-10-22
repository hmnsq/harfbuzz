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

স্থির অন্তঃরেখ hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ধ্রুবক অক্ষর **svg,
                                     unsigned *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  unsigned তথ্য_len = 0;
  ধ্রুবক অক্ষর *তথ্য = hb_blob_get_তথ্য (blob, &তথ্য_len);

  যদি (!তথ্য || !তথ্য_len)
    যাও fail;

  যদি (hb_blob_is_gzip (তথ্য, তথ্য_len))
  {
    uint32_t expected_আকার = 0;
    যদি (hb_gzip_get_uncompressed_আকার (তথ্য, তথ্য_len, &expected_আকার) &&
        unlikely ((size_t) expected_আকার > (size_t) HB_SVG_MAX_DOCUMENT_আকার))
      যাও fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_আকার);
    যদি (!uncompressed)
      যাও fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    তথ্য = hb_blob_get_তথ্য (blob, &তথ্য_len);
    যদি (!তথ্য || !তথ্য_len)
      যাও fail;
  }

  যদি (unlikely ((size_t) তথ্য_len > (size_t) HB_SVG_MAX_DOCUMENT_আকার))
    যাও fail;

  যদি (svg) *svg = তথ্য;
  যদি (len) *len = তথ্য_len;
  ফেরত blob;

fail:
  hb_blob_destroy (blob);
  যদি (svg) *svg = nullptr;
  যদি (len) *len = 0;
  ফেরত nullptr;
}

কাঠামো SVGDocument_সূচক_Entry
{
  পূর্ণসংখ্যা cmp (hb_codepoint_t g) ধ্রুবক
  { ফেরত g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () ধ্রুবক
  { ফেরত startGlyphID; }

  hb_codepoint_t get_end_glyph () ধ্রুবক
  { ফেরত endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, unsigned পূর্ণসংখ্যা সূচক_offset) ধ্রুবক
  {
    ফেরত hb_blob_create_sub_blob (svg_blob,
				    সূচক_offset + (unsigned পূর্ণসংখ্যা) svgDoc,
				    svg_Doc_দৈর্ঘ্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svg_Doc_দৈর্ঘ্য));
  }

  সুরক্ষিত:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svg_Doc_দৈর্ঘ্য;	/* Length of the SVG document.
				 * Must be non-zero. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

কাঠামো SVG
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  কাঠামো svg_id_span_t
  {
    ধ্রুবক অক্ষর *p;
    unsigned len;

    যৌক্তিক operator == (ধ্রুবক svg_id_span_t &o) ধ্রুবক
    {
      ফেরত len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ধ্রুবক
    {
      uint32_t h = hb_hash (len);
      জন্য (unsigned i = 0; i < len; i++)
        h = h * 33u + (unsigned অক্ষর) p[i];
      ফেরত h;
    }
  };

  কাঠামো svg_defs_entry_t
  {
    svg_id_span_t id;
    unsigned start;
    unsigned end;
  };

  কাঠামো svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    ধ্রুবক অক্ষর *svg = nullptr;
    unsigned len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত svgDocEntries; }

  কাঠামো accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ধ্রুবক
    {
      ফেরত table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    unsigned get_document_গুনতি () ধ্রুবক
    { ফেরত table->get_document_গুনতি (); }

    যৌক্তিক get_glyph_document_সূচক (hb_codepoint_t glyph_id, unsigned *সূচক_2) ধ্রুবক
    { ফেরত table->get_glyph_document_সূচক (glyph_id, সূচক_2); }

    যৌক্তিক get_document_glyph_range (unsigned সূচক_2,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) ধ্রুবক
    { ফেরত table->get_document_glyph_range (সূচক_2, start_glyph, end_glyph); }

    যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত table->has_তথ্য (); }

    ধ্রুবক svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ধ্রুবক অক্ষর *svg,
                             unsigned len,
                             unsigned doc_সূচক,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) ধ্রুবক;

    ধ্রুবক অক্ষর *
    doc_cache_get_svg (ধ্রুবক svg_doc_cache_t *doc,
                       unsigned *len) ধ্রুবক;

    ধ্রুবক hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (ধ্রুবক svg_doc_cache_t *doc) ধ্রুবক;

    যৌক্তিক
    doc_cache_get_glyph_span (ধ্রুবক svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              unsigned *start,
                              unsigned *end) ধ্রুবক;

    যৌক্তিক
    doc_cache_find_id_span (ধ্রুবক svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            unsigned *start,
                            unsigned *end) ধ্রুবক;

    যৌক্তিক
    doc_cache_find_id_cstr (ধ্রুবক svg_doc_cache_t *doc,
                            ধ্রুবক অক্ষর *id,
                            unsigned *start,
                            unsigned *end) ধ্রুবক;

    যৌক্তিক paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, শূন্য *তথ্য) ধ্রুবক
    {
      যদি (!has_তথ্য ())
        ফেরত মিথ্যা;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      যদি (blob == hb_blob_get_empty ())
        ফেরত মিথ্যা;

      যৌক্তিক ret = funcs->image (তথ্য,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      ফেরত ret;
    }

    ব্যক্তিগত:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ধ্রুবক অক্ষর *svg,
                    unsigned len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) ধ্রুবক;

    স্থির শূন্য destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    সর্বজনীন:
    DEFINE_আকার_STATIC (আকার (hb_blob_ptr_t<SVG>) +
                        আকার (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ধ্রুবক SVGDocument_সূচক_Entry &get_glyph_entry (hb_codepoint_t glyph_id) ধ্রুবক
  { ফেরত (এই+svgDocEntries).bsearch (glyph_id); }

  unsigned get_document_গুনতি () ধ্রুবক
  {
    যদি (!has_তথ্য ())
      ফেরত 0;
    ফেরত (এই + svgDocEntries).len;
  }

  যৌক্তিক get_glyph_document_সূচক (hb_codepoint_t glyph_id, unsigned *সূচক_2) ধ্রুবক
  {
    যদি (!has_তথ্য ())
      ফেরত মিথ্যা;
    ফেরত (এই + svgDocEntries).bfind (glyph_id, সূচক_2);
  }

  যৌক্তিক get_document_glyph_range (unsigned সূচক_2,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) ধ্রুবক
  {
    যদি (!has_তথ্য ())
      ফেরত মিথ্যা;

    ধ্রুবক auto &entries = এই + svgDocEntries;
    যদি (সূচক_2 >= entries.len)
      ফেরত মিথ্যা;

    ধ্রুবক auto &entry = entries.arrayZ[সূচক_2];
    যদি (start_glyph) *start_glyph = entry.get_start_glyph ();
    যদি (end_glyph) *end_glyph = entry.get_end_glyph ();
    ফেরত সত্য;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  (এই+svgDocEntries).sanitize_shallow (c)));
  }

  সুরক্ষিত:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocument_সূচক_Entry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (10);
};

নামাকাশ _hb_svg_cache_impl {

কাঠামো glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

কাঠামো id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

কাঠামো open_elem_t
{
  unsigned start;
  SVG::svg_id_span_t id;
  যৌক্তিক in_defs_content;
  যৌক্তিক is_defs;
};

স্থির ধ্রুবক unsigned MAX_DEPTH = 128;

স্থির অন্তঃরেখ পূর্ণসংখ্যা
find_substr (ধ্রুবক অক্ষর *s,
             unsigned n,
             unsigned from,
             ধ্রুবক অক্ষর *needle,
             unsigned needle_len)
{
  যদি (!needle_len || from >= n || needle_len > n)
    ফেরত -1;
  জন্য (unsigned i = from; i + needle_len <= n; i++)
    যদি (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      ফেরত (পূর্ণসংখ্যা) i;
  ফেরত -1;
}

স্থির অন্তঃরেখ যৌক্তিক
parse_id_in_start_tag (ধ্রুবক অক্ষর *svg,
                       unsigned tag_start,
                       unsigned tag_end,
                       SVG::svg_id_span_t *id)
{
  unsigned p = tag_start;
  যখন (p + 4 <= tag_end)
  {
    যদি (!memcmp (svg + p, "id=\"", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      যখন (e < tag_end && svg[e] != '"') e++;
      যদি (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ফেরত সত্য;
      }
    }
    যদি (!memcmp (svg + p, "id='", 4))
    {
      unsigned b = p + 4;
      unsigned e = b;
      যখন (e < tag_end && svg[e] != '\'') e++;
      যদি (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        ফেরত সত্য;
      }
    }
    p++;
  }
  ফেরত মিথ্যা;
}

স্থির অন্তঃরেখ যৌক্তিক
parse_glyph_id_span (ধ্রুবক SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  যদি (id.len <= 5 || memcmp (id.p, "glyph", 5))
    ফেরত মিথ্যা;

  hb_codepoint_t gid = 0;
  জন্য (unsigned i = 5; i < id.len; i++)
  {
    unsigned অক্ষর c = (unsigned অক্ষর) id.p[i];
    যদি (c < '0' || c > '9')
      ফেরত মিথ্যা;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    যদি (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      ফেরত মিথ্যা;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  ফেরত সত্য;
}

স্থির অন্তঃরেখ যৌক্তিক
parse_cache_entries_linear (ধ্রুবক অক্ষর *svg,
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
    ফেরত মিথ্যা;

  unsigned defs_depth = 0;
  unsigned i = 0;
  যখন (i < len)
  {
    যদি (svg[i] != '<')
    {
      i++;
      চালু;
    }

    যদি (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      পূর্ণসংখ্যা cend = find_substr (svg, len, i + 4, "-->", 3);
      যদি (cend < 0) ফেরত মিথ্যা;
      i = (unsigned) cend + 3;
      চালু;
    }
    যদি (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      পূর্ণসংখ্যা cend = find_substr (svg, len, i + 9, "]]>", 3);
      যদি (cend < 0) ফেরত মিথ্যা;
      i = (unsigned) cend + 3;
      চালু;
    }

    যৌক্তিক closing = (i + 1 < len && svg[i + 1] == '/');
    যৌক্তিক special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    unsigned gt = i + 1;
    অক্ষর quote = 0;
    যখন (gt < len)
    {
      অক্ষর c = svg[gt];
      যদি (quote)
      {
        যদি (c == quote) quote = 0;
      }
      অন্যথা
      {
        যদি (c == '"' || c == '\'')
          quote = c;
        অন্যথা যদি (c == '>')
          বিরতি;
      }
      gt++;
    }
    যদি (gt >= len)
      ফেরত মিথ্যা;

    যদি (special)
    {
      i = gt + 1;
      চালু;
    }

    unsigned p = i + (closing ? 2 : 1);
    যখন (p < gt && isspace ((unsigned অক্ষর) svg[p])) p++;
    ধ্রুবক অক্ষর *name = svg + p;
    unsigned name_len = 0;
    যখন (p + name_len < gt)
    {
      unsigned অক্ষর c = (unsigned অক্ষর) name[name_len];
      যদি (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        বিরতি;
      name_len++;
    }
    যৌক্তিক is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    যদি (closing)
    {
      যদি (!depth)
      {
        i = gt + 1;
        চালু;
      }

      open_elem_t e = স্তূপ[--depth];
      unsigned end = gt + 1;

      যদি (e.id.len)
      {
        যদি (unlikely (!id_entries->ঠেলো_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          ফেরত মিথ্যা;

        যদি (e.in_defs_content)
        {
          যদি (unlikely (!defs_entries->ঠেলো_or_fail ()))
            ফেরত মিথ্যা;
          auto &slot = defs_entries->লেজ ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        যদি (parse_glyph_id_span (e.id, &gid))
        {
          যদি (unlikely (!glyph_spans->ঠেলো_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ফেরত মিথ্যা;
        }
      }

      যদি (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      চালু;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    unsigned r = gt;
    যখন (r > i && isspace ((unsigned অক্ষর) svg[r - 1])) r--;
    যৌক্তিক self_closing = (r > i && svg[r - 1] == '/');

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
          ফেরত মিথ্যা;

        যদি (e.in_defs_content)
        {
          যদি (unlikely (!defs_entries->ঠেলো_or_fail ()))
            ফেরত মিথ্যা;
          auto &slot = defs_entries->লেজ ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        যদি (parse_glyph_id_span (e.id, &gid))
        {
          যদি (unlikely (!glyph_spans->ঠেলো_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            ফেরত মিথ্যা;
        }
      }
    }
    অন্যথা
    {
      যদি (unlikely (depth >= MAX_DEPTH))
        ফেরত মিথ্যা;
      স্তূপ[depth++] = e;
      যদি (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  ফেরত সত্য;
}

} /* namespace _hb_svg_cache_impl */

অন্তঃরেখ
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  unsigned doc_গুনতি = table->get_document_গুনতি ();
  যদি (doc_গুনতি && unlikely (!doc_caches.resize (doc_গুনতি)))
    doc_caches.clear ();
  জন্য (unsigned i = 0; i < doc_caches.দৈর্ঘ্য; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

অন্তঃরেখ
SVG::accelerator_t::~accelerator_t ()
{
  জন্য (unsigned i = 0; i < doc_caches.দৈর্ঘ্য; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

অন্তঃরেখ শূন্য
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  যদি (!doc)
    ফেরত;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

অন্তঃরেখ SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ধ্রুবক অক্ষর *svg,
                                    unsigned len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) ধ্রুবক
{
  স্থির ধ্রুবক uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  auto *doc = (svg_doc_cache_t *) hb_malloc (আকার (svg_doc_cache_t));
  যদি (!doc)
    ফেরত nullptr;

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
    ফেরত nullptr;
  }

  unsigned glyph_গুনতি = end_glyph - start_glyph + 1;
  যদি (!doc->glyph_spans.resize ((পূর্ণসংখ্যা) glyph_গুনতি))
  {
    destroy_doc_cache (doc);
    ফেরত nullptr;
  }
  জন্য (unsigned i = 0; i < glyph_গুনতি; i++)
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
    ফেরত nullptr;
  }

  জন্য (unsigned i = 0; i < glyph_spans.দৈর্ঘ্য; i++)
  {
    ধ্রুবক auto &span = glyph_spans.arrayZ[i];
    যদি (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      চালু;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  জন্য (unsigned i = 0; i < id_entries.দৈর্ঘ্য; i++)
  {
    ধ্রুবক auto &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    যদি (doc->id_spans.has (e.id, &out))
      চালু;
    যদি (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      ফেরত nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  ফেরত doc;
}

অন্তঃরেখ ধ্রুবক SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ধ্রুবক অক্ষর *svg,
                                              unsigned len,
                                              unsigned doc_সূচক,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) ধ্রুবক
{
  যদি (doc_সূচক >= doc_caches.দৈর্ঘ্য)
    ফেরত nullptr;

  auto &slot = doc_caches.arrayZ[doc_সূচক];
  auto *doc = slot.get_acquire ();
  যদি (doc)
    ফেরত doc;

  auto *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  যদি (!fresh)
    ফেরত nullptr;

  auto *expected = (svg_doc_cache_t *) nullptr;
  যদি (slot.cmpexch (expected, fresh))
    ফেরত fresh;

  destroy_doc_cache (fresh);
  ফেরত expected;
}

অন্তঃরেখ ধ্রুবক অক্ষর *
SVG::accelerator_t::doc_cache_get_svg (ধ্রুবক svg_doc_cache_t *doc,
                                       unsigned *len) ধ্রুবক
{
  যদি (!doc)
  {
    যদি (len) *len = 0;
    ফেরত nullptr;
  }
  যদি (len) *len = doc->len;
  ফেরত doc->svg;
}

অন্তঃরেখ ধ্রুবক hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ধ্রুবক svg_doc_cache_t *doc) ধ্রুবক
{
  ফেরত doc ? &doc->defs_entries : nullptr;
}

অন্তঃরেখ যৌক্তিক
SVG::accelerator_t::doc_cache_get_glyph_span (ধ্রুবক svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              unsigned *start,
                                              unsigned *end) ধ্রুবক
{
  স্থির ধ্রুবক uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  যদি (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    ফেরত মিথ্যা;

  ধ্রুবক auto &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  যদি (span.first == INVALID_SPAN)
    ফেরত মিথ্যা;
  যদি (unlikely (span.first > span.second || span.second > doc->len))
    ফেরত মিথ্যা;

  যদি (start) *start = span.first;
  যদি (end) *end = span.second;
  ফেরত সত্য;
}

অন্তঃরেখ যৌক্তিক
SVG::accelerator_t::doc_cache_find_id_span (ধ্রুবক svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            unsigned *start,
                                            unsigned *end) ধ্রুবক
{
  যদি (!doc || !id.p || !id.len)
    ফেরত মিথ্যা;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  যদি (!doc->id_spans.has (id, &span))
    ফেরত মিথ্যা;
  যদি (unlikely (span->first > span->second || span->second > doc->len))
    ফেরত মিথ্যা;
  যদি (start) *start = span->first;
  যদি (end) *end = span->second;
  ফেরত সত্য;
}

অন্তঃরেখ যৌক্তিক
SVG::accelerator_t::doc_cache_find_id_cstr (ধ্রুবক svg_doc_cache_t *doc,
                                            ধ্রুবক অক্ষর *id,
                                            unsigned *start,
                                            unsigned *end) ধ্রুবক
{
  যদি (!id) ফেরত মিথ্যা;
  svg_id_span_t key = {id, (unsigned) strlen (id)};
  ফেরত doc_cache_find_id_span (doc, key, start, end);
}

কাঠামো SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
