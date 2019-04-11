#include "pch.h"
#include "GASolver.h"
#include <algorithm>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <map>


GASolver::GASolver()
{
}

GASolver::GASolver(int boardSize, int populationSize, int generationLimit, double crossOverProbabilty, double mutationProbabilty)
{
	m_Board_Size = boardSize;
	m_Population_Size = populationSize;
	m_GenerationLimit = generationLimit;
	m_CrossOverProbability = crossOverProbabilty;
	m_MutationProbability = mutationProbabilty;
	int m_BadPopulation = sqrt(m_Population_Size);

}

void GASolver::SetBoardSize(int n)
{
	m_Board_Size = n;
}

int GASolver::GetFitness(Gene &curGene)
{
	int t1 = 0; // from left corner
	int t2 = 0; // from right corner
	std::vector<int> f1, f2;

	for (int i = 0; i < curGene.queensPos.size(); i++) {
		f1.push_back(curGene.queensPos[i] - i);
		f2.push_back(curGene.queensPos[i] + i);
	}
	std::sort(f1.begin(), f1.end());
	std::sort(f2.begin(), f2.end());
	for (int i = 1; i < f1.size(); i++) {
		if (f1[i] == f1[i - 1]) t1++;
		if (f2[i] == f2[i - 1]) t2++;
	}
	curGene.fitness = t1 + t2;
	return curGene.fitness;
}

void GASolver::GeneratePopulation(int size)
{
	std::vector<int> zero;
	for (int i = 0; i < m_Board_Size; i++)
		zero.push_back(i);

	for (int i = 0; i < size; i++) {
		Gene newGene;
		newGene.queensPos = zero;
		newGene.fitness = GetFitness(newGene);
		m_Population.push_back(newGene);

		std::random_shuffle(zero.begin(), zero.end());
	}
	m_BadPopulation = floor(sqrt(size));
}


int GASolver::GetSumFitness()
{
	m_SumFitness = 0;
	for (int i = 0; i < m_Population.size(); i++)
		m_SumFitness += m_Population[i].fitness;
	return m_SumFitness;
}

bool GASolver::fitnessCMP(const Gene &a, const Gene &b)
{
	return a.fitness > b.fitness;
}

