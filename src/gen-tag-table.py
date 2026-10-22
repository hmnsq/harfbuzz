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

आयात collections
आयात html
तः html.parser आयात HTMLParser
आयात itertools
आयात os
आयात re
आयात sys
आयात unicodedata

जर len (sys.argv) > 1 आणि sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = सत्य
अन्यथा:
	output_rust = असत्य

जर len (sys.argv) != 3:
	sys.exit (__doc__)

def expect (condition, message=None):
	जर नाही condition:
		जर message is None:
			उठवा AssertionError
		उठवा AssertionError (message)

def लिख (s):
	sys.stdout.flush ()
	sys.stdout.buffer.लिख (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = set (" -—–.,;:’'ʼ()[]/!=>_+")

def sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	जर नाही isinstance (s, str):
		उठवा TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	जर नाही s:
		परत ''
	साठी ch मध्ये s:
		cat = unicodedata.category (ch)
		जर नाही (cat.startswith (('L', 'M', 'N', 'Z')) किंवा ch मध्ये _ALLOWED_COMMENT_PUNCT):
			उठवा मूल्य_Error ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	परत s

def sanitize_header (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	जर नाही isinstance (s, str):
		उठवा TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	जर नाही re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		उठवा मूल्य_Error ("Invalid header line %r: contains disallowed characters" % s)
	परत s

def sanitize_bcp47_tag (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	जर नाही isinstance (s, str):
		उठवा TypeError ("Expected string, got %s" % type (s).__name__)
	जर नाही re.fullmatch (r'[a-zA-Z0-9-]+', s):
		उठवा मूल्य_Error ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	परत s

def sanitize_tag (tag):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	जर नाही isinstance (tag, str):
		उठवा TypeError ("Expected string for tag, got %s" % type (tag).__name__)
	जर tag == DEFAULT_LANGUAGE_SYSTEM:
		परत ''
	जर नाही re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', tag):
		उठवा मूल्य_Error ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % tag)
	परत tag

DEFAULT_LANGUAGE_SYSTEM = ''

# from https://www-01.sil.org/iso639-3/iso-639-3.tab
ISO_639_3_TO_1 = {
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

वर्ग LanguageTag (object):
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
	def __init__ (self, tag):
		वैश्विक bcp_47
		self.subtags = tag.lower ().split ('-')
		self.grandfathered = tag.lower () मध्ये bcp_47.grandfathered
		जर self.grandfathered:
			self.language = tag.lower ()
			self.script = ''
			self.region = ''
			self.variant = ''
		अन्यथा:
			self.language = self.subtags[0]
			self.script = self._find_first (निनावी s: len (s) == 4 आणि s[0] > '9', self.subtags)
			self.region = self._find_first (निनावी s: len (s) == 2 आणि s[0] > '9' किंवा len (s) == 3 आणि s[0] <= '9', self.subtags[1:])
			self.variant = self._find_first (निनावी s: len (s) > 4 किंवा len (s) == 4 आणि s[0] <= '9', self.subtags)

	def __str__(self):
		परत '-'.join(self.subtags)

	def __repr__ (self):
		परत 'LanguageTag(%r)' % str(self)

	@staticmethod
	def _find_first (function, sequence):
		प्रयत्न:
			परत next (iter (filter (function, sequence)))
		अपवाद StopIteration:
			परत None

	def is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		परत नाही (len (self.subtags) == 1
			किंवा self.grandfathered
			आणि len (self.subtags[1]) != 3
			आणि ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	def get_group (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		परत ('und'
			जर (self.language == 'und'
				किंवा self.variant मध्ये bcp_47.prefixes आणि len (bcp_47.prefixes[self.variant]) == 1)
			अन्यथा self.language[0])

वर्ग OpenTypeRegistryParser (HTMLParser):
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
	def __init__ (self):
		HTMLParser.__init__ (self)
		self.header = ''
		self.names = {}
		self.ranks = collections.defaultdict (पूर्णांक)
		self.to_bcp_47 = collections.defaultdict (set)
		self.from_bcp_47 = collections.defaultdict (set)
		self.from_bcp_47_uninherited = None
		# Whether the parser is in a <td> element
		self._td = असत्य
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = असत्य
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	def handle_starttag (self, tag, attrs):
		जर tag == 'a':
			जर self._current_tr आणि नाही self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = सत्य
		किंवाजर tag == 'br':
			self._disengaged = सत्य
		किंवाजर tag == 'meta':
			साठी attr, मूल्य_2 मध्ये attrs:
				जर attr == 'name' आणि मूल्य_2 == 'updated_at':
					self.header = self.get_starttag_text ()
					थांबा
		किंवाजर tag == 'td':
			self._td = सत्य
			self._current_tr.append ('')
		किंवाजर tag == 'tr':
			self._disengaged = असत्य
			self._current_tr = []

	def handle_endtag (self, tag):
		जर tag == 'td':
			self._td = असत्य
			self._disengaged = असत्य
		किंवाजर tag == 'tr' आणि self._current_tr:
			expect (2 <= len (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			tag = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			जर len (tag) > 4:
				expect (tag.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % tag)
				name += ' (deprecated)'
				tag = tag.split (' ')[0]
				rank = 1
			self.names[tag] = re.sub (' languages$', '', name)
			जर नाही self._current_tr[2]:
				परत
			iso_codes = self._current_tr[2].strip ()
			self.to_bcp_47[tag].update (ISO_639_3_TO_1.get (code, code) साठी code मध्ये iso_codes.replace (' ', '').split (','))
			rank += 2 * len (self.to_bcp_47[tag])
			self.ranks[tag] = rank

	def handle_data (self, data):
		जर self._td आणि नाही self._disengaged:
			self._current_tr[-1] += data

	def handle_charref (self, name):
		self.handle_data (html.unescape ('&#%s;' % name))

	def handle_entityref (self, name):
		self.handle_data (html.unescape ('&%s;' % name))

	def parse (self, filename):
		"""Parse the OpenType language system tag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		with open (filename, encoding='utf-8') इव f:
			self.feed (f.पठ ())
		expect (self.header)
		साठी tag, iso_codes मध्ये self.to_bcp_47.वस्तुसंच ():
			साठी iso_code मध्ये iso_codes:
				self.from_bcp_47[iso_code].add (tag)

	def add_language (self, bcp_47_tag, ot_tag):
		"""Add a language as if it were in the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag. If the tag is more than just
				a language subtag, and if the language subtag is a
				macrolanguage, then new languages are added corresponding
				to the macrolanguages' individual languages with the
				remainder of the tag appended.
			ot_tag (str): An OpenType language system tag.
		"""
		वैश्विक bcp_47
		self.to_bcp_47[ot_tag].add (bcp_47_tag)
		self.from_bcp_47[bcp_47_tag].add (ot_tag)
		जर bcp_47_tag.lower () नाही मध्ये bcp_47.grandfathered:
			प्रयत्न:
				[macrolanguage, suffix] = bcp_47_tag.split ('-', 1)
				जर macrolanguage मध्ये bcp_47.macrolanguages:
					s = set ()
					साठी language मध्ये bcp_47.macrolanguages[macrolanguage]:
						जर language.lower () नाही मध्ये bcp_47.grandfathered:
							s.add ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			अपवाद मूल्य_Error:
				pass

	@staticmethod
	def _remove_language (tag_1, dict_1, dict_2):
		साठी tag_2 मध्ये dict_1.काढा (tag_1):
			dict_2[tag_2].remove (tag_1)
			जर नाही dict_2[tag_2]:
				del dict_2[tag_2]

	def remove_language_ot (self, ot_tag):
		"""Remove an OpenType tag from the registry.

		Args:
			ot_tag (str): An OpenType tag.
		"""
		self._remove_language (ot_tag, self.to_bcp_47, self.from_bcp_47)

	def remove_language_bcp_47 (self, bcp_47_tag):
		"""Remove a BCP 47 tag from the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag.
		"""
		self._remove_language (bcp_47_tag, self.from_bcp_47, self.to_bcp_47)

	def inherit_from_macrolanguages (self):
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
		वैश्विक bcp_47
		first_time = self.from_bcp_47_uninherited is None
		जर first_time:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		साठी macrolanguage, languages मध्ये dict (bcp_47.macrolanguages).वस्तुसंच ():
			ot_macrolanguages = {
				ot_macrolanguage साठी ot_macrolanguage मध्ये self.from_bcp_47_uninherited.get (macrolanguage, set ())
			}
			blocked_ot_macrolanguages = set ()
			जर 'retired code' नाही मध्ये bcp_47.scopes.get (macrolanguage, ''):
				साठी ot_macrolanguage मध्ये ot_macrolanguages:
					round_trip_macrolanguages = {
						l साठी l मध्ये self.to_bcp_47[ot_macrolanguage]
						जर 'retired code' नाही मध्ये bcp_47.scopes.get (l, '')
					}
					round_trip_languages = {
						l साठी l मध्ये languages
						जर 'retired code' नाही मध्ये bcp_47.scopes.get (l, '')
					}
					intersection = round_trip_macrolanguages & round_trip_languages
					जर intersection आणि intersection != round_trip_languages:
						blocked_ot_macrolanguages.add (ot_macrolanguage)
			जर ot_macrolanguages:
				साठी ot_macrolanguage मध्ये ot_macrolanguages:
					जर ot_macrolanguage नाही मध्ये blocked_ot_macrolanguages:
						साठी language मध्ये languages:
							self.add_language (language, ot_macrolanguage)
							जर नाही blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			किंवाजर first_time:
				साठी language मध्ये languages:
					जर language मध्ये self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					अन्यथा:
						ot_macrolanguages.clear ()
					जर नाही ot_macrolanguages:
						थांबा
				साठी ot_macrolanguage मध्ये ot_macrolanguages:
					self.add_language (macrolanguage, ot_macrolanguage)

	def sort_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		साठी language, tags मध्ये self.from_bcp_47.वस्तुसंच ():
			self.from_bcp_47[language] = sorted (tags,
					किल्ली_2=निनावी t: (self.ranks[t] + rank_delta (language, t), t))

ot = OpenTypeRegistryParser ()

वर्ग BCP47Parser (object):
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
	def __init__ (self):
		self.header = ''
		self.names = {}
		self.scopes = {}
		self.macrolanguages = collections.defaultdict (set)
		self.prefixes = collections.defaultdict (set)
		self.grandfathered = set ()

	def parse (self, filename):
		"""Parse the BCP 47 subtag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		with open (filename, encoding='utf-8') इव f:
			subtag_type = None
			subtag = None
			deprecated = असत्य
			has_preferred_मूल्य = असत्य
			ओळ_buffer = ''
			साठी ओळ मध्ये itertools.chain (f, ['']):
				ओळ = ओळ.rstrip ()
				जर ओळ.startswith (' '):
					ओळ_buffer += ओळ[1:]
					सुरूठेवा
				ओळ, ओळ_buffer = ओळ_buffer, ओळ
				जर ओळ.startswith ('Type: '):
					subtag_type = ओळ.split (' ')[1]
					deprecated = असत्य
					has_preferred_मूल्य = असत्य
				किंवाजर ओळ.startswith ('Subtag: ') किंवा ओळ.startswith ('Tag: '):
					subtag = ओळ.split (' ')[1]
					जर subtag_type == 'grandfathered':
						self.grandfathered.add (subtag.lower ())
				किंवाजर ओळ.startswith ('Description: '):
					description = ओळ.split (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					जर subtag मध्ये self.names:
						self.names[subtag] += '\n' + description
					अन्यथा:
						self.names[subtag] = description
				किंवाजर subtag_type == 'language' किंवा subtag_type == 'grandfathered':
					जर ओळ.startswith ('Scope: '):
						scope = ओळ.split (' ')[1]
						जर scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						किंवाजर scope == 'collection':
							scope = ' [collection]'
						अन्यथा:
							सुरूठेवा
						self.scopes[subtag] = scope
					किंवाजर ओळ.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = सत्य
					किंवाजर deprecated आणि ओळ.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						साठी language मध्ये ओळ.replace (',', '').split (' ')[2:]:
							self._add_macrolanguage (subtag, language)
					किंवाजर ओळ.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_मूल्य = सत्य
						macrolanguage = ओळ.split (' ')[1]
						self._add_macrolanguage (macrolanguage, subtag)
					किंवाजर नाही has_preferred_मूल्य आणि ओळ.startswith ('Macrolanguage: '):
						self._add_macrolanguage (ओळ.split (' ')[1], subtag)
				किंवाजर subtag_type == 'variant':
					जर ओळ.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					किंवाजर ओळ.startswith ('Prefix: '):
						self.prefixes[subtag].add (ओळ.split (' ')[1])
				किंवाजर ओळ.startswith ('File-Date: '):
					self.header = ओळ
		expect (self.header)

	def _add_macrolanguage (self, macrolanguage, language):
		वैश्विक ot
		जर language नाही मध्ये ot.from_bcp_47:
			साठी l मध्ये self.macrolanguages.get (language, set ()):
				self._add_macrolanguage (macrolanguage, l)
		जर macrolanguage नाही मध्ये ot.from_bcp_47:
			साठी ls मध्ये list (self.macrolanguages.मूल्ये ()):
				जर macrolanguage मध्ये ls:
					ls.add (language)
					परत
		self.macrolanguages[macrolanguage].add (language)

	def remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		साठी macrolanguage, languages मध्ये self.macrolanguages.वस्तुसंच ():
			साठी language मध्ये languages:
				inverted[language].append (macrolanguage)
		साठी language, macrolanguages मध्ये inverted.वस्तुसंच ():
			जर len (macrolanguages) > 1:
				macrolanguages.sort (किल्ली_2=निनावी ml: len (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.काढा ()
				साठी macrolanguage मध्ये macrolanguages:
					self._add_macrolanguage (biggest_macrolanguage, macrolanguage)

	def _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		परत self.names[subtag].split ('\n')[0] + self.scopes.get (subtag, '')

	def get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		जर lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		जर lt.region:
			name += '; ' + self._get_name_piece (lt.region.upper ())
		जर lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		परत name

bcp_47 = BCP47Parser ()

ot.parse (sys.argv[1])
bcp_47.parse (sys.argv[2])

ot.add_language ('ary', 'MOR')

ot.add_language ('ath', 'ATH')

ot.add_language ('bai', 'BML')

ot.ranks['BAL'] = ot.ranks['KAR'] + 1

ot.add_language ('ber', 'BBR')

ot.remove_language_ot ('PGR')
ot.add_language ('el-polyton', 'PGR')

bcp_47.names['flm'] = 'Falam Chin'
bcp_47.scopes['flm'] = ' (retired code)'
bcp_47.macrolanguages['flm'] = {'cfm'}

ot.ranks['FNE'] = ot.ranks['TNE'] + 1

ot.add_language ('und-fonipa', 'IPPH')

ot.add_language ('und-fonnapa', 'APPH')

ot.add_language ('und-fonupa', 'UPPH')

ot.add_language ('ga-Latg', 'IRT')

ot.add_language ('hy-arevmda', 'HYE')

ot.remove_language_ot ('KGE')
ot.add_language ('und-Geok', 'KGE')

ot.add_language ('kht', 'KHN')
ot.names['KHN'] = ot.names['KHT'] + ' (Microsoft fonts)'
ot.ranks['KHN'] = ot.ranks['KHT'] + 1

ot.ranks['LCR'] = ot.ranks['MCR'] + 1

ot.names['MAL'] = 'Malayalam Traditional'
ot.ranks['MLR'] += 1

bcp_47.names['mhv'] = 'Arakanese'
bcp_47.scopes['mhv'] = ' (retired code)'

ot.add_language ('mnw-TH', 'MONT')

ot.add_language ('no', 'NOR')

ot.add_language ('oc-provenc', 'PRO')

ot.remove_language_ot ('QUZ')
ot.add_language ('qu', 'QUZ')
ot.add_language ('qub', 'QWH')
ot.add_language ('qud', 'QVI')
ot.add_language ('qug', 'QVI')
ot.add_language ('qul', 'QUH')
ot.add_language ('qup', 'QVI')
ot.add_language ('qur', 'QWH')
ot.add_language ('qus', 'QUH')
ot.add_language ('quw', 'QVI')
ot.add_language ('qux', 'QWH')
ot.add_language ('qva', 'QWH')
ot.add_language ('qvh', 'QWH')
ot.add_language ('qvj', 'QVI')
ot.add_language ('qvl', 'QWH')
ot.add_language ('qvm', 'QWH')
ot.add_language ('qvn', 'QWH')
ot.add_language ('qvo', 'QVI')
ot.add_language ('qvp', 'QWH')
ot.add_language ('qvw', 'QWH')
ot.add_language ('qvz', 'QVI')
ot.add_language ('qwa', 'QWH')
ot.add_language ('qws', 'QWH')
ot.add_language ('qxa', 'QWH')
ot.add_language ('qxc', 'QWH')
ot.add_language ('qxh', 'QWH')
ot.add_language ('qxl', 'QVI')
ot.add_language ('qxn', 'QWH')
ot.add_language ('qxo', 'QWH')
ot.add_language ('qxr', 'QVI')
ot.add_language ('qxt', 'QWH')
ot.add_language ('qxw', 'QWH')

bcp_47.macrolanguages['ro-MD'].add ('mo')

ot.remove_language_ot ('SYRE')
ot.remove_language_ot ('SYRJ')
ot.remove_language_ot ('SYRN')
ot.add_language ('und-Syre', 'SYRE')
ot.add_language ('und-Syrj', 'SYRJ')
ot.add_language ('und-Syrn', 'SYRN')

bcp_47.names['xst'] = "Silt'e"
bcp_47.scopes['xst'] = ' (retired code)'
bcp_47.macrolanguages['xst'] = {'stv', 'wle'}

ot.add_language ('xwo', 'TOD')

ot.remove_language_ot ('ZHH')
ot.remove_language_ot ('ZHP')
ot.remove_language_ot ('ZHT')
ot.remove_language_ot ('ZHTM')
bcp_47.macrolanguages['zh'].remove ('lzh')
bcp_47.macrolanguages['zh'].remove ('yue')
ot.add_language ('zh-Hant-MO', 'ZHH')
ot.add_language ('zh-Hant-MO', 'ZHTM')
ot.add_language ('zh-Hant-HK', 'ZHH')
ot.add_language ('zh-Hans', 'ZHS')
ot.add_language ('zh-Hant', 'ZHT')
ot.add_language ('zh-HK', 'ZHH')
ot.add_language ('zh-MO', 'ZHH')
ot.add_language ('zh-MO', 'ZHTM')
ot.add_language ('zh-TW', 'ZHT')
ot.add_language ('lzh', 'ZHT')
ot.add_language ('lzh-Hans', 'ZHS')
ot.add_language ('yue', 'ZHH')
ot.add_language ('yue-Hans', 'ZHS')

def rank_delta (bcp_47, ot):
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
	जर bcp_47 == 'ak' आणि ot == 'AKA':
		परत -1
	जर bcp_47 == 'tw' आणि ot == 'TWI':
		परत -1
	परत 0

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
ot.names[DEFAULT_LANGUAGE_SYSTEM] = ''
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.मूल्ये ()) + 1
साठी tricky_ot_tag मध्ये filter (निनावी tag: re.match ('[A-Z]{3}$', tag), ot.names):
	possible_bcp_47_tag = tricky_ot_tag.lower ()
	जर possible_bcp_47_tag मध्ये bcp_47.names आणि नाही ot.from_bcp_47[possible_bcp_47_tag]:
		ot.add_language (possible_bcp_47_tag, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_tag] = set ()
ot.sort_languages ()

जर output_rust:
	def hb_tag_rust (tag):
		जर tag == DEFAULT_LANGUAGE_SYSTEM:
			परत 'Tag::new(&[0; 4])'
		sanitize_tag (tag)
		परत 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % tag)[:4])

	def get_variant_set_rust (name):
		परत set (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				साठी n मध्ये re.split ('[\n(),]', name) जर n)

	def language_name_intersection_rust (a, b):
		परत get_variant_set_rust (a).intersection (get_variant_set_rust (b))

	def get_matching_language_name_rust (intersection, candidates):
		परत next (iter (c साठी c मध्ये candidates जर नाही intersection.isdisjoint (get_variant_set_rust (c))))

	print ('// WARNING: this file was generated by ../../harfbuzz/src/gen-tag-table.py --rust')
	print ()
	print ('use read_fonts::types::Tag;')
	print ()
	print ('pub(crate) struct LangTag {')
	print ('    pub language: [u8; 4],')
	print ('    pub tag: Tag,')
	print ('}')
	print ()
	print ('const fn lang(language: &[u8]) -> [u8; 4] {')
	print ('    let mut bytes = [0; 4];')
	print ('    let mut i = 0;')
	print ()
	print ('    while i < language.len() {')
	print ('        bytes[i] = language[i];')
	print ('        i += 1;')
	print ('    }')
	print ()
	print ('    bytes')
	print ('}')
	print ()
	print ('use super::tag::{lang_matches, strncmp, subtag_matches};')
	print ()
	print ('#[rustfmt::skip]')
	print ('pub(crate) static OPEN_TYPE_LANGUAGES: &[LangTag] = &[')

	साठी language, tags मध्ये sorted (ot.from_bcp_47.वस्तुसंच ()):
		जर language == '' किंवा '-' मध्ये language:
			सुरूठेवा
		commented_out = len (language) == 3 आणि len (tags) == 1 आणि language == tags[0].lower ()
		साठी tag मध्ये tags:
			print ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' जर commented_out अन्यथा '    ', sanitize_bcp47_tag (language), hb_tag_rust (tag)), end='')
			print (' // ', end='')
			bcp_47_name = bcp_47.names.get (language, '')
			bcp_47_name_candidates = bcp_47_name.split ('\n')
			ot_name = ot.names[tag]
			scope = bcp_47.scopes.get (language, '')
			जर tag == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			अन्यथा:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				जर नाही intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				अन्यथा:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.names[language] = name
					comment = '%s%s' % (name जर len (name) > len (ot_name) अन्यथा ot_name, scope)
			print (sanitize_comment (comment))

	print ('];')
	print ()
	print ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	print ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	def print_subtag_matches_rust (subtag, new_ओळ):
		जर subtag:
			जर new_ओळ:
				print (' && ', end='')
			print ('subtag_matches(language, "-%s")' % sanitize_bcp47_tag (subtag), end='')

	complex_tags = collections.defaultdict (list)
	साठी initial, group मध्ये itertools.groupby ((lt_tags साठी lt_tags मध्ये [
				(LanguageTag (language), tags)
				साठी language, tags मध्ये sorted (ot.from_bcp_47.वस्तुसंच (),
					किल्ली_2=निनावी i: (-len (i[0]), i[0]))
			] जर lt_tags[0].is_complex ()),
			किल्ली_2=निनावी lt_tags: lt_tags[0].get_group ()):
		complex_tags[initial] += group

	साठी initial, वस्तुसंच मध्ये sorted (complex_tags.वस्तुसंच ()):
		जर initial != 'und':
			सुरूठेवा
		साठी lt, tags मध्ये वस्तुसंच:
			जर नाही tags:
				सुरूठेवा
			जर lt.variant मध्ये bcp_47.prefixes:
				expect (next (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			print ('    if ', end='')
			print_subtag_matches_rust (lt.script, असत्य)
			print_subtag_matches_rust (lt.region, असत्य)
			print_subtag_matches_rust (lt.variant, असत्य)
			print (' {')
			print ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			जर len (tags) == 1:
				print ('        tags.push(%s); // %s' % (hb_tag_rust (tags[0]), sanitize_comment (ot.names[tags[0]])))
			अन्यथा:
				print ('        let possible_tags = &[')
				साठी tag मध्ये tags:
					print ('            %s, // %s' % (hb_tag_rust (tag), sanitize_comment (ot.names[tag])))
				print ('        ];')
				print ('        tags.extend_from_slice(possible_tags);')
			print ('        return true;')
			print ('    }')

	print ('    match language.as_bytes()[0] {')
	साठी initial, वस्तुसंच मध्ये sorted (complex_tags.वस्तुसंच ()):
		जर initial == 'und':
			सुरूठेवा
		print ("        b'%s' => {" % sanitize_bcp47_tag (initial))
		साठी lt, tags मध्ये वस्तुसंच:
			जर नाही tags:
				सुरूठेवा
			print ('            if ', end='')
			script = lt.script
			region = lt.region
			जर lt.grandfathered:
				print ('&language[1..] == "%s"' % sanitize_bcp47_tag (lt.language[1:]), end='')
			अन्यथा:
				string_literal = lt.language[1:] + '-'
				जर script:
					string_literal += script
					script = None
					जर region:
						string_literal += '-' + region
						region = None
				जर string_literal[-1] == '-':
					print ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
				अन्यथा:
					print ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_tag (string_literal), end='')
			print_subtag_matches_rust (script, सत्य)
			print_subtag_matches_rust (region, सत्य)
			print_subtag_matches_rust (lt.variant, सत्य)
			print (' {')
			print ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			जर len (tags) == 1:
				print ('                tags.push(%s); // %s' % (hb_tag_rust (tags[0]), sanitize_comment (ot.names[tags[0]])))
			अन्यथा:
				print ('                let possible_tags = &[')
				साठी tag मध्ये tags:
					print ('                    %s, // %s' % (hb_tag_rust (tag), sanitize_comment (ot.names[tag])))
				print ('                ];')
				print ('                tags.extend_from_slice(possible_tags);')
			print ('                return true;')
			print ('            }')
		print ('        }')
	print ('        _ => {}')
	print ('    }')
	print ('    false')
	print ('}')
	sys.exit (0)

print ('/* == Start of generated table == */')
print ('/*')
print (' * The following table is generated by running:')
print (' *')
print (' *   %s languagetags language-subtag-registry' % os.path.basename (sys.argv[0]))
print (' *')
print (' * on files with these headers:')
print (' *')
print (' * %s' % sanitize_header (ot.header))
print (' * %s' % sanitize_header (bcp_47.header))
print (' */')
print ()
print ('#ifndef HB_OT_TAG_TABLE_HH')
print ('#define HB_OT_TAG_TABLE_HH')
print ()

def hb_tag (tag):
	"""Convert a tag to ``HB_TAG`` form.

	Args:
		tag (str): An OpenType tag.

	Returns:
		A snippet of C++ representing ``tag``.
	"""
	जर tag == DEFAULT_LANGUAGE_SYSTEM:
		परत 'HB_TAG_NONE\t       '
	sanitize_tag (tag)
	परत "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % tag)[:4])

def get_variant_set (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	परत set (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			साठी n मध्ये re.split ('[\n(),]', name) जर n)

def language_name_intersection (a, b):
	"""Return the names in common between two language names.

	Args:
		a (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.
		b (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		The normalized language names shared by ``a`` and ``b``.
	"""
	परत get_variant_set (a).intersection (get_variant_set (b))

def get_matching_language_name (intersection, candidates):
	परत next (iter (c साठी c मध्ये candidates जर नाही intersection.isdisjoint (get_variant_set (c))))

def same_tag (bcp_47_tag, ot_tags):
	परत len (bcp_47_tag) == 3 आणि len (ot_tags) == 1 आणि bcp_47_tag == ot_tags[0].lower ()

def format_language_comment (language, tag):
	bcp_47_name = bcp_47.names.get (language, '')
	bcp_47_name_candidates = bcp_47_name.split ('\n')
	ot_name = ot.names[tag]
	scope = bcp_47.scopes.get (language, '')
	जर tag == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	अन्यथा:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		जर नाही intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		अन्यथा:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.names[language] = name
			comment = '%s%s' % (name जर len (name) > len (ot_name) अन्यथा ot_name, scope)
	परत sanitize_comment (comment)

print ('static const LangTag ot_languages2[] = {')
साठी language, tags मध्ये sorted (ot.from_bcp_47.वस्तुसंच ()):
	जर language == '' किंवा '-' मध्ये language किंवा len (language) != 2:
		सुरूठेवा
	साठी tag मध्ये tags:
		print ('  {%s,\t%s},\t/* %s */' % (
			hb_tag (language),
			hb_tag (tag),
			format_language_comment (language, tag)))
print ('};')
print ()

print ('#ifndef HB_NO_LANGUAGE_LONG')

languages3_blocked = []
languages3_singles = []
languages3_multi_ranges = []
languages3_multi_मूल्ये = []
साठी language, tags मध्ये sorted (ot.from_bcp_47.वस्तुसंच ()):
	जर language == '' किंवा '-' मध्ये language किंवा len (language) != 3:
		सुरूठेवा
	जर same_tag (language, tags):
		सुरूठेवा
	जर tags == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, tags[0]))
		सुरूठेवा
	जर len (tags) == 1:
		languages3_singles.append ((language, tags[0]))
		सुरूठेवा
	offset = len (languages3_multi_मूल्ये)
	languages3_multi_मूल्ये.extend ((language, tag) साठी tag मध्ये tags)
	languages3_multi_ranges.append ((language, offset, len (tags), tags[0]))

print ('static const hb_tag_t ot_languages3_blocked[] = {')
साठी language, tag मध्ये languages3_blocked:
	print ('  %s,\t/* %s */' % (hb_tag (language), format_language_comment (language, tag)))
print ('};')
print ()

print ('static const LangTag ot_languages3[] = {')
साठी language, tag मध्ये languages3_singles:
	print ('  {%s,\t%s},\t/* %s */' % (
		hb_tag (language),
		hb_tag (tag),
		format_language_comment (language, tag)))
print ('};')
print ()

print ('static const hb_tag_t ot_languages3_multi_values[] = {')
साठी language, tag मध्ये languages3_multi_मूल्ये:
	print ('  %s,\t/* %s */' % (hb_tag (tag), format_language_comment (language, tag)))
print ('};')
print ()

print ('static const LangTagRange ot_languages3_multi[] = {')
साठी language, offset, मोजणी, first_tag मध्ये languages3_multi_ranges:
	print ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_tag (language),
		पूर्णांक(offset),
		पूर्णांक(मोजणी),
		format_language_comment (language, first_tag)))
print ('};')
print ('#endif')
print ()

print ('/**')
print (' * hb_ot_tags_from_complex_language:')
print (' * @lang_str: a BCP 47 language tag to convert.')
print (' * @limit: a pointer to the end of the substring of @lang_str to consider for')
print (' * conversion.')
print (' * @count: maximum number of language tags to retrieve (IN) and actual number of')
print (' * language tags retrieved (OUT). If no tags are retrieved, it is not modified.')
print (' * @tags: array of size at least @language_count to store the language tag')
print (' * results')
print (' *')
print (' * Converts a multi-subtag BCP 47 language tag to language tags.')
print (' *')
print (' * Return value: Whether any language systems were retrieved.')
print (' **/')
print ('static inline bool')
print ('hb_ot_tags_from_complex_language (const char   *lang_str,')
print ('\t\t\t\t  const char   *limit,')
print ('\t\t\t\t  unsigned int *count /* IN/OUT */,')
print ('\t\t\t\t  hb_tag_t     *tags /* OUT */)')
print ('{')

def print_subtag_matches (subtag, string, new_ओळ):
	जर subtag:
		जर new_ओळ:
			print ()
			print ('\t&& ', end='')
		print ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_tag (subtag), 1 + len (subtag)), end='')

complex_tags = collections.defaultdict (list)
साठी initial, group मध्ये itertools.groupby ((lt_tags साठी lt_tags मध्ये [
			(LanguageTag (language), tags)
			साठी language, tags मध्ये sorted (ot.from_bcp_47.वस्तुसंच (),
				किल्ली_2=निनावी i: (-len (i[0]), i[0]))
		] जर lt_tags[0].is_complex ()),
		किल्ली_2=निनावी lt_tags: lt_tags[0].get_group ()):
	complex_tags[initial] += group

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
साठी initial, वस्तुसंच मध्ये sorted (complex_tags.वस्तुसंच ()):
	जर initial != 'und':
		सुरूठेवा
	साठी lt, tags मध्ये वस्तुसंच:
		जर नाही tags:
			सुरूठेवा
		subtag_len = 0
		subtag_len += 1 + len (lt.script) जर lt.script is नाही None अन्यथा 0
		subtag_len += 1 + len (lt.region) जर lt.region is नाही None अन्यथा 0
		subtag_len += 1 + len (lt.variant) जर lt.variant is नाही None अन्यथा 0
		min_subtag_len = min(subtag_len, min_subtag_len)

print ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
print ('  {')
print ("    const char *p = strchr (lang_str, '-');")
print ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
साठी initial, वस्तुसंच मध्ये sorted (complex_tags.वस्तुसंच ()):
	जर initial != 'und':
		सुरूठेवा
	साठी lt, tags मध्ये वस्तुसंच:
		जर नाही tags:
			सुरूठेवा
		जर lt.variant मध्ये bcp_47.prefixes:
			expect (next (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		print ('    if (', end='')
		print_subtag_matches (lt.script, 'p', असत्य)
		print_subtag_matches (lt.region, 'p', असत्य)
		print_subtag_matches (lt.variant, 'p', असत्य)
		print (')')
		print ('    {')
		लिख ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		print ()
		जर len (tags) == 1:
			लिख ('      tags[0] = %s;  /* %s */' % (hb_tag (tags[0]), sanitize_comment (ot.names[tags[0]])))
			print ()
			print ('      *count = 1;')
		अन्यथा:
			print ('    hb_tag_t possible_tags[] = {')
			साठी tag मध्ये tags:
				लिख ('      %s,  /* %s */' % (hb_tag (tag), sanitize_comment (ot.names[tag])))
				print ()
			print ('      };')
			print ('      for (i = 0; i < %s && i < *count; i++)' % len (tags))
			print ('\ttags[i] = possible_tags[i];')
			print ('      *count = i;')
		print ('      return true;')
		print ('    }')
print ('  }')
print ('out:')

print ('  switch (lang_str[0])')
print ('  {')
साठी initial, वस्तुसंच मध्ये sorted (complex_tags.वस्तुसंच ()):
	जर initial == 'und':
		सुरूठेवा
	print ("  case '%s':" % sanitize_bcp47_tag (initial))
	साठी lt, tags मध्ये वस्तुसंच:
		जर नाही tags:
			सुरूठेवा
		print ('    if (', end='')
		script = lt.script
		region = lt.region
		जर lt.grandfathered:
			print ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_tag (lt.language[1:]), end='')
		अन्यथा:
			string_literal = lt.language[1:] + '-'
			जर script:
				string_literal += script
				script = None
				जर region:
					string_literal += '-' + region
					region = None
			जर string_literal[-1] == '-':
				print ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
			अन्यथा:
				print ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
		print_subtag_matches (script, 'lang_str', सत्य)
		print_subtag_matches (region, 'lang_str', सत्य)
		print_subtag_matches (lt.variant, 'lang_str', सत्य)
		print (')')
		print ('    {')
		लिख ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		print ()
		जर len (tags) == 1:
			लिख ('      tags[0] = %s;  /* %s */' % (hb_tag (tags[0]), sanitize_comment (ot.names[tags[0]])))
			print ()
			print ('      *count = 1;')
		अन्यथा:
			print ('      unsigned int i;')
			print ('      hb_tag_t possible_tags[] = {')
			साठी tag मध्ये tags:
				लिख ('\t%s,  /* %s */' % (hb_tag (tag), sanitize_comment (ot.names[tag])))
				print ()
			print ('      };')
			print ('      for (i = 0; i < %s && i < *count; i++)' % len (tags))
			print ('\ttags[i] = possible_tags[i];')
			print ('      *count = i;')
		print ('      return true;')
		print ('    }')
	print ('    break;')

print ('  }')
print ('  return false;')
print ('}')
print ()
print ('/**')
print (' * hb_ot_ambiguous_tag_to_language')
print (' * @tag: A language tag.')
print (' *')
print (' * Converts @tag to a BCP 47 language tag if it is ambiguous (it corresponds to')
print (' * many language tags) and the best tag is not the first (sorted alphabetically,')
print (' * with two-letter tags having priority over all three-letter tags), or if the')
print (' * best tag consists of multiple subtags, or if the best tag does not appear in')
print (' * #ot_languages2 or #ot_languages3.')
print (' *')
print (' * Return value: The #hb_language_t corresponding to the BCP 47 language tag,')
print (' * or #HB_LANGUAGE_INVALID if @tag is not ambiguous.')
print (' **/')
print ('static inline hb_language_t')
print ('hb_ot_ambiguous_tag_to_language (hb_tag_t tag)')
print ('{')
print ('  switch (tag)')
print ('  {')

def verify_disambiguation_dict ():
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
	वैश्विक bcp_47
	वैश्विक disambiguation
	वैश्विक ot
	साठी ot_tag, bcp_47_tags मध्ये ot.to_bcp_47.वस्तुसंच ():
		जर ot_tag == DEFAULT_LANGUAGE_SYSTEM:
			primary_tags = []
		अन्यथा:
			primary_tags = list (t साठी t मध्ये bcp_47_tags जर t नाही मध्ये bcp_47.grandfathered आणि ot.from_bcp_47.get (t)[0] == ot_tag)
		जर len (primary_tags) == 1:
			expect (ot_tag नाही मध्ये disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_tag)
			जर '-' मध्ये primary_tags[0]:
				disambiguation[ot_tag] = primary_tags[0]
			अन्यथा:
				first_tag = sorted ((t साठी t मध्ये bcp_47_tags जर t नाही मध्ये bcp_47.grandfathered आणि ot_tag मध्ये ot.from_bcp_47.get (t)),
						किल्ली_2=निनावी t: (len (t), t))[0]
				जर primary_tags[0] != first_tag:
					disambiguation[ot_tag] = primary_tags[0]
		किंवाजर len (primary_tags) == 0:
			expect (ot_tag नाही मध्ये disambiguation, 'There is no possible valid disambiguation for %s' % ot_tag)
		अन्यथा:
			original_languages = [t साठी t मध्ये primary_tags जर t मध्ये ot.from_bcp_47_uninherited आणि 'retired code' नाही मध्ये bcp_47.scopes.get (t, '')]
			जर len (original_languages) == 1:
				macrolanguages = original_languages
			अन्यथा:
				macrolanguages = [t साठी t मध्ये primary_tags जर bcp_47.scopes.get (t) == ' [macrolanguage]']
			जर len (macrolanguages) != 1:
				macrolanguages = list (t साठी t मध्ये primary_tags जर bcp_47.scopes.get (t) == ' [collection]')
			जर len (macrolanguages) != 1:
				macrolanguages = list (t साठी t मध्ये primary_tags जर 'retired code' नाही मध्ये bcp_47.scopes.get (t, ''))
			जर len (macrolanguages) != 1:
				macrolanguages = list (t साठी t मध्ये primary_tags जर t.lower () == ISO_639_3_TO_1.get (ot_tag.lower (), ot_tag.lower ()))
			जर len (macrolanguages) != 1:
				macrolanguages = list (t साठी t मध्ये primary_tags जर '-' नाही मध्ये t)
			जर len (macrolanguages) != 1:
				expect (ot_tag मध्ये disambiguation, 'ambiguous OT tag: %s %s' % (ot_tag, sorted (primary_tags)))
				expect (disambiguation[ot_tag] मध्ये bcp_47_tags,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_tag], ot_tag))
			किंवाजर ot_tag नाही मध्ये disambiguation:
				disambiguation[ot_tag] = macrolanguages[0]
			जर '-' नाही मध्ये disambiguation[ot_tag]:
				different_bcp_47_tags = sorted ((t साठी t मध्ये bcp_47_tags जर नाही same_tag (t, ot.from_bcp_47.get (t))),
						किल्ली_2=निनावी t: (len (t), t))
				जर different_bcp_47_tags आणि disambiguation[ot_tag] == different_bcp_47_tags[0]:
					del disambiguation[ot_tag]
	साठी ot_tag मध्ये disambiguation.किल्ल्या ():
		expect (ot_tag मध्ये ot.to_bcp_47, 'unknown OT tag: %s' % ot_tag)

verify_disambiguation_dict ()
साठी ot_tag, bcp_47_tag मध्ये sorted (disambiguation.वस्तुसंच ()):
	लिख ('  case %s:  /* %s */' % (hb_tag (ot_tag), sanitize_comment (ot.names[ot_tag])))
	print ()
	लिख ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_tag (bcp_47_tag), sanitize_comment (bcp_47.get_name (LanguageTag (bcp_47_tag)))))
	print ()

print ('  default:')
print ('    return HB_LANGUAGE_INVALID;')
print ('  }')
print ('}')

print ()
print ('#endif /* HB_OT_TAG_TABLE_HH */')
print ()
print ('/* == End of generated table == */')
