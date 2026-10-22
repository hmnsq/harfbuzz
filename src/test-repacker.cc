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
                    بیعلامت len,
                    hb_serialize_context_t* c)
{
  حرف* obj = c->allocate_اندازه<حرف> (len);
  hb_memcpy (obj, value, len);
}

ساكن فراغ شروع_object(ثابت حرف* برچسب_2,
                         بیعلامت len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (برچسب_2, len, c);
}

ساكن بیعلامت جمع_کن_object(ثابت حرف* برچسب_2,
                           بیعلامت len,
                           hb_serialize_context_t* c,
                           منطقي shared = خطأ)
{
  شروع_object (برچسب_2, len, c);
  أرجع c->برداشت_pack (shared);
}


ساكن فراغ جمع_کن_جابه_جایی (بیعلامت id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* جابه_جایی = c->شروع_embed<OT::Offset16> ();
  c->extend_min (جابه_جایی);
  c->جمع_کن_link (*جابه_جایی, id);
}

ساكن فراغ جمع_کن_24_جابه_جایی (بیعلامت id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* جابه_جایی = c->شروع_embed<OT::Offset24> ();
  c->extend_min (جابه_جایی);
  c->جمع_کن_link (*جابه_جایی, id);
}

ساكن فراغ جمع_کن_wide_جابه_جایی (بیعلامت id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* جابه_جایی = c->شروع_embed<OT::Offset32> ();
  c->extend_min (جابه_جایی);
  c->جمع_کن_link (*جابه_جایی, id);
}

ساكن فراغ جمع_کن_gsubgpos_سرآیند (بیعلامت lookup_list,
                                 hb_serialize_context_t* c)
{
  حرف سرآیند[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  شروع_object (سرآیند, 8, c);
  جمع_کن_جابه_جایی (lookup_list, c);
  c->برداشت_pack (خطأ);
}

ساكن فراغ جمع_کن_gsubgpos_سرآیند_1_2 (بیعلامت lookup_list,
                                     hb_serialize_context_t* c)
{
  حرف سرآیند[] = {
    0, 1, 0, 2, //نسخه
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  شروع_object (سرآیند, حجم (سرآیند), c);
  جمع_کن_wide_جابه_جایی (lookup_list, c);
  c->برداشت_pack (خطأ);
}

ساكن بیعلامت جمع_کن_lookup_list (ثابت بیعلامت* lookups,
                                 حرف شمار,
                                 hb_serialize_context_t* c)
{
  حرف lookup_شمار[] = {0, شمار};
  شروع_object  ((حرف *) &lookup_شمار, 2, c);

  لكل (صحيح i = 0; i < شمار; i++)
    جمع_کن_جابه_جایی (lookups[i], c);

  أرجع c->برداشت_pack (خطأ);
}

ساكن بیعلامت جمع_کن_lookup_list_1_2 (ثابت بیعلامت* lookups,
                                     حرف شمار,
                                     hb_serialize_context_t* c)
{
  حرف lookup_شمار[] = {0, شمار};
  شروع_object ((حرف *) &lookup_شمار, 2, c);

  لكل (صحيح i = 0; i < شمار; i++)
    جمع_کن_wide_جابه_جایی (lookups[i], c);

  أرجع c->برداشت_pack (خطأ);
}

ساكن فراغ شروع_lookup (int8_t type,
                          int8_t رقم_subtables,
                          hb_serialize_context_t* c)
{
  حرف lookup[] = {
    0, (حرف)type, // type
    0, (حرف)2, // flag
    0, (حرف)رقم_subtables, // num subtables
  };

  شروع_object (lookup, 6, c);
}

ساكن بیعلامت پایان_lookup (hb_serialize_context_t* c)
{
  حرف filter[] = {0, 0};
  extend (filter, 2, c);
  أرجع c->برداشت_pack (خطأ);
}

ساكن بیعلامت جمع_کن_extension (بیعلامت فرزند,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  حرف ext[] = {
    0, 1,
    0, (حرف) type,
  };

  شروع_object (ext, 4, c);
  جمع_کن_wide_جابه_جایی (فرزند, c);

  أرجع c->برداشت_pack (خطأ);

}

// Adds coverage table fro [start, end]
ساكن بیعلامت جمع_کن_coverage (بیعلامت شروع, بیعلامت انتها,
                              hb_serialize_context_t* c,
                              منطقي shared = خطأ)
{
  إذا (انتها - شروع == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //شمار

      (uint8_t) ((شروع >> 8) & 0xFF),
      (uint8_t) (شروع & 0xFF), // glyph[0]
    };
    أرجع جمع_کن_object ((حرف*) coverage, 6, c, shared);
  }

  إذا (انتها - شروع == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //شمار

      (uint8_t) ((شروع >> 8) & 0xFF),
      (uint8_t) (شروع & 0xFF), // glyph[0]

      (uint8_t) ((انتها >> 8) & 0xFF),
      (uint8_t) (انتها & 0xFF), // glyph[1]
    };
    أرجع جمع_کن_object ((حرف*) coverage, 8, c, shared);
  }

  uint8_t coverage[] = {
    0, 2, // format
    0, 1, // range count

    (uint8_t) ((شروع >> 8) & 0xFF),
    (uint8_t) (شروع & 0xFF), //شروع

    (uint8_t) ((انتها >> 8) & 0xFF),
    (uint8_t) (انتها & 0xFF), //انتها

    0, 0,
  };
  أرجع جمع_کن_object ((حرف*) coverage, 10, c, shared);
}


قالب<نام_نوع It>
ساكن بیعلامت جمع_کن_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  أرجع c->برداشت_pack (خطأ);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
ساكن بیعلامت جمع_کن_class_def (uint16_t شروع_glyph,
                               uint16_t انتها_glyph,
                               hb_serialize_context_t* c)
{
  بیعلامت شمار = انتها_glyph - شروع_glyph;
  uint8_t سرآیند[] = {
    0, 1, // format

    (uint8_t) ((شروع_glyph >> 8) & 0xFF),
    (uint8_t) (شروع_glyph & 0xFF), // start_glyph

    (uint8_t) ((شمار >> 8) & 0xFF),
    (uint8_t) (شمار & 0xFF), //شمار
  };

  شروع_object ((حرف*) سرآیند, 6, c);
  لكل (uint16_t i = 1; i <= شمار; i++)
  {
    uint8_t class_مقدار_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //شمار
    };
    extend ((حرف*) class_مقدار_2, 2, c);
  }

  أرجع c->برداشت_pack (خطأ);
}

ساكن بیعلامت جمع_کن_pair_pos_1 (بیعلامت* pair_sets,
                                حرف شمار,
                                بیعلامت coverage,
                                hb_serialize_context_t* c)
{
  حرف format[] = {
    0, 1
  };

  شروع_object (format, 2, c);
  جمع_کن_جابه_جایی (coverage, c);

  حرف مقدار_format[] = {
    0, 0,
    0, 0,
    0, شمار,
  };
  extend (مقدار_format, 6, c);

  لكل (حرف i = 0; i < شمار; i++)
    جمع_کن_جابه_جایی (pair_sets[(بیعلامت) i], c);

  أرجع c->برداشت_pack (خطأ);
}

ساكن فراغ جمع_کن_liga_مجموعه_سرآیند (حرف liga_شمار,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_شمار_بایتها[] = {
    (uint8_t) (0xFF & (liga_شمار >> 8)),
    (uint8_t) (0xFF & liga_شمار),
  };

  شروع_object ((ثابت حرف*) liga_شمار_بایتها, 2, c);
}

ساكن فراغ جمع_کن_liga_سرآیند (بیعلامت coverage, بیعلامت liga_مجموعه_شمار, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  شروع_object ((ثابت حرف*) format, 2, c);
  جمع_کن_جابه_جایی(coverage, c);

  uint8_t liga_مجموعه_شمار_بایتها[] = {
    (uint8_t) (0xFF & (liga_مجموعه_شمار >> 8)),
    (uint8_t) (0xFF & liga_مجموعه_شمار),
  };

  extend((ثابت حرف *) liga_مجموعه_شمار_بایتها, 2, c);
}

ساكن بیعلامت جمع_کن_pair_pos_2 (بیعلامت starting_class,
                                بیعلامت coverage,
                                بیعلامت class_def_1, uint16_t class_def_1_شمار,
                                بیعلامت class_def_2, uint16_t class_def_2_شمار,
                                بیعلامت* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  شروع_object ((حرف*) format, 2, c);
  جمع_کن_جابه_جایی (coverage, c);

  بیعلامت رقم_مقادیر_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  إذا (device_tables) {
    format2 |= 0x20;
    رقم_مقادیر_3 += 1;
  }
  uint8_t مقدار_format[] = {
    0, format1,
    0, format2,
  };

  extend ((حرف*) مقدار_format, 4, c);

  جمع_کن_جابه_جایی (class_def_1, c);
  جمع_کن_جابه_جایی (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_شمار >> 8) & 0xFF),
    (uint8_t) (class_def_1_شمار & 0xFF),
    (uint8_t) ((class_def_2_شمار >> 8) & 0xFF),
    (uint8_t) (class_def_2_شمار & 0xFF),
  };
  extend ((حرف*) class_counts, 4, c);

  بیعلامت رقم_بایتها_در_هر_رکورد = class_def_2_شمار * رقم_مقادیر_3 * 2;
  uint8_t* رکورد = (uint8_t*) حافظه_صفر_بده (1, رقم_بایتها_در_هر_رکورد);
  صحيح device_شاخص = 0;
  لكل (uint16_t i = 0; i < class_def_1_شمار; i++)
  {

    لكل (uint16_t j = 0; j < class_def_2_شمار; j++)
    {
      لكل (صحيح k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((حرف*) value, 2, c);
      }

      إذا (device_tables) {
        جمع_کن_جابه_جایی (device_tables[device_شاخص++], c);
      }
    }
  }
  حرر (رکورد);

  أرجع c->برداشت_pack (خطأ);
}

