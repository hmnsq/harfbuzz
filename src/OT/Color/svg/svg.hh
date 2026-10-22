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

#నిర్వచితం_కాకపోతే OT_COLOR_SVG_SVG_HH
#నిర్వచించు OT_COLOR_SVG_SVG_HH

#అన్తర్భావ "../../../hb-open-type.hh"
#అన్తర్భావ "../../../hb-blob.hh"
#అన్తర్భావ "../../../hb-limits.hh"
#అన్తర్భావ "../../../hb-map.hh"
#అన్తర్భావ "../../../hb-paint.hh"
#అన్తర్భావ "../../../hb-zlib.hh"
#అన్తర్భావ <ctype.h>
#అన్తర్భావ <string.h>

/*
 * SVG -- SVG (Scalable Vector Graphics)
 * https://docs.microsoft.com/en-us/typography/opentype/spec/svg
 */

#నిర్వచించు HB_OT_ట్యాగ్_SVG HB_ట్యాగ్('S','V','G',' ')


నామాకాశ OT {

స్థిర అంతఃవరుస hb_blob_t *
hb_ot_svg_reference_normalized_blob (hb_blob_t *image,
                                     స్థిరరాశి అక్షరం **svg,
                                     గుర్తులేని *len)
{
  hb_blob_t *blob = hb_blob_reference (image);
  గుర్తులేని దత్తాంశం_len = 0;
  స్థిరరాశి అక్షరం *దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &దత్తాంశం_len);

  ఒకవేళ (!దత్తాంశం || !దత్తాంశం_len)
    వెళ్ళు fail;

  ఒకవేళ (hb_blob_is_gzip (దత్తాంశం, దత్తాంశం_len))
  {
    uint32_t expected_కొలత = 0;
    ఒకవేళ (hb_gzip_get_uncompressed_కొలత (దత్తాంశం, దత్తాంశం_len, &expected_కొలత) &&
        unlikely ((size_t) expected_కొలత > (size_t) HB_SVG_MAX_DOCUMENT_కొలత))
      వెళ్ళు fail;

    hb_blob_t *uncompressed = hb_blob_decompress_gzip (blob,
                                                       HB_SVG_MAX_DOCUMENT_కొలత);
    ఒకవేళ (!uncompressed)
      వెళ్ళు fail;

    hb_blob_destroy (blob);
    blob = uncompressed;
    దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &దత్తాంశం_len);
    ఒకవేళ (!దత్తాంశం || !దత్తాంశం_len)
      వెళ్ళు fail;
  }

  ఒకవేళ (unlikely ((size_t) దత్తాంశం_len > (size_t) HB_SVG_MAX_DOCUMENT_కొలత))
    వెళ్ళు fail;

  ఒకవేళ (svg) *svg = దత్తాంశం;
  ఒకవేళ (len) *len = దత్తాంశం_len;
  తిరిగి blob;

fail:
  hb_blob_destroy (blob);
  ఒకవేళ (svg) *svg = శూన్యసూచిక;
  ఒకవేళ (len) *len = 0;
  తిరిగి శూన్యసూచిక;
}

