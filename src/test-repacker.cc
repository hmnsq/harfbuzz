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
                    بنانشان len,
                    hb_serialize_context_t* c)
{
  حرف* obj = c->allocate_سائيز<حرف> (len);
  hb_memcpy (obj, value, len);
}

ساكن فراغ شروعات_object(ثابت حرف* ٽيگ_2,
                         بنانشان len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (ٽيگ_2, len, c);
}

ساكن بنانشان جوڙيو_object(ثابت حرف* ٽيگ_2,
                           بنانشان len,
                           hb_serialize_context_t* c,
                           منطقي shared = خطأ)
{
  شروعات_object (ٽيگ_2, len, c);
  أرجع c->ڪڍو_pack (shared);
}


ساكن فراغ جوڙيو_وٿي (بنانشان id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* وٿي = c->شروعات_embed<OT::Offset16> ();
  c->extend_min (وٿي);
  c->جوڙيو_link (*وٿي, id);
}

ساكن فراغ جوڙيو_24_وٿي (بنانشان id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* وٿي = c->شروعات_embed<OT::Offset24> ();
  c->extend_min (وٿي);
  c->جوڙيو_link (*وٿي, id);
}

ساكن فراغ جوڙيو_wide_وٿي (بنانشان id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* وٿي = c->شروعات_embed<OT::Offset32> ();
  c->extend_min (وٿي);
  c->جوڙيو_link (*وٿي, id);
}

ساكن فراغ جوڙيو_gsubgpos_سرو (بنانشان lookup_list,
                                 hb_serialize_context_t* c)
{
  حرف سرو[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  شروعات_object (سرو, 8, c);
  جوڙيو_وٿي (lookup_list, c);
  c->ڪڍو_pack (خطأ);
}

ساكن فراغ جوڙيو_gsubgpos_سرو_1_2 (بنانشان lookup_list,
                                     hb_serialize_context_t* c)
{
  حرف سرو[] = {
    0, 1, 0, 2, //نسخو
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  شروعات_object (سرو, حجم (سرو), c);
  جوڙيو_wide_وٿي (lookup_list, c);
  c->ڪڍو_pack (خطأ);
}

ساكن بنانشان جوڙيو_lookup_list (ثابت بنانشان* lookups,
                                 حرف ڳڻپ,
                                 hb_serialize_context_t* c)
{
  حرف lookup_ڳڻپ[] = {0, ڳڻپ};
  شروعات_object  ((حرف *) &lookup_ڳڻپ, 2, c);

  لكل (صحيح i = 0; i < ڳڻپ; i++)
    جوڙيو_وٿي (lookups[i], c);

  أرجع c->ڪڍو_pack (خطأ);
}

ساكن بنانشان جوڙيو_lookup_list_1_2 (ثابت بنانشان* lookups,
                                     حرف ڳڻپ,
                                     hb_serialize_context_t* c)
{
  حرف lookup_ڳڻپ[] = {0, ڳڻپ};
  شروعات_object ((حرف *) &lookup_ڳڻپ, 2, c);

  لكل (صحيح i = 0; i < ڳڻپ; i++)
    جوڙيو_wide_وٿي (lookups[i], c);

  أرجع c->ڪڍو_pack (خطأ);
}

ساكن فراغ شروعات_lookup (int8_t type,
                          int8_t انگ_subtables,
                          hb_serialize_context_t* c)
{
  حرف lookup[] = {
    0, (حرف)type, // type
    0, (حرف)2, // flag
    0, (حرف)انگ_subtables, // num subtables
  };

  شروعات_object (lookup, 6, c);
}

ساكن بنانشان پڄاڻي_lookup (hb_serialize_context_t* c)
{
  حرف filter[] = {0, 0};
  extend (filter, 2, c);
  أرجع c->ڪڍو_pack (خطأ);
}

ساكن بنانشان جوڙيو_extension (بنانشان ٻار,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  حرف ext[] = {
    0, 1,
    0, (حرف) type,
  };

  شروعات_object (ext, 4, c);
  جوڙيو_wide_وٿي (ٻار, c);

  أرجع c->ڪڍو_pack (خطأ);

}

// Adds coverage table fro [start, end]
ساكن بنانشان جوڙيو_coverage (بنانشان شروعات, بنانشان پڇاڙي,
                              hb_serialize_context_t* c,
                              منطقي shared = خطأ)
{
  إذا (پڇاڙي - شروعات == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //ڳڻپ

      (uint8_t) ((شروعات >> 8) & 0xFF),
      (uint8_t) (شروعات & 0xFF), // glyph[0]
    };
    أرجع جوڙيو_object ((حرف*) coverage, 6, c, shared);
  }

  إذا (پڇاڙي - شروعات == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //ڳڻپ

      (uint8_t) ((شروعات >> 8) & 0xFF),
      (uint8_t) (شروعات & 0xFF), // glyph[0]

      (uint8_t) ((پڇاڙي >> 8) & 0xFF),
      (uint8_t) (پڇاڙي & 0xFF), // glyph[1]
    };
    أرجع جوڙيو_object ((حرف*) coverage, 8, c, shared);
  }

  uint8_t coverage[] = {
    0, 2, // format
    0, 1, // range count

    (uint8_t) ((شروعات >> 8) & 0xFF),
    (uint8_t) (شروعات & 0xFF), //شروعات

    (uint8_t) ((پڇاڙي >> 8) & 0xFF),
    (uint8_t) (پڇاڙي & 0xFF), //پڇاڙي

    0, 0,
  };
  أرجع جوڙيو_object ((حرف*) coverage, 10, c, shared);
}


سانچو<قسم_نالو It>
ساكن بنانشان جوڙيو_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  أرجع c->ڪڍو_pack (خطأ);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
ساكن بنانشان جوڙيو_class_def (uint16_t شروعات_glyph,
                               uint16_t پڇاڙي_glyph,
                               hb_serialize_context_t* c)
{
  بنانشان ڳڻپ = پڇاڙي_glyph - شروعات_glyph;
  uint8_t سرو[] = {
    0, 1, // format

    (uint8_t) ((شروعات_glyph >> 8) & 0xFF),
    (uint8_t) (شروعات_glyph & 0xFF), // start_glyph

    (uint8_t) ((ڳڻپ >> 8) & 0xFF),
    (uint8_t) (ڳڻپ & 0xFF), //ڳڻپ
  };

  شروعات_object ((حرف*) سرو, 6, c);
  لكل (uint16_t i = 1; i <= ڳڻپ; i++)
  {
    uint8_t class_قدر_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //ڳڻپ
    };
    extend ((حرف*) class_قدر_2, 2, c);
  }

  أرجع c->ڪڍو_pack (خطأ);
}

ساكن بنانشان جوڙيو_pair_pos_1 (بنانشان* pair_sets,
                                حرف ڳڻپ,
                                بنانشان coverage,
                                hb_serialize_context_t* c)
{
  حرف format[] = {
    0, 1
  };

  شروعات_object (format, 2, c);
  جوڙيو_وٿي (coverage, c);

  حرف قدر_format[] = {
    0, 0,
    0, 0,
    0, ڳڻپ,
  };
  extend (قدر_format, 6, c);

  لكل (حرف i = 0; i < ڳڻپ; i++)
    جوڙيو_وٿي (pair_sets[(بنانشان) i], c);

  أرجع c->ڪڍو_pack (خطأ);
}

ساكن فراغ جوڙيو_liga_سيٽ_سرو (حرف liga_ڳڻپ,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_ڳڻپ_بائيٽون[] = {
    (uint8_t) (0xFF & (liga_ڳڻپ >> 8)),
    (uint8_t) (0xFF & liga_ڳڻپ),
  };

  شروعات_object ((ثابت حرف*) liga_ڳڻپ_بائيٽون, 2, c);
}

ساكن فراغ جوڙيو_liga_سرو (بنانشان coverage, بنانشان liga_سيٽ_ڳڻپ, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  شروعات_object ((ثابت حرف*) format, 2, c);
  جوڙيو_وٿي(coverage, c);

  uint8_t liga_سيٽ_ڳڻپ_بائيٽون[] = {
    (uint8_t) (0xFF & (liga_سيٽ_ڳڻپ >> 8)),
    (uint8_t) (0xFF & liga_سيٽ_ڳڻپ),
  };

  extend((ثابت حرف *) liga_سيٽ_ڳڻپ_بائيٽون, 2, c);
}

ساكن بنانشان جوڙيو_pair_pos_2 (بنانشان starting_class,
                                بنانشان coverage,
                                بنانشان class_def_1, uint16_t class_def_1_ڳڻپ,
                                بنانشان class_def_2, uint16_t class_def_2_ڳڻپ,
                                بنانشان* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  شروعات_object ((حرف*) format, 2, c);
  جوڙيو_وٿي (coverage, c);

  بنانشان انگ_قدرون_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  إذا (device_tables) {
    format2 |= 0x20;
    انگ_قدرون_3 += 1;
  }
  uint8_t قدر_format[] = {
    0, format1,
    0, format2,
  };

  extend ((حرف*) قدر_format, 4, c);

  جوڙيو_وٿي (class_def_1, c);
  جوڙيو_وٿي (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_ڳڻپ >> 8) & 0xFF),
    (uint8_t) (class_def_1_ڳڻپ & 0xFF),
    (uint8_t) ((class_def_2_ڳڻپ >> 8) & 0xFF),
    (uint8_t) (class_def_2_ڳڻپ & 0xFF),
  };
  extend ((حرف*) class_counts, 4, c);

  بنانشان انگ_بائيٽون_في_رڪارڊ = class_def_2_ڳڻپ * انگ_قدرون_3 * 2;
  uint8_t* رڪارڊ = (uint8_t*) صفر_يادگيري_ڏيو (1, انگ_بائيٽون_في_رڪارڊ);
  صحيح device_اشاريو = 0;
  لكل (uint16_t i = 0; i < class_def_1_ڳڻپ; i++)
  {

    لكل (uint16_t j = 0; j < class_def_2_ڳڻپ; j++)
    {
      لكل (صحيح k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((حرف*) value, 2, c);
      }

      إذا (device_tables) {
        جوڙيو_وٿي (device_tables[device_اشاريو++], c);
      }
    }
  }
  حرر (رڪارڊ);

  أرجع c->ڪڍو_pack (خطأ);
}

