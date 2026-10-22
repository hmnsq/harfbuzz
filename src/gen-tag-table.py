#!/usr/bin/env python3

"""Generator of the mapping from OpenType tags to BCP 47 tags and vice
versa.

It creates a ``const LangTag[]``, matching the tags from the OpenType
languages system tag list to the language subtags of the BCP 47 language
subtag registry, with some manual adjustments. The mappings are
supplemented with macrolanguages' sublanguages and retired codes'
replacements, according to BCP 47 and some manual additions where BCP 47
omits a retired code entirely.

Also generated is a function, ``hb_ot_ambiguous_tag_to_language``,
intended for use by ``hb_ot_tag_to_language``. It maps OpenType tags
back to BCP 47 tags. Ambiguous OpenType tags (those that correspond to
multiple BCP 47 tags) are listed here, except when the alphabetically
first BCP 47 tag happens to be the chosen disambiguated tag. In that
case, the fallback behavior will choose the right tag anyway.

usage: ./gen-tag-table.py [--rust] languagetags language-subtag-registry

Input files:
* https://docs.microsoft.com/en-us/typography/opentype/spec/languagetags
* https://www.iana.org/assignments/language-subtag-registry/language-subtag-registry
"""

استورد collections
استورد html
من html.parser استورد HTMLParser
استورد itertools
استورد os
استورد re
استورد sys
استورد unicodedata

إذا اوږدوالی (sys.argv) > 1 و sys.argv[1] == '--rust':
	لرېکړه sys.argv[1]
	output_rust = ریښتیا
وإلا:
	output_rust = خطأ

إذا اوږدوالی (sys.argv) != 3:
	sys.exit (__doc__)

تعریف expect (condition, پیغام=هیڅ):
	إذا ليس condition:
		إذا پیغام دی هیڅ:
			ارفع AssertionError
		ارفع AssertionError (پیغام)

