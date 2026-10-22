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

#अन्तर्भाव <cstdint>
#अन्तर्भाव <string>

#अन्तर्भाव "hb-repacker.hh"
#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "graph/serialize.hh"

स्थिर शून्य extend (ध्रुव वर्ण* value,
                    unsigned len,
                    hb_serialize_context_t* c)
{
  वर्ण* obj = c->allocate_size<वर्ण> (len);
  hb_memcpy (obj, value, len);
}

स्थिर शून्य start_object(ध्रुव वर्ण* tag,
                         unsigned len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (tag, len, c);
}

स्थिर unsigned add_object(ध्रुव वर्ण* tag,
                           unsigned len,
                           hb_serialize_context_t* c,
                           तार्किक shared = मिच्छा)
{
  start_object (tag, len, c);
  पटिनिवत्त c->निष्कासय_pack (shared);
}


स्थिर शून्य add_offset (unsigned id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* offset = c->start_embed<OT::Offset16> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

स्थिर शून्य add_24_offset (unsigned id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* offset = c->start_embed<OT::Offset24> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

स्थिर शून्य add_wide_offset (unsigned id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* offset = c->start_embed<OT::Offset32> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

स्थिर शून्य add_gsubgpos_header (unsigned lookup_list,
                                 hb_serialize_context_t* c)
{
  वर्ण header[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  start_object (header, 8, c);
  add_offset (lookup_list, c);
  c->निष्कासय_pack (मिच्छा);
}

स्थिर शून्य add_gsubgpos_header_1_2 (unsigned lookup_list,
                                     hb_serialize_context_t* c)
{
  वर्ण header[] = {
    0, 1, 0, 2, // version
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  start_object (header, परिमाण (header), c);
  add_wide_offset (lookup_list, c);
  c->निष्कासय_pack (मिच्छा);
}

स्थिर unsigned add_lookup_list (ध्रुव unsigned* lookups,
                                 वर्ण गणनम्,
                                 hb_serialize_context_t* c)
{
  वर्ण lookup_गणनम्[] = {0, गणनम्};
  start_object  ((वर्ण *) &lookup_गणनम्, 2, c);

  अत्थाय (पूर्णाङ्क i = 0; i < गणनम्; i++)
    add_offset (lookups[i], c);

  पटिनिवत्त c->निष्कासय_pack (मिच्छा);
}

स्थिर unsigned add_lookup_list_1_2 (ध्रुव unsigned* lookups,
                                     वर्ण गणनम्,
                                     hb_serialize_context_t* c)
{
  वर्ण lookup_गणनम्[] = {0, गणनम्};
  start_object ((वर्ण *) &lookup_गणनम्, 2, c);

  अत्थाय (पूर्णाङ्क i = 0; i < गणनम्; i++)
    add_wide_offset (lookups[i], c);

  पटिनिवत्त c->निष्कासय_pack (मिच्छा);
}

स्थिर शून्य start_lookup (int8_t type,
                          int8_t अङ्कः_subtables,
                          hb_serialize_context_t* c)
{
  वर्ण lookup[] = {
    0, (वर्ण)type, // type
    0, (वर्ण)2, // flag
    0, (वर्ण)अङ्कः_subtables, // num subtables
  };

  start_object (lookup, 6, c);
}

स्थिर unsigned finish_lookup (hb_serialize_context_t* c)
{
  वर्ण filter[] = {0, 0};
  extend (filter, 2, c);
  पटिनिवत्त c->निष्कासय_pack (मिच्छा);
}

स्थिर unsigned add_extension (unsigned child,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  वर्ण ext[] = {
    0, 1,
    0, (वर्ण) type,
  };

  start_object (ext, 4, c);
  add_wide_offset (child, c);

  पटिनिवत्त c->निष्कासय_pack (मिच्छा);

}

// Adds coverage table fro [start, end]
स्थिर unsigned add_coverage (unsigned start, unsigned end,
                              hb_serialize_context_t* c,
                              तार्किक shared = मिच्छा)
{
  सचे (end - start == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //गणनम्

      (uint8_t) ((start >> 8) & 0xFF),
      (uint8_t) (start & 0xFF), // glyph[0]
    };
    पटिनिवत्त add_object ((वर्ण*) coverage, 6, c, shared);
  }

  सचे (end - start == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //गणनम्

      (uint8_t) ((start >> 8) & 0xFF),
      (uint8_t) (start & 0xFF), // glyph[0]

      (uint8_t) ((end >> 8) & 0xFF),
      (uint8_t) (end & 0xFF), // glyph[1]
    };
    पटिनिवत्त add_object ((वर्ण*) coverage, 8, c, shared);
  }

  uint8_t coverage[] = {
    0, 2, // format
    0, 1, // range count

    (uint8_t) ((start >> 8) & 0xFF),
    (uint8_t) (start & 0xFF), // start

    (uint8_t) ((end >> 8) & 0xFF),
    (uint8_t) (end & 0xFF), // end

    0, 0,
  };
  पटिनिवत्त add_object ((वर्ण*) coverage, 10, c, shared);
}


template<typename It>
स्थिर unsigned add_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  पटिनिवत्त c->निष्कासय_pack (मिच्छा);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
स्थिर unsigned add_class_def (uint16_t start_glyph,
                               uint16_t end_glyph,
                               hb_serialize_context_t* c)
{
  unsigned गणनम् = end_glyph - start_glyph;
  uint8_t header[] = {
    0, 1, // format

    (uint8_t) ((start_glyph >> 8) & 0xFF),
    (uint8_t) (start_glyph & 0xFF), // start_glyph

    (uint8_t) ((गणनम् >> 8) & 0xFF),
    (uint8_t) (गणनम् & 0xFF), //गणनम्
  };

  start_object ((वर्ण*) header, 6, c);
  अत्थाय (uint16_t i = 1; i <= गणनम्; i++)
  {
    uint8_t class_मानम्_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //गणनम्
    };
    extend ((वर्ण*) class_मानम्_2, 2, c);
  }

  पटिनिवत्त c->निष्कासय_pack (मिच्छा);
}

स्थिर unsigned add_pair_pos_1 (unsigned* pair_sets,
                                वर्ण गणनम्,
                                unsigned coverage,
                                hb_serialize_context_t* c)
{
  वर्ण format[] = {
    0, 1
  };

  start_object (format, 2, c);
  add_offset (coverage, c);

  वर्ण मानम्_format[] = {
    0, 0,
    0, 0,
    0, गणनम्,
  };
  extend (मानम्_format, 6, c);

  अत्थाय (वर्ण i = 0; i < गणनम्; i++)
    add_offset (pair_sets[(unsigned) i], c);

  पटिनिवत्त c->निष्कासय_pack (मिच्छा);
}

स्थिर शून्य add_liga_set_header (वर्ण liga_गणनम्,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_गणनम्_bytes[] = {
    (uint8_t) (0xFF & (liga_गणनम् >> 8)),
    (uint8_t) (0xFF & liga_गणनम्),
  };

  start_object ((ध्रुव वर्ण*) liga_गणनम्_bytes, 2, c);
}

स्थिर शून्य add_liga_header (unsigned coverage, unsigned liga_set_गणनम्, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  start_object ((ध्रुव वर्ण*) format, 2, c);
  add_offset(coverage, c);

  uint8_t liga_set_गणनम्_bytes[] = {
    (uint8_t) (0xFF & (liga_set_गणनम् >> 8)),
    (uint8_t) (0xFF & liga_set_गणनम्),
  };

  extend((ध्रुव वर्ण *) liga_set_गणनम्_bytes, 2, c);
}

स्थिर unsigned add_pair_pos_2 (unsigned starting_class,
                                unsigned coverage,
                                unsigned class_def_1, uint16_t class_def_1_गणनम्,
                                unsigned class_def_2, uint16_t class_def_2_गणनम्,
                                unsigned* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  start_object ((वर्ण*) format, 2, c);
  add_offset (coverage, c);

  unsigned अङ्कः_मानानि_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  सचे (device_tables) {
    format2 |= 0x20;
    अङ्कः_मानानि_3 += 1;
  }
  uint8_t मानम्_format[] = {
    0, format1,
    0, format2,
  };

  extend ((वर्ण*) मानम्_format, 4, c);

  add_offset (class_def_1, c);
  add_offset (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_गणनम् >> 8) & 0xFF),
    (uint8_t) (class_def_1_गणनम् & 0xFF),
    (uint8_t) ((class_def_2_गणनम् >> 8) & 0xFF),
    (uint8_t) (class_def_2_गणनम् & 0xFF),
  };
  extend ((वर्ण*) class_counts, 4, c);

  unsigned अङ्कः_bytes_per_record = class_def_2_गणनम् * अङ्कः_मानानि_3 * 2;
  uint8_t* record = (uint8_t*) calloc (1, अङ्कः_bytes_per_record);
  पूर्णाङ्क device_index = 0;
  अत्थाय (uint16_t i = 0; i < class_def_1_गणनम्; i++)
  {

    अत्थाय (uint16_t j = 0; j < class_def_2_गणनम्; j++)
    {
      अत्थाय (पूर्णाङ्क k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((वर्ण*) value, 2, c);
      }

      सचे (device_tables) {
        add_offset (device_tables[device_index++], c);
      }
    }
  }
  मुक्त (record);

  पटिनिवत्त c->निष्कासय_pack (मिच्छा);
}

