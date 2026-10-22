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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#define HB_OT_TAG_VORG HB_TAG('V','O','R','G')

নামাকাশ OT {

template <typename Types>
সংরচনা VertOriginMetric
{
  পূর্ণাঙ্ক cmp (hb_codepoint_t g) ধ্রুব { প্রত্যাগম glyph.cmp (g); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্বজনিক:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  সার্বজনিক:
  DEFINE_SIZE_STATIC (Types::HBGlyphID::static_size + FWORD::static_size);
};

template <typename Types>
সংরচনা VORGFormat1_2
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  তার্কিক has_data () ধ্রুব { প্রত্যাগম version.to_int (); }

  HB_ALWAYS_INLINE
  পূর্ণাঙ্ক get_y_origin (hb_codepoint_t glyph) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক i;
    যদি (!vertYOrigins.bfind (glyph, &i))
      প্রত্যাগম defaultVertOriginY;
    প্রত্যাগম vertYOrigins[i].vertOriginY;
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    যদি (unlikely (!c->extend_min ((*এতত্))))  প্রত্যাগম;

    এতত্->version.major = Types::size == 2 ? 1 : 2;
    এতত্->version.minor = 0;

    এতত্->defaultVertOriginY = defaultVertOriginY;
    এতত্->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *vorg_মৌলিক = c->serializer->start_embed<VORGFormat1_2> ();
    যদি (unlikely (!c->serializer->check_success (vorg_মৌলিক))) return_trace (অসত্য);

    auto it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ধ্রুব VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_মান_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		প্রত্যাগম metric;
	      })
    ;

    /* serialize the new table */
    vorg_মৌলিক->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  রক্ষিত:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::template SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  সার্বজনিক:
  DEFINE_SIZE_ARRAY(6 + Types::HBUINT::static_size, vertYOrigins);
};

সংরচনা VORG
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  তার্কিক has_data () ধ্রুব { প্রত্যাগম u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  পূর্ণাঙ্ক get_y_origin (hb_codepoint_t glyph) ধ্রুব
  {
    বিকল্প (u.version.v.major) {
    পক্ষ 1: প্রত্যাগম u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 2: প্রত্যাগম u.version2.get_y_origin (glyph);
#endif
    স্বভাব: প্রত্যাগম 0;
    }
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    বিকল্প (u.version.v.major) {
    পক্ষ 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 2: return_trace (u.version2.subset (c));
#endif
    স্বভাব: return_trace (অসত্য);
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!u.version.v.sanitize (c))) return_trace (অসত্য);
    hb_barrier ();
    বিকল্প (u.version.v.major) {
    পক্ষ 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    স্বভাব: return_trace (অসত্য);
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  সার্বজনিক:
  DEFINE_SIZE_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
