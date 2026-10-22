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

#جيڪڏهن_وصف_نه_ٿيل HB_ترجيح_قطار_HH
#وصف_ڪريو HB_ترجيح_قطار_HH

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
سانچو <قسم_نالو K>
بنية hb_ترجيح_قطار_t
{
 عام:
  تعريفنوع hb_pair_t<K, بنانشان> شي_t;

 خاص:
  hb_ويڪٽر_t<شي_t> ڍير_2;

 عام:

  hb_ترجيح_قطار_t () = افتراضي;
  hb_ترجيح_قطار_t (hb_ويڪٽر_t<شي_t>&& other) : ڍير_2 (std::move (other))
  {
    // Heapify the vector.
    لكل (صحيح i = (ڍير_2.ڊيگهائي / 2) - 1; i >= 0; i--)
      bubble_هيٺ_ڏانهن (i);
  }

  فراغ reset () { ڍير_2.clear (); }

  منطقي اندريون_error () ثابت { أرجع ڍير_2.اندريون_error (); }

  منطقي alloc (بنانشان سائيز)
  { أرجع ڍير_2.alloc (سائيز); }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  فراغ insert (K ترجيح, بنانشان value)
  {
    ڍير_2.push (شي_t (ترجيح, value));
    إذا (unlikely (ڍير_2.اندريون_error ())) أرجع;
    bubble_مٿي (ڍير_2.ڊيگهائي - 1);
  }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  شي_t ڪڍو_minimum ()
  {
    أكد (!is_empty ());

    شي_t نتيجو = ڍير_2.arrayZ[0];

    ڍير_2.arrayZ[0] = ڍير_2.arrayZ[ڍير_2.ڊيگهائي - 1];
    ڍير_2.resize (ڍير_2.ڊيگهائي - 1);

    إذا (!is_empty ())
      bubble_هيٺ_ڏانهن (0);

    أرجع نتيجو;
  }

  ثابت شي_t& minimum ()
  {
    أرجع ڍير_2[0];
  }

  منطقي is_empty () ثابت { أرجع ڍير_2.ڊيگهائي == 0; }
  واضح عامل منطقي () ثابت { أرجع !is_empty (); }
  بنانشان صحيح get_population () ثابت { أرجع ڍير_2.ڊيگهائي; }

  /* Sink interface. */
  hb_ترجيح_قطار_t& عامل << (شي_t شي)
  { insert (شي.پهريون, شي.second); أرجع *هذا; }

 خاص:

  ساكن constexpr بنانشان والد (بنانشان اشاريو_2)
  {
    أرجع (اشاريو_2 - 1) / 2;
  }

  ساكن constexpr بنانشان کاٻو_ٻار (بنانشان اشاريو_2)
  {
    أرجع 2 * اشاريو_2 + 1;
  }

  ساكن constexpr بنانشان ساڄو_ٻار (بنانشان اشاريو_2)
  {
    أرجع 2 * اشاريو_2 + 2;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_هيٺ_ڏانهن (بنانشان اشاريو_2)
  {
    repeat:
    أكد (اشاريو_2 < ڍير_2.ڊيگهائي);

    بنانشان کاٻي = کاٻو_ٻار (اشاريو_2);
    بنانشان ساڄي = ساڄو_ٻار (اشاريو_2);

    منطقي has_کاٻو = کاٻي < ڍير_2.ڊيگهائي;
    إذا (!has_کاٻو)
      // If there's no left, then there's also no right.
      أرجع;

    منطقي has_ساڄو = ساڄي < ڍير_2.ڊيگهائي;
    إذا (ڍير_2.arrayZ[اشاريو_2].پهريون <= ڍير_2.arrayZ[کاٻي].پهريون
        && (!has_ساڄو || ڍير_2.arrayZ[اشاريو_2].پهريون <= ڍير_2.arrayZ[ساڄي].پهريون))
      أرجع;

    بنانشان ٻار;
    إذا (!has_ساڄو || ڍير_2.arrayZ[کاٻي].پهريون < ڍير_2.arrayZ[ساڄي].پهريون)
      ٻار = کاٻي;
    وإلا
      ٻار = ساڄي;

    swap (اشاريو_2, ٻار);
    اشاريو_2 = ٻار;
    اذهب repeat;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_مٿي (بنانشان اشاريو_2)
  {
    repeat:
    أكد (اشاريو_2 < ڍير_2.ڊيگهائي);

    إذا (اشاريو_2 == 0) أرجع;

    بنانشان والد_اشاريو = والد (اشاريو_2);
    إذا (ڍير_2.arrayZ[والد_اشاريو].پهريون <= ڍير_2.arrayZ[اشاريو_2].پهريون)
      أرجع;

    swap (اشاريو_2, والد_اشاريو);
    اشاريو_2 = والد_اشاريو;
    اذهب repeat;
  }

  فراغ swap (بنانشان a, بنانشان b) noexcept
  {
    أكد (a < ڍير_2.ڊيگهائي);
    أكد (b < ڍير_2.ڊيگهائي);
    hb_مٽا_سٽا (ڍير_2.arrayZ[a], ڍير_2.arrayZ[b]);
  }
};

#جيڪڏهن_پورو /* HB_PRIORITY_QUEUE_HH */
