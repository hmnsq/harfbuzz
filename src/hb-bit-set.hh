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

#ifndef HB_BIT_SET_HH
#define HB_BIT_SET_HH

#ضمن "hb.hh"
#ضمن "hb-bit-page.hh"

بنية hb_bit_set_t
{
  hb_bit_set_t () = افتراضي;
  ~hb_bit_set_t () = افتراضي;

  hb_bit_set_t (ثابت hb_bit_set_t& other) : hb_bit_set_t () { set (other, true); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*هذا, other); }
  hb_bit_set_t& operator= (ثابت hb_bit_set_t& other) { set (other); أرجع *هذا; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*هذا, other); أرجع *هذا; }
  friend فراغ swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    إذا (likely (!a.successful || !b.successful))
      أرجع;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  فراغ init ()
  {
    successful = true;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  فراغ fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  بنية page_map_t
  {
    صحيح cmp (ثابت page_map_t &o) ثابت { أرجع cmp (o.major); }
    صحيح cmp (uint32_t o_major) ثابت { أرجع (صحيح) o_major - (صحيح) major; }

    uint32_t major;
    uint32_t index;
  };

  منطقي successful = true; /* Allocations successful */
  mutable unsigned صحيح population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  فراغ err () { إذا (successful) successful = خطأ; } /* TODO Remove */
  منطقي in_error () ثابت { أرجع !successful; }

  منطقي resize (unsigned صحيح سانى, منطقي clear = true, منطقي exact_size = خطأ)
  {
    إذا (unlikely (!successful)) أرجع خطأ;

    إذا (pages.length < سانى && (unsigned) pages.allocated < سانى && سانى <= 2)
      exact_size = true; // Most sets are small and local

    إذا (unlikely (!pages.resize_full (سانى, clear, exact_size) ||
	!page_map.resize_full (سانى, clear, خطأ)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = خطأ;
      أرجع خطأ;
    }
    أرجع true;
  }

  فراغ alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = true;
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
    إذا (!pages.length && !population) أرجع;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  منطقي is_empty () ثابت
  {
    unsigned صحيح سانى = pages.length;
    لكل (unsigned صحيح i = 0; i < سانى; i++)
      إذا (!pages[i].is_empty ())
	أرجع خطأ;
    أرجع true;
  }
  explicit operator منطقي () ثابت { أرجع !is_empty (); }

  uint32_t hash () ثابت
  {
    uint32_t h = 0;
    لكل (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      إذا (unlikely (page.is_empty ())) استمر;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    أرجع h;
  }

  خاص:
  فراغ dirty () { population = UINT_MAX; }
  عام:

  فراغ add (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (unlikely (g == INVALID)) أرجع;
    dirty ();
    page_t *page = page_for (g, true); إذا (unlikely (!page)) أرجع;
    page->add (g);
  }
  فراغ add_bits (hb_codepoint_t g, uint64_t bits)
  {
    إذا (unlikely (!successful) || unlikely (!bits)) أرجع;
    أكد (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); إذا (unlikely (!page)) أرجع;
    page->add_bits (g, bits);
  }
  منطقي add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!successful)) أرجع true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (a > b || a == INVALID || b == INVALID)) أرجع خطأ;
    dirty ();
    unsigned صحيح ma = get_major (a);
    unsigned صحيح mb = get_major (b);
    إذا (ma == mb)
    {
      page_t *page = page_for (a, true); إذا (unlikely (!page)) أرجع خطأ;
      page->add_range (a, b);
    }
    وإلا
    {
      page_t *page = page_for (a, true); إذا (unlikely (!page)) أرجع خطأ;
      page->add_range (a, major_start (ma + 1) - 1);

      لكل (unsigned صحيح m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); إذا (unlikely (!page)) أرجع خطأ;
	page->init1 ();
      }

      page = page_for (b, true); إذا (unlikely (!page)) أرجع خطأ;
      page->add_range (major_start (mb), b);
    }
    أرجع true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  ساكن مضمن ثابت Type& StructAtOffsetUnaligned(ثابت فراغ *P, unsigned صحيح offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    أرجع * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + offset);
