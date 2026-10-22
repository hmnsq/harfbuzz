/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#اگر_غیر_تعریف_شدہ HB_CFF_چوڑائی_OPTIMIZER_HH
#تعریف_کرو HB_CFF_چوڑائی_OPTIMIZER_HH

#شامل "hb.hh"

نامخلا CFF {

/* Calculate byte cost for encoding a width delta */
ساکن اندرونی غیرعلامتی
چوڑائی_delta_لاگت (صحیح delta)
{
  delta = مطلق_قدر (delta);
  اگر (delta <= 107) واپس 1;
  اگر (delta <= 1131) واپس 2;
  واپس 5;
}

/* Cumulative sum forward */
ساکن خالی
cumsum_forward (ثابت hb_hashmap_t<غیرعلامتی, غیرعلامتی> &freq,
                غیرعلامتی min_w, غیرعلامتی max_w,
                hb_سمتیہ_t<غیرعلامتی> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  غیرعلامتی v = 0;
  برائے (غیرعلامتی x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
ساکن خالی
cummax_forward (ثابت hb_hashmap_t<غیرعلامتی, غیرعلامتی> &freq,
                غیرعلامتی min_w, غیرعلامتی max_w,
                hb_سمتیہ_t<غیرعلامتی> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  غیرعلامتی v = 0;
  برائے (غیرعلامتی x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
ساکن خالی
cumsum_backward (ثابت hb_hashmap_t<غیرعلامتی, غیرعلامتی> &freq,
                 غیرعلامتی min_w, غیرعلامتی max_w,
                 hb_سمتیہ_t<غیرعلامتی> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  غیرعلامتی v = 0;
  برائے (صحیح x = (صحیح) max_w; x >= (صحیح) min_w; x--)
  {
    v += freq.get ((غیرعلامتی) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
ساکن خالی
cummax_backward (ثابت hb_hashmap_t<غیرعلامتی, غیرعلامتی> &freq,
                 غیرعلامتی min_w, غیرعلامتی max_w,
                 hb_سمتیہ_t<غیرعلامتی> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  غیرعلامتی v = 0;
  برائے (صحیح x = (صحیح) max_w; x >= (صحیح) min_w; x--)
  {
    v = hb_max (v, freq.get ((غیرعلامتی) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
ساکن اندرونی غیرعلامتی
محفوظ_get (ثابت hb_سمتیہ_t<غیرعلامتی> &vec, صحیح x, غیرعلامتی min_w, غیرعلامتی max_w)
{
  اگر (x < (صحیح) min_w || x > (صحیح) max_w) واپس 0;
  واپس vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
ساکن خالی
optimize_widths (ثابت hb_سمتیہ_t<غیرعلامتی> &چوڑائی_list,
                 غیرعلامتی &default_چوڑائی,
                 غیرعلامتی &nominal_چوڑائی)
{
  اگر (چوڑائی_list.لمبائی == 0)
  {
    default_چوڑائی = nominal_چوڑائی = 0;
    واپس;
  }

  /* Build frequency map */
  hb_hashmap_t<غیرعلامتی, غیرعلامتی> widths;
  غیرعلامتی min_w = چوڑائی_list[0];
  غیرعلامتی max_w = چوڑائی_list[0];

  برائے (غیرعلامتی w : چوڑائی_list)
  {
    widths.سیٹ (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_سمتیہ_t<غیرعلامتی> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  خودکار nomn_لاگت = [&] (غیرعلامتی x) -> غیرعلامتی {
    واپس محفوظ_get (cumFrqU, x, min_w, max_w) +
           محفوظ_get (cumFrqU, x - 108, min_w, max_w) +
           محفوظ_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           محفوظ_get (cumFrqD, x, min_w, max_w) +
           محفوظ_get (cumFrqD, x + 108, min_w, max_w) +
           محفوظ_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  خودکار dflt_لاگت = [&] (غیرعلامتی x) -> غیرعلامتی {
    غیرعلامتی u = hb_max (hb_max (محفوظ_get (cumMaxU, x, min_w, max_w),
                                  محفوظ_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         محفوظ_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    غیرعلامتی d = hb_max (hb_max (محفوظ_get (cumMaxD, x, min_w, max_w),
                                  محفوظ_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         محفوظ_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    واپس hb_max (u, d);
  };

  /* Find best nominal */
  غیرعلامتی بہترین_nominal = min_w;
  غیرعلامتی بہترین_لاگت = nomn_لاگت (min_w) - dflt_لاگت (min_w);

  برائے (غیرعلامتی x = min_w + 1; x <= max_w; x++)
  {
    غیرعلامتی لاگت = nomn_لاگت (x) - dflt_لاگت (x);
    اگر (لاگت < بہترین_لاگت)
    {
      بہترین_لاگت = لاگت;
      بہترین_nominal = x;
    }
  }

  /* Work back the best default */
  غیرعلامتی بہترین_default = بہترین_nominal;
  غیرعلامتی بہترین_default_لاگت = (غیرعلامتی) -1;

  /* Check candidates around best_nominal */
  صحیح candidates[] = {
    (صحیح) بہترین_nominal,
    (صحیح) بہترین_nominal - 108,
    (صحیح) بہترین_nominal - 1132,
    (صحیح) بہترین_nominal + 108,
    (صحیح) بہترین_nominal + 1132
  };

  برائے (صحیح candidate : candidates)
  {
    اگر (candidate < (صحیح) min_w || candidate > (صحیح) max_w)
      جاری;

    /* Compute actual cost with this default */
    غیرعلامتی لاگت = 0;
    برائے (خودکار kv : widths.iter ())
    {
      غیرعلامتی w = kv.پہلا;
      غیرعلامتی freq = kv.second;

      اگر (w == (غیرعلامتی) candidate)
        جاری;

      لاگت += freq * چوڑائی_delta_لاگت ((صحیح) w - (صحیح) بہترین_nominal);
    }

    اگر (لاگت < بہترین_default_لاگت)
    {
      بہترین_default_لاگت = لاگت;
      بہترین_default = (غیرعلامتی) candidate;
    }
  }

  default_چوڑائی = بہترین_default;
  nominal_چوڑائی = بہترین_nominal;
}

} /* namespace CFF */

#اگر_ختم /* HB_CFF_WIDTH_OPTIMIZER_HH */
