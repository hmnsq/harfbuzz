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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-bit-page.hh"

கட்டமைப்பு hb_bit_set_t
{
  hb_bit_set_t () = இயல்பு;
  ~hb_bit_set_t () = இயல்பு;

  hb_bit_set_t (மாறிலி hb_bit_set_t& other) : hb_bit_set_t () { set (other, உண்மை); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*இது, other); }
  hb_bit_set_t& operator= (மாறிலி hb_bit_set_t& other) { set (other); திரும்பு *இது; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*இது, other); திரும்பு *இது; }
  friend வெறுமை swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    எனில் (likely (!a.successful || !b.successful))
      திரும்பு;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  வெறுமை init ()
  {
    successful = உண்மை;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  வெறுமை fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  கட்டமைப்பு page_map_t
  {
    முழு cmp (மாறிலி page_map_t &o) மாறிலி { திரும்பு cmp (o.major); }
    முழு cmp (uint32_t o_major) மாறிலி { திரும்பு (முழு) o_major - (முழு) major; }

    uint32_t major;
    uint32_t index;
  };

  தருக்க successful = உண்மை; /* Allocations successful */
  mutable unsigned முழு population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  வெறுமை err () { எனில் (successful) successful = பொய்; } /* TODO Remove */
  தருக்க in_error () மாறிலி { திரும்பு !successful; }

  தருக்க resize (unsigned முழு மொத்தம், தருக்க clear = உண்மை, தருக்க exact_size = பொய்)
  {
    எனில் (unlikely (!successful)) திரும்பு பொய்;

    எனில் (pages.length < மொத்தம் && (unsigned) pages.allocated < மொத்தம் && மொத்தம் <= 2)
      exact_size = உண்மை; // Most sets are small and local

    எனில் (unlikely (!pages.resize_full (மொத்தம், clear, exact_size) ||
	!page_map.resize_full (மொத்தம், clear, பொய்)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = பொய்;
      திரும்பு பொய்;
    }
    திரும்பு உண்மை;
  }

  வெறுமை alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = உண்மை;
    clear ();
    திரும்பு *இது;
  }

  வெறுமை clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    எனில் (!pages.length && !population) திரும்பு;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  தருக்க is_empty () மாறிலி
  {
    unsigned முழு மொத்தம் = pages.length;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எனில் (!pages[i].is_empty ())
	திரும்பு பொய்;
    திரும்பு உண்மை;
  }
  explicit operator தருக்க () மாறிலி { திரும்பு !is_empty (); }

  uint32_t hash () மாறிலி
  {
    uint32_t h = 0;
    க்கு (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      எனில் (unlikely (page.is_empty ())) தொடர்;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    திரும்பு h;
  }

  தனிப்பட்ட:
  வெறுமை dirty () { population = UINT_MAX; }
  பொது:

  வெறுமை add (hb_codepoint_t g)
  {
    எனில் (unlikely (!successful)) திரும்பு;
    எனில் (unlikely (g == INVALID)) திரும்பு;
    dirty ();
    page_t *page = page_for (g, உண்மை); எனில் (unlikely (!page)) திரும்பு;
    page->add (g);
  }
  வெறுமை add_bits (hb_codepoint_t g, uint64_t bits)
  {
    எனில் (unlikely (!successful) || unlikely (!bits)) திரும்பு;
    ப்ரதிஜ்ஞா (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, உண்மை); எனில் (unlikely (!page)) திரும்பு;
    page->add_bits (g, bits);
  }
  தருக்க add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    எனில் (unlikely (!successful)) திரும்பு உண்மை; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    எனில் (unlikely (a > b || a == INVALID || b == INVALID)) திரும்பு பொய்;
    dirty ();
    unsigned முழு ma = get_major (a);
    unsigned முழு mb = get_major (b);
    எனில் (ma == mb)
    {
      page_t *page = page_for (a, உண்மை); எனில் (unlikely (!page)) திரும்பு பொய்;
      page->add_range (a, b);
    }
    இல்லையெனில்
    {
      page_t *page = page_for (a, உண்மை); எனில் (unlikely (!page)) திரும்பு பொய்;
      page->add_range (a, major_start (ma + 1) - 1);

      க்கு (unsigned முழு m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), உண்மை); எனில் (unlikely (!page)) திரும்பு பொய்;
	page->init1 ();
      }

      page = page_for (b, உண்மை); எனில் (unlikely (!page)) திரும்பு பொய்;
      page->add_range (major_start (mb), b);
    }
    திரும்பு உண்மை;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  நிலையான உள்வரி மாறிலி Type& StructAtOffsetUnaligned(மாறிலி வெறுமை *P, unsigned முழு offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    திரும்பு * reinterpret_cast<மாறிலி Type*> ((மாறிலி எழுத்து *) P + offset);
#pragma GCC diagnostic எடு
  }

  template <typename T>
  வெறுமை set_array (தருக்க v, மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  {
    எனில் (unlikely (!successful)) திரும்பு;
    எனில் (!மொத்தம்) திரும்பு;
    dirty ();
    hb_codepoint_t g = *array;
    போது (மொத்தம்)
    {
      unsigned முழு m = get_major (g);
      page_t *page = page_for (g, v); எனில் (unlikely (v && !page)) திரும்பு;
      unsigned முழு start = major_start (m);
      unsigned முழு end = major_start (m + 1);
      செய்
      {
        எனில் (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	மொத்தம்--;
      }
      போது (மொத்தம் && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  வெறுமை add_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  { set_array (உண்மை, array, மொத்தம், stride); }
  template <typename T>
  வெறுமை add_array (மாறிலி hb_array_t<மாறிலி T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  வெறுமை del_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  { set_array (பொய், array, மொத்தம், stride); }
  template <typename T>
  வெறுமை del_array (மாறிலி hb_array_t<மாறிலி T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  தருக்க set_sorted_array (தருக்க v, மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  {
    எனில் (unlikely (!successful)) திரும்பு உண்மை; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    எனில் (unlikely (!மொத்தம்)) திரும்பு உண்மை;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    போது (மொத்தம்)
    {
      unsigned முழு m = get_major (g);
      page_t *page = page_for (g, v); எனில் (unlikely (v && !page)) திரும்பு பொய்;
      unsigned முழு end = major_start (m + 1);
      செய்
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	எனில் (g < last_g) திரும்பு பொய்;
	last_g = g;

        எனில் (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	மொத்தம்--;
      }
      போது (மொத்தம் && (g = *array, g < end));
    }
    திரும்பு உண்மை;
  }

  template <typename T>
  தருக்க add_sorted_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  { திரும்பு set_sorted_array (உண்மை, array, மொத்தம், stride); }
  template <typename T>
  தருக்க add_sorted_array (மாறிலி hb_sorted_array_t<மாறிலி T>& arr) { திரும்பு add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  தருக்க del_sorted_array (மாறிலி T *array, unsigned முழு மொத்தம், unsigned முழு stride=அளவு(T))
  { திரும்பு set_sorted_array (பொய், array, மொத்தம், stride); }
  template <typename T>
  தருக்க del_sorted_array (மாறிலி hb_sorted_array_t<மாறிலி T>& arr) { திரும்பு del_sorted_array (&arr, arr.len ()); }

  வெறுமை del (hb_codepoint_t g)
  {
    எனில் (unlikely (!successful)) திரும்பு;
    page_t *page = page_for (g);
    எனில் (!page)
      திரும்பு;
    dirty ();
    page->del (g);
  }
  வெறுமை del_bits (hb_codepoint_t g, uint64_t bits)
  {
    எனில் (unlikely (!successful) || unlikely (!bits)) திரும்பு;
    ப்ரதிஜ்ஞா (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    எனில் (!page)
      திரும்பு;
    dirty ();
    page->del_bits (g, bits);
  }

  தனிப்பட்ட:
  வெறுமை del_pages (முழு ds, முழு de)
  {
    எனில் (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      எனில் (unlikely (!allocate_compact_workspace (compact_workspace))) திரும்பு;

      unsigned முழு write_index = 0;
      க்கு (unsigned முழு i = 0; i < page_map.length; i++)
      {
	முழு m = (முழு) page_map.arrayZ[i].major;
	எனில் (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  பொது:
  வெறுமை del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    எனில் (unlikely (!successful)) திரும்பு;
    எனில் (unlikely (a > b || a == INVALID)) திரும்பு;
    dirty ();
    unsigned முழு ma = get_major (a);
    unsigned முழு mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    முழு ds = (a == major_start (ma))? (முழு) ma: (முழு) (ma + 1);
    முழு de = (b + 1 == major_start (mb + 1))? (முழு) mb: ((முழு) mb - 1);
    எனில் (ds > de || (முழு) ma < ds)
    {
      page_t *page = page_for (a);
      எனில் (page)
      {
	எனில் (ma == mb)
	  page->del_range (a, b);
	இல்லையெனில்
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    எனில் (de < (முழு) mb && ma != mb)
    {
      page_t *page = page_for (b);
      எனில் (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  தருக்க get (hb_codepoint_t g) மாறிலி
  {
    மாறிலி page_t *page = page_for (g);
    எனில் (!page)
      திரும்பு பொய்;
    திரும்பு page->get (g);
  }
  தருக்க may_have (hb_codepoint_t g) மாறிலி { திரும்பு get (g); }

  /* Has interface. */
  தருக்க operator [] (hb_codepoint_t k) மாறிலி { திரும்பு get (k); }
  தருக்க has (hb_codepoint_t k) மாறிலி { திரும்பு (*இது)[k]; }
  /* Predicate. */
  தருக்க operator () (hb_codepoint_t k) மாறிலி { திரும்பு has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); திரும்பு *இது; }
  hb_bit_set_t& operator << (மாறிலி hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); திரும்பு *இது; }

  தருக்க intersects (மாறிலி hb_bit_set_t &other) மாறிலி
  {
    unsigned முழு na = pages.length;
    unsigned முழு nb = other.pages.length;

    unsigned முழு a = 0, b = 0;
    க்கு (; a < na && b < nb; )
    {
      எனில் (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	எனில் (page_at (a).intersects (other.page_at (b)))
	  திரும்பு உண்மை;
	a++;
	b++;
      }
      இல்லையெனில் எனில் (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      இல்லையெனில்
	b++;
    }
    திரும்பு பொய்;
  }
  தருக்க may_intersect (மாறிலி hb_bit_set_t &other) மாறிலி
  { திரும்பு intersects (other); }

  தருக்க intersects (hb_codepoint_t first, hb_codepoint_t last) மாறிலி
  {
    hb_codepoint_t c = first - 1;
    திரும்பு next (&c) && c <= last;
  }
  வெறுமை set (மாறிலி hb_bit_set_t &other, தருக்க exact_size = பொய்)
  {
    எனில் (unlikely (!successful)) திரும்பு;
    unsigned முழு மொத்தம் = other.pages.length;
    எனில் (unlikely (!resize  (மொத்தம், பொய், exact_size)))
      திரும்பு;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  தருக்க is_equal (மாறிலி hb_bit_set_t &other) மாறிலி
  {
    எனில் (has_population () && other.has_population () &&
	population != other.population)
      திரும்பு பொய்;

    unsigned முழு na = pages.length;
    unsigned முழு nb = other.pages.length;

    unsigned முழு a = 0, b = 0;
    க்கு (; a < na && b < nb; )
    {
      எனில் (page_at (a).is_empty ()) { a++; தொடர்; }
      எனில் (other.page_at (b).is_empty ()) { b++; தொடர்; }
      எனில் (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	திரும்பு பொய்;
      a++;
      b++;
    }
    க்கு (; a < na; a++)
      எனில் (!page_at (a).is_empty ()) { திரும்பு பொய்; }
    க்கு (; b < nb; b++)
      எனில் (!other.page_at (b).is_empty ()) { திரும்பு பொய்; }

    திரும்பு உண்மை;
  }

  தருக்க is_subset (மாறிலி hb_bit_set_t &larger_set) மாறிலி
  {
    எனில் (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      திரும்பு பொய்;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    போது (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      எனில் (spm < lpm) {
        எனில் (!sp.is_empty ())
          திரும்பு பொய்;
        spi++;
        தொடர்;
      }

      எனில் (lpm < spm) {
        lpi++;
        தொடர்;
      }

      auto lp = larger_set.page_at (lpi);
      எனில் (!sp.is_subset (lp))
        திரும்பு பொய்;

      spi++;
      lpi++;
    }

    போது (spi < page_map.length)
      எனில் (!page_at (spi++).is_empty ())
        திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  தனிப்பட்ட:
  தருக்க allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    எனில் (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = பொய்;
      திரும்பு பொய்;
    }

    திரும்பு உண்மை;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  வெறுமை compact (hb_vector_t<unsigned>& workspace,
                unsigned முழு length)
  {
    ப்ரதிஜ்ஞா(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    க்கு (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  வெறுமை compact_pages (மாறிலி hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned முழு write_index = 0;
    க்கு (unsigned முழு i = 0; i < pages.length; i++)
    {
      எனில் (old_index_to_page_map_index[i] == 0xFFFFFFFF) தொடர்;

      எனில் (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  பொது:

  வெறுமை process_ (hb_bit_page_t::vector_t (*op) (மாறிலி hb_bit_page_t::vector_t &, மாறிலி hb_bit_page_t::vector_t &),
		 தருக்க passthru_இடது, தருக்க passthru_வலது,
		 மாறிலி hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    எனில் (unlikely (!successful)) திரும்பு;

    dirty ();

    unsigned முழு na = pages.length;
    unsigned முழு nb = other.pages.length;
    unsigned முழு next_page = na;

    unsigned முழு மொத்தம் = 0, new_மொத்தம் = 0;
    unsigned முழு a = 0, b = 0;
    unsigned முழு write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    எனில் (!passthru_இடது)
    {
      தருக்க allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      எனில் (unlikely (!allocated))
      {
	successful = பொய்;
	திரும்பு;
      }
    }

    க்கு (; a < na && b < nb; )
    {
      எனில் (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	எனில் (!passthru_இடது)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  எனில் (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	மொத்தம்++;
	a++;
	b++;
      }
      இல்லையெனில் எனில் (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	எனில் (passthru_இடது)
	  மொத்தம்++;
	a++;
      }
      இல்லையெனில்
      {
	எனில் (passthru_வலது)
	  மொத்தம்++;
	b++;
      }
    }
    எனில் (passthru_இடது)
      மொத்தம் += na - a;
    எனில் (passthru_வலது)
      மொத்தம் += nb - b;

    எனில் (!passthru_இடது)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    எனில் (unlikely (!resize (மொத்தம்)))
      திரும்பு;

    new_மொத்தம் = மொத்தம்;

    /* Process in-place backward. */
    a = na;
    b = nb;
    க்கு (; a && b; )
    {
      எனில் (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	மொத்தம்--;
	page_map.arrayZ[மொத்தம்] = page_map.arrayZ[a];
	page_at (மொத்தம்).v = op (page_at (a).v, other.page_at (b).v);
	page_at (மொத்தம்).dirty ();
      }
      இல்லையெனில் எனில் (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	எனில் (passthru_இடது)
	{
	  மொத்தம்--;
	  page_map.arrayZ[மொத்தம்] = page_map.arrayZ[a];
	}
      }
      இல்லையெனில்
      {
	b--;
	எனில் (passthru_வலது)
	{
	  மொத்தம்--;
	  page_map.arrayZ[மொத்தம்].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[மொத்தம்].index = next_page++;
	  page_at (மொத்தம்) = other.page_at (b);
	}
      }
    }
    எனில் (passthru_இடது)
      போது (a)
      {
	a--;
	மொத்தம்--;
	page_map.arrayZ[மொத்தம்] = page_map.arrayZ[a];
      }
    எனில் (passthru_வலது)
      போது (b)
      {
	b--;
	மொத்தம்--;
	page_map.arrayZ[மொத்தம்].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[மொத்தம்].index = next_page++;
	page_at (மொத்தம்) = other.page_at (b);
      }
    ப்ரதிஜ்ஞா (!மொத்தம்);
    resize (new_மொத்தம்);
  }
  template <typename Op>
  நிலையான hb_bit_page_t::vector_t
  op_ (மாறிலி hb_bit_page_t::vector_t &a, மாறிலி hb_bit_page_t::vector_t &b)
  { திரும்பு Op{} (a, b); }
  template <typename Op>
  வெறுமை process (மாறிலி Op& op,
		மாறிலி hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  வெறுமை union_ (மாறிலி hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  வெறுமை intersect (மாறிலி hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  வெறுமை subtract (மாறிலி hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  வெறுமை symmetric_difference (மாறிலி hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  தருக்க next (hb_codepoint_t *codepoint) மாறிலி
  {
    எனில் (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      திரும்பு *codepoint != INVALID;
    }

    மாறிலி auto* page_map_array = page_map.arrayZ;
    unsigned முழு major = get_major (*codepoint);
    unsigned முழு i = last_page_lookup;

    எனில் (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      எனில் (i >= page_map.length) {
        *codepoint = INVALID;
        திரும்பு பொய்;
      }
      last_page_lookup = i;
    }

    மாறிலி auto* pages_array = pages.arrayZ;
    மாறிலி page_map_t &current = page_map_array[i];
    எனில் (likely (current.major == major))
    {
      எனில் (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        திரும்பு உண்மை;
      }
      i++;
    }

    க்கு (; i < page_map.length; i++)
    {
      மாறிலி page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      எனில் (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	திரும்பு உண்மை;
      }
    }
    *codepoint = INVALID;
    திரும்பு பொய்;
  }
  தருக்க next_bits (hb_codepoint_t *codepoint, uint64_t *bits) மாறிலி
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned முழு i = 0;
    unsigned முழு elt = 0;
    எனில் (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      எனில் (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        செல் done;
      base += page_t::ELT_BITS;

      unsigned முழு major = get_major (base);
      i = last_page_lookup;
      எனில் (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	எனில் (i >= page_map.length)
	  செல் done;
      }
      எனில் (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    க்கு (; i < page_map.length; i++, elt = 0)
    {
      மாறிலி page_map_t &map = page_map.arrayZ[i];
      மாறிலி page_t &page = pages.arrayZ[map.index];
      க்கு (; elt < page_t::len (); elt++)
	எனில் (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  திரும்பு உண்மை;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    திரும்பு பொய்;
  }
  தருக்க previous (hb_codepoint_t *codepoint) மாறிலி
  {
    எனில் (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      திரும்பு *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned முழு i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    எனில் (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      எனில் (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	திரும்பு உண்மை;
      }
    }
    i--;
    க்கு (; (முழு) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      எனில் (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	திரும்பு உண்மை;
      }
    }
    *codepoint = INVALID;
    திரும்பு பொய்;
  }
  தருக்க next_range (hb_codepoint_t *first, hb_codepoint_t *last) மாறிலி
  {
    hb_codepoint_t i;

    i = *last;
    எனில் (!next (&i))
    {
      *last = *first = INVALID;
      திரும்பு பொய்;
    }

    *last = *first = i;

    மாறிலி auto* page_map_array = page_map.arrayZ;
    மாறிலி auto* pages_array = pages.arrayZ;
    unsigned முழு major = get_major (*last);
    unsigned முழு p_index = last_page_lookup;

    எனில் (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      ப்ரதிஜ்ஞா(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned முழு rem = page_remainder (*last);
    unsigned முழு elt = rem / page_t::ELT_BITS;
    unsigned முழு bit = rem & page_t::ELT_MASK;

    மாறிலி page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned முழு run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    எனில் (run < page_t::ELT_BITS - bit)
      திரும்பு உண்மை;

    elt++;

    போது (உண்மை)
    {
      க்கு (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        எனில் (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          தொடர்;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        திரும்பு உண்மை;
      }

      p_index++;


      எனில் (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        திரும்பு உண்மை;

      மாறிலி auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  தருக்க previous_range (hb_codepoint_t *first, hb_codepoint_t *last) மாறிலி
  {
    hb_codepoint_t i;

    i = *first;
    எனில் (!previous (&i))
    {
      *last = *first = INVALID;
      திரும்பு பொய்;
    }

    *last = *first = i;

    மாறிலி auto* page_map_array = page_map.arrayZ;
    மாறிலி auto* pages_array = pages.arrayZ;
    unsigned முழு major = get_major (*first);
    unsigned முழு p_index = last_page_lookup;

    எனில் (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      ப்ரதிஜ்ஞா(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned முழு rem = page_remainder (*first);
    முழு elt = rem / page_t::ELT_BITS;
    unsigned முழு bit = rem & page_t::ELT_MASK;

    மாறிலி page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned முழு run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    எனில் (run < bit + 1)
      திரும்பு உண்மை;

    elt--;

    போது (உண்மை)
    {
      க்கு (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        எனில் (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          தொடர்;
        }
        uint64_t inv = ~w;
        unsigned முழு zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        திரும்பு உண்மை;
      }


      எனில் (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        திரும்பு உண்மை;

      p_index--;
      மாறிலி auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned முழு next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned முழு    size) மாறிலி
  {
    // By default, start at the first bit of the first page of values.
    unsigned முழு start_page = 0;
    unsigned முழு start_page_மதிப்பு = 0;
    எனில் (unlikely (codepoint != INVALID))
    {
      மாறிலி auto* page_map_array = page_map.arrayZ;
      unsigned முழு major = get_major (codepoint);
      unsigned முழு i = last_page_lookup;
      எனில் (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	எனில் (i >= page_map.length)
	  திரும்பு 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      எனில் (page_map_array[i].major == major)
      {
        start_page_மதிப்பு = page_remainder (codepoint + 1);
        எனில் (unlikely (start_page_மதிப்பு == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_மதிப்பு = 0;
        }
      }
    }

    unsigned முழு initial_size = size;
    க்கு (unsigned முழு i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned முழு n = pages[page_map.arrayZ[i].index].லிக (base, start_page_மதிப்பு, out, size);
      out += n;
      size -= n;
      start_page_மதிப்பு = 0;
    }
    திரும்பு initial_size - size;
  }

  unsigned முழு next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned முழு    size) மாறிலி
  {
    unsigned முழு initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned முழு start_page = 0;
    unsigned முழு start_page_மதிப்பு = 0;
    எனில் (unlikely (codepoint != INVALID))
    {
      மாறிலி auto* page_map_array = page_map.arrayZ;
      unsigned முழு major = get_major (codepoint);
      unsigned முழு i = last_page_lookup;
      எனில் (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        எனில் (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          போது (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          திரும்பு initial_size - size;
        }
      }
      start_page = i;
      எனில் (page_map_array[i].major == major)
      {
        start_page_மதிப்பு = page_remainder (codepoint + 1);
        எனில் (unlikely (start_page_மதிப்பு == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_மதிப்பு = 0;
        }
      }
    }

    hb_codepoint_t next_மதிப்பு = codepoint + 1;
    க்கு (unsigned முழு i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned முழு n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_மதிப்பு, out, size, &next_மதிப்பு);
      out += n;
      size -= n;
      start_page_மதிப்பு = 0;
    }
    போது (next_மதிப்பு < HB_SET_மதிப்பு_INVALID && size) {
      *out++ = next_மதிப்பு++;
      size--;
    }
    திரும்பு initial_size - size;
  }

  தருக்க has_population () மாறிலி { திரும்பு population != UINT_MAX; }
  unsigned முழு get_population () மாறிலி
  {
    எனில் (has_population ())
      திரும்பு population;

    unsigned முழு எடு = 0;
    unsigned முழு மொத்தம் = pages.length;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      எடு += pages[i].get_population ();

    population = எடு;
    திரும்பு எடு;
  }
  தருக்க get_singleton (hb_codepoint_t *codepoint) மாறிலி
  {
    எனில் (has_population ())
    {
      எனில் (population != 1) திரும்பு பொய்;
      *codepoint = get_min ();
      திரும்பு உண்மை;
    }

    hb_codepoint_t singleton = INVALID;
    க்கு (மாறிலி auto &map : page_map)
    {
      மாறிலி auto &page = pages.arrayZ[map.index];
      க்கு (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	எனில் (!bits) தொடர்;
	எனில் (singleton != INVALID || (bits & (bits - 1)))
	  திரும்பு பொய்;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    எனில் (singleton == INVALID)
    {
      population = 0;
      திரும்பு பொய்;
    }
    population = 1;
    *codepoint = singleton;
    திரும்பு உண்மை;
  }
  hb_codepoint_t get_min () மாறிலி
  {
    unsigned மொத்தம் = pages.length;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      மாறிலி auto& map = page_map.arrayZ[i];
      மாறிலி auto& page = pages.arrayZ[map.index];

      எனில் (!page.is_empty ())
	திரும்பு map.major * page_t::PAGE_BITS + page.get_min ();
    }
    திரும்பு INVALID;
  }
  hb_codepoint_t get_max () மாறிலி
  {
    unsigned மொத்தம் = pages.length;
    க்கு (signed i = மொத்தம் - 1; i >= 0; i--)
    {
      மாறிலி auto& map = page_map.arrayZ[(unsigned) i];
      மாறிலி auto& page = pages.arrayZ[map.index];

      எனில் (!page.is_empty ())
	திரும்பு map.major * page_t::PAGE_BITS + page.get_max ();
    }
    திரும்பு INVALID;
  }

  நிலையான constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  கட்டமைப்பு iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    நிலையான constexpr தருக்க is_sorted_iterator = உண்மை;
    நிலையான constexpr தருக்க has_fast_len = உண்மை;
    iter_t (மாறிலி hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    தருக்க init = உண்மை) : s (&s_), v (INVALID), l(0)
    {
      எனில் (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    வகைவரை hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () மாறிலி { திரும்பு v; }
    தருக்க __more__ () மாறிலி { திரும்பு v != INVALID; }
    வெறுமை __next__ () { s->next (&v); எனில் (l) l--; }
    வெறுமை __prev__ () { s->previous (&v); }
    unsigned __len__ () மாறிலி { திரும்பு l; }
    iter_t end () மாறிலி { திரும்பு iter_t (*s, பொய்); }
    தருக்க operator != (மாறிலி iter_t& o) மாறிலி
    { திரும்பு v != o.v; }

    பாதுகாக்கப்பட்ட:
    மாறிலி hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () மாறிலி { திரும்பு iter_t (*இது); }
  operator iter_t () மாறிலி { திரும்பு iter (); }

  பாதுகாக்கப்பட்ட:

  page_t *page_for (hb_codepoint_t g, தருக்க insert = பொய்)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    எனில் (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      எனில் (cached_page.major == major)
	திரும்பு &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    எனில் (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      எனில் (!insert)
        திரும்பு nullptr;

      எனில் (unlikely (!resize (pages.length + 1)))
	திரும்பு nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    திரும்பு &pages.arrayZ[page_map.arrayZ[i].index];
  }
  மாறிலி page_t *page_for (hb_codepoint_t g) மாறிலி
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    எனில் (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      எனில் (cached_page.major == major)
	திரும்பு &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    எனில் (!page_map.bfind (key, &i))
      திரும்பு nullptr;

    last_page_lookup = i;
    திரும்பு &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned முழு i)
  {
    ப்ரதிஜ்ஞா (i < page_map.length);
    திரும்பு pages.arrayZ[page_map.arrayZ[i].index];
  }
  மாறிலி page_t &page_at (unsigned முழு i) மாறிலி
  {
    ப்ரதிஜ்ஞா (i < page_map.length);
    திரும்பு pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned முழு get_major (hb_codepoint_t g) மாறிலி { திரும்பு g >> page_t::PAGE_BITS_LOG_2; }
  unsigned முழு page_remainder (hb_codepoint_t g) மாறிலி { திரும்பு g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned முழு major) மாறிலி { திரும்பு major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
