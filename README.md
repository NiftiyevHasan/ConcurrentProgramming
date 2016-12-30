--------- COMPILE AND RUN ON THE COMMAND LINE ---------

	COMPILER : g++
	
	There are four .cpp files

	sequential.cpp for sequential execution
		
		g++ -g -Wall -o sequential sequential.cpp
		./sequential
		
	parallel.cpp for parallel execution
		
		g++ -g -Wall -o parallel parallel.cpp -fopenmp
		./parallel
		
		
	optimizd.cpp for optimized execution
		
		g++ -g -Wall -o optimized optimized.cpp -fopenmp
		./optimized
	
	aio.cpp for all versions with mean, sd and sample sizes

	g++ -g -Wall -o aio aio.cpp -fopenmp
		./aio

----------------- SAMPLE CALCULATION ------------------

	Equation : 	n = (100zs/rμ)^2
	Where 		μ = mean
				s = standard deviation
				r = required accuracy
				z = z value
	
------------------------ Note -------------------------

	Parallel Library : OpenMP
	Optimization : Cache Optimization
	By default matrix size is set to 1000
	Change variable N if you wand different matrix size	

