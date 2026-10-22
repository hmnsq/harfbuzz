/*
 * Copyright © 2022  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#అన్తర్భావ "graph.hh"
#అన్తర్భావ "../hb-ot-layout-common.hh"
#అన్తర్భావ "graph-result.hh"

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_CLASSDEF_లేఖాచిత్రం_HH
#నిర్వచించు లేఖాచిత్రం_CLASSDEF_లేఖాచిత్రం_HH

నామాకాశ లేఖాచిత్రం {

మూస <రకం_పేరు Types>
నిర్మాణం ClassDefFormat1_3 : బహిరంగ OT::ClassDefFormat1_3<Types>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత();
    constexpr గుర్తులేని min_కొలత = OT::ClassDefFormat1_3<Types>::min_కొలత;
    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();
    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత + ఇది->class_విలువ.get_కొలత () - ఇది->class_విలువ.len.get_కొలత ()))
      తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }
};

మూస <రకం_పేరు Types>
నిర్మాణం ClassDefFormat2_4 : బహిరంగ OT::ClassDefFormat2_4<Types>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత();
    constexpr గుర్తులేని min_కొలత = OT::ClassDefFormat2_4<Types>::min_కొలత;
    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();
    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత + ఇది->range_రికార్డు.get_కొలత () - ఇది->range_రికార్డు.len.get_కొలత ()))
      తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }
};

నిర్మాణం ClassDef : బహిరంగ OT::ClassDef
{
  మూస<రకం_పేరు It>
  స్థిర లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_class_def (gsubgpos_లేఖాచిత్రం_context_t& c,
                                             గుర్తులేని మాతృ_id,
                                             గుర్తులేని link_position,
                                             It glyph_and_class,
                                             గుర్తులేని max_size)
  {
    TRY_కేటాయించు (గుర్తులేని class_def_ప్రధానసంఖ్య_id, c.లేఖాచిత్రం.new_కణుపు (శూన్యసూచిక, శూన్యసూచిక));
    స్వయంచాలక& class_def_ప్రధానసంఖ్య_శీర్షం = c.లేఖాచిత్రం.vertices_[class_def_ప్రధానసంఖ్య_id];
    TRY (make_class_def (c, glyph_and_class, class_def_ప్రధానసంఖ్య_id, max_size));

    TRY(c.లేఖాచిత్రం.vertices_[మాతృ_id].కూడు_real_link (SmallTypes::కొలత, class_def_ప్రధానసంఖ్య_id, link_position));
    TRY(class_def_ప్రధానసంఖ్య_శీర్షం.కూడు_మాతృ (మాతృ_id, అబద్ధం));

    తిరిగి Ok();
  }

  మూస<రకం_పేరు It>
  స్థిర లేఖాచిత్రం_ఫలితం_t<శూన్య> make_class_def (gsubgpos_లేఖాచిత్రం_context_t& c,
                                              It glyph_and_class,
                                              గుర్తులేని dest_obj,
                                              గుర్తులేని max_size)
  {
    అక్షరం* బఫరు = (అక్షరం*) hb_calloc (1, max_size);
    ఒకవేళ (unlikely (!బఫరు))
      తిరిగి Err(కేటాయింపు_FAILURE);

    hb_serialize_context_t serializer (బఫరు, max_size);
    OT::ClassDef_serialize (&serializer, glyph_and_class);
    serializer.చివర_serialize ();
    ఒకవేళ (unlikely (serializer.లోపల_error ()))
    {
      hb_free (బఫరు);
      తిరిగి Err(కేటాయింపు_FAILURE);
    }

    hb_బైట్లు_t class_def_copy = serializer.copy_బైట్లు ();
    ఒకవేళ (unlikely (!class_def_copy.arrayZ))
    {
      hb_free (బఫరు);
      తిరిగి Err(కేటాయింపు_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    స్వయంచాలక res = c.కూడు_బఫరు ((అక్షరం *) class_def_copy.arrayZ);
    ఒకవేళ (unlikely (!res.is_ok ()))
    {
      hb_free (బఫరు);
      hb_free ((అక్షరం *) class_def_copy.arrayZ);
      తిరిగి res;
    }


    స్వయంచాలక& v = c.లేఖాచిత్రం.vertices_[dest_obj];
    అక్షరం* తల_2 = (అక్షరం *) class_def_copy.arrayZ;
    v.సమితి_బఫరు (తల_2, తల_2 + class_def_copy.నిడివి);

    hb_free (బఫరు);
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత();
    ఒకవేళ (unlikely (శీర్షం_len < OT::ClassDef::min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();
    ఎంపిక (u.format.v)
    {
    స్థితి 1: తిరిగి ((ClassDefFormat1_3<SmallTypes>*)ఇది)->sanitize (శీర్షం);
    స్థితి 2: తిరిగి ((ClassDefFormat2_4<SmallTypes>*)ఇది)->sanitize (శీర్షం);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: తిరిగి ((ClassDefFormat1_3<MediumTypes>*)ఇది)->sanitize (శీర్షం);
    స్థితి 4: తిరిగి ((ClassDefFormat2_4<MediumTypes>*)ఇది)->sanitize (శీర్షం);
#అయితే_ముగింపు
    అప్రమేయ: తిరిగి Err(SANITIZE_FAILURE);
    }
  }
};


నిర్మాణం class_def_కొలత_estimator_t
{
  మూస<రకం_పేరు It>
  స్థిర లేఖాచిత్రం_ఫలితం_t<class_def_కొలత_estimator_t> create (It glyph_and_class)
  {
    class_def_కొలత_estimator_t estimator;
    estimator.reset();
    కొరకు (స్వయంచాలక p : + glyph_and_class)
    {
      గుర్తులేని gid = p.మొదటి;
      గుర్తులేని klass = p.second;

      hb_సమితి_t* glyphs;
      ఒకవేళ (estimator.glyphs_చొప్పున_class.has (klass, &glyphs) && glyphs) {
        glyphs->కూడు (gid);
        కొనసాగు;
      }

      hb_సమితి_t new_glyphs;
      new_glyphs.కూడు (gid);
      estimator.glyphs_చొప్పున_class.సమితి (klass, std::move (new_glyphs));
    }

    TRY(estimator.గమ్యం_ఫలితం ());

    కొరకు (గుర్తులేని klass : estimator.glyphs_చొప్పున_class.కీలకాలు ())
    {
      ఒకవేళ (!klass) కొనసాగు; // class 0 doesn't get encoded.

      స్థిరరాశి hb_సమితి_t& glyphs = estimator.glyphs_చొప్పున_class.get (klass);
      hb_codepoint_t ప్రారంభం = HB_సమితి_విలువ_INVALID;
      hb_codepoint_t చివర = HB_సమితి_విలువ_INVALID;

      గుర్తులేని లెక్క = 0;
      అయితే (glyphs.తదుపరి_range (&ప్రారంభం, &చివర))
        లెక్క++;

      estimator.అంకె_ranges_చొప్పున_class.సమితి (klass, లెక్క);
    }

    TRY(estimator.గమ్యం_ఫలితం ());
    తిరిగి estimator;
  }

  శూన్య reset() {
    included_glyphs.clear();
    included_class_def_glyphs.clear();
    included_classes.clear();
    class_def_range_లెక్క = 0;
    class_max = 0;
    coverage_కొలత_val = compute_coverage_కొలత ();
  }

  // Size of coverage for all glyphs added via 'add_class_def_size'.
  గుర్తులేని coverage_కొలత () స్థిరరాశి
  {
    తిరిగి coverage_కొలత_val;
  }

  // Compute the new size of the ClassDef table if all glyphs associated with 'klass' were added.
  గుర్తులేని కూడు_class_def_కొలత (గుర్తులేని klass)
  {
    ఒకవేళ (!included_classes.has(klass)) {
      hb_సమితి_t* glyphs = శూన్యసూచిక;
      ఒకవేళ (glyphs_చొప్పున_class.has(klass, &glyphs)) {
        గుర్తులేని అంకె_glyphs = included_glyphs.get_population();
        included_glyphs.union_(*glyphs);
        ఒకవేళ (klass)
          included_class_def_glyphs.union_(*glyphs);
        ఒకవేళ (అంకె_glyphs != included_glyphs.get_population())
          coverage_కొలత_val = compute_coverage_కొలత ();
      }

      ఒకవేళ (klass)
      {
        class_def_range_లెక్క += అంకె_ranges_చొప్పున_class.get (klass);
        ఒకవేళ (glyphs && klass > class_max)
          class_max = klass;
      }
      included_classes.కూడు(klass);
    }

    తిరిగి class_def_కొలత ();
  }

  గుర్తులేని అంకె_glyph_ranges (స్థిరరాశి hb_సమితి_t &glyphs) స్థిరరాశి {
    hb_codepoint_t ప్రారంభం = HB_సమితి_విలువ_INVALID;
    hb_codepoint_t చివర = HB_సమితి_విలువ_INVALID;

    గుర్తులేని లెక్క = 0;
    అయితే (glyphs.తదుపరి_range (&ప్రారంభం, &చివర)) {
        లెక్క++;
    }
    తిరిగి లెక్క;
  }

 అంతరంగ:
  లేఖాచిత్రం_ఫలితం_t<శూన్య> గమ్యం_ఫలితం() స్థిరరాశి
  {
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (అంకె_ranges_చొప్పున_class, కేటాయింపు_FAILURE));
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (glyphs_చొప్పున_class, కేటాయింపు_FAILURE));
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (included_classes, కేటాయింపు_FAILURE));
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (included_glyphs, కేటాయింపు_FAILURE));
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (included_class_def_glyphs, కేటాయింపు_FAILURE));

    కొరకు (స్థిరరాశి hb_సమితి_t& s : glyphs_చొప్పున_class.values ())
    {
          TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (s, కేటాయింపు_FAILURE));
    }
    తిరిగి Ok();
  }

  class_def_కొలత_estimator_t ()
      : అంకె_ranges_చొప్పున_class (), glyphs_చొప్పున_class () {}

  గుర్తులేని compute_coverage_కొలత () స్థిరరాశి
  {
    గుర్తులేని glyph_కొలత = glyph_id_కొలత (included_glyphs);
    గుర్తులేని coverage_పునాది_కొలత = OT::HBUINT16::static_కొలత + glyph_కొలత;
    గుర్తులేని range_కొలత = 3 * glyph_కొలత;
    గుర్తులేని format1_కొలత = coverage_పునాది_కొలత + glyph_కొలత * included_glyphs.get_population();
    గుర్తులేని format2_కొలత = coverage_పునాది_కొలత + range_కొలత * అంకె_glyph_ranges (included_glyphs);
    తిరిగి hb_min(format1_కొలత, format2_కొలత);
  }

  గుర్తులేని glyph_id_కొలత (స్థిరరాశి hb_సమితి_t &glyphs) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (!glyphs.is_empty () && glyphs.get_max () > 0xFFFFu)
      తిరిగి OT::Layout::MediumTypes::కొలత;
#అయితే_ముగింపు
    తిరిగి OT::Layout::SmallTypes::కొలత;
  }

  గుర్తులేని class_def_కొలత () స్థిరరాశి
  {
    ఒకవేళ (included_class_def_glyphs.is_empty ())
      తిరిగి OT::HBUINT16::static_కొలత + OT::Layout::SmallTypes::కొలత;

    గుర్తులేని ఉత్తమ_కొలత = UINT_MAX;
    hb_codepoint_t glyph_max = included_class_def_glyphs.get_max ();
    గుర్తులేని glyph_పరిధి = glyph_max - included_class_def_glyphs.get_min () + 1;

    ఒకవేళ (glyph_max <= 0xFFFFu && glyph_పరిధి <= 0xFFFFu && class_max <= 0xFFFFu)
      ఉత్తమ_కొలత = hb_min (ఉత్తమ_కొలత, 6 + 2 * glyph_పరిధి);
    ఒకవేళ (glyph_max <= 0xFFFFu && class_def_range_లెక్క <= 0xFFFFu && class_max <= 0xFFFFu)
      ఉత్తమ_కొలత = hb_min (ఉత్తమ_కొలత, 4 + 6 * class_def_range_లెక్క);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (glyph_max <= 0xFFFFFFu && glyph_పరిధి <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
      ఉత్తమ_కొలత = hb_min (ఉత్తమ_కొలత, 8 + 3 * glyph_పరిధి);
    ఒకవేళ (glyph_max <= 0xFFFFFFu && class_def_range_లెక్క <= 0xFFFFFFu && class_max <= 0xFFFFu)
      ఉత్తమ_కొలత = hb_min (ఉత్తమ_కొలత, 5 + 8 * class_def_range_లెక్క);
#అయితే_ముగింపు
    తిరిగి ఉత్తమ_కొలత;
  }

  hb_hashmap_t<గుర్తులేని, గుర్తులేని> అంకె_ranges_చొప్పున_class;
  hb_hashmap_t<గుర్తులేని, hb_సమితి_t> glyphs_చొప్పున_class;
  hb_సమితి_t included_classes;
  hb_సమితి_t included_glyphs;
  hb_సమితి_t included_class_def_glyphs;
  గుర్తులేని class_def_range_లెక్క;
  గుర్తులేని class_max;
  గుర్తులేని coverage_కొలత_val;
};


}

#అయితే_ముగింపు  // GRAPH_CLASSDEF_GRAPH_HH
