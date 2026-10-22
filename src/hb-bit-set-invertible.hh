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

#ifndef HB_BIT_SET_INVERTIBLE_HH
#define HB_BIT_SET_INVERTIBLE_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set.hh"


بنية hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  منطقي inverted = خطأ;

  hb_bit_set_invertible_t () = افتراضي;
  hb_bit_set_invertible_t (ثابت hb_bit_set_invertible_t& o) = افتراضي;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*هذا, other); }
  hb_bit_set_invertible_t& operator= (ثابت hb_bit_set_invertible_t& o) = افتراضي;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*هذا, other); أرجع *هذا; }
  friend فراغ swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    إذا (likely (!a.s.successful || !b.s.successful))
      أرجع;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  فراغ init () { s.init (); inverted = خطأ; }
  فراغ fini () { s.fini (); }
  فراغ err () { s.err (); }
  منطقي in_error () ثابت { أرجع s.in_error (); }
  explicit operator منطقي () ثابت { أرجع !is_empty (); }

  فراغ alloc (unsigned sz) { s.alloc (sz); }
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
    previous (&v);
    أرجع v;
  }
  unsigned صحيح get_population () ثابت
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


  فراغ add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  فراغ add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  منطقي add_range (hb_codepoint_t a, hb_codepoint_t b)
  { أرجع unlikely (inverted) ? ((فراغ) s.del_range (a, b), true) : s.add_range (a, b); }

  template <typename T>
  فراغ add_array (ثابت T *array, unsigned صحيح کيراءن, unsigned صحيح stride=حجم(T))
  { inverted ? s.del_array (array, کيراءن, stride) : s.add_array (array, کيراءن, stride); }
  template <typename T>
  فراغ add_array (ثابت hb_array_t<ثابت T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  منطقي add_sorted_array (ثابت T *array, unsigned صحيح کيراءن, unsigned صحيح stride=حجم(T))
  { أرجع inverted ? s.del_sorted_array (array, کيراءن, stride) : s.add_sorted_array (array, کيراءن, stride); }
  template <typename T>
  منطقي add_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع add_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (فراغ) s.add_range (a, b) : s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { أرجع s.get (g) ^ inverted; }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي operator [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي operator () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); أرجع *هذا; }
  hb_bit_set_invertible_t& operator << (ثابت hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_bit_set_invertible_t &other) ثابت
  { أرجع inverted || other.inverted || s.intersects (other.s); }

  منطقي intersects (ثابت hb_bit_set_invertible_t &other) ثابت
  {
    إذا (likely (!inverted))
      أرجع unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    إذا (likely (!other.inverted))
      أرجع !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    إذا ((uint64_t) population + other_population < INVALID)
      أرجع true;

    أرجع population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  منطقي intersects (hb_codepoint_t first, hb_codepoint_t last) ثابت
  {
    hb_codepoint_t c = first - 1;
    أرجع next (&c) && c <= last;
  }

  فراغ set (ثابت hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    إذا (likely (s.successful))
      inverted = other.inverted;
  }

  منطقي is_equal (ثابت hb_bit_set_invertible_t &other) ثابت
  {
    إذا (likely (inverted == other.inverted))
      أرجع s.is_equal (other.s);
    وإلا
    {
      إذا (get_population () != other.get_population ())
	أرجع خطأ;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      أرجع hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { أرجع _.first == _.second; }));
    }
  }

  منطقي is_subset (ثابت hb_bit_set_invertible_t &larger_set) ثابت
  {
    إذا (unlikely (inverted != larger_set.inverted))
    {
      إذا (inverted)
	أرجع hb_all (iter (), larger_set.s);
      وإلا
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	أرجع !s.intersects (larger_set.s);
    }
    وإلا
      أرجع unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  محمي:
  template <typename Op>
  فراغ process (ثابت Op& op,
		ثابت hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  عام:
  فراغ union_ (ثابت hb_bit_set_invertible_t &other)
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
  فراغ intersect (ثابت hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
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
  فراغ subtract (ثابت hb_bit_set_invertible_t &other)
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
  فراغ symmetric_difference (ثابت hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    إذا (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.next (codepoint);

    auto old = *codepoint;
    إذا (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    auto v = old;
    s.next (&v);
    إذا (old + 1 < v)
    {
      *codepoint = old + 1;
      أرجع true;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    أرجع *codepoint != INVALID;
  }
  منطقي next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    إذا (unlikely (*codepoint == INVALID))
      base = 0;
    وإلا
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      إذا (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	اذهب done;
      base += hb_bit_page_t::ELT_BITS;
    }

    طالما (true)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      إذا (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      منطقي last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      إذا (last)
	*bits &= UINT64_MAX >> 1;
      إذا (*bits)
      {
	*codepoint = base;
	أرجع true;
      }
      إذا (unlikely (last))
	اكسر;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    أرجع خطأ;
  }
  منطقي previous (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.previous (codepoint);

    auto old = *codepoint;
    إذا (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    auto v = old;
    s.previous (&v);

    إذا (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      أرجع true;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    أرجع *codepoint != INVALID;
  }
  منطقي next_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.next_range (first, last);

    إذا (!next (last))
    {
      *last = *first = INVALID;
      أرجع خطأ;
    }

    *first = *last;
    s.next (last);
    --*last;
    أرجع true;
  }
  منطقي previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.previous_range (first, last);

    إذا (!previous (first))
    {
      *last = *first = INVALID;
      أرجع خطأ;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    أرجع true;
  }

  unsigned صحيح next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned صحيح    size) ثابت
  {
    أرجع inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  ساكن constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = true;
    ساكن constexpr منطقي has_fast_len = true;
    iter_t (ثابت hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    منطقي init = true) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      إذا (init)
	advance ();
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { أرجع v; }
    منطقي __more__ () ثابت { أرجع v != INVALID; }
    فراغ __next__ ()
    {
      remaining--;
      advance ();
    }
    فراغ __prev__ ()
    {
      إذا (s->previous (&v))
      {
	متزامن ();
	remaining++;
      }
      وإلا
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () ثابت
    {
      إذا (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = true;
      }
      أرجع remaining;
    }
    iter_t end () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي operator != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable منطقي have_len;
    hb_codepoint_t base;
    uint64_t bits;

    فراغ advance ()
    {
      إذا (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      وإلا إذا (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      وإلا
	v = INVALID;
    }

    فراغ متزامن ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned صحيح bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  operator iter_t () ثابت { أرجع iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */
