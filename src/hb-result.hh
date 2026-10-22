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

#که_نه_تعریف_شوی HB_پایله_HH
#تعریف_کړه HB_پایله_HH

#ضمن "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
کينډۍ <ډول_نوم T>
بنية hb_پایله_ok_t
{
  T value;
};

کينډۍ <>
بنية hb_پایله_ok_t<فراغ> {};

کينډۍ <ډول_نوم E>
بنية hb_پایله_err_t
{
  E error;

  کينډۍ <ډول_نوم OtherE>
  constexpr منطقي چلوونکی == (ثابت hb_پایله_err_t<OtherE>& other) ثابت { أرجع error == other.error; }
  کينډۍ <ډول_نوم OtherE>
  constexpr منطقي چلوونکی != (ثابت hb_پایله_err_t<OtherE>& other) ثابت { أرجع error != other.error; }
};

کينډۍ <ډول_نوم X> بنية hb_is_پایله_نښان : hb_false_type {};
کينډۍ <ډول_نوم X> بنية hb_is_پایله_نښان<hb_پایله_ok_t<X>> : hb_true_type {};
کينډۍ <ډول_نوم X> بنية hb_is_پایله_نښان<hb_پایله_err_t<X>> : hb_true_type {};

کينډۍ <ډول_نوم T>
ساكن مضمن constexpr hb_پایله_ok_t<T&&> Ok (T&& v)
{
  أرجع hb_پایله_ok_t<T&&>{std::forward<T> (v)};
}

ساكن مضمن constexpr hb_پایله_ok_t<فراغ> Ok ()
{
  أرجع hb_پایله_ok_t<فراغ>{};
}

کينډۍ <ډول_نوم E>
ساكن مضمن constexpr hb_پایله_err_t<E&&> Err (E&& e)
{
  أرجع hb_پایله_err_t<E&&>{std::forward<E> (e)};
}

