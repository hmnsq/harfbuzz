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

#اگر_تعریف_نشده HB_بیت_صفحه_HH
#تعریف_کن HB_بیت_صفحه_HH

#ضمن "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
قالب <نام_نوع elt_t, بیعلامت صحيح byte_اندازه>
بنية hb_وکتور_اندازه_t
{
  elt_t& عملگر [] (بیعلامت صحيح i) { أرجع v[i]; }
  ثابت elt_t& عملگر [] (بیعلامت صحيح i) ثابت { أرجع v[i]; }

  فراغ init0 ()
  {
    لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (v); i++)
      v[i] = 0;
  }
  فراغ init1 ()
  {
    لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (v); i++)
      v[i] = (elt_t) -1;
  }

  قالب <نام_نوع Op>
  hb_وکتور_اندازه_t process (ثابت Op& op) ثابت
  {
    hb_وکتور_اندازه_t r;
    لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (v); i++)
      r.v[i] = op (v[i]);
    أرجع r;
  }
  قالب <نام_نوع Op>
  hb_وکتور_اندازه_t process (ثابت Op& op, ثابت hb_وکتور_اندازه_t &o) ثابت
  {
    hb_وکتور_اندازه_t r;
    لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    أرجع r;
  }
  hb_وکتور_اندازه_t عملگر | (ثابت hb_وکتور_اندازه_t &o) ثابت
  { أرجع process (hb_bitwise_or, o); }
  hb_وکتور_اندازه_t عملگر & (ثابت hb_وکتور_اندازه_t &o) ثابت
  { أرجع process (hb_bitwise_and, o); }
  hb_وکتور_اندازه_t عملگر ^ (ثابت hb_وکتور_اندازه_t &o) ثابت
  { أرجع process (hb_bitwise_xor, o); }
  hb_وکتور_اندازه_t عملگر ~ () ثابت
  { أرجع process (hb_bitwise_neg); }

  عملگر منطقي () ثابت
  {
    لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (v); i++)
      إذا (v[i])
	أرجع درست;
    أرجع خطأ;
  }
  عملگر بیعلامت صحيح () ثابت
  {
    بیعلامت صحيح r = 0;
    لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (v); i++)
      r += hb_popcount (v[i]);
    أرجع r;
  }
  منطقي عملگر == (ثابت hb_وکتور_اندازه_t &o) ثابت
  {
    لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (v); i++)
      إذا (v[i] != o.v[i])
	أرجع خطأ;
    أرجع درست;
  }

  hb_array_t<ثابت elt_t> iter () ثابت
  { أرجع hb_array (v); }

  خاص:
  static_assert (0 == byte_اندازه % حجم (elt_t), "");
  elt_t v[byte_اندازه / حجم (elt_t)];
};


