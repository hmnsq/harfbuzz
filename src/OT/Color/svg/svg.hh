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

#अगर_अघोषित OT_COLOR_SVG_SVG_HH
#घोषणा OT_COLOR_SVG_SVG_HH

#समावेश "../../../hb-open-type.hh"
#समावेश "../../../hb-blob.hh"
#समावेश "../../../hb-limits.hh"
#समावेश "../../../hb-map.hh"
#समावेश "../../../hb-paint.hh"
#समावेश "../../../hb-zlib.hh"
#समावेश <ctype.h>
#समावेश <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#घोषणा HB_OT_TAG_SVG HB_TAG('S','V','G',' ')


namespace OT {

अटल अंतरभूत hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     स्थिर अक्षर **svg,
                                     अचिन्हित *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  अचिन्हित तथ्य_len = 0;
  स्थिर अक्षर *तथ्य = hb_blob_get_तथ्य (blob, &तथ्य_len);

  अगर (!तथ्य || !तथ्य_len)
    जाओ हार;

  अगर (hb_blob_is_gzip (तथ्य, तथ्य_len))
  {
    uint32_t expected_आकार = 0;
    अगर (hb_gzip_get_uncompressed_आकार (तथ्य, तथ्य_len, &expected_आकार) &&
        unlikely ((माप_प्रकार) expected_आकार > (माप_प्रकार) HB_SVG_MAX_DOCUMENT_आकार))
      जाओ हार;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_आकार);
    अगर (!uncompressed)
      जाओ हार;

    hb_blob_destroy (blob);
    blob = uncompressed;
    तथ्य = hb_blob_get_तथ्य (blob, &तथ्य_len);
    अगर (!तथ्य || !तथ्य_len)
      जाओ हार;
  }

  अगर (unlikely ((माप_प्रकार) तथ्य_len > (माप_प्रकार) HB_SVG_MAX_DOCUMENT_आकार))
    जाओ हार;

  अगर (svg) *svg = तथ्य;
  अगर (len) *len = तथ्य_len;
  वापस blob;

हार:
  hb_blob_destroy (blob);
  अगर (svg) *svg = nullptr;
  अगर (len) *len = 0;
  वापस nullptr;
}

काष्ठा SVGDocument_सूचकांक_Entry
{
  पूर्णांक cmp (hb_codepoint_t g) स्थिर
  { वापस g < startGlyphID ? -1 : g > endGlyphID ? 1 : 0; }

  hb_codepoint_t get_start_glyph () स्थिर
  { वापस startGlyphID; }

  hb_codepoint_t get_end_glyph () स्थिर
  { वापस endGlyphID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, अचिन्हित पूर्णांक सूचकांक_offset) स्थिर
  {
    वापस hb_blob_create_sub_blob (svg_blob,
				    सूचकांक_offset + (अचिन्हित पूर्णांक) svgDoc,
				    svg_Doc_लंबाई);
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, base, svg_Doc_लंबाई));
  }

  रक्षित:
  HBUINT16	startGlyphID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	endGlyphID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32To<UnsizedArrayOf<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svg_Doc_लंबाई;	/* Length of the SVG document.
				 * Must be non-zero. */
  खुला:
  DEFINE_आकार_STATIC (12);
};

