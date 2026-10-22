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
ढाँचा <typename T>
संरचना hb_result_ok_t
{
  T value;
};

ढाँचा <>
संरचना hb_result_ok_t<शून्य> {};

ढाँचा <typename E>
संरचना hb_result_err_t
{
  E error;

  ढाँचा <typename OtherE>
  constexpr तार्किक चालक == (स्थिरांक hb_result_err_t<OtherE>& other) स्थिरांक { परत error == other.error; }
  ढाँचा <typename OtherE>
  constexpr तार्किक चालक != (स्थिरांक hb_result_err_t<OtherE>& other) स्थिरांक { परत error != other.error; }
};

ढाँचा <typename X> संरचना _hb_is_result_tag : hb_false_type {};
ढाँचा <typename X> संरचना _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
ढाँचा <typename X> संरचना _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

ढाँचा <typename T>
स्थिर ओळीत constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  परत hb_result_ok_t<T&&>{std::forward<T> (v)};
}

स्थिर ओळीत constexpr hb_result_ok_t<शून्य> Ok ()
{
  परत hb_result_ok_t<शून्य>{};
}

ढाँचा <typename E>
स्थिर ओळीत constexpr hb_result_err_t<E&&> Err (E&& e)
{
  परत hb_result_err_t<E&&>{std::forward<E> (e)};
}

