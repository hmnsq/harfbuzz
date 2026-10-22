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
#ਅਨ੍ਤਰ੍ਭਾਵ "../OT/Layout/Common/Coverage.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_COVERAGE_ਗ੍ਰਾਫ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_COVERAGE_ਗ੍ਰਾਫ_HH

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (
  ਅਚਲ OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼
) {
  size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ();
  constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = OT::Layout::Common::CoverageFormat1_3<Types>::min_ਮਾਪ;
  ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
  hb_barrier ();
  ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ + thiz->glyphArray.get_ਮਾਪ () - thiz->glyphArray.len.get_ਮਾਪ ()))
    ਵਾਪਸ Err(SANITIZE_FAILURE);
  ਵਾਪਸ Ok();
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (
  ਅਚਲ OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼
) {
  size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ();
  constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਮਾਪ = OT::Layout::Common::CoverageFormat2_4<Types>::min_ਮਾਪ;
  ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
  hb_barrier ();
  ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < min_ਮਾਪ + thiz->range_ਰਿਕਾਰਡ.get_ਮਾਪ () - thiz->range_ਰਿਕਾਰਡ.len.get_ਮਾਪ ()))
    ਵਾਪਸ Err(SANITIZE_FAILURE);
  ਵਾਪਸ Ok();
}

ਬਣਤਰ Coverage : ਜਨਤਕ OT::Layout::Common::Coverage
{
  ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> clone_coverage (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_id,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਮਾਪਾ_id,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ link_position,
                                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ)

  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਮਾਪ = c.ਗ੍ਰਾਫ.vertices_[coverage_id].ਸਾਰਣੀ_ਮਾਪ ();
    ਸਵੈਚਾਲਿਤ& coverage_v = c.ਗ੍ਰਾਫ.vertices_[coverage_id];
    ਅਚਲ Coverage* coverage_ਸਾਰਣੀ = (ਅਚਲ Coverage*) coverage_v.obj().ਸਿਰ_2;
    ਜੇ (unlikely (!coverage_ਸਾਰਣੀ))
      ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    TRY (coverage_ਸਾਰਣੀ->sanitize (coverage_v));

    ਸਵੈਚਾਲਿਤ new_coverage =
        + hb_zip (coverage_ਸਾਰਣੀ->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> p) {
          ਵਾਪਸ p.second >= ਸ਼ੁਰੂ && p.second < ਸਿਰਾ;
        })
        | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
        ;

    ਵਾਪਸ ਜੋੜੋ_coverage (c, new_ਮਾਪਾ_id, link_position, new_coverage, coverage_ਮਾਪ);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ It>
  ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> ਜੋੜੋ_coverage (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                            ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_id,
                                            ਬਿਨਾਨਿਸ਼ਾਨੀ link_position,
                                            It glyphs,
                                            ਬਿਨਾਨਿਸ਼ਾਨੀ max_size)
  {
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਅਭਾਜ_id, c.ਗ੍ਰਾਫ.new_ਗੰਢ (ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ));
    ਸਵੈਚਾਲਿਤ& coverage_ਅਭਾਜ_ਸ਼ੀਰਸ਼ = c.ਗ੍ਰਾਫ.vertices_[coverage_ਅਭਾਜ_id];
    TRY (make_coverage (c, glyphs, coverage_ਅਭਾਜ_id, max_size));

    TRY(c.ਗ੍ਰਾਫ.vertices_[ਮਾਪਾ_id].ਜੋੜੋ_real_link (SmallTypes::ਮਾਪ, coverage_ਅਭਾਜ_id, link_position));
    TRY(coverage_ਅਭਾਜ_ਸ਼ੀਰਸ਼.ਜੋੜੋ_ਮਾਪਾ (ਮਾਪਾ_id, ਝੂਠ));

    ਵਾਪਸ Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> filter_coverage (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                                ਬਿਨਾਨਿਸ਼ਾਨੀ existing_coverage,
                                                ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ) {
    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_ਮਾਪ = c.ਗ੍ਰਾਫ.vertices_[existing_coverage].ਸਾਰਣੀ_ਮਾਪ ();
    ਸਵੈਚਾਲਿਤ& coverage_v = c.ਗ੍ਰਾਫ.vertices_[existing_coverage];
    ਅਚਲ Coverage* coverage_ਸਾਰਣੀ = (ਅਚਲ Coverage*) coverage_v.obj().ਸਿਰ_2;
    ਜੇ (unlikely (!coverage_ਸਾਰਣੀ))
      ਵਾਪਸ Err(INVALID_ਦਲੀਲ);
    TRY (coverage_ਸਾਰਣੀ->sanitize (coverage_v));

    ਸਵੈਚਾਲਿਤ new_coverage =
        + hb_zip (coverage_ਸਾਰਣੀ->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> p) {
          ਵਾਪਸ p.second >= ਸ਼ੁਰੂ && p.second < ਸਿਰਾ;
        })
        | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
        ;

    ਵਾਪਸ make_coverage (c, new_coverage, existing_coverage, coverage_ਮਾਪ * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ It>
  ਸਥਿਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> make_coverage (gsubgpos_ਗ੍ਰਾਫ_context_t& c,
                                             It glyphs,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ dest_obj,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ max_size)
  {
    ਅੱਖਰ* ਬਫਰ = (ਅੱਖਰ*) hb_calloc (1, max_size);
    ਜੇ (unlikely (!ਬਫਰ))
      ਵਾਪਸ Err(ਵੰਡ_FAILURE);

    hb_serialize_context_t serializer (ਬਫਰ, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.ਸਿਰਾ_serialize ();
    ਜੇ (unlikely (serializer.ਵਿੱਚ_error ()))
    {
      hb_free (ਬਫਰ);
      ਵਾਪਸ Err(ਵੰਡ_FAILURE);
    }

    hb_ਬਾਈਟਾਂ_t coverage_copy = serializer.copy_ਬਾਈਟਾਂ ();
    ਜੇ (unlikely (!coverage_copy.arrayZ)) {
      hb_free (ਬਫਰ);
      ਵਾਪਸ Err(ਵੰਡ_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    ਸਵੈਚਾਲਿਤ res = c.ਜੋੜੋ_ਬਫਰ ((ਅੱਖਰ *) coverage_copy.arrayZ);
    ਜੇ (unlikely (!res.is_ok ()))
    {
      hb_free (ਬਫਰ);
      hb_free ((ਅੱਖਰ *) coverage_copy.arrayZ);
      ਵਾਪਸ res;
    }

    ਸਵੈਚਾਲਿਤ& v = c.ਗ੍ਰਾਫ.vertices_[dest_obj];
    ਅੱਖਰ* ਸਿਰ_2 = (ਅੱਖਰ *) coverage_copy.arrayZ;
    v.ਸੈੱਟ_ਬਫਰ (ਸਿਰ_2, ਸਿਰ_2 + coverage_copy.ਲੰਮਾਈ);

    hb_free (ਬਫਰ);
    ਵਾਪਸ Ok();
  }

  ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> sanitize (ਅਚਲ ਗ੍ਰਾਫ_t::ਸ਼ੀਰਸ਼_t& ਸ਼ੀਰਸ਼) ਅਚਲ
  {
    size_t ਸ਼ੀਰਸ਼_len = ਸ਼ੀਰਸ਼.ਸਾਰਣੀ_ਮਾਪ ();
    ਜੇ (unlikely (ਸ਼ੀਰਸ਼_len < OT::Layout::Common::Coverage::min_ਮਾਪ)) ਵਾਪਸ Err(SANITIZE_FAILURE);
    hb_barrier ();
    ਚੋਣ (u.format.v)
    {
    ਹਾਲਤ 1: ਵਾਪਸ ਗ੍ਰਾਫ::sanitize ((ਅਚਲ OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) ਇਹ, ਸ਼ੀਰਸ਼);
    ਹਾਲਤ 2: ਵਾਪਸ ਗ੍ਰਾਫ::sanitize ((ਅਚਲ OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) ਇਹ, ਸ਼ੀਰਸ਼);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: ਵਾਪਸ ਗ੍ਰਾਫ::sanitize ((ਅਚਲ OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) ਇਹ, ਸ਼ੀਰਸ਼);
    ਹਾਲਤ 4: ਵਾਪਸ ਗ੍ਰਾਫ::sanitize ((ਅਚਲ OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) ਇਹ, ਸ਼ੀਰਸ਼);
#ਜੇ_ਅੰਤ
    ਮੂਲ: ਵਾਪਸ Err(SANITIZE_FAILURE);
    }
  }
};


}

#ਜੇ_ਅੰਤ  // GRAPH_COVERAGE_GRAPH_HH