ساكن بیعلامت جمع_کن_mark_پایه_pos_1 (بیعلامت mark_coverage,
                                     بیعلامت پایه_coverage,
                                     بیعلامت mark_array,
                                     بیعلامت پایه_array,
                                     بیعلامت class_شمار,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  شروع_object ((حرف*) format, 2, c);
  جمع_کن_جابه_جایی (mark_coverage, c);
  جمع_کن_جابه_جایی (پایه_coverage, c);

  uint8_t شمار[] = {
    (uint8_t) ((class_شمار >> 8) & 0xFF),
    (uint8_t) (class_شمار & 0xFF),
  };
  extend ((حرف*) شمار, 2, c);

  جمع_کن_جابه_جایی (mark_array, c);
  جمع_کن_جابه_جایی (پایه_array, c);

  أرجع c->برداشت_pack (خطأ);
}

قالب<صحيح mark_شمار,
    صحيح class_شمار,
    صحيح پایه_شمار,
    صحيح جدول_شمار_2>
بنية Mark_پایه_Pos_Buffers
{
  بیعلامت پایه_anchors[class_شمار * پایه_شمار];
  بیعلامت mark_anchors[mark_شمار];
  uint8_t anchor_buffers[class_شمار * پایه_شمار + 100];
  uint8_t class_میانگیر[class_شمار * 2];

  Mark_پایه_Pos_Buffers(hb_serialize_context_t* c)
  {
    لكل (بیعلامت i = 0; i < حجم(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    لكل (بیعلامت i = 0; i < class_شمار * پایه_شمار; i++)
    {
      پایه_anchors[i] = جمع_کن_object ((حرف*) &anchor_buffers[i], 100, c);
      إذا (i < class_شمار) {
        class_میانگیر[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_میانگیر[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    لكل (بیعلامت i = 0; i < mark_شمار; i++)
    {
      mark_anchors[i] = جمع_کن_object ((حرف*) &anchor_buffers[i], 4, c);
    }
  }

  بیعلامت create_mark_پایه_pos_1 (بیعلامت جدول_شاخص, hb_serialize_context_t* c)
  {
    بیعلامت class_در_هر_جدول = class_شمار / جدول_شمار_2;
    بیعلامت mark_در_هر_class = mark_شمار / class_شمار;
    بیعلامت شروع_class = class_در_هر_جدول * جدول_شاخص;
    بیعلامت انتها_class = class_در_هر_جدول * (جدول_شاخص + 1) - 1;

    // baseArray
    uint8_t پایه_شمار_میانگیر[] = {
      (uint8_t) ((پایه_شمار >> 8) & 0xFF),
      (uint8_t) (پایه_شمار & 0xFF),

    };
    شروع_object ((حرف*) پایه_شمار_میانگیر, 2, c);
    لكل (بیعلامت پایه = 0; پایه < پایه_شمار; پایه++)
    {
      لكل (بیعلامت klass = شروع_class; klass <= انتها_class; klass++)
      {
        بیعلامت i = پایه * class_شمار + klass;
        جمع_کن_جابه_جایی (پایه_anchors[i], c);
      }
    }
    بیعلامت پایه_array = c->برداشت_pack (خطأ);

    // markArray
    بیعلامت رقم_marks = class_در_هر_جدول * mark_در_هر_class;
    uint8_t mark_شمار_میانگیر[] = {
      (uint8_t) ((رقم_marks >> 8) & 0xFF),
      (uint8_t) (رقم_marks & 0xFF),
    };
    شروع_object ((حرف*) mark_شمار_میانگیر, 2, c);
    لكل (بیعلامت mark = 0; mark < mark_شمار; mark++)
    {
      بیعلامت klass = mark % class_شمار;
      إذا (klass < شروع_class || klass > انتها_class) استمر;
      klass -= شروع_class;

      extend ((حرف*) &class_میانگیر[2 * klass], 2, c);
      جمع_کن_جابه_جایی (mark_anchors[mark], c);
    }
    بیعلامت mark_array = c->برداشت_pack (خطأ);

    // markCoverage
    خودکار it =
        + hb_range ((hb_codepoint_t) mark_شمار)
        | hb_filter ([&] (hb_codepoint_t mark) {
          بیعلامت klass = mark % class_شمار;
          أرجع klass >= class_در_هر_جدول * جدول_شاخص &&
              klass < class_در_هر_جدول * (جدول_شاخص + 1);
        })
        ;
    بیعلامت mark_coverage = جمع_کن_coverage (it, c);

    // baseCoverage
    بیعلامت پایه_coverage = جمع_کن_coverage (10, 10 + پایه_شمار - 1, c);

    أرجع جمع_کن_mark_پایه_pos_1 (mark_coverage,
                                پایه_coverage,
                                mark_array,
                                پایه_array,
                                class_در_هر_جدول,
                                c);
  }
};

ساكن فراغ پیاپی_resolve_سرریز_test (ثابت حرف* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       بیعلامت رقم_iterations = 0,
                                       منطقي recalculate_extensions = خطأ,
                                       hb_برچسب_t برچسب_2 = HB_برچسب ('G', 'S', 'U', 'B'),
                                       منطقي check_دودویی_equivalence = خطأ)
{
  چاپ_کن (">>> Testing overflowing resolution for %s\n",
          name);

  گراف_t گراف = گراف_t::create (overflowing.object_گراف ()).value ();

  گراف_t expected_گراف = گراف_t::create (expected.object_گراف ()).value ();
  إذا (گراف::will_سرریز (expected_گراف).value ())
  {
    إذا (check_دودویی_equivalence) {
      چاپ_کن("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_دودویی_equivalence);
    }
    hb_always_assert (expected_گراف.تخصیص_بده_spaces ().is_ok ());
    hb_always_assert (expected_گراف.مرتب_سازی_کوتاهترین_فاصله ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.جابه_جایی_سرریز ());
  خودکار r = hb_resolve_گراف_overflows (برچسب_2,
                                      رقم_iterations,
                                      recalculate_extensions,
                                      گراف);
  إذا (r.is_err())
    چاپ_کن("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", گراف::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = گراف::serialize (گراف).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = گراف::serialize (expected_گراف).value ();
  hb_always_assert (out2);
  إذا (check_دودویی_equivalence) {
    بیعلامت l1, l2;
    ثابت حرف* d1 = hb_blob_get_داده(out1, &l1);
    ثابت حرف* d2 = hb_blob_get_داده(out2, &l2);

    منطقي تطابق = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    إذا (!تطابق) {
      چاپ_کن("## Result:\n");
      گراف.print();
      چاپ_کن("## Expected:\n");
      expected_گراف.print();
      hb_always_assert(تطابق);
    }
  }

  hb_blob_destroy (out1);
  hb_blob_destroy (out2);

  // Check the graphs are equivalent
  گراف.normalize ();
  expected_گراف.normalize ();
  إذا (!(گراف == expected_گراف)) {
    چاپ_کن("## Expected:\n");
    expected_گراف.print();
    چاپ_کن("## Result:\n");
    گراف.print();
  }
  hb_always_assert (گراف == expected_گراف);
}

ساكن فراغ جمع_کن_virtual_جابه_جایی (بیعلامت id,
                                hb_serialize_context_t* c)
{
  c->جمع_کن_virtual_link (id);
}

ساكن فراغ
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بیعلامت obj_1 = جمع_کن_object ("ghi", 3, c);
  بیعلامت obj_2 = جمع_کن_object ("def", 3, c);

  شروع_object ("abc", 3, c);
  جمع_کن_جابه_جایی (obj_2, c);
  جمع_کن_جابه_جایی (obj_1, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_virtual (hb_serialize_context_t* c, منطقي with_سرریز)
{
  std::string large_string(50000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_4, obj_5;
  إذا (with_سرریز) {
    obj_5 = جمع_کن_object("55555", 5, c);
    obj_4 = جمع_کن_object(large_string.c_str(), 50000, c);
  } وإلا {
    obj_4 = جمع_کن_object(large_string.c_str(), 50000, c);
    obj_5 = جمع_کن_object("55555", 5, c);
  }

  شروع_object(large_string.c_str(), 20000, c);
  جمع_کن_جابه_جایی(obj_5, c);
  بیعلامت obj_3 = c->برداشت_pack(خطأ);

  شروع_object("2", 2, c);
  جمع_کن_virtual_جابه_جایی(obj_5, c);
  بیعلامت obj_2 = c->برداشت_pack(خطأ);

  // obj 1
  شروع_object("1", 1, c);
  جمع_کن_جابه_جایی(obj_2, c);
  جمع_کن_جابه_جایی(obj_3, c);
  جمع_کن_جابه_جایی(obj_4, c);
  c->برداشت_pack(خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_سرریز (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_1 = جمع_کن_object (large_string.c_str(), 10000, c);
  بیعلامت obj_2 = جمع_کن_object (large_string.c_str(), 20000, c);
  بیعلامت obj_3 = جمع_کن_object (large_string.c_str(), 50000, c);

  شروع_object ("abc", 3, c);
  جمع_کن_جابه_جایی (obj_3, c);
  جمع_کن_جابه_جایی (obj_2, c);
  جمع_کن_جابه_جایی (obj_1, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_اولویت_سرریز (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_e = جمع_کن_object ("e", 1, c);
  بیعلامت obj_d = جمع_کن_object ("d", 1, c);

  شروع_object (large_string.c_str (), 50000, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str (), 20000, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_جابه_جایی (obj_b, c);
  جمع_کن_جابه_جایی (obj_c, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_اولویت_سرریز_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_e = جمع_کن_object ("e", 1, c);

  شروع_object (large_string.c_str (), 50000, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  بیعلامت obj_d = جمع_کن_object ("d", 1, c);

  شروع_object (large_string.c_str (), 20000, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_جابه_جایی (obj_b, c);
  جمع_کن_جابه_جایی (obj_c, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}


ساكن فراغ
populate_serializer_with_dedup_سرریز (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_1 = جمع_کن_object ("def", 3, c);

  شروع_object (large_string.c_str(), 60000, c);
  جمع_کن_جابه_جایی (obj_1, c);
  بیعلامت obj_2 = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 10000, c);
  جمع_کن_جابه_جایی (obj_2, c);
  جمع_کن_جابه_جایی (obj_1, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_مضرب_dedup_سرریز (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت برگ = جمع_کن_object("def", 3, c);

  constexpr بیعلامت رقم_mid_nodes = 20;
  بیعلامت mid_nodes[رقم_mid_nodes];
  لكل (بیعلامت i = 0; i < رقم_mid_nodes; i++) {
    شروع_object(large_string.c_str(), 10000 + i, c);
    جمع_کن_جابه_جایی(برگ, c);
    mid_nodes[i] = c->برداشت_pack(خطأ);
  }

  شروع_object("abc", 3, c);
  لكل (بیعلامت i = 0; i < رقم_mid_nodes; i++) {
    جمع_کن_wide_جابه_جایی(mid_nodes[i], c);
  }
  c->برداشت_pack(خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_سرریز (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_4 = جمع_کن_object ("4", 1, c);

  شروع_object (large_string.c_str(), 60000, c);
  جمع_کن_جابه_جایی (obj_4, c);
  بیعلامت obj_3 = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 10000, c);
  جمع_کن_جابه_جایی (obj_4, c);
  بیعلامت obj_2 = c->برداشت_pack (خطأ);

  شروع_object ("1", 1, c);
  جمع_کن_wide_جابه_جایی (obj_3, c);
  جمع_کن_جابه_جایی (obj_2, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_سرریز_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_f = جمع_کن_object ("f", 1, c);

  شروع_object ("e", 1, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object ("d", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 60000, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_h = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 60000, c);
  جمع_کن_جابه_جایی (obj_c, c);
  جمع_کن_جابه_جایی (obj_h, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 10000, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_g = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 11000, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_i = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_wide_جابه_جایی (obj_b, c);
  جمع_کن_جابه_جایی (obj_g, c);
  جمع_کن_جابه_جایی (obj_i, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_سرریز_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();


  // space 1

  بیعلامت obj_f_اول = جمع_کن_object ("f", 1, c);

  شروع_object ("e", 1, c);
  جمع_کن_جابه_جایی (obj_f_اول, c);
  بیعلامت obj_e_اول = c->برداشت_pack (خطأ);

  شروع_object ("d", 1, c);
  جمع_کن_جابه_جایی (obj_e_اول, c);
  بیعلامت obj_d_اول = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 60000, c);
  جمع_کن_جابه_جایی (obj_d_اول, c);
  بیعلامت obj_h = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_e_اول, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 60000, c);
  جمع_کن_جابه_جایی (obj_c, c);
  جمع_کن_جابه_جایی (obj_h, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  // space 0

  بیعلامت obj_f = جمع_کن_object ("f", 1, c);

  شروع_object ("e", 1, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);


  شروع_object ("d", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 11000, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_i = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 10000, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_g = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_wide_جابه_جایی (obj_b, c);
  جمع_کن_جابه_جایی (obj_g, c);
  جمع_کن_جابه_جایی (obj_i, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_سرریز_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_d = جمع_کن_object ("f", 1, c);
  بیعلامت obj_e = جمع_کن_object ("f", 1, c);

  شروع_object (large_string.c_str(), 60000, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_b = c->برداشت_pack ();

  شروع_object (large_string.c_str(), 60000, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack ();


  شروع_object ("a", 1, c);
  جمع_کن_wide_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_c, c);
  c->برداشت_pack ();

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_repack_آخرین (hb_serialize_context_t* c, منطقي with_سرریز)
{
  std::string large_string(70000, 'c');
  c->شروع_serialize<حرف> ();
  c->push();

  // Obj E
  بیعلامت obj_e_1, obj_e_2;
  إذا (with_سرریز) {
    obj_e_1 = جمع_کن_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } وإلا {
    obj_e_2 = جمع_کن_object("a", 1, c);
  }

  // Obj D
  c->push();
  جمع_کن_جابه_جایی(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  بیعلامت obj_d = c->برداشت_pack(خطأ);

  جمع_کن_جابه_جایی(obj_d, c);
  hb_always_assert(c->آخرین_added_فرزند_شاخص() == obj_d);

  إذا (!with_سرریز) {
    obj_e_1 = جمع_کن_object("a", 1, c);
  }

  // Obj C
  c->push();
  جمع_کن_جابه_جایی(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  بیعلامت obj_c = c->برداشت_pack(خطأ);

  جمع_کن_جابه_جایی(obj_c, c);

  // Obj B
  بیعلامت obj_b = جمع_کن_object("b", 1, c);
  جمع_کن_جابه_جایی(obj_b, c);

  // Obj A
  c->repack_آخرین(obj_d);
  c->برداشت_pack(خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_spaces (hb_serialize_context_t* c, منطقي with_سرریز)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_i;

  إذا (with_سرریز)
    obj_i = جمع_کن_object ("i", 1, c);

  // Space 2
  بیعلامت obj_h = جمع_کن_object ("h", 1, c);

  شروع_object (large_string.c_str(), 30000, c);
  جمع_کن_جابه_جایی (obj_h, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  // Space 1
  إذا (!with_سرریز)
    obj_i = جمع_کن_object ("i", 1, c);

  شروع_object (large_string.c_str(), 30000, c);
  جمع_کن_جابه_جایی (obj_i, c);
  بیعلامت obj_g = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 30000, c);
  جمع_کن_جابه_جایی (obj_i, c);
  بیعلامت obj_f = c->برداشت_pack (خطأ);

  شروع_object ("d", 1, c);
  جمع_کن_جابه_جایی (obj_g, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_wide_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_c, c);
  جمع_کن_wide_جابه_جایی (obj_d, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_g = جمع_کن_object ("g", 1, c);
  بیعلامت obj_h = جمع_کن_object ("h", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_g, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_h, c);
  بیعلامت obj_f = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object ("d", 1, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  جمع_کن_جابه_جایی (obj_h, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_c, c);
  جمع_کن_wide_جابه_جایی (obj_d, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_g_اول = جمع_کن_object ("g", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_g_اول, c);
  بیعلامت obj_e_اول = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_e_اول, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  بیعلامت obj_h_اول = جمع_کن_object ("h", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_h_اول, c);
  بیعلامت obj_f = c->برداشت_pack (خطأ);

  شروع_object ("d", 1, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  بیعلامت obj_g = جمع_کن_object ("g", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_g, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  بیعلامت obj_h = جمع_کن_object ("h", 1, c);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  جمع_کن_جابه_جایی (obj_h, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_c, c);
  جمع_کن_wide_جابه_جایی (obj_d, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize ();
}

ساكن فراغ
populate_serializer_short_and_wide_subgraph_ریشه (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_e = جمع_کن_object ("e", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_c, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_c, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_c, c);
  جمع_کن_wide_جابه_جایی (obj_d, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_short_and_wide_subgraph_ریشه_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_e_اول = جمع_کن_object ("e", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_e_اول, c);
  بیعلامت obj_c_اول = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_c_اول, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  بیعلامت obj_e = جمع_کن_object ("e", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);


  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_c, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_c_اول, c);
  جمع_کن_wide_جابه_جایی (obj_d, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_تقسیم_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_f = جمع_کن_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 40000, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_wide_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_c, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_تقسیم_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_f = جمع_کن_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str(), 40000, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_c, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_تقسیم_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_f_اول = جمع_کن_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جمع_کن_جابه_جایی (obj_f_اول, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  بیعلامت obj_f = جمع_کن_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_wide_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_c, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_تقسیم_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->شروع_serialize<حرف> ();

  // Space 2

  بیعلامت obj_f_double_اول = جمع_کن_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جمع_کن_جابه_جایی (obj_f_double_اول, c);
  بیعلامت obj_d_اول = c->برداشت_pack (خطأ);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_d_اول, c);
  بیعلامت obj_b_اول = c->برداشت_pack (خطأ);

  // Space 1

  بیعلامت obj_f_اول = جمع_کن_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جمع_کن_جابه_جایی (obj_f_اول, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  // Space 0

  بیعلامت obj_f = جمع_کن_object ("f", 1, c);

  شروع_object (large_string.c_str(), 40000, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  //ریشه
  شروع_object ("a", 1, c);
  جمع_کن_جابه_جایی (obj_b, c);
  جمع_کن_wide_جابه_جایی (obj_b_اول, c);
  جمع_کن_wide_جابه_جایی (obj_c, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بیعلامت obj_5 = جمع_کن_object ("mn", 2, c);

  بیعلامت obj_4 = جمع_کن_object ("jkl", 3, c);

  شروع_object ("ghi", 3, c);
  جمع_کن_جابه_جایی (obj_4, c);
  بیعلامت obj_3 = c->برداشت_pack (خطأ);

  شروع_object ("def", 3, c);
  جمع_کن_جابه_جایی (obj_3, c);
  بیعلامت obj_2 = c->برداشت_pack (خطأ);

  شروع_object ("abc", 3, c);
  جمع_کن_جابه_جایی (obj_2, c);
  جمع_کن_جابه_جایی (obj_4, c);
  جمع_کن_جابه_جایی (obj_5, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بیعلامت obj_6 = جمع_کن_object ("opqrst", 6, c);

  بیعلامت obj_5 = جمع_کن_object ("mn", 2, c);

  شروع_object ("jkl", 3, c);
  جمع_کن_جابه_جایی (obj_6, c);
  بیعلامت obj_4 = c->برداشت_pack (خطأ);

  شروع_object ("ghi", 3, c);
  جمع_کن_جابه_جایی (obj_4, c);
  بیعلامت obj_3 = c->برداشت_pack (خطأ);

  شروع_object ("def", 3, c);
  جمع_کن_جابه_جایی (obj_3, c);
  بیعلامت obj_2 = c->برداشت_pack (خطأ);

  شروع_object ("abc", 3, c);
  جمع_کن_جابه_جایی (obj_2, c);
  جمع_کن_جابه_جایی (obj_4, c);
  جمع_کن_جابه_جایی (obj_5, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بیعلامت obj_d = جمع_کن_object ("d", 1, c);

  شروع_object ("b", 1, c);
  جمع_کن_جابه_جایی (obj_d, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("e", 1, c);
  جمع_کن_virtual_جابه_جایی (obj_b, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  شروع_object ("c", 1, c);
  جمع_کن_جابه_جایی (obj_e, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_جابه_جایی (obj_b, c);
  جمع_کن_جابه_جایی (obj_c, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_24_and_32_بیت_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت obj_f = جمع_کن_object ("f", 1, c);
  بیعلامت obj_g = جمع_کن_object ("g", 1, c);
  بیعلامت obj_j = جمع_کن_object ("j", 1, c);
  بیعلامت obj_k = جمع_کن_object ("k", 1, c);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_f, c);
  بیعلامت obj_c = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_g, c);
  بیعلامت obj_d = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_j, c);
  بیعلامت obj_h = c->برداشت_pack (خطأ);

  شروع_object (large_string.c_str (), 40000, c);
  جمع_کن_جابه_جایی (obj_k, c);
  بیعلامت obj_i = c->برداشت_pack (خطأ);

  شروع_object ("e", 1, c);
  جمع_کن_wide_جابه_جایی (obj_h, c);
  جمع_کن_wide_جابه_جایی (obj_i, c);
  بیعلامت obj_e = c->برداشت_pack (خطأ);

  شروع_object ("b", 1, c);
  جمع_کن_24_جابه_جایی (obj_c, c);
  جمع_کن_24_جابه_جایی (obj_d, c);
  جمع_کن_24_جابه_جایی (obj_e, c);
  بیعلامت obj_b = c->برداشت_pack (خطأ);

  شروع_object ("a", 1, c);
  جمع_کن_24_جابه_جایی (obj_b, c);
  c->برداشت_pack (خطأ);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              صحيح رقم_extensions = 0,
                                              منطقي shared_subtables = خطأ,
                                              منطقي نسخه_1_2 = خطأ)
{
  constexpr صحيح رقم_lookups = 5;
  constexpr صحيح رقم_subtables = رقم_lookups * 2;
  بیعلامت صحيح lookups[رقم_lookups];
  بیعلامت صحيح subtables[رقم_subtables];
  بیعلامت صحيح extensions[رقم_subtables];

  std::string large_string(60000, 'a');
  c->شروع_serialize<حرف> ();


  لكل (صحيح i = رقم_subtables - 1; i >= 0; i--)
    subtables[i] = جمع_کن_object(large_string.c_str (), 15000 + i, c);

  لكل (صحيح i = رقم_subtables - 1;
       i >= (رقم_lookups - رقم_extensions) * 2;
       i--)
  {
    extensions[i] = جمع_کن_extension (subtables[i], 5, c);
  }

  لكل (صحيح i = رقم_lookups - 1; i >= 0; i--)
  {
    منطقي is_ext = (i >= (رقم_lookups - رقم_extensions));

    شروع_lookup (is_ext ? (حرف) 7 : (حرف) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    إذا (is_ext) {
      إذا (shared_subtables && i > 2) {
        جمع_کن_جابه_جایی (extensions[i * 2 - 1], c);
      }
      جمع_کن_جابه_جایی (extensions[i * 2], c);
      جمع_کن_جابه_جایی (extensions[i * 2 + 1], c);
    } وإلا {
      إذا (shared_subtables && i > 2) {
        جمع_کن_جابه_جایی (subtables[i * 2 - 1], c);
      }
      جمع_کن_جابه_جایی (subtables[i * 2], c);
      جمع_کن_جابه_جایی (subtables[i * 2 + 1], c);
    }

    lookups[i] = پایان_lookup (c);
  }

  بیعلامت lookup_list = نسخه_1_2
                         ? جمع_کن_lookup_list_1_2 (lookups, رقم_lookups, c)
                         : جمع_کن_lookup_list (lookups, رقم_lookups, c);

  إذا (نسخه_1_2)
    جمع_کن_gsubgpos_سرآیند_1_2 (lookup_list, c);
  وإلا
    جمع_کن_gsubgpos_سرآیند (lookup_list, c);

  c->انتها_serialize();
}

قالب<صحيح رقم_pair_pos_1, صحيح رقم_pair_مجموعه>
ساكن فراغ
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           منطقي as_extension = خطأ)
{
  std::string large_string(60000, 'a');
  c->شروع_serialize<حرف> ();

  constexpr صحيح مجموع_pair_مجموعه = رقم_pair_pos_1 * رقم_pair_مجموعه;
  بیعلامت pair_مجموعه_2[مجموع_pair_مجموعه];
  بیعلامت coverage[رقم_pair_pos_1];
  بیعلامت pair_pos_1[رقم_pair_pos_1];

  لكل (صحيح i = رقم_pair_pos_1 - 1; i >= 0; i--)
  {
    لكل (صحيح j = (i + 1) * رقم_pair_مجموعه - 1; j >= i * رقم_pair_مجموعه; j--)
      pair_مجموعه_2[j] = جمع_کن_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = جمع_کن_coverage (i * رقم_pair_مجموعه,
                                (i + 1) * رقم_pair_مجموعه - 1, c);

    pair_pos_1[i] = جمع_کن_pair_pos_1 (&pair_مجموعه_2[i * رقم_pair_مجموعه],
                                    رقم_pair_مجموعه,
                                    coverage[i],
                                    c);
  }

  بیعلامت pair_pos_2 = جمع_کن_object (large_string.c_str(), 200, c);

  إذا (as_extension) {
    pair_pos_2 = جمع_کن_extension (pair_pos_2, 2, c);
    لكل (صحيح i = رقم_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = جمع_کن_extension (pair_pos_1[i], 2, c);
  }

  شروع_lookup (as_extension ? 9 : 2, 1 + رقم_pair_pos_1, c);

  لكل (صحيح i = 0; i < رقم_pair_pos_1; i++)
    جمع_کن_جابه_جایی (pair_pos_1[i], c);
  جمع_کن_جابه_جایی (pair_pos_2, c);

  بیعلامت lookup = پایان_lookup (c);

  بیعلامت lookup_list = جمع_کن_lookup_list (&lookup, 1, c);

  جمع_کن_gsubgpos_سرآیند (lookup_list, c);

  c->انتها_serialize();
}

قالب<صحيح رقم_pair_pos_2, صحيح رقم_class_1, صحيح رقم_class_2>
ساكن فراغ
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           منطقي as_extension = خطأ,
                                           منطقي with_device_tables = خطأ,
                                           منطقي extra_جدول = درست)
{
  std::string large_string(100000, 'a');
  c->شروع_serialize<حرف> ();

  بیعلامت coverage[رقم_pair_pos_2];
  بیعلامت class_def_1[رقم_pair_pos_2];
  بیعلامت class_def_2[رقم_pair_pos_2];
  بیعلامت pair_pos_2[رقم_pair_pos_2];

  بیعلامت* device_tables = (بیعلامت*) حافظه_صفر_بده (رقم_pair_pos_2 * رقم_class_1 * رقم_class_2,
                                                حجم(بیعلامت));

  // Total glyphs = num_class_1 * num_pair_pos_2
  لكل (صحيح i = رقم_pair_pos_2 - 1; i >= 0; i--)
  {
    بیعلامت شروع_glyph = 5 + i * رقم_class_1;
    إذا (رقم_class_2 >= رقم_class_1)
    {
      class_def_2[i] = جمع_کن_class_def (11,
                                      10 + رقم_class_2, c);
      class_def_1[i] = جمع_کن_class_def (شروع_glyph + 1,
                                      شروع_glyph + رقم_class_1,
                                      c);
    } وإلا {
      class_def_1[i] = جمع_کن_class_def (شروع_glyph + 1,
                                      شروع_glyph + رقم_class_1,
                                      c);
      class_def_2[i] = جمع_کن_class_def (11,
                                      10 + رقم_class_2, c);
    }

    coverage[i] = جمع_کن_coverage (شروع_glyph,
                                شروع_glyph + رقم_class_1 - 1,
                                c);

    إذا (with_device_tables)
    {
      لكل(صحيح j = (i + 1) * رقم_class_1 * رقم_class_2 - 1;
          j >= i * رقم_class_1 * رقم_class_2;
          j--)
      {
        uint8_t جدول[] = {
          (uint8_t) ((j >> 8) & 0xFF),
          (uint8_t) (j & 0xFF),
        };
        device_tables[j] = جمع_کن_object ((حرف*) جدول, 2, c);
      }
    }

    pair_pos_2[i] = جمع_کن_pair_pos_2 (1 + i * رقم_class_1,
                                    coverage[i],
                                    class_def_1[i], رقم_class_1,
                                    class_def_2[i], رقم_class_2,
                                    with_device_tables
                                    ? &device_tables[i * رقم_class_1 * رقم_class_2]
                                    : اشاره_تهی,
                                    c);
  }


  بیعلامت pair_pos_1 = 0;
  إذا (extra_جدول) pair_pos_1 = جمع_کن_object (large_string.c_str(), 100000, c);

  إذا (as_extension) {
    لكل (صحيح i = رقم_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = جمع_کن_extension (pair_pos_2[i], 2, c);

    إذا (extra_جدول)
      pair_pos_1 = جمع_کن_extension (pair_pos_1, 2, c);
  }

  شروع_lookup (as_extension ? 9 : 2, 1 + رقم_pair_pos_2, c);

  إذا (extra_جدول)
    جمع_کن_جابه_جایی (pair_pos_1, c);

  لكل (صحيح i = 0; i < رقم_pair_pos_2; i++)
    جمع_کن_جابه_جایی (pair_pos_2[i], c);

  بیعلامت lookup = پایان_lookup (c);

  بیعلامت lookup_list = جمع_کن_lookup_list (&lookup, 1, c);

  جمع_کن_gsubgpos_سرآیند (lookup_list, c);

  c->انتها_serialize();

  حرر (device_tables);
}

قالب<صحيح mark_شمار,
    صحيح class_شمار,
    صحيح پایه_شمار,
    صحيح جدول_شمار_2>
ساكن فراغ
populate_serializer_with_large_mark_پایه_pos_1 (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  Mark_پایه_Pos_Buffers<mark_شمار, class_شمار, پایه_شمار, جدول_شمار_2> buffers (c);

  بیعلامت mark_پایه_pos[جدول_شمار_2];
  لكل (بیعلامت i = 0; i < جدول_شمار_2; i++)
    mark_پایه_pos[i] = buffers.create_mark_پایه_pos_1 (i, c);

  لكل (صحيح i = 0; i < جدول_شمار_2; i++)
    mark_پایه_pos[i] = جمع_کن_extension (mark_پایه_pos[i], 4, c);

  شروع_lookup (9, جدول_شمار_2, c);

  لكل (صحيح i = 0; i < جدول_شمار_2; i++)
    جمع_کن_جابه_جایی (mark_پایه_pos[i], c);

  بیعلامت lookup = پایان_lookup (c);

  بیعلامت lookup_list = جمع_کن_lookup_list (&lookup, 1, c);

  جمع_کن_gsubgpos_سرآیند (lookup_list, c);

  c->انتها_serialize();
}

قالب<بیعلامت Lig_Subst_شمار,
         بیعلامت Liga_مجموعه_شمار,
         بیعلامت Liga_در_هر_مجموعه_شمار,
         بیعلامت Liga_اندازه>
ساكن فراغ populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    منطقي sequential_liga_sets,
                                                    منطقي shared,
                                                    بیعلامت *liga_subst_id,
                                                    منطقي unique_lig_str = خطأ)
{
  حرف ch = 'a';

  بیعلامت liga[Liga_مجموعه_شمار * Liga_در_هر_مجموعه_شمار];
  بیعلامت liga_مجموعه[Liga_مجموعه_شمار];
  لكل (بیعلامت l = 0; l < Lig_Subst_شمار; l++) {
    بیعلامت coverage_شروع = 0;
    بیعلامت coverage_انتها = Liga_مجموعه_شمار - 1;
    إذا (sequential_liga_sets) {
      coverage_شروع = l * Liga_مجموعه_شمار;
      coverage_انتها = (l + 1) * Liga_مجموعه_شمار - 1;
    }
    بیعلامت coverage = جمع_کن_coverage(coverage_شروع, coverage_انتها, c, shared);

    لكل (بیعلامت i = 0; i < Liga_مجموعه_شمار; i++) {
      لكل (بیعلامت j = 0; j < Liga_در_هر_مجموعه_شمار; j++)
      {
        std::string large_string(100000, ch);
        شروع_object (large_string.c_str(), Liga_اندازه, c);
        إذا (unique_lig_str)
          ch += 1;
        جمع_کن_virtual_جابه_جایی(coverage, c);
        liga[i * Liga_در_هر_مجموعه_شمار + j] = c->برداشت_pack (shared);
      }

      جمع_کن_liga_مجموعه_سرآیند(Liga_در_هر_مجموعه_شمار, c);
      جمع_کن_virtual_جابه_جایی(coverage, c);
      لكل (بیعلامت j = 0; j < Liga_در_هر_مجموعه_شمار; j++)
      {
        جمع_کن_جابه_جایی(liga[i * Liga_در_هر_مجموعه_شمار + j], c);
      }

      liga_مجموعه[i] = c->برداشت_pack(shared);
    }

    جمع_کن_liga_سرآیند(coverage, Liga_مجموعه_شمار, c);
    لكل (بیعلامت i = 0; i < Liga_مجموعه_شمار; i++)
    {
      جمع_کن_جابه_جایی(liga_مجموعه[i], c);
    }

    liga_subst_id[l] = c->برداشت_pack(shared);
  }

  لكل (بیعلامت l = 0; l < Lig_Subst_شمار; l++) {
    liga_subst_id[l] = جمع_کن_extension(liga_subst_id[l], 4, c);
  }
}

قالب<بیعلامت liga_subst_شمار,
         بیعلامت liga_مجموعه_شمار,
         بیعلامت liga_در_هر_مجموعه_شمار,
         بیعلامت liga_اندازه>
ساكن فراغ
populate_serializer_with_large_liga (hb_serialize_context_t* c, منطقي sequential_liga_sets)
{
  c->شروع_serialize<حرف> ();

  بیعلامت liga_subst[liga_subst_شمار];
  populate_serializer_with_large_ligsubst<liga_subst_شمار, liga_مجموعه_شمار, liga_در_هر_مجموعه_شمار, liga_اندازه>(c, sequential_liga_sets, خطأ, liga_subst);

  شروع_lookup (7, liga_subst_شمار, c);
  لكل (بیعلامت l = 0; l < liga_subst_شمار; l++) {
    جمع_کن_جابه_جایی(liga_subst[l], c);
  }

  بیعلامت lookup = پایان_lookup (c);

  بیعلامت lookup_list = جمع_کن_lookup_list (&lookup, 1, c);

  جمع_کن_gsubgpos_سرآیند (lookup_list, c);

  c->انتها_serialize();
}

ساكن فراغ
populate_serializer_with_large_liga_overlapping_clone_نتیجه (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->شروع_serialize<حرف> ();

  constexpr بیعلامت liga_اندازه = 30000;

  بیعلامت liga[6];
  بیعلامت liga_subst[3];
  بیعلامت liga_مجموعه[2];

  // LigaSubst 3
  بیعلامت coverage = جمع_کن_coverage(1, 1, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    شروع_object (large_string.c_str(), liga_اندازه, c);
    جمع_کن_virtual_جابه_جایی(coverage, c);
    liga[i] = c->برداشت_pack (خطأ);
  }

  جمع_کن_liga_مجموعه_سرآیند(2, c);
  جمع_کن_virtual_جابه_جایی(coverage, c);
  لكل (بیعلامت i = 0; i < 2; i++)
    جمع_کن_جابه_جایی(liga[i], c);
  liga_مجموعه[0] = c->برداشت_pack(خطأ);

  جمع_کن_liga_سرآیند(coverage, 1, c);
  جمع_کن_جابه_جایی(liga_مجموعه[0], c);
  liga_subst[2] = c->برداشت_pack(خطأ);

  // LigaSubst 2
  coverage = جمع_کن_coverage(0, 1, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    شروع_object (large_string.c_str(), liga_اندازه, c);
    جمع_کن_virtual_جابه_جایی(coverage, c);
    liga[i] = c->برداشت_pack (خطأ);
  }

  جمع_کن_liga_مجموعه_سرآیند(1, c);
  جمع_کن_virtual_جابه_جایی(coverage, c);
  جمع_کن_جابه_جایی(liga[1], c);
  liga_مجموعه[1] = c->برداشت_pack(خطأ);

  جمع_کن_liga_مجموعه_سرآیند(1, c);
  جمع_کن_virtual_جابه_جایی(coverage, c);
  جمع_کن_جابه_جایی(liga[0], c);
  liga_مجموعه[0] = c->برداشت_pack(خطأ);

  جمع_کن_liga_سرآیند(coverage, 2, c);
  جمع_کن_جابه_جایی(liga_مجموعه[0], c);
  جمع_کن_جابه_جایی(liga_مجموعه[1], c);
  liga_subst[1] = c->برداشت_pack(خطأ);

  // LigaSubst 1
  coverage = جمع_کن_coverage(0, 0, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    شروع_object (large_string.c_str(), liga_اندازه, c);
    جمع_کن_virtual_جابه_جایی(coverage, c);
    liga[i] = c->برداشت_pack (خطأ);
  }

  جمع_کن_liga_مجموعه_سرآیند(2, c);
  جمع_کن_virtual_جابه_جایی(coverage, c);
  لكل (بیعلامت i = 0; i < 2; i++)
    جمع_کن_جابه_جایی(liga[i], c);
  liga_مجموعه[0] = c->برداشت_pack(خطأ);

  جمع_کن_liga_سرآیند(coverage, 1, c);
  جمع_کن_جابه_جایی(liga_مجموعه[0], c);
  liga_subst[0] = c->برداشت_pack(خطأ);

  لكل (صحيح l = 2; l >= 0; l--) {
    liga_subst[l] = جمع_کن_extension(liga_subst[l], 4, c);
  }

  شروع_lookup (7, 3, c);
  لكل (بیعلامت l = 0; l < 3; l++) {
    جمع_کن_جابه_جایی(liga_subst[l], c);
  }

  بیعلامت lookup = پایان_lookup (c);

  بیعلامت lookup_list = جمع_کن_lookup_list (&lookup, 1, c);

  جمع_کن_gsubgpos_سرآیند (lookup_list, c);

  c->انتها_serialize();
}

قالب<بیعلامت liga_subst_شمار,
         بیعلامت liga_مجموعه_شمار,
         بیعلامت liga_در_هر_مجموعه_شمار,
         بیعلامت liga_اندازه>
ساكن فراغ
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, منطقي sequential_liga_sets)
{
  c->شروع_serialize<حرف> ();

  بیعلامت lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  بیعلامت liga_subst_idx[liga_subst_شمار];
  populate_serializer_with_large_ligsubst<liga_subst_شمار, liga_مجموعه_شمار, liga_در_هر_مجموعه_شمار, liga_اندازه>(c, sequential_liga_sets, درست, liga_subst_idx, درست);

  شروع_lookup (7, liga_subst_شمار, c);
  لكل (بیعلامت l = 0; l < liga_subst_شمار; l++) {
    جمع_کن_جابه_جایی(liga_subst_idx[l], c);
  }

  lookups[0] = پایان_lookup (c);

  // Lookup with 2 LigSubst tables
  بیعلامت liga_subst[liga_subst_شمار + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, خطأ, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_شمار, liga_مجموعه_شمار, liga_در_هر_مجموعه_شمار, liga_اندازه>(c, sequential_liga_sets, درست, &liga_subst[1], درست);

  شروع_lookup (7, liga_subst_شمار + 1, c);
  لكل (بیعلامت l = 0; l <= liga_subst_شمار; l++) {
    جمع_کن_جابه_جایی(liga_subst[l], c);
  }

  lookups[1] = پایان_lookup (c);

  بیعلامت lookup_list = جمع_کن_lookup_list (lookups, 2, c);
  جمع_کن_gsubgpos_سرآیند (lookup_list, c);

  c->انتها_serialize();
}

قالب<بیعلامت liga_subst_شمار,
         بیعلامت liga_مجموعه_شمار,
         بیعلامت liga_در_هر_مجموعه_شمار,
         بیعلامت liga_اندازه>
ساكن فراغ
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->شروع_serialize<حرف> ();

  بیعلامت liga_subst[liga_subst_شمار + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, درست, درست, liga_subst, درست);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_شمار, liga_مجموعه_شمار, liga_در_هر_مجموعه_شمار, liga_اندازه>(c, درست, درست, &liga_subst[1], درست);

  شروع_lookup (7, liga_subst_شمار + 1, c);
  لكل (بیعلامت l = 0; l <= liga_subst_شمار; l++) {
    جمع_کن_جابه_جایی(liga_subst[l], c);
  }

  بیعلامت lookup = پایان_lookup (c);

  بیعلامت lookup_list = جمع_کن_lookup_list (&lookup, 1, c);
  جمع_کن_gsubgpos_سرآیند (lookup_list, c);

  c->انتها_serialize();
}

ساكن فراغ test_مرتب_سازی_کوتاهترین ()
{
  size_t میانگیر_اندازه = 100;
  فراغ* میانگیر_a = حافظه_بده (میانگیر_اندازه);
  فراغ* میانگیر_e = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t a (میانگیر_a, میانگیر_اندازه);
  hb_serialize_context_t e (میانگیر_e, میانگیر_اندازه);
  populate_serializer_complex_2 (&a);

  گراف_t گراف = گراف_t::create (a.object_گراف ()).value ();
  hb_always_assert (گراف.مرتب_سازی_کوتاهترین_فاصله ().is_ok());
  گراف.normalize();

  // Expected graph
  e.شروع_serialize();
  بیعلامت jkl = جمع_کن_object ("jkl", 3, &e);

  شروع_object("ghi", 3, &e);
  جمع_کن_جابه_جایی(jkl, &e);
  بیعلامت ghi = e.برداشت_pack(خطأ);

  شروع_object("def", 3, &e);
  جمع_کن_جابه_جایی(ghi, &e);
  بیعلامت def = e.برداشت_pack(خطأ);

  بیعلامت mn = جمع_کن_object("mn", 2, &e);

  شروع_object ("abc", 3, &e);
  جمع_کن_جابه_جایی (def, &e);
  جمع_کن_جابه_جایی (jkl, &e);
  جمع_کن_جابه_جایی (mn, &e);
  e.برداشت_pack (خطأ);
  e.انتها_serialize();

  گراف_t expected = گراف_t::create (e.object_گراف ()).value ();
  expected.normalize();

  أكد(expected == گراف);

  حرر (میانگیر_a);
  حرر (میانگیر_e);
}

ساكن فراغ test_duplicate_برگ ()
{
  size_t میانگیر_اندازه = 100;
  فراغ* میانگیر_a = حافظه_بده (میانگیر_اندازه);
  فراغ* میانگیر_e = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t a (میانگیر_a, میانگیر_اندازه);
  hb_serialize_context_t e (میانگیر_e, میانگیر_اندازه);
  populate_serializer_complex_2 (&a);

  گراف_t گراف = گراف_t::create (a.object_گراف ()).value ();
  hb_always_assert (گراف.duplicate (4, 1, خطأ).is_ok ());
  گراف.normalize();

  e.شروع_serialize();
  بیعلامت mn = جمع_کن_object("mn", 2, &e);
  بیعلامت jkl_2 = جمع_کن_object("jkl", 3, &e);

  شروع_object("ghi", 3, &e);
  جمع_کن_جابه_جایی(jkl_2, &e);
  بیعلامت ghi = e.برداشت_pack(خطأ);

  شروع_object("def", 3, &e);
  جمع_کن_جابه_جایی(ghi, &e);
  بیعلامت def = e.برداشت_pack(خطأ);

  بیعلامت jkl_1 = جمع_کن_object("jkl", 3, &e);

  شروع_object("abc", 3, &e);
  جمع_کن_جابه_جایی(def, &e);
  جمع_کن_جابه_جایی(jkl_1, &e);
  جمع_کن_جابه_جایی(mn, &e);
  e.برداشت_pack(خطأ);

  گراف_t expected = گراف_t::create (e.object_گراف ()).value ();
  expected.normalize();

  أكد(expected == گراف);

  حرر (میانگیر_a);
  حرر (میانگیر_e);
}

ساكن فراغ test_duplicate_interior ()
{
  size_t میانگیر_اندازه = 100;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_complex_3 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();
  hb_always_assert (گراف.duplicate (3, 2, خطأ).is_ok ());

  hb_always_assert(مقایسه_رشته (گراف.object (6).سر_2, "jkl", 3) == 0);
  hb_always_assert(گراف.object (6).real_links.درازی == 1);
  hb_always_assert(گراف.object (6).real_links[0].objidx == 0);

  hb_always_assert(مقایسه_رشته (گراف.object (5).سر_2, "abc", 3) == 0);
  hb_always_assert(گراف.object (5).real_links.درازی == 3);
  hb_always_assert(گراف.object (5).real_links[0].objidx == 4);
  hb_always_assert(گراف.object (5).real_links[1].objidx == 2);
  hb_always_assert(گراف.object (5).real_links[2].objidx == 1);

  hb_always_assert(مقایسه_رشته (گراف.object (4).سر_2, "def", 3) == 0);
  hb_always_assert(گراف.object (4).real_links.درازی == 1);
  hb_always_assert(گراف.object (4).real_links[0].objidx == 3);

  hb_always_assert(مقایسه_رشته (گراف.object (3).سر_2, "ghi", 3) == 0);
  hb_always_assert(گراف.object (3).real_links.درازی == 1);
  hb_always_assert(گراف.object (3).real_links[0].objidx == 6);

  hb_always_assert(مقایسه_رشته (گراف.object (2).سر_2, "jkl", 3) == 0);
  hb_always_assert(گراف.object (2).real_links.درازی == 1);
  hb_always_assert(گراف.object (2).real_links[0].objidx == 0);

  hb_always_assert(مقایسه_رشته (گراف.object (1).سر_2, "mn", 2) == 0);
  hb_always_assert(گراف.object (1).real_links.درازی == 0);

  hb_always_assert(مقایسه_رشته (گراف.object (0).سر_2, "opqrst", 6) == 0);
  hb_always_assert(گراف.object (0).real_links.درازی == 0);

  حرر (میانگیر);
}

ساكن فراغ
test_serialize ()
{
  size_t میانگیر_اندازه = 100;
  فراغ* میانگیر_1 = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c1 (میانگیر_1, میانگیر_اندازه);
  populate_serializer_simple (&c1);
  hb_بایتها_t expected = c1.copy_بایتها ();

  گراف_t گراف = گراف_t::create (c1.object_گراف ()).value ();
  hb_blob_t* بیرون = گراف::serialize (گراف).value ();
  حرر (میانگیر_1);

  hb_بایتها_t actual = بیرون->as_بایتها ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (بیرون);
}

ساكن فراغ test_will_سرریز_1 ()
{
  size_t میانگیر_اندازه = 100;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);

  populate_serializer_complex_2 (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (!گراف::will_سرریز (گراف, اشاره_تهی).value ());

  حرر (میانگیر);
}

ساكن فراغ test_will_سرریز_2 ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_سرریز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (گراف::will_سرریز (گراف, اشاره_تهی).value ());

  حرر (میانگیر);
}

ساكن فراغ test_will_سرریز_3 ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_dedup_سرریز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (گراف::will_سرریز (گراف, اشاره_تهی).value ());

  حرر (میانگیر);
}

ساكن فراغ test_resolve_overflows_از_طریق_مرتب_سازی ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_سرریز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* بیرون = hb_resolve_overflows (c.object_گراف (), HB_برچسب_NONE);
  hb_always_assert (بیرون);
  hb_بایتها_t نتیجه = بیرون->as_بایتها ();
  hb_always_assert (نتیجه.درازی == (80000 + 3 + 3 * 2));

  حرر (میانگیر);
  hb_blob_destroy (بیرون);
}

ساكن فراغ test_resolve_overflows_از_طریق_duplication ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_dedup_سرریز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* بیرون = hb_resolve_overflows (c.object_گراف (), HB_برچسب_NONE);
  hb_always_assert (بیرون);
  hb_بایتها_t نتیجه = بیرون->as_بایتها ();
  hb_always_assert (نتیجه.درازی == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  حرر (میانگیر);
  hb_blob_destroy (بیرون);
}

ساكن فراغ test_resolve_overflows_از_طریق_مضرب_duplications ()
{
  size_t میانگیر_اندازه = 300000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_مضرب_dedup_سرریز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* بیرون = hb_resolve_overflows (c.object_گراف (), HB_برچسب_NONE, 5);
  hb_always_assert (بیرون);

  حرر (میانگیر);
  hb_blob_destroy (بیرون);
}

ساكن فراغ test_resolve_overflows_از_طریق_space_assignment ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_spaces (&c, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_spaces (&e, خطأ);

  پیاپی_resolve_سرریز_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_overflows_از_طریق_isolation ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_isolation_سرریز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.جابه_جایی_سرریز ());
  hb_blob_t* بیرون = hb_resolve_overflows (c.object_گراف (), HB_برچسب ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (بیرون);
  hb_بایتها_t نتیجه = بیرون->as_بایتها ();
  hb_always_assert (نتیجه.درازی == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  حرر (میانگیر);
  hb_blob_destroy (بیرون);
}

ساكن فراغ test_resolve_overflows_از_طریق_isolation_with_recursive_duplication ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_isolation_سرریز_complex (&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_isolation_سرریز_complex_expected (&e);

  پیاپی_resolve_سرریز_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_overflows_از_طریق_isolating_16bit_space ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_spaces_16bit_connection (&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_spaces_16bit_connection_expected (&e);

  پیاپی_resolve_سرریز_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_overflows_از_طریق_isolating_16bit_space_2 ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_short_and_wide_subgraph_ریشه (&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_short_and_wide_subgraph_ریشه_expected (&e);

  پیاپی_resolve_سرریز_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_overflows_از_طریق_isolation_spaces ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_isolation_سرریز_spaces (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.جابه_جایی_سرریز ());
  hb_blob_t* بیرون = hb_resolve_overflows (c.object_گراف (), HB_برچسب ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (بیرون);
  hb_بایتها_t نتیجه = بیرون->as_بایتها ();

  بیعلامت expected_درازی = 3 + 2 * 60000; // objects
  expected_درازی += 2 * 4 + 2 * 2; // links
  hb_always_assert (نتیجه.درازی == expected_درازی);

  حرر (میانگیر);
  hb_blob_destroy (بیرون);
}

ساكن فراغ test_resolve_mixed_overflows_از_طریق_isolation_spaces ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_24_and_32_بیت_offsets (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.جابه_جایی_سرریز ());
  hb_blob_t* بیرون = hb_resolve_overflows (c.object_گراف (), HB_برچسب ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (بیرون);
  hb_بایتها_t نتیجه = بیرون->as_بایتها ();

  بیعلامت expected_درازی =
      // Objects
      7 +
      4 * 40000;

  expected_درازی +=
      // Links
      2 * 4 +  // 32
      4 * 3 +  // 24
      4 * 2;   // 16

  hb_always_assert (نتیجه.درازی == expected_درازی);

  حرر (میانگیر);
  hb_blob_destroy (بیرون);
}

ساكن فراغ test_resolve_with_extension_promotion ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_extension_promotion (&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_extension_promotion (&e, 3);

  پیاپی_resolve_سرریز_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             درست);
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_shared_extension_promotion ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_extension_promotion (&c, 0, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_extension_promotion (&e, 3, درست);

  پیاپی_resolve_سرریز_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             درست);
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

#اگر_تعریف_نشده HB_NO_BEYOND_64K
ساكن فراغ test_resolve_with_extension_promotion_1_2 ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_extension_promotion (&c, 0, خطأ, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_extension_promotion (&e, 3, خطأ, درست);

  پیاپی_resolve_سرریز_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             درست);
  حرر (میانگیر);
  حرر (expected_میانگیر);
}
#پایان_اگر

ساكن فراغ test_resolve_with_repeated_extension_promotion ()
{
  size_t میانگیر_اندازه = 1000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);

  c.شروع_serialize<حرف> ();
  بیعلامت subtable = جمع_کن_object ("subt", 4, &c);
  شروع_lookup (5, 2, &c);
  جمع_کن_جابه_جایی (subtable, &c);
  جمع_کن_جابه_جایی (subtable, &c);
  بیعلامت lookup = پایان_lookup (&c);
  بیعلامت lookup_list = جمع_کن_lookup_list (&lookup, 1, &c);
  جمع_کن_gsubgpos_سرآیند (lookup_list, &c);
  c.انتها_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();
  خودکار context = گراف::gsubgpos_گراف_context_t::create (HB_برچسب ('G', 'S', 'U', 'B'),
                                                          گراف).value ();
  hb_always_assert (context.lookups.get_population () == 1);
  بیعلامت lookup_شاخص = *context.lookups.کلیدها ();
  گراف::Lookup* lookup_جدول_2 = context.lookups.get (lookup_شاخص);
  hb_always_assert (lookup_جدول_2->make_extension (context, lookup_شاخص).is_ok ());

  ثابت خودکار& lookup_رأس = گراف.vertices_[lookup_شاخص];
  hb_always_assert (lookup_رأس.obj ().real_links.درازی == 2);
  بیعلامت extension_شاخص = lookup_رأس.obj ().real_links[0].objidx;
  hb_always_assert (lookup_رأس.obj ().real_links[1].objidx == extension_شاخص);

  خودکار* extension = گراف.vertices_[extension_شاخص].as<
      گراف::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
  بیعلامت promoted_subtable = extension->get_subtable_شاخص (گراف, extension_شاخص).value ();
  hb_always_assert (گراف.vertices_[promoted_subtable].جدول_اندازه () == 4);
  hb_always_assert (گراف.update_parents ().is_ok ());
  hb_always_assert (گراف.vertices_[extension_شاخص].incoming_یالها_2 () == 2);
  hb_always_assert (گراف.vertices_[promoted_subtable].incoming_یالها_2 () == 1);

  hb_blob_t* بیرون = گراف::serialize (گراف).value ();
  hb_blob_destroy (بیرون);
  حرر (میانگیر);
}

ساكن فراغ test_resolve_with_basic_pair_pos_1_تقسیم ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, درست);

  پیاپی_resolve_سرریز_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'P', 'O', 'S'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_extension_pair_pos_1_تقسیم ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, درست);

  پیاپی_resolve_سرریز_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'P', 'O', 'S'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_basic_pair_pos_2_تقسیم ()
{
  size_t میانگیر_اندازه = 300000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, درست);

  پیاپی_resolve_سرریز_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'P', 'O', 'S'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_close_مقصد_حد_pair_pos_2_تقسیم ()
{
  size_t میانگیر_اندازه = 300000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, درست, خطأ, خطأ);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, درست, خطأ, خطأ);

  پیاپی_resolve_سرریز_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'P', 'O', 'S'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_pair_pos_2_تقسیم_with_device_tables ()
{
  size_t میانگیر_اندازه = 300000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, خطأ, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, درست, درست);

  پیاپی_resolve_سرریز_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'P', 'O', 'S'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_basic_mark_پایه_pos_1_تقسیم ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_mark_پایه_pos_1 <40, 10, 110, 1>(&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_mark_پایه_pos_1 <40, 10, 110, 2>(&e);

  پیاپی_resolve_سرریز_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'P', 'O', 'S'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_basic_liga_تقسیم ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, خطأ);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, خطأ);

  پیاپی_resolve_سرریز_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'S', 'U', 'B'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_liga_تقسیم_move ()
{
  size_t میانگیر_اندازه = 400000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, درست);

  پیاپی_resolve_سرریز_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'S', 'U', 'B'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_liga_تقسیم_overlapping_clone ()
{
  size_t میانگیر_اندازه = 400000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_large_liga_overlapping_clone_نتیجه(&e);

  پیاپی_resolve_سرریز_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'S', 'U', 'B'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_liga_تقسیم_shared_جدول ()
{
  size_t میانگیر_اندازه = 400000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, درست);

  پیاپی_resolve_سرریز_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'S', 'U', 'B'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_with_liga_تقسیم_shared_coverage ()
{
  size_t میانگیر_اندازه = 400000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  پیاپی_resolve_سرریز_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             درست,
                             HB_برچسب('G', 'S', 'U', 'B'));
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_overflows_از_طریق_splitting_spaces ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_تقسیم_spaces (&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_تقسیم_spaces_expected (&e);

  پیاپی_resolve_سرریز_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  حرر (میانگیر);
  حرر (expected_میانگیر);

}

ساكن فراغ test_resolve_overflows_از_طریق_splitting_spaces_2 ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_تقسیم_spaces_2 (&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_تقسیم_spaces_expected_2 (&e);

  پیاپی_resolve_سرریز_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ test_resolve_overflows_از_طریق_اولویت ()
{
  size_t میانگیر_اندازه = 160000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_اولویت_سرریز (&c);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_اولویت_سرریز_expected (&e);

  پیاپی_resolve_سرریز_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  حرر (میانگیر);
  حرر (expected_میانگیر);
}


ساكن فراغ test_virtual_link ()
{
  size_t میانگیر_اندازه = 100;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_virtual_link (&c);

  hb_blob_t* بیرون = hb_resolve_overflows (c.object_گراف (), HB_برچسب_NONE);
  hb_always_assert (بیرون);

  hb_بایتها_t نتیجه = بیرون->as_بایتها ();
  hb_always_assert (نتیجه.درازی == 5 + 4 * 2);
  hb_always_assert (نتیجه[0]  == 'a');
  hb_always_assert (نتیجه[5]  == 'c');
  hb_always_assert (نتیجه[8]  == 'e');
  hb_always_assert (نتیجه[9]  == 'b');
  hb_always_assert (نتیجه[12] == 'd');

  حرر (میانگیر);
  hb_blob_destroy (بیرون);
}

ساكن فراغ
test_shared_گره_with_virtual_links ()
{
  size_t میانگیر_اندازه = 100;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);

  c.شروع_serialize<حرف> ();

  بیعلامت obj_b = جمع_کن_object ("b", 1, &c);
  بیعلامت obj_c = جمع_کن_object ("c", 1, &c);

  شروع_object ("d", 1, &c);
  جمع_کن_virtual_جابه_جایی (obj_b, &c);
  بیعلامت obj_d_1 = c.برداشت_pack ();

  شروع_object ("d", 1, &c);
  جمع_کن_virtual_جابه_جایی (obj_c, &c);
  بیعلامت obj_d_2 = c.برداشت_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  شروع_object ("a", 1, &c);
  جمع_کن_جابه_جایی (obj_b, &c);
  جمع_کن_جابه_جایی (obj_c, &c);
  جمع_کن_جابه_جایی (obj_d_1, &c);
  جمع_کن_جابه_جایی (obj_d_2, &c);
  c.برداشت_pack ();
  c.انتها_serialize ();

  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links.درازی == 2);
  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links[1].objidx == obj_c);
  حرر(میانگیر);
}

ساكن فراغ
test_repack_آخرین ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_with_repack_آخرین (&c, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_with_repack_آخرین (&e, خطأ);

  پیاپی_resolve_سرریز_test ("test_repack_last",
                             c,
                             e,
                             20,
                             خطأ,
                             HB_برچسب('a', 'b', 'c', 'd'),
                             درست);

  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ
test_dont_duplicate_virtual ()
{
  size_t میانگیر_اندازه = 200000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (میانگیر);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_virtual (&c, درست);

  فراغ* expected_میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_always_assert (expected_میانگیر);
  hb_serialize_context_t e (expected_میانگیر, میانگیر_اندازه);
  populate_serializer_virtual (&e, خطأ);

  پیاپی_resolve_سرریز_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             خطأ,
                             HB_برچسب('a', 'b', 'c', 'd'),
                             درست);

  حرر (میانگیر);
  حرر (expected_میانگیر);
}

ساكن فراغ
test_deep_گراف_traversal ()
{
  size_t میانگیر_اندازه = 1000000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);

  c.شروع_serialize ();
  بیعلامت prev = جمع_کن_object ("leaf", 4, &c);
  لكل (بیعلامت i = 0; i < 2000; i++)
  {
    شروع_object ("node", 4, &c);
    جمع_کن_جابه_جایی (prev, &c);
    prev = c.برداشت_pack (خطأ);
  }
  c.انتها_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // Test find_subgraph (set)
  hb_مجموعه_t دیدهشده_مجموعه;
  hb_always_assert (گراف.پیدا_کن_subgraph (گراف.ریشه_idx (), دیدهشده_مجموعه).is_ok ());
  hb_always_assert (دیدهشده_مجموعه.get_population () == 2001);

  // Test find_subgraph_size
  hb_مجموعه_t اندازه_مجموعه;
  size_t sz = گراف.پیدا_کن_subgraph_اندازه (گراف.ریشه_idx (), اندازه_مجموعه).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.مجموعه (گراف.ریشه_idx (), 1);
  hb_always_assert (گراف.پیدا_کن_subgraph (گراف.ریشه_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (گراف.تخصیص_بده_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t شاخص_map;
  hb_always_assert (گراف.duplicate_subgraph (گراف.ریشه_idx (), شاخص_map).is_ok ());
  hb_always_assert (شاخص_map.get_population () == 2001);

  حرر (میانگیر);
}

ساكن فراغ
test_32bit_roots_traversal ()
{
  size_t میانگیر_اندازه = 1000;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);

  c.شروع_serialize ();
  بیعلامت leaf1 = جمع_کن_object ("l1", 2, &c);
  بیعلامت leaf2 = جمع_کن_object ("l2", 2, &c);

  شروع_object ("mid", 3, &c);
  جمع_کن_wide_جابه_جایی (leaf1, &c);
  بیعلامت mid = c.برداشت_pack (خطأ);

  شروع_object ("root", 4, &c);
  جمع_کن_جابه_جایی (mid, &c);
  جمع_کن_wide_جابه_جایی (leaf2, &c);
  c.برداشت_pack (خطأ);
  c.انتها_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_مجموعه_t roots;
  hb_always_assert (گراف.پیدا_کن_32bit_roots (گراف.ریشه_idx (), roots).is_ok ());
  hb_always_assert (roots.has (leaf1 - 1));
  hb_always_assert (roots.has (leaf2 - 1));
  hb_always_assert (!roots.has (mid - 1));
  hb_always_assert (roots.get_population () == 2);

  حرر (میانگیر);

  // Test case where DFS fails but BFS succeeds:
  // root -16-> a
  // root -32-> a
  // a -32-> b
  //
  // DFS will first explore the 16 bit link to a and then recurse the 32 bit link from a to b
  // and consider b a root. However, b should not be a root since it's not a top level 32 bit link.
  // BFS will handle this correctly by discovering a as a top level root and then avoiding further
  // exploring it.
  میانگیر_اندازه = 1000;
  میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c2 (میانگیر, میانگیر_اندازه);

  c2.شروع_serialize ();
  بیعلامت b = جمع_کن_object ("b", 1, &c2);

  شروع_object ("a", 1, &c2);
  جمع_کن_wide_جابه_جایی (b, &c2);
  بیعلامت a = c2.برداشت_pack (خطأ);

  شروع_object ("r", 1, &c2);
  جمع_کن_جابه_جایی (a, &c2);
  جمع_کن_wide_جابه_جایی (a, &c2);
  c2.برداشت_pack (خطأ);

  c2.انتها_serialize ();

  گراف_t graph2 = گراف_t::create (c2.object_گراف ()).value ();

  hb_مجموعه_t roots2;
  hb_always_assert (graph2.پیدا_کن_32bit_roots (graph2.ریشه_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  حرر (میانگیر);
}

ساكن فراغ test_invalid_link_objidx ()
{
  size_t میانگیر_اندازه = 100;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_complex_2 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // Poison a link to point to (unsigned) -1
  خودکار links = گراف.vertices_[گراف.ریشه_idx ()].real_links_writer();
  links->objidx = (بیعلامت) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!گراف.مرتب_سازی_کوتاهترین_فاصله ().is_ok ());

  حرر (میانگیر);
}

ساكن فراغ test_invalid_link_جمع_کن_and_move ()
{
  size_t میانگیر_اندازه = 100;
  فراغ* میانگیر = حافظه_بده (میانگیر_اندازه);
  hb_serialize_context_t c (میانگیر, میانگیر_اندازه);
  populate_serializer_complex_2 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!گراف.جمع_کن_link (&dummy, 0, (بیعلامت) -1).is_ok ());

  // move_child with non-existent offset
  خودکار res = گراف.move_فرزند (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  حرر (میانگیر);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

صحيح
اصلی (صحيح argc, حرف **argv)
{
  test_invalid_link_objidx ();
  test_invalid_link_جمع_کن_and_move ();
  test_serialize ();
  test_مرتب_سازی_کوتاهترین ();
  test_will_سرریز_1 ();
  test_will_سرریز_2 ();
  test_will_سرریز_3 ();
  test_resolve_overflows_از_طریق_مرتب_سازی ();
  test_resolve_overflows_از_طریق_duplication ();
  test_resolve_overflows_از_طریق_مضرب_duplications ();
  test_resolve_overflows_از_طریق_اولویت ();
  test_resolve_overflows_از_طریق_space_assignment ();
  test_resolve_overflows_از_طریق_isolation ();
  test_resolve_overflows_از_طریق_isolation_with_recursive_duplication ();
  test_resolve_overflows_از_طریق_isolation_spaces ();
  test_resolve_overflows_از_طریق_isolating_16bit_space ();
  test_resolve_overflows_از_طریق_isolating_16bit_space_2 ();
  test_resolve_overflows_از_طریق_splitting_spaces ();
  test_resolve_overflows_از_طریق_splitting_spaces_2 ();
  test_resolve_mixed_overflows_از_طریق_isolation_spaces ();
  test_duplicate_برگ ();
  test_duplicate_interior ();
  test_virtual_link ();
  test_repack_آخرین();
  test_shared_گره_with_virtual_links ();
  test_resolve_with_extension_promotion ();
  test_resolve_with_shared_extension_promotion ();
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  test_resolve_with_extension_promotion_1_2 ();
#پایان_اگر
  test_resolve_with_repeated_extension_promotion ();
  test_resolve_with_basic_pair_pos_1_تقسیم ();
  test_resolve_with_extension_pair_pos_1_تقسیم ();
  test_resolve_with_basic_pair_pos_2_تقسیم ();
  test_resolve_with_pair_pos_2_تقسیم_with_device_tables ();
  test_resolve_with_close_مقصد_حد_pair_pos_2_تقسیم ();
  test_resolve_with_basic_mark_پایه_pos_1_تقسیم ();
  test_resolve_with_basic_liga_تقسیم ();
  test_resolve_with_liga_تقسیم_move ();
  test_dont_duplicate_virtual ();
  test_resolve_with_liga_تقسیم_overlapping_clone ();
  test_resolve_with_liga_تقسیم_shared_جدول ();
  test_resolve_with_liga_تقسیم_shared_coverage ();
  test_deep_گراف_traversal ();
  test_32bit_roots_traversal ();

  // TODO(grieger): have run overflow tests compare graph equality not final packed binary.
  // TODO(grieger): split test where multiple subtables in one lookup are split to test link ordering.
  // TODO(grieger): split test where coverage table in subtable that is being split is shared.
  // TODO(grieger): test with extensions already mixed in as well.
  // TODO(grieger): test two layer ext promotion setup.
  // TODO(grieger): test sorting by subtables per byte in ext. promotion.
}
