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

#اگر_غیر_تعریف_شدہ OT_COLOR_SVG_SVG_HH
#تعریف_کرو OT_COLOR_SVG_SVG_HH

#شامل "../../../hb-open-type.hh"
#شامل "../../../hb-blob.hh"
#شامل "../../../hb-limits.hh"
#شامل "../../../hb-map.hh"
#شامل "../../../hb-paint.hh"
#شامل "../../../hb-zlib.hh"
#شامل <ctype.h>
#شامل <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#تعریف_کرو HB_OT_لیبل_SVG HB_لیبل('S','V','G',' ')


نامخلا OT {

ساکن اندرونی hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ثابت حرف **svg,
                                     غیرعلامتی *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  غیرعلامتی ڈیٹا_len = 0;
  ثابت حرف *ڈیٹا = hb_blob_get_ڈیٹا (blob, &ڈیٹا_len);

  اگر (!ڈیٹا || !ڈیٹا_len)
    جاؤ fail;

  اگر (hb_blob_is_gzip (ڈیٹا, ڈیٹا_len))
  {
    uint32_t expected_سائز = 0;
    اگر (hb_gzip_get_uncompressed_سائز (ڈیٹا, ڈیٹا_len, &expected_سائز) &&
        unlikely ((size_t) expected_سائز > (size_t) HB_SVG_MAX_DOCUMENT_سائز))
      جاؤ fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_سائز);
    اگر (!uncompressed)
      جاؤ fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    ڈیٹا = hb_blob_get_ڈیٹا (blob, &ڈیٹا_len);
    اگر (!ڈیٹا || !ڈیٹا_len)
      جاؤ fail;
  }

  اگر (unlikely ((size_t) ڈیٹا_len > (size_t) HB_SVG_MAX_DOCUMENT_سائز))
    جاؤ fail;

  اگر (svg) *svg = ڈیٹا;
  اگر (len) *len = ڈیٹا_len;
  واپس blob;

fail:
  hb_blob_destroy (blob);
  اگر (svg) *svg = خالی_اشاریہ;
  اگر (len) *len = 0;
  واپس خالی_اشاریہ;
}

ساخت SVGDocument_اشاریہ_اندراج
{
  صحیح cmp (hb_codepoint_t g) ثابت
  { واپس g < آغاز_Glyph_ID ? -1 : g > انتہا_Glyph_ID ? 1 : 0; }

  hb_codepoint_t get_آغاز_glyph () ثابت
  { واپس آغاز_Glyph_ID; }

  hb_codepoint_t get_انتہا_glyph () ثابت
  { واپس انتہا_Glyph_ID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, غیرعلامتی صحیح اشاریہ_انحراف) ثابت
  {
    واپس hb_blob_create_sub_blob (svg_blob,
				    اشاریہ_انحراف + (غیرعلامتی صحیح) svgDoc,
				    svg_Doc_لمبائی);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, بنیاد, svg_Doc_لمبائی));
  }

  محفوظ:
  HBUINT16	آغاز_Glyph_ID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	انتہا_Glyph_ID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32_مقصد<Unsized_Array_کا<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svg_Doc_لمبائی;	/* Length of the SVG document.
				 * Must be non-zero. */
  عوامی:
  DEFINE_سائز_STATIC (12);
};

