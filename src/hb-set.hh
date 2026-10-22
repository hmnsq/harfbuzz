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

#ifndef HB_SET_HH
#define HB_SET_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-bit-set-invertible.hh"
#ಅನ್ತರ್ಭಾವ "hb-bit-vector.hh" // Just to include


template <typename impl_t>
ಸಂರಚನಾ hb_sparseset_t
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ realloc_move = ಸತ್ಯ;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ಧ್ರುವ hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (ಧ್ರುವ hb_sparseset_t& other) { set (other); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  friend ಶೂನ್ಯ swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    ಪ್ರತಿ (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ಧ್ರುವ Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *ಏತತ್);
  }

  ಶೂನ್ಯ init ()
  {
    hb_object_init (ಏತತ್);
    s.init ();
  }
  ಶೂನ್ಯ fini ()
  {
    hb_object_fini (ಏತತ್);
    s.fini ();
  }

  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_empty (); }

  ಶೂನ್ಯ err () { s.err (); }
  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.in_error (); }

  ಶೂನ್ಯ alloc (unsigned sz) { s.alloc (sz); }
  ಶೂನ್ಯ reset () { s.reset (); }
  ಶೂನ್ಯ clear () { s.clear (); }
  ಶೂನ್ಯ invert () { s.invert (); }
  ತಾರ್ಕಿಕ is_inverted () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.is_inverted (); }
  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.is_empty (); }
  uint32_t hash () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.hash (); }

  ಶೂನ್ಯ add (hb_codepoint_t g) { s.add (g); }
  ಶೂನ್ಯ add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  ತಾರ್ಕಿಕ add_range (hb_codepoint_t first, hb_codepoint_t last) { ಪ್ರತ್ಯಾಗಮ s.add_range (first, last); }

  template <typename T>
  ಶೂನ್ಯ add_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  { s.add_array (array, ಎಣಿಕೆ, stride); }
  template <typename T>
  ಶೂನ್ಯ add_array (ಧ್ರುವ hb_array_t<ಧ್ರುವ T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  ತಾರ್ಕಿಕ add_sorted_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  { ಪ್ರತ್ಯಾಗಮ s.add_sorted_array (array, ಎಣಿಕೆ, stride); }
  template <typename T>
  ತಾರ್ಕಿಕ add_sorted_array (ಧ್ರುವ hb_sorted_array_t<ಧ್ರುವ T>& arr) { ಪ್ರತ್ಯಾಗಮ add_sorted_array (&arr, arr.len ()); }

  ಶೂನ್ಯ del (hb_codepoint_t g) { s.del (g); }
  ಶೂನ್ಯ del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  ತಾರ್ಕಿಕ get (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.get (g); }
  ತಾರ್ಕಿಕ may_have (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }

  /* Has interface. */
  ತಾರ್ಕಿಕ operator [] (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (k); }
  ತಾರ್ಕಿಕ has (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[k]; }

  /* Predicate. */
  ತಾರ್ಕಿಕ operator () (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  hb_sparseset_t& operator << (ಧ್ರುವ hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  ತಾರ್ಕಿಕ may_intersect (ಧ್ರುವ hb_sparseset_t &other) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ s.may_intersect (other.s); }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_sparseset_t &other) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ s.intersects (other.s); }

  ತಾರ್ಕಿಕ intersects (hb_codepoint_t first, hb_codepoint_t last) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ s.intersects (first, last); }

  ಶೂನ್ಯ set (ಧ್ರುವ hb_sparseset_t &other) { s.set (other.s); }

  ತಾರ್ಕಿಕ is_equal (ಧ್ರುವ hb_sparseset_t &other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.is_equal (other.s); }
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_set_t &other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_equal (other); }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_set_t &other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_equal (other); }

  ತಾರ್ಕಿಕ is_subset (ಧ್ರುವ hb_sparseset_t &larger_set) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.is_subset (larger_set.s); }

  ಶೂನ್ಯ union_ (ಧ್ರುವ hb_sparseset_t &other) { s.union_ (other.s); }
  ಶೂನ್ಯ intersect (ಧ್ರುವ hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  ಶೂನ್ಯ subtract (ಧ್ರುವ hb_sparseset_t &other) { s.subtract (other.s); }
  ಶೂನ್ಯ symmetric_difference (ಧ್ರುವ hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  ತಾರ್ಕಿಕ next (hb_codepoint_t *codepoint) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.next (codepoint); }
  ತಾರ್ಕಿಕ next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ s.next_bits (codepoint, bits); }
  ತಾರ್ಕಿಕ previous (hb_codepoint_t *codepoint) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.previous (codepoint); }
  ತಾರ್ಕಿಕ next_range (hb_codepoint_t *first, hb_codepoint_t *last) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ s.next_range (first, last); }
  ತಾರ್ಕಿಕ previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ s.previous_range (first, last); }
  unsigned ಪೂರ್ಣಾಙ್ಕ next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned ಪೂರ್ಣಾಙ್ಕ size) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ s.next_many (codepoint, out, size); }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_population () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.get_population (); }
  ತಾರ್ಕಿಕ get_singleton (hb_codepoint_t *codepoint) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.get_min (); }
  hb_codepoint_t get_max () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ s.get_max (); }

  ಸ್ಥಿರ constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter_t (ಏತತ್->s); }
  operator iter_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter (); }
};

ಸಂರಚನಾ hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = ಸ್ವಭಾವ;
  hb_set_t () : sparseset () {};
  hb_set_t (ಧ್ರುವ hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (ಧ್ರುವ hb_set_t&) = ಸ್ವಭಾವ;
  hb_set_t& operator = (hb_set_t&&) = ಸ್ವಭಾವ;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (ಧ್ರುವ Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  hb_set_t& operator << (ಧ್ರುವ hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
};

static_assert (hb_set_t::INVALID == HB_SET_ಮೌಲ್ಯ_INVALID, "");


#endif /* HB_SET_HH */
