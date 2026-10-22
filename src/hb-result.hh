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

#ifndef HB_RESULT_HH
#define HB_RESULT_HH

#شامل "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
ساختار hb_result_ok_t
{
  T value;
};

template <>
ساختار hb_result_ok_t<تهی> {};

template <typename E>
ساختار hb_result_err_t
{
  E error;

  template <typename OtherE>
  constexpr منطقی operator == (ثابت hb_result_err_t<OtherE>& other) ثابت { بازگشت error == other.error; }
  template <typename OtherE>
  constexpr منطقی operator != (ثابت hb_result_err_t<OtherE>& other) ثابت { بازگشت error != other.error; }
};

template <typename X> ساختار _hb_is_result_tag : hb_false_type {};
template <typename X> ساختار _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
template <typename X> ساختار _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

template <typename T>
ایستا درونخطی constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  بازگشت hb_result_ok_t<T&&>{std::forward<T> (v)};
}

ایستا درونخطی constexpr hb_result_ok_t<تهی> Ok ()
{
  بازگشت hb_result_ok_t<تهی>{};
}

template <typename E>
ایستا درونخطی constexpr hb_result_err_t<E&&> Err (E&& e)
{
  بازگشت hb_result_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
ساختار HB_NODISCARD_STRUCT hb_result_t
{
  خصوصی:
  اجتماع
  {
    T val_;
    E err_;
  };
  منطقی is_ok_;

  تهی destroy ()
  {
    اگر (is_ok_)
      val_.~T ();
    وگرنه
      err_.~E ();
  }

  تهی init(ثابت hb_result_t& o) {
    is_ok_ = o.is_ok_;
    اگر (is_ok_)
      جدید (std::addressof (val_)) T (o.val_);
    وگرنه
      جدید (std::addressof (err_)) E (o.err_);
  }

  تهی init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    اگر (is_ok_)
      جدید (std::addressof (val_)) T (std::move (o.val_));
    وگرنه
      جدید (std::addressof (err_)) E (std::move (o.err_));
  }

  عمومی:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (ثابت hb_result_t& o) : is_ok_ (نادرست)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (نادرست)
  {
    init (std::move (o));
  }

  hb_result_t& operator = (ثابت hb_result_t& o)
  {
    اگر (این == &o) بازگشت *این;
    اگر (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    وگرنه اگر (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    وگرنه
    {
      destroy ();
      init (o);
    }
    بازگشت *این;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    اگر (این == &o) بازگشت *این;
    اگر (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    وگرنه اگر (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    وگرنه
    {
      destroy ();
      init (std::move (o));
    }
    بازگشت *این;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (درست)
  {
    جدید (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, ثابت U&>::value))>
  hb_result_t (ثابت hb_result_ok_t<U>& o) : is_ok_ (درست)
  {
    جدید (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (نادرست)
  {
    جدید (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_result_t (ثابت hb_result_err_t<F>& e) : is_ok_ (نادرست)
  {
    جدید (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (نادرست)
  {
    جدید (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (درست)
  {
    جدید (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  ایستا hb_result_t from (T&& fallible, E error_مقدار) {
    اگر (fallible.in_error()) {
      بازگشت Err(error_مقدار);
    }
    بازگشت Ok(fallible);
  }

  منطقی is_ok () ثابت { بازگشت is_ok_; }
  منطقی is_err () ثابت { بازگشت !is_ok_; }
  explicit operator منطقی () ثابت { بازگشت is_ok (); }

  T& value () & { أكد (is_ok_); بازگشت val_; }
  ثابت T& value () ثابت & { أكد (is_ok_); بازگشت val_; }
  T value () && { أكد (is_ok_); بازگشت std::move (val_); }

  T& operator * () & { بازگشت value (); }
  ثابت T& operator * () ثابت & { بازگشت value (); }
  T operator * () && { بازگشت std::move (*این).value (); }

  T* operator -> () { بازگشت std::addressof (value ()); }
  ثابت T* operator -> () ثابت { بازگشت std::addressof (value ()); }

  template <typename U>
  T مقدار_or (U&& default_مقدار) ثابت &
  {
    اگر (is_ok_) بازگشت val_;
    بازگشت std::forward<U> (default_مقدار);
  }

  template <typename U>
  T مقدار_or (U&& default_مقدار) &&
  {
    اگر (is_ok_) بازگشت std::move (val_);
    بازگشت std::forward<U> (default_مقدار);
  }

  T مقدار_or_default () ثابت &
  {
    اگر (is_ok_) بازگشت val_;
    بازگشت T ();
  }

  T مقدار_or_default () &&
  {
    اگر (is_ok_) بازگشت std::move (val_);
    بازگشت T ();
  }

  ثابت E& error () ثابت & { أكد (!is_ok_); بازگشت err_; }
  E& error () & { أكد (!is_ok_); بازگشت err_; }
  E error () && { أكد (!is_ok_); بازگشت std::move (err_); }

  منطقی operator == (ثابت hb_result_t& o) ثابت
  {
    اگر (is_ok_ != o.is_ok_) بازگشت نادرست;
    اگر (is_ok_) بازگشت val_ == o.val_;
    بازگشت err_ == o.err_;
  }

  منطقی operator != (ثابت hb_result_t& o) ثابت
  {
    بازگشت !(*این == o);
  }

  template <typename U>
  منطقی operator == (ثابت hb_result_ok_t<U>& o) ثابت
  {
    بازگشت is_ok_ && val_ == o.value;
  }

  template <typename U>
  منطقی operator != (ثابت hb_result_ok_t<U>& o) ثابت
  {
    بازگشت !(*این == o);
  }

  template <typename F>
  منطقی operator == (ثابت hb_result_err_t<F>& e) ثابت
  {
    بازگشت !is_ok_ && err_ == e.error;
  }

  template <typename F>
  منطقی operator != (ثابت hb_result_err_t<F>& e) ثابت
  {
    بازگشت !(*این == e);
  }

  template <typename U>
  منطقی operator == (ثابت U&) ثابت = delete;

  template <typename U>
  منطقی operator != (ثابت U&) ثابت = delete;
};

// Partial specialization for void
template <typename E>
ساختار HB_NODISCARD_STRUCT hb_result_t<تهی, E>
{
  خصوصی:
  اجتماع
  {
    E err_;
  };
  منطقی is_ok_;

  template <typename, typename> friend ساختار hb_result_t;

  عمومی:
  ~hb_result_t ()
  {
    اگر (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (درست) {}
  hb_result_t (hb_result_ok_t<تهی>) : is_ok_ (درست) {}

  hb_result_t (ثابت hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    اگر (!is_ok_)
      جدید (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    اگر (!is_ok_)
      جدید (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& operator = (ثابت hb_result_t& o)
  {
    اگر (این == &o) بازگشت *این;
    اگر (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    وگرنه اگر (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = درست;
    }
    وگرنه اگر (is_ok_ && !o.is_ok_)
    {
      جدید (std::addressof (err_)) E (o.err_);
      is_ok_ = نادرست;
    }
    بازگشت *این;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    اگر (این == &o) بازگشت *این;
    اگر (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    وگرنه اگر (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = درست;
    }
    وگرنه اگر (is_ok_ && !o.is_ok_)
    {
      جدید (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = نادرست;
    }
    بازگشت *این;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (نادرست)
  {
    جدید (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_result_t (ثابت hb_result_err_t<F>& e) : is_ok_ (نادرست)
  {
    جدید (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (نادرست)
  {
    جدید (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  ایستا hb_result_t<تهی, E> from (ثابت V& fallible, E error_مقدار) {
    اگر (fallible.in_error()) {
      بازگشت Err(error_مقدار);
    }
    بازگشت Ok();
  }

  منطقی is_ok () ثابت { بازگشت is_ok_; }
  منطقی is_err () ثابت { بازگشت !is_ok_; }
  explicit operator منطقی () ثابت { بازگشت is_ok (); }

  ثابت E& error () ثابت & { أكد (!is_ok_); بازگشت err_; }
  E& error () & { أكد (!is_ok_); بازگشت err_; }
  E error () && { أكد (!is_ok_); بازگشت std::move (err_); }

  تهی operator * () & { بازگشت; }
  تهی operator * () ثابت & { بازگشت; }
  تهی operator * () && { بازگشت; }

  منطقی operator == (ثابت hb_result_t<تهی, E>& o) ثابت
  {
    بازگشت is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  منطقی operator != (ثابت hb_result_t<تهی, E>& o) ثابت
  {
    بازگشت !(*این == o);
  }

  منطقی operator == (hb_result_ok_t<تهی>) ثابت
  {
    بازگشت is_ok_;
  }

  منطقی operator != (hb_result_ok_t<تهی>) ثابت
  {
    بازگشت !is_ok_;
  }

  template <typename F>
  منطقی operator == (ثابت hb_result_err_t<F>& e) ثابت
  {
    بازگشت !is_ok_ && err_ == e.error;
  }

  template <typename F>
  منطقی operator != (ثابت hb_result_err_t<F>& e) ثابت
  {
    بازگشت !(*این == e);
  }

  template <typename U>
  منطقی operator == (ثابت U&) ثابت = delete;

  template <typename U>
  منطقی operator != (ثابت U&) ثابت = delete;
};

template <typename U, typename T, typename E>
ایستا درونخطی منطقی operator == (ثابت hb_result_ok_t<U>& o, ثابت hb_result_t<T, E>& r)
{
  بازگشت r == o;
}
template <typename U, typename T, typename E>
ایستا درونخطی منطقی operator != (ثابت hb_result_ok_t<U>& o, ثابت hb_result_t<T, E>& r)
{
  بازگشت r != o;
}
template <typename F, typename T, typename E>
ایستا درونخطی منطقی operator == (ثابت hb_result_err_t<F>& e, ثابت hb_result_t<T, E>& r)
{
  بازگشت r == e;
}
template <typename F, typename T, typename E>
ایستا درونخطی منطقی operator != (ثابت hb_result_err_t<F>& e, ثابت hb_result_t<T, E>& r)
{
  بازگشت r != e;
}

#ifndef TRY
#define TRY(عبارت)                                                             \
  انجامبده {                                                                        \
    auto _res = (عبارت);                                                       \
    اگر (unlikely (!_res.is_ok())) بازگشت Err(std::move(_res).error());        \
  } وقتی (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  اگر (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) بازگشت Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