کينډۍ <ډول_نوم T, ډول_نوم E>
بنية HB_NODISCARD_STRUCT hb_پایله_t
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

  فراغ init(ثابت hb_پایله_t& o) {
    is_ok_ = o.is_ok_;
    إذا (is_ok_)
      جديد (std::addressof (val_)) T (o.val_);
    وإلا
      جديد (std::addressof (err_)) E (o.err_);
  }

  فراغ init(hb_پایله_t&& o) {
    is_ok_ = o.is_ok_;
    إذا (is_ok_)
      جديد (std::addressof (val_)) T (std::move (o.val_));
    وإلا
      جديد (std::addressof (err_)) E (std::move (o.err_));
  }

  عام:
  ~hb_پایله_t ()
  {
    destroy ();
  }

  hb_پایله_t () = ړنګول;

  hb_پایله_t (ثابت hb_پایله_t& o) : is_ok_ (خطأ)
  {
    init (o);
  }

  hb_پایله_t (hb_پایله_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (خطأ)
  {
    init (std::move (o));
  }

  hb_پایله_t& چلوونکی = (ثابت hb_پایله_t& o)
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

  hb_پایله_t& چلوونکی = (hb_پایله_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
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
  کينډۍ <ډول_نوم U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_پایله_t (hb_پایله_ok_t<U>&& o) : is_ok_ (ریښتیا)
  {
    جديد (std::addressof (val_)) T (std::move (o.value));
  }

  کينډۍ <ډول_نوم U,
            hb_enable_if ((std::is_constructible<T, ثابت U&>::value))>
  hb_پایله_t (ثابت hb_پایله_ok_t<U>& o) : is_ok_ (ریښتیا)
  {
    جديد (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  کينډۍ <ډول_نوم F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_پایله_t (hb_پایله_err_t<F>&& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move (e.error));
  }

  کينډۍ <ډول_نوم F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_پایله_t (ثابت hb_پایله_err_t<F>& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  کينډۍ <ډول_نوم F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_پایله_t (F e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  کينډۍ <ډول_نوم U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_پایله_t) &&
                           !hb_is_پایله_نښان<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_پایله_t (U&& v) : is_ok_ (ریښتیا)
  {
    جديد (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  ساكن hb_پایله_t from (T&& fallible, E error_ارزښت) {
    إذا (fallible.دننه_error()) {
      أرجع Err(error_ارزښت);
    }
    أرجع Ok(fallible);
  }

  منطقي is_ok () ثابت { أرجع is_ok_; }
  منطقي is_err () ثابت { أرجع !is_ok_; }
  څرګند چلوونکی منطقي () ثابت { أرجع is_ok (); }

  T& value () & { أكد (is_ok_); أرجع val_; }
  ثابت T& value () ثابت & { أكد (is_ok_); أرجع val_; }
  T value () && { أكد (is_ok_); أرجع std::move (val_); }

  T& چلوونکی * () & { أرجع value (); }
  ثابت T& چلوونکی * () ثابت & { أرجع value (); }
  T چلوونکی * () && { أرجع std::move (*هذا).value (); }

  T* چلوونکی -> () { أرجع std::addressof (value ()); }
  ثابت T* چلوونکی -> () ثابت { أرجع std::addressof (value ()); }

  کينډۍ <ډول_نوم U>
  T ارزښت_or (U&& default_ارزښت) ثابت &
  {
    إذا (is_ok_) أرجع val_;
    أرجع std::forward<U> (default_ارزښت);
  }

  کينډۍ <ډول_نوم U>
  T ارزښت_or (U&& default_ارزښت) &&
  {
    إذا (is_ok_) أرجع std::move (val_);
    أرجع std::forward<U> (default_ارزښت);
  }

  T ارزښت_or_default () ثابت &
  {
    إذا (is_ok_) أرجع val_;
    أرجع T ();
  }

  T ارزښت_or_default () &&
  {
    إذا (is_ok_) أرجع std::move (val_);
    أرجع T ();
  }

  ثابت E& error () ثابت & { أكد (!is_ok_); أرجع err_; }
  E& error () & { أكد (!is_ok_); أرجع err_; }
  E error () && { أكد (!is_ok_); أرجع std::move (err_); }

  منطقي چلوونکی == (ثابت hb_پایله_t& o) ثابت
  {
    إذا (is_ok_ != o.is_ok_) أرجع خطأ;
    إذا (is_ok_) أرجع val_ == o.val_;
    أرجع err_ == o.err_;
  }

  منطقي چلوونکی != (ثابت hb_پایله_t& o) ثابت
  {
    أرجع !(*هذا == o);
  }

  کينډۍ <ډول_نوم U>
  منطقي چلوونکی == (ثابت hb_پایله_ok_t<U>& o) ثابت
  {
    أرجع is_ok_ && val_ == o.value;
  }

  کينډۍ <ډول_نوم U>
  منطقي چلوونکی != (ثابت hb_پایله_ok_t<U>& o) ثابت
  {
    أرجع !(*هذا == o);
  }

  کينډۍ <ډول_نوم F>
  منطقي چلوونکی == (ثابت hb_پایله_err_t<F>& e) ثابت
  {
    أرجع !is_ok_ && err_ == e.error;
  }

  کينډۍ <ډول_نوم F>
  منطقي چلوونکی != (ثابت hb_پایله_err_t<F>& e) ثابت
  {
    أرجع !(*هذا == e);
  }

  کينډۍ <ډول_نوم U>
  منطقي چلوونکی == (ثابت U&) ثابت = ړنګول;

  کينډۍ <ډول_نوم U>
  منطقي چلوونکی != (ثابت U&) ثابت = ړنګول;
};

// Partial specialization for void
کينډۍ <ډول_نوم E>
بنية HB_NODISCARD_STRUCT hb_پایله_t<فراغ, E>
{
  خاص:
  اتحاد
  {
    E err_;
  };
  منطقي is_ok_;

  کينډۍ <ډول_نوم, ډول_نوم> ملګری بنية hb_پایله_t;

  عام:
  ~hb_پایله_t ()
  {
    إذا (!is_ok_)
      err_.~E ();
  }

  hb_پایله_t () : is_ok_ (ریښتیا) {}
  hb_پایله_t (hb_پایله_ok_t<فراغ>) : is_ok_ (ریښتیا) {}

  hb_پایله_t (ثابت hb_پایله_t& o) : is_ok_ (o.is_ok_)
  {
    إذا (!is_ok_)
      جديد (std::addressof (err_)) E (o.err_);
  }

  hb_پایله_t (hb_پایله_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    إذا (!is_ok_)
      جديد (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_پایله_t& چلوونکی = (ثابت hb_پایله_t& o)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    وإلا إذا (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ریښتیا;
    }
    وإلا إذا (is_ok_ && !o.is_ok_)
    {
      جديد (std::addressof (err_)) E (o.err_);
      is_ok_ = خطأ;
    }
    أرجع *هذا;
  }

  hb_پایله_t& چلوونکی = (hb_پایله_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    وإلا إذا (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ریښتیا;
    }
    وإلا إذا (is_ok_ && !o.is_ok_)
    {
      جديد (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = خطأ;
    }
    أرجع *هذا;
  }

  کينډۍ <ډول_نوم F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_پایله_t (hb_پایله_err_t<F>&& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move (e.error));
  }

  کينډۍ <ډول_نوم F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_پایله_t (ثابت hb_پایله_err_t<F>& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_پایله_t (E e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  کينډۍ<ډول_نوم V>
  ساكن hb_پایله_t<فراغ, E> from (ثابت V& fallible, E error_ارزښت) {
    إذا (fallible.دننه_error()) {
      أرجع Err(error_ارزښت);
    }
    أرجع Ok();
  }

  منطقي is_ok () ثابت { أرجع is_ok_; }
  منطقي is_err () ثابت { أرجع !is_ok_; }
  څرګند چلوونکی منطقي () ثابت { أرجع is_ok (); }

  ثابت E& error () ثابت & { أكد (!is_ok_); أرجع err_; }
  E& error () & { أكد (!is_ok_); أرجع err_; }
  E error () && { أكد (!is_ok_); أرجع std::move (err_); }

  فراغ چلوونکی * () & { أرجع; }
  فراغ چلوونکی * () ثابت & { أرجع; }
  فراغ چلوونکی * () && { أرجع; }

  منطقي چلوونکی == (ثابت hb_پایله_t<فراغ, E>& o) ثابت
  {
    أرجع is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  منطقي چلوونکی != (ثابت hb_پایله_t<فراغ, E>& o) ثابت
  {
    أرجع !(*هذا == o);
  }

  منطقي چلوونکی == (hb_پایله_ok_t<فراغ>) ثابت
  {
    أرجع is_ok_;
  }

  منطقي چلوونکی != (hb_پایله_ok_t<فراغ>) ثابت
  {
    أرجع !is_ok_;
  }

  کينډۍ <ډول_نوم F>
  منطقي چلوونکی == (ثابت hb_پایله_err_t<F>& e) ثابت
  {
    أرجع !is_ok_ && err_ == e.error;
  }

  کينډۍ <ډول_نوم F>
  منطقي چلوونکی != (ثابت hb_پایله_err_t<F>& e) ثابت
  {
    أرجع !(*هذا == e);
  }

  کينډۍ <ډول_نوم U>
  منطقي چلوونکی == (ثابت U&) ثابت = ړنګول;

  کينډۍ <ډول_نوم U>
  منطقي چلوونکی != (ثابت U&) ثابت = ړنګول;
};

کينډۍ <ډول_نوم U, ډول_نوم T, ډول_نوم E>
ساكن مضمن منطقي چلوونکی == (ثابت hb_پایله_ok_t<U>& o, ثابت hb_پایله_t<T, E>& r)
{
  أرجع r == o;
}
کينډۍ <ډول_نوم U, ډول_نوم T, ډول_نوم E>
ساكن مضمن منطقي چلوونکی != (ثابت hb_پایله_ok_t<U>& o, ثابت hb_پایله_t<T, E>& r)
{
  أرجع r != o;
}
کينډۍ <ډول_نوم F, ډول_نوم T, ډول_نوم E>
ساكن مضمن منطقي چلوونکی == (ثابت hb_پایله_err_t<F>& e, ثابت hb_پایله_t<T, E>& r)
{
  أرجع r == e;
}
کينډۍ <ډول_نوم F, ډول_نوم T, ډول_نوم E>
ساكن مضمن منطقي چلوونکی != (ثابت hb_پایله_err_t<F>& e, ثابت hb_پایله_t<T, E>& r)
{
  أرجع r != e;
}

#که_نه_تعریف_شوی TRY
#تعریف_کړه TRY(بیان)                                                             \
  افعل {                                                                        \
    خپلکاری _res = (بیان);                                                       \
    إذا (unlikely (!_res.is_ok())) أرجع Err(std::move(_res).error());        \
  } طالما (0)
#د_که_پای

#که_نه_تعریف_شوی TRY_ټاکل
#تعریف_کړه TRY_ټاکل(var, ...)                                                  \
  خپلکاری HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  إذا (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) أرجع Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#د_که_پای

#د_که_پای /* HB_RESULT_HH */
