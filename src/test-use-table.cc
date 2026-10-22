#ضمن "hb-ot-shaper-use-table.hh"

صحيح اصلی (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
  {
    لكل (بیعلامت u = 0; u < 0x10FFFFu; u++)
      چاپ_کن ("U+%04X %d\n", u, hb_use_get_category (u));
    أرجع 0;
  }

  hb_codepoint_t u;
  از_رشته_بخوان (argv[1], "%x", &u);

  چاپ_کن ("%d\n", hb_use_get_category (u));

  أرجع 0;
}
