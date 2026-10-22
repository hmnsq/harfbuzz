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

#که_نه_تعریف_شوی HB_بټ_پاڼه_HH
#تعریف_کړه HB_بټ_پاڼه_HH

#ضمن "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
کينډۍ <ډول_نوم elt_t, بینښه صحيح byte_اندازه>
بنية hb_ویکټور_اندازه_t
{
  elt_t& چلوونکی [] (بینښه صحيح i) { أرجع v[i]; }
  ثابت elt_t& چلوونکی [] (بینښه صحيح i) ثابت { أرجع v[i]; }

  فراغ init0 ()
  {
    لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (v); i++)
      v[i] = 0;
  }
  فراغ init1 ()
  {
    لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (v); i++)
      v[i] = (elt_t) -1;
  }

  کينډۍ <ډول_نوم Op>
  hb_ویکټور_اندازه_t process (ثابت Op& op) ثابت
  {
    hb_ویکټور_اندازه_t r;
    لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (v); i++)
      r.v[i] = op (v[i]);
    أرجع r;
  }
  کينډۍ <ډول_نوم Op>
  hb_ویکټور_اندازه_t process (ثابت Op& op, ثابت hb_ویکټور_اندازه_t &o) ثابت
  {
    hb_ویکټور_اندازه_t r;
    لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    أرجع r;
  }
  hb_ویکټور_اندازه_t چلوونکی | (ثابت hb_ویکټور_اندازه_t &o) ثابت
  { أرجع process (hb_bitwise_or, o); }
  hb_ویکټور_اندازه_t چلوونکی & (ثابت hb_ویکټور_اندازه_t &o) ثابت
  { أرجع process (hb_bitwise_and, o); }
  hb_ویکټور_اندازه_t چلوونکی ^ (ثابت hb_ویکټور_اندازه_t &o) ثابت
  { أرجع process (hb_bitwise_xor, o); }
  hb_ویکټور_اندازه_t چلوونکی ~ () ثابت
  { أرجع process (hb_bitwise_neg); }

  چلوونکی منطقي () ثابت
  {
    لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (v); i++)
      إذا (v[i])
	أرجع ریښتیا;
    أرجع خطأ;
  }
  چلوونکی بینښه صحيح () ثابت
  {
    بینښه صحيح r = 0;
    لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (v); i++)
      r += hb_popcount (v[i]);
    أرجع r;
  }
  منطقي چلوونکی == (ثابت hb_ویکټور_اندازه_t &o) ثابت
  {
    لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (v); i++)
      إذا (v[i] != o.v[i])
	أرجع خطأ;
    أرجع ریښتیا;
  }

  hb_array_t<ثابت elt_t> iter () ثابت
  { أرجع hb_array (v); }

  خاص:
  static_assert (0 == byte_اندازه % حجم (elt_t), "");
  elt_t v[byte_اندازه / حجم (elt_t)];
};


