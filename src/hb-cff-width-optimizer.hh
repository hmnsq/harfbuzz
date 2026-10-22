/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#جيڪڏهن_وصف_نه_ٿيل HB_CFF_ويڪر_OPTIMIZER_HH
#وصف_ڪريو HB_CFF_ويڪر_OPTIMIZER_HH

#ضمن "hb.hh"

نطاق CFF {

/* Calculate byte cost for encoding a width delta */
ساكن مضمن بنانشان
ويڪر_delta_لاڳت (صحيح delta)
{
  delta = مطلق_قدر (delta);
  إذا (delta <= 107) أرجع 1;
  إذا (delta <= 1131) أرجع 2;
  أرجع 5;
}

/* Cumulative sum forward */
ساكن فراغ
cumsum_forward (ثابت hb_hashmap_t<بنانشان, بنانشان> &freq,
                بنانشان min_w, بنانشان max_w,
                hb_ويڪٽر_t<بنانشان> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  بنانشان v = 0;
  لكل (بنانشان x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
ساكن فراغ
cummax_forward (ثابت hb_hashmap_t<بنانشان, بنانشان> &freq,
                بنانشان min_w, بنانشان max_w,
                hb_ويڪٽر_t<بنانشان> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  بنانشان v = 0;
  لكل (بنانشان x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
ساكن فراغ
cumsum_backward (ثابت hb_hashmap_t<بنانشان, بنانشان> &freq,
                 بنانشان min_w, بنانشان max_w,
                 hb_ويڪٽر_t<بنانشان> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  بنانشان v = 0;
  لكل (صحيح x = (صحيح) max_w; x >= (صحيح) min_w; x--)
  {
    v += freq.get ((بنانشان) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
ساكن فراغ
cummax_backward (ثابت hb_hashmap_t<بنانشان, بنانشان> &freq,
                 بنانشان min_w, بنانشان max_w,
                 hb_ويڪٽر_t<بنانشان> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  بنانشان v = 0;
  لكل (صحيح x = (صحيح) max_w; x >= (صحيح) min_w; x--)
  {
    v = hb_max (v, freq.get ((بنانشان) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
ساكن مضمن بنانشان
محفوظ_get (ثابت hb_ويڪٽر_t<بنانشان> &vec, صحيح x, بنانشان min_w, بنانشان max_w)
{
  إذا (x < (صحيح) min_w || x > (صحيح) max_w) أرجع 0;
  أرجع vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
ساكن فراغ
optimize_widths (ثابت hb_ويڪٽر_t<بنانشان> &ويڪر_list,
                 بنانشان &default_ويڪر,
                 بنانشان &nominal_ويڪر)
{
  إذا (ويڪر_list.ڊيگهائي == 0)
  {
    default_ويڪر = nominal_ويڪر = 0;
    أرجع;
  }

  /* Build frequency map */
  hb_hashmap_t<بنانشان, بنانشان> widths;
  بنانشان min_w = ويڪر_list[0];
  بنانشان max_w = ويڪر_list[0];

  لكل (بنانشان w : ويڪر_list)
  {
    widths.سيٽ (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_ويڪٽر_t<بنانشان> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  خودڪار nomn_لاڳت = [&] (بنانشان x) -> بنانشان {
    أرجع محفوظ_get (cumFrqU, x, min_w, max_w) +
           محفوظ_get (cumFrqU, x - 108, min_w, max_w) +
           محفوظ_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           محفوظ_get (cumFrqD, x, min_w, max_w) +
           محفوظ_get (cumFrqD, x + 108, min_w, max_w) +
           محفوظ_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  خودڪار dflt_لاڳت = [&] (بنانشان x) -> بنانشان {
    بنانشان u = hb_max (hb_max (محفوظ_get (cumMaxU, x, min_w, max_w),
                                  محفوظ_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         محفوظ_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    بنانشان d = hb_max (hb_max (محفوظ_get (cumMaxD, x, min_w, max_w),
                                  محفوظ_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         محفوظ_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    أرجع hb_max (u, d);
  };

  /* Find best nominal */
  بنانشان بهترين_nominal = min_w;
  بنانشان بهترين_لاڳت = nomn_لاڳت (min_w) - dflt_لاڳت (min_w);

  لكل (بنانشان x = min_w + 1; x <= max_w; x++)
  {
    بنانشان لاڳت = nomn_لاڳت (x) - dflt_لاڳت (x);
    إذا (لاڳت < بهترين_لاڳت)
    {
      بهترين_لاڳت = لاڳت;
      بهترين_nominal = x;
    }
  }

  /* Work back the best default */
  بنانشان بهترين_default = بهترين_nominal;
  بنانشان بهترين_default_لاڳت = (بنانشان) -1;

  /* Check candidates around best_nominal */
  صحيح candidates[] = {
    (صحيح) بهترين_nominal,
    (صحيح) بهترين_nominal - 108,
    (صحيح) بهترين_nominal - 1132,
    (صحيح) بهترين_nominal + 108,
    (صحيح) بهترين_nominal + 1132
  };

  لكل (صحيح candidate : candidates)
  {
    إذا (candidate < (صحيح) min_w || candidate > (صحيح) max_w)
      استمر;

    /* Compute actual cost with this default */
    بنانشان لاڳت = 0;
    لكل (خودڪار kv : widths.iter ())
    {
      بنانشان w = kv.پهريون;
      بنانشان freq = kv.second;

      إذا (w == (بنانشان) candidate)
        استمر;

      لاڳت += freq * ويڪر_delta_لاڳت ((صحيح) w - (صحيح) بهترين_nominal);
    }

    إذا (لاڳت < بهترين_default_لاڳت)
    {
      بهترين_default_لاڳت = لاڳت;
      بهترين_default = (بنانشان) candidate;
    }
  }

  default_ويڪر = بهترين_default;
  nominal_ويڪر = بهترين_nominal;
}

} /* namespace CFF */

#جيڪڏهن_پورو /* HB_CFF_WIDTH_OPTIMIZER_HH */