تعریف اكتب (s):
	sys.stdout.flush ()
	sys.stdout.بفر.اكتب (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = سېټ (" -—–.,;:’'ʼ()[]/!=>_+")

تعریف sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (s, str):
		ارفع TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	إذا ليس s:
		أرجع ''
	لكل ch في s:
		cat = unicodedata.category (ch)
		إذا ليس (cat.startswith (('L', 'M', 'N', 'Z')) أو ch في _ALLOWED_COMMENT_PUNCT):
			ارفع ارزښت_Error ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	أرجع s

تعریف sanitize_سرلیک (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (s, str):
		ارفع TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	إذا ليس re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		ارفع ارزښت_Error ("Invalid header line %r: contains disallowed characters" % s)
	أرجع s

تعریف sanitize_bcp47_نښان (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (s, str):
		ارفع TypeError ("Expected string, got %s" % type (s).__name__)
	إذا ليس re.fullmatch (r'[a-zA-Z0-9-]+', s):
		ارفع ارزښت_Error ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	أرجع s

تعریف sanitize_نښان (نښان_2):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (نښان_2, str):
		ارفع TypeError ("Expected string for tag, got %s" % type (نښان_2).__name__)
	إذا نښان_2 == DEFAULT_LANGUAGE_SYSTEM:
		أرجع ''
	إذا ليس re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', نښان_2):
		ارفع ارزښت_Error ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % نښان_2)
	أرجع نښان_2

DEFAULT_LANGUAGE_SYSTEM = ''

# from https://www-01.sil.org/iso639-3/iso-639-3.tab
ISO_639_3_منزل_1 = {
	'aar': 'aa',
	'abk': 'ab',
	'afr': 'af',
	'aka': 'ak',
	'amh': 'am',
	'ara': 'ar',
	'arg': 'an',
	'asm': 'as',
	'ava': 'av',
	'ave': 'ae',
	'aym': 'ay',
	'aze': 'az',
	'bak': 'ba',
	'bam': 'bm',
	'bel': 'be',
	'ben': 'bn',
	'bis': 'bi',
	'bod': 'bo',
	'bos': 'bs',
	'bre': 'br',
	'bul': 'bg',
	'cat': 'ca',
	'ces': 'cs',
	'cha': 'ch',
	'che': 'ce',
	'chu': 'cu',
	'chv': 'cv',
	'cor': 'kw',
	'cos': 'co',
	'cre': 'cr',
	'cym': 'cy',
	'dan': 'da',
	'deu': 'de',
	'div': 'dv',
	'dzo': 'dz',
	'ell': 'el',
	'eng': 'en',
	'epo': 'eo',
	'est': 'et',
	'eus': 'eu',
	'ewe': 'ee',
	'fao': 'fo',
	'fas': 'fa',
	'fij': 'fj',
	'fin': 'fi',
	'fra': 'fr',
	'fry': 'fy',
	'ful': 'ff',
	'gla': 'gd',
	'gle': 'ga',
	'glg': 'gl',
	'glv': 'gv',
	'grn': 'gn',
	'guj': 'gu',
	'hat': 'ht',
	'hau': 'ha',
	'hbs': 'sh',
	'heb': 'he',
	'her': 'hz',
	'hin': 'hi',
	'hmo': 'ho',
	'hrv': 'hr',
	'hun': 'hu',
	'hye': 'hy',
	'ibo': 'ig',
	'ido': 'io',
	'iii': 'ii',
	'iku': 'iu',
	'ile': 'ie',
	'ina': 'ia',
	'ind': 'id',
	'ipk': 'ik',
	'isl': 'is',
	'ita': 'it',
	'jav': 'jv',
	'jpn': 'ja',
	'kal': 'kl',
	'kan': 'kn',
	'kas': 'ks',
	'kat': 'ka',
	'kau': 'kr',
	'kaz': 'kk',
	'khm': 'km',
	'kik': 'ki',
	'kin': 'rw',
	'kir': 'ky',
	'kom': 'kv',
	'kon': 'kg',
	'kor': 'ko',
	'kua': 'kj',
	'kur': 'ku',
	'lao': 'lo',
	'lat': 'la',
	'lav': 'lv',
	'lim': 'li',
	'lin': 'ln',
	'lit': 'lt',
	'ltz': 'lb',
	'lub': 'lu',
	'lug': 'lg',
	'mah': 'mh',
	'mal': 'ml',
	'mar': 'mr',
	'mkd': 'mk',
	'mlg': 'mg',
	'mlt': 'mt',
	'mol': 'mo',
	'mon': 'mn',
	'mri': 'mi',
	'msa': 'ms',
	'mya': 'my',
	'nau': 'na',
	'nav': 'nv',
	'nbl': 'nr',
	'nde': 'nd',
	'ndo': 'ng',
	'nep': 'ne',
	'nld': 'nl',
	'nno': 'nn',
	'nob': 'nb',
	'nor': 'no',
	'nya': 'ny',
	'oci': 'oc',
	'oji': 'oj',
	'ori': 'or',
	'orm': 'om',
	'oss': 'os',
	'pan': 'pa',
	'pli': 'pi',
	'pol': 'pl',
	'por': 'pt',
	'pus': 'ps',
	'que': 'qu',
	'roh': 'rm',
	'ron': 'ro',
	'run': 'rn',
	'rus': 'ru',
	'sag': 'sg',
	'san': 'sa',
	'sin': 'si',
	'slk': 'sk',
	'slv': 'sl',
	'sme': 'se',
	'smo': 'sm',
	'sna': 'sn',
	'snd': 'sd',
	'som': 'so',
	'sot': 'st',
	'spa': 'es',
	'sqi': 'sq',
	'srd': 'sc',
	'srp': 'sr',
	'ssw': 'ss',
	'sun': 'su',
	'swa': 'sw',
	'swe': 'sv',
	'tah': 'ty',
	'tam': 'ta',
	'tat': 'tt',
	'tel': 'te',
	'tgk': 'tg',
	'tgl': 'tl',
	'tha': 'th',
	'tir': 'ti',
	'ton': 'to',
	'tsn': 'tn',
	'tso': 'ts',
	'tuk': 'tk',
	'tur': 'tr',
	'twi': 'tw',
	'uig': 'ug',
	'ukr': 'uk',
	'urd': 'ur',
	'uzb': 'uz',
	'ven': 've',
	'vie': 'vi',
	'vol': 'vo',
	'wln': 'wa',
	'wol': 'wo',
	'xho': 'xh',
	'yid': 'yi',
	'yor': 'yo',
	'zha': 'za',
	'zho': 'zh',
	'zul': 'zu',
}

صنف Language_نښان (object):
	"""A BCP 47 language tag.

	Attributes:
		subtags (List[str]): The list of subtags in this tag.
		grandfathered (bool): Whether this tag is grandfathered. If
			``true``, the entire lowercased tag is the ``language``
			and the other subtag fields are empty.
		language (str): The language subtag.
		script (str): The script subtag.
		region (str): The region subtag.
		variant (str): The variant subtag.

	Args:
		tag (str): A BCP 47 language tag.

	"""
	تعریف __init__ (self, نښان_2):
		عام bcp_47
		self.subtags = نښان_2.ښکتنی ().وېشل ('-')
		self.grandfathered = نښان_2.ښکتنی () في bcp_47.grandfathered
		إذا self.grandfathered:
			self.language = نښان_2.ښکتنی ()
			self.script = ''
			self.region = ''
			self.variant = ''
		وإلا:
			self.language = self.subtags[0]
			self.script = self.پیدا_کړه_لومړی (مجهول s: اوږدوالی (s) == 4 و s[0] > '9', self.subtags)
			self.region = self.پیدا_کړه_لومړی (مجهول s: اوږدوالی (s) == 2 و s[0] > '9' أو اوږدوالی (s) == 3 و s[0] <= '9', self.subtags[1:])
			self.variant = self.پیدا_کړه_لومړی (مجهول s: اوږدوالی (s) > 4 أو اوږدوالی (s) == 4 و s[0] <= '9', self.subtags)

	تعریف __str__(self):
		أرجع '-'.join(self.subtags)

	تعریف __repr__ (self):
		أرجع 'LanguageTag(%r)' % str(self)

	@staticmethod
	تعریف پیدا_کړه_لومړی (function, sequence):
		حاول:
			أرجع بل (iter (filter (function, sequence)))
		استثناء StopIteration:
			أرجع هیڅ

	تعریف is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		أرجع ليس (اوږدوالی (self.subtags) == 1
			أو self.grandfathered
			و اوږدوالی (self.subtags[1]) != 3
			و ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	تعریف get_ډله (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		أرجع ('und'
			إذا (self.language == 'und'
				أو self.variant في bcp_47.prefixes و اوږدوالی (bcp_47.prefixes[self.variant]) == 1)
			وإلا self.language[0])

صنف OpenTypeRegistryParser (HTMLParser):
	"""A parser for the OpenType language system tag registry.

	Attributes:
		header (str): The "last updated" line of the registry.
		names (Mapping[str, str]): A map of language system tags to the
			names they are given in the registry.
		ranks (DefaultDict[str, int]): A map of language system tags to
			numbers. If a single BCP 47 tag corresponds to multiple
			OpenType tags, the tags are ordered in increasing order by
			rank. The rank is based on the number of BCP 47 tags
			associated with a tag, though it may be manually modified.
		to_bcp_47 (DefaultDict[str, AbstractSet[str]]): A map of
			OpenType language system tags to sets of BCP 47 tags.
		from_bcp_47 (DefaultDict[str, AbstractSet[str]]): ``to_bcp_47``
			inverted. Its values start as unsorted sets;
			``sort_languages`` converts them to sorted lists.
		from_bcp_47_uninherited (Optional[Dict[str, AbstractSet[str]]]):
			A copy of ``from_bcp_47``. It starts as ``None`` and is
			populated at the beginning of the first call to
			``inherit_from_macrolanguages``.

	"""
	تعریف __init__ (self):
		HTMLParser.__init__ (self)
		self.سرلیک = ''
		self.نومونه = {}
		self.ranks = collections.defaultdict (صحيح)
		self.منزل_bcp_47 = collections.defaultdict (سېټ)
		self.from_bcp_47 = collections.defaultdict (سېټ)
		self.from_bcp_47_uninherited = هیڅ
		# Whether the parser is in a <td> element
		self._td = خطأ
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = خطأ
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	تعریف handle_starttag (self, نښان_2, attrs):
		إذا نښان_2 == 'a':
			إذا self._current_tr و ليس self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = ریښتیا
		وإلاإذا نښان_2 == 'br':
			self._disengaged = ریښتیا
		وإلاإذا نښان_2 == 'meta':
			لكل attr, ارزښت_2 في attrs:
				إذا attr == 'name' و ارزښت_2 == 'updated_at':
					self.سرلیک = self.get_starttag_متن ()
					اكسر
		وإلاإذا نښان_2 == 'td':
			self._td = ریښتیا
			self._current_tr.append ('')
		وإلاإذا نښان_2 == 'tr':
			self._disengaged = خطأ
			self._current_tr = []

	تعریف handle_endtag (self, نښان_2):
		إذا نښان_2 == 'td':
			self._td = خطأ
			self._disengaged = خطأ
		وإلاإذا نښان_2 == 'tr' و self._current_tr:
			expect (2 <= اوږدوالی (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			نښان_2 = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			إذا اوږدوالی (نښان_2) > 4:
				expect (نښان_2.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % نښان_2)
				name += ' (deprecated)'
				نښان_2 = نښان_2.وېشل (' ')[0]
				rank = 1
			self.نومونه[نښان_2] = re.sub (' languages$', '', name)
			إذا ليس self._current_tr[2]:
				أرجع
			iso_codes = self._current_tr[2].strip ()
			self.منزل_bcp_47[نښان_2].update (ISO_639_3_منزل_1.get (کوډ, کوډ) لكل کوډ في iso_codes.replace (' ', '').وېشل (','))
			rank += 2 * اوږدوالی (self.منزل_bcp_47[نښان_2])
			self.ranks[نښان_2] = rank

	تعریف handle_معلومات (self, معلومات):
		إذا self._td و ليس self._disengaged:
			self._current_tr[-1] += معلومات

	تعریف handle_charref (self, name):
		self.handle_معلومات (html.unescape ('&#%s;' % name))

	تعریف handle_entityref (self, name):
		self.handle_معلومات (html.unescape ('&%s;' % name))

	تعریف parse (self, filename):
		"""Parse the OpenType language system tag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		سره open (filename, encoding='utf-8') كـ f:
			self.feed (f.اقرأ ())
		expect (self.سرلیک)
		لكل نښان_2, iso_codes في self.منزل_bcp_47.توکي ():
			لكل iso_کوډ في iso_codes:
				self.from_bcp_47[iso_کوډ].جمع_کړه (نښان_2)

	تعریف جمع_کړه_language (self, bcp_47_نښان, ot_نښان):
		"""Add a language as if it were in the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag. If the tag is more than just
				a language subtag, and if the language subtag is a
				macrolanguage, then new languages are added corresponding
				to the macrolanguages' individual languages with the
				remainder of the tag appended.
			ot_tag (str): An OpenType language system tag.
		"""
		عام bcp_47
		self.منزل_bcp_47[ot_نښان].جمع_کړه (bcp_47_نښان)
		self.from_bcp_47[bcp_47_نښان].جمع_کړه (ot_نښان)
		إذا bcp_47_نښان.ښکتنی () ليس في bcp_47.grandfathered:
			حاول:
				[macrolanguage, suffix] = bcp_47_نښان.وېشل ('-', 1)
				إذا macrolanguage في bcp_47.macrolanguages:
					s = سېټ ()
					لكل language في bcp_47.macrolanguages[macrolanguage]:
						إذا language.ښکتنی () ليس في bcp_47.grandfathered:
							s.جمع_کړه ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			استثناء ارزښت_Error:
				تیرشه

	@staticmethod
	تعریف _remove_language (نښان_1, dict_1, dict_2):
		لكل نښان_2_2 في dict_1.راوباسه (نښان_1):
			dict_2[نښان_2_2].remove (نښان_1)
			إذا ليس dict_2[نښان_2_2]:
				لرېکړه dict_2[نښان_2_2]

	تعریف remove_language_ot (self, ot_نښان):
		"""Remove an OpenType tag from the registry.

		Args:
			ot_tag (str): An OpenType tag.
		"""
		self._remove_language (ot_نښان, self.منزل_bcp_47, self.from_bcp_47)

	تعریف remove_language_bcp_47 (self, bcp_47_نښان):
		"""Remove a BCP 47 tag from the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag.
		"""
		self._remove_language (bcp_47_نښان, self.from_bcp_47, self.منزل_bcp_47)

	تعریف inherit_from_macrolanguages (self):
		"""Copy mappings from macrolanguages to individual languages.

		If a BCP 47 tag for an individual mapping has no OpenType
		mapping but its macrolanguage does, the mapping is copied to
		the individual language. For example, als (Tosk Albanian) has no
		explicit mapping, so it inherits from sq (Albanian) the mapping
		to SQI.

		However, if an OpenType tag maps to a BCP 47 macrolanguage and
		some but not all of its individual languages, the mapping is not
		inherited from the macrolanguage to the missing individual
		languages. For example, INUK (Nunavik Inuktitut) is mapped to
		ike (Eastern Canadian Inuktitut) and iu (Inuktitut) but not to
		ikt (Inuinnaqtun, which is an individual language of iu), so
		this method does not add a mapping from ikt to INUK.

		If a BCP 47 tag for a macrolanguage has no OpenType mapping but
		some of its individual languages do, their mappings are copied
		to the macrolanguage.
		"""
		عام bcp_47
		لومړی_وخت = self.from_bcp_47_uninherited دی هیڅ
		إذا لومړی_وخت:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		لكل macrolanguage, languages في dict (bcp_47.macrolanguages).توکي ():
			ot_macrolanguages = {
				ot_macrolanguage لكل ot_macrolanguage في self.from_bcp_47_uninherited.get (macrolanguage, سېټ ())
			}
			blocked_ot_macrolanguages = سېټ ()
			إذا 'retired code' ليس في bcp_47.scopes.get (macrolanguage, ''):
				لكل ot_macrolanguage في ot_macrolanguages:
					پړاو_trip_macrolanguages = {
						l لكل l في self.منزل_bcp_47[ot_macrolanguage]
						إذا 'retired code' ليس في bcp_47.scopes.get (l, '')
					}
					پړاو_trip_languages = {
						l لكل l في languages
						إذا 'retired code' ليس في bcp_47.scopes.get (l, '')
					}
					intersection = پړاو_trip_macrolanguages & پړاو_trip_languages
					إذا intersection و intersection != پړاو_trip_languages:
						blocked_ot_macrolanguages.جمع_کړه (ot_macrolanguage)
			إذا ot_macrolanguages:
				لكل ot_macrolanguage في ot_macrolanguages:
					إذا ot_macrolanguage ليس في blocked_ot_macrolanguages:
						لكل language في languages:
							self.جمع_کړه_language (language, ot_macrolanguage)
							إذا ليس blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			وإلاإذا لومړی_وخت:
				لكل language في languages:
					إذا language في self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					وإلا:
						ot_macrolanguages.clear ()
					إذا ليس ot_macrolanguages:
						اكسر
				لكل ot_macrolanguage في ot_macrolanguages:
					self.جمع_کړه_language (macrolanguage, ot_macrolanguage)

	تعریف ترتیبول_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		لكل language, نښانونه في self.from_bcp_47.توکي ():
			self.from_bcp_47[language] = sorted (نښانونه,
					کلی_2=مجهول t: (self.ranks[t] + rank_delta (language, t), t))

ot = OpenTypeRegistryParser ()

صنف BCP47Parser (object):
	"""A parser for the BCP 47 subtag registry.

	Attributes:
		header (str): The "File-Date" line of the registry.
		names (Mapping[str, str]): A map of subtags to the names they
			are given in the registry. Each value is a
			``'\\n'``-separated list of names.
		scopes (Mapping[str, str]): A map of language subtags to strings
			suffixed to language names, including suffixes to explain
			language scopes.
		macrolanguages (DefaultDict[str, AbstractSet[str]]): A map of
			language subtags to the sets of language subtags which
			inherit from them. See
			``OpenTypeRegistryParser.inherit_from_macrolanguages``.
		prefixes (DefaultDict[str, AbstractSet[str]]): A map of variant
			subtags to their prefixes.
		grandfathered (AbstractSet[str]): The set of grandfathered tags,
			normalized to lowercase.

	"""
	تعریف __init__ (self):
		self.سرلیک = ''
		self.نومونه = {}
		self.scopes = {}
		self.macrolanguages = collections.defaultdict (سېټ)
		self.prefixes = collections.defaultdict (سېټ)
		self.grandfathered = سېټ ()

	تعریف parse (self, filename):
		"""Parse the BCP 47 subtag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		سره open (filename, encoding='utf-8') كـ f:
			subtag_type = هیڅ
			subtag = هیڅ
			deprecated = خطأ
			has_preferred_ارزښت = خطأ
			کرښه_بفر = ''
			لكل کرښه في itertools.chain (f, ['']):
				کرښه = کرښه.rstrip ()
				إذا کرښه.startswith (' '):
					کرښه_بفر += کرښه[1:]
					استمر
				کرښه, کرښه_بفر = کرښه_بفر, کرښه
				إذا کرښه.startswith ('Type: '):
					subtag_type = کرښه.وېشل (' ')[1]
					deprecated = خطأ
					has_preferred_ارزښت = خطأ
				وإلاإذا کرښه.startswith ('Subtag: ') أو کرښه.startswith ('Tag: '):
					subtag = کرښه.وېشل (' ')[1]
					إذا subtag_type == 'grandfathered':
						self.grandfathered.جمع_کړه (subtag.ښکتنی ())
				وإلاإذا کرښه.startswith ('Description: '):
					description = کرښه.وېشل (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					إذا subtag في self.نومونه:
						self.نومونه[subtag] += '\n' + description
					وإلا:
						self.نومونه[subtag] = description
				وإلاإذا subtag_type == 'language' أو subtag_type == 'grandfathered':
					إذا کرښه.startswith ('Scope: '):
						scope = کرښه.وېشل (' ')[1]
						إذا scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						وإلاإذا scope == 'collection':
							scope = ' [collection]'
						وإلا:
							استمر
						self.scopes[subtag] = scope
					وإلاإذا کرښه.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = ریښتیا
					وإلاإذا deprecated و کرښه.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						لكل language في کرښه.replace (',', '').وېشل (' ')[2:]:
							self.جمع_کړه_macrolanguage (subtag, language)
					وإلاإذا کرښه.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_ارزښت = ریښتیا
						macrolanguage = کرښه.وېشل (' ')[1]
						self.جمع_کړه_macrolanguage (macrolanguage, subtag)
					وإلاإذا ليس has_preferred_ارزښت و کرښه.startswith ('Macrolanguage: '):
						self.جمع_کړه_macrolanguage (کرښه.وېشل (' ')[1], subtag)
				وإلاإذا subtag_type == 'variant':
					إذا کرښه.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					وإلاإذا کرښه.startswith ('Prefix: '):
						self.prefixes[subtag].جمع_کړه (کرښه.وېشل (' ')[1])
				وإلاإذا کرښه.startswith ('File-Date: '):
					self.سرلیک = کرښه
		expect (self.سرلیک)

	تعریف جمع_کړه_macrolanguage (self, macrolanguage, language):
		عام ot
		إذا language ليس في ot.from_bcp_47:
			لكل l في self.macrolanguages.get (language, سېټ ()):
				self.جمع_کړه_macrolanguage (macrolanguage, l)
		إذا macrolanguage ليس في ot.from_bcp_47:
			لكل ls في list (self.macrolanguages.ارزښتونه ()):
				إذا macrolanguage في ls:
					ls.جمع_کړه (language)
					أرجع
		self.macrolanguages[macrolanguage].جمع_کړه (language)

	تعریف remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		لكل macrolanguage, languages في self.macrolanguages.توکي ():
			لكل language في languages:
				inverted[language].append (macrolanguage)
		لكل language, macrolanguages في inverted.توکي ():
			إذا اوږدوالی (macrolanguages) > 1:
				macrolanguages.ترتیبول (کلی_2=مجهول ml: اوږدوالی (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.راوباسه ()
				لكل macrolanguage في macrolanguages:
					self.جمع_کړه_macrolanguage (biggest_macrolanguage, macrolanguage)

	تعریف _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		أرجع self.نومونه[subtag].وېشل ('\n')[0] + self.scopes.get (subtag, '')

	تعریف get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		إذا lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		إذا lt.region:
			name += '; ' + self._get_name_piece (lt.region.پاسنی ())
		إذا lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		أرجع name

bcp_47 = BCP47Parser ()

ot.parse (sys.argv[1])
bcp_47.parse (sys.argv[2])

ot.جمع_کړه_language ('ary', 'MOR')

ot.جمع_کړه_language ('ath', 'ATH')

ot.جمع_کړه_language ('bai', 'BML')

ot.ranks['BAL'] = ot.ranks['KAR'] + 1

ot.جمع_کړه_language ('ber', 'BBR')

ot.remove_language_ot ('PGR')
ot.جمع_کړه_language ('el-polyton', 'PGR')

bcp_47.نومونه['flm'] = 'Falam Chin'
bcp_47.scopes['flm'] = ' (retired code)'
bcp_47.macrolanguages['flm'] = {'cfm'}

ot.ranks['FNE'] = ot.ranks['TNE'] + 1

ot.جمع_کړه_language ('und-fonipa', 'IPPH')

ot.جمع_کړه_language ('und-fonnapa', 'APPH')

ot.جمع_کړه_language ('und-fonupa', 'UPPH')

ot.جمع_کړه_language ('ga-Latg', 'IRT')

ot.جمع_کړه_language ('hy-arevmda', 'HYE')

ot.remove_language_ot ('KGE')
ot.جمع_کړه_language ('und-Geok', 'KGE')

ot.جمع_کړه_language ('kht', 'KHN')
ot.نومونه['KHN'] = ot.نومونه['KHT'] + ' (Microsoft fonts)'
ot.ranks['KHN'] = ot.ranks['KHT'] + 1

ot.ranks['LCR'] = ot.ranks['MCR'] + 1

ot.نومونه['MAL'] = 'Malayalam Traditional'
ot.ranks['MLR'] += 1

bcp_47.نومونه['mhv'] = 'Arakanese'
bcp_47.scopes['mhv'] = ' (retired code)'

ot.جمع_کړه_language ('mnw-TH', 'MONT')

ot.جمع_کړه_language ('no', 'NOR')

ot.جمع_کړه_language ('oc-provenc', 'PRO')

ot.remove_language_ot ('QUZ')
ot.جمع_کړه_language ('qu', 'QUZ')
ot.جمع_کړه_language ('qub', 'QWH')
ot.جمع_کړه_language ('qud', 'QVI')
ot.جمع_کړه_language ('qug', 'QVI')
ot.جمع_کړه_language ('qul', 'QUH')
ot.جمع_کړه_language ('qup', 'QVI')
ot.جمع_کړه_language ('qur', 'QWH')
ot.جمع_کړه_language ('qus', 'QUH')
ot.جمع_کړه_language ('quw', 'QVI')
ot.جمع_کړه_language ('qux', 'QWH')
ot.جمع_کړه_language ('qva', 'QWH')
ot.جمع_کړه_language ('qvh', 'QWH')
ot.جمع_کړه_language ('qvj', 'QVI')
ot.جمع_کړه_language ('qvl', 'QWH')
ot.جمع_کړه_language ('qvm', 'QWH')
ot.جمع_کړه_language ('qvn', 'QWH')
ot.جمع_کړه_language ('qvo', 'QVI')
ot.جمع_کړه_language ('qvp', 'QWH')
ot.جمع_کړه_language ('qvw', 'QWH')
ot.جمع_کړه_language ('qvz', 'QVI')
ot.جمع_کړه_language ('qwa', 'QWH')
ot.جمع_کړه_language ('qws', 'QWH')
ot.جمع_کړه_language ('qxa', 'QWH')
ot.جمع_کړه_language ('qxc', 'QWH')
ot.جمع_کړه_language ('qxh', 'QWH')
ot.جمع_کړه_language ('qxl', 'QVI')
ot.جمع_کړه_language ('qxn', 'QWH')
ot.جمع_کړه_language ('qxo', 'QWH')
ot.جمع_کړه_language ('qxr', 'QVI')
ot.جمع_کړه_language ('qxt', 'QWH')
ot.جمع_کړه_language ('qxw', 'QWH')

bcp_47.macrolanguages['ro-MD'].جمع_کړه ('mo')

ot.remove_language_ot ('SYRE')
ot.remove_language_ot ('SYRJ')
ot.remove_language_ot ('SYRN')
ot.جمع_کړه_language ('und-Syre', 'SYRE')
ot.جمع_کړه_language ('und-Syrj', 'SYRJ')
ot.جمع_کړه_language ('und-Syrn', 'SYRN')

bcp_47.نومونه['xst'] = "Silt'e"
bcp_47.scopes['xst'] = ' (retired code)'
bcp_47.macrolanguages['xst'] = {'stv', 'wle'}

ot.جمع_کړه_language ('xwo', 'TOD')

ot.remove_language_ot ('ZHH')
ot.remove_language_ot ('ZHP')
ot.remove_language_ot ('ZHT')
ot.remove_language_ot ('ZHTM')
bcp_47.macrolanguages['zh'].remove ('lzh')
bcp_47.macrolanguages['zh'].remove ('yue')
ot.جمع_کړه_language ('zh-Hant-MO', 'ZHH')
ot.جمع_کړه_language ('zh-Hant-MO', 'ZHTM')
ot.جمع_کړه_language ('zh-Hant-HK', 'ZHH')
ot.جمع_کړه_language ('zh-Hans', 'ZHS')
ot.جمع_کړه_language ('zh-Hant', 'ZHT')
ot.جمع_کړه_language ('zh-HK', 'ZHH')
ot.جمع_کړه_language ('zh-MO', 'ZHH')
ot.جمع_کړه_language ('zh-MO', 'ZHTM')
ot.جمع_کړه_language ('zh-TW', 'ZHT')
ot.جمع_کړه_language ('lzh', 'ZHT')
ot.جمع_کړه_language ('lzh-Hans', 'ZHS')
ot.جمع_کړه_language ('yue', 'ZHH')
ot.جمع_کړه_language ('yue-Hans', 'ZHS')

تعریف rank_delta (bcp_47, ot):
	"""Return a delta to apply to a BCP 47 tag's rank.

	Most OpenType tags have a constant rank, but a few have ranks that
	depend on the BCP 47 tag.

	Args:
		bcp_47 (str): A BCP 47 tag.
		ot (str): An OpenType tag to.

	Returns:
		A number to add to ``ot``'s rank when sorting ``bcp_47``'s
		OpenType equivalents.
	"""
	إذا bcp_47 == 'ak' و ot == 'AKA':
		أرجع -1
	إذا bcp_47 == 'tw' و ot == 'TWI':
		أرجع -1
	أرجع 0

disambiguation = {
	'ALT': 'alt',
	'ARK': 'rki',
	'ATH': 'ath',
	'BHI': 'bhb',
	'BLN': 'bjt',
	'BTI': 'beb',
	'CCHN': 'cco',
	'CMR': 'swb',
	'CPP': 'crp',
	'CRR': 'crx',
	'DUJ': 'dwu',
	'ECR': 'crj',
	'HAL': 'cfm',
	'HND': 'hnd',
	'HYE': 'hyw',
	'KIS': 'kqs',
	'KUI': 'uki',
	'LRC': 'bqi',
	'NDB': 'nd',
	'NIS': 'njz',
	'PLG': 'pce',
	'PRO': 'pro',
	'QIN': 'bgr',
	'QUH': 'quh',
	'QVI': 'qvi',
	'QWH': 'qwh',
	'SIG': 'stv',
	'SRB': 'sr',
	'SXT': 'xnj',
	'ZHH': 'zh-HK',
	'ZHS': 'zh-Hans',
	'ZHT': 'zh-Hant',
	'ZHTM': 'zh-MO',
}

ot.inherit_from_macrolanguages ()
bcp_47.remove_extra_macrolanguages ()
ot.inherit_from_macrolanguages ()
ot.نومونه[DEFAULT_LANGUAGE_SYSTEM] = ''
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.ارزښتونه ()) + 1
لكل tricky_ot_نښان في filter (مجهول نښان_2: re.سمون ('[A-Z]{3}$', نښان_2), ot.نومونه):
	possible_bcp_47_نښان = tricky_ot_نښان.ښکتنی ()
	إذا possible_bcp_47_نښان في bcp_47.نومونه و ليس ot.from_bcp_47[possible_bcp_47_نښان]:
		ot.جمع_کړه_language (possible_bcp_47_نښان, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_نښان] = سېټ ()
ot.ترتیبول_languages ()

إذا output_rust:
	تعریف hb_نښان_rust (نښان_2):
		إذا نښان_2 == DEFAULT_LANGUAGE_SYSTEM:
			أرجع 'Tag::new(&[0; 4])'
		sanitize_نښان (نښان_2)
		أرجع 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % نښان_2)[:4])

	تعریف get_variant_سېټ_rust (name):
		أرجع سېټ (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				لكل n في re.وېشل ('[\n(),]', name) إذا n)

	تعریف language_name_intersection_rust (a, b):
		أرجع get_variant_سېټ_rust (a).intersection (get_variant_سېټ_rust (b))

	تعریف get_matching_language_name_rust (intersection, candidates):
		أرجع بل (iter (c لكل c في candidates إذا ليس intersection.isdisjoint (get_variant_سېټ_rust (c))))

	چاپ ('// WARNING: this file was generated by ../../harfbuzz/src/gen-tag-table.py --rust')
	چاپ ()
	چاپ ('use read_fonts::types::Tag;')
	چاپ ()
	چاپ ('pub(crate) struct LangTag {')
	چاپ ('    pub language: [u8; 4],')
	چاپ ('    pub tag: Tag,')
	چاپ ('}')
	چاپ ()
	چاپ ('const fn lang(language: &[u8]) -> [u8; 4] {')
	چاپ ('    let mut bytes = [0; 4];')
	چاپ ('    let mut i = 0;')
	چاپ ()
	چاپ ('    while i < language.len() {')
	چاپ ('        bytes[i] = language[i];')
	چاپ ('        i += 1;')
	چاپ ('    }')
	چاپ ()
	چاپ ('    bytes')
	چاپ ('}')
	چاپ ()
	چاپ ('use super::tag::{lang_matches, strncmp, subtag_matches};')
	چاپ ()
	چاپ ('#[rustfmt::skip]')
	چاپ ('pub(crate) static OPEN_TYPE_LANGUAGES: &[LangTag] = &[')

	لكل language, نښانونه في sorted (ot.from_bcp_47.توکي ()):
		إذا language == '' أو '-' في language:
			استمر
		commented_بهر = اوږدوالی (language) == 3 و اوږدوالی (نښانونه) == 1 و language == نښانونه[0].ښکتنی ()
		لكل نښان_2 في نښانونه:
			چاپ ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' إذا commented_بهر وإلا '    ', sanitize_bcp47_نښان (language), hb_نښان_rust (نښان_2)), پای_ټکی='')
			چاپ (' // ', پای_ټکی='')
			bcp_47_name = bcp_47.نومونه.get (language, '')
			bcp_47_name_candidates = bcp_47_name.وېشل ('\n')
			ot_name = ot.نومونه[نښان_2]
			scope = bcp_47.scopes.get (language, '')
			إذا نښان_2 == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			وإلا:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				إذا ليس intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				وإلا:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.نومونه[language] = name
					comment = '%s%s' % (name إذا اوږدوالی (name) > اوږدوالی (ot_name) وإلا ot_name, scope)
			چاپ (sanitize_comment (comment))

	چاپ ('];')
	چاپ ()
	چاپ ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	چاپ ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	تعریف print_subtag_matches_rust (subtag, new_کرښه):
		إذا subtag:
			إذا new_کرښه:
				چاپ (' && ', پای_ټکی='')
			چاپ ('subtag_matches(language, "-%s")' % sanitize_bcp47_نښان (subtag), پای_ټکی='')

	complex_نښانونه = collections.defaultdict (list)
	لكل initial, ډله في itertools.groupby ((lt_نښانونه لكل lt_نښانونه في [
				(Language_نښان (language), نښانونه)
				لكل language, نښانونه في sorted (ot.from_bcp_47.توکي (),
					کلی_2=مجهول i: (-اوږدوالی (i[0]), i[0]))
			] إذا lt_نښانونه[0].is_complex ()),
			کلی_2=مجهول lt_نښانونه: lt_نښانونه[0].get_ډله ()):
		complex_نښانونه[initial] += ډله

	لكل initial, توکي في sorted (complex_نښانونه.توکي ()):
		إذا initial != 'und':
			استمر
		لكل lt, نښانونه في توکي:
			إذا ليس نښانونه:
				استمر
			إذا lt.variant في bcp_47.prefixes:
				expect (بل (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			چاپ ('    if ', پای_ټکی='')
			print_subtag_matches_rust (lt.script, خطأ)
			print_subtag_matches_rust (lt.region, خطأ)
			print_subtag_matches_rust (lt.variant, خطأ)
			چاپ (' {')
			چاپ ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			إذا اوږدوالی (نښانونه) == 1:
				چاپ ('        tags.push(%s); // %s' % (hb_نښان_rust (نښانونه[0]), sanitize_comment (ot.نومونه[نښانونه[0]])))
			وإلا:
				چاپ ('        let possible_tags = &[')
				لكل نښان_2 في نښانونه:
					چاپ ('            %s, // %s' % (hb_نښان_rust (نښان_2), sanitize_comment (ot.نومونه[نښان_2])))
				چاپ ('        ];')
				چاپ ('        tags.extend_from_slice(possible_tags);')
			چاپ ('        return true;')
			چاپ ('    }')

	چاپ ('    match language.as_bytes()[0] {')
	لكل initial, توکي في sorted (complex_نښانونه.توکي ()):
		إذا initial == 'und':
			استمر
		چاپ ("        b'%s' => {" % sanitize_bcp47_نښان (initial))
		لكل lt, نښانونه في توکي:
			إذا ليس نښانونه:
				استمر
			چاپ ('            if ', پای_ټکی='')
			script = lt.script
			region = lt.region
			إذا lt.grandfathered:
				چاپ ('&language[1..] == "%s"' % sanitize_bcp47_نښان (lt.language[1:]), پای_ټکی='')
			وإلا:
				string_literal = lt.language[1:] + '-'
				إذا script:
					string_literal += script
					script = هیڅ
					إذا region:
						string_literal += '-' + region
						region = هیڅ
				إذا string_literal[-1] == '-':
					چاپ ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_نښان (string_literal), اوږدوالی (string_literal)), پای_ټکی='')
				وإلا:
					چاپ ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_نښان (string_literal), پای_ټکی='')
			print_subtag_matches_rust (script, ریښتیا)
			print_subtag_matches_rust (region, ریښتیا)
			print_subtag_matches_rust (lt.variant, ریښتیا)
			چاپ (' {')
			چاپ ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			إذا اوږدوالی (نښانونه) == 1:
				چاپ ('                tags.push(%s); // %s' % (hb_نښان_rust (نښانونه[0]), sanitize_comment (ot.نومونه[نښانونه[0]])))
			وإلا:
				چاپ ('                let possible_tags = &[')
				لكل نښان_2 في نښانونه:
					چاپ ('                    %s, // %s' % (hb_نښان_rust (نښان_2), sanitize_comment (ot.نومونه[نښان_2])))
				چاپ ('                ];')
				چاپ ('                tags.extend_from_slice(possible_tags);')
			چاپ ('                return true;')
			چاپ ('            }')
		چاپ ('        }')
	چاپ ('        _ => {}')
	چاپ ('    }')
	چاپ ('    false')
	چاپ ('}')
	sys.exit (0)

چاپ ('/* == Start of generated table == */')
چاپ ('/*')
چاپ (' * The following table is generated by running:')
چاپ (' *')
چاپ (' *   %s languagetags language-subtag-registry' % os.لار.basename (sys.argv[0]))
چاپ (' *')
چاپ (' * on files with these headers:')
چاپ (' *')
چاپ (' * %s' % sanitize_سرلیک (ot.سرلیک))
چاپ (' * %s' % sanitize_سرلیک (bcp_47.سرلیک))
چاپ (' */')
چاپ ()
چاپ ('#ifndef HB_OT_TAG_TABLE_HH')
چاپ ('#define HB_OT_TAG_TABLE_HH')
چاپ ()

تعریف hb_نښان (نښان_2):
	"""Convert a tag to ``HB_TAG`` form.

	Args:
		tag (str): An OpenType tag.

	Returns:
		A snippet of C++ representing ``tag``.
	"""
	إذا نښان_2 == DEFAULT_LANGUAGE_SYSTEM:
		أرجع 'HB_TAG_NONE\t       '
	sanitize_نښان (نښان_2)
	أرجع "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % نښان_2)[:4])

تعریف get_variant_سېټ (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	أرجع سېټ (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			لكل n في re.وېشل ('[\n(),]', name) إذا n)

تعریف language_name_intersection (a, b):
	"""Return the names in common between two language names.

	Args:
		a (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.
		b (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		The normalized language names shared by ``a`` and ``b``.
	"""
	أرجع get_variant_سېټ (a).intersection (get_variant_سېټ (b))

تعریف get_matching_language_name (intersection, candidates):
	أرجع بل (iter (c لكل c في candidates إذا ليس intersection.isdisjoint (get_variant_سېټ (c))))

تعریف same_نښان (bcp_47_نښان, ot_نښانونه):
	أرجع اوږدوالی (bcp_47_نښان) == 3 و اوږدوالی (ot_نښانونه) == 1 و bcp_47_نښان == ot_نښانونه[0].ښکتنی ()

تعریف format_language_comment (language, نښان_2):
	bcp_47_name = bcp_47.نومونه.get (language, '')
	bcp_47_name_candidates = bcp_47_name.وېشل ('\n')
	ot_name = ot.نومونه[نښان_2]
	scope = bcp_47.scopes.get (language, '')
	إذا نښان_2 == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	وإلا:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		إذا ليس intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		وإلا:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.نومونه[language] = name
			comment = '%s%s' % (name إذا اوږدوالی (name) > اوږدوالی (ot_name) وإلا ot_name, scope)
	أرجع sanitize_comment (comment)

چاپ ('static const LangTag ot_languages2[] = {')
لكل language, نښانونه في sorted (ot.from_bcp_47.توکي ()):
	إذا language == '' أو '-' في language أو اوږدوالی (language) != 2:
		استمر
	لكل نښان_2 في نښانونه:
		چاپ ('  {%s,\t%s},\t/* %s */' % (
			hb_نښان (language),
			hb_نښان (نښان_2),
			format_language_comment (language, نښان_2)))
چاپ ('};')
چاپ ()

چاپ ('#ifndef HB_NO_LANGUAGE_LONG')

languages3_blocked = []
languages3_singles = []
languages3_multi_ranges = []
languages3_multi_ارزښتونه = []
لكل language, نښانونه في sorted (ot.from_bcp_47.توکي ()):
	إذا language == '' أو '-' في language أو اوږدوالی (language) != 3:
		استمر
	إذا same_نښان (language, نښانونه):
		استمر
	إذا نښانونه == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, نښانونه[0]))
		استمر
	إذا اوږدوالی (نښانونه) == 1:
		languages3_singles.append ((language, نښانونه[0]))
		استمر
	دننه_واټن = اوږدوالی (languages3_multi_ارزښتونه)
	languages3_multi_ارزښتونه.extend ((language, نښان_2) لكل نښان_2 في نښانونه)
	languages3_multi_ranges.append ((language, دننه_واټن, اوږدوالی (نښانونه), نښانونه[0]))

چاپ ('static const hb_tag_t ot_languages3_blocked[] = {')
لكل language, نښان_2 في languages3_blocked:
	چاپ ('  %s,\t/* %s */' % (hb_نښان (language), format_language_comment (language, نښان_2)))
چاپ ('};')
چاپ ()

چاپ ('static const LangTag ot_languages3[] = {')
لكل language, نښان_2 في languages3_singles:
	چاپ ('  {%s,\t%s},\t/* %s */' % (
		hb_نښان (language),
		hb_نښان (نښان_2),
		format_language_comment (language, نښان_2)))
چاپ ('};')
چاپ ()

چاپ ('static const hb_tag_t ot_languages3_multi_values[] = {')
لكل language, نښان_2 في languages3_multi_ارزښتونه:
	چاپ ('  %s,\t/* %s */' % (hb_نښان (نښان_2), format_language_comment (language, نښان_2)))
چاپ ('};')
چاپ ()

چاپ ('static const LangTagRange ot_languages3_multi[] = {')
لكل language, دننه_واټن, شمېر, لومړی_نښان في languages3_multi_ranges:
	چاپ ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_نښان (language),
		صحيح(دننه_واټن),
		صحيح(شمېر),
		format_language_comment (language, لومړی_نښان)))
چاپ ('};')
چاپ ('#endif')
چاپ ()

چاپ ('/**')
چاپ (' * hb_ot_tags_from_complex_language:')
چاپ (' * @lang_str: a BCP 47 language tag to convert.')
چاپ (' * @limit: a pointer to the end of the substring of @lang_str to consider for')
چاپ (' * conversion.')
چاپ (' * @count: maximum number of language tags to retrieve (IN) and actual number of')
چاپ (' * language tags retrieved (OUT). If no tags are retrieved, it is not modified.')
چاپ (' * @tags: array of size at least @language_count to store the language tag')
چاپ (' * results')
چاپ (' *')
چاپ (' * Converts a multi-subtag BCP 47 language tag to language tags.')
چاپ (' *')
چاپ (' * Return value: Whether any language systems were retrieved.')
چاپ (' **/')
چاپ ('static inline bool')
چاپ ('hb_ot_tags_from_complex_language (const char   *lang_str,')
چاپ ('\t\t\t\t  const char   *limit,')
چاپ ('\t\t\t\t  unsigned int *count /* IN/OUT */,')
چاپ ('\t\t\t\t  hb_tag_t     *tags /* OUT */)')
چاپ ('{')

تعریف print_subtag_matches (subtag, string, new_کرښه):
	إذا subtag:
		إذا new_کرښه:
			چاپ ()
			چاپ ('\t&& ', پای_ټکی='')
		چاپ ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_نښان (subtag), 1 + اوږدوالی (subtag)), پای_ټکی='')

