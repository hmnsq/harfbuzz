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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-bit-page.hh"

સંરચના hb_bit_set_t
{
  hb_bit_set_t () = મૂળભૂત;
  ~hb_bit_set_t () = મૂળભૂત;

  hb_bit_set_t (અચલ hb_bit_set_t& other) : hb_bit_set_t () { set (other, સાચું); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*આ, other); }
  hb_bit_set_t& operator= (અચલ hb_bit_set_t& other) { set (other); પાછા *આ; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*આ, other); પાછા *આ; }
  friend શૂન્ય swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    જો (likely (!a.successful || !b.successful))
      પાછા;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  શૂન્ય init ()
  {
    successful = સાચું;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  શૂન્ય fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  સંરચના page_map_t
  {
    પૂર્ણાંક cmp (અચલ page_map_t &o) અચલ { પાછા cmp (o.major); }
    પૂર્ણાંક cmp (uint32_t o_major) અચલ { પાછા (પૂર્ણાંક) o_major - (પૂર્ણાંક) major; }

    uint32_t major;
    uint32_t index;
  };

  તાર્કિક successful = સાચું; /* Allocations successful */
  mutable unsigned પૂર્ણાંક population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  શૂન્ય err () { જો (successful) successful = ખોટું; } /* TODO Remove */
  તાર્કિક in_error () અચલ { પાછા !successful; }

  તાર્કિક resize (unsigned પૂર્ણાંક ગણતરી, તાર્કિક clear = સાચું, તાર્કિક exact_size = ખોટું)
  {
    જો (unlikely (!successful)) પાછા ખોટું;

    જો (pages.length < ગણતરી && (unsigned) pages.allocated < ગણતરી && ગણતરી <= 2)
      exact_size = સાચું; // Most sets are small and local

    જો (unlikely (!pages.resize_full (ગણતરી, clear, exact_size) ||
	!page_map.resize_full (ગણતરી, clear, ખોટું)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = ખોટું;
      પાછા ખોટું;
    }
    પાછા સાચું;
  }

  શૂન્ય alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = સાચું;
    clear ();
    પાછા *આ;
  }

  શૂન્ય clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    જો (!pages.length && !population) પાછા;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  તાર્કિક is_empty () અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = pages.length;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (!pages[i].is_empty ())
	પાછા ખોટું;
    પાછા સાચું;
  }
  explicit operator તાર્કિક () અચલ { પાછા !is_empty (); }

  uint32_t hash () અચલ
  {
    uint32_t h = 0;
    માટે (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      જો (unlikely (page.is_empty ())) ચાલુ;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    પાછા h;
  }

  ખાનગી:
  શૂન્ય dirty () { population = UINT_MAX; }
  જાહેર:

  શૂન્ય add (hb_codepoint_t g)
  {
    જો (unlikely (!successful)) પાછા;
    જો (unlikely (g == INVALID)) પાછા;
    dirty ();
    page_t *page = page_for (g, સાચું); જો (unlikely (!page)) પાછા;
    page->add (g);
  }
  શૂન્ય add_bits (hb_codepoint_t g, uint64_t bits)
  {
    જો (unlikely (!successful) || unlikely (!bits)) પાછા;
    પ્રતિજ્ઞા (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, સાચું); જો (unlikely (!page)) પાછા;
    page->add_bits (g, bits);
  }
  તાર્કિક add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    જો (unlikely (!successful)) પાછા સાચું; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    જો (unlikely (a > b || a == INVALID || b == INVALID)) પાછા ખોટું;
    dirty ();
    unsigned પૂર્ણાંક ma = get_major (a);
    unsigned પૂર્ણાંક mb = get_major (b);
    જો (ma == mb)
    {
      page_t *page = page_for (a, સાચું); જો (unlikely (!page)) પાછા ખોટું;
      page->add_range (a, b);
    }
    નહીંતર
    {
      page_t *page = page_for (a, સાચું); જો (unlikely (!page)) પાછા ખોટું;
      page->add_range (a, major_start (ma + 1) - 1);

      માટે (unsigned પૂર્ણાંક m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), સાચું); જો (unlikely (!page)) પાછા ખોટું;
	page->init1 ();
      }

      page = page_for (b, સાચું); જો (unlikely (!page)) પાછા ખોટું;
      page->add_range (major_start (mb), b);
    }
    પાછા સાચું;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  સ્થિર અંતઃરેખ અચલ Type& StructAtOffsetUnaligned(અચલ શૂન્ય *P, unsigned પૂર્ણાંક offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    પાછા * reinterpret_cast<અચલ Type*> ((અચલ અક્ષર *) P + offset);
#pragma GCC diagnostic કાઢો
  }

  template <typename T>
  શૂન્ય set_array (તાર્કિક v, અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  {
    જો (unlikely (!successful)) પાછા;
    જો (!ગણતરી) પાછા;
    dirty ();
    hb_codepoint_t g = *array;
    જ્યારે (ગણતરી)
    {
      unsigned પૂર્ણાંક m = get_major (g);
      page_t *page = page_for (g, v); જો (unlikely (v && !page)) પાછા;
      unsigned પૂર્ણાંક start = major_start (m);
      unsigned પૂર્ણાંક end = major_start (m + 1);
      કરો
      {
        જો (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	ગણતરી--;
      }
      જ્યારે (ગણતરી && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  શૂન્ય add_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  { set_array (સાચું, array, ગણતરી, stride); }
  template <typename T>
  શૂન્ય add_array (અચલ hb_array_t<અચલ T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  શૂન્ય del_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  { set_array (ખોટું, array, ગણતરી, stride); }
  template <typename T>
  શૂન્ય del_array (અચલ hb_array_t<અચલ T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  તાર્કિક set_sorted_array (તાર્કિક v, અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  {
    જો (unlikely (!successful)) પાછા સાચું; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    જો (unlikely (!ગણતરી)) પાછા સાચું;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    જ્યારે (ગણતરી)
    {
      unsigned પૂર્ણાંક m = get_major (g);
      page_t *page = page_for (g, v); જો (unlikely (v && !page)) પાછા ખોટું;
      unsigned પૂર્ણાંક end = major_start (m + 1);
      કરો
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	જો (g < last_g) પાછા ખોટું;
	last_g = g;

        જો (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	ગણતરી--;
      }
      જ્યારે (ગણતરી && (g = *array, g < end));
    }
    પાછા સાચું;
  }

  template <typename T>
  તાર્કિક add_sorted_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  { પાછા set_sorted_array (સાચું, array, ગણતરી, stride); }
  template <typename T>
  તાર્કિક add_sorted_array (અચલ hb_sorted_array_t<અચલ T>& arr) { પાછા add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  તાર્કિક del_sorted_array (અચલ T *array, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride=કદ(T))
  { પાછા set_sorted_array (ખોટું, array, ગણતરી, stride); }
  template <typename T>
  તાર્કિક del_sorted_array (અચલ hb_sorted_array_t<અચલ T>& arr) { પાછા del_sorted_array (&arr, arr.len ()); }

  શૂન્ય del (hb_codepoint_t g)
  {
    જો (unlikely (!successful)) પાછા;
    page_t *page = page_for (g);
    જો (!page)
      પાછા;
    dirty ();
    page->del (g);
  }
  શૂન્ય del_bits (hb_codepoint_t g, uint64_t bits)
  {
    જો (unlikely (!successful) || unlikely (!bits)) પાછા;
    પ્રતિજ્ઞા (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    જો (!page)
      પાછા;
    dirty ();
    page->del_bits (g, bits);
  }

  ખાનગી:
  શૂન્ય del_pages (પૂર્ણાંક ds, પૂર્ણાંક de)
  {
    જો (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      જો (unlikely (!allocate_compact_workspace (compact_workspace))) પાછા;

      unsigned પૂર્ણાંક write_index = 0;
      માટે (unsigned પૂર્ણાંક i = 0; i < page_map.length; i++)
      {
	પૂર્ણાંક m = (પૂર્ણાંક) page_map.arrayZ[i].major;
	જો (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  જાહેર:
  શૂન્ય del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    જો (unlikely (!successful)) પાછા;
    જો (unlikely (a > b || a == INVALID)) પાછા;
    dirty ();
    unsigned પૂર્ણાંક ma = get_major (a);
    unsigned પૂર્ણાંક mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    પૂર્ણાંક ds = (a == major_start (ma))? (પૂર્ણાંક) ma: (પૂર્ણાંક) (ma + 1);
    પૂર્ણાંક de = (b + 1 == major_start (mb + 1))? (પૂર્ણાંક) mb: ((પૂર્ણાંક) mb - 1);
    જો (ds > de || (પૂર્ણાંક) ma < ds)
    {
      page_t *page = page_for (a);
      જો (page)
      {
	જો (ma == mb)
	  page->del_range (a, b);
	નહીંતર
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    જો (de < (પૂર્ણાંક) mb && ma != mb)
    {
      page_t *page = page_for (b);
      જો (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  તાર્કિક get (hb_codepoint_t g) અચલ
  {
    અચલ page_t *page = page_for (g);
    જો (!page)
      પાછા ખોટું;
    પાછા page->get (g);
  }
  તાર્કિક may_have (hb_codepoint_t g) અચલ { પાછા get (g); }

  /* Has interface. */
  તાર્કિક operator [] (hb_codepoint_t k) અચલ { પાછા get (k); }
  તાર્કિક has (hb_codepoint_t k) અચલ { પાછા (*આ)[k]; }
  /* Predicate. */
  તાર્કિક operator () (hb_codepoint_t k) અચલ { પાછા has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); પાછા *આ; }
  hb_bit_set_t& operator << (અચલ hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); પાછા *આ; }

  તાર્કિક intersects (અચલ hb_bit_set_t &other) અચલ
  {
    unsigned પૂર્ણાંક na = pages.length;
    unsigned પૂર્ણાંક nb = other.pages.length;

    unsigned પૂર્ણાંક a = 0, b = 0;
    માટે (; a < na && b < nb; )
    {
      જો (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	જો (page_at (a).intersects (other.page_at (b)))
	  પાછા સાચું;
	a++;
	b++;
      }
      નહીંતર જો (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      નહીંતર
	b++;
    }
    પાછા ખોટું;
  }
  તાર્કિક may_intersect (અચલ hb_bit_set_t &other) અચલ
  { પાછા intersects (other); }

  તાર્કિક intersects (hb_codepoint_t first, hb_codepoint_t last) અચલ
  {
    hb_codepoint_t c = first - 1;
    પાછા next (&c) && c <= last;
  }
  શૂન્ય set (અચલ hb_bit_set_t &other, તાર્કિક exact_size = ખોટું)
  {
    જો (unlikely (!successful)) પાછા;
    unsigned પૂર્ણાંક ગણતરી = other.pages.length;
    જો (unlikely (!resize  (ગણતરી, ખોટું, exact_size)))
      પાછા;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  તાર્કિક is_equal (અચલ hb_bit_set_t &other) અચલ
  {
    જો (has_population () && other.has_population () &&
	population != other.population)
      પાછા ખોટું;

    unsigned પૂર્ણાંક na = pages.length;
    unsigned પૂર્ણાંક nb = other.pages.length;

    unsigned પૂર્ણાંક a = 0, b = 0;
    માટે (; a < na && b < nb; )
    {
      જો (page_at (a).is_empty ()) { a++; ચાલુ; }
      જો (other.page_at (b).is_empty ()) { b++; ચાલુ; }
      જો (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	પાછા ખોટું;
      a++;
      b++;
    }
    માટે (; a < na; a++)
      જો (!page_at (a).is_empty ()) { પાછા ખોટું; }
    માટે (; b < nb; b++)
      જો (!other.page_at (b).is_empty ()) { પાછા ખોટું; }

    પાછા સાચું;
  }

  તાર્કિક is_subset (અચલ hb_bit_set_t &larger_set) અચલ
  {
    જો (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      પાછા ખોટું;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    જ્યારે (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      જો (spm < lpm) {
        જો (!sp.is_empty ())
          પાછા ખોટું;
        spi++;
        ચાલુ;
      }

      જો (lpm < spm) {
        lpi++;
        ચાલુ;
      }

      auto lp = larger_set.page_at (lpi);
      જો (!sp.is_subset (lp))
        પાછા ખોટું;

      spi++;
      lpi++;
    }

    જ્યારે (spi < page_map.length)
      જો (!page_at (spi++).is_empty ())
        પાછા ખોટું;

    પાછા સાચું;
  }

  ખાનગી:
  તાર્કિક allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    જો (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = ખોટું;
      પાછા ખોટું;
    }

    પાછા સાચું;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  શૂન્ય compact (hb_vector_t<unsigned>& workspace,
                unsigned પૂર્ણાંક length)
  {
    પ્રતિજ્ઞા(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    માટે (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  શૂન્ય compact_pages (અચલ hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned પૂર્ણાંક write_index = 0;
    માટે (unsigned પૂર્ણાંક i = 0; i < pages.length; i++)
    {
      જો (old_index_to_page_map_index[i] == 0xFFFFFFFF) ચાલુ;

      જો (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  જાહેર:

  શૂન્ય process_ (hb_bit_page_t::vector_t (*op) (અચલ hb_bit_page_t::vector_t &, અચલ hb_bit_page_t::vector_t &),
		 તાર્કિક passthru_ડાબું, તાર્કિક passthru_જમણું,
		 અચલ hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    જો (unlikely (!successful)) પાછા;

    dirty ();

    unsigned પૂર્ણાંક na = pages.length;
    unsigned પૂર્ણાંક nb = other.pages.length;
    unsigned પૂર્ણાંક next_page = na;

    unsigned પૂર્ણાંક ગણતરી = 0, new_ગણતરી = 0;
    unsigned પૂર્ણાંક a = 0, b = 0;
    unsigned પૂર્ણાંક write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    જો (!passthru_ડાબું)
    {
      તાર્કિક allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      જો (unlikely (!allocated))
      {
	successful = ખોટું;
	પાછા;
      }
    }

    માટે (; a < na && b < nb; )
    {
      જો (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	જો (!passthru_ડાબું)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  જો (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	ગણતરી++;
	a++;
	b++;
      }
      નહીંતર જો (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	જો (passthru_ડાબું)
	  ગણતરી++;
	a++;
      }
      નહીંતર
      {
	જો (passthru_જમણું)
	  ગણતરી++;
	b++;
      }
    }
    જો (passthru_ડાબું)
      ગણતરી += na - a;
    જો (passthru_જમણું)
      ગણતરી += nb - b;

    જો (!passthru_ડાબું)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    જો (unlikely (!resize (ગણતરી)))
      પાછા;

    new_ગણતરી = ગણતરી;

    /* Process in-place backward. */
    a = na;
    b = nb;
    માટે (; a && b; )
    {
      જો (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	ગણતરી--;
	page_map.arrayZ[ગણતરી] = page_map.arrayZ[a];
	page_at (ગણતરી).v = op (page_at (a).v, other.page_at (b).v);
	page_at (ગણતરી).dirty ();
      }
      નહીંતર જો (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	જો (passthru_ડાબું)
	{
	  ગણતરી--;
	  page_map.arrayZ[ગણતરી] = page_map.arrayZ[a];
	}
      }
      નહીંતર
      {
	b--;
	જો (passthru_જમણું)
	{
	  ગણતરી--;
	  page_map.arrayZ[ગણતરી].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[ગણતરી].index = next_page++;
	  page_at (ગણતરી) = other.page_at (b);
	}
      }
    }
    જો (passthru_ડાબું)
      જ્યારે (a)
      {
	a--;
	ગણતરી--;
	page_map.arrayZ[ગણતરી] = page_map.arrayZ[a];
      }
    જો (passthru_જમણું)
      જ્યારે (b)
      {
	b--;
	ગણતરી--;
	page_map.arrayZ[ગણતરી].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[ગણતરી].index = next_page++;
	page_at (ગણતરી) = other.page_at (b);
      }
    પ્રતિજ્ઞા (!ગણતરી);
    resize (new_ગણતરી);
  }
  template <typename Op>
  સ્થિર hb_bit_page_t::vector_t
  op_ (અચલ hb_bit_page_t::vector_t &a, અચલ hb_bit_page_t::vector_t &b)
  { પાછા Op{} (a, b); }
  template <typename Op>
  શૂન્ય process (અચલ Op& op,
		અચલ hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  શૂન્ય union_ (અચલ hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  શૂન્ય intersect (અચલ hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  શૂન્ય subtract (અચલ hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  શૂન્ય symmetric_difference (અચલ hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  તાર્કિક next (hb_codepoint_t *codepoint) અચલ
  {
    જો (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      પાછા *codepoint != INVALID;
    }

    અચલ auto* page_map_array = page_map.arrayZ;
    unsigned પૂર્ણાંક major = get_major (*codepoint);
    unsigned પૂર્ણાંક i = last_page_lookup;

    જો (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      જો (i >= page_map.length) {
        *codepoint = INVALID;
        પાછા ખોટું;
      }
      last_page_lookup = i;
    }

    અચલ auto* pages_array = pages.arrayZ;
    અચલ page_map_t &current = page_map_array[i];
    જો (likely (current.major == major))
    {
      જો (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        પાછા સાચું;
      }
      i++;
    }

    માટે (; i < page_map.length; i++)
    {
      અચલ page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      જો (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	પાછા સાચું;
      }
    }
    *codepoint = INVALID;
    પાછા ખોટું;
  }
  તાર્કિક next_bits (hb_codepoint_t *codepoint, uint64_t *bits) અચલ
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned પૂર્ણાંક i = 0;
    unsigned પૂર્ણાંક elt = 0;
    જો (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      જો (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        જાઓ done;
      base += page_t::ELT_BITS;

      unsigned પૂર્ણાંક major = get_major (base);
      i = last_page_lookup;
      જો (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	જો (i >= page_map.length)
	  જાઓ done;
      }
      જો (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    માટે (; i < page_map.length; i++, elt = 0)
    {
      અચલ page_map_t &map = page_map.arrayZ[i];
      અચલ page_t &page = pages.arrayZ[map.index];
      માટે (; elt < page_t::len (); elt++)
	જો (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  પાછા સાચું;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    પાછા ખોટું;
  }
  તાર્કિક previous (hb_codepoint_t *codepoint) અચલ
  {
    જો (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      પાછા *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned પૂર્ણાંક i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    જો (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      જો (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	પાછા સાચું;
      }
    }
    i--;
    માટે (; (પૂર્ણાંક) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      જો (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	પાછા સાચું;
      }
    }
    *codepoint = INVALID;
    પાછા ખોટું;
  }
  તાર્કિક next_range (hb_codepoint_t *first, hb_codepoint_t *last) અચલ
  {
    hb_codepoint_t i;

    i = *last;
    જો (!next (&i))
    {
      *last = *first = INVALID;
      પાછા ખોટું;
    }

    *last = *first = i;

    અચલ auto* page_map_array = page_map.arrayZ;
    અચલ auto* pages_array = pages.arrayZ;
    unsigned પૂર્ણાંક major = get_major (*last);
    unsigned પૂર્ણાંક p_index = last_page_lookup;

    જો (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      પ્રતિજ્ઞા(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned પૂર્ણાંક rem = page_remainder (*last);
    unsigned પૂર્ણાંક elt = rem / page_t::ELT_BITS;
    unsigned પૂર્ણાંક bit = rem & page_t::ELT_MASK;

    અચલ page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned પૂર્ણાંક run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    જો (run < page_t::ELT_BITS - bit)
      પાછા સાચું;

    elt++;

    જ્યારે (સાચું)
    {
      માટે (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        જો (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          ચાલુ;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        પાછા સાચું;
      }

      p_index++;


      જો (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        પાછા સાચું;

      અચલ auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  તાર્કિક previous_range (hb_codepoint_t *first, hb_codepoint_t *last) અચલ
  {
    hb_codepoint_t i;

    i = *first;
    જો (!previous (&i))
    {
      *last = *first = INVALID;
      પાછા ખોટું;
    }

    *last = *first = i;

    અચલ auto* page_map_array = page_map.arrayZ;
    અચલ auto* pages_array = pages.arrayZ;
    unsigned પૂર્ણાંક major = get_major (*first);
    unsigned પૂર્ણાંક p_index = last_page_lookup;

    જો (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      પ્રતિજ્ઞા(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned પૂર્ણાંક rem = page_remainder (*first);
    પૂર્ણાંક elt = rem / page_t::ELT_BITS;
    unsigned પૂર્ણાંક bit = rem & page_t::ELT_MASK;

    અચલ page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned પૂર્ણાંક run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    જો (run < bit + 1)
      પાછા સાચું;

    elt--;

    જ્યારે (સાચું)
    {
      માટે (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        જો (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          ચાલુ;
        }
        uint64_t inv = ~w;
        unsigned પૂર્ણાંક zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        પાછા સાચું;
      }


      જો (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        પાછા સાચું;

      p_index--;
      અચલ auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned પૂર્ણાંક next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned પૂર્ણાંક    size) અચલ
  {
    // By default, start at the first bit of the first page of values.
    unsigned પૂર્ણાંક start_page = 0;
    unsigned પૂર્ણાંક start_page_કિંમત = 0;
    જો (unlikely (codepoint != INVALID))
    {
      અચલ auto* page_map_array = page_map.arrayZ;
      unsigned પૂર્ણાંક major = get_major (codepoint);
      unsigned પૂર્ણાંક i = last_page_lookup;
      જો (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	જો (i >= page_map.length)
	  પાછા 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      જો (page_map_array[i].major == major)
      {
        start_page_કિંમત = page_remainder (codepoint + 1);
        જો (unlikely (start_page_કિંમત == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_કિંમત = 0;
        }
      }
    }

    unsigned પૂર્ણાંક initial_size = size;
    માટે (unsigned પૂર્ણાંક i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned પૂર્ણાંક n = pages[page_map.arrayZ[i].index].લિખ (base, start_page_કિંમત, out, size);
      out += n;
      size -= n;
      start_page_કિંમત = 0;
    }
    પાછા initial_size - size;
  }

  unsigned પૂર્ણાંક next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned પૂર્ણાંક    size) અચલ
  {
    unsigned પૂર્ણાંક initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned પૂર્ણાંક start_page = 0;
    unsigned પૂર્ણાંક start_page_કિંમત = 0;
    જો (unlikely (codepoint != INVALID))
    {
      અચલ auto* page_map_array = page_map.arrayZ;
      unsigned પૂર્ણાંક major = get_major (codepoint);
      unsigned પૂર્ણાંક i = last_page_lookup;
      જો (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        જો (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          જ્યારે (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          પાછા initial_size - size;
        }
      }
      start_page = i;
      જો (page_map_array[i].major == major)
      {
        start_page_કિંમત = page_remainder (codepoint + 1);
        જો (unlikely (start_page_કિંમત == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_કિંમત = 0;
        }
      }
    }

    hb_codepoint_t next_કિંમત = codepoint + 1;
    માટે (unsigned પૂર્ણાંક i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned પૂર્ણાંક n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_કિંમત, out, size, &next_કિંમત);
      out += n;
      size -= n;
      start_page_કિંમત = 0;
    }
    જ્યારે (next_કિંમત < HB_SET_કિંમત_INVALID && size) {
      *out++ = next_કિંમત++;
      size--;
    }
    પાછા initial_size - size;
  }

  તાર્કિક has_population () અચલ { પાછા population != UINT_MAX; }
  unsigned પૂર્ણાંક get_population () અચલ
  {
    જો (has_population ())
      પાછા population;

    unsigned પૂર્ણાંક કાઢો = 0;
    unsigned પૂર્ણાંક ગણતરી = pages.length;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      કાઢો += pages[i].get_population ();

    population = કાઢો;
    પાછા કાઢો;
  }
  તાર્કિક get_singleton (hb_codepoint_t *codepoint) અચલ
  {
    જો (has_population ())
    {
      જો (population != 1) પાછા ખોટું;
      *codepoint = get_min ();
      પાછા સાચું;
    }

    hb_codepoint_t singleton = INVALID;
    માટે (અચલ auto &map : page_map)
    {
      અચલ auto &page = pages.arrayZ[map.index];
      માટે (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	જો (!bits) ચાલુ;
	જો (singleton != INVALID || (bits & (bits - 1)))
	  પાછા ખોટું;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    જો (singleton == INVALID)
    {
      population = 0;
      પાછા ખોટું;
    }
    population = 1;
    *codepoint = singleton;
    પાછા સાચું;
  }
  hb_codepoint_t get_min () અચલ
  {
    unsigned ગણતરી = pages.length;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      અચલ auto& map = page_map.arrayZ[i];
      અચલ auto& page = pages.arrayZ[map.index];

      જો (!page.is_empty ())
	પાછા map.major * page_t::PAGE_BITS + page.get_min ();
    }
    પાછા INVALID;
  }
  hb_codepoint_t get_max () અચલ
  {
    unsigned ગણતરી = pages.length;
    માટે (signed i = ગણતરી - 1; i >= 0; i--)
    {
      અચલ auto& map = page_map.arrayZ[(unsigned) i];
      અચલ auto& page = pages.arrayZ[map.index];

      જો (!page.is_empty ())
	પાછા map.major * page_t::PAGE_BITS + page.get_max ();
    }
    પાછા INVALID;
  }

  સ્થિર constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  સંરચના iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    સ્થિર constexpr તાર્કિક is_sorted_iterator = સાચું;
    સ્થિર constexpr તાર્કિક has_fast_len = સાચું;
    iter_t (અચલ hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    તાર્કિક init = સાચું) : s (&s_), v (INVALID), l(0)
    {
      જો (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    પ્રકારવ્યાખ્યા hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () અચલ { પાછા v; }
    તાર્કિક __more__ () અચલ { પાછા v != INVALID; }
    શૂન્ય __next__ () { s->next (&v); જો (l) l--; }
    શૂન્ય __prev__ () { s->previous (&v); }
    unsigned __len__ () અચલ { પાછા l; }
    iter_t end () અચલ { પાછા iter_t (*s, ખોટું); }
    તાર્કિક operator != (અચલ iter_t& o) અચલ
    { પાછા v != o.v; }

    સુરક્ષિત:
    અચલ hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () અચલ { પાછા iter_t (*આ); }
  operator iter_t () અચલ { પાછા iter (); }

  સુરક્ષિત:

  page_t *page_for (hb_codepoint_t g, તાર્કિક insert = ખોટું)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    જો (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      જો (cached_page.major == major)
	પાછા &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    જો (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      જો (!insert)
        પાછા nullptr;

      જો (unlikely (!resize (pages.length + 1)))
	પાછા nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    પાછા &pages.arrayZ[page_map.arrayZ[i].index];
  }
  અચલ page_t *page_for (hb_codepoint_t g) અચલ
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    જો (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      જો (cached_page.major == major)
	પાછા &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    જો (!page_map.bfind (key, &i))
      પાછા nullptr;

    last_page_lookup = i;
    પાછા &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned પૂર્ણાંક i)
  {
    પ્રતિજ્ઞા (i < page_map.length);
    પાછા pages.arrayZ[page_map.arrayZ[i].index];
  }
  અચલ page_t &page_at (unsigned પૂર્ણાંક i) અચલ
  {
    પ્રતિજ્ઞા (i < page_map.length);
    પાછા pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned પૂર્ણાંક get_major (hb_codepoint_t g) અચલ { પાછા g >> page_t::PAGE_BITS_LOG_2; }
  unsigned પૂર્ણાંક page_remainder (hb_codepoint_t g) અચલ { પાછા g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned પૂર્ણાંક major) અચલ { પાછા major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
