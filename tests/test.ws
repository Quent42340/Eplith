var = 52;
str = "string";
var2 = 50;
var3 = var + var2;
var4 = var < var2;
str2 = "I found a " + str + "!";
bool = true;
var5 = 5 - (2 + 3) + 1 * (4 - 2);
print("\ttest: " + str2 + " and var3 = " + var3 + "; That's " + bool + "\n");

if(var4) {
	if(bool) {
		print("Okay, that's not normal.\n");
	} else {
		print("Whoa! WTF?!\n");
	}
} else {
	if(!bool) {
		print("Err, there is something wrong.\n");
	} else {
		print("Yeah, it's normal!\n");
	}
}

if(5 < 1) {
	print("Are you kidding me?\n");
} 
else if(4 > 2) {
	print("Okay that's good.\n");
} else {
	print("But what the fuck?\n");
}

x = 0;
while(x < 3) {
	print(x + "\n");
	x = x + 1;
}

do {
	print(x + "\n");
	x = x - 1;
} while(x > -1);

for(i = 0->2) {
	print(i + "\n");
}

a = b = 50;
c = 5 % 2;
d = bool && var4;
e = bool || var4;