నిర్మాణం SVGDocument_సూచీ_నమోదు
{
  పూర్ణాంకం cmp (hb_codepoint_t g) స్థిరరాశి
  { తిరిగి g < ప్రారంభం_Glyph_ID ? -1 : g > చివర_Glyph_ID ? 1 : 0; }

  hb_codepoint_t get_ప్రారంభం_glyph () స్థిరరాశి
  { తిరిగి ప్రారంభం_Glyph_ID; }

  hb_codepoint_t get_చివర_glyph () స్థిరరాశి
  { తిరిగి చివర_Glyph_ID; }

  hb_blob_t *reference_blob (hb_blob_t *svg_blob, గుర్తులేని పూర్ణాంకం సూచీ_స్థానభ్రంశం) స్థిరరాశి
  {
    తిరిగి hb_blob_create_sub_blob (svg_blob,
				    సూచీ_స్థానభ్రంశం + (గుర్తులేని పూర్ణాంకం) svgDoc,
				    svg_Doc_నిడివి);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  svgDoc.sanitize (c, పునాది, svg_Doc_నిడివి));
  }

  రక్షిత:
  HBUINT16	ప్రారంభం_Glyph_ID;	/* The first glyph ID in the range described by
				 * this index entry. */
  HBUINT16	చివర_Glyph_ID;	/* The last glyph ID in the range described by
				 * this index entry. Must be >= startGlyphID. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<HBUINT8>>
		svgDoc;		/* Offset from the beginning of the SVG Document Index
				 * to an SVG document. Must be non-zero. */
  HBUINT32	svg_Doc_నిడివి;	/* Length of the SVG document.
				 * Must be non-zero. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

నిర్మాణం SVG
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_SVG;

  నిర్మాణం svg_id_పరిధి_t
  {
    స్థిరరాశి అక్షరం *p;
    గుర్తులేని len;

    తార్కిక క్రియాకారకం == (స్థిరరాశి svg_id_పరిధి_t &o) స్థిరరాశి
    {
      తిరిగి len == o.len && !memcmp (p, o.p, len);
    }

    uint32_t hash () స్థిరరాశి
    {
      uint32_t h = hb_hash (len);
      కొరకు (గుర్తులేని i = 0; i < len; i++)
        h = h * 33u + (గుర్తులేని అక్షరం) p[i];
      తిరిగి h;
    }
  };

  నిర్మాణం svg_defs_నమోదు_t
  {
    svg_id_పరిధి_t id;
    గుర్తులేని ప్రారంభం;
    గుర్తులేని చివర;
  };

  నిర్మాణం svg_doc_cache_t
  {
    hb_blob_t *blob = శూన్యసూచిక;
    స్థిరరాశి అక్షరం *svg = శూన్యసూచిక;
    గుర్తులేని len = 0;
    hb_సదిశ_t<svg_defs_నమోదు_t> defs_entries;
    hb_codepoint_t ప్రారంభం_glyph = HB_CODEPOINT_INVALID;
    hb_codepoint_t చివర_glyph = HB_CODEPOINT_INVALID;
    hb_సదిశ_t<hb_pair_t<uint32_t, uint32_t>> glyph_spans;
    hb_hashmap_t<svg_id_పరిధి_t, hb_pair_t<uint32_t, uint32_t>> id_spans;
  };

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి svgDocEntries; }

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face);
    ~accelerator_t ();

    hb_blob_t *reference_blob_for_glyph (hb_codepoint_t glyph_id) స్థిరరాశి
    {
      తిరిగి పట్టిక->get_glyph_నమోదు (glyph_id).reference_blob (పట్టిక.get_blob (),
							       పట్టిక->svgDocEntries);
    }

    గుర్తులేని get_document_లెక్క () స్థిరరాశి
    { తిరిగి పట్టిక->get_document_లెక్క (); }

    తార్కిక get_glyph_document_సూచీ (hb_codepoint_t glyph_id, గుర్తులేని *సూచీ_2) స్థిరరాశి
    { తిరిగి పట్టిక->get_glyph_document_సూచీ (glyph_id, సూచీ_2); }

    తార్కిక get_document_glyph_range (గుర్తులేని సూచీ_2,
                                   hb_codepoint_t *ప్రారంభం_glyph,
                                   hb_codepoint_t *చివర_glyph) స్థిరరాశి
    { తిరిగి పట్టిక->get_document_glyph_range (సూచీ_2, ప్రారంభం_glyph, చివర_glyph); }

    తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి పట్టిక->has_దత్తాంశం (); }

    స్థిరరాశి svg_doc_cache_t *
    get_or_create_doc_cache (hb_blob_t *image,
                             స్థిరరాశి అక్షరం *svg,
                             గుర్తులేని len,
                             గుర్తులేని doc_సూచీ,
                             hb_codepoint_t ప్రారంభం_glyph,
                             hb_codepoint_t చివర_glyph) స్థిరరాశి;

    స్థిరరాశి అక్షరం *
    doc_cache_get_svg (స్థిరరాశి svg_doc_cache_t *doc,
                       గుర్తులేని *len) స్థిరరాశి;

    స్థిరరాశి hb_సదిశ_t<svg_defs_నమోదు_t> *
    doc_cache_get_defs_entries (స్థిరరాశి svg_doc_cache_t *doc) స్థిరరాశి;

    తార్కిక
    doc_cache_get_glyph_పరిధి (స్థిరరాశి svg_doc_cache_t *doc,
                              hb_codepoint_t glyph,
                              గుర్తులేని *ప్రారంభం,
                              గుర్తులేని *చివర) స్థిరరాశి;

    తార్కిక
    doc_cache_కనుగొను_id_పరిధి (స్థిరరాశి svg_doc_cache_t *doc,
                            svg_id_పరిధి_t id,
                            గుర్తులేని *ప్రారంభం,
                            గుర్తులేని *చివర) స్థిరరాశి;

    తార్కిక
    doc_cache_కనుగొను_id_cstr (స్థిరరాశి svg_doc_cache_t *doc,
                            స్థిరరాశి అక్షరం *id,
                            గుర్తులేని *ప్రారంభం,
                            గుర్తులేని *చివర) స్థిరరాశి;

    తార్కిక paint_glyph (hb_font_t *font HB_వాడని, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, శూన్య *దత్తాంశం) స్థిరరాశి
    {
      ఒకవేళ (!has_దత్తాంశం ())
        తిరిగి అబద్ధం;

      hb_blob_t *blob = reference_blob_for_glyph (glyph);

      ఒకవేళ (blob == hb_blob_get_ఖాళీలు ())
        తిరిగి అబద్ధం;

      తార్కిక ret = funcs->image (దత్తాంశం,
			       blob,
			       0, 0,
			       HB_PAINT_IMAGE_FORMAT_SVG,
			       0.f,
			       శూన్యసూచిక);

      hb_blob_destroy (blob);

      తిరిగి ret;
    }

    అంతరంగ:
    svg_doc_cache_t *
    make_doc_cache (hb_blob_t *image,
                    స్థిరరాశి అక్షరం *svg,
                    గుర్తులేని len,
                    hb_codepoint_t ప్రారంభం_glyph,
                    hb_codepoint_t చివర_glyph) స్థిరరాశి;

    స్థిర శూన్య destroy_doc_cache (svg_doc_cache_t *doc);

    hb_blob_ptr_t<SVG> పట్టిక;
    మార్చదగిన hb_సదిశ_t<hb_atomic_t<svg_doc_cache_t *>> doc_caches;
    బహిరంగ:
    DEFINE_కొలత_STATIC (పరిమాణం (hb_blob_ptr_t<SVG>) +
                        పరిమాణం (hb_సదిశ_t<hb_atomic_t<svg_doc_cache_t *>>));
  };

  స్థిరరాశి SVGDocument_సూచీ_నమోదు &get_glyph_నమోదు (hb_codepoint_t glyph_id) స్థిరరాశి
  { తిరిగి (ఇది+svgDocEntries).bsearch (glyph_id); }

  గుర్తులేని get_document_లెక్క () స్థిరరాశి
  {
    ఒకవేళ (!has_దత్తాంశం ())
      తిరిగి 0;
    తిరిగి (ఇది + svgDocEntries).len;
  }

  తార్కిక get_glyph_document_సూచీ (hb_codepoint_t glyph_id, గుర్తులేని *సూచీ_2) స్థిరరాశి
  {
    ఒకవేళ (!has_దత్తాంశం ())
      తిరిగి అబద్ధం;
    తిరిగి (ఇది + svgDocEntries).bfind (glyph_id, సూచీ_2);
  }

  తార్కిక get_document_glyph_range (గుర్తులేని సూచీ_2,
                                 hb_codepoint_t *ప్రారంభం_glyph,
                                 hb_codepoint_t *చివర_glyph) స్థిరరాశి
  {
    ఒకవేళ (!has_దత్తాంశం ())
      తిరిగి అబద్ధం;

    స్థిరరాశి స్వయంచాలక &entries = ఇది + svgDocEntries;
    ఒకవేళ (సూచీ_2 >= entries.len)
      తిరిగి అబద్ధం;

    స్థిరరాశి స్వయంచాలక &నమోదు_2 = entries.arrayZ[సూచీ_2];
    ఒకవేళ (ప్రారంభం_glyph) *ప్రారంభం_glyph = నమోదు_2.get_ప్రారంభం_glyph ();
    ఒకవేళ (చివర_glyph) *చివర_glyph = నమోదు_2.get_చివర_glyph ();
    తిరిగి నిజం;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  (ఇది+svgDocEntries).sanitize_shallow (c)));
  }

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* Table version (starting at 0). */
  Offset32_గమ్యం<Sorted_Array16_యొక్క<SVGDocument_సూచీ_నమోదు>>
		svgDocEntries;	/* Offset (relative to the start of the SVG table) to the
				 * SVG Documents Index. Must be non-zero. */
				/* Array of SVG Document Index Entries. */
  HBUINT32	reserved;	/* Set to 0. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (10);
};

నామాకాశ _hb_svg_cache_impl {

నిర్మాణం glyph_నమోదు_t
{
  hb_codepoint_t glyph;
  uint32_t ప్రారంభం;
  uint32_t చివర;
};

నిర్మాణం id_నమోదు_t
{
  SVG::svg_id_పరిధి_t id;
  uint32_t ప్రారంభం;
  uint32_t చివర;
};

నిర్మాణం open_elem_t
{
  గుర్తులేని ప్రారంభం;
  SVG::svg_id_పరిధి_t id;
  తార్కిక లోపల_defs_content;
  తార్కిక is_defs;
};

స్థిర స్థిరరాశి గుర్తులేని MAX_లోతు = 128;

స్థిర అంతఃవరుస పూర్ణాంకం
కనుగొను_substr (స్థిరరాశి అక్షరం *s,
             గుర్తులేని n,
             గుర్తులేని from,
             స్థిరరాశి అక్షరం *needle,
             గుర్తులేని needle_len)
{
  ఒకవేళ (!needle_len || from >= n || needle_len > n)
    తిరిగి -1;
  కొరకు (గుర్తులేని i = from; i + needle_len <= n; i++)
    ఒకవేళ (s[i] == needle[0] && !memcmp (s + i, needle, needle_len))
      తిరిగి (పూర్ణాంకం) i;
  తిరిగి -1;
}

స్థిర అంతఃవరుస తార్కిక
parse_id_లోపల_ప్రారంభం_ట్యాగ్ (స్థిరరాశి అక్షరం *svg,
                       గుర్తులేని ట్యాగ్_ప్రారంభం,
                       గుర్తులేని ట్యాగ్_చివర,
                       SVG::svg_id_పరిధి_t *id)
{
  గుర్తులేని p = ట్యాగ్_ప్రారంభం;
  అయితే (p + 4 <= ట్యాగ్_చివర)
  {
    ఒకవేళ (!memcmp (svg + p, "id=\"", 4))
    {
      గుర్తులేని b = p + 4;
      గుర్తులేని e = b;
      అయితే (e < ట్యాగ్_చివర && svg[e] != '"') e++;
      ఒకవేళ (e <= ట్యాగ్_చివర && e > b)
      {
        *id = {svg + b, e - b};
        తిరిగి నిజం;
      }
    }
    ఒకవేళ (!memcmp (svg + p, "id='", 4))
    {
      గుర్తులేని b = p + 4;
      గుర్తులేని e = b;
      అయితే (e < ట్యాగ్_చివర && svg[e] != '\'') e++;
      ఒకవేళ (e <= ట్యాగ్_చివర && e > b)
      {
        *id = {svg + b, e - b};
        తిరిగి నిజం;
      }
    }
    p++;
  }
  తిరిగి అబద్ధం;
}

స్థిర అంతఃవరుస తార్కిక
parse_glyph_id_పరిధి (స్థిరరాశి SVG::svg_id_పరిధి_t &id,
                     hb_codepoint_t *glyph)
{
  ఒకవేళ (id.len <= 5 || memcmp (id.p, "glyph", 5))
    తిరిగి అబద్ధం;

  hb_codepoint_t gid = 0;
  కొరకు (గుర్తులేని i = 5; i < id.len; i++)
  {
    గుర్తులేని అక్షరం c = (గుర్తులేని అక్షరం) id.p[i];
    ఒకవేళ (c < '0' || c > '9')
      తిరిగి అబద్ధం;
    hb_codepoint_t digit = (hb_codepoint_t) (c - '0');
    ఒకవేళ (unlikely (gid > HB_CODEPOINT_INVALID / 10 ||
                  (gid == HB_CODEPOINT_INVALID / 10 &&
                   digit > HB_CODEPOINT_INVALID % 10)))
      తిరిగి అబద్ధం;
    gid = (hb_codepoint_t) (gid * 10 + digit);
  }

  *glyph = gid;
  తిరిగి నిజం;
}

స్థిర అంతఃవరుస తార్కిక
parse_cache_entries_రేఖీయ (స్థిరరాశి అక్షరం *svg,
                            గుర్తులేని len,
                            hb_సదిశ_t<SVG::svg_defs_నమోదు_t> *defs_entries,
                            hb_సదిశ_t<glyph_నమోదు_t> *glyph_spans,
                            hb_సదిశ_t<id_నమోదు_t> *id_entries)
{
  open_elem_t దొంతర[MAX_లోతు] = {};
  గుర్తులేని లోతు = 0;
  ఒకవేళ (unlikely (!defs_entries->alloc (256) ||
                !glyph_spans->alloc (256) ||
                !id_entries->alloc (256)))
    తిరిగి అబద్ధం;

  గుర్తులేని defs_లోతు = 0;
  గుర్తులేని i = 0;
  అయితే (i < len)
  {
    ఒకవేళ (svg[i] != '<')
    {
      i++;
      కొనసాగు;
    }

    ఒకవేళ (i + 4 <= len && !memcmp (svg + i, "<!--", 4))
    {
      పూర్ణాంకం cend = కనుగొను_substr (svg, len, i + 4, "-->", 3);
      ఒకవేళ (cend < 0) తిరిగి అబద్ధం;
      i = (గుర్తులేని) cend + 3;
      కొనసాగు;
    }
    ఒకవేళ (i + 9 <= len && !memcmp (svg + i, "<![CDATA[", 9))
    {
      పూర్ణాంకం cend = కనుగొను_substr (svg, len, i + 9, "]]>", 3);
      ఒకవేళ (cend < 0) తిరిగి అబద్ధం;
      i = (గుర్తులేని) cend + 3;
      కొనసాగు;
    }

    తార్కిక closing = (i + 1 < len && svg[i + 1] == '/');
    తార్కిక special = (i + 1 < len && (svg[i + 1] == '!' || svg[i + 1] == '?'));

    గుర్తులేని gt = i + 1;
    అక్షరం quote = 0;
    అయితే (gt < len)
    {
      అక్షరం c = svg[gt];
      ఒకవేళ (quote)
      {
        ఒకవేళ (c == quote) quote = 0;
      }
      లేకపోతే
      {
        ఒకవేళ (c == '"' || c == '\'')
          quote = c;
        లేకపోతే ఒకవేళ (c == '>')
          ఆపు;
      }
      gt++;
    }
    ఒకవేళ (gt >= len)
      తిరిగి అబద్ధం;

    ఒకవేళ (special)
    {
      i = gt + 1;
      కొనసాగు;
    }

    గుర్తులేని p = i + (closing ? 2 : 1);
    అయితే (p < gt && ఖాళీనా ((గుర్తులేని అక్షరం) svg[p])) p++;
    స్థిరరాశి అక్షరం *name = svg + p;
    గుర్తులేని name_len = 0;
    అయితే (p + name_len < gt)
    {
      గుర్తులేని అక్షరం c = (గుర్తులేని అక్షరం) name[name_len];
      ఒకవేళ (!(ISALNUM (c) || c == '_' || c == '-' || c == ':'))
        ఆపు;
      name_len++;
    }
    తార్కిక is_defs = (name_len == 4 && !memcmp (name, "defs", 4));

    ఒకవేళ (closing)
    {
      ఒకవేళ (!లోతు)
      {
        i = gt + 1;
        కొనసాగు;
      }

      open_elem_t e = దొంతర[--లోతు];
      గుర్తులేని చివర = gt + 1;

      ఒకవేళ (e.id.len)
      {
        ఒకవేళ (unlikely (!id_entries->నెట్టు_or_fail (id_నమోదు_t {e.id, (uint32_t) e.ప్రారంభం, (uint32_t) చివర})))
          తిరిగి అబద్ధం;

        ఒకవేళ (e.లోపల_defs_content)
        {
          ఒకవేళ (unlikely (!defs_entries->నెట్టు_or_fail ()))
            తిరిగి అబద్ధం;
          స్వయంచాలక &స్థానం = defs_entries->తోక ();
          స్థానం.id = e.id;
          స్థానం.ప్రారంభం = e.ప్రారంభం;
          స్థానం.చివర = చివర;
        }

        hb_codepoint_t gid;
        ఒకవేళ (parse_glyph_id_పరిధి (e.id, &gid))
        {
          ఒకవేళ (unlikely (!glyph_spans->నెట్టు_or_fail (glyph_నమోదు_t {gid, (uint32_t) e.ప్రారంభం, (uint32_t) చివర})))
            తిరిగి అబద్ధం;
        }
      }

      ఒకవేళ (e.is_defs && defs_లోతు)
        defs_లోతు--;

      i = చివర;
      కొనసాగు;
    }

    SVG::svg_id_పరిధి_t id = {};
    parse_id_లోపల_ప్రారంభం_ట్యాగ్ (svg, i, gt, &id);

    గుర్తులేని r = gt;
    అయితే (r > i && ఖాళీనా ((గుర్తులేని అక్షరం) svg[r - 1])) r--;
    తార్కిక self_closing = (r > i && svg[r - 1] == '/');

    open_elem_t e = {};
    e.ప్రారంభం = i;
    e.id = id;
    e.లోపల_defs_content = defs_లోతు > 0;
    e.is_defs = is_defs;

    ఒకవేళ (self_closing)
    {
      గుర్తులేని చివర = gt + 1;
      ఒకవేళ (e.id.len)
      {
        ఒకవేళ (unlikely (!id_entries->నెట్టు_or_fail (id_నమోదు_t {e.id, (uint32_t) e.ప్రారంభం, (uint32_t) చివర})))
          తిరిగి అబద్ధం;

        ఒకవేళ (e.లోపల_defs_content)
        {
          ఒకవేళ (unlikely (!defs_entries->నెట్టు_or_fail ()))
            తిరిగి అబద్ధం;
          స్వయంచాలక &స్థానం = defs_entries->తోక ();
          స్థానం.id = e.id;
          స్థానం.ప్రారంభం = e.ప్రారంభం;
          స్థానం.చివర = చివర;
        }

        hb_codepoint_t gid;
        ఒకవేళ (parse_glyph_id_పరిధి (e.id, &gid))
        {
          ఒకవేళ (unlikely (!glyph_spans->నెట్టు_or_fail (glyph_నమోదు_t {gid, (uint32_t) e.ప్రారంభం, (uint32_t) చివర})))
            తిరిగి అబద్ధం;
        }
      }
    }
    లేకపోతే
    {
      ఒకవేళ (unlikely (లోతు >= MAX_లోతు))
        తిరిగి అబద్ధం;
      దొంతర[లోతు++] = e;
      ఒకవేళ (is_defs)
        defs_లోతు++;
    }

    i = gt + 1;
  }

  తిరిగి నిజం;
}

} /* namespace _hb_svg_cache_impl */

