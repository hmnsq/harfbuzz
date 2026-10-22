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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OT_POST_పట్టిక_HH
#నిర్వచించు HB_OT_POST_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-var-mvar-table.hh"

#నిర్వచించు HB_STRING_ARRAY_NAME format1_పేర్లు
#నిర్వచించు HB_STRING_ARRAY_LIST "hb-ot-post-macroman.hh"
#అన్తర్భావ "hb-string-array.hh"
#నిర్వచనం_రద్దు HB_STRING_ARRAY_LIST
#నిర్వచనం_రద్దు HB_STRING_ARRAY_NAME

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */
#నిర్వచించు HB_OT_ట్యాగ్_post HB_ట్యాగ్('p','o','s','t')


నామాకాశ OT {


నిర్మాణం post_V2_తోక
{
  మిత్ర నిర్మాణం post;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (glyph_Name_సూచీ.sanitize (c));
  }

  మూస<రకం_పేరు Iterator>
  తార్కిక serialize (hb_serialize_context_t *c,
                  Iterator it,
                  స్థిరరాశి శూన్య* _post) స్థిరరాశి;

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి;

  రక్షిత:
  Array16_యొక్క<HBUINT16>	glyph_Name_సూచీ;	/* This is not an offset, but is the
					 * ordinal number of the glyph in 'post'
					 * string tables. */
/*UnsizedArrayOf<HBUINT8>
			namesX;*/	/* Glyph names with length bytes [variable]
					 * (a Pascal string). */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, glyph_Name_సూచీ);
};

నిర్మాణం post
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_post;

  తార్కిక serialize (hb_serialize_context_t *c, తార్కిక glyph_పేర్లు) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    post *post_ప్రధానసంఖ్య = c->allocate_min<post> ();
    ఒకవేళ (unlikely (!post_ప్రధానసంఖ్య))  return_trace (అబద్ధం);

    hb_memcpy (post_ప్రధానసంఖ్య, ఇది, post::min_కొలత);
    ఒకవేళ (!glyph_పేర్లు)
      return_trace (c->check_కేటాయించు (post_ప్రధానసంఖ్య->సంస్కరణ.major, 3,
                                     HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)); // Version 3 does not have any glyph names.

    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *post_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed<post> ();

    తార్కిక glyph_పేర్లు = c->plan->flags & HB_SUBSET_FLAGS_GLYPH_పేర్లు;
    ఒకవేళ (!serialize (c->serializer, glyph_పేర్లు))
      return_trace (అబద్ధం);

#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (c->plan->normalized_coords)
    {
      స్వయంచాలక &MVAR = *c->plan->ఆధారం->పట్టిక.MVAR;
      స్వయంచాలక *పట్టిక = post_ప్రధానసంఖ్య;

      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_UNDERLINE_కొలత,   underlineThickness);
      HB_కూడు_MVAR_VAR (HB_OT_METRICS_ట్యాగ్_UNDERLINE_స్థానభ్రంశం, underlinePosition);
    }
