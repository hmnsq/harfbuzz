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
কাঠামো VertOriginMetric
{
  পূর্ণসংখ্যা cmp (hb_codepoint_t g) ধ্রুবক { ফেরত glyph.cmp (g); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  সর্বজনীন:
  DEFINE_আকার_STATIC (Types::HBGlyphID::static_আকার + FWORD::static_আকার);
};

template <typename Types>
কাঠামো VORGFormat1_2
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int (); }

  HB_ALWAYS_INLINE
  পূর্ণসংখ্যা get_y_origin (hb_codepoint_t glyph) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i;
    যদি (!vertYOrigins.bfind (glyph, &i))
      ফেরত defaultVertOriginY;
    ফেরত vertYOrigins[i].vertOriginY;
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    যদি (unlikely (!c->extend_min ((*এই))))  ফেরত;

    এই->version.major = Types::আকার_2 == 2 ? 1 : 2;
    এই->version.minor = 0;

    এই->defaultVertOriginY = defaultVertOriginY;
    এই->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *vorg_মৌলিক = c->serializer->start_embed<VORGFormat1_2> ();
    যদি (unlikely (!c->serializer->check_success (vorg_মৌলিক))) return_trace (মিথ্যা);

    auto it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ধ্রুবক VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_মান_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		ফেরত metric;
	      })
    ;

    /* serialize the new table */
    vorg_মৌলিক->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  version.major == (Types::আকার_2 == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  সুরক্ষিত:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::template SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  সর্বজনীন:
  DEFINE_আকার_ARRAY(6 + Types::HBUINT::static_আকার, vertYOrigins);
};

কাঠামো VORG
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  পূর্ণসংখ্যা get_y_origin (hb_codepoint_t glyph) ধ্রুবক
  {
    নির্বাচন (u.version.v.major) {
    ক্ষেত্র 1: ফেরত u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 2: ফেরত u.version2.get_y_origin (glyph);
#endif
    পূর্বনির্ধারিত: ফেরত 0;
    }
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    নির্বাচন (u.version.v.major) {
    ক্ষেত্র 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 2: return_trace (u.version2.subset (c));
#endif
    পূর্বনির্ধারিত: return_trace (মিথ্যা);
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!u.version.v.sanitize (c))) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.version.v.major) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    পূর্বনির্ধারিত: return_trace (মিথ্যা);
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  সর্বজনীন:
  DEFINE_আকার_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
