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

#اگر_تعریف_نشده HB_بیت_مجموعه_HH
#تعریف_کن HB_بیت_مجموعه_HH

#ضمن "hb.hh"
#ضمن "hb-bit-page.hh"

بنية hb_بیت_مجموعه_t
{
  hb_بیت_مجموعه_t () = افتراضي;
  ~hb_بیت_مجموعه_t () = افتراضي;

  hb_بیت_مجموعه_t (ثابت hb_بیت_مجموعه_t& other) : hb_بیت_مجموعه_t () { مجموعه (other, درست); }
  hb_بیت_مجموعه_t ( hb_بیت_مجموعه_t&& other)  noexcept : hb_بیت_مجموعه_t () { hb_جابجا (*هذا, other); }
  hb_بیت_مجموعه_t& عملگر= (ثابت hb_بیت_مجموعه_t& other) { مجموعه (other); أرجع *هذا; }
  hb_بیت_مجموعه_t& عملگر= (hb_بیت_مجموعه_t&& other)  noexcept { hb_جابجا (*هذا, other); أرجع *هذا; }
  دوست فراغ swap (hb_بیت_مجموعه_t &a, hb_بیت_مجموعه_t &b) noexcept
  {
    إذا (likely (!a.successful || !b.successful))
      أرجع;
    hb_جابجا (a.population, b.population);
    hb_جابجا (a.آخرین_صفحه_lookup, b.آخرین_صفحه_lookup);
    hb_جابجا (a.صفحه_map, b.صفحه_map);
    hb_جابجا (a.صفحات, b.صفحات);
  }

  فراغ init ()
  {
    successful = درست;
    population = 0;
    آخرین_صفحه_lookup = 0;
    صفحه_map.init ();
    صفحات.init ();
  }
  فراغ fini ()
  {
    صفحه_map.fini ();
    صفحات.fini ();
  }

  استفاده صفحه_t = hb_بیت_صفحه_t;
  بنية صفحه_map_t
  {
    صحيح cmp (ثابت صفحه_map_t &o) ثابت { أرجع cmp (o.major); }
    صحيح cmp (uint32_t o_major) ثابت { أرجع (صحيح) o_major - (صحيح) major; }

    uint32_t major;
    uint32_t شاخص_2;
  };

  منطقي successful = درست; /* Allocations successful */
  تغییرپذیر بیعلامت صحيح population = 0;
  تغییرپذیر hb_atomic_t<بیعلامت> آخرین_صفحه_lookup = 0;
  hb_sorted_وکتور_t<صفحه_map_t> صفحه_map;
  hb_وکتور_t<صفحه_t> صفحات;

  فراغ err () { إذا (successful) successful = خطأ; } /* TODO Remove */
  منطقي درون_error () ثابت { أرجع !successful; }

  منطقي resize (بیعلامت صحيح شمار, منطقي clear = درست, منطقي exact_اندازه = خطأ)
  {
    إذا (unlikely (!successful)) أرجع خطأ;

    إذا (صفحات.درازی < شمار && (بیعلامت) صفحات.allocated < شمار && شمار <= 2)
      exact_اندازه = درست; // Most sets are small and local

    إذا (unlikely (!صفحات.resize_پر (شمار, clear, exact_اندازه) ||
	!صفحه_map.resize_پر (شمار, clear, خطأ)))
    {
      صفحات.resize_پر (صفحه_map.درازی, clear, exact_اندازه);
      successful = خطأ;
      أرجع خطأ;
    }
    أرجع درست;
  }

  فراغ alloc (بیعلامت sz)
  {
    sz >>= (صفحه_t::صفحه_بیتها_LOG_2 - 1);
    صفحات.alloc (sz);
    صفحه_map.alloc (sz);
  }

  hb_بیت_مجموعه_t& reset ()
  {
    successful = درست;
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
    إذا (!صفحات.درازی && !population) أرجع;
    صفحات.clear ();
    صفحه_map.clear ();
    population = 0;
  }
  منطقي is_empty () ثابت
  {
    بیعلامت صحيح شمار = صفحات.درازی;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (!صفحات[i].is_empty ())
	أرجع خطأ;
    أرجع درست;
  }
  صریح عملگر منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    uint32_t h = 0;
    لكل (خودکار &map : صفحه_map)
    {
      خودکار &صفحه = صفحات.arrayZ[map.شاخص_2];
      إذا (unlikely (صفحه.is_empty ())) استمر;
      h = h * 31 + hb_hash (map.major) + hb_hash (صفحه);
    }
    أرجع h;
  }

  خاص:
  فراغ dirty () { population = UINT_MAX; }
  عام:

  فراغ جمع_کن (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (unlikely (g == INVALID)) أرجع;
    dirty ();
    صفحه_t *صفحه = صفحه_for (g, درست); إذا (unlikely (!صفحه)) أرجع;
    صفحه->جمع_کن (g);
  }
  فراغ جمع_کن_بیتها (hb_codepoint_t g, uint64_t بیتها)
  {
    إذا (unlikely (!successful) || unlikely (!بیتها)) أرجع;
    أكد (!(g & (صفحه_t::ELT_بیتها - 1)));
    dirty ();
    صفحه_t *صفحه = صفحه_for (g, درست); إذا (unlikely (!صفحه)) أرجع;
    صفحه->جمع_کن_بیتها (g, بیتها);
  }
  منطقي جمع_کن_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!successful)) أرجع درست; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (a > b || a == INVALID || b == INVALID)) أرجع خطأ;
    dirty ();
    بیعلامت صحيح ma = get_major (a);
    بیعلامت صحيح mb = get_major (b);
    إذا (ma == mb)
    {
      صفحه_t *صفحه = صفحه_for (a, درست); إذا (unlikely (!صفحه)) أرجع خطأ;
      صفحه->جمع_کن_range (a, b);
    }
    وإلا
    {
      صفحه_t *صفحه = صفحه_for (a, درست); إذا (unlikely (!صفحه)) أرجع خطأ;
      صفحه->جمع_کن_range (a, major_شروع (ma + 1) - 1);

      لكل (بیعلامت صحيح m = ma + 1; m < mb; m++)
      {
	صفحه = صفحه_for (major_شروع (m), درست); إذا (unlikely (!صفحه)) أرجع خطأ;
	صفحه->init1 ();
      }

      صفحه = صفحه_for (b, درست); إذا (unlikely (!صفحه)) أرجع خطأ;
      صفحه->جمع_کن_range (major_شروع (mb), b);
    }
    أرجع درست;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  قالب<نام_نوع Type>
  ساكن مضمن ثابت Type& Struct_At_جابه_جایی_Unaligned(ثابت فراغ *P, بیعلامت صحيح جابه_جایی)
  {
#رهنمود GCC diagnostic push
#رهنمود GCC diagnostic ignored "-Wcast-align"
    أرجع * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + جابه_جایی);
#رهنمود GCC diagnostic برداشت
  }

  قالب <نام_نوع T>
  فراغ مجموعه_array (منطقي v, ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (!شمار) أرجع;
    dirty ();
    hb_codepoint_t g = *array;
    طالما (شمار)
    {
      بیعلامت صحيح m = get_major (g);
      صفحه_t *صفحه = صفحه_for (g, v); إذا (unlikely (v && !صفحه)) أرجع;
      بیعلامت صحيح شروع = major_شروع (m);
      بیعلامت صحيح انتها = major_شروع (m + 1);
      افعل
      {
        إذا (g != INVALID && (v || صفحه)) /* The v check is to optimize out the page check if v is true. */
	  صفحه->مجموعه (g, v);

	array = &Struct_At_جابه_جایی_Unaligned<T> (array, stride);
	شمار--;
      }
      طالما (شمار && (g = *array, شروع <= g && g < انتها));
    }
  }

  قالب <نام_نوع T>
  فراغ جمع_کن_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  { مجموعه_array (درست, array, شمار, stride); }
  قالب <نام_نوع T>
  فراغ جمع_کن_array (ثابت hb_array_t<ثابت T>& arr) { جمع_کن_array (&arr, arr.len ()); }

  قالب <نام_نوع T>
  فراغ del_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  { مجموعه_array (خطأ, array, شمار, stride); }
  قالب <نام_نوع T>
  فراغ del_array (ثابت hb_array_t<ثابت T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  قالب <نام_نوع T>
  منطقي مجموعه_sorted_array (منطقي v, ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع درست; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (!شمار)) أرجع درست;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t آخرین_g = g;
    طالما (شمار)
    {
      بیعلامت صحيح m = get_major (g);
      صفحه_t *صفحه = صفحه_for (g, v); إذا (unlikely (v && !صفحه)) أرجع خطأ;
      بیعلامت صحيح انتها = major_شروع (m + 1);
      افعل
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	إذا (g < آخرین_g) أرجع خطأ;
	آخرین_g = g;

        إذا (g != INVALID && (v || صفحه)) /* The v check is to optimize out the page check if v is true. */
	  صفحه->مجموعه (g, v);

	array = &Struct_At_جابه_جایی_Unaligned<T> (array, stride);
	شمار--;
      }
      طالما (شمار && (g = *array, g < انتها));
    }
    أرجع درست;
  }

  قالب <نام_نوع T>
  منطقي جمع_کن_sorted_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  { أرجع مجموعه_sorted_array (درست, array, شمار, stride); }
  قالب <نام_نوع T>
  منطقي جمع_کن_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جمع_کن_sorted_array (&arr, arr.len ()); }

  قالب <نام_نوع T>
  منطقي del_sorted_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  { أرجع مجموعه_sorted_array (خطأ, array, شمار, stride); }
  قالب <نام_نوع T>
  منطقي del_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع del_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    صفحه_t *صفحه = صفحه_for (g);
    إذا (!صفحه)
      أرجع;
    dirty ();
    صفحه->del (g);
  }
  فراغ del_بیتها (hb_codepoint_t g, uint64_t بیتها)
  {
    إذا (unlikely (!successful) || unlikely (!بیتها)) أرجع;
    أكد (!(g & (صفحه_t::ELT_بیتها - 1)));
    صفحه_t *صفحه = صفحه_for (g);
    إذا (!صفحه)
      أرجع;
    dirty ();
    صفحه->del_بیتها (g, بیتها);
  }

  خاص:
  فراغ del_صفحات (صحيح ds, صحيح de)
  {
    إذا (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_وکتور_t<بیعلامت> compact_workspace;
      إذا (unlikely (!allocate_compact_workspace (compact_workspace))) أرجع;

      بیعلامت صحيح write_شاخص = 0;
      لكل (بیعلامت صحيح i = 0; i < صفحه_map.درازی; i++)
      {
	صحيح m = (صحيح) صفحه_map.arrayZ[i].major;
	إذا (m < ds || de < m)
	  صفحه_map.arrayZ[write_شاخص++] = صفحه_map.arrayZ[i];
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
    بیعلامت صحيح ma = get_major (a);
    بیعلامت صحيح mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    صحيح ds = (a == major_شروع (ma))? (صحيح) ma: (صحيح) (ma + 1);
    صحيح de = (b + 1 == major_شروع (mb + 1))? (صحيح) mb: ((صحيح) mb - 1);
    إذا (ds > de || (صحيح) ma < ds)
    {
      صفحه_t *صفحه = صفحه_for (a);
      إذا (صفحه)
      {
	إذا (ma == mb)
	  صفحه->del_range (a, b);
	وإلا
	  صفحه->del_range (a, major_شروع (ma + 1) - 1);
      }
    }
    إذا (de < (صحيح) mb && ma != mb)
    {
      صفحه_t *صفحه = صفحه_for (b);
      إذا (صفحه)
	صفحه->del_range (major_شروع (mb), b);
    }
    del_صفحات (ds, de);
  }

  منطقي get (hb_codepoint_t g) ثابت
  {
    ثابت صفحه_t *صفحه = صفحه_for (g);
    إذا (!صفحه)
      أرجع خطأ;
    أرجع صفحه->get (g);
  }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي عملگر [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي عملگر () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_بیت_مجموعه_t& عملگر << (hb_codepoint_t v)
  { جمع_کن (v); أرجع *هذا; }
  hb_بیت_مجموعه_t& عملگر << (ثابت hb_codepoint_pair_t& range)
  { جمع_کن_range (range.اولی, range.second); أرجع *هذا; }

  منطقي intersects (ثابت hb_بیت_مجموعه_t &other) ثابت
  {
    بیعلامت صحيح na = صفحات.درازی;
    بیعلامت صحيح nb = other.صفحات.درازی;

    بیعلامت صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (صفحه_map.arrayZ[a].major == other.صفحه_map.arrayZ[b].major)
      {
	إذا (صفحه_at (a).intersects (other.صفحه_at (b)))
	  أرجع درست;
	a++;
	b++;
      }
      وإلا إذا (صفحه_map.arrayZ[a].major < other.صفحه_map.arrayZ[b].major)
	a++;
      وإلا
	b++;
    }
    أرجع خطأ;
  }
  منطقي may_intersect (ثابت hb_بیت_مجموعه_t &other) ثابت
  { أرجع intersects (other); }

  منطقي intersects (hb_codepoint_t اولی, hb_codepoint_t آخرین) ثابت
  {
    hb_codepoint_t c = اولی - 1;
    أرجع next (&c) && c <= آخرین;
  }
  فراغ مجموعه (ثابت hb_بیت_مجموعه_t &other, منطقي exact_اندازه = خطأ)
  {
    إذا (unlikely (!successful)) أرجع;
    بیعلامت صحيح شمار = other.صفحات.درازی;
    إذا (unlikely (!resize  (شمار, خطأ, exact_اندازه)))
      أرجع;
    population = other.population;

    صفحه_map = other.صفحه_map;
    صفحات = other.صفحات;
  }

  منطقي is_equal (ثابت hb_بیت_مجموعه_t &other) ثابت
  {
    إذا (has_population () && other.has_population () &&
	population != other.population)
      أرجع خطأ;

    بیعلامت صحيح na = صفحات.درازی;
    بیعلامت صحيح nb = other.صفحات.درازی;

    بیعلامت صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (صفحه_at (a).is_empty ()) { a++; استمر; }
      إذا (other.صفحه_at (b).is_empty ()) { b++; استمر; }
      إذا (صفحه_map.arrayZ[a].major != other.صفحه_map.arrayZ[b].major ||
	  !صفحه_at (a).is_equal (other.صفحه_at (b)))
	أرجع خطأ;
      a++;
      b++;
    }
    لكل (; a < na; a++)
      إذا (!صفحه_at (a).is_empty ()) { أرجع خطأ; }
    لكل (; b < nb; b++)
      إذا (!other.صفحه_at (b).is_empty ()) { أرجع خطأ; }

    أرجع درست;
  }

  منطقي is_subset (ثابت hb_بیت_مجموعه_t &larger_مجموعه) ثابت
  {
    إذا (has_population () && larger_مجموعه.has_population () &&
	population > larger_مجموعه.population)
      أرجع خطأ;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    طالما (spi < صفحه_map.درازی && lpi < larger_مجموعه.صفحه_map.درازی)
    {
      uint32_t spm = صفحه_map.arrayZ[spi].major;
      uint32_t lpm = larger_مجموعه.صفحه_map.arrayZ[lpi].major;
      خودکار sp = صفحه_at (spi);

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

      خودکار lp = larger_مجموعه.صفحه_at (lpi);
      إذا (!sp.is_subset (lp))
        أرجع خطأ;

      spi++;
      lpi++;
    }

    طالما (spi < صفحه_map.درازی)
      إذا (!صفحه_at (spi++).is_empty ())
        أرجع خطأ;

    أرجع درست;
  }

  خاص:
  منطقي allocate_compact_workspace (hb_وکتور_t<بیعلامت>& workspace)
  {
    إذا (unlikely (!workspace.resize_exact (صفحات.درازی)))
    {
      successful = خطأ;
      أرجع خطأ;
    }

    أرجع درست;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  فراغ compact (hb_وکتور_t<بیعلامت>& workspace,
                بیعلامت صحيح درازی)
  {
    أكد(workspace.درازی == صفحات.درازی);
    hb_وکتور_t<بیعلامت>& old_شاخص_مقصد_صفحه_map_شاخص = workspace;

    hb_fill (old_شاخص_مقصد_صفحه_map_شاخص.writer(), 0xFFFFFFFF);
    لكل (بیعلامت i = 0; i < درازی; i++)
      old_شاخص_مقصد_صفحه_map_شاخص[صفحه_map[i].شاخص_2] =  i;

    compact_صفحات (old_شاخص_مقصد_صفحه_map_شاخص);
  }
  فراغ compact_صفحات (ثابت hb_وکتور_t<بیعلامت>& old_شاخص_مقصد_صفحه_map_شاخص)
  {
    بیعلامت صحيح write_شاخص = 0;
    لكل (بیعلامت صحيح i = 0; i < صفحات.درازی; i++)
    {
      إذا (old_شاخص_مقصد_صفحه_map_شاخص[i] == 0xFFFFFFFF) استمر;

      إذا (write_شاخص < i)
	صفحات[write_شاخص] = صفحات[i];

      صفحه_map[old_شاخص_مقصد_صفحه_map_شاخص[i]].شاخص_2 = write_شاخص;
      write_شاخص++;
    }
  }
  عام:

  فراغ process_ (hb_بیت_صفحه_t::وکتور_t (*op) (ثابت hb_بیت_صفحه_t::وکتور_t &, ثابت hb_بیت_صفحه_t::وکتور_t &),
		 منطقي passthru_چپ, منطقي passthru_راست,
		 ثابت hb_بیت_مجموعه_t &other,
		 hb_وکتور_t<بیعلامت> *workspace = اشاره_تهی)
  {
    إذا (unlikely (!successful)) أرجع;

    dirty ();

    بیعلامت صحيح na = صفحات.درازی;
    بیعلامت صحيح nb = other.صفحات.درازی;
    بیعلامت صحيح بعدی_صفحه = na;

    بیعلامت صحيح شمار = 0, new_شمار = 0;
    بیعلامت صحيح a = 0, b = 0;
    بیعلامت صحيح write_شاخص = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_وکتور_t<بیعلامت> local_workspace;
    hb_وکتور_t<بیعلامت> &compact_workspace = workspace ? *workspace : local_workspace;
    إذا (!passthru_چپ)
    {
      منطقي allocated = workspace ? compact_workspace.resize (صفحات.درازی)
				 : allocate_compact_workspace (compact_workspace);
      إذا (unlikely (!allocated))
      {
	successful = خطأ;
	أرجع;
      }
    }

    لكل (; a < na && b < nb; )
    {
      إذا (صفحه_map.arrayZ[a].major == other.صفحه_map.arrayZ[b].major)
      {
	إذا (!passthru_چپ)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  إذا (write_شاخص < a)
	    صفحه_map.arrayZ[write_شاخص] = صفحه_map.arrayZ[a];
	  write_شاخص++;
	}

	شمار++;
	a++;
	b++;
      }
      وإلا إذا (صفحه_map.arrayZ[a].major < other.صفحه_map.arrayZ[b].major)
      {
	إذا (passthru_چپ)
	  شمار++;
	a++;
      }
      وإلا
      {
	إذا (passthru_راست)
	  شمار++;
	b++;
      }
    }
    إذا (passthru_چپ)
      شمار += na - a;
    إذا (passthru_راست)
      شمار += nb - b;

    إذا (!passthru_چپ)
    {
      na  = write_شاخص;
      بعدی_صفحه = write_شاخص;
      compact (compact_workspace, write_شاخص);
    }

    إذا (unlikely (!resize (شمار)))
      أرجع;

    new_شمار = شمار;

    /* Process in-place backward. */
    a = na;
    b = nb;
    لكل (; a && b; )
    {
      إذا (صفحه_map.arrayZ[a - 1].major == other.صفحه_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	شمار--;
	صفحه_map.arrayZ[شمار] = صفحه_map.arrayZ[a];
	صفحه_at (شمار).v = op (صفحه_at (a).v, other.صفحه_at (b).v);
	صفحه_at (شمار).dirty ();
      }
      وإلا إذا (صفحه_map.arrayZ[a - 1].major > other.صفحه_map.arrayZ[b - 1].major)
      {
	a--;
	إذا (passthru_چپ)
	{
	  شمار--;
	  صفحه_map.arrayZ[شمار] = صفحه_map.arrayZ[a];
	}
      }
      وإلا
      {
	b--;
	إذا (passthru_راست)
	{
	  شمار--;
	  صفحه_map.arrayZ[شمار].major = other.صفحه_map.arrayZ[b].major;
	  صفحه_map.arrayZ[شمار].شاخص_2 = بعدی_صفحه++;
	  صفحه_at (شمار) = other.صفحه_at (b);
	}
      }
    }
    إذا (passthru_چپ)
      طالما (a)
      {
	a--;
	شمار--;
	صفحه_map.arrayZ[شمار] = صفحه_map.arrayZ[a];
      }
    إذا (passthru_راست)
      طالما (b)
      {
	b--;
	شمار--;
	صفحه_map.arrayZ[شمار].major = other.صفحه_map.arrayZ[b].major;
	صفحه_map.arrayZ[شمار].شاخص_2 = بعدی_صفحه++;
	صفحه_at (شمار) = other.صفحه_at (b);
      }
    أكد (!شمار);
    resize (new_شمار);
  }
  قالب <نام_نوع Op>
  ساكن hb_بیت_صفحه_t::وکتور_t
  op_ (ثابت hb_بیت_صفحه_t::وکتور_t &a, ثابت hb_بیت_صفحه_t::وکتور_t &b)
  { أرجع Op{} (a, b); }
  قالب <نام_نوع Op>
  فراغ process (ثابت Op& op,
		ثابت hb_بیت_مجموعه_t &other,
		hb_وکتور_t<بیعلامت> *workspace = اشاره_تهی)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  فراغ union_ (ثابت hb_بیت_مجموعه_t &other) { process (hb_bitwise_or, other); }
  فراغ intersect (ثابت hb_بیت_مجموعه_t &other,
		  hb_وکتور_t<بیعلامت> *workspace = اشاره_تهی)
  { process (hb_bitwise_and, other, workspace); }
  فراغ تفریق (ثابت hb_بیت_مجموعه_t &other) { process (hb_bitwise_gt, other); }
  فراغ متقارن_difference (ثابت hb_بیت_مجموعه_t &other) { process (hb_bitwise_xor, other); }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      أرجع *codepoint != INVALID;
    }

    ثابت خودکار* صفحه_map_array = صفحه_map.arrayZ;
    بیعلامت صحيح major = get_major (*codepoint);
    بیعلامت صحيح i = آخرین_صفحه_lookup;

    إذا (unlikely (i >= صفحه_map.درازی || صفحه_map_array[i].major != major))
    {
      صفحه_map.bfind (major, &i, HB_NOT_یافت_STORE_CLOSEST);
      إذا (i >= صفحه_map.درازی) {
        *codepoint = INVALID;
        أرجع خطأ;
      }
      آخرین_صفحه_lookup = i;
    }

    ثابت خودکار* صفحات_array = صفحات.arrayZ;
    ثابت صفحه_map_t &current = صفحه_map_array[i];
    إذا (likely (current.major == major))
    {
      إذا (صفحات_array[current.شاخص_2].next (codepoint))
      {
        *codepoint += current.major * صفحه_t::صفحه_بیتها;
        أرجع درست;
      }
      i++;
    }

    لكل (; i < صفحه_map.درازی; i++)
    {
      ثابت صفحه_map_t &current = صفحه_map_array[i];
      hb_codepoint_t m = صفحات_array[current.شاخص_2].get_min ();
      إذا (m != INVALID)
      {
	*codepoint = current.major * صفحه_t::صفحه_بیتها + m;
        آخرین_صفحه_lookup = i;
	أرجع درست;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي بعدی_بیتها (hb_codepoint_t *codepoint, uint64_t *بیتها) ثابت
  {
    static_assert (صفحه_t::ELT_بیتها == 64, "");

    بیعلامت صحيح i = 0;
    بیعلامت صحيح elt = 0;
    إذا (likely (*codepoint != INVALID))
    {
      hb_codepoint_t پایه = *codepoint & ~صفحه_t::ELT_ماسک;
      إذا (unlikely (پایه >= INVALID - صفحه_t::ELT_بیتها + 1))
        اذهب انجامشده;
      پایه += صفحه_t::ELT_بیتها;

      بیعلامت صحيح major = get_major (پایه);
      i = آخرین_صفحه_lookup;
      إذا (unlikely (i >= صفحه_map.درازی || صفحه_map.arrayZ[i].major != major))
      {
	صفحه_map.bfind (major, &i, HB_NOT_یافت_STORE_CLOSEST);
	إذا (i >= صفحه_map.درازی)
	  اذهب انجامشده;
      }
      إذا (صفحه_map.arrayZ[i].major == major)
	elt = صفحه_remainder (پایه) / صفحه_t::ELT_بیتها;
    }

    لكل (; i < صفحه_map.درازی; i++, elt = 0)
    {
      ثابت صفحه_map_t &map = صفحه_map.arrayZ[i];
      ثابت صفحه_t &صفحه = صفحات.arrayZ[map.شاخص_2];
      لكل (; elt < صفحه_t::len (); elt++)
	إذا (صفحه.v[elt])
	{
	  *codepoint = major_شروع (map.major) + elt * صفحه_t::ELT_بیتها;
	  *بیتها = صفحه.v[elt];
	  آخرین_صفحه_lookup = i;
	  أرجع درست;
	}
    }

  انجامشده:
    *codepoint = INVALID;
    *بیتها = 0;
    أرجع خطأ;
  }
  منطقي قبلی (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      أرجع *codepoint != INVALID;
    }

    صفحه_map_t map = {get_major (*codepoint), 0};
    بیعلامت صحيح i;
    صفحه_map.bfind (map, &i, HB_NOT_یافت_STORE_CLOSEST);
    إذا (i < صفحه_map.درازی && صفحه_map.arrayZ[i].major == map.major)
    {
      إذا (صفحات[صفحه_map.arrayZ[i].شاخص_2].قبلی (codepoint))
      {
	*codepoint += صفحه_map.arrayZ[i].major * صفحه_t::صفحه_بیتها;
	أرجع درست;
      }
    }
    i--;
    لكل (; (صحيح) i >= 0; i--)
    {
      hb_codepoint_t m = صفحات.arrayZ[صفحه_map.arrayZ[i].شاخص_2].get_max ();
      إذا (m != INVALID)
      {
	*codepoint = صفحه_map.arrayZ[i].major * صفحه_t::صفحه_بیتها + m;
	أرجع درست;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي بعدی_range (hb_codepoint_t *اولی, hb_codepoint_t *آخرین) ثابت
  {
    hb_codepoint_t i;

    i = *آخرین;
    إذا (!next (&i))
    {
      *آخرین = *اولی = INVALID;
      أرجع خطأ;
    }

    *آخرین = *اولی = i;

    ثابت خودکار* صفحه_map_array = صفحه_map.arrayZ;
    ثابت خودکار* صفحات_array = صفحات.arrayZ;
    بیعلامت صحيح major = get_major (*آخرین);
    بیعلامت صحيح p_شاخص = آخرین_صفحه_lookup;

    إذا (unlikely (p_شاخص >= صفحه_map.درازی || صفحه_map_array[p_شاخص].major != major))
    {
      صفحه_map.bfind (major, &p_شاخص, HB_NOT_یافت_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      أكد(p_شاخص < صفحه_map.درازی && صفحه_map_array[p_شاخص].major == major);
      آخرین_صفحه_lookup = p_شاخص;
    }

    بیعلامت صحيح rem = صفحه_remainder (*آخرین);
    بیعلامت صحيح elt = rem / صفحه_t::ELT_بیتها;
    بیعلامت صحيح bit = rem & صفحه_t::ELT_ماسک;

    ثابت صفحه_t *صفحه = &صفحات_array[صفحه_map_array[p_شاخص].شاخص_2];
    بیعلامت صحيح پیاپی = hb_ctz (~(صفحه->v[elt] >> bit));
    *آخرین += پیاپی - 1;
    إذا (پیاپی < صفحه_t::ELT_بیتها - bit)
      أرجع درست;

    elt++;

    طالما (درست)
    {
      لكل (; elt < صفحه_t::len (); elt++)
      {
        uint64_t w = صفحه->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *آخرین += صفحه_t::ELT_بیتها;
          استمر;
        }
        uint64_t inv = ~w;
        *آخرین += hb_ctz (inv);
        أرجع درست;
      }

      p_شاخص++;


      إذا (p_شاخص >= صفحه_map.درازی || صفحه_map_array[p_شاخص].major != major + 1)
        أرجع درست;

      ثابت خودکار& مدخل = صفحه_map_array[p_شاخص];
      major = مدخل.major;
      آخرین_صفحه_lookup = p_شاخص;
      صفحه = &صفحات_array[مدخل.شاخص_2];
      elt = 0;
    }
  }

  منطقي قبلی_range (hb_codepoint_t *اولی, hb_codepoint_t *آخرین) ثابت
  {
    hb_codepoint_t i;

    i = *اولی;
    إذا (!قبلی (&i))
    {
      *آخرین = *اولی = INVALID;
      أرجع خطأ;
    }

    *آخرین = *اولی = i;

    ثابت خودکار* صفحه_map_array = صفحه_map.arrayZ;
    ثابت خودکار* صفحات_array = صفحات.arrayZ;
    بیعلامت صحيح major = get_major (*اولی);
    بیعلامت صحيح p_شاخص = آخرین_صفحه_lookup;

    إذا (unlikely (p_شاخص >= صفحه_map.درازی || صفحه_map_array[p_شاخص].major != major))
    {
      صفحه_map.bfind (major, &p_شاخص, HB_NOT_یافت_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      أكد(p_شاخص < صفحه_map.درازی && صفحه_map_array[p_شاخص].major == major);
      آخرین_صفحه_lookup = p_شاخص;
    }

    بیعلامت صحيح rem = صفحه_remainder (*اولی);
    صحيح elt = rem / صفحه_t::ELT_بیتها;
    بیعلامت صحيح bit = rem & صفحه_t::ELT_ماسک;

    ثابت صفحه_t *صفحه = &صفحات_array[صفحه_map_array[p_شاخص].شاخص_2];
    uint64_t inv = ~(صفحه->v[elt] << (63 - bit));
    بیعلامت صحيح پیاپی = hb_بیت_storage (inv) ? (صفحه_t::ELT_بیتها - hb_بیت_storage (inv)) : صفحه_t::ELT_بیتها;
    *اولی -= پیاپی - 1;
    إذا (پیاپی < bit + 1)
      أرجع درست;

    elt--;

    طالما (درست)
    {
      لكل (; elt >= 0; elt--)
      {
        uint64_t w = صفحه->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *اولی -= صفحه_t::ELT_بیتها;
          استمر;
        }
        uint64_t inv = ~w;
        بیعلامت صحيح zeros = hb_بیت_storage (inv) ? (صفحه_t::ELT_بیتها - hb_بیت_storage (inv)) : صفحه_t::ELT_بیتها;
        *اولی -= zeros;
        أرجع درست;
      }


      إذا (p_شاخص == 0 || صفحه_map_array[p_شاخص - 1].major != major - 1)
        أرجع درست;

      p_شاخص--;
      ثابت خودکار& مدخل = صفحه_map_array[p_شاخص];

      major = مدخل.major;
      آخرین_صفحه_lookup = p_شاخص;
      صفحه = &صفحات_array[مدخل.شاخص_2];
      elt = صفحه_t::len () - 1;
    }
  }

  بیعلامت صحيح بعدی_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *بیرون,
			  بیعلامت صحيح    اندازه) ثابت
  {
    // By default, start at the first bit of the first page of values.
    بیعلامت صحيح شروع_صفحه = 0;
    بیعلامت صحيح شروع_صفحه_مقدار = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت خودکار* صفحه_map_array = صفحه_map.arrayZ;
      بیعلامت صحيح major = get_major (codepoint);
      بیعلامت صحيح i = آخرین_صفحه_lookup;
      إذا (unlikely (i >= صفحه_map.درازی || صفحه_map_array[i].major != major))
      {
	صفحه_map.bfind (major, &i, HB_NOT_یافت_STORE_CLOSEST);
	إذا (i >= صفحه_map.درازی)
	  أرجع 0;  // codepoint is greater than our max element.
      }
      شروع_صفحه = i;
      إذا (صفحه_map_array[i].major == major)
      {
        شروع_صفحه_مقدار = صفحه_remainder (codepoint + 1);
        إذا (unlikely (شروع_صفحه_مقدار == 0))
        {
          // The export-after value was last in the page. Start on next page.
          شروع_صفحه++;
          شروع_صفحه_مقدار = 0;
        }
      }
    }

    بیعلامت صحيح initial_اندازه = اندازه;
    لكل (بیعلامت صحيح i = شروع_صفحه; i < صفحه_map.درازی && اندازه; i++)
    {
      uint32_t پایه = major_شروع (صفحه_map.arrayZ[i].major);
      بیعلامت صحيح n = صفحات[صفحه_map.arrayZ[i].شاخص_2].اكتب (پایه, شروع_صفحه_مقدار, بیرون, اندازه);
      بیرون += n;
      اندازه -= n;
      شروع_صفحه_مقدار = 0;
    }
    أرجع initial_اندازه - اندازه;
  }

  بیعلامت صحيح بعدی_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *بیرون,
				   بیعلامت صحيح    اندازه) ثابت
  {
    بیعلامت صحيح initial_اندازه = اندازه;
    // By default, start at the first bit of the first page of values.
    بیعلامت صحيح شروع_صفحه = 0;
    بیعلامت صحيح شروع_صفحه_مقدار = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت خودکار* صفحه_map_array = صفحه_map.arrayZ;
      بیعلامت صحيح major = get_major (codepoint);
      بیعلامت صحيح i = آخرین_صفحه_lookup;
      إذا (unlikely (i >= صفحه_map.درازی || صفحه_map_array[i].major != major))
      {
        صفحه_map.bfind(major, &i, HB_NOT_یافت_STORE_CLOSEST);
        إذا (unlikely (i >= صفحه_map.درازی))
        {
          // codepoint is greater than our max element.
          طالما (++codepoint != INVALID && اندازه)
          {
            *بیرون++ = codepoint;
            اندازه--;
          }
          أرجع initial_اندازه - اندازه;
        }
      }
      شروع_صفحه = i;
      إذا (صفحه_map_array[i].major == major)
      {
        شروع_صفحه_مقدار = صفحه_remainder (codepoint + 1);
        إذا (unlikely (شروع_صفحه_مقدار == 0))
        {
          // The export-after value was last in the page. Start on next page.
          شروع_صفحه++;
          شروع_صفحه_مقدار = 0;
        }
      }
    }

    hb_codepoint_t بعدی_مقدار = codepoint + 1;
    لكل (بیعلامت صحيح i=شروع_صفحه; i<صفحه_map.درازی && اندازه; i++)
    {
      uint32_t پایه = major_شروع (صفحه_map.arrayZ[i].major);
      بیعلامت صحيح n = صفحات[صفحه_map.arrayZ[i].شاخص_2].write_inverted (پایه, شروع_صفحه_مقدار, بیرون, اندازه, &بعدی_مقدار);
      بیرون += n;
      اندازه -= n;
      شروع_صفحه_مقدار = 0;
    }
    طالما (بعدی_مقدار < HB_مجموعه_مقدار_INVALID && اندازه) {
      *بیرون++ = بعدی_مقدار++;
      اندازه--;
    }
    أرجع initial_اندازه - اندازه;
  }

  منطقي has_population () ثابت { أرجع population != UINT_MAX; }
  بیعلامت صحيح get_population () ثابت
  {
    إذا (has_population ())
      أرجع population;

    بیعلامت صحيح برداشت = 0;
    بیعلامت صحيح شمار = صفحات.درازی;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      برداشت += صفحات[i].get_population ();

    population = برداشت;
    أرجع برداشت;
  }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    إذا (has_population ())
    {
      إذا (population != 1) أرجع خطأ;
      *codepoint = get_min ();
      أرجع درست;
    }

    hb_codepoint_t singleton = INVALID;
    لكل (ثابت خودکار &map : صفحه_map)
    {
      ثابت خودکار &صفحه = صفحات.arrayZ[map.شاخص_2];
      لكل (بیعلامت i = 0; i < صفحه_t::len (); i++)
      {
	صفحه_t::elt_t بیتها = صفحه.v[i];
	إذا (!بیتها) استمر;
	إذا (singleton != INVALID || (بیتها & (بیتها - 1)))
	  أرجع خطأ;
	singleton = map.major * صفحه_t::صفحه_بیتها +
		    i * صفحه_t::ELT_بیتها + hb_ctz (بیتها);
      }
    }

    إذا (singleton == INVALID)
    {
      population = 0;
      أرجع خطأ;
    }
    population = 1;
    *codepoint = singleton;
    أرجع درست;
  }
  hb_codepoint_t get_min () ثابت
  {
    بیعلامت شمار = صفحات.درازی;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      ثابت خودکار& map = صفحه_map.arrayZ[i];
      ثابت خودکار& صفحه = صفحات.arrayZ[map.شاخص_2];

      إذا (!صفحه.is_empty ())
	أرجع map.major * صفحه_t::صفحه_بیتها + صفحه.get_min ();
    }
    أرجع INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    بیعلامت شمار = صفحات.درازی;
    لكل (علامتدار i = شمار - 1; i >= 0; i--)
    {
      ثابت خودکار& map = صفحه_map.arrayZ[(بیعلامت) i];
      ثابت خودکار& صفحه = صفحات.arrayZ[map.شاخص_2];

      إذا (!صفحه.is_empty ())
	أرجع map.major * صفحه_t::صفحه_بیتها + صفحه.get_max ();
    }
    أرجع INVALID;
  }

  ساكن constexpr hb_codepoint_t INVALID = صفحه_t::INVALID;

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = درست;
    ساكن constexpr منطقي has_fast_len = درست;
    iter_t (ثابت hb_بیت_مجموعه_t &s_ = Null (hb_بیت_مجموعه_t),
	    منطقي init = درست) : s (&s_), v (INVALID), l(0)
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
    فراغ __prev__ () { s->قبلی (&v); }
    بیعلامت __len__ () ثابت { أرجع l; }
    iter_t انتها () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي عملگر != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_بیت_مجموعه_t *s;
    hb_codepoint_t v;
    بیعلامت l;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  عملگر iter_t () ثابت { أرجع iter (); }

  محمي:

  صفحه_t *صفحه_for (hb_codepoint_t g, منطقي insert = خطأ)
  {
    بیعلامت major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    بیعلامت i = آخرین_صفحه_lookup;
    إذا (likely (i < صفحه_map.درازی))
    {
      خودکار &cached_صفحه = صفحه_map.arrayZ[i];
      إذا (cached_صفحه.major == major)
	أرجع &صفحات.arrayZ[cached_صفحه.شاخص_2];
    }

    صفحه_map_t map = {major, صفحات.درازی};
    إذا (!صفحه_map.bfind (map, &i, HB_NOT_یافت_STORE_CLOSEST))
    {
      إذا (!insert)
        أرجع اشاره_تهی;

      إذا (unlikely (!resize (صفحات.درازی + 1)))
	أرجع اشاره_تهی;

      صفحات.arrayZ[map.شاخص_2].init0 ();
      memmove (صفحه_map.arrayZ + i + 1,
	       صفحه_map.arrayZ + i,
	       (صفحه_map.درازی - 1 - i) * صفحه_map.قلم_اندازه);
      صفحه_map.arrayZ[i] = map;
    }

    آخرین_صفحه_lookup = i;
    أرجع &صفحات.arrayZ[صفحه_map.arrayZ[i].شاخص_2];
  }
  ثابت صفحه_t *صفحه_for (hb_codepoint_t g) ثابت
  {
    بیعلامت major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    بیعلامت i = آخرین_صفحه_lookup;
    إذا (likely (i < صفحه_map.درازی))
    {
      خودکار &cached_صفحه = صفحه_map.arrayZ[i];
      إذا (cached_صفحه.major == major)
	أرجع &صفحات.arrayZ[cached_صفحه.شاخص_2];
    }

    صفحه_map_t key = {major};
    إذا (!صفحه_map.bfind (key, &i))
      أرجع اشاره_تهی;

    آخرین_صفحه_lookup = i;
    أرجع &صفحات.arrayZ[صفحه_map.arrayZ[i].شاخص_2];
  }
  صفحه_t &صفحه_at (بیعلامت صحيح i)
  {
    أكد (i < صفحه_map.درازی);
    أرجع صفحات.arrayZ[صفحه_map.arrayZ[i].شاخص_2];
  }
  ثابت صفحه_t &صفحه_at (بیعلامت صحيح i) ثابت
  {
    أكد (i < صفحه_map.درازی);
    أرجع صفحات.arrayZ[صفحه_map.arrayZ[i].شاخص_2];
  }
  بیعلامت صحيح get_major (hb_codepoint_t g) ثابت { أرجع g >> صفحه_t::صفحه_بیتها_LOG_2; }
  بیعلامت صحيح صفحه_remainder (hb_codepoint_t g) ثابت { أرجع g & صفحه_t::صفحه_BITMASK; }
  hb_codepoint_t major_شروع (بیعلامت صحيح major) ثابت { أرجع major << صفحه_t::صفحه_بیتها_LOG_2; }
};


#پایان_اگر /* HB_BIT_SET_HH */
