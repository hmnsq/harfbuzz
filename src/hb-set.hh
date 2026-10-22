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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-bit-set-invertible.hh"
#ଅନ୍ତର୍ଭାଵ "hb-bit-vector.hh" // Just to include


template <typename impl_t>
ସଂରଚନା hb_sparseset_t
{
  ସ୍ଥିର constexpr ତାର୍କିକ realloc_move = ସତ୍ୟ;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ଅଚଳ hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (ଅଚଳ hb_sparseset_t& other) { set (other); ଫେରନ୍ତୁ *ଏହି; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); ଫେରନ୍ତୁ *ଏହି; }
  friend ଶୂନ୍ୟ swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    ପାଇଁ (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ଅଚଳ Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *ଏହି);
  }

  ଶୂନ୍ୟ init ()
  {
    hb_object_init (ଏହି);
    s.init ();
  }
  ଶୂନ୍ୟ fini ()
  {
    hb_object_fini (ଏହି);
    s.fini ();
  }

  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ !is_empty (); }

  ଶୂନ୍ୟ err () { s.err (); }
  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ s.in_error (); }

  ଶୂନ୍ୟ alloc (unsigned sz) { s.alloc (sz); }
  ଶୂନ୍ୟ reset () { s.reset (); }
  ଶୂନ୍ୟ clear () { s.clear (); }
  ଶୂନ୍ୟ invert () { s.invert (); }
  ତାର୍କିକ is_inverted () ଅଚଳ { ଫେରନ୍ତୁ s.is_inverted (); }
  ତାର୍କିକ is_empty () ଅଚଳ { ଫେରନ୍ତୁ s.is_empty (); }
  uint32_t hash () ଅଚଳ { ଫେରନ୍ତୁ s.hash (); }

  ଶୂନ୍ୟ add (hb_codepoint_t g) { s.add (g); }
  ଶୂନ୍ୟ add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  ତାର୍କିକ add_range (hb_codepoint_t first, hb_codepoint_t last) { ଫେରନ୍ତୁ s.add_range (first, last); }

  template <typename T>
  ଶୂନ୍ୟ add_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  { s.add_array (array, ଗଣନା_2, stride); }
  template <typename T>
  ଶୂନ୍ୟ add_array (ଅଚଳ hb_array_t<ଅଚଳ T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  ତାର୍କିକ add_sorted_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  { ଫେରନ୍ତୁ s.add_sorted_array (array, ଗଣନା_2, stride); }
  template <typename T>
  ତାର୍କିକ add_sorted_array (ଅଚଳ hb_sorted_array_t<ଅଚଳ T>& arr) { ଫେରନ୍ତୁ add_sorted_array (&arr, arr.len ()); }

  ଶୂନ୍ୟ del (hb_codepoint_t g) { s.del (g); }
  ଶୂନ୍ୟ del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  ତାର୍କିକ get (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ s.get (g); }
  ତାର୍କିକ may_have (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }

  /* Has interface. */
  ତାର୍କିକ operator [] (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ get (k); }
  ତାର୍କିକ has (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ (*ଏହି)[k]; }

  /* Predicate. */
  ତାର୍କିକ operator () (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); ଫେରନ୍ତୁ *ଏହି; }
  hb_sparseset_t& operator << (ଅଚଳ hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ଫେରନ୍ତୁ *ଏହି; }

  ତାର୍କିକ may_intersect (ଅଚଳ hb_sparseset_t &other) ଅଚଳ
  { ଫେରନ୍ତୁ s.may_intersect (other.s); }

  ତାର୍କିକ intersects (ଅଚଳ hb_sparseset_t &other) ଅଚଳ
  { ଫେରନ୍ତୁ s.intersects (other.s); }

  ତାର୍କିକ intersects (hb_codepoint_t first, hb_codepoint_t last) ଅଚଳ
  { ଫେରନ୍ତୁ s.intersects (first, last); }

  ଶୂନ୍ୟ set (ଅଚଳ hb_sparseset_t &other) { s.set (other.s); }

  ତାର୍କିକ is_equal (ଅଚଳ hb_sparseset_t &other) ଅଚଳ { ଫେରନ୍ତୁ s.is_equal (other.s); }
  ତାର୍କିକ operator == (ଅଚଳ hb_set_t &other) ଅଚଳ { ଫେରନ୍ତୁ is_equal (other); }
  ତାର୍କିକ operator != (ଅଚଳ hb_set_t &other) ଅଚଳ { ଫେରନ୍ତୁ !is_equal (other); }

  ତାର୍କିକ is_subset (ଅଚଳ hb_sparseset_t &larger_set) ଅଚଳ { ଫେରନ୍ତୁ s.is_subset (larger_set.s); }

  ଶୂନ୍ୟ union_ (ଅଚଳ hb_sparseset_t &other) { s.union_ (other.s); }
  ଶୂନ୍ୟ intersect (ଅଚଳ hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  ଶୂନ୍ୟ subtract (ଅଚଳ hb_sparseset_t &other) { s.subtract (other.s); }
  ଶୂନ୍ୟ symmetric_difference (ଅଚଳ hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  ତାର୍କିକ next (hb_codepoint_t *codepoint) ଅଚଳ { ଫେରନ୍ତୁ s.next (codepoint); }
  ତାର୍କିକ next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ଅଚଳ
  { ଫେରନ୍ତୁ s.next_bits (codepoint, bits); }
  ତାର୍କିକ previous (hb_codepoint_t *codepoint) ଅଚଳ { ଫେରନ୍ତୁ s.previous (codepoint); }
  ତାର୍କିକ next_range (hb_codepoint_t *first, hb_codepoint_t *last) ଅଚଳ
  { ଫେରନ୍ତୁ s.next_range (first, last); }
  ତାର୍କିକ previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ଅଚଳ
  { ଫେରନ୍ତୁ s.previous_range (first, last); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size) ଅଚଳ
  { ଫେରନ୍ତୁ s.next_many (codepoint, out, size); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_population () ଅଚଳ { ଫେରନ୍ତୁ s.get_population (); }
  ତାର୍କିକ get_singleton (hb_codepoint_t *codepoint) ଅଚଳ { ଫେରନ୍ତୁ s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ଅଚଳ { ଫେରନ୍ତୁ s.get_min (); }
  hb_codepoint_t get_max () ଅଚଳ { ଫେରନ୍ତୁ s.get_max (); }

  ସ୍ଥିର constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () ଅଚଳ { ଫେରନ୍ତୁ iter_t (ଏହି->s); }
  operator iter_t () ଅଚଳ { ଫେରନ୍ତୁ iter (); }
};

ସଂରଚନା hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_set_t () : sparseset () {};
  hb_set_t (ଅଚଳ hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (ଅଚଳ hb_set_t&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_set_t& operator = (hb_set_t&&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (ଅଚଳ Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); ଫେରନ୍ତୁ *ଏହି; }
  hb_set_t& operator << (ଅଚଳ hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); ଫେରନ୍ତୁ *ଏହି; }
};

static_assert (hb_set_t::INVALID == HB_SET_ମୂଲ୍ୟ_INVALID, "");


#endif /* HB_SET_HH */
