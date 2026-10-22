/*
 * Copyright © 2012,2017  Google, Inc.
 * Copyright © 2021 Behdad Esfahbod
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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_تعریف_نشده HB_بیت_مجموعه_INVERTIBLE_HH
#تعریف_کن HB_بیت_مجموعه_INVERTIBLE_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set.hh"


بنية hb_بیت_مجموعه_invertible_t
{
  hb_بیت_مجموعه_t s;
  منطقي inverted = خطأ;

  hb_بیت_مجموعه_invertible_t () = افتراضي;
  hb_بیت_مجموعه_invertible_t (ثابت hb_بیت_مجموعه_invertible_t& o) = افتراضي;
  hb_بیت_مجموعه_invertible_t (hb_بیت_مجموعه_invertible_t&& other)  noexcept : hb_بیت_مجموعه_invertible_t () { hb_جابجا (*هذا, other); }
  hb_بیت_مجموعه_invertible_t& عملگر= (ثابت hb_بیت_مجموعه_invertible_t& o) = افتراضي;
  hb_بیت_مجموعه_invertible_t& عملگر= (hb_بیت_مجموعه_invertible_t&& other)  noexcept { hb_جابجا (*هذا, other); أرجع *هذا; }
  دوست فراغ swap (hb_بیت_مجموعه_invertible_t &a, hb_بیت_مجموعه_invertible_t &b) noexcept
  {
    إذا (likely (!a.s.successful || !b.s.successful))
      أرجع;
    hb_جابجا (a.inverted, b.inverted);
    hb_جابجا (a.s, b.s);
  }

  فراغ init () { s.init (); inverted = خطأ; }
  فراغ fini () { s.fini (); }
  فراغ err () { s.err (); }
  منطقي درون_error () ثابت { أرجع s.درون_error (); }
  صریح عملگر منطقي () ثابت { أرجع !is_empty (); }

  فراغ alloc (بیعلامت sz) { s.alloc (sz); }
  فراغ reset ()
  {
    s.reset ();
    inverted = خطأ;
  }
  فراغ clear ()
  {
    s.clear ();
    إذا (likely (s.successful))
      inverted = خطأ;
  }
  فراغ invert ()
  {
    إذا (likely (s.successful))
      inverted = !inverted;
  }

  منطقي is_inverted () ثابت
  {
    أرجع inverted;
  }

  منطقي is_empty () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    أرجع v == INVALID;
  }
  uint32_t hash () ثابت { أرجع s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    أرجع v;
  }
  hb_codepoint_t get_max () ثابت
  {
    hb_codepoint_t v = INVALID;
    قبلی (&v);
    أرجع v;
  }
  بیعلامت صحيح get_population () ثابت
  { أرجع inverted ? INVALID - s.get_population () : s.get_population (); }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.get_singleton (codepoint);
    إذا (s.get_population () != INVALID - 1)
      أرجع خطأ;
    *codepoint = get_min ();
    أرجع *codepoint != INVALID;
  }


  فراغ جمع_کن (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.جمع_کن (g); }
  فراغ جمع_کن_بیتها (hb_codepoint_t g, uint64_t بیتها)
  { unlikely (inverted) ? s.del_بیتها (g, بیتها) : s.جمع_کن_بیتها (g, بیتها); }
  منطقي جمع_کن_range (hb_codepoint_t a, hb_codepoint_t b)
  { أرجع unlikely (inverted) ? ((فراغ) s.del_range (a, b), درست) : s.جمع_کن_range (a, b); }

  قالب <نام_نوع T>
  فراغ جمع_کن_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  { inverted ? s.del_array (array, شمار, stride) : s.جمع_کن_array (array, شمار, stride); }
  قالب <نام_نوع T>
  فراغ جمع_کن_array (ثابت hb_array_t<ثابت T>& arr) { جمع_کن_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  قالب <نام_نوع T>
  منطقي جمع_کن_sorted_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  { أرجع inverted ? s.del_sorted_array (array, شمار, stride) : s.جمع_کن_sorted_array (array, شمار, stride); }
  قالب <نام_نوع T>
  منطقي جمع_کن_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جمع_کن_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { unlikely (inverted) ? s.جمع_کن (g) : s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (فراغ) s.جمع_کن_range (a, b) : s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { أرجع s.get (g) ^ inverted; }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي عملگر [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي عملگر () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_بیت_مجموعه_invertible_t& عملگر << (hb_codepoint_t v)
  { جمع_کن (v); أرجع *هذا; }
  hb_بیت_مجموعه_invertible_t& عملگر << (ثابت hb_codepoint_pair_t& range)
  { جمع_کن_range (range.اولی, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_بیت_مجموعه_invertible_t &other) ثابت
  { أرجع inverted || other.inverted || s.intersects (other.s); }

  منطقي intersects (ثابت hb_بیت_مجموعه_invertible_t &other) ثابت
  {
    إذا (likely (!inverted))
      أرجع unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    إذا (likely (!other.inverted))
      أرجع !other.s.is_subset (s);

    بیعلامت population = s.get_population ();
    بیعلامت other_population = other.s.get_population ();
    إذا ((uint64_t) population + other_population < INVALID)
      أرجع درست;

    أرجع population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  منطقي intersects (hb_codepoint_t اولی, hb_codepoint_t آخرین) ثابت
  {
    hb_codepoint_t c = اولی - 1;
    أرجع next (&c) && c <= آخرین;
  }

  فراغ مجموعه (ثابت hb_بیت_مجموعه_invertible_t &other)
  {
    s.مجموعه (other.s);
    إذا (likely (s.successful))
      inverted = other.inverted;
  }

  منطقي is_equal (ثابت hb_بیت_مجموعه_invertible_t &other) ثابت
  {
    إذا (likely (inverted == other.inverted))
      أرجع s.is_equal (other.s);
    وإلا
    {
      إذا (get_population () != other.get_population ())
	أرجع خطأ;

      /* TODO Add iter_ranges() and use here. */
      خودکار it1 = iter ();
      خودکار it2 = other.iter ();
      أرجع hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { أرجع _.اولی == _.second; }));
    }
  }

  منطقي is_subset (ثابت hb_بیت_مجموعه_invertible_t &larger_مجموعه) ثابت
  {
    إذا (unlikely (inverted != larger_مجموعه.inverted))
    {
      إذا (inverted)
	أرجع hb_all (iter (), larger_مجموعه.s);
      وإلا
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	أرجع !s.intersects (larger_مجموعه.s);
    }
    وإلا
      أرجع unlikely (inverted) ? larger_مجموعه.s.is_subset (s) : s.is_subset (larger_مجموعه.s);
  }

  محمي:
  قالب <نام_نوع Op>
  فراغ process (ثابت Op& op,
		ثابت hb_بیت_مجموعه_invertible_t &other,
		hb_وکتور_t<بیعلامت> *workspace = اشاره_تهی)
  { s.process (op, other.s, workspace); }
  عام:
  فراغ union_ (ثابت hb_بیت_مجموعه_invertible_t &other)
  {
    إذا (likely (inverted == other.inverted))
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_and, other);
      وإلا
	process (hb_bitwise_or, other); /* Main branch. */
    }
    وإلا
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_gt, other);
      وإلا
	process (hb_bitwise_lt, other);
    }
    إذا (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  فراغ intersect (ثابت hb_بیت_مجموعه_invertible_t &other,
		  hb_وکتور_t<بیعلامت> *workspace = اشاره_تهی)
  {
    إذا (likely (inverted == other.inverted))
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      وإلا
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    وإلا
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      وإلا
	process (hb_bitwise_gt, other, workspace);
    }
    إذا (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  فراغ تفریق (ثابت hb_بیت_مجموعه_invertible_t &other)
  {
    إذا (likely (inverted == other.inverted))
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_lt, other);
      وإلا
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    وإلا
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_or, other);
      وإلا
	process (hb_bitwise_and, other);
    }
    إذا (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  فراغ متقارن_difference (ثابت hb_بیت_مجموعه_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    إذا (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.next (codepoint);

    خودکار old = *codepoint;
    إذا (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    خودکار v = old;
    s.next (&v);
    إذا (old + 1 < v)
    {
      *codepoint = old + 1;
      أرجع درست;
    }

    v = old;
    s.بعدی_range (&old, &v);

    *codepoint = v + 1;
    أرجع *codepoint != INVALID;
  }
  منطقي بعدی_بیتها (hb_codepoint_t *codepoint, uint64_t *بیتها) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.بعدی_بیتها (codepoint, بیتها);

    hb_codepoint_t پایه;
    إذا (unlikely (*codepoint == INVALID))
      پایه = 0;
    وإلا
    {
      پایه = *codepoint & ~hb_بیت_صفحه_t::ELT_ماسک;
      إذا (unlikely (پایه >= INVALID - hb_بیت_صفحه_t::ELT_بیتها + 1))
	اذهب انجامشده;
      پایه += hb_بیت_صفحه_t::ELT_بیتها;
    }

    طالما (درست)
    {
      hb_codepoint_t cursor = پایه ? پایه - hb_بیت_صفحه_t::ELT_بیتها : INVALID;
      uint64_t excluded = 0;
      إذا (!s.بعدی_بیتها (&cursor, &excluded) || cursor != پایه)
	excluded = 0;
      *بیتها = ~excluded;
      منطقي آخرین = unlikely (پایه >= INVALID - hb_بیت_صفحه_t::ELT_بیتها + 1);
      إذا (آخرین)
	*بیتها &= UINT64_MAX >> 1;
      إذا (*بیتها)
      {
	*codepoint = پایه;
	أرجع درست;
      }
      إذا (unlikely (آخرین))
	اكسر;
      پایه += hb_بیت_صفحه_t::ELT_بیتها;
    }

  انجامشده:
    *codepoint = INVALID;
    *بیتها = 0;
    أرجع خطأ;
  }
  منطقي قبلی (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.قبلی (codepoint);

    خودکار old = *codepoint;
    إذا (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    خودکار v = old;
    s.قبلی (&v);

    إذا (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      أرجع درست;
    }

    v = old;
    s.قبلی_range (&v, &old);

    *codepoint = v - 1;
    أرجع *codepoint != INVALID;
  }
  منطقي بعدی_range (hb_codepoint_t *اولی, hb_codepoint_t *آخرین) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.بعدی_range (اولی, آخرین);

    إذا (!next (آخرین))
    {
      *آخرین = *اولی = INVALID;
      أرجع خطأ;
    }

    *اولی = *آخرین;
    s.next (آخرین);
    --*آخرین;
    أرجع درست;
  }
  منطقي قبلی_range (hb_codepoint_t *اولی, hb_codepoint_t *آخرین) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.قبلی_range (اولی, آخرین);

    إذا (!قبلی (اولی))
    {
      *آخرین = *اولی = INVALID;
      أرجع خطأ;
    }

    *آخرین = *اولی;
    s.قبلی (اولی);
    ++*اولی;
    أرجع درست;
  }

  بیعلامت صحيح بعدی_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *بیرون,
			  بیعلامت صحيح    اندازه) ثابت
  {
    أرجع inverted ? s.بعدی_many_inverted (codepoint, بیرون, اندازه)
		    : s.بعدی_many (codepoint, بیرون, اندازه);
  }

  ساكن constexpr hb_codepoint_t INVALID = hb_بیت_مجموعه_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = درست;
    ساكن constexpr منطقي has_fast_len = درست;
    iter_t (ثابت hb_بیت_مجموعه_invertible_t &s_ = Null (hb_بیت_مجموعه_invertible_t),
    منطقي init = درست) : s (&s_), v (INVALID), مانده (0),
				     have_len (!init), پایه (INVALID), بیتها (0)
    {
      إذا (init)
	advance ();
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { أرجع v; }
    منطقي __more__ () ثابت { أرجع v != INVALID; }
    فراغ __next__ ()
    {
      مانده--;
      advance ();
    }
    فراغ __prev__ ()
    {
      إذا (s->قبلی (&v))
      {
	متزامن ();
	مانده++;
      }
      وإلا
      {
	پایه = INVALID;
	بیتها = 0;
      }
    }
    بیعلامت __len__ () ثابت
    {
      إذا (unlikely (!have_len))
      {
	مانده += s->get_population ();
	have_len = درست;
      }
      أرجع مانده;
    }
    iter_t انتها () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي عملگر != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_بیت_مجموعه_invertible_t *s;
    hb_codepoint_t v;
    تغییرپذیر بیعلامت مانده;
    تغییرپذیر منطقي have_len;
    hb_codepoint_t پایه;
    uint64_t بیتها;

    فراغ advance ()
    {
      إذا (likely (بیتها))
      {
	v = پایه + hb_ctz (بیتها);
	بیتها &= بیتها - 1;
      }
      وإلا إذا (s->بعدی_بیتها (&پایه, &بیتها))
      {
	v = پایه + hb_ctz (بیتها);
	بیتها &= بیتها - 1;
      }
      وإلا
	v = INVALID;
    }

    فراغ متزامن ()
    {
      پایه = v & ~hb_بیت_صفحه_t::ELT_ماسک;
      hb_codepoint_t cursor = پایه ? پایه - hb_بیت_صفحه_t::ELT_بیتها : INVALID;
      s->بعدی_بیتها (&cursor, &بیتها);
      بیعلامت صحيح bit = v & (hb_بیت_صفحه_t::ELT_بیتها - 1);
      بیتها = bit == hb_بیت_صفحه_t::ELT_بیتها - 1 ? 0 : بیتها >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  عملگر iter_t () ثابت { أرجع iter (); }
};


#پایان_اگر /* HB_BIT_SET_INVERTIBLE_HH */
