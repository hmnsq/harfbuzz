#ضمن "hb-ot-shaper-use-table.hh"

صحيح مکھ (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
  {
    لكل (بنانشانی u = 0; u < 0x10FFFFu; u++)
      لکھو ("U+%04X %d\n", u, hb_use_get_category (u));
    أرجع 0;
  }

  hb_codepoint_t u;
  لڑی_توں_پڑھو (argv[1], "%x", &u);

  لکھو ("%d\n", hb_use_get_category (u));

  أرجع 0;
}
