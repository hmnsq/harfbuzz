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

#ଅନ୍ତର୍ଭାଵ <cstdint>
#ଅନ୍ତର୍ଭାଵ <string>

#ଅନ୍ତର୍ଭାଵ "hb-repacker.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "graph/serialize.hh"

ସ୍ଥିର ଶୂନ୍ୟ extend (ଅଚଳ ଅକ୍ଷର* value,
                    unsigned len,
                    hb_serialize_context_t* c)
{
  ଅକ୍ଷର* obj = c->allocate_size<ଅକ୍ଷର> (len);
  hb_memcpy (obj, value, len);
}

ସ୍ଥିର ଶୂନ୍ୟ start_object(ଅଚଳ ଅକ୍ଷର* tag,
                         unsigned len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (tag, len, c);
}

ସ୍ଥିର unsigned add_object(ଅଚଳ ଅକ୍ଷର* tag,
                           unsigned len,
                           hb_serialize_context_t* c,
                           ତାର୍କିକ shared = ମିଥ୍ୟା)
{
  start_object (tag, len, c);
  ଫେରନ୍ତୁ c->କାଢ଼_pack (shared);
}


ସ୍ଥିର ଶୂନ୍ୟ add_offset (unsigned id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* offset = c->start_embed<OT::Offset16> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

ସ୍ଥିର ଶୂନ୍ୟ add_24_offset (unsigned id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* offset = c->start_embed<OT::Offset24> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

ସ୍ଥିର ଶୂନ୍ୟ add_wide_offset (unsigned id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* offset = c->start_embed<OT::Offset32> ();
  c->extend_min (offset);
  c->add_link (*offset, id);
}

ସ୍ଥିର ଶୂନ୍ୟ add_gsubgpos_header (unsigned lookup_list,
                                 hb_serialize_context_t* c)
{
  ଅକ୍ଷର header[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  start_object (header, 8, c);
  add_offset (lookup_list, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);
}

ସ୍ଥିର ଶୂନ୍ୟ add_gsubgpos_header_1_2 (unsigned lookup_list,
                                     hb_serialize_context_t* c)
{
  ଅକ୍ଷର header[] = {
    0, 1, 0, 2, // version
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  start_object (header, ଆକାର (header), c);
  add_wide_offset (lookup_list, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);
}

ସ୍ଥିର unsigned add_lookup_list (ଅଚଳ unsigned* lookups,
                                 ଅକ୍ଷର ଗଣନା_2,
                                 hb_serialize_context_t* c)
{
  ଅକ୍ଷର lookup_ଗଣନା[] = {0, ଗଣନା_2};
  start_object  ((ଅକ୍ଷର *) &lookup_ଗଣନା, 2, c);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    add_offset (lookups[i], c);

  ଫେରନ୍ତୁ c->କାଢ଼_pack (ମିଥ୍ୟା);
}

ସ୍ଥିର unsigned add_lookup_list_1_2 (ଅଚଳ unsigned* lookups,
                                     ଅକ୍ଷର ଗଣନା_2,
                                     hb_serialize_context_t* c)
{
  ଅକ୍ଷର lookup_ଗଣନା[] = {0, ଗଣନା_2};
  start_object ((ଅକ୍ଷର *) &lookup_ଗଣନା, 2, c);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    add_wide_offset (lookups[i], c);

  ଫେରନ୍ତୁ c->କାଢ଼_pack (ମିଥ୍ୟା);
}

ସ୍ଥିର ଶୂନ୍ୟ start_lookup (int8_t type,
                          int8_t ଅଙ୍କ_subtables,
                          hb_serialize_context_t* c)
{
  ଅକ୍ଷର lookup[] = {
    0, (ଅକ୍ଷର)type, // type
    0, (ଅକ୍ଷର)2, // flag
    0, (ଅକ୍ଷର)ଅଙ୍କ_subtables, // num subtables
  };

  start_object (lookup, 6, c);
}

ସ୍ଥିର unsigned finish_lookup (hb_serialize_context_t* c)
{
  ଅକ୍ଷର filter[] = {0, 0};
  extend (filter, 2, c);
  ଫେରନ୍ତୁ c->କାଢ଼_pack (ମିଥ୍ୟା);
}

ସ୍ଥିର unsigned add_extension (unsigned child,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  ଅକ୍ଷର ext[] = {
    0, 1,
    0, (ଅକ୍ଷର) type,
  };

  start_object (ext, 4, c);
  add_wide_offset (child, c);

  ଫେରନ୍ତୁ c->କାଢ଼_pack (ମିଥ୍ୟା);

}

// Adds coverage table fro [start, end]
ସ୍ଥିର unsigned add_coverage (unsigned start, unsigned end,
                              hb_serialize_context_t* c,
                              ତାର୍କିକ shared = ମିଥ୍ୟା)
{
  ଯଦି (end - start == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //ଗଣନା

      (uint8_t) ((start >> 8) & 0xFF),
      (uint8_t) (start & 0xFF), // glyph[0]
    };
    ଫେରନ୍ତୁ add_object ((ଅକ୍ଷର*) coverage, 6, c, shared);
  }

  ଯଦି (end - start == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //ଗଣନା

      (uint8_t) ((start >> 8) & 0xFF),
      (uint8_t) (start & 0xFF), // glyph[0]

      (uint8_t) ((end >> 8) & 0xFF),
      (uint8_t) (end & 0xFF), // glyph[1]
    };
    ଫେରନ୍ତୁ add_object ((ଅକ୍ଷର*) coverage, 8, c, shared);
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
  ଫେରନ୍ତୁ add_object ((ଅକ୍ଷର*) coverage, 10, c, shared);
}


template<typename It>
ସ୍ଥିର unsigned add_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  ଫେରନ୍ତୁ c->କାଢ଼_pack (ମିଥ୍ୟା);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
ସ୍ଥିର unsigned add_class_def (uint16_t start_glyph,
                               uint16_t end_glyph,
                               hb_serialize_context_t* c)
{
  unsigned ଗଣନା_2 = end_glyph - start_glyph;
  uint8_t header[] = {
    0, 1, // format

    (uint8_t) ((start_glyph >> 8) & 0xFF),
    (uint8_t) (start_glyph & 0xFF), // start_glyph

    (uint8_t) ((ଗଣନା_2 >> 8) & 0xFF),
    (uint8_t) (ଗଣନା_2 & 0xFF), //ଗଣନା
  };

  start_object ((ଅକ୍ଷର*) header, 6, c);
  ପାଇଁ (uint16_t i = 1; i <= ଗଣନା_2; i++)
  {
    uint8_t class_ମୂଲ୍ୟ_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //ଗଣନା
    };
    extend ((ଅକ୍ଷର*) class_ମୂଲ୍ୟ_2, 2, c);
  }

  ଫେରନ୍ତୁ c->କାଢ଼_pack (ମିଥ୍ୟା);
}

ସ୍ଥିର unsigned add_pair_pos_1 (unsigned* pair_sets,
                                ଅକ୍ଷର ଗଣନା_2,
                                unsigned coverage,
                                hb_serialize_context_t* c)
{
  ଅକ୍ଷର format[] = {
    0, 1
  };

  start_object (format, 2, c);
  add_offset (coverage, c);

  ଅକ୍ଷର ମୂଲ୍ୟ_format[] = {
    0, 0,
    0, 0,
    0, ଗଣନା_2,
  };
  extend (ମୂଲ୍ୟ_format, 6, c);

  ପାଇଁ (ଅକ୍ଷର i = 0; i < ଗଣନା_2; i++)
    add_offset (pair_sets[(unsigned) i], c);

  ଫେରନ୍ତୁ c->କାଢ଼_pack (ମିଥ୍ୟା);
}

ସ୍ଥିର ଶୂନ୍ୟ add_liga_set_header (ଅକ୍ଷର liga_ଗଣନା,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_ଗଣନା_bytes[] = {
    (uint8_t) (0xFF & (liga_ଗଣନା >> 8)),
    (uint8_t) (0xFF & liga_ଗଣନା),
  };

  start_object ((ଅଚଳ ଅକ୍ଷର*) liga_ଗଣନା_bytes, 2, c);
}

ସ୍ଥିର ଶୂନ୍ୟ add_liga_header (unsigned coverage, unsigned liga_set_ଗଣନା, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  start_object ((ଅଚଳ ଅକ୍ଷର*) format, 2, c);
  add_offset(coverage, c);

  uint8_t liga_set_ଗଣନା_bytes[] = {
    (uint8_t) (0xFF & (liga_set_ଗଣନା >> 8)),
    (uint8_t) (0xFF & liga_set_ଗଣନା),
  };

  extend((ଅଚଳ ଅକ୍ଷର *) liga_set_ଗଣନା_bytes, 2, c);
}

ସ୍ଥିର unsigned add_pair_pos_2 (unsigned starting_class,
                                unsigned coverage,
                                unsigned class_def_1, uint16_t class_def_1_ଗଣନା,
                                unsigned class_def_2, uint16_t class_def_2_ଗଣନା,
                                unsigned* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  start_object ((ଅକ୍ଷର*) format, 2, c);
  add_offset (coverage, c);

  unsigned ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  ଯଦି (device_tables) {
    format2 |= 0x20;
    ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 += 1;
  }
  uint8_t ମୂଲ୍ୟ_format[] = {
    0, format1,
    0, format2,
  };

  extend ((ଅକ୍ଷର*) ମୂଲ୍ୟ_format, 4, c);

  add_offset (class_def_1, c);
  add_offset (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_ଗଣନା >> 8) & 0xFF),
    (uint8_t) (class_def_1_ଗଣନା & 0xFF),
    (uint8_t) ((class_def_2_ଗଣନା >> 8) & 0xFF),
    (uint8_t) (class_def_2_ଗଣନା & 0xFF),
  };
  extend ((ଅକ୍ଷର*) class_counts, 4, c);

  unsigned ଅଙ୍କ_bytes_per_record = class_def_2_ଗଣନା * ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_3 * 2;
  uint8_t* record = (uint8_t*) calloc (1, ଅଙ୍କ_bytes_per_record);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା device_index = 0;
  ପାଇଁ (uint16_t i = 0; i < class_def_1_ଗଣନା; i++)
  {

    ପାଇଁ (uint16_t j = 0; j < class_def_2_ଗଣନା; j++)
    {
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((ଅକ୍ଷର*) value, 2, c);
      }

      ଯଦି (device_tables) {
        add_offset (device_tables[device_index++], c);
      }
    }
  }
  ମୁକ୍ତ (record);

  ଫେରନ୍ତୁ c->କାଢ଼_pack (ମିଥ୍ୟା);
}

