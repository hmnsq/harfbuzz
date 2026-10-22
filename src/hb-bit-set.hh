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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-bit-page.hh"

কাঠামো hb_bit_set_t
{
  hb_bit_set_t () = পূর্বনির্ধারিত;
  ~hb_bit_set_t () = পূর্বনির্ধারিত;

  hb_bit_set_t (ধ্রুবক hb_bit_set_t& other) : hb_bit_set_t () { set (other, সত্য); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*এই, other); }
  hb_bit_set_t& operator= (ধ্রুবক hb_bit_set_t& other) { set (other); ফেরত *এই; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*এই, other); ফেরত *এই; }
  friend শূন্য swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    যদি (likely (!a.successful || !b.successful))
      ফেরত;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  শূন্য init ()
  {
    successful = সত্য;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  শূন্য fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  কাঠামো page_map_t
  {
    পূর্ণসংখ্যা cmp (ধ্রুবক page_map_t &o) ধ্রুবক { ফেরত cmp (o.major); }
    পূর্ণসংখ্যা cmp (uint32_t o_major) ধ্রুবক { ফেরত (পূর্ণসংখ্যা) o_major - (পূর্ণসংখ্যা) major; }

    uint32_t major;
    uint32_t সূচক_2;
  };

  যৌক্তিক successful = সত্য; /* Allocations successful */
  mutable unsigned পূর্ণসংখ্যা population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  শূন্য err () { যদি (successful) successful = মিথ্যা; } /* TODO Remove */
  যৌক্তিক in_error () ধ্রুবক { ফেরত !successful; }

  যৌক্তিক resize (unsigned পূর্ণসংখ্যা গুনতি, যৌক্তিক clear = সত্য, যৌক্তিক exact_আকার = মিথ্যা)
  {
    যদি (unlikely (!successful)) ফেরত মিথ্যা;

    যদি (pages.দৈর্ঘ্য < গুনতি && (unsigned) pages.allocated < গুনতি && গুনতি <= 2)
      exact_আকার = সত্য; // Most sets are small and local

    যদি (unlikely (!pages.resize_full (গুনতি, clear, exact_আকার) ||
	!page_map.resize_full (গুনতি, clear, মিথ্যা)))
    {
      pages.resize_full (page_map.দৈর্ঘ্য, clear, exact_আকার);
      successful = মিথ্যা;
      ফেরত মিথ্যা;
    }
    ফেরত সত্য;
  }

  শূন্য alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = সত্য;
    clear ();
    ফেরত *এই;
  }

  শূন্য clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    যদি (!pages.দৈর্ঘ্য && !population) ফেরত;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  যৌক্তিক is_empty () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = pages.দৈর্ঘ্য;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (!pages[i].is_empty ())
	ফেরত মিথ্যা;
    ফেরত সত্য;
  }
  explicit operator যৌক্তিক () ধ্রুবক { ফেরত !is_empty (); }

  uint32_t hash () ধ্রুবক
  {
    uint32_t h = 0;
    জন্য (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.সূচক_2];
      যদি (unlikely (page.is_empty ())) চালু;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    ফেরত h;
  }

  ব্যক্তিগত:
  শূন্য dirty () { population = UINT_MAX; }
  সর্বজনীন:

  শূন্য add (hb_codepoint_t g)
  {
    যদি (unlikely (!successful)) ফেরত;
    যদি (unlikely (g == INVALID)) ফেরত;
    dirty ();
    page_t *page = page_for (g, সত্য); যদি (unlikely (!page)) ফেরত;
    page->add (g);
  }
  শূন্য add_bits (hb_codepoint_t g, uint64_t bits)
  {
    যদি (unlikely (!successful) || unlikely (!bits)) ফেরত;
    প্রতিজ্ঞা (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, সত্য); যদি (unlikely (!page)) ফেরত;
    page->add_bits (g, bits);
  }
  যৌক্তিক add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    যদি (unlikely (!successful)) ফেরত সত্য; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    যদি (unlikely (a > b || a == INVALID || b == INVALID)) ফেরত মিথ্যা;
    dirty ();
    unsigned পূর্ণসংখ্যা ma = get_major (a);
    unsigned পূর্ণসংখ্যা mb = get_major (b);
    যদি (ma == mb)
    {
      page_t *page = page_for (a, সত্য); যদি (unlikely (!page)) ফেরত মিথ্যা;
      page->add_range (a, b);
    }
    অন্যথা
    {
      page_t *page = page_for (a, সত্য); যদি (unlikely (!page)) ফেরত মিথ্যা;
      page->add_range (a, major_start (ma + 1) - 1);

      জন্য (unsigned পূর্ণসংখ্যা m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), সত্য); যদি (unlikely (!page)) ফেরত মিথ্যা;
	page->init1 ();
      }

      page = page_for (b, সত্য); যদি (unlikely (!page)) ফেরত মিথ্যা;
      page->add_range (major_start (mb), b);
    }
    ফেরত সত্য;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  স্থির অন্তঃরেখ ধ্রুবক Type& StructAtOffsetUnaligned(ধ্রুবক শূন্য *P, unsigned পূর্ণসংখ্যা offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    ফেরত * reinterpret_cast<ধ্রুবক Type*> ((ধ্রুবক অক্ষর *) P + offset);
#pragma GCC diagnostic বের_করো
  }

  template <typename T>
  শূন্য set_array (যৌক্তিক v, ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  {
    যদি (unlikely (!successful)) ফেরত;
    যদি (!গুনতি) ফেরত;
    dirty ();
    hb_codepoint_t g = *array;
    যখন (গুনতি)
    {
      unsigned পূর্ণসংখ্যা m = get_major (g);
      page_t *page = page_for (g, v); যদি (unlikely (v && !page)) ফেরত;
      unsigned পূর্ণসংখ্যা start = major_start (m);
      unsigned পূর্ণসংখ্যা end = major_start (m + 1);
      করো
      {
        যদি (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	গুনতি--;
      }
      যখন (গুনতি && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  শূন্য add_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  { set_array (সত্য, array, গুনতি, stride); }
  template <typename T>
  শূন্য add_array (ধ্রুবক hb_array_t<ধ্রুবক T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  শূন্য del_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  { set_array (মিথ্যা, array, গুনতি, stride); }
  template <typename T>
  শূন্য del_array (ধ্রুবক hb_array_t<ধ্রুবক T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  যৌক্তিক set_sorted_array (যৌক্তিক v, ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  {
    যদি (unlikely (!successful)) ফেরত সত্য; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    যদি (unlikely (!গুনতি)) ফেরত সত্য;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    যখন (গুনতি)
    {
      unsigned পূর্ণসংখ্যা m = get_major (g);
      page_t *page = page_for (g, v); যদি (unlikely (v && !page)) ফেরত মিথ্যা;
      unsigned পূর্ণসংখ্যা end = major_start (m + 1);
      করো
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	যদি (g < last_g) ফেরত মিথ্যা;
	last_g = g;

        যদি (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	গুনতি--;
      }
      যখন (গুনতি && (g = *array, g < end));
    }
    ফেরত সত্য;
  }

  template <typename T>
  যৌক্তিক add_sorted_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  { ফেরত set_sorted_array (সত্য, array, গুনতি, stride); }
  template <typename T>
  যৌক্তিক add_sorted_array (ধ্রুবক hb_sorted_array_t<ধ্রুবক T>& arr) { ফেরত add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  যৌক্তিক del_sorted_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  { ফেরত set_sorted_array (মিথ্যা, array, গুনতি, stride); }
  template <typename T>
  যৌক্তিক del_sorted_array (ধ্রুবক hb_sorted_array_t<ধ্রুবক T>& arr) { ফেরত del_sorted_array (&arr, arr.len ()); }

  শূন্য del (hb_codepoint_t g)
  {
    যদি (unlikely (!successful)) ফেরত;
    page_t *page = page_for (g);
    যদি (!page)
      ফেরত;
    dirty ();
    page->del (g);
  }
  শূন্য del_bits (hb_codepoint_t g, uint64_t bits)
  {
    যদি (unlikely (!successful) || unlikely (!bits)) ফেরত;
    প্রতিজ্ঞা (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    যদি (!page)
      ফেরত;
    dirty ();
    page->del_bits (g, bits);
  }

  ব্যক্তিগত:
  শূন্য del_pages (পূর্ণসংখ্যা ds, পূর্ণসংখ্যা de)
  {
    যদি (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      যদি (unlikely (!allocate_compact_workspace (compact_workspace))) ফেরত;

      unsigned পূর্ণসংখ্যা write_সূচক = 0;
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < page_map.দৈর্ঘ্য; i++)
      {
	পূর্ণসংখ্যা m = (পূর্ণসংখ্যা) page_map.arrayZ[i].major;
	যদি (m < ds || de < m)
	  page_map.arrayZ[write_সূচক++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_সূচক);
      resize (write_সূচক);
    }
  }


  সর্বজনীন:
  শূন্য del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    যদি (unlikely (!successful)) ফেরত;
    যদি (unlikely (a > b || a == INVALID)) ফেরত;
    dirty ();
    unsigned পূর্ণসংখ্যা ma = get_major (a);
    unsigned পূর্ণসংখ্যা mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    পূর্ণসংখ্যা ds = (a == major_start (ma))? (পূর্ণসংখ্যা) ma: (পূর্ণসংখ্যা) (ma + 1);
    পূর্ণসংখ্যা de = (b + 1 == major_start (mb + 1))? (পূর্ণসংখ্যা) mb: ((পূর্ণসংখ্যা) mb - 1);
    যদি (ds > de || (পূর্ণসংখ্যা) ma < ds)
    {
      page_t *page = page_for (a);
      যদি (page)
      {
	যদি (ma == mb)
	  page->del_range (a, b);
	অন্যথা
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    যদি (de < (পূর্ণসংখ্যা) mb && ma != mb)
    {
      page_t *page = page_for (b);
      যদি (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  যৌক্তিক get (hb_codepoint_t g) ধ্রুবক
  {
    ধ্রুবক page_t *page = page_for (g);
    যদি (!page)
      ফেরত মিথ্যা;
    ফেরত page->get (g);
  }
  যৌক্তিক may_have (hb_codepoint_t g) ধ্রুবক { ফেরত get (g); }

  /* Has interface. */
  যৌক্তিক operator [] (hb_codepoint_t k) ধ্রুবক { ফেরত get (k); }
  যৌক্তিক has (hb_codepoint_t k) ধ্রুবক { ফেরত (*এই)[k]; }
  /* Predicate. */
  যৌক্তিক operator () (hb_codepoint_t k) ধ্রুবক { ফেরত has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); ফেরত *এই; }
  hb_bit_set_t& operator << (ধ্রুবক hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ফেরত *এই; }

  যৌক্তিক intersects (ধ্রুবক hb_bit_set_t &other) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা na = pages.দৈর্ঘ্য;
    unsigned পূর্ণসংখ্যা nb = other.pages.দৈর্ঘ্য;

    unsigned পূর্ণসংখ্যা a = 0, b = 0;
    জন্য (; a < na && b < nb; )
    {
      যদি (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	যদি (page_at (a).intersects (other.page_at (b)))
	  ফেরত সত্য;
	a++;
	b++;
      }
      অন্যথা যদি (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      অন্যথা
	b++;
    }
    ফেরত মিথ্যা;
  }
  যৌক্তিক may_intersect (ধ্রুবক hb_bit_set_t &other) ধ্রুবক
  { ফেরত intersects (other); }

  যৌক্তিক intersects (hb_codepoint_t first, hb_codepoint_t last) ধ্রুবক
  {
    hb_codepoint_t c = first - 1;
    ফেরত next (&c) && c <= last;
  }
  শূন্য set (ধ্রুবক hb_bit_set_t &other, যৌক্তিক exact_আকার = মিথ্যা)
  {
    যদি (unlikely (!successful)) ফেরত;
    unsigned পূর্ণসংখ্যা গুনতি = other.pages.দৈর্ঘ্য;
    যদি (unlikely (!resize  (গুনতি, মিথ্যা, exact_আকার)))
      ফেরত;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  যৌক্তিক is_equal (ধ্রুবক hb_bit_set_t &other) ধ্রুবক
  {
    যদি (has_population () && other.has_population () &&
	population != other.population)
      ফেরত মিথ্যা;

    unsigned পূর্ণসংখ্যা na = pages.দৈর্ঘ্য;
    unsigned পূর্ণসংখ্যা nb = other.pages.দৈর্ঘ্য;

    unsigned পূর্ণসংখ্যা a = 0, b = 0;
    জন্য (; a < na && b < nb; )
    {
      যদি (page_at (a).is_empty ()) { a++; চালু; }
      যদি (other.page_at (b).is_empty ()) { b++; চালু; }
      যদি (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	ফেরত মিথ্যা;
      a++;
      b++;
    }
    জন্য (; a < na; a++)
      যদি (!page_at (a).is_empty ()) { ফেরত মিথ্যা; }
    জন্য (; b < nb; b++)
      যদি (!other.page_at (b).is_empty ()) { ফেরত মিথ্যা; }

    ফেরত সত্য;
  }

  যৌক্তিক is_subset (ধ্রুবক hb_bit_set_t &larger_set) ধ্রুবক
  {
    যদি (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      ফেরত মিথ্যা;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    যখন (spi < page_map.দৈর্ঘ্য && lpi < larger_set.page_map.দৈর্ঘ্য)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      যদি (spm < lpm) {
        যদি (!sp.is_empty ())
          ফেরত মিথ্যা;
        spi++;
        চালু;
      }

      যদি (lpm < spm) {
        lpi++;
        চালু;
      }

      auto lp = larger_set.page_at (lpi);
      যদি (!sp.is_subset (lp))
        ফেরত মিথ্যা;

      spi++;
      lpi++;
    }

    যখন (spi < page_map.দৈর্ঘ্য)
      যদি (!page_at (spi++).is_empty ())
        ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  ব্যক্তিগত:
  যৌক্তিক allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    যদি (unlikely (!workspace.resize_exact (pages.দৈর্ঘ্য)))
    {
      successful = মিথ্যা;
      ফেরত মিথ্যা;
    }

    ফেরত সত্য;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  শূন্য compact (hb_vector_t<unsigned>& workspace,
                unsigned পূর্ণসংখ্যা দৈর্ঘ্য)
  {
    প্রতিজ্ঞা(workspace.দৈর্ঘ্য == pages.দৈর্ঘ্য);
    hb_vector_t<unsigned>& old_সূচক_to_page_map_সূচক = workspace;

    hb_fill (old_সূচক_to_page_map_সূচক.writer(), 0xFFFFFFFF);
    জন্য (unsigned i = 0; i < দৈর্ঘ্য; i++)
      old_সূচক_to_page_map_সূচক[page_map[i].সূচক_2] =  i;

    compact_pages (old_সূচক_to_page_map_সূচক);
  }
  শূন্য compact_pages (ধ্রুবক hb_vector_t<unsigned>& old_সূচক_to_page_map_সূচক)
  {
    unsigned পূর্ণসংখ্যা write_সূচক = 0;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < pages.দৈর্ঘ্য; i++)
    {
      যদি (old_সূচক_to_page_map_সূচক[i] == 0xFFFFFFFF) চালু;

      যদি (write_সূচক < i)
	pages[write_সূচক] = pages[i];

      page_map[old_সূচক_to_page_map_সূচক[i]].সূচক_2 = write_সূচক;
      write_সূচক++;
    }
  }
  সর্বজনীন:

  শূন্য process_ (hb_bit_page_t::vector_t (*op) (ধ্রুবক hb_bit_page_t::vector_t &, ধ্রুবক hb_bit_page_t::vector_t &),
		 যৌক্তিক passthru_বাম, যৌক্তিক passthru_ডান,
		 ধ্রুবক hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    যদি (unlikely (!successful)) ফেরত;

    dirty ();

    unsigned পূর্ণসংখ্যা na = pages.দৈর্ঘ্য;
    unsigned পূর্ণসংখ্যা nb = other.pages.দৈর্ঘ্য;
    unsigned পূর্ণসংখ্যা পরবর্তী_page = na;

    unsigned পূর্ণসংখ্যা গুনতি = 0, new_গুনতি = 0;
    unsigned পূর্ণসংখ্যা a = 0, b = 0;
    unsigned পূর্ণসংখ্যা write_সূচক = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    যদি (!passthru_বাম)
    {
      যৌক্তিক allocated = workspace ? compact_workspace.resize (pages.দৈর্ঘ্য)
				 : allocate_compact_workspace (compact_workspace);
      যদি (unlikely (!allocated))
      {
	successful = মিথ্যা;
	ফেরত;
      }
    }

    জন্য (; a < na && b < nb; )
    {
      যদি (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	যদি (!passthru_বাম)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  যদি (write_সূচক < a)
	    page_map.arrayZ[write_সূচক] = page_map.arrayZ[a];
	  write_সূচক++;
	}

	গুনতি++;
	a++;
	b++;
      }
      অন্যথা যদি (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	যদি (passthru_বাম)
	  গুনতি++;
	a++;
      }
      অন্যথা
      {
	যদি (passthru_ডান)
	  গুনতি++;
	b++;
      }
    }
    যদি (passthru_বাম)
      গুনতি += na - a;
    যদি (passthru_ডান)
      গুনতি += nb - b;

    যদি (!passthru_বাম)
    {
      na  = write_সূচক;
      পরবর্তী_page = write_সূচক;
      compact (compact_workspace, write_সূচক);
    }

    যদি (unlikely (!resize (গুনতি)))
      ফেরত;

    new_গুনতি = গুনতি;

    /* Process in-place backward. */
    a = na;
    b = nb;
    জন্য (; a && b; )
    {
      যদি (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	গুনতি--;
	page_map.arrayZ[গুনতি] = page_map.arrayZ[a];
	page_at (গুনতি).v = op (page_at (a).v, other.page_at (b).v);
	page_at (গুনতি).dirty ();
      }
      অন্যথা যদি (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	যদি (passthru_বাম)
	{
	  গুনতি--;
	  page_map.arrayZ[গুনতি] = page_map.arrayZ[a];
	}
      }
      অন্যথা
      {
	b--;
	যদি (passthru_ডান)
	{
	  গুনতি--;
	  page_map.arrayZ[গুনতি].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[গুনতি].সূচক_2 = পরবর্তী_page++;
	  page_at (গুনতি) = other.page_at (b);
	}
      }
    }
    যদি (passthru_বাম)
      যখন (a)
      {
	a--;
	গুনতি--;
	page_map.arrayZ[গুনতি] = page_map.arrayZ[a];
      }
    যদি (passthru_ডান)
      যখন (b)
      {
	b--;
	গুনতি--;
	page_map.arrayZ[গুনতি].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[গুনতি].সূচক_2 = পরবর্তী_page++;
	page_at (গুনতি) = other.page_at (b);
      }
    প্রতিজ্ঞা (!গুনতি);
    resize (new_গুনতি);
  }
  template <typename Op>
  স্থির hb_bit_page_t::vector_t
  op_ (ধ্রুবক hb_bit_page_t::vector_t &a, ধ্রুবক hb_bit_page_t::vector_t &b)
  { ফেরত Op{} (a, b); }
  template <typename Op>
  শূন্য process (ধ্রুবক Op& op,
		ধ্রুবক hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  শূন্য union_ (ধ্রুবক hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  শূন্য intersect (ধ্রুবক hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  শূন্য subtract (ধ্রুবক hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  শূন্য symmetric_difference (ধ্রুবক hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  যৌক্তিক next (hb_codepoint_t *codepoint) ধ্রুবক
  {
    যদি (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ফেরত *codepoint != INVALID;
    }

    ধ্রুবক auto* page_map_array = page_map.arrayZ;
    unsigned পূর্ণসংখ্যা major = get_major (*codepoint);
    unsigned পূর্ণসংখ্যা i = last_page_lookup;

    যদি (unlikely (i >= page_map.দৈর্ঘ্য || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      যদি (i >= page_map.দৈর্ঘ্য) {
        *codepoint = INVALID;
        ফেরত মিথ্যা;
      }
      last_page_lookup = i;
    }

    ধ্রুবক auto* pages_array = pages.arrayZ;
    ধ্রুবক page_map_t &current = page_map_array[i];
    যদি (likely (current.major == major))
    {
      যদি (pages_array[current.সূচক_2].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        ফেরত সত্য;
      }
      i++;
    }

    জন্য (; i < page_map.দৈর্ঘ্য; i++)
    {
      ধ্রুবক page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.সূচক_2].get_min ();
      যদি (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	ফেরত সত্য;
      }
    }
    *codepoint = INVALID;
    ফেরত মিথ্যা;
  }
  যৌক্তিক পরবর্তী_bits (hb_codepoint_t *codepoint, uint64_t *bits) ধ্রুবক
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned পূর্ণসংখ্যা i = 0;
    unsigned পূর্ণসংখ্যা elt = 0;
    যদি (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      যদি (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        যাও done;
      base += page_t::ELT_BITS;

      unsigned পূর্ণসংখ্যা major = get_major (base);
      i = last_page_lookup;
      যদি (unlikely (i >= page_map.দৈর্ঘ্য || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	যদি (i >= page_map.দৈর্ঘ্য)
	  যাও done;
      }
      যদি (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    জন্য (; i < page_map.দৈর্ঘ্য; i++, elt = 0)
    {
      ধ্রুবক page_map_t &map = page_map.arrayZ[i];
      ধ্রুবক page_t &page = pages.arrayZ[map.সূচক_2];
      জন্য (; elt < page_t::len (); elt++)
	যদি (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  ফেরত সত্য;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ফেরত মিথ্যা;
  }
  যৌক্তিক previous (hb_codepoint_t *codepoint) ধ্রুবক
  {
    যদি (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ফেরত *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned পূর্ণসংখ্যা i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    যদি (i < page_map.দৈর্ঘ্য && page_map.arrayZ[i].major == map.major)
    {
      যদি (pages[page_map.arrayZ[i].সূচক_2].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	ফেরত সত্য;
      }
    }
    i--;
    জন্য (; (পূর্ণসংখ্যা) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].সূচক_2].get_max ();
      যদি (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	ফেরত সত্য;
      }
    }
    *codepoint = INVALID;
    ফেরত মিথ্যা;
  }
  যৌক্তিক পরবর্তী_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুবক
  {
    hb_codepoint_t i;

    i = *last;
    যদি (!next (&i))
    {
      *last = *first = INVALID;
      ফেরত মিথ্যা;
    }

    *last = *first = i;

    ধ্রুবক auto* page_map_array = page_map.arrayZ;
    ধ্রুবক auto* pages_array = pages.arrayZ;
    unsigned পূর্ণসংখ্যা major = get_major (*last);
    unsigned পূর্ণসংখ্যা p_সূচক = last_page_lookup;

    যদি (unlikely (p_সূচক >= page_map.দৈর্ঘ্য || page_map_array[p_সূচক].major != major))
    {
      page_map.bfind (major, &p_সূচক, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      প্রতিজ্ঞা(p_সূচক < page_map.দৈর্ঘ্য && page_map_array[p_সূচক].major == major);
      last_page_lookup = p_সূচক;
    }

    unsigned পূর্ণসংখ্যা rem = page_remainder (*last);
    unsigned পূর্ণসংখ্যা elt = rem / page_t::ELT_BITS;
    unsigned পূর্ণসংখ্যা bit = rem & page_t::ELT_MASK;

    ধ্রুবক page_t *page = &pages_array[page_map_array[p_সূচক].সূচক_2];
    unsigned পূর্ণসংখ্যা run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    যদি (run < page_t::ELT_BITS - bit)
      ফেরত সত্য;

    elt++;

    যখন (সত্য)
    {
      জন্য (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        যদি (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          চালু;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        ফেরত সত্য;
      }

      p_সূচক++;


      যদি (p_সূচক >= page_map.দৈর্ঘ্য || page_map_array[p_সূচক].major != major + 1)
        ফেরত সত্য;

      ধ্রুবক auto& entry = page_map_array[p_সূচক];
      major = entry.major;
      last_page_lookup = p_সূচক;
      page = &pages_array[entry.সূচক_2];
      elt = 0;
    }
  }

  যৌক্তিক previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুবক
  {
    hb_codepoint_t i;

    i = *first;
    যদি (!previous (&i))
    {
      *last = *first = INVALID;
      ফেরত মিথ্যা;
    }

    *last = *first = i;

    ধ্রুবক auto* page_map_array = page_map.arrayZ;
    ধ্রুবক auto* pages_array = pages.arrayZ;
    unsigned পূর্ণসংখ্যা major = get_major (*first);
    unsigned পূর্ণসংখ্যা p_সূচক = last_page_lookup;

    যদি (unlikely (p_সূচক >= page_map.দৈর্ঘ্য || page_map_array[p_সূচক].major != major))
    {
      page_map.bfind (major, &p_সূচক, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      প্রতিজ্ঞা(p_সূচক < page_map.দৈর্ঘ্য && page_map_array[p_সূচক].major == major);
      last_page_lookup = p_সূচক;
    }

    unsigned পূর্ণসংখ্যা rem = page_remainder (*first);
    পূর্ণসংখ্যা elt = rem / page_t::ELT_BITS;
    unsigned পূর্ণসংখ্যা bit = rem & page_t::ELT_MASK;

    ধ্রুবক page_t *page = &pages_array[page_map_array[p_সূচক].সূচক_2];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned পূর্ণসংখ্যা run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    যদি (run < bit + 1)
      ফেরত সত্য;

    elt--;

    যখন (সত্য)
    {
      জন্য (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        যদি (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          চালু;
        }
        uint64_t inv = ~w;
        unsigned পূর্ণসংখ্যা zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        ফেরত সত্য;
      }


      যদি (p_সূচক == 0 || page_map_array[p_সূচক - 1].major != major - 1)
        ফেরত সত্য;

      p_সূচক--;
      ধ্রুবক auto& entry = page_map_array[p_সূচক];

      major = entry.major;
      last_page_lookup = p_সূচক;
      page = &pages_array[entry.সূচক_2];
      elt = page_t::len () - 1;
    }
  }

  unsigned পূর্ণসংখ্যা পরবর্তী_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned পূর্ণসংখ্যা    আকার_2) ধ্রুবক
  {
    // By default, start at the first bit of the first page of values.
    unsigned পূর্ণসংখ্যা start_page = 0;
    unsigned পূর্ণসংখ্যা start_page_মান = 0;
    যদি (unlikely (codepoint != INVALID))
    {
      ধ্রুবক auto* page_map_array = page_map.arrayZ;
      unsigned পূর্ণসংখ্যা major = get_major (codepoint);
      unsigned পূর্ণসংখ্যা i = last_page_lookup;
      যদি (unlikely (i >= page_map.দৈর্ঘ্য || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	যদি (i >= page_map.দৈর্ঘ্য)
	  ফেরত 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      যদি (page_map_array[i].major == major)
      {
        start_page_মান = page_remainder (codepoint + 1);
        যদি (unlikely (start_page_মান == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_মান = 0;
        }
      }
    }

    unsigned পূর্ণসংখ্যা initial_আকার = আকার_2;
    জন্য (unsigned পূর্ণসংখ্যা i = start_page; i < page_map.দৈর্ঘ্য && আকার_2; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned পূর্ণসংখ্যা n = pages[page_map.arrayZ[i].সূচক_2].লিখ (base, start_page_মান, out, আকার_2);
      out += n;
      আকার_2 -= n;
      start_page_মান = 0;
    }
    ফেরত initial_আকার - আকার_2;
  }

  unsigned পূর্ণসংখ্যা পরবর্তী_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned পূর্ণসংখ্যা    আকার_2) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা initial_আকার = আকার_2;
    // By default, start at the first bit of the first page of values.
    unsigned পূর্ণসংখ্যা start_page = 0;
    unsigned পূর্ণসংখ্যা start_page_মান = 0;
    যদি (unlikely (codepoint != INVALID))
    {
      ধ্রুবক auto* page_map_array = page_map.arrayZ;
      unsigned পূর্ণসংখ্যা major = get_major (codepoint);
      unsigned পূর্ণসংখ্যা i = last_page_lookup;
      যদি (unlikely (i >= page_map.দৈর্ঘ্য || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        যদি (unlikely (i >= page_map.দৈর্ঘ্য))
        {
          // codepoint is greater than our max element.
          যখন (++codepoint != INVALID && আকার_2)
          {
            *out++ = codepoint;
            আকার_2--;
          }
          ফেরত initial_আকার - আকার_2;
        }
      }
      start_page = i;
      যদি (page_map_array[i].major == major)
      {
        start_page_মান = page_remainder (codepoint + 1);
        যদি (unlikely (start_page_মান == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_মান = 0;
        }
      }
    }

    hb_codepoint_t পরবর্তী_মান = codepoint + 1;
    জন্য (unsigned পূর্ণসংখ্যা i=start_page; i<page_map.দৈর্ঘ্য && আকার_2; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned পূর্ণসংখ্যা n = pages[page_map.arrayZ[i].সূচক_2].write_inverted (base, start_page_মান, out, আকার_2, &পরবর্তী_মান);
      out += n;
      আকার_2 -= n;
      start_page_মান = 0;
    }
    যখন (পরবর্তী_মান < HB_SET_মান_INVALID && আকার_2) {
      *out++ = পরবর্তী_মান++;
      আকার_2--;
    }
    ফেরত initial_আকার - আকার_2;
  }

  যৌক্তিক has_population () ধ্রুবক { ফেরত population != UINT_MAX; }
  unsigned পূর্ণসংখ্যা get_population () ধ্রুবক
  {
    যদি (has_population ())
      ফেরত population;

    unsigned পূর্ণসংখ্যা বের_করো = 0;
    unsigned পূর্ণসংখ্যা গুনতি = pages.দৈর্ঘ্য;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      বের_করো += pages[i].get_population ();

    population = বের_করো;
    ফেরত বের_করো;
  }
  যৌক্তিক get_singleton (hb_codepoint_t *codepoint) ধ্রুবক
  {
    যদি (has_population ())
    {
      যদি (population != 1) ফেরত মিথ্যা;
      *codepoint = get_min ();
      ফেরত সত্য;
    }

    hb_codepoint_t singleton = INVALID;
    জন্য (ধ্রুবক auto &map : page_map)
    {
      ধ্রুবক auto &page = pages.arrayZ[map.সূচক_2];
      জন্য (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	যদি (!bits) চালু;
	যদি (singleton != INVALID || (bits & (bits - 1)))
	  ফেরত মিথ্যা;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    যদি (singleton == INVALID)
    {
      population = 0;
      ফেরত মিথ্যা;
    }
    population = 1;
    *codepoint = singleton;
    ফেরত সত্য;
  }
  hb_codepoint_t get_min () ধ্রুবক
  {
    unsigned গুনতি = pages.দৈর্ঘ্য;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      ধ্রুবক auto& map = page_map.arrayZ[i];
      ধ্রুবক auto& page = pages.arrayZ[map.সূচক_2];

      যদি (!page.is_empty ())
	ফেরত map.major * page_t::PAGE_BITS + page.get_min ();
    }
    ফেরত INVALID;
  }
  hb_codepoint_t get_max () ধ্রুবক
  {
    unsigned গুনতি = pages.দৈর্ঘ্য;
    জন্য (signed i = গুনতি - 1; i >= 0; i--)
    {
      ধ্রুবক auto& map = page_map.arrayZ[(unsigned) i];
      ধ্রুবক auto& page = pages.arrayZ[map.সূচক_2];

      যদি (!page.is_empty ())
	ফেরত map.major * page_t::PAGE_BITS + page.get_max ();
    }
    ফেরত INVALID;
  }

  স্থির constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  কাঠামো iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    স্থির constexpr যৌক্তিক is_sorted_iterator = সত্য;
    স্থির constexpr যৌক্তিক has_fast_len = সত্য;
    iter_t (ধ্রুবক hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    যৌক্তিক init = সত্য) : s (&s_), v (INVALID), l(0)
    {
      যদি (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    প্রকারলেখ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ধ্রুবক { ফেরত v; }
    যৌক্তিক __more__ () ধ্রুবক { ফেরত v != INVALID; }
    শূন্য __next__ () { s->next (&v); যদি (l) l--; }
    শূন্য __prev__ () { s->previous (&v); }
    unsigned __len__ () ধ্রুবক { ফেরত l; }
    iter_t end () ধ্রুবক { ফেরত iter_t (*s, মিথ্যা); }
    যৌক্তিক operator != (ধ্রুবক iter_t& o) ধ্রুবক
    { ফেরত v != o.v; }

    সুরক্ষিত:
    ধ্রুবক hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () ধ্রুবক { ফেরত iter_t (*এই); }
  operator iter_t () ধ্রুবক { ফেরত iter (); }

  সুরক্ষিত:

  page_t *page_for (hb_codepoint_t g, যৌক্তিক insert = মিথ্যা)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    যদি (likely (i < page_map.দৈর্ঘ্য))
    {
      auto &cached_page = page_map.arrayZ[i];
      যদি (cached_page.major == major)
	ফেরত &pages.arrayZ[cached_page.সূচক_2];
    }

    page_map_t map = {major, pages.দৈর্ঘ্য};
    যদি (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      যদি (!insert)
        ফেরত nullptr;

      যদি (unlikely (!resize (pages.দৈর্ঘ্য + 1)))
	ফেরত nullptr;

      pages.arrayZ[map.সূচক_2].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.দৈর্ঘ্য - 1 - i) * page_map.item_আকার);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    ফেরত &pages.arrayZ[page_map.arrayZ[i].সূচক_2];
  }
  ধ্রুবক page_t *page_for (hb_codepoint_t g) ধ্রুবক
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    যদি (likely (i < page_map.দৈর্ঘ্য))
    {
      auto &cached_page = page_map.arrayZ[i];
      যদি (cached_page.major == major)
	ফেরত &pages.arrayZ[cached_page.সূচক_2];
    }

    page_map_t key = {major};
    যদি (!page_map.bfind (key, &i))
      ফেরত nullptr;

    last_page_lookup = i;
    ফেরত &pages.arrayZ[page_map.arrayZ[i].সূচক_2];
  }
  page_t &page_at (unsigned পূর্ণসংখ্যা i)
  {
    প্রতিজ্ঞা (i < page_map.দৈর্ঘ্য);
    ফেরত pages.arrayZ[page_map.arrayZ[i].সূচক_2];
  }
  ধ্রুবক page_t &page_at (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  {
    প্রতিজ্ঞা (i < page_map.দৈর্ঘ্য);
    ফেরত pages.arrayZ[page_map.arrayZ[i].সূচক_2];
  }
  unsigned পূর্ণসংখ্যা get_major (hb_codepoint_t g) ধ্রুবক { ফেরত g >> page_t::PAGE_BITS_LOG_2; }
  unsigned পূর্ণসংখ্যা page_remainder (hb_codepoint_t g) ধ্রুবক { ফেরত g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned পূর্ণসংখ্যা major) ধ্রুবক { ফেরত major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
