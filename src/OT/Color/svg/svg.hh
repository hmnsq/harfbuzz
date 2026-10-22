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

#جيڪڏهن_وصف_نه_ٿيل OT_COLOR_SVG_SVG_HH
#وصف_ڪريو OT_COLOR_SVG_SVG_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-blob.hh"
#ضمن "../../../hb-limits.hh"
#ضمن "../../../hb-map.hh"
#ضمن "../../../hb-paint.hh"
#ضمن "../../../hb-zlib.hh"
#ضمن <ctype.h>
#ضمن <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#وصف_ڪريو HB_OT_ٽيگ_SVG HB_ٽيگ('S','V','G',' ')


نطاق OT {

ساكن مضمن hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ثابت حرف **svg,
                                     بنانشان *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  بنانشان معلومات_len = 0;
  ثابت حرف *معلومات = hb_blob_get_معلومات (blob, &معلومات_len);

  إذا (!معلومات || !معلومات_len)
    اذهب fail;

  إذا (hb_blob_is_gzip (معلومات, معلومات_len))
  {
    uint32_t expected_سائيز = 0;
    إذا (hb_gzip_get_uncompressed_سائيز (معلومات, معلومات_len, &expected_سائيز) &&
        unlikely ((size_t) expected_سائيز > (size_t) HB_SVG_MAX_DOCUMENT_سائيز))
      اذهب fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_سائيز);
    إذا (!uncompressed)
      اذهب fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    معلومات = hb_blob_get_معلومات (blob, &معلومات_len);
    إذا (!معلومات || !معلومات_len)
      اذهب fail;
  }

  إذا (unlikely ((size_t) معلومات_len > (size_t) HB_SVG_MAX_DOCUMENT_سائيز))
    اذهب fail;

  إذا (svg) *svg = معلومات;
  إذا (len) *len = معلومات_len;
  أرجع blob;

fail:
  hb_blob_destroy (blob);
  إذا (svg) *svg = خالي_اشارو;
  إذا (len) *len = 0;
  أرجع خالي_اشارو;
}

بنية SVGDocument_اشاريو_داخلا
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < شروعات_Glyph_ID ? -1 : g > پڇاڙي_Glyph_ID ? 1 : 0; }

  hb_codepoint_t get_شروعات_glyph () ثابت
  { أرجع شروعات_Glyph_ID; }

  hb_codepoint_t get_پڇاڙي_glyph () ثابت
  { أرجع پڇاڙي_Glyph_ID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, بنانشان صحيح اشاريو_وٿي) ثابت
  {
    أرجع hb_blob_create_sub_blob (svg_blob,
				    اشاريو_وٿي + (بنانشان صحيح) svgDoc,
				    svg_Doc_ڊيگهائي);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, بنياد, svg_Doc_ڊيگهائي));
  }

  محمي:
  HBUINT16	شروعات_Glyph_ID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	پڇاڙي_Glyph_ID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32_منزل<Unsized_Array_جو<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svg_Doc_ڊيگهائي;	/* Length of the SVG document.
				 * Must be non-zero. */
  عام:
  DEFINE_سائيز_STATIC (12);
};