بنية hb_بیت_صفحه_t
{
  hb_بیت_صفحه_t () { init0 (); }

  فراغ init0 () { v.init0 (); population = 0; }
  فراغ init1 () { v.init1 (); population = صفحه_بیتها; }

  فراغ dirty () { population = UINT_MAX; }

  ساكن مضمن constexpr بیعلامت len ()
  { أرجع ARRAY_درازی_CONST (v); }

  عملگر منطقي () ثابت { أرجع !is_empty (); }
  منطقي is_empty () ثابت
  {
    إذا (has_population ()) أرجع !population;
    منطقي خالی_2 = !v;
    إذا (خالی_2) population = 0;
    أرجع خالی_2;
  }
  uint32_t hash () ثابت
  {
    أرجع hb_بایتها_t ((ثابت حرف *) &v, حجم (v)).hash ();
  }

  فراغ جمع_کن (hb_codepoint_t g) { elt (g) |= ماسک (g); dirty (); }
  فراغ جمع_کن_بیتها (hb_codepoint_t g, uint64_t بیتها) { elt (g) |= بیتها; dirty (); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~ماسک (g); dirty (); }
  فراغ del_بیتها (hb_codepoint_t g, uint64_t بیتها) { elt (g) &= ~بیتها; dirty (); }
  فراغ مجموعه (hb_codepoint_t g, منطقي value) { إذا (value) جمع_کن (g); وإلا del (g); }
  منطقي get (hb_codepoint_t g) ثابت { أرجع elt (g) & ماسک (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  منطقي عملگر [] (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي عملگر () (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي has (hb_codepoint_t g) ثابت { أرجع get (g); }

  فراغ جمع_کن_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    إذا (la == lb)
      *la |= (ماسک (b) << 1) - ماسک(a);
    وإلا إذا (likely (la < lb))
    {
      *la |= ~(ماسک (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (حرف *) lb - (حرف *) la);

      *lb |= ((ماسک (b) << 1) - 1llu);
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
      *la &= ~((ماسک (b) << 1llu) - ماسک(a));
    وإلا إذا (likely (la < lb))
    {
      *la &= ماسک (a) - 1;
      la++;

      hb_memset (la, 0, (حرف *) lb - (حرف *) la);

      *lb &= ~((ماسک (b) << 1) - 1llu);
    }
    وإلا
      أرجع;
    dirty ();
  }
  فراغ مجموعه_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { إذا (v) جمع_کن_range (a, b); وإلا del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  بیعلامت صحيح اكتب (uint32_t        پایه,
		      بیعلامت صحيح    شروع_مقدار,
		      hb_codepoint_t *p,
		      بیعلامت صحيح    اندازه) ثابت
  {
    بیعلامت صحيح شروع_v = شروع_مقدار / ELT_بیتها;
    بیعلامت صحيح شروع_بیت = شروع_مقدار & ELT_ماسک;
    بیعلامت صحيح شمار = 0;
    لكل (بیعلامت i = شروع_v; i < len () && شمار < اندازه; i++)
    {
      elt_t بیتها = v[i];
      uint32_t v_پایه = پایه | (i * ELT_بیتها);
      لكل (بیعلامت صحيح j = شروع_بیت; j < ELT_بیتها && شمار < اندازه; j++)
      {
	إذا ((elt_t(1) << j) & بیتها) {
	  *p++ = v_پایه | j;
	  شمار++;
	}
      }
      شروع_بیت = 0;
    }
    أرجع شمار;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  بیعلامت صحيح write_inverted (uint32_t        پایه,
			       بیعلامت صحيح    شروع_مقدار,
			       hb_codepoint_t *p,
			       بیعلامت صحيح    اندازه,
			       hb_codepoint_t *بعدی_مقدار) ثابت
  {
    بیعلامت صحيح شروع_v = شروع_مقدار / ELT_بیتها;
    بیعلامت صحيح شروع_بیت = شروع_مقدار & ELT_ماسک;
    بیعلامت صحيح شمار = 0;
    لكل (بیعلامت i = شروع_v; i < len () && شمار < اندازه; i++)
    {
      elt_t بیتها = v[i];
      uint32_t v_جابه_جایی = i * ELT_بیتها;
      لكل (بیعلامت صحيح j = شروع_بیت; j < ELT_بیتها && شمار < اندازه; j++)
      {
	إذا ((elt_t(1) << j) & بیتها)
	{
	  hb_codepoint_t value = پایه | v_جابه_جایی | j;
	  // Emit all the missing values from next_value up to value - 1.
	  لكل (hb_codepoint_t k = *بعدی_مقدار; k < value && شمار < اندازه; k++)
	  {
	    *p++ = k;
	    شمار++;
	  }
	  // Skip over this value;
	  *بعدی_مقدار = value + 1;
	}
      }
      شروع_بیت = 0;
    }
    أرجع شمار;
  }

  منطقي عملگر == (ثابت hb_بیت_صفحه_t &other) ثابت { أرجع is_equal (other); }
  منطقي is_equal (ثابت hb_بیت_صفحه_t &other) ثابت { أرجع v == other.v; }
  منطقي intersects (ثابت hb_بیت_صفحه_t &other) ثابت
  {
    لكل (بیعلامت i = 0; i < len (); i++)
      إذا (v[i] & other.v[i])
	أرجع درست;
    أرجع خطأ;
  }
  منطقي may_intersect (ثابت hb_بیت_صفحه_t &other) ثابت
  { أرجع intersects (other); }

  منطقي عملگر <= (ثابت hb_بیت_صفحه_t &larger_صفحه) ثابت { أرجع is_subset (larger_صفحه); }
  منطقي is_subset (ثابت hb_بیت_صفحه_t &larger_صفحه) ثابت
  {
    إذا (has_population () && larger_صفحه.has_population () &&
	population > larger_صفحه.population)
      أرجع خطأ;

    لكل (بیعلامت i = 0; i < len (); i++)
      إذا (~larger_صفحه.v[i] & v[i])
	أرجع خطأ;
    أرجع درست;
  }

  منطقي has_population () ثابت { أرجع population != UINT_MAX; }
  بیعلامت get_population () ثابت
  {
    إذا (has_population ()) أرجع population;
    أرجع population = v;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    بیعلامت صحيح m = (*codepoint + 1) & ماسک_2;
    إذا (!m)
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }
    بیعلامت صحيح i = m / ELT_بیتها;
    بیعلامت صحيح j = m & ELT_ماسک;

    ثابت elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    لكل (ثابت elt_t *p = &vv; i < len (); p = ((ثابت elt_t *) &v[0]) + (++i))
      إذا (*p)
      {
	*codepoint = i * ELT_بیتها + elt_get_min (*p);
	أرجع درست;
      }

    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي قبلی (hb_codepoint_t *codepoint) ثابت
  {
    بیعلامت صحيح m = (*codepoint - 1) & ماسک_2;
    إذا (m == ماسک_2)
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }
    بیعلامت صحيح i = m / ELT_بیتها;
    بیعلامت صحيح j = m & ELT_ماسک;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ثابت elt_t ماسک = j < 8 * حجم (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ثابت elt_t vv = v[i] & ماسک;
    ثابت elt_t *p = &vv;
    طالما (درست)
    {
      إذا (*p)
      {
	*codepoint = i * ELT_بیتها + elt_get_max (*p);
	أرجع درست;
      }
      إذا ((صحيح) i <= 0) اكسر;
      p = &v[--i];
    }

    *codepoint = INVALID;
    أرجع خطأ;
  }
  hb_codepoint_t get_min () ثابت
  {
    لكل (بیعلامت صحيح i = 0; i < len (); i++)
      إذا (v[i])
	أرجع i * ELT_بیتها + elt_get_min (v[i]);
    أرجع INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    لكل (صحيح i = len () - 1; i >= 0; i--)
      إذا (v[i])
	أرجع i * ELT_بیتها + elt_get_max (v[i]);
    أرجع INVALID;
  }

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = درست;
    iter_t (ثابت hb_بیت_صفحه_t &s_ = Null (hb_بیت_صفحه_t), منطقي init = درست) : s (&s_), v (INVALID)
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
    فراغ __prev__ () { s->قبلی (&v); }
    iter_t انتها () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي عملگر != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_بیت_صفحه_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  عملگر iter_t () ثابت { أرجع iter (); }

  ساكن constexpr hb_codepoint_t INVALID = HB_مجموعه_مقدار_INVALID;

  تعريفنوع بیعلامت دراز دراز elt_t;
  ساكن constexpr بیعلامت صفحه_بیتها_LOG_2 = 9; // 512 bits
  ساكن constexpr بیعلامت صفحه_بیتها = 1 << صفحه_بیتها_LOG_2;
  static_assert (1 << صفحه_بیتها_LOG_2 == صفحه_بیتها, "");
  static_assert ((صفحه_بیتها & ((صفحه_بیتها) - 1)) == 0, "");
  ساكن constexpr بیعلامت صفحه_BITMASK = صفحه_بیتها - 1;

  ساكن بیعلامت صحيح elt_get_min (ثابت elt_t &elt) { أرجع hb_ctz (elt); }
  ساكن بیعلامت صحيح elt_get_max (ثابت elt_t &elt) { أرجع hb_بیت_storage (elt) - 1; }

  تعريفنوع hb_وکتور_اندازه_t<elt_t, صفحه_بیتها / 8> وکتور_t;

  ساكن constexpr بیعلامت ELT_بیتها = حجم (elt_t) * 8;
  ساكن constexpr بیعلامت ELT_ماسک = ELT_بیتها - 1;

  ساكن constexpr بیعلامت بیتها_2 = حجم (وکتور_t) * 8;
  ساكن constexpr بیعلامت ماسک_2 = بیتها_2 - 1;
  static_assert ((بیعلامت) صفحه_بیتها == (بیعلامت) بیتها_2, "");

  elt_t &elt (hb_codepoint_t g) { أرجع v[(g & ماسک_2) / ELT_بیتها]; }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت { أرجع v[(g & ماسک_2) / ELT_بیتها]; }
  ساكن constexpr elt_t ماسک (hb_codepoint_t g) { أرجع elt_t (1) << (g & ELT_ماسک); }

  تغییرپذیر بیعلامت population;
  وکتور_t v;
};


#پایان_اگر /* HB_BIT_PAGE_HH */
