#include <ECF/ECF.h>
#include "ECF/ECF_base.h"
#include "BeesAlgorithm.h"
#include <random>
#include <numeric>

void BeesAlgorithm::registerParameters(StateP state)
{
    registerParameter(state, "n", (voidP)new uint(100), ECF::INT, "Number of bees (default: 100)");
}

bool BeesAlgorithm::initialize(StateP state)
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

    voidP nP = getParameterValue(state, "n");
    n_ = *((uint*)nP.get());

    return true;
}

bool BeesAlgorithm::advanceGeneration(StateP state, DemeP deme)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(lbound_, ubound_);

    IndividualP bestIndividual = *std::min_element(deme->begin(), deme->end(),
        [](const IndividualP& a, const IndividualP& b) {
            return a->fitness->getValue() < b->fitness->getValue();
        });

    DemeP newDeme = std::make_shared<Deme>(*deme);

    newDeme->at(0) = bestIndividual;

    for (uint i = 1; i < deme->getSize(); i++) {
        IndividualP individual = deme->at(i);
        FloatingPointP genotype = std::static_pointer_cast<FloatingPoint::FloatingPoint>(individual->getGenotype(0));
        FloatingPointP bestGenotype = std::static_pointer_cast<FloatingPoint::FloatingPoint>(bestIndividual->getGenotype(0));

        for (uint j = 0; j < dimension_; ++j) {
            double newSolution = bestGenotype->realValue[j] + (dis(gen) - 0.5) * 0.1;

            if (newSolution < lbound_)
                genotype->realValue[j] = lbound_;
            else if (newSolution > ubound_)
                genotype->realValue[j] = ubound_;
            else
                genotype->realValue[j] = newSolution;
        }

        evaluate(individual);
    }

    *deme = *newDeme;

    return true;
}

typedef std::shared_ptr<BeesAlgorithm> BeesAlgorithmP;
