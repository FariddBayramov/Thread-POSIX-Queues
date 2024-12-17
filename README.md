#  Thread-POSIX Queues Project

Welcome to the **Thread-POSIX Queues Project**! This project implements multiple approaches to find the **k largest numbers** using **Fork**, **POSIX message queues**, and **POSIX threads**. It is built using the C programming language and provides experimental comparisons documented in the included report.

---

## 🚀 Features

### 1. **Find Top-k Using Fork**
- A program that finds the **k largest numbers** using multiple processes created via **Fork**.
- Executes in parallel to improve performance.

### 2. **Find Top-k Using POSIX Message Queues**
- Implements **POSIX message queues** for inter-process communication to determine the top-k largest numbers.
- Ensures efficient message handling and data sharing.

### 3. **Find Top-k Using POSIX Threads**
- Utilizes **POSIX threads** to achieve multi-threaded computation for finding the k largest numbers.
- Designed for efficient CPU utilization and concurrency.

### 4. **Comprehensive Report**
- Includes a **report.pdf** file documenting experimental results, comparisons, and performance analysis for the different methods.

---

## 🛠️ Technologies Used

- **C Language** - Core programming language.
- **POSIX Threads** - For multi-threaded programming.
- **POSIX Message Queues** - For inter-process communication.
- **Fork System Call** - For process creation.
- **Makefile** - For project compilation and executable generation.

---

## 🎮 How to Compile and Run the Project

1. Clone the repository to your local machine:
   ```bash
   git clone https://github.com/FariddBayramov/thread-posix-queues.git
   ```

2. Navigate to the project directory:
   ```bash
   cd thread-posix-queues
   ```

3. Compile the project:
   ```bash
   make
   ```

4. Run the programs:

- **Using Fork:**
   ```bash
   ./findtopk <k> <N> <infile1> ...<infileN> <outfile>
   ```
- **Using POSIX Message Queues:**
   ```bash
   ./findtopk_mqueue <k> <N> <infile1> ...<infileN> <outfile>
   ```
- **Using POSIX Threads:**
   ```bash
   ./findtopk_thread <k> <N> <infile1> ...<infileN> <outfile>
   ```

**Parameters:**
- `<k>`: Number of largest elements to find.
- `<N>`: Number of input files.
- `<infile1> ... <infileN>`: Input file names containing numbers.
- `<outfile>`: Output file name to store the results.

---

## 📂 Project Structure

```plaintext
Thread-POSIX-Queues/
│
├── Makefile                       # Compilation instructions for all programs
├── findtopk.c                     # Program to find top-k numbers using Fork
├── findtopk_mqueue.c              # Program to find top-k using POSIX message queues
├── findtopk_thread.c              # Program to find top-k using POSIX threads
├── report.pdf                     # Report with experimental results
└── README.md                      # Project documentation
```

---

## 📊 Notes

- Ensure you have a **C language compiler** (e.g., `gcc`) installed.
- For detailed experimental results and analysis, refer to the **report.pdf** file.
- Programs are designed to run efficiently with multi-processing and multi-threading techniques.

---

## 🧑‍💻 Author

- **FaridBayramov** 
- GitHub: [FariddBayramov](https://github.com/FariddBayramov)

---

Thank you for exploring the **Thread-POSIX Queues Project**! 🧵✨ Compare and analyze multi-threaded and multi-process solutions seamlessly.

