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
                                     अचर वर्ण **svg,
                                     अचिन्हित *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  अचिन्हित data_len = 0;
  अचर वर्ण *data = hb_blob_get_data (blob, &data_len);

  यदि (!data || !data_len)
    गच्छ fail;

  यदि (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    यदि (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((माप_प्रकार) expected_size > (माप_प्रकार) HB_SVG_MAX_DOCUMENT_SIZE))
      गच्छ fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    यदि (!uncompressed)
      गच्छ fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    यदि (!data || !data_len)
      गच्छ fail;
  }

  यदि (unlikely ((माप_प्रकार) data_len > (माप_प्रकार) HB_SVG_MAX_DOCUMENT_SIZE))
    गच्छ fail;

  यदि (svg) *svg = data;
  यदि (len) *len = data_len;
  फर्काऊ blob;

fail:
  hb_blob_destroy (blob);
  यदि (svg) *svg = nullptr;
  यदि (len) *len = 0;
  फर्काऊ nullptr;
}

संरचना SVGDocumentIndexEntry
{
  पूर्णाङ्क cmp (hb_codepoint_t g) अचर
  { फर्काऊ g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () अचर
  { फर्काऊ startGlyphID; }

  hb_codepoint_t get_end_glyph () अचर
  { फर्काऊ endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, अचिन्हित पूर्णाङ्क index_offset) अचर
  {
    फर्काऊ hb_blob_create_sub_blob (svg_blob,
				    index_offset + (अचिन्हित पूर्णाङ्क) svgDoc,
				    svgDocLength);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
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
    अचर वर्ण *p;
    अचिन्हित len;

    तार्किक चालक == (अचर svg_id_span_t &o) अचर
    {
      फर्काऊ len == o.len && !स_भेद (p, o.p, len);
    }

    uint32_t hash () अचर
    {
      uint32_t h = hb_hash (len);
      लागि (अचिन्हित i = 0; i < len; i++)
        h = h * 33u + (अचिन्हित वर्ण) p[i];
      फर्काऊ h;
    }
  };

  संरचना svg_defs_entry_t
  {
    svg_id_span_t id;
    अचिन्हित start;
    अचिन्हित end;
  };

  संरचना svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    अचर वर्ण *svg = nullptr;
    अचिन्हित len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  तार्किक has_data () अचर { फर्काऊ svgDocEntries; }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) अचर
    {
      फर्काऊ table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    अचिन्हित get_document_गन्ती () अचर
    { फर्काऊ table->get_document_गन्ती (); }

    तार्किक get_glyph_document_index (hb_codepoint_t glyph_id, अचिन्हित *index) अचर
    { फर्काऊ table->get_glyph_document_index (glyph_id, index); }

    तार्किक get_document_glyph_range (अचिन्हित index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) अचर
    { फर्काऊ table->get_document_glyph_range (index, start_glyph, end_glyph); }

    तार्किक has_data () अचर { फर्काऊ table->has_data (); }

    अचर svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             अचर वर्ण *svg,
                             अचिन्हित len,
                             अचिन्हित doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) अचर;

    अचर वर्ण *
    doc_cache_get_svg (अचर svg_doc_cache_t *doc,
                       अचिन्हित *len) अचर;

    अचर hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (अचर svg_doc_cache_t *doc) अचर;

    तार्किक
    doc_cache_get_glyph_span (अचर svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              अचिन्हित *start,
                              अचिन्हित *end) अचर;

    तार्किक
    doc_cache_find_id_span (अचर svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            अचिन्हित *start,
                            अचिन्हित *end) अचर;

    तार्किक
    doc_cache_find_id_cstr (अचर svg_doc_cache_t *doc,
                            अचर वर्ण *id,
                            अचिन्हित *start,
                            अचिन्हित *end) अचर;

    तार्किक paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, शून्य *data) अचर
    {
      यदि (!has_data ())
        फर्काऊ असत्य;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      यदि (blob == hb_blob_get_empty ())
        फर्काऊ असत्य;

      तार्किक ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      फर्काऊ ret;
    }

    गुप्त:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    अचर वर्ण *svg,
                    अचिन्हित len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) अचर;

    स्थिर शून्य destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    सार्वजनिक:
    DEFINE_SIZE_STATIC (परिमाण (hb_blob_ptr_t<SVG>) +
                        परिमाण (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  अचर SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) अचर
  { फर्काऊ (एतत्+svgDocEntries).द्वा_खोज (glyph_id); }

  अचिन्हित get_document_गन्ती () अचर
  {
    यदि (!has_data ())
      फर्काऊ 0;
    फर्काऊ (एतत् + svgDocEntries).len;
  }

  तार्किक get_glyph_document_index (hb_codepoint_t glyph_id, अचिन्हित *index) अचर
  {
    यदि (!has_data ())
      फर्काऊ असत्य;
    फर्काऊ (एतत् + svgDocEntries).bfind (glyph_id, index);
  }

  तार्किक get_document_glyph_range (अचिन्हित index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) अचर
  {
    यदि (!has_data ())
      फर्काऊ असत्य;

    अचर स्वतः &entries = एतत् + svgDocEntries;
    यदि (index >= entries.len)
      फर्काऊ असत्य;

    अचर स्वतः &entry = entries.arrayZ[index];
    यदि (start_glyph) *start_glyph = entry.get_start_glyph ();
    यदि (end_glyph) *end_glyph = entry.get_end_glyph ();
    फर्काऊ सत्य;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  अचिन्हित start;
  SVG::svg_id_span_t id;
  तार्किक in_defs_content;
  तार्किक is_defs;
};

स्थिर अचर अचिन्हित MAX_DEPTH = 128;

स्थिर अन्तःस्थ पूर्णाङ्क
find_substr (अचर वर्ण *s,
             अचिन्हित n,
             अचिन्हित from,
             अचर वर्ण *needle,
             अचिन्हित needle_len)
{
  यदि (!needle_len || from >= n || needle_len > n)
    फर्काऊ -1;
  लागि (अचिन्हित i = from; i + needle_len <= n; i++)
    यदि (s[i] == needle[0] && !स_भेद (s + i, needle, needle_len))
      फर्काऊ (पूर्णाङ्क) i;
  फर्काऊ -1;
}

स्थिर अन्तःस्थ तार्किक
parse_id_in_start_tag (अचर वर्ण *svg,
                       अचिन्हित tag_start,
                       अचिन्हित tag_end,
                       SVG::svg_id_span_t *id)
{
  अचिन्हित p = tag_start;
  जबसम्म (p + 4 <= tag_end)
  {
    यदि (!स_भेद (svg + p, "id=\"", 4))
    {
      अचिन्हित b = p + 4;
      अचिन्हित e = b;
      जबसम्म (e < tag_end && svg[e] != '"') e++;
      यदि (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        फर्काऊ सत्य;
      }
    }
    यदि (!स_भेद (svg + p, "id='", 4))
    {
      अचिन्हित b = p + 4;
      अचिन्हित e = b;
      जबसम्म (e < tag_end && svg[e] != '\'') e++;
      यदि (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        फर्काऊ सत्य;
      }
    }
    p++;
  }
  फर्काऊ असत्य;
}

स्थिर अन्तःस्थ तार्किक
parse_glyph_id_span (अचर SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  यदि (id.len <= 5 || स_भेद (id.p, "glyph", 5))
    फर्काऊ असत्य;

  hb_codepoint_t gid = 0;
  लागि (अचिन्हित i = 5; i < id.len; i++)
  {
    अचिन्हित वर्ण c = (अचिन्हित वर्ण) id.p[i];
    यदि (c < '0' || c > '9')
      फर्काऊ असत्य;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    यदि (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      फर्काऊ असत्य;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  फर्काऊ सत्य;
}

स्थिर अन्तःस्थ तार्किक
parse_cache_entries_linear (अचर वर्ण *svg,
                            अचिन्हित len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t थाक[MAX_DEPTH] = {};
  अचिन्हित depth = 0;
  यदि (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    फर्काऊ असत्य;

  अचिन्हित defs_depth = 0;
  अचिन्हित i = 0;
  जबसम्म (i < len)
  {
    यदि (svg[i] != '<')
    {
      i++;
      जारी;
    }

    यदि (i + 4 <= len && !स_भेद (svg + i, "<!--", 4))
    {
      पूर्णाङ्क cend = find_substr (svg, len, i + 4, "-->", 3);
      यदि (cend < 0) फर्काऊ असत्य;
      i = (अचिन्हित) cend + 3;
      जारी;
    }
    यदि (i + 9 <= len && !स_भेद (svg + i, "<![CDATA[", 9))
    {
      पूर्णाङ्क cend = find_substr (svg, len, i + 9, "]]>", 3);
      यदि (cend < 0) फर्काऊ असत्य;
      i = (अचिन्हित) cend + 3;
      जारी;
    }

    तार्किक closing = (i + 1 < len && svg[i + 1] == '/');
    तार्किक special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    अचिन्हित gt = i + 1;
    वर्ण quote = 0;
    जबसम्म (gt < len)
    {
      वर्ण c = svg[gt];
      यदि (quote)
      {
        यदि (c == quote) quote = 0;
      }
      अन्यथा
      {
        यदि (c == '"' || c == '\'')
          quote = c;
        अन्यथा यदि (c == '>')
          रोक;
      }
      gt++;
    }
    यदि (gt >= len)
      फर्काऊ असत्य;

    यदि (special)
    {
      i = gt + 1;
      जारी;
    }

    अचिन्हित p = i + (closing ? 2 : 1);
    जबसम्म (p < gt && है_खाली ((अचिन्हित वर्ण) svg[p])) p++;
    अचर वर्ण *name = svg + p;
    अचिन्हित name_len = 0;
    जबसम्म (p + name_len < gt)
    {
      अचिन्हित वर्ण c = (अचिन्हित वर्ण) name[name_len];
      यदि (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        रोक;
      name_len++;
    }
    तार्किक is_defs = (name_len == 4 && !स_भेद (name, "defs", 4));

    यदि (closing)
    {
      यदि (!depth)
      {
        i = gt + 1;
        जारी;
      }

      open_elem_t e = थाक[--depth];
      अचिन्हित end = gt + 1;

      यदि (e.id.len)
      {
        यदि (unlikely (!id_entries->थप_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          फर्काऊ असत्य;

        यदि (e.in_defs_content)
        {
          यदि (unlikely (!defs_entries->थप_or_fail ()))
            फर्काऊ असत्य;
          स्वतः &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        यदि (parse_glyph_id_span (e.id, &gid))
        {
          यदि (unlikely (!glyph_spans->थप_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            फर्काऊ असत्य;
        }
      }

      यदि (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      जारी;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    अचिन्हित r = gt;
    जबसम्म (r > i && है_खाली ((अचिन्हित वर्ण) svg[r - 1])) r--;
    तार्किक self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    यदि (self_closing)
    {
      अचिन्हित end = gt + 1;
      यदि (e.id.len)
      {
        यदि (unlikely (!id_entries->थप_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          फर्काऊ असत्य;

        यदि (e.in_defs_content)
        {
          यदि (unlikely (!defs_entries->थप_or_fail ()))
            फर्काऊ असत्य;
          स्वतः &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        यदि (parse_glyph_id_span (e.id, &gid))
        {
          यदि (unlikely (!glyph_spans->थप_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            फर्काऊ असत्य;
        }
      }
    }
    अन्यथा
    {
      यदि (unlikely (depth >= MAX_DEPTH))
        फर्काऊ असत्य;
      थाक[depth++] = e;
      यदि (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  फर्काऊ सत्य;
}

} /* namespace _hb_svg_cache_impl */

अन्तःस्थ
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  अचिन्हित doc_गन्ती = table->get_document_गन्ती ();
  यदि (doc_गन्ती && unlikely (!doc_caches.resize (doc_गन्ती)))
    doc_caches.clear ();
  लागि (अचिन्हित i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

अन्तःस्थ
SVG::accelerator_t::~accelerator_t ()
{
  लागि (अचिन्हित i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

अन्तःस्थ शून्य
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  यदि (!doc)
    फर्काऊ;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

अन्तःस्थ SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    अचर वर्ण *svg,
                                    अचिन्हित len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) अचर
{
  स्थिर अचर uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  स्वतः *doc = (svg_doc_cache_t *) hb_malloc (परिमाण (svg_doc_cache_t));
  यदि (!doc)
    फर्काऊ nullptr;

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

  यदि (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    फर्काऊ nullptr;
  }

  अचिन्हित glyph_गन्ती = end_glyph - start_glyph + 1;
  यदि (!doc->glyph_spans.resize ((पूर्णाङ्क) glyph_गन्ती))
  {
    destroy_doc_cache (doc);
    फर्काऊ nullptr;
  }
  लागि (अचिन्हित i = 0; i < glyph_गन्ती; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  यदि (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    फर्काऊ nullptr;
  }

  लागि (अचिन्हित i = 0; i < glyph_spans.length; i++)
  {
    अचर स्वतः &span = glyph_spans.arrayZ[i];
    यदि (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      जारी;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  लागि (अचिन्हित i = 0; i < id_entries.length; i++)
  {
    अचर स्वतः &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    यदि (doc->id_spans.has (e.id, &out))
      जारी;
    यदि (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      फर्काऊ nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  फर्काऊ doc;
}

अन्तःस्थ अचर SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              अचर वर्ण *svg,
                                              अचिन्हित len,
                                              अचिन्हित doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) अचर
{
  यदि (doc_index >= doc_caches.length)
    फर्काऊ nullptr;

  स्वतः &slot = doc_caches.arrayZ[doc_index];
  स्वतः *doc = slot.get_acquire ();
  यदि (doc)
    फर्काऊ doc;

  स्वतः *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  यदि (!fresh)
    फर्काऊ nullptr;

  स्वतः *expected = (svg_doc_cache_t *) nullptr;
  यदि (slot.cmpexch (expected, fresh))
    फर्काऊ fresh;

  destroy_doc_cache (fresh);
  फर्काऊ expected;
}

अन्तःस्थ अचर वर्ण *
SVG::accelerator_t::doc_cache_get_svg (अचर svg_doc_cache_t *doc,
                                       अचिन्हित *len) अचर
{
  यदि (!doc)
  {
    यदि (len) *len = 0;
    फर्काऊ nullptr;
  }
  यदि (len) *len = doc->len;
  फर्काऊ doc->svg;
}

अन्तःस्थ अचर hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (अचर svg_doc_cache_t *doc) अचर
{
  फर्काऊ doc ? &doc->defs_entries : nullptr;
}

अन्तःस्थ तार्किक
SVG::accelerator_t::doc_cache_get_glyph_span (अचर svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              अचिन्हित *start,
                                              अचिन्हित *end) अचर
{
  स्थिर अचर uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  यदि (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    फर्काऊ असत्य;

  अचर स्वतः &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  यदि (span.first == INVALID_SPAN)
    फर्काऊ असत्य;
  यदि (unlikely (span.first > span.second || span.second > doc->len))
    फर्काऊ असत्य;

  यदि (start) *start = span.first;
  यदि (end) *end = span.second;
  फर्काऊ सत्य;
}

अन्तःस्थ तार्किक
SVG::accelerator_t::doc_cache_find_id_span (अचर svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            अचिन्हित *start,
                                            अचिन्हित *end) अचर
{
  यदि (!doc || !id.p || !id.len)
    फर्काऊ असत्य;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  यदि (!doc->id_spans.has (id, &span))
    फर्काऊ असत्य;
  यदि (unlikely (span->first > span->second || span->second > doc->len))
    फर्काऊ असत्य;
  यदि (start) *start = span->first;
  यदि (end) *end = span->second;
  फर्काऊ सत्य;
}

अन्तःस्थ तार्किक
SVG::accelerator_t::doc_cache_find_id_cstr (अचर svg_doc_cache_t *doc,
                                            अचर वर्ण *id,
                                            अचिन्हित *start,
                                            अचिन्हित *end) अचर
{
  यदि (!id) फर्काऊ असत्य;
  svg_id_span_t key = {id, (अचिन्हित) म_माप (id)};
  फर्काऊ doc_cache_find_id_span (doc, key, start, end);
}

संरचना SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
