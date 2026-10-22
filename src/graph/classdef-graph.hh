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
#شامل "../hb-ot-layout-common.hh"
#شامل "graph-result.hh"

#اگر_غیر_تعریف_شدہ گراف_CLASSDEF_گراف_HH
#تعریف_کرو گراف_CLASSDEF_گراف_HH

نامخلا گراف {

سانچہ <قسم_نام Types>
ساخت ClassDefFormat1_3 : عوامی OT::ClassDefFormat1_3<Types>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز();
    constexpr غیرعلامتی min_سائز = OT::ClassDefFormat1_3<Types>::min_سائز;
    اگر (unlikely (راس_len < min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (راس_len < min_سائز + یہ->class_قدر.get_سائز () - یہ->class_قدر.len.get_سائز ()))
      واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }
};

سانچہ <قسم_نام Types>
ساخت ClassDefFormat2_4 : عوامی OT::ClassDefFormat2_4<Types>
{
  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز();
    constexpr غیرعلامتی min_سائز = OT::ClassDefFormat2_4<Types>::min_سائز;
    اگر (unlikely (راس_len < min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();
    اگر (unlikely (راس_len < min_سائز + یہ->range_ریکارڈ.get_سائز () - یہ->range_ریکارڈ.len.get_سائز ()))
      واپس Err(SANITIZE_FAILURE);
    واپس Ok();
  }
};

ساخت ClassDef : عوامی OT::ClassDef
{
  سانچہ<قسم_نام It>
  ساکن گراف_نتیجہ_t<خالی> جوڑو_class_def (gsubgpos_گراف_context_t& c,
                                             غیرعلامتی والد_id,
                                             غیرعلامتی link_position,
                                             It glyph_and_class,
                                             غیرعلامتی max_size)
  {
    TRY_تفویض (غیرعلامتی class_def_مفرد_id, c.گراف.new_گرہ (خالی_اشاریہ, خالی_اشاریہ));
    خودکار& class_def_مفرد_راس = c.گراف.vertices_[class_def_مفرد_id];
    TRY (make_class_def (c, glyph_and_class, class_def_مفرد_id, max_size));

    TRY(c.گراف.vertices_[والد_id].جوڑو_real_link (SmallTypes::سائز, class_def_مفرد_id, link_position));
    TRY(class_def_مفرد_راس.جوڑو_والد (والد_id, جھوٹ));

    واپس Ok();
  }

  سانچہ<قسم_نام It>
  ساکن گراف_نتیجہ_t<خالی> make_class_def (gsubgpos_گراف_context_t& c,
                                              It glyph_and_class,
                                              غیرعلامتی dest_obj,
                                              غیرعلامتی max_size)
  {
    حرف* بفر = (حرف*) hb_calloc (1, max_size);
    اگر (unlikely (!بفر))
      واپس Err(تخصیص_FAILURE);

    hb_serialize_context_t serializer (بفر, max_size);
    OT::ClassDef_serialize (&serializer, glyph_and_class);
    serializer.انتہا_serialize ();
    اگر (unlikely (serializer.اندر_error ()))
    {
      hb_free (بفر);
      واپس Err(تخصیص_FAILURE);
    }

    hb_بائٹیں_t class_def_copy = serializer.copy_بائٹیں ();
    اگر (unlikely (!class_def_copy.arrayZ))
    {
      hb_free (بفر);
      واپس Err(تخصیص_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خودکار res = c.جوڑو_بفر ((حرف *) class_def_copy.arrayZ);
    اگر (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      hb_free ((حرف *) class_def_copy.arrayZ);
      واپس res;
    }


    خودکار& v = c.گراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) class_def_copy.arrayZ;
    v.سیٹ_بفر (سر_2, سر_2 + class_def_copy.لمبائی);

    hb_free (بفر);
    واپس Ok();
  }

  گراف_نتیجہ_t<خالی> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائز();
    اگر (unlikely (راس_len < OT::ClassDef::min_سائز)) واپس Err(SANITIZE_FAILURE);
    hb_barrier ();
    انتخاب (u.format.v)
    {
    حالت 1: واپس ((ClassDefFormat1_3<SmallTypes>*)یہ)->sanitize (راس);
    حالت 2: واپس ((ClassDefFormat2_4<SmallTypes>*)یہ)->sanitize (راس);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: واپس ((ClassDefFormat1_3<MediumTypes>*)یہ)->sanitize (راس);
    حالت 4: واپس ((ClassDefFormat2_4<MediumTypes>*)یہ)->sanitize (راس);
#اگر_ختم
    طےشدہ: واپس Err(SANITIZE_FAILURE);
    }
  }
};


ساخت class_def_سائز_estimator_t
{
  سانچہ<قسم_نام It>
  ساکن گراف_نتیجہ_t<class_def_سائز_estimator_t> create (It glyph_and_class)
  {
    class_def_سائز_estimator_t estimator;
    estimator.reset();
    برائے (خودکار p : + glyph_and_class)
    {
      غیرعلامتی gid = p.پہلا;
      غیرعلامتی klass = p.second;

      hb_سیٹ_t* glyphs;
      اگر (estimator.glyphs_فی_class.has (klass, &glyphs) && glyphs) {
        glyphs->جوڑو (gid);
        جاری;
      }

      hb_سیٹ_t new_glyphs;
      new_glyphs.جوڑو (gid);
      estimator.glyphs_فی_class.سیٹ (klass, std::move (new_glyphs));
    }

    TRY(estimator.مقصد_نتیجہ ());

    برائے (غیرعلامتی klass : estimator.glyphs_فی_class.کلیدیں ())
    {
      اگر (!klass) جاری; // class 0 doesn't get encoded.

      ثابت hb_سیٹ_t& glyphs = estimator.glyphs_فی_class.get (klass);
      hb_codepoint_t آغاز = HB_سیٹ_قدر_INVALID;
      hb_codepoint_t انتہا = HB_سیٹ_قدر_INVALID;

      غیرعلامتی گنتی = 0;
      جب (glyphs.اگلا_range (&آغاز, &انتہا))
        گنتی++;

      estimator.عدد_ranges_فی_class.سیٹ (klass, گنتی);
    }

    TRY(estimator.مقصد_نتیجہ ());
    واپس estimator;
  }

  خالی reset() {
    included_glyphs.clear();
    included_class_def_glyphs.clear();
    included_classes.clear();
    class_def_range_گنتی = 0;
    class_max = 0;
    coverage_سائز_val = compute_coverage_سائز ();
  }

  // Size of coverage for all glyphs added via 'add_class_def_size'.
  غیرعلامتی coverage_سائز () ثابت
  {
    واپس coverage_سائز_val;
  }

  // Compute the new size of the ClassDef table if all glyphs associated with 'klass' were added.
  غیرعلامتی جوڑو_class_def_سائز (غیرعلامتی klass)
  {
    اگر (!included_classes.has(klass)) {
      hb_سیٹ_t* glyphs = خالی_اشاریہ;
      اگر (glyphs_فی_class.has(klass, &glyphs)) {
        غیرعلامتی عدد_glyphs = included_glyphs.get_population();
        included_glyphs.union_(*glyphs);
        اگر (klass)
          included_class_def_glyphs.union_(*glyphs);
        اگر (عدد_glyphs != included_glyphs.get_population())
          coverage_سائز_val = compute_coverage_سائز ();
      }

      اگر (klass)
      {
        class_def_range_گنتی += عدد_ranges_فی_class.get (klass);
        اگر (glyphs && klass > class_max)
          class_max = klass;
      }
      included_classes.جوڑو(klass);
    }

    واپس class_def_سائز ();
  }

  غیرعلامتی عدد_glyph_ranges (ثابت hb_سیٹ_t &glyphs) ثابت {
    hb_codepoint_t آغاز = HB_سیٹ_قدر_INVALID;
    hb_codepoint_t انتہا = HB_سیٹ_قدر_INVALID;

    غیرعلامتی گنتی = 0;
    جب (glyphs.اگلا_range (&آغاز, &انتہا)) {
        گنتی++;
    }
    واپس گنتی;
  }

 نجی:
  گراف_نتیجہ_t<خالی> مقصد_نتیجہ() ثابت
  {
    TRY (گراف_نتیجہ_t<خالی>::from (عدد_ranges_فی_class, تخصیص_FAILURE));
    TRY (گراف_نتیجہ_t<خالی>::from (glyphs_فی_class, تخصیص_FAILURE));
    TRY (گراف_نتیجہ_t<خالی>::from (included_classes, تخصیص_FAILURE));
    TRY (گراف_نتیجہ_t<خالی>::from (included_glyphs, تخصیص_FAILURE));
    TRY (گراف_نتیجہ_t<خالی>::from (included_class_def_glyphs, تخصیص_FAILURE));

    برائے (ثابت hb_سیٹ_t& s : glyphs_فی_class.values ())
    {
          TRY (گراف_نتیجہ_t<خالی>::from (s, تخصیص_FAILURE));
    }
    واپس Ok();
  }

  class_def_سائز_estimator_t ()
      : عدد_ranges_فی_class (), glyphs_فی_class () {}

  غیرعلامتی compute_coverage_سائز () ثابت
  {
    غیرعلامتی glyph_سائز = glyph_id_سائز (included_glyphs);
    غیرعلامتی coverage_بنیاد_سائز = OT::HBUINT16::static_سائز + glyph_سائز;
    غیرعلامتی range_سائز = 3 * glyph_سائز;
    غیرعلامتی format1_سائز = coverage_بنیاد_سائز + glyph_سائز * included_glyphs.get_population();
    غیرعلامتی format2_سائز = coverage_بنیاد_سائز + range_سائز * عدد_glyph_ranges (included_glyphs);
    واپس hb_min(format1_سائز, format2_سائز);
  }

  غیرعلامتی glyph_id_سائز (ثابت hb_سیٹ_t &glyphs) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (!glyphs.is_empty () && glyphs.get_max () > 0xFFFFu)
      واپس OT::Layout::MediumTypes::سائز;
#اگر_ختم
    واپس OT::Layout::SmallTypes::سائز;
  }

  غیرعلامتی class_def_سائز () ثابت
  {
    اگر (included_class_def_glyphs.is_empty ())
      واپس OT::HBUINT16::static_سائز + OT::Layout::SmallTypes::سائز;

    غیرعلامتی بہترین_سائز = UINT_MAX;
    hb_codepoint_t glyph_max = included_class_def_glyphs.get_max ();
    غیرعلامتی glyph_وسعت = glyph_max - included_class_def_glyphs.get_min () + 1;

    اگر (glyph_max <= 0xFFFFu && glyph_وسعت <= 0xFFFFu && class_max <= 0xFFFFu)
      بہترین_سائز = hb_min (بہترین_سائز, 6 + 2 * glyph_وسعت);
    اگر (glyph_max <= 0xFFFFu && class_def_range_گنتی <= 0xFFFFu && class_max <= 0xFFFFu)
      بہترین_سائز = hb_min (بہترین_سائز, 4 + 6 * class_def_range_گنتی);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (glyph_max <= 0xFFFFFFu && glyph_وسعت <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
      بہترین_سائز = hb_min (بہترین_سائز, 8 + 3 * glyph_وسعت);
    اگر (glyph_max <= 0xFFFFFFu && class_def_range_گنتی <= 0xFFFFFFu && class_max <= 0xFFFFu)
      بہترین_سائز = hb_min (بہترین_سائز, 5 + 8 * class_def_range_گنتی);
#اگر_ختم
    واپس بہترین_سائز;
  }

  hb_hashmap_t<غیرعلامتی, غیرعلامتی> عدد_ranges_فی_class;
  hb_hashmap_t<غیرعلامتی, hb_سیٹ_t> glyphs_فی_class;
  hb_سیٹ_t included_classes;
  hb_سیٹ_t included_glyphs;
  hb_سیٹ_t included_class_def_glyphs;
  غیرعلامتی class_def_range_گنتی;
  غیرعلامتی class_max;
  غیرعلامتی coverage_سائز_val;
};


}

#اگر_ختم  // GRAPH_CLASSDEF_GRAPH_HH
