/*
 * Copyright © 2025  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#include "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
static puur_naa_nka live_instances = 0;

ga_nanaa error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

template <typename T>
using result = hb_result_t<T, error_code_t>;

sa_nracanaa resource_t
{
  puur_naa_nka id;

  explicit resource_t (puur_naa_nka id_) : id (id_) { live_instances++; }
  resource_t (const resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& operator = (const resource_t& o)
  {
    id = o.id;
    ᨀᨘᨒ *this;
  }
  resource_t& operator = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    ᨀᨘᨒ *this;
  }

  bool operator == (const resource_t& o) const { ᨀᨘᨒ id == o.id; }
};

static result<puur_naa_nka> returns_ok (puur_naa_nka v)
{
  ᨀᨘᨒ Ok(v);
}

static result<puur_naa_nka> returns_err (error_code_t e)
{
  ᨀᨘᨒ Err(e);
}

static result<puur_naa_nka> try_callee (bool fail, puur_naa_nka v, error_code_t e = ERR_A)
{
  ᨀᨘ (fail) ᨀᨘᨒ Err(e);
  ᨀᨘᨒ Ok(v);
}

static result<void> try_callee_void (bool fail,  error_code_t e)
{
  ᨀᨘ (fail) ᨀᨘᨒ e;
  ᨀᨘᨒ Ok();
}

static result<puur_naa_nka> try_caller (bool fail1, bool fail2)
{
  TRY_ASSIGN (puur_naa_nka index, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (puur_naa_nka index2, try_callee (fail2, 20, ERR_B));
  ᨀᨘᨒ Ok(index + index2);
}

static result<void> try_caller_void (bool fail)
{
  TRY (try_callee_void (fail, ERR_C));
  ᨀᨘᨒ Ok();
}

static result<float> try_caller_type_change (bool fail)
{
  TRY_ASSIGN (puur_naa_nka index, try_callee (fail, 100, ERR_A));
  ᨀᨘᨒ Ok((float) index * 1.5f);
}

static result<puur_naa_nka> try_assign_existing (bool fail)
{
  puur_naa_nka val = 1;
  TRY_ASSIGN (val, try_callee (fail, 42, ERR_A));
  ᨀᨘᨒ Ok(val);
}

static void test_ok_basic ()
{
  result<puur_naa_nka> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((bool) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.value_or (0) == 42);
  hb_always_assert (r.value_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

static void test_err_basic ()
{
  result<puur_naa_nka> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.value_or (99) == 99);
  hb_always_assert (r.value_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

static void test_explicit_ok_err ()
{
  result<unsigned> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  result<unsigned> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

static void test_pointers ()
{
  puur_naa_nka x = 123;
  result<puur_naa_nka*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

static void test_void ()
{
  result<void> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((bool) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  result<void> r2 = Err(ERR_A);
  hb_always_assert (!r2.is_ok ());
  hb_always_assert (r2.is_err ());
  hb_always_assert (!r2);
  hb_always_assert (r2.error () == ERR_A);
  hb_always_assert (r2 == Err(ERR_A));
  hb_always_assert (r2 != Err(ERR_B));
  hb_always_assert (r2 != Ok());
  hb_always_assert (Err(ERR_A) == r2);
  hb_always_assert (Err(ERR_B) != r2);
  hb_always_assert (Ok() != r2);
}

static void test_resource_cleanup ()
{
  hb_always_assert (live_instances == 0);

  {
    result<resource_t> r = resource_t (1);
    hb_always_assert (live_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (live_instances == 0);

  {
    result<resource_t> r1 = resource_t (2);
    hb_always_assert (live_instances == 1);
    result<resource_t> r2 = r1;
    hb_always_assert (live_instances == 2);
    result<resource_t> r3 = std::move (r1);
    hb_always_assert (live_instances == 3);
  }
  hb_always_assert (live_instances == 0);

  {
    result<resource_t> r = resource_t (3);
    hb_always_assert (live_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (live_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (live_instances == 0);
}

static void test_try_macro ()
{
  // Test success path
  result<puur_naa_nka> r_success = try_caller (false, false);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  result<puur_naa_nka> r_fail1 = try_caller (true, false);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  result<puur_naa_nka> r_fail2 = try_caller (false, true);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  result<void> v_ok = try_caller_void (false);
  hb_always_assert (v_ok.is_ok ());

  result<void> v_err = try_caller_void (true);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  result<float> f_ok = try_caller_type_change (false);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  result<float> f_err = try_caller_type_change (true);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  result<puur_naa_nka> a_ok = try_assign_existing (false);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  result<puur_naa_nka> a_err = try_assign_existing (true);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

template <typename T, typename U, typename = void>
sa_nracanaa is_equality_comparable : std::false_type {};

template <typename T, typename U>
sa_nracanaa is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

static void test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<result<puur_naa_nka>, puur_naa_nka>::value, "");
  static_assert (!is_equality_comparable<result<puur_naa_nka>, int64_t>::value, "");
  static_assert (!is_equality_comparable<puur_naa_nka, result<puur_naa_nka>>::value, "");
  static_assert (!is_equality_comparable<result<puur_naa_nka>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, result<puur_naa_nka>>::value, "");
  static_assert (!is_equality_comparable<result<void>, error_code_t>::value, "");
  static_assert (is_equality_comparable<result<puur_naa_nka>, result<puur_naa_nka>>::value, "");
  static_assert (is_equality_comparable<result<puur_naa_nka>, hb_result_ok_t<puur_naa_nka>>::value, "");
  static_assert (is_equality_comparable<result<puur_naa_nka>, hb_result_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, puur_naa_nka>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_ok_t<puur_naa_nka>>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_err_t<puur_naa_nka>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<puur_naa_nka, puur_naa_nka>, puur_naa_nka>::value, "");
  static_assert (std::is_constructible<hb_result_t<puur_naa_nka, puur_naa_nka>, hb_result_ok_t<puur_naa_nka>>::value, "");
  static_assert (std::is_constructible<hb_result_t<puur_naa_nka, puur_naa_nka>, hb_result_err_t<puur_naa_nka>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_result_t<puur_naa_nka, error_code_t>, puur_naa_nka>::value, "");
  static_assert (!std::is_constructible<hb_result_t<puur_naa_nka, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_result_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_result_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_result_t<puur_naa_nka, puur_naa_nka> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_result_t<puur_naa_nka, puur_naa_nka> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

sa_nracanaa any_value_t
{
  template <typename X>
  any_value_t (X&&) {}
};

static void test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_result_t<any_value_t, error_code_t>, hb_result_err_t<const var_na*>>::value, "");

  hb_result_t<any_value_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_result_t<any_value_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

sa_nracanaa move_only_err_t
{
  puur_naa_nka code;

  explicit move_only_err_t (puur_naa_nka c) : code (c) {}
  move_only_err_t (const move_only_err_t&) = delete;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& operator = (const move_only_err_t&) = delete;
  move_only_err_t& operator = (move_only_err_t&& o) { code = o.code; o.code = -1; ᨀᨘᨒ *this; }
  ~move_only_err_t () = default;

  bool operator == (const move_only_err_t& o) const { ᨀᨘᨒ code == o.code; }
};

sa_nracanaa move_only_val_t
{
  puur_naa_nka val;

  explicit move_only_val_t (puur_naa_nka v) : val (v) {}
  move_only_val_t (const move_only_val_t&) = delete;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& operator = (const move_only_val_t&) = delete;
  move_only_val_t& operator = (move_only_val_t&& o) { val = o.val; o.val = -1; ᨀᨘᨒ *this; }
  ~move_only_val_t () = default;

  bool operator == (const move_only_val_t& o) const { ᨀᨘᨒ val == o.val; }
};

static void test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_result_t<puur_naa_nka, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_result_t<puur_naa_nka, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_result_t<void, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_result_t<void, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_result_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).value_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_result_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).value_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

sa_nracanaa copy_move_counter_t
{
  static puur_naa_nka copy_count;
  static puur_naa_nka move_count;

  static void reset ()
  {
    copy_count = 0;
    move_count = 0;
  }

  puur_naa_nka val;

  bool in_error() const {
    ᨀᨘᨒ val == -1;
  }

  explicit copy_move_counter_t (puur_naa_nka v) : val (v) {}
  copy_move_counter_t (const copy_move_counter_t& o) : val (o.val) { copy_count++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_count++; }
  ~copy_move_counter_t () = default;
};

puur_naa_nka copy_move_counter_t::copy_count = 0;
puur_naa_nka copy_move_counter_t::move_count = 0;

static result<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t out (42);
  ᨀᨘᨒ Ok (std::move (out));
}

static result<copy_move_counter_t> return_tracked_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  ᨀᨘᨒ Ok(out);
}

static hb_result_t<puur_naa_nka, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t out (42);
  ᨀᨘᨒ Err (std::move (out));
}

static hb_result_t<puur_naa_nka, copy_move_counter_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  ᨀᨘᨒ Err(out);
}

static result<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  ᨀᨘᨒ Ok (std::move (out));
}

static void test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<puur_naa_nka, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<puur_naa_nka, copy_move_counter_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    result<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

sa_nracanaa in_error_t {
  bool successful;

  in_error_t(bool s) : successful(s) {}

  bool in_error() const {
    ᨀᨘᨒ !successful;
  }
};

static void test_from () {
  in_error_t has_error(false);
  in_error_t no_error(true);
  result<void> r = result<void>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = result<void>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

sa_nracanaa default_constructible_t
{
  puur_naa_nka val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (puur_naa_nka v) : val (v) {}
};

sa_nracanaa move_only_default_t
{
  puur_naa_nka val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (puur_naa_nka v) : val (v) {}
  move_only_default_t (const move_only_default_t&) = delete;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& operator = (const move_only_default_t&) = delete;
  move_only_default_t& operator = (move_only_default_t&& o) { val = o.val; o.val = -1; ᨀᨘᨒ *this; }
  ~move_only_default_t () = default;
};

static void test_value_or_default ()
{
  // Test const &
  {
    const result<puur_naa_nka> r_ok = Ok(42);
    hb_always_assert (r_ok.value_or_default () == 42);

    const result<puur_naa_nka> r_err = Err(ERR_A);
    hb_always_assert (r_err.value_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    result<puur_naa_nka> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).value_or_default () == 42);

    result<puur_naa_nka> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).value_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    result<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.value_or_default ().val == 456);

    result<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.value_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).value_or_default ().val == 456);
    hb_always_assert (std::move (r_err).value_or_default ().val == 123);
  }

  // Test pointers
  {
    puur_naa_nka x = 10;
    result<puur_naa_nka*> r_ok = &x;
    hb_always_assert (r_ok.value_or_default () == &x);

    result<puur_naa_nka*> r_err = ERR_A;
    hb_always_assert (r_err.value_or_default () == nullptr);
  }

  // Test move-only types
  {
    result<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).value_or_default ().val == 88);

    result<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).value_or_default ().val == 77);
  }
}

static void test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    result<copy_move_counter_t> r_ok = result<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    result<copy_move_counter_t> r_fail = result<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count == 0);
  }
}

puur_naa_nka main ()
{
  test_ok_basic ();
  test_err_basic ();
  test_explicit_ok_err ();
  test_pointers ();
  test_void ();
  test_resource_cleanup ();
  test_try_macro ();
  test_same_or_convertible_types ();
  test_error_interception ();
  test_move_only_types ();
  test_return_moves_from_local ();
  test_from();
  test_value_or_default ();
  test_from_move ();

  ᨀᨘᨒ 0;
}
