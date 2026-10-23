#ifndef Bees_algorithm_h
#define Bees_algorithm_h



class BeesAlgorithm : public Algorithm
{
protected:
	
	uint n_;

	double ubound_;
	double lbound_;
	uint dimension_;

public:
	BeesAlgorithm()
	{
		// the algorithm name will be used in config file (see below)
		name_ = "BeesAlgorithm";

	}
	void registerParameters(StateP state);
	bool initialize(StateP state);
	bool advanceGeneration(StateP state, DemeP deme);

};
typedef std::shared_ptr<BeesAlgorithm> BeesAlgorithmP;

#endif // Bees_algorithm_h