అంతఃవరుస
SVG::accelerator_t::accelerator_t (hb_face_t *face)
{
  పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<SVG> (face);
  doc_caches.init ();
  గుర్తులేని doc_లెక్క = పట్టిక->get_document_లెక్క ();
  ఒకవేళ (doc_లెక్క && unlikely (!doc_caches.resize (doc_లెక్క)))
    doc_caches.clear ();
  కొరకు (గుర్తులేని i = 0; i < doc_caches.నిడివి; i++)
    doc_caches.arrayZ[i].సమితి_relaxed (శూన్యసూచిక);
}

అంతఃవరుస
SVG::accelerator_t::~accelerator_t ()
{
  కొరకు (గుర్తులేని i = 0; i < doc_caches.నిడివి; i++)
    destroy_doc_cache (doc_caches.arrayZ[i].get_relaxed ());
  doc_caches.fini ();
  పట్టిక.destroy ();
}

అంతఃవరుస శూన్య
SVG::accelerator_t::destroy_doc_cache (svg_doc_cache_t *doc)
{
  ఒకవేళ (!doc)
    తిరిగి;
  doc->glyph_spans.fini ();
  doc->defs_entries.fini ();
  doc->id_spans.fini ();
  hb_blob_destroy (doc->blob);
  hb_free (doc);
}

