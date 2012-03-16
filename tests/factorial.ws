function factorial(n) {
	print("Entering function\n");
	print("n = " + n + " n == 0 ? " + (n == 0) + "\n");
	if(n == 0) {
		print("Truc\n");
		return 1;
	} else {
		print("Machin\n");
		return n * factorial(n - 1);
	}
}

print(factorial(1) + "\n");

