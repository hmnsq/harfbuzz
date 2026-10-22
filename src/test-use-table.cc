#شامل "hb-ot-shaper-use-table.hh"

صحیح مین (صحیح argc, حرف **argv)
{
  اگر (argc != 2)
  {
    برائے (غیرعلامتی u = 0; u < 0x10FFFFu; u++)
      لکھو ("U+%04X %d\n", u, hb_use_get_category (u));
    واپس 0;
  }

  hb_codepoint_t u;
  لڑی_سے_پڑھو (argv[1], "%x", &u);

  لکھو ("%d\n", hb_use_get_category (u));

  واپس 0;
}
