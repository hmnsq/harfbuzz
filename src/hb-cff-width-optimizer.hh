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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

ନାମାକାଶ CFF {

/* Calculate byte cost for encoding a width delta */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned
width_delta_cost (ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta)
{
  delta = abs (delta);
  ଯଦି (delta <= 107) ଫେରନ୍ତୁ 1;
  ଯଦି (delta <= 1131) ଫେରନ୍ତୁ 2;
  ଫେରନ୍ତୁ 5;
}

/* Cumulative sum forward */
ସ୍ଥିର ଶୂନ୍ୟ
cumsum_forward (ଅଚଳ hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  ପାଇଁ (unsigned x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
ସ୍ଥିର ଶୂନ୍ୟ
cummax_forward (ଅଚଳ hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  ପାଇଁ (unsigned x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
ସ୍ଥିର ଶୂନ୍ୟ
cumsum_backward (ଅଚଳ hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) max_w; x >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) min_w; x--)
  {
    v += freq.get ((unsigned) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
ସ୍ଥିର ଶୂନ୍ୟ
cummax_backward (ଅଚଳ hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) max_w; x >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) min_w; x--)
  {
    v = hb_max (v, freq.get ((unsigned) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned
safe_get (ଅଚଳ hb_vector_t<unsigned> &vec, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, unsigned min_w, unsigned max_w)
{
  ଯଦି (x < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) min_w || x > (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) max_w) ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
ସ୍ଥିର ଶୂନ୍ୟ
optimize_widths (ଅଚଳ hb_vector_t<unsigned> &width_list,
                 unsigned &default_width,
                 unsigned &nominal_width)
{
  ଯଦି (width_list.length == 0)
  {
    default_width = nominal_width = 0;
    ଫେରନ୍ତୁ;
  }

  /* Build frequency map */
  hb_hashmap_t<unsigned, unsigned> widths;
  unsigned min_w = width_list[0];
  unsigned max_w = width_list[0];

  ପାଇଁ (unsigned w : width_list)
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
    ଫେରନ୍ତୁ safe_get (cumFrqU, x, min_w, max_w) +
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
    ଫେରନ୍ତୁ hb_max (u, d);
  };

  /* Find best nominal */
  unsigned best_nominal = min_w;
  unsigned best_cost = nomnCost (min_w) - dfltCost (min_w);

  ପାଇଁ (unsigned x = min_w + 1; x <= max_w; x++)
  {
    unsigned cost = nomnCost (x) - dfltCost (x);
    ଯଦି (cost < best_cost)
    {
      best_cost = cost;
      best_nominal = x;
    }
  }

  /* Work back the best default */
  unsigned best_default = best_nominal;
  unsigned best_default_cost = (unsigned) -1;

  /* Check candidates around best_nominal */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା candidates[] = {
    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) best_nominal,
    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) best_nominal - 108,
    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) best_nominal - 1132,
    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) best_nominal + 108,
    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) best_nominal + 1132
  };

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା candidate : candidates)
  {
    ଯଦି (candidate < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) min_w || candidate > (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) max_w)
      ଜାରି;

    /* Compute actual cost with this default */
    unsigned cost = 0;
    ପାଇଁ (auto kv : widths.iter ())
    {
      unsigned w = kv.first;
      unsigned freq = kv.second;

      ଯଦି (w == (unsigned) candidate)
        ଜାରି;

      cost += freq * width_delta_cost ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) w - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) best_nominal);
    }

    ଯଦି (cost < best_default_cost)
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
