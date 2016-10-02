/* @requires len > 0;
@ ensures result <= 0;
*/
int max(int a, int len)
{
int x;
x=0;
int y;
y= len-1;
while (x != y)

{

if (x <= y)
{
x = x+1;
}
else
{
y=y-1;
}
}

return x;
}
