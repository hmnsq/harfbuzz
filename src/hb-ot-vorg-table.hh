/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VORG_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VORG_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_VORG HB_ਟੈਗ('V','O','R','G')

ਨਾਮਾਕਾਸ਼ OT {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ VertOriginMetric
{
  ਪੂਰਨ cmp (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ glyph.cmp (g); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  ਕਿਸਮ_ਨਾਂ Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (Types::HBGlyphID::static_ਮਾਪ + FWORD::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ VORGFormat1_2
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_VORG;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }

  HB_ALWAYS_INLINE
  ਪੂਰਨ get_y_origin (hb_codepoint_t glyph) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
    ਜੇ (!vertYOrigins.bfind (glyph, &i))
      ਵਾਪਸ defaultVertOriginY;
    ਵਾਪਸ vertYOrigins[i].vertOriginY;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    ਜੇ (unlikely (!c->extend_min ((*ਇਹ))))  ਵਾਪਸ;

    ਇਹ->ਸੰਸਕਰਣ.major = Types::ਮਾਪ == 2 ? 1 : 2;
    ਇਹ->ਸੰਸਕਰਣ.minor = 0;

    ਇਹ->defaultVertOriginY = defaultVertOriginY;
    ਇਹ->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *vorg_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed<VORGFormat1_2> ();
    ਜੇ (unlikely (!c->serializer->check_success (vorg_ਅਭਾਜ))) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ਅਚਲ VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_ਸੈੱਟ_ਮੁੱਲ_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		ਵਾਪਸ metric;
	      })
    ;

    /* serialize the new table */
    vorg_ਅਭਾਜ->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਸੰਸਕਰਣ.major == (Types::ਮਾਪ == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Sorted_Array_ਦਾ<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY(6 + Types::HBUINT::static_ਮਾਪ, vertYOrigins);
};

ਬਣਤਰ VORG
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_VORG;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ u.ਸੰਸਕਰਣ.v.ਮੰਜ਼ਿਲ_int (); }

  HB_ALWAYS_INLINE
  ਪੂਰਨ get_y_origin (hb_codepoint_t glyph) ਅਚਲ
  {
    ਚੋਣ (u.ਸੰਸਕਰਣ.v.major) {
    ਹਾਲਤ 1: ਵਾਪਸ u.version1.get_y_origin (glyph);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 2: ਵਾਪਸ u.version2.get_y_origin (glyph);
#ਜੇ_ਅੰਤ
    ਮੂਲ: ਵਾਪਸ 0;
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਚੋਣ (u.ਸੰਸਕਰਣ.v.major) {
    ਹਾਲਤ 1: return_trace (u.version1.subset (c));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 2: return_trace (u.version2.subset (c));
#ਜੇ_ਅੰਤ
    ਮੂਲ: return_trace (ਝੂਠ);
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!u.ਸੰਸਕਰਣ.v.sanitize (c))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.ਸੰਸਕਰਣ.v.major) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#ਜੇ_ਅੰਤ
    ਮੂਲ: return_trace (ਝੂਠ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ {
  Fixed_ਸੰਸਕਰਣ<>v;		/* Version of VORG table. */
  }				ਸੰਸਕਰਣ;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#ਜੇ_ਅੰਤ
  } u;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (4, ਸੰਸਕਰਣ.v);
};
} /* namespace OT */

#ਜੇ_ਅੰਤ /* HB_OT_VORG_TABLE_HH */
