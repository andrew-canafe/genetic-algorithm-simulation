# Genetic-Algorithm-Simulation

## Details
- Runs on Linux
- Runs on Windows, but further testing needed for bugs
- Written in C++

## Sample output


## How it works
A Genetic Algorithm is an evolutionary algorithm that works similarly to the process of natural selection, using fitness values to determine which of the current offspring are suitable for giving birth the next generation. Individuals within a population are described by chromosomes, and chromosomes are further described by genes. To give birth to the next generation, the fittest two parents are paired together and cross-shuffle their genes for each particular chromosome. The process is typically repeated until 50% of the population with the lowest fitness values can be perfectly replaced by the newly created children. Alongside the newly created children, 50% of the population with the highest fitness values will remain in the pool for the next generation. Lastly, mutations must be introduced every once in a while when shuffling genes in order to escape the local minima whereby the chromosomes of a population become too identical to each other. During a mutation for a particular chromosome, an arbitrarily selected gene is modified to a new random value within its possible domains. In this simulation, I have set the mutation rate to 5% with respect to each cross-shuffle between two chromosomes.

## How to compile on Linux
g++ -g gar.cpp -o gar -std=c++11