long long GASolver::Rand(long long a, long long b)
{
	return a + ((long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
		(long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
		(long long)rand() * (RAND_MAX + 1) +
		rand()) % (b - a + 1);
}

double GASolver::GetSelectionProbabilty(Gene &curGene)
{
	curGene.selectionProbability = (1.0 - (double) curGene.fitness / (double) m_SumFitness) / ((double) (m_Population.size() - 1));
	return curGene.selectionProbability;
}

void GASolver::GenerateCumulativeDistribution()
{
	GetSumFitness();
	for (int i = 0; i < m_Population.size(); i++) {
		double k = GetSelectionProbabilty(m_Population[i]);
		m_Population[i].cumulativeDistribution = k;
		if (i > 0)
			m_Population[i].cumulativeDistribution += m_Population[i - 1].cumulativeDistribution;
	}
}


int GASolver::GetParentFromBad(int prevIdx)
{
	int idx;
	do {
		idx = Rand(0, 1000000) % (m_BadPopulation + 1);
	} while (idx == prevIdx);
	return idx;
}

int GASolver::GetParentFromMain(int prevIdx)
{
	double k;
	int l, r, mid;
	do {
		int k1 = m_Population[m_BadPopulation].cumulativeDistribution * 1000000 + 1;
		k = (double)Rand(k1, 1000000) / 1000000.0;

		l = m_BadPopulation, r = m_Population.size();
		while (r - l > 1) {
			mid = (l + r) / 2;
			if (m_Population[mid].cumulativeDistribution <= k)
				l = mid;
			else r = mid;
		}
	} while (l == prevIdx);

	return l;
}

void GASolver::CrossOver()
{	
	m_ChildPopulation.clear();
	GenerateCumulativeDistribution();

	// main & main
	for (int i = 0; i < m_Population.size() - m_BadPopulation; i++) {
		int pA_idx = GetParentFromMain(-1);
		int pB_idx = GetParentFromMain(pA_idx);

		double r = (double)Rand(0, 1000000) / 1000000.0;
		if (r > m_CrossOverProbability) continue;
		Gene childC;
		CrossOverMethod(pA_idx, pB_idx, childC);

		r = (double)Rand(0, 1000000) / 1000000.0;
		if (r <= m_MutationProbability) Mutation(childC);

		GetFitness(childC);
		m_ChildPopulation.push_back(childC);
	}

	// bad & bad
	for (int i = 0; i < m_BadPopulation; i++) {
		int pA_idx = GetParentFromBad(-1);
		int pB_idx = GetParentFromBad(pA_idx);

		double r = (double)Rand(0, 1000000) / 1000000.0;
		if (r > m_CrossOverProbability) continue;
		Gene childC;
		CrossOverMethod(pA_idx, pB_idx, childC);

		r = (double)Rand(0, 1000000) / 1000000.0;
		if (r <= m_MutationProbability) Mutation(childC);

		GetFitness(childC);
		m_ChildPopulation.push_back(childC);
	}

	// bad & main
	for (int i = 0; i < m_Population.size() - m_BadPopulation; i++) {
		int pA_idx = GetParentFromMain(-1);
		int pB_idx = GetParentFromBad(-1);

		double r = (double)Rand(0, 1000000) / 1000000.0;
		if (r > m_CrossOverProbability) continue;
		Gene childC;
		CrossOverMethod(pA_idx, pB_idx, childC);

		r = (double)Rand(0, 1000000) / 1000000.0;
		if (r <= m_MutationProbability) Mutation(childC);

		GetFitness(childC);
		m_ChildPopulation.push_back(childC);
	}
}


void GASolver::CrossOverMethod(int pA_idx, int pB_idx, Gene& cC)
{	
	int l = Rand(0, m_Population[pA_idx].queensPos.size() - 2);
	int r = Rand(l + 1, m_Population[pA_idx].queensPos.size() - 1);

	for (int i = l; i <= r; i++) {
		int k = m_Population[pA_idx].queensPos[i];
		m_bUsedPos[k] = true;
	}

	int i = 0, j = 0;

	while (i < m_Board_Size) {
		int k = 0;
		if (l <= i && i <= r) {
			k = m_Population[pA_idx].queensPos[i];
			cC.queensPos.push_back(k);
		}
		else if (j < m_Board_Size) {
			do {
				k = m_Population[pB_idx].queensPos[j];
				++j;
			} while (j < m_Board_Size && m_bUsedPos[k]);

			if (!m_bUsedPos[k])
				cC.queensPos.push_back(k);
		}
		i++;
	}

	for (int i = l; i <= r; i++) {
		int k = m_Population[pA_idx].queensPos[i];
		m_bUsedPos[k] = false;
	}
}

void GASolver::Mutation(Gene &curGene)
{
	int i = Rand(0, curGene.queensPos.size() - 2);
	int j = Rand(i + 1, curGene.queensPos.size() - 1);
	std::swap(curGene.queensPos[i], curGene.queensPos[j]);
}

void GASolver::CreateNewGeneration()
{
	m_ResPopulation.clear();
	CrossOver();
	std::sort(m_Population.begin(), m_Population.end(), fitnessCMP);
	std::sort(m_ChildPopulation.begin(), m_ChildPopulation.end(), fitnessCMP);

	int i = m_Population.size() - 1;
	int j = m_ChildPopulation.size() - 1;
	while (i >= 0 || j >= 0) {
		if (i >= 0 && m_Population[i].fitness < m_ChildPopulation[j].fitness) {
			m_ResPopulation.push_back(m_Population[i]);
			i--;
		}
		else if (j >= 0) {
			m_ResPopulation.push_back(m_ChildPopulation[j]);
			j--;
		}
		if (m_ResPopulation.size() == m_Population_Size) break;
	}
	m_Population.clear();
	m_Population = m_ResPopulation;

}

bool GASolver::CheckStopCrt()
{
	if (m_Board_Size < 4 && m_Board_Size != 1) return true;
	if (genCnt == m_GenerationLimit) return true;
	for (int i = 0; i < m_Population.size(); i++)
		if (m_Population[i].fitness == 0) {
			m_Result = m_Population[i].queensPos;
			return true;
		}
	return false;
}

void GASolver::Run()
{	
	m_Result.clear();
	srand(time(NULL));
	genCnt++;
	CreateNewGeneration();
}

void GASolver::Reset()
{
	m_Population.clear();
	m_ChildPopulation.clear();
	m_Result.clear();
	
	GeneratePopulation(m_Population_Size);
	genCnt = 0;
}

std::vector<int> GASolver::GetResult()
{
	return m_Result;
	
}

GASolver::~GASolver()
{
}
