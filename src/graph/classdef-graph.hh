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
#ضمن "../hb-ot-layout-common.hh"
#ضمن "graph-result.hh"

#اگر_تعریف_نشده گراف_CLASSDEF_گراف_HH
#تعریف_کن گراف_CLASSDEF_گراف_HH

نطاق گراف {

قالب <نام_نوع Types>
بنية ClassDefFormat1_3 : عام OT::ClassDefFormat1_3<Types>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه();
    constexpr بیعلامت min_اندازه = OT::ClassDefFormat1_3<Types>::min_اندازه;
    إذا (unlikely (رأس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (رأس_len < min_اندازه + هذا->class_مقدار.get_اندازه () - هذا->class_مقدار.len.get_اندازه ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

قالب <نام_نوع Types>
بنية ClassDefFormat2_4 : عام OT::ClassDefFormat2_4<Types>
{
  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه();
    constexpr بیعلامت min_اندازه = OT::ClassDefFormat2_4<Types>::min_اندازه;
    إذا (unlikely (رأس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (رأس_len < min_اندازه + هذا->range_رکورد.get_اندازه () - هذا->range_رکورد.len.get_اندازه ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

بنية ClassDef : عام OT::ClassDef
{
  قالب<نام_نوع It>
  ساكن گراف_نتیجه_t<فراغ> جمع_کن_class_def (gsubgpos_گراف_context_t& c,
                                             بیعلامت والد_id,
                                             بیعلامت link_position,
                                             It glyph_and_class,
                                             بیعلامت max_size)
  {
    TRY_تخصیص_بده (بیعلامت class_def_اول_id, c.گراف.new_گره (اشاره_تهی, اشاره_تهی));
    خودکار& class_def_اول_رأس = c.گراف.vertices_[class_def_اول_id];
    TRY (make_class_def (c, glyph_and_class, class_def_اول_id, max_size));

    TRY(c.گراف.vertices_[والد_id].جمع_کن_real_link (SmallTypes::اندازه, class_def_اول_id, link_position));
    TRY(class_def_اول_رأس.جمع_کن_والد (والد_id, خطأ));

    أرجع Ok();
  }

  قالب<نام_نوع It>
  ساكن گراف_نتیجه_t<فراغ> make_class_def (gsubgpos_گراف_context_t& c,
                                              It glyph_and_class,
                                              بیعلامت dest_obj,
                                              بیعلامت max_size)
  {
    حرف* میانگیر = (حرف*) hb_calloc (1, max_size);
    إذا (unlikely (!میانگیر))
      أرجع Err(تخصیص_FAILURE);

    hb_serialize_context_t serializer (میانگیر, max_size);
    OT::ClassDef_serialize (&serializer, glyph_and_class);
    serializer.انتها_serialize ();
    إذا (unlikely (serializer.درون_error ()))
    {
      hb_free (میانگیر);
      أرجع Err(تخصیص_FAILURE);
    }

    hb_بایتها_t class_def_copy = serializer.copy_بایتها ();
    إذا (unlikely (!class_def_copy.arrayZ))
    {
      hb_free (میانگیر);
      أرجع Err(تخصیص_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خودکار res = c.جمع_کن_میانگیر ((حرف *) class_def_copy.arrayZ);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (میانگیر);
      hb_free ((حرف *) class_def_copy.arrayZ);
      أرجع res;
    }


    خودکار& v = c.گراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) class_def_copy.arrayZ;
    v.مجموعه_میانگیر (سر_2, سر_2 + class_def_copy.درازی);

    hb_free (میانگیر);
    أرجع Ok();
  }

  گراف_نتیجه_t<فراغ> sanitize (ثابت گراف_t::رأس_t& رأس) ثابت
  {
    size_t رأس_len = رأس.جدول_اندازه();
    إذا (unlikely (رأس_len < OT::ClassDef::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: أرجع ((ClassDefFormat1_3<SmallTypes>*)هذا)->sanitize (رأس);
    حالة 2: أرجع ((ClassDefFormat2_4<SmallTypes>*)هذا)->sanitize (رأس);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: أرجع ((ClassDefFormat1_3<MediumTypes>*)هذا)->sanitize (رأس);
    حالة 4: أرجع ((ClassDefFormat2_4<MediumTypes>*)هذا)->sanitize (رأس);
#پایان_اگر
    افتراضي: أرجع Err(SANITIZE_FAILURE);
    }
  }
};


بنية class_def_اندازه_estimator_t
{
  قالب<نام_نوع It>
  ساكن گراف_نتیجه_t<class_def_اندازه_estimator_t> create (It glyph_and_class)
  {
    class_def_اندازه_estimator_t estimator;
    estimator.reset();
    لكل (خودکار p : + glyph_and_class)
    {
      بیعلامت gid = p.اولی;
      بیعلامت klass = p.second;

      hb_مجموعه_t* glyphs;
      إذا (estimator.glyphs_در_هر_class.has (klass, &glyphs) && glyphs) {
        glyphs->جمع_کن (gid);
        استمر;
      }

      hb_مجموعه_t new_glyphs;
      new_glyphs.جمع_کن (gid);
      estimator.glyphs_در_هر_class.مجموعه (klass, std::move (new_glyphs));
    }

    TRY(estimator.مقصد_نتیجه ());

    لكل (بیعلامت klass : estimator.glyphs_در_هر_class.کلیدها ())
    {
      إذا (!klass) استمر; // class 0 doesn't get encoded.

      ثابت hb_مجموعه_t& glyphs = estimator.glyphs_در_هر_class.get (klass);
      hb_codepoint_t شروع = HB_مجموعه_مقدار_INVALID;
      hb_codepoint_t انتها = HB_مجموعه_مقدار_INVALID;

      بیعلامت شمار = 0;
      طالما (glyphs.بعدی_range (&شروع, &انتها))
        شمار++;

      estimator.رقم_ranges_در_هر_class.مجموعه (klass, شمار);
    }

    TRY(estimator.مقصد_نتیجه ());
    أرجع estimator;
  }

  فراغ reset() {
    included_glyphs.clear();
    included_class_def_glyphs.clear();
    included_classes.clear();
    class_def_range_شمار = 0;
    class_max = 0;
    coverage_اندازه_val = compute_coverage_اندازه ();
  }

  // Size of coverage for all glyphs added via 'add_class_def_size'.
  بیعلامت coverage_اندازه () ثابت
  {
    أرجع coverage_اندازه_val;
  }

  // Compute the new size of the ClassDef table if all glyphs associated with 'klass' were added.
  بیعلامت جمع_کن_class_def_اندازه (بیعلامت klass)
  {
    إذا (!included_classes.has(klass)) {
      hb_مجموعه_t* glyphs = اشاره_تهی;
      إذا (glyphs_در_هر_class.has(klass, &glyphs)) {
        بیعلامت رقم_glyphs = included_glyphs.get_population();
        included_glyphs.union_(*glyphs);
        إذا (klass)
          included_class_def_glyphs.union_(*glyphs);
        إذا (رقم_glyphs != included_glyphs.get_population())
          coverage_اندازه_val = compute_coverage_اندازه ();
      }

      إذا (klass)
      {
        class_def_range_شمار += رقم_ranges_در_هر_class.get (klass);
        إذا (glyphs && klass > class_max)
          class_max = klass;
      }
      included_classes.جمع_کن(klass);
    }

    أرجع class_def_اندازه ();
  }

  بیعلامت رقم_glyph_ranges (ثابت hb_مجموعه_t &glyphs) ثابت {
    hb_codepoint_t شروع = HB_مجموعه_مقدار_INVALID;
    hb_codepoint_t انتها = HB_مجموعه_مقدار_INVALID;

    بیعلامت شمار = 0;
    طالما (glyphs.بعدی_range (&شروع, &انتها)) {
        شمار++;
    }
    أرجع شمار;
  }

 خاص:
  گراف_نتیجه_t<فراغ> مقصد_نتیجه() ثابت
  {
    TRY (گراف_نتیجه_t<فراغ>::from (رقم_ranges_در_هر_class, تخصیص_FAILURE));
    TRY (گراف_نتیجه_t<فراغ>::from (glyphs_در_هر_class, تخصیص_FAILURE));
    TRY (گراف_نتیجه_t<فراغ>::from (included_classes, تخصیص_FAILURE));
    TRY (گراف_نتیجه_t<فراغ>::from (included_glyphs, تخصیص_FAILURE));
    TRY (گراف_نتیجه_t<فراغ>::from (included_class_def_glyphs, تخصیص_FAILURE));

    لكل (ثابت hb_مجموعه_t& s : glyphs_در_هر_class.values ())
    {
          TRY (گراف_نتیجه_t<فراغ>::from (s, تخصیص_FAILURE));
    }
    أرجع Ok();
  }

  class_def_اندازه_estimator_t ()
      : رقم_ranges_در_هر_class (), glyphs_در_هر_class () {}

  بیعلامت compute_coverage_اندازه () ثابت
  {
    بیعلامت glyph_اندازه = glyph_id_اندازه (included_glyphs);
    بیعلامت coverage_پایه_اندازه = OT::HBUINT16::static_اندازه + glyph_اندازه;
    بیعلامت range_اندازه = 3 * glyph_اندازه;
    بیعلامت format1_اندازه = coverage_پایه_اندازه + glyph_اندازه * included_glyphs.get_population();
    بیعلامت format2_اندازه = coverage_پایه_اندازه + range_اندازه * رقم_glyph_ranges (included_glyphs);
    أرجع hb_min(format1_اندازه, format2_اندازه);
  }

  بیعلامت glyph_id_اندازه (ثابت hb_مجموعه_t &glyphs) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (!glyphs.is_empty () && glyphs.get_max () > 0xFFFFu)
      أرجع OT::Layout::MediumTypes::اندازه;
#پایان_اگر
    أرجع OT::Layout::SmallTypes::اندازه;
  }

  بیعلامت class_def_اندازه () ثابت
  {
    إذا (included_class_def_glyphs.is_empty ())
      أرجع OT::HBUINT16::static_اندازه + OT::Layout::SmallTypes::اندازه;

    بیعلامت بهترین_اندازه = UINT_MAX;
    hb_codepoint_t glyph_max = included_class_def_glyphs.get_max ();
    بیعلامت glyph_بازه = glyph_max - included_class_def_glyphs.get_min () + 1;

    إذا (glyph_max <= 0xFFFFu && glyph_بازه <= 0xFFFFu && class_max <= 0xFFFFu)
      بهترین_اندازه = hb_min (بهترین_اندازه, 6 + 2 * glyph_بازه);
    إذا (glyph_max <= 0xFFFFu && class_def_range_شمار <= 0xFFFFu && class_max <= 0xFFFFu)
      بهترین_اندازه = hb_min (بهترین_اندازه, 4 + 6 * class_def_range_شمار);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (glyph_max <= 0xFFFFFFu && glyph_بازه <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
      بهترین_اندازه = hb_min (بهترین_اندازه, 8 + 3 * glyph_بازه);
    إذا (glyph_max <= 0xFFFFFFu && class_def_range_شمار <= 0xFFFFFFu && class_max <= 0xFFFFu)
      بهترین_اندازه = hb_min (بهترین_اندازه, 5 + 8 * class_def_range_شمار);
#پایان_اگر
    أرجع بهترین_اندازه;
  }

  hb_hashmap_t<بیعلامت, بیعلامت> رقم_ranges_در_هر_class;
  hb_hashmap_t<بیعلامت, hb_مجموعه_t> glyphs_در_هر_class;
  hb_مجموعه_t included_classes;
  hb_مجموعه_t included_glyphs;
  hb_مجموعه_t included_class_def_glyphs;
  بیعلامت class_def_range_شمار;
  بیعلامت class_max;
  بیعلامت coverage_اندازه_val;
};


}

#پایان_اگر  // GRAPH_CLASSDEF_GRAPH_HH
