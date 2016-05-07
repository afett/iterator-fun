Thank you Markus and Alex. There goes my weekend!

	std::map<int, bool> foo;
	BOOST_FOREACH(bool x, foo | boost::adaptors::map_values) {
		...
	}

WTF, How does this work?

C++ is for designing frameworks... and it's not good at this. As a bonus
you have to put up with all the extra complexity when you actually want
to "get shit done (TM)". Can't believe we're actually using this kind
of crap.
