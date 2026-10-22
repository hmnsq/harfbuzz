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

#અન્તર્ભાવ "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
સ્થિર પૂર્ણાંક live_instances = 0;

ગણના error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

template <typename T>
using result = hb_result_t<T, error_code_t>;

સંરચના resource_t
{
  પૂર્ણાંક id;

  explicit resource_t (પૂર્ણાંક id_) : id (id_) { live_instances++; }
  resource_t (અચલ resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& operator = (અચલ resource_t& o)
  {
    id = o.id;
    પાછા *આ;
  }
  resource_t& operator = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    પાછા *આ;
  }

  તાર્કિક operator == (અચલ resource_t& o) અચલ { પાછા id == o.id; }
};

સ્થિર result<પૂર્ણાંક> returns_ok (પૂર્ણાંક v)
{
  પાછા Ok(v);
}

સ્થિર result<પૂર્ણાંક> returns_err (error_code_t e)
{
  પાછા Err(e);
}

સ્થિર result<પૂર્ણાંક> try_callee (તાર્કિક fail, પૂર્ણાંક v, error_code_t e = ERR_A)
{
  જો (fail) પાછા Err(e);
  પાછા Ok(v);
}

સ્થિર result<શૂન્ય> try_callee_void (તાર્કિક fail,  error_code_t e)
{
  જો (fail) પાછા e;
  પાછા Ok();
}

