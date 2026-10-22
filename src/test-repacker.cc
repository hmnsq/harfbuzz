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
                    بینښه len,
                    hb_serialize_context_t* c)
{
  حرف* obj = c->allocate_اندازه<حرف> (len);
  hb_memcpy (obj, value, len);
}

ساكن فراغ پیل_object(ثابت حرف* نښان_2,
                         بینښه len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (نښان_2, len, c);
}

ساكن بینښه جمع_کړه_object(ثابت حرف* نښان_2,
                           بینښه len,
                           hb_serialize_context_t* c,
                           منطقي shared = خطأ)
{
  پیل_object (نښان_2, len, c);
  أرجع c->راوباسه_pack (shared);
}


ساكن فراغ جمع_کړه_دننه_واټن (بینښه id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* دننه_واټن = c->پیل_embed<OT::Offset16> ();
  c->extend_min (دننه_واټن);
  c->جمع_کړه_link (*دننه_واټن, id);
}

ساكن فراغ جمع_کړه_24_دننه_واټن (بینښه id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* دننه_واټن = c->پیل_embed<OT::Offset24> ();
  c->extend_min (دننه_واټن);
  c->جمع_کړه_link (*دننه_واټن, id);
}

ساكن فراغ جمع_کړه_wide_دننه_واټن (بینښه id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* دننه_واټن = c->پیل_embed<OT::Offset32> ();
  c->extend_min (دننه_واټن);
  c->جمع_کړه_link (*دننه_واټن, id);
}

ساكن فراغ جمع_کړه_gsubgpos_سرلیک (بینښه lookup_list,
                                 hb_serialize_context_t* c)
{
  حرف سرلیک[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  پیل_object (سرلیک, 8, c);
  جمع_کړه_دننه_واټن (lookup_list, c);
  c->راوباسه_pack (خطأ);
}

ساكن فراغ جمع_کړه_gsubgpos_سرلیک_1_2 (بینښه lookup_list,
                                     hb_serialize_context_t* c)
{
  حرف سرلیک[] = {
    0, 1, 0, 2, //نسخه
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  پیل_object (سرلیک, حجم (سرلیک), c);
  جمع_کړه_wide_دننه_واټن (lookup_list, c);
  c->راوباسه_pack (خطأ);
}

ساكن بینښه جمع_کړه_lookup_list (ثابت بینښه* lookups,
                                 حرف شمېر,
                                 hb_serialize_context_t* c)
{
  حرف lookup_شمېر[] = {0, شمېر};
  پیل_object  ((حرف *) &lookup_شمېر, 2, c);

  لكل (صحيح i = 0; i < شمېر; i++)
    جمع_کړه_دننه_واټن (lookups[i], c);

  أرجع c->راوباسه_pack (خطأ);
}

ساكن بینښه جمع_کړه_lookup_list_1_2 (ثابت بینښه* lookups,
                                     حرف شمېر,
                                     hb_serialize_context_t* c)
{
  حرف lookup_شمېر[] = {0, شمېر};
  پیل_object ((حرف *) &lookup_شمېر, 2, c);

  لكل (صحيح i = 0; i < شمېر; i++)
    جمع_کړه_wide_دننه_واټن (lookups[i], c);

  أرجع c->راوباسه_pack (خطأ);
}

ساكن فراغ پیل_lookup (int8_t type,
                          int8_t عدد_subtables,
                          hb_serialize_context_t* c)
{
  حرف lookup[] = {
    0, (حرف)type, // type
    0, (حرف)2, // flag
    0, (حرف)عدد_subtables, // num subtables
  };

  پیل_object (lookup, 6, c);
}

ساكن بینښه پای_lookup (hb_serialize_context_t* c)
{
  حرف filter[] = {0, 0};
  extend (filter, 2, c);
  أرجع c->راوباسه_pack (خطأ);
}

ساكن بینښه جمع_کړه_extension (بینښه ماشوم,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  حرف ext[] = {
    0, 1,
    0, (حرف) type,
  };

  پیل_object (ext, 4, c);
  جمع_کړه_wide_دننه_واټن (ماشوم, c);

  أرجع c->راوباسه_pack (خطأ);

}

// Adds coverage table fro [start, end]
ساكن بینښه جمع_کړه_coverage (بینښه پیل, بینښه پای_ټکی,
                              hb_serialize_context_t* c,
                              منطقي shared = خطأ)
{
  إذا (پای_ټکی - پیل == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //شمېر

      (uint8_t) ((پیل >> 8) & 0xFF),
      (uint8_t) (پیل & 0xFF), // glyph[0]
    };
    أرجع جمع_کړه_object ((حرف*) coverage, 6, c, shared);
  }

  إذا (پای_ټکی - پیل == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //شمېر

      (uint8_t) ((پیل >> 8) & 0xFF),
      (uint8_t) (پیل & 0xFF), // glyph[0]

      (uint8_t) ((پای_ټکی >> 8) & 0xFF),
      (uint8_t) (پای_ټکی & 0xFF), // glyph[1]
    };
    أرجع جمع_کړه_object ((حرف*) coverage, 8, c, shared);
  }

  uint8_t coverage[] = {
    0, 2, // format
    0, 1, // range count

    (uint8_t) ((پیل >> 8) & 0xFF),
    (uint8_t) (پیل & 0xFF), //پیل

    (uint8_t) ((پای_ټکی >> 8) & 0xFF),
    (uint8_t) (پای_ټکی & 0xFF), //پای_ټکی

    0, 0,
  };
  أرجع جمع_کړه_object ((حرف*) coverage, 10, c, shared);
}


کينډۍ<ډول_نوم It>
ساكن بینښه جمع_کړه_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  أرجع c->راوباسه_pack (خطأ);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
ساكن بینښه جمع_کړه_class_def (uint16_t پیل_glyph,
                               uint16_t پای_ټکی_glyph,
                               hb_serialize_context_t* c)
{
  بینښه شمېر = پای_ټکی_glyph - پیل_glyph;
  uint8_t سرلیک[] = {
    0, 1, // format

    (uint8_t) ((پیل_glyph >> 8) & 0xFF),
    (uint8_t) (پیل_glyph & 0xFF), // start_glyph

    (uint8_t) ((شمېر >> 8) & 0xFF),
    (uint8_t) (شمېر & 0xFF), //شمېر
  };

  پیل_object ((حرف*) سرلیک, 6, c);
  لكل (uint16_t i = 1; i <= شمېر; i++)
  {
    uint8_t class_ارزښت_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //شمېر
    };
    extend ((حرف*) class_ارزښت_2, 2, c);
  }

  أرجع c->راوباسه_pack (خطأ);
}

ساكن بینښه جمع_کړه_pair_pos_1 (بینښه* pair_sets,
                                حرف شمېر,
                                بینښه coverage,
                                hb_serialize_context_t* c)
{
  حرف format[] = {
    0, 1
  };

  پیل_object (format, 2, c);
  جمع_کړه_دننه_واټن (coverage, c);

  حرف ارزښت_format[] = {
    0, 0,
    0, 0,
    0, شمېر,
  };
  extend (ارزښت_format, 6, c);

  لكل (حرف i = 0; i < شمېر; i++)
    جمع_کړه_دننه_واټن (pair_sets[(بینښه) i], c);

  أرجع c->راوباسه_pack (خطأ);
}

ساكن فراغ جمع_کړه_liga_سېټ_سرلیک (حرف liga_شمېر,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_شمېر_بایټونه[] = {
    (uint8_t) (0xFF & (liga_شمېر >> 8)),
    (uint8_t) (0xFF & liga_شمېر),
  };

  پیل_object ((ثابت حرف*) liga_شمېر_بایټونه, 2, c);
}

ساكن فراغ جمع_کړه_liga_سرلیک (بینښه coverage, بینښه liga_سېټ_شمېر, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  پیل_object ((ثابت حرف*) format, 2, c);
  جمع_کړه_دننه_واټن(coverage, c);

  uint8_t liga_سېټ_شمېر_بایټونه[] = {
    (uint8_t) (0xFF & (liga_سېټ_شمېر >> 8)),
    (uint8_t) (0xFF & liga_سېټ_شمېر),
  };

  extend((ثابت حرف *) liga_سېټ_شمېر_بایټونه, 2, c);
}

ساكن بینښه جمع_کړه_pair_pos_2 (بینښه starting_class,
                                بینښه coverage,
                                بینښه class_def_1, uint16_t class_def_1_شمېر,
                                بینښه class_def_2, uint16_t class_def_2_شمېر,
                                بینښه* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  پیل_object ((حرف*) format, 2, c);
  جمع_کړه_دننه_واټن (coverage, c);

  بینښه عدد_ارزښتونه_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  إذا (device_tables) {
    format2 |= 0x20;
    عدد_ارزښتونه_3 += 1;
  }
  uint8_t ارزښت_format[] = {
    0, format1,
    0, format2,
  };

  extend ((حرف*) ارزښت_format, 4, c);

  جمع_کړه_دننه_واټن (class_def_1, c);
  جمع_کړه_دننه_واټن (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_شمېر >> 8) & 0xFF),
    (uint8_t) (class_def_1_شمېر & 0xFF),
    (uint8_t) ((class_def_2_شمېر >> 8) & 0xFF),
    (uint8_t) (class_def_2_شمېر & 0xFF),
  };
  extend ((حرف*) class_counts, 4, c);

  بینښه عدد_بایټونه_فی_ریکارډ = class_def_2_شمېر * عدد_ارزښتونه_3 * 2;
  uint8_t* ریکارډ = (uint8_t*) حافظه_صفر_ورکړه (1, عدد_بایټونه_فی_ریکارډ);
  صحيح device_شاخص = 0;
  لكل (uint16_t i = 0; i < class_def_1_شمېر; i++)
  {

    لكل (uint16_t j = 0; j < class_def_2_شمېر; j++)
    {
      لكل (صحيح k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((حرف*) value, 2, c);
      }

      إذا (device_tables) {
        جمع_کړه_دننه_واټن (device_tables[device_شاخص++], c);
      }
    }
  }
  حرر (ریکارډ);

  أرجع c->راوباسه_pack (خطأ);
}

