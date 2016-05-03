# EURunsplitter

This is a tool for splitting eudaq events into several smaller ones.


Compiling:
export EUDAQ=/path/to/my/eudaq/installation
make

Usage:
./runsplitter ./relative/path/to/data original_runnr begin_new_runnr [evtsPerRun = 500 000]