ढाँचा <typename T, typename E>
संरचना HB_NODISCARD_STRUCT hb_result_t
{
  खाजगी:
  संघ
  {
    T val_;
    E err_;
  };
  तार्किक is_ok_;

  शून्य destroy ()
  {
    जर (is_ok_)
      val_.~T ();
    अन्यथा
      err_.~E ();
  }

  शून्य init(स्थिरांक hb_result_t& o) {
    is_ok_ = o.is_ok_;
    जर (is_ok_)
      नवीन (std::addressof (val_)) T (o.val_);
    अन्यथा
      नवीन (std::addressof (err_)) E (o.err_);
  }

  शून्य init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    जर (is_ok_)
      नवीन (std::addressof (val_)) T (std::move (o.val_));
    अन्यथा
      नवीन (std::addressof (err_)) E (std::move (o.err_));
  }

  सार्वजनिक:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (स्थिरांक hb_result_t& o) : is_ok_ (असत्य)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (असत्य)
  {
    init (std::move (o));
  }

  hb_result_t& चालक = (स्थिरांक hb_result_t& o)
  {
    जर (हे == &o) परत *हे;
    जर (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    अन्यथा जर (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    अन्यथा
    {
      destroy ();
      init (o);
    }
    परत *हे;
  }

  hb_result_t& चालक = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    जर (हे == &o) परत *हे;
    जर (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    अन्यथा जर (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    अन्यथा
    {
      destroy ();
      init (std::move (o));
    }
    परत *हे;
  }

  // Construct from Ok(...) tag
  ढाँचा <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (सत्य)
  {
    नवीन (std::addressof (val_)) T (std::move (o.value));
  }

  ढाँचा <typename U,
            hb_enable_if ((std::is_constructible<T, स्थिरांक U&>::value))>
  hb_result_t (स्थिरांक hb_result_ok_t<U>& o) : is_ok_ (सत्य)
  {
    नवीन (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  ढाँचा <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (असत्य)
  {
    नवीन (std::addressof (err_)) E (std::move (e.error));
  }

  ढाँचा <typename F,
            hb_enable_if ((std::is_constructible<E, स्थिरांक F&>::value))>
  hb_result_t (स्थिरांक hb_result_err_t<F>& e) : is_ok_ (असत्य)
  {
    नवीन (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  ढाँचा <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (असत्य)
  {
    नवीन (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  ढाँचा <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (सत्य)
  {
    नवीन (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  स्थिर hb_result_t from (T&& fallible, E error_मूल्य) {
    जर (fallible.in_error()) {
      परत Err(error_मूल्य);
    }
    परत Ok(fallible);
  }

  तार्किक is_ok () स्थिरांक { परत is_ok_; }
  तार्किक is_err () स्थिरांक { परत !is_ok_; }
  explicit चालक तार्किक () स्थिरांक { परत is_ok (); }

  T& value () & { प्रतिज्ञा (is_ok_); परत val_; }
  स्थिरांक T& value () स्थिरांक & { प्रतिज्ञा (is_ok_); परत val_; }
  T value () && { प्रतिज्ञा (is_ok_); परत std::move (val_); }

  T& चालक * () & { परत value (); }
  स्थिरांक T& चालक * () स्थिरांक & { परत value (); }
  T चालक * () && { परत std::move (*हे).value (); }

  T* चालक -> () { परत std::addressof (value ()); }
  स्थिरांक T* चालक -> () स्थिरांक { परत std::addressof (value ()); }

  ढाँचा <typename U>
  T मूल्य_or (U&& default_मूल्य) स्थिरांक &
  {
    जर (is_ok_) परत val_;
    परत std::forward<U> (default_मूल्य);
  }

  ढाँचा <typename U>
  T मूल्य_or (U&& default_मूल्य) &&
  {
    जर (is_ok_) परत std::move (val_);
    परत std::forward<U> (default_मूल्य);
  }

  T मूल्य_or_default () स्थिरांक &
  {
    जर (is_ok_) परत val_;
    परत T ();
  }

  T मूल्य_or_default () &&
  {
    जर (is_ok_) परत std::move (val_);
    परत T ();
  }

  स्थिरांक E& error () स्थिरांक & { प्रतिज्ञा (!is_ok_); परत err_; }
  E& error () & { प्रतिज्ञा (!is_ok_); परत err_; }
  E error () && { प्रतिज्ञा (!is_ok_); परत std::move (err_); }

  तार्किक चालक == (स्थिरांक hb_result_t& o) स्थिरांक
  {
    जर (is_ok_ != o.is_ok_) परत असत्य;
    जर (is_ok_) परत val_ == o.val_;
    परत err_ == o.err_;
  }

  तार्किक चालक != (स्थिरांक hb_result_t& o) स्थिरांक
  {
    परत !(*हे == o);
  }

  ढाँचा <typename U>
  तार्किक चालक == (स्थिरांक hb_result_ok_t<U>& o) स्थिरांक
  {
    परत is_ok_ && val_ == o.value;
  }

  ढाँचा <typename U>
  तार्किक चालक != (स्थिरांक hb_result_ok_t<U>& o) स्थिरांक
  {
    परत !(*हे == o);
  }

  ढाँचा <typename F>
  तार्किक चालक == (स्थिरांक hb_result_err_t<F>& e) स्थिरांक
  {
    परत !is_ok_ && err_ == e.error;
  }

  ढाँचा <typename F>
  तार्किक चालक != (स्थिरांक hb_result_err_t<F>& e) स्थिरांक
  {
    परत !(*हे == e);
  }

  ढाँचा <typename U>
  तार्किक चालक == (स्थिरांक U&) स्थिरांक = delete;

  ढाँचा <typename U>
  तार्किक चालक != (स्थिरांक U&) स्थिरांक = delete;
};

// Partial specialization for void
ढाँचा <typename E>
संरचना HB_NODISCARD_STRUCT hb_result_t<शून्य, E>
{
  खाजगी:
  संघ
  {
    E err_;
  };
  तार्किक is_ok_;

  ढाँचा <typename, typename> मित्र संरचना hb_result_t;

  सार्वजनिक:
  ~hb_result_t ()
  {
    जर (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (सत्य) {}
  hb_result_t (hb_result_ok_t<शून्य>) : is_ok_ (सत्य) {}

  hb_result_t (स्थिरांक hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    जर (!is_ok_)
      नवीन (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    जर (!is_ok_)
      नवीन (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& चालक = (स्थिरांक hb_result_t& o)
  {
    जर (हे == &o) परत *हे;
    जर (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    अन्यथा जर (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = सत्य;
    }
    अन्यथा जर (is_ok_ && !o.is_ok_)
    {
      नवीन (std::addressof (err_)) E (o.err_);
      is_ok_ = असत्य;
    }
    परत *हे;
  }

  hb_result_t& चालक = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    जर (हे == &o) परत *हे;
    जर (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    अन्यथा जर (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = सत्य;
    }
    अन्यथा जर (is_ok_ && !o.is_ok_)
    {
      नवीन (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = असत्य;
    }
    परत *हे;
  }

  ढाँचा <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (असत्य)
  {
    नवीन (std::addressof (err_)) E (std::move (e.error));
  }

  ढाँचा <typename F,
            hb_enable_if ((std::is_constructible<E, स्थिरांक F&>::value))>
  hb_result_t (स्थिरांक hb_result_err_t<F>& e) : is_ok_ (असत्य)
  {
    नवीन (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (असत्य)
  {
    नवीन (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  ढाँचा<typename V>
  स्थिर hb_result_t<शून्य, E> from (स्थिरांक V& fallible, E error_मूल्य) {
    जर (fallible.in_error()) {
      परत Err(error_मूल्य);
    }
    परत Ok();
  }

  तार्किक is_ok () स्थिरांक { परत is_ok_; }
  तार्किक is_err () स्थिरांक { परत !is_ok_; }
  explicit चालक तार्किक () स्थिरांक { परत is_ok (); }

  स्थिरांक E& error () स्थिरांक & { प्रतिज्ञा (!is_ok_); परत err_; }
  E& error () & { प्रतिज्ञा (!is_ok_); परत err_; }
  E error () && { प्रतिज्ञा (!is_ok_); परत std::move (err_); }

  शून्य चालक * () & { परत; }
  शून्य चालक * () स्थिरांक & { परत; }
  शून्य चालक * () && { परत; }

  तार्किक चालक == (स्थिरांक hb_result_t<शून्य, E>& o) स्थिरांक
  {
    परत is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  तार्किक चालक != (स्थिरांक hb_result_t<शून्य, E>& o) स्थिरांक
  {
    परत !(*हे == o);
  }

  तार्किक चालक == (hb_result_ok_t<शून्य>) स्थिरांक
  {
    परत is_ok_;
  }

  तार्किक चालक != (hb_result_ok_t<शून्य>) स्थिरांक
  {
    परत !is_ok_;
  }

  ढाँचा <typename F>
  तार्किक चालक == (स्थिरांक hb_result_err_t<F>& e) स्थिरांक
  {
    परत !is_ok_ && err_ == e.error;
  }

  ढाँचा <typename F>
  तार्किक चालक != (स्थिरांक hb_result_err_t<F>& e) स्थिरांक
  {
    परत !(*हे == e);
  }

  ढाँचा <typename U>
  तार्किक चालक == (स्थिरांक U&) स्थिरांक = delete;

  ढाँचा <typename U>
  तार्किक चालक != (स्थिरांक U&) स्थिरांक = delete;
};

ढाँचा <typename U, typename T, typename E>
स्थिर ओळीत तार्किक चालक == (स्थिरांक hb_result_ok_t<U>& o, स्थिरांक hb_result_t<T, E>& r)
{
  परत r == o;
}
ढाँचा <typename U, typename T, typename E>
स्थिर ओळीत तार्किक चालक != (स्थिरांक hb_result_ok_t<U>& o, स्थिरांक hb_result_t<T, E>& r)
{
  परत r != o;
}
ढाँचा <typename F, typename T, typename E>
स्थिर ओळीत तार्किक चालक == (स्थिरांक hb_result_err_t<F>& e, स्थिरांक hb_result_t<T, E>& r)
{
  परत r == e;
}
ढाँचा <typename F, typename T, typename E>
स्थिर ओळीत तार्किक चालक != (स्थिरांक hb_result_err_t<F>& e, स्थिरांक hb_result_t<T, E>& r)
{
  परत r != e;
}

#ifndef TRY
#define TRY(अभिव्यक्ती)                                                             \
  करा {                                                                        \
    स्वतः _res = (अभिव्यक्ती);                                                       \
    जर (unlikely (!_res.is_ok())) परत Err(std::move(_res).error());        \
  } पर्यंत (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  स्वतः HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  जर (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) परत Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
