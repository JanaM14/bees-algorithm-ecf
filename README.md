# Bees Algorithm and Grouped Bees Algorithm (ECF)

C++ implementation of the **Bees Algorithm (BA)** and the **Grouped Bees Algorithm (GBA)** within the Evolutionary Computation Framework (ECF), benchmarked on standard optimization test functions and compared with established algorithms.

Developed as my bachelor's thesis at the Faculty of Electrical Engineering and Computing (FER), University of Zagreb, February – July 2024.

**Thesis (PDF, in Croatian):** https://1drv.ms/b/c/edf75f83efd29991/IQCRmdLvg1_3IIDtNwsPAAAAAfzhFQ41G2BUa5jVIu1o-10?e=GIKAP5

## Overview

The Bees Algorithm is a swarm-based optimization method inspired by how honey bees forage for nectar. The Grouped Bees Algorithm extends it by dividing the bees into groups that search neighbourhoods of different radii, which lets the search move gradually from focused local search towards random search.

This project covers:

- Implementation of BA and GBA as ECF algorithms in C++
- A parameter study of GBA (neighbourhood radius `R_ngh` and number of groups `nGroups`)
- A comparison with Steady State Tournament, OptIA and Clonalg

## Experiments

All experiments use a population of 500 individuals, 5000 generations and 10 independent runs per setting.

**1. Parameter tuning (genotype dimension 3)**

| Test function | Algorithm | What was tested |
|---|---|---|
| Rosenbrock | BA, GBA | Genotype dimension 3 and 5; `R_ngh` ∈ {0.001, 0.01, 0.1, 0.2, 0.3, 0.5}; `nGroups` ∈ {2, 3, 5, 8, 10, 15} |
| Rastrigin | BA, GBA | Same parameter ranges |

Best values found: `R_ngh = 0.1` for both functions, `nGroups = 2` for Rosenbrock and `nGroups = 3` for Rastrigin.

**2. Comparison with other algorithms (dimension 10, bounds [-50, 50])**

Schaffer F6, Ackley and Schaffer F7, compared against Steady State Tournament, OptIA and Clonalg (all from ECF).

## Key results

- With tuned parameters, GBA converges stably in low dimensions (3) on both Rosenbrock and Rastrigin.
- Smaller numbers of groups worked better than larger ones.
- In 10 dimensions, both bee algorithms were clearly outperformed by the established algorithms, and GBA performed worse than BA.
- Conclusion: both algorithms are usable for low-dimensional problems, but would need structural improvements for higher dimensions.



## Requirements

- C++ compiler with C++[11/14/17] support
- [ECF – Evolutionary Computation Framework](link-to-ECF) [version]
- [CMake / Make, version]
- [Python 3 with matplotlib, if you used it for the plots]


## References

- D. T. Pham et al., *The Bees Algorithm – A Novel Tool for Complex Optimisation Problems*, Cardiff University.
- H. R. Nasrinpour, A. M. Bavani, M. Teshnehlab, *Grouped Bees Algorithm: A Grouped Version of the Bees Algorithm*, 201