સ્થિર result<પૂર્ણાંક> try_caller (તાર્કિક fail1, તાર્કિક fail2)
{
  TRY_ASSIGN (પૂર્ણાંક index, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (પૂર્ણાંક index2, try_callee (fail2, 20, ERR_B));
  પાછા Ok(index + index2);
}

સ્થિર result<શૂન્ય> try_caller_void (તાર્કિક fail)
{
  TRY (try_callee_void (fail, ERR_C));
  પાછા Ok();
}

સ્થિર result<દશાંશ> try_caller_type_change (તાર્કિક fail)
{
  TRY_ASSIGN (પૂર્ણાંક index, try_callee (fail, 100, ERR_A));
  પાછા Ok((દશાંશ) index * 1.5f);
}

સ્થિર result<પૂર્ણાંક> try_assign_existing (તાર્કિક fail)
{
  પૂર્ણાંક val = 1;
  TRY_ASSIGN (val, try_callee (fail, 42, ERR_A));
  પાછા Ok(val);
}

સ્થિર શૂન્ય test_ok_basic ()
{
  result<પૂર્ણાંક> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((તાર્કિક) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.કિંમત_or (0) == 42);
  hb_always_assert (r.કિંમત_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

સ્થિર શૂન્ય test_err_basic ()
{
  result<પૂર્ણાંક> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.કિંમત_or (99) == 99);
  hb_always_assert (r.કિંમત_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

સ્થિર શૂન્ય test_explicit_ok_err ()
{
  result<unsigned> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  result<unsigned> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

સ્થિર શૂન્ય test_pointers ()
{
  પૂર્ણાંક x = 123;
  result<પૂર્ણાંક*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

સ્થિર શૂન્ય test_void ()
{
  result<શૂન્ય> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((તાર્કિક) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  result<શૂન્ય> r2 = Err(ERR_A);
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

સ્થિર શૂન્ય test_resource_cleanup ()
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

સ્થિર શૂન્ય test_try_macro ()
{
  // Test success path
  result<પૂર્ણાંક> r_success = try_caller (ખોટું, ખોટું);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  result<પૂર્ણાંક> r_fail1 = try_caller (સાચું, ખોટું);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  result<પૂર્ણાંક> r_fail2 = try_caller (ખોટું, સાચું);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  result<શૂન્ય> v_ok = try_caller_void (ખોટું);
  hb_always_assert (v_ok.is_ok ());

  result<શૂન્ય> v_err = try_caller_void (સાચું);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  result<દશાંશ> f_ok = try_caller_type_change (ખોટું);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  result<દશાંશ> f_err = try_caller_type_change (સાચું);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  result<પૂર્ણાંક> a_ok = try_assign_existing (ખોટું);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  result<પૂર્ણાંક> a_err = try_assign_existing (સાચું);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

template <typename T, typename U, typename = શૂન્ય>
સંરચના is_equality_comparable : std::false_type {};

template <typename T, typename U>
સંરચના is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

સ્થિર શૂન્ય test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<result<પૂર્ણાંક>, પૂર્ણાંક>::value, "");
  static_assert (!is_equality_comparable<result<પૂર્ણાંક>, int64_t>::value, "");
  static_assert (!is_equality_comparable<પૂર્ણાંક, result<પૂર્ણાંક>>::value, "");
  static_assert (!is_equality_comparable<result<પૂર્ણાંક>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, result<પૂર્ણાંક>>::value, "");
  static_assert (!is_equality_comparable<result<શૂન્ય>, error_code_t>::value, "");
  static_assert (is_equality_comparable<result<પૂર્ણાંક>, result<પૂર્ણાંક>>::value, "");
  static_assert (is_equality_comparable<result<પૂર્ણાંક>, hb_result_ok_t<પૂર્ણાંક>>::value, "");
  static_assert (is_equality_comparable<result<પૂર્ણાંક>, hb_result_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, પૂર્ણાંક>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_ok_t<પૂર્ણાંક>>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_err_t<પૂર્ણાંક>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<પૂર્ણાંક, પૂર્ણાંક>, પૂર્ણાંક>::value, "");
  static_assert (std::is_constructible<hb_result_t<પૂર્ણાંક, પૂર્ણાંક>, hb_result_ok_t<પૂર્ણાંક>>::value, "");
  static_assert (std::is_constructible<hb_result_t<પૂર્ણાંક, પૂર્ણાંક>, hb_result_err_t<પૂર્ણાંક>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_result_t<પૂર્ણાંક, error_code_t>, પૂર્ણાંક>::value, "");
  static_assert (!std::is_constructible<hb_result_t<પૂર્ણાંક, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_result_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_result_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_result_t<પૂર્ણાંક, પૂર્ણાંક> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_result_t<પૂર્ણાંક, પૂર્ણાંક> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

સંરચના any_કિંમત_t
{
  template <typename X>
  any_કિંમત_t (X&&) {}
};

સ્થિર શૂન્ય test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_result_t<any_કિંમત_t, error_code_t>, hb_result_err_t<અચલ અક્ષર*>>::value, "");

  hb_result_t<any_કિંમત_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_result_t<any_કિંમત_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

સંરચના move_only_err_t
{
  પૂર્ણાંક code;

  explicit move_only_err_t (પૂર્ણાંક c) : code (c) {}
  move_only_err_t (અચલ move_only_err_t&) = delete;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& operator = (અચલ move_only_err_t&) = delete;
  move_only_err_t& operator = (move_only_err_t&& o) { code = o.code; o.code = -1; પાછા *આ; }
  ~move_only_err_t () = મૂળભૂત;

  તાર્કિક operator == (અચલ move_only_err_t& o) અચલ { પાછા code == o.code; }
};

સંરચના move_only_val_t
{
  પૂર્ણાંક val;

  explicit move_only_val_t (પૂર્ણાંક v) : val (v) {}
  move_only_val_t (અચલ move_only_val_t&) = delete;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& operator = (અચલ move_only_val_t&) = delete;
  move_only_val_t& operator = (move_only_val_t&& o) { val = o.val; o.val = -1; પાછા *આ; }
  ~move_only_val_t () = મૂળભૂત;

  તાર્કિક operator == (અચલ move_only_val_t& o) અચલ { પાછા val == o.val; }
};

સ્થિર શૂન્ય test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_result_t<પૂર્ણાંક, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_result_t<પૂર્ણાંક, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_result_t<શૂન્ય, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_result_t<શૂન્ય, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_result_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).કિંમત_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_result_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).કિંમત_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

સંરચના copy_move_counter_t
{
  સ્થિર પૂર્ણાંક copy_ગણતરી;
  સ્થિર પૂર્ણાંક move_ગણતરી;

  સ્થિર શૂન્ય reset ()
  {
    copy_ગણતરી = 0;
    move_ગણતરી = 0;
  }

  પૂર્ણાંક val;

  તાર્કિક in_error() અચલ {
    પાછા val == -1;
  }

  explicit copy_move_counter_t (પૂર્ણાંક v) : val (v) {}
  copy_move_counter_t (અચલ copy_move_counter_t& o) : val (o.val) { copy_ગણતરી++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_ગણતરી++; }
  ~copy_move_counter_t () = મૂળભૂત;
};

પૂર્ણાંક copy_move_counter_t::copy_ગણતરી = 0;
પૂર્ણાંક copy_move_counter_t::move_ગણતરી = 0;

સ્થિર result<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t out (42);
  પાછા Ok (std::move (out));
}

સ્થિર result<copy_move_counter_t> return_tracked_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  પાછા Ok(out);
}

સ્થિર hb_result_t<પૂર્ણાંક, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t out (42);
  પાછા Err (std::move (out));
}

સ્થિર hb_result_t<પૂર્ણાંક, copy_move_counter_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  પાછા Err(out);
}

સ્થિર result<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  પાછા Ok (std::move (out));
}

સ્થિર શૂન્ય test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ગણતરી == 0);
    hb_always_assert (copy_move_counter_t::move_ગણતરી >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ગણતરી == 0);
    hb_always_assert (copy_move_counter_t::move_ગણતરી >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<પૂર્ણાંક, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ગણતરી == 0);
    hb_always_assert (copy_move_counter_t::move_ગણતરી >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<પૂર્ણાંક, copy_move_counter_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ગણતરી == 0);
    hb_always_assert (copy_move_counter_t::move_ગણતરી >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    result<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

સંરચના in_error_t {
  તાર્કિક successful;

  in_error_t(તાર્કિક s) : successful(s) {}

  તાર્કિક in_error() અચલ {
    પાછા !successful;
  }
};

સ્થિર શૂન્ય test_from () {
  in_error_t has_error(ખોટું);
  in_error_t no_error(સાચું);
  result<શૂન્ય> r = result<શૂન્ય>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = result<શૂન્ય>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

સંરચના default_constructible_t
{
  પૂર્ણાંક val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (પૂર્ણાંક v) : val (v) {}
};

સંરચના move_only_default_t
{
  પૂર્ણાંક val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (પૂર્ણાંક v) : val (v) {}
  move_only_default_t (અચલ move_only_default_t&) = delete;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& operator = (અચલ move_only_default_t&) = delete;
  move_only_default_t& operator = (move_only_default_t&& o) { val = o.val; o.val = -1; પાછા *આ; }
  ~move_only_default_t () = મૂળભૂત;
};

સ્થિર શૂન્ય test_કિંમત_or_default ()
{
  // Test const &
  {
    અચલ result<પૂર્ણાંક> r_ok = Ok(42);
    hb_always_assert (r_ok.કિંમત_or_default () == 42);

    અચલ result<પૂર્ણાંક> r_err = Err(ERR_A);
    hb_always_assert (r_err.કિંમત_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    result<પૂર્ણાંક> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).કિંમત_or_default () == 42);

    result<પૂર્ણાંક> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).કિંમત_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    result<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.કિંમત_or_default ().val == 456);

    result<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.કિંમત_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).કિંમત_or_default ().val == 456);
    hb_always_assert (std::move (r_err).કિંમત_or_default ().val == 123);
  }

  // Test pointers
  {
    પૂર્ણાંક x = 10;
    result<પૂર્ણાંક*> r_ok = &x;
    hb_always_assert (r_ok.કિંમત_or_default () == &x);

    result<પૂર્ણાંક*> r_err = ERR_A;
    hb_always_assert (r_err.કિંમત_or_default () == nullptr);
  }

  // Test move-only types
  {
    result<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).કિંમત_or_default ().val == 88);

    result<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).કિંમત_or_default ().val == 77);
  }
}

સ્થિર શૂન્ય test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    result<copy_move_counter_t> r_ok = result<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_ગણતરી == 0);
    hb_always_assert (copy_move_counter_t::move_ગણતરી == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    result<copy_move_counter_t> r_fail = result<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_ગણતરી == 0);
    hb_always_assert (copy_move_counter_t::move_ગણતરી == 0);
  }
}

પૂર્ણાંક મુખ્ય ()
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
  test_કિંમત_or_default ();
  test_from_move ();

  પાછા 0;
}
