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

#اگر_تعریف_نشده OT_COLOR_SVG_SVG_HH
#تعریف_کن OT_COLOR_SVG_SVG_HH

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

#تعریف_کن HB_OT_برچسب_SVG HB_برچسب('S','V','G',' ')


نطاق OT {

ساكن مضمن hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ثابت حرف **svg,
                                     بیعلامت *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  بیعلامت داده_len = 0;
  ثابت حرف *داده = hb_blob_get_داده (blob, &داده_len);

  إذا (!داده || !داده_len)
    اذهب fail;

  إذا (hb_blob_is_gzip (داده, داده_len))
  {
    uint32_t expected_اندازه = 0;
    إذا (hb_gzip_get_uncompressed_اندازه (داده, داده_len, &expected_اندازه) &&
        unlikely ((size_t) expected_اندازه > (size_t) HB_SVG_MAX_DOCUMENT_اندازه))
      اذهب fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_اندازه);
    إذا (!uncompressed)
      اذهب fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    داده = hb_blob_get_داده (blob, &داده_len);
    إذا (!داده || !داده_len)
      اذهب fail;
  }

  إذا (unlikely ((size_t) داده_len > (size_t) HB_SVG_MAX_DOCUMENT_اندازه))
    اذهب fail;

  إذا (svg) *svg = داده;
  إذا (len) *len = داده_len;
  أرجع blob;

fail:
  hb_blob_destroy (blob);
  إذا (svg) *svg = اشاره_تهی;
  إذا (len) *len = 0;
  أرجع اشاره_تهی;
}

بنية SVGDocument_شاخص_مدخل
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < شروع_Glyph_ID ? -1 : g > انتها_Glyph_ID ? 1 : 0; }

  hb_codepoint_t get_شروع_glyph () ثابت
  { أرجع شروع_Glyph_ID; }

  hb_codepoint_t get_انتها_glyph () ثابت
  { أرجع انتها_Glyph_ID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, بیعلامت صحيح شاخص_جابه_جایی) ثابت
  {
    أرجع hb_blob_create_sub_blob (svg_blob,
				    شاخص_جابه_جایی + (بیعلامت صحيح) svgDoc,
				    svg_Doc_درازی);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, پایه, svg_Doc_درازی));
  }

  محمي:
  HBUINT16	شروع_Glyph_ID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	انتها_Glyph_ID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32_مقصد<Unsized_Array_از<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svg_Doc_درازی;	/* Length of the SVG document.
				 * Must be non-zero. */
  عام:
  DEFINE_اندازه_STATIC (12);
};

