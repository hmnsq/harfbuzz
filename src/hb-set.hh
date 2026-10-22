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

#که_نه_تعریف_شوی HB_سېټ_HH
#تعریف_کړه HB_سېټ_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set-invertible.hh"
#ضمن "hb-bit-vector.hh" // Just to include


کينډۍ <ډول_نوم impl_t>
بنية hb_sparseset_t
{
  ساكن constexpr منطقي realloc_move = ریښتیا;

  hb_object_سرلیک_t سرلیک;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ثابت hb_sparseset_t& other) : hb_sparseset_t () { سېټ (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& چلوونکی = (ثابت hb_sparseset_t& other) { سېټ (other); أرجع *هذا; }
  hb_sparseset_t& چلوونکی = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); أرجع *هذا; }
  ملګری فراغ swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_بدلون (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    لكل (خپلکاری&& توکی : lst)
      جمع_کړه (توکی);
  }
  کينډۍ <ډول_نوم Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ثابت Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *هذا);
  }

  فراغ init ()
  {
    hb_object_init (هذا);
    s.init ();
  }
  فراغ fini ()
  {
    hb_object_fini (هذا);
    s.fini ();
  }

  څرګند چلوونکی منطقي () ثابت { أرجع !is_empty (); }

  فراغ err () { s.err (); }
  منطقي دننه_error () ثابت { أرجع s.دننه_error (); }

  فراغ alloc (بینښه sz) { s.alloc (sz); }
  فراغ reset () { s.reset (); }
  فراغ clear () { s.clear (); }
  فراغ invert () { s.invert (); }
  منطقي is_inverted () ثابت { أرجع s.is_inverted (); }
  منطقي is_empty () ثابت { أرجع s.is_empty (); }
  uint32_t hash () ثابت { أرجع s.hash (); }

  فراغ جمع_کړه (hb_codepoint_t g) { s.جمع_کړه (g); }
  فراغ جمع_کړه_بټونه (hb_codepoint_t g, uint64_t بټونه) { s.جمع_کړه_بټونه (g, بټونه); }
  منطقي جمع_کړه_range (hb_codepoint_t لومړی, hb_codepoint_t وروستی) { أرجع s.جمع_کړه_range (لومړی, وروستی); }

  کينډۍ <ډول_نوم T>
  فراغ جمع_کړه_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  { s.جمع_کړه_array (array, شمېر, stride); }
  کينډۍ <ډول_نوم T>
  فراغ جمع_کړه_array (ثابت hb_array_t<ثابت T>& arr) { جمع_کړه_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  کينډۍ <ډول_نوم T>
  منطقي جمع_کړه_sorted_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  { أرجع s.جمع_کړه_sorted_array (array, شمېر, stride); }
  کينډۍ <ډول_نوم T>
  منطقي جمع_کړه_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جمع_کړه_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { أرجع s.get (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي چلوونکی [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }

  /* Predicate. */
  منطقي چلوونکی () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_sparseset_t& چلوونکی << (hb_codepoint_t v)
  { جمع_کړه (v); أرجع *هذا; }
  hb_sparseset_t& چلوونکی << (ثابت hb_codepoint_pair_t& range)
  { جمع_کړه_range (range.لومړی, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.may_intersect (other.s); }

  منطقي intersects (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.intersects (other.s); }

  منطقي intersects (hb_codepoint_t لومړی, hb_codepoint_t وروستی) ثابت
  { أرجع s.intersects (لومړی, وروستی); }

  فراغ سېټ (ثابت hb_sparseset_t &other) { s.سېټ (other.s); }

  منطقي is_equal (ثابت hb_sparseset_t &other) ثابت { أرجع s.is_equal (other.s); }
  منطقي چلوونکی == (ثابت hb_سېټ_t &other) ثابت { أرجع is_equal (other); }
  منطقي چلوونکی != (ثابت hb_سېټ_t &other) ثابت { أرجع !is_equal (other); }

  منطقي is_subset (ثابت hb_sparseset_t &larger_سېټ) ثابت { أرجع s.is_subset (larger_سېټ.s); }

  فراغ union_ (ثابت hb_sparseset_t &other) { s.union_ (other.s); }
  فراغ intersect (ثابت hb_sparseset_t &other,
		  hb_ویکټور_t<بینښه> *workspace = تش_اشاره)
  { s.intersect (other.s, workspace); }
  فراغ کمول (ثابت hb_sparseset_t &other) { s.کمول (other.s); }
  فراغ متناظر_difference (ثابت hb_sparseset_t &other) { s.متناظر_difference (other.s); }

  منطقي next (hb_codepoint_t *codepoint) ثابت { أرجع s.next (codepoint); }
  منطقي بل_بټونه (hb_codepoint_t *codepoint, uint64_t *بټونه) ثابت
  { أرجع s.بل_بټونه (codepoint, بټونه); }
  منطقي پخوانی (hb_codepoint_t *codepoint) ثابت { أرجع s.پخوانی (codepoint); }
  منطقي بل_range (hb_codepoint_t *لومړی, hb_codepoint_t *وروستی) ثابت
  { أرجع s.بل_range (لومړی, وروستی); }
  منطقي پخوانی_range (hb_codepoint_t *لومړی, hb_codepoint_t *وروستی) ثابت
  { أرجع s.پخوانی_range (لومړی, وروستی); }
  بینښه صحيح بل_many (hb_codepoint_t codepoint, hb_codepoint_t *بهر, بینښه صحيح اندازه) ثابت
  { أرجع s.بل_many (codepoint, بهر, اندازه); }

  بینښه صحيح get_population () ثابت { أرجع s.get_population (); }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت { أرجع s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ثابت { أرجع s.get_min (); }
  hb_codepoint_t get_max () ثابت { أرجع s.get_max (); }

  ساكن constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  کارول iter_t = ډول_نوم impl_t::iter_t;
  iter_t iter () ثابت { أرجع iter_t (هذا->s); }
  چلوونکی iter_t () ثابت { أرجع iter (); }
};

بنية hb_سېټ_t : hb_sparseset_t<hb_بټ_سېټ_invertible_t>
{
  کارول sparseset = hb_sparseset_t<hb_بټ_سېټ_invertible_t>;

  ~hb_سېټ_t () = افتراضي;
  hb_سېټ_t () : sparseset () {};
  hb_سېټ_t (ثابت hb_سېټ_t &o) : sparseset ((sparseset &) o) {};
  hb_سېټ_t (hb_سېټ_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_سېټ_t& چلوونکی = (ثابت hb_سېټ_t&) = افتراضي;
  hb_سېټ_t& چلوونکی = (hb_سېټ_t&&) = افتراضي;
  hb_سېټ_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  کينډۍ <ډول_نوم Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_سېټ_t (ثابت Iterable &o) : sparseset (o) {}

  hb_سېټ_t& چلوونکی << (hb_codepoint_t v)
  { sparseset::چلوونکی<< (v); أرجع *هذا; }
  hb_سېټ_t& چلوونکی << (ثابت hb_codepoint_pair_t& range)
  { sparseset::چلوونکی<< (range); أرجع *هذا; }
};

static_assert (hb_سېټ_t::INVALID == HB_سېټ_ارزښت_INVALID, "");


#د_که_پای /* HB_SET_HH */
