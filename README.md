# GramResolve-Grammar-Analyzer-Resolver# 📘 Grammar Analyzer & Conflict Detection Tool  
### *Course Project – Compiler Construction*  
### *Academic Year: 2024–25*

---

## 👥 Team Members

| Name | Roll Number |
|------|-------------|
| **Shivpratap Singh** | 2023UCP1655 |
| **Varsh Bansal** | 2023UCP1585 |

---

## 📌 Project Overview

This project implements a complete **Context-Free Grammar Analysis and Parsing Toolkit**, capable of:

- Computing **FIRST** and **FOLLOW** sets  
- Building **LL(1) Parsing Tables**  
- Performing **LL(1) predictive parsing**  
- Constructing **LR(0) item sets**  
- Building **SLR(0) Action/GOTO tables**  
- Simulating **LR parsing with stack tracing**  
- Detecting **LL(1), LR(0), and SLR parsing conflicts**  

The project is designed to provide deep insight into grammar behavior and parsing table construction, helping identify ambiguities and structural issues in grammars.

---

## 📁 Project Structure


```bash

📦 GramResolve
│
├── 📁 src
│   ├── 📁 grammar
│   │    ├── Grammar.h / Grammar.cpp
│   │    └── Production.h / Production.cpp
│   │
│   ├── 📁 analysis
│   │    ├── FirstFollow.h / FirstFollow.cpp
│   │    └── conflictDetector.h / conflictDetector.cpp
│   │
│   ├── 📁 parser
│   │    ├── LL1Parser.h / LL1Parser.cpp
│   │    └── LR0Parser.h / LR0Parser.cpp
│   │
│   ├── 📁 resolver
│   │    └── conflictResolver.h / conflictResolver.cpp
│   │
│   ├── 📁 visualizer
│   │    └── (future UI / report generation modules)
│   │
│   └── main.cpp   ← Entry point of the project
│
├── 📁 data
│   └── sample_grammar.txt   ← Input grammar file
│
├── 📁 tests
│   └── additional grammars & inputs
│
├── Makefile
└── README.md


```

---

## 🧠 System Features

### ✔ FIRST & FOLLOW Computation  
The system parses input CFG and generates FIRST and FOLLOW sets for all non-terminals.

---

### ✔ LL(1) Parsing Table Construction  
Automatically generates the LL(1) table and reports:

- Multiple-entry conflicts  
- Missing entries  
- Ambiguous productions  

---

### ✔ LL(1) Parsing Simulation  
Performs predictive parsing with live trace of:

- Stack content  
- Input pointer  
- Applied productions  

---

### ✔ LR(0) Automaton Construction  
Builds:

- Canonical LR(0) Item Sets  
- GOTO transitions  
- SLR-based ACTION/GOTO tables  

---

### ✔ LR Parsing Simulation  
Displays every step:

- State stack  
- Symbol stack  
- Remaining input  
- Shift/Reduce operations  

---

### ✔ Conflict Detection Engine  
The system automatically detects:

| Conflict Type | Description |
|---------------|-------------|
| **LL(1) Multiple Entry** | More than one production in a table cell |
| **Shift/Reduce Conflict** | LR parser has both shift and reduce action |
| **Reduce/Reduce Conflict** | Multiple reductions possible |
| **SLR Follow-set conflicts** | FOLLOW-based collisions in reduce placement |

All conflicts are summarized and logged.

---


### ✔ Conflict Resolution Module  
The system provides detailed explanations and suggestions for every detected conflict, including:

| Conflict Type | Resolution Approach |
|---------------|---------------------|
| **LL(1) Multiple Entry** | Explains FIRST/FOLLOW overlap and suggests left factoring or removing ε-ambiguity |
| **Shift/Reduce Conflict** | Identifies ambiguous grammar patterns and suggests precedence/associativity rules or grammar refactoring |
| **Reduce/Reduce Conflict** | Highlights conflicting reductions and suggests restructuring or splitting productions |
| **SLR Follow-set Conflicts** | Points out FOLLOW-set collisions and recommends grammar transformation to avoid identical lookaheads |

Each conflict is accompanied by:
- Cause of the conflict  
- Involved productions/actions  
- Suggested fix or grammar rewrite direction  
- Notes on whether the grammar can ever be made LL(1) or LR(0)

All resolutions are displayed in a clean, explanatory format.


## ▶️ How to Compile & Run

### **Using Makefile**

The project includes a Makefile for easy building and execution.

#### **🔧 Build the Project**

```bash
# Build the project
make
```
#### **🚀 Run the Executable**

```bash
# Build the project
make run
```

#### **🧹 Clean Build Files**

```bash
# Build the project
make clean
```



## 📝 Sample Output (Abbreviated)

- FIRST/FOLLOW Tables  
- LL(1) Table  
- LR(0) Item Sets  
- ACTION/GOTO Tables  
- Parsing Trace  
- Conflict Report  

The full detailed output is generated inside:


