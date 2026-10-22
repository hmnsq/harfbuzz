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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#define HB_OT_TAG_VORG HB_TAG('V','O','R','G')

നാമാകാശ OT {

template <typename Types>
ഘടന VertOriginMetric
{
  പൂർണ്ണസംഖ്യ cmp (hb_codepoint_t g) സ്ഥിരം { തിരികെ glyph.cmp (g); }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  പൊതു:
  DEFINE_SIZE_STATIC (Types::HBGlyphID::static_size + FWORD::static_size);
};

template <typename Types>
ഘടന VORGFormat1_2
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }

  HB_ALWAYS_INLINE
  പൂർണ്ണസംഖ്യ get_y_origin (hb_codepoint_t glyph) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ (!vertYOrigins.bfind (glyph, &i))
      തിരികെ defaultVertOriginY;
    തിരികെ vertYOrigins[i].vertOriginY;
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    എങ്കിൽ (unlikely (!c->extend_min ((*ഇത്))))  തിരികെ;

    ഇത്->version.major = Types::size == 2 ? 1 : 2;
    ഇത്->version.minor = 0;

    ഇത്->defaultVertOriginY = defaultVertOriginY;
    ഇത്->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *vorg_അഭാജ്യം = c->serializer->start_embed<VORGFormat1_2> ();
    എങ്കിൽ (unlikely (!c->serializer->check_success (vorg_അഭാജ്യം))) return_trace (അസത്യം);

    auto it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (സ്ഥിരം VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_മൂല്യം_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		തിരികെ metric;
	      })
    ;

    /* serialize the new table */
    vorg_അഭാജ്യം->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  സംരക്ഷിത:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::template SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  പൊതു:
  DEFINE_SIZE_ARRAY(6 + Types::HBUINT::static_size, vertYOrigins);
};

ഘടന VORG
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  തർക്കം has_data () സ്ഥിരം { തിരികെ u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  പൂർണ്ണസംഖ്യ get_y_origin (hb_codepoint_t glyph) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.version.v.major) {
    അവസ്ഥ 1: തിരികെ u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 2: തിരികെ u.version2.get_y_origin (glyph);
#endif
    സ്ഥിരസ്ഥിതി: തിരികെ 0;
    }
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    തിരഞ്ഞെടുപ്പ് (u.version.v.major) {
    അവസ്ഥ 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 2: return_trace (u.version2.subset (c));
#endif
    സ്ഥിരസ്ഥിതി: return_trace (അസത്യം);
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!u.version.v.sanitize (c))) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.version.v.major) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    സ്ഥിരസ്ഥിതി: return_trace (അസത്യം);
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  പൊതു:
  DEFINE_SIZE_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