بنية hb_بټ_پاڼه_t
{
  hb_بټ_پاڼه_t () { init0 (); }

  فراغ init0 () { v.init0 (); population = 0; }
  فراغ init1 () { v.init1 (); population = پاڼه_بټونه; }

  فراغ dirty () { population = UINT_MAX; }

  ساكن مضمن constexpr بینښه len ()
  { أرجع ARRAY_اوږدتیا_CONST (v); }

  چلوونکی منطقي () ثابت { أرجع !is_empty (); }
  منطقي is_empty () ثابت
  {
    إذا (has_population ()) أرجع !population;
    منطقي تشه_2 = !v;
    إذا (تشه_2) population = 0;
    أرجع تشه_2;
  }
  uint32_t hash () ثابت
  {
    أرجع hb_بایټونه_t ((ثابت حرف *) &v, حجم (v)).hash ();
  }

  فراغ جمع_کړه (hb_codepoint_t g) { elt (g) |= ماسک (g); dirty (); }
  فراغ جمع_کړه_بټونه (hb_codepoint_t g, uint64_t بټونه) { elt (g) |= بټونه; dirty (); }
  فراغ del (hb_codepoint_t g) { elt (g) &= ~ماسک (g); dirty (); }
  فراغ del_بټونه (hb_codepoint_t g, uint64_t بټونه) { elt (g) &= ~بټونه; dirty (); }
  فراغ سېټ (hb_codepoint_t g, منطقي value) { إذا (value) جمع_کړه (g); وإلا del (g); }
  منطقي get (hb_codepoint_t g) ثابت { أرجع elt (g) & ماسک (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  منطقي چلوونکی [] (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي چلوونکی () (hb_codepoint_t g) ثابت { أرجع get (g); }
  منطقي has (hb_codepoint_t g) ثابت { أرجع get (g); }

  فراغ جمع_کړه_range (hb_codepoint_t a, hb_codepoint_t b)
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
  فراغ سېټ_range (hb_codepoint_t a, hb_codepoint_t b, منطقي v)
  { إذا (v) جمع_کړه_range (a, b); وإلا del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  بینښه صحيح اكتب (uint32_t        بنسټ,
		      بینښه صحيح    پیل_ارزښت,
		      hb_codepoint_t *p,
		      بینښه صحيح    اندازه) ثابت
  {
    بینښه صحيح پیل_v = پیل_ارزښت / ELT_بټونه;
    بینښه صحيح پیل_بټ = پیل_ارزښت & ELT_ماسک;
    بینښه صحيح شمېر = 0;
    لكل (بینښه i = پیل_v; i < len () && شمېر < اندازه; i++)
    {
      elt_t بټونه = v[i];
      uint32_t v_بنسټ = بنسټ | (i * ELT_بټونه);
      لكل (بینښه صحيح j = پیل_بټ; j < ELT_بټونه && شمېر < اندازه; j++)
      {
	إذا ((elt_t(1) << j) & بټونه) {
	  *p++ = v_بنسټ | j;
	  شمېر++;
	}
      }
      پیل_بټ = 0;
    }
    أرجع شمېر;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  بینښه صحيح write_inverted (uint32_t        بنسټ,
			       بینښه صحيح    پیل_ارزښت,
			       hb_codepoint_t *p,
			       بینښه صحيح    اندازه,
			       hb_codepoint_t *بل_ارزښت) ثابت
  {
    بینښه صحيح پیل_v = پیل_ارزښت / ELT_بټونه;
    بینښه صحيح پیل_بټ = پیل_ارزښت & ELT_ماسک;
    بینښه صحيح شمېر = 0;
    لكل (بینښه i = پیل_v; i < len () && شمېر < اندازه; i++)
    {
      elt_t بټونه = v[i];
      uint32_t v_دننه_واټن = i * ELT_بټونه;
      لكل (بینښه صحيح j = پیل_بټ; j < ELT_بټونه && شمېر < اندازه; j++)
      {
	إذا ((elt_t(1) << j) & بټونه)
	{
	  hb_codepoint_t value = بنسټ | v_دننه_واټن | j;
	  // Emit all the missing values from next_value up to value - 1.
	  لكل (hb_codepoint_t k = *بل_ارزښت; k < value && شمېر < اندازه; k++)
	  {
	    *p++ = k;
	    شمېر++;
	  }
	  // Skip over this value;
	  *بل_ارزښت = value + 1;
	}
      }
      پیل_بټ = 0;
    }
    أرجع شمېر;
  }

  منطقي چلوونکی == (ثابت hb_بټ_پاڼه_t &other) ثابت { أرجع is_equal (other); }
  منطقي is_equal (ثابت hb_بټ_پاڼه_t &other) ثابت { أرجع v == other.v; }
  منطقي intersects (ثابت hb_بټ_پاڼه_t &other) ثابت
  {
    لكل (بینښه i = 0; i < len (); i++)
      إذا (v[i] & other.v[i])
	أرجع ریښتیا;
    أرجع خطأ;
  }
  منطقي may_intersect (ثابت hb_بټ_پاڼه_t &other) ثابت
  { أرجع intersects (other); }

  منطقي چلوونکی <= (ثابت hb_بټ_پاڼه_t &larger_پاڼه) ثابت { أرجع is_subset (larger_پاڼه); }
  منطقي is_subset (ثابت hb_بټ_پاڼه_t &larger_پاڼه) ثابت
  {
    إذا (has_population () && larger_پاڼه.has_population () &&
	population > larger_پاڼه.population)
      أرجع خطأ;

    لكل (بینښه i = 0; i < len (); i++)
      إذا (~larger_پاڼه.v[i] & v[i])
	أرجع خطأ;
    أرجع ریښتیا;
  }

  منطقي has_population () ثابت { أرجع population != UINT_MAX; }
  بینښه get_population () ثابت
  {
    إذا (has_population ()) أرجع population;
    أرجع population = v;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    بینښه صحيح m = (*codepoint + 1) & ماسک_2;
    إذا (!m)
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }
    بینښه صحيح i = m / ELT_بټونه;
    بینښه صحيح j = m & ELT_ماسک;

    ثابت elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    لكل (ثابت elt_t *p = &vv; i < len (); p = ((ثابت elt_t *) &v[0]) + (++i))
      إذا (*p)
      {
	*codepoint = i * ELT_بټونه + elt_get_min (*p);
	أرجع ریښتیا;
      }

    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي پخوانی (hb_codepoint_t *codepoint) ثابت
  {
    بینښه صحيح m = (*codepoint - 1) & ماسک_2;
    إذا (m == ماسک_2)
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }
    بینښه صحيح i = m / ELT_بټونه;
    بینښه صحيح j = m & ELT_ماسک;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ثابت elt_t ماسک = j < 8 * حجم (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ثابت elt_t vv = v[i] & ماسک;
    ثابت elt_t *p = &vv;
    طالما (ریښتیا)
    {
      إذا (*p)
      {
	*codepoint = i * ELT_بټونه + elt_get_max (*p);
	أرجع ریښتیا;
      }
      إذا ((صحيح) i <= 0) اكسر;
      p = &v[--i];
    }

    *codepoint = INVALID;
    أرجع خطأ;
  }
  hb_codepoint_t get_min () ثابت
  {
    لكل (بینښه صحيح i = 0; i < len (); i++)
      إذا (v[i])
	أرجع i * ELT_بټونه + elt_get_min (v[i]);
    أرجع INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    لكل (صحيح i = len () - 1; i >= 0; i--)
      إذا (v[i])
	أرجع i * ELT_بټونه + elt_get_max (v[i]);
    أرجع INVALID;
  }

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = ریښتیا;
    iter_t (ثابت hb_بټ_پاڼه_t &s_ = Null (hb_بټ_پاڼه_t), منطقي init = ریښتیا) : s (&s_), v (INVALID)
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
    فراغ __prev__ () { s->پخوانی (&v); }
    iter_t پای_ټکی () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي چلوونکی != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_بټ_پاڼه_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  چلوونکی iter_t () ثابت { أرجع iter (); }

  ساكن constexpr hb_codepoint_t INVALID = HB_سېټ_ارزښت_INVALID;

  تعريفنوع بینښه اوږد اوږد elt_t;
  ساكن constexpr بینښه پاڼه_بټونه_LOG_2 = 9; // 512 bits
  ساكن constexpr بینښه پاڼه_بټونه = 1 << پاڼه_بټونه_LOG_2;
  static_assert (1 << پاڼه_بټونه_LOG_2 == پاڼه_بټونه, "");
  static_assert ((پاڼه_بټونه & ((پاڼه_بټونه) - 1)) == 0, "");
  ساكن constexpr بینښه پاڼه_BITMASK = پاڼه_بټونه - 1;

  ساكن بینښه صحيح elt_get_min (ثابت elt_t &elt) { أرجع hb_ctz (elt); }
  ساكن بینښه صحيح elt_get_max (ثابت elt_t &elt) { أرجع hb_بټ_storage (elt) - 1; }

  تعريفنوع hb_ویکټور_اندازه_t<elt_t, پاڼه_بټونه / 8> ویکټور_t;

  ساكن constexpr بینښه ELT_بټونه = حجم (elt_t) * 8;
  ساكن constexpr بینښه ELT_ماسک = ELT_بټونه - 1;

  ساكن constexpr بینښه بټونه_2 = حجم (ویکټور_t) * 8;
  ساكن constexpr بینښه ماسک_2 = بټونه_2 - 1;
  static_assert ((بینښه) پاڼه_بټونه == (بینښه) بټونه_2, "");

  elt_t &elt (hb_codepoint_t g) { أرجع v[(g & ماسک_2) / ELT_بټونه]; }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت { أرجع v[(g & ماسک_2) / ELT_بټونه]; }
  ساكن constexpr elt_t ماسک (hb_codepoint_t g) { أرجع elt_t (1) << (g & ELT_ماسک); }

  بدلیدونکی بینښه population;
  ویکټور_t v;
};


#د_که_پای /* HB_BIT_PAGE_HH */
