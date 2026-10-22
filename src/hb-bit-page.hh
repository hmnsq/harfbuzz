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

#اگر_غیر_تعریف_شدہ HB_ثنائی_ہندسہ_صفحہ_HH
#تعریف_کرو HB_ثنائی_ہندسہ_صفحہ_HH

#شامل "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
سانچہ <قسم_نام elt_t, غیرعلامتی صحیح byte_سائز>
ساخت hb_سمتیہ_سائز_t
{
  elt_t& عامل [] (غیرعلامتی صحیح i) { واپس v[i]; }
  ثابت elt_t& عامل [] (غیرعلامتی صحیح i) ثابت { واپس v[i]; }

  خالی init0 ()
  {
    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (v); i++)
      v[i] = 0;
  }
  خالی init1 ()
  {
    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (v); i++)
      v[i] = (elt_t) -1;
  }

  سانچہ <قسم_نام Op>
  hb_سمتیہ_سائز_t process (ثابت Op& op) ثابت
  {
    hb_سمتیہ_سائز_t r;
    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (v); i++)
      r.v[i] = op (v[i]);
    واپس r;
  }
  سانچہ <قسم_نام Op>
  hb_سمتیہ_سائز_t process (ثابت Op& op, ثابت hb_سمتیہ_سائز_t &o) ثابت
  {
    hb_سمتیہ_سائز_t r;
    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    واپس r;
  }
  hb_سمتیہ_سائز_t عامل | (ثابت hb_سمتیہ_سائز_t &o) ثابت
  { واپس process (hb_bitwise_or, o); }
  hb_سمتیہ_سائز_t عامل & (ثابت hb_سمتیہ_سائز_t &o) ثابت
  { واپس process (hb_bitwise_and, o); }
  hb_سمتیہ_سائز_t عامل ^ (ثابت hb_سمتیہ_سائز_t &o) ثابت
  { واپس process (hb_bitwise_xor, o); }
  hb_سمتیہ_سائز_t عامل ~ () ثابت
  { واپس process (hb_bitwise_neg); }

  عامل منطقی () ثابت
  {
    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (v); i++)
      اگر (v[i])
	واپس سچ;
    واپس جھوٹ;
  }
  عامل غیرعلامتی صحیح () ثابت
  {
    غیرعلامتی صحیح r = 0;
    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (v); i++)
      r += hb_popcount (v[i]);
    واپس r;
  }
  منطقی عامل == (ثابت hb_سمتیہ_سائز_t &o) ثابت
  {
    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (v); i++)
      اگر (v[i] != o.v[i])
	واپس جھوٹ;
    واپس سچ;
  }

  hb_array_t<ثابت elt_t> iter () ثابت
  { واپس hb_array (v); }

  نجی:
  static_assert (0 == byte_سائز % حجم (elt_t), "");
  elt_t v[byte_سائز / حجم (elt_t)];
};


