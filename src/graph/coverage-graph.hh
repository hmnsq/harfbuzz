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

#اگر_تعریف_نشده گراف_COVERAGE_گراف_HH
#تعریف_کن گراف_COVERAGE_گراف_HH

نطاق گراف {

قالب <نام_نوع Types>
ساكن گراف_نتیجه_t<فراغ> sanitize (
  ثابت OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  ثابت گراف_t::رأس_t& رأس
) {
  size_t رأس_len = رأس.جدول_اندازه();
  constexpr بیعلامت min_اندازه = OT::Layout::Common::CoverageFormat1_3<Types>::min_اندازه;
  إذا (unlikely (رأس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
  hb_barrier ();
  إذا (unlikely (رأس_len < min_اندازه + thiz->glyphArray.get_اندازه () - thiz->glyphArray.len.get_اندازه ()))
    أرجع Err(SANITIZE_FAILURE);
  أرجع Ok();
}

قالب <نام_نوع Types>
ساكن گراف_نتیجه_t<فراغ> sanitize (
  ثابت OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  ثابت گراف_t::رأس_t& رأس
) {
  size_t رأس_len = رأس.جدول_اندازه();
  constexpr بیعلامت min_اندازه = OT::Layout::Common::CoverageFormat2_4<Types>::min_اندازه;
  إذا (unlikely (رأس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
  hb_barrier ();
  إذا (unlikely (رأس_len < min_اندازه + thiz->range_رکورد.get_اندازه () - thiz->range_رکورد.len.get_اندازه ()))
    أرجع Err(SANITIZE_FAILURE);
  أرجع Ok();
}

بنية Coverage : عام OT::Layout::Common::Coverage
{
  ساكن گراف_نتیجه_t<فراغ> clone_coverage (gsubgpos_گراف_context_t& c,
                                              بیعلامت coverage_id,
                                              بیعلامت new_والد_id,
                                              بیعلامت link_position,
                                              بیعلامت شروع, بیعلامت انتها)

  {
    بیعلامت coverage_اندازه = c.گراف.vertices_[coverage_id].جدول_اندازه ();
    خودکار& coverage_v = c.گراف.vertices_[coverage_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول))
      أرجع Err(INVALID_آرگومان);
    TRY (coverage_جدول->sanitize (coverage_v));

    خودکار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بیعلامت, بیعلامت> p) {
          أرجع p.second >= شروع && p.second < انتها;
        })
        | hb_map_retains_ترتیبدهی (hb_اولی)
        ;

    أرجع جمع_کن_coverage (c, new_والد_id, link_position, new_coverage, coverage_اندازه);
  }

  قالب<نام_نوع It>
  ساكن گراف_نتیجه_t<فراغ> جمع_کن_coverage (gsubgpos_گراف_context_t& c,
                                            بیعلامت والد_id,
                                            بیعلامت link_position,
                                            It glyphs,
                                            بیعلامت max_size)
  {
    TRY_تخصیص_بده (بیعلامت coverage_اول_id, c.گراف.new_گره (اشاره_تهی, اشاره_تهی));
    خودکار& coverage_اول_رأس = c.گراف.vertices_[coverage_اول_id];
    TRY (make_coverage (c, glyphs, coverage_اول_id, max_size));

    TRY(c.گراف.vertices_[والد_id].جمع_کن_real_link (SmallTypes::اندازه, coverage_اول_id, link_position));
    TRY(coverage_اول_رأس.جمع_کن_والد (والد_id, خطأ));

    أرجع Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  ساكن گراف_نتیجه_t<فراغ> filter_coverage (gsubgpos_گراف_context_t& c,
                                                بیعلامت existing_coverage,
                                                بیعلامت شروع, بیعلامت انتها) {
    بیعلامت coverage_اندازه = c.گراف.vertices_[existing_coverage].جدول_اندازه ();
    خودکار& coverage_v = c.گراف.vertices_[existing_coverage];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول))
      أرجع Err(INVALID_آرگومان);
    TRY (coverage_جدول->sanitize (coverage_v));

    خودکار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بیعلامت, بیعلامت> p) {
          أرجع p.second >= شروع && p.second < انتها;
        })
        | hb_map_retains_ترتیبدهی (hb_اولی)
        ;

    أرجع make_coverage (c, new_coverage, existing_coverage, coverage_اندازه * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  قالب<نام_نوع It>
  ساكن گراف_نتیجه_t<فراغ> make_coverage (gsubgpos_گراف_context_t& c,
                                             It glyphs,
                                             بیعلامت dest_obj,
                                             بیعلامت max_size)
  {
    حرف* میانگیر = (حرف*) hb_calloc (1, max_size);
    إذا (unlikely (!میانگیر))
      أرجع Err(تخصیص_FAILURE);

    hb_serialize_context_t serializer (میانگیر, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.انتها_serialize ();
    إذا (unlikely (serializer.درون_error ()))
    {
      hb_free (میانگیر);
      أرجع Err(تخصیص_FAILURE);
    }

    hb_بایتها_t coverage_copy = serializer.copy_بایتها ();
    إذا (unlikely (!coverage_copy.arrayZ)) {
      hb_free (میانگیر);
      أرجع Err(تخصیص_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خودکار res = c.جمع_کن_میانگیر ((حرف *) coverage_copy.arrayZ);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (میانگیر);
      hb_free ((حرف *) coverage_copy.arrayZ);
      أرجع res;
    }

    خودکار& v = c.گراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) coverage_copy.arrayZ;
    v.مجموعه_میانگیر (سر_2, سر_2 + coverage_copy.درازی);

    hb_free (میانگیر);
    أرجع Ok();
  }

  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه ();
    إذا (unlikely (رأس_len < OT::Layout::Common::Coverage::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) هذا, رأس);
    حالة 2: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) هذا, رأس);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) هذا, رأس);
    حالة 4: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) هذا, رأس);
#پایان_اگر
    افتراضي: أرجع Err(SANITIZE_FAILURE);
    }
  }
};


}

#پایان_اگر  // GRAPH_COVERAGE_GRAPH_HH