#pragma GCC diagnostic چىقىرىش
  }

  template <typename T>
  فراغ set_array (منطقي v, ثابت T *array, unsigned صحيح سانى, unsigned صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (!سانى) أرجع;
    dirty ();
    hb_codepoint_t g = *array;
    طالما (سانى)
    {
      unsigned صحيح m = get_major (g);
      page_t *page = page_for (g, v); إذا (unlikely (v && !page)) أرجع;
      unsigned صحيح start = major_start (m);
      unsigned صحيح end = major_start (m + 1);
      افعل
      {
        إذا (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	سانى--;
      }
      طالما (سانى && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  فراغ add_array (ثابت T *array, unsigned صحيح سانى, unsigned صحيح stride=حجم(T))
  { set_array (true, array, سانى, stride); }
  template <typename T>
  فراغ add_array (ثابت hb_array_t<ثابت T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  فراغ del_array (ثابت T *array, unsigned صحيح سانى, unsigned صحيح stride=حجم(T))
  { set_array (خطأ, array, سانى, stride); }
  template <typename T>
  فراغ del_array (ثابت hb_array_t<ثابت T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  منطقي set_sorted_array (منطقي v, ثابت T *array, unsigned صحيح سانى, unsigned صحيح stride=حجم(T))
  {
    إذا (unlikely (!successful)) أرجع true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    إذا (unlikely (!سانى)) أرجع true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    طالما (سانى)
    {
      unsigned صحيح m = get_major (g);
      page_t *page = page_for (g, v); إذا (unlikely (v && !page)) أرجع خطأ;
      unsigned صحيح end = major_start (m + 1);
      افعل
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	إذا (g < last_g) أرجع خطأ;
	last_g = g;

        إذا (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	سانى--;
      }
      طالما (سانى && (g = *array, g < end));
    }
    أرجع true;
  }

  template <typename T>
  منطقي add_sorted_array (ثابت T *array, unsigned صحيح سانى, unsigned صحيح stride=حجم(T))
  { أرجع set_sorted_array (true, array, سانى, stride); }
  template <typename T>
  منطقي add_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  منطقي del_sorted_array (ثابت T *array, unsigned صحيح سانى, unsigned صحيح stride=حجم(T))
  { أرجع set_sorted_array (خطأ, array, سانى, stride); }
  template <typename T>
  منطقي del_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع del_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g)
  {
    إذا (unlikely (!successful)) أرجع;
    page_t *page = page_for (g);
    إذا (!page)
      أرجع;
    dirty ();
    page->del (g);
  }
  فراغ del_bits (hb_codepoint_t g, uint64_t bits)
  {
    إذا (unlikely (!successful) || unlikely (!bits)) أرجع;
    أكد (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    إذا (!page)
      أرجع;
    dirty ();
    page->del_bits (g, bits);
  }

  خاص:
  فراغ del_pages (صحيح ds, صحيح de)
  {
    إذا (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      إذا (unlikely (!allocate_compact_workspace (compact_workspace))) أرجع;

      unsigned صحيح write_index = 0;
      لكل (unsigned صحيح i = 0; i < page_map.length; i++)
      {
	صحيح m = (صحيح) page_map.arrayZ[i].major;
	إذا (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  عام:
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    إذا (unlikely (!successful)) أرجع;
    إذا (unlikely (a > b || a == INVALID)) أرجع;
    dirty ();
    unsigned صحيح ma = get_major (a);
    unsigned صحيح mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    صحيح ds = (a == major_start (ma))? (صحيح) ma: (صحيح) (ma + 1);
    صحيح de = (b + 1 == major_start (mb + 1))? (صحيح) mb: ((صحيح) mb - 1);
    إذا (ds > de || (صحيح) ma < ds)
    {
      page_t *page = page_for (a);
      إذا (page)
      {
	إذا (ma == mb)
	  page->del_range (a, b);
	وإلا
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    إذا (de < (صحيح) mb && ma != mb)
    {
      page_t *page = page_for (b);
      إذا (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  منطقي get (hb_codepoint_t g) ثابت
  {
    ثابت page_t *page = page_for (g);
    إذا (!page)
      أرجع خطأ;
    أرجع page->get (g);
  }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي operator [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي operator () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); أرجع *هذا; }
  hb_bit_set_t& operator << (ثابت hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); أرجع *هذا; }

  منطقي intersects (ثابت hb_bit_set_t &other) ثابت
  {
    unsigned صحيح na = pages.length;
    unsigned صحيح nb = other.pages.length;

    unsigned صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	إذا (page_at (a).intersects (other.page_at (b)))
	  أرجع true;
	a++;
	b++;
      }
      وإلا إذا (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      وإلا
	b++;
    }
    أرجع خطأ;
  }
  منطقي may_intersect (ثابت hb_bit_set_t &other) ثابت
  { أرجع intersects (other); }

  منطقي intersects (hb_codepoint_t first, hb_codepoint_t last) ثابت
  {
    hb_codepoint_t c = first - 1;
    أرجع next (&c) && c <= last;
  }
  فراغ set (ثابت hb_bit_set_t &other, منطقي exact_size = خطأ)
  {
    إذا (unlikely (!successful)) أرجع;
    unsigned صحيح سانى = other.pages.length;
    إذا (unlikely (!resize  (سانى, خطأ, exact_size)))
      أرجع;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  منطقي is_equal (ثابت hb_bit_set_t &other) ثابت
  {
    إذا (has_population () && other.has_population () &&
	population != other.population)
      أرجع خطأ;

    unsigned صحيح na = pages.length;
    unsigned صحيح nb = other.pages.length;

    unsigned صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      إذا (page_at (a).is_empty ()) { a++; استمر; }
      إذا (other.page_at (b).is_empty ()) { b++; استمر; }
      إذا (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	أرجع خطأ;
      a++;
      b++;
    }
    لكل (; a < na; a++)
      إذا (!page_at (a).is_empty ()) { أرجع خطأ; }
    لكل (; b < nb; b++)
      إذا (!other.page_at (b).is_empty ()) { أرجع خطأ; }

    أرجع true;
  }

  منطقي is_subset (ثابت hb_bit_set_t &larger_set) ثابت
  {
    إذا (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      أرجع خطأ;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    طالما (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

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

      auto lp = larger_set.page_at (lpi);
      إذا (!sp.is_subset (lp))
        أرجع خطأ;

      spi++;
      lpi++;
    }

    طالما (spi < page_map.length)
      إذا (!page_at (spi++).is_empty ())
        أرجع خطأ;

    أرجع true;
  }

  خاص:
  منطقي allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    إذا (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = خطأ;
      أرجع خطأ;
    }

    أرجع true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  فراغ compact (hb_vector_t<unsigned>& workspace,
                unsigned صحيح length)
  {
    أكد(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    لكل (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  فراغ compact_pages (ثابت hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned صحيح write_index = 0;
    لكل (unsigned صحيح i = 0; i < pages.length; i++)
    {
      إذا (old_index_to_page_map_index[i] == 0xFFFFFFFF) استمر;

      إذا (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  عام:

  فراغ process_ (hb_bit_page_t::vector_t (*op) (ثابت hb_bit_page_t::vector_t &, ثابت hb_bit_page_t::vector_t &),
		 منطقي passthru_سول, منطقي passthru_ئوڭ,
		 ثابت hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    إذا (unlikely (!successful)) أرجع;

    dirty ();

    unsigned صحيح na = pages.length;
    unsigned صحيح nb = other.pages.length;
    unsigned صحيح next_page = na;

    unsigned صحيح سانى = 0, new_سانى = 0;
    unsigned صحيح a = 0, b = 0;
    unsigned صحيح write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    إذا (!passthru_سول)
    {
      منطقي allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      إذا (unlikely (!allocated))
      {
	successful = خطأ;
	أرجع;
      }
    }

    لكل (; a < na && b < nb; )
    {
      إذا (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	إذا (!passthru_سول)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  إذا (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	سانى++;
	a++;
	b++;
      }
      وإلا إذا (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	إذا (passthru_سول)
	  سانى++;
	a++;
      }
      وإلا
      {
	إذا (passthru_ئوڭ)
	  سانى++;
	b++;
      }
    }
    إذا (passthru_سول)
      سانى += na - a;
    إذا (passthru_ئوڭ)
      سانى += nb - b;

    إذا (!passthru_سول)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    إذا (unlikely (!resize (سانى)))
      أرجع;

    new_سانى = سانى;

    /* Process in-place backward. */
    a = na;
    b = nb;
    لكل (; a && b; )
    {
      إذا (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	سانى--;
	page_map.arrayZ[سانى] = page_map.arrayZ[a];
	page_at (سانى).v = op (page_at (a).v, other.page_at (b).v);
	page_at (سانى).dirty ();
      }
      وإلا إذا (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	إذا (passthru_سول)
	{
	  سانى--;
	  page_map.arrayZ[سانى] = page_map.arrayZ[a];
	}
      }
      وإلا
      {
	b--;
	إذا (passthru_ئوڭ)
	{
	  سانى--;
	  page_map.arrayZ[سانى].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[سانى].index = next_page++;
	  page_at (سانى) = other.page_at (b);
	}
      }
    }
    إذا (passthru_سول)
      طالما (a)
      {
	a--;
	سانى--;
	page_map.arrayZ[سانى] = page_map.arrayZ[a];
      }
    إذا (passthru_ئوڭ)
      طالما (b)
      {
	b--;
	سانى--;
	page_map.arrayZ[سانى].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[سانى].index = next_page++;
	page_at (سانى) = other.page_at (b);
      }
    أكد (!سانى);
    resize (new_سانى);
  }
  template <typename Op>
  ساكن hb_bit_page_t::vector_t
  op_ (ثابت hb_bit_page_t::vector_t &a, ثابت hb_bit_page_t::vector_t &b)
  { أرجع Op{} (a, b); }
  template <typename Op>
  فراغ process (ثابت Op& op,
		ثابت hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  فراغ union_ (ثابت hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  فراغ intersect (ثابت hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  فراغ subtract (ثابت hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  فراغ symmetric_difference (ثابت hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      أرجع *codepoint != INVALID;
    }

    ثابت auto* page_map_array = page_map.arrayZ;
    unsigned صحيح major = get_major (*codepoint);
    unsigned صحيح i = last_page_lookup;

    إذا (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      إذا (i >= page_map.length) {
        *codepoint = INVALID;
        أرجع خطأ;
      }
      last_page_lookup = i;
    }

    ثابت auto* pages_array = pages.arrayZ;
    ثابت page_map_t &current = page_map_array[i];
    إذا (likely (current.major == major))
    {
      إذا (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        أرجع true;
      }
      i++;
    }

    لكل (; i < page_map.length; i++)
    {
      ثابت page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      إذا (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	أرجع true;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ثابت
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned صحيح i = 0;
    unsigned صحيح elt = 0;
    إذا (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      إذا (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        اذهب done;
      base += page_t::ELT_BITS;

      unsigned صحيح major = get_major (base);
      i = last_page_lookup;
      إذا (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	إذا (i >= page_map.length)
	  اذهب done;
      }
      إذا (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    لكل (; i < page_map.length; i++, elt = 0)
    {
      ثابت page_map_t &map = page_map.arrayZ[i];
      ثابت page_t &page = pages.arrayZ[map.index];
      لكل (; elt < page_t::len (); elt++)
	إذا (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  أرجع true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    أرجع خطأ;
  }
  منطقي previous (hb_codepoint_t *codepoint) ثابت
  {
    إذا (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      أرجع *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned صحيح i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    إذا (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      إذا (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	أرجع true;
      }
    }
    i--;
    لكل (; (صحيح) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      إذا (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	أرجع true;
      }
    }
    *codepoint = INVALID;
    أرجع خطأ;
  }
  منطقي next_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    hb_codepoint_t i;

    i = *last;
    إذا (!next (&i))
    {
      *last = *first = INVALID;
      أرجع خطأ;
    }

    *last = *first = i;

    ثابت auto* page_map_array = page_map.arrayZ;
    ثابت auto* pages_array = pages.arrayZ;
    unsigned صحيح major = get_major (*last);
    unsigned صحيح p_index = last_page_lookup;

    إذا (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      أكد(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned صحيح rem = page_remainder (*last);
    unsigned صحيح elt = rem / page_t::ELT_BITS;
    unsigned صحيح bit = rem & page_t::ELT_MASK;

    ثابت page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned صحيح run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    إذا (run < page_t::ELT_BITS - bit)
      أرجع true;

    elt++;

    طالما (true)
    {
      لكل (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          استمر;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        أرجع true;
      }

      p_index++;


      إذا (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        أرجع true;

      ثابت auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  منطقي previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    hb_codepoint_t i;

    i = *first;
    إذا (!previous (&i))
    {
      *last = *first = INVALID;
      أرجع خطأ;
    }

    *last = *first = i;

    ثابت auto* page_map_array = page_map.arrayZ;
    ثابت auto* pages_array = pages.arrayZ;
    unsigned صحيح major = get_major (*first);
    unsigned صحيح p_index = last_page_lookup;

    إذا (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      أكد(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned صحيح rem = page_remainder (*first);
    صحيح elt = rem / page_t::ELT_BITS;
    unsigned صحيح bit = rem & page_t::ELT_MASK;

    ثابت page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned صحيح run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    إذا (run < bit + 1)
      أرجع true;

    elt--;

    طالما (true)
    {
      لكل (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        إذا (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          استمر;
        }
        uint64_t inv = ~w;
        unsigned صحيح zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        أرجع true;
      }


      إذا (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        أرجع true;

      p_index--;
      ثابت auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned صحيح next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned صحيح    size) ثابت
  {
    // By default, start at the first bit of the first page of values.
    unsigned صحيح start_page = 0;
    unsigned صحيح start_page_قىممەت = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت auto* page_map_array = page_map.arrayZ;
      unsigned صحيح major = get_major (codepoint);
      unsigned صحيح i = last_page_lookup;
      إذا (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	إذا (i >= page_map.length)
	  أرجع 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      إذا (page_map_array[i].major == major)
      {
        start_page_قىممەت = page_remainder (codepoint + 1);
        إذا (unlikely (start_page_قىممەت == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_قىممەت = 0;
        }
      }
    }

    unsigned صحيح initial_size = size;
    لكل (unsigned صحيح i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned صحيح n = pages[page_map.arrayZ[i].index].اكتب (base, start_page_قىممەت, out, size);
      out += n;
      size -= n;
      start_page_قىممەت = 0;
    }
    أرجع initial_size - size;
  }

  unsigned صحيح next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned صحيح    size) ثابت
  {
    unsigned صحيح initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned صحيح start_page = 0;
    unsigned صحيح start_page_قىممەت = 0;
    إذا (unlikely (codepoint != INVALID))
    {
      ثابت auto* page_map_array = page_map.arrayZ;
      unsigned صحيح major = get_major (codepoint);
      unsigned صحيح i = last_page_lookup;
      إذا (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        إذا (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          طالما (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          أرجع initial_size - size;
        }
      }
      start_page = i;
      إذا (page_map_array[i].major == major)
      {
        start_page_قىممەت = page_remainder (codepoint + 1);
        إذا (unlikely (start_page_قىممەت == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_قىممەت = 0;
        }
      }
    }

    hb_codepoint_t next_قىممەت = codepoint + 1;
    لكل (unsigned صحيح i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned صحيح n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_قىممەت, out, size, &next_قىممەت);
      out += n;
      size -= n;
      start_page_قىممەت = 0;
    }
    طالما (next_قىممەت < HB_SET_قىممەت_INVALID && size) {
      *out++ = next_قىممەت++;
      size--;
    }
    أرجع initial_size - size;
  }

  منطقي has_population () ثابت { أرجع population != UINT_MAX; }
  unsigned صحيح get_population () ثابت
  {
    إذا (has_population ())
      أرجع population;

    unsigned صحيح چىقىرىش = 0;
    unsigned صحيح سانى = pages.length;
    لكل (unsigned صحيح i = 0; i < سانى; i++)
      چىقىرىش += pages[i].get_population ();

    population = چىقىرىش;
    أرجع چىقىرىش;
  }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    إذا (has_population ())
    {
      إذا (population != 1) أرجع خطأ;
      *codepoint = get_min ();
      أرجع true;
    }

    hb_codepoint_t singleton = INVALID;
    لكل (ثابت auto &map : page_map)
    {
      ثابت auto &page = pages.arrayZ[map.index];
      لكل (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	إذا (!bits) استمر;
	إذا (singleton != INVALID || (bits & (bits - 1)))
	  أرجع خطأ;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    إذا (singleton == INVALID)
    {
      population = 0;
      أرجع خطأ;
    }
    population = 1;
    *codepoint = singleton;
    أرجع true;
  }
  hb_codepoint_t get_min () ثابت
  {
    unsigned سانى = pages.length;
    لكل (unsigned i = 0; i < سانى; i++)
    {
      ثابت auto& map = page_map.arrayZ[i];
      ثابت auto& page = pages.arrayZ[map.index];

      إذا (!page.is_empty ())
	أرجع map.major * page_t::PAGE_BITS + page.get_min ();
    }
    أرجع INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    unsigned سانى = pages.length;
    لكل (signed i = سانى - 1; i >= 0; i--)
    {
      ثابت auto& map = page_map.arrayZ[(unsigned) i];
      ثابت auto& page = pages.arrayZ[map.index];

      إذا (!page.is_empty ())
	أرجع map.major * page_t::PAGE_BITS + page.get_max ();
    }
    أرجع INVALID;
  }

  ساكن constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = true;
    ساكن constexpr منطقي has_fast_len = true;
    iter_t (ثابت hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    منطقي init = true) : s (&s_), v (INVALID), l(0)
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
    فراغ __prev__ () { s->previous (&v); }
    unsigned __len__ () ثابت { أرجع l; }
    iter_t end () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي operator != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  operator iter_t () ثابت { أرجع iter (); }

  محمي:

  page_t *page_for (hb_codepoint_t g, منطقي insert = خطأ)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    إذا (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      إذا (cached_page.major == major)
	أرجع &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    إذا (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      إذا (!insert)
        أرجع nullptr;

      إذا (unlikely (!resize (pages.length + 1)))
	أرجع nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    أرجع &pages.arrayZ[page_map.arrayZ[i].index];
  }
  ثابت page_t *page_for (hb_codepoint_t g) ثابت
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    إذا (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      إذا (cached_page.major == major)
	أرجع &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    إذا (!page_map.bfind (key, &i))
      أرجع nullptr;

    last_page_lookup = i;
    أرجع &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned صحيح i)
  {
    أكد (i < page_map.length);
    أرجع pages.arrayZ[page_map.arrayZ[i].index];
  }
  ثابت page_t &page_at (unsigned صحيح i) ثابت
  {
    أكد (i < page_map.length);
    أرجع pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned صحيح get_major (hb_codepoint_t g) ثابت { أرجع g >> page_t::PAGE_BITS_LOG_2; }
  unsigned صحيح page_remainder (hb_codepoint_t g) ثابت { أرجع g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned صحيح major) ثابت { أرجع major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
