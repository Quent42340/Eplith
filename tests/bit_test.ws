a = ~7; # -8 ( ~x = -x - 1; )
a *= -1; # 8
print("a = " + a + "\n");
b = 5 & 3; # 1
b += 5; # 6
print("b = " + b + "\n");
c = 5 | 3; # 7
c *= 3; # 21
print("c = " + c + "\n");
d = 5 >< 3; # 6
e = 0xF << 4; # 0xF0 ( 240 )
f = 0xF >> 2; # 0x3 ( 3 )
print("e = " + (hex)e + " f = " + (hex)f + "\n");

print((hex)e);
print("\n");

