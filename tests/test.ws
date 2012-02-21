@(var) = 52;
print(@(var));
#!$(str) = "string";
@(var2) = 50;
@(var3) = @(var) + @(var2);
@(var4) = @(var) < @(var2);
$(str2) = "I found a " + $(str) + "!";
print("\ttest: " + $(str2) + "\n");
!#

#!
@(x) = 0;
while(@(x) < 3) {
	print(@(x));
	@(x) = @(x) + 1;
}
!#
