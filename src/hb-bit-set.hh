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

#اگر_غیر_تعریف_شدہ HB_ثنائی_ہندسہ_سیٹ_HH
#تعریف_کرو HB_ثنائی_ہندسہ_سیٹ_HH

#شامل "hb.hh"
#شامل "hb-bit-page.hh"

ساخت hb_ثنائی_ہندسہ_سیٹ_t
{
  hb_ثنائی_ہندسہ_سیٹ_t () = طےشدہ;
  ~hb_ثنائی_ہندسہ_سیٹ_t () = طےشدہ;

  hb_ثنائی_ہندسہ_سیٹ_t (ثابت hb_ثنائی_ہندسہ_سیٹ_t& other) : hb_ثنائی_ہندسہ_سیٹ_t () { سیٹ (other, سچ); }
  hb_ثنائی_ہندسہ_سیٹ_t ( hb_ثنائی_ہندسہ_سیٹ_t&& other)  noexcept : hb_ثنائی_ہندسہ_سیٹ_t () { hb_تبادلہ (*یہ, other); }
  hb_ثنائی_ہندسہ_سیٹ_t& عامل= (ثابت hb_ثنائی_ہندسہ_سیٹ_t& other) { سیٹ (other); واپس *یہ; }
  hb_ثنائی_ہندسہ_سیٹ_t& عامل= (hb_ثنائی_ہندسہ_سیٹ_t&& other)  noexcept { hb_تبادلہ (*یہ, other); واپس *یہ; }
  دوست خالی swap (hb_ثنائی_ہندسہ_سیٹ_t &a, hb_ثنائی_ہندسہ_سیٹ_t &b) noexcept
  {
    اگر (likely (!a.successful || !b.successful))
      واپس;
    hb_تبادلہ (a.population, b.population);
    hb_تبادلہ (a.آخری_صفحہ_lookup, b.آخری_صفحہ_lookup);
    hb_تبادلہ (a.صفحہ_map, b.صفحہ_map);
    hb_تبادلہ (a.صفحات, b.صفحات);
  }

  خالی init ()
  {
    successful = سچ;
    population = 0;
    آخری_صفحہ_lookup = 0;
    صفحہ_map.init ();
    صفحات.init ();
  }
  خالی fini ()
  {
    صفحہ_map.fini ();
    صفحات.fini ();
  }

  استعمال صفحہ_t = hb_ثنائی_ہندسہ_صفحہ_t;
  ساخت صفحہ_map_t
  {
    صحیح cmp (ثابت صفحہ_map_t &o) ثابت { واپس cmp (o.major); }
    صحیح cmp (uint32_t o_major) ثابت { واپس (صحیح) o_major - (صحیح) major; }

    uint32_t major;
    uint32_t اشاریہ_2;
  };

  منطقی successful = سچ; /* Allocations successful */
  تغیرپذیر غیرعلامتی صحیح population = 0;
  تغیرپذیر hb_atomic_t<غیرعلامتی> آخری_صفحہ_lookup = 0;
  hb_sorted_سمتیہ_t<صفحہ_map_t> صفحہ_map;
  hb_سمتیہ_t<صفحہ_t> صفحات;

  خالی err () { اگر (successful) successful = جھوٹ; } /* TODO Remove */
  منطقی اندر_error () ثابت { واپس !successful; }

  منطقی resize (غیرعلامتی صحیح گنتی, منطقی clear = سچ, منطقی exact_سائز = جھوٹ)
  {
    اگر (unlikely (!successful)) واپس جھوٹ;

    اگر (صفحات.لمبائی < گنتی && (غیرعلامتی) صفحات.allocated < گنتی && گنتی <= 2)
      exact_سائز = سچ; // Most sets are small and local

    اگر (unlikely (!صفحات.resize_بھرا (گنتی, clear, exact_سائز) ||
	!صفحہ_map.resize_بھرا (گنتی, clear, جھوٹ)))
    {
      صفحات.resize_بھرا (صفحہ_map.لمبائی, clear, exact_سائز);
      successful = جھوٹ;
      واپس جھوٹ;
    }
    واپس سچ;
  }

  خالی alloc (غیرعلامتی sz)
  {
    sz >>= (صفحہ_t::صفحہ_بٹ_LOG_2 - 1);
    صفحات.alloc (sz);
    صفحہ_map.alloc (sz);
  }

  hb_ثنائی_ہندسہ_سیٹ_t& reset ()
  {
    successful = سچ;
    clear ();
    واپس *یہ;
  }

  خالی clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    اگر (!صفحات.لمبائی && !population) واپس;
    صفحات.clear ();
    صفحہ_map.clear ();
    population = 0;
  }
  منطقی is_empty () ثابت
  {
    غیرعلامتی صحیح گنتی = صفحات.لمبائی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (!صفحات[i].is_empty ())
	واپس جھوٹ;
    واپس سچ;
  }
  صریح عامل منطقی () ثابت { واپس !is_empty (); }

  uint32_t hash () ثابت
  {
    uint32_t h = 0;
    برائے (خودکار &map : صفحہ_map)
    {
      خودکار &صفحہ = صفحات.arrayZ[map.اشاریہ_2];
      اگر (unlikely (صفحہ.is_empty ())) جاری;
      h = h * 31 + hb_hash (map.major) + hb_hash (صفحہ);
    }
    واپس h;
  }

  نجی:
  خالی dirty () { population = UINT_MAX; }
  عوامی:

  خالی جوڑو (hb_codepoint_t g)
  {
    اگر (unlikely (!successful)) واپس;
    اگر (unlikely (g == INVALID)) واپس;
    dirty ();
    صفحہ_t *صفحہ = صفحہ_for (g, سچ); اگر (unlikely (!صفحہ)) واپس;
    صفحہ->جوڑو (g);
  }
  خالی جوڑو_بٹ (hb_codepoint_t g, uint64_t بٹ)
  {
    اگر (unlikely (!successful) || unlikely (!بٹ)) واپس;
    أكد (!(g & (صفحہ_t::ELT_بٹ - 1)));
    dirty ();
    صفحہ_t *صفحہ = صفحہ_for (g, سچ); اگر (unlikely (!صفحہ)) واپس;
    صفحہ->جوڑو_بٹ (g, بٹ);
  }
  منطقی جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    اگر (unlikely (!successful)) واپس سچ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    اگر (unlikely (a > b || a == INVALID || b == INVALID)) واپس جھوٹ;
    dirty ();
    غیرعلامتی صحیح ma = get_major (a);
    غیرعلامتی صحیح mb = get_major (b);
    اگر (ma == mb)
    {
      صفحہ_t *صفحہ = صفحہ_for (a, سچ); اگر (unlikely (!صفحہ)) واپس جھوٹ;
      صفحہ->جوڑو_range (a, b);
    }
    ورنہ
    {
      صفحہ_t *صفحہ = صفحہ_for (a, سچ); اگر (unlikely (!صفحہ)) واپس جھوٹ;
      صفحہ->جوڑو_range (a, major_آغاز (ma + 1) - 1);

      برائے (غیرعلامتی صحیح m = ma + 1; m < mb; m++)
      {
	صفحہ = صفحہ_for (major_آغاز (m), سچ); اگر (unlikely (!صفحہ)) واپس جھوٹ;
	صفحہ->init1 ();
      }

      صفحہ = صفحہ_for (b, سچ); اگر (unlikely (!صفحہ)) واپس جھوٹ;
      صفحہ->جوڑو_range (major_آغاز (mb), b);
    }
    واپس سچ;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  سانچہ<قسم_نام Type>
  ساکن اندرونی ثابت Type& Struct_At_انحراف_Unaligned(ثابت خالی *P, غیرعلامتی صحیح انحراف)
  {
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
    واپس * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + انحراف);
