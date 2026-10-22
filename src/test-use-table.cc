#ضمن "hb-ot-shaper-use-table.hh"

صحيح الرئيسية (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
  {
    لكل (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    إرجاع 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  إرجاع 0;
}
