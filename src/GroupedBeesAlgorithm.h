#ifndef Grouped_bees_algorithm_h
#define Grouped_bees_algorithm_h



class GroupedBeesAlgorithm : public Algorithm
{
protected:
	double ubound_;
	double lbound_;
	uint dimension_;
	uint nGroups_;
	double R_ngh_;
	std::vector<int> groups;
	std::vector<int> recruited_bees;
	std::vector<double> ngh;

	void initializeParameters();
	std::vector<double> generateRandomSolution();
	std::vector<double> beeWaggleDance(const std::vector<double>& solution, double ngh);

public:
	GroupedBeesAlgorithm()
	{
		// the algorithm name will be used in config file (see below)
		name_ = "GroupedBeesAlgorithm";

	}
	void registerParameters(StateP state);
	bool initialize(StateP state);
	bool advanceGeneration(StateP state, DemeP deme);

};
typedef std::shared_ptr<GroupedBeesAlgorithm> GroupedBeesAlgorithmP;

#endif // Grouped_bees_algorithm_h
