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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-bit-page.hh"

ഘടന hb_bit_set_t
{
  hb_bit_set_t () = സ്ഥിരസ്ഥിതി;
  ~hb_bit_set_t () = സ്ഥിരസ്ഥിതി;

  hb_bit_set_t (സ്ഥിരം hb_bit_set_t& other) : hb_bit_set_t () { set (other, സത്യം); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*ഇത്, other); }
  hb_bit_set_t& operator= (സ്ഥിരം hb_bit_set_t& other) { set (other); തിരികെ *ഇത്; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*ഇത്, other); തിരികെ *ഇത്; }
  friend ശൂന്യം swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    എങ്കിൽ (likely (!a.successful || !b.successful))
      തിരികെ;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  ശൂന്യം init ()
  {
    successful = സത്യം;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  ശൂന്യം fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  ഘടന page_map_t
  {
    പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം page_map_t &o) സ്ഥിരം { തിരികെ cmp (o.major); }
    പൂർണ്ണസംഖ്യ cmp (uint32_t o_major) സ്ഥിരം { തിരികെ (പൂർണ്ണസംഖ്യ) o_major - (പൂർണ്ണസംഖ്യ) major; }

    uint32_t major;
    uint32_t index;
  };

  തർക്കം successful = സത്യം; /* Allocations successful */
  mutable unsigned പൂർണ്ണസംഖ്യ population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  ശൂന്യം err () { എങ്കിൽ (successful) successful = അസത്യം; } /* TODO Remove */
  തർക്കം in_error () സ്ഥിരം { തിരികെ !successful; }

  തർക്കം resize (unsigned പൂർണ്ണസംഖ്യ എണ്ണം, തർക്കം clear = സത്യം, തർക്കം exact_size = അസത്യം)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ അസത്യം;

    എങ്കിൽ (pages.length < എണ്ണം && (unsigned) pages.allocated < എണ്ണം && എണ്ണം <= 2)
      exact_size = സത്യം; // Most sets are small and local

    എങ്കിൽ (unlikely (!pages.resize_full (എണ്ണം, clear, exact_size) ||
	!page_map.resize_full (എണ്ണം, clear, അസത്യം)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = അസത്യം;
      തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
  }

  ശൂന്യം alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = സത്യം;
    clear ();
    തിരികെ *ഇത്;
  }

  ശൂന്യം clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    എങ്കിൽ (!pages.length && !population) തിരികെ;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  തർക്കം is_empty () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = pages.length;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എങ്കിൽ (!pages[i].is_empty ())
	തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }
  explicit operator തർക്കം () സ്ഥിരം { തിരികെ !is_empty (); }

  uint32_t hash () സ്ഥിരം
  {
    uint32_t h = 0;
    വേണ്ടി (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      എങ്കിൽ (unlikely (page.is_empty ())) തുടരുക;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    തിരികെ h;
  }

  സ്വകാര്യ:
  ശൂന്യം dirty () { population = UINT_MAX; }
  പൊതു:

  ശൂന്യം add (hb_codepoint_t g)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ;
    എങ്കിൽ (unlikely (g == INVALID)) തിരികെ;
    dirty ();
    page_t *page = page_for (g, സത്യം); എങ്കിൽ (unlikely (!page)) തിരികെ;
    page->add (g);
  }
  ശൂന്യം add_bits (hb_codepoint_t g, uint64_t bits)
  {
    എങ്കിൽ (unlikely (!successful) || unlikely (!bits)) തിരികെ;
    പ്രതിജ്ഞാ (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, സത്യം); എങ്കിൽ (unlikely (!page)) തിരികെ;
    page->add_bits (g, bits);
  }
  തർക്കം add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ സത്യം; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    എങ്കിൽ (unlikely (a > b || a == INVALID || b == INVALID)) തിരികെ അസത്യം;
    dirty ();
    unsigned പൂർണ്ണസംഖ്യ ma = get_major (a);
    unsigned പൂർണ്ണസംഖ്യ mb = get_major (b);
    എങ്കിൽ (ma == mb)
    {
      page_t *page = page_for (a, സത്യം); എങ്കിൽ (unlikely (!page)) തിരികെ അസത്യം;
      page->add_range (a, b);
    }
    അല്ലെങ്കിൽ
    {
      page_t *page = page_for (a, സത്യം); എങ്കിൽ (unlikely (!page)) തിരികെ അസത്യം;
      page->add_range (a, major_start (ma + 1) - 1);

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), സത്യം); എങ്കിൽ (unlikely (!page)) തിരികെ അസത്യം;
	page->init1 ();
      }

      page = page_for (b, സത്യം); എങ്കിൽ (unlikely (!page)) തിരികെ അസത്യം;
      page->add_range (major_start (mb), b);
    }
    തിരികെ സത്യം;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  സ്ഥിതം വരിയിൽ സ്ഥിരം Type& StructAtOffsetUnaligned(സ്ഥിരം ശൂന്യം *P, unsigned പൂർണ്ണസംഖ്യ offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    തിരികെ * reinterpret_cast<സ്ഥിരം Type*> ((സ്ഥിരം അക്ഷരം *) P + offset);
#pragma GCC diagnostic എടുക്കുക
  }

  template <typename T>
  ശൂന്യം set_array (തർക്കം v, സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ;
    എങ്കിൽ (!എണ്ണം) തിരികെ;
    dirty ();
    hb_codepoint_t g = *array;
    ആകുമ്പോൾ (എണ്ണം)
    {
      unsigned പൂർണ്ണസംഖ്യ m = get_major (g);
      page_t *page = page_for (g, v); എങ്കിൽ (unlikely (v && !page)) തിരികെ;
      unsigned പൂർണ്ണസംഖ്യ start = major_start (m);
      unsigned പൂർണ്ണസംഖ്യ end = major_start (m + 1);
      ചെയ്യുക
      {
        എങ്കിൽ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	എണ്ണം--;
      }
      ആകുമ്പോൾ (എണ്ണം && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  ശൂന്യം add_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  { set_array (സത്യം, array, എണ്ണം, stride); }
  template <typename T>
  ശൂന്യം add_array (സ്ഥിരം hb_array_t<സ്ഥിരം T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  ശൂന്യം del_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  { set_array (അസത്യം, array, എണ്ണം, stride); }
  template <typename T>
  ശൂന്യം del_array (സ്ഥിരം hb_array_t<സ്ഥിരം T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  തർക്കം set_sorted_array (തർക്കം v, സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ സത്യം; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    എങ്കിൽ (unlikely (!എണ്ണം)) തിരികെ സത്യം;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ആകുമ്പോൾ (എണ്ണം)
    {
      unsigned പൂർണ്ണസംഖ്യ m = get_major (g);
      page_t *page = page_for (g, v); എങ്കിൽ (unlikely (v && !page)) തിരികെ അസത്യം;
      unsigned പൂർണ്ണസംഖ്യ end = major_start (m + 1);
      ചെയ്യുക
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	എങ്കിൽ (g < last_g) തിരികെ അസത്യം;
	last_g = g;

        എങ്കിൽ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	എണ്ണം--;
      }
      ആകുമ്പോൾ (എണ്ണം && (g = *array, g < end));
    }
    തിരികെ സത്യം;
  }

  template <typename T>
  തർക്കം add_sorted_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  { തിരികെ set_sorted_array (സത്യം, array, എണ്ണം, stride); }
  template <typename T>
  തർക്കം add_sorted_array (സ്ഥിരം hb_sorted_array_t<സ്ഥിരം T>& arr) { തിരികെ add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  തർക്കം del_sorted_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  { തിരികെ set_sorted_array (അസത്യം, array, എണ്ണം, stride); }
  template <typename T>
  തർക്കം del_sorted_array (സ്ഥിരം hb_sorted_array_t<സ്ഥിരം T>& arr) { തിരികെ del_sorted_array (&arr, arr.len ()); }

  ശൂന്യം del (hb_codepoint_t g)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ;
    page_t *page = page_for (g);
    എങ്കിൽ (!page)
      തിരികെ;
    dirty ();
    page->del (g);
  }
  ശൂന്യം del_bits (hb_codepoint_t g, uint64_t bits)
  {
    എങ്കിൽ (unlikely (!successful) || unlikely (!bits)) തിരികെ;
    പ്രതിജ്ഞാ (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    എങ്കിൽ (!page)
      തിരികെ;
    dirty ();
    page->del_bits (g, bits);
  }

  സ്വകാര്യ:
  ശൂന്യം del_pages (പൂർണ്ണസംഖ്യ ds, പൂർണ്ണസംഖ്യ de)
  {
    എങ്കിൽ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      എങ്കിൽ (unlikely (!allocate_compact_workspace (compact_workspace))) തിരികെ;

      unsigned പൂർണ്ണസംഖ്യ write_index = 0;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < page_map.length; i++)
      {
	പൂർണ്ണസംഖ്യ m = (പൂർണ്ണസംഖ്യ) page_map.arrayZ[i].major;
	എങ്കിൽ (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  പൊതു:
  ശൂന്യം del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ;
    എങ്കിൽ (unlikely (a > b || a == INVALID)) തിരികെ;
    dirty ();
    unsigned പൂർണ്ണസംഖ്യ ma = get_major (a);
    unsigned പൂർണ്ണസംഖ്യ mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    പൂർണ്ണസംഖ്യ ds = (a == major_start (ma))? (പൂർണ്ണസംഖ്യ) ma: (പൂർണ്ണസംഖ്യ) (ma + 1);
    പൂർണ്ണസംഖ്യ de = (b + 1 == major_start (mb + 1))? (പൂർണ്ണസംഖ്യ) mb: ((പൂർണ്ണസംഖ്യ) mb - 1);
    എങ്കിൽ (ds > de || (പൂർണ്ണസംഖ്യ) ma < ds)
    {
      page_t *page = page_for (a);
      എങ്കിൽ (page)
      {
	എങ്കിൽ (ma == mb)
	  page->del_range (a, b);
	അല്ലെങ്കിൽ
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    എങ്കിൽ (de < (പൂർണ്ണസംഖ്യ) mb && ma != mb)
    {
      page_t *page = page_for (b);
      എങ്കിൽ (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  തർക്കം get (hb_codepoint_t g) സ്ഥിരം
  {
    സ്ഥിരം page_t *page = page_for (g);
    എങ്കിൽ (!page)
      തിരികെ അസത്യം;
    തിരികെ page->get (g);
  }
  തർക്കം may_have (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }

  /* Has interface. */
  തർക്കം operator [] (hb_codepoint_t k) സ്ഥിരം { തിരികെ get (k); }
  തർക്കം has (hb_codepoint_t k) സ്ഥിരം { തിരികെ (*ഇത്)[k]; }
  /* Predicate. */
  തർക്കം operator () (hb_codepoint_t k) സ്ഥിരം { തിരികെ has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); തിരികെ *ഇത്; }
  hb_bit_set_t& operator << (സ്ഥിരം hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); തിരികെ *ഇത്; }

  തർക്കം intersects (സ്ഥിരം hb_bit_set_t &other) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ na = pages.length;
    unsigned പൂർണ്ണസംഖ്യ nb = other.pages.length;

    unsigned പൂർണ്ണസംഖ്യ a = 0, b = 0;
    വേണ്ടി (; a < na && b < nb; )
    {
      എങ്കിൽ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	എങ്കിൽ (page_at (a).intersects (other.page_at (b)))
	  തിരികെ സത്യം;
	a++;
	b++;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      അല്ലെങ്കിൽ
	b++;
    }
    തിരികെ അസത്യം;
  }
  തർക്കം may_intersect (സ്ഥിരം hb_bit_set_t &other) സ്ഥിരം
  { തിരികെ intersects (other); }

  തർക്കം intersects (hb_codepoint_t first, hb_codepoint_t last) സ്ഥിരം
  {
    hb_codepoint_t c = first - 1;
    തിരികെ next (&c) && c <= last;
  }
  ശൂന്യം set (സ്ഥിരം hb_bit_set_t &other, തർക്കം exact_size = അസത്യം)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = other.pages.length;
    എങ്കിൽ (unlikely (!resize  (എണ്ണം, അസത്യം, exact_size)))
      തിരികെ;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  തർക്കം is_equal (സ്ഥിരം hb_bit_set_t &other) സ്ഥിരം
  {
    എങ്കിൽ (has_population () && other.has_population () &&
	population != other.population)
      തിരികെ അസത്യം;

    unsigned പൂർണ്ണസംഖ്യ na = pages.length;
    unsigned പൂർണ്ണസംഖ്യ nb = other.pages.length;

    unsigned പൂർണ്ണസംഖ്യ a = 0, b = 0;
    വേണ്ടി (; a < na && b < nb; )
    {
      എങ്കിൽ (page_at (a).is_empty ()) { a++; തുടരുക; }
      എങ്കിൽ (other.page_at (b).is_empty ()) { b++; തുടരുക; }
      എങ്കിൽ (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	തിരികെ അസത്യം;
      a++;
      b++;
    }
    വേണ്ടി (; a < na; a++)
      എങ്കിൽ (!page_at (a).is_empty ()) { തിരികെ അസത്യം; }
    വേണ്ടി (; b < nb; b++)
      എങ്കിൽ (!other.page_at (b).is_empty ()) { തിരികെ അസത്യം; }

    തിരികെ സത്യം;
  }

  തർക്കം is_subset (സ്ഥിരം hb_bit_set_t &larger_set) സ്ഥിരം
  {
    എങ്കിൽ (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      തിരികെ അസത്യം;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ആകുമ്പോൾ (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      എങ്കിൽ (spm < lpm) {
        എങ്കിൽ (!sp.is_empty ())
          തിരികെ അസത്യം;
        spi++;
        തുടരുക;
      }

      എങ്കിൽ (lpm < spm) {
        lpi++;
        തുടരുക;
      }

      auto lp = larger_set.page_at (lpi);
      എങ്കിൽ (!sp.is_subset (lp))
        തിരികെ അസത്യം;

      spi++;
      lpi++;
    }

    ആകുമ്പോൾ (spi < page_map.length)
      എങ്കിൽ (!page_at (spi++).is_empty ())
        തിരികെ അസത്യം;

    തിരികെ സത്യം;
  }

  സ്വകാര്യ:
  തർക്കം allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    എങ്കിൽ (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = അസത്യം;
      തിരികെ അസത്യം;
    }

    തിരികെ സത്യം;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  ശൂന്യം compact (hb_vector_t<unsigned>& workspace,
                unsigned പൂർണ്ണസംഖ്യ length)
  {
    പ്രതിജ്ഞാ(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    വേണ്ടി (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  ശൂന്യം compact_pages (സ്ഥിരം hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned പൂർണ്ണസംഖ്യ write_index = 0;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < pages.length; i++)
    {
      എങ്കിൽ (old_index_to_page_map_index[i] == 0xFFFFFFFF) തുടരുക;

      എങ്കിൽ (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  പൊതു:

  ശൂന്യം process_ (hb_bit_page_t::vector_t (*op) (സ്ഥിരം hb_bit_page_t::vector_t &, സ്ഥിരം hb_bit_page_t::vector_t &),
		 തർക്കം passthru_ഇടത്, തർക്കം passthru_വലത്,
		 സ്ഥിരം hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    എങ്കിൽ (unlikely (!successful)) തിരികെ;

    dirty ();

    unsigned പൂർണ്ണസംഖ്യ na = pages.length;
    unsigned പൂർണ്ണസംഖ്യ nb = other.pages.length;
    unsigned പൂർണ്ണസംഖ്യ next_page = na;

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = 0, new_എണ്ണം = 0;
    unsigned പൂർണ്ണസംഖ്യ a = 0, b = 0;
    unsigned പൂർണ്ണസംഖ്യ write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    എങ്കിൽ (!passthru_ഇടത്)
    {
      തർക്കം allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      എങ്കിൽ (unlikely (!allocated))
      {
	successful = അസത്യം;
	തിരികെ;
      }
    }

    വേണ്ടി (; a < na && b < nb; )
    {
      എങ്കിൽ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	എങ്കിൽ (!passthru_ഇടത്)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  എങ്കിൽ (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	എണ്ണം++;
	a++;
	b++;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	എങ്കിൽ (passthru_ഇടത്)
	  എണ്ണം++;
	a++;
      }
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (passthru_വലത്)
	  എണ്ണം++;
	b++;
      }
    }
    എങ്കിൽ (passthru_ഇടത്)
      എണ്ണം += na - a;
    എങ്കിൽ (passthru_വലത്)
      എണ്ണം += nb - b;

    എങ്കിൽ (!passthru_ഇടത്)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    എങ്കിൽ (unlikely (!resize (എണ്ണം)))
      തിരികെ;

    new_എണ്ണം = എണ്ണം;

    /* Process in-place backward. */
    a = na;
    b = nb;
    വേണ്ടി (; a && b; )
    {
      എങ്കിൽ (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	എണ്ണം--;
	page_map.arrayZ[എണ്ണം] = page_map.arrayZ[a];
	page_at (എണ്ണം).v = op (page_at (a).v, other.page_at (b).v);
	page_at (എണ്ണം).dirty ();
      }
      അല്ലെങ്കിൽ എങ്കിൽ (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	എങ്കിൽ (passthru_ഇടത്)
	{
	  എണ്ണം--;
	  page_map.arrayZ[എണ്ണം] = page_map.arrayZ[a];
	}
      }
      അല്ലെങ്കിൽ
      {
	b--;
	എങ്കിൽ (passthru_വലത്)
	{
	  എണ്ണം--;
	  page_map.arrayZ[എണ്ണം].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[എണ്ണം].index = next_page++;
	  page_at (എണ്ണം) = other.page_at (b);
	}
      }
    }
    എങ്കിൽ (passthru_ഇടത്)
      ആകുമ്പോൾ (a)
      {
	a--;
	എണ്ണം--;
	page_map.arrayZ[എണ്ണം] = page_map.arrayZ[a];
      }
    എങ്കിൽ (passthru_വലത്)
      ആകുമ്പോൾ (b)
      {
	b--;
	എണ്ണം--;
	page_map.arrayZ[എണ്ണം].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[എണ്ണം].index = next_page++;
	page_at (എണ്ണം) = other.page_at (b);
      }
    പ്രതിജ്ഞാ (!എണ്ണം);
    resize (new_എണ്ണം);
  }
  template <typename Op>
  സ്ഥിതം hb_bit_page_t::vector_t
  op_ (സ്ഥിരം hb_bit_page_t::vector_t &a, സ്ഥിരം hb_bit_page_t::vector_t &b)
  { തിരികെ Op{} (a, b); }
  template <typename Op>
  ശൂന്യം process (സ്ഥിരം Op& op,
		സ്ഥിരം hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  ശൂന്യം union_ (സ്ഥിരം hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  ശൂന്യം intersect (സ്ഥിരം hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  ശൂന്യം subtract (സ്ഥിരം hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  ശൂന്യം symmetric_difference (സ്ഥിരം hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  തർക്കം next (hb_codepoint_t *codepoint) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      തിരികെ *codepoint != INVALID;
    }

    സ്ഥിരം auto* page_map_array = page_map.arrayZ;
    unsigned പൂർണ്ണസംഖ്യ major = get_major (*codepoint);
    unsigned പൂർണ്ണസംഖ്യ i = last_page_lookup;

    എങ്കിൽ (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      എങ്കിൽ (i >= page_map.length) {
        *codepoint = INVALID;
        തിരികെ അസത്യം;
      }
      last_page_lookup = i;
    }

    സ്ഥിരം auto* pages_array = pages.arrayZ;
    സ്ഥിരം page_map_t &current = page_map_array[i];
    എങ്കിൽ (likely (current.major == major))
    {
      എങ്കിൽ (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        തിരികെ സത്യം;
      }
      i++;
    }

    വേണ്ടി (; i < page_map.length; i++)
    {
      സ്ഥിരം page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      എങ്കിൽ (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	തിരികെ സത്യം;
      }
    }
    *codepoint = INVALID;
    തിരികെ അസത്യം;
  }
  തർക്കം next_bits (hb_codepoint_t *codepoint, uint64_t *bits) സ്ഥിരം
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned പൂർണ്ണസംഖ്യ i = 0;
    unsigned പൂർണ്ണസംഖ്യ elt = 0;
    എങ്കിൽ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      എങ്കിൽ (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        പോകുക done;
      base += page_t::ELT_BITS;

      unsigned പൂർണ്ണസംഖ്യ major = get_major (base);
      i = last_page_lookup;
      എങ്കിൽ (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	എങ്കിൽ (i >= page_map.length)
	  പോകുക done;
      }
      എങ്കിൽ (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    വേണ്ടി (; i < page_map.length; i++, elt = 0)
    {
      സ്ഥിരം page_map_t &map = page_map.arrayZ[i];
      സ്ഥിരം page_t &page = pages.arrayZ[map.index];
      വേണ്ടി (; elt < page_t::len (); elt++)
	എങ്കിൽ (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  തിരികെ സത്യം;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    തിരികെ അസത്യം;
  }
  തർക്കം previous (hb_codepoint_t *codepoint) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      തിരികെ *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned പൂർണ്ണസംഖ്യ i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    എങ്കിൽ (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      എങ്കിൽ (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	തിരികെ സത്യം;
      }
    }
    i--;
    വേണ്ടി (; (പൂർണ്ണസംഖ്യ) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      എങ്കിൽ (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	തിരികെ സത്യം;
      }
    }
    *codepoint = INVALID;
    തിരികെ അസത്യം;
  }
  തർക്കം next_range (hb_codepoint_t *first, hb_codepoint_t *last) സ്ഥിരം
  {
    hb_codepoint_t i;

    i = *last;
    എങ്കിൽ (!next (&i))
    {
      *last = *first = INVALID;
      തിരികെ അസത്യം;
    }

    *last = *first = i;

    സ്ഥിരം auto* page_map_array = page_map.arrayZ;
    സ്ഥിരം auto* pages_array = pages.arrayZ;
    unsigned പൂർണ്ണസംഖ്യ major = get_major (*last);
    unsigned പൂർണ്ണസംഖ്യ p_index = last_page_lookup;

    എങ്കിൽ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      പ്രതിജ്ഞാ(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned പൂർണ്ണസംഖ്യ rem = page_remainder (*last);
    unsigned പൂർണ്ണസംഖ്യ elt = rem / page_t::ELT_BITS;
    unsigned പൂർണ്ണസംഖ്യ bit = rem & page_t::ELT_MASK;

    സ്ഥിരം page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned പൂർണ്ണസംഖ്യ run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    എങ്കിൽ (run < page_t::ELT_BITS - bit)
      തിരികെ സത്യം;

    elt++;

    ആകുമ്പോൾ (സത്യം)
    {
      വേണ്ടി (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        എങ്കിൽ (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          തുടരുക;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        തിരികെ സത്യം;
      }

      p_index++;


      എങ്കിൽ (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        തിരികെ സത്യം;

      സ്ഥിരം auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  തർക്കം previous_range (hb_codepoint_t *first, hb_codepoint_t *last) സ്ഥിരം
  {
    hb_codepoint_t i;

    i = *first;
    എങ്കിൽ (!previous (&i))
    {
      *last = *first = INVALID;
      തിരികെ അസത്യം;
    }

    *last = *first = i;

    സ്ഥിരം auto* page_map_array = page_map.arrayZ;
    സ്ഥിരം auto* pages_array = pages.arrayZ;
    unsigned പൂർണ്ണസംഖ്യ major = get_major (*first);
    unsigned പൂർണ്ണസംഖ്യ p_index = last_page_lookup;

    എങ്കിൽ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      പ്രതിജ്ഞാ(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned പൂർണ്ണസംഖ്യ rem = page_remainder (*first);
    പൂർണ്ണസംഖ്യ elt = rem / page_t::ELT_BITS;
    unsigned പൂർണ്ണസംഖ്യ bit = rem & page_t::ELT_MASK;

    സ്ഥിരം page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned പൂർണ്ണസംഖ്യ run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    എങ്കിൽ (run < bit + 1)
      തിരികെ സത്യം;

    elt--;

    ആകുമ്പോൾ (സത്യം)
    {
      വേണ്ടി (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        എങ്കിൽ (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          തുടരുക;
        }
        uint64_t inv = ~w;
        unsigned പൂർണ്ണസംഖ്യ zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        തിരികെ സത്യം;
      }


      എങ്കിൽ (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        തിരികെ സത്യം;

      p_index--;
      സ്ഥിരം auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned പൂർണ്ണസംഖ്യ next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned പൂർണ്ണസംഖ്യ    size) സ്ഥിരം
  {
    // By default, start at the first bit of the first page of values.
    unsigned പൂർണ്ണസംഖ്യ start_page = 0;
    unsigned പൂർണ്ണസംഖ്യ start_page_മൂല്യം = 0;
    എങ്കിൽ (unlikely (codepoint != INVALID))
    {
      സ്ഥിരം auto* page_map_array = page_map.arrayZ;
      unsigned പൂർണ്ണസംഖ്യ major = get_major (codepoint);
      unsigned പൂർണ്ണസംഖ്യ i = last_page_lookup;
      എങ്കിൽ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	എങ്കിൽ (i >= page_map.length)
	  തിരികെ 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      എങ്കിൽ (page_map_array[i].major == major)
      {
        start_page_മൂല്യം = page_remainder (codepoint + 1);
        എങ്കിൽ (unlikely (start_page_മൂല്യം == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_മൂല്യം = 0;
        }
      }
    }

    unsigned പൂർണ്ണസംഖ്യ initial_size = size;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned പൂർണ്ണസംഖ്യ n = pages[page_map.arrayZ[i].index].ലിഖ (base, start_page_മൂല്യം, out, size);
      out += n;
      size -= n;
      start_page_മൂല്യം = 0;
    }
    തിരികെ initial_size - size;
  }

  unsigned പൂർണ്ണസംഖ്യ next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned പൂർണ്ണസംഖ്യ    size) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned പൂർണ്ണസംഖ്യ start_page = 0;
    unsigned പൂർണ്ണസംഖ്യ start_page_മൂല്യം = 0;
    എങ്കിൽ (unlikely (codepoint != INVALID))
    {
      സ്ഥിരം auto* page_map_array = page_map.arrayZ;
      unsigned പൂർണ്ണസംഖ്യ major = get_major (codepoint);
      unsigned പൂർണ്ണസംഖ്യ i = last_page_lookup;
      എങ്കിൽ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        എങ്കിൽ (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ആകുമ്പോൾ (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          തിരികെ initial_size - size;
        }
      }
      start_page = i;
      എങ്കിൽ (page_map_array[i].major == major)
      {
        start_page_മൂല്യം = page_remainder (codepoint + 1);
        എങ്കിൽ (unlikely (start_page_മൂല്യം == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_മൂല്യം = 0;
        }
      }
    }

    hb_codepoint_t next_മൂല്യം = codepoint + 1;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned പൂർണ്ണസംഖ്യ n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_മൂല്യം, out, size, &next_മൂല്യം);
      out += n;
      size -= n;
      start_page_മൂല്യം = 0;
    }
    ആകുമ്പോൾ (next_മൂല്യം < HB_SET_മൂല്യം_INVALID && size) {
      *out++ = next_മൂല്യം++;
      size--;
    }
    തിരികെ initial_size - size;
  }

  തർക്കം has_population () സ്ഥിരം { തിരികെ population != UINT_MAX; }
  unsigned പൂർണ്ണസംഖ്യ get_population () സ്ഥിരം
  {
    എങ്കിൽ (has_population ())
      തിരികെ population;

    unsigned പൂർണ്ണസംഖ്യ എടുക്കുക = 0;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = pages.length;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      എടുക്കുക += pages[i].get_population ();

    population = എടുക്കുക;
    തിരികെ എടുക്കുക;
  }
  തർക്കം get_singleton (hb_codepoint_t *codepoint) സ്ഥിരം
  {
    എങ്കിൽ (has_population ())
    {
      എങ്കിൽ (population != 1) തിരികെ അസത്യം;
      *codepoint = get_min ();
      തിരികെ സത്യം;
    }

    hb_codepoint_t singleton = INVALID;
    വേണ്ടി (സ്ഥിരം auto &map : page_map)
    {
      സ്ഥിരം auto &page = pages.arrayZ[map.index];
      വേണ്ടി (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	എങ്കിൽ (!bits) തുടരുക;
	എങ്കിൽ (singleton != INVALID || (bits & (bits - 1)))
	  തിരികെ അസത്യം;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    എങ്കിൽ (singleton == INVALID)
    {
      population = 0;
      തിരികെ അസത്യം;
    }
    population = 1;
    *codepoint = singleton;
    തിരികെ സത്യം;
  }
  hb_codepoint_t get_min () സ്ഥിരം
  {
    unsigned എണ്ണം = pages.length;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      സ്ഥിരം auto& map = page_map.arrayZ[i];
      സ്ഥിരം auto& page = pages.arrayZ[map.index];

      എങ്കിൽ (!page.is_empty ())
	തിരികെ map.major * page_t::PAGE_BITS + page.get_min ();
    }
    തിരികെ INVALID;
  }
  hb_codepoint_t get_max () സ്ഥിരം
  {
    unsigned എണ്ണം = pages.length;
    വേണ്ടി (signed i = എണ്ണം - 1; i >= 0; i--)
    {
      സ്ഥിരം auto& map = page_map.arrayZ[(unsigned) i];
      സ്ഥിരം auto& page = pages.arrayZ[map.index];

      എങ്കിൽ (!page.is_empty ())
	തിരികെ map.major * page_t::PAGE_BITS + page.get_max ();
    }
    തിരികെ INVALID;
  }

  സ്ഥിതം constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  ഘടന iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    സ്ഥിതം constexpr തർക്കം is_sorted_iterator = സത്യം;
    സ്ഥിതം constexpr തർക്കം has_fast_len = സത്യം;
    iter_t (സ്ഥിരം hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    തർക്കം init = സത്യം) : s (&s_), v (INVALID), l(0)
    {
      എങ്കിൽ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    തരനിർവചനം hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () സ്ഥിരം { തിരികെ v; }
    തർക്കം __more__ () സ്ഥിരം { തിരികെ v != INVALID; }
    ശൂന്യം __next__ () { s->next (&v); എങ്കിൽ (l) l--; }
    ശൂന്യം __prev__ () { s->previous (&v); }
    unsigned __len__ () സ്ഥിരം { തിരികെ l; }
    iter_t end () സ്ഥിരം { തിരികെ iter_t (*s, അസത്യം); }
    തർക്കം operator != (സ്ഥിരം iter_t& o) സ്ഥിരം
    { തിരികെ v != o.v; }

    സംരക്ഷിത:
    സ്ഥിരം hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () സ്ഥിരം { തിരികെ iter_t (*ഇത്); }
  operator iter_t () സ്ഥിരം { തിരികെ iter (); }

  സംരക്ഷിത:

  page_t *page_for (hb_codepoint_t g, തർക്കം insert = അസത്യം)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    എങ്കിൽ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      എങ്കിൽ (cached_page.major == major)
	തിരികെ &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    എങ്കിൽ (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      എങ്കിൽ (!insert)
        തിരികെ nullptr;

      എങ്കിൽ (unlikely (!resize (pages.length + 1)))
	തിരികെ nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    തിരികെ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  സ്ഥിരം page_t *page_for (hb_codepoint_t g) സ്ഥിരം
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    എങ്കിൽ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      എങ്കിൽ (cached_page.major == major)
	തിരികെ &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    എങ്കിൽ (!page_map.bfind (key, &i))
      തിരികെ nullptr;

    last_page_lookup = i;
    തിരികെ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned പൂർണ്ണസംഖ്യ i)
  {
    പ്രതിജ്ഞാ (i < page_map.length);
    തിരികെ pages.arrayZ[page_map.arrayZ[i].index];
  }
  സ്ഥിരം page_t &page_at (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
    പ്രതിജ്ഞാ (i < page_map.length);
    തിരികെ pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned പൂർണ്ണസംഖ്യ get_major (hb_codepoint_t g) സ്ഥിരം { തിരികെ g >> page_t::PAGE_BITS_LOG_2; }
  unsigned പൂർണ്ണസംഖ്യ page_remainder (hb_codepoint_t g) സ്ഥിരം { തിരികെ g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned പൂർണ്ണസംഖ്യ major) സ്ഥിരം { തിരികെ major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