అంతఃవరుస SVG::svg_doc_cache_t *
SVG::accelerator_t::make_doc_cache (hb_blob_t *image,
                                    స్థిరరాశి అక్షరం *svg,
                                    గుర్తులేని len,
                                    hb_codepoint_t ప్రారంభం_glyph,
                                    hb_codepoint_t చివర_glyph) స్థిరరాశి
{
  స్థిర స్థిరరాశి uint32_t INVALID_పరిధి = 0xFFFFFFFFu;

  స్వయంచాలక *doc = (svg_doc_cache_t *) hb_malloc (పరిమాణం (svg_doc_cache_t));
  ఒకవేళ (!doc)
    తిరిగి శూన్యసూచిక;

  doc->blob = శూన్యసూచిక;
  doc->svg = శూన్యసూచిక;
  doc->len = 0;
  doc->defs_entries.init ();
  doc->ప్రారంభం_glyph = HB_CODEPOINT_INVALID;
  doc->చివర_glyph = HB_CODEPOINT_INVALID;
  doc->glyph_spans.init ();
  doc->id_spans.init ();

  doc->blob = hb_blob_reference (image);
  doc->svg = svg;
  doc->len = len;
  doc->ప్రారంభం_glyph = ప్రారంభం_glyph;
  doc->చివర_glyph = చివర_glyph;

  ఒకవేళ (unlikely (ప్రారంభం_glyph == HB_CODEPOINT_INVALID || చివర_glyph < ప్రారంభం_glyph))
  {
    destroy_doc_cache (doc);
    తిరిగి శూన్యసూచిక;
  }

  గుర్తులేని glyph_లెక్క = చివర_glyph - ప్రారంభం_glyph + 1;
  ఒకవేళ (!doc->glyph_spans.resize ((పూర్ణాంకం) glyph_లెక్క))
  {
    destroy_doc_cache (doc);
    తిరిగి శూన్యసూచిక;
  }
  కొరకు (గుర్తులేని i = 0; i < glyph_లెక్క; i++)
    doc->glyph_spans.arrayZ[i] = hb_pair_t<uint32_t, uint32_t> (INVALID_పరిధి, INVALID_పరిధి);

  hb_సదిశ_t<_hb_svg_cache_impl::glyph_నమోదు_t> glyph_spans;
  glyph_spans.init ();
  hb_సదిశ_t<_hb_svg_cache_impl::id_నమోదు_t> id_entries;
  id_entries.init ();
  ఒకవేళ (!_hb_svg_cache_impl::parse_cache_entries_రేఖీయ (svg, len,
                                                       &doc->defs_entries,
                                                       &glyph_spans,
                                                       &id_entries))
  {
    id_entries.fini ();
    glyph_spans.fini ();
    destroy_doc_cache (doc);
    తిరిగి శూన్యసూచిక;
  }

  కొరకు (గుర్తులేని i = 0; i < glyph_spans.నిడివి; i++)
  {
    స్థిరరాశి స్వయంచాలక &పరిధి = glyph_spans.arrayZ[i];
    ఒకవేళ (unlikely (పరిధి.glyph < ప్రారంభం_glyph || పరిధి.glyph > చివర_glyph))
      కొనసాగు;
    doc->glyph_spans.arrayZ[పరిధి.glyph - ప్రారంభం_glyph] = hb_pair_t<uint32_t, uint32_t> (పరిధి.ప్రారంభం, పరిధి.చివర);
  }

  కొరకు (గుర్తులేని i = 0; i < id_entries.నిడివి; i++)
  {
    స్థిరరాశి స్వయంచాలక &e = id_entries.arrayZ[i];
    hb_pair_t<uint32_t, uint32_t> *బహిర్గతం = శూన్యసూచిక;
    ఒకవేళ (doc->id_spans.has (e.id, &బహిర్గతం))
      కొనసాగు;
    ఒకవేళ (unlikely (!doc->id_spans.సమితి (e.id, hb_pair_t<uint32_t, uint32_t> (e.ప్రారంభం, e.చివర))))
    {
      id_entries.fini ();
      glyph_spans.fini ();
      destroy_doc_cache (doc);
      తిరిగి శూన్యసూచిక;
    }
  }

  id_entries.fini ();
  glyph_spans.fini ();
  తిరిగి doc;
}

