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

#ifndef HB_OT_VORG_TABLE_HH
#define HB_OT_VORG_TABLE_HH

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#define HB_OT_TAG_VORG HB_TAG('V','O','R','G')

ನಾಮಾಕಾಶ OT {

template <typename Types>
ಸಂರಚನಾ VertOriginMetric
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ glyph.cmp (g); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (Types::HBGlyphID::static_size + FWORD::static_size);
};

template <typename Types>
ಸಂರಚನಾ VORGFormat1_2
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }

  HB_ALWAYS_INLINE
  ಪೂರ್ಣಾಙ್ಕ get_y_origin (hb_codepoint_t glyph) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ (!vertYOrigins.bfind (glyph, &i))
      ಪ್ರತ್ಯಾಗಮ defaultVertOriginY;
    ಪ್ರತ್ಯಾಗಮ vertYOrigins[i].vertOriginY;
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    ಯದಿ (unlikely (!c->extend_min ((*ಏತತ್))))  ಪ್ರತ್ಯಾಗಮ;

    ಏತತ್->version.major = Types::size == 2 ? 1 : 2;
    ಏತತ್->version.minor = 0;

    ಏತತ್->defaultVertOriginY = defaultVertOriginY;
    ಏತತ್->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *vorg_ಅವಿಭಾಜ್ಯ = c->serializer->start_embed<VORGFormat1_2> ();
    ಯದಿ (unlikely (!c->serializer->check_success (vorg_ಅವಿಭಾಜ್ಯ))) return_trace (ಅಸತ್ಯ);

    auto it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ಧ್ರುವ VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_ಮೌಲ್ಯ_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		ಪ್ರತ್ಯಾಗಮ metric;
	      })
    ;

    /* serialize the new table */
    vorg_ಅವಿಭಾಜ್ಯ->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::template SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY(6 + Types::HBUINT::static_size, vertYOrigins);
};

ಸಂರಚನಾ VORG
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  ಪೂರ್ಣಾಙ್ಕ get_y_origin (hb_codepoint_t glyph) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.version.v.major) {
    ಪಕ್ಷ 1: ಪ್ರತ್ಯಾಗಮ u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 2: ಪ್ರತ್ಯಾಗಮ u.version2.get_y_origin (glyph);
#endif
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ವಿಕಲ್ಪ (u.version.v.major) {
    ಪಕ್ಷ 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 2: return_trace (u.version2.subset (c));
#endif
    ಸ್ವಭಾವ: return_trace (ಅಸತ್ಯ);
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!u.version.v.sanitize (c))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.version.v.major) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    ಸ್ವಭಾವ: return_trace (ಅಸತ್ಯ);
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
