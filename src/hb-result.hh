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

#অন্তর্ভাৱ "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
সংরচনা hb_result_ok_t
{
  T value;
};

template <>
সংরচনা hb_result_ok_t<শূন্য> {};

template <typename E>
সংরচনা hb_result_err_t
{
  E error;

  template <typename OtherE>
  constexpr তার্কিক operator == (ধ্রুৱ hb_result_err_t<OtherE>& other) ধ্রুৱ { ঘূৰাই error == other.error; }
  template <typename OtherE>
  constexpr তার্কিক operator != (ধ্রুৱ hb_result_err_t<OtherE>& other) ধ্রুৱ { ঘূৰাই error != other.error; }
};

template <typename X> সংরচনা _hb_is_result_tag : hb_false_type {};
template <typename X> সংরচনা _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
template <typename X> সংরচনা _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

template <typename T>
স্থির অন্তঃস্থ constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  ঘূৰাই hb_result_ok_t<T&&>{std::forward<T> (v)};
}

স্থির অন্তঃস্থ constexpr hb_result_ok_t<শূন্য> Ok ()
{
  ঘূৰাই hb_result_ok_t<শূন্য>{};
}

template <typename E>
স্থির অন্তঃস্থ constexpr hb_result_err_t<E&&> Err (E&& e)
{
  ঘূৰাই hb_result_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
সংরচনা HB_NODISCARD_STRUCT hb_result_t
{
  গুপ্ত:
  সঙ্ঘ
  {
    T val_;
    E err_;
  };
  তার্কিক is_ok_;

  শূন্য destroy ()
  {
    যদি (is_ok_)
      val_.~T ();
    অন্যথা
      err_.~E ();
  }

  শূন্য init(ধ্রুৱ hb_result_t& o) {
    is_ok_ = o.is_ok_;
    যদি (is_ok_)
      নূতন (std::addressof (val_)) T (o.val_);
    অন্যথা
      নূতন (std::addressof (err_)) E (o.err_);
  }

  শূন্য init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    যদি (is_ok_)
      নূতন (std::addressof (val_)) T (std::move (o.val_));
    অন্যথা
      নূতন (std::addressof (err_)) E (std::move (o.err_));
  }

  সার্ৱজনিক:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (ধ্রুৱ hb_result_t& o) : is_ok_ (মিছা)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (মিছা)
  {
    init (std::move (o));
  }

  hb_result_t& operator = (ধ্রুৱ hb_result_t& o)
  {
    যদি (এতত্ == &o) ঘূৰাই *এতত্;
    যদি (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    অন্যথা যদি (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    অন্যথা
    {
      destroy ();
      init (o);
    }
    ঘূৰাই *এতত্;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    যদি (এতত্ == &o) ঘূৰাই *এতত্;
    যদি (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    অন্যথা যদি (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    অন্যথা
    {
      destroy ();
      init (std::move (o));
    }
    ঘূৰাই *এতত্;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (সঁচা)
  {
    নূতন (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, ধ্রুৱ U&>::value))>
  hb_result_t (ধ্রুৱ hb_result_ok_t<U>& o) : is_ok_ (সঁচা)
  {
    নূতন (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (মিছা)
  {
    নূতন (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ধ্রুৱ F&>::value))>
  hb_result_t (ধ্রুৱ hb_result_err_t<F>& e) : is_ok_ (মিছা)
  {
    নূতন (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (মিছা)
  {
    নূতন (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (সঁচা)
  {
    নূতন (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  স্থির hb_result_t from (T&& fallible, E error_মান) {
    যদি (fallible.in_error()) {
      ঘূৰাই Err(error_মান);
    }
    ঘূৰাই Ok(fallible);
  }

  তার্কিক is_ok () ধ্রুৱ { ঘূৰাই is_ok_; }
  তার্কিক is_err () ধ্রুৱ { ঘূৰাই !is_ok_; }
  explicit operator তার্কিক () ধ্রুৱ { ঘূৰাই is_ok (); }

  T& value () & { প্রতিজ্ঞা (is_ok_); ঘূৰাই val_; }
  ধ্রুৱ T& value () ধ্রুৱ & { প্রতিজ্ঞা (is_ok_); ঘূৰাই val_; }
  T value () && { প্রতিজ্ঞা (is_ok_); ঘূৰাই std::move (val_); }

  T& operator * () & { ঘূৰাই value (); }
  ধ্রুৱ T& operator * () ধ্রুৱ & { ঘূৰাই value (); }
  T operator * () && { ঘূৰাই std::move (*এতত্).value (); }

  T* operator -> () { ঘূৰাই std::addressof (value ()); }
  ধ্রুৱ T* operator -> () ধ্রুৱ { ঘূৰাই std::addressof (value ()); }

  template <typename U>
  T মান_or (U&& default_মান) ধ্রুৱ &
  {
    যদি (is_ok_) ঘূৰাই val_;
    ঘূৰাই std::forward<U> (default_মান);
  }

  template <typename U>
  T মান_or (U&& default_মান) &&
  {
    যদি (is_ok_) ঘূৰাই std::move (val_);
    ঘূৰাই std::forward<U> (default_মান);
  }

  T মান_or_default () ধ্রুৱ &
  {
    যদি (is_ok_) ঘূৰাই val_;
    ঘূৰাই T ();
  }

  T মান_or_default () &&
  {
    যদি (is_ok_) ঘূৰাই std::move (val_);
    ঘূৰাই T ();
  }

  ধ্রুৱ E& error () ধ্রুৱ & { প্রতিজ্ঞা (!is_ok_); ঘূৰাই err_; }
  E& error () & { প্রতিজ্ঞা (!is_ok_); ঘূৰাই err_; }
  E error () && { প্রতিজ্ঞা (!is_ok_); ঘূৰাই std::move (err_); }

  তার্কিক operator == (ধ্রুৱ hb_result_t& o) ধ্রুৱ
  {
    যদি (is_ok_ != o.is_ok_) ঘূৰাই মিছা;
    যদি (is_ok_) ঘূৰাই val_ == o.val_;
    ঘূৰাই err_ == o.err_;
  }

  তার্কিক operator != (ধ্রুৱ hb_result_t& o) ধ্রুৱ
  {
    ঘূৰাই !(*এতত্ == o);
  }

  template <typename U>
  তার্কিক operator == (ধ্রুৱ hb_result_ok_t<U>& o) ধ্রুৱ
  {
    ঘূৰাই is_ok_ && val_ == o.value;
  }

  template <typename U>
  তার্কিক operator != (ধ্রুৱ hb_result_ok_t<U>& o) ধ্রুৱ
  {
    ঘূৰাই !(*এতত্ == o);
  }

  template <typename F>
  তার্কিক operator == (ধ্রুৱ hb_result_err_t<F>& e) ধ্রুৱ
  {
    ঘূৰাই !is_ok_ && err_ == e.error;
  }

  template <typename F>
  তার্কিক operator != (ধ্রুৱ hb_result_err_t<F>& e) ধ্রুৱ
  {
    ঘূৰাই !(*এতত্ == e);
  }

  template <typename U>
  তার্কিক operator == (ধ্রুৱ U&) ধ্রুৱ = delete;

  template <typename U>
  তার্কিক operator != (ধ্রুৱ U&) ধ্রুৱ = delete;
};

// Partial specialization for void
template <typename E>
সংরচনা HB_NODISCARD_STRUCT hb_result_t<শূন্য, E>
{
  গুপ্ত:
  সঙ্ঘ
  {
    E err_;
  };
  তার্কিক is_ok_;

  template <typename, typename> friend সংরচনা hb_result_t;

  সার্ৱজনিক:
  ~hb_result_t ()
  {
    যদি (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (সঁচা) {}
  hb_result_t (hb_result_ok_t<শূন্য>) : is_ok_ (সঁচা) {}

  hb_result_t (ধ্রুৱ hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    যদি (!is_ok_)
      নূতন (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    যদি (!is_ok_)
      নূতন (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& operator = (ধ্রুৱ hb_result_t& o)
  {
    যদি (এতত্ == &o) ঘূৰাই *এতত্;
    যদি (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    অন্যথা যদি (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = সঁচা;
    }
    অন্যথা যদি (is_ok_ && !o.is_ok_)
    {
      নূতন (std::addressof (err_)) E (o.err_);
      is_ok_ = মিছা;
    }
    ঘূৰাই *এতত্;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    যদি (এতত্ == &o) ঘূৰাই *এতত্;
    যদি (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    অন্যথা যদি (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = সঁচা;
    }
    অন্যথা যদি (is_ok_ && !o.is_ok_)
    {
      নূতন (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = মিছা;
    }
    ঘূৰাই *এতত্;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (মিছা)
  {
    নূতন (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ধ্রুৱ F&>::value))>
  hb_result_t (ধ্রুৱ hb_result_err_t<F>& e) : is_ok_ (মিছা)
  {
    নূতন (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (মিছা)
  {
    নূতন (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  স্থির hb_result_t<শূন্য, E> from (ধ্রুৱ V& fallible, E error_মান) {
    যদি (fallible.in_error()) {
      ঘূৰাই Err(error_মান);
    }
    ঘূৰাই Ok();
  }

  তার্কিক is_ok () ধ্রুৱ { ঘূৰাই is_ok_; }
  তার্কিক is_err () ধ্রুৱ { ঘূৰাই !is_ok_; }
  explicit operator তার্কিক () ধ্রুৱ { ঘূৰাই is_ok (); }

  ধ্রুৱ E& error () ধ্রুৱ & { প্রতিজ্ঞা (!is_ok_); ঘূৰাই err_; }
  E& error () & { প্রতিজ্ঞা (!is_ok_); ঘূৰাই err_; }
  E error () && { প্রতিজ্ঞা (!is_ok_); ঘূৰাই std::move (err_); }

  শূন্য operator * () & { ঘূৰাই; }
  শূন্য operator * () ধ্রুৱ & { ঘূৰাই; }
  শূন্য operator * () && { ঘূৰাই; }

  তার্কিক operator == (ধ্রুৱ hb_result_t<শূন্য, E>& o) ধ্রুৱ
  {
    ঘূৰাই is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  তার্কিক operator != (ধ্রুৱ hb_result_t<শূন্য, E>& o) ধ্রুৱ
  {
    ঘূৰাই !(*এতত্ == o);
  }

  তার্কিক operator == (hb_result_ok_t<শূন্য>) ধ্রুৱ
  {
    ঘূৰাই is_ok_;
  }

  তার্কিক operator != (hb_result_ok_t<শূন্য>) ধ্রুৱ
  {
    ঘূৰাই !is_ok_;
  }

  template <typename F>
  তার্কিক operator == (ধ্রুৱ hb_result_err_t<F>& e) ধ্রুৱ
  {
    ঘূৰাই !is_ok_ && err_ == e.error;
  }

  template <typename F>
  তার্কিক operator != (ধ্রুৱ hb_result_err_t<F>& e) ধ্রুৱ
  {
    ঘূৰাই !(*এতত্ == e);
  }

  template <typename U>
  তার্কিক operator == (ধ্রুৱ U&) ধ্রুৱ = delete;

  template <typename U>
  তার্কিক operator != (ধ্রুৱ U&) ধ্রুৱ = delete;
};

template <typename U, typename T, typename E>
স্থির অন্তঃস্থ তার্কিক operator == (ধ্রুৱ hb_result_ok_t<U>& o, ধ্রুৱ hb_result_t<T, E>& r)
{
  ঘূৰাই r == o;
}
template <typename U, typename T, typename E>
স্থির অন্তঃস্থ তার্কিক operator != (ধ্রুৱ hb_result_ok_t<U>& o, ধ্রুৱ hb_result_t<T, E>& r)
{
  ঘূৰাই r != o;
}
template <typename F, typename T, typename E>
স্থির অন্তঃস্থ তার্কিক operator == (ধ্রুৱ hb_result_err_t<F>& e, ধ্রুৱ hb_result_t<T, E>& r)
{
  ঘূৰাই r == e;
}
template <typename F, typename T, typename E>
স্থির অন্তঃস্থ তার্কিক operator != (ধ্রুৱ hb_result_err_t<F>& e, ধ্রুৱ hb_result_t<T, E>& r)
{
  ঘূৰাই r != e;
}

#ifndef TRY
#define TRY(ৰাশিমালা)                                                             \
  কৰা {                                                                        \
    auto _res = (ৰাশিমালা);                                                       \
    যদি (unlikely (!_res.is_ok())) ঘূৰাই Err(std::move(_res).error());        \
  } যেতিয়ালৈ (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  যদি (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) ঘূৰাই Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
