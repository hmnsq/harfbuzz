#ضمن "hb-ot-shaper-use-table.hh"

صحيح مکيه (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
  {
    لكل (بنانشان u = 0; u < 0x10FFFFu; u++)
      لکو ("U+%04X %d\n", u, hb_use_get_category (u));
    أرجع 0;
  }

  hb_codepoint_t u;
  لڙي_مان_پڙهو (argv[1], "%x", &u);

  لکو ("%d\n", hb_use_get_category (u));

  أرجع 0;
}
