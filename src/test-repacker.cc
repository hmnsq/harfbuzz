/*
 * Copyright © 2020  Google, Inc.
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

#అన్తర్భావ <cstdint>
#అన్తర్భావ <string>

#అన్తర్భావ "hb-repacker.hh"
#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "graph/serialize.hh"

స్థిర శూన్య extend (స్థిరరాశి అక్షరం* value,
                    గుర్తులేని len,
                    hb_serialize_context_t* c)
{
  అక్షరం* obj = c->allocate_కొలత<అక్షరం> (len);
  hb_memcpy (obj, value, len);
}

స్థిర శూన్య ప్రారంభం_object(స్థిరరాశి అక్షరం* ట్యాగ్_2,
                         గుర్తులేని len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (ట్యాగ్_2, len, c);
}

స్థిర గుర్తులేని కూడు_object(స్థిరరాశి అక్షరం* ట్యాగ్_2,
                           గుర్తులేని len,
                           hb_serialize_context_t* c,
                           తార్కిక shared = అబద్ధం)
{
  ప్రారంభం_object (ట్యాగ్_2, len, c);
  తిరిగి c->తీయి_pack (shared);
}


స్థిర శూన్య కూడు_స్థానభ్రంశం (గుర్తులేని id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* స్థానభ్రంశం = c->ప్రారంభం_embed<OT::Offset16> ();
  c->extend_min (స్థానభ్రంశం);
  c->కూడు_link (*స్థానభ్రంశం, id);
}

స్థిర శూన్య కూడు_24_స్థానభ్రంశం (గుర్తులేని id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* స్థానభ్రంశం = c->ప్రారంభం_embed<OT::Offset24> ();
  c->extend_min (స్థానభ్రంశం);
  c->కూడు_link (*స్థానభ్రంశం, id);
}

స్థిర శూన్య కూడు_wide_స్థానభ్రంశం (గుర్తులేని id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* స్థానభ్రంశం = c->ప్రారంభం_embed<OT::Offset32> ();
  c->extend_min (స్థానభ్రంశం);
  c->కూడు_link (*స్థానభ్రంశం, id);
}

స్థిర శూన్య కూడు_gsubgpos_శీర్షిక (గుర్తులేని lookup_list,
                                 hb_serialize_context_t* c)
{
  అక్షరం శీర్షిక[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  ప్రారంభం_object (శీర్షిక, 8, c);
  కూడు_స్థానభ్రంశం (lookup_list, c);
  c->తీయి_pack (అబద్ధం);
}

స్థిర శూన్య కూడు_gsubgpos_శీర్షిక_1_2 (గుర్తులేని lookup_list,
                                     hb_serialize_context_t* c)
{
  అక్షరం శీర్షిక[] = {
    0, 1, 0, 2, //సంస్కరణ
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  ప్రారంభం_object (శీర్షిక, పరిమాణం (శీర్షిక), c);
  కూడు_wide_స్థానభ్రంశం (lookup_list, c);
  c->తీయి_pack (అబద్ధం);
}

స్థిర గుర్తులేని కూడు_lookup_list (స్థిరరాశి గుర్తులేని* lookups,
                                 అక్షరం లెక్క,
                                 hb_serialize_context_t* c)
{
  అక్షరం lookup_లెక్క[] = {0, లెక్క};
  ప్రారంభం_object  ((అక్షరం *) &lookup_లెక్క, 2, c);

  కొరకు (పూర్ణాంకం i = 0; i < లెక్క; i++)
    కూడు_స్థానభ్రంశం (lookups[i], c);

  తిరిగి c->తీయి_pack (అబద్ధం);
}

స్థిర గుర్తులేని కూడు_lookup_list_1_2 (స్థిరరాశి గుర్తులేని* lookups,
                                     అక్షరం లెక్క,
                                     hb_serialize_context_t* c)
{
  అక్షరం lookup_లెక్క[] = {0, లెక్క};
  ప్రారంభం_object ((అక్షరం *) &lookup_లెక్క, 2, c);

  కొరకు (పూర్ణాంకం i = 0; i < లెక్క; i++)
    కూడు_wide_స్థానభ్రంశం (lookups[i], c);

  తిరిగి c->తీయి_pack (అబద్ధం);
}

స్థిర శూన్య ప్రారంభం_lookup (int8_t type,
                          int8_t అంకె_subtables,
                          hb_serialize_context_t* c)
{
  అక్షరం lookup[] = {
    0, (అక్షరం)type, // type
    0, (అక్షరం)2, // flag
    0, (అక్షరం)అంకె_subtables, // num subtables
  };

  ప్రారంభం_object (lookup, 6, c);
}

స్థిర గుర్తులేని ముగింపు_lookup (hb_serialize_context_t* c)
{
  అక్షరం filter[] = {0, 0};
  extend (filter, 2, c);
  తిరిగి c->తీయి_pack (అబద్ధం);
}

స్థిర గుర్తులేని కూడు_extension (గుర్తులేని బిడ్డ,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  అక్షరం ext[] = {
    0, 1,
    0, (అక్షరం) type,
  };

  ప్రారంభం_object (ext, 4, c);
  కూడు_wide_స్థానభ్రంశం (బిడ్డ, c);

  తిరిగి c->తీయి_pack (అబద్ధం);

}

// Adds coverage table fro [start, end]
స్థిర గుర్తులేని కూడు_coverage (గుర్తులేని ప్రారంభం, గుర్తులేని చివర,
                              hb_serialize_context_t* c,
                              తార్కిక shared = అబద్ధం)
{
  ఒకవేళ (చివర - ప్రారంభం == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //లెక్క

      (uint8_t) ((ప్రారంభం >> 8) & 0xFF),
      (uint8_t) (ప్రారంభం & 0xFF), // glyph[0]
    };
    తిరిగి కూడు_object ((అక్షరం*) coverage, 6, c, shared);
  }

  ఒకవేళ (చివర - ప్రారంభం == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //లెక్క

      (uint8_t) ((ప్రారంభం >> 8) & 0xFF),
      (uint8_t) (ప్రారంభం & 0xFF), // glyph[0]

      (uint8_t) ((చివర >> 8) & 0xFF),
      (uint8_t) (చివర & 0xFF), // glyph[1]
    };
    తిరిగి కూడు_object ((అక్షరం*) coverage, 8, c, shared);
  }

  uint8_t coverage[] = {
    0, 2, // format
    0, 1, // range count

    (uint8_t) ((ప్రారంభం >> 8) & 0xFF),
    (uint8_t) (ప్రారంభం & 0xFF), //ప్రారంభం

    (uint8_t) ((చివర >> 8) & 0xFF),
    (uint8_t) (చివర & 0xFF), //చివర

    0, 0,
  };
  తిరిగి కూడు_object ((అక్షరం*) coverage, 10, c, shared);
}


మూస<రకం_పేరు It>
స్థిర గుర్తులేని కూడు_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  తిరిగి c->తీయి_pack (అబద్ధం);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
స్థిర గుర్తులేని కూడు_class_def (uint16_t ప్రారంభం_glyph,
                               uint16_t చివర_glyph,
                               hb_serialize_context_t* c)
{
  గుర్తులేని లెక్క = చివర_glyph - ప్రారంభం_glyph;
  uint8_t శీర్షిక[] = {
    0, 1, // format

    (uint8_t) ((ప్రారంభం_glyph >> 8) & 0xFF),
    (uint8_t) (ప్రారంభం_glyph & 0xFF), // start_glyph

    (uint8_t) ((లెక్క >> 8) & 0xFF),
    (uint8_t) (లెక్క & 0xFF), //లెక్క
  };

  ప్రారంభం_object ((అక్షరం*) శీర్షిక, 6, c);
  కొరకు (uint16_t i = 1; i <= లెక్క; i++)
  {
    uint8_t class_విలువ_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //లెక్క
    };
    extend ((అక్షరం*) class_విలువ_2, 2, c);
  }

  తిరిగి c->తీయి_pack (అబద్ధం);
}

స్థిర గుర్తులేని కూడు_pair_pos_1 (గుర్తులేని* pair_sets,
                                అక్షరం లెక్క,
                                గుర్తులేని coverage,
                                hb_serialize_context_t* c)
{
  అక్షరం format[] = {
    0, 1
  };

  ప్రారంభం_object (format, 2, c);
  కూడు_స్థానభ్రంశం (coverage, c);

  అక్షరం విలువ_format[] = {
    0, 0,
    0, 0,
    0, లెక్క,
  };
  extend (విలువ_format, 6, c);

  కొరకు (అక్షరం i = 0; i < లెక్క; i++)
    కూడు_స్థానభ్రంశం (pair_sets[(గుర్తులేని) i], c);

  తిరిగి c->తీయి_pack (అబద్ధం);
}

స్థిర శూన్య కూడు_liga_సమితి_శీర్షిక (అక్షరం liga_లెక్క,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_లెక్క_బైట్లు[] = {
    (uint8_t) (0xFF & (liga_లెక్క >> 8)),
    (uint8_t) (0xFF & liga_లెక్క),
  };

  ప్రారంభం_object ((స్థిరరాశి అక్షరం*) liga_లెక్క_బైట్లు, 2, c);
}

స్థిర శూన్య కూడు_liga_శీర్షిక (గుర్తులేని coverage, గుర్తులేని liga_సమితి_లెక్క, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  ప్రారంభం_object ((స్థిరరాశి అక్షరం*) format, 2, c);
  కూడు_స్థానభ్రంశం(coverage, c);

  uint8_t liga_సమితి_లెక్క_బైట్లు[] = {
    (uint8_t) (0xFF & (liga_సమితి_లెక్క >> 8)),
    (uint8_t) (0xFF & liga_సమితి_లెక్క),
  };

  extend((స్థిరరాశి అక్షరం *) liga_సమితి_లెక్క_బైట్లు, 2, c);
}

స్థిర గుర్తులేని కూడు_pair_pos_2 (గుర్తులేని starting_class,
                                గుర్తులేని coverage,
                                గుర్తులేని class_def_1, uint16_t class_def_1_లెక్క,
                                గుర్తులేని class_def_2, uint16_t class_def_2_లెక్క,
                                గుర్తులేని* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  ప్రారంభం_object ((అక్షరం*) format, 2, c);
  కూడు_స్థానభ్రంశం (coverage, c);

  గుర్తులేని అంకె_విలువలు_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  ఒకవేళ (device_tables) {
    format2 |= 0x20;
    అంకె_విలువలు_3 += 1;
  }
  uint8_t విలువ_format[] = {
    0, format1,
    0, format2,
  };

  extend ((అక్షరం*) విలువ_format, 4, c);

  కూడు_స్థానభ్రంశం (class_def_1, c);
  కూడు_స్థానభ్రంశం (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_లెక్క >> 8) & 0xFF),
    (uint8_t) (class_def_1_లెక్క & 0xFF),
    (uint8_t) ((class_def_2_లెక్క >> 8) & 0xFF),
    (uint8_t) (class_def_2_లెక్క & 0xFF),
  };
  extend ((అక్షరం*) class_counts, 4, c);

  గుర్తులేని అంకె_బైట్లు_చొప్పున_రికార్డు = class_def_2_లెక్క * అంకె_విలువలు_3 * 2;
  uint8_t* రికార్డు = (uint8_t*) సున్నా_జ్ఞాపకం_ఇవ్వు (1, అంకె_బైట్లు_చొప్పున_రికార్డు);
  పూర్ణాంకం device_సూచీ = 0;
  కొరకు (uint16_t i = 0; i < class_def_1_లెక్క; i++)
  {

    కొరకు (uint16_t j = 0; j < class_def_2_లెక్క; j++)
    {
      కొరకు (పూర్ణాంకం k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((అక్షరం*) value, 2, c);
      }

      ఒకవేళ (device_tables) {
        కూడు_స్థానభ్రంశం (device_tables[device_సూచీ++], c);
      }
    }
  }
  విడుదల (రికార్డు);

  తిరిగి c->తీయి_pack (అబద్ధం);
}

స్థిర గుర్తులేని కూడు_mark_పునాది_pos_1 (గుర్తులేని mark_coverage,
                                     గుర్తులేని పునాది_coverage,
                                     గుర్తులేని mark_array,
                                     గుర్తులేని పునాది_array,
                                     గుర్తులేని class_లెక్క,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  ప్రారంభం_object ((అక్షరం*) format, 2, c);
  కూడు_స్థానభ్రంశం (mark_coverage, c);
  కూడు_స్థానభ్రంశం (పునాది_coverage, c);

  uint8_t లెక్క[] = {
    (uint8_t) ((class_లెక్క >> 8) & 0xFF),
    (uint8_t) (class_లెక్క & 0xFF),
  };
  extend ((అక్షరం*) లెక్క, 2, c);

  కూడు_స్థానభ్రంశం (mark_array, c);
  కూడు_స్థానభ్రంశం (పునాది_array, c);

  తిరిగి c->తీయి_pack (అబద్ధం);
}

మూస<పూర్ణాంకం mark_లెక్క,
    పూర్ణాంకం class_లెక్క,
    పూర్ణాంకం పునాది_లెక్క,
    పూర్ణాంకం పట్టిక_లెక్క_2>
నిర్మాణం Mark_పునాది_Pos_Buffers
{
  గుర్తులేని పునాది_anchors[class_లెక్క * పునాది_లెక్క];
  గుర్తులేని mark_anchors[mark_లెక్క];
  uint8_t anchor_buffers[class_లెక్క * పునాది_లెక్క + 100];
  uint8_t class_బఫరు[class_లెక్క * 2];

  Mark_పునాది_Pos_Buffers(hb_serialize_context_t* c)
  {
    కొరకు (గుర్తులేని i = 0; i < పరిమాణం(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    కొరకు (గుర్తులేని i = 0; i < class_లెక్క * పునాది_లెక్క; i++)
    {
      పునాది_anchors[i] = కూడు_object ((అక్షరం*) &anchor_buffers[i], 100, c);
      ఒకవేళ (i < class_లెక్క) {
        class_బఫరు[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_బఫరు[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    కొరకు (గుర్తులేని i = 0; i < mark_లెక్క; i++)
    {
      mark_anchors[i] = కూడు_object ((అక్షరం*) &anchor_buffers[i], 4, c);
    }
  }

  గుర్తులేని create_mark_పునాది_pos_1 (గుర్తులేని పట్టిక_సూచీ, hb_serialize_context_t* c)
  {
    గుర్తులేని class_చొప్పున_పట్టిక = class_లెక్క / పట్టిక_లెక్క_2;
    గుర్తులేని mark_చొప్పున_class = mark_లెక్క / class_లెక్క;
    గుర్తులేని ప్రారంభం_class = class_చొప్పున_పట్టిక * పట్టిక_సూచీ;
    గుర్తులేని చివర_class = class_చొప్పున_పట్టిక * (పట్టిక_సూచీ + 1) - 1;

    // baseArray
    uint8_t పునాది_లెక్క_బఫరు[] = {
      (uint8_t) ((పునాది_లెక్క >> 8) & 0xFF),
      (uint8_t) (పునాది_లెక్క & 0xFF),

    };
    ప్రారంభం_object ((అక్షరం*) పునాది_లెక్క_బఫరు, 2, c);
    కొరకు (గుర్తులేని పునాది = 0; పునాది < పునాది_లెక్క; పునాది++)
    {
      కొరకు (గుర్తులేని klass = ప్రారంభం_class; klass <= చివర_class; klass++)
      {
        గుర్తులేని i = పునాది * class_లెక్క + klass;
        కూడు_స్థానభ్రంశం (పునాది_anchors[i], c);
      }
    }
    గుర్తులేని పునాది_array = c->తీయి_pack (అబద్ధం);

    // markArray
    గుర్తులేని అంకె_marks = class_చొప్పున_పట్టిక * mark_చొప్పున_class;
    uint8_t mark_లెక్క_బఫరు[] = {
      (uint8_t) ((అంకె_marks >> 8) & 0xFF),
      (uint8_t) (అంకె_marks & 0xFF),
    };
    ప్రారంభం_object ((అక్షరం*) mark_లెక్క_బఫరు, 2, c);
    కొరకు (గుర్తులేని mark = 0; mark < mark_లెక్క; mark++)
    {
      గుర్తులేని klass = mark % class_లెక్క;
      ఒకవేళ (klass < ప్రారంభం_class || klass > చివర_class) కొనసాగు;
      klass -= ప్రారంభం_class;

      extend ((అక్షరం*) &class_బఫరు[2 * klass], 2, c);
      కూడు_స్థానభ్రంశం (mark_anchors[mark], c);
    }
    గుర్తులేని mark_array = c->తీయి_pack (అబద్ధం);

    // markCoverage
    స్వయంచాలక it =
        + hb_range ((hb_codepoint_t) mark_లెక్క)
        | hb_filter ([&] (hb_codepoint_t mark) {
          గుర్తులేని klass = mark % class_లెక్క;
          తిరిగి klass >= class_చొప్పున_పట్టిక * పట్టిక_సూచీ &&
              klass < class_చొప్పున_పట్టిక * (పట్టిక_సూచీ + 1);
        })
        ;
    గుర్తులేని mark_coverage = కూడు_coverage (it, c);

    // baseCoverage
    గుర్తులేని పునాది_coverage = కూడు_coverage (10, 10 + పునాది_లెక్క - 1, c);

    తిరిగి కూడు_mark_పునాది_pos_1 (mark_coverage,
                                పునాది_coverage,
                                mark_array,
                                పునాది_array,
                                class_చొప్పున_పట్టిక,
                                c);
  }
};

స్థిర శూన్య నిరంతరం_resolve_అధికప్రవాహం_test (స్థిరరాశి అక్షరం* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       గుర్తులేని అంకె_iterations = 0,
                                       తార్కిక recalculate_extensions = అబద్ధం,
                                       hb_ట్యాగ్_t ట్యాగ్_2 = HB_ట్యాగ్ ('G', 'S', 'U', 'B'),
                                       తార్కిక check_ద్విమాన_equivalence = అబద్ధం)
{
  రాయి (">>> Testing overflowing resolution for %s\n",
          name);

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (overflowing.object_లేఖాచిత్రం ()).value ();

  లేఖాచిత్రం_t expected_లేఖాచిత్రం = లేఖాచిత్రం_t::create (expected.object_లేఖాచిత్రం ()).value ();
  ఒకవేళ (లేఖాచిత్రం::will_అధికప్రవాహం (expected_లేఖాచిత్రం).value ())
  {
    ఒకవేళ (check_ద్విమాన_equivalence) {
      రాయి("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_ద్విమాన_equivalence);
    }
    hb_always_assert (expected_లేఖాచిత్రం.కేటాయించు_spaces ().is_ok ());
    hb_always_assert (expected_లేఖాచిత్రం.క్రమీకరించు_అతితక్కువ_దూరం ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.స్థానభ్రంశం_అధికప్రవాహం ());
  స్వయంచాలక r = hb_resolve_లేఖాచిత్రం_overflows (ట్యాగ్_2,
                                      అంకె_iterations,
                                      recalculate_extensions,
                                      లేఖాచిత్రం);
  ఒకవేళ (r.is_err())
    రాయి("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", లేఖాచిత్రం::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = లేఖాచిత్రం::serialize (లేఖాచిత్రం).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = లేఖాచిత్రం::serialize (expected_లేఖాచిత్రం).value ();
  hb_always_assert (out2);
  ఒకవేళ (check_ద్విమాన_equivalence) {
    గుర్తులేని l1, l2;
    స్థిరరాశి అక్షరం* d1 = hb_blob_get_దత్తాంశం(out1, &l1);
    స్థిరరాశి అక్షరం* d2 = hb_blob_get_దత్తాంశం(out2, &l2);

    తార్కిక సరిపోలిక = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    ఒకవేళ (!సరిపోలిక) {
      రాయి("## Result:\n");
      లేఖాచిత్రం.print();
      రాయి("## Expected:\n");
      expected_లేఖాచిత్రం.print();
      hb_always_assert(సరిపోలిక);
    }
  }

  hb_blob_destroy (out1);
  hb_blob_destroy (out2);

  // Check the graphs are equivalent
  లేఖాచిత్రం.normalize ();
  expected_లేఖాచిత్రం.normalize ();
  ఒకవేళ (!(లేఖాచిత్రం == expected_లేఖాచిత్రం)) {
    రాయి("## Expected:\n");
    expected_లేఖాచిత్రం.print();
    రాయి("## Result:\n");
    లేఖాచిత్రం.print();
  }
  hb_always_assert (లేఖాచిత్రం == expected_లేఖాచిత్రం);
}

స్థిర శూన్య కూడు_virtual_స్థానభ్రంశం (గుర్తులేని id,
                                hb_serialize_context_t* c)
{
  c->కూడు_virtual_link (id);
}

స్థిర శూన్య
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_1 = కూడు_object ("ghi", 3, c);
  గుర్తులేని obj_2 = కూడు_object ("def", 3, c);

  ప్రారంభం_object ("abc", 3, c);
  కూడు_స్థానభ్రంశం (obj_2, c);
  కూడు_స్థానభ్రంశం (obj_1, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_virtual (hb_serialize_context_t* c, తార్కిక with_అధికప్రవాహం)
{
  std::string large_string(50000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_4, obj_5;
  ఒకవేళ (with_అధికప్రవాహం) {
    obj_5 = కూడు_object("55555", 5, c);
    obj_4 = కూడు_object(large_string.c_str(), 50000, c);
  } లేకపోతే {
    obj_4 = కూడు_object(large_string.c_str(), 50000, c);
    obj_5 = కూడు_object("55555", 5, c);
  }

  ప్రారంభం_object(large_string.c_str(), 20000, c);
  కూడు_స్థానభ్రంశం(obj_5, c);
  గుర్తులేని obj_3 = c->తీయి_pack(అబద్ధం);

  ప్రారంభం_object("2", 2, c);
  కూడు_virtual_స్థానభ్రంశం(obj_5, c);
  గుర్తులేని obj_2 = c->తీయి_pack(అబద్ధం);

  // obj 1
  ప్రారంభం_object("1", 1, c);
  కూడు_స్థానభ్రంశం(obj_2, c);
  కూడు_స్థానభ్రంశం(obj_3, c);
  కూడు_స్థానభ్రంశం(obj_4, c);
  c->తీయి_pack(అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_అధికప్రవాహం (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_1 = కూడు_object (large_string.c_str(), 10000, c);
  గుర్తులేని obj_2 = కూడు_object (large_string.c_str(), 20000, c);
  గుర్తులేని obj_3 = కూడు_object (large_string.c_str(), 50000, c);

  ప్రారంభం_object ("abc", 3, c);
  కూడు_స్థానభ్రంశం (obj_3, c);
  కూడు_స్థానభ్రంశం (obj_2, c);
  కూడు_స్థానభ్రంశం (obj_1, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_ప్రాధాన్యత_అధికప్రవాహం (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_e = కూడు_object ("e", 1, c);
  గుర్తులేని obj_d = కూడు_object ("d", 1, c);

  ప్రారంభం_object (large_string.c_str (), 50000, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str (), 20000, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_స్థానభ్రంశం (obj_b, c);
  కూడు_స్థానభ్రంశం (obj_c, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_ప్రాధాన్యత_అధికప్రవాహం_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_e = కూడు_object ("e", 1, c);

  ప్రారంభం_object (large_string.c_str (), 50000, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  గుర్తులేని obj_d = కూడు_object ("d", 1, c);

  ప్రారంభం_object (large_string.c_str (), 20000, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_స్థానభ్రంశం (obj_b, c);
  కూడు_స్థానభ్రంశం (obj_c, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}


స్థిర శూన్య
populate_serializer_with_dedup_అధికప్రవాహం (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_1 = కూడు_object ("def", 3, c);

  ప్రారంభం_object (large_string.c_str(), 60000, c);
  కూడు_స్థానభ్రంశం (obj_1, c);
  గుర్తులేని obj_2 = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 10000, c);
  కూడు_స్థానభ్రంశం (obj_2, c);
  కూడు_స్థానభ్రంశం (obj_1, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_గుణిజం_dedup_అధికప్రవాహం (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని ఆకు = కూడు_object("def", 3, c);

  constexpr గుర్తులేని అంకె_mid_nodes = 20;
  గుర్తులేని mid_nodes[అంకె_mid_nodes];
  కొరకు (గుర్తులేని i = 0; i < అంకె_mid_nodes; i++) {
    ప్రారంభం_object(large_string.c_str(), 10000 + i, c);
    కూడు_స్థానభ్రంశం(ఆకు, c);
    mid_nodes[i] = c->తీయి_pack(అబద్ధం);
  }

  ప్రారంభం_object("abc", 3, c);
  కొరకు (గుర్తులేని i = 0; i < అంకె_mid_nodes; i++) {
    కూడు_wide_స్థానభ్రంశం(mid_nodes[i], c);
  }
  c->తీయి_pack(అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_isolation_అధికప్రవాహం (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_4 = కూడు_object ("4", 1, c);

  ప్రారంభం_object (large_string.c_str(), 60000, c);
  కూడు_స్థానభ్రంశం (obj_4, c);
  గుర్తులేని obj_3 = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 10000, c);
  కూడు_స్థానభ్రంశం (obj_4, c);
  గుర్తులేని obj_2 = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("1", 1, c);
  కూడు_wide_స్థానభ్రంశం (obj_3, c);
  కూడు_స్థానభ్రంశం (obj_2, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_isolation_అధికప్రవాహం_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_f = కూడు_object ("f", 1, c);

  ప్రారంభం_object ("e", 1, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("d", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 60000, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_h = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 60000, c);
  కూడు_స్థానభ్రంశం (obj_c, c);
  కూడు_స్థానభ్రంశం (obj_h, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 10000, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_g = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 11000, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_i = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_wide_స్థానభ్రంశం (obj_b, c);
  కూడు_స్థానభ్రంశం (obj_g, c);
  కూడు_స్థానభ్రంశం (obj_i, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_isolation_అధికప్రవాహం_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();


  // space 1

  గుర్తులేని obj_f_ప్రధానసంఖ్య = కూడు_object ("f", 1, c);

  ప్రారంభం_object ("e", 1, c);
  కూడు_స్థానభ్రంశం (obj_f_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_e_ప్రధానసంఖ్య = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("d", 1, c);
  కూడు_స్థానభ్రంశం (obj_e_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_d_ప్రధానసంఖ్య = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 60000, c);
  కూడు_స్థానభ్రంశం (obj_d_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_h = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_e_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 60000, c);
  కూడు_స్థానభ్రంశం (obj_c, c);
  కూడు_స్థానభ్రంశం (obj_h, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  // space 0

  గుర్తులేని obj_f = కూడు_object ("f", 1, c);

  ప్రారంభం_object ("e", 1, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);


  ప్రారంభం_object ("d", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 11000, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_i = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 10000, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_g = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_wide_స్థానభ్రంశం (obj_b, c);
  కూడు_స్థానభ్రంశం (obj_g, c);
  కూడు_స్థానభ్రంశం (obj_i, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_isolation_అధికప్రవాహం_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_d = కూడు_object ("f", 1, c);
  గుర్తులేని obj_e = కూడు_object ("f", 1, c);

  ప్రారంభం_object (large_string.c_str(), 60000, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_b = c->తీయి_pack ();

  ప్రారంభం_object (large_string.c_str(), 60000, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack ();


  ప్రారంభం_object ("a", 1, c);
  కూడు_wide_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_c, c);
  c->తీయి_pack ();

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_repack_చివరి (hb_serialize_context_t* c, తార్కిక with_అధికప్రవాహం)
{
  std::string large_string(70000, 'c');
  c->ప్రారంభం_serialize<అక్షరం> ();
  c->push();

  // Obj E
  గుర్తులేని obj_e_1, obj_e_2;
  ఒకవేళ (with_అధికప్రవాహం) {
    obj_e_1 = కూడు_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } లేకపోతే {
    obj_e_2 = కూడు_object("a", 1, c);
  }

  // Obj D
  c->push();
  కూడు_స్థానభ్రంశం(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  గుర్తులేని obj_d = c->తీయి_pack(అబద్ధం);

  కూడు_స్థానభ్రంశం(obj_d, c);
  hb_always_assert(c->చివరి_added_బిడ్డ_సూచీ() == obj_d);

  ఒకవేళ (!with_అధికప్రవాహం) {
    obj_e_1 = కూడు_object("a", 1, c);
  }

  // Obj C
  c->push();
  కూడు_స్థానభ్రంశం(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  గుర్తులేని obj_c = c->తీయి_pack(అబద్ధం);

  కూడు_స్థానభ్రంశం(obj_c, c);

  // Obj B
  గుర్తులేని obj_b = కూడు_object("b", 1, c);
  కూడు_స్థానభ్రంశం(obj_b, c);

  // Obj A
  c->repack_చివరి(obj_d);
  c->తీయి_pack(అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_spaces (hb_serialize_context_t* c, తార్కిక with_అధికప్రవాహం)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_i;

  ఒకవేళ (with_అధికప్రవాహం)
    obj_i = కూడు_object ("i", 1, c);

  // Space 2
  గుర్తులేని obj_h = కూడు_object ("h", 1, c);

  ప్రారంభం_object (large_string.c_str(), 30000, c);
  కూడు_స్థానభ్రంశం (obj_h, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  // Space 1
  ఒకవేళ (!with_అధికప్రవాహం)
    obj_i = కూడు_object ("i", 1, c);

  ప్రారంభం_object (large_string.c_str(), 30000, c);
  కూడు_స్థానభ్రంశం (obj_i, c);
  గుర్తులేని obj_g = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 30000, c);
  కూడు_స్థానభ్రంశం (obj_i, c);
  గుర్తులేని obj_f = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("d", 1, c);
  కూడు_స్థానభ్రంశం (obj_g, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_wide_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_c, c);
  కూడు_wide_స్థానభ్రంశం (obj_d, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_g = కూడు_object ("g", 1, c);
  గుర్తులేని obj_h = కూడు_object ("h", 1, c);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_g, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_h, c);
  గుర్తులేని obj_f = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("d", 1, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  కూడు_స్థానభ్రంశం (obj_h, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_c, c);
  కూడు_wide_స్థానభ్రంశం (obj_d, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_g_ప్రధానసంఖ్య = కూడు_object ("g", 1, c);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_g_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_e_ప్రధానసంఖ్య = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_e_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  గుర్తులేని obj_h_ప్రధానసంఖ్య = కూడు_object ("h", 1, c);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_h_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_f = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("d", 1, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  గుర్తులేని obj_g = కూడు_object ("g", 1, c);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_g, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  గుర్తులేని obj_h = కూడు_object ("h", 1, c);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  కూడు_స్థానభ్రంశం (obj_h, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_c, c);
  కూడు_wide_స్థానభ్రంశం (obj_d, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize ();
}

స్థిర శూన్య
populate_serializer_short_and_wide_subgraph_మూలం (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_e = కూడు_object ("e", 1, c);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_c, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_c, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_c, c);
  కూడు_wide_స్థానభ్రంశం (obj_d, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_short_and_wide_subgraph_మూలం_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_e_ప్రధానసంఖ్య = కూడు_object ("e", 1, c);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_e_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_c_ప్రధానసంఖ్య = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_c_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  గుర్తులేని obj_e = కూడు_object ("e", 1, c);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);


  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_c, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_c_ప్రధానసంఖ్య, c);
  కూడు_wide_స్థానభ్రంశం (obj_d, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_విభజించు_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_f = కూడు_object ("f", 1, c);

  ప్రారంభం_object (large_string.c_str(), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_wide_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_c, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_విభజించు_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_f = కూడు_object ("f", 1, c);

  ప్రారంభం_object (large_string.c_str(), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str(), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_c, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_విభజించు_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_f_ప్రధానసంఖ్య = కూడు_object ("f", 1, c);

  ప్రారంభం_object (large_string.c_str(), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  గుర్తులేని obj_f = కూడు_object ("f", 1, c);

  ప్రారంభం_object (large_string.c_str(), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_wide_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_c, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_విభజించు_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  // Space 2

  గుర్తులేని obj_f_double_ప్రధానసంఖ్య = కూడు_object ("f", 1, c);

  ప్రారంభం_object (large_string.c_str(), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f_double_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_d_ప్రధానసంఖ్య = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_d_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_b_ప్రధానసంఖ్య = c->తీయి_pack (అబద్ధం);

  // Space 1

  గుర్తులేని obj_f_ప్రధానసంఖ్య = కూడు_object ("f", 1, c);

  ప్రారంభం_object (large_string.c_str(), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f_ప్రధానసంఖ్య, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  // Space 0

  గుర్తులేని obj_f = కూడు_object ("f", 1, c);

  ప్రారంభం_object (large_string.c_str(), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  //మూలం
  ప్రారంభం_object ("a", 1, c);
  కూడు_స్థానభ్రంశం (obj_b, c);
  కూడు_wide_స్థానభ్రంశం (obj_b_ప్రధానసంఖ్య, c);
  కూడు_wide_స్థానభ్రంశం (obj_c, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_5 = కూడు_object ("mn", 2, c);

  గుర్తులేని obj_4 = కూడు_object ("jkl", 3, c);

  ప్రారంభం_object ("ghi", 3, c);
  కూడు_స్థానభ్రంశం (obj_4, c);
  గుర్తులేని obj_3 = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("def", 3, c);
  కూడు_స్థానభ్రంశం (obj_3, c);
  గుర్తులేని obj_2 = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("abc", 3, c);
  కూడు_స్థానభ్రంశం (obj_2, c);
  కూడు_స్థానభ్రంశం (obj_4, c);
  కూడు_స్థానభ్రంశం (obj_5, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_6 = కూడు_object ("opqrst", 6, c);

  గుర్తులేని obj_5 = కూడు_object ("mn", 2, c);

  ప్రారంభం_object ("jkl", 3, c);
  కూడు_స్థానభ్రంశం (obj_6, c);
  గుర్తులేని obj_4 = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("ghi", 3, c);
  కూడు_స్థానభ్రంశం (obj_4, c);
  గుర్తులేని obj_3 = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("def", 3, c);
  కూడు_స్థానభ్రంశం (obj_3, c);
  గుర్తులేని obj_2 = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("abc", 3, c);
  కూడు_స్థానభ్రంశం (obj_2, c);
  కూడు_స్థానభ్రంశం (obj_4, c);
  కూడు_స్థానభ్రంశం (obj_5, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_d = కూడు_object ("d", 1, c);

  ప్రారంభం_object ("b", 1, c);
  కూడు_స్థానభ్రంశం (obj_d, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("e", 1, c);
  కూడు_virtual_స్థానభ్రంశం (obj_b, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("c", 1, c);
  కూడు_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_స్థానభ్రంశం (obj_b, c);
  కూడు_స్థానభ్రంశం (obj_c, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_24_and_32_బిట్టు_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_f = కూడు_object ("f", 1, c);
  గుర్తులేని obj_g = కూడు_object ("g", 1, c);
  గుర్తులేని obj_j = కూడు_object ("j", 1, c);
  గుర్తులేని obj_k = కూడు_object ("k", 1, c);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_f, c);
  గుర్తులేని obj_c = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_g, c);
  గుర్తులేని obj_d = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_j, c);
  గుర్తులేని obj_h = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object (large_string.c_str (), 40000, c);
  కూడు_స్థానభ్రంశం (obj_k, c);
  గుర్తులేని obj_i = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("e", 1, c);
  కూడు_wide_స్థానభ్రంశం (obj_h, c);
  కూడు_wide_స్థానభ్రంశం (obj_i, c);
  గుర్తులేని obj_e = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("b", 1, c);
  కూడు_24_స్థానభ్రంశం (obj_c, c);
  కూడు_24_స్థానభ్రంశం (obj_d, c);
  కూడు_24_స్థానభ్రంశం (obj_e, c);
  గుర్తులేని obj_b = c->తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("a", 1, c);
  కూడు_24_స్థానభ్రంశం (obj_b, c);
  c->తీయి_pack (అబద్ధం);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              పూర్ణాంకం అంకె_extensions = 0,
                                              తార్కిక shared_subtables = అబద్ధం,
                                              తార్కిక సంస్కరణ_1_2 = అబద్ధం)
{
  constexpr పూర్ణాంకం అంకె_lookups = 5;
  constexpr పూర్ణాంకం అంకె_subtables = అంకె_lookups * 2;
  గుర్తులేని పూర్ణాంకం lookups[అంకె_lookups];
  గుర్తులేని పూర్ణాంకం subtables[అంకె_subtables];
  గుర్తులేని పూర్ణాంకం extensions[అంకె_subtables];

  std::string large_string(60000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();


  కొరకు (పూర్ణాంకం i = అంకె_subtables - 1; i >= 0; i--)
    subtables[i] = కూడు_object(large_string.c_str (), 15000 + i, c);

  కొరకు (పూర్ణాంకం i = అంకె_subtables - 1;
       i >= (అంకె_lookups - అంకె_extensions) * 2;
       i--)
  {
    extensions[i] = కూడు_extension (subtables[i], 5, c);
  }

  కొరకు (పూర్ణాంకం i = అంకె_lookups - 1; i >= 0; i--)
  {
    తార్కిక is_ext = (i >= (అంకె_lookups - అంకె_extensions));

    ప్రారంభం_lookup (is_ext ? (అక్షరం) 7 : (అక్షరం) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    ఒకవేళ (is_ext) {
      ఒకవేళ (shared_subtables && i > 2) {
        కూడు_స్థానభ్రంశం (extensions[i * 2 - 1], c);
      }
      కూడు_స్థానభ్రంశం (extensions[i * 2], c);
      కూడు_స్థానభ్రంశం (extensions[i * 2 + 1], c);
    } లేకపోతే {
      ఒకవేళ (shared_subtables && i > 2) {
        కూడు_స్థానభ్రంశం (subtables[i * 2 - 1], c);
      }
      కూడు_స్థానభ్రంశం (subtables[i * 2], c);
      కూడు_స్థానభ్రంశం (subtables[i * 2 + 1], c);
    }

    lookups[i] = ముగింపు_lookup (c);
  }

  గుర్తులేని lookup_list = సంస్కరణ_1_2
                         ? కూడు_lookup_list_1_2 (lookups, అంకె_lookups, c)
                         : కూడు_lookup_list (lookups, అంకె_lookups, c);

  ఒకవేళ (సంస్కరణ_1_2)
    కూడు_gsubgpos_శీర్షిక_1_2 (lookup_list, c);
  లేకపోతే
    కూడు_gsubgpos_శీర్షిక (lookup_list, c);

  c->చివర_serialize();
}

మూస<పూర్ణాంకం అంకె_pair_pos_1, పూర్ణాంకం అంకె_pair_సమితి>
స్థిర శూన్య
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           తార్కిక as_extension = అబద్ధం)
{
  std::string large_string(60000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  constexpr పూర్ణాంకం మొత్తం_pair_సమితి = అంకె_pair_pos_1 * అంకె_pair_సమితి;
  గుర్తులేని pair_సమితి_2[మొత్తం_pair_సమితి];
  గుర్తులేని coverage[అంకె_pair_pos_1];
  గుర్తులేని pair_pos_1[అంకె_pair_pos_1];

  కొరకు (పూర్ణాంకం i = అంకె_pair_pos_1 - 1; i >= 0; i--)
  {
    కొరకు (పూర్ణాంకం j = (i + 1) * అంకె_pair_సమితి - 1; j >= i * అంకె_pair_సమితి; j--)
      pair_సమితి_2[j] = కూడు_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = కూడు_coverage (i * అంకె_pair_సమితి,
                                (i + 1) * అంకె_pair_సమితి - 1, c);

    pair_pos_1[i] = కూడు_pair_pos_1 (&pair_సమితి_2[i * అంకె_pair_సమితి],
                                    అంకె_pair_సమితి,
                                    coverage[i],
                                    c);
  }

  గుర్తులేని pair_pos_2 = కూడు_object (large_string.c_str(), 200, c);

  ఒకవేళ (as_extension) {
    pair_pos_2 = కూడు_extension (pair_pos_2, 2, c);
    కొరకు (పూర్ణాంకం i = అంకె_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = కూడు_extension (pair_pos_1[i], 2, c);
  }

  ప్రారంభం_lookup (as_extension ? 9 : 2, 1 + అంకె_pair_pos_1, c);

  కొరకు (పూర్ణాంకం i = 0; i < అంకె_pair_pos_1; i++)
    కూడు_స్థానభ్రంశం (pair_pos_1[i], c);
  కూడు_స్థానభ్రంశం (pair_pos_2, c);

  గుర్తులేని lookup = ముగింపు_lookup (c);

  గుర్తులేని lookup_list = కూడు_lookup_list (&lookup, 1, c);

  కూడు_gsubgpos_శీర్షిక (lookup_list, c);

  c->చివర_serialize();
}

మూస<పూర్ణాంకం అంకె_pair_pos_2, పూర్ణాంకం అంకె_class_1, పూర్ణాంకం అంకె_class_2>
స్థిర శూన్య
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           తార్కిక as_extension = అబద్ధం,
                                           తార్కిక with_device_tables = అబద్ధం,
                                           తార్కిక extra_పట్టిక = నిజం)
{
  std::string large_string(100000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని coverage[అంకె_pair_pos_2];
  గుర్తులేని class_def_1[అంకె_pair_pos_2];
  గుర్తులేని class_def_2[అంకె_pair_pos_2];
  గుర్తులేని pair_pos_2[అంకె_pair_pos_2];

  గుర్తులేని* device_tables = (గుర్తులేని*) సున్నా_జ్ఞాపకం_ఇవ్వు (అంకె_pair_pos_2 * అంకె_class_1 * అంకె_class_2,
                                                పరిమాణం(గుర్తులేని));

  // Total glyphs = num_class_1 * num_pair_pos_2
  కొరకు (పూర్ణాంకం i = అంకె_pair_pos_2 - 1; i >= 0; i--)
  {
    గుర్తులేని ప్రారంభం_glyph = 5 + i * అంకె_class_1;
    ఒకవేళ (అంకె_class_2 >= అంకె_class_1)
    {
      class_def_2[i] = కూడు_class_def (11,
                                      10 + అంకె_class_2, c);
      class_def_1[i] = కూడు_class_def (ప్రారంభం_glyph + 1,
                                      ప్రారంభం_glyph + అంకె_class_1,
                                      c);
    } లేకపోతే {
      class_def_1[i] = కూడు_class_def (ప్రారంభం_glyph + 1,
                                      ప్రారంభం_glyph + అంకె_class_1,
                                      c);
      class_def_2[i] = కూడు_class_def (11,
                                      10 + అంకె_class_2, c);
    }

    coverage[i] = కూడు_coverage (ప్రారంభం_glyph,
                                ప్రారంభం_glyph + అంకె_class_1 - 1,
                                c);

    ఒకవేళ (with_device_tables)
    {
      కొరకు(పూర్ణాంకం j = (i + 1) * అంకె_class_1 * అంకె_class_2 - 1;
          j >= i * అంకె_class_1 * అంకె_class_2;
          j--)
      {
        uint8_t పట్టిక[] = {
          (uint8_t) ((j >> 8) & 0xFF),
          (uint8_t) (j & 0xFF),
        };
        device_tables[j] = కూడు_object ((అక్షరం*) పట్టిక, 2, c);
      }
    }

    pair_pos_2[i] = కూడు_pair_pos_2 (1 + i * అంకె_class_1,
                                    coverage[i],
                                    class_def_1[i], అంకె_class_1,
                                    class_def_2[i], అంకె_class_2,
                                    with_device_tables
                                    ? &device_tables[i * అంకె_class_1 * అంకె_class_2]
                                    : శూన్యసూచిక,
                                    c);
  }


  గుర్తులేని pair_pos_1 = 0;
  ఒకవేళ (extra_పట్టిక) pair_pos_1 = కూడు_object (large_string.c_str(), 100000, c);

  ఒకవేళ (as_extension) {
    కొరకు (పూర్ణాంకం i = అంకె_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = కూడు_extension (pair_pos_2[i], 2, c);

    ఒకవేళ (extra_పట్టిక)
      pair_pos_1 = కూడు_extension (pair_pos_1, 2, c);
  }

  ప్రారంభం_lookup (as_extension ? 9 : 2, 1 + అంకె_pair_pos_2, c);

  ఒకవేళ (extra_పట్టిక)
    కూడు_స్థానభ్రంశం (pair_pos_1, c);

  కొరకు (పూర్ణాంకం i = 0; i < అంకె_pair_pos_2; i++)
    కూడు_స్థానభ్రంశం (pair_pos_2[i], c);

  గుర్తులేని lookup = ముగింపు_lookup (c);

  గుర్తులేని lookup_list = కూడు_lookup_list (&lookup, 1, c);

  కూడు_gsubgpos_శీర్షిక (lookup_list, c);

  c->చివర_serialize();

  విడుదల (device_tables);
}

మూస<పూర్ణాంకం mark_లెక్క,
    పూర్ణాంకం class_లెక్క,
    పూర్ణాంకం పునాది_లెక్క,
    పూర్ణాంకం పట్టిక_లెక్క_2>
స్థిర శూన్య
populate_serializer_with_large_mark_పునాది_pos_1 (hb_serialize_context_t* c)
{
  c->ప్రారంభం_serialize<అక్షరం> ();

  Mark_పునాది_Pos_Buffers<mark_లెక్క, class_లెక్క, పునాది_లెక్క, పట్టిక_లెక్క_2> buffers (c);

  గుర్తులేని mark_పునాది_pos[పట్టిక_లెక్క_2];
  కొరకు (గుర్తులేని i = 0; i < పట్టిక_లెక్క_2; i++)
    mark_పునాది_pos[i] = buffers.create_mark_పునాది_pos_1 (i, c);

  కొరకు (పూర్ణాంకం i = 0; i < పట్టిక_లెక్క_2; i++)
    mark_పునాది_pos[i] = కూడు_extension (mark_పునాది_pos[i], 4, c);

  ప్రారంభం_lookup (9, పట్టిక_లెక్క_2, c);

  కొరకు (పూర్ణాంకం i = 0; i < పట్టిక_లెక్క_2; i++)
    కూడు_స్థానభ్రంశం (mark_పునాది_pos[i], c);

  గుర్తులేని lookup = ముగింపు_lookup (c);

  గుర్తులేని lookup_list = కూడు_lookup_list (&lookup, 1, c);

  కూడు_gsubgpos_శీర్షిక (lookup_list, c);

  c->చివర_serialize();
}

మూస<గుర్తులేని Lig_Subst_లెక్క,
         గుర్తులేని Liga_సమితి_లెక్క,
         గుర్తులేని Liga_చొప్పున_సమితి_లెక్క,
         గుర్తులేని Liga_కొలత>
స్థిర శూన్య populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    తార్కిక sequential_liga_sets,
                                                    తార్కిక shared,
                                                    గుర్తులేని *liga_subst_id,
                                                    తార్కిక unique_lig_str = అబద్ధం)
{
  అక్షరం ch = 'a';

  గుర్తులేని liga[Liga_సమితి_లెక్క * Liga_చొప్పున_సమితి_లెక్క];
  గుర్తులేని liga_సమితి[Liga_సమితి_లెక్క];
  కొరకు (గుర్తులేని l = 0; l < Lig_Subst_లెక్క; l++) {
    గుర్తులేని coverage_ప్రారంభం = 0;
    గుర్తులేని coverage_చివర = Liga_సమితి_లెక్క - 1;
    ఒకవేళ (sequential_liga_sets) {
      coverage_ప్రారంభం = l * Liga_సమితి_లెక్క;
      coverage_చివర = (l + 1) * Liga_సమితి_లెక్క - 1;
    }
    గుర్తులేని coverage = కూడు_coverage(coverage_ప్రారంభం, coverage_చివర, c, shared);

    కొరకు (గుర్తులేని i = 0; i < Liga_సమితి_లెక్క; i++) {
      కొరకు (గుర్తులేని j = 0; j < Liga_చొప్పున_సమితి_లెక్క; j++)
      {
        std::string large_string(100000, ch);
        ప్రారంభం_object (large_string.c_str(), Liga_కొలత, c);
        ఒకవేళ (unique_lig_str)
          ch += 1;
        కూడు_virtual_స్థానభ్రంశం(coverage, c);
        liga[i * Liga_చొప్పున_సమితి_లెక్క + j] = c->తీయి_pack (shared);
      }

      కూడు_liga_సమితి_శీర్షిక(Liga_చొప్పున_సమితి_లెక్క, c);
      కూడు_virtual_స్థానభ్రంశం(coverage, c);
      కొరకు (గుర్తులేని j = 0; j < Liga_చొప్పున_సమితి_లెక్క; j++)
      {
        కూడు_స్థానభ్రంశం(liga[i * Liga_చొప్పున_సమితి_లెక్క + j], c);
      }

      liga_సమితి[i] = c->తీయి_pack(shared);
    }

    కూడు_liga_శీర్షిక(coverage, Liga_సమితి_లెక్క, c);
    కొరకు (గుర్తులేని i = 0; i < Liga_సమితి_లెక్క; i++)
    {
      కూడు_స్థానభ్రంశం(liga_సమితి[i], c);
    }

    liga_subst_id[l] = c->తీయి_pack(shared);
  }

  కొరకు (గుర్తులేని l = 0; l < Lig_Subst_లెక్క; l++) {
    liga_subst_id[l] = కూడు_extension(liga_subst_id[l], 4, c);
  }
}

మూస<గుర్తులేని liga_subst_లెక్క,
         గుర్తులేని liga_సమితి_లెక్క,
         గుర్తులేని liga_చొప్పున_సమితి_లెక్క,
         గుర్తులేని liga_కొలత>
స్థిర శూన్య
populate_serializer_with_large_liga (hb_serialize_context_t* c, తార్కిక sequential_liga_sets)
{
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని liga_subst[liga_subst_లెక్క];
  populate_serializer_with_large_ligsubst<liga_subst_లెక్క, liga_సమితి_లెక్క, liga_చొప్పున_సమితి_లెక్క, liga_కొలత>(c, sequential_liga_sets, అబద్ధం, liga_subst);

  ప్రారంభం_lookup (7, liga_subst_లెక్క, c);
  కొరకు (గుర్తులేని l = 0; l < liga_subst_లెక్క; l++) {
    కూడు_స్థానభ్రంశం(liga_subst[l], c);
  }

  గుర్తులేని lookup = ముగింపు_lookup (c);

  గుర్తులేని lookup_list = కూడు_lookup_list (&lookup, 1, c);

  కూడు_gsubgpos_శీర్షిక (lookup_list, c);

  c->చివర_serialize();
}

స్థిర శూన్య
populate_serializer_with_large_liga_overlapping_clone_ఫలితం (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->ప్రారంభం_serialize<అక్షరం> ();

  constexpr గుర్తులేని liga_కొలత = 30000;

  గుర్తులేని liga[6];
  గుర్తులేని liga_subst[3];
  గుర్తులేని liga_సమితి[2];

  // LigaSubst 3
  గుర్తులేని coverage = కూడు_coverage(1, 1, c);
  కొరకు (పూర్ణాంకం i = 1; i >= 0; i--) {
    ప్రారంభం_object (large_string.c_str(), liga_కొలత, c);
    కూడు_virtual_స్థానభ్రంశం(coverage, c);
    liga[i] = c->తీయి_pack (అబద్ధం);
  }

  కూడు_liga_సమితి_శీర్షిక(2, c);
  కూడు_virtual_స్థానభ్రంశం(coverage, c);
  కొరకు (గుర్తులేని i = 0; i < 2; i++)
    కూడు_స్థానభ్రంశం(liga[i], c);
  liga_సమితి[0] = c->తీయి_pack(అబద్ధం);

  కూడు_liga_శీర్షిక(coverage, 1, c);
  కూడు_స్థానభ్రంశం(liga_సమితి[0], c);
  liga_subst[2] = c->తీయి_pack(అబద్ధం);

  // LigaSubst 2
  coverage = కూడు_coverage(0, 1, c);
  కొరకు (పూర్ణాంకం i = 1; i >= 0; i--) {
    ప్రారంభం_object (large_string.c_str(), liga_కొలత, c);
    కూడు_virtual_స్థానభ్రంశం(coverage, c);
    liga[i] = c->తీయి_pack (అబద్ధం);
  }

  కూడు_liga_సమితి_శీర్షిక(1, c);
  కూడు_virtual_స్థానభ్రంశం(coverage, c);
  కూడు_స్థానభ్రంశం(liga[1], c);
  liga_సమితి[1] = c->తీయి_pack(అబద్ధం);

  కూడు_liga_సమితి_శీర్షిక(1, c);
  కూడు_virtual_స్థానభ్రంశం(coverage, c);
  కూడు_స్థానభ్రంశం(liga[0], c);
  liga_సమితి[0] = c->తీయి_pack(అబద్ధం);

  కూడు_liga_శీర్షిక(coverage, 2, c);
  కూడు_స్థానభ్రంశం(liga_సమితి[0], c);
  కూడు_స్థానభ్రంశం(liga_సమితి[1], c);
  liga_subst[1] = c->తీయి_pack(అబద్ధం);

  // LigaSubst 1
  coverage = కూడు_coverage(0, 0, c);
  కొరకు (పూర్ణాంకం i = 1; i >= 0; i--) {
    ప్రారంభం_object (large_string.c_str(), liga_కొలత, c);
    కూడు_virtual_స్థానభ్రంశం(coverage, c);
    liga[i] = c->తీయి_pack (అబద్ధం);
  }

  కూడు_liga_సమితి_శీర్షిక(2, c);
  కూడు_virtual_స్థానభ్రంశం(coverage, c);
  కొరకు (గుర్తులేని i = 0; i < 2; i++)
    కూడు_స్థానభ్రంశం(liga[i], c);
  liga_సమితి[0] = c->తీయి_pack(అబద్ధం);

  కూడు_liga_శీర్షిక(coverage, 1, c);
  కూడు_స్థానభ్రంశం(liga_సమితి[0], c);
  liga_subst[0] = c->తీయి_pack(అబద్ధం);

  కొరకు (పూర్ణాంకం l = 2; l >= 0; l--) {
    liga_subst[l] = కూడు_extension(liga_subst[l], 4, c);
  }

  ప్రారంభం_lookup (7, 3, c);
  కొరకు (గుర్తులేని l = 0; l < 3; l++) {
    కూడు_స్థానభ్రంశం(liga_subst[l], c);
  }

  గుర్తులేని lookup = ముగింపు_lookup (c);

  గుర్తులేని lookup_list = కూడు_lookup_list (&lookup, 1, c);

  కూడు_gsubgpos_శీర్షిక (lookup_list, c);

  c->చివర_serialize();
}

మూస<గుర్తులేని liga_subst_లెక్క,
         గుర్తులేని liga_సమితి_లెక్క,
         గుర్తులేని liga_చొప్పున_సమితి_లెక్క,
         గుర్తులేని liga_కొలత>
స్థిర శూన్య
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, తార్కిక sequential_liga_sets)
{
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  గుర్తులేని liga_subst_idx[liga_subst_లెక్క];
  populate_serializer_with_large_ligsubst<liga_subst_లెక్క, liga_సమితి_లెక్క, liga_చొప్పున_సమితి_లెక్క, liga_కొలత>(c, sequential_liga_sets, నిజం, liga_subst_idx, నిజం);

  ప్రారంభం_lookup (7, liga_subst_లెక్క, c);
  కొరకు (గుర్తులేని l = 0; l < liga_subst_లెక్క; l++) {
    కూడు_స్థానభ్రంశం(liga_subst_idx[l], c);
  }

  lookups[0] = ముగింపు_lookup (c);

  // Lookup with 2 LigSubst tables
  గుర్తులేని liga_subst[liga_subst_లెక్క + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, అబద్ధం, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_లెక్క, liga_సమితి_లెక్క, liga_చొప్పున_సమితి_లెక్క, liga_కొలత>(c, sequential_liga_sets, నిజం, &liga_subst[1], నిజం);

  ప్రారంభం_lookup (7, liga_subst_లెక్క + 1, c);
  కొరకు (గుర్తులేని l = 0; l <= liga_subst_లెక్క; l++) {
    కూడు_స్థానభ్రంశం(liga_subst[l], c);
  }

  lookups[1] = ముగింపు_lookup (c);

  గుర్తులేని lookup_list = కూడు_lookup_list (lookups, 2, c);
  కూడు_gsubgpos_శీర్షిక (lookup_list, c);

  c->చివర_serialize();
}

మూస<గుర్తులేని liga_subst_లెక్క,
         గుర్తులేని liga_సమితి_లెక్క,
         గుర్తులేని liga_చొప్పున_సమితి_లెక్క,
         గుర్తులేని liga_కొలత>
స్థిర శూన్య
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని liga_subst[liga_subst_లెక్క + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, నిజం, నిజం, liga_subst, నిజం);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_లెక్క, liga_సమితి_లెక్క, liga_చొప్పున_సమితి_లెక్క, liga_కొలత>(c, నిజం, నిజం, &liga_subst[1], నిజం);

  ప్రారంభం_lookup (7, liga_subst_లెక్క + 1, c);
  కొరకు (గుర్తులేని l = 0; l <= liga_subst_లెక్క; l++) {
    కూడు_స్థానభ్రంశం(liga_subst[l], c);
  }

  గుర్తులేని lookup = ముగింపు_lookup (c);

  గుర్తులేని lookup_list = కూడు_lookup_list (&lookup, 1, c);
  కూడు_gsubgpos_శీర్షిక (lookup_list, c);

  c->చివర_serialize();
}

స్థిర శూన్య test_క్రమీకరించు_అతితక్కువ ()
{
  size_t బఫరు_కొలత = 100;
  శూన్య* బఫరు_a = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  శూన్య* బఫరు_e = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t a (బఫరు_a, బఫరు_కొలత);
  hb_serialize_context_t e (బఫరు_e, బఫరు_కొలత);
  populate_serializer_complex_2 (&a);

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (a.object_లేఖాచిత్రం ()).value ();
  hb_always_assert (లేఖాచిత్రం.క్రమీకరించు_అతితక్కువ_దూరం ().is_ok());
  లేఖాచిత్రం.normalize();

  // Expected graph
  e.ప్రారంభం_serialize();
  గుర్తులేని jkl = కూడు_object ("jkl", 3, &e);

  ప్రారంభం_object("ghi", 3, &e);
  కూడు_స్థానభ్రంశం(jkl, &e);
  గుర్తులేని ghi = e.తీయి_pack(అబద్ధం);

  ప్రారంభం_object("def", 3, &e);
  కూడు_స్థానభ్రంశం(ghi, &e);
  గుర్తులేని def = e.తీయి_pack(అబద్ధం);

  గుర్తులేని mn = కూడు_object("mn", 2, &e);

  ప్రారంభం_object ("abc", 3, &e);
  కూడు_స్థానభ్రంశం (def, &e);
  కూడు_స్థానభ్రంశం (jkl, &e);
  కూడు_స్థానభ్రంశం (mn, &e);
  e.తీయి_pack (అబద్ధం);
  e.చివర_serialize();

  లేఖాచిత్రం_t expected = లేఖాచిత్రం_t::create (e.object_లేఖాచిత్రం ()).value ();
  expected.normalize();

  ప్రతిజ్ఞా(expected == లేఖాచిత్రం);

  విడుదల (బఫరు_a);
  విడుదల (బఫరు_e);
}

స్థిర శూన్య test_duplicate_ఆకు ()
{
  size_t బఫరు_కొలత = 100;
  శూన్య* బఫరు_a = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  శూన్య* బఫరు_e = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t a (బఫరు_a, బఫరు_కొలత);
  hb_serialize_context_t e (బఫరు_e, బఫరు_కొలత);
  populate_serializer_complex_2 (&a);

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (a.object_లేఖాచిత్రం ()).value ();
  hb_always_assert (లేఖాచిత్రం.duplicate (4, 1, అబద్ధం).is_ok ());
  లేఖాచిత్రం.normalize();

  e.ప్రారంభం_serialize();
  గుర్తులేని mn = కూడు_object("mn", 2, &e);
  గుర్తులేని jkl_2 = కూడు_object("jkl", 3, &e);

  ప్రారంభం_object("ghi", 3, &e);
  కూడు_స్థానభ్రంశం(jkl_2, &e);
  గుర్తులేని ghi = e.తీయి_pack(అబద్ధం);

  ప్రారంభం_object("def", 3, &e);
  కూడు_స్థానభ్రంశం(ghi, &e);
  గుర్తులేని def = e.తీయి_pack(అబద్ధం);

  గుర్తులేని jkl_1 = కూడు_object("jkl", 3, &e);

  ప్రారంభం_object("abc", 3, &e);
  కూడు_స్థానభ్రంశం(def, &e);
  కూడు_స్థానభ్రంశం(jkl_1, &e);
  కూడు_స్థానభ్రంశం(mn, &e);
  e.తీయి_pack(అబద్ధం);

  లేఖాచిత్రం_t expected = లేఖాచిత్రం_t::create (e.object_లేఖాచిత్రం ()).value ();
  expected.normalize();

  ప్రతిజ్ఞా(expected == లేఖాచిత్రం);

  విడుదల (బఫరు_a);
  విడుదల (బఫరు_e);
}

స్థిర శూన్య test_duplicate_interior ()
{
  size_t బఫరు_కొలత = 100;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_complex_3 (&c);

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();
  hb_always_assert (లేఖాచిత్రం.duplicate (3, 2, అబద్ధం).is_ok ());

  hb_always_assert(అక్షరశ్రేణి_పోలిక (లేఖాచిత్రం.object (6).తల_2, "jkl", 3) == 0);
  hb_always_assert(లేఖాచిత్రం.object (6).real_links.నిడివి == 1);
  hb_always_assert(లేఖాచిత్రం.object (6).real_links[0].objidx == 0);

  hb_always_assert(అక్షరశ్రేణి_పోలిక (లేఖాచిత్రం.object (5).తల_2, "abc", 3) == 0);
  hb_always_assert(లేఖాచిత్రం.object (5).real_links.నిడివి == 3);
  hb_always_assert(లేఖాచిత్రం.object (5).real_links[0].objidx == 4);
  hb_always_assert(లేఖాచిత్రం.object (5).real_links[1].objidx == 2);
  hb_always_assert(లేఖాచిత్రం.object (5).real_links[2].objidx == 1);

  hb_always_assert(అక్షరశ్రేణి_పోలిక (లేఖాచిత్రం.object (4).తల_2, "def", 3) == 0);
  hb_always_assert(లేఖాచిత్రం.object (4).real_links.నిడివి == 1);
  hb_always_assert(లేఖాచిత్రం.object (4).real_links[0].objidx == 3);

  hb_always_assert(అక్షరశ్రేణి_పోలిక (లేఖాచిత్రం.object (3).తల_2, "ghi", 3) == 0);
  hb_always_assert(లేఖాచిత్రం.object (3).real_links.నిడివి == 1);
  hb_always_assert(లేఖాచిత్రం.object (3).real_links[0].objidx == 6);

  hb_always_assert(అక్షరశ్రేణి_పోలిక (లేఖాచిత్రం.object (2).తల_2, "jkl", 3) == 0);
  hb_always_assert(లేఖాచిత్రం.object (2).real_links.నిడివి == 1);
  hb_always_assert(లేఖాచిత్రం.object (2).real_links[0].objidx == 0);

  hb_always_assert(అక్షరశ్రేణి_పోలిక (లేఖాచిత్రం.object (1).తల_2, "mn", 2) == 0);
  hb_always_assert(లేఖాచిత్రం.object (1).real_links.నిడివి == 0);

  hb_always_assert(అక్షరశ్రేణి_పోలిక (లేఖాచిత్రం.object (0).తల_2, "opqrst", 6) == 0);
  hb_always_assert(లేఖాచిత్రం.object (0).real_links.నిడివి == 0);

  విడుదల (బఫరు);
}

స్థిర శూన్య
test_serialize ()
{
  size_t బఫరు_కొలత = 100;
  శూన్య* బఫరు_1 = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c1 (బఫరు_1, బఫరు_కొలత);
  populate_serializer_simple (&c1);
  hb_బైట్లు_t expected = c1.copy_బైట్లు ();

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c1.object_లేఖాచిత్రం ()).value ();
  hb_blob_t* బహిర్గతం = లేఖాచిత్రం::serialize (లేఖాచిత్రం).value ();
  విడుదల (బఫరు_1);

  hb_బైట్లు_t actual = బహిర్గతం->as_బైట్లు ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (బహిర్గతం);
}

స్థిర శూన్య test_will_అధికప్రవాహం_1 ()
{
  size_t బఫరు_కొలత = 100;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);

  populate_serializer_complex_2 (&c);
  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_always_assert (!లేఖాచిత్రం::will_అధికప్రవాహం (లేఖాచిత్రం, శూన్యసూచిక).value ());

  విడుదల (బఫరు);
}

స్థిర శూన్య test_will_అధికప్రవాహం_2 ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_అధికప్రవాహం (&c);
  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_always_assert (లేఖాచిత్రం::will_అధికప్రవాహం (లేఖాచిత్రం, శూన్యసూచిక).value ());

  విడుదల (బఫరు);
}

స్థిర శూన్య test_will_అధికప్రవాహం_3 ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_dedup_అధికప్రవాహం (&c);
  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_always_assert (లేఖాచిత్రం::will_అధికప్రవాహం (లేఖాచిత్రం, శూన్యసూచిక).value ());

  విడుదల (బఫరు);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_క్రమీకరించు ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_అధికప్రవాహం (&c);
  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_blob_t* బహిర్గతం = hb_resolve_overflows (c.object_లేఖాచిత్రం (), HB_ట్యాగ్_NONE);
  hb_always_assert (బహిర్గతం);
  hb_బైట్లు_t ఫలితం = బహిర్గతం->as_బైట్లు ();
  hb_always_assert (ఫలితం.నిడివి == (80000 + 3 + 3 * 2));

  విడుదల (బఫరు);
  hb_blob_destroy (బహిర్గతం);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_duplication ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_dedup_అధికప్రవాహం (&c);
  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_blob_t* బహిర్గతం = hb_resolve_overflows (c.object_లేఖాచిత్రం (), HB_ట్యాగ్_NONE);
  hb_always_assert (బహిర్గతం);
  hb_బైట్లు_t ఫలితం = బహిర్గతం->as_బైట్లు ();
  hb_always_assert (ఫలితం.నిడివి == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  విడుదల (బఫరు);
  hb_blob_destroy (బహిర్గతం);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_గుణిజం_duplications ()
{
  size_t బఫరు_కొలత = 300000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_గుణిజం_dedup_అధికప్రవాహం (&c);
  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_blob_t* బహిర్గతం = hb_resolve_overflows (c.object_లేఖాచిత్రం (), HB_ట్యాగ్_NONE, 5);
  hb_always_assert (బహిర్గతం);

  విడుదల (బఫరు);
  hb_blob_destroy (బహిర్గతం);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_space_assignment ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_spaces (&c, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_spaces (&e, అబద్ధం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_isolation ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_isolation_అధికప్రవాహం (&c);
  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_always_assert (c.స్థానభ్రంశం_అధికప్రవాహం ());
  hb_blob_t* బహిర్గతం = hb_resolve_overflows (c.object_లేఖాచిత్రం (), HB_ట్యాగ్ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (బహిర్గతం);
  hb_బైట్లు_t ఫలితం = బహిర్గతం->as_బైట్లు ();
  hb_always_assert (ఫలితం.నిడివి == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  విడుదల (బఫరు);
  hb_blob_destroy (బహిర్గతం);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_isolation_with_recursive_duplication ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_isolation_అధికప్రవాహం_complex (&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_isolation_అధికప్రవాహం_complex_expected (&e);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_isolating_16bit_space ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_spaces_16bit_connection (&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_spaces_16bit_connection_expected (&e);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_isolating_16bit_space_2 ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_short_and_wide_subgraph_మూలం (&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_short_and_wide_subgraph_మూలం_expected (&e);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_isolation_spaces ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_isolation_అధికప్రవాహం_spaces (&c);
  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_always_assert (c.స్థానభ్రంశం_అధికప్రవాహం ());
  hb_blob_t* బహిర్గతం = hb_resolve_overflows (c.object_లేఖాచిత్రం (), HB_ట్యాగ్ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (బహిర్గతం);
  hb_బైట్లు_t ఫలితం = బహిర్గతం->as_బైట్లు ();

  గుర్తులేని expected_నిడివి = 3 + 2 * 60000; // objects
  expected_నిడివి += 2 * 4 + 2 * 2; // links
  hb_always_assert (ఫలితం.నిడివి == expected_నిడివి);

  విడుదల (బఫరు);
  hb_blob_destroy (బహిర్గతం);
}

స్థిర శూన్య test_resolve_mixed_overflows_ద్వారా_isolation_spaces ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_24_and_32_బిట్టు_offsets (&c);
  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_always_assert (c.స్థానభ్రంశం_అధికప్రవాహం ());
  hb_blob_t* బహిర్గతం = hb_resolve_overflows (c.object_లేఖాచిత్రం (), HB_ట్యాగ్ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (బహిర్గతం);
  hb_బైట్లు_t ఫలితం = బహిర్గతం->as_బైట్లు ();

  గుర్తులేని expected_నిడివి =
      // Objects
      7 +
      4 * 40000;

  expected_నిడివి +=
      // Links
      2 * 4 +  // 32
      4 * 3 +  // 24
      4 * 2;   // 16

  hb_always_assert (ఫలితం.నిడివి == expected_నిడివి);

  విడుదల (బఫరు);
  hb_blob_destroy (బహిర్గతం);
}

స్థిర శూన్య test_resolve_with_extension_promotion ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_extension_promotion (&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_extension_promotion (&e, 3);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             నిజం);
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_shared_extension_promotion ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_extension_promotion (&c, 0, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_extension_promotion (&e, 3, నిజం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             నిజం);
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
స్థిర శూన్య test_resolve_with_extension_promotion_1_2 ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_extension_promotion (&c, 0, అబద్ధం, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_extension_promotion (&e, 3, అబద్ధం, నిజం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             నిజం);
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}
#అయితే_ముగింపు

స్థిర శూన్య test_resolve_with_repeated_extension_promotion ()
{
  size_t బఫరు_కొలత = 1000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);

  c.ప్రారంభం_serialize<అక్షరం> ();
  గుర్తులేని subtable = కూడు_object ("subt", 4, &c);
  ప్రారంభం_lookup (5, 2, &c);
  కూడు_స్థానభ్రంశం (subtable, &c);
  కూడు_స్థానభ్రంశం (subtable, &c);
  గుర్తులేని lookup = ముగింపు_lookup (&c);
  గుర్తులేని lookup_list = కూడు_lookup_list (&lookup, 1, &c);
  కూడు_gsubgpos_శీర్షిక (lookup_list, &c);
  c.చివర_serialize ();

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();
  స్వయంచాలక context = లేఖాచిత్రం::gsubgpos_లేఖాచిత్రం_context_t::create (HB_ట్యాగ్ ('G', 'S', 'U', 'B'),
                                                          లేఖాచిత్రం).value ();
  hb_always_assert (context.lookups.get_population () == 1);
  గుర్తులేని lookup_సూచీ = *context.lookups.కీలకాలు ();
  లేఖాచిత్రం::Lookup* lookup_పట్టిక_2 = context.lookups.get (lookup_సూచీ);
  hb_always_assert (lookup_పట్టిక_2->make_extension (context, lookup_సూచీ).is_ok ());

  స్థిరరాశి స్వయంచాలక& lookup_శీర్షం = లేఖాచిత్రం.vertices_[lookup_సూచీ];
  hb_always_assert (lookup_శీర్షం.obj ().real_links.నిడివి == 2);
  గుర్తులేని extension_సూచీ = lookup_శీర్షం.obj ().real_links[0].objidx;
  hb_always_assert (lookup_శీర్షం.obj ().real_links[1].objidx == extension_సూచీ);

  స్వయంచాలక* extension = లేఖాచిత్రం.vertices_[extension_సూచీ].as<
      లేఖాచిత్రం::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
  గుర్తులేని promoted_subtable = extension->get_subtable_సూచీ (లేఖాచిత్రం, extension_సూచీ).value ();
  hb_always_assert (లేఖాచిత్రం.vertices_[promoted_subtable].పట్టిక_కొలత () == 4);
  hb_always_assert (లేఖాచిత్రం.update_parents ().is_ok ());
  hb_always_assert (లేఖాచిత్రం.vertices_[extension_సూచీ].incoming_అంచులు_2 () == 2);
  hb_always_assert (లేఖాచిత్రం.vertices_[promoted_subtable].incoming_అంచులు_2 () == 1);

  hb_blob_t* బహిర్గతం = లేఖాచిత్రం::serialize (లేఖాచిత్రం).value ();
  hb_blob_destroy (బహిర్గతం);
  విడుదల (బఫరు);
}

స్థిర శూన్య test_resolve_with_basic_pair_pos_1_విభజించు ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, నిజం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'P', 'O', 'S'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_extension_pair_pos_1_విభజించు ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, నిజం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'P', 'O', 'S'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_basic_pair_pos_2_విభజించు ()
{
  size_t బఫరు_కొలత = 300000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, నిజం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'P', 'O', 'S'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_close_గమ్యం_హద్దు_pair_pos_2_విభజించు ()
{
  size_t బఫరు_కొలత = 300000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, నిజం, అబద్ధం, అబద్ధం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, నిజం, అబద్ధం, అబద్ధం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'P', 'O', 'S'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_pair_pos_2_విభజించు_with_device_tables ()
{
  size_t బఫరు_కొలత = 300000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, అబద్ధం, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, నిజం, నిజం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'P', 'O', 'S'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_basic_mark_పునాది_pos_1_విభజించు ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_mark_పునాది_pos_1 <40, 10, 110, 1>(&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_mark_పునాది_pos_1 <40, 10, 110, 2>(&e);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'P', 'O', 'S'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_basic_liga_విభజించు ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, అబద్ధం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, అబద్ధం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'S', 'U', 'B'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_liga_విభజించు_move ()
{
  size_t బఫరు_కొలత = 400000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, నిజం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'S', 'U', 'B'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_liga_విభజించు_overlapping_clone ()
{
  size_t బఫరు_కొలత = 400000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_large_liga_overlapping_clone_ఫలితం(&e);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'S', 'U', 'B'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_liga_విభజించు_shared_పట్టిక ()
{
  size_t బఫరు_కొలత = 400000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, నిజం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'S', 'U', 'B'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_with_liga_విభజించు_shared_coverage ()
{
  size_t బఫరు_కొలత = 400000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             నిజం,
                             HB_ట్యాగ్('G', 'S', 'U', 'B'));
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_splitting_spaces ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_విభజించు_spaces (&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_విభజించు_spaces_expected (&e);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);

}

స్థిర శూన్య test_resolve_overflows_ద్వారా_splitting_spaces_2 ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_విభజించు_spaces_2 (&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_విభజించు_spaces_expected_2 (&e);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య test_resolve_overflows_ద్వారా_ప్రాధాన్యత ()
{
  size_t బఫరు_కొలత = 160000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_ప్రాధాన్యత_అధికప్రవాహం (&c);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_ప్రాధాన్యత_అధికప్రవాహం_expected (&e);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}


స్థిర శూన్య test_virtual_link ()
{
  size_t బఫరు_కొలత = 100;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_virtual_link (&c);

  hb_blob_t* బహిర్గతం = hb_resolve_overflows (c.object_లేఖాచిత్రం (), HB_ట్యాగ్_NONE);
  hb_always_assert (బహిర్గతం);

  hb_బైట్లు_t ఫలితం = బహిర్గతం->as_బైట్లు ();
  hb_always_assert (ఫలితం.నిడివి == 5 + 4 * 2);
  hb_always_assert (ఫలితం[0]  == 'a');
  hb_always_assert (ఫలితం[5]  == 'c');
  hb_always_assert (ఫలితం[8]  == 'e');
  hb_always_assert (ఫలితం[9]  == 'b');
  hb_always_assert (ఫలితం[12] == 'd');

  విడుదల (బఫరు);
  hb_blob_destroy (బహిర్గతం);
}

స్థిర శూన్య
test_shared_కణుపు_with_virtual_links ()
{
  size_t బఫరు_కొలత = 100;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);

  c.ప్రారంభం_serialize<అక్షరం> ();

  గుర్తులేని obj_b = కూడు_object ("b", 1, &c);
  గుర్తులేని obj_c = కూడు_object ("c", 1, &c);

  ప్రారంభం_object ("d", 1, &c);
  కూడు_virtual_స్థానభ్రంశం (obj_b, &c);
  గుర్తులేని obj_d_1 = c.తీయి_pack ();

  ప్రారంభం_object ("d", 1, &c);
  కూడు_virtual_స్థానభ్రంశం (obj_c, &c);
  గుర్తులేని obj_d_2 = c.తీయి_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  ప్రారంభం_object ("a", 1, &c);
  కూడు_స్థానభ్రంశం (obj_b, &c);
  కూడు_స్థానభ్రంశం (obj_c, &c);
  కూడు_స్థానభ్రంశం (obj_d_1, &c);
  కూడు_స్థానభ్రంశం (obj_d_2, &c);
  c.తీయి_pack ();
  c.చివర_serialize ();

  hb_always_assert(c.object_లేఖాచిత్రం() [obj_d_1]->virtual_links.నిడివి == 2);
  hb_always_assert(c.object_లేఖాచిత్రం() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_లేఖాచిత్రం() [obj_d_1]->virtual_links[1].objidx == obj_c);
  విడుదల(బఫరు);
}

స్థిర శూన్య
test_repack_చివరి ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_with_repack_చివరి (&c, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_with_repack_చివరి (&e, అబద్ధం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_repack_last",
                             c,
                             e,
                             20,
                             అబద్ధం,
                             HB_ట్యాగ్('a', 'b', 'c', 'd'),
                             నిజం);

  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య
test_dont_duplicate_virtual ()
{
  size_t బఫరు_కొలత = 200000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (బఫరు);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_virtual (&c, నిజం);

  శూన్య* expected_బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_always_assert (expected_బఫరు);
  hb_serialize_context_t e (expected_బఫరు, బఫరు_కొలత);
  populate_serializer_virtual (&e, అబద్ధం);

  నిరంతరం_resolve_అధికప్రవాహం_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             అబద్ధం,
                             HB_ట్యాగ్('a', 'b', 'c', 'd'),
                             నిజం);

  విడుదల (బఫరు);
  విడుదల (expected_బఫరు);
}

స్థిర శూన్య
test_deep_లేఖాచిత్రం_traversal ()
{
  size_t బఫరు_కొలత = 1000000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);

  c.ప్రారంభం_serialize ();
  గుర్తులేని prev = కూడు_object ("leaf", 4, &c);
  కొరకు (గుర్తులేని i = 0; i < 2000; i++)
  {
    ప్రారంభం_object ("node", 4, &c);
    కూడు_స్థానభ్రంశం (prev, &c);
    prev = c.తీయి_pack (అబద్ధం);
  }
  c.చివర_serialize ();

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  // Test find_subgraph (set)
  hb_సమితి_t సందర్శించిన_సమితి;
  hb_always_assert (లేఖాచిత్రం.కనుగొను_subgraph (లేఖాచిత్రం.మూలం_idx (), సందర్శించిన_సమితి).is_ok ());
  hb_always_assert (సందర్శించిన_సమితి.get_population () == 2001);

  // Test find_subgraph_size
  hb_సమితి_t కొలత_సమితి;
  size_t sz = లేఖాచిత్రం.కనుగొను_subgraph_కొలత (లేఖాచిత్రం.మూలం_idx (), కొలత_సమితి).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.సమితి (లేఖాచిత్రం.మూలం_idx (), 1);
  hb_always_assert (లేఖాచిత్రం.కనుగొను_subgraph (లేఖాచిత్రం.మూలం_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (లేఖాచిత్రం.కేటాయించు_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t సూచీ_map;
  hb_always_assert (లేఖాచిత్రం.duplicate_subgraph (లేఖాచిత్రం.మూలం_idx (), సూచీ_map).is_ok ());
  hb_always_assert (సూచీ_map.get_population () == 2001);

  విడుదల (బఫరు);
}

స్థిర శూన్య
test_32bit_roots_traversal ()
{
  size_t బఫరు_కొలత = 1000;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);

  c.ప్రారంభం_serialize ();
  గుర్తులేని leaf1 = కూడు_object ("l1", 2, &c);
  గుర్తులేని leaf2 = కూడు_object ("l2", 2, &c);

  ప్రారంభం_object ("mid", 3, &c);
  కూడు_wide_స్థానభ్రంశం (leaf1, &c);
  గుర్తులేని mid = c.తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("root", 4, &c);
  కూడు_స్థానభ్రంశం (mid, &c);
  కూడు_wide_స్థానభ్రంశం (leaf2, &c);
  c.తీయి_pack (అబద్ధం);
  c.చివర_serialize ();

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  hb_సమితి_t roots;
  hb_always_assert (లేఖాచిత్రం.కనుగొను_32bit_roots (లేఖాచిత్రం.మూలం_idx (), roots).is_ok ());
  hb_always_assert (roots.has (leaf1 - 1));
  hb_always_assert (roots.has (leaf2 - 1));
  hb_always_assert (!roots.has (mid - 1));
  hb_always_assert (roots.get_population () == 2);

  విడుదల (బఫరు);

  // Test case where DFS fails but BFS succeeds:
  // root -16-> a
  // root -32-> a
  // a -32-> b
  //
  // DFS will first explore the 16 bit link to a and then recurse the 32 bit link from a to b
  // and consider b a root. However, b should not be a root since it's not a top level 32 bit link.
  // BFS will handle this correctly by discovering a as a top level root and then avoiding further
  // exploring it.
  బఫరు_కొలత = 1000;
  బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c2 (బఫరు, బఫరు_కొలత);

  c2.ప్రారంభం_serialize ();
  గుర్తులేని b = కూడు_object ("b", 1, &c2);

  ప్రారంభం_object ("a", 1, &c2);
  కూడు_wide_స్థానభ్రంశం (b, &c2);
  గుర్తులేని a = c2.తీయి_pack (అబద్ధం);

  ప్రారంభం_object ("r", 1, &c2);
  కూడు_స్థానభ్రంశం (a, &c2);
  కూడు_wide_స్థానభ్రంశం (a, &c2);
  c2.తీయి_pack (అబద్ధం);

  c2.చివర_serialize ();

  లేఖాచిత్రం_t graph2 = లేఖాచిత్రం_t::create (c2.object_లేఖాచిత్రం ()).value ();

  hb_సమితి_t roots2;
  hb_always_assert (graph2.కనుగొను_32bit_roots (graph2.మూలం_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  విడుదల (బఫరు);
}

స్థిర శూన్య test_invalid_link_objidx ()
{
  size_t బఫరు_కొలత = 100;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_complex_2 (&c);

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  // Poison a link to point to (unsigned) -1
  స్వయంచాలక links = లేఖాచిత్రం.vertices_[లేఖాచిత్రం.మూలం_idx ()].real_links_writer();
  links->objidx = (గుర్తులేని) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!లేఖాచిత్రం.క్రమీకరించు_అతితక్కువ_దూరం ().is_ok ());

  విడుదల (బఫరు);
}

స్థిర శూన్య test_invalid_link_కూడు_and_move ()
{
  size_t బఫరు_కొలత = 100;
  శూన్య* బఫరు = జ్ఞాపకం_ఇవ్వు (బఫరు_కొలత);
  hb_serialize_context_t c (బఫరు, బఫరు_కొలత);
  populate_serializer_complex_2 (&c);

  లేఖాచిత్రం_t లేఖాచిత్రం = లేఖాచిత్రం_t::create (c.object_లేఖాచిత్రం ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!లేఖాచిత్రం.కూడు_link (&dummy, 0, (గుర్తులేని) -1).is_ok ());

  // move_child with non-existent offset
  స్వయంచాలక res = లేఖాచిత్రం.move_బిడ్డ (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  విడుదల (బఫరు);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  test_invalid_link_objidx ();
  test_invalid_link_కూడు_and_move ();
  test_serialize ();
  test_క్రమీకరించు_అతితక్కువ ();
  test_will_అధికప్రవాహం_1 ();
  test_will_అధికప్రవాహం_2 ();
  test_will_అధికప్రవాహం_3 ();
  test_resolve_overflows_ద్వారా_క్రమీకరించు ();
  test_resolve_overflows_ద్వారా_duplication ();
  test_resolve_overflows_ద్వారా_గుణిజం_duplications ();
  test_resolve_overflows_ద్వారా_ప్రాధాన్యత ();
  test_resolve_overflows_ద్వారా_space_assignment ();
  test_resolve_overflows_ద్వారా_isolation ();
  test_resolve_overflows_ద్వారా_isolation_with_recursive_duplication ();
  test_resolve_overflows_ద్వారా_isolation_spaces ();
  test_resolve_overflows_ద్వారా_isolating_16bit_space ();
  test_resolve_overflows_ద్వారా_isolating_16bit_space_2 ();
  test_resolve_overflows_ద్వారా_splitting_spaces ();
  test_resolve_overflows_ద్వారా_splitting_spaces_2 ();
  test_resolve_mixed_overflows_ద్వారా_isolation_spaces ();
  test_duplicate_ఆకు ();
  test_duplicate_interior ();
  test_virtual_link ();
  test_repack_చివరి();
  test_shared_కణుపు_with_virtual_links ();
  test_resolve_with_extension_promotion ();
  test_resolve_with_shared_extension_promotion ();
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  test_resolve_with_extension_promotion_1_2 ();
#అయితే_ముగింపు
  test_resolve_with_repeated_extension_promotion ();
  test_resolve_with_basic_pair_pos_1_విభజించు ();
  test_resolve_with_extension_pair_pos_1_విభజించు ();
  test_resolve_with_basic_pair_pos_2_విభజించు ();
  test_resolve_with_pair_pos_2_విభజించు_with_device_tables ();
  test_resolve_with_close_గమ్యం_హద్దు_pair_pos_2_విభజించు ();
  test_resolve_with_basic_mark_పునాది_pos_1_విభజించు ();
  test_resolve_with_basic_liga_విభజించు ();
  test_resolve_with_liga_విభజించు_move ();
  test_dont_duplicate_virtual ();
  test_resolve_with_liga_విభజించు_overlapping_clone ();
  test_resolve_with_liga_విభజించు_shared_పట్టిక ();
  test_resolve_with_liga_విభజించు_shared_coverage ();
  test_deep_లేఖాచిత్రం_traversal ();
  test_32bit_roots_traversal ();

  // TODO(grieger): have run overflow tests compare graph equality not final packed binary.
  // TODO(grieger): split test where multiple subtables in one lookup are split to test link ordering.
  // TODO(grieger): split test where coverage table in subtable that is being split is shared.
  // TODO(grieger): test with extensions already mixed in as well.
  // TODO(grieger): test two layer ext promotion setup.
  // TODO(grieger): test sorting by subtables per byte in ext. promotion.
}
