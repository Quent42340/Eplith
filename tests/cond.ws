function test(n) {
	if(n == 0) {
		print("Test: Zero\n");
		return 1;
	} else {
		print("Test: " + n + "\n");
		return n * test(n - 1);
	}
}

print(test(2) + "\n");

