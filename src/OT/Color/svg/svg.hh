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

स्थिर ओळीत hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     स्थिरांक अक्षर **svg,
                                     अचिन्हित *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  अचिन्हित data_len = 0;
  स्थिरांक अक्षर *data = hb_blob_get_data (blob, &data_len);

  जर (!data || !data_len)
    जा fail;

  जर (hb_blob_is_gzip (data, data_len))
  {
    uint32_t expected_size = 0;
    जर (hb_gzip_get_uncompressed_size (data, data_len, &expected_size) &&
        unlikely ((माप_प्रकार) expected_size > (माप_प्रकार) HB_SVG_MAX_DOCUMENT_SIZE))
      जा fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_SIZE);
    जर (!uncompressed)
      जा fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    data = hb_blob_get_data (blob, &data_len);
    जर (!data || !data_len)
      जा fail;
  }

  जर (unlikely ((माप_प्रकार) data_len > (माप_प्रकार) HB_SVG_MAX_DOCUMENT_SIZE))
    जा fail;

  जर (svg) *svg = data;
  जर (len) *len = data_len;
  परत blob;

fail:
  hb_blob_destroy (blob);
  जर (svg) *svg = nullptr;
  जर (len) *len = 0;
  परत nullptr;
}

संरचना SVGDocumentIndexEntry
{
  पूर्णांक cmp (hb_codepoint_t g) स्थिरांक
  { परत g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () स्थिरांक
  { परत startGlyphID; }

  hb_codepoint_t get_end_glyph () स्थिरांक
  { परत endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, अचिन्हित पूर्णांक index_offset) स्थिरांक
  {
    परत hb_blob_create_sub_blob (svg_blob,
				    index_offset + (अचिन्हित पूर्णांक) svgDoc,
				    svgDocLength);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svgDocLength));
  }

  संरक्षित:
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
    स्थिरांक अक्षर *p;
    अचिन्हित len;

    तार्किक चालक == (स्थिरांक svg_id_span_t &o) स्थिरांक
    {
      परत len == o.len && !स_भेद (p, o.p, len);
    }

    uint32_t hash () स्थिरांक
    {
      uint32_t h = hb_hash (len);
      साठी (अचिन्हित i = 0; i < len; i++)
        h = h * 33u + (अचिन्हित अक्षर) p[i];
      परत h;
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
    स्थिरांक अक्षर *svg = nullptr;
    अचिन्हित len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  तार्किक has_data () स्थिरांक { परत svgDocEntries; }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) स्थिरांक
    {
      परत table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    अचिन्हित get_document_मोजणी () स्थिरांक
    { परत table->get_document_मोजणी (); }

    तार्किक get_glyph_document_index (hb_codepoint_t glyph_id, अचिन्हित *index) स्थिरांक
    { परत table->get_glyph_document_index (glyph_id, index); }

    तार्किक get_document_glyph_range (अचिन्हित index,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) स्थिरांक
    { परत table->get_document_glyph_range (index, start_glyph, end_glyph); }

    तार्किक has_data () स्थिरांक { परत table->has_data (); }

    स्थिरांक svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             स्थिरांक अक्षर *svg,
                             अचिन्हित len,
                             अचिन्हित doc_index,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) स्थिरांक;

    स्थिरांक अक्षर *
    doc_cache_get_svg (स्थिरांक svg_doc_cache_t *doc,
                       अचिन्हित *len) स्थिरांक;

    स्थिरांक hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (स्थिरांक svg_doc_cache_t *doc) स्थिरांक;

    तार्किक
    doc_cache_get_glyph_span (स्थिरांक svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              अचिन्हित *start,
                              अचिन्हित *end) स्थिरांक;

    तार्किक
    doc_cache_find_id_span (स्थिरांक svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            अचिन्हित *start,
                            अचिन्हित *end) स्थिरांक;

    तार्किक
    doc_cache_find_id_cstr (स्थिरांक svg_doc_cache_t *doc,
                            स्थिरांक अक्षर *id,
                            अचिन्हित *start,
                            अचिन्हित *end) स्थिरांक;

    तार्किक paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, शून्य *data) स्थिरांक
    {
      जर (!has_data ())
        परत असत्य;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      जर (blob == hb_blob_get_empty ())
        परत असत्य;

      तार्किक ret = funcs->image (data,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      परत ret;
    }

    खाजगी:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    स्थिरांक अक्षर *svg,
                    अचिन्हित len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) स्थिरांक;

    स्थिर शून्य destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    सार्वजनिक:
    DEFINE_SIZE_STATIC (आकार (hb_blob_ptr_t<SVG>) +
                        आकार (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  स्थिरांक SVGDocumentIndexEntry &get_glyph_entry (hb_codepoint_t glyph_id) स्थिरांक
  { परत (हे+svgDocEntries).द्वा_खोज (glyph_id); }

  अचिन्हित get_document_मोजणी () स्थिरांक
  {
    जर (!has_data ())
      परत 0;
    परत (हे + svgDocEntries).len;
  }

  तार्किक get_glyph_document_index (hb_codepoint_t glyph_id, अचिन्हित *index) स्थिरांक
  {
    जर (!has_data ())
      परत असत्य;
    परत (हे + svgDocEntries).bfind (glyph_id, index);
  }

  तार्किक get_document_glyph_range (अचिन्हित index,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) स्थिरांक
  {
    जर (!has_data ())
      परत असत्य;

    स्थिरांक स्वतः &entries = हे + svgDocEntries;
    जर (index >= entries.len)
      परत असत्य;

    स्थिरांक स्वतः &entry = entries.arrayZ[index];
    जर (start_glyph) *start_glyph = entry.get_start_glyph ();
    जर (end_glyph) *end_glyph = entry.get_end_glyph ();
    परत सत्य;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  (हे+svgDocEntries).sanitize_shallow (c)));
  }

  संरक्षित:
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

