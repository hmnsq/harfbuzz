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

#جے_بنا_تعریف گراف_COVERAGE_گراف_HH
#تعریف_کرو گراف_COVERAGE_گراف_HH

نطاق گراف {

سانچا <قسم_ناں Types>
ساكن گراف_نتیجہ_t<فراغ> sanitize (
  ثابت OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  ثابت گراف_t::راس_t& راس
) {
  size_t راس_len = راس.جدول_ناپ();
  constexpr بنانشانی min_ناپ = OT::Layout::Common::CoverageFormat1_3<Types>::min_ناپ;
  إذا (unlikely (راس_len < min_ناپ)) أرجع Err(SANITIZE_FAILURE);
  hb_barrier ();
  إذا (unlikely (راس_len < min_ناپ + thiz->glyphArray.get_ناپ () - thiz->glyphArray.len.get_ناپ ()))
    أرجع Err(SANITIZE_FAILURE);
  أرجع Ok();
}

سانچا <قسم_ناں Types>
ساكن گراف_نتیجہ_t<فراغ> sanitize (
  ثابت OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  ثابت گراف_t::راس_t& راس
) {
  size_t راس_len = راس.جدول_ناپ();
  constexpr بنانشانی min_ناپ = OT::Layout::Common::CoverageFormat2_4<Types>::min_ناپ;
  إذا (unlikely (راس_len < min_ناپ)) أرجع Err(SANITIZE_FAILURE);
  hb_barrier ();
  إذا (unlikely (راس_len < min_ناپ + thiz->range_ریکارڈ.get_ناپ () - thiz->range_ریکارڈ.len.get_ناپ ()))
    أرجع Err(SANITIZE_FAILURE);
  أرجع Ok();
}

بنية Coverage : عام OT::Layout::Common::Coverage
{
  ساكن گراف_نتیجہ_t<فراغ> clone_coverage (gsubgpos_گراف_context_t& c,
                                              بنانشانی coverage_id,
                                              بنانشانی new_ماپا_id,
                                              بنانشانی link_position,
                                              بنانشانی شروع, بنانشانی سرا)

  {
    بنانشانی coverage_ناپ = c.گراف.vertices_[coverage_id].جدول_ناپ ();
    خودکار& coverage_v = c.گراف.vertices_[coverage_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول))
      أرجع Err(INVALID_دلیل);
    TRY (coverage_جدول->sanitize (coverage_v));

    خودکار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بنانشانی, بنانشانی> p) {
          أرجع p.second >= شروع && p.second < سرا;
        })
        | hb_map_retains_چھانٹی (hb_پہلا)
        ;

    أرجع جوڑو_coverage (c, new_ماپا_id, link_position, new_coverage, coverage_ناپ);
  }

  سانچا<قسم_ناں It>
  ساكن گراف_نتیجہ_t<فراغ> جوڑو_coverage (gsubgpos_گراف_context_t& c,
                                            بنانشانی ماپا_id,
                                            بنانشانی link_position,
                                            It glyphs,
                                            بنانشانی max_size)
  {
    TRY_سونپو (بنانشانی coverage_مفرد_id, c.گراف.new_گرہ (خالی_سوچک, خالی_سوچک));
    خودکار& coverage_مفرد_راس = c.گراف.vertices_[coverage_مفرد_id];
    TRY (make_coverage (c, glyphs, coverage_مفرد_id, max_size));

    TRY(c.گراف.vertices_[ماپا_id].جوڑو_real_link (SmallTypes::ناپ, coverage_مفرد_id, link_position));
    TRY(coverage_مفرد_راس.جوڑو_ماپا (ماپا_id, خطأ));

    أرجع Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  ساكن گراف_نتیجہ_t<فراغ> filter_coverage (gsubgpos_گراف_context_t& c,
                                                بنانشانی existing_coverage,
                                                بنانشانی شروع, بنانشانی سرا) {
    بنانشانی coverage_ناپ = c.گراف.vertices_[existing_coverage].جدول_ناپ ();
    خودکار& coverage_v = c.گراف.vertices_[existing_coverage];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    إذا (unlikely (!coverage_جدول))
      أرجع Err(INVALID_دلیل);
    TRY (coverage_جدول->sanitize (coverage_v));

    خودکار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<بنانشانی, بنانشانی> p) {
          أرجع p.second >= شروع && p.second < سرا;
        })
        | hb_map_retains_چھانٹی (hb_پہلا)
        ;

    أرجع make_coverage (c, new_coverage, existing_coverage, coverage_ناپ * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  سانچا<قسم_ناں It>
  ساكن گراف_نتیجہ_t<فراغ> make_coverage (gsubgpos_گراف_context_t& c,
                                             It glyphs,
                                             بنانشانی dest_obj,
                                             بنانشانی max_size)
  {
    حرف* بفر = (حرف*) hb_calloc (1, max_size);
    إذا (unlikely (!بفر))
      أرجع Err(ونڈ_FAILURE);

    hb_serialize_context_t serializer (بفر, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.سرا_serialize ();
    إذا (unlikely (serializer.وچ_error ()))
    {
      hb_free (بفر);
      أرجع Err(ونڈ_FAILURE);
    }

    hb_بائٹاں_t coverage_copy = serializer.copy_بائٹاں ();
    إذا (unlikely (!coverage_copy.arrayZ)) {
      hb_free (بفر);
      أرجع Err(ونڈ_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خودکار res = c.جوڑو_بفر ((حرف *) coverage_copy.arrayZ);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      hb_free ((حرف *) coverage_copy.arrayZ);
      أرجع res;
    }

    خودکار& v = c.گراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) coverage_copy.arrayZ;
    v.سیٹ_بفر (سر_2, سر_2 + coverage_copy.لمائی);

    hb_free (بفر);
    أرجع Ok();
  }

  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ ();
    إذا (unlikely (راس_len < OT::Layout::Common::Coverage::min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) هذا, راس);
    حالة 2: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) هذا, راس);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) هذا, راس);
    حالة 4: أرجع گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) هذا, راس);
#جے_انت
    افتراضي: أرجع Err(SANITIZE_FAILURE);
    }
  }
};


}

#جے_انت  // GRAPH_COVERAGE_GRAPH_HH
