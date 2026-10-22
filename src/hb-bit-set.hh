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

  hb_bit_set_t (ثابت hb_bit_set_t& other) : hb_bit_set_t () { set (other, אמת); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*هذا, other); }
  hb_bit_set_t& operator= (ثابت hb_bit_set_t& other) { set (other); החזר *هذا; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*هذا, other); החזר *هذا; }
  friend فراغ swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    אם (likely (!a.successful || !b.successful))
      החזר;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  فراغ init ()
  {
    successful = אמת;
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
    صحيح cmp (ثابت page_map_t &o) ثابت { החזר cmp (o.major); }
    صحيح cmp (uint32_t o_major) ثابت { החזר (صحيح) o_major - (صحيح) major; }

    uint32_t major;
    uint32_t index;
  };

  منطقي successful = אמת; /* Allocations successful */
  mutable unsigned صحيح population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  فراغ err () { אם (successful) successful = שקר; } /* TODO Remove */
  منطقي in_error () ثابت { החזר !successful; }

  منطقي resize (unsigned صحيح count, منطقي clear = אמת, منطقي exact_size = שקר)
  {
    אם (unlikely (!successful)) החזר שקר;

    אם (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = אמת; // Most sets are small and local

    אם (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, שקר)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = שקר;
      החזר שקר;
    }
    החזר אמת;
  }

  فراغ alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = אמת;
    clear ();
    החזר *هذا;
  }

  فراغ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    אם (!pages.length && !population) החזר;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  منطقي is_empty () ثابت
  {
    unsigned صحيح count = pages.length;
    עבור (unsigned صحيح i = 0; i < count; i++)
      אם (!pages[i].is_empty ())
	החזר שקר;
    החזר אמת;
  }
  explicit operator منطقي () ثابت { החזר !is_empty (); }

  uint32_t hash () ثابت
  {
    uint32_t h = 0;
    עבור (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      אם (unlikely (page.is_empty ())) استمر;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    החזר h;
  }

  خاص:
  فراغ dirty () { population = UINT_MAX; }
  عام:

  فراغ add (hb_codepoint_t g)
  {
    אם (unlikely (!successful)) החזר;
    אם (unlikely (g == INVALID)) החזר;
    dirty ();
    page_t *page = page_for (g, אמת); אם (unlikely (!page)) החזר;
    page->add (g);
  }
  فراغ add_bits (hb_codepoint_t g, uint64_t bits)
  {
    אם (unlikely (!successful) || unlikely (!bits)) החזר;
    أكد (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, אמת); אם (unlikely (!page)) החזר;
    page->add_bits (g, bits);
  }
  منطقي add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    אם (unlikely (!successful)) החזר אמת; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    אם (unlikely (a > b || a == INVALID || b == INVALID)) החזר שקר;
    dirty ();
    unsigned صحيح ma = get_major (a);
    unsigned صحيح mb = get_major (b);
    אם (ma == mb)
    {
      page_t *page = page_for (a, אמת); אם (unlikely (!page)) החזר שקר;
      page->add_range (a, b);
    }
    אחרת
    {
      page_t *page = page_for (a, אמת); אם (unlikely (!page)) החזר שקר;
      page->add_range (a, major_start (ma + 1) - 1);

      עבור (unsigned صحيح m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), אמת); אם (unlikely (!page)) החזר שקר;
	page->init1 ();
      }

      page = page_for (b, אמת); אם (unlikely (!page)) החזר שקר;
      page->add_range (major_start (mb), b);
    }
    החזר אמת;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  ساكن مضمن ثابت Type& StructAtOffsetUnaligned(ثابت فراغ *P, unsigned صحيح offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    החזר * reinterpret_cast<ثابت Type*> ((ثابت حرف *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  فراغ set_array (منطقي v, ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  {
    אם (unlikely (!successful)) החזר;
    אם (!count) החזר;
    dirty ();
    hb_codepoint_t g = *array;
    כלעוד (count)
    {
      unsigned صحيح m = get_major (g);
      page_t *page = page_for (g, v); אם (unlikely (v && !page)) החזר;
      unsigned صحيح start = major_start (m);
      unsigned صحيح end = major_start (m + 1);
      עשה
      {
        אם (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      כלעוד (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  فراغ add_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { set_array (אמת, array, count, stride); }
  template <typename T>
  فراغ add_array (ثابت hb_array_t<ثابت T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  فراغ del_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { set_array (שקר, array, count, stride); }
  template <typename T>
  فراغ del_array (ثابت hb_array_t<ثابت T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  منطقي set_sorted_array (منطقي v, ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  {
    אם (unlikely (!successful)) החזר אמת; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    אם (unlikely (!count)) החזר אמת;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    כלעוד (count)
    {
      unsigned صحيح m = get_major (g);
      page_t *page = page_for (g, v); אם (unlikely (v && !page)) החזר שקר;
      unsigned صحيح end = major_start (m + 1);
      עשה
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	אם (g < last_g) החזר שקר;
	last_g = g;

        אם (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      כלעוד (count && (g = *array, g < end));
    }
    החזר אמת;
  }

  template <typename T>
  منطقي add_sorted_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { החזר set_sorted_array (אמת, array, count, stride); }
  template <typename T>
  منطقي add_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { החזר add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  منطقي del_sorted_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { החזר set_sorted_array (שקר, array, count, stride); }
  template <typename T>
  منطقي del_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { החזר del_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g)
  {
    אם (unlikely (!successful)) החזר;
    page_t *page = page_for (g);
    אם (!page)
      החזר;
    dirty ();
    page->del (g);
  }
  فراغ del_bits (hb_codepoint_t g, uint64_t bits)
  {
    אם (unlikely (!successful) || unlikely (!bits)) החזר;
    أكد (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    אם (!page)
      החזר;
    dirty ();
    page->del_bits (g, bits);
  }

  خاص:
  فراغ del_pages (صحيح ds, صحيح de)
  {
    אם (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      אם (unlikely (!allocate_compact_workspace (compact_workspace))) החזר;

      unsigned صحيح write_index = 0;
      עבור (unsigned صحيح i = 0; i < page_map.length; i++)
      {
	صحيح m = (صحيح) page_map.arrayZ[i].major;
	אם (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  عام:
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    אם (unlikely (!successful)) החזר;
    אם (unlikely (a > b || a == INVALID)) החזר;
    dirty ();
    unsigned صحيح ma = get_major (a);
    unsigned صحيح mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    صحيح ds = (a == major_start (ma))? (صحيح) ma: (صحيح) (ma + 1);
    صحيح de = (b + 1 == major_start (mb + 1))? (صحيح) mb: ((صحيح) mb - 1);
    אם (ds > de || (صحيح) ma < ds)
    {
      page_t *page = page_for (a);
      אם (page)
      {
	אם (ma == mb)
	  page->del_range (a, b);
	אחרת
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    אם (de < (صحيح) mb && ma != mb)
    {
      page_t *page = page_for (b);
      אם (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  منطقي get (hb_codepoint_t g) ثابت
  {
    ثابت page_t *page = page_for (g);
    אם (!page)
      החזר שקר;
    החזר page->get (g);
  }
  منطقي may_have (hb_codepoint_t g) ثابت { החזר get (g); }

  /* Has interface. */
  منطقي operator [] (hb_codepoint_t k) ثابت { החזר get (k); }
  منطقي has (hb_codepoint_t k) ثابت { החזר (*هذا)[k]; }
  /* Predicate. */
  منطقي operator () (hb_codepoint_t k) ثابت { החזר has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); החזר *هذا; }
  hb_bit_set_t& operator << (ثابت hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); החזר *هذا; }

  منطقي intersects (ثابت hb_bit_set_t &other) ثابت
  {
    unsigned صحيح na = pages.length;
    unsigned صحيح nb = other.pages.length;

    unsigned صحيح a = 0, b = 0;
    עבור (; a < na && b < nb; )
    {
      אם (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	אם (page_at (a).intersects (other.page_at (b)))
	  החזר אמת;
	a++;
	b++;
      }
      אחרת אם (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      אחרת
	b++;
    }
    החזר שקר;
  }
  منطقي may_intersect (ثابت hb_bit_set_t &other) ثابت
  { החזר intersects (other); }

  منطقي intersects (hb_codepoint_t first, hb_codepoint_t last) ثابت
  {
    hb_codepoint_t c = first - 1;
    החזר next (&c) && c <= last;
  }
  فراغ set (ثابت hb_bit_set_t &other, منطقي exact_size = שקר)
  {
    אם (unlikely (!successful)) החזר;
    unsigned صحيح count = other.pages.length;
    אם (unlikely (!resize  (count, שקר, exact_size)))
      החזר;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  منطقي is_equal (ثابت hb_bit_set_t &other) ثابت
  {
    אם (has_population () && other.has_population () &&
	population != other.population)
      החזר שקר;

    unsigned صحيح na = pages.length;
    unsigned صحيح nb = other.pages.length;

    unsigned صحيح a = 0, b = 0;
    עבור (; a < na && b < nb; )
    {
      אם (page_at (a).is_empty ()) { a++; استمر; }
      אם (other.page_at (b).is_empty ()) { b++; استمر; }
      אם (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	החזר שקר;
      a++;
      b++;
    }
    עבור (; a < na; a++)
      אם (!page_at (a).is_empty ()) { החזר שקר; }
    עבור (; b < nb; b++)
      אם (!other.page_at (b).is_empty ()) { החזר שקר; }

    החזר אמת;
  }

  منطقي is_subset (ثابت hb_bit_set_t &larger_set) ثابت
  {
    אם (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      החזר שקר;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    כלעוד (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      אם (spm < lpm) {
        אם (!sp.is_empty ())
          החזר שקר;
        spi++;
        استمر;
      }

      אם (lpm < spm) {
        lpi++;
        استمر;
      }

      auto lp = larger_set.page_at (lpi);
      אם (!sp.is_subset (lp))
        החזר שקר;

      spi++;
      lpi++;
    }

    כלעוד (spi < page_map.length)
      אם (!page_at (spi++).is_empty ())
        החזר שקר;

    החזר אמת;
  }

  خاص:
  منطقي allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    אם (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = שקר;
      החזר שקר;
    }

    החזר אמת;
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
    עבור (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  فراغ compact_pages (ثابت hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned صحيح write_index = 0;
    עבור (unsigned صحيح i = 0; i < pages.length; i++)
    {
      אם (old_index_to_page_map_index[i] == 0xFFFFFFFF) استمر;

      אם (write_index < i)
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
    אם (unlikely (!successful)) החזר;

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
    אם (!passthru_left)
    {
      منطقي allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      אם (unlikely (!allocated))
      {
	successful = שקר;
	החזר;
      }
    }

    עבור (; a < na && b < nb; )
    {
      אם (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	אם (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  אם (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      אחרת אם (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	אם (passthru_left)
	  count++;
	a++;
      }
      אחרת
      {
	אם (passthru_right)
	  count++;
	b++;
      }
    }
    אם (passthru_left)
      count += na - a;
    אם (passthru_right)
      count += nb - b;

    אם (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    אם (unlikely (!resize (count)))
      החזר;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    עבור (; a && b; )
    {
      אם (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      אחרת אם (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	אם (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      אחרת
      {
	b--;
	אם (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    אם (passthru_left)
      כלעוד (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    אם (passthru_right)
      כלעוד (b)
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
  { החזר Op{} (a, b); }
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
    אם (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      החזר *codepoint != INVALID;
    }

    ثابت auto* page_map_array = page_map.arrayZ;
    unsigned صحيح major = get_major (*codepoint);
    unsigned صحيح i = last_page_lookup;

    אם (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      אם (i >= page_map.length) {
        *codepoint = INVALID;
        החזר שקר;
      }
      last_page_lookup = i;
    }

    ثابت auto* pages_array = pages.arrayZ;
    ثابت page_map_t &current = page_map_array[i];
    אם (likely (current.major == major))
    {
      אם (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        החזר אמת;
      }
      i++;
    }

    עבור (; i < page_map.length; i++)
    {
      ثابت page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      אם (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	החזר אמת;
      }
    }
    *codepoint = INVALID;
    החזר שקר;
  }
  منطقي next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ثابت
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned صحيح i = 0;
    unsigned صحيح elt = 0;
    אם (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      אם (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        اذهب done;
      base += page_t::ELT_BITS;

      unsigned صحيح major = get_major (base);
      i = last_page_lookup;
      אם (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	אם (i >= page_map.length)
	  اذهب done;
      }
      אם (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    עבור (; i < page_map.length; i++, elt = 0)
    {
      ثابت page_map_t &map = page_map.arrayZ[i];
      ثابت page_t &page = pages.arrayZ[map.index];
      עבור (; elt < page_t::len (); elt++)
	אם (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  החזר אמת;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    החזר שקר;
  }
  منطقي previous (hb_codepoint_t *codepoint) ثابت
  {
    אם (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      החזר *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned صحيح i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    אם (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      אם (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	החזר אמת;
      }
    }
    i--;
    עבור (; (صحيح) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      אם (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	החזר אמת;
      }
    }
    *codepoint = INVALID;
    החזר שקר;
  }
  منطقي next_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    hb_codepoint_t i;

    i = *last;
    אם (!next (&i))
    {
      *last = *first = INVALID;
      החזר שקר;
    }

    *last = *first = i;

    ثابت auto* page_map_array = page_map.arrayZ;
    ثابت auto* pages_array = pages.arrayZ;
    unsigned صحيح major = get_major (*last);
    unsigned صحيح p_index = last_page_lookup;

    אם (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
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
    אם (run < page_t::ELT_BITS - bit)
      החזר אמת;

    elt++;

    כלעוד (אמת)
    {
      עבור (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        אם (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          استمر;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        החזר אמת;
      }

      p_index++;


      אם (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        החזר אמת;

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
    אם (!previous (&i))
    {
      *last = *first = INVALID;
      החזר שקר;
    }

    *last = *first = i;

    ثابت auto* page_map_array = page_map.arrayZ;
    ثابت auto* pages_array = pages.arrayZ;
    unsigned صحيح major = get_major (*first);
    unsigned صحيح p_index = last_page_lookup;

    אם (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
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
    אם (run < bit + 1)
      החזר אמת;

    elt--;

    כלעוד (אמת)
    {
      עבור (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        אם (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          استمر;
        }
        uint64_t inv = ~w;
        unsigned صحيح zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        החזר אמת;
      }


      אם (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        החזר אמת;

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
    אם (unlikely (codepoint != INVALID))
    {
      ثابت auto* page_map_array = page_map.arrayZ;
      unsigned صحيح major = get_major (codepoint);
      unsigned صحيح i = last_page_lookup;
      אם (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	אם (i >= page_map.length)
	  החזר 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      אם (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        אם (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned صحيح initial_size = size;
    עבור (unsigned صحيح i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned صحيح n = pages[page_map.arrayZ[i].index].اكتب (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    החזר initial_size - size;
  }

  unsigned صحيح next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned صحيح    size) ثابت
  {
    unsigned صحيح initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned صحيح start_page = 0;
    unsigned صحيح start_page_value = 0;
    אם (unlikely (codepoint != INVALID))
    {
      ثابت auto* page_map_array = page_map.arrayZ;
      unsigned صحيح major = get_major (codepoint);
      unsigned صحيح i = last_page_lookup;
      אם (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        אם (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          כלעוד (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          החזר initial_size - size;
        }
      }
      start_page = i;
      אם (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        אם (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    עבור (unsigned صحيح i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned صحيح n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    כלעוד (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    החזר initial_size - size;
  }

  منطقي has_population () ثابت { החזר population != UINT_MAX; }
  unsigned صحيح get_population () ثابت
  {
    אם (has_population ())
      החזר population;

    unsigned صحيح pop = 0;
    unsigned صحيح count = pages.length;
    עבור (unsigned صحيح i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    החזר pop;
  }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    אם (has_population ())
    {
      אם (population != 1) החזר שקר;
      *codepoint = get_min ();
      החזר אמת;
    }

    hb_codepoint_t singleton = INVALID;
    עבור (ثابت auto &map : page_map)
    {
      ثابت auto &page = pages.arrayZ[map.index];
      עבור (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	אם (!bits) استمر;
	אם (singleton != INVALID || (bits & (bits - 1)))
	  החזר שקר;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    אם (singleton == INVALID)
    {
      population = 0;
      החזר שקר;
    }
    population = 1;
    *codepoint = singleton;
    החזר אמת;
  }
  hb_codepoint_t get_min () ثابت
  {
    unsigned count = pages.length;
    עבור (unsigned i = 0; i < count; i++)
    {
      ثابت auto& map = page_map.arrayZ[i];
      ثابت auto& page = pages.arrayZ[map.index];

      אם (!page.is_empty ())
	החזר map.major * page_t::PAGE_BITS + page.get_min ();
    }
    החזר INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    unsigned count = pages.length;
    עבור (signed i = count - 1; i >= 0; i--)
    {
      ثابت auto& map = page_map.arrayZ[(unsigned) i];
      ثابت auto& page = pages.arrayZ[map.index];

      אם (!page.is_empty ())
	החזר map.major * page_t::PAGE_BITS + page.get_max ();
    }
    החזר INVALID;
  }

  ساكن constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = אמת;
    ساكن constexpr منطقي has_fast_len = אמת;
    iter_t (ثابت hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    منطقي init = אמת) : s (&s_), v (INVALID), l(0)
    {
      אם (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { החזר v; }
    منطقي __more__ () ثابت { החזר v != INVALID; }
    فراغ __next__ () { s->next (&v); אם (l) l--; }
    فراغ __prev__ () { s->previous (&v); }
    unsigned __len__ () ثابت { החזר l; }
    iter_t end () ثابت { החזר iter_t (*s, שקר); }
    منطقي operator != (ثابت iter_t& o) ثابت
    { החזר v != o.v; }

    محمي:
    ثابت hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () ثابت { החזר iter_t (*هذا); }
  operator iter_t () ثابت { החזר iter (); }

  محمي:

  page_t *page_for (hb_codepoint_t g, منطقي insert = שקר)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    אם (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      אם (cached_page.major == major)
	החזר &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    אם (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      אם (!insert)
        החזר nullptr;

      אם (unlikely (!resize (pages.length + 1)))
	החזר nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    החזר &pages.arrayZ[page_map.arrayZ[i].index];
  }
  ثابت page_t *page_for (hb_codepoint_t g) ثابت
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    אם (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      אם (cached_page.major == major)
	החזר &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    אם (!page_map.bfind (key, &i))
      החזר nullptr;

    last_page_lookup = i;
    החזר &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned صحيح i)
  {
    أكد (i < page_map.length);
    החזר pages.arrayZ[page_map.arrayZ[i].index];
  }
  ثابت page_t &page_at (unsigned صحيح i) ثابت
  {
    أكد (i < page_map.length);
    החזר pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned صحيح get_major (hb_codepoint_t g) ثابت { החזר g >> page_t::PAGE_BITS_LOG_2; }
  unsigned صحيح page_remainder (hb_codepoint_t g) ثابت { החזר g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned صحيح major) ثابت { החזר major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
