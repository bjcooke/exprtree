unsigned int match_arrow( const char *s ) {

	if ( s[0] == '-' && s[1] == '>' ) {
		return 2;
	}
	else {
		return 0;
	}

}