complex_نښانونه = collections.defaultdict (list)
لكل initial, ډله في itertools.groupby ((lt_نښانونه لكل lt_نښانونه في [
			(Language_نښان (language), نښانونه)
			لكل language, نښانونه في sorted (ot.from_bcp_47.توکي (),
				کلی_2=مجهول i: (-اوږدوالی (i[0]), i[0]))
		] إذا lt_نښانونه[0].is_complex ()),
		کلی_2=مجهول lt_نښانونه: lt_نښانونه[0].get_ډله ()):
	complex_نښانونه[initial] += ډله

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
لكل initial, توکي في sorted (complex_نښانونه.توکي ()):
	إذا initial != 'und':
		استمر
	لكل lt, نښانونه في توکي:
		إذا ليس نښانونه:
			استمر
		subtag_len = 0
		subtag_len += 1 + اوږدوالی (lt.script) إذا lt.script دی ليس هیڅ وإلا 0
		subtag_len += 1 + اوږدوالی (lt.region) إذا lt.region دی ليس هیڅ وإلا 0
		subtag_len += 1 + اوږدوالی (lt.variant) إذا lt.variant دی ليس هیڅ وإلا 0
		min_subtag_len = min(subtag_len, min_subtag_len)

چاپ ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
چاپ ('  {')
چاپ ("    const char *p = strchr (lang_str, '-');")
چاپ ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
لكل initial, توکي في sorted (complex_نښانونه.توکي ()):
	إذا initial != 'und':
		استمر
	لكل lt, نښانونه في توکي:
		إذا ليس نښانونه:
			استمر
		إذا lt.variant في bcp_47.prefixes:
			expect (بل (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		چاپ ('    if (', پای_ټکی='')
		print_subtag_matches (lt.script, 'p', خطأ)
		print_subtag_matches (lt.region, 'p', خطأ)
		print_subtag_matches (lt.variant, 'p', خطأ)
		چاپ (')')
		چاپ ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		چاپ ()
		إذا اوږدوالی (نښانونه) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_نښان (نښانونه[0]), sanitize_comment (ot.نومونه[نښانونه[0]])))
			چاپ ()
			چاپ ('      *count = 1;')
		وإلا:
			چاپ ('    hb_tag_t possible_tags[] = {')
			لكل نښان_2 في نښانونه:
				اكتب ('      %s,  /* %s */' % (hb_نښان (نښان_2), sanitize_comment (ot.نومونه[نښان_2])))
				چاپ ()
			چاپ ('      };')
			چاپ ('      for (i = 0; i < %s && i < *count; i++)' % اوږدوالی (نښانونه))
			چاپ ('\ttags[i] = possible_tags[i];')
			چاپ ('      *count = i;')
		چاپ ('      return true;')
		چاپ ('    }')
