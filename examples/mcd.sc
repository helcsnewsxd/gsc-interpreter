var a = 48;
var b = 18;

while (b != 0) {
  var q = a / b; // Notice that it's integer division
  var r = a - q * b;

  a = b;
  b = r;
}

print "The GCD is:";
print a;
