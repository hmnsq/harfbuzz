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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-bit-set-invertible.hh"
#অন্তর্ভাব "hb-bit-vector.hh" // Just to include


template <typename impl_t>
সংরচনা hb_sparseset_t
{
  স্থির constexpr তার্কিক realloc_move = সত্য;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ধ্রুব hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (ধ্রুব hb_sparseset_t& other) { set (other); প্রত্যাগম *এতত্; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); প্রত্যাগম *এতত্; }
  friend শূন্য swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    প্রতি (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ধ্রুব Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *এতত্);
  }

  শূন্য init ()
  {
    hb_object_init (এতত্);
    s.init ();
  }
  শূন্য fini ()
  {
    hb_object_fini (এতত্);
    s.fini ();
  }

  explicit operator তার্কিক () ধ্রুব { প্রত্যাগম !is_empty (); }

  শূন্য err () { s.err (); }
  তার্কিক in_error () ধ্রুব { প্রত্যাগম s.in_error (); }

  শূন্য alloc (unsigned sz) { s.alloc (sz); }
  শূন্য reset () { s.reset (); }
  শূন্য clear () { s.clear (); }
  শূন্য invert () { s.invert (); }
  তার্কিক is_inverted () ধ্রুব { প্রত্যাগম s.is_inverted (); }
  তার্কিক is_empty () ধ্রুব { প্রত্যাগম s.is_empty (); }
  uint32_t hash () ধ্রুব { প্রত্যাগম s.hash (); }

  শূন্য add (hb_codepoint_t g) { s.add (g); }
  শূন্য add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  তার্কিক add_range (hb_codepoint_t first, hb_codepoint_t last) { প্রত্যাগম s.add_range (first, last); }

  template <typename T>
  শূন্য add_array (ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  { s.add_array (array, গুনতি, stride); }
  template <typename T>
  শূন্য add_array (ধ্রুব hb_array_t<ধ্রুব T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  তার্কিক add_sorted_array (ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  { প্রত্যাগম s.add_sorted_array (array, গুনতি, stride); }
  template <typename T>
  তার্কিক add_sorted_array (ধ্রুব hb_sorted_array_t<ধ্রুব T>& arr) { প্রত্যাগম add_sorted_array (&arr, arr.len ()); }

  শূন্য del (hb_codepoint_t g) { s.del (g); }
  শূন্য del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  তার্কিক get (hb_codepoint_t g) ধ্রুব { প্রত্যাগম s.get (g); }
  তার্কিক may_have (hb_codepoint_t g) ধ্রুব { প্রত্যাগম get (g); }

  /* Has interface. */
  তার্কিক operator [] (hb_codepoint_t k) ধ্রুব { প্রত্যাগম get (k); }
  তার্কিক has (hb_codepoint_t k) ধ্রুব { প্রত্যাগম (*এতত্)[k]; }

  /* Predicate. */
  তার্কিক operator () (hb_codepoint_t k) ধ্রুব { প্রত্যাগম has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); প্রত্যাগম *এতত্; }
  hb_sparseset_t& operator << (ধ্রুব hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); প্রত্যাগম *এতত্; }

  তার্কিক may_intersect (ধ্রুব hb_sparseset_t &other) ধ্রুব
  { প্রত্যাগম s.may_intersect (other.s); }

  তার্কিক intersects (ধ্রুব hb_sparseset_t &other) ধ্রুব
  { প্রত্যাগম s.intersects (other.s); }

  তার্কিক intersects (hb_codepoint_t first, hb_codepoint_t last) ধ্রুব
  { প্রত্যাগম s.intersects (first, last); }

  শূন্য set (ধ্রুব hb_sparseset_t &other) { s.set (other.s); }

  তার্কিক is_equal (ধ্রুব hb_sparseset_t &other) ধ্রুব { প্রত্যাগম s.is_equal (other.s); }
  তার্কিক operator == (ধ্রুব hb_set_t &other) ধ্রুব { প্রত্যাগম is_equal (other); }
  তার্কিক operator != (ধ্রুব hb_set_t &other) ধ্রুব { প্রত্যাগম !is_equal (other); }

  তার্কিক is_subset (ধ্রুব hb_sparseset_t &larger_set) ধ্রুব { প্রত্যাগম s.is_subset (larger_set.s); }

  শূন্য union_ (ধ্রুব hb_sparseset_t &other) { s.union_ (other.s); }
  শূন্য intersect (ধ্রুব hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  শূন্য subtract (ধ্রুব hb_sparseset_t &other) { s.subtract (other.s); }
  শূন্য symmetric_difference (ধ্রুব hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  তার্কিক next (hb_codepoint_t *codepoint) ধ্রুব { প্রত্যাগম s.next (codepoint); }
  তার্কিক next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ধ্রুব
  { প্রত্যাগম s.next_bits (codepoint, bits); }
  তার্কিক previous (hb_codepoint_t *codepoint) ধ্রুব { প্রত্যাগম s.previous (codepoint); }
  তার্কিক next_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুব
  { প্রত্যাগম s.next_range (first, last); }
  তার্কিক previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুব
  { প্রত্যাগম s.previous_range (first, last); }
  unsigned পূর্ণাঙ্ক next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned পূর্ণাঙ্ক size) ধ্রুব
  { প্রত্যাগম s.next_many (codepoint, out, size); }

  unsigned পূর্ণাঙ্ক get_population () ধ্রুব { প্রত্যাগম s.get_population (); }
  তার্কিক get_singleton (hb_codepoint_t *codepoint) ধ্রুব { প্রত্যাগম s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ধ্রুব { প্রত্যাগম s.get_min (); }
  hb_codepoint_t get_max () ধ্রুব { প্রত্যাগম s.get_max (); }

  স্থির constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () ধ্রুব { প্রত্যাগম iter_t (এতত্->s); }
  operator iter_t () ধ্রুব { প্রত্যাগম iter (); }
};

সংরচনা hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = স্বভাব;
  hb_set_t () : sparseset () {};
  hb_set_t (ধ্রুব hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (ধ্রুব hb_set_t&) = স্বভাব;
  hb_set_t& operator = (hb_set_t&&) = স্বভাব;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (ধ্রুব Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); প্রত্যাগম *এতত্; }
  hb_set_t& operator << (ধ্রুব hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); প্রত্যাগম *এতত্; }
};

static_assert (hb_set_t::INVALID == HB_SET_মান_INVALID, "");


#endif /* HB_SET_HH */
