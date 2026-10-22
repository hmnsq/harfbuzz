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

#اگر_غیر_تعریف_شدہ HB_سیٹ_HH
#تعریف_کرو HB_سیٹ_HH

#شامل "hb.hh"
#شامل "hb-bit-set-invertible.hh"
#شامل "hb-bit-vector.hh" // Just to include


سانچہ <قسم_نام impl_t>
ساخت hb_sparseset_t
{
  ساکن constexpr منطقی realloc_move = سچ;

  hb_object_سرخی_t سرخی;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ثابت hb_sparseset_t& other) : hb_sparseset_t () { سیٹ (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& عامل = (ثابت hb_sparseset_t& other) { سیٹ (other); واپس *یہ; }
  hb_sparseset_t& عامل = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); واپس *یہ; }
  دوست خالی swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_تبادلہ (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    برائے (خودکار&& شے : lst)
      جوڑو (شے);
  }
  سانچہ <قسم_نام Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ثابت Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *یہ);
  }

  خالی init ()
  {
    hb_object_init (یہ);
    s.init ();
  }
  خالی fini ()
  {
    hb_object_fini (یہ);
    s.fini ();
  }

  صریح عامل منطقی () ثابت { واپس !is_empty (); }

  خالی err () { s.err (); }
  منطقی اندر_error () ثابت { واپس s.اندر_error (); }

  خالی alloc (غیرعلامتی sz) { s.alloc (sz); }
  خالی reset () { s.reset (); }
  خالی clear () { s.clear (); }
  خالی invert () { s.invert (); }
  منطقی is_inverted () ثابت { واپس s.is_inverted (); }
  منطقی is_empty () ثابت { واپس s.is_empty (); }
  uint32_t hash () ثابت { واپس s.hash (); }

  خالی جوڑو (hb_codepoint_t g) { s.جوڑو (g); }
  خالی جوڑو_بٹ (hb_codepoint_t g, uint64_t بٹ) { s.جوڑو_بٹ (g, بٹ); }
  منطقی جوڑو_range (hb_codepoint_t پہلا, hb_codepoint_t آخری) { واپس s.جوڑو_range (پہلا, آخری); }

  سانچہ <قسم_نام T>
  خالی جوڑو_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  { s.جوڑو_array (array, گنتی, stride); }
  سانچہ <قسم_نام T>
  خالی جوڑو_array (ثابت hb_array_t<ثابت T>& arr) { جوڑو_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچہ <قسم_نام T>
  منطقی جوڑو_sorted_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  { واپس s.جوڑو_sorted_array (array, گنتی, stride); }
  سانچہ <قسم_نام T>
  منطقی جوڑو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { واپس جوڑو_sorted_array (&arr, arr.len ()); }

  خالی del (hb_codepoint_t g) { s.del (g); }
  خالی del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  منطقی get (hb_codepoint_t g) ثابت { واپس s.get (g); }
  منطقی may_have (hb_codepoint_t g) ثابت { واپس get (g); }

  /* Has interface. */
  منطقی عامل [] (hb_codepoint_t k) ثابت { واپس get (k); }
  منطقی has (hb_codepoint_t k) ثابت { واپس (*یہ)[k]; }

  /* Predicate. */
  منطقی عامل () (hb_codepoint_t k) ثابت { واپس has (k); }

  /* Sink interface. */
  hb_sparseset_t& عامل << (hb_codepoint_t v)
  { جوڑو (v); واپس *یہ; }
  hb_sparseset_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { جوڑو_range (range.پہلا, range.second); واپس *یہ; }

  منطقی may_intersect (ثابت hb_sparseset_t &other) ثابت
  { واپس s.may_intersect (other.s); }

  منطقی intersects (ثابت hb_sparseset_t &other) ثابت
  { واپس s.intersects (other.s); }

  منطقی intersects (hb_codepoint_t پہلا, hb_codepoint_t آخری) ثابت
  { واپس s.intersects (پہلا, آخری); }

  خالی سیٹ (ثابت hb_sparseset_t &other) { s.سیٹ (other.s); }

  منطقی is_equal (ثابت hb_sparseset_t &other) ثابت { واپس s.is_equal (other.s); }
  منطقی عامل == (ثابت hb_سیٹ_t &other) ثابت { واپس is_equal (other); }
  منطقی عامل != (ثابت hb_سیٹ_t &other) ثابت { واپس !is_equal (other); }

  منطقی is_subset (ثابت hb_sparseset_t &larger_سیٹ) ثابت { واپس s.is_subset (larger_سیٹ.s); }

  خالی union_ (ثابت hb_sparseset_t &other) { s.union_ (other.s); }
  خالی intersect (ثابت hb_sparseset_t &other,
		  hb_سمتیہ_t<غیرعلامتی> *workspace = خالی_اشاریہ)
  { s.intersect (other.s, workspace); }
  خالی تفریق (ثابت hb_sparseset_t &other) { s.تفریق (other.s); }
  خالی متشاکل_difference (ثابت hb_sparseset_t &other) { s.متشاکل_difference (other.s); }

  منطقی next (hb_codepoint_t *codepoint) ثابت { واپس s.next (codepoint); }
  منطقی اگلا_بٹ (hb_codepoint_t *codepoint, uint64_t *بٹ) ثابت
  { واپس s.اگلا_بٹ (codepoint, بٹ); }
  منطقی پچھلا (hb_codepoint_t *codepoint) ثابت { واپس s.پچھلا (codepoint); }
  منطقی اگلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  { واپس s.اگلا_range (پہلا, آخری); }
  منطقی پچھلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  { واپس s.پچھلا_range (پہلا, آخری); }
  غیرعلامتی صحیح اگلا_many (hb_codepoint_t codepoint, hb_codepoint_t *برآمد, غیرعلامتی صحیح سائز) ثابت
  { واپس s.اگلا_many (codepoint, برآمد, سائز); }

  غیرعلامتی صحیح get_population () ثابت { واپس s.get_population (); }
  منطقی get_singleton (hb_codepoint_t *codepoint) ثابت { واپس s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ثابت { واپس s.get_min (); }
  hb_codepoint_t get_max () ثابت { واپس s.get_max (); }

  ساکن constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  استعمال iter_t = قسم_نام impl_t::iter_t;
  iter_t iter () ثابت { واپس iter_t (یہ->s); }
  عامل iter_t () ثابت { واپس iter (); }
};

ساخت hb_سیٹ_t : hb_sparseset_t<hb_ثنائی_ہندسہ_سیٹ_invertible_t>
{
  استعمال sparseset = hb_sparseset_t<hb_ثنائی_ہندسہ_سیٹ_invertible_t>;

  ~hb_سیٹ_t () = طےشدہ;
  hb_سیٹ_t () : sparseset () {};
  hb_سیٹ_t (ثابت hb_سیٹ_t &o) : sparseset ((sparseset &) o) {};
  hb_سیٹ_t (hb_سیٹ_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_سیٹ_t& عامل = (ثابت hb_سیٹ_t&) = طےشدہ;
  hb_سیٹ_t& عامل = (hb_سیٹ_t&&) = طےشدہ;
  hb_سیٹ_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  سانچہ <قسم_نام Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_سیٹ_t (ثابت Iterable &o) : sparseset (o) {}

  hb_سیٹ_t& عامل << (hb_codepoint_t v)
  { sparseset::عامل<< (v); واپس *یہ; }
  hb_سیٹ_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { sparseset::عامل<< (range); واپس *یہ; }
};

static_assert (hb_سیٹ_t::INVALID == HB_سیٹ_قدر_INVALID, "");


#اگر_ختم /* HB_SET_HH */
