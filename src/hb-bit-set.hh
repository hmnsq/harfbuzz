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

#جے_بنا_تعریف HB_ثنائی_ہندسہ_سیٹ_HH
#تعریف_کرو HB_ثنائی_ہندسہ_سیٹ_HH

#ضمن "hb.hh"
#ضمن "hb-bit-page.hh"

بنية hb_ثنائی_ہندسہ_سیٹ_t
{
  hb_ثنائی_ہندسہ_سیٹ_t () = افتراضي;
  ~hb_ثنائی_ہندسہ_سیٹ_t () = افتراضي;

  hb_ثنائی_ہندسہ_سیٹ_t (ثابت hb_ثنائی_ہندسہ_سیٹ_t& other) : hb_ثنائی_ہندسہ_سیٹ_t () { سیٹ (other, سچ); }
  hb_ثنائی_ہندسہ_سیٹ_t ( hb_ثنائی_ہندسہ_سیٹ_t&& other)  noexcept : hb_ثنائی_ہندسہ_سیٹ_t () { hb_وٹاندرا (*هذا, other); }
  hb_ثنائی_ہندسہ_سیٹ_t& عامل= (ثابت hb_ثنائی_ہندسہ_سیٹ_t& other) { سیٹ (other); أرجع *هذا; }
  hb_ثنائی_ہندسہ_سیٹ_t& عامل= (hb_ثنائی_ہندسہ_سیٹ_t&& other)  noexcept { hb_وٹاندرا (*هذا, other); أرجع *هذا; }
  یار فراغ swap (hb_ثنائی_ہندسہ_سیٹ_t &a, hb_ثنائی_ہندسہ_سیٹ_t &b) noexcept
  {
    إذا (likely (!a.successful || !b.successful))
      أرجع;
    hb_وٹاندرا (a.population, b.population);
    hb_وٹاندرا (a.آخری_ورقہ_lookup, b.آخری_ورقہ_lookup);
    hb_وٹاندرا (a.ورقہ_map, b.ورقہ_map);
    hb_وٹاندرا (a.ورقے, b.ورقے);
  }

  فراغ init ()
  {
    successful = سچ;
    population = 0;
    آخری_ورقہ_lookup = 0;
    ورقہ_map.init ();
    ورقے.init ();
  }
  فراغ fini ()
  {
    ورقہ_map.fini ();
    ورقے.fini ();
  }

  ورتوں ورقہ_t = hb_ثنائی_ہندسہ_ورقہ_t;
  بنية ورقہ_map_t
  {
    صحيح cmp (ثابت ورقہ_map_t &o) ثابت { أرجع cmp (o.major); }
    صحيح cmp (uint32_t o_major) ثابت { أرجع (صحيح) o_major - (صحيح) major; }

    uint32_t major;
    uint32_t اشاریہ_2;
  };

  منطقي successful = سچ; /* Allocations successful */
  بدلنجوگ بنانشانی صحيح population = 0;
  بدلنجوگ hb_atomic_t<بنانشانی> آخری_ورقہ_lookup = 0;
  hb_sorted_سمتیہ_t<ورقہ_map_t> ورقہ_map;
  hb_سمتیہ_t<ورقہ_t> ورقے;

  فراغ err () { إذا (successful) successful = خطأ; } /* TODO Remove */
  منطقي وچ_error () ثابت { أرجع !successful; }

  منطقي resize (بنانشانی صحيح گنتی, منطقي clear = سچ, منطقي exact_ناپ = خطأ)
  {
    إذا (unlikely (!successful)) أرجع خطأ;

    إذا (ورقے.لمائی < گنتی && (بنانشانی) ورقے.allocated < گنتی && گنتی <= 2)
      exact_ناپ = سچ; // Most sets are small and local

    إذا (unlikely (!ورقے.resize_بھریا (گنتی, clear, exact_ناپ) ||
	!ورقہ_map.resize_بھریا (گنتی, clear, خطأ)))
    {
      ورقے.resize_بھریا (ورقہ_map.لمائی, clear, exact_ناپ);
      successful = خطأ;
      أرجع خطأ;
    }
    أرجع سچ;
  }

  فراغ alloc (بنانشانی sz)
  {
    sz >>= (ورقہ_t::ورقہ_بٹ_LOG_2 - 1);
    ورقے.alloc (sz);
    ورقہ_map.alloc (sz);
  }

  hb_ثنائی_ہندسہ_سیٹ_t& reset ()
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
    إذا (!ورقے.لمائی && !population) أرجع;
    ورقے.clear ();
    ورقہ_map.clear ();
    population = 0;
  }
  منطقي is_empty () ثابت
  {
    بنانشانی صحيح گنتی = ورقے.لمائی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (!ورقے[i].is_empty ())
	أرجع خطأ;
    أرجع سچ;
  }
  واضح عامل منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    uint32_t h = 0;
    لكل (خودکار &map : ورقہ_map)
    {
      خودکار &ورقہ = ورقے.arrayZ[map.اشاریہ_2];
      إذا (unlikely (ورقہ.is_empty ())) استمر;
      h = h * 31 + hb_hash (map.major) + hb_hash (ورقہ);
    }
    أرجع h;
  }

  خاص:
  فراغ dirty () { population = UINT_MAX; }
  عام:

  فراغ جوڑو (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (unlikely (g == INVALID)) أرجع;
    dirty ();
    ورقہ_t *ورقہ = ورقہ_for (g, سچ); إذا (unlikely (!ورقہ)) أرجع;
    ورقہ->جوڑو (g);
  }
  فراغ جوڑو_بٹ (hb_codepoint_t g, uint64_t بٹ)
  {
    إذا (unlikely (!successful) || unlikely (!بٹ)) أرجع;
    أكد (!(g & (ورقہ_t::ELT_بٹ - 1)));
    dirty ();
    ورقہ_t *ورقہ = ورقہ_for (g, سچ); إذا (unlikely (!ورقہ)) أرجع;
    ورقہ->جوڑو_بٹ (g, بٹ);
  }
  منطقي جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!successful)) أرجع سچ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (a > b || a == INVALID || b == INVALID)) أرجع خطأ;
    dirty ();
    بنانشانی صحيح ma = get_major (a);
    بنانشانی صحيح mb = get_major (b);
    إذا (ma == mb)
    {
      ورقہ_t *ورقہ = ورقہ_for (a, سچ); إذا (unlikely (!ورقہ)) أرجع خطأ;
      ورقہ->جوڑو_range (a, b);
    }
    وإلا
    {
      ورقہ_t *ورقہ = ورقہ_for (a, سچ); إذا (unlikely (!ورقہ)) أرجع خطأ;
      ورقہ->جوڑو_range (a, major_شروع (ma + 1) - 1);

      لكل (بنانشانی صحيح m = ma + 1; m < mb; m++)
      {
	ورقہ = ورقہ_for (major_شروع (m), سچ); إذا (unlikely (!ورقہ)) أرجع خطأ;
	ورقہ->init1 ();
      }

      ورقہ = ورقہ_for (b, سچ); إذا (unlikely (!ورقہ)) أرجع خطأ;
      ورقہ->جوڑو_range (major_شروع (mb), b);
    }
    أرجع سچ;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  سانچا<قسم_ناں Type>
  ساكن مضمن ثابت Type& Struct_At_وتھ_Unaligned(ثابت فراغ *P, بنانشانی صحيح وتھ)
  {
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-align"
    أرجع * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + وتھ);
