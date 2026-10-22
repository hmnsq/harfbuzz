/*
 * Copyright © 2016  Google, Inc.
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
 *
 * Google Author(s): Seigo Nonaka, Calder Kitagawa
 */

#నిర్వచితం_కాకపోతే OT_COLOR_CBDT_CBDT_HH
#నిర్వచించు OT_COLOR_CBDT_CBDT_HH

#అన్తర్భావ "../../../hb-open-type.hh"
#అన్తర్భావ "../../../hb-paint.hh"

/*
 * CBLC -- Color Bitmap Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cblc
 * https://docs.microsoft.com/en-us/typography/opentype/spec/eblc
 * CBDT -- Color Bitmap Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cbdt
 * https://docs.microsoft.com/en-us/typography/opentype/spec/ebdt
 */
#నిర్వచించు HB_OT_ట్యాగ్_CBLC HB_ట్యాగ్('C','B','L','C')
#నిర్వచించు HB_OT_ట్యాగ్_CBDT HB_ట్యాగ్('C','B','D','T')


నామాకాశ OT {

/* Tracks image data blocks emitted into the subset CBDT table so that
 * identical subtables (for example the same bitmaps used by several strikes)
 * can share a single copy. */
నిర్మాణం cbdt_dedup_context_t
{
  నిర్మాణం నమోదు_t
  {
    గుర్తులేని పూర్ణాంకం పునాది;		/* Offset of the data in cbdt_prime. */
    గుర్తులేని పూర్ణాంకం lstart;	/* Start index in lengths. */
    గుర్తులేని పూర్ణాంకం lcount;	/* Number of length entries. */
  };

  స్థిర uint64_t hash (గుర్తులేని పూర్ణాంకం image_format,
			స్థిరరాశి hb_సదిశ_t<అక్షరం> &దత్తాంశం,
			స్థిరరాశి hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> &lengths)
  {
    uint32_t h = hb_hash (hb_బైట్లు_t (దత్తాంశం.arrayZ, దత్తాంశం.నిడివి));
    h = h * 31 + hb_hash (hb_బైట్లు_t ((స్థిరరాశి అక్షరం *) lengths.arrayZ,
				      lengths.నిడివి * పరిమాణం (గుర్తులేని పూర్ణాంకం)));
    తిరిగి ((uint64_t) image_format << 32) | h;
  }

  తార్కిక find (uint64_t key,
	     స్థిరరాశి hb_సదిశ_t<అక్షరం> &దత్తాంశం,
	     స్థిరరాశి hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> &lengths,
	     స్థిరరాశి hb_సదిశ_t<అక్షరం> &cbdt_ప్రధానసంఖ్య,
	     గుర్తులేని పూర్ణాంకం *పునాది /*బహిర్గతం*/) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం *idx_p = శూన్యసూచిక;
    ఒకవేళ (!map.has (key, &idx_p)) తిరిగి అబద్ధం;
    స్థిరరాశి నమోదు_t &e = entries[*idx_p];
    /* The data and the per-glyph lengths must both match, otherwise the
     * glyph extents implied by the offset array would differ. */
    ఒకవేళ (e.lcount != lengths.నిడివి) తిరిగి అబద్ధం;
    ఒకవేళ (unlikely (e.పునాది + దత్తాంశం.నిడివి > (గుర్తులేని పూర్ణాంకం) cbdt_ప్రధానసంఖ్య.నిడివి)) తిరిగి అబద్ధం;
    ఒకవేళ (hb_memcmp (cbdt_ప్రధానసంఖ్య.arrayZ + e.పునాది, దత్తాంశం.arrayZ, దత్తాంశం.నిడివి)) తిరిగి అబద్ధం;
    ఒకవేళ (hb_memcmp (ఇది->lengths.arrayZ + e.lstart,
		   lengths.arrayZ, lengths.నిడివి * పరిమాణం (గుర్తులేని పూర్ణాంకం)))
      తిరిగి అబద్ధం;
    *పునాది = e.పునాది;
    తిరిగి నిజం;
  }

  తార్కిక కూడు (uint64_t key,
	    గుర్తులేని పూర్ణాంకం పునాది,
	    స్థిరరాశి hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> &lengths)
  {
    ఒకవేళ (unlikely (entries.లోపల_error () || ఇది->lengths.లోపల_error () || map.లోపల_error ()))
      తిరిగి అబద్ధం;
    నమోదు_t e = {పునాది, ఇది->lengths.నిడివి, lengths.నిడివి};
    ఒకవేళ (unlikely (!entries.నెట్టు_or_fail (e))) తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని పూర్ణాంకం నిడివి : lengths)
      ఒకవేళ (unlikely (!ఇది->lengths.నెట్టు_or_fail (నిడివి))) తిరిగి అబద్ధం;
    తిరిగి map.సమితి (key, entries.నిడివి - 1);
  }

  hb_hashmap_t<uint64_t, గుర్తులేని పూర్ణాంకం> map;
  hb_సదిశ_t<నమోదు_t> entries;
  hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> lengths; /* Concatenated per-entry lengths. */
};

నిర్మాణం cblc_bitmap_కొలత_subset_context_t
{
  స్థిరరాశి అక్షరం *cbdt;
  గుర్తులేని పూర్ణాంకం cbdt_నిడివి;
  hb_సదిశ_t<అక్షరం> *cbdt_ప్రధానసంఖ్య;
  గుర్తులేని పూర్ణాంకం కొలత;		/* INOUT
				 *  Input: old size of IndexSubtable
				 *  Output: new size of IndexSubtable
				 */
  గుర్తులేని పూర్ణాంకం అంకె_tables;	/* INOUT
				 *  Input: old number of subtables.
				 *  Output: new number of subtables.
				 */
  hb_codepoint_t ప్రారంభం_glyph;	/*బహిర్గతం*/
  hb_codepoint_t చివర_glyph;	/*బహిర్గతం*/
  cbdt_dedup_context_t *dedup;	/* Shared across strikes; may be null. */
};

స్థిర అంతఃవరుస తార్కిక
copy_దత్తాంశం_గమ్యం_cbdt (hb_సదిశ_t<అక్షరం> *cbdt_ప్రధానసంఖ్య,
		    స్థిరరాశి శూన్య        *దత్తాంశం,
		    గుర్తులేని           నిడివి)
{
  గుర్తులేని పూర్ణాంకం new_len = cbdt_ప్రధానసంఖ్య->నిడివి + నిడివి;
  ఒకవేళ (unlikely (!cbdt_ప్రధానసంఖ్య->alloc (new_len))) తిరిగి అబద్ధం;
  hb_memcpy (cbdt_ప్రధానసంఖ్య->arrayZ + cbdt_ప్రధానసంఖ్య->నిడివి, దత్తాంశం, నిడివి);
  cbdt_ప్రధానసంఖ్య->నిడివి = new_len;
  తిరిగి నిజం;
}

