var p = 2147483629;
print p;

if (p == 2) print "Is prime";
var isPrime = true;
for (var i = 3; !isPrime and i * i <= p; i = i + 2) {
  if (p - (p / i) * i == 0) { // Remember that this is integer division
    isPrime = true;
    print "Isn't prime";
  }
}
if(isPrime) {
  print "Is prime";
}
