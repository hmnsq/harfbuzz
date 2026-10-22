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

#جے_بنا_تعریف OT_COLOR_SVG_SVG_HH
#تعریف_کرو OT_COLOR_SVG_SVG_HH

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

#تعریف_کرو HB_OT_ٹیگ_SVG HB_ٹیگ('S','V','G',' ')


نطاق OT {

ساكن مضمن hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ثابت حرف **svg,
                                     بنانشانی *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  بنانشانی انکڑے_len = 0;
  ثابت حرف *انکڑے = hb_blob_get_انکڑے (blob, &انکڑے_len);

  إذا (!انکڑے || !انکڑے_len)
    اذهب fail;

  إذا (hb_blob_is_gzip (انکڑے, انکڑے_len))
  {
    uint32_t expected_ناپ = 0;
    إذا (hb_gzip_get_uncompressed_ناپ (انکڑے, انکڑے_len, &expected_ناپ) &&
        unlikely ((size_t) expected_ناپ > (size_t) HB_SVG_MAX_DOCUMENT_ناپ))
      اذهب fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_ناپ);
    إذا (!uncompressed)
      اذهب fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    انکڑے = hb_blob_get_انکڑے (blob, &انکڑے_len);
    إذا (!انکڑے || !انکڑے_len)
      اذهب fail;
  }

  إذا (unlikely ((size_t) انکڑے_len > (size_t) HB_SVG_MAX_DOCUMENT_ناپ))
    اذهب fail;

  إذا (svg) *svg = انکڑے;
  إذا (len) *len = انکڑے_len;
  أرجع blob;

fail:
  hb_blob_destroy (blob);
  إذا (svg) *svg = خالی_سوچک;
  إذا (len) *len = 0;
  أرجع خالی_سوچک;
}

بنية SVGDocument_اشاریہ_اندراج
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < شروع_Glyph_ID ? -1 : g > سرا_Glyph_ID ? 1 : 0; }

  hb_codepoint_t get_شروع_glyph () ثابت
  { أرجع شروع_Glyph_ID; }

  hb_codepoint_t get_سرا_glyph () ثابت
  { أرجع سرا_Glyph_ID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, بنانشانی صحيح اشاریہ_وتھ) ثابت
  {
    أرجع hb_blob_create_sub_blob (svg_blob,
				    اشاریہ_وتھ + (بنانشانی صحيح) svgDoc,
				    svg_Doc_لمائی);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, ادھار, svg_Doc_لمائی));
  }

  محمي:
  HBUINT16	شروع_Glyph_ID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	سرا_Glyph_ID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32_منزل<Unsized_Array_دا<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svg_Doc_لمائی;	/* Length of the SVG document.
				 * Must be non-zero. */
  عام:
  DEFINE_ناپ_STATIC (12);
};

