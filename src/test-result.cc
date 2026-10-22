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

#அந்தர்பாவ "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
நிலையான முழு live_instances = 0;

எண்ணிக்கை error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

template <typename T>
using result = hb_result_t<T, error_code_t>;

கட்டமைப்பு resource_t
{
  முழு id;

  explicit resource_t (முழு id_) : id (id_) { live_instances++; }
  resource_t (மாறிலி resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& operator = (மாறிலி resource_t& o)
  {
    id = o.id;
    திரும்பு *இது;
  }
  resource_t& operator = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    திரும்பு *இது;
  }

  தருக்க operator == (மாறிலி resource_t& o) மாறிலி { திரும்பு id == o.id; }
};

நிலையான result<முழு> returns_ok (முழு v)
{
  திரும்பு Ok(v);
}

நிலையான result<முழு> returns_err (error_code_t e)
{
  திரும்பு Err(e);
}

நிலையான result<முழு> try_callee (தருக்க fail, முழு v, error_code_t e = ERR_A)
{
  எனில் (fail) திரும்பு Err(e);
  திரும்பு Ok(v);
}

நிலையான result<வெறுமை> try_callee_void (தருக்க fail,  error_code_t e)
{
  எனில் (fail) திரும்பு e;
  திரும்பு Ok();
}