ساخت SVG
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_SVG;

  ساخت svg_id_وسعت_t
  {
    ثابت حرف *p;
    غیرعلامتی len;

    منطقی عامل == (ثابت svg_id_وسعت_t &o) ثابت
    {
      واپس len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ثابت
    {
      uint32_t h = hb_hash (len);
      برائے (غیرعلامتی i = 0; i < len; i++)
        h = h * 33u + (غیرعلامتی حرف) p[i];
      واپس h;
    }
  };

  ساخت svg_defs_اندراج_t
  {
    svg_id_وسعت_t id;
    غیرعلامتی آغاز;
    غیرعلامتی انتہا;
  };

  ساخت svg_doc_cache_t
  {
    hb_blob_t *blob = خالی_اشاریہ;
    ثابت حرف *svg = خالی_اشاریہ;
    غیرعلامتی len = 0;
    hb_سمتیہ_t<svg_defs_اندراج_t> defs_entries;
    hb_codepoint_t آغاز_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t انتہا_glyph = HB_CODEPOINT_INVALID;
    hb_سمتیہ_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_وسعت_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  منطقی has_ڈیٹا () ثابت { واپس svgDocEntries; }

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ثابت
    {
      واپس جدول->get_glyph_اندراج (glyph_id).reference_blob (جدول.get_blob (),
							       جدول->svgDocEntries);
    }

    غیرعلامتی get_document_گنتی () ثابت
    { واپس جدول->get_document_گنتی (); }

    منطقی get_glyph_document_اشاریہ (hb_codepoint_t glyph_id, غیرعلامتی *اشاریہ_2) ثابت
    { واپس جدول->get_glyph_document_اشاریہ (glyph_id, اشاریہ_2); }

    منطقی get_document_glyph_range (غیرعلامتی اشاریہ_2,
                                   hb_codepoint_t *آغاز_glyph,
                                   hb_codepoint_t *انتہا_glyph) ثابت
    { واپس جدول->get_document_glyph_range (اشاریہ_2, آغاز_glyph, انتہا_glyph); }

    منطقی has_ڈیٹا () ثابت { واپس جدول->has_ڈیٹا (); }

    ثابت svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ثابت حرف *svg,
                             غیرعلامتی len,
                             غیرعلامتی doc_اشاریہ,
                             hb_codepoint_t آغاز_glyph,
                             hb_codepoint_t انتہا_glyph) ثابت;

    ثابت حرف *
    doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                       غیرعلامتی *len) ثابت;

    ثابت hb_سمتیہ_t<svg_defs_اندراج_t> *
    doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت;

    منطقی
    doc_cache_get_glyph_وسعت (ثابت svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              غیرعلامتی *آغاز,
                              غیرعلامتی *انتہا) ثابت;

    منطقی
    doc_cache_ڈھونڈو_id_وسعت (ثابت svg_doc_cache_t *doc,
                            svg_id_وسعت_t id,
                            غیرعلامتی *آغاز,
                            غیرعلامتی *انتہا) ثابت;

    منطقی
    doc_cache_ڈھونڈو_id_cstr (ثابت svg_doc_cache_t *doc,
                            ثابت حرف *id,
                            غیرعلامتی *آغاز,
                            غیرعلامتی *انتہا) ثابت;

    منطقی paint_glyph (hb_font_t *font HB_غیر_مستعمل, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, خالی *ڈیٹا) ثابت
    {
      اگر (!has_ڈیٹا ())
        واپس جھوٹ;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      اگر (blob == hb_blob_get_خالی_خانے ())
        واپس جھوٹ;

      منطقی ret = funcs->image (ڈیٹا,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       خالی_اشاریہ);

      hb_blob_destroy (blob);

      واپس ret;
    }

    نجی:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ثابت حرف *svg,
                    غیرعلامتی len,
                    hb_codepoint_t آغاز_glyph,
                    hb_codepoint_t انتہا_glyph) ثابت;

    ساکن خالی destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> جدول;
    تغیرپذیر hb_سمتیہ_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    عوامی:
    DEFINE_سائز_STATIC (حجم (hb_blob_ptr_t<SVG>) +
                        حجم (hb_سمتیہ_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ثابت SVGDocument_اشاریہ_اندراج &get_glyph_اندراج (hb_codepoint_t glyph_id) ثابت
  { واپس (یہ+svgDocEntries).bsearch (glyph_id); }

  غیرعلامتی get_document_گنتی () ثابت
  {
    اگر (!has_ڈیٹا ())
      واپس 0;
    واپس (یہ + svgDocEntries).len;
  }

  منطقی get_glyph_document_اشاریہ (hb_codepoint_t glyph_id, غیرعلامتی *اشاریہ_2) ثابت
  {
    اگر (!has_ڈیٹا ())
      واپس جھوٹ;
    واپس (یہ + svgDocEntries).bfind (glyph_id, اشاریہ_2);
  }

  منطقی get_document_glyph_range (غیرعلامتی اشاریہ_2,
                                 hb_codepoint_t *آغاز_glyph,
                                 hb_codepoint_t *انتہا_glyph) ثابت
  {
    اگر (!has_ڈیٹا ())
      واپس جھوٹ;

    ثابت خودکار &entries = یہ + svgDocEntries;
    اگر (اشاریہ_2 >= entries.len)
      واپس جھوٹ;

    ثابت خودکار &اندراج_2 = entries.arrayZ[اشاریہ_2];
    اگر (آغاز_glyph) *آغاز_glyph = اندراج_2.get_آغاز_glyph ();
    اگر (انتہا_glyph) *انتہا_glyph = اندراج_2.get_انتہا_glyph ();
    واپس سچ;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  (یہ+svgDocEntries).sanitize_shallow (c)));
  }

  محفوظ:
  HBUINT16	نسخہ;	/* Table version (starting at 0). */
  Offset32_مقصد<Sorted_Array16_کا<SVGDocument_اشاریہ_اندراج>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  عوامی:
  DEFINE_سائز_STATIC (10);
};