स्थिर स्थिरांक अचिन्हित MAX_DEPTH = 128;

स्थिर ओळीत पूर्णांक
find_substr (स्थिरांक अक्षर *s,
             अचिन्हित n,
             अचिन्हित from,
             स्थिरांक अक्षर *needle,
             अचिन्हित needle_len)
{
  जर (!needle_len || from >= n || needle_len > n)
    परत -1;
  साठी (अचिन्हित i = from; i + needle_len <= n; i++)
    जर (s[i] == needle[0] && !स_भेद (s + i, needle, needle_len))
      परत (पूर्णांक) i;
  परत -1;
}

स्थिर ओळीत तार्किक
parse_id_in_start_tag (स्थिरांक अक्षर *svg,
                       अचिन्हित tag_start,
                       अचिन्हित tag_end,
                       SVG::svg_id_span_t *id)
{
  अचिन्हित p = tag_start;
  पर्यंत (p + 4 <= tag_end)
  {
    जर (!स_भेद (svg + p, "id=\"", 4))
    {
      अचिन्हित b = p + 4;
      अचिन्हित e = b;
      पर्यंत (e < tag_end && svg[e] != '"') e++;
      जर (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        परत सत्य;
      }
    }
    जर (!स_भेद (svg + p, "id='", 4))
    {
      अचिन्हित b = p + 4;
      अचिन्हित e = b;
      पर्यंत (e < tag_end && svg[e] != '\'') e++;
      जर (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        परत सत्य;
      }
    }
    p++;
  }
  परत असत्य;
}

