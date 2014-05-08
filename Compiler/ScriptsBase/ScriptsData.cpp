

#include "internal.h"



//c-code block
const TScanFlag gC_Comments[] =
{
	{"//", "\n"},
	{"/*", "*/"},
};

const int gC_CommentsSize = sizeof(gC_Comments) / sizeof(TScanFlag);

const char * gC_ReservedKeywordList[] =
{
	// reserved Keyword
	 "int", "char", "double", "enum", "typedef",  "void", "return", 
	 "if", "else", "for", "while", "continue", "break", "printf", "main",
	/*"auto","switch","case","register","extern","union","const", "float", "short", "unsigned",
	  "signed","long", "default", "goto", "sizeof", "volatile","struct","do","static","read", "write",*/

	// operations
	"=", "+", "-", "*", "/", "%", "&", "|", "~", "!", "<", ">",
	
	// interpunctions
	"(", ")", "{", "}", "[", "]", ",", ".", ";", ":", "?"
};

const int gC_ReservedKeywordListSize = sizeof(gC_ReservedKeywordList)/ sizeof(*gC_ReservedKeywordList);


//Map-code block:
const TScanFlag gMap_Comments[] =
{
	{"//", "\n"},
	{"/*", "*/"},
};

const int gMap_CommentsSize = sizeof(gMap_Comments) / sizeof(TScanFlag);

const char * gMap_ReservedKeywordList[] =
{
	// reserved Keyword
	"#", "field", "sectionname", 

	// operations
	"=", "[", "]", "{", "}", "-", "+"
};

const int gMap_ReservedKeywordListSize = sizeof(gMap_ReservedKeywordList)/ sizeof(*gMap_ReservedKeywordList);