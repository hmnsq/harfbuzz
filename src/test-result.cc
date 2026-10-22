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

#समावेश "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
अटल पूर्णांक live_instances = 0;

क्रमागत error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

ढाँचा <typename T>
using परिणाम = hb_परिणाम_t<T, error_code_t>;

काष्ठा resource_t
{
  पूर्णांक id;

  explicit resource_t (पूर्णांक id_) : id (id_) { live_instances++; }
  resource_t (स्थिर resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& चालक = (स्थिर resource_t& o)
  {
    id = o.id;
    वापस *यह;
  }
  resource_t& चालक = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    वापस *यह;
  }

  bool चालक == (स्थिर resource_t& o) स्थिर { वापस id == o.id; }
};

अटल परिणाम<पूर्णांक> returns_ok (पूर्णांक v)
{
  वापस Ok(v);
}

अटल परिणाम<पूर्णांक> returns_err (error_code_t e)
{
  वापस Err(e);
}

अटल परिणाम<पूर्णांक> try_callee (bool हार, पूर्णांक v, error_code_t e = ERR_A)
{
  अगर (हार) वापस Err(e);
  वापस Ok(v);
}

अटल परिणाम<व्योम> try_callee_void (bool हार,  error_code_t e)
{
  अगर (हार) वापस e;
  वापस Ok();
}