#ہدایت GCC diagnostic نکالو
  }

  سانچہ <قسم_نام T>
  خالی سیٹ_array (منطقی v, ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  {
    اگر (unlikely (!successful)) واپس;
    اگر (!گنتی) واپس;
    dirty ();
    hb_codepoint_t g = *array;
    جب (گنتی)
    {
      غیرعلامتی صحیح m = get_major (g);
      صفحہ_t *صفحہ = صفحہ_for (g, v); اگر (unlikely (v && !صفحہ)) واپس;
      غیرعلامتی صحیح آغاز = major_آغاز (m);
      غیرعلامتی صحیح انتہا = major_آغاز (m + 1);
      کرو
      {
        اگر (g != INVALID && (v || صفحہ)) /* The v check is to optimize out the page check if v is true. */
	  صفحہ->سیٹ (g, v);

	array = &Struct_At_انحراف_Unaligned<T> (array, stride);
	گنتی--;
      }
      جب (گنتی && (g = *array, آغاز <= g && g < انتہا));
    }
  }

  سانچہ <قسم_نام T>
  خالی جوڑو_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  { سیٹ_array (سچ, array, گنتی, stride); }
  سانچہ <قسم_نام T>
  خالی جوڑو_array (ثابت hb_array_t<ثابت T>& arr) { جوڑو_array (&arr, arr.len ()); }

  سانچہ <قسم_نام T>
  خالی del_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  { سیٹ_array (جھوٹ, array, گنتی, stride); }
  سانچہ <قسم_نام T>
  خالی del_array (ثابت hb_array_t<ثابت T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچہ <قسم_نام T>
  منطقی سیٹ_sorted_array (منطقی v, ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  {
    اگر (unlikely (!successful)) واپس سچ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    اگر (unlikely (!گنتی)) واپس سچ;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t آخری_g = g;
    جب (گنتی)
    {
      غیرعلامتی صحیح m = get_major (g);
      صفحہ_t *صفحہ = صفحہ_for (g, v); اگر (unlikely (v && !صفحہ)) واپس جھوٹ;
      غیرعلامتی صحیح انتہا = major_آغاز (m + 1);
      کرو
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	اگر (g < آخری_g) واپس جھوٹ;
	آخری_g = g;

        اگر (g != INVALID && (v || صفحہ)) /* The v check is to optimize out the page check if v is true. */
	  صفحہ->سیٹ (g, v);

	array = &Struct_At_انحراف_Unaligned<T> (array, stride);
	گنتی--;
      }
      جب (گنتی && (g = *array, g < انتہا));
    }
    واپس سچ;
  }

  سانچہ <قسم_نام T>
  منطقی جوڑو_sorted_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  { واپس سیٹ_sorted_array (سچ, array, گنتی, stride); }
  سانچہ <قسم_نام T>
  منطقی جوڑو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { واپس جوڑو_sorted_array (&arr, arr.len ()); }

  سانچہ <قسم_نام T>
  منطقی del_sorted_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  { واپس سیٹ_sorted_array (جھوٹ, array, گنتی, stride); }
  سانچہ <قسم_نام T>
  منطقی del_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { واپس del_sorted_array (&arr, arr.len ()); }

  خالی del (hb_codepoint_t g)
  {
    اگر (unlikely (!successful)) واپس;
    صفحہ_t *صفحہ = صفحہ_for (g);
    اگر (!صفحہ)
      واپس;
    dirty ();
    صفحہ->del (g);
  }
  خالی del_بٹ (hb_codepoint_t g, uint64_t بٹ)
  {
    اگر (unlikely (!successful) || unlikely (!بٹ)) واپس;
    أكد (!(g & (صفحہ_t::ELT_بٹ - 1)));
    صفحہ_t *صفحہ = صفحہ_for (g);
    اگر (!صفحہ)
      واپس;
    dirty ();
    صفحہ->del_بٹ (g, بٹ);
  }

  نجی:
  خالی del_صفحات (صحیح ds, صحیح de)
  {
    اگر (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_سمتیہ_t<غیرعلامتی> compact_workspace;
      اگر (unlikely (!allocate_compact_workspace (compact_workspace))) واپس;

      غیرعلامتی صحیح write_اشاریہ = 0;
      برائے (غیرعلامتی صحیح i = 0; i < صفحہ_map.لمبائی; i++)
      {
	صحیح m = (صحیح) صفحہ_map.arrayZ[i].major;
	اگر (m < ds || de < m)
	  صفحہ_map.arrayZ[write_اشاریہ++] = صفحہ_map.arrayZ[i];
      }
      compact (compact_workspace, write_اشاریہ);
      resize (write_اشاریہ);
    }
  }


  عوامی:
  خالی del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    اگر (unlikely (!successful)) واپس;
    اگر (unlikely (a > b || a == INVALID)) واپس;
    dirty ();
    غیرعلامتی صحیح ma = get_major (a);
    غیرعلامتی صحیح mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    صحیح ds = (a == major_آغاز (ma))? (صحیح) ma: (صحیح) (ma + 1);
    صحیح de = (b + 1 == major_آغاز (mb + 1))? (صحیح) mb: ((صحیح) mb - 1);
    اگر (ds > de || (صحیح) ma < ds)
    {
      صفحہ_t *صفحہ = صفحہ_for (a);
      اگر (صفحہ)
      {
	اگر (ma == mb)
	  صفحہ->del_range (a, b);
	ورنہ
	  صفحہ->del_range (a, major_آغاز (ma + 1) - 1);
      }
    }
    اگر (de < (صحیح) mb && ma != mb)
    {
      صفحہ_t *صفحہ = صفحہ_for (b);
      اگر (صفحہ)
	صفحہ->del_range (major_آغاز (mb), b);
    }
    del_صفحات (ds, de);
  }

  منطقی get (hb_codepoint_t g) ثابت
  {
    ثابت صفحہ_t *صفحہ = صفحہ_for (g);
    اگر (!صفحہ)
      واپس جھوٹ;
    واپس صفحہ->get (g);
  }
  منطقی may_have (hb_codepoint_t g) ثابت { واپس get (g); }

  /* Has interface. */
  منطقی عامل [] (hb_codepoint_t k) ثابت { واپس get (k); }
  منطقی has (hb_codepoint_t k) ثابت { واپس (*یہ)[k]; }
  /* Predicate. */
  منطقی عامل () (hb_codepoint_t k) ثابت { واپس has (k); }

  /* Sink interface. */
  hb_ثنائی_ہندسہ_سیٹ_t& عامل << (hb_codepoint_t v)
  { جوڑو (v); واپس *یہ; }
  hb_ثنائی_ہندسہ_سیٹ_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { جوڑو_range (range.پہلا, range.second); واپس *یہ; }

  منطقی intersects (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) ثابت
  {
    غیرعلامتی صحیح na = صفحات.لمبائی;
    غیرعلامتی صحیح nb = other.صفحات.لمبائی;

    غیرعلامتی صحیح a = 0, b = 0;
    برائے (; a < na && b < nb; )
    {
      اگر (صفحہ_map.arrayZ[a].major == other.صفحہ_map.arrayZ[b].major)
      {
	اگر (صفحہ_at (a).intersects (other.صفحہ_at (b)))
	  واپس سچ;
	a++;
	b++;
      }
      ورنہ اگر (صفحہ_map.arrayZ[a].major < other.صفحہ_map.arrayZ[b].major)
	a++;
      ورنہ
	b++;
    }
    واپس جھوٹ;
  }
  منطقی may_intersect (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) ثابت
  { واپس intersects (other); }

  منطقی intersects (hb_codepoint_t پہلا, hb_codepoint_t آخری) ثابت
  {
    hb_codepoint_t c = پہلا - 1;
    واپس next (&c) && c <= آخری;
  }
  خالی سیٹ (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other, منطقی exact_سائز = جھوٹ)
  {
    اگر (unlikely (!successful)) واپس;
    غیرعلامتی صحیح گنتی = other.صفحات.لمبائی;
    اگر (unlikely (!resize  (گنتی, جھوٹ, exact_سائز)))
      واپس;
    population = other.population;

    صفحہ_map = other.صفحہ_map;
    صفحات = other.صفحات;
  }

  منطقی is_equal (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) ثابت
  {
    اگر (has_population () && other.has_population () &&
	population != other.population)
      واپس جھوٹ;

    غیرعلامتی صحیح na = صفحات.لمبائی;
    غیرعلامتی صحیح nb = other.صفحات.لمبائی;

    غیرعلامتی صحیح a = 0, b = 0;
    برائے (; a < na && b < nb; )
    {
      اگر (صفحہ_at (a).is_empty ()) { a++; جاری; }
      اگر (other.صفحہ_at (b).is_empty ()) { b++; جاری; }
      اگر (صفحہ_map.arrayZ[a].major != other.صفحہ_map.arrayZ[b].major ||
	  !صفحہ_at (a).is_equal (other.صفحہ_at (b)))
	واپس جھوٹ;
      a++;
      b++;
    }
    برائے (; a < na; a++)
      اگر (!صفحہ_at (a).is_empty ()) { واپس جھوٹ; }
    برائے (; b < nb; b++)
      اگر (!other.صفحہ_at (b).is_empty ()) { واپس جھوٹ; }

    واپس سچ;
  }

  منطقی is_subset (ثابت hb_ثنائی_ہندسہ_سیٹ_t &larger_سیٹ) ثابت
  {
    اگر (has_population () && larger_سیٹ.has_population () &&
	population > larger_سیٹ.population)
      واپس جھوٹ;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    جب (spi < صفحہ_map.لمبائی && lpi < larger_سیٹ.صفحہ_map.لمبائی)
    {
      uint32_t spm = صفحہ_map.arrayZ[spi].major;
      uint32_t lpm = larger_سیٹ.صفحہ_map.arrayZ[lpi].major;
      خودکار sp = صفحہ_at (spi);

      اگر (spm < lpm) {
        اگر (!sp.is_empty ())
          واپس جھوٹ;
        spi++;
        جاری;
      }

      اگر (lpm < spm) {
        lpi++;
        جاری;
      }

      خودکار lp = larger_سیٹ.صفحہ_at (lpi);
      اگر (!sp.is_subset (lp))
        واپس جھوٹ;

      spi++;
      lpi++;
    }

    جب (spi < صفحہ_map.لمبائی)
      اگر (!صفحہ_at (spi++).is_empty ())
        واپس جھوٹ;

    واپس سچ;
  }

  نجی:
  منطقی allocate_compact_workspace (hb_سمتیہ_t<غیرعلامتی>& workspace)
  {
    اگر (unlikely (!workspace.resize_exact (صفحات.لمبائی)))
    {
      successful = جھوٹ;
      واپس جھوٹ;
    }

    واپس سچ;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  خالی compact (hb_سمتیہ_t<غیرعلامتی>& workspace,
                غیرعلامتی صحیح لمبائی)
  {
    أكد(workspace.لمبائی == صفحات.لمبائی);
    hb_سمتیہ_t<غیرعلامتی>& old_اشاریہ_مقصد_صفحہ_map_اشاریہ = workspace;

    hb_fill (old_اشاریہ_مقصد_صفحہ_map_اشاریہ.writer(), 0xFFFFFFFF);
    برائے (غیرعلامتی i = 0; i < لمبائی; i++)
      old_اشاریہ_مقصد_صفحہ_map_اشاریہ[صفحہ_map[i].اشاریہ_2] =  i;

    compact_صفحات (old_اشاریہ_مقصد_صفحہ_map_اشاریہ);
  }
  خالی compact_صفحات (ثابت hb_سمتیہ_t<غیرعلامتی>& old_اشاریہ_مقصد_صفحہ_map_اشاریہ)
  {
    غیرعلامتی صحیح write_اشاریہ = 0;
    برائے (غیرعلامتی صحیح i = 0; i < صفحات.لمبائی; i++)
    {
      اگر (old_اشاریہ_مقصد_صفحہ_map_اشاریہ[i] == 0xFFFFFFFF) جاری;

      اگر (write_اشاریہ < i)
	صفحات[write_اشاریہ] = صفحات[i];

      صفحہ_map[old_اشاریہ_مقصد_صفحہ_map_اشاریہ[i]].اشاریہ_2 = write_اشاریہ;
      write_اشاریہ++;
    }
  }
  عوامی:

  خالی process_ (hb_ثنائی_ہندسہ_صفحہ_t::سمتیہ_t (*op) (ثابت hb_ثنائی_ہندسہ_صفحہ_t::سمتیہ_t &, ثابت hb_ثنائی_ہندسہ_صفحہ_t::سمتیہ_t &),
		 منطقی passthru_بایاں, منطقی passthru_دایاں,
		 ثابت hb_ثنائی_ہندسہ_سیٹ_t &other,
		 hb_سمتیہ_t<غیرعلامتی> *workspace = خالی_اشاریہ)
  {
    اگر (unlikely (!successful)) واپس;

    dirty ();

    غیرعلامتی صحیح na = صفحات.لمبائی;
    غیرعلامتی صحیح nb = other.صفحات.لمبائی;
    غیرعلامتی صحیح اگلا_صفحہ = na;

    غیرعلامتی صحیح گنتی = 0, new_گنتی = 0;
    غیرعلامتی صحیح a = 0, b = 0;
    غیرعلامتی صحیح write_اشاریہ = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_سمتیہ_t<غیرعلامتی> local_workspace;
    hb_سمتیہ_t<غیرعلامتی> &compact_workspace = workspace ? *workspace : local_workspace;
    اگر (!passthru_بایاں)
    {
      منطقی allocated = workspace ? compact_workspace.resize (صفحات.لمبائی)
				 : allocate_compact_workspace (compact_workspace);
      اگر (unlikely (!allocated))
      {
	successful = جھوٹ;
	واپس;
      }
    }

    برائے (; a < na && b < nb; )
    {
      اگر (صفحہ_map.arrayZ[a].major == other.صفحہ_map.arrayZ[b].major)
      {
	اگر (!passthru_بایاں)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  اگر (write_اشاریہ < a)
	    صفحہ_map.arrayZ[write_اشاریہ] = صفحہ_map.arrayZ[a];
	  write_اشاریہ++;
	}

	گنتی++;
	a++;
	b++;
      }
      ورنہ اگر (صفحہ_map.arrayZ[a].major < other.صفحہ_map.arrayZ[b].major)
      {
	اگر (passthru_بایاں)
	  گنتی++;
	a++;
      }
      ورنہ
      {
	اگر (passthru_دایاں)
	  گنتی++;
	b++;
      }
    }
    اگر (passthru_بایاں)
      گنتی += na - a;
    اگر (passthru_دایاں)
      گنتی += nb - b;

    اگر (!passthru_بایاں)
    {
      na  = write_اشاریہ;
      اگلا_صفحہ = write_اشاریہ;
      compact (compact_workspace, write_اشاریہ);
    }

    اگر (unlikely (!resize (گنتی)))
      واپس;

    new_گنتی = گنتی;

    /* Process in-place backward. */
    a = na;
    b = nb;
    برائے (; a && b; )
    {
      اگر (صفحہ_map.arrayZ[a - 1].major == other.صفحہ_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	گنتی--;
	صفحہ_map.arrayZ[گنتی] = صفحہ_map.arrayZ[a];
	صفحہ_at (گنتی).v = op (صفحہ_at (a).v, other.صفحہ_at (b).v);
	صفحہ_at (گنتی).dirty ();
      }
      ورنہ اگر (صفحہ_map.arrayZ[a - 1].major > other.صفحہ_map.arrayZ[b - 1].major)
      {
	a--;
	اگر (passthru_بایاں)
	{
	  گنتی--;
	  صفحہ_map.arrayZ[گنتی] = صفحہ_map.arrayZ[a];
	}
      }
      ورنہ
      {
	b--;
	اگر (passthru_دایاں)
	{
	  گنتی--;
	  صفحہ_map.arrayZ[گنتی].major = other.صفحہ_map.arrayZ[b].major;
	  صفحہ_map.arrayZ[گنتی].اشاریہ_2 = اگلا_صفحہ++;
	  صفحہ_at (گنتی) = other.صفحہ_at (b);
	}
      }
    }
    اگر (passthru_بایاں)
      جب (a)
      {
	a--;
	گنتی--;
	صفحہ_map.arrayZ[گنتی] = صفحہ_map.arrayZ[a];
      }
    اگر (passthru_دایاں)
      جب (b)
      {
	b--;
	گنتی--;
	صفحہ_map.arrayZ[گنتی].major = other.صفحہ_map.arrayZ[b].major;
	صفحہ_map.arrayZ[گنتی].اشاریہ_2 = اگلا_صفحہ++;
	صفحہ_at (گنتی) = other.صفحہ_at (b);
      }
    أكد (!گنتی);
    resize (new_گنتی);
  }
  سانچہ <قسم_نام Op>
  ساکن hb_ثنائی_ہندسہ_صفحہ_t::سمتیہ_t
  op_ (ثابت hb_ثنائی_ہندسہ_صفحہ_t::سمتیہ_t &a, ثابت hb_ثنائی_ہندسہ_صفحہ_t::سمتیہ_t &b)
  { واپس Op{} (a, b); }
  سانچہ <قسم_نام Op>
  خالی process (ثابت Op& op,
		ثابت hb_ثنائی_ہندسہ_سیٹ_t &other,
		hb_سمتیہ_t<غیرعلامتی> *workspace = خالی_اشاریہ)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  خالی union_ (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) { process (hb_bitwise_or, other); }
  خالی intersect (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other,
		  hb_سمتیہ_t<غیرعلامتی> *workspace = خالی_اشاریہ)
  { process (hb_bitwise_and, other, workspace); }
  خالی تفریق (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) { process (hb_bitwise_gt, other); }
  خالی متشاکل_difference (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) { process (hb_bitwise_xor, other); }

  منطقی next (hb_codepoint_t *codepoint) ثابت
  {
    اگر (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      واپس *codepoint != INVALID;
    }

    ثابت خودکار* صفحہ_map_array = صفحہ_map.arrayZ;
    غیرعلامتی صحیح major = get_major (*codepoint);
    غیرعلامتی صحیح i = آخری_صفحہ_lookup;

    اگر (unlikely (i >= صفحہ_map.لمبائی || صفحہ_map_array[i].major != major))
    {
      صفحہ_map.bfind (major, &i, HB_NOT_ملا_STORE_CLOSEST);
      اگر (i >= صفحہ_map.لمبائی) {
        *codepoint = INVALID;
        واپس جھوٹ;
      }
      آخری_صفحہ_lookup = i;
    }

    ثابت خودکار* صفحات_array = صفحات.arrayZ;
    ثابت صفحہ_map_t &current = صفحہ_map_array[i];
    اگر (likely (current.major == major))
    {
      اگر (صفحات_array[current.اشاریہ_2].next (codepoint))
      {
        *codepoint += current.major * صفحہ_t::صفحہ_بٹ;
        واپس سچ;
      }
      i++;
    }

    برائے (; i < صفحہ_map.لمبائی; i++)
    {
      ثابت صفحہ_map_t &current = صفحہ_map_array[i];
      hb_codepoint_t m = صفحات_array[current.اشاریہ_2].get_min ();
      اگر (m != INVALID)
      {
	*codepoint = current.major * صفحہ_t::صفحہ_بٹ + m;
        آخری_صفحہ_lookup = i;
	واپس سچ;
      }
    }
    *codepoint = INVALID;
    واپس جھوٹ;
  }
  منطقی اگلا_بٹ (hb_codepoint_t *codepoint, uint64_t *بٹ) ثابت
  {
    static_assert (صفحہ_t::ELT_بٹ == 64, "");

    غیرعلامتی صحیح i = 0;
    غیرعلامتی صحیح elt = 0;
    اگر (likely (*codepoint != INVALID))
    {
      hb_codepoint_t بنیاد = *codepoint & ~صفحہ_t::ELT_نقاب;
      اگر (unlikely (بنیاد >= INVALID - صفحہ_t::ELT_بٹ + 1))
        جاؤ مکمل;
      بنیاد += صفحہ_t::ELT_بٹ;

      غیرعلامتی صحیح major = get_major (بنیاد);
      i = آخری_صفحہ_lookup;
      اگر (unlikely (i >= صفحہ_map.لمبائی || صفحہ_map.arrayZ[i].major != major))
      {
	صفحہ_map.bfind (major, &i, HB_NOT_ملا_STORE_CLOSEST);
	اگر (i >= صفحہ_map.لمبائی)
	  جاؤ مکمل;
      }
      اگر (صفحہ_map.arrayZ[i].major == major)
	elt = صفحہ_remainder (بنیاد) / صفحہ_t::ELT_بٹ;
    }

    برائے (; i < صفحہ_map.لمبائی; i++, elt = 0)
    {
      ثابت صفحہ_map_t &map = صفحہ_map.arrayZ[i];
      ثابت صفحہ_t &صفحہ = صفحات.arrayZ[map.اشاریہ_2];
      برائے (; elt < صفحہ_t::len (); elt++)
	اگر (صفحہ.v[elt])
	{
	  *codepoint = major_آغاز (map.major) + elt * صفحہ_t::ELT_بٹ;
	  *بٹ = صفحہ.v[elt];
	  آخری_صفحہ_lookup = i;
	  واپس سچ;
	}
    }

  مکمل:
    *codepoint = INVALID;
    *بٹ = 0;
    واپس جھوٹ;
  }
  منطقی پچھلا (hb_codepoint_t *codepoint) ثابت
  {
    اگر (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      واپس *codepoint != INVALID;
    }

    صفحہ_map_t map = {get_major (*codepoint), 0};
    غیرعلامتی صحیح i;
    صفحہ_map.bfind (map, &i, HB_NOT_ملا_STORE_CLOSEST);
    اگر (i < صفحہ_map.لمبائی && صفحہ_map.arrayZ[i].major == map.major)
    {
      اگر (صفحات[صفحہ_map.arrayZ[i].اشاریہ_2].پچھلا (codepoint))
      {
	*codepoint += صفحہ_map.arrayZ[i].major * صفحہ_t::صفحہ_بٹ;
	واپس سچ;
      }
    }
    i--;
    برائے (; (صحیح) i >= 0; i--)
    {
      hb_codepoint_t m = صفحات.arrayZ[صفحہ_map.arrayZ[i].اشاریہ_2].get_max ();
      اگر (m != INVALID)
      {
	*codepoint = صفحہ_map.arrayZ[i].major * صفحہ_t::صفحہ_بٹ + m;
	واپس سچ;
      }
    }
    *codepoint = INVALID;
    واپس جھوٹ;
  }
  منطقی اگلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  {
    hb_codepoint_t i;

    i = *آخری;
    اگر (!next (&i))
    {
      *آخری = *پہلا = INVALID;
      واپس جھوٹ;
    }

    *آخری = *پہلا = i;

    ثابت خودکار* صفحہ_map_array = صفحہ_map.arrayZ;
    ثابت خودکار* صفحات_array = صفحات.arrayZ;
    غیرعلامتی صحیح major = get_major (*آخری);
    غیرعلامتی صحیح p_اشاریہ = آخری_صفحہ_lookup;

    اگر (unlikely (p_اشاریہ >= صفحہ_map.لمبائی || صفحہ_map_array[p_اشاریہ].major != major))
    {
      صفحہ_map.bfind (major, &p_اشاریہ, HB_NOT_ملا_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      أكد(p_اشاریہ < صفحہ_map.لمبائی && صفحہ_map_array[p_اشاریہ].major == major);
      آخری_صفحہ_lookup = p_اشاریہ;
    }

    غیرعلامتی صحیح rem = صفحہ_remainder (*آخری);
    غیرعلامتی صحیح elt = rem / صفحہ_t::ELT_بٹ;
    غیرعلامتی صحیح bit = rem & صفحہ_t::ELT_نقاب;

    ثابت صفحہ_t *صفحہ = &صفحات_array[صفحہ_map_array[p_اشاریہ].اشاریہ_2];
    غیرعلامتی صحیح تسلسل = hb_ctz (~(صفحہ->v[elt] >> bit));
    *آخری += تسلسل - 1;
    اگر (تسلسل < صفحہ_t::ELT_بٹ - bit)
      واپس سچ;

    elt++;

    جب (سچ)
    {
      برائے (; elt < صفحہ_t::len (); elt++)
      {
        uint64_t w = صفحہ->v[elt];
        اگر (w == (uint64_t) -1)
        {
          *آخری += صفحہ_t::ELT_بٹ;
          جاری;
        }
        uint64_t inv = ~w;
        *آخری += hb_ctz (inv);
        واپس سچ;
      }

      p_اشاریہ++;


      اگر (p_اشاریہ >= صفحہ_map.لمبائی || صفحہ_map_array[p_اشاریہ].major != major + 1)
        واپس سچ;

      ثابت خودکار& اندراج_2 = صفحہ_map_array[p_اشاریہ];
      major = اندراج_2.major;
      آخری_صفحہ_lookup = p_اشاریہ;
      صفحہ = &صفحات_array[اندراج_2.اشاریہ_2];
      elt = 0;
    }
  }

  منطقی پچھلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  {
    hb_codepoint_t i;

    i = *پہلا;
    اگر (!پچھلا (&i))
    {
      *آخری = *پہلا = INVALID;
      واپس جھوٹ;
    }

    *آخری = *پہلا = i;

    ثابت خودکار* صفحہ_map_array = صفحہ_map.arrayZ;
    ثابت خودکار* صفحات_array = صفحات.arrayZ;
    غیرعلامتی صحیح major = get_major (*پہلا);
    غیرعلامتی صحیح p_اشاریہ = آخری_صفحہ_lookup;

    اگر (unlikely (p_اشاریہ >= صفحہ_map.لمبائی || صفحہ_map_array[p_اشاریہ].major != major))
    {
      صفحہ_map.bfind (major, &p_اشاریہ, HB_NOT_ملا_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      أكد(p_اشاریہ < صفحہ_map.لمبائی && صفحہ_map_array[p_اشاریہ].major == major);
      آخری_صفحہ_lookup = p_اشاریہ;
    }

    غیرعلامتی صحیح rem = صفحہ_remainder (*پہلا);
    صحیح elt = rem / صفحہ_t::ELT_بٹ;
    غیرعلامتی صحیح bit = rem & صفحہ_t::ELT_نقاب;

    ثابت صفحہ_t *صفحہ = &صفحات_array[صفحہ_map_array[p_اشاریہ].اشاریہ_2];
    uint64_t inv = ~(صفحہ->v[elt] << (63 - bit));
    غیرعلامتی صحیح تسلسل = hb_ثنائی_ہندسہ_storage (inv) ? (صفحہ_t::ELT_بٹ - hb_ثنائی_ہندسہ_storage (inv)) : صفحہ_t::ELT_بٹ;
    *پہلا -= تسلسل - 1;
    اگر (تسلسل < bit + 1)
      واپس سچ;

    elt--;

    جب (سچ)
    {
      برائے (; elt >= 0; elt--)
      {
        uint64_t w = صفحہ->v[elt];
        اگر (w == (uint64_t) -1)
        {
          *پہلا -= صفحہ_t::ELT_بٹ;
          جاری;
        }
        uint64_t inv = ~w;
        غیرعلامتی صحیح zeros = hb_ثنائی_ہندسہ_storage (inv) ? (صفحہ_t::ELT_بٹ - hb_ثنائی_ہندسہ_storage (inv)) : صفحہ_t::ELT_بٹ;
        *پہلا -= zeros;
        واپس سچ;
      }


      اگر (p_اشاریہ == 0 || صفحہ_map_array[p_اشاریہ - 1].major != major - 1)
        واپس سچ;

      p_اشاریہ--;
      ثابت خودکار& اندراج_2 = صفحہ_map_array[p_اشاریہ];

      major = اندراج_2.major;
      آخری_صفحہ_lookup = p_اشاریہ;
      صفحہ = &صفحات_array[اندراج_2.اشاریہ_2];
      elt = صفحہ_t::len () - 1;
    }
  }

  غیرعلامتی صحیح اگلا_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *برآمد,
			  غیرعلامتی صحیح    سائز) ثابت
  {
    // By default, start at the first bit of the first page of values.
    غیرعلامتی صحیح آغاز_صفحہ = 0;
    غیرعلامتی صحیح آغاز_صفحہ_قدر = 0;
    اگر (unlikely (codepoint != INVALID))
    {
      ثابت خودکار* صفحہ_map_array = صفحہ_map.arrayZ;
      غیرعلامتی صحیح major = get_major (codepoint);
      غیرعلامتی صحیح i = آخری_صفحہ_lookup;
      اگر (unlikely (i >= صفحہ_map.لمبائی || صفحہ_map_array[i].major != major))
      {
	صفحہ_map.bfind (major, &i, HB_NOT_ملا_STORE_CLOSEST);
	اگر (i >= صفحہ_map.لمبائی)
	  واپس 0;  // codepoint is greater than our max element.
      }
      آغاز_صفحہ = i;
      اگر (صفحہ_map_array[i].major == major)
      {
        آغاز_صفحہ_قدر = صفحہ_remainder (codepoint + 1);
        اگر (unlikely (آغاز_صفحہ_قدر == 0))
        {
          // The export-after value was last in the page. Start on next page.
          آغاز_صفحہ++;
          آغاز_صفحہ_قدر = 0;
        }
      }
    }

    غیرعلامتی صحیح initial_سائز = سائز;
    برائے (غیرعلامتی صحیح i = آغاز_صفحہ; i < صفحہ_map.لمبائی && سائز; i++)
    {
      uint32_t بنیاد = major_آغاز (صفحہ_map.arrayZ[i].major);
      غیرعلامتی صحیح n = صفحات[صفحہ_map.arrayZ[i].اشاریہ_2].اكتب (بنیاد, آغاز_صفحہ_قدر, برآمد, سائز);
      برآمد += n;
      سائز -= n;
      آغاز_صفحہ_قدر = 0;
    }
    واپس initial_سائز - سائز;
  }

  غیرعلامتی صحیح اگلا_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *برآمد,
				   غیرعلامتی صحیح    سائز) ثابت
  {
    غیرعلامتی صحیح initial_سائز = سائز;
    // By default, start at the first bit of the first page of values.
    غیرعلامتی صحیح آغاز_صفحہ = 0;
    غیرعلامتی صحیح آغاز_صفحہ_قدر = 0;
    اگر (unlikely (codepoint != INVALID))
    {
      ثابت خودکار* صفحہ_map_array = صفحہ_map.arrayZ;
      غیرعلامتی صحیح major = get_major (codepoint);
      غیرعلامتی صحیح i = آخری_صفحہ_lookup;
      اگر (unlikely (i >= صفحہ_map.لمبائی || صفحہ_map_array[i].major != major))
      {
        صفحہ_map.bfind(major, &i, HB_NOT_ملا_STORE_CLOSEST);
        اگر (unlikely (i >= صفحہ_map.لمبائی))
        {
          // codepoint is greater than our max element.
          جب (++codepoint != INVALID && سائز)
          {
            *برآمد++ = codepoint;
            سائز--;
          }
          واپس initial_سائز - سائز;
        }
      }
      آغاز_صفحہ = i;
      اگر (صفحہ_map_array[i].major == major)
      {
        آغاز_صفحہ_قدر = صفحہ_remainder (codepoint + 1);
        اگر (unlikely (آغاز_صفحہ_قدر == 0))
        {
          // The export-after value was last in the page. Start on next page.
          آغاز_صفحہ++;
          آغاز_صفحہ_قدر = 0;
        }
      }
    }

    hb_codepoint_t اگلا_قدر = codepoint + 1;
    برائے (غیرعلامتی صحیح i=آغاز_صفحہ; i<صفحہ_map.لمبائی && سائز; i++)
    {
      uint32_t بنیاد = major_آغاز (صفحہ_map.arrayZ[i].major);
      غیرعلامتی صحیح n = صفحات[صفحہ_map.arrayZ[i].اشاریہ_2].write_inverted (بنیاد, آغاز_صفحہ_قدر, برآمد, سائز, &اگلا_قدر);
      برآمد += n;
      سائز -= n;
      آغاز_صفحہ_قدر = 0;
    }
    جب (اگلا_قدر < HB_سیٹ_قدر_INVALID && سائز) {
      *برآمد++ = اگلا_قدر++;
      سائز--;
    }
    واپس initial_سائز - سائز;
  }

  منطقی has_population () ثابت { واپس population != UINT_MAX; }
  غیرعلامتی صحیح get_population () ثابت
  {
    اگر (has_population ())
      واپس population;

    غیرعلامتی صحیح نکالو = 0;
    غیرعلامتی صحیح گنتی = صفحات.لمبائی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      نکالو += صفحات[i].get_population ();

    population = نکالو;
    واپس نکالو;
  }
  منطقی get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    اگر (has_population ())
    {
      اگر (population != 1) واپس جھوٹ;
      *codepoint = get_min ();
      واپس سچ;
    }

    hb_codepoint_t singleton = INVALID;
    برائے (ثابت خودکار &map : صفحہ_map)
    {
      ثابت خودکار &صفحہ = صفحات.arrayZ[map.اشاریہ_2];
      برائے (غیرعلامتی i = 0; i < صفحہ_t::len (); i++)
      {
	صفحہ_t::elt_t بٹ = صفحہ.v[i];
	اگر (!بٹ) جاری;
	اگر (singleton != INVALID || (بٹ & (بٹ - 1)))
	  واپس جھوٹ;
	singleton = map.major * صفحہ_t::صفحہ_بٹ +
		    i * صفحہ_t::ELT_بٹ + hb_ctz (بٹ);
      }
    }

    اگر (singleton == INVALID)
    {
      population = 0;
      واپس جھوٹ;
    }
    population = 1;
    *codepoint = singleton;
    واپس سچ;
  }
  hb_codepoint_t get_min () ثابت
  {
    غیرعلامتی گنتی = صفحات.لمبائی;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      ثابت خودکار& map = صفحہ_map.arrayZ[i];
      ثابت خودکار& صفحہ = صفحات.arrayZ[map.اشاریہ_2];

      اگر (!صفحہ.is_empty ())
	واپس map.major * صفحہ_t::صفحہ_بٹ + صفحہ.get_min ();
    }
    واپس INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    غیرعلامتی گنتی = صفحات.لمبائی;
    برائے (علامتی i = گنتی - 1; i >= 0; i--)
    {
      ثابت خودکار& map = صفحہ_map.arrayZ[(غیرعلامتی) i];
      ثابت خودکار& صفحہ = صفحات.arrayZ[map.اشاریہ_2];

      اگر (!صفحہ.is_empty ())
	واپس map.major * صفحہ_t::صفحہ_بٹ + صفحہ.get_max ();
    }
    واپس INVALID;
  }

  ساکن constexpr hb_codepoint_t INVALID = صفحہ_t::INVALID;

  /*
   * Iterator implementation.
   */
  ساخت iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساکن constexpr منطقی is_sorted_iterator = سچ;
    ساکن constexpr منطقی has_fast_len = سچ;
    iter_t (ثابت hb_ثنائی_ہندسہ_سیٹ_t &s_ = Null (hb_ثنائی_ہندسہ_سیٹ_t),
	    منطقی init = سچ) : s (&s_), v (INVALID), l(0)
    {
      اگر (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    قسمتعریف hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { واپس v; }
    منطقی __more__ () ثابت { واپس v != INVALID; }
    خالی __next__ () { s->next (&v); اگر (l) l--; }
    خالی __prev__ () { s->پچھلا (&v); }
    غیرعلامتی __len__ () ثابت { واپس l; }
    iter_t انتہا () ثابت { واپس iter_t (*s, جھوٹ); }
    منطقی عامل != (ثابت iter_t& o) ثابت
    { واپس v != o.v; }

    محفوظ:
    ثابت hb_ثنائی_ہندسہ_سیٹ_t *s;
    hb_codepoint_t v;
    غیرعلامتی l;
  };
  iter_t iter () ثابت { واپس iter_t (*یہ); }
  عامل iter_t () ثابت { واپس iter (); }

  محفوظ:

  صفحہ_t *صفحہ_for (hb_codepoint_t g, منطقی insert = جھوٹ)
  {
    غیرعلامتی major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    غیرعلامتی i = آخری_صفحہ_lookup;
    اگر (likely (i < صفحہ_map.لمبائی))
    {
      خودکار &cached_صفحہ = صفحہ_map.arrayZ[i];
      اگر (cached_صفحہ.major == major)
	واپس &صفحات.arrayZ[cached_صفحہ.اشاریہ_2];
    }

    صفحہ_map_t map = {major, صفحات.لمبائی};
    اگر (!صفحہ_map.bfind (map, &i, HB_NOT_ملا_STORE_CLOSEST))
    {
      اگر (!insert)
        واپس خالی_اشاریہ;

      اگر (unlikely (!resize (صفحات.لمبائی + 1)))
	واپس خالی_اشاریہ;

      صفحات.arrayZ[map.اشاریہ_2].init0 ();
      memmove (صفحہ_map.arrayZ + i + 1,
	       صفحہ_map.arrayZ + i,
	       (صفحہ_map.لمبائی - 1 - i) * صفحہ_map.شے_سائز);
      صفحہ_map.arrayZ[i] = map;
    }

    آخری_صفحہ_lookup = i;
    واپس &صفحات.arrayZ[صفحہ_map.arrayZ[i].اشاریہ_2];
  }
  ثابت صفحہ_t *صفحہ_for (hb_codepoint_t g) ثابت
  {
    غیرعلامتی major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    غیرعلامتی i = آخری_صفحہ_lookup;
    اگر (likely (i < صفحہ_map.لمبائی))
    {
      خودکار &cached_صفحہ = صفحہ_map.arrayZ[i];
      اگر (cached_صفحہ.major == major)
	واپس &صفحات.arrayZ[cached_صفحہ.اشاریہ_2];
    }

    صفحہ_map_t key = {major};
    اگر (!صفحہ_map.bfind (key, &i))
      واپس خالی_اشاریہ;

    آخری_صفحہ_lookup = i;
    واپس &صفحات.arrayZ[صفحہ_map.arrayZ[i].اشاریہ_2];
  }
  صفحہ_t &صفحہ_at (غیرعلامتی صحیح i)
  {
    أكد (i < صفحہ_map.لمبائی);
    واپس صفحات.arrayZ[صفحہ_map.arrayZ[i].اشاریہ_2];
  }
  ثابت صفحہ_t &صفحہ_at (غیرعلامتی صحیح i) ثابت
  {
    أكد (i < صفحہ_map.لمبائی);
    واپس صفحات.arrayZ[صفحہ_map.arrayZ[i].اشاریہ_2];
  }
  غیرعلامتی صحیح get_major (hb_codepoint_t g) ثابت { واپس g >> صفحہ_t::صفحہ_بٹ_LOG_2; }
  غیرعلامتی صحیح صفحہ_remainder (hb_codepoint_t g) ثابت { واپس g & صفحہ_t::صفحہ_BITMASK; }
  hb_codepoint_t major_آغاز (غیرعلامتی صحیح major) ثابت { واپس major << صفحہ_t::صفحہ_بٹ_LOG_2; }
};


#اگر_ختم /* HB_BIT_SET_HH */
