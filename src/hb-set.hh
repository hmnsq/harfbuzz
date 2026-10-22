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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-bit-set-invertible.hh"
#અન્તર્ભાવ "hb-bit-vector.hh" // Just to include


template <typename impl_t>
સંરચના hb_sparseset_t
{
  સ્થિર constexpr તાર્કિક realloc_move = સાચું;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (અચલ hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (અચલ hb_sparseset_t& other) { set (other); પાછા *આ; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); પાછા *આ; }
  friend શૂન્ય swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    માટે (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (અચલ Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *આ);
  }

  શૂન્ય init ()
  {
    hb_object_init (આ);
    s.init ();
  }
  શૂન્ય fini ()
  {
    hb_object_fini (આ);
    s.fini ();
  }

  explicit operator તાર્કિક () અચલ { પાછા !is_empty (); }

  શૂન્ય err () { s.err (); }
  તાર્કિક in_error () અચલ { પાછા s.in_error (); }

  શૂન્ય alloc (unsigned sz) { s.alloc (sz); }
  શૂન્ય reset () { s.reset (); }
  શૂન્ય clear () { s.clear (); }
  શૂન્ય invert () { s.invert (); }
  તાર્કિક is_inverted () અચલ { પાછા s.is_inverted (); }
  તાર્કિક is_empty () અચલ { પાછા s.is_empty (); }
  uint32_t hash () અચલ { પાછા s.hash (); }

  શૂન્ય add (hb_codepoint_t g) { s.add (g); }
  શૂન્ય add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  તાર્કિક add_range (hb_codepoint_t first, hb_codepoint_t last) { પાછા s.add_range (first, last); }

  template <typename T>
  શૂન્ય add_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  { s.add_array (array, ગણતરી, stride); }
  template <typename T>
  શૂન્ય add_array (અચલ hb_array_t<અચલ T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  તાર્કિક add_sorted_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  { પાછા s.add_sorted_array (array, ગણતરી, stride); }
  template <typename T>
  તાર્કિક add_sorted_array (અચલ hb_sorted_array_t<અચલ T>& arr) { પાછા add_sorted_array (&arr, arr.len ()); }

  શૂન્ય del (hb_codepoint_t g) { s.del (g); }
  શૂન્ય del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  તાર્કિક get (hb_codepoint_t g) અચલ { પાછા s.get (g); }
  તાર્કિક may_have (hb_codepoint_t g) અચલ { પાછા get (g); }

  /* Has interface. */
  તાર્કિક operator [] (hb_codepoint_t k) અચલ { પાછા get (k); }
  તાર્કિક has (hb_codepoint_t k) અચલ { પાછા (*આ)[k]; }

  /* Predicate. */
  તાર્કિક operator () (hb_codepoint_t k) અચલ { પાછા has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); પાછા *આ; }
  hb_sparseset_t& operator << (અચલ hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); પાછા *આ; }

  તાર્કિક may_intersect (અચલ hb_sparseset_t &other) અચલ
  { પાછા s.may_intersect (other.s); }

  તાર્કિક intersects (અચલ hb_sparseset_t &other) અચલ
  { પાછા s.intersects (other.s); }

  તાર્કિક intersects (hb_codepoint_t first, hb_codepoint_t last) અચલ
  { પાછા s.intersects (first, last); }

  શૂન્ય set (અચલ hb_sparseset_t &other) { s.set (other.s); }

  તાર્કિક is_equal (અચલ hb_sparseset_t &other) અચલ { પાછા s.is_equal (other.s); }
  તાર્કિક operator == (અચલ hb_set_t &other) અચલ { પાછા is_equal (other); }
  તાર્કિક operator != (અચલ hb_set_t &other) અચલ { પાછા !is_equal (other); }

  તાર્કિક is_subset (અચલ hb_sparseset_t &larger_set) અચલ { પાછા s.is_subset (larger_set.s); }

  શૂન્ય union_ (અચલ hb_sparseset_t &other) { s.union_ (other.s); }
  શૂન્ય intersect (અચલ hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  શૂન્ય subtract (અચલ hb_sparseset_t &other) { s.subtract (other.s); }
  શૂન્ય symmetric_difference (અચલ hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  તાર્કિક next (hb_codepoint_t *codepoint) અચલ { પાછા s.next (codepoint); }
  તાર્કિક next_bits (hb_codepoint_t *codepoint, uint64_t *bits) અચલ
  { પાછા s.next_bits (codepoint, bits); }
  તાર્કિક previous (hb_codepoint_t *codepoint) અચલ { પાછા s.previous (codepoint); }
  તાર્કિક next_range (hb_codepoint_t *first, hb_codepoint_t *last) અચલ
  { પાછા s.next_range (first, last); }
  તાર્કિક previous_range (hb_codepoint_t *first, hb_codepoint_t *last) અચલ
  { પાછા s.previous_range (first, last); }
  unsigned પૂર્ણાંક next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned પૂર્ણાંક size) અચલ
  { પાછા s.next_many (codepoint, out, size); }

  unsigned પૂર્ણાંક get_population () અચલ { પાછા s.get_population (); }
  તાર્કિક get_singleton (hb_codepoint_t *codepoint) અચલ { પાછા s.get_singleton (codepoint); }
  hb_codepoint_t get_min () અચલ { પાછા s.get_min (); }
  hb_codepoint_t get_max () અચલ { પાછા s.get_max (); }

  સ્થિર constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () અચલ { પાછા iter_t (આ->s); }
  operator iter_t () અચલ { પાછા iter (); }
};

સંરચના hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = મૂળભૂત;
  hb_set_t () : sparseset () {};
  hb_set_t (અચલ hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (અચલ hb_set_t&) = મૂળભૂત;
  hb_set_t& operator = (hb_set_t&&) = મૂળભૂત;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (અચલ Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); પાછા *આ; }
  hb_set_t& operator << (અચલ hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); પાછા *આ; }
};

static_assert (hb_set_t::INVALID == HB_SET_કિંમત_INVALID, "");


#endif /* HB_SET_HH */
