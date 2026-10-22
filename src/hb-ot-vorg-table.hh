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

#اگر_غیر_تعریف_شدہ HB_OT_VORG_جدول_HH
#تعریف_کرو HB_OT_VORG_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#تعریف_کرو HB_OT_لیبل_VORG HB_لیبل('V','O','R','G')

نامخلا OT {

سانچہ <قسم_نام Types>
ساخت VertOriginMetric
{
  صحیح cmp (hb_codepoint_t g) ثابت { واپس glyph.cmp (g); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  قسم_نام Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  عوامی:
  DEFINE_سائز_STATIC (Types::HBGlyphID::static_سائز + FWORD::static_سائز);
};

سانچہ <قسم_نام Types>
ساخت VORGFormat1_2
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_VORG;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }

  HB_ALWAYS_INLINE
  صحیح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    غیرعلامتی صحیح i;
    اگر (!vertYOrigins.bfind (glyph, &i))
      واپس defaultVertOriginY;
    واپس vertYOrigins[i].vertOriginY;
  }

  سانچہ <قسم_نام Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  خالی serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    اگر (unlikely (!c->extend_min ((*یہ))))  واپس;

    یہ->نسخہ.major = Types::سائز == 2 ? 1 : 2;
    یہ->نسخہ.minor = 0;

    یہ->defaultVertOriginY = defaultVertOriginY;
    یہ->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *vorg_مفرد = c->serializer->آغاز_embed<VORGFormat1_2> ();
    اگر (unlikely (!c->serializer->check_success (vorg_مفرد))) return_trace (جھوٹ);

    خودکار it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ثابت VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_سیٹ_قدر_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		واپس metric;
	      })
    ;

    /* serialize the new table */
    vorg_مفرد->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  نسخہ.major == (Types::سائز == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  قسم_نام Types::سانچہ Sorted_Array_کا<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  عوامی:
  DEFINE_سائز_ARRAY(6 + Types::HBUINT::static_سائز, vertYOrigins);
};

ساخت VORG
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_VORG;

  منطقی has_ڈیٹا () ثابت { واپس u.نسخہ.v.مقصد_int (); }

  HB_ALWAYS_INLINE
  صحیح get_y_origin (hb_codepoint_t glyph) ثابت
  {
    انتخاب (u.نسخہ.v.major) {
    حالت 1: واپس u.version1.get_y_origin (glyph);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2: واپس u.version2.get_y_origin (glyph);
#اگر_ختم
    طےشدہ: واپس 0;
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    انتخاب (u.نسخہ.v.major) {
    حالت 1: return_trace (u.version1.subset (c));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2: return_trace (u.version2.subset (c));
#اگر_ختم
    طےشدہ: return_trace (جھوٹ);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!u.نسخہ.v.sanitize (c))) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.نسخہ.v.major) {
    حالت 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#اگر_ختم
    طےشدہ: return_trace (جھوٹ);
    }
  }

  محفوظ:
  اتحاد {
  ساخت {
  Fixed_نسخہ<>v;		/* Version of VORG table. */
  }				نسخہ;
  VORGFormat1_2<Layout::SmallTypes>version1;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#اگر_ختم
  } u;

  عوامی:
  DEFINE_سائز_UNION (4, نسخہ.v);
};
} /* namespace OT */

#اگر_ختم /* HB_OT_VORG_TABLE_HH */
