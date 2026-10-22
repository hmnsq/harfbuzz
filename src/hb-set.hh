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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-bit-set-invertible.hh"
#अन्तर्भाव "hb-bit-vector.hh" // Just to include


ढाँचा <typename impl_t>
संरचना hb_sparseset_t
{
  स्थिर constexpr तार्किक realloc_move = सत्य;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ध्रुव hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& चालक = (ध्रुव hb_sparseset_t& other) { set (other); प्रत्यागम *एतत्; }
  hb_sparseset_t& चालक = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); प्रत्यागम *एतत्; }
  मित्र शून्य swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    प्रति (स्वतः&& item : lst)
      add (item);
  }
  ढाँचा <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ध्रुव Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *एतत्);
  }

  शून्य init ()
  {
    hb_object_init (एतत्);
    s.init ();
  }
  शून्य fini ()
  {
    hb_object_fini (एतत्);
    s.fini ();
  }

  explicit चालक तार्किक () ध्रुव { प्रत्यागम !is_empty (); }

  शून्य err () { s.err (); }
  तार्किक in_error () ध्रुव { प्रत्यागम s.in_error (); }

  शून्य alloc (अचिन्हित sz) { s.alloc (sz); }
  शून्य reset () { s.reset (); }
  शून्य clear () { s.clear (); }
  शून्य invert () { s.invert (); }
  तार्किक is_inverted () ध्रुव { प्रत्यागम s.is_inverted (); }
  तार्किक is_empty () ध्रुव { प्रत्यागम s.is_empty (); }
  uint32_t hash () ध्रुव { प्रत्यागम s.hash (); }

  शून्य add (hb_codepoint_t g) { s.add (g); }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  तार्किक add_range (hb_codepoint_t first, hb_codepoint_t last) { प्रत्यागम s.add_range (first, last); }

  ढाँचा <typename T>
  शून्य add_array (ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  { s.add_array (array, गिनती, stride); }
  ढाँचा <typename T>
  शून्य add_array (ध्रुव hb_array_t<ध्रुव T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename T>
  तार्किक add_sorted_array (ध्रुव T *array, अचिन्हित पूर्णाङ्क गिनती, अचिन्हित पूर्णाङ्क stride=परिमाण(T))
  { प्रत्यागम s.add_sorted_array (array, गिनती, stride); }
  ढाँचा <typename T>
  तार्किक add_sorted_array (ध्रुव hb_sorted_array_t<ध्रुव T>& arr) { प्रत्यागम add_sorted_array (&arr, arr.len ()); }

  शून्य del (hb_codepoint_t g) { s.del (g); }
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  तार्किक get (hb_codepoint_t g) ध्रुव { प्रत्यागम s.get (g); }
  तार्किक may_have (hb_codepoint_t g) ध्रुव { प्रत्यागम get (g); }

  /* Has interface. */
  तार्किक चालक [] (hb_codepoint_t k) ध्रुव { प्रत्यागम get (k); }
  तार्किक has (hb_codepoint_t k) ध्रुव { प्रत्यागम (*एतत्)[k]; }

  /* Predicate. */
  तार्किक चालक () (hb_codepoint_t k) ध्रुव { प्रत्यागम has (k); }

  /* Sink interface. */
  hb_sparseset_t& चालक << (hb_codepoint_t v)
  { add (v); प्रत्यागम *एतत्; }
  hb_sparseset_t& चालक << (ध्रुव hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); प्रत्यागम *एतत्; }

  तार्किक may_intersect (ध्रुव hb_sparseset_t &other) ध्रुव
  { प्रत्यागम s.may_intersect (other.s); }

  तार्किक intersects (ध्रुव hb_sparseset_t &other) ध्रुव
  { प्रत्यागम s.intersects (other.s); }

  तार्किक intersects (hb_codepoint_t first, hb_codepoint_t last) ध्रुव
  { प्रत्यागम s.intersects (first, last); }

  शून्य set (ध्रुव hb_sparseset_t &other) { s.set (other.s); }

  तार्किक is_equal (ध्रुव hb_sparseset_t &other) ध्रुव { प्रत्यागम s.is_equal (other.s); }
  तार्किक चालक == (ध्रुव hb_set_t &other) ध्रुव { प्रत्यागम is_equal (other); }
  तार्किक चालक != (ध्रुव hb_set_t &other) ध्रुव { प्रत्यागम !is_equal (other); }

  तार्किक is_subset (ध्रुव hb_sparseset_t &larger_set) ध्रुव { प्रत्यागम s.is_subset (larger_set.s); }

  शून्य union_ (ध्रुव hb_sparseset_t &other) { s.union_ (other.s); }
  शून्य intersect (ध्रुव hb_sparseset_t &other,
		  hb_vector_t<अचिन्हित> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  शून्य subtract (ध्रुव hb_sparseset_t &other) { s.subtract (other.s); }
  शून्य symmetric_difference (ध्रुव hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  तार्किक next (hb_codepoint_t *codepoint) ध्रुव { प्रत्यागम s.next (codepoint); }
  तार्किक next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ध्रुव
  { प्रत्यागम s.next_bits (codepoint, bits); }
  तार्किक previous (hb_codepoint_t *codepoint) ध्रुव { प्रत्यागम s.previous (codepoint); }
  तार्किक next_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  { प्रत्यागम s.next_range (first, last); }
  तार्किक previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  { प्रत्यागम s.previous_range (first, last); }
  अचिन्हित पूर्णाङ्क next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, अचिन्हित पूर्णाङ्क size) ध्रुव
  { प्रत्यागम s.next_many (codepoint, out, size); }

  अचिन्हित पूर्णाङ्क get_population () ध्रुव { प्रत्यागम s.get_population (); }
  तार्किक get_singleton (hb_codepoint_t *codepoint) ध्रुव { प्रत्यागम s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ध्रुव { प्रत्यागम s.get_min (); }
  hb_codepoint_t get_max () ध्रुव { प्रत्यागम s.get_max (); }

  स्थिर constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () ध्रुव { प्रत्यागम iter_t (एतत्->s); }
  चालक iter_t () ध्रुव { प्रत्यागम iter (); }
};

संरचना hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = स्वभाव;
  hb_set_t () : sparseset () {};
  hb_set_t (ध्रुव hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& चालक = (ध्रुव hb_set_t&) = स्वभाव;
  hb_set_t& चालक = (hb_set_t&&) = स्वभाव;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (ध्रुव Iterable &o) : sparseset (o) {}

  hb_set_t& चालक << (hb_codepoint_t v)
  { sparseset::चालक<< (v); प्रत्यागम *एतत्; }
  hb_set_t& चालक << (ध्रुव hb_codepoint_pair_t& range)
  { sparseset::चालक<< (range); प्रत्यागम *एतत्; }
};

static_assert (hb_set_t::INVALID == HB_SET_मूल्य_INVALID, "");


#endif /* HB_SET_HH */