#అయితే_ముగింపు

    Triple *axis_range;
    ఒకవేళ (c->plan->user_axes_location.has (HB_ట్యాగ్ ('s','l','n','t'), &axis_range))
    {
      దశాంశం italic_angle = hb_max (-90.f, hb_min (axis_range->మధ్య, 90.f));
      ఒకవేళ (post_ప్రధానసంఖ్య->italicAngle.గమ్యం_float () != italic_angle)
        post_ప్రధానసంఖ్య->italicAngle.సమితి_float (italic_angle);
    }

    ఒకవేళ (glyph_పేర్లు && సంస్కరణ.major == 2)
    {
      hb_barrier ();
      return_trace (v2X.subset (c));
    }

    return_trace (నిజం);
  }

  నిర్మాణం accelerator_t
  {
    మిత్ర నిర్మాణం post_V2_తోక;

    accelerator_t (hb_face_t *face)
    {
      పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<post> (face);
      గుర్తులేని పూర్ణాంకం పట్టిక_నిడివి = పట్టిక.get_నిడివి ();

      సంస్కరణ = పట్టిక->సంస్కరణ.గమ్యం_int ();
      ఒకవేళ (సంస్కరణ != 0x00020000) తిరిగి;
      hb_barrier ();

      స్థిరరాశి post_V2_తోక &v2 = పట్టిక->v2X;

      glyph_Name_సూచీ = &v2.glyph_Name_సూచీ;
      pool = &StructAfter<uint8_t> (v2.glyph_Name_సూచీ);

      స్థిరరాశి uint8_t *చివర = (స్థిరరాశి uint8_t *) (స్థిరరాశి శూన్య *) పట్టిక + పట్టిక_నిడివి;
      సూచీ_గమ్యం_స్థానభ్రంశం.alloc (hb_min (face->get_అంకె_glyphs (), పట్టిక_నిడివి / 8));
      కొరకు (స్థిరరాశి uint8_t *దత్తాంశం = pool;
	   సూచీ_గమ్యం_స్థానభ్రంశం.నిడివి < 65535 && దత్తాంశం < చివర && దత్తాంశం + *దత్తాంశం < చివర;
	   దత్తాంశం += 1 + *దత్తాంశం)
	సూచీ_గమ్యం_స్థానభ్రంశం.push (దత్తాంశం - pool);
    }
    ~accelerator_t ()
    {
      hb_free (gids_sorted_by_name.get_acquire ());
      పట్టిక.destroy ();
    }

    తార్కిక get_glyph_name (hb_codepoint_t glyph,
			 అక్షరం *buf, గుర్తులేని పూర్ణాంకం buf_len) స్థిరరాశి
    {
      hb_బైట్లు_t s = కనుగొను_glyph_name (glyph);
      ఒకవేళ (!s.నిడివి) తిరిగి అబద్ధం;
      ఒకవేళ (!buf_len) తిరిగి నిజం;
      గుర్తులేని పూర్ణాంకం len = hb_min (buf_len - 1, s.నిడివి);
      strncpy (buf, s.arrayZ, len);
      buf[len] = '\0';
      తిరిగి నిజం;
    }

    తార్కిక get_glyph_from_name (స్థిరరాశి అక్షరం *name, పూర్ణాంకం len,
			      hb_codepoint_t *glyph) స్థిరరాశి
    {
      గుర్తులేని పూర్ణాంకం లెక్క = get_glyph_లెక్క ();
      ఒకవేళ (unlikely (!లెక్క)) తిరిగి అబద్ధం;

      ఒకవేళ (len < 0) len = అక్షరశ్రేణి_పొడవు (name);

      ఒకవేళ (unlikely (!len)) తిరిగి అబద్ధం;

    retry:
      uint16_t *gids = gids_sorted_by_name.get_acquire ();

      ఒకవేళ (unlikely (!gids))
      {
	gids = (uint16_t *) hb_malloc2 (లెక్క, పరిమాణం (gids[0]));
	ఒకవేళ (unlikely (!gids))
	  తిరిగి అబద్ధం; /* Anything better?! */

	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
	  gids[i] = i;
	స్వయంచాలక thiz = ఇది;
	hb_array_t<uint16_t> (gids, లెక్క)
	  .qsort ([thiz] (స్థిరరాశి uint16_t &a, స్థిరరాశి uint16_t &b) {
	    /* hb_array_t::cmp has reversed argument order, so
	     * x.cmp(y) returns sign of (y - x).  Pass (b, a) to
	     * get standard "negative when a < b" ordering. */
	    తిరిగి thiz->కనుగొను_glyph_name (b).cmp (thiz->కనుగొను_glyph_name (a));
	  });

	ఒకవేళ (unlikely (!gids_sorted_by_name.cmpexch (శూన్యసూచిక, gids)))
	{
	  hb_free (gids);
	  వెళ్ళు retry;
	}
      }

      hb_బైట్లు_t st (name, len);
      స్వయంచాలక* gid = hb_bsearch (st, gids, లెక్క, పరిమాణం (gids[0]), cmp_కీలకం, (శూన్య *) ఇది);
      ఒకవేళ (gid)
      {
	*glyph = *gid;
	తిరిగి నిజం;
      }

      తిరిగి అబద్ధం;
    }

    hb_blob_ptr_t<post> పట్టిక;

    రక్షిత:

    గుర్తులేని పూర్ణాంకం get_glyph_లెక్క () స్థిరరాశి
    {
      ఒకవేళ (సంస్కరణ == 0x00010000)
      {
        hb_barrier ();
	తిరిగి format1_పేర్లు_నిడివి;
      }

      ఒకవేళ (సంస్కరణ == 0x00020000)
      {
        hb_barrier ();
	తిరిగి glyph_Name_సూచీ->len;
      }

      తిరిగి 0;
    }

    స్థిర పూర్ణాంకం cmp_కీలకం (స్థిరరాశి శూన్య *pk, స్థిరరాశి శూన్య *po, శూన్య *arg)
    {
      స్థిరరాశి accelerator_t *thiz = (స్థిరరాశి accelerator_t *) arg;
      స్థిరరాశి hb_బైట్లు_t *key = (స్థిరరాశి hb_బైట్లు_t *) pk;
      uint16_t o = * (స్థిరరాశి uint16_t *) po;
      తిరిగి thiz->కనుగొను_glyph_name (o).cmp (*key);
    }

    hb_బైట్లు_t కనుగొను_glyph_name (hb_codepoint_t glyph) స్థిరరాశి
    {
      ఒకవేళ (సంస్కరణ == 0x00010000)
      {
        hb_barrier ();
	ఒకవేళ (glyph >= format1_పేర్లు_నిడివి)
	  తిరిగి hb_బైట్లు_t ();

	తిరిగి format1_పేర్లు (glyph);
      }

      ఒకవేళ (సంస్కరణ != 0x00020000)
	తిరిగి hb_బైట్లు_t ();
      hb_barrier ();

      ఒకవేళ (glyph >= glyph_Name_సూచీ->len)
	తిరిగి hb_బైట్లు_t ();

      గుర్తులేని పూర్ణాంకం సూచీ_2 = glyph_Name_సూచీ->arrayZ[glyph];
      ఒకవేళ (సూచీ_2 < format1_పేర్లు_నిడివి)
	తిరిగి format1_పేర్లు (సూచీ_2);
      సూచీ_2 -= format1_పేర్లు_నిడివి;

      ఒకవేళ (సూచీ_2 >= సూచీ_గమ్యం_స్థానభ్రంశం.నిడివి)
	తిరిగి hb_బైట్లు_t ();
      గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = సూచీ_గమ్యం_స్థానభ్రంశం[సూచీ_2];

      స్థిరరాశి uint8_t *దత్తాంశం = pool + స్థానభ్రంశం;
      గుర్తులేని పూర్ణాంకం name_నిడివి = *దత్తాంశం;
      దత్తాంశం++;

      తిరిగి hb_బైట్లు_t ((స్థిరరాశి అక్షరం *) దత్తాంశం, name_నిడివి);
    }

    అంతరంగ:
    uint32_t సంస్కరణ;
    స్థిరరాశి Array16_యొక్క<HBUINT16> *glyph_Name_సూచీ = శూన్యసూచిక;
    hb_సదిశ_t<uint32_t> సూచీ_గమ్యం_స్థానభ్రంశం;
    స్థిరరాశి uint8_t *pool = శూన్యసూచిక;
    మార్చదగిన hb_atomic_t<uint16_t *> gids_sorted_by_name;
  };

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  (సంస్కరణ.గమ్యం_int () == 0x00010000 ||
		   (సంస్కరణ.గమ్యం_int () == 0x00020000 && hb_barrier () && v2X.sanitize (c)) ||
		   సంస్కరణ.గమ్యం_int () == 0x00030000));
  }

  బహిరంగ:
  Fixed_సంస్కరణ<>సంస్కరణ;		/* 0x00010000 for version 1.0
					 * 0x00020000 for version 2.0
					 * 0x00025000 for version 2.5 (deprecated)
					 * 0x00030000 for version 3.0 */
  F16DOT16	italicAngle;		/* Italic angle in counter-clockwise degrees
					 * from the vertical. Zero for upright text,
					 * negative for text that leans to the right
					 * (forward). */
  FWORD		underlinePosition;	/* This is the suggested distance of the top
					 * of the underline from the baseline
					 * (negative values indicate below baseline).
					 * The PostScript definition of this FontInfo
					 * dictionary key (the y coordinate of the
					 * center of the stroke) is not used for
					 * historical reasons. The value of the
					 * PostScript key may be calculated by
					 * subtracting half the underlineThickness
					 * from the value of this field. */
  FWORD		underlineThickness;	/* Suggested values for the underline
					   thickness. */
  HBUINT32	isFixedPitch;		/* Set to 0 if the font is proportionally
					 * spaced, non-zero if the font is not
					 * proportionally spaced (i.e. monospaced). */
  HBUINT32	minMemType42;		/* Minimum memory usage when an OpenType font
					 * is downloaded. */
  HBUINT32	maxMemType42;		/* Maximum memory usage when an OpenType font
					 * is downloaded. */
  HBUINT32	minMemType1;		/* Minimum memory usage when an OpenType font
					 * is downloaded as a Type 1 font. */
  HBUINT32	maxMemType1;		/* Maximum memory usage when an OpenType font
					 * is downloaded as a Type 1 font. */
  post_V2_తోక	v2X;
  DEFINE_కొలత_MIN (32);
};

నిర్మాణం post_accelerator_t : post::accelerator_t {
  post_accelerator_t (hb_face_t *face) : post::accelerator_t (face) {}
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_POST_TABLE_HH */
