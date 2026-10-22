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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-bit-set-invertible.hh"
#அந்தர்பாவ "hb-bit-vector.hh" // Just to include


template <typename impl_t>
கட்டமைப்பு hb_sparseset_t
{
  நிலையான constexpr தருக்க realloc_move = உண்மை;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (மாறிலி hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (மாறிலி hb_sparseset_t& other) { set (other); திரும்பு *இது; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); திரும்பு *இது; }
  friend வெறுமை swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    க்கு (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (மாறிலி Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *இது);
  }

  வெறுமை init ()
  {
    hb_object_init (இது);
    s.init ();
  }
  வெறுமை fini ()
  {
    hb_object_fini (இது);
    s.fini ();
  }

  explicit operator தருக்க () மாறிலி { திரும்பு !is_empty (); }

  வெறுமை err () { s.err (); }
  தருக்க in_error () மாறிலி { திரும்பு s.in_error (); }

  வெறுமை alloc (unsigned sz) { s.alloc (sz); }
  வெறுமை reset () { s.reset (); }
  வெறுமை clear () { s.clear (); }
  வெறுமை invert () { s.invert (); }
  தருக்க is_inverted () மாறிலி { திரும்பு s.is_inverted (); }
  தருக்க is_empty () மாறிலி { திரும்பு s.is_empty (); }
  uint32_t hash () மாறிலி { திரும்பு s.hash (); }

  வெறுமை add (hb_codepoint_t g) { s.add (g); }
  வெறுமை add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  தருக்க add_range (hb_codepoint_t first, hb_codepoint_t last) { திரும்பு s.add_range (first, last); }

  template <typename T>
  வெறுமை add_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  { s.add_array (array, மொத்தம், stride); }
  template <typename T>
  வெறுமை add_array (மாறிலி hb_array_t<மாறிலி T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  தருக்க add_sorted_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  { திரும்பு s.add_sorted_array (array, மொத்தம், stride); }
  template <typename T>
  தருக்க add_sorted_array (மாறிலி hb_sorted_array_t<மாறிலி T>& arr) { திரும்பு add_sorted_array (&arr, arr.len ()); }

  வெறுமை del (hb_codepoint_t g) { s.del (g); }
  வெறுமை del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  தருக்க get (hb_codepoint_t g) மாறிலி { திரும்பு s.get (g); }
  தருக்க may_have (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }

  /* Has interface. */
  தருக்க operator [] (hb_codepoint_t k) மாறிலி { திரும்பு get (k); }
  தருக்க has (hb_codepoint_t k) மாறிலி { திரும்பு (*இது)[k]; }

  /* Predicate. */
  தருக்க operator () (hb_codepoint_t k) மாறிலி { திரும்பு has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); திரும்பு *இது; }
  hb_sparseset_t& operator << (மாறிலி hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); திரும்பு *இது; }

  தருக்க may_intersect (மாறிலி hb_sparseset_t &other) மாறிலி
  { திரும்பு s.may_intersect (other.s); }

  தருக்க intersects (மாறிலி hb_sparseset_t &other) மாறிலி
  { திரும்பு s.intersects (other.s); }

  தருக்க intersects (hb_codepoint_t first, hb_codepoint_t last) மாறிலி
  { திரும்பு s.intersects (first, last); }

  வெறுமை set (மாறிலி hb_sparseset_t &other) { s.set (other.s); }

  தருக்க is_equal (மாறிலி hb_sparseset_t &other) மாறிலி { திரும்பு s.is_equal (other.s); }
  தருக்க operator == (மாறிலி hb_set_t &other) மாறிலி { திரும்பு is_equal (other); }
  தருக்க operator != (மாறிலி hb_set_t &other) மாறிலி { திரும்பு !is_equal (other); }

  தருக்க is_subset (மாறிலி hb_sparseset_t &larger_set) மாறிலி { திரும்பு s.is_subset (larger_set.s); }

  வெறுமை union_ (மாறிலி hb_sparseset_t &other) { s.union_ (other.s); }
  வெறுமை intersect (மாறிலி hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  வெறுமை subtract (மாறிலி hb_sparseset_t &other) { s.subtract (other.s); }
  வெறுமை symmetric_difference (மாறிலி hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  தருக்க next (hb_codepoint_t *codepoint) மாறிலி { திரும்பு s.next (codepoint); }
  தருக்க next_bits (hb_codepoint_t *codepoint, uint64_t *bits) மாறிலி
  { திரும்பு s.next_bits (codepoint, bits); }
  தருக்க previous (hb_codepoint_t *codepoint) மாறிலி { திரும்பு s.previous (codepoint); }
  தருக்க next_range (hb_codepoint_t *first, hb_codepoint_t *last) மாறிலி
  { திரும்பு s.next_range (first, last); }
  தருக்க previous_range (hb_codepoint_t *first, hb_codepoint_t *last) மாறிலி
  { திரும்பு s.previous_range (first, last); }
  unsigned முழு next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned முழு size) மாறிலி
  { திரும்பு s.next_many (codepoint, out, size); }

  unsigned முழு get_population () மாறிலி { திரும்பு s.get_population (); }
  தருக்க get_singleton (hb_codepoint_t *codepoint) மாறிலி { திரும்பு s.get_singleton (codepoint); }
  hb_codepoint_t get_min () மாறிலி { திரும்பு s.get_min (); }
  hb_codepoint_t get_max () மாறிலி { திரும்பு s.get_max (); }

  நிலையான constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () மாறிலி { திரும்பு iter_t (இது->s); }
  operator iter_t () மாறிலி { திரும்பு iter (); }
};

கட்டமைப்பு hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = இயல்பு;
  hb_set_t () : sparseset () {};
  hb_set_t (மாறிலி hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (மாறிலி hb_set_t&) = இயல்பு;
  hb_set_t& operator = (hb_set_t&&) = இயல்பு;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (மாறிலி Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); திரும்பு *இது; }
  hb_set_t& operator << (மாறிலி hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); திரும்பு *இது; }
};

static_assert (hb_set_t::INVALID == HB_SET_மதிப்பு_INVALID, "");


#endif /* HB_SET_HH */
