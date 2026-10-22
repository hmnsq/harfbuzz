/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CFF_ਚੌੜਾਈ_OPTIMIZER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CFF_ਚੌੜਾਈ_OPTIMIZER_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

ਨਾਮਾਕਾਸ਼ CFF {

/* Calculate byte cost for encoding a width delta */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
ਚੌੜਾਈ_delta_ਲਾਗਤ (ਪੂਰਨ delta)
{
  delta = ਨਿਰਪੇਖ (delta);
  ਜੇ (delta <= 107) ਵਾਪਸ 1;
  ਜੇ (delta <= 1131) ਵਾਪਸ 2;
  ਵਾਪਸ 5;
}

/* Cumulative sum forward */
ਸਥਿਰ ਖਾਲੀ
cumsum_forward (ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> &freq,
                ਬਿਨਾਨਿਸ਼ਾਨੀ min_w, ਬਿਨਾਨਿਸ਼ਾਨੀ max_w,
                hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  ਬਿਨਾਨਿਸ਼ਾਨੀ v = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
ਸਥਿਰ ਖਾਲੀ
cummax_forward (ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> &freq,
                ਬਿਨਾਨਿਸ਼ਾਨੀ min_w, ਬਿਨਾਨਿਸ਼ਾਨੀ max_w,
                hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  ਬਿਨਾਨਿਸ਼ਾਨੀ v = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
ਸਥਿਰ ਖਾਲੀ
cumsum_backward (ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> &freq,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ min_w, ਬਿਨਾਨਿਸ਼ਾਨੀ max_w,
                 hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  ਬਿਨਾਨਿਸ਼ਾਨੀ v = 0;
  ਲਈ (ਪੂਰਨ x = (ਪੂਰਨ) max_w; x >= (ਪੂਰਨ) min_w; x--)
  {
    v += freq.get ((ਬਿਨਾਨਿਸ਼ਾਨੀ) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
ਸਥਿਰ ਖਾਲੀ
cummax_backward (ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> &freq,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ min_w, ਬਿਨਾਨਿਸ਼ਾਨੀ max_w,
                 hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  ਬਿਨਾਨਿਸ਼ਾਨੀ v = 0;
  ਲਈ (ਪੂਰਨ x = (ਪੂਰਨ) max_w; x >= (ਪੂਰਨ) min_w; x--)
  {
    v = hb_max (v, freq.get ((ਬਿਨਾਨਿਸ਼ਾਨੀ) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
ਸੁਰੱਖਿਅਤ_get (ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &vec, ਪੂਰਨ x, ਬਿਨਾਨਿਸ਼ਾਨੀ min_w, ਬਿਨਾਨਿਸ਼ਾਨੀ max_w)
{
  ਜੇ (x < (ਪੂਰਨ) min_w || x > (ਪੂਰਨ) max_w) ਵਾਪਸ 0;
  ਵਾਪਸ vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
ਸਥਿਰ ਖਾਲੀ
optimize_widths (ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &ਚੌੜਾਈ_list,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ &default_ਚੌੜਾਈ,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ &nominal_ਚੌੜਾਈ)
{
  ਜੇ (ਚੌੜਾਈ_list.ਲੰਮਾਈ == 0)
  {
    default_ਚੌੜਾਈ = nominal_ਚੌੜਾਈ = 0;
    ਵਾਪਸ;
  }

  /* Build frequency map */
  hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> widths;
  ਬਿਨਾਨਿਸ਼ਾਨੀ min_w = ਚੌੜਾਈ_list[0];
  ਬਿਨਾਨਿਸ਼ਾਨੀ max_w = ਚੌੜਾਈ_list[0];

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ w : ਚੌੜਾਈ_list)
  {
    widths.ਸੈੱਟ (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  ਸਵੈਚਾਲਿਤ nomn_ਲਾਗਤ = [&] (ਬਿਨਾਨਿਸ਼ਾਨੀ x) -> ਬਿਨਾਨਿਸ਼ਾਨੀ {
    ਵਾਪਸ ਸੁਰੱਖਿਅਤ_get (cumFrqU, x, min_w, max_w) +
           ਸੁਰੱਖਿਅਤ_get (cumFrqU, x - 108, min_w, max_w) +
           ਸੁਰੱਖਿਅਤ_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           ਸੁਰੱਖਿਅਤ_get (cumFrqD, x, min_w, max_w) +
           ਸੁਰੱਖਿਅਤ_get (cumFrqD, x + 108, min_w, max_w) +
           ਸੁਰੱਖਿਅਤ_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  ਸਵੈਚਾਲਿਤ dflt_ਲਾਗਤ = [&] (ਬਿਨਾਨਿਸ਼ਾਨੀ x) -> ਬਿਨਾਨਿਸ਼ਾਨੀ {
    ਬਿਨਾਨਿਸ਼ਾਨੀ u = hb_max (hb_max (ਸੁਰੱਖਿਅਤ_get (cumMaxU, x, min_w, max_w),
                                  ਸੁਰੱਖਿਅਤ_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         ਸੁਰੱਖਿਅਤ_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    ਬਿਨਾਨਿਸ਼ਾਨੀ d = hb_max (hb_max (ਸੁਰੱਖਿਅਤ_get (cumMaxD, x, min_w, max_w),
                                  ਸੁਰੱਖਿਅਤ_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         ਸੁਰੱਖਿਅਤ_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    ਵਾਪਸ hb_max (u, d);
  };

  /* Find best nominal */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_nominal = min_w;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_ਲਾਗਤ = nomn_ਲਾਗਤ (min_w) - dflt_ਲਾਗਤ (min_w);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = min_w + 1; x <= max_w; x++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ = nomn_ਲਾਗਤ (x) - dflt_ਲਾਗਤ (x);
    ਜੇ (ਲਾਗਤ < ਉੱਤਮ_ਲਾਗਤ)
    {
      ਉੱਤਮ_ਲਾਗਤ = ਲਾਗਤ;
      ਉੱਤਮ_nominal = x;
    }
  }

  /* Work back the best default */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_default = ਉੱਤਮ_nominal;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_default_ਲਾਗਤ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1;

  /* Check candidates around best_nominal */
  ਪੂਰਨ candidates[] = {
    (ਪੂਰਨ) ਉੱਤਮ_nominal,
    (ਪੂਰਨ) ਉੱਤਮ_nominal - 108,
    (ਪੂਰਨ) ਉੱਤਮ_nominal - 1132,
    (ਪੂਰਨ) ਉੱਤਮ_nominal + 108,
    (ਪੂਰਨ) ਉੱਤਮ_nominal + 1132
  };

  ਲਈ (ਪੂਰਨ candidate : candidates)
  {
    ਜੇ (candidate < (ਪੂਰਨ) min_w || candidate > (ਪੂਰਨ) max_w)
      ਜਾਰੀ;

    /* Compute actual cost with this default */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ = 0;
    ਲਈ (ਸਵੈਚਾਲਿਤ kv : widths.iter ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ w = kv.ਪਹਿਲਾ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ freq = kv.second;

      ਜੇ (w == (ਬਿਨਾਨਿਸ਼ਾਨੀ) candidate)
        ਜਾਰੀ;

      ਲਾਗਤ += freq * ਚੌੜਾਈ_delta_ਲਾਗਤ ((ਪੂਰਨ) w - (ਪੂਰਨ) ਉੱਤਮ_nominal);
    }

    ਜੇ (ਲਾਗਤ < ਉੱਤਮ_default_ਲਾਗਤ)
    {
      ਉੱਤਮ_default_ਲਾਗਤ = ਲਾਗਤ;
      ਉੱਤਮ_default = (ਬਿਨਾਨਿਸ਼ਾਨੀ) candidate;
    }
  }

  default_ਚੌੜਾਈ = ਉੱਤਮ_default;
  nominal_ਚੌੜਾਈ = ਉੱਤਮ_nominal;
}

} /* namespace CFF */

#ਜੇ_ਅੰਤ /* HB_CFF_WIDTH_OPTIMIZER_HH */
