## MIRADAS Combinatorial Optimization (No-Wait CTSP1)

This repository contains a **heuristic combinatorial optimization solver** developed for **MIRADAS**, a robotic instrument at the **Gran Telescopio Canarias (GTC)**. The goal is to compute high-quality observation plans by **prioritizing and sequencing targets** under real operational constraints.

---

## Problem statement (high level)

Given a set of candidate astronomical targets and a set of feasibility constraints between consecutive acquisitions/observations, we want to produce an **ordered plan** that:

- Respects instrument/operational constraints between consecutive targets, and  
- Optimizes an objective related to **target prioritization** (e.g., maximizing total priority / minimizing cost).

In practice, the difficulty comes from the fact that not all transitions between targets are feasible, and feasibility depends on the instrument’s physical and operational limitations.

---

## Modeling: No-Wait CTSP1

The problem is **modeled and formalized as a variant of the Consistent Traveling Salesman Problem (CTSP1) with a _no-wait_ constraint**.

- **CTSP1 (Consistent TSP)** captures the idea that the tour is not only about distances/costs: it must also satisfy **consistency/feasibility conditions** between consecutive decisions.
- The **no-wait** setting enforces that **no idle time is allowed** between consecutive operations.  
  This greatly increases difficulty because it reduces flexibility: even if two targets are individually feasible, their **transition** may be infeasible when waiting is not permitted.

As a result, the search space becomes highly constrained, and exact methods quickly become impractical for large instances.

---

## Solution approach

To handle the computational complexity, the repository provides a **heuristic solver** based on:

- **Constructive heuristics** to build initial feasible solutions  
- **Local search** to iteratively improve solution quality  
- A focus on **practical performance** and scalability for large problem sizes

The implementation is written in **C++**, designed to be fast and easy to benchmark.

---

## Experimental evaluation

The solver is evaluated through an extensive benchmark on **756 TSPLIB instances**, including an **exhaustive analysis of results**, such as:

- Solution quality across different instance families and sizes    
- Runtime/performance behavior

These experiments are intended to provide a clear picture of when the heuristics work best and how performance scales with problem difficulty.

---