نامخلا _hb_svg_cache_impl {

ساخت glyph_اندراج_t
{
  hb_codepoint_t glyph;
  uint32_t آغاز;
  uint32_t انتہا;
};

ساخت id_اندراج_t
{
  SVG::svg_id_وسعت_t id;
  uint32_t آغاز;
  uint32_t انتہا;
};

ساخت open_elem_t
{
  غیرعلامتی آغاز;
  SVG::svg_id_وسعت_t id;
  منطقی اندر_defs_content;
  منطقی is_defs;
};

ساکن ثابت غیرعلامتی MAX_گہرائی = 128;

ساکن اندرونی صحیح
ڈھونڈو_substr (ثابت حرف *s,
             غیرعلامتی n,
             غیرعلامتی from,
             ثابت حرف *needle,
             غیرعلامتی needle_len)
{
  اگر (!needle_len || from >= n || needle_len > n)
    واپس -1;
  برائے (غیرعلامتی i = from; i + needle_len <= n; i++)
    اگر (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      واپس (صحیح) i;
  واپس -1;
}

ساکن اندرونی منطقی
parse_id_اندر_آغاز_لیبل (ثابت حرف *svg,
                       غیرعلامتی لیبل_آغاز,
                       غیرعلامتی لیبل_انتہا,
                       SVG::svg_id_وسعت_t *id)
{
  غیرعلامتی p = لیبل_آغاز;
  جب (p + 4 <= لیبل_انتہا)
  {
    اگر (!memcmp (svg + p, "id=\"", 4))
    {
      غیرعلامتی b = p + 4;
      غیرعلامتی e = b;
      جب (e < لیبل_انتہا && svg[e] != '"') e++;
      اگر (e <= لیبل_انتہا && e > b)
      {
        *id = {svg + b, e - b};
        واپس سچ;
      }
    }
    اگر (!memcmp (svg + p, "id='", 4))
    {
      غیرعلامتی b = p + 4;
      غیرعلامتی e = b;
      جب (e < لیبل_انتہا && svg[e] != '\'') e++;
      اگر (e <= لیبل_انتہا && e > b)
      {
        *id = {svg + b, e - b};
        واپس سچ;
      }
    }
    p++;
  }
  واپس جھوٹ;
}

ساکن اندرونی منطقی
parse_glyph_id_وسعت (ثابت SVG::svg_id_وسعت_t &id,
                     hb_codepoint_t *glyph)
{
  اگر (id.len <= 5 || memcmp (id.p, "glyph", 5))
    واپس جھوٹ;

  hb_codepoint_t gid = 0;
  برائے (غیرعلامتی i = 5; i < id.len; i++)
  {
    غیرعلامتی حرف c = (غیرعلامتی حرف) id.p[i];
    اگر (c < '0' || c > '9')
      واپس جھوٹ;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    اگر (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      واپس جھوٹ;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  واپس سچ;
}

ساکن اندرونی منطقی
parse_cache_entries_خطی (ثابت حرف *svg,
                            غیرعلامتی len,
                            hb_سمتیہ_t<SVG::svg_defs_اندراج_t> *defs_entries,
                            hb_سمتیہ_t<glyph_اندراج_t> *glyph_spans,
                            hb_سمتیہ_t<id_اندراج_t> *id_entries)
{
  open_elem_t ڈھیر[MAX_گہرائی] = {};
  غیرعلامتی گہرائی = 0;
  اگر (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    واپس جھوٹ;

  غیرعلامتی defs_گہرائی = 0;
  غیرعلامتی i = 0;
  جب (i < len)
  {
    اگر (svg[i] != '<')
    {
      i++;
      جاری;
    }

    اگر (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      صحیح cend = ڈھونڈو_substr (svg, len, i + 4, "-->", 3);
      اگر (cend < 0) واپس جھوٹ;
      i = (غیرعلامتی) cend + 3;
      جاری;
    }
    اگر (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      صحیح cend = ڈھونڈو_substr (svg, len, i + 9, "]]>", 3);
      اگر (cend < 0) واپس جھوٹ;
      i = (غیرعلامتی) cend + 3;
      جاری;
    }

    منطقی closing = (i + 1 < len && svg[i + 1] == '/');
    منطقی special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    غیرعلامتی gt = i + 1;
    حرف quote = 0;
    جب (gt < len)
    {
      حرف c = svg[gt];
      اگر (quote)
      {
        اگر (c == quote) quote = 0;
      }
      ورنہ
      {
        اگر (c == '"' || c == '\'')
          quote = c;
        ورنہ اگر (c == '>')
          توڑو;
      }
      gt++;
    }
    اگر (gt >= len)
      واپس جھوٹ;

    اگر (special)
    {
      i = gt + 1;
      جاری;
    }

    غیرعلامتی p = i + (closing ? 2 : 1);
    جب (p < gt && خلا_ہے ((غیرعلامتی حرف) svg[p])) p++;
    ثابت حرف *name = svg + p;
    غیرعلامتی name_len = 0;
    جب (p + name_len < gt)
    {
      غیرعلامتی حرف c = (غیرعلامتی حرف) name[name_len];
      اگر (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        توڑو;
      name_len++;
    }
    منطقی is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    اگر (closing)
    {
      اگر (!گہرائی)
      {
        i = gt + 1;
        جاری;
      }

      open_elem_t e = ڈھیر[--گہرائی];
      غیرعلامتی انتہا = gt + 1;

      اگر (e.id.len)
      {
        اگر (unlikely (!id_entries->ڈالو_or_fail (id_اندراج_t {e.id, (uint32_t) e.آغاز, (uint32_t) انتہا})))
          واپس جھوٹ;

        اگر (e.اندر_defs_content)
        {
          اگر (unlikely (!defs_entries->ڈالو_or_fail ()))
            واپس جھوٹ;
          خودکار &خانہ = defs_entries->دم ();
          خانہ.id = e.id;
          خانہ.آغاز = e.آغاز;
          خانہ.انتہا = انتہا;
        }

        hb_codepoint_t gid;
        اگر (parse_glyph_id_وسعت (e.id, &gid))
        {
          اگر (unlikely (!glyph_spans->ڈالو_or_fail (glyph_اندراج_t {gid, (uint32_t) e.آغاز, (uint32_t) انتہا})))
            واپس جھوٹ;
        }
      }

      اگر (e.is_defs && defs_گہرائی)
        defs_گہرائی--;

      i = انتہا;
      جاری;
    }

    SVG::svg_id_وسعت_t id = {};
    parse_id_اندر_آغاز_لیبل (svg, i, gt, &id);

    غیرعلامتی r = gt;
    جب (r > i && خلا_ہے ((غیرعلامتی حرف) svg[r - 1])) r--;
    منطقی self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.آغاز = i;
    e.id = id;
    e.اندر_defs_content = defs_گہرائی > 0;
    e.is_defs = is_defs;

    اگر (self_closing)
    {
      غیرعلامتی انتہا = gt + 1;
      اگر (e.id.len)
      {
        اگر (unlikely (!id_entries->ڈالو_or_fail (id_اندراج_t {e.id, (uint32_t) e.آغاز, (uint32_t) انتہا})))
          واپس جھوٹ;

        اگر (e.اندر_defs_content)
        {
          اگر (unlikely (!defs_entries->ڈالو_or_fail ()))
            واپس جھوٹ;
          خودکار &خانہ = defs_entries->دم ();
          خانہ.id = e.id;
          خانہ.آغاز = e.آغاز;
          خانہ.انتہا = انتہا;
        }

        hb_codepoint_t gid;
        اگر (parse_glyph_id_وسعت (e.id, &gid))
        {
          اگر (unlikely (!glyph_spans->ڈالو_or_fail (glyph_اندراج_t {gid, (uint32_t) e.آغاز, (uint32_t) انتہا})))
            واپس جھوٹ;
        }
      }
    }
    ورنہ
    {
      اگر (unlikely (گہرائی >= MAX_گہرائی))
        واپس جھوٹ;
      ڈھیر[گہرائی++] = e;
      اگر (is_defs)
        defs_گہرائی++;
    }

    i = gt + 1;
  }

  واپس سچ;
}

} /* namespace _hb_svg_cache_impl */

اندرونی
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  جدول = hb_sanitize_context_t ().reference_جدول<SVG> (face);
  doc_caches.init ();
  غیرعلامتی doc_گنتی = جدول->get_document_گنتی ();
  اگر (doc_گنتی && unlikely (!doc_caches.resize (doc_گنتی)))
    doc_caches.clear ();
  برائے (غیرعلامتی i = 0; i < doc_caches.لمبائی; i++)
    doc_caches.arrayZ[i].سیٹ_relaxed (خالی_اشاریہ);
}

اندرونی
SVG::accelerator_t::~accelerator_t ()
{
  برائے (غیرعلامتی i = 0; i < doc_caches.لمبائی; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  جدول.destroy ();
}

اندرونی خالی
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  اگر (!doc)
    واپس;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

اندرونی SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    ثابت حرف *svg,
                                    غیرعلامتی len,
                                    hb_codepoint_t آغاز_glyph,
                                    hb_codepoint_t انتہا_glyph) ثابت
{
  ساکن ثابت uint32_t INVALID_وسعت = 0xFFFFFFFFu;

  خودکار *doc = (svg_doc_cache_t *) hb_malloc (حجم (svg_doc_cache_t));
  اگر (!doc)
    واپس خالی_اشاریہ;

  doc->blob = خالی_اشاریہ;
  doc->svg = خالی_اشاریہ;
  doc->len = 0;
  doc->defs_entries.init ();
  doc->آغاز_glyph = HB_CODEPOINT_INVALID;
  doc->انتہا_glyph = HB_CODEPOINT_INVALID;
  doc->glyph_spans.init ();
  doc->id_spans.init ();

  doc->blob = hb_blob_reference (image);
  doc->svg = svg;
  doc->len = len;
  doc->آغاز_glyph = آغاز_glyph;
  doc->انتہا_glyph = انتہا_glyph;

  اگر (unlikely (آغاز_glyph == HB_CODEPOINT_INVALID || انتہا_glyph < آغاز_glyph))
  {
    destroy_doc_cache (doc);
    واپس خالی_اشاریہ;
  }

  غیرعلامتی glyph_گنتی = انتہا_glyph - آغاز_glyph + 1;
  اگر (!doc->glyph_spans.resize ((صحیح) glyph_گنتی))
  {
    destroy_doc_cache (doc);
    واپس خالی_اشاریہ;
  }
  برائے (غیرعلامتی i = 0; i < glyph_گنتی; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_وسعت, INVALID_وسعت);

  hb_سمتیہ_t<_hb_svg_cache_impl::glyph_اندراج_t> glyph_spans;
  glyph_spans.init ();
  hb_سمتیہ_t<_hb_svg_cache_impl::id_اندراج_t> id_entries;
  id_entries.init ();
  اگر (!_hb_svg_cache_impl::parse_cache_entries_خطی (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    واپس خالی_اشاریہ;
  }

  برائے (غیرعلامتی i = 0; i < glyph_spans.لمبائی; i++)
  {
    ثابت خودکار &وسعت = glyph_spans.arrayZ[i];
    اگر (unlikely (وسعت.glyph < آغاز_glyph || وسعت.glyph > انتہا_glyph))
      جاری;
    doc->glyph_spans.arrayZ[وسعت.glyph - آغاز_glyph] = hb_pair_t<uint32_t, uint32_t> (وسعت.آغاز, وسعت.انتہا);
  }

  برائے (غیرعلامتی i = 0; i < id_entries.لمبائی; i++)
  {
    ثابت خودکار &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *برآمد = خالی_اشاریہ;
    اگر (doc->id_spans.has (e.id, &برآمد))
      جاری;
    اگر (unlikely (!doc->id_spans.سیٹ (e.id, hb_pair_t<uint32_t, uint32_t> (e.آغاز, e.انتہا))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      واپس خالی_اشاریہ;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  واپس doc;
}

اندرونی ثابت SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ثابت حرف *svg,
                                              غیرعلامتی len,
                                              غیرعلامتی doc_اشاریہ,
                                              hb_codepoint_t آغاز_glyph,
                                              hb_codepoint_t انتہا_glyph) ثابت
{
  اگر (doc_اشاریہ >= doc_caches.لمبائی)
    واپس خالی_اشاریہ;

  خودکار &خانہ = doc_caches.arrayZ[doc_اشاریہ];
  خودکار *doc = خانہ.get_acquire ();
  اگر (doc)
    واپس doc;

  خودکار *fresh = make_doc_cache (image, svg, len, آغاز_glyph, انتہا_glyph);
  اگر (!fresh)
    واپس خالی_اشاریہ;

  خودکار *expected = (svg_doc_cache_t *) خالی_اشاریہ;
  اگر (خانہ.cmpexch (expected, fresh))
    واپس fresh;

  destroy_doc_cache (fresh);
  واپس expected;
}

اندرونی ثابت حرف *
SVG::accelerator_t::doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                                       غیرعلامتی *len) ثابت
{
  اگر (!doc)
  {
    اگر (len) *len = 0;
    واپس خالی_اشاریہ;
  }
  اگر (len) *len = doc->len;
  واپس doc->svg;
}

اندرونی ثابت hb_سمتیہ_t<SVG::svg_defs_اندراج_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت
{
  واپس doc ? &doc->defs_entries : خالی_اشاریہ;
}

اندرونی منطقی
SVG::accelerator_t::doc_cache_get_glyph_وسعت (ثابت svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              غیرعلامتی *آغاز,
                                              غیرعلامتی *انتہا) ثابت
{
  ساکن ثابت uint32_t INVALID_وسعت = 0xFFFFFFFFu;
  اگر (!doc || doc->آغاز_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->آغاز_glyph || glyph > doc->انتہا_glyph)
    واپس جھوٹ;

  ثابت خودکار &وسعت = doc->glyph_spans.arrayZ[glyph - doc->آغاز_glyph];
  اگر (وسعت.پہلا == INVALID_وسعت)
    واپس جھوٹ;
  اگر (unlikely (وسعت.پہلا > وسعت.second || وسعت.second > doc->len))
    واپس جھوٹ;

  اگر (آغاز) *آغاز = وسعت.پہلا;
  اگر (انتہا) *انتہا = وسعت.second;
  واپس سچ;
}

اندرونی منطقی
SVG::accelerator_t::doc_cache_ڈھونڈو_id_وسعت (ثابت svg_doc_cache_t *doc,
                                            svg_id_وسعت_t id,
                                            غیرعلامتی *آغاز,
                                            غیرعلامتی *انتہا) ثابت
{
  اگر (!doc || !id.p || !id.len)
    واپس جھوٹ;
  hb_pair_t<uint32_t, uint32_t> *وسعت = خالی_اشاریہ;
  اگر (!doc->id_spans.has (id, &وسعت))
    واپس جھوٹ;
  اگر (unlikely (وسعت->پہلا > وسعت->second || وسعت->second > doc->len))
    واپس جھوٹ;
  اگر (آغاز) *آغاز = وسعت->پہلا;
  اگر (انتہا) *انتہا = وسعت->second;
  واپس سچ;
}

اندرونی منطقی
SVG::accelerator_t::doc_cache_ڈھونڈو_id_cstr (ثابت svg_doc_cache_t *doc,
                                            ثابت حرف *id,
                                            غیرعلامتی *آغاز,
                                            غیرعلامتی *انتہا) ثابت
{
  اگر (!id) واپس جھوٹ;
  svg_id_وسعت_t key = {id, (غیرعلامتی) لڑی_لمبائی (id)};
  واپس doc_cache_ڈھونڈو_id_وسعت (doc, key, آغاز, انتہا);
}

ساخت SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#اگر_ختم /* OT_COLOR_SVG_SVG_HH */