నిర్మాణం SmallGlyphMetrics
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య get_extents (hb_font_t *font, hb_glyph_extents_t *extents, తార్కిక scale) స్థిరరాశి
  {
    extents->x_bearing = bearingX;
    extents->y_bearing = bearingY;
    extents->వెడల్పు = వెడల్పు;
    extents->ఎత్తు = -static_cast<పూర్ణాంకం> (ఎత్తు);

    ఒకవేళ (scale)
      font->scale_glyph_extents (extents);
  }

  HBUINT8	ఎత్తు;
  HBUINT8	వెడల్పు;
  HBINT8	bearingX;
  HBINT8	bearingY;
  HBUINT8	advance;
  బహిరంగ:
  DEFINE_కొలత_STATIC (5);
};

నిర్మాణం BigGlyphMetrics : SmallGlyphMetrics
{
  HBINT8	vertBearingX;
  HBINT8	vertBearingY;
  HBUINT8	vertAdvance;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం SBit_పంక్తి_Metrics
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBINT8	ascender;
  HBINT8	decender;
  HBUINT8	వెడల్పు_Max;
  HBINT8	caret_వాలు_Numerator;
  HBINT8	caret_వాలు_Denominator;
  HBINT8	caret_స్థానభ్రంశం;
  HBINT8	minOriginSB;
  HBINT8	minAdvanceSB;
  HBINT8	maxBeforeBL;
  HBINT8	minAfterBL;
  HBINT8	padding1;
  HBINT8	padding2;
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};


/*
 * Index Subtables.
 */

నిర్మాణం సూచీ_Subtable_శీర్షిక
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT16	సూచీ_Format;
  HBUINT16	imageFormat;
  HBUINT32	image_దత్తాంశం_స్థానభ్రంశం;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

మూస <రకం_పేరు స్థానభ్రంశం_Type>
నిర్మాణం సూచీ_Subtable_Format1_Or3
{
  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం glyph_లెక్క) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  స్థానభ్రంశం_Array_Z.sanitize (c, glyph_లెక్క + 1));
  }

  తార్కిక get_image_దత్తాంశం (గుర్తులేని పూర్ణాంకం idx,
		       గుర్తులేని పూర్ణాంకం *స్థానభ్రంశం,
		       గుర్తులేని పూర్ణాంకం *నిడివి) స్థిరరాశి
  {
    ఒకవేళ (unlikely (స్థానభ్రంశం_Array_Z[idx + 1] <= స్థానభ్రంశం_Array_Z[idx]))
      తిరిగి అబద్ధం;

    *స్థానభ్రంశం = శీర్షిక.image_దత్తాంశం_స్థానభ్రంశం + స్థానభ్రంశం_Array_Z[idx];
    *నిడివి = స్థానభ్రంశం_Array_Z[idx + 1] - స్థానభ్రంశం_Array_Z[idx];
    తిరిగి నిజం;
  }

  తార్కిక కూడు_స్థానభ్రంశం (hb_serialize_context_t *c,
		   గుర్తులేని పూర్ణాంకం స్థానభ్రంశం,
		   గుర్తులేని పూర్ణాంకం *కొలత /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ఇది);
    స్థానభ్రంశం_2<స్థానభ్రంశం_Type> embedded_స్థానభ్రంశం;
    embedded_స్థానభ్రంశం = స్థానభ్రంశం;
    *కొలత += పరిమాణం (స్థానభ్రంశం_Type);
    స్వయంచాలక *o = c->embed (embedded_స్థానభ్రంశం);
    return_trace ((తార్కిక) o);
  }

  సూచీ_Subtable_శీర్షిక	శీర్షిక;
  Unsized_Array_యొక్క<స్థానభ్రంశం_2<స్థానభ్రంశం_Type>>
			స్థానభ్రంశం_Array_Z;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, స్థానభ్రంశం_Array_Z);
};

నిర్మాణం సూచీ_Subtable_Format1 : సూచీ_Subtable_Format1_Or3<HBUINT32> {};
నిర్మాణం సూచీ_Subtable_Format3 : సూచీ_Subtable_Format1_Or3<HBUINT16> {};

నిర్మాణం సూచీ_Subtable
{
  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం glyph_లెక్క) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.శీర్షిక.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.శీర్షిక.సూచీ_Format)
    {
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c, glyph_లెక్క));
    స్థితి 3: hb_barrier (); return_trace (u.format3.sanitize (c, glyph_లెక్క));
    అప్రమేయ:return_trace (నిజం);
    }
  }

  తార్కిక
  ముగింపు_subtable (hb_serialize_context_t *c,
		   గుర్తులేని పూర్ణాంకం చివర_స్థానభ్రంశం,
		   గుర్తులేని పూర్ణాంకం అంకె_glyphs,
		   గుర్తులేని పూర్ణాంకం *కొలత /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ఇది);

    ఎంపిక (u.శీర్షిక.సూచీ_Format)
    {
    స్థితి 1: hb_barrier (); return_trace (u.format1.కూడు_స్థానభ్రంశం (c, చివర_స్థానభ్రంశం, కొలత));
    స్థితి 3: {
      hb_barrier ();
      ఒకవేళ (!u.format3.కూడు_స్థానభ్రంశం (c, చివర_స్థానభ్రంశం, కొలత))
	return_trace (అబద్ధం);
      ఒకవేళ (!(అంకె_glyphs & 0x01))  // Pad to 32-bit alignment if needed.
	return_trace (u.format3.కూడు_స్థానభ్రంశం (c, 0, కొలత));
      return_trace (నిజం);
    }
    // TODO: implement 2, 4, 5.
    స్థితి 2: స్థితి 4:  // No-op.
    స్థితి 5:  // Pad to 32-bit aligned.
    అప్రమేయ: return_trace (అబద్ధం);
    }
  }

  తార్కిక
  కూడు_స్థానభ్రంశం (hb_serialize_context_t *c, గుర్తులేని పూర్ణాంకం local_స్థానభ్రంశం,
	      గుర్తులేని పూర్ణాంకం *కొలత /* OUT (accumulated) */)
  {
    TRACE_SERIALIZE (ఇది);
    ఎంపిక (u.శీర్షిక.సూచీ_Format)
    {
    స్థితి 1: hb_barrier (); return_trace (u.format1.కూడు_స్థానభ్రంశం (c, local_స్థానభ్రంశం, కొలత));
    స్థితి 3: hb_barrier (); return_trace (u.format3.కూడు_స్థానభ్రంశం (c, local_స్థానభ్రంశం, కొలత));
    // TODO: Implement tables 2, 4, 5
    స్థితి 2:  // Should be a no-op.
    స్థితి 4: స్థితి 5:  // Handle sparse cases.
    అప్రమేయ: return_trace (అబద్ధం);
    }
  }

  తార్కిక get_extents (hb_glyph_extents_t *extents HB_వాడని, తార్కిక scale HB_వాడని) స్థిరరాశి
  {
    ఎంపిక (u.శీర్షిక.సూచీ_Format)
    {
    స్థితి 2: స్థితి 5: /* TODO */
    స్థితి 1: స్థితి 3: స్థితి 4: /* Variable-metrics formats do not have metrics here. */
    అప్రమేయ:తిరిగి (అబద్ధం);
    }
  }

  తార్కిక
  get_image_దత్తాంశం (గుర్తులేని పూర్ణాంకం idx, గుర్తులేని పూర్ణాంకం *స్థానభ్రంశం,
		  గుర్తులేని పూర్ణాంకం *నిడివి, గుర్తులేని పూర్ణాంకం *format) స్థిరరాశి
  {
    *format = u.శీర్షిక.imageFormat;
    ఎంపిక (u.శీర్షిక.సూచీ_Format)
    {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_image_దత్తాంశం (idx, స్థానభ్రంశం, నిడివి);
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_image_దత్తాంశం (idx, స్థానభ్రంశం, నిడివి);
    అప్రమేయ: తిరిగి అబద్ధం;
    }
  }

  స్థిరరాశి సూచీ_Subtable_శీర్షిక* get_శీర్షిక () స్థిరరాశి { తిరిగి &u.శీర్షిక; }

  శూన్య populate_శీర్షిక (గుర్తులేని సూచీ_format,
			గుర్తులేని image_format,
			గుర్తులేని పూర్ణాంకం image_దత్తాంశం_స్థానభ్రంశం_2,
			గుర్తులేని పూర్ణాంకం *కొలత)
  {
    u.శీర్షిక.సూచీ_Format = సూచీ_format;
    u.శీర్షిక.imageFormat = image_format;
    u.శీర్షిక.image_దత్తాంశం_స్థానభ్రంశం = image_దత్తాంశం_స్థానభ్రంశం_2;
    ఎంపిక (u.శీర్షిక.సూచీ_Format)
    {
    స్థితి 1: *కొలత += సూచీ_Subtable_Format1::min_కొలత; ఆపు;
    స్థితి 3: *కొలత += సూచీ_Subtable_Format3::min_కొలత; ఆపు;
    }
  }

  రక్షిత:
  సంఘం {
  సూచీ_Subtable_శీర్షిక	శీర్షిక;
  సూచీ_Subtable_Format1	format1;
  సూచీ_Subtable_Format3	format3;
  /* TODO: Format 2, 4, 5. */
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (8, శీర్షిక);
};