بنية SVG
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_SVG;

  بنية svg_id_وسعت_t
  {
    ثابت حرف *p;
    بنانشان len;

    منطقي عامل == (ثابت svg_id_وسعت_t &o) ثابت
    {
      أرجع len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ثابت
    {
      uint32_t h = hb_hash (len);
      لكل (بنانشان i = 0; i < len; i++)
        h = h * 33u + (بنانشان حرف) p[i];
      أرجع h;
    }
  };

  بنية svg_defs_داخلا_t
  {
    svg_id_وسعت_t id;
    بنانشان شروعات;
    بنانشان پڇاڙي;
  };

  بنية svg_doc_cache_t
  {
    hb_blob_t *blob = خالي_اشارو;
    ثابت حرف *svg = خالي_اشارو;
    بنانشان len = 0;
    hb_ويڪٽر_t<svg_defs_داخلا_t> defs_entries;
    hb_codepoint_t شروعات_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t پڇاڙي_glyph = HB_CODEPOINT_INVALID;
    hb_ويڪٽر_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_وسعت_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  منطقي has_معلومات () ثابت { أرجع svgDocEntries; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ثابت
    {
      أرجع جدول->get_glyph_داخلا (glyph_id).reference_blob (جدول.get_blob (),
							       جدول->svgDocEntries);
    }

    بنانشان get_document_ڳڻپ () ثابت
    { أرجع جدول->get_document_ڳڻپ (); }

    منطقي get_glyph_document_اشاريو (hb_codepoint_t glyph_id, بنانشان *اشاريو_2) ثابت
    { أرجع جدول->get_glyph_document_اشاريو (glyph_id, اشاريو_2); }

    منطقي get_document_glyph_range (بنانشان اشاريو_2,
                                   hb_codepoint_t *شروعات_glyph,
                                   hb_codepoint_t *پڇاڙي_glyph) ثابت
    { أرجع جدول->get_document_glyph_range (اشاريو_2, شروعات_glyph, پڇاڙي_glyph); }

    منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

    ثابت svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ثابت حرف *svg,
                             بنانشان len,
                             بنانشان doc_اشاريو,
                             hb_codepoint_t شروعات_glyph,
                             hb_codepoint_t پڇاڙي_glyph) ثابت;

    ثابت حرف *
    doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                       بنانشان *len) ثابت;

    ثابت hb_ويڪٽر_t<svg_defs_داخلا_t> *
    doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت;

    منطقي
    doc_cache_get_glyph_وسعت (ثابت svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              بنانشان *شروعات,
                              بنانشان *پڇاڙي) ثابت;

    منطقي
    doc_cache_ڳوليو_id_وسعت (ثابت svg_doc_cache_t *doc,
                            svg_id_وسعت_t id,
                            بنانشان *شروعات,
                            بنانشان *پڇاڙي) ثابت;

    منطقي
    doc_cache_ڳوليو_id_cstr (ثابت svg_doc_cache_t *doc,
                            ثابت حرف *id,
                            بنانشان *شروعات,
                            بنانشان *پڇاڙي) ثابت;

    منطقي paint_glyph (hb_font_t *font HB_اڻ_استعمال_ٿيل, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *معلومات) ثابت
    {
      إذا (!has_معلومات ())
        أرجع خطأ;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      إذا (blob == hb_blob_get_خالي_جايون ())
        أرجع خطأ;

      منطقي ret = funcs->image (معلومات,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       خالي_اشارو);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ثابت حرف *svg,
                    بنانشان len,
                    hb_codepoint_t شروعات_glyph,
                    hb_codepoint_t پڇاڙي_glyph) ثابت;

    ساكن فراغ destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> جدول;
    بدلجندڙ hb_ويڪٽر_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    عام:
    DEFINE_سائيز_STATIC (حجم (hb_blob_ptr_t<SVG>) +
                        حجم (hb_ويڪٽر_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ثابت SVGDocument_اشاريو_داخلا &get_glyph_داخلا (hb_codepoint_t glyph_id) ثابت
  { أرجع (هذا+svgDocEntries).bsearch (glyph_id); }

  بنانشان get_document_ڳڻپ () ثابت
  {
    إذا (!has_معلومات ())
      أرجع 0;
    أرجع (هذا + svgDocEntries).len;
  }

  منطقي get_glyph_document_اشاريو (hb_codepoint_t glyph_id, بنانشان *اشاريو_2) ثابت
  {
    إذا (!has_معلومات ())
      أرجع خطأ;
    أرجع (هذا + svgDocEntries).bfind (glyph_id, اشاريو_2);
  }

  منطقي get_document_glyph_range (بنانشان اشاريو_2,
                                 hb_codepoint_t *شروعات_glyph,
                                 hb_codepoint_t *پڇاڙي_glyph) ثابت
  {
    إذا (!has_معلومات ())
      أرجع خطأ;

    ثابت خودڪار &entries = هذا + svgDocEntries;
    إذا (اشاريو_2 >= entries.len)
      أرجع خطأ;

    ثابت خودڪار &داخلا = entries.arrayZ[اشاريو_2];
    إذا (شروعات_glyph) *شروعات_glyph = داخلا.get_شروعات_glyph ();
    إذا (پڇاڙي_glyph) *پڇاڙي_glyph = داخلا.get_پڇاڙي_glyph ();
    أرجع سچ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  (هذا+svgDocEntries).sanitize_shallow (c)));
  }

  محمي:
  HBUINT16	نسخو;	/* Table version (starting at 0). */
  Offset32_منزل<Sorted_Array16_جو<SVGDocument_اشاريو_داخلا>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  عام:
  DEFINE_سائيز_STATIC (10);
};

