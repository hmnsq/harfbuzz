#অন্তর্ভাব "hb-ot-shaper-use-table.hh"

পূর্ণসংখ্যা প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  যদি (argc != 2)
  {
    জন্য (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ফেরত 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ফেরত 0;
}
