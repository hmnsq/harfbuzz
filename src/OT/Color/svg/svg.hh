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

#که_نه_تعریف_شوی OT_COLOR_SVG_SVG_HH
#تعریف_کړه OT_COLOR_SVG_SVG_HH

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

#تعریف_کړه HB_OT_نښان_SVG HB_نښان('S','V','G',' ')


نطاق OT {

ساكن مضمن hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     ثابت حرف **svg,
                                     بینښه *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  بینښه معلومات_len = 0;
  ثابت حرف *معلومات = hb_blob_get_معلومات (blob, &معلومات_len);

  إذا (!معلومات || !معلومات_len)
    اذهب fail;

  إذا (hb_blob_is_gzip (معلومات, معلومات_len))
  {
    uint32_t expected_اندازه = 0;
    إذا (hb_gzip_get_uncompressed_اندازه (معلومات, معلومات_len, &expected_اندازه) &&
        unlikely ((size_t) expected_اندازه > (size_t) HB_SVG_MAX_DOCUMENT_اندازه))
      اذهب fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_اندازه);
    إذا (!uncompressed)
      اذهب fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    معلومات = hb_blob_get_معلومات (blob, &معلومات_len);
    إذا (!معلومات || !معلومات_len)
      اذهب fail;
  }

  إذا (unlikely ((size_t) معلومات_len > (size_t) HB_SVG_MAX_DOCUMENT_اندازه))
    اذهب fail;

  إذا (svg) *svg = معلومات;
  إذا (len) *len = معلومات_len;
  أرجع blob;

fail:
  hb_blob_destroy (blob);
  إذا (svg) *svg = تش_اشاره;
  إذا (len) *len = 0;
  أرجع تش_اشاره;
}

بنية SVGDocument_شاخص_ثبت
{
  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < پیل_Glyph_ID ? -1 : g > پای_ټکی_Glyph_ID ? 1 : 0; }

  hb_codepoint_t get_پیل_glyph () ثابت
  { أرجع پیل_Glyph_ID; }

  hb_codepoint_t get_پای_ټکی_glyph () ثابت
  { أرجع پای_ټکی_Glyph_ID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, بینښه صحيح شاخص_دننه_واټن) ثابت
  {
    أرجع hb_blob_create_sub_blob (svg_blob,
				    شاخص_دننه_واټن + (بینښه صحيح) svgDoc,
				    svg_Doc_اوږدتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, بنسټ, svg_Doc_اوږدتیا));
  }

  محمي:
  HBUINT16	پیل_Glyph_ID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	پای_ټکی_Glyph_ID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32_منزل<Unsized_Array_د<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svg_Doc_اوږدتیا;	/* Length of the SVG document.
				 * Must be non-zero. */
  عام:
  DEFINE_اندازه_STATIC (12);
};