#ہدایت GCC diagnostic نکالو
  }

  سانچا <قسم_ناں T>
  فراغ سیٹ_array (منطقي v, ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (!گنتی) أرجع;
    dirty ();
    hb_codepoint_t g = *array;
    طالما (گنتی)
    {
      بنانشانی صحيح m = get_major (g);
      ورقہ_t *ورقہ = ورقہ_for (g, v); إذا (unlikely (v && !ورقہ)) أرجع;
      بنانشانی صحيح شروع = major_شروع (m);
      بنانشانی صحيح سرا = major_شروع (m + 1);
      افعل
      {
        إذا (g != INVALID && (v || ورقہ)) /* The v check is to optimize out the page check if v is true. */
	  ورقہ->سیٹ (g, v);

	array = &Struct_At_وتھ_Unaligned<T> (array, stride);
	گنتی--;
      }
      طالما (گنتی && (g = *array, شروع <= g && g < سرا));
    }
  }

  سانچا <قسم_ناں T>
  فراغ جوڑو_array (ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  { سیٹ_array (سچ, array, گنتی, stride); }
  سانچا <قسم_ناں T>
  فراغ جوڑو_array (ثابت hb_array_t<ثابت T>& arr) { جوڑو_array (&arr, arr.len ()); }

  سانچا <قسم_ناں T>
  فراغ del_array (ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  { سیٹ_array (خطأ, array, گنتی, stride); }
  سانچا <قسم_ناں T>
  فراغ del_array (ثابت hb_array_t<ثابت T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچا <قسم_ناں T>
  منطقي سیٹ_sorted_array (منطقي v, ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع سچ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (!گنتی)) أرجع سچ;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t آخری_g = g;
    طالما (گنتی)
    {
      بنانشانی صحيح m = get_major (g);
      ورقہ_t *ورقہ = ورقہ_for (g, v); إذا (unlikely (v && !ورقہ)) أرجع خطأ;
      بنانشانی صحيح سرا = major_شروع (m + 1);
      افعل
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	إذا (g < آخری_g) أرجع خطأ;
	آخری_g = g;

        إذا (g != INVALID && (v || ورقہ)) /* The v check is to optimize out the page check if v is true. */
	  ورقہ->سیٹ (g, v);

	array = &Struct_At_وتھ_Unaligned<T> (array, stride);
	گنتی--;
      }
      طالما (گنتی && (g = *array, g < سرا));
    }
    أرجع سچ;
  }

  سانچا <قسم_ناں T>
  منطقي جوڑو_sorted_array (ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  { أرجع سیٹ_sorted_array (سچ, array, گنتی, stride); }
  سانچا <قسم_ناں T>
  منطقي جوڑو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جوڑو_sorted_array (&arr, arr.len ()); }

  سانچا <قسم_ناں T>
  منطقي del_sorted_array (ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  { أرجع سیٹ_sorted_array (خطأ, array, گنتی, stride); }
  سانچا <قسم_ناں T>
  منطقي del_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع del_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    ورقہ_t *ورقہ = ورقہ_for (g);
    إذا (!ورقہ)
      أرجع;
    dirty ();
    ورقہ->del (g);
  }
  فراغ del_بٹ (hb_codepoint_t g, uint64_t بٹ)
  {
    إذا (unlikely (!successful) || unlikely (!بٹ)) أرجع;
    أكد (!(g & (ورقہ_t::ELT_بٹ - 1)));
    ورقہ_t *ورقہ = ورقہ_for (g);
    إذا (!ورقہ)
      أرجع;
    dirty ();
    ورقہ->del_بٹ (g, بٹ);
  }

  خاص:
  فراغ del_ورقے (صحيح ds, صحيح de)
  {
    إذا (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_سمتیہ_t<بنانشانی> compact_workspace;
      إذا (unlikely (!allocate_compact_workspace (compact_workspace))) أرجع;

      بنانشانی صحيح write_اشاریہ = 0;
      لكل (بنانشانی صحيح i = 0; i < ورقہ_map.لمائی; i++)
      {
	صحيح m = (صحيح) ورقہ_map.arrayZ[i].major;
	إذا (m < ds || de < m)
	  ورقہ_map.arrayZ[write_اشاریہ++] = ورقہ_map.arrayZ[i];
      }
      compact (compact_workspace, write_اشاریہ);
      resize (write_اشاریہ);
    }
  }


  عام:
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (unlikely (a > b || a == INVALID)) أرجع;
    dirty ();
    بنانشانی صحيح ma = get_major (a);
    بنانشانی صحيح mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    صحيح ds = (a == major_شروع (ma))? (صحيح) ma: (صحيح) (ma + 1);
    صحيح de = (b + 1 == major_شروع (mb + 1))? (صحيح) mb: ((صحيح) mb - 1);
    إذا (ds > de || (صحيح) ma < ds)
    {
      ورقہ_t *ورقہ = ورقہ_for (a);
      إذا (ورقہ)
      {
	إذا (ma == mb)
	  ورقہ->del_range (a, b);
	وإلا
	  ورقہ->del_range (a, major_شروع (ma + 1) - 1);
      }
    }
    إذا (de < (صحيح) mb && ma != mb)
    {
      ورقہ_t *ورقہ = ورقہ_for (b);
      إذا (ورقہ)
	ورقہ->del_range (major_شروع (mb), b);
    }
    del_ورقے (ds, de);
  }

  منطقي get (hb_codepoint_t g) ثابت
  {
    ثابت ورقہ_t *ورقہ = ورقہ_for (g);
    إذا (!ورقہ)
      أرجع خطأ;
    أرجع ورقہ->get (g);
  }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي عامل [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي عامل () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_ثنائی_ہندسہ_سیٹ_t& عامل << (hb_codepoint_t v)
  { جوڑو (v); أرجع *هذا; }
  hb_ثنائی_ہندسہ_سیٹ_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { جوڑو_range (range.پہلا, range.second); أرجع *هذا; }

  منطقي intersects (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) ثابت
  {
    بنانشانی صحيح na = ورقے.لمائی;
    بنانشانی صحيح nb = other.ورقے.لمائی;

    بنانشانی صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (ورقہ_map.arrayZ[a].major == other.ورقہ_map.arrayZ[b].major)
      {
	إذا (ورقہ_at (a).intersects (other.ورقہ_at (b)))
	  أرجع سچ;
	a++;
	b++;
      }
      وإلا إذا (ورقہ_map.arrayZ[a].major < other.ورقہ_map.arrayZ[b].major)
	a++;
      وإلا
	b++;
    }
    أرجع خطأ;
  }
  منطقي may_intersect (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) ثابت
  { أرجع intersects (other); }

  منطقي intersects (hb_codepoint_t پہلا, hb_codepoint_t آخری) ثابت
  {
    hb_codepoint_t c = پہلا - 1;
    أرجع next (&c) && c <= آخری;
  }
  فراغ سیٹ (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other, منطقي exact_ناپ = خطأ)
  {
    إذا (unlikely (!successful)) أرجع;
    بنانشانی صحيح گنتی = other.ورقے.لمائی;
    إذا (unlikely (!resize  (گنتی, خطأ, exact_ناپ)))
      أرجع;
    population = other.population;

    ورقہ_map = other.ورقہ_map;
    ورقے = other.ورقے;
  }

  منطقي is_equal (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) ثابت
  {
    إذا (has_population () && other.has_population () &&
	population != other.population)
      أرجع خطأ;

    بنانشانی صحيح na = ورقے.لمائی;
    بنانشانی صحيح nb = other.ورقے.لمائی;

    بنانشانی صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (ورقہ_at (a).is_empty ()) { a++; استمر; }
      إذا (other.ورقہ_at (b).is_empty ()) { b++; استمر; }
      إذا (ورقہ_map.arrayZ[a].major != other.ورقہ_map.arrayZ[b].major ||
	  !ورقہ_at (a).is_equal (other.ورقہ_at (b)))
	أرجع خطأ;
      a++;
      b++;
    }
    لكل (; a < na; a++)
      إذا (!ورقہ_at (a).is_empty ()) { أرجع خطأ; }
    لكل (; b < nb; b++)
      إذا (!other.ورقہ_at (b).is_empty ()) { أرجع خطأ; }

    أرجع سچ;
  }

  منطقي is_subset (ثابت hb_ثنائی_ہندسہ_سیٹ_t &larger_سیٹ) ثابت
  {
    إذا (has_population () && larger_سیٹ.has_population () &&
	population > larger_سیٹ.population)
      أرجع خطأ;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    طالما (spi < ورقہ_map.لمائی && lpi < larger_سیٹ.ورقہ_map.لمائی)
    {
      uint32_t spm = ورقہ_map.arrayZ[spi].major;
      uint32_t lpm = larger_سیٹ.ورقہ_map.arrayZ[lpi].major;
      خودکار sp = ورقہ_at (spi);

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

      خودکار lp = larger_سیٹ.ورقہ_at (lpi);
      إذا (!sp.is_subset (lp))
        أرجع خطأ;

      spi++;
      lpi++;
    }

    طالما (spi < ورقہ_map.لمائی)
      إذا (!ورقہ_at (spi++).is_empty ())
        أرجع خطأ;

    أرجع سچ;
  }

  خاص:
  منطقي allocate_compact_workspace (hb_سمتیہ_t<بنانشانی>& workspace)
  {
    إذا (unlikely (!workspace.resize_exact (ورقے.لمائی)))
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
  فراغ compact (hb_سمتیہ_t<بنانشانی>& workspace,
                بنانشانی صحيح لمائی)
  {
    أكد(workspace.لمائی == ورقے.لمائی);
    hb_سمتیہ_t<بنانشانی>& old_اشاریہ_منزل_ورقہ_map_اشاریہ = workspace;

    hb_fill (old_اشاریہ_منزل_ورقہ_map_اشاریہ.writer(), 0xFFFFFFFF);
    لكل (بنانشانی i = 0; i < لمائی; i++)
      old_اشاریہ_منزل_ورقہ_map_اشاریہ[ورقہ_map[i].اشاریہ_2] =  i;

    compact_ورقے (old_اشاریہ_منزل_ورقہ_map_اشاریہ);
  }
  فراغ compact_ورقے (ثابت hb_سمتیہ_t<بنانشانی>& old_اشاریہ_منزل_ورقہ_map_اشاریہ)
  {
    بنانشانی صحيح write_اشاریہ = 0;
    لكل (بنانشانی صحيح i = 0; i < ورقے.لمائی; i++)
    {
      إذا (old_اشاریہ_منزل_ورقہ_map_اشاریہ[i] == 0xFFFFFFFF) استمر;

      إذا (write_اشاریہ < i)
	ورقے[write_اشاریہ] = ورقے[i];

      ورقہ_map[old_اشاریہ_منزل_ورقہ_map_اشاریہ[i]].اشاریہ_2 = write_اشاریہ;
      write_اشاریہ++;
    }
  }
  عام:

  فراغ process_ (hb_ثنائی_ہندسہ_ورقہ_t::سمتیہ_t (*op) (ثابت hb_ثنائی_ہندسہ_ورقہ_t::سمتیہ_t &, ثابت hb_ثنائی_ہندسہ_ورقہ_t::سمتیہ_t &),
		 منطقي passthru_بایاں, منطقي passthru_دایاں,
		 ثابت hb_ثنائی_ہندسہ_سیٹ_t &other,
		 hb_سمتیہ_t<بنانشانی> *workspace = خالی_سوچک)
  {
    إذا (unlikely (!successful)) أرجع;

    dirty ();

    بنانشانی صحيح na = ورقے.لمائی;
    بنانشانی صحيح nb = other.ورقے.لمائی;
    بنانشانی صحيح اگلا_ورقہ = na;

    بنانشانی صحيح گنتی = 0, new_گنتی = 0;
    بنانشانی صحيح a = 0, b = 0;
    بنانشانی صحيح write_اشاریہ = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_سمتیہ_t<بنانشانی> local_workspace;
    hb_سمتیہ_t<بنانشانی> &compact_workspace = workspace ? *workspace : local_workspace;
    إذا (!passthru_بایاں)
    {
      منطقي allocated = workspace ? compact_workspace.resize (ورقے.لمائی)
				 : allocate_compact_workspace (compact_workspace);
      إذا (unlikely (!allocated))
      {
	successful = خطأ;
	أرجع;
      }
    }

    لكل (; a < na && b < nb; )
    {
      إذا (ورقہ_map.arrayZ[a].major == other.ورقہ_map.arrayZ[b].major)
      {
	إذا (!passthru_بایاں)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  إذا (write_اشاریہ < a)
	    ورقہ_map.arrayZ[write_اشاریہ] = ورقہ_map.arrayZ[a];
	  write_اشاریہ++;
	}

	گنتی++;
	a++;
	b++;
      }
      وإلا إذا (ورقہ_map.arrayZ[a].major < other.ورقہ_map.arrayZ[b].major)
      {
	إذا (passthru_بایاں)
	  گنتی++;
	a++;
      }
      وإلا
      {
	إذا (passthru_دایاں)
	  گنتی++;
	b++;
      }
    }
    إذا (passthru_بایاں)
      گنتی += na - a;
    إذا (passthru_دایاں)
      گنتی += nb - b;

    إذا (!passthru_بایاں)
    {
      na  = write_اشاریہ;
      اگلا_ورقہ = write_اشاریہ;
      compact (compact_workspace, write_اشاریہ);
    }

    إذا (unlikely (!resize (گنتی)))
      أرجع;

    new_گنتی = گنتی;

    /* Process in-place backward. */
    a = na;
    b = nb;
    لكل (; a && b; )
    {
      إذا (ورقہ_map.arrayZ[a - 1].major == other.ورقہ_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	گنتی--;
	ورقہ_map.arrayZ[گنتی] = ورقہ_map.arrayZ[a];
	ورقہ_at (گنتی).v = op (ورقہ_at (a).v, other.ورقہ_at (b).v);
	ورقہ_at (گنتی).dirty ();
      }
      وإلا إذا (ورقہ_map.arrayZ[a - 1].major > other.ورقہ_map.arrayZ[b - 1].major)
      {
	a--;
	إذا (passthru_بایاں)
	{
	  گنتی--;
	  ورقہ_map.arrayZ[گنتی] = ورقہ_map.arrayZ[a];
	}
      }
      وإلا
      {
	b--;
	إذا (passthru_دایاں)
	{
	  گنتی--;
	  ورقہ_map.arrayZ[گنتی].major = other.ورقہ_map.arrayZ[b].major;
	  ورقہ_map.arrayZ[گنتی].اشاریہ_2 = اگلا_ورقہ++;
	  ورقہ_at (گنتی) = other.ورقہ_at (b);
	}
      }
    }
    إذا (passthru_بایاں)
      طالما (a)
      {
	a--;
	گنتی--;
	ورقہ_map.arrayZ[گنتی] = ورقہ_map.arrayZ[a];
      }
    إذا (passthru_دایاں)
      طالما (b)
      {
	b--;
	گنتی--;
	ورقہ_map.arrayZ[گنتی].major = other.ورقہ_map.arrayZ[b].major;
	ورقہ_map.arrayZ[گنتی].اشاریہ_2 = اگلا_ورقہ++;
	ورقہ_at (گنتی) = other.ورقہ_at (b);
      }
    أكد (!گنتی);
    resize (new_گنتی);
  }
  سانچا <قسم_ناں Op>
  ساكن hb_ثنائی_ہندسہ_ورقہ_t::سمتیہ_t
  op_ (ثابت hb_ثنائی_ہندسہ_ورقہ_t::سمتیہ_t &a, ثابت hb_ثنائی_ہندسہ_ورقہ_t::سمتیہ_t &b)
  { أرجع Op{} (a, b); }
  سانچا <قسم_ناں Op>
  فراغ process (ثابت Op& op,
		ثابت hb_ثنائی_ہندسہ_سیٹ_t &other,
		hb_سمتیہ_t<بنانشانی> *workspace = خالی_سوچک)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  فراغ union_ (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) { process (hb_bitwise_or, other); }
  فراغ intersect (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other,
		  hb_سمتیہ_t<بنانشانی> *workspace = خالی_سوچک)
  { process (hb_bitwise_and, other, workspace); }
  فراغ گھٹاؤ (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) { process (hb_bitwise_gt, other); }
  فراغ متشاکل_difference (ثابت hb_ثنائی_ہندسہ_سیٹ_t &other) { process (hb_bitwise_xor, other); }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      أرجع *codepoint != INVALID;
    }

    ثابت خودکار* ورقہ_map_array = ورقہ_map.arrayZ;
    بنانشانی صحيح major = get_major (*codepoint);
    بنانشانی صحيح i = آخری_ورقہ_lookup;

    إذا (unlikely (i >= ورقہ_map.لمائی || ورقہ_map_array[i].major != major))
    {
      ورقہ_map.bfind (major, &i, HB_NOT_لبھیا_STORE_CLOSEST);
      إذا (i >= ورقہ_map.لمائی) {
        *codepoint = INVALID;
        أرجع خطأ;
      }
      آخری_ورقہ_lookup = i;
    }

    ثابت خودکار* ورقے_array = ورقے.arrayZ;
    ثابت ورقہ_map_t &current = ورقہ_map_array[i];
    إذا (likely (current.major == major))
    {
      إذا (ورقے_array[current.اشاریہ_2].next (codepoint))
      {
        *codepoint += current.major * ورقہ_t::ورقہ_بٹ;
        أرجع سچ;
      }
      i++;
    }

    لكل (; i < ورقہ_map.لمائی; i++)
    {
      ثابت ورقہ_map_t &current = ورقہ_map_array[i];
      hb_codepoint_t m = ورقے_array[current.اشاریہ_2].get_min ();
      إذا (m != INVALID)
      {
	*codepoint = current.major * ورقہ_t::ورقہ_بٹ + m;
        آخری_ورقہ_lookup = i;
	أرجع سچ;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي اگلا_بٹ (hb_codepoint_t *codepoint, uint64_t *بٹ) ثابت
  {
    static_assert (ورقہ_t::ELT_بٹ == 64, "");

    بنانشانی صحيح i = 0;
    بنانشانی صحيح elt = 0;
    إذا (likely (*codepoint != INVALID))
    {
      hb_codepoint_t ادھار = *codepoint & ~ورقہ_t::ELT_نقاب;
      إذا (unlikely (ادھار >= INVALID - ورقہ_t::ELT_بٹ + 1))
        اذهب پورا;
      ادھار += ورقہ_t::ELT_بٹ;

      بنانشانی صحيح major = get_major (ادھار);
      i = آخری_ورقہ_lookup;
      إذا (unlikely (i >= ورقہ_map.لمائی || ورقہ_map.arrayZ[i].major != major))
      {
	ورقہ_map.bfind (major, &i, HB_NOT_لبھیا_STORE_CLOSEST);
	إذا (i >= ورقہ_map.لمائی)
	  اذهب پورا;
      }
      إذا (ورقہ_map.arrayZ[i].major == major)
	elt = ورقہ_remainder (ادھار) / ورقہ_t::ELT_بٹ;
    }

    لكل (; i < ورقہ_map.لمائی; i++, elt = 0)
    {
      ثابت ورقہ_map_t &map = ورقہ_map.arrayZ[i];
      ثابت ورقہ_t &ورقہ = ورقے.arrayZ[map.اشاریہ_2];
      لكل (; elt < ورقہ_t::len (); elt++)
	إذا (ورقہ.v[elt])
	{
	  *codepoint = major_شروع (map.major) + elt * ورقہ_t::ELT_بٹ;
	  *بٹ = ورقہ.v[elt];
	  آخری_ورقہ_lookup = i;
	  أرجع سچ;
	}
    }

  پورا:
    *codepoint = INVALID;
    *بٹ = 0;
    أرجع خطأ;
  }
  منطقي پچھلا (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      أرجع *codepoint != INVALID;
    }

    ورقہ_map_t map = {get_major (*codepoint), 0};
    بنانشانی صحيح i;
    ورقہ_map.bfind (map, &i, HB_NOT_لبھیا_STORE_CLOSEST);
    إذا (i < ورقہ_map.لمائی && ورقہ_map.arrayZ[i].major == map.major)
    {
      إذا (ورقے[ورقہ_map.arrayZ[i].اشاریہ_2].پچھلا (codepoint))
      {
	*codepoint += ورقہ_map.arrayZ[i].major * ورقہ_t::ورقہ_بٹ;
	أرجع سچ;
      }
    }
    i--;
    لكل (; (صحيح) i >= 0; i--)
    {
      hb_codepoint_t m = ورقے.arrayZ[ورقہ_map.arrayZ[i].اشاریہ_2].get_max ();
      إذا (m != INVALID)
      {
	*codepoint = ورقہ_map.arrayZ[i].major * ورقہ_t::ورقہ_بٹ + m;
	أرجع سچ;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي اگلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  {
    hb_codepoint_t i;

    i = *آخری;
    إذا (!next (&i))
    {
      *آخری = *پہلا = INVALID;
      أرجع خطأ;
    }

    *آخری = *پہلا = i;

    ثابت خودکار* ورقہ_map_array = ورقہ_map.arrayZ;
    ثابت خودکار* ورقے_array = ورقے.arrayZ;
    بنانشانی صحيح major = get_major (*آخری);
    بنانشانی صحيح p_اشاریہ = آخری_ورقہ_lookup;

    إذا (unlikely (p_اشاریہ >= ورقہ_map.لمائی || ورقہ_map_array[p_اشاریہ].major != major))
    {
      ورقہ_map.bfind (major, &p_اشاریہ, HB_NOT_لبھیا_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      أكد(p_اشاریہ < ورقہ_map.لمائی && ورقہ_map_array[p_اشاریہ].major == major);
      آخری_ورقہ_lookup = p_اشاریہ;
    }

    بنانشانی صحيح rem = ورقہ_remainder (*آخری);
    بنانشانی صحيح elt = rem / ورقہ_t::ELT_بٹ;
    بنانشانی صحيح bit = rem & ورقہ_t::ELT_نقاب;

    ثابت ورقہ_t *ورقہ = &ورقے_array[ورقہ_map_array[p_اشاریہ].اشاریہ_2];
    بنانشانی صحيح لگاتار = hb_ctz (~(ورقہ->v[elt] >> bit));
    *آخری += لگاتار - 1;
    إذا (لگاتار < ورقہ_t::ELT_بٹ - bit)
      أرجع سچ;

    elt++;

    طالما (سچ)
    {
      لكل (; elt < ورقہ_t::len (); elt++)
      {
        uint64_t w = ورقہ->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *آخری += ورقہ_t::ELT_بٹ;
          استمر;
        }
        uint64_t inv = ~w;
        *آخری += hb_ctz (inv);
        أرجع سچ;
      }

      p_اشاریہ++;


      إذا (p_اشاریہ >= ورقہ_map.لمائی || ورقہ_map_array[p_اشاریہ].major != major + 1)
        أرجع سچ;

      ثابت خودکار& اندراج = ورقہ_map_array[p_اشاریہ];
      major = اندراج.major;
      آخری_ورقہ_lookup = p_اشاریہ;
      ورقہ = &ورقے_array[اندراج.اشاریہ_2];
      elt = 0;
    }
  }

  منطقي پچھلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  {
    hb_codepoint_t i;

    i = *پہلا;
    إذا (!پچھلا (&i))
    {
      *آخری = *پہلا = INVALID;
      أرجع خطأ;
    }

    *آخری = *پہلا = i;

    ثابت خودکار* ورقہ_map_array = ورقہ_map.arrayZ;
    ثابت خودکار* ورقے_array = ورقے.arrayZ;
    بنانشانی صحيح major = get_major (*پہلا);
    بنانشانی صحيح p_اشاریہ = آخری_ورقہ_lookup;

    إذا (unlikely (p_اشاریہ >= ورقہ_map.لمائی || ورقہ_map_array[p_اشاریہ].major != major))
    {
      ورقہ_map.bfind (major, &p_اشاریہ, HB_NOT_لبھیا_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      أكد(p_اشاریہ < ورقہ_map.لمائی && ورقہ_map_array[p_اشاریہ].major == major);
      آخری_ورقہ_lookup = p_اشاریہ;
    }

    بنانشانی صحيح rem = ورقہ_remainder (*پہلا);
    صحيح elt = rem / ورقہ_t::ELT_بٹ;
    بنانشانی صحيح bit = rem & ورقہ_t::ELT_نقاب;

    ثابت ورقہ_t *ورقہ = &ورقے_array[ورقہ_map_array[p_اشاریہ].اشاریہ_2];
    uint64_t inv = ~(ورقہ->v[elt] << (63 - bit));
    بنانشانی صحيح لگاتار = hb_ثنائی_ہندسہ_storage (inv) ? (ورقہ_t::ELT_بٹ - hb_ثنائی_ہندسہ_storage (inv)) : ورقہ_t::ELT_بٹ;
    *پہلا -= لگاتار - 1;
    إذا (لگاتار < bit + 1)
      أرجع سچ;

    elt--;

    طالما (سچ)
    {
      لكل (; elt >= 0; elt--)
      {
        uint64_t w = ورقہ->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *پہلا -= ورقہ_t::ELT_بٹ;
          استمر;
        }
        uint64_t inv = ~w;
        بنانشانی صحيح zeros = hb_ثنائی_ہندسہ_storage (inv) ? (ورقہ_t::ELT_بٹ - hb_ثنائی_ہندسہ_storage (inv)) : ورقہ_t::ELT_بٹ;
        *پہلا -= zeros;
        أرجع سچ;
      }


      إذا (p_اشاریہ == 0 || ورقہ_map_array[p_اشاریہ - 1].major != major - 1)
        أرجع سچ;

      p_اشاریہ--;
      ثابت خودکار& اندراج = ورقہ_map_array[p_اشاریہ];

      major = اندراج.major;
      آخری_ورقہ_lookup = p_اشاریہ;
      ورقہ = &ورقے_array[اندراج.اشاریہ_2];
      elt = ورقہ_t::len () - 1;
    }
  }

  بنانشانی صحيح اگلا_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *باہرلا,
			  بنانشانی صحيح    ناپ) ثابت
  {
    // By default, start at the first bit of the first page of values.
    بنانشانی صحيح شروع_ورقہ = 0;
    بنانشانی صحيح شروع_ورقہ_قدر = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت خودکار* ورقہ_map_array = ورقہ_map.arrayZ;
      بنانشانی صحيح major = get_major (codepoint);
      بنانشانی صحيح i = آخری_ورقہ_lookup;
      إذا (unlikely (i >= ورقہ_map.لمائی || ورقہ_map_array[i].major != major))
      {
	ورقہ_map.bfind (major, &i, HB_NOT_لبھیا_STORE_CLOSEST);
	إذا (i >= ورقہ_map.لمائی)
	  أرجع 0;  // codepoint is greater than our max element.
      }
      شروع_ورقہ = i;
      إذا (ورقہ_map_array[i].major == major)
      {
        شروع_ورقہ_قدر = ورقہ_remainder (codepoint + 1);
        إذا (unlikely (شروع_ورقہ_قدر == 0))
        {
          // The export-after value was last in the page. Start on next page.
          شروع_ورقہ++;
          شروع_ورقہ_قدر = 0;
        }
      }
    }

    بنانشانی صحيح initial_ناپ = ناپ;
    لكل (بنانشانی صحيح i = شروع_ورقہ; i < ورقہ_map.لمائی && ناپ; i++)
    {
      uint32_t ادھار = major_شروع (ورقہ_map.arrayZ[i].major);
      بنانشانی صحيح n = ورقے[ورقہ_map.arrayZ[i].اشاریہ_2].اكتب (ادھار, شروع_ورقہ_قدر, باہرلا, ناپ);
      باہرلا += n;
      ناپ -= n;
      شروع_ورقہ_قدر = 0;
    }
    أرجع initial_ناپ - ناپ;
  }

  بنانشانی صحيح اگلا_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *باہرلا,
				   بنانشانی صحيح    ناپ) ثابت
  {
    بنانشانی صحيح initial_ناپ = ناپ;
    // By default, start at the first bit of the first page of values.
    بنانشانی صحيح شروع_ورقہ = 0;
    بنانشانی صحيح شروع_ورقہ_قدر = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت خودکار* ورقہ_map_array = ورقہ_map.arrayZ;
      بنانشانی صحيح major = get_major (codepoint);
      بنانشانی صحيح i = آخری_ورقہ_lookup;
      إذا (unlikely (i >= ورقہ_map.لمائی || ورقہ_map_array[i].major != major))
      {
        ورقہ_map.bfind(major, &i, HB_NOT_لبھیا_STORE_CLOSEST);
        إذا (unlikely (i >= ورقہ_map.لمائی))
        {
          // codepoint is greater than our max element.
          طالما (++codepoint != INVALID && ناپ)
          {
            *باہرلا++ = codepoint;
            ناپ--;
          }
          أرجع initial_ناپ - ناپ;
        }
      }
      شروع_ورقہ = i;
      إذا (ورقہ_map_array[i].major == major)
      {
        شروع_ورقہ_قدر = ورقہ_remainder (codepoint + 1);
        إذا (unlikely (شروع_ورقہ_قدر == 0))
        {
          // The export-after value was last in the page. Start on next page.
          شروع_ورقہ++;
          شروع_ورقہ_قدر = 0;
        }
      }
    }

    hb_codepoint_t اگلا_قدر = codepoint + 1;
    لكل (بنانشانی صحيح i=شروع_ورقہ; i<ورقہ_map.لمائی && ناپ; i++)
    {
      uint32_t ادھار = major_شروع (ورقہ_map.arrayZ[i].major);
      بنانشانی صحيح n = ورقے[ورقہ_map.arrayZ[i].اشاریہ_2].write_inverted (ادھار, شروع_ورقہ_قدر, باہرلا, ناپ, &اگلا_قدر);
      باہرلا += n;
      ناپ -= n;
      شروع_ورقہ_قدر = 0;
    }
    طالما (اگلا_قدر < HB_سیٹ_قدر_INVALID && ناپ) {
      *باہرلا++ = اگلا_قدر++;
      ناپ--;
    }
    أرجع initial_ناپ - ناپ;
  }

  منطقي has_population () ثابت { أرجع population != UINT_MAX; }
  بنانشانی صحيح get_population () ثابت
  {
    إذا (has_population ())
      أرجع population;

    بنانشانی صحيح نکالو = 0;
    بنانشانی صحيح گنتی = ورقے.لمائی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      نکالو += ورقے[i].get_population ();

    population = نکالو;
    أرجع نکالو;
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
    لكل (ثابت خودکار &map : ورقہ_map)
    {
      ثابت خودکار &ورقہ = ورقے.arrayZ[map.اشاریہ_2];
      لكل (بنانشانی i = 0; i < ورقہ_t::len (); i++)
      {
	ورقہ_t::elt_t بٹ = ورقہ.v[i];
	إذا (!بٹ) استمر;
	إذا (singleton != INVALID || (بٹ & (بٹ - 1)))
	  أرجع خطأ;
	singleton = map.major * ورقہ_t::ورقہ_بٹ +
		    i * ورقہ_t::ELT_بٹ + hb_ctz (بٹ);
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
    بنانشانی گنتی = ورقے.لمائی;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      ثابت خودکار& map = ورقہ_map.arrayZ[i];
      ثابت خودکار& ورقہ = ورقے.arrayZ[map.اشاریہ_2];

      إذا (!ورقہ.is_empty ())
	أرجع map.major * ورقہ_t::ورقہ_بٹ + ورقہ.get_min ();
    }
    أرجع INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    بنانشانی گنتی = ورقے.لمائی;
    لكل (نشانیوالا i = گنتی - 1; i >= 0; i--)
    {
      ثابت خودکار& map = ورقہ_map.arrayZ[(بنانشانی) i];
      ثابت خودکار& ورقہ = ورقے.arrayZ[map.اشاریہ_2];

      إذا (!ورقہ.is_empty ())
	أرجع map.major * ورقہ_t::ورقہ_بٹ + ورقہ.get_max ();
    }
    أرجع INVALID;
  }

  ساكن constexpr hb_codepoint_t INVALID = ورقہ_t::INVALID;

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = سچ;
    ساكن constexpr منطقي has_fast_len = سچ;
    iter_t (ثابت hb_ثنائی_ہندسہ_سیٹ_t &s_ = Null (hb_ثنائی_ہندسہ_سیٹ_t),
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
    فراغ __prev__ () { s->پچھلا (&v); }
    بنانشانی __len__ () ثابت { أرجع l; }
    iter_t سرا () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_ثنائی_ہندسہ_سیٹ_t *s;
    hb_codepoint_t v;
    بنانشانی l;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  عامل iter_t () ثابت { أرجع iter (); }

  محمي:

  ورقہ_t *ورقہ_for (hb_codepoint_t g, منطقي insert = خطأ)
  {
    بنانشانی major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    بنانشانی i = آخری_ورقہ_lookup;
    إذا (likely (i < ورقہ_map.لمائی))
    {
      خودکار &cached_ورقہ = ورقہ_map.arrayZ[i];
      إذا (cached_ورقہ.major == major)
	أرجع &ورقے.arrayZ[cached_ورقہ.اشاریہ_2];
    }

    ورقہ_map_t map = {major, ورقے.لمائی};
    إذا (!ورقہ_map.bfind (map, &i, HB_NOT_لبھیا_STORE_CLOSEST))
    {
      إذا (!insert)
        أرجع خالی_سوچک;

      إذا (unlikely (!resize (ورقے.لمائی + 1)))
	أرجع خالی_سوچک;

      ورقے.arrayZ[map.اشاریہ_2].init0 ();
      memmove (ورقہ_map.arrayZ + i + 1,
	       ورقہ_map.arrayZ + i,
	       (ورقہ_map.لمائی - 1 - i) * ورقہ_map.شے_ناپ);
      ورقہ_map.arrayZ[i] = map;
    }

    آخری_ورقہ_lookup = i;
    أرجع &ورقے.arrayZ[ورقہ_map.arrayZ[i].اشاریہ_2];
  }
  ثابت ورقہ_t *ورقہ_for (hb_codepoint_t g) ثابت
  {
    بنانشانی major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    بنانشانی i = آخری_ورقہ_lookup;
    إذا (likely (i < ورقہ_map.لمائی))
    {
      خودکار &cached_ورقہ = ورقہ_map.arrayZ[i];
      إذا (cached_ورقہ.major == major)
	أرجع &ورقے.arrayZ[cached_ورقہ.اشاریہ_2];
    }

    ورقہ_map_t key = {major};
    إذا (!ورقہ_map.bfind (key, &i))
      أرجع خالی_سوچک;

    آخری_ورقہ_lookup = i;
    أرجع &ورقے.arrayZ[ورقہ_map.arrayZ[i].اشاریہ_2];
  }
  ورقہ_t &ورقہ_at (بنانشانی صحيح i)
  {
    أكد (i < ورقہ_map.لمائی);
    أرجع ورقے.arrayZ[ورقہ_map.arrayZ[i].اشاریہ_2];
  }
  ثابت ورقہ_t &ورقہ_at (بنانشانی صحيح i) ثابت
  {
    أكد (i < ورقہ_map.لمائی);
    أرجع ورقے.arrayZ[ورقہ_map.arrayZ[i].اشاریہ_2];
  }
  بنانشانی صحيح get_major (hb_codepoint_t g) ثابت { أرجع g >> ورقہ_t::ورقہ_بٹ_LOG_2; }
  بنانشانی صحيح ورقہ_remainder (hb_codepoint_t g) ثابت { أرجع g & ورقہ_t::ورقہ_BITMASK; }
  hb_codepoint_t major_شروع (بنانشانی صحيح major) ثابت { أرجع major << ورقہ_t::ورقہ_بٹ_LOG_2; }
};


#جے_انت /* HB_BIT_SET_HH */
