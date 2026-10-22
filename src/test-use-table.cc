#ضمن "hb-ot-shaper-use-table.hh"

صحيح اصلي (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
  {
    لكل (بینښه u = 0; u < 0x10FFFFu; u++)
      چاپ_کړه ("U+%04X %d\n", u, hb_use_get_category (u));
    أرجع 0;
  }

  hb_codepoint_t u;
  له_مزي_ولوله (argv[1], "%x", &u);

  چاپ_کړه ("%d\n", hb_use_get_category (u));

  أرجع 0;
}
