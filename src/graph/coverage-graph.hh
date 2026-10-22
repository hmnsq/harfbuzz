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

#شامل "graph.hh"
#شامل "../OT/Layout/Common/Coverage.hh"

#اگر_غیر_تعریف_شدہ گراف_COVERAGE_گراف_HH
#تعریف_کرو گراف_COVERAGE_گراف_HH

نامخلا گراف {

سانچہ <قسم_نام Types>
ساکن گراف_نتیجہ_t<خالی> sanitize (
  ثابت OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  ثابت گراف_t::راس_t& راس
) {
  size_t راس_len = راس.جدول_سائز();
  constexpr غیرعلامتی min_سائز = OT::Layout::Common::CoverageFormat1_3<Types>::min_سائز;
  اگر (unlikely (راس_len < min_سائز)) واپس Err(SANITIZE_FAILURE);
  hb_barrier ();
  اگر (unlikely (راس_len < min_سائز + thiz->glyphArray.get_سائز () - thiz->glyphArray.len.get_سائز ()))
    واپس Err(SANITIZE_FAILURE);
  واپس Ok();
}

سانچہ <قسم_نام Types>
ساکن گراف_نتیجہ_t<خالی> sanitize (
  ثابت OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  ثابت گراف_t::راس_t& راس
) {
  size_t راس_len = راس.جدول_سائز();
  constexpr غیرعلامتی min_سائز = OT::Layout::Common::CoverageFormat2_4<Types>::min_سائز;
  اگر (unlikely (راس_len < min_سائز)) واپس Err(SANITIZE_FAILURE);
  hb_barrier ();
  اگر (unlikely (راس_len < min_سائز + thiz->range_ریکارڈ.get_سائز () - thiz->range_ریکارڈ.len.get_سائز ()))
    واپس Err(SANITIZE_FAILURE);
  واپس Ok();
}

ساخت Coverage : عوامی OT::Layout::Common::Coverage
{
  ساکن گراف_نتیجہ_t<خالی> clone_coverage (gsubgpos_گراف_context_t& c,
                                              غیرعلامتی coverage_id,
                                              غیرعلامتی new_والد_id,
                                              غیرعلامتی link_position,
                                              غیرعلامتی آغاز, غیرعلامتی انتہا)

  {
    غیرعلامتی coverage_سائز = c.گراف.vertices_[coverage_id].جدول_سائز ();
    خودکار& coverage_v = c.گراف.vertices_[coverage_id];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    اگر (unlikely (!coverage_جدول))
      واپس Err(INVALID_دلیل);
    TRY (coverage_جدول->sanitize (coverage_v));

    خودکار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<غیرعلامتی, غیرعلامتی> p) {
          واپس p.second >= آغاز && p.second < انتہا;
        })
        | hb_map_retains_ترتیب_کاری (hb_پہلا)
        ;

    واپس جوڑو_coverage (c, new_والد_id, link_position, new_coverage, coverage_سائز);
  }

  سانچہ<قسم_نام It>
  ساکن گراف_نتیجہ_t<خالی> جوڑو_coverage (gsubgpos_گراف_context_t& c,
                                            غیرعلامتی والد_id,
                                            غیرعلامتی link_position,
                                            It glyphs,
                                            غیرعلامتی max_size)
  {
    TRY_تفویض (غیرعلامتی coverage_مفرد_id, c.گراف.new_گرہ (خالی_اشاریہ, خالی_اشاریہ));
    خودکار& coverage_مفرد_راس = c.گراف.vertices_[coverage_مفرد_id];
    TRY (make_coverage (c, glyphs, coverage_مفرد_id, max_size));

    TRY(c.گراف.vertices_[والد_id].جوڑو_real_link (SmallTypes::سائز, coverage_مفرد_id, link_position));
    TRY(coverage_مفرد_راس.جوڑو_والد (والد_id, جھوٹ));

    واپس Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  ساکن گراف_نتیجہ_t<خالی> filter_coverage (gsubgpos_گراف_context_t& c,
                                                غیرعلامتی existing_coverage,
                                                غیرعلامتی آغاز, غیرعلامتی انتہا) {
    غیرعلامتی coverage_سائز = c.گراف.vertices_[existing_coverage].جدول_سائز ();
    خودکار& coverage_v = c.گراف.vertices_[existing_coverage];
    ثابت Coverage* coverage_جدول = (ثابت Coverage*) coverage_v.obj().سر_2;
    اگر (unlikely (!coverage_جدول))
      واپس Err(INVALID_دلیل);
    TRY (coverage_جدول->sanitize (coverage_v));

    خودکار new_coverage =
        + hb_zip (coverage_جدول->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<غیرعلامتی, غیرعلامتی> p) {
          واپس p.second >= آغاز && p.second < انتہا;
        })
        | hb_map_retains_ترتیب_کاری (hb_پہلا)
        ;

    واپس make_coverage (c, new_coverage, existing_coverage, coverage_سائز * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  سانچہ<قسم_نام It>
  ساکن گراف_نتیجہ_t<خالی> make_coverage (gsubgpos_گراف_context_t& c,
                                             It glyphs,
                                             غیرعلامتی dest_obj,
                                             غیرعلامتی max_size)
  {
    حرف* بفر = (حرف*) hb_calloc (1, max_size);
    اگر (unlikely (!بفر))
      واپس Err(تخصیص_FAILURE);

    hb_serialize_context_t serializer (بفر, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.انتہا_serialize ();
    اگر (unlikely (serializer.اندر_error ()))
    {
      hb_free (بفر);
      واپس Err(تخصیص_FAILURE);
    }

    hb_بائٹیں_t coverage_copy = serializer.copy_بائٹیں ();
    اگر (unlikely (!coverage_copy.arrayZ)) {
      hb_free (بفر);
      واپس Err(تخصیص_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خودکار res = c.جوڑو_بفر ((حرف *) coverage_copy.arrayZ);
    اگر (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      hb_free ((حرف *) coverage_copy.arrayZ);
      واپس res;
    }

    خودکار& v = c.گراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) coverage_copy.arrayZ;
    v.سیٹ_بفر (سر_2, سر_2 + coverage_copy.لمبائی);

    hb_free (بفر);
    واپس Ok();
  }

  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز ();
    اگر (unlikely (راس_len < OT::Layout::Common::Coverage::min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();
    انتخاب (u.format.v)
    {
    حالت 1: واپس گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) یہ, راس);
    حالت 2: واپس گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) یہ, راس);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: واپس گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) یہ, راس);
    حالت 4: واپس گراف::sanitize ((ثابت OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) یہ, راس);
#اگر_ختم
    طےشدہ: واپس Err(SANITIZE_FAILURE);
    }
  }
};


}

#اگر_ختم  // GRAPH_COVERAGE_GRAPH_HH
