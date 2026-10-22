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

#که_نه_تعریف_شوی HB_بټ_سېټ_HH
#تعریف_کړه HB_بټ_سېټ_HH

#ضمن "hb.hh"
#ضمن "hb-bit-page.hh"

بنية hb_بټ_سېټ_t
{
  hb_بټ_سېټ_t () = افتراضي;
  ~hb_بټ_سېټ_t () = افتراضي;

  hb_بټ_سېټ_t (ثابت hb_بټ_سېټ_t& other) : hb_بټ_سېټ_t () { سېټ (other, ریښتیا); }
  hb_بټ_سېټ_t ( hb_بټ_سېټ_t&& other)  noexcept : hb_بټ_سېټ_t () { hb_بدلون (*هذا, other); }
  hb_بټ_سېټ_t& چلوونکی= (ثابت hb_بټ_سېټ_t& other) { سېټ (other); أرجع *هذا; }
  hb_بټ_سېټ_t& چلوونکی= (hb_بټ_سېټ_t&& other)  noexcept { hb_بدلون (*هذا, other); أرجع *هذا; }
  ملګری فراغ swap (hb_بټ_سېټ_t &a, hb_بټ_سېټ_t &b) noexcept
  {
    إذا (likely (!a.successful || !b.successful))
      أرجع;
    hb_بدلون (a.population, b.population);
    hb_بدلون (a.وروستی_پاڼه_lookup, b.وروستی_پاڼه_lookup);
    hb_بدلون (a.پاڼه_map, b.پاڼه_map);
    hb_بدلون (a.پاڼې, b.پاڼې);
  }

  فراغ init ()
  {
    successful = ریښتیا;
    population = 0;
    وروستی_پاڼه_lookup = 0;
    پاڼه_map.init ();
    پاڼې.init ();
  }
  فراغ fini ()
  {
    پاڼه_map.fini ();
    پاڼې.fini ();
  }

  کارول پاڼه_t = hb_بټ_پاڼه_t;
  بنية پاڼه_map_t
  {
    صحيح cmp (ثابت پاڼه_map_t &o) ثابت { أرجع cmp (o.major); }
    صحيح cmp (uint32_t o_major) ثابت { أرجع (صحيح) o_major - (صحيح) major; }

    uint32_t major;
    uint32_t شاخص_2;
  };

  منطقي successful = ریښتیا; /* Allocations successful */
  بدلیدونکی بینښه صحيح population = 0;
  بدلیدونکی hb_atomic_t<بینښه> وروستی_پاڼه_lookup = 0;
  hb_sorted_ویکټور_t<پاڼه_map_t> پاڼه_map;
  hb_ویکټور_t<پاڼه_t> پاڼې;

  فراغ err () { إذا (successful) successful = خطأ; } /* TODO Remove */
  منطقي دننه_error () ثابت { أرجع !successful; }

  منطقي resize (بینښه صحيح شمېر, منطقي clear = ریښتیا, منطقي exact_اندازه = خطأ)
  {
    إذا (unlikely (!successful)) أرجع خطأ;

    إذا (پاڼې.اوږدتیا < شمېر && (بینښه) پاڼې.allocated < شمېر && شمېر <= 2)
      exact_اندازه = ریښتیا; // Most sets are small and local

    إذا (unlikely (!پاڼې.resize_ډک (شمېر, clear, exact_اندازه) ||
	!پاڼه_map.resize_ډک (شمېر, clear, خطأ)))
    {
      پاڼې.resize_ډک (پاڼه_map.اوږدتیا, clear, exact_اندازه);
      successful = خطأ;
      أرجع خطأ;
    }
    أرجع ریښتیا;
  }

  فراغ alloc (بینښه sz)
  {
    sz >>= (پاڼه_t::پاڼه_بټونه_LOG_2 - 1);
    پاڼې.alloc (sz);
    پاڼه_map.alloc (sz);
  }

  hb_بټ_سېټ_t& reset ()
  {
    successful = ریښتیا;
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
    إذا (!پاڼې.اوږدتیا && !population) أرجع;
    پاڼې.clear ();
    پاڼه_map.clear ();
    population = 0;
  }
  منطقي is_empty () ثابت
  {
    بینښه صحيح شمېر = پاڼې.اوږدتیا;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (!پاڼې[i].is_empty ())
	أرجع خطأ;
    أرجع ریښتیا;
  }
  څرګند چلوونکی منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    uint32_t h = 0;
    لكل (خپلکاری &map : پاڼه_map)
    {
      خپلکاری &پاڼه = پاڼې.arrayZ[map.شاخص_2];
      إذا (unlikely (پاڼه.is_empty ())) استمر;
      h = h * 31 + hb_hash (map.major) + hb_hash (پاڼه);
    }
    أرجع h;
  }

  خاص:
  فراغ dirty () { population = UINT_MAX; }
  عام:

  فراغ جمع_کړه (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (unlikely (g == INVALID)) أرجع;
    dirty ();
    پاڼه_t *پاڼه = پاڼه_for (g, ریښتیا); إذا (unlikely (!پاڼه)) أرجع;
    پاڼه->جمع_کړه (g);
  }
  فراغ جمع_کړه_بټونه (hb_codepoint_t g, uint64_t بټونه)
  {
    إذا (unlikely (!successful) || unlikely (!بټونه)) أرجع;
    أكد (!(g & (پاڼه_t::ELT_بټونه - 1)));
    dirty ();
    پاڼه_t *پاڼه = پاڼه_for (g, ریښتیا); إذا (unlikely (!پاڼه)) أرجع;
    پاڼه->جمع_کړه_بټونه (g, بټونه);
  }
  منطقي جمع_کړه_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!successful)) أرجع ریښتیا; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (a > b || a == INVALID || b == INVALID)) أرجع خطأ;
    dirty ();
    بینښه صحيح ma = get_major (a);
    بینښه صحيح mb = get_major (b);
    إذا (ma == mb)
    {
      پاڼه_t *پاڼه = پاڼه_for (a, ریښتیا); إذا (unlikely (!پاڼه)) أرجع خطأ;
      پاڼه->جمع_کړه_range (a, b);
    }
    وإلا
    {
      پاڼه_t *پاڼه = پاڼه_for (a, ریښتیا); إذا (unlikely (!پاڼه)) أرجع خطأ;
      پاڼه->جمع_کړه_range (a, major_پیل (ma + 1) - 1);

      لكل (بینښه صحيح m = ma + 1; m < mb; m++)
      {
	پاڼه = پاڼه_for (major_پیل (m), ریښتیا); إذا (unlikely (!پاڼه)) أرجع خطأ;
	پاڼه->init1 ();
      }

      پاڼه = پاڼه_for (b, ریښتیا); إذا (unlikely (!پاڼه)) أرجع خطأ;
      پاڼه->جمع_کړه_range (major_پیل (mb), b);
    }
    أرجع ریښتیا;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  کينډۍ<ډول_نوم Type>
  ساكن مضمن ثابت Type& Struct_At_دننه_واټن_Unaligned(ثابت فراغ *P, بینښه صحيح دننه_واټن)
  {
#لارښوونه GCC diagnostic push
#لارښوونه GCC diagnostic ignored "-Wcast-align"
    أرجع * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + دننه_واټن);
#لارښوونه GCC diagnostic راوباسه
  }

  کينډۍ <ډول_نوم T>
  فراغ سېټ_array (منطقي v, ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (!شمېر) أرجع;
    dirty ();
    hb_codepoint_t g = *array;
    طالما (شمېر)
    {
      بینښه صحيح m = get_major (g);
      پاڼه_t *پاڼه = پاڼه_for (g, v); إذا (unlikely (v && !پاڼه)) أرجع;
      بینښه صحيح پیل = major_پیل (m);
      بینښه صحيح پای_ټکی = major_پیل (m + 1);
      افعل
      {
        إذا (g != INVALID && (v || پاڼه)) /* The v check is to optimize out the page check if v is true. */
	  پاڼه->سېټ (g, v);

	array = &Struct_At_دننه_واټن_Unaligned<T> (array, stride);
	شمېر--;
      }
      طالما (شمېر && (g = *array, پیل <= g && g < پای_ټکی));
    }
  }

  کينډۍ <ډول_نوم T>
  فراغ جمع_کړه_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  { سېټ_array (ریښتیا, array, شمېر, stride); }
  کينډۍ <ډول_نوم T>
  فراغ جمع_کړه_array (ثابت hb_array_t<ثابت T>& arr) { جمع_کړه_array (&arr, arr.len ()); }

  کينډۍ <ډول_نوم T>
  فراغ del_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  { سېټ_array (خطأ, array, شمېر, stride); }
  کينډۍ <ډول_نوم T>
  فراغ del_array (ثابت hb_array_t<ثابت T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  کينډۍ <ډول_نوم T>
  منطقي سېټ_sorted_array (منطقي v, ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع ریښتیا; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (!شمېر)) أرجع ریښتیا;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t وروستی_g = g;
    طالما (شمېر)
    {
      بینښه صحيح m = get_major (g);
      پاڼه_t *پاڼه = پاڼه_for (g, v); إذا (unlikely (v && !پاڼه)) أرجع خطأ;
      بینښه صحيح پای_ټکی = major_پیل (m + 1);
      افعل
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	إذا (g < وروستی_g) أرجع خطأ;
	وروستی_g = g;

        إذا (g != INVALID && (v || پاڼه)) /* The v check is to optimize out the page check if v is true. */
	  پاڼه->سېټ (g, v);

	array = &Struct_At_دننه_واټن_Unaligned<T> (array, stride);
	شمېر--;
      }
      طالما (شمېر && (g = *array, g < پای_ټکی));
    }
    أرجع ریښتیا;
  }

  کينډۍ <ډول_نوم T>
  منطقي جمع_کړه_sorted_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  { أرجع سېټ_sorted_array (ریښتیا, array, شمېر, stride); }
  کينډۍ <ډول_نوم T>
  منطقي جمع_کړه_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جمع_کړه_sorted_array (&arr, arr.len ()); }

  کينډۍ <ډول_نوم T>
  منطقي del_sorted_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  { أرجع سېټ_sorted_array (خطأ, array, شمېر, stride); }
  کينډۍ <ډول_نوم T>
  منطقي del_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع del_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    پاڼه_t *پاڼه = پاڼه_for (g);
    إذا (!پاڼه)
      أرجع;
    dirty ();
    پاڼه->del (g);
  }
  فراغ del_بټونه (hb_codepoint_t g, uint64_t بټونه)
  {
    إذا (unlikely (!successful) || unlikely (!بټونه)) أرجع;
    أكد (!(g & (پاڼه_t::ELT_بټونه - 1)));
    پاڼه_t *پاڼه = پاڼه_for (g);
    إذا (!پاڼه)
      أرجع;
    dirty ();
    پاڼه->del_بټونه (g, بټونه);
  }

  خاص:
  فراغ del_پاڼې (صحيح ds, صحيح de)
  {
    إذا (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_ویکټور_t<بینښه> compact_workspace;
      إذا (unlikely (!allocate_compact_workspace (compact_workspace))) أرجع;

      بینښه صحيح write_شاخص = 0;
      لكل (بینښه صحيح i = 0; i < پاڼه_map.اوږدتیا; i++)
      {
	صحيح m = (صحيح) پاڼه_map.arrayZ[i].major;
	إذا (m < ds || de < m)
	  پاڼه_map.arrayZ[write_شاخص++] = پاڼه_map.arrayZ[i];
      }
      compact (compact_workspace, write_شاخص);
      resize (write_شاخص);
    }
  }


  عام:
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (unlikely (a > b || a == INVALID)) أرجع;
    dirty ();
    بینښه صحيح ma = get_major (a);
    بینښه صحيح mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    صحيح ds = (a == major_پیل (ma))? (صحيح) ma: (صحيح) (ma + 1);
    صحيح de = (b + 1 == major_پیل (mb + 1))? (صحيح) mb: ((صحيح) mb - 1);
    إذا (ds > de || (صحيح) ma < ds)
    {
      پاڼه_t *پاڼه = پاڼه_for (a);
      إذا (پاڼه)
      {
	إذا (ma == mb)
	  پاڼه->del_range (a, b);
	وإلا
	  پاڼه->del_range (a, major_پیل (ma + 1) - 1);
      }
    }
    إذا (de < (صحيح) mb && ma != mb)
    {
      پاڼه_t *پاڼه = پاڼه_for (b);
      إذا (پاڼه)
	پاڼه->del_range (major_پیل (mb), b);
    }
    del_پاڼې (ds, de);
  }

  منطقي get (hb_codepoint_t g) ثابت
  {
    ثابت پاڼه_t *پاڼه = پاڼه_for (g);
    إذا (!پاڼه)
      أرجع خطأ;
    أرجع پاڼه->get (g);
  }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي چلوونکی [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي چلوونکی () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_بټ_سېټ_t& چلوونکی << (hb_codepoint_t v)
  { جمع_کړه (v); أرجع *هذا; }
  hb_بټ_سېټ_t& چلوونکی << (ثابت hb_codepoint_pair_t& range)
  { جمع_کړه_range (range.لومړی, range.second); أرجع *هذا; }

  منطقي intersects (ثابت hb_بټ_سېټ_t &other) ثابت
  {
    بینښه صحيح na = پاڼې.اوږدتیا;
    بینښه صحيح nb = other.پاڼې.اوږدتیا;

    بینښه صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (پاڼه_map.arrayZ[a].major == other.پاڼه_map.arrayZ[b].major)
      {
	إذا (پاڼه_at (a).intersects (other.پاڼه_at (b)))
	  أرجع ریښتیا;
	a++;
	b++;
      }
      وإلا إذا (پاڼه_map.arrayZ[a].major < other.پاڼه_map.arrayZ[b].major)
	a++;
      وإلا
	b++;
    }
    أرجع خطأ;
  }
  منطقي may_intersect (ثابت hb_بټ_سېټ_t &other) ثابت
  { أرجع intersects (other); }

  منطقي intersects (hb_codepoint_t لومړی, hb_codepoint_t وروستی) ثابت
  {
    hb_codepoint_t c = لومړی - 1;
    أرجع next (&c) && c <= وروستی;
  }
  فراغ سېټ (ثابت hb_بټ_سېټ_t &other, منطقي exact_اندازه = خطأ)
  {
    إذا (unlikely (!successful)) أرجع;
    بینښه صحيح شمېر = other.پاڼې.اوږدتیا;
    إذا (unlikely (!resize  (شمېر, خطأ, exact_اندازه)))
      أرجع;
    population = other.population;

    پاڼه_map = other.پاڼه_map;
    پاڼې = other.پاڼې;
  }

  منطقي is_equal (ثابت hb_بټ_سېټ_t &other) ثابت
  {
    إذا (has_population () && other.has_population () &&
	population != other.population)
      أرجع خطأ;

    بینښه صحيح na = پاڼې.اوږدتیا;
    بینښه صحيح nb = other.پاڼې.اوږدتیا;

    بینښه صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (پاڼه_at (a).is_empty ()) { a++; استمر; }
      إذا (other.پاڼه_at (b).is_empty ()) { b++; استمر; }
      إذا (پاڼه_map.arrayZ[a].major != other.پاڼه_map.arrayZ[b].major ||
	  !پاڼه_at (a).is_equal (other.پاڼه_at (b)))
	أرجع خطأ;
      a++;
      b++;
    }
    لكل (; a < na; a++)
      إذا (!پاڼه_at (a).is_empty ()) { أرجع خطأ; }
    لكل (; b < nb; b++)
      إذا (!other.پاڼه_at (b).is_empty ()) { أرجع خطأ; }

    أرجع ریښتیا;
  }

  منطقي is_subset (ثابت hb_بټ_سېټ_t &larger_سېټ) ثابت
  {
    إذا (has_population () && larger_سېټ.has_population () &&
	population > larger_سېټ.population)
      أرجع خطأ;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    طالما (spi < پاڼه_map.اوږدتیا && lpi < larger_سېټ.پاڼه_map.اوږدتیا)
    {
      uint32_t spm = پاڼه_map.arrayZ[spi].major;
      uint32_t lpm = larger_سېټ.پاڼه_map.arrayZ[lpi].major;
      خپلکاری sp = پاڼه_at (spi);

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

      خپلکاری lp = larger_سېټ.پاڼه_at (lpi);
      إذا (!sp.is_subset (lp))
        أرجع خطأ;

      spi++;
      lpi++;
    }

    طالما (spi < پاڼه_map.اوږدتیا)
      إذا (!پاڼه_at (spi++).is_empty ())
        أرجع خطأ;

    أرجع ریښتیا;
  }

  خاص:
  منطقي allocate_compact_workspace (hb_ویکټور_t<بینښه>& workspace)
  {
    إذا (unlikely (!workspace.resize_exact (پاڼې.اوږدتیا)))
    {
      successful = خطأ;
      أرجع خطأ;
    }

    أرجع ریښتیا;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  فراغ compact (hb_ویکټور_t<بینښه>& workspace,
                بینښه صحيح اوږدتیا)
  {
    أكد(workspace.اوږدتیا == پاڼې.اوږدتیا);
    hb_ویکټور_t<بینښه>& old_شاخص_منزل_پاڼه_map_شاخص = workspace;

    hb_fill (old_شاخص_منزل_پاڼه_map_شاخص.writer(), 0xFFFFFFFF);
    لكل (بینښه i = 0; i < اوږدتیا; i++)
      old_شاخص_منزل_پاڼه_map_شاخص[پاڼه_map[i].شاخص_2] =  i;

    compact_پاڼې (old_شاخص_منزل_پاڼه_map_شاخص);
  }
  فراغ compact_پاڼې (ثابت hb_ویکټور_t<بینښه>& old_شاخص_منزل_پاڼه_map_شاخص)
  {
    بینښه صحيح write_شاخص = 0;
    لكل (بینښه صحيح i = 0; i < پاڼې.اوږدتیا; i++)
    {
      إذا (old_شاخص_منزل_پاڼه_map_شاخص[i] == 0xFFFFFFFF) استمر;

      إذا (write_شاخص < i)
	پاڼې[write_شاخص] = پاڼې[i];

      پاڼه_map[old_شاخص_منزل_پاڼه_map_شاخص[i]].شاخص_2 = write_شاخص;
      write_شاخص++;
    }
  }
  عام:

  فراغ process_ (hb_بټ_پاڼه_t::ویکټور_t (*op) (ثابت hb_بټ_پاڼه_t::ویکټور_t &, ثابت hb_بټ_پاڼه_t::ویکټور_t &),
		 منطقي passthru_کیڼ, منطقي passthru_ښی,
		 ثابت hb_بټ_سېټ_t &other,
		 hb_ویکټور_t<بینښه> *workspace = تش_اشاره)
  {
    إذا (unlikely (!successful)) أرجع;

    dirty ();

    بینښه صحيح na = پاڼې.اوږدتیا;
    بینښه صحيح nb = other.پاڼې.اوږدتیا;
    بینښه صحيح بل_پاڼه = na;

    بینښه صحيح شمېر = 0, new_شمېر = 0;
    بینښه صحيح a = 0, b = 0;
    بینښه صحيح write_شاخص = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_ویکټور_t<بینښه> local_workspace;
    hb_ویکټور_t<بینښه> &compact_workspace = workspace ? *workspace : local_workspace;
    إذا (!passthru_کیڼ)
    {
      منطقي allocated = workspace ? compact_workspace.resize (پاڼې.اوږدتیا)
				 : allocate_compact_workspace (compact_workspace);
      إذا (unlikely (!allocated))
      {
	successful = خطأ;
	أرجع;
      }
    }

    لكل (; a < na && b < nb; )
    {
      إذا (پاڼه_map.arrayZ[a].major == other.پاڼه_map.arrayZ[b].major)
      {
	إذا (!passthru_کیڼ)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  إذا (write_شاخص < a)
	    پاڼه_map.arrayZ[write_شاخص] = پاڼه_map.arrayZ[a];
	  write_شاخص++;
	}

	شمېر++;
	a++;
	b++;
      }
      وإلا إذا (پاڼه_map.arrayZ[a].major < other.پاڼه_map.arrayZ[b].major)
      {
	إذا (passthru_کیڼ)
	  شمېر++;
	a++;
      }
      وإلا
      {
	إذا (passthru_ښی)
	  شمېر++;
	b++;
      }
    }
    إذا (passthru_کیڼ)
      شمېر += na - a;
    إذا (passthru_ښی)
      شمېر += nb - b;

    إذا (!passthru_کیڼ)
    {
      na  = write_شاخص;
      بل_پاڼه = write_شاخص;
      compact (compact_workspace, write_شاخص);
    }

    إذا (unlikely (!resize (شمېر)))
      أرجع;

    new_شمېر = شمېر;

    /* Process in-place backward. */
    a = na;
    b = nb;
    لكل (; a && b; )
    {
      إذا (پاڼه_map.arrayZ[a - 1].major == other.پاڼه_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	شمېر--;
	پاڼه_map.arrayZ[شمېر] = پاڼه_map.arrayZ[a];
	پاڼه_at (شمېر).v = op (پاڼه_at (a).v, other.پاڼه_at (b).v);
	پاڼه_at (شمېر).dirty ();
      }
      وإلا إذا (پاڼه_map.arrayZ[a - 1].major > other.پاڼه_map.arrayZ[b - 1].major)
      {
	a--;
	إذا (passthru_کیڼ)
	{
	  شمېر--;
	  پاڼه_map.arrayZ[شمېر] = پاڼه_map.arrayZ[a];
	}
      }
      وإلا
      {
	b--;
	إذا (passthru_ښی)
	{
	  شمېر--;
	  پاڼه_map.arrayZ[شمېر].major = other.پاڼه_map.arrayZ[b].major;
	  پاڼه_map.arrayZ[شمېر].شاخص_2 = بل_پاڼه++;
	  پاڼه_at (شمېر) = other.پاڼه_at (b);
	}
      }
    }
    إذا (passthru_کیڼ)
      طالما (a)
      {
	a--;
	شمېر--;
	پاڼه_map.arrayZ[شمېر] = پاڼه_map.arrayZ[a];
      }
    إذا (passthru_ښی)
      طالما (b)
      {
	b--;
	شمېر--;
	پاڼه_map.arrayZ[شمېر].major = other.پاڼه_map.arrayZ[b].major;
	پاڼه_map.arrayZ[شمېر].شاخص_2 = بل_پاڼه++;
	پاڼه_at (شمېر) = other.پاڼه_at (b);
      }
    أكد (!شمېر);
    resize (new_شمېر);
  }
  کينډۍ <ډول_نوم Op>
  ساكن hb_بټ_پاڼه_t::ویکټور_t
  op_ (ثابت hb_بټ_پاڼه_t::ویکټور_t &a, ثابت hb_بټ_پاڼه_t::ویکټور_t &b)
  { أرجع Op{} (a, b); }
  کينډۍ <ډول_نوم Op>
  فراغ process (ثابت Op& op,
		ثابت hb_بټ_سېټ_t &other,
		hb_ویکټور_t<بینښه> *workspace = تش_اشاره)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  فراغ union_ (ثابت hb_بټ_سېټ_t &other) { process (hb_bitwise_or, other); }
  فراغ intersect (ثابت hb_بټ_سېټ_t &other,
		  hb_ویکټور_t<بینښه> *workspace = تش_اشاره)
  { process (hb_bitwise_and, other, workspace); }
  فراغ کمول (ثابت hb_بټ_سېټ_t &other) { process (hb_bitwise_gt, other); }
  فراغ متناظر_difference (ثابت hb_بټ_سېټ_t &other) { process (hb_bitwise_xor, other); }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      أرجع *codepoint != INVALID;
    }

    ثابت خپلکاری* پاڼه_map_array = پاڼه_map.arrayZ;
    بینښه صحيح major = get_major (*codepoint);
    بینښه صحيح i = وروستی_پاڼه_lookup;

    إذا (unlikely (i >= پاڼه_map.اوږدتیا || پاڼه_map_array[i].major != major))
    {
      پاڼه_map.bfind (major, &i, HB_NOT_وموندل_STORE_CLOSEST);
      إذا (i >= پاڼه_map.اوږدتیا) {
        *codepoint = INVALID;
        أرجع خطأ;
      }
      وروستی_پاڼه_lookup = i;
    }

    ثابت خپلکاری* پاڼې_array = پاڼې.arrayZ;
    ثابت پاڼه_map_t &current = پاڼه_map_array[i];
    إذا (likely (current.major == major))
    {
      إذا (پاڼې_array[current.شاخص_2].next (codepoint))
      {
        *codepoint += current.major * پاڼه_t::پاڼه_بټونه;
        أرجع ریښتیا;
      }
      i++;
    }

    لكل (; i < پاڼه_map.اوږدتیا; i++)
    {
      ثابت پاڼه_map_t &current = پاڼه_map_array[i];
      hb_codepoint_t m = پاڼې_array[current.شاخص_2].get_min ();
      إذا (m != INVALID)
      {
	*codepoint = current.major * پاڼه_t::پاڼه_بټونه + m;
        وروستی_پاڼه_lookup = i;
	أرجع ریښتیا;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي بل_بټونه (hb_codepoint_t *codepoint, uint64_t *بټونه) ثابت
  {
    static_assert (پاڼه_t::ELT_بټونه == 64, "");

    بینښه صحيح i = 0;
    بینښه صحيح elt = 0;
    إذا (likely (*codepoint != INVALID))
    {
      hb_codepoint_t بنسټ = *codepoint & ~پاڼه_t::ELT_ماسک;
      إذا (unlikely (بنسټ >= INVALID - پاڼه_t::ELT_بټونه + 1))
        اذهب بشپړ;
      بنسټ += پاڼه_t::ELT_بټونه;

      بینښه صحيح major = get_major (بنسټ);
      i = وروستی_پاڼه_lookup;
      إذا (unlikely (i >= پاڼه_map.اوږدتیا || پاڼه_map.arrayZ[i].major != major))
      {
	پاڼه_map.bfind (major, &i, HB_NOT_وموندل_STORE_CLOSEST);
	إذا (i >= پاڼه_map.اوږدتیا)
	  اذهب بشپړ;
      }
      إذا (پاڼه_map.arrayZ[i].major == major)
	elt = پاڼه_remainder (بنسټ) / پاڼه_t::ELT_بټونه;
    }

    لكل (; i < پاڼه_map.اوږدتیا; i++, elt = 0)
    {
      ثابت پاڼه_map_t &map = پاڼه_map.arrayZ[i];
      ثابت پاڼه_t &پاڼه = پاڼې.arrayZ[map.شاخص_2];
      لكل (; elt < پاڼه_t::len (); elt++)
	إذا (پاڼه.v[elt])
	{
	  *codepoint = major_پیل (map.major) + elt * پاڼه_t::ELT_بټونه;
	  *بټونه = پاڼه.v[elt];
	  وروستی_پاڼه_lookup = i;
	  أرجع ریښتیا;
	}
    }

  بشپړ:
    *codepoint = INVALID;
    *بټونه = 0;
    أرجع خطأ;
  }
  منطقي پخوانی (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      أرجع *codepoint != INVALID;
    }

    پاڼه_map_t map = {get_major (*codepoint), 0};
    بینښه صحيح i;
    پاڼه_map.bfind (map, &i, HB_NOT_وموندل_STORE_CLOSEST);
    إذا (i < پاڼه_map.اوږدتیا && پاڼه_map.arrayZ[i].major == map.major)
    {
      إذا (پاڼې[پاڼه_map.arrayZ[i].شاخص_2].پخوانی (codepoint))
      {
	*codepoint += پاڼه_map.arrayZ[i].major * پاڼه_t::پاڼه_بټونه;
	أرجع ریښتیا;
      }
    }
    i--;
    لكل (; (صحيح) i >= 0; i--)
    {
      hb_codepoint_t m = پاڼې.arrayZ[پاڼه_map.arrayZ[i].شاخص_2].get_max ();
      إذا (m != INVALID)
      {
	*codepoint = پاڼه_map.arrayZ[i].major * پاڼه_t::پاڼه_بټونه + m;
	أرجع ریښتیا;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي بل_range (hb_codepoint_t *لومړی, hb_codepoint_t *وروستی) ثابت
  {
    hb_codepoint_t i;

    i = *وروستی;
    إذا (!next (&i))
    {
      *وروستی = *لومړی = INVALID;
      أرجع خطأ;
    }

    *وروستی = *لومړی = i;

    ثابت خپلکاری* پاڼه_map_array = پاڼه_map.arrayZ;
    ثابت خپلکاری* پاڼې_array = پاڼې.arrayZ;
    بینښه صحيح major = get_major (*وروستی);
    بینښه صحيح p_شاخص = وروستی_پاڼه_lookup;

    إذا (unlikely (p_شاخص >= پاڼه_map.اوږدتیا || پاڼه_map_array[p_شاخص].major != major))
    {
      پاڼه_map.bfind (major, &p_شاخص, HB_NOT_وموندل_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      أكد(p_شاخص < پاڼه_map.اوږدتیا && پاڼه_map_array[p_شاخص].major == major);
      وروستی_پاڼه_lookup = p_شاخص;
    }

    بینښه صحيح rem = پاڼه_remainder (*وروستی);
    بینښه صحيح elt = rem / پاڼه_t::ELT_بټونه;
    بینښه صحيح bit = rem & پاڼه_t::ELT_ماسک;

    ثابت پاڼه_t *پاڼه = &پاڼې_array[پاڼه_map_array[p_شاخص].شاخص_2];
    بینښه صحيح پرلپسې = hb_ctz (~(پاڼه->v[elt] >> bit));
    *وروستی += پرلپسې - 1;
    إذا (پرلپسې < پاڼه_t::ELT_بټونه - bit)
      أرجع ریښتیا;

    elt++;

    طالما (ریښتیا)
    {
      لكل (; elt < پاڼه_t::len (); elt++)
      {
        uint64_t w = پاڼه->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *وروستی += پاڼه_t::ELT_بټونه;
          استمر;
        }
        uint64_t inv = ~w;
        *وروستی += hb_ctz (inv);
        أرجع ریښتیا;
      }

      p_شاخص++;


      إذا (p_شاخص >= پاڼه_map.اوږدتیا || پاڼه_map_array[p_شاخص].major != major + 1)
        أرجع ریښتیا;

      ثابت خپلکاری& ثبت = پاڼه_map_array[p_شاخص];
      major = ثبت.major;
      وروستی_پاڼه_lookup = p_شاخص;
      پاڼه = &پاڼې_array[ثبت.شاخص_2];
      elt = 0;
    }
  }

  منطقي پخوانی_range (hb_codepoint_t *لومړی, hb_codepoint_t *وروستی) ثابت
  {
    hb_codepoint_t i;

    i = *لومړی;
    إذا (!پخوانی (&i))
    {
      *وروستی = *لومړی = INVALID;
      أرجع خطأ;
    }

    *وروستی = *لومړی = i;

    ثابت خپلکاری* پاڼه_map_array = پاڼه_map.arrayZ;
    ثابت خپلکاری* پاڼې_array = پاڼې.arrayZ;
    بینښه صحيح major = get_major (*لومړی);
    بینښه صحيح p_شاخص = وروستی_پاڼه_lookup;

    إذا (unlikely (p_شاخص >= پاڼه_map.اوږدتیا || پاڼه_map_array[p_شاخص].major != major))
    {
      پاڼه_map.bfind (major, &p_شاخص, HB_NOT_وموندل_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      أكد(p_شاخص < پاڼه_map.اوږدتیا && پاڼه_map_array[p_شاخص].major == major);
      وروستی_پاڼه_lookup = p_شاخص;
    }

    بینښه صحيح rem = پاڼه_remainder (*لومړی);
    صحيح elt = rem / پاڼه_t::ELT_بټونه;
    بینښه صحيح bit = rem & پاڼه_t::ELT_ماسک;

    ثابت پاڼه_t *پاڼه = &پاڼې_array[پاڼه_map_array[p_شاخص].شاخص_2];
    uint64_t inv = ~(پاڼه->v[elt] << (63 - bit));
    بینښه صحيح پرلپسې = hb_بټ_storage (inv) ? (پاڼه_t::ELT_بټونه - hb_بټ_storage (inv)) : پاڼه_t::ELT_بټونه;
    *لومړی -= پرلپسې - 1;
    إذا (پرلپسې < bit + 1)
      أرجع ریښتیا;

    elt--;

    طالما (ریښتیا)
    {
      لكل (; elt >= 0; elt--)
      {
        uint64_t w = پاڼه->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *لومړی -= پاڼه_t::ELT_بټونه;
          استمر;
        }
        uint64_t inv = ~w;
        بینښه صحيح zeros = hb_بټ_storage (inv) ? (پاڼه_t::ELT_بټونه - hb_بټ_storage (inv)) : پاڼه_t::ELT_بټونه;
        *لومړی -= zeros;
        أرجع ریښتیا;
      }


      إذا (p_شاخص == 0 || پاڼه_map_array[p_شاخص - 1].major != major - 1)
        أرجع ریښتیا;

      p_شاخص--;
      ثابت خپلکاری& ثبت = پاڼه_map_array[p_شاخص];

      major = ثبت.major;
      وروستی_پاڼه_lookup = p_شاخص;
      پاڼه = &پاڼې_array[ثبت.شاخص_2];
      elt = پاڼه_t::len () - 1;
    }
  }

  بینښه صحيح بل_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *بهر,
			  بینښه صحيح    اندازه) ثابت
  {
    // By default, start at the first bit of the first page of values.
    بینښه صحيح پیل_پاڼه = 0;
    بینښه صحيح پیل_پاڼه_ارزښت = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت خپلکاری* پاڼه_map_array = پاڼه_map.arrayZ;
      بینښه صحيح major = get_major (codepoint);
      بینښه صحيح i = وروستی_پاڼه_lookup;
      إذا (unlikely (i >= پاڼه_map.اوږدتیا || پاڼه_map_array[i].major != major))
      {
	پاڼه_map.bfind (major, &i, HB_NOT_وموندل_STORE_CLOSEST);
	إذا (i >= پاڼه_map.اوږدتیا)
	  أرجع 0;  // codepoint is greater than our max element.
      }
      پیل_پاڼه = i;
      إذا (پاڼه_map_array[i].major == major)
      {
        پیل_پاڼه_ارزښت = پاڼه_remainder (codepoint + 1);
        إذا (unlikely (پیل_پاڼه_ارزښت == 0))
        {
          // The export-after value was last in the page. Start on next page.
          پیل_پاڼه++;
          پیل_پاڼه_ارزښت = 0;
        }
      }
    }

    بینښه صحيح initial_اندازه = اندازه;
    لكل (بینښه صحيح i = پیل_پاڼه; i < پاڼه_map.اوږدتیا && اندازه; i++)
    {
      uint32_t بنسټ = major_پیل (پاڼه_map.arrayZ[i].major);
      بینښه صحيح n = پاڼې[پاڼه_map.arrayZ[i].شاخص_2].اكتب (بنسټ, پیل_پاڼه_ارزښت, بهر, اندازه);
      بهر += n;
      اندازه -= n;
      پیل_پاڼه_ارزښت = 0;
    }
    أرجع initial_اندازه - اندازه;
  }

  بینښه صحيح بل_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *بهر,
				   بینښه صحيح    اندازه) ثابت
  {
    بینښه صحيح initial_اندازه = اندازه;
    // By default, start at the first bit of the first page of values.
    بینښه صحيح پیل_پاڼه = 0;
    بینښه صحيح پیل_پاڼه_ارزښت = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت خپلکاری* پاڼه_map_array = پاڼه_map.arrayZ;
      بینښه صحيح major = get_major (codepoint);
      بینښه صحيح i = وروستی_پاڼه_lookup;
      إذا (unlikely (i >= پاڼه_map.اوږدتیا || پاڼه_map_array[i].major != major))
      {
        پاڼه_map.bfind(major, &i, HB_NOT_وموندل_STORE_CLOSEST);
        إذا (unlikely (i >= پاڼه_map.اوږدتیا))
        {
          // codepoint is greater than our max element.
          طالما (++codepoint != INVALID && اندازه)
          {
            *بهر++ = codepoint;
            اندازه--;
          }
          أرجع initial_اندازه - اندازه;
        }
      }
      پیل_پاڼه = i;
      إذا (پاڼه_map_array[i].major == major)
      {
        پیل_پاڼه_ارزښت = پاڼه_remainder (codepoint + 1);
        إذا (unlikely (پیل_پاڼه_ارزښت == 0))
        {
          // The export-after value was last in the page. Start on next page.
          پیل_پاڼه++;
          پیل_پاڼه_ارزښت = 0;
        }
      }
    }

    hb_codepoint_t بل_ارزښت = codepoint + 1;
    لكل (بینښه صحيح i=پیل_پاڼه; i<پاڼه_map.اوږدتیا && اندازه; i++)
    {
      uint32_t بنسټ = major_پیل (پاڼه_map.arrayZ[i].major);
      بینښه صحيح n = پاڼې[پاڼه_map.arrayZ[i].شاخص_2].write_inverted (بنسټ, پیل_پاڼه_ارزښت, بهر, اندازه, &بل_ارزښت);
      بهر += n;
      اندازه -= n;
      پیل_پاڼه_ارزښت = 0;
    }
    طالما (بل_ارزښت < HB_سېټ_ارزښت_INVALID && اندازه) {
      *بهر++ = بل_ارزښت++;
      اندازه--;
    }
    أرجع initial_اندازه - اندازه;
  }

  منطقي has_population () ثابت { أرجع population != UINT_MAX; }
  بینښه صحيح get_population () ثابت
  {
    إذا (has_population ())
      أرجع population;

    بینښه صحيح راوباسه = 0;
    بینښه صحيح شمېر = پاڼې.اوږدتیا;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      راوباسه += پاڼې[i].get_population ();

    population = راوباسه;
    أرجع راوباسه;
  }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    إذا (has_population ())
    {
      إذا (population != 1) أرجع خطأ;
      *codepoint = get_min ();
      أرجع ریښتیا;
    }

    hb_codepoint_t singleton = INVALID;
    لكل (ثابت خپلکاری &map : پاڼه_map)
    {
      ثابت خپلکاری &پاڼه = پاڼې.arrayZ[map.شاخص_2];
      لكل (بینښه i = 0; i < پاڼه_t::len (); i++)
      {
	پاڼه_t::elt_t بټونه = پاڼه.v[i];
	إذا (!بټونه) استمر;
	إذا (singleton != INVALID || (بټونه & (بټونه - 1)))
	  أرجع خطأ;
	singleton = map.major * پاڼه_t::پاڼه_بټونه +
		    i * پاڼه_t::ELT_بټونه + hb_ctz (بټونه);
      }
    }

    إذا (singleton == INVALID)
    {
      population = 0;
      أرجع خطأ;
    }
    population = 1;
    *codepoint = singleton;
    أرجع ریښتیا;
  }
  hb_codepoint_t get_min () ثابت
  {
    بینښه شمېر = پاڼې.اوږدتیا;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      ثابت خپلکاری& map = پاڼه_map.arrayZ[i];
      ثابت خپلکاری& پاڼه = پاڼې.arrayZ[map.شاخص_2];

      إذا (!پاڼه.is_empty ())
	أرجع map.major * پاڼه_t::پاڼه_بټونه + پاڼه.get_min ();
    }
    أرجع INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    بینښه شمېر = پاڼې.اوږدتیا;
    لكل (نښهدار i = شمېر - 1; i >= 0; i--)
    {
      ثابت خپلکاری& map = پاڼه_map.arrayZ[(بینښه) i];
      ثابت خپلکاری& پاڼه = پاڼې.arrayZ[map.شاخص_2];

      إذا (!پاڼه.is_empty ())
	أرجع map.major * پاڼه_t::پاڼه_بټونه + پاڼه.get_max ();
    }
    أرجع INVALID;
  }

  ساكن constexpr hb_codepoint_t INVALID = پاڼه_t::INVALID;

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = ریښتیا;
    ساكن constexpr منطقي has_fast_len = ریښتیا;
    iter_t (ثابت hb_بټ_سېټ_t &s_ = Null (hb_بټ_سېټ_t),
	    منطقي init = ریښتیا) : s (&s_), v (INVALID), l(0)
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
    فراغ __prev__ () { s->پخوانی (&v); }
    بینښه __len__ () ثابت { أرجع l; }
    iter_t پای_ټکی () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي چلوونکی != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_بټ_سېټ_t *s;
    hb_codepoint_t v;
    بینښه l;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  چلوونکی iter_t () ثابت { أرجع iter (); }

  محمي:

  پاڼه_t *پاڼه_for (hb_codepoint_t g, منطقي insert = خطأ)
  {
    بینښه major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    بینښه i = وروستی_پاڼه_lookup;
    إذا (likely (i < پاڼه_map.اوږدتیا))
    {
      خپلکاری &cached_پاڼه = پاڼه_map.arrayZ[i];
      إذا (cached_پاڼه.major == major)
	أرجع &پاڼې.arrayZ[cached_پاڼه.شاخص_2];
    }

    پاڼه_map_t map = {major, پاڼې.اوږدتیا};
    إذا (!پاڼه_map.bfind (map, &i, HB_NOT_وموندل_STORE_CLOSEST))
    {
      إذا (!insert)
        أرجع تش_اشاره;

      إذا (unlikely (!resize (پاڼې.اوږدتیا + 1)))
	أرجع تش_اشاره;

      پاڼې.arrayZ[map.شاخص_2].init0 ();
      memmove (پاڼه_map.arrayZ + i + 1,
	       پاڼه_map.arrayZ + i,
	       (پاڼه_map.اوږدتیا - 1 - i) * پاڼه_map.توکی_اندازه);
      پاڼه_map.arrayZ[i] = map;
    }

    وروستی_پاڼه_lookup = i;
    أرجع &پاڼې.arrayZ[پاڼه_map.arrayZ[i].شاخص_2];
  }
  ثابت پاڼه_t *پاڼه_for (hb_codepoint_t g) ثابت
  {
    بینښه major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    بینښه i = وروستی_پاڼه_lookup;
    إذا (likely (i < پاڼه_map.اوږدتیا))
    {
      خپلکاری &cached_پاڼه = پاڼه_map.arrayZ[i];
      إذا (cached_پاڼه.major == major)
	أرجع &پاڼې.arrayZ[cached_پاڼه.شاخص_2];
    }

    پاڼه_map_t key = {major};
    إذا (!پاڼه_map.bfind (key, &i))
      أرجع تش_اشاره;

    وروستی_پاڼه_lookup = i;
    أرجع &پاڼې.arrayZ[پاڼه_map.arrayZ[i].شاخص_2];
  }
  پاڼه_t &پاڼه_at (بینښه صحيح i)
  {
    أكد (i < پاڼه_map.اوږدتیا);
    أرجع پاڼې.arrayZ[پاڼه_map.arrayZ[i].شاخص_2];
  }
  ثابت پاڼه_t &پاڼه_at (بینښه صحيح i) ثابت
  {
    أكد (i < پاڼه_map.اوږدتیا);
    أرجع پاڼې.arrayZ[پاڼه_map.arrayZ[i].شاخص_2];
  }
  بینښه صحيح get_major (hb_codepoint_t g) ثابت { أرجع g >> پاڼه_t::پاڼه_بټونه_LOG_2; }
  بینښه صحيح پاڼه_remainder (hb_codepoint_t g) ثابت { أرجع g & پاڼه_t::پاڼه_BITMASK; }
  hb_codepoint_t major_پیل (بینښه صحيح major) ثابت { أرجع major << پاڼه_t::پاڼه_بټونه_LOG_2; }
};


#د_که_پای /* HB_BIT_SET_HH */
