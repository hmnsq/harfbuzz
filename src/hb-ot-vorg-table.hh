/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#నిర్వచితం_కాకపోతే HB_OT_VORG_పట్టిక_HH
#నిర్వచించు HB_OT_VORG_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#నిర్వచించు HB_OT_ట్యాగ్_VORG HB_ట్యాగ్('V','O','R','G')

నామాకాశ OT {

మూస <రకం_పేరు Types>
నిర్మాణం VertOriginMetric
{
  పూర్ణాంకం cmp (hb_codepoint_t g) స్థిరరాశి { తిరిగి glyph.cmp (g); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  రకం_పేరు Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  బహిరంగ:
  DEFINE_కొలత_STATIC (Types::HBGlyphID::static_కొలత + FWORD::static_కొలత);
};

మూస <రకం_పేరు Types>
నిర్మాణం VORGFormat1_2
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_VORG;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }

  HB_ALWAYS_INLINE
  పూర్ణాంకం get_y_origin (hb_codepoint_t glyph) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i;
    ఒకవేళ (!vertYOrigins.bfind (glyph, &i))
      తిరిగి defaultVertOriginY;
    తిరిగి vertYOrigins[i].vertOriginY;
  }

  మూస <రకం_పేరు Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    ఒకవేళ (unlikely (!c->extend_min ((*ఇది))))  తిరిగి;

    ఇది->సంస్కరణ.major = Types::కొలత == 2 ? 1 : 2;
    ఇది->సంస్కరణ.minor = 0;

    ఇది->defaultVertOriginY = defaultVertOriginY;
    ఇది->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *vorg_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed<VORGFormat1_2> ();
    ఒకవేళ (unlikely (!c->serializer->check_success (vorg_ప్రధానసంఖ్య))) return_trace (అబద్ధం);

    స్వయంచాలక it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (స్థిరరాశి VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_సమితి_విలువ_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		తిరిగి metric;
	      })
    ;

    /* serialize the new table */
    vorg_ప్రధానసంఖ్య->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  సంస్కరణ.major == (Types::కొలత == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  రకం_పేరు Types::మూస Sorted_Array_యొక్క<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  బహిరంగ:
  DEFINE_కొలత_ARRAY(6 + Types::HBUINT::static_కొలత, vertYOrigins);
};

నిర్మాణం VORG
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_VORG;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి u.సంస్కరణ.v.గమ్యం_int (); }

  HB_ALWAYS_INLINE
  పూర్ణాంకం get_y_origin (hb_codepoint_t glyph) స్థిరరాశి
  {
    ఎంపిక (u.సంస్కరణ.v.major) {
    స్థితి 1: తిరిగి u.version1.get_y_origin (glyph);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 2: తిరిగి u.version2.get_y_origin (glyph);
#అయితే_ముగింపు
    అప్రమేయ: తిరిగి 0;
    }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఎంపిక (u.సంస్కరణ.v.major) {
    స్థితి 1: return_trace (u.version1.subset (c));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 2: return_trace (u.version2.subset (c));
#అయితే_ముగింపు
    అప్రమేయ: return_trace (అబద్ధం);
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!u.సంస్కరణ.v.sanitize (c))) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.సంస్కరణ.v.major) {
    స్థితి 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#అయితే_ముగింపు
    అప్రమేయ: return_trace (అబద్ధం);
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం {
  Fixed_సంస్కరణ<>v;		/* Version of VORG table. */
  }				సంస్కరణ;
  VORGFormat1_2<Layout::SmallTypes>version1;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#అయితే_ముగింపు
  } u;

  బహిరంగ:
  DEFINE_కొలత_UNION (4, సంస్కరణ.v);
};
} /* namespace OT */

#అయితే_ముగింపు /* HB_OT_VORG_TABLE_HH */
