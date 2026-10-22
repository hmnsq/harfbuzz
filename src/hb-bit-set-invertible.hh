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

#شامل "hb.hh"
#شامل "hb-bit-set.hh"


ساختار hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  منطقی inverted = نادرست;

  hb_bit_set_invertible_t () = پیشفرض;
  hb_bit_set_invertible_t (ثابت hb_bit_set_invertible_t& o) = پیشفرض;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*این, other); }
  hb_bit_set_invertible_t& operator= (ثابت hb_bit_set_invertible_t& o) = پیشفرض;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*این, other); بازگشت *این; }
  friend تهی swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    اگر (likely (!a.s.successful || !b.s.successful))
      بازگشت;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  تهی init () { s.init (); inverted = نادرست; }
  تهی fini () { s.fini (); }
  تهی err () { s.err (); }
  منطقی in_error () ثابت { بازگشت s.in_error (); }
  explicit operator منطقی () ثابت { بازگشت !is_empty (); }

  تهی alloc (unsigned sz) { s.alloc (sz); }
  تهی reset ()
  {
    s.reset ();
    inverted = نادرست;
  }
  تهی clear ()
  {
    s.clear ();
    اگر (likely (s.successful))
      inverted = نادرست;
  }
  تهی invert ()
  {
    اگر (likely (s.successful))
      inverted = !inverted;
  }

  منطقی is_inverted () ثابت
  {
    بازگشت inverted;
  }

  منطقی is_empty () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    بازگشت v == INVALID;
  }
  uint32_t hash () ثابت { بازگشت s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    بازگشت v;
  }
  hb_codepoint_t get_max () ثابت
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    بازگشت v;
  }
  unsigned صحیح get_population () ثابت
  { بازگشت inverted ? INVALID - s.get_population () : s.get_population (); }
  منطقی get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    اگر (likely (!inverted))
      بازگشت s.get_singleton (codepoint);
    اگر (s.get_population () != INVALID - 1)
      بازگشت نادرست;
    *codepoint = get_min ();
    بازگشت *codepoint != INVALID;
  }


  تهی add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  تهی add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  منطقی add_range (hb_codepoint_t a, hb_codepoint_t b)
  { بازگشت unlikely (inverted) ? ((تهی) s.del_range (a, b), درست) : s.add_range (a, b); }

  template <typename T>
  تهی add_array (ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  { inverted ? s.del_array (array, شمار, stride) : s.add_array (array, شمار, stride); }
  template <typename T>
  تهی add_array (ثابت hb_array_t<ثابت T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  منطقی add_sorted_array (ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  { بازگشت inverted ? s.del_sorted_array (array, شمار, stride) : s.add_sorted_array (array, شمار, stride); }
  template <typename T>
  منطقی add_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { بازگشت add_sorted_array (&arr, arr.len ()); }

  تهی del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  تهی del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (تهی) s.add_range (a, b) : s.del_range (a, b); }

  منطقی get (hb_codepoint_t g) ثابت { بازگشت s.get (g) ^ inverted; }
  منطقی may_have (hb_codepoint_t g) ثابت { بازگشت get (g); }

  /* Has interface. */
  منطقی operator [] (hb_codepoint_t k) ثابت { بازگشت get (k); }
  منطقی has (hb_codepoint_t k) ثابت { بازگشت (*این)[k]; }
  /* Predicate. */
  منطقی operator () (hb_codepoint_t k) ثابت { بازگشت has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); بازگشت *این; }
  hb_bit_set_invertible_t& operator << (ثابت hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); بازگشت *این; }

  منطقی may_intersect (ثابت hb_bit_set_invertible_t &other) ثابت
  { بازگشت inverted || other.inverted || s.intersects (other.s); }

  منطقی intersects (ثابت hb_bit_set_invertible_t &other) ثابت
  {
    اگر (likely (!inverted))
      بازگشت unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    اگر (likely (!other.inverted))
      بازگشت !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    اگر ((uint64_t) population + other_population < INVALID)
      بازگشت درست;

    بازگشت population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  منطقی intersects (hb_codepoint_t first, hb_codepoint_t last) ثابت
  {
    hb_codepoint_t c = first - 1;
    بازگشت next (&c) && c <= last;
  }

  تهی set (ثابت hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    اگر (likely (s.successful))
      inverted = other.inverted;
  }

  منطقی is_equal (ثابت hb_bit_set_invertible_t &other) ثابت
  {
    اگر (likely (inverted == other.inverted))
      بازگشت s.is_equal (other.s);
    وگرنه
    {
      اگر (get_population () != other.get_population ())
	بازگشت نادرست;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      بازگشت hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { بازگشت _.first == _.second; }));
    }
  }

  منطقی is_subset (ثابت hb_bit_set_invertible_t &larger_set) ثابت
  {
    اگر (unlikely (inverted != larger_set.inverted))
    {
      اگر (inverted)
	بازگشت hb_all (iter (), larger_set.s);
      وگرنه
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	بازگشت !s.intersects (larger_set.s);
    }
    وگرنه
      بازگشت unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  محافظتشده:
  template <typename Op>
  تهی process (ثابت Op& op,
		ثابت hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  عمومی:
  تهی union_ (ثابت hb_bit_set_invertible_t &other)
  {
    اگر (likely (inverted == other.inverted))
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_and, other);
      وگرنه
	process (hb_bitwise_or, other); /* Main branch. */
    }
    وگرنه
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_gt, other);
      وگرنه
	process (hb_bitwise_lt, other);
    }
    اگر (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  تهی intersect (ثابت hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    اگر (likely (inverted == other.inverted))
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      وگرنه
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    وگرنه
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      وگرنه
	process (hb_bitwise_gt, other, workspace);
    }
    اگر (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  تهی subtract (ثابت hb_bit_set_invertible_t &other)
  {
    اگر (likely (inverted == other.inverted))
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_lt, other);
      وگرنه
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    وگرنه
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_or, other);
      وگرنه
	process (hb_bitwise_and, other);
    }
    اگر (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  تهی symmetric_difference (ثابت hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    اگر (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  منطقی next (hb_codepoint_t *codepoint) ثابت
  {
    اگر (likely (!inverted))
      بازگشت s.next (codepoint);

    auto old = *codepoint;
    اگر (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      بازگشت نادرست;
    }

    auto v = old;
    s.next (&v);
    اگر (old + 1 < v)
    {
      *codepoint = old + 1;
      بازگشت درست;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    بازگشت *codepoint != INVALID;
  }
  منطقی next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ثابت
  {
    اگر (likely (!inverted))
      بازگشت s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    اگر (unlikely (*codepoint == INVALID))
      base = 0;
    وگرنه
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      اگر (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	برو done;
      base += hb_bit_page_t::ELT_BITS;
    }

    وقتی (درست)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      اگر (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      منطقی last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      اگر (last)
	*bits &= UINT64_MAX >> 1;
      اگر (*bits)
      {
	*codepoint = base;
	بازگشت درست;
      }
      اگر (unlikely (last))
	بشکن;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    بازگشت نادرست;
  }
  منطقی previous (hb_codepoint_t *codepoint) ثابت
  {
    اگر (likely (!inverted))
      بازگشت s.previous (codepoint);

    auto old = *codepoint;
    اگر (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      بازگشت نادرست;
    }

    auto v = old;
    s.previous (&v);

    اگر (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      بازگشت درست;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    بازگشت *codepoint != INVALID;
  }
  منطقی next_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    اگر (likely (!inverted))
      بازگشت s.next_range (first, last);

    اگر (!next (last))
    {
      *last = *first = INVALID;
      بازگشت نادرست;
    }

    *first = *last;
    s.next (last);
    --*last;
    بازگشت درست;
  }
  منطقی previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    اگر (likely (!inverted))
      بازگشت s.previous_range (first, last);

    اگر (!previous (first))
    {
      *last = *first = INVALID;
      بازگشت نادرست;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    بازگشت درست;
  }

  unsigned صحیح next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned صحیح    size) ثابت
  {
    بازگشت inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  ایستا constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  ساختار iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ایستا constexpr منطقی is_sorted_iterator = درست;
    ایستا constexpr منطقی has_fast_len = درست;
    iter_t (ثابت hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    منطقی init = درست) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      اگر (init)
	advance ();
    }

    تعریفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { بازگشت v; }
    منطقی __more__ () ثابت { بازگشت v != INVALID; }
    تهی __next__ ()
    {
      remaining--;
      advance ();
    }
    تهی __prev__ ()
    {
      اگر (s->previous (&v))
      {
	همگام ();
	remaining++;
      }
      وگرنه
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () ثابت
    {
      اگر (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = درست;
      }
      بازگشت remaining;
    }
    iter_t end () ثابت { بازگشت iter_t (*s, نادرست); }
    منطقی operator != (ثابت iter_t& o) ثابت
    { بازگشت v != o.v; }

    محافظتشده:
    ثابت hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable منطقی have_len;
    hb_codepoint_t base;
    uint64_t bits;

    تهی advance ()
    {
      اگر (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      وگرنه اگر (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      وگرنه
	v = INVALID;
    }

    تهی همگام ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned صحیح bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ثابت { بازگشت iter_t (*این); }
  operator iter_t () ثابت { بازگشت iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */
