/*
 * Copyright © 2012,2017  Google, Inc.
 * Copyright © 2021 Behdad Esfahbod
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
 * Google Author(s): Behdad Esfahbod
 */

#جيڪڏهن_وصف_نه_ٿيل HB_ٻٽو_انگ_سيٽ_HH
#وصف_ڪريو HB_ٻٽو_انگ_سيٽ_HH

#ضمن "hb.hh"
#ضمن "hb-bit-page.hh"

بنية hb_ٻٽو_انگ_سيٽ_t
{
  hb_ٻٽو_انگ_سيٽ_t () = افتراضي;
  ~hb_ٻٽو_انگ_سيٽ_t () = افتراضي;

  hb_ٻٽو_انگ_سيٽ_t (ثابت hb_ٻٽو_انگ_سيٽ_t& other) : hb_ٻٽو_انگ_سيٽ_t () { سيٽ (other, سچ); }
  hb_ٻٽو_انگ_سيٽ_t ( hb_ٻٽو_انگ_سيٽ_t&& other)  noexcept : hb_ٻٽو_انگ_سيٽ_t () { hb_مٽا_سٽا (*هذا, other); }
  hb_ٻٽو_انگ_سيٽ_t& عامل= (ثابت hb_ٻٽو_انگ_سيٽ_t& other) { سيٽ (other); أرجع *هذا; }
  hb_ٻٽو_انگ_سيٽ_t& عامل= (hb_ٻٽو_انگ_سيٽ_t&& other)  noexcept { hb_مٽا_سٽا (*هذا, other); أرجع *هذا; }
  دوست فراغ swap (hb_ٻٽو_انگ_سيٽ_t &a, hb_ٻٽو_انگ_سيٽ_t &b) noexcept
  {
    إذا (likely (!a.successful || !b.successful))
      أرجع;
    hb_مٽا_سٽا (a.population, b.population);
    hb_مٽا_سٽا (a.آخري_صفحو_lookup, b.آخري_صفحو_lookup);
    hb_مٽا_سٽا (a.صفحو_map, b.صفحو_map);
    hb_مٽا_سٽا (a.صفحا, b.صفحا);
  }

  فراغ init ()
  {
    successful = سچ;
    population = 0;
    آخري_صفحو_lookup = 0;
    صفحو_map.init ();
    صفحا.init ();
  }
  فراغ fini ()
  {
    صفحو_map.fini ();
    صفحا.fini ();
  }

  استعمال صفحو_t = hb_ٻٽو_انگ_صفحو_t;
  بنية صفحو_map_t
  {
    صحيح cmp (ثابت صفحو_map_t &o) ثابت { أرجع cmp (o.major); }
    صحيح cmp (uint32_t o_major) ثابت { أرجع (صحيح) o_major - (صحيح) major; }

    uint32_t major;
    uint32_t اشاريو_2;
  };

  منطقي successful = سچ; /* Allocations successful */
  بدلجندڙ بنانشان صحيح population = 0;
  بدلجندڙ hb_atomic_t<بنانشان> آخري_صفحو_lookup = 0;
  hb_sorted_ويڪٽر_t<صفحو_map_t> صفحو_map;
  hb_ويڪٽر_t<صفحو_t> صفحا;

  فراغ err () { إذا (successful) successful = خطأ; } /* TODO Remove */
  منطقي اندريون_error () ثابت { أرجع !successful; }

  منطقي resize (بنانشان صحيح ڳڻپ, منطقي clear = سچ, منطقي exact_سائيز = خطأ)
  {
    إذا (unlikely (!successful)) أرجع خطأ;

    إذا (صفحا.ڊيگهائي < ڳڻپ && (بنانشان) صفحا.allocated < ڳڻپ && ڳڻپ <= 2)
      exact_سائيز = سچ; // Most sets are small and local

    إذا (unlikely (!صفحا.resize_ڀريل (ڳڻپ, clear, exact_سائيز) ||
	!صفحو_map.resize_ڀريل (ڳڻپ, clear, خطأ)))
    {
      صفحا.resize_ڀريل (صفحو_map.ڊيگهائي, clear, exact_سائيز);
      successful = خطأ;
      أرجع خطأ;
    }
    أرجع سچ;
  }

  فراغ alloc (بنانشان sz)
  {
    sz >>= (صفحو_t::صفحو_بٽ_LOG_2 - 1);
    صفحا.alloc (sz);
    صفحو_map.alloc (sz);
  }

  hb_ٻٽو_انگ_سيٽ_t& reset ()
  {
    successful = سچ;
    clear ();
    أرجع *هذا;
  }

  فراغ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    إذا (!صفحا.ڊيگهائي && !population) أرجع;
    صفحا.clear ();
    صفحو_map.clear ();
    population = 0;
  }
  منطقي is_empty () ثابت
  {
    بنانشان صحيح ڳڻپ = صفحا.ڊيگهائي;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (!صفحا[i].is_empty ())
	أرجع خطأ;
    أرجع سچ;
  }
  واضح عامل منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    uint32_t h = 0;
    لكل (خودڪار &map : صفحو_map)
    {
      خودڪار &صفحو = صفحا.arrayZ[map.اشاريو_2];
      إذا (unlikely (صفحو.is_empty ())) استمر;
      h = h * 31 + hb_hash (map.major) + hb_hash (صفحو);
    }
    أرجع h;
  }

  خاص:
  فراغ dirty () { population = UINT_MAX; }
  عام:

  فراغ جوڙيو (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (unlikely (g == INVALID)) أرجع;
    dirty ();
    صفحو_t *صفحو = صفحو_for (g, سچ); إذا (unlikely (!صفحو)) أرجع;
    صفحو->جوڙيو (g);
  }
  فراغ جوڙيو_بٽ (hb_codepoint_t g, uint64_t بٽ)
  {
    إذا (unlikely (!successful) || unlikely (!بٽ)) أرجع;
    أكد (!(g & (صفحو_t::ELT_بٽ - 1)));
    dirty ();
    صفحو_t *صفحو = صفحو_for (g, سچ); إذا (unlikely (!صفحو)) أرجع;
    صفحو->جوڙيو_بٽ (g, بٽ);
  }
  منطقي جوڙيو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!successful)) أرجع سچ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (a > b || a == INVALID || b == INVALID)) أرجع خطأ;
    dirty ();
    بنانشان صحيح ma = get_major (a);
    بنانشان صحيح mb = get_major (b);
    إذا (ma == mb)
    {
      صفحو_t *صفحو = صفحو_for (a, سچ); إذا (unlikely (!صفحو)) أرجع خطأ;
      صفحو->جوڙيو_range (a, b);
    }
    وإلا
    {
      صفحو_t *صفحو = صفحو_for (a, سچ); إذا (unlikely (!صفحو)) أرجع خطأ;
      صفحو->جوڙيو_range (a, major_شروعات (ma + 1) - 1);

      لكل (بنانشان صحيح m = ma + 1; m < mb; m++)
      {
	صفحو = صفحو_for (major_شروعات (m), سچ); إذا (unlikely (!صفحو)) أرجع خطأ;
	صفحو->init1 ();
      }

      صفحو = صفحو_for (b, سچ); إذا (unlikely (!صفحو)) أرجع خطأ;
      صفحو->جوڙيو_range (major_شروعات (mb), b);
    }
    أرجع سچ;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  سانچو<قسم_نالو Type>
  ساكن مضمن ثابت Type& Struct_At_وٿي_Unaligned(ثابت فراغ *P, بنانشان صحيح وٿي)
  {
#هدايت GCC diagnostic push
#هدايت GCC diagnostic ignored "-Wcast-align"
    أرجع * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + وٿي);
#هدايت GCC diagnostic ڪڍو
  }

  سانچو <قسم_نالو T>
  فراغ سيٽ_array (منطقي v, ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (!ڳڻپ) أرجع;
    dirty ();
    hb_codepoint_t g = *array;
    طالما (ڳڻپ)
    {
      بنانشان صحيح m = get_major (g);
      صفحو_t *صفحو = صفحو_for (g, v); إذا (unlikely (v && !صفحو)) أرجع;
      بنانشان صحيح شروعات = major_شروعات (m);
      بنانشان صحيح پڇاڙي = major_شروعات (m + 1);
      افعل
      {
        إذا (g != INVALID && (v || صفحو)) /* The v check is to optimize out the page check if v is true. */
	  صفحو->سيٽ (g, v);

	array = &Struct_At_وٿي_Unaligned<T> (array, stride);
	ڳڻپ--;
      }
      طالما (ڳڻپ && (g = *array, شروعات <= g && g < پڇاڙي));
    }
  }

  سانچو <قسم_نالو T>
  فراغ جوڙيو_array (ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  { سيٽ_array (سچ, array, ڳڻپ, stride); }
  سانچو <قسم_نالو T>
  فراغ جوڙيو_array (ثابت hb_array_t<ثابت T>& arr) { جوڙيو_array (&arr, arr.len ()); }

  سانچو <قسم_نالو T>
  فراغ del_array (ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  { سيٽ_array (خطأ, array, ڳڻپ, stride); }
  سانچو <قسم_نالو T>
  فراغ del_array (ثابت hb_array_t<ثابت T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچو <قسم_نالو T>
  منطقي سيٽ_sorted_array (منطقي v, ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع سچ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (!ڳڻپ)) أرجع سچ;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t آخري_g = g;
    طالما (ڳڻپ)
    {
      بنانشان صحيح m = get_major (g);
      صفحو_t *صفحو = صفحو_for (g, v); إذا (unlikely (v && !صفحو)) أرجع خطأ;
      بنانشان صحيح پڇاڙي = major_شروعات (m + 1);
      افعل
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	إذا (g < آخري_g) أرجع خطأ;
	آخري_g = g;

        إذا (g != INVALID && (v || صفحو)) /* The v check is to optimize out the page check if v is true. */
	  صفحو->سيٽ (g, v);

	array = &Struct_At_وٿي_Unaligned<T> (array, stride);
	ڳڻپ--;
      }
      طالما (ڳڻپ && (g = *array, g < پڇاڙي));
    }
    أرجع سچ;
  }

  سانچو <قسم_نالو T>
  منطقي جوڙيو_sorted_array (ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  { أرجع سيٽ_sorted_array (سچ, array, ڳڻپ, stride); }
  سانچو <قسم_نالو T>
  منطقي جوڙيو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جوڙيو_sorted_array (&arr, arr.len ()); }

  سانچو <قسم_نالو T>
  منطقي del_sorted_array (ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  { أرجع سيٽ_sorted_array (خطأ, array, ڳڻپ, stride); }
  سانچو <قسم_نالو T>
  منطقي del_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع del_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    صفحو_t *صفحو = صفحو_for (g);
    إذا (!صفحو)
      أرجع;
    dirty ();
    صفحو->del (g);
  }
  فراغ del_بٽ (hb_codepoint_t g, uint64_t بٽ)
  {
    إذا (unlikely (!successful) || unlikely (!بٽ)) أرجع;
    أكد (!(g & (صفحو_t::ELT_بٽ - 1)));
    صفحو_t *صفحو = صفحو_for (g);
    إذا (!صفحو)
      أرجع;
    dirty ();
    صفحو->del_بٽ (g, بٽ);
  }

  خاص:
  فراغ del_صفحا (صحيح ds, صحيح de)
  {
    إذا (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_ويڪٽر_t<بنانشان> compact_workspace;
      إذا (unlikely (!allocate_compact_workspace (compact_workspace))) أرجع;

      بنانشان صحيح write_اشاريو = 0;
      لكل (بنانشان صحيح i = 0; i < صفحو_map.ڊيگهائي; i++)
      {
	صحيح m = (صحيح) صفحو_map.arrayZ[i].major;
	إذا (m < ds || de < m)
	  صفحو_map.arrayZ[write_اشاريو++] = صفحو_map.arrayZ[i];
      }
      compact (compact_workspace, write_اشاريو);
      resize (write_اشاريو);
    }
  }


  عام:
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (unlikely (a > b || a == INVALID)) أرجع;
    dirty ();
    بنانشان صحيح ma = get_major (a);
    بنانشان صحيح mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    صحيح ds = (a == major_شروعات (ma))? (صحيح) ma: (صحيح) (ma + 1);
    صحيح de = (b + 1 == major_شروعات (mb + 1))? (صحيح) mb: ((صحيح) mb - 1);
    إذا (ds > de || (صحيح) ma < ds)
    {
      صفحو_t *صفحو = صفحو_for (a);
      إذا (صفحو)
      {
	إذا (ma == mb)
	  صفحو->del_range (a, b);
	وإلا
	  صفحو->del_range (a, major_شروعات (ma + 1) - 1);
      }
    }
    إذا (de < (صحيح) mb && ma != mb)
    {
      صفحو_t *صفحو = صفحو_for (b);
      إذا (صفحو)
	صفحو->del_range (major_شروعات (mb), b);
    }
    del_صفحا (ds, de);
  }

  منطقي get (hb_codepoint_t g) ثابت
  {
    ثابت صفحو_t *صفحو = صفحو_for (g);
    إذا (!صفحو)
      أرجع خطأ;
    أرجع صفحو->get (g);
  }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي عامل [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي عامل () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_ٻٽو_انگ_سيٽ_t& عامل << (hb_codepoint_t v)
  { جوڙيو (v); أرجع *هذا; }
  hb_ٻٽو_انگ_سيٽ_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { جوڙيو_range (range.پهريون, range.second); أرجع *هذا; }

  منطقي intersects (ثابت hb_ٻٽو_انگ_سيٽ_t &other) ثابت
  {
    بنانشان صحيح na = صفحا.ڊيگهائي;
    بنانشان صحيح nb = other.صفحا.ڊيگهائي;

    بنانشان صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (صفحو_map.arrayZ[a].major == other.صفحو_map.arrayZ[b].major)
      {
	إذا (صفحو_at (a).intersects (other.صفحو_at (b)))
	  أرجع سچ;
	a++;
	b++;
      }
      وإلا إذا (صفحو_map.arrayZ[a].major < other.صفحو_map.arrayZ[b].major)
	a++;
      وإلا
	b++;
    }
    أرجع خطأ;
  }
  منطقي may_intersect (ثابت hb_ٻٽو_انگ_سيٽ_t &other) ثابت
  { أرجع intersects (other); }

  منطقي intersects (hb_codepoint_t پهريون, hb_codepoint_t آخري) ثابت
  {
    hb_codepoint_t c = پهريون - 1;
    أرجع next (&c) && c <= آخري;
  }
  فراغ سيٽ (ثابت hb_ٻٽو_انگ_سيٽ_t &other, منطقي exact_سائيز = خطأ)
  {
    إذا (unlikely (!successful)) أرجع;
    بنانشان صحيح ڳڻپ = other.صفحا.ڊيگهائي;
    إذا (unlikely (!resize  (ڳڻپ, خطأ, exact_سائيز)))
      أرجع;
    population = other.population;

    صفحو_map = other.صفحو_map;
    صفحا = other.صفحا;
  }

  منطقي is_equal (ثابت hb_ٻٽو_انگ_سيٽ_t &other) ثابت
  {
    إذا (has_population () && other.has_population () &&
	population != other.population)
      أرجع خطأ;

    بنانشان صحيح na = صفحا.ڊيگهائي;
    بنانشان صحيح nb = other.صفحا.ڊيگهائي;

    بنانشان صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (صفحو_at (a).is_empty ()) { a++; استمر; }
      إذا (other.صفحو_at (b).is_empty ()) { b++; استمر; }
      إذا (صفحو_map.arrayZ[a].major != other.صفحو_map.arrayZ[b].major ||
	  !صفحو_at (a).is_equal (other.صفحو_at (b)))
	أرجع خطأ;
      a++;
      b++;
    }
    لكل (; a < na; a++)
      إذا (!صفحو_at (a).is_empty ()) { أرجع خطأ; }
    لكل (; b < nb; b++)
      إذا (!other.صفحو_at (b).is_empty ()) { أرجع خطأ; }

    أرجع سچ;
  }

  منطقي is_subset (ثابت hb_ٻٽو_انگ_سيٽ_t &larger_سيٽ) ثابت
  {
    إذا (has_population () && larger_سيٽ.has_population () &&
	population > larger_سيٽ.population)
      أرجع خطأ;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    طالما (spi < صفحو_map.ڊيگهائي && lpi < larger_سيٽ.صفحو_map.ڊيگهائي)
    {
      uint32_t spm = صفحو_map.arrayZ[spi].major;
      uint32_t lpm = larger_سيٽ.صفحو_map.arrayZ[lpi].major;
      خودڪار sp = صفحو_at (spi);

      إذا (spm < lpm) {
        إذا (!sp.is_empty ())
          أرجع خطأ;
        spi++;
        استمر;
      }

      إذا (lpm < spm) {
        lpi++;
        استمر;
      }

      خودڪار lp = larger_سيٽ.صفحو_at (lpi);
      إذا (!sp.is_subset (lp))
        أرجع خطأ;

      spi++;
      lpi++;
    }

    طالما (spi < صفحو_map.ڊيگهائي)
      إذا (!صفحو_at (spi++).is_empty ())
        أرجع خطأ;

    أرجع سچ;
  }

  خاص:
  منطقي allocate_compact_workspace (hb_ويڪٽر_t<بنانشان>& workspace)
  {
    إذا (unlikely (!workspace.resize_exact (صفحا.ڊيگهائي)))
    {
      successful = خطأ;
      أرجع خطأ;
    }

    أرجع سچ;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  فراغ compact (hb_ويڪٽر_t<بنانشان>& workspace,
                بنانشان صحيح ڊيگهائي)
  {
    أكد(workspace.ڊيگهائي == صفحا.ڊيگهائي);
    hb_ويڪٽر_t<بنانشان>& old_اشاريو_منزل_صفحو_map_اشاريو = workspace;

    hb_fill (old_اشاريو_منزل_صفحو_map_اشاريو.writer(), 0xFFFFFFFF);
    لكل (بنانشان i = 0; i < ڊيگهائي; i++)
      old_اشاريو_منزل_صفحو_map_اشاريو[صفحو_map[i].اشاريو_2] =  i;

    compact_صفحا (old_اشاريو_منزل_صفحو_map_اشاريو);
  }
  فراغ compact_صفحا (ثابت hb_ويڪٽر_t<بنانشان>& old_اشاريو_منزل_صفحو_map_اشاريو)
  {
    بنانشان صحيح write_اشاريو = 0;
    لكل (بنانشان صحيح i = 0; i < صفحا.ڊيگهائي; i++)
    {
      إذا (old_اشاريو_منزل_صفحو_map_اشاريو[i] == 0xFFFFFFFF) استمر;

      إذا (write_اشاريو < i)
	صفحا[write_اشاريو] = صفحا[i];

      صفحو_map[old_اشاريو_منزل_صفحو_map_اشاريو[i]].اشاريو_2 = write_اشاريو;
      write_اشاريو++;
    }
  }
  عام:

  فراغ process_ (hb_ٻٽو_انگ_صفحو_t::ويڪٽر_t (*op) (ثابت hb_ٻٽو_انگ_صفحو_t::ويڪٽر_t &, ثابت hb_ٻٽو_انگ_صفحو_t::ويڪٽر_t &),
		 منطقي passthru_کاٻو, منطقي passthru_ساڄو,
		 ثابت hb_ٻٽو_انگ_سيٽ_t &other,
		 hb_ويڪٽر_t<بنانشان> *workspace = خالي_اشارو)
  {
    إذا (unlikely (!successful)) أرجع;

    dirty ();

    بنانشان صحيح na = صفحا.ڊيگهائي;
    بنانشان صحيح nb = other.صفحا.ڊيگهائي;
    بنانشان صحيح ايندڙ_صفحو = na;

    بنانشان صحيح ڳڻپ = 0, new_ڳڻپ = 0;
    بنانشان صحيح a = 0, b = 0;
    بنانشان صحيح write_اشاريو = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_ويڪٽر_t<بنانشان> local_workspace;
    hb_ويڪٽر_t<بنانشان> &compact_workspace = workspace ? *workspace : local_workspace;
    إذا (!passthru_کاٻو)
    {
      منطقي allocated = workspace ? compact_workspace.resize (صفحا.ڊيگهائي)
				 : allocate_compact_workspace (compact_workspace);
      إذا (unlikely (!allocated))
      {
	successful = خطأ;
	أرجع;
      }
    }

    لكل (; a < na && b < nb; )
    {
      إذا (صفحو_map.arrayZ[a].major == other.صفحو_map.arrayZ[b].major)
      {
	إذا (!passthru_کاٻو)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  إذا (write_اشاريو < a)
	    صفحو_map.arrayZ[write_اشاريو] = صفحو_map.arrayZ[a];
	  write_اشاريو++;
	}

	ڳڻپ++;
	a++;
	b++;
      }
      وإلا إذا (صفحو_map.arrayZ[a].major < other.صفحو_map.arrayZ[b].major)
      {
	إذا (passthru_کاٻو)
	  ڳڻپ++;
	a++;
      }
      وإلا
      {
	إذا (passthru_ساڄو)
	  ڳڻپ++;
	b++;
      }
    }
    إذا (passthru_کاٻو)
      ڳڻپ += na - a;
    إذا (passthru_ساڄو)
      ڳڻپ += nb - b;

    إذا (!passthru_کاٻو)
    {
      na  = write_اشاريو;
      ايندڙ_صفحو = write_اشاريو;
      compact (compact_workspace, write_اشاريو);
    }

    إذا (unlikely (!resize (ڳڻپ)))
      أرجع;

    new_ڳڻپ = ڳڻپ;

    /* Process in-place backward. */
    a = na;
    b = nb;
    لكل (; a && b; )
    {
      إذا (صفحو_map.arrayZ[a - 1].major == other.صفحو_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	ڳڻپ--;
	صفحو_map.arrayZ[ڳڻپ] = صفحو_map.arrayZ[a];
	صفحو_at (ڳڻپ).v = op (صفحو_at (a).v, other.صفحو_at (b).v);
	صفحو_at (ڳڻپ).dirty ();
      }
      وإلا إذا (صفحو_map.arrayZ[a - 1].major > other.صفحو_map.arrayZ[b - 1].major)
      {
	a--;
	إذا (passthru_کاٻو)
	{
	  ڳڻپ--;
	  صفحو_map.arrayZ[ڳڻپ] = صفحو_map.arrayZ[a];
	}
      }
      وإلا
      {
	b--;
	إذا (passthru_ساڄو)
	{
	  ڳڻپ--;
	  صفحو_map.arrayZ[ڳڻپ].major = other.صفحو_map.arrayZ[b].major;
	  صفحو_map.arrayZ[ڳڻپ].اشاريو_2 = ايندڙ_صفحو++;
	  صفحو_at (ڳڻپ) = other.صفحو_at (b);
	}
      }
    }
    إذا (passthru_کاٻو)
      طالما (a)
      {
	a--;
	ڳڻپ--;
	صفحو_map.arrayZ[ڳڻپ] = صفحو_map.arrayZ[a];
      }
    إذا (passthru_ساڄو)
      طالما (b)
      {
	b--;
	ڳڻپ--;
	صفحو_map.arrayZ[ڳڻپ].major = other.صفحو_map.arrayZ[b].major;
	صفحو_map.arrayZ[ڳڻپ].اشاريو_2 = ايندڙ_صفحو++;
	صفحو_at (ڳڻپ) = other.صفحو_at (b);
      }
    أكد (!ڳڻپ);
    resize (new_ڳڻپ);
  }
  سانچو <قسم_نالو Op>
  ساكن hb_ٻٽو_انگ_صفحو_t::ويڪٽر_t
  op_ (ثابت hb_ٻٽو_انگ_صفحو_t::ويڪٽر_t &a, ثابت hb_ٻٽو_انگ_صفحو_t::ويڪٽر_t &b)
  { أرجع Op{} (a, b); }
  سانچو <قسم_نالو Op>
  فراغ process (ثابت Op& op,
		ثابت hb_ٻٽو_انگ_سيٽ_t &other,
		hb_ويڪٽر_t<بنانشان> *workspace = خالي_اشارو)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  فراغ union_ (ثابت hb_ٻٽو_انگ_سيٽ_t &other) { process (hb_bitwise_or, other); }
  فراغ intersect (ثابت hb_ٻٽو_انگ_سيٽ_t &other,
		  hb_ويڪٽر_t<بنانشان> *workspace = خالي_اشارو)
  { process (hb_bitwise_and, other, workspace); }
  فراغ ڪٽيو (ثابت hb_ٻٽو_انگ_سيٽ_t &other) { process (hb_bitwise_gt, other); }
  فراغ متناظر_difference (ثابت hb_ٻٽو_انگ_سيٽ_t &other) { process (hb_bitwise_xor, other); }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      أرجع *codepoint != INVALID;
    }

    ثابت خودڪار* صفحو_map_array = صفحو_map.arrayZ;
    بنانشان صحيح major = get_major (*codepoint);
    بنانشان صحيح i = آخري_صفحو_lookup;

    إذا (unlikely (i >= صفحو_map.ڊيگهائي || صفحو_map_array[i].major != major))
    {
      صفحو_map.bfind (major, &i, HB_NOT_مليو_STORE_CLOSEST);
      إذا (i >= صفحو_map.ڊيگهائي) {
        *codepoint = INVALID;
        أرجع خطأ;
      }
      آخري_صفحو_lookup = i;
    }

    ثابت خودڪار* صفحا_array = صفحا.arrayZ;
    ثابت صفحو_map_t &current = صفحو_map_array[i];
    إذا (likely (current.major == major))
    {
      إذا (صفحا_array[current.اشاريو_2].next (codepoint))
      {
        *codepoint += current.major * صفحو_t::صفحو_بٽ;
        أرجع سچ;
      }
      i++;
    }

    لكل (; i < صفحو_map.ڊيگهائي; i++)
    {
      ثابت صفحو_map_t &current = صفحو_map_array[i];
      hb_codepoint_t m = صفحا_array[current.اشاريو_2].get_min ();
      إذا (m != INVALID)
      {
	*codepoint = current.major * صفحو_t::صفحو_بٽ + m;
        آخري_صفحو_lookup = i;
	أرجع سچ;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي ايندڙ_بٽ (hb_codepoint_t *codepoint, uint64_t *بٽ) ثابت
  {
    static_assert (صفحو_t::ELT_بٽ == 64, "");

    بنانشان صحيح i = 0;
    بنانشان صحيح elt = 0;
    إذا (likely (*codepoint != INVALID))
    {
      hb_codepoint_t بنياد = *codepoint & ~صفحو_t::ELT_نقاب;
      إذا (unlikely (بنياد >= INVALID - صفحو_t::ELT_بٽ + 1))
        اذهب پورو;
      بنياد += صفحو_t::ELT_بٽ;

      بنانشان صحيح major = get_major (بنياد);
      i = آخري_صفحو_lookup;
      إذا (unlikely (i >= صفحو_map.ڊيگهائي || صفحو_map.arrayZ[i].major != major))
      {
	صفحو_map.bfind (major, &i, HB_NOT_مليو_STORE_CLOSEST);
	إذا (i >= صفحو_map.ڊيگهائي)
	  اذهب پورو;
      }
      إذا (صفحو_map.arrayZ[i].major == major)
	elt = صفحو_remainder (بنياد) / صفحو_t::ELT_بٽ;
    }

    لكل (; i < صفحو_map.ڊيگهائي; i++, elt = 0)
    {
      ثابت صفحو_map_t &map = صفحو_map.arrayZ[i];
      ثابت صفحو_t &صفحو = صفحا.arrayZ[map.اشاريو_2];
      لكل (; elt < صفحو_t::len (); elt++)
	إذا (صفحو.v[elt])
	{
	  *codepoint = major_شروعات (map.major) + elt * صفحو_t::ELT_بٽ;
	  *بٽ = صفحو.v[elt];
	  آخري_صفحو_lookup = i;
	  أرجع سچ;
	}
    }

  پورو:
    *codepoint = INVALID;
    *بٽ = 0;
    أرجع خطأ;
  }
  منطقي اڳيون (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      أرجع *codepoint != INVALID;
    }

    صفحو_map_t map = {get_major (*codepoint), 0};
    بنانشان صحيح i;
    صفحو_map.bfind (map, &i, HB_NOT_مليو_STORE_CLOSEST);
    إذا (i < صفحو_map.ڊيگهائي && صفحو_map.arrayZ[i].major == map.major)
    {
      إذا (صفحا[صفحو_map.arrayZ[i].اشاريو_2].اڳيون (codepoint))
      {
	*codepoint += صفحو_map.arrayZ[i].major * صفحو_t::صفحو_بٽ;
	أرجع سچ;
      }
    }
    i--;
    لكل (; (صحيح) i >= 0; i--)
    {
      hb_codepoint_t m = صفحا.arrayZ[صفحو_map.arrayZ[i].اشاريو_2].get_max ();
      إذا (m != INVALID)
      {
	*codepoint = صفحو_map.arrayZ[i].major * صفحو_t::صفحو_بٽ + m;
	أرجع سچ;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي ايندڙ_range (hb_codepoint_t *پهريون, hb_codepoint_t *آخري) ثابت
  {
    hb_codepoint_t i;

    i = *آخري;
    إذا (!next (&i))
    {
      *آخري = *پهريون = INVALID;
      أرجع خطأ;
    }

    *آخري = *پهريون = i;

    ثابت خودڪار* صفحو_map_array = صفحو_map.arrayZ;
    ثابت خودڪار* صفحا_array = صفحا.arrayZ;
    بنانشان صحيح major = get_major (*آخري);
    بنانشان صحيح p_اشاريو = آخري_صفحو_lookup;

    إذا (unlikely (p_اشاريو >= صفحو_map.ڊيگهائي || صفحو_map_array[p_اشاريو].major != major))
    {
      صفحو_map.bfind (major, &p_اشاريو, HB_NOT_مليو_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      أكد(p_اشاريو < صفحو_map.ڊيگهائي && صفحو_map_array[p_اشاريو].major == major);
      آخري_صفحو_lookup = p_اشاريو;
    }

    بنانشان صحيح rem = صفحو_remainder (*آخري);
    بنانشان صحيح elt = rem / صفحو_t::ELT_بٽ;
    بنانشان صحيح bit = rem & صفحو_t::ELT_نقاب;

    ثابت صفحو_t *صفحو = &صفحا_array[صفحو_map_array[p_اشاريو].اشاريو_2];
    بنانشان صحيح لاڳيتو = hb_ctz (~(صفحو->v[elt] >> bit));
    *آخري += لاڳيتو - 1;
    إذا (لاڳيتو < صفحو_t::ELT_بٽ - bit)
      أرجع سچ;

    elt++;

    طالما (سچ)
    {
      لكل (; elt < صفحو_t::len (); elt++)
      {
        uint64_t w = صفحو->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *آخري += صفحو_t::ELT_بٽ;
          استمر;
        }
        uint64_t inv = ~w;
        *آخري += hb_ctz (inv);
        أرجع سچ;
      }

      p_اشاريو++;


      إذا (p_اشاريو >= صفحو_map.ڊيگهائي || صفحو_map_array[p_اشاريو].major != major + 1)
        أرجع سچ;

      ثابت خودڪار& داخلا = صفحو_map_array[p_اشاريو];
      major = داخلا.major;
      آخري_صفحو_lookup = p_اشاريو;
      صفحو = &صفحا_array[داخلا.اشاريو_2];
      elt = 0;
    }
  }

  منطقي اڳيون_range (hb_codepoint_t *پهريون, hb_codepoint_t *آخري) ثابت
  {
    hb_codepoint_t i;

    i = *پهريون;
    إذا (!اڳيون (&i))
    {
      *آخري = *پهريون = INVALID;
      أرجع خطأ;
    }

    *آخري = *پهريون = i;

    ثابت خودڪار* صفحو_map_array = صفحو_map.arrayZ;
    ثابت خودڪار* صفحا_array = صفحا.arrayZ;
    بنانشان صحيح major = get_major (*پهريون);
    بنانشان صحيح p_اشاريو = آخري_صفحو_lookup;

    إذا (unlikely (p_اشاريو >= صفحو_map.ڊيگهائي || صفحو_map_array[p_اشاريو].major != major))
    {
      صفحو_map.bfind (major, &p_اشاريو, HB_NOT_مليو_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      أكد(p_اشاريو < صفحو_map.ڊيگهائي && صفحو_map_array[p_اشاريو].major == major);
      آخري_صفحو_lookup = p_اشاريو;
    }

    بنانشان صحيح rem = صفحو_remainder (*پهريون);
    صحيح elt = rem / صفحو_t::ELT_بٽ;
    بنانشان صحيح bit = rem & صفحو_t::ELT_نقاب;

    ثابت صفحو_t *صفحو = &صفحا_array[صفحو_map_array[p_اشاريو].اشاريو_2];
    uint64_t inv = ~(صفحو->v[elt] << (63 - bit));
    بنانشان صحيح لاڳيتو = hb_ٻٽو_انگ_storage (inv) ? (صفحو_t::ELT_بٽ - hb_ٻٽو_انگ_storage (inv)) : صفحو_t::ELT_بٽ;
    *پهريون -= لاڳيتو - 1;
    إذا (لاڳيتو < bit + 1)
      أرجع سچ;

    elt--;

    طالما (سچ)
    {
      لكل (; elt >= 0; elt--)
      {
        uint64_t w = صفحو->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *پهريون -= صفحو_t::ELT_بٽ;
          استمر;
        }
        uint64_t inv = ~w;
        بنانشان صحيح zeros = hb_ٻٽو_انگ_storage (inv) ? (صفحو_t::ELT_بٽ - hb_ٻٽو_انگ_storage (inv)) : صفحو_t::ELT_بٽ;
        *پهريون -= zeros;
        أرجع سچ;
      }


      إذا (p_اشاريو == 0 || صفحو_map_array[p_اشاريو - 1].major != major - 1)
        أرجع سچ;

      p_اشاريو--;
      ثابت خودڪار& داخلا = صفحو_map_array[p_اشاريو];

      major = داخلا.major;
      آخري_صفحو_lookup = p_اشاريو;
      صفحو = &صفحا_array[داخلا.اشاريو_2];
      elt = صفحو_t::len () - 1;
    }
  }

  بنانشان صحيح ايندڙ_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *ٻاهريون,
			  بنانشان صحيح    سائيز) ثابت
  {
    // By default, start at the first bit of the first page of values.
    بنانشان صحيح شروعات_صفحو = 0;
    بنانشان صحيح شروعات_صفحو_قدر = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت خودڪار* صفحو_map_array = صفحو_map.arrayZ;
      بنانشان صحيح major = get_major (codepoint);
      بنانشان صحيح i = آخري_صفحو_lookup;
      إذا (unlikely (i >= صفحو_map.ڊيگهائي || صفحو_map_array[i].major != major))
      {
	صفحو_map.bfind (major, &i, HB_NOT_مليو_STORE_CLOSEST);
	إذا (i >= صفحو_map.ڊيگهائي)
	  أرجع 0;  // codepoint is greater than our max element.
      }
      شروعات_صفحو = i;
      إذا (صفحو_map_array[i].major == major)
      {
        شروعات_صفحو_قدر = صفحو_remainder (codepoint + 1);
        إذا (unlikely (شروعات_صفحو_قدر == 0))
        {
          // The export-after value was last in the page. Start on next page.
          شروعات_صفحو++;
          شروعات_صفحو_قدر = 0;
        }
      }
    }

    بنانشان صحيح initial_سائيز = سائيز;
    لكل (بنانشان صحيح i = شروعات_صفحو; i < صفحو_map.ڊيگهائي && سائيز; i++)
    {
      uint32_t بنياد = major_شروعات (صفحو_map.arrayZ[i].major);
      بنانشان صحيح n = صفحا[صفحو_map.arrayZ[i].اشاريو_2].اكتب (بنياد, شروعات_صفحو_قدر, ٻاهريون, سائيز);
      ٻاهريون += n;
      سائيز -= n;
      شروعات_صفحو_قدر = 0;
    }
    أرجع initial_سائيز - سائيز;
  }

  بنانشان صحيح ايندڙ_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *ٻاهريون,
				   بنانشان صحيح    سائيز) ثابت
  {
    بنانشان صحيح initial_سائيز = سائيز;
    // By default, start at the first bit of the first page of values.
    بنانشان صحيح شروعات_صفحو = 0;
    بنانشان صحيح شروعات_صفحو_قدر = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت خودڪار* صفحو_map_array = صفحو_map.arrayZ;
      بنانشان صحيح major = get_major (codepoint);
      بنانشان صحيح i = آخري_صفحو_lookup;
      إذا (unlikely (i >= صفحو_map.ڊيگهائي || صفحو_map_array[i].major != major))
      {
        صفحو_map.bfind(major, &i, HB_NOT_مليو_STORE_CLOSEST);
        إذا (unlikely (i >= صفحو_map.ڊيگهائي))
        {
          // codepoint is greater than our max element.
          طالما (++codepoint != INVALID && سائيز)
          {
            *ٻاهريون++ = codepoint;
            سائيز--;
          }
          أرجع initial_سائيز - سائيز;
        }
      }
      شروعات_صفحو = i;
      إذا (صفحو_map_array[i].major == major)
      {
        شروعات_صفحو_قدر = صفحو_remainder (codepoint + 1);
        إذا (unlikely (شروعات_صفحو_قدر == 0))
        {
          // The export-after value was last in the page. Start on next page.
          شروعات_صفحو++;
          شروعات_صفحو_قدر = 0;
        }
      }
    }

    hb_codepoint_t ايندڙ_قدر = codepoint + 1;
    لكل (بنانشان صحيح i=شروعات_صفحو; i<صفحو_map.ڊيگهائي && سائيز; i++)
    {
      uint32_t بنياد = major_شروعات (صفحو_map.arrayZ[i].major);
      بنانشان صحيح n = صفحا[صفحو_map.arrayZ[i].اشاريو_2].write_inverted (بنياد, شروعات_صفحو_قدر, ٻاهريون, سائيز, &ايندڙ_قدر);
      ٻاهريون += n;
      سائيز -= n;
      شروعات_صفحو_قدر = 0;
    }
    طالما (ايندڙ_قدر < HB_سيٽ_قدر_INVALID && سائيز) {
      *ٻاهريون++ = ايندڙ_قدر++;
      سائيز--;
    }
    أرجع initial_سائيز - سائيز;
  }

  منطقي has_population () ثابت { أرجع population != UINT_MAX; }
  بنانشان صحيح get_population () ثابت
  {
    إذا (has_population ())
      أرجع population;

    بنانشان صحيح ڪڍو = 0;
    بنانشان صحيح ڳڻپ = صفحا.ڊيگهائي;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      ڪڍو += صفحا[i].get_population ();

    population = ڪڍو;
    أرجع ڪڍو;
  }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    إذا (has_population ())
    {
      إذا (population != 1) أرجع خطأ;
      *codepoint = get_min ();
      أرجع سچ;
    }

    hb_codepoint_t singleton = INVALID;
    لكل (ثابت خودڪار &map : صفحو_map)
    {
      ثابت خودڪار &صفحو = صفحا.arrayZ[map.اشاريو_2];
      لكل (بنانشان i = 0; i < صفحو_t::len (); i++)
      {
	صفحو_t::elt_t بٽ = صفحو.v[i];
	إذا (!بٽ) استمر;
	إذا (singleton != INVALID || (بٽ & (بٽ - 1)))
	  أرجع خطأ;
	singleton = map.major * صفحو_t::صفحو_بٽ +
		    i * صفحو_t::ELT_بٽ + hb_ctz (بٽ);
      }
    }

    إذا (singleton == INVALID)
    {
      population = 0;
      أرجع خطأ;
    }
    population = 1;
    *codepoint = singleton;
    أرجع سچ;
  }
  hb_codepoint_t get_min () ثابت
  {
    بنانشان ڳڻپ = صفحا.ڊيگهائي;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      ثابت خودڪار& map = صفحو_map.arrayZ[i];
      ثابت خودڪار& صفحو = صفحا.arrayZ[map.اشاريو_2];

      إذا (!صفحو.is_empty ())
	أرجع map.major * صفحو_t::صفحو_بٽ + صفحو.get_min ();
    }
    أرجع INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    بنانشان ڳڻپ = صفحا.ڊيگهائي;
    لكل (نشانوارو i = ڳڻپ - 1; i >= 0; i--)
    {
      ثابت خودڪار& map = صفحو_map.arrayZ[(بنانشان) i];
      ثابت خودڪار& صفحو = صفحا.arrayZ[map.اشاريو_2];

      إذا (!صفحو.is_empty ())
	أرجع map.major * صفحو_t::صفحو_بٽ + صفحو.get_max ();
    }
    أرجع INVALID;
  }

  ساكن constexpr hb_codepoint_t INVALID = صفحو_t::INVALID;

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = سچ;
    ساكن constexpr منطقي has_fast_len = سچ;
    iter_t (ثابت hb_ٻٽو_انگ_سيٽ_t &s_ = Null (hb_ٻٽو_انگ_سيٽ_t),
	    منطقي init = سچ) : s (&s_), v (INVALID), l(0)
    {
      إذا (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { أرجع v; }
    منطقي __more__ () ثابت { أرجع v != INVALID; }
    فراغ __next__ () { s->next (&v); إذا (l) l--; }
    فراغ __prev__ () { s->اڳيون (&v); }
    بنانشان __len__ () ثابت { أرجع l; }
    iter_t پڇاڙي () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_ٻٽو_انگ_سيٽ_t *s;
    hb_codepoint_t v;
    بنانشان l;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  عامل iter_t () ثابت { أرجع iter (); }

  محمي:

  صفحو_t *صفحو_for (hb_codepoint_t g, منطقي insert = خطأ)
  {
    بنانشان major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    بنانشان i = آخري_صفحو_lookup;
    إذا (likely (i < صفحو_map.ڊيگهائي))
    {
      خودڪار &cached_صفحو = صفحو_map.arrayZ[i];
      إذا (cached_صفحو.major == major)
	أرجع &صفحا.arrayZ[cached_صفحو.اشاريو_2];
    }

    صفحو_map_t map = {major, صفحا.ڊيگهائي};
    إذا (!صفحو_map.bfind (map, &i, HB_NOT_مليو_STORE_CLOSEST))
    {
      إذا (!insert)
        أرجع خالي_اشارو;

      إذا (unlikely (!resize (صفحا.ڊيگهائي + 1)))
	أرجع خالي_اشارو;

      صفحا.arrayZ[map.اشاريو_2].init0 ();
      memmove (صفحو_map.arrayZ + i + 1,
	       صفحو_map.arrayZ + i,
	       (صفحو_map.ڊيگهائي - 1 - i) * صفحو_map.شي_سائيز);
      صفحو_map.arrayZ[i] = map;
    }

    آخري_صفحو_lookup = i;
    أرجع &صفحا.arrayZ[صفحو_map.arrayZ[i].اشاريو_2];
  }
  ثابت صفحو_t *صفحو_for (hb_codepoint_t g) ثابت
  {
    بنانشان major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    بنانشان i = آخري_صفحو_lookup;
    إذا (likely (i < صفحو_map.ڊيگهائي))
    {
      خودڪار &cached_صفحو = صفحو_map.arrayZ[i];
      إذا (cached_صفحو.major == major)
	أرجع &صفحا.arrayZ[cached_صفحو.اشاريو_2];
    }

    صفحو_map_t key = {major};
    إذا (!صفحو_map.bfind (key, &i))
      أرجع خالي_اشارو;

    آخري_صفحو_lookup = i;
    أرجع &صفحا.arrayZ[صفحو_map.arrayZ[i].اشاريو_2];
  }
  صفحو_t &صفحو_at (بنانشان صحيح i)
  {
    أكد (i < صفحو_map.ڊيگهائي);
    أرجع صفحا.arrayZ[صفحو_map.arrayZ[i].اشاريو_2];
  }
  ثابت صفحو_t &صفحو_at (بنانشان صحيح i) ثابت
  {
    أكد (i < صفحو_map.ڊيگهائي);
    أرجع صفحا.arrayZ[صفحو_map.arrayZ[i].اشاريو_2];
  }
  بنانشان صحيح get_major (hb_codepoint_t g) ثابت { أرجع g >> صفحو_t::صفحو_بٽ_LOG_2; }
  بنانشان صحيح صفحو_remainder (hb_codepoint_t g) ثابت { أرجع g & صفحو_t::صفحو_BITMASK; }
  hb_codepoint_t major_شروعات (بنانشان صحيح major) ثابت { أرجع major << صفحو_t::صفحو_بٽ_LOG_2; }
};


#جيڪڏهن_پورو /* HB_BIT_SET_HH */