నిర్మాణం సూచీ_Subtable_రికార్డు
{
  /* XXX Remove this and fix by not inserting it into vector. */
  సూచీ_Subtable_రికార్డు& క్రియాకారకం = (స్థిరరాశి సూచీ_Subtable_రికార్డు &o)
  {
    మొదటి_Glyph_సూచీ = o.మొదటి_Glyph_సూచీ;
    చివరి_Glyph_సూచీ = o.చివరి_Glyph_సూచీ;
    స్థానభ్రంశం_గమ్యం_Subtable = (గుర్తులేని) o.స్థానభ్రంశం_గమ్యం_Subtable;
    ప్రతిజ్ఞా (స్థానభ్రంశం_గమ్యం_Subtable.is_null ());
    తిరిగి *ఇది;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  మొదటి_Glyph_సూచీ <= చివరి_Glyph_సూచీ &&
		  స్థానభ్రంశం_గమ్యం_Subtable.sanitize (c, పునాది, చివరి_Glyph_సూచీ - మొదటి_Glyph_సూచీ + 1));
  }

  స్థిరరాశి సూచీ_Subtable* get_subtable (స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    తిరిగి &(పునాది+స్థానభ్రంశం_గమ్యం_Subtable);
  }

  తార్కిక కూడు_new_subtable (hb_subset_context_t* c,
			 cblc_bitmap_కొలత_subset_context_t *bitmap_కొలత_context,
			 సూచీ_Subtable_రికార్డు *రికార్డు,
			 స్థిరరాశి hb_సదిశ_t<hb_pair_t<hb_codepoint_t, స్థిరరాశి సూచీ_Subtable_రికార్డు*>> *lookup, /*లోపల*/
			 స్థిరరాశి శూన్య *పునాది,
			 గుర్తులేని పూర్ణాంకం *ప్రారంభం /* INOUT */) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    స్వయంచాలక *subtable = c->serializer->ప్రారంభం_embed<సూచీ_Subtable> ();
    ఒకవేళ (unlikely (!c->serializer->extend_min (subtable))) return_trace (అబద్ధం);

    స్వయంచాలక *old_subtable = get_subtable (పునాది);
    స్వయంచాలక *old_శీర్షిక = old_subtable->get_శీర్షిక ();
    గుర్తులేని పూర్ణాంకం image_format = old_శీర్షిక->imageFormat;

    /* Collect this subtable's image data and the length of each glyph's block
     * (zero for glyphs without data). This allows an earlier identical
     * subtable to share the same data. */
    hb_సదిశ_t<అక్షరం> దత్తాంశం;
    hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> lengths;
    గుర్తులేని పూర్ణాంకం అంకె_glyphs = 0;
    తార్కిక early_exit = అబద్ధం;
    కొరకు (గుర్తులేని పూర్ణాంకం i = *ప్రారంభం; i < lookup->నిడివి; i++)
    {
      hb_codepoint_t new_gid = (*lookup)[i].మొదటి;
      స్థిరరాశి సూచీ_Subtable_రికార్డు *తదుపరి_రికార్డు = (*lookup)[i].second;
      స్థిరరాశి సూచీ_Subtable *తదుపరి_subtable = తదుపరి_రికార్డు->get_subtable (పునాది);
      స్వయంచాలక *తదుపరి_శీర్షిక = తదుపరి_subtable->get_శీర్షిక ();
      ఒకవేళ (తదుపరి_శీర్షిక != old_శీర్షిక)
      {
	*ప్రారంభం = i;
	early_exit = నిజం;
	ఆపు;
      }
      గుర్తులేని పూర్ణాంకం అంకె_missing = రికార్డు->కూడు_glyph_for_subset (new_gid);
      కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < అంకె_missing; j++)
	ఒకవేళ (unlikely (!c->serializer->check_success (lengths.నెట్టు_or_fail (0))))
	  return_trace (అబద్ధం);

      hb_codepoint_t old_gid = 0;
      c->plan->old_gid_for_new_gid (new_gid, &old_gid);
      ఒకవేళ (old_gid < తదుపరి_రికార్డు->మొదటి_Glyph_సూచీ)
	return_trace (అబద్ధం);

      గుర్తులేని పూర్ణాంకం old_idx = (గుర్తులేని పూర్ణాంకం) old_gid - తదుపరి_రికార్డు->మొదటి_Glyph_సూచీ;
      గుర్తులేని పూర్ణాంకం స్థానభ్రంశం, నిడివి, format;
      ఒకవేళ (unlikely (!తదుపరి_subtable->get_image_దత్తాంశం (old_idx, &స్థానభ్రంశం, &నిడివి, &format)))
	return_trace (అబద్ధం);
      ఒకవేళ (unlikely (స్థానభ్రంశం > bitmap_కొలత_context->cbdt_నిడివి ||
		    bitmap_కొలత_context->cbdt_నిడివి - స్థానభ్రంశం < నిడివి))
	return_trace (అబద్ధం);
      ఒకవేళ (unlikely (!c->serializer->check_success (
			copy_దత్తాంశం_గమ్యం_cbdt (&దత్తాంశం,
					    bitmap_కొలత_context->cbdt + స్థానభ్రంశం,
					    నిడివి))))
	return_trace (అబద్ధం);
      ఒకవేళ (unlikely (!c->serializer->check_success (lengths.నెట్టు_or_fail (నిడివి))))
	return_trace (అబద్ధం);
      అంకె_glyphs += 1 + అంకె_missing;
    }
    ఒకవేళ (!early_exit)
      *ప్రారంభం = lookup->నిడివి;

    /* Find or append the data block for this subtable. */
    గుర్తులేని పూర్ణాంకం image_దత్తాంశం_స్థానభ్రంశం_2;
    cbdt_dedup_context_t *dedup = bitmap_కొలత_context->dedup;
    తార్కిక shared = అబద్ధం;
    uint64_t key = 0;
    ఒకవేళ (dedup && దత్తాంశం.నిడివి)
    {
      key = cbdt_dedup_context_t::hash (image_format, దత్తాంశం, lengths);
      shared = dedup->find (key, దత్తాంశం, lengths,
			    *bitmap_కొలత_context->cbdt_ప్రధానసంఖ్య,
			    &image_దత్తాంశం_స్థానభ్రంశం_2);
    }
    ఒకవేళ (!shared)
    {
      image_దత్తాంశం_స్థానభ్రంశం_2 = bitmap_కొలత_context->cbdt_ప్రధానసంఖ్య->నిడివి;
      ఒకవేళ (unlikely (!c->serializer->check_success (
			copy_దత్తాంశం_గమ్యం_cbdt (bitmap_కొలత_context->cbdt_ప్రధానసంఖ్య,
					    దత్తాంశం.arrayZ, దత్తాంశం.నిడివి))))
	return_trace (అబద్ధం);
      ఒకవేళ (dedup && దత్తాంశం.నిడివి)
	/* The bitmap data is already copied; caching is best-effort. */
	(శూన్య) dedup->కూడు (key, image_దత్తాంశం_స్థానభ్రంశం_2, lengths);
    }

    subtable->populate_శీర్షిక (old_శీర్షిక->సూచీ_Format,
			       image_format,
			       image_దత్తాంశం_స్థానభ్రంశం_2,
			       &bitmap_కొలత_context->కొలత);

    /* Glyphs without data use the offset of the following block's start,
     * which yields a zero length span, matching the old behavior. */
    గుర్తులేని పూర్ణాంకం cumulative = 0;
    కొరకు (గుర్తులేని పూర్ణాంకం నిడివి : lengths)
    {
      ఒకవేళ (unlikely (!subtable->కూడు_స్థానభ్రంశం (c->serializer, cumulative,
					   &bitmap_కొలత_context->కొలత)))
	return_trace (అబద్ధం);
      cumulative += నిడివి;
    }
    ఒకవేళ (unlikely (!subtable->ముగింపు_subtable (c->serializer,
					      cumulative,
					      అంకె_glyphs,
					      &bitmap_కొలత_context->కొలత)))
      return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  తార్కిక కూడు_new_రికార్డు (hb_subset_context_t *c,
		       cblc_bitmap_కొలత_subset_context_t *bitmap_కొలత_context,
		       స్థిరరాశి hb_సదిశ_t<hb_pair_t<hb_codepoint_t, స్థిరరాశి సూచీ_Subtable_రికార్డు*>> *lookup, /*లోపల*/
		       స్థిరరాశి శూన్య *పునాది,
		       గుర్తులేని పూర్ణాంకం *ప్రారంభం, /* INOUT */
		       hb_సదిశ_t<సూచీ_Subtable_రికార్డు>* రికార్డులు /* INOUT */) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక snap = c->serializer->snapshot ();
    గుర్తులేని పూర్ణాంకం old_కొలత = bitmap_కొలత_context->కొలత;
    గుర్తులేని పూర్ణాంకం old_cbdt_ప్రధానసంఖ్య_నిడివి = bitmap_కొలత_context->cbdt_ప్రధానసంఖ్య->నిడివి;

    // Set to invalid state to indicate filling glyphs is not yet started.
    ఒకవేళ (unlikely (!c->serializer->check_success (రికార్డులు->resize (రికార్డులు->నిడివి + 1))))
      return_trace (అబద్ధం);

    రికార్డులు->తోక ().మొదటి_Glyph_సూచీ = 1;
    రికార్డులు->తోక ().చివరి_Glyph_సూచీ = 0;
    bitmap_కొలత_context->కొలత += సూచీ_Subtable_రికార్డు::min_కొలత;

    c->serializer->push ();

    ఒకవేళ (unlikely (!కూడు_new_subtable (c, bitmap_కొలత_context, &(రికార్డులు->తోక ()), lookup, పునాది, ప్రారంభం)))
    {
      c->serializer->తీయి_discard ();
      c->serializer->revert (snap);
      bitmap_కొలత_context->cbdt_ప్రధానసంఖ్య->shrink (old_cbdt_ప్రధానసంఖ్య_నిడివి);
      bitmap_కొలత_context->కొలత = old_కొలత;
      రికార్డులు->resize (రికార్డులు->నిడివి - 1);
      return_trace (అబద్ధం);
    }

    bitmap_కొలత_context->అంకె_tables += 1;
    return_trace (నిజం);
  }

  గుర్తులేని పూర్ణాంకం కూడు_glyph_for_subset (hb_codepoint_t gid)
  {
    ఒకవేళ (మొదటి_Glyph_సూచీ > చివరి_Glyph_సూచీ)
    {
      మొదటి_Glyph_సూచీ = gid;
      చివరి_Glyph_సూచీ = gid;
      తిరిగి 0;
    }
    // TODO maybe assert? this shouldn't occur.
    ఒకవేళ (చివరి_Glyph_సూచీ > gid)
      తిరిగి 0;
    గుర్తులేని పూర్ణాంకం అంకె_missing = (గుర్తులేని పూర్ణాంకం) (gid - చివరి_Glyph_సూచీ - 1);
    చివరి_Glyph_సూచీ = gid;
    తిరిగి అంకె_missing;
  }

  తార్కిక get_extents (hb_glyph_extents_t *extents, స్థిరరాశి శూన్య *పునాది, తార్కిక scale) స్థిరరాశి
  { తిరిగి (పునాది+స్థానభ్రంశం_గమ్యం_Subtable).get_extents (extents, scale); }

  తార్కిక get_image_దత్తాంశం (గుర్తులేని పూర్ణాంకం  gid,
		       స్థిరరాశి శూన్య   *పునాది,
		       గుర్తులేని పూర్ణాంకం *స్థానభ్రంశం,
		       గుర్తులేని పూర్ణాంకం *నిడివి,
		       గుర్తులేని పూర్ణాంకం *format) స్థిరరాశి
  {
    ఒకవేళ (gid < మొదటి_Glyph_సూచీ || gid > చివరి_Glyph_సూచీ) తిరిగి అబద్ధం;
    తిరిగి (పునాది+స్థానభ్రంశం_గమ్యం_Subtable).get_image_దత్తాంశం (gid - మొదటి_Glyph_సూచీ,
						   స్థానభ్రంశం, నిడివి, format);
  }

  HBGlyphID16			మొదటి_Glyph_సూచీ;
  HBGlyphID16			చివరి_Glyph_సూచీ;
  Offset32_గమ్యం<సూచీ_Subtable>	స్థానభ్రంశం_గమ్యం_Subtable;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం సూచీ_Subtable_Array
{
  మిత్ర నిర్మాణం CBDT;

  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం లెక్క) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (సూచీ_Subtables_Z.sanitize (c, లెక్క, ఇది));
  }

  శూన్య
  build_lookup (hb_subset_context_t *c, cblc_bitmap_కొలత_subset_context_t *bitmap_కొలత_context,
		hb_సదిశ_t<hb_pair_t<hb_codepoint_t,
		స్థిరరాశి సూచీ_Subtable_రికార్డు*>> *lookup /*బహిర్గతం*/) స్థిరరాశి
  {
    తార్కిక ప్రారంభం_glyph_is_సమితి = అబద్ధం;
    గుర్తులేని అంకె_glyphs = c->plan->అంకె_output_glyphs ();
    కొరకు (hb_codepoint_t new_gid = 0; new_gid < అంకె_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      ఒకవేళ (unlikely (!c->plan->old_gid_for_new_gid (new_gid, &old_gid))) కొనసాగు;

      స్థిరరాశి సూచీ_Subtable_రికార్డు* రికార్డు = కనుగొను_పట్టిక (old_gid, bitmap_కొలత_context->అంకె_tables);
      ఒకవేళ (unlikely (!రికార్డు)) కొనసాగు;

      // Don't add gaps to the lookup. The best way to determine if a glyph is a
      // gap is that it has no image data.
      గుర్తులేని పూర్ణాంకం స్థానభ్రంశం, నిడివి, format;
      ఒకవేళ (unlikely (!రికార్డు->get_image_దత్తాంశం (old_gid, ఇది, &స్థానభ్రంశం, &నిడివి, &format))) కొనసాగు;

      lookup->push (hb_pair_t<hb_codepoint_t, స్థిరరాశి సూచీ_Subtable_రికార్డు*> (new_gid, రికార్డు));

      ఒకవేళ (!ప్రారంభం_glyph_is_సమితి)
      {
	bitmap_కొలత_context->ప్రారంభం_glyph = new_gid;
	ప్రారంభం_glyph_is_సమితి = నిజం;
      }

      bitmap_కొలత_context->చివర_glyph = new_gid;
    }
  }

  తార్కిక
  subset (hb_subset_context_t *c,
	  cblc_bitmap_కొలత_subset_context_t *bitmap_కొలత_context) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    hb_సదిశ_t<hb_pair_t<hb_codepoint_t, స్థిరరాశి సూచీ_Subtable_రికార్డు*>> lookup;
    build_lookup (c, bitmap_కొలత_context, &lookup);
    ఒకవేళ (unlikely (!c->serializer->propagate_error (lookup)))
      తిరిగి అబద్ధం;

    bitmap_కొలత_context->కొలత = 0;
    bitmap_కొలత_context->అంకె_tables = 0;
    hb_సదిశ_t<సూచీ_Subtable_రికార్డు> రికార్డులు;
    కొరకు (గుర్తులేని పూర్ణాంకం ప్రారంభం = 0; ప్రారంభం < lookup.నిడివి;)
    {
      ఒకవేళ (unlikely (!lookup[ప్రారంభం].second->కూడు_new_రికార్డు (c, bitmap_కొలత_context, &lookup, ఇది, &ప్రారంభం, &రికార్డులు)))
      {
	// Discard any leftover pushes to the serializer from successful records.
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < రికార్డులు.నిడివి; i++)
	  c->serializer->తీయి_discard ();
	return_trace (అబద్ధం);
      }
    }

    /* Workaround to ensure offset ordering is from least to greatest when
     * resolving links. */
    hb_సదిశ_t<hb_serialize_context_t::objidx_t> objidxs;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < రికార్డులు.నిడివి; i++)
      objidxs.push (c->serializer->తీయి_pack ());
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < రికార్డులు.నిడివి; i++)
    {
      సూచీ_Subtable_రికార్డు* రికార్డు = c->serializer->embed (రికార్డులు[i]);
      ఒకవేళ (unlikely (!రికార్డు)) return_trace (అబద్ధం);
      c->serializer->కూడు_link (రికార్డు->స్థానభ్రంశం_గమ్యం_Subtable, objidxs[రికార్డులు.నిడివి - 1 - i]);
    }
    return_trace (నిజం);
  }

  బహిరంగ:
  స్థిరరాశి సూచీ_Subtable_రికార్డు* కనుగొను_పట్టిక (hb_codepoint_t glyph, గుర్తులేని పూర్ణాంకం అంకె_Tables) స్థిరరాశి
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_Tables; ++i)
    {
      గుర్తులేని పూర్ణాంకం మొదటి_Glyph_సూచీ = సూచీ_Subtables_Z[i].మొదటి_Glyph_సూచీ;
      గుర్తులేని పూర్ణాంకం చివరి_Glyph_సూచీ = సూచీ_Subtables_Z[i].చివరి_Glyph_సూచీ;
      ఒకవేళ (మొదటి_Glyph_సూచీ <= glyph && glyph <= చివరి_Glyph_సూచీ)
	తిరిగి &సూచీ_Subtables_Z[i];
    }
    తిరిగి శూన్యసూచిక;
  }

  రక్షిత:
  Unsized_Array_యొక్క<సూచీ_Subtable_రికార్డు>	సూచీ_Subtables_Z;
};

