#include "pch.h"
#include "GASolver.h"
#include <algorithm>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <vector>


GASolver::GASolver()
{
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
	Gene zero;
	for (int i = 0; i < m_Board_Size; i++)
		zero.queensPos.push_back(i);

	for (int i = 0; i < size; i++) {
		Gene newGene;
		newGene.queensPos = zero.queensPos;
		newGene.fitness = GetFitness(newGene);
		m_Population.push_back(newGene);

		std::random_shuffle(zero.queensPos.begin(), zero.queensPos.end());
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
	return a.fitness < b.fitness;
}

long long GASolver::Rand(long long a, long long b)
{
	return a + ((long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) * (RAND_MAX + 1) +
		(long long)rand() * (RAND_MAX + 1) * (RAND_MAX + 1) +
		(long long)rand() * (RAND_MAX + 1) +
		rand()) % (b - a + 1);
}

int GASolver::GetSelectionProbabilty(Gene &curGene)
{
	curGene.selectionProbability = (1.0 - (double)curGene.fitness / (double)m_SumFitness) / (double)(m_Population.size() - 1);
	return curGene.selectionProbability;
}


void GASolver::GenerateCumulativeDistribution()
{
	GetSumFitness();
	for (int i = 0; i < m_Population.size(); i++) {
		int k = GetSelectionProbabilty(m_Population[i]);
		if (i == 0) m_Population[i].cumulativeDistribution = k;
		else m_Population[i].cumulativeDistribution = m_Population[i - 1].cumulativeDistribution + k;
	}
}


GASolver::Gene GASolver::GetParentFromBad(double prev)
{
	Gene resGene;
	int idx = rand() % (m_BadPopulation + 1);
	while (m_Population[idx].cumulativeDistribution == prev) {
		idx = rand() % (m_BadPopulation + 1);
	}
	resGene = m_Population[idx];
	return resGene;
}


GASolver::Gene GASolver::GetParentFromMain(double prev)
{
	Gene resGene;
	double k;
	int l, r, mid;

	do {
		k = (double)rand() / (double)RAND_MAX;
		if (k < m_Population[m_BadPopulation].cumulativeDistribution)
			k += m_Population[m_BadPopulation].cumulativeDistribution;

		l = m_BadPopulation, r = m_Population.size();
		while (r - l > 1) {
			mid = (l + r) / 2;
			if (m_Population[mid].cumulativeDistribution >= k)
				l = mid;
			else r = mid;
		}
		resGene = m_Population[l];
	} while (resGene.cumulativeDistribution != prev);
	return resGene;
}

void GASolver::CrossOver()
{	
	m_ChildPopulation.clear();
	std::sort(m_Population.begin(), m_Population.end(), fitnessCMP);
	GenerateCumulativeDistribution();
	Gene parentA, parentB, childC;

	// main & main
	for (int i = 0; i < m_Population.size() - m_BadPopulation; i++) {
		parentA = GetParentFromMain(-1);
		parentB = GetParentFromMain(parentA.cumulativeDistribution);
		double r = (double) rand() / (double) RAND_MAX;
		if (r > m_CrossOverProbability) continue;
		Gene childC = CrossOverMethod(parentA, parentB);
		r = (double)rand() / (double)RAND_MAX;
		if (r <= m_MutationProbability) Mutation(childC);
		m_ChildPopulation.push_back(childC);
	}

	// bad & bad
	for (int i = 0; i < m_BadPopulation; i++) {
		parentA = GetParentFromBad(-1);
		parentB = GetParentFromBad(parentA.cumulativeDistribution);
		double r = (double)rand() / (double)RAND_MAX;
		if (r > m_CrossOverProbability) continue;
		Gene childC = CrossOverMethod(parentA, parentB);
		r = (double)rand() / (double)RAND_MAX;
		if (r <= m_MutationProbability) Mutation(childC);
		m_ChildPopulation.push_back(childC);
	}

	// bad & main
	for (int i = 0; i < m_Population.size() - m_BadPopulation; i++) {
		parentA = GetParentFromMain(-1);
		parentB = GetParentFromBad(-1);
		double r = (double)rand() / (double)RAND_MAX;
		if (r > m_CrossOverProbability) continue;
		Gene childC = CrossOverMethod(parentA, parentB);
		r = (double)rand() / (double)RAND_MAX;
		if (r <= m_MutationProbability) Mutation(childC);
		GetFitness(childC);
		m_ChildPopulation.push_back(childC);
	}
}


GASolver::Gene GASolver::CrossOverMethod(Gene pA, Gene pB)
{	
	Gene resGene;
	int l = Rand(0, pA.queensPos.size() - 3);
	int r = Rand(l + 2, pA.queensPos.size() - 1);
	bool flag[1000] = { 0 };
	for (int i = l; i <= r; i++)
		flag[pA.queensPos[i]] = true;
	for (int i = 0, j = 0; i < pB.queensPos.size(); i++) {
		if (l <= i || i <= r)
			resGene.queensPos.push_back(pA.queensPos[i]);
		else if (!flag[pB.queensPos[j]]) {
			resGene.queensPos.push_back(pB.queensPos[j]);
			j++;
		}
		else j++;
	}
	return resGene;
}

void GASolver::Mutation(Gene &curGene)
{
	int i = Rand(0, curGene.queensPos.size() - 1);
	int j = Rand(0, curGene.queensPos.size() - 1);
	while(j == i) 
		j = Rand(0, curGene.queensPos.size()- 1);
	std::swap(curGene.queensPos[i], curGene.queensPos[j]);
}

void GASolver::CreateNewGeneration()
{
	std::vector<Gene> resPopulation;
	CrossOver();
	std::sort(m_ChildPopulation.begin(), m_ChildPopulation.end(), fitnessCMP);
	int i = m_Population.size() - 1;
	int j = m_ChildPopulation.size() - 1;
	while (i >= 0 && j >= 0) {
		if (m_Population[i].fitness > m_ChildPopulation[j].fitness) {
			resPopulation.push_back(m_Population[i]);
			i--;
		}
		else {
			resPopulation.push_back(m_ChildPopulation[j]);
			j--;
		}
		if (resPopulation.size() == m_Population.size()) break;
	}
	m_Population = resPopulation;
}

bool GASolver::CheckStopCrt()
{
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
	GeneratePopulation(1000);
	int genCnt = 0;
	while (!CheckStopCrt() && genCnt <= m_GenerationLimit) {
		CreateNewGeneration();
		genCnt++;
	}
}

void GASolver::Reset()
{
	m_Population.clear();
	m_ChildPopulation.clear();
	m_Result.clear();
}

std::vector<int> GASolver::GetResult()
{
	return m_Result;
}

GASolver::~GASolver()
{
}