काष्ठा SVG
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_SVG;

  काष्ठा svg_id_span_t
  {
    स्थिर अक्षर *p;
    अचिन्हित len;

    bool चालक == (स्थिर svg_id_span_t &o) स्थिर
    {
      वापस len == o.len && !स_भेद (p, o.p, len);
    }

    uint32_t hash () स्थिर
    {
      uint32_t h = hb_hash (len);
      क्रम (अचिन्हित i = 0; i < len; i++)
        h = h * 33u + (अचिन्हित अक्षर) p[i];
      वापस h;
    }
  };

  काष्ठा svg_defs_entry_t
  {
    svg_id_span_t id;
    अचिन्हित start;
    अचिन्हित end;
  };

  काष्ठा svg_doc_cache_t
  {
    hb_blob_t *blob = nullptr;
    स्थिर अक्षर *svg = nullptr;
    अचिन्हित len = 0;
    hb_vector_t<svg_defs_entry_t> defs_entries;
    hb_codepoint_t start_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t end_glyph = HB_CODEPOINT_INVALID;
    hb_vector_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_span_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  bool has_तथ्य () स्थिर { वापस svgDocEntries; }

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) स्थिर
    {
      वापस table->get_glyph_entry (glyph_id).reference_blob (table.get_blob (),
							       table->svgDocEntries);
    }

    अचिन्हित get_document_गिनती () स्थिर
    { वापस table->get_document_गिनती (); }

    bool get_glyph_document_सूचकांक (hb_codepoint_t glyph_id, अचिन्हित *सूचकांक_2) स्थिर
    { वापस table->get_glyph_document_सूचकांक (glyph_id, सूचकांक_2); }

    bool get_document_glyph_range (अचिन्हित सूचकांक_2,
                                   hb_codepoint_t *start_glyph,
                                   hb_codepoint_t *end_glyph) स्थिर
    { वापस table->get_document_glyph_range (सूचकांक_2, start_glyph, end_glyph); }

    bool has_तथ्य () स्थिर { वापस table->has_तथ्य (); }

    स्थिर svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             स्थिर अक्षर *svg,
                             अचिन्हित len,
                             अचिन्हित doc_सूचकांक,
                             hb_codepoint_t start_glyph,
                             hb_codepoint_t end_glyph) स्थिर;

    स्थिर अक्षर *
    doc_cache_get_svg (स्थिर svg_doc_cache_t *doc,
                       अचिन्हित *len) स्थिर;

    स्थिर hb_vector_t<svg_defs_entry_t> *
    doc_cache_get_defs_entries (स्थिर svg_doc_cache_t *doc) स्थिर;

    bool
    doc_cache_get_glyph_span (स्थिर svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              अचिन्हित *start,
                              अचिन्हित *end) स्थिर;

    bool
    doc_cache_find_id_span (स्थिर svg_doc_cache_t *doc,
                            svg_id_span_t id,
                            अचिन्हित *start,
                            अचिन्हित *end) स्थिर;

    bool
    doc_cache_find_id_cstr (स्थिर svg_doc_cache_t *doc,
                            स्थिर अक्षर *id,
                            अचिन्हित *start,
                            अचिन्हित *end) स्थिर;

    bool paint_glyph (hb_font_t *font HB_UNUSED, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, व्योम *तथ्य) स्थिर
    {
      अगर (!has_तथ्य ())
        वापस false;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      अगर (blob == hb_blob_get_empty ())
        वापस false;

      bool ret = funcs->image (तथ्य,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       nullptr);

      hb_blob_destroy (blob);

      वापस ret;
    }

    निजी:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    स्थिर अक्षर *svg,
                    अचिन्हित len,
                    hb_codepoint_t start_glyph,
                    hb_codepoint_t end_glyph) स्थिर;

    अटल व्योम destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> table;
    mutable hb_vector_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    खुला:
    DEFINE_आकार_STATIC (माप (hb_blob_ptr_t<SVG>) +
                        माप (hb_vector_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  स्थिर SVGDocument_सूचकांक_Entry &get_glyph_entry (hb_codepoint_t glyph_id) स्थिर
  { वापस (यह+svgDocEntries).द्वा_खोज (glyph_id); }

  अचिन्हित get_document_गिनती () स्थिर
  {
    अगर (!has_तथ्य ())
      वापस 0;
    वापस (यह + svgDocEntries).len;
  }

  bool get_glyph_document_सूचकांक (hb_codepoint_t glyph_id, अचिन्हित *सूचकांक_2) स्थिर
  {
    अगर (!has_तथ्य ())
      वापस false;
    वापस (यह + svgDocEntries).bfind (glyph_id, सूचकांक_2);
  }

  bool get_document_glyph_range (अचिन्हित सूचकांक_2,
                                 hb_codepoint_t *start_glyph,
                                 hb_codepoint_t *end_glyph) स्थिर
  {
    अगर (!has_तथ्य ())
      वापस false;

    स्थिर स्वतः &entries = यह + svgDocEntries;
    अगर (सूचकांक_2 >= entries.len)
      वापस false;

    स्थिर स्वतः &entry = entries.arrayZ[सूचकांक_2];
    अगर (start_glyph) *start_glyph = entry.get_start_glyph ();
    अगर (end_glyph) *end_glyph = entry.get_end_glyph ();
    वापस true;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  (यह+svgDocEntries).sanitize_shallow (c)));
  }

  रक्षित:
  HBUINT16	version;	/* Table version (starting at 0). */
  Offset32To<SortedArray16Of<SVGDocument_सूचकांक_Entry>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  खुला:
  DEFINE_आकार_STATIC (10);
};

