/*
 * Copyright © 2022  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#ضمن "graph.hh"
#ضمن "../OT/Layout/Common/Coverage.hh"

#که_نه_تعریف_شوی ګراف_COVERAGE_ګراف_HH
#تعریف_کړه ګراف_COVERAGE_ګراف_HH

نطاق ګراف {

کينډۍ <ډول_نوم Types>
ساكن ګراف_پایله_t<فراغ> sanitize (
  ثابت OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  ثابت ګراف_t::راس_t& راس
) {
  size_t راس_len = راس.جدول_اندازه();
  constexpr بینښه min_اندازه = OT::Layout::Common::CoverageFormat1_3<Types>::min_اندازه;
  إذا (unlikely (راس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
  hb_barrier ();
  إذا (unlikely (راس_len < min_اندازه + thiz->glyphArray.get_اندازه () - thiz->glyphArray.len.get_اندازه ()))
    أرجع Err(SANITIZE_FAILURE);
  أرجع Ok();
}

کينډۍ <ډول_نوم Types>
ساكن ګراف_پایله_t<فراغ> sanitize (
  ثابت OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  ثابت ګراف_t::راس_t& راس
) {
  size_t راس_len = راس.جدول_اندازه();
  constexpr بینښه min_اندازه = OT::Layout::Common::CoverageFormat2_4<Types>::min_اندازه;
  إذا (unlikely (راس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
  hb_barrier ();
  إذا (unlikely (راس_len < min_اندازه + thiz->range_ریکارډ.get_اندازه () - thiz->range_ریکارډ.len.get_اندازه ()))
    أرجع Err(SANITIZE_FAILURE);
  أرجع Ok();
}

بنية Coverage : عام OT::Layout::Common::Coverage
{
  ساكن ګراف_پایله_t<فراغ> clone_coverage (gsubgpos_ګراف_context_t& c,
                                              بینښه coverage_id,
                                              بینښه new_پلار_id,
                                              بینښه link_position,
                                              بینښه پیل, بینښه پای_ټکی)

  {
    بینښه coverage_اندازه = c.ګراف.vertices_[coverage_id].جدول_اندازه ();
    خپلکاری& coverage_v = c.ګراف.vertices_[coverage_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول))
      أرجع Err(INVALID_دلیل);
    TRY (coverage_جدول->sanitize (coverage_v));

    خپلکاری new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بینښه, بینښه> p) {
          أرجع p.second >= پیل && p.second < پای_ټکی;
        })
        | hb_map_retains_ترتیبونه (hb_لومړی)
        ;

    أرجع جمع_کړه_coverage (c, new_پلار_id, link_position, new_coverage, coverage_اندازه);
  }

  کينډۍ<ډول_نوم It>
  ساكن ګراف_پایله_t<فراغ> جمع_کړه_coverage (gsubgpos_ګراف_context_t& c,
                                            بینښه پلار_id,
                                            بینښه link_position,
                                            It glyphs,
                                            بینښه max_size)
  {
    TRY_ټاکل (بینښه coverage_اولي_id, c.ګراف.new_غوټه (تش_اشاره, تش_اشاره));
    خپلکاری& coverage_اولي_راس = c.ګراف.vertices_[coverage_اولي_id];
    TRY (make_coverage (c, glyphs, coverage_اولي_id, max_size));

    TRY(c.ګراف.vertices_[پلار_id].جمع_کړه_real_link (SmallTypes::اندازه, coverage_اولي_id, link_position));
    TRY(coverage_اولي_راس.جمع_کړه_پلار (پلار_id, خطأ));

    أرجع Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  ساكن ګراف_پایله_t<فراغ> filter_coverage (gsubgpos_ګراف_context_t& c,
                                                بینښه existing_coverage,
                                                بینښه پیل, بینښه پای_ټکی) {
    بینښه coverage_اندازه = c.ګراف.vertices_[existing_coverage].جدول_اندازه ();
    خپلکاری& coverage_v = c.ګراف.vertices_[existing_coverage];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول))
      أرجع Err(INVALID_دلیل);
    TRY (coverage_جدول->sanitize (coverage_v));

    خپلکاری new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بینښه, بینښه> p) {
          أرجع p.second >= پیل && p.second < پای_ټکی;
        })
        | hb_map_retains_ترتیبونه (hb_لومړی)
        ;

    أرجع make_coverage (c, new_coverage, existing_coverage, coverage_اندازه * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  کينډۍ<ډول_نوم It>
  ساكن ګراف_پایله_t<فراغ> make_coverage (gsubgpos_ګراف_context_t& c,
                                             It glyphs,
                                             بینښه dest_obj,
                                             بینښه max_size)
  {
    حرف* بفر = (حرف*) hb_calloc (1, max_size);
    إذا (unlikely (!بفر))
      أرجع Err(ویش_FAILURE);

    hb_serialize_context_t serializer (بفر, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.پای_ټکی_serialize ();
    إذا (unlikely (serializer.دننه_error ()))
    {
      hb_free (بفر);
      أرجع Err(ویش_FAILURE);
    }

    hb_بایټونه_t coverage_copy = serializer.copy_بایټونه ();
    إذا (unlikely (!coverage_copy.arrayZ)) {
      hb_free (بفر);
      أرجع Err(ویش_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خپلکاری res = c.جمع_کړه_بفر ((حرف *) coverage_copy.arrayZ);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      hb_free ((حرف *) coverage_copy.arrayZ);
      أرجع res;
    }

    خپلکاری& v = c.ګراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) coverage_copy.arrayZ;
    v.سېټ_بفر (سر_2, سر_2 + coverage_copy.اوږدتیا);

    hb_free (بفر);
    أرجع Ok();
  }

  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه ();
    إذا (unlikely (راس_len < OT::Layout::Common::Coverage::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: أرجع ګراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) هذا, راس);
    حالة 2: أرجع ګراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) هذا, راس);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: أرجع ګراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) هذا, راس);
    حالة 4: أرجع ګراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) هذا, راس);
#د_که_پای
    افتراضي: أرجع Err(SANITIZE_FAILURE);
    }
  }
};


}

#د_که_پای  // GRAPH_COVERAGE_GRAPH_HH