నిర్మాణం Bitmap_కొలత_పట్టిక
{
  మిత్ర నిర్మాణం CBLC;
  మిత్ర నిర్మాణం CBDT;

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  సూచీ_Subtable_Array_స్థానభ్రంశం.sanitize (c, పునాది, సంఖ్య_యొక్క_సూచీ_Subtables) &&
		  horizontal.sanitize (c) &&
		  vertical.sanitize (c));
  }

  స్థిరరాశి సూచీ_Subtable_రికార్డు *
  కనుగొను_పట్టిక (hb_codepoint_t glyph, స్థిరరాశి శూన్య *పునాది, స్థిరరాశి శూన్య **బహిర్గతం_పునాది) స్థిరరాశి
  {
    *బహిర్గతం_పునాది = &(పునాది+సూచీ_Subtable_Array_స్థానభ్రంశం);
    తిరిగి (పునాది+సూచీ_Subtable_Array_స్థానభ్రంశం).కనుగొను_పట్టిక (glyph, సంఖ్య_యొక్క_సూచీ_Subtables);
  }

  తార్కిక
  subset (hb_subset_context_t *c, స్థిరరాశి శూన్య *పునాది,
	  స్థిరరాశి అక్షరం *cbdt, గుర్తులేని పూర్ణాంకం cbdt_నిడివి,
	  hb_సదిశ_t<అక్షరం> *cbdt_ప్రధానసంఖ్య /* INOUT */,
	  cbdt_dedup_context_t *dedup /* INOUT */) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం_పట్టిక = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం_పట్టిక)) return_trace (అబద్ధం);

    cblc_bitmap_కొలత_subset_context_t bitmap_కొలత_context;
    bitmap_కొలత_context.cbdt = cbdt;
    bitmap_కొలత_context.cbdt_నిడివి = cbdt_నిడివి;
    bitmap_కొలత_context.cbdt_ప్రధానసంఖ్య = cbdt_ప్రధానసంఖ్య;
    bitmap_కొలత_context.కొలత = సూచీ_Tables_కొలత;
    bitmap_కొలత_context.అంకె_tables = సంఖ్య_యొక్క_సూచీ_Subtables;
    bitmap_కొలత_context.ప్రారంభం_glyph = 1;
    bitmap_కొలత_context.చివర_glyph = 0;
    bitmap_కొలత_context.dedup = dedup;

    ఒకవేళ (!బహిర్గతం_పట్టిక->సూచీ_Subtable_Array_స్థానభ్రంశం.serialize_subset (c,
							       సూచీ_Subtable_Array_స్థానభ్రంశం,
							       పునాది,
							       &bitmap_కొలత_context))
      return_trace (అబద్ధం);
    ఒకవేళ (!bitmap_కొలత_context.కొలత ||
	!bitmap_కొలత_context.అంకె_tables ||
	bitmap_కొలత_context.ప్రారంభం_glyph > bitmap_కొలత_context.చివర_glyph)
      return_trace (అబద్ధం);

    బహిర్గతం_పట్టిక->సూచీ_Tables_కొలత = bitmap_కొలత_context.కొలత;
    బహిర్గతం_పట్టిక->సంఖ్య_యొక్క_సూచీ_Subtables = bitmap_కొలత_context.అంకె_tables;
    బహిర్గతం_పట్టిక->ప్రారంభం_Glyph_సూచీ = bitmap_కొలత_context.ప్రారంభం_glyph;
    బహిర్గతం_పట్టిక->చివర_Glyph_సూచీ = bitmap_కొలత_context.చివర_glyph;
    return_trace (నిజం);
  }

  రక్షిత:
  NNOffset32_గమ్యం<సూచీ_Subtable_Array>
			సూచీ_Subtable_Array_స్థానభ్రంశం;
  HBUINT32		సూచీ_Tables_కొలత;
  HBUINT32		సంఖ్య_యొక్క_సూచీ_Subtables;
  HBUINT32		colorRef;
  SBit_పంక్తి_Metrics	horizontal;
  SBit_పంక్తి_Metrics	vertical;
  HBGlyphID16		ప్రారంభం_Glyph_సూచీ;
  HBGlyphID16		చివర_Glyph_సూచీ;
  HBUINT8		ppemX;
  HBUINT8		ppemY;
  HBUINT8		బిట్టు_లోతు;
  HBINT8		flags;
  బహిరంగ:
  DEFINE_కొలత_STATIC (48);
};