namespace _hb_svg_cache_impl {

काष्ठा glyph_entry_t
{
  hb_codepoint_t glyph;
  uint32_t start;
  uint32_t end;
};

काष्ठा id_entry_t
{
  SVG::svg_id_span_t id;
  uint32_t start;
  uint32_t end;
};

काष्ठा open_elem_t
{
  अचिन्हित start;
  SVG::svg_id_span_t id;
  bool in_defs_content;
  bool is_defs;
};

अटल स्थिर अचिन्हित MAX_DEPTH = 128;

अटल अंतरभूत पूर्णांक
find_substr (स्थिर अक्षर *s,
             अचिन्हित n,
             अचिन्हित from,
             स्थिर अक्षर *needle,
             अचिन्हित needle_len)
{
  अगर (!needle_len || from >= n || needle_len > n)
    वापस -1;
  क्रम (अचिन्हित i = from; i + needle_len <= n; i++)
    अगर (s[i] == needle[0] && !स_भेद (s + i, needle, needle_len))
      वापस (पूर्णांक) i;
  वापस -1;
}

अटल अंतरभूत bool
parse_id_in_start_tag (स्थिर अक्षर *svg,
                       अचिन्हित tag_start,
                       अचिन्हित tag_end,
                       SVG::svg_id_span_t *id)
{
  अचिन्हित p = tag_start;
  जबतक (p + 4 <= tag_end)
  {
    अगर (!स_भेद (svg + p, "id=\"", 4))
    {
      अचिन्हित b = p + 4;
      अचिन्हित e = b;
      जबतक (e < tag_end && svg[e] != '"') e++;
      अगर (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        वापस true;
      }
    }
    अगर (!स_भेद (svg + p, "id='", 4))
    {
      अचिन्हित b = p + 4;
      अचिन्हित e = b;
      जबतक (e < tag_end && svg[e] != '\'') e++;
      अगर (e <= tag_end && e > b)
      {
        *id = {svg + b, e - b};
        वापस true;
      }
    }
    p++;
  }
  वापस false;
}

अटल अंतरभूत bool
parse_glyph_id_span (स्थिर SVG::svg_id_span_t &id,
                     hb_codepoint_t *glyph)
{
  अगर (id.len <= 5 || स_भेद (id.p, "glyph", 5))
    वापस false;

  hb_codepoint_t gid = 0;
  क्रम (अचिन्हित i = 5; i < id.len; i++)
  {
    अचिन्हित अक्षर c = (अचिन्हित अक्षर) id.p[i];
    अगर (c < '0' || c > '9')
      वापस false;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    अगर (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      वापस false;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  वापस true;
}

अटल अंतरभूत bool
parse_cache_entries_linear (स्थिर अक्षर *svg,
                            अचिन्हित len,
                            hb_vector_t<SVG::svg_defs_entry_t> *defs_entries,
                            hb_vector_t<glyph_entry_t> *glyph_spans,
                            hb_vector_t<id_entry_t> *id_entries)
{
  open_elem_t ढेर[MAX_DEPTH] = {};
  अचिन्हित depth = 0;
  अगर (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    वापस false;

  अचिन्हित defs_depth = 0;
  अचिन्हित i = 0;
  जबतक (i < len)
  {
    अगर (svg[i] != '<')
    {
      i++;
      जारी;
    }

    अगर (i + 4 <= len && !स_भेद (svg + i, "<!--", 4))
    {
      पूर्णांक cend = find_substr (svg, len, i + 4, "-->", 3);
      अगर (cend < 0) वापस false;
      i = (अचिन्हित) cend + 3;
      जारी;
    }
    अगर (i + 9 <= len && !स_भेद (svg + i, "<![CDATA[", 9))
    {
      पूर्णांक cend = find_substr (svg, len, i + 9, "]]>", 3);
      अगर (cend < 0) वापस false;
      i = (अचिन्हित) cend + 3;
      जारी;
    }

    bool closing = (i + 1 < len && svg[i + 1] == '/');
    bool special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    अचिन्हित gt = i + 1;
    अक्षर quote = 0;
    जबतक (gt < len)
    {
      अक्षर c = svg[gt];
      अगर (quote)
      {
        अगर (c == quote) quote = 0;
      }
      अन्यथा
      {
        अगर (c == '"' || c == '\'')
          quote = c;
        अन्यथा अगर (c == '>')
          अवरोध;
      }
      gt++;
    }
    अगर (gt >= len)
      वापस false;

    अगर (special)
    {
      i = gt + 1;
      जारी;
    }

    अचिन्हित p = i + (closing ? 2 : 1);
    जबतक (p < gt && है_खाली ((अचिन्हित अक्षर) svg[p])) p++;
    स्थिर अक्षर *name = svg + p;
    अचिन्हित name_len = 0;
    जबतक (p + name_len < gt)
    {
      अचिन्हित अक्षर c = (अचिन्हित अक्षर) name[name_len];
      अगर (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        अवरोध;
      name_len++;
    }
    bool is_defs = (name_len == 4 && !स_भेद (name, "defs", 4));

    अगर (closing)
    {
      अगर (!depth)
      {
        i = gt + 1;
        जारी;
      }

      open_elem_t e = ढेर[--depth];
      अचिन्हित end = gt + 1;

      अगर (e.id.len)
      {
        अगर (unlikely (!id_entries->धकेलो_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          वापस false;

        अगर (e.in_defs_content)
        {
          अगर (unlikely (!defs_entries->धकेलो_or_fail ()))
            वापस false;
          स्वतः &slot = defs_entries->पुच्छ ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        अगर (parse_glyph_id_span (e.id, &gid))
        {
          अगर (unlikely (!glyph_spans->धकेलो_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            वापस false;
        }
      }

      अगर (e.is_defs && defs_depth)
        defs_depth--;

      i = end;
      जारी;
    }

    SVG::svg_id_span_t id = {};
    parse_id_in_start_tag (svg, i, gt, &id);

    अचिन्हित r = gt;
    जबतक (r > i && है_खाली ((अचिन्हित अक्षर) svg[r - 1])) r--;
    bool self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.start = i;
    e.id = id;
    e.in_defs_content = defs_depth > 0;
    e.is_defs = is_defs;

    अगर (self_closing)
    {
      अचिन्हित end = gt + 1;
      अगर (e.id.len)
      {
        अगर (unlikely (!id_entries->धकेलो_or_fail (id_entry_t {e.id, (uint32_t) e.start, (uint32_t) end})))
          वापस false;

        अगर (e.in_defs_content)
        {
          अगर (unlikely (!defs_entries->धकेलो_or_fail ()))
            वापस false;
          स्वतः &slot = defs_entries->पुच्छ ();
          slot.id = e.id;
          slot.start = e.start;
          slot.end = end;
        }

        hb_codepoint_t gid;
        अगर (parse_glyph_id_span (e.id, &gid))
        {
          अगर (unlikely (!glyph_spans->धकेलो_or_fail (glyph_entry_t {gid, (uint32_t) e.start, (uint32_t) end})))
            वापस false;
        }
      }
    }
    अन्यथा
    {
      अगर (unlikely (depth >= MAX_DEPTH))
        वापस false;
      ढेर[depth++] = e;
      अगर (is_defs)
        defs_depth++;
    }

    i = gt + 1;
  }

  वापस true;
}

} /* namespace _hb_svg_cache_impl */

अंतरभूत
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  table = hb_sanitize_context_t ().reference_table<SVG> (face);
  doc_caches.init ();
  अचिन्हित doc_गिनती = table->get_document_गिनती ();
  अगर (doc_गिनती && unlikely (!doc_caches.resize (doc_गिनती)))
    doc_caches.सफा ();
  क्रम (अचिन्हित i = 0; i < doc_caches.लंबाई; i++)
    doc_caches.arrayZ[i].set_relaxed (nullptr);
}

अंतरभूत
SVG::accelerator_t::~accelerator_t ()
{
  क्रम (अचिन्हित i = 0; i < doc_caches.लंबाई; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  table.destroy ();
}

अंतरभूत व्योम
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  अगर (!doc)
    वापस;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

अंतरभूत SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    स्थिर अक्षर *svg,
                                    अचिन्हित len,
                                    hb_codepoint_t start_glyph,
                                    hb_codepoint_t end_glyph) स्थिर
{
  अटल स्थिर uint32_t INVALID_SPAN = 0xFFFFFFFFu;

  स्वतः *doc = (svg_doc_cache_t *) hb_malloc (माप (svg_doc_cache_t));
  अगर (!doc)
    वापस nullptr;

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

  अगर (unlikely (start_glyph == HB_CODEPOINT_INVALID || end_glyph < start_glyph))
  {
    destroy_doc_cache (doc);
    वापस nullptr;
  }

  अचिन्हित glyph_गिनती = end_glyph - start_glyph + 1;
  अगर (!doc->glyph_spans.resize ((पूर्णांक) glyph_गिनती))
  {
    destroy_doc_cache (doc);
    वापस nullptr;
  }
  क्रम (अचिन्हित i = 0; i < glyph_गिनती; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_SPAN, INVALID_SPAN);

  hb_vector_t<_hb_svg_cache_impl::glyph_entry_t> glyph_spans;
  glyph_spans.init ();
  hb_vector_t<_hb_svg_cache_impl::id_entry_t> id_entries;
  id_entries.init ();
  अगर (!_hb_svg_cache_impl::parse_cache_entries_linear (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    वापस nullptr;
  }

  क्रम (अचिन्हित i = 0; i < glyph_spans.लंबाई; i++)
  {
    स्थिर स्वतः &span = glyph_spans.arrayZ[i];
    अगर (unlikely (span.glyph < start_glyph || span.glyph > end_glyph))
      जारी;
    doc->glyph_spans.arrayZ[span.glyph - start_glyph] = hb_pair_t<uint32_t, uint32_t> (span.start, span.end);
  }

  क्रम (अचिन्हित i = 0; i < id_entries.लंबाई; i++)
  {
    स्थिर स्वतः &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *out = nullptr;
    अगर (doc->id_spans.has (e.id, &out))
      जारी;
    अगर (unlikely (!doc->id_spans.set (e.id, hb_pair_t<uint32_t, uint32_t> (e.start, e.end))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      वापस nullptr;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  वापस doc;
}

अंतरभूत स्थिर SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              स्थिर अक्षर *svg,
                                              अचिन्हित len,
                                              अचिन्हित doc_सूचकांक,
                                              hb_codepoint_t start_glyph,
                                              hb_codepoint_t end_glyph) स्थिर
{
  अगर (doc_सूचकांक >= doc_caches.लंबाई)
    वापस nullptr;

  स्वतः &slot = doc_caches.arrayZ[doc_सूचकांक];
  स्वतः *doc = slot.get_acquire ();
  अगर (doc)
    वापस doc;

  स्वतः *fresh = make_doc_cache (image, svg, len, start_glyph, end_glyph);
  अगर (!fresh)
    वापस nullptr;

  स्वतः *expected = (svg_doc_cache_t *) nullptr;
  अगर (slot.cmpexch (expected, fresh))
    वापस fresh;

  destroy_doc_cache (fresh);
  वापस expected;
}

अंतरभूत स्थिर अक्षर *
SVG::accelerator_t::doc_cache_get_svg (स्थिर svg_doc_cache_t *doc,
                                       अचिन्हित *len) स्थिर
{
  अगर (!doc)
  {
    अगर (len) *len = 0;
    वापस nullptr;
  }
  अगर (len) *len = doc->len;
  वापस doc->svg;
}

अंतरभूत स्थिर hb_vector_t<SVG::svg_defs_entry_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (स्थिर svg_doc_cache_t *doc) स्थिर
{
  वापस doc ? &doc->defs_entries : nullptr;
}

अंतरभूत bool
SVG::accelerator_t::doc_cache_get_glyph_span (स्थिर svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              अचिन्हित *start,
                                              अचिन्हित *end) स्थिर
{
  अटल स्थिर uint32_t INVALID_SPAN = 0xFFFFFFFFu;
  अगर (!doc || doc->start_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->start_glyph || glyph > doc->end_glyph)
    वापस false;

  स्थिर स्वतः &span = doc->glyph_spans.arrayZ[glyph - doc->start_glyph];
  अगर (span.first == INVALID_SPAN)
    वापस false;
  अगर (unlikely (span.first > span.second || span.second > doc->len))
    वापस false;

  अगर (start) *start = span.first;
  अगर (end) *end = span.second;
  वापस true;
}

अंतरभूत bool
SVG::accelerator_t::doc_cache_find_id_span (स्थिर svg_doc_cache_t *doc,
                                            svg_id_span_t id,
                                            अचिन्हित *start,
                                            अचिन्हित *end) स्थिर
{
  अगर (!doc || !id.p || !id.len)
    वापस false;
  hb_pair_t<uint32_t, uint32_t> *span = nullptr;
  अगर (!doc->id_spans.has (id, &span))
    वापस false;
  अगर (unlikely (span->first > span->second || span->second > doc->len))
    वापस false;
  अगर (start) *start = span->first;
  अगर (end) *end = span->second;
  वापस true;
}

अंतरभूत bool
SVG::accelerator_t::doc_cache_find_id_cstr (स्थिर svg_doc_cache_t *doc,
                                            स्थिर अक्षर *id,
                                            अचिन्हित *start,
                                            अचिन्हित *end) स्थिर
{
  अगर (!id) वापस false;
  svg_id_span_t key = {id, (अचिन्हित) म_माप (id)};
  वापस doc_cache_find_id_span (doc, key, start, end);
}

काष्ठा SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#पूर्ण_अगर /* OT_COLOR_SVG_SVG_HH */
