import os

def points_per_test(nr):
	val = 0.75

	if nr >= 40:
		val = 1
	if nr >= 60:
		val = 1.75

	return val

creating_refs = False

if creating_refs:
	numTests = 80
	print("Creating tests")

	for i in range(0, numTests):
		in_file = "in/test{}.in".format(i)
		out_file = "out/test{}.out".format(i)
		ref_file = "ref/test{}.ref".format(i)

		os.system("./tema1 < {} > {}".format(in_file, ref_file))

else:
	numTests = 80

	norPunct = 0
	valPunct = 0

	print("------------------------------------")
	print("Checker TEMA 1 SD")
	print("------------------------------------")
	print("Running tests.....normal....valgrind")
	print("------------------------------------")

	for i in range(0, numTests):
		in_file = "in/test{}.in".format(i)
		out_file = "out/test{}.out".format(i)
		ref_file = "ref/test{}.ref".format(i)

		retVal = os.system("valgrind -q --log-file=v.out --leak-check=full --error-exitcode=13 ./tema1 < {} > {}".format(in_file, out_file))

		ret = os.system("diff {} {}".format(ref_file, out_file))
		
		if ret == 0:
			norPunct = norPunct + points_per_test(i)
			if retVal == 0:
				valPunct = valPunct + 0.25
				print("Running test{:0>2d}....PASSED....PASSED".format(i))
			else:
				print("Running test{:0>2d}....PASSED....FAILED".format(i))
		else:
			print("Running test{:0>2d}....FAILED....FAILED".format(i))

	print("------------------------------------")
	print("Punctaj total {}/85.0".format(norPunct))
	print("Punctaj valgrind {}/20.0".format(valPunct))
	os.system("rm -rf v.out")
