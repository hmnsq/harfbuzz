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

درآمد collections
درآمد html
سے html.parser درآمد HTMLParser
درآمد itertools
درآمد os
درآمد re
درآمد sys
درآمد unicodedata

اگر لمبائی (sys.argv) > 1 اور sys.argv[1] == '--rust':
	مٹاؤ sys.argv[1]
	output_rust = سچ
ورنہ:
	output_rust = جھوٹ

اگر لمبائی (sys.argv) != 3:
	sys.exit (__doc__)

تعریف expect (condition, پیغام=کچھ_نہیں):
	اگر نہیں condition:
		اگر پیغام ہے کچھ_نہیں:
			اٹھاؤ AssertionError
		اٹھاؤ AssertionError (پیغام)

تعریف اكتب (s):
	sys.stdout.flush ()
	sys.stdout.بفر.اكتب (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = سیٹ (" -—–.,;:’'ʼ()[]/!=>_+")

تعریف sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	اگر نہیں isinstance (s, str):
		اٹھاؤ TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	اگر نہیں s:
		واپس ''
	برائے ch میں s:
		cat = unicodedata.category (ch)
		اگر نہیں (cat.startswith (('L', 'M', 'N', 'Z')) یا ch میں _ALLOWED_COMMENT_PUNCT):
			اٹھاؤ قدر_Error ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	واپس s

تعریف sanitize_سرخی (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	اگر نہیں isinstance (s, str):
		اٹھاؤ TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	اگر نہیں re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		اٹھاؤ قدر_Error ("Invalid header line %r: contains disallowed characters" % s)
	واپس s

تعریف sanitize_bcp47_لیبل (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	اگر نہیں isinstance (s, str):
		اٹھاؤ TypeError ("Expected string, got %s" % type (s).__name__)
	اگر نہیں re.fullmatch (r'[a-zA-Z0-9-]+', s):
		اٹھاؤ قدر_Error ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	واپس s

تعریف sanitize_لیبل (لیبل_2):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	اگر نہیں isinstance (لیبل_2, str):
		اٹھاؤ TypeError ("Expected string for tag, got %s" % type (لیبل_2).__name__)
	اگر لیبل_2 == DEFAULT_LANGUAGE_SYSTEM:
		واپس ''
	اگر نہیں re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', لیبل_2):
		اٹھاؤ قدر_Error ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % لیبل_2)
	واپس لیبل_2

DEFAULT_LANGUAGE_SYSTEM = ''

# from https://www-01.sil.org/iso639-3/iso-639-3.tab
ISO_639_3_مقصد_1 = {
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

جماعت Language_لیبل (object):
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
	تعریف __init__ (self, لیبل_2):
		عام bcp_47
		self.subtags = لیبل_2.زیریں ().بانٹو ('-')
		self.grandfathered = لیبل_2.زیریں () میں bcp_47.grandfathered
		اگر self.grandfathered:
			self.language = لیبل_2.زیریں ()
			self.script = ''
			self.region = ''
			self.variant = ''
		ورنہ:
			self.language = self.subtags[0]
			self.script = self.ڈھونڈو_پہلا (بےنام s: لمبائی (s) == 4 اور s[0] > '9', self.subtags)
			self.region = self.ڈھونڈو_پہلا (بےنام s: لمبائی (s) == 2 اور s[0] > '9' یا لمبائی (s) == 3 اور s[0] <= '9', self.subtags[1:])
			self.variant = self.ڈھونڈو_پہلا (بےنام s: لمبائی (s) > 4 یا لمبائی (s) == 4 اور s[0] <= '9', self.subtags)

	تعریف __str__(self):
		واپس '-'.join(self.subtags)

	تعریف __repr__ (self):
		واپس 'LanguageTag(%r)' % str(self)

	@staticmethod
	تعریف ڈھونڈو_پہلا (function, sequence):
		کوشش:
			واپس اگلا (iter (filter (function, sequence)))
		استثنا StopIteration:
			واپس کچھ_نہیں

	تعریف is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		واپس نہیں (لمبائی (self.subtags) == 1
			یا self.grandfathered
			اور لمبائی (self.subtags[1]) != 3
			اور ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	تعریف get_گروہ (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		واپس ('und'
			اگر (self.language == 'und'
				یا self.variant میں bcp_47.prefixes اور لمبائی (bcp_47.prefixes[self.variant]) == 1)
			ورنہ self.language[0])

جماعت OpenTypeRegistryParser (HTMLParser):
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
		self.سرخی = ''
		self.نام = {}
		self.ranks = collections.defaultdict (صحیح)
		self.مقصد_bcp_47 = collections.defaultdict (سیٹ)
		self.from_bcp_47 = collections.defaultdict (سیٹ)
		self.from_bcp_47_uninherited = کچھ_نہیں
		# Whether the parser is in a <td> element
		self._td = جھوٹ
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = جھوٹ
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	تعریف handle_starttag (self, لیبل_2, attrs):
		اگر لیبل_2 == 'a':
			اگر self._current_tr اور نہیں self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = سچ
		وگرنہ_اگر لیبل_2 == 'br':
			self._disengaged = سچ
		وگرنہ_اگر لیبل_2 == 'meta':
			برائے attr, قدر_2 میں attrs:
				اگر attr == 'name' اور قدر_2 == 'updated_at':
					self.سرخی = self.get_starttag_متن ()
					توڑو
		وگرنہ_اگر لیبل_2 == 'td':
			self._td = سچ
			self._current_tr.append ('')
		وگرنہ_اگر لیبل_2 == 'tr':
			self._disengaged = جھوٹ
			self._current_tr = []

	تعریف handle_endtag (self, لیبل_2):
		اگر لیبل_2 == 'td':
			self._td = جھوٹ
			self._disengaged = جھوٹ
		وگرنہ_اگر لیبل_2 == 'tr' اور self._current_tr:
			expect (2 <= لمبائی (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			لیبل_2 = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			اگر لمبائی (لیبل_2) > 4:
				expect (لیبل_2.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % لیبل_2)
				name += ' (deprecated)'
				لیبل_2 = لیبل_2.بانٹو (' ')[0]
				rank = 1
			self.نام[لیبل_2] = re.sub (' languages$', '', name)
			اگر نہیں self._current_tr[2]:
				واپس
			iso_codes = self._current_tr[2].strip ()
			self.مقصد_bcp_47[لیبل_2].update (ISO_639_3_مقصد_1.get (رمز, رمز) برائے رمز میں iso_codes.replace (' ', '').بانٹو (','))
			rank += 2 * لمبائی (self.مقصد_bcp_47[لیبل_2])
			self.ranks[لیبل_2] = rank

	تعریف handle_ڈیٹا (self, ڈیٹا):
		اگر self._td اور نہیں self._disengaged:
			self._current_tr[-1] += ڈیٹا

	تعریف handle_charref (self, name):
		self.handle_ڈیٹا (html.unescape ('&#%s;' % name))

	تعریف handle_entityref (self, name):
		self.handle_ڈیٹا (html.unescape ('&%s;' % name))

	تعریف parse (self, filename):
		"""Parse the OpenType language system tag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		ساتھ open (filename, encoding='utf-8') بطور f:
			self.feed (f.اقرأ ())
		expect (self.سرخی)
		برائے لیبل_2, iso_codes میں self.مقصد_bcp_47.اشیاء ():
			برائے iso_رمز میں iso_codes:
				self.from_bcp_47[iso_رمز].جوڑو (لیبل_2)

	تعریف جوڑو_language (self, bcp_47_لیبل, ot_لیبل):
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
		self.مقصد_bcp_47[ot_لیبل].جوڑو (bcp_47_لیبل)
		self.from_bcp_47[bcp_47_لیبل].جوڑو (ot_لیبل)
		اگر bcp_47_لیبل.زیریں () نہیں میں bcp_47.grandfathered:
			کوشش:
				[macrolanguage, suffix] = bcp_47_لیبل.بانٹو ('-', 1)
				اگر macrolanguage میں bcp_47.macrolanguages:
					s = سیٹ ()
					برائے language میں bcp_47.macrolanguages[macrolanguage]:
						اگر language.زیریں () نہیں میں bcp_47.grandfathered:
							s.جوڑو ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			استثنا قدر_Error:
				گزرو

	@staticmethod
	تعریف _remove_language (لیبل_1, dict_1, dict_2):
		برائے لیبل_2_2 میں dict_1.نکالو (لیبل_1):
			dict_2[لیبل_2_2].remove (لیبل_1)
			اگر نہیں dict_2[لیبل_2_2]:
				مٹاؤ dict_2[لیبل_2_2]

	تعریف remove_language_ot (self, ot_لیبل):
		"""Remove an OpenType tag from the registry.

		Args:
			ot_tag (str): An OpenType tag.
		"""
		self._remove_language (ot_لیبل, self.مقصد_bcp_47, self.from_bcp_47)

	تعریف remove_language_bcp_47 (self, bcp_47_لیبل):
		"""Remove a BCP 47 tag from the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag.
		"""
		self._remove_language (bcp_47_لیبل, self.from_bcp_47, self.مقصد_bcp_47)

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
		پہلا_وقت = self.from_bcp_47_uninherited ہے کچھ_نہیں
		اگر پہلا_وقت:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		برائے macrolanguage, languages میں dict (bcp_47.macrolanguages).اشیاء ():
			ot_macrolanguages = {
				ot_macrolanguage برائے ot_macrolanguage میں self.from_bcp_47_uninherited.get (macrolanguage, سیٹ ())
			}
			blocked_ot_macrolanguages = سیٹ ()
			اگر 'retired code' نہیں میں bcp_47.scopes.get (macrolanguage, ''):
				برائے ot_macrolanguage میں ot_macrolanguages:
					باری_trip_macrolanguages = {
						l برائے l میں self.مقصد_bcp_47[ot_macrolanguage]
						اگر 'retired code' نہیں میں bcp_47.scopes.get (l, '')
					}
					باری_trip_languages = {
						l برائے l میں languages
						اگر 'retired code' نہیں میں bcp_47.scopes.get (l, '')
					}
					intersection = باری_trip_macrolanguages & باری_trip_languages
					اگر intersection اور intersection != باری_trip_languages:
						blocked_ot_macrolanguages.جوڑو (ot_macrolanguage)
			اگر ot_macrolanguages:
				برائے ot_macrolanguage میں ot_macrolanguages:
					اگر ot_macrolanguage نہیں میں blocked_ot_macrolanguages:
						برائے language میں languages:
							self.جوڑو_language (language, ot_macrolanguage)
							اگر نہیں blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			وگرنہ_اگر پہلا_وقت:
				برائے language میں languages:
					اگر language میں self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					ورنہ:
						ot_macrolanguages.clear ()
					اگر نہیں ot_macrolanguages:
						توڑو
				برائے ot_macrolanguage میں ot_macrolanguages:
					self.جوڑو_language (macrolanguage, ot_macrolanguage)

	تعریف ترتیب_دو_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		برائے language, لیبل_فہرست میں self.from_bcp_47.اشیاء ():
			self.from_bcp_47[language] = sorted (لیبل_فہرست,
					کلید_2=بےنام t: (self.ranks[t] + rank_delta (language, t), t))

ot = OpenTypeRegistryParser ()

جماعت BCP47Parser (object):
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
		self.سرخی = ''
		self.نام = {}
		self.scopes = {}
		self.macrolanguages = collections.defaultdict (سیٹ)
		self.prefixes = collections.defaultdict (سیٹ)
		self.grandfathered = سیٹ ()

	تعریف parse (self, filename):
		"""Parse the BCP 47 subtag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		ساتھ open (filename, encoding='utf-8') بطور f:
			subtag_type = کچھ_نہیں
			subtag = کچھ_نہیں
			deprecated = جھوٹ
			has_preferred_قدر = جھوٹ
			سطر_بفر = ''
			برائے سطر میں itertools.chain (f, ['']):
				سطر = سطر.rstrip ()
				اگر سطر.startswith (' '):
					سطر_بفر += سطر[1:]
					جاری
				سطر, سطر_بفر = سطر_بفر, سطر
				اگر سطر.startswith ('Type: '):
					subtag_type = سطر.بانٹو (' ')[1]
					deprecated = جھوٹ
					has_preferred_قدر = جھوٹ
				وگرنہ_اگر سطر.startswith ('Subtag: ') یا سطر.startswith ('Tag: '):
					subtag = سطر.بانٹو (' ')[1]
					اگر subtag_type == 'grandfathered':
						self.grandfathered.جوڑو (subtag.زیریں ())
				وگرنہ_اگر سطر.startswith ('Description: '):
					description = سطر.بانٹو (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					اگر subtag میں self.نام:
						self.نام[subtag] += '\n' + description
					ورنہ:
						self.نام[subtag] = description
				وگرنہ_اگر subtag_type == 'language' یا subtag_type == 'grandfathered':
					اگر سطر.startswith ('Scope: '):
						scope = سطر.بانٹو (' ')[1]
						اگر scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						وگرنہ_اگر scope == 'collection':
							scope = ' [collection]'
						ورنہ:
							جاری
						self.scopes[subtag] = scope
					وگرنہ_اگر سطر.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = سچ
					وگرنہ_اگر deprecated اور سطر.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						برائے language میں سطر.replace (',', '').بانٹو (' ')[2:]:
							self.جوڑو_macrolanguage (subtag, language)
					وگرنہ_اگر سطر.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_قدر = سچ
						macrolanguage = سطر.بانٹو (' ')[1]
						self.جوڑو_macrolanguage (macrolanguage, subtag)
					وگرنہ_اگر نہیں has_preferred_قدر اور سطر.startswith ('Macrolanguage: '):
						self.جوڑو_macrolanguage (سطر.بانٹو (' ')[1], subtag)
				وگرنہ_اگر subtag_type == 'variant':
					اگر سطر.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					وگرنہ_اگر سطر.startswith ('Prefix: '):
						self.prefixes[subtag].جوڑو (سطر.بانٹو (' ')[1])
				وگرنہ_اگر سطر.startswith ('File-Date: '):
					self.سرخی = سطر
		expect (self.سرخی)

	تعریف جوڑو_macrolanguage (self, macrolanguage, language):
		عام ot
		اگر language نہیں میں ot.from_bcp_47:
			برائے l میں self.macrolanguages.get (language, سیٹ ()):
				self.جوڑو_macrolanguage (macrolanguage, l)
		اگر macrolanguage نہیں میں ot.from_bcp_47:
			برائے ls میں list (self.macrolanguages.اقدار ()):
				اگر macrolanguage میں ls:
					ls.جوڑو (language)
					واپس
		self.macrolanguages[macrolanguage].جوڑو (language)

	تعریف remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		برائے macrolanguage, languages میں self.macrolanguages.اشیاء ():
			برائے language میں languages:
				inverted[language].append (macrolanguage)
		برائے language, macrolanguages میں inverted.اشیاء ():
			اگر لمبائی (macrolanguages) > 1:
				macrolanguages.ترتیب_دو (کلید_2=بےنام ml: لمبائی (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.نکالو ()
				برائے macrolanguage میں macrolanguages:
					self.جوڑو_macrolanguage (biggest_macrolanguage, macrolanguage)

	تعریف _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		واپس self.نام[subtag].بانٹو ('\n')[0] + self.scopes.get (subtag, '')

	تعریف get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		اگر lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		اگر lt.region:
			name += '; ' + self._get_name_piece (lt.region.بالائی ())
		اگر lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		واپس name

bcp_47 = BCP47Parser ()

ot.parse (sys.argv[1])
bcp_47.parse (sys.argv[2])

ot.جوڑو_language ('ary', 'MOR')

ot.جوڑو_language ('ath', 'ATH')

ot.جوڑو_language ('bai', 'BML')

ot.ranks['BAL'] = ot.ranks['KAR'] + 1

ot.جوڑو_language ('ber', 'BBR')

ot.remove_language_ot ('PGR')
ot.جوڑو_language ('el-polyton', 'PGR')

bcp_47.نام['flm'] = 'Falam Chin'
bcp_47.scopes['flm'] = ' (retired code)'
bcp_47.macrolanguages['flm'] = {'cfm'}

ot.ranks['FNE'] = ot.ranks['TNE'] + 1

ot.جوڑو_language ('und-fonipa', 'IPPH')

ot.جوڑو_language ('und-fonnapa', 'APPH')

ot.جوڑو_language ('und-fonupa', 'UPPH')

ot.جوڑو_language ('ga-Latg', 'IRT')

ot.جوڑو_language ('hy-arevmda', 'HYE')

ot.remove_language_ot ('KGE')
ot.جوڑو_language ('und-Geok', 'KGE')

ot.جوڑو_language ('kht', 'KHN')
ot.نام['KHN'] = ot.نام['KHT'] + ' (Microsoft fonts)'
ot.ranks['KHN'] = ot.ranks['KHT'] + 1

ot.ranks['LCR'] = ot.ranks['MCR'] + 1

ot.نام['MAL'] = 'Malayalam Traditional'
ot.ranks['MLR'] += 1

bcp_47.نام['mhv'] = 'Arakanese'
bcp_47.scopes['mhv'] = ' (retired code)'

ot.جوڑو_language ('mnw-TH', 'MONT')

ot.جوڑو_language ('no', 'NOR')

ot.جوڑو_language ('oc-provenc', 'PRO')

ot.remove_language_ot ('QUZ')
ot.جوڑو_language ('qu', 'QUZ')
ot.جوڑو_language ('qub', 'QWH')
ot.جوڑو_language ('qud', 'QVI')
ot.جوڑو_language ('qug', 'QVI')
ot.جوڑو_language ('qul', 'QUH')
ot.جوڑو_language ('qup', 'QVI')
ot.جوڑو_language ('qur', 'QWH')
ot.جوڑو_language ('qus', 'QUH')
ot.جوڑو_language ('quw', 'QVI')
ot.جوڑو_language ('qux', 'QWH')
ot.جوڑو_language ('qva', 'QWH')
ot.جوڑو_language ('qvh', 'QWH')
ot.جوڑو_language ('qvj', 'QVI')
ot.جوڑو_language ('qvl', 'QWH')
ot.جوڑو_language ('qvm', 'QWH')
ot.جوڑو_language ('qvn', 'QWH')
ot.جوڑو_language ('qvo', 'QVI')
ot.جوڑو_language ('qvp', 'QWH')
ot.جوڑو_language ('qvw', 'QWH')
ot.جوڑو_language ('qvz', 'QVI')
ot.جوڑو_language ('qwa', 'QWH')
ot.جوڑو_language ('qws', 'QWH')
ot.جوڑو_language ('qxa', 'QWH')
ot.جوڑو_language ('qxc', 'QWH')
ot.جوڑو_language ('qxh', 'QWH')
ot.جوڑو_language ('qxl', 'QVI')
ot.جوڑو_language ('qxn', 'QWH')
ot.جوڑو_language ('qxo', 'QWH')
ot.جوڑو_language ('qxr', 'QVI')
ot.جوڑو_language ('qxt', 'QWH')
ot.جوڑو_language ('qxw', 'QWH')

bcp_47.macrolanguages['ro-MD'].جوڑو ('mo')

ot.remove_language_ot ('SYRE')
ot.remove_language_ot ('SYRJ')
ot.remove_language_ot ('SYRN')
ot.جوڑو_language ('und-Syre', 'SYRE')
ot.جوڑو_language ('und-Syrj', 'SYRJ')
ot.جوڑو_language ('und-Syrn', 'SYRN')

bcp_47.نام['xst'] = "Silt'e"
bcp_47.scopes['xst'] = ' (retired code)'
bcp_47.macrolanguages['xst'] = {'stv', 'wle'}

ot.جوڑو_language ('xwo', 'TOD')

ot.remove_language_ot ('ZHH')
ot.remove_language_ot ('ZHP')
ot.remove_language_ot ('ZHT')
ot.remove_language_ot ('ZHTM')
bcp_47.macrolanguages['zh'].remove ('lzh')
bcp_47.macrolanguages['zh'].remove ('yue')
ot.جوڑو_language ('zh-Hant-MO', 'ZHH')
ot.جوڑو_language ('zh-Hant-MO', 'ZHTM')
ot.جوڑو_language ('zh-Hant-HK', 'ZHH')
ot.جوڑو_language ('zh-Hans', 'ZHS')
ot.جوڑو_language ('zh-Hant', 'ZHT')
ot.جوڑو_language ('zh-HK', 'ZHH')
ot.جوڑو_language ('zh-MO', 'ZHH')
ot.جوڑو_language ('zh-MO', 'ZHTM')
ot.جوڑو_language ('zh-TW', 'ZHT')
ot.جوڑو_language ('lzh', 'ZHT')
ot.جوڑو_language ('lzh-Hans', 'ZHS')
ot.جوڑو_language ('yue', 'ZHH')
ot.جوڑو_language ('yue-Hans', 'ZHS')

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
	اگر bcp_47 == 'ak' اور ot == 'AKA':
		واپس -1
	اگر bcp_47 == 'tw' اور ot == 'TWI':
		واپس -1
	واپس 0

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
ot.نام[DEFAULT_LANGUAGE_SYSTEM] = ''
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.اقدار ()) + 1
برائے tricky_ot_لیبل میں filter (بےنام لیبل_2: re.مطابقت ('[A-Z]{3}$', لیبل_2), ot.نام):
	possible_bcp_47_لیبل = tricky_ot_لیبل.زیریں ()
	اگر possible_bcp_47_لیبل میں bcp_47.نام اور نہیں ot.from_bcp_47[possible_bcp_47_لیبل]:
		ot.جوڑو_language (possible_bcp_47_لیبل, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_لیبل] = سیٹ ()
ot.ترتیب_دو_languages ()

اگر output_rust:
	تعریف hb_لیبل_rust (لیبل_2):
		اگر لیبل_2 == DEFAULT_LANGUAGE_SYSTEM:
			واپس 'Tag::new(&[0; 4])'
		sanitize_لیبل (لیبل_2)
		واپس 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % لیبل_2)[:4])

	تعریف get_variant_سیٹ_rust (name):
		واپس سیٹ (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				برائے n میں re.بانٹو ('[\n(),]', name) اگر n)

	تعریف language_name_intersection_rust (a, b):
		واپس get_variant_سیٹ_rust (a).intersection (get_variant_سیٹ_rust (b))

	تعریف get_matching_language_name_rust (intersection, candidates):
		واپس اگلا (iter (c برائے c میں candidates اگر نہیں intersection.isdisjoint (get_variant_سیٹ_rust (c))))

	چھاپو ('// WARNING: this file was generated by ../../harfbuzz/src/gen-tag-table.py --rust')
	چھاپو ()
	چھاپو ('use read_fonts::types::Tag;')
	چھاپو ()
	چھاپو ('pub(crate) struct LangTag {')
	چھاپو ('    pub language: [u8; 4],')
	چھاپو ('    pub tag: Tag,')
	چھاپو ('}')
	چھاپو ()
	چھاپو ('const fn lang(language: &[u8]) -> [u8; 4] {')
	چھاپو ('    let mut bytes = [0; 4];')
	چھاپو ('    let mut i = 0;')
	چھاپو ()
	چھاپو ('    while i < language.len() {')
	چھاپو ('        bytes[i] = language[i];')
	چھاپو ('        i += 1;')
	چھاپو ('    }')
	چھاپو ()
	چھاپو ('    bytes')
	چھاپو ('}')
	چھاپو ()
	چھاپو ('use super::tag::{lang_matches, strncmp, subtag_matches};')
	چھاپو ()
	چھاپو ('#[rustfmt::skip]')
	چھاپو ('pub(crate) static OPEN_TYPE_LANGUAGES: &[LangTag] = &[')

	برائے language, لیبل_فہرست میں sorted (ot.from_bcp_47.اشیاء ()):
		اگر language == '' یا '-' میں language:
			جاری
		commented_برآمد = لمبائی (language) == 3 اور لمبائی (لیبل_فہرست) == 1 اور language == لیبل_فہرست[0].زیریں ()
		برائے لیبل_2 میں لیبل_فہرست:
			چھاپو ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' اگر commented_برآمد ورنہ '    ', sanitize_bcp47_لیبل (language), hb_لیبل_rust (لیبل_2)), انتہا='')
			چھاپو (' // ', انتہا='')
			bcp_47_name = bcp_47.نام.get (language, '')
			bcp_47_name_candidates = bcp_47_name.بانٹو ('\n')
			ot_name = ot.نام[لیبل_2]
			scope = bcp_47.scopes.get (language, '')
			اگر لیبل_2 == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			ورنہ:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				اگر نہیں intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				ورنہ:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.نام[language] = name
					comment = '%s%s' % (name اگر لمبائی (name) > لمبائی (ot_name) ورنہ ot_name, scope)
			چھاپو (sanitize_comment (comment))

	چھاپو ('];')
	چھاپو ()
	چھاپو ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	چھاپو ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	تعریف print_subtag_matches_rust (subtag, new_سطر):
		اگر subtag:
			اگر new_سطر:
				چھاپو (' && ', انتہا='')
			چھاپو ('subtag_matches(language, "-%s")' % sanitize_bcp47_لیبل (subtag), انتہا='')

	complex_لیبل_فہرست = collections.defaultdict (list)
	برائے initial, گروہ میں itertools.groupby ((lt_لیبل_فہرست برائے lt_لیبل_فہرست میں [
				(Language_لیبل (language), لیبل_فہرست)
				برائے language, لیبل_فہرست میں sorted (ot.from_bcp_47.اشیاء (),
					کلید_2=بےنام i: (-لمبائی (i[0]), i[0]))
			] اگر lt_لیبل_فہرست[0].is_complex ()),
			کلید_2=بےنام lt_لیبل_فہرست: lt_لیبل_فہرست[0].get_گروہ ()):
		complex_لیبل_فہرست[initial] += گروہ

	برائے initial, اشیاء میں sorted (complex_لیبل_فہرست.اشیاء ()):
		اگر initial != 'und':
			جاری
		برائے lt, لیبل_فہرست میں اشیاء:
			اگر نہیں لیبل_فہرست:
				جاری
			اگر lt.variant میں bcp_47.prefixes:
				expect (اگلا (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			چھاپو ('    if ', انتہا='')
			print_subtag_matches_rust (lt.script, جھوٹ)
			print_subtag_matches_rust (lt.region, جھوٹ)
			print_subtag_matches_rust (lt.variant, جھوٹ)
			چھاپو (' {')
			چھاپو ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			اگر لمبائی (لیبل_فہرست) == 1:
				چھاپو ('        tags.push(%s); // %s' % (hb_لیبل_rust (لیبل_فہرست[0]), sanitize_comment (ot.نام[لیبل_فہرست[0]])))
			ورنہ:
				چھاپو ('        let possible_tags = &[')
				برائے لیبل_2 میں لیبل_فہرست:
					چھاپو ('            %s, // %s' % (hb_لیبل_rust (لیبل_2), sanitize_comment (ot.نام[لیبل_2])))
				چھاپو ('        ];')
				چھاپو ('        tags.extend_from_slice(possible_tags);')
			چھاپو ('        return true;')
			چھاپو ('    }')

	چھاپو ('    match language.as_bytes()[0] {')
	برائے initial, اشیاء میں sorted (complex_لیبل_فہرست.اشیاء ()):
		اگر initial == 'und':
			جاری
		چھاپو ("        b'%s' => {" % sanitize_bcp47_لیبل (initial))
		برائے lt, لیبل_فہرست میں اشیاء:
			اگر نہیں لیبل_فہرست:
				جاری
			چھاپو ('            if ', انتہا='')
			script = lt.script
			region = lt.region
			اگر lt.grandfathered:
				چھاپو ('&language[1..] == "%s"' % sanitize_bcp47_لیبل (lt.language[1:]), انتہا='')
			ورنہ:
				string_literal = lt.language[1:] + '-'
				اگر script:
					string_literal += script
					script = کچھ_نہیں
					اگر region:
						string_literal += '-' + region
						region = کچھ_نہیں
				اگر string_literal[-1] == '-':
					چھاپو ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_لیبل (string_literal), لمبائی (string_literal)), انتہا='')
				ورنہ:
					چھاپو ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_لیبل (string_literal), انتہا='')
			print_subtag_matches_rust (script, سچ)
			print_subtag_matches_rust (region, سچ)
			print_subtag_matches_rust (lt.variant, سچ)
			چھاپو (' {')
			چھاپو ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			اگر لمبائی (لیبل_فہرست) == 1:
				چھاپو ('                tags.push(%s); // %s' % (hb_لیبل_rust (لیبل_فہرست[0]), sanitize_comment (ot.نام[لیبل_فہرست[0]])))
			ورنہ:
				چھاپو ('                let possible_tags = &[')
				برائے لیبل_2 میں لیبل_فہرست:
					چھاپو ('                    %s, // %s' % (hb_لیبل_rust (لیبل_2), sanitize_comment (ot.نام[لیبل_2])))
				چھاپو ('                ];')
				چھاپو ('                tags.extend_from_slice(possible_tags);')
			چھاپو ('                return true;')
			چھاپو ('            }')
		چھاپو ('        }')
	چھاپو ('        _ => {}')
	چھاپو ('    }')
	چھاپو ('    false')
	چھاپو ('}')
	sys.exit (0)

چھاپو ('/* == Start of generated table == */')
چھاپو ('/*')
چھاپو (' * The following table is generated by running:')
چھاپو (' *')
چھاپو (' *   %s languagetags language-subtag-registry' % os.راستہ.basename (sys.argv[0]))
چھاپو (' *')
چھاپو (' * on files with these headers:')
چھاپو (' *')
چھاپو (' * %s' % sanitize_سرخی (ot.سرخی))
چھاپو (' * %s' % sanitize_سرخی (bcp_47.سرخی))
چھاپو (' */')
چھاپو ()
چھاپو ('#ifndef HB_OT_TAG_TABLE_HH')
چھاپو ('#define HB_OT_TAG_TABLE_HH')
چھاپو ()

تعریف hb_لیبل (لیبل_2):
	"""Convert a tag to ``HB_TAG`` form.

	Args:
		tag (str): An OpenType tag.

	Returns:
		A snippet of C++ representing ``tag``.
	"""
	اگر لیبل_2 == DEFAULT_LANGUAGE_SYSTEM:
		واپس 'HB_TAG_NONE\t       '
	sanitize_لیبل (لیبل_2)
	واپس "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % لیبل_2)[:4])

تعریف get_variant_سیٹ (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	واپس سیٹ (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			برائے n میں re.بانٹو ('[\n(),]', name) اگر n)

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
	واپس get_variant_سیٹ (a).intersection (get_variant_سیٹ (b))

تعریف get_matching_language_name (intersection, candidates):
	واپس اگلا (iter (c برائے c میں candidates اگر نہیں intersection.isdisjoint (get_variant_سیٹ (c))))

تعریف same_لیبل (bcp_47_لیبل, ot_لیبل_فہرست):
	واپس لمبائی (bcp_47_لیبل) == 3 اور لمبائی (ot_لیبل_فہرست) == 1 اور bcp_47_لیبل == ot_لیبل_فہرست[0].زیریں ()

تعریف format_language_comment (language, لیبل_2):
	bcp_47_name = bcp_47.نام.get (language, '')
	bcp_47_name_candidates = bcp_47_name.بانٹو ('\n')
	ot_name = ot.نام[لیبل_2]
	scope = bcp_47.scopes.get (language, '')
	اگر لیبل_2 == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	ورنہ:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		اگر نہیں intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		ورنہ:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.نام[language] = name
			comment = '%s%s' % (name اگر لمبائی (name) > لمبائی (ot_name) ورنہ ot_name, scope)
	واپس sanitize_comment (comment)

چھاپو ('static const LangTag ot_languages2[] = {')
برائے language, لیبل_فہرست میں sorted (ot.from_bcp_47.اشیاء ()):
	اگر language == '' یا '-' میں language یا لمبائی (language) != 2:
		جاری
	برائے لیبل_2 میں لیبل_فہرست:
		چھاپو ('  {%s,\t%s},\t/* %s */' % (
			hb_لیبل (language),
			hb_لیبل (لیبل_2),
			format_language_comment (language, لیبل_2)))
چھاپو ('};')
چھاپو ()

چھاپو ('#ifndef HB_NO_LANGUAGE_LONG')

languages3_blocked = []
languages3_singles = []
languages3_multi_ranges = []
languages3_multi_اقدار = []
برائے language, لیبل_فہرست میں sorted (ot.from_bcp_47.اشیاء ()):
	اگر language == '' یا '-' میں language یا لمبائی (language) != 3:
		جاری
	اگر same_لیبل (language, لیبل_فہرست):
		جاری
	اگر لیبل_فہرست == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, لیبل_فہرست[0]))
		جاری
	اگر لمبائی (لیبل_فہرست) == 1:
		languages3_singles.append ((language, لیبل_فہرست[0]))
		جاری
	انحراف = لمبائی (languages3_multi_اقدار)
	languages3_multi_اقدار.extend ((language, لیبل_2) برائے لیبل_2 میں لیبل_فہرست)
	languages3_multi_ranges.append ((language, انحراف, لمبائی (لیبل_فہرست), لیبل_فہرست[0]))

چھاپو ('static const hb_tag_t ot_languages3_blocked[] = {')
برائے language, لیبل_2 میں languages3_blocked:
	چھاپو ('  %s,\t/* %s */' % (hb_لیبل (language), format_language_comment (language, لیبل_2)))
چھاپو ('};')
چھاپو ()

چھاپو ('static const LangTag ot_languages3[] = {')
برائے language, لیبل_2 میں languages3_singles:
	چھاپو ('  {%s,\t%s},\t/* %s */' % (
		hb_لیبل (language),
		hb_لیبل (لیبل_2),
		format_language_comment (language, لیبل_2)))
چھاپو ('};')
چھاپو ()

چھاپو ('static const hb_tag_t ot_languages3_multi_values[] = {')
برائے language, لیبل_2 میں languages3_multi_اقدار:
	چھاپو ('  %s,\t/* %s */' % (hb_لیبل (لیبل_2), format_language_comment (language, لیبل_2)))
چھاپو ('};')
چھاپو ()

چھاپو ('static const LangTagRange ot_languages3_multi[] = {')
برائے language, انحراف, گنتی, پہلا_لیبل میں languages3_multi_ranges:
	چھاپو ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_لیبل (language),
		صحیح(انحراف),
		صحیح(گنتی),
		format_language_comment (language, پہلا_لیبل)))
چھاپو ('};')
چھاپو ('#endif')
چھاپو ()

چھاپو ('/**')
چھاپو (' * hb_ot_tags_from_complex_language:')
چھاپو (' * @lang_str: a BCP 47 language tag to convert.')
چھاپو (' * @limit: a pointer to the end of the substring of @lang_str to consider for')
چھاپو (' * conversion.')
چھاپو (' * @count: maximum number of language tags to retrieve (IN) and actual number of')
چھاپو (' * language tags retrieved (OUT). If no tags are retrieved, it is not modified.')
چھاپو (' * @tags: array of size at least @language_count to store the language tag')
چھاپو (' * results')
چھاپو (' *')
چھاپو (' * Converts a multi-subtag BCP 47 language tag to language tags.')
چھاپو (' *')
چھاپو (' * Return value: Whether any language systems were retrieved.')
چھاپو (' **/')
چھاپو ('static inline bool')
چھاپو ('hb_ot_tags_from_complex_language (const char   *lang_str,')
چھاپو ('\t\t\t\t  const char   *limit,')
چھاپو ('\t\t\t\t  unsigned int *count /* IN/OUT */,')
چھاپو ('\t\t\t\t  hb_tag_t     *tags /* OUT */)')
چھاپو ('{')

تعریف print_subtag_matches (subtag, string, new_سطر):
	اگر subtag:
		اگر new_سطر:
			چھاپو ()
			چھاپو ('\t&& ', انتہا='')
		چھاپو ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_لیبل (subtag), 1 + لمبائی (subtag)), انتہا='')

complex_لیبل_فہرست = collections.defaultdict (list)
برائے initial, گروہ میں itertools.groupby ((lt_لیبل_فہرست برائے lt_لیبل_فہرست میں [
			(Language_لیبل (language), لیبل_فہرست)
			برائے language, لیبل_فہرست میں sorted (ot.from_bcp_47.اشیاء (),
				کلید_2=بےنام i: (-لمبائی (i[0]), i[0]))
		] اگر lt_لیبل_فہرست[0].is_complex ()),
		کلید_2=بےنام lt_لیبل_فہرست: lt_لیبل_فہرست[0].get_گروہ ()):
	complex_لیبل_فہرست[initial] += گروہ

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
برائے initial, اشیاء میں sorted (complex_لیبل_فہرست.اشیاء ()):
	اگر initial != 'und':
		جاری
	برائے lt, لیبل_فہرست میں اشیاء:
		اگر نہیں لیبل_فہرست:
			جاری
		subtag_len = 0
		subtag_len += 1 + لمبائی (lt.script) اگر lt.script ہے نہیں کچھ_نہیں ورنہ 0
		subtag_len += 1 + لمبائی (lt.region) اگر lt.region ہے نہیں کچھ_نہیں ورنہ 0
		subtag_len += 1 + لمبائی (lt.variant) اگر lt.variant ہے نہیں کچھ_نہیں ورنہ 0
		min_subtag_len = min(subtag_len, min_subtag_len)

چھاپو ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
چھاپو ('  {')
چھاپو ("    const char *p = strchr (lang_str, '-');")
چھاپو ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
برائے initial, اشیاء میں sorted (complex_لیبل_فہرست.اشیاء ()):
	اگر initial != 'und':
		جاری
	برائے lt, لیبل_فہرست میں اشیاء:
		اگر نہیں لیبل_فہرست:
			جاری
		اگر lt.variant میں bcp_47.prefixes:
			expect (اگلا (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		چھاپو ('    if (', انتہا='')
		print_subtag_matches (lt.script, 'p', جھوٹ)
		print_subtag_matches (lt.region, 'p', جھوٹ)
		print_subtag_matches (lt.variant, 'p', جھوٹ)
		چھاپو (')')
		چھاپو ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		چھاپو ()
		اگر لمبائی (لیبل_فہرست) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_لیبل (لیبل_فہرست[0]), sanitize_comment (ot.نام[لیبل_فہرست[0]])))
			چھاپو ()
			چھاپو ('      *count = 1;')
		ورنہ:
			چھاپو ('    hb_tag_t possible_tags[] = {')
			برائے لیبل_2 میں لیبل_فہرست:
				اكتب ('      %s,  /* %s */' % (hb_لیبل (لیبل_2), sanitize_comment (ot.نام[لیبل_2])))
				چھاپو ()
			چھاپو ('      };')
			چھاپو ('      for (i = 0; i < %s && i < *count; i++)' % لمبائی (لیبل_فہرست))
			چھاپو ('\ttags[i] = possible_tags[i];')
			چھاپو ('      *count = i;')
		چھاپو ('      return true;')
		چھاپو ('    }')
چھاپو ('  }')
چھاپو ('out:')

چھاپو ('  switch (lang_str[0])')
چھاپو ('  {')
برائے initial, اشیاء میں sorted (complex_لیبل_فہرست.اشیاء ()):
	اگر initial == 'und':
		جاری
	چھاپو ("  case '%s':" % sanitize_bcp47_لیبل (initial))
	برائے lt, لیبل_فہرست میں اشیاء:
		اگر نہیں لیبل_فہرست:
			جاری
		چھاپو ('    if (', انتہا='')
		script = lt.script
		region = lt.region
		اگر lt.grandfathered:
			چھاپو ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_لیبل (lt.language[1:]), انتہا='')
		ورنہ:
			string_literal = lt.language[1:] + '-'
			اگر script:
				string_literal += script
				script = کچھ_نہیں
				اگر region:
					string_literal += '-' + region
					region = کچھ_نہیں
			اگر string_literal[-1] == '-':
				چھاپو ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_لیبل (string_literal), لمبائی (string_literal)), انتہا='')
			ورنہ:
				چھاپو ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_لیبل (string_literal), لمبائی (string_literal)), انتہا='')
		print_subtag_matches (script, 'lang_str', سچ)
		print_subtag_matches (region, 'lang_str', سچ)
		print_subtag_matches (lt.variant, 'lang_str', سچ)
		چھاپو (')')
		چھاپو ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		چھاپو ()
		اگر لمبائی (لیبل_فہرست) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_لیبل (لیبل_فہرست[0]), sanitize_comment (ot.نام[لیبل_فہرست[0]])))
			چھاپو ()
			چھاپو ('      *count = 1;')
		ورنہ:
			چھاپو ('      unsigned int i;')
			چھاپو ('      hb_tag_t possible_tags[] = {')
			برائے لیبل_2 میں لیبل_فہرست:
				اكتب ('\t%s,  /* %s */' % (hb_لیبل (لیبل_2), sanitize_comment (ot.نام[لیبل_2])))
				چھاپو ()
			چھاپو ('      };')
			چھاپو ('      for (i = 0; i < %s && i < *count; i++)' % لمبائی (لیبل_فہرست))
			چھاپو ('\ttags[i] = possible_tags[i];')
			چھاپو ('      *count = i;')
		چھاپو ('      return true;')
		چھاپو ('    }')
	چھاپو ('    break;')

