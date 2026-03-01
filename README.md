## MIRADAS Combinatorial Optimization (No-Wait CTSP1)

This repository contains a **heuristic combinatorial optimization solver** developed for **MIRADAS**, a robotic instrument at the **Gran Telescopio Canarias (GTC)**. The goal is to compute high-quality observation plans by **prioritizing and sequencing targets** under real operational constraints.

---

## Problem statement (high level)

The telescope instrument MIRADAS operates with **multiple robotic arms** that must repeatedly **reconfigure** to acquire and observe a sequence of astronomical targets. The planning problem is to build an observation plan where **each arm visits a sequence of targets**, while the instrument remains operationally feasible at every step.

In a typical plan, the instrument goes through a series of **configurations**:
- In each configuration, each arm is assigned to a target (or remains unused), enabling multi-object observations.
- Between consecutive configurations, arms must **move/reposition** to their next assigned targets.

The key challenge is that these arm routes are **coupled** by hard operational constraints, such as:
- **Kinematic feasibility** (each arm can only reach certain targets/configurations),
- **Collision/interaction constraints** between arms during a configuration and/or during transitions,
- **Multi-arm consistency** when a target requires coordinated acquisition (e.g., multiple arms must observe within a bounded temporal alignment).

### No-wait requirement
This project focuses on the **no-wait** setting: once a configuration is completed, the instrument proceeds immediately to the next one—**arms cannot “pause” or insert idle time** to resolve mismatches. Timing is therefore tightly determined by the chosen transitions, and a plan that would be feasible with waiting can become infeasible under no-wait operation.

Overall, the goal is to produce **feasible multi-arm target sequences** that maximize observation efficiency (e.g., reduce total repositioning time/cost while respecting target priorities), under these tightly constrained, synchronized dynamics.

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
