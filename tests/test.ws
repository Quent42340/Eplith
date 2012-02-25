@(var) = 52;
@(str) = "string";
@(var2) = 50;
@(var3) = @(var) + @(var2);
@(var4) = @(var) < @(var2);
@(str2) = "I found a " + @(str) + "!";
@(str3) = @(str) < @(str2);
print("\ttest: " + @(str2) + " and @(var3) = " + @(var3) + ";\n");

#!
@(x) = 0;
while(@(x) < 3) {
	print(@(x));
	@(x) = @(x) + 1;
}
!#
