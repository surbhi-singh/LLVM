/* @requires n >= 0;
 @ensures result == 0;
 */

 int testcase2(int n)
 {
 printf ("results in error as post condition is not satisfied");
 if( n == 0){
 n=n+5;
}
 return n;
 }