چھاپو ('  }')
چھاپو ('  return false;')
چھاپو ('}')
چھاپو ()
چھاپو ('/**')
چھاپو (' * hb_ot_ambiguous_tag_to_language')
چھاپو (' * @tag: A language tag.')
چھاپو (' *')
چھاپو (' * Converts @tag to a BCP 47 language tag if it is ambiguous (it corresponds to')
چھاپو (' * many language tags) and the best tag is not the first (sorted alphabetically,')
چھاپو (' * with two-letter tags having priority over all three-letter tags), or if the')
چھاپو (' * best tag consists of multiple subtags, or if the best tag does not appear in')
چھاپو (' * #ot_languages2 or #ot_languages3.')
چھاپو (' *')
چھاپو (' * Return value: The #hb_language_t corresponding to the BCP 47 language tag,')
چھاپو (' * or #HB_LANGUAGE_INVALID if @tag is not ambiguous.')
چھاپو (' **/')
چھاپو ('static inline hb_language_t')
چھاپو ('hb_ot_ambiguous_tag_to_language (hb_tag_t tag)')
چھاپو ('{')
چھاپو ('  switch (tag)')
چھاپو ('  {')

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
	برائے ot_لیبل, bcp_47_لیبل_فہرست میں ot.مقصد_bcp_47.اشیاء ():
		اگر ot_لیبل == DEFAULT_LANGUAGE_SYSTEM:
			primary_لیبل_فہرست = []
		ورنہ:
			primary_لیبل_فہرست = list (t برائے t میں bcp_47_لیبل_فہرست اگر t نہیں میں bcp_47.grandfathered اور ot.from_bcp_47.get (t)[0] == ot_لیبل)
		اگر لمبائی (primary_لیبل_فہرست) == 1:
			expect (ot_لیبل نہیں میں disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_لیبل)
			اگر '-' میں primary_لیبل_فہرست[0]:
				disambiguation[ot_لیبل] = primary_لیبل_فہرست[0]
			ورنہ:
				پہلا_لیبل = sorted ((t برائے t میں bcp_47_لیبل_فہرست اگر t نہیں میں bcp_47.grandfathered اور ot_لیبل میں ot.from_bcp_47.get (t)),
						کلید_2=بےنام t: (لمبائی (t), t))[0]
				اگر primary_لیبل_فہرست[0] != پہلا_لیبل:
					disambiguation[ot_لیبل] = primary_لیبل_فہرست[0]
		وگرنہ_اگر لمبائی (primary_لیبل_فہرست) == 0:
			expect (ot_لیبل نہیں میں disambiguation, 'There is no possible valid disambiguation for %s' % ot_لیبل)
		ورنہ:
			original_languages = [t برائے t میں primary_لیبل_فہرست اگر t میں ot.from_bcp_47_uninherited اور 'retired code' نہیں میں bcp_47.scopes.get (t, '')]
			اگر لمبائی (original_languages) == 1:
				macrolanguages = original_languages
			ورنہ:
				macrolanguages = [t برائے t میں primary_لیبل_فہرست اگر bcp_47.scopes.get (t) == ' [macrolanguage]']
			اگر لمبائی (macrolanguages) != 1:
				macrolanguages = list (t برائے t میں primary_لیبل_فہرست اگر bcp_47.scopes.get (t) == ' [collection]')
			اگر لمبائی (macrolanguages) != 1:
				macrolanguages = list (t برائے t میں primary_لیبل_فہرست اگر 'retired code' نہیں میں bcp_47.scopes.get (t, ''))
			اگر لمبائی (macrolanguages) != 1:
				macrolanguages = list (t برائے t میں primary_لیبل_فہرست اگر t.زیریں () == ISO_639_3_مقصد_1.get (ot_لیبل.زیریں (), ot_لیبل.زیریں ()))
			اگر لمبائی (macrolanguages) != 1:
				macrolanguages = list (t برائے t میں primary_لیبل_فہرست اگر '-' نہیں میں t)
			اگر لمبائی (macrolanguages) != 1:
				expect (ot_لیبل میں disambiguation, 'ambiguous OT tag: %s %s' % (ot_لیبل, sorted (primary_لیبل_فہرست)))
				expect (disambiguation[ot_لیبل] میں bcp_47_لیبل_فہرست,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_لیبل], ot_لیبل))
			وگرنہ_اگر ot_لیبل نہیں میں disambiguation:
				disambiguation[ot_لیبل] = macrolanguages[0]
			اگر '-' نہیں میں disambiguation[ot_لیبل]:
				different_bcp_47_لیبل_فہرست = sorted ((t برائے t میں bcp_47_لیبل_فہرست اگر نہیں same_لیبل (t, ot.from_bcp_47.get (t))),
						کلید_2=بےنام t: (لمبائی (t), t))
				اگر different_bcp_47_لیبل_فہرست اور disambiguation[ot_لیبل] == different_bcp_47_لیبل_فہرست[0]:
					مٹاؤ disambiguation[ot_لیبل]
	برائے ot_لیبل میں disambiguation.کلیدیں ():
		expect (ot_لیبل میں ot.مقصد_bcp_47, 'unknown OT tag: %s' % ot_لیبل)

verify_disambiguation_dict ()
برائے ot_لیبل, bcp_47_لیبل میں sorted (disambiguation.اشیاء ()):
	اكتب ('  case %s:  /* %s */' % (hb_لیبل (ot_لیبل), sanitize_comment (ot.نام[ot_لیبل])))
	چھاپو ()
	اكتب ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_لیبل (bcp_47_لیبل), sanitize_comment (bcp_47.get_name (Language_لیبل (bcp_47_لیبل)))))
	چھاپو ()

چھاپو ('  default:')
چھاپو ('    return HB_LANGUAGE_INVALID;')
چھاپو ('  }')
چھاپو ('}')

چھاپو ()
چھاپو ('#endif /* HB_OT_TAG_TABLE_HH */')
چھاپو ()
چھاپو ('/* == End of generated table == */')
