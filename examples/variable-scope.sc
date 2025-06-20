var x = 1;
print x; // Must print 1
{
  var x = 2;
  print x; // Must print 2
  {
    x = 3;
    print x; // Must print 3
  }
  print x; // Must print 3 (because the variable was modified,
           //               not re-declared)
}
print x; // Must print 1
{
  var y = 4;
  print y; // Must print 4
}
var y;
print y; // Must print nil
