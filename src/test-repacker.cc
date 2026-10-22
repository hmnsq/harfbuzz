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

#ضمن <cstdint>
#ضمن <string>

#ضمن "hb-repacker.hh"
#ضمن "hb-open-type.hh"
#ضمن "graph/serialize.hh"

ساكن فراغ extend (ثابت حرف* value,
                    بنانشانی len,
                    hb_serialize_context_t* c)
{
  حرف* obj = c->allocate_ناپ<حرف> (len);
  hb_memcpy (obj, value, len);
}

ساكن فراغ شروع_object(ثابت حرف* ٹیگ_2,
                         بنانشانی len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (ٹیگ_2, len, c);
}

ساكن بنانشانی جوڑو_object(ثابت حرف* ٹیگ_2,
                           بنانشانی len,
                           hb_serialize_context_t* c,
                           منطقي shared = خطأ)
{
  شروع_object (ٹیگ_2, len, c);
  أرجع c->نکالو_pack (shared);
}


ساكن فراغ جوڑو_وتھ (بنانشانی id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* وتھ = c->شروع_embed<OT::Offset16> ();
  c->extend_min (وتھ);
  c->جوڑو_link (*وتھ, id);
}

ساكن فراغ جوڑو_24_وتھ (بنانشانی id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* وتھ = c->شروع_embed<OT::Offset24> ();
  c->extend_min (وتھ);
  c->جوڑو_link (*وتھ, id);
}

ساكن فراغ جوڑو_wide_وتھ (بنانشانی id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* وتھ = c->شروع_embed<OT::Offset32> ();
  c->extend_min (وتھ);
  c->جوڑو_link (*وتھ, id);
}

ساكن فراغ جوڑو_gsubgpos_سرخی (بنانشانی lookup_list,
                                 hb_serialize_context_t* c)
{
  حرف سرخی[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  شروع_object (سرخی, 8, c);
  جوڑو_وتھ (lookup_list, c);
  c->نکالو_pack (خطأ);
}

ساكن فراغ جوڑو_gsubgpos_سرخی_1_2 (بنانشانی lookup_list,
                                     hb_serialize_context_t* c)
{
  حرف سرخی[] = {
    0, 1, 0, 2, //نسخہ
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  شروع_object (سرخی, حجم (سرخی), c);
  جوڑو_wide_وتھ (lookup_list, c);
  c->نکالو_pack (خطأ);
}

ساكن بنانشانی جوڑو_lookup_list (ثابت بنانشانی* lookups,
                                 حرف گنتی,
                                 hb_serialize_context_t* c)
{
  حرف lookup_گنتی[] = {0, گنتی};
  شروع_object  ((حرف *) &lookup_گنتی, 2, c);

  لكل (صحيح i = 0; i < گنتی; i++)
    جوڑو_وتھ (lookups[i], c);

  أرجع c->نکالو_pack (خطأ);
}

ساكن بنانشانی جوڑو_lookup_list_1_2 (ثابت بنانشانی* lookups,
                                     حرف گنتی,
                                     hb_serialize_context_t* c)
{
  حرف lookup_گنتی[] = {0, گنتی};
  شروع_object ((حرف *) &lookup_گنتی, 2, c);

  لكل (صحيح i = 0; i < گنتی; i++)
    جوڑو_wide_وتھ (lookups[i], c);

  أرجع c->نکالو_pack (خطأ);
}

ساكن فراغ شروع_lookup (int8_t type,
                          int8_t عدد_subtables,
                          hb_serialize_context_t* c)
{
  حرف lookup[] = {
    0, (حرف)type, // type
    0, (حرف)2, // flag
    0, (حرف)عدد_subtables, // num subtables
  };

  شروع_object (lookup, 6, c);
}

ساكن بنانشانی انت_lookup (hb_serialize_context_t* c)
{
  حرف filter[] = {0, 0};
  extend (filter, 2, c);
  أرجع c->نکالو_pack (خطأ);
}

ساكن بنانشانی جوڑو_extension (بنانشانی بچہ,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  حرف ext[] = {
    0, 1,
    0, (حرف) type,
  };

  شروع_object (ext, 4, c);
  جوڑو_wide_وتھ (بچہ, c);

  أرجع c->نکالو_pack (خطأ);

}

// Adds coverage table fro [start, end]
ساكن بنانشانی جوڑو_coverage (بنانشانی شروع, بنانشانی سرا,
                              hb_serialize_context_t* c,
                              منطقي shared = خطأ)
{
  إذا (سرا - شروع == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //گنتی

      (uint8_t) ((شروع >> 8) & 0xFF),
      (uint8_t) (شروع & 0xFF), // glyph[0]
    };
    أرجع جوڑو_object ((حرف*) coverage, 6, c, shared);
  }

  إذا (سرا - شروع == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //گنتی

      (uint8_t) ((شروع >> 8) & 0xFF),
      (uint8_t) (شروع & 0xFF), // glyph[0]

      (uint8_t) ((سرا >> 8) & 0xFF),
      (uint8_t) (سرا & 0xFF), // glyph[1]
    };
    أرجع جوڑو_object ((حرف*) coverage, 8, c, shared);
  }

  uint8_t coverage[] = {
    0, 2, // format
    0, 1, // range count

    (uint8_t) ((شروع >> 8) & 0xFF),
    (uint8_t) (شروع & 0xFF), //شروع

    (uint8_t) ((سرا >> 8) & 0xFF),
    (uint8_t) (سرا & 0xFF), //سرا

    0, 0,
  };
  أرجع جوڑو_object ((حرف*) coverage, 10, c, shared);
}


سانچا<قسم_ناں It>
ساكن بنانشانی جوڑو_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  أرجع c->نکالو_pack (خطأ);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
ساكن بنانشانی جوڑو_class_def (uint16_t شروع_glyph,
                               uint16_t سرا_glyph,
                               hb_serialize_context_t* c)
{
  بنانشانی گنتی = سرا_glyph - شروع_glyph;
  uint8_t سرخی[] = {
    0, 1, // format

    (uint8_t) ((شروع_glyph >> 8) & 0xFF),
    (uint8_t) (شروع_glyph & 0xFF), // start_glyph

    (uint8_t) ((گنتی >> 8) & 0xFF),
    (uint8_t) (گنتی & 0xFF), //گنتی
  };

  شروع_object ((حرف*) سرخی, 6, c);
  لكل (uint16_t i = 1; i <= گنتی; i++)
  {
    uint8_t class_قدر_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //گنتی
    };
    extend ((حرف*) class_قدر_2, 2, c);
  }

  أرجع c->نکالو_pack (خطأ);
}

ساكن بنانشانی جوڑو_pair_pos_1 (بنانشانی* pair_sets,
                                حرف گنتی,
                                بنانشانی coverage,
                                hb_serialize_context_t* c)
{
  حرف format[] = {
    0, 1
  };

  شروع_object (format, 2, c);
  جوڑو_وتھ (coverage, c);

  حرف قدر_format[] = {
    0, 0,
    0, 0,
    0, گنتی,
  };
  extend (قدر_format, 6, c);

  لكل (حرف i = 0; i < گنتی; i++)
    جوڑو_وتھ (pair_sets[(بنانشانی) i], c);

  أرجع c->نکالو_pack (خطأ);
}

ساكن فراغ جوڑو_liga_سیٹ_سرخی (حرف liga_گنتی,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_گنتی_بائٹاں[] = {
    (uint8_t) (0xFF & (liga_گنتی >> 8)),
    (uint8_t) (0xFF & liga_گنتی),
  };

  شروع_object ((ثابت حرف*) liga_گنتی_بائٹاں, 2, c);
}

ساكن فراغ جوڑو_liga_سرخی (بنانشانی coverage, بنانشانی liga_سیٹ_گنتی, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  شروع_object ((ثابت حرف*) format, 2, c);
  جوڑو_وتھ(coverage, c);

  uint8_t liga_سیٹ_گنتی_بائٹاں[] = {
    (uint8_t) (0xFF & (liga_سیٹ_گنتی >> 8)),
    (uint8_t) (0xFF & liga_سیٹ_گنتی),
  };

  extend((ثابت حرف *) liga_سیٹ_گنتی_بائٹاں, 2, c);
}

ساكن بنانشانی جوڑو_pair_pos_2 (بنانشانی starting_class,
                                بنانشانی coverage,
                                بنانشانی class_def_1, uint16_t class_def_1_گنتی,
                                بنانشانی class_def_2, uint16_t class_def_2_گنتی,
                                بنانشانی* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  شروع_object ((حرف*) format, 2, c);
  جوڑو_وتھ (coverage, c);

  بنانشانی عدد_اقدار_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  إذا (device_tables) {
    format2 |= 0x20;
    عدد_اقدار_3 += 1;
  }
  uint8_t قدر_format[] = {
    0, format1,
    0, format2,
  };

  extend ((حرف*) قدر_format, 4, c);

  جوڑو_وتھ (class_def_1, c);
  جوڑو_وتھ (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_گنتی >> 8) & 0xFF),
    (uint8_t) (class_def_1_گنتی & 0xFF),
    (uint8_t) ((class_def_2_گنتی >> 8) & 0xFF),
    (uint8_t) (class_def_2_گنتی & 0xFF),
  };
  extend ((حرف*) class_counts, 4, c);

  بنانشانی عدد_بائٹاں_فی_ریکارڈ = class_def_2_گنتی * عدد_اقدار_3 * 2;
  uint8_t* ریکارڈ = (uint8_t*) صفر_حافظہ_دیو (1, عدد_بائٹاں_فی_ریکارڈ);
  صحيح device_اشاریہ = 0;
  لكل (uint16_t i = 0; i < class_def_1_گنتی; i++)
  {

    لكل (uint16_t j = 0; j < class_def_2_گنتی; j++)
    {
      لكل (صحيح k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((حرف*) value, 2, c);
      }

      إذا (device_tables) {
        جوڑو_وتھ (device_tables[device_اشاریہ++], c);
      }
    }
  }
  حرر (ریکارڈ);

  أرجع c->نکالو_pack (خطأ);
}

