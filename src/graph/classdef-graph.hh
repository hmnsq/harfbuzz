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

#که_نه_تعریف_شوی ګراف_CLASSDEF_ګراف_HH
#تعریف_کړه ګراف_CLASSDEF_ګراف_HH

نطاق ګراف {

کينډۍ <ډول_نوم Types>
بنية ClassDefFormat1_3 : عام OT::ClassDefFormat1_3<Types>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه();
    constexpr بینښه min_اندازه = OT::ClassDefFormat1_3<Types>::min_اندازه;
    إذا (unlikely (راس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < min_اندازه + هذا->class_ارزښت.get_اندازه () - هذا->class_ارزښت.len.get_اندازه ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

کينډۍ <ډول_نوم Types>
بنية ClassDefFormat2_4 : عام OT::ClassDefFormat2_4<Types>
{
  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه();
    constexpr بینښه min_اندازه = OT::ClassDefFormat2_4<Types>::min_اندازه;
    إذا (unlikely (راس_len < min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < min_اندازه + هذا->range_ریکارډ.get_اندازه () - هذا->range_ریکارډ.len.get_اندازه ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

بنية ClassDef : عام OT::ClassDef
{
  کينډۍ<ډول_نوم It>
  ساكن ګراف_پایله_t<فراغ> جمع_کړه_class_def (gsubgpos_ګراف_context_t& c,
                                             بینښه پلار_id,
                                             بینښه link_position,
                                             It glyph_and_class,
                                             بینښه max_size)
  {
    TRY_ټاکل (بینښه class_def_اولي_id, c.ګراف.new_غوټه (تش_اشاره, تش_اشاره));
    خپلکاری& class_def_اولي_راس = c.ګراف.vertices_[class_def_اولي_id];
    TRY (make_class_def (c, glyph_and_class, class_def_اولي_id, max_size));

    TRY(c.ګراف.vertices_[پلار_id].جمع_کړه_real_link (SmallTypes::اندازه, class_def_اولي_id, link_position));
    TRY(class_def_اولي_راس.جمع_کړه_پلار (پلار_id, خطأ));

    أرجع Ok();
  }

  کينډۍ<ډول_نوم It>
  ساكن ګراف_پایله_t<فراغ> make_class_def (gsubgpos_ګراف_context_t& c,
                                              It glyph_and_class,
                                              بینښه dest_obj,
                                              بینښه max_size)
  {
    حرف* بفر = (حرف*) hb_calloc (1, max_size);
    إذا (unlikely (!بفر))
      أرجع Err(ویش_FAILURE);

    hb_serialize_context_t serializer (بفر, max_size);
    OT::ClassDef_serialize (&serializer, glyph_and_class);
    serializer.پای_ټکی_serialize ();
    إذا (unlikely (serializer.دننه_error ()))
    {
      hb_free (بفر);
      أرجع Err(ویش_FAILURE);
    }

    hb_بایټونه_t class_def_copy = serializer.copy_بایټونه ();
    إذا (unlikely (!class_def_copy.arrayZ))
    {
      hb_free (بفر);
      أرجع Err(ویش_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خپلکاری res = c.جمع_کړه_بفر ((حرف *) class_def_copy.arrayZ);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      hb_free ((حرف *) class_def_copy.arrayZ);
      أرجع res;
    }


    خپلکاری& v = c.ګراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) class_def_copy.arrayZ;
    v.سېټ_بفر (سر_2, سر_2 + class_def_copy.اوږدتیا);

    hb_free (بفر);
    أرجع Ok();
  }

  ګراف_پایله_t<فراغ> sanitize (ثابت ګراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_اندازه();
    إذا (unlikely (راس_len < OT::ClassDef::min_اندازه)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: أرجع ((ClassDefFormat1_3<SmallTypes>*)هذا)->sanitize (راس);
    حالة 2: أرجع ((ClassDefFormat2_4<SmallTypes>*)هذا)->sanitize (راس);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: أرجع ((ClassDefFormat1_3<MediumTypes>*)هذا)->sanitize (راس);
    حالة 4: أرجع ((ClassDefFormat2_4<MediumTypes>*)هذا)->sanitize (راس);
#د_که_پای
    افتراضي: أرجع Err(SANITIZE_FAILURE);
    }
  }
};


بنية class_def_اندازه_estimator_t
{
  کينډۍ<ډول_نوم It>
  ساكن ګراف_پایله_t<class_def_اندازه_estimator_t> create (It glyph_and_class)
  {
    class_def_اندازه_estimator_t estimator;
    estimator.reset();
    لكل (خپلکاری p : + glyph_and_class)
    {
      بینښه gid = p.لومړی;
      بینښه klass = p.second;

      hb_سېټ_t* glyphs;
      إذا (estimator.glyphs_فی_class.has (klass, &glyphs) && glyphs) {
        glyphs->جمع_کړه (gid);
        استمر;
      }

      hb_سېټ_t new_glyphs;
      new_glyphs.جمع_کړه (gid);
      estimator.glyphs_فی_class.سېټ (klass, std::move (new_glyphs));
    }

    TRY(estimator.منزل_پایله ());

    لكل (بینښه klass : estimator.glyphs_فی_class.کلیګانې ())
    {
      إذا (!klass) استمر; // class 0 doesn't get encoded.

      ثابت hb_سېټ_t& glyphs = estimator.glyphs_فی_class.get (klass);
      hb_codepoint_t پیل = HB_سېټ_ارزښت_INVALID;
      hb_codepoint_t پای_ټکی = HB_سېټ_ارزښت_INVALID;

      بینښه شمېر = 0;
      طالما (glyphs.بل_range (&پیل, &پای_ټکی))
        شمېر++;

      estimator.عدد_ranges_فی_class.سېټ (klass, شمېر);
    }

    TRY(estimator.منزل_پایله ());
    أرجع estimator;
  }

  فراغ reset() {
    included_glyphs.clear();
    included_class_def_glyphs.clear();
    included_classes.clear();
    class_def_range_شمېر = 0;
    class_max = 0;
    coverage_اندازه_val = compute_coverage_اندازه ();
  }

  // Size of coverage for all glyphs added via 'add_class_def_size'.
  بینښه coverage_اندازه () ثابت
  {
    أرجع coverage_اندازه_val;
  }

  // Compute the new size of the ClassDef table if all glyphs associated with 'klass' were added.
  بینښه جمع_کړه_class_def_اندازه (بینښه klass)
  {
    إذا (!included_classes.has(klass)) {
      hb_سېټ_t* glyphs = تش_اشاره;
      إذا (glyphs_فی_class.has(klass, &glyphs)) {
        بینښه عدد_glyphs = included_glyphs.get_population();
        included_glyphs.union_(*glyphs);
        إذا (klass)
          included_class_def_glyphs.union_(*glyphs);
        إذا (عدد_glyphs != included_glyphs.get_population())
          coverage_اندازه_val = compute_coverage_اندازه ();
      }

      إذا (klass)
      {
        class_def_range_شمېر += عدد_ranges_فی_class.get (klass);
        إذا (glyphs && klass > class_max)
          class_max = klass;
      }
      included_classes.جمع_کړه(klass);
    }

    أرجع class_def_اندازه ();
  }

  بینښه عدد_glyph_ranges (ثابت hb_سېټ_t &glyphs) ثابت {
    hb_codepoint_t پیل = HB_سېټ_ارزښت_INVALID;
    hb_codepoint_t پای_ټکی = HB_سېټ_ارزښت_INVALID;

    بینښه شمېر = 0;
    طالما (glyphs.بل_range (&پیل, &پای_ټکی)) {
        شمېر++;
    }
    أرجع شمېر;
  }

 خاص:
  ګراف_پایله_t<فراغ> منزل_پایله() ثابت
  {
    TRY (ګراف_پایله_t<فراغ>::from (عدد_ranges_فی_class, ویش_FAILURE));
    TRY (ګراف_پایله_t<فراغ>::from (glyphs_فی_class, ویش_FAILURE));
    TRY (ګراف_پایله_t<فراغ>::from (included_classes, ویش_FAILURE));
    TRY (ګراف_پایله_t<فراغ>::from (included_glyphs, ویش_FAILURE));
    TRY (ګراف_پایله_t<فراغ>::from (included_class_def_glyphs, ویش_FAILURE));

    لكل (ثابت hb_سېټ_t& s : glyphs_فی_class.values ())
    {
          TRY (ګراف_پایله_t<فراغ>::from (s, ویش_FAILURE));
    }
    أرجع Ok();
  }

  class_def_اندازه_estimator_t ()
      : عدد_ranges_فی_class (), glyphs_فی_class () {}

  بینښه compute_coverage_اندازه () ثابت
  {
    بینښه glyph_اندازه = glyph_id_اندازه (included_glyphs);
    بینښه coverage_بنسټ_اندازه = OT::HBUINT16::static_اندازه + glyph_اندازه;
    بینښه range_اندازه = 3 * glyph_اندازه;
    بینښه format1_اندازه = coverage_بنسټ_اندازه + glyph_اندازه * included_glyphs.get_population();
    بینښه format2_اندازه = coverage_بنسټ_اندازه + range_اندازه * عدد_glyph_ranges (included_glyphs);
    أرجع hb_min(format1_اندازه, format2_اندازه);
  }

  بینښه glyph_id_اندازه (ثابت hb_سېټ_t &glyphs) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (!glyphs.is_empty () && glyphs.get_max () > 0xFFFFu)
      أرجع OT::Layout::MediumTypes::اندازه;
#د_که_پای
    أرجع OT::Layout::SmallTypes::اندازه;
  }

  بینښه class_def_اندازه () ثابت
  {
    إذا (included_class_def_glyphs.is_empty ())
      أرجع OT::HBUINT16::static_اندازه + OT::Layout::SmallTypes::اندازه;

    بینښه غوره_اندازه = UINT_MAX;
    hb_codepoint_t glyph_max = included_class_def_glyphs.get_max ();
    بینښه glyph_پراختیا = glyph_max - included_class_def_glyphs.get_min () + 1;

    إذا (glyph_max <= 0xFFFFu && glyph_پراختیا <= 0xFFFFu && class_max <= 0xFFFFu)
      غوره_اندازه = hb_min (غوره_اندازه, 6 + 2 * glyph_پراختیا);
    إذا (glyph_max <= 0xFFFFu && class_def_range_شمېر <= 0xFFFFu && class_max <= 0xFFFFu)
      غوره_اندازه = hb_min (غوره_اندازه, 4 + 6 * class_def_range_شمېر);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (glyph_max <= 0xFFFFFFu && glyph_پراختیا <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
      غوره_اندازه = hb_min (غوره_اندازه, 8 + 3 * glyph_پراختیا);
    إذا (glyph_max <= 0xFFFFFFu && class_def_range_شمېر <= 0xFFFFFFu && class_max <= 0xFFFFu)
      غوره_اندازه = hb_min (غوره_اندازه, 5 + 8 * class_def_range_شمېر);
#د_که_پای
    أرجع غوره_اندازه;
  }

  hb_hashmap_t<بینښه, بینښه> عدد_ranges_فی_class;
  hb_hashmap_t<بینښه, hb_سېټ_t> glyphs_فی_class;
  hb_سېټ_t included_classes;
  hb_سېټ_t included_glyphs;
  hb_سېټ_t included_class_def_glyphs;
  بینښه class_def_range_شمېر;
  بینښه class_max;
  بینښه coverage_اندازه_val;
};


}

#د_که_پای  // GRAPH_CLASSDEF_GRAPH_HH
