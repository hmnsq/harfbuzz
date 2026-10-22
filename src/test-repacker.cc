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

#شامل <cstdint>
#شامل <string>

#شامل "hb-repacker.hh"
#شامل "hb-open-type.hh"
#شامل "graph/serialize.hh"

ساکن خالی extend (ثابت حرف* value,
                    غیرعلامتی len,
                    hb_serialize_context_t* c)
{
  حرف* obj = c->allocate_سائز<حرف> (len);
  hb_memcpy (obj, value, len);
}

ساکن خالی آغاز_object(ثابت حرف* لیبل_2,
                         غیرعلامتی len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (لیبل_2, len, c);
}

ساکن غیرعلامتی جوڑو_object(ثابت حرف* لیبل_2,
                           غیرعلامتی len,
                           hb_serialize_context_t* c,
                           منطقی shared = جھوٹ)
{
  آغاز_object (لیبل_2, len, c);
  واپس c->نکالو_pack (shared);
}


ساکن خالی جوڑو_انحراف (غیرعلامتی id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* انحراف = c->آغاز_embed<OT::Offset16> ();
  c->extend_min (انحراف);
  c->جوڑو_link (*انحراف, id);
}

ساکن خالی جوڑو_24_انحراف (غیرعلامتی id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* انحراف = c->آغاز_embed<OT::Offset24> ();
  c->extend_min (انحراف);
  c->جوڑو_link (*انحراف, id);
}

ساکن خالی جوڑو_wide_انحراف (غیرعلامتی id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* انحراف = c->آغاز_embed<OT::Offset32> ();
  c->extend_min (انحراف);
  c->جوڑو_link (*انحراف, id);
}

ساکن خالی جوڑو_gsubgpos_سرخی (غیرعلامتی lookup_list,
                                 hb_serialize_context_t* c)
{
  حرف سرخی[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  آغاز_object (سرخی, 8, c);
  جوڑو_انحراف (lookup_list, c);
  c->نکالو_pack (جھوٹ);
}

ساکن خالی جوڑو_gsubgpos_سرخی_1_2 (غیرعلامتی lookup_list,
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

  آغاز_object (سرخی, حجم (سرخی), c);
  جوڑو_wide_انحراف (lookup_list, c);
  c->نکالو_pack (جھوٹ);
}

ساکن غیرعلامتی جوڑو_lookup_list (ثابت غیرعلامتی* lookups,
                                 حرف گنتی,
                                 hb_serialize_context_t* c)
{
  حرف lookup_گنتی[] = {0, گنتی};
  آغاز_object  ((حرف *) &lookup_گنتی, 2, c);

  برائے (صحیح i = 0; i < گنتی; i++)
    جوڑو_انحراف (lookups[i], c);

  واپس c->نکالو_pack (جھوٹ);
}

ساکن غیرعلامتی جوڑو_lookup_list_1_2 (ثابت غیرعلامتی* lookups,
                                     حرف گنتی,
                                     hb_serialize_context_t* c)
{
  حرف lookup_گنتی[] = {0, گنتی};
  آغاز_object ((حرف *) &lookup_گنتی, 2, c);

  برائے (صحیح i = 0; i < گنتی; i++)
    جوڑو_wide_انحراف (lookups[i], c);

  واپس c->نکالو_pack (جھوٹ);
}

ساکن خالی آغاز_lookup (int8_t type,
                          int8_t عدد_subtables,
                          hb_serialize_context_t* c)
{
  حرف lookup[] = {
    0, (حرف)type, // type
    0, (حرف)2, // flag
    0, (حرف)عدد_subtables, // num subtables
  };

  آغاز_object (lookup, 6, c);
}

ساکن غیرعلامتی اختتام_lookup (hb_serialize_context_t* c)
{
  حرف filter[] = {0, 0};
  extend (filter, 2, c);
  واپس c->نکالو_pack (جھوٹ);
}

ساکن غیرعلامتی جوڑو_extension (غیرعلامتی بچہ,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  حرف ext[] = {
    0, 1,
    0, (حرف) type,
  };

  آغاز_object (ext, 4, c);
  جوڑو_wide_انحراف (بچہ, c);

  واپس c->نکالو_pack (جھوٹ);

}

// Adds coverage table fro [start, end]
ساکن غیرعلامتی جوڑو_coverage (غیرعلامتی آغاز, غیرعلامتی انتہا,
                              hb_serialize_context_t* c,
                              منطقی shared = جھوٹ)
{
  اگر (انتہا - آغاز == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //گنتی

      (uint8_t) ((آغاز >> 8) & 0xFF),
      (uint8_t) (آغاز & 0xFF), // glyph[0]
    };
    واپس جوڑو_object ((حرف*) coverage, 6, c, shared);
  }

  اگر (انتہا - آغاز == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //گنتی

      (uint8_t) ((آغاز >> 8) & 0xFF),
      (uint8_t) (آغاز & 0xFF), // glyph[0]

      (uint8_t) ((انتہا >> 8) & 0xFF),
      (uint8_t) (انتہا & 0xFF), // glyph[1]
    };
    واپس جوڑو_object ((حرف*) coverage, 8, c, shared);
  }

  uint8_t coverage[] = {
    0, 2, // format
    0, 1, // range count

    (uint8_t) ((آغاز >> 8) & 0xFF),
    (uint8_t) (آغاز & 0xFF), //آغاز

    (uint8_t) ((انتہا >> 8) & 0xFF),
    (uint8_t) (انتہا & 0xFF), //انتہا

    0, 0,
  };
  واپس جوڑو_object ((حرف*) coverage, 10, c, shared);
}


سانچہ<قسم_نام It>
ساکن غیرعلامتی جوڑو_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  واپس c->نکالو_pack (جھوٹ);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
ساکن غیرعلامتی جوڑو_class_def (uint16_t آغاز_glyph,
                               uint16_t انتہا_glyph,
                               hb_serialize_context_t* c)
{
  غیرعلامتی گنتی = انتہا_glyph - آغاز_glyph;
  uint8_t سرخی[] = {
    0, 1, // format

    (uint8_t) ((آغاز_glyph >> 8) & 0xFF),
    (uint8_t) (آغاز_glyph & 0xFF), // start_glyph

    (uint8_t) ((گنتی >> 8) & 0xFF),
    (uint8_t) (گنتی & 0xFF), //گنتی
  };

  آغاز_object ((حرف*) سرخی, 6, c);
  برائے (uint16_t i = 1; i <= گنتی; i++)
  {
    uint8_t class_قدر_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //گنتی
    };
    extend ((حرف*) class_قدر_2, 2, c);
  }

  واپس c->نکالو_pack (جھوٹ);
}

ساکن غیرعلامتی جوڑو_pair_pos_1 (غیرعلامتی* pair_sets,
                                حرف گنتی,
                                غیرعلامتی coverage,
                                hb_serialize_context_t* c)
{
  حرف format[] = {
    0, 1
  };

  آغاز_object (format, 2, c);
  جوڑو_انحراف (coverage, c);

  حرف قدر_format[] = {
    0, 0,
    0, 0,
    0, گنتی,
  };
  extend (قدر_format, 6, c);

  برائے (حرف i = 0; i < گنتی; i++)
    جوڑو_انحراف (pair_sets[(غیرعلامتی) i], c);

  واپس c->نکالو_pack (جھوٹ);
}

ساکن خالی جوڑو_liga_سیٹ_سرخی (حرف liga_گنتی,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_گنتی_بائٹیں[] = {
    (uint8_t) (0xFF & (liga_گنتی >> 8)),
    (uint8_t) (0xFF & liga_گنتی),
  };

  آغاز_object ((ثابت حرف*) liga_گنتی_بائٹیں, 2, c);
}

ساکن خالی جوڑو_liga_سرخی (غیرعلامتی coverage, غیرعلامتی liga_سیٹ_گنتی, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  آغاز_object ((ثابت حرف*) format, 2, c);
  جوڑو_انحراف(coverage, c);

  uint8_t liga_سیٹ_گنتی_بائٹیں[] = {
    (uint8_t) (0xFF & (liga_سیٹ_گنتی >> 8)),
    (uint8_t) (0xFF & liga_سیٹ_گنتی),
  };

  extend((ثابت حرف *) liga_سیٹ_گنتی_بائٹیں, 2, c);
}

ساکن غیرعلامتی جوڑو_pair_pos_2 (غیرعلامتی starting_class,
                                غیرعلامتی coverage,
                                غیرعلامتی class_def_1, uint16_t class_def_1_گنتی,
                                غیرعلامتی class_def_2, uint16_t class_def_2_گنتی,
                                غیرعلامتی* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  آغاز_object ((حرف*) format, 2, c);
  جوڑو_انحراف (coverage, c);

  غیرعلامتی عدد_اقدار_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  اگر (device_tables) {
    format2 |= 0x20;
    عدد_اقدار_3 += 1;
  }
  uint8_t قدر_format[] = {
    0, format1,
    0, format2,
  };

  extend ((حرف*) قدر_format, 4, c);

  جوڑو_انحراف (class_def_1, c);
  جوڑو_انحراف (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_گنتی >> 8) & 0xFF),
    (uint8_t) (class_def_1_گنتی & 0xFF),
    (uint8_t) ((class_def_2_گنتی >> 8) & 0xFF),
    (uint8_t) (class_def_2_گنتی & 0xFF),
  };
  extend ((حرف*) class_counts, 4, c);

  غیرعلامتی عدد_بائٹیں_فی_ریکارڈ = class_def_2_گنتی * عدد_اقدار_3 * 2;
  uint8_t* ریکارڈ = (uint8_t*) صفر_حافظہ_دو (1, عدد_بائٹیں_فی_ریکارڈ);
  صحیح device_اشاریہ = 0;
  برائے (uint16_t i = 0; i < class_def_1_گنتی; i++)
  {

    برائے (uint16_t j = 0; j < class_def_2_گنتی; j++)
    {
      برائے (صحیح k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((حرف*) value, 2, c);
      }

      اگر (device_tables) {
        جوڑو_انحراف (device_tables[device_اشاریہ++], c);
      }
    }
  }
  آزاد (ریکارڈ);

  واپس c->نکالو_pack (جھوٹ);
}

