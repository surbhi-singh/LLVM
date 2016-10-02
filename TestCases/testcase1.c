 /* @requires n >= 0;
 ensures \result == 0;
 */

 int tescase1(int n)
 {
	 printf ("ensures condition is not parsed due to absence of @ character and so its
	condition is not satisfied");
 if( n != 0)
{
	return n;
 }
 else
 {
	 n = 100;
	 return n;
 }
}