بنية SVG
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_SVG;

  بنية svg_id_پراختیا_t
  {
    ثابت حرف *p;
    بینښه len;

    منطقي چلوونکی == (ثابت svg_id_پراختیا_t &o) ثابت
    {
      أرجع len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () ثابت
    {
      uint32_t h = hb_hash (len);
      لكل (بینښه i = 0; i < len; i++)
        h = h * 33u + (بینښه حرف) p[i];
      أرجع h;
    }
  };

  بنية svg_defs_ثبت_t
  {
    svg_id_پراختیا_t id;
    بینښه پیل;
    بینښه پای_ټکی;
  };

  بنية svg_doc_cache_t
  {
    hb_blob_t *blob = تش_اشاره;
    ثابت حرف *svg = تش_اشاره;
    بینښه len = 0;
    hb_ویکټور_t<svg_defs_ثبت_t> defs_entries;
    hb_codepoint_t پیل_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t پای_ټکی_glyph = HB_CODEPOINT_INVALID;
    hb_ویکټور_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_پراختیا_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  منطقي has_معلومات () ثابت { أرجع svgDocEntries; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) ثابت
    {
      أرجع جدول->get_glyph_ثبت (glyph_id).reference_blob (جدول.get_blob (),
							       جدول->svgDocEntries);
    }

    بینښه get_document_شمېر () ثابت
    { أرجع جدول->get_document_شمېر (); }

    منطقي get_glyph_document_شاخص (hb_codepoint_t glyph_id, بینښه *شاخص_2) ثابت
    { أرجع جدول->get_glyph_document_شاخص (glyph_id, شاخص_2); }

    منطقي get_document_glyph_range (بینښه شاخص_2,
                                   hb_codepoint_t *پیل_glyph,
                                   hb_codepoint_t *پای_ټکی_glyph) ثابت
    { أرجع جدول->get_document_glyph_range (شاخص_2, پیل_glyph, پای_ټکی_glyph); }

    منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

    ثابت svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             ثابت حرف *svg,
                             بینښه len,
                             بینښه doc_شاخص,
                             hb_codepoint_t پیل_glyph,
                             hb_codepoint_t پای_ټکی_glyph) ثابت;

    ثابت حرف *
    doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                       بینښه *len) ثابت;

    ثابت hb_ویکټور_t<svg_defs_ثبت_t> *
    doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت;

    منطقي
    doc_cache_get_glyph_پراختیا (ثابت svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              بینښه *پیل,
                              بینښه *پای_ټکی) ثابت;

    منطقي
    doc_cache_پیدا_کړه_id_پراختیا (ثابت svg_doc_cache_t *doc,
                            svg_id_پراختیا_t id,
                            بینښه *پیل,
                            بینښه *پای_ټکی) ثابت;

    منطقي
    doc_cache_پیدا_کړه_id_cstr (ثابت svg_doc_cache_t *doc,
                            ثابت حرف *id,
                            بینښه *پیل,
                            بینښه *پای_ټکی) ثابت;

    منطقي paint_glyph (hb_font_t *font HB_ناکارول_شوی, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *معلومات) ثابت
    {
      إذا (!has_معلومات ())
        أرجع خطأ;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      إذا (blob == hb_blob_get_تشه ())
        أرجع خطأ;

      منطقي ret = funcs->image (معلومات,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       تش_اشاره);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    ثابت حرف *svg,
                    بینښه len,
                    hb_codepoint_t پیل_glyph,
                    hb_codepoint_t پای_ټکی_glyph) ثابت;

    ساكن فراغ destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> جدول;
    بدلیدونکی hb_ویکټور_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    عام:
    DEFINE_اندازه_STATIC (حجم (hb_blob_ptr_t<SVG>) +
                        حجم (hb_ویکټور_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  ثابت SVGDocument_شاخص_ثبت &get_glyph_ثبت (hb_codepoint_t glyph_id) ثابت
  { أرجع (هذا+svgDocEntries).bsearch (glyph_id); }

  بینښه get_document_شمېر () ثابت
  {
    إذا (!has_معلومات ())
      أرجع 0;
    أرجع (هذا + svgDocEntries).len;
  }

  منطقي get_glyph_document_شاخص (hb_codepoint_t glyph_id, بینښه *شاخص_2) ثابت
  {
    إذا (!has_معلومات ())
      أرجع خطأ;
    أرجع (هذا + svgDocEntries).bfind (glyph_id, شاخص_2);
  }

  منطقي get_document_glyph_range (بینښه شاخص_2,
                                 hb_codepoint_t *پیل_glyph,
                                 hb_codepoint_t *پای_ټکی_glyph) ثابت
  {
    إذا (!has_معلومات ())
      أرجع خطأ;

    ثابت خپلکاری &entries = هذا + svgDocEntries;
    إذا (شاخص_2 >= entries.len)
      أرجع خطأ;

    ثابت خپلکاری &ثبت = entries.arrayZ[شاخص_2];
    إذا (پیل_glyph) *پیل_glyph = ثبت.get_پیل_glyph ();
    إذا (پای_ټکی_glyph) *پای_ټکی_glyph = ثبت.get_پای_ټکی_glyph ();
    أرجع ریښتیا;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  (هذا+svgDocEntries).sanitize_shallow (c)));
  }

  محمي:
  HBUINT16	نسخه;	/* Table version (starting at 0). */
  Offset32_منزل<Sorted_Array16_د<SVGDocument_شاخص_ثبت>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  عام:
  DEFINE_اندازه_STATIC (10);
};