ساكن بنانشانی جوڑو_mark_ادھار_pos_1 (بنانشانی mark_coverage,
                                     بنانشانی ادھار_coverage,
                                     بنانشانی mark_array,
                                     بنانشانی ادھار_array,
                                     بنانشانی class_گنتی,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  شروع_object ((حرف*) format, 2, c);
  جوڑو_وتھ (mark_coverage, c);
  جوڑو_وتھ (ادھار_coverage, c);

  uint8_t گنتی[] = {
    (uint8_t) ((class_گنتی >> 8) & 0xFF),
    (uint8_t) (class_گنتی & 0xFF),
  };
  extend ((حرف*) گنتی, 2, c);

  جوڑو_وتھ (mark_array, c);
  جوڑو_وتھ (ادھار_array, c);

  أرجع c->نکالو_pack (خطأ);
}

سانچا<صحيح mark_گنتی,
    صحيح class_گنتی,
    صحيح ادھار_گنتی,
    صحيح جدول_گنتی_2>
بنية Mark_ادھار_Pos_Buffers
{
  بنانشانی ادھار_anchors[class_گنتی * ادھار_گنتی];
  بنانشانی mark_anchors[mark_گنتی];
  uint8_t anchor_buffers[class_گنتی * ادھار_گنتی + 100];
  uint8_t class_بفر[class_گنتی * 2];

  Mark_ادھار_Pos_Buffers(hb_serialize_context_t* c)
  {
    لكل (بنانشانی i = 0; i < حجم(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    لكل (بنانشانی i = 0; i < class_گنتی * ادھار_گنتی; i++)
    {
      ادھار_anchors[i] = جوڑو_object ((حرف*) &anchor_buffers[i], 100, c);
      إذا (i < class_گنتی) {
        class_بفر[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_بفر[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    لكل (بنانشانی i = 0; i < mark_گنتی; i++)
    {
      mark_anchors[i] = جوڑو_object ((حرف*) &anchor_buffers[i], 4, c);
    }
  }

  بنانشانی create_mark_ادھار_pos_1 (بنانشانی جدول_اشاریہ, hb_serialize_context_t* c)
  {
    بنانشانی class_فی_جدول = class_گنتی / جدول_گنتی_2;
    بنانشانی mark_فی_class = mark_گنتی / class_گنتی;
    بنانشانی شروع_class = class_فی_جدول * جدول_اشاریہ;
    بنانشانی سرا_class = class_فی_جدول * (جدول_اشاریہ + 1) - 1;

    // baseArray
    uint8_t ادھار_گنتی_بفر[] = {
      (uint8_t) ((ادھار_گنتی >> 8) & 0xFF),
      (uint8_t) (ادھار_گنتی & 0xFF),

    };
    شروع_object ((حرف*) ادھار_گنتی_بفر, 2, c);
    لكل (بنانشانی ادھار = 0; ادھار < ادھار_گنتی; ادھار++)
    {
      لكل (بنانشانی klass = شروع_class; klass <= سرا_class; klass++)
      {
        بنانشانی i = ادھار * class_گنتی + klass;
        جوڑو_وتھ (ادھار_anchors[i], c);
      }
    }
    بنانشانی ادھار_array = c->نکالو_pack (خطأ);

    // markArray
    بنانشانی عدد_marks = class_فی_جدول * mark_فی_class;
    uint8_t mark_گنتی_بفر[] = {
      (uint8_t) ((عدد_marks >> 8) & 0xFF),
      (uint8_t) (عدد_marks & 0xFF),
    };
    شروع_object ((حرف*) mark_گنتی_بفر, 2, c);
    لكل (بنانشانی mark = 0; mark < mark_گنتی; mark++)
    {
      بنانشانی klass = mark % class_گنتی;
      إذا (klass < شروع_class || klass > سرا_class) استمر;
      klass -= شروع_class;

      extend ((حرف*) &class_بفر[2 * klass], 2, c);
      جوڑو_وتھ (mark_anchors[mark], c);
    }
    بنانشانی mark_array = c->نکالو_pack (خطأ);

    // markCoverage
    خودکار it =
        + hb_range ((hb_codepoint_t) mark_گنتی)
        | hb_filter ([&] (hb_codepoint_t mark) {
          بنانشانی klass = mark % class_گنتی;
          أرجع klass >= class_فی_جدول * جدول_اشاریہ &&
              klass < class_فی_جدول * (جدول_اشاریہ + 1);
        })
        ;
    بنانشانی mark_coverage = جوڑو_coverage (it, c);

    // baseCoverage
    بنانشانی ادھار_coverage = جوڑو_coverage (10, 10 + ادھار_گنتی - 1, c);

    أرجع جوڑو_mark_ادھار_pos_1 (mark_coverage,
                                ادھار_coverage,
                                mark_array,
                                ادھار_array,
                                class_فی_جدول,
                                c);
  }
};

ساكن فراغ لگاتار_resolve_حد_توں_ودھ_test (ثابت حرف* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       بنانشانی عدد_iterations = 0,
                                       منطقي recalculate_extensions = خطأ,
                                       hb_ٹیگ_t ٹیگ_2 = HB_ٹیگ ('G', 'S', 'U', 'B'),
                                       منطقي check_ثنائی_equivalence = خطأ)
{
  لکھو (">>> Testing overflowing resolution for %s\n",
          name);

  گراف_t گراف = گراف_t::create (overflowing.object_گراف ()).value ();

  گراف_t expected_گراف = گراف_t::create (expected.object_گراف ()).value ();
  إذا (گراف::will_حد_توں_ودھ (expected_گراف).value ())
  {
    إذا (check_ثنائی_equivalence) {
      لکھو("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_ثنائی_equivalence);
    }
    hb_always_assert (expected_گراف.سونپو_spaces ().is_ok ());
    hb_always_assert (expected_گراف.ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.وتھ_حد_توں_ودھ ());
  خودکار r = hb_resolve_گراف_overflows (ٹیگ_2,
                                      عدد_iterations,
                                      recalculate_extensions,
                                      گراف);
  إذا (r.is_err())
    لکھو("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", گراف::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = گراف::serialize (گراف).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = گراف::serialize (expected_گراف).value ();
  hb_always_assert (out2);
  إذا (check_ثنائی_equivalence) {
    بنانشانی l1, l2;
    ثابت حرف* d1 = hb_blob_get_انکڑے(out1, &l1);
    ثابت حرف* d2 = hb_blob_get_انکڑے(out2, &l2);

    منطقي میل = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    إذا (!میل) {
      لکھو("## Result:\n");
      گراف.print();
      لکھو("## Expected:\n");
      expected_گراف.print();
      hb_always_assert(میل);
    }
  }

  hb_blob_destroy (out1);
  hb_blob_destroy (out2);

  // Check the graphs are equivalent
  گراف.normalize ();
  expected_گراف.normalize ();
  إذا (!(گراف == expected_گراف)) {
    لکھو("## Expected:\n");
    expected_گراف.print();
    لکھو("## Result:\n");
    گراف.print();
  }
  hb_always_assert (گراف == expected_گراف);
}

ساكن فراغ جوڑو_virtual_وتھ (بنانشانی id,
                                hb_serialize_context_t* c)
{
  c->جوڑو_virtual_link (id);
}

ساكن فراغ
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بنانشانی obj_1 = جوڑو_object ("ghi", 3, c);
  بنانشانی obj_2 = جوڑو_object ("def", 3, c);

  شروع_object ("abc", 3, c);
  جوڑو_وتھ (obj_2, c);
  جوڑو_وتھ (obj_1, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_virtual (hb_serialize_context_t* c, منطقي with_حد_توں_ودھ)
{
  std::string large_string(50000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_4, obj_5;
  إذا (with_حد_توں_ودھ) {
    obj_5 = جوڑو_object("55555", 5, c);
    obj_4 = جوڑو_object(large_string.c_str(), 50000, c);
  } وإلا {
    obj_4 = جوڑو_object(large_string.c_str(), 50000, c);
    obj_5 = جوڑو_object("55555", 5, c);
  }

  شروع_object(large_string.c_str(), 20000, c);
  جوڑو_وتھ(obj_5, c);
  بنانشانی obj_3 = c->نکالو_pack(خطأ);

  شروع_object("2", 2, c);
  جوڑو_virtual_وتھ(obj_5, c);
  بنانشانی obj_2 = c->نکالو_pack(خطأ);

  // obj 1
  شروع_object("1", 1, c);
  جوڑو_وتھ(obj_2, c);
  جوڑو_وتھ(obj_3, c);
  جوڑو_وتھ(obj_4, c);
  c->نکالو_pack(خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_حد_توں_ودھ (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_1 = جوڑو_object (large_string.c_str(), 10000, c);
  بنانشانی obj_2 = جوڑو_object (large_string.c_str(), 20000, c);
  بنانشانی obj_3 = جوڑو_object (large_string.c_str(), 50000, c);

  شروع_object ("abc", 3, c);
  جوڑو_وتھ (obj_3, c);
  جوڑو_وتھ (obj_2, c);
  جوڑو_وتھ (obj_1, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_ترجیح_حد_توں_ودھ (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_e = جوڑو_object ("e", 1, c);
  بنانشانی obj_d = جوڑو_object ("d", 1, c);

  شروع_object (large_string.c_str (), 50000, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str (), 20000, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_وتھ (obj_b, c);
  جوڑو_وتھ (obj_c, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_ترجیح_حد_توں_ودھ_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_e = جوڑو_object ("e", 1, c);

  شروع_object (large_string.c_str (), 50000, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  بنانشانی obj_d = جوڑو_object ("d", 1, c);

  شروع_object (large_string.c_str (), 20000, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_وتھ (obj_b, c);
  جوڑو_وتھ (obj_c, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}


ساكن فراغ
populate_serializer_with_dedup_حد_توں_ودھ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_1 = جوڑو_object ("def", 3, c);

  شروع_object (large_string.c_str(), 60000, c);
  جوڑو_وتھ (obj_1, c);
  بنانشانی obj_2 = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 10000, c);
  جوڑو_وتھ (obj_2, c);
  جوڑو_وتھ (obj_1, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_مضرب_dedup_حد_توں_ودھ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی پتی = جوڑو_object("def", 3, c);

  constexpr بنانشانی عدد_mid_nodes = 20;
  بنانشانی mid_nodes[عدد_mid_nodes];
  لكل (بنانشانی i = 0; i < عدد_mid_nodes; i++) {
    شروع_object(large_string.c_str(), 10000 + i, c);
    جوڑو_وتھ(پتی, c);
    mid_nodes[i] = c->نکالو_pack(خطأ);
  }

  شروع_object("abc", 3, c);
  لكل (بنانشانی i = 0; i < عدد_mid_nodes; i++) {
    جوڑو_wide_وتھ(mid_nodes[i], c);
  }
  c->نکالو_pack(خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_حد_توں_ودھ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_4 = جوڑو_object ("4", 1, c);

  شروع_object (large_string.c_str(), 60000, c);
  جوڑو_وتھ (obj_4, c);
  بنانشانی obj_3 = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 10000, c);
  جوڑو_وتھ (obj_4, c);
  بنانشانی obj_2 = c->نکالو_pack (خطأ);

  شروع_object ("1", 1, c);
  جوڑو_wide_وتھ (obj_3, c);
  جوڑو_وتھ (obj_2, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_حد_توں_ودھ_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_f = جوڑو_object ("f", 1, c);

  شروع_object ("e", 1, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object ("d", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 60000, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_h = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 60000, c);
  جوڑو_وتھ (obj_c, c);
  جوڑو_وتھ (obj_h, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 10000, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_g = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 11000, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_i = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_wide_وتھ (obj_b, c);
  جوڑو_وتھ (obj_g, c);
  جوڑو_وتھ (obj_i, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_حد_توں_ودھ_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();


  // space 1

  بنانشانی obj_f_مفرد = جوڑو_object ("f", 1, c);

  شروع_object ("e", 1, c);
  جوڑو_وتھ (obj_f_مفرد, c);
  بنانشانی obj_e_مفرد = c->نکالو_pack (خطأ);

  شروع_object ("d", 1, c);
  جوڑو_وتھ (obj_e_مفرد, c);
  بنانشانی obj_d_مفرد = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 60000, c);
  جوڑو_وتھ (obj_d_مفرد, c);
  بنانشانی obj_h = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_e_مفرد, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 60000, c);
  جوڑو_وتھ (obj_c, c);
  جوڑو_وتھ (obj_h, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  // space 0

  بنانشانی obj_f = جوڑو_object ("f", 1, c);

  شروع_object ("e", 1, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);


  شروع_object ("d", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 11000, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_i = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 10000, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_g = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_wide_وتھ (obj_b, c);
  جوڑو_وتھ (obj_g, c);
  جوڑو_وتھ (obj_i, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_حد_توں_ودھ_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_d = جوڑو_object ("f", 1, c);
  بنانشانی obj_e = جوڑو_object ("f", 1, c);

  شروع_object (large_string.c_str(), 60000, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_b = c->نکالو_pack ();

  شروع_object (large_string.c_str(), 60000, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack ();


  شروع_object ("a", 1, c);
  جوڑو_wide_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_c, c);
  c->نکالو_pack ();

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_repack_آخری (hb_serialize_context_t* c, منطقي with_حد_توں_ودھ)
{
  std::string large_string(70000, 'c');
  c->شروع_serialize<حرف> ();
  c->push();

  // Obj E
  بنانشانی obj_e_1, obj_e_2;
  إذا (with_حد_توں_ودھ) {
    obj_e_1 = جوڑو_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } وإلا {
    obj_e_2 = جوڑو_object("a", 1, c);
  }

  // Obj D
  c->push();
  جوڑو_وتھ(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  بنانشانی obj_d = c->نکالو_pack(خطأ);

  جوڑو_وتھ(obj_d, c);
  hb_always_assert(c->آخری_added_بچہ_اشاریہ() == obj_d);

  إذا (!with_حد_توں_ودھ) {
    obj_e_1 = جوڑو_object("a", 1, c);
  }

  // Obj C
  c->push();
  جوڑو_وتھ(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  بنانشانی obj_c = c->نکالو_pack(خطأ);

  جوڑو_وتھ(obj_c, c);

  // Obj B
  بنانشانی obj_b = جوڑو_object("b", 1, c);
  جوڑو_وتھ(obj_b, c);

  // Obj A
  c->repack_آخری(obj_d);
  c->نکالو_pack(خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_spaces (hb_serialize_context_t* c, منطقي with_حد_توں_ودھ)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_i;

  إذا (with_حد_توں_ودھ)
    obj_i = جوڑو_object ("i", 1, c);

  // Space 2
  بنانشانی obj_h = جوڑو_object ("h", 1, c);

  شروع_object (large_string.c_str(), 30000, c);
  جوڑو_وتھ (obj_h, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  // Space 1
  إذا (!with_حد_توں_ودھ)
    obj_i = جوڑو_object ("i", 1, c);

  شروع_object (large_string.c_str(), 30000, c);
  جوڑو_وتھ (obj_i, c);
  بنانشانی obj_g = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 30000, c);
  جوڑو_وتھ (obj_i, c);
  بنانشانی obj_f = c->نکالو_pack (خطأ);

  شروع_object ("d", 1, c);
  جوڑو_وتھ (obj_g, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_wide_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_c, c);
  جوڑو_wide_وتھ (obj_d, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_g = جوڑو_object ("g", 1, c);
  بنانشانی obj_h = جوڑو_object ("h", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_g, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_h, c);
  بنانشانی obj_f = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object ("d", 1, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_e, c);
  جوڑو_وتھ (obj_h, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_c, c);
  جوڑو_wide_وتھ (obj_d, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_g_مفرد = جوڑو_object ("g", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_g_مفرد, c);
  بنانشانی obj_e_مفرد = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_e_مفرد, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  بنانشانی obj_h_مفرد = جوڑو_object ("h", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_h_مفرد, c);
  بنانشانی obj_f = c->نکالو_pack (خطأ);

  شروع_object ("d", 1, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  بنانشانی obj_g = جوڑو_object ("g", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_g, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  بنانشانی obj_h = جوڑو_object ("h", 1, c);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_e, c);
  جوڑو_وتھ (obj_h, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_c, c);
  جوڑو_wide_وتھ (obj_d, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize ();
}

ساكن فراغ
populate_serializer_short_and_wide_subgraph_جڑ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_e = جوڑو_object ("e", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_c, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_c, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_c, c);
  جوڑو_wide_وتھ (obj_d, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_short_and_wide_subgraph_جڑ_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_e_مفرد = جوڑو_object ("e", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_e_مفرد, c);
  بنانشانی obj_c_مفرد = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_c_مفرد, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  بنانشانی obj_e = جوڑو_object ("e", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);


  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_c, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_c_مفرد, c);
  جوڑو_wide_وتھ (obj_d, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_ونڈو_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_f = جوڑو_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 40000, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_wide_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_c, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_ونڈو_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_f = جوڑو_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str(), 40000, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_c, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_ونڈو_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_f_مفرد = جوڑو_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جوڑو_وتھ (obj_f_مفرد, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  بنانشانی obj_f = جوڑو_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_wide_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_c, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_ونڈو_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  // Space 2

  بنانشانی obj_f_double_مفرد = جوڑو_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جوڑو_وتھ (obj_f_double_مفرد, c);
  بنانشانی obj_d_مفرد = c->نکالو_pack (خطأ);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_d_مفرد, c);
  بنانشانی obj_b_مفرد = c->نکالو_pack (خطأ);

  // Space 1

  بنانشانی obj_f_مفرد = جوڑو_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جوڑو_وتھ (obj_f_مفرد, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  // Space 0

  بنانشانی obj_f = جوڑو_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  //جڑ
  شروع_object ("a", 1, c);
  جوڑو_وتھ (obj_b, c);
  جوڑو_wide_وتھ (obj_b_مفرد, c);
  جوڑو_wide_وتھ (obj_c, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بنانشانی obj_5 = جوڑو_object ("mn", 2, c);

  بنانشانی obj_4 = جوڑو_object ("jkl", 3, c);

  شروع_object ("ghi", 3, c);
  جوڑو_وتھ (obj_4, c);
  بنانشانی obj_3 = c->نکالو_pack (خطأ);

  شروع_object ("def", 3, c);
  جوڑو_وتھ (obj_3, c);
  بنانشانی obj_2 = c->نکالو_pack (خطأ);

  شروع_object ("abc", 3, c);
  جوڑو_وتھ (obj_2, c);
  جوڑو_وتھ (obj_4, c);
  جوڑو_وتھ (obj_5, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بنانشانی obj_6 = جوڑو_object ("opqrst", 6, c);

  بنانشانی obj_5 = جوڑو_object ("mn", 2, c);

  شروع_object ("jkl", 3, c);
  جوڑو_وتھ (obj_6, c);
  بنانشانی obj_4 = c->نکالو_pack (خطأ);

  شروع_object ("ghi", 3, c);
  جوڑو_وتھ (obj_4, c);
  بنانشانی obj_3 = c->نکالو_pack (خطأ);

  شروع_object ("def", 3, c);
  جوڑو_وتھ (obj_3, c);
  بنانشانی obj_2 = c->نکالو_pack (خطأ);

  شروع_object ("abc", 3, c);
  جوڑو_وتھ (obj_2, c);
  جوڑو_وتھ (obj_4, c);
  جوڑو_وتھ (obj_5, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بنانشانی obj_d = جوڑو_object ("d", 1, c);

  شروع_object ("b", 1, c);
  جوڑو_وتھ (obj_d, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("e", 1, c);
  جوڑو_virtual_وتھ (obj_b, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  شروع_object ("c", 1, c);
  جوڑو_وتھ (obj_e, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_وتھ (obj_b, c);
  جوڑو_وتھ (obj_c, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_24_and_32_ثنائی_ہندسہ_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی obj_f = جوڑو_object ("f", 1, c);
  بنانشانی obj_g = جوڑو_object ("g", 1, c);
  بنانشانی obj_j = جوڑو_object ("j", 1, c);
  بنانشانی obj_k = جوڑو_object ("k", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_f, c);
  بنانشانی obj_c = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_g, c);
  بنانشانی obj_d = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_j, c);
  بنانشانی obj_h = c->نکالو_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جوڑو_وتھ (obj_k, c);
  بنانشانی obj_i = c->نکالو_pack (خطأ);

  شروع_object ("e", 1, c);
  جوڑو_wide_وتھ (obj_h, c);
  جوڑو_wide_وتھ (obj_i, c);
  بنانشانی obj_e = c->نکالو_pack (خطأ);

  شروع_object ("b", 1, c);
  جوڑو_24_وتھ (obj_c, c);
  جوڑو_24_وتھ (obj_d, c);
  جوڑو_24_وتھ (obj_e, c);
  بنانشانی obj_b = c->نکالو_pack (خطأ);

  شروع_object ("a", 1, c);
  جوڑو_24_وتھ (obj_b, c);
  c->نکالو_pack (خطأ);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              صحيح عدد_extensions = 0,
                                              منطقي shared_subtables = خطأ,
                                              منطقي نسخہ_1_2 = خطأ)
{
  constexpr صحيح عدد_lookups = 5;
  constexpr صحيح عدد_subtables = عدد_lookups * 2;
  بنانشانی صحيح lookups[عدد_lookups];
  بنانشانی صحيح subtables[عدد_subtables];
  بنانشانی صحيح extensions[عدد_subtables];

  std::string large_string(60000, 'a');
  c->شروع_serialize<حرف> ();


  لكل (صحيح i = عدد_subtables - 1; i >= 0; i--)
    subtables[i] = جوڑو_object(large_string.c_str (), 15000 + i, c);

  لكل (صحيح i = عدد_subtables - 1;
       i >= (عدد_lookups - عدد_extensions) * 2;
       i--)
  {
    extensions[i] = جوڑو_extension (subtables[i], 5, c);
  }

  لكل (صحيح i = عدد_lookups - 1; i >= 0; i--)
  {
    منطقي is_ext = (i >= (عدد_lookups - عدد_extensions));

    شروع_lookup (is_ext ? (حرف) 7 : (حرف) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    إذا (is_ext) {
      إذا (shared_subtables && i > 2) {
        جوڑو_وتھ (extensions[i * 2 - 1], c);
      }
      جوڑو_وتھ (extensions[i * 2], c);
      جوڑو_وتھ (extensions[i * 2 + 1], c);
    } وإلا {
      إذا (shared_subtables && i > 2) {
        جوڑو_وتھ (subtables[i * 2 - 1], c);
      }
      جوڑو_وتھ (subtables[i * 2], c);
      جوڑو_وتھ (subtables[i * 2 + 1], c);
    }

    lookups[i] = انت_lookup (c);
  }

  بنانشانی lookup_list = نسخہ_1_2
                         ? جوڑو_lookup_list_1_2 (lookups, عدد_lookups, c)
                         : جوڑو_lookup_list (lookups, عدد_lookups, c);

  إذا (نسخہ_1_2)
    جوڑو_gsubgpos_سرخی_1_2 (lookup_list, c);
  وإلا
    جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->سرا_serialize();
}

سانچا<صحيح عدد_pair_pos_1, صحيح عدد_pair_سیٹ>
ساكن فراغ
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           منطقي as_extension = خطأ)
{
  std::string large_string(60000, 'a');
  c->شروع_serialize<حرف> ();

  constexpr صحيح کل_pair_سیٹ = عدد_pair_pos_1 * عدد_pair_سیٹ;
  بنانشانی pair_سیٹ_2[کل_pair_سیٹ];
  بنانشانی coverage[عدد_pair_pos_1];
  بنانشانی pair_pos_1[عدد_pair_pos_1];

  لكل (صحيح i = عدد_pair_pos_1 - 1; i >= 0; i--)
  {
    لكل (صحيح j = (i + 1) * عدد_pair_سیٹ - 1; j >= i * عدد_pair_سیٹ; j--)
      pair_سیٹ_2[j] = جوڑو_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = جوڑو_coverage (i * عدد_pair_سیٹ,
                                (i + 1) * عدد_pair_سیٹ - 1, c);

    pair_pos_1[i] = جوڑو_pair_pos_1 (&pair_سیٹ_2[i * عدد_pair_سیٹ],
                                    عدد_pair_سیٹ,
                                    coverage[i],
                                    c);
  }

  بنانشانی pair_pos_2 = جوڑو_object (large_string.c_str(), 200, c);

  إذا (as_extension) {
    pair_pos_2 = جوڑو_extension (pair_pos_2, 2, c);
    لكل (صحيح i = عدد_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = جوڑو_extension (pair_pos_1[i], 2, c);
  }

  شروع_lookup (as_extension ? 9 : 2, 1 + عدد_pair_pos_1, c);

  لكل (صحيح i = 0; i < عدد_pair_pos_1; i++)
    جوڑو_وتھ (pair_pos_1[i], c);
  جوڑو_وتھ (pair_pos_2, c);

  بنانشانی lookup = انت_lookup (c);

  بنانشانی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->سرا_serialize();
}

سانچا<صحيح عدد_pair_pos_2, صحيح عدد_class_1, صحيح عدد_class_2>
ساكن فراغ
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           منطقي as_extension = خطأ,
                                           منطقي with_device_tables = خطأ,
                                           منطقي extra_جدول = سچ)
{
  std::string large_string(100000, 'a');
  c->شروع_serialize<حرف> ();

  بنانشانی coverage[عدد_pair_pos_2];
  بنانشانی class_def_1[عدد_pair_pos_2];
  بنانشانی class_def_2[عدد_pair_pos_2];
  بنانشانی pair_pos_2[عدد_pair_pos_2];

  بنانشانی* device_tables = (بنانشانی*) صفر_حافظہ_دیو (عدد_pair_pos_2 * عدد_class_1 * عدد_class_2,
                                                حجم(بنانشانی));

  // Total glyphs = num_class_1 * num_pair_pos_2
  لكل (صحيح i = عدد_pair_pos_2 - 1; i >= 0; i--)
  {
    بنانشانی شروع_glyph = 5 + i * عدد_class_1;
    إذا (عدد_class_2 >= عدد_class_1)
    {
      class_def_2[i] = جوڑو_class_def (11,
                                      10 + عدد_class_2, c);
      class_def_1[i] = جوڑو_class_def (شروع_glyph + 1,
                                      شروع_glyph + عدد_class_1,
                                      c);
    } وإلا {
      class_def_1[i] = جوڑو_class_def (شروع_glyph + 1,
                                      شروع_glyph + عدد_class_1,
                                      c);
      class_def_2[i] = جوڑو_class_def (11,
                                      10 + عدد_class_2, c);
    }

    coverage[i] = جوڑو_coverage (شروع_glyph,
                                شروع_glyph + عدد_class_1 - 1,
                                c);

    إذا (with_device_tables)
    {
      لكل(صحيح j = (i + 1) * عدد_class_1 * عدد_class_2 - 1;
          j >= i * عدد_class_1 * عدد_class_2;
          j--)
      {
        uint8_t جدول[] = {
          (uint8_t) ((j >> 8) & 0xFF),
          (uint8_t) (j & 0xFF),
        };
        device_tables[j] = جوڑو_object ((حرف*) جدول, 2, c);
      }
    }

    pair_pos_2[i] = جوڑو_pair_pos_2 (1 + i * عدد_class_1,
                                    coverage[i],
                                    class_def_1[i], عدد_class_1,
                                    class_def_2[i], عدد_class_2,
                                    with_device_tables
                                    ? &device_tables[i * عدد_class_1 * عدد_class_2]
                                    : خالی_سوچک,
                                    c);
  }


  بنانشانی pair_pos_1 = 0;
  إذا (extra_جدول) pair_pos_1 = جوڑو_object (large_string.c_str(), 100000, c);

  إذا (as_extension) {
    لكل (صحيح i = عدد_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = جوڑو_extension (pair_pos_2[i], 2, c);

    إذا (extra_جدول)
      pair_pos_1 = جوڑو_extension (pair_pos_1, 2, c);
  }

  شروع_lookup (as_extension ? 9 : 2, 1 + عدد_pair_pos_2, c);

  إذا (extra_جدول)
    جوڑو_وتھ (pair_pos_1, c);

  لكل (صحيح i = 0; i < عدد_pair_pos_2; i++)
    جوڑو_وتھ (pair_pos_2[i], c);

  بنانشانی lookup = انت_lookup (c);

  بنانشانی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->سرا_serialize();

  حرر (device_tables);
}

سانچا<صحيح mark_گنتی,
    صحيح class_گنتی,
    صحيح ادھار_گنتی,
    صحيح جدول_گنتی_2>
ساكن فراغ
populate_serializer_with_large_mark_ادھار_pos_1 (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  Mark_ادھار_Pos_Buffers<mark_گنتی, class_گنتی, ادھار_گنتی, جدول_گنتی_2> buffers (c);

  بنانشانی mark_ادھار_pos[جدول_گنتی_2];
  لكل (بنانشانی i = 0; i < جدول_گنتی_2; i++)
    mark_ادھار_pos[i] = buffers.create_mark_ادھار_pos_1 (i, c);

  لكل (صحيح i = 0; i < جدول_گنتی_2; i++)
    mark_ادھار_pos[i] = جوڑو_extension (mark_ادھار_pos[i], 4, c);

  شروع_lookup (9, جدول_گنتی_2, c);

  لكل (صحيح i = 0; i < جدول_گنتی_2; i++)
    جوڑو_وتھ (mark_ادھار_pos[i], c);

  بنانشانی lookup = انت_lookup (c);

  بنانشانی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->سرا_serialize();
}

سانچا<بنانشانی Lig_Subst_گنتی,
         بنانشانی Liga_سیٹ_گنتی,
         بنانشانی Liga_فی_سیٹ_گنتی,
         بنانشانی Liga_ناپ>
ساكن فراغ populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    منطقي sequential_liga_sets,
                                                    منطقي shared,
                                                    بنانشانی *liga_subst_id,
                                                    منطقي unique_lig_str = خطأ)
{
  حرف ch = 'a';

  بنانشانی liga[Liga_سیٹ_گنتی * Liga_فی_سیٹ_گنتی];
  بنانشانی liga_سیٹ[Liga_سیٹ_گنتی];
  لكل (بنانشانی l = 0; l < Lig_Subst_گنتی; l++) {
    بنانشانی coverage_شروع = 0;
    بنانشانی coverage_سرا = Liga_سیٹ_گنتی - 1;
    إذا (sequential_liga_sets) {
      coverage_شروع = l * Liga_سیٹ_گنتی;
      coverage_سرا = (l + 1) * Liga_سیٹ_گنتی - 1;
    }
    بنانشانی coverage = جوڑو_coverage(coverage_شروع, coverage_سرا, c, shared);

    لكل (بنانشانی i = 0; i < Liga_سیٹ_گنتی; i++) {
      لكل (بنانشانی j = 0; j < Liga_فی_سیٹ_گنتی; j++)
      {
        std::string large_string(100000, ch);
        شروع_object (large_string.c_str(), Liga_ناپ, c);
        إذا (unique_lig_str)
          ch += 1;
        جوڑو_virtual_وتھ(coverage, c);
        liga[i * Liga_فی_سیٹ_گنتی + j] = c->نکالو_pack (shared);
      }

      جوڑو_liga_سیٹ_سرخی(Liga_فی_سیٹ_گنتی, c);
      جوڑو_virtual_وتھ(coverage, c);
      لكل (بنانشانی j = 0; j < Liga_فی_سیٹ_گنتی; j++)
      {
        جوڑو_وتھ(liga[i * Liga_فی_سیٹ_گنتی + j], c);
      }

      liga_سیٹ[i] = c->نکالو_pack(shared);
    }

    جوڑو_liga_سرخی(coverage, Liga_سیٹ_گنتی, c);
    لكل (بنانشانی i = 0; i < Liga_سیٹ_گنتی; i++)
    {
      جوڑو_وتھ(liga_سیٹ[i], c);
    }

    liga_subst_id[l] = c->نکالو_pack(shared);
  }

  لكل (بنانشانی l = 0; l < Lig_Subst_گنتی; l++) {
    liga_subst_id[l] = جوڑو_extension(liga_subst_id[l], 4, c);
  }
}

سانچا<بنانشانی liga_subst_گنتی,
         بنانشانی liga_سیٹ_گنتی,
         بنانشانی liga_فی_سیٹ_گنتی,
         بنانشانی liga_ناپ>
ساكن فراغ
populate_serializer_with_large_liga (hb_serialize_context_t* c, منطقي sequential_liga_sets)
{
  c->شروع_serialize<حرف> ();

  بنانشانی liga_subst[liga_subst_گنتی];
  populate_serializer_with_large_ligsubst<liga_subst_گنتی, liga_سیٹ_گنتی, liga_فی_سیٹ_گنتی, liga_ناپ>(c, sequential_liga_sets, خطأ, liga_subst);

  شروع_lookup (7, liga_subst_گنتی, c);
  لكل (بنانشانی l = 0; l < liga_subst_گنتی; l++) {
    جوڑو_وتھ(liga_subst[l], c);
  }

  بنانشانی lookup = انت_lookup (c);

  بنانشانی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->سرا_serialize();
}

ساكن فراغ
populate_serializer_with_large_liga_overlapping_clone_نتیجہ (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->شروع_serialize<حرف> ();

  constexpr بنانشانی liga_ناپ = 30000;

  بنانشانی liga[6];
  بنانشانی liga_subst[3];
  بنانشانی liga_سیٹ[2];

  // LigaSubst 3
  بنانشانی coverage = جوڑو_coverage(1, 1, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    شروع_object (large_string.c_str(), liga_ناپ, c);
    جوڑو_virtual_وتھ(coverage, c);
    liga[i] = c->نکالو_pack (خطأ);
  }

  جوڑو_liga_سیٹ_سرخی(2, c);
  جوڑو_virtual_وتھ(coverage, c);
  لكل (بنانشانی i = 0; i < 2; i++)
    جوڑو_وتھ(liga[i], c);
  liga_سیٹ[0] = c->نکالو_pack(خطأ);

  جوڑو_liga_سرخی(coverage, 1, c);
  جوڑو_وتھ(liga_سیٹ[0], c);
  liga_subst[2] = c->نکالو_pack(خطأ);

  // LigaSubst 2
  coverage = جوڑو_coverage(0, 1, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    شروع_object (large_string.c_str(), liga_ناپ, c);
    جوڑو_virtual_وتھ(coverage, c);
    liga[i] = c->نکالو_pack (خطأ);
  }

  جوڑو_liga_سیٹ_سرخی(1, c);
  جوڑو_virtual_وتھ(coverage, c);
  جوڑو_وتھ(liga[1], c);
  liga_سیٹ[1] = c->نکالو_pack(خطأ);

  جوڑو_liga_سیٹ_سرخی(1, c);
  جوڑو_virtual_وتھ(coverage, c);
  جوڑو_وتھ(liga[0], c);
  liga_سیٹ[0] = c->نکالو_pack(خطأ);

  جوڑو_liga_سرخی(coverage, 2, c);
  جوڑو_وتھ(liga_سیٹ[0], c);
  جوڑو_وتھ(liga_سیٹ[1], c);
  liga_subst[1] = c->نکالو_pack(خطأ);

  // LigaSubst 1
  coverage = جوڑو_coverage(0, 0, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    شروع_object (large_string.c_str(), liga_ناپ, c);
    جوڑو_virtual_وتھ(coverage, c);
    liga[i] = c->نکالو_pack (خطأ);
  }

  جوڑو_liga_سیٹ_سرخی(2, c);
  جوڑو_virtual_وتھ(coverage, c);
  لكل (بنانشانی i = 0; i < 2; i++)
    جوڑو_وتھ(liga[i], c);
  liga_سیٹ[0] = c->نکالو_pack(خطأ);

  جوڑو_liga_سرخی(coverage, 1, c);
  جوڑو_وتھ(liga_سیٹ[0], c);
  liga_subst[0] = c->نکالو_pack(خطأ);

  لكل (صحيح l = 2; l >= 0; l--) {
    liga_subst[l] = جوڑو_extension(liga_subst[l], 4, c);
  }

  شروع_lookup (7, 3, c);
  لكل (بنانشانی l = 0; l < 3; l++) {
    جوڑو_وتھ(liga_subst[l], c);
  }

  بنانشانی lookup = انت_lookup (c);

  بنانشانی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->سرا_serialize();
}

سانچا<بنانشانی liga_subst_گنتی,
         بنانشانی liga_سیٹ_گنتی,
         بنانشانی liga_فی_سیٹ_گنتی,
         بنانشانی liga_ناپ>
ساكن فراغ
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, منطقي sequential_liga_sets)
{
  c->شروع_serialize<حرف> ();

  بنانشانی lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  بنانشانی liga_subst_idx[liga_subst_گنتی];
  populate_serializer_with_large_ligsubst<liga_subst_گنتی, liga_سیٹ_گنتی, liga_فی_سیٹ_گنتی, liga_ناپ>(c, sequential_liga_sets, سچ, liga_subst_idx, سچ);

  شروع_lookup (7, liga_subst_گنتی, c);
  لكل (بنانشانی l = 0; l < liga_subst_گنتی; l++) {
    جوڑو_وتھ(liga_subst_idx[l], c);
  }

  lookups[0] = انت_lookup (c);

  // Lookup with 2 LigSubst tables
  بنانشانی liga_subst[liga_subst_گنتی + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, خطأ, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_گنتی, liga_سیٹ_گنتی, liga_فی_سیٹ_گنتی, liga_ناپ>(c, sequential_liga_sets, سچ, &liga_subst[1], سچ);

  شروع_lookup (7, liga_subst_گنتی + 1, c);
  لكل (بنانشانی l = 0; l <= liga_subst_گنتی; l++) {
    جوڑو_وتھ(liga_subst[l], c);
  }

  lookups[1] = انت_lookup (c);

  بنانشانی lookup_list = جوڑو_lookup_list (lookups, 2, c);
  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->سرا_serialize();
}

سانچا<بنانشانی liga_subst_گنتی,
         بنانشانی liga_سیٹ_گنتی,
         بنانشانی liga_فی_سیٹ_گنتی,
         بنانشانی liga_ناپ>
ساكن فراغ
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بنانشانی liga_subst[liga_subst_گنتی + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, سچ, سچ, liga_subst, سچ);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_گنتی, liga_سیٹ_گنتی, liga_فی_سیٹ_گنتی, liga_ناپ>(c, سچ, سچ, &liga_subst[1], سچ);

  شروع_lookup (7, liga_subst_گنتی + 1, c);
  لكل (بنانشانی l = 0; l <= liga_subst_گنتی; l++) {
    جوڑو_وتھ(liga_subst[l], c);
  }

  بنانشانی lookup = انت_lookup (c);

  بنانشانی lookup_list = جوڑو_lookup_list (&lookup, 1, c);
  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->سرا_serialize();
}

ساكن فراغ test_ترتیب_دیو_سبھ_توں_تھوڑا ()
{
  size_t بفر_ناپ = 100;
  فراغ* بفر_a = حافظہ_دیو (بفر_ناپ);
  فراغ* بفر_e = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t a (بفر_a, بفر_ناپ);
  hb_serialize_context_t e (بفر_e, بفر_ناپ);
  populate_serializer_complex_2 (&a);

  گراف_t گراف = گراف_t::create (a.object_گراف ()).value ();
  hb_always_assert (گراف.ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ ().is_ok());
  گراف.normalize();

  // Expected graph
  e.شروع_serialize();
  بنانشانی jkl = جوڑو_object ("jkl", 3, &e);

  شروع_object("ghi", 3, &e);
  جوڑو_وتھ(jkl, &e);
  بنانشانی ghi = e.نکالو_pack(خطأ);

  شروع_object("def", 3, &e);
  جوڑو_وتھ(ghi, &e);
  بنانشانی def = e.نکالو_pack(خطأ);

  بنانشانی mn = جوڑو_object("mn", 2, &e);

  شروع_object ("abc", 3, &e);
  جوڑو_وتھ (def, &e);
  جوڑو_وتھ (jkl, &e);
  جوڑو_وتھ (mn, &e);
  e.نکالو_pack (خطأ);
  e.سرا_serialize();

  گراف_t expected = گراف_t::create (e.object_گراف ()).value ();
  expected.normalize();

  أكد(expected == گراف);

  حرر (بفر_a);
  حرر (بفر_e);
}

ساكن فراغ test_duplicate_پتی ()
{
  size_t بفر_ناپ = 100;
  فراغ* بفر_a = حافظہ_دیو (بفر_ناپ);
  فراغ* بفر_e = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t a (بفر_a, بفر_ناپ);
  hb_serialize_context_t e (بفر_e, بفر_ناپ);
  populate_serializer_complex_2 (&a);

  گراف_t گراف = گراف_t::create (a.object_گراف ()).value ();
  hb_always_assert (گراف.duplicate (4, 1, خطأ).is_ok ());
  گراف.normalize();

  e.شروع_serialize();
  بنانشانی mn = جوڑو_object("mn", 2, &e);
  بنانشانی jkl_2 = جوڑو_object("jkl", 3, &e);

  شروع_object("ghi", 3, &e);
  جوڑو_وتھ(jkl_2, &e);
  بنانشانی ghi = e.نکالو_pack(خطأ);

  شروع_object("def", 3, &e);
  جوڑو_وتھ(ghi, &e);
  بنانشانی def = e.نکالو_pack(خطأ);

  بنانشانی jkl_1 = جوڑو_object("jkl", 3, &e);

  شروع_object("abc", 3, &e);
  جوڑو_وتھ(def, &e);
  جوڑو_وتھ(jkl_1, &e);
  جوڑو_وتھ(mn, &e);
  e.نکالو_pack(خطأ);

  گراف_t expected = گراف_t::create (e.object_گراف ()).value ();
  expected.normalize();

  أكد(expected == گراف);

  حرر (بفر_a);
  حرر (بفر_e);
}

ساكن فراغ test_duplicate_interior ()
{
  size_t بفر_ناپ = 100;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_complex_3 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();
  hb_always_assert (گراف.duplicate (3, 2, خطأ).is_ok ());

  hb_always_assert(لڑی_مقابلہ (گراف.object (6).سر_2, "jkl", 3) == 0);
  hb_always_assert(گراف.object (6).real_links.لمائی == 1);
  hb_always_assert(گراف.object (6).real_links[0].objidx == 0);

  hb_always_assert(لڑی_مقابلہ (گراف.object (5).سر_2, "abc", 3) == 0);
  hb_always_assert(گراف.object (5).real_links.لمائی == 3);
  hb_always_assert(گراف.object (5).real_links[0].objidx == 4);
  hb_always_assert(گراف.object (5).real_links[1].objidx == 2);
  hb_always_assert(گراف.object (5).real_links[2].objidx == 1);

  hb_always_assert(لڑی_مقابلہ (گراف.object (4).سر_2, "def", 3) == 0);
  hb_always_assert(گراف.object (4).real_links.لمائی == 1);
  hb_always_assert(گراف.object (4).real_links[0].objidx == 3);

  hb_always_assert(لڑی_مقابلہ (گراف.object (3).سر_2, "ghi", 3) == 0);
  hb_always_assert(گراف.object (3).real_links.لمائی == 1);
  hb_always_assert(گراف.object (3).real_links[0].objidx == 6);

  hb_always_assert(لڑی_مقابلہ (گراف.object (2).سر_2, "jkl", 3) == 0);
  hb_always_assert(گراف.object (2).real_links.لمائی == 1);
  hb_always_assert(گراف.object (2).real_links[0].objidx == 0);

  hb_always_assert(لڑی_مقابلہ (گراف.object (1).سر_2, "mn", 2) == 0);
  hb_always_assert(گراف.object (1).real_links.لمائی == 0);

  hb_always_assert(لڑی_مقابلہ (گراف.object (0).سر_2, "opqrst", 6) == 0);
  hb_always_assert(گراف.object (0).real_links.لمائی == 0);

  حرر (بفر);
}

ساكن فراغ
test_serialize ()
{
  size_t بفر_ناپ = 100;
  فراغ* بفر_1 = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c1 (بفر_1, بفر_ناپ);
  populate_serializer_simple (&c1);
  hb_بائٹاں_t expected = c1.copy_بائٹاں ();

  گراف_t گراف = گراف_t::create (c1.object_گراف ()).value ();
  hb_blob_t* باہرلا = گراف::serialize (گراف).value ();
  حرر (بفر_1);

  hb_بائٹاں_t actual = باہرلا->as_بائٹاں ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (باہرلا);
}

ساكن فراغ test_will_حد_توں_ودھ_1 ()
{
  size_t بفر_ناپ = 100;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);

  populate_serializer_complex_2 (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (!گراف::will_حد_توں_ودھ (گراف, خالی_سوچک).value ());

  حرر (بفر);
}

ساكن فراغ test_will_حد_توں_ودھ_2 ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_حد_توں_ودھ (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (گراف::will_حد_توں_ودھ (گراف, خالی_سوچک).value ());

  حرر (بفر);
}

ساكن فراغ test_will_حد_توں_ودھ_3 ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_dedup_حد_توں_ودھ (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (گراف::will_حد_توں_ودھ (گراف, خالی_سوچک).value ());

  حرر (بفر);
}

ساكن فراغ test_resolve_overflows_راہیں_ترتیب_دیو ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_حد_توں_ودھ (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* باہرلا = hb_resolve_overflows (c.object_گراف (), HB_ٹیگ_NONE);
  hb_always_assert (باہرلا);
  hb_بائٹاں_t نتیجہ = باہرلا->as_بائٹاں ();
  hb_always_assert (نتیجہ.لمائی == (80000 + 3 + 3 * 2));

  حرر (بفر);
  hb_blob_destroy (باہرلا);
}

ساكن فراغ test_resolve_overflows_راہیں_duplication ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_dedup_حد_توں_ودھ (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* باہرلا = hb_resolve_overflows (c.object_گراف (), HB_ٹیگ_NONE);
  hb_always_assert (باہرلا);
  hb_بائٹاں_t نتیجہ = باہرلا->as_بائٹاں ();
  hb_always_assert (نتیجہ.لمائی == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  حرر (بفر);
  hb_blob_destroy (باہرلا);
}

ساكن فراغ test_resolve_overflows_راہیں_مضرب_duplications ()
{
  size_t بفر_ناپ = 300000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_مضرب_dedup_حد_توں_ودھ (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* باہرلا = hb_resolve_overflows (c.object_گراف (), HB_ٹیگ_NONE, 5);
  hb_always_assert (باہرلا);

  حرر (بفر);
  hb_blob_destroy (باہرلا);
}

ساكن فراغ test_resolve_overflows_راہیں_space_assignment ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_spaces (&c, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_spaces (&e, خطأ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_راہیں_isolation ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_isolation_حد_توں_ودھ (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.وتھ_حد_توں_ودھ ());
  hb_blob_t* باہرلا = hb_resolve_overflows (c.object_گراف (), HB_ٹیگ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (باہرلا);
  hb_بائٹاں_t نتیجہ = باہرلا->as_بائٹاں ();
  hb_always_assert (نتیجہ.لمائی == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  حرر (بفر);
  hb_blob_destroy (باہرلا);
}

ساكن فراغ test_resolve_overflows_راہیں_isolation_with_recursive_duplication ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_isolation_حد_توں_ودھ_complex (&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_isolation_حد_توں_ودھ_complex_expected (&e);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_راہیں_isolating_16bit_space ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_spaces_16bit_connection (&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_spaces_16bit_connection_expected (&e);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_راہیں_isolating_16bit_space_2 ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_short_and_wide_subgraph_جڑ (&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_short_and_wide_subgraph_جڑ_expected (&e);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_راہیں_isolation_spaces ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_isolation_حد_توں_ودھ_spaces (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.وتھ_حد_توں_ودھ ());
  hb_blob_t* باہرلا = hb_resolve_overflows (c.object_گراف (), HB_ٹیگ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (باہرلا);
  hb_بائٹاں_t نتیجہ = باہرلا->as_بائٹاں ();

  بنانشانی expected_لمائی = 3 + 2 * 60000; // objects
  expected_لمائی += 2 * 4 + 2 * 2; // links
  hb_always_assert (نتیجہ.لمائی == expected_لمائی);

  حرر (بفر);
  hb_blob_destroy (باہرلا);
}

ساكن فراغ test_resolve_mixed_overflows_راہیں_isolation_spaces ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_24_and_32_ثنائی_ہندسہ_offsets (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.وتھ_حد_توں_ودھ ());
  hb_blob_t* باہرلا = hb_resolve_overflows (c.object_گراف (), HB_ٹیگ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (باہرلا);
  hb_بائٹاں_t نتیجہ = باہرلا->as_بائٹاں ();

  بنانشانی expected_لمائی =
      // Objects
      7 +
      4 * 40000;

  expected_لمائی +=
      // Links
      2 * 4 +  // 32
      4 * 3 +  // 24
      4 * 2;   // 16

  hb_always_assert (نتیجہ.لمائی == expected_لمائی);

  حرر (بفر);
  hb_blob_destroy (باہرلا);
}

ساكن فراغ test_resolve_with_extension_promotion ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_extension_promotion (&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_extension_promotion (&e, 3);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             سچ);
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_shared_extension_promotion ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_extension_promotion (&c, 0, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_extension_promotion (&e, 3, سچ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             سچ);
  حرر (بفر);
  حرر (expected_بفر);
}

#جے_بنا_تعریف HB_NO_BEYOND_64K
ساكن فراغ test_resolve_with_extension_promotion_1_2 ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_extension_promotion (&c, 0, خطأ, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_extension_promotion (&e, 3, خطأ, سچ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             سچ);
  حرر (بفر);
  حرر (expected_بفر);
}
#جے_انت

ساكن فراغ test_resolve_with_repeated_extension_promotion ()
{
  size_t بفر_ناپ = 1000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);

  c.شروع_serialize<حرف> ();
  بنانشانی subtable = جوڑو_object ("subt", 4, &c);
  شروع_lookup (5, 2, &c);
  جوڑو_وتھ (subtable, &c);
  جوڑو_وتھ (subtable, &c);
  بنانشانی lookup = انت_lookup (&c);
  بنانشانی lookup_list = جوڑو_lookup_list (&lookup, 1, &c);
  جوڑو_gsubgpos_سرخی (lookup_list, &c);
  c.سرا_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();
  خودکار context = گراف::gsubgpos_گراف_context_t::create (HB_ٹیگ ('G', 'S', 'U', 'B'),
                                                          گراف).value ();
  hb_always_assert (context.lookups.get_population () == 1);
  بنانشانی lookup_اشاریہ = *context.lookups.کلیدیں ();
  گراف::Lookup* lookup_جدول_2 = context.lookups.get (lookup_اشاریہ);
  hb_always_assert (lookup_جدول_2->make_extension (context, lookup_اشاریہ).is_ok ());

  ثابت خودکار& lookup_راس = گراف.vertices_[lookup_اشاریہ];
  hb_always_assert (lookup_راس.obj ().real_links.لمائی == 2);
  بنانشانی extension_اشاریہ = lookup_راس.obj ().real_links[0].objidx;
  hb_always_assert (lookup_راس.obj ().real_links[1].objidx == extension_اشاریہ);

  خودکار* extension = گراف.vertices_[extension_اشاریہ].as<
      گراف::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
  بنانشانی promoted_subtable = extension->get_subtable_اشاریہ (گراف, extension_اشاریہ).value ();
  hb_always_assert (گراف.vertices_[promoted_subtable].جدول_ناپ () == 4);
  hb_always_assert (گراف.update_parents ().is_ok ());
  hb_always_assert (گراف.vertices_[extension_اشاریہ].incoming_کنارے_2 () == 2);
  hb_always_assert (گراف.vertices_[promoted_subtable].incoming_کنارے_2 () == 1);

  hb_blob_t* باہرلا = گراف::serialize (گراف).value ();
  hb_blob_destroy (باہرلا);
  حرر (بفر);
}

ساكن فراغ test_resolve_with_basic_pair_pos_1_ونڈو ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, سچ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_extension_pair_pos_1_ونڈو ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, سچ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_basic_pair_pos_2_ونڈو ()
{
  size_t بفر_ناپ = 300000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, سچ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_close_منزل_حد_بندی_pair_pos_2_ونڈو ()
{
  size_t بفر_ناپ = 300000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, سچ, خطأ, خطأ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, سچ, خطأ, خطأ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_pair_pos_2_ونڈو_with_device_tables ()
{
  size_t بفر_ناپ = 300000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, خطأ, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, سچ, سچ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_basic_mark_ادھار_pos_1_ونڈو ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_large_mark_ادھار_pos_1 <40, 10, 110, 1>(&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_large_mark_ادھار_pos_1 <40, 10, 110, 2>(&e);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_basic_liga_ونڈو ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, خطأ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, خطأ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_ونڈو_move ()
{
  size_t بفر_ناپ = 400000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, سچ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_ونڈو_overlapping_clone ()
{
  size_t بفر_ناپ = 400000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_large_liga_overlapping_clone_نتیجہ(&e);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_ونڈو_shared_جدول ()
{
  size_t بفر_ناپ = 400000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, سچ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_ونڈو_shared_coverage ()
{
  size_t بفر_ناپ = 400000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٹیگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_راہیں_splitting_spaces ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_ونڈو_spaces (&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_ونڈو_spaces_expected (&e);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  حرر (بفر);
  حرر (expected_بفر);

}

ساكن فراغ test_resolve_overflows_راہیں_splitting_spaces_2 ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_ونڈو_spaces_2 (&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_ونڈو_spaces_expected_2 (&e);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_راہیں_ترجیح ()
{
  size_t بفر_ناپ = 160000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_ترجیح_حد_توں_ودھ (&c);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_ترجیح_حد_توں_ودھ_expected (&e);

  لگاتار_resolve_حد_توں_ودھ_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  حرر (بفر);
  حرر (expected_بفر);
}


ساكن فراغ test_virtual_link ()
{
  size_t بفر_ناپ = 100;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_virtual_link (&c);

  hb_blob_t* باہرلا = hb_resolve_overflows (c.object_گراف (), HB_ٹیگ_NONE);
  hb_always_assert (باہرلا);

  hb_بائٹاں_t نتیجہ = باہرلا->as_بائٹاں ();
  hb_always_assert (نتیجہ.لمائی == 5 + 4 * 2);
  hb_always_assert (نتیجہ[0]  == 'a');
  hb_always_assert (نتیجہ[5]  == 'c');
  hb_always_assert (نتیجہ[8]  == 'e');
  hb_always_assert (نتیجہ[9]  == 'b');
  hb_always_assert (نتیجہ[12] == 'd');

  حرر (بفر);
  hb_blob_destroy (باہرلا);
}

ساكن فراغ
test_shared_گرہ_with_virtual_links ()
{
  size_t بفر_ناپ = 100;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);

  c.شروع_serialize<حرف> ();

  بنانشانی obj_b = جوڑو_object ("b", 1, &c);
  بنانشانی obj_c = جوڑو_object ("c", 1, &c);

  شروع_object ("d", 1, &c);
  جوڑو_virtual_وتھ (obj_b, &c);
  بنانشانی obj_d_1 = c.نکالو_pack ();

  شروع_object ("d", 1, &c);
  جوڑو_virtual_وتھ (obj_c, &c);
  بنانشانی obj_d_2 = c.نکالو_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  شروع_object ("a", 1, &c);
  جوڑو_وتھ (obj_b, &c);
  جوڑو_وتھ (obj_c, &c);
  جوڑو_وتھ (obj_d_1, &c);
  جوڑو_وتھ (obj_d_2, &c);
  c.نکالو_pack ();
  c.سرا_serialize ();

  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links.لمائی == 2);
  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links[1].objidx == obj_c);
  حرر(بفر);
}

ساكن فراغ
test_repack_آخری ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_with_repack_آخری (&c, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_with_repack_آخری (&e, خطأ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_repack_last",
                             c,
                             e,
                             20,
                             خطأ,
                             HB_ٹیگ('a', 'b', 'c', 'd'),
                             سچ);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ
test_dont_duplicate_virtual ()
{
  size_t بفر_ناپ = 200000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_virtual (&c, سچ);

  فراغ* expected_بفر = حافظہ_دیو (بفر_ناپ);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_ناپ);
  populate_serializer_virtual (&e, خطأ);

  لگاتار_resolve_حد_توں_ودھ_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             خطأ,
                             HB_ٹیگ('a', 'b', 'c', 'd'),
                             سچ);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ
test_deep_گراف_traversal ()
{
  size_t بفر_ناپ = 1000000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);

  c.شروع_serialize ();
  بنانشانی prev = جوڑو_object ("leaf", 4, &c);
  لكل (بنانشانی i = 0; i < 2000; i++)
  {
    شروع_object ("node", 4, &c);
    جوڑو_وتھ (prev, &c);
    prev = c.نکالو_pack (خطأ);
  }
  c.سرا_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // Test find_subgraph (set)
  hb_سیٹ_t ویکھیا_سیٹ;
  hb_always_assert (گراف.لبھو_subgraph (گراف.جڑ_idx (), ویکھیا_سیٹ).is_ok ());
  hb_always_assert (ویکھیا_سیٹ.get_population () == 2001);

  // Test find_subgraph_size
  hb_سیٹ_t ناپ_سیٹ;
  size_t sz = گراف.لبھو_subgraph_ناپ (گراف.جڑ_idx (), ناپ_سیٹ).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.سیٹ (گراف.جڑ_idx (), 1);
  hb_always_assert (گراف.لبھو_subgraph (گراف.جڑ_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (گراف.سونپو_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t اشاریہ_map;
  hb_always_assert (گراف.duplicate_subgraph (گراف.جڑ_idx (), اشاریہ_map).is_ok ());
  hb_always_assert (اشاریہ_map.get_population () == 2001);

  حرر (بفر);
}

ساكن فراغ
test_32bit_roots_traversal ()
{
  size_t بفر_ناپ = 1000;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);

  c.شروع_serialize ();
  بنانشانی leaf1 = جوڑو_object ("l1", 2, &c);
  بنانشانی leaf2 = جوڑو_object ("l2", 2, &c);

  شروع_object ("mid", 3, &c);
  جوڑو_wide_وتھ (leaf1, &c);
  بنانشانی mid = c.نکالو_pack (خطأ);

  شروع_object ("root", 4, &c);
  جوڑو_وتھ (mid, &c);
  جوڑو_wide_وتھ (leaf2, &c);
  c.نکالو_pack (خطأ);
  c.سرا_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_سیٹ_t roots;
  hb_always_assert (گراف.لبھو_32bit_roots (گراف.جڑ_idx (), roots).is_ok ());
  hb_always_assert (roots.has (leaf1 - 1));
  hb_always_assert (roots.has (leaf2 - 1));
  hb_always_assert (!roots.has (mid - 1));
  hb_always_assert (roots.get_population () == 2);

  حرر (بفر);

  // Test case where DFS fails but BFS succeeds:
  // root -16-> a
  // root -32-> a
  // a -32-> b
  //
  // DFS will first explore the 16 bit link to a and then recurse the 32 bit link from a to b
  // and consider b a root. However, b should not be a root since it's not a top level 32 bit link.
  // BFS will handle this correctly by discovering a as a top level root and then avoiding further
  // exploring it.
  بفر_ناپ = 1000;
  بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c2 (بفر, بفر_ناپ);

  c2.شروع_serialize ();
  بنانشانی b = جوڑو_object ("b", 1, &c2);

  شروع_object ("a", 1, &c2);
  جوڑو_wide_وتھ (b, &c2);
  بنانشانی a = c2.نکالو_pack (خطأ);

  شروع_object ("r", 1, &c2);
  جوڑو_وتھ (a, &c2);
  جوڑو_wide_وتھ (a, &c2);
  c2.نکالو_pack (خطأ);

  c2.سرا_serialize ();

  گراف_t graph2 = گراف_t::create (c2.object_گراف ()).value ();

  hb_سیٹ_t roots2;
  hb_always_assert (graph2.لبھو_32bit_roots (graph2.جڑ_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  حرر (بفر);
}

ساكن فراغ test_invalid_link_objidx ()
{
  size_t بفر_ناپ = 100;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_complex_2 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // Poison a link to point to (unsigned) -1
  خودکار links = گراف.vertices_[گراف.جڑ_idx ()].real_links_writer();
  links->objidx = (بنانشانی) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!گراف.ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ ().is_ok ());

  حرر (بفر);
}

ساكن فراغ test_invalid_link_جوڑو_and_move ()
{
  size_t بفر_ناپ = 100;
  فراغ* بفر = حافظہ_دیو (بفر_ناپ);
  hb_serialize_context_t c (بفر, بفر_ناپ);
  populate_serializer_complex_2 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!گراف.جوڑو_link (&dummy, 0, (بنانشانی) -1).is_ok ());

  // move_child with non-existent offset
  خودکار res = گراف.move_بچہ (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  حرر (بفر);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

صحيح
مکھ (صحيح argc, حرف **argv)
{
  test_invalid_link_objidx ();
  test_invalid_link_جوڑو_and_move ();
  test_serialize ();
  test_ترتیب_دیو_سبھ_توں_تھوڑا ();
  test_will_حد_توں_ودھ_1 ();
  test_will_حد_توں_ودھ_2 ();
  test_will_حد_توں_ودھ_3 ();
  test_resolve_overflows_راہیں_ترتیب_دیو ();
  test_resolve_overflows_راہیں_duplication ();
  test_resolve_overflows_راہیں_مضرب_duplications ();
  test_resolve_overflows_راہیں_ترجیح ();
  test_resolve_overflows_راہیں_space_assignment ();
  test_resolve_overflows_راہیں_isolation ();
  test_resolve_overflows_راہیں_isolation_with_recursive_duplication ();
  test_resolve_overflows_راہیں_isolation_spaces ();
  test_resolve_overflows_راہیں_isolating_16bit_space ();
  test_resolve_overflows_راہیں_isolating_16bit_space_2 ();
  test_resolve_overflows_راہیں_splitting_spaces ();
  test_resolve_overflows_راہیں_splitting_spaces_2 ();
  test_resolve_mixed_overflows_راہیں_isolation_spaces ();
  test_duplicate_پتی ();
  test_duplicate_interior ();
  test_virtual_link ();
  test_repack_آخری();
  test_shared_گرہ_with_virtual_links ();
  test_resolve_with_extension_promotion ();
  test_resolve_with_shared_extension_promotion ();
#جے_بنا_تعریف HB_NO_BEYOND_64K
  test_resolve_with_extension_promotion_1_2 ();
#جے_انت
  test_resolve_with_repeated_extension_promotion ();
  test_resolve_with_basic_pair_pos_1_ونڈو ();
  test_resolve_with_extension_pair_pos_1_ونڈو ();
  test_resolve_with_basic_pair_pos_2_ونڈو ();
  test_resolve_with_pair_pos_2_ونڈو_with_device_tables ();
  test_resolve_with_close_منزل_حد_بندی_pair_pos_2_ونڈو ();
  test_resolve_with_basic_mark_ادھار_pos_1_ونڈو ();
  test_resolve_with_basic_liga_ونڈو ();
  test_resolve_with_liga_ونڈو_move ();
  test_dont_duplicate_virtual ();
  test_resolve_with_liga_ونڈو_overlapping_clone ();
  test_resolve_with_liga_ونڈو_shared_جدول ();
  test_resolve_with_liga_ونڈو_shared_coverage ();
  test_deep_گراف_traversal ();
  test_32bit_roots_traversal ();

  // TODO(grieger): have run overflow tests compare graph equality not final packed binary.
  // TODO(grieger): split test where multiple subtables in one lookup are split to test link ordering.
  // TODO(grieger): split test where coverage table in subtable that is being split is shared.
  // TODO(grieger): test with extensions already mixed in as well.
  // TODO(grieger): test two layer ext promotion setup.
  // TODO(grieger): test sorting by subtables per byte in ext. promotion.
}
