/*
 * Copyright © 2022  Google, Inc.
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

#ਅਨ੍ਤਰ੍ਭਾਵ "graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "graph-result.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_CLASSDEF_ਗ੍ਰਾਫ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_CLASSDEF_ਗ੍ਰਾਫ_HH

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ClassDefFormat1_3 : ਜਨਤਕ OT::ClassDefFormat1_3<Types>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ();
    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = OT::ClassDefFormat1_3<Types>::min_ਮਾਪ;
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ + ਇਹ->class_ਮੁੱਲ.get_ਮਾਪ () - ਇਹ->class_ਮੁੱਲ.len.get_ਮਾਪ ()))
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ClassDefFormat2_4 : ਜਨਤਕ OT::ClassDefFormat2_4<Types>
{
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ();
    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = OT::ClassDefFormat2_4<Types>::min_ਮਾਪ;
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ + ਇਹ->range_ਰਿਕਾਰਡ.get_ਮਾਪ () - ਇਹ->range_ਰਿਕਾਰਡ.len.get_ਮਾਪ ()))
      ਵਾਪਸ Err(SANITIZE_FAILURE);
    ਵਾਪਸ Ok();
  }
};

ਬਣਤਰ ClassDef : ਜਨਤਕ OT::ClassDef
{
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ It>
  ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_class_def (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_id,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ link_position,
                                             It glyph_and_class,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ max_size)
  {
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_ਅਭਾਜ_id, c.ਗ੍ਰਾਫ.new_ਗੰਢ (ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ));
    ਸਵੈਚਾਲਿਤ& class_def_ਅਭਾਜ_ਸ਼ੀਰਸ਼ = c.ਗ੍ਰਾਫ.vertices_[class_def_ਅਭਾਜ_id];
    TRY (make_class_def (c, glyph_and_class, class_def_ਅਭਾਜ_id, max_size));

    TRY(c.ਗ੍ਰਾਫ.vertices_[ਮਾਪਾ_id].ਜੋੜੋ_real_link (SmallTypes::ਮਾਪ, class_def_ਅਭਾਜ_id, link_position));
    TRY(class_def_ਅਭਾਜ_ਸ਼ੀਰਸ਼.ਜੋੜੋ_ਮਾਪਾ (ਮਾਪਾ_id, ਝੂਠ));

    ਵਾਪਸ Ok();
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ It>
  ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> make_class_def (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                              It glyph_and_class,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ dest_obj,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ max_size)
  {
    ਅੱਖਰ* ਬਫਰ = (ਅੱਖਰ*) hb_calloc (1, max_size);
    ਜੇ (unlikely (!ਬਫਰ))
      ਵਾਪਸ Err(ਵੰਡ_FAILURE);

    hb_serialize_context_t serializer (ਬਫਰ, max_size);
    OT::ClassDef_serialize (&serializer, glyph_and_class);
    serializer.ਸਿਰਾ_serialize ();
    ਜੇ (unlikely (serializer.ਵਿੱਚ_error ()))
    {
      hb_free (ਬਫਰ);
      ਵਾਪਸ Err(ਵੰਡ_FAILURE);
    }

    hb_ਬਾਈਟਾਂ_t class_def_copy = serializer.copy_ਬਾਈਟਾਂ ();
    ਜੇ (unlikely (!class_def_copy.arrayZ))
    {
      hb_free (ਬਫਰ);
      ਵਾਪਸ Err(ਵੰਡ_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    ਸਵੈਚਾਲਿਤ res = c.ਜੋੜੋ_ਬਫਰ ((ਅੱਖਰ *) class_def_copy.arrayZ);
    ਜੇ (unlikely (!res.is_ok ()))
    {
      hb_free (ਬਫਰ);
      hb_free ((ਅੱਖਰ *) class_def_copy.arrayZ);
      ਵਾਪਸ res;
    }


    ਸਵੈਚਾਲਿਤ& v = c.ਗ੍ਰਾਫ.vertices_[dest_obj];
    ਅੱਖਰ* ਸਿਰ_2 = (ਅੱਖਰ *) class_def_copy.arrayZ;
    v.ਸੈੱਟ_ਬਫਰ (ਸਿਰ_2, ਸਿਰ_2 + class_def_copy.ਲੰਮਾਈ);

    hb_free (ਬਫਰ);
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < OT::ClassDef::min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: ਵਾਪਸ ((ClassDefFormat1_3<SmallTypes>*)ਇਹ)->sanitize (ਸ਼ੀਰਸ਼);
    ਹਾਲਤ 2: ਵਾਪਸ ((ClassDefFormat2_4<SmallTypes>*)ਇਹ)->sanitize (ਸ਼ੀਰਸ਼);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: ਵਾਪਸ ((ClassDefFormat1_3<MediumTypes>*)ਇਹ)->sanitize (ਸ਼ੀਰਸ਼);
    ਹਾਲਤ 4: ਵਾਪਸ ((ClassDefFormat2_4<MediumTypes>*)ਇਹ)->sanitize (ਸ਼ੀਰਸ਼);
#ਜੇ_ਅੰਤ
    ਮੂਲ: ਵਾਪਸ Err(SANITIZE_FAILURE);
    }
  }
};


ਬਣਤਰ class_def_ਮਾਪ_estimator_t
{
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ It>
  ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<class_def_ਮਾਪ_estimator_t> create (It glyph_and_class)
  {
    class_def_ਮਾਪ_estimator_t estimator;
    estimator.reset();
    ਲਈ (ਸਵੈਚਾਲਿਤ p : + glyph_and_class)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ gid = p.ਪਹਿਲਾ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ klass = p.second;

      hb_ਸੈੱਟ_t* glyphs;
      ਜੇ (estimator.glyphs_ਪ੍ਰਤੀ_class.has (klass, &glyphs) && glyphs) {
        glyphs->ਜੋੜੋ (gid);
        ਜਾਰੀ;
      }

      hb_ਸੈੱਟ_t new_glyphs;
      new_glyphs.ਜੋੜੋ (gid);
      estimator.glyphs_ਪ੍ਰਤੀ_class.ਸੈੱਟ (klass, std::move (new_glyphs));
    }

    TRY(estimator.ਮੰਜ਼ਿਲ_ਨਤੀਜਾ ());

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ klass : estimator.glyphs_ਪ੍ਰਤੀ_class.ਕੁੰਜੀਆਂ ())
    {
      ਜੇ (!klass) ਜਾਰੀ; // class 0 doesn't get encoded.

      ਅਚਲ hb_ਸੈੱਟ_t& glyphs = estimator.glyphs_ਪ੍ਰਤੀ_class.get (klass);
      hb_codepoint_t ਸ਼ੁਰੂ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
      hb_codepoint_t ਸਿਰਾ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
      ਜਦੋਂ (glyphs.ਅਗਲਾ_range (&ਸ਼ੁਰੂ, &ਸਿਰਾ))
        ਗਿਣਤੀ_2++;

      estimator.ਅੰਕ_ranges_ਪ੍ਰਤੀ_class.ਸੈੱਟ (klass, ਗਿਣਤੀ_2);
    }

    TRY(estimator.ਮੰਜ਼ਿਲ_ਨਤੀਜਾ ());
    ਵਾਪਸ estimator;
  }

  ਖਾਲੀ reset() {
    included_glyphs.clear();
    included_class_def_glyphs.clear();
    included_classes.clear();
    class_def_range_ਗਿਣਤੀ = 0;
    class_max = 0;
    coverage_ਮਾਪ_val = compute_coverage_ਮਾਪ ();
  }

  // Size of coverage for all glyphs added via 'add_class_def_size'.
  ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਮਾਪ () ਅਚਲ
  {
    ਵਾਪਸ coverage_ਮਾਪ_val;
  }

  // Compute the new size of the ClassDef table if all glyphs associated with 'klass' were added.
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_class_def_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ klass)
  {
    ਜੇ (!included_classes.has(klass)) {
      hb_ਸੈੱਟ_t* glyphs = ਖਾਲੀ_ਸੂਚਕ;
      ਜੇ (glyphs_ਪ੍ਰਤੀ_class.has(klass, &glyphs)) {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = included_glyphs.get_population();
        included_glyphs.union_(*glyphs);
        ਜੇ (klass)
          included_class_def_glyphs.union_(*glyphs);
        ਜੇ (ਅੰਕ_glyphs != included_glyphs.get_population())
          coverage_ਮਾਪ_val = compute_coverage_ਮਾਪ ();
      }

      ਜੇ (klass)
      {
        class_def_range_ਗਿਣਤੀ += ਅੰਕ_ranges_ਪ੍ਰਤੀ_class.get (klass);
        ਜੇ (glyphs && klass > class_max)
          class_max = klass;
      }
      included_classes.ਜੋੜੋ(klass);
    }

    ਵਾਪਸ class_def_ਮਾਪ ();
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyph_ranges (ਅਚਲ hb_ਸੈੱਟ_t &glyphs) ਅਚਲ {
    hb_codepoint_t ਸ਼ੁਰੂ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
    hb_codepoint_t ਸਿਰਾ = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
    ਜਦੋਂ (glyphs.ਅਗਲਾ_range (&ਸ਼ੁਰੂ, &ਸਿਰਾ)) {
        ਗਿਣਤੀ_2++;
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
  }

 ਨਿਜੀ:
  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਮੰਜ਼ਿਲ_ਨਤੀਜਾ() ਅਚਲ
  {
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (ਅੰਕ_ranges_ਪ੍ਰਤੀ_class, ਵੰਡ_FAILURE));
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (glyphs_ਪ੍ਰਤੀ_class, ਵੰਡ_FAILURE));
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (included_classes, ਵੰਡ_FAILURE));
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (included_glyphs, ਵੰਡ_FAILURE));
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (included_class_def_glyphs, ਵੰਡ_FAILURE));

    ਲਈ (ਅਚਲ hb_ਸੈੱਟ_t& s : glyphs_ਪ੍ਰਤੀ_class.values ())
    {
          TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (s, ਵੰਡ_FAILURE));
    }
    ਵਾਪਸ Ok();
  }

  class_def_ਮਾਪ_estimator_t ()
      : ਅੰਕ_ranges_ਪ੍ਰਤੀ_class (), glyphs_ਪ੍ਰਤੀ_class () {}

  ਬਿਨਾਨਿਸ਼ਾਨੀ compute_coverage_ਮਾਪ () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਮਾਪ = glyph_id_ਮਾਪ (included_glyphs);
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਅਧਾਰ_ਮਾਪ = OT::HBUINT16::static_ਮਾਪ + glyph_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ range_ਮਾਪ = 3 * glyph_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ format1_ਮਾਪ = coverage_ਅਧਾਰ_ਮਾਪ + glyph_ਮਾਪ * included_glyphs.get_population();
    ਬਿਨਾਨਿਸ਼ਾਨੀ format2_ਮਾਪ = coverage_ਅਧਾਰ_ਮਾਪ + range_ਮਾਪ * ਅੰਕ_glyph_ranges (included_glyphs);
    ਵਾਪਸ hb_min(format1_ਮਾਪ, format2_ਮਾਪ);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_id_ਮਾਪ (ਅਚਲ hb_ਸੈੱਟ_t &glyphs) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (!glyphs.is_empty () && glyphs.get_max () > 0xFFFFu)
      ਵਾਪਸ OT::Layout::MediumTypes::ਮਾਪ;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ OT::Layout::SmallTypes::ਮਾਪ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_ਮਾਪ () ਅਚਲ
  {
    ਜੇ (included_class_def_glyphs.is_empty ())
      ਵਾਪਸ OT::HBUINT16::static_ਮਾਪ + OT::Layout::SmallTypes::ਮਾਪ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_ਮਾਪ = UINT_MAX;
    hb_codepoint_t glyph_max = included_class_def_glyphs.get_max ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਵਿਸਤਾਰ = glyph_max - included_class_def_glyphs.get_min () + 1;

    ਜੇ (glyph_max <= 0xFFFFu && glyph_ਵਿਸਤਾਰ <= 0xFFFFu && class_max <= 0xFFFFu)
      ਉੱਤਮ_ਮਾਪ = hb_min (ਉੱਤਮ_ਮਾਪ, 6 + 2 * glyph_ਵਿਸਤਾਰ);
    ਜੇ (glyph_max <= 0xFFFFu && class_def_range_ਗਿਣਤੀ <= 0xFFFFu && class_max <= 0xFFFFu)
      ਉੱਤਮ_ਮਾਪ = hb_min (ਉੱਤਮ_ਮਾਪ, 4 + 6 * class_def_range_ਗਿਣਤੀ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (glyph_max <= 0xFFFFFFu && glyph_ਵਿਸਤਾਰ <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
      ਉੱਤਮ_ਮਾਪ = hb_min (ਉੱਤਮ_ਮਾਪ, 8 + 3 * glyph_ਵਿਸਤਾਰ);
    ਜੇ (glyph_max <= 0xFFFFFFu && class_def_range_ਗਿਣਤੀ <= 0xFFFFFFu && class_max <= 0xFFFFu)
      ਉੱਤਮ_ਮਾਪ = hb_min (ਉੱਤਮ_ਮਾਪ, 5 + 8 * class_def_range_ਗਿਣਤੀ);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਉੱਤਮ_ਮਾਪ;
  }

  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> ਅੰਕ_ranges_ਪ੍ਰਤੀ_class;
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_ਸੈੱਟ_t> glyphs_ਪ੍ਰਤੀ_class;
  hb_ਸੈੱਟ_t included_classes;
  hb_ਸੈੱਟ_t included_glyphs;
  hb_ਸੈੱਟ_t included_class_def_glyphs;
  ਬਿਨਾਨਿਸ਼ਾਨੀ class_def_range_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ class_max;
  ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਮਾਪ_val;
};


}

#ਜੇ_ਅੰਤ  // GRAPH_CLASSDEF_GRAPH_HH
