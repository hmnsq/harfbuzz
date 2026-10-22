/*
 * Copyright © 2020  Google, Inc.
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

#اگر_تعریف_نشده HB_اولویت_صف_HH
#تعریف_کن HB_اولویت_صف_HH

#ضمن "hb.hh"
#ضمن "hb-vector.hh"

/*
 * hb_priority_queue_t
 *
 * Priority queue implemented as a binary heap. Supports extract minimum
 * and insert operations.
 *
 * The priority queue is implemented as a binary heap, which is a complete
 * binary tree. The root of the tree is the minimum element. The heap
 * property is that the priority of a node is less than or equal to the
 * priority of its children. The heap is stored in an array, with the
 * children of node i stored at indices 2i + 1 and 2i + 2.
 */
قالب <نام_نوع K>
بنية hb_اولویت_صف_t
{
 عام:
  تعريفنوع hb_pair_t<K, بیعلامت> قلم_t;

 خاص:
  hb_وکتور_t<قلم_t> انباشته;

 عام:

  hb_اولویت_صف_t () = افتراضي;
  hb_اولویت_صف_t (hb_وکتور_t<قلم_t>&& other) : انباشته (std::move (other))
  {
    // Heapify the vector.
    لكل (صحيح i = (انباشته.درازی / 2) - 1; i >= 0; i--)
      bubble_پایین (i);
  }

  فراغ reset () { انباشته.clear (); }

  منطقي درون_error () ثابت { أرجع انباشته.درون_error (); }

  منطقي alloc (بیعلامت اندازه)
  { أرجع انباشته.alloc (اندازه); }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  فراغ insert (K اولویت, بیعلامت value)
  {
    انباشته.push (قلم_t (اولویت, value));
    إذا (unlikely (انباشته.درون_error ())) أرجع;
    bubble_به_بالا (انباشته.درازی - 1);
  }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  قلم_t برداشت_minimum ()
  {
    أكد (!is_empty ());

    قلم_t نتیجه = انباشته.arrayZ[0];

    انباشته.arrayZ[0] = انباشته.arrayZ[انباشته.درازی - 1];
    انباشته.resize (انباشته.درازی - 1);

    إذا (!is_empty ())
      bubble_پایین (0);

    أرجع نتیجه;
  }

  ثابت قلم_t& minimum ()
  {
    أرجع انباشته[0];
  }

  منطقي is_empty () ثابت { أرجع انباشته.درازی == 0; }
  صریح عملگر منطقي () ثابت { أرجع !is_empty (); }
  بیعلامت صحيح get_population () ثابت { أرجع انباشته.درازی; }

  /* Sink interface. */
  hb_اولویت_صف_t& عملگر << (قلم_t قلم)
  { insert (قلم.اولی, قلم.second); أرجع *هذا; }

 خاص:

  ساكن constexpr بیعلامت والد (بیعلامت شاخص_2)
  {
    أرجع (شاخص_2 - 1) / 2;
  }

  ساكن constexpr بیعلامت چپ_فرزند (بیعلامت شاخص_2)
  {
    أرجع 2 * شاخص_2 + 1;
  }

  ساكن constexpr بیعلامت راست_فرزند (بیعلامت شاخص_2)
  {
    أرجع 2 * شاخص_2 + 2;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_پایین (بیعلامت شاخص_2)
  {
    repeat:
    أكد (شاخص_2 < انباشته.درازی);

    بیعلامت چپ = چپ_فرزند (شاخص_2);
    بیعلامت راست = راست_فرزند (شاخص_2);

    منطقي has_چپ = چپ < انباشته.درازی;
    إذا (!has_چپ)
      // If there's no left, then there's also no right.
      أرجع;

    منطقي has_راست = راست < انباشته.درازی;
    إذا (انباشته.arrayZ[شاخص_2].اولی <= انباشته.arrayZ[چپ].اولی
        && (!has_راست || انباشته.arrayZ[شاخص_2].اولی <= انباشته.arrayZ[راست].اولی))
      أرجع;

    بیعلامت فرزند;
    إذا (!has_راست || انباشته.arrayZ[چپ].اولی < انباشته.arrayZ[راست].اولی)
      فرزند = چپ;
    وإلا
      فرزند = راست;

    swap (شاخص_2, فرزند);
    شاخص_2 = فرزند;
    اذهب repeat;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_به_بالا (بیعلامت شاخص_2)
  {
    repeat:
    أكد (شاخص_2 < انباشته.درازی);

    إذا (شاخص_2 == 0) أرجع;

    بیعلامت والد_شاخص = والد (شاخص_2);
    إذا (انباشته.arrayZ[والد_شاخص].اولی <= انباشته.arrayZ[شاخص_2].اولی)
      أرجع;

    swap (شاخص_2, والد_شاخص);
    شاخص_2 = والد_شاخص;
    اذهب repeat;
  }

  فراغ swap (بیعلامت a, بیعلامت b) noexcept
  {
    أكد (a < انباشته.درازی);
    أكد (b < انباشته.درازی);
    hb_جابجا (انباشته.arrayZ[a], انباشته.arrayZ[b]);
  }
};

#پایان_اگر /* HB_PRIORITY_QUEUE_HH */