نطاق _hb_svg_cache_impl {

بنية glyph_ثبت_t
{
  hb_codepoint_t glyph;
  uint32_t پیل;
  uint32_t پای_ټکی;
};

بنية id_ثبت_t
{
  SVG::svg_id_پراختیا_t id;
  uint32_t پیل;
  uint32_t پای_ټکی;
};

بنية open_elem_t
{
  بینښه پیل;
  SVG::svg_id_پراختیا_t id;
  منطقي دننه_defs_content;
  منطقي is_defs;
};

ساكن ثابت بینښه MAX_ژوروالی = 128;

ساكن مضمن صحيح
پیدا_کړه_substr (ثابت حرف *s,
             بینښه n,
             بینښه from,
             ثابت حرف *needle,
             بینښه needle_len)
{
  إذا (!needle_len || from >= n || needle_len > n)
    أرجع -1;
  لكل (بینښه i = from; i + needle_len <= n; i++)
    إذا (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      أرجع (صحيح) i;
  أرجع -1;
}

ساكن مضمن منطقي
parse_id_دننه_پیل_نښان (ثابت حرف *svg,
                       بینښه نښان_پیل,
                       بینښه نښان_پای_ټکی,
                       SVG::svg_id_پراختیا_t *id)
{
  بینښه p = نښان_پیل;
  طالما (p + 4 <= نښان_پای_ټکی)
  {
    إذا (!memcmp (svg + p, "id=\"", 4))
    {
      بینښه b = p + 4;
      بینښه e = b;
      طالما (e < نښان_پای_ټکی && svg[e] != '"') e++;
      إذا (e <= نښان_پای_ټکی && e > b)
      {
        *id = {svg + b, e - b};
        أرجع ریښتیا;
      }
    }
    إذا (!memcmp (svg + p, "id='", 4))
    {
      بینښه b = p + 4;
      بینښه e = b;
      طالما (e < نښان_پای_ټکی && svg[e] != '\'') e++;
      إذا (e <= نښان_پای_ټکی && e > b)
      {
        *id = {svg + b, e - b};
        أرجع ریښتیا;
      }
    }
    p++;
  }
  أرجع خطأ;
}

ساكن مضمن منطقي
parse_glyph_id_پراختیا (ثابت SVG::svg_id_پراختیا_t &id,
                     hb_codepoint_t *glyph)
{
  إذا (id.len <= 5 || memcmp (id.p, "glyph", 5))
    أرجع خطأ;

  hb_codepoint_t gid = 0;
  لكل (بینښه i = 5; i < id.len; i++)
  {
    بینښه حرف c = (بینښه حرف) id.p[i];
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
  أرجع ریښتیا;
}

ساكن مضمن منطقي
parse_cache_entries_خطي (ثابت حرف *svg,
                            بینښه len,
                            hb_ویکټور_t<SVG::svg_defs_ثبت_t> *defs_entries,
                            hb_ویکټور_t<glyph_ثبت_t> *glyph_spans,
                            hb_ویکټور_t<id_ثبت_t> *id_entries)
{
  open_elem_t ډېری[MAX_ژوروالی] = {};
  بینښه ژوروالی = 0;
  إذا (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    أرجع خطأ;

  بینښه defs_ژوروالی = 0;
  بینښه i = 0;
  طالما (i < len)
  {
    إذا (svg[i] != '<')
    {
      i++;
      استمر;
    }

    إذا (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      صحيح cend = پیدا_کړه_substr (svg, len, i + 4, "-->", 3);
      إذا (cend < 0) أرجع خطأ;
      i = (بینښه) cend + 3;
      استمر;
    }
    إذا (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      صحيح cend = پیدا_کړه_substr (svg, len, i + 9, "]]>", 3);
      إذا (cend < 0) أرجع خطأ;
      i = (بینښه) cend + 3;
      استمر;
    }

    منطقي closing = (i + 1 < len && svg[i + 1] == '/');
    منطقي special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    بینښه gt = i + 1;
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

    بینښه p = i + (closing ? 2 : 1);
    طالما (p < gt && که_تشه ((بینښه حرف) svg[p])) p++;
    ثابت حرف *name = svg + p;
    بینښه name_len = 0;
    طالما (p + name_len < gt)
    {
      بینښه حرف c = (بینښه حرف) name[name_len];
      إذا (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        اكسر;
      name_len++;
    }
    منطقي is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    إذا (closing)
    {
      إذا (!ژوروالی)
      {
        i = gt + 1;
        استمر;
      }

      open_elem_t e = ډېری[--ژوروالی];
      بینښه پای_ټکی = gt + 1;

      إذا (e.id.len)
      {
        إذا (unlikely (!id_entries->ټېل_or_fail (id_ثبت_t {e.id, (uint32_t) e.پیل, (uint32_t) پای_ټکی})))
          أرجع خطأ;

        إذا (e.دننه_defs_content)
        {
          إذا (unlikely (!defs_entries->ټېل_or_fail ()))
            أرجع خطأ;
          خپلکاری &خونه = defs_entries->لکۍ ();
          خونه.id = e.id;
          خونه.پیل = e.پیل;
          خونه.پای_ټکی = پای_ټکی;
        }

        hb_codepoint_t gid;
        إذا (parse_glyph_id_پراختیا (e.id, &gid))
        {
          إذا (unlikely (!glyph_spans->ټېل_or_fail (glyph_ثبت_t {gid, (uint32_t) e.پیل, (uint32_t) پای_ټکی})))
            أرجع خطأ;
        }
      }

      إذا (e.is_defs && defs_ژوروالی)
        defs_ژوروالی--;

      i = پای_ټکی;
      استمر;
    }

    SVG::svg_id_پراختیا_t id = {};
    parse_id_دننه_پیل_نښان (svg, i, gt, &id);

    بینښه r = gt;
    طالما (r > i && که_تشه ((بینښه حرف) svg[r - 1])) r--;
    منطقي self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.پیل = i;
    e.id = id;
    e.دننه_defs_content = defs_ژوروالی > 0;
    e.is_defs = is_defs;

    إذا (self_closing)
    {
      بینښه پای_ټکی = gt + 1;
      إذا (e.id.len)
      {
        إذا (unlikely (!id_entries->ټېل_or_fail (id_ثبت_t {e.id, (uint32_t) e.پیل, (uint32_t) پای_ټکی})))
          أرجع خطأ;

        إذا (e.دننه_defs_content)
        {
          إذا (unlikely (!defs_entries->ټېل_or_fail ()))
            أرجع خطأ;
          خپلکاری &خونه = defs_entries->لکۍ ();
          خونه.id = e.id;
          خونه.پیل = e.پیل;
          خونه.پای_ټکی = پای_ټکی;
        }

        hb_codepoint_t gid;
        إذا (parse_glyph_id_پراختیا (e.id, &gid))
        {
          إذا (unlikely (!glyph_spans->ټېل_or_fail (glyph_ثبت_t {gid, (uint32_t) e.پیل, (uint32_t) پای_ټکی})))
            أرجع خطأ;
        }
      }
    }
    وإلا
    {
      إذا (unlikely (ژوروالی >= MAX_ژوروالی))
        أرجع خطأ;
      ډېری[ژوروالی++] = e;
      إذا (is_defs)
        defs_ژوروالی++;
    }

    i = gt + 1;
  }

  أرجع ریښتیا;
}

} /* namespace _hb_svg_cache_impl */

مضمن
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  جدول = hb_sanitize_context_t ().reference_جدول<SVG> (face);
  doc_caches.init ();
  بینښه doc_شمېر = جدول->get_document_شمېر ();
  إذا (doc_شمېر && unlikely (!doc_caches.resize (doc_شمېر)))
    doc_caches.clear ();
  لكل (بینښه i = 0; i < doc_caches.اوږدتیا; i++)
    doc_caches.arrayZ[i].سېټ_relaxed (تش_اشاره);
}

مضمن
SVG::accelerator_t::~accelerator_t ()
{
  لكل (بینښه i = 0; i < doc_caches.اوږدتیا; i++)
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
                                    بینښه len,
                                    hb_codepoint_t پیل_glyph,
                                    hb_codepoint_t پای_ټکی_glyph) ثابت
{
  ساكن ثابت uint32_t INVALID_پراختیا = 0xFFFFFFFFu;

  خپلکاری *doc = (svg_doc_cache_t *) hb_malloc (حجم (svg_doc_cache_t));
  إذا (!doc)
    أرجع تش_اشاره;

  doc->blob = تش_اشاره;
  doc->svg = تش_اشاره;
  doc->len = 0;
  doc->defs_entries.init ();
  doc->پیل_glyph = HB_CODEPOINT_INVALID;
  doc->پای_ټکی_glyph = HB_CODEPOINT_INVALID;
  doc->glyph_spans.init ();
  doc->id_spans.init ();

  doc->blob = hb_blob_reference (image);
  doc->svg = svg;
  doc->len = len;
  doc->پیل_glyph = پیل_glyph;
  doc->پای_ټکی_glyph = پای_ټکی_glyph;

  إذا (unlikely (پیل_glyph == HB_CODEPOINT_INVALID || پای_ټکی_glyph < پیل_glyph))
  {
    destroy_doc_cache (doc);
    أرجع تش_اشاره;
  }

  بینښه glyph_شمېر = پای_ټکی_glyph - پیل_glyph + 1;
  إذا (!doc->glyph_spans.resize ((صحيح) glyph_شمېر))
  {
    destroy_doc_cache (doc);
    أرجع تش_اشاره;
  }
  لكل (بینښه i = 0; i < glyph_شمېر; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_پراختیا, INVALID_پراختیا);

  hb_ویکټور_t<_hb_svg_cache_impl::glyph_ثبت_t> glyph_spans;
  glyph_spans.init ();
  hb_ویکټور_t<_hb_svg_cache_impl::id_ثبت_t> id_entries;
  id_entries.init ();
  إذا (!_hb_svg_cache_impl::parse_cache_entries_خطي (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    أرجع تش_اشاره;
  }

  لكل (بینښه i = 0; i < glyph_spans.اوږدتیا; i++)
  {
    ثابت خپلکاری &پراختیا = glyph_spans.arrayZ[i];
    إذا (unlikely (پراختیا.glyph < پیل_glyph || پراختیا.glyph > پای_ټکی_glyph))
      استمر;
    doc->glyph_spans.arrayZ[پراختیا.glyph - پیل_glyph] = hb_pair_t<uint32_t, uint32_t> (پراختیا.پیل, پراختیا.پای_ټکی);
  }

  لكل (بینښه i = 0; i < id_entries.اوږدتیا; i++)
  {
    ثابت خپلکاری &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *بهر = تش_اشاره;
    إذا (doc->id_spans.has (e.id, &بهر))
      استمر;
    إذا (unlikely (!doc->id_spans.سېټ (e.id, hb_pair_t<uint32_t, uint32_t> (e.پیل, e.پای_ټکی))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      أرجع تش_اشاره;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  أرجع doc;
}

مضمن ثابت SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              ثابت حرف *svg,
                                              بینښه len,
                                              بینښه doc_شاخص,
                                              hb_codepoint_t پیل_glyph,
                                              hb_codepoint_t پای_ټکی_glyph) ثابت
{
  إذا (doc_شاخص >= doc_caches.اوږدتیا)
    أرجع تش_اشاره;

  خپلکاری &خونه = doc_caches.arrayZ[doc_شاخص];
  خپلکاری *doc = خونه.get_acquire ();
  إذا (doc)
    أرجع doc;

  خپلکاری *fresh = make_doc_cache (image, svg, len, پیل_glyph, پای_ټکی_glyph);
  إذا (!fresh)
    أرجع تش_اشاره;

  خپلکاری *expected = (svg_doc_cache_t *) تش_اشاره;
  إذا (خونه.cmpexch (expected, fresh))
    أرجع fresh;

  destroy_doc_cache (fresh);
  أرجع expected;
}

مضمن ثابت حرف *
SVG::accelerator_t::doc_cache_get_svg (ثابت svg_doc_cache_t *doc,
                                       بینښه *len) ثابت
{
  إذا (!doc)
  {
    إذا (len) *len = 0;
    أرجع تش_اشاره;
  }
  إذا (len) *len = doc->len;
  أرجع doc->svg;
}

مضمن ثابت hb_ویکټور_t<SVG::svg_defs_ثبت_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (ثابت svg_doc_cache_t *doc) ثابت
{
  أرجع doc ? &doc->defs_entries : تش_اشاره;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_get_glyph_پراختیا (ثابت svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              بینښه *پیل,
                                              بینښه *پای_ټکی) ثابت
{
  ساكن ثابت uint32_t INVALID_پراختیا = 0xFFFFFFFFu;
  إذا (!doc || doc->پیل_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->پیل_glyph || glyph > doc->پای_ټکی_glyph)
    أرجع خطأ;

  ثابت خپلکاری &پراختیا = doc->glyph_spans.arrayZ[glyph - doc->پیل_glyph];
  إذا (پراختیا.لومړی == INVALID_پراختیا)
    أرجع خطأ;
  إذا (unlikely (پراختیا.لومړی > پراختیا.second || پراختیا.second > doc->len))
    أرجع خطأ;

  إذا (پیل) *پیل = پراختیا.لومړی;
  إذا (پای_ټکی) *پای_ټکی = پراختیا.second;
  أرجع ریښتیا;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_پیدا_کړه_id_پراختیا (ثابت svg_doc_cache_t *doc,
                                            svg_id_پراختیا_t id,
                                            بینښه *پیل,
                                            بینښه *پای_ټکی) ثابت
{
  إذا (!doc || !id.p || !id.len)
    أرجع خطأ;
  hb_pair_t<uint32_t, uint32_t> *پراختیا = تش_اشاره;
  إذا (!doc->id_spans.has (id, &پراختیا))
    أرجع خطأ;
  إذا (unlikely (پراختیا->لومړی > پراختیا->second || پراختیا->second > doc->len))
    أرجع خطأ;
  إذا (پیل) *پیل = پراختیا->لومړی;
  إذا (پای_ټکی) *پای_ټکی = پراختیا->second;
  أرجع ریښتیا;
}

مضمن منطقي
SVG::accelerator_t::doc_cache_پیدا_کړه_id_cstr (ثابت svg_doc_cache_t *doc,
                                            ثابت حرف *id,
                                            بینښه *پیل,
                                            بینښه *پای_ټکی) ثابت
{
  إذا (!id) أرجع خطأ;
  svg_id_پراختیا_t key = {id, (بینښه) مزي_اوږدوالی (id)};
  أرجع doc_cache_پیدا_کړه_id_پراختیا (doc, key, پیل, پای_ټکی);
}

بنية SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#د_که_پای /* OT_COLOR_SVG_SVG_HH */
