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

#که_نه_تعریف_شوی HB_OT_VORG_جدول_HH
#تعریف_کړه HB_OT_VORG_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#تعریف_کړه HB_OT_نښان_VORG HB_نښان('V','O','R','G')

نطاق OT {

کينډۍ <ډول_نوم Types>
بنية VertOriginMetric
{
  صحيح cmp (hb_codepoint_t g) ثابت { أرجع glyph.cmp (g); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  ډول_نوم Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  عام:
  DEFINE_اندازه_STATIC (Types::HBGlyphID::static_اندازه + FWORD::static_اندازه);
};

کينډۍ <ډول_نوم Types>
بنية VORGFormat1_2
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_VORG;

  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int (); }

  HB_ALWAYS_INLINE
  صحيح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    بینښه صحيح i;
    إذا (!vertYOrigins.bfind (glyph, &i))
      أرجع defaultVertOriginY;
    أرجع vertYOrigins[i].vertOriginY;
  }

  کينډۍ <ډول_نوم Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    إذا (unlikely (!c->extend_min ((*هذا))))  أرجع;

    هذا->نسخه.major = Types::اندازه == 2 ? 1 : 2;
    هذا->نسخه.minor = 0;

    هذا->defaultVertOriginY = defaultVertOriginY;
    هذا->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *vorg_اولي = c->serializer->پیل_embed<VORGFormat1_2> ();
    إذا (unlikely (!c->serializer->check_success (vorg_اولي))) return_trace (خطأ);

    خپلکاری it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ثابت VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_سېټ_ارزښت_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		أرجع metric;
	      })
    ;

    /* serialize the new table */
    vorg_اولي->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  نسخه.major == (Types::اندازه == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  ډول_نوم Types::کينډۍ Sorted_Array_د<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  عام:
  DEFINE_اندازه_ARRAY(6 + Types::HBUINT::static_اندازه, vertYOrigins);
};

بنية VORG
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_VORG;

  منطقي has_معلومات () ثابت { أرجع u.نسخه.v.منزل_int (); }

  HB_ALWAYS_INLINE
  صحيح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    بدل (u.نسخه.v.major) {
    حالة 1: أرجع u.version1.get_y_origin (glyph);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 2: أرجع u.version2.get_y_origin (glyph);
#د_که_پای
    افتراضي: أرجع 0;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.نسخه.v.major) {
    حالة 1: return_trace (u.version1.subset (c));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 2: return_trace (u.version2.subset (c));
#د_که_پای
    افتراضي: return_trace (خطأ);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.نسخه.v.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.نسخه.v.major) {
    حالة 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#د_که_پای
    افتراضي: return_trace (خطأ);
    }
  }

  محمي:
  اتحاد {
  بنية {
  Fixed_نسخه<>v;		/* Version of VORG table. */
  }				نسخه;
  VORGFormat1_2<Layout::SmallTypes>version1;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#د_که_پای
  } u;

  عام:
  DEFINE_اندازه_UNION (4, نسخه.v);
};
} /* namespace OT */

#د_که_پای /* HB_OT_VORG_TABLE_HH */