ساخت hb_ثنائی_ہندسہ_صفحہ_t
{
  hb_ثنائی_ہندسہ_صفحہ_t () { init0 (); }

  خالی init0 () { v.init0 (); population = 0; }
  خالی init1 () { v.init1 (); population = صفحہ_بٹ; }

  خالی dirty () { population = UINT_MAX; }

  ساکن اندرونی constexpr غیرعلامتی len ()
  { واپس ARRAY_لمبائی_CONST (v); }

  عامل منطقی () ثابت { واپس !is_empty (); }
  منطقی is_empty () ثابت
  {
    اگر (has_population ()) واپس !population;
    منطقی خالی_خانے_2 = !v;
    اگر (خالی_خانے_2) population = 0;
    واپس خالی_خانے_2;
  }
  uint32_t hash () ثابت
  {
    واپس hb_بائٹیں_t ((ثابت حرف *) &v, حجم (v)).hash ();
  }

  خالی جوڑو (hb_codepoint_t g) { elt (g) |= نقاب (g); dirty (); }
  خالی جوڑو_بٹ (hb_codepoint_t g, uint64_t بٹ) { elt (g) |= بٹ; dirty (); }
  خالی del (hb_codepoint_t g) { elt (g) &= ~نقاب (g); dirty (); }
  خالی del_بٹ (hb_codepoint_t g, uint64_t بٹ) { elt (g) &= ~بٹ; dirty (); }
  خالی سیٹ (hb_codepoint_t g, منطقی value) { اگر (value) جوڑو (g); ورنہ del (g); }
  منطقی get (hb_codepoint_t g) ثابت { واپس elt (g) & نقاب (g); }
  منطقی may_have (hb_codepoint_t g) ثابت { واپس get (g); }

  منطقی عامل [] (hb_codepoint_t g) ثابت { واپس get (g); }
  منطقی عامل () (hb_codepoint_t g) ثابت { واپس get (g); }
  منطقی has (hb_codepoint_t g) ثابت { واپس get (g); }

  خالی جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    اگر (la == lb)
      *la |= (نقاب (b) << 1) - نقاب(a);
    ورنہ اگر (likely (la < lb))
    {
      *la |= ~(نقاب (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (حرف *) lb - (حرف *) la);

      *lb |= ((نقاب (b) << 1) - 1llu);
    }
    ورنہ
      واپس;
    dirty ();
  }
  خالی del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    اگر (la == lb)
      *la &= ~((نقاب (b) << 1llu) - نقاب(a));
    ورنہ اگر (likely (la < lb))
    {
      *la &= نقاب (a) - 1;
      la++;

      hb_memset (la, 0, (حرف *) lb - (حرف *) la);

      *lb &= ~((نقاب (b) << 1) - 1llu);
    }
    ورنہ
      واپس;
    dirty ();
  }
  خالی سیٹ_range (hb_codepoint_t a, hb_codepoint_t b, منطقی v)
  { اگر (v) جوڑو_range (a, b); ورنہ del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  غیرعلامتی صحیح اكتب (uint32_t        بنیاد,
		      غیرعلامتی صحیح    آغاز_قدر,
		      hb_codepoint_t *p,
		      غیرعلامتی صحیح    سائز) ثابت
  {
    غیرعلامتی صحیح آغاز_v = آغاز_قدر / ELT_بٹ;
    غیرعلامتی صحیح آغاز_ثنائی_ہندسہ = آغاز_قدر & ELT_نقاب;
    غیرعلامتی صحیح گنتی = 0;
    برائے (غیرعلامتی i = آغاز_v; i < len () && گنتی < سائز; i++)
    {
      elt_t بٹ = v[i];
      uint32_t v_بنیاد = بنیاد | (i * ELT_بٹ);
      برائے (غیرعلامتی صحیح j = آغاز_ثنائی_ہندسہ; j < ELT_بٹ && گنتی < سائز; j++)
      {
	اگر ((elt_t(1) << j) & بٹ) {
	  *p++ = v_بنیاد | j;
	  گنتی++;
	}
      }
      آغاز_ثنائی_ہندسہ = 0;
    }
    واپس گنتی;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  غیرعلامتی صحیح write_inverted (uint32_t        بنیاد,
			       غیرعلامتی صحیح    آغاز_قدر,
			       hb_codepoint_t *p,
			       غیرعلامتی صحیح    سائز,
			       hb_codepoint_t *اگلا_قدر) ثابت
  {
    غیرعلامتی صحیح آغاز_v = آغاز_قدر / ELT_بٹ;
    غیرعلامتی صحیح آغاز_ثنائی_ہندسہ = آغاز_قدر & ELT_نقاب;
    غیرعلامتی صحیح گنتی = 0;
    برائے (غیرعلامتی i = آغاز_v; i < len () && گنتی < سائز; i++)
    {
      elt_t بٹ = v[i];
      uint32_t v_انحراف = i * ELT_بٹ;
      برائے (غیرعلامتی صحیح j = آغاز_ثنائی_ہندسہ; j < ELT_بٹ && گنتی < سائز; j++)
      {
	اگر ((elt_t(1) << j) & بٹ)
	{
	  hb_codepoint_t value = بنیاد | v_انحراف | j;
	  // Emit all the missing values from next_value up to value - 1.
	  برائے (hb_codepoint_t k = *اگلا_قدر; k < value && گنتی < سائز; k++)
	  {
	    *p++ = k;
	    گنتی++;
	  }
	  // Skip over this value;
	  *اگلا_قدر = value + 1;
	}
      }
      آغاز_ثنائی_ہندسہ = 0;
    }
    واپس گنتی;
  }

  منطقی عامل == (ثابت hb_ثنائی_ہندسہ_صفحہ_t &other) ثابت { واپس is_equal (other); }
  منطقی is_equal (ثابت hb_ثنائی_ہندسہ_صفحہ_t &other) ثابت { واپس v == other.v; }
  منطقی intersects (ثابت hb_ثنائی_ہندسہ_صفحہ_t &other) ثابت
  {
    برائے (غیرعلامتی i = 0; i < len (); i++)
      اگر (v[i] & other.v[i])
	واپس سچ;
    واپس جھوٹ;
  }
  منطقی may_intersect (ثابت hb_ثنائی_ہندسہ_صفحہ_t &other) ثابت
  { واپس intersects (other); }

  منطقی عامل <= (ثابت hb_ثنائی_ہندسہ_صفحہ_t &larger_صفحہ) ثابت { واپس is_subset (larger_صفحہ); }
  منطقی is_subset (ثابت hb_ثنائی_ہندسہ_صفحہ_t &larger_صفحہ) ثابت
  {
    اگر (has_population () && larger_صفحہ.has_population () &&
	population > larger_صفحہ.population)
      واپس جھوٹ;

    برائے (غیرعلامتی i = 0; i < len (); i++)
      اگر (~larger_صفحہ.v[i] & v[i])
	واپس جھوٹ;
    واپس سچ;
  }

  منطقی has_population () ثابت { واپس population != UINT_MAX; }
  غیرعلامتی get_population () ثابت
  {
    اگر (has_population ()) واپس population;
    واپس population = v;
  }

  منطقی next (hb_codepoint_t *codepoint) ثابت
  {
    غیرعلامتی صحیح m = (*codepoint + 1) & نقاب_2;
    اگر (!m)
    {
      *codepoint = INVALID;
      واپس جھوٹ;
    }
    غیرعلامتی صحیح i = m / ELT_بٹ;
    غیرعلامتی صحیح j = m & ELT_نقاب;

    ثابت elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    برائے (ثابت elt_t *p = &vv; i < len (); p = ((ثابت elt_t *) &v[0]) + (++i))
      اگر (*p)
      {
	*codepoint = i * ELT_بٹ + elt_get_min (*p);
	واپس سچ;
      }

    *codepoint = INVALID;
    واپس جھوٹ;
  }
  منطقی پچھلا (hb_codepoint_t *codepoint) ثابت
  {
    غیرعلامتی صحیح m = (*codepoint - 1) & نقاب_2;
    اگر (m == نقاب_2)
    {
      *codepoint = INVALID;
      واپس جھوٹ;
    }
    غیرعلامتی صحیح i = m / ELT_بٹ;
    غیرعلامتی صحیح j = m & ELT_نقاب;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    ثابت elt_t نقاب = j < 8 * حجم (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    ثابت elt_t vv = v[i] & نقاب;
    ثابت elt_t *p = &vv;
    جب (سچ)
    {
      اگر (*p)
      {
	*codepoint = i * ELT_بٹ + elt_get_max (*p);
	واپس سچ;
      }
      اگر ((صحیح) i <= 0) توڑو;
      p = &v[--i];
    }

    *codepoint = INVALID;
    واپس جھوٹ;
  }
  hb_codepoint_t get_min () ثابت
  {
    برائے (غیرعلامتی صحیح i = 0; i < len (); i++)
      اگر (v[i])
	واپس i * ELT_بٹ + elt_get_min (v[i]);
    واپس INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    برائے (صحیح i = len () - 1; i >= 0; i--)
      اگر (v[i])
	واپس i * ELT_بٹ + elt_get_max (v[i]);
    واپس INVALID;
  }

  /*
   * Iterator implementation.
   */
  ساخت iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساکن constexpr منطقی is_sorted_iterator = سچ;
    iter_t (ثابت hb_ثنائی_ہندسہ_صفحہ_t &s_ = Null (hb_ثنائی_ہندسہ_صفحہ_t), منطقی init = سچ) : s (&s_), v (INVALID)
    {
      اگر (init)
	v = s->get_min ();
    }

    قسمتعریف hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { واپس v; }
    منطقی __more__ () ثابت { واپس v != INVALID; }
    خالی __next__ () {
       s->next (&v);
    }
    خالی __prev__ () { s->پچھلا (&v); }
    iter_t انتہا () ثابت { واپس iter_t (*s, جھوٹ); }
    منطقی عامل != (ثابت iter_t& o) ثابت
    { واپس v != o.v; }

    محفوظ:
    ثابت hb_ثنائی_ہندسہ_صفحہ_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () ثابت { واپس iter_t (*یہ); }
  عامل iter_t () ثابت { واپس iter (); }

  ساکن constexpr hb_codepoint_t INVALID = HB_سیٹ_قدر_INVALID;

  قسمتعریف غیرعلامتی طویل طویل elt_t;
  ساکن constexpr غیرعلامتی صفحہ_بٹ_LOG_2 = 9; // 512 bits
  ساکن constexpr غیرعلامتی صفحہ_بٹ = 1 << صفحہ_بٹ_LOG_2;
  static_assert (1 << صفحہ_بٹ_LOG_2 == صفحہ_بٹ, "");
  static_assert ((صفحہ_بٹ & ((صفحہ_بٹ) - 1)) == 0, "");
  ساکن constexpr غیرعلامتی صفحہ_BITMASK = صفحہ_بٹ - 1;

  ساکن غیرعلامتی صحیح elt_get_min (ثابت elt_t &elt) { واپس hb_ctz (elt); }
  ساکن غیرعلامتی صحیح elt_get_max (ثابت elt_t &elt) { واپس hb_ثنائی_ہندسہ_storage (elt) - 1; }

  قسمتعریف hb_سمتیہ_سائز_t<elt_t, صفحہ_بٹ / 8> سمتیہ_t;

  ساکن constexpr غیرعلامتی ELT_بٹ = حجم (elt_t) * 8;
  ساکن constexpr غیرعلامتی ELT_نقاب = ELT_بٹ - 1;

  ساکن constexpr غیرعلامتی بٹ_2 = حجم (سمتیہ_t) * 8;
  ساکن constexpr غیرعلامتی نقاب_2 = بٹ_2 - 1;
  static_assert ((غیرعلامتی) صفحہ_بٹ == (غیرعلامتی) بٹ_2, "");

  elt_t &elt (hb_codepoint_t g) { واپس v[(g & نقاب_2) / ELT_بٹ]; }
  ثابت elt_t& elt (hb_codepoint_t g) ثابت { واپس v[(g & نقاب_2) / ELT_بٹ]; }
  ساکن constexpr elt_t نقاب (hb_codepoint_t g) { واپس elt_t (1) << (g & ELT_نقاب); }

  تغیرپذیر غیرعلامتی population;
  سمتیہ_t v;
};


#اگر_ختم /* HB_BIT_PAGE_HH */
