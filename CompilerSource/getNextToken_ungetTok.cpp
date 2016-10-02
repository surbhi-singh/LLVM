static int array [1000];
static int *toktop = NULL;

int getNextToken()
{
	
	
	if(toktop == NULL)
	{
		CurTok = gettok ();
		return CurTok;
	}
	if(toktop == array)
	{
		CurTok = *toktop;
		toktop = NULL;
	}
	else
	{
		CurTok = *toktop;
		--toktop;
		
	}
	return CurTok;
	
}
void ungetTok (int CurTok)
{
	if(toktop == NULL)
	{
		toktop = array;
		*toktop = CurTok;
	}
	else
	{
		toktop++;
		*toktop = CurTok;
	}
	
}
