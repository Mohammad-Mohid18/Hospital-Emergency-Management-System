# 🏥 Hospital Emergency Management System  
**C++ | Data Structures | GUI-Ready**

---

## 📌 Project Overview

The **Hospital Emergency Management System** is a **team-based academic project** developed as part of an **Operating Systems / Data Structures course**.  
The system simulates a real-world **hospital emergency workflow**, focusing on efficient **patient record management**, **emergency prioritization**, and **scalable GUI integration**.

The project is designed in a **modular architecture**, where each core functionality is implemented as a separate module and later integrated into a **single GUI-based application**. This approach reflects real-world software engineering practices such as separation of concerns, modularity, and collaborative development.

---

## 🎯 Project Objectives

This project was created to:

- ✅ Simulate how hospitals manage emergency patients efficiently  
- ✅ Demonstrate practical usage of **data structures** in real-world scenarios  
- ✅ Improve understanding of **BSTs, Heaps, and priority-based scheduling**  
- ✅ Prepare a backend system that can be seamlessly integrated with a GUI  

### 🩺 Real-World Problem Solved
Hospitals must:
- Quickly identify **critical patients**
- Maintain **organized patient records**
- Retrieve and update patient data efficiently  

Using **Binary Search Trees** and **Min Heaps**, the system ensures:
- Fast patient lookup
- Automatic emergency prioritization
- Structured and scalable data handling

### 🔍 Why BST and Min Heap?
- **Binary Search Tree (BST)**  
  Ideal for managing patient records because it allows:
  - Fast searching by Patient ID
  - Sorted data access
  - Easy updates and deletions  

- **Min Heap (Priority Queue)**  
  Perfect for emergency handling because:
  - The most critical patient is always processed first
  - Insert and extract operations are efficient
  - Matches real emergency room workflows  

---

## 🛠️ Technologies Used

### 🔹 Language
- **C++**

### 🔹 Core Concepts
- Binary Search Tree (BST)
- Min Heap (Priority Queue)
- File Handling
- Modular Programming
- Object-Oriented Programming (OOP)
  - Classes
  - Encapsulation

### 🔹 GUI (Planned / In Progress)
- Dear ImGui
- OpenGL / GLFW *(optional, for rendering)*

---

## 🧱 Project Architecture

The system is divided into **three major modules**:

---

### 🔹 Module 1: Emergency Queue (Min Heap)

**Purpose:**  
Handles emergency patients based on severity level.

**Data Structure:**  
Min Heap (Priority Queue)

**Priority Levels:**
- `1` → Critical
- `2` → Urgent
- `3` → Standard

**Key Features:**
- Insert patient into emergency queue
- Extract next patient to be treated
- Peek next patient without removal
- GUI-friendly getters to display patient list

**Why Min Heap?**  
A Min Heap guarantees that the patient with the **highest urgency (lowest priority number)** is always processed first, making it ideal for emergency room management.

---

### 🔹 Module 2: Patient Records Management (Binary Search Tree)

**Purpose:**  
Manages long-term patient records efficiently.

**Data Structure:**  
Binary Search Tree (BST)

**BST Key:**  
Patient ID

**Core Features:**
- Insert patient record
- Search patient by ID
- Delete patient record
- Update patient information
- In-order traversal (sorted patient list)
- Filter patients by priority
- Tree height calculation
- Total node count statistics
- Visualize BST structure (proof of correctness)

**File Handling:**
- Save patient records to file
- Load patient records from file

This module ensures **fast access, structured storage, and scalability**.

---

### 🔹 Module 3: GUI Integration (In Progress)

**Goal:**  
Integrate Module 1 and Module 2 into a graphical interface.

**GUI Responsibilities:**
- Capture patient input
- Display patient tables
- Show next emergency patient
- Trigger heap and BST operations

**Design Principle:**  
The GUI is strictly separated from business logic, ensuring:
- Clean codebase
- Easy testing
- Future extensibility

---

## 📂 Folder Structure

