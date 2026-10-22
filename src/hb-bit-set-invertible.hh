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

#جيڪڏهن_وصف_نه_ٿيل HB_ٻٽو_انگ_سيٽ_INVERTIBLE_HH
#وصف_ڪريو HB_ٻٽو_انگ_سيٽ_INVERTIBLE_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set.hh"


بنية hb_ٻٽو_انگ_سيٽ_invertible_t
{
  hb_ٻٽو_انگ_سيٽ_t s;
  منطقي inverted = خطأ;

  hb_ٻٽو_انگ_سيٽ_invertible_t () = افتراضي;
  hb_ٻٽو_انگ_سيٽ_invertible_t (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t& o) = افتراضي;
  hb_ٻٽو_انگ_سيٽ_invertible_t (hb_ٻٽو_انگ_سيٽ_invertible_t&& other)  noexcept : hb_ٻٽو_انگ_سيٽ_invertible_t () { hb_مٽا_سٽا (*هذا, other); }
  hb_ٻٽو_انگ_سيٽ_invertible_t& عامل= (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t& o) = افتراضي;
  hb_ٻٽو_انگ_سيٽ_invertible_t& عامل= (hb_ٻٽو_انگ_سيٽ_invertible_t&& other)  noexcept { hb_مٽا_سٽا (*هذا, other); أرجع *هذا; }
  دوست فراغ swap (hb_ٻٽو_انگ_سيٽ_invertible_t &a, hb_ٻٽو_انگ_سيٽ_invertible_t &b) noexcept
  {
    إذا (likely (!a.s.successful || !b.s.successful))
      أرجع;
    hb_مٽا_سٽا (a.inverted, b.inverted);
    hb_مٽا_سٽا (a.s, b.s);
  }

  فراغ init () { s.init (); inverted = خطأ; }
  فراغ fini () { s.fini (); }
  فراغ err () { s.err (); }
  منطقي اندريون_error () ثابت { أرجع s.اندريون_error (); }
  واضح عامل منطقي () ثابت { أرجع !is_empty (); }

  فراغ alloc (بنانشان sz) { s.alloc (sz); }
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
    اڳيون (&v);
    أرجع v;
  }
  بنانشان صحيح get_population () ثابت
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


  فراغ جوڙيو (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.جوڙيو (g); }
  فراغ جوڙيو_بٽ (hb_codepoint_t g, uint64_t بٽ)
  { unlikely (inverted) ? s.del_بٽ (g, بٽ) : s.جوڙيو_بٽ (g, بٽ); }
  منطقي جوڙيو_range (hb_codepoint_t a, hb_codepoint_t b)
  { أرجع unlikely (inverted) ? ((فراغ) s.del_range (a, b), سچ) : s.جوڙيو_range (a, b); }

  سانچو <قسم_نالو T>
  فراغ جوڙيو_array (ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  { inverted ? s.del_array (array, ڳڻپ, stride) : s.جوڙيو_array (array, ڳڻپ, stride); }
  سانچو <قسم_نالو T>
  فراغ جوڙيو_array (ثابت hb_array_t<ثابت T>& arr) { جوڙيو_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچو <قسم_نالو T>
  منطقي جوڙيو_sorted_array (ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  { أرجع inverted ? s.del_sorted_array (array, ڳڻپ, stride) : s.جوڙيو_sorted_array (array, ڳڻپ, stride); }
  سانچو <قسم_نالو T>
  منطقي جوڙيو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جوڙيو_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { unlikely (inverted) ? s.جوڙيو (g) : s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (فراغ) s.جوڙيو_range (a, b) : s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { أرجع s.get (g) ^ inverted; }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي عامل [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي عامل () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_ٻٽو_انگ_سيٽ_invertible_t& عامل << (hb_codepoint_t v)
  { جوڙيو (v); أرجع *هذا; }
  hb_ٻٽو_انگ_سيٽ_invertible_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { جوڙيو_range (range.پهريون, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &other) ثابت
  { أرجع inverted || other.inverted || s.intersects (other.s); }

  منطقي intersects (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &other) ثابت
  {
    إذا (likely (!inverted))
      أرجع unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    إذا (likely (!other.inverted))
      أرجع !other.s.is_subset (s);

    بنانشان population = s.get_population ();
    بنانشان other_population = other.s.get_population ();
    إذا ((uint64_t) population + other_population < INVALID)
      أرجع سچ;

    أرجع population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  منطقي intersects (hb_codepoint_t پهريون, hb_codepoint_t آخري) ثابت
  {
    hb_codepoint_t c = پهريون - 1;
    أرجع next (&c) && c <= آخري;
  }

  فراغ سيٽ (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &other)
  {
    s.سيٽ (other.s);
    إذا (likely (s.successful))
      inverted = other.inverted;
  }

  منطقي is_equal (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &other) ثابت
  {
    إذا (likely (inverted == other.inverted))
      أرجع s.is_equal (other.s);
    وإلا
    {
      إذا (get_population () != other.get_population ())
	أرجع خطأ;

      /* TODO Add iter_ranges() and use here. */
      خودڪار it1 = iter ();
      خودڪار it2 = other.iter ();
      أرجع hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { أرجع _.پهريون == _.second; }));
    }
  }

  منطقي is_subset (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &larger_سيٽ) ثابت
  {
    إذا (unlikely (inverted != larger_سيٽ.inverted))
    {
      إذا (inverted)
	أرجع hb_all (iter (), larger_سيٽ.s);
      وإلا
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	أرجع !s.intersects (larger_سيٽ.s);
    }
    وإلا
      أرجع unlikely (inverted) ? larger_سيٽ.s.is_subset (s) : s.is_subset (larger_سيٽ.s);
  }

  محمي:
  سانچو <قسم_نالو Op>
  فراغ process (ثابت Op& op,
		ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &other,
		hb_ويڪٽر_t<بنانشان> *workspace = خالي_اشارو)
  { s.process (op, other.s, workspace); }
  عام:
  فراغ union_ (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &other)
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
  فراغ intersect (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &other,
		  hb_ويڪٽر_t<بنانشان> *workspace = خالي_اشارو)
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
  فراغ ڪٽيو (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &other)
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
  فراغ متناظر_difference (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    إذا (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.next (codepoint);

    خودڪار old = *codepoint;
    إذا (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    خودڪار v = old;
    s.next (&v);
    إذا (old + 1 < v)
    {
      *codepoint = old + 1;
      أرجع سچ;
    }

    v = old;
    s.ايندڙ_range (&old, &v);

    *codepoint = v + 1;
    أرجع *codepoint != INVALID;
  }
  منطقي ايندڙ_بٽ (hb_codepoint_t *codepoint, uint64_t *بٽ) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.ايندڙ_بٽ (codepoint, بٽ);

    hb_codepoint_t بنياد;
    إذا (unlikely (*codepoint == INVALID))
      بنياد = 0;
    وإلا
    {
      بنياد = *codepoint & ~hb_ٻٽو_انگ_صفحو_t::ELT_نقاب;
      إذا (unlikely (بنياد >= INVALID - hb_ٻٽو_انگ_صفحو_t::ELT_بٽ + 1))
	اذهب پورو;
      بنياد += hb_ٻٽو_انگ_صفحو_t::ELT_بٽ;
    }

    طالما (سچ)
    {
      hb_codepoint_t cursor = بنياد ? بنياد - hb_ٻٽو_انگ_صفحو_t::ELT_بٽ : INVALID;
      uint64_t excluded = 0;
      إذا (!s.ايندڙ_بٽ (&cursor, &excluded) || cursor != بنياد)
	excluded = 0;
      *بٽ = ~excluded;
      منطقي آخري = unlikely (بنياد >= INVALID - hb_ٻٽو_انگ_صفحو_t::ELT_بٽ + 1);
      إذا (آخري)
	*بٽ &= UINT64_MAX >> 1;
      إذا (*بٽ)
      {
	*codepoint = بنياد;
	أرجع سچ;
      }
      إذا (unlikely (آخري))
	اكسر;
      بنياد += hb_ٻٽو_انگ_صفحو_t::ELT_بٽ;
    }

  پورو:
    *codepoint = INVALID;
    *بٽ = 0;
    أرجع خطأ;
  }
  منطقي اڳيون (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.اڳيون (codepoint);

    خودڪار old = *codepoint;
    إذا (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    خودڪار v = old;
    s.اڳيون (&v);

    إذا (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      أرجع سچ;
    }

    v = old;
    s.اڳيون_range (&v, &old);

    *codepoint = v - 1;
    أرجع *codepoint != INVALID;
  }
  منطقي ايندڙ_range (hb_codepoint_t *پهريون, hb_codepoint_t *آخري) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.ايندڙ_range (پهريون, آخري);

    إذا (!next (آخري))
    {
      *آخري = *پهريون = INVALID;
      أرجع خطأ;
    }

    *پهريون = *آخري;
    s.next (آخري);
    --*آخري;
    أرجع سچ;
  }
  منطقي اڳيون_range (hb_codepoint_t *پهريون, hb_codepoint_t *آخري) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.اڳيون_range (پهريون, آخري);

    إذا (!اڳيون (پهريون))
    {
      *آخري = *پهريون = INVALID;
      أرجع خطأ;
    }

    *آخري = *پهريون;
    s.اڳيون (پهريون);
    ++*پهريون;
    أرجع سچ;
  }

  بنانشان صحيح ايندڙ_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *ٻاهريون,
			  بنانشان صحيح    سائيز) ثابت
  {
    أرجع inverted ? s.ايندڙ_many_inverted (codepoint, ٻاهريون, سائيز)
		    : s.ايندڙ_many (codepoint, ٻاهريون, سائيز);
  }

  ساكن constexpr hb_codepoint_t INVALID = hb_ٻٽو_انگ_سيٽ_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = سچ;
    ساكن constexpr منطقي has_fast_len = سچ;
    iter_t (ثابت hb_ٻٽو_انگ_سيٽ_invertible_t &s_ = Null (hb_ٻٽو_انگ_سيٽ_invertible_t),
    منطقي init = سچ) : s (&s_), v (INVALID), بچيل (0),
				     have_len (!init), بنياد (INVALID), بٽ (0)
    {
      إذا (init)
	advance ();
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { أرجع v; }
    منطقي __more__ () ثابت { أرجع v != INVALID; }
    فراغ __next__ ()
    {
      بچيل--;
      advance ();
    }
    فراغ __prev__ ()
    {
      إذا (s->اڳيون (&v))
      {
	متزامن ();
	بچيل++;
      }
      وإلا
      {
	بنياد = INVALID;
	بٽ = 0;
      }
    }
    بنانشان __len__ () ثابت
    {
      إذا (unlikely (!have_len))
      {
	بچيل += s->get_population ();
	have_len = سچ;
      }
      أرجع بچيل;
    }
    iter_t پڇاڙي () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_ٻٽو_انگ_سيٽ_invertible_t *s;
    hb_codepoint_t v;
    بدلجندڙ بنانشان بچيل;
    بدلجندڙ منطقي have_len;
    hb_codepoint_t بنياد;
    uint64_t بٽ;

    فراغ advance ()
    {
      إذا (likely (بٽ))
      {
	v = بنياد + hb_ctz (بٽ);
	بٽ &= بٽ - 1;
      }
      وإلا إذا (s->ايندڙ_بٽ (&بنياد, &بٽ))
      {
	v = بنياد + hb_ctz (بٽ);
	بٽ &= بٽ - 1;
      }
      وإلا
	v = INVALID;
    }

    فراغ متزامن ()
    {
      بنياد = v & ~hb_ٻٽو_انگ_صفحو_t::ELT_نقاب;
      hb_codepoint_t cursor = بنياد ? بنياد - hb_ٻٽو_انگ_صفحو_t::ELT_بٽ : INVALID;
      s->ايندڙ_بٽ (&cursor, &بٽ);
      بنانشان صحيح bit = v & (hb_ٻٽو_انگ_صفحو_t::ELT_بٽ - 1);
      بٽ = bit == hb_ٻٽو_انگ_صفحو_t::ELT_بٽ - 1 ? 0 : بٽ >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  عامل iter_t () ثابت { أرجع iter (); }
};


#جيڪڏهن_پورو /* HB_BIT_SET_INVERTIBLE_HH */