स्थिर unsigned add_mark_base_pos_1 (unsigned mark_coverage,
                                     unsigned base_coverage,
                                     unsigned mark_array,
                                     unsigned base_array,
                                     unsigned class_गणनम्,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  start_object ((वर्ण*) format, 2, c);
  add_offset (mark_coverage, c);
  add_offset (base_coverage, c);

  uint8_t गणनम्[] = {
    (uint8_t) ((class_गणनम् >> 8) & 0xFF),
    (uint8_t) (class_गणनम् & 0xFF),
  };
  extend ((वर्ण*) गणनम्, 2, c);

  add_offset (mark_array, c);
  add_offset (base_array, c);

  पटिनिवत्त c->निष्कासय_pack (मिच्छा);
}

template<पूर्णाङ्क mark_गणनम्,
    पूर्णाङ्क class_गणनम्,
    पूर्णाङ्क base_गणनम्,
    पूर्णाङ्क table_गणनम्_2>
संरचना MarkBasePosBuffers
{
  unsigned base_anchors[class_गणनम् * base_गणनम्];
  unsigned mark_anchors[mark_गणनम्];
  uint8_t anchor_buffers[class_गणनम् * base_गणनम् + 100];
  uint8_t class_buffer[class_गणनम् * 2];

  MarkBasePosBuffers(hb_serialize_context_t* c)
  {
    अत्थाय (unsigned i = 0; i < परिमाण(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    अत्थाय (unsigned i = 0; i < class_गणनम् * base_गणनम्; i++)
    {
      base_anchors[i] = add_object ((वर्ण*) &anchor_buffers[i], 100, c);
      सचे (i < class_गणनम्) {
        class_buffer[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_buffer[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    अत्थाय (unsigned i = 0; i < mark_गणनम्; i++)
    {
      mark_anchors[i] = add_object ((वर्ण*) &anchor_buffers[i], 4, c);
    }
  }

  unsigned create_mark_base_pos_1 (unsigned table_index, hb_serialize_context_t* c)
  {
    unsigned class_per_table = class_गणनम् / table_गणनम्_2;
    unsigned mark_per_class = mark_गणनम् / class_गणनम्;
    unsigned start_class = class_per_table * table_index;
    unsigned end_class = class_per_table * (table_index + 1) - 1;

    // baseArray
    uint8_t base_गणनम्_buffer[] = {
      (uint8_t) ((base_गणनम् >> 8) & 0xFF),
      (uint8_t) (base_गणनम् & 0xFF),

    };
    start_object ((वर्ण*) base_गणनम्_buffer, 2, c);
    अत्थाय (unsigned base = 0; base < base_गणनम्; base++)
    {
      अत्थाय (unsigned klass = start_class; klass <= end_class; klass++)
      {
        unsigned i = base * class_गणनम् + klass;
        add_offset (base_anchors[i], c);
      }
    }
    unsigned base_array = c->निष्कासय_pack (मिच्छा);

    // markArray
    unsigned अङ्कः_marks = class_per_table * mark_per_class;
    uint8_t mark_गणनम्_buffer[] = {
      (uint8_t) ((अङ्कः_marks >> 8) & 0xFF),
      (uint8_t) (अङ्कः_marks & 0xFF),
    };
    start_object ((वर्ण*) mark_गणनम्_buffer, 2, c);
    अत्थाय (unsigned mark = 0; mark < mark_गणनम्; mark++)
    {
      unsigned klass = mark % class_गणनम्;
      सचे (klass < start_class || klass > end_class) अनुवर्त;
      klass -= start_class;

      extend ((वर्ण*) &class_buffer[2 * klass], 2, c);
      add_offset (mark_anchors[mark], c);
    }
    unsigned mark_array = c->निष्कासय_pack (मिच्छा);

    // markCoverage
    auto it =
        + hb_range ((hb_codepoint_t) mark_गणनम्)
        | hb_filter ([&] (hb_codepoint_t mark) {
          unsigned klass = mark % class_गणनम्;
          पटिनिवत्त klass >= class_per_table * table_index &&
              klass < class_per_table * (table_index + 1);
        })
        ;
    unsigned mark_coverage = add_coverage (it, c);

    // baseCoverage
    unsigned base_coverage = add_coverage (10, 10 + base_गणनम् - 1, c);

    पटिनिवत्त add_mark_base_pos_1 (mark_coverage,
                                base_coverage,
                                mark_array,
                                base_array,
                                class_per_table,
                                c);
  }
};

स्थिर शून्य run_resolve_overflow_test (ध्रुव वर्ण* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       unsigned अङ्कः_iterations = 0,
                                       तार्किक recalculate_extensions = मिच्छा,
                                       hb_tag_t tag = HB_TAG ('G', 'S', 'U', 'B'),
                                       तार्किक check_द्विमान_equivalence = मिच्छा)
{
  printf (">>> Testing overflowing resolution for %s\n",
          name);

  graph_t graph = graph_t::create (overflowing.object_graph ()).value ();

  graph_t expected_graph = graph_t::create (expected.object_graph ()).value ();
  सचे (graph::will_overflow (expected_graph).value ())
  {
    सचे (check_द्विमान_equivalence) {
      printf("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_द्विमान_equivalence);
    }
    hb_always_assert (expected_graph.assign_spaces ().is_ok ());
    hb_always_assert (expected_graph.sort_shortest_distance ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.offset_overflow ());
  auto r = hb_resolve_graph_overflows (tag,
                                      अङ्कः_iterations,
                                      recalculate_extensions,
                                      graph);
  सचे (r.is_err())
    printf("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", graph::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = graph::serialize (graph).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = graph::serialize (expected_graph).value ();
  hb_always_assert (out2);
  सचे (check_द्विमान_equivalence) {
    unsigned l1, l2;
    ध्रुव वर्ण* d1 = hb_blob_get_data(out1, &l1);
    ध्रुव वर्ण* d2 = hb_blob_get_data(out2, &l2);

    तार्किक match = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    सचे (!match) {
      printf("## Result:\n");
      graph.print();
      printf("## Expected:\n");
      expected_graph.print();
      hb_always_assert(match);
    }
  }

  hb_blob_destroy (out1);
  hb_blob_destroy (out2);

  // Check the graphs are equivalent
  graph.normalize ();
  expected_graph.normalize ();
  सचे (!(graph == expected_graph)) {
    printf("## Expected:\n");
    expected_graph.print();
    printf("## Result:\n");
    graph.print();
  }
  hb_always_assert (graph == expected_graph);
}

स्थिर शून्य add_virtual_offset (unsigned id,
                                hb_serialize_context_t* c)
{
  c->add_virtual_link (id);
}

स्थिर शून्य
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->start_serialize<वर्ण> ();

  unsigned obj_1 = add_object ("ghi", 3, c);
  unsigned obj_2 = add_object ("def", 3, c);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_virtual (hb_serialize_context_t* c, तार्किक with_overflow)
{
  std::string large_string(50000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_4, obj_5;
  सचे (with_overflow) {
    obj_5 = add_object("55555", 5, c);
    obj_4 = add_object(large_string.c_str(), 50000, c);
  } अञ्ञथा {
    obj_4 = add_object(large_string.c_str(), 50000, c);
    obj_5 = add_object("55555", 5, c);
  }

  start_object(large_string.c_str(), 20000, c);
  add_offset(obj_5, c);
  unsigned obj_3 = c->निष्कासय_pack(मिच्छा);

  start_object("2", 2, c);
  add_virtual_offset(obj_5, c);
  unsigned obj_2 = c->निष्कासय_pack(मिच्छा);

  // obj 1
  start_object("1", 1, c);
  add_offset(obj_2, c);
  add_offset(obj_3, c);
  add_offset(obj_4, c);
  c->निष्कासय_pack(मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_overflow (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_1 = add_object (large_string.c_str(), 10000, c);
  unsigned obj_2 = add_object (large_string.c_str(), 20000, c);
  unsigned obj_3 = add_object (large_string.c_str(), 50000, c);

  start_object ("abc", 3, c);
  add_offset (obj_3, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_priority_overflow (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_e = add_object ("e", 1, c);
  unsigned obj_d = add_object ("d", 1, c);

  start_object (large_string.c_str (), 50000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str (), 20000, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_priority_overflow_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 50000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  unsigned obj_d = add_object ("d", 1, c);

  start_object (large_string.c_str (), 20000, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}


स्थिर शून्य
populate_serializer_with_dedup_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_1 = add_object ("def", 3, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_1, c);
  unsigned obj_2 = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_multiple_dedup_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned leaf = add_object("def", 3, c);

  constexpr unsigned अङ्कः_mid_nodes = 20;
  unsigned mid_nodes[अङ्कः_mid_nodes];
  अत्थाय (unsigned i = 0; i < अङ्कः_mid_nodes; i++) {
    start_object(large_string.c_str(), 10000 + i, c);
    add_offset(leaf, c);
    mid_nodes[i] = c->निष्कासय_pack(मिच्छा);
  }

  start_object("abc", 3, c);
  अत्थाय (unsigned i = 0; i < अङ्कः_mid_nodes; i++) {
    add_wide_offset(mid_nodes[i], c);
  }
  c->निष्कासय_pack(मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_isolation_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_4 = add_object ("4", 1, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_4, c);
  unsigned obj_3 = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_4, c);
  unsigned obj_2 = c->निष्कासय_pack (मिच्छा);

  start_object ("1", 1, c);
  add_wide_offset (obj_3, c);
  add_offset (obj_2, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_isolation_overflow_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_f = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object ("d", 1, c);
  add_offset (obj_e, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d, c);
  unsigned obj_h = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_c, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_d, c);
  unsigned obj_g = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 11000, c);
  add_offset (obj_d, c);
  unsigned obj_i = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_offset (obj_g, c);
  add_offset (obj_i, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_isolation_overflow_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();


  // space 1

  unsigned obj_f_अविभाज्यः = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f_अविभाज्यः, c);
  unsigned obj_e_अविभाज्यः = c->निष्कासय_pack (मिच्छा);

  start_object ("d", 1, c);
  add_offset (obj_e_अविभाज्यः, c);
  unsigned obj_d_अविभाज्यः = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d_अविभाज्यः, c);
  unsigned obj_h = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_e_अविभाज्यः, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_c, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  // space 0

  unsigned obj_f = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);


  start_object ("d", 1, c);
  add_offset (obj_e, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 11000, c);
  add_offset (obj_d, c);
  unsigned obj_i = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_d, c);
  unsigned obj_g = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_offset (obj_g, c);
  add_offset (obj_i, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_isolation_overflow_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_d = add_object ("f", 1, c);
  unsigned obj_e = add_object ("f", 1, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->निष्कासय_pack ();

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack ();


  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->निष्कासय_pack ();

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_repack_last (hb_serialize_context_t* c, तार्किक with_overflow)
{
  std::string large_string(70000, 'c');
  c->start_serialize<वर्ण> ();
  c->push();

  // Obj E
  unsigned obj_e_1, obj_e_2;
  सचे (with_overflow) {
    obj_e_1 = add_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } अञ्ञथा {
    obj_e_2 = add_object("a", 1, c);
  }

  // Obj D
  c->push();
  add_offset(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  unsigned obj_d = c->निष्कासय_pack(मिच्छा);

  add_offset(obj_d, c);
  hb_always_assert(c->last_added_child_index() == obj_d);

  सचे (!with_overflow) {
    obj_e_1 = add_object("a", 1, c);
  }

  // Obj C
  c->push();
  add_offset(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  unsigned obj_c = c->निष्कासय_pack(मिच्छा);

  add_offset(obj_c, c);

  // Obj B
  unsigned obj_b = add_object("b", 1, c);
  add_offset(obj_b, c);

  // Obj A
  c->repack_last(obj_d);
  c->निष्कासय_pack(मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_spaces (hb_serialize_context_t* c, तार्किक with_overflow)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_i;

  सचे (with_overflow)
    obj_i = add_object ("i", 1, c);

  // Space 2
  unsigned obj_h = add_object ("h", 1, c);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_h, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  // Space 1
  सचे (!with_overflow)
    obj_i = add_object ("i", 1, c);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_i, c);
  unsigned obj_g = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_i, c);
  unsigned obj_f = c->निष्कासय_pack (मिच्छा);

  start_object ("d", 1, c);
  add_offset (obj_g, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_f, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_g = add_object ("g", 1, c);
  unsigned obj_h = add_object ("h", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_h, c);
  unsigned obj_f = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object ("d", 1, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_g_अविभाज्यः = add_object ("g", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g_अविभाज्यः, c);
  unsigned obj_e_अविभाज्यः = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_e_अविभाज्यः, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  unsigned obj_h_अविभाज्यः = add_object ("h", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_h_अविभाज्यः, c);
  unsigned obj_f = c->निष्कासय_pack (मिच्छा);

  start_object ("d", 1, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  unsigned obj_g = add_object ("g", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  unsigned obj_h = add_object ("h", 1, c);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize ();
}

स्थिर शून्य
populate_serializer_short_and_wide_subgraph_मूलम् (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_c, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object ("b", 1, c);
  add_offset (obj_c, c);
  add_offset (obj_e, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_short_and_wide_subgraph_मूलम्_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_e_अविभाज्यः = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e_अविभाज्यः, c);
  unsigned obj_c_अविभाज्यः = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_c_अविभाज्यः, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  unsigned obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);


  start_object ("b", 1, c);
  add_offset (obj_c, c);
  add_offset (obj_e, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c_अविभाज्यः, c);
  add_wide_offset (obj_d, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_split_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_split_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_split_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_f_अविभाज्यः = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_अविभाज्यः, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_split_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->start_serialize<वर्ण> ();

  // Space 2

  unsigned obj_f_double_अविभाज्यः = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_double_अविभाज्यः, c);
  unsigned obj_d_अविभाज्यः = c->निष्कासय_pack (मिच्छा);

  start_object ("b", 1, c);
  add_offset (obj_d_अविभाज्यः, c);
  unsigned obj_b_अविभाज्यः = c->निष्कासय_pack (मिच्छा);

  // Space 1

  unsigned obj_f_अविभाज्यः = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_अविभाज्यः, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  // Space 0

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  //मूलम्
  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_b_अविभाज्यः, c);
  add_wide_offset (obj_c, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->start_serialize<वर्ण> ();

  unsigned obj_5 = add_object ("mn", 2, c);

  unsigned obj_4 = add_object ("jkl", 3, c);

  start_object ("ghi", 3, c);
  add_offset (obj_4, c);
  unsigned obj_3 = c->निष्कासय_pack (मिच्छा);

  start_object ("def", 3, c);
  add_offset (obj_3, c);
  unsigned obj_2 = c->निष्कासय_pack (मिच्छा);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_4, c);
  add_offset (obj_5, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->start_serialize<वर्ण> ();

  unsigned obj_6 = add_object ("opqrst", 6, c);

  unsigned obj_5 = add_object ("mn", 2, c);

  start_object ("jkl", 3, c);
  add_offset (obj_6, c);
  unsigned obj_4 = c->निष्कासय_pack (मिच्छा);

  start_object ("ghi", 3, c);
  add_offset (obj_4, c);
  unsigned obj_3 = c->निष्कासय_pack (मिच्छा);

  start_object ("def", 3, c);
  add_offset (obj_3, c);
  unsigned obj_2 = c->निष्कासय_pack (मिच्छा);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_4, c);
  add_offset (obj_5, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->start_serialize<वर्ण> ();

  unsigned obj_d = add_object ("d", 1, c);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("e", 1, c);
  add_virtual_offset (obj_b, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_24_and_32_bit_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned obj_f = add_object ("f", 1, c);
  unsigned obj_g = add_object ("g", 1, c);
  unsigned obj_j = add_object ("j", 1, c);
  unsigned obj_k = add_object ("k", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_c = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  unsigned obj_d = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_j, c);
  unsigned obj_h = c->निष्कासय_pack (मिच्छा);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_k, c);
  unsigned obj_i = c->निष्कासय_pack (मिच्छा);

  start_object ("e", 1, c);
  add_wide_offset (obj_h, c);
  add_wide_offset (obj_i, c);
  unsigned obj_e = c->निष्कासय_pack (मिच्छा);

  start_object ("b", 1, c);
  add_24_offset (obj_c, c);
  add_24_offset (obj_d, c);
  add_24_offset (obj_e, c);
  unsigned obj_b = c->निष्कासय_pack (मिच्छा);

  start_object ("a", 1, c);
  add_24_offset (obj_b, c);
  c->निष्कासय_pack (मिच्छा);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              पूर्णाङ्क अङ्कः_extensions = 0,
                                              तार्किक shared_subtables = मिच्छा,
                                              तार्किक version_1_2 = मिच्छा)
{
  constexpr पूर्णाङ्क अङ्कः_lookups = 5;
  constexpr पूर्णाङ्क अङ्कः_subtables = अङ्कः_lookups * 2;
  unsigned पूर्णाङ्क lookups[अङ्कः_lookups];
  unsigned पूर्णाङ्क subtables[अङ्कः_subtables];
  unsigned पूर्णाङ्क extensions[अङ्कः_subtables];

  std::string large_string(60000, 'a');
  c->start_serialize<वर्ण> ();


  अत्थाय (पूर्णाङ्क i = अङ्कः_subtables - 1; i >= 0; i--)
    subtables[i] = add_object(large_string.c_str (), 15000 + i, c);

  अत्थाय (पूर्णाङ्क i = अङ्कः_subtables - 1;
       i >= (अङ्कः_lookups - अङ्कः_extensions) * 2;
       i--)
  {
    extensions[i] = add_extension (subtables[i], 5, c);
  }

  अत्थाय (पूर्णाङ्क i = अङ्कः_lookups - 1; i >= 0; i--)
  {
    तार्किक is_ext = (i >= (अङ्कः_lookups - अङ्कः_extensions));

    start_lookup (is_ext ? (वर्ण) 7 : (वर्ण) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    सचे (is_ext) {
      सचे (shared_subtables && i > 2) {
        add_offset (extensions[i * 2 - 1], c);
      }
      add_offset (extensions[i * 2], c);
      add_offset (extensions[i * 2 + 1], c);
    } अञ्ञथा {
      सचे (shared_subtables && i > 2) {
        add_offset (subtables[i * 2 - 1], c);
      }
      add_offset (subtables[i * 2], c);
      add_offset (subtables[i * 2 + 1], c);
    }

    lookups[i] = finish_lookup (c);
  }

  unsigned lookup_list = version_1_2
                         ? add_lookup_list_1_2 (lookups, अङ्कः_lookups, c)
                         : add_lookup_list (lookups, अङ्कः_lookups, c);

  सचे (version_1_2)
    add_gsubgpos_header_1_2 (lookup_list, c);
  अञ्ञथा
    add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<पूर्णाङ्क अङ्कः_pair_pos_1, पूर्णाङ्क अङ्कः_pair_set>
स्थिर शून्य
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           तार्किक as_extension = मिच्छा)
{
  std::string large_string(60000, 'a');
  c->start_serialize<वर्ण> ();

  constexpr पूर्णाङ्क total_pair_set = अङ्कः_pair_pos_1 * अङ्कः_pair_set;
  unsigned pair_set[total_pair_set];
  unsigned coverage[अङ्कः_pair_pos_1];
  unsigned pair_pos_1[अङ्कः_pair_pos_1];

  अत्थाय (पूर्णाङ्क i = अङ्कः_pair_pos_1 - 1; i >= 0; i--)
  {
    अत्थाय (पूर्णाङ्क j = (i + 1) * अङ्कः_pair_set - 1; j >= i * अङ्कः_pair_set; j--)
      pair_set[j] = add_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = add_coverage (i * अङ्कः_pair_set,
                                (i + 1) * अङ्कः_pair_set - 1, c);

    pair_pos_1[i] = add_pair_pos_1 (&pair_set[i * अङ्कः_pair_set],
                                    अङ्कः_pair_set,
                                    coverage[i],
                                    c);
  }

  unsigned pair_pos_2 = add_object (large_string.c_str(), 200, c);

  सचे (as_extension) {
    pair_pos_2 = add_extension (pair_pos_2, 2, c);
    अत्थाय (पूर्णाङ्क i = अङ्कः_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = add_extension (pair_pos_1[i], 2, c);
  }

  start_lookup (as_extension ? 9 : 2, 1 + अङ्कः_pair_pos_1, c);

  अत्थाय (पूर्णाङ्क i = 0; i < अङ्कः_pair_pos_1; i++)
    add_offset (pair_pos_1[i], c);
  add_offset (pair_pos_2, c);

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<पूर्णाङ्क अङ्कः_pair_pos_2, पूर्णाङ्क अङ्कः_class_1, पूर्णाङ्क अङ्कः_class_2>
स्थिर शून्य
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           तार्किक as_extension = मिच्छा,
                                           तार्किक with_device_tables = मिच्छा,
                                           तार्किक extra_table = सच्च)
{
  std::string large_string(100000, 'a');
  c->start_serialize<वर्ण> ();

  unsigned coverage[अङ्कः_pair_pos_2];
  unsigned class_def_1[अङ्कः_pair_pos_2];
  unsigned class_def_2[अङ्कः_pair_pos_2];
  unsigned pair_pos_2[अङ्कः_pair_pos_2];

  unsigned* device_tables = (unsigned*) calloc (अङ्कः_pair_pos_2 * अङ्कः_class_1 * अङ्कः_class_2,
                                                परिमाण(unsigned));

  // Total glyphs = num_class_1 * num_pair_pos_2
  अत्थाय (पूर्णाङ्क i = अङ्कः_pair_pos_2 - 1; i >= 0; i--)
  {
    unsigned start_glyph = 5 + i * अङ्कः_class_1;
    सचे (अङ्कः_class_2 >= अङ्कः_class_1)
    {
      class_def_2[i] = add_class_def (11,
                                      10 + अङ्कः_class_2, c);
      class_def_1[i] = add_class_def (start_glyph + 1,
                                      start_glyph + अङ्कः_class_1,
                                      c);
    } अञ्ञथा {
      class_def_1[i] = add_class_def (start_glyph + 1,
                                      start_glyph + अङ्कः_class_1,
                                      c);
      class_def_2[i] = add_class_def (11,
                                      10 + अङ्कः_class_2, c);
    }

    coverage[i] = add_coverage (start_glyph,
                                start_glyph + अङ्कः_class_1 - 1,
                                c);

    सचे (with_device_tables)
    {
      अत्थाय(पूर्णाङ्क j = (i + 1) * अङ्कः_class_1 * अङ्कः_class_2 - 1;
          j >= i * अङ्कः_class_1 * अङ्कः_class_2;
          j--)
      {
        uint8_t table[] = {
          (uint8_t) ((j >> 8) & 0xFF),
          (uint8_t) (j & 0xFF),
        };
        device_tables[j] = add_object ((वर्ण*) table, 2, c);
      }
    }

    pair_pos_2[i] = add_pair_pos_2 (1 + i * अङ्कः_class_1,
                                    coverage[i],
                                    class_def_1[i], अङ्कः_class_1,
                                    class_def_2[i], अङ्कः_class_2,
                                    with_device_tables
                                    ? &device_tables[i * अङ्कः_class_1 * अङ्कः_class_2]
                                    : nullptr,
                                    c);
  }


  unsigned pair_pos_1 = 0;
  सचे (extra_table) pair_pos_1 = add_object (large_string.c_str(), 100000, c);

  सचे (as_extension) {
    अत्थाय (पूर्णाङ्क i = अङ्कः_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = add_extension (pair_pos_2[i], 2, c);

    सचे (extra_table)
      pair_pos_1 = add_extension (pair_pos_1, 2, c);
  }

  start_lookup (as_extension ? 9 : 2, 1 + अङ्कः_pair_pos_2, c);

  सचे (extra_table)
    add_offset (pair_pos_1, c);

  अत्थाय (पूर्णाङ्क i = 0; i < अङ्कः_pair_pos_2; i++)
    add_offset (pair_pos_2[i], c);

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();

  मुक्त (device_tables);
}

template<पूर्णाङ्क mark_गणनम्,
    पूर्णाङ्क class_गणनम्,
    पूर्णाङ्क base_गणनम्,
    पूर्णाङ्क table_गणनम्_2>
स्थिर शून्य
populate_serializer_with_large_mark_base_pos_1 (hb_serialize_context_t* c)
{
  c->start_serialize<वर्ण> ();

  MarkBasePosBuffers<mark_गणनम्, class_गणनम्, base_गणनम्, table_गणनम्_2> buffers (c);

  unsigned mark_base_pos[table_गणनम्_2];
  अत्थाय (unsigned i = 0; i < table_गणनम्_2; i++)
    mark_base_pos[i] = buffers.create_mark_base_pos_1 (i, c);

  अत्थाय (पूर्णाङ्क i = 0; i < table_गणनम्_2; i++)
    mark_base_pos[i] = add_extension (mark_base_pos[i], 4, c);

  start_lookup (9, table_गणनम्_2, c);

  अत्थाय (पूर्णाङ्क i = 0; i < table_गणनम्_2; i++)
    add_offset (mark_base_pos[i], c);

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<unsigned Lig_Subst_गणनम्,
         unsigned Liga_Set_गणनम्,
         unsigned Liga_Per_Set_गणनम्,
         unsigned LigaSize>
स्थिर शून्य populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    तार्किक sequential_liga_sets,
                                                    तार्किक shared,
                                                    unsigned *liga_subst_id,
                                                    तार्किक unique_lig_str = मिच्छा)
{
  वर्ण ch = 'a';

  unsigned liga[Liga_Set_गणनम् * Liga_Per_Set_गणनम्];
  unsigned liga_set[Liga_Set_गणनम्];
  अत्थाय (unsigned l = 0; l < Lig_Subst_गणनम्; l++) {
    unsigned coverage_start = 0;
    unsigned coverage_end = Liga_Set_गणनम् - 1;
    सचे (sequential_liga_sets) {
      coverage_start = l * Liga_Set_गणनम्;
      coverage_end = (l + 1) * Liga_Set_गणनम् - 1;
    }
    unsigned coverage = add_coverage(coverage_start, coverage_end, c, shared);

    अत्थाय (unsigned i = 0; i < Liga_Set_गणनम्; i++) {
      अत्थाय (unsigned j = 0; j < Liga_Per_Set_गणनम्; j++)
      {
        std::string large_string(100000, ch);
        start_object (large_string.c_str(), LigaSize, c);
        सचे (unique_lig_str)
          ch += 1;
        add_virtual_offset(coverage, c);
        liga[i * Liga_Per_Set_गणनम् + j] = c->निष्कासय_pack (shared);
      }

      add_liga_set_header(Liga_Per_Set_गणनम्, c);
      add_virtual_offset(coverage, c);
      अत्थाय (unsigned j = 0; j < Liga_Per_Set_गणनम्; j++)
      {
        add_offset(liga[i * Liga_Per_Set_गणनम् + j], c);
      }

      liga_set[i] = c->निष्कासय_pack(shared);
    }

    add_liga_header(coverage, Liga_Set_गणनम्, c);
    अत्थाय (unsigned i = 0; i < Liga_Set_गणनम्; i++)
    {
      add_offset(liga_set[i], c);
    }

    liga_subst_id[l] = c->निष्कासय_pack(shared);
  }

  अत्थाय (unsigned l = 0; l < Lig_Subst_गणनम्; l++) {
    liga_subst_id[l] = add_extension(liga_subst_id[l], 4, c);
  }
}

template<unsigned liga_subst_गणनम्,
         unsigned liga_set_गणनम्,
         unsigned liga_per_set_गणनम्,
         unsigned liga_size>
स्थिर शून्य
populate_serializer_with_large_liga (hb_serialize_context_t* c, तार्किक sequential_liga_sets)
{
  c->start_serialize<वर्ण> ();

  unsigned liga_subst[liga_subst_गणनम्];
  populate_serializer_with_large_ligsubst<liga_subst_गणनम्, liga_set_गणनम्, liga_per_set_गणनम्, liga_size>(c, sequential_liga_sets, मिच्छा, liga_subst);

  start_lookup (7, liga_subst_गणनम्, c);
  अत्थाय (unsigned l = 0; l < liga_subst_गणनम्; l++) {
    add_offset(liga_subst[l], c);
  }

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_large_liga_overlapping_clone_result (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->start_serialize<वर्ण> ();

  constexpr unsigned liga_size = 30000;

  unsigned liga[6];
  unsigned liga_subst[3];
  unsigned liga_set[2];

  // LigaSubst 3
  unsigned coverage = add_coverage(1, 1, c);
  अत्थाय (पूर्णाङ्क i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->निष्कासय_pack (मिच्छा);
  }

  add_liga_set_header(2, c);
  add_virtual_offset(coverage, c);
  अत्थाय (unsigned i = 0; i < 2; i++)
    add_offset(liga[i], c);
  liga_set[0] = c->निष्कासय_pack(मिच्छा);

  add_liga_header(coverage, 1, c);
  add_offset(liga_set[0], c);
  liga_subst[2] = c->निष्कासय_pack(मिच्छा);

  // LigaSubst 2
  coverage = add_coverage(0, 1, c);
  अत्थाय (पूर्णाङ्क i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->निष्कासय_pack (मिच्छा);
  }

  add_liga_set_header(1, c);
  add_virtual_offset(coverage, c);
  add_offset(liga[1], c);
  liga_set[1] = c->निष्कासय_pack(मिच्छा);

  add_liga_set_header(1, c);
  add_virtual_offset(coverage, c);
  add_offset(liga[0], c);
  liga_set[0] = c->निष्कासय_pack(मिच्छा);

  add_liga_header(coverage, 2, c);
  add_offset(liga_set[0], c);
  add_offset(liga_set[1], c);
  liga_subst[1] = c->निष्कासय_pack(मिच्छा);

  // LigaSubst 1
  coverage = add_coverage(0, 0, c);
  अत्थाय (पूर्णाङ्क i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->निष्कासय_pack (मिच्छा);
  }

  add_liga_set_header(2, c);
  add_virtual_offset(coverage, c);
  अत्थाय (unsigned i = 0; i < 2; i++)
    add_offset(liga[i], c);
  liga_set[0] = c->निष्कासय_pack(मिच्छा);

  add_liga_header(coverage, 1, c);
  add_offset(liga_set[0], c);
  liga_subst[0] = c->निष्कासय_pack(मिच्छा);

  अत्थाय (पूर्णाङ्क l = 2; l >= 0; l--) {
    liga_subst[l] = add_extension(liga_subst[l], 4, c);
  }

  start_lookup (7, 3, c);
  अत्थाय (unsigned l = 0; l < 3; l++) {
    add_offset(liga_subst[l], c);
  }

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<unsigned liga_subst_गणनम्,
         unsigned liga_set_गणनम्,
         unsigned liga_per_set_गणनम्,
         unsigned liga_size>
स्थिर शून्य
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, तार्किक sequential_liga_sets)
{
  c->start_serialize<वर्ण> ();

  unsigned lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  unsigned liga_subst_idx[liga_subst_गणनम्];
  populate_serializer_with_large_ligsubst<liga_subst_गणनम्, liga_set_गणनम्, liga_per_set_गणनम्, liga_size>(c, sequential_liga_sets, सच्च, liga_subst_idx, सच्च);

  start_lookup (7, liga_subst_गणनम्, c);
  अत्थाय (unsigned l = 0; l < liga_subst_गणनम्; l++) {
    add_offset(liga_subst_idx[l], c);
  }

  lookups[0] = finish_lookup (c);

  // Lookup with 2 LigSubst tables
  unsigned liga_subst[liga_subst_गणनम् + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, मिच्छा, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_गणनम्, liga_set_गणनम्, liga_per_set_गणनम्, liga_size>(c, sequential_liga_sets, सच्च, &liga_subst[1], सच्च);

  start_lookup (7, liga_subst_गणनम् + 1, c);
  अत्थाय (unsigned l = 0; l <= liga_subst_गणनम्; l++) {
    add_offset(liga_subst[l], c);
  }

  lookups[1] = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (lookups, 2, c);
  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<unsigned liga_subst_गणनम्,
         unsigned liga_set_गणनम्,
         unsigned liga_per_set_गणनम्,
         unsigned liga_size>
स्थिर शून्य
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->start_serialize<वर्ण> ();

  unsigned liga_subst[liga_subst_गणनम् + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, सच्च, सच्च, liga_subst, सच्च);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_गणनम्, liga_set_गणनम्, liga_per_set_गणनम्, liga_size>(c, सच्च, सच्च, &liga_subst[1], सच्च);

  start_lookup (7, liga_subst_गणनम् + 1, c);
  अत्थाय (unsigned l = 0; l <= liga_subst_गणनम्; l++) {
    add_offset(liga_subst[l], c);
  }

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);
  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

स्थिर शून्य test_sort_shortest ()
{
  size_t buffer_size = 100;
  शून्य* buffer_a = malloc (buffer_size);
  शून्य* buffer_e = malloc (buffer_size);
  hb_serialize_context_t a (buffer_a, buffer_size);
  hb_serialize_context_t e (buffer_e, buffer_size);
  populate_serializer_complex_2 (&a);

  graph_t graph = graph_t::create (a.object_graph ()).value ();
  hb_always_assert (graph.sort_shortest_distance ().is_ok());
  graph.normalize();

  // Expected graph
  e.start_serialize();
  unsigned jkl = add_object ("jkl", 3, &e);

  start_object("ghi", 3, &e);
  add_offset(jkl, &e);
  unsigned ghi = e.निष्कासय_pack(मिच्छा);

  start_object("def", 3, &e);
  add_offset(ghi, &e);
  unsigned def = e.निष्कासय_pack(मिच्छा);

  unsigned mn = add_object("mn", 2, &e);

  start_object ("abc", 3, &e);
  add_offset (def, &e);
  add_offset (jkl, &e);
  add_offset (mn, &e);
  e.निष्कासय_pack (मिच्छा);
  e.end_serialize();

  graph_t expected = graph_t::create (e.object_graph ()).value ();
  expected.normalize();

  प्रतिज्ञा(expected == graph);

  मुक्त (buffer_a);
  मुक्त (buffer_e);
}

स्थिर शून्य test_duplicate_leaf ()
{
  size_t buffer_size = 100;
  शून्य* buffer_a = malloc (buffer_size);
  शून्य* buffer_e = malloc (buffer_size);
  hb_serialize_context_t a (buffer_a, buffer_size);
  hb_serialize_context_t e (buffer_e, buffer_size);
  populate_serializer_complex_2 (&a);

  graph_t graph = graph_t::create (a.object_graph ()).value ();
  hb_always_assert (graph.duplicate (4, 1, मिच्छा).is_ok ());
  graph.normalize();

  e.start_serialize();
  unsigned mn = add_object("mn", 2, &e);
  unsigned jkl_2 = add_object("jkl", 3, &e);

  start_object("ghi", 3, &e);
  add_offset(jkl_2, &e);
  unsigned ghi = e.निष्कासय_pack(मिच्छा);

  start_object("def", 3, &e);
  add_offset(ghi, &e);
  unsigned def = e.निष्कासय_pack(मिच्छा);

  unsigned jkl_1 = add_object("jkl", 3, &e);

  start_object("abc", 3, &e);
  add_offset(def, &e);
  add_offset(jkl_1, &e);
  add_offset(mn, &e);
  e.निष्कासय_pack(मिच्छा);

  graph_t expected = graph_t::create (e.object_graph ()).value ();
  expected.normalize();

  प्रतिज्ञा(expected == graph);

  मुक्त (buffer_a);
  मुक्त (buffer_e);
}

स्थिर शून्य test_duplicate_interior ()
{
  size_t buffer_size = 100;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_3 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();
  hb_always_assert (graph.duplicate (3, 2, मिच्छा).is_ok ());

  hb_always_assert(strncmp (graph.object (6).head, "jkl", 3) == 0);
  hb_always_assert(graph.object (6).real_links.length == 1);
  hb_always_assert(graph.object (6).real_links[0].objidx == 0);

  hb_always_assert(strncmp (graph.object (5).head, "abc", 3) == 0);
  hb_always_assert(graph.object (5).real_links.length == 3);
  hb_always_assert(graph.object (5).real_links[0].objidx == 4);
  hb_always_assert(graph.object (5).real_links[1].objidx == 2);
  hb_always_assert(graph.object (5).real_links[2].objidx == 1);

  hb_always_assert(strncmp (graph.object (4).head, "def", 3) == 0);
  hb_always_assert(graph.object (4).real_links.length == 1);
  hb_always_assert(graph.object (4).real_links[0].objidx == 3);

  hb_always_assert(strncmp (graph.object (3).head, "ghi", 3) == 0);
  hb_always_assert(graph.object (3).real_links.length == 1);
  hb_always_assert(graph.object (3).real_links[0].objidx == 6);

  hb_always_assert(strncmp (graph.object (2).head, "jkl", 3) == 0);
  hb_always_assert(graph.object (2).real_links.length == 1);
  hb_always_assert(graph.object (2).real_links[0].objidx == 0);

  hb_always_assert(strncmp (graph.object (1).head, "mn", 2) == 0);
  hb_always_assert(graph.object (1).real_links.length == 0);

  hb_always_assert(strncmp (graph.object (0).head, "opqrst", 6) == 0);
  hb_always_assert(graph.object (0).real_links.length == 0);

  मुक्त (buffer);
}

स्थिर शून्य
test_serialize ()
{
  size_t buffer_size = 100;
  शून्य* buffer_1 = malloc (buffer_size);
  hb_serialize_context_t c1 (buffer_1, buffer_size);
  populate_serializer_simple (&c1);
  hb_bytes_t expected = c1.copy_bytes ();

  graph_t graph = graph_t::create (c1.object_graph ()).value ();
  hb_blob_t* out = graph::serialize (graph).value ();
  मुक्त (buffer_1);

  hb_bytes_t actual = out->as_bytes ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (out);
}

स्थिर शून्य test_will_overflow_1 ()
{
  size_t buffer_size = 100;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  populate_serializer_complex_2 (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (!graph::will_overflow (graph, nullptr).value ());

  मुक्त (buffer);
}

स्थिर शून्य test_will_overflow_2 ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (graph::will_overflow (graph, nullptr).value ());

  मुक्त (buffer);
}

स्थिर शून्य test_will_overflow_3 ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (graph::will_overflow (graph, nullptr).value ());

  मुक्त (buffer);
}

स्थिर शून्य test_resolve_overflows_via_sort ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == (80000 + 3 + 3 * 2));

  मुक्त (buffer);
  hb_blob_destroy (out);
}

स्थिर शून्य test_resolve_overflows_via_duplication ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  मुक्त (buffer);
  hb_blob_destroy (out);
}

स्थिर शून्य test_resolve_overflows_via_multiple_duplications ()
{
  size_t buffer_size = 300000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_multiple_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE, 5);
  hb_always_assert (out);

  मुक्त (buffer);
  hb_blob_destroy (out);
}

स्थिर शून्य test_resolve_overflows_via_space_assignment ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_spaces (&c, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_spaces (&e, मिच्छा);

  run_resolve_overflow_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_overflows_via_isolation ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_isolation_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (c.offset_overflow ());
  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  मुक्त (buffer);
  hb_blob_destroy (out);
}

स्थिर शून्य test_resolve_overflows_via_isolation_with_recursive_duplication ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_isolation_overflow_complex (&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_isolation_overflow_complex_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_overflows_via_isolating_16bit_space ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_spaces_16bit_connection (&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_spaces_16bit_connection_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_overflows_via_isolating_16bit_space_2 ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_short_and_wide_subgraph_मूलम् (&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_short_and_wide_subgraph_मूलम्_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_overflows_via_isolation_spaces ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_isolation_overflow_spaces (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (c.offset_overflow ());
  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();

  unsigned expected_length = 3 + 2 * 60000; // objects
  expected_length += 2 * 4 + 2 * 2; // links
  hb_always_assert (result.length == expected_length);

  मुक्त (buffer);
  hb_blob_destroy (out);
}

स्थिर शून्य test_resolve_mixed_overflows_via_isolation_spaces ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_24_and_32_bit_offsets (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (c.offset_overflow ());
  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();

  unsigned expected_length =
      // Objects
      7 +
      4 * 40000;

  expected_length +=
      // Links
      2 * 4 +  // 32
      4 * 3 +  // 24
      4 * 2;   // 16

  hb_always_assert (result.length == expected_length);

  मुक्त (buffer);
  hb_blob_destroy (out);
}

स्थिर शून्य test_resolve_with_extension_promotion ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             सच्च);
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_shared_extension_promotion ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c, 0, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3, सच्च);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             सच्च);
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

#ifndef HB_NO_BEYOND_64K
स्थिर शून्य test_resolve_with_extension_promotion_1_2 ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c, 0, मिच्छा, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3, मिच्छा, सच्च);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             सच्च);
  मुक्त (buffer);
  मुक्त (expected_buffer);
}
#endif

स्थिर शून्य test_resolve_with_repeated_extension_promotion ()
{
  size_t buffer_size = 1000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize<वर्ण> ();
  unsigned subtable = add_object ("subt", 4, &c);
  start_lookup (5, 2, &c);
  add_offset (subtable, &c);
  add_offset (subtable, &c);
  unsigned lookup = finish_lookup (&c);
  unsigned lookup_list = add_lookup_list (&lookup, 1, &c);
  add_gsubgpos_header (lookup_list, &c);
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();
  auto context = graph::gsubgpos_graph_context_t::create (HB_TAG ('G', 'S', 'U', 'B'),
                                                          graph).value ();
  hb_always_assert (context.lookups.get_population () == 1);
  unsigned lookup_index = *context.lookups.कुञ्चिकाः ();
  graph::Lookup* lookup_table = context.lookups.get (lookup_index);
  hb_always_assert (lookup_table->make_extension (context, lookup_index).is_ok ());

  ध्रुव auto& lookup_vertex = graph.vertices_[lookup_index];
  hb_always_assert (lookup_vertex.obj ().real_links.length == 2);
  unsigned extension_index = lookup_vertex.obj ().real_links[0].objidx;
  hb_always_assert (lookup_vertex.obj ().real_links[1].objidx == extension_index);

  auto* extension = graph.vertices_[extension_index].as<
      graph::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
  unsigned promoted_subtable = extension->get_subtable_index (graph, extension_index).value ();
  hb_always_assert (graph.vertices_[promoted_subtable].table_size () == 4);
  hb_always_assert (graph.update_parents ().is_ok ());
  hb_always_assert (graph.vertices_[extension_index].incoming_edges () == 2);
  hb_always_assert (graph.vertices_[promoted_subtable].incoming_edges () == 1);

  hb_blob_t* out = graph::serialize (graph).value ();
  hb_blob_destroy (out);
  मुक्त (buffer);
}

स्थिर शून्य test_resolve_with_basic_pair_pos_1_split ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, सच्च);

  run_resolve_overflow_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_extension_pair_pos_1_split ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, सच्च);

  run_resolve_overflow_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_basic_pair_pos_2_split ()
{
  size_t buffer_size = 300000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, सच्च);

  run_resolve_overflow_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_close_to_limit_pair_pos_2_split ()
{
  size_t buffer_size = 300000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, सच्च, मिच्छा, मिच्छा);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, सच्च, मिच्छा, मिच्छा);

  run_resolve_overflow_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_pair_pos_2_split_with_device_tables ()
{
  size_t buffer_size = 300000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, मिच्छा, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, सच्च, सच्च);

  run_resolve_overflow_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_basic_mark_base_pos_1_split ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_mark_base_pos_1 <40, 10, 110, 1>(&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_mark_base_pos_1 <40, 10, 110, 2>(&e);

  run_resolve_overflow_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_basic_liga_split ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, मिच्छा);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, मिच्छा);

  run_resolve_overflow_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_liga_split_move ()
{
  size_t buffer_size = 400000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, सच्च);

  run_resolve_overflow_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_liga_split_overlapping_clone ()
{
  size_t buffer_size = 400000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga_overlapping_clone_result(&e);

  run_resolve_overflow_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_liga_split_shared_table ()
{
  size_t buffer_size = 400000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, सच्च);

  run_resolve_overflow_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_liga_split_shared_coverage ()
{
  size_t buffer_size = 400000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  run_resolve_overflow_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             सच्च,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_overflows_via_splitting_spaces ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_split_spaces (&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_split_spaces_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  मुक्त (buffer);
  मुक्त (expected_buffer);

}

स्थिर शून्य test_resolve_overflows_via_splitting_spaces_2 ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_split_spaces_2 (&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_split_spaces_expected_2 (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_overflows_via_priority ()
{
  size_t buffer_size = 160000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_priority_overflow (&c);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_priority_overflow_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  मुक्त (buffer);
  मुक्त (expected_buffer);
}


स्थिर शून्य test_virtual_link ()
{
  size_t buffer_size = 100;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_virtual_link (&c);

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE);
  hb_always_assert (out);

  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == 5 + 4 * 2);
  hb_always_assert (result[0]  == 'a');
  hb_always_assert (result[5]  == 'c');
  hb_always_assert (result[8]  == 'e');
  hb_always_assert (result[9]  == 'b');
  hb_always_assert (result[12] == 'd');

  मुक्त (buffer);
  hb_blob_destroy (out);
}

स्थिर शून्य
test_shared_ग्रन्थिः_with_virtual_links ()
{
  size_t buffer_size = 100;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize<वर्ण> ();

  unsigned obj_b = add_object ("b", 1, &c);
  unsigned obj_c = add_object ("c", 1, &c);

  start_object ("d", 1, &c);
  add_virtual_offset (obj_b, &c);
  unsigned obj_d_1 = c.निष्कासय_pack ();

  start_object ("d", 1, &c);
  add_virtual_offset (obj_c, &c);
  unsigned obj_d_2 = c.निष्कासय_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  start_object ("a", 1, &c);
  add_offset (obj_b, &c);
  add_offset (obj_c, &c);
  add_offset (obj_d_1, &c);
  add_offset (obj_d_2, &c);
  c.निष्कासय_pack ();
  c.end_serialize ();

  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links.length == 2);
  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links[1].objidx == obj_c);
  मुक्त(buffer);
}

स्थिर शून्य
test_repack_last ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_repack_last (&c, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_repack_last (&e, मिच्छा);

  run_resolve_overflow_test ("test_repack_last",
                             c,
                             e,
                             20,
                             मिच्छा,
                             HB_TAG('a', 'b', 'c', 'd'),
                             सच्च);

  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य
test_dont_duplicate_virtual ()
{
  size_t buffer_size = 200000;
  शून्य* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_virtual (&c, सच्च);

  शून्य* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_virtual (&e, मिच्छा);

  run_resolve_overflow_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             मिच्छा,
                             HB_TAG('a', 'b', 'c', 'd'),
                             सच्च);

  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य
test_deep_graph_traversal ()
{
  size_t buffer_size = 1000000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize ();
  unsigned prev = add_object ("leaf", 4, &c);
  अत्थाय (unsigned i = 0; i < 2000; i++)
  {
    start_object ("node", 4, &c);
    add_offset (prev, &c);
    prev = c.निष्कासय_pack (मिच्छा);
  }
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // Test find_subgraph (set)
  hb_set_t visited_set;
  hb_always_assert (graph.find_subgraph (graph.मूलम्_idx (), visited_set).is_ok ());
  hb_always_assert (visited_set.get_population () == 2001);

  // Test find_subgraph_size
  hb_set_t size_set;
  size_t sz = graph.find_subgraph_size (graph.मूलम्_idx (), size_set).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.set (graph.मूलम्_idx (), 1);
  hb_always_assert (graph.find_subgraph (graph.मूलम्_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (graph.assign_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t index_map;
  hb_always_assert (graph.duplicate_subgraph (graph.मूलम्_idx (), index_map).is_ok ());
  hb_always_assert (index_map.get_population () == 2001);

  मुक्त (buffer);
}

स्थिर शून्य
test_32bit_roots_traversal ()
{
  size_t buffer_size = 1000;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize ();
  unsigned leaf1 = add_object ("l1", 2, &c);
  unsigned leaf2 = add_object ("l2", 2, &c);

  start_object ("mid", 3, &c);
  add_wide_offset (leaf1, &c);
  unsigned mid = c.निष्कासय_pack (मिच्छा);

  start_object ("root", 4, &c);
  add_offset (mid, &c);
  add_wide_offset (leaf2, &c);
  c.निष्कासय_pack (मिच्छा);
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_set_t roots;
  hb_always_assert (graph.find_32bit_roots (graph.मूलम्_idx (), roots).is_ok ());
  hb_always_assert (roots.has (leaf1 - 1));
  hb_always_assert (roots.has (leaf2 - 1));
  hb_always_assert (!roots.has (mid - 1));
  hb_always_assert (roots.get_population () == 2);

  मुक्त (buffer);

  // Test case where DFS fails but BFS succeeds:
  // root -16-> a
  // root -32-> a
  // a -32-> b
  //
  // DFS will first explore the 16 bit link to a and then recurse the 32 bit link from a to b
  // and consider b a root. However, b should not be a root since it's not a top level 32 bit link.
  // BFS will handle this correctly by discovering a as a top level root and then avoiding further
  // exploring it.
  buffer_size = 1000;
  buffer = malloc (buffer_size);
  hb_serialize_context_t c2 (buffer, buffer_size);

  c2.start_serialize ();
  unsigned b = add_object ("b", 1, &c2);

  start_object ("a", 1, &c2);
  add_wide_offset (b, &c2);
  unsigned a = c2.निष्कासय_pack (मिच्छा);

  start_object ("r", 1, &c2);
  add_offset (a, &c2);
  add_wide_offset (a, &c2);
  c2.निष्कासय_pack (मिच्छा);

  c2.end_serialize ();

  graph_t graph2 = graph_t::create (c2.object_graph ()).value ();

  hb_set_t roots2;
  hb_always_assert (graph2.find_32bit_roots (graph2.मूलम्_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  मुक्त (buffer);
}

स्थिर शून्य test_invalid_link_objidx ()
{
  size_t buffer_size = 100;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_2 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // Poison a link to point to (unsigned) -1
  auto links = graph.vertices_[graph.मूलम्_idx ()].real_links_writer();
  links->objidx = (unsigned) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!graph.sort_shortest_distance ().is_ok ());

  मुक्त (buffer);
}

स्थिर शून्य test_invalid_link_add_and_move ()
{
  size_t buffer_size = 100;
  शून्य* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_2 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!graph.add_link (&dummy, 0, (unsigned) -1).is_ok ());

  // move_child with non-existent offset
  auto res = graph.move_child (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  मुक्त (buffer);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  test_invalid_link_objidx ();
  test_invalid_link_add_and_move ();
  test_serialize ();
  test_sort_shortest ();
  test_will_overflow_1 ();
  test_will_overflow_2 ();
  test_will_overflow_3 ();
  test_resolve_overflows_via_sort ();
  test_resolve_overflows_via_duplication ();
  test_resolve_overflows_via_multiple_duplications ();
  test_resolve_overflows_via_priority ();
  test_resolve_overflows_via_space_assignment ();
  test_resolve_overflows_via_isolation ();
  test_resolve_overflows_via_isolation_with_recursive_duplication ();
  test_resolve_overflows_via_isolation_spaces ();
  test_resolve_overflows_via_isolating_16bit_space ();
  test_resolve_overflows_via_isolating_16bit_space_2 ();
  test_resolve_overflows_via_splitting_spaces ();
  test_resolve_overflows_via_splitting_spaces_2 ();
  test_resolve_mixed_overflows_via_isolation_spaces ();
  test_duplicate_leaf ();
  test_duplicate_interior ();
  test_virtual_link ();
  test_repack_last();
  test_shared_ग्रन्थिः_with_virtual_links ();
  test_resolve_with_extension_promotion ();
  test_resolve_with_shared_extension_promotion ();
#ifndef HB_NO_BEYOND_64K
  test_resolve_with_extension_promotion_1_2 ();
#endif
  test_resolve_with_repeated_extension_promotion ();
  test_resolve_with_basic_pair_pos_1_split ();
  test_resolve_with_extension_pair_pos_1_split ();
  test_resolve_with_basic_pair_pos_2_split ();
  test_resolve_with_pair_pos_2_split_with_device_tables ();
  test_resolve_with_close_to_limit_pair_pos_2_split ();
  test_resolve_with_basic_mark_base_pos_1_split ();
  test_resolve_with_basic_liga_split ();
  test_resolve_with_liga_split_move ();
  test_dont_duplicate_virtual ();
  test_resolve_with_liga_split_overlapping_clone ();
  test_resolve_with_liga_split_shared_table ();
  test_resolve_with_liga_split_shared_coverage ();
  test_deep_graph_traversal ();
  test_32bit_roots_traversal ();

  // TODO(grieger): have run overflow tests compare graph equality not final packed binary.
  // TODO(grieger): split test where multiple subtables in one lookup are split to test link ordering.
  // TODO(grieger): split test where coverage table in subtable that is being split is shared.
  // TODO(grieger): test with extensions already mixed in as well.
  // TODO(grieger): test two layer ext promotion setup.
  // TODO(grieger): test sorting by subtables per byte in ext. promotion.
}