स्थिर ओळीत तार्किक
parse_glyph_id_span (स्थिरांक SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  जर (id.len <= 5 || स_भेद (id.p, "glyph", 5))
    परत असत्य;

  hb_codepoint_t gid = 0;
  साठी (अचिन्हित i = 5; i < id.len; i++)
  {
    अचिन्हित अक्षर c = (अचिन्हित अक्षर) id.p[i];
    जर (c < '0' || c > '9')
      परत असत्य;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    जर (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      परत असत्य;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  परत सत्य;
}

स्थिर ओळीत तार्किक
parse_cache_entries_linear (स्थिरांक अक्षर *svg,
                            अचिन्हित len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t चळत[MAX_DEPTH] = {};
  अचिन्हित depth = 0;
  जर (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    परत असत्य;

  अचिन्हित defs_depth = 0;
  अचिन्हित i = 0;
  पर्यंत (i < len)
  {
    जर (svg[i] != '<')
    {
      i++;
      सुरूठेवा;
    }

    जर (i + 4 <= len && !स_भेद (svg + i, "<!--", 4))
    {
      पूर्णांक cend = find_substr (svg, len, i + 4, "-->", 3);
      जर (cend < 0) परत असत्य;
      i = (अचिन्हित) cend + 3;
      सुरूठेवा;
    }
    जर (i + 9 <= len && !स_भेद (svg + i, "<![CDATA[", 9))
    {
      पूर्णांक cend = find_substr (svg, len, i + 9, "]]>", 3);
      जर (cend < 0) परत असत्य;
      i = (अचिन्हित) cend + 3;
      सुरूठेवा;
    }

    तार्किक closing = (i + 1 < len && svg[i + 1] == '/');
    तार्किक special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    अचिन्हित gt = i + 1;
    अक्षर quote = 0;
    पर्यंत (gt < len)
    {
      अक्षर c = svg[gt];
      जर (quote)
      {
        जर (c == quote) quote = 0;
      }
      अन्यथा
      {
        जर (c == '"' || c == '\'')
          quote = c;
        अन्यथा जर (c == '>')
          थांबा;
      }
      gt++;
    }
    जर (gt >= len)
      परत असत्य;

    जर (special)
    {
      i = gt + 1;
      सुरूठेवा;
    }

    अचिन्हित p = i + (closing ? 2 : 1);
    पर्यंत (p < gt && है_खाली ((अचिन्हित अक्षर) svg[p])) p++;
    स्थिरांक अक्षर *name = svg + p;
    अचिन्हित name_len = 0;
    पर्यंत (p + name_len < gt)
    {
      अचिन्हित अक्षर c = (अचिन्हित अक्षर) name[name_len];
      जर (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        थांबा;
      name_len++;
    }
    तार्किक is_defs = (name_len == 4 && !स_भेद (name, "defs", 4));

    जर (closing)
    {
      जर (!depth)
      {
        i = gt + 1;
        सुरूठेवा;
      }

      open_elem_t e = चळत[--depth];
      अचिन्हित end = gt + 1;

      जर (e.id.len)
      {
        जर (unlikely (!id_entries->ढकला_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          परत असत्य;

        जर (e.in_defs_content)
        {
          जर (unlikely (!defs_entries->ढकला_or_fail ()))
            परत असत्य;
          स्वतः &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        जर (parse_glyph_id_span (e.id, &gid))
        {
          जर (unlikely (!glyph_spans->ढकला_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            परत असत्य;
        }
      }

      जर (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      सुरूठेवा;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    अचिन्हित r = gt;
    पर्यंत (r > i && है_खाली ((अचिन्हित अक्षर) svg[r - 1])) r--;
    तार्किक self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    जर (self_closing)
    {
      अचिन्हित end = gt + 1;
      जर (e.id.len)
      {
        जर (unlikely (!id_entries->ढकला_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          परत असत्य;

        जर (e.in_defs_content)
        {
          जर (unlikely (!defs_entries->ढकला_or_fail ()))
            परत असत्य;
          स्वतः &slot = defs_entries->tail ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        जर (parse_glyph_id_span (e.id, &gid))
        {
          जर (unlikely (!glyph_spans->ढकला_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            परत असत्य;
        }
      }
    }
    अन्यथा
    {
      जर (unlikely (depth >= MAX_DEPTH))
        परत असत्य;
      चळत[depth++] = e;
      जर (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  परत सत्य;
}

} /* namespace _hb_svg_cache_impl */

ओळीत
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  अचिन्हित doc_मोजणी = table->get_document_मोजणी ();
  जर (doc_मोजणी && unlikely (!doc_caches.resize (doc_मोजणी)))
    doc_caches.clear ();
  साठी (अचिन्हित i = 0; i < doc_caches.length; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

ओळीत
SVG::accelerator_t::~accelerator_t ()
{
  साठी (अचिन्हित i = 0; i < doc_caches.length; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

ओळीत शून्य
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  जर (!doc)
    परत;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

ओळीत SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    स्थिरांक अक्षर *svg,
                                    अचिन्हित len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) स्थिरांक
{
  स्थिर स्थिरांक uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  स्वतः *doc = (svg_doc_cache_t *) hb_malloc (आकार (svg_doc_cache_t));
  जर (!doc)
    परत nullptr;

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

  जर (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    परत nullptr;
  }

  अचिन्हित glyph_मोजणी = end_glyph - start_glyph + 1;
  जर (!doc->glyph_spans.resize ((पूर्णांक) glyph_मोजणी))
  {
    destroy_doc_cache (doc);
    परत nullptr;
  }
  साठी (अचिन्हित i = 0; i < glyph_मोजणी; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  जर (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    परत nullptr;
  }

  साठी (अचिन्हित i = 0; i < glyph_spans.length; i++)
  {
    स्थिरांक स्वतः &span = glyph_spans.arrayZ[i];
    जर (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      सुरूठेवा;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  साठी (अचिन्हित i = 0; i < id_entries.length; i++)
  {
    स्थिरांक स्वतः &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    जर (doc->id_spans.has (e.id, &out))
      सुरूठेवा;
    जर (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      परत nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  परत doc;
}

ओळीत स्थिरांक SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              स्थिरांक अक्षर *svg,
                                              अचिन्हित len,
                                              अचिन्हित doc_index,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) स्थिरांक
{
  जर (doc_index >= doc_caches.length)
    परत nullptr;

  स्वतः &slot = doc_caches.arrayZ[doc_index];
  स्वतः *doc = slot.get_acquire ();
  जर (doc)
    परत doc;

  स्वतः *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  जर (!fresh)
    परत nullptr;

  स्वतः *expected = (svg_doc_cache_t *) nullptr;
  जर (slot.cmpexch (expected, fresh))
    परत fresh;

  destroy_doc_cache (fresh);
  परत expected;
}

ओळीत स्थिरांक अक्षर *
SVG::accelerator_t::doc_cache_get_svg (स्थिरांक svg_doc_cache_t *doc,
                                       अचिन्हित *len) स्थिरांक
{
  जर (!doc)
  {
    जर (len) *len = 0;
    परत nullptr;
  }
  जर (len) *len = doc->len;
  परत doc->svg;
}

ओळीत स्थिरांक hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (स्थिरांक svg_doc_cache_t *doc) स्थिरांक
{
  परत doc ? &doc->defs_entries : nullptr;
}

ओळीत तार्किक
SVG::accelerator_t::doc_cache_get_glyph_span (स्थिरांक svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              अचिन्हित *start,
                                              अचिन्हित *end) स्थिरांक
{
  स्थिर स्थिरांक uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  जर (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    परत असत्य;

  स्थिरांक स्वतः &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  जर (span.first == INVALID_SPAN)
    परत असत्य;
  जर (unlikely (span.first > span.second || span.second > doc->len))
    परत असत्य;

  जर (start) *start = span.first;
  जर (end) *end = span.second;
  परत सत्य;
}

ओळीत तार्किक
SVG::accelerator_t::doc_cache_find_id_span (स्थिरांक svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            अचिन्हित *start,
                                            अचिन्हित *end) स्थिरांक
{
  जर (!doc || !id.p || !id.len)
    परत असत्य;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  जर (!doc->id_spans.has (id, &span))
    परत असत्य;
  जर (unlikely (span->first > span->second || span->second > doc->len))
    परत असत्य;
  जर (start) *start = span->first;
  जर (end) *end = span->second;
  परत सत्य;
}

ओळीत तार्किक
SVG::accelerator_t::doc_cache_find_id_cstr (स्थिरांक svg_doc_cache_t *doc,
                                            स्थिरांक अक्षर *id,
                                            अचिन्हित *start,
                                            अचिन्हित *end) स्थिरांक
{
  जर (!id) परत असत्य;
  svg_id_span_t key = {id, (अचिन्हित) म_माप (id)};
  परत doc_cache_find_id_span (doc, key, start, end);
}

संरचना SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* OT_COLOR_SVG_SVG_HH */
