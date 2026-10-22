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

#شامل "hb.hh"

فضاینام CFF {

/* Calculate byte cost for encoding a width delta */
ایستا درونخطی unsigned
width_delta_cost (صحیح delta)
{
  delta = abs (delta);
  اگر (delta <= 107) بازگشت 1;
  اگر (delta <= 1131) بازگشت 2;
  بازگشت 5;
}

/* Cumulative sum forward */
ایستا تهی
cumsum_forward (ثابت hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  برای (unsigned x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
ایستا تهی
cummax_forward (ثابت hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  برای (unsigned x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
ایستا تهی
cumsum_backward (ثابت hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  برای (صحیح x = (صحیح) max_w; x >= (صحیح) min_w; x--)
  {
    v += freq.get ((unsigned) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
ایستا تهی
cummax_backward (ثابت hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  برای (صحیح x = (صحیح) max_w; x >= (صحیح) min_w; x--)
  {
    v = hb_max (v, freq.get ((unsigned) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
ایستا درونخطی unsigned
safe_get (ثابت hb_vector_t<unsigned> &vec, صحیح x, unsigned min_w, unsigned max_w)
{
  اگر (x < (صحیح) min_w || x > (صحیح) max_w) بازگشت 0;
  بازگشت vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
ایستا تهی
optimize_widths (ثابت hb_vector_t<unsigned> &width_list,
                 unsigned &default_width,
                 unsigned &nominal_width)
{
  اگر (width_list.length == 0)
  {
    default_width = nominal_width = 0;
    بازگشت;
  }

  /* Build frequency map */
  hb_hashmap_t<unsigned, unsigned> widths;
  unsigned min_w = width_list[0];
  unsigned max_w = width_list[0];

  برای (unsigned w : width_list)
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
    بازگشت safe_get (cumFrqU, x, min_w, max_w) +
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
    بازگشت hb_max (u, d);
  };

  /* Find best nominal */
  unsigned best_nominal = min_w;
  unsigned best_cost = nomnCost (min_w) - dfltCost (min_w);

  برای (unsigned x = min_w + 1; x <= max_w; x++)
  {
    unsigned cost = nomnCost (x) - dfltCost (x);
    اگر (cost < best_cost)
    {
      best_cost = cost;
      best_nominal = x;
    }
  }

  /* Work back the best default */
  unsigned best_default = best_nominal;
  unsigned best_default_cost = (unsigned) -1;

  /* Check candidates around best_nominal */
  صحیح candidates[] = {
    (صحیح) best_nominal,
    (صحیح) best_nominal - 108,
    (صحیح) best_nominal - 1132,
    (صحیح) best_nominal + 108,
    (صحیح) best_nominal + 1132
  };

  برای (صحیح candidate : candidates)
  {
    اگر (candidate < (صحیح) min_w || candidate > (صحیح) max_w)
      ادامه;

    /* Compute actual cost with this default */
    unsigned cost = 0;
    برای (auto kv : widths.iter ())
    {
      unsigned w = kv.first;
      unsigned freq = kv.second;

      اگر (w == (unsigned) candidate)
        ادامه;

      cost += freq * width_delta_cost ((صحیح) w - (صحیح) best_nominal);
    }

    اگر (cost < best_default_cost)
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
