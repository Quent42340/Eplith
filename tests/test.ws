@(var) = 52;
@(str) = "string";
@(var2) = 50;
@(var3) = @(var) + @(var2);
@(var4) = @(var) < @(var2);
@(str2) = "I found a " + @(str) + "!";
@(bool) = true;
print("\ttest: " + @(str2) + " and @(var3) = " + @(var3) + "; That's " + @(bool) + "\n");

#!
@(x) = 0;
while(@(x) < 3) {
	print(@(x));
	@(x) = @(x) + 1;
}
!#
