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

#جيڪڏهن_وصف_نه_ٿيل HB_نتيجو_HH
#وصف_ڪريو HB_نتيجو_HH

#ضمن "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
سانچو <قسم_نالو T>
بنية hb_نتيجو_ok_t
{
  T value;
};

سانچو <>
بنية hb_نتيجو_ok_t<فراغ> {};

سانچو <قسم_نالو E>
بنية hb_نتيجو_err_t
{
  E error;

  سانچو <قسم_نالو OtherE>
  constexpr منطقي عامل == (ثابت hb_نتيجو_err_t<OtherE>& other) ثابت { أرجع error == other.error; }
  سانچو <قسم_نالو OtherE>
  constexpr منطقي عامل != (ثابت hb_نتيجو_err_t<OtherE>& other) ثابت { أرجع error != other.error; }
};

سانچو <قسم_نالو X> بنية hb_is_نتيجو_ٽيگ : hb_false_type {};
سانچو <قسم_نالو X> بنية hb_is_نتيجو_ٽيگ<hb_نتيجو_ok_t<X>> : hb_true_type {};
سانچو <قسم_نالو X> بنية hb_is_نتيجو_ٽيگ<hb_نتيجو_err_t<X>> : hb_true_type {};

سانچو <قسم_نالو T>
ساكن مضمن constexpr hb_نتيجو_ok_t<T&&> Ok (T&& v)
{
  أرجع hb_نتيجو_ok_t<T&&>{std::forward<T> (v)};
}

ساكن مضمن constexpr hb_نتيجو_ok_t<فراغ> Ok ()
{
  أرجع hb_نتيجو_ok_t<فراغ>{};
}

سانچو <قسم_نالو E>
ساكن مضمن constexpr hb_نتيجو_err_t<E&&> Err (E&& e)
{
  أرجع hb_نتيجو_err_t<E&&>{std::forward<E> (e)};
}