بنية SVG
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_SVG;

  بنية svg_id_بازه_t
  {
    ثابت حرف *p;
    بیعلامت len;

    منطقي عملگر == (ثابت svg_id_بازه_t &o) ثابت
    {
      أرجع len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ثابت
    {
      uint32_t h = hb_hash (len);
      لكل (بیعلامت i = 0; i < len; i++)
        h = h * 33u + (بیعلامت حرف) p[i];
      أرجع h;
    }
  };

  بنية svg_defs_مدخل_t
  {
    svg_id_بازه_t id;
    بیعلامت شروع;
    بیعلامت انتها;
  };

  بنية svg_doc_cache_t
  {
    hb_blob_t *blob = اشاره_تهی;
    ثابت حرف *svg = اشاره_تهی;
    بیعلامت len = 0;
    hb_وکتور_t<svg_defs_مدخل_t> defs_entries;
    hb_codepoint_t شروع_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t انتها_glyph = HB_CODEPOINT_INVALID;
    hb_وکتور_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_بازه_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  منطقي has_داده () ثابت { أرجع svgDocEntries; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ثابت
    {
      أرجع جدول->get_glyph_مدخل (glyph_id).reference_blob (جدول.get_blob (),
							       جدول->svgDocEntries);
    }

    بیعلامت get_document_شمار () ثابت
    { أرجع جدول->get_document_شمار (); }

    منطقي get_glyph_document_شاخص (hb_codepoint_t glyph_id, بیعلامت *شاخص_2) ثابت
    { أرجع جدول->get_glyph_document_شاخص (glyph_id, شاخص_2); }

    منطقي get_document_glyph_range (بیعلامت شاخص_2,
                                   hb_codepoint_t *شروع_glyph,
                                   hb_codepoint_t *انتها_glyph) ثابت
    { أرجع جدول->get_document_glyph_range (شاخص_2, شروع_glyph, انتها_glyph); }

    منطقي has_داده () ثابت { أرجع جدول->has_داده (); }

    ثابت svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ثابت حرف *svg,
                             بیعلامت len,
                             بیعلامت doc_شاخص,
                             hb_codepoint_t شروع_glyph,
                             hb_codepoint_t انتها_glyph) ثابت;

    ثابت حرف *
    doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                       بیعلامت *len) ثابت;

    ثابت hb_وکتور_t<svg_defs_مدخل_t> *
    doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت;

    منطقي
    doc_cache_get_glyph_بازه (ثابت svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              بیعلامت *شروع,
                              بیعلامت *انتها) ثابت;

    منطقي
    doc_cache_پیدا_کن_id_بازه (ثابت svg_doc_cache_t *doc,
                            svg_id_بازه_t id,
                            بیعلامت *شروع,
                            بیعلامت *انتها) ثابت;

    منطقي
    doc_cache_پیدا_کن_id_cstr (ثابت svg_doc_cache_t *doc,
                            ثابت حرف *id,
                            بیعلامت *شروع,
                            بیعلامت *انتها) ثابت;

    منطقي paint_glyph (hb_font_t *font HB_استفادهنشده, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *داده) ثابت
    {
      إذا (!has_داده ())
        أرجع خطأ;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      إذا (blob == hb_blob_get_خالی ())
        أرجع خطأ;

      منطقي ret = funcs->image (داده,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       اشاره_تهی);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ثابت حرف *svg,
                    بیعلامت len,
                    hb_codepoint_t شروع_glyph,
                    hb_codepoint_t انتها_glyph) ثابت;

    ساكن فراغ destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> جدول;
    تغییرپذیر hb_وکتور_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    عام:
    DEFINE_اندازه_STATIC (حجم (hb_blob_ptr_t<SVG>) +
                        حجم (hb_وکتور_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ثابت SVGDocument_شاخص_مدخل &get_glyph_مدخل (hb_codepoint_t glyph_id) ثابت
  { أرجع (هذا+svgDocEntries).bsearch (glyph_id); }

  بیعلامت get_document_شمار () ثابت
  {
    إذا (!has_داده ())
      أرجع 0;
    أرجع (هذا + svgDocEntries).len;
  }

  منطقي get_glyph_document_شاخص (hb_codepoint_t glyph_id, بیعلامت *شاخص_2) ثابت
  {
    إذا (!has_داده ())
      أرجع خطأ;
    أرجع (هذا + svgDocEntries).bfind (glyph_id, شاخص_2);
  }

  منطقي get_document_glyph_range (بیعلامت شاخص_2,
                                 hb_codepoint_t *شروع_glyph,
                                 hb_codepoint_t *انتها_glyph) ثابت
  {
    إذا (!has_داده ())
      أرجع خطأ;

    ثابت خودکار &entries = هذا + svgDocEntries;
    إذا (شاخص_2 >= entries.len)
      أرجع خطأ;

    ثابت خودکار &مدخل = entries.arrayZ[شاخص_2];
    إذا (شروع_glyph) *شروع_glyph = مدخل.get_شروع_glyph ();
    إذا (انتها_glyph) *انتها_glyph = مدخل.get_انتها_glyph ();
    أرجع درست;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  (هذا+svgDocEntries).sanitize_shallow (c)));
  }

  محمي:
  HBUINT16	نسخه;	/* Table version (starting at 0). */
  Offset32_مقصد<Sorted_Array16_از<SVGDocument_شاخص_مدخل>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  عام:
  DEFINE_اندازه_STATIC (10);
};