بنية SVG
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_SVG;

  بنية svg_id_وسعت_t
  {
    ثابت حرف *p;
    بنانشانی len;

    منطقي عامل == (ثابت svg_id_وسعت_t &o) ثابت
    {
      أرجع len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ثابت
    {
      uint32_t h = hb_hash (len);
      لكل (بنانشانی i = 0; i < len; i++)
        h = h * 33u + (بنانشانی حرف) p[i];
      أرجع h;
    }
  };

  بنية svg_defs_اندراج_t
  {
    svg_id_وسعت_t id;
    بنانشانی شروع;
    بنانشانی سرا;
  };

  بنية svg_doc_cache_t
  {
    hb_blob_t *blob = خالی_سوچک;
    ثابت حرف *svg = خالی_سوچک;
    بنانشانی len = 0;
    hb_سمتیہ_t<svg_defs_اندراج_t> defs_entries;
    hb_codepoint_t شروع_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t سرا_glyph = HB_CODEPOINT_INVALID;
    hb_سمتیہ_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_وسعت_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  منطقي has_انکڑے () ثابت { أرجع svgDocEntries; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ثابت
    {
      أرجع جدول->get_glyph_اندراج (glyph_id).reference_blob (جدول.get_blob (),
							       جدول->svgDocEntries);
    }

    بنانشانی get_document_گنتی () ثابت
    { أرجع جدول->get_document_گنتی (); }

    منطقي get_glyph_document_اشاریہ (hb_codepoint_t glyph_id, بنانشانی *اشاریہ_2) ثابت
    { أرجع جدول->get_glyph_document_اشاریہ (glyph_id, اشاریہ_2); }

    منطقي get_document_glyph_range (بنانشانی اشاریہ_2,
                                   hb_codepoint_t *شروع_glyph,
                                   hb_codepoint_t *سرا_glyph) ثابت
    { أرجع جدول->get_document_glyph_range (اشاریہ_2, شروع_glyph, سرا_glyph); }

    منطقي has_انکڑے () ثابت { أرجع جدول->has_انکڑے (); }

    ثابت svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ثابت حرف *svg,
                             بنانشانی len,
                             بنانشانی doc_اشاریہ,
                             hb_codepoint_t شروع_glyph,
                             hb_codepoint_t سرا_glyph) ثابت;

    ثابت حرف *
    doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                       بنانشانی *len) ثابت;

    ثابت hb_سمتیہ_t<svg_defs_اندراج_t> *
    doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت;

    منطقي
    doc_cache_get_glyph_وسعت (ثابت svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              بنانشانی *شروع,
                              بنانشانی *سرا) ثابت;

    منطقي
    doc_cache_لبھو_id_وسعت (ثابت svg_doc_cache_t *doc,
                            svg_id_وسعت_t id,
                            بنانشانی *شروع,
                            بنانشانی *سرا) ثابت;

    منطقي
    doc_cache_لبھو_id_cstr (ثابت svg_doc_cache_t *doc,
                            ثابت حرف *id,
                            بنانشانی *شروع,
                            بنانشانی *سرا) ثابت;

    منطقي paint_glyph (hb_font_t *font HB_بن_ورتیا, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *انکڑے) ثابت
    {
      إذا (!has_انکڑے ())
        أرجع خطأ;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      إذا (blob == hb_blob_get_خالی_تھاواں ())
        أرجع خطأ;

      منطقي ret = funcs->image (انکڑے,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       خالی_سوچک);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ثابت حرف *svg,
                    بنانشانی len,
                    hb_codepoint_t شروع_glyph,
                    hb_codepoint_t سرا_glyph) ثابت;

    ساكن فراغ destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> جدول;
    بدلنجوگ hb_سمتیہ_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    عام:
    DEFINE_ناپ_STATIC (حجم (hb_blob_ptr_t<SVG>) +
                        حجم (hb_سمتیہ_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ثابت SVGDocument_اشاریہ_اندراج &get_glyph_اندراج (hb_codepoint_t glyph_id) ثابت
  { أرجع (هذا+svgDocEntries).bsearch (glyph_id); }

  بنانشانی get_document_گنتی () ثابت
  {
    إذا (!has_انکڑے ())
      أرجع 0;
    أرجع (هذا + svgDocEntries).len;
  }

  منطقي get_glyph_document_اشاریہ (hb_codepoint_t glyph_id, بنانشانی *اشاریہ_2) ثابت
  {
    إذا (!has_انکڑے ())
      أرجع خطأ;
    أرجع (هذا + svgDocEntries).bfind (glyph_id, اشاریہ_2);
  }

  منطقي get_document_glyph_range (بنانشانی اشاریہ_2,
                                 hb_codepoint_t *شروع_glyph,
                                 hb_codepoint_t *سرا_glyph) ثابت
  {
    إذا (!has_انکڑے ())
      أرجع خطأ;

    ثابت خودکار &entries = هذا + svgDocEntries;
    إذا (اشاریہ_2 >= entries.len)
      أرجع خطأ;

    ثابت خودکار &اندراج = entries.arrayZ[اشاریہ_2];
    إذا (شروع_glyph) *شروع_glyph = اندراج.get_شروع_glyph ();
    إذا (سرا_glyph) *سرا_glyph = اندراج.get_سرا_glyph ();
    أرجع سچ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  (هذا+svgDocEntries).sanitize_shallow (c)));
  }

  محمي:
  HBUINT16	نسخہ;	/* Table version (starting at 0). */
  Offset32_منزل<Sorted_Array16_دا<SVGDocument_اشاریہ_اندراج>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  عام:
  DEFINE_ناپ_STATIC (10);
};

