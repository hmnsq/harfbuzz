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

#شامل "hb-open-type.hh"
#شامل "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#define HB_OT_TAG_VORG HB_TAG('V','O','R','G')

فضاینام OT {

template <typename Types>
ساختار VertOriginMetric
{
  صحیح cmp (hb_codepoint_t g) ثابت { بازگشت glyph.cmp (g); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  عمومی:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  عمومی:
  DEFINE_SIZE_STATIC (Types::HBGlyphID::static_size + FWORD::static_size);
};

template <typename Types>
ساختار VORGFormat1_2
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  منطقی has_data () ثابت { بازگشت version.to_int (); }

  HB_ALWAYS_INLINE
  صحیح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    unsigned صحیح i;
    اگر (!vertYOrigins.bfind (glyph, &i))
      بازگشت defaultVertOriginY;
    بازگشت vertYOrigins[i].vertOriginY;
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  تهی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    اگر (unlikely (!c->extend_min ((*این))))  بازگشت;

    این->version.major = Types::size == 2 ? 1 : 2;
    این->version.minor = 0;

    این->defaultVertOriginY = defaultVertOriginY;
    این->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *vorg_اول = c->serializer->start_embed<VORGFormat1_2> ();
    اگر (unlikely (!c->serializer->check_success (vorg_اول))) return_trace (نادرست);

    auto it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ثابت VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_مقدار_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		بازگشت metric;
	      })
    ;

    /* serialize the new table */
    vorg_اول->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  محافظتشده:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::template SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  عمومی:
  DEFINE_SIZE_ARRAY(6 + Types::HBUINT::static_size, vertYOrigins);
};

ساختار VORG
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  منطقی has_data () ثابت { بازگشت u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  صحیح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    انتخاب (u.version.v.major) {
    حالت 1: بازگشت u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    حالت 2: بازگشت u.version2.get_y_origin (glyph);
#endif
    پیشفرض: بازگشت 0;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    انتخاب (u.version.v.major) {
    حالت 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    حالت 2: return_trace (u.version2.subset (c));
#endif
    پیشفرض: return_trace (نادرست);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!u.version.v.sanitize (c))) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.version.v.major) {
    حالت 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    حالت 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    پیشفرض: return_trace (نادرست);
    }
  }

  محافظتشده:
  اجتماع {
  ساختار {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  عمومی:
  DEFINE_SIZE_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