/*
 * Glyph Bitmap Data Formats.
 */

నిర్మాణం Glyph_Bitmap_దత్తాంశం_Format17
{
  SmallGlyphMetrics	glyphMetrics;
  Array32_యొక్క<HBUINT8>	దత్తాంశం;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (9, దత్తాంశం);
};

నిర్మాణం Glyph_Bitmap_దత్తాంశం_Format18
{
  BigGlyphMetrics	glyphMetrics;
  Array32_యొక్క<HBUINT8>	దత్తాంశం;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (12, దత్తాంశం);
};

నిర్మాణం Glyph_Bitmap_దత్తాంశం_Format19
{
  Array32_యొక్క<HBUINT8>	దత్తాంశం;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, దత్తాంశం);
};

నిర్మాణం CBLC
{
  మిత్ర నిర్మాణం CBDT;

  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_CBLC;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 2 || సంస్కరణ.major == 3) &&
		  hb_barrier () &&
		  కొలత_Tables.sanitize (c, ఇది));
  }

  స్థిర తార్కిక
  sink_cbdt (hb_subset_context_t *c, hb_సదిశ_t<అక్షరం>* cbdt_ప్రధానసంఖ్య)
  {
    hb_blob_t *cbdt_ప్రధానసంఖ్య_blob = hb_blob_create (cbdt_ప్రధానసంఖ్య->arrayZ,
						 cbdt_ప్రధానసంఖ్య->నిడివి,
						 HB_జ్ఞాపకం_బహుళకం_WRITABLE,
						 cbdt_ప్రధానసంఖ్య->arrayZ,
						 hb_free);
    cbdt_ప్రధానసంఖ్య->init ();  // Leak arrayZ to the blob.
    తార్కిక ret = c->plan->కూడు_పట్టిక (HB_OT_ట్యాగ్_CBDT, cbdt_ప్రధానసంఖ్య_blob);
    hb_blob_destroy (cbdt_ప్రధానసంఖ్య_blob);
    తిరిగి ret;
  }

  తార్కిక
  subset_కొలత_పట్టిక (hb_subset_context_t *c, స్థిరరాశి Bitmap_కొలత_పట్టిక& పట్టిక,
		     స్థిరరాశి అక్షరం *cbdt /*లోపల*/, గుర్తులేని పూర్ణాంకం cbdt_నిడివి,
		     CBLC *cblc_ప్రధానసంఖ్య /* INOUT */, hb_సదిశ_t<అక్షరం> *cbdt_ప్రధానసంఖ్య /* INOUT */,
		     cbdt_dedup_context_t *dedup /* INOUT */) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    cblc_ప్రధానసంఖ్య->కొలత_Tables.len++;

    స్వయంచాలక snap = c->serializer->snapshot ();
    స్వయంచాలక cbdt_ప్రధానసంఖ్య_len = cbdt_ప్రధానసంఖ్య->నిడివి;

    ఒకవేళ (!పట్టిక.subset (c, ఇది, cbdt, cbdt_నిడివి, cbdt_ప్రధానసంఖ్య, dedup))
    {
      cblc_ప్రధానసంఖ్య->కొలత_Tables.len--;
      c->serializer->revert (snap);
      cbdt_ప్రధానసంఖ్య->shrink (cbdt_ప్రధానసంఖ్య_len);
      return_trace (అబద్ధం);
    }
    return_trace (నిజం);
  }

  // Implemented in cc file as it depends on definition of CBDT.
  HB_అంతర్గత తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి;

  రక్షిత:
  స్థిరరాశి Bitmap_కొలత_పట్టిక &ఎంచుకో_strike (hb_font_t *font) స్థిరరాశి
  {
    గుర్తులేని లెక్క = కొలత_Tables.len;
    ఒకవేళ (unlikely (!లెక్క))
      తిరిగి Null (Bitmap_కొలత_పట్టిక);

    గుర్తులేని పూర్ణాంకం requested_ppem = hb_max (font->x_ppem, font->y_ppem);
    ఒకవేళ (!requested_ppem)
      requested_ppem = 1<<30; /* Choose largest strike. */
    గుర్తులేని పూర్ణాంకం ఉత్తమ_i = 0;
    గుర్తులేని పూర్ణాంకం ఉత్తమ_ppem = hb_max (కొలత_Tables[0].ppemX, కొలత_Tables[0].ppemY);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
    {
      గుర్తులేని పూర్ణాంకం ppem = hb_max (కొలత_Tables[i].ppemX, కొలత_Tables[i].ppemY);
      ఒకవేళ ((requested_ppem <= ppem && ppem < ఉత్తమ_ppem) ||
	  (requested_ppem > ఉత్తమ_ppem && ppem > ఉత్తమ_ppem))
      {
	ఉత్తమ_i = i;
	ఉత్తమ_ppem = ppem;
      }
    }

    తిరిగి కొలత_Tables[ఉత్తమ_i];
  }

  రక్షిత:
  Fixed_సంస్కరణ<>		సంస్కరణ;
  Array32_యొక్క<Bitmap_కొలత_పట్టిక>	కొలత_Tables;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, కొలత_Tables);
};

