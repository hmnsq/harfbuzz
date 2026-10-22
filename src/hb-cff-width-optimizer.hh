/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#ifndef HB_CFF_WIDTH_OPTIMIZER_HH
#define HB_CFF_WIDTH_OPTIMIZER_HH

#অন্তর্ভাব "hb.hh"

নামাকাশ CFF {

/* Calculate byte cost for encoding a width delta */
স্থির অন্তঃরেখ unsigned
width_delta_cost (পূর্ণসংখ্যা delta)
{
  delta = abs (delta);
  যদি (delta <= 107) ফেরত 1;
  যদি (delta <= 1131) ফেরত 2;
  ফেরত 5;
}

/* Cumulative sum forward */
স্থির শূন্য
cumsum_forward (ধ্রুবক hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  জন্য (unsigned x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
স্থির শূন্য
cummax_forward (ধ্রুবক hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  জন্য (unsigned x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
স্থির শূন্য
cumsum_backward (ধ্রুবক hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  জন্য (পূর্ণসংখ্যা x = (পূর্ণসংখ্যা) max_w; x >= (পূর্ণসংখ্যা) min_w; x--)
  {
    v += freq.get ((unsigned) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
স্থির শূন্য
cummax_backward (ধ্রুবক hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  জন্য (পূর্ণসংখ্যা x = (পূর্ণসংখ্যা) max_w; x >= (পূর্ণসংখ্যা) min_w; x--)
  {
    v = hb_max (v, freq.get ((unsigned) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
স্থির অন্তঃরেখ unsigned
safe_get (ধ্রুবক hb_vector_t<unsigned> &vec, পূর্ণসংখ্যা x, unsigned min_w, unsigned max_w)
{
  যদি (x < (পূর্ণসংখ্যা) min_w || x > (পূর্ণসংখ্যা) max_w) ফেরত 0;
  ফেরত vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
স্থির শূন্য
optimize_widths (ধ্রুবক hb_vector_t<unsigned> &width_list,
                 unsigned &default_width,
                 unsigned &nominal_width)
{
  যদি (width_list.দৈর্ঘ্য == 0)
  {
    default_width = nominal_width = 0;
    ফেরত;
  }

  /* Build frequency map */
  hb_hashmap_t<unsigned, unsigned> widths;
  unsigned min_w = width_list[0];
  unsigned max_w = width_list[0];

  জন্য (unsigned w : width_list)
  {
    widths.set (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_vector_t<unsigned> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  auto nomnCost = [&] (unsigned x) -> unsigned {
    ফেরত safe_get (cumFrqU, x, min_w, max_w) +
           safe_get (cumFrqU, x - 108, min_w, max_w) +
           safe_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           safe_get (cumFrqD, x, min_w, max_w) +
           safe_get (cumFrqD, x + 108, min_w, max_w) +
           safe_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  auto dfltCost = [&] (unsigned x) -> unsigned {
    unsigned u = hb_max (hb_max (safe_get (cumMaxU, x, min_w, max_w),
                                  safe_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         safe_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    unsigned d = hb_max (hb_max (safe_get (cumMaxD, x, min_w, max_w),
                                  safe_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         safe_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    ফেরত hb_max (u, d);
  };

  /* Find best nominal */
  unsigned best_nominal = min_w;
  unsigned best_cost = nomnCost (min_w) - dfltCost (min_w);

  জন্য (unsigned x = min_w + 1; x <= max_w; x++)
  {
    unsigned cost = nomnCost (x) - dfltCost (x);
    যদি (cost < best_cost)
    {
      best_cost = cost;
      best_nominal = x;
    }
  }

  /* Work back the best default */
  unsigned best_default = best_nominal;
  unsigned best_default_cost = (unsigned) -1;

  /* Check candidates around best_nominal */
  পূর্ণসংখ্যা candidates[] = {
    (পূর্ণসংখ্যা) best_nominal,
    (পূর্ণসংখ্যা) best_nominal - 108,
    (পূর্ণসংখ্যা) best_nominal - 1132,
    (পূর্ণসংখ্যা) best_nominal + 108,
    (পূর্ণসংখ্যা) best_nominal + 1132
  };

  জন্য (পূর্ণসংখ্যা candidate : candidates)
  {
    যদি (candidate < (পূর্ণসংখ্যা) min_w || candidate > (পূর্ণসংখ্যা) max_w)
      চালু;

    /* Compute actual cost with this default */
    unsigned cost = 0;
    জন্য (auto kv : widths.iter ())
    {
      unsigned w = kv.first;
      unsigned freq = kv.second;

      যদি (w == (unsigned) candidate)
        চালু;

      cost += freq * width_delta_cost ((পূর্ণসংখ্যা) w - (পূর্ণসংখ্যা) best_nominal);
    }

    যদি (cost < best_default_cost)
    {
      best_default_cost = cost;
      best_default = (unsigned) candidate;
    }
  }

  default_width = best_default;
  nominal_width = best_nominal;
}

} /* namespace CFF */

#endif /* HB_CFF_WIDTH_OPTIMIZER_HH */