نطاق _hb_svg_cache_impl {

بنية glyph_مدخل_t
{
  hb_codepoint_t glyph;
  uint32_t شروع;
  uint32_t انتها;
};

بنية id_مدخل_t
{
  SVG::svg_id_بازه_t id;
  uint32_t شروع;
  uint32_t انتها;
};

بنية open_elem_t
{
  بیعلامت شروع;
  SVG::svg_id_بازه_t id;
  منطقي درون_defs_content;
  منطقي is_defs;
};

ساكن ثابت بیعلامت MAX_عمق = 128;

ساكن مضمن صحيح
پیدا_کن_substr (ثابت حرف *s,
             بیعلامت n,
             بیعلامت from,
             ثابت حرف *needle,
             بیعلامت needle_len)
{
  إذا (!needle_len || from >= n || needle_len > n)
    أرجع -1;
  لكل (بیعلامت i = from; i + needle_len <= n; i++)
    إذا (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      أرجع (صحيح) i;
  أرجع -1;
}

ساكن مضمن منطقي
parse_id_درون_شروع_برچسب (ثابت حرف *svg,
                       بیعلامت برچسب_شروع,
                       بیعلامت برچسب_انتها,
                       SVG::svg_id_بازه_t *id)
{
  بیعلامت p = برچسب_شروع;
  طالما (p + 4 <= برچسب_انتها)
  {
    إذا (!memcmp (svg + p, "id=\"", 4))
    {
      بیعلامت b = p + 4;
      بیعلامت e = b;
      طالما (e < برچسب_انتها && svg[e] != '"') e++;
      إذا (e <= برچسب_انتها && e > b)
      {
        *id = {svg + b, e - b};
        أرجع درست;
      }
    }
    إذا (!memcmp (svg + p, "id='", 4))
    {
      بیعلامت b = p + 4;
      بیعلامت e = b;
      طالما (e < برچسب_انتها && svg[e] != '\'') e++;
      إذا (e <= برچسب_انتها && e > b)
      {
        *id = {svg + b, e - b};
        أرجع درست;
      }
    }
    p++;
  }
  أرجع خطأ;
}

ساكن مضمن منطقي
parse_glyph_id_بازه (ثابت SVG::svg_id_بازه_t &id,
                     hb_codepoint_t *glyph)
{
  إذا (id.len <= 5 || memcmp (id.p, "glyph", 5))
    أرجع خطأ;

  hb_codepoint_t gid = 0;
  لكل (بیعلامت i = 5; i < id.len; i++)
  {
    بیعلامت حرف c = (بیعلامت حرف) id.p[i];
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
  أرجع درست;
}

ساكن مضمن منطقي
parse_cache_entries_خطی (ثابت حرف *svg,
                            بیعلامت len,
                            hb_وکتور_t<SVG::svg_defs_مدخل_t> *defs_entries,
                            hb_وکتور_t<glyph_مدخل_t> *glyph_spans,
                            hb_وکتور_t<id_مدخل_t> *id_entries)
{
  open_elem_t پشته[MAX_عمق] = {};
  بیعلامت عمق = 0;
  إذا (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    أرجع خطأ;

  بیعلامت defs_عمق = 0;
  بیعلامت i = 0;
  طالما (i < len)
  {
    إذا (svg[i] != '<')
    {
      i++;
      استمر;
    }

    إذا (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      صحيح cend = پیدا_کن_substr (svg, len, i + 4, "-->", 3);
      إذا (cend < 0) أرجع خطأ;
      i = (بیعلامت) cend + 3;
      استمر;
    }
    إذا (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      صحيح cend = پیدا_کن_substr (svg, len, i + 9, "]]>", 3);
      إذا (cend < 0) أرجع خطأ;
      i = (بیعلامت) cend + 3;
      استمر;
    }

    منطقي closing = (i + 1 < len && svg[i + 1] == '/');
    منطقي special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    بیعلامت gt = i + 1;
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

    بیعلامت p = i + (closing ? 2 : 1);
    طالما (p < gt && آیا_فاصله ((بیعلامت حرف) svg[p])) p++;
    ثابت حرف *name = svg + p;
    بیعلامت name_len = 0;
    طالما (p + name_len < gt)
    {
      بیعلامت حرف c = (بیعلامت حرف) name[name_len];
      إذا (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        اكسر;
      name_len++;
    }
    منطقي is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    إذا (closing)
    {
      إذا (!عمق)
      {
        i = gt + 1;
        استمر;
      }

      open_elem_t e = پشته[--عمق];
      بیعلامت انتها = gt + 1;

      إذا (e.id.len)
      {
        إذا (unlikely (!id_entries->افزودن_or_fail (id_مدخل_t {e.id, (uint32_t) e.شروع, (uint32_t) انتها})))
          أرجع خطأ;

        إذا (e.درون_defs_content)
        {
          إذا (unlikely (!defs_entries->افزودن_or_fail ()))
            أرجع خطأ;
          خودکار &جایگاه = defs_entries->دم ();
          جایگاه.id = e.id;
          جایگاه.شروع = e.شروع;
          جایگاه.انتها = انتها;
        }

        hb_codepoint_t gid;
        إذا (parse_glyph_id_بازه (e.id, &gid))
        {
          إذا (unlikely (!glyph_spans->افزودن_or_fail (glyph_مدخل_t {gid, (uint32_t) e.شروع, (uint32_t) انتها})))
            أرجع خطأ;
        }
      }

      إذا (e.is_defs && defs_عمق)
        defs_عمق--;

      i = انتها;
      استمر;
    }

    SVG::svg_id_بازه_t id = {};
    parse_id_درون_شروع_برچسب (svg, i, gt, &id);

    بیعلامت r = gt;
    طالما (r > i && آیا_فاصله ((بیعلامت حرف) svg[r - 1])) r--;
    منطقي self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.شروع = i;
    e.id = id;
    e.درون_defs_content = defs_عمق > 0;
    e.is_defs = is_defs;

    إذا (self_closing)
    {
      بیعلامت انتها = gt + 1;
      إذا (e.id.len)
      {
        إذا (unlikely (!id_entries->افزودن_or_fail (id_مدخل_t {e.id, (uint32_t) e.شروع, (uint32_t) انتها})))
          أرجع خطأ;

        إذا (e.درون_defs_content)
        {
          إذا (unlikely (!defs_entries->افزودن_or_fail ()))
            أرجع خطأ;
          خودکار &جایگاه = defs_entries->دم ();
          جایگاه.id = e.id;
          جایگاه.شروع = e.شروع;
          جایگاه.انتها = انتها;
        }

        hb_codepoint_t gid;
        إذا (parse_glyph_id_بازه (e.id, &gid))
        {
          إذا (unlikely (!glyph_spans->افزودن_or_fail (glyph_مدخل_t {gid, (uint32_t) e.شروع, (uint32_t) انتها})))
            أرجع خطأ;
        }
      }
    }
    وإلا
    {
      إذا (unlikely (عمق >= MAX_عمق))
        أرجع خطأ;
      پشته[عمق++] = e;
      إذا (is_defs)
        defs_عمق++;
    }

    i = gt + 1;
  }

  أرجع درست;
}

} /* namespace _hb_svg_cache_impl */

مضمن
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  جدول = hb_sanitize_context_t ().reference_جدول<SVG> (face);
  doc_caches.init ();
  بیعلامت doc_شمار = جدول->get_document_شمار ();
  إذا (doc_شمار && unlikely (!doc_caches.resize (doc_شمار)))
    doc_caches.clear ();
  لكل (بیعلامت i = 0; i < doc_caches.درازی; i++)
    doc_caches.arrayZ[i].مجموعه_relaxed (اشاره_تهی);
}

مضمن
SVG::accelerator_t::~accelerator_t ()
{
  لكل (بیعلامت i = 0; i < doc_caches.درازی; i++)
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
                                    بیعلامت len,
                                    hb_codepoint_t شروع_glyph,
                                    hb_codepoint_t انتها_glyph) ثابت
{
  ساكن ثابت uint32_t INVALID_بازه = 0xFFFFFFFFu;

  خودکار *doc = (svg_doc_cache_t *) hb_malloc (حجم (svg_doc_cache_t));
  إذا (!doc)
    أرجع اشاره_تهی;

  doc->blob = اشاره_تهی;
  doc->svg = اشاره_تهی;
  doc->len = 0;
  doc->defs_entries.init ();
  doc->شروع_glyph = HB_CODEPOINT_INVALID;
  doc->انتها_glyph = HB_CODEPOINT_INVALID;
  doc->glyph_spans.init ();
  doc->id_spans.init ();

  doc->blob = hb_blob_reference (image);
  doc->svg = svg;
  doc->len = len;
  doc->شروع_glyph = شروع_glyph;
  doc->انتها_glyph = انتها_glyph;

  إذا (unlikely (شروع_glyph == HB_CODEPOINT_INVALID || انتها_glyph < شروع_glyph))
  {
    destroy_doc_cache (doc);
    أرجع اشاره_تهی;
  }

  بیعلامت glyph_شمار = انتها_glyph - شروع_glyph + 1;
  إذا (!doc->glyph_spans.resize ((صحيح) glyph_شمار))
  {
    destroy_doc_cache (doc);
    أرجع اشاره_تهی;
  }
  لكل (بیعلامت i = 0; i < glyph_شمار; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_بازه, INVALID_بازه);

  hb_وکتور_t<_hb_svg_cache_impl::glyph_مدخل_t> glyph_spans;
  glyph_spans.init ();
  hb_وکتور_t<_hb_svg_cache_impl::id_مدخل_t> id_entries;
  id_entries.init ();
  إذا (!_hb_svg_cache_impl::parse_cache_entries_خطی (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    أرجع اشاره_تهی;
  }

  لكل (بیعلامت i = 0; i < glyph_spans.درازی; i++)
  {
    ثابت خودکار &بازه = glyph_spans.arrayZ[i];
    إذا (unlikely (بازه.glyph < شروع_glyph || بازه.glyph > انتها_glyph))
      استمر;
    doc->glyph_spans.arrayZ[بازه.glyph - شروع_glyph] = hb_pair_t<uint32_t, uint32_t> (بازه.شروع, بازه.انتها);
  }

  لكل (بیعلامت i = 0; i < id_entries.درازی; i++)
  {
    ثابت خودکار &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *بیرون = اشاره_تهی;
    إذا (doc->id_spans.has (e.id, &بیرون))
      استمر;
    إذا (unlikely (!doc->id_spans.مجموعه (e.id, hb_pair_t<uint32_t, uint32_t> (e.شروع, e.انتها))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      أرجع اشاره_تهی;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  أرجع doc;
}

مضمن ثابت SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ثابت حرف *svg,
                                              بیعلامت len,
                                              بیعلامت doc_شاخص,
                                              hb_codepoint_t شروع_glyph,
                                              hb_codepoint_t انتها_glyph) ثابت
{
  إذا (doc_شاخص >= doc_caches.درازی)
    أرجع اشاره_تهی;

  خودکار &جایگاه = doc_caches.arrayZ[doc_شاخص];
  خودکار *doc = جایگاه.get_acquire ();
  إذا (doc)
    أرجع doc;

  خودکار *fresh = make_doc_cache (image, svg, len, شروع_glyph, انتها_glyph);
  إذا (!fresh)
    أرجع اشاره_تهی;

  خودکار *expected = (svg_doc_cache_t *) اشاره_تهی;
  إذا (جایگاه.cmpexch (expected, fresh))
    أرجع fresh;

  destroy_doc_cache (fresh);
  أرجع expected;
}

مضمن ثابت حرف *
SVG::accelerator_t::doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                                       بیعلامت *len) ثابت
{
  إذا (!doc)
  {
    إذا (len) *len = 0;
    أرجع اشاره_تهی;
  }
  إذا (len) *len = doc->len;
  أرجع doc->svg;
}

مضمن ثابت hb_وکتور_t<SVG::svg_defs_مدخل_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت
{
  أرجع doc ? &doc->defs_entries : اشاره_تهی;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_get_glyph_بازه (ثابت svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              بیعلامت *شروع,
                                              بیعلامت *انتها) ثابت
{
  ساكن ثابت uint32_t INVALID_بازه = 0xFFFFFFFFu;
  إذا (!doc || doc->شروع_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->شروع_glyph || glyph > doc->انتها_glyph)
    أرجع خطأ;

  ثابت خودکار &بازه = doc->glyph_spans.arrayZ[glyph - doc->شروع_glyph];
  إذا (بازه.اولی == INVALID_بازه)
    أرجع خطأ;
  إذا (unlikely (بازه.اولی > بازه.second || بازه.second > doc->len))
    أرجع خطأ;

  إذا (شروع) *شروع = بازه.اولی;
  إذا (انتها) *انتها = بازه.second;
  أرجع درست;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_پیدا_کن_id_بازه (ثابت svg_doc_cache_t *doc,
                                            svg_id_بازه_t id,
                                            بیعلامت *شروع,
                                            بیعلامت *انتها) ثابت
{
  إذا (!doc || !id.p || !id.len)
    أرجع خطأ;
  hb_pair_t<uint32_t, uint32_t> *بازه = اشاره_تهی;
  إذا (!doc->id_spans.has (id, &بازه))
    أرجع خطأ;
  إذا (unlikely (بازه->اولی > بازه->second || بازه->second > doc->len))
    أرجع خطأ;
  إذا (شروع) *شروع = بازه->اولی;
  إذا (انتها) *انتها = بازه->second;
  أرجع درست;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_پیدا_کن_id_cstr (ثابت svg_doc_cache_t *doc,
                                            ثابت حرف *id,
                                            بیعلامت *شروع,
                                            بیعلامت *انتها) ثابت
{
  إذا (!id) أرجع خطأ;
  svg_id_بازه_t key = {id, (بیعلامت) طول_رشته (id)};
  أرجع doc_cache_پیدا_کن_id_بازه (doc, key, شروع, انتها);
}

بنية SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#پایان_اگر /* OT_COLOR_SVG_SVG_HH */
