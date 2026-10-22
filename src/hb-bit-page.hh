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

#جے_بنا_تعریف HB_ثنائی_ہندسہ_ورقہ_HH
#تعریف_کرو HB_ثنائی_ہندسہ_ورقہ_HH

#ضمن "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
سانچا <قسم_ناں elt_t, بنانشانی صحيح byte_ناپ>
بنية hb_سمتیہ_ناپ_t
{
  elt_t& عامل [] (بنانشانی صحيح i) { أرجع v[i]; }
  ثابت elt_t& عامل [] (بنانشانی صحيح i) ثابت { أرجع v[i]; }

  فراغ init0 ()
  {
    لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (v); i++)
      v[i] = 0;
  }
  فراغ init1 ()
  {
    لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (v); i++)
      v[i] = (elt_t) -1;
  }

  سانچا <قسم_ناں Op>
  hb_سمتیہ_ناپ_t process (ثابت Op& op) ثابت
  {
    hb_سمتیہ_ناپ_t r;
    لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (v); i++)
      r.v[i] = op (v[i]);
    أرجع r;
  }
  سانچا <قسم_ناں Op>
  hb_سمتیہ_ناپ_t process (ثابت Op& op, ثابت hb_سمتیہ_ناپ_t &o) ثابت
  {
    hb_سمتیہ_ناپ_t r;
    لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    أرجع r;
  }
  hb_سمتیہ_ناپ_t عامل | (ثابت hb_سمتیہ_ناپ_t &o) ثابت
  { أرجع process (hb_bitwise_or, o); }
  hb_سمتیہ_ناپ_t عامل & (ثابت hb_سمتیہ_ناپ_t &o) ثابت
  { أرجع process (hb_bitwise_and, o); }
  hb_سمتیہ_ناپ_t عامل ^ (ثابت hb_سمتیہ_ناپ_t &o) ثابت
  { أرجع process (hb_bitwise_xor, o); }
  hb_سمتیہ_ناپ_t عامل ~ () ثابت
  { أرجع process (hb_bitwise_neg); }

  عامل منطقي () ثابت
  {
    لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (v); i++)
      إذا (v[i])
	أرجع سچ;
    أرجع خطأ;
  }
  عامل بنانشانی صحيح () ثابت
  {
    بنانشانی صحيح r = 0;
    لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (v); i++)
      r += hb_popcount (v[i]);
    أرجع r;
  }
  منطقي عامل == (ثابت hb_سمتیہ_ناپ_t &o) ثابت
  {
    لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (v); i++)
      إذا (v[i] != o.v[i])
	أرجع خطأ;
    أرجع سچ;
  }

  hb_array_t<ثابت elt_t> iter () ثابت
  { أرجع hb_array (v); }

  خاص:
  static_assert (0 == byte_ناپ % حجم (elt_t), "");
  elt_t v[byte_ناپ / حجم (elt_t)];
};


