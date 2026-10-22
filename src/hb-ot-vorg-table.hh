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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_VORG_جدول_HH
#وصف_ڪريو HB_OT_VORG_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#وصف_ڪريو HB_OT_ٽيگ_VORG HB_ٽيگ('V','O','R','G')

نطاق OT {

سانچو <قسم_نالو Types>
بنية VertOriginMetric
{
  صحيح cmp (hb_codepoint_t g) ثابت { أرجع glyph.cmp (g); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  قسم_نالو Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  عام:
  DEFINE_سائيز_STATIC (Types::HBGlyphID::static_سائيز + FWORD::static_سائيز);
};

سانچو <قسم_نالو Types>
بنية VORGFormat1_2
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_VORG;

  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int (); }

  HB_ALWAYS_INLINE
  صحيح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    بنانشان صحيح i;
    إذا (!vertYOrigins.bfind (glyph, &i))
      أرجع defaultVertOriginY;
    أرجع vertYOrigins[i].vertOriginY;
  }

  سانچو <قسم_نالو Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    إذا (unlikely (!c->extend_min ((*هذا))))  أرجع;

    هذا->نسخو.major = Types::سائيز == 2 ? 1 : 2;
    هذا->نسخو.minor = 0;

    هذا->defaultVertOriginY = defaultVertOriginY;
    هذا->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *vorg_مفرد = c->serializer->شروعات_embed<VORGFormat1_2> ();
    إذا (unlikely (!c->serializer->check_success (vorg_مفرد))) return_trace (خطأ);

    خودڪار it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ثابت VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_سيٽ_قدر_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		أرجع metric;
	      })
    ;

    /* serialize the new table */
    vorg_مفرد->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  نسخو.major == (Types::سائيز == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  محمي:
  Fixed_نسخو<>نسخو;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  قسم_نالو Types::سانچو Sorted_Array_جو<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  عام:
  DEFINE_سائيز_ARRAY(6 + Types::HBUINT::static_سائيز, vertYOrigins);
};

بنية VORG
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_VORG;

  منطقي has_معلومات () ثابت { أرجع u.نسخو.v.منزل_int (); }

  HB_ALWAYS_INLINE
  صحيح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    بدل (u.نسخو.v.major) {
    حالة 1: أرجع u.version1.get_y_origin (glyph);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 2: أرجع u.version2.get_y_origin (glyph);
#جيڪڏهن_پورو
    افتراضي: أرجع 0;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.نسخو.v.major) {
    حالة 1: return_trace (u.version1.subset (c));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 2: return_trace (u.version2.subset (c));
#جيڪڏهن_پورو
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.نسخو.v.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.نسخو.v.major) {
    حالة 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#جيڪڏهن_پورو
    افتراضي: return_trace (خطأ);
    }
  }

  محمي:
  اتحاد {
  بنية {
  Fixed_نسخو<>v;		/* Version of VORG table. */
  }				نسخو;
  VORGFormat1_2<Layout::SmallTypes>version1;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#جيڪڏهن_پورو
  } u;

  عام:
  DEFINE_سائيز_UNION (4, نسخو.v);
};
} /* namespace OT */

#جيڪڏهن_پورو /* HB_OT_VORG_TABLE_HH */
