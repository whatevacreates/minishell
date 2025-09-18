# 🐚 Minishell  

A **42 Lausanne** project – building our own minimal shell inspired by **bash**.  
We aimed for robustness, and our implementation proved **unbreakable** – earning us a **distinction (125%)** at 42 Lausanne.  

---

## 📖 About the Project  

This project was developed in collaboration with **Bei** (42 Lausanne).  
Our goal was to implement a fully functional shell that mimics the behavior of **bash**, handling parsing, execution, and builtins.  

Key features:  
- Command parsing & execution  
- Built-in commands (cd, echo, export, unset, env, exit, …)  
- Chained Redirections (`>`, `<`, `>>`, `<<`)  
- Chained Pipes (`|`)  
- Environment variable expansion  
- Signal handling (Ctrl-C, Ctrl-\ etc.)  
- Error handling for invalid commands and edge cases
- Handling wildcards 
---

🏗️ Architecture

Our minishell is built on a tree traversal architecture:

Parsing – the input command line is parsed into tokens.

Tree Building – tokens are structured into a syntax tree.

Execution – the tree is traversed and executed node by node, ensuring proper handling of pipes, redirections, and command order.

This design made the project clean, modular, and robust — a core reason our minishell proved unbreakable.

## ⚙️ How to Start  

Clone the repository:  
```bash
git clone https://github.com/whatevacreates/minishell.git
cd minishell

make        # build minishell
make clean  # remove objects
make fclean # full clean (objects + binary)
make re     # rebuild

./minishell

🧪 Tests

Try the following commands inside ./minishell:

Basics
echo hello world
pwd
cd ..
pwd

Environment & Variables
export TEST=42
echo $TEST
unset TEST
echo $TEST

Redirections
echo "Hello 42" > file.txt
cat < file.txt
echo "Append this" >> file.txt
cat file.txt

Pipes
ls -la | grep minishell
echo "one two three" | wc -w

Combined
cat < file.txt | grep Hello | wc -l

Signals

Ctrl+C → should display a new prompt without quitting.

Ctrl+D → should exit minishell gracefully.

Ctrl+\ → should be ignored.

Error Handling
cd nonexistent_directory
ls | nonexistent_command
echo $UNDEFINED

🏆 Achievements

Unbreakable implementation

Distinction (125%) at 42 Lausanne

Fully mimics bash for project requirements

👩‍💻 Maintainers:

Eva Przybyla

Bei (42 Lausanne)