అంతఃవరుస స్థిరరాశి SVG::svg_doc_cache_t *
SVG::accelerator_t::get_or_create_doc_cache (hb_blob_t *image,
                                              స్థిరరాశి అక్షరం *svg,
                                              గుర్తులేని len,
                                              గుర్తులేని doc_సూచీ,
                                              hb_codepoint_t ప్రారంభం_glyph,
                                              hb_codepoint_t చివర_glyph) స్థిరరాశి
{
  ఒకవేళ (doc_సూచీ >= doc_caches.నిడివి)
    తిరిగి శూన్యసూచిక;

  స్వయంచాలక &స్థానం = doc_caches.arrayZ[doc_సూచీ];
  స్వయంచాలక *doc = స్థానం.get_acquire ();
  ఒకవేళ (doc)
    తిరిగి doc;

  స్వయంచాలక *fresh = make_doc_cache (image, svg, len, ప్రారంభం_glyph, చివర_glyph);
  ఒకవేళ (!fresh)
    తిరిగి శూన్యసూచిక;

  స్వయంచాలక *expected = (svg_doc_cache_t *) శూన్యసూచిక;
  ఒకవేళ (స్థానం.cmpexch (expected, fresh))
    తిరిగి fresh;

  destroy_doc_cache (fresh);
  తిరిగి expected;
}

