#pragma once

#include <vector>

class GASolver
{
private:
	int m_Board_Size = 8;
	int m_BadPopulation;
	int m_SumFitness;
	double m_CrossOverProbability = 0.8;
	double m_MutationProbability = 0.25;
	int m_GenerationLimit = 1000;

	struct Gene {
		std::vector<int> queensPos;
		int fitness = -1;
		double selectionProbability = -1;
		double cumulativeDistribution = 0;
	};
	std::vector<Gene> m_Population;
	std::vector<Gene> m_ChildPopulation;
	std::vector<int> m_Result;

public:
	GASolver();
	void SetBoardSize(int n);
	int GetFitness(Gene &curGene);
	int GetSumFitness();
	static bool fitnessCMP(const Gene &a, const Gene &b);
	long long Rand(long long a, long long b);
	int GetSelectionProbabilty(Gene &curGene);
	void GenerateCumulativeDistribution();
	void GeneratePopulation(int size = 1000);
	Gene GetParentFromBad(double prev);
	Gene GetParentFromMain(double prev);
	void CrossOver();
	Gene CrossOverMethod(Gene pA, Gene pB);
	void Mutation(Gene &curGene);
	void CreateNewGeneration();
	bool CheckStopCrt();
	void Run();
	void Reset();
	std::vector<int> GetResult();
	~GASolver();
};