ସ୍ଥିର unsigned add_mark_base_pos_1 (unsigned mark_coverage,
                                     unsigned base_coverage,
                                     unsigned mark_array,
                                     unsigned base_array,
                                     unsigned class_ଗଣନା,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  start_object ((ଅକ୍ଷର*) format, 2, c);
  add_offset (mark_coverage, c);
  add_offset (base_coverage, c);

  uint8_t ଗଣନା_2[] = {
    (uint8_t) ((class_ଗଣନା >> 8) & 0xFF),
    (uint8_t) (class_ଗଣନା & 0xFF),
  };
  extend ((ଅକ୍ଷର*) ଗଣନା_2, 2, c);

  add_offset (mark_array, c);
  add_offset (base_array, c);

  ଫେରନ୍ତୁ c->କାଢ଼_pack (ମିଥ୍ୟା);
}

template<ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_ଗଣନା,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_ଗଣନା,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା base_ଗଣନା,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_ଗଣନା_2>
ସଂରଚନା MarkBasePosBuffers
{
  unsigned base_anchors[class_ଗଣନା * base_ଗଣନା];
  unsigned mark_anchors[mark_ଗଣନା];
  uint8_t anchor_buffers[class_ଗଣନା * base_ଗଣନା + 100];
  uint8_t class_buffer[class_ଗଣନା * 2];

  MarkBasePosBuffers(hb_serialize_context_t* c)
  {
    ପାଇଁ (unsigned i = 0; i < ଆକାର(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    ପାଇଁ (unsigned i = 0; i < class_ଗଣନା * base_ଗଣନା; i++)
    {
      base_anchors[i] = add_object ((ଅକ୍ଷର*) &anchor_buffers[i], 100, c);
      ଯଦି (i < class_ଗଣନା) {
        class_buffer[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_buffer[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    ପାଇଁ (unsigned i = 0; i < mark_ଗଣନା; i++)
    {
      mark_anchors[i] = add_object ((ଅକ୍ଷର*) &anchor_buffers[i], 4, c);
    }
  }

  unsigned create_mark_base_pos_1 (unsigned table_index, hb_serialize_context_t* c)
  {
    unsigned class_per_table = class_ଗଣନା / table_ଗଣନା_2;
    unsigned mark_per_class = mark_ଗଣନା / class_ଗଣନା;
    unsigned start_class = class_per_table * table_index;
    unsigned end_class = class_per_table * (table_index + 1) - 1;

    // baseArray
    uint8_t base_ଗଣନା_buffer[] = {
      (uint8_t) ((base_ଗଣନା >> 8) & 0xFF),
      (uint8_t) (base_ଗଣନା & 0xFF),

    };
    start_object ((ଅକ୍ଷର*) base_ଗଣନା_buffer, 2, c);
    ପାଇଁ (unsigned base = 0; base < base_ଗଣନା; base++)
    {
      ପାଇଁ (unsigned klass = start_class; klass <= end_class; klass++)
      {
        unsigned i = base * class_ଗଣନା + klass;
        add_offset (base_anchors[i], c);
      }
    }
    unsigned base_array = c->କାଢ଼_pack (ମିଥ୍ୟା);

    // markArray
    unsigned ଅଙ୍କ_marks = class_per_table * mark_per_class;
    uint8_t mark_ଗଣନା_buffer[] = {
      (uint8_t) ((ଅଙ୍କ_marks >> 8) & 0xFF),
      (uint8_t) (ଅଙ୍କ_marks & 0xFF),
    };
    start_object ((ଅକ୍ଷର*) mark_ଗଣନା_buffer, 2, c);
    ପାଇଁ (unsigned mark = 0; mark < mark_ଗଣନା; mark++)
    {
      unsigned klass = mark % class_ଗଣନା;
      ଯଦି (klass < start_class || klass > end_class) ଜାରି;
      klass -= start_class;

      extend ((ଅକ୍ଷର*) &class_buffer[2 * klass], 2, c);
      add_offset (mark_anchors[mark], c);
    }
    unsigned mark_array = c->କାଢ଼_pack (ମିଥ୍ୟା);

    // markCoverage
    auto it =
        + hb_range ((hb_codepoint_t) mark_ଗଣନା)
        | hb_filter ([&] (hb_codepoint_t mark) {
          unsigned klass = mark % class_ଗଣନା;
          ଫେରନ୍ତୁ klass >= class_per_table * table_index &&
              klass < class_per_table * (table_index + 1);
        })
        ;
    unsigned mark_coverage = add_coverage (it, c);

    // baseCoverage
    unsigned base_coverage = add_coverage (10, 10 + base_ଗଣନା - 1, c);

    ଫେରନ୍ତୁ add_mark_base_pos_1 (mark_coverage,
                                base_coverage,
                                mark_array,
                                base_array,
                                class_per_table,
                                c);
  }
};

ସ୍ଥିର ଶୂନ୍ୟ run_resolve_overflow_test (ଅଚଳ ଅକ୍ଷର* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       unsigned ଅଙ୍କ_iterations = 0,
                                       ତାର୍କିକ recalculate_extensions = ମିଥ୍ୟା,
                                       hb_tag_t tag = HB_TAG ('G', 'S', 'U', 'B'),
                                       ତାର୍କିକ check_ଦ୍ୱିମିକ_equivalence = ମିଥ୍ୟା)
{
  printf (">>> Testing overflowing resolution for %s\n",
          name);

  graph_t graph = graph_t::create (overflowing.object_graph ()).value ();

  graph_t expected_graph = graph_t::create (expected.object_graph ()).value ();
  ଯଦି (graph::will_overflow (expected_graph).value ())
  {
    ଯଦି (check_ଦ୍ୱିମିକ_equivalence) {
      printf("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_ଦ୍ୱିମିକ_equivalence);
    }
    hb_always_assert (expected_graph.assign_spaces ().is_ok ());
    hb_always_assert (expected_graph.sort_shortest_distance ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.offset_overflow ());
  auto r = hb_resolve_graph_overflows (tag,
                                      ଅଙ୍କ_iterations,
                                      recalculate_extensions,
                                      graph);
  ଯଦି (r.is_err())
    printf("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", graph::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = graph::serialize (graph).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = graph::serialize (expected_graph).value ();
  hb_always_assert (out2);
  ଯଦି (check_ଦ୍ୱିମିକ_equivalence) {
    unsigned l1, l2;
    ଅଚଳ ଅକ୍ଷର* d1 = hb_blob_get_data(out1, &l1);
    ଅଚଳ ଅକ୍ଷର* d2 = hb_blob_get_data(out2, &l2);

    ତାର୍କିକ match = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    ଯଦି (!match) {
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
  ଯଦି (!(graph == expected_graph)) {
    printf("## Expected:\n");
    expected_graph.print();
    printf("## Result:\n");
    graph.print();
  }
  hb_always_assert (graph == expected_graph);
}

ସ୍ଥିର ଶୂନ୍ୟ add_virtual_offset (unsigned id,
                                hb_serialize_context_t* c)
{
  c->add_virtual_link (id);
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_1 = add_object ("ghi", 3, c);
  unsigned obj_2 = add_object ("def", 3, c);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_virtual (hb_serialize_context_t* c, ତାର୍କିକ with_overflow)
{
  std::string large_string(50000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_4, obj_5;
  ଯଦି (with_overflow) {
    obj_5 = add_object("55555", 5, c);
    obj_4 = add_object(large_string.c_str(), 50000, c);
  } ଅନ୍ୟଥା {
    obj_4 = add_object(large_string.c_str(), 50000, c);
    obj_5 = add_object("55555", 5, c);
  }

  start_object(large_string.c_str(), 20000, c);
  add_offset(obj_5, c);
  unsigned obj_3 = c->କାଢ଼_pack(ମିଥ୍ୟା);

  start_object("2", 2, c);
  add_virtual_offset(obj_5, c);
  unsigned obj_2 = c->କାଢ଼_pack(ମିଥ୍ୟା);

  // obj 1
  start_object("1", 1, c);
  add_offset(obj_2, c);
  add_offset(obj_3, c);
  add_offset(obj_4, c);
  c->କାଢ଼_pack(ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_overflow (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_1 = add_object (large_string.c_str(), 10000, c);
  unsigned obj_2 = add_object (large_string.c_str(), 20000, c);
  unsigned obj_3 = add_object (large_string.c_str(), 50000, c);

  start_object ("abc", 3, c);
  add_offset (obj_3, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_priority_overflow (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_e = add_object ("e", 1, c);
  unsigned obj_d = add_object ("d", 1, c);

  start_object (large_string.c_str (), 50000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str (), 20000, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_priority_overflow_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 50000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  unsigned obj_d = add_object ("d", 1, c);

  start_object (large_string.c_str (), 20000, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}


ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_dedup_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_1 = add_object ("def", 3, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_1, c);
  unsigned obj_2 = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_2, c);
  add_offset (obj_1, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_multiple_dedup_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned leaf = add_object("def", 3, c);

  constexpr unsigned ଅଙ୍କ_mid_nodes = 20;
  unsigned mid_nodes[ଅଙ୍କ_mid_nodes];
  ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_mid_nodes; i++) {
    start_object(large_string.c_str(), 10000 + i, c);
    add_offset(leaf, c);
    mid_nodes[i] = c->କାଢ଼_pack(ମିଥ୍ୟା);
  }

  start_object("abc", 3, c);
  ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_mid_nodes; i++) {
    add_wide_offset(mid_nodes[i], c);
  }
  c->କାଢ଼_pack(ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_isolation_overflow (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_4 = add_object ("4", 1, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_4, c);
  unsigned obj_3 = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_4, c);
  unsigned obj_2 = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("1", 1, c);
  add_wide_offset (obj_3, c);
  add_offset (obj_2, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_isolation_overflow_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_f = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("d", 1, c);
  add_offset (obj_e, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d, c);
  unsigned obj_h = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_c, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_d, c);
  unsigned obj_g = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 11000, c);
  add_offset (obj_d, c);
  unsigned obj_i = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_offset (obj_g, c);
  add_offset (obj_i, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_isolation_overflow_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();


  // space 1

  unsigned obj_f_ମୌଳିକ = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f_ମୌଳିକ, c);
  unsigned obj_e_ମୌଳିକ = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("d", 1, c);
  add_offset (obj_e_ମୌଳିକ, c);
  unsigned obj_d_ମୌଳିକ = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d_ମୌଳିକ, c);
  unsigned obj_h = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_e_ମୌଳିକ, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_c, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  // space 0

  unsigned obj_f = add_object ("f", 1, c);

  start_object ("e", 1, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);


  start_object ("d", 1, c);
  add_offset (obj_e, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 11000, c);
  add_offset (obj_d, c);
  unsigned obj_i = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 10000, c);
  add_offset (obj_d, c);
  unsigned obj_g = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_offset (obj_g, c);
  add_offset (obj_i, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_isolation_overflow_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_d = add_object ("f", 1, c);
  unsigned obj_e = add_object ("f", 1, c);

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->କାଢ଼_pack ();

  start_object (large_string.c_str(), 60000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack ();


  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->କାଢ଼_pack ();

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_repack_last (hb_serialize_context_t* c, ତାର୍କିକ with_overflow)
{
  std::string large_string(70000, 'c');
  c->start_serialize<ଅକ୍ଷର> ();
  c->push();

  // Obj E
  unsigned obj_e_1, obj_e_2;
  ଯଦି (with_overflow) {
    obj_e_1 = add_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } ଅନ୍ୟଥା {
    obj_e_2 = add_object("a", 1, c);
  }

  // Obj D
  c->push();
  add_offset(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  unsigned obj_d = c->କାଢ଼_pack(ମିଥ୍ୟା);

  add_offset(obj_d, c);
  hb_always_assert(c->last_added_child_index() == obj_d);

  ଯଦି (!with_overflow) {
    obj_e_1 = add_object("a", 1, c);
  }

  // Obj C
  c->push();
  add_offset(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  unsigned obj_c = c->କାଢ଼_pack(ମିଥ୍ୟା);

  add_offset(obj_c, c);

  // Obj B
  unsigned obj_b = add_object("b", 1, c);
  add_offset(obj_b, c);

  // Obj A
  c->repack_last(obj_d);
  c->କାଢ଼_pack(ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_spaces (hb_serialize_context_t* c, ତାର୍କିକ with_overflow)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_i;

  ଯଦି (with_overflow)
    obj_i = add_object ("i", 1, c);

  // Space 2
  unsigned obj_h = add_object ("h", 1, c);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_h, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  // Space 1
  ଯଦି (!with_overflow)
    obj_i = add_object ("i", 1, c);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_i, c);
  unsigned obj_g = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 30000, c);
  add_offset (obj_i, c);
  unsigned obj_f = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("d", 1, c);
  add_offset (obj_g, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_f, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_g = add_object ("g", 1, c);
  unsigned obj_h = add_object ("h", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_h, c);
  unsigned obj_f = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("d", 1, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_g_ମୌଳିକ = add_object ("g", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g_ମୌଳିକ, c);
  unsigned obj_e_ମୌଳିକ = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_e_ମୌଳିକ, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  unsigned obj_h_ମୌଳିକ = add_object ("h", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_h_ମୌଳିକ, c);
  unsigned obj_f = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("d", 1, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  unsigned obj_g = add_object ("g", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  unsigned obj_h = add_object ("h", 1, c);

  start_object ("b", 1, c);
  add_offset (obj_e, c);
  add_offset (obj_h, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize ();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_short_and_wide_subgraph_ମୂଳ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_c, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("b", 1, c);
  add_offset (obj_c, c);
  add_offset (obj_e, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  add_wide_offset (obj_d, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_short_and_wide_subgraph_ମୂଳ_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_e_ମୌଳିକ = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e_ମୌଳିକ, c);
  unsigned obj_c_ମୌଳିକ = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_c_ମୌଳିକ, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  unsigned obj_e = add_object ("e", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);


  start_object ("b", 1, c);
  add_offset (obj_c, c);
  add_offset (obj_e, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_c_ମୌଳିକ, c);
  add_wide_offset (obj_d, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_split_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_split_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_split_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_f_ମୌଳିକ = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_ମୌଳିକ, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_wide_offset (obj_b, c);
  add_wide_offset (obj_c, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_split_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  // Space 2

  unsigned obj_f_double_ମୌଳିକ = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_double_ମୌଳିକ, c);
  unsigned obj_d_ମୌଳିକ = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("b", 1, c);
  add_offset (obj_d_ମୌଳିକ, c);
  unsigned obj_b_ମୌଳିକ = c->କାଢ଼_pack (ମିଥ୍ୟା);

  // Space 1

  unsigned obj_f_ମୌଳିକ = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f_ମୌଳିକ, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  // Space 0

  unsigned obj_f = add_object ("f", 1, c);

  start_object (large_string.c_str(), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  //ମୂଳ
  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_wide_offset (obj_b_ମୌଳିକ, c);
  add_wide_offset (obj_c, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_5 = add_object ("mn", 2, c);

  unsigned obj_4 = add_object ("jkl", 3, c);

  start_object ("ghi", 3, c);
  add_offset (obj_4, c);
  unsigned obj_3 = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("def", 3, c);
  add_offset (obj_3, c);
  unsigned obj_2 = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_4, c);
  add_offset (obj_5, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_6 = add_object ("opqrst", 6, c);

  unsigned obj_5 = add_object ("mn", 2, c);

  start_object ("jkl", 3, c);
  add_offset (obj_6, c);
  unsigned obj_4 = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("ghi", 3, c);
  add_offset (obj_4, c);
  unsigned obj_3 = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("def", 3, c);
  add_offset (obj_3, c);
  unsigned obj_2 = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("abc", 3, c);
  add_offset (obj_2, c);
  add_offset (obj_4, c);
  add_offset (obj_5, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_d = add_object ("d", 1, c);

  start_object ("b", 1, c);
  add_offset (obj_d, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("e", 1, c);
  add_virtual_offset (obj_b, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("c", 1, c);
  add_offset (obj_e, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_offset (obj_b, c);
  add_offset (obj_c, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_24_and_32_bit_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned obj_f = add_object ("f", 1, c);
  unsigned obj_g = add_object ("g", 1, c);
  unsigned obj_j = add_object ("j", 1, c);
  unsigned obj_k = add_object ("k", 1, c);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_f, c);
  unsigned obj_c = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_g, c);
  unsigned obj_d = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_j, c);
  unsigned obj_h = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object (large_string.c_str (), 40000, c);
  add_offset (obj_k, c);
  unsigned obj_i = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("e", 1, c);
  add_wide_offset (obj_h, c);
  add_wide_offset (obj_i, c);
  unsigned obj_e = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("b", 1, c);
  add_24_offset (obj_c, c);
  add_24_offset (obj_d, c);
  add_24_offset (obj_e, c);
  unsigned obj_b = c->କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("a", 1, c);
  add_24_offset (obj_b, c);
  c->କାଢ଼_pack (ମିଥ୍ୟା);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_extensions = 0,
                                              ତାର୍କିକ shared_subtables = ମିଥ୍ୟା,
                                              ତାର୍କିକ version_1_2 = ମିଥ୍ୟା)
{
  constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_lookups = 5;
  constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_subtables = ଅଙ୍କ_lookups * 2;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookups[ଅଙ୍କ_lookups];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtables[ଅଙ୍କ_subtables];
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା extensions[ଅଙ୍କ_subtables];

  std::string large_string(60000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();


  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଅଙ୍କ_subtables - 1; i >= 0; i--)
    subtables[i] = add_object(large_string.c_str (), 15000 + i, c);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଅଙ୍କ_subtables - 1;
       i >= (ଅଙ୍କ_lookups - ଅଙ୍କ_extensions) * 2;
       i--)
  {
    extensions[i] = add_extension (subtables[i], 5, c);
  }

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଅଙ୍କ_lookups - 1; i >= 0; i--)
  {
    ତାର୍କିକ is_ext = (i >= (ଅଙ୍କ_lookups - ଅଙ୍କ_extensions));

    start_lookup (is_ext ? (ଅକ୍ଷର) 7 : (ଅକ୍ଷର) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    ଯଦି (is_ext) {
      ଯଦି (shared_subtables && i > 2) {
        add_offset (extensions[i * 2 - 1], c);
      }
      add_offset (extensions[i * 2], c);
      add_offset (extensions[i * 2 + 1], c);
    } ଅନ୍ୟଥା {
      ଯଦି (shared_subtables && i > 2) {
        add_offset (subtables[i * 2 - 1], c);
      }
      add_offset (subtables[i * 2], c);
      add_offset (subtables[i * 2 + 1], c);
    }

    lookups[i] = finish_lookup (c);
  }

  unsigned lookup_list = version_1_2
                         ? add_lookup_list_1_2 (lookups, ଅଙ୍କ_lookups, c)
                         : add_lookup_list (lookups, ଅଙ୍କ_lookups, c);

  ଯଦି (version_1_2)
    add_gsubgpos_header_1_2 (lookup_list, c);
  ଅନ୍ୟଥା
    add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_pair_pos_1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_pair_set>
ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           ତାର୍କିକ as_extension = ମିଥ୍ୟା)
{
  std::string large_string(60000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_pair_set = ଅଙ୍କ_pair_pos_1 * ଅଙ୍କ_pair_set;
  unsigned pair_set[total_pair_set];
  unsigned coverage[ଅଙ୍କ_pair_pos_1];
  unsigned pair_pos_1[ଅଙ୍କ_pair_pos_1];

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଅଙ୍କ_pair_pos_1 - 1; i >= 0; i--)
  {
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = (i + 1) * ଅଙ୍କ_pair_set - 1; j >= i * ଅଙ୍କ_pair_set; j--)
      pair_set[j] = add_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = add_coverage (i * ଅଙ୍କ_pair_set,
                                (i + 1) * ଅଙ୍କ_pair_set - 1, c);

    pair_pos_1[i] = add_pair_pos_1 (&pair_set[i * ଅଙ୍କ_pair_set],
                                    ଅଙ୍କ_pair_set,
                                    coverage[i],
                                    c);
  }

  unsigned pair_pos_2 = add_object (large_string.c_str(), 200, c);

  ଯଦି (as_extension) {
    pair_pos_2 = add_extension (pair_pos_2, 2, c);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଅଙ୍କ_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = add_extension (pair_pos_1[i], 2, c);
  }

  start_lookup (as_extension ? 9 : 2, 1 + ଅଙ୍କ_pair_pos_1, c);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_pair_pos_1; i++)
    add_offset (pair_pos_1[i], c);
  add_offset (pair_pos_2, c);

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_pair_pos_2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_class_1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_class_2>
ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           ତାର୍କିକ as_extension = ମିଥ୍ୟା,
                                           ତାର୍କିକ with_device_tables = ମିଥ୍ୟା,
                                           ତାର୍କିକ extra_table = ସତ୍ୟ)
{
  std::string large_string(100000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned coverage[ଅଙ୍କ_pair_pos_2];
  unsigned class_def_1[ଅଙ୍କ_pair_pos_2];
  unsigned class_def_2[ଅଙ୍କ_pair_pos_2];
  unsigned pair_pos_2[ଅଙ୍କ_pair_pos_2];

  unsigned* device_tables = (unsigned*) calloc (ଅଙ୍କ_pair_pos_2 * ଅଙ୍କ_class_1 * ଅଙ୍କ_class_2,
                                                ଆକାର(unsigned));

  // Total glyphs = num_class_1 * num_pair_pos_2
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଅଙ୍କ_pair_pos_2 - 1; i >= 0; i--)
  {
    unsigned start_glyph = 5 + i * ଅଙ୍କ_class_1;
    ଯଦି (ଅଙ୍କ_class_2 >= ଅଙ୍କ_class_1)
    {
      class_def_2[i] = add_class_def (11,
                                      10 + ଅଙ୍କ_class_2, c);
      class_def_1[i] = add_class_def (start_glyph + 1,
                                      start_glyph + ଅଙ୍କ_class_1,
                                      c);
    } ଅନ୍ୟଥା {
      class_def_1[i] = add_class_def (start_glyph + 1,
                                      start_glyph + ଅଙ୍କ_class_1,
                                      c);
      class_def_2[i] = add_class_def (11,
                                      10 + ଅଙ୍କ_class_2, c);
    }

    coverage[i] = add_coverage (start_glyph,
                                start_glyph + ଅଙ୍କ_class_1 - 1,
                                c);

    ଯଦି (with_device_tables)
    {
      ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = (i + 1) * ଅଙ୍କ_class_1 * ଅଙ୍କ_class_2 - 1;
          j >= i * ଅଙ୍କ_class_1 * ଅଙ୍କ_class_2;
          j--)
      {
        uint8_t table[] = {
          (uint8_t) ((j >> 8) & 0xFF),
          (uint8_t) (j & 0xFF),
        };
        device_tables[j] = add_object ((ଅକ୍ଷର*) table, 2, c);
      }
    }

    pair_pos_2[i] = add_pair_pos_2 (1 + i * ଅଙ୍କ_class_1,
                                    coverage[i],
                                    class_def_1[i], ଅଙ୍କ_class_1,
                                    class_def_2[i], ଅଙ୍କ_class_2,
                                    with_device_tables
                                    ? &device_tables[i * ଅଙ୍କ_class_1 * ଅଙ୍କ_class_2]
                                    : nullptr,
                                    c);
  }


  unsigned pair_pos_1 = 0;
  ଯଦି (extra_table) pair_pos_1 = add_object (large_string.c_str(), 100000, c);

  ଯଦି (as_extension) {
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଅଙ୍କ_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = add_extension (pair_pos_2[i], 2, c);

    ଯଦି (extra_table)
      pair_pos_1 = add_extension (pair_pos_1, 2, c);
  }

  start_lookup (as_extension ? 9 : 2, 1 + ଅଙ୍କ_pair_pos_2, c);

  ଯଦି (extra_table)
    add_offset (pair_pos_1, c);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_pair_pos_2; i++)
    add_offset (pair_pos_2[i], c);

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();

  ମୁକ୍ତ (device_tables);
}

template<ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_ଗଣନା,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_ଗଣନା,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା base_ଗଣନା,
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_ଗଣନା_2>
ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_large_mark_base_pos_1 (hb_serialize_context_t* c)
{
  c->start_serialize<ଅକ୍ଷର> ();

  MarkBasePosBuffers<mark_ଗଣନା, class_ଗଣନା, base_ଗଣନା, table_ଗଣନା_2> buffers (c);

  unsigned mark_base_pos[table_ଗଣନା_2];
  ପାଇଁ (unsigned i = 0; i < table_ଗଣନା_2; i++)
    mark_base_pos[i] = buffers.create_mark_base_pos_1 (i, c);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < table_ଗଣନା_2; i++)
    mark_base_pos[i] = add_extension (mark_base_pos[i], 4, c);

  start_lookup (9, table_ଗଣନା_2, c);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < table_ଗଣନା_2; i++)
    add_offset (mark_base_pos[i], c);

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<unsigned Lig_Subst_ଗଣନା,
         unsigned Liga_Set_ଗଣନା,
         unsigned Liga_Per_Set_ଗଣନା,
         unsigned LigaSize>
ସ୍ଥିର ଶୂନ୍ୟ populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    ତାର୍କିକ sequential_liga_sets,
                                                    ତାର୍କିକ shared,
                                                    unsigned *liga_subst_id,
                                                    ତାର୍କିକ unique_lig_str = ମିଥ୍ୟା)
{
  ଅକ୍ଷର ch = 'a';

  unsigned liga[Liga_Set_ଗଣନା * Liga_Per_Set_ଗଣନା];
  unsigned liga_set[Liga_Set_ଗଣନା];
  ପାଇଁ (unsigned l = 0; l < Lig_Subst_ଗଣନା; l++) {
    unsigned coverage_start = 0;
    unsigned coverage_end = Liga_Set_ଗଣନା - 1;
    ଯଦି (sequential_liga_sets) {
      coverage_start = l * Liga_Set_ଗଣନା;
      coverage_end = (l + 1) * Liga_Set_ଗଣନା - 1;
    }
    unsigned coverage = add_coverage(coverage_start, coverage_end, c, shared);

    ପାଇଁ (unsigned i = 0; i < Liga_Set_ଗଣନା; i++) {
      ପାଇଁ (unsigned j = 0; j < Liga_Per_Set_ଗଣନା; j++)
      {
        std::string large_string(100000, ch);
        start_object (large_string.c_str(), LigaSize, c);
        ଯଦି (unique_lig_str)
          ch += 1;
        add_virtual_offset(coverage, c);
        liga[i * Liga_Per_Set_ଗଣନା + j] = c->କାଢ଼_pack (shared);
      }

      add_liga_set_header(Liga_Per_Set_ଗଣନା, c);
      add_virtual_offset(coverage, c);
      ପାଇଁ (unsigned j = 0; j < Liga_Per_Set_ଗଣନା; j++)
      {
        add_offset(liga[i * Liga_Per_Set_ଗଣନା + j], c);
      }

      liga_set[i] = c->କାଢ଼_pack(shared);
    }

    add_liga_header(coverage, Liga_Set_ଗଣନା, c);
    ପାଇଁ (unsigned i = 0; i < Liga_Set_ଗଣନା; i++)
    {
      add_offset(liga_set[i], c);
    }

    liga_subst_id[l] = c->କାଢ଼_pack(shared);
  }

  ପାଇଁ (unsigned l = 0; l < Lig_Subst_ଗଣନା; l++) {
    liga_subst_id[l] = add_extension(liga_subst_id[l], 4, c);
  }
}

template<unsigned liga_subst_ଗଣନା,
         unsigned liga_set_ଗଣନା,
         unsigned liga_per_set_ଗଣନା,
         unsigned liga_size>
ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_large_liga (hb_serialize_context_t* c, ତାର୍କିକ sequential_liga_sets)
{
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned liga_subst[liga_subst_ଗଣନା];
  populate_serializer_with_large_ligsubst<liga_subst_ଗଣନା, liga_set_ଗଣନା, liga_per_set_ଗଣନା, liga_size>(c, sequential_liga_sets, ମିଥ୍ୟା, liga_subst);

  start_lookup (7, liga_subst_ଗଣନା, c);
  ପାଇଁ (unsigned l = 0; l < liga_subst_ଗଣନା; l++) {
    add_offset(liga_subst[l], c);
  }

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_large_liga_overlapping_clone_result (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->start_serialize<ଅକ୍ଷର> ();

  constexpr unsigned liga_size = 30000;

  unsigned liga[6];
  unsigned liga_subst[3];
  unsigned liga_set[2];

  // LigaSubst 3
  unsigned coverage = add_coverage(1, 1, c);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->କାଢ଼_pack (ମିଥ୍ୟା);
  }

  add_liga_set_header(2, c);
  add_virtual_offset(coverage, c);
  ପାଇଁ (unsigned i = 0; i < 2; i++)
    add_offset(liga[i], c);
  liga_set[0] = c->କାଢ଼_pack(ମିଥ୍ୟା);

  add_liga_header(coverage, 1, c);
  add_offset(liga_set[0], c);
  liga_subst[2] = c->କାଢ଼_pack(ମିଥ୍ୟା);

  // LigaSubst 2
  coverage = add_coverage(0, 1, c);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->କାଢ଼_pack (ମିଥ୍ୟା);
  }

  add_liga_set_header(1, c);
  add_virtual_offset(coverage, c);
  add_offset(liga[1], c);
  liga_set[1] = c->କାଢ଼_pack(ମିଥ୍ୟା);

  add_liga_set_header(1, c);
  add_virtual_offset(coverage, c);
  add_offset(liga[0], c);
  liga_set[0] = c->କାଢ଼_pack(ମିଥ୍ୟା);

  add_liga_header(coverage, 2, c);
  add_offset(liga_set[0], c);
  add_offset(liga_set[1], c);
  liga_subst[1] = c->କାଢ଼_pack(ମିଥ୍ୟା);

  // LigaSubst 1
  coverage = add_coverage(0, 0, c);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i >= 0; i--) {
    start_object (large_string.c_str(), liga_size, c);
    add_virtual_offset(coverage, c);
    liga[i] = c->କାଢ଼_pack (ମିଥ୍ୟା);
  }

  add_liga_set_header(2, c);
  add_virtual_offset(coverage, c);
  ପାଇଁ (unsigned i = 0; i < 2; i++)
    add_offset(liga[i], c);
  liga_set[0] = c->କାଢ଼_pack(ମିଥ୍ୟା);

  add_liga_header(coverage, 1, c);
  add_offset(liga_set[0], c);
  liga_subst[0] = c->କାଢ଼_pack(ମିଥ୍ୟା);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = 2; l >= 0; l--) {
    liga_subst[l] = add_extension(liga_subst[l], 4, c);
  }

  start_lookup (7, 3, c);
  ପାଇଁ (unsigned l = 0; l < 3; l++) {
    add_offset(liga_subst[l], c);
  }

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);

  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<unsigned liga_subst_ଗଣନା,
         unsigned liga_set_ଗଣନା,
         unsigned liga_per_set_ଗଣନା,
         unsigned liga_size>
ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, ତାର୍କିକ sequential_liga_sets)
{
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  unsigned liga_subst_idx[liga_subst_ଗଣନା];
  populate_serializer_with_large_ligsubst<liga_subst_ଗଣନା, liga_set_ଗଣନା, liga_per_set_ଗଣନା, liga_size>(c, sequential_liga_sets, ସତ୍ୟ, liga_subst_idx, ସତ୍ୟ);

  start_lookup (7, liga_subst_ଗଣନା, c);
  ପାଇଁ (unsigned l = 0; l < liga_subst_ଗଣନା; l++) {
    add_offset(liga_subst_idx[l], c);
  }

  lookups[0] = finish_lookup (c);

  // Lookup with 2 LigSubst tables
  unsigned liga_subst[liga_subst_ଗଣନା + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, ମିଥ୍ୟା, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_ଗଣନା, liga_set_ଗଣନା, liga_per_set_ଗଣନା, liga_size>(c, sequential_liga_sets, ସତ୍ୟ, &liga_subst[1], ସତ୍ୟ);

  start_lookup (7, liga_subst_ଗଣନା + 1, c);
  ପାଇଁ (unsigned l = 0; l <= liga_subst_ଗଣନା; l++) {
    add_offset(liga_subst[l], c);
  }

  lookups[1] = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (lookups, 2, c);
  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

template<unsigned liga_subst_ଗଣନା,
         unsigned liga_set_ଗଣନା,
         unsigned liga_per_set_ଗଣନା,
         unsigned liga_size>
ସ୍ଥିର ଶୂନ୍ୟ
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->start_serialize<ଅକ୍ଷର> ();

  unsigned liga_subst[liga_subst_ଗଣନା + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, ସତ୍ୟ, ସତ୍ୟ, liga_subst, ସତ୍ୟ);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_ଗଣନା, liga_set_ଗଣନା, liga_per_set_ଗଣନା, liga_size>(c, ସତ୍ୟ, ସତ୍ୟ, &liga_subst[1], ସତ୍ୟ);

  start_lookup (7, liga_subst_ଗଣନା + 1, c);
  ପାଇଁ (unsigned l = 0; l <= liga_subst_ଗଣନା; l++) {
    add_offset(liga_subst[l], c);
  }

  unsigned lookup = finish_lookup (c);

  unsigned lookup_list = add_lookup_list (&lookup, 1, c);
  add_gsubgpos_header (lookup_list, c);

  c->end_serialize();
}

ସ୍ଥିର ଶୂନ୍ୟ test_sort_shortest ()
{
  size_t buffer_size = 100;
  ଶୂନ୍ୟ* buffer_a = malloc (buffer_size);
  ଶୂନ୍ୟ* buffer_e = malloc (buffer_size);
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
  unsigned ghi = e.କାଢ଼_pack(ମିଥ୍ୟା);

  start_object("def", 3, &e);
  add_offset(ghi, &e);
  unsigned def = e.କାଢ଼_pack(ମିଥ୍ୟା);

  unsigned mn = add_object("mn", 2, &e);

  start_object ("abc", 3, &e);
  add_offset (def, &e);
  add_offset (jkl, &e);
  add_offset (mn, &e);
  e.କାଢ଼_pack (ମିଥ୍ୟା);
  e.end_serialize();

  graph_t expected = graph_t::create (e.object_graph ()).value ();
  expected.normalize();

  ପ୍ରତିଜ୍ଞା(expected == graph);

  ମୁକ୍ତ (buffer_a);
  ମୁକ୍ତ (buffer_e);
}

ସ୍ଥିର ଶୂନ୍ୟ test_duplicate_leaf ()
{
  size_t buffer_size = 100;
  ଶୂନ୍ୟ* buffer_a = malloc (buffer_size);
  ଶୂନ୍ୟ* buffer_e = malloc (buffer_size);
  hb_serialize_context_t a (buffer_a, buffer_size);
  hb_serialize_context_t e (buffer_e, buffer_size);
  populate_serializer_complex_2 (&a);

  graph_t graph = graph_t::create (a.object_graph ()).value ();
  hb_always_assert (graph.duplicate (4, 1, ମିଥ୍ୟା).is_ok ());
  graph.normalize();

  e.start_serialize();
  unsigned mn = add_object("mn", 2, &e);
  unsigned jkl_2 = add_object("jkl", 3, &e);

  start_object("ghi", 3, &e);
  add_offset(jkl_2, &e);
  unsigned ghi = e.କାଢ଼_pack(ମିଥ୍ୟା);

  start_object("def", 3, &e);
  add_offset(ghi, &e);
  unsigned def = e.କାଢ଼_pack(ମିଥ୍ୟା);

  unsigned jkl_1 = add_object("jkl", 3, &e);

  start_object("abc", 3, &e);
  add_offset(def, &e);
  add_offset(jkl_1, &e);
  add_offset(mn, &e);
  e.କାଢ଼_pack(ମିଥ୍ୟା);

  graph_t expected = graph_t::create (e.object_graph ()).value ();
  expected.normalize();

  ପ୍ରତିଜ୍ଞା(expected == graph);

  ମୁକ୍ତ (buffer_a);
  ମୁକ୍ତ (buffer_e);
}

ସ୍ଥିର ଶୂନ୍ୟ test_duplicate_interior ()
{
  size_t buffer_size = 100;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_3 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();
  hb_always_assert (graph.duplicate (3, 2, ମିଥ୍ୟା).is_ok ());

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

  ମୁକ୍ତ (buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ
test_serialize ()
{
  size_t buffer_size = 100;
  ଶୂନ୍ୟ* buffer_1 = malloc (buffer_size);
  hb_serialize_context_t c1 (buffer_1, buffer_size);
  populate_serializer_simple (&c1);
  hb_bytes_t expected = c1.copy_bytes ();

  graph_t graph = graph_t::create (c1.object_graph ()).value ();
  hb_blob_t* out = graph::serialize (graph).value ();
  ମୁକ୍ତ (buffer_1);

  hb_bytes_t actual = out->as_bytes ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (out);
}

ସ୍ଥିର ଶୂନ୍ୟ test_will_overflow_1 ()
{
  size_t buffer_size = 100;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  populate_serializer_complex_2 (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (!graph::will_overflow (graph, nullptr).value ());

  ମୁକ୍ତ (buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_will_overflow_2 ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (graph::will_overflow (graph, nullptr).value ());

  ମୁକ୍ତ (buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_will_overflow_3 ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (graph::will_overflow (graph, nullptr).value ());

  ମୁକ୍ତ (buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_sort ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == (80000 + 3 + 3 * 2));

  ମୁକ୍ତ (buffer);
  hb_blob_destroy (out);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_duplication ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  ମୁକ୍ତ (buffer);
  hb_blob_destroy (out);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_multiple_duplications ()
{
  size_t buffer_size = 300000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_multiple_dedup_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG_NONE, 5);
  hb_always_assert (out);

  ମୁକ୍ତ (buffer);
  hb_blob_destroy (out);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_space_assignment ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_spaces (&c, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_spaces (&e, ମିଥ୍ୟା);

  run_resolve_overflow_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_isolation ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_isolation_overflow (&c);
  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_always_assert (c.offset_overflow ());
  hb_blob_t* out = hb_resolve_overflows (c.object_graph (), HB_TAG ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (out);
  hb_bytes_t result = out->as_bytes ();
  hb_always_assert (result.length == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  ମୁକ୍ତ (buffer);
  hb_blob_destroy (out);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_isolation_with_recursive_duplication ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_isolation_overflow_complex (&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_isolation_overflow_complex_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_isolating_16bit_space ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_spaces_16bit_connection (&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_spaces_16bit_connection_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_isolating_16bit_space_2 ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_short_and_wide_subgraph_ମୂଳ (&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_short_and_wide_subgraph_ମୂଳ_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_isolation_spaces ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
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

  ମୁକ୍ତ (buffer);
  hb_blob_destroy (out);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_mixed_overflows_via_isolation_spaces ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
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

  ମୁକ୍ତ (buffer);
  hb_blob_destroy (out);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_extension_promotion ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             ସତ୍ୟ);
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_shared_extension_promotion ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c, 0, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3, ସତ୍ୟ);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             ସତ୍ୟ);
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

#ifndef HB_NO_BEYOND_64K
ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_extension_promotion_1_2 ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_extension_promotion (&c, 0, ମିଥ୍ୟା, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_extension_promotion (&e, 3, ମିଥ୍ୟା, ସତ୍ୟ);

  run_resolve_overflow_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             ସତ୍ୟ);
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}
#endif

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_repeated_extension_promotion ()
{
  size_t buffer_size = 1000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize<ଅକ୍ଷର> ();
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
  unsigned lookup_index = *context.lookups.ଚାବିଗୁଡ଼ିକ ();
  graph::Lookup* lookup_table = context.lookups.get (lookup_index);
  hb_always_assert (lookup_table->make_extension (context, lookup_index).is_ok ());

  ଅଚଳ auto& lookup_vertex = graph.vertices_[lookup_index];
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
  ମୁକ୍ତ (buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_basic_pair_pos_1_split ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, ସତ୍ୟ);

  run_resolve_overflow_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'P', 'O', 'S'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_extension_pair_pos_1_split ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, ସତ୍ୟ);

  run_resolve_overflow_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'P', 'O', 'S'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_basic_pair_pos_2_split ()
{
  size_t buffer_size = 300000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, ସତ୍ୟ);

  run_resolve_overflow_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'P', 'O', 'S'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_close_to_limit_pair_pos_2_split ()
{
  size_t buffer_size = 300000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, ସତ୍ୟ, ମିଥ୍ୟା, ମିଥ୍ୟା);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, ସତ୍ୟ, ମିଥ୍ୟା, ମିଥ୍ୟା);

  run_resolve_overflow_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'P', 'O', 'S'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_pair_pos_2_split_with_device_tables ()
{
  size_t buffer_size = 300000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, ମିଥ୍ୟା, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, ସତ୍ୟ, ସତ୍ୟ);

  run_resolve_overflow_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'P', 'O', 'S'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_basic_mark_base_pos_1_split ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_mark_base_pos_1 <40, 10, 110, 1>(&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_mark_base_pos_1 <40, 10, 110, 2>(&e);

  run_resolve_overflow_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'P', 'O', 'S'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_basic_liga_split ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, ମିଥ୍ୟା);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, ମିଥ୍ୟା);

  run_resolve_overflow_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'S', 'U', 'B'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_liga_split_move ()
{
  size_t buffer_size = 400000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, ସତ୍ୟ);

  run_resolve_overflow_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'S', 'U', 'B'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_liga_split_overlapping_clone ()
{
  size_t buffer_size = 400000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_large_liga_overlapping_clone_result(&e);

  run_resolve_overflow_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'S', 'U', 'B'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_liga_split_shared_table ()
{
  size_t buffer_size = 400000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, ସତ୍ୟ);

  run_resolve_overflow_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'S', 'U', 'B'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_with_liga_split_shared_coverage ()
{
  size_t buffer_size = 400000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  run_resolve_overflow_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             ସତ୍ୟ,
                             HB_TAG('G', 'S', 'U', 'B'));
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_splitting_spaces ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_split_spaces (&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_split_spaces_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);

}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_splitting_spaces_2 ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_split_spaces_2 (&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_split_spaces_expected_2 (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_resolve_overflows_via_priority ()
{
  size_t buffer_size = 160000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_priority_overflow (&c);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_priority_overflow_expected (&e);

  run_resolve_overflow_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}


ସ୍ଥିର ଶୂନ୍ୟ test_virtual_link ()
{
  size_t buffer_size = 100;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
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

  ମୁକ୍ତ (buffer);
  hb_blob_destroy (out);
}

ସ୍ଥିର ଶୂନ୍ୟ
test_shared_ଗଣ୍ଠି_with_virtual_links ()
{
  size_t buffer_size = 100;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize<ଅକ୍ଷର> ();

  unsigned obj_b = add_object ("b", 1, &c);
  unsigned obj_c = add_object ("c", 1, &c);

  start_object ("d", 1, &c);
  add_virtual_offset (obj_b, &c);
  unsigned obj_d_1 = c.କାଢ଼_pack ();

  start_object ("d", 1, &c);
  add_virtual_offset (obj_c, &c);
  unsigned obj_d_2 = c.କାଢ଼_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  start_object ("a", 1, &c);
  add_offset (obj_b, &c);
  add_offset (obj_c, &c);
  add_offset (obj_d_1, &c);
  add_offset (obj_d_2, &c);
  c.କାଢ଼_pack ();
  c.end_serialize ();

  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links.length == 2);
  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_graph() [obj_d_1]->virtual_links[1].objidx == obj_c);
  ମୁକ୍ତ(buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ
test_repack_last ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_with_repack_last (&c, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_with_repack_last (&e, ମିଥ୍ୟା);

  run_resolve_overflow_test ("test_repack_last",
                             c,
                             e,
                             20,
                             ମିଥ୍ୟା,
                             HB_TAG('a', 'b', 'c', 'd'),
                             ସତ୍ୟ);

  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ
test_dont_duplicate_virtual ()
{
  size_t buffer_size = 200000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_always_assert (buffer);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_virtual (&c, ସତ୍ୟ);

  ଶୂନ୍ୟ* expected_buffer = malloc (buffer_size);
  hb_always_assert (expected_buffer);
  hb_serialize_context_t e (expected_buffer, buffer_size);
  populate_serializer_virtual (&e, ମିଥ୍ୟା);

  run_resolve_overflow_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             ମିଥ୍ୟା,
                             HB_TAG('a', 'b', 'c', 'd'),
                             ସତ୍ୟ);

  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (expected_buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ
test_deep_graph_traversal ()
{
  size_t buffer_size = 1000000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize ();
  unsigned prev = add_object ("leaf", 4, &c);
  ପାଇଁ (unsigned i = 0; i < 2000; i++)
  {
    start_object ("node", 4, &c);
    add_offset (prev, &c);
    prev = c.କାଢ଼_pack (ମିଥ୍ୟା);
  }
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // Test find_subgraph (set)
  hb_set_t visited_set;
  hb_always_assert (graph.find_subgraph (graph.ମୂଳ_idx (), visited_set).is_ok ());
  hb_always_assert (visited_set.get_population () == 2001);

  // Test find_subgraph_size
  hb_set_t size_set;
  size_t sz = graph.find_subgraph_size (graph.ମୂଳ_idx (), size_set).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.set (graph.ମୂଳ_idx (), 1);
  hb_always_assert (graph.find_subgraph (graph.ମୂଳ_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (graph.assign_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t index_map;
  hb_always_assert (graph.duplicate_subgraph (graph.ମୂଳ_idx (), index_map).is_ok ());
  hb_always_assert (index_map.get_population () == 2001);

  ମୁକ୍ତ (buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ
test_32bit_roots_traversal ()
{
  size_t buffer_size = 1000;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);

  c.start_serialize ();
  unsigned leaf1 = add_object ("l1", 2, &c);
  unsigned leaf2 = add_object ("l2", 2, &c);

  start_object ("mid", 3, &c);
  add_wide_offset (leaf1, &c);
  unsigned mid = c.କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("root", 4, &c);
  add_offset (mid, &c);
  add_wide_offset (leaf2, &c);
  c.କାଢ଼_pack (ମିଥ୍ୟା);
  c.end_serialize ();

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  hb_set_t roots;
  hb_always_assert (graph.find_32bit_roots (graph.ମୂଳ_idx (), roots).is_ok ());
  hb_always_assert (roots.has (leaf1 - 1));
  hb_always_assert (roots.has (leaf2 - 1));
  hb_always_assert (!roots.has (mid - 1));
  hb_always_assert (roots.get_population () == 2);

  ମୁକ୍ତ (buffer);

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
  unsigned a = c2.କାଢ଼_pack (ମିଥ୍ୟା);

  start_object ("r", 1, &c2);
  add_offset (a, &c2);
  add_wide_offset (a, &c2);
  c2.କାଢ଼_pack (ମିଥ୍ୟା);

  c2.end_serialize ();

  graph_t graph2 = graph_t::create (c2.object_graph ()).value ();

  hb_set_t roots2;
  hb_always_assert (graph2.find_32bit_roots (graph2.ମୂଳ_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  ମୁକ୍ତ (buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_invalid_link_objidx ()
{
  size_t buffer_size = 100;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_2 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // Poison a link to point to (unsigned) -1
  auto links = graph.vertices_[graph.ମୂଳ_idx ()].real_links_writer();
  links->objidx = (unsigned) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!graph.sort_shortest_distance ().is_ok ());

  ମୁକ୍ତ (buffer);
}

ସ୍ଥିର ଶୂନ୍ୟ test_invalid_link_add_and_move ()
{
  size_t buffer_size = 100;
  ଶୂନ୍ୟ* buffer = malloc (buffer_size);
  hb_serialize_context_t c (buffer, buffer_size);
  populate_serializer_complex_2 (&c);

  graph_t graph = graph_t::create (c.object_graph ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!graph.add_link (&dummy, 0, (unsigned) -1).is_ok ());

  // move_child with non-existent offset
  auto res = graph.move_child (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  ମୁକ୍ତ (buffer);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
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
  test_shared_ଗଣ୍ଠି_with_virtual_links ();
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
