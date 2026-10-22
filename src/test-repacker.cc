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

#ਅਨ੍ਤਰ੍ਭਾਵ <cstdint>
#ਅਨ੍ਤਰ੍ਭਾਵ <string>

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-repacker.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "graph/serialize.hh"

ਸਥਿਰ ਖਾਲੀ extend (ਅਚਲ ਅੱਖਰ* value,
                    ਬਿਨਾਨਿਸ਼ਾਨੀ len,
                    hb_serialize_context_t* c)
{
  ਅੱਖਰ* obj = c->allocate_ਮਾਪ<ਅੱਖਰ> (len);
  hb_memcpy (obj, value, len);
}

ਸਥਿਰ ਖਾਲੀ ਸ਼ੁਰੂ_object(ਅਚਲ ਅੱਖਰ* ਟੈਗ_2,
                         ਬਿਨਾਨਿਸ਼ਾਨੀ len,
                         hb_serialize_context_t* c)
{
  c->push ();
  extend (ਟੈਗ_2, len, c);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_object(ਅਚਲ ਅੱਖਰ* ਟੈਗ_2,
                           ਬਿਨਾਨਿਸ਼ਾਨੀ len,
                           hb_serialize_context_t* c,
                           ਤਾਰਕਿਕ shared = ਝੂਠ)
{
  ਸ਼ੁਰੂ_object (ਟੈਗ_2, len, c);
  ਵਾਪਸ c->ਕੱਢੋ_pack (shared);
}


ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ id,
                        hb_serialize_context_t* c)
{
  OT::Offset16* ਵਿੱਥ = c->ਸ਼ੁਰੂ_embed<OT::Offset16> ();
  c->extend_min (ਵਿੱਥ);
  c->ਜੋੜੋ_link (*ਵਿੱਥ, id);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_24_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ id,
                           hb_serialize_context_t* c)
{
  OT::Offset24* ਵਿੱਥ = c->ਸ਼ੁਰੂ_embed<OT::Offset24> ();
  c->extend_min (ਵਿੱਥ);
  c->ਜੋੜੋ_link (*ਵਿੱਥ, id);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_wide_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ id,
                             hb_serialize_context_t* c)
{
  OT::Offset32* ਵਿੱਥ = c->ਸ਼ੁਰੂ_embed<OT::Offset32> ();
  c->extend_min (ਵਿੱਥ);
  c->ਜੋੜੋ_link (*ਵਿੱਥ, id);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list,
                                 hb_serialize_context_t* c)
{
  ਅੱਖਰ ਸਿਰਲੇਖ[] = {
    0, 1, // major
    0, 0, // minor
    0, 0, // script list
    0, 0, // feature list
  };

  ਸ਼ੁਰੂ_object (ਸਿਰਲੇਖ, 8, c);
  ਜੋੜੋ_ਵਿੱਥ (lookup_list, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ_1_2 (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list,
                                     hb_serialize_context_t* c)
{
  ਅੱਖਰ ਸਿਰਲੇਖ[] = {
    0, 1, 0, 2, //ਸੰਸਕਰਣ
    0, 0, // script list
    0, 0, // feature list
    0, 0, // lookup list
    0, 0, 0, 0, // feature variations
    0, 0, 0, 0, // script list 2
    0, 0, 0, 0, // feature list 2
  };

  ਸ਼ੁਰੂ_object (ਸਿਰਲੇਖ, ਆਕਾਰ (ਸਿਰਲੇਖ), c);
  ਜੋੜੋ_wide_ਵਿੱਥ (lookup_list, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_lookup_list (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ* lookups,
                                 ਅੱਖਰ ਗਿਣਤੀ_2,
                                 hb_serialize_context_t* c)
{
  ਅੱਖਰ lookup_ਗਿਣਤੀ[] = {0, ਗਿਣਤੀ_2};
  ਸ਼ੁਰੂ_object  ((ਅੱਖਰ *) &lookup_ਗਿਣਤੀ, 2, c);

  ਲਈ (ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੋੜੋ_ਵਿੱਥ (lookups[i], c);

  ਵਾਪਸ c->ਕੱਢੋ_pack (ਝੂਠ);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_lookup_list_1_2 (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ* lookups,
                                     ਅੱਖਰ ਗਿਣਤੀ_2,
                                     hb_serialize_context_t* c)
{
  ਅੱਖਰ lookup_ਗਿਣਤੀ[] = {0, ਗਿਣਤੀ_2};
  ਸ਼ੁਰੂ_object ((ਅੱਖਰ *) &lookup_ਗਿਣਤੀ, 2, c);

  ਲਈ (ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੋੜੋ_wide_ਵਿੱਥ (lookups[i], c);

  ਵਾਪਸ c->ਕੱਢੋ_pack (ਝੂਠ);
}

ਸਥਿਰ ਖਾਲੀ ਸ਼ੁਰੂ_lookup (int8_t type,
                          int8_t ਅੰਕ_subtables,
                          hb_serialize_context_t* c)
{
  ਅੱਖਰ lookup[] = {
    0, (ਅੱਖਰ)type, // type
    0, (ਅੱਖਰ)2, // flag
    0, (ਅੱਖਰ)ਅੰਕ_subtables, // num subtables
  };

  ਸ਼ੁਰੂ_object (lookup, 6, c);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਤ_lookup (hb_serialize_context_t* c)
{
  ਅੱਖਰ filter[] = {0, 0};
  extend (filter, 2, c);
  ਵਾਪਸ c->ਕੱਢੋ_pack (ਝੂਠ);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_extension (ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ,
                               uint8_t type,
                               hb_serialize_context_t* c)
{
  ਅੱਖਰ ext[] = {
    0, 1,
    0, (ਅੱਖਰ) type,
  };

  ਸ਼ੁਰੂ_object (ext, 4, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (ਬੱਚਾ, c);

  ਵਾਪਸ c->ਕੱਢੋ_pack (ਝੂਠ);

}

// Adds coverage table fro [start, end]
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_coverage (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ,
                              hb_serialize_context_t* c,
                              ਤਾਰਕਿਕ shared = ਝੂਠ)
{
  ਜੇ (ਸਿਰਾ - ਸ਼ੁਰੂ == 0) {
    uint8_t coverage[] = {
      0, 1, // format
      0, 1, //ਗਿਣਤੀ

      (uint8_t) ((ਸ਼ੁਰੂ >> 8) & 0xFF),
      (uint8_t) (ਸ਼ੁਰੂ & 0xFF), // glyph[0]
    };
    ਵਾਪਸ ਜੋੜੋ_object ((ਅੱਖਰ*) coverage, 6, c, shared);
  }

  ਜੇ (ਸਿਰਾ - ਸ਼ੁਰੂ == 1)
  {
    uint8_t coverage[] = {
      0, 1, // format
      0, 2, //ਗਿਣਤੀ

      (uint8_t) ((ਸ਼ੁਰੂ >> 8) & 0xFF),
      (uint8_t) (ਸ਼ੁਰੂ & 0xFF), // glyph[0]

      (uint8_t) ((ਸਿਰਾ >> 8) & 0xFF),
      (uint8_t) (ਸਿਰਾ & 0xFF), // glyph[1]
    };
    ਵਾਪਸ ਜੋੜੋ_object ((ਅੱਖਰ*) coverage, 8, c, shared);
  }

  uint8_t coverage[] = {
    0, 2, // format
    0, 1, // range count

    (uint8_t) ((ਸ਼ੁਰੂ >> 8) & 0xFF),
    (uint8_t) (ਸ਼ੁਰੂ & 0xFF), //ਸ਼ੁਰੂ

    (uint8_t) ((ਸਿਰਾ >> 8) & 0xFF),
    (uint8_t) (ਸਿਰਾ & 0xFF), //ਸਿਰਾ

    0, 0,
  };
  ਵਾਪਸ ਜੋੜੋ_object ((ਅੱਖਰ*) coverage, 10, c, shared);
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ It>
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_coverage (It it,
                              hb_serialize_context_t* c)
{
  c->push ();
  OT::Layout::Common::Coverage_serialize (c, it);
  ਵਾਪਸ c->ਕੱਢੋ_pack (ਝੂਠ);
}

// Adds a class that maps glyphs from [start_glyph, end_glyph)
// to classes 1...n
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_class_def (uint16_t ਸ਼ੁਰੂ_glyph,
                               uint16_t ਸਿਰਾ_glyph,
                               hb_serialize_context_t* c)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਸਿਰਾ_glyph - ਸ਼ੁਰੂ_glyph;
  uint8_t ਸਿਰਲੇਖ[] = {
    0, 1, // format

    (uint8_t) ((ਸ਼ੁਰੂ_glyph >> 8) & 0xFF),
    (uint8_t) (ਸ਼ੁਰੂ_glyph & 0xFF), // start_glyph

    (uint8_t) ((ਗਿਣਤੀ_2 >> 8) & 0xFF),
    (uint8_t) (ਗਿਣਤੀ_2 & 0xFF), //ਗਿਣਤੀ
  };

  ਸ਼ੁਰੂ_object ((ਅੱਖਰ*) ਸਿਰਲੇਖ, 6, c);
  ਲਈ (uint16_t i = 1; i <= ਗਿਣਤੀ_2; i++)
  {
    uint8_t class_ਮੁੱਲ_2[] = {
      (uint8_t) ((i >> 8) & 0xFF),
      (uint8_t) (i & 0xFF), //ਗਿਣਤੀ
    };
    extend ((ਅੱਖਰ*) class_ਮੁੱਲ_2, 2, c);
  }

  ਵਾਪਸ c->ਕੱਢੋ_pack (ਝੂਠ);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_pair_pos_1 (ਬਿਨਾਨਿਸ਼ਾਨੀ* pair_sets,
                                ਅੱਖਰ ਗਿਣਤੀ_2,
                                ਬਿਨਾਨਿਸ਼ਾਨੀ coverage,
                                hb_serialize_context_t* c)
{
  ਅੱਖਰ format[] = {
    0, 1
  };

  ਸ਼ੁਰੂ_object (format, 2, c);
  ਜੋੜੋ_ਵਿੱਥ (coverage, c);

  ਅੱਖਰ ਮੁੱਲ_format[] = {
    0, 0,
    0, 0,
    0, ਗਿਣਤੀ_2,
  };
  extend (ਮੁੱਲ_format, 6, c);

  ਲਈ (ਅੱਖਰ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੋੜੋ_ਵਿੱਥ (pair_sets[(ਬਿਨਾਨਿਸ਼ਾਨੀ) i], c);

  ਵਾਪਸ c->ਕੱਢੋ_pack (ਝੂਠ);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_liga_ਸੈੱਟ_ਸਿਰਲੇਖ (ਅੱਖਰ liga_ਗਿਣਤੀ,
                                 hb_serialize_context_t* c)
{
  uint8_t liga_ਗਿਣਤੀ_ਬਾਈਟਾਂ[] = {
    (uint8_t) (0xFF & (liga_ਗਿਣਤੀ >> 8)),
    (uint8_t) (0xFF & liga_ਗਿਣਤੀ),
  };

  ਸ਼ੁਰੂ_object ((ਅਚਲ ਅੱਖਰ*) liga_ਗਿਣਤੀ_ਬਾਈਟਾਂ, 2, c);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_liga_ਸਿਰਲੇਖ (ਬਿਨਾਨਿਸ਼ਾਨੀ coverage, ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਗਿਣਤੀ, hb_serialize_context_t* c)
{
  uint8_t format[] = {0, 1};
  ਸ਼ੁਰੂ_object ((ਅਚਲ ਅੱਖਰ*) format, 2, c);
  ਜੋੜੋ_ਵਿੱਥ(coverage, c);

  uint8_t liga_ਸੈੱਟ_ਗਿਣਤੀ_ਬਾਈਟਾਂ[] = {
    (uint8_t) (0xFF & (liga_ਸੈੱਟ_ਗਿਣਤੀ >> 8)),
    (uint8_t) (0xFF & liga_ਸੈੱਟ_ਗਿਣਤੀ),
  };

  extend((ਅਚਲ ਅੱਖਰ *) liga_ਸੈੱਟ_ਗਿਣਤੀ_ਬਾਈਟਾਂ, 2, c);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_pair_pos_2 (ਬਿਨਾਨਿਸ਼ਾਨੀ starting_class,
                                ਬਿਨਾਨਿਸ਼ਾਨੀ coverage,
                                ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_1, uint16_t class_def_1_ਗਿਣਤੀ,
                                ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_2, uint16_t class_def_2_ਗਿਣਤੀ,
                                ਬਿਨਾਨਿਸ਼ਾਨੀ* device_tables,
                                hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 2
  };

  ਸ਼ੁਰੂ_object ((ਅੱਖਰ*) format, 2, c);
  ਜੋੜੋ_ਵਿੱਥ (coverage, c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਮੁੱਲਾਂ_3 = 4;
  uint8_t format1 = 0x01 | 0x02 | 0x08;
  uint8_t format2 = 0x04;
  ਜੇ (device_tables) {
    format2 |= 0x20;
    ਅੰਕ_ਮੁੱਲਾਂ_3 += 1;
  }
  uint8_t ਮੁੱਲ_format[] = {
    0, format1,
    0, format2,
  };

  extend ((ਅੱਖਰ*) ਮੁੱਲ_format, 4, c);

  ਜੋੜੋ_ਵਿੱਥ (class_def_1, c);
  ਜੋੜੋ_ਵਿੱਥ (class_def_2, c);

  uint8_t class_counts[] = {
    (uint8_t) ((class_def_1_ਗਿਣਤੀ >> 8) & 0xFF),
    (uint8_t) (class_def_1_ਗਿਣਤੀ & 0xFF),
    (uint8_t) ((class_def_2_ਗਿਣਤੀ >> 8) & 0xFF),
    (uint8_t) (class_def_2_ਗਿਣਤੀ & 0xFF),
  };
  extend ((ਅੱਖਰ*) class_counts, 4, c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਰਿਕਾਰਡ = class_def_2_ਗਿਣਤੀ * ਅੰਕ_ਮੁੱਲਾਂ_3 * 2;
  uint8_t* ਰਿਕਾਰਡ = (uint8_t*) ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ (1, ਅੰਕ_ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਰਿਕਾਰਡ);
  ਪੂਰਨ device_ਸੂਚਕਾਂਕ = 0;
  ਲਈ (uint16_t i = 0; i < class_def_1_ਗਿਣਤੀ; i++)
  {

    ਲਈ (uint16_t j = 0; j < class_def_2_ਗਿਣਤੀ; j++)
    {
      ਲਈ (ਪੂਰਨ k = 0; k < 4; k++) {
        uint8_t value[] = {
          (uint8_t) (i + starting_class),
          (uint8_t) (i + starting_class),
        };
        extend ((ਅੱਖਰ*) value, 2, c);
      }

      ਜੇ (device_tables) {
        ਜੋੜੋ_ਵਿੱਥ (device_tables[device_ਸੂਚਕਾਂਕ++], c);
      }
    }
  }
  ਮੁਕਤ (ਰਿਕਾਰਡ);

  ਵਾਪਸ c->ਕੱਢੋ_pack (ਝੂਠ);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_mark_ਅਧਾਰ_pos_1 (ਬਿਨਾਨਿਸ਼ਾਨੀ mark_coverage,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_coverage,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ mark_array,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_array,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਗਿਣਤੀ,
                                     hb_serialize_context_t* c)
{
  uint8_t format[] = {
    0, 1
  };

  ਸ਼ੁਰੂ_object ((ਅੱਖਰ*) format, 2, c);
  ਜੋੜੋ_ਵਿੱਥ (mark_coverage, c);
  ਜੋੜੋ_ਵਿੱਥ (ਅਧਾਰ_coverage, c);

  uint8_t ਗਿਣਤੀ_2[] = {
    (uint8_t) ((class_ਗਿਣਤੀ >> 8) & 0xFF),
    (uint8_t) (class_ਗਿਣਤੀ & 0xFF),
  };
  extend ((ਅੱਖਰ*) ਗਿਣਤੀ_2, 2, c);

  ਜੋੜੋ_ਵਿੱਥ (mark_array, c);
  ਜੋੜੋ_ਵਿੱਥ (ਅਧਾਰ_array, c);

  ਵਾਪਸ c->ਕੱਢੋ_pack (ਝੂਠ);
}

ਸਾਂਚਾ<ਪੂਰਨ mark_ਗਿਣਤੀ,
    ਪੂਰਨ class_ਗਿਣਤੀ,
    ਪੂਰਨ ਅਧਾਰ_ਗਿਣਤੀ,
    ਪੂਰਨ ਸਾਰਣੀ_ਗਿਣਤੀ_2>
ਬਣਤਰ Mark_ਅਧਾਰ_Pos_Buffers
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_anchors[class_ਗਿਣਤੀ * ਅਧਾਰ_ਗਿਣਤੀ];
  ਬਿਨਾਨਿਸ਼ਾਨੀ mark_anchors[mark_ਗਿਣਤੀ];
  uint8_t anchor_buffers[class_ਗਿਣਤੀ * ਅਧਾਰ_ਗਿਣਤੀ + 100];
  uint8_t class_ਬਫਰ[class_ਗਿਣਤੀ * 2];

  Mark_ਅਧਾਰ_Pos_Buffers(hb_serialize_context_t* c)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਆਕਾਰ(anchor_buffers) / 2; i++)
    {
      OT::HBUINT16* value = (OT::HBUINT16*) (&anchor_buffers[2*i]);
      *value = i;
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < class_ਗਿਣਤੀ * ਅਧਾਰ_ਗਿਣਤੀ; i++)
    {
      ਅਧਾਰ_anchors[i] = ਜੋੜੋ_object ((ਅੱਖਰ*) &anchor_buffers[i], 100, c);
      ਜੇ (i < class_ਗਿਣਤੀ) {
        class_ਬਫਰ[i*2] = (uint8_t) ((i >> 8) & 0xFF);
        class_ਬਫਰ[i*2 + 1] = (uint8_t) (i & 0xFF);
      }
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < mark_ਗਿਣਤੀ; i++)
    {
      mark_anchors[i] = ਜੋੜੋ_object ((ਅੱਖਰ*) &anchor_buffers[i], 4, c);
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ create_mark_ਅਧਾਰ_pos_1 (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਾਰਣੀ_ਸੂਚਕਾਂਕ, hb_serialize_context_t* c)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ class_ਪ੍ਰਤੀ_ਸਾਰਣੀ = class_ਗਿਣਤੀ / ਸਾਰਣੀ_ਗਿਣਤੀ_2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ mark_ਪ੍ਰਤੀ_class = mark_ਗਿਣਤੀ / class_ਗਿਣਤੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_class = class_ਪ੍ਰਤੀ_ਸਾਰਣੀ * ਸਾਰਣੀ_ਸੂਚਕਾਂਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_class = class_ਪ੍ਰਤੀ_ਸਾਰਣੀ * (ਸਾਰਣੀ_ਸੂਚਕਾਂਕ + 1) - 1;

    // baseArray
    uint8_t ਅਧਾਰ_ਗਿਣਤੀ_ਬਫਰ[] = {
      (uint8_t) ((ਅਧਾਰ_ਗਿਣਤੀ >> 8) & 0xFF),
      (uint8_t) (ਅਧਾਰ_ਗਿਣਤੀ & 0xFF),

    };
    ਸ਼ੁਰੂ_object ((ਅੱਖਰ*) ਅਧਾਰ_ਗਿਣਤੀ_ਬਫਰ, 2, c);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ = 0; ਅਧਾਰ < ਅਧਾਰ_ਗਿਣਤੀ; ਅਧਾਰ++)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ klass = ਸ਼ੁਰੂ_class; klass <= ਸਿਰਾ_class; klass++)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਅਧਾਰ * class_ਗਿਣਤੀ + klass;
        ਜੋੜੋ_ਵਿੱਥ (ਅਧਾਰ_anchors[i], c);
      }
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_array = c->ਕੱਢੋ_pack (ਝੂਠ);

    // markArray
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_marks = class_ਪ੍ਰਤੀ_ਸਾਰਣੀ * mark_ਪ੍ਰਤੀ_class;
    uint8_t mark_ਗਿਣਤੀ_ਬਫਰ[] = {
      (uint8_t) ((ਅੰਕ_marks >> 8) & 0xFF),
      (uint8_t) (ਅੰਕ_marks & 0xFF),
    };
    ਸ਼ੁਰੂ_object ((ਅੱਖਰ*) mark_ਗਿਣਤੀ_ਬਫਰ, 2, c);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ mark = 0; mark < mark_ਗਿਣਤੀ; mark++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ klass = mark % class_ਗਿਣਤੀ;
      ਜੇ (klass < ਸ਼ੁਰੂ_class || klass > ਸਿਰਾ_class) ਜਾਰੀ;
      klass -= ਸ਼ੁਰੂ_class;

      extend ((ਅੱਖਰ*) &class_ਬਫਰ[2 * klass], 2, c);
      ਜੋੜੋ_ਵਿੱਥ (mark_anchors[mark], c);
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ mark_array = c->ਕੱਢੋ_pack (ਝੂਠ);

    // markCoverage
    ਸਵੈਚਾਲਿਤ it =
        + hb_range ((hb_codepoint_t) mark_ਗਿਣਤੀ)
        | hb_filter ([&] (hb_codepoint_t mark) {
          ਬਿਨਾਨਿਸ਼ਾਨੀ klass = mark % class_ਗਿਣਤੀ;
          ਵਾਪਸ klass >= class_ਪ੍ਰਤੀ_ਸਾਰਣੀ * ਸਾਰਣੀ_ਸੂਚਕਾਂਕ &&
              klass < class_ਪ੍ਰਤੀ_ਸਾਰਣੀ * (ਸਾਰਣੀ_ਸੂਚਕਾਂਕ + 1);
        })
        ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ mark_coverage = ਜੋੜੋ_coverage (it, c);

    // baseCoverage
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ_coverage = ਜੋੜੋ_coverage (10, 10 + ਅਧਾਰ_ਗਿਣਤੀ - 1, c);

    ਵਾਪਸ ਜੋੜੋ_mark_ਅਧਾਰ_pos_1 (mark_coverage,
                                ਅਧਾਰ_coverage,
                                mark_array,
                                ਅਧਾਰ_array,
                                class_ਪ੍ਰਤੀ_ਸਾਰਣੀ,
                                c);
  }
};

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test (ਅਚਲ ਅੱਖਰ* name,
                                       hb_serialize_context_t& overflowing,
                                       hb_serialize_context_t& expected,
                                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_iterations = 0,
                                       ਤਾਰਕਿਕ recalculate_extensions = ਝੂਠ,
                                       hb_ਟੈਗ_t ਟੈਗ_2 = HB_ਟੈਗ ('G', 'S', 'U', 'B'),
                                       ਤਾਰਕਿਕ check_ਦੁਆਧਾਰੀ_equivalence = ਝੂਠ)
{
  ਲਿਖੋ (">>> Testing overflowing resolution for %s\n",
          name);

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (overflowing.object_ਗ੍ਰਾਫ ()).value ();

  ਗ੍ਰਾਫ_t expected_ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (expected.object_ਗ੍ਰਾਫ ()).value ();
  ਜੇ (ਗ੍ਰਾਫ::will_ਉਛਾਲ (expected_ਗ੍ਰਾਫ).value ())
  {
    ਜੇ (check_ਦੁਆਧਾਰੀ_equivalence) {
      ਲਿਖੋ("when binary equivalence checking is enabled, the expected graph cannot overflow.");
      hb_always_assert(!check_ਦੁਆਧਾਰੀ_equivalence);
    }
    hb_always_assert (expected_ਗ੍ਰਾਫ.ਸੌਂਪੋ_spaces ().is_ok ());
    hb_always_assert (expected_ਗ੍ਰਾਫ.ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ ().is_ok ());
  }

  // Check that overflow resolution succeeds
  hb_always_assert (overflowing.ਵਿੱਥ_ਉਛਾਲ ());
  ਸਵੈਚਾਲਿਤ r = hb_resolve_ਗ੍ਰਾਫ_overflows (ਟੈਗ_2,
                                      ਅੰਕ_iterations,
                                      recalculate_extensions,
                                      ਗ੍ਰਾਫ);
  ਜੇ (r.is_err())
    ਲਿਖੋ("run_resolve_overflow_test: overflow resolution failed, cause: %s\n", ਗ੍ਰਾਫ::to_string(r.error()));
  hb_always_assert (r.is_ok());

  // Check the graphs can be serialized.
  hb_blob_t* out1 = ਗ੍ਰਾਫ::serialize (ਗ੍ਰਾਫ).value ();
  hb_always_assert (out1);
  hb_blob_t* out2 = ਗ੍ਰਾਫ::serialize (expected_ਗ੍ਰਾਫ).value ();
  hb_always_assert (out2);
  ਜੇ (check_ਦੁਆਧਾਰੀ_equivalence) {
    ਬਿਨਾਨਿਸ਼ਾਨੀ l1, l2;
    ਅਚਲ ਅੱਖਰ* d1 = hb_blob_get_ਅੰਕੜੇ(out1, &l1);
    ਅਚਲ ਅੱਖਰ* d2 = hb_blob_get_ਅੰਕੜੇ(out2, &l2);

    ਤਾਰਕਿਕ ਮੇਲ = (l1 == l2) && (memcmp(d1, d2, l1) == 0);
    ਜੇ (!ਮੇਲ) {
      ਲਿਖੋ("## Result:\n");
      ਗ੍ਰਾਫ.print();
      ਲਿਖੋ("## Expected:\n");
      expected_ਗ੍ਰਾਫ.print();
      hb_always_assert(ਮੇਲ);
    }
  }

  hb_blob_destroy (out1);
  hb_blob_destroy (out2);

  // Check the graphs are equivalent
  ਗ੍ਰਾਫ.normalize ();
  expected_ਗ੍ਰਾਫ.normalize ();
  ਜੇ (!(ਗ੍ਰਾਫ == expected_ਗ੍ਰਾਫ)) {
    ਲਿਖੋ("## Expected:\n");
    expected_ਗ੍ਰਾਫ.print();
    ਲਿਖੋ("## Result:\n");
    ਗ੍ਰਾਫ.print();
  }
  hb_always_assert (ਗ੍ਰਾਫ == expected_ਗ੍ਰਾਫ);
}

ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_virtual_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ id,
                                hb_serialize_context_t* c)
{
  c->ਜੋੜੋ_virtual_link (id);
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_simple (hb_serialize_context_t* c)
{
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_1 = ਜੋੜੋ_object ("ghi", 3, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_2 = ਜੋੜੋ_object ("def", 3, c);

  ਸ਼ੁਰੂ_object ("abc", 3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_2, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_1, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_virtual (hb_serialize_context_t* c, ਤਾਰਕਿਕ with_ਉਛਾਲ)
{
  std::string large_string(50000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_4, obj_5;
  ਜੇ (with_ਉਛਾਲ) {
    obj_5 = ਜੋੜੋ_object("55555", 5, c);
    obj_4 = ਜੋੜੋ_object(large_string.c_str(), 50000, c);
  } ਨਹੀਂਤਾਂ {
    obj_4 = ਜੋੜੋ_object(large_string.c_str(), 50000, c);
    obj_5 = ਜੋੜੋ_object("55555", 5, c);
  }

  ਸ਼ੁਰੂ_object(large_string.c_str(), 20000, c);
  ਜੋੜੋ_ਵਿੱਥ(obj_5, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_3 = c->ਕੱਢੋ_pack(ਝੂਠ);

  ਸ਼ੁਰੂ_object("2", 2, c);
  ਜੋੜੋ_virtual_ਵਿੱਥ(obj_5, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_2 = c->ਕੱਢੋ_pack(ਝੂਠ);

  // obj 1
  ਸ਼ੁਰੂ_object("1", 1, c);
  ਜੋੜੋ_ਵਿੱਥ(obj_2, c);
  ਜੋੜੋ_ਵਿੱਥ(obj_3, c);
  ਜੋੜੋ_ਵਿੱਥ(obj_4, c);
  c->ਕੱਢੋ_pack(ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_ਉਛਾਲ (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_1 = ਜੋੜੋ_object (large_string.c_str(), 10000, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_2 = ਜੋੜੋ_object (large_string.c_str(), 20000, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_3 = ਜੋੜੋ_object (large_string.c_str(), 50000, c);

  ਸ਼ੁਰੂ_object ("abc", 3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_2, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_1, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_ਤਰਜੀਹ_ਉਛਾਲ (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = ਜੋੜੋ_object ("e", 1, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = ਜੋੜੋ_object ("d", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 50000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 20000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_ਤਰਜੀਹ_ਉਛਾਲ_expected (hb_serialize_context_t* c)
{
  std::string large_string(50000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = ਜੋੜੋ_object ("e", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 50000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = ਜੋੜੋ_object ("d", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 20000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}


ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_dedup_ਉਛਾਲ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_1 = ਜੋੜੋ_object ("def", 3, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 60000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_1, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_2 = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 10000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_2, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_1, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_ਗੁਣਜ_dedup_ਉਛਾਲ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੱਤੀ = ਜੋੜੋ_object("def", 3, c);

  constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_mid_nodes = 20;
  ਬਿਨਾਨਿਸ਼ਾਨੀ mid_nodes[ਅੰਕ_mid_nodes];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_mid_nodes; i++) {
    ਸ਼ੁਰੂ_object(large_string.c_str(), 10000 + i, c);
    ਜੋੜੋ_ਵਿੱਥ(ਪੱਤੀ, c);
    mid_nodes[i] = c->ਕੱਢੋ_pack(ਝੂਠ);
  }

  ਸ਼ੁਰੂ_object("abc", 3, c);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_mid_nodes; i++) {
    ਜੋੜੋ_wide_ਵਿੱਥ(mid_nodes[i], c);
  }
  c->ਕੱਢੋ_pack(ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_isolation_ਉਛਾਲ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_4 = ਜੋੜੋ_object ("4", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 60000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_4, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_3 = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 10000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_4, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_2 = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("1", 1, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_2, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_isolation_ਉਛਾਲ_complex (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object ("e", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("d", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 60000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_h = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 60000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_h, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 10000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_g = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 11000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_i = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_g, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_i, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_isolation_ਉਛਾਲ_complex_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();


  // space 1

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f_ਅਭਾਜ = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object ("e", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e_ਅਭਾਜ = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("d", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d_ਅਭਾਜ = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 60000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_h = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 60000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_h, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  // space 0

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object ("e", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);


  ਸ਼ੁਰੂ_object ("d", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 11000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_i = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 10000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_g = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_g, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_i, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_isolation_ਉਛਾਲ_spaces (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = ਜੋੜੋ_object ("f", 1, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 60000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack ();

  ਸ਼ੁਰੂ_object (large_string.c_str(), 60000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack ();


  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c, c);
  c->ਕੱਢੋ_pack ();

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_repack_ਆਖਰੀ (hb_serialize_context_t* c, ਤਾਰਕਿਕ with_ਉਛਾਲ)
{
  std::string large_string(70000, 'c');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();
  c->push();

  // Obj E
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e_1, obj_e_2;
  ਜੇ (with_ਉਛਾਲ) {
    obj_e_1 = ਜੋੜੋ_object("a", 1, c);
    obj_e_2 = obj_e_1;
  } ਨਹੀਂਤਾਂ {
    obj_e_2 = ਜੋੜੋ_object("a", 1, c);
  }

  // Obj D
  c->push();
  ਜੋੜੋ_ਵਿੱਥ(obj_e_2, c);
  extend(large_string.c_str(), 30000, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack(ਝੂਠ);

  ਜੋੜੋ_ਵਿੱਥ(obj_d, c);
  hb_always_assert(c->ਆਖਰੀ_added_ਬੱਚਾ_ਸੂਚਕਾਂਕ() == obj_d);

  ਜੇ (!with_ਉਛਾਲ) {
    obj_e_1 = ਜੋੜੋ_object("a", 1, c);
  }

  // Obj C
  c->push();
  ਜੋੜੋ_ਵਿੱਥ(obj_e_1, c);
  extend(large_string.c_str(), 40000, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack(ਝੂਠ);

  ਜੋੜੋ_ਵਿੱਥ(obj_c, c);

  // Obj B
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = ਜੋੜੋ_object("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ(obj_b, c);

  // Obj A
  c->repack_ਆਖਰੀ(obj_d);
  c->ਕੱਢੋ_pack(ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_spaces (hb_serialize_context_t* c, ਤਾਰਕਿਕ with_ਉਛਾਲ)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_i;

  ਜੇ (with_ਉਛਾਲ)
    obj_i = ਜੋੜੋ_object ("i", 1, c);

  // Space 2
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_h = ਜੋੜੋ_object ("h", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 30000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_h, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  // Space 1
  ਜੇ (!with_ਉਛਾਲ)
    obj_i = ਜੋੜੋ_object ("i", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 30000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_i, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_g = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 30000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_i, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("d", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_g, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_d, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_spaces_16bit_connection (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_g = ਜੋੜੋ_object ("g", 1, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_h = ਜੋੜੋ_object ("h", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_g, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_h, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("d", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_h, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_d, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_spaces_16bit_connection_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_g_ਅਭਾਜ = ਜੋੜੋ_object ("g", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_g_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e_ਅਭਾਜ = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_h_ਅਭਾਜ = ਜੋੜੋ_object ("h", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_h_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("d", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_g = ਜੋੜੋ_object ("g", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_g, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_h = ਜੋੜੋ_object ("h", 1, c);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_h, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_d, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize ();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_short_and_wide_subgraph_ਜੜ੍ਹ (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = ਜੋੜੋ_object ("e", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_d, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_short_and_wide_subgraph_ਜੜ੍ਹ_expected (hb_serialize_context_t* c)
{
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e_ਅਭਾਜ = ਜੋੜੋ_object ("e", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c_ਅਭਾਜ = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = ਜੋੜੋ_object ("e", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);


  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c_ਅਭਾਜ, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_d, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_ਵੰਡੋ_spaces (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_ਵੰਡੋ_spaces_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space
  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_ਵੰਡੋ_spaces_expected (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f_ਅਭਾਜ = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_ਵੰਡੋ_spaces_expected_2 (hb_serialize_context_t* c)
{
  // Overflow needs to be resolved by splitting the single space

  std::string large_string(70000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  // Space 2

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f_double_ਅਭਾਜ = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f_double_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d_ਅਭਾਜ = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b_ਅਭਾਜ = c->ਕੱਢੋ_pack (ਝੂਠ);

  // Space 1

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f_ਅਭਾਜ = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f_ਅਭਾਜ, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  // Space 0

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = ਜੋੜੋ_object ("f", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str(), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  //ਜੜ੍ਹ
  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_b_ਅਭਾਜ, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_c, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_complex_2 (hb_serialize_context_t* c)
{
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_5 = ਜੋੜੋ_object ("mn", 2, c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_4 = ਜੋੜੋ_object ("jkl", 3, c);

  ਸ਼ੁਰੂ_object ("ghi", 3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_4, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_3 = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("def", 3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_3, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_2 = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("abc", 3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_2, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_4, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_5, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_complex_3 (hb_serialize_context_t* c)
{
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_6 = ਜੋੜੋ_object ("opqrst", 6, c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_5 = ਜੋੜੋ_object ("mn", 2, c);

  ਸ਼ੁਰੂ_object ("jkl", 3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_6, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_4 = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("ghi", 3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_4, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_3 = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("def", 3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_3, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_2 = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("abc", 3, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_2, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_4, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_5, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_virtual_link (hb_serialize_context_t* c)
{
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = ਜੋੜੋ_object ("d", 1, c);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("e", 1, c);
  ਜੋੜੋ_virtual_ਵਿੱਥ (obj_b, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("c", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_24_and_32_ਦੋਆਧਾਰੀ_ਅੰਕ_offsets (hb_serialize_context_t* c)
{
  std::string large_string(60000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_f = ਜੋੜੋ_object ("f", 1, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_g = ਜੋੜੋ_object ("g", 1, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_j = ਜੋੜੋ_object ("j", 1, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_k = ਜੋੜੋ_object ("k", 1, c);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_f, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_g, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_j, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_h = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object (large_string.c_str (), 40000, c);
  ਜੋੜੋ_ਵਿੱਥ (obj_k, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_i = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("e", 1, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_h, c);
  ਜੋੜੋ_wide_ਵਿੱਥ (obj_i, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_e = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("b", 1, c);
  ਜੋੜੋ_24_ਵਿੱਥ (obj_c, c);
  ਜੋੜੋ_24_ਵਿੱਥ (obj_d, c);
  ਜੋੜੋ_24_ਵਿੱਥ (obj_e, c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = c->ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("a", 1, c);
  ਜੋੜੋ_24_ਵਿੱਥ (obj_b, c);
  c->ਕੱਢੋ_pack (ਝੂਠ);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_extension_promotion (hb_serialize_context_t* c,
                                              ਪੂਰਨ ਅੰਕ_extensions = 0,
                                              ਤਾਰਕਿਕ shared_subtables = ਝੂਠ,
                                              ਤਾਰਕਿਕ ਸੰਸਕਰਣ_1_2 = ਝੂਠ)
{
  constexpr ਪੂਰਨ ਅੰਕ_lookups = 5;
  constexpr ਪੂਰਨ ਅੰਕ_subtables = ਅੰਕ_lookups * 2;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookups[ਅੰਕ_lookups];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtables[ਅੰਕ_subtables];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ extensions[ਅੰਕ_subtables];

  std::string large_string(60000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();


  ਲਈ (ਪੂਰਨ i = ਅੰਕ_subtables - 1; i >= 0; i--)
    subtables[i] = ਜੋੜੋ_object(large_string.c_str (), 15000 + i, c);

  ਲਈ (ਪੂਰਨ i = ਅੰਕ_subtables - 1;
       i >= (ਅੰਕ_lookups - ਅੰਕ_extensions) * 2;
       i--)
  {
    extensions[i] = ਜੋੜੋ_extension (subtables[i], 5, c);
  }

  ਲਈ (ਪੂਰਨ i = ਅੰਕ_lookups - 1; i >= 0; i--)
  {
    ਤਾਰਕਿਕ is_ext = (i >= (ਅੰਕ_lookups - ਅੰਕ_extensions));

    ਸ਼ੁਰੂ_lookup (is_ext ? (ਅੱਖਰ) 7 : (ਅੱਖਰ) 5,
                  shared_subtables && i > 2 ? 3 : 2,
                  c);

    ਜੇ (is_ext) {
      ਜੇ (shared_subtables && i > 2) {
        ਜੋੜੋ_ਵਿੱਥ (extensions[i * 2 - 1], c);
      }
      ਜੋੜੋ_ਵਿੱਥ (extensions[i * 2], c);
      ਜੋੜੋ_ਵਿੱਥ (extensions[i * 2 + 1], c);
    } ਨਹੀਂਤਾਂ {
      ਜੇ (shared_subtables && i > 2) {
        ਜੋੜੋ_ਵਿੱਥ (subtables[i * 2 - 1], c);
      }
      ਜੋੜੋ_ਵਿੱਥ (subtables[i * 2], c);
      ਜੋੜੋ_ਵਿੱਥ (subtables[i * 2 + 1], c);
    }

    lookups[i] = ਅੰਤ_lookup (c);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list = ਸੰਸਕਰਣ_1_2
                         ? ਜੋੜੋ_lookup_list_1_2 (lookups, ਅੰਕ_lookups, c)
                         : ਜੋੜੋ_lookup_list (lookups, ਅੰਕ_lookups, c);

  ਜੇ (ਸੰਸਕਰਣ_1_2)
    ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ_1_2 (lookup_list, c);
  ਨਹੀਂਤਾਂ
    ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (lookup_list, c);

  c->ਸਿਰਾ_serialize();
}

ਸਾਂਚਾ<ਪੂਰਨ ਅੰਕ_pair_pos_1, ਪੂਰਨ ਅੰਕ_pair_ਸੈੱਟ>
ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_large_pair_pos_1 (hb_serialize_context_t* c,
                                           ਤਾਰਕਿਕ as_extension = ਝੂਠ)
{
  std::string large_string(60000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  constexpr ਪੂਰਨ ਕੁੱਲ_pair_ਸੈੱਟ = ਅੰਕ_pair_pos_1 * ਅੰਕ_pair_ਸੈੱਟ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ pair_ਸੈੱਟ_2[ਕੁੱਲ_pair_ਸੈੱਟ];
  ਬਿਨਾਨਿਸ਼ਾਨੀ coverage[ਅੰਕ_pair_pos_1];
  ਬਿਨਾਨਿਸ਼ਾਨੀ pair_pos_1[ਅੰਕ_pair_pos_1];

  ਲਈ (ਪੂਰਨ i = ਅੰਕ_pair_pos_1 - 1; i >= 0; i--)
  {
    ਲਈ (ਪੂਰਨ j = (i + 1) * ਅੰਕ_pair_ਸੈੱਟ - 1; j >= i * ਅੰਕ_pair_ਸੈੱਟ; j--)
      pair_ਸੈੱਟ_2[j] = ਜੋੜੋ_object (large_string.c_str (), 30000 + j, c);

    coverage[i] = ਜੋੜੋ_coverage (i * ਅੰਕ_pair_ਸੈੱਟ,
                                (i + 1) * ਅੰਕ_pair_ਸੈੱਟ - 1, c);

    pair_pos_1[i] = ਜੋੜੋ_pair_pos_1 (&pair_ਸੈੱਟ_2[i * ਅੰਕ_pair_ਸੈੱਟ],
                                    ਅੰਕ_pair_ਸੈੱਟ,
                                    coverage[i],
                                    c);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ pair_pos_2 = ਜੋੜੋ_object (large_string.c_str(), 200, c);

  ਜੇ (as_extension) {
    pair_pos_2 = ਜੋੜੋ_extension (pair_pos_2, 2, c);
    ਲਈ (ਪੂਰਨ i = ਅੰਕ_pair_pos_1 - 1; i >= 0; i--)
      pair_pos_1[i] = ਜੋੜੋ_extension (pair_pos_1[i], 2, c);
  }

  ਸ਼ੁਰੂ_lookup (as_extension ? 9 : 2, 1 + ਅੰਕ_pair_pos_1, c);

  ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_pair_pos_1; i++)
    ਜੋੜੋ_ਵਿੱਥ (pair_pos_1[i], c);
  ਜੋੜੋ_ਵਿੱਥ (pair_pos_2, c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup = ਅੰਤ_lookup (c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list = ਜੋੜੋ_lookup_list (&lookup, 1, c);

  ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (lookup_list, c);

  c->ਸਿਰਾ_serialize();
}

ਸਾਂਚਾ<ਪੂਰਨ ਅੰਕ_pair_pos_2, ਪੂਰਨ ਅੰਕ_class_1, ਪੂਰਨ ਅੰਕ_class_2>
ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_large_pair_pos_2 (hb_serialize_context_t* c,
                                           ਤਾਰਕਿਕ as_extension = ਝੂਠ,
                                           ਤਾਰਕਿਕ with_device_tables = ਝੂਠ,
                                           ਤਾਰਕਿਕ extra_ਸਾਰਣੀ = ਸੱਚ)
{
  std::string large_string(100000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ coverage[ਅੰਕ_pair_pos_2];
  ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_1[ਅੰਕ_pair_pos_2];
  ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_2[ਅੰਕ_pair_pos_2];
  ਬਿਨਾਨਿਸ਼ਾਨੀ pair_pos_2[ਅੰਕ_pair_pos_2];

  ਬਿਨਾਨਿਸ਼ਾਨੀ* device_tables = (ਬਿਨਾਨਿਸ਼ਾਨੀ*) ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਅੰਕ_pair_pos_2 * ਅੰਕ_class_1 * ਅੰਕ_class_2,
                                                ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ));

  // Total glyphs = num_class_1 * num_pair_pos_2
  ਲਈ (ਪੂਰਨ i = ਅੰਕ_pair_pos_2 - 1; i >= 0; i--)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_glyph = 5 + i * ਅੰਕ_class_1;
    ਜੇ (ਅੰਕ_class_2 >= ਅੰਕ_class_1)
    {
      class_def_2[i] = ਜੋੜੋ_class_def (11,
                                      10 + ਅੰਕ_class_2, c);
      class_def_1[i] = ਜੋੜੋ_class_def (ਸ਼ੁਰੂ_glyph + 1,
                                      ਸ਼ੁਰੂ_glyph + ਅੰਕ_class_1,
                                      c);
    } ਨਹੀਂਤਾਂ {
      class_def_1[i] = ਜੋੜੋ_class_def (ਸ਼ੁਰੂ_glyph + 1,
                                      ਸ਼ੁਰੂ_glyph + ਅੰਕ_class_1,
                                      c);
      class_def_2[i] = ਜੋੜੋ_class_def (11,
                                      10 + ਅੰਕ_class_2, c);
    }

    coverage[i] = ਜੋੜੋ_coverage (ਸ਼ੁਰੂ_glyph,
                                ਸ਼ੁਰੂ_glyph + ਅੰਕ_class_1 - 1,
                                c);

    ਜੇ (with_device_tables)
    {
      ਲਈ(ਪੂਰਨ j = (i + 1) * ਅੰਕ_class_1 * ਅੰਕ_class_2 - 1;
          j >= i * ਅੰਕ_class_1 * ਅੰਕ_class_2;
          j--)
      {
        uint8_t ਸਾਰਣੀ[] = {
          (uint8_t) ((j >> 8) & 0xFF),
          (uint8_t) (j & 0xFF),
        };
        device_tables[j] = ਜੋੜੋ_object ((ਅੱਖਰ*) ਸਾਰਣੀ, 2, c);
      }
    }

    pair_pos_2[i] = ਜੋੜੋ_pair_pos_2 (1 + i * ਅੰਕ_class_1,
                                    coverage[i],
                                    class_def_1[i], ਅੰਕ_class_1,
                                    class_def_2[i], ਅੰਕ_class_2,
                                    with_device_tables
                                    ? &device_tables[i * ਅੰਕ_class_1 * ਅੰਕ_class_2]
                                    : ਖਾਲੀ_ਸੂਚਕ,
                                    c);
  }


  ਬਿਨਾਨਿਸ਼ਾਨੀ pair_pos_1 = 0;
  ਜੇ (extra_ਸਾਰਣੀ) pair_pos_1 = ਜੋੜੋ_object (large_string.c_str(), 100000, c);

  ਜੇ (as_extension) {
    ਲਈ (ਪੂਰਨ i = ਅੰਕ_pair_pos_2 - 1; i >= 0; i--)
      pair_pos_2[i] = ਜੋੜੋ_extension (pair_pos_2[i], 2, c);

    ਜੇ (extra_ਸਾਰਣੀ)
      pair_pos_1 = ਜੋੜੋ_extension (pair_pos_1, 2, c);
  }

  ਸ਼ੁਰੂ_lookup (as_extension ? 9 : 2, 1 + ਅੰਕ_pair_pos_2, c);

  ਜੇ (extra_ਸਾਰਣੀ)
    ਜੋੜੋ_ਵਿੱਥ (pair_pos_1, c);

  ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_pair_pos_2; i++)
    ਜੋੜੋ_ਵਿੱਥ (pair_pos_2[i], c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup = ਅੰਤ_lookup (c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list = ਜੋੜੋ_lookup_list (&lookup, 1, c);

  ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (lookup_list, c);

  c->ਸਿਰਾ_serialize();

  ਮੁਕਤ (device_tables);
}

ਸਾਂਚਾ<ਪੂਰਨ mark_ਗਿਣਤੀ,
    ਪੂਰਨ class_ਗਿਣਤੀ,
    ਪੂਰਨ ਅਧਾਰ_ਗਿਣਤੀ,
    ਪੂਰਨ ਸਾਰਣੀ_ਗਿਣਤੀ_2>
ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_large_mark_ਅਧਾਰ_pos_1 (hb_serialize_context_t* c)
{
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  Mark_ਅਧਾਰ_Pos_Buffers<mark_ਗਿਣਤੀ, class_ਗਿਣਤੀ, ਅਧਾਰ_ਗਿਣਤੀ, ਸਾਰਣੀ_ਗਿਣਤੀ_2> buffers (c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ mark_ਅਧਾਰ_pos[ਸਾਰਣੀ_ਗਿਣਤੀ_2];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਸਾਰਣੀ_ਗਿਣਤੀ_2; i++)
    mark_ਅਧਾਰ_pos[i] = buffers.create_mark_ਅਧਾਰ_pos_1 (i, c);

  ਲਈ (ਪੂਰਨ i = 0; i < ਸਾਰਣੀ_ਗਿਣਤੀ_2; i++)
    mark_ਅਧਾਰ_pos[i] = ਜੋੜੋ_extension (mark_ਅਧਾਰ_pos[i], 4, c);

  ਸ਼ੁਰੂ_lookup (9, ਸਾਰਣੀ_ਗਿਣਤੀ_2, c);

  ਲਈ (ਪੂਰਨ i = 0; i < ਸਾਰਣੀ_ਗਿਣਤੀ_2; i++)
    ਜੋੜੋ_ਵਿੱਥ (mark_ਅਧਾਰ_pos[i], c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup = ਅੰਤ_lookup (c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list = ਜੋੜੋ_lookup_list (&lookup, 1, c);

  ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (lookup_list, c);

  c->ਸਿਰਾ_serialize();
}

ਸਾਂਚਾ<ਬਿਨਾਨਿਸ਼ਾਨੀ Lig_Subst_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ Liga_ਸੈੱਟ_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ Liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ Liga_ਮਾਪ>
ਸਥਿਰ ਖਾਲੀ populate_serializer_with_large_ligsubst(hb_serialize_context_t* c,
                                                    ਤਾਰਕਿਕ sequential_liga_sets,
                                                    ਤਾਰਕਿਕ shared,
                                                    ਬਿਨਾਨਿਸ਼ਾਨੀ *liga_subst_id,
                                                    ਤਾਰਕਿਕ unique_lig_str = ਝੂਠ)
{
  ਅੱਖਰ ch = 'a';

  ਬਿਨਾਨਿਸ਼ਾਨੀ liga[Liga_ਸੈੱਟ_ਗਿਣਤੀ * Liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ];
  ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ[Liga_ਸੈੱਟ_ਗਿਣਤੀ];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ l = 0; l < Lig_Subst_ਗਿਣਤੀ; l++) {
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਸ਼ੁਰੂ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਸਿਰਾ = Liga_ਸੈੱਟ_ਗਿਣਤੀ - 1;
    ਜੇ (sequential_liga_sets) {
      coverage_ਸ਼ੁਰੂ = l * Liga_ਸੈੱਟ_ਗਿਣਤੀ;
      coverage_ਸਿਰਾ = (l + 1) * Liga_ਸੈੱਟ_ਗਿਣਤੀ - 1;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage = ਜੋੜੋ_coverage(coverage_ਸ਼ੁਰੂ, coverage_ਸਿਰਾ, c, shared);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < Liga_ਸੈੱਟ_ਗਿਣਤੀ; i++) {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < Liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ; j++)
      {
        std::string large_string(100000, ch);
        ਸ਼ੁਰੂ_object (large_string.c_str(), Liga_ਮਾਪ, c);
        ਜੇ (unique_lig_str)
          ch += 1;
        ਜੋੜੋ_virtual_ਵਿੱਥ(coverage, c);
        liga[i * Liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ + j] = c->ਕੱਢੋ_pack (shared);
      }

      ਜੋੜੋ_liga_ਸੈੱਟ_ਸਿਰਲੇਖ(Liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ, c);
      ਜੋੜੋ_virtual_ਵਿੱਥ(coverage, c);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < Liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ; j++)
      {
        ਜੋੜੋ_ਵਿੱਥ(liga[i * Liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ + j], c);
      }

      liga_ਸੈੱਟ[i] = c->ਕੱਢੋ_pack(shared);
    }

    ਜੋੜੋ_liga_ਸਿਰਲੇਖ(coverage, Liga_ਸੈੱਟ_ਗਿਣਤੀ, c);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < Liga_ਸੈੱਟ_ਗਿਣਤੀ; i++)
    {
      ਜੋੜੋ_ਵਿੱਥ(liga_ਸੈੱਟ[i], c);
    }

    liga_subst_id[l] = c->ਕੱਢੋ_pack(shared);
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ l = 0; l < Lig_Subst_ਗਿਣਤੀ; l++) {
    liga_subst_id[l] = ਜੋੜੋ_extension(liga_subst_id[l], 4, c);
  }
}

ਸਾਂਚਾ<ਬਿਨਾਨਿਸ਼ਾਨੀ liga_subst_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਮਾਪ>
ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_large_liga (hb_serialize_context_t* c, ਤਾਰਕਿਕ sequential_liga_sets)
{
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ liga_subst[liga_subst_ਗਿਣਤੀ];
  populate_serializer_with_large_ligsubst<liga_subst_ਗਿਣਤੀ, liga_ਸੈੱਟ_ਗਿਣਤੀ, liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ, liga_ਮਾਪ>(c, sequential_liga_sets, ਝੂਠ, liga_subst);

  ਸ਼ੁਰੂ_lookup (7, liga_subst_ਗਿਣਤੀ, c);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ l = 0; l < liga_subst_ਗਿਣਤੀ; l++) {
    ਜੋੜੋ_ਵਿੱਥ(liga_subst[l], c);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup = ਅੰਤ_lookup (c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list = ਜੋੜੋ_lookup_list (&lookup, 1, c);

  ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (lookup_list, c);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_large_liga_overlapping_clone_ਨਤੀਜਾ (hb_serialize_context_t* c)
{
  std::string large_string(100000, 'a');
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਮਾਪ = 30000;

  ਬਿਨਾਨਿਸ਼ਾਨੀ liga[6];
  ਬਿਨਾਨਿਸ਼ਾਨੀ liga_subst[3];
  ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ[2];

  // LigaSubst 3
  ਬਿਨਾਨਿਸ਼ਾਨੀ coverage = ਜੋੜੋ_coverage(1, 1, c);
  ਲਈ (ਪੂਰਨ i = 1; i >= 0; i--) {
    ਸ਼ੁਰੂ_object (large_string.c_str(), liga_ਮਾਪ, c);
    ਜੋੜੋ_virtual_ਵਿੱਥ(coverage, c);
    liga[i] = c->ਕੱਢੋ_pack (ਝੂਠ);
  }

  ਜੋੜੋ_liga_ਸੈੱਟ_ਸਿਰਲੇਖ(2, c);
  ਜੋੜੋ_virtual_ਵਿੱਥ(coverage, c);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 2; i++)
    ਜੋੜੋ_ਵਿੱਥ(liga[i], c);
  liga_ਸੈੱਟ[0] = c->ਕੱਢੋ_pack(ਝੂਠ);

  ਜੋੜੋ_liga_ਸਿਰਲੇਖ(coverage, 1, c);
  ਜੋੜੋ_ਵਿੱਥ(liga_ਸੈੱਟ[0], c);
  liga_subst[2] = c->ਕੱਢੋ_pack(ਝੂਠ);

  // LigaSubst 2
  coverage = ਜੋੜੋ_coverage(0, 1, c);
  ਲਈ (ਪੂਰਨ i = 1; i >= 0; i--) {
    ਸ਼ੁਰੂ_object (large_string.c_str(), liga_ਮਾਪ, c);
    ਜੋੜੋ_virtual_ਵਿੱਥ(coverage, c);
    liga[i] = c->ਕੱਢੋ_pack (ਝੂਠ);
  }

  ਜੋੜੋ_liga_ਸੈੱਟ_ਸਿਰਲੇਖ(1, c);
  ਜੋੜੋ_virtual_ਵਿੱਥ(coverage, c);
  ਜੋੜੋ_ਵਿੱਥ(liga[1], c);
  liga_ਸੈੱਟ[1] = c->ਕੱਢੋ_pack(ਝੂਠ);

  ਜੋੜੋ_liga_ਸੈੱਟ_ਸਿਰਲੇਖ(1, c);
  ਜੋੜੋ_virtual_ਵਿੱਥ(coverage, c);
  ਜੋੜੋ_ਵਿੱਥ(liga[0], c);
  liga_ਸੈੱਟ[0] = c->ਕੱਢੋ_pack(ਝੂਠ);

  ਜੋੜੋ_liga_ਸਿਰਲੇਖ(coverage, 2, c);
  ਜੋੜੋ_ਵਿੱਥ(liga_ਸੈੱਟ[0], c);
  ਜੋੜੋ_ਵਿੱਥ(liga_ਸੈੱਟ[1], c);
  liga_subst[1] = c->ਕੱਢੋ_pack(ਝੂਠ);

  // LigaSubst 1
  coverage = ਜੋੜੋ_coverage(0, 0, c);
  ਲਈ (ਪੂਰਨ i = 1; i >= 0; i--) {
    ਸ਼ੁਰੂ_object (large_string.c_str(), liga_ਮਾਪ, c);
    ਜੋੜੋ_virtual_ਵਿੱਥ(coverage, c);
    liga[i] = c->ਕੱਢੋ_pack (ਝੂਠ);
  }

  ਜੋੜੋ_liga_ਸੈੱਟ_ਸਿਰਲੇਖ(2, c);
  ਜੋੜੋ_virtual_ਵਿੱਥ(coverage, c);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 2; i++)
    ਜੋੜੋ_ਵਿੱਥ(liga[i], c);
  liga_ਸੈੱਟ[0] = c->ਕੱਢੋ_pack(ਝੂਠ);

  ਜੋੜੋ_liga_ਸਿਰਲੇਖ(coverage, 1, c);
  ਜੋੜੋ_ਵਿੱਥ(liga_ਸੈੱਟ[0], c);
  liga_subst[0] = c->ਕੱਢੋ_pack(ਝੂਠ);

  ਲਈ (ਪੂਰਨ l = 2; l >= 0; l--) {
    liga_subst[l] = ਜੋੜੋ_extension(liga_subst[l], 4, c);
  }

  ਸ਼ੁਰੂ_lookup (7, 3, c);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ l = 0; l < 3; l++) {
    ਜੋੜੋ_ਵਿੱਥ(liga_subst[l], c);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup = ਅੰਤ_lookup (c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list = ਜੋੜੋ_lookup_list (&lookup, 1, c);

  ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (lookup_list, c);

  c->ਸਿਰਾ_serialize();
}

ਸਾਂਚਾ<ਬਿਨਾਨਿਸ਼ਾਨੀ liga_subst_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਮਾਪ>
ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_shared_large_liga (hb_serialize_context_t* c, ਤਾਰਕਿਕ sequential_liga_sets)
{
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookups[2];
  // Lookup
  // LigSubst: shared with another Lookup table, needs splitting
  ਬਿਨਾਨਿਸ਼ਾਨੀ liga_subst_idx[liga_subst_ਗਿਣਤੀ];
  populate_serializer_with_large_ligsubst<liga_subst_ਗਿਣਤੀ, liga_ਸੈੱਟ_ਗਿਣਤੀ, liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ, liga_ਮਾਪ>(c, sequential_liga_sets, ਸੱਚ, liga_subst_idx, ਸੱਚ);

  ਸ਼ੁਰੂ_lookup (7, liga_subst_ਗਿਣਤੀ, c);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ l = 0; l < liga_subst_ਗਿਣਤੀ; l++) {
    ਜੋੜੋ_ਵਿੱਥ(liga_subst_idx[l], c);
  }

  lookups[0] = ਅੰਤ_lookup (c);

  // Lookup with 2 LigSubst tables
  ਬਿਨਾਨਿਸ਼ਾਨੀ liga_subst[liga_subst_ਗਿਣਤੀ + 1];
  // LigSubst: small one, not shared, no split
  populate_serializer_with_large_ligsubst<1, 1, 1, 10>(c, sequential_liga_sets, ਝੂਠ, liga_subst);

  // LigSubst: shared, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_ਗਿਣਤੀ, liga_ਸੈੱਟ_ਗਿਣਤੀ, liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ, liga_ਮਾਪ>(c, sequential_liga_sets, ਸੱਚ, &liga_subst[1], ਸੱਚ);

  ਸ਼ੁਰੂ_lookup (7, liga_subst_ਗਿਣਤੀ + 1, c);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ l = 0; l <= liga_subst_ਗਿਣਤੀ; l++) {
    ਜੋੜੋ_ਵਿੱਥ(liga_subst[l], c);
  }

  lookups[1] = ਅੰਤ_lookup (c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list = ਜੋੜੋ_lookup_list (lookups, 2, c);
  ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (lookup_list, c);

  c->ਸਿਰਾ_serialize();
}

ਸਾਂਚਾ<ਬਿਨਾਨਿਸ਼ਾਨੀ liga_subst_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਸੈੱਟ_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ,
         ਬਿਨਾਨਿਸ਼ਾਨੀ liga_ਮਾਪ>
ਸਥਿਰ ਖਾਲੀ
populate_serializer_with_liga_shared_coverage (hb_serialize_context_t* c)
{
  c->ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ liga_subst[liga_subst_ਗਿਣਤੀ + 1];
  // LigSubst: small one, no split, coverage shared
  populate_serializer_with_large_ligsubst<1, 6, 2, 10>(c, ਸੱਚ, ਸੱਚ, liga_subst, ਸੱਚ);

  // LigSubst: shared coverage, needs splitting
  populate_serializer_with_large_ligsubst<liga_subst_ਗਿਣਤੀ, liga_ਸੈੱਟ_ਗਿਣਤੀ, liga_ਪ੍ਰਤੀ_ਸੈੱਟ_ਗਿਣਤੀ, liga_ਮਾਪ>(c, ਸੱਚ, ਸੱਚ, &liga_subst[1], ਸੱਚ);

  ਸ਼ੁਰੂ_lookup (7, liga_subst_ਗਿਣਤੀ + 1, c);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ l = 0; l <= liga_subst_ਗਿਣਤੀ; l++) {
    ਜੋੜੋ_ਵਿੱਥ(liga_subst[l], c);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup = ਅੰਤ_lookup (c);

  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list = ਜੋੜੋ_lookup_list (&lookup, 1, c);
  ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (lookup_list, c);

  c->ਸਿਰਾ_serialize();
}

ਸਥਿਰ ਖਾਲੀ test_ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ ()
{
  size_t ਬਫਰ_ਮਾਪ = 100;
  ਖਾਲੀ* ਬਫਰ_a = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  ਖਾਲੀ* ਬਫਰ_e = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t a (ਬਫਰ_a, ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t e (ਬਫਰ_e, ਬਫਰ_ਮਾਪ);
  populate_serializer_complex_2 (&a);

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (a.object_ਗ੍ਰਾਫ ()).value ();
  hb_always_assert (ਗ੍ਰਾਫ.ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ ().is_ok());
  ਗ੍ਰਾਫ.normalize();

  // Expected graph
  e.ਸ਼ੁਰੂ_serialize();
  ਬਿਨਾਨਿਸ਼ਾਨੀ jkl = ਜੋੜੋ_object ("jkl", 3, &e);

  ਸ਼ੁਰੂ_object("ghi", 3, &e);
  ਜੋੜੋ_ਵਿੱਥ(jkl, &e);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ghi = e.ਕੱਢੋ_pack(ਝੂਠ);

  ਸ਼ੁਰੂ_object("def", 3, &e);
  ਜੋੜੋ_ਵਿੱਥ(ghi, &e);
  ਬਿਨਾਨਿਸ਼ਾਨੀ def = e.ਕੱਢੋ_pack(ਝੂਠ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ mn = ਜੋੜੋ_object("mn", 2, &e);

  ਸ਼ੁਰੂ_object ("abc", 3, &e);
  ਜੋੜੋ_ਵਿੱਥ (def, &e);
  ਜੋੜੋ_ਵਿੱਥ (jkl, &e);
  ਜੋੜੋ_ਵਿੱਥ (mn, &e);
  e.ਕੱਢੋ_pack (ਝੂਠ);
  e.ਸਿਰਾ_serialize();

  ਗ੍ਰਾਫ_t expected = ਗ੍ਰਾਫ_t::create (e.object_ਗ੍ਰਾਫ ()).value ();
  expected.normalize();

  ਪ੍ਰਤਿਜ੍ਞਾ(expected == ਗ੍ਰਾਫ);

  ਮੁਕਤ (ਬਫਰ_a);
  ਮੁਕਤ (ਬਫਰ_e);
}

ਸਥਿਰ ਖਾਲੀ test_duplicate_ਪੱਤੀ ()
{
  size_t ਬਫਰ_ਮਾਪ = 100;
  ਖਾਲੀ* ਬਫਰ_a = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  ਖਾਲੀ* ਬਫਰ_e = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t a (ਬਫਰ_a, ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t e (ਬਫਰ_e, ਬਫਰ_ਮਾਪ);
  populate_serializer_complex_2 (&a);

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (a.object_ਗ੍ਰਾਫ ()).value ();
  hb_always_assert (ਗ੍ਰਾਫ.duplicate (4, 1, ਝੂਠ).is_ok ());
  ਗ੍ਰਾਫ.normalize();

  e.ਸ਼ੁਰੂ_serialize();
  ਬਿਨਾਨਿਸ਼ਾਨੀ mn = ਜੋੜੋ_object("mn", 2, &e);
  ਬਿਨਾਨਿਸ਼ਾਨੀ jkl_2 = ਜੋੜੋ_object("jkl", 3, &e);

  ਸ਼ੁਰੂ_object("ghi", 3, &e);
  ਜੋੜੋ_ਵਿੱਥ(jkl_2, &e);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ghi = e.ਕੱਢੋ_pack(ਝੂਠ);

  ਸ਼ੁਰੂ_object("def", 3, &e);
  ਜੋੜੋ_ਵਿੱਥ(ghi, &e);
  ਬਿਨਾਨਿਸ਼ਾਨੀ def = e.ਕੱਢੋ_pack(ਝੂਠ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ jkl_1 = ਜੋੜੋ_object("jkl", 3, &e);

  ਸ਼ੁਰੂ_object("abc", 3, &e);
  ਜੋੜੋ_ਵਿੱਥ(def, &e);
  ਜੋੜੋ_ਵਿੱਥ(jkl_1, &e);
  ਜੋੜੋ_ਵਿੱਥ(mn, &e);
  e.ਕੱਢੋ_pack(ਝੂਠ);

  ਗ੍ਰਾਫ_t expected = ਗ੍ਰਾਫ_t::create (e.object_ਗ੍ਰਾਫ ()).value ();
  expected.normalize();

  ਪ੍ਰਤਿਜ੍ਞਾ(expected == ਗ੍ਰਾਫ);

  ਮੁਕਤ (ਬਫਰ_a);
  ਮੁਕਤ (ਬਫਰ_e);
}

ਸਥਿਰ ਖਾਲੀ test_duplicate_interior ()
{
  size_t ਬਫਰ_ਮਾਪ = 100;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_complex_3 (&c);

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();
  hb_always_assert (ਗ੍ਰਾਫ.duplicate (3, 2, ਝੂਠ).is_ok ());

  hb_always_assert(ਲੜੀ_ਤੁਲਨਾ (ਗ੍ਰਾਫ.object (6).ਸਿਰ_2, "jkl", 3) == 0);
  hb_always_assert(ਗ੍ਰਾਫ.object (6).real_links.ਲੰਮਾਈ == 1);
  hb_always_assert(ਗ੍ਰਾਫ.object (6).real_links[0].objidx == 0);

  hb_always_assert(ਲੜੀ_ਤੁਲਨਾ (ਗ੍ਰਾਫ.object (5).ਸਿਰ_2, "abc", 3) == 0);
  hb_always_assert(ਗ੍ਰਾਫ.object (5).real_links.ਲੰਮਾਈ == 3);
  hb_always_assert(ਗ੍ਰਾਫ.object (5).real_links[0].objidx == 4);
  hb_always_assert(ਗ੍ਰਾਫ.object (5).real_links[1].objidx == 2);
  hb_always_assert(ਗ੍ਰਾਫ.object (5).real_links[2].objidx == 1);

  hb_always_assert(ਲੜੀ_ਤੁਲਨਾ (ਗ੍ਰਾਫ.object (4).ਸਿਰ_2, "def", 3) == 0);
  hb_always_assert(ਗ੍ਰਾਫ.object (4).real_links.ਲੰਮਾਈ == 1);
  hb_always_assert(ਗ੍ਰਾਫ.object (4).real_links[0].objidx == 3);

  hb_always_assert(ਲੜੀ_ਤੁਲਨਾ (ਗ੍ਰਾਫ.object (3).ਸਿਰ_2, "ghi", 3) == 0);
  hb_always_assert(ਗ੍ਰਾਫ.object (3).real_links.ਲੰਮਾਈ == 1);
  hb_always_assert(ਗ੍ਰਾਫ.object (3).real_links[0].objidx == 6);

  hb_always_assert(ਲੜੀ_ਤੁਲਨਾ (ਗ੍ਰਾਫ.object (2).ਸਿਰ_2, "jkl", 3) == 0);
  hb_always_assert(ਗ੍ਰਾਫ.object (2).real_links.ਲੰਮਾਈ == 1);
  hb_always_assert(ਗ੍ਰਾਫ.object (2).real_links[0].objidx == 0);

  hb_always_assert(ਲੜੀ_ਤੁਲਨਾ (ਗ੍ਰਾਫ.object (1).ਸਿਰ_2, "mn", 2) == 0);
  hb_always_assert(ਗ੍ਰਾਫ.object (1).real_links.ਲੰਮਾਈ == 0);

  hb_always_assert(ਲੜੀ_ਤੁਲਨਾ (ਗ੍ਰਾਫ.object (0).ਸਿਰ_2, "opqrst", 6) == 0);
  hb_always_assert(ਗ੍ਰਾਫ.object (0).real_links.ਲੰਮਾਈ == 0);

  ਮੁਕਤ (ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ
test_serialize ()
{
  size_t ਬਫਰ_ਮਾਪ = 100;
  ਖਾਲੀ* ਬਫਰ_1 = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c1 (ਬਫਰ_1, ਬਫਰ_ਮਾਪ);
  populate_serializer_simple (&c1);
  hb_ਬਾਈਟਾਂ_t expected = c1.copy_ਬਾਈਟਾਂ ();

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c1.object_ਗ੍ਰਾਫ ()).value ();
  hb_blob_t* ਬਾਹਰਲਾ = ਗ੍ਰਾਫ::serialize (ਗ੍ਰਾਫ).value ();
  ਮੁਕਤ (ਬਫਰ_1);

  hb_ਬਾਈਟਾਂ_t actual = ਬਾਹਰਲਾ->as_ਬਾਈਟਾਂ ();
  hb_always_assert (actual == expected);
  expected.fini ();
  hb_blob_destroy (ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਖਾਲੀ test_will_ਉਛਾਲ_1 ()
{
  size_t ਬਫਰ_ਮਾਪ = 100;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);

  populate_serializer_complex_2 (&c);
  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_always_assert (!ਗ੍ਰਾਫ::will_ਉਛਾਲ (ਗ੍ਰਾਫ, ਖਾਲੀ_ਸੂਚਕ).value ());

  ਮੁਕਤ (ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_will_ਉਛਾਲ_2 ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_ਉਛਾਲ (&c);
  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_always_assert (ਗ੍ਰਾਫ::will_ਉਛਾਲ (ਗ੍ਰਾਫ, ਖਾਲੀ_ਸੂਚਕ).value ());

  ਮੁਕਤ (ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_will_ਉਛਾਲ_3 ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_dedup_ਉਛਾਲ (&c);
  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_always_assert (ਗ੍ਰਾਫ::will_ਉਛਾਲ (ਗ੍ਰਾਫ, ਖਾਲੀ_ਸੂਚਕ).value ());

  ਮੁਕਤ (ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_ਛਾਂਟੋ ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_ਉਛਾਲ (&c);
  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_blob_t* ਬਾਹਰਲਾ = hb_resolve_overflows (c.object_ਗ੍ਰਾਫ (), HB_ਟੈਗ_NONE);
  hb_always_assert (ਬਾਹਰਲਾ);
  hb_ਬਾਈਟਾਂ_t ਨਤੀਜਾ = ਬਾਹਰਲਾ->as_ਬਾਈਟਾਂ ();
  hb_always_assert (ਨਤੀਜਾ.ਲੰਮਾਈ == (80000 + 3 + 3 * 2));

  ਮੁਕਤ (ਬਫਰ);
  hb_blob_destroy (ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_duplication ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_dedup_ਉਛਾਲ (&c);
  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_blob_t* ਬਾਹਰਲਾ = hb_resolve_overflows (c.object_ਗ੍ਰਾਫ (), HB_ਟੈਗ_NONE);
  hb_always_assert (ਬਾਹਰਲਾ);
  hb_ਬਾਈਟਾਂ_t ਨਤੀਜਾ = ਬਾਹਰਲਾ->as_ਬਾਈਟਾਂ ();
  hb_always_assert (ਨਤੀਜਾ.ਲੰਮਾਈ == (10000 + 2 * 2 + 60000 + 2 + 3 * 2));

  ਮੁਕਤ (ਬਫਰ);
  hb_blob_destroy (ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_ਗੁਣਜ_duplications ()
{
  size_t ਬਫਰ_ਮਾਪ = 300000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_ਗੁਣਜ_dedup_ਉਛਾਲ (&c);
  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_blob_t* ਬਾਹਰਲਾ = hb_resolve_overflows (c.object_ਗ੍ਰਾਫ (), HB_ਟੈਗ_NONE, 5);
  hb_always_assert (ਬਾਹਰਲਾ);

  ਮੁਕਤ (ਬਫਰ);
  hb_blob_destroy (ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_space_assignment ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_spaces (&c, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_spaces (&e, ਝੂਠ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_overflows_via_space_assignment",
                             c,
                             e);

  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_isolation ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_isolation_ਉਛਾਲ (&c);
  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_always_assert (c.ਵਿੱਥ_ਉਛਾਲ ());
  hb_blob_t* ਬਾਹਰਲਾ = hb_resolve_overflows (c.object_ਗ੍ਰਾਫ (), HB_ਟੈਗ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (ਬਾਹਰਲਾ);
  hb_ਬਾਈਟਾਂ_t ਨਤੀਜਾ = ਬਾਹਰਲਾ->as_ਬਾਈਟਾਂ ();
  hb_always_assert (ਨਤੀਜਾ.ਲੰਮਾਈ == (1 + 10000 + 60000 + 1 + 1
                            + 4 + 3 * 2));

  ਮੁਕਤ (ਬਫਰ);
  hb_blob_destroy (ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_isolation_with_recursive_duplication ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_isolation_ਉਛਾਲ_complex (&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_isolation_ਉਛਾਲ_complex_expected (&e);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_overflows_via_isolation_with_recursive_duplication",
                             c,
                             e);
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_isolating_16bit_space ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_spaces_16bit_connection (&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_spaces_16bit_connection_expected (&e);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_overflows_via_isolating_16bit_space",
                             c,
                             e);

  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_isolating_16bit_space_2 ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_short_and_wide_subgraph_ਜੜ੍ਹ (&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_short_and_wide_subgraph_ਜੜ੍ਹ_expected (&e);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_overflows_via_isolating_16bit_space_2",
                             c,
                             e);

  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_isolation_spaces ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_isolation_ਉਛਾਲ_spaces (&c);
  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_always_assert (c.ਵਿੱਥ_ਉਛਾਲ ());
  hb_blob_t* ਬਾਹਰਲਾ = hb_resolve_overflows (c.object_ਗ੍ਰਾਫ (), HB_ਟੈਗ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (ਬਾਹਰਲਾ);
  hb_ਬਾਈਟਾਂ_t ਨਤੀਜਾ = ਬਾਹਰਲਾ->as_ਬਾਈਟਾਂ ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ expected_ਲੰਮਾਈ = 3 + 2 * 60000; // objects
  expected_ਲੰਮਾਈ += 2 * 4 + 2 * 2; // links
  hb_always_assert (ਨਤੀਜਾ.ਲੰਮਾਈ == expected_ਲੰਮਾਈ);

  ਮੁਕਤ (ਬਫਰ);
  hb_blob_destroy (ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_mixed_overflows_ਰਾਹੀਂ_isolation_spaces ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_24_and_32_ਦੋਆਧਾਰੀ_ਅੰਕ_offsets (&c);
  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_always_assert (c.ਵਿੱਥ_ਉਛਾਲ ());
  hb_blob_t* ਬਾਹਰਲਾ = hb_resolve_overflows (c.object_ਗ੍ਰਾਫ (), HB_ਟੈਗ ('G', 'S', 'U', 'B'), 0);
  hb_always_assert (ਬਾਹਰਲਾ);
  hb_ਬਾਈਟਾਂ_t ਨਤੀਜਾ = ਬਾਹਰਲਾ->as_ਬਾਈਟਾਂ ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ expected_ਲੰਮਾਈ =
      // Objects
      7 +
      4 * 40000;

  expected_ਲੰਮਾਈ +=
      // Links
      2 * 4 +  // 32
      4 * 3 +  // 24
      4 * 2;   // 16

  hb_always_assert (ਨਤੀਜਾ.ਲੰਮਾਈ == expected_ਲੰਮਾਈ);

  ਮੁਕਤ (ਬਫਰ);
  hb_blob_destroy (ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_extension_promotion ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_extension_promotion (&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_extension_promotion (&e, 3);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             ਸੱਚ);
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_shared_extension_promotion ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_extension_promotion (&c, 0, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_extension_promotion (&e, 3, ਸੱਚ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_extension_promotion",
                             c,
                             e,
                             20,
                             ਸੱਚ);
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
ਸਥਿਰ ਖਾਲੀ test_resolve_with_extension_promotion_1_2 ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_extension_promotion (&c, 0, ਝੂਠ, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_extension_promotion (&e, 3, ਝੂਠ, ਸੱਚ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_extension_promotion_1_2",
                             c,
                             e,
                             20,
                             ਸੱਚ);
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ test_resolve_with_repeated_extension_promotion ()
{
  size_t ਬਫਰ_ਮਾਪ = 1000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);

  c.ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ subtable = ਜੋੜੋ_object ("subt", 4, &c);
  ਸ਼ੁਰੂ_lookup (5, 2, &c);
  ਜੋੜੋ_ਵਿੱਥ (subtable, &c);
  ਜੋੜੋ_ਵਿੱਥ (subtable, &c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup = ਅੰਤ_lookup (&c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_list = ਜੋੜੋ_lookup_list (&lookup, 1, &c);
  ਜੋੜੋ_gsubgpos_ਸਿਰਲੇਖ (lookup_list, &c);
  c.ਸਿਰਾ_serialize ();

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();
  ਸਵੈਚਾਲਿਤ context = ਗ੍ਰਾਫ::gsubgpos_ਗ੍ਰਾਫ_context_t::create (HB_ਟੈਗ ('G', 'S', 'U', 'B'),
                                                          ਗ੍ਰਾਫ).value ();
  hb_always_assert (context.lookups.get_population () == 1);
  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ = *context.lookups.ਕੁੰਜੀਆਂ ();
  ਗ੍ਰਾਫ::Lookup* lookup_ਸਾਰਣੀ_2 = context.lookups.get (lookup_ਸੂਚਕਾਂਕ);
  hb_always_assert (lookup_ਸਾਰਣੀ_2->make_extension (context, lookup_ਸੂਚਕਾਂਕ).is_ok ());

  ਅਚਲ ਸਵੈਚਾਲਿਤ& lookup_ਸ਼ੀਰਸ਼ = ਗ੍ਰਾਫ.vertices_[lookup_ਸੂਚਕਾਂਕ];
  hb_always_assert (lookup_ਸ਼ੀਰਸ਼.obj ().real_links.ਲੰਮਾਈ == 2);
  ਬਿਨਾਨਿਸ਼ਾਨੀ extension_ਸੂਚਕਾਂਕ = lookup_ਸ਼ੀਰਸ਼.obj ().real_links[0].objidx;
  hb_always_assert (lookup_ਸ਼ੀਰਸ਼.obj ().real_links[1].objidx == extension_ਸੂਚਕਾਂਕ);

  ਸਵੈਚਾਲਿਤ* extension = ਗ੍ਰਾਫ.vertices_[extension_ਸੂਚਕਾਂਕ].as<
      ਗ੍ਰਾਫ::ExtensionFormat1<OT::Layout::GSUB_impl::ExtensionSubst>*> ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ promoted_subtable = extension->get_subtable_ਸੂਚਕਾਂਕ (ਗ੍ਰਾਫ, extension_ਸੂਚਕਾਂਕ).value ();
  hb_always_assert (ਗ੍ਰਾਫ.vertices_[promoted_subtable].ਸਾਰਣੀ_ਮਾਪ () == 4);
  hb_always_assert (ਗ੍ਰਾਫ.update_parents ().is_ok ());
  hb_always_assert (ਗ੍ਰਾਫ.vertices_[extension_ਸੂਚਕਾਂਕ].incoming_ਕਿਨਾਰੇ_2 () == 2);
  hb_always_assert (ਗ੍ਰਾਫ.vertices_[promoted_subtable].incoming_ਕਿਨਾਰੇ_2 () == 1);

  hb_blob_t* ਬਾਹਰਲਾ = ਗ੍ਰਾਫ::serialize (ਗ੍ਰਾਫ).value ();
  hb_blob_destroy (ਬਾਹਰਲਾ);
  ਮੁਕਤ (ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_basic_pair_pos_1_ਵੰਡੋ ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, ਸੱਚ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_basic_pair_pos_1_split",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'P', 'O', 'S'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_extension_pair_pos_1_ਵੰਡੋ ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_1 <1, 4>(&c, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_1 <2, 2>(&e, ਸੱਚ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_extension_pair_pos_1_split",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'P', 'O', 'S'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_basic_pair_pos_2_ਵੰਡੋ ()
{
  size_t ਬਫਰ_ਮਾਪ = 300000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_2 <1, 4, 3000>(&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_2 <2, 2, 3000>(&e, ਸੱਚ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_basic_pair_pos_2_split",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'P', 'O', 'S'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_close_ਮੰਜ਼ਿਲ_ਹੱਦਬੰਦੀ_pair_pos_2_ਵੰਡੋ ()
{
  size_t ਬਫਰ_ਮਾਪ = 300000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_2 <1, 1636, 10>(&c, ਸੱਚ, ਝੂਠ, ਝੂਠ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_2 <2, 818, 10>(&e, ਸੱਚ, ਝੂਠ, ਝੂਠ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_close_to_limit_pair_pos_2_split",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'P', 'O', 'S'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_pair_pos_2_ਵੰਡੋ_with_device_tables ()
{
  size_t ਬਫਰ_ਮਾਪ = 300000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_2 <1, 4, 2000>(&c, ਝੂਠ, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_pair_pos_2 <2, 2, 2000>(&e, ਸੱਚ, ਸੱਚ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_pair_pos_2_split_with_device_tables",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'P', 'O', 'S'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_basic_mark_ਅਧਾਰ_pos_1_ਵੰਡੋ ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_mark_ਅਧਾਰ_pos_1 <40, 10, 110, 1>(&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_mark_ਅਧਾਰ_pos_1 <40, 10, 110, 2>(&e);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_basic_mark_base_pos_1_split",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'P', 'O', 'S'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_basic_liga_ਵੰਡੋ ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_liga<1, 1, 2, 40000>(&c, ਝੂਠ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_liga<2, 1, 1, 40000>(&e, ਝੂਠ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_basic_liga_split",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'S', 'U', 'B'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_liga_ਵੰਡੋ_move ()
{
  size_t ਬਫਰ_ਮਾਪ = 400000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_liga<1, 6, 2, 16000>(&c, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_liga<3, 2, 2, 16000>(&e, ਸੱਚ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_liga_split_move",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'S', 'U', 'B'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_liga_ਵੰਡੋ_overlapping_clone ()
{
  size_t ਬਫਰ_ਮਾਪ = 400000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_liga<1, 2, 3, 30000>(&c, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_large_liga_overlapping_clone_ਨਤੀਜਾ(&e);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_liga_split_overlapping_clone",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'S', 'U', 'B'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_liga_ਵੰਡੋ_shared_ਸਾਰਣੀ ()
{
  size_t ਬਫਰ_ਮਾਪ = 400000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_shared_large_liga<1, 6, 2, 16000>(&c, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_shared_large_liga<3, 2, 2, 16000>(&e, ਸੱਚ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_liga_split_shared_table",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'S', 'U', 'B'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_with_liga_ਵੰਡੋ_shared_coverage ()
{
  size_t ਬਫਰ_ਮਾਪ = 400000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_liga_shared_coverage<1, 6, 2, 16000>(&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_liga_shared_coverage<3, 2, 2, 16000>(&e);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_with_liga_split_shared_coverage",
                             c,
                             e,
                             20,
                             ਸੱਚ,
                             HB_ਟੈਗ('G', 'S', 'U', 'B'));
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_splitting_spaces ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_ਵੰਡੋ_spaces (&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_ਵੰਡੋ_spaces_expected (&e);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_overflows_via_splitting_spaces",
                             c,
                             e,
                             1);

  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);

}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_splitting_spaces_2 ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_ਵੰਡੋ_spaces_2 (&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_ਵੰਡੋ_spaces_expected_2 (&e);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_overflows_via_splitting_spaces_2",
                             c,
                             e,
                             1);
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_resolve_overflows_ਰਾਹੀਂ_ਤਰਜੀਹ ()
{
  size_t ਬਫਰ_ਮਾਪ = 160000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_ਤਰਜੀਹ_ਉਛਾਲ (&c);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_ਤਰਜੀਹ_ਉਛਾਲ_expected (&e);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_resolve_overflows_via_priority",
                             c,
                             e,
                             3);
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}


ਸਥਿਰ ਖਾਲੀ test_virtual_link ()
{
  size_t ਬਫਰ_ਮਾਪ = 100;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_virtual_link (&c);

  hb_blob_t* ਬਾਹਰਲਾ = hb_resolve_overflows (c.object_ਗ੍ਰਾਫ (), HB_ਟੈਗ_NONE);
  hb_always_assert (ਬਾਹਰਲਾ);

  hb_ਬਾਈਟਾਂ_t ਨਤੀਜਾ = ਬਾਹਰਲਾ->as_ਬਾਈਟਾਂ ();
  hb_always_assert (ਨਤੀਜਾ.ਲੰਮਾਈ == 5 + 4 * 2);
  hb_always_assert (ਨਤੀਜਾ[0]  == 'a');
  hb_always_assert (ਨਤੀਜਾ[5]  == 'c');
  hb_always_assert (ਨਤੀਜਾ[8]  == 'e');
  hb_always_assert (ਨਤੀਜਾ[9]  == 'b');
  hb_always_assert (ਨਤੀਜਾ[12] == 'd');

  ਮੁਕਤ (ਬਫਰ);
  hb_blob_destroy (ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਖਾਲੀ
test_shared_ਗੰਢ_with_virtual_links ()
{
  size_t ਬਫਰ_ਮਾਪ = 100;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);

  c.ਸ਼ੁਰੂ_serialize<ਅੱਖਰ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_b = ਜੋੜੋ_object ("b", 1, &c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_c = ਜੋੜੋ_object ("c", 1, &c);

  ਸ਼ੁਰੂ_object ("d", 1, &c);
  ਜੋੜੋ_virtual_ਵਿੱਥ (obj_b, &c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d_1 = c.ਕੱਢੋ_pack ();

  ਸ਼ੁਰੂ_object ("d", 1, &c);
  ਜੋੜੋ_virtual_ਵਿੱਥ (obj_c, &c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ obj_d_2 = c.ਕੱਢੋ_pack ();

  hb_always_assert (obj_d_1 == obj_d_2);

  ਸ਼ੁਰੂ_object ("a", 1, &c);
  ਜੋੜੋ_ਵਿੱਥ (obj_b, &c);
  ਜੋੜੋ_ਵਿੱਥ (obj_c, &c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d_1, &c);
  ਜੋੜੋ_ਵਿੱਥ (obj_d_2, &c);
  c.ਕੱਢੋ_pack ();
  c.ਸਿਰਾ_serialize ();

  hb_always_assert(c.object_ਗ੍ਰਾਫ() [obj_d_1]->virtual_links.ਲੰਮਾਈ == 2);
  hb_always_assert(c.object_ਗ੍ਰਾਫ() [obj_d_1]->virtual_links[0].objidx == obj_b);
  hb_always_assert(c.object_ਗ੍ਰਾਫ() [obj_d_1]->virtual_links[1].objidx == obj_c);
  ਮੁਕਤ(ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ
test_repack_ਆਖਰੀ ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_repack_ਆਖਰੀ (&c, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_with_repack_ਆਖਰੀ (&e, ਝੂਠ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_repack_last",
                             c,
                             e,
                             20,
                             ਝੂਠ,
                             HB_ਟੈਗ('a', 'b', 'c', 'd'),
                             ਸੱਚ);

  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ
test_dont_duplicate_virtual ()
{
  size_t ਬਫਰ_ਮਾਪ = 200000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (ਬਫਰ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_virtual (&c, ਸੱਚ);

  ਖਾਲੀ* expected_ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_always_assert (expected_ਬਫਰ);
  hb_serialize_context_t e (expected_ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_virtual (&e, ਝੂਠ);

  ਲੜੀਵਾਰ_resolve_ਉਛਾਲ_test ("test_dont_duplicate_virtual",
                             c,
                             e,
                             20,
                             ਝੂਠ,
                             HB_ਟੈਗ('a', 'b', 'c', 'd'),
                             ਸੱਚ);

  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (expected_ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ
test_deep_ਗ੍ਰਾਫ_traversal ()
{
  size_t ਬਫਰ_ਮਾਪ = 1000000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);

  c.ਸ਼ੁਰੂ_serialize ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ prev = ਜੋੜੋ_object ("leaf", 4, &c);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 2000; i++)
  {
    ਸ਼ੁਰੂ_object ("node", 4, &c);
    ਜੋੜੋ_ਵਿੱਥ (prev, &c);
    prev = c.ਕੱਢੋ_pack (ਝੂਠ);
  }
  c.ਸਿਰਾ_serialize ();

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  // Test find_subgraph (set)
  hb_ਸੈੱਟ_t ਦੇਖਿਆ_ਸੈੱਟ;
  hb_always_assert (ਗ੍ਰਾਫ.ਲੱਭੋ_subgraph (ਗ੍ਰਾਫ.ਜੜ੍ਹ_idx (), ਦੇਖਿਆ_ਸੈੱਟ).is_ok ());
  hb_always_assert (ਦੇਖਿਆ_ਸੈੱਟ.get_population () == 2001);

  // Test find_subgraph_size
  hb_ਸੈੱਟ_t ਮਾਪ_ਸੈੱਟ;
  size_t sz = ਗ੍ਰਾਫ.ਲੱਭੋ_subgraph_ਮਾਪ (ਗ੍ਰਾਫ.ਜੜ੍ਹ_idx (), ਮਾਪ_ਸੈੱਟ).value ();
  hb_always_assert (sz == 4 + 2000 * 6);

  // Test find_subgraph (map)
  hb_map_t map;
  map.ਸੈੱਟ (ਗ੍ਰਾਫ.ਜੜ੍ਹ_idx (), 1);
  hb_always_assert (ਗ੍ਰਾਫ.ਲੱਭੋ_subgraph (ਗ੍ਰਾਫ.ਜੜ੍ਹ_idx (), map).is_ok ());
  hb_always_assert (map.get_population () == 2001);

  // Test assign_spaces (which exercises find_connected_nodes and find_space_roots)
  hb_always_assert (ਗ੍ਰਾਫ.ਸੌਂਪੋ_spaces ().is_ok ());

  // Test duplicate_subgraph
  hb_map_t ਸੂਚਕਾਂਕ_map;
  hb_always_assert (ਗ੍ਰਾਫ.duplicate_subgraph (ਗ੍ਰਾਫ.ਜੜ੍ਹ_idx (), ਸੂਚਕਾਂਕ_map).is_ok ());
  hb_always_assert (ਸੂਚਕਾਂਕ_map.get_population () == 2001);

  ਮੁਕਤ (ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ
test_32bit_roots_traversal ()
{
  size_t ਬਫਰ_ਮਾਪ = 1000;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);

  c.ਸ਼ੁਰੂ_serialize ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ leaf1 = ਜੋੜੋ_object ("l1", 2, &c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ leaf2 = ਜੋੜੋ_object ("l2", 2, &c);

  ਸ਼ੁਰੂ_object ("mid", 3, &c);
  ਜੋੜੋ_wide_ਵਿੱਥ (leaf1, &c);
  ਬਿਨਾਨਿਸ਼ਾਨੀ mid = c.ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("root", 4, &c);
  ਜੋੜੋ_ਵਿੱਥ (mid, &c);
  ਜੋੜੋ_wide_ਵਿੱਥ (leaf2, &c);
  c.ਕੱਢੋ_pack (ਝੂਠ);
  c.ਸਿਰਾ_serialize ();

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  hb_ਸੈੱਟ_t roots;
  hb_always_assert (ਗ੍ਰਾਫ.ਲੱਭੋ_32bit_roots (ਗ੍ਰਾਫ.ਜੜ੍ਹ_idx (), roots).is_ok ());
  hb_always_assert (roots.has (leaf1 - 1));
  hb_always_assert (roots.has (leaf2 - 1));
  hb_always_assert (!roots.has (mid - 1));
  hb_always_assert (roots.get_population () == 2);

  ਮੁਕਤ (ਬਫਰ);

  // Test case where DFS fails but BFS succeeds:
  // root -16-> a
  // root -32-> a
  // a -32-> b
  //
  // DFS will first explore the 16 bit link to a and then recurse the 32 bit link from a to b
  // and consider b a root. However, b should not be a root since it's not a top level 32 bit link.
  // BFS will handle this correctly by discovering a as a top level root and then avoiding further
  // exploring it.
  ਬਫਰ_ਮਾਪ = 1000;
  ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c2 (ਬਫਰ, ਬਫਰ_ਮਾਪ);

  c2.ਸ਼ੁਰੂ_serialize ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ b = ਜੋੜੋ_object ("b", 1, &c2);

  ਸ਼ੁਰੂ_object ("a", 1, &c2);
  ਜੋੜੋ_wide_ਵਿੱਥ (b, &c2);
  ਬਿਨਾਨਿਸ਼ਾਨੀ a = c2.ਕੱਢੋ_pack (ਝੂਠ);

  ਸ਼ੁਰੂ_object ("r", 1, &c2);
  ਜੋੜੋ_ਵਿੱਥ (a, &c2);
  ਜੋੜੋ_wide_ਵਿੱਥ (a, &c2);
  c2.ਕੱਢੋ_pack (ਝੂਠ);

  c2.ਸਿਰਾ_serialize ();

  ਗ੍ਰਾਫ_t graph2 = ਗ੍ਰਾਫ_t::create (c2.object_ਗ੍ਰਾਫ ()).value ();

  hb_ਸੈੱਟ_t roots2;
  hb_always_assert (graph2.ਲੱਭੋ_32bit_roots (graph2.ਜੜ੍ਹ_idx (), roots2).is_ok ());
  hb_always_assert (roots2.get_population () == 1);
  hb_always_assert (roots2.has (a - 1));
  hb_always_assert (!roots2.has (b - 1));

  ਮੁਕਤ (ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_invalid_link_objidx ()
{
  size_t ਬਫਰ_ਮਾਪ = 100;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_complex_2 (&c);

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  // Poison a link to point to (unsigned) -1
  ਸਵੈਚਾਲਿਤ links = ਗ੍ਰਾਫ.vertices_[ਗ੍ਰਾਫ.ਜੜ੍ਹ_idx ()].real_links_writer();
  links->objidx = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;

  // Running Dijkstra / shortest distance sorting must not crash or OOB access,
  // and must return an error.
  hb_always_assert (!ਗ੍ਰਾਫ.ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ ().is_ok ());

  ਮੁਕਤ (ਬਫਰ);
}

ਸਥਿਰ ਖਾਲੀ test_invalid_link_ਜੋੜੋ_and_move ()
{
  size_t ਬਫਰ_ਮਾਪ = 100;
  ਖਾਲੀ* ਬਫਰ = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਬਫਰ_ਮਾਪ);
  hb_serialize_context_t c (ਬਫਰ, ਬਫਰ_ਮਾਪ);
  populate_serializer_complex_2 (&c);

  ਗ੍ਰਾਫ_t ਗ੍ਰਾਫ = ਗ੍ਰਾਫ_t::create (c.object_ਗ੍ਰਾਫ ()).value ();

  // add_link with out-of-bounds child
  OT::Offset16 dummy;
  hb_always_assert (!ਗ੍ਰਾਫ.ਜੋੜੋ_link (&dummy, 0, (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1).is_ok ());

  // move_child with non-existent offset
  ਸਵੈਚਾਲਿਤ res = ਗ੍ਰਾਫ.move_ਬੱਚਾ (0, &dummy, 1, &dummy);
  hb_always_assert (!res.is_ok ());

  ਮੁਕਤ (ਬਫਰ);
}

// TODO(garretrieger): update will_overflow tests to check the overflows array.
// TODO(garretrieger): add tests for priority raising.

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  test_invalid_link_objidx ();
  test_invalid_link_ਜੋੜੋ_and_move ();
  test_serialize ();
  test_ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ ();
  test_will_ਉਛਾਲ_1 ();
  test_will_ਉਛਾਲ_2 ();
  test_will_ਉਛਾਲ_3 ();
  test_resolve_overflows_ਰਾਹੀਂ_ਛਾਂਟੋ ();
  test_resolve_overflows_ਰਾਹੀਂ_duplication ();
  test_resolve_overflows_ਰਾਹੀਂ_ਗੁਣਜ_duplications ();
  test_resolve_overflows_ਰਾਹੀਂ_ਤਰਜੀਹ ();
  test_resolve_overflows_ਰਾਹੀਂ_space_assignment ();
  test_resolve_overflows_ਰਾਹੀਂ_isolation ();
  test_resolve_overflows_ਰਾਹੀਂ_isolation_with_recursive_duplication ();
  test_resolve_overflows_ਰਾਹੀਂ_isolation_spaces ();
  test_resolve_overflows_ਰਾਹੀਂ_isolating_16bit_space ();
  test_resolve_overflows_ਰਾਹੀਂ_isolating_16bit_space_2 ();
  test_resolve_overflows_ਰਾਹੀਂ_splitting_spaces ();
  test_resolve_overflows_ਰਾਹੀਂ_splitting_spaces_2 ();
  test_resolve_mixed_overflows_ਰਾਹੀਂ_isolation_spaces ();
  test_duplicate_ਪੱਤੀ ();
  test_duplicate_interior ();
  test_virtual_link ();
  test_repack_ਆਖਰੀ();
  test_shared_ਗੰਢ_with_virtual_links ();
  test_resolve_with_extension_promotion ();
  test_resolve_with_shared_extension_promotion ();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  test_resolve_with_extension_promotion_1_2 ();
#ਜੇ_ਅੰਤ
  test_resolve_with_repeated_extension_promotion ();
  test_resolve_with_basic_pair_pos_1_ਵੰਡੋ ();
  test_resolve_with_extension_pair_pos_1_ਵੰਡੋ ();
  test_resolve_with_basic_pair_pos_2_ਵੰਡੋ ();
  test_resolve_with_pair_pos_2_ਵੰਡੋ_with_device_tables ();
  test_resolve_with_close_ਮੰਜ਼ਿਲ_ਹੱਦਬੰਦੀ_pair_pos_2_ਵੰਡੋ ();
  test_resolve_with_basic_mark_ਅਧਾਰ_pos_1_ਵੰਡੋ ();
  test_resolve_with_basic_liga_ਵੰਡੋ ();
  test_resolve_with_liga_ਵੰਡੋ_move ();
  test_dont_duplicate_virtual ();
  test_resolve_with_liga_ਵੰਡੋ_overlapping_clone ();
  test_resolve_with_liga_ਵੰਡੋ_shared_ਸਾਰਣੀ ();
  test_resolve_with_liga_ਵੰਡੋ_shared_coverage ();
  test_deep_ਗ੍ਰਾਫ_traversal ();
  test_32bit_roots_traversal ();

  // TODO(grieger): have run overflow tests compare graph equality not final packed binary.
  // TODO(grieger): split test where multiple subtables in one lookup are split to test link ordering.
  // TODO(grieger): split test where coverage table in subtable that is being split is shared.
  // TODO(grieger): test with extensions already mixed in as well.
  // TODO(grieger): test two layer ext promotion setup.
  // TODO(grieger): test sorting by subtables per byte in ext. promotion.
}