అంతఃవరుస స్థిరరాశి అక్షరం *
SVG::accelerator_t::doc_cache_get_svg (స్థిరరాశి svg_doc_cache_t *doc,
                                       గుర్తులేని *len) స్థిరరాశి
{
  ఒకవేళ (!doc)
  {
    ఒకవేళ (len) *len = 0;
    తిరిగి శూన్యసూచిక;
  }
  ఒకవేళ (len) *len = doc->len;
  తిరిగి doc->svg;
}

అంతఃవరుస స్థిరరాశి hb_సదిశ_t<SVG::svg_defs_నమోదు_t> *
SVG::accelerator_t::doc_cache_get_defs_entries (స్థిరరాశి svg_doc_cache_t *doc) స్థిరరాశి
{
  తిరిగి doc ? &doc->defs_entries : శూన్యసూచిక;
}

అంతఃవరుస తార్కిక
SVG::accelerator_t::doc_cache_get_glyph_పరిధి (స్థిరరాశి svg_doc_cache_t *doc,
                                              hb_codepoint_t glyph,
                                              గుర్తులేని *ప్రారంభం,
                                              గుర్తులేని *చివర) స్థిరరాశి
{
  స్థిర స్థిరరాశి uint32_t INVALID_పరిధి = 0xFFFFFFFFu;
  ఒకవేళ (!doc || doc->ప్రారంభం_glyph == HB_CODEPOINT_INVALID ||
      glyph < doc->ప్రారంభం_glyph || glyph > doc->చివర_glyph)
    తిరిగి అబద్ధం;

  స్థిరరాశి స్వయంచాలక &పరిధి = doc->glyph_spans.arrayZ[glyph - doc->ప్రారంభం_glyph];
  ఒకవేళ (పరిధి.మొదటి == INVALID_పరిధి)
    తిరిగి అబద్ధం;
  ఒకవేళ (unlikely (పరిధి.మొదటి > పరిధి.second || పరిధి.second > doc->len))
    తిరిగి అబద్ధం;

  ఒకవేళ (ప్రారంభం) *ప్రారంభం = పరిధి.మొదటి;
  ఒకవేళ (చివర) *చివర = పరిధి.second;
  తిరిగి నిజం;
}