நிலையான result<முழு> try_caller (தருக்க fail1, தருக்க fail2)
{
  TRY_ASSIGN (முழு index, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (முழு index2, try_callee (fail2, 20, ERR_B));
  திரும்பு Ok(index + index2);
}

நிலையான result<வெறுமை> try_caller_void (தருக்க fail)
{
  TRY (try_callee_void (fail, ERR_C));
  திரும்பு Ok();
}

நிலையான result<தசம> try_caller_type_change (தருக்க fail)
{
  TRY_ASSIGN (முழு index, try_callee (fail, 100, ERR_A));
  திரும்பு Ok((தசம) index * 1.5f);
}

நிலையான result<முழு> try_assign_existing (தருக்க fail)
{
  முழு val = 1;
  TRY_ASSIGN (val, try_callee (fail, 42, ERR_A));
  திரும்பு Ok(val);
}

நிலையான வெறுமை test_ok_basic ()
{
  result<முழு> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((தருக்க) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.மதிப்பு_or (0) == 42);
  hb_always_assert (r.மதிப்பு_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

நிலையான வெறுமை test_err_basic ()
{
  result<முழு> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.மதிப்பு_or (99) == 99);
  hb_always_assert (r.மதிப்பு_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

நிலையான வெறுமை test_explicit_ok_err ()
{
  result<unsigned> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  result<unsigned> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

நிலையான வெறுமை test_pointers ()
{
  முழு x = 123;
  result<முழு*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

நிலையான வெறுமை test_void ()
{
  result<வெறுமை> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((தருக்க) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  result<வெறுமை> r2 = Err(ERR_A);
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

நிலையான வெறுமை test_resource_cleanup ()
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

நிலையான வெறுமை test_try_macro ()
{
  // Test success path
  result<முழு> r_success = try_caller (பொய், பொய்);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  result<முழு> r_fail1 = try_caller (உண்மை, பொய்);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  result<முழு> r_fail2 = try_caller (பொய், உண்மை);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  result<வெறுமை> v_ok = try_caller_void (பொய்);
  hb_always_assert (v_ok.is_ok ());

  result<வெறுமை> v_err = try_caller_void (உண்மை);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  result<தசம> f_ok = try_caller_type_change (பொய்);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  result<தசம> f_err = try_caller_type_change (உண்மை);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  result<முழு> a_ok = try_assign_existing (பொய்);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  result<முழு> a_err = try_assign_existing (உண்மை);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

template <typename T, typename U, typename = வெறுமை>
கட்டமைப்பு is_equality_comparable : std::false_type {};

template <typename T, typename U>
கட்டமைப்பு is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

நிலையான வெறுமை test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<result<முழு>, முழு>::value, "");
  static_assert (!is_equality_comparable<result<முழு>, int64_t>::value, "");
  static_assert (!is_equality_comparable<முழு, result<முழு>>::value, "");
  static_assert (!is_equality_comparable<result<முழு>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, result<முழு>>::value, "");
  static_assert (!is_equality_comparable<result<வெறுமை>, error_code_t>::value, "");
  static_assert (is_equality_comparable<result<முழு>, result<முழு>>::value, "");
  static_assert (is_equality_comparable<result<முழு>, hb_result_ok_t<முழு>>::value, "");
  static_assert (is_equality_comparable<result<முழு>, hb_result_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, முழு>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_ok_t<முழு>>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_err_t<முழு>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<முழு, முழு>, முழு>::value, "");
  static_assert (std::is_constructible<hb_result_t<முழு, முழு>, hb_result_ok_t<முழு>>::value, "");
  static_assert (std::is_constructible<hb_result_t<முழு, முழு>, hb_result_err_t<முழு>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_result_t<முழு, error_code_t>, முழு>::value, "");
  static_assert (!std::is_constructible<hb_result_t<முழு, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_result_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_result_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_result_t<முழு, முழு> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_result_t<முழு, முழு> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

கட்டமைப்பு any_மதிப்பு_t
{
  template <typename X>
  any_மதிப்பு_t (X&&) {}
};

நிலையான வெறுமை test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_result_t<any_மதிப்பு_t, error_code_t>, hb_result_err_t<மாறிலி எழுத்து*>>::value, "");

  hb_result_t<any_மதிப்பு_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_result_t<any_மதிப்பு_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

கட்டமைப்பு move_only_err_t
{
  முழு code;

  explicit move_only_err_t (முழு c) : code (c) {}
  move_only_err_t (மாறிலி move_only_err_t&) = delete;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& operator = (மாறிலி move_only_err_t&) = delete;
  move_only_err_t& operator = (move_only_err_t&& o) { code = o.code; o.code = -1; திரும்பு *இது; }
  ~move_only_err_t () = இயல்பு;

  தருக்க operator == (மாறிலி move_only_err_t& o) மாறிலி { திரும்பு code == o.code; }
};

கட்டமைப்பு move_only_val_t
{
  முழு val;

  explicit move_only_val_t (முழு v) : val (v) {}
  move_only_val_t (மாறிலி move_only_val_t&) = delete;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& operator = (மாறிலி move_only_val_t&) = delete;
  move_only_val_t& operator = (move_only_val_t&& o) { val = o.val; o.val = -1; திரும்பு *இது; }
  ~move_only_val_t () = இயல்பு;

  தருக்க operator == (மாறிலி move_only_val_t& o) மாறிலி { திரும்பு val == o.val; }
};

நிலையான வெறுமை test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_result_t<முழு, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_result_t<முழு, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_result_t<வெறுமை, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_result_t<வெறுமை, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_result_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).மதிப்பு_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_result_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).மதிப்பு_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

கட்டமைப்பு copy_move_counter_t
{
  நிலையான முழு copy_மொத்தம்;
  நிலையான முழு move_மொத்தம்;

  நிலையான வெறுமை reset ()
  {
    copy_மொத்தம் = 0;
    move_மொத்தம் = 0;
  }

  முழு val;

  தருக்க in_error() மாறிலி {
    திரும்பு val == -1;
  }

  explicit copy_move_counter_t (முழு v) : val (v) {}
  copy_move_counter_t (மாறிலி copy_move_counter_t& o) : val (o.val) { copy_மொத்தம்++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_மொத்தம்++; }
  ~copy_move_counter_t () = இயல்பு;
};

முழு copy_move_counter_t::copy_மொத்தம் = 0;
முழு copy_move_counter_t::move_மொத்தம் = 0;

நிலையான result<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t out (42);
  திரும்பு Ok (std::move (out));
}

நிலையான result<copy_move_counter_t> return_tracked_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  திரும்பு Ok(out);
}

நிலையான hb_result_t<முழு, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t out (42);
  திரும்பு Err (std::move (out));
}

நிலையான hb_result_t<முழு, copy_move_counter_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  திரும்பு Err(out);
}

நிலையான result<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  திரும்பு Ok (std::move (out));
}

நிலையான வெறுமை test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_மொத்தம் == 0);
    hb_always_assert (copy_move_counter_t::move_மொத்தம் >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_மொத்தம் == 0);
    hb_always_assert (copy_move_counter_t::move_மொத்தம் >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<முழு, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_மொத்தம் == 0);
    hb_always_assert (copy_move_counter_t::move_மொத்தம் >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<முழு, copy_move_counter_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_மொத்தம் == 0);
    hb_always_assert (copy_move_counter_t::move_மொத்தம் >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    result<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

கட்டமைப்பு in_error_t {
  தருக்க successful;

  in_error_t(தருக்க s) : successful(s) {}

  தருக்க in_error() மாறிலி {
    திரும்பு !successful;
  }
};

நிலையான வெறுமை test_from () {
  in_error_t has_error(பொய்);
  in_error_t no_error(உண்மை);
  result<வெறுமை> r = result<வெறுமை>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = result<வெறுமை>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

கட்டமைப்பு default_constructible_t
{
  முழு val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (முழு v) : val (v) {}
};

கட்டமைப்பு move_only_default_t
{
  முழு val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (முழு v) : val (v) {}
  move_only_default_t (மாறிலி move_only_default_t&) = delete;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& operator = (மாறிலி move_only_default_t&) = delete;
  move_only_default_t& operator = (move_only_default_t&& o) { val = o.val; o.val = -1; திரும்பு *இது; }
  ~move_only_default_t () = இயல்பு;
};

நிலையான வெறுமை test_மதிப்பு_or_default ()
{
  // Test const &
  {
    மாறிலி result<முழு> r_ok = Ok(42);
    hb_always_assert (r_ok.மதிப்பு_or_default () == 42);

    மாறிலி result<முழு> r_err = Err(ERR_A);
    hb_always_assert (r_err.மதிப்பு_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    result<முழு> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).மதிப்பு_or_default () == 42);

    result<முழு> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).மதிப்பு_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    result<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.மதிப்பு_or_default ().val == 456);

    result<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.மதிப்பு_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).மதிப்பு_or_default ().val == 456);
    hb_always_assert (std::move (r_err).மதிப்பு_or_default ().val == 123);
  }

  // Test pointers
  {
    முழு x = 10;
    result<முழு*> r_ok = &x;
    hb_always_assert (r_ok.மதிப்பு_or_default () == &x);

    result<முழு*> r_err = ERR_A;
    hb_always_assert (r_err.மதிப்பு_or_default () == nullptr);
  }

  // Test move-only types
  {
    result<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).மதிப்பு_or_default ().val == 88);

    result<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).மதிப்பு_or_default ().val == 77);
  }
}

நிலையான வெறுமை test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    result<copy_move_counter_t> r_ok = result<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_மொத்தம் == 0);
    hb_always_assert (copy_move_counter_t::move_மொத்தம் == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    result<copy_move_counter_t> r_fail = result<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_மொத்தம் == 0);
    hb_always_assert (copy_move_counter_t::move_மொத்தம் == 0);
  }
}

முழு முதன்மை ()
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
  test_மதிப்பு_or_default ();
  test_from_move ();

  திரும்பு 0;
}
