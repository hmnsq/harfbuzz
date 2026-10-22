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

#جيڪڏهن_وصف_نه_ٿيل HB_ٻٽو_انگ_صفحو_HH
#وصف_ڪريو HB_ٻٽو_انگ_صفحو_HH

#ضمن "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
سانچو <قسم_نالو elt_t, بنانشان صحيح byte_سائيز>
بنية hb_ويڪٽر_سائيز_t
{
  elt_t& عامل [] (بنانشان صحيح i) { أرجع v[i]; }
  ثابت elt_t& عامل [] (بنانشان صحيح i) ثابت { أرجع v[i]; }

  فراغ init0 ()
  {
    لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (v); i++)
      v[i] = 0;
  }
  فراغ init1 ()
  {
    لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (v); i++)
      v[i] = (elt_t) -1;
  }

  سانچو <قسم_نالو Op>
  hb_ويڪٽر_سائيز_t process (ثابت Op& op) ثابت
  {
    hb_ويڪٽر_سائيز_t r;
    لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (v); i++)
      r.v[i] = op (v[i]);
    أرجع r;
  }
  سانچو <قسم_نالو Op>
  hb_ويڪٽر_سائيز_t process (ثابت Op& op, ثابت hb_ويڪٽر_سائيز_t &o) ثابت
  {
    hb_ويڪٽر_سائيز_t r;
    لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    أرجع r;
  }
  hb_ويڪٽر_سائيز_t عامل | (ثابت hb_ويڪٽر_سائيز_t &o) ثابت
  { أرجع process (hb_bitwise_or, o); }
  hb_ويڪٽر_سائيز_t عامل & (ثابت hb_ويڪٽر_سائيز_t &o) ثابت
  { أرجع process (hb_bitwise_and, o); }
  hb_ويڪٽر_سائيز_t عامل ^ (ثابت hb_ويڪٽر_سائيز_t &o) ثابت
  { أرجع process (hb_bitwise_xor, o); }
  hb_ويڪٽر_سائيز_t عامل ~ () ثابت
  { أرجع process (hb_bitwise_neg); }

  عامل منطقي () ثابت
  {
    لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (v); i++)
      إذا (v[i])
	أرجع سچ;
    أرجع خطأ;
  }
  عامل بنانشان صحيح () ثابت
  {
    بنانشان صحيح r = 0;
    لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (v); i++)
      r += hb_popcount (v[i]);
    أرجع r;
  }
  منطقي عامل == (ثابت hb_ويڪٽر_سائيز_t &o) ثابت
  {
    لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (v); i++)
      إذا (v[i] != o.v[i])
	أرجع خطأ;
    أرجع سچ;
  }

  hb_array_t<ثابت elt_t> iter () ثابت
  { أرجع hb_array (v); }

  خاص:
  static_assert (0 == byte_سائيز % حجم (elt_t), "");
  elt_t v[byte_سائيز / حجم (elt_t)];
};