అంతఃవరుస తార్కిక
SVG::accelerator_t::doc_cache_కనుగొను_id_పరిధి (స్థిరరాశి svg_doc_cache_t *doc,
                                            svg_id_పరిధి_t id,
                                            గుర్తులేని *ప్రారంభం,
                                            గుర్తులేని *చివర) స్థిరరాశి
{
  ఒకవేళ (!doc || !id.p || !id.len)
    తిరిగి అబద్ధం;
  hb_pair_t<uint32_t, uint32_t> *పరిధి = శూన్యసూచిక;
  ఒకవేళ (!doc->id_spans.has (id, &పరిధి))
    తిరిగి అబద్ధం;
  ఒకవేళ (unlikely (పరిధి->మొదటి > పరిధి->second || పరిధి->second > doc->len))
    తిరిగి అబద్ధం;
  ఒకవేళ (ప్రారంభం) *ప్రారంభం = పరిధి->మొదటి;
  ఒకవేళ (చివర) *చివర = పరిధి->second;
  తిరిగి నిజం;
}

అంతఃవరుస తార్కిక
SVG::accelerator_t::doc_cache_కనుగొను_id_cstr (స్థిరరాశి svg_doc_cache_t *doc,
                                            స్థిరరాశి అక్షరం *id,
                                            గుర్తులేని *ప్రారంభం,
                                            గుర్తులేని *చివర) స్థిరరాశి
{
  ఒకవేళ (!id) తిరిగి అబద్ధం;
  svg_id_పరిధి_t key = {id, (గుర్తులేని) అక్షరశ్రేణి_పొడవు (id)};
  తిరిగి doc_cache_కనుగొను_id_పరిధి (doc, key, ప్రారంభం, చివర);
}

నిర్మాణం SVG_accelerator_t : SVG::accelerator_t {
  SVG_accelerator_t (hb_face_t *face) : SVG::accelerator_t (face) {}
};

} /* namespace OT */


#అయితే_ముగింపు /* OT_COLOR_SVG_SVG_HH */