अटल परिणाम<पूर्णांक> try_caller (bool fail1, bool fail2)
{
  TRY_ASSIGN (पूर्णांक सूचकांक_2, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (पूर्णांक index2, try_callee (fail2, 20, ERR_B));
  वापस Ok(सूचकांक_2 + index2);
}

अटल परिणाम<व्योम> try_caller_void (bool हार)
{
  TRY (try_callee_void (हार, ERR_C));
  वापस Ok();
}

अटल परिणाम<भग्न> try_caller_type_change (bool हार)
{
  TRY_ASSIGN (पूर्णांक सूचकांक_2, try_callee (हार, 100, ERR_A));
  वापस Ok((भग्न) सूचकांक_2 * 1.5f);
}

अटल परिणाम<पूर्णांक> try_assign_existing (bool हार)
{
  पूर्णांक val = 1;
  TRY_ASSIGN (val, try_callee (हार, 42, ERR_A));
  वापस Ok(val);
}

अटल व्योम test_ok_basic ()
{
  परिणाम<पूर्णांक> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((bool) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.मूल्य_or (0) == 42);
  hb_always_assert (r.मूल्य_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

अटल व्योम test_err_basic ()
{
  परिणाम<पूर्णांक> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.मूल्य_or (99) == 99);
  hb_always_assert (r.मूल्य_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

अटल व्योम test_explicit_ok_err ()
{
  परिणाम<अचिन्हित> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  परिणाम<अचिन्हित> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

अटल व्योम test_pointers ()
{
  पूर्णांक x = 123;
  परिणाम<पूर्णांक*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

अटल व्योम test_void ()
{
  परिणाम<व्योम> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((bool) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  परिणाम<व्योम> r2 = Err(ERR_A);
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

अटल व्योम test_resource_cleanup ()
{
  hb_always_assert (live_instances == 0);

  {
    परिणाम<resource_t> r = resource_t (1);
    hb_always_assert (live_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (live_instances == 0);

  {
    परिणाम<resource_t> r1 = resource_t (2);
    hb_always_assert (live_instances == 1);
    परिणाम<resource_t> r2 = r1;
    hb_always_assert (live_instances == 2);
    परिणाम<resource_t> r3 = std::move (r1);
    hb_always_assert (live_instances == 3);
  }
  hb_always_assert (live_instances == 0);

  {
    परिणाम<resource_t> r = resource_t (3);
    hb_always_assert (live_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (live_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (live_instances == 0);
}

अटल व्योम test_try_macro ()
{
  // Test success path
  परिणाम<पूर्णांक> r_success = try_caller (false, false);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  परिणाम<पूर्णांक> r_fail1 = try_caller (true, false);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  परिणाम<पूर्णांक> r_fail2 = try_caller (false, true);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  परिणाम<व्योम> v_ok = try_caller_void (false);
  hb_always_assert (v_ok.is_ok ());

  परिणाम<व्योम> v_err = try_caller_void (true);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  परिणाम<भग्न> f_ok = try_caller_type_change (false);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  परिणाम<भग्न> f_err = try_caller_type_change (true);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  परिणाम<पूर्णांक> a_ok = try_assign_existing (false);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  परिणाम<पूर्णांक> a_err = try_assign_existing (true);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

ढाँचा <typename T, typename U, typename = व्योम>
काष्ठा is_equality_comparable : std::false_type {};

ढाँचा <typename T, typename U>
काष्ठा is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

अटल व्योम test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<परिणाम<पूर्णांक>, पूर्णांक>::value, "");
  static_assert (!is_equality_comparable<परिणाम<पूर्णांक>, int64_t>::value, "");
  static_assert (!is_equality_comparable<पूर्णांक, परिणाम<पूर्णांक>>::value, "");
  static_assert (!is_equality_comparable<परिणाम<पूर्णांक>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, परिणाम<पूर्णांक>>::value, "");
  static_assert (!is_equality_comparable<परिणाम<व्योम>, error_code_t>::value, "");
  static_assert (is_equality_comparable<परिणाम<पूर्णांक>, परिणाम<पूर्णांक>>::value, "");
  static_assert (is_equality_comparable<परिणाम<पूर्णांक>, hb_परिणाम_ok_t<पूर्णांक>>::value, "");
  static_assert (is_equality_comparable<परिणाम<पूर्णांक>, hb_परिणाम_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_परिणाम_t<int64_t, int32_t>, पूर्णांक>::value, "");
  static_assert (!std::is_constructible<hb_परिणाम_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_परिणाम_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_परिणाम_t<int64_t, int32_t>, hb_परिणाम_ok_t<पूर्णांक>>::value, "");
  static_assert (std::is_constructible<hb_परिणाम_t<int64_t, int32_t>, hb_परिणाम_err_t<पूर्णांक>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_परिणाम_t<पूर्णांक, पूर्णांक>, पूर्णांक>::value, "");
  static_assert (std::is_constructible<hb_परिणाम_t<पूर्णांक, पूर्णांक>, hb_परिणाम_ok_t<पूर्णांक>>::value, "");
  static_assert (std::is_constructible<hb_परिणाम_t<पूर्णांक, पूर्णांक>, hb_परिणाम_err_t<पूर्णांक>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_परिणाम_t<पूर्णांक, error_code_t>, पूर्णांक>::value, "");
  static_assert (!std::is_constructible<hb_परिणाम_t<पूर्णांक, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_परिणाम_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_परिणाम_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_परिणाम_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_परिणाम_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_परिणाम_t<पूर्णांक, पूर्णांक> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_परिणाम_t<पूर्णांक, पूर्णांक> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

काष्ठा any_मूल्य_t
{
  ढाँचा <typename X>
  any_मूल्य_t (X&&) {}
};

अटल व्योम test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_परिणाम_t<any_मूल्य_t, error_code_t>, hb_परिणाम_err_t<स्थिर अक्षर*>>::value, "");

  hb_परिणाम_t<any_मूल्य_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_परिणाम_t<any_मूल्य_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

काष्ठा move_only_err_t
{
  पूर्णांक code;

  explicit move_only_err_t (पूर्णांक c) : code (c) {}
  move_only_err_t (स्थिर move_only_err_t&) = मिटाओ;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& चालक = (स्थिर move_only_err_t&) = मिटाओ;
  move_only_err_t& चालक = (move_only_err_t&& o) { code = o.code; o.code = -1; वापस *यह; }
  ~move_only_err_t () = शेष;

  bool चालक == (स्थिर move_only_err_t& o) स्थिर { वापस code == o.code; }
};

काष्ठा move_only_val_t
{
  पूर्णांक val;

  explicit move_only_val_t (पूर्णांक v) : val (v) {}
  move_only_val_t (स्थिर move_only_val_t&) = मिटाओ;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& चालक = (स्थिर move_only_val_t&) = मिटाओ;
  move_only_val_t& चालक = (move_only_val_t&& o) { val = o.val; o.val = -1; वापस *यह; }
  ~move_only_val_t () = शेष;

  bool चालक == (स्थिर move_only_val_t& o) स्थिर { वापस val == o.val; }
};

अटल व्योम test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_परिणाम_t<पूर्णांक, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_परिणाम_t<पूर्णांक, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_परिणाम_t<व्योम, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_परिणाम_t<व्योम, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_परिणाम_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).मूल्य_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_परिणाम_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).मूल्य_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

काष्ठा copy_move_गणक_t
{
  अटल पूर्णांक copy_गिनती;
  अटल पूर्णांक move_गिनती;

  अटल व्योम reset ()
  {
    copy_गिनती = 0;
    move_गिनती = 0;
  }

  पूर्णांक val;

  bool in_error() स्थिर {
    वापस val == -1;
  }

  explicit copy_move_गणक_t (पूर्णांक v) : val (v) {}
  copy_move_गणक_t (स्थिर copy_move_गणक_t& o) : val (o.val) { copy_गिनती++; }
  copy_move_गणक_t (copy_move_गणक_t&& o) : val (o.val) { o.val = -1; move_गिनती++; }
  ~copy_move_गणक_t () = शेष;
};

पूर्णांक copy_move_गणक_t::copy_गिनती = 0;
पूर्णांक copy_move_गणक_t::move_गिनती = 0;

अटल परिणाम<copy_move_गणक_t> return_tracked_from_local ()
{
  copy_move_गणक_t out (42);
  वापस Ok (std::move (out));
}

अटल परिणाम<copy_move_गणक_t> return_tracked_from_local_with_tag ()
{
  copy_move_गणक_t out (42);
  वापस Ok(out);
}

अटल hb_परिणाम_t<पूर्णांक, copy_move_गणक_t> return_tracked_error_from_local ()
{
  copy_move_गणक_t out (42);
  वापस Err (std::move (out));
}

अटल hb_परिणाम_t<पूर्णांक, copy_move_गणक_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_गणक_t out (42);
  वापस Err(out);
}

अटल परिणाम<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  वापस Ok (std::move (out));
}

अटल व्योम test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_गणक_t::reset ();
    परिणाम<copy_move_गणक_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_गणक_t::copy_गिनती == 0);
    hb_always_assert (copy_move_गणक_t::move_गिनती >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_गणक_t::reset ();
    परिणाम<copy_move_गणक_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_गणक_t::copy_गिनती == 0);
    hb_always_assert (copy_move_गणक_t::move_गिनती >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_गणक_t::reset ();
    hb_परिणाम_t<पूर्णांक, copy_move_गणक_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_गणक_t::copy_गिनती == 0);
    hb_always_assert (copy_move_गणक_t::move_गिनती >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_गणक_t::reset ();
    hb_परिणाम_t<पूर्णांक, copy_move_गणक_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_गणक_t::copy_गिनती == 0);
    hb_always_assert (copy_move_गणक_t::move_गिनती >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    परिणाम<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

काष्ठा in_error_t {
  bool successful;

  in_error_t(bool s) : successful(s) {}

  bool in_error() स्थिर {
    वापस !successful;
  }
};

अटल व्योम test_from () {
  in_error_t has_error(false);
  in_error_t no_error(true);
  परिणाम<व्योम> r = परिणाम<व्योम>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = परिणाम<व्योम>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

काष्ठा default_constructible_t
{
  पूर्णांक val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (पूर्णांक v) : val (v) {}
};

काष्ठा move_only_default_t
{
  पूर्णांक val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (पूर्णांक v) : val (v) {}
  move_only_default_t (स्थिर move_only_default_t&) = मिटाओ;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& चालक = (स्थिर move_only_default_t&) = मिटाओ;
  move_only_default_t& चालक = (move_only_default_t&& o) { val = o.val; o.val = -1; वापस *यह; }
  ~move_only_default_t () = शेष;
};

अटल व्योम test_मूल्य_or_default ()
{
  // Test const &
  {
    स्थिर परिणाम<पूर्णांक> r_ok = Ok(42);
    hb_always_assert (r_ok.मूल्य_or_default () == 42);

    स्थिर परिणाम<पूर्णांक> r_err = Err(ERR_A);
    hb_always_assert (r_err.मूल्य_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    परिणाम<पूर्णांक> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).मूल्य_or_default () == 42);

    परिणाम<पूर्णांक> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).मूल्य_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    परिणाम<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.मूल्य_or_default ().val == 456);

    परिणाम<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.मूल्य_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).मूल्य_or_default ().val == 456);
    hb_always_assert (std::move (r_err).मूल्य_or_default ().val == 123);
  }

  // Test pointers
  {
    पूर्णांक x = 10;
    परिणाम<पूर्णांक*> r_ok = &x;
    hb_always_assert (r_ok.मूल्य_or_default () == &x);

    परिणाम<पूर्णांक*> r_err = ERR_A;
    hb_always_assert (r_err.मूल्य_or_default () == nullptr);
  }

  // Test move-only types
  {
    परिणाम<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).मूल्य_or_default ().val == 88);

    परिणाम<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).मूल्य_or_default ().val == 77);
  }
}

अटल व्योम test_from_move()
{
  {
    copy_move_गणक_t::reset();
    copy_move_गणक_t ok (24);
    परिणाम<copy_move_गणक_t> r_ok = परिणाम<copy_move_गणक_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_गणक_t::copy_गिनती == 0);
    hb_always_assert (copy_move_गणक_t::move_गिनती == 1);
  }

  {
    copy_move_गणक_t::reset();
    copy_move_गणक_t हार (-1);
    परिणाम<copy_move_गणक_t> r_fail = परिणाम<copy_move_गणक_t>::from(std::move(हार), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_गणक_t::copy_गिनती == 0);
    hb_always_assert (copy_move_गणक_t::move_गिनती == 0);
  }
}

पूर्णांक मुख्य ()
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
  test_मूल्य_or_default ();
  test_from_move ();

  वापस 0;
}
