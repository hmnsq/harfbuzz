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

#جيڪڏهن_وصف_نه_ٿيل گراف_COVERAGE_گراف_HH
#وصف_ڪريو گراف_COVERAGE_گراف_HH

نطاق گراف {

سانچو <قسم_نالو Types>
ساكن گراف_نتيجو_t<فراغ> sanitize (
  ثابت OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  ثابت گراف_t::راس_t& راس
) {
  size_t راس_len = راس.جدول_سائيز();
  constexpr بنانشان min_سائيز = OT::Layout::Common::CoverageFormat1_3<Types>::min_سائيز;
  إذا (unlikely (راس_len < min_سائيز)) أرجع Err(SANITIZE_FAILURE);
  hb_barrier ();
  إذا (unlikely (راس_len < min_سائيز + thiz->glyphArray.get_سائيز () - thiz->glyphArray.len.get_سائيز ()))
    أرجع Err(SANITIZE_FAILURE);
  أرجع Ok();
}

سانچو <قسم_نالو Types>
ساكن گراف_نتيجو_t<فراغ> sanitize (
  ثابت OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  ثابت گراف_t::راس_t& راس
) {
  size_t راس_len = راس.جدول_سائيز();
  constexpr بنانشان min_سائيز = OT::Layout::Common::CoverageFormat2_4<Types>::min_سائيز;
  إذا (unlikely (راس_len < min_سائيز)) أرجع Err(SANITIZE_FAILURE);
  hb_barrier ();
  إذا (unlikely (راس_len < min_سائيز + thiz->range_رڪارڊ.get_سائيز () - thiz->range_رڪارڊ.len.get_سائيز ()))
    أرجع Err(SANITIZE_FAILURE);
  أرجع Ok();
}

بنية Coverage : عام OT::Layout::Common::Coverage
{
  ساكن گراف_نتيجو_t<فراغ> clone_coverage (gsubgpos_گراف_context_t& c,
                                              بنانشان coverage_id,
                                              بنانشان new_والد_id,
                                              بنانشان link_position,
                                              بنانشان شروعات, بنانشان پڇاڙي)

  {
    بنانشان coverage_سائيز = c.گراف.vertices_[coverage_id].جدول_سائيز ();
    خودڪار& coverage_v = c.گراف.vertices_[coverage_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول))
      أرجع Err(INVALID_دليل);
    TRY (coverage_جدول->sanitize (coverage_v));

    خودڪار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بنانشان, بنانشان> p) {
          أرجع p.second >= شروعات && p.second < پڇاڙي;
        })
        | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
        ;

    أرجع جوڙيو_coverage (c, new_والد_id, link_position, new_coverage, coverage_سائيز);
  }

  سانچو<قسم_نالو It>
  ساكن گراف_نتيجو_t<فراغ> جوڙيو_coverage (gsubgpos_گراف_context_t& c,
                                            بنانشان والد_id,
                                            بنانشان link_position,
                                            It glyphs,
                                            بنانشان max_size)
  {
    TRY_سونپيو (بنانشان coverage_مفرد_id, c.گراف.new_ڳنڍ (خالي_اشارو, خالي_اشارو));
    خودڪار& coverage_مفرد_راس = c.گراف.vertices_[coverage_مفرد_id];
    TRY (make_coverage (c, glyphs, coverage_مفرد_id, max_size));

    TRY(c.گراف.vertices_[والد_id].جوڙيو_real_link (SmallTypes::سائيز, coverage_مفرد_id, link_position));
    TRY(coverage_مفرد_راس.جوڙيو_والد (والد_id, خطأ));

    أرجع Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  ساكن گراف_نتيجو_t<فراغ> filter_coverage (gsubgpos_گراف_context_t& c,
                                                بنانشان existing_coverage,
                                                بنانشان شروعات, بنانشان پڇاڙي) {
    بنانشان coverage_سائيز = c.گراف.vertices_[existing_coverage].جدول_سائيز ();
    خودڪار& coverage_v = c.گراف.vertices_[existing_coverage];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول))
      أرجع Err(INVALID_دليل);
    TRY (coverage_جدول->sanitize (coverage_v));

    خودڪار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بنانشان, بنانشان> p) {
          أرجع p.second >= شروعات && p.second < پڇاڙي;
        })
        | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
        ;

    أرجع make_coverage (c, new_coverage, existing_coverage, coverage_سائيز * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  سانچو<قسم_نالو It>
  ساكن گراف_نتيجو_t<فراغ> make_coverage (gsubgpos_گراف_context_t& c,
                                             It glyphs,
                                             بنانشان dest_obj,
                                             بنانشان max_size)
  {
    حرف* بفر = (حرف*) hb_calloc (1, max_size);
    إذا (unlikely (!بفر))
      أرجع Err(ورهاست_FAILURE);

    hb_serialize_context_t serializer (بفر, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.پڇاڙي_serialize ();
    إذا (unlikely (serializer.اندريون_error ()))
    {
      hb_free (بفر);
      أرجع Err(ورهاست_FAILURE);
    }

    hb_بائيٽون_t coverage_copy = serializer.copy_بائيٽون ();
    إذا (unlikely (!coverage_copy.arrayZ)) {
      hb_free (بفر);
      أرجع Err(ورهاست_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خودڪار res = c.جوڙيو_بفر ((حرف *) coverage_copy.arrayZ);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      hb_free ((حرف *) coverage_copy.arrayZ);
      أرجع res;
    }

    خودڪار& v = c.گراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) coverage_copy.arrayZ;
    v.سيٽ_بفر (سر_2, سر_2 + coverage_copy.ڊيگهائي);

    hb_free (بفر);
    أرجع Ok();
  }

  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز ();
    إذا (unlikely (راس_len < OT::Layout::Common::Coverage::min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) هذا, راس);
    حالة 2: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) هذا, راس);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) هذا, راس);
    حالة 4: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) هذا, راس);
#جيڪڏهن_پورو
    افتراضي: أرجع Err(SANITIZE_FAILURE);
    }
  }
};


}

#جيڪڏهن_پورو  // GRAPH_COVERAGE_GRAPH_HH
