# Finding Network Motifs in an Email Network

**Course:** CS1005 – Discrete Structures  
**Semester:** Fall 2026  
**Section:** B  
**Group parameter:** R = 5298  
**Selected departments:** Department 16 and Department 20

## Project overview

This project analyzes directed email communication in the Email-Eu-core network. It loads and cleans the network, ranks departments by their internal directed edges, selects a department pair using the group parameter, constructs the corresponding induced subgraphs, counts the 13 connected directed triad classes, generates degree-preserving randomized networks, calculates Z-scores, and exports results for visualization.

## Group parameters

| Parameter | Value |
|---|---:|
| Roll-number suffixes used for R | 0847, 2225, 2226 |
| R | 5298 |
| Section | B |
| Department-pair index (`R % 190`) | 168 |
| Department A | 16 |
| Department B | 20 |
| Randomized networks per department (`20 + R % 31`) | 48 |
| Swap multiplier (`5 + R % 6`) | 5 |
| Random seed | 5298 |
| Department 16 swap attempts per network | 1,180 |
| Department 20 swap attempts per network | 350 |

## Dataset and expected files

Place the two extracted text files in the `data` directory:

```text
data/
    email-Eu-core.txt
    email-Eu-core-department-labels.txt
```

The edge file contains directed source-target pairs. The department-label file maps each node to a department. The program removes self-loops and duplicate directed edges and uses the node range represented by the dataset.

## Project structure

```text
email-network-motifs-cpp/
├── data/
│   ├── email-Eu-core.txt
│   └── email-Eu-core-department-labels.txt
├── results/
│   ├── z_scores.csv
│   ├── triad_z_scores.png
│   └── triad_z_scores.pdf
├── graph.cpp
├── graph.h
├── triads.cpp
├── triads.h
├── randomization.cpp
├── randomization.h
├── statistics.cpp
├── statistics.h
├── main.cpp
├── plot_results.py
└── README.md
```

## Build and run

Use a C++17 compiler. From the project root, run:

```powershell
g++ -std=c++17 main.cpp graph.cpp triads.cpp randomization.cpp statistics.cpp -o project.exe
.\project.exe
```

The C++ program creates the `results` directory if it does not exist and writes `results/z_scores.csv`.

Install the Python plotting dependencies:

```powershell
python -m pip install pandas matplotlib numpy
```

Generate the chart:

```powershell
python plot_results.py
```

The script exports a 300-DPI PNG and a PDF to the `results` directory. Run the updated C++ executable before plotting so that the CSV contains the department IDs and group parameter used by the chart.

## Methodology

### 1. Network preparation

The program reads the directed edge list, removes self-loops, removes duplicate directed edges, and loads department labels. Department rankings are based on the number of directed edges whose endpoints belong to the same department.

### 2. Department selection and induced subgraphs

The 190 possible pairs among the top 20 ranked departments are indexed. The selected pair is determined by `R % 190`. For this group, the selected pair is Department 16 and Department 20. Each department subgraph contains its department members and all original directed edges whose endpoints both belong to that department.

### 3. Directed triad classification and census

Each three-node directed configuration is converted to a six-bit code. The code is canonicalized over all six permutations of the three node labels. The canonical codes are mapped to the 13 connected triad classes in the project implementation:

`3, 6, 7, 10, 11, 15, 21, 23, 25, 27, 30, 31, 63`.

The census counts each unordered set of three nodes once and records its connected triad class.

### 4. Degree-preserving randomization

The randomization procedure selects two directed edges `(a,b)` and `(c,d)` and proposes the directed double-edge swap `(a,d)` and `(c,b)`. A swap is rejected if it creates a self-loop or an edge that already exists. The procedure checks that the number of edges is unchanged, that there are no duplicate edges or self-loops, and that every represented node retains its in-degree and out-degree. Assertions and runtime checks are included in the randomization implementation.

Each department receives 48 randomized networks. The number of swap attempts per network is five times the department's original edge count. Random seeds are derived reproducibly from R and the network index.

### 5. Statistical analysis

For each triad class, the program calculates the mean and population standard deviation across randomized networks. The Z-score is:

\[
Z = \frac{O - \mu}{\sigma}
\]

where `O` is the observed triad count, `μ` is the randomized mean, and `σ` is the randomized population standard deviation. When the standard deviation is zero, the project rule is applied and the reported Z-score is zero.

The chart includes reference lines at +2 and -2. These are used as conventional reference thresholds for interpreting unusually high or low counts; they should not be presented as a formal hypothesis test or proof of statistical significance.

## Results currently obtained

The latest recorded program output gives the following department-level totals:

| Measure | Department 16 | Department 20 |
|---|---:|---:|
| Nodes | 25 | 14 |
| Directed edges | 236 | 70 |
| Connected triads counted | 1,018 | 131 |
| Randomized networks | 48 | 48 |
| Swap attempts per randomized network | 1,180 | 350 |

The latest recorded Z-score output is:

| Triad class | Department 16 Z | Department 20 Z |
|---:|---:|---:|
| 1 | -1.41 | -0.24 |
| 2 | -0.71 | -1.61 |
| 3 | 1.19 | -1.65 |
| 4 | 0.13 | -1.04 |
| 5 | -0.32 | -0.39 |
| 6 | -1.15 | -1.01 |
| 7 | -0.71 | -1.28 |
| 8 | 0.23 | 1.95 |
| 9 | -1.26 | -0.55 |
| 10 | -0.80 | -1.40 |
| 11 | 0.62 | -1.48 |
| 12 | -0.64 | -1.23 |
| 13 | 1.43 | 1.84 |

### Interpretation

Department 16 has more connected triads in absolute terms than Department 20. This is consistent with Department 16 having more members and more internal directed edges in the extracted subgraph. Raw triad totals alone do not establish that one department communicates more efficiently or has stronger collaboration; the departments differ in size and edge count.

The Z-scores compare each department's observed triad count with the distribution produced by its own randomized networks. In the recorded results, none of the 26 Z-scores exceeds +2 or falls below -2. Department 20's Class 8 Z-score (1.95) is close to, but below, the +2 reference threshold. Its Class 13 Z-score is 1.84. These values may be described as relatively high compared with their randomized means, but they do not cross the chosen reference threshold.

The results therefore do not show a triad class crossing the project's ±2 screening thresholds in either department. The analysis is conditional on the selected departments, the randomization procedure, the number of randomized networks, and the swap settings. A finite number of attempted swaps does not by itself prove that the randomization chains have fully mixed or that all generated networks are independent.

**Reproducibility note:** The table above reflects the previously recorded run. Re-run the updated executable to regenerate the CSV and chart after applying the latest code changes. If any output changes, update this section using the new CSV rather than retaining stale values.

## Group members and contributions

The roll-number suffixes used to calculate R are recorded below. Add each member's full name and confirm their actual contribution before submission; names and individual contributions are not inferred here.

| Member | Roll-number suffix | Contribution |
|---|---:|---|
| Member 1 — add full name | 0847 | Confirm and describe actual contribution |
| Member 2 — add full name | 2225 | Confirm and describe actual contribution |
| Member 3 — add full name | 2226 | Confirm and describe actual contribution |

## Limitations

- The analysis covers the two departments selected by the group-parameter rule, not every department in the full network.
- Z-scores depend on the randomized-network model and its settings.
- The ±2 lines are reference thresholds and are not a substitute for a formal inferential procedure.
- The randomization checks verify degree preservation and graph validity, but do not establish that the swap chain has mixed sufficiently.
- Raw triad totals should be interpreted in light of department size and edge count.

## Final verification checklist

- [ ] Confirm the data files are in `data/` and are the intended dataset files.
- [ ] Compile all five C++ source files using C++17.
- [ ] Run the executable and confirm the complete output is produced.
- [ ] Confirm the randomization assertions and runtime checks pass.
- [ ] Confirm `results/z_scores.csv` is regenerated.
- [ ] Run `plot_results.py` and inspect both chart exports.
- [ ] Replace the group-member name placeholders and verify contribution descriptions.
- [ ] Check that the README's result tables match the final CSV.
- [ ] Commit the final source code, README and required result files to GitHub.
