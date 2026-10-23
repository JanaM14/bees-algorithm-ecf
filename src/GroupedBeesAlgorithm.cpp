#include <ECF/ECF.h>
#include "ECF/ECF_base.h"
#include "GroupedBeesAlgorithm.h"

void GroupedBeesAlgorithm::registerParameters(StateP state)
{
    registerParameter(state, "nGroups", (voidP)new uint(2), ECF::INT, "Number of groups (default: 2)");
    registerParameter(state, "R_ngh", (voidP)new double(0.1), ECF::DOUBLE, "Radius (default: 0.1)");

}

bool GroupedBeesAlgorithm::initialize(StateP state)
{

    GenotypeP activeGenotype = state->getGenotypes()[0];

    voidP sptr = state->getRegistry()->getEntry("population.size");
    uint size = *((uint*)sptr.get());

    voidP lBound = state->getGenotypes()[0]->getParameterValue(state, "lbound");
    lbound_ = *((double*)lBound.get());
    voidP uBound = state->getGenotypes()[0]->getParameterValue(state, "ubound");
    ubound_ = *((double*)uBound.get());


    voidP dimension = state->getGenotypes()[0]->getParameterValue(state, "dimension");
    dimension_ = *((uint*)dimension.get());


    voidP nGroupsP = getParameterValue(state, "nGroups");
    nGroups_ = *((uint*)nGroupsP.get());

    voidP R_nghP = getParameterValue(state, "R_ngh");
    R_ngh_ = *((double*)R_nghP.get());


    initializeParameters();

    return true;
}

void GroupedBeesAlgorithm::initializeParameters() {
    double k = 3.0 * nGroups_ / ((nGroups_ + 1) * (nGroups_ + 1) * (nGroups_ + 1) - 1);

    groups.resize(nGroups_);
    recruited_bees.resize(nGroups_);
    ngh.resize(nGroups_);
    std::vector<double> a(nGroups_);
    std::vector<double> b(nGroups_);

    for (int i = 0; i < nGroups_; ++i) {
        a[i] = ((ubound_ - lbound_) / 2.0 - R_ngh_) / ((nGroups_ + 1) * (nGroups_ + 1) - 1);
        b[i] = R_ngh_ - a[i];
    }

    for (int i = 0; i < nGroups_; ++i) {
        groups[i] = floor(k * (i + 1) * (i + 1));
        if (groups[i] == 0) {
            groups[i] = 1;
        }
        recruited_bees[i] = (nGroups_ + 1 - i) * (nGroups_ + 1 - i);
        ngh[i] = a[i] * (i + 1) * (i + 1) + b[i];
    }
}

std::vector<double> GroupedBeesAlgorithm::generateRandomSolution()
{
    std::vector<double> solution(dimension_);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(lbound_, ubound_);

    for (int i = 0; i < dimension_; ++i) {
        solution[i] = dis(gen);
    }

    return solution;
}

std::vector<double> GroupedBeesAlgorithm::beeWaggleDance(const std::vector<double>& solution, double ngh)
{
    std::vector<double> newSolution(dimension_);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < dimension_; ++i) {
        newSolution[i] = (solution[i] - ngh) + 2 * ngh * dis(gen);
        if (newSolution[i] < lbound_) newSolution[i] = lbound_;
        if (newSolution[i] > ubound_) newSolution[i] = ubound_;
    }

    return newSolution;
}


bool GroupedBeesAlgorithm::advanceGeneration(StateP state, DemeP deme)
{
    for (auto& ind : *deme) {
        evaluate(ind);
    }

    std::sort(deme->begin(), deme->end(), [](IndividualP a, IndividualP b) {
        return a->fitness->getValue() < b->fitness->getValue();
        });



    int beeIndex = 0;

    for (int group = 0; group < nGroups_; ++group) {
        for (int j = 0; j < groups[group]; ++j) {
            for (int i = 0; i < recruited_bees[group]; ++i) {
                FloatingPointP genotype = std::static_pointer_cast<FloatingPoint::FloatingPoint>(deme->at(beeIndex)->getGenotype(0));
                std::vector<double> solution(genotype->realValue.begin(), genotype->realValue.begin() + dimension_);
                std::vector<double> newSolution = beeWaggleDance(solution, ngh[group]);

                for (int k = 0; k < dimension_; ++k) {
                    genotype->realValue[k] = newSolution[k];
                }

                evaluate(deme->at(beeIndex));
                
            }
            beeIndex++;
        }
    }

    for (int i = beeIndex; i < deme->size(); ++i) {
        std::vector<double> solution = generateRandomSolution();
        FloatingPointP genotype = std::static_pointer_cast<FloatingPoint::FloatingPoint>(deme->at(i)->getGenotype(0));

        for (int k = 0; k < dimension_; ++k) {
            genotype->realValue[k] = solution[k];
        }

        evaluate(deme->at(i));
    }


    std::sort(deme->begin(), deme->end(), [](IndividualP a, IndividualP b) {
        return a->fitness->getValue() < b->fitness->getValue();
        });


    return true;
}