ساكن بینښه جمع_کړه_mark_بنسټ_pos_1 (بینښه mark_coverage,
                                     بینښه بنسټ_coverage,
                                     بینښه mark_array,
                                     بینښه بنسټ_array,
                                     بینښه class_شمېر,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  پیل_object ((حرف*) format, 2, c);
  جمع_کړه_دننه_واټن (mark_coverage, c);
  جمع_کړه_دننه_واټن (بنسټ_coverage, c);

  uint8_t شمېر[] = {
    (uint8_t) ((class_شمېر >> 8) & 0xFF),
    (uint8_t) (class_شمېر & 0xFF),
  };
  extend ((حرف*) شمېر, 2, c);

  جمع_کړه_دننه_واټن (mark_array, c);
  جمع_کړه_دننه_واټن (بنسټ_array, c);

  أرجع c->راوباسه_pack (خطأ);
}

کينډۍ<صحيح mark_شمېر,
    صحيح class_شمېر,
    صحيح بنسټ_شمېر,
    صحيح جدول_شمېر_2>
بنية Mark_بنسټ_Pos_Buffers
{
  بینښه بنسټ_anchors[class_شمېر * بنسټ_شمېر];
  بینښه mark_anchors[mark_شمېر];
  uint8_t anchor_buffers[class_شمېر * بنسټ_شمېر + 100];
  uint8_t class_بفر[class_شمېر * 2];

  Mark_بنسټ_Pos_Buffers(hb_serialize_context_t* c)
  {
    لكل (بینښه i = 0; i < حجم(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    لكل (بینښه i = 0; i < class_شمېر * بنسټ_شمېر; i++)
    {
      بنسټ_anchors[i] = جمع_کړه_object ((حرف*) &anchor_buffers[i], 100, c);
      إذا (i < class_شمېر) {
        class_بفر[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_بفر[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    لكل (بینښه i = 0; i < mark_شمېر; i++)
    {
      mark_anchors[i] = جمع_کړه_object ((حرف*) &anchor_buffers[i], 4, c);
    }
  }

  بینښه create_mark_بنسټ_pos_1 (بینښه جدول_شاخص, hb_serialize_context_t* c)
  {
    بینښه class_فی_جدول = class_شمېر / جدول_شمېر_2;
    بینښه mark_فی_class = mark_شمېر / class_شمېر;
    بینښه پیل_class = class_فی_جدول * جدول_شاخص;
    بینښه پای_ټکی_class = class_فی_جدول * (جدول_شاخص + 1) - 1;

    // baseArray
    uint8_t بنسټ_شمېر_بفر[] = {
      (uint8_t) ((بنسټ_شمېر >> 8) & 0xFF),
      (uint8_t) (بنسټ_شمېر & 0xFF),

    };
    پیل_object ((حرف*) بنسټ_شمېر_بفر, 2, c);
    لكل (بینښه بنسټ = 0; بنسټ < بنسټ_شمېر; بنسټ++)
    {
      لكل (بینښه klass = پیل_class; klass <= پای_ټکی_class; klass++)
      {
        بینښه i = بنسټ * class_شمېر + klass;
        جمع_کړه_دننه_واټن (بنسټ_anchors[i], c);
      }
    }
    بینښه بنسټ_array = c->راوباسه_pack (خطأ);

    // markArray
    بینښه عدد_marks = class_فی_جدول * mark_فی_class;
    uint8_t mark_شمېر_بفر[] = {
      (uint8_t) ((عدد_marks >> 8) & 0xFF),
      (uint8_t) (عدد_marks & 0xFF),
    };
    پیل_object ((حرف*) mark_شمېر_بفر, 2, c);
    لكل (بینښه mark = 0; mark < mark_شمېر; mark++)
    {
      بینښه klass = mark % class_شمېر;
      إذا (klass < پیل_class || klass > پای_ټکی_class) استمر;
      klass -= پیل_class;

      extend ((حرف*) &class_بفر[2 * klass], 2, c);
      جمع_کړه_دننه_واټن (mark_anchors[mark], c);
    }
    بینښه mark_array = c->راوباسه_pack (خطأ);

    // markCoverage
    خپلکاری it =
        + hb_range ((hb_codepoint_t) mark_شمېر)
        | hb_filter ([&] (hb_codepoint_t mark) {
          بینښه klass = mark % class_شمېر;
          أرجع klass >= class_فی_جدول * جدول_شاخص &&
              klass < class_فی_جدول * (جدول_شاخص + 1);
        })
        ;
    بینښه mark_coverage = جمع_کړه_coverage (it, c);

    // baseCoverage
    بینښه بنسټ_coverage = جمع_کړه_coverage (10, 10 + بنسټ_شمېر - 1, c);

    أرجع جمع_کړه_mark_بنسټ_pos_1 (mark_coverage,
                                بنسټ_coverage,
                                mark_array,
                                بنسټ_array,
                                class_فی_جدول,
                                c);
  }
};

ساكن فراغ پرلپسې_resolve_ډکېدنه_test (ثابت حرف* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       بینښه عدد_iterations = 0,
                                       منطقي recalculate_extensions = خطأ,
                                       hb_نښان_t نښان_2 = HB_نښان ('G', 'S', 'U', 'B'),
                                       منطقي check_دوګونی_equivalence = خطأ)
{
  چاپ_کړه (">>> Testing overflowing resolution for %s\n",
          name);

  ګراف_t ګراف = ګراف_t::create (overflowing.object_ګراف ()).value ();

  ګراف_t expected_ګراف = ګراف_t::create (expected.object_ګراف ()).value ();
  إذا (ګراف::will_ډکېدنه (expected_ګراف).value ())
  {
    إذا (check_دوګونی_equivalence) {
      چاپ_کړه("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_دوګونی_equivalence);
    }
    hb_always_assert (expected_ګراف.ټاکل_spaces ().is_ok ());
    hb_always_assert (expected_ګراف.ترتیبول_تر_ټولو_لنډ_واټن ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.دننه_واټن_ډکېدنه ());
  خپلکاری r = hb_resolve_ګراف_overflows (نښان_2,
                                      عدد_iterations,
                                      recalculate_extensions,
                                      ګراف);
  إذا (r.is_err())
    چاپ_کړه("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", ګراف::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = ګراف::serialize (ګراف).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = ګراف::serialize (expected_ګراف).value ();
  hb_always_assert (out2);
  إذا (check_دوګونی_equivalence) {
    بینښه l1, l2;
    ثابت حرف* d1 = hb_blob_get_معلومات(out1, &l1);
    ثابت حرف* d2 = hb_blob_get_معلومات(out2, &l2);

    منطقي سمون = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    إذا (!سمون) {
      چاپ_کړه("## Result:\n");
      ګراف.print();
      چاپ_کړه("## Expected:\n");
      expected_ګراف.print();
      hb_always_assert(سمون);
    }
  }

  hb_blob_destroy (out1);
  hb_blob_destroy (out2);

  // Check the graphs are equivalent
  ګراف.normalize ();
  expected_ګراف.normalize ();
  إذا (!(ګراف == expected_ګراف)) {
    چاپ_کړه("## Expected:\n");
    expected_ګراف.print();
    چاپ_کړه("## Result:\n");
    ګراف.print();
  }
  hb_always_assert (ګراف == expected_ګراف);
}

ساكن فراغ جمع_کړه_virtual_دننه_واټن (بینښه id,
                                hb_serialize_context_t* c)
{
  c->جمع_کړه_virtual_link (id);
}

ساكن فراغ
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->پیل_serialize<حرف> ();

  بینښه obj_1 = جمع_کړه_object ("ghi", 3, c);
  بینښه obj_2 = جمع_کړه_object ("def", 3, c);

  پیل_object ("abc", 3, c);
  جمع_کړه_دننه_واټن (obj_2, c);
  جمع_کړه_دننه_واټن (obj_1, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_virtual (hb_serialize_context_t* c, منطقي with_ډکېدنه)
{
  std::string large_string(50000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_4, obj_5;
  إذا (with_ډکېدنه) {
    obj_5 = جمع_کړه_object("55555", 5, c);
    obj_4 = جمع_کړه_object(large_string.c_str(), 50000, c);
  } وإلا {
    obj_4 = جمع_کړه_object(large_string.c_str(), 50000, c);
    obj_5 = جمع_کړه_object("55555", 5, c);
  }

  پیل_object(large_string.c_str(), 20000, c);
  جمع_کړه_دننه_واټن(obj_5, c);
  بینښه obj_3 = c->راوباسه_pack(خطأ);

  پیل_object("2", 2, c);
  جمع_کړه_virtual_دننه_واټن(obj_5, c);
  بینښه obj_2 = c->راوباسه_pack(خطأ);

  // obj 1
  پیل_object("1", 1, c);
  جمع_کړه_دننه_واټن(obj_2, c);
  جمع_کړه_دننه_واټن(obj_3, c);
  جمع_کړه_دننه_واټن(obj_4, c);
  c->راوباسه_pack(خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_ډکېدنه (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_1 = جمع_کړه_object (large_string.c_str(), 10000, c);
  بینښه obj_2 = جمع_کړه_object (large_string.c_str(), 20000, c);
  بینښه obj_3 = جمع_کړه_object (large_string.c_str(), 50000, c);

  پیل_object ("abc", 3, c);
  جمع_کړه_دننه_واټن (obj_3, c);
  جمع_کړه_دننه_واټن (obj_2, c);
  جمع_کړه_دننه_واټن (obj_1, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_لومړیتوب_ډکېدنه (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_e = جمع_کړه_object ("e", 1, c);
  بینښه obj_d = جمع_کړه_object ("d", 1, c);

  پیل_object (large_string.c_str (), 50000, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str (), 20000, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_دننه_واټن (obj_b, c);
  جمع_کړه_دننه_واټن (obj_c, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_لومړیتوب_ډکېدنه_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_e = جمع_کړه_object ("e", 1, c);

  پیل_object (large_string.c_str (), 50000, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  بینښه obj_d = جمع_کړه_object ("d", 1, c);

  پیل_object (large_string.c_str (), 20000, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_دننه_واټن (obj_b, c);
  جمع_کړه_دننه_واټن (obj_c, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}


ساكن فراغ
populate_serializer_with_dedup_ډکېدنه (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_1 = جمع_کړه_object ("def", 3, c);

  پیل_object (large_string.c_str(), 60000, c);
  جمع_کړه_دننه_واټن (obj_1, c);
  بینښه obj_2 = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 10000, c);
  جمع_کړه_دننه_واټن (obj_2, c);
  جمع_کړه_دننه_واټن (obj_1, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_مضرب_dedup_ډکېدنه (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه ونې_پاڼه = جمع_کړه_object("def", 3, c);

  constexpr بینښه عدد_mid_nodes = 20;
  بینښه mid_nodes[عدد_mid_nodes];
  لكل (بینښه i = 0; i < عدد_mid_nodes; i++) {
    پیل_object(large_string.c_str(), 10000 + i, c);
    جمع_کړه_دننه_واټن(ونې_پاڼه, c);
    mid_nodes[i] = c->راوباسه_pack(خطأ);
  }

  پیل_object("abc", 3, c);
  لكل (بینښه i = 0; i < عدد_mid_nodes; i++) {
    جمع_کړه_wide_دننه_واټن(mid_nodes[i], c);
  }
  c->راوباسه_pack(خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_ډکېدنه (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_4 = جمع_کړه_object ("4", 1, c);

  پیل_object (large_string.c_str(), 60000, c);
  جمع_کړه_دننه_واټن (obj_4, c);
  بینښه obj_3 = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 10000, c);
  جمع_کړه_دننه_واټن (obj_4, c);
  بینښه obj_2 = c->راوباسه_pack (خطأ);

  پیل_object ("1", 1, c);
  جمع_کړه_wide_دننه_واټن (obj_3, c);
  جمع_کړه_دننه_واټن (obj_2, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_ډکېدنه_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_f = جمع_کړه_object ("f", 1, c);

  پیل_object ("e", 1, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object ("d", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 60000, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_h = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 60000, c);
  جمع_کړه_دننه_واټن (obj_c, c);
  جمع_کړه_دننه_واټن (obj_h, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 10000, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_g = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 11000, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_i = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_wide_دننه_واټن (obj_b, c);
  جمع_کړه_دننه_واټن (obj_g, c);
  جمع_کړه_دننه_واټن (obj_i, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_ډکېدنه_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();


  // space 1

  بینښه obj_f_اولي = جمع_کړه_object ("f", 1, c);

  پیل_object ("e", 1, c);
  جمع_کړه_دننه_واټن (obj_f_اولي, c);
  بینښه obj_e_اولي = c->راوباسه_pack (خطأ);

  پیل_object ("d", 1, c);
  جمع_کړه_دننه_واټن (obj_e_اولي, c);
  بینښه obj_d_اولي = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 60000, c);
  جمع_کړه_دننه_واټن (obj_d_اولي, c);
  بینښه obj_h = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_e_اولي, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 60000, c);
  جمع_کړه_دننه_واټن (obj_c, c);
  جمع_کړه_دننه_واټن (obj_h, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  // space 0

  بینښه obj_f = جمع_کړه_object ("f", 1, c);

  پیل_object ("e", 1, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);


  پیل_object ("d", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 11000, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_i = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 10000, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_g = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_wide_دننه_واټن (obj_b, c);
  جمع_کړه_دننه_واټن (obj_g, c);
  جمع_کړه_دننه_واټن (obj_i, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_ډکېدنه_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_d = جمع_کړه_object ("f", 1, c);
  بینښه obj_e = جمع_کړه_object ("f", 1, c);

  پیل_object (large_string.c_str(), 60000, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_b = c->راوباسه_pack ();

  پیل_object (large_string.c_str(), 60000, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack ();


  پیل_object ("a", 1, c);
  جمع_کړه_wide_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_c, c);
  c->راوباسه_pack ();

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_repack_وروستی (hb_serialize_context_t* c, منطقي with_ډکېدنه)
{
  std::string large_string(70000, 'c');
  c->پیل_serialize<حرف> ();
  c->push();

  // Obj E
  بینښه obj_e_1, obj_e_2;
  إذا (with_ډکېدنه) {
    obj_e_1 = جمع_کړه_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } وإلا {
    obj_e_2 = جمع_کړه_object("a", 1, c);
  }

  // Obj D
  c->push();
  جمع_کړه_دننه_واټن(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  بینښه obj_d = c->راوباسه_pack(خطأ);

  جمع_کړه_دننه_واټن(obj_d, c);
  hb_always_assert(c->وروستی_added_ماشوم_شاخص() == obj_d);

  إذا (!with_ډکېدنه) {
    obj_e_1 = جمع_کړه_object("a", 1, c);
  }

  // Obj C
  c->push();
  جمع_کړه_دننه_واټن(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  بینښه obj_c = c->راوباسه_pack(خطأ);

  جمع_کړه_دننه_واټن(obj_c, c);

  // Obj B
  بینښه obj_b = جمع_کړه_object("b", 1, c);
  جمع_کړه_دننه_واټن(obj_b, c);

  // Obj A
  c->repack_وروستی(obj_d);
  c->راوباسه_pack(خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_spaces (hb_serialize_context_t* c, منطقي with_ډکېدنه)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_i;

  إذا (with_ډکېدنه)
    obj_i = جمع_کړه_object ("i", 1, c);

  // Space 2
  بینښه obj_h = جمع_کړه_object ("h", 1, c);

  پیل_object (large_string.c_str(), 30000, c);
  جمع_کړه_دننه_واټن (obj_h, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  // Space 1
  إذا (!with_ډکېدنه)
    obj_i = جمع_کړه_object ("i", 1, c);

  پیل_object (large_string.c_str(), 30000, c);
  جمع_کړه_دننه_واټن (obj_i, c);
  بینښه obj_g = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 30000, c);
  جمع_کړه_دننه_واټن (obj_i, c);
  بینښه obj_f = c->راوباسه_pack (خطأ);

  پیل_object ("d", 1, c);
  جمع_کړه_دننه_واټن (obj_g, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_wide_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_c, c);
  جمع_کړه_wide_دننه_واټن (obj_d, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_g = جمع_کړه_object ("g", 1, c);
  بینښه obj_h = جمع_کړه_object ("h", 1, c);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_g, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_h, c);
  بینښه obj_f = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object ("d", 1, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  جمع_کړه_دننه_واټن (obj_h, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_c, c);
  جمع_کړه_wide_دننه_واټن (obj_d, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_g_اولي = جمع_کړه_object ("g", 1, c);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_g_اولي, c);
  بینښه obj_e_اولي = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_e_اولي, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  بینښه obj_h_اولي = جمع_کړه_object ("h", 1, c);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_h_اولي, c);
  بینښه obj_f = c->راوباسه_pack (خطأ);

  پیل_object ("d", 1, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  بینښه obj_g = جمع_کړه_object ("g", 1, c);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_g, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  بینښه obj_h = جمع_کړه_object ("h", 1, c);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  جمع_کړه_دننه_واټن (obj_h, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_c, c);
  جمع_کړه_wide_دننه_واټن (obj_d, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize ();
}

ساكن فراغ
populate_serializer_short_and_wide_subgraph_ریښه (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_e = جمع_کړه_object ("e", 1, c);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_c, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_c, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_c, c);
  جمع_کړه_wide_دننه_واټن (obj_d, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_short_and_wide_subgraph_ریښه_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_e_اولي = جمع_کړه_object ("e", 1, c);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_e_اولي, c);
  بینښه obj_c_اولي = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_c_اولي, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  بینښه obj_e = جمع_کړه_object ("e", 1, c);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);


  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_c, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_c_اولي, c);
  جمع_کړه_wide_دننه_واټن (obj_d, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_وېشل_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_f = جمع_کړه_object ("f", 1, c);

  پیل_object (large_string.c_str(), 40000, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 40000, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_wide_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_c, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_وېشل_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_f = جمع_کړه_object ("f", 1, c);

  پیل_object (large_string.c_str(), 40000, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str(), 40000, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_c, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_وېشل_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_f_اولي = جمع_کړه_object ("f", 1, c);

  پیل_object (large_string.c_str(), 40000, c);
  جمع_کړه_دننه_واټن (obj_f_اولي, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  بینښه obj_f = جمع_کړه_object ("f", 1, c);

  پیل_object (large_string.c_str(), 40000, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_wide_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_c, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_وېشل_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->پیل_serialize<حرف> ();

  // Space 2

  بینښه obj_f_double_اولي = جمع_کړه_object ("f", 1, c);

  پیل_object (large_string.c_str(), 40000, c);
  جمع_کړه_دننه_واټن (obj_f_double_اولي, c);
  بینښه obj_d_اولي = c->راوباسه_pack (خطأ);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_d_اولي, c);
  بینښه obj_b_اولي = c->راوباسه_pack (خطأ);

  // Space 1

  بینښه obj_f_اولي = جمع_کړه_object ("f", 1, c);

  پیل_object (large_string.c_str(), 40000, c);
  جمع_کړه_دننه_واټن (obj_f_اولي, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  // Space 0

  بینښه obj_f = جمع_کړه_object ("f", 1, c);

  پیل_object (large_string.c_str(), 40000, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  //ریښه
  پیل_object ("a", 1, c);
  جمع_کړه_دننه_واټن (obj_b, c);
  جمع_کړه_wide_دننه_واټن (obj_b_اولي, c);
  جمع_کړه_wide_دننه_واټن (obj_c, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->پیل_serialize<حرف> ();

  بینښه obj_5 = جمع_کړه_object ("mn", 2, c);

  بینښه obj_4 = جمع_کړه_object ("jkl", 3, c);

  پیل_object ("ghi", 3, c);
  جمع_کړه_دننه_واټن (obj_4, c);
  بینښه obj_3 = c->راوباسه_pack (خطأ);

  پیل_object ("def", 3, c);
  جمع_کړه_دننه_واټن (obj_3, c);
  بینښه obj_2 = c->راوباسه_pack (خطأ);

  پیل_object ("abc", 3, c);
  جمع_کړه_دننه_واټن (obj_2, c);
  جمع_کړه_دننه_واټن (obj_4, c);
  جمع_کړه_دننه_واټن (obj_5, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->پیل_serialize<حرف> ();

  بینښه obj_6 = جمع_کړه_object ("opqrst", 6, c);

  بینښه obj_5 = جمع_کړه_object ("mn", 2, c);

  پیل_object ("jkl", 3, c);
  جمع_کړه_دننه_واټن (obj_6, c);
  بینښه obj_4 = c->راوباسه_pack (خطأ);

  پیل_object ("ghi", 3, c);
  جمع_کړه_دننه_واټن (obj_4, c);
  بینښه obj_3 = c->راوباسه_pack (خطأ);

  پیل_object ("def", 3, c);
  جمع_کړه_دننه_واټن (obj_3, c);
  بینښه obj_2 = c->راوباسه_pack (خطأ);

  پیل_object ("abc", 3, c);
  جمع_کړه_دننه_واټن (obj_2, c);
  جمع_کړه_دننه_واټن (obj_4, c);
  جمع_کړه_دننه_واټن (obj_5, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->پیل_serialize<حرف> ();

  بینښه obj_d = جمع_کړه_object ("d", 1, c);

  پیل_object ("b", 1, c);
  جمع_کړه_دننه_واټن (obj_d, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("e", 1, c);
  جمع_کړه_virtual_دننه_واټن (obj_b, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  پیل_object ("c", 1, c);
  جمع_کړه_دننه_واټن (obj_e, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_دننه_واټن (obj_b, c);
  جمع_کړه_دننه_واټن (obj_c, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_24_and_32_بټ_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه obj_f = جمع_کړه_object ("f", 1, c);
  بینښه obj_g = جمع_کړه_object ("g", 1, c);
  بینښه obj_j = جمع_کړه_object ("j", 1, c);
  بینښه obj_k = جمع_کړه_object ("k", 1, c);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_f, c);
  بینښه obj_c = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_g, c);
  بینښه obj_d = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_j, c);
  بینښه obj_h = c->راوباسه_pack (خطأ);

  پیل_object (large_string.c_str (), 40000, c);
  جمع_کړه_دننه_واټن (obj_k, c);
  بینښه obj_i = c->راوباسه_pack (خطأ);

  پیل_object ("e", 1, c);
  جمع_کړه_wide_دننه_واټن (obj_h, c);
  جمع_کړه_wide_دننه_واټن (obj_i, c);
  بینښه obj_e = c->راوباسه_pack (خطأ);

  پیل_object ("b", 1, c);
  جمع_کړه_24_دننه_واټن (obj_c, c);
  جمع_کړه_24_دننه_واټن (obj_d, c);
  جمع_کړه_24_دننه_واټن (obj_e, c);
  بینښه obj_b = c->راوباسه_pack (خطأ);

  پیل_object ("a", 1, c);
  جمع_کړه_24_دننه_واټن (obj_b, c);
  c->راوباسه_pack (خطأ);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              صحيح عدد_extensions = 0,
                                              منطقي shared_subtables = خطأ,
                                              منطقي نسخه_1_2 = خطأ)
{
  constexpr صحيح عدد_lookups = 5;
  constexpr صحيح عدد_subtables = عدد_lookups * 2;
  بینښه صحيح lookups[عدد_lookups];
  بینښه صحيح subtables[عدد_subtables];
  بینښه صحيح extensions[عدد_subtables];

  std::string large_string(60000, 'a');
  c->پیل_serialize<حرف> ();


  لكل (صحيح i = عدد_subtables - 1; i >= 0; i--)
    subtables[i] = جمع_کړه_object(large_string.c_str (), 15000 + i, c);

  لكل (صحيح i = عدد_subtables - 1;
       i >= (عدد_lookups - عدد_extensions) * 2;
       i--)
  {
    extensions[i] = جمع_کړه_extension (subtables[i], 5, c);
  }

  لكل (صحيح i = عدد_lookups - 1; i >= 0; i--)
  {
    منطقي is_ext = (i >= (عدد_lookups - عدد_extensions));

    پیل_lookup (is_ext ? (حرف) 7 : (حرف) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    إذا (is_ext) {
      إذا (shared_subtables && i > 2) {
        جمع_کړه_دننه_واټن (extensions[i * 2 - 1], c);
      }
      جمع_کړه_دننه_واټن (extensions[i * 2], c);
      جمع_کړه_دننه_واټن (extensions[i * 2 + 1], c);
    } وإلا {
      إذا (shared_subtables && i > 2) {
        جمع_کړه_دننه_واټن (subtables[i * 2 - 1], c);
      }
      جمع_کړه_دننه_واټن (subtables[i * 2], c);
      جمع_کړه_دننه_واټن (subtables[i * 2 + 1], c);
    }

    lookups[i] = پای_lookup (c);
  }

  بینښه lookup_list = نسخه_1_2
                         ? جمع_کړه_lookup_list_1_2 (lookups, عدد_lookups, c)
                         : جمع_کړه_lookup_list (lookups, عدد_lookups, c);

  إذا (نسخه_1_2)
    جمع_کړه_gsubgpos_سرلیک_1_2 (lookup_list, c);
  وإلا
    جمع_کړه_gsubgpos_سرلیک (lookup_list, c);

  c->پای_ټکی_serialize();
}

کينډۍ<صحيح عدد_pair_pos_1, صحيح عدد_pair_سېټ>
ساكن فراغ
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           منطقي as_extension = خطأ)
{
  std::string large_string(60000, 'a');
  c->پیل_serialize<حرف> ();

  constexpr صحيح ټول_pair_سېټ = عدد_pair_pos_1 * عدد_pair_سېټ;
  بینښه pair_سېټ_2[ټول_pair_سېټ];
  بینښه coverage[عدد_pair_pos_1];
  بینښه pair_pos_1[عدد_pair_pos_1];

  لكل (صحيح i = عدد_pair_pos_1 - 1; i >= 0; i--)
  {
    لكل (صحيح j = (i + 1) * عدد_pair_سېټ - 1; j >= i * عدد_pair_سېټ; j--)
      pair_سېټ_2[j] = جمع_کړه_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = جمع_کړه_coverage (i * عدد_pair_سېټ,
                                (i + 1) * عدد_pair_سېټ - 1, c);

    pair_pos_1[i] = جمع_کړه_pair_pos_1 (&pair_سېټ_2[i * عدد_pair_سېټ],
                                    عدد_pair_سېټ,
                                    coverage[i],
                                    c);
  }

  بینښه pair_pos_2 = جمع_کړه_object (large_string.c_str(), 200, c);

  إذا (as_extension) {
    pair_pos_2 = جمع_کړه_extension (pair_pos_2, 2, c);
    لكل (صحيح i = عدد_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = جمع_کړه_extension (pair_pos_1[i], 2, c);
  }

  پیل_lookup (as_extension ? 9 : 2, 1 + عدد_pair_pos_1, c);

  لكل (صحيح i = 0; i < عدد_pair_pos_1; i++)
    جمع_کړه_دننه_واټن (pair_pos_1[i], c);
  جمع_کړه_دننه_واټن (pair_pos_2, c);

  بینښه lookup = پای_lookup (c);

  بینښه lookup_list = جمع_کړه_lookup_list (&lookup, 1, c);

  جمع_کړه_gsubgpos_سرلیک (lookup_list, c);

  c->پای_ټکی_serialize();
}

کينډۍ<صحيح عدد_pair_pos_2, صحيح عدد_class_1, صحيح عدد_class_2>
ساكن فراغ
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           منطقي as_extension = خطأ,
                                           منطقي with_device_tables = خطأ,
                                           منطقي extra_جدول = ریښتیا)
{
  std::string large_string(100000, 'a');
  c->پیل_serialize<حرف> ();

  بینښه coverage[عدد_pair_pos_2];
  بینښه class_def_1[عدد_pair_pos_2];
  بینښه class_def_2[عدد_pair_pos_2];
  بینښه pair_pos_2[عدد_pair_pos_2];

  بینښه* device_tables = (بینښه*) حافظه_صفر_ورکړه (عدد_pair_pos_2 * عدد_class_1 * عدد_class_2,
                                                حجم(بینښه));

  // Total glyphs = num_class_1 * num_pair_pos_2
  لكل (صحيح i = عدد_pair_pos_2 - 1; i >= 0; i--)
  {
    بینښه پیل_glyph = 5 + i * عدد_class_1;
    إذا (عدد_class_2 >= عدد_class_1)
    {
      class_def_2[i] = جمع_کړه_class_def (11,
                                      10 + عدد_class_2, c);
      class_def_1[i] = جمع_کړه_class_def (پیل_glyph + 1,
                                      پیل_glyph + عدد_class_1,
                                      c);
    } وإلا {
      class_def_1[i] = جمع_کړه_class_def (پیل_glyph + 1,
                                      پیل_glyph + عدد_class_1,
                                      c);
      class_def_2[i] = جمع_کړه_class_def (11,
                                      10 + عدد_class_2, c);
    }

    coverage[i] = جمع_کړه_coverage (پیل_glyph,
                                پیل_glyph + عدد_class_1 - 1,
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
        device_tables[j] = جمع_کړه_object ((حرف*) جدول, 2, c);
      }
    }

    pair_pos_2[i] = جمع_کړه_pair_pos_2 (1 + i * عدد_class_1,
                                    coverage[i],
                                    class_def_1[i], عدد_class_1,
                                    class_def_2[i], عدد_class_2,
                                    with_device_tables
                                    ? &device_tables[i * عدد_class_1 * عدد_class_2]
                                    : تش_اشاره,
                                    c);
  }


  بینښه pair_pos_1 = 0;
  إذا (extra_جدول) pair_pos_1 = جمع_کړه_object (large_string.c_str(), 100000, c);

  إذا (as_extension) {
    لكل (صحيح i = عدد_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = جمع_کړه_extension (pair_pos_2[i], 2, c);

    إذا (extra_جدول)
      pair_pos_1 = جمع_کړه_extension (pair_pos_1, 2, c);
  }

  پیل_lookup (as_extension ? 9 : 2, 1 + عدد_pair_pos_2, c);

  إذا (extra_جدول)
    جمع_کړه_دننه_واټن (pair_pos_1, c);

  لكل (صحيح i = 0; i < عدد_pair_pos_2; i++)
    جمع_کړه_دننه_واټن (pair_pos_2[i], c);

  بینښه lookup = پای_lookup (c);

  بینښه lookup_list = جمع_کړه_lookup_list (&lookup, 1, c);

  جمع_کړه_gsubgpos_سرلیک (lookup_list, c);

  c->پای_ټکی_serialize();

  حرر (device_tables);
}

کينډۍ<صحيح mark_شمېر,
    صحيح class_شمېر,
    صحيح بنسټ_شمېر,
    صحيح جدول_شمېر_2>
ساكن فراغ
populate_serializer_with_large_mark_بنسټ_pos_1 (hb_serialize_context_t* c)
{
  c->پیل_serialize<حرف> ();

  Mark_بنسټ_Pos_Buffers<mark_شمېر, class_شمېر, بنسټ_شمېر, جدول_شمېر_2> buffers (c);

  بینښه mark_بنسټ_pos[جدول_شمېر_2];
  لكل (بینښه i = 0; i < جدول_شمېر_2; i++)
    mark_بنسټ_pos[i] = buffers.create_mark_بنسټ_pos_1 (i, c);

  لكل (صحيح i = 0; i < جدول_شمېر_2; i++)
    mark_بنسټ_pos[i] = جمع_کړه_extension (mark_بنسټ_pos[i], 4, c);

  پیل_lookup (9, جدول_شمېر_2, c);

  لكل (صحيح i = 0; i < جدول_شمېر_2; i++)
    جمع_کړه_دننه_واټن (mark_بنسټ_pos[i], c);

  بینښه lookup = پای_lookup (c);

  بینښه lookup_list = جمع_کړه_lookup_list (&lookup, 1, c);

  جمع_کړه_gsubgpos_سرلیک (lookup_list, c);

  c->پای_ټکی_serialize();
}

کينډۍ<بینښه Lig_Subst_شمېر,
         بینښه Liga_سېټ_شمېر,
         بینښه Liga_فی_سېټ_شمېر,
         بینښه Liga_اندازه>
ساكن فراغ populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    منطقي sequential_liga_sets,
                                                    منطقي shared,
                                                    بینښه *liga_subst_id,
                                                    منطقي unique_lig_str = خطأ)
{
  حرف ch = 'a';

  بینښه liga[Liga_سېټ_شمېر * Liga_فی_سېټ_شمېر];
  بینښه liga_سېټ[Liga_سېټ_شمېر];
  لكل (بینښه l = 0; l < Lig_Subst_شمېر; l++) {
    بینښه coverage_پیل = 0;
    بینښه coverage_پای_ټکی = Liga_سېټ_شمېر - 1;
    إذا (sequential_liga_sets) {
      coverage_پیل = l * Liga_سېټ_شمېر;
      coverage_پای_ټکی = (l + 1) * Liga_سېټ_شمېر - 1;
    }
    بینښه coverage = جمع_کړه_coverage(coverage_پیل, coverage_پای_ټکی, c, shared);

    لكل (بینښه i = 0; i < Liga_سېټ_شمېر; i++) {
      لكل (بینښه j = 0; j < Liga_فی_سېټ_شمېر; j++)
      {
        std::string large_string(100000, ch);
        پیل_object (large_string.c_str(), Liga_اندازه, c);
        إذا (unique_lig_str)
          ch += 1;
        جمع_کړه_virtual_دننه_واټن(coverage, c);
        liga[i * Liga_فی_سېټ_شمېر + j] = c->راوباسه_pack (shared);
      }

      جمع_کړه_liga_سېټ_سرلیک(Liga_فی_سېټ_شمېر, c);
      جمع_کړه_virtual_دننه_واټن(coverage, c);
      لكل (بینښه j = 0; j < Liga_فی_سېټ_شمېر; j++)
      {
        جمع_کړه_دننه_واټن(liga[i * Liga_فی_سېټ_شمېر + j], c);
      }

      liga_سېټ[i] = c->راوباسه_pack(shared);
    }

    جمع_کړه_liga_سرلیک(coverage, Liga_سېټ_شمېر, c);
    لكل (بینښه i = 0; i < Liga_سېټ_شمېر; i++)
    {
      جمع_کړه_دننه_واټن(liga_سېټ[i], c);
    }

    liga_subst_id[l] = c->راوباسه_pack(shared);
  }

  لكل (بینښه l = 0; l < Lig_Subst_شمېر; l++) {
    liga_subst_id[l] = جمع_کړه_extension(liga_subst_id[l], 4, c);
  }
}

کينډۍ<بینښه liga_subst_شمېر,
         بینښه liga_سېټ_شمېر,
         بینښه liga_فی_سېټ_شمېر,
         بینښه liga_اندازه>
ساكن فراغ
populate_serializer_with_large_liga (hb_serialize_context_t* c, منطقي sequential_liga_sets)
{
  c->پیل_serialize<حرف> ();

  بینښه liga_subst[liga_subst_شمېر];
  populate_serializer_with_large_ligsubst<liga_subst_شمېر, liga_سېټ_شمېر, liga_فی_سېټ_شمېر, liga_اندازه>(c, sequential_liga_sets, خطأ, liga_subst);

  پیل_lookup (7, liga_subst_شمېر, c);
  لكل (بینښه l = 0; l < liga_subst_شمېر; l++) {
    جمع_کړه_دننه_واټن(liga_subst[l], c);
  }

  بینښه lookup = پای_lookup (c);

  بینښه lookup_list = جمع_کړه_lookup_list (&lookup, 1, c);

  جمع_کړه_gsubgpos_سرلیک (lookup_list, c);

  c->پای_ټکی_serialize();
}

ساكن فراغ
populate_serializer_with_large_liga_overlapping_clone_پایله (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->پیل_serialize<حرف> ();

  constexpr بینښه liga_اندازه = 30000;

  بینښه liga[6];
  بینښه liga_subst[3];
  بینښه liga_سېټ[2];

  // LigaSubst 3
  بینښه coverage = جمع_کړه_coverage(1, 1, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    پیل_object (large_string.c_str(), liga_اندازه, c);
    جمع_کړه_virtual_دننه_واټن(coverage, c);
    liga[i] = c->راوباسه_pack (خطأ);
  }

  جمع_کړه_liga_سېټ_سرلیک(2, c);
  جمع_کړه_virtual_دننه_واټن(coverage, c);
  لكل (بینښه i = 0; i < 2; i++)
    جمع_کړه_دننه_واټن(liga[i], c);
  liga_سېټ[0] = c->راوباسه_pack(خطأ);

  جمع_کړه_liga_سرلیک(coverage, 1, c);
  جمع_کړه_دننه_واټن(liga_سېټ[0], c);
  liga_subst[2] = c->راوباسه_pack(خطأ);

  // LigaSubst 2
  coverage = جمع_کړه_coverage(0, 1, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    پیل_object (large_string.c_str(), liga_اندازه, c);
    جمع_کړه_virtual_دننه_واټن(coverage, c);
    liga[i] = c->راوباسه_pack (خطأ);
  }

  جمع_کړه_liga_سېټ_سرلیک(1, c);
  جمع_کړه_virtual_دننه_واټن(coverage, c);
  جمع_کړه_دننه_واټن(liga[1], c);
  liga_سېټ[1] = c->راوباسه_pack(خطأ);

  جمع_کړه_liga_سېټ_سرلیک(1, c);
  جمع_کړه_virtual_دننه_واټن(coverage, c);
  جمع_کړه_دننه_واټن(liga[0], c);
  liga_سېټ[0] = c->راوباسه_pack(خطأ);

  جمع_کړه_liga_سرلیک(coverage, 2, c);
  جمع_کړه_دننه_واټن(liga_سېټ[0], c);
  جمع_کړه_دننه_واټن(liga_سېټ[1], c);
  liga_subst[1] = c->راوباسه_pack(خطأ);

  // LigaSubst 1
  coverage = جمع_کړه_coverage(0, 0, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    پیل_object (large_string.c_str(), liga_اندازه, c);
    جمع_کړه_virtual_دننه_واټن(coverage, c);
    liga[i] = c->راوباسه_pack (خطأ);
  }

  جمع_کړه_liga_سېټ_سرلیک(2, c);
  جمع_کړه_virtual_دننه_واټن(coverage, c);
  لكل (بینښه i = 0; i < 2; i++)
    جمع_کړه_دننه_واټن(liga[i], c);
  liga_سېټ[0] = c->راوباسه_pack(خطأ);

  جمع_کړه_liga_سرلیک(coverage, 1, c);
  جمع_کړه_دننه_واټن(liga_سېټ[0], c);
  liga_subst[0] = c->راوباسه_pack(خطأ);

  لكل (صحيح l = 2; l >= 0; l--) {
    liga_subst[l] = جمع_کړه_extension(liga_subst[l], 4, c);
  }

  پیل_lookup (7, 3, c);
  لكل (بینښه l = 0; l < 3; l++) {
    جمع_کړه_دننه_واټن(liga_subst[l], c);
  }

  بینښه lookup = پای_lookup (c);

  بینښه lookup_list = جمع_کړه_lookup_list (&lookup, 1, c);

  جمع_کړه_gsubgpos_سرلیک (lookup_list, c);

  c->پای_ټکی_serialize();
}

کينډۍ<بینښه liga_subst_شمېر,
         بینښه liga_سېټ_شمېر,
         بینښه liga_فی_سېټ_شمېر,
         بینښه liga_اندازه>
ساكن فراغ
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, منطقي sequential_liga_sets)
{
  c->پیل_serialize<حرف> ();

  بینښه lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  بینښه liga_subst_idx[liga_subst_شمېر];
  populate_serializer_with_large_ligsubst<liga_subst_شمېر, liga_سېټ_شمېر, liga_فی_سېټ_شمېر, liga_اندازه>(c, sequential_liga_sets, ریښتیا, liga_subst_idx, ریښتیا);

  پیل_lookup (7, liga_subst_شمېر, c);
  لكل (بینښه l = 0; l < liga_subst_شمېر; l++) {
    جمع_کړه_دننه_واټن(liga_subst_idx[l], c);
  }

  lookups[0] = پای_lookup (c);

  // Lookup with 2 LigSubst tables
  بینښه liga_subst[liga_subst_شمېر + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, خطأ, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_شمېر, liga_سېټ_شمېر, liga_فی_سېټ_شمېر, liga_اندازه>(c, sequential_liga_sets, ریښتیا, &liga_subst[1], ریښتیا);

  پیل_lookup (7, liga_subst_شمېر + 1, c);
  لكل (بینښه l = 0; l <= liga_subst_شمېر; l++) {
    جمع_کړه_دننه_واټن(liga_subst[l], c);
  }

  lookups[1] = پای_lookup (c);

  بینښه lookup_list = جمع_کړه_lookup_list (lookups, 2, c);
  جمع_کړه_gsubgpos_سرلیک (lookup_list, c);

  c->پای_ټکی_serialize();
}

کينډۍ<بینښه liga_subst_شمېر,
         بینښه liga_سېټ_شمېر,
         بینښه liga_فی_سېټ_شمېر,
         بینښه liga_اندازه>
ساكن فراغ
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->پیل_serialize<حرف> ();

  بینښه liga_subst[liga_subst_شمېر + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, ریښتیا, ریښتیا, liga_subst, ریښتیا);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_شمېر, liga_سېټ_شمېر, liga_فی_سېټ_شمېر, liga_اندازه>(c, ریښتیا, ریښتیا, &liga_subst[1], ریښتیا);

  پیل_lookup (7, liga_subst_شمېر + 1, c);
  لكل (بینښه l = 0; l <= liga_subst_شمېر; l++) {
    جمع_کړه_دننه_واټن(liga_subst[l], c);
  }

  بینښه lookup = پای_lookup (c);

  بینښه lookup_list = جمع_کړه_lookup_list (&lookup, 1, c);
  جمع_کړه_gsubgpos_سرلیک (lookup_list, c);

  c->پای_ټکی_serialize();
}

ساكن فراغ test_ترتیبول_تر_ټولو_لنډ ()
{
  size_t بفر_اندازه = 100;
  فراغ* بفر_a = حافظه_ورکړه (بفر_اندازه);
  فراغ* بفر_e = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t a (بفر_a, بفر_اندازه);
  hb_serialize_context_t e (بفر_e, بفر_اندازه);
  populate_serializer_complex_2 (&a);

  ګراف_t ګراف = ګراف_t::create (a.object_ګراف ()).value ();
  hb_always_assert (ګراف.ترتیبول_تر_ټولو_لنډ_واټن ().is_ok());
  ګراف.normalize();

  // Expected graph
  e.پیل_serialize();
  بینښه jkl = جمع_کړه_object ("jkl", 3, &e);

  پیل_object("ghi", 3, &e);
  جمع_کړه_دننه_واټن(jkl, &e);
  بینښه ghi = e.راوباسه_pack(خطأ);

  پیل_object("def", 3, &e);
  جمع_کړه_دننه_واټن(ghi, &e);
  بینښه def = e.راوباسه_pack(خطأ);

  بینښه mn = جمع_کړه_object("mn", 2, &e);

  پیل_object ("abc", 3, &e);
  جمع_کړه_دننه_واټن (def, &e);
  جمع_کړه_دننه_واټن (jkl, &e);
  جمع_کړه_دننه_واټن (mn, &e);
  e.راوباسه_pack (خطأ);
  e.پای_ټکی_serialize();

  ګراف_t expected = ګراف_t::create (e.object_ګراف ()).value ();
  expected.normalize();

  أكد(expected == ګراف);

  حرر (بفر_a);
  حرر (بفر_e);
}

ساكن فراغ test_duplicate_ونې_پاڼه ()
{
  size_t بفر_اندازه = 100;
  فراغ* بفر_a = حافظه_ورکړه (بفر_اندازه);
  فراغ* بفر_e = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t a (بفر_a, بفر_اندازه);
  hb_serialize_context_t e (بفر_e, بفر_اندازه);
  populate_serializer_complex_2 (&a);

  ګراف_t ګراف = ګراف_t::create (a.object_ګراف ()).value ();
  hb_always_assert (ګراف.duplicate (4, 1, خطأ).is_ok ());
  ګراف.normalize();

  e.پیل_serialize();
  بینښه mn = جمع_کړه_object("mn", 2, &e);
  بینښه jkl_2 = جمع_کړه_object("jkl", 3, &e);

  پیل_object("ghi", 3, &e);
  جمع_کړه_دننه_واټن(jkl_2, &e);
  بینښه ghi = e.راوباسه_pack(خطأ);

  پیل_object("def", 3, &e);
  جمع_کړه_دننه_واټن(ghi, &e);
  بینښه def = e.راوباسه_pack(خطأ);

  بینښه jkl_1 = جمع_کړه_object("jkl", 3, &e);

  پیل_object("abc", 3, &e);
  جمع_کړه_دننه_واټن(def, &e);
  جمع_کړه_دننه_واټن(jkl_1, &e);
  جمع_کړه_دننه_واټن(mn, &e);
  e.راوباسه_pack(خطأ);

  ګراف_t expected = ګراف_t::create (e.object_ګراف ()).value ();
  expected.normalize();

  أكد(expected == ګراف);

  حرر (بفر_a);
  حرر (بفر_e);
}

ساكن فراغ test_duplicate_interior ()
{
  size_t بفر_اندازه = 100;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_complex_3 (&c);

  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();
  hb_always_assert (ګراف.duplicate (3, 2, خطأ).is_ok ());

  hb_always_assert(مزي_پرتله (ګراف.object (6).سر_2, "jkl", 3) == 0);
  hb_always_assert(ګراف.object (6).real_links.اوږدتیا == 1);
  hb_always_assert(ګراف.object (6).real_links[0].objidx == 0);

  hb_always_assert(مزي_پرتله (ګراف.object (5).سر_2, "abc", 3) == 0);
  hb_always_assert(ګراف.object (5).real_links.اوږدتیا == 3);
  hb_always_assert(ګراف.object (5).real_links[0].objidx == 4);
  hb_always_assert(ګراف.object (5).real_links[1].objidx == 2);
  hb_always_assert(ګراف.object (5).real_links[2].objidx == 1);

  hb_always_assert(مزي_پرتله (ګراف.object (4).سر_2, "def", 3) == 0);
  hb_always_assert(ګراف.object (4).real_links.اوږدتیا == 1);
  hb_always_assert(ګراف.object (4).real_links[0].objidx == 3);

  hb_always_assert(مزي_پرتله (ګراف.object (3).سر_2, "ghi", 3) == 0);
  hb_always_assert(ګراف.object (3).real_links.اوږدتیا == 1);
  hb_always_assert(ګراف.object (3).real_links[0].objidx == 6);

  hb_always_assert(مزي_پرتله (ګراف.object (2).سر_2, "jkl", 3) == 0);
  hb_always_assert(ګراف.object (2).real_links.اوږدتیا == 1);
  hb_always_assert(ګراف.object (2).real_links[0].objidx == 0);

  hb_always_assert(مزي_پرتله (ګراف.object (1).سر_2, "mn", 2) == 0);
  hb_always_assert(ګراف.object (1).real_links.اوږدتیا == 0);

  hb_always_assert(مزي_پرتله (ګراف.object (0).سر_2, "opqrst", 6) == 0);
  hb_always_assert(ګراف.object (0).real_links.اوږدتیا == 0);

  حرر (بفر);
}

ساكن فراغ
test_serialize ()
{
  size_t بفر_اندازه = 100;
  فراغ* بفر_1 = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c1 (بفر_1, بفر_اندازه);
  populate_serializer_simple (&c1);
  hb_بایټونه_t expected = c1.copy_بایټونه ();

  ګراف_t ګراف = ګراف_t::create (c1.object_ګراف ()).value ();
  hb_blob_t* بهر = ګراف::serialize (ګراف).value ();
  حرر (بفر_1);

  hb_بایټونه_t actual = بهر->as_بایټونه ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (بهر);
}

ساكن فراغ test_will_ډکېدنه_1 ()
{
  size_t بفر_اندازه = 100;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);

  populate_serializer_complex_2 (&c);
  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_always_assert (!ګراف::will_ډکېدنه (ګراف, تش_اشاره).value ());

  حرر (بفر);
}

ساكن فراغ test_will_ډکېدنه_2 ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_ډکېدنه (&c);
  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_always_assert (ګراف::will_ډکېدنه (ګراف, تش_اشاره).value ());

  حرر (بفر);
}

ساكن فراغ test_will_ډکېدنه_3 ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_dedup_ډکېدنه (&c);
  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_always_assert (ګراف::will_ډکېدنه (ګراف, تش_اشاره).value ());

  حرر (بفر);
}

ساكن فراغ test_resolve_overflows_له_لارې_ترتیبول ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_ډکېدنه (&c);
  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_blob_t* بهر = hb_resolve_overflows (c.object_ګراف (), HB_نښان_NONE);
  hb_always_assert (بهر);
  hb_بایټونه_t پایله = بهر->as_بایټونه ();
  hb_always_assert (پایله.اوږدتیا == (80000 + 3 + 3 * 2));

  حرر (بفر);
  hb_blob_destroy (بهر);
}

ساكن فراغ test_resolve_overflows_له_لارې_duplication ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_dedup_ډکېدنه (&c);
  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_blob_t* بهر = hb_resolve_overflows (c.object_ګراف (), HB_نښان_NONE);
  hb_always_assert (بهر);
  hb_بایټونه_t پایله = بهر->as_بایټونه ();
  hb_always_assert (پایله.اوږدتیا == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  حرر (بفر);
  hb_blob_destroy (بهر);
}

ساكن فراغ test_resolve_overflows_له_لارې_مضرب_duplications ()
{
  size_t بفر_اندازه = 300000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_مضرب_dedup_ډکېدنه (&c);
  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_blob_t* بهر = hb_resolve_overflows (c.object_ګراف (), HB_نښان_NONE, 5);
  hb_always_assert (بهر);

  حرر (بفر);
  hb_blob_destroy (بهر);
}

ساكن فراغ test_resolve_overflows_له_لارې_space_assignment ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_spaces (&c, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_spaces (&e, خطأ);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_له_لارې_isolation ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_isolation_ډکېدنه (&c);
  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_always_assert (c.دننه_واټن_ډکېدنه ());
  hb_blob_t* بهر = hb_resolve_overflows (c.object_ګراف (), HB_نښان ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (بهر);
  hb_بایټونه_t پایله = بهر->as_بایټونه ();
  hb_always_assert (پایله.اوږدتیا == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  حرر (بفر);
  hb_blob_destroy (بهر);
}

ساكن فراغ test_resolve_overflows_له_لارې_isolation_with_recursive_duplication ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_isolation_ډکېدنه_complex (&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_isolation_ډکېدنه_complex_expected (&e);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_له_لارې_isolating_16bit_space ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_spaces_16bit_connection (&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_spaces_16bit_connection_expected (&e);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_له_لارې_isolating_16bit_space_2 ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_short_and_wide_subgraph_ریښه (&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_short_and_wide_subgraph_ریښه_expected (&e);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_له_لارې_isolation_spaces ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_isolation_ډکېدنه_spaces (&c);
  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_always_assert (c.دننه_واټن_ډکېدنه ());
  hb_blob_t* بهر = hb_resolve_overflows (c.object_ګراف (), HB_نښان ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (بهر);
  hb_بایټونه_t پایله = بهر->as_بایټونه ();

  بینښه expected_اوږدتیا = 3 + 2 * 60000; // objects
  expected_اوږدتیا += 2 * 4 + 2 * 2; // links
  hb_always_assert (پایله.اوږدتیا == expected_اوږدتیا);

  حرر (بفر);
  hb_blob_destroy (بهر);
}

ساكن فراغ test_resolve_mixed_overflows_له_لارې_isolation_spaces ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_24_and_32_بټ_offsets (&c);
  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_always_assert (c.دننه_واټن_ډکېدنه ());
  hb_blob_t* بهر = hb_resolve_overflows (c.object_ګراف (), HB_نښان ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (بهر);
  hb_بایټونه_t پایله = بهر->as_بایټونه ();

  بینښه expected_اوږدتیا =
      // Objects
      7 +
      4 * 40000;

  expected_اوږدتیا +=
      // Links
      2 * 4 +  // 32
      4 * 3 +  // 24
      4 * 2;   // 16

  hb_always_assert (پایله.اوږدتیا == expected_اوږدتیا);

  حرر (بفر);
  hb_blob_destroy (بهر);
}

ساكن فراغ test_resolve_with_extension_promotion ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_extension_promotion (&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_extension_promotion (&e, 3);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             ریښتیا);
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_shared_extension_promotion ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_extension_promotion (&c, 0, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_extension_promotion (&e, 3, ریښتیا);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             ریښتیا);
  حرر (بفر);
  حرر (expected_بفر);
}

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
ساكن فراغ test_resolve_with_extension_promotion_1_2 ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_extension_promotion (&c, 0, خطأ, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_extension_promotion (&e, 3, خطأ, ریښتیا);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             ریښتیا);
  حرر (بفر);
  حرر (expected_بفر);
}
#د_که_پای

ساكن فراغ test_resolve_with_repeated_extension_promotion ()
{
  size_t بفر_اندازه = 1000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);

  c.پیل_serialize<حرف> ();
  بینښه subtable = جمع_کړه_object ("subt", 4, &c);
  پیل_lookup (5, 2, &c);
  جمع_کړه_دننه_واټن (subtable, &c);
  جمع_کړه_دننه_واټن (subtable, &c);
  بینښه lookup = پای_lookup (&c);
  بینښه lookup_list = جمع_کړه_lookup_list (&lookup, 1, &c);
  جمع_کړه_gsubgpos_سرلیک (lookup_list, &c);
  c.پای_ټکی_serialize ();

  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();
  خپلکاری context = ګراف::gsubgpos_ګراف_context_t::create (HB_نښان ('G', 'S', 'U', 'B'),
                                                          ګراف).value ();
  hb_always_assert (context.lookups.get_population () == 1);
  بینښه lookup_شاخص = *context.lookups.کلیګانې ();
  ګراف::Lookup* lookup_جدول_2 = context.lookups.get (lookup_شاخص);
  hb_always_assert (lookup_جدول_2->make_extension (context, lookup_شاخص).is_ok ());

  ثابت خپلکاری& lookup_راس = ګراف.vertices_[lookup_شاخص];
  hb_always_assert (lookup_راس.obj ().real_links.اوږدتیا == 2);
  بینښه extension_شاخص = lookup_راس.obj ().real_links[0].objidx;
  hb_always_assert (lookup_راس.obj ().real_links[1].objidx == extension_شاخص);

  خپلکاری* extension = ګراف.vertices_[extension_شاخص].as<
      ګراف::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
  بینښه promoted_subtable = extension->get_subtable_شاخص (ګراف, extension_شاخص).value ();
  hb_always_assert (ګراف.vertices_[promoted_subtable].جدول_اندازه () == 4);
  hb_always_assert (ګراف.update_parents ().is_ok ());
  hb_always_assert (ګراف.vertices_[extension_شاخص].incoming_څنډې_2 () == 2);
  hb_always_assert (ګراف.vertices_[promoted_subtable].incoming_څنډې_2 () == 1);

  hb_blob_t* بهر = ګراف::serialize (ګراف).value ();
  hb_blob_destroy (بهر);
  حرر (بفر);
}

ساكن فراغ test_resolve_with_basic_pair_pos_1_وېشل ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, ریښتیا);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_extension_pair_pos_1_وېشل ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, ریښتیا);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_basic_pair_pos_2_وېشل ()
{
  size_t بفر_اندازه = 300000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, ریښتیا);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_close_منزل_حد_pair_pos_2_وېشل ()
{
  size_t بفر_اندازه = 300000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, ریښتیا, خطأ, خطأ);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, ریښتیا, خطأ, خطأ);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_pair_pos_2_وېشل_with_device_tables ()
{
  size_t بفر_اندازه = 300000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, خطأ, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, ریښتیا, ریښتیا);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_basic_mark_بنسټ_pos_1_وېشل ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_large_mark_بنسټ_pos_1 <40, 10, 110, 1>(&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_large_mark_بنسټ_pos_1 <40, 10, 110, 2>(&e);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_basic_liga_وېشل ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, خطأ);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, خطأ);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_وېشل_move ()
{
  size_t بفر_اندازه = 400000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, ریښتیا);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_وېشل_overlapping_clone ()
{
  size_t بفر_اندازه = 400000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_large_liga_overlapping_clone_پایله(&e);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_وېشل_shared_جدول ()
{
  size_t بفر_اندازه = 400000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, ریښتیا);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_وېشل_shared_coverage ()
{
  size_t بفر_اندازه = 400000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             ریښتیا,
                             HB_نښان('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_له_لارې_splitting_spaces ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_وېشل_spaces (&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_وېشل_spaces_expected (&e);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  حرر (بفر);
  حرر (expected_بفر);

}

ساكن فراغ test_resolve_overflows_له_لارې_splitting_spaces_2 ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_وېشل_spaces_2 (&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_وېشل_spaces_expected_2 (&e);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_له_لارې_لومړیتوب ()
{
  size_t بفر_اندازه = 160000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_لومړیتوب_ډکېدنه (&c);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_لومړیتوب_ډکېدنه_expected (&e);

  پرلپسې_resolve_ډکېدنه_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  حرر (بفر);
  حرر (expected_بفر);
}


ساكن فراغ test_virtual_link ()
{
  size_t بفر_اندازه = 100;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_virtual_link (&c);

  hb_blob_t* بهر = hb_resolve_overflows (c.object_ګراف (), HB_نښان_NONE);
  hb_always_assert (بهر);

  hb_بایټونه_t پایله = بهر->as_بایټونه ();
  hb_always_assert (پایله.اوږدتیا == 5 + 4 * 2);
  hb_always_assert (پایله[0]  == 'a');
  hb_always_assert (پایله[5]  == 'c');
  hb_always_assert (پایله[8]  == 'e');
  hb_always_assert (پایله[9]  == 'b');
  hb_always_assert (پایله[12] == 'd');

  حرر (بفر);
  hb_blob_destroy (بهر);
}

ساكن فراغ
test_shared_غوټه_with_virtual_links ()
{
  size_t بفر_اندازه = 100;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);

  c.پیل_serialize<حرف> ();

  بینښه obj_b = جمع_کړه_object ("b", 1, &c);
  بینښه obj_c = جمع_کړه_object ("c", 1, &c);

  پیل_object ("d", 1, &c);
  جمع_کړه_virtual_دننه_واټن (obj_b, &c);
  بینښه obj_d_1 = c.راوباسه_pack ();

  پیل_object ("d", 1, &c);
  جمع_کړه_virtual_دننه_واټن (obj_c, &c);
  بینښه obj_d_2 = c.راوباسه_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  پیل_object ("a", 1, &c);
  جمع_کړه_دننه_واټن (obj_b, &c);
  جمع_کړه_دننه_واټن (obj_c, &c);
  جمع_کړه_دننه_واټن (obj_d_1, &c);
  جمع_کړه_دننه_واټن (obj_d_2, &c);
  c.راوباسه_pack ();
  c.پای_ټکی_serialize ();

  hb_always_assert(c.object_ګراف() [obj_d_1]->virtual_links.اوږدتیا == 2);
  hb_always_assert(c.object_ګراف() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_ګراف() [obj_d_1]->virtual_links[1].objidx == obj_c);
  حرر(بفر);
}

ساكن فراغ
test_repack_وروستی ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_with_repack_وروستی (&c, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_with_repack_وروستی (&e, خطأ);

  پرلپسې_resolve_ډکېدنه_test ("test_repack_last",
                             c,
                             e,
                             20,
                             خطأ,
                             HB_نښان('a', 'b', 'c', 'd'),
                             ریښتیا);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ
test_dont_duplicate_virtual ()
{
  size_t بفر_اندازه = 200000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_virtual (&c, ریښتیا);

  فراغ* expected_بفر = حافظه_ورکړه (بفر_اندازه);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_اندازه);
  populate_serializer_virtual (&e, خطأ);

  پرلپسې_resolve_ډکېدنه_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             خطأ,
                             HB_نښان('a', 'b', 'c', 'd'),
                             ریښتیا);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ
test_deep_ګراف_traversal ()
{
  size_t بفر_اندازه = 1000000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);

  c.پیل_serialize ();
  بینښه prev = جمع_کړه_object ("leaf", 4, &c);
  لكل (بینښه i = 0; i < 2000; i++)
  {
    پیل_object ("node", 4, &c);
    جمع_کړه_دننه_واټن (prev, &c);
    prev = c.راوباسه_pack (خطأ);
  }
  c.پای_ټکی_serialize ();

  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  // Test find_subgraph (set)
  hb_سېټ_t لیدل_شوی_سېټ;
  hb_always_assert (ګراف.پیدا_کړه_subgraph (ګراف.ریښه_idx (), لیدل_شوی_سېټ).is_ok ());
  hb_always_assert (لیدل_شوی_سېټ.get_population () == 2001);

  // Test find_subgraph_size
  hb_سېټ_t اندازه_سېټ;
  size_t sz = ګراف.پیدا_کړه_subgraph_اندازه (ګراف.ریښه_idx (), اندازه_سېټ).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.سېټ (ګراف.ریښه_idx (), 1);
  hb_always_assert (ګراف.پیدا_کړه_subgraph (ګراف.ریښه_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (ګراف.ټاکل_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t شاخص_map;
  hb_always_assert (ګراف.duplicate_subgraph (ګراف.ریښه_idx (), شاخص_map).is_ok ());
  hb_always_assert (شاخص_map.get_population () == 2001);

  حرر (بفر);
}

ساكن فراغ
test_32bit_roots_traversal ()
{
  size_t بفر_اندازه = 1000;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);

  c.پیل_serialize ();
  بینښه leaf1 = جمع_کړه_object ("l1", 2, &c);
  بینښه leaf2 = جمع_کړه_object ("l2", 2, &c);

  پیل_object ("mid", 3, &c);
  جمع_کړه_wide_دننه_واټن (leaf1, &c);
  بینښه mid = c.راوباسه_pack (خطأ);

  پیل_object ("root", 4, &c);
  جمع_کړه_دننه_واټن (mid, &c);
  جمع_کړه_wide_دننه_واټن (leaf2, &c);
  c.راوباسه_pack (خطأ);
  c.پای_ټکی_serialize ();

  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  hb_سېټ_t roots;
  hb_always_assert (ګراف.پیدا_کړه_32bit_roots (ګراف.ریښه_idx (), roots).is_ok ());
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
  بفر_اندازه = 1000;
  بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c2 (بفر, بفر_اندازه);

  c2.پیل_serialize ();
  بینښه b = جمع_کړه_object ("b", 1, &c2);

  پیل_object ("a", 1, &c2);
  جمع_کړه_wide_دننه_واټن (b, &c2);
  بینښه a = c2.راوباسه_pack (خطأ);

  پیل_object ("r", 1, &c2);
  جمع_کړه_دننه_واټن (a, &c2);
  جمع_کړه_wide_دننه_واټن (a, &c2);
  c2.راوباسه_pack (خطأ);

  c2.پای_ټکی_serialize ();

  ګراف_t graph2 = ګراف_t::create (c2.object_ګراف ()).value ();

  hb_سېټ_t roots2;
  hb_always_assert (graph2.پیدا_کړه_32bit_roots (graph2.ریښه_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  حرر (بفر);
}

ساكن فراغ test_invalid_link_objidx ()
{
  size_t بفر_اندازه = 100;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_complex_2 (&c);

  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  // Poison a link to point to (unsigned) -1
  خپلکاری links = ګراف.vertices_[ګراف.ریښه_idx ()].real_links_writer();
  links->objidx = (بینښه) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!ګراف.ترتیبول_تر_ټولو_لنډ_واټن ().is_ok ());

  حرر (بفر);
}

ساكن فراغ test_invalid_link_جمع_کړه_and_move ()
{
  size_t بفر_اندازه = 100;
  فراغ* بفر = حافظه_ورکړه (بفر_اندازه);
  hb_serialize_context_t c (بفر, بفر_اندازه);
  populate_serializer_complex_2 (&c);

  ګراف_t ګراف = ګراف_t::create (c.object_ګراف ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!ګراف.جمع_کړه_link (&dummy, 0, (بینښه) -1).is_ok ());

  // move_child with non-existent offset
  خپلکاری res = ګراف.move_ماشوم (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  حرر (بفر);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

صحيح
اصلي (صحيح argc, حرف **argv)
{
  test_invalid_link_objidx ();
  test_invalid_link_جمع_کړه_and_move ();
  test_serialize ();
  test_ترتیبول_تر_ټولو_لنډ ();
  test_will_ډکېدنه_1 ();
  test_will_ډکېدنه_2 ();
  test_will_ډکېدنه_3 ();
  test_resolve_overflows_له_لارې_ترتیبول ();
  test_resolve_overflows_له_لارې_duplication ();
  test_resolve_overflows_له_لارې_مضرب_duplications ();
  test_resolve_overflows_له_لارې_لومړیتوب ();
  test_resolve_overflows_له_لارې_space_assignment ();
  test_resolve_overflows_له_لارې_isolation ();
  test_resolve_overflows_له_لارې_isolation_with_recursive_duplication ();
  test_resolve_overflows_له_لارې_isolation_spaces ();
  test_resolve_overflows_له_لارې_isolating_16bit_space ();
  test_resolve_overflows_له_لارې_isolating_16bit_space_2 ();
  test_resolve_overflows_له_لارې_splitting_spaces ();
  test_resolve_overflows_له_لارې_splitting_spaces_2 ();
  test_resolve_mixed_overflows_له_لارې_isolation_spaces ();
  test_duplicate_ونې_پاڼه ();
  test_duplicate_interior ();
  test_virtual_link ();
  test_repack_وروستی();
  test_shared_غوټه_with_virtual_links ();
  test_resolve_with_extension_promotion ();
  test_resolve_with_shared_extension_promotion ();
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  test_resolve_with_extension_promotion_1_2 ();
#د_که_پای
  test_resolve_with_repeated_extension_promotion ();
  test_resolve_with_basic_pair_pos_1_وېشل ();
  test_resolve_with_extension_pair_pos_1_وېشل ();
  test_resolve_with_basic_pair_pos_2_وېشل ();
  test_resolve_with_pair_pos_2_وېشل_with_device_tables ();
  test_resolve_with_close_منزل_حد_pair_pos_2_وېشل ();
  test_resolve_with_basic_mark_بنسټ_pos_1_وېشل ();
  test_resolve_with_basic_liga_وېشل ();
  test_resolve_with_liga_وېشل_move ();
  test_dont_duplicate_virtual ();
  test_resolve_with_liga_وېشل_overlapping_clone ();
  test_resolve_with_liga_وېشل_shared_جدول ();
  test_resolve_with_liga_وېشل_shared_coverage ();
  test_deep_ګراف_traversal ();
  test_32bit_roots_traversal ();

  // TODO(grieger): have run overflow tests compare graph equality not final packed binary.
  // TODO(grieger): split test where multiple subtables in one lookup are split to test link ordering.
  // TODO(grieger): split test where coverage table in subtable that is being split is shared.
  // TODO(grieger): test with extensions already mixed in as well.
  // TODO(grieger): test two layer ext promotion setup.
  // TODO(grieger): test sorting by subtables per byte in ext. promotion.
}
