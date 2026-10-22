#অন্তর্ভাব "hb-ot-shaper-use-table.hh"

পূর্ণাঙ্ক main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  যদি (argc != 2)
  {
    প্রতি (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    প্রত্যাগম 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  প্রত্যাগম 0;
}