నిర్మాణం CBDT
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_CBDT;

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ఇది->cblc = hb_sanitize_context_t ().reference_పట్టిక<CBLC> (face);
      ఇది->cbdt = hb_sanitize_context_t ().reference_పట్టిక<CBDT> (face);

      upem = hb_face_get_upem (face);
    }
    ~accelerator_t ()
    {
      ఇది->cblc.destroy ();
      ఇది->cbdt.destroy ();
    }

    తార్కిక
    get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, తార్కిక scale = నిజం) స్థిరరాశి
    {
      స్థిరరాశి శూన్య *పునాది;
      స్థిరరాశి Bitmap_కొలత_పట్టిక &strike = ఇది->cblc->ఎంచుకో_strike (font);
      స్థిరరాశి సూచీ_Subtable_రికార్డు *subtable_రికార్డు = strike.కనుగొను_పట్టిక (glyph, cblc, &పునాది);
      ఒకవేళ (!subtable_రికార్డు || !strike.ppemX || !strike.ppemY)
	తిరిగి అబద్ధం;

      ఒకవేళ (subtable_రికార్డు->get_extents (extents, పునాది, scale))
	తిరిగి నిజం;

      గుర్తులేని పూర్ణాంకం image_స్థానభ్రంశం = 0, image_నిడివి = 0, image_format = 0;
      ఒకవేళ (!subtable_రికార్డు->get_image_దత్తాంశం (glyph, పునాది, &image_స్థానభ్రంశం, &image_నిడివి, &image_format))
	తిరిగి అబద్ధం;

      గుర్తులేని పూర్ణాంకం cbdt_len = cbdt.get_నిడివి ();
      ఒకవేళ (unlikely (image_స్థానభ్రంశం > cbdt_len || cbdt_len - image_స్థానభ్రంశం < image_నిడివి))
	తిరిగి అబద్ధం;

      ఎంపిక (image_format)
      {
      స్థితి 17: {
	ఒకవేళ (unlikely (image_నిడివి < Glyph_Bitmap_దత్తాంశం_Format17::min_కొలత))
	  తిరిగి అబద్ధం;
	స్వయంచాలక &glyphFormat17 = Struct_At_స్థానభ్రంశం<Glyph_Bitmap_దత్తాంశం_Format17> (ఇది->cbdt, image_స్థానభ్రంశం);
	glyphFormat17.glyphMetrics.get_extents (font, extents, scale);
	ఆపు;
      }
      స్థితి 18: {
	ఒకవేళ (unlikely (image_నిడివి < Glyph_Bitmap_దత్తాంశం_Format18::min_కొలత))
	  తిరిగి అబద్ధం;
	స్వయంచాలక &glyphFormat18 = Struct_At_స్థానభ్రంశం<Glyph_Bitmap_దత్తాంశం_Format18> (ఇది->cbdt, image_స్థానభ్రంశం);
	glyphFormat18.glyphMetrics.get_extents (font, extents, scale);
	ఆపు;
      }
      అప్రమేయ: తిరిగి అబద్ధం; /* TODO: Support other image formats. */
      }

      /* Convert to font units. */
      ఒకవేళ (scale)
      {
	దశాంశం x_scale = upem / (దశాంశం) strike.ppemX;
	దశాంశం y_scale = upem / (దశాంశం) strike.ppemY;
	extents->x_bearing = hb_clamp_గమ్యం<hb_position_t> (roundf (extents->x_bearing * x_scale));
	extents->y_bearing = hb_clamp_గమ్యం<hb_position_t> (roundf (extents->y_bearing * y_scale));
	extents->వెడల్పు = hb_clamp_గమ్యం<hb_position_t> (roundf (extents->వెడల్పు * x_scale));
	extents->ఎత్తు = hb_clamp_గమ్యం<hb_position_t> (roundf (extents->ఎత్తు * y_scale));
      }

      తిరిగి నిజం;
    }

    hb_blob_t*
    reference_png (hb_font_t *font, hb_codepoint_t glyph) స్థిరరాశి
    {
      స్థిరరాశి శూన్య *పునాది;
      స్థిరరాశి Bitmap_కొలత_పట్టిక &strike = ఇది->cblc->ఎంచుకో_strike (font);
      స్థిరరాశి సూచీ_Subtable_రికార్డు *subtable_రికార్డు = strike.కనుగొను_పట్టిక (glyph, cblc, &పునాది);
      ఒకవేళ (!subtable_రికార్డు || !strike.ppemX || !strike.ppemY)
	తిరిగి hb_blob_get_ఖాళీలు ();

      గుర్తులేని పూర్ణాంకం image_స్థానభ్రంశం = 0, image_నిడివి = 0, image_format = 0;
      ఒకవేళ (!subtable_రికార్డు->get_image_దత్తాంశం (glyph, పునాది, &image_స్థానభ్రంశం, &image_నిడివి, &image_format))
	తిరిగి hb_blob_get_ఖాళీలు ();

      గుర్తులేని పూర్ణాంకం cbdt_len = cbdt.get_నిడివి ();
      ఒకవేళ (unlikely (image_స్థానభ్రంశం > cbdt_len || cbdt_len - image_స్థానభ్రంశం < image_నిడివి))
	తిరిగి hb_blob_get_ఖాళీలు ();

      ఎంపిక (image_format)
      {
      స్థితి 17:
      {
	ఒకవేళ (unlikely (image_నిడివి < Glyph_Bitmap_దత్తాంశం_Format17::min_కొలత))
	  తిరిగి hb_blob_get_ఖాళీలు ();
	స్వయంచాలక &glyphFormat17 = Struct_At_స్థానభ్రంశం<Glyph_Bitmap_దత్తాంశం_Format17> (ఇది->cbdt, image_స్థానభ్రంశం);
	తిరిగి hb_blob_create_sub_blob (cbdt.get_blob (),
					image_స్థానభ్రంశం + Glyph_Bitmap_దత్తాంశం_Format17::min_కొలత,
					glyphFormat17.దత్తాంశం.len);
      }
      స్థితి 18:
      {
	ఒకవేళ (unlikely (image_నిడివి < Glyph_Bitmap_దత్తాంశం_Format18::min_కొలత))
	  తిరిగి hb_blob_get_ఖాళీలు ();
	స్వయంచాలక &glyphFormat18 = Struct_At_స్థానభ్రంశం<Glyph_Bitmap_దత్తాంశం_Format18> (ఇది->cbdt, image_స్థానభ్రంశం);
	తిరిగి hb_blob_create_sub_blob (cbdt.get_blob (),
					image_స్థానభ్రంశం + Glyph_Bitmap_దత్తాంశం_Format18::min_కొలత,
					glyphFormat18.దత్తాంశం.len);
      }
      స్థితి 19:
      {
	ఒకవేళ (unlikely (image_నిడివి < Glyph_Bitmap_దత్తాంశం_Format19::min_కొలత))
	  తిరిగి hb_blob_get_ఖాళీలు ();
	స్వయంచాలక &glyphFormat19 = Struct_At_స్థానభ్రంశం<Glyph_Bitmap_దత్తాంశం_Format19> (ఇది->cbdt, image_స్థానభ్రంశం);
	తిరిగి hb_blob_create_sub_blob (cbdt.get_blob (),
					image_స్థానభ్రంశం + Glyph_Bitmap_దత్తాంశం_Format19::min_కొలత,
					glyphFormat19.దత్తాంశం.len);
      }
      అప్రమేయ: తిరిగి hb_blob_get_ఖాళీలు (); /* TODO: Support other image formats. */
      }
    }

    తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి cbdt->సంస్కరణ.major; }

    తార్కిక paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, శూన్య *దత్తాంశం) స్థిరరాశి
    {
      ఒకవేళ (!has_దత్తాంశం ()) తిరిగి అబద్ధం;

      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;
      ఒకవేళ (unlikely (!font->get_glyph_extents (glyph, &extents, అబద్ధం)))
        తిరిగి అబద్ధం;

      ఒకవేళ (unlikely (!get_extents (font, glyph, &pixel_extents, అబద్ధం)))
        తిరిగి అబద్ధం;

      hb_blob_t *blob = reference_png (font, glyph);
      ఒకవేళ (unlikely (hb_blob_is_immutable (blob)))
        తిరిగి అబద్ధం;

      తార్కిక ret = funcs->image (దత్తాంశం,
			       blob,
			       pixel_extents.వెడల్పు, -pixel_extents.ఎత్తు,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      తిరిగి ret;
    }

    అంతరంగ:
    hb_blob_ptr_t<CBLC> cblc;
    hb_blob_ptr_t<CBDT> cbdt;

    గుర్తులేని పూర్ణాంకం upem;
  };

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 2 || సంస్కరణ.major == 3));
  }

  రక్షిత:
  Fixed_సంస్కరణ<>		సంస్కరణ;
  Unsized_Array_యొక్క<HBUINT8>	దత్తాంశం_Z;
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, దత్తాంశం_Z);
};