بنية hb_ثنائی_ہندسہ_ورقہ_t
{
  hb_ثنائی_ہندسہ_ورقہ_t () { init0 (); }

  فراغ init0 () { v.init0 (); population = 0; }
  فراغ init1 () { v.init1 (); population = ورقہ_بٹ; }

  فراغ dirty () { population = UINT_MAX; }

  ساكن مضمن constexpr بنانشانی len ()
  { أرجع ARRAY_لمائی_CONST (v); }

  عامل منطقي () ثابت { أرجع !is_empty (); }
  منطقي is_empty () ثابت
  {
    إذا (has_population ()) أرجع !population;
    منطقي خالی_تھاواں_2 = !v;
    إذا (خالی_تھاواں_2) population = 0;
    أرجع خالی_تھاواں_2;
  }
  uint32_t hash () ثابت
  {
    أرجع hb_بائٹاں_t ((ثابت حرف *) &v, حجم (v)).hash ();
  }

  فراغ جوڑو (hb_codepoint_t g) { elt (g) |= نقاب (g); dirty (); }
  فراغ جوڑو_بٹ (hb_codepoint_t g, uint64_t بٹ) { elt (g) |= بٹ; dirty (); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~نقاب (g); dirty (); }
  فراغ del_بٹ (hb_codepoint_t g, uint64_t بٹ) { elt (g) &= ~بٹ; dirty (); }
  فراغ سیٹ (hb_codepoint_t g, منطقي value) { إذا (value) جوڑو (g); وإلا del (g); }
  منطقي get (hb_codepoint_t g) ثابت { أرجع elt (g) & نقاب (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  منطقي عامل [] (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي عامل () (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي has (hb_codepoint_t g) ثابت { أرجع get (g); }

  فراغ جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
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
  فراغ سیٹ_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { إذا (v) جوڑو_range (a, b); وإلا del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  بنانشانی صحيح اكتب (uint32_t        ادھار,
		      بنانشانی صحيح    شروع_قدر,
		      hb_codepoint_t *p,
		      بنانشانی صحيح    ناپ) ثابت
  {
    بنانشانی صحيح شروع_v = شروع_قدر / ELT_بٹ;
    بنانشانی صحيح شروع_ثنائی_ہندسہ = شروع_قدر & ELT_نقاب;
    بنانشانی صحيح گنتی = 0;
    لكل (بنانشانی i = شروع_v; i < len () && گنتی < ناپ; i++)
    {
      elt_t بٹ = v[i];
      uint32_t v_ادھار = ادھار | (i * ELT_بٹ);
      لكل (بنانشانی صحيح j = شروع_ثنائی_ہندسہ; j < ELT_بٹ && گنتی < ناپ; j++)
      {
	إذا ((elt_t(1) << j) & بٹ) {
	  *p++ = v_ادھار | j;
	  گنتی++;
	}
      }
      شروع_ثنائی_ہندسہ = 0;
    }
    أرجع گنتی;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  بنانشانی صحيح write_inverted (uint32_t        ادھار,
			       بنانشانی صحيح    شروع_قدر,
			       hb_codepoint_t *p,
			       بنانشانی صحيح    ناپ,
			       hb_codepoint_t *اگلا_قدر) ثابت
  {
    بنانشانی صحيح شروع_v = شروع_قدر / ELT_بٹ;
    بنانشانی صحيح شروع_ثنائی_ہندسہ = شروع_قدر & ELT_نقاب;
    بنانشانی صحيح گنتی = 0;
    لكل (بنانشانی i = شروع_v; i < len () && گنتی < ناپ; i++)
    {
      elt_t بٹ = v[i];
      uint32_t v_وتھ = i * ELT_بٹ;
      لكل (بنانشانی صحيح j = شروع_ثنائی_ہندسہ; j < ELT_بٹ && گنتی < ناپ; j++)
      {
	إذا ((elt_t(1) << j) & بٹ)
	{
	  hb_codepoint_t value = ادھار | v_وتھ | j;
	  // Emit all the missing values from next_value up to value - 1.
	  لكل (hb_codepoint_t k = *اگلا_قدر; k < value && گنتی < ناپ; k++)
	  {
	    *p++ = k;
	    گنتی++;
	  }
	  // Skip over this value;
	  *اگلا_قدر = value + 1;
	}
      }
      شروع_ثنائی_ہندسہ = 0;
    }
    أرجع گنتی;
  }

  منطقي عامل == (ثابت hb_ثنائی_ہندسہ_ورقہ_t &other) ثابت { أرجع is_equal (other); }
  منطقي is_equal (ثابت hb_ثنائی_ہندسہ_ورقہ_t &other) ثابت { أرجع v == other.v; }
  منطقي intersects (ثابت hb_ثنائی_ہندسہ_ورقہ_t &other) ثابت
  {
    لكل (بنانشانی i = 0; i < len (); i++)
      إذا (v[i] & other.v[i])
	أرجع سچ;
    أرجع خطأ;
  }
  منطقي may_intersect (ثابت hb_ثنائی_ہندسہ_ورقہ_t &other) ثابت
  { أرجع intersects (other); }

  منطقي عامل <= (ثابت hb_ثنائی_ہندسہ_ورقہ_t &larger_ورقہ) ثابت { أرجع is_subset (larger_ورقہ); }
  منطقي is_subset (ثابت hb_ثنائی_ہندسہ_ورقہ_t &larger_ورقہ) ثابت
  {
    إذا (has_population () && larger_ورقہ.has_population () &&
	population > larger_ورقہ.population)
      أرجع خطأ;

    لكل (بنانشانی i = 0; i < len (); i++)
      إذا (~larger_ورقہ.v[i] & v[i])
	أرجع خطأ;
    أرجع سچ;
  }

  منطقي has_population () ثابت { أرجع population != UINT_MAX; }
  بنانشانی get_population () ثابت
  {
    إذا (has_population ()) أرجع population;
    أرجع population = v;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    بنانشانی صحيح m = (*codepoint + 1) & نقاب_2;
    إذا (!m)
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }
    بنانشانی صحيح i = m / ELT_بٹ;
    بنانشانی صحيح j = m & ELT_نقاب;

    ثابت elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    لكل (ثابت elt_t *p = &vv; i < len (); p = ((ثابت elt_t *) &v[0]) + (++i))
      إذا (*p)
      {
	*codepoint = i * ELT_بٹ + elt_get_min (*p);
	أرجع سچ;
      }

    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي پچھلا (hb_codepoint_t *codepoint) ثابت
  {
    بنانشانی صحيح m = (*codepoint - 1) & نقاب_2;
    إذا (m == نقاب_2)
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }
    بنانشانی صحيح i = m / ELT_بٹ;
    بنانشانی صحيح j = m & ELT_نقاب;

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
	*codepoint = i * ELT_بٹ + elt_get_max (*p);
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
    لكل (بنانشانی صحيح i = 0; i < len (); i++)
      إذا (v[i])
	أرجع i * ELT_بٹ + elt_get_min (v[i]);
    أرجع INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    لكل (صحيح i = len () - 1; i >= 0; i--)
      إذا (v[i])
	أرجع i * ELT_بٹ + elt_get_max (v[i]);
    أرجع INVALID;
  }

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = سچ;
    iter_t (ثابت hb_ثنائی_ہندسہ_ورقہ_t &s_ = Null (hb_ثنائی_ہندسہ_ورقہ_t), منطقي init = سچ) : s (&s_), v (INVALID)
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
    فراغ __prev__ () { s->پچھلا (&v); }
    iter_t سرا () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_ثنائی_ہندسہ_ورقہ_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  عامل iter_t () ثابت { أرجع iter (); }

  ساكن constexpr hb_codepoint_t INVALID = HB_سیٹ_قدر_INVALID;

  تعريفنوع بنانشانی لما لما elt_t;
  ساكن constexpr بنانشانی ورقہ_بٹ_LOG_2 = 9; // 512 bits
  ساكن constexpr بنانشانی ورقہ_بٹ = 1 << ورقہ_بٹ_LOG_2;
  static_assert (1 << ورقہ_بٹ_LOG_2 == ورقہ_بٹ, "");
  static_assert ((ورقہ_بٹ & ((ورقہ_بٹ) - 1)) == 0, "");
  ساكن constexpr بنانشانی ورقہ_BITMASK = ورقہ_بٹ - 1;

  ساكن بنانشانی صحيح elt_get_min (ثابت elt_t &elt) { أرجع hb_ctz (elt); }
  ساكن بنانشانی صحيح elt_get_max (ثابت elt_t &elt) { أرجع hb_ثنائی_ہندسہ_storage (elt) - 1; }

  تعريفنوع hb_سمتیہ_ناپ_t<elt_t, ورقہ_بٹ / 8> سمتیہ_t;

  ساكن constexpr بنانشانی ELT_بٹ = حجم (elt_t) * 8;
  ساكن constexpr بنانشانی ELT_نقاب = ELT_بٹ - 1;

  ساكن constexpr بنانشانی بٹ_2 = حجم (سمتیہ_t) * 8;
  ساكن constexpr بنانشانی نقاب_2 = بٹ_2 - 1;
  static_assert ((بنانشانی) ورقہ_بٹ == (بنانشانی) بٹ_2, "");

  elt_t &elt (hb_codepoint_t g) { أرجع v[(g & نقاب_2) / ELT_بٹ]; }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت { أرجع v[(g & نقاب_2) / ELT_بٹ]; }
  ساكن constexpr elt_t نقاب (hb_codepoint_t g) { أرجع elt_t (1) << (g & ELT_نقاب); }

  بدلنجوگ بنانشانی population;
  سمتیہ_t v;
};


#جے_انت /* HB_BIT_PAGE_HH */