بنية hb_ٻٽو_انگ_صفحو_t
{
  hb_ٻٽو_انگ_صفحو_t () { init0 (); }

  فراغ init0 () { v.init0 (); population = 0; }
  فراغ init1 () { v.init1 (); population = صفحو_بٽ; }

  فراغ dirty () { population = UINT_MAX; }

  ساكن مضمن constexpr بنانشان len ()
  { أرجع ARRAY_ڊيگهائي_CONST (v); }

  عامل منطقي () ثابت { أرجع !is_empty (); }
  منطقي is_empty () ثابت
  {
    إذا (has_population ()) أرجع !population;
    منطقي خالي_جايون_2 = !v;
    إذا (خالي_جايون_2) population = 0;
    أرجع خالي_جايون_2;
  }
  uint32_t hash () ثابت
  {
    أرجع hb_بائيٽون_t ((ثابت حرف *) &v, حجم (v)).hash ();
  }

  فراغ جوڙيو (hb_codepoint_t g) { elt (g) |= نقاب (g); dirty (); }
  فراغ جوڙيو_بٽ (hb_codepoint_t g, uint64_t بٽ) { elt (g) |= بٽ; dirty (); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~نقاب (g); dirty (); }
  فراغ del_بٽ (hb_codepoint_t g, uint64_t بٽ) { elt (g) &= ~بٽ; dirty (); }
  فراغ سيٽ (hb_codepoint_t g, منطقي value) { إذا (value) جوڙيو (g); وإلا del (g); }
  منطقي get (hb_codepoint_t g) ثابت { أرجع elt (g) & نقاب (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  منطقي عامل [] (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي عامل () (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي has (hb_codepoint_t g) ثابت { أرجع get (g); }

  فراغ جوڙيو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    إذا (la == lb)
      *la |= (نقاب (b) << 1) - نقاب(a);
    وإلا إذا (likely (la < lb))
    {
      *la |= ~(نقاب (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (حرف *) lb - (حرف *) la);

      *lb |= ((نقاب (b) << 1) - 1llu);
    }
    وإلا
      أرجع;
    dirty ();
  }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    إذا (la == lb)
      *la &= ~((نقاب (b) << 1llu) - نقاب(a));
    وإلا إذا (likely (la < lb))
    {
      *la &= نقاب (a) - 1;
      la++;

      hb_memset (la, 0, (حرف *) lb - (حرف *) la);

      *lb &= ~((نقاب (b) << 1) - 1llu);
    }
    وإلا
      أرجع;
    dirty ();
  }
  فراغ سيٽ_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { إذا (v) جوڙيو_range (a, b); وإلا del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  بنانشان صحيح اكتب (uint32_t        بنياد,
		      بنانشان صحيح    شروعات_قدر,
		      hb_codepoint_t *p,
		      بنانشان صحيح    سائيز) ثابت
  {
    بنانشان صحيح شروعات_v = شروعات_قدر / ELT_بٽ;
    بنانشان صحيح شروعات_ٻٽو_انگ = شروعات_قدر & ELT_نقاب;
    بنانشان صحيح ڳڻپ = 0;
    لكل (بنانشان i = شروعات_v; i < len () && ڳڻپ < سائيز; i++)
    {
      elt_t بٽ = v[i];
      uint32_t v_بنياد = بنياد | (i * ELT_بٽ);
      لكل (بنانشان صحيح j = شروعات_ٻٽو_انگ; j < ELT_بٽ && ڳڻپ < سائيز; j++)
      {
	إذا ((elt_t(1) << j) & بٽ) {
	  *p++ = v_بنياد | j;
	  ڳڻپ++;
	}
      }
      شروعات_ٻٽو_انگ = 0;
    }
    أرجع ڳڻپ;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  بنانشان صحيح write_inverted (uint32_t        بنياد,
			       بنانشان صحيح    شروعات_قدر,
			       hb_codepoint_t *p,
			       بنانشان صحيح    سائيز,
			       hb_codepoint_t *ايندڙ_قدر) ثابت
  {
    بنانشان صحيح شروعات_v = شروعات_قدر / ELT_بٽ;
    بنانشان صحيح شروعات_ٻٽو_انگ = شروعات_قدر & ELT_نقاب;
    بنانشان صحيح ڳڻپ = 0;
    لكل (بنانشان i = شروعات_v; i < len () && ڳڻپ < سائيز; i++)
    {
      elt_t بٽ = v[i];
      uint32_t v_وٿي = i * ELT_بٽ;
      لكل (بنانشان صحيح j = شروعات_ٻٽو_انگ; j < ELT_بٽ && ڳڻپ < سائيز; j++)
      {
	إذا ((elt_t(1) << j) & بٽ)
	{
	  hb_codepoint_t value = بنياد | v_وٿي | j;
	  // Emit all the missing values from next_value up to value - 1.
	  لكل (hb_codepoint_t k = *ايندڙ_قدر; k < value && ڳڻپ < سائيز; k++)
	  {
	    *p++ = k;
	    ڳڻپ++;
	  }
	  // Skip over this value;
	  *ايندڙ_قدر = value + 1;
	}
      }
      شروعات_ٻٽو_انگ = 0;
    }
    أرجع ڳڻپ;
  }

  منطقي عامل == (ثابت hb_ٻٽو_انگ_صفحو_t &other) ثابت { أرجع is_equal (other); }
  منطقي is_equal (ثابت hb_ٻٽو_انگ_صفحو_t &other) ثابت { أرجع v == other.v; }
  منطقي intersects (ثابت hb_ٻٽو_انگ_صفحو_t &other) ثابت
  {
    لكل (بنانشان i = 0; i < len (); i++)
      إذا (v[i] & other.v[i])
	أرجع سچ;
    أرجع خطأ;
  }
  منطقي may_intersect (ثابت hb_ٻٽو_انگ_صفحو_t &other) ثابت
  { أرجع intersects (other); }

  منطقي عامل <= (ثابت hb_ٻٽو_انگ_صفحو_t &larger_صفحو) ثابت { أرجع is_subset (larger_صفحو); }
  منطقي is_subset (ثابت hb_ٻٽو_انگ_صفحو_t &larger_صفحو) ثابت
  {
    إذا (has_population () && larger_صفحو.has_population () &&
	population > larger_صفحو.population)
      أرجع خطأ;

    لكل (بنانشان i = 0; i < len (); i++)
      إذا (~larger_صفحو.v[i] & v[i])
	أرجع خطأ;
    أرجع سچ;
  }

  منطقي has_population () ثابت { أرجع population != UINT_MAX; }
  بنانشان get_population () ثابت
  {
    إذا (has_population ()) أرجع population;
    أرجع population = v;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    بنانشان صحيح m = (*codepoint + 1) & نقاب_2;
    إذا (!m)
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }
    بنانشان صحيح i = m / ELT_بٽ;
    بنانشان صحيح j = m & ELT_نقاب;

    ثابت elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    لكل (ثابت elt_t *p = &vv; i < len (); p = ((ثابت elt_t *) &v[0]) + (++i))
      إذا (*p)
      {
	*codepoint = i * ELT_بٽ + elt_get_min (*p);
	أرجع سچ;
      }

    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي اڳيون (hb_codepoint_t *codepoint) ثابت
  {
    بنانشان صحيح m = (*codepoint - 1) & نقاب_2;
    إذا (m == نقاب_2)
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }
    بنانشان صحيح i = m / ELT_بٽ;
    بنانشان صحيح j = m & ELT_نقاب;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ثابت elt_t نقاب = j < 8 * حجم (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ثابت elt_t vv = v[i] & نقاب;
    ثابت elt_t *p = &vv;
    طالما (سچ)
    {
      إذا (*p)
      {
	*codepoint = i * ELT_بٽ + elt_get_max (*p);
	أرجع سچ;
      }
      إذا ((صحيح) i <= 0) اكسر;
      p = &v[--i];
    }

    *codepoint = INVALID;
    أرجع خطأ;
  }
  hb_codepoint_t get_min () ثابت
  {
    لكل (بنانشان صحيح i = 0; i < len (); i++)
      إذا (v[i])
	أرجع i * ELT_بٽ + elt_get_min (v[i]);
    أرجع INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    لكل (صحيح i = len () - 1; i >= 0; i--)
      إذا (v[i])
	أرجع i * ELT_بٽ + elt_get_max (v[i]);
    أرجع INVALID;
  }

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = سچ;
    iter_t (ثابت hb_ٻٽو_انگ_صفحو_t &s_ = Null (hb_ٻٽو_انگ_صفحو_t), منطقي init = سچ) : s (&s_), v (INVALID)
    {
      إذا (init)
	v = s->get_min ();
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { أرجع v; }
    منطقي __more__ () ثابت { أرجع v != INVALID; }
    فراغ __next__ () {
       s->next (&v);
    }
    فراغ __prev__ () { s->اڳيون (&v); }
    iter_t پڇاڙي () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_ٻٽو_انگ_صفحو_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  عامل iter_t () ثابت { أرجع iter (); }

  ساكن constexpr hb_codepoint_t INVALID = HB_سيٽ_قدر_INVALID;

  تعريفنوع بنانشان ڊگهو ڊگهو elt_t;
  ساكن constexpr بنانشان صفحو_بٽ_LOG_2 = 9; // 512 bits
  ساكن constexpr بنانشان صفحو_بٽ = 1 << صفحو_بٽ_LOG_2;
  static_assert (1 << صفحو_بٽ_LOG_2 == صفحو_بٽ, "");
  static_assert ((صفحو_بٽ & ((صفحو_بٽ) - 1)) == 0, "");
  ساكن constexpr بنانشان صفحو_BITMASK = صفحو_بٽ - 1;

  ساكن بنانشان صحيح elt_get_min (ثابت elt_t &elt) { أرجع hb_ctz (elt); }
  ساكن بنانشان صحيح elt_get_max (ثابت elt_t &elt) { أرجع hb_ٻٽو_انگ_storage (elt) - 1; }

  تعريفنوع hb_ويڪٽر_سائيز_t<elt_t, صفحو_بٽ / 8> ويڪٽر_t;

  ساكن constexpr بنانشان ELT_بٽ = حجم (elt_t) * 8;
  ساكن constexpr بنانشان ELT_نقاب = ELT_بٽ - 1;

  ساكن constexpr بنانشان بٽ_2 = حجم (ويڪٽر_t) * 8;
  ساكن constexpr بنانشان نقاب_2 = بٽ_2 - 1;
  static_assert ((بنانشان) صفحو_بٽ == (بنانشان) بٽ_2, "");

  elt_t &elt (hb_codepoint_t g) { أرجع v[(g & نقاب_2) / ELT_بٽ]; }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت { أرجع v[(g & نقاب_2) / ELT_بٽ]; }
  ساكن constexpr elt_t نقاب (hb_codepoint_t g) { أرجع elt_t (1) << (g & ELT_نقاب); }

  بدلجندڙ بنانشان population;
  ويڪٽر_t v;
};


#جيڪڏهن_پورو /* HB_BIT_PAGE_HH */
