#شامل "hb-ot-shaper-use-table.hh"

صحیح اصلی (صحیح argc, نویسه **argv)
{
  اگر (argc != 2)
  {
    برای (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    بازگشت 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  بازگشت 0;
}
