 int slowsubstract(int z, int x)
 /* @requires x >= 0;

@ensures \result == prev(z) - prev(x);
 */
 {
 int x_loopIn = x;
int z_loopIn = z;
 while (x != 0)
 {

z = z-1;

x = x-1;
 }
 return z;
 }

