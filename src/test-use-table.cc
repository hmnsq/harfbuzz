#অন্তর্ভাৱ "hb-ot-shaper-use-table.hh"

পূর্ণাঙ্ক main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  যদি (argc != 2)
  {
    বাবে (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ঘূৰাই 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ঘূৰাই 0;
}
