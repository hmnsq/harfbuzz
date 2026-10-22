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

#اگر_تعریف_نشده HB_OT_VORG_جدول_HH
#تعریف_کن HB_OT_VORG_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#تعریف_کن HB_OT_برچسب_VORG HB_برچسب('V','O','R','G')

نطاق OT {

قالب <نام_نوع Types>
بنية VertOriginMetric
{
  صحيح cmp (hb_codepoint_t g) ثابت { أرجع glyph.cmp (g); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  نام_نوع Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  عام:
  DEFINE_اندازه_STATIC (Types::HBGlyphID::static_اندازه + FWORD::static_اندازه);
};

قالب <نام_نوع Types>
بنية VORGFormat1_2
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_VORG;

  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int (); }

  HB_ALWAYS_INLINE
  صحيح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    بیعلامت صحيح i;
    إذا (!vertYOrigins.bfind (glyph, &i))
      أرجع defaultVertOriginY;
    أرجع vertYOrigins[i].vertOriginY;
  }

  قالب <نام_نوع Iterator,
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
    خودکار *vorg_اول = c->serializer->شروع_embed<VORGFormat1_2> ();
    إذا (unlikely (!c->serializer->check_success (vorg_اول))) return_trace (خطأ);

    خودکار it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ثابت VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_مجموعه_مقدار_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		أرجع metric;
	      })
    ;

    /* serialize the new table */
    vorg_اول->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (درست);
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
  نام_نوع Types::قالب Sorted_Array_از<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  عام:
  DEFINE_اندازه_ARRAY(6 + Types::HBUINT::static_اندازه, vertYOrigins);
};

بنية VORG
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_VORG;

  منطقي has_داده () ثابت { أرجع u.نسخه.v.مقصد_int (); }

  HB_ALWAYS_INLINE
  صحيح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    بدل (u.نسخه.v.major) {
    حالة 1: أرجع u.version1.get_y_origin (glyph);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 2: أرجع u.version2.get_y_origin (glyph);
#پایان_اگر
    افتراضي: أرجع 0;
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.نسخه.v.major) {
    حالة 1: return_trace (u.version1.subset (c));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 2: return_trace (u.version2.subset (c));
#پایان_اگر
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
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#پایان_اگر
    افتراضي: return_trace (خطأ);
    }
  }

  محمي:
  اتحاد {
  بنية {
  Fixed_نسخه<>v;		/* Version of VORG table. */
  }				نسخه;
  VORGFormat1_2<Layout::SmallTypes>version1;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#پایان_اگر
  } u;

  عام:
  DEFINE_اندازه_UNION (4, نسخه.v);
};
} /* namespace OT */

#پایان_اگر /* HB_OT_VORG_TABLE_HH */
