#pragma once

#include <vector>

class GASolver
{
private:
	int m_Board_Size = 8;
	int m_Population_Size = 1000;
	int m_GenerationLimit = 100;
	int m_BadPopulation = sqrt(m_Population_Size);
	int m_SumFitness;
	double m_CrossOverProbability = 0.8;
	double m_MutationProbability = 0.25;
	bool m_bUsedPos[10010] = { 0 };

	struct Gene {
		std::vector<int> queensPos;
		int fitness = -1;
		double selectionProbability = -1;
		double cumulativeDistribution = 0;
	};
	std::vector<Gene> m_Population;
	std::vector<Gene> m_ChildPopulation;
	std::vector<Gene> m_ResPopulation;
	std::vector<int> m_Result;

public:
	GASolver();
	GASolver(int boardSize, int populationSize, int generationLimit, double crossOverProbabilty, double mutationProbabilty);
	int genCnt = 0;
	void SetBoardSize(int n);
	int GetFitness(Gene &curGene);
	int GetSumFitness();
	static bool fitnessCMP(const Gene &a, const Gene &b);
	long long Rand(long long a, long long b);
	double GetSelectionProbabilty(Gene &curGene);
	void GenerateCumulativeDistribution();
	void GeneratePopulation(int size = 1000);
	int GetParentFromBad(int prevIdx);
	int GetParentFromMain(int prevIdx);
	void CrossOver();
	void CrossOverMethod(int pA_idx, int pB_idx, Gene& cC);
	void Mutation(Gene &curGene);
	void CreateNewGeneration();
	bool CheckStopCrt();
	void Run();
	void Reset();
	std::vector<int> GetResult();
	~GASolver();
};

