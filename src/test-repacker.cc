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

#அந்தர்பாவ <cstdint>
#அந்தர்பாவ <string>

#அந்தர்பாவ "hb-repacker.hh"
#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "graph/serialize.hh"

நிலையான வெறுமை extend (மாறிலி எழுத்து* value,
                    unsigned len,
                    hb_serialize_context_t* c)
{
  எழுத்து* obj = c->allocate_size<எழுத்து> (len);
  hb_memcpy (obj, value, len);
}

நிலையான வெறுமை start_object(மாறிலி எழுத்து* tag,
                         unsigned len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (tag, len, c);
}

நிலையான unsigned add_object(மாறிலி எழுத்து* tag,
                           unsigned len,
                           hb_serialize_context_t* c,
                           தருக்க shared = பொய்)
{
  start_object (tag, len, c);
  திரும்பு c->எடு_pack (shared);
}


நிலையான வெறுமை add_offset (unsigned id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* offset = c->start_embed<OT::Offset16> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

நிலையான வெறுமை add_24_offset (unsigned id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* offset = c->start_embed<OT::Offset24> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

நிலையான வெறுமை add_wide_offset (unsigned id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* offset = c->start_embed<OT::Offset32> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

நிலையான வெறுமை add_gsubgpos_header (unsigned lookup_list,
                                 hb_serialize_context_t* c)
{
  எழுத்து header[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  start_object (header, 8, c);
  add_offset (lookup_list, c);
  c->எடு_pack (பொய்);
}

நிலையான வெறுமை add_gsubgpos_header_1_2 (unsigned lookup_list,
                                     hb_serialize_context_t* c)
{
  எழுத்து header[] = {
    0, 1, 0, 2, // version
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  start_object (header, அளவு (header), c);
  add_wide_offset (lookup_list, c);
  c->எடு_pack (பொய்);
}

நிலையான unsigned add_lookup_list (மாறிலி unsigned* lookups,
                                 எழுத்து மொத்தம்,
                                 hb_serialize_context_t* c)
{
  எழுத்து lookup_மொத்தம்[] = {0, மொத்தம்};
  start_object  ((எழுத்து *) &lookup_மொத்தம், 2, c);

  க்கு (முழு i = 0; i < மொத்தம்; i++)
    add_offset (lookups[i], c);

  திரும்பு c->எடு_pack (பொய்);
}

நிலையான unsigned add_lookup_list_1_2 (மாறிலி unsigned* lookups,
                                     எழுத்து மொத்தம்,
                                     hb_serialize_context_t* c)
{
  எழுத்து lookup_மொத்தம்[] = {0, மொத்தம்};
  start_object ((எழுத்து *) &lookup_மொத்தம், 2, c);

  க்கு (முழு i = 0; i < மொத்தம்; i++)
    add_wide_offset (lookups[i], c);

  திரும்பு c->எடு_pack (பொய்);
}

நிலையான வெறுமை start_lookup (int8_t type,
                          int8_t இலக்கம்_subtables,
                          hb_serialize_context_t* c)
{
  எழுத்து lookup[] = {
    0, (எழுத்து)type, // type
    0, (எழுத்து)2, // flag
    0, (எழுத்து)இலக்கம்_subtables, // num subtables
  };

  start_object (lookup, 6, c);
}

நிலையான unsigned finish_lookup (hb_serialize_context_t* c)
{
  எழுத்து filter[] = {0, 0};
  extend (filter, 2, c);
  திரும்பு c->எடு_pack (பொய்);
}

நிலையான unsigned add_extension (unsigned child,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  எழுத்து ext[] = {
    0, 1,
    0, (எழுத்து) type,
  };

  start_object (ext, 4, c);
  add_wide_offset (child, c);

  திரும்பு c->எடு_pack (பொய்);

}

// Adds coverage table fro [start, end]
நிலையான unsigned add_coverage (unsigned start, unsigned end,
                              hb_serialize_context_t* c,
                              தருக்க shared = பொய்)
{
  எனில் (end - start == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //மொத்தம்

      (uint8_t) ((start >> 8) & 0xFF),
      (uint8_t) (start & 0xFF), // glyph[0]
    };
    திரும்பு add_object ((எழுத்து*) coverage, 6, c, shared);
  }

  எனில் (end - start == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //மொத்தம்

      (uint8_t) ((start >> 8) & 0xFF),
      (uint8_t) (start & 0xFF), // glyph[0]

      (uint8_t) ((end >> 8) & 0xFF),
      (uint8_t) (end & 0xFF), // glyph[1]
    };
    திரும்பு add_object ((எழுத்து*) coverage, 8, c, shared);
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
  திரும்பு add_object ((எழுத்து*) coverage, 10, c, shared);
}


template<typename It>
நிலையான unsigned add_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  திரும்பு c->எடு_pack (பொய்);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
நிலையான unsigned add_class_def (uint16_t start_glyph,
                               uint16_t end_glyph,
                               hb_serialize_context_t* c)
{
  unsigned மொத்தம் = end_glyph - start_glyph;
  uint8_t header[] = {
    0, 1, // format

    (uint8_t) ((start_glyph >> 8) & 0xFF),
    (uint8_t) (start_glyph & 0xFF), // start_glyph

    (uint8_t) ((மொத்தம் >> 8) & 0xFF),
    (uint8_t) (மொத்தம் & 0xFF), //மொத்தம்
  };

  start_object ((எழுத்து*) header, 6, c);
  க்கு (uint16_t i = 1; i <= மொத்தம்; i++)
  {
    uint8_t class_மதிப்பு_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //மொத்தம்
    };
    extend ((எழுத்து*) class_மதிப்பு_2, 2, c);
  }

  திரும்பு c->எடு_pack (பொய்);
}

நிலையான unsigned add_pair_pos_1 (unsigned* pair_sets,
                                எழுத்து மொத்தம்,
                                unsigned coverage,
                                hb_serialize_context_t* c)
{
  எழுத்து format[] = {
    0, 1
  };

  start_object (format, 2, c);
  add_offset (coverage, c);

  எழுத்து மதிப்பு_format[] = {
    0, 0,
    0, 0,
    0, மொத்தம்,
  };
  extend (மதிப்பு_format, 6, c);

  க்கு (எழுத்து i = 0; i < மொத்தம்; i++)
    add_offset (pair_sets[(unsigned) i], c);

  திரும்பு c->எடு_pack (பொய்);
}

நிலையான வெறுமை add_liga_set_header (எழுத்து liga_மொத்தம்,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_மொத்தம்_bytes[] = {
    (uint8_t) (0xFF & (liga_மொத்தம் >> 8)),
    (uint8_t) (0xFF & liga_மொத்தம்),
  };

  start_object ((மாறிலி எழுத்து*) liga_மொத்தம்_bytes, 2, c);
}

நிலையான வெறுமை add_liga_header (unsigned coverage, unsigned liga_set_மொத்தம், hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  start_object ((மாறிலி எழுத்து*) format, 2, c);
  add_offset(coverage, c);

  uint8_t liga_set_மொத்தம்_bytes[] = {
    (uint8_t) (0xFF & (liga_set_மொத்தம் >> 8)),
    (uint8_t) (0xFF & liga_set_மொத்தம்),
  };

  extend((மாறிலி எழுத்து *) liga_set_மொத்தம்_bytes, 2, c);
}

நிலையான unsigned add_pair_pos_2 (unsigned starting_class,
                                unsigned coverage,
                                unsigned class_def_1, uint16_t class_def_1_மொத்தம்,
                                unsigned class_def_2, uint16_t class_def_2_மொத்தம்,
                                unsigned* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  start_object ((எழுத்து*) format, 2, c);
  add_offset (coverage, c);

  unsigned இலக்கம்_மதிப்புகள்_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  எனில் (device_tables) {
    format2 |= 0x20;
    இலக்கம்_மதிப்புகள்_3 += 1;
  }
  uint8_t மதிப்பு_format[] = {
    0, format1,
    0, format2,
  };

  extend ((எழுத்து*) மதிப்பு_format, 4, c);

  add_offset (class_def_1, c);
  add_offset (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_மொத்தம் >> 8) & 0xFF),
    (uint8_t) (class_def_1_மொத்தம் & 0xFF),
    (uint8_t) ((class_def_2_மொத்தம் >> 8) & 0xFF),
    (uint8_t) (class_def_2_மொத்தம் & 0xFF),
  };
  extend ((எழுத்து*) class_counts, 4, c);

  unsigned இலக்கம்_bytes_per_record = class_def_2_மொத்தம் * இலக்கம்_மதிப்புகள்_3 * 2;
  uint8_t* record = (uint8_t*) calloc (1, இலக்கம்_bytes_per_record);
  முழு device_index = 0;
  க்கு (uint16_t i = 0; i < class_def_1_மொத்தம்; i++)
  {

    க்கு (uint16_t j = 0; j < class_def_2_மொத்தம்; j++)
    {
      க்கு (முழு k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((எழுத்து*) value, 2, c);
      }

      எனில் (device_tables) {
        add_offset (device_tables[device_index++], c);
      }
    }
  }
  விடுவி (record);

  திரும்பு c->எடு_pack (பொய்);
}

நிலையான unsigned add_mark_base_pos_1 (unsigned mark_coverage,
                                     unsigned base_coverage,
                                     unsigned mark_array,
                                     unsigned base_array,
                                     unsigned class_மொத்தம்,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  start_object ((எழுத்து*) format, 2, c);
  add_offset (mark_coverage, c);
  add_offset (base_coverage, c);

  uint8_t மொத்தம்[] = {
    (uint8_t) ((class_மொத்தம் >> 8) & 0xFF),
    (uint8_t) (class_மொத்தம் & 0xFF),
  };
  extend ((எழுத்து*) மொத்தம், 2, c);

  add_offset (mark_array, c);
  add_offset (base_array, c);

  திரும்பு c->எடு_pack (பொய்);
}

template<முழு mark_மொத்தம்,
    முழு class_மொத்தம்,
    முழு base_மொத்தம்,
    முழு table_மொத்தம்_2>
கட்டமைப்பு MarkBasePosBuffers
{
  unsigned base_anchors[class_மொத்தம் * base_மொத்தம்];
  unsigned mark_anchors[mark_மொத்தம்];
  uint8_t anchor_buffers[class_மொத்தம் * base_மொத்தம் + 100];
  uint8_t class_buffer[class_மொத்தம் * 2];

  MarkBasePosBuffers(hb_serialize_context_t* c)
  {
    க்கு (unsigned i = 0; i < அளவு(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    க்கு (unsigned i = 0; i < class_மொத்தம் * base_மொத்தம்; i++)
    {
      base_anchors[i] = add_object ((எழுத்து*) &anchor_buffers[i], 100, c);
      எனில் (i < class_மொத்தம்) {
        class_buffer[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_buffer[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    க்கு (unsigned i = 0; i < mark_மொத்தம்; i++)
    {
      mark_anchors[i] = add_object ((எழுத்து*) &anchor_buffers[i], 4, c);
    }
  }

  unsigned create_mark_base_pos_1 (unsigned table_index, hb_serialize_context_t* c)
  {
    unsigned class_per_table = class_மொத்தம் / table_மொத்தம்_2;
    unsigned mark_per_class = mark_மொத்தம் / class_மொத்தம்;
    unsigned start_class = class_per_table * table_index;
    unsigned end_class = class_per_table * (table_index + 1) - 1;

    // baseArray
    uint8_t base_மொத்தம்_buffer[] = {
      (uint8_t) ((base_மொத்தம் >> 8) & 0xFF),
      (uint8_t) (base_மொத்தம் & 0xFF),

    };
    start_object ((எழுத்து*) base_மொத்தம்_buffer, 2, c);
    க்கு (unsigned base = 0; base < base_மொத்தம்; base++)
    {
      க்கு (unsigned klass = start_class; klass <= end_class; klass++)
      {
        unsigned i = base * class_மொத்தம் + klass;
        add_offset (base_anchors[i], c);
      }
    }
    unsigned base_array = c->எடு_pack (பொய்);

    // markArray
    unsigned இலக்கம்_marks = class_per_table * mark_per_class;
    uint8_t mark_மொத்தம்_buffer[] = {
      (uint8_t) ((இலக்கம்_marks >> 8) & 0xFF),
      (uint8_t) (இலக்கம்_marks & 0xFF),
    };
    start_object ((எழுத்து*) mark_மொத்தம்_buffer, 2, c);
    க்கு (unsigned mark = 0; mark < mark_மொத்தம்; mark++)
    {
      unsigned klass = mark % class_மொத்தம்;
      எனில் (klass < start_class || klass > end_class) தொடர்;
      klass -= start_class;

      extend ((எழுத்து*) &class_buffer[2 * klass], 2, c);
      add_offset (mark_anchors[mark], c);
    }
    unsigned mark_array = c->எடு_pack (பொய்);

    // markCoverage
    auto it =
        + hb_range ((hb_codepoint_t) mark_மொத்தம்)
        | hb_filter ([&] (hb_codepoint_t mark) {
          unsigned klass = mark % class_மொத்தம்;
          திரும்பு klass >= class_per_table * table_index &&
              klass < class_per_table * (table_index + 1);
        })
        ;
    unsigned mark_coverage = add_coverage (it, c);

    // baseCoverage
    unsigned base_coverage = add_coverage (10, 10 + base_மொத்தம் - 1, c);

    திரும்பு add_mark_base_pos_1 (mark_coverage,
                                base_coverage,
                                mark_array,
                                base_array,
                                class_per_table,
                                c);
  }
};

நிலையான வெறுமை run_resolve_overflow_test (மாறிலி எழுத்து* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       unsigned இலக்கம்_iterations = 0,
                                       தருக்க recalculate_extensions = பொய்,
                                       hb_tag_t tag = HB_TAG ('G', 'S', 'U', 'B'),
                                       தருக்க check_இருமம்_equivalence = பொய்)
{
  printf (">>> Testing overflowing resolution for %s\n",
          name);

  graph_t graph = graph_t::create (overflowing.object_graph ()).value ();

  graph_t expected_graph = graph_t::create (expected.object_graph ()).value ();
  எனில் (graph::will_overflow (expected_graph).value ())
  {
    எனில் (check_இருமம்_equivalence) {
      printf("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_இருமம்_equivalence);
    }
    hb_always_assert (expected_graph.assign_spaces ().is_ok ());
    hb_always_assert (expected_graph.sort_shortest_distance ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.offset_overflow ());
  auto r = hb_resolve_graph_overflows (tag,
                                      இலக்கம்_iterations,
                                      recalculate_extensions,
                                      graph);
  எனில் (r.is_err())
    printf("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", graph::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = graph::serialize (graph).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = graph::serialize (expected_graph).value ();
  hb_always_assert (out2);
  எனில் (check_இருமம்_equivalence) {
    unsigned l1, l2;
    மாறிலி எழுத்து* d1 = hb_blob_get_data(out1, &l1);
    மாறிலி எழுத்து* d2 = hb_blob_get_data(out2, &l2);

    தருக்க match = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    எனில் (!match) {
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
  எனில் (!(graph == expected_graph)) {
    printf("## Expected:\n");
    expected_graph.print();
    printf("## Result:\n");
    graph.print();
  }
  hb_always_assert (graph == expected_graph);
}

நிலையான வெறுமை add_virtual_offset (unsigned id,
                                hb_serialize_context_t* c)
{
  c->add_virtual_link (id);
}

நிலையான வெறுமை
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->start_serialize<எழுத்து> ();

  unsigned obj_1 = add_object ("ghi", 3, c);
  unsigned obj_2 = add_object ("def", 3, c);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_virtual (hb_serialize_context_t* c, தருக்க with_overflow)
{
  std::string large_string(50000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_4, obj_5;
  எனில் (with_overflow) {
    obj_5 = add_object("55555", 5, c);
    obj_4 = add_object(large_string.c_str(), 50000, c);
  } இல்லையெனில் {
    obj_4 = add_object(large_string.c_str(), 50000, c);
    obj_5 = add_object("55555", 5, c);
  }

  start_object(large_string.c_str(), 20000, c);
  add_offset(obj_5, c);
  unsigned obj_3 = c->எடு_pack(பொய்);

  start_object("2", 2, c);
  add_virtual_offset(obj_5, c);
  unsigned obj_2 = c->எடு_pack(பொய்);

  // obj 1
  start_object("1", 1, c);
  add_offset(obj_2, c);
  add_offset(obj_3, c);
  add_offset(obj_4, c);
  c->எடு_pack(பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_overflow (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_1 = add_object (large_string.c_str(), 10000, c);
  unsigned obj_2 = add_object (large_string.c_str(), 20000, c);
  unsigned obj_3 = add_object (large_string.c_str(), 50000, c);

  start_object ("abc", 3, c);
  add_offset (obj_3, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_priority_overflow (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_e = add_object ("e", 1, c);
  unsigned obj_d = add_object ("d", 1, c);

  start_object (large_string.c_str (), 50000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object (large_string.c_str (), 20000, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_priority_overflow_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 50000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  unsigned obj_d = add_object ("d", 1, c);

  start_object (large_string.c_str (), 20000, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}


நிலையான வெறுமை
populate_serializer_with_dedup_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_1 = add_object ("def", 3, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_1, c);
  unsigned obj_2 = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_multiple_dedup_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned leaf = add_object("def", 3, c);

  constexpr unsigned இலக்கம்_mid_nodes = 20;
  unsigned mid_nodes[இலக்கம்_mid_nodes];
  க்கு (unsigned i = 0; i < இலக்கம்_mid_nodes; i++) {
    start_object(large_string.c_str(), 10000 + i, c);
    add_offset(leaf, c);
    mid_nodes[i] = c->எடு_pack(பொய்);
  }

  start_object("abc", 3, c);
  க்கு (unsigned i = 0; i < இலக்கம்_mid_nodes; i++) {
    add_wide_offset(mid_nodes[i], c);
  }
  c->எடு_pack(பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_isolation_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_4 = add_object ("4", 1, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_4, c);
  unsigned obj_3 = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_4, c);
  unsigned obj_2 = c->எடு_pack (பொய்);

  start_object ("1", 1, c);
  add_wide_offset (obj_3, c);
  add_offset (obj_2, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_isolation_overflow_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_f = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object ("d", 1, c);
  add_offset (obj_e, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d, c);
  unsigned obj_h = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_c, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_d, c);
  unsigned obj_g = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 11000, c);
  add_offset (obj_d, c);
  unsigned obj_i = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_offset (obj_g, c);
  add_offset (obj_i, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_isolation_overflow_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();


  // space 1

  unsigned obj_f_பகாஎண் = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f_பகாஎண், c);
  unsigned obj_e_பகாஎண் = c->எடு_pack (பொய்);

  start_object ("d", 1, c);
  add_offset (obj_e_பகாஎண், c);
  unsigned obj_d_பகாஎண் = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d_பகாஎண், c);
  unsigned obj_h = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_e_பகாஎண், c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_c, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  // space 0

  unsigned obj_f = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->எடு_pack (பொய்);


  start_object ("d", 1, c);
  add_offset (obj_e, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 11000, c);
  add_offset (obj_d, c);
  unsigned obj_i = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_d, c);
  unsigned obj_g = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_offset (obj_g, c);
  add_offset (obj_i, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_isolation_overflow_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_d = add_object ("f", 1, c);
  unsigned obj_e = add_object ("f", 1, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->எடு_pack ();

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack ();


  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->எடு_pack ();

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_repack_last (hb_serialize_context_t* c, தருக்க with_overflow)
{
  std::string large_string(70000, 'c');
  c->start_serialize<எழுத்து> ();
  c->push();

  // Obj E
  unsigned obj_e_1, obj_e_2;
  எனில் (with_overflow) {
    obj_e_1 = add_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } இல்லையெனில் {
    obj_e_2 = add_object("a", 1, c);
  }

  // Obj D
  c->push();
  add_offset(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  unsigned obj_d = c->எடு_pack(பொய்);

  add_offset(obj_d, c);
  hb_always_assert(c->last_added_child_index() == obj_d);

  எனில் (!with_overflow) {
    obj_e_1 = add_object("a", 1, c);
  }

  // Obj C
  c->push();
  add_offset(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  unsigned obj_c = c->எடு_pack(பொய்);

  add_offset(obj_c, c);

  // Obj B
  unsigned obj_b = add_object("b", 1, c);
  add_offset(obj_b, c);

  // Obj A
  c->repack_last(obj_d);
  c->எடு_pack(பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_spaces (hb_serialize_context_t* c, தருக்க with_overflow)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_i;

  எனில் (with_overflow)
    obj_i = add_object ("i", 1, c);

  // Space 2
  unsigned obj_h = add_object ("h", 1, c);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_h, c);
  unsigned obj_e = c->எடு_pack (பொய்);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  // Space 1
  எனில் (!with_overflow)
    obj_i = add_object ("i", 1, c);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_i, c);
  unsigned obj_g = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_i, c);
  unsigned obj_f = c->எடு_pack (பொய்);

  start_object ("d", 1, c);
  add_offset (obj_g, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_f, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_g = add_object ("g", 1, c);
  unsigned obj_h = add_object ("h", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  unsigned obj_e = c->எடு_pack (பொய்);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_h, c);
  unsigned obj_f = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object ("d", 1, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_g_பகாஎண் = add_object ("g", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g_பகாஎண், c);
  unsigned obj_e_பகாஎண் = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_e_பகாஎண், c);
  unsigned obj_c = c->எடு_pack (பொய்);

  unsigned obj_h_பகாஎண் = add_object ("h", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_h_பகாஎண், c);
  unsigned obj_f = c->எடு_pack (பொய்);

  start_object ("d", 1, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  unsigned obj_g = add_object ("g", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  unsigned obj_e = c->எடு_pack (பொய்);

  unsigned obj_h = add_object ("h", 1, c);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->எடு_pack (பொய்);

  c->end_serialize ();
}

நிலையான வெறுமை
populate_serializer_short_and_wide_subgraph_வேர் (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_c, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object ("b", 1, c);
  add_offset (obj_c, c);
  add_offset (obj_e, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_short_and_wide_subgraph_வேர்_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_e_பகாஎண் = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e_பகாஎண், c);
  unsigned obj_c_பகாஎண் = c->எடு_pack (பொய்);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_c_பகாஎண், c);
  unsigned obj_d = c->எடு_pack (பொய்);

  unsigned obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);


  start_object ("b", 1, c);
  add_offset (obj_c, c);
  add_offset (obj_e, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c_பகாஎண், c);
  add_wide_offset (obj_d, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_split_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->எடு_pack (பொய்);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_split_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->எடு_pack (பொய்);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_split_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_f_பகாஎண் = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_பகாஎண், c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_split_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->start_serialize<எழுத்து> ();

  // Space 2

  unsigned obj_f_double_பகாஎண் = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_double_பகாஎண், c);
  unsigned obj_d_பகாஎண் = c->எடு_pack (பொய்);

  start_object ("b", 1, c);
  add_offset (obj_d_பகாஎண், c);
  unsigned obj_b_பகாஎண் = c->எடு_pack (பொய்);

  // Space 1

  unsigned obj_f_பகாஎண் = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_பகாஎண், c);
  unsigned obj_e = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  // Space 0

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  //வேர்
  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_b_பகாஎண், c);
  add_wide_offset (obj_c, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->start_serialize<எழுத்து> ();

  unsigned obj_5 = add_object ("mn", 2, c);

  unsigned obj_4 = add_object ("jkl", 3, c);

  start_object ("ghi", 3, c);
  add_offset (obj_4, c);
  unsigned obj_3 = c->எடு_pack (பொய்);

  start_object ("def", 3, c);
  add_offset (obj_3, c);
  unsigned obj_2 = c->எடு_pack (பொய்);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_4, c);
  add_offset (obj_5, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->start_serialize<எழுத்து> ();

  unsigned obj_6 = add_object ("opqrst", 6, c);

  unsigned obj_5 = add_object ("mn", 2, c);

  start_object ("jkl", 3, c);
  add_offset (obj_6, c);
  unsigned obj_4 = c->எடு_pack (பொய்);

  start_object ("ghi", 3, c);
  add_offset (obj_4, c);
  unsigned obj_3 = c->எடு_pack (பொய்);

  start_object ("def", 3, c);
  add_offset (obj_3, c);
  unsigned obj_2 = c->எடு_pack (பொய்);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_4, c);
  add_offset (obj_5, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->start_serialize<எழுத்து> ();

  unsigned obj_d = add_object ("d", 1, c);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("e", 1, c);
  add_virtual_offset (obj_b, c);
  unsigned obj_e = c->எடு_pack (பொய்);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_24_and_32_bit_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned obj_f = add_object ("f", 1, c);
  unsigned obj_g = add_object ("g", 1, c);
  unsigned obj_j = add_object ("j", 1, c);
  unsigned obj_k = add_object ("k", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_c = c->எடு_pack (பொய்);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  unsigned obj_d = c->எடு_pack (பொய்);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_j, c);
  unsigned obj_h = c->எடு_pack (பொய்);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_k, c);
  unsigned obj_i = c->எடு_pack (பொய்);

  start_object ("e", 1, c);
  add_wide_offset (obj_h, c);
  add_wide_offset (obj_i, c);
  unsigned obj_e = c->எடு_pack (பொய்);

  start_object ("b", 1, c);
  add_24_offset (obj_c, c);
  add_24_offset (obj_d, c);
  add_24_offset (obj_e, c);
  unsigned obj_b = c->எடு_pack (பொய்);

  start_object ("a", 1, c);
  add_24_offset (obj_b, c);
  c->எடு_pack (பொய்);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              முழு இலக்கம்_extensions = 0,
                                              தருக்க shared_subtables = பொய்,
                                              தருக்க version_1_2 = பொய்)
{
  constexpr முழு இலக்கம்_lookups = 5;
  constexpr முழு இலக்கம்_subtables = இலக்கம்_lookups * 2;
  unsigned முழு lookups[இலக்கம்_lookups];
  unsigned முழு subtables[இலக்கம்_subtables];
  unsigned முழு extensions[இலக்கம்_subtables];

  std::string large_string(60000, 'a');
  c->start_serialize<எழுத்து> ();


  க்கு (முழு i = இலக்கம்_subtables - 1; i >= 0; i--)
    subtables[i] = add_object(large_string.c_str (), 15000 + i, c);

  க்கு (முழு i = இலக்கம்_subtables - 1;
       i >= (இலக்கம்_lookups - இலக்கம்_extensions) * 2;
       i--)
  {
    extensions[i] = add_extension (subtables[i], 5, c);
  }

  க்கு (முழு i = இலக்கம்_lookups - 1; i >= 0; i--)
  {
    தருக்க is_ext = (i >= (இலக்கம்_lookups - இலக்கம்_extensions));

    start_lookup (is_ext ? (எழுத்து) 7 : (எழுத்து) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    எனில் (is_ext) {
      எனில் (shared_subtables && i > 2) {
        add_offset (extensions[i * 2 - 1], c);
      }
      add_offset (extensions[i * 2], c);
      add_offset (extensions[i * 2 + 1], c);
    } இல்லையெனில் {
      எனில் (shared_subtables && i > 2) {
        add_offset (subtables[i * 2 - 1], c);
      }
      add_offset (subtables[i * 2], c);
      add_offset (subtables[i * 2 + 1], c);
    }

    lookups[i] = finish_lookup (c);
  }

  unsigned lookup_list = version_1_2
                         ? add_lookup_list_1_2 (lookups, இலக்கம்_lookups, c)
                         : add_lookup_list (lookups, இலக்கம்_lookups, c);

  எனில் (version_1_2)
    add_gsubgpos_header_1_2 (lookup_list, c);
  இல்லையெனில்
    add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<முழு இலக்கம்_pair_pos_1, முழு இலக்கம்_pair_set>
நிலையான வெறுமை
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           தருக்க as_extension = பொய்)
{
  std::string large_string(60000, 'a');
  c->start_serialize<எழுத்து> ();

  constexpr முழு total_pair_set = இலக்கம்_pair_pos_1 * இலக்கம்_pair_set;
  unsigned pair_set[total_pair_set];
  unsigned coverage[இலக்கம்_pair_pos_1];
  unsigned pair_pos_1[இலக்கம்_pair_pos_1];

  க்கு (முழு i = இலக்கம்_pair_pos_1 - 1; i >= 0; i--)
  {
    க்கு (முழு j = (i + 1) * இலக்கம்_pair_set - 1; j >= i * இலக்கம்_pair_set; j--)
      pair_set[j] = add_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = add_coverage (i * இலக்கம்_pair_set,
                                (i + 1) * இலக்கம்_pair_set - 1, c);

    pair_pos_1[i] = add_pair_pos_1 (&pair_set[i * இலக்கம்_pair_set],
                                    இலக்கம்_pair_set,
                                    coverage[i],
                                    c);
  }

  unsigned pair_pos_2 = add_object (large_string.c_str(), 200, c);

  எனில் (as_extension) {
    pair_pos_2 = add_extension (pair_pos_2, 2, c);
    க்கு (முழு i = இலக்கம்_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = add_extension (pair_pos_1[i], 2, c);
  }

  start_lookup (as_extension ? 9 : 2, 1 + இலக்கம்_pair_pos_1, c);

  க்கு (முழு i = 0; i < இலக்கம்_pair_pos_1; i++)
    add_offset (pair_pos_1[i], c);
  add_offset (pair_pos_2, c);

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<முழு இலக்கம்_pair_pos_2, முழு இலக்கம்_class_1, முழு இலக்கம்_class_2>
நிலையான வெறுமை
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           தருக்க as_extension = பொய்,
                                           தருக்க with_device_tables = பொய்,
                                           தருக்க extra_table = உண்மை)
{
  std::string large_string(100000, 'a');
  c->start_serialize<எழுத்து> ();

  unsigned coverage[இலக்கம்_pair_pos_2];
  unsigned class_def_1[இலக்கம்_pair_pos_2];
  unsigned class_def_2[இலக்கம்_pair_pos_2];
  unsigned pair_pos_2[இலக்கம்_pair_pos_2];

  unsigned* device_tables = (unsigned*) calloc (இலக்கம்_pair_pos_2 * இலக்கம்_class_1 * இலக்கம்_class_2,
                                                அளவு(unsigned));

  // Total glyphs = num_class_1 * num_pair_pos_2
  க்கு (முழு i = இலக்கம்_pair_pos_2 - 1; i >= 0; i--)
  {
    unsigned start_glyph = 5 + i * இலக்கம்_class_1;
    எனில் (இலக்கம்_class_2 >= இலக்கம்_class_1)
    {
      class_def_2[i] = add_class_def (11,
                                      10 + இலக்கம்_class_2, c);
      class_def_1[i] = add_class_def (start_glyph + 1,
                                      start_glyph + இலக்கம்_class_1,
                                      c);
    } இல்லையெனில் {
      class_def_1[i] = add_class_def (start_glyph + 1,
                                      start_glyph + இலக்கம்_class_1,
                                      c);
      class_def_2[i] = add_class_def (11,
                                      10 + இலக்கம்_class_2, c);
    }

    coverage[i] = add_coverage (start_glyph,
                                start_glyph + இலக்கம்_class_1 - 1,
                                c);

    எனில் (with_device_tables)
    {
      க்கு(முழு j = (i + 1) * இலக்கம்_class_1 * இலக்கம்_class_2 - 1;
          j >= i * இலக்கம்_class_1 * இலக்கம்_class_2;
          j--)
      {
        uint8_t table[] = {
          (uint8_t) ((j >> 8) & 0xFF),
          (uint8_t) (j & 0xFF),
        };
        device_tables[j] = add_object ((எழுத்து*) table, 2, c);
      }
    }

    pair_pos_2[i] = add_pair_pos_2 (1 + i * இலக்கம்_class_1,
                                    coverage[i],
                                    class_def_1[i], இலக்கம்_class_1,
                                    class_def_2[i], இலக்கம்_class_2,
                                    with_device_tables
                                    ? &device_tables[i * இலக்கம்_class_1 * இலக்கம்_class_2]
                                    : nullptr,
                                    c);
  }


  unsigned pair_pos_1 = 0;
  எனில் (extra_table) pair_pos_1 = add_object (large_string.c_str(), 100000, c);

  எனில் (as_extension) {
    க்கு (முழு i = இலக்கம்_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = add_extension (pair_pos_2[i], 2, c);

    எனில் (extra_table)
      pair_pos_1 = add_extension (pair_pos_1, 2, c);
  }

  start_lookup (as_extension ? 9 : 2, 1 + இலக்கம்_pair_pos_2, c);

  எனில் (extra_table)
    add_offset (pair_pos_1, c);

  க்கு (முழு i = 0; i < இலக்கம்_pair_pos_2; i++)
    add_offset (pair_pos_2[i], c);

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();

  விடுவி (device_tables);
}

template<முழு mark_மொத்தம்,
    முழு class_மொத்தம்,
    முழு base_மொத்தம்,
    முழு table_மொத்தம்_2>
நிலையான வெறுமை
populate_serializer_with_large_mark_base_pos_1 (hb_serialize_context_t* c)
{
  c->start_serialize<எழுத்து> ();

  MarkBasePosBuffers<mark_மொத்தம், class_மொத்தம், base_மொத்தம், table_மொத்தம்_2> buffers (c);

  unsigned mark_base_pos[table_மொத்தம்_2];
  க்கு (unsigned i = 0; i < table_மொத்தம்_2; i++)
    mark_base_pos[i] = buffers.create_mark_base_pos_1 (i, c);

  க்கு (முழு i = 0; i < table_மொத்தம்_2; i++)
    mark_base_pos[i] = add_extension (mark_base_pos[i], 4, c);

  start_lookup (9, table_மொத்தம்_2, c);

  க்கு (முழு i = 0; i < table_மொத்தம்_2; i++)
    add_offset (mark_base_pos[i], c);

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<unsigned Lig_Subst_மொத்தம்,
         unsigned Liga_Set_மொத்தம்,
         unsigned Liga_Per_Set_மொத்தம்,
         unsigned LigaSize>
நிலையான வெறுமை populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    தருக்க sequential_liga_sets,
                                                    தருக்க shared,
                                                    unsigned *liga_subst_id,
                                                    தருக்க unique_lig_str = பொய்)
{
  எழுத்து ch = 'a';

  unsigned liga[Liga_Set_மொத்தம் * Liga_Per_Set_மொத்தம்];
  unsigned liga_set[Liga_Set_மொத்தம்];
  க்கு (unsigned l = 0; l < Lig_Subst_மொத்தம்; l++) {
    unsigned coverage_start = 0;
    unsigned coverage_end = Liga_Set_மொத்தம் - 1;
    எனில் (sequential_liga_sets) {
      coverage_start = l * Liga_Set_மொத்தம்;
      coverage_end = (l + 1) * Liga_Set_மொத்தம் - 1;
    }
    unsigned coverage = add_coverage(coverage_start, coverage_end, c, shared);

    க்கு (unsigned i = 0; i < Liga_Set_மொத்தம்; i++) {
      க்கு (unsigned j = 0; j < Liga_Per_Set_மொத்தம்; j++)
      {
        std::string large_string(100000, ch);
        start_object (large_string.c_str(), LigaSize, c);
        எனில் (unique_lig_str)
          ch += 1;
        add_virtual_offset(coverage, c);
        liga[i * Liga_Per_Set_மொத்தம் + j] = c->எடு_pack (shared);
      }

      add_liga_set_header(Liga_Per_Set_மொத்தம், c);
      add_virtual_offset(coverage, c);
      க்கு (unsigned j = 0; j < Liga_Per_Set_மொத்தம்; j++)
      {
        add_offset(liga[i * Liga_Per_Set_மொத்தம் + j], c);
      }

      liga_set[i] = c->எடு_pack(shared);
    }

    add_liga_header(coverage, Liga_Set_மொத்தம், c);
    க்கு (unsigned i = 0; i < Liga_Set_மொத்தம்; i++)
    {
      add_offset(liga_set[i], c);
    }

    liga_subst_id[l] = c->எடு_pack(shared);
  }

  க்கு (unsigned l = 0; l < Lig_Subst_மொத்தம்; l++) {
    liga_subst_id[l] = add_extension(liga_subst_id[l], 4, c);
  }
}

template<unsigned liga_subst_மொத்தம்,
         unsigned liga_set_மொத்தம்,
         unsigned liga_per_set_மொத்தம்,
         unsigned liga_size>
நிலையான வெறுமை
populate_serializer_with_large_liga (hb_serialize_context_t* c, தருக்க sequential_liga_sets)
{
  c->start_serialize<எழுத்து> ();

  unsigned liga_subst[liga_subst_மொத்தம்];
  populate_serializer_with_large_ligsubst<liga_subst_மொத்தம், liga_set_மொத்தம், liga_per_set_மொத்தம், liga_size>(c, sequential_liga_sets, பொய், liga_subst);

  start_lookup (7, liga_subst_மொத்தம், c);
  க்கு (unsigned l = 0; l < liga_subst_மொத்தம்; l++) {
    add_offset(liga_subst[l], c);
  }

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

நிலையான வெறுமை
populate_serializer_with_large_liga_overlapping_clone_result (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->start_serialize<எழுத்து> ();

  constexpr unsigned liga_size = 30000;

  unsigned liga[6];
  unsigned liga_subst[3];
  unsigned liga_set[2];

  // LigaSubst 3
  unsigned coverage = add_coverage(1, 1, c);
  க்கு (முழு i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->எடு_pack (பொய்);
  }

  add_liga_set_header(2, c);
  add_virtual_offset(coverage, c);
  க்கு (unsigned i = 0; i < 2; i++)
    add_offset(liga[i], c);
  liga_set[0] = c->எடு_pack(பொய்);

  add_liga_header(coverage, 1, c);
  add_offset(liga_set[0], c);
  liga_subst[2] = c->எடு_pack(பொய்);

  // LigaSubst 2
  coverage = add_coverage(0, 1, c);
  க்கு (முழு i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->எடு_pack (பொய்);
  }

  add_liga_set_header(1, c);
  add_virtual_offset(coverage, c);
  add_offset(liga[1], c);
  liga_set[1] = c->எடு_pack(பொய்);

  add_liga_set_header(1, c);
  add_virtual_offset(coverage, c);
  add_offset(liga[0], c);
  liga_set[0] = c->எடு_pack(பொய்);

  add_liga_header(coverage, 2, c);
  add_offset(liga_set[0], c);
  add_offset(liga_set[1], c);
  liga_subst[1] = c->எடு_pack(பொய்);

  // LigaSubst 1
  coverage = add_coverage(0, 0, c);
  க்கு (முழு i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->எடு_pack (பொய்);
  }

  add_liga_set_header(2, c);
  add_virtual_offset(coverage, c);
  க்கு (unsigned i = 0; i < 2; i++)
    add_offset(liga[i], c);
  liga_set[0] = c->எடு_pack(பொய்);

  add_liga_header(coverage, 1, c);
  add_offset(liga_set[0], c);
  liga_subst[0] = c->எடு_pack(பொய்);

  க்கு (முழு l = 2; l >= 0; l--) {
    liga_subst[l] = add_extension(liga_subst[l], 4, c);
  }

  start_lookup (7, 3, c);
  க்கு (unsigned l = 0; l < 3; l++) {
    add_offset(liga_subst[l], c);
  }

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<unsigned liga_subst_மொத்தம்,
         unsigned liga_set_மொத்தம்,
         unsigned liga_per_set_மொத்தம்,
         unsigned liga_size>
நிலையான வெறுமை
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, தருக்க sequential_liga_sets)
{
  c->start_serialize<எழுத்து> ();

  unsigned lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  unsigned liga_subst_idx[liga_subst_மொத்தம்];
  populate_serializer_with_large_ligsubst<liga_subst_மொத்தம், liga_set_மொத்தம், liga_per_set_மொத்தம், liga_size>(c, sequential_liga_sets, உண்மை, liga_subst_idx, உண்மை);

  start_lookup (7, liga_subst_மொத்தம், c);
  க்கு (unsigned l = 0; l < liga_subst_மொத்தம்; l++) {
    add_offset(liga_subst_idx[l], c);
  }

  lookups[0] = finish_lookup (c);

  // Lookup with 2 LigSubst tables
  unsigned liga_subst[liga_subst_மொத்தம் + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, பொய், liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_மொத்தம், liga_set_மொத்தம், liga_per_set_மொத்தம், liga_size>(c, sequential_liga_sets, உண்மை, &liga_subst[1], உண்மை);

  start_lookup (7, liga_subst_மொத்தம் + 1, c);
  க்கு (unsigned l = 0; l <= liga_subst_மொத்தம்; l++) {
    add_offset(liga_subst[l], c);
  }

  lookups[1] = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (lookups, 2, c);
  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<unsigned liga_subst_மொத்தம்,
         unsigned liga_set_மொத்தம்,
         unsigned liga_per_set_மொத்தம்,
         unsigned liga_size>
நிலையான வெறுமை
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->start_serialize<எழுத்து> ();

  unsigned liga_subst[liga_subst_மொத்தம் + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, உண்மை, உண்மை, liga_subst, உண்மை);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_மொத்தம், liga_set_மொத்தம், liga_per_set_மொத்தம், liga_size>(c, உண்மை, உண்மை, &liga_subst[1], உண்மை);

  start_lookup (7, liga_subst_மொத்தம் + 1, c);
  க்கு (unsigned l = 0; l <= liga_subst_மொத்தம்; l++) {
    add_offset(liga_subst[l], c);
  }

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);
  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

நிலையான வெறுமை test_sort_shortest ()
{
  size_t buffer_size = 100;
  வெறுமை* buffer_a = malloc (buffer_size);
  வெறுமை* buffer_e = malloc (buffer_size);
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
  unsigned ghi = e.எடு_pack(பொய்);

  start_object("def", 3, &e);
  add_offset(ghi, &e);
  unsigned def = e.எடு_pack(பொய்);

  unsigned mn = add_object("mn", 2, &e);

  start_object ("abc", 3, &e);
  add_offset (def, &e);
  add_offset (jkl, &e);
  add_offset (mn, &e);
  e.எடு_pack (பொய்);
  e.end_serialize();

  graph_t expected = graph_t::create (e.object_graph ()).value ();
  expected.normalize();

  ப்ரதிஜ்ஞா(expected == graph);

  விடுவி (buffer_a);
  விடுவி (buffer_e);
}

நிலையான வெறுமை test_duplicate_leaf ()
{
  size_t buffer_size = 100;
  வெறுமை* buffer_a = malloc (buffer_size);
  வெறுமை* buffer_e = malloc (buffer_size);
  hb_serialize_context_t a (buffer_a, buffer_size);
  hb_serialize_context_t e (buffer_e, buffer_size);
  populate_serializer_complex_2 (&a);

  graph_t graph = graph_t::create (a.object_graph ()).value ();
  hb_always_assert (graph.duplicate (4, 1, பொய்).is_ok ());
  graph.normalize();

  e.start_serialize();
  unsigned mn = add_object("mn", 2, &e);
  unsigned jkl_2 = add_object("jkl", 3, &e);

  start_object("ghi", 3, &e);
  add_offset(jkl_2, &e);
  unsigned ghi = e.எடு_pack(பொய்);

  start_object("def", 3, &e);
  add_offset(ghi, &e);
  unsigned def = e.எடு_pack(பொய்);

  unsigned jkl_1 = add_object("jkl", 3, &e);

  start_object("abc", 3, &e);
  add_offset(def, &e);
  add_offset(jkl_1, &e);
  add_offset(mn, &e);
  e.எடு_pack(பொய்);

  graph_t expected = graph_t::create (e.object_graph ()).value ();
  expected.normalize();

  ப்ரதிஜ்ஞா(expected == graph);

  விடுவி (buffer_a);
  விடுவி (buffer_e);
}

நிலையான வெறுமை test_duplicate_interior ()
{
  size_t buffer_size = 100;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_3 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();
  hb_always_assert (graph.duplicate (3, 2, பொய்).is_ok ());

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

  விடுவி (buffer);
}

நிலையான வெறுமை
test_serialize ()
{
  size_t buffer_size = 100;
  வெறுமை* buffer_1 = malloc (buffer_size);
  hb_serialize_context_t c1 (buffer_1, buffer_size);
  populate_serializer_simple (&c1);
  hb_bytes_t expected = c1.copy_bytes ();

  graph_t graph = graph_t::create (c1.object_graph ()).value ();
  hb_blob_t* out = graph::serialize (graph).value ();
  விடுவி (buffer_1);

  hb_bytes_t actual = out->as_bytes ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (out);
}

நிலையான வெறுமை test_will_overflow_1 ()
{
  size_t buffer_size = 100;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  populate_serializer_complex_2 (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (!graph::will_overflow (graph, nullptr).value ());

  விடுவி (buffer);
}

நிலையான வெறுமை test_will_overflow_2 ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (graph::will_overflow (graph, nullptr).value ());

  விடுவி (buffer);
}

நிலையான வெறுமை test_will_overflow_3 ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (graph::will_overflow (graph, nullptr).value ());

  விடுவி (buffer);
}

நிலையான வெறுமை test_resolve_overflows_via_sort ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == (80000 + 3 + 3 * 2));

  விடுவி (buffer);
  hb_blob_destroy (out);
}

நிலையான வெறுமை test_resolve_overflows_via_duplication ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  விடுவி (buffer);
  hb_blob_destroy (out);
}

நிலையான வெறுமை test_resolve_overflows_via_multiple_duplications ()
{
  size_t buffer_size = 300000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_multiple_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE, 5);
  hb_always_assert (out);

  விடுவி (buffer);
  hb_blob_destroy (out);
}

நிலையான வெறுமை test_resolve_overflows_via_space_assignment ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_spaces (&c, உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_spaces (&e, பொய்);

  run_resolve_overflow_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_overflows_via_isolation ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_isolation_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (c.offset_overflow ());
  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  விடுவி (buffer);
  hb_blob_destroy (out);
}

நிலையான வெறுமை test_resolve_overflows_via_isolation_with_recursive_duplication ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_isolation_overflow_complex (&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_isolation_overflow_complex_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_overflows_via_isolating_16bit_space ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_spaces_16bit_connection (&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_spaces_16bit_connection_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_overflows_via_isolating_16bit_space_2 ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_short_and_wide_subgraph_வேர் (&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_short_and_wide_subgraph_வேர்_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_overflows_via_isolation_spaces ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
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

  விடுவி (buffer);
  hb_blob_destroy (out);
}

நிலையான வெறுமை test_resolve_mixed_overflows_via_isolation_spaces ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
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

  விடுவி (buffer);
  hb_blob_destroy (out);
}

நிலையான வெறுமை test_resolve_with_extension_promotion ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             உண்மை);
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_shared_extension_promotion ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c, 0, உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3, உண்மை);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             உண்மை);
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

#ifndef HB_NO_BEYOND_64K
நிலையான வெறுமை test_resolve_with_extension_promotion_1_2 ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c, 0, பொய், உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3, பொய், உண்மை);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             உண்மை);
  விடுவி (buffer);
  விடுவி (expected_buffer);
}
#endif

நிலையான வெறுமை test_resolve_with_repeated_extension_promotion ()
{
  size_t buffer_size = 1000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize<எழுத்து> ();
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
  unsigned lookup_index = *context.lookups.சாவிகள் ();
  graph::Lookup* lookup_table = context.lookups.get (lookup_index);
  hb_always_assert (lookup_table->make_extension (context, lookup_index).is_ok ());

  மாறிலி auto& lookup_vertex = graph.vertices_[lookup_index];
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
  விடுவி (buffer);
}

நிலையான வெறுமை test_resolve_with_basic_pair_pos_1_split ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, உண்மை);

  run_resolve_overflow_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'P', 'O', 'S'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_extension_pair_pos_1_split ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, உண்மை);

  run_resolve_overflow_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'P', 'O', 'S'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_basic_pair_pos_2_split ()
{
  size_t buffer_size = 300000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, உண்மை);

  run_resolve_overflow_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'P', 'O', 'S'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_close_to_limit_pair_pos_2_split ()
{
  size_t buffer_size = 300000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, உண்மை, பொய், பொய்);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, உண்மை, பொய், பொய்);

  run_resolve_overflow_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'P', 'O', 'S'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_pair_pos_2_split_with_device_tables ()
{
  size_t buffer_size = 300000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, பொய், உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, உண்மை, உண்மை);

  run_resolve_overflow_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'P', 'O', 'S'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_basic_mark_base_pos_1_split ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_mark_base_pos_1 <40, 10, 110, 1>(&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_mark_base_pos_1 <40, 10, 110, 2>(&e);

  run_resolve_overflow_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'P', 'O', 'S'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_basic_liga_split ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, பொய்);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, பொய்);

  run_resolve_overflow_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'S', 'U', 'B'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_liga_split_move ()
{
  size_t buffer_size = 400000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, உண்மை);

  run_resolve_overflow_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'S', 'U', 'B'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_liga_split_overlapping_clone ()
{
  size_t buffer_size = 400000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga_overlapping_clone_result(&e);

  run_resolve_overflow_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'S', 'U', 'B'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_liga_split_shared_table ()
{
  size_t buffer_size = 400000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, உண்மை);

  run_resolve_overflow_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'S', 'U', 'B'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_with_liga_split_shared_coverage ()
{
  size_t buffer_size = 400000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  run_resolve_overflow_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             உண்மை,
                             HB_TAG('G', 'S', 'U', 'B'));
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_overflows_via_splitting_spaces ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_split_spaces (&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_split_spaces_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  விடுவி (buffer);
  விடுவி (expected_buffer);

}

நிலையான வெறுமை test_resolve_overflows_via_splitting_spaces_2 ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_split_spaces_2 (&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_split_spaces_expected_2 (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை test_resolve_overflows_via_priority ()
{
  size_t buffer_size = 160000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_priority_overflow (&c);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_priority_overflow_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  விடுவி (buffer);
  விடுவி (expected_buffer);
}


நிலையான வெறுமை test_virtual_link ()
{
  size_t buffer_size = 100;
  வெறுமை* buffer = malloc (buffer_size);
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

  விடுவி (buffer);
  hb_blob_destroy (out);
}

நிலையான வெறுமை
test_shared_முனை_with_virtual_links ()
{
  size_t buffer_size = 100;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize<எழுத்து> ();

  unsigned obj_b = add_object ("b", 1, &c);
  unsigned obj_c = add_object ("c", 1, &c);

  start_object ("d", 1, &c);
  add_virtual_offset (obj_b, &c);
  unsigned obj_d_1 = c.எடு_pack ();

  start_object ("d", 1, &c);
  add_virtual_offset (obj_c, &c);
  unsigned obj_d_2 = c.எடு_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  start_object ("a", 1, &c);
  add_offset (obj_b, &c);
  add_offset (obj_c, &c);
  add_offset (obj_d_1, &c);
  add_offset (obj_d_2, &c);
  c.எடு_pack ();
  c.end_serialize ();

  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links.length == 2);
  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links[1].objidx == obj_c);
  விடுவி(buffer);
}

நிலையான வெறுமை
test_repack_last ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_repack_last (&c, உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_repack_last (&e, பொய்);

  run_resolve_overflow_test ("test_repack_last",
                             c,
                             e,
                             20,
                             பொய்,
                             HB_TAG('a', 'b', 'c', 'd'),
                             உண்மை);

  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை
test_dont_duplicate_virtual ()
{
  size_t buffer_size = 200000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_virtual (&c, உண்மை);

  வெறுமை* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_virtual (&e, பொய்);

  run_resolve_overflow_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             பொய்,
                             HB_TAG('a', 'b', 'c', 'd'),
                             உண்மை);

  விடுவி (buffer);
  விடுவி (expected_buffer);
}

நிலையான வெறுமை
test_deep_graph_traversal ()
{
  size_t buffer_size = 1000000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize ();
  unsigned prev = add_object ("leaf", 4, &c);
  க்கு (unsigned i = 0; i < 2000; i++)
  {
    start_object ("node", 4, &c);
    add_offset (prev, &c);
    prev = c.எடு_pack (பொய்);
  }
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // Test find_subgraph (set)
  hb_set_t visited_set;
  hb_always_assert (graph.find_subgraph (graph.வேர்_idx (), visited_set).is_ok ());
  hb_always_assert (visited_set.get_population () == 2001);

  // Test find_subgraph_size
  hb_set_t size_set;
  size_t sz = graph.find_subgraph_size (graph.வேர்_idx (), size_set).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.set (graph.வேர்_idx (), 1);
  hb_always_assert (graph.find_subgraph (graph.வேர்_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (graph.assign_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t index_map;
  hb_always_assert (graph.duplicate_subgraph (graph.வேர்_idx (), index_map).is_ok ());
  hb_always_assert (index_map.get_population () == 2001);

  விடுவி (buffer);
}

நிலையான வெறுமை
test_32bit_roots_traversal ()
{
  size_t buffer_size = 1000;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize ();
  unsigned leaf1 = add_object ("l1", 2, &c);
  unsigned leaf2 = add_object ("l2", 2, &c);

  start_object ("mid", 3, &c);
  add_wide_offset (leaf1, &c);
  unsigned mid = c.எடு_pack (பொய்);

  start_object ("root", 4, &c);
  add_offset (mid, &c);
  add_wide_offset (leaf2, &c);
  c.எடு_pack (பொய்);
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_set_t roots;
  hb_always_assert (graph.find_32bit_roots (graph.வேர்_idx (), roots).is_ok ());
  hb_always_assert (roots.has (leaf1 - 1));
  hb_always_assert (roots.has (leaf2 - 1));
  hb_always_assert (!roots.has (mid - 1));
  hb_always_assert (roots.get_population () == 2);

  விடுவி (buffer);

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
  unsigned a = c2.எடு_pack (பொய்);

  start_object ("r", 1, &c2);
  add_offset (a, &c2);
  add_wide_offset (a, &c2);
  c2.எடு_pack (பொய்);

  c2.end_serialize ();

  graph_t graph2 = graph_t::create (c2.object_graph ()).value ();

  hb_set_t roots2;
  hb_always_assert (graph2.find_32bit_roots (graph2.வேர்_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  விடுவி (buffer);
}

நிலையான வெறுமை test_invalid_link_objidx ()
{
  size_t buffer_size = 100;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_2 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // Poison a link to point to (unsigned) -1
  auto links = graph.vertices_[graph.வேர்_idx ()].real_links_writer();
  links->objidx = (unsigned) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!graph.sort_shortest_distance ().is_ok ());

  விடுவி (buffer);
}

நிலையான வெறுமை test_invalid_link_add_and_move ()
{
  size_t buffer_size = 100;
  வெறுமை* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_2 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!graph.add_link (&dummy, 0, (unsigned) -1).is_ok ());

  // move_child with non-existent offset
  auto res = graph.move_child (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  விடுவி (buffer);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

முழு
முதன்மை (முழு argc, எழுத்து **argv)
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
  test_shared_முனை_with_virtual_links ();
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
