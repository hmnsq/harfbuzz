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

स्थिर शून्य extend (स्थिरांक अक्षर* value,
                    अचिन्हित len,
                    hb_serialize_context_t* c)
{
  अक्षर* obj = c->allocate_size<अक्षर> (len);
  hb_memcpy (obj, value, len);
}

स्थिर शून्य start_object(स्थिरांक अक्षर* tag,
                         अचिन्हित len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (tag, len, c);
}

स्थिर अचिन्हित add_object(स्थिरांक अक्षर* tag,
                           अचिन्हित len,
                           hb_serialize_context_t* c,
                           तार्किक shared = असत्य)
{
  start_object (tag, len, c);
  परत c->काढा_pack (shared);
}


स्थिर शून्य add_offset (अचिन्हित id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* offset = c->start_embed<OT::Offset16> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

स्थिर शून्य add_24_offset (अचिन्हित id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* offset = c->start_embed<OT::Offset24> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

स्थिर शून्य add_wide_offset (अचिन्हित id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* offset = c->start_embed<OT::Offset32> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

स्थिर शून्य add_gsubgpos_header (अचिन्हित lookup_list,
                                 hb_serialize_context_t* c)
{
  अक्षर header[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  start_object (header, 8, c);
  add_offset (lookup_list, c);
  c->काढा_pack (असत्य);
}

स्थिर शून्य add_gsubgpos_header_1_2 (अचिन्हित lookup_list,
                                     hb_serialize_context_t* c)
{
  अक्षर header[] = {
    0, 1, 0, 2, // version
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  start_object (header, आकार (header), c);
  add_wide_offset (lookup_list, c);
  c->काढा_pack (असत्य);
}

स्थिर अचिन्हित add_lookup_list (स्थिरांक अचिन्हित* lookups,
                                 अक्षर मोजणी,
                                 hb_serialize_context_t* c)
{
  अक्षर lookup_मोजणी[] = {0, मोजणी};
  start_object  ((अक्षर *) &lookup_मोजणी, 2, c);

  साठी (पूर्णांक i = 0; i < मोजणी; i++)
    add_offset (lookups[i], c);

  परत c->काढा_pack (असत्य);
}

स्थिर अचिन्हित add_lookup_list_1_2 (स्थिरांक अचिन्हित* lookups,
                                     अक्षर मोजणी,
                                     hb_serialize_context_t* c)
{
  अक्षर lookup_मोजणी[] = {0, मोजणी};
  start_object ((अक्षर *) &lookup_मोजणी, 2, c);

  साठी (पूर्णांक i = 0; i < मोजणी; i++)
    add_wide_offset (lookups[i], c);

  परत c->काढा_pack (असत्य);
}

स्थिर शून्य start_lookup (int8_t type,
                          int8_t अंक_subtables,
                          hb_serialize_context_t* c)
{
  अक्षर lookup[] = {
    0, (अक्षर)type, // type
    0, (अक्षर)2, // flag
    0, (अक्षर)अंक_subtables, // num subtables
  };

  start_object (lookup, 6, c);
}

स्थिर अचिन्हित finish_lookup (hb_serialize_context_t* c)
{
  अक्षर filter[] = {0, 0};
  extend (filter, 2, c);
  परत c->काढा_pack (असत्य);
}

स्थिर अचिन्हित add_extension (अचिन्हित child,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  अक्षर ext[] = {
    0, 1,
    0, (अक्षर) type,
  };

  start_object (ext, 4, c);
  add_wide_offset (child, c);

  परत c->काढा_pack (असत्य);

}

// Adds coverage table fro [start, end]
स्थिर अचिन्हित add_coverage (अचिन्हित start, अचिन्हित end,
                              hb_serialize_context_t* c,
                              तार्किक shared = असत्य)
{
  जर (end - start == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //मोजणी

      (uint8_t) ((start >> 8) & 0xFF),
      (uint8_t) (start & 0xFF), // glyph[0]
    };
    परत add_object ((अक्षर*) coverage, 6, c, shared);
  }

  जर (end - start == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //मोजणी

      (uint8_t) ((start >> 8) & 0xFF),
      (uint8_t) (start & 0xFF), // glyph[0]

      (uint8_t) ((end >> 8) & 0xFF),
      (uint8_t) (end & 0xFF), // glyph[1]
    };
    परत add_object ((अक्षर*) coverage, 8, c, shared);
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
  परत add_object ((अक्षर*) coverage, 10, c, shared);
}


ढाँचा<typename It>
स्थिर अचिन्हित add_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  परत c->काढा_pack (असत्य);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
स्थिर अचिन्हित add_class_def (uint16_t start_glyph,
                               uint16_t end_glyph,
                               hb_serialize_context_t* c)
{
  अचिन्हित मोजणी = end_glyph - start_glyph;
  uint8_t header[] = {
    0, 1, // format

    (uint8_t) ((start_glyph >> 8) & 0xFF),
    (uint8_t) (start_glyph & 0xFF), // start_glyph

    (uint8_t) ((मोजणी >> 8) & 0xFF),
    (uint8_t) (मोजणी & 0xFF), //मोजणी
  };

  start_object ((अक्षर*) header, 6, c);
  साठी (uint16_t i = 1; i <= मोजणी; i++)
  {
    uint8_t class_मूल्य_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //मोजणी
    };
    extend ((अक्षर*) class_मूल्य_2, 2, c);
  }

  परत c->काढा_pack (असत्य);
}

स्थिर अचिन्हित add_pair_pos_1 (अचिन्हित* pair_sets,
                                अक्षर मोजणी,
                                अचिन्हित coverage,
                                hb_serialize_context_t* c)
{
  अक्षर format[] = {
    0, 1
  };

  start_object (format, 2, c);
  add_offset (coverage, c);

  अक्षर मूल्य_format[] = {
    0, 0,
    0, 0,
    0, मोजणी,
  };
  extend (मूल्य_format, 6, c);

  साठी (अक्षर i = 0; i < मोजणी; i++)
    add_offset (pair_sets[(अचिन्हित) i], c);

  परत c->काढा_pack (असत्य);
}

स्थिर शून्य add_liga_set_header (अक्षर liga_मोजणी,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_मोजणी_bytes[] = {
    (uint8_t) (0xFF & (liga_मोजणी >> 8)),
    (uint8_t) (0xFF & liga_मोजणी),
  };

  start_object ((स्थिरांक अक्षर*) liga_मोजणी_bytes, 2, c);
}

स्थिर शून्य add_liga_header (अचिन्हित coverage, अचिन्हित liga_set_मोजणी, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  start_object ((स्थिरांक अक्षर*) format, 2, c);
  add_offset(coverage, c);

  uint8_t liga_set_मोजणी_bytes[] = {
    (uint8_t) (0xFF & (liga_set_मोजणी >> 8)),
    (uint8_t) (0xFF & liga_set_मोजणी),
  };

  extend((स्थिरांक अक्षर *) liga_set_मोजणी_bytes, 2, c);
}

स्थिर अचिन्हित add_pair_pos_2 (अचिन्हित starting_class,
                                अचिन्हित coverage,
                                अचिन्हित class_def_1, uint16_t class_def_1_मोजणी,
                                अचिन्हित class_def_2, uint16_t class_def_2_मोजणी,
                                अचिन्हित* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  start_object ((अक्षर*) format, 2, c);
  add_offset (coverage, c);

  अचिन्हित अंक_मूल्ये_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  जर (device_tables) {
    format2 |= 0x20;
    अंक_मूल्ये_3 += 1;
  }
  uint8_t मूल्य_format[] = {
    0, format1,
    0, format2,
  };

  extend ((अक्षर*) मूल्य_format, 4, c);

  add_offset (class_def_1, c);
  add_offset (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_मोजणी >> 8) & 0xFF),
    (uint8_t) (class_def_1_मोजणी & 0xFF),
    (uint8_t) ((class_def_2_मोजणी >> 8) & 0xFF),
    (uint8_t) (class_def_2_मोजणी & 0xFF),
  };
  extend ((अक्षर*) class_counts, 4, c);

  अचिन्हित अंक_bytes_per_record = class_def_2_मोजणी * अंक_मूल्ये_3 * 2;
  uint8_t* record = (uint8_t*) सुस्मृति (1, अंक_bytes_per_record);
  पूर्णांक device_index = 0;
  साठी (uint16_t i = 0; i < class_def_1_मोजणी; i++)
  {

    साठी (uint16_t j = 0; j < class_def_2_मोजणी; j++)
    {
      साठी (पूर्णांक k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((अक्षर*) value, 2, c);
      }

      जर (device_tables) {
        add_offset (device_tables[device_index++], c);
      }
    }
  }
  मुक्त (record);

  परत c->काढा_pack (असत्य);
}

स्थिर अचिन्हित add_mark_base_pos_1 (अचिन्हित mark_coverage,
                                     अचिन्हित base_coverage,
                                     अचिन्हित mark_array,
                                     अचिन्हित base_array,
                                     अचिन्हित class_मोजणी,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  start_object ((अक्षर*) format, 2, c);
  add_offset (mark_coverage, c);
  add_offset (base_coverage, c);

  uint8_t मोजणी[] = {
    (uint8_t) ((class_मोजणी >> 8) & 0xFF),
    (uint8_t) (class_मोजणी & 0xFF),
  };
  extend ((अक्षर*) मोजणी, 2, c);

  add_offset (mark_array, c);
  add_offset (base_array, c);

  परत c->काढा_pack (असत्य);
}

ढाँचा<पूर्णांक mark_मोजणी,
    पूर्णांक class_मोजणी,
    पूर्णांक base_मोजणी,
    पूर्णांक table_मोजणी_2>
संरचना MarkBasePosBuffers
{
  अचिन्हित base_anchors[class_मोजणी * base_मोजणी];
  अचिन्हित mark_anchors[mark_मोजणी];
  uint8_t anchor_buffers[class_मोजणी * base_मोजणी + 100];
  uint8_t class_buffer[class_मोजणी * 2];

  MarkBasePosBuffers(hb_serialize_context_t* c)
  {
    साठी (अचिन्हित i = 0; i < आकार(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    साठी (अचिन्हित i = 0; i < class_मोजणी * base_मोजणी; i++)
    {
      base_anchors[i] = add_object ((अक्षर*) &anchor_buffers[i], 100, c);
      जर (i < class_मोजणी) {
        class_buffer[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_buffer[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    साठी (अचिन्हित i = 0; i < mark_मोजणी; i++)
    {
      mark_anchors[i] = add_object ((अक्षर*) &anchor_buffers[i], 4, c);
    }
  }

  अचिन्हित create_mark_base_pos_1 (अचिन्हित table_index, hb_serialize_context_t* c)
  {
    अचिन्हित class_per_table = class_मोजणी / table_मोजणी_2;
    अचिन्हित mark_per_class = mark_मोजणी / class_मोजणी;
    अचिन्हित start_class = class_per_table * table_index;
    अचिन्हित end_class = class_per_table * (table_index + 1) - 1;

    // baseArray
    uint8_t base_मोजणी_buffer[] = {
      (uint8_t) ((base_मोजणी >> 8) & 0xFF),
      (uint8_t) (base_मोजणी & 0xFF),

    };
    start_object ((अक्षर*) base_मोजणी_buffer, 2, c);
    साठी (अचिन्हित base = 0; base < base_मोजणी; base++)
    {
      साठी (अचिन्हित klass = start_class; klass <= end_class; klass++)
      {
        अचिन्हित i = base * class_मोजणी + klass;
        add_offset (base_anchors[i], c);
      }
    }
    अचिन्हित base_array = c->काढा_pack (असत्य);

    // markArray
    अचिन्हित अंक_marks = class_per_table * mark_per_class;
    uint8_t mark_मोजणी_buffer[] = {
      (uint8_t) ((अंक_marks >> 8) & 0xFF),
      (uint8_t) (अंक_marks & 0xFF),
    };
    start_object ((अक्षर*) mark_मोजणी_buffer, 2, c);
    साठी (अचिन्हित mark = 0; mark < mark_मोजणी; mark++)
    {
      अचिन्हित klass = mark % class_मोजणी;
      जर (klass < start_class || klass > end_class) सुरूठेवा;
      klass -= start_class;

      extend ((अक्षर*) &class_buffer[2 * klass], 2, c);
      add_offset (mark_anchors[mark], c);
    }
    अचिन्हित mark_array = c->काढा_pack (असत्य);

    // markCoverage
    स्वतः it =
        + hb_range ((hb_codepoint_t) mark_मोजणी)
        | hb_filter ([&] (hb_codepoint_t mark) {
          अचिन्हित klass = mark % class_मोजणी;
          परत klass >= class_per_table * table_index &&
              klass < class_per_table * (table_index + 1);
        })
        ;
    अचिन्हित mark_coverage = add_coverage (it, c);

    // baseCoverage
    अचिन्हित base_coverage = add_coverage (10, 10 + base_मोजणी - 1, c);

    परत add_mark_base_pos_1 (mark_coverage,
                                base_coverage,
                                mark_array,
                                base_array,
                                class_per_table,
                                c);
  }
};

स्थिर शून्य run_resolve_overflow_test (स्थिरांक अक्षर* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       अचिन्हित अंक_iterations = 0,
                                       तार्किक recalculate_extensions = असत्य,
                                       hb_tag_t tag = HB_TAG ('G', 'S', 'U', 'B'),
                                       तार्किक check_द्विमान_equivalence = असत्य)
{
  म_लिखो (">>> Testing overflowing resolution for %s\n",
          name);

  graph_t graph = graph_t::create (overflowing.object_graph ()).value ();

  graph_t expected_graph = graph_t::create (expected.object_graph ()).value ();
  जर (graph::will_overflow (expected_graph).value ())
  {
    जर (check_द्विमान_equivalence) {
      म_लिखो("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_द्विमान_equivalence);
    }
    hb_always_assert (expected_graph.assign_spaces ().is_ok ());
    hb_always_assert (expected_graph.sort_shortest_distance ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.offset_overflow ());
  स्वतः r = hb_resolve_graph_overflows (tag,
                                      अंक_iterations,
                                      recalculate_extensions,
                                      graph);
  जर (r.is_err())
    म_लिखो("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", graph::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = graph::serialize (graph).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = graph::serialize (expected_graph).value ();
  hb_always_assert (out2);
  जर (check_द्विमान_equivalence) {
    अचिन्हित l1, l2;
    स्थिरांक अक्षर* d1 = hb_blob_get_data(out1, &l1);
    स्थिरांक अक्षर* d2 = hb_blob_get_data(out2, &l2);

    तार्किक match = (l1 == l2) && (स_भेद(d1, d2, l1) == 0);
    जर (!match) {
      म_लिखो("## Result:\n");
      graph.print();
      म_लिखो("## Expected:\n");
      expected_graph.print();
      hb_always_assert(match);
    }
  }

  hb_blob_destroy (out1);
  hb_blob_destroy (out2);

  // Check the graphs are equivalent
  graph.normalize ();
  expected_graph.normalize ();
  जर (!(graph == expected_graph)) {
    म_लिखो("## Expected:\n");
    expected_graph.print();
    म_लिखो("## Result:\n");
    graph.print();
  }
  hb_always_assert (graph == expected_graph);
}

स्थिर शून्य add_virtual_offset (अचिन्हित id,
                                hb_serialize_context_t* c)
{
  c->add_virtual_link (id);
}

स्थिर शून्य
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_1 = add_object ("ghi", 3, c);
  अचिन्हित obj_2 = add_object ("def", 3, c);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_virtual (hb_serialize_context_t* c, तार्किक with_overflow)
{
  std::string large_string(50000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_4, obj_5;
  जर (with_overflow) {
    obj_5 = add_object("55555", 5, c);
    obj_4 = add_object(large_string.c_str(), 50000, c);
  } अन्यथा {
    obj_4 = add_object(large_string.c_str(), 50000, c);
    obj_5 = add_object("55555", 5, c);
  }

  start_object(large_string.c_str(), 20000, c);
  add_offset(obj_5, c);
  अचिन्हित obj_3 = c->काढा_pack(असत्य);

  start_object("2", 2, c);
  add_virtual_offset(obj_5, c);
  अचिन्हित obj_2 = c->काढा_pack(असत्य);

  // obj 1
  start_object("1", 1, c);
  add_offset(obj_2, c);
  add_offset(obj_3, c);
  add_offset(obj_4, c);
  c->काढा_pack(असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_overflow (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_1 = add_object (large_string.c_str(), 10000, c);
  अचिन्हित obj_2 = add_object (large_string.c_str(), 20000, c);
  अचिन्हित obj_3 = add_object (large_string.c_str(), 50000, c);

  start_object ("abc", 3, c);
  add_offset (obj_3, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_priority_overflow (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_e = add_object ("e", 1, c);
  अचिन्हित obj_d = add_object ("d", 1, c);

  start_object (large_string.c_str (), 50000, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object (large_string.c_str (), 20000, c);
  add_offset (obj_d, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_priority_overflow_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 50000, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  अचिन्हित obj_d = add_object ("d", 1, c);

  start_object (large_string.c_str (), 20000, c);
  add_offset (obj_d, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}


स्थिर शून्य
populate_serializer_with_dedup_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_1 = add_object ("def", 3, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_1, c);
  अचिन्हित obj_2 = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_multiple_dedup_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित leaf = add_object("def", 3, c);

  constexpr अचिन्हित अंक_mid_nodes = 20;
  अचिन्हित mid_nodes[अंक_mid_nodes];
  साठी (अचिन्हित i = 0; i < अंक_mid_nodes; i++) {
    start_object(large_string.c_str(), 10000 + i, c);
    add_offset(leaf, c);
    mid_nodes[i] = c->काढा_pack(असत्य);
  }

  start_object("abc", 3, c);
  साठी (अचिन्हित i = 0; i < अंक_mid_nodes; i++) {
    add_wide_offset(mid_nodes[i], c);
  }
  c->काढा_pack(असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_isolation_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_4 = add_object ("4", 1, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_4, c);
  अचिन्हित obj_3 = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_4, c);
  अचिन्हित obj_2 = c->काढा_pack (असत्य);

  start_object ("1", 1, c);
  add_wide_offset (obj_3, c);
  add_offset (obj_2, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_isolation_overflow_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_f = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object ("d", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d, c);
  अचिन्हित obj_h = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_c, c);
  add_offset (obj_h, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_d, c);
  अचिन्हित obj_g = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 11000, c);
  add_offset (obj_d, c);
  अचिन्हित obj_i = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_offset (obj_g, c);
  add_offset (obj_i, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_isolation_overflow_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();


  // space 1

  अचिन्हित obj_f_मूळसंख्या = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f_मूळसंख्या, c);
  अचिन्हित obj_e_मूळसंख्या = c->काढा_pack (असत्य);

  start_object ("d", 1, c);
  add_offset (obj_e_मूळसंख्या, c);
  अचिन्हित obj_d_मूळसंख्या = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d_मूळसंख्या, c);
  अचिन्हित obj_h = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_e_मूळसंख्या, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_c, c);
  add_offset (obj_h, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  // space 0

  अचिन्हित obj_f = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);


  start_object ("d", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 11000, c);
  add_offset (obj_d, c);
  अचिन्हित obj_i = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_d, c);
  अचिन्हित obj_g = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_offset (obj_g, c);
  add_offset (obj_i, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_isolation_overflow_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_d = add_object ("f", 1, c);
  अचिन्हित obj_e = add_object ("f", 1, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d, c);
  अचिन्हित obj_b = c->काढा_pack ();

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack ();


  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->काढा_pack ();

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_repack_last (hb_serialize_context_t* c, तार्किक with_overflow)
{
  std::string large_string(70000, 'c');
  c->start_serialize<अक्षर> ();
  c->push();

  // Obj E
  अचिन्हित obj_e_1, obj_e_2;
  जर (with_overflow) {
    obj_e_1 = add_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } अन्यथा {
    obj_e_2 = add_object("a", 1, c);
  }

  // Obj D
  c->push();
  add_offset(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  अचिन्हित obj_d = c->काढा_pack(असत्य);

  add_offset(obj_d, c);
  hb_always_assert(c->last_added_child_index() == obj_d);

  जर (!with_overflow) {
    obj_e_1 = add_object("a", 1, c);
  }

  // Obj C
  c->push();
  add_offset(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  अचिन्हित obj_c = c->काढा_pack(असत्य);

  add_offset(obj_c, c);

  // Obj B
  अचिन्हित obj_b = add_object("b", 1, c);
  add_offset(obj_b, c);

  // Obj A
  c->repack_last(obj_d);
  c->काढा_pack(असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_spaces (hb_serialize_context_t* c, तार्किक with_overflow)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_i;

  जर (with_overflow)
    obj_i = add_object ("i", 1, c);

  // Space 2
  अचिन्हित obj_h = add_object ("h", 1, c);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_h, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  // Space 1
  जर (!with_overflow)
    obj_i = add_object ("i", 1, c);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_i, c);
  अचिन्हित obj_g = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_i, c);
  अचिन्हित obj_f = c->काढा_pack (असत्य);

  start_object ("d", 1, c);
  add_offset (obj_g, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_f, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_g = add_object ("g", 1, c);
  अचिन्हित obj_h = add_object ("h", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_h, c);
  अचिन्हित obj_f = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object ("d", 1, c);
  add_offset (obj_f, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  add_offset (obj_h, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_g_मूळसंख्या = add_object ("g", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g_मूळसंख्या, c);
  अचिन्हित obj_e_मूळसंख्या = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_e_मूळसंख्या, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  अचिन्हित obj_h_मूळसंख्या = add_object ("h", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_h_मूळसंख्या, c);
  अचिन्हित obj_f = c->काढा_pack (असत्य);

  start_object ("d", 1, c);
  add_offset (obj_f, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  अचिन्हित obj_g = add_object ("g", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  अचिन्हित obj_h = add_object ("h", 1, c);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  add_offset (obj_h, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->काढा_pack (असत्य);

  c->end_serialize ();
}

स्थिर शून्य
populate_serializer_short_and_wide_subgraph_मूळ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_c, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object ("b", 1, c);
  add_offset (obj_c, c);
  add_offset (obj_e, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_short_and_wide_subgraph_मूळ_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_e_मूळसंख्या = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e_मूळसंख्या, c);
  अचिन्हित obj_c_मूळसंख्या = c->काढा_pack (असत्य);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_c_मूळसंख्या, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  अचिन्हित obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);


  start_object ("b", 1, c);
  add_offset (obj_c, c);
  add_offset (obj_e, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c_मूळसंख्या, c);
  add_wide_offset (obj_d, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_split_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_split_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_split_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_f_मूळसंख्या = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_मूळसंख्या, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  अचिन्हित obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_split_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->start_serialize<अक्षर> ();

  // Space 2

  अचिन्हित obj_f_double_मूळसंख्या = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_double_मूळसंख्या, c);
  अचिन्हित obj_d_मूळसंख्या = c->काढा_pack (असत्य);

  start_object ("b", 1, c);
  add_offset (obj_d_मूळसंख्या, c);
  अचिन्हित obj_b_मूळसंख्या = c->काढा_pack (असत्य);

  // Space 1

  अचिन्हित obj_f_मूळसंख्या = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_मूळसंख्या, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  // Space 0

  अचिन्हित obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  //मूळ
  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_b_मूळसंख्या, c);
  add_wide_offset (obj_c, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_5 = add_object ("mn", 2, c);

  अचिन्हित obj_4 = add_object ("jkl", 3, c);

  start_object ("ghi", 3, c);
  add_offset (obj_4, c);
  अचिन्हित obj_3 = c->काढा_pack (असत्य);

  start_object ("def", 3, c);
  add_offset (obj_3, c);
  अचिन्हित obj_2 = c->काढा_pack (असत्य);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_4, c);
  add_offset (obj_5, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_6 = add_object ("opqrst", 6, c);

  अचिन्हित obj_5 = add_object ("mn", 2, c);

  start_object ("jkl", 3, c);
  add_offset (obj_6, c);
  अचिन्हित obj_4 = c->काढा_pack (असत्य);

  start_object ("ghi", 3, c);
  add_offset (obj_4, c);
  अचिन्हित obj_3 = c->काढा_pack (असत्य);

  start_object ("def", 3, c);
  add_offset (obj_3, c);
  अचिन्हित obj_2 = c->काढा_pack (असत्य);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_4, c);
  add_offset (obj_5, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_d = add_object ("d", 1, c);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("e", 1, c);
  add_virtual_offset (obj_b, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_24_and_32_bit_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित obj_f = add_object ("f", 1, c);
  अचिन्हित obj_g = add_object ("g", 1, c);
  अचिन्हित obj_j = add_object ("j", 1, c);
  अचिन्हित obj_k = add_object ("k", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_f, c);
  अचिन्हित obj_c = c->काढा_pack (असत्य);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  अचिन्हित obj_d = c->काढा_pack (असत्य);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_j, c);
  अचिन्हित obj_h = c->काढा_pack (असत्य);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_k, c);
  अचिन्हित obj_i = c->काढा_pack (असत्य);

  start_object ("e", 1, c);
  add_wide_offset (obj_h, c);
  add_wide_offset (obj_i, c);
  अचिन्हित obj_e = c->काढा_pack (असत्य);

  start_object ("b", 1, c);
  add_24_offset (obj_c, c);
  add_24_offset (obj_d, c);
  add_24_offset (obj_e, c);
  अचिन्हित obj_b = c->काढा_pack (असत्य);

  start_object ("a", 1, c);
  add_24_offset (obj_b, c);
  c->काढा_pack (असत्य);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              पूर्णांक अंक_extensions = 0,
                                              तार्किक shared_subtables = असत्य,
                                              तार्किक version_1_2 = असत्य)
{
  constexpr पूर्णांक अंक_lookups = 5;
  constexpr पूर्णांक अंक_subtables = अंक_lookups * 2;
  अचिन्हित पूर्णांक lookups[अंक_lookups];
  अचिन्हित पूर्णांक subtables[अंक_subtables];
  अचिन्हित पूर्णांक extensions[अंक_subtables];

  std::string large_string(60000, 'a');
  c->start_serialize<अक्षर> ();


  साठी (पूर्णांक i = अंक_subtables - 1; i >= 0; i--)
    subtables[i] = add_object(large_string.c_str (), 15000 + i, c);

  साठी (पूर्णांक i = अंक_subtables - 1;
       i >= (अंक_lookups - अंक_extensions) * 2;
       i--)
  {
    extensions[i] = add_extension (subtables[i], 5, c);
  }

  साठी (पूर्णांक i = अंक_lookups - 1; i >= 0; i--)
  {
    तार्किक is_ext = (i >= (अंक_lookups - अंक_extensions));

    start_lookup (is_ext ? (अक्षर) 7 : (अक्षर) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    जर (is_ext) {
      जर (shared_subtables && i > 2) {
        add_offset (extensions[i * 2 - 1], c);
      }
      add_offset (extensions[i * 2], c);
      add_offset (extensions[i * 2 + 1], c);
    } अन्यथा {
      जर (shared_subtables && i > 2) {
        add_offset (subtables[i * 2 - 1], c);
      }
      add_offset (subtables[i * 2], c);
      add_offset (subtables[i * 2 + 1], c);
    }

    lookups[i] = finish_lookup (c);
  }

  अचिन्हित lookup_list = version_1_2
                         ? add_lookup_list_1_2 (lookups, अंक_lookups, c)
                         : add_lookup_list (lookups, अंक_lookups, c);

  जर (version_1_2)
    add_gsubgpos_header_1_2 (lookup_list, c);
  अन्यथा
    add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

ढाँचा<पूर्णांक अंक_pair_pos_1, पूर्णांक अंक_pair_set>
स्थिर शून्य
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           तार्किक as_extension = असत्य)
{
  std::string large_string(60000, 'a');
  c->start_serialize<अक्षर> ();

  constexpr पूर्णांक total_pair_set = अंक_pair_pos_1 * अंक_pair_set;
  अचिन्हित pair_set[total_pair_set];
  अचिन्हित coverage[अंक_pair_pos_1];
  अचिन्हित pair_pos_1[अंक_pair_pos_1];

  साठी (पूर्णांक i = अंक_pair_pos_1 - 1; i >= 0; i--)
  {
    साठी (पूर्णांक j = (i + 1) * अंक_pair_set - 1; j >= i * अंक_pair_set; j--)
      pair_set[j] = add_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = add_coverage (i * अंक_pair_set,
                                (i + 1) * अंक_pair_set - 1, c);

    pair_pos_1[i] = add_pair_pos_1 (&pair_set[i * अंक_pair_set],
                                    अंक_pair_set,
                                    coverage[i],
                                    c);
  }

  अचिन्हित pair_pos_2 = add_object (large_string.c_str(), 200, c);

  जर (as_extension) {
    pair_pos_2 = add_extension (pair_pos_2, 2, c);
    साठी (पूर्णांक i = अंक_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = add_extension (pair_pos_1[i], 2, c);
  }

  start_lookup (as_extension ? 9 : 2, 1 + अंक_pair_pos_1, c);

  साठी (पूर्णांक i = 0; i < अंक_pair_pos_1; i++)
    add_offset (pair_pos_1[i], c);
  add_offset (pair_pos_2, c);

  अचिन्हित lookup = finish_lookup (c);

  अचिन्हित lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

ढाँचा<पूर्णांक अंक_pair_pos_2, पूर्णांक अंक_class_1, पूर्णांक अंक_class_2>
स्थिर शून्य
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           तार्किक as_extension = असत्य,
                                           तार्किक with_device_tables = असत्य,
                                           तार्किक extra_table = सत्य)
{
  std::string large_string(100000, 'a');
  c->start_serialize<अक्षर> ();

  अचिन्हित coverage[अंक_pair_pos_2];
  अचिन्हित class_def_1[अंक_pair_pos_2];
  अचिन्हित class_def_2[अंक_pair_pos_2];
  अचिन्हित pair_pos_2[अंक_pair_pos_2];

  अचिन्हित* device_tables = (अचिन्हित*) सुस्मृति (अंक_pair_pos_2 * अंक_class_1 * अंक_class_2,
                                                आकार(अचिन्हित));

  // Total glyphs = num_class_1 * num_pair_pos_2
  साठी (पूर्णांक i = अंक_pair_pos_2 - 1; i >= 0; i--)
  {
    अचिन्हित start_glyph = 5 + i * अंक_class_1;
    जर (अंक_class_2 >= अंक_class_1)
    {
      class_def_2[i] = add_class_def (11,
                                      10 + अंक_class_2, c);
      class_def_1[i] = add_class_def (start_glyph + 1,
                                      start_glyph + अंक_class_1,
                                      c);
    } अन्यथा {
      class_def_1[i] = add_class_def (start_glyph + 1,
                                      start_glyph + अंक_class_1,
                                      c);
      class_def_2[i] = add_class_def (11,
                                      10 + अंक_class_2, c);
    }

    coverage[i] = add_coverage (start_glyph,
                                start_glyph + अंक_class_1 - 1,
                                c);

    जर (with_device_tables)
    {
      साठी(पूर्णांक j = (i + 1) * अंक_class_1 * अंक_class_2 - 1;
          j >= i * अंक_class_1 * अंक_class_2;
          j--)
      {
        uint8_t table[] = {
          (uint8_t) ((j >> 8) & 0xFF),
          (uint8_t) (j & 0xFF),
        };
        device_tables[j] = add_object ((अक्षर*) table, 2, c);
      }
    }

    pair_pos_2[i] = add_pair_pos_2 (1 + i * अंक_class_1,
                                    coverage[i],
                                    class_def_1[i], अंक_class_1,
                                    class_def_2[i], अंक_class_2,
                                    with_device_tables
                                    ? &device_tables[i * अंक_class_1 * अंक_class_2]
                                    : nullptr,
                                    c);
  }


  अचिन्हित pair_pos_1 = 0;
  जर (extra_table) pair_pos_1 = add_object (large_string.c_str(), 100000, c);

  जर (as_extension) {
    साठी (पूर्णांक i = अंक_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = add_extension (pair_pos_2[i], 2, c);

    जर (extra_table)
      pair_pos_1 = add_extension (pair_pos_1, 2, c);
  }

  start_lookup (as_extension ? 9 : 2, 1 + अंक_pair_pos_2, c);

  जर (extra_table)
    add_offset (pair_pos_1, c);

  साठी (पूर्णांक i = 0; i < अंक_pair_pos_2; i++)
    add_offset (pair_pos_2[i], c);

  अचिन्हित lookup = finish_lookup (c);

  अचिन्हित lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();

  मुक्त (device_tables);
}

ढाँचा<पूर्णांक mark_मोजणी,
    पूर्णांक class_मोजणी,
    पूर्णांक base_मोजणी,
    पूर्णांक table_मोजणी_2>
स्थिर शून्य
populate_serializer_with_large_mark_base_pos_1 (hb_serialize_context_t* c)
{
  c->start_serialize<अक्षर> ();

  MarkBasePosBuffers<mark_मोजणी, class_मोजणी, base_मोजणी, table_मोजणी_2> buffers (c);

  अचिन्हित mark_base_pos[table_मोजणी_2];
  साठी (अचिन्हित i = 0; i < table_मोजणी_2; i++)
    mark_base_pos[i] = buffers.create_mark_base_pos_1 (i, c);

  साठी (पूर्णांक i = 0; i < table_मोजणी_2; i++)
    mark_base_pos[i] = add_extension (mark_base_pos[i], 4, c);

  start_lookup (9, table_मोजणी_2, c);

  साठी (पूर्णांक i = 0; i < table_मोजणी_2; i++)
    add_offset (mark_base_pos[i], c);

  अचिन्हित lookup = finish_lookup (c);

  अचिन्हित lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

ढाँचा<अचिन्हित Lig_Subst_मोजणी,
         अचिन्हित Liga_Set_मोजणी,
         अचिन्हित Liga_Per_Set_मोजणी,
         अचिन्हित LigaSize>
स्थिर शून्य populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    तार्किक sequential_liga_sets,
                                                    तार्किक shared,
                                                    अचिन्हित *liga_subst_id,
                                                    तार्किक unique_lig_str = असत्य)
{
  अक्षर ch = 'a';

  अचिन्हित liga[Liga_Set_मोजणी * Liga_Per_Set_मोजणी];
  अचिन्हित liga_set[Liga_Set_मोजणी];
  साठी (अचिन्हित l = 0; l < Lig_Subst_मोजणी; l++) {
    अचिन्हित coverage_start = 0;
    अचिन्हित coverage_end = Liga_Set_मोजणी - 1;
    जर (sequential_liga_sets) {
      coverage_start = l * Liga_Set_मोजणी;
      coverage_end = (l + 1) * Liga_Set_मोजणी - 1;
    }
    अचिन्हित coverage = add_coverage(coverage_start, coverage_end, c, shared);

    साठी (अचिन्हित i = 0; i < Liga_Set_मोजणी; i++) {
      साठी (अचिन्हित j = 0; j < Liga_Per_Set_मोजणी; j++)
      {
        std::string large_string(100000, ch);
        start_object (large_string.c_str(), LigaSize, c);
        जर (unique_lig_str)
          ch += 1;
        add_virtual_offset(coverage, c);
        liga[i * Liga_Per_Set_मोजणी + j] = c->काढा_pack (shared);
      }

      add_liga_set_header(Liga_Per_Set_मोजणी, c);
      add_virtual_offset(coverage, c);
      साठी (अचिन्हित j = 0; j < Liga_Per_Set_मोजणी; j++)
      {
        add_offset(liga[i * Liga_Per_Set_मोजणी + j], c);
      }

      liga_set[i] = c->काढा_pack(shared);
    }

    add_liga_header(coverage, Liga_Set_मोजणी, c);
    साठी (अचिन्हित i = 0; i < Liga_Set_मोजणी; i++)
    {
      add_offset(liga_set[i], c);
    }

    liga_subst_id[l] = c->काढा_pack(shared);
  }

  साठी (अचिन्हित l = 0; l < Lig_Subst_मोजणी; l++) {
    liga_subst_id[l] = add_extension(liga_subst_id[l], 4, c);
  }
}

ढाँचा<अचिन्हित liga_subst_मोजणी,
         अचिन्हित liga_set_मोजणी,
         अचिन्हित liga_per_set_मोजणी,
         अचिन्हित liga_size>
स्थिर शून्य
populate_serializer_with_large_liga (hb_serialize_context_t* c, तार्किक sequential_liga_sets)
{
  c->start_serialize<अक्षर> ();

  अचिन्हित liga_subst[liga_subst_मोजणी];
  populate_serializer_with_large_ligsubst<liga_subst_मोजणी, liga_set_मोजणी, liga_per_set_मोजणी, liga_size>(c, sequential_liga_sets, असत्य, liga_subst);

  start_lookup (7, liga_subst_मोजणी, c);
  साठी (अचिन्हित l = 0; l < liga_subst_मोजणी; l++) {
    add_offset(liga_subst[l], c);
  }

  अचिन्हित lookup = finish_lookup (c);

  अचिन्हित lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

स्थिर शून्य
populate_serializer_with_large_liga_overlapping_clone_result (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->start_serialize<अक्षर> ();

  constexpr अचिन्हित liga_size = 30000;

  अचिन्हित liga[6];
  अचिन्हित liga_subst[3];
  अचिन्हित liga_set[2];

  // LigaSubst 3
  अचिन्हित coverage = add_coverage(1, 1, c);
  साठी (पूर्णांक i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->काढा_pack (असत्य);
  }

  add_liga_set_header(2, c);
  add_virtual_offset(coverage, c);
  साठी (अचिन्हित i = 0; i < 2; i++)
    add_offset(liga[i], c);
  liga_set[0] = c->काढा_pack(असत्य);

  add_liga_header(coverage, 1, c);
  add_offset(liga_set[0], c);
  liga_subst[2] = c->काढा_pack(असत्य);

  // LigaSubst 2
  coverage = add_coverage(0, 1, c);
  साठी (पूर्णांक i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->काढा_pack (असत्य);
  }

  add_liga_set_header(1, c);
  add_virtual_offset(coverage, c);
  add_offset(liga[1], c);
  liga_set[1] = c->काढा_pack(असत्य);

  add_liga_set_header(1, c);
  add_virtual_offset(coverage, c);
  add_offset(liga[0], c);
  liga_set[0] = c->काढा_pack(असत्य);

  add_liga_header(coverage, 2, c);
  add_offset(liga_set[0], c);
  add_offset(liga_set[1], c);
  liga_subst[1] = c->काढा_pack(असत्य);

  // LigaSubst 1
  coverage = add_coverage(0, 0, c);
  साठी (पूर्णांक i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->काढा_pack (असत्य);
  }

  add_liga_set_header(2, c);
  add_virtual_offset(coverage, c);
  साठी (अचिन्हित i = 0; i < 2; i++)
    add_offset(liga[i], c);
  liga_set[0] = c->काढा_pack(असत्य);

  add_liga_header(coverage, 1, c);
  add_offset(liga_set[0], c);
  liga_subst[0] = c->काढा_pack(असत्य);

  साठी (पूर्णांक l = 2; l >= 0; l--) {
    liga_subst[l] = add_extension(liga_subst[l], 4, c);
  }

  start_lookup (7, 3, c);
  साठी (अचिन्हित l = 0; l < 3; l++) {
    add_offset(liga_subst[l], c);
  }

  अचिन्हित lookup = finish_lookup (c);

  अचिन्हित lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

ढाँचा<अचिन्हित liga_subst_मोजणी,
         अचिन्हित liga_set_मोजणी,
         अचिन्हित liga_per_set_मोजणी,
         अचिन्हित liga_size>
स्थिर शून्य
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, तार्किक sequential_liga_sets)
{
  c->start_serialize<अक्षर> ();

  अचिन्हित lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  अचिन्हित liga_subst_idx[liga_subst_मोजणी];
  populate_serializer_with_large_ligsubst<liga_subst_मोजणी, liga_set_मोजणी, liga_per_set_मोजणी, liga_size>(c, sequential_liga_sets, सत्य, liga_subst_idx, सत्य);

  start_lookup (7, liga_subst_मोजणी, c);
  साठी (अचिन्हित l = 0; l < liga_subst_मोजणी; l++) {
    add_offset(liga_subst_idx[l], c);
  }

  lookups[0] = finish_lookup (c);

  // Lookup with 2 LigSubst tables
  अचिन्हित liga_subst[liga_subst_मोजणी + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, असत्य, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_मोजणी, liga_set_मोजणी, liga_per_set_मोजणी, liga_size>(c, sequential_liga_sets, सत्य, &liga_subst[1], सत्य);

  start_lookup (7, liga_subst_मोजणी + 1, c);
  साठी (अचिन्हित l = 0; l <= liga_subst_मोजणी; l++) {
    add_offset(liga_subst[l], c);
  }

  lookups[1] = finish_lookup (c);

  अचिन्हित lookup_list = add_lookup_list (lookups, 2, c);
  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

ढाँचा<अचिन्हित liga_subst_मोजणी,
         अचिन्हित liga_set_मोजणी,
         अचिन्हित liga_per_set_मोजणी,
         अचिन्हित liga_size>
स्थिर शून्य
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->start_serialize<अक्षर> ();

  अचिन्हित liga_subst[liga_subst_मोजणी + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, सत्य, सत्य, liga_subst, सत्य);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_मोजणी, liga_set_मोजणी, liga_per_set_मोजणी, liga_size>(c, सत्य, सत्य, &liga_subst[1], सत्य);

  start_lookup (7, liga_subst_मोजणी + 1, c);
  साठी (अचिन्हित l = 0; l <= liga_subst_मोजणी; l++) {
    add_offset(liga_subst[l], c);
  }

  अचिन्हित lookup = finish_lookup (c);

  अचिन्हित lookup_list = add_lookup_list (&lookup, 1, c);
  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

स्थिर शून्य test_sort_shortest ()
{
  माप_प्रकार buffer_size = 100;
  शून्य* buffer_a = दो_स्मृति (buffer_size);
  शून्य* buffer_e = दो_स्मृति (buffer_size);
  hb_serialize_context_t a (buffer_a, buffer_size);
  hb_serialize_context_t e (buffer_e, buffer_size);
  populate_serializer_complex_2 (&a);

  graph_t graph = graph_t::create (a.object_graph ()).value ();
  hb_always_assert (graph.sort_shortest_distance ().is_ok());
  graph.normalize();

  // Expected graph
  e.start_serialize();
  अचिन्हित jkl = add_object ("jkl", 3, &e);

  start_object("ghi", 3, &e);
  add_offset(jkl, &e);
  अचिन्हित ghi = e.काढा_pack(असत्य);

  start_object("def", 3, &e);
  add_offset(ghi, &e);
  अचिन्हित def = e.काढा_pack(असत्य);

  अचिन्हित mn = add_object("mn", 2, &e);

  start_object ("abc", 3, &e);
  add_offset (def, &e);
  add_offset (jkl, &e);
  add_offset (mn, &e);
  e.काढा_pack (असत्य);
  e.end_serialize();

  graph_t expected = graph_t::create (e.object_graph ()).value ();
  expected.normalize();

  प्रतिज्ञा(expected == graph);

  मुक्त (buffer_a);
  मुक्त (buffer_e);
}

स्थिर शून्य test_duplicate_leaf ()
{
  माप_प्रकार buffer_size = 100;
  शून्य* buffer_a = दो_स्मृति (buffer_size);
  शून्य* buffer_e = दो_स्मृति (buffer_size);
  hb_serialize_context_t a (buffer_a, buffer_size);
  hb_serialize_context_t e (buffer_e, buffer_size);
  populate_serializer_complex_2 (&a);

  graph_t graph = graph_t::create (a.object_graph ()).value ();
  hb_always_assert (graph.duplicate (4, 1, असत्य).is_ok ());
  graph.normalize();

  e.start_serialize();
  अचिन्हित mn = add_object("mn", 2, &e);
  अचिन्हित jkl_2 = add_object("jkl", 3, &e);

  start_object("ghi", 3, &e);
  add_offset(jkl_2, &e);
  अचिन्हित ghi = e.काढा_pack(असत्य);

  start_object("def", 3, &e);
  add_offset(ghi, &e);
  अचिन्हित def = e.काढा_pack(असत्य);

  अचिन्हित jkl_1 = add_object("jkl", 3, &e);

  start_object("abc", 3, &e);
  add_offset(def, &e);
  add_offset(jkl_1, &e);
  add_offset(mn, &e);
  e.काढा_pack(असत्य);

  graph_t expected = graph_t::create (e.object_graph ()).value ();
  expected.normalize();

  प्रतिज्ञा(expected == graph);

  मुक्त (buffer_a);
  मुक्त (buffer_e);
}

स्थिर शून्य test_duplicate_interior ()
{
  माप_प्रकार buffer_size = 100;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_3 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();
  hb_always_assert (graph.duplicate (3, 2, असत्य).is_ok ());

  hb_always_assert(म_भेदन (graph.object (6).head, "jkl", 3) == 0);
  hb_always_assert(graph.object (6).real_links.length == 1);
  hb_always_assert(graph.object (6).real_links[0].objidx == 0);

  hb_always_assert(म_भेदन (graph.object (5).head, "abc", 3) == 0);
  hb_always_assert(graph.object (5).real_links.length == 3);
  hb_always_assert(graph.object (5).real_links[0].objidx == 4);
  hb_always_assert(graph.object (5).real_links[1].objidx == 2);
  hb_always_assert(graph.object (5).real_links[2].objidx == 1);

  hb_always_assert(म_भेदन (graph.object (4).head, "def", 3) == 0);
  hb_always_assert(graph.object (4).real_links.length == 1);
  hb_always_assert(graph.object (4).real_links[0].objidx == 3);

  hb_always_assert(म_भेदन (graph.object (3).head, "ghi", 3) == 0);
  hb_always_assert(graph.object (3).real_links.length == 1);
  hb_always_assert(graph.object (3).real_links[0].objidx == 6);

  hb_always_assert(म_भेदन (graph.object (2).head, "jkl", 3) == 0);
  hb_always_assert(graph.object (2).real_links.length == 1);
  hb_always_assert(graph.object (2).real_links[0].objidx == 0);

  hb_always_assert(म_भेदन (graph.object (1).head, "mn", 2) == 0);
  hb_always_assert(graph.object (1).real_links.length == 0);

  hb_always_assert(म_भेदन (graph.object (0).head, "opqrst", 6) == 0);
  hb_always_assert(graph.object (0).real_links.length == 0);

  मुक्त (buffer);
}

स्थिर शून्य
test_serialize ()
{
  माप_प्रकार buffer_size = 100;
  शून्य* buffer_1 = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 100;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  populate_serializer_complex_2 (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (!graph::will_overflow (graph, nullptr).value ());

  मुक्त (buffer);
}

स्थिर शून्य test_will_overflow_2 ()
{
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (graph::will_overflow (graph, nullptr).value ());

  मुक्त (buffer);
}

स्थिर शून्य test_will_overflow_3 ()
{
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (graph::will_overflow (graph, nullptr).value ());

  मुक्त (buffer);
}

स्थिर शून्य test_resolve_overflows_via_sort ()
{
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 300000;
  शून्य* buffer = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_spaces (&c, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_spaces (&e, असत्य);

  run_resolve_overflow_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_overflows_via_isolation ()
{
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_isolation_overflow_complex (&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_spaces_16bit_connection (&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_short_and_wide_subgraph_मूळ (&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_short_and_wide_subgraph_मूळ_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_overflows_via_isolation_spaces ()
{
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_isolation_overflow_spaces (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (c.offset_overflow ());
  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();

  अचिन्हित expected_length = 3 + 2 * 60000; // objects
  expected_length += 2 * 4 + 2 * 2; // links
  hb_always_assert (result.length == expected_length);

  मुक्त (buffer);
  hb_blob_destroy (out);
}

स्थिर शून्य test_resolve_mixed_overflows_via_isolation_spaces ()
{
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_24_and_32_bit_offsets (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (c.offset_overflow ());
  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();

  अचिन्हित expected_length =
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
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             सत्य);
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_shared_extension_promotion ()
{
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c, 0, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3, सत्य);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             सत्य);
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

#ifndef HB_NO_BEYOND_64K
स्थिर शून्य test_resolve_with_extension_promotion_1_2 ()
{
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c, 0, असत्य, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3, असत्य, सत्य);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             सत्य);
  मुक्त (buffer);
  मुक्त (expected_buffer);
}
#endif

स्थिर शून्य test_resolve_with_repeated_extension_promotion ()
{
  माप_प्रकार buffer_size = 1000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize<अक्षर> ();
  अचिन्हित subtable = add_object ("subt", 4, &c);
  start_lookup (5, 2, &c);
  add_offset (subtable, &c);
  add_offset (subtable, &c);
  अचिन्हित lookup = finish_lookup (&c);
  अचिन्हित lookup_list = add_lookup_list (&lookup, 1, &c);
  add_gsubgpos_header (lookup_list, &c);
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();
  स्वतः context = graph::gsubgpos_graph_context_t::create (HB_TAG ('G', 'S', 'U', 'B'),
                                                          graph).value ();
  hb_always_assert (context.lookups.get_population () == 1);
  अचिन्हित lookup_index = *context.lookups.किल्ल्या ();
  graph::Lookup* lookup_table = context.lookups.get (lookup_index);
  hb_always_assert (lookup_table->make_extension (context, lookup_index).is_ok ());

  स्थिरांक स्वतः& lookup_vertex = graph.vertices_[lookup_index];
  hb_always_assert (lookup_vertex.obj ().real_links.length == 2);
  अचिन्हित extension_index = lookup_vertex.obj ().real_links[0].objidx;
  hb_always_assert (lookup_vertex.obj ().real_links[1].objidx == extension_index);

  स्वतः* extension = graph.vertices_[extension_index].as<
      graph::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
  अचिन्हित promoted_subtable = extension->get_subtable_index (graph, extension_index).value ();
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
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, सत्य);

  run_resolve_overflow_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_extension_pair_pos_1_split ()
{
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, सत्य);

  run_resolve_overflow_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_basic_pair_pos_2_split ()
{
  माप_प्रकार buffer_size = 300000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, सत्य);

  run_resolve_overflow_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_close_to_limit_pair_pos_2_split ()
{
  माप_प्रकार buffer_size = 300000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, सत्य, असत्य, असत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, सत्य, असत्य, असत्य);

  run_resolve_overflow_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_pair_pos_2_split_with_device_tables ()
{
  माप_प्रकार buffer_size = 300000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, असत्य, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, सत्य, सत्य);

  run_resolve_overflow_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_basic_mark_base_pos_1_split ()
{
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_mark_base_pos_1 <40, 10, 110, 1>(&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_mark_base_pos_1 <40, 10, 110, 2>(&e);

  run_resolve_overflow_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'P', 'O', 'S'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_basic_liga_split ()
{
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, असत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, असत्य);

  run_resolve_overflow_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_liga_split_move ()
{
  माप_प्रकार buffer_size = 400000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, सत्य);

  run_resolve_overflow_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_liga_split_overlapping_clone ()
{
  माप_प्रकार buffer_size = 400000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga_overlapping_clone_result(&e);

  run_resolve_overflow_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_liga_split_shared_table ()
{
  माप_प्रकार buffer_size = 400000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, सत्य);

  run_resolve_overflow_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_with_liga_split_shared_coverage ()
{
  माप_प्रकार buffer_size = 400000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  run_resolve_overflow_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             सत्य,
                             HB_TAG('G', 'S', 'U', 'B'));
  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य test_resolve_overflows_via_splitting_spaces ()
{
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_split_spaces (&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_split_spaces_2 (&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 160000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_priority_overflow (&c);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
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
  माप_प्रकार buffer_size = 100;
  शून्य* buffer = दो_स्मृति (buffer_size);
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
test_shared_गाठ_with_virtual_links ()
{
  माप_प्रकार buffer_size = 100;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize<अक्षर> ();

  अचिन्हित obj_b = add_object ("b", 1, &c);
  अचिन्हित obj_c = add_object ("c", 1, &c);

  start_object ("d", 1, &c);
  add_virtual_offset (obj_b, &c);
  अचिन्हित obj_d_1 = c.काढा_pack ();

  start_object ("d", 1, &c);
  add_virtual_offset (obj_c, &c);
  अचिन्हित obj_d_2 = c.काढा_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  start_object ("a", 1, &c);
  add_offset (obj_b, &c);
  add_offset (obj_c, &c);
  add_offset (obj_d_1, &c);
  add_offset (obj_d_2, &c);
  c.काढा_pack ();
  c.end_serialize ();

  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links.length == 2);
  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links[1].objidx == obj_c);
  मुक्त(buffer);
}

स्थिर शून्य
test_repack_last ()
{
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_repack_last (&c, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_repack_last (&e, असत्य);

  run_resolve_overflow_test ("test_repack_last",
                             c,
                             e,
                             20,
                             असत्य,
                             HB_TAG('a', 'b', 'c', 'd'),
                             सत्य);

  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य
test_dont_duplicate_virtual ()
{
  माप_प्रकार buffer_size = 200000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_virtual (&c, सत्य);

  शून्य* expected_buffer = दो_स्मृति (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_virtual (&e, असत्य);

  run_resolve_overflow_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             असत्य,
                             HB_TAG('a', 'b', 'c', 'd'),
                             सत्य);

  मुक्त (buffer);
  मुक्त (expected_buffer);
}

स्थिर शून्य
test_deep_graph_traversal ()
{
  माप_प्रकार buffer_size = 1000000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize ();
  अचिन्हित prev = add_object ("leaf", 4, &c);
  साठी (अचिन्हित i = 0; i < 2000; i++)
  {
    start_object ("node", 4, &c);
    add_offset (prev, &c);
    prev = c.काढा_pack (असत्य);
  }
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // Test find_subgraph (set)
  hb_set_t visited_set;
  hb_always_assert (graph.find_subgraph (graph.मूळ_idx (), visited_set).is_ok ());
  hb_always_assert (visited_set.get_population () == 2001);

  // Test find_subgraph_size
  hb_set_t size_set;
  माप_प्रकार sz = graph.find_subgraph_size (graph.मूळ_idx (), size_set).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.set (graph.मूळ_idx (), 1);
  hb_always_assert (graph.find_subgraph (graph.मूळ_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (graph.assign_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t index_map;
  hb_always_assert (graph.duplicate_subgraph (graph.मूळ_idx (), index_map).is_ok ());
  hb_always_assert (index_map.get_population () == 2001);

  मुक्त (buffer);
}

स्थिर शून्य
test_32bit_roots_traversal ()
{
  माप_प्रकार buffer_size = 1000;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize ();
  अचिन्हित leaf1 = add_object ("l1", 2, &c);
  अचिन्हित leaf2 = add_object ("l2", 2, &c);

  start_object ("mid", 3, &c);
  add_wide_offset (leaf1, &c);
  अचिन्हित mid = c.काढा_pack (असत्य);

  start_object ("root", 4, &c);
  add_offset (mid, &c);
  add_wide_offset (leaf2, &c);
  c.काढा_pack (असत्य);
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_set_t roots;
  hb_always_assert (graph.find_32bit_roots (graph.मूळ_idx (), roots).is_ok ());
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
  buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c2 (buffer, buffer_size);

  c2.start_serialize ();
  अचिन्हित b = add_object ("b", 1, &c2);

  start_object ("a", 1, &c2);
  add_wide_offset (b, &c2);
  अचिन्हित a = c2.काढा_pack (असत्य);

  start_object ("r", 1, &c2);
  add_offset (a, &c2);
  add_wide_offset (a, &c2);
  c2.काढा_pack (असत्य);

  c2.end_serialize ();

  graph_t graph2 = graph_t::create (c2.object_graph ()).value ();

  hb_set_t roots2;
  hb_always_assert (graph2.find_32bit_roots (graph2.मूळ_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  मुक्त (buffer);
}

स्थिर शून्य test_invalid_link_objidx ()
{
  माप_प्रकार buffer_size = 100;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_2 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // Poison a link to point to (unsigned) -1
  स्वतः links = graph.vertices_[graph.मूळ_idx ()].real_links_writer();
  links->objidx = (अचिन्हित) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!graph.sort_shortest_distance ().is_ok ());

  मुक्त (buffer);
}

स्थिर शून्य test_invalid_link_add_and_move ()
{
  माप_प्रकार buffer_size = 100;
  शून्य* buffer = दो_स्मृति (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_2 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!graph.add_link (&dummy, 0, (अचिन्हित) -1).is_ok ());

  // move_child with non-existent offset
  स्वतः res = graph.move_child (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  मुक्त (buffer);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
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
  test_shared_गाठ_with_virtual_links ();
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