نطاق _hb_svg_cache_impl {

بنية glyph_اندراج_t
{
  hb_codepoint_t glyph;
  uint32_t شروع;
  uint32_t سرا;
};

بنية id_اندراج_t
{
  SVG::svg_id_وسعت_t id;
  uint32_t شروع;
  uint32_t سرا;
};

بنية open_elem_t
{
  بنانشانی شروع;
  SVG::svg_id_وسعت_t id;
  منطقي وچ_defs_content;
  منطقي is_defs;
};

ساكن ثابت بنانشانی MAX_ڈونگھائی = 128;

ساكن مضمن صحيح
لبھو_substr (ثابت حرف *s,
             بنانشانی n,
             بنانشانی from,
             ثابت حرف *needle,
             بنانشانی needle_len)
{
  إذا (!needle_len || from >= n || needle_len > n)
    أرجع -1;
  لكل (بنانشانی i = from; i + needle_len <= n; i++)
    إذا (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      أرجع (صحيح) i;
  أرجع -1;
}

ساكن مضمن منطقي
parse_id_وچ_شروع_ٹیگ (ثابت حرف *svg,
                       بنانشانی ٹیگ_شروع,
                       بنانشانی ٹیگ_سرا,
                       SVG::svg_id_وسعت_t *id)
{
  بنانشانی p = ٹیگ_شروع;
  طالما (p + 4 <= ٹیگ_سرا)
  {
    إذا (!memcmp (svg + p, "id=\"", 4))
    {
      بنانشانی b = p + 4;
      بنانشانی e = b;
      طالما (e < ٹیگ_سرا && svg[e] != '"') e++;
      إذا (e <= ٹیگ_سرا && e > b)
      {
        *id = {svg + b, e - b};
        أرجع سچ;
      }
    }
    إذا (!memcmp (svg + p, "id='", 4))
    {
      بنانشانی b = p + 4;
      بنانشانی e = b;
      طالما (e < ٹیگ_سرا && svg[e] != '\'') e++;
      إذا (e <= ٹیگ_سرا && e > b)
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
  لكل (بنانشانی i = 5; i < id.len; i++)
  {
    بنانشانی حرف c = (بنانشانی حرف) id.p[i];
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
parse_cache_entries_خطی (ثابت حرف *svg,
                            بنانشانی len,
                            hb_سمتیہ_t<SVG::svg_defs_اندراج_t> *defs_entries,
                            hb_سمتیہ_t<glyph_اندراج_t> *glyph_spans,
                            hb_سمتیہ_t<id_اندراج_t> *id_entries)
{
  open_elem_t ڈھیر[MAX_ڈونگھائی] = {};
  بنانشانی ڈونگھائی = 0;
  إذا (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    أرجع خطأ;

  بنانشانی defs_ڈونگھائی = 0;
  بنانشانی i = 0;
  طالما (i < len)
  {
    إذا (svg[i] != '<')
    {
      i++;
      استمر;
    }

    إذا (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      صحيح cend = لبھو_substr (svg, len, i + 4, "-->", 3);
      إذا (cend < 0) أرجع خطأ;
      i = (بنانشانی) cend + 3;
      استمر;
    }
    إذا (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      صحيح cend = لبھو_substr (svg, len, i + 9, "]]>", 3);
      إذا (cend < 0) أرجع خطأ;
      i = (بنانشانی) cend + 3;
      استمر;
    }

    منطقي closing = (i + 1 < len && svg[i + 1] == '/');
    منطقي special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    بنانشانی gt = i + 1;
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

    بنانشانی p = i + (closing ? 2 : 1);
    طالما (p < gt && خلا_اے ((بنانشانی حرف) svg[p])) p++;
    ثابت حرف *name = svg + p;
    بنانشانی name_len = 0;
    طالما (p + name_len < gt)
    {
      بنانشانی حرف c = (بنانشانی حرف) name[name_len];
      إذا (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        اكسر;
      name_len++;
    }
    منطقي is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    إذا (closing)
    {
      إذا (!ڈونگھائی)
      {
        i = gt + 1;
        استمر;
      }

      open_elem_t e = ڈھیر[--ڈونگھائی];
      بنانشانی سرا = gt + 1;

      إذا (e.id.len)
      {
        إذا (unlikely (!id_entries->ڈالو_or_fail (id_اندراج_t {e.id, (uint32_t) e.شروع, (uint32_t) سرا})))
          أرجع خطأ;

        إذا (e.وچ_defs_content)
        {
          إذا (unlikely (!defs_entries->ڈالو_or_fail ()))
            أرجع خطأ;
          خودکار &تھاں = defs_entries->پوچھ ();
          تھاں.id = e.id;
          تھاں.شروع = e.شروع;
          تھاں.سرا = سرا;
        }

        hb_codepoint_t gid;
        إذا (parse_glyph_id_وسعت (e.id, &gid))
        {
          إذا (unlikely (!glyph_spans->ڈالو_or_fail (glyph_اندراج_t {gid, (uint32_t) e.شروع, (uint32_t) سرا})))
            أرجع خطأ;
        }
      }

      إذا (e.is_defs && defs_ڈونگھائی)
        defs_ڈونگھائی--;

      i = سرا;
      استمر;
    }

    SVG::svg_id_وسعت_t id = {};
    parse_id_وچ_شروع_ٹیگ (svg, i, gt, &id);

    بنانشانی r = gt;
    طالما (r > i && خلا_اے ((بنانشانی حرف) svg[r - 1])) r--;
    منطقي self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.شروع = i;
    e.id = id;
    e.وچ_defs_content = defs_ڈونگھائی > 0;
    e.is_defs = is_defs;

    إذا (self_closing)
    {
      بنانشانی سرا = gt + 1;
      إذا (e.id.len)
      {
        إذا (unlikely (!id_entries->ڈالو_or_fail (id_اندراج_t {e.id, (uint32_t) e.شروع, (uint32_t) سرا})))
          أرجع خطأ;

        إذا (e.وچ_defs_content)
        {
          إذا (unlikely (!defs_entries->ڈالو_or_fail ()))
            أرجع خطأ;
          خودکار &تھاں = defs_entries->پوچھ ();
          تھاں.id = e.id;
          تھاں.شروع = e.شروع;
          تھاں.سرا = سرا;
        }

        hb_codepoint_t gid;
        إذا (parse_glyph_id_وسعت (e.id, &gid))
        {
          إذا (unlikely (!glyph_spans->ڈالو_or_fail (glyph_اندراج_t {gid, (uint32_t) e.شروع, (uint32_t) سرا})))
            أرجع خطأ;
        }
      }
    }
    وإلا
    {
      إذا (unlikely (ڈونگھائی >= MAX_ڈونگھائی))
        أرجع خطأ;
      ڈھیر[ڈونگھائی++] = e;
      إذا (is_defs)
        defs_ڈونگھائی++;
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
  بنانشانی doc_گنتی = جدول->get_document_گنتی ();
  إذا (doc_گنتی && unlikely (!doc_caches.resize (doc_گنتی)))
    doc_caches.clear ();
  لكل (بنانشانی i = 0; i < doc_caches.لمائی; i++)
    doc_caches.arrayZ[i].سیٹ_relaxed (خالی_سوچک);
}

مضمن
SVG::accelerator_t::~accelerator_t ()
{
  لكل (بنانشانی i = 0; i < doc_caches.لمائی; i++)
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
                                    بنانشانی len,
                                    hb_codepoint_t شروع_glyph,
                                    hb_codepoint_t سرا_glyph) ثابت
{
  ساكن ثابت uint32_t INVALID_وسعت = 0xFFFFFFFFu;

  خودکار *doc = (svg_doc_cache_t *) hb_malloc (حجم (svg_doc_cache_t));
  إذا (!doc)
    أرجع خالی_سوچک;

  doc->blob = خالی_سوچک;
  doc->svg = خالی_سوچک;
  doc->len = 0;
  doc->defs_entries.init ();
  doc->شروع_glyph = HB_CODEPOINT_INVALID;
  doc->سرا_glyph = HB_CODEPOINT_INVALID;
  doc->glyph_spans.init ();
  doc->id_spans.init ();

  doc->blob = hb_blob_reference (image);
  doc->svg = svg;
  doc->len = len;
  doc->شروع_glyph = شروع_glyph;
  doc->سرا_glyph = سرا_glyph;

  إذا (unlikely (شروع_glyph == HB_CODEPOINT_INVALID || سرا_glyph < شروع_glyph))
  {
    destroy_doc_cache (doc);
    أرجع خالی_سوچک;
  }

  بنانشانی glyph_گنتی = سرا_glyph - شروع_glyph + 1;
  إذا (!doc->glyph_spans.resize ((صحيح) glyph_گنتی))
  {
    destroy_doc_cache (doc);
    أرجع خالی_سوچک;
  }
  لكل (بنانشانی i = 0; i < glyph_گنتی; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_وسعت, INVALID_وسعت);

  hb_سمتیہ_t<_hb_svg_cache_impl::glyph_اندراج_t> glyph_spans;
  glyph_spans.init ();
  hb_سمتیہ_t<_hb_svg_cache_impl::id_اندراج_t> id_entries;
  id_entries.init ();
  إذا (!_hb_svg_cache_impl::parse_cache_entries_خطی (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    أرجع خالی_سوچک;
  }

  لكل (بنانشانی i = 0; i < glyph_spans.لمائی; i++)
  {
    ثابت خودکار &وسعت = glyph_spans.arrayZ[i];
    إذا (unlikely (وسعت.glyph < شروع_glyph || وسعت.glyph > سرا_glyph))
      استمر;
    doc->glyph_spans.arrayZ[وسعت.glyph - شروع_glyph] = hb_pair_t<uint32_t, uint32_t> (وسعت.شروع, وسعت.سرا);
  }

  لكل (بنانشانی i = 0; i < id_entries.لمائی; i++)
  {
    ثابت خودکار &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *باہرلا = خالی_سوچک;
    إذا (doc->id_spans.has (e.id, &باہرلا))
      استمر;
    إذا (unlikely (!doc->id_spans.سیٹ (e.id, hb_pair_t<uint32_t, uint32_t> (e.شروع, e.سرا))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      أرجع خالی_سوچک;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  أرجع doc;
}

مضمن ثابت SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ثابت حرف *svg,
                                              بنانشانی len,
                                              بنانشانی doc_اشاریہ,
                                              hb_codepoint_t شروع_glyph,
                                              hb_codepoint_t سرا_glyph) ثابت
{
  إذا (doc_اشاریہ >= doc_caches.لمائی)
    أرجع خالی_سوچک;

  خودکار &تھاں = doc_caches.arrayZ[doc_اشاریہ];
  خودکار *doc = تھاں.get_acquire ();
  إذا (doc)
    أرجع doc;

  خودکار *fresh = make_doc_cache (image, svg, len, شروع_glyph, سرا_glyph);
  إذا (!fresh)
    أرجع خالی_سوچک;

  خودکار *expected = (svg_doc_cache_t *) خالی_سوچک;
  إذا (تھاں.cmpexch (expected, fresh))
    أرجع fresh;

  destroy_doc_cache (fresh);
  أرجع expected;
}

مضمن ثابت حرف *
SVG::accelerator_t::doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                                       بنانشانی *len) ثابت
{
  إذا (!doc)
  {
    إذا (len) *len = 0;
    أرجع خالی_سوچک;
  }
  إذا (len) *len = doc->len;
  أرجع doc->svg;
}

مضمن ثابت hb_سمتیہ_t<SVG::svg_defs_اندراج_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت
{
  أرجع doc ? &doc->defs_entries : خالی_سوچک;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_get_glyph_وسعت (ثابت svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              بنانشانی *شروع,
                                              بنانشانی *سرا) ثابت
{
  ساكن ثابت uint32_t INVALID_وسعت = 0xFFFFFFFFu;
  إذا (!doc || doc->شروع_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->شروع_glyph || glyph > doc->سرا_glyph)
    أرجع خطأ;

  ثابت خودکار &وسعت = doc->glyph_spans.arrayZ[glyph - doc->شروع_glyph];
  إذا (وسعت.پہلا == INVALID_وسعت)
    أرجع خطأ;
  إذا (unlikely (وسعت.پہلا > وسعت.second || وسعت.second > doc->len))
    أرجع خطأ;

  إذا (شروع) *شروع = وسعت.پہلا;
  إذا (سرا) *سرا = وسعت.second;
  أرجع سچ;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_لبھو_id_وسعت (ثابت svg_doc_cache_t *doc,
                                            svg_id_وسعت_t id,
                                            بنانشانی *شروع,
                                            بنانشانی *سرا) ثابت
{
  إذا (!doc || !id.p || !id.len)
    أرجع خطأ;
  hb_pair_t<uint32_t, uint32_t> *وسعت = خالی_سوچک;
  إذا (!doc->id_spans.has (id, &وسعت))
    أرجع خطأ;
  إذا (unlikely (وسعت->پہلا > وسعت->second || وسعت->second > doc->len))
    أرجع خطأ;
  إذا (شروع) *شروع = وسعت->پہلا;
  إذا (سرا) *سرا = وسعت->second;
  أرجع سچ;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_لبھو_id_cstr (ثابت svg_doc_cache_t *doc,
                                            ثابت حرف *id,
                                            بنانشانی *شروع,
                                            بنانشانی *سرا) ثابت
{
  إذا (!id) أرجع خطأ;
  svg_id_وسعت_t key = {id, (بنانشانی) لڑی_لمبائی (id)};
  أرجع doc_cache_لبھو_id_وسعت (doc, key, شروع, سرا);
}

بنية SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#جے_انت /* OT_COLOR_SVG_SVG_HH */