نطاق _hb_svg_cache_impl {

بنية glyph_داخلا_t
{
  hb_codepoint_t glyph;
  uint32_t شروعات;
  uint32_t پڇاڙي;
};

بنية id_داخلا_t
{
  SVG::svg_id_وسعت_t id;
  uint32_t شروعات;
  uint32_t پڇاڙي;
};

بنية open_elem_t
{
  بنانشان شروعات;
  SVG::svg_id_وسعت_t id;
  منطقي اندريون_defs_content;
  منطقي is_defs;
};

ساكن ثابت بنانشان MAX_اونهائي = 128;

ساكن مضمن صحيح
ڳوليو_substr (ثابت حرف *s,
             بنانشان n,
             بنانشان from,
             ثابت حرف *needle,
             بنانشان needle_len)
{
  إذا (!needle_len || from >= n || needle_len > n)
    أرجع -1;
  لكل (بنانشان i = from; i + needle_len <= n; i++)
    إذا (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      أرجع (صحيح) i;
  أرجع -1;
}

ساكن مضمن منطقي
parse_id_اندريون_شروعات_ٽيگ (ثابت حرف *svg,
                       بنانشان ٽيگ_شروعات,
                       بنانشان ٽيگ_پڇاڙي,
                       SVG::svg_id_وسعت_t *id)
{
  بنانشان p = ٽيگ_شروعات;
  طالما (p + 4 <= ٽيگ_پڇاڙي)
  {
    إذا (!memcmp (svg + p, "id=\"", 4))
    {
      بنانشان b = p + 4;
      بنانشان e = b;
      طالما (e < ٽيگ_پڇاڙي && svg[e] != '"') e++;
      إذا (e <= ٽيگ_پڇاڙي && e > b)
      {
        *id = {svg + b, e - b};
        أرجع سچ;
      }
    }
    إذا (!memcmp (svg + p, "id='", 4))
    {
      بنانشان b = p + 4;
      بنانشان e = b;
      طالما (e < ٽيگ_پڇاڙي && svg[e] != '\'') e++;
      إذا (e <= ٽيگ_پڇاڙي && e > b)
      {
        *id = {svg + b, e - b};
        أرجع سچ;
      }
    }
    p++;
  }
  أرجع خطأ;
}

ساكن مضمن منطقي
parse_glyph_id_وسعت (ثابت SVG::svg_id_وسعت_t &id,
                     hb_codepoint_t *glyph)
{
  إذا (id.len <= 5 || memcmp (id.p, "glyph", 5))
    أرجع خطأ;

  hb_codepoint_t gid = 0;
  لكل (بنانشان i = 5; i < id.len; i++)
  {
    بنانشان حرف c = (بنانشان حرف) id.p[i];
    إذا (c < '0' || c > '9')
      أرجع خطأ;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    إذا (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      أرجع خطأ;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  أرجع سچ;
}

ساكن مضمن منطقي
parse_cache_entries_لڪيري (ثابت حرف *svg,
                            بنانشان len,
                            hb_ويڪٽر_t<SVG::svg_defs_داخلا_t> *defs_entries,
                            hb_ويڪٽر_t<glyph_داخلا_t> *glyph_spans,
                            hb_ويڪٽر_t<id_داخلا_t> *id_entries)
{
  open_elem_t ڍير[MAX_اونهائي] = {};
  بنانشان اونهائي = 0;
  إذا (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    أرجع خطأ;

  بنانشان defs_اونهائي = 0;
  بنانشان i = 0;
  طالما (i < len)
  {
    إذا (svg[i] != '<')
    {
      i++;
      استمر;
    }

    إذا (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      صحيح cend = ڳوليو_substr (svg, len, i + 4, "-->", 3);
      إذا (cend < 0) أرجع خطأ;
      i = (بنانشان) cend + 3;
      استمر;
    }
    إذا (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      صحيح cend = ڳوليو_substr (svg, len, i + 9, "]]>", 3);
      إذا (cend < 0) أرجع خطأ;
      i = (بنانشان) cend + 3;
      استمر;
    }

    منطقي closing = (i + 1 < len && svg[i + 1] == '/');
    منطقي special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    بنانشان gt = i + 1;
    حرف quote = 0;
    طالما (gt < len)
    {
      حرف c = svg[gt];
      إذا (quote)
      {
        إذا (c == quote) quote = 0;
      }
      وإلا
      {
        إذا (c == '"' || c == '\'')
          quote = c;
        وإلا إذا (c == '>')
          اكسر;
      }
      gt++;
    }
    إذا (gt >= len)
      أرجع خطأ;

    إذا (special)
    {
      i = gt + 1;
      استمر;
    }

    بنانشان p = i + (closing ? 2 : 1);
    طالما (p < gt && خال_آهي ((بنانشان حرف) svg[p])) p++;
    ثابت حرف *name = svg + p;
    بنانشان name_len = 0;
    طالما (p + name_len < gt)
    {
      بنانشان حرف c = (بنانشان حرف) name[name_len];
      إذا (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        اكسر;
      name_len++;
    }
    منطقي is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    إذا (closing)
    {
      إذا (!اونهائي)
      {
        i = gt + 1;
        استمر;
      }

      open_elem_t e = ڍير[--اونهائي];
      بنانشان پڇاڙي = gt + 1;

      إذا (e.id.len)
      {
        إذا (unlikely (!id_entries->ڌڪو_or_fail (id_داخلا_t {e.id, (uint32_t) e.شروعات, (uint32_t) پڇاڙي})))
          أرجع خطأ;

        إذا (e.اندريون_defs_content)
        {
          إذا (unlikely (!defs_entries->ڌڪو_or_fail ()))
            أرجع خطأ;
          خودڪار &جڳهه = defs_entries->پڇ ();
          جڳهه.id = e.id;
          جڳهه.شروعات = e.شروعات;
          جڳهه.پڇاڙي = پڇاڙي;
        }

        hb_codepoint_t gid;
        إذا (parse_glyph_id_وسعت (e.id, &gid))
        {
          إذا (unlikely (!glyph_spans->ڌڪو_or_fail (glyph_داخلا_t {gid, (uint32_t) e.شروعات, (uint32_t) پڇاڙي})))
            أرجع خطأ;
        }
      }

      إذا (e.is_defs && defs_اونهائي)
        defs_اونهائي--;

      i = پڇاڙي;
      استمر;
    }

    SVG::svg_id_وسعت_t id = {};
    parse_id_اندريون_شروعات_ٽيگ (svg, i, gt, &id);

    بنانشان r = gt;
    طالما (r > i && خال_آهي ((بنانشان حرف) svg[r - 1])) r--;
    منطقي self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.شروعات = i;
    e.id = id;
    e.اندريون_defs_content = defs_اونهائي > 0;
    e.is_defs = is_defs;

    إذا (self_closing)
    {
      بنانشان پڇاڙي = gt + 1;
      إذا (e.id.len)
      {
        إذا (unlikely (!id_entries->ڌڪو_or_fail (id_داخلا_t {e.id, (uint32_t) e.شروعات, (uint32_t) پڇاڙي})))
          أرجع خطأ;

        إذا (e.اندريون_defs_content)
        {
          إذا (unlikely (!defs_entries->ڌڪو_or_fail ()))
            أرجع خطأ;
          خودڪار &جڳهه = defs_entries->پڇ ();
          جڳهه.id = e.id;
          جڳهه.شروعات = e.شروعات;
          جڳهه.پڇاڙي = پڇاڙي;
        }

        hb_codepoint_t gid;
        إذا (parse_glyph_id_وسعت (e.id, &gid))
        {
          إذا (unlikely (!glyph_spans->ڌڪو_or_fail (glyph_داخلا_t {gid, (uint32_t) e.شروعات, (uint32_t) پڇاڙي})))
            أرجع خطأ;
        }
      }
    }
    وإلا
    {
      إذا (unlikely (اونهائي >= MAX_اونهائي))
        أرجع خطأ;
      ڍير[اونهائي++] = e;
      إذا (is_defs)
        defs_اونهائي++;
    }

    i = gt + 1;
  }

  أرجع سچ;
}

} /* namespace _hb_svg_cache_impl */

مضمن
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  جدول = hb_sanitize_context_t ().reference_جدول<SVG> (face);
  doc_caches.init ();
  بنانشان doc_ڳڻپ = جدول->get_document_ڳڻپ ();
  إذا (doc_ڳڻپ && unlikely (!doc_caches.resize (doc_ڳڻپ)))
    doc_caches.clear ();
  لكل (بنانشان i = 0; i < doc_caches.ڊيگهائي; i++)
    doc_caches.arrayZ[i].سيٽ_relaxed (خالي_اشارو);
}

مضمن
SVG::accelerator_t::~accelerator_t ()
{
  لكل (بنانشان i = 0; i < doc_caches.ڊيگهائي; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  جدول.destroy ();
}

مضمن فراغ
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  إذا (!doc)
    أرجع;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

مضمن SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ثابت حرف *svg,
                                    بنانشان len,
                                    hb_codepoint_t شروعات_glyph,
                                    hb_codepoint_t پڇاڙي_glyph) ثابت
{
  ساكن ثابت uint32_t INVALID_وسعت = 0xFFFFFFFFu;

  خودڪار *doc = (svg_doc_cache_t *) hb_malloc (حجم (svg_doc_cache_t));
  إذا (!doc)
    أرجع خالي_اشارو;

  doc->blob = خالي_اشارو;
  doc->svg = خالي_اشارو;
  doc->len = 0;
  doc->defs_entries.init ();
  doc->شروعات_glyph = HB_CODEPOINT_INVALID;
  doc->پڇاڙي_glyph = HB_CODEPOINT_INVALID;
  doc->glyph_spans.init ();
  doc->id_spans.init ();

  doc->blob = hb_blob_reference (image);
  doc->svg = svg;
  doc->len = len;
  doc->شروعات_glyph = شروعات_glyph;
  doc->پڇاڙي_glyph = پڇاڙي_glyph;

  إذا (unlikely (شروعات_glyph == HB_CODEPOINT_INVALID || پڇاڙي_glyph < شروعات_glyph))
  {
    destroy_doc_cache (doc);
    أرجع خالي_اشارو;
  }

  بنانشان glyph_ڳڻپ = پڇاڙي_glyph - شروعات_glyph + 1;
  إذا (!doc->glyph_spans.resize ((صحيح) glyph_ڳڻپ))
  {
    destroy_doc_cache (doc);
    أرجع خالي_اشارو;
  }
  لكل (بنانشان i = 0; i < glyph_ڳڻپ; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_وسعت, INVALID_وسعت);

  hb_ويڪٽر_t<_hb_svg_cache_impl::glyph_داخلا_t> glyph_spans;
  glyph_spans.init ();
  hb_ويڪٽر_t<_hb_svg_cache_impl::id_داخلا_t> id_entries;
  id_entries.init ();
  إذا (!_hb_svg_cache_impl::parse_cache_entries_لڪيري (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    أرجع خالي_اشارو;
  }

  لكل (بنانشان i = 0; i < glyph_spans.ڊيگهائي; i++)
  {
    ثابت خودڪار &وسعت = glyph_spans.arrayZ[i];
    إذا (unlikely (وسعت.glyph < شروعات_glyph || وسعت.glyph > پڇاڙي_glyph))
      استمر;
    doc->glyph_spans.arrayZ[وسعت.glyph - شروعات_glyph] = hb_pair_t<uint32_t, uint32_t> (وسعت.شروعات, وسعت.پڇاڙي);
  }

  لكل (بنانشان i = 0; i < id_entries.ڊيگهائي; i++)
  {
    ثابت خودڪار &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *ٻاهريون = خالي_اشارو;
    إذا (doc->id_spans.has (e.id, &ٻاهريون))
      استمر;
    إذا (unlikely (!doc->id_spans.سيٽ (e.id, hb_pair_t<uint32_t, uint32_t> (e.شروعات, e.پڇاڙي))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      أرجع خالي_اشارو;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  أرجع doc;
}

مضمن ثابت SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ثابت حرف *svg,
                                              بنانشان len,
                                              بنانشان doc_اشاريو,
                                              hb_codepoint_t شروعات_glyph,
                                              hb_codepoint_t پڇاڙي_glyph) ثابت
{
  إذا (doc_اشاريو >= doc_caches.ڊيگهائي)
    أرجع خالي_اشارو;

  خودڪار &جڳهه = doc_caches.arrayZ[doc_اشاريو];
  خودڪار *doc = جڳهه.get_acquire ();
  إذا (doc)
    أرجع doc;

  خودڪار *fresh = make_doc_cache (image, svg, len, شروعات_glyph, پڇاڙي_glyph);
  إذا (!fresh)
    أرجع خالي_اشارو;

  خودڪار *expected = (svg_doc_cache_t *) خالي_اشارو;
  إذا (جڳهه.cmpexch (expected, fresh))
    أرجع fresh;

  destroy_doc_cache (fresh);
  أرجع expected;
}

مضمن ثابت حرف *
SVG::accelerator_t::doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                                       بنانشان *len) ثابت
{
  إذا (!doc)
  {
    إذا (len) *len = 0;
    أرجع خالي_اشارو;
  }
  إذا (len) *len = doc->len;
  أرجع doc->svg;
}

مضمن ثابت hb_ويڪٽر_t<SVG::svg_defs_داخلا_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت
{
  أرجع doc ? &doc->defs_entries : خالي_اشارو;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_get_glyph_وسعت (ثابت svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              بنانشان *شروعات,
                                              بنانشان *پڇاڙي) ثابت
{
  ساكن ثابت uint32_t INVALID_وسعت = 0xFFFFFFFFu;
  إذا (!doc || doc->شروعات_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->شروعات_glyph || glyph > doc->پڇاڙي_glyph)
    أرجع خطأ;

  ثابت خودڪار &وسعت = doc->glyph_spans.arrayZ[glyph - doc->شروعات_glyph];
  إذا (وسعت.پهريون == INVALID_وسعت)
    أرجع خطأ;
  إذا (unlikely (وسعت.پهريون > وسعت.second || وسعت.second > doc->len))
    أرجع خطأ;

  إذا (شروعات) *شروعات = وسعت.پهريون;
  إذا (پڇاڙي) *پڇاڙي = وسعت.second;
  أرجع سچ;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_ڳوليو_id_وسعت (ثابت svg_doc_cache_t *doc,
                                            svg_id_وسعت_t id,
                                            بنانشان *شروعات,
                                            بنانشان *پڇاڙي) ثابت
{
  إذا (!doc || !id.p || !id.len)
    أرجع خطأ;
  hb_pair_t<uint32_t, uint32_t> *وسعت = خالي_اشارو;
  إذا (!doc->id_spans.has (id, &وسعت))
    أرجع خطأ;
  إذا (unlikely (وسعت->پهريون > وسعت->second || وسعت->second > doc->len))
    أرجع خطأ;
  إذا (شروعات) *شروعات = وسعت->پهريون;
  إذا (پڇاڙي) *پڇاڙي = وسعت->second;
  أرجع سچ;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_ڳوليو_id_cstr (ثابت svg_doc_cache_t *doc,
                                            ثابت حرف *id,
                                            بنانشان *شروعات,
                                            بنانشان *پڇاڙي) ثابت
{
  إذا (!id) أرجع خطأ;
  svg_id_وسعت_t key = {id, (بنانشان) لڙي_ڊيگهه (id)};
  أرجع doc_cache_ڳوليو_id_وسعت (doc, key, شروعات, پڇاڙي);
}

بنية SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#جيڪڏهن_پورو /* OT_COLOR_SVG_SVG_HH */
