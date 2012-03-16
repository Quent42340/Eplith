function puts(n) {
	print("--- : " + n + "\n");
	if(n == 0) {
		print("Truc\n");
		return 1;
	} else {
		print("Machin\n");
		return 1 * puts(0);
	}
}

print(puts(1) + "\n");