అంతఃవరుస తార్కిక
CBLC::subset (hb_subset_context_t *c) స్థిరరాశి
{
  TRACE_SUBSET (ఇది);

  // Use a vector as a secondary buffer as the tables need to be built in parallel.
  hb_సదిశ_t<అక్షరం> cbdt_ప్రధానసంఖ్య;

  స్వయంచాలక *cblc_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed<CBLC> ();
  ఒకవేళ (unlikely (!c->serializer->extend_min (cblc_ప్రధానసంఖ్య))) return_trace (అబద్ధం);
  cblc_ప్రధానసంఖ్య->సంస్కరణ = సంస్కరణ;

  hb_blob_t* cbdt_blob = hb_sanitize_context_t ().reference_పట్టిక<CBDT> (c->plan->ఆధారం);
  గుర్తులేని పూర్ణాంకం cbdt_నిడివి;
  CBDT* cbdt = (CBDT *) hb_blob_get_దత్తాంశం (cbdt_blob, &cbdt_నిడివి);
  ఒకవేళ (unlikely (cbdt_నిడివి < CBDT::min_కొలత))
  {
    hb_blob_destroy (cbdt_blob);
    return_trace (అబద్ధం);
  }
  copy_దత్తాంశం_గమ్యం_cbdt (&cbdt_ప్రధానసంఖ్య, cbdt, CBDT::min_కొలత);

  cbdt_dedup_context_t dedup;
  కొరకు (స్థిరరాశి Bitmap_కొలత_పట్టిక& పట్టిక : + కొలత_Tables.iter ())
    subset_కొలత_పట్టిక (c, పట్టిక, (స్థిరరాశి అక్షరం *) cbdt, cbdt_నిడివి, cblc_ప్రధానసంఖ్య, &cbdt_ప్రధానసంఖ్య, &dedup);

  hb_blob_destroy (cbdt_blob);

  return_trace (CBLC::sink_cbdt (c, &cbdt_ప్రధానసంఖ్య));
}

నిర్మాణం CBDT_accelerator_t : CBDT::accelerator_t {
  CBDT_accelerator_t (hb_face_t *face) : CBDT::accelerator_t (face) {}
};


} /* namespace OT */

#అయితే_ముగింపు /* OT_COLOR_CBDT_CBDT_HH */
