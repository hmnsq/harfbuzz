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

  hb_bit_set_t (ثابت hb_bit_set_t& other) : hb_bit_set_t () { set (other, ܫܪܪܐ); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*هذا, other); }
  hb_bit_set_t& operator= (ثابت hb_bit_set_t& other) { set (other); ܦܢܐ *هذا; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*هذا, other); ܦܢܐ *هذا; }
  friend فراغ swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    ܐܢ (likely (!a.successful || !b.successful))
      ܦܢܐ;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  فراغ init ()
  {
    successful = ܫܪܪܐ;
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
    صحيح cmp (ثابت page_map_t &o) ثابت { ܦܢܐ cmp (o.major); }
    صحيح cmp (uint32_t o_major) ثابت { ܦܢܐ (صحيح) o_major - (صحيح) major; }

    uint32_t major;
    uint32_t index;
  };

  منطقي successful = ܫܪܪܐ; /* Allocations successful */
  mutable unsigned صحيح population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  فراغ err () { ܐܢ (successful) successful = ܕܓܠܐ; } /* TODO Remove */
  منطقي in_error () ثابت { ܦܢܐ !successful; }

  منطقي resize (unsigned صحيح count, منطقي clear = ܫܪܪܐ, منطقي exact_size = ܕܓܠܐ)
  {
    ܐܢ (unlikely (!successful)) ܦܢܐ ܕܓܠܐ;

    ܐܢ (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = ܫܪܪܐ; // Most sets are small and local

    ܐܢ (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, ܕܓܠܐ)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = ܕܓܠܐ;
      ܦܢܐ ܕܓܠܐ;
    }
    ܦܢܐ ܫܪܪܐ;
  }

  فراغ alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = ܫܪܪܐ;
    clear ();
    ܦܢܐ *هذا;
  }

  فراغ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ܐܢ (!pages.length && !population) ܦܢܐ;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  منطقي is_empty () ثابت
  {
    unsigned صحيح count = pages.length;
    لكل (unsigned صحيح i = 0; i < count; i++)
      ܐܢ (!pages[i].is_empty ())
	ܦܢܐ ܕܓܠܐ;
    ܦܢܐ ܫܪܪܐ;
  }
  explicit operator منطقي () ثابت { ܦܢܐ !is_empty (); }

  uint32_t hash () ثابت
  {
    uint32_t h = 0;
    لكل (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      ܐܢ (unlikely (page.is_empty ())) استمر;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    ܦܢܐ h;
  }

  خاص:
  فراغ dirty () { population = UINT_MAX; }
  عام:

  فراغ add (hb_codepoint_t g)
  {
    ܐܢ (unlikely (!successful)) ܦܢܐ;
    ܐܢ (unlikely (g == INVALID)) ܦܢܐ;
    dirty ();
    page_t *page = page_for (g, ܫܪܪܐ); ܐܢ (unlikely (!page)) ܦܢܐ;
    page->add (g);
  }
  فراغ add_bits (hb_codepoint_t g, uint64_t bits)
  {
    ܐܢ (unlikely (!successful) || unlikely (!bits)) ܦܢܐ;
    أكد (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, ܫܪܪܐ); ܐܢ (unlikely (!page)) ܦܢܐ;
    page->add_bits (g, bits);
  }
  منطقي add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ܐܢ (unlikely (!successful)) ܦܢܐ ܫܪܪܐ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ܐܢ (unlikely (a > b || a == INVALID || b == INVALID)) ܦܢܐ ܕܓܠܐ;
    dirty ();
    unsigned صحيح ma = get_major (a);
    unsigned صحيح mb = get_major (b);
    ܐܢ (ma == mb)
    {
      page_t *page = page_for (a, ܫܪܪܐ); ܐܢ (unlikely (!page)) ܦܢܐ ܕܓܠܐ;
      page->add_range (a, b);
    }
    ܐܚܪܢܐ
    {
      page_t *page = page_for (a, ܫܪܪܐ); ܐܢ (unlikely (!page)) ܦܢܐ ܕܓܠܐ;
      page->add_range (a, major_start (ma + 1) - 1);

      لكل (unsigned صحيح m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), ܫܪܪܐ); ܐܢ (unlikely (!page)) ܦܢܐ ܕܓܠܐ;
	page->init1 ();
      }

      page = page_for (b, ܫܪܪܐ); ܐܢ (unlikely (!page)) ܦܢܐ ܕܓܠܐ;
      page->add_range (major_start (mb), b);
    }
    ܦܢܐ ܫܪܪܐ;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  ساكن مضمن ثابت Type& StructAtOffsetUnaligned(ثابت فراغ *P, unsigned صحيح offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    ܦܢܐ * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  فراغ set_array (منطقي v, ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  {
    ܐܢ (unlikely (!successful)) ܦܢܐ;
    ܐܢ (!count) ܦܢܐ;
    dirty ();
    hb_codepoint_t g = *array;
    ܟܡܐ (count)
    {
      unsigned صحيح m = get_major (g);
      page_t *page = page_for (g, v); ܐܢ (unlikely (v && !page)) ܦܢܐ;
      unsigned صحيح start = major_start (m);
      unsigned صحيح end = major_start (m + 1);
      ܥܒܕ
      {
        ܐܢ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ܟܡܐ (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  فراغ add_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { set_array (ܫܪܪܐ, array, count, stride); }
  template <typename T>
  فراغ add_array (ثابت hb_array_t<ثابت T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  فراغ del_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { set_array (ܕܓܠܐ, array, count, stride); }
  template <typename T>
  فراغ del_array (ثابت hb_array_t<ثابت T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  منطقي set_sorted_array (منطقي v, ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  {
    ܐܢ (unlikely (!successful)) ܦܢܐ ܫܪܪܐ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ܐܢ (unlikely (!count)) ܦܢܐ ܫܪܪܐ;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ܟܡܐ (count)
    {
      unsigned صحيح m = get_major (g);
      page_t *page = page_for (g, v); ܐܢ (unlikely (v && !page)) ܦܢܐ ܕܓܠܐ;
      unsigned صحيح end = major_start (m + 1);
      ܥܒܕ
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ܐܢ (g < last_g) ܦܢܐ ܕܓܠܐ;
	last_g = g;

        ܐܢ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ܟܡܐ (count && (g = *array, g < end));
    }
    ܦܢܐ ܫܪܪܐ;
  }

  template <typename T>
  منطقي add_sorted_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { ܦܢܐ set_sorted_array (ܫܪܪܐ, array, count, stride); }
  template <typename T>
  منطقي add_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { ܦܢܐ add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  منطقي del_sorted_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { ܦܢܐ set_sorted_array (ܕܓܠܐ, array, count, stride); }
  template <typename T>
  منطقي del_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { ܦܢܐ del_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g)
  {
    ܐܢ (unlikely (!successful)) ܦܢܐ;
    page_t *page = page_for (g);
    ܐܢ (!page)
      ܦܢܐ;
    dirty ();
    page->del (g);
  }
  فراغ del_bits (hb_codepoint_t g, uint64_t bits)
  {
    ܐܢ (unlikely (!successful) || unlikely (!bits)) ܦܢܐ;
    أكد (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    ܐܢ (!page)
      ܦܢܐ;
    dirty ();
    page->del_bits (g, bits);
  }

  خاص:
  فراغ del_pages (صحيح ds, صحيح de)
  {
    ܐܢ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      ܐܢ (unlikely (!allocate_compact_workspace (compact_workspace))) ܦܢܐ;

      unsigned صحيح write_index = 0;
      لكل (unsigned صحيح i = 0; i < page_map.length; i++)
      {
	صحيح m = (صحيح) page_map.arrayZ[i].major;
	ܐܢ (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  عام:
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ܐܢ (unlikely (!successful)) ܦܢܐ;
    ܐܢ (unlikely (a > b || a == INVALID)) ܦܢܐ;
    dirty ();
    unsigned صحيح ma = get_major (a);
    unsigned صحيح mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    صحيح ds = (a == major_start (ma))? (صحيح) ma: (صحيح) (ma + 1);
    صحيح de = (b + 1 == major_start (mb + 1))? (صحيح) mb: ((صحيح) mb - 1);
    ܐܢ (ds > de || (صحيح) ma < ds)
    {
      page_t *page = page_for (a);
      ܐܢ (page)
      {
	ܐܢ (ma == mb)
	  page->del_range (a, b);
	ܐܚܪܢܐ
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    ܐܢ (de < (صحيح) mb && ma != mb)
    {
      page_t *page = page_for (b);
      ܐܢ (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  منطقي get (hb_codepoint_t g) ثابت
  {
    ثابت page_t *page = page_for (g);
    ܐܢ (!page)
      ܦܢܐ ܕܓܠܐ;
    ܦܢܐ page->get (g);
  }
  منطقي may_have (hb_codepoint_t g) ثابت { ܦܢܐ get (g); }

  /* Has interface. */
  منطقي operator [] (hb_codepoint_t k) ثابت { ܦܢܐ get (k); }
  منطقي has (hb_codepoint_t k) ثابت { ܦܢܐ (*هذا)[k]; }
  /* Predicate. */
  منطقي operator () (hb_codepoint_t k) ثابت { ܦܢܐ has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); ܦܢܐ *هذا; }
  hb_bit_set_t& operator << (ثابت hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ܦܢܐ *هذا; }

  منطقي intersects (ثابت hb_bit_set_t &other) ثابت
  {
    unsigned صحيح na = pages.length;
    unsigned صحيح nb = other.pages.length;

    unsigned صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      ܐܢ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ܐܢ (page_at (a).intersects (other.page_at (b)))
	  ܦܢܐ ܫܪܪܐ;
	a++;
	b++;
      }
      ܐܚܪܢܐ ܐܢ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      ܐܚܪܢܐ
	b++;
    }
    ܦܢܐ ܕܓܠܐ;
  }
  منطقي may_intersect (ثابت hb_bit_set_t &other) ثابت
  { ܦܢܐ intersects (other); }

  منطقي intersects (hb_codepoint_t first, hb_codepoint_t last) ثابت
  {
    hb_codepoint_t c = first - 1;
    ܦܢܐ next (&c) && c <= last;
  }
  فراغ set (ثابت hb_bit_set_t &other, منطقي exact_size = ܕܓܠܐ)
  {
    ܐܢ (unlikely (!successful)) ܦܢܐ;
    unsigned صحيح count = other.pages.length;
    ܐܢ (unlikely (!resize  (count, ܕܓܠܐ, exact_size)))
      ܦܢܐ;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  منطقي is_equal (ثابت hb_bit_set_t &other) ثابت
  {
    ܐܢ (has_population () && other.has_population () &&
	population != other.population)
      ܦܢܐ ܕܓܠܐ;

    unsigned صحيح na = pages.length;
    unsigned صحيح nb = other.pages.length;

    unsigned صحيح a = 0, b = 0;
    لكل (; a < na && b < nb; )
    {
      ܐܢ (page_at (a).is_empty ()) { a++; استمر; }
      ܐܢ (other.page_at (b).is_empty ()) { b++; استمر; }
      ܐܢ (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	ܦܢܐ ܕܓܠܐ;
      a++;
      b++;
    }
    لكل (; a < na; a++)
      ܐܢ (!page_at (a).is_empty ()) { ܦܢܐ ܕܓܠܐ; }
    لكل (; b < nb; b++)
      ܐܢ (!other.page_at (b).is_empty ()) { ܦܢܐ ܕܓܠܐ; }

    ܦܢܐ ܫܪܪܐ;
  }

  منطقي is_subset (ثابت hb_bit_set_t &larger_set) ثابت
  {
    ܐܢ (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      ܦܢܐ ܕܓܠܐ;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ܟܡܐ (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      ܐܢ (spm < lpm) {
        ܐܢ (!sp.is_empty ())
          ܦܢܐ ܕܓܠܐ;
        spi++;
        استمر;
      }

      ܐܢ (lpm < spm) {
        lpi++;
        استمر;
      }

      auto lp = larger_set.page_at (lpi);
      ܐܢ (!sp.is_subset (lp))
        ܦܢܐ ܕܓܠܐ;

      spi++;
      lpi++;
    }

    ܟܡܐ (spi < page_map.length)
      ܐܢ (!page_at (spi++).is_empty ())
        ܦܢܐ ܕܓܠܐ;

    ܦܢܐ ܫܪܪܐ;
  }

  خاص:
  منطقي allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    ܐܢ (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = ܕܓܠܐ;
      ܦܢܐ ܕܓܠܐ;
    }

    ܦܢܐ ܫܪܪܐ;
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
      ܐܢ (old_index_to_page_map_index[i] == 0xFFFFFFFF) استمر;

      ܐܢ (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  عام:

  فراغ process_ (hb_bit_page_t::vector_t (*op) (ثابت hb_bit_page_t::vector_t &, ثابت hb_bit_page_t::vector_t &),
		 منطقي passthru_left, منطقي passthru_right,
		 ثابت hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    ܐܢ (unlikely (!successful)) ܦܢܐ;

    dirty ();

    unsigned صحيح na = pages.length;
    unsigned صحيح nb = other.pages.length;
    unsigned صحيح next_page = na;

    unsigned صحيح count = 0, newCount = 0;
    unsigned صحيح a = 0, b = 0;
    unsigned صحيح write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    ܐܢ (!passthru_left)
    {
      منطقي allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      ܐܢ (unlikely (!allocated))
      {
	successful = ܕܓܠܐ;
	ܦܢܐ;
      }
    }

    لكل (; a < na && b < nb; )
    {
      ܐܢ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ܐܢ (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ܐܢ (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      ܐܚܪܢܐ ܐܢ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	ܐܢ (passthru_left)
	  count++;
	a++;
      }
      ܐܚܪܢܐ
      {
	ܐܢ (passthru_right)
	  count++;
	b++;
      }
    }
    ܐܢ (passthru_left)
      count += na - a;
    ܐܢ (passthru_right)
      count += nb - b;

    ܐܢ (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    ܐܢ (unlikely (!resize (count)))
      ܦܢܐ;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    لكل (; a && b; )
    {
      ܐܢ (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      ܐܚܪܢܐ ܐܢ (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	ܐܢ (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      ܐܚܪܢܐ
      {
	b--;
	ܐܢ (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    ܐܢ (passthru_left)
      ܟܡܐ (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    ܐܢ (passthru_right)
      ܟܡܐ (b)
      {
	b--;
	count--;
	page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[count].index = next_page++;
	page_at (count) = other.page_at (b);
      }
    أكد (!count);
    resize (newCount);
  }
  template <typename Op>
  ساكن hb_bit_page_t::vector_t
  op_ (ثابت hb_bit_page_t::vector_t &a, ثابت hb_bit_page_t::vector_t &b)
  { ܦܢܐ Op{} (a, b); }
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
    ܐܢ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ܦܢܐ *codepoint != INVALID;
    }

    ثابت auto* page_map_array = page_map.arrayZ;
    unsigned صحيح major = get_major (*codepoint);
    unsigned صحيح i = last_page_lookup;

    ܐܢ (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      ܐܢ (i >= page_map.length) {
        *codepoint = INVALID;
        ܦܢܐ ܕܓܠܐ;
      }
      last_page_lookup = i;
    }

    ثابت auto* pages_array = pages.arrayZ;
    ثابت page_map_t &current = page_map_array[i];
    ܐܢ (likely (current.major == major))
    {
      ܐܢ (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        ܦܢܐ ܫܪܪܐ;
      }
      i++;
    }

    لكل (; i < page_map.length; i++)
    {
      ثابت page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      ܐܢ (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	ܦܢܐ ܫܪܪܐ;
      }
    }
    *codepoint = INVALID;
    ܦܢܐ ܕܓܠܐ;
  }
  منطقي next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ثابت
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned صحيح i = 0;
    unsigned صحيح elt = 0;
    ܐܢ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      ܐܢ (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        اذهب done;
      base += page_t::ELT_BITS;

      unsigned صحيح major = get_major (base);
      i = last_page_lookup;
      ܐܢ (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ܐܢ (i >= page_map.length)
	  اذهب done;
      }
      ܐܢ (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    لكل (; i < page_map.length; i++, elt = 0)
    {
      ثابت page_map_t &map = page_map.arrayZ[i];
      ثابت page_t &page = pages.arrayZ[map.index];
      لكل (; elt < page_t::len (); elt++)
	ܐܢ (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  ܦܢܐ ܫܪܪܐ;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ܦܢܐ ܕܓܠܐ;
  }
  منطقي previous (hb_codepoint_t *codepoint) ثابت
  {
    ܐܢ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ܦܢܐ *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned صحيح i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    ܐܢ (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      ܐܢ (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	ܦܢܐ ܫܪܪܐ;
      }
    }
    i--;
    لكل (; (صحيح) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      ܐܢ (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	ܦܢܐ ܫܪܪܐ;
      }
    }
    *codepoint = INVALID;
    ܦܢܐ ܕܓܠܐ;
  }
  منطقي next_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    hb_codepoint_t i;

    i = *last;
    ܐܢ (!next (&i))
    {
      *last = *first = INVALID;
      ܦܢܐ ܕܓܠܐ;
    }

    *last = *first = i;

    ثابت auto* page_map_array = page_map.arrayZ;
    ثابت auto* pages_array = pages.arrayZ;
    unsigned صحيح major = get_major (*last);
    unsigned صحيح p_index = last_page_lookup;

    ܐܢ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
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
    ܐܢ (run < page_t::ELT_BITS - bit)
      ܦܢܐ ܫܪܪܐ;

    elt++;

    ܟܡܐ (ܫܪܪܐ)
    {
      لكل (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        ܐܢ (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          استمر;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        ܦܢܐ ܫܪܪܐ;
      }

      p_index++;


      ܐܢ (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        ܦܢܐ ܫܪܪܐ;

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
    ܐܢ (!previous (&i))
    {
      *last = *first = INVALID;
      ܦܢܐ ܕܓܠܐ;
    }

    *last = *first = i;

    ثابت auto* page_map_array = page_map.arrayZ;
    ثابت auto* pages_array = pages.arrayZ;
    unsigned صحيح major = get_major (*first);
    unsigned صحيح p_index = last_page_lookup;

    ܐܢ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
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
    ܐܢ (run < bit + 1)
      ܦܢܐ ܫܪܪܐ;

    elt--;

    ܟܡܐ (ܫܪܪܐ)
    {
      لكل (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        ܐܢ (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          استمر;
        }
        uint64_t inv = ~w;
        unsigned صحيح zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        ܦܢܐ ܫܪܪܐ;
      }


      ܐܢ (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        ܦܢܐ ܫܪܪܐ;

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
    unsigned صحيح start_page_value = 0;
    ܐܢ (unlikely (codepoint != INVALID))
    {
      ثابت auto* page_map_array = page_map.arrayZ;
      unsigned صحيح major = get_major (codepoint);
      unsigned صحيح i = last_page_lookup;
      ܐܢ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ܐܢ (i >= page_map.length)
	  ܦܢܐ 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      ܐܢ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ܐܢ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned صحيح initial_size = size;
    لكل (unsigned صحيح i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned صحيح n = pages[page_map.arrayZ[i].index].اكتب (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ܦܢܐ initial_size - size;
  }

  unsigned صحيح next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned صحيح    size) ثابت
  {
    unsigned صحيح initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned صحيح start_page = 0;
    unsigned صحيح start_page_value = 0;
    ܐܢ (unlikely (codepoint != INVALID))
    {
      ثابت auto* page_map_array = page_map.arrayZ;
      unsigned صحيح major = get_major (codepoint);
      unsigned صحيح i = last_page_lookup;
      ܐܢ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        ܐܢ (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ܟܡܐ (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          ܦܢܐ initial_size - size;
        }
      }
      start_page = i;
      ܐܢ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ܐܢ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    لكل (unsigned صحيح i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned صحيح n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ܟܡܐ (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    ܦܢܐ initial_size - size;
  }

  منطقي has_population () ثابت { ܦܢܐ population != UINT_MAX; }
  unsigned صحيح get_population () ثابت
  {
    ܐܢ (has_population ())
      ܦܢܐ population;

    unsigned صحيح pop = 0;
    unsigned صحيح count = pages.length;
    لكل (unsigned صحيح i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    ܦܢܐ pop;
  }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    ܐܢ (has_population ())
    {
      ܐܢ (population != 1) ܦܢܐ ܕܓܠܐ;
      *codepoint = get_min ();
      ܦܢܐ ܫܪܪܐ;
    }

    hb_codepoint_t singleton = INVALID;
    لكل (ثابت auto &map : page_map)
    {
      ثابت auto &page = pages.arrayZ[map.index];
      لكل (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	ܐܢ (!bits) استمر;
	ܐܢ (singleton != INVALID || (bits & (bits - 1)))
	  ܦܢܐ ܕܓܠܐ;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    ܐܢ (singleton == INVALID)
    {
      population = 0;
      ܦܢܐ ܕܓܠܐ;
    }
    population = 1;
    *codepoint = singleton;
    ܦܢܐ ܫܪܪܐ;
  }
  hb_codepoint_t get_min () ثابت
  {
    unsigned count = pages.length;
    لكل (unsigned i = 0; i < count; i++)
    {
      ثابت auto& map = page_map.arrayZ[i];
      ثابت auto& page = pages.arrayZ[map.index];

      ܐܢ (!page.is_empty ())
	ܦܢܐ map.major * page_t::PAGE_BITS + page.get_min ();
    }
    ܦܢܐ INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    unsigned count = pages.length;
    لكل (signed i = count - 1; i >= 0; i--)
    {
      ثابت auto& map = page_map.arrayZ[(unsigned) i];
      ثابت auto& page = pages.arrayZ[map.index];

      ܐܢ (!page.is_empty ())
	ܦܢܐ map.major * page_t::PAGE_BITS + page.get_max ();
    }
    ܦܢܐ INVALID;
  }

  ساكن constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = ܫܪܪܐ;
    ساكن constexpr منطقي has_fast_len = ܫܪܪܐ;
    iter_t (ثابت hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    منطقي init = ܫܪܪܐ) : s (&s_), v (INVALID), l(0)
    {
      ܐܢ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { ܦܢܐ v; }
    منطقي __more__ () ثابت { ܦܢܐ v != INVALID; }
    فراغ __next__ () { s->next (&v); ܐܢ (l) l--; }
    فراغ __prev__ () { s->previous (&v); }
    unsigned __len__ () ثابت { ܦܢܐ l; }
    iter_t end () ثابت { ܦܢܐ iter_t (*s, ܕܓܠܐ); }
    منطقي operator != (ثابت iter_t& o) ثابت
    { ܦܢܐ v != o.v; }

    محمي:
    ثابت hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () ثابت { ܦܢܐ iter_t (*هذا); }
  operator iter_t () ثابت { ܦܢܐ iter (); }

  محمي:

  page_t *page_for (hb_codepoint_t g, منطقي insert = ܕܓܠܐ)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ܐܢ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ܐܢ (cached_page.major == major)
	ܦܢܐ &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    ܐܢ (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      ܐܢ (!insert)
        ܦܢܐ nullptr;

      ܐܢ (unlikely (!resize (pages.length + 1)))
	ܦܢܐ nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    ܦܢܐ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  ثابت page_t *page_for (hb_codepoint_t g) ثابت
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ܐܢ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ܐܢ (cached_page.major == major)
	ܦܢܐ &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    ܐܢ (!page_map.bfind (key, &i))
      ܦܢܐ nullptr;

    last_page_lookup = i;
    ܦܢܐ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned صحيح i)
  {
    أكد (i < page_map.length);
    ܦܢܐ pages.arrayZ[page_map.arrayZ[i].index];
  }
  ثابت page_t &page_at (unsigned صحيح i) ثابت
  {
    أكد (i < page_map.length);
    ܦܢܐ pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned صحيح get_major (hb_codepoint_t g) ثابت { ܦܢܐ g >> page_t::PAGE_BITS_LOG_2; }
  unsigned صحيح page_remainder (hb_codepoint_t g) ثابت { ܦܢܐ g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned صحيح major) ثابت { ܦܢܐ major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
