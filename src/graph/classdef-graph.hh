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

#جے_بنا_تعریف گراف_CLASSDEF_گراف_HH
#تعریف_کرو گراف_CLASSDEF_گراف_HH

نطاق گراف {

سانچا <قسم_ناں Types>
بنية ClassDefFormat1_3 : عام OT::ClassDefFormat1_3<Types>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ();
    constexpr بنانشانی min_ناپ = OT::ClassDefFormat1_3<Types>::min_ناپ;
    إذا (unlikely (راس_len < min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < min_ناپ + هذا->class_قدر.get_ناپ () - هذا->class_قدر.len.get_ناپ ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

سانچا <قسم_ناں Types>
بنية ClassDefFormat2_4 : عام OT::ClassDefFormat2_4<Types>
{
  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ();
    constexpr بنانشانی min_ناپ = OT::ClassDefFormat2_4<Types>::min_ناپ;
    إذا (unlikely (راس_len < min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < min_ناپ + هذا->range_ریکارڈ.get_ناپ () - هذا->range_ریکارڈ.len.get_ناپ ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

بنية ClassDef : عام OT::ClassDef
{
  سانچا<قسم_ناں It>
  ساكن گراف_نتیجہ_t<فراغ> جوڑو_class_def (gsubgpos_گراف_context_t& c,
                                             بنانشانی ماپا_id,
                                             بنانشانی link_position,
                                             It glyph_and_class,
                                             بنانشانی max_size)
  {
    TRY_سونپو (بنانشانی class_def_مفرد_id, c.گراف.new_گرہ (خالی_سوچک, خالی_سوچک));
    خودکار& class_def_مفرد_راس = c.گراف.vertices_[class_def_مفرد_id];
    TRY (make_class_def (c, glyph_and_class, class_def_مفرد_id, max_size));

    TRY(c.گراف.vertices_[ماپا_id].جوڑو_real_link (SmallTypes::ناپ, class_def_مفرد_id, link_position));
    TRY(class_def_مفرد_راس.جوڑو_ماپا (ماپا_id, خطأ));

    أرجع Ok();
  }

  سانچا<قسم_ناں It>
  ساكن گراف_نتیجہ_t<فراغ> make_class_def (gsubgpos_گراف_context_t& c,
                                              It glyph_and_class,
                                              بنانشانی dest_obj,
                                              بنانشانی max_size)
  {
    حرف* بفر = (حرف*) hb_calloc (1, max_size);
    إذا (unlikely (!بفر))
      أرجع Err(ونڈ_FAILURE);

    hb_serialize_context_t serializer (بفر, max_size);
    OT::ClassDef_serialize (&serializer, glyph_and_class);
    serializer.سرا_serialize ();
    إذا (unlikely (serializer.وچ_error ()))
    {
      hb_free (بفر);
      أرجع Err(ونڈ_FAILURE);
    }

    hb_بائٹاں_t class_def_copy = serializer.copy_بائٹاں ();
    إذا (unlikely (!class_def_copy.arrayZ))
    {
      hb_free (بفر);
      أرجع Err(ونڈ_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خودکار res = c.جوڑو_بفر ((حرف *) class_def_copy.arrayZ);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      hb_free ((حرف *) class_def_copy.arrayZ);
      أرجع res;
    }


    خودکار& v = c.گراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) class_def_copy.arrayZ;
    v.سیٹ_بفر (سر_2, سر_2 + class_def_copy.لمائی);

    hb_free (بفر);
    أرجع Ok();
  }

  گراف_نتیجہ_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_ناپ();
    إذا (unlikely (راس_len < OT::ClassDef::min_ناپ)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: أرجع ((ClassDefFormat1_3<SmallTypes>*)هذا)->sanitize (راس);
    حالة 2: أرجع ((ClassDefFormat2_4<SmallTypes>*)هذا)->sanitize (راس);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: أرجع ((ClassDefFormat1_3<MediumTypes>*)هذا)->sanitize (راس);
    حالة 4: أرجع ((ClassDefFormat2_4<MediumTypes>*)هذا)->sanitize (راس);
#جے_انت
    افتراضي: أرجع Err(SANITIZE_FAILURE);
    }
  }
};


بنية class_def_ناپ_estimator_t
{
  سانچا<قسم_ناں It>
  ساكن گراف_نتیجہ_t<class_def_ناپ_estimator_t> create (It glyph_and_class)
  {
    class_def_ناپ_estimator_t estimator;
    estimator.reset();
    لكل (خودکار p : + glyph_and_class)
    {
      بنانشانی gid = p.پہلا;
      بنانشانی klass = p.second;

      hb_سیٹ_t* glyphs;
      إذا (estimator.glyphs_فی_class.has (klass, &glyphs) && glyphs) {
        glyphs->جوڑو (gid);
        استمر;
      }

      hb_سیٹ_t new_glyphs;
      new_glyphs.جوڑو (gid);
      estimator.glyphs_فی_class.سیٹ (klass, std::move (new_glyphs));
    }

    TRY(estimator.منزل_نتیجہ ());

    لكل (بنانشانی klass : estimator.glyphs_فی_class.کلیدیں ())
    {
      إذا (!klass) استمر; // class 0 doesn't get encoded.

      ثابت hb_سیٹ_t& glyphs = estimator.glyphs_فی_class.get (klass);
      hb_codepoint_t شروع = HB_سیٹ_قدر_INVALID;
      hb_codepoint_t سرا = HB_سیٹ_قدر_INVALID;

      بنانشانی گنتی = 0;
      طالما (glyphs.اگلا_range (&شروع, &سرا))
        گنتی++;

      estimator.عدد_ranges_فی_class.سیٹ (klass, گنتی);
    }

    TRY(estimator.منزل_نتیجہ ());
    أرجع estimator;
  }

  فراغ reset() {
    included_glyphs.clear();
    included_class_def_glyphs.clear();
    included_classes.clear();
    class_def_range_گنتی = 0;
    class_max = 0;
    coverage_ناپ_val = compute_coverage_ناپ ();
  }

  // Size of coverage for all glyphs added via 'add_class_def_size'.
  بنانشانی coverage_ناپ () ثابت
  {
    أرجع coverage_ناپ_val;
  }

  // Compute the new size of the ClassDef table if all glyphs associated with 'klass' were added.
  بنانشانی جوڑو_class_def_ناپ (بنانشانی klass)
  {
    إذا (!included_classes.has(klass)) {
      hb_سیٹ_t* glyphs = خالی_سوچک;
      إذا (glyphs_فی_class.has(klass, &glyphs)) {
        بنانشانی عدد_glyphs = included_glyphs.get_population();
        included_glyphs.union_(*glyphs);
        إذا (klass)
          included_class_def_glyphs.union_(*glyphs);
        إذا (عدد_glyphs != included_glyphs.get_population())
          coverage_ناپ_val = compute_coverage_ناپ ();
      }

      إذا (klass)
      {
        class_def_range_گنتی += عدد_ranges_فی_class.get (klass);
        إذا (glyphs && klass > class_max)
          class_max = klass;
      }
      included_classes.جوڑو(klass);
    }

    أرجع class_def_ناپ ();
  }

  بنانشانی عدد_glyph_ranges (ثابت hb_سیٹ_t &glyphs) ثابت {
    hb_codepoint_t شروع = HB_سیٹ_قدر_INVALID;
    hb_codepoint_t سرا = HB_سیٹ_قدر_INVALID;

    بنانشانی گنتی = 0;
    طالما (glyphs.اگلا_range (&شروع, &سرا)) {
        گنتی++;
    }
    أرجع گنتی;
  }

 خاص:
  گراف_نتیجہ_t<فراغ> منزل_نتیجہ() ثابت
  {
    TRY (گراف_نتیجہ_t<فراغ>::from (عدد_ranges_فی_class, ونڈ_FAILURE));
    TRY (گراف_نتیجہ_t<فراغ>::from (glyphs_فی_class, ونڈ_FAILURE));
    TRY (گراف_نتیجہ_t<فراغ>::from (included_classes, ونڈ_FAILURE));
    TRY (گراف_نتیجہ_t<فراغ>::from (included_glyphs, ونڈ_FAILURE));
    TRY (گراف_نتیجہ_t<فراغ>::from (included_class_def_glyphs, ونڈ_FAILURE));

    لكل (ثابت hb_سیٹ_t& s : glyphs_فی_class.values ())
    {
          TRY (گراف_نتیجہ_t<فراغ>::from (s, ونڈ_FAILURE));
    }
    أرجع Ok();
  }

  class_def_ناپ_estimator_t ()
      : عدد_ranges_فی_class (), glyphs_فی_class () {}

  بنانشانی compute_coverage_ناپ () ثابت
  {
    بنانشانی glyph_ناپ = glyph_id_ناپ (included_glyphs);
    بنانشانی coverage_ادھار_ناپ = OT::HBUINT16::static_ناپ + glyph_ناپ;
    بنانشانی range_ناپ = 3 * glyph_ناپ;
    بنانشانی format1_ناپ = coverage_ادھار_ناپ + glyph_ناپ * included_glyphs.get_population();
    بنانشانی format2_ناپ = coverage_ادھار_ناپ + range_ناپ * عدد_glyph_ranges (included_glyphs);
    أرجع hb_min(format1_ناپ, format2_ناپ);
  }

  بنانشانی glyph_id_ناپ (ثابت hb_سیٹ_t &glyphs) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (!glyphs.is_empty () && glyphs.get_max () > 0xFFFFu)
      أرجع OT::Layout::MediumTypes::ناپ;
#جے_انت
    أرجع OT::Layout::SmallTypes::ناپ;
  }

  بنانشانی class_def_ناپ () ثابت
  {
    إذا (included_class_def_glyphs.is_empty ())
      أرجع OT::HBUINT16::static_ناپ + OT::Layout::SmallTypes::ناپ;

    بنانشانی بہترین_ناپ = UINT_MAX;
    hb_codepoint_t glyph_max = included_class_def_glyphs.get_max ();
    بنانشانی glyph_وسعت = glyph_max - included_class_def_glyphs.get_min () + 1;

    إذا (glyph_max <= 0xFFFFu && glyph_وسعت <= 0xFFFFu && class_max <= 0xFFFFu)
      بہترین_ناپ = hb_min (بہترین_ناپ, 6 + 2 * glyph_وسعت);
    إذا (glyph_max <= 0xFFFFu && class_def_range_گنتی <= 0xFFFFu && class_max <= 0xFFFFu)
      بہترین_ناپ = hb_min (بہترین_ناپ, 4 + 6 * class_def_range_گنتی);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (glyph_max <= 0xFFFFFFu && glyph_وسعت <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
      بہترین_ناپ = hb_min (بہترین_ناپ, 8 + 3 * glyph_وسعت);
    إذا (glyph_max <= 0xFFFFFFu && class_def_range_گنتی <= 0xFFFFFFu && class_max <= 0xFFFFu)
      بہترین_ناپ = hb_min (بہترین_ناپ, 5 + 8 * class_def_range_گنتی);
#جے_انت
    أرجع بہترین_ناپ;
  }

  hb_hashmap_t<بنانشانی, بنانشانی> عدد_ranges_فی_class;
  hb_hashmap_t<بنانشانی, hb_سیٹ_t> glyphs_فی_class;
  hb_سیٹ_t included_classes;
  hb_سیٹ_t included_glyphs;
  hb_سیٹ_t included_class_def_glyphs;
  بنانشانی class_def_range_گنتی;
  بنانشانی class_max;
  بنانشانی coverage_ناپ_val;
};


}

#جے_انت  // GRAPH_CLASSDEF_GRAPH_HH