چاپ ('  }')
چاپ ('out:')

چاپ ('  switch (lang_str[0])')
چاپ ('  {')
لكل initial, توکي في sorted (complex_نښانونه.توکي ()):
	إذا initial == 'und':
		استمر
	چاپ ("  case '%s':" % sanitize_bcp47_نښان (initial))
	لكل lt, نښانونه في توکي:
		إذا ليس نښانونه:
			استمر
		چاپ ('    if (', پای_ټکی='')
		script = lt.script
		region = lt.region
		إذا lt.grandfathered:
			چاپ ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_نښان (lt.language[1:]), پای_ټکی='')
		وإلا:
			string_literal = lt.language[1:] + '-'
			إذا script:
				string_literal += script
				script = هیڅ
				إذا region:
					string_literal += '-' + region
					region = هیڅ
			إذا string_literal[-1] == '-':
				چاپ ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_نښان (string_literal), اوږدوالی (string_literal)), پای_ټکی='')
			وإلا:
				چاپ ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_نښان (string_literal), اوږدوالی (string_literal)), پای_ټکی='')
		print_subtag_matches (script, 'lang_str', ریښتیا)
		print_subtag_matches (region, 'lang_str', ریښتیا)
		print_subtag_matches (lt.variant, 'lang_str', ریښتیا)
		چاپ (')')
		چاپ ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		چاپ ()
		إذا اوږدوالی (نښانونه) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_نښان (نښانونه[0]), sanitize_comment (ot.نومونه[نښانونه[0]])))
			چاپ ()
			چاپ ('      *count = 1;')
		وإلا:
			چاپ ('      unsigned int i;')
			چاپ ('      hb_tag_t possible_tags[] = {')
			لكل نښان_2 في نښانونه:
				اكتب ('\t%s,  /* %s */' % (hb_نښان (نښان_2), sanitize_comment (ot.نومونه[نښان_2])))
				چاپ ()
			چاپ ('      };')
			چاپ ('      for (i = 0; i < %s && i < *count; i++)' % اوږدوالی (نښانونه))
			چاپ ('\ttags[i] = possible_tags[i];')
			چاپ ('      *count = i;')
		چاپ ('      return true;')
		چاپ ('    }')
	چاپ ('    break;')

