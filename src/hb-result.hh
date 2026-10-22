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

#अन्तर्भाव "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
संरचना hb_result_ok_t
{
  T value;
};

template <>
संरचना hb_result_ok_t<शून्य> {};

template <typename E>
संरचना hb_result_err_t
{
  E error;

  template <typename OtherE>
  constexpr तार्किक operator == (ध्रुव hb_result_err_t<OtherE>& other) ध्रुव { पटिनिवत्त error == other.error; }
  template <typename OtherE>
  constexpr तार्किक operator != (ध्रुव hb_result_err_t<OtherE>& other) ध्रुव { पटिनिवत्त error != other.error; }
};

template <typename X> संरचना _hb_is_result_tag : hb_false_type {};
template <typename X> संरचना _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
template <typename X> संरचना _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

template <typename T>
स्थिर अन्तःस्थ constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  पटिनिवत्त hb_result_ok_t<T&&>{std::forward<T> (v)};
}

स्थिर अन्तःस्थ constexpr hb_result_ok_t<शून्य> Ok ()
{
  पटिनिवत्त hb_result_ok_t<शून्य>{};
}

template <typename E>
स्थिर अन्तःस्थ constexpr hb_result_err_t<E&&> Err (E&& e)
{
  पटिनिवत्त hb_result_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
संरचना HB_NODISCARD_STRUCT hb_result_t
{
  गुप्त:
  सङ्घ
  {
    T val_;
    E err_;
  };
  तार्किक is_ok_;

  शून्य destroy ()
  {
    सचे (is_ok_)
      val_.~T ();
    अञ्ञथा
      err_.~E ();
  }

  शून्य init(ध्रुव hb_result_t& o) {
    is_ok_ = o.is_ok_;
    सचे (is_ok_)
      नूतन (std::addressof (val_)) T (o.val_);
    अञ्ञथा
      नूतन (std::addressof (err_)) E (o.err_);
  }

  शून्य init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    सचे (is_ok_)
      नूतन (std::addressof (val_)) T (std::move (o.val_));
    अञ्ञथा
      नूतन (std::addressof (err_)) E (std::move (o.err_));
  }

  सार्वजनिक:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (ध्रुव hb_result_t& o) : is_ok_ (मिच्छा)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (मिच्छा)
  {
    init (std::move (o));
  }

  hb_result_t& operator = (ध्रुव hb_result_t& o)
  {
    सचे (एतत् == &o) पटिनिवत्त *एतत्;
    सचे (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    अञ्ञथा सचे (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    अञ्ञथा
    {
      destroy ();
      init (o);
    }
    पटिनिवत्त *एतत्;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    सचे (एतत् == &o) पटिनिवत्त *एतत्;
    सचे (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    अञ्ञथा सचे (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    अञ्ञथा
    {
      destroy ();
      init (std::move (o));
    }
    पटिनिवत्त *एतत्;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (सच्च)
  {
    नूतन (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, ध्रुव U&>::value))>
  hb_result_t (ध्रुव hb_result_ok_t<U>& o) : is_ok_ (सच्च)
  {
    नूतन (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (मिच्छा)
  {
    नूतन (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ध्रुव F&>::value))>
  hb_result_t (ध्रुव hb_result_err_t<F>& e) : is_ok_ (मिच्छा)
  {
    नूतन (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (मिच्छा)
  {
    नूतन (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (सच्च)
  {
    नूतन (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  स्थिर hb_result_t from (T&& fallible, E error_मानम्) {
    सचे (fallible.in_error()) {
      पटिनिवत्त Err(error_मानम्);
    }
    पटिनिवत्त Ok(fallible);
  }

  तार्किक is_ok () ध्रुव { पटिनिवत्त is_ok_; }
  तार्किक is_err () ध्रुव { पटिनिवत्त !is_ok_; }
  explicit operator तार्किक () ध्रुव { पटिनिवत्त is_ok (); }

  T& value () & { प्रतिज्ञा (is_ok_); पटिनिवत्त val_; }
  ध्रुव T& value () ध्रुव & { प्रतिज्ञा (is_ok_); पटिनिवत्त val_; }
  T value () && { प्रतिज्ञा (is_ok_); पटिनिवत्त std::move (val_); }

  T& operator * () & { पटिनिवत्त value (); }
  ध्रुव T& operator * () ध्रुव & { पटिनिवत्त value (); }
  T operator * () && { पटिनिवत्त std::move (*एतत्).value (); }

  T* operator -> () { पटिनिवत्त std::addressof (value ()); }
  ध्रुव T* operator -> () ध्रुव { पटिनिवत्त std::addressof (value ()); }

  template <typename U>
  T मानम्_or (U&& default_मानम्) ध्रुव &
  {
    सचे (is_ok_) पटिनिवत्त val_;
    पटिनिवत्त std::forward<U> (default_मानम्);
  }

  template <typename U>
  T मानम्_or (U&& default_मानम्) &&
  {
    सचे (is_ok_) पटिनिवत्त std::move (val_);
    पटिनिवत्त std::forward<U> (default_मानम्);
  }

  T मानम्_or_default () ध्रुव &
  {
    सचे (is_ok_) पटिनिवत्त val_;
    पटिनिवत्त T ();
  }

  T मानम्_or_default () &&
  {
    सचे (is_ok_) पटिनिवत्त std::move (val_);
    पटिनिवत्त T ();
  }

  ध्रुव E& error () ध्रुव & { प्रतिज्ञा (!is_ok_); पटिनिवत्त err_; }
  E& error () & { प्रतिज्ञा (!is_ok_); पटिनिवत्त err_; }
  E error () && { प्रतिज्ञा (!is_ok_); पटिनिवत्त std::move (err_); }

  तार्किक operator == (ध्रुव hb_result_t& o) ध्रुव
  {
    सचे (is_ok_ != o.is_ok_) पटिनिवत्त मिच्छा;
    सचे (is_ok_) पटिनिवत्त val_ == o.val_;
    पटिनिवत्त err_ == o.err_;
  }

  तार्किक operator != (ध्रुव hb_result_t& o) ध्रुव
  {
    पटिनिवत्त !(*एतत् == o);
  }

  template <typename U>
  तार्किक operator == (ध्रुव hb_result_ok_t<U>& o) ध्रुव
  {
    पटिनिवत्त is_ok_ && val_ == o.value;
  }

  template <typename U>
  तार्किक operator != (ध्रुव hb_result_ok_t<U>& o) ध्रुव
  {
    पटिनिवत्त !(*एतत् == o);
  }

  template <typename F>
  तार्किक operator == (ध्रुव hb_result_err_t<F>& e) ध्रुव
  {
    पटिनिवत्त !is_ok_ && err_ == e.error;
  }

  template <typename F>
  तार्किक operator != (ध्रुव hb_result_err_t<F>& e) ध्रुव
  {
    पटिनिवत्त !(*एतत् == e);
  }

  template <typename U>
  तार्किक operator == (ध्रुव U&) ध्रुव = delete;

  template <typename U>
  तार्किक operator != (ध्रुव U&) ध्रुव = delete;
};

// Partial specialization for void
template <typename E>
संरचना HB_NODISCARD_STRUCT hb_result_t<शून्य, E>
{
  गुप्त:
  सङ्घ
  {
    E err_;
  };
  तार्किक is_ok_;

  template <typename, typename> friend संरचना hb_result_t;

  सार्वजनिक:
  ~hb_result_t ()
  {
    सचे (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (सच्च) {}
  hb_result_t (hb_result_ok_t<शून्य>) : is_ok_ (सच्च) {}

  hb_result_t (ध्रुव hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    सचे (!is_ok_)
      नूतन (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    सचे (!is_ok_)
      नूतन (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& operator = (ध्रुव hb_result_t& o)
  {
    सचे (एतत् == &o) पटिनिवत्त *एतत्;
    सचे (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    अञ्ञथा सचे (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = सच्च;
    }
    अञ्ञथा सचे (is_ok_ && !o.is_ok_)
    {
      नूतन (std::addressof (err_)) E (o.err_);
      is_ok_ = मिच्छा;
    }
    पटिनिवत्त *एतत्;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    सचे (एतत् == &o) पटिनिवत्त *एतत्;
    सचे (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    अञ्ञथा सचे (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = सच्च;
    }
    अञ्ञथा सचे (is_ok_ && !o.is_ok_)
    {
      नूतन (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = मिच्छा;
    }
    पटिनिवत्त *एतत्;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (मिच्छा)
  {
    नूतन (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ध्रुव F&>::value))>
  hb_result_t (ध्रुव hb_result_err_t<F>& e) : is_ok_ (मिच्छा)
  {
    नूतन (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (मिच्छा)
  {
    नूतन (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  स्थिर hb_result_t<शून्य, E> from (ध्रुव V& fallible, E error_मानम्) {
    सचे (fallible.in_error()) {
      पटिनिवत्त Err(error_मानम्);
    }
    पटिनिवत्त Ok();
  }

  तार्किक is_ok () ध्रुव { पटिनिवत्त is_ok_; }
  तार्किक is_err () ध्रुव { पटिनिवत्त !is_ok_; }
  explicit operator तार्किक () ध्रुव { पटिनिवत्त is_ok (); }

  ध्रुव E& error () ध्रुव & { प्रतिज्ञा (!is_ok_); पटिनिवत्त err_; }
  E& error () & { प्रतिज्ञा (!is_ok_); पटिनिवत्त err_; }
  E error () && { प्रतिज्ञा (!is_ok_); पटिनिवत्त std::move (err_); }

  शून्य operator * () & { पटिनिवत्त; }
  शून्य operator * () ध्रुव & { पटिनिवत्त; }
  शून्य operator * () && { पटिनिवत्त; }

  तार्किक operator == (ध्रुव hb_result_t<शून्य, E>& o) ध्रुव
  {
    पटिनिवत्त is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  तार्किक operator != (ध्रुव hb_result_t<शून्य, E>& o) ध्रुव
  {
    पटिनिवत्त !(*एतत् == o);
  }

  तार्किक operator == (hb_result_ok_t<शून्य>) ध्रुव
  {
    पटिनिवत्त is_ok_;
  }

  तार्किक operator != (hb_result_ok_t<शून्य>) ध्रुव
  {
    पटिनिवत्त !is_ok_;
  }

  template <typename F>
  तार्किक operator == (ध्रुव hb_result_err_t<F>& e) ध्रुव
  {
    पटिनिवत्त !is_ok_ && err_ == e.error;
  }

  template <typename F>
  तार्किक operator != (ध्रुव hb_result_err_t<F>& e) ध्रुव
  {
    पटिनिवत्त !(*एतत् == e);
  }

  template <typename U>
  तार्किक operator == (ध्रुव U&) ध्रुव = delete;

  template <typename U>
  तार्किक operator != (ध्रुव U&) ध्रुव = delete;
};

template <typename U, typename T, typename E>
स्थिर अन्तःस्थ तार्किक operator == (ध्रुव hb_result_ok_t<U>& o, ध्रुव hb_result_t<T, E>& r)
{
  पटिनिवत्त r == o;
}
template <typename U, typename T, typename E>
स्थिर अन्तःस्थ तार्किक operator != (ध्रुव hb_result_ok_t<U>& o, ध्रुव hb_result_t<T, E>& r)
{
  पटिनिवत्त r != o;
}
template <typename F, typename T, typename E>
स्थिर अन्तःस्थ तार्किक operator == (ध्रुव hb_result_err_t<F>& e, ध्रुव hb_result_t<T, E>& r)
{
  पटिनिवत्त r == e;
}
template <typename F, typename T, typename E>
स्थिर अन्तःस्थ तार्किक operator != (ध्रुव hb_result_err_t<F>& e, ध्रुव hb_result_t<T, E>& r)
{
  पटिनिवत्त r != e;
}

#ifndef TRY
#define TRY(व्यञ्जकम्)                                                             \
  करोहि {                                                                        \
    auto _res = (व्यञ्जकम्);                                                       \
    सचे (unlikely (!_res.is_ok())) पटिनिवत्त Err(std::move(_res).error());        \
  } याव (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  सचे (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) पटिनिवत्त Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
