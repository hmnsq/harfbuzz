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

#جيڪڏهن_وصف_نه_ٿيل گراف_CLASSDEF_گراف_HH
#وصف_ڪريو گراف_CLASSDEF_گراف_HH

نطاق گراف {

سانچو <قسم_نالو Types>
بنية ClassDefFormat1_3 : عام OT::ClassDefFormat1_3<Types>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز();
    constexpr بنانشان min_سائيز = OT::ClassDefFormat1_3<Types>::min_سائيز;
    إذا (unlikely (راس_len < min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < min_سائيز + هذا->class_قدر.get_سائيز () - هذا->class_قدر.len.get_سائيز ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

سانچو <قسم_نالو Types>
بنية ClassDefFormat2_4 : عام OT::ClassDefFormat2_4<Types>
{
  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز();
    constexpr بنانشان min_سائيز = OT::ClassDefFormat2_4<Types>::min_سائيز;
    إذا (unlikely (راس_len < min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    إذا (unlikely (راس_len < min_سائيز + هذا->range_رڪارڊ.get_سائيز () - هذا->range_رڪارڊ.len.get_سائيز ()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }
};

بنية ClassDef : عام OT::ClassDef
{
  سانچو<قسم_نالو It>
  ساكن گراف_نتيجو_t<فراغ> جوڙيو_class_def (gsubgpos_گراف_context_t& c,
                                             بنانشان والد_id,
                                             بنانشان link_position,
                                             It glyph_and_class,
                                             بنانشان max_size)
  {
    TRY_سونپيو (بنانشان class_def_مفرد_id, c.گراف.new_ڳنڍ (خالي_اشارو, خالي_اشارو));
    خودڪار& class_def_مفرد_راس = c.گراف.vertices_[class_def_مفرد_id];
    TRY (make_class_def (c, glyph_and_class, class_def_مفرد_id, max_size));

    TRY(c.گراف.vertices_[والد_id].جوڙيو_real_link (SmallTypes::سائيز, class_def_مفرد_id, link_position));
    TRY(class_def_مفرد_راس.جوڙيو_والد (والد_id, خطأ));

    أرجع Ok();
  }

  سانچو<قسم_نالو It>
  ساكن گراف_نتيجو_t<فراغ> make_class_def (gsubgpos_گراف_context_t& c,
                                              It glyph_and_class,
                                              بنانشان dest_obj,
                                              بنانشان max_size)
  {
    حرف* بفر = (حرف*) hb_calloc (1, max_size);
    إذا (unlikely (!بفر))
      أرجع Err(ورهاست_FAILURE);

    hb_serialize_context_t serializer (بفر, max_size);
    OT::ClassDef_serialize (&serializer, glyph_and_class);
    serializer.پڇاڙي_serialize ();
    إذا (unlikely (serializer.اندريون_error ()))
    {
      hb_free (بفر);
      أرجع Err(ورهاست_FAILURE);
    }

    hb_بائيٽون_t class_def_copy = serializer.copy_بائيٽون ();
    إذا (unlikely (!class_def_copy.arrayZ))
    {
      hb_free (بفر);
      أرجع Err(ورهاست_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    خودڪار res = c.جوڙيو_بفر ((حرف *) class_def_copy.arrayZ);
    إذا (unlikely (!res.is_ok ()))
    {
      hb_free (بفر);
      hb_free ((حرف *) class_def_copy.arrayZ);
      أرجع res;
    }


    خودڪار& v = c.گراف.vertices_[dest_obj];
    حرف* سر_2 = (حرف *) class_def_copy.arrayZ;
    v.سيٽ_بفر (سر_2, سر_2 + class_def_copy.ڊيگهائي);

    hb_free (بفر);
    أرجع Ok();
  }

  گراف_نتيجو_t<فراغ> sanitize (ثابت گراف_t::راس_t& راس) ثابت
  {
    size_t راس_len = راس.جدول_سائيز();
    إذا (unlikely (راس_len < OT::ClassDef::min_سائيز)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();
    بدل (u.format.v)
    {
    حالة 1: أرجع ((ClassDefFormat1_3<SmallTypes>*)هذا)->sanitize (راس);
    حالة 2: أرجع ((ClassDefFormat2_4<SmallTypes>*)هذا)->sanitize (راس);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: أرجع ((ClassDefFormat1_3<MediumTypes>*)هذا)->sanitize (راس);
    حالة 4: أرجع ((ClassDefFormat2_4<MediumTypes>*)هذا)->sanitize (راس);
#جيڪڏهن_پورو
    افتراضي: أرجع Err(SANITIZE_FAILURE);
    }
  }
};


بنية class_def_سائيز_estimator_t
{
  سانچو<قسم_نالو It>
  ساكن گراف_نتيجو_t<class_def_سائيز_estimator_t> create (It glyph_and_class)
  {
    class_def_سائيز_estimator_t estimator;
    estimator.reset();
    لكل (خودڪار p : + glyph_and_class)
    {
      بنانشان gid = p.پهريون;
      بنانشان klass = p.second;

      hb_سيٽ_t* glyphs;
      إذا (estimator.glyphs_في_class.has (klass, &glyphs) && glyphs) {
        glyphs->جوڙيو (gid);
        استمر;
      }

      hb_سيٽ_t new_glyphs;
      new_glyphs.جوڙيو (gid);
      estimator.glyphs_في_class.سيٽ (klass, std::move (new_glyphs));
    }

    TRY(estimator.منزل_نتيجو ());

    لكل (بنانشان klass : estimator.glyphs_في_class.ڪنجيون ())
    {
      إذا (!klass) استمر; // class 0 doesn't get encoded.

      ثابت hb_سيٽ_t& glyphs = estimator.glyphs_في_class.get (klass);
      hb_codepoint_t شروعات = HB_سيٽ_قدر_INVALID;
      hb_codepoint_t پڇاڙي = HB_سيٽ_قدر_INVALID;

      بنانشان ڳڻپ = 0;
      طالما (glyphs.ايندڙ_range (&شروعات, &پڇاڙي))
        ڳڻپ++;

      estimator.انگ_ranges_في_class.سيٽ (klass, ڳڻپ);
    }

    TRY(estimator.منزل_نتيجو ());
    أرجع estimator;
  }

  فراغ reset() {
    included_glyphs.clear();
    included_class_def_glyphs.clear();
    included_classes.clear();
    class_def_range_ڳڻپ = 0;
    class_max = 0;
    coverage_سائيز_val = compute_coverage_سائيز ();
  }

  // Size of coverage for all glyphs added via 'add_class_def_size'.
  بنانشان coverage_سائيز () ثابت
  {
    أرجع coverage_سائيز_val;
  }

  // Compute the new size of the ClassDef table if all glyphs associated with 'klass' were added.
  بنانشان جوڙيو_class_def_سائيز (بنانشان klass)
  {
    إذا (!included_classes.has(klass)) {
      hb_سيٽ_t* glyphs = خالي_اشارو;
      إذا (glyphs_في_class.has(klass, &glyphs)) {
        بنانشان انگ_glyphs = included_glyphs.get_population();
        included_glyphs.union_(*glyphs);
        إذا (klass)
          included_class_def_glyphs.union_(*glyphs);
        إذا (انگ_glyphs != included_glyphs.get_population())
          coverage_سائيز_val = compute_coverage_سائيز ();
      }

      إذا (klass)
      {
        class_def_range_ڳڻپ += انگ_ranges_في_class.get (klass);
        إذا (glyphs && klass > class_max)
          class_max = klass;
      }
      included_classes.جوڙيو(klass);
    }

    أرجع class_def_سائيز ();
  }

  بنانشان انگ_glyph_ranges (ثابت hb_سيٽ_t &glyphs) ثابت {
    hb_codepoint_t شروعات = HB_سيٽ_قدر_INVALID;
    hb_codepoint_t پڇاڙي = HB_سيٽ_قدر_INVALID;

    بنانشان ڳڻپ = 0;
    طالما (glyphs.ايندڙ_range (&شروعات, &پڇاڙي)) {
        ڳڻپ++;
    }
    أرجع ڳڻپ;
  }

 خاص:
  گراف_نتيجو_t<فراغ> منزل_نتيجو() ثابت
  {
    TRY (گراف_نتيجو_t<فراغ>::from (انگ_ranges_في_class, ورهاست_FAILURE));
    TRY (گراف_نتيجو_t<فراغ>::from (glyphs_في_class, ورهاست_FAILURE));
    TRY (گراف_نتيجو_t<فراغ>::from (included_classes, ورهاست_FAILURE));
    TRY (گراف_نتيجو_t<فراغ>::from (included_glyphs, ورهاست_FAILURE));
    TRY (گراف_نتيجو_t<فراغ>::from (included_class_def_glyphs, ورهاست_FAILURE));

    لكل (ثابت hb_سيٽ_t& s : glyphs_في_class.values ())
    {
          TRY (گراف_نتيجو_t<فراغ>::from (s, ورهاست_FAILURE));
    }
    أرجع Ok();
  }

  class_def_سائيز_estimator_t ()
      : انگ_ranges_في_class (), glyphs_في_class () {}

  بنانشان compute_coverage_سائيز () ثابت
  {
    بنانشان glyph_سائيز = glyph_id_سائيز (included_glyphs);
    بنانشان coverage_بنياد_سائيز = OT::HBUINT16::static_سائيز + glyph_سائيز;
    بنانشان range_سائيز = 3 * glyph_سائيز;
    بنانشان format1_سائيز = coverage_بنياد_سائيز + glyph_سائيز * included_glyphs.get_population();
    بنانشان format2_سائيز = coverage_بنياد_سائيز + range_سائيز * انگ_glyph_ranges (included_glyphs);
    أرجع hb_min(format1_سائيز, format2_سائيز);
  }

  بنانشان glyph_id_سائيز (ثابت hb_سيٽ_t &glyphs) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (!glyphs.is_empty () && glyphs.get_max () > 0xFFFFu)
      أرجع OT::Layout::MediumTypes::سائيز;
#جيڪڏهن_پورو
    أرجع OT::Layout::SmallTypes::سائيز;
  }

  بنانشان class_def_سائيز () ثابت
  {
    إذا (included_class_def_glyphs.is_empty ())
      أرجع OT::HBUINT16::static_سائيز + OT::Layout::SmallTypes::سائيز;

    بنانشان بهترين_سائيز = UINT_MAX;
    hb_codepoint_t glyph_max = included_class_def_glyphs.get_max ();
    بنانشان glyph_وسعت = glyph_max - included_class_def_glyphs.get_min () + 1;

    إذا (glyph_max <= 0xFFFFu && glyph_وسعت <= 0xFFFFu && class_max <= 0xFFFFu)
      بهترين_سائيز = hb_min (بهترين_سائيز, 6 + 2 * glyph_وسعت);
    إذا (glyph_max <= 0xFFFFu && class_def_range_ڳڻپ <= 0xFFFFu && class_max <= 0xFFFFu)
      بهترين_سائيز = hb_min (بهترين_سائيز, 4 + 6 * class_def_range_ڳڻپ);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (glyph_max <= 0xFFFFFFu && glyph_وسعت <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
      بهترين_سائيز = hb_min (بهترين_سائيز, 8 + 3 * glyph_وسعت);
    إذا (glyph_max <= 0xFFFFFFu && class_def_range_ڳڻپ <= 0xFFFFFFu && class_max <= 0xFFFFu)
      بهترين_سائيز = hb_min (بهترين_سائيز, 5 + 8 * class_def_range_ڳڻپ);
#جيڪڏهن_پورو
    أرجع بهترين_سائيز;
  }

  hb_hashmap_t<بنانشان, بنانشان> انگ_ranges_في_class;
  hb_hashmap_t<بنانشان, hb_سيٽ_t> glyphs_في_class;
  hb_سيٽ_t included_classes;
  hb_سيٽ_t included_glyphs;
  hb_سيٽ_t included_class_def_glyphs;
  بنانشان class_def_range_ڳڻپ;
  بنانشان class_max;
  بنانشان coverage_سائيز_val;
};


}

#جيڪڏهن_پورو  // GRAPH_CLASSDEF_GRAPH_HH
