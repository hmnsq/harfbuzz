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

#شامل "hb.hh"
#شامل "hb-bit-page.hh"

ساختار hb_bit_set_t
{
  hb_bit_set_t () = پیشفرض;
  ~hb_bit_set_t () = پیشفرض;

  hb_bit_set_t (ثابت hb_bit_set_t& other) : hb_bit_set_t () { set (other, درست); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*این, other); }
  hb_bit_set_t& operator= (ثابت hb_bit_set_t& other) { set (other); بازگشت *این; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*این, other); بازگشت *این; }
  friend تهی swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    اگر (likely (!a.successful || !b.successful))
      بازگشت;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  تهی init ()
  {
    successful = درست;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  تهی fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  ساختار page_map_t
  {
    صحیح cmp (ثابت page_map_t &o) ثابت { بازگشت cmp (o.major); }
    صحیح cmp (uint32_t o_major) ثابت { بازگشت (صحیح) o_major - (صحیح) major; }

    uint32_t major;
    uint32_t index;
  };

  منطقی successful = درست; /* Allocations successful */
  mutable unsigned صحیح population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  تهی err () { اگر (successful) successful = نادرست; } /* TODO Remove */
  منطقی in_error () ثابت { بازگشت !successful; }

  منطقی resize (unsigned صحیح شمار, منطقی clear = درست, منطقی exact_size = نادرست)
  {
    اگر (unlikely (!successful)) بازگشت نادرست;

    اگر (pages.length < شمار && (unsigned) pages.allocated < شمار && شمار <= 2)
      exact_size = درست; // Most sets are small and local

    اگر (unlikely (!pages.resize_full (شمار, clear, exact_size) ||
	!page_map.resize_full (شمار, clear, نادرست)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = نادرست;
      بازگشت نادرست;
    }
    بازگشت درست;
  }

  تهی alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = درست;
    clear ();
    بازگشت *این;
  }

  تهی clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    اگر (!pages.length && !population) بازگشت;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  منطقی is_empty () ثابت
  {
    unsigned صحیح شمار = pages.length;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (!pages[i].is_empty ())
	بازگشت نادرست;
    بازگشت درست;
  }
  explicit operator منطقی () ثابت { بازگشت !is_empty (); }

  uint32_t hash () ثابت
  {
    uint32_t h = 0;
    برای (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      اگر (unlikely (page.is_empty ())) ادامه;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    بازگشت h;
  }

  خصوصی:
  تهی dirty () { population = UINT_MAX; }
  عمومی:

  تهی add (hb_codepoint_t g)
  {
    اگر (unlikely (!successful)) بازگشت;
    اگر (unlikely (g == INVALID)) بازگشت;
    dirty ();
    page_t *page = page_for (g, درست); اگر (unlikely (!page)) بازگشت;
    page->add (g);
  }
  تهی add_bits (hb_codepoint_t g, uint64_t bits)
  {
    اگر (unlikely (!successful) || unlikely (!bits)) بازگشت;
    أكد (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, درست); اگر (unlikely (!page)) بازگشت;
    page->add_bits (g, bits);
  }
  منطقی add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    اگر (unlikely (!successful)) بازگشت درست; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    اگر (unlikely (a > b || a == INVALID || b == INVALID)) بازگشت نادرست;
    dirty ();
    unsigned صحیح ma = get_major (a);
    unsigned صحیح mb = get_major (b);
    اگر (ma == mb)
    {
      page_t *page = page_for (a, درست); اگر (unlikely (!page)) بازگشت نادرست;
      page->add_range (a, b);
    }
    وگرنه
    {
      page_t *page = page_for (a, درست); اگر (unlikely (!page)) بازگشت نادرست;
      page->add_range (a, major_start (ma + 1) - 1);

      برای (unsigned صحیح m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), درست); اگر (unlikely (!page)) بازگشت نادرست;
	page->init1 ();
      }

      page = page_for (b, درست); اگر (unlikely (!page)) بازگشت نادرست;
      page->add_range (major_start (mb), b);
    }
    بازگشت درست;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  ایستا درونخطی ثابت Type& StructAtOffsetUnaligned(ثابت تهی *P, unsigned صحیح offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    بازگشت * reinterpret_cast<ثابت Type*> ((ثابت نویسه *) P + offset);
#pragma GCC diagnostic برداشت
  }

  template <typename T>
  تهی set_array (منطقی v, ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  {
    اگر (unlikely (!successful)) بازگشت;
    اگر (!شمار) بازگشت;
    dirty ();
    hb_codepoint_t g = *array;
    وقتی (شمار)
    {
      unsigned صحیح m = get_major (g);
      page_t *page = page_for (g, v); اگر (unlikely (v && !page)) بازگشت;
      unsigned صحیح start = major_start (m);
      unsigned صحیح end = major_start (m + 1);
      انجامبده
      {
        اگر (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	شمار--;
      }
      وقتی (شمار && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  تهی add_array (ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  { set_array (درست, array, شمار, stride); }
  template <typename T>
  تهی add_array (ثابت hb_array_t<ثابت T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  تهی del_array (ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  { set_array (نادرست, array, شمار, stride); }
  template <typename T>
  تهی del_array (ثابت hb_array_t<ثابت T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  منطقی set_sorted_array (منطقی v, ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  {
    اگر (unlikely (!successful)) بازگشت درست; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    اگر (unlikely (!شمار)) بازگشت درست;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    وقتی (شمار)
    {
      unsigned صحیح m = get_major (g);
      page_t *page = page_for (g, v); اگر (unlikely (v && !page)) بازگشت نادرست;
      unsigned صحیح end = major_start (m + 1);
      انجامبده
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	اگر (g < last_g) بازگشت نادرست;
	last_g = g;

        اگر (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	شمار--;
      }
      وقتی (شمار && (g = *array, g < end));
    }
    بازگشت درست;
  }

  template <typename T>
  منطقی add_sorted_array (ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  { بازگشت set_sorted_array (درست, array, شمار, stride); }
  template <typename T>
  منطقی add_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { بازگشت add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  منطقی del_sorted_array (ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  { بازگشت set_sorted_array (نادرست, array, شمار, stride); }
  template <typename T>
  منطقی del_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { بازگشت del_sorted_array (&arr, arr.len ()); }

  تهی del (hb_codepoint_t g)
  {
    اگر (unlikely (!successful)) بازگشت;
    page_t *page = page_for (g);
    اگر (!page)
      بازگشت;
    dirty ();
    page->del (g);
  }
  تهی del_bits (hb_codepoint_t g, uint64_t bits)
  {
    اگر (unlikely (!successful) || unlikely (!bits)) بازگشت;
    أكد (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    اگر (!page)
      بازگشت;
    dirty ();
    page->del_bits (g, bits);
  }

  خصوصی:
  تهی del_pages (صحیح ds, صحیح de)
  {
    اگر (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      اگر (unlikely (!allocate_compact_workspace (compact_workspace))) بازگشت;

      unsigned صحیح write_index = 0;
      برای (unsigned صحیح i = 0; i < page_map.length; i++)
      {
	صحیح m = (صحیح) page_map.arrayZ[i].major;
	اگر (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  عمومی:
  تهی del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    اگر (unlikely (!successful)) بازگشت;
    اگر (unlikely (a > b || a == INVALID)) بازگشت;
    dirty ();
    unsigned صحیح ma = get_major (a);
    unsigned صحیح mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    صحیح ds = (a == major_start (ma))? (صحیح) ma: (صحیح) (ma + 1);
    صحیح de = (b + 1 == major_start (mb + 1))? (صحیح) mb: ((صحیح) mb - 1);
    اگر (ds > de || (صحیح) ma < ds)
    {
      page_t *page = page_for (a);
      اگر (page)
      {
	اگر (ma == mb)
	  page->del_range (a, b);
	وگرنه
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    اگر (de < (صحیح) mb && ma != mb)
    {
      page_t *page = page_for (b);
      اگر (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  منطقی get (hb_codepoint_t g) ثابت
  {
    ثابت page_t *page = page_for (g);
    اگر (!page)
      بازگشت نادرست;
    بازگشت page->get (g);
  }
  منطقی may_have (hb_codepoint_t g) ثابت { بازگشت get (g); }

  /* Has interface. */
  منطقی operator [] (hb_codepoint_t k) ثابت { بازگشت get (k); }
  منطقی has (hb_codepoint_t k) ثابت { بازگشت (*این)[k]; }
  /* Predicate. */
  منطقی operator () (hb_codepoint_t k) ثابت { بازگشت has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); بازگشت *این; }
  hb_bit_set_t& operator << (ثابت hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); بازگشت *این; }

  منطقی intersects (ثابت hb_bit_set_t &other) ثابت
  {
    unsigned صحیح na = pages.length;
    unsigned صحیح nb = other.pages.length;

    unsigned صحیح a = 0, b = 0;
    برای (; a < na && b < nb; )
    {
      اگر (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	اگر (page_at (a).intersects (other.page_at (b)))
	  بازگشت درست;
	a++;
	b++;
      }
      وگرنه اگر (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      وگرنه
	b++;
    }
    بازگشت نادرست;
  }
  منطقی may_intersect (ثابت hb_bit_set_t &other) ثابت
  { بازگشت intersects (other); }

  منطقی intersects (hb_codepoint_t first, hb_codepoint_t last) ثابت
  {
    hb_codepoint_t c = first - 1;
    بازگشت next (&c) && c <= last;
  }
  تهی set (ثابت hb_bit_set_t &other, منطقی exact_size = نادرست)
  {
    اگر (unlikely (!successful)) بازگشت;
    unsigned صحیح شمار = other.pages.length;
    اگر (unlikely (!resize  (شمار, نادرست, exact_size)))
      بازگشت;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  منطقی is_equal (ثابت hb_bit_set_t &other) ثابت
  {
    اگر (has_population () && other.has_population () &&
	population != other.population)
      بازگشت نادرست;

    unsigned صحیح na = pages.length;
    unsigned صحیح nb = other.pages.length;

    unsigned صحیح a = 0, b = 0;
    برای (; a < na && b < nb; )
    {
      اگر (page_at (a).is_empty ()) { a++; ادامه; }
      اگر (other.page_at (b).is_empty ()) { b++; ادامه; }
      اگر (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	بازگشت نادرست;
      a++;
      b++;
    }
    برای (; a < na; a++)
      اگر (!page_at (a).is_empty ()) { بازگشت نادرست; }
    برای (; b < nb; b++)
      اگر (!other.page_at (b).is_empty ()) { بازگشت نادرست; }

    بازگشت درست;
  }

  منطقی is_subset (ثابت hb_bit_set_t &larger_set) ثابت
  {
    اگر (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      بازگشت نادرست;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    وقتی (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      اگر (spm < lpm) {
        اگر (!sp.is_empty ())
          بازگشت نادرست;
        spi++;
        ادامه;
      }

      اگر (lpm < spm) {
        lpi++;
        ادامه;
      }

      auto lp = larger_set.page_at (lpi);
      اگر (!sp.is_subset (lp))
        بازگشت نادرست;

      spi++;
      lpi++;
    }

    وقتی (spi < page_map.length)
      اگر (!page_at (spi++).is_empty ())
        بازگشت نادرست;

    بازگشت درست;
  }

  خصوصی:
  منطقی allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    اگر (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = نادرست;
      بازگشت نادرست;
    }

    بازگشت درست;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  تهی compact (hb_vector_t<unsigned>& workspace,
                unsigned صحیح length)
  {
    أكد(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    برای (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  تهی compact_pages (ثابت hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned صحیح write_index = 0;
    برای (unsigned صحیح i = 0; i < pages.length; i++)
    {
      اگر (old_index_to_page_map_index[i] == 0xFFFFFFFF) ادامه;

      اگر (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  عمومی:

  تهی process_ (hb_bit_page_t::vector_t (*op) (ثابت hb_bit_page_t::vector_t &, ثابت hb_bit_page_t::vector_t &),
		 منطقی passthru_چپ, منطقی passthru_راست,
		 ثابت hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    اگر (unlikely (!successful)) بازگشت;

    dirty ();

    unsigned صحیح na = pages.length;
    unsigned صحیح nb = other.pages.length;
    unsigned صحیح next_page = na;

    unsigned صحیح شمار = 0, new_شمار = 0;
    unsigned صحیح a = 0, b = 0;
    unsigned صحیح write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    اگر (!passthru_چپ)
    {
      منطقی allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      اگر (unlikely (!allocated))
      {
	successful = نادرست;
	بازگشت;
      }
    }

    برای (; a < na && b < nb; )
    {
      اگر (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	اگر (!passthru_چپ)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  اگر (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	شمار++;
	a++;
	b++;
      }
      وگرنه اگر (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	اگر (passthru_چپ)
	  شمار++;
	a++;
      }
      وگرنه
      {
	اگر (passthru_راست)
	  شمار++;
	b++;
      }
    }
    اگر (passthru_چپ)
      شمار += na - a;
    اگر (passthru_راست)
      شمار += nb - b;

    اگر (!passthru_چپ)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    اگر (unlikely (!resize (شمار)))
      بازگشت;

    new_شمار = شمار;

    /* Process in-place backward. */
    a = na;
    b = nb;
    برای (; a && b; )
    {
      اگر (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	شمار--;
	page_map.arrayZ[شمار] = page_map.arrayZ[a];
	page_at (شمار).v = op (page_at (a).v, other.page_at (b).v);
	page_at (شمار).dirty ();
      }
      وگرنه اگر (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	اگر (passthru_چپ)
	{
	  شمار--;
	  page_map.arrayZ[شمار] = page_map.arrayZ[a];
	}
      }
      وگرنه
      {
	b--;
	اگر (passthru_راست)
	{
	  شمار--;
	  page_map.arrayZ[شمار].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[شمار].index = next_page++;
	  page_at (شمار) = other.page_at (b);
	}
      }
    }
    اگر (passthru_چپ)
      وقتی (a)
      {
	a--;
	شمار--;
	page_map.arrayZ[شمار] = page_map.arrayZ[a];
      }
    اگر (passthru_راست)
      وقتی (b)
      {
	b--;
	شمار--;
	page_map.arrayZ[شمار].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[شمار].index = next_page++;
	page_at (شمار) = other.page_at (b);
      }
    أكد (!شمار);
    resize (new_شمار);
  }
  template <typename Op>
  ایستا hb_bit_page_t::vector_t
  op_ (ثابت hb_bit_page_t::vector_t &a, ثابت hb_bit_page_t::vector_t &b)
  { بازگشت Op{} (a, b); }
  template <typename Op>
  تهی process (ثابت Op& op,
		ثابت hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  تهی union_ (ثابت hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  تهی intersect (ثابت hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  تهی subtract (ثابت hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  تهی symmetric_difference (ثابت hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  منطقی next (hb_codepoint_t *codepoint) ثابت
  {
    اگر (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      بازگشت *codepoint != INVALID;
    }

    ثابت auto* page_map_array = page_map.arrayZ;
    unsigned صحیح major = get_major (*codepoint);
    unsigned صحیح i = last_page_lookup;

    اگر (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      اگر (i >= page_map.length) {
        *codepoint = INVALID;
        بازگشت نادرست;
      }
      last_page_lookup = i;
    }

    ثابت auto* pages_array = pages.arrayZ;
    ثابت page_map_t &current = page_map_array[i];
    اگر (likely (current.major == major))
    {
      اگر (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        بازگشت درست;
      }
      i++;
    }

    برای (; i < page_map.length; i++)
    {
      ثابت page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      اگر (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	بازگشت درست;
      }
    }
    *codepoint = INVALID;
    بازگشت نادرست;
  }
  منطقی next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ثابت
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned صحیح i = 0;
    unsigned صحیح elt = 0;
    اگر (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      اگر (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        برو done;
      base += page_t::ELT_BITS;

      unsigned صحیح major = get_major (base);
      i = last_page_lookup;
      اگر (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	اگر (i >= page_map.length)
	  برو done;
      }
      اگر (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    برای (; i < page_map.length; i++, elt = 0)
    {
      ثابت page_map_t &map = page_map.arrayZ[i];
      ثابت page_t &page = pages.arrayZ[map.index];
      برای (; elt < page_t::len (); elt++)
	اگر (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  بازگشت درست;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    بازگشت نادرست;
  }
  منطقی previous (hb_codepoint_t *codepoint) ثابت
  {
    اگر (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      بازگشت *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned صحیح i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    اگر (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      اگر (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	بازگشت درست;
      }
    }
    i--;
    برای (; (صحیح) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      اگر (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	بازگشت درست;
      }
    }
    *codepoint = INVALID;
    بازگشت نادرست;
  }
  منطقی next_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    hb_codepoint_t i;

    i = *last;
    اگر (!next (&i))
    {
      *last = *first = INVALID;
      بازگشت نادرست;
    }

    *last = *first = i;

    ثابت auto* page_map_array = page_map.arrayZ;
    ثابت auto* pages_array = pages.arrayZ;
    unsigned صحیح major = get_major (*last);
    unsigned صحیح p_index = last_page_lookup;

    اگر (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      أكد(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned صحیح rem = page_remainder (*last);
    unsigned صحیح elt = rem / page_t::ELT_BITS;
    unsigned صحیح bit = rem & page_t::ELT_MASK;

    ثابت page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned صحیح run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    اگر (run < page_t::ELT_BITS - bit)
      بازگشت درست;

    elt++;

    وقتی (درست)
    {
      برای (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        اگر (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          ادامه;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        بازگشت درست;
      }

      p_index++;


      اگر (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        بازگشت درست;

      ثابت auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  منطقی previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    hb_codepoint_t i;

    i = *first;
    اگر (!previous (&i))
    {
      *last = *first = INVALID;
      بازگشت نادرست;
    }

    *last = *first = i;

    ثابت auto* page_map_array = page_map.arrayZ;
    ثابت auto* pages_array = pages.arrayZ;
    unsigned صحیح major = get_major (*first);
    unsigned صحیح p_index = last_page_lookup;

    اگر (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      أكد(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned صحیح rem = page_remainder (*first);
    صحیح elt = rem / page_t::ELT_BITS;
    unsigned صحیح bit = rem & page_t::ELT_MASK;

    ثابت page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned صحیح run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    اگر (run < bit + 1)
      بازگشت درست;

    elt--;

    وقتی (درست)
    {
      برای (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        اگر (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          ادامه;
        }
        uint64_t inv = ~w;
        unsigned صحیح zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        بازگشت درست;
      }


      اگر (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        بازگشت درست;

      p_index--;
      ثابت auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned صحیح next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned صحیح    size) ثابت
  {
    // By default, start at the first bit of the first page of values.
    unsigned صحیح start_page = 0;
    unsigned صحیح start_page_مقدار = 0;
    اگر (unlikely (codepoint != INVALID))
    {
      ثابت auto* page_map_array = page_map.arrayZ;
      unsigned صحیح major = get_major (codepoint);
      unsigned صحیح i = last_page_lookup;
      اگر (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	اگر (i >= page_map.length)
	  بازگشت 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      اگر (page_map_array[i].major == major)
      {
        start_page_مقدار = page_remainder (codepoint + 1);
        اگر (unlikely (start_page_مقدار == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_مقدار = 0;
        }
      }
    }

    unsigned صحیح initial_size = size;
    برای (unsigned صحیح i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned صحیح n = pages[page_map.arrayZ[i].index].اكتب (base, start_page_مقدار, out, size);
      out += n;
      size -= n;
      start_page_مقدار = 0;
    }
    بازگشت initial_size - size;
  }

  unsigned صحیح next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned صحیح    size) ثابت
  {
    unsigned صحیح initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned صحیح start_page = 0;
    unsigned صحیح start_page_مقدار = 0;
    اگر (unlikely (codepoint != INVALID))
    {
      ثابت auto* page_map_array = page_map.arrayZ;
      unsigned صحیح major = get_major (codepoint);
      unsigned صحیح i = last_page_lookup;
      اگر (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        اگر (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          وقتی (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          بازگشت initial_size - size;
        }
      }
      start_page = i;
      اگر (page_map_array[i].major == major)
      {
        start_page_مقدار = page_remainder (codepoint + 1);
        اگر (unlikely (start_page_مقدار == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_مقدار = 0;
        }
      }
    }

    hb_codepoint_t next_مقدار = codepoint + 1;
    برای (unsigned صحیح i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned صحیح n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_مقدار, out, size, &next_مقدار);
      out += n;
      size -= n;
      start_page_مقدار = 0;
    }
    وقتی (next_مقدار < HB_SET_مقدار_INVALID && size) {
      *out++ = next_مقدار++;
      size--;
    }
    بازگشت initial_size - size;
  }

  منطقی has_population () ثابت { بازگشت population != UINT_MAX; }
  unsigned صحیح get_population () ثابت
  {
    اگر (has_population ())
      بازگشت population;

    unsigned صحیح برداشت = 0;
    unsigned صحیح شمار = pages.length;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      برداشت += pages[i].get_population ();

    population = برداشت;
    بازگشت برداشت;
  }
  منطقی get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    اگر (has_population ())
    {
      اگر (population != 1) بازگشت نادرست;
      *codepoint = get_min ();
      بازگشت درست;
    }

    hb_codepoint_t singleton = INVALID;
    برای (ثابت auto &map : page_map)
    {
      ثابت auto &page = pages.arrayZ[map.index];
      برای (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	اگر (!bits) ادامه;
	اگر (singleton != INVALID || (bits & (bits - 1)))
	  بازگشت نادرست;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    اگر (singleton == INVALID)
    {
      population = 0;
      بازگشت نادرست;
    }
    population = 1;
    *codepoint = singleton;
    بازگشت درست;
  }
  hb_codepoint_t get_min () ثابت
  {
    unsigned شمار = pages.length;
    برای (unsigned i = 0; i < شمار; i++)
    {
      ثابت auto& map = page_map.arrayZ[i];
      ثابت auto& page = pages.arrayZ[map.index];

      اگر (!page.is_empty ())
	بازگشت map.major * page_t::PAGE_BITS + page.get_min ();
    }
    بازگشت INVALID;
  }
  hb_codepoint_t get_max () ثابت
  {
    unsigned شمار = pages.length;
    برای (signed i = شمار - 1; i >= 0; i--)
    {
      ثابت auto& map = page_map.arrayZ[(unsigned) i];
      ثابت auto& page = pages.arrayZ[map.index];

      اگر (!page.is_empty ())
	بازگشت map.major * page_t::PAGE_BITS + page.get_max ();
    }
    بازگشت INVALID;
  }

  ایستا constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  ساختار iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ایستا constexpr منطقی is_sorted_iterator = درست;
    ایستا constexpr منطقی has_fast_len = درست;
    iter_t (ثابت hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    منطقی init = درست) : s (&s_), v (INVALID), l(0)
    {
      اگر (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    تعریفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { بازگشت v; }
    منطقی __more__ () ثابت { بازگشت v != INVALID; }
    تهی __next__ () { s->next (&v); اگر (l) l--; }
    تهی __prev__ () { s->previous (&v); }
    unsigned __len__ () ثابت { بازگشت l; }
    iter_t end () ثابت { بازگشت iter_t (*s, نادرست); }
    منطقی operator != (ثابت iter_t& o) ثابت
    { بازگشت v != o.v; }

    محافظتشده:
    ثابت hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () ثابت { بازگشت iter_t (*این); }
  operator iter_t () ثابت { بازگشت iter (); }

  محافظتشده:

  page_t *page_for (hb_codepoint_t g, منطقی insert = نادرست)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    اگر (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      اگر (cached_page.major == major)
	بازگشت &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    اگر (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      اگر (!insert)
        بازگشت nullptr;

      اگر (unlikely (!resize (pages.length + 1)))
	بازگشت nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    بازگشت &pages.arrayZ[page_map.arrayZ[i].index];
  }
  ثابت page_t *page_for (hb_codepoint_t g) ثابت
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    اگر (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      اگر (cached_page.major == major)
	بازگشت &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    اگر (!page_map.bfind (key, &i))
      بازگشت nullptr;

    last_page_lookup = i;
    بازگشت &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned صحیح i)
  {
    أكد (i < page_map.length);
    بازگشت pages.arrayZ[page_map.arrayZ[i].index];
  }
  ثابت page_t &page_at (unsigned صحیح i) ثابت
  {
    أكد (i < page_map.length);
    بازگشت pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned صحیح get_major (hb_codepoint_t g) ثابت { بازگشت g >> page_t::PAGE_BITS_LOG_2; }
  unsigned صحیح page_remainder (hb_codepoint_t g) ثابت { بازگشت g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned صحیح major) ثابت { بازگشت major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