سانچو <قسم_نالو T, قسم_نالو E>
بنية HB_NODISCARD_STRUCT hb_نتيجو_t
{
  خاص:
  اتحاد
  {
    T val_;
    E err_;
  };
  منطقي is_ok_;

  فراغ destroy ()
  {
    إذا (is_ok_)
      val_.~T ();
    وإلا
      err_.~E ();
  }

  فراغ init(ثابت hb_نتيجو_t& o) {
    is_ok_ = o.is_ok_;
    إذا (is_ok_)
      جديد (std::addressof (val_)) T (o.val_);
    وإلا
      جديد (std::addressof (err_)) E (o.err_);
  }

  فراغ init(hb_نتيجو_t&& o) {
    is_ok_ = o.is_ok_;
    إذا (is_ok_)
      جديد (std::addressof (val_)) T (std::move (o.val_));
    وإلا
      جديد (std::addressof (err_)) E (std::move (o.err_));
  }

  عام:
  ~hb_نتيجو_t ()
  {
    destroy ();
  }

  hb_نتيجو_t () = ميٽيو;

  hb_نتيجو_t (ثابت hb_نتيجو_t& o) : is_ok_ (خطأ)
  {
    init (o);
  }

  hb_نتيجو_t (hb_نتيجو_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (خطأ)
  {
    init (std::move (o));
  }

  hb_نتيجو_t& عامل = (ثابت hb_نتيجو_t& o)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    وإلا إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    وإلا
    {
      destroy ();
      init (o);
    }
    أرجع *هذا;
  }

  hb_نتيجو_t& عامل = (hb_نتيجو_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    وإلا إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    وإلا
    {
      destroy ();
      init (std::move (o));
    }
    أرجع *هذا;
  }

  // Construct from Ok(...) tag
  سانچو <قسم_نالو U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_نتيجو_t (hb_نتيجو_ok_t<U>&& o) : is_ok_ (سچ)
  {
    جديد (std::addressof (val_)) T (std::move (o.value));
  }

  سانچو <قسم_نالو U,
            hb_enable_if ((std::is_constructible<T, ثابت U&>::value))>
  hb_نتيجو_t (ثابت hb_نتيجو_ok_t<U>& o) : is_ok_ (سچ)
  {
    جديد (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  سانچو <قسم_نالو F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_نتيجو_t (hb_نتيجو_err_t<F>&& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move (e.error));
  }

  سانچو <قسم_نالو F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_نتيجو_t (ثابت hb_نتيجو_err_t<F>& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  سانچو <قسم_نالو F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_نتيجو_t (F e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  سانچو <قسم_نالو U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_نتيجو_t) &&
                           !hb_is_نتيجو_ٽيگ<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_نتيجو_t (U&& v) : is_ok_ (سچ)
  {
    جديد (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  ساكن hb_نتيجو_t from (T&& fallible, E error_قدر) {
    إذا (fallible.اندريون_error()) {
      أرجع Err(error_قدر);
    }
    أرجع Ok(fallible);
  }

  منطقي is_ok () ثابت { أرجع is_ok_; }
  منطقي is_err () ثابت { أرجع !is_ok_; }
  واضح عامل منطقي () ثابت { أرجع is_ok (); }

  T& value () & { أكد (is_ok_); أرجع val_; }
  ثابت T& value () ثابت & { أكد (is_ok_); أرجع val_; }
  T value () && { أكد (is_ok_); أرجع std::move (val_); }

  T& عامل * () & { أرجع value (); }
  ثابت T& عامل * () ثابت & { أرجع value (); }
  T عامل * () && { أرجع std::move (*هذا).value (); }

  T* عامل -> () { أرجع std::addressof (value ()); }
  ثابت T* عامل -> () ثابت { أرجع std::addressof (value ()); }

  سانچو <قسم_نالو U>
  T قدر_or (U&& default_قدر) ثابت &
  {
    إذا (is_ok_) أرجع val_;
    أرجع std::forward<U> (default_قدر);
  }

  سانچو <قسم_نالو U>
  T قدر_or (U&& default_قدر) &&
  {
    إذا (is_ok_) أرجع std::move (val_);
    أرجع std::forward<U> (default_قدر);
  }

  T قدر_or_default () ثابت &
  {
    إذا (is_ok_) أرجع val_;
    أرجع T ();
  }

  T قدر_or_default () &&
  {
    إذا (is_ok_) أرجع std::move (val_);
    أرجع T ();
  }

  ثابت E& error () ثابت & { أكد (!is_ok_); أرجع err_; }
  E& error () & { أكد (!is_ok_); أرجع err_; }
  E error () && { أكد (!is_ok_); أرجع std::move (err_); }

  منطقي عامل == (ثابت hb_نتيجو_t& o) ثابت
  {
    إذا (is_ok_ != o.is_ok_) أرجع خطأ;
    إذا (is_ok_) أرجع val_ == o.val_;
    أرجع err_ == o.err_;
  }

  منطقي عامل != (ثابت hb_نتيجو_t& o) ثابت
  {
    أرجع !(*هذا == o);
  }

  سانچو <قسم_نالو U>
  منطقي عامل == (ثابت hb_نتيجو_ok_t<U>& o) ثابت
  {
    أرجع is_ok_ && val_ == o.value;
  }

  سانچو <قسم_نالو U>
  منطقي عامل != (ثابت hb_نتيجو_ok_t<U>& o) ثابت
  {
    أرجع !(*هذا == o);
  }

  سانچو <قسم_نالو F>
  منطقي عامل == (ثابت hb_نتيجو_err_t<F>& e) ثابت
  {
    أرجع !is_ok_ && err_ == e.error;
  }

  سانچو <قسم_نالو F>
  منطقي عامل != (ثابت hb_نتيجو_err_t<F>& e) ثابت
  {
    أرجع !(*هذا == e);
  }

  سانچو <قسم_نالو U>
  منطقي عامل == (ثابت U&) ثابت = ميٽيو;

  سانچو <قسم_نالو U>
  منطقي عامل != (ثابت U&) ثابت = ميٽيو;
};

// Partial specialization for void
سانچو <قسم_نالو E>
بنية HB_NODISCARD_STRUCT hb_نتيجو_t<فراغ, E>
{
  خاص:
  اتحاد
  {
    E err_;
  };
  منطقي is_ok_;

  سانچو <قسم_نالو, قسم_نالو> دوست بنية hb_نتيجو_t;

  عام:
  ~hb_نتيجو_t ()
  {
    إذا (!is_ok_)
      err_.~E ();
  }

  hb_نتيجو_t () : is_ok_ (سچ) {}
  hb_نتيجو_t (hb_نتيجو_ok_t<فراغ>) : is_ok_ (سچ) {}

  hb_نتيجو_t (ثابت hb_نتيجو_t& o) : is_ok_ (o.is_ok_)
  {
    إذا (!is_ok_)
      جديد (std::addressof (err_)) E (o.err_);
  }

  hb_نتيجو_t (hb_نتيجو_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    إذا (!is_ok_)
      جديد (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_نتيجو_t& عامل = (ثابت hb_نتيجو_t& o)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    وإلا إذا (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = سچ;
    }
    وإلا إذا (is_ok_ && !o.is_ok_)
    {
      جديد (std::addressof (err_)) E (o.err_);
      is_ok_ = خطأ;
    }
    أرجع *هذا;
  }

  hb_نتيجو_t& عامل = (hb_نتيجو_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    وإلا إذا (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = سچ;
    }
    وإلا إذا (is_ok_ && !o.is_ok_)
    {
      جديد (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = خطأ;
    }
    أرجع *هذا;
  }

  سانچو <قسم_نالو F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_نتيجو_t (hb_نتيجو_err_t<F>&& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move (e.error));
  }

  سانچو <قسم_نالو F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_نتيجو_t (ثابت hb_نتيجو_err_t<F>& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_نتيجو_t (E e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  سانچو<قسم_نالو V>
  ساكن hb_نتيجو_t<فراغ, E> from (ثابت V& fallible, E error_قدر) {
    إذا (fallible.اندريون_error()) {
      أرجع Err(error_قدر);
    }
    أرجع Ok();
  }

  منطقي is_ok () ثابت { أرجع is_ok_; }
  منطقي is_err () ثابت { أرجع !is_ok_; }
  واضح عامل منطقي () ثابت { أرجع is_ok (); }

  ثابت E& error () ثابت & { أكد (!is_ok_); أرجع err_; }
  E& error () & { أكد (!is_ok_); أرجع err_; }
  E error () && { أكد (!is_ok_); أرجع std::move (err_); }

  فراغ عامل * () & { أرجع; }
  فراغ عامل * () ثابت & { أرجع; }
  فراغ عامل * () && { أرجع; }

  منطقي عامل == (ثابت hb_نتيجو_t<فراغ, E>& o) ثابت
  {
    أرجع is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  منطقي عامل != (ثابت hb_نتيجو_t<فراغ, E>& o) ثابت
  {
    أرجع !(*هذا == o);
  }

  منطقي عامل == (hb_نتيجو_ok_t<فراغ>) ثابت
  {
    أرجع is_ok_;
  }

  منطقي عامل != (hb_نتيجو_ok_t<فراغ>) ثابت
  {
    أرجع !is_ok_;
  }

  سانچو <قسم_نالو F>
  منطقي عامل == (ثابت hb_نتيجو_err_t<F>& e) ثابت
  {
    أرجع !is_ok_ && err_ == e.error;
  }

  سانچو <قسم_نالو F>
  منطقي عامل != (ثابت hb_نتيجو_err_t<F>& e) ثابت
  {
    أرجع !(*هذا == e);
  }

  سانچو <قسم_نالو U>
  منطقي عامل == (ثابت U&) ثابت = ميٽيو;

  سانچو <قسم_نالو U>
  منطقي عامل != (ثابت U&) ثابت = ميٽيو;
};

سانچو <قسم_نالو U, قسم_نالو T, قسم_نالو E>
ساكن مضمن منطقي عامل == (ثابت hb_نتيجو_ok_t<U>& o, ثابت hb_نتيجو_t<T, E>& r)
{
  أرجع r == o;
}
سانچو <قسم_نالو U, قسم_نالو T, قسم_نالو E>
ساكن مضمن منطقي عامل != (ثابت hb_نتيجو_ok_t<U>& o, ثابت hb_نتيجو_t<T, E>& r)
{
  أرجع r != o;
}
سانچو <قسم_نالو F, قسم_نالو T, قسم_نالو E>
ساكن مضمن منطقي عامل == (ثابت hb_نتيجو_err_t<F>& e, ثابت hb_نتيجو_t<T, E>& r)
{
  أرجع r == e;
}
سانچو <قسم_نالو F, قسم_نالو T, قسم_نالو E>
ساكن مضمن منطقي عامل != (ثابت hb_نتيجو_err_t<F>& e, ثابت hb_نتيجو_t<T, E>& r)
{
  أرجع r != e;
}

#جيڪڏهن_وصف_نه_ٿيل TRY
#وصف_ڪريو TRY(اظهار)                                                             \
  افعل {                                                                        \
    خودڪار _res = (اظهار);                                                       \
    إذا (unlikely (!_res.is_ok())) أرجع Err(std::move(_res).error());        \
  } طالما (0)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل TRY_سونپيو
#وصف_ڪريو TRY_سونپيو(var, ...)                                                  \
  خودڪار HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  إذا (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) أرجع Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* HB_RESULT_HH */
