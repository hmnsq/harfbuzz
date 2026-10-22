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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#define HB_OT_TAG_VORG HB_TAG('V','O','R','G')

નામાકાશ OT {

template <typename Types>
સંરચના VertOriginMetric
{
  પૂર્ણાંક cmp (hb_codepoint_t g) અચલ { પાછા glyph.cmp (g); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  જાહેર:
  DEFINE_SIZE_STATIC (Types::HBGlyphID::static_size + FWORD::static_size);
};

template <typename Types>
સંરચના VORGFormat1_2
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  તાર્કિક has_data () અચલ { પાછા version.to_int (); }

  HB_ALWAYS_INLINE
  પૂર્ણાંક get_y_origin (hb_codepoint_t glyph) અચલ
  {
    unsigned પૂર્ણાંક i;
    જો (!vertYOrigins.bfind (glyph, &i))
      પાછા defaultVertOriginY;
    પાછા vertYOrigins[i].vertOriginY;
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    જો (unlikely (!c->extend_min ((*આ))))  પાછા;

    આ->version.major = Types::size == 2 ? 1 : 2;
    આ->version.minor = 0;

    આ->defaultVertOriginY = defaultVertOriginY;
    આ->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *vorg_અવિભાજ્ય = c->serializer->start_embed<VORGFormat1_2> ();
    જો (unlikely (!c->serializer->check_success (vorg_અવિભાજ્ય))) return_trace (ખોટું);

    auto it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (અચલ VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_કિંમત_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		પાછા metric;
	      })
    ;

    /* serialize the new table */
    vorg_અવિભાજ્ય->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  સુરક્ષિત:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::template SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  જાહેર:
  DEFINE_SIZE_ARRAY(6 + Types::HBUINT::static_size, vertYOrigins);
};

સંરચના VORG
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  તાર્કિક has_data () અચલ { પાછા u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  પૂર્ણાંક get_y_origin (hb_codepoint_t glyph) અચલ
  {
    પસંદગી (u.version.v.major) {
    સ્થિતિ 1: પાછા u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 2: પાછા u.version2.get_y_origin (glyph);
#endif
    મૂળભૂત: પાછા 0;
    }
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    પસંદગી (u.version.v.major) {
    સ્થિતિ 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 2: return_trace (u.version2.subset (c));
#endif
    મૂળભૂત: return_trace (ખોટું);
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!u.version.v.sanitize (c))) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.version.v.major) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    મૂળભૂત: return_trace (ખોટું);
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  જાહેર:
  DEFINE_SIZE_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