ساکن غیرعلامتی جوڑو_mark_بنیاد_pos_1 (غیرعلامتی mark_coverage,
                                     غیرعلامتی بنیاد_coverage,
                                     غیرعلامتی mark_array,
                                     غیرعلامتی بنیاد_array,
                                     غیرعلامتی class_گنتی,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  آغاز_object ((حرف*) format, 2, c);
  جوڑو_انحراف (mark_coverage, c);
  جوڑو_انحراف (بنیاد_coverage, c);

  uint8_t گنتی[] = {
    (uint8_t) ((class_گنتی >> 8) & 0xFF),
    (uint8_t) (class_گنتی & 0xFF),
  };
  extend ((حرف*) گنتی, 2, c);

  جوڑو_انحراف (mark_array, c);
  جوڑو_انحراف (بنیاد_array, c);

  واپس c->نکالو_pack (جھوٹ);
}

سانچہ<صحیح mark_گنتی,
    صحیح class_گنتی,
    صحیح بنیاد_گنتی,
    صحیح جدول_گنتی_2>
ساخت Mark_بنیاد_Pos_Buffers
{
  غیرعلامتی بنیاد_anchors[class_گنتی * بنیاد_گنتی];
  غیرعلامتی mark_anchors[mark_گنتی];
  uint8_t anchor_buffers[class_گنتی * بنیاد_گنتی + 100];
  uint8_t class_بفر[class_گنتی * 2];

  Mark_بنیاد_Pos_Buffers(hb_serialize_context_t* c)
  {
    برائے (غیرعلامتی i = 0; i < حجم(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    برائے (غیرعلامتی i = 0; i < class_گنتی * بنیاد_گنتی; i++)
    {
      بنیاد_anchors[i] = جوڑو_object ((حرف*) &anchor_buffers[i], 100, c);
      اگر (i < class_گنتی) {
        class_بفر[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_بفر[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    برائے (غیرعلامتی i = 0; i < mark_گنتی; i++)
    {
      mark_anchors[i] = جوڑو_object ((حرف*) &anchor_buffers[i], 4, c);
    }
  }

  غیرعلامتی create_mark_بنیاد_pos_1 (غیرعلامتی جدول_اشاریہ, hb_serialize_context_t* c)
  {
    غیرعلامتی class_فی_جدول = class_گنتی / جدول_گنتی_2;
    غیرعلامتی mark_فی_class = mark_گنتی / class_گنتی;
    غیرعلامتی آغاز_class = class_فی_جدول * جدول_اشاریہ;
    غیرعلامتی انتہا_class = class_فی_جدول * (جدول_اشاریہ + 1) - 1;

    // baseArray
    uint8_t بنیاد_گنتی_بفر[] = {
      (uint8_t) ((بنیاد_گنتی >> 8) & 0xFF),
      (uint8_t) (بنیاد_گنتی & 0xFF),

    };
    آغاز_object ((حرف*) بنیاد_گنتی_بفر, 2, c);
    برائے (غیرعلامتی بنیاد = 0; بنیاد < بنیاد_گنتی; بنیاد++)
    {
      برائے (غیرعلامتی klass = آغاز_class; klass <= انتہا_class; klass++)
      {
        غیرعلامتی i = بنیاد * class_گنتی + klass;
        جوڑو_انحراف (بنیاد_anchors[i], c);
      }
    }
    غیرعلامتی بنیاد_array = c->نکالو_pack (جھوٹ);

    // markArray
    غیرعلامتی عدد_marks = class_فی_جدول * mark_فی_class;
    uint8_t mark_گنتی_بفر[] = {
      (uint8_t) ((عدد_marks >> 8) & 0xFF),
      (uint8_t) (عدد_marks & 0xFF),
    };
    آغاز_object ((حرف*) mark_گنتی_بفر, 2, c);
    برائے (غیرعلامتی mark = 0; mark < mark_گنتی; mark++)
    {
      غیرعلامتی klass = mark % class_گنتی;
      اگر (klass < آغاز_class || klass > انتہا_class) جاری;
      klass -= آغاز_class;

      extend ((حرف*) &class_بفر[2 * klass], 2, c);
      جوڑو_انحراف (mark_anchors[mark], c);
    }
    غیرعلامتی mark_array = c->نکالو_pack (جھوٹ);

    // markCoverage
    خودکار it =
        + hb_range ((hb_codepoint_t) mark_گنتی)
        | hb_filter ([&] (hb_codepoint_t mark) {
          غیرعلامتی klass = mark % class_گنتی;
          واپس klass >= class_فی_جدول * جدول_اشاریہ &&
              klass < class_فی_جدول * (جدول_اشاریہ + 1);
        })
        ;
    غیرعلامتی mark_coverage = جوڑو_coverage (it, c);

    // baseCoverage
    غیرعلامتی بنیاد_coverage = جوڑو_coverage (10, 10 + بنیاد_گنتی - 1, c);

    واپس جوڑو_mark_بنیاد_pos_1 (mark_coverage,
                                بنیاد_coverage,
                                mark_array,
                                بنیاد_array,
                                class_فی_جدول,
                                c);
  }
};

ساکن خالی تسلسل_resolve_حد_سے_تجاوز_test (ثابت حرف* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       غیرعلامتی عدد_iterations = 0,
                                       منطقی recalculate_extensions = جھوٹ,
                                       hb_لیبل_t لیبل_2 = HB_لیبل ('G', 'S', 'U', 'B'),
                                       منطقی check_ثنائی_equivalence = جھوٹ)
{
  لکھو (">>> Testing overflowing resolution for %s\n",
          name);

  گراف_t گراف = گراف_t::create (overflowing.object_گراف ()).value ();

  گراف_t expected_گراف = گراف_t::create (expected.object_گراف ()).value ();
  اگر (گراف::will_حد_سے_تجاوز (expected_گراف).value ())
  {
    اگر (check_ثنائی_equivalence) {
      لکھو("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_ثنائی_equivalence);
    }
    hb_always_assert (expected_گراف.تفویض_spaces ().is_ok ());
    hb_always_assert (expected_گراف.ترتیب_دو_مختصر_ترین_فاصلہ ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.انحراف_حد_سے_تجاوز ());
  خودکار r = hb_resolve_گراف_overflows (لیبل_2,
                                      عدد_iterations,
                                      recalculate_extensions,
                                      گراف);
  اگر (r.is_err())
    لکھو("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", گراف::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = گراف::serialize (گراف).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = گراف::serialize (expected_گراف).value ();
  hb_always_assert (out2);
  اگر (check_ثنائی_equivalence) {
    غیرعلامتی l1, l2;
    ثابت حرف* d1 = hb_blob_get_ڈیٹا(out1, &l1);
    ثابت حرف* d2 = hb_blob_get_ڈیٹا(out2, &l2);

    منطقی مطابقت = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    اگر (!مطابقت) {
      لکھو("## Result:\n");
      گراف.print();
      لکھو("## Expected:\n");
      expected_گراف.print();
      hb_always_assert(مطابقت);
    }
  }

  hb_blob_destroy (out1);
  hb_blob_destroy (out2);

  // Check the graphs are equivalent
  گراف.normalize ();
  expected_گراف.normalize ();
  اگر (!(گراف == expected_گراف)) {
    لکھو("## Expected:\n");
    expected_گراف.print();
    لکھو("## Result:\n");
    گراف.print();
  }
  hb_always_assert (گراف == expected_گراف);
}

ساکن خالی جوڑو_virtual_انحراف (غیرعلامتی id,
                                hb_serialize_context_t* c)
{
  c->جوڑو_virtual_link (id);
}

ساکن خالی
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_1 = جوڑو_object ("ghi", 3, c);
  غیرعلامتی obj_2 = جوڑو_object ("def", 3, c);

  آغاز_object ("abc", 3, c);
  جوڑو_انحراف (obj_2, c);
  جوڑو_انحراف (obj_1, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_virtual (hb_serialize_context_t* c, منطقی with_حد_سے_تجاوز)
{
  std::string large_string(50000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_4, obj_5;
  اگر (with_حد_سے_تجاوز) {
    obj_5 = جوڑو_object("55555", 5, c);
    obj_4 = جوڑو_object(large_string.c_str(), 50000, c);
  } ورنہ {
    obj_4 = جوڑو_object(large_string.c_str(), 50000, c);
    obj_5 = جوڑو_object("55555", 5, c);
  }

  آغاز_object(large_string.c_str(), 20000, c);
  جوڑو_انحراف(obj_5, c);
  غیرعلامتی obj_3 = c->نکالو_pack(جھوٹ);

  آغاز_object("2", 2, c);
  جوڑو_virtual_انحراف(obj_5, c);
  غیرعلامتی obj_2 = c->نکالو_pack(جھوٹ);

  // obj 1
  آغاز_object("1", 1, c);
  جوڑو_انحراف(obj_2, c);
  جوڑو_انحراف(obj_3, c);
  جوڑو_انحراف(obj_4, c);
  c->نکالو_pack(جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_حد_سے_تجاوز (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_1 = جوڑو_object (large_string.c_str(), 10000, c);
  غیرعلامتی obj_2 = جوڑو_object (large_string.c_str(), 20000, c);
  غیرعلامتی obj_3 = جوڑو_object (large_string.c_str(), 50000, c);

  آغاز_object ("abc", 3, c);
  جوڑو_انحراف (obj_3, c);
  جوڑو_انحراف (obj_2, c);
  جوڑو_انحراف (obj_1, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_ترجیح_حد_سے_تجاوز (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_e = جوڑو_object ("e", 1, c);
  غیرعلامتی obj_d = جوڑو_object ("d", 1, c);

  آغاز_object (large_string.c_str (), 50000, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str (), 20000, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_انحراف (obj_b, c);
  جوڑو_انحراف (obj_c, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_ترجیح_حد_سے_تجاوز_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_e = جوڑو_object ("e", 1, c);

  آغاز_object (large_string.c_str (), 50000, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  غیرعلامتی obj_d = جوڑو_object ("d", 1, c);

  آغاز_object (large_string.c_str (), 20000, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_انحراف (obj_b, c);
  جوڑو_انحراف (obj_c, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}


ساکن خالی
populate_serializer_with_dedup_حد_سے_تجاوز (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_1 = جوڑو_object ("def", 3, c);

  آغاز_object (large_string.c_str(), 60000, c);
  جوڑو_انحراف (obj_1, c);
  غیرعلامتی obj_2 = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 10000, c);
  جوڑو_انحراف (obj_2, c);
  جوڑو_انحراف (obj_1, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_مضرب_dedup_حد_سے_تجاوز (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی برگ = جوڑو_object("def", 3, c);

  constexpr غیرعلامتی عدد_mid_nodes = 20;
  غیرعلامتی mid_nodes[عدد_mid_nodes];
  برائے (غیرعلامتی i = 0; i < عدد_mid_nodes; i++) {
    آغاز_object(large_string.c_str(), 10000 + i, c);
    جوڑو_انحراف(برگ, c);
    mid_nodes[i] = c->نکالو_pack(جھوٹ);
  }

  آغاز_object("abc", 3, c);
  برائے (غیرعلامتی i = 0; i < عدد_mid_nodes; i++) {
    جوڑو_wide_انحراف(mid_nodes[i], c);
  }
  c->نکالو_pack(جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_isolation_حد_سے_تجاوز (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_4 = جوڑو_object ("4", 1, c);

  آغاز_object (large_string.c_str(), 60000, c);
  جوڑو_انحراف (obj_4, c);
  غیرعلامتی obj_3 = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 10000, c);
  جوڑو_انحراف (obj_4, c);
  غیرعلامتی obj_2 = c->نکالو_pack (جھوٹ);

  آغاز_object ("1", 1, c);
  جوڑو_wide_انحراف (obj_3, c);
  جوڑو_انحراف (obj_2, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_isolation_حد_سے_تجاوز_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_f = جوڑو_object ("f", 1, c);

  آغاز_object ("e", 1, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object ("d", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 60000, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_h = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 60000, c);
  جوڑو_انحراف (obj_c, c);
  جوڑو_انحراف (obj_h, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 10000, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_g = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 11000, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_i = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_wide_انحراف (obj_b, c);
  جوڑو_انحراف (obj_g, c);
  جوڑو_انحراف (obj_i, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_isolation_حد_سے_تجاوز_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();


  // space 1

  غیرعلامتی obj_f_مفرد = جوڑو_object ("f", 1, c);

  آغاز_object ("e", 1, c);
  جوڑو_انحراف (obj_f_مفرد, c);
  غیرعلامتی obj_e_مفرد = c->نکالو_pack (جھوٹ);

  آغاز_object ("d", 1, c);
  جوڑو_انحراف (obj_e_مفرد, c);
  غیرعلامتی obj_d_مفرد = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 60000, c);
  جوڑو_انحراف (obj_d_مفرد, c);
  غیرعلامتی obj_h = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_e_مفرد, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 60000, c);
  جوڑو_انحراف (obj_c, c);
  جوڑو_انحراف (obj_h, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  // space 0

  غیرعلامتی obj_f = جوڑو_object ("f", 1, c);

  آغاز_object ("e", 1, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);


  آغاز_object ("d", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 11000, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_i = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 10000, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_g = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_wide_انحراف (obj_b, c);
  جوڑو_انحراف (obj_g, c);
  جوڑو_انحراف (obj_i, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_isolation_حد_سے_تجاوز_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_d = جوڑو_object ("f", 1, c);
  غیرعلامتی obj_e = جوڑو_object ("f", 1, c);

  آغاز_object (large_string.c_str(), 60000, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_b = c->نکالو_pack ();

  آغاز_object (large_string.c_str(), 60000, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack ();


  آغاز_object ("a", 1, c);
  جوڑو_wide_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_c, c);
  c->نکالو_pack ();

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_repack_آخری (hb_serialize_context_t* c, منطقی with_حد_سے_تجاوز)
{
  std::string large_string(70000, 'c');
  c->آغاز_serialize<حرف> ();
  c->push();

  // Obj E
  غیرعلامتی obj_e_1, obj_e_2;
  اگر (with_حد_سے_تجاوز) {
    obj_e_1 = جوڑو_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } ورنہ {
    obj_e_2 = جوڑو_object("a", 1, c);
  }

  // Obj D
  c->push();
  جوڑو_انحراف(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  غیرعلامتی obj_d = c->نکالو_pack(جھوٹ);

  جوڑو_انحراف(obj_d, c);
  hb_always_assert(c->آخری_added_بچہ_اشاریہ() == obj_d);

  اگر (!with_حد_سے_تجاوز) {
    obj_e_1 = جوڑو_object("a", 1, c);
  }

  // Obj C
  c->push();
  جوڑو_انحراف(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  غیرعلامتی obj_c = c->نکالو_pack(جھوٹ);

  جوڑو_انحراف(obj_c, c);

  // Obj B
  غیرعلامتی obj_b = جوڑو_object("b", 1, c);
  جوڑو_انحراف(obj_b, c);

  // Obj A
  c->repack_آخری(obj_d);
  c->نکالو_pack(جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_spaces (hb_serialize_context_t* c, منطقی with_حد_سے_تجاوز)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_i;

  اگر (with_حد_سے_تجاوز)
    obj_i = جوڑو_object ("i", 1, c);

  // Space 2
  غیرعلامتی obj_h = جوڑو_object ("h", 1, c);

  آغاز_object (large_string.c_str(), 30000, c);
  جوڑو_انحراف (obj_h, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  // Space 1
  اگر (!with_حد_سے_تجاوز)
    obj_i = جوڑو_object ("i", 1, c);

  آغاز_object (large_string.c_str(), 30000, c);
  جوڑو_انحراف (obj_i, c);
  غیرعلامتی obj_g = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 30000, c);
  جوڑو_انحراف (obj_i, c);
  غیرعلامتی obj_f = c->نکالو_pack (جھوٹ);

  آغاز_object ("d", 1, c);
  جوڑو_انحراف (obj_g, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_wide_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_c, c);
  جوڑو_wide_انحراف (obj_d, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_g = جوڑو_object ("g", 1, c);
  غیرعلامتی obj_h = جوڑو_object ("h", 1, c);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_g, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_h, c);
  غیرعلامتی obj_f = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object ("d", 1, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_e, c);
  جوڑو_انحراف (obj_h, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_c, c);
  جوڑو_wide_انحراف (obj_d, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_g_مفرد = جوڑو_object ("g", 1, c);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_g_مفرد, c);
  غیرعلامتی obj_e_مفرد = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_e_مفرد, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  غیرعلامتی obj_h_مفرد = جوڑو_object ("h", 1, c);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_h_مفرد, c);
  غیرعلامتی obj_f = c->نکالو_pack (جھوٹ);

  آغاز_object ("d", 1, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  غیرعلامتی obj_g = جوڑو_object ("g", 1, c);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_g, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  غیرعلامتی obj_h = جوڑو_object ("h", 1, c);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_e, c);
  جوڑو_انحراف (obj_h, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_c, c);
  جوڑو_wide_انحراف (obj_d, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize ();
}

ساکن خالی
populate_serializer_short_and_wide_subgraph_جڑ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_e = جوڑو_object ("e", 1, c);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_c, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_c, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_c, c);
  جوڑو_wide_انحراف (obj_d, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_short_and_wide_subgraph_جڑ_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_e_مفرد = جوڑو_object ("e", 1, c);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_e_مفرد, c);
  غیرعلامتی obj_c_مفرد = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_c_مفرد, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  غیرعلامتی obj_e = جوڑو_object ("e", 1, c);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);


  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_c, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_c_مفرد, c);
  جوڑو_wide_انحراف (obj_d, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_بانٹو_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_f = جوڑو_object ("f", 1, c);

  آغاز_object (large_string.c_str(), 40000, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 40000, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_wide_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_c, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_بانٹو_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_f = جوڑو_object ("f", 1, c);

  آغاز_object (large_string.c_str(), 40000, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str(), 40000, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_c, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_بانٹو_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_f_مفرد = جوڑو_object ("f", 1, c);

  آغاز_object (large_string.c_str(), 40000, c);
  جوڑو_انحراف (obj_f_مفرد, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  غیرعلامتی obj_f = جوڑو_object ("f", 1, c);

  آغاز_object (large_string.c_str(), 40000, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_wide_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_c, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_بانٹو_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->آغاز_serialize<حرف> ();

  // Space 2

  غیرعلامتی obj_f_double_مفرد = جوڑو_object ("f", 1, c);

  آغاز_object (large_string.c_str(), 40000, c);
  جوڑو_انحراف (obj_f_double_مفرد, c);
  غیرعلامتی obj_d_مفرد = c->نکالو_pack (جھوٹ);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_d_مفرد, c);
  غیرعلامتی obj_b_مفرد = c->نکالو_pack (جھوٹ);

  // Space 1

  غیرعلامتی obj_f_مفرد = جوڑو_object ("f", 1, c);

  آغاز_object (large_string.c_str(), 40000, c);
  جوڑو_انحراف (obj_f_مفرد, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  // Space 0

  غیرعلامتی obj_f = جوڑو_object ("f", 1, c);

  آغاز_object (large_string.c_str(), 40000, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  //جڑ
  آغاز_object ("a", 1, c);
  جوڑو_انحراف (obj_b, c);
  جوڑو_wide_انحراف (obj_b_مفرد, c);
  جوڑو_wide_انحراف (obj_c, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_5 = جوڑو_object ("mn", 2, c);

  غیرعلامتی obj_4 = جوڑو_object ("jkl", 3, c);

  آغاز_object ("ghi", 3, c);
  جوڑو_انحراف (obj_4, c);
  غیرعلامتی obj_3 = c->نکالو_pack (جھوٹ);

  آغاز_object ("def", 3, c);
  جوڑو_انحراف (obj_3, c);
  غیرعلامتی obj_2 = c->نکالو_pack (جھوٹ);

  آغاز_object ("abc", 3, c);
  جوڑو_انحراف (obj_2, c);
  جوڑو_انحراف (obj_4, c);
  جوڑو_انحراف (obj_5, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_6 = جوڑو_object ("opqrst", 6, c);

  غیرعلامتی obj_5 = جوڑو_object ("mn", 2, c);

  آغاز_object ("jkl", 3, c);
  جوڑو_انحراف (obj_6, c);
  غیرعلامتی obj_4 = c->نکالو_pack (جھوٹ);

  آغاز_object ("ghi", 3, c);
  جوڑو_انحراف (obj_4, c);
  غیرعلامتی obj_3 = c->نکالو_pack (جھوٹ);

  آغاز_object ("def", 3, c);
  جوڑو_انحراف (obj_3, c);
  غیرعلامتی obj_2 = c->نکالو_pack (جھوٹ);

  آغاز_object ("abc", 3, c);
  جوڑو_انحراف (obj_2, c);
  جوڑو_انحراف (obj_4, c);
  جوڑو_انحراف (obj_5, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_d = جوڑو_object ("d", 1, c);

  آغاز_object ("b", 1, c);
  جوڑو_انحراف (obj_d, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("e", 1, c);
  جوڑو_virtual_انحراف (obj_b, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  آغاز_object ("c", 1, c);
  جوڑو_انحراف (obj_e, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_انحراف (obj_b, c);
  جوڑو_انحراف (obj_c, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_24_and_32_ثنائی_ہندسہ_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی obj_f = جوڑو_object ("f", 1, c);
  غیرعلامتی obj_g = جوڑو_object ("g", 1, c);
  غیرعلامتی obj_j = جوڑو_object ("j", 1, c);
  غیرعلامتی obj_k = جوڑو_object ("k", 1, c);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_f, c);
  غیرعلامتی obj_c = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_g, c);
  غیرعلامتی obj_d = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_j, c);
  غیرعلامتی obj_h = c->نکالو_pack (جھوٹ);

  آغاز_object (large_string.c_str (), 40000, c);
  جوڑو_انحراف (obj_k, c);
  غیرعلامتی obj_i = c->نکالو_pack (جھوٹ);

  آغاز_object ("e", 1, c);
  جوڑو_wide_انحراف (obj_h, c);
  جوڑو_wide_انحراف (obj_i, c);
  غیرعلامتی obj_e = c->نکالو_pack (جھوٹ);

  آغاز_object ("b", 1, c);
  جوڑو_24_انحراف (obj_c, c);
  جوڑو_24_انحراف (obj_d, c);
  جوڑو_24_انحراف (obj_e, c);
  غیرعلامتی obj_b = c->نکالو_pack (جھوٹ);

  آغاز_object ("a", 1, c);
  جوڑو_24_انحراف (obj_b, c);
  c->نکالو_pack (جھوٹ);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              صحیح عدد_extensions = 0,
                                              منطقی shared_subtables = جھوٹ,
                                              منطقی نسخہ_1_2 = جھوٹ)
{
  constexpr صحیح عدد_lookups = 5;
  constexpr صحیح عدد_subtables = عدد_lookups * 2;
  غیرعلامتی صحیح lookups[عدد_lookups];
  غیرعلامتی صحیح subtables[عدد_subtables];
  غیرعلامتی صحیح extensions[عدد_subtables];

  std::string large_string(60000, 'a');
  c->آغاز_serialize<حرف> ();


  برائے (صحیح i = عدد_subtables - 1; i >= 0; i--)
    subtables[i] = جوڑو_object(large_string.c_str (), 15000 + i, c);

  برائے (صحیح i = عدد_subtables - 1;
       i >= (عدد_lookups - عدد_extensions) * 2;
       i--)
  {
    extensions[i] = جوڑو_extension (subtables[i], 5, c);
  }

  برائے (صحیح i = عدد_lookups - 1; i >= 0; i--)
  {
    منطقی is_ext = (i >= (عدد_lookups - عدد_extensions));

    آغاز_lookup (is_ext ? (حرف) 7 : (حرف) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    اگر (is_ext) {
      اگر (shared_subtables && i > 2) {
        جوڑو_انحراف (extensions[i * 2 - 1], c);
      }
      جوڑو_انحراف (extensions[i * 2], c);
      جوڑو_انحراف (extensions[i * 2 + 1], c);
    } ورنہ {
      اگر (shared_subtables && i > 2) {
        جوڑو_انحراف (subtables[i * 2 - 1], c);
      }
      جوڑو_انحراف (subtables[i * 2], c);
      جوڑو_انحراف (subtables[i * 2 + 1], c);
    }

    lookups[i] = اختتام_lookup (c);
  }

  غیرعلامتی lookup_list = نسخہ_1_2
                         ? جوڑو_lookup_list_1_2 (lookups, عدد_lookups, c)
                         : جوڑو_lookup_list (lookups, عدد_lookups, c);

  اگر (نسخہ_1_2)
    جوڑو_gsubgpos_سرخی_1_2 (lookup_list, c);
  ورنہ
    جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->انتہا_serialize();
}

سانچہ<صحیح عدد_pair_pos_1, صحیح عدد_pair_سیٹ>
ساکن خالی
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           منطقی as_extension = جھوٹ)
{
  std::string large_string(60000, 'a');
  c->آغاز_serialize<حرف> ();

  constexpr صحیح کل_pair_سیٹ = عدد_pair_pos_1 * عدد_pair_سیٹ;
  غیرعلامتی pair_سیٹ_2[کل_pair_سیٹ];
  غیرعلامتی coverage[عدد_pair_pos_1];
  غیرعلامتی pair_pos_1[عدد_pair_pos_1];

  برائے (صحیح i = عدد_pair_pos_1 - 1; i >= 0; i--)
  {
    برائے (صحیح j = (i + 1) * عدد_pair_سیٹ - 1; j >= i * عدد_pair_سیٹ; j--)
      pair_سیٹ_2[j] = جوڑو_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = جوڑو_coverage (i * عدد_pair_سیٹ,
                                (i + 1) * عدد_pair_سیٹ - 1, c);

    pair_pos_1[i] = جوڑو_pair_pos_1 (&pair_سیٹ_2[i * عدد_pair_سیٹ],
                                    عدد_pair_سیٹ,
                                    coverage[i],
                                    c);
  }

  غیرعلامتی pair_pos_2 = جوڑو_object (large_string.c_str(), 200, c);

  اگر (as_extension) {
    pair_pos_2 = جوڑو_extension (pair_pos_2, 2, c);
    برائے (صحیح i = عدد_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = جوڑو_extension (pair_pos_1[i], 2, c);
  }

  آغاز_lookup (as_extension ? 9 : 2, 1 + عدد_pair_pos_1, c);

  برائے (صحیح i = 0; i < عدد_pair_pos_1; i++)
    جوڑو_انحراف (pair_pos_1[i], c);
  جوڑو_انحراف (pair_pos_2, c);

  غیرعلامتی lookup = اختتام_lookup (c);

  غیرعلامتی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->انتہا_serialize();
}

سانچہ<صحیح عدد_pair_pos_2, صحیح عدد_class_1, صحیح عدد_class_2>
ساکن خالی
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           منطقی as_extension = جھوٹ,
                                           منطقی with_device_tables = جھوٹ,
                                           منطقی extra_جدول = سچ)
{
  std::string large_string(100000, 'a');
  c->آغاز_serialize<حرف> ();

  غیرعلامتی coverage[عدد_pair_pos_2];
  غیرعلامتی class_def_1[عدد_pair_pos_2];
  غیرعلامتی class_def_2[عدد_pair_pos_2];
  غیرعلامتی pair_pos_2[عدد_pair_pos_2];

  غیرعلامتی* device_tables = (غیرعلامتی*) صفر_حافظہ_دو (عدد_pair_pos_2 * عدد_class_1 * عدد_class_2,
                                                حجم(غیرعلامتی));

  // Total glyphs = num_class_1 * num_pair_pos_2
  برائے (صحیح i = عدد_pair_pos_2 - 1; i >= 0; i--)
  {
    غیرعلامتی آغاز_glyph = 5 + i * عدد_class_1;
    اگر (عدد_class_2 >= عدد_class_1)
    {
      class_def_2[i] = جوڑو_class_def (11,
                                      10 + عدد_class_2, c);
      class_def_1[i] = جوڑو_class_def (آغاز_glyph + 1,
                                      آغاز_glyph + عدد_class_1,
                                      c);
    } ورنہ {
      class_def_1[i] = جوڑو_class_def (آغاز_glyph + 1,
                                      آغاز_glyph + عدد_class_1,
                                      c);
      class_def_2[i] = جوڑو_class_def (11,
                                      10 + عدد_class_2, c);
    }

    coverage[i] = جوڑو_coverage (آغاز_glyph,
                                آغاز_glyph + عدد_class_1 - 1,
                                c);

    اگر (with_device_tables)
    {
      برائے(صحیح j = (i + 1) * عدد_class_1 * عدد_class_2 - 1;
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
                                    : خالی_اشاریہ,
                                    c);
  }


  غیرعلامتی pair_pos_1 = 0;
  اگر (extra_جدول) pair_pos_1 = جوڑو_object (large_string.c_str(), 100000, c);

  اگر (as_extension) {
    برائے (صحیح i = عدد_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = جوڑو_extension (pair_pos_2[i], 2, c);

    اگر (extra_جدول)
      pair_pos_1 = جوڑو_extension (pair_pos_1, 2, c);
  }

  آغاز_lookup (as_extension ? 9 : 2, 1 + عدد_pair_pos_2, c);

  اگر (extra_جدول)
    جوڑو_انحراف (pair_pos_1, c);

  برائے (صحیح i = 0; i < عدد_pair_pos_2; i++)
    جوڑو_انحراف (pair_pos_2[i], c);

  غیرعلامتی lookup = اختتام_lookup (c);

  غیرعلامتی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->انتہا_serialize();

  آزاد (device_tables);
}

سانچہ<صحیح mark_گنتی,
    صحیح class_گنتی,
    صحیح بنیاد_گنتی,
    صحیح جدول_گنتی_2>
ساکن خالی
populate_serializer_with_large_mark_بنیاد_pos_1 (hb_serialize_context_t* c)
{
  c->آغاز_serialize<حرف> ();

  Mark_بنیاد_Pos_Buffers<mark_گنتی, class_گنتی, بنیاد_گنتی, جدول_گنتی_2> buffers (c);

  غیرعلامتی mark_بنیاد_pos[جدول_گنتی_2];
  برائے (غیرعلامتی i = 0; i < جدول_گنتی_2; i++)
    mark_بنیاد_pos[i] = buffers.create_mark_بنیاد_pos_1 (i, c);

  برائے (صحیح i = 0; i < جدول_گنتی_2; i++)
    mark_بنیاد_pos[i] = جوڑو_extension (mark_بنیاد_pos[i], 4, c);

  آغاز_lookup (9, جدول_گنتی_2, c);

  برائے (صحیح i = 0; i < جدول_گنتی_2; i++)
    جوڑو_انحراف (mark_بنیاد_pos[i], c);

  غیرعلامتی lookup = اختتام_lookup (c);

  غیرعلامتی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->انتہا_serialize();
}

سانچہ<غیرعلامتی Lig_Subst_گنتی,
         غیرعلامتی Liga_سیٹ_گنتی,
         غیرعلامتی Liga_فی_سیٹ_گنتی,
         غیرعلامتی Liga_سائز>
ساکن خالی populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    منطقی sequential_liga_sets,
                                                    منطقی shared,
                                                    غیرعلامتی *liga_subst_id,
                                                    منطقی unique_lig_str = جھوٹ)
{
  حرف ch = 'a';

  غیرعلامتی liga[Liga_سیٹ_گنتی * Liga_فی_سیٹ_گنتی];
  غیرعلامتی liga_سیٹ[Liga_سیٹ_گنتی];
  برائے (غیرعلامتی l = 0; l < Lig_Subst_گنتی; l++) {
    غیرعلامتی coverage_آغاز = 0;
    غیرعلامتی coverage_انتہا = Liga_سیٹ_گنتی - 1;
    اگر (sequential_liga_sets) {
      coverage_آغاز = l * Liga_سیٹ_گنتی;
      coverage_انتہا = (l + 1) * Liga_سیٹ_گنتی - 1;
    }
    غیرعلامتی coverage = جوڑو_coverage(coverage_آغاز, coverage_انتہا, c, shared);

    برائے (غیرعلامتی i = 0; i < Liga_سیٹ_گنتی; i++) {
      برائے (غیرعلامتی j = 0; j < Liga_فی_سیٹ_گنتی; j++)
      {
        std::string large_string(100000, ch);
        آغاز_object (large_string.c_str(), Liga_سائز, c);
        اگر (unique_lig_str)
          ch += 1;
        جوڑو_virtual_انحراف(coverage, c);
        liga[i * Liga_فی_سیٹ_گنتی + j] = c->نکالو_pack (shared);
      }

      جوڑو_liga_سیٹ_سرخی(Liga_فی_سیٹ_گنتی, c);
      جوڑو_virtual_انحراف(coverage, c);
      برائے (غیرعلامتی j = 0; j < Liga_فی_سیٹ_گنتی; j++)
      {
        جوڑو_انحراف(liga[i * Liga_فی_سیٹ_گنتی + j], c);
      }

      liga_سیٹ[i] = c->نکالو_pack(shared);
    }

    جوڑو_liga_سرخی(coverage, Liga_سیٹ_گنتی, c);
    برائے (غیرعلامتی i = 0; i < Liga_سیٹ_گنتی; i++)
    {
      جوڑو_انحراف(liga_سیٹ[i], c);
    }

    liga_subst_id[l] = c->نکالو_pack(shared);
  }

  برائے (غیرعلامتی l = 0; l < Lig_Subst_گنتی; l++) {
    liga_subst_id[l] = جوڑو_extension(liga_subst_id[l], 4, c);
  }
}

سانچہ<غیرعلامتی liga_subst_گنتی,
         غیرعلامتی liga_سیٹ_گنتی,
         غیرعلامتی liga_فی_سیٹ_گنتی,
         غیرعلامتی liga_سائز>
ساکن خالی
populate_serializer_with_large_liga (hb_serialize_context_t* c, منطقی sequential_liga_sets)
{
  c->آغاز_serialize<حرف> ();

  غیرعلامتی liga_subst[liga_subst_گنتی];
  populate_serializer_with_large_ligsubst<liga_subst_گنتی, liga_سیٹ_گنتی, liga_فی_سیٹ_گنتی, liga_سائز>(c, sequential_liga_sets, جھوٹ, liga_subst);

  آغاز_lookup (7, liga_subst_گنتی, c);
  برائے (غیرعلامتی l = 0; l < liga_subst_گنتی; l++) {
    جوڑو_انحراف(liga_subst[l], c);
  }

  غیرعلامتی lookup = اختتام_lookup (c);

  غیرعلامتی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->انتہا_serialize();
}

ساکن خالی
populate_serializer_with_large_liga_overlapping_clone_نتیجہ (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->آغاز_serialize<حرف> ();

  constexpr غیرعلامتی liga_سائز = 30000;

  غیرعلامتی liga[6];
  غیرعلامتی liga_subst[3];
  غیرعلامتی liga_سیٹ[2];

  // LigaSubst 3
  غیرعلامتی coverage = جوڑو_coverage(1, 1, c);
  برائے (صحیح i = 1; i >= 0; i--) {
    آغاز_object (large_string.c_str(), liga_سائز, c);
    جوڑو_virtual_انحراف(coverage, c);
    liga[i] = c->نکالو_pack (جھوٹ);
  }

  جوڑو_liga_سیٹ_سرخی(2, c);
  جوڑو_virtual_انحراف(coverage, c);
  برائے (غیرعلامتی i = 0; i < 2; i++)
    جوڑو_انحراف(liga[i], c);
  liga_سیٹ[0] = c->نکالو_pack(جھوٹ);

  جوڑو_liga_سرخی(coverage, 1, c);
  جوڑو_انحراف(liga_سیٹ[0], c);
  liga_subst[2] = c->نکالو_pack(جھوٹ);

  // LigaSubst 2
  coverage = جوڑو_coverage(0, 1, c);
  برائے (صحیح i = 1; i >= 0; i--) {
    آغاز_object (large_string.c_str(), liga_سائز, c);
    جوڑو_virtual_انحراف(coverage, c);
    liga[i] = c->نکالو_pack (جھوٹ);
  }

  جوڑو_liga_سیٹ_سرخی(1, c);
  جوڑو_virtual_انحراف(coverage, c);
  جوڑو_انحراف(liga[1], c);
  liga_سیٹ[1] = c->نکالو_pack(جھوٹ);

  جوڑو_liga_سیٹ_سرخی(1, c);
  جوڑو_virtual_انحراف(coverage, c);
  جوڑو_انحراف(liga[0], c);
  liga_سیٹ[0] = c->نکالو_pack(جھوٹ);

  جوڑو_liga_سرخی(coverage, 2, c);
  جوڑو_انحراف(liga_سیٹ[0], c);
  جوڑو_انحراف(liga_سیٹ[1], c);
  liga_subst[1] = c->نکالو_pack(جھوٹ);

  // LigaSubst 1
  coverage = جوڑو_coverage(0, 0, c);
  برائے (صحیح i = 1; i >= 0; i--) {
    آغاز_object (large_string.c_str(), liga_سائز, c);
    جوڑو_virtual_انحراف(coverage, c);
    liga[i] = c->نکالو_pack (جھوٹ);
  }

  جوڑو_liga_سیٹ_سرخی(2, c);
  جوڑو_virtual_انحراف(coverage, c);
  برائے (غیرعلامتی i = 0; i < 2; i++)
    جوڑو_انحراف(liga[i], c);
  liga_سیٹ[0] = c->نکالو_pack(جھوٹ);

  جوڑو_liga_سرخی(coverage, 1, c);
  جوڑو_انحراف(liga_سیٹ[0], c);
  liga_subst[0] = c->نکالو_pack(جھوٹ);

  برائے (صحیح l = 2; l >= 0; l--) {
    liga_subst[l] = جوڑو_extension(liga_subst[l], 4, c);
  }

  آغاز_lookup (7, 3, c);
  برائے (غیرعلامتی l = 0; l < 3; l++) {
    جوڑو_انحراف(liga_subst[l], c);
  }

  غیرعلامتی lookup = اختتام_lookup (c);

  غیرعلامتی lookup_list = جوڑو_lookup_list (&lookup, 1, c);

  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->انتہا_serialize();
}

سانچہ<غیرعلامتی liga_subst_گنتی,
         غیرعلامتی liga_سیٹ_گنتی,
         غیرعلامتی liga_فی_سیٹ_گنتی,
         غیرعلامتی liga_سائز>
ساکن خالی
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, منطقی sequential_liga_sets)
{
  c->آغاز_serialize<حرف> ();

  غیرعلامتی lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  غیرعلامتی liga_subst_idx[liga_subst_گنتی];
  populate_serializer_with_large_ligsubst<liga_subst_گنتی, liga_سیٹ_گنتی, liga_فی_سیٹ_گنتی, liga_سائز>(c, sequential_liga_sets, سچ, liga_subst_idx, سچ);

  آغاز_lookup (7, liga_subst_گنتی, c);
  برائے (غیرعلامتی l = 0; l < liga_subst_گنتی; l++) {
    جوڑو_انحراف(liga_subst_idx[l], c);
  }

  lookups[0] = اختتام_lookup (c);

  // Lookup with 2 LigSubst tables
  غیرعلامتی liga_subst[liga_subst_گنتی + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, جھوٹ, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_گنتی, liga_سیٹ_گنتی, liga_فی_سیٹ_گنتی, liga_سائز>(c, sequential_liga_sets, سچ, &liga_subst[1], سچ);

  آغاز_lookup (7, liga_subst_گنتی + 1, c);
  برائے (غیرعلامتی l = 0; l <= liga_subst_گنتی; l++) {
    جوڑو_انحراف(liga_subst[l], c);
  }

  lookups[1] = اختتام_lookup (c);

  غیرعلامتی lookup_list = جوڑو_lookup_list (lookups, 2, c);
  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->انتہا_serialize();
}

سانچہ<غیرعلامتی liga_subst_گنتی,
         غیرعلامتی liga_سیٹ_گنتی,
         غیرعلامتی liga_فی_سیٹ_گنتی,
         غیرعلامتی liga_سائز>
ساکن خالی
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->آغاز_serialize<حرف> ();

  غیرعلامتی liga_subst[liga_subst_گنتی + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, سچ, سچ, liga_subst, سچ);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_گنتی, liga_سیٹ_گنتی, liga_فی_سیٹ_گنتی, liga_سائز>(c, سچ, سچ, &liga_subst[1], سچ);

  آغاز_lookup (7, liga_subst_گنتی + 1, c);
  برائے (غیرعلامتی l = 0; l <= liga_subst_گنتی; l++) {
    جوڑو_انحراف(liga_subst[l], c);
  }

  غیرعلامتی lookup = اختتام_lookup (c);

  غیرعلامتی lookup_list = جوڑو_lookup_list (&lookup, 1, c);
  جوڑو_gsubgpos_سرخی (lookup_list, c);

  c->انتہا_serialize();
}

ساکن خالی test_ترتیب_دو_مختصر_ترین ()
{
  size_t بفر_سائز = 100;
  خالی* بفر_a = حافظہ_دو (بفر_سائز);
  خالی* بفر_e = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t a (بفر_a, بفر_سائز);
  hb_serialize_context_t e (بفر_e, بفر_سائز);
  populate_serializer_complex_2 (&a);

  گراف_t گراف = گراف_t::create (a.object_گراف ()).value ();
  hb_always_assert (گراف.ترتیب_دو_مختصر_ترین_فاصلہ ().is_ok());
  گراف.normalize();

  // Expected graph
  e.آغاز_serialize();
  غیرعلامتی jkl = جوڑو_object ("jkl", 3, &e);

  آغاز_object("ghi", 3, &e);
  جوڑو_انحراف(jkl, &e);
  غیرعلامتی ghi = e.نکالو_pack(جھوٹ);

  آغاز_object("def", 3, &e);
  جوڑو_انحراف(ghi, &e);
  غیرعلامتی def = e.نکالو_pack(جھوٹ);

  غیرعلامتی mn = جوڑو_object("mn", 2, &e);

  آغاز_object ("abc", 3, &e);
  جوڑو_انحراف (def, &e);
  جوڑو_انحراف (jkl, &e);
  جوڑو_انحراف (mn, &e);
  e.نکالو_pack (جھوٹ);
  e.انتہا_serialize();

  گراف_t expected = گراف_t::create (e.object_گراف ()).value ();
  expected.normalize();

  أكد(expected == گراف);

  آزاد (بفر_a);
  آزاد (بفر_e);
}

ساکن خالی test_duplicate_برگ ()
{
  size_t بفر_سائز = 100;
  خالی* بفر_a = حافظہ_دو (بفر_سائز);
  خالی* بفر_e = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t a (بفر_a, بفر_سائز);
  hb_serialize_context_t e (بفر_e, بفر_سائز);
  populate_serializer_complex_2 (&a);

  گراف_t گراف = گراف_t::create (a.object_گراف ()).value ();
  hb_always_assert (گراف.duplicate (4, 1, جھوٹ).is_ok ());
  گراف.normalize();

  e.آغاز_serialize();
  غیرعلامتی mn = جوڑو_object("mn", 2, &e);
  غیرعلامتی jkl_2 = جوڑو_object("jkl", 3, &e);

  آغاز_object("ghi", 3, &e);
  جوڑو_انحراف(jkl_2, &e);
  غیرعلامتی ghi = e.نکالو_pack(جھوٹ);

  آغاز_object("def", 3, &e);
  جوڑو_انحراف(ghi, &e);
  غیرعلامتی def = e.نکالو_pack(جھوٹ);

  غیرعلامتی jkl_1 = جوڑو_object("jkl", 3, &e);

  آغاز_object("abc", 3, &e);
  جوڑو_انحراف(def, &e);
  جوڑو_انحراف(jkl_1, &e);
  جوڑو_انحراف(mn, &e);
  e.نکالو_pack(جھوٹ);

  گراف_t expected = گراف_t::create (e.object_گراف ()).value ();
  expected.normalize();

  أكد(expected == گراف);

  آزاد (بفر_a);
  آزاد (بفر_e);
}

ساکن خالی test_duplicate_interior ()
{
  size_t بفر_سائز = 100;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_complex_3 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();
  hb_always_assert (گراف.duplicate (3, 2, جھوٹ).is_ok ());

  hb_always_assert(لڑی_موازنہ (گراف.object (6).سر_2, "jkl", 3) == 0);
  hb_always_assert(گراف.object (6).real_links.لمبائی == 1);
  hb_always_assert(گراف.object (6).real_links[0].objidx == 0);

  hb_always_assert(لڑی_موازنہ (گراف.object (5).سر_2, "abc", 3) == 0);
  hb_always_assert(گراف.object (5).real_links.لمبائی == 3);
  hb_always_assert(گراف.object (5).real_links[0].objidx == 4);
  hb_always_assert(گراف.object (5).real_links[1].objidx == 2);
  hb_always_assert(گراف.object (5).real_links[2].objidx == 1);

  hb_always_assert(لڑی_موازنہ (گراف.object (4).سر_2, "def", 3) == 0);
  hb_always_assert(گراف.object (4).real_links.لمبائی == 1);
  hb_always_assert(گراف.object (4).real_links[0].objidx == 3);

  hb_always_assert(لڑی_موازنہ (گراف.object (3).سر_2, "ghi", 3) == 0);
  hb_always_assert(گراف.object (3).real_links.لمبائی == 1);
  hb_always_assert(گراف.object (3).real_links[0].objidx == 6);

  hb_always_assert(لڑی_موازنہ (گراف.object (2).سر_2, "jkl", 3) == 0);
  hb_always_assert(گراف.object (2).real_links.لمبائی == 1);
  hb_always_assert(گراف.object (2).real_links[0].objidx == 0);

  hb_always_assert(لڑی_موازنہ (گراف.object (1).سر_2, "mn", 2) == 0);
  hb_always_assert(گراف.object (1).real_links.لمبائی == 0);

  hb_always_assert(لڑی_موازنہ (گراف.object (0).سر_2, "opqrst", 6) == 0);
  hb_always_assert(گراف.object (0).real_links.لمبائی == 0);

  آزاد (بفر);
}

ساکن خالی
test_serialize ()
{
  size_t بفر_سائز = 100;
  خالی* بفر_1 = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c1 (بفر_1, بفر_سائز);
  populate_serializer_simple (&c1);
  hb_بائٹیں_t expected = c1.copy_بائٹیں ();

  گراف_t گراف = گراف_t::create (c1.object_گراف ()).value ();
  hb_blob_t* برآمد = گراف::serialize (گراف).value ();
  آزاد (بفر_1);

  hb_بائٹیں_t actual = برآمد->as_بائٹیں ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (برآمد);
}

ساکن خالی test_will_حد_سے_تجاوز_1 ()
{
  size_t بفر_سائز = 100;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);

  populate_serializer_complex_2 (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (!گراف::will_حد_سے_تجاوز (گراف, خالی_اشاریہ).value ());

  آزاد (بفر);
}

ساکن خالی test_will_حد_سے_تجاوز_2 ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_حد_سے_تجاوز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (گراف::will_حد_سے_تجاوز (گراف, خالی_اشاریہ).value ());

  آزاد (بفر);
}

ساکن خالی test_will_حد_سے_تجاوز_3 ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_dedup_حد_سے_تجاوز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (گراف::will_حد_سے_تجاوز (گراف, خالی_اشاریہ).value ());

  آزاد (بفر);
}

ساکن خالی test_resolve_overflows_بذریعہ_ترتیب_دو ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_حد_سے_تجاوز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* برآمد = hb_resolve_overflows (c.object_گراف (), HB_لیبل_NONE);
  hb_always_assert (برآمد);
  hb_بائٹیں_t نتیجہ = برآمد->as_بائٹیں ();
  hb_always_assert (نتیجہ.لمبائی == (80000 + 3 + 3 * 2));

  آزاد (بفر);
  hb_blob_destroy (برآمد);
}

ساکن خالی test_resolve_overflows_بذریعہ_duplication ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_dedup_حد_سے_تجاوز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* برآمد = hb_resolve_overflows (c.object_گراف (), HB_لیبل_NONE);
  hb_always_assert (برآمد);
  hb_بائٹیں_t نتیجہ = برآمد->as_بائٹیں ();
  hb_always_assert (نتیجہ.لمبائی == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  آزاد (بفر);
  hb_blob_destroy (برآمد);
}

ساکن خالی test_resolve_overflows_بذریعہ_مضرب_duplications ()
{
  size_t بفر_سائز = 300000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_مضرب_dedup_حد_سے_تجاوز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* برآمد = hb_resolve_overflows (c.object_گراف (), HB_لیبل_NONE, 5);
  hb_always_assert (برآمد);

  آزاد (بفر);
  hb_blob_destroy (برآمد);
}

ساکن خالی test_resolve_overflows_بذریعہ_space_assignment ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_spaces (&c, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_spaces (&e, جھوٹ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_overflows_بذریعہ_isolation ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_isolation_حد_سے_تجاوز (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.انحراف_حد_سے_تجاوز ());
  hb_blob_t* برآمد = hb_resolve_overflows (c.object_گراف (), HB_لیبل ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (برآمد);
  hb_بائٹیں_t نتیجہ = برآمد->as_بائٹیں ();
  hb_always_assert (نتیجہ.لمبائی == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  آزاد (بفر);
  hb_blob_destroy (برآمد);
}

ساکن خالی test_resolve_overflows_بذریعہ_isolation_with_recursive_duplication ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_isolation_حد_سے_تجاوز_complex (&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_isolation_حد_سے_تجاوز_complex_expected (&e);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_overflows_بذریعہ_isolating_16bit_space ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_spaces_16bit_connection (&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_spaces_16bit_connection_expected (&e);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_overflows_بذریعہ_isolating_16bit_space_2 ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_short_and_wide_subgraph_جڑ (&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_short_and_wide_subgraph_جڑ_expected (&e);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_overflows_بذریعہ_isolation_spaces ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_isolation_حد_سے_تجاوز_spaces (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.انحراف_حد_سے_تجاوز ());
  hb_blob_t* برآمد = hb_resolve_overflows (c.object_گراف (), HB_لیبل ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (برآمد);
  hb_بائٹیں_t نتیجہ = برآمد->as_بائٹیں ();

  غیرعلامتی expected_لمبائی = 3 + 2 * 60000; // objects
  expected_لمبائی += 2 * 4 + 2 * 2; // links
  hb_always_assert (نتیجہ.لمبائی == expected_لمبائی);

  آزاد (بفر);
  hb_blob_destroy (برآمد);
}

ساکن خالی test_resolve_mixed_overflows_بذریعہ_isolation_spaces ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_24_and_32_ثنائی_ہندسہ_offsets (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.انحراف_حد_سے_تجاوز ());
  hb_blob_t* برآمد = hb_resolve_overflows (c.object_گراف (), HB_لیبل ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (برآمد);
  hb_بائٹیں_t نتیجہ = برآمد->as_بائٹیں ();

  غیرعلامتی expected_لمبائی =
      // Objects
      7 +
      4 * 40000;

  expected_لمبائی +=
      // Links
      2 * 4 +  // 32
      4 * 3 +  // 24
      4 * 2;   // 16

  hb_always_assert (نتیجہ.لمبائی == expected_لمبائی);

  آزاد (بفر);
  hb_blob_destroy (برآمد);
}

ساکن خالی test_resolve_with_extension_promotion ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_extension_promotion (&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_extension_promotion (&e, 3);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             سچ);
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_shared_extension_promotion ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_extension_promotion (&c, 0, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_extension_promotion (&e, 3, سچ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             سچ);
  آزاد (بفر);
  آزاد (expected_بفر);
}

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
ساکن خالی test_resolve_with_extension_promotion_1_2 ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_extension_promotion (&c, 0, جھوٹ, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_extension_promotion (&e, 3, جھوٹ, سچ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             سچ);
  آزاد (بفر);
  آزاد (expected_بفر);
}
#اگر_ختم

ساکن خالی test_resolve_with_repeated_extension_promotion ()
{
  size_t بفر_سائز = 1000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);

  c.آغاز_serialize<حرف> ();
  غیرعلامتی subtable = جوڑو_object ("subt", 4, &c);
  آغاز_lookup (5, 2, &c);
  جوڑو_انحراف (subtable, &c);
  جوڑو_انحراف (subtable, &c);
  غیرعلامتی lookup = اختتام_lookup (&c);
  غیرعلامتی lookup_list = جوڑو_lookup_list (&lookup, 1, &c);
  جوڑو_gsubgpos_سرخی (lookup_list, &c);
  c.انتہا_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();
  خودکار context = گراف::gsubgpos_گراف_context_t::create (HB_لیبل ('G', 'S', 'U', 'B'),
                                                          گراف).value ();
  hb_always_assert (context.lookups.get_population () == 1);
  غیرعلامتی lookup_اشاریہ = *context.lookups.کلیدیں ();
  گراف::Lookup* lookup_جدول_2 = context.lookups.get (lookup_اشاریہ);
  hb_always_assert (lookup_جدول_2->make_extension (context, lookup_اشاریہ).is_ok ());

  ثابت خودکار& lookup_راس = گراف.vertices_[lookup_اشاریہ];
  hb_always_assert (lookup_راس.obj ().real_links.لمبائی == 2);
  غیرعلامتی extension_اشاریہ = lookup_راس.obj ().real_links[0].objidx;
  hb_always_assert (lookup_راس.obj ().real_links[1].objidx == extension_اشاریہ);

  خودکار* extension = گراف.vertices_[extension_اشاریہ].as<
      گراف::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
  غیرعلامتی promoted_subtable = extension->get_subtable_اشاریہ (گراف, extension_اشاریہ).value ();
  hb_always_assert (گراف.vertices_[promoted_subtable].جدول_سائز () == 4);
  hb_always_assert (گراف.update_parents ().is_ok ());
  hb_always_assert (گراف.vertices_[extension_اشاریہ].incoming_کنارے_2 () == 2);
  hb_always_assert (گراف.vertices_[promoted_subtable].incoming_کنارے_2 () == 1);

  hb_blob_t* برآمد = گراف::serialize (گراف).value ();
  hb_blob_destroy (برآمد);
  آزاد (بفر);
}

ساکن خالی test_resolve_with_basic_pair_pos_1_بانٹو ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, سچ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'P', 'O', 'S'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_extension_pair_pos_1_بانٹو ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, سچ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'P', 'O', 'S'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_basic_pair_pos_2_بانٹو ()
{
  size_t بفر_سائز = 300000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, سچ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'P', 'O', 'S'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_close_مقصد_حد_بندی_pair_pos_2_بانٹو ()
{
  size_t بفر_سائز = 300000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, سچ, جھوٹ, جھوٹ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, سچ, جھوٹ, جھوٹ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'P', 'O', 'S'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_pair_pos_2_بانٹو_with_device_tables ()
{
  size_t بفر_سائز = 300000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, جھوٹ, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, سچ, سچ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'P', 'O', 'S'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_basic_mark_بنیاد_pos_1_بانٹو ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_large_mark_بنیاد_pos_1 <40, 10, 110, 1>(&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_large_mark_بنیاد_pos_1 <40, 10, 110, 2>(&e);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'P', 'O', 'S'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_basic_liga_بانٹو ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, جھوٹ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, جھوٹ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'S', 'U', 'B'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_liga_بانٹو_move ()
{
  size_t بفر_سائز = 400000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, سچ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'S', 'U', 'B'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_liga_بانٹو_overlapping_clone ()
{
  size_t بفر_سائز = 400000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_large_liga_overlapping_clone_نتیجہ(&e);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'S', 'U', 'B'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_liga_بانٹو_shared_جدول ()
{
  size_t بفر_سائز = 400000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, سچ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'S', 'U', 'B'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_with_liga_بانٹو_shared_coverage ()
{
  size_t بفر_سائز = 400000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             سچ,
                             HB_لیبل('G', 'S', 'U', 'B'));
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_overflows_بذریعہ_splitting_spaces ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_بانٹو_spaces (&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_بانٹو_spaces_expected (&e);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  آزاد (بفر);
  آزاد (expected_بفر);

}

ساکن خالی test_resolve_overflows_بذریعہ_splitting_spaces_2 ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_بانٹو_spaces_2 (&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_بانٹو_spaces_expected_2 (&e);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی test_resolve_overflows_بذریعہ_ترجیح ()
{
  size_t بفر_سائز = 160000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_ترجیح_حد_سے_تجاوز (&c);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_ترجیح_حد_سے_تجاوز_expected (&e);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  آزاد (بفر);
  آزاد (expected_بفر);
}


ساکن خالی test_virtual_link ()
{
  size_t بفر_سائز = 100;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_virtual_link (&c);

  hb_blob_t* برآمد = hb_resolve_overflows (c.object_گراف (), HB_لیبل_NONE);
  hb_always_assert (برآمد);

  hb_بائٹیں_t نتیجہ = برآمد->as_بائٹیں ();
  hb_always_assert (نتیجہ.لمبائی == 5 + 4 * 2);
  hb_always_assert (نتیجہ[0]  == 'a');
  hb_always_assert (نتیجہ[5]  == 'c');
  hb_always_assert (نتیجہ[8]  == 'e');
  hb_always_assert (نتیجہ[9]  == 'b');
  hb_always_assert (نتیجہ[12] == 'd');

  آزاد (بفر);
  hb_blob_destroy (برآمد);
}

ساکن خالی
test_shared_گرہ_with_virtual_links ()
{
  size_t بفر_سائز = 100;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);

  c.آغاز_serialize<حرف> ();

  غیرعلامتی obj_b = جوڑو_object ("b", 1, &c);
  غیرعلامتی obj_c = جوڑو_object ("c", 1, &c);

  آغاز_object ("d", 1, &c);
  جوڑو_virtual_انحراف (obj_b, &c);
  غیرعلامتی obj_d_1 = c.نکالو_pack ();

  آغاز_object ("d", 1, &c);
  جوڑو_virtual_انحراف (obj_c, &c);
  غیرعلامتی obj_d_2 = c.نکالو_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  آغاز_object ("a", 1, &c);
  جوڑو_انحراف (obj_b, &c);
  جوڑو_انحراف (obj_c, &c);
  جوڑو_انحراف (obj_d_1, &c);
  جوڑو_انحراف (obj_d_2, &c);
  c.نکالو_pack ();
  c.انتہا_serialize ();

  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links.لمبائی == 2);
  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links[1].objidx == obj_c);
  آزاد(بفر);
}

ساکن خالی
test_repack_آخری ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_with_repack_آخری (&c, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_with_repack_آخری (&e, جھوٹ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_repack_last",
                             c,
                             e,
                             20,
                             جھوٹ,
                             HB_لیبل('a', 'b', 'c', 'd'),
                             سچ);

  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی
test_dont_duplicate_virtual ()
{
  size_t بفر_سائز = 200000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_virtual (&c, سچ);

  خالی* expected_بفر = حافظہ_دو (بفر_سائز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائز);
  populate_serializer_virtual (&e, جھوٹ);

  تسلسل_resolve_حد_سے_تجاوز_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             جھوٹ,
                             HB_لیبل('a', 'b', 'c', 'd'),
                             سچ);

  آزاد (بفر);
  آزاد (expected_بفر);
}

ساکن خالی
test_deep_گراف_traversal ()
{
  size_t بفر_سائز = 1000000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);

  c.آغاز_serialize ();
  غیرعلامتی prev = جوڑو_object ("leaf", 4, &c);
  برائے (غیرعلامتی i = 0; i < 2000; i++)
  {
    آغاز_object ("node", 4, &c);
    جوڑو_انحراف (prev, &c);
    prev = c.نکالو_pack (جھوٹ);
  }
  c.انتہا_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // Test find_subgraph (set)
  hb_سیٹ_t دیکھا_سیٹ;
  hb_always_assert (گراف.ڈھونڈو_subgraph (گراف.جڑ_idx (), دیکھا_سیٹ).is_ok ());
  hb_always_assert (دیکھا_سیٹ.get_population () == 2001);

  // Test find_subgraph_size
  hb_سیٹ_t سائز_سیٹ;
  size_t sz = گراف.ڈھونڈو_subgraph_سائز (گراف.جڑ_idx (), سائز_سیٹ).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.سیٹ (گراف.جڑ_idx (), 1);
  hb_always_assert (گراف.ڈھونڈو_subgraph (گراف.جڑ_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (گراف.تفویض_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t اشاریہ_map;
  hb_always_assert (گراف.duplicate_subgraph (گراف.جڑ_idx (), اشاریہ_map).is_ok ());
  hb_always_assert (اشاریہ_map.get_population () == 2001);

  آزاد (بفر);
}

ساکن خالی
test_32bit_roots_traversal ()
{
  size_t بفر_سائز = 1000;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);

  c.آغاز_serialize ();
  غیرعلامتی leaf1 = جوڑو_object ("l1", 2, &c);
  غیرعلامتی leaf2 = جوڑو_object ("l2", 2, &c);

  آغاز_object ("mid", 3, &c);
  جوڑو_wide_انحراف (leaf1, &c);
  غیرعلامتی mid = c.نکالو_pack (جھوٹ);

  آغاز_object ("root", 4, &c);
  جوڑو_انحراف (mid, &c);
  جوڑو_wide_انحراف (leaf2, &c);
  c.نکالو_pack (جھوٹ);
  c.انتہا_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_سیٹ_t roots;
  hb_always_assert (گراف.ڈھونڈو_32bit_roots (گراف.جڑ_idx (), roots).is_ok ());
  hb_always_assert (roots.has (leaf1 - 1));
  hb_always_assert (roots.has (leaf2 - 1));
  hb_always_assert (!roots.has (mid - 1));
  hb_always_assert (roots.get_population () == 2);

  آزاد (بفر);

  // Test case where DFS fails but BFS succeeds:
  // root -16-> a
  // root -32-> a
  // a -32-> b
  //
  // DFS will first explore the 16 bit link to a and then recurse the 32 bit link from a to b
  // and consider b a root. However, b should not be a root since it's not a top level 32 bit link.
  // BFS will handle this correctly by discovering a as a top level root and then avoiding further
  // exploring it.
  بفر_سائز = 1000;
  بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c2 (بفر, بفر_سائز);

  c2.آغاز_serialize ();
  غیرعلامتی b = جوڑو_object ("b", 1, &c2);

  آغاز_object ("a", 1, &c2);
  جوڑو_wide_انحراف (b, &c2);
  غیرعلامتی a = c2.نکالو_pack (جھوٹ);

  آغاز_object ("r", 1, &c2);
  جوڑو_انحراف (a, &c2);
  جوڑو_wide_انحراف (a, &c2);
  c2.نکالو_pack (جھوٹ);

  c2.انتہا_serialize ();

  گراف_t graph2 = گراف_t::create (c2.object_گراف ()).value ();

  hb_سیٹ_t roots2;
  hb_always_assert (graph2.ڈھونڈو_32bit_roots (graph2.جڑ_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  آزاد (بفر);
}

ساکن خالی test_invalid_link_objidx ()
{
  size_t بفر_سائز = 100;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_complex_2 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // Poison a link to point to (unsigned) -1
  خودکار links = گراف.vertices_[گراف.جڑ_idx ()].real_links_writer();
  links->objidx = (غیرعلامتی) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!گراف.ترتیب_دو_مختصر_ترین_فاصلہ ().is_ok ());

  آزاد (بفر);
}

ساکن خالی test_invalid_link_جوڑو_and_move ()
{
  size_t بفر_سائز = 100;
  خالی* بفر = حافظہ_دو (بفر_سائز);
  hb_serialize_context_t c (بفر, بفر_سائز);
  populate_serializer_complex_2 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!گراف.جوڑو_link (&dummy, 0, (غیرعلامتی) -1).is_ok ());

  // move_child with non-existent offset
  خودکار res = گراف.move_بچہ (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  آزاد (بفر);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

صحیح
مین (صحیح argc, حرف **argv)
{
  test_invalid_link_objidx ();
  test_invalid_link_جوڑو_and_move ();
  test_serialize ();
  test_ترتیب_دو_مختصر_ترین ();
  test_will_حد_سے_تجاوز_1 ();
  test_will_حد_سے_تجاوز_2 ();
  test_will_حد_سے_تجاوز_3 ();
  test_resolve_overflows_بذریعہ_ترتیب_دو ();
  test_resolve_overflows_بذریعہ_duplication ();
  test_resolve_overflows_بذریعہ_مضرب_duplications ();
  test_resolve_overflows_بذریعہ_ترجیح ();
  test_resolve_overflows_بذریعہ_space_assignment ();
  test_resolve_overflows_بذریعہ_isolation ();
  test_resolve_overflows_بذریعہ_isolation_with_recursive_duplication ();
  test_resolve_overflows_بذریعہ_isolation_spaces ();
  test_resolve_overflows_بذریعہ_isolating_16bit_space ();
  test_resolve_overflows_بذریعہ_isolating_16bit_space_2 ();
  test_resolve_overflows_بذریعہ_splitting_spaces ();
  test_resolve_overflows_بذریعہ_splitting_spaces_2 ();
  test_resolve_mixed_overflows_بذریعہ_isolation_spaces ();
  test_duplicate_برگ ();
  test_duplicate_interior ();
  test_virtual_link ();
  test_repack_آخری();
  test_shared_گرہ_with_virtual_links ();
  test_resolve_with_extension_promotion ();
  test_resolve_with_shared_extension_promotion ();
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  test_resolve_with_extension_promotion_1_2 ();
#اگر_ختم
  test_resolve_with_repeated_extension_promotion ();
  test_resolve_with_basic_pair_pos_1_بانٹو ();
  test_resolve_with_extension_pair_pos_1_بانٹو ();
  test_resolve_with_basic_pair_pos_2_بانٹو ();
  test_resolve_with_pair_pos_2_بانٹو_with_device_tables ();
  test_resolve_with_close_مقصد_حد_بندی_pair_pos_2_بانٹو ();
  test_resolve_with_basic_mark_بنیاد_pos_1_بانٹو ();
  test_resolve_with_basic_liga_بانٹو ();
  test_resolve_with_liga_بانٹو_move ();
  test_dont_duplicate_virtual ();
  test_resolve_with_liga_بانٹو_overlapping_clone ();
  test_resolve_with_liga_بانٹو_shared_جدول ();
  test_resolve_with_liga_بانٹو_shared_coverage ();
  test_deep_گراف_traversal ();
  test_32bit_roots_traversal ();

  // TODO(grieger): have run overflow tests compare graph equality not final packed binary.
  // TODO(grieger): split test where multiple subtables in one lookup are split to test link ordering.
  // TODO(grieger): split test where coverage table in subtable that is being split is shared.
  // TODO(grieger): test with extensions already mixed in as well.
  // TODO(grieger): test two layer ext promotion setup.
  // TODO(grieger): test sorting by subtables per byte in ext. promotion.
}
