@(var) = 52;
@(str) = "string";
@(var2) = 50;
@(var3) = @(var) + @(var2);
@(var4) = @(var) < @(var2);
@(str2) = "I found a " + @(str) + "!";
@(bool) = true;
@(var5) = 5 - (2 + 3) + 1 * (4 - 2);
print("\ttest: " + @(str2) + " and @(var3) = " + @(var3) + "; That's " + @(bool) + "\n");

if(@(var4)) {
	print("Okay, that's not normal.\n");
} else {
	print("Yeah, it's normal!\n");
}

#!
@(x) = 0;
while(@(x) < 3) {
	print(@(x));
	@(x) = @(x) + 1;
}
!#
