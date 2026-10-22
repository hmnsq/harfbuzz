/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#اگر_تعریف_نشده HB_CFF_عرض_OPTIMIZER_HH
#تعریف_کن HB_CFF_عرض_OPTIMIZER_HH

#ضمن "hb.hh"

نطاق CFF {

/* Calculate byte cost for encoding a width delta */
ساكن مضمن بیعلامت
عرض_delta_هزینه (صحيح delta)
{
  delta = قدر_مطلق (delta);
  إذا (delta <= 107) أرجع 1;
  إذا (delta <= 1131) أرجع 2;
  أرجع 5;
}

/* Cumulative sum forward */
ساكن فراغ
cumsum_forward (ثابت hb_hashmap_t<بیعلامت, بیعلامت> &freq,
                بیعلامت min_w, بیعلامت max_w,
                hb_وکتور_t<بیعلامت> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  بیعلامت v = 0;
  لكل (بیعلامت x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
ساكن فراغ
cummax_forward (ثابت hb_hashmap_t<بیعلامت, بیعلامت> &freq,
                بیعلامت min_w, بیعلامت max_w,
                hb_وکتور_t<بیعلامت> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  بیعلامت v = 0;
  لكل (بیعلامت x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
ساكن فراغ
cumsum_backward (ثابت hb_hashmap_t<بیعلامت, بیعلامت> &freq,
                 بیعلامت min_w, بیعلامت max_w,
                 hb_وکتور_t<بیعلامت> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  بیعلامت v = 0;
  لكل (صحيح x = (صحيح) max_w; x >= (صحيح) min_w; x--)
  {
    v += freq.get ((بیعلامت) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
ساكن فراغ
cummax_backward (ثابت hb_hashmap_t<بیعلامت, بیعلامت> &freq,
                 بیعلامت min_w, بیعلامت max_w,
                 hb_وکتور_t<بیعلامت> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  بیعلامت v = 0;
  لكل (صحيح x = (صحيح) max_w; x >= (صحيح) min_w; x--)
  {
    v = hb_max (v, freq.get ((بیعلامت) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
ساكن مضمن بیعلامت
امن_get (ثابت hb_وکتور_t<بیعلامت> &vec, صحيح x, بیعلامت min_w, بیعلامت max_w)
{
  إذا (x < (صحيح) min_w || x > (صحيح) max_w) أرجع 0;
  أرجع vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
ساكن فراغ
optimize_widths (ثابت hb_وکتور_t<بیعلامت> &عرض_list,
                 بیعلامت &default_عرض,
                 بیعلامت &nominal_عرض)
{
  إذا (عرض_list.درازی == 0)
  {
    default_عرض = nominal_عرض = 0;
    أرجع;
  }

  /* Build frequency map */
  hb_hashmap_t<بیعلامت, بیعلامت> widths;
  بیعلامت min_w = عرض_list[0];
  بیعلامت max_w = عرض_list[0];

  لكل (بیعلامت w : عرض_list)
  {
    widths.مجموعه (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_وکتور_t<بیعلامت> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  خودکار nomn_هزینه = [&] (بیعلامت x) -> بیعلامت {
    أرجع امن_get (cumFrqU, x, min_w, max_w) +
           امن_get (cumFrqU, x - 108, min_w, max_w) +
           امن_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           امن_get (cumFrqD, x, min_w, max_w) +
           امن_get (cumFrqD, x + 108, min_w, max_w) +
           امن_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  خودکار dflt_هزینه = [&] (بیعلامت x) -> بیعلامت {
    بیعلامت u = hb_max (hb_max (امن_get (cumMaxU, x, min_w, max_w),
                                  امن_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         امن_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    بیعلامت d = hb_max (hb_max (امن_get (cumMaxD, x, min_w, max_w),
                                  امن_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         امن_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    أرجع hb_max (u, d);
  };

  /* Find best nominal */
  بیعلامت بهترین_nominal = min_w;
  بیعلامت بهترین_هزینه = nomn_هزینه (min_w) - dflt_هزینه (min_w);

  لكل (بیعلامت x = min_w + 1; x <= max_w; x++)
  {
    بیعلامت هزینه = nomn_هزینه (x) - dflt_هزینه (x);
    إذا (هزینه < بهترین_هزینه)
    {
      بهترین_هزینه = هزینه;
      بهترین_nominal = x;
    }
  }

  /* Work back the best default */
  بیعلامت بهترین_default = بهترین_nominal;
  بیعلامت بهترین_default_هزینه = (بیعلامت) -1;

  /* Check candidates around best_nominal */
  صحيح candidates[] = {
    (صحيح) بهترین_nominal,
    (صحيح) بهترین_nominal - 108,
    (صحيح) بهترین_nominal - 1132,
    (صحيح) بهترین_nominal + 108,
    (صحيح) بهترین_nominal + 1132
  };

  لكل (صحيح candidate : candidates)
  {
    إذا (candidate < (صحيح) min_w || candidate > (صحيح) max_w)
      استمر;

    /* Compute actual cost with this default */
    بیعلامت هزینه = 0;
    لكل (خودکار kv : widths.iter ())
    {
      بیعلامت w = kv.اولی;
      بیعلامت freq = kv.second;

      إذا (w == (بیعلامت) candidate)
        استمر;

      هزینه += freq * عرض_delta_هزینه ((صحيح) w - (صحيح) بهترین_nominal);
    }

    إذا (هزینه < بهترین_default_هزینه)
    {
      بهترین_default_هزینه = هزینه;
      بهترین_default = (بیعلامت) candidate;
    }
  }

  default_عرض = بهترین_default;
  nominal_عرض = بهترین_nominal;
}

} /* namespace CFF */

#پایان_اگر /* HB_CFF_WIDTH_OPTIMIZER_HH */