ساكن بنانشان جوڙيو_mark_بنياد_pos_1 (بنانشان mark_coverage,
                                     بنانشان بنياد_coverage,
                                     بنانشان mark_array,
                                     بنانشان بنياد_array,
                                     بنانشان class_ڳڻپ,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  شروعات_object ((حرف*) format, 2, c);
  جوڙيو_وٿي (mark_coverage, c);
  جوڙيو_وٿي (بنياد_coverage, c);

  uint8_t ڳڻپ[] = {
    (uint8_t) ((class_ڳڻپ >> 8) & 0xFF),
    (uint8_t) (class_ڳڻپ & 0xFF),
  };
  extend ((حرف*) ڳڻپ, 2, c);

  جوڙيو_وٿي (mark_array, c);
  جوڙيو_وٿي (بنياد_array, c);

  أرجع c->ڪڍو_pack (خطأ);
}

سانچو<صحيح mark_ڳڻپ,
    صحيح class_ڳڻپ,
    صحيح بنياد_ڳڻپ,
    صحيح جدول_ڳڻپ_2>
بنية Mark_بنياد_Pos_Buffers
{
  بنانشان بنياد_anchors[class_ڳڻپ * بنياد_ڳڻپ];
  بنانشان mark_anchors[mark_ڳڻپ];
  uint8_t anchor_buffers[class_ڳڻپ * بنياد_ڳڻپ + 100];
  uint8_t class_بفر[class_ڳڻپ * 2];

  Mark_بنياد_Pos_Buffers(hb_serialize_context_t* c)
  {
    لكل (بنانشان i = 0; i < حجم(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    لكل (بنانشان i = 0; i < class_ڳڻپ * بنياد_ڳڻپ; i++)
    {
      بنياد_anchors[i] = جوڙيو_object ((حرف*) &anchor_buffers[i], 100, c);
      إذا (i < class_ڳڻپ) {
        class_بفر[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_بفر[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    لكل (بنانشان i = 0; i < mark_ڳڻپ; i++)
    {
      mark_anchors[i] = جوڙيو_object ((حرف*) &anchor_buffers[i], 4, c);
    }
  }

  بنانشان create_mark_بنياد_pos_1 (بنانشان جدول_اشاريو, hb_serialize_context_t* c)
  {
    بنانشان class_في_جدول = class_ڳڻپ / جدول_ڳڻپ_2;
    بنانشان mark_في_class = mark_ڳڻپ / class_ڳڻپ;
    بنانشان شروعات_class = class_في_جدول * جدول_اشاريو;
    بنانشان پڇاڙي_class = class_في_جدول * (جدول_اشاريو + 1) - 1;

    // baseArray
    uint8_t بنياد_ڳڻپ_بفر[] = {
      (uint8_t) ((بنياد_ڳڻپ >> 8) & 0xFF),
      (uint8_t) (بنياد_ڳڻپ & 0xFF),

    };
    شروعات_object ((حرف*) بنياد_ڳڻپ_بفر, 2, c);
    لكل (بنانشان بنياد = 0; بنياد < بنياد_ڳڻپ; بنياد++)
    {
      لكل (بنانشان klass = شروعات_class; klass <= پڇاڙي_class; klass++)
      {
        بنانشان i = بنياد * class_ڳڻپ + klass;
        جوڙيو_وٿي (بنياد_anchors[i], c);
      }
    }
    بنانشان بنياد_array = c->ڪڍو_pack (خطأ);

    // markArray
    بنانشان انگ_marks = class_في_جدول * mark_في_class;
    uint8_t mark_ڳڻپ_بفر[] = {
      (uint8_t) ((انگ_marks >> 8) & 0xFF),
      (uint8_t) (انگ_marks & 0xFF),
    };
    شروعات_object ((حرف*) mark_ڳڻپ_بفر, 2, c);
    لكل (بنانشان mark = 0; mark < mark_ڳڻپ; mark++)
    {
      بنانشان klass = mark % class_ڳڻپ;
      إذا (klass < شروعات_class || klass > پڇاڙي_class) استمر;
      klass -= شروعات_class;

      extend ((حرف*) &class_بفر[2 * klass], 2, c);
      جوڙيو_وٿي (mark_anchors[mark], c);
    }
    بنانشان mark_array = c->ڪڍو_pack (خطأ);

    // markCoverage
    خودڪار it =
        + hb_range ((hb_codepoint_t) mark_ڳڻپ)
        | hb_filter ([&] (hb_codepoint_t mark) {
          بنانشان klass = mark % class_ڳڻپ;
          أرجع klass >= class_في_جدول * جدول_اشاريو &&
              klass < class_في_جدول * (جدول_اشاريو + 1);
        })
        ;
    بنانشان mark_coverage = جوڙيو_coverage (it, c);

    // baseCoverage
    بنانشان بنياد_coverage = جوڙيو_coverage (10, 10 + بنياد_ڳڻپ - 1, c);

    أرجع جوڙيو_mark_بنياد_pos_1 (mark_coverage,
                                بنياد_coverage,
                                mark_array,
                                بنياد_array,
                                class_في_جدول,
                                c);
  }
};

ساكن فراغ لاڳيتو_resolve_حد_کان_مٿي_test (ثابت حرف* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       بنانشان انگ_iterations = 0,
                                       منطقي recalculate_extensions = خطأ,
                                       hb_ٽيگ_t ٽيگ_2 = HB_ٽيگ ('G', 'S', 'U', 'B'),
                                       منطقي check_ٻٽو_equivalence = خطأ)
{
  لکو (">>> Testing overflowing resolution for %s\n",
          name);

  گراف_t گراف = گراف_t::create (overflowing.object_گراف ()).value ();

  گراف_t expected_گراف = گراف_t::create (expected.object_گراف ()).value ();
  إذا (گراف::will_حد_کان_مٿي (expected_گراف).value ())
  {
    إذا (check_ٻٽو_equivalence) {
      لکو("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_ٻٽو_equivalence);
    }
    hb_always_assert (expected_گراف.سونپيو_spaces ().is_ok ());
    hb_always_assert (expected_گراف.ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.وٿي_حد_کان_مٿي ());
  خودڪار r = hb_resolve_گراف_overflows (ٽيگ_2,
                                      انگ_iterations,
                                      recalculate_extensions,
                                      گراف);
  إذا (r.is_err())
    لکو("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", گراف::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = گراف::serialize (گراف).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = گراف::serialize (expected_گراف).value ();
  hb_always_assert (out2);
  إذا (check_ٻٽو_equivalence) {
    بنانشان l1, l2;
    ثابت حرف* d1 = hb_blob_get_معلومات(out1, &l1);
    ثابت حرف* d2 = hb_blob_get_معلومات(out2, &l2);

    منطقي ميلاپ = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    إذا (!ميلاپ) {
      لکو("## Result:\n");
      گراف.print();
      لکو("## Expected:\n");
      expected_گراف.print();
      hb_always_assert(ميلاپ);
    }
  }

  hb_blob_destroy (out1);
  hb_blob_destroy (out2);

  // Check the graphs are equivalent
  گراف.normalize ();
  expected_گراف.normalize ();
  إذا (!(گراف == expected_گراف)) {
    لکو("## Expected:\n");
    expected_گراف.print();
    لکو("## Result:\n");
    گراف.print();
  }
  hb_always_assert (گراف == expected_گراف);
}

ساكن فراغ جوڙيو_virtual_وٿي (بنانشان id,
                                hb_serialize_context_t* c)
{
  c->جوڙيو_virtual_link (id);
}

ساكن فراغ
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->شروعات_serialize<حرف> ();

  بنانشان obj_1 = جوڙيو_object ("ghi", 3, c);
  بنانشان obj_2 = جوڙيو_object ("def", 3, c);

  شروعات_object ("abc", 3, c);
  جوڙيو_وٿي (obj_2, c);
  جوڙيو_وٿي (obj_1, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_virtual (hb_serialize_context_t* c, منطقي with_حد_کان_مٿي)
{
  std::string large_string(50000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_4, obj_5;
  إذا (with_حد_کان_مٿي) {
    obj_5 = جوڙيو_object("55555", 5, c);
    obj_4 = جوڙيو_object(large_string.c_str(), 50000, c);
  } وإلا {
    obj_4 = جوڙيو_object(large_string.c_str(), 50000, c);
    obj_5 = جوڙيو_object("55555", 5, c);
  }

  شروعات_object(large_string.c_str(), 20000, c);
  جوڙيو_وٿي(obj_5, c);
  بنانشان obj_3 = c->ڪڍو_pack(خطأ);

  شروعات_object("2", 2, c);
  جوڙيو_virtual_وٿي(obj_5, c);
  بنانشان obj_2 = c->ڪڍو_pack(خطأ);

  // obj 1
  شروعات_object("1", 1, c);
  جوڙيو_وٿي(obj_2, c);
  جوڙيو_وٿي(obj_3, c);
  جوڙيو_وٿي(obj_4, c);
  c->ڪڍو_pack(خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_حد_کان_مٿي (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_1 = جوڙيو_object (large_string.c_str(), 10000, c);
  بنانشان obj_2 = جوڙيو_object (large_string.c_str(), 20000, c);
  بنانشان obj_3 = جوڙيو_object (large_string.c_str(), 50000, c);

  شروعات_object ("abc", 3, c);
  جوڙيو_وٿي (obj_3, c);
  جوڙيو_وٿي (obj_2, c);
  جوڙيو_وٿي (obj_1, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_ترجيح_حد_کان_مٿي (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_e = جوڙيو_object ("e", 1, c);
  بنانشان obj_d = جوڙيو_object ("d", 1, c);

  شروعات_object (large_string.c_str (), 50000, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str (), 20000, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_وٿي (obj_b, c);
  جوڙيو_وٿي (obj_c, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_ترجيح_حد_کان_مٿي_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_e = جوڙيو_object ("e", 1, c);

  شروعات_object (large_string.c_str (), 50000, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  بنانشان obj_d = جوڙيو_object ("d", 1, c);

  شروعات_object (large_string.c_str (), 20000, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_وٿي (obj_b, c);
  جوڙيو_وٿي (obj_c, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}


ساكن فراغ
populate_serializer_with_dedup_حد_کان_مٿي (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_1 = جوڙيو_object ("def", 3, c);

  شروعات_object (large_string.c_str(), 60000, c);
  جوڙيو_وٿي (obj_1, c);
  بنانشان obj_2 = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 10000, c);
  جوڙيو_وٿي (obj_2, c);
  جوڙيو_وٿي (obj_1, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_گڻو_dedup_حد_کان_مٿي (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان پن = جوڙيو_object("def", 3, c);

  constexpr بنانشان انگ_mid_nodes = 20;
  بنانشان mid_nodes[انگ_mid_nodes];
  لكل (بنانشان i = 0; i < انگ_mid_nodes; i++) {
    شروعات_object(large_string.c_str(), 10000 + i, c);
    جوڙيو_وٿي(پن, c);
    mid_nodes[i] = c->ڪڍو_pack(خطأ);
  }

  شروعات_object("abc", 3, c);
  لكل (بنانشان i = 0; i < انگ_mid_nodes; i++) {
    جوڙيو_wide_وٿي(mid_nodes[i], c);
  }
  c->ڪڍو_pack(خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_حد_کان_مٿي (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_4 = جوڙيو_object ("4", 1, c);

  شروعات_object (large_string.c_str(), 60000, c);
  جوڙيو_وٿي (obj_4, c);
  بنانشان obj_3 = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 10000, c);
  جوڙيو_وٿي (obj_4, c);
  بنانشان obj_2 = c->ڪڍو_pack (خطأ);

  شروعات_object ("1", 1, c);
  جوڙيو_wide_وٿي (obj_3, c);
  جوڙيو_وٿي (obj_2, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_حد_کان_مٿي_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_f = جوڙيو_object ("f", 1, c);

  شروعات_object ("e", 1, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object ("d", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 60000, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_h = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 60000, c);
  جوڙيو_وٿي (obj_c, c);
  جوڙيو_وٿي (obj_h, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 10000, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_g = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 11000, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_i = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_wide_وٿي (obj_b, c);
  جوڙيو_وٿي (obj_g, c);
  جوڙيو_وٿي (obj_i, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_حد_کان_مٿي_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();


  // space 1

  بنانشان obj_f_مفرد = جوڙيو_object ("f", 1, c);

  شروعات_object ("e", 1, c);
  جوڙيو_وٿي (obj_f_مفرد, c);
  بنانشان obj_e_مفرد = c->ڪڍو_pack (خطأ);

  شروعات_object ("d", 1, c);
  جوڙيو_وٿي (obj_e_مفرد, c);
  بنانشان obj_d_مفرد = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 60000, c);
  جوڙيو_وٿي (obj_d_مفرد, c);
  بنانشان obj_h = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_e_مفرد, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 60000, c);
  جوڙيو_وٿي (obj_c, c);
  جوڙيو_وٿي (obj_h, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  // space 0

  بنانشان obj_f = جوڙيو_object ("f", 1, c);

  شروعات_object ("e", 1, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);


  شروعات_object ("d", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 11000, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_i = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 10000, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_g = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_wide_وٿي (obj_b, c);
  جوڙيو_وٿي (obj_g, c);
  جوڙيو_وٿي (obj_i, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_isolation_حد_کان_مٿي_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_d = جوڙيو_object ("f", 1, c);
  بنانشان obj_e = جوڙيو_object ("f", 1, c);

  شروعات_object (large_string.c_str(), 60000, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_b = c->ڪڍو_pack ();

  شروعات_object (large_string.c_str(), 60000, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack ();


  شروعات_object ("a", 1, c);
  جوڙيو_wide_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_c, c);
  c->ڪڍو_pack ();

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_repack_آخري (hb_serialize_context_t* c, منطقي with_حد_کان_مٿي)
{
  std::string large_string(70000, 'c');
  c->شروعات_serialize<حرف> ();
  c->push();

  // Obj E
  بنانشان obj_e_1, obj_e_2;
  إذا (with_حد_کان_مٿي) {
    obj_e_1 = جوڙيو_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } وإلا {
    obj_e_2 = جوڙيو_object("a", 1, c);
  }

  // Obj D
  c->push();
  جوڙيو_وٿي(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  بنانشان obj_d = c->ڪڍو_pack(خطأ);

  جوڙيو_وٿي(obj_d, c);
  hb_always_assert(c->آخري_added_ٻار_اشاريو() == obj_d);

  إذا (!with_حد_کان_مٿي) {
    obj_e_1 = جوڙيو_object("a", 1, c);
  }

  // Obj C
  c->push();
  جوڙيو_وٿي(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  بنانشان obj_c = c->ڪڍو_pack(خطأ);

  جوڙيو_وٿي(obj_c, c);

  // Obj B
  بنانشان obj_b = جوڙيو_object("b", 1, c);
  جوڙيو_وٿي(obj_b, c);

  // Obj A
  c->repack_آخري(obj_d);
  c->ڪڍو_pack(خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_spaces (hb_serialize_context_t* c, منطقي with_حد_کان_مٿي)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_i;

  إذا (with_حد_کان_مٿي)
    obj_i = جوڙيو_object ("i", 1, c);

  // Space 2
  بنانشان obj_h = جوڙيو_object ("h", 1, c);

  شروعات_object (large_string.c_str(), 30000, c);
  جوڙيو_وٿي (obj_h, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  // Space 1
  إذا (!with_حد_کان_مٿي)
    obj_i = جوڙيو_object ("i", 1, c);

  شروعات_object (large_string.c_str(), 30000, c);
  جوڙيو_وٿي (obj_i, c);
  بنانشان obj_g = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 30000, c);
  جوڙيو_وٿي (obj_i, c);
  بنانشان obj_f = c->ڪڍو_pack (خطأ);

  شروعات_object ("d", 1, c);
  جوڙيو_وٿي (obj_g, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_wide_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_c, c);
  جوڙيو_wide_وٿي (obj_d, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_g = جوڙيو_object ("g", 1, c);
  بنانشان obj_h = جوڙيو_object ("h", 1, c);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_g, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_h, c);
  بنانشان obj_f = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object ("d", 1, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_e, c);
  جوڙيو_وٿي (obj_h, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_c, c);
  جوڙيو_wide_وٿي (obj_d, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_g_مفرد = جوڙيو_object ("g", 1, c);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_g_مفرد, c);
  بنانشان obj_e_مفرد = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_e_مفرد, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  بنانشان obj_h_مفرد = جوڙيو_object ("h", 1, c);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_h_مفرد, c);
  بنانشان obj_f = c->ڪڍو_pack (خطأ);

  شروعات_object ("d", 1, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  بنانشان obj_g = جوڙيو_object ("g", 1, c);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_g, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  بنانشان obj_h = جوڙيو_object ("h", 1, c);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_e, c);
  جوڙيو_وٿي (obj_h, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_c, c);
  جوڙيو_wide_وٿي (obj_d, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize ();
}

ساكن فراغ
populate_serializer_short_and_wide_subgraph_پاڙ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_e = جوڙيو_object ("e", 1, c);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_c, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_c, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_c, c);
  جوڙيو_wide_وٿي (obj_d, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_short_and_wide_subgraph_پاڙ_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_e_مفرد = جوڙيو_object ("e", 1, c);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_e_مفرد, c);
  بنانشان obj_c_مفرد = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_c_مفرد, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  بنانشان obj_e = جوڙيو_object ("e", 1, c);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);


  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_c, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_c_مفرد, c);
  جوڙيو_wide_وٿي (obj_d, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_ورهايو_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_f = جوڙيو_object ("f", 1, c);

  شروعات_object (large_string.c_str(), 40000, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 40000, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_wide_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_c, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_ورهايو_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_f = جوڙيو_object ("f", 1, c);

  شروعات_object (large_string.c_str(), 40000, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str(), 40000, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_c, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_ورهايو_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_f_مفرد = جوڙيو_object ("f", 1, c);

  شروعات_object (large_string.c_str(), 40000, c);
  جوڙيو_وٿي (obj_f_مفرد, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  بنانشان obj_f = جوڙيو_object ("f", 1, c);

  شروعات_object (large_string.c_str(), 40000, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_wide_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_c, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_ورهايو_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->شروعات_serialize<حرف> ();

  // Space 2

  بنانشان obj_f_double_مفرد = جوڙيو_object ("f", 1, c);

  شروعات_object (large_string.c_str(), 40000, c);
  جوڙيو_وٿي (obj_f_double_مفرد, c);
  بنانشان obj_d_مفرد = c->ڪڍو_pack (خطأ);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_d_مفرد, c);
  بنانشان obj_b_مفرد = c->ڪڍو_pack (خطأ);

  // Space 1

  بنانشان obj_f_مفرد = جوڙيو_object ("f", 1, c);

  شروعات_object (large_string.c_str(), 40000, c);
  جوڙيو_وٿي (obj_f_مفرد, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  // Space 0

  بنانشان obj_f = جوڙيو_object ("f", 1, c);

  شروعات_object (large_string.c_str(), 40000, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  //پاڙ
  شروعات_object ("a", 1, c);
  جوڙيو_وٿي (obj_b, c);
  جوڙيو_wide_وٿي (obj_b_مفرد, c);
  جوڙيو_wide_وٿي (obj_c, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->شروعات_serialize<حرف> ();

  بنانشان obj_5 = جوڙيو_object ("mn", 2, c);

  بنانشان obj_4 = جوڙيو_object ("jkl", 3, c);

  شروعات_object ("ghi", 3, c);
  جوڙيو_وٿي (obj_4, c);
  بنانشان obj_3 = c->ڪڍو_pack (خطأ);

  شروعات_object ("def", 3, c);
  جوڙيو_وٿي (obj_3, c);
  بنانشان obj_2 = c->ڪڍو_pack (خطأ);

  شروعات_object ("abc", 3, c);
  جوڙيو_وٿي (obj_2, c);
  جوڙيو_وٿي (obj_4, c);
  جوڙيو_وٿي (obj_5, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->شروعات_serialize<حرف> ();

  بنانشان obj_6 = جوڙيو_object ("opqrst", 6, c);

  بنانشان obj_5 = جوڙيو_object ("mn", 2, c);

  شروعات_object ("jkl", 3, c);
  جوڙيو_وٿي (obj_6, c);
  بنانشان obj_4 = c->ڪڍو_pack (خطأ);

  شروعات_object ("ghi", 3, c);
  جوڙيو_وٿي (obj_4, c);
  بنانشان obj_3 = c->ڪڍو_pack (خطأ);

  شروعات_object ("def", 3, c);
  جوڙيو_وٿي (obj_3, c);
  بنانشان obj_2 = c->ڪڍو_pack (خطأ);

  شروعات_object ("abc", 3, c);
  جوڙيو_وٿي (obj_2, c);
  جوڙيو_وٿي (obj_4, c);
  جوڙيو_وٿي (obj_5, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->شروعات_serialize<حرف> ();

  بنانشان obj_d = جوڙيو_object ("d", 1, c);

  شروعات_object ("b", 1, c);
  جوڙيو_وٿي (obj_d, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("e", 1, c);
  جوڙيو_virtual_وٿي (obj_b, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  شروعات_object ("c", 1, c);
  جوڙيو_وٿي (obj_e, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_وٿي (obj_b, c);
  جوڙيو_وٿي (obj_c, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_24_and_32_ٻٽو_انگ_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان obj_f = جوڙيو_object ("f", 1, c);
  بنانشان obj_g = جوڙيو_object ("g", 1, c);
  بنانشان obj_j = جوڙيو_object ("j", 1, c);
  بنانشان obj_k = جوڙيو_object ("k", 1, c);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_f, c);
  بنانشان obj_c = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_g, c);
  بنانشان obj_d = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_j, c);
  بنانشان obj_h = c->ڪڍو_pack (خطأ);

  شروعات_object (large_string.c_str (), 40000, c);
  جوڙيو_وٿي (obj_k, c);
  بنانشان obj_i = c->ڪڍو_pack (خطأ);

  شروعات_object ("e", 1, c);
  جوڙيو_wide_وٿي (obj_h, c);
  جوڙيو_wide_وٿي (obj_i, c);
  بنانشان obj_e = c->ڪڍو_pack (خطأ);

  شروعات_object ("b", 1, c);
  جوڙيو_24_وٿي (obj_c, c);
  جوڙيو_24_وٿي (obj_d, c);
  جوڙيو_24_وٿي (obj_e, c);
  بنانشان obj_b = c->ڪڍو_pack (خطأ);

  شروعات_object ("a", 1, c);
  جوڙيو_24_وٿي (obj_b, c);
  c->ڪڍو_pack (خطأ);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              صحيح انگ_extensions = 0,
                                              منطقي shared_subtables = خطأ,
                                              منطقي نسخو_1_2 = خطأ)
{
  constexpr صحيح انگ_lookups = 5;
  constexpr صحيح انگ_subtables = انگ_lookups * 2;
  بنانشان صحيح lookups[انگ_lookups];
  بنانشان صحيح subtables[انگ_subtables];
  بنانشان صحيح extensions[انگ_subtables];

  std::string large_string(60000, 'a');
  c->شروعات_serialize<حرف> ();


  لكل (صحيح i = انگ_subtables - 1; i >= 0; i--)
    subtables[i] = جوڙيو_object(large_string.c_str (), 15000 + i, c);

  لكل (صحيح i = انگ_subtables - 1;
       i >= (انگ_lookups - انگ_extensions) * 2;
       i--)
  {
    extensions[i] = جوڙيو_extension (subtables[i], 5, c);
  }

  لكل (صحيح i = انگ_lookups - 1; i >= 0; i--)
  {
    منطقي is_ext = (i >= (انگ_lookups - انگ_extensions));

    شروعات_lookup (is_ext ? (حرف) 7 : (حرف) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    إذا (is_ext) {
      إذا (shared_subtables && i > 2) {
        جوڙيو_وٿي (extensions[i * 2 - 1], c);
      }
      جوڙيو_وٿي (extensions[i * 2], c);
      جوڙيو_وٿي (extensions[i * 2 + 1], c);
    } وإلا {
      إذا (shared_subtables && i > 2) {
        جوڙيو_وٿي (subtables[i * 2 - 1], c);
      }
      جوڙيو_وٿي (subtables[i * 2], c);
      جوڙيو_وٿي (subtables[i * 2 + 1], c);
    }

    lookups[i] = پڄاڻي_lookup (c);
  }

  بنانشان lookup_list = نسخو_1_2
                         ? جوڙيو_lookup_list_1_2 (lookups, انگ_lookups, c)
                         : جوڙيو_lookup_list (lookups, انگ_lookups, c);

  إذا (نسخو_1_2)
    جوڙيو_gsubgpos_سرو_1_2 (lookup_list, c);
  وإلا
    جوڙيو_gsubgpos_سرو (lookup_list, c);

  c->پڇاڙي_serialize();
}

سانچو<صحيح انگ_pair_pos_1, صحيح انگ_pair_سيٽ>
ساكن فراغ
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           منطقي as_extension = خطأ)
{
  std::string large_string(60000, 'a');
  c->شروعات_serialize<حرف> ();

  constexpr صحيح ڪل_pair_سيٽ = انگ_pair_pos_1 * انگ_pair_سيٽ;
  بنانشان pair_سيٽ_2[ڪل_pair_سيٽ];
  بنانشان coverage[انگ_pair_pos_1];
  بنانشان pair_pos_1[انگ_pair_pos_1];

  لكل (صحيح i = انگ_pair_pos_1 - 1; i >= 0; i--)
  {
    لكل (صحيح j = (i + 1) * انگ_pair_سيٽ - 1; j >= i * انگ_pair_سيٽ; j--)
      pair_سيٽ_2[j] = جوڙيو_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = جوڙيو_coverage (i * انگ_pair_سيٽ,
                                (i + 1) * انگ_pair_سيٽ - 1, c);

    pair_pos_1[i] = جوڙيو_pair_pos_1 (&pair_سيٽ_2[i * انگ_pair_سيٽ],
                                    انگ_pair_سيٽ,
                                    coverage[i],
                                    c);
  }

  بنانشان pair_pos_2 = جوڙيو_object (large_string.c_str(), 200, c);

  إذا (as_extension) {
    pair_pos_2 = جوڙيو_extension (pair_pos_2, 2, c);
    لكل (صحيح i = انگ_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = جوڙيو_extension (pair_pos_1[i], 2, c);
  }

  شروعات_lookup (as_extension ? 9 : 2, 1 + انگ_pair_pos_1, c);

  لكل (صحيح i = 0; i < انگ_pair_pos_1; i++)
    جوڙيو_وٿي (pair_pos_1[i], c);
  جوڙيو_وٿي (pair_pos_2, c);

  بنانشان lookup = پڄاڻي_lookup (c);

  بنانشان lookup_list = جوڙيو_lookup_list (&lookup, 1, c);

  جوڙيو_gsubgpos_سرو (lookup_list, c);

  c->پڇاڙي_serialize();
}

سانچو<صحيح انگ_pair_pos_2, صحيح انگ_class_1, صحيح انگ_class_2>
ساكن فراغ
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           منطقي as_extension = خطأ,
                                           منطقي with_device_tables = خطأ,
                                           منطقي extra_جدول = سچ)
{
  std::string large_string(100000, 'a');
  c->شروعات_serialize<حرف> ();

  بنانشان coverage[انگ_pair_pos_2];
  بنانشان class_def_1[انگ_pair_pos_2];
  بنانشان class_def_2[انگ_pair_pos_2];
  بنانشان pair_pos_2[انگ_pair_pos_2];

  بنانشان* device_tables = (بنانشان*) صفر_يادگيري_ڏيو (انگ_pair_pos_2 * انگ_class_1 * انگ_class_2,
                                                حجم(بنانشان));

  // Total glyphs = num_class_1 * num_pair_pos_2
  لكل (صحيح i = انگ_pair_pos_2 - 1; i >= 0; i--)
  {
    بنانشان شروعات_glyph = 5 + i * انگ_class_1;
    إذا (انگ_class_2 >= انگ_class_1)
    {
      class_def_2[i] = جوڙيو_class_def (11,
                                      10 + انگ_class_2, c);
      class_def_1[i] = جوڙيو_class_def (شروعات_glyph + 1,
                                      شروعات_glyph + انگ_class_1,
                                      c);
    } وإلا {
      class_def_1[i] = جوڙيو_class_def (شروعات_glyph + 1,
                                      شروعات_glyph + انگ_class_1,
                                      c);
      class_def_2[i] = جوڙيو_class_def (11,
                                      10 + انگ_class_2, c);
    }

    coverage[i] = جوڙيو_coverage (شروعات_glyph,
                                شروعات_glyph + انگ_class_1 - 1,
                                c);

    إذا (with_device_tables)
    {
      لكل(صحيح j = (i + 1) * انگ_class_1 * انگ_class_2 - 1;
          j >= i * انگ_class_1 * انگ_class_2;
          j--)
      {
        uint8_t جدول[] = {
          (uint8_t) ((j >> 8) & 0xFF),
          (uint8_t) (j & 0xFF),
        };
        device_tables[j] = جوڙيو_object ((حرف*) جدول, 2, c);
      }
    }

    pair_pos_2[i] = جوڙيو_pair_pos_2 (1 + i * انگ_class_1,
                                    coverage[i],
                                    class_def_1[i], انگ_class_1,
                                    class_def_2[i], انگ_class_2,
                                    with_device_tables
                                    ? &device_tables[i * انگ_class_1 * انگ_class_2]
                                    : خالي_اشارو,
                                    c);
  }


  بنانشان pair_pos_1 = 0;
  إذا (extra_جدول) pair_pos_1 = جوڙيو_object (large_string.c_str(), 100000, c);

  إذا (as_extension) {
    لكل (صحيح i = انگ_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = جوڙيو_extension (pair_pos_2[i], 2, c);

    إذا (extra_جدول)
      pair_pos_1 = جوڙيو_extension (pair_pos_1, 2, c);
  }

  شروعات_lookup (as_extension ? 9 : 2, 1 + انگ_pair_pos_2, c);

  إذا (extra_جدول)
    جوڙيو_وٿي (pair_pos_1, c);

  لكل (صحيح i = 0; i < انگ_pair_pos_2; i++)
    جوڙيو_وٿي (pair_pos_2[i], c);

  بنانشان lookup = پڄاڻي_lookup (c);

  بنانشان lookup_list = جوڙيو_lookup_list (&lookup, 1, c);

  جوڙيو_gsubgpos_سرو (lookup_list, c);

  c->پڇاڙي_serialize();

  حرر (device_tables);
}

سانچو<صحيح mark_ڳڻپ,
    صحيح class_ڳڻپ,
    صحيح بنياد_ڳڻپ,
    صحيح جدول_ڳڻپ_2>
ساكن فراغ
populate_serializer_with_large_mark_بنياد_pos_1 (hb_serialize_context_t* c)
{
  c->شروعات_serialize<حرف> ();

  Mark_بنياد_Pos_Buffers<mark_ڳڻپ, class_ڳڻپ, بنياد_ڳڻپ, جدول_ڳڻپ_2> buffers (c);

  بنانشان mark_بنياد_pos[جدول_ڳڻپ_2];
  لكل (بنانشان i = 0; i < جدول_ڳڻپ_2; i++)
    mark_بنياد_pos[i] = buffers.create_mark_بنياد_pos_1 (i, c);

  لكل (صحيح i = 0; i < جدول_ڳڻپ_2; i++)
    mark_بنياد_pos[i] = جوڙيو_extension (mark_بنياد_pos[i], 4, c);

  شروعات_lookup (9, جدول_ڳڻپ_2, c);

  لكل (صحيح i = 0; i < جدول_ڳڻپ_2; i++)
    جوڙيو_وٿي (mark_بنياد_pos[i], c);

  بنانشان lookup = پڄاڻي_lookup (c);

  بنانشان lookup_list = جوڙيو_lookup_list (&lookup, 1, c);

  جوڙيو_gsubgpos_سرو (lookup_list, c);

  c->پڇاڙي_serialize();
}

سانچو<بنانشان Lig_Subst_ڳڻپ,
         بنانشان Liga_سيٽ_ڳڻپ,
         بنانشان Liga_في_سيٽ_ڳڻپ,
         بنانشان Liga_سائيز>
ساكن فراغ populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    منطقي sequential_liga_sets,
                                                    منطقي shared,
                                                    بنانشان *liga_subst_id,
                                                    منطقي unique_lig_str = خطأ)
{
  حرف ch = 'a';

  بنانشان liga[Liga_سيٽ_ڳڻپ * Liga_في_سيٽ_ڳڻپ];
  بنانشان liga_سيٽ[Liga_سيٽ_ڳڻپ];
  لكل (بنانشان l = 0; l < Lig_Subst_ڳڻپ; l++) {
    بنانشان coverage_شروعات = 0;
    بنانشان coverage_پڇاڙي = Liga_سيٽ_ڳڻپ - 1;
    إذا (sequential_liga_sets) {
      coverage_شروعات = l * Liga_سيٽ_ڳڻپ;
      coverage_پڇاڙي = (l + 1) * Liga_سيٽ_ڳڻپ - 1;
    }
    بنانشان coverage = جوڙيو_coverage(coverage_شروعات, coverage_پڇاڙي, c, shared);

    لكل (بنانشان i = 0; i < Liga_سيٽ_ڳڻپ; i++) {
      لكل (بنانشان j = 0; j < Liga_في_سيٽ_ڳڻپ; j++)
      {
        std::string large_string(100000, ch);
        شروعات_object (large_string.c_str(), Liga_سائيز, c);
        إذا (unique_lig_str)
          ch += 1;
        جوڙيو_virtual_وٿي(coverage, c);
        liga[i * Liga_في_سيٽ_ڳڻپ + j] = c->ڪڍو_pack (shared);
      }

      جوڙيو_liga_سيٽ_سرو(Liga_في_سيٽ_ڳڻپ, c);
      جوڙيو_virtual_وٿي(coverage, c);
      لكل (بنانشان j = 0; j < Liga_في_سيٽ_ڳڻپ; j++)
      {
        جوڙيو_وٿي(liga[i * Liga_في_سيٽ_ڳڻپ + j], c);
      }

      liga_سيٽ[i] = c->ڪڍو_pack(shared);
    }

    جوڙيو_liga_سرو(coverage, Liga_سيٽ_ڳڻپ, c);
    لكل (بنانشان i = 0; i < Liga_سيٽ_ڳڻپ; i++)
    {
      جوڙيو_وٿي(liga_سيٽ[i], c);
    }

    liga_subst_id[l] = c->ڪڍو_pack(shared);
  }

  لكل (بنانشان l = 0; l < Lig_Subst_ڳڻپ; l++) {
    liga_subst_id[l] = جوڙيو_extension(liga_subst_id[l], 4, c);
  }
}

سانچو<بنانشان liga_subst_ڳڻپ,
         بنانشان liga_سيٽ_ڳڻپ,
         بنانشان liga_في_سيٽ_ڳڻپ,
         بنانشان liga_سائيز>
ساكن فراغ
populate_serializer_with_large_liga (hb_serialize_context_t* c, منطقي sequential_liga_sets)
{
  c->شروعات_serialize<حرف> ();

  بنانشان liga_subst[liga_subst_ڳڻپ];
  populate_serializer_with_large_ligsubst<liga_subst_ڳڻپ, liga_سيٽ_ڳڻپ, liga_في_سيٽ_ڳڻپ, liga_سائيز>(c, sequential_liga_sets, خطأ, liga_subst);

  شروعات_lookup (7, liga_subst_ڳڻپ, c);
  لكل (بنانشان l = 0; l < liga_subst_ڳڻپ; l++) {
    جوڙيو_وٿي(liga_subst[l], c);
  }

  بنانشان lookup = پڄاڻي_lookup (c);

  بنانشان lookup_list = جوڙيو_lookup_list (&lookup, 1, c);

  جوڙيو_gsubgpos_سرو (lookup_list, c);

  c->پڇاڙي_serialize();
}

ساكن فراغ
populate_serializer_with_large_liga_overlapping_clone_نتيجو (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->شروعات_serialize<حرف> ();

  constexpr بنانشان liga_سائيز = 30000;

  بنانشان liga[6];
  بنانشان liga_subst[3];
  بنانشان liga_سيٽ[2];

  // LigaSubst 3
  بنانشان coverage = جوڙيو_coverage(1, 1, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    شروعات_object (large_string.c_str(), liga_سائيز, c);
    جوڙيو_virtual_وٿي(coverage, c);
    liga[i] = c->ڪڍو_pack (خطأ);
  }

  جوڙيو_liga_سيٽ_سرو(2, c);
  جوڙيو_virtual_وٿي(coverage, c);
  لكل (بنانشان i = 0; i < 2; i++)
    جوڙيو_وٿي(liga[i], c);
  liga_سيٽ[0] = c->ڪڍو_pack(خطأ);

  جوڙيو_liga_سرو(coverage, 1, c);
  جوڙيو_وٿي(liga_سيٽ[0], c);
  liga_subst[2] = c->ڪڍو_pack(خطأ);

  // LigaSubst 2
  coverage = جوڙيو_coverage(0, 1, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    شروعات_object (large_string.c_str(), liga_سائيز, c);
    جوڙيو_virtual_وٿي(coverage, c);
    liga[i] = c->ڪڍو_pack (خطأ);
  }

  جوڙيو_liga_سيٽ_سرو(1, c);
  جوڙيو_virtual_وٿي(coverage, c);
  جوڙيو_وٿي(liga[1], c);
  liga_سيٽ[1] = c->ڪڍو_pack(خطأ);

  جوڙيو_liga_سيٽ_سرو(1, c);
  جوڙيو_virtual_وٿي(coverage, c);
  جوڙيو_وٿي(liga[0], c);
  liga_سيٽ[0] = c->ڪڍو_pack(خطأ);

  جوڙيو_liga_سرو(coverage, 2, c);
  جوڙيو_وٿي(liga_سيٽ[0], c);
  جوڙيو_وٿي(liga_سيٽ[1], c);
  liga_subst[1] = c->ڪڍو_pack(خطأ);

  // LigaSubst 1
  coverage = جوڙيو_coverage(0, 0, c);
  لكل (صحيح i = 1; i >= 0; i--) {
    شروعات_object (large_string.c_str(), liga_سائيز, c);
    جوڙيو_virtual_وٿي(coverage, c);
    liga[i] = c->ڪڍو_pack (خطأ);
  }

  جوڙيو_liga_سيٽ_سرو(2, c);
  جوڙيو_virtual_وٿي(coverage, c);
  لكل (بنانشان i = 0; i < 2; i++)
    جوڙيو_وٿي(liga[i], c);
  liga_سيٽ[0] = c->ڪڍو_pack(خطأ);

  جوڙيو_liga_سرو(coverage, 1, c);
  جوڙيو_وٿي(liga_سيٽ[0], c);
  liga_subst[0] = c->ڪڍو_pack(خطأ);

  لكل (صحيح l = 2; l >= 0; l--) {
    liga_subst[l] = جوڙيو_extension(liga_subst[l], 4, c);
  }

  شروعات_lookup (7, 3, c);
  لكل (بنانشان l = 0; l < 3; l++) {
    جوڙيو_وٿي(liga_subst[l], c);
  }

  بنانشان lookup = پڄاڻي_lookup (c);

  بنانشان lookup_list = جوڙيو_lookup_list (&lookup, 1, c);

  جوڙيو_gsubgpos_سرو (lookup_list, c);

  c->پڇاڙي_serialize();
}

سانچو<بنانشان liga_subst_ڳڻپ,
         بنانشان liga_سيٽ_ڳڻپ,
         بنانشان liga_في_سيٽ_ڳڻپ,
         بنانشان liga_سائيز>
ساكن فراغ
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, منطقي sequential_liga_sets)
{
  c->شروعات_serialize<حرف> ();

  بنانشان lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  بنانشان liga_subst_idx[liga_subst_ڳڻپ];
  populate_serializer_with_large_ligsubst<liga_subst_ڳڻپ, liga_سيٽ_ڳڻپ, liga_في_سيٽ_ڳڻپ, liga_سائيز>(c, sequential_liga_sets, سچ, liga_subst_idx, سچ);

  شروعات_lookup (7, liga_subst_ڳڻپ, c);
  لكل (بنانشان l = 0; l < liga_subst_ڳڻپ; l++) {
    جوڙيو_وٿي(liga_subst_idx[l], c);
  }

  lookups[0] = پڄاڻي_lookup (c);

  // Lookup with 2 LigSubst tables
  بنانشان liga_subst[liga_subst_ڳڻپ + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, خطأ, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_ڳڻپ, liga_سيٽ_ڳڻپ, liga_في_سيٽ_ڳڻپ, liga_سائيز>(c, sequential_liga_sets, سچ, &liga_subst[1], سچ);

  شروعات_lookup (7, liga_subst_ڳڻپ + 1, c);
  لكل (بنانشان l = 0; l <= liga_subst_ڳڻپ; l++) {
    جوڙيو_وٿي(liga_subst[l], c);
  }

  lookups[1] = پڄاڻي_lookup (c);

  بنانشان lookup_list = جوڙيو_lookup_list (lookups, 2, c);
  جوڙيو_gsubgpos_سرو (lookup_list, c);

  c->پڇاڙي_serialize();
}

سانچو<بنانشان liga_subst_ڳڻپ,
         بنانشان liga_سيٽ_ڳڻپ,
         بنانشان liga_في_سيٽ_ڳڻپ,
         بنانشان liga_سائيز>
ساكن فراغ
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->شروعات_serialize<حرف> ();

  بنانشان liga_subst[liga_subst_ڳڻپ + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, سچ, سچ, liga_subst, سچ);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_ڳڻپ, liga_سيٽ_ڳڻپ, liga_في_سيٽ_ڳڻپ, liga_سائيز>(c, سچ, سچ, &liga_subst[1], سچ);

  شروعات_lookup (7, liga_subst_ڳڻپ + 1, c);
  لكل (بنانشان l = 0; l <= liga_subst_ڳڻپ; l++) {
    جوڙيو_وٿي(liga_subst[l], c);
  }

  بنانشان lookup = پڄاڻي_lookup (c);

  بنانشان lookup_list = جوڙيو_lookup_list (&lookup, 1, c);
  جوڙيو_gsubgpos_سرو (lookup_list, c);

  c->پڇاڙي_serialize();
}

ساكن فراغ test_ترتيب_ڏيو_سڀ_کان_ٿورو ()
{
  size_t بفر_سائيز = 100;
  فراغ* بفر_a = يادگيري_ڏيو (بفر_سائيز);
  فراغ* بفر_e = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t a (بفر_a, بفر_سائيز);
  hb_serialize_context_t e (بفر_e, بفر_سائيز);
  populate_serializer_complex_2 (&a);

  گراف_t گراف = گراف_t::create (a.object_گراف ()).value ();
  hb_always_assert (گراف.ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو ().is_ok());
  گراف.normalize();

  // Expected graph
  e.شروعات_serialize();
  بنانشان jkl = جوڙيو_object ("jkl", 3, &e);

  شروعات_object("ghi", 3, &e);
  جوڙيو_وٿي(jkl, &e);
  بنانشان ghi = e.ڪڍو_pack(خطأ);

  شروعات_object("def", 3, &e);
  جوڙيو_وٿي(ghi, &e);
  بنانشان def = e.ڪڍو_pack(خطأ);

  بنانشان mn = جوڙيو_object("mn", 2, &e);

  شروعات_object ("abc", 3, &e);
  جوڙيو_وٿي (def, &e);
  جوڙيو_وٿي (jkl, &e);
  جوڙيو_وٿي (mn, &e);
  e.ڪڍو_pack (خطأ);
  e.پڇاڙي_serialize();

  گراف_t expected = گراف_t::create (e.object_گراف ()).value ();
  expected.normalize();

  أكد(expected == گراف);

  حرر (بفر_a);
  حرر (بفر_e);
}

ساكن فراغ test_duplicate_پن ()
{
  size_t بفر_سائيز = 100;
  فراغ* بفر_a = يادگيري_ڏيو (بفر_سائيز);
  فراغ* بفر_e = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t a (بفر_a, بفر_سائيز);
  hb_serialize_context_t e (بفر_e, بفر_سائيز);
  populate_serializer_complex_2 (&a);

  گراف_t گراف = گراف_t::create (a.object_گراف ()).value ();
  hb_always_assert (گراف.duplicate (4, 1, خطأ).is_ok ());
  گراف.normalize();

  e.شروعات_serialize();
  بنانشان mn = جوڙيو_object("mn", 2, &e);
  بنانشان jkl_2 = جوڙيو_object("jkl", 3, &e);

  شروعات_object("ghi", 3, &e);
  جوڙيو_وٿي(jkl_2, &e);
  بنانشان ghi = e.ڪڍو_pack(خطأ);

  شروعات_object("def", 3, &e);
  جوڙيو_وٿي(ghi, &e);
  بنانشان def = e.ڪڍو_pack(خطأ);

  بنانشان jkl_1 = جوڙيو_object("jkl", 3, &e);

  شروعات_object("abc", 3, &e);
  جوڙيو_وٿي(def, &e);
  جوڙيو_وٿي(jkl_1, &e);
  جوڙيو_وٿي(mn, &e);
  e.ڪڍو_pack(خطأ);

  گراف_t expected = گراف_t::create (e.object_گراف ()).value ();
  expected.normalize();

  أكد(expected == گراف);

  حرر (بفر_a);
  حرر (بفر_e);
}

ساكن فراغ test_duplicate_interior ()
{
  size_t بفر_سائيز = 100;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_complex_3 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();
  hb_always_assert (گراف.duplicate (3, 2, خطأ).is_ok ());

  hb_always_assert(لڙي_ڀيٽ (گراف.object (6).سر_2, "jkl", 3) == 0);
  hb_always_assert(گراف.object (6).real_links.ڊيگهائي == 1);
  hb_always_assert(گراف.object (6).real_links[0].objidx == 0);

  hb_always_assert(لڙي_ڀيٽ (گراف.object (5).سر_2, "abc", 3) == 0);
  hb_always_assert(گراف.object (5).real_links.ڊيگهائي == 3);
  hb_always_assert(گراف.object (5).real_links[0].objidx == 4);
  hb_always_assert(گراف.object (5).real_links[1].objidx == 2);
  hb_always_assert(گراف.object (5).real_links[2].objidx == 1);

  hb_always_assert(لڙي_ڀيٽ (گراف.object (4).سر_2, "def", 3) == 0);
  hb_always_assert(گراف.object (4).real_links.ڊيگهائي == 1);
  hb_always_assert(گراف.object (4).real_links[0].objidx == 3);

  hb_always_assert(لڙي_ڀيٽ (گراف.object (3).سر_2, "ghi", 3) == 0);
  hb_always_assert(گراف.object (3).real_links.ڊيگهائي == 1);
  hb_always_assert(گراف.object (3).real_links[0].objidx == 6);

  hb_always_assert(لڙي_ڀيٽ (گراف.object (2).سر_2, "jkl", 3) == 0);
  hb_always_assert(گراف.object (2).real_links.ڊيگهائي == 1);
  hb_always_assert(گراف.object (2).real_links[0].objidx == 0);

  hb_always_assert(لڙي_ڀيٽ (گراف.object (1).سر_2, "mn", 2) == 0);
  hb_always_assert(گراف.object (1).real_links.ڊيگهائي == 0);

  hb_always_assert(لڙي_ڀيٽ (گراف.object (0).سر_2, "opqrst", 6) == 0);
  hb_always_assert(گراف.object (0).real_links.ڊيگهائي == 0);

  حرر (بفر);
}

ساكن فراغ
test_serialize ()
{
  size_t بفر_سائيز = 100;
  فراغ* بفر_1 = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c1 (بفر_1, بفر_سائيز);
  populate_serializer_simple (&c1);
  hb_بائيٽون_t expected = c1.copy_بائيٽون ();

  گراف_t گراف = گراف_t::create (c1.object_گراف ()).value ();
  hb_blob_t* ٻاهريون = گراف::serialize (گراف).value ();
  حرر (بفر_1);

  hb_بائيٽون_t actual = ٻاهريون->as_بائيٽون ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (ٻاهريون);
}

ساكن فراغ test_will_حد_کان_مٿي_1 ()
{
  size_t بفر_سائيز = 100;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);

  populate_serializer_complex_2 (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (!گراف::will_حد_کان_مٿي (گراف, خالي_اشارو).value ());

  حرر (بفر);
}

ساكن فراغ test_will_حد_کان_مٿي_2 ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_حد_کان_مٿي (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (گراف::will_حد_کان_مٿي (گراف, خالي_اشارو).value ());

  حرر (بفر);
}

ساكن فراغ test_will_حد_کان_مٿي_3 ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_dedup_حد_کان_مٿي (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (گراف::will_حد_کان_مٿي (گراف, خالي_اشارو).value ());

  حرر (بفر);
}

ساكن فراغ test_resolve_overflows_ذريعي_ترتيب_ڏيو ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_حد_کان_مٿي (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* ٻاهريون = hb_resolve_overflows (c.object_گراف (), HB_ٽيگ_NONE);
  hb_always_assert (ٻاهريون);
  hb_بائيٽون_t نتيجو = ٻاهريون->as_بائيٽون ();
  hb_always_assert (نتيجو.ڊيگهائي == (80000 + 3 + 3 * 2));

  حرر (بفر);
  hb_blob_destroy (ٻاهريون);
}

ساكن فراغ test_resolve_overflows_ذريعي_duplication ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_dedup_حد_کان_مٿي (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* ٻاهريون = hb_resolve_overflows (c.object_گراف (), HB_ٽيگ_NONE);
  hb_always_assert (ٻاهريون);
  hb_بائيٽون_t نتيجو = ٻاهريون->as_بائيٽون ();
  hb_always_assert (نتيجو.ڊيگهائي == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  حرر (بفر);
  hb_blob_destroy (ٻاهريون);
}

ساكن فراغ test_resolve_overflows_ذريعي_گڻو_duplications ()
{
  size_t بفر_سائيز = 300000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_گڻو_dedup_حد_کان_مٿي (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_blob_t* ٻاهريون = hb_resolve_overflows (c.object_گراف (), HB_ٽيگ_NONE, 5);
  hb_always_assert (ٻاهريون);

  حرر (بفر);
  hb_blob_destroy (ٻاهريون);
}

ساكن فراغ test_resolve_overflows_ذريعي_space_assignment ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_spaces (&c, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_spaces (&e, خطأ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_ذريعي_isolation ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_isolation_حد_کان_مٿي (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.وٿي_حد_کان_مٿي ());
  hb_blob_t* ٻاهريون = hb_resolve_overflows (c.object_گراف (), HB_ٽيگ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (ٻاهريون);
  hb_بائيٽون_t نتيجو = ٻاهريون->as_بائيٽون ();
  hb_always_assert (نتيجو.ڊيگهائي == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  حرر (بفر);
  hb_blob_destroy (ٻاهريون);
}

ساكن فراغ test_resolve_overflows_ذريعي_isolation_with_recursive_duplication ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_isolation_حد_کان_مٿي_complex (&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_isolation_حد_کان_مٿي_complex_expected (&e);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_ذريعي_isolating_16bit_space ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_spaces_16bit_connection (&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_spaces_16bit_connection_expected (&e);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_ذريعي_isolating_16bit_space_2 ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_short_and_wide_subgraph_پاڙ (&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_short_and_wide_subgraph_پاڙ_expected (&e);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_ذريعي_isolation_spaces ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_isolation_حد_کان_مٿي_spaces (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.وٿي_حد_کان_مٿي ());
  hb_blob_t* ٻاهريون = hb_resolve_overflows (c.object_گراف (), HB_ٽيگ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (ٻاهريون);
  hb_بائيٽون_t نتيجو = ٻاهريون->as_بائيٽون ();

  بنانشان expected_ڊيگهائي = 3 + 2 * 60000; // objects
  expected_ڊيگهائي += 2 * 4 + 2 * 2; // links
  hb_always_assert (نتيجو.ڊيگهائي == expected_ڊيگهائي);

  حرر (بفر);
  hb_blob_destroy (ٻاهريون);
}

ساكن فراغ test_resolve_mixed_overflows_ذريعي_isolation_spaces ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_24_and_32_ٻٽو_انگ_offsets (&c);
  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_always_assert (c.وٿي_حد_کان_مٿي ());
  hb_blob_t* ٻاهريون = hb_resolve_overflows (c.object_گراف (), HB_ٽيگ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (ٻاهريون);
  hb_بائيٽون_t نتيجو = ٻاهريون->as_بائيٽون ();

  بنانشان expected_ڊيگهائي =
      // Objects
      7 +
      4 * 40000;

  expected_ڊيگهائي +=
      // Links
      2 * 4 +  // 32
      4 * 3 +  // 24
      4 * 2;   // 16

  hb_always_assert (نتيجو.ڊيگهائي == expected_ڊيگهائي);

  حرر (بفر);
  hb_blob_destroy (ٻاهريون);
}

ساكن فراغ test_resolve_with_extension_promotion ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_extension_promotion (&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_extension_promotion (&e, 3);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             سچ);
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_shared_extension_promotion ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_extension_promotion (&c, 0, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_extension_promotion (&e, 3, سچ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             سچ);
  حرر (بفر);
  حرر (expected_بفر);
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
ساكن فراغ test_resolve_with_extension_promotion_1_2 ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_extension_promotion (&c, 0, خطأ, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_extension_promotion (&e, 3, خطأ, سچ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             سچ);
  حرر (بفر);
  حرر (expected_بفر);
}
#جيڪڏهن_پورو

ساكن فراغ test_resolve_with_repeated_extension_promotion ()
{
  size_t بفر_سائيز = 1000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);

  c.شروعات_serialize<حرف> ();
  بنانشان subtable = جوڙيو_object ("subt", 4, &c);
  شروعات_lookup (5, 2, &c);
  جوڙيو_وٿي (subtable, &c);
  جوڙيو_وٿي (subtable, &c);
  بنانشان lookup = پڄاڻي_lookup (&c);
  بنانشان lookup_list = جوڙيو_lookup_list (&lookup, 1, &c);
  جوڙيو_gsubgpos_سرو (lookup_list, &c);
  c.پڇاڙي_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();
  خودڪار context = گراف::gsubgpos_گراف_context_t::create (HB_ٽيگ ('G', 'S', 'U', 'B'),
                                                          گراف).value ();
  hb_always_assert (context.lookups.get_population () == 1);
  بنانشان lookup_اشاريو = *context.lookups.ڪنجيون ();
  گراف::Lookup* lookup_جدول_2 = context.lookups.get (lookup_اشاريو);
  hb_always_assert (lookup_جدول_2->make_extension (context, lookup_اشاريو).is_ok ());

  ثابت خودڪار& lookup_راس = گراف.vertices_[lookup_اشاريو];
  hb_always_assert (lookup_راس.obj ().real_links.ڊيگهائي == 2);
  بنانشان extension_اشاريو = lookup_راس.obj ().real_links[0].objidx;
  hb_always_assert (lookup_راس.obj ().real_links[1].objidx == extension_اشاريو);

  خودڪار* extension = گراف.vertices_[extension_اشاريو].as<
      گراف::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
  بنانشان promoted_subtable = extension->get_subtable_اشاريو (گراف, extension_اشاريو).value ();
  hb_always_assert (گراف.vertices_[promoted_subtable].جدول_سائيز () == 4);
  hb_always_assert (گراف.update_parents ().is_ok ());
  hb_always_assert (گراف.vertices_[extension_اشاريو].incoming_ڪنارا_2 () == 2);
  hb_always_assert (گراف.vertices_[promoted_subtable].incoming_ڪنارا_2 () == 1);

  hb_blob_t* ٻاهريون = گراف::serialize (گراف).value ();
  hb_blob_destroy (ٻاهريون);
  حرر (بفر);
}

ساكن فراغ test_resolve_with_basic_pair_pos_1_ورهايو ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, سچ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_extension_pair_pos_1_ورهايو ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, سچ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_basic_pair_pos_2_ورهايو ()
{
  size_t بفر_سائيز = 300000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, سچ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_close_منزل_حد_بندي_pair_pos_2_ورهايو ()
{
  size_t بفر_سائيز = 300000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, سچ, خطأ, خطأ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, سچ, خطأ, خطأ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_pair_pos_2_ورهايو_with_device_tables ()
{
  size_t بفر_سائيز = 300000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, خطأ, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, سچ, سچ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_basic_mark_بنياد_pos_1_ورهايو ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_large_mark_بنياد_pos_1 <40, 10, 110, 1>(&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_large_mark_بنياد_pos_1 <40, 10, 110, 2>(&e);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'P', 'O', 'S'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_basic_liga_ورهايو ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, خطأ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, خطأ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_ورهايو_move ()
{
  size_t بفر_سائيز = 400000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, سچ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_ورهايو_overlapping_clone ()
{
  size_t بفر_سائيز = 400000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_large_liga_overlapping_clone_نتيجو(&e);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_ورهايو_shared_جدول ()
{
  size_t بفر_سائيز = 400000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, سچ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_with_liga_ورهايو_shared_coverage ()
{
  size_t بفر_سائيز = 400000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             سچ,
                             HB_ٽيگ('G', 'S', 'U', 'B'));
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_ذريعي_splitting_spaces ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_ورهايو_spaces (&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_ورهايو_spaces_expected (&e);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  حرر (بفر);
  حرر (expected_بفر);

}

ساكن فراغ test_resolve_overflows_ذريعي_splitting_spaces_2 ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_ورهايو_spaces_2 (&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_ورهايو_spaces_expected_2 (&e);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ test_resolve_overflows_ذريعي_ترجيح ()
{
  size_t بفر_سائيز = 160000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_ترجيح_حد_کان_مٿي (&c);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_ترجيح_حد_کان_مٿي_expected (&e);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  حرر (بفر);
  حرر (expected_بفر);
}


ساكن فراغ test_virtual_link ()
{
  size_t بفر_سائيز = 100;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_virtual_link (&c);

  hb_blob_t* ٻاهريون = hb_resolve_overflows (c.object_گراف (), HB_ٽيگ_NONE);
  hb_always_assert (ٻاهريون);

  hb_بائيٽون_t نتيجو = ٻاهريون->as_بائيٽون ();
  hb_always_assert (نتيجو.ڊيگهائي == 5 + 4 * 2);
  hb_always_assert (نتيجو[0]  == 'a');
  hb_always_assert (نتيجو[5]  == 'c');
  hb_always_assert (نتيجو[8]  == 'e');
  hb_always_assert (نتيجو[9]  == 'b');
  hb_always_assert (نتيجو[12] == 'd');

  حرر (بفر);
  hb_blob_destroy (ٻاهريون);
}

ساكن فراغ
test_shared_ڳنڍ_with_virtual_links ()
{
  size_t بفر_سائيز = 100;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);

  c.شروعات_serialize<حرف> ();

  بنانشان obj_b = جوڙيو_object ("b", 1, &c);
  بنانشان obj_c = جوڙيو_object ("c", 1, &c);

  شروعات_object ("d", 1, &c);
  جوڙيو_virtual_وٿي (obj_b, &c);
  بنانشان obj_d_1 = c.ڪڍو_pack ();

  شروعات_object ("d", 1, &c);
  جوڙيو_virtual_وٿي (obj_c, &c);
  بنانشان obj_d_2 = c.ڪڍو_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  شروعات_object ("a", 1, &c);
  جوڙيو_وٿي (obj_b, &c);
  جوڙيو_وٿي (obj_c, &c);
  جوڙيو_وٿي (obj_d_1, &c);
  جوڙيو_وٿي (obj_d_2, &c);
  c.ڪڍو_pack ();
  c.پڇاڙي_serialize ();

  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links.ڊيگهائي == 2);
  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_گراف() [obj_d_1]->virtual_links[1].objidx == obj_c);
  حرر(بفر);
}

ساكن فراغ
test_repack_آخري ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_with_repack_آخري (&c, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_with_repack_آخري (&e, خطأ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_repack_last",
                             c,
                             e,
                             20,
                             خطأ,
                             HB_ٽيگ('a', 'b', 'c', 'd'),
                             سچ);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ
test_dont_duplicate_virtual ()
{
  size_t بفر_سائيز = 200000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (بفر);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_virtual (&c, سچ);

  فراغ* expected_بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_always_assert (expected_بفر);
  hb_serialize_context_t e (expected_بفر, بفر_سائيز);
  populate_serializer_virtual (&e, خطأ);

  لاڳيتو_resolve_حد_کان_مٿي_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             خطأ,
                             HB_ٽيگ('a', 'b', 'c', 'd'),
                             سچ);

  حرر (بفر);
  حرر (expected_بفر);
}

ساكن فراغ
test_deep_گراف_traversal ()
{
  size_t بفر_سائيز = 1000000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);

  c.شروعات_serialize ();
  بنانشان prev = جوڙيو_object ("leaf", 4, &c);
  لكل (بنانشان i = 0; i < 2000; i++)
  {
    شروعات_object ("node", 4, &c);
    جوڙيو_وٿي (prev, &c);
    prev = c.ڪڍو_pack (خطأ);
  }
  c.پڇاڙي_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // Test find_subgraph (set)
  hb_سيٽ_t گهميل_سيٽ;
  hb_always_assert (گراف.ڳوليو_subgraph (گراف.پاڙ_idx (), گهميل_سيٽ).is_ok ());
  hb_always_assert (گهميل_سيٽ.get_population () == 2001);

  // Test find_subgraph_size
  hb_سيٽ_t سائيز_سيٽ;
  size_t sz = گراف.ڳوليو_subgraph_سائيز (گراف.پاڙ_idx (), سائيز_سيٽ).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.سيٽ (گراف.پاڙ_idx (), 1);
  hb_always_assert (گراف.ڳوليو_subgraph (گراف.پاڙ_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (گراف.سونپيو_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t اشاريو_map;
  hb_always_assert (گراف.duplicate_subgraph (گراف.پاڙ_idx (), اشاريو_map).is_ok ());
  hb_always_assert (اشاريو_map.get_population () == 2001);

  حرر (بفر);
}

ساكن فراغ
test_32bit_roots_traversal ()
{
  size_t بفر_سائيز = 1000;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);

  c.شروعات_serialize ();
  بنانشان leaf1 = جوڙيو_object ("l1", 2, &c);
  بنانشان leaf2 = جوڙيو_object ("l2", 2, &c);

  شروعات_object ("mid", 3, &c);
  جوڙيو_wide_وٿي (leaf1, &c);
  بنانشان mid = c.ڪڍو_pack (خطأ);

  شروعات_object ("root", 4, &c);
  جوڙيو_وٿي (mid, &c);
  جوڙيو_wide_وٿي (leaf2, &c);
  c.ڪڍو_pack (خطأ);
  c.پڇاڙي_serialize ();

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  hb_سيٽ_t roots;
  hb_always_assert (گراف.ڳوليو_32bit_roots (گراف.پاڙ_idx (), roots).is_ok ());
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
  بفر_سائيز = 1000;
  بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c2 (بفر, بفر_سائيز);

  c2.شروعات_serialize ();
  بنانشان b = جوڙيو_object ("b", 1, &c2);

  شروعات_object ("a", 1, &c2);
  جوڙيو_wide_وٿي (b, &c2);
  بنانشان a = c2.ڪڍو_pack (خطأ);

  شروعات_object ("r", 1, &c2);
  جوڙيو_وٿي (a, &c2);
  جوڙيو_wide_وٿي (a, &c2);
  c2.ڪڍو_pack (خطأ);

  c2.پڇاڙي_serialize ();

  گراف_t graph2 = گراف_t::create (c2.object_گراف ()).value ();

  hb_سيٽ_t roots2;
  hb_always_assert (graph2.ڳوليو_32bit_roots (graph2.پاڙ_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  حرر (بفر);
}

ساكن فراغ test_invalid_link_objidx ()
{
  size_t بفر_سائيز = 100;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_complex_2 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // Poison a link to point to (unsigned) -1
  خودڪار links = گراف.vertices_[گراف.پاڙ_idx ()].real_links_writer();
  links->objidx = (بنانشان) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!گراف.ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو ().is_ok ());

  حرر (بفر);
}

ساكن فراغ test_invalid_link_جوڙيو_and_move ()
{
  size_t بفر_سائيز = 100;
  فراغ* بفر = يادگيري_ڏيو (بفر_سائيز);
  hb_serialize_context_t c (بفر, بفر_سائيز);
  populate_serializer_complex_2 (&c);

  گراف_t گراف = گراف_t::create (c.object_گراف ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!گراف.جوڙيو_link (&dummy, 0, (بنانشان) -1).is_ok ());

  // move_child with non-existent offset
  خودڪار res = گراف.move_ٻار (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  حرر (بفر);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

صحيح
مکيه (صحيح argc, حرف **argv)
{
  test_invalid_link_objidx ();
  test_invalid_link_جوڙيو_and_move ();
  test_serialize ();
  test_ترتيب_ڏيو_سڀ_کان_ٿورو ();
  test_will_حد_کان_مٿي_1 ();
  test_will_حد_کان_مٿي_2 ();
  test_will_حد_کان_مٿي_3 ();
  test_resolve_overflows_ذريعي_ترتيب_ڏيو ();
  test_resolve_overflows_ذريعي_duplication ();
  test_resolve_overflows_ذريعي_گڻو_duplications ();
  test_resolve_overflows_ذريعي_ترجيح ();
  test_resolve_overflows_ذريعي_space_assignment ();
  test_resolve_overflows_ذريعي_isolation ();
  test_resolve_overflows_ذريعي_isolation_with_recursive_duplication ();
  test_resolve_overflows_ذريعي_isolation_spaces ();
  test_resolve_overflows_ذريعي_isolating_16bit_space ();
  test_resolve_overflows_ذريعي_isolating_16bit_space_2 ();
  test_resolve_overflows_ذريعي_splitting_spaces ();
  test_resolve_overflows_ذريعي_splitting_spaces_2 ();
  test_resolve_mixed_overflows_ذريعي_isolation_spaces ();
  test_duplicate_پن ();
  test_duplicate_interior ();
  test_virtual_link ();
  test_repack_آخري();
  test_shared_ڳنڍ_with_virtual_links ();
  test_resolve_with_extension_promotion ();
  test_resolve_with_shared_extension_promotion ();
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  test_resolve_with_extension_promotion_1_2 ();
#جيڪڏهن_پورو
  test_resolve_with_repeated_extension_promotion ();
  test_resolve_with_basic_pair_pos_1_ورهايو ();
  test_resolve_with_extension_pair_pos_1_ورهايو ();
  test_resolve_with_basic_pair_pos_2_ورهايو ();
  test_resolve_with_pair_pos_2_ورهايو_with_device_tables ();
  test_resolve_with_close_منزل_حد_بندي_pair_pos_2_ورهايو ();
  test_resolve_with_basic_mark_بنياد_pos_1_ورهايو ();
  test_resolve_with_basic_liga_ورهايو ();
  test_resolve_with_liga_ورهايو_move ();
  test_dont_duplicate_virtual ();
  test_resolve_with_liga_ورهايو_overlapping_clone ();
  test_resolve_with_liga_ورهايو_shared_جدول ();
  test_resolve_with_liga_ورهايو_shared_coverage ();
  test_deep_گراف_traversal ();
  test_32bit_roots_traversal ();

  // TODO(grieger): have run overflow tests compare graph equality not final packed binary.
  // TODO(grieger): split test where multiple subtables in one lookup are split to test link ordering.
  // TODO(grieger): split test where coverage table in subtable that is being split is shared.
  // TODO(grieger): test with extensions already mixed in as well.
  // TODO(grieger): test two layer ext promotion setup.
  // TODO(grieger): test sorting by subtables per byte in ext. promotion.
}
