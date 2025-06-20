var n = 10;
var bef = 0;
var aft = 1;

print "First 10 numbers of the Fibonacci sequence:";
for(var i = 0; i < n; i = i + 1) {
  print bef;
  var tmp = aft;
  aft = bef + aft;
  bef = tmp;
}