چاپ ('  }')
چاپ ('  return false;')
چاپ ('}')
چاپ ()
چاپ ('/**')
چاپ (' * hb_ot_ambiguous_tag_to_language')
چاپ (' * @tag: A language tag.')
چاپ (' *')
چاپ (' * Converts @tag to a BCP 47 language tag if it is ambiguous (it corresponds to')
چاپ (' * many language tags) and the best tag is not the first (sorted alphabetically,')
چاپ (' * with two-letter tags having priority over all three-letter tags), or if the')
چاپ (' * best tag consists of multiple subtags, or if the best tag does not appear in')
چاپ (' * #ot_languages2 or #ot_languages3.')
چاپ (' *')
چاپ (' * Return value: The #hb_language_t corresponding to the BCP 47 language tag,')
چاپ (' * or #HB_LANGUAGE_INVALID if @tag is not ambiguous.')
چاپ (' **/')
چاپ ('static inline hb_language_t')
چاپ ('hb_ot_ambiguous_tag_to_language (hb_tag_t tag)')
چاپ ('{')
چاپ ('  switch (tag)')
چاپ ('  {')

تعریف verify_disambiguation_dict ():
	"""Verify and normalize ``disambiguation``.

	``disambiguation`` is a map of ambiguous OpenType language system
	tags to the particular BCP 47 tags they correspond to. This function
	checks that all its keys really are ambiguous and that each key's
	value is valid for that key. It checks that no ambiguous tag is
	missing, except when it can figure out which BCP 47 tag is the best
	by itself.

	It modifies ``disambiguation`` to remove keys whose values are the
	same as those that the fallback would return anyway, and to add
	ambiguous keys whose disambiguations it determined automatically.

	Raises:
		AssertionError: Verification failed.
	"""
	عام bcp_47
	عام disambiguation
	عام ot
	لكل ot_نښان, bcp_47_نښانونه في ot.منزل_bcp_47.توکي ():
		إذا ot_نښان == DEFAULT_LANGUAGE_SYSTEM:
			primary_نښانونه = []
		وإلا:
			primary_نښانونه = list (t لكل t في bcp_47_نښانونه إذا t ليس في bcp_47.grandfathered و ot.from_bcp_47.get (t)[0] == ot_نښان)
		إذا اوږدوالی (primary_نښانونه) == 1:
			expect (ot_نښان ليس في disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_نښان)
			إذا '-' في primary_نښانونه[0]:
				disambiguation[ot_نښان] = primary_نښانونه[0]
			وإلا:
				لومړی_نښان = sorted ((t لكل t في bcp_47_نښانونه إذا t ليس في bcp_47.grandfathered و ot_نښان في ot.from_bcp_47.get (t)),
						کلی_2=مجهول t: (اوږدوالی (t), t))[0]
				إذا primary_نښانونه[0] != لومړی_نښان:
					disambiguation[ot_نښان] = primary_نښانونه[0]
		وإلاإذا اوږدوالی (primary_نښانونه) == 0:
			expect (ot_نښان ليس في disambiguation, 'There is no possible valid disambiguation for %s' % ot_نښان)
		وإلا:
			original_languages = [t لكل t في primary_نښانونه إذا t في ot.from_bcp_47_uninherited و 'retired code' ليس في bcp_47.scopes.get (t, '')]
			إذا اوږدوالی (original_languages) == 1:
				macrolanguages = original_languages
			وإلا:
				macrolanguages = [t لكل t في primary_نښانونه إذا bcp_47.scopes.get (t) == ' [macrolanguage]']
			إذا اوږدوالی (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_نښانونه إذا bcp_47.scopes.get (t) == ' [collection]')
			إذا اوږدوالی (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_نښانونه إذا 'retired code' ليس في bcp_47.scopes.get (t, ''))
			إذا اوږدوالی (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_نښانونه إذا t.ښکتنی () == ISO_639_3_منزل_1.get (ot_نښان.ښکتنی (), ot_نښان.ښکتنی ()))
			إذا اوږدوالی (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_نښانونه إذا '-' ليس في t)
			إذا اوږدوالی (macrolanguages) != 1:
				expect (ot_نښان في disambiguation, 'ambiguous OT tag: %s %s' % (ot_نښان, sorted (primary_نښانونه)))
				expect (disambiguation[ot_نښان] في bcp_47_نښانونه,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_نښان], ot_نښان))
			وإلاإذا ot_نښان ليس في disambiguation:
				disambiguation[ot_نښان] = macrolanguages[0]
			إذا '-' ليس في disambiguation[ot_نښان]:
				different_bcp_47_نښانونه = sorted ((t لكل t في bcp_47_نښانونه إذا ليس same_نښان (t, ot.from_bcp_47.get (t))),
						کلی_2=مجهول t: (اوږدوالی (t), t))
				إذا different_bcp_47_نښانونه و disambiguation[ot_نښان] == different_bcp_47_نښانونه[0]:
					لرېکړه disambiguation[ot_نښان]
	لكل ot_نښان في disambiguation.کلیګانې ():
		expect (ot_نښان في ot.منزل_bcp_47, 'unknown OT tag: %s' % ot_نښان)

verify_disambiguation_dict ()
لكل ot_نښان, bcp_47_نښان في sorted (disambiguation.توکي ()):
	اكتب ('  case %s:  /* %s */' % (hb_نښان (ot_نښان), sanitize_comment (ot.نومونه[ot_نښان])))
	چاپ ()
	اكتب ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_نښان (bcp_47_نښان), sanitize_comment (bcp_47.get_name (Language_نښان (bcp_47_نښان)))))
	چاپ ()

چاپ ('  default:')
چاپ ('    return HB_LANGUAGE_INVALID;')
چاپ ('  }')
چاپ ('}')

چاپ ()
چاپ ('#endif /* HB_OT_TAG_TABLE_HH */')
چاپ ()
چاپ ('/* == End of generated table == */')
