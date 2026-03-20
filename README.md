# sql_engine
Lightweight SQL Engine for managing a database, similar to SQLite. 

## Features:
- Interactive menu for managing tables and executing queries
- Standard SQL queries (SELECT, INSERT, etc.)
- Import data from CSV files
- Table indexing for faster searches
- Help menu
- Execute commands from '.txt' files
- Data persistence for tables and their indexes

## Build & Run
```bash
cmake -S . -B build 
cmake --build build
./bin/sql_engine
